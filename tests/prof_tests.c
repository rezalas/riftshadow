#include "catch.hpp"
#include "../code/prof.h"
#include "../code/merc.h"

SCENARIO("testing interpreting proficiency commands", "[InterpCommand]")
{
    CProficiencies::AssignPsns();

    GIVEN("an invalid command")
    {
        WHEN("a character calls it")
        {
            char_data* player = new char_data();
            player->name = "player1";
            player->pcdata = new pc_data();
            player->Profs()->SetChar(player);

            char* command = "";
            char* argument = "";
            THEN("it should return false")
            {
                bool result = player->Profs()->InterpCommand(command, argument);

                REQUIRE(result == false);
            }
        }
    }

    GIVEN("a valid command")
    {
        WHEN("a character without the skill calls it")
        {
            char_data* player = new char_data();
            player->name = "player1";
            player->pcdata = new pc_data();
            player->Profs()->SetChar(player);

            char* command = "firestart";
            char* argument = "";
            THEN("it should return false")
            {
                bool result = player->Profs()->InterpCommand(command, argument);

                REQUIRE(result == false);
            }
        }
        //uses prof name: firestarting, command: firestart, argument req: none
        WHEN("a character with the skill calls it")
        {
            char_data* player = new char_data();
            player->name = "player1";
            player->pcdata = new pc_data();
            player->in_room = new room_index_data();
            player->in_room->sector_type = SECT_WATER;
            player->Profs()->SetChar(player);
            int profNum = 7;
            auto* playerProf = player->Profs();
            player->Profs()->SetProf(profNum, 1);
            

            char* command = "firestart";
            char* argument = "";
            THEN("it should return true")
            {
                bool result = player->Profs()->InterpCommand(command, argument);

                REQUIRE(result == true);
            }
        }
    }
}

SCENARIO("testing outputting character known proficiencies", "[ListKnownProficiencies]")
{
    GIVEN("a player with no proficiencies")
    {
        WHEN("the function is called")
        {
            THEN("it should send a single line of text to the player buffer")
            {
                char_data* player = new char_data();
                player->name = "player1";
                player->pcdata = new pc_data();
                player->desc = new descriptor_data();
                player->desc->outbuf = "\n\r";
                player->desc->outtop = 2;
                player->desc->outsize = 100;
                player->Profs()->SetChar(player);

                player->Profs()->ListKnownProficiencies(player);

                REQUIRE(player->desc->outbuf);
            }
        }
    }
}