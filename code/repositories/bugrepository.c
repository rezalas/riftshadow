#include "bugrepository.h"

/// The explicit column list, in schema order.
/// @note `Date` is a MySQL keyword, so it is backticked.
static const std::string kSelect =
	"SELECT ID, AddedBy, `Date`, Summary, Description, Status FROM buglist";

/// Maps one result row onto a Bug, in schema column order.
static Bug MapBug(const Row &row)
{
	Bug bug;
	bug.ID = row.I32(0);
	bug.AddedBy = row.Str(1);
	bug.Date = row.Str(2);
	bug.Summary = row.Str(3);
	bug.Description = row.Str(4);
	bug.Status = row.I32(5);
	return bug;
}

/// Finds a bug by its primary key.
/// @param id: The bug id.
/// @returns The matching bug (0 or 1 row; empty on error or if none).
std::vector<Bug> BugRepository::FindById(int id)
{
	auto stmt = db.Prepare(kSelect + " WHERE ID = ?");

	stmt->Bind(id);

	return stmt->Query<Bug>(MapBug);
}

/// Finds the most recent unresolved bugs, newest first.
/// @param limit: The maximum number of bugs to return.
/// @returns The unresolved bugs ordered by id descending (empty if none).
std::vector<Bug> BugRepository::FindUnresolved(int limit)
{
	auto stmt = db.Prepare(kSelect + " WHERE Status = 0 ORDER BY ID DESC LIMIT ?");

	stmt->Bind(limit);

	return stmt->Query<Bug>(MapBug);
}

/// Finds the most recent bugs regardless of status, newest first.
/// @param limit: The maximum number of bugs to return.
/// @returns The bugs ordered by id descending (empty if none).
std::vector<Bug> BugRepository::FindRecent(int limit)
{
	auto stmt = db.Prepare(kSelect + " ORDER BY ID DESC LIMIT ?");

	stmt->Bind(limit);

	return stmt->Query<Bug>(MapBug);
}

/// Creates a new bug report (Status defaults to 0/unresolved via the entity).
/// @param bug: The bug to create (AddedBy, Date, Summary, Description, Status used).
/// @returns true on success; false otherwise.
bool BugRepository::Add(const Bug &bug)
{
	auto stmt = db.Prepare(
		"INSERT INTO buglist (AddedBy, `Date`, Summary, Description, Status) "
		"VALUES (?, ?, ?, ?, ?)");

	stmt->Bind(bug.AddedBy)
		.Bind(bug.Date)
		.Bind(bug.Summary)
		.Bind(bug.Description)
		.Bind(bug.Status);

	return stmt->Execute() >= 0;
}

/// Marks a bug as resolved (Status = 1).
/// @param id: The id of the bug to resolve.
/// @returns The number of rows updated.
int BugRepository::MarkResolved(int id)
{
	auto stmt = db.Prepare("UPDATE buglist SET Status = 1 WHERE ID = ?");

	stmt->Bind(id);

	auto affected = stmt->Execute();
	return affected < 0 ? 0 : (int)affected;
}
