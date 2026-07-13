#include "catch.hpp"
#include <algorithm>
#include "db_mocks.h"
#include "../code/repositories/bugrepository.h"

// buglist schema column order: ID, AddedBy, Date, Summary, Description, Status.
// `Date` is a MySQL keyword, so it is backticked in every query that names it.

SCENARIO("finding a bug by id", "[bug][repository]")
{
	GIVEN("a session holding the requested bug")
	{
		FakeDbSession db;
		//                    ID    AddedBy       Date          Summary       Description   Status
		db.Rows.push_back(Row({"7", "Calenduil", "08/18/2001", "Troopers",   "Full text.", "0"}));

		BugRepository repo(db);

		WHEN("FindById is called")
		{
			auto bugs = repo.FindById(7);

			THEN("it selects explicit columns with the id bound, never SELECT *")
			{
				REQUIRE(db.PrepareCount == 1);
				REQUIRE(db.LastSql ==
					"SELECT ID, AddedBy, `Date`, Summary, Description, Status "
					"FROM buglist WHERE ID = ?");
				REQUIRE(db.LastSql.find("*") == std::string::npos);
				REQUIRE(db.Binds == std::vector<std::string>{"7"});
			}

			THEN("it maps every column onto the entity in schema order")
			{
				REQUIRE(bugs.size() == 1);
				REQUIRE(bugs[0].ID == 7);
				REQUIRE(bugs[0].AddedBy == "Calenduil");
				REQUIRE(bugs[0].Date == "08/18/2001");
				REQUIRE(bugs[0].Summary == "Troopers");
				REQUIRE(bugs[0].Description == "Full text.");
				REQUIRE(bugs[0].Status == 0);
			}
		}
	}

	GIVEN("a session with no matching bug")
	{
		FakeDbSession db;	// no rows configured

		BugRepository repo(db);

		WHEN("FindById is called")
		{
			auto bugs = repo.FindById(999);

			THEN("it returns nothing but still binds the requested id")
			{
				REQUIRE(bugs.empty());
				REQUIRE(db.Binds == std::vector<std::string>{"999"});
			}
		}
	}
}

SCENARIO("finding unresolved bugs", "[bug][repository]")
{
	GIVEN("a session holding unresolved bugs")
	{
		FakeDbSession db;
		db.Rows.push_back(Row({"2", "Karsus", "date", "b", "d", "0"}));
		db.Rows.push_back(Row({"1", "Morglum", "date", "a", "d", "0"}));

		BugRepository repo(db);

		WHEN("FindUnresolved is called")
		{
			auto bugs = repo.FindUnresolved(8);

			THEN("it filters Status = 0, orders newest first, and binds the limit")
			{
				REQUIRE(db.LastSql ==
					"SELECT ID, AddedBy, `Date`, Summary, Description, Status "
					"FROM buglist WHERE Status = 0 ORDER BY ID DESC LIMIT ?");
				REQUIRE(db.Binds == std::vector<std::string>{"8"});
				REQUIRE(bugs.size() == 2);
			}
		}
	}
}

SCENARIO("finding recent bugs regardless of status", "[bug][repository]")
{
	GIVEN("a session holding bugs")
	{
		FakeDbSession db;
		db.Rows.push_back(Row({"12", "Erifdel", "date", "s", "d", "1"}));

		BugRepository repo(db);

		WHEN("FindRecent is called")
		{
			auto bugs = repo.FindRecent(5);

			THEN("it orders newest first with no status filter and binds the limit")
			{
				REQUIRE(db.LastSql ==
					"SELECT ID, AddedBy, `Date`, Summary, Description, Status "
					"FROM buglist ORDER BY ID DESC LIMIT ?");
				REQUIRE(db.Binds == std::vector<std::string>{"5"});
				REQUIRE(bugs.size() == 1);
				REQUIRE(bugs[0].Status == 1);
			}
		}
	}
}

SCENARIO("adding a bug report", "[bug][repository]")
{
	GIVEN("a populated bug and a session that accepts the insert")
	{
		FakeDbSession db;
		db.ExecuteResult = 1;

		Bug bug;
		bug.AddedBy = "Reporter";
		bug.Date = "07/13/2026";
		bug.Summary = "Something is broken";
		bug.Description = "A longer description.";
		bug.Status = 0;

		BugRepository repo(db);

		WHEN("Add is called")
		{
			bool ok = repo.Add(bug);

			THEN("it inserts an explicit column list, binding five columns in order")
			{
				REQUIRE(ok);
				REQUIRE(db.ExecuteCount == 1);
				REQUIRE(db.LastSql ==
					"INSERT INTO buglist (AddedBy, `Date`, Summary, Description, Status) "
					"VALUES (?, ?, ?, ?, ?)");
				REQUIRE(db.Binds == std::vector<std::string>{
					"Reporter", "07/13/2026", "Something is broken",
					"A longer description.", "0"});
			}
		}
	}

	GIVEN("a session whose insert errors")
	{
		FakeDbSession db;
		db.ExecuteResult = -1;

		Bug bug;
		bug.AddedBy = "X";

		BugRepository repo(db);

		WHEN("Add is called")
		{
			THEN("it reports failure")
			{
				REQUIRE(repo.Add(bug) == false);
			}
		}
	}

	GIVEN("a SQL-injection payload in the description (which used escape-free interpolation)")
	{
		FakeDbSession db;
		db.ExecuteResult = 1;

		Bug bug;
		bug.AddedBy = "X";
		bug.Date = "d";
		bug.Summary = "s";
		bug.Description = "'),(nullptr,'x','y','z','w',1);--";
		bug.Status = 0;

		BugRepository repo(db);

		WHEN("Add is called")
		{
			repo.Add(bug);

			THEN("the SQL stays fixed and the payload never leaves the bound value")
			{
				REQUIRE(db.LastSql ==
					"INSERT INTO buglist (AddedBy, `Date`, Summary, Description, Status) "
					"VALUES (?, ?, ?, ?, ?)");
				REQUIRE(std::count(db.LastSql.begin(), db.LastSql.end(), '?') == 5);
				REQUIRE(db.Binds.size() == 5);
				REQUIRE(db.Binds[3] == "'),(nullptr,'x','y','z','w',1);--");
			}
		}
	}
}

SCENARIO("marking a bug resolved", "[bug][repository]")
{
	GIVEN("a session that reports one row updated")
	{
		FakeDbSession db;
		db.ExecuteResult = 1;

		BugRepository repo(db);

		WHEN("MarkResolved is called")
		{
			int updated = repo.MarkResolved(3);

			THEN("it sets Status = 1 by the bound id, not interpolated")
			{
				REQUIRE(updated == 1);
				REQUIRE(db.ExecuteCount == 1);
				REQUIRE(db.LastSql == "UPDATE buglist SET Status = 1 WHERE ID = ?");
				REQUIRE(db.Binds == std::vector<std::string>{"3"});
			}
		}
	}

	GIVEN("a session whose update errors")
	{
		FakeDbSession db;
		db.ExecuteResult = -1;

		BugRepository repo(db);

		WHEN("MarkResolved is called")
		{
			THEN("it reports zero rows updated")
			{
				REQUIRE(repo.MarkResolved(1) == 0);
			}
		}
	}
}
