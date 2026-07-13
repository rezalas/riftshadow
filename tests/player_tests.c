#include "catch.hpp"
#include <algorithm>
#include "db_mocks.h"
#include "../code/repositories/playerrepository.h"

// players schema column order:
//   name, lastlogin, level, class, race, cabal, sex, hours, align, ethos,
//   logins, noc_logins, c_logins, gold, pks, induct
// The `class` column maps to the member `class_` (C++ keyword). It stays bare
// `class` in the SQL -- it is not a MySQL reserved word.
// The `players` table lives in `rift_core`, so the live code constructs the
// repository with RS.Db (not RS.DbRift).

static Player MakePlayer()
{
	Player p;
	p.name = "Karsus";
	p.lastlogin = 1088202528;
	p.level = 51;
	p.class_ = 3;
	p.race = 4;
	p.cabal = 2;
	p.sex = 1;
	p.hours = 340;
	p.align = -500;
	p.ethos = 1;
	p.logins = 12;
	p.noc_logins = 7;
	p.c_logins = 5;
	p.gold = 987654;
	p.pks = 42.0f;
	p.induct = 6;
	return p;
}

SCENARIO("creating a new character stats row", "[player][repository]")
{
	GIVEN("a populated player and a session that accepts the insert")
	{
		FakeDbSession db;
		db.ExecuteResult = 1;

		Player p = MakePlayer();

		PlayerRepository repo(db);

		WHEN("Add is called")
		{
			bool ok = repo.Add(p);

			THEN("it inserts an explicit 16-column list, never a positional VALUES")
			{
				REQUIRE(ok);
				REQUIRE(db.ExecuteCount == 1);
				REQUIRE(db.LastSql ==
					"INSERT INTO players "
					"(name, lastlogin, level, class, race, cabal, sex, hours, align, ethos, "
					"logins, noc_logins, c_logins, gold, pks, induct) "
					"VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
				REQUIRE(db.LastSql.find("*") == std::string::npos);
			}

			THEN("it binds all sixteen columns in schema order")
			{
				REQUIRE(db.Binds.size() == 16);
				REQUIRE(db.Binds[0] == "Karsus");
				REQUIRE(db.Binds[1] == "1088202528");
				REQUIRE(db.Binds[2] == "51");
				REQUIRE(db.Binds[3] == "3");	// class
				REQUIRE(db.Binds[4] == "4");
				REQUIRE(db.Binds[5] == "2");
				REQUIRE(db.Binds[6] == "1");
				REQUIRE(db.Binds[7] == "340");
				REQUIRE(db.Binds[8] == "-500");
				REQUIRE(db.Binds[9] == "1");
				REQUIRE(db.Binds[10] == "12");
				REQUIRE(db.Binds[11] == "7");
				REQUIRE(db.Binds[12] == "5");
				REQUIRE(db.Binds[13] == "987654");
				REQUIRE(db.Binds[14] == std::to_string((double)p.pks));
				REQUIRE(db.Binds[15] == "6");
			}
		}
	}

	GIVEN("a session whose insert errors")
	{
		FakeDbSession db;
		db.ExecuteResult = -1;

		Player p;
		p.name = "X";

		PlayerRepository repo(db);

		WHEN("Add is called")
		{
			THEN("it reports failure")
			{
				REQUIRE(repo.Add(p) == false);
			}
		}
	}

	GIVEN("a SQL-injection payload in the name (which used %s interpolation)")
	{
		FakeDbSession db;
		db.ExecuteResult = 1;

		Player p;
		p.name = "', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);--";

		PlayerRepository repo(db);

		WHEN("Add is called")
		{
			repo.Add(p);

			THEN("the SQL stays fixed and the payload never leaves the bound value")
			{
				REQUIRE(std::count(db.LastSql.begin(), db.LastSql.end(), '?') == 16);
				REQUIRE(db.Binds.size() == 16);
				REQUIRE(db.Binds[0] == "', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);--");
			}
		}
	}
}

SCENARIO("saving an existing character's stats", "[player][repository]")
{
	GIVEN("a populated player and a session that accepts the update")
	{
		FakeDbSession db;
		db.ExecuteResult = 1;

		Player p = MakePlayer();

		PlayerRepository repo(db);

		WHEN("SaveStats is called")
		{
			bool ok = repo.SaveStats(p);

			THEN("it updates the mutable stat columns keyed on name, LIMIT 1")
			{
				REQUIRE(ok);
				REQUIRE(db.LastSql ==
					"UPDATE players SET pks = ?, level = ?, class = ?, race = ?, cabal = ?, "
					"sex = ?, hours = ?, align = ?, ethos = ?, gold = ? WHERE name = ? LIMIT 1");
			}

			THEN("it binds the ten stat columns then the name key")
			{
				REQUIRE(db.Binds.size() == 11);
				REQUIRE(db.Binds[0] == std::to_string((double)p.pks));
				REQUIRE(db.Binds[1] == "51");	// level
				REQUIRE(db.Binds[2] == "3");	// class
				REQUIRE(db.Binds[3] == "4");	// race
				REQUIRE(db.Binds[4] == "2");	// cabal
				REQUIRE(db.Binds[5] == "1");	// sex
				REQUIRE(db.Binds[6] == "340");	// hours
				REQUIRE(db.Binds[7] == "-500");	// align
				REQUIRE(db.Binds[8] == "1");	// ethos
				REQUIRE(db.Binds[9] == "987654"); // gold
				REQUIRE(db.Binds[10] == "Karsus"); // name key
			}
		}
	}

	GIVEN("a session whose update errors")
	{
		FakeDbSession db;
		db.ExecuteResult = -1;

		PlayerRepository repo(db);

		WHEN("SaveStats is called")
		{
			THEN("it reports failure")
			{
				REQUIRE(repo.SaveStats(MakePlayer()) == false);
			}
		}
	}
}

SCENARIO("recording a login", "[player][repository]")
{
	GIVEN("a session that accepts the update")
	{
		FakeDbSession db;
		db.ExecuteResult = 1;

		PlayerRepository repo(db);

		WHEN("RecordLogin is called")
		{
			bool ok = repo.RecordLogin("Karsus", 1088202528);

			THEN("it increments logins in-place and stamps the bound lastlogin/name")
			{
				REQUIRE(ok);
				REQUIRE(db.LastSql ==
					"UPDATE players SET logins = logins + 1, lastlogin = ? WHERE name = ?");
				REQUIRE(db.Binds == std::vector<std::string>{"1088202528", "Karsus"});
			}
		}
	}
}

SCENARIO("renaming a character", "[player][repository]")
{
	GIVEN("a session that accepts the update")
	{
		FakeDbSession db;
		db.ExecuteResult = 1;

		PlayerRepository repo(db);

		WHEN("Rename is called")
		{
			bool ok = repo.Rename("Karsus", "Vecna");

			THEN("it updates name keyed on the old name, binding new then old, LIMIT 1")
			{
				REQUIRE(ok);
				REQUIRE(db.LastSql == "UPDATE players SET name = ? WHERE name = ? LIMIT 1");
				REQUIRE(db.Binds == std::vector<std::string>{"Vecna", "Karsus"});
			}
		}
	}
}

SCENARIO("setting a character's induction rank", "[player][repository]")
{
	GIVEN("a session that accepts the update")
	{
		FakeDbSession db;
		db.ExecuteResult = 1;

		PlayerRepository repo(db);

		WHEN("SetInduct is called")
		{
			bool ok = repo.SetInduct("Karsus", 4);

			THEN("it updates induct keyed on name, binding induct then name")
			{
				REQUIRE(ok);
				REQUIRE(db.LastSql == "UPDATE players SET induct = ? WHERE name = ?");
				REQUIRE(db.Binds == std::vector<std::string>{"4", "Karsus"});
			}
		}
	}
}

SCENARIO("removing a character's stats row", "[player][repository]")
{
	GIVEN("a session that reports a row removed")
	{
		FakeDbSession db;
		db.ExecuteResult = 1;

		PlayerRepository repo(db);

		WHEN("RemoveByName is called")
		{
			int removed = repo.RemoveByName("Karsus");

			THEN("it deletes by the bound name")
			{
				REQUIRE(removed == 1);
				REQUIRE(db.LastSql == "DELETE FROM players WHERE name = ?");
				REQUIRE(db.Binds == std::vector<std::string>{"Karsus"});
			}
		}
	}

	GIVEN("a session whose delete errors")
	{
		FakeDbSession db;
		db.ExecuteResult = -1;

		PlayerRepository repo(db);

		WHEN("RemoveByName is called")
		{
			THEN("it reports zero rows removed")
			{
				REQUIRE(repo.RemoveByName("Karsus") == 0);
			}
		}
	}
}

SCENARIO("listing all character names", "[player][repository]")
{
	GIVEN("a session holding several names")
	{
		FakeDbSession db;
		db.Rows.push_back(Row({"Karsus"}));
		db.Rows.push_back(Row({"Vecna"}));

		PlayerRepository repo(db);

		WHEN("FindAllNames is called")
		{
			auto names = repo.FindAllNames();

			THEN("it selects only names, never SELECT *, and projects them")
			{
				REQUIRE(db.LastSql == "SELECT name FROM players");
				REQUIRE(db.LastSql.find("*") == std::string::npos);
				REQUIRE(names == std::vector<std::string>{"Karsus", "Vecna"});
			}
		}
	}
}

SCENARIO("listing inactive character names", "[player][repository]")
{
	GIVEN("a session holding stale names")
	{
		FakeDbSession db;
		db.Rows.push_back(Row({"Idler"}));

		PlayerRepository repo(db);

		WHEN("FindNamesInactiveSince is called")
		{
			auto names = repo.FindNamesInactiveSince(1000000);

			THEN("it filters on a bound lastlogin cutoff, never interpolating it")
			{
				REQUIRE(db.LastSql == "SELECT name FROM players WHERE lastlogin < ?");
				REQUIRE(db.Binds == std::vector<std::string>{"1000000"});
				REQUIRE(names == std::vector<std::string>{"Idler"});
			}
		}
	}
}

SCENARIO("listing top characters by PK count", "[player][repository]")
{
	GIVEN("a session holding the top killer")
	{
		FakeDbSession db;
		db.Rows.push_back(Row({"Butcher"}));

		PlayerRepository repo(db);

		WHEN("FindTopByPks is called")
		{
			auto names = repo.FindTopByPks(1);

			THEN("it orders by pks desc with a bound LIMIT, never SELECT *")
			{
				REQUIRE(db.LastSql == "SELECT name FROM players ORDER BY pks DESC LIMIT ?");
				REQUIRE(db.LastSql.find("*") == std::string::npos);
				REQUIRE(db.Binds == std::vector<std::string>{"1"});
				REQUIRE(names == std::vector<std::string>{"Butcher"});
			}
		}
	}
}

SCENARIO("counting characters with gold above a level", "[player][repository]")
{
	GIVEN("a session returning the count")
	{
		FakeDbSession db;
		db.Rows.push_back(Row({"137"}));

		PlayerRepository repo(db);

		WHEN("CountGoldAboveLevel is called")
		{
			int count = repo.CountGoldAboveLevel(30);

			THEN("it counts gold above a bound level and returns the scalar")
			{
				REQUIRE(db.LastSql == "SELECT COUNT(gold) FROM players WHERE level > ?");
				REQUIRE(db.Binds == std::vector<std::string>{"30"});
				REQUIRE(count == 137);
			}
		}
	}

	GIVEN("a session returning no rows")
	{
		FakeDbSession db;	// no rows configured

		PlayerRepository repo(db);

		WHEN("CountGoldAboveLevel is called")
		{
			THEN("it returns zero")
			{
				REQUIRE(repo.CountGoldAboveLevel(30) == 0);
			}
		}
	}
}

SCENARIO("summing gold across all characters", "[player][repository]")
{
	GIVEN("a session returning the sum")
	{
		FakeDbSession db;
		db.Rows.push_back(Row({"9000000000"})); // exceeds 32-bit, proves I64 read

		PlayerRepository repo(db);

		WHEN("SumGold is called")
		{
			long long total = repo.SumGold();

			THEN("it sums gold with no filter and returns a 64-bit scalar")
			{
				REQUIRE(db.LastSql == "SELECT SUM(gold) FROM players");
				REQUIRE(db.Binds.empty());
				REQUIRE(total == 9000000000LL);
			}
		}
	}

	GIVEN("a session returning no rows")
	{
		FakeDbSession db;	// no rows configured

		PlayerRepository repo(db);

		WHEN("SumGold is called")
		{
			THEN("it returns zero")
			{
				REQUIRE(repo.SumGold() == 0);
			}
		}
	}
}
