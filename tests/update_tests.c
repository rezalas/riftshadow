#include "catch.hpp"
#include "../code/update.h"
#include "../code/room.h"
#include "../code/weather_enums.h"

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
				REQUIRE(strstr(expected,result) != NULL);
			}
		}
		WHEN("an unexpectedly large number of hours is passed for the age in get_age_name_new")
		{ 
			auto result = get_age_name_new(9999999,race);
			auto expected = "worm-food";
			THEN("worm-food should be returned")
			{
				REQUIRE(strstr(expected,result) != NULL);
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
				REQUIRE(strstr(expected,result) != NULL);
			}
		}
		WHEN("20 hours is passed for the age in get_age_name_new")
		{ // 20 hours = 72000 seconds
			auto result = get_age_name_new(72000,race);
			auto expected = "young";
			THEN("young should be returned")
			{
				REQUIRE(strstr(expected,result) != NULL);
			}
		}
		WHEN("120 hours is passed for the age in get_age_name_new")
		{ // 120 hours = 432000 seconds
			auto result = get_age_name_new(432000,race);
			auto expected = "mature";
			THEN("mature should be returned")
			{
				REQUIRE(strstr(expected,result) != NULL);
			}
		}
		WHEN("220 hours is passed for the age in get_age_name_new")
		{
			auto result = get_age_name_new(792000,race);
			auto expected = "middle aged";
			THEN("middle aged should be returned")
			{
				REQUIRE(strstr(expected,result) != NULL);
			}
		}
		WHEN("300 hours is passed for the age in get_age_name_new")
		{
			auto result = get_age_name_new(1080000,race);
			auto expected = "old";
			THEN("old should be returned")
			{
				REQUIRE(strstr(expected,result) != NULL);
			}
		}
		WHEN("350 hours is passed for the age in get_age_name_new")
		{ 
			auto result = get_age_name_new(1260000,race);
			auto expected = "very old";
			THEN("very old should be returned")
			{
				REQUIRE(strstr(expected,result) != NULL);
			}
		}
		WHEN("1000 hours is passed for the age in get_age_name_new")
		{ 
			auto result = get_age_name_new(3600000,race);
			auto expected = "ancient";
			THEN("ancient should be returned")
			{
				REQUIRE(strstr(expected,result) != NULL);
			}
		}
		WHEN("1001 hours is passed for the age in get_age_name_new")
		{ 
			auto result = get_age_name_new(3603600,race);
			auto expected = "worm-food";
			THEN("worm-food should be returned")
			{
				REQUIRE(strstr(expected,result) != NULL);
			}
		}
	}
}

