#include "inductionrepository.h"

/// Records a cabal induction in the database.
/// @note Only written when an immortal inducts another immortal (the legacy call
///   sites guard on is_immortal for both parties).
/// @param induction: The induction to record.
/// @returns true on success; false otherwise.
bool InductionRepository::Add(const Induction &induction)
{
	auto stmt = db.Prepare(
		"INSERT INTO inductions (ch, victim, cabal, ctime, chsite, victimsite) "
		"VALUES (?, ?, ?, ?, ?, ?)");

	stmt->Bind(induction.ch)
		.Bind(induction.victim)
		.Bind(induction.cabal)
		.Bind(induction.ctime)
		.Bind(induction.chsite)
		.Bind(induction.victimsite);

	return stmt->Execute() >= 0;
}

/// Removes every induction record attributed to a character.
/// @param name: The name of the character to remove.
/// @returns The number of rows removed.
int InductionRepository::RemoveByChar(const std::string &name)
{
	auto stmt = db.Prepare("DELETE FROM inductions WHERE ch = ?");

	stmt->Bind(name);

	auto affected = stmt->Execute();
	return affected < 0 ? 0 : (int)affected;
}
