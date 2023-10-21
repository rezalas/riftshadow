#include "catch.hpp"
#include "../code/merc.h"
#include "../code/cabal.h"
#include "../code/lookup.h"
#include "../code/magic.h"
#include "../code/tables.h"
#include "../code/db.h"

char_data* TestHelperCreatePlayer(char *name = "player 1")
{
	auto player = new char_data();
	player->pcdata = new pc_data();
	player->name = name;
	auto dnew = new descriptor_data();
	dnew->showstr_head = nullptr;
	dnew->showstr_point = nullptr;
	dnew->outsize = 2000;
	dnew->pEdit = nullptr;	  /* OLC */
	dnew->pString = nullptr; /* OLC */
	dnew->editor = 0;	  /* OLC */
	dnew->outbuf = new char[dnew->outsize];
	dnew->outtop = 0;	
	player->desc = dnew;

	return player;
}

void TestHelperAddPlayerToCabal(char_data* player, char *cabalName = "guild")
{
	auto cabalCode = cabal_lookup(cabalName);
	player->cabal = cabalCode;
	player->pcdata->cabal_level = cabal_table[cabalCode].start_level;
	cabal_members[cabalCode]++;
}

SCENARIO("testing cabal horde check", "[check_horde]")
{
    GIVEN("a character in horde")
    {
        char_data* player = TestHelperCreatePlayer("player 1");
        player->cabal = CABAL_HORDE;

        WHEN("check_horde is called")
        {
            bool result = check_horde(player);

            THEN("it should return true")
            {
                REQUIRE(result == true);
            }
        }
    }
    GIVEN("a character not in horde but in another cabal")
    {
        char_data* player = new char_data();
        player->cabal = CABAL_BOUNTY;

        WHEN("check_horde is called")
        {
            bool result = check_horde(player);

            THEN("it should return false")
            {
                REQUIRE(result == false);
            }
        }
    }
    GIVEN("a character not in any cabal")
    {
        char_data* player = new char_data();

        WHEN("check_horde is called")
        {
            bool result = check_horde(player);

            THEN("it should return false")
            {
                REQUIRE(result == false);
            }
        }
    }
}

SCENARIO("testing update cabal skills","[update_cskills]")
{
	GIVEN("a null value instead of a character")
	{
		WHEN("update_cskills is called ")
		{
			THEN("it should return without crashing")
			{				
				REQUIRE_NOTHROW(update_cskills(nullptr));
			}
		}
	}
	GIVEN("a character not in a cabal")
	{
		WHEN("update_cskills is called")
		{
			auto player = TestHelperCreatePlayer("player 1");

			for (auto skill : cabal_skills)
				player->pcdata->learned[skill_lookup(skill.skill)] = 70;
			
			update_cskills(player);
			THEN("it should set all cabal skills to zero")
			{
				for(auto skill : cabal_skills)
				{
					REQUIRE(player->pcdata->learned[skill_lookup(skill.skill)] == 0);
				}
			}
		}
	}
	GIVEN("a character in a cabal")
	{
		WHEN("update_cskills is called")
		{
			auto player = TestHelperCreatePlayer("player 1");
			TestHelperAddPlayerToCabal(player);

			update_cskills(player);
			THEN("only skills associated with that cabal should increase, and only skills the player has levels for.")
			{
				for(auto skill : cabal_skills)
				{
					if(skill.cabal != player->cabal)
					{
						REQUIRE(player->pcdata->learned[skill_lookup(skill.skill)] == 0);
					}
					else if(player->pcdata->cabal_level < skill.level)
					{
						REQUIRE(player->pcdata->learned[skill_lookup(skill.skill)] == 0);	
					}
					else 
					{
						REQUIRE(player->pcdata->learned[skill_lookup(skill.skill)] > 0);
					}
				}
			}
		
		}
	}
}