#include "catch.hpp"
#include <string>
#include "../code/entity/pc_data.h"
#include "../code/lookup.h"
#include "../code/merc.h"
#include "world_fixture.h"

SCENARIO("listing the demons a character has bargained with", "[get_demon_names]")
{
	GIVEN("a player granted favor by a greater demon and by no lesser one")
	{
		TestWorld world;
		auto room = world.CreateRoom();
		auto player = world.CreatePlayer("Bargainer", room);

		for (int i = 0; i < MAX_LESSER; i++)
			player->pcdata->lesserdata[i] = FAVOR_NONE;

		for (int i = 0; i < MAX_GREATER; i++)
			player->pcdata->greaterdata[i] = FAVOR_NONE;

		player->pcdata->greaterdata[GREATER_GERYON] = FAVOR_GRANTED;

		WHEN("their demons are listed")
		{
			std::string names = get_demon_names(player);

			THEN("the greater demon is read from the greater-demon favors")
			{
				// The greater half of demon_table follows the lesser half, so
				// the table index runs past MAX_LESSER while the favor array
				// starts again at zero. Reading lesserdata at the table index
				// overran it, and only gave the right answer because
				// greaterdata happens to follow it in pc_data. The sanitizer
				// build's bounds check reports the overrun.
				REQUIRE(names == "Lesser: None\t\tGreater: Geryon (Granted)  ");
			}
		}
	}
}
