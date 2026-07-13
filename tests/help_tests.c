#include "catch.hpp"
#include <algorithm>
#include "db_mocks.h"
#include "../code/repositories/helprepository.h"

// helpfiles schema column order: id, title, skill, minlevel, helpdata, author.

SCENARIO("finding a helpfile by id", "[help][repository]")
{
	GIVEN("a session holding the requested helpfile")
	{
		FakeDbSession db;
		//                    id     title          skill   minlevel  helpdata        author
		db.Rows.push_back(Row({"42", "TRAIN GAIN", "none", "0",      "Syntax: train", ""}));

		HelpRepository repo(db);

		WHEN("FindById is called")
		{
			auto helps = repo.FindById(42);

			THEN("it selects explicit columns with the id bound, never SELECT *")
			{
				REQUIRE(db.PrepareCount == 1);
				REQUIRE(db.LastSql ==
					"SELECT id, title, skill, minlevel, helpdata, author "
					"FROM helpfiles WHERE id = ?");
				REQUIRE(db.LastSql.find("*") == std::string::npos);
				REQUIRE(db.Binds == std::vector<std::string>{"42"});
			}

			THEN("it maps every column onto the entity in schema order")
			{
				REQUIRE(helps.size() == 1);
				REQUIRE(helps[0].id == 42);
				REQUIRE(helps[0].title == "TRAIN GAIN");
				REQUIRE(helps[0].skill == "none");
				REQUIRE(helps[0].minlevel == 0);
				REQUIRE(helps[0].helpdata == "Syntax: train");
				REQUIRE(helps[0].author == "");
			}
		}
	}

	GIVEN("a session with no matching helpfile")
	{
		FakeDbSession db;	// no rows configured

		HelpRepository repo(db);

		WHEN("FindById is called")
		{
			auto helps = repo.FindById(999);

			THEN("it returns nothing but still binds the requested id")
			{
				REQUIRE(helps.empty());
				REQUIRE(db.Binds == std::vector<std::string>{"999"});
			}
		}
	}
}

SCENARIO("finding helpfiles by title regex", "[help][repository]")
{
	GIVEN("a session that returns matches")
	{
		FakeDbSession db;
		db.Rows.push_back(Row({"1", "SCION", "none", "0", "body", ""}));

		HelpRepository repo(db);

		WHEN("FindByTitle is called")
		{
			auto helps = repo.FindByTitle("scion");

			THEN("it uses RLIKE ordered by id with the pattern bound, not interpolated")
			{
				REQUIRE(db.LastSql ==
					"SELECT id, title, skill, minlevel, helpdata, author "
					"FROM helpfiles WHERE title RLIKE ? ORDER BY id ASC");
				REQUIRE(db.Binds == std::vector<std::string>{"scion"});
				REQUIRE(helps.size() == 1);
			}
		}
	}
}

SCENARIO("finding helpfiles by quoted or exact title", "[help][repository]")
{
	GIVEN("a session used for the help-command disambiguation query")
	{
		FakeDbSession db;
		db.Rows.push_back(Row({"5", "TRAIN", "none", "0", "body", ""}));

		HelpRepository repo(db);

		WHEN("FindByQuotedOrExactTitle is called")
		{
			auto helps = repo.FindByQuotedOrExactTitle("train");

			THEN("it binds the quoted pattern for RLIKE and the bare title for =")
			{
				REQUIRE(db.LastSql ==
					"SELECT id, title, skill, minlevel, helpdata, author "
					"FROM helpfiles WHERE title RLIKE ? OR title = ?");
				REQUIRE(db.Binds == std::vector<std::string>{"'train'", "train"});
				REQUIRE(helps.size() == 1);
			}
		}
	}
}

SCENARIO("listing helpfiles by a searchable field", "[help][repository]")
{
	GIVEN("a session and a whitelisted field")
	{
		FakeDbSession db;
		db.Rows.push_back(Row({"7", "FOO", "none", "0", "body", ""}));

		HelpRepository repo(db);

		WHEN("FindByFieldRegex is called with a valid field (any case)")
		{
			auto helps = repo.FindByFieldRegex("TITLE", "foo");

			THEN("it lowercases the whitelisted column and binds only the value")
			{
				REQUIRE(db.PrepareCount == 1);
				REQUIRE(db.LastSql ==
					"SELECT id, title, skill, minlevel, helpdata, author "
					"FROM helpfiles WHERE title RLIKE ?");
				REQUIRE(db.Binds == std::vector<std::string>{"foo"});
				REQUIRE(helps.size() == 1);
			}
		}
	}

	GIVEN("a session and a field name that is not whitelisted")
	{
		FakeDbSession db;

		HelpRepository repo(db);

		WHEN("FindByFieldRegex is called with an injection attempt in the field")
		{
			auto helps = repo.FindByFieldRegex("id; DROP TABLE helpfiles;--", "x");

			THEN("no statement is prepared and nothing is returned")
			{
				REQUIRE(helps.empty());
				REQUIRE(db.PrepareCount == 0);
			}
		}
	}
}

