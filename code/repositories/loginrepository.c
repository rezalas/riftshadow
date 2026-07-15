#include "loginrepository.h"

/// The explicit column list, in schema order.
/// @note `time` is a MySQL keyword, so it is backticked.
static const std::string kSelect =
	"SELECT name, site, `time`, ctime, played, obj, lobj, type FROM logins";

/// Maps one result row onto a Login, in schema column order.
static Login MapLogin(const Row &row)
{
	Login login;
	login.name = row.Str(0);
	login.site = row.Str(1);
	login.time = row.Str(2);
	login.ctime = row.I64(3);
	login.played = row.I32(4);
	login.obj = row.I32(5);
	login.lobj = row.I32(6);
	login.type = row.I32(7);
	return login;
}

/// Creates a login event (create/login/logout) in the database.
/// @param login: The event to record (all eight columns are supplied; the table
///   has no auto-increment key).
/// @returns true on success; false otherwise.
bool LoginRepository::Add(const Login &login)
{
	auto stmt = db.Prepare(
		"INSERT INTO logins (name, site, `time`, ctime, played, obj, lobj, type) "
		"VALUES (?, ?, ?, ?, ?, ?, ?, ?)");

	stmt->Bind(login.name)
		.Bind(login.site)
		.Bind(login.time)
		.Bind(login.ctime)
		.Bind(login.played)
		.Bind(login.obj)
		.Bind(login.lobj)
		.Bind(login.type);

	return stmt->Execute() >= 0;
}

/// Finds the distinct character names that ever connected from a matching site.
/// @note Backs the `site` admin view's "players from site" listing; the caller
///   only needs the names, so this returns a name projection, not entities.
/// @param sitePattern: The site regular expression (RLIKE) to match.
/// @returns The distinct matching names (empty if none).
std::vector<std::string> LoginRepository::FindDistinctNamesBySite(const std::string &sitePattern)
{
	auto stmt = db.Prepare("SELECT DISTINCT name FROM logins WHERE site RLIKE ?");

	stmt->Bind(sitePattern);

	return stmt->Query<std::string>([](const Row &row) { return row.Str(0); });
}

/// Searches the login history by a regex matched against name, site, or time,
/// newest first (the `ltrack` command).
/// @param pattern: The regular expression (RLIKE) matched against name/site/time.
/// @param type: Restrict to this event type (0/1/2); pass a negative value for
///   no type filter.
/// @returns The matching login events ordered by ctime descending (empty if none).
std::vector<Login> LoginRepository::Search(const std::string &pattern, int type)
{
	std::string sql = kSelect + " WHERE (name RLIKE ? OR site RLIKE ? OR `time` RLIKE ?)";
	if (type >= 0)
		sql += " AND type = ?";
	sql += " ORDER BY ctime DESC";

	auto stmt = db.Prepare(sql);

	stmt->Bind(pattern)
		.Bind(pattern)
		.Bind(pattern);
	if (type >= 0)
		stmt->Bind(type);

	return stmt->Query<Login>(MapLogin);
}

/// Removes every login event older than a cutoff (the periodic prune).
/// @param cutoff: Events with a ctime strictly less than this are removed.
/// @returns The number of rows removed.
int LoginRepository::RemoveOlderThan(long long cutoff)
{
	auto stmt = db.Prepare("DELETE FROM logins WHERE ctime < ?");

	stmt->Bind(cutoff);

	auto affected = stmt->Execute();
	return affected < 0 ? 0 : (int)affected;
}

/// Removes every login event for a character (part of the player-nuke sweep).
/// @param name: The character's name.
/// @returns The number of rows removed.
int LoginRepository::RemoveByName(const std::string &name)
{
	auto stmt = db.Prepare("DELETE FROM logins WHERE name = ?");

	stmt->Bind(name);

	auto affected = stmt->Execute();
	return affected < 0 ? 0 : (int)affected;
}
