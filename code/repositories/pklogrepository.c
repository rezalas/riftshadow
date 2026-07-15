#include "pklogrepository.h"

/// The explicit column list, in schema order.
/// @note `date` is a MySQL keyword, so it is backticked.
static const std::string kSelect =
	"SELECT killer, killercabal, victim, victimcabal, `date`, room, ctime FROM pklogs";

/// Maps one result row onto a PkLog, in schema column order.
static PkLog MapPkLog(const Row &row)
{
	PkLog pklog;
	pklog.killer = row.Str(0);
	pklog.killercabal = row.I32(1);
	pklog.victim = row.Str(2);
	pklog.victimcabal = row.I32(3);
	pklog.date = row.Str(4);
	pklog.room = row.Str(5);
	pklog.ctime = row.I64(6);
	return pklog;
}

/// Creates a player kill in the database.
/// @param pklog: The kill to record (all seven columns are supplied; the table
///   has no auto-increment key).
/// @returns true on success; false otherwise.
bool PkLogRepository::Add(const PkLog &pklog)
{
	auto stmt = db.Prepare(
		"INSERT INTO pklogs (killer, killercabal, victim, victimcabal, `date`, room, ctime) "
		"VALUES (?, ?, ?, ?, ?, ?, ?)");

	stmt->Bind(pklog.killer)
		.Bind(pklog.killercabal)
		.Bind(pklog.victim)
		.Bind(pklog.victimcabal)
		.Bind(pklog.date)
		.Bind(pklog.room)
		.Bind(pklog.ctime);

	return stmt->Execute() >= 0;
}

/// Searches the kill history by a regex matched against a chosen field (the
/// `pktrack` command). The field selects which column(s) the pattern matches;
/// the column names are fixed by PkLogField, never taken from user input.
/// @param field: Which column(s) to match the regex against.
/// @param pattern: The regular expression (RLIKE) to match.
/// @returns The matching kills (empty if none).
std::vector<PkLog> PkLogRepository::Search(PkLogField field, const std::string &pattern)
{
	std::string where;
	bool matchesTwice = false;

	switch (field)
	{
		case PkLogField::Killer:			where = "killer RLIKE ?"; break;
		case PkLogField::Victim:			where = "victim RLIKE ?"; break;
		case PkLogField::KillerOrVictim:	where = "killer RLIKE ? OR victim RLIKE ?"; matchesTwice = true; break;
		case PkLogField::Date:				where = "`date` RLIKE ?"; break;
		case PkLogField::Room:				where = "room RLIKE ?"; break;
	}

	auto stmt = db.Prepare(kSelect + " WHERE " + where);

	stmt->Bind(pattern);
	if (matchesTwice)
		stmt->Bind(pattern);

	return stmt->Query<PkLog>(MapPkLog);
}

/// Removes every kill older than a cutoff (the periodic prune).
/// @param cutoff: Kills with a ctime strictly less than this are removed.
/// @returns The number of rows removed.
int PkLogRepository::RemoveOlderThan(long long cutoff)
{
	auto stmt = db.Prepare("DELETE FROM pklogs WHERE ctime < ?");

	stmt->Bind(cutoff);

	auto affected = stmt->Execute();
	return affected < 0 ? 0 : (int)affected;
}

/// Removes every kill credited to a killer (part of the player-nuke sweep).
/// @note Mirrors the legacy sweep, which keyed only on `killer` (rows where the
///   nuked character was the victim are left intact).
/// @param killer: The killer's name.
/// @returns The number of rows removed.
int PkLogRepository::RemoveByKiller(const std::string &killer)
{
	auto stmt = db.Prepare("DELETE FROM pklogs WHERE killer = ?");

	stmt->Bind(killer);

	auto affected = stmt->Execute();
	return affected < 0 ? 0 : (int)affected;
}
