#ifndef NOTEREPOSITORY_H
#define NOTEREPOSITORY_H

#include <string>
#include <vector>
#include "../stdlibs/dbsession.h"

/// Model used to represent the `notes` table (one board post: note, idea,
/// penalty, news, or change entry).
/// @note Column order in the schema --
///   type, sender, date, to_list, subject, message, timestamp
/// @note The entity field for the body is `message`, matching the schema column
///   verbatim (the legacy NOTE_DATA struct calls the same value `text`).
/// @note The `notes` table lives in the `rift` database, not `rift_core`.
///   Construct this repository with RS.DbRift, never RS.Db.
struct Note
{
	int			type = 0;		// note category (NOTE_NOTE, NOTE_IDEA, ...)
	std::string	sender;			// the poster's name
	std::string	date;			// human-readable post date (ctime string)
	std::string	to_list;		// recipient list (names, "all", "immortal", ...)
	std::string	subject;		// the subject line
	std::string	message;		// the note body
	long long	timestamp = 0;	// time the note was posted (unix epoch seconds)
};

/// Repository used to manage the `notes` table.
class NoteRepository
{
public:
	explicit NoteRepository(IDbSession &db) : db(db) {}

	std::vector<Note> FindByType(int type);
	bool Add(const Note &note);
	int Remove(long long timestamp, const std::string &sender);
	int RemoveOlderThan(long long cutoff);

private:
	IDbSession &	db;
};

#endif /* NOTEREPOSITORY_H */
