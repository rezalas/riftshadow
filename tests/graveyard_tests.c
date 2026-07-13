#include "catch.hpp"
#include <algorithm>
#include "db_mocks.h"
#include "../code/repositories/graveyardrepository.h"

// graveyard schema column order: ID, Pname, Pfrags, Pfgood, Pfneutral, Pfevil,
// Pfdeaths, Pmdeaths, Phours. ID is an auto_increment PRIMARY KEY, so Add supplies
// only the other eight columns. Columns are PascalCase, mirrored verbatim (same
// convention as buglist, Step 7); none is a MySQL keyword, so nothing is backticked.

static const std::string kInsert =
	"INSERT INTO graveyard "
	"(Pname, Pfrags, Pfgood, Pfneutral, Pfevil, Pfdeaths, Pmdeaths, Phours) "
	"VALUES (?, ?, ?, ?, ?, ?, ?, ?)";

SCENARIO("adding a graveyard memorial row", "[graveyard][repository]")
{
	GIVEN("a populated memorial and a session that accepts the insert")
	{
		FakeDbSession db;
		db.ExecuteResult = 1;

		Graveyard grave;
		grave.Pname = "Gandalf";
		grave.Pfrags = 12.0f;
		grave.Pfgood = 5.0f;
		grave.Pfneutral = 4.0f;
		grave.Pfevil = 3.0f;
		grave.Pfdeaths = 7.0f;
		grave.Pmdeaths = 2;
		grave.Phours = 340;

		GraveyardRepository repo(db);

		WHEN("Add is called")
		{
			bool ok = repo.Add(grave);

			THEN("it inserts an explicit column list, letting the server assign ID")
			{
				REQUIRE(ok);
				REQUIRE(db.ExecuteCount == 1);
				REQUIRE(db.LastSql == kInsert);
				REQUIRE(db.LastSql.find("*") == std::string::npos);
			}

			THEN("it binds every column in schema order, ID omitted")
			{
				REQUIRE(db.Binds.size() == 8);
				REQUIRE(db.Binds[0] == "Gandalf");
				REQUIRE(db.Binds[1] == std::to_string((double)grave.Pfrags));
				REQUIRE(db.Binds[2] == std::to_string((double)grave.Pfgood));
				REQUIRE(db.Binds[3] == std::to_string((double)grave.Pfneutral));
				REQUIRE(db.Binds[4] == std::to_string((double)grave.Pfevil));
				REQUIRE(db.Binds[5] == std::to_string((double)grave.Pfdeaths));
				REQUIRE(db.Binds[6] == "2");
				REQUIRE(db.Binds[7] == "340");
			}
		}
	}

	GIVEN("a session whose insert errors")
	{
		FakeDbSession db;
		db.ExecuteResult = -1;

		Graveyard grave;
		grave.Pname = "Gandalf";

		GraveyardRepository repo(db);

		WHEN("Add is called")
		{
			THEN("it reports failure")
			{
				REQUIRE(repo.Add(grave) == false);
			}
		}
	}

	GIVEN("a SQL-injection payload in the character name")
	{
		FakeDbSession db;
		db.ExecuteResult = 1;

		Graveyard grave;
		grave.Pname = "'); DROP TABLE graveyard;--";

		GraveyardRepository repo(db);

		WHEN("Add is called")
		{
			repo.Add(grave);

			THEN("the SQL stays fixed and the payload never leaves the bound value")
			{
				REQUIRE(std::count(db.LastSql.begin(), db.LastSql.end(), '?') == 8);
				REQUIRE(db.Binds.size() == 8);
				REQUIRE(db.Binds[0] == "'); DROP TABLE graveyard;--");
			}
		}
	}
}
