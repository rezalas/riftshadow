#include "catch.hpp"
#include <algorithm>
#include "db_mocks.h"
#include "../code/repositories/sitetrackerrepository.h"

// sitetracker schema column order: site_id, site_name, denials.
// site_id is an auto_increment PRIMARY KEY, so Add supplies only the other two columns.

static const std::string kSelectCols =
	"SELECT site_id, site_name, denials FROM sitetracker";

SCENARIO("listing every tracked site", "[sitetracker][repository]")
{
	GIVEN("a session holding tracked sites")
	{
		FakeDbSession db;
		//                    site_id site_name  denials
		db.Rows.push_back(Row({"1", "aol.com", "13"}));
		db.Rows.push_back(Row({"3", "bellatlantic.net", "1"}));

		SiteTrackerRepository repo(db);

		WHEN("FindAll is called")
		{
			auto results = repo.FindAll();

			THEN("it selects an explicit column list, never SELECT *")
			{
				REQUIRE(db.LastSql == kSelectCols);
				REQUIRE(db.LastSql.find("*") == std::string::npos);
				REQUIRE(db.Binds.empty());
			}

			THEN("it maps every column onto the entity in schema order")
			{
				REQUIRE(results.size() == 2);
				REQUIRE(results[0].site_id == 1);
				REQUIRE(results[0].site_name == "aol.com");
				REQUIRE(results[0].denials == 13);
				REQUIRE(results[1].site_id == 3);
				REQUIRE(results[1].site_name == "bellatlantic.net");
				REQUIRE(results[1].denials == 1);
			}
		}
	}
}

SCENARIO("viewing a single tracked site by id", "[sitetracker][repository]")
{
	GIVEN("a session holding a matching site")
	{
		FakeDbSession db;
		db.Rows.push_back(Row({"3", "bellatlantic.net", "1"}));

		SiteTrackerRepository repo(db);

		WHEN("FindById is called")
		{
			auto results = repo.FindById(3);

			THEN("it selects the explicit column list by the bound site id, never SELECT *")
			{
				REQUIRE(db.LastSql == kSelectCols + " WHERE site_id = ?");
				REQUIRE(db.LastSql.find("*") == std::string::npos);
				REQUIRE(db.Binds == std::vector<std::string>{"3"});
			}

			THEN("it maps the row onto the entity in schema order")
			{
				REQUIRE(results.size() == 1);
				REQUIRE(results[0].site_id == 3);
				REQUIRE(results[0].site_name == "bellatlantic.net");
				REQUIRE(results[0].denials == 1);
			}
		}
	}

	GIVEN("a session holding no matching site")
	{
		FakeDbSession db; // no rows

		SiteTrackerRepository repo(db);

		WHEN("FindById is called")
		{
			THEN("it reports an empty result")
			{
				REQUIRE(repo.FindById(99).empty());
			}
		}
	}
}

SCENARIO("adding a tracked site", "[sitetracker][repository]")
{
	GIVEN("a populated site and a session that accepts the insert")
	{
		FakeDbSession db;
		db.ExecuteResult = 1;

		SiteTracker site;
		site.site_name = "example.com";

		SiteTrackerRepository repo(db);

		WHEN("Add is called")
		{
			bool ok = repo.Add(site);

			THEN("it inserts an explicit column list, letting the server assign site_id")
			{
				REQUIRE(ok);
				REQUIRE(db.ExecuteCount == 1);
				REQUIRE(db.LastSql ==
					"INSERT INTO sitetracker (site_name, denials) VALUES (?, ?)");
				REQUIRE(db.Binds == std::vector<std::string>{"example.com", "0"});
			}
		}
	}

	GIVEN("a session whose insert errors")
	{
		FakeDbSession db;
		db.ExecuteResult = -1;

		SiteTracker site;
		site.site_name = "example.com";

		SiteTrackerRepository repo(db);

		WHEN("Add is called")
		{
			THEN("it reports failure")
			{
				REQUIRE(repo.Add(site) == false);
			}
		}
	}

	GIVEN("a SQL-injection payload in the site name (which used escape_string)")
	{
		FakeDbSession db;
		db.ExecuteResult = 1;

		SiteTracker site;
		site.site_name = "'); DROP TABLE sitetracker;--";

		SiteTrackerRepository repo(db);

		WHEN("Add is called")
		{
			repo.Add(site);

			THEN("the SQL stays fixed and the payload never leaves the bound value")
			{
				REQUIRE(std::count(db.LastSql.begin(), db.LastSql.end(), '?') == 2);
				REQUIRE(db.Binds.size() == 2);
				REQUIRE(db.Binds[0] == "'); DROP TABLE sitetracker;--");
			}
		}
	}
}

SCENARIO("incrementing denials by host", "[sitetracker][repository]")
{
	GIVEN("a session that reports a row updated")
	{
		FakeDbSession db;
		db.ExecuteResult = 1;

		SiteTrackerRepository repo(db);

		WHEN("IncrementDenialsByHost is called")
		{
			int updated = repo.IncrementDenialsByHost("someguy.aol.com");

			THEN("it updates via a bound RLIKE with the host as the subject")
			{
				REQUIRE(updated == 1);
				REQUIRE(db.LastSql ==
					"UPDATE sitetracker SET denials = denials + 1 WHERE ? RLIKE site_name");
				REQUIRE(db.Binds == std::vector<std::string>{"someguy.aol.com"});
			}
		}
	}

	GIVEN("a session whose update errors")
	{
		FakeDbSession db;
		db.ExecuteResult = -1;

		SiteTrackerRepository repo(db);

		WHEN("IncrementDenialsByHost is called")
		{
			THEN("it reports zero rows updated")
			{
				REQUIRE(repo.IncrementDenialsByHost("someguy.aol.com") == 0);
			}
		}
	}
}

SCENARIO("removing a tracked site", "[sitetracker][repository]")
{
	GIVEN("a session that reports a row removed")
	{
		FakeDbSession db;
		db.ExecuteResult = 1;

		SiteTrackerRepository repo(db);

		WHEN("Remove is called")
		{
			int removed = repo.Remove(3);

			THEN("it deletes by the bound site_id primary key")
			{
				REQUIRE(removed == 1);
				REQUIRE(db.LastSql == "DELETE FROM sitetracker WHERE site_id = ?");
				REQUIRE(db.Binds == std::vector<std::string>{"3"});
			}
		}
	}

	GIVEN("a session whose delete errors")
	{
		FakeDbSession db;
		db.ExecuteResult = -1;

		SiteTrackerRepository repo(db);

		WHEN("Remove is called")
		{
			THEN("it reports zero rows removed")
			{
				REQUIRE(repo.Remove(3) == 0);
			}
		}
	}
}
