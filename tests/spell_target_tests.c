#include "catch.hpp"
#include "../code/merc.h"
#include "../code/entity/spell_target.h"

//
// SpellTarget exists to make one mistake impossible: reading a payload as a
// kind it is not. These cover the guarantee itself rather than any spell, so
// they need no world.
//
// The entities below are allocated and never freed on purpose. Their
// destructors expect a loaded world and crash without one, which is why the
// other tests here allocate the same way. SpellTarget only stores the pointer
// and never reads through it, so an entity that is never initialised is
// enough for these.
//

SCENARIO("a spell target reports the kind it was built from", "[spell_target]")
{
	GIVEN("a target built from a character")
	{
		char_data *victim = new char_data();
		SpellTarget target(victim);

		THEN("it reports a character and nothing else")
		{
			REQUIRE(target.IsChar());
			REQUIRE_FALSE(target.IsObj());
			REQUIRE_FALSE(target.IsRoom());
			REQUIRE_FALSE(target.IsDir());
			REQUIRE_FALSE(target.IsNone());
			REQUIRE(target.AsChar() == victim);
		}
	}

	GIVEN("a target built from an object")
	{
		obj_data *item = new obj_data();
		SpellTarget target(item);

		THEN("it reports an object and hands the object back")
		{
			REQUIRE(target.IsObj());
			REQUIRE_FALSE(target.IsChar());
			REQUIRE(target.AsObj() == item);
		}
	}

	GIVEN("a target built from a room")
	{
		room_index_data *room = new room_index_data();
		SpellTarget target(room);

		THEN("it reports a room and hands the room back")
		{
			REQUIRE(target.IsRoom());
			REQUIRE_FALSE(target.IsChar());
			REQUIRE(target.AsRoom() == room);
		}
	}

	GIVEN("a target built from a direction")
	{
		SpellTarget target = SpellTarget::Direction(3);

		THEN("it reports a direction and hands the value back")
		{
			REQUIRE(target.IsDir());
			REQUIRE_FALSE(target.IsChar());
			REQUIRE(target.AsDir() == 3);
		}
	}

	GIVEN("a default constructed target")
	{
		SpellTarget target;

		THEN("it holds nothing")
		{
			REQUIRE(target.IsNone());
			REQUIRE_FALSE(target.IsChar());
			REQUIRE_FALSE(target.IsObj());
			REQUIRE_FALSE(target.IsRoom());
			REQUIRE_FALSE(target.IsDir());
		}
	}
}

SCENARIO("reading a spell target as the wrong kind yields nothing", "[spell_target]")
{
	GIVEN("a target holding an object")
	{
		obj_data *item = new obj_data();
		SpellTarget target(item);

		THEN("reading it as a character yields null rather than the object")
		{
			// This is the case that used to reinterpret an object's memory as
			// a character and read fields out of it.
			REQUIRE(target.AsChar() == nullptr);
			REQUIRE(target.AsRoom() == nullptr);
			REQUIRE(target.AsDir() == -1);
		}
	}

	GIVEN("a target holding a character")
	{
		char_data *victim = new char_data();
		SpellTarget target(victim);

		THEN("reading it as an object yields null")
		{
			REQUIRE(target.AsObj() == nullptr);
			REQUIRE(target.AsRoom() == nullptr);
		}
	}

	GIVEN("a target holding nothing")
	{
		SpellTarget target;

		THEN("every accessor yields an empty value")
		{
			REQUIRE(target.AsChar() == nullptr);
			REQUIRE(target.AsObj() == nullptr);
			REQUIRE(target.AsRoom() == nullptr);
			REQUIRE(target.AsDir() == -1);
		}
	}
}

SCENARIO("a spell target names the kind it holds", "[spell_target]")
{
	GIVEN("targets of each kind")
	{
		char_data *victim = new char_data();
		obj_data *item = new obj_data();
		room_index_data *room = new room_index_data();

		THEN("each names itself")
		{
			REQUIRE(std::string(SpellTarget(victim).KindName()) == "a character");
			REQUIRE(std::string(SpellTarget(item).KindName()) == "an object");
			REQUIRE(std::string(SpellTarget(room).KindName()) == "a room");
			REQUIRE(std::string(SpellTarget::Direction(0).KindName()) == "a direction");
			REQUIRE(std::string(SpellTarget().KindName()) == "nothing");
		}
	}
}
