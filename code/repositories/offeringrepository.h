#ifndef OFFERINGREPOSITORY_H
#define OFFERINGREPOSITORY_H

#include <string>
#include <vector>
#include "../stdlibs/dbsession.h"

/// Model used to represent the `offerings` table (one item offered by a player
/// at a deity's altar).
/// @note Column order in the schema --
///   deity, offeringvnum, offeringshort, player, status, time, id
/// @note `time` is a MySQL keyword, so it is backticked wherever named (like
///   `votes.time`). No other column needs quoting.
/// @note The `offerings` table lives in the `rift` database, not `rift_core`.
///   Construct this repository with RS.DbRift, never RS.Db.
struct Offering
{
	std::string	deity;			// recipient deity (the altar room's owner)
	int			offeringvnum = 0;	// vnum of the offered object
	std::string	offeringshort;	// short description of the offered object
	std::string	player;			// name of the offering player
	int			status = 0;		// 0 = new/unviewed, 1 = rejected, 2 = accepted
	long long	time = 0;		// unix epoch seconds the offering was made
	int			id = 0;			// auto_increment primary key
};

/// Repository used to manage the `offerings` table.
class OfferingRepository
{
public:
	explicit OfferingRepository(IDbSession &db) : db(db) {}

	std::vector<Offering> FindByDeity(const std::string &deity);
	std::vector<Offering> FindUnviewedByDeity(const std::string &deity);
	std::vector<Offering> FindLatestByDeityAndPlayer(const std::string &deity, const std::string &player);
	bool Add(const Offering &offering);
	int SetStatus(int id, int status);
	int RemoveOlderThan(long long cutoff);
	int RemoveByPlayer(const std::string &name);

private:
	IDbSession &	db;
};

#endif /* OFFERINGREPOSITORY_H */
