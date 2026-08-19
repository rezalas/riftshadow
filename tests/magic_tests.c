#include <cstring>

#include "catch.hpp"
#include "../code/magic.h"
#include "../code/merc.h"
#include "../code/enums.h"
#include "../code/const.h"
#include "../code/handler.h"
#include "../code/characterClasses/sorcerer.h"
#include "../code/entity/room_index_data.h"

char* substr(char* arr, int begin, int len = 0)
{	
    char* res = new char[len + 1];
    for (int i = 0; i < len; i++)
        res[i] = *(arr + begin + i);
    res[len] = 0;
    return res;
}

char* AllUpper(char* arr)
{
	auto len = strlen(arr);
	char* res = new char[len + 1];
	for(size_t i = 0; i < len; i++)
	{
		if(arr[i] == '\0')
			break;
		res[i] = toupper(arr[i]);
	}
	res[len] = 0;
	return res;
}
char* AllLower(char* arr)
{
	const size_t len = strlen(arr);
	for(size_t i = 0; i < len; i++)
	{
		if(arr[i] == '\0')
			break;
		arr[i] = tolower(arr[i]);
	}
	return arr;
}

SCENARIO("testing skill lookup","[skill_lookup]")
{
	GIVEN("a null name")
	{
		WHEN("skill_lookup is called")
		{
			auto expected = -1;
			auto actual = skill_lookup(nullptr);
			THEN("it should return negative 1")
			{
				REQUIRE(actual == expected);
			}
		}
	}
	GIVEN("an empty name")
	{
		WHEN("skill_lookup is called")
		{
			auto expected = -1;
			auto actual = skill_lookup("");
			THEN("it should return negative 1")
			{
				REQUIRE(actual == expected);
			}
		}
	}
	GIVEN("an invalid name")
	{
		WHEN("skill_lookup is called")
		{
			auto expected = -1;
			auto actual = skill_lookup("Jimmy dean sausage");
			THEN("it should return negative 1")
			{
				REQUIRE(actual == expected);
			}
		}
	}
	GIVEN("a valid name uppercase")
	{
		WHEN("skill_lookup is called")
		{
			// skill_table should always have at least 6 skills. this should be safe
			auto expected = 5;
			auto skillName = AllUpper(skill_table[expected].name); 
			auto actual = skill_lookup(skillName);
			THEN("it should return the expected index")
			{
				REQUIRE(actual == expected);
			}
		}
	}
	GIVEN("an valid name mixed case")
	{
		WHEN("skill_lookup is called")
		{
			// skill_table should always have at least 6 skills. this should be safe
			auto expected = 5;
			auto skillName = skill_table[expected].name; 
			auto firstHalf = AllLower(substr(skillName,0,3));
			auto secondHalf = AllUpper(substr(skillName,3,(sizeof(skillName) - 3)));
			std::string combinedSkillName = std::string(firstHalf) + std::string(secondHalf);
			auto actual = skill_lookup(combinedSkillName.c_str());
			THEN("it should return the expected index")
			{
				REQUIRE(actual == expected);
			}
		}
	}
}
SCENARIO("a conflagration pulses in a room with no exits", "[conflagration_pulse]")
{
	GIVEN("a burning, empty room whose six exits are all null")
	{
		auto room = new room_index_data();
		room->sector_type = SECT_CONFLAGRATION;
		room->people = nullptr;

		for (auto door = 0; door < MAX_DIR; door++)
			room->exit[door] = nullptr;

		ROOM_AFFECT_DATA af;
		init_affect_room(&af);

		WHEN("the pulse runs enough times to clear its one-in-three gate")
		{
			// The spread step picks a random exit. Choosing by rejection sampling
			// never terminates when there is nothing to choose from, so the point
			// of this case is that the calls return at all.
			for (auto i = 0; i < 200; i++)
				conflagration_pulse(room, &af);

			THEN("every call returns and the room does not spread")
			{
				REQUIRE(room->sector_type == SECT_CONFLAGRATION);
			}
		}

		delete room;
	}
}
