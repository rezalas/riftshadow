#ifndef SITETRACKERREPOSITORY_H
#define SITETRACKERREPOSITORY_H

#include <string>
#include <vector>
#include "../stdlibs/dbsession.h"

/// Model used to represent the `sitetracker` table (one tracked site/IP, with the
/// running count of players denied access from it; drives the `sitetrack` command).
/// @note Column order in the schema --
///   site_id, site_name, denials
/// @note `site_id` is an auto_increment PRIMARY KEY, so Add supplies only the
///   other two columns and lets the server assign it.
/// @note No column needs backticking (unlike `sitecomments`, whose `comment`
///   column is a keyword).
/// @note The `sitetracker` table lives in the `rift` database, not `rift_core`.
///   Construct this repository with RS.DbRift, never RS.Db.
struct SiteTracker
{
	int			site_id = 0;		// auto_increment primary key
	std::string	site_name;			// the tracked site IP/domain (tinytext)
	int			denials = 0;		// running count of players denied from this site
};

/// Repository used to manage the `sitetracker` table.
class SiteTrackerRepository
{
public:
	explicit SiteTrackerRepository(IDbSession &db) : db(db) {}

	std::vector<SiteTracker> FindAll();
	std::vector<SiteTracker> FindById(int siteId);
	bool Add(const SiteTracker &site);
	int IncrementDenialsByHost(const std::string &host);
	int Remove(int siteId);

private:
	IDbSession &	db;
};

#endif /* SITETRACKERREPOSITORY_H */
