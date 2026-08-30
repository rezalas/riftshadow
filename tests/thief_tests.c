#include "catch.hpp"
#include "../code/characterClasses/thief.h"
#include "../code/entity/char_data.h"
#include "../code/entity/room_index_data.h"
#include "../code/enums.h"
#include "../code/handler.h"
#include "../code/lookup.h"
#include "world_fixture.h"

// The direction indexes direction_lookup hands back. There is no enumeration
// for them, so the tests name them here rather than repeating the numbers.
static const int DoorNorth = 0;
static const int DoorSouth = 2;

/// Whether the room carries a tripwire laid across the exit given.
static bool RoomIsWiredFor(ROOM_INDEX_DATA *room, int door)
{
	for (auto &raf : room->affected)
	{
		if (raf.type == gsn_tripwire && raf.modifier == door)
			return true;
	}

	return false;
}

/// Lays tripwires until one is placed, or gives up. The skill roll can fail, so
/// a single call is not enough to say the direction was read correctly, and the
/// character's own cooldown affect has to come off between attempts.
static bool TripwireEverLands(CHAR_DATA *ch, char *direction, int door, int attempts)
{
	for (int attempt = 0; attempt < attempts; attempt++)
	{
		affect_strip(ch, gsn_tripwire);
		do_tripwire(ch, direction);

		if (RoomIsWiredFor(ch->in_room, door))
			return true;
	}

	return false;
}

SCENARIO("laying a tripwire in a direction", "[do_tripwire]")
{
	TestWorld::WireSkillNumbers();

	GIVEN("a thief in a room with an exit north")
	{
		TestWorld world;
		auto room = world.CreateRoom();
		auto beyond = world.CreateRoom("The Room Beyond");
		world.LinkRooms(room, beyond, DoorNorth);

		auto ch = world.CreatePlayer("Sneak", room, CLASS_THIEF);
		ch->pcdata->learned[gsn_tripwire] = 100;

		WHEN("the thief lays a wire north")
		{
			THEN("the wire goes across the north exit")
			{
				REQUIRE(TripwireEverLands(ch, (char *)"north", DoorNorth, 50));
			}
		}

		WHEN("the thief names a direction with no exit")
		{
			TestWorld::ClearOutput(ch);
			do_tripwire(ch, (char *)"south");

			THEN("the attempt is refused and no wire is laid")
			{
				REQUIRE(TestWorld::Heard(ch, "That's not a valid direction."));
				REQUIRE_FALSE(RoomIsWiredFor(room, DoorSouth));
			}
		}

		WHEN("the thief names something that is not a direction at all")
		{
			TestWorld::ClearOutput(ch);
			do_tripwire(ch, (char *)"banana");

			THEN("the attempt is refused")
			{
				REQUIRE(TestWorld::Heard(ch, "That's not a valid direction."));
			}
		}
	}

	GIVEN("a character who has never learned the skill")
	{
		TestWorld world;
		auto room = world.CreateRoom();
		auto beyond = world.CreateRoom("The Room Beyond");
		world.LinkRooms(room, beyond, DoorNorth);

		auto ch = world.CreatePlayer("Lubber", room, CLASS_THIEF);

		WHEN("they try to lay a wire")
		{
			TestWorld::ClearOutput(ch);
			do_tripwire(ch, (char *)"north");

			THEN("the command is refused before anything reads the direction")
			{
				REQUIRE(TestWorld::Heard(ch, "Huh?"));
				REQUIRE_FALSE(RoomIsWiredFor(room, DoorNorth));
			}
		}
	}
}
