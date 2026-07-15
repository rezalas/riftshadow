#ifndef HELPREPOSITORY_H
#define HELPREPOSITORY_H

#include <string>
#include <vector>
#include "../stdlibs/dbsession.h"

/// Model used to represent the `helpfiles` table (one database-backed helpfile).
/// @note Column order in the schema --
///   id, title, skill, minlevel, helpdata, author
/// @note `author` exists in the schema but is unused by the game code (NULL/'' in
///   the live data); it is mapped for a faithful 1:1 mirror of the table.
/// @note The `helpfiles` table lives in the `rift` database, not `rift_core`.
///   Construct this repository with RS.DbRift, never RS.Db.
struct Help
{
	int			id = 0;			// auto_increment primary key
	std::string	title;			// title / keywords the help search matches on
	std::string	skill;			// required skill name ("none", "olc", "heroimm", ...)
	int			minlevel = 0;	// minimum level to view (tinyint unsigned)
	std::string	helpdata;		// the helpfile body text
	std::string	author;			// author (unused by game code; NULL/'' in data)
};

/// Repository used to manage the `helpfiles` table.
class HelpRepository
{
public:
	explicit HelpRepository(IDbSession &db) : db(db) {}

	std::vector<Help> FindById(int id);
	std::vector<Help> FindByTitle(const std::string &pattern);
	std::vector<Help> FindByQuotedOrExactTitle(const std::string &title);
	std::vector<Help> FindByFieldRegex(const std::string &field, const std::string &value);
	std::vector<Help> FindAll();
	bool Add(const Help &help);
	int Update(int id, const std::string &helpdata);
	int Remove(int id);

private:
	IDbSession &	db;
};

#endif /* HELPREPOSITORY_H */
