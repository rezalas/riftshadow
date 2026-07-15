#ifndef BANREPOSITORY_H
#define BANREPOSITORY_H

#include <string>
#include <vector>
#include "../stdlibs/dbsession.h"

/// Model used to represent the `bans` table. 
/// @note Column order in the schema -- 
///   site, by, reason, date, duration, ban_type, host_type
struct Ban
{
	std::string	site;
	std::string	by;
	std::string	reason;
	std::string	date;			// datetime; formatted MM/DD/YYYY when listed
	long long	duration = 0;	// -1 == permanent
	int			ban_type = 0;	// NBAN_ALL / NBAN_NEWBIE
	int			host_type = 0;	// NBAN_IP / NBAN_HOST
};

/// Repository used to manage the `bans` table. 
class BanRepository
{
public:
	explicit BanRepository(IDbSession &db) : db(db) {}

	std::vector<Ban> FindByType(int banType, int hostType);
	std::vector<Ban> FindAllOrderedByDuration();
	bool Add(const Ban &ban);
	int RemoveBySiteContaining(const std::string &fragment);
private:
	IDbSession &	db;
};

#endif /* BANREPOSITORY_H */
