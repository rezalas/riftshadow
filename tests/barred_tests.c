#include "catch.hpp"
#include "world_fixture.h"

#include "../code/act_move.h"
#include "../code/lookup.h"

//
// A mob can be given a barred entry: a room it refuses to let people into
// unless one of their numbers compares the way it wants. Which number is
// decided by the criterion, and the criterion, the comparison and the message
// style are three separate families that all spell their values BAR_ and all
// start at zero.
//

namespace
{
	/// Gives the mob a barred entry aimed at a room. The entry belongs to the
	/// prototype, which the fixture deletes without looking inside it, so the
	/// caller releases it.
	BARRED_DATA *BarEntryTo(CHAR_DATA *mob, ROOM_INDEX_DATA *to_room, BarCriterion type, BarComparison comparison, short value, BarMessage msg_type)
	{
		auto bar = new BARRED_DATA();

		bar->type = type;
		bar->comparison = comparison;
		bar->value = value;
		bar->vnum = to_room->vnum;
		bar->msg_type = msg_type;
		bar->message = palloc_string("you shall not pass");
		bar->message_two = nullptr;

		mob->pIndexData->barred_entry = bar;

		return bar;
	}

	void ReleaseBarEntry(CHAR_DATA *mob)
	{
		BARRED_DATA *bar = mob->pIndexData->barred_entry;

		mob->pIndexData->barred_entry = nullptr;

		if (bar != nullptr)
		{
			free_pstring(bar->message);
			delete bar;
		}
	}
}

SCENARIO("a mob barring entry on a brand nobody is wearing", "[barred]")
{
	TestWorld world;

	auto hall = world.CreateRoom("The Hall");
	auto vault = world.CreateRoom("The Vault");
	vault->vnum = 4200;

	auto guard = world.CreateMob("guard", hall);
	auto mover = world.CreatePlayer("Mover", hall);

	GIVEN("a guard that bars the vault to anyone whose brand is not vnum zero")
	{
		// Vnum zero is no brand at all, so this entry lets everybody through.
		// Reaching that answer means reading the mover's brand, and the mover
		// does not have one.
		BarEntryTo(guard, vault, BAR_TATTOO, BAR_EQUAL_TO, 0, BAR_SAY);

		WHEN("someone with no brand tries to walk in")
		{
			bool barred = check_barred(mover, vault);

			THEN("they are let through rather than read out of nothing")
			{
				REQUIRE_FALSE(barred);
			}
		}

		ReleaseBarEntry(guard);
	}
}

SCENARIO("a mob barring entry on a level", "[barred]")
{
	TestWorld world;

	auto hall = world.CreateRoom("The Hall");
	auto vault = world.CreateRoom("The Vault");
	vault->vnum = 4201;

	auto guard = world.CreateMob("guard", hall);
	auto mover = world.CreatePlayer("Mover", hall);

	mover->level = 20;

	GIVEN("a guard that wants a level greater than the mover's")
	{
		BarEntryTo(guard, vault, BAR_LEVEL, BAR_GREATER_THAN, 30, BAR_ECHO);

		WHEN("the mover tries to walk in")
		{
			bool barred = check_barred(mover, vault);

			THEN("they are turned away")
			{
				REQUIRE(barred);
			}
		}

		ReleaseBarEntry(guard);
	}

	GIVEN("a guard that wants a level the mover has")
	{
		BarEntryTo(guard, vault, BAR_LEVEL, BAR_EQUAL_TO, 20, BAR_ECHO);

		WHEN("the mover tries to walk in")
		{
			bool barred = check_barred(mover, vault);

			THEN("they are let through")
			{
				REQUIRE_FALSE(barred);
			}
		}

		ReleaseBarEntry(guard);
	}
}

SCENARIO("the words an area file spells a barred entry with", "[barred]")
{
	GIVEN("each value of the three families")
	{
		THEN("it writes the word the loader reads back")
		{
			REQUIRE(bar_comparison_lookup(bar_comparison_name(BAR_EQUAL_TO)) == BAR_EQUAL_TO);
			REQUIRE(bar_comparison_lookup(bar_comparison_name(BAR_LESS_THAN)) == BAR_LESS_THAN);
			REQUIRE(bar_comparison_lookup(bar_comparison_name(BAR_GREATER_THAN)) == BAR_GREATER_THAN);

			REQUIRE(bar_message_lookup(bar_message_name(BAR_SAY)) == BAR_SAY);
			REQUIRE(bar_message_lookup(bar_message_name(BAR_ECHO)) == BAR_ECHO);
			REQUIRE(bar_message_lookup(bar_message_name(BAR_EMOTE)) == BAR_EMOTE);

			REQUIRE(!strcmp(bar_criterion_name(BAR_CLASS), "class"));
			REQUIRE(!strcmp(bar_criterion_name(BAR_CABAL), "cabal"));
			REQUIRE(!strcmp(bar_criterion_name(BAR_SIZE), "size"));
			REQUIRE(!strcmp(bar_criterion_name(BAR_TATTOO), "tattoo"));
			REQUIRE(!strcmp(bar_criterion_name(BAR_LEVEL), "level"));
		}
	}

	GIVEN("a word that is not one of them")
	{
		// There is no comparison value that means "not a comparison", so the
		// caller has to answer for the empty rather than being handed a value
		// that reads as EQUALTO.
		THEN("nothing comes back")
		{
			REQUIRE_FALSE(bar_comparison_lookup("sideways").has_value());
			REQUIRE_FALSE(bar_message_lookup("semaphore").has_value());
		}
	}
}
