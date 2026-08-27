#include "catch.hpp"
#include "world_fixture.h"
#include "../code/mprog.h"
#include "../code/enums.h"
#include "../code/mud.h"

// mprog_give and mprog_drop queue the unlink and the relink as two separate
// events on the same tick, so their relative order decides which inventory the
// object ends up in. Both were inverted: obj_to_char/obj_to_room repoint
// carried_by, so the trailing obj_from_char acted on the destination instead of
// the mob, leaving the object linked in two lists at once.
SCENARIO("Testing a mob handing an object to a character", "[mprog_give]")
{
	GIVEN("a mob carrying an object, and a character in the same room")
	{
		TestWorld world;
		auto room = world.CreateRoom();
		auto mob = world.CreateMob("Mob", room);
		auto ch = world.CreatePlayer("Player", room);
		auto obj = world.CreateItem("test_trinket", "a test trinket");

		obj_to_char(obj, mob);

		REQUIRE(mob->carrying == obj);

		WHEN("mprog_give queues the handover and the queue is processed")
		{
			mprog_give(1, nullptr, obj, mob, ch);
			RS.Queue.ProcessQueue();

			THEN("the object leaves the mob and is carried by the character")
			{
				REQUIRE(Deref(obj->carried_by) == ch);
				REQUIRE(ch->carrying == obj);
				REQUIRE(mob->carrying == nullptr);
			}
		}
	}
}

SCENARIO("Testing a mob dropping an object", "[mprog_drop]")
{
	GIVEN("a mob carrying an object")
	{
		TestWorld world;
		auto room = world.CreateRoom();
		auto mob = world.CreateMob("Mob", room);
		auto obj = world.CreateItem("test_trinket", "a test trinket");

		obj_to_char(obj, mob);

		REQUIRE(mob->carrying == obj);

		WHEN("mprog_drop queues the drop and the queue is processed")
		{
			mprog_drop(1, nullptr, obj, mob, mob);
			RS.Queue.ProcessQueue();

			THEN("the object leaves the mob and lies in the room")
			{
				REQUIRE(Deref(obj->carried_by) == nullptr);
				REQUIRE(obj->in_room == room);
				REQUIRE(room->contents == obj);
				REQUIRE(mob->carrying == nullptr);
			}
		}
	}
}

//
// pulse_prog_demon -- the summoned demon's per-pulse behaviour.
//
// It gives up and returns to the Hells when its quarry is gone, and one of the
// ways "gone" is spelled is `hunting` no longer resolving to anybody. The
// give-up branch used to announce having taken the quarry's soul without
// checking there was a quarry to read.
//
// Reachability, stated because it changes what this test is worth: nothing in
// the tree spawns the mobs this prog is attached to (area/skills.are hangs it
// on vnums 2935-2937, which no code creates and no area resets), so the crash
// is latent rather than live. The guard is still wrong, and the call below is
// the branch nothing else reaches.
//

SCENARIO("a demon whose quarry is gone gives up without reading it", "[mprog_demon]")
{
	GIVEN("a demon with a master and no quarry")
	{
		TestWorld world;
		auto room = world.CreateRoom();
		// Both NPCs: extract_char's die_follower walk reaches can_see, which
		// reads pcdata on anything that is not one.
		auto master = world.CreateMob("Summoner", room);
		auto demon = world.CreateMob("Demon", room);

		demon->master = master->self;

		REQUIRE(Deref(demon->hunting) == nullptr);

		WHEN("the pulse comes round")
		{
			pulse_prog_demon(demon);

			THEN("it runs the give-up branch through instead of dereferencing the quarry it does not have")
			{
				// Reaching char_from_room is the observable end of that branch.
				// The extract stops short of freeing here, since this demon was
				// never linked into char_list, so the room is what to assert on.
				REQUIRE(demon->in_room == nullptr);
			}
		}
	}
}
