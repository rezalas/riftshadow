#include "playerrepository.h"

/// Creates a fresh character's stats row (character creation).
/// @param player: The new character. All sixteen columns are written; a new
///   character starts with logins/noc_logins/c_logins/gold/pks/induct at 0, so
///   the caller leaves those defaulted.
/// @returns true on success; false otherwise.
bool PlayerRepository::Add(const Player &player)
{
	auto stmt = db.Prepare(
		"INSERT INTO players "
		"(name, lastlogin, level, class, race, cabal, sex, hours, align, ethos, "
		"logins, noc_logins, c_logins, gold, pks, induct) "
		"VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");

	stmt->Bind(player.name)
		.Bind(player.lastlogin)
		.Bind(player.level)
		.Bind(player.class_)
		.Bind(player.race)
		.Bind(player.cabal)
		.Bind(player.sex)
		.Bind(player.hours)
		.Bind(player.align)
		.Bind(player.ethos)
		.Bind(player.logins)
		.Bind(player.noc_logins)
		.Bind(player.c_logins)
		.Bind(player.gold)
		.Bind((double)player.pks)
		.Bind(player.induct);

	return stmt->Execute() >= 0;
}

/// Persists the mutable stats of an existing character (the per-save write in
/// save_char_obj). Keyed on the unique name.
/// @param player: The character whose stats are saved. Only the columns the game
///   updates on save are written (pks, level, class, race, cabal, sex, hours,
///   align, ethos, gold); name is the key.
/// @returns true on success; false otherwise.
bool PlayerRepository::SaveStats(const Player &player)
{
	auto stmt = db.Prepare(
		"UPDATE players SET pks = ?, level = ?, class = ?, race = ?, cabal = ?, "
		"sex = ?, hours = ?, align = ?, ethos = ?, gold = ? WHERE name = ? LIMIT 1");

	stmt->Bind((double)player.pks)
		.Bind(player.level)
		.Bind(player.class_)
		.Bind(player.race)
		.Bind(player.cabal)
		.Bind(player.sex)
		.Bind(player.hours)
		.Bind(player.align)
		.Bind(player.ethos)
		.Bind(player.gold)
		.Bind(player.name);

	return stmt->Execute() >= 0;
}

/// Records a login: bumps the login counter and stamps the last-login time.
/// @note `logins = logins + 1` is a self-referential update, so it stays literal
///   SQL; only the new timestamp and the name are bound.
/// @param name: The character's true name.
/// @param lastlogin: The unix epoch of this logon.
/// @returns true on success; false otherwise.
bool PlayerRepository::RecordLogin(const std::string &name, int lastlogin)
{
	auto stmt = db.Prepare(
		"UPDATE players SET logins = logins + 1, lastlogin = ? WHERE name = ?");

	stmt->Bind(lastlogin)
		.Bind(name);

	return stmt->Execute() >= 0;
}

/// Renames a character's stats row (the admin rename command).
/// @param oldName: The character's current name.
/// @param newName: The new name to store.
/// @returns true on success; false otherwise.
bool PlayerRepository::Rename(const std::string &oldName, const std::string &newName)
{
	auto stmt = db.Prepare("UPDATE players SET name = ? WHERE name = ? LIMIT 1");

	stmt->Bind(newName)
		.Bind(oldName);

	return stmt->Execute() >= 0;
}

/// Updates a character's cabal induction rank.
/// @param name: The character's true name.
/// @param induct: The new induction rank.
/// @returns true on success; false otherwise.
bool PlayerRepository::SetInduct(const std::string &name, int induct)
{
	auto stmt = db.Prepare("UPDATE players SET induct = ? WHERE name = ?");

	stmt->Bind(induct)
		.Bind(name);

	return stmt->Execute() >= 0;
}

/// Removes a character's stats row (character deletion / player-nuke sweep /
/// level>=52 immortal promotion).
/// @param name: The character's name.
/// @returns The number of rows removed.
int PlayerRepository::RemoveByName(const std::string &name)
{
	auto stmt = db.Prepare("DELETE FROM players WHERE name = ?");

	stmt->Bind(name);

	auto affected = stmt->Execute();
	return affected < 0 ? 0 : (int)affected;
}

