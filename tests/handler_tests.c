#include "catch.hpp"
#include "../code/merc.h"
#include "../code/handler.h"
#include "../code/act_comm.h"
#include "../code/entity/char_data.h"
#include "../code/entity/obj_data.h"
#include "../code/entity/room_index_data.h"
#include "../code/entity/area_data.h"
#include "../code/entity/mob_index_data.h"
#include "../code/macros.h"
#include "../code/entity/handles.h"
#include "../code/enums.h"
#include "../code/db.h"
#include "../code/recycle.h"
#include "../code/pstring.h"
#include "../code/mspec.h"
#include "../code/lookup.h"
#include "../code/tables.h"
#include "world_fixture.h"

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
			int test = race_lookup(nullptr);

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
			int test = liq_lookup(nullptr);

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
			int test = weapon_lookup(nullptr);

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
			int test = weapon_type_lookup(nullptr);

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
			auto test = item_lookup("food");

			THEN("then it should return the correct item type")
			{
				REQUIRE(test == ITEM_FOOD);
			}
		}

		WHEN("item_lookup function is called with the item in uppercase")
		{
			auto test = item_lookup("FOOD");

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
			auto test = item_lookup("bar");

			THEN("then it should answer with nothing")
			{
				REQUIRE_FALSE(test.has_value());
			}
		}

		WHEN("item_lookup function is called with a valid item but with extra characters")
		{
			auto test = item_lookup("food  ");

			THEN("then it should answer with nothing")
			{
				REQUIRE_FALSE(test.has_value());
			}
		}

		WHEN("item_lookup function is called with special characters")
		{
			auto test = item_lookup("ÿàÞ");

			THEN("then it should answer with nothing")
			{
				REQUIRE_FALSE(test.has_value());
			}
		}
	}

	GIVEN("an uninitialized item")
	{
		WHEN("item_lookup function is called")
		{
			auto test = item_lookup(nullptr);

			THEN("then it should answer with nothing")
			{
				REQUIRE_FALSE(test.has_value());
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
			char* test = item_name_lookup(static_cast<ItemType>(-18));

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
			int test = attack_lookup(nullptr);

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
			long test = wiznet_lookup(nullptr);

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

// Characterization tests for the char-affect list handlers (affect_to_char,
// affect_remove, affect_strip). These pin the observable add/find/remove/strip
// contract via the stable API (is_affected / affect_find) so it is provably
// preserved when ch->affected changes container.
SCENARIO("affect_to_char adds a char affect", "[affect_to_char]")
{
	GIVEN("a fresh character")
	{
		auto ch = new char_data();
		ch->name = "tester";

		WHEN("an affect is given to the character")
		{
			AFFECT_DATA af;
			init_affect(&af);
			af.type = 100;
			af.where = TO_AFFECTS;
			af.level = 7;
			af.duration = 5;
			af.modifier = 3;
			affect_to_char(ch, &af);

			THEN("the character reports being affected by it")
			{
				REQUIRE(is_affected(ch, 100) == true);
				REQUIRE(is_affected(ch, 999) == false);
			}

			THEN("the stored affect carries the same fields")
			{
				AFFECT_DATA *found = affect_find(ch->affected, 100);
				REQUIRE(found != nullptr);
				REQUIRE(found->level == 7);
				REQUIRE(found->duration == 5);
				REQUIRE(found->modifier == 3);
			}

			THEN("init_duration is seeded from duration")
			{
				AFFECT_DATA *found = affect_find(ch->affected, 100);
				REQUIRE(found != nullptr);
				REQUIRE(found->init_duration == 5);
			}
		}
	}
}

SCENARIO("affect_remove removes a char affect", "[affect_remove]")
{
	GIVEN("a character with a single affect")
	{
		auto ch = new char_data();
		ch->name = "tester";

		AFFECT_DATA af;
		init_affect(&af);
		af.type = 100;
		af.where = TO_AFFECTS;
		af.duration = 5;
		affect_to_char(ch, &af);

		WHEN("that affect is removed")
		{
			affect_remove(ch, affect_find(ch->affected, 100));

			THEN("the character is no longer affected")
			{
				REQUIRE(is_affected(ch, 100) == false);
				REQUIRE(affect_find(ch->affected, 100) == nullptr);
			}
		}
	}
}

SCENARIO("removing one of several char affects preserves the rest", "[affect_remove]")
{
	GIVEN("a character with three distinct affects")
	{
		auto ch = new char_data();
		ch->name = "tester";

		for (int type : {100, 101, 102})
		{
			AFFECT_DATA af;
			init_affect(&af);
			af.type = type;
			af.where = TO_AFFECTS;
			af.duration = 5;
			affect_to_char(ch, &af);
		}

		WHEN("the middle-added affect is removed")
		{
			affect_remove(ch, affect_find(ch->affected, 101));

			THEN("only that affect is gone")
			{
				REQUIRE(is_affected(ch, 100) == true);
				REQUIRE(is_affected(ch, 101) == false);
				REQUIRE(is_affected(ch, 102) == true);
			}
		}
	}
}

SCENARIO("affect_strip removes all affects of a type but leaves others", "[affect_strip]")
{
	GIVEN("a character affected twice by one type and once by another")
	{
		auto ch = new char_data();
		ch->name = "tester";

		for (int type : {100, 100, 101})
		{
			AFFECT_DATA af;
			init_affect(&af);
			af.type = type;
			af.where = TO_AFFECTS;
			af.duration = 5;
			affect_to_char(ch, &af);
		}

		WHEN("that type is stripped")
		{
			affect_strip(ch, 100);

			THEN("every instance of it is gone and the other type remains")
			{
				REQUIRE(is_affected(ch, 100) == false);
				REQUIRE(affect_find(ch->affected, 100) == nullptr);
				REQUIRE(is_affected(ch, 101) == true);
			}
		}
	}
}

//
// The entity slot maps. These exercise the real new_char/free_char and
// new_obj/free_obj rather than a stand-in, because the property that matters
// is the interaction with the free lists: those functions hand the same
// address back out, and the whole point of a handle is that reuse of an
// address must not resurrect a reference to the thing that used to live there.
//
// (Handle and SlotMap are covered on their own terms in handle_tests.c; these
// are about the wiring.)
//

SCENARIO("new_char registers a character and free_char expires it", "[handles]")
{
	GIVEN("a character from new_char")
	{
		CHAR_DATA *ch = new_char();

		THEN("it has a live self handle that derefs back to itself")
		{
			REQUIRE(!ch->self.IsNull());
			REQUIRE(Deref(ch->self) == ch);
		}

		WHEN("it is freed")
		{
			Handle<CHAR_DATA> stale = ch->self;
			free_char(ch);

			THEN("handles taken before the free no longer resolve")
			{
				REQUIRE(Deref(stale) == nullptr);
			}
		}
	}
}

SCENARIO("a recycled character slot does not resurrect an old handle", "[handles]")
{
	GIVEN("a character that has been destroyed")
	{
		CHAR_DATA *first = new_char();
		Handle<CHAR_DATA> stale = first->self;

		free_char(first);

		WHEN("a new character is allocated, reusing that slot")
		{
			// The address is no longer what gets recycled, because a destroyed
			// character is really deleted now. But the slot map still hands the
			// freed slot to the next Add, and the slot is what a handle names.
			// Holding SlotCount steady across the allocation is what proves the
			// reuse happened, and so keeps the assertions below from passing
			// for the trivial reason.
			std::size_t slotsBefore = charHandles.SlotCount();

			CHAR_DATA *second = new_char();

			THEN("the old handle stays expired even though the slot matches")
			{
				REQUIRE(charHandles.SlotCount() == slotsBefore);

				REQUIRE(Deref(stale) == nullptr);
				REQUIRE(stale != second->self);
				REQUIRE(Deref(second->self) == second);
			}

			free_char(second);
		}
	}
}

// Freeing the same character twice used to be spelled with a raw pointer, and
// the free list is what made that legal to write: the struct stayed mapped, so
// the second call could still read `ch->self`, find the slot retired and bail
// out. Now the first free really deletes, and reading `ch->self` afterwards is
// a use-after-free before any guard gets a chance to run.
//
// What survives is the property production actually relies on. Every one of the
// nine free_char call sites reaches its character through Deref of a handle, so
// once the first free expires the slot the second call is handed nullptr. The
// handle is the guard, and it is a guard against the double free rather than a
// recovery from it.
SCENARIO("freeing a character twice through its handle is a silent no-op", "[handles]")
{
	GIVEN("a character that has already been freed")
	{
		CHAR_DATA *ch = new_char();
		Handle<CHAR_DATA> handle = ch->self;

		free_char(ch);

		WHEN("the same handle is freed again, as the login paths do")
		{
			REQUIRE(Deref(handle) == nullptr);

			free_char(Deref(handle));

			THEN("nothing was destroyed twice and allocation carries on")
			{
				CHAR_DATA *first = new_char();
				CHAR_DATA *second = new_char();

				REQUIRE(first != second);
				REQUIRE(Deref(first->self) == first);
				REQUIRE(Deref(second->self) == second);

				free_char(first);
				free_char(second);
			}
		}
	}
}

SCENARIO("freeing a character the slot maps never issued does nothing", "[handles]")
{
	GIVEN("a stack-built character, as a test factory produces")
	{
		CHAR_DATA ch;

		WHEN("it is handed to free_char")
		{
			// It has a null self handle, so it never resolves to itself and the
			// guard rejects it. The old bool did the same by being false, but
			// only because value-initialisation happened to zero it. Here it is
			// the type's stated invariant rather than a coincidence.
			//
			// The guard matters more now than it did. Getting it wrong used to
			// mean adopting a stack address onto the free list. It now means
			// calling delete on one.
			free_char(&ch);

			THEN("it is neither destroyed nor handed back out")
			{
				CHAR_DATA *fresh = new_char();

				REQUIRE(fresh != &ch);

				free_char(fresh);
			}
		}
	}
}

SCENARIO("new_obj registers an object and free_obj expires it", "[handles]")
{
	GIVEN("an object from new_obj")
	{
		OBJ_DATA *obj = new_obj();

		THEN("it has a live self handle that derefs back to itself")
		{
			REQUIRE(!obj->self.IsNull());
			REQUIRE(Deref(obj->self) == obj);
		}

		WHEN("it is freed")
		{
			Handle<OBJ_DATA> stale = obj->self;
			free_obj(obj);

			THEN("handles taken before the free no longer resolve")
			{
				REQUIRE(Deref(stale) == nullptr);
			}
		}
	}
}

SCENARIO("an entity that never came from new_char is safe by default", "[handles]")
{
	GIVEN("a bare character built without new_char, as the tests do")
	{
		auto ch = new char_data();

		THEN("its self handle is null and derefs to nothing rather than to junk")
		{
			REQUIRE(ch->self.IsNull());
			REQUIRE(Deref(ch->self) == nullptr);
		}
	}
}

//
// ch->on -- what it means for the reference to go away.
//
// These pin the behavior of the two places that clear it today, because that
// behavior is the specification against which any change is judged.
//
// Worth stating plainly, because it is easy to get backwards: the clear inside
// obj_from_room is NOT lifetime bookkeeping. Nine of that function's ten
// callers leave the object entirely alive -- a player picking it up, wind
// blowing it to the next room, a portal being relocated. Only extract_obj is
// about death. So the rule it encodes is "the furniture left the room", which
// is a question about position, not about whether the object still exists.
//
// SittingOn is the single point that has to change if the field's type
// changes; every assertion below is written through it and stays as-is.
//

namespace
{
	OBJ_DATA *SittingOn(CHAR_DATA *ch)
	{
		return Deref(ch->on);
	}

	ROOM_INDEX_DATA *MakeRoomForSitting()
	{
		auto room = new room_index_data();
		room->area = new area_data();	// char_from_room decrements area->nplayer

		return room;
	}

	void PutCharInRoomForSitting(CHAR_DATA *ch, ROOM_INDEX_DATA *room)
	{
		ch->in_room = room;
		ch->next_in_room = room->people;
		room->people = ch;
	}

	void PutObjInRoomForSitting(OBJ_DATA *obj, ROOM_INDEX_DATA *room)
	{
		obj->in_room = room;
		obj->next_content = room->contents;
		room->contents = obj;
	}
}

SCENARIO("a character stops sitting on furniture that leaves the room", "[on]")
{
	GIVEN("two characters each sitting on a different object in one room")
	{
		auto room = MakeRoomForSitting();
		OBJ_DATA *chair = new_obj();
		OBJ_DATA *bench = new_obj();
		auto sitter = new char_data();
		auto bystander = new char_data();

		PutObjInRoomForSitting(chair, room);
		PutObjInRoomForSitting(bench, room);
		PutCharInRoomForSitting(sitter, room);
		PutCharInRoomForSitting(bystander, room);

		sitter->on = chair->self;
		bystander->on = bench->self;

		WHEN("the chair is taken out of the room but not destroyed")
		{
			obj_from_room(chair);

			THEN("whoever was on it no longer is")
			{
				REQUIRE(SittingOn(sitter) == nullptr);
			}

			THEN("someone on a different object is left alone")
			{
				REQUIRE(SittingOn(bystander) == bench);
			}

			THEN("the chair itself is still a perfectly live object")
			{
				// This is the whole reason the clear above cannot be replaced
				// by a liveness check: the object did not die, it moved.
				REQUIRE(Deref(chair->self) == chair);
			}
		}
	}
}

SCENARIO("the on-clear reaches only the room the object left", "[on]")
{
	GIVEN("a character in another room referring to the object")
	{
		auto room = MakeRoomForSitting();
		auto elsewhere = MakeRoomForSitting();
		OBJ_DATA *chair = new_obj();
		auto sitter = new char_data();
		auto distant = new char_data();

		PutObjInRoomForSitting(chair, room);
		PutCharInRoomForSitting(sitter, room);
		PutCharInRoomForSitting(distant, elsewhere);

		sitter->on = chair->self;
		distant->on = chair->self;

		WHEN("the chair leaves the room")
		{
			obj_from_room(chair);

			THEN("only the character in that room is cleared")
			{
				// Documents the mechanism's reach rather than a reachable game
				// state: the walk is over in_room->people, so it is scoped to
				// one room and is not a global sweep.
				REQUIRE(SittingOn(sitter) == nullptr);
				REQUIRE(SittingOn(distant) == chair);
			}
		}
	}
}

SCENARIO("a character stops sitting on anything when it leaves the room", "[on]")
{
	GIVEN("a character sitting on an object")
	{
		auto room = MakeRoomForSitting();
		OBJ_DATA *chair = new_obj();
		auto sitter = new char_data();

		PutObjInRoomForSitting(chair, room);
		PutCharInRoomForSitting(sitter, room);
		sitter->on = chair->self;

		WHEN("the character leaves")
		{
			char_from_room(sitter);

			THEN("it is no longer on anything")
			{
				REQUIRE(SittingOn(sitter) == nullptr);
			}
		}
	}
}

SCENARIO("a character is not left pointing at furniture that was destroyed", "[on]")
{
	GIVEN("a character sitting on an object")
	{
		auto room = MakeRoomForSitting();
		OBJ_DATA *chair = new_obj();
		auto sitter = new char_data();

		PutObjInRoomForSitting(chair, room);
		PutCharInRoomForSitting(sitter, room);
		sitter->on = chair->self;

		REQUIRE(SittingOn(sitter) == chair);

		WHEN("the object is destroyed without going through obj_from_room")
		{
			// obj_from_room clears the reference for anyone in the room, so it
			// hides this case. Two paths do not run it: extract_obj's not-found
			// bail-out, and an object freed while held. Those are the ones that
			// used to leave a pointer into a recycled object behind.
			free_obj(chair);

			THEN("the reference reads as nothing rather than as freed memory")
			{
				REQUIRE(SittingOn(sitter) == nullptr);
			}
		}
	}
}

//
// Deref(ch->last_fought) -- the mob hunt target, and what clears it.
//
// Unlike ch->on and ch->fighting, this field's scrub really is lifetime
// machinery. The clear that matters lives inside extract_char itself, under a
// "remove all tracking" comment, and extract_char is only ever reached when a
// character is leaving the world for good. Every other place that clears
// last_fought is a mob deciding to give up the hunt -- losing the trail,
// a peace command, the quarry fleeing -- and those all leave both characters
// alive.
//
// So this is the first field whose scrub loop clause can actually go. These
// tests pin the behavior it provides, so that the handle can be shown to
// provide the same thing.
//
// Tracking() is the single point that has to change if the field's type
// changes; every assertion below is written through it and stays as-is.
//

namespace
{
	CHAR_DATA *Tracking(CHAR_DATA *ch)
	{
		return Deref(ch->last_fought);
	}

	/// extract_char takes the NPC path -- it decrements pIndexData->count and
	/// skips the altar-room redirect that only applies to players -- so these
	/// are mobs, with the index data that path requires.
	CHAR_DATA *MakeTracker(ROOM_INDEX_DATA *room)
	{
		CHAR_DATA *mob = new_char();
		mob->pIndexData = new mob_index_data();
		SET_BIT(mob->act, ACT_IS_NPC);
		mob->hit = 100;

		mob->in_room = room;
		mob->next_in_room = room->people;
		room->people = mob;

		char_list.push_front(std::unique_ptr<CHAR_DATA>(mob));
		mob->globalNode = char_list.begin();

		return mob;
	}

	ROOM_INDEX_DATA *MakeTrackingRoom()
	{
		auto room = new room_index_data();
		room->area = new area_data();

		return room;
	}

	void ClearTrackers()
	{
		// Erasing the node is what destroys the character now, so clearing the
		// list is the whole teardown.
		char_list.clear();
	}
}

SCENARIO("extract_char clears every tracker's reference to the extracted character", "[last_fought]")
{
	GIVEN("two mobs hunting a third and one hunting someone else")
	{
		auto room = MakeTrackingRoom();
		CHAR_DATA *quarry = MakeTracker(room);
		CHAR_DATA *hunter = MakeTracker(room);
		CHAR_DATA *secondHunter = MakeTracker(room);
		CHAR_DATA *otherQuarry = MakeTracker(room);
		CHAR_DATA *unrelated = MakeTracker(room);

		hunter->last_fought = quarry->self;
		secondHunter->last_fought = quarry->self;
		unrelated->last_fought = otherQuarry->self;

		WHEN("the quarry is extracted from the world")
		{
			extract_char(quarry, true);

			THEN("nobody is left hunting it")
			{
				REQUIRE(Tracking(hunter) == nullptr);
				REQUIRE(Tracking(secondHunter) == nullptr);
			}

			THEN("a mob hunting somebody else is left alone")
			{
				REQUIRE(Tracking(unrelated) == otherQuarry);
			}
		}

		ClearTrackers();
	}
}

SCENARIO("a mob that gives up the hunt does not disturb the quarry", "[last_fought]")
{
	GIVEN("a mob hunting another")
	{
		auto room = MakeTrackingRoom();
		CHAR_DATA *quarry = MakeTracker(room);
		CHAR_DATA *hunter = MakeTracker(room);

		hunter->last_fought = quarry->self;

		WHEN("the hunter loses interest rather than the quarry dying")
		{
			// The other half of the field's contract, and the reason the
			// clears outside extract_char have to stay: this one says "stop
			// hunting", not "the quarry is gone".
			hunter->last_fought = nullptr;

			THEN("the hunter is no longer tracking but the quarry is untouched")
			{
				REQUIRE(Tracking(hunter) == nullptr);
				REQUIRE(Deref(quarry->self) == quarry);
			}
		}

		ClearTrackers();
	}
}

SCENARIO("a hunter's reference to a freed quarry reads as nothing", "[last_fought]")
{
	GIVEN("a mob hunting another")
	{
		auto room = MakeTrackingRoom();
		CHAR_DATA *quarry = MakeTracker(room);
		CHAR_DATA *hunter = MakeTracker(room);

		hunter->last_fought = quarry->self;

		REQUIRE(Tracking(hunter) == quarry);

		WHEN("the quarry is freed without extract_char's sweep running")
		{
			// extract_char used to clear this reference by walking char_list.
			// The paths that never reach that walk -- its own not-found
			// bail-out, say -- are the ones that used to leave a pointer into
			// a character struct the free list can hand straight back out.
			free_char(quarry);

			THEN("the reference reads as nothing rather than as recycled memory")
			{
				REQUIRE(Tracking(hunter) == nullptr);
			}
		}

		ClearTrackers();
	}
}

//
// Deref(ch->reply) -- who `reply` answers, and what clears it.
//
// Like last_fought, this field's expiry is genuine lifetime machinery: the
// clear is a char_list pass written inline in extract_char, and that pass does
// nothing else. The other two places that clear reply are game rules that
// leave both characters alive -- do_noreply, where a player deliberately
// closes their ears, and the wizard invis/incog commands, where cloaking drops
// your own pending reply.
//
// Replying() is the single point that has to change if the field's type
// changes; every assertion below is written through it and stays as-is.
//

namespace
{
	CHAR_DATA *Replying(CHAR_DATA *ch)
	{
		return Deref(ch->reply);
	}
}

SCENARIO("extract_char clears every pending reply aimed at the extracted character", "[reply]")
{
	GIVEN("two characters holding a reply to a third and one replying elsewhere")
	{
		auto room = MakeTrackingRoom();
		CHAR_DATA *speaker = MakeTracker(room);
		CHAR_DATA *listener = MakeTracker(room);
		CHAR_DATA *secondListener = MakeTracker(room);
		CHAR_DATA *someoneElse = MakeTracker(room);
		CHAR_DATA *unrelated = MakeTracker(room);

		listener->reply = speaker->self;
		secondListener->reply = speaker->self;
		unrelated->reply = someoneElse->self;

		WHEN("the speaker is extracted from the world")
		{
			extract_char(speaker, true);

			THEN("nobody is left holding a reply to it")
			{
				REQUIRE(Replying(listener) == nullptr);
				REQUIRE(Replying(secondListener) == nullptr);
			}

			THEN("a reply aimed at somebody else is left alone")
			{
				REQUIRE(Replying(unrelated) == someoneElse);
			}
		}

		ClearTrackers();
	}
}

SCENARIO("the reply sweep does not disturb the character being replied to", "[reply]")
{
	GIVEN("one character holding a reply to another")
	{
		auto room = MakeTrackingRoom();
		CHAR_DATA *speaker = MakeTracker(room);
		CHAR_DATA *listener = MakeTracker(room);

		listener->reply = speaker->self;

		WHEN("the listener drops the reply rather than the speaker dying")
		{
			// The other half of the contract, and the reason the clears
			// outside extract_char stay: this one says "stop answering them",
			// not "they are gone".
			listener->reply = nullptr;

			THEN("the listener has no reply but the speaker is untouched")
			{
				REQUIRE(Replying(listener) == nullptr);
				REQUIRE(Deref(speaker->self) == speaker);
			}
		}

		ClearTrackers();
	}
}

SCENARIO("a reply to a freed character reads as nothing", "[reply]")
{
	GIVEN("one character holding a reply to another")
	{
		auto room = MakeTrackingRoom();
		CHAR_DATA *speaker = MakeTracker(room);
		CHAR_DATA *listener = MakeTracker(room);

		listener->reply = speaker->self;

		REQUIRE(Replying(listener) == speaker);

		WHEN("the speaker is freed without extract_char's sweep running")
		{
			// That sweep is gone now, so this is the only thing standing
			// between `reply` and a pointer into a character struct the free
			// list can hand straight back out to the next new_char.
			free_char(speaker);

			THEN("the reference reads as nothing rather than as recycled memory")
			{
				REQUIRE(Replying(listener) == nullptr);
			}
		}

		ClearTrackers();
	}
}

//
// Deref(ch->master) / Deref(ch->leader) / Deref(ch->pet) -- the follower graph.
//
// These three differ from the fields above in that their clears live in
// helpers -- stop_follower, die_follower, nuke_pets -- rather than inline in
// extract_char, and those helpers are reached overwhelmingly for reasons that
// have nothing to do with anyone dying. die_follower alone has four callers
// and only one of them (extract_char) is a lifetime event; the others are a
// player setting nofollow, a sphere of plasma, and an item prog putting its
// owner to sleep. So the sweeps stay and the fields convert in place.
//
// Two behaviors here are deliberate game rules that a naive migration would
// "fix" into regressions, and both are pinned below:
//
//   * A follower whose leader dies is made its OWN leader, not leaderless.
//   * A follower secretly trailing via gsn_trail is exempt from the sweep and
//     stays attached. That exemption is correct while the master is alive --
//     the point of the skill is that the target cannot shake you -- and it is
//     what makes `master` outlive its target when the master is extracted.
//
// Master()/Leader()/Pet() are the single points that have to change if the
// field types change; every assertion below is written through them.
//

namespace
{
	CHAR_DATA *Master(CHAR_DATA *ch)
	{
		return Deref(ch->master);
	}

	CHAR_DATA *Leader(CHAR_DATA *ch)
	{
		return Deref(ch->leader);
	}

	CHAR_DATA *Pet(CHAR_DATA *ch)
	{
		return Deref(ch->pet);
	}

	/// gsn_trail and gsn_animate_dead are both zero in an unbooted test
	/// binary, and die_follower branches on both -- an affect added for one
	/// would answer for the other. Give trail a distinct value for the
	/// duration of a test.
	class ScopedTrailSkill
	{
	public:
		ScopedTrailSkill() : saved(gsn_trail) { gsn_trail = 500; }
		~ScopedTrailSkill() { gsn_trail = saved; }

	private:
		short saved;
	};

	void MakeTrailing(CHAR_DATA *ch)
	{
		AFFECT_DATA af;
		init_affect(&af);
		af.type = gsn_trail;
		af.where = TO_AFFECTS;
		af.duration = -1;
		affect_to_char(ch, &af);
	}
}

SCENARIO("die_follower detaches ordinary followers", "[follower]")
{
	GIVEN("a master with two followers and an unrelated pair")
	{
		auto room = MakeTrackingRoom();
		CHAR_DATA *master = MakeTracker(room);
		CHAR_DATA *followerOne = MakeTracker(room);
		CHAR_DATA *followerTwo = MakeTracker(room);
		CHAR_DATA *otherMaster = MakeTracker(room);
		CHAR_DATA *otherFollower = MakeTracker(room);

		followerOne->master = master->self;
		followerTwo->master = master->self;
		otherFollower->master = otherMaster->self;

		WHEN("the master sheds its followers without dying")
		{
			die_follower(master);

			THEN("its own followers are detached")
			{
				REQUIRE(Master(followerOne) == nullptr);
				REQUIRE(Master(followerTwo) == nullptr);
			}

			THEN("somebody else's follower is left alone")
			{
				REQUIRE(Master(otherFollower) == otherMaster);
			}

			THEN("the master is still perfectly alive")
			{
				REQUIRE(Deref(master->self) == master);
			}
		}

		ClearTrackers();
	}
}

SCENARIO("a follower whose leader dies becomes its own leader", "[follower]")
{
	GIVEN("a leader with a follower")
	{
		auto room = MakeTrackingRoom();
		CHAR_DATA *leader = MakeTracker(room);
		CHAR_DATA *follower = MakeTracker(room);

		follower->leader = leader->self;

		WHEN("the leader sheds its group")
		{
			die_follower(leader);

			THEN("the follower leads itself rather than being left leaderless")
			{
				// Intentional game logic, not an oversight: a character with
				// no group still leads one, consisting of itself. Turning this
				// into a null would be a gameplay regression.
				REQUIRE(Leader(follower) == follower);
			}
		}

		ClearTrackers();
	}
}

SCENARIO("a secret trailer survives the follower sweep", "[follower]")
{
	GIVEN("a master with one open follower and one trailing it secretly")
	{
		ScopedTrailSkill trailSkill;

		auto room = MakeTrackingRoom();
		CHAR_DATA *master = MakeTracker(room);
		CHAR_DATA *openFollower = MakeTracker(room);
		CHAR_DATA *trailer = MakeTracker(room);

		openFollower->master = master->self;
		trailer->master = master->self;
		MakeTrailing(trailer);

		REQUIRE(is_affected(trailer, gsn_trail) == true);

		WHEN("the master sheds its followers while staying alive")
		{
			die_follower(master);

			THEN("the open follower is detached but the trailer is not")
			{
				// The whole point of the skill: the target cannot shake
				// someone who is trailing them. This exemption is correct
				// here, and must survive the field becoming a handle.
				REQUIRE(Master(openFollower) == nullptr);
				REQUIRE(Master(trailer) == master);
			}
		}

		ClearTrackers();
	}
}

SCENARIO("stop_follower clears the master's pet when the pet stops following", "[follower]")
{
	GIVEN("a master with a pet")
	{
		auto room = MakeTrackingRoom();
		CHAR_DATA *master = MakeTracker(room);
		CHAR_DATA *pet = MakeTracker(room);

		pet->master = master->self;
		master->pet = pet->self;

		WHEN("the pet stops following")
		{
			stop_follower(pet);

			THEN("both ends of the relationship are cleared")
			{
				REQUIRE(Pet(master) == nullptr);
				REQUIRE(Master(pet) == nullptr);
			}

			THEN("the pet itself is still alive")
			{
				REQUIRE(Deref(pet->self) == pet);
			}
		}

		ClearTrackers();
	}
}

SCENARIO("a secret trailer does not outlive the master it was trailing", "[follower]")
{
	GIVEN("a master being trailed secretly")
	{
		ScopedTrailSkill trailSkill;

		auto room = MakeTrackingRoom();
		CHAR_DATA *master = MakeTracker(room);
		CHAR_DATA *trailer = MakeTracker(room);

		trailer->master = master->self;
		MakeTrailing(trailer);

		REQUIRE(Master(trailer) == master);

		WHEN("the master is extracted from the world")
		{
			// The sweep in die_follower deliberately skips trailers, and that
			// is right while the master is alive. It is not right once the
			// master has been freed: the reference used to survive into a
			// character struct that the free list hands straight back out, so
			// the trailer silently ended up attached to whoever was allocated
			// next.
			extract_char(master, true);

			THEN("its reference reads as nothing rather than as recycled memory")
			{
				REQUIRE(Master(trailer) == nullptr);
			}

			THEN("the slot really was reissued, which is what made this silent")
			{
				// The slot rather than the address: a destroyed character is really
				// deleted now, so reuse of the memory is an allocator accident. The
				// slot map does still recycle the freed slot, and the slot is what a
				// handle names, so this is what keeps the assertion below honest.
				std::size_t slotsBefore = charHandles.SlotCount();

				CHAR_DATA *newcomer = new_char();

				REQUIRE(charHandles.SlotCount() == slotsBefore);
				REQUIRE(Master(trailer) == nullptr);
			}
		}

		ClearTrackers();
	}
}

//
// Deref(ch->defending) / Deref(ch->analyzePC).
//
// Neither is cleared by extract_char. `defending` is scrubbed only on the
// voluntary-quit path (do_quit_new, just before it extracts the quitter) and
// by do_defend when a character stops defending; `analyzePC` is never cleared
// anywhere. So on any death that is not a quit -- which is most of them --
// both were left pointing into a character struct the free list can hand
// straight back out.
//
// That makes these the thinnest scrub story of any field in the graph: there
// is almost nothing to characterize, because there was almost nothing there.
// What is pinned below is the part that must not change (the reference is
// scoped to its own target and unrelated deaths leave it alone) and the part
// that does (it now expires).
//

namespace
{
	CHAR_DATA *Defending(CHAR_DATA *ch)
	{
		return Deref(ch->defending);
	}

	CHAR_DATA *Analyzing(CHAR_DATA *ch)
	{
		return Deref(ch->analyzePC);
	}
}

SCENARIO("an unrelated death leaves defending and analyzePC alone", "[defending]")
{
	GIVEN("a defender and an analyzer, plus a bystander who dies")
	{
		auto room = MakeTrackingRoom();
		CHAR_DATA *protector = MakeTracker(room);
		CHAR_DATA *ward = MakeTracker(room);
		CHAR_DATA *analyzer = MakeTracker(room);
		CHAR_DATA *studied = MakeTracker(room);
		CHAR_DATA *bystander = MakeTracker(room);

		protector->defending = ward->self;
		analyzer->analyzePC = studied->self;

		WHEN("somebody else entirely is extracted")
		{
			extract_char(bystander, true);

			THEN("both references are untouched")
			{
				REQUIRE(Defending(protector) == ward);
				REQUIRE(Analyzing(analyzer) == studied);
			}
		}

		ClearTrackers();
	}
}

SCENARIO("defending and analyzePC do not outlive their target", "[defending]")
{
	GIVEN("a defender and an analyzer pointed at one character")
	{
		auto room = MakeTrackingRoom();
		CHAR_DATA *protector = MakeTracker(room);
		CHAR_DATA *analyzer = MakeTracker(room);
		CHAR_DATA *target = MakeTracker(room);

		protector->defending = target->self;
		analyzer->analyzePC = target->self;

		REQUIRE(Defending(protector) == target);
		REQUIRE(Analyzing(analyzer) == target);

		WHEN("the target dies")
		{
			// extract_char clears neither field -- it never did -- so before
			// these became handles both references survived into a recycled
			// character struct. analyzePC is the quieter of the two: every one
			// of its uses is an identity compare, so a recycled address does
			// not crash, it silently matches the wrong character.
			extract_char(target, true);

			THEN("both references read as nothing")
			{
				REQUIRE(Defending(protector) == nullptr);
				REQUIRE(Analyzing(analyzer) == nullptr);
			}

			THEN("they stay nothing even once the slot is reissued")
			{
				// The slot rather than the address: a destroyed character is really
				// deleted now, so reuse of the memory is an allocator accident. The
				// slot map does still recycle the freed slot, and the slot is what a
				// handle names, so this is what keeps the assertion below honest.
				std::size_t slotsBefore = charHandles.SlotCount();

				CHAR_DATA *newcomer = new_char();

				REQUIRE(charHandles.SlotCount() == slotsBefore);
				REQUIRE(Defending(protector) == nullptr);
				REQUIRE(Analyzing(analyzer) == nullptr);
			}
		}

		ClearTrackers();
	}
}

//
// ch->hunting -- the mob-hunt-quarry link.
//
// The thinnest scrub story in the graph, thinner even than defending: this one
// is never cleared anywhere at all. There are two setters -- the vnum-3002 face
// sucker's greet prog and the sorcerer's aerial anchor -- and no clear on
// extract, on quit, or on any other path.
//
// Nothing crashes, because every reachable reader is an identity compare
// against a char_list walk rather than a dereference. That is what makes it
// quiet: a fresh character that lands on a dead one's address silently
// inherits the dead one's anchor and its face sucker. So the scenario that
// matters is the reissued address, not the freed read.
//

namespace
{
	// Both helpers exist so the identical test source compiles against the raw
	// pointer and against the handle -- when the field type flips, only these
	// two bodies move.
	CHAR_DATA *Hunting(CHAR_DATA *ch)
	{
		return Deref(ch->hunting);
	}

	void Hunt(CHAR_DATA *hunter, CHAR_DATA *quarry)
	{
		hunter->hunting = quarry->self;
	}
}

SCENARIO("an unrelated death leaves a mob's quarry alone", "[hunting]")
{
	GIVEN("two mobs hunting one character, and a bystander")
	{
		auto room = MakeTrackingRoom();
		CHAR_DATA *quarry = MakeTracker(room);
		CHAR_DATA *sucker = MakeTracker(room);
		CHAR_DATA *anchor = MakeTracker(room);
		CHAR_DATA *bystander = MakeTracker(room);

		Hunt(sucker, quarry);
		Hunt(anchor, quarry);

		WHEN("somebody else entirely is extracted")
		{
			extract_char(bystander, true);

			THEN("both hunters still name their quarry")
			{
				REQUIRE(Hunting(sucker) == quarry);
				REQUIRE(Hunting(anchor) == quarry);
			}
		}

		ClearTrackers();
	}
}

SCENARIO("a mob's quarry does not outlive it", "[hunting]")
{
	GIVEN("a mob hunting a character")
	{
		auto room = MakeTrackingRoom();
		CHAR_DATA *quarry = MakeTracker(room);
		CHAR_DATA *anchor = MakeTracker(room);

		Hunt(anchor, quarry);

		REQUIRE(Hunting(anchor) == quarry);

		WHEN("the quarry dies")
		{
			extract_char(quarry, true);

			THEN("the hunter names nothing")
			{
				REQUIRE(Hunting(anchor) == nullptr);
			}

			THEN("the next character to take the slot is not mistaken for it")
			{
				// The whole defect, in three lines. Every reachable reader of
				// this field walks char_list asking "is this the mob hunting
				// me?" and answers with ==, so the newcomer used to inherit the
				// dead character's anchor rather than crash on it.
				// The slot rather than the address: a destroyed character is really
				// deleted now, so reuse of the memory is an allocator accident. The
				// slot map does still recycle the freed slot, and the slot is what a
				// handle names, so this is what keeps the assertion below honest.
				std::size_t slotsBefore = charHandles.SlotCount();

				CHAR_DATA *newcomer = new_char();

				REQUIRE(charHandles.SlotCount() == slotsBefore);
				REQUIRE(Hunting(anchor) != newcomer);
				REQUIRE(Hunting(anchor) == nullptr);
			}
		}

		ClearTrackers();
	}
}

//
// d->character / d->original -- the descriptor half of the bidirectional pair.
//
// This is the only two-way link in the graph: a connected player is
// d->character == ch and ch->desc == d, and a switched immortal is
// d->character == the possessed mob, d->original == the immortal, with the
// mob holding the descriptor and the immortal's own desc null. Exactly one
// character points back at a descriptor at any moment, and it is always
// d->character.
//
// The two halves of the link are scrubbed very differently:
//
//   d->character  cleared inline right after the free, in five places --
//                 extract_char, and four login-screen rejections in comm.c
//                 that all read `free_char(d->character); d->character = 0`.
//   d->original   cleared NOWHERE except do_return, which is a wizard
//                 returning to their body, not a lifetime event.
//
// So `original` is the same shape as hunting and analyzePC -- except that it
// is not merely compared. check_playing dereferences `dold->original->true_name`
// for every descriptor in the list on every login attempt, so a stale original
// is read by the next person to connect.
//

namespace
{
	// Reader and setter both, so this file compiles unchanged on either side of
	// the type flip -- see the note on the hunting helpers above.
	CHAR_DATA *Body(DESCRIPTOR_DATA *d)
	{
		return Deref(d->character);
	}

	CHAR_DATA *OriginalBody(DESCRIPTOR_DATA *d)
	{
		return Deref(d->original);
	}

	void Connect(DESCRIPTOR_DATA *d, CHAR_DATA *ch)
	{
		d->character = ch->self;
		ch->desc = d->self;
	}

	/// The state do_switch leaves behind: the descriptor drives the mob, the
	/// immortal is parked in `original`, and only the mob points back.
	void Possess(DESCRIPTOR_DATA *d, CHAR_DATA *mob, CHAR_DATA *immortal)
	{
		d->character = mob->self;
		d->original = immortal->self;
		mob->desc = d->self;
		immortal->desc = nullptr;
	}
}

SCENARIO("a descriptor's body reference does not outlive the body", "[descriptor]")
{
	GIVEN("a character on a descriptor")
	{
		auto room = MakeTrackingRoom();
		CHAR_DATA *player = MakeTracker(room);
		DESCRIPTOR_DATA *d = new_descriptor();

		// new_descriptor resets a recycled descriptor with `*d = d_zero`, an
		// all-zero static. Both fields have to read as nothing after that, or a
		// reused descriptor would inherit the previous connection's body.
		REQUIRE(Body(d) == nullptr);
		REQUIRE(OriginalBody(d) == nullptr);

		Connect(d, player);

		REQUIRE(Body(d) == player);

		WHEN("the character is extracted")
		{
			extract_char(player, true);

			THEN("the descriptor names nobody")
			{
				// Today this holds because extract_char clears it by hand on
				// the line before free_char. That clear is inline in a lifetime
				// path, so it is deletable -- and this assertion is what proves
				// the deletion rather than merely surviving it.
				REQUIRE(Body(d) == nullptr);
			}

			THEN("the next character to take the slot is not adopted")
			{
				// The slot rather than the address: a destroyed character is really
				// deleted now, so reuse of the memory is an allocator accident. The
				// slot map does still recycle the freed slot, and the slot is what a
				// handle names, so this is what keeps the assertion below honest.
				std::size_t slotsBefore = charHandles.SlotCount();

				CHAR_DATA *newcomer = new_char();

				REQUIRE(charHandles.SlotCount() == slotsBefore);
				REQUIRE(Body(d) != newcomer);
			}
		}

		free_descriptor(d);
		ClearTrackers();
	}
}

SCENARIO("a switched immortal's original body does not outlive it", "[descriptor]")
{
	GIVEN("an immortal switched into a mob")
	{
		auto room = MakeTrackingRoom();
		CHAR_DATA *immortal = MakeTracker(room);
		CHAR_DATA *mob = MakeTracker(room);
		DESCRIPTOR_DATA *d = new_descriptor();

		Possess(d, mob, immortal);

		REQUIRE(Body(d) == mob);
		REQUIRE(OriginalBody(d) == immortal);

		WHEN("the original body is freed while the switch is still in effect")
		{
			// close_socket does exactly this -- `free_char(dclose->original ?
			// dclose->original : dclose->character)` -- and extract_char clears
			// only `character`, never `original`.
			free_char(immortal);

			THEN("the descriptor no longer names the original body")
			{
				REQUIRE(OriginalBody(d) == nullptr);
			}

			THEN("the next character to take the address is not adopted as the original")
			{
				// check_playing reads `dold->original->true_name` for every
				// descriptor on every login, so this one is dereferenced, not
				// just compared.
				// The slot rather than the address: a destroyed character is really
				// deleted now, so reuse of the memory is an allocator accident. The
				// slot map does still recycle the freed slot, and the slot is what a
				// handle names, so this is what keeps the assertion below honest.
				std::size_t slotsBefore = charHandles.SlotCount();

				CHAR_DATA *newcomer = new_char();

				REQUIRE(charHandles.SlotCount() == slotsBefore);
				REQUIRE(OriginalBody(d) != newcomer);
			}
		}

		free_descriptor(d);
		ClearTrackers();
	}
}

//
// ch->desc -- the character half of the bidirectional pair.
//
// The inverse of d->character, and scrubbed far less carefully than it. The
// only clear on a lifetime path is close_socket's, and it is guarded:
//
//     if (dclose->connected == CON_PLAYING && !merc_down)
//         ch->desc = nullptr;                       // link-dead, body lives on
//     else
//         free_char(dclose->original ? dclose->original : dclose->character);
//     ...
//     descriptor_list.erase(dclose->globalNode);
//
// Take the else branch with an immortal switched into a mob and it frees the
// immortal, never touches the mob, and then destroys the descriptor, leaving
// the mob naming one that is gone. The address is no longer handed straight back
// out, but the slot map does recycle the freed slot, so an index-only reference
// would still find the next connection there and the mob would be holding a live
// player's socket.
//
// Narrow: it needs a shutdown or a non-CON_PLAYING close with a switch in
// effect. It is the least severe dangle in the graph, not the worst.
//

namespace
{
	DESCRIPTOR_DATA *Connection(CHAR_DATA *ch)
	{
		return Deref(ch->desc);
	}

	void Attach(CHAR_DATA *ch, DESCRIPTOR_DATA *d)
	{
		ch->desc = d ? d->self : nullptr;
	}
}

SCENARIO("losing the link leaves the body in the world", "[chdesc]")
{
	GIVEN("a player on a descriptor")
	{
		auto room = MakeTrackingRoom();
		CHAR_DATA *player = MakeTracker(room);
		DESCRIPTOR_DATA *d = new_descriptor();

		Attach(player, d);

		REQUIRE(Connection(player) == d);

		WHEN("the socket closes but the body is left in the world")
		{
			// close_socket's CON_PLAYING branch. This is the half that must not
			// change: it means "the connection went away", not "the body did".
			// close_socket used to null ch->desc by hand just before freeing
			// the descriptor; this asserts the free alone is enough, which is
			// what makes that line deletable.
			free_descriptor(d);

			THEN("the body is still alive and simply has no connection")
			{
				REQUIRE(Connection(player) == nullptr);
				REQUIRE(Deref(player->self) == player);
			}
		}

		ClearTrackers();
	}
}

SCENARIO("a possessed mob does not outlive the connection driving it", "[chdesc]")
{
	GIVEN("an immortal switched into a mob")
	{
		auto room = MakeTrackingRoom();
		CHAR_DATA *immortal = MakeTracker(room);
		CHAR_DATA *mob = MakeTracker(room);
		DESCRIPTOR_DATA *d = new_descriptor();

		Possess(d, mob, immortal);

		REQUIRE(Connection(mob) == d);
		REQUIRE(Connection(immortal) == nullptr);

		WHEN("the socket closes on the shutdown path, which frees only the immortal")
		{
			free_char(immortal);
			free_descriptor(d);

			THEN("the mob names no connection")
			{
				REQUIRE(Connection(mob) == nullptr);
			}

			THEN("the next connection to take the slot is not adopted")
			{
				// Every reader of this field asks "is this a player with a live
				// connection", so a recycled descriptor turns a mob into one.
				//
				// The address is no longer what gets recycled, because a closed
				// descriptor is really destroyed now. But the slot map still
				// hands the freed slot to the next Add, and the slot is what a
				// handle names. Holding SlotCount steady across the reissue is
				// what proves the reuse happened, and so keeps the assertion
				// below from passing for the trivial reason.
				std::size_t slotsBefore = descriptorHandles.SlotCount();

				DESCRIPTOR_DATA *newcomer = new_descriptor();

				REQUIRE(descriptorHandles.SlotCount() == slotsBefore);
				REQUIRE(Connection(mob) != newcomer);

				free_descriptor(newcomer);
			}
		}

		ClearTrackers();
	}
}

//
// ch->last_fight_opponent -- "who did I last fight", for the lost-link wiznet and
// do_stat.
//
// Not a string field. It is a `char *` holding the *other character's*
// true_name allocation, compared by pointer identity, and the only setter is
// update_pc_last_fight -- which early-returns unless both parties are players.
// extract_char's scrub carries the identical !is_npc guard on both sides, so
// the mismatch that guard could hide is unreachable by construction.
//
// The hole is somewhere else. `true_name` is free_pstring'd and reallocated on
// two paths that leave the character *alive* -- do_rename, and update.c's
// name-repair tick -- and after either, the alias points at released memory
// that extract_char's scrub can never match, because it compares against the
// character's current true_name and that buffer is no longer anyone's.
//
// free_pstring is a real delete[], not a free list, so unlike every other
// defect in this graph this one is ordinary heap memory and ASAN sees it.
//

namespace
{
	const char *LastOpponentName(CHAR_DATA *ch)
	{
		CHAR_DATA *opponent = Deref(ch->last_fight_opponent);

		return opponent ? opponent->true_name : nullptr;
	}

	void RecordFight(CHAR_DATA *a, CHAR_DATA *b)
	{
		a->last_fight_opponent = b->self;
	}

	CHAR_DATA *MakeNamedPlayer(ROOM_INDEX_DATA *room, const char *name)
	{
		CHAR_DATA *ch = MakeTracker(room);

		REMOVE_BIT(ch->act, ACT_IS_NPC);
		// extract_char's remaining scrub walk reads tch->pcdata->trusting for
		// every player in char_list, so a test player needs real pcdata.
		ch->pcdata = std::make_unique<pc_data>();
		ch->true_name = palloc_string(name);
		ch->name = palloc_string(name);

		return ch;
	}
}

SCENARIO("the last opponent is forgotten when they leave the world", "[lastfight]")
{
	GIVEN("two players who have fought each other")
	{
		auto room = MakeTrackingRoom();
		CHAR_DATA *fighter = MakeNamedPlayer(room, "Alice");
		CHAR_DATA *opponent = MakeNamedPlayer(room, "Bob");

		RecordFight(fighter, opponent);

		REQUIRE(LastOpponentName(fighter) != nullptr);
		REQUIRE(strcmp(LastOpponentName(fighter), "Bob") == 0);

		WHEN("the opponent is extracted")
		{
			extract_char(opponent, true);

			THEN("the record reads as nothing rather than as a released buffer")
			{
				REQUIRE(LastOpponentName(fighter) == nullptr);
			}
		}

		ClearTrackers();
	}
}

SCENARIO("a slain player is still in the world, so extract_char is not a lifetime event for them", "[lastfight]")
{
	GIVEN("two players who have fought each other")
	{
		auto room = MakeTrackingRoom();
		CHAR_DATA *fighter = MakeNamedPlayer(room, "Alice");
		CHAR_DATA *opponent = MakeNamedPlayer(room, "Bob");

		RecordFight(fighter, opponent);

		WHEN("the opponent is extracted the way raw_kill does it, without pulling them")
		{
			// raw_kill is the only caller that passes fPull = false, and for a
			// player extract_char then returns early to the altar without
			// freeing anything. So this call leaves Bob alive and in char_list.
			extract_char(opponent, false);

			THEN("the opponent is still alive")
			{
				REQUIRE(Deref(opponent->self) == opponent);
			}

			THEN("clearing the other side's record is raw_kill's job, not extract_char's")
			{
				// extract_char cannot be responsible for this: the reference is
				// still perfectly live, because its target is. The record is
				// reset on death because that is the game rule, and raw_kill is
				// where the rule lives.
				//
				// NOT COVERED, deliberately: that raw_kill actually applies the
				// rule. Reaching it needs a killer, a corpse, the ghost affect
				// set, group and cabal state and a populated room index -- far
				// more world than this suite stands up, and a mock of it would
				// only assert that the mock was written correctly. The walk in
				// raw_kill, immediately after its extract_char call, is checked
				// by reading. If that walk is ever moved or dropped, nothing
				// here will fail -- so treat it as load-bearing.
				REQUIRE(LastOpponentName(fighter) != nullptr);
			}
		}

		ClearTrackers();
	}
}

SCENARIO("the last opponent survives a rename", "[lastfight]")
{
	GIVEN("two players who have fought each other")
	{
		auto room = MakeTrackingRoom();
		CHAR_DATA *fighter = MakeNamedPlayer(room, "Alice");
		CHAR_DATA *opponent = MakeNamedPlayer(room, "Bob");

		RecordFight(fighter, opponent);

		WHEN("the opponent is renamed")
		{
			// do_rename and update.c's name-repair tick both free_pstring the
			// old true_name and allocate a new one, and both leave the
			// character alive -- so extract_char's scrub never runs, and could
			// not match the released buffer if it did.
			//
			// The old buffer is deliberately kept alive here rather than freed:
			// reading a released one is undefined, and the question this asks
			// is which *name* the record reports, not whether the read traps.
			// The trap is real and ASAN sees it; that is demonstrated
			// separately, because a permanently undefined test is a bad test.
			char *releasedInProduction = opponent->true_name;
			opponent->true_name = palloc_string("Robert");

			THEN("the record names the opponent by their current name")
			{
				REQUIRE(strcmp(LastOpponentName(fighter), "Robert") == 0);
			}

			free_pstring(releasedInProduction);
		}

		ClearTrackers();
	}
}

//
// ch->pcdata->trusting -- who this player has authorised to take questionable
// actions against them.
//
// A plain CHAR -> CHAR back-reference that happens to live on PC_DATA rather
// than CHAR_DATA, which is the only reason §1's original table missed it. Set
// only by do_trust, which rejects NPCs, so both ends are always players.
//
// Its clause in extract_char's walk is guarded !is_npc on both sides, and that
// makes it half lifetime and half game rule -- see the scenarios below.
//

namespace
{
	CHAR_DATA *Trusting(CHAR_DATA *ch)
	{
		return Deref(ch->pcdata->trusting);
	}

	void Trust(CHAR_DATA *ch, CHAR_DATA *victim)
	{
		ch->pcdata->trusting = victim->self;
	}
}

SCENARIO("trust does not outlive the trusted player", "[trusting]")
{
	GIVEN("a player trusting another")
	{
		auto room = MakeTrackingRoom();
		CHAR_DATA *truster = MakeNamedPlayer(room, "Alice");
		CHAR_DATA *trusted = MakeNamedPlayer(room, "Bob");
		CHAR_DATA *bystander = MakeNamedPlayer(room, "Carol");

		Trust(truster, trusted);

		REQUIRE(Trusting(truster) == trusted);

		WHEN("the trusted player is pulled from the world")
		{
			// fPull = true is the real removal -- quit, delete_char. This is
			// the half of the clause that is lifetime, and the half a handle
			// covers for free.
			extract_char(trusted, true);

			THEN("the trust reads as nothing")
			{
				REQUIRE(Trusting(truster) == nullptr);
			}

			THEN("it stays nothing once the address is reissued")
			{
				// The slot rather than the address: a destroyed character is really
				// deleted now, so reuse of the memory is an allocator accident. The
				// slot map does still recycle the freed slot, and the slot is what a
				// handle names, so this is what keeps the assertion below honest.
				std::size_t slotsBefore = charHandles.SlotCount();

				CHAR_DATA *newcomer = new_char();

				REQUIRE(charHandles.SlotCount() == slotsBefore);
				REQUIRE(Trusting(truster) != newcomer);
			}
		}

		WHEN("somebody unrelated is pulled from the world")
		{
			extract_char(bystander, true);

			THEN("the trust is untouched")
			{
				REQUIRE(Trusting(truster) == trusted);
			}
		}

		ClearTrackers();
	}
}

SCENARIO("a slain player is still trustworthy as far as the world is concerned", "[trusting]")
{
	GIVEN("a player trusting another")
	{
		auto room = MakeTrackingRoom();
		CHAR_DATA *truster = MakeNamedPlayer(room, "Alice");
		CHAR_DATA *trusted = MakeNamedPlayer(room, "Bob");

		Trust(truster, trusted);

		WHEN("the trusted player is slain, which does not pull them from the world")
		{
			extract_char(trusted, false);

			THEN("the trusted player is still alive")
			{
				REQUIRE(Deref(trusted->self) == trusted);
			}

			THEN("dropping the trust is the death rule's job, not extract_char's")
			{
				// The reference is still live because its target is. That
				// death cancels trust is a rule, and rules belong where the
				// death happens -- raw_kill -- not in the lifetime teardown.
				//
				// NOT COVERED, deliberately, and for the same reason as the
				// [lastfight] scenario above: this suite does not execute
				// raw_kill, so nothing here proves the rule still fires. Both
				// clears live in one walk directly after raw_kill's
				// extract_char call; that walk is verified by reading only.
				REQUIRE(Trusting(truster) != nullptr);
			}
		}

		ClearTrackers();
	}
}

//
// The four affect `owner` fields -- AFFECT_DATA, ROOM_AFFECT_DATA,
// AREA_AFFECT_DATA and OBJ_AFFECT_DATA.
//
// `owner` is "who caused this", and it is read for damage attribution, for
// is_safe/is_same_group/is_same_cabal checks, for messaging, and in a handful
// of places for "has the causer died". Four containers hold affects that carry
// one -- ch->affected, room->affected, area->affected, obj->affected -- plus
// obj->charaffs, which holds AFFECT_DATA on an object rather than a character.
//
// Only ONE of those five was ever scrubbed. extract_char walked char_list and
// nulled `owner` in every ch->affected list; the room affects owned by the
// dying character were removed outright by a second walk over
// top_affected_room. Area affects, object affects and obj->charaffs were never
// touched at all, so their owners simply dangled -- and free_char recycles
// addresses, so "dangled" means "started naming whoever was allocated next".
//
// The two walks are not the same kind of thing, and the conversion treats them
// differently. See the scenarios below.
//

namespace
{
	CHAR_DATA *AffectOwner(const AFFECT_DATA &af)
	{
		return Deref(af.owner);
	}

	CHAR_DATA *AffectOwner(const ROOM_AFFECT_DATA &raf)
	{
		return Deref(raf.owner);
	}

	CHAR_DATA *AffectOwner(const AREA_AFFECT_DATA &aaf)
	{
		return Deref(aaf.owner);
	}

	CHAR_DATA *AffectOwner(const OBJ_AFFECT_DATA &oaf)
	{
		return Deref(oaf.owner);
	}

	// The setter half of the pair. Every assignment in these scenarios goes
	// through it for the same reason every read goes through AffectOwner: this
	// file has to compile and run identically either side of the type flip, so
	// the only things that may mention the field's type are these two bodies.
	template <typename AffectT>
	void SetAffectOwner(AffectT &af, CHAR_DATA *owner)
	{
		af.owner = owner->self;
	}

	AFFECT_DATA &GiveCharAffect(CHAR_DATA *victim, CHAR_DATA *owner, int type)
	{
		AFFECT_DATA af;
		init_affect(&af);

		af.type = static_cast<short>(type);
		af.duration = 10;
		SetAffectOwner(af, owner);

		victim->affected.push_back(af);

		return victim->affected.back();
	}

	OBJ_AFFECT_DATA &GiveObjAffect(OBJ_DATA *obj, CHAR_DATA *owner, int type)
	{
		OBJ_AFFECT_DATA oaf;
		init_affect_obj(&oaf);

		oaf.type = static_cast<short>(type);
		oaf.duration = 10;
		SetAffectOwner(oaf, owner);

		obj->affected.push_back(oaf);

		return obj->affected.back();
	}

	AREA_AFFECT_DATA &GiveAreaAffect(AREA_DATA *area, CHAR_DATA *owner, int type)
	{
		AREA_AFFECT_DATA aaf;
		init_affect_area(&aaf);

		aaf.type = static_cast<short>(type);
		aaf.duration = 10;
		SetAffectOwner(aaf, owner);

		area->affected.push_back(aaf);

		return area->affected.back();
	}

	ROOM_AFFECT_DATA &GiveRoomAffect(ROOM_INDEX_DATA *room, CHAR_DATA *owner, int type)
	{
		ROOM_AFFECT_DATA raf;
		init_affect_room(&raf);

		raf.type = static_cast<short>(type);
		raf.duration = 10;
		SetAffectOwner(raf, owner);

		room->affected.push_back(raf);

		// affect_to_room does this as a side effect of adding the first affect,
		// and extract_char's second walk only visits rooms on this list.
		if (top_affected_room != room && room->aff_next == nullptr)
		{
			room->aff_next = top_affected_room;
			top_affected_room = room;
		}

		return room->affected.back();
	}

	void ClearAffectedRooms()
	{
		top_affected_room = nullptr;
	}
}

SCENARIO("an affect does not remember a caster who has left the world", "[affectowner]")
{
	GIVEN("a mob carrying an affect cast by another")
	{
		auto room = MakeTrackingRoom();
		CHAR_DATA *caster = MakeTracker(room);
		CHAR_DATA *victim = MakeTracker(room);
		CHAR_DATA *bystander = MakeTracker(room);

		AFFECT_DATA &af = GiveCharAffect(victim, caster, 1);
		AFFECT_DATA &unrelated = GiveCharAffect(bystander, victim, 1);

		REQUIRE(AffectOwner(af) == caster);

		WHEN("the caster is pulled from the world")
		{
			extract_char(caster, true);

			THEN("the affect names nobody")
			{
				REQUIRE(AffectOwner(af) == nullptr);
			}

			THEN("an affect owned by somebody else is left alone")
			{
				REQUIRE(AffectOwner(unrelated) == victim);
			}

			THEN("it still names nobody once the address is reissued")
			{
				// The whole point. free_char pushes onto a free list, so the
				// next new_char hands back the caster's address; a raw
				// back-reference would silently start naming the newcomer.
				// The slot rather than the address: a destroyed character is really
				// deleted now, so reuse of the memory is an allocator accident. The
				// slot map does still recycle the freed slot, and the slot is what a
				// handle names, so this is what keeps the assertion below honest.
				std::size_t slotsBefore = charHandles.SlotCount();

				CHAR_DATA *newcomer = new_char();

				REQUIRE(charHandles.SlotCount() == slotsBefore);
				REQUIRE(AffectOwner(af) != newcomer);
				REQUIRE(AffectOwner(af) == nullptr);
			}
		}

		ClearTrackers();
	}
}

SCENARIO("an object's affect does not remember a caster who has left the world", "[affectowner]")
{
	GIVEN("an object carrying an affect cast by a character")
	{
		auto room = MakeTrackingRoom();
		CHAR_DATA *caster = MakeTracker(room);
		OBJ_DATA *obj = new_obj();

		OBJ_AFFECT_DATA &oaf = GiveObjAffect(obj, caster, 1);

		REQUIRE(AffectOwner(oaf) == caster);

		WHEN("the caster is pulled from the world")
		{
			// Nothing has ever scrubbed this one -- extract_char's walk only
			// ever visited ch->affected -- so before the conversion the field
			// held a freed pointer here, and named the next character to take
			// the address.
			extract_char(caster, true);

			THEN("the affect names nobody")
			{
				REQUIRE(AffectOwner(oaf) == nullptr);
			}

			THEN("it still names nobody once the address is reissued")
			{
				// The slot rather than the address: a destroyed character is really
				// deleted now, so reuse of the memory is an allocator accident. The
				// slot map does still recycle the freed slot, and the slot is what a
				// handle names, so this is what keeps the assertion below honest.
				std::size_t slotsBefore = charHandles.SlotCount();

				CHAR_DATA *newcomer = new_char();

				REQUIRE(charHandles.SlotCount() == slotsBefore);
				REQUIRE(AffectOwner(oaf) != newcomer);
			}
		}

		free_obj(obj);
		ClearTrackers();
	}
}

SCENARIO("an area's affect does not remember a caster who has left the world", "[affectowner]")
{
	GIVEN("an area carrying an affect cast by a character")
	{
		auto room = MakeTrackingRoom();
		CHAR_DATA *caster = MakeTracker(room);

		AREA_AFFECT_DATA &aaf = GiveAreaAffect(room->area, caster, 1);

		REQUIRE(AffectOwner(aaf) == caster);

		WHEN("the caster is pulled from the world")
		{
			// Never scrubbed either, and area affects are the longest-lived of
			// the four -- a zone-wide effect outlasts the fight that spawned it
			// by design.
			extract_char(caster, true);

			THEN("the affect names nobody")
			{
				REQUIRE(AffectOwner(aaf) == nullptr);
			}

			THEN("it still names nobody once the address is reissued")
			{
				// The slot rather than the address: a destroyed character is really
				// deleted now, so reuse of the memory is an allocator accident. The
				// slot map does still recycle the freed slot, and the slot is what a
				// handle names, so this is what keeps the assertion below honest.
				std::size_t slotsBefore = charHandles.SlotCount();

				CHAR_DATA *newcomer = new_char();

				REQUIRE(charHandles.SlotCount() == slotsBefore);
				REQUIRE(AffectOwner(aaf) != newcomer);
			}
		}

		ClearTrackers();
	}
}

SCENARIO("a room affect placed by a character dies with them", "[affectowner]")
{
	GIVEN("a room carrying an affect placed by a character")
	{
		auto room = MakeTrackingRoom();
		CHAR_DATA *caster = MakeTracker(room);
		CHAR_DATA *other = MakeTracker(room);

		GiveRoomAffect(room, caster, 1);
		GiveRoomAffect(room, other, 2);

		REQUIRE(room->affected.size() == 2);

		WHEN("the caster is pulled from the world")
		{
			extract_char(caster, true);

			THEN("their room affect is removed outright, not merely disowned")
			{
				// This is the one walk in extract_char that survives the
				// conversion, and it survives because it does not expire a
				// reference -- it destroys the affect. A handle would leave the
				// wall of fire burning in the room with nobody to attribute it
				// to, and every consumer of raf->owner in act_move.c and
				// update.c passes it straight to damage_new and is_safe.
				REQUIRE(room->affected.size() == 1);
				REQUIRE(AffectOwner(room->affected.front()) == other);
			}
		}

		ClearAffectedRooms();
		ClearTrackers();
	}
}

SCENARIO("a slain player is still in the world, so their affects still name them", "[affectowner]")
{
	GIVEN("a mob carrying an affect cast by a player")
	{
		auto room = MakeTrackingRoom();
		CHAR_DATA *caster = MakeNamedPlayer(room, "Alice");
		CHAR_DATA *victim = MakeTracker(room);

		AFFECT_DATA &af = GiveCharAffect(victim, caster, 1);

		WHEN("the caster is extracted the way raw_kill does it, without pulling them")
		{
			extract_char(caster, false);

			THEN("the caster is still alive")
			{
				REQUIRE(Deref(caster->self) == caster);
			}

			THEN("the affect still names them")
			{
				// This is the assertion that changes sides, and it is
				// deliberate. extract_char's walk sat ABOVE the early return
				// that sends a slain player to the altar without freeing them,
				// so it nulled `owner` for a character who was still alive.
				//
				// Unlike last_fight_opponent and pcdata->trusting, that is not
				// a game rule wearing lifetime clothing, and the evidence is
				// that the readers disagree with it: update.c's mark of wrath
				// asks `paf->owner->ghost > 0` -- literally "has my marked foe
				// died" -- which can only be answered if `owner` still resolves
				// after a death. The walk nulled it first, so that check was a
				// null dereference rather than a rule. ap.c's burning_pulse
				// (`owner->level`) and act_info.c's mark display
				// (`af->owner->name`) are the same shape.
				//
				// So nothing is relocated to raw_kill for this field. The walk
				// is deleted and the reference is left to expire on its own,
				// which happens only when the caster is really gone.
				REQUIRE(AffectOwner(af) == caster);
			}
		}

		ClearTrackers();
	}
}

//
// d->snoop_by -- the connection watching this one.
//
// The last raw cross-reference in the descriptor pair, and the only one whose
// scrub loop was correct: close_socket swept descriptor_list clearing every
// d->snoop_by that named the connection going away, and free_descriptor is
// reached from nowhere else that a snooped connection could be looking at.
//
// So there is no defect here to demonstrate, and saying so matters -- the
// scenarios below document the mechanism that replaces the sweep rather than a
// bug that it missed.
//

namespace
{
	DESCRIPTOR_DATA *SnoopedBy(DESCRIPTOR_DATA *d)
	{
		return Deref(d->snoop_by);
	}

	void Snoop(DESCRIPTOR_DATA *snooper, DESCRIPTOR_DATA *target)
	{
		target->snoop_by = snooper->self;
	}
}

SCENARIO("a snoop does not outlive the connection running it", "[snoop]")
{
	GIVEN("one connection snooping another")
	{
		DESCRIPTOR_DATA *snooper = new_descriptor();
		DESCRIPTOR_DATA *watched = new_descriptor();

		Snoop(snooper, watched);

		REQUIRE(SnoopedBy(watched) == snooper);

		WHEN("the snooping connection is freed")
		{
			// close_socket is where the sweep lived, and it is far more world
			// than this suite stands up -- process_output, wiznet, the
			// character teardown. What the sweep was *for* is this: once the
			// snooper is gone, nothing may still name it.
			free_descriptor(snooper);

			THEN("the watched connection names no snooper")
			{
				REQUIRE(SnoopedBy(watched) == nullptr);
			}

			THEN("it still names none once the slot is reissued")
			{
				// This is the part the sweep could not have given: a reused
				// descriptor slot would have satisfied an index-only reference,
				// and every reader of this field writes the snooped output
				// straight into whatever it names. SlotCount holding steady is
				// what proves the freed slot really was handed back out.
				std::size_t slotsBefore = descriptorHandles.SlotCount();

				DESCRIPTOR_DATA *newcomer = new_descriptor();

				REQUIRE(descriptorHandles.SlotCount() == slotsBefore);
				REQUIRE(SnoopedBy(watched) != newcomer);

				free_descriptor(newcomer);
			}
		}

		free_descriptor(watched);
	}
}

SCENARIO("a connection that stops snooping leaves its target alone", "[snoop]")
{
	GIVEN("one connection snooping another")
	{
		DESCRIPTOR_DATA *snooper = new_descriptor();
		DESCRIPTOR_DATA *watched = new_descriptor();

		Snoop(snooper, watched);

		WHEN("the snoop is dropped rather than the connection closing")
		{
			// do_snoop's own clear, which stays: "snoop self" means "drop every
			// snoop I am running", and nobody has left the world.
			watched->snoop_by = nullptr;

			THEN("the snooper is untouched")
			{
				REQUIRE(SnoopedBy(watched) == nullptr);
				REQUIRE(Deref(snooper->self) == snooper);
			}
		}

		free_descriptor(snooper);
		free_descriptor(watched);
	}
}

//
// equip_char's rejection path and who it applies to.
//
// The alignment and ethos restrictions on a piece of equipment are a player
// rule. A mob wearing what its area file gave it is not making a moral choice,
// and the sixteen-odd resets that hand an aligned item to a mob depend on that.
// The guard that says so used to bind only the first of the seven checks after
// it, so every check but ANTI_EVIL applied to mobs as well, and the item was
// dropped on the floor of the room the mob was reset into.
//
// Written against the fixture rather than by hand because the rejection path
// fires two act() calls and an obj_to_room before it returns. Asserting that it
// did not fire needs the room the object would have been dropped into and a
// watcher who would have been told about it.
//

SCENARIO("an aligned item does not reject the mob wearing it", "[equip_char]")
{
	GIVEN("a good mob holding an item that rejects the good")
	{
		TestWorld world;
		auto room = world.CreateRoom();
		auto mob = world.CreateMob("guard", room);
		auto watcher = world.CreatePlayer("Watcher", room);
		auto obj = world.CreateItem("blade", "a dark blade");

		mob->alignment = 1000;
		SET_BIT(obj->extra_flags, ITEM_ANTI_GOOD);
		obj_to_char(obj, mob);

		REQUIRE(Deref(obj->carried_by) == mob);

		WHEN("the mob is equipped with it")
		{
			TestWorld::ClearOutput(watcher);
			equip_char(mob, obj, WEAR_WIELD, false);

			THEN("it wears the item instead of dropping it")
			{
				REQUIRE(obj->wear_loc == WEAR_WIELD);
				REQUIRE(Deref(obj->carried_by) == mob);
				REQUIRE(obj->in_room == nullptr);
				REQUIRE(room->contents == nullptr);
			}

			THEN("nobody in the room is told it was dropped")
			{
				REQUIRE_FALSE(TestWorld::Heard(watcher, "drops"));
			}
		}
	}

	GIVEN("a good player holding the same item")
	{
		TestWorld world;
		auto room = world.CreateRoom();
		auto player = world.CreatePlayer("Pilgrim", room);
		auto watcher = world.CreatePlayer("Watcher", room);
		auto obj = world.CreateItem("blade", "a dark blade");

		player->alignment = 1000;
		SET_BIT(obj->extra_flags, ITEM_ANTI_GOOD);
		obj_to_char(obj, player);

		WHEN("the player is equipped with it")
		{
			TestWorld::ClearOutput(player);
			TestWorld::ClearOutput(watcher);
			equip_char(player, obj, WEAR_WIELD, false);

			THEN("the item rejects them and lands in the room")
			{
				REQUIRE(obj->wear_loc == WEAR_NONE);
				REQUIRE(obj->in_room == room);
				REQUIRE(Deref(obj->carried_by) == nullptr);
			}

			THEN("both messages are sent")
			{
				REQUIRE(TestWorld::Heard(player, "rejects you"));
				REQUIRE(TestWorld::Heard(watcher, "drops"));
			}
		}
	}
}

// An object reaches extract_obj from several directions, and one of them is the
// destruction of whatever is carrying it. Nothing on that path establishes that
// the object came from a prototype, and the count it decrements belongs to one.
SCENARIO("extracting an object that was never stamped from a prototype", "[extract_obj]")
{
	GIVEN("an object built by hand, with no prototype behind it")
	{
		TestWorld world;

		auto obj = new obj_data();
		obj->self = objectHandles.Add(obj);
		obj->name = palloc_string("orphan");
		obj->short_descr = palloc_string("an orphaned trinket");
		obj->description = palloc_string("An orphaned trinket lies here.");
		obj->item_type = ITEM_TRASH;
		obj->wear_loc = WEAR_NONE;

		REQUIRE(obj->pIndexData == nullptr);

		WHEN("it is extracted")
		{
			// Without the guard this reads limcount through the null prototype
			// and the process dies here. It never reaches the object list check
			// below, which is the part that reports the object as unaccounted for.
			extract_obj(obj);

			THEN("the object survives to be released by its caller")
			{
				// extract_obj returns early for an object that was never linked,
				// saying so in the log and leaving the object to whoever built it.
				REQUIRE(Deref(obj->self) == obj);
			}
		}

		free_obj(obj);
	}
}

// A pet escorting a player through the academy keeps ticking after the player
// stops existing. The leader it is escorting is reached through a handle, and a
// handle to a freed character resolves to nothing.
SCENARIO("an academy pet whose leader has gone", "[apet_at_room]")
{
	GIVEN("a pet whose leader has been freed")
	{
		TestWorld world;
		auto room = world.CreateRoom();
		auto player = world.CreatePlayer("Escorted", room);
		auto pet = world.CreateMob("an academy guide", room);

		pet->leader = player->self;

		REQUIRE(Deref(pet->leader) == player);

		WHEN("the leader is freed and the pet reaches a room")
		{
			free_char(player);

			REQUIRE(Deref(pet->leader) == nullptr);

			THEN("the pet does nothing rather than writing through the dead handle")
			{
				// Without the guard the first statement of apet_at_room assigns
				// through a null pointer and the process dies here.
				apet_at_room(pet, CIM_FOOD);

				REQUIRE(Deref(pet->master) == nullptr);
			}
		}
	}
}

// can_see is read at its call sites as a filter that rejects a character who is
// not there: several callers resolve one out of a handle and pass the result
// straight in, guarded by nothing but the call itself. Every line of the body
// dereferences both arguments, so the predicate has to answer for the absent
// case rather than crash inside it.
SCENARIO("can_see answers for a character that is not there", "[can_see]")
{
	GIVEN("a character in a lit room")
	{
		TestWorld world;
		auto room = world.CreateRoom();
		auto watcher = world.CreatePlayer("Watcher", room);
		auto seen = world.CreatePlayer("Seen", room);

		WHEN("the victim is absent")
		{
			THEN("nothing is visible rather than the test dying inside itself")
			{
				// Without the guard this dereferences null a few lines into the
				// body and the process dies here.
				REQUIRE(can_see(watcher, nullptr) == false);
			}
		}

		WHEN("the viewer is absent")
		{
			THEN("nothing is visible")
			{
				REQUIRE(can_see(nullptr, seen) == false);
			}
		}

		WHEN("both are absent")
		{
			THEN("the answer is still no")
			{
				// The ch == victim shortcut sits above most of the body and
				// would answer true for two null pointers, so the guard has to
				// come before it.
				REQUIRE(can_see(nullptr, nullptr) == false);
			}
		}

		WHEN("both are present")
		{
			THEN("they can see each other, so the guard did not swallow the normal case")
			{
				REQUIRE(can_see(watcher, seen) == true);
				REQUIRE(can_see(seen, watcher) == true);
			}
		}
	}
}

//
// What comes off a cosmetic slot.
//
// equip_char returns as soon as it has put a cosmetic on: no armour class, no
// applies, no affects, no immunity or resistance bits. unequip_char has the
// matching early return, but it cleared the object's slot first and then asked
// the object which slot it had been in, so the test never matched and a
// cosmetic had taken off it everything that was never put on.
//

SCENARIO("taking off a cosmetic item", "[unequip_char]")
{
	GIVEN("a player wearing a cosmetic that carries an apply")
	{
		TestWorld world;
		auto room = world.CreateRoom();
		auto player = world.CreatePlayer("Wearer", room);
		auto obj = world.CreateItem("mask", "a porcelain mask");

		OBJ_APPLY_DATA app;
		app.location = APPLY_HITROLL;
		app.modifier = 5;
		app.type = 0;
		obj->apply.push_front(app);

		obj_to_char(obj, player);

		int hitrollBefore = player->hitroll;

		WHEN("it is worn")
		{
			equip_char(player, obj, WEAR_COSMETIC, false);

			THEN("the apply is not added, because a cosmetic carries no effect")
			{
				REQUIRE(obj->wear_loc == WEAR_COSMETIC);
				REQUIRE(player->hitroll == hitrollBefore);
			}

			WHEN("it is taken off again")
			{
				unequip_char(player, obj, false);

				THEN("the apply is not taken away either")
				{
					REQUIRE(obj->wear_loc == WEAR_NONE);
					REQUIRE(player->hitroll == hitrollBefore);
				}
			}
		}
	}
}

//
// The wear location table is scanned rather than indexed.
//
// flag_lookup and flag_name_lookup both walk until they reach a row with a null
// name, so a table without one runs off its own end for every lookup that does
// not match.
//

SCENARIO("looking up a wear location that is not in the table", "[wear_locations]")
{
	GIVEN("the wear location table")
	{
		// A missing terminator cannot be caught from here: the scan runs into
		// whatever the linker put next and stops at the first null name it
		// finds there, which is why this went unnoticed. What catches it is
		// AddressSanitizer, which reports the read past the end of the object.
		THEN("a name it does not carry answers no")
		{
			REQUIRE(flag_lookup("nonesuch", wear_locations) == NO_FLAG);
		}

		THEN("a value it does not carry answers nothing")
		{
			REQUIRE(flag_name_lookup(9999, wear_locations) == nullptr);
		}

		THEN("the names it does carry still answer")
		{
			REQUIRE(flag_lookup("wield", wear_locations) == wear_index(WEAR_WIELD));
			REQUIRE(flag_lookup("hold", wear_locations) == wear_index(WEAR_HOLD));
		}
	}
}
