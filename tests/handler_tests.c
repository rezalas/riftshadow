#include "catch.hpp"
#include "../code/handler.h"
#include "../code/db.h"

// TEST_CASE("Test capitalization", "[string]" )
// {
// 	const char* sample = "this string is lower case.";
// 	char* test = capitalize(sample);
// 	char* shouldBe = "This string is lower case.";
// 	REQUIRE(strcmp(test, shouldBe) == 0);
// }

SCENARIO("Testing race lookup", "[race_lookup]")
{
	GIVEN("a valid race")
	{
		WHEN("race_lookup function is called")
		{
			int test = race_lookup("elf");

			THEN("then it should return the correct index")
			{
				REQUIRE(test == 4);
			}
		}

		WHEN("race_lookup function is called with the race in uppercase")
		{
			int test = race_lookup("ELF");

			THEN("then it should return the correct index")
			{
				REQUIRE(test == 4);
			}
		}
	}

	GIVEN("an invalid race")
	{
		WHEN("race_lookup function is called")
		{
			int test = race_lookup("foo");

			THEN("then it should return an index of -1")
			{
				REQUIRE(test == -1);
			}
		}

		WHEN("race_lookup function is called with a valid race but with extra characters")
		{
			int test = race_lookup("elf  ");

			THEN("then it should return an index of -1")
			{
				REQUIRE(test == -1);
			}
		}

		WHEN("race_lookup function is called with special characters")
		{
			int test = race_lookup("ÿàÞ");

			THEN("then it should return an index of -1")
			{
				REQUIRE(test == -1);
			}
		}
	}

	GIVEN("an uninitialized race")
	{
		WHEN("race_lookup function is called")
		{
			int test = race_lookup(NULL);

			THEN("then it should return an index of -1")
			{
				REQUIRE(test == -1);
			}
		}
	}
}

SCENARIO("Testing liq lookup", "[liq_lookup]")
{
	GIVEN("a valid liquid")
	{
		WHEN("liq_lookup function is called")
		{
			int test = liq_lookup("coffee");

			THEN("then it should return the correct index")
			{
				REQUIRE(test == 12);
			}
		}

		WHEN("liq_lookup function is called with the liquid in uppercase")
		{
			int test = liq_lookup("COFFEE");

			THEN("then it should return the correct index")
			{
				REQUIRE(test == 12);
			}
		}
	}

	GIVEN("an invalid liquid")
	{
		WHEN("liq_lookup function is called")
		{
			int test = liq_lookup("foo");

			THEN("then it should return an index of 0")
			{
				REQUIRE(test == 0);
			}
		}

		WHEN("liq_lookup function is called with a valid liquid but with extra characters")
		{
			int test = liq_lookup("coffee  ");

			THEN("then it should return an index of 0")
			{
				REQUIRE(test == 0);
			}
		}

		WHEN("liq_lookup function is called with special characters")
		{
			int test = liq_lookup("ÿàÞ");

			THEN("then it should return an index of 0")
			{
				REQUIRE(test == 0);
			}
		}
	}

	GIVEN("an uninitialized liquid")
	{
		WHEN("liq_lookup function is called")
		{
			int test = liq_lookup(NULL);

			THEN("then it should return an index of 0")
			{
				REQUIRE(test == 0);
			}
		}
	}
}

SCENARIO("Testing weapon lookup", "[weapon_lookup]")
{
	GIVEN("a valid weapon")
	{
		WHEN("weapon_lookup function is called")
		{
			int test = weapon_lookup("polearm");

			THEN("then it should return the correct index")
			{
				REQUIRE(test == 7);
			}
		}

		WHEN("weapon_lookup function is called with the weapon in uppercase")
		{
			int test = weapon_lookup("POLEARM");

			THEN("then it should return the correct index")
			{
				REQUIRE(test == 7);
			}
		}
	}

	GIVEN("an invalid weapon")
	{
		WHEN("weapon_lookup function is called")
		{
			int test = weapon_lookup("foo");

			THEN("then it should return an index of -1")
			{
				REQUIRE(test == -1);
			}
		}

		WHEN("weapon_lookup function is called with a valid weapon but with extra characters")
		{
			int test = weapon_lookup("polearm  ");

			THEN("then it should return an index of -1")
			{
				REQUIRE(test == -1);
			}
		}

		WHEN("weapon_lookup function is called with special characters")
		{
			int test = weapon_lookup("ÿàÞ");

			THEN("then it should return an index of -1")
			{
				REQUIRE(test == -1);
			}
		}
	}

	GIVEN("an uninitialized weapon")
	{
		WHEN("weapon_lookup function is called")
		{
			int test = weapon_lookup(NULL);

			THEN("then it should return an index of -1")
			{
				REQUIRE(test == -1);
			}
		}
	}
}

