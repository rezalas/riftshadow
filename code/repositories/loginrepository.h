#ifndef LOGINREPOSITORY_H
#define LOGINREPOSITORY_H

#include <string>
#include <vector>
#include "../stdlibs/dbsession.h"

/// Model used to represent the `logins` table (one login/logout/create audit row).
/// @note Column order in the schema --
///   name, site, time, ctime, played, obj, lobj, type
/// @note `time` is a MySQL keyword, so it is backticked wherever named (like
///   `votes.time` / `offerings.time`). No other column needs quoting.
/// @note The `logins` table lives in the `rift` database, not `rift_core`.
///   Construct this repository with RS.DbRift, never RS.Db.
struct Login
{
	std::string	name;			// the character's true name
	std::string	site;			// the connecting host/site
	std::string	time;			// the display timestamp string (e.g. "06/25/2004  5:28pm")
	long long	ctime = 0;		// unix epoch seconds the event occurred
	int			played = 0;		// minutes played this session (logout only); -1 otherwise
	int			obj = 0;		// count of carried objects; -1 for a create event
	int			lobj = 0;		// count of carried objects incl. contained; -1 for a create event
	int			type = 0;		// 0 = create, 1 = login, 2 = logout
};

/// Repository used to manage the `logins` table.
class LoginRepository
{
public:
	explicit LoginRepository(IDbSession &db) : db(db) {}

	bool Add(const Login &login);
	std::vector<std::string> FindDistinctNamesBySite(const std::string &sitePattern);
	std::vector<Login> Search(const std::string &pattern, int type);
	int RemoveOlderThan(long long cutoff);
	int RemoveByName(const std::string &name);

private:
	IDbSession &	db;
};

#endif /* LOGINREPOSITORY_H */
