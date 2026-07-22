#include "catch.hpp"
#include <algorithm>
#include "db_mocks.h"
#include "../code/repositories/banrepository.h"
#include "../code/banflags.h"

SCENARIO("finding bans by type", "[ban][repository]")
{
	GIVEN("a session holding two matching bans")
	{
		FakeDbSession db;
		db.Rows.push_back(Row({"66.66.66.66", "-1"}));
		db.Rows.push_back(Row({"13.13.13.13", "500"}));

		BanRepository repo(db);

		WHEN("FindByType is called")
		{
			auto bans = repo.FindByType(NBAN_NEWBIE, NBAN_HOST);

			THEN("it issues a single parameterised query with the type bound, not interpolated")
			{
				REQUIRE(db.PrepareCount == 1);
				REQUIRE(db.LastSql == "SELECT site, duration FROM bans WHERE ban_type = ? AND host_type = ?");
				REQUIRE(db.Binds == std::vector<std::string>{"1", "0"});
			}

			THEN("it maps only the selected site and duration columns")
			{
				REQUIRE(bans.size() == 2);
				REQUIRE(bans[0].site == "66.66.66.66");
				REQUIRE(bans[0].duration == -1);
				REQUIRE(bans[1].site == "13.13.13.13");
				REQUIRE(bans[1].duration == 500);

				// FindByType deliberately selects only site+duration; the rest stay default.
				REQUIRE(bans[0].by.empty());
				REQUIRE(bans[0].reason.empty());
			}
		}
	}

	GIVEN("a session with no matching bans")
	{
		FakeDbSession db;	// no rows configured

		BanRepository repo(db);

		WHEN("FindByType is called")
		{
			auto bans = repo.FindByType(NBAN_ALL, NBAN_IP);

			THEN("it returns nothing but still binds the requested type")
			{
				REQUIRE(bans.empty());
				REQUIRE(db.Binds == std::vector<std::string>{"0", "1"});
			}
		}
	}
}

SCENARIO("listing all bans for display", "[ban][repository]")
{
	GIVEN("a session holding two bans")
	{
		FakeDbSession db;
		//                    site         by       reason   date          dur    btype  htype
		db.Rows.push_back(Row({"evil.com",  "Morg",  "spam",  "07/04/2026", "-1",  "1",   "0"}));
		db.Rows.push_back(Row({"bad.net",   "Odji",  "abuse", "01/01/2026", "100", "0",   "1"}));

		BanRepository repo(db);

		WHEN("FindAllOrderedByDuration is called")
		{
			auto bans = repo.FindAllOrderedByDuration();

			THEN("it selects explicit columns, formats the date, and orders server-side")
			{
				REQUIRE(db.LastSql.find("DATE_FORMAT(date, '%m/%d/%Y')") != std::string::npos);
				REQUIRE(db.LastSql.find("ORDER BY duration DESC") != std::string::npos);
				REQUIRE(db.LastSql.find("*") == std::string::npos);
				REQUIRE(db.Binds.empty());
			}

			THEN("it maps every column onto the entity")
			{
				REQUIRE(bans.size() == 2);

				REQUIRE(bans[0].site == "evil.com");
				REQUIRE(bans[0].by == "Morg");
				REQUIRE(bans[0].reason == "spam");
				REQUIRE(bans[0].date == "07/04/2026");
				REQUIRE(bans[0].duration == -1);
				REQUIRE(bans[0].ban_type == 1);
				REQUIRE(bans[0].host_type == 0);

				REQUIRE(bans[1].site == "bad.net");
				REQUIRE(bans[1].duration == 100);
				REQUIRE(bans[1].ban_type == 0);
				REQUIRE(bans[1].host_type == 1);
			}
		}
	}
}

SCENARIO("adding a ban", "[ban][repository]")
{
	GIVEN("a fully populated ban and a session that accepts the insert")
	{
		FakeDbSession db;
		db.ExecuteResult = 1;

		Ban ban;
		ban.site = "192.168.0.1";
		ban.by = "Morglum";
		ban.reason = "testing";
		ban.date = "2026-07-11";
		ban.duration = -1;
		ban.ban_type = NBAN_NEWBIE;
		ban.host_type = NBAN_IP;

		BanRepository repo(db);

		WHEN("Add is called")
		{
			bool ok = repo.Add(ban);

			THEN("it executes an INSERT binding every column in order")
			{
				REQUIRE(ok);
				REQUIRE(db.ExecuteCount == 1);
				REQUIRE(db.LastSql.find("INSERT INTO bans") != std::string::npos);
				REQUIRE(db.Binds == std::vector<std::string>{
					"192.168.0.1", "Morglum", "testing", "2026-07-11", "-1", "1", "1"});
			}
		}
	}

	GIVEN("a session whose insert errors")
	{
		FakeDbSession db;
		db.ExecuteResult = -1;

		Ban ban;
		ban.site = "10.0.0.1";

		BanRepository repo(db);

		WHEN("Add is called")
		{
			bool ok = repo.Add(ban);

			THEN("it reports failure")
			{
				REQUIRE(ok == false);
			}
		}
	}
}

SCENARIO("unbanning a site", "[ban][repository]")
{
	GIVEN("a session that removes two rows")
	{
		FakeDbSession db;
		db.ExecuteResult = 2;

		BanRepository repo(db);

		WHEN("RemoveBySiteContaining is called")
		{
			int removed = repo.RemoveBySiteContaining("evil");

			THEN("it deletes with a bound LIKE, wildcards living in the value")
			{
				REQUIRE(removed == 2);
				REQUIRE(db.LastSql == "DELETE FROM bans WHERE site LIKE ?");
				REQUIRE(db.Binds == std::vector<std::string>{"%evil%"});
			}
		}
	}

	GIVEN("a session where nothing matches")
	{
		FakeDbSession db;
		db.ExecuteResult = 0;

		BanRepository repo(db);

		WHEN("RemoveBySiteContaining is called")
		{
			THEN("it returns 0")
			{
				REQUIRE(repo.RemoveBySiteContaining("nosuchsite") == 0);
			}
		}
	}

	GIVEN("a session whose delete errors")
	{
		FakeDbSession db;
		db.ExecuteResult = -1;

		BanRepository repo(db);

		WHEN("RemoveBySiteContaining is called")
		{
			THEN("it treats the error as 0 rows removed")
			{
				REQUIRE(repo.RemoveBySiteContaining("evil") == 0);
			}
		}
	}

	GIVEN("a SQL-injection payload as the fragment")
	{
		FakeDbSession db;
		db.ExecuteResult = 0;

		// The classic payload that would drop the whole table through string
		// interpolation. Here it is a single bound parameter against `LIKE ?`.
		const std::string payload = "' OR '1'='1";

		BanRepository repo(db);

		WHEN("RemoveBySiteContaining is called")
		{
			repo.RemoveBySiteContaining(payload);

			THEN("the SQL stays fixed and the whole payload is confined to the bound value")
			{
				REQUIRE(db.LastSql == "DELETE FROM bans WHERE site LIKE ?");
				REQUIRE(std::count(db.LastSql.begin(), db.LastSql.end(), '?') == 1);
				REQUIRE(db.Binds.size() == 1);
				REQUIRE(db.Binds[0] == "%' OR '1'='1%");
			}
		}
	}
}
