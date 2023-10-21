#include "catch.hpp"
#include "../code/act_obj.h"
#include "../code/merc.h"
#include "../code/handler.h"
#include "../code/const.h"
#include "../code/lookup.h"
#include "../code/tables.h"
#include "../code/db.h"
//#include "../code/bootup.h"


void TestHelperLoadGSNList()
{
	int sn;

	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
		if ( skill_table[sn].pgsn != nullptr)
			*skill_table[sn].pgsn = sn;
	}
}

void TestHelperAddPlayerToRoom(char_data *player, room_index_data *room)
{
    player->in_room = room;
    player->next_in_room = room->people;
    room->people = player;
}

void TestHelperSetPlayerStats(char_data *player, short value)
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

char_data* TestHelperCreateNPCWithShop(int setGold = 1000)
{
	auto npc = TestHelperCreateNPC();
	npc->pIndexData->pShop = new shop_data();
	npc->gold = setGold;
	return npc;
}

void TestHelperLoadCClass()
{
	auto cclass = new CClass();
	cclass->name = "ANTI_PALADIN";
	cclass->index = 5;
	CClass::first = cclass;
	CClass::first->name;
}

char_data* TestHelperCreatePlayer(char *name, obj_data *item = nullptr)
{
	auto player = new char_data();
	player->name = name;
	auto dnew = new descriptor_data();
	player->pcdata = new pc_data();
	TestHelperLoadCClass();
	player->SetClass(5);
	player->level = 51;
	dnew->showstr_head = nullptr;
	dnew->showstr_point = nullptr;
	dnew->outsize = 2000;
	dnew->pEdit = nullptr;	  /* OLC */
	dnew->pString = nullptr; /* OLC */
	dnew->editor = 0;	  /* OLC */
	dnew->outbuf = new char[dnew->outsize];
	dnew->outtop = 0;	
	player->desc = dnew;

	if(item != nullptr)
		obj_to_char(item, player);
	

	return player;
}

void TestHelperAddPlayerToCabal(char_data* player, char *cabalName = "guild")
{
	auto cabalCode = cabal_lookup(cabalName);
	player->cabal = cabalCode;
	player->pcdata->cabal_level = cabal_table[cabalCode].start_level;
	cabal_members[cabalCode]++;
}

room_index_data* TestHelperCreateRoom()
{
	auto room = new room_index_data();
	room->light = 3; // classifies the room as 'not dark'
	room->area = new area_data();
	
	return room;
}

obj_data* TestHelperCreateItem(char *itemName = "broken_lamp", int cost = 0)
{
	auto item = new obj_data();
	item->name = itemName;
	item->cost = cost;
	item->wear_loc = -1;
	item->description = "A magically generated item for testing";

	return item;
}

SCENARIO("testing finding shop keepers","[find_keeper]")
{
	TestHelperLoadGSNList();

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
                REQUIRE(result == nullptr);
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
                REQUIRE(result == nullptr);
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
                REQUIRE(result == nullptr);
            }
        }
    }
}
SCENARIO("testing selling to merchants", "[do_sell]")
{
	TestHelperLoadGSNList();

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
				REQUIRE(strstr(actual,expected) != nullptr);
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
				REQUIRE(strstr(actual, expected) != nullptr);
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
				REQUIRE(strstr(actual, expected) != nullptr);
			}
		}

		WHEN("an argument is provided, but the item has no value")
		{
			auto item = TestHelperCreateItem("broken_spoon");
			auto player = TestHelperCreatePlayer("player 1", item);
			auto keeper = TestHelperCreateNPCWithShop();
			auto room = TestHelperCreateRoom();
			TestHelperAddPlayerToRoom(player, room);
			TestHelperAddPlayerToRoom(keeper, room);
			
			do_sell(player, item->name);

			THEN("it should not be sold to the shopkeeper")
			{
				REQUIRE(player->carrying == item);
				REQUIRE(player->gold == 0);
				REQUIRE(keeper->carrying != item);
			}
		}

		WHEN("an item of value is provided, but the character is in horde")
		{
			auto item = TestHelperCreateItem("broken_spoon", 100);
			auto player = TestHelperCreatePlayer("player 1", item);
			TestHelperAddPlayerToCabal(player, "horde");
			auto keeper = TestHelperCreateNPCWithShop();
			auto room = TestHelperCreateRoom();
			TestHelperAddPlayerToRoom(player, room);
			TestHelperAddPlayerToRoom(keeper, room);
			
			do_sell(player, item->name);
			auto actual = player->desc->outbuf;
			auto expected = "You would never trade for coin when coin can be taken!";
			THEN("it should not be sold to the shopkeeper")
			{
				REQUIRE(strstr(actual, expected) != nullptr);
				REQUIRE(player->carrying == item);
				REQUIRE(player->gold == 0);
				REQUIRE(keeper->carrying != item);
			}
		}

		WHEN("an item of value is provided, but the char doesn't have the haggle skill")
		{
			auto item = TestHelperCreateItem("broken_spoon", 100);
			auto player = TestHelperCreatePlayer("player 1", item);
			auto keeper = TestHelperCreateNPCWithShop();
			auto room = TestHelperCreateRoom();
			TestHelperAddPlayerToRoom(player, room);
			TestHelperAddPlayerToRoom(keeper, room);
			
			do_sell(player, item->name);
			auto expected = "The shopkeeper haggles with you about the price - you seem poorly equipped for this.";
			auto actual = player->desc->outbuf;

			THEN("it should be sold to the shopkeeper at an unskilled multiplier")
			{
				REQUIRE(strstr(actual,expected) != nullptr);
				REQUIRE(player->carrying != item);
				REQUIRE(player->gold > 0);
				REQUIRE(keeper->carrying == item);
			}
		}

		WHEN("an item of value is provided and the char has the haggle skill")
		{
			auto item = TestHelperCreateItem("broken_spoon", 100);
			auto player = TestHelperCreatePlayer("player 1", item);
			player->pcdata->learned[222] = 100;

			auto keeper = TestHelperCreateNPCWithShop();
			auto room = TestHelperCreateRoom();
			TestHelperAddPlayerToRoom(player, room);
			TestHelperAddPlayerToRoom(keeper, room);
			
			do_sell(player, item->name);
			auto expected = "The shopkeeper reluctantly concedes to your final price.";
			auto actual = player->desc->outbuf;

			THEN("it should be sold to the shopkeeper at a skilled multiplier")
			{
				REQUIRE(strstr(actual,expected) != nullptr);
				REQUIRE(player->carrying != item);
				REQUIRE(player->gold > 0);
				REQUIRE(keeper->carrying == item);
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