/// Finds a character's full stats row by name (the `dbinfo`/`show_database_info`
/// admin lookup).
/// @note Explicit column list, id-ordered to match the entity, rather than a
///   positional `SELECT *`.
/// @param name: The character's true name (bound, never interpolated).
/// @returns A vector holding the single matching row, or empty if no such
///   character exists.
std::vector<Player> PlayerRepository::FindByName(const std::string &name)
{
	auto stmt = db.Prepare(
		"SELECT name, lastlogin, level, class, race, cabal, sex, hours, align, "
		"ethos, logins, noc_logins, c_logins, gold, pks, induct "
		"FROM players WHERE name = ? LIMIT 1");

	stmt->Bind(name);

	return stmt->Query<Player>([](const Row &row) {
		Player player;
		player.name = row.Str(0);
		player.lastlogin = row.I32(1);
		player.level = row.I32(2);
		player.class_ = row.I32(3);
		player.race = row.I32(4);
		player.cabal = row.I32(5);
		player.sex = row.I32(6);
		player.hours = row.I32(7);
		player.align = row.I32(8);
		player.ethos = row.I32(9);
		player.logins = row.I32(10);
		player.noc_logins = row.I32(11);
		player.c_logins = row.I32(12);
		player.gold = row.I64(13);
		player.pks = row.Flt(14);
		player.induct = row.I32(15);
		return player;
	});
}

/// Lists every stored character name (used to reconcile the DB against pfiles).
/// @returns All names (empty if none).
std::vector<std::string> PlayerRepository::FindAllNames()
{
	auto stmt = db.Prepare("SELECT name FROM players");

	return stmt->Query<std::string>([](const Row &row) { return row.Str(0); });
}

/// Lists the names of characters that have not logged in since a cutoff (the
/// auto-delete pass).
/// @param cutoff: Characters whose lastlogin is strictly less than this are
///   returned. The caller passes `now - inactivityWindow`.
/// @returns The matching names (empty if none).
std::vector<std::string> PlayerRepository::FindNamesInactiveSince(long cutoff)
{
	auto stmt = db.Prepare("SELECT name FROM players WHERE lastlogin < ?");

	stmt->Bind((long long)cutoff);

	return stmt->Query<std::string>([](const Row &row) { return row.Str(0); });
}

/// Lists character names ordered by PK count, highest first.
/// @param limit: The maximum number of names to return (bound, not interpolated).
/// @returns The top names by pks (empty if none).
std::vector<std::string> PlayerRepository::FindTopByPks(int limit)
{
	auto stmt = db.Prepare("SELECT name FROM players ORDER BY pks DESC LIMIT ?");

	stmt->Bind(limit);

	return stmt->Query<std::string>([](const Row &row) { return row.Str(0); });
}

/// Counts the characters above a level that carry a gold value (the gold-economy
/// tuning input; historically `COUNT(gold) WHERE level > 30`).
/// @param level: The exclusive lower level bound.
/// @returns The matching count (0 if none or on error).
int PlayerRepository::CountGoldAboveLevel(int level)
{
	auto stmt = db.Prepare("SELECT COUNT(gold) FROM players WHERE level > ?");

	stmt->Bind(level);

	auto counts = stmt->Query<int>([](const Row &row) { return row.I32(0); });
	return counts.empty() ? 0 : counts[0];
}

/// Sums the gold across every character (the gold-economy tuning input).
/// @returns The total gold (0 if no rows or on error).
long long PlayerRepository::SumGold()
{
	auto stmt = db.Prepare("SELECT SUM(gold) FROM players");

	auto sums = stmt->Query<long long>([](const Row &row) { return row.I64(0); });
	return sums.empty() ? 0 : sums[0];
}

/// Computes the game-wide cabal "power" percentage -- the share of logins that
/// happened while a cabal was powered, averaged across every character that has
/// ever logged in while powered (`c_logins > 0`).
/// @note `avg(c_logins) / (avg(c_logins) + avg(noc_logins)) * 100`. Used as the
///   game-wide baseline in `show_database_info`.
/// @returns The percentage (0.0 if no rows / the average is NULL).
float PlayerRepository::AverageCabalPowerPercent()
{
	auto stmt = db.Prepare(
		"SELECT avg(c_logins) / (avg(c_logins) + avg(noc_logins)) * 100 "
		"FROM players WHERE c_logins > 0");

	auto vals = stmt->Query<float>([](const Row &row) { return row.Flt(0); });
	return vals.empty() ? 0.0f : vals[0];
}

/// Computes the cabal-wide "power" percentage restricted to a single cabal --
/// the same average as the argument-less overload, filtered to one cabal.
/// @param cabal: The cabal id to restrict the average to (bound, never
///   interpolated).
/// @returns The percentage (0.0 if the cabal has no qualifying rows).
float PlayerRepository::AverageCabalPowerPercent(int cabal)
{
	auto stmt = db.Prepare(
		"SELECT avg(c_logins) / (avg(c_logins) + avg(noc_logins)) * 100 "
		"FROM players WHERE c_logins > 0 AND cabal = ?");

	stmt->Bind(cabal);

	auto vals = stmt->Query<float>([](const Row &row) { return row.Flt(0); });
	return vals.empty() ? 0.0f : vals[0];
}
