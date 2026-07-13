#ifndef PLAYERREPOSITORY_H
#define PLAYERREPOSITORY_H

#include <string>
#include <vector>
#include "../stdlibs/dbsession.h"

/// Model used to represent the `players` table (one persisted-stats row per
/// character; the flat pfile holds the rest of the character's state).
/// @note Column order in the schema --
///   name, lastlogin, level, class, race, cabal, sex, hours, align, ethos,
///   logins, noc_logins, c_logins, gold, pks, induct
/// @note One-column exception to the "mirror the schema name verbatim" rule:
///   the `class` column maps to the member `class_` because `class` is a C++
///   keyword and cannot name a data member. It stays bare `class` in the SQL
///   (it is not a MySQL reserved word). Every other member mirrors its column
///   name exactly.
/// @note The `players` table lives in the `rift_core` database, so construct
///   this repository with RS.Db (like bans/theft), never RS.DbRift.
struct Player
{
	std::string	name;			// varchar(20), the character's true name
	int			lastlogin = 0;	// unix epoch of the last logon
	int			level = 0;
	int			class_ = 0;		// schema column `class` (C++ keyword -> class_)
	int			race = 0;
	int			cabal = 0;
	int			sex = 0;
	int			hours = 0;		// hours played
	int			align = 0;
	int			ethos = 0;
	int			logins = 0;		// total logins
	int			noc_logins = 0;	// logins while cabal was not powered
	int			c_logins = 0;	// logins while cabal was powered
	long		gold = 0;		// gold on hand + banked (written as %ld historically)
	float		pks = 0.0f;		// PK kill count
	int			induct = 0;		// tinyint, cabal induction rank
};

/// Repository used to manage the `players` table.
class PlayerRepository
{
public:
	explicit PlayerRepository(IDbSession &db) : db(db) {}

	// Writes
	bool Add(const Player &player);
	bool SaveStats(const Player &player);
	bool RecordLogin(const std::string &name, int lastlogin);
	bool Rename(const std::string &oldName, const std::string &newName);
	bool SetInduct(const std::string &name, int induct);
	int RemoveByName(const std::string &name);

	// Name projections
	std::vector<std::string> FindAllNames();
	std::vector<std::string> FindNamesInactiveSince(long cutoff);
	std::vector<std::string> FindTopByPks(int limit);

	// Scalar stat aggregates
	int CountGoldAboveLevel(int level);
	long long SumGold();

private:
	IDbSession &	db;
};

#endif /* PLAYERREPOSITORY_H */
