#ifndef THEFTREPOSITORY_H
#define THEFTREPOSITORY_H

#include <string>
#include "../stdlibs/dbsession.h"

/// Model used to represent the `theft` table.
/// @note Column order in the schema --
///   ID, ch, victim, obj, success
/// @note ID is an auto_increment primary key filled in by the database, so it
///   has no entity field; inserts never supply it.
struct Theft
{
	std::string	ch;				// the thief (players.name)
	std::string	victim;			// the victim (players.name)
	std::string	obj;			// stolen item's short desc, the gold amount, or "none"
	int success = 0;			// 1 == the theft succeeded, 0 == the thief was caught
};

/// Repository used to manage the `theft` table.
class TheftRepository
{
public:
	explicit TheftRepository(IDbSession &db) : db(db) {}

	bool Add(const Theft &theft);
	int RemoveByChar(const std::string &name);

private:
	IDbSession &	db;
};

#endif /* THEFTREPOSITORY_H */
