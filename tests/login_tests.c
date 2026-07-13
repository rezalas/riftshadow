#include "catch.hpp"
#include <algorithm>
#include "db_mocks.h"
#include "../code/repositories/loginrepository.h"

// logins schema column order: name, site, time, ctime, played, obj, lobj, type.
// `time` is a MySQL keyword, so it is backticked in every query that names it.

static const std::string kSelectCols =
	"SELECT name, site, `time`, ctime, played, obj, lobj, type FROM logins";

SCENARIO("recording a login event", "[login][repository]")
{
	GIVEN("a populated login and a session that accepts the insert")
	{
		FakeDbSession db;
		db.ExecuteResult = 1;

		Login login;
		login.name = "Karsus";
		login.site = "host.example.net";
		login.time = "06/25/2004  5:28pm";
		login.ctime = 1088202528;
		login.played = 42;
		login.obj = 7;
		login.lobj = 3;
		login.type = 2;

		LoginRepository repo(db);

		WHEN("Add is called")
		{
			bool ok = repo.Add(login);

			THEN("it inserts an explicit column list, binding eight columns in order")
			{
				REQUIRE(ok);
				REQUIRE(db.ExecuteCount == 1);
				REQUIRE(db.LastSql ==
					"INSERT INTO logins (name, site, `time`, ctime, played, obj, lobj, type) "
					"VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
				REQUIRE(db.Binds == std::vector<std::string>{
					"Karsus", "host.example.net", "06/25/2004  5:28pm",
					"1088202528", "42", "7", "3", "2"});
			}
		}
	}

	GIVEN("a session whose insert errors")
	{
		FakeDbSession db;
		db.ExecuteResult = -1;

		Login login;
		login.name = "X";

		LoginRepository repo(db);

		WHEN("Add is called")
		{
			THEN("it reports failure")
			{
				REQUIRE(repo.Add(login) == false);
			}
		}
	}

	GIVEN("a SQL-injection payload in the site (which used escape_string)")
	{
		FakeDbSession db;
		db.ExecuteResult = 1;

		Login login;
		login.name = "Karsus";
		login.site = "', '', 0, 0, 0, 0, 0);--";
		login.time = "now";
		login.ctime = 1;
		login.played = -1;
		login.obj = -1;
		login.lobj = -1;
		login.type = 1;

		LoginRepository repo(db);

		WHEN("Add is called")
		{
			repo.Add(login);

			THEN("the SQL stays fixed and the payload never leaves the bound value")
			{
				REQUIRE(std::count(db.LastSql.begin(), db.LastSql.end(), '?') == 8);
				REQUIRE(db.Binds.size() == 8);
				REQUIRE(db.Binds[1] == "', '', 0, 0, 0, 0, 0);--");
			}
		}
	}
}

SCENARIO("finding distinct names from a site", "[login][repository]")
{
	GIVEN("a session holding names that connected from a site")
	{
		FakeDbSession db;
		db.Rows.push_back(Row({"Karsus"}));
		db.Rows.push_back(Row({"Morglum"}));

		LoginRepository repo(db);

		WHEN("FindDistinctNamesBySite is called")
		{
			auto names = repo.FindDistinctNamesBySite("host\\.example\\.net");

			THEN("it selects distinct names, binding the site regex, never SELECT *")
			{
				REQUIRE(db.LastSql == "SELECT DISTINCT name FROM logins WHERE site RLIKE ?");
				REQUIRE(db.LastSql.find("*") == std::string::npos);
				REQUIRE(db.Binds == std::vector<std::string>{"host\\.example\\.net"});
			}

			THEN("it returns the projected names")
			{
				REQUIRE(names == std::vector<std::string>{"Karsus", "Morglum"});
			}
		}
	}

	GIVEN("a session with no matching names")
	{
		FakeDbSession db;	// no rows configured

		LoginRepository repo(db);

		WHEN("FindDistinctNamesBySite is called")
		{
			auto names = repo.FindDistinctNamesBySite("nobody");

			THEN("it returns nothing but still binds the site regex")
			{
				REQUIRE(names.empty());
				REQUIRE(db.Binds == std::vector<std::string>{"nobody"});
			}
		}
	}
}

SCENARIO("searching the login history", "[login][repository]")
{
	GIVEN("a session holding matching login events")
	{
		FakeDbSession db;
		//                    name      site     time     ctime    played obj  lobj type
		db.Rows.push_back(Row({"Karsus", "hostA", "then",  "2000", "12",  "5", "2", "2"}));

		LoginRepository repo(db);

		WHEN("Search is called with no type filter")
		{
			auto results = repo.Search("Kar", -1);

			THEN("it matches name/site/time regexes, newest first, with no type clause")
			{
				REQUIRE(db.LastSql == kSelectCols +
					" WHERE (name RLIKE ? OR site RLIKE ? OR `time` RLIKE ?) ORDER BY ctime DESC");
				REQUIRE(db.LastSql.find("*") == std::string::npos);
				REQUIRE(db.Binds == std::vector<std::string>{"Kar", "Kar", "Kar"});
			}

			THEN("it maps every column onto the entity in schema order")
			{
				REQUIRE(results.size() == 1);
				REQUIRE(results[0].name == "Karsus");
				REQUIRE(results[0].site == "hostA");
				REQUIRE(results[0].time == "then");
				REQUIRE(results[0].ctime == 2000);
				REQUIRE(results[0].played == 12);
				REQUIRE(results[0].obj == 5);
				REQUIRE(results[0].lobj == 2);
				REQUIRE(results[0].type == 2);
			}
		}
	}

	GIVEN("a session for a type-filtered search")
	{
		FakeDbSession db;

		LoginRepository repo(db);

		WHEN("Search is called with a type filter")
		{
			repo.Search("Kar", 2);

			THEN("it appends the type clause and binds the pattern thrice then the type")
			{
				REQUIRE(db.LastSql == kSelectCols +
					" WHERE (name RLIKE ? OR site RLIKE ? OR `time` RLIKE ?) AND type = ? "
					"ORDER BY ctime DESC");
				REQUIRE(db.Binds == std::vector<std::string>{"Kar", "Kar", "Kar", "2"});
			}
		}
	}
}

SCENARIO("pruning old logins", "[login][repository]")
{
	GIVEN("a session that reports rows removed")
	{
		FakeDbSession db;
		db.ExecuteResult = 6;

		LoginRepository repo(db);

		WHEN("RemoveOlderThan is called")
		{
			int removed = repo.RemoveOlderThan(500);

			THEN("it deletes by the bound cutoff on ctime")
			{
				REQUIRE(removed == 6);
				REQUIRE(db.LastSql == "DELETE FROM logins WHERE ctime < ?");
				REQUIRE(db.Binds == std::vector<std::string>{"500"});
			}
		}
	}

	GIVEN("a session whose delete errors")
	{
		FakeDbSession db;
		db.ExecuteResult = -1;

		LoginRepository repo(db);

		WHEN("RemoveOlderThan is called")
		{
			THEN("it reports zero rows removed")
			{
				REQUIRE(repo.RemoveOlderThan(500) == 0);
			}
		}
	}
}

SCENARIO("removing a character's logins", "[login][repository]")
{
	GIVEN("a session that reports rows removed")
	{
		FakeDbSession db;
		db.ExecuteResult = 3;

		LoginRepository repo(db);

		WHEN("RemoveByName is called")
		{
			int removed = repo.RemoveByName("Karsus");

			THEN("it deletes by the bound character name")
			{
				REQUIRE(removed == 3);
				REQUIRE(db.LastSql == "DELETE FROM logins WHERE name = ?");
				REQUIRE(db.Binds == std::vector<std::string>{"Karsus"});
			}
		}
	}
}
