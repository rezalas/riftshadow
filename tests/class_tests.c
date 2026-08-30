#include "catch.hpp"
#include <algorithm>
#include "db_mocks.h"
#include "../code/repositories/classrepository.h"
#include "world_fixture.h"

// class_table schema column order:
//   id, name, who_name, attr_prime, align, weapon, gainconst,
//   base_group, default_group, ctype, status
// id is the ordering key only; it is not stored on ClassRow (it maps positionally
// to CClass::index, assigned 0-based in id-ascending order by CClass::LoadClassTable).

static const std::string kSelectSql =
	"SELECT id, name, who_name, attr_prime, align, weapon, gainconst, "
	"base_group, default_group, ctype, status "
	"FROM class_table ORDER BY id ASC";

SCENARIO("reading every class definition for the boot-time load", "[class][repository]")
{
	GIVEN("a session holding class rows out of insertion order")
	{
		FakeDbSession db;
		//                    id  name        who  att al wpn gc  base       default    ct st
		db.Rows.push_back(Row({"1", "warrior",  "War", "1", "0", "5", "3", "combat", "warfare", "0", "1"}));
		db.Rows.push_back(Row({"4", "paladin",  "Pal", "2", "1", "5", "2", "combat", "holy",    "2", "1"}));

		ClassRepository repo(db);

		WHEN("FindAllOrdered is called")
		{
			auto results = repo.FindAllOrdered();

			THEN("it selects an explicit column list ordered by id, never SELECT *")
			{
				REQUIRE(db.LastSql == kSelectSql);
				REQUIRE(db.LastSql.find("*") == std::string::npos);
				REQUIRE(db.LastSql.find("ORDER BY id ASC") != std::string::npos);
				REQUIRE(db.Binds.empty());
			}

			THEN("it maps every column onto ClassRow in schema order, skipping id")
			{
				REQUIRE(results.size() == 2);

				REQUIRE(results[0].name == "warrior");
				REQUIRE(results[0].who_name == "War");
				REQUIRE(results[0].attr_prime == 1);
				REQUIRE(results[0].align == 0);
				REQUIRE(results[0].weapon == 5);
				REQUIRE(results[0].gainconst == 3);
				REQUIRE(results[0].base_group == "combat");
				REQUIRE(results[0].default_group == "warfare");
				REQUIRE(results[0].ctype == 0);
				REQUIRE(results[0].status == 1);

				REQUIRE(results[1].name == "paladin");
				REQUIRE(results[1].who_name == "Pal");
				REQUIRE(results[1].attr_prime == 2);
				REQUIRE(results[1].align == 1);
				REQUIRE(results[1].weapon == 5);
				REQUIRE(results[1].gainconst == 2);
				REQUIRE(results[1].base_group == "combat");
				REQUIRE(results[1].default_group == "holy");
				REQUIRE(results[1].ctype == 2);
				REQUIRE(results[1].status == 1);
			}

			THEN("it preserves the server's row order (the load-bearing id order)")
			{
				REQUIRE(results[0].name == "warrior");
				REQUIRE(results[1].name == "paladin");
			}
		}
	}

	GIVEN("a session holding no class rows")
	{
		FakeDbSession db; // no rows

		ClassRepository repo(db);

		WHEN("FindAllOrdered is called")
		{
			THEN("it reports an empty result")
			{
				REQUIRE(repo.FindAllOrdered().empty());
			}
		}
	}

	GIVEN("a session whose fetch errors")
	{
		FakeDbSession db;
		db.FetchSucceeds = false;
		db.Rows.push_back(Row({"1", "warrior", "War", "1", "0", "5", "3", "combat", "warfare", "0", "1"}));

		ClassRepository repo(db);

		WHEN("FindAllOrdered is called")
		{
			THEN("it reports an empty result rather than a partial one")
			{
				REQUIRE(repo.FindAllOrdered().empty());
			}
		}
	}
}


//
// CClass::Lookup answers with nothing rather than with a class when the name is
// not one. It used to answer -1, which is a number a class field can hold and
// which three of its callers assigned onward without checking.
//
SCENARIO("looking a class up by name", "[CClass][Lookup]")
{
	TestWorld world;

	GIVEN("a name the class table carries")
	{
		THEN("the class comes back")
		{
			auto found = CClass::Lookup("sorcerer");

			REQUIRE(found);
			REQUIRE(*found == CLASS_SORCERER);
		}
	}

	GIVEN("a name the class table does not carry")
	{
		THEN("nothing comes back, rather than a class")
		{
			REQUIRE_FALSE(CClass::Lookup("bricklayer").has_value());
		}
	}

	GIVEN("an empty name")
	{
		// Characterized, not endorsed. The comparison walks until either
		// string ends and calls that a match, so an empty query is a prefix of
		// the first class in the table and is answered with it. Nothing here
		// changes that.
		THEN("the first class in the table answers")
		{
			auto found = CClass::Lookup("");

			REQUIRE(found);
			REQUIRE(*found == CClass::first->index);
		}
	}

	GIVEN("every class in the table")
	{
		THEN("its own name finds it")
		{
			for (CClass *cclass = CClass::first; cclass != nullptr; cclass = cclass->next)
			{
				auto found = CClass::Lookup(cclass->name.c_str());

				REQUIRE(found);
				REQUIRE(*found == cclass->index);
			}
		}
	}
}
