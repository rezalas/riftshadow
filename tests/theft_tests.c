#include "catch.hpp"
#include <algorithm>
#include "db_mocks.h"
#include "../code/repositories/theftrepository.h"

SCENARIO("recording a theft attempt", "[theft][repository]")
{
	GIVEN("a caught attempt and a session that accepts the insert")
	{
		FakeDbSession db;
		db.ExecuteResult = 1;

		Theft record;
		record.ch = "Morglum";
		record.victim = "Odji";
		record.obj = "none";
		record.success = 0;

		TheftRepository repo(db);

		WHEN("Add is called")
		{
			bool ok = repo.Add(record);

			THEN("it executes an INSERT binding every column in order, ID omitted")
			{
				REQUIRE(ok);
				REQUIRE(db.ExecuteCount == 1);
				REQUIRE(db.LastSql ==
					"INSERT INTO theft (ch, victim, obj, success) VALUES (?, ?, ?, ?)");
				REQUIRE(db.LastSql.find("ID") == std::string::npos);
				REQUIRE(db.Binds == std::vector<std::string>{"Morglum", "Odji", "none", "0"});
			}
		}
	}

	GIVEN("a successful item theft")
	{
		FakeDbSession db;
		db.ExecuteResult = 1;

		Theft record;
		record.ch = "Morglum";
		record.victim = "Odji";
		record.obj = "a jewelled dagger";
		record.success = 1;

		TheftRepository repo(db);

		WHEN("Add is called")
		{
			bool ok = repo.Add(record);

			THEN("the stolen item and success flag are bound as values")
			{
				REQUIRE(ok);
				REQUIRE(db.Binds == std::vector<std::string>{
					"Morglum", "Odji", "a jewelled dagger", "1"});
			}
		}
	}

	GIVEN("a session whose insert errors")
	{
		FakeDbSession db;
		db.ExecuteResult = -1;

		Theft record;
		record.ch = "Morglum";

		TheftRepository repo(db);

		WHEN("Add is called")
		{
			bool ok = repo.Add(record);

			THEN("it reports failure")
			{
				REQUIRE(ok == false);
			}
		}
	}

	GIVEN("a SQL-injection payload in the victim name")
	{
		FakeDbSession db;
		db.ExecuteResult = 1;

		// A payload that would break out of the VALUES list through string
		// interpolation. Here it stays confined to a single bound value.
		Theft record;
		record.ch = "Morglum";
		record.victim = "'),('x','y','z',1);--";
		record.obj = "none";
		record.success = 0;

		TheftRepository repo(db);

		WHEN("Add is called")
		{
			repo.Add(record);

			THEN("the SQL stays fixed and the payload never leaves the bound value")
			{
				REQUIRE(db.LastSql ==
					"INSERT INTO theft (ch, victim, obj, success) VALUES (?, ?, ?, ?)");
				REQUIRE(std::count(db.LastSql.begin(), db.LastSql.end(), '?') == 4);
				REQUIRE(db.Binds.size() == 4);
				REQUIRE(db.Binds[1] == "'),('x','y','z',1);--");
			}
		}
	}
}

SCENARIO("removing a character's theft records", "[theft][repository]")
{
	GIVEN("a session that removes three rows")
	{
		FakeDbSession db;
		db.ExecuteResult = 3;

		TheftRepository repo(db);

		WHEN("RemoveByChar is called")
		{
			int removed = repo.RemoveByChar("Morglum");

			THEN("it deletes with the name bound, not interpolated")
			{
				REQUIRE(removed == 3);
				REQUIRE(db.LastSql == "DELETE FROM theft WHERE ch = ?");
				REQUIRE(db.Binds == std::vector<std::string>{"Morglum"});
			}
		}
	}

	GIVEN("a session where nothing matches")
	{
		FakeDbSession db;
		db.ExecuteResult = 0;

		TheftRepository repo(db);

		WHEN("RemoveByChar is called")
		{
			THEN("it returns 0")
			{
				REQUIRE(repo.RemoveByChar("Nobody") == 0);
			}
		}
	}

	GIVEN("a session whose delete errors")
	{
		FakeDbSession db;
		db.ExecuteResult = -1;

		TheftRepository repo(db);

		WHEN("RemoveByChar is called")
		{
			THEN("it treats the error as 0 rows removed")
			{
				REQUIRE(repo.RemoveByChar("Morglum") == 0);
			}
		}
	}
}
