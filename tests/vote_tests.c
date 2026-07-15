#include "catch.hpp"
#include <algorithm>
#include "db_mocks.h"
#include "../code/repositories/voterepository.h"

SCENARIO("finding the ballots cast in a cabal's election", "[vote][repository]")
{
	GIVEN("a session holding two ballots")
	{
		FakeDbSession db;
		//                    voter        vote_for     cabal  time         host
		db.Rows.push_back(Row({"Plintose",  "Plintose",  "1",   "997924032", "pool-63.53.48.74.irvn.grid.net"}));
		db.Rows.push_back(Row({"Saberhagen","Plintose",  "1",   "997924100", "cbr-56K-241.tpgi.com.au"}));

		VoteRepository repo(db);

		WHEN("FindByCabal is called")
		{
			auto votes = repo.FindByCabal(1);

			THEN("it selects explicit columns with the cabal bound, not interpolated")
			{
				REQUIRE(db.PrepareCount == 1);
				REQUIRE(db.LastSql ==
					"SELECT voter, vote_for, cabal, `time`, host FROM votes WHERE cabal = ?");
				REQUIRE(db.LastSql.find("*") == std::string::npos);
				REQUIRE(db.Binds == std::vector<std::string>{"1"});
			}

			THEN("it maps every column onto the entity in schema order")
			{
				REQUIRE(votes.size() == 2);

				REQUIRE(votes[0].voter == "Plintose");
				REQUIRE(votes[0].vote_for == "Plintose");
				REQUIRE(votes[0].cabal == 1);
				REQUIRE(votes[0].time == 997924032);
				REQUIRE(votes[0].host == "pool-63.53.48.74.irvn.grid.net");

				REQUIRE(votes[1].voter == "Saberhagen");
				REQUIRE(votes[1].vote_for == "Plintose");
				REQUIRE(votes[1].time == 997924100);
			}
		}
	}

	GIVEN("a session with no ballots for the cabal")
	{
		FakeDbSession db;	// no rows configured

		VoteRepository repo(db);

		WHEN("FindByCabal is called")
		{
			auto votes = repo.FindByCabal(3);

			THEN("it returns nothing but still binds the requested cabal")
			{
				REQUIRE(votes.empty());
				REQUIRE(db.Binds == std::vector<std::string>{"3"});
			}
		}
	}
}

SCENARIO("finding the ballots a voter has cast", "[vote][repository]")
{
	GIVEN("a session holding the voter's single ballot")
	{
		FakeDbSession db;
		db.Rows.push_back(Row({"Seikilos", "Karsus", "1", "997924032", "some.host"}));

		VoteRepository repo(db);

		WHEN("FindByVoter is called")
		{
			auto votes = repo.FindByVoter("Seikilos");

			THEN("it queries by the bound voter name, not interpolated")
			{
				REQUIRE(db.LastSql ==
					"SELECT voter, vote_for, cabal, `time`, host FROM votes WHERE voter = ?");
				REQUIRE(db.Binds == std::vector<std::string>{"Seikilos"});
			}

			THEN("it maps the ballot, exposing who was voted for")
			{
				REQUIRE(votes.size() == 1);
				REQUIRE(votes.front().vote_for == "Karsus");
			}
		}
	}

	GIVEN("a session where the voter has not voted")
	{
		FakeDbSession db;	// no rows configured

		VoteRepository repo(db);

		WHEN("FindByVoter is called")
		{
			THEN("the empty result signals they may still vote")
			{
				REQUIRE(repo.FindByVoter("Nobody").empty());
			}
		}
	}
}

SCENARIO("casting a ballot", "[vote][repository]")
{
	GIVEN("a fully populated vote and a session that accepts the insert")
	{
		FakeDbSession db;
		db.ExecuteResult = 1;

		Vote vote;
		vote.voter = "Seikilos";
		vote.vote_for = "Karsus";
		vote.cabal = 1;
		vote.time = 997924032;
		vote.host = "nic-28-c151-191.new.rr.com";

		VoteRepository repo(db);

		WHEN("Add is called")
		{
			bool ok = repo.Add(vote);

			THEN("it executes an INSERT binding every column in schema order")
			{
				REQUIRE(ok);
				REQUIRE(db.ExecuteCount == 1);
				REQUIRE(db.LastSql ==
					"INSERT INTO votes (voter, vote_for, cabal, `time`, host) "
					"VALUES (?, ?, ?, ?, ?)");
				REQUIRE(db.Binds == std::vector<std::string>{
					"Seikilos", "Karsus", "1", "997924032", "nic-28-c151-191.new.rr.com"});
			}
		}
	}

	GIVEN("a session whose insert errors")
	{
		FakeDbSession db;
		db.ExecuteResult = -1;

		Vote vote;
		vote.voter = "Seikilos";

		VoteRepository repo(db);

		WHEN("Add is called")
		{
			THEN("it reports failure")
			{
				REQUIRE(repo.Add(vote) == false);
			}
		}
	}

	GIVEN("a SQL-injection payload in the host (the field that used escape_string)")
	{
		FakeDbSession db;
		db.ExecuteResult = 1;

		// A payload that would break out of the VALUES list through string
		// interpolation. Here it stays confined to a single bound value, so
		// escape_string is no longer needed.
		Vote vote;
		vote.voter = "Seikilos";
		vote.vote_for = "Karsus";
		vote.cabal = 1;
		vote.time = 997924032;
		vote.host = "'),('x','y',1,0,'z');--";

		VoteRepository repo(db);

		WHEN("Add is called")
		{
			repo.Add(vote);

			THEN("the SQL stays fixed and the payload never leaves the bound value")
			{
				REQUIRE(db.LastSql ==
					"INSERT INTO votes (voter, vote_for, cabal, `time`, host) "
					"VALUES (?, ?, ?, ?, ?)");
				REQUIRE(std::count(db.LastSql.begin(), db.LastSql.end(), '?') == 5);
				REQUIRE(db.Binds.size() == 5);
				REQUIRE(db.Binds[4] == "'),('x','y',1,0,'z');--");
			}
		}
	}
}
