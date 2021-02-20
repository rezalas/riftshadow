#include <string.h>
#include <stddef.h>
#include "catch.hpp"
#include "../code/act_obj.h"
#include "../code/merc.h"
#include "../code/handler.h"

void TestHelperAddPlayerToRoom(char_data *player, room_index_data *room)
{
    player->in_room = room;
    player->next_in_room = room->people;
    room->people = player;
}

void TestHelperSetPlayerStats(char_data *player, sh_int value)
{
    player->perm_stat[STAT_STR] = value;
    player->perm_stat[STAT_INT] = value;
    player->perm_stat[STAT_WIS] = value;
    player->perm_stat[STAT_DEX] = value;
    player->perm_stat[STAT_CON] = value;

    player->race = 2; // 2 is human
    //player->SetClass(1);
    player->pIndexData->SetClass(1);
    player->level = 1;
}

char_data* TestHelperCreateNPC()
{
	auto npc = new char_data();
	SET_BIT(npc->act, ACT_IS_NPC);
	npc->pIndexData = new mob_index_data();
	return npc;
}

char_data* TestHelperCreateNPCWithShop()
{
	auto npc = TestHelperCreateNPC();
	npc->pIndexData->pShop = new shop_data();
	return npc;
}


char_data* TestHelperCreatePlayer(char *name)
{
	auto player = new char_data();
	player->name = name;
	auto dnew = new descriptor_data();
	dnew->showstr_head = NULL;
	dnew->showstr_point = NULL;
	dnew->outsize = 2000;
	dnew->pEdit = NULL;	  /* OLC */
	dnew->pString = NULL; /* OLC */
	dnew->editor = 0;	  /* OLC */
	dnew->outbuf = new char[dnew->outsize];
	dnew->outtop = 0;	
	player->desc = dnew;

	return player;
}

room_index_data* TestHelperCreateRoom()
{
	auto room = new room_index_data();
	room->area = new area_data();
	return room;
}

SCENARIO("testing finding shop keepers","[find_keeper]")
{
    GIVEN("a player character in a room")
    {
        WHEN("find_keeper is called while alone")
        {
            char_data* player = new char_data();
            room_index_data* room = new room_index_data();
            TestHelperAddPlayerToRoom(player, room);
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
            char_data* player1 = TestHelperCreatePlayer("player 1");
            char_data* npc =  TestHelperCreateNPC();
            room_index_data* room = TestHelperCreateRoom();

            TestHelperAddPlayerToRoom(npc, room);
            TestHelperAddPlayerToRoom(player1, room);

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
            char_data* player1 = TestHelperCreatePlayer("player 1");
            // setup NPC with flag and shop index
            char_data* npc = TestHelperCreateNPCWithShop();
			// setup room for player and npc
            room_index_data* room = TestHelperCreateRoom();
            
			// add both to the room
            TestHelperAddPlayerToRoom(npc, room);
            TestHelperAddPlayerToRoom(player1, room);

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
            char_data* player1 = TestHelperCreatePlayer("player 1");
            char_data* player2 = TestHelperCreatePlayer("player 2");

            room_index_data* room = TestHelperCreateRoom();

            TestHelperAddPlayerToRoom(player1, room);
            TestHelperAddPlayerToRoom(player2, room);

             auto result = find_keeper(player1);
            THEN("find_keeper should return null")
            {
                REQUIRE(result == NULL);
            }
        }
    }
}
SCENARIO("testing selling to merchants", "[do_sell]")
{
	GIVEN("a player calls sell")
	{
		WHEN("no arguments are provided")
		{
			char *arg = "";	
			char_data *player1 = TestHelperCreatePlayer("player 1"); // create a player 
			auto npc = TestHelperCreateNPCWithShop(); // create a shopkeeper	
			room_index_data* room = TestHelperCreateRoom(); // build the shop floor
            // add both to a shop
            TestHelperAddPlayerToRoom(npc, room);
            TestHelperAddPlayerToRoom(player1, room);

			do_sell(player1, arg);
			auto expected = "Sell what?";
			auto actual = player1->desc->outbuf;
			THEN("do_sell should return after notifying the player and rejecting the command.")
			{
				REQUIRE(strstr(actual,expected) != NULL);
			}
		}

		WHEN("an argument is provided but no merchants are present")
		{
			char *arg = "broken_spoon";
			auto player = TestHelperCreatePlayer("player 1");
			auto room = TestHelperCreateRoom();
			TestHelperAddPlayerToRoom(player, room);
			do_sell(player, arg);
			auto expected = "Sell to whom? Yourself?";
			auto actual = player->desc->outbuf;

			THEN("do_sell should return after notifying the player and rejecting the command.")
			{
				REQUIRE(strstr(actual, expected) != NULL);
			}
		}

		WHEN("an argument is provided but the item is not posessed")
		{
			char *arg = "broken_spoon";
			auto player = TestHelperCreatePlayer("player 1");
			auto keeper = TestHelperCreateNPCWithShop();
			auto room = TestHelperCreateRoom();
			TestHelperAddPlayerToRoom(player, room);
			TestHelperAddPlayerToRoom(keeper, room);
			
			do_sell(player, arg);
			auto expected = "You cannot sell what you do not have.";
			auto actual = player->desc->outbuf;

			THEN("do_sell should return after notifying the player and rejecting the command.")
			{
				REQUIRE(strstr(actual, expected) != NULL);
			}
		}
	}
}

// SCENARIO("testing looting containers", "[do_get]")
// {
    
//     GIVEN("a player character looting a generic container")
//     {
//         room_index_data* room = new room_index_data();
//         room->area = new area_data();
//         room->light = 3;

//         obj_data *container = new OBJ_DATA();
//         container->name = "sut";
//         container->item_type = ITEM_CONTAINER;
//         container->pIndexData = new obj_index_data();

//         char_data* player1 = new char_data();
//         player1->name = "player 1";
//         player1->pcdata = new PC_DATA();
//         TestHelperSetPlayerStats(player1, 20);

//         obj_data* itemToGet = new obj_data();
//         itemToGet->name = "bobble";
//         SET_BIT(itemToGet->wear_flags, ITEM_TAKE);
        
//         TestHelperAddPlayerToRoom(player1, room);
//         obj_to_room(container, room);
//         obj_to_obj(itemToGet, container);

//         WHEN("do_get is passed ' bobble from sut'")
//         {
//             do_get(player1, "bobble from sut");
//             THEN("the item should move to the players inventory")
//             {
//                 REQUIRE(get_obj_carry(player1, "bobble",player1) == itemToGet);
//             }
//         }
//     }
// }