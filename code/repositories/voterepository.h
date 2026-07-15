#ifndef VOTEREPOSITORY_H
#define VOTEREPOSITORY_H

#include <string>
#include <vector>
#include "../stdlibs/dbsession.h"

/// Model used to represent the `votes` table (one cabal-election ballot).
/// @note Column order in the schema --
///   voter, vote_for, cabal, time, host
/// @note The `votes` table lives in the `rift` database, not `rift_core`.
///   Construct this repository with RS.DbRift, never RS.Db.
struct Vote
{
	std::string	voter;			// the voter (players.name)
	std::string	vote_for;		// the candidate (players.name)
	int			cabal = 0;		// the cabal whose poll this ballot belongs to
	long long	time = 0;		// time the vote was cast (unix epoch seconds)
	std::string	host;			// the voter's host
};

/// Repository used to manage the `votes` table.
class VoteRepository
{
public:
	explicit VoteRepository(IDbSession &db) : db(db) {}

	std::vector<Vote> FindByCabal(int cabal);
	std::vector<Vote> FindByVoter(const std::string &voter);
	bool Add(const Vote &vote);

private:
	IDbSession &	db;
};

#endif /* VOTEREPOSITORY_H */
