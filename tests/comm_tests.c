#include <cstring>
#include <fcntl.h>
#include <unistd.h>

#include "catch.hpp"
#include "../code/comm.h"
#include "../code/merc.h"
#include "../code/recycle.h"
#include "world_fixture.h"

// process_output draws the prompt, and drawing the prompt used to be able to
// destroy the character it is drawn for: every write goes through
// write_to_buffer, and a buffer that passed 32000 bytes closed the socket and
// freed whoever was on the other end. The re-reads in that function exist for
// that reason. An overflow only marks the connection now (see below), but a
// character can still be freed by other means while its connection is open.
// These cover what happens when one of them comes back empty.
//
// A fixture connection carries no socket, and the flush at the end of
// process_output fails without one, so these tests hand it a writable file
// descriptor. That is what makes the return value mean "the prompt was drawn"
// rather than "there was nowhere to send it".

namespace
{
	/// A writable descriptor that discards, so write_to_descriptor succeeds and
	/// the return value of process_output reports the prompt rather than the socket.
	class NullSink
	{
	public:
		NullSink() : fd(open("/dev/null", O_WRONLY)) {}
		~NullSink() { if (fd >= 0) close(fd); }

		NullSink(const NullSink &) = delete;
		NullSink &operator=(const NullSink &) = delete;

		int fd;
	};
}

SCENARIO("the prompt is not drawn for a character that is already gone", "[process_output]")
{
	GIVEN("a playing connection whose character has been freed")
	{
		TestWorld world;
		NullSink sink;
		auto room = world.CreateRoom();
		auto player = world.CreatePlayer("Ghost", room);
		auto connection = Deref(player->desc);

		REQUIRE(sink.fd >= 0);
		REQUIRE(connection != nullptr);
		REQUIRE(Deref(connection->character) == player);

		connection->descriptor = sink.fd;

		WHEN("the character is freed while the connection is still open")
		{
			free_char(player);

			REQUIRE(Deref(connection->character) == nullptr);

			THEN("process_output reports failure rather than reading through the dead handle")
			{
				// Without the guard this dereferences a retired handle and the
				// process dies here. The return value is the visible half, and
				// not crashing is the half under test.
				REQUIRE(process_output(connection, true) == false);
			}

			THEN("nothing is written to the connection on the way out")
			{
				connection->outtop = 0;

				process_output(connection, true);

				// Every prompt write happens after the resolve, so bailing at
				// the resolve leaves the output buffer untouched.
				REQUIRE(connection->outtop == 0);
			}
		}

		connection->descriptor = -1;
	}
}

SCENARIO("a connection with a live character still reaches the prompt", "[process_output]")
{
	GIVEN("a playing connection whose character is alive")
	{
		TestWorld world;
		NullSink sink;
		auto room = world.CreateRoom();
		auto player = world.CreatePlayer("Solid", room);
		auto connection = Deref(player->desc);

		REQUIRE(sink.fd >= 0);
		REQUIRE(connection != nullptr);

		connection->descriptor = sink.fd;

		WHEN("output is processed with the prompt asked for")
		{
			connection->outtop = 0;

			bool drawn = process_output(connection, true);

			THEN("the prompt is drawn and the write succeeds")
			{
				// The reason the test above means anything: the guard has to
				// stop the dead case without also stopping the live one. Same
				// call, same connection, opposite answer.
				REQUIRE(drawn == true);
			}
		}

		connection->descriptor = -1;
	}
}

// An output buffer that passes its ceiling used to close the connection on the
// spot, inside write_to_buffer, which made every output call a call that could
// destroy the connection being written to and free the character on the end of
// it. The overflow now only marks the connection. It takes no more output, and
// ClosePendingConnections closes it at the end of the pulse, at a point where
// nothing is holding a pointer to it.

namespace
{
	/// Fills the output buffer to its ceiling, so the next write of any length
	/// overflows it. The buffer really is that size.
	void FillOutputToCeiling(DESCRIPTOR_DATA *d)
	{
		delete[] d->outbuf;

		d->outsize = 32000;
		d->outbuf = new char[d->outsize];
		d->outtop = d->outsize - 1;
		memset(d->outbuf, 'x', d->outtop);
	}

