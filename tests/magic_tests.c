#include <cstring>

#include "catch.hpp"
#include "../code/magic.h"
#include "../code/merc.h"
#include "../code/enums.h"
#include "../code/const.h"
#include "../code/handler.h"
#include "../code/characterClasses/sorcerer.h"
#include "../code/entity/room_index_data.h"
#include "world_fixture.h"

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


//
// rust, and the worn slots it was silently skipping.
//
// The spell picks a penalty from the slot the piece is worn in. Two of the
// slot groups fell through to the default label, which is a `continue`, so the
// arms, hands, feet, legs, about and body groups were skipped entirely: no
// affect on the item and neither of the two messages. Only the finger, neck,
// wrist, waist and head groups ever rusted.
//
// Both the save and the per-item roll are random, so the cast is repeated until
// it lands. The reachable slot is the control: it says the loop is capable of
// rusting something at all, which is what makes "the body piece was rusted"
// worth reading.
//

static bool RustReachesSlot(WearLocation wearLoc, int attempts)
{
	TestWorld::WireSkillNumbers();

	for (int attempt = 0; attempt < attempts; attempt++)
	{
		TestWorld world;
		auto room = world.CreateRoom();
		auto caster = world.CreatePlayer("Caster", room);
		auto victim = world.CreatePlayer("Victim", room);
		auto armour = world.CreateItem("hauberk", "an iron hauberk");

		armour->material = (char *)"iron";
		obj_to_char(armour, victim);
		armour->wear_loc = wearLoc;

		TestWorld::ClearOutput(caster);
		TestWorld::ClearOutput(victim);

		spell_rust(gsn_rust, 50, caster, victim, CastMode::Spell);

		if (armour->affected.empty())
			continue;

		// Both messages, because the missing break suppressed the affect and
		// the messages together, and a fix that restored only the affect would
		// still leave both sides of the cast with nothing to see.
		REQUIRE(TestWorld::Heard(victim, "covered with a thick patina of rust"));
		REQUIRE(TestWorld::Heard(caster, "covered with a thick patina of rust"));

		// The penalty is what the slot selects, and it is the value the
		// fall-through skipped past.
		REQUIRE(armour->affected.front().location == obj_location(APPLY_DEX));
		REQUIRE(armour->affected.front().modifier == (wearLoc == WEAR_BODY ? -4 : -2));

		return true;
	}

	return false;
}

SCENARIO("rust reaches every armour slot it names", "[spell_rust]")
{
	GIVEN("a victim wearing a metal helmet, a slot that always rusted")
	{
		WHEN("rust is cast until it takes hold")
		{
			THEN("the helmet rusts")
			{
				REQUIRE(RustReachesSlot(WEAR_HEAD, 200));
			}
		}
	}

	GIVEN("a victim wearing metal body armour")
	{
		WHEN("rust is cast until it takes hold")
		{
			THEN("the body armour rusts as well")
			{
				REQUIRE(RustReachesSlot(WEAR_BODY, 200));
			}
		}
	}
}
