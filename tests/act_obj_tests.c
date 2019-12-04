#include <string.h>
#include <stddef.h>
#include "catch.hpp"
#include "../code/act_obj.h"
#include "../code/merc.h"

SCENARIO("testing finding shop keepers","[find_keeper]")
{
    GIVEN("a player character in a room")
    {
        WHEN("find_keeper is called while alone")
        {
            char_data* player = new char_data();
            room_index_data* room = new room_index_data();
            player->in_room = room;
            player->next_in_room = room->people;
            room->people = player;
            auto result = find_keeper(player);
            THEN("find_keeper should return null")
            {
                REQUIRE(result == NULL);
            }
        }
    }
    
    GIVEN("a player character and an NPC non-keeper in a room")
    {
        WHEN("find_keeper is called")
        {
            char_data* player1 = new char_data();
            player1->name = "player 1";
            char_data* npc = new char_data();
            SET_BIT(npc->act, ACT_IS_NPC);

            room_index_data* room = new room_index_data();

            npc->in_room = room;
            npc->next_in_room = room->people;
            room->people = npc;

            player1->in_room = room;
            player1->next_in_room = room->people;
            room->people = player1;

            auto result = find_keeper(player1);
            THEN("find_keeper should return null")
            {
                REQUIRE(result == NULL);
            }
        }
    }

    GIVEN("a player character and an NPC keeper in a room")
    {
        WHEN("find_keeper is called")
        {
            char_data* player1 = new char_data();
            player1->name = "player 1";
            // setup NPC with flag and shop index
            char_data* npc = new char_data();
            SET_BIT(npc->act, ACT_IS_NPC);
            npc->pIndexData = new mob_index_data();
            npc->pIndexData->pShop = new shop_data();

            room_index_data* room = new room_index_data();
            room->area = new area_data();
            
            npc->in_room = room;
            npc->next_in_room = room->people;
            room->people = npc;

            player1->in_room = room;
            player1->next_in_room = room->people;
            room->people = player1;

            auto result = find_keeper(player1);
            THEN("find_keeper should return the NPC")
            {
                REQUIRE(result == npc);
            }
        }
    }

    GIVEN("two players and only non-keeper players in the room")
    {
        WHEN("find_keeper is called")
        {
            char_data* player1 = new char_data();
            player1->name = "player 1";
            char_data* player2 = new char_data();
            player2->name = "player 2";

            room_index_data* room = new room_index_data();

            player1->in_room = room;
            player1->next_in_room = room->people;
            room->people = player1;
            player2->in_room = room;
            player2->next_in_room = room->people;
            room->people = player2;

             auto result = find_keeper(player1);
            THEN("find_keeper should return null")
            {
                REQUIRE(result == NULL);
            }
        }
    }
}