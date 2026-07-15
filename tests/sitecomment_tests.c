#include "catch.hpp"
#include <algorithm>
#include "db_mocks.h"
#include "../code/repositories/sitecommentrepository.h"

// sitecomments schema column order: site_id, comment_id, comment_name, comment_date, comment.
// `comment` is a MySQL keyword, so it is backticked in every query that names it.
// comment_id is an auto_increment PRIMARY KEY, so Add supplies only the other four columns.

static const std::string kSelectCols =
	"SELECT site_id, comment_id, comment_name, comment_date, `comment` FROM sitecomments";

SCENARIO("counting the comments on a site", "[sitecomment][repository]")
{
	GIVEN("a session that reports a count")
	{
		FakeDbSession db;
		db.Rows.push_back(Row({"3"}));

		SiteCommentRepository repo(db);

		WHEN("CountBySite is called")
		{
			int count = repo.CountBySite(7);

			THEN("it counts by the bound site id, never SELECT *")
			{
				REQUIRE(count == 3);
				REQUIRE(db.LastSql == "SELECT COUNT(*) FROM sitecomments WHERE site_id = ?");
				REQUIRE(db.LastSql.find("*") != std::string::npos); // COUNT(*), no column star otherwise
				REQUIRE(db.Binds == std::vector<std::string>{"7"});
			}
		}
	}

	GIVEN("a session holding no comments")
	{
		FakeDbSession db; // no rows

		SiteCommentRepository repo(db);

		WHEN("CountBySite is called")
		{
			THEN("it reports zero")
			{
				REQUIRE(repo.CountBySite(7) == 0);
			}
		}
	}
}

SCENARIO("listing the comments on a site", "[sitecomment][repository]")
{
	GIVEN("a session holding matching comments")
	{
		FakeDbSession db;
		//                    site_id comment_id name    date               comment
		db.Rows.push_back(Row({"2", "1", "Dev", "Tue Nov 14,  5:27pm", "Aol is a bunch of losers."}));

		SiteCommentRepository repo(db);

		WHEN("FindBySite is called")
		{
			auto results = repo.FindBySite(2);

			THEN("it selects an explicit column list by the bound site id, never SELECT *")
			{
				REQUIRE(db.LastSql == kSelectCols + " WHERE site_id = ?");
				REQUIRE(db.LastSql.find("*") == std::string::npos);
				REQUIRE(db.Binds == std::vector<std::string>{"2"});
			}

			THEN("it maps every column onto the entity in schema order")
			{
				REQUIRE(results.size() == 1);
				REQUIRE(results[0].site_id == 2);
				REQUIRE(results[0].comment_id == 1);
				REQUIRE(results[0].comment_name == "Dev");
				REQUIRE(results[0].comment_date == "Tue Nov 14,  5:27pm");
				REQUIRE(results[0].comment == "Aol is a bunch of losers.");
			}
		}
	}
}

SCENARIO("adding a comment to a site", "[sitecomment][repository]")
{
	GIVEN("a populated comment and a session that accepts the insert")
	{
		FakeDbSession db;
		db.ExecuteResult = 1;

		SiteComment comment;
		comment.site_id = 3;
		comment.comment_name = "Dev";
		comment.comment_date = "Tue Nov 14,  5:53pm";
		comment.comment = "I really, really hate Bell Atlantic.";

		SiteCommentRepository repo(db);

		WHEN("Add is called")
		{
			bool ok = repo.Add(comment);

			THEN("it inserts an explicit column list, letting the server assign comment_id")
			{
				REQUIRE(ok);
				REQUIRE(db.ExecuteCount == 1);
				REQUIRE(db.LastSql ==
					"INSERT INTO sitecomments (site_id, comment_name, comment_date, `comment`) "
					"VALUES (?, ?, ?, ?)");
				REQUIRE(db.Binds == std::vector<std::string>{
					"3", "Dev", "Tue Nov 14,  5:53pm", "I really, really hate Bell Atlantic."});
			}
		}
	}

	GIVEN("a session whose insert errors")
	{
		FakeDbSession db;
		db.ExecuteResult = -1;

		SiteComment comment;
		comment.site_id = 1;

		SiteCommentRepository repo(db);

		WHEN("Add is called")
		{
			THEN("it reports failure")
			{
				REQUIRE(repo.Add(comment) == false);
			}
		}
	}

	GIVEN("a SQL-injection payload in the comment body (which used escape_string)")
	{
		FakeDbSession db;
		db.ExecuteResult = 1;

		SiteComment comment;
		comment.site_id = 1;
		comment.comment_name = "Dev";
		comment.comment_date = "now";
		comment.comment = "\"); DROP TABLE sitecomments;--";

		SiteCommentRepository repo(db);

		WHEN("Add is called")
		{
			repo.Add(comment);

			THEN("the SQL stays fixed and the payload never leaves the bound value")
			{
				REQUIRE(std::count(db.LastSql.begin(), db.LastSql.end(), '?') == 4);
				REQUIRE(db.Binds.size() == 4);
				REQUIRE(db.Binds[3] == "\"); DROP TABLE sitecomments;--");
			}
		}
	}
}

SCENARIO("removing a comment", "[sitecomment][repository]")
{
	GIVEN("a session that reports a row removed")
	{
		FakeDbSession db;
		db.ExecuteResult = 1;

		SiteCommentRepository repo(db);

		WHEN("Remove is called")
		{
			int removed = repo.Remove(42);

			THEN("it deletes by the bound comment_id primary key")
			{
				REQUIRE(removed == 1);
				REQUIRE(db.LastSql == "DELETE FROM sitecomments WHERE comment_id = ?");
				REQUIRE(db.Binds == std::vector<std::string>{"42"});
			}
		}
	}

	GIVEN("a session whose delete errors")
	{
		FakeDbSession db;
		db.ExecuteResult = -1;

		SiteCommentRepository repo(db);

		WHEN("Remove is called")
		{
			THEN("it reports zero rows removed")
			{
				REQUIRE(repo.Remove(42) == 0);
			}
		}
	}
}
