#include <cstring>

#include "catch.hpp"
#include "../code/update.h"
#include "../code/direction.h"
#include "../code/weather_enums.h"
#include "../code/magic.h"
#include "../code/const.h"
#include "world_fixture.h"

SCENARIO("checking if lunar cycles advance with the right values","[LunarCycle_update]")
{
	GIVEN("A moon with values outside the scope of a cycle")
	{
		short int testMoon = -5;
		short int testMoon_pos = -5;
		WHEN("LunarCycle_update is called with invalid values")
		{
			LunarCycle_update(&testMoon, &testMoon_pos);
			THEN("The moon and position should be set to a new cycle")
			{
				LunarCyclePosition expectedMoon = LunarCyclePosition::NewMoon;
				short expectedPos = 0;
				REQUIRE(testMoon == expectedMoon);
				REQUIRE(testMoon_pos == expectedPos);
			}
		}
	}

	GIVEN("A moon at the start of a cycle")
	{
		short int newMoon = 0;
		short int newMoonPos = 0;
		WHEN("LunarCycle_Update is called with additional days remaining in the phase")
		{
			auto expectedMoon = LunarCyclePosition::NewMoon;
			auto expectedPos = newMoonPos + 1;
			LunarCycle_update(&newMoon, &newMoonPos);
			THEN("The moon should remain in that phase")
			{
				REQUIRE(newMoon == expectedMoon);
				REQUIRE(newMoonPos == expectedPos);
			}
		}

		WHEN("LunarCycle_Update is called on the last day of a phase")
		{
			newMoonPos = 44;
			auto expectedMoon = LunarCyclePosition::CrescentWaxing;
			auto expectedPos = newMoonPos + 1;
			LunarCycle_update(&newMoon, &newMoonPos);
			THEN("The moon should remain in that phase")
			{
				REQUIRE(newMoon == expectedMoon);
				REQUIRE(newMoonPos == expectedPos);
			}
		}
	}
}

SCENARIO("Verifying reverse_d reverses to the proper directions", "[reverse_d]")
{
	GIVEN("any integer value that should represent a direction")
	{
		int direction = 0;
		WHEN("reverse_d is passed a value that is not mapped to a direction")
		{
			direction = -9001;
			int result = reverse_d(direction);

			THEN("reverse_d should return zero")
			{
				int expected = 0;
				
				REQUIRE(result == expected);
			}
		}

		WHEN("reverse_d is passed a value that maps to Up")
		{
			direction = Directions::Up;
			int result = reverse_d(direction);

			THEN("reverse_d should return Down")
			{
				int expected = Directions::Down;
				
				REQUIRE(result == expected);
			}
		}

		WHEN("reverse_d is passed a value that maps to Down")
		{
			direction = Directions::Down;
			int result = reverse_d(direction);

			THEN("reverse_d should return Up")
			{
				int expected = Directions::Up;
				
				REQUIRE(result == expected);
			}
		}

		WHEN("reverse_d is passed a value that maps to West")
		{
			direction = Directions::West;
			int result = reverse_d(direction);

			THEN("reverse_d should return East")
			{
				int expected = Directions::East;
				
				REQUIRE(result == expected);
			}
		}

		WHEN("reverse_d is passed a value that maps to East")
		{
			direction = Directions::East;
			int result = reverse_d(direction);

			THEN("reverse_d should return West")
			{
				int expected = Directions::West;
				
				REQUIRE(result == expected);
			}
		}

		WHEN("reverse_d is passed a value that maps to North")
		{
			direction = Directions::North;
			int result = reverse_d(direction);

			THEN("reverse_d should return South")
			{
				int expected = Directions::South;
				
				REQUIRE(result == expected);
			}
		}

		WHEN("reverse_d is passed a value that maps to South")
		{
			direction = Directions::South;
			int result = reverse_d(direction);

			THEN("reverse_d should return North")
			{
				int expected = Directions::North;
				
				REQUIRE(result == expected);
			}
		}
	}
}