	/// Puts a connection on descriptor_list for the life of a scenario, as
	/// init_descriptor does, and takes it back off if it is still there at the
	/// end, so that whoever built it frees it exactly once: the world for a
	/// player's connection, this for a bare one. Declare it after the
	/// TestWorld, so it goes first.
	class ListedConnection
	{
	public:
		explicit ListedConnection(DESCRIPTOR_DATA *d, bool ownedByWorld = true)
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
}

SCENARIO("an output buffer that overflows marks its connection instead of closing it", "[write_to_buffer]")
{
	GIVEN("a listed playing connection whose buffer is at its ceiling")
	{
		TestWorld world;
		auto room = world.CreateRoom();
		auto player = world.CreatePlayer("Chatty", room);
		DESCRIPTOR_DATA *d = Deref(player->desc);
		auto connection = d->self;
		ListedConnection listed(d);

		FillOutputToCeiling(d);

		WHEN("one more line is written to it")
		{
			write_to_buffer(d, "One line too many.\n\r", 0);

			THEN("the connection is still there, marked to close, with its buffer emptied")
			{
				REQUIRE(Deref(connection) == d);
				REQUIRE(d->closePending);
				REQUIRE(d->outtop == 0);
				REQUIRE(Deref(player->desc) == d);
			}

			AND_WHEN("more is written to it afterwards")
			{
				write_to_buffer(d, "Nobody will read this.\n\r", 0);

				THEN("the output is dropped rather than buffered")
				{
					REQUIRE(d->outtop == 0);
				}
			}

			AND_WHEN("the end of the pulse closes pending connections")
			{
				ClosePendingConnections();

				THEN("the connection is closed")
				{
					REQUIRE(Deref(connection) == nullptr);
					REQUIRE(Deref(player->desc) == nullptr);
				}
			}
		}
	}

	GIVEN("a listed connection that has not overflowed")
	{
		TestWorld world;
		auto room = world.CreateRoom();
		auto player = world.CreatePlayer("Quiet", room);
		DESCRIPTOR_DATA *d = Deref(player->desc);
		auto connection = d->self;
		ListedConnection listed(d);

		WHEN("the end of the pulse closes pending connections")
		{
			ClosePendingConnections();

			THEN("it is left open")
			{
				REQUIRE(Deref(connection) == d);
			}
		}
	}
}

SCENARIO("closing a pending connection can overflow another, and both close in the same sweep", "[write_to_buffer]")
{
	GIVEN("two playing characters in one room, both buffers full, the second already overflowed")
	{
		// Closing a playing connection tells the room the character lost their
		// link. That can overflow a neighbour's buffer and mark it too. The
		// neighbour is listed ahead of the one being closed, so a single walk
		// has already passed it by the time it is marked.
		TestWorld world;
		auto room = world.CreateRoom();
		auto neighbour = world.CreatePlayer("Neighbour", room);
		auto overflowed = world.CreatePlayer("Overflowed", room);
		DESCRIPTOR_DATA *nd = Deref(neighbour->desc);
		DESCRIPTOR_DATA *od = Deref(overflowed->desc);
		auto neighbourConnection = nd->self;
		auto overflowedConnection = od->self;
		ListedConnection neighbourListed(nd);
		ListedConnection overflowedListed(od);

		FillOutputToCeiling(nd);
		od->closePending = true;

		WHEN("the end of the pulse closes pending connections")
		{
			ClosePendingConnections();

			THEN("both are closed, not just the one that was marked when it started")
			{
				REQUIRE(Deref(overflowedConnection) == nullptr);
				REQUIRE(Deref(neighbourConnection) == nullptr);
			}
		}
	}
}

SCENARIO("check_playing answers a connection whose buffer overflows on the reply", "[check_playing]")
{
	GIVEN("a character already playing, and a second connection logging in as them with a full buffer")
	{
		TestWorld world;
		auto room = world.CreateRoom();
		auto player = world.CreatePlayer("Twice", room);
		DESCRIPTOR_DATA *playing = Deref(player->desc);
		ListedConnection playingListed(playing);

		DESCRIPTOR_DATA *d = new_descriptor();
		auto connection = d->self;

		d->descriptor = -1;
		d->connected = CON_GET_OLD_PASSWORD;
		d->outsize = 2000;
		d->outbuf = new char[d->outsize];
		FillOutputToCeiling(d);

		ListedConnection listed(d, false);

		WHEN("check_playing finds the match and writes its question")
		{
			bool found = check_playing(d, (char *)"Twice");

			THEN("it reports the match, and the connection is waiting to close rather than gone")
			{
				REQUIRE(found);
				REQUIRE(Deref(connection) == d);
				REQUIRE(d->closePending);
			}
		}
	}
}
