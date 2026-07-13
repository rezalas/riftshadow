#include "catch.hpp"
#include <algorithm>
#include "db_mocks.h"
#include "../code/repositories/offeringrepository.h"

// offerings schema column order: deity, offeringvnum, offeringshort, player,
// status, time, id. `time` is a MySQL keyword, so it is backticked in every
// query that names it.

static const std::string kSelectCols =
	"SELECT deity, offeringvnum, offeringshort, player, status, `time`, id FROM offerings";

SCENARIO("finding a deity's offerings", "[offering][repository]")
{
	GIVEN("a session holding offerings to a deity")
	{
		FakeDbSession db;
		//                    deity      vnum   short          player     status time    id
		db.Rows.push_back(Row({"Erifdel", "1234", "a jade idol", "Karsus", "0",  "1000", "7"}));
		db.Rows.push_back(Row({"Erifdel", "5678", "a gold ring", "Morglum", "2", "2000", "9"}));

		OfferingRepository repo(db);

		WHEN("FindByDeity is called")
		{
			auto offers = repo.FindByDeity("Erifdel");

			THEN("it selects explicit columns for the deity, oldest first, never SELECT *")
			{
				REQUIRE(db.PrepareCount == 1);
				REQUIRE(db.LastSql == kSelectCols + " WHERE deity = ? ORDER BY `time` ASC");
				REQUIRE(db.LastSql.find("*") == std::string::npos);
				REQUIRE(db.Binds == std::vector<std::string>{"Erifdel"});
			}

			THEN("it maps every column onto the entity in schema order")
			{
				REQUIRE(offers.size() == 2);
				REQUIRE(offers[0].deity == "Erifdel");
				REQUIRE(offers[0].offeringvnum == 1234);
				REQUIRE(offers[0].offeringshort == "a jade idol");
				REQUIRE(offers[0].player == "Karsus");
				REQUIRE(offers[0].status == 0);
				REQUIRE(offers[0].time == 1000);
				REQUIRE(offers[0].id == 7);
				REQUIRE(offers[1].status == 2);
				REQUIRE(offers[1].id == 9);
			}
		}
	}
}

SCENARIO("finding a deity's unviewed offerings", "[offering][repository]")
{
	GIVEN("a session holding unviewed offerings")
	{
		FakeDbSession db;
		db.Rows.push_back(Row({"Erifdel", "1", "an idol", "Karsus", "0", "1000", "3"}));

		OfferingRepository repo(db);

		WHEN("FindUnviewedByDeity is called")
		{
			auto offers = repo.FindUnviewedByDeity("Erifdel");

			THEN("it filters status = 0 for the deity, oldest first, binding only the deity")
			{
				REQUIRE(db.LastSql ==
					kSelectCols + " WHERE deity = ? AND status = 0 ORDER BY `time` ASC");
				REQUIRE(db.Binds == std::vector<std::string>{"Erifdel"});
				REQUIRE(offers.size() == 1);
			}
		}
	}

	GIVEN("a session with no unviewed offerings")
	{
		FakeDbSession db;	// no rows configured

		OfferingRepository repo(db);

		WHEN("FindUnviewedByDeity is called")
		{
			auto offers = repo.FindUnviewedByDeity("Erifdel");

			THEN("it returns nothing but still binds the deity")
			{
				REQUIRE(offers.empty());
				REQUIRE(db.Binds == std::vector<std::string>{"Erifdel"});
			}
		}
	}
}

SCENARIO("finding a player's latest offering to a deity", "[offering][repository]")
{
	GIVEN("a session holding the player's most recent offering")
	{
		FakeDbSession db;
		db.Rows.push_back(Row({"Erifdel", "9", "a relic", "Karsus", "1", "3000", "11"}));

		OfferingRepository repo(db);

		WHEN("FindLatestByDeityAndPlayer is called")
		{
			auto offers = repo.FindLatestByDeityAndPlayer("Erifdel", "Karsus");

			THEN("it orders newest first, limits to one, and binds deity then player")
			{
				REQUIRE(db.LastSql ==
					kSelectCols + " WHERE deity = ? AND player = ? ORDER BY `time` DESC LIMIT 1");
				REQUIRE(db.Binds == std::vector<std::string>{"Erifdel", "Karsus"});
				REQUIRE(offers.size() == 1);
				REQUIRE(offers[0].status == 1);
			}
		}
	}
}

