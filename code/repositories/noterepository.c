#include "noterepository.h"

/// Maps one result row onto a Note, in schema column order.
static Note MapNote(const Row &row)
{
	Note note;
	note.type = row.I32(0);
	note.sender = row.Str(1);
	note.date = row.Str(2);
	note.to_list = row.Str(3);
	note.subject = row.Str(4);
	note.message = row.Str(5);
	note.timestamp = row.I64(6);
	return note;
}

/// Finds every note of a given category, oldest first.
/// @note The live callers filter each note through hide_note/is_note_to, so this
///   returns the whole spool for the type and lets the caller decide visibility.
/// @param type: The note category (NOTE_NOTE, NOTE_IDEA, ...).
/// @returns The notes of that type ordered by timestamp ascending (empty if none).
std::vector<Note> NoteRepository::FindByType(int type)
{
	auto stmt = db.Prepare(
		"SELECT type, sender, date, to_list, subject, message, timestamp "
		"FROM notes WHERE type = ? ORDER BY timestamp ASC");

	stmt->Bind(type);

	return stmt->Query<Note>(MapNote);
}

/// Creates a new note in the database.
/// @param note: The note to create.
/// @returns true on success; false otherwise.
bool NoteRepository::Add(const Note &note)
{
	auto stmt = db.Prepare(
		"INSERT INTO notes (type, sender, date, to_list, subject, message, timestamp) "
		"VALUES (?, ?, ?, ?, ?, ?, ?)");

	stmt->Bind(note.type)
		.Bind(note.sender)
		.Bind(note.date)
		.Bind(note.to_list)
		.Bind(note.subject)
		.Bind(note.message)
		.Bind(note.timestamp);

	return stmt->Execute() >= 0;
}

/// Removes a single note identified by its post time and sender.
/// @param timestamp: The note's post timestamp.
/// @param sender: The note's sender.
/// @returns The number of rows removed.
int NoteRepository::Remove(long long timestamp, const std::string &sender)
{
	auto stmt = db.Prepare("DELETE FROM notes WHERE timestamp = ? AND sender = ?");

	stmt->Bind(timestamp)
		.Bind(sender);

	auto affected = stmt->Execute();
	return affected < 0 ? 0 : (int)affected;
}

/// Removes every note posted before a cutoff (the periodic board prune).
/// @param cutoff: Notes with a timestamp strictly less than this are removed.
/// @returns The number of rows removed.
int NoteRepository::RemoveOlderThan(long long cutoff)
{
	auto stmt = db.Prepare("DELETE FROM notes WHERE timestamp < ?");

	stmt->Bind(cutoff);

	auto affected = stmt->Execute();
	return affected < 0 ? 0 : (int)affected;
}