SCENARIO("checking that get_age_name_new provides the right age names", "[get_age_name_new]")
{
	GIVEN("An age outside the specified range and a race of zero")
	{
		auto race = 0;
		WHEN("a negative number is passed for the age in get_age_name_new")
		{
			auto result = get_age_name_new(-9001,race);
			auto expected = "young";
			THEN("young should be returned")
			{
				REQUIRE(strstr(expected,result) != nullptr);
			}
		}
		WHEN("an unexpectedly large number of hours is passed for the age in get_age_name_new")
		{ 
			auto result = get_age_name_new(9999999,race);
			auto expected = "worm-food";
			THEN("worm-food should be returned")
			{
				REQUIRE(strstr(expected,result) != nullptr);
			}
		}
	}
	GIVEN("An age inside the specified ranges and a race of zero")
	{
		auto race = 0;
		WHEN("zero is passed for the age in get_age_name_new")
		{
			auto result = get_age_name_new(0,race);
			auto expected = "young";
			THEN("young should be returned")
			{
				REQUIRE(strstr(expected,result) != nullptr);
			}
		}
		WHEN("20 hours is passed for the age in get_age_name_new")
		{ // 20 hours = 72000 seconds
			auto result = get_age_name_new(72000,race);
			auto expected = "young";
			THEN("young should be returned")
			{
				REQUIRE(strstr(expected,result) != nullptr);
			}
		}
		WHEN("120 hours is passed for the age in get_age_name_new")
		{ // 120 hours = 432000 seconds
			auto result = get_age_name_new(432000,race);
			auto expected = "mature";
			THEN("mature should be returned")
			{
				REQUIRE(strstr(expected,result) != nullptr);
			}
		}
		WHEN("220 hours is passed for the age in get_age_name_new")
		{
			auto result = get_age_name_new(792000,race);
			auto expected = "middle aged";
			THEN("middle aged should be returned")
			{
				REQUIRE(strstr(expected,result) != nullptr);
			}
		}
		WHEN("300 hours is passed for the age in get_age_name_new")
		{
			auto result = get_age_name_new(1080000,race);
			auto expected = "old";
			THEN("old should be returned")
			{
				REQUIRE(strstr(expected,result) != nullptr);
			}
		}
		WHEN("350 hours is passed for the age in get_age_name_new")
		{ 
			auto result = get_age_name_new(1260000,race);
			auto expected = "very old";
			THEN("very old should be returned")
			{
				REQUIRE(strstr(expected,result) != nullptr);
			}
		}
		WHEN("1000 hours is passed for the age in get_age_name_new")
		{ 
			auto result = get_age_name_new(3600000,race);
			auto expected = "ancient";
			THEN("ancient should be returned")
			{
				REQUIRE(strstr(expected,result) != nullptr);
			}
		}
		WHEN("1001 hours is passed for the age in get_age_name_new")
		{ 
			auto result = get_age_name_new(3603600,race);
			auto expected = "worm-food";
			THEN("worm-food should be returned")
			{
				REQUIRE(strstr(expected,result) != nullptr);
			}
		}
	}
}



//
// The paladin commune renewal on the expiry tick, and who it asks about trust.
//
// A commune that reaches duration zero normally expires. A paladin's channeling
// can renew it instead, on their own affects and on those they placed on
// somebody else, and the second case is meant to require that the paladin
// trusts the character carrying it. The condition read
// `(A || (B && C)) && D` while its indentation claimed `((A || B) && C) && D`,
// so C, the trust check, sat inside the branch for an affect with no owner. An
// affect with an owner is the common case, and it renewed without ever asking.
//
// The renewal is a percentage roll, so the trusted case is run until it fires.
// It is also the control for the untrusted case: "the commune was not renewed"
// means nothing unless renewal is reachable at all.
//

static bool CommuneRenewedOnExpiry(bool trusted, int attempts)
{
	TestWorld::WireSkillNumbers();

	int sn = skill_lookup("detect evil");
	REQUIRE(sn > 0);
	// The renewal is refused outright for anything a cleanse could strip, so
	// this test would be measuring the wrong branch with such a skill.
	REQUIRE(!(skill_table[sn].dispel & CAN_CLEANSE));

	for (int attempt = 0; attempt < attempts; attempt++)
	{
		TestWorld world;
		auto room = world.CreateRoom();
		auto paladin = world.CreatePlayer("Paladin", room, CLASS_PALADIN);
		auto believer = world.CreatePlayer("Believer", room);

		// Channeling opens to paladins above level 50, so a level 50 paladin
		// has none of it and nothing would ever be renewed.
		paladin->level = 60;
		paladin->pcdata->learned[gsn_channeling] = 100;

		if (trusted)
			paladin->pcdata->trusting = believer->self;

		AFFECT_DATA af;
		init_affect(&af);
		af.where = TO_AFFECTS;
		af.aftype = AFT_COMMUNE;
		af.type = sn;
		af.level = 50;
		af.duration = 20;
		af.owner = paladin->self;

		affect_to_char(believer, &af);

		// affect_to_char records the duration it was given as the one a renewal
		// restores, so the affect is aged to its last tick afterwards rather
		// than added at zero.
		AFFECT_DATA *live = affect_find(believer->affected, sn);
		REQUIRE(live != nullptr);
		live->duration = 0;

		world.LinkToCharList(paladin);
		world.LinkToCharList(believer);

		char_update();

		if (is_affected(believer, sn))
			return true;
	}

	return false;
}

SCENARIO("a paladin's commune is only renewed for somebody they trust", "[char_update]")
{
	GIVEN("a commune placed on a character the paladin trusts")
	{
		WHEN("it reaches its last tick")
		{
			THEN("channeling renews it")
			{
				REQUIRE(CommuneRenewedOnExpiry(true, 60));
			}
		}
	}

	GIVEN("a commune placed on a character the paladin does not trust")
	{
		WHEN("it reaches its last tick")
		{
			THEN("it expires like any other affect")
			{
				REQUIRE_FALSE(CommuneRenewedOnExpiry(false, 60));
			}
		}
	}
}
