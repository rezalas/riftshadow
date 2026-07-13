#include "offeringrepository.h"

/// The explicit column list, in schema order.
/// @note `time` is a MySQL keyword, so it is backticked.
static const std::string kSelect =
	"SELECT deity, offeringvnum, offeringshort, player, status, `time`, id FROM offerings";

/// Maps one result row onto an Offering, in schema column order.
static Offering MapOffering(const Row &row)
{
	Offering offering;
	offering.deity = row.Str(0);
	offering.offeringvnum = row.I32(1);
	offering.offeringshort = row.Str(2);
	offering.player = row.Str(3);
	offering.status = row.I32(4);
	offering.time = row.I64(5);
	offering.id = row.I32(6);
	return offering;
}

/// Finds every offering made to a deity, oldest first.
/// @param deity: The deity (altar room owner) the offerings were made to.
/// @returns The offerings ordered by time ascending (empty if none).
std::vector<Offering> OfferingRepository::FindByDeity(const std::string &deity)
{
	auto stmt = db.Prepare(kSelect + " WHERE deity = ? ORDER BY `time` ASC");

	stmt->Bind(deity);

	return stmt->Query<Offering>(MapOffering);
}

/// Finds a deity's unviewed offerings (status = 0), oldest first.
/// @note Backs the `listoffer auto` existence check; the caller only inspects
///   whether the result is non-empty.
/// @param deity: The deity (altar room owner) the offerings were made to.
/// @returns The unviewed offerings ordered by time ascending (empty if none).
std::vector<Offering> OfferingRepository::FindUnviewedByDeity(const std::string &deity)
{
	auto stmt = db.Prepare(kSelect + " WHERE deity = ? AND status = 0 ORDER BY `time` ASC");

	stmt->Bind(deity);

	return stmt->Query<Offering>(MapOffering);
}

/// Finds the most recent offering a player made to a deity.
/// @param deity: The deity (altar room owner) the offering was made to.
/// @param player: The offering player's name.
/// @returns The latest matching offering (0 or 1 row; empty if none).
std::vector<Offering> OfferingRepository::FindLatestByDeityAndPlayer(const std::string &deity, const std::string &player)
{
	auto stmt = db.Prepare(kSelect + " WHERE deity = ? AND player = ? ORDER BY `time` DESC LIMIT 1");

	stmt->Bind(deity)
		.Bind(player);

	return stmt->Query<Offering>(MapOffering);
}

/// Creates a new offering (status defaults to 0/unviewed via the entity).
/// @param offering: The offering to create (id auto-increments; not supplied).
/// @returns true on success; false otherwise.
bool OfferingRepository::Add(const Offering &offering)
{
	auto stmt = db.Prepare(
		"INSERT INTO offerings (deity, offeringvnum, offeringshort, player, status, `time`) "
		"VALUES (?, ?, ?, ?, ?, ?)");

	stmt->Bind(offering.deity)
		.Bind(offering.offeringvnum)
		.Bind(offering.offeringshort)
		.Bind(offering.player)
		.Bind(offering.status)
		.Bind(offering.time);

	return stmt->Execute() >= 0;
}

/// Sets an offering's status (new/rejected/accepted) by its primary key.
/// @note The legacy update matched on `time` + `player`; the entity now carries
///   the auto_increment `id`, so this targets the exact row by primary key.
/// @param id: The offering's id.
/// @param status: The new status (0 = new, 1 = rejected, 2 = accepted).
/// @returns The number of rows updated.
int OfferingRepository::SetStatus(int id, int status)
{
	auto stmt = db.Prepare("UPDATE offerings SET status = ? WHERE id = ?");

	stmt->Bind(status)
		.Bind(id);

	auto affected = stmt->Execute();
	return affected < 0 ? 0 : (int)affected;
}

/// Removes every offering made before a cutoff (the periodic prune).
/// @param cutoff: Offerings with a time strictly less than this are removed.
/// @returns The number of rows removed.
int OfferingRepository::RemoveOlderThan(long long cutoff)
{
	auto stmt = db.Prepare("DELETE FROM offerings WHERE `time` < ?");

	stmt->Bind(cutoff);

	auto affected = stmt->Execute();
	return affected < 0 ? 0 : (int)affected;
}

/// Removes every offering made by a player (part of the player-nuke sweep).
/// @param name: The offering player's name.
/// @returns The number of rows removed.
int OfferingRepository::RemoveByPlayer(const std::string &name)
{
	auto stmt = db.Prepare("DELETE FROM offerings WHERE player = ?");

	stmt->Bind(name);

	auto affected = stmt->Execute();
	return affected < 0 ? 0 : (int)affected;
}
