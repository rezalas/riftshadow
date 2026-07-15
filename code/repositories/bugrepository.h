#ifndef BUGREPOSITORY_H
#define BUGREPOSITORY_H

#include <string>
#include <vector>
#include "../stdlibs/dbsession.h"

/// Model used to represent the `buglist` table (one reported bug).
/// @note Column order in the schema --
///   ID, AddedBy, Date, Summary, Description, Status
/// @note Unlike most tables, `buglist` names its columns in PascalCase, so the
///   entity fields mirror them verbatim (ID, AddedBy, Date, ...) rather than the
///   usual snake_case. `Date` is a MySQL keyword, so it is backticked wherever named.
/// @note The `buglist` table lives in the `rift` database, not `rift_core`.
///   Construct this repository with RS.DbRift, never RS.Db.
struct Bug
{
	int			ID = 0;			// auto_increment primary key (mediumint unsigned)
	std::string	AddedBy;		// name of the reporter
	std::string	Date;			// free-form date string (log_time()); tinytext, not a datetime
	std::string	Summary;		// one-line summary (text)
	std::string	Description;	// detailed description (text)
	int			Status = 0;		// 0 = unresolved, 1 = resolved (tinyint unsigned)
};

/// Repository used to manage the `buglist` table.
class BugRepository
{
public:
	explicit BugRepository(IDbSession &db) : db(db) {}

	std::vector<Bug> FindById(int id);
	std::vector<Bug> FindUnresolved(int limit);
	std::vector<Bug> FindRecent(int limit);
	bool Add(const Bug &bug);
	int MarkResolved(int id);

private:
	IDbSession &	db;
};

#endif /* BUGREPOSITORY_H */
