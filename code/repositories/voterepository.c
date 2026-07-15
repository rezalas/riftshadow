#include "voterepository.h"

/// Maps one result row onto a Vote, in schema column order.
/// @note `time` is a MySQL reserved word, so it is backticked wherever named.
static Vote MapVote(const Row &row)
{
	Vote vote;
	vote.voter = row.Str(0);
	vote.vote_for = row.Str(1);
	vote.cabal = row.I32(2);
	vote.time = row.I64(3);
	vote.host = row.Str(4);
	return vote;
}

/// Finds every ballot cast in a cabal's election.
/// @param cabal: The cabal whose poll to read.
/// @returns A list of the cabal's votes (empty if none, or on error).
std::vector<Vote> VoteRepository::FindByCabal(int cabal)
{
	auto stmt = db.Prepare(
		"SELECT voter, vote_for, cabal, `time`, host FROM votes WHERE cabal = ?");

	stmt->Bind(cabal);

	return stmt->Query<Vote>(MapVote);
}

/// Finds every ballot a given voter has cast.
/// @note The live caller only needs to know whether the voter has voted and for
///   whom; a non-empty result answers both.
/// @param voter: The voter's name.
/// @returns A list of the voter's votes (empty if they have not voted).
std::vector<Vote> VoteRepository::FindByVoter(const std::string &voter)
{
	auto stmt = db.Prepare(
		"SELECT voter, vote_for, cabal, `time`, host FROM votes WHERE voter = ?");

	stmt->Bind(voter);

	return stmt->Query<Vote>(MapVote);
}

/// Creates a ballot in the database.
/// @param vote: The vote to create.
/// @returns true on success; false otherwise.
bool VoteRepository::Add(const Vote &vote)
{
	auto stmt = db.Prepare(
		"INSERT INTO votes (voter, vote_for, cabal, `time`, host) "
		"VALUES (?, ?, ?, ?, ?)");

	stmt->Bind(vote.voter)
		.Bind(vote.vote_for)
		.Bind(vote.cabal)
		.Bind(vote.time)
		.Bind(vote.host);

	return stmt->Execute() >= 0;
}
