#include <algorithm>
#include <cctype>
#include "helprepository.h"

/// The explicit column list, in schema order.
static const std::string kSelect =
	"SELECT id, title, skill, minlevel, helpdata, author FROM helpfiles";

/// Maps one result row onto a Help, in schema column order.
static Help MapHelp(const Row &row)
{
	Help help;
	help.id = row.I32(0);
	help.title = row.Str(1);
	help.skill = row.Str(2);
	help.minlevel = row.I32(3);
	help.helpdata = row.Str(4);
	help.author = row.Str(5);
	return help;
}

/// Finds a helpfile by its primary key.
/// @param id: The helpfile id.
/// @returns The matching helpfile (0 or 1 row; empty on error or if none).
std::vector<Help> HelpRepository::FindById(int id)
{
	auto stmt = db.Prepare(kSelect + " WHERE id = ?");

	stmt->Bind(id);

	return stmt->Query<Help>(MapHelp);
}

/// Finds every helpfile whose title matches a regex, lowest id first.
/// @param pattern: The title regular expression to match.
/// @returns The matching helpfiles ordered by id ascending (empty if none).
std::vector<Help> HelpRepository::FindByTitle(const std::string &pattern)
{
	auto stmt = db.Prepare(kSelect + " WHERE title RLIKE ? ORDER BY id ASC");

	stmt->Bind(pattern);

	return stmt->Query<Help>(MapHelp);
}

/// Finds helpfiles matching a quoted-title regex or an exact title. 
/// @param title: The search text (bound both quoted for regex and bare for `=`).
/// @returns The matching helpfiles (empty if none).
std::vector<Help> HelpRepository::FindByQuotedOrExactTitle(const std::string &title)
{
	auto stmt = db.Prepare(kSelect + " WHERE title RLIKE ? OR title = ?");

	stmt->Bind("'" + title + "'")
		.Bind(title);

	return stmt->Query<Help>(MapHelp);
}

/// Finds every helpfile whose given column matches a regex (the `listhelp
/// <field> <value>` search).
/// @param field: The column to search (id, title, skill, minlevel, helpdata).
/// @param value: The regular expression to match against that column.
/// @returns The matching helpfiles (empty if none, or if the field is unknown).
std::vector<Help> HelpRepository::FindByFieldRegex(const std::string &field, const std::string &value)
{
	// A column name cannot be a bound parameter, so `field` is whitelisted to
	// a fixed set of column identifiers (case-insensitive). Anything outside the
	// set yields no query and an empty result, so user input can never inject SQL
	// through the field name. The `value` regex is always a bound parameter.
	std::string column = field;
	std::transform(column.begin(), column.end(), column.begin(),
		[](unsigned char c) { return std::tolower(c); });

	if (column != "id" && column != "title" && column != "skill"
		&& column != "minlevel" && column != "helpdata")
		return {};

	auto stmt = db.Prepare(kSelect + " WHERE " + column + " RLIKE ?");

	stmt->Bind(value);

	return stmt->Query<Help>(MapHelp);
}

/// Finds every helpfile.
/// @returns Every helpfile in the table (empty on error).
std::vector<Help> HelpRepository::FindAll()
{
	auto stmt = db.Prepare(kSelect);

	return stmt->Query<Help>(MapHelp);
}

/// Creates a new helpfile.
/// @param help: The helpfile to create (title, skill, minlevel, helpdata used).
/// @returns true on success; false otherwise.
bool HelpRepository::Add(const Help &help)
{
	// TODO: Add support for author, if we ever want to use it. The code doesn't at the moment.
	auto stmt = db.Prepare(
		"INSERT INTO helpfiles (title, skill, minlevel, helpdata) "
		"VALUES (?, ?, ?, ?)");

	stmt->Bind(help.title)
		.Bind(help.skill)
		.Bind(help.minlevel)
		.Bind(help.helpdata);

	return stmt->Execute() >= 0;
}

/// Updates a helpfile's body text.
/// @param id: The id of the helpfile to update.
/// @param helpdata: The new body text.
/// @returns The number of rows updated.
int HelpRepository::Update(int id, const std::string &helpdata)
{
	auto stmt = db.Prepare("UPDATE helpfiles SET helpdata = ? WHERE id = ?");

	stmt->Bind(helpdata)
		.Bind(id);

	auto affected = stmt->Execute();
	return affected < 0 ? 0 : (int)affected;
}

/// Removes a helpfile by its primary key.
/// @param id: The id of the helpfile to remove.
/// @returns The number of rows removed.
int HelpRepository::Remove(int id)
{
	auto stmt = db.Prepare("DELETE FROM helpfiles WHERE id = ?");

	stmt->Bind(id);

	auto affected = stmt->Execute();
	return affected < 0 ? 0 : (int)affected;
}