SCENARIO("listing every helpfile", "[help][repository]")
{
	GIVEN("a session holding several helpfiles")
	{
		FakeDbSession db;
		db.Rows.push_back(Row({"1", "A", "none", "0", "a", ""}));
		db.Rows.push_back(Row({"2", "B", "none", "0", "b", ""}));

		HelpRepository repo(db);

		WHEN("FindAll is called")
		{
			auto helps = repo.FindAll();

			THEN("it selects every helpfile with no filter")
			{
				REQUIRE(db.LastSql ==
					"SELECT id, title, skill, minlevel, helpdata, author FROM helpfiles");
				REQUIRE(db.Binds.empty());
				REQUIRE(helps.size() == 2);
			}
		}
	}
}

SCENARIO("adding a helpfile", "[help][repository]")
{
	GIVEN("a populated helpfile and a session that accepts the insert")
	{
		FakeDbSession db;
		db.ExecuteResult = 1;

		Help help;
		help.title = "NEW HELP";
		help.skill = "none";
		help.minlevel = 5;
		help.helpdata = "Some body text.";

		HelpRepository repo(db);

		WHEN("Add is called")
		{
			bool ok = repo.Add(help);

			THEN("it inserts an explicit column list, binding four columns in order")
			{
				REQUIRE(ok);
				REQUIRE(db.ExecuteCount == 1);
				REQUIRE(db.LastSql ==
					"INSERT INTO helpfiles (title, skill, minlevel, helpdata) "
					"VALUES (?, ?, ?, ?)");
				REQUIRE(db.Binds == std::vector<std::string>{
					"NEW HELP", "none", "5", "Some body text."});
			}
		}
	}

	GIVEN("a session whose insert errors")
	{
		FakeDbSession db;
		db.ExecuteResult = -1;

		Help help;
		help.title = "X";

		HelpRepository repo(db);

		WHEN("Add is called")
		{
			THEN("it reports failure")
			{
				REQUIRE(repo.Add(help) == false);
			}
		}
	}

	GIVEN("a SQL-injection payload in the body (the field that used escape_string)")
	{
		FakeDbSession db;
		db.ExecuteResult = 1;

		Help help;
		help.title = "T";
		help.skill = "none";
		help.minlevel = 0;
		help.helpdata = "\"),(0,'x','y',1,'z');--";

		HelpRepository repo(db);

		WHEN("Add is called")
		{
			repo.Add(help);

			THEN("the SQL stays fixed and the payload never leaves the bound value")
			{
				REQUIRE(db.LastSql ==
					"INSERT INTO helpfiles (title, skill, minlevel, helpdata) "
					"VALUES (?, ?, ?, ?)");
				REQUIRE(std::count(db.LastSql.begin(), db.LastSql.end(), '?') == 4);
				REQUIRE(db.Binds.size() == 4);
				REQUIRE(db.Binds[3] == "\"),(0,'x','y',1,'z');--");
			}
		}
	}
}

SCENARIO("updating a helpfile body", "[help][repository]")
{
	GIVEN("a session that reports one row updated")
	{
		FakeDbSession db;
		db.ExecuteResult = 1;

		HelpRepository repo(db);

		WHEN("Update is called")
		{
			int updated = repo.Update(42, "new body");

			THEN("it updates helpdata by the bound id, not interpolated")
			{
				REQUIRE(updated == 1);
				REQUIRE(db.ExecuteCount == 1);
				REQUIRE(db.LastSql == "UPDATE helpfiles SET helpdata = ? WHERE id = ?");
				REQUIRE(db.Binds == std::vector<std::string>{"new body", "42"});
			}
		}
	}
}

SCENARIO("removing a helpfile", "[help][repository]")
{
	GIVEN("a session that reports one row removed")
	{
		FakeDbSession db;
		db.ExecuteResult = 1;

		HelpRepository repo(db);

		WHEN("Remove is called")
		{
			int removed = repo.Remove(42);

			THEN("it deletes by the bound id")
			{
				REQUIRE(removed == 1);
				REQUIRE(db.LastSql == "DELETE FROM helpfiles WHERE id = ?");
				REQUIRE(db.Binds == std::vector<std::string>{"42"});
			}
		}
	}

	GIVEN("a session whose delete errors")
	{
		FakeDbSession db;
		db.ExecuteResult = -1;

		HelpRepository repo(db);

		WHEN("Remove is called")
		{
			THEN("it reports zero rows removed")
			{
				REQUIRE(repo.Remove(1) == 0);
			}
		}
	}
}
