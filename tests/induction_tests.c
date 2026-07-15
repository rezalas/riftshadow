#include "catch.hpp"
#include <algorithm>
#include "db_mocks.h"
#include "../code/repositories/inductionrepository.h"

SCENARIO("recording a cabal induction", "[induction][repository]")
{
	GIVEN("an induction and a session that accepts the insert")
	{
		FakeDbSession db;
		db.ExecuteResult = 1;

		Induction record;
		record.ch = "Seikilos";
		record.victim = "Karsus";
		record.cabal = 1;
		record.ctime = 997515318;
		record.chsite = "nic-28-c151-191.new.rr.com";
		record.victimsite = "cbr-56K-241.tpgi.com.au";

		InductionRepository repo(db);

		WHEN("Add is called")
		{
			bool ok = repo.Add(record);

			THEN("it executes an INSERT binding every column in order, ID omitted")
			{
				REQUIRE(ok);
				REQUIRE(db.ExecuteCount == 1);
				REQUIRE(db.LastSql ==
					"INSERT INTO inductions (ch, victim, cabal, ctime, chsite, victimsite) "
					"VALUES (?, ?, ?, ?, ?, ?)");
				REQUIRE(db.LastSql.find("ID") == std::string::npos);
				REQUIRE(db.Binds == std::vector<std::string>{
					"Seikilos", "Karsus", "1", "997515318",
					"nic-28-c151-191.new.rr.com", "cbr-56K-241.tpgi.com.au"});
			}
		}
	}

	GIVEN("a session whose insert errors")
	{
		FakeDbSession db;
		db.ExecuteResult = -1;

		Induction record;
		record.ch = "Seikilos";

		InductionRepository repo(db);

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
		Induction record;
		record.ch = "Seikilos";
		record.victim = "'),('x','y',1,0,'z','w');--";
		record.cabal = 1;
		record.ctime = 997515318;

		InductionRepository repo(db);

		WHEN("Add is called")
		{
			repo.Add(record);

			THEN("the SQL stays fixed and the payload never leaves the bound value")
			{
				REQUIRE(db.LastSql ==
					"INSERT INTO inductions (ch, victim, cabal, ctime, chsite, victimsite) "
					"VALUES (?, ?, ?, ?, ?, ?)");
				REQUIRE(std::count(db.LastSql.begin(), db.LastSql.end(), '?') == 6);
				REQUIRE(db.Binds.size() == 6);
				REQUIRE(db.Binds[1] == "'),('x','y',1,0,'z','w');--");
			}
		}
	}
}

SCENARIO("removing a character's induction records", "[induction][repository]")
{
	GIVEN("a session that removes two rows")
	{
		FakeDbSession db;
		db.ExecuteResult = 2;

		InductionRepository repo(db);

		WHEN("RemoveByChar is called")
		{
			int removed = repo.RemoveByChar("Seikilos");

			THEN("it deletes with the name bound, not interpolated")
			{
				REQUIRE(removed == 2);
				REQUIRE(db.LastSql == "DELETE FROM inductions WHERE ch = ?");
				REQUIRE(db.Binds == std::vector<std::string>{"Seikilos"});
			}
		}
	}

	GIVEN("a session where nothing matches")
	{
		FakeDbSession db;
		db.ExecuteResult = 0;

		InductionRepository repo(db);

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

		InductionRepository repo(db);

		WHEN("RemoveByChar is called")
		{
			THEN("it treats the error as 0 rows removed")
			{
				REQUIRE(repo.RemoveByChar("Seikilos") == 0);
			}
		}
	}
}
