#include "catch.hpp"
#include <algorithm>
#include "db_mocks.h"
#include "../code/repositories/pklogrepository.h"

// pklogs schema column order: killer, killercabal, victim, victimcabal, date, room, ctime.
// `date` is a MySQL keyword, so it is backticked in every query that names it.

static const std::string kSelectCols =
	"SELECT killer, killercabal, victim, victimcabal, `date`, room, ctime FROM pklogs";

SCENARIO("recording a player kill", "[pklog][repository]")
{
	GIVEN("a populated pklog and a session that accepts the insert")
	{
		FakeDbSession db;
		db.ExecuteResult = 1;

		PkLog pklog;
		pklog.killer = "Fyndin";
		pklog.killercabal = 5;
		pklog.victim = "Cohtram";
		pklog.victimcabal = 3;
		pklog.date = "03/24/2004  1:07pm";
		pklog.room = "The Chat Room (1200)";
		pklog.ctime = 1080151669;

		PkLogRepository repo(db);

		WHEN("Add is called")
		{
			bool ok = repo.Add(pklog);

			THEN("it inserts an explicit column list, binding seven columns in order")
			{
				REQUIRE(ok);
				REQUIRE(db.ExecuteCount == 1);
				REQUIRE(db.LastSql ==
					"INSERT INTO pklogs (killer, killercabal, victim, victimcabal, `date`, room, ctime) "
					"VALUES (?, ?, ?, ?, ?, ?, ?)");
				REQUIRE(db.Binds == std::vector<std::string>{
					"Fyndin", "5", "Cohtram", "3",
					"03/24/2004  1:07pm", "The Chat Room (1200)", "1080151669"});
			}
		}
	}

	GIVEN("a session whose insert errors")
	{
		FakeDbSession db;
		db.ExecuteResult = -1;

		PkLog pklog;
		pklog.killer = "X";

		PkLogRepository repo(db);

		WHEN("Add is called")
		{
			THEN("it reports failure")
			{
				REQUIRE(repo.Add(pklog) == false);
			}
		}
	}

	GIVEN("a SQL-injection payload in the room (which used escape_string)")
	{
		FakeDbSession db;
		db.ExecuteResult = 1;

		PkLog pklog;
		pklog.killer = "Fyndin";
		pklog.killercabal = 0;
		pklog.victim = "Cohtram";
		pklog.victimcabal = 0;
		pklog.date = "now";
		pklog.room = "', 0, '', 0, '', '', 0);--";
		pklog.ctime = 1;

		PkLogRepository repo(db);

		WHEN("Add is called")
		{
			repo.Add(pklog);

			THEN("the SQL stays fixed and the payload never leaves the bound value")
			{
				REQUIRE(std::count(db.LastSql.begin(), db.LastSql.end(), '?') == 7);
				REQUIRE(db.Binds.size() == 7);
				REQUIRE(db.Binds[5] == "', 0, '', 0, '', '', 0);--");
			}
		}
	}
}

SCENARIO("searching the kill history", "[pklog][repository]")
{
	GIVEN("a session holding matching kills")
	{
		FakeDbSession db;
		//                    killer   kcab victim    vcab date     room     ctime
		db.Rows.push_back(Row({"Fyndin", "5", "Cohtram", "3", "then", "hall", "2000"}));

		PkLogRepository repo(db);

		WHEN("Search matches on the killer (wins)")
		{
			auto results = repo.Search(PkLogField::Killer, "Fyn");

			THEN("it matches only the killer regex, never SELECT *")
			{
				REQUIRE(db.LastSql == kSelectCols + " WHERE killer RLIKE ?");
				REQUIRE(db.LastSql.find("*") == std::string::npos);
				REQUIRE(db.Binds == std::vector<std::string>{"Fyn"});
			}

			THEN("it maps every column onto the entity in schema order")
			{
				REQUIRE(results.size() == 1);
				REQUIRE(results[0].killer == "Fyndin");
				REQUIRE(results[0].killercabal == 5);
				REQUIRE(results[0].victim == "Cohtram");
				REQUIRE(results[0].victimcabal == 3);
				REQUIRE(results[0].date == "then");
				REQUIRE(results[0].room == "hall");
				REQUIRE(results[0].ctime == 2000);
			}
		}
	}

	GIVEN("a session for each single-column search mode")
	{
		FakeDbSession db;
		PkLogRepository repo(db);

		WHEN("Search matches on the victim (losses)")
		{
			repo.Search(PkLogField::Victim, "Coh");

			THEN("it matches only the victim regex")
			{
				REQUIRE(db.LastSql == kSelectCols + " WHERE victim RLIKE ?");
				REQUIRE(db.Binds == std::vector<std::string>{"Coh"});
			}
		}

		WHEN("Search matches on the date")
		{
			repo.Search(PkLogField::Date, "2004");

			THEN("it matches the backticked date keyword column")
			{
				REQUIRE(db.LastSql == kSelectCols + " WHERE `date` RLIKE ?");
				REQUIRE(db.Binds == std::vector<std::string>{"2004"});
			}
		}

		WHEN("Search matches on the room (location)")
		{
			repo.Search(PkLogField::Room, "Chat");

			THEN("it matches only the room regex")
			{
				REQUIRE(db.LastSql == kSelectCols + " WHERE room RLIKE ?");
				REQUIRE(db.Binds == std::vector<std::string>{"Chat"});
			}
		}
	}

	GIVEN("a session for the killer-or-victim search mode")
	{
		FakeDbSession db;
		PkLogRepository repo(db);

		WHEN("Search matches on killer or victim (all)")
		{
			repo.Search(PkLogField::KillerOrVictim, "Fyn");

			THEN("it ORs both columns and binds the pattern twice")
			{
				REQUIRE(db.LastSql == kSelectCols + " WHERE killer RLIKE ? OR victim RLIKE ?");
				REQUIRE(db.Binds == std::vector<std::string>{"Fyn", "Fyn"});
			}
		}
	}
}

SCENARIO("pruning old kills", "[pklog][repository]")
{
	GIVEN("a session that reports rows removed")
	{
		FakeDbSession db;
		db.ExecuteResult = 4;

		PkLogRepository repo(db);

		WHEN("RemoveOlderThan is called")
		{
			int removed = repo.RemoveOlderThan(500);

			THEN("it deletes by the bound cutoff on ctime")
			{
				REQUIRE(removed == 4);
				REQUIRE(db.LastSql == "DELETE FROM pklogs WHERE ctime < ?");
				REQUIRE(db.Binds == std::vector<std::string>{"500"});
			}
		}
	}

	GIVEN("a session whose delete errors")
	{
		FakeDbSession db;
		db.ExecuteResult = -1;

		PkLogRepository repo(db);

		WHEN("RemoveOlderThan is called")
		{
			THEN("it reports zero rows removed")
			{
				REQUIRE(repo.RemoveOlderThan(500) == 0);
			}
		}
	}
}

SCENARIO("removing a killer's kills", "[pklog][repository]")
{
	GIVEN("a session that reports rows removed")
	{
		FakeDbSession db;
		db.ExecuteResult = 2;

		PkLogRepository repo(db);

		WHEN("RemoveByKiller is called")
		{
			int removed = repo.RemoveByKiller("Fyndin");

			THEN("it deletes by the bound killer name only")
			{
				REQUIRE(removed == 2);
				REQUIRE(db.LastSql == "DELETE FROM pklogs WHERE killer = ?");
				REQUIRE(db.Binds == std::vector<std::string>{"Fyndin"});
			}
		}
	}
}
