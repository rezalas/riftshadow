#include "catch.hpp"
#include "../code/mprog.h"
#include "../code/handler.h"
#include "../code/entity/char_data.h"
#include "../code/entity/obj_data.h"
#include "../code/entity/room_index_data.h"
#include "../code/entity/area_data.h"
#include "../code/enums.h"
#include "../code/mud.h"

static room_index_data* CreateTestRoom()
{
	auto room = new room_index_data();
	room->light = 3;
	room->area = new area_data();

	return room;
}

static obj_data* CreateTestItem()
{
	auto item = new obj_data();
	item->name = "test_trinket";
	item->description = "A test item";
	item->wear_loc = -1;

	return item;
}

static char_data* CreateTestChar(char *name, room_index_data *room)
{
	auto ch = new char_data();
	ch->name = name;
	char_to_room(ch, room);

	return ch;
}

// mprog_give and mprog_drop queue the unlink and the relink as two separate
// events on the same tick, so their relative order decides which inventory the
// object ends up in. Both were inverted: obj_to_char/obj_to_room repoint
// carried_by, so the trailing obj_from_char acted on the destination instead of
// the mob, leaving the object linked in two lists at once.
SCENARIO("Testing a mob handing an object to a character", "[mprog_give]")
{
	GIVEN("a mob carrying an object, and a character in the same room")
	{
		auto room = CreateTestRoom();
		auto mob = CreateTestChar((char*)"Mob", room);
		auto ch = CreateTestChar((char*)"Player", room);
		auto obj = CreateTestItem();

		obj_to_char(obj, mob);

		REQUIRE(mob->carrying == obj);

		WHEN("mprog_give queues the handover and the queue is processed")
		{
			mprog_give(1, nullptr, obj, mob, ch);
			RS.Queue.ProcessQueue();

			THEN("the object leaves the mob and is carried by the character")
			{
				REQUIRE(obj->carried_by == ch);
				REQUIRE(ch->carrying == obj);
				REQUIRE(mob->carrying == nullptr);
			}
		}
	}
}

SCENARIO("Testing a mob dropping an object", "[mprog_drop]")
{
	GIVEN("a mob carrying an object")
	{
		auto room = CreateTestRoom();
		auto mob = CreateTestChar((char*)"Mob", room);
		auto obj = CreateTestItem();

		obj_to_char(obj, mob);

		REQUIRE(mob->carrying == obj);

		WHEN("mprog_drop queues the drop and the queue is processed")
		{
			mprog_drop(1, nullptr, obj, mob, mob);
			RS.Queue.ProcessQueue();

			THEN("the object leaves the mob and lies in the room")
			{
				REQUIRE(obj->carried_by == nullptr);
				REQUIRE(obj->in_room == room);
				REQUIRE(room->contents == obj);
				REQUIRE(mob->carrying == nullptr);
			}
		}
	}
}
