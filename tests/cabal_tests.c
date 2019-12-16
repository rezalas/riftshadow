#include "catch.hpp"
#include "../code/cabal.h"
#include "../code/merc.h"

SCENARIO("testing cabal horde check", "[check_horde]")
{
    GIVEN("a character in horde")
    {
        char_data* player = new char_data();
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