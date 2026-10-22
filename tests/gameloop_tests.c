#include "catch.hpp"
#include <cstring>
#include <memory>
#include "../code/comm.h"
#include "../code/db.h"
#include "../code/entity/descriptor_data.h"
#include "../code/entity/pc_data.h"
#include "../code/gameloop.h"
#include "../code/merc.h"
#include "../code/recycle.h"
#include "world_fixture.h"

// ProcessConnectionInput runs one command for one connection, and the command
// it runs can close that connection. close_socket erases the descriptor from
// descriptor_list, which destroys it, so nothing after the dispatch may touch
// the pointer it was handed.
//
// The scenarios below only check that the connection is gone afterwards. The
// use-after-free itself is a write into freed memory, which a plain build does
// not notice, so these tests catch a regression in the sanitizer build, where
// AddressSanitizer reports it. That build runs in CI.

/// Puts a connection on descriptor_list for the life of a scenario, which is
/// what init_descriptor does for a real one. close_socket only destroys
/// connections that are linked, so a test that wants the destruction has to
/// link it.
///
/// Declare it after the TestWorld, so it goes first. If the connection is still
/// open when the scenario ends -- which is what a regression looks like -- the
/// list gives it up rather than destroying it, so that whoever built it frees
/// it exactly once: the world for a player's connection, this for a bare one.
class ListedConnection
{
public:
	ListedConnection(DESCRIPTOR_DATA *d, bool ownedByWorld)
		: connection(d->self), ownedByWorld(ownedByWorld)
	{
		descriptor_list.push_back(std::unique_ptr<DESCRIPTOR_DATA>(d));
		d->globalNode = std::prev(descriptor_list.end());
	}

	~ListedConnection()
	{
		DESCRIPTOR_DATA *d = Deref(connection);

		if (d == nullptr)
			return;

		d->globalNode->release();
		descriptor_list.erase(d->globalNode);
		d->globalNode = {};

		if (!ownedByWorld)
			free_descriptor(d);
	}

	ListedConnection(const ListedConnection &) = delete;
	ListedConnection &operator=(const ListedConnection &) = delete;

private:
	Handle<DESCRIPTOR_DATA> connection;
	bool ownedByWorld;
};

/// Registers a room under a vnum for the life of a scenario, so get_room_index
/// finds it. The fixture's rooms are not in room_index_hash, and stop_idling
/// asks for limbo by number.
class RegisteredRoom
{
public:
	RegisteredRoom(ROOM_INDEX_DATA *room, int vnum)
		: room(room), slot(room_index_hash[vnum % MAX_KEY_HASH])
	{
		room->vnum = vnum;
		room->next = slot;
		slot = room;
	}

	~RegisteredRoom() { slot = room->next; }

	RegisteredRoom(const RegisteredRoom &) = delete;
	RegisteredRoom &operator=(const RegisteredRoom &) = delete;

private:
	ROOM_INDEX_DATA *room;
	ROOM_INDEX_DATA *&slot;
};

/// Fills the output buffer to its ceiling, so the next write of any length
/// overflows it and write_to_buffer closes the connection. The buffer really
/// is that size, so nothing reading up to outtop strays out of it.
static void FillOutputToCeiling(DESCRIPTOR_DATA *d)
{
	delete[] d->outbuf;

	d->outsize = 32000;
	d->outbuf = new char[d->outsize];
	d->outtop = d->outsize - 1;
	memset(d->outbuf, 'x', d->outtop);
}

/// Hands the connection a line of input, as read_from_descriptor would.
static void TypeLine(DESCRIPTOR_DATA *d, const char *line)
{
	snprintf(d->inbuf, sizeof(d->inbuf), "%s\n", line);
}

SCENARIO("a command that closes its own connection", "[gameloop][lifetime]")
{
	GIVEN("a playing character whose output buffer is at its ceiling")
	{
		TestWorld world;
		ROOM_INDEX_DATA *room = world.CreateRoom();
		CHAR_DATA *player = world.CreatePlayer("Tester", room);
		DESCRIPTOR_DATA *d = Deref(player->desc);
		auto connection = d->self;
		ListedConnection listed(d, true);

		FillOutputToCeiling(d);

		WHEN("the player types a command whose reply overflows the buffer")
		{
			TypeLine(d, "xyzzy");
			ProcessConnectionInput(d);

			THEN("the connection is closed, and the dispatch did not use it afterwards")
			{
				REQUIRE(Deref(connection) == nullptr);
			}
		}
	}

	GIVEN("a playing character with a queued command and a full output buffer")
	{
		TestWorld world;
		ROOM_INDEX_DATA *room = world.CreateRoom();
		CHAR_DATA *player = world.CreatePlayer("Tester", room);
		DESCRIPTOR_DATA *d = Deref(player->desc);
		auto connection = d->self;
		ListedConnection listed(d, true);

		FillOutputToCeiling(d);

		strcpy(player->pcdata->queue[0], "xyzzy");
		player->pcdata->write_next = 1;
		player->pcdata->pending = true;
		player->wait = 0;

		WHEN("the queued command runs and its reply overflows the buffer")
		{
			ProcessConnectionInput(d);

			THEN("the connection is closed, and the queue shuffle did not read it afterwards")
			{
				REQUIRE(Deref(connection) == nullptr);
			}
		}
	}

	GIVEN("a connection still at the name prompt")
	{
		TestWorld world;
		DESCRIPTOR_DATA *d = new_descriptor();
		auto connection = d->self;

		d->descriptor = -1;			// not a real socket; see CreateConnection
		d->connected = CON_GET_NAME;
		d->outsize = 2000;
		d->outbuf = new char[d->outsize];

		ListedConnection listed(d, false);

		WHEN("it sends an empty line, which nanny answers by closing it")
		{
			TypeLine(d, "");
			ProcessConnectionInput(d);

			THEN("the connection is closed, and the dispatch did not use it afterwards")
			{
				REQUIRE(Deref(connection) == nullptr);
			}
		}
	}

	GIVEN("an idle player in limbo, and a bystander where they will return to, both buffers full")
	{
		// stop_idling brings the player back with a room message. The
		// bystander's full buffer overflows on it, and closing a playing
		// connection tells the room that the bystander lost their link. The
		// returning player is in that room by then, so that message overflows
		// the player's own buffer and closes the player's connection, before
		// any command has run at all.
		TestWorld world;
		ROOM_INDEX_DATA *limbo = world.CreateRoom("Limbo");
		ROOM_INDEX_DATA *room = world.CreateRoom();
		RegisteredRoom registered(limbo, ROOM_VNUM_LIMBO);

		CHAR_DATA *bystander = world.CreatePlayer("Bystander", room);
		DESCRIPTOR_DATA *bd = Deref(bystander->desc);
		ListedConnection bystanderListed(bd, true);

		CHAR_DATA *player = world.CreatePlayer("Tester", limbo);
		DESCRIPTOR_DATA *d = Deref(player->desc);
		auto connection = d->self;
		ListedConnection listed(d, true);

		player->was_in_room = room;
		FillOutputToCeiling(bd);
		FillOutputToCeiling(d);

		WHEN("the player types anything, which wakes them first")
		{
			TypeLine(d, "xyzzy");
			ProcessConnectionInput(d);

			THEN("the connection is closed, and nothing after the wake used it")
			{
				REQUIRE(Deref(connection) == nullptr);
			}
		}
	}
}
