#ifndef PKLOGREPOSITORY_H
#define PKLOGREPOSITORY_H

#include <string>
#include <vector>
#include "../stdlibs/dbsession.h"

/// Model used to represent the `pklogs` table (one player-kill audit row).
/// @note Column order in the schema --
///   killer, killercabal, victim, victimcabal, date, room, ctime
/// @note `date` is a MySQL keyword, so it is backticked wherever named (like
///   `votes.time` / `buglist.Date`). No other column needs quoting.
/// @note The `pklogs` table lives in the `rift` database, not `rift_core`.
///   Construct this repository with RS.DbRift, never RS.Db.
struct PkLog
{
	std::string	killer;				// the killer's true name
	int			killercabal = 0;	// the killer's cabal id at the time of the kill
	std::string	victim;				// the victim's true name
	int			victimcabal = 0;	// the victim's cabal id at the time of the kill
	std::string	date;				// the display timestamp string (e.g. "03/24/2004  1:07pm")
	std::string	room;				// the room name + vnum where the kill happened (e.g. "The Chat Room (1200)")
	long long	ctime = 0;			// unix epoch seconds the kill occurred
};

/// Selects which column(s) a PkLog search matches its regex against (the
/// `pktrack` command's wins/losses/all/date/location modes). The set is fixed
/// and never derives from user input, so the mapping to column names is safe.
enum class PkLogField
{
	Killer,			// wins:     killer RLIKE ?
	Victim,			// losses:   victim RLIKE ?
	KillerOrVictim,	// all:      killer RLIKE ? OR victim RLIKE ?
	Date,			// date:     `date` RLIKE ?
	Room			// location: room RLIKE ?
};

/// Repository used to manage the `pklogs` table.
class PkLogRepository
{
public:
	explicit PkLogRepository(IDbSession &db) : db(db) {}

	bool Add(const PkLog &pklog);
	std::vector<PkLog> Search(PkLogField field, const std::string &pattern);
	int RemoveOlderThan(long long cutoff);
	int RemoveByKiller(const std::string &killer);

private:
	IDbSession &	db;
};

#endif /* PKLOGREPOSITORY_H */
