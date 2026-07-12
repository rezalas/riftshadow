#include "theftrepository.h"

/// Records a theft attempt in the database.
/// @note The obj field carries whatever was taken: an item's short description,
///   the gold amount as text, or "none" for a caught attempt.
/// @param theft: The theft attempt to record.
/// @returns true on success; false otherwise.
bool TheftRepository::Add(const Theft &theft)
{
	auto stmt = db.Prepare(
		"INSERT INTO theft (ch, victim, obj, success) VALUES (?, ?, ?, ?)");

	stmt->Bind(theft.ch)
		.Bind(theft.victim)
		.Bind(theft.obj)
		.Bind(theft.success);

	return stmt->Execute() >= 0;
}

/// Removes every theft record attributed to a character.
/// @param name: The name of the character to remove.
/// @returns The number of rows removed.
int TheftRepository::RemoveByChar(const std::string &name)
{
	auto stmt = db.Prepare("DELETE FROM theft WHERE ch = ?");

	stmt->Bind(name);

	auto affected = stmt->Execute();
	return affected < 0 ? 0 : (int)affected;
}
