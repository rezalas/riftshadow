#include "catch.hpp"
#include <algorithm>
#include "db_mocks.h"
#include "../code/repositories/noterepository.h"

// NOTE_NOTE == 0, matching code/note.h.

SCENARIO("finding the notes of a given type", "[note][repository]")
{
	GIVEN("a session holding two notes of the requested type")
	{
		FakeDbSession db;
		//                    type sender      date              to_list     subject      message      timestamp
		db.Rows.push_back(Row({"0", "Plintose", "Mon Jan  1 ...", "all",      "Welcome",   "hello all", "997924032"}));
		db.Rows.push_back(Row({"0", "Karsus",   "Tue Jan  2 ...", "immortal", "Meeting",   "body text", "997924100"}));

		NoteRepository repo(db);

		WHEN("FindByType is called")
		{
			auto notes = repo.FindByType(0);

			THEN("it selects explicit columns ordered by timestamp with the type bound")
			{
				REQUIRE(db.PrepareCount == 1);
				REQUIRE(db.LastSql ==
					"SELECT type, sender, date, to_list, subject, message, timestamp "
					"FROM notes WHERE type = ? ORDER BY timestamp ASC");
				REQUIRE(db.LastSql.find("*") == std::string::npos);
				REQUIRE(db.Binds == std::vector<std::string>{"0"});
			}

			THEN("it maps every column onto the entity in schema order")
			{
				REQUIRE(notes.size() == 2);

				REQUIRE(notes[0].type == 0);
				REQUIRE(notes[0].sender == "Plintose");
				REQUIRE(notes[0].date == "Mon Jan  1 ...");
				REQUIRE(notes[0].to_list == "all");
				REQUIRE(notes[0].subject == "Welcome");
				REQUIRE(notes[0].message == "hello all");
				REQUIRE(notes[0].timestamp == 997924032);

				REQUIRE(notes[1].sender == "Karsus");
				REQUIRE(notes[1].to_list == "immortal");
				REQUIRE(notes[1].message == "body text");
				REQUIRE(notes[1].timestamp == 997924100);
			}
		}
	}

	GIVEN("a session with no notes of the type")
	{
		FakeDbSession db;	// no rows configured

		NoteRepository repo(db);

		WHEN("FindByType is called")
		{
			auto notes = repo.FindByType(3);

			THEN("it returns nothing but still binds the requested type")
			{
				REQUIRE(notes.empty());
				REQUIRE(db.Binds == std::vector<std::string>{"3"});
			}
		}
	}
}

SCENARIO("posting a note", "[note][repository]")
{
	GIVEN("a fully populated note and a session that accepts the insert")
	{
		FakeDbSession db;
		db.ExecuteResult = 1;

		Note note;
		note.type = 0;
		note.sender = "Plintose";
		note.date = "Mon Jan  1 00:00:00 2001";
		note.to_list = "all";
		note.subject = "Welcome";
		note.message = "Greetings, everyone.";
		note.timestamp = 997924032;

		NoteRepository repo(db);

		WHEN("Add is called")
		{
			bool ok = repo.Add(note);

			THEN("it executes an INSERT binding every column in schema order")
			{
				REQUIRE(ok);
				REQUIRE(db.ExecuteCount == 1);
				REQUIRE(db.LastSql ==
					"INSERT INTO notes (type, sender, date, to_list, subject, message, timestamp) "
					"VALUES (?, ?, ?, ?, ?, ?, ?)");
				REQUIRE(db.Binds == std::vector<std::string>{
					"0", "Plintose", "Mon Jan  1 00:00:00 2001", "all", "Welcome",
					"Greetings, everyone.", "997924032"});
			}
		}
	}

	GIVEN("a session whose insert errors")
	{
		FakeDbSession db;
		db.ExecuteResult = -1;

		Note note;
		note.sender = "Plintose";

		NoteRepository repo(db);

		WHEN("Add is called")
		{
			THEN("it reports failure")
			{
				REQUIRE(repo.Add(note) == false);
			}
		}
	}

	GIVEN("a SQL-injection payload in the message (the field that used escape_string)")
	{
		FakeDbSession db;
		db.ExecuteResult = 1;

		// A payload that would break out of the VALUES list through string
		// interpolation. Here it stays confined to a single bound value, so
		// escape_string is no longer needed.
		Note note;
		note.type = 0;
		note.sender = "Plintose";
		note.date = "d";
		note.to_list = "all";
		note.subject = "s";
		note.message = "\"),(0,'x','y','z','w','p',1);--";
		note.timestamp = 997924032;

		NoteRepository repo(db);

		WHEN("Add is called")
		{
			repo.Add(note);

			THEN("the SQL stays fixed and the payload never leaves the bound value")
			{
				REQUIRE(db.LastSql ==
					"INSERT INTO notes (type, sender, date, to_list, subject, message, timestamp) "
					"VALUES (?, ?, ?, ?, ?, ?, ?)");
				REQUIRE(std::count(db.LastSql.begin(), db.LastSql.end(), '?') == 7);
				REQUIRE(db.Binds.size() == 7);
				REQUIRE(db.Binds[5] == "\"),(0,'x','y','z','w','p',1);--");
			}
		}
	}
}

SCENARIO("removing a single note", "[note][repository]")
{
	GIVEN("a session that reports one row removed")
	{
		FakeDbSession db;
		db.ExecuteResult = 1;

		NoteRepository repo(db);

		WHEN("Remove is called with a timestamp and sender")
		{
			int removed = repo.Remove(997924032, "Plintose");

			THEN("it deletes by the bound timestamp and sender, not interpolated")
			{
				REQUIRE(removed == 1);
				REQUIRE(db.ExecuteCount == 1);
				REQUIRE(db.LastSql ==
					"DELETE FROM notes WHERE timestamp = ? AND sender = ?");
				REQUIRE(db.Binds == std::vector<std::string>{"997924032", "Plintose"});
			}
		}
	}

	GIVEN("a session whose delete errors")
	{
		FakeDbSession db;
		db.ExecuteResult = -1;

		NoteRepository repo(db);

		WHEN("Remove is called")
		{
			THEN("it reports zero rows removed")
			{
				REQUIRE(repo.Remove(1, "Nobody") == 0);
			}
		}
	}
}

SCENARIO("pruning old notes", "[note][repository]")
{
	GIVEN("a session that reports several rows removed")
	{
		FakeDbSession db;
		db.ExecuteResult = 5;

		NoteRepository repo(db);

		WHEN("RemoveOlderThan is called with a cutoff")
		{
			int removed = repo.RemoveOlderThan(1000000000);

			THEN("it deletes everything before the bound cutoff")
			{
				REQUIRE(removed == 5);
				REQUIRE(db.LastSql == "DELETE FROM notes WHERE timestamp < ?");
				REQUIRE(db.Binds == std::vector<std::string>{"1000000000"});
			}
		}
	}
}
