#ifndef INDUCTIONREPOSITORY_H
#define INDUCTIONREPOSITORY_H

#include <string>
#include "../stdlibs/dbsession.h"

/// Model used to represent the `inductions` table (a cabal-induction audit record,
/// written only when an immortal inducts another immortal).
/// @note Column order in the schema --
///   ID, ch, victim, cabal, ctime, chsite, victimsite
/// @note The `inductions` table lives in the `rift` database, not `rift_core`.
///   Construct this repository with RS.DbRift, never RS.Db.
/// @note ID is not supplied by inserts (it defaults to 0 in the schema), so it has
///   no entity field -- mirroring the legacy insert column list exactly.
struct Induction
{
	std::string	ch;				// the inductor (players.name)
	std::string	victim;			// the inductee (players.name)
	int			cabal = 0;		// the cabal inducted into (cabal id)
	long long	ctime = 0;		// time of induction (unix epoch seconds)
	std::string	chsite;			// the inductor's host
	std::string	victimsite;		// the inductee's host
};

/// Repository used to manage the `inductions` table.
class InductionRepository
{
public:
	explicit InductionRepository(IDbSession &db) : db(db) {}

	bool Add(const Induction &induction);
	int RemoveByChar(const std::string &name);

private:
	IDbSession &	db;
};

#endif /* INDUCTIONREPOSITORY_H */