SCENARIO("Testing weapon type lookup", "[weapon_type_lookup]")
{
	GIVEN("a valid weapon")
	{
		WHEN("weapon_type_lookup function is called")
		{
			int test = weapon_type_lookup("sword");

			THEN("then it should return the correct type")
			{
				REQUIRE(test == WEAPON_SWORD);
			}
		}

		WHEN("weapon_type_lookup function is called with the weapon in uppercase")
		{
			int test = weapon_type_lookup("SWORD");

			THEN("then it should return the correct type")
			{
				REQUIRE(test == WEAPON_SWORD);
			}
		}
	}

	GIVEN("an invalid weapon")
	{
		WHEN("weapon_type_lookup function is called")
		{
			int test = weapon_type_lookup("foo");

			THEN("then it should return the WEAPON_EXOTIC type")
			{
				REQUIRE(test == WEAPON_EXOTIC);
			}
		}

		WHEN("weapon_type_lookup function is called with a valid weapon but with extra characters")
		{
			int test = weapon_type_lookup("sword  ");

			THEN("then it should return the WEAPON_EXOTIC type")
			{
				REQUIRE(test == WEAPON_EXOTIC);
			}
		}

		WHEN("weapon_type_lookup function is called with special characters")
		{
			int test = weapon_type_lookup("ÿàÞ");

			THEN("then it should return the WEAPON_EXOTIC type")
			{
				REQUIRE(test == WEAPON_EXOTIC);
			}
		}
	}

	GIVEN("an uninitialized weapon")
	{
		WHEN("weapon_type_lookup function is called")
		{
			int test = weapon_type_lookup(NULL);

			THEN("then it should return the WEAPON_EXOTIC type")
			{
				REQUIRE(test == WEAPON_EXOTIC);
			}
		}
	}
}

SCENARIO("Testing weapon name lookup", "[weapon_name_lookup]")
{
	GIVEN("a valid weapon")
	{
		WHEN("weapon_name_lookup function is called")
		{
			char* test = weapon_name_lookup(WEAPON_SPEAR);

			THEN("then it should return the correct weapon name")
			{
				REQUIRE(!str_prefix(test, "spear"));
			}
		}
	}

	GIVEN("an invalid weapon")
	{
		WHEN("weapon_name_lookup function is called")
		{
			char* test = weapon_name_lookup(-3);

			THEN("then it should return \"weapon\" as the weapon name")
			{
				REQUIRE(!str_prefix(test, "weapon"));
			}
		}

		WHEN("weapon_name_lookup function is called with a passed in default name")
		{
			char* test = weapon_name_lookup(-3, "exotic");

			THEN("then it should return the default name as the weapon name")
			{
				REQUIRE(!str_prefix(test, "exotic"));
			}
		}
	}
}

SCENARIO("Testing item lookup", "[item_lookup]")
{
	GIVEN("a valid item")
	{
		WHEN("item_lookup function is called")
		{
			int test = item_lookup("food");

			THEN("then it should return the correct item type")
			{
				REQUIRE(test == ITEM_FOOD);
			}
		}

		WHEN("item_lookup function is called with the item in uppercase")
		{
			int test = item_lookup("FOOD");

			THEN("then it should return the correct item type")
			{
				REQUIRE(test == ITEM_FOOD);
			}
		}
	}

	GIVEN("an invalid item")
	{
		WHEN("item_lookup function is called")
		{
			int test = item_lookup("bar");

			THEN("then it should return an index of -1")
			{
				REQUIRE(test == -1);
			}
		}

		WHEN("item_lookup function is called with a valid item but with extra characters")
		{
			int test = item_lookup("food  ");

			THEN("then it should return an index of -1")
			{
				REQUIRE(test == -1);
			}
		}

		WHEN("item_lookup function is called with special characters")
		{
			int test = item_lookup("ÿàÞ");

			THEN("then it should return an index of -1")
			{
				REQUIRE(test == -1);
			}
		}
	}

	GIVEN("an uninitialized item")
	{
		WHEN("item_lookup function is called")
		{
			int test = item_lookup(NULL);

			THEN("then it should return an index of -1")
			{
				REQUIRE(test == -1);
			}
		}
	}
}