SCENARIO("adding an offering", "[offering][repository]")
{
	GIVEN("a populated offering and a session that accepts the insert")
	{
		FakeDbSession db;
		db.ExecuteResult = 1;

		Offering offering;
		offering.deity = "Erifdel";
		offering.offeringvnum = 4242;
		offering.offeringshort = "a silver chalice";
		offering.player = "Karsus";
		offering.status = 0;
		offering.time = 123456;

		OfferingRepository repo(db);

		WHEN("Add is called")
		{
			bool ok = repo.Add(offering);

			THEN("it inserts an explicit column list (no id), binding six columns in order")
			{
				REQUIRE(ok);
				REQUIRE(db.ExecuteCount == 1);
				REQUIRE(db.LastSql ==
					"INSERT INTO offerings (deity, offeringvnum, offeringshort, player, status, `time`) "
					"VALUES (?, ?, ?, ?, ?, ?)");
				REQUIRE(db.Binds == std::vector<std::string>{
					"Erifdel", "4242", "a silver chalice", "Karsus", "0", "123456"});
			}
		}
	}

	GIVEN("a session whose insert errors")
	{
		FakeDbSession db;
		db.ExecuteResult = -1;

		Offering offering;
		offering.deity = "X";

		OfferingRepository repo(db);

		WHEN("Add is called")
		{
			THEN("it reports failure")
			{
				REQUIRE(repo.Add(offering) == false);
			}
		}
	}

	GIVEN("a SQL-injection payload in the short description (which used escape_string)")
	{
		FakeDbSession db;
		db.ExecuteResult = 1;

		Offering offering;
		offering.deity = "Erifdel";
		offering.offeringvnum = 1;
		offering.offeringshort = "', 0, 0, nullptr);--";
		offering.player = "Karsus";
		offering.status = 0;
		offering.time = 1;

		OfferingRepository repo(db);

		WHEN("Add is called")
		{
			repo.Add(offering);

			THEN("the SQL stays fixed and the payload never leaves the bound value")
			{
				REQUIRE(std::count(db.LastSql.begin(), db.LastSql.end(), '?') == 6);
				REQUIRE(db.Binds.size() == 6);
				REQUIRE(db.Binds[2] == "', 0, 0, nullptr);--");
			}
		}
	}
}

SCENARIO("setting an offering's status", "[offering][repository]")
{
	GIVEN("a session that reports one row updated")
	{
		FakeDbSession db;
		db.ExecuteResult = 1;

		OfferingRepository repo(db);

		WHEN("SetStatus is called")
		{
			int updated = repo.SetStatus(7, 2);

			THEN("it updates status by the bound id, not interpolated")
			{
				REQUIRE(updated == 1);
				REQUIRE(db.ExecuteCount == 1);
				REQUIRE(db.LastSql == "UPDATE offerings SET status = ? WHERE id = ?");
				REQUIRE(db.Binds == std::vector<std::string>{"2", "7"});
			}
		}
	}

	GIVEN("a session whose update errors")
	{
		FakeDbSession db;
		db.ExecuteResult = -1;

		OfferingRepository repo(db);

		WHEN("SetStatus is called")
		{
			THEN("it reports zero rows updated")
			{
				REQUIRE(repo.SetStatus(1, 1) == 0);
			}
		}
	}
}

SCENARIO("pruning old offerings", "[offering][repository]")
{
	GIVEN("a session that reports rows removed")
	{
		FakeDbSession db;
		db.ExecuteResult = 4;

		OfferingRepository repo(db);

		WHEN("RemoveOlderThan is called")
		{
			int removed = repo.RemoveOlderThan(500);

			THEN("it deletes by the bound cutoff on the backticked time column")
			{
				REQUIRE(removed == 4);
				REQUIRE(db.LastSql == "DELETE FROM offerings WHERE `time` < ?");
				REQUIRE(db.Binds == std::vector<std::string>{"500"});
			}
		}
	}

	GIVEN("a session whose delete errors")
	{
		FakeDbSession db;
		db.ExecuteResult = -1;

		OfferingRepository repo(db);

		WHEN("RemoveOlderThan is called")
		{
			THEN("it reports zero rows removed")
			{
				REQUIRE(repo.RemoveOlderThan(500) == 0);
			}
		}
	}
}

SCENARIO("removing a player's offerings", "[offering][repository]")
{
	GIVEN("a session that reports rows removed")
	{
		FakeDbSession db;
		db.ExecuteResult = 2;

		OfferingRepository repo(db);

		WHEN("RemoveByPlayer is called")
		{
			int removed = repo.RemoveByPlayer("Karsus");

			THEN("it deletes by the bound player name")
			{
				REQUIRE(removed == 2);
				REQUIRE(db.LastSql == "DELETE FROM offerings WHERE player = ?");
				REQUIRE(db.Binds == std::vector<std::string>{"Karsus"});
			}
		}
	}
}