SCENARIO("Testing item name lookup", "[item_name_lookup]")
{
	GIVEN("a valid item")
	{
		WHEN("item_name_lookup function is called")
		{
			char* test = item_name_lookup(ITEM_KEY);

			THEN("then it should return the correct weapon name")
			{
				REQUIRE(!str_prefix(test, "key"));
			}
		}
	}

	GIVEN("an invalid item")
	{
		WHEN("item_name_lookup function is called")
		{
			char* test = item_name_lookup(-18);

			THEN("then it should return \"none\" as the weapon name")
			{
				REQUIRE(!str_prefix(test, "none"));
			}
		}
	}
}

SCENARIO("Testing attack lookup", "[attack_lookup]")
{
	GIVEN("a valid attack")
	{
		WHEN("attack_lookup function is called")
		{
			int test = attack_lookup("punch");

			THEN("then it should return the correct index")
			{
				REQUIRE(test == 17);
			}
		}

		WHEN("attack_lookup function is called with the attack in uppercase")
		{
			int test = attack_lookup("PUNCH");

			THEN("then it should return the correct index")
			{
				REQUIRE(test == 17);
			}
		}
	}

	GIVEN("an invalid attack")
	{
		WHEN("attack_lookup function is called")
		{
			int test = attack_lookup("foo");

			THEN("then it should return an index of 0")
			{
				REQUIRE(test == 0);
			}
		}

		WHEN("attack_lookup function is called with a valid attack but with extra characters")
		{
			int test = attack_lookup("punch  ");

			THEN("then it should return an index of 0")
			{
				REQUIRE(test == 0);
			}
		}

		WHEN("attack_lookup function is called with special characters")
		{
			int test = attack_lookup("ÿàÞ");

			THEN("then it should return an index of 0")
			{
				REQUIRE(test == 0);
			}
		}
	}

	GIVEN("an uninitialized attack")
	{
		WHEN("attack_lookup function is called")
		{
			int test = attack_lookup(NULL);

			THEN("then it should return an index of 0")
			{
				REQUIRE(test == 0);
			}
		}
	}
}

SCENARIO("Testing wiznet lookup", "[wiznet_lookup]")
{
	GIVEN("a valid wiznet flag")
	{
		WHEN("wiznet_lookup function is called")
		{
			long test = wiznet_lookup("cabal");

			THEN("then it should return the correct index")
			{
				REQUIRE(test == 20);
			}
		}

		WHEN("wiznet_lookup function is called with the wiznet flag in uppercase")
		{
			long test = wiznet_lookup("CABAL");

			THEN("then it should return the correct index")
			{
				REQUIRE(test == 20);
			}
		}
	}

	GIVEN("an invalid wiznet flag")
	{
		WHEN("wiznet_lookup function is called")
		{
			long test = wiznet_lookup("foo");

			THEN("then it should return an index of -1")
			{
				REQUIRE(test == -1);
			}
		}

		WHEN("wiznet_lookup function is called with a valid wiznet but with extra characters")
		{
			long test = wiznet_lookup("cabal  ");

			THEN("then it should return an index of -1")
			{
				REQUIRE(test == -1);
			}
		}

		WHEN("wiznet_lookup function is called with special characters")
		{
			long test = wiznet_lookup("ÿàÞ");

			THEN("then it should return an index of -1")
			{
				REQUIRE(test == -1);
			}
		}
	}

	GIVEN("an uninitialized wiznet flag")
	{
		WHEN("wiznet_lookup function is called")
		{
			long test = wiznet_lookup(NULL);

			THEN("then it should return an index of -1")
			{
				REQUIRE(test == -1);
			}
		}
	}
}

SCENARIO("Testing deduct gold cost from character","[deduct_cost]")
{
	GIVEN("a character with zero gold")
	{
		auto player = new char_data();
		player->gold = 0;

		WHEN("gold is taken from that character")
		{
			deduct_cost(player, 1000);
			THEN("their gold should be set to zero")
			{
				REQUIRE(player->gold == 0);
			}
		}
	}
}
