#include "banrepository.h"

/// Finds the active bans matching a specific ban type and host type.
/// @note This function only populates the site and duration fields.
/// @param banType: The type of ban. Should be either NBAN_ALL or NBAN_NEWBIE.
/// @param hostType: The type of host. Should be either NBAN_IP or NBAN_HOST.
/// @returns A list of bans containing the specified ban type and host type.
std::vector<Ban> BanRepository::FindByType(int banType, int hostType)
{
	auto stmt = db.Prepare(
		"SELECT site, duration FROM bans WHERE ban_type = ? AND host_type = ?");

	stmt->Bind(banType).Bind(hostType);

	return stmt->Query<Ban>([](const Row &row) {
		Ban ban;
		ban.site = row.Str(0);
		ban.duration = row.I64(1);
		return ban;
	});
}

/// Finds all bans in the system sorted by duration in descending order.
/// @note The date field is pre-formatted (MM/DD/YYYY).
/// @returns A list of all bans in the system.
std::vector<Ban> BanRepository::FindAllOrderedByDuration()
{
	auto stmt = db.Prepare(
		"SELECT site, `by`, reason, DATE_FORMAT(date, '%m/%d/%Y'), "
		"duration, ban_type, host_type "
		"FROM bans ORDER BY duration DESC");

	return stmt->Query<Ban>([](const Row &row) {
		Ban ban;
		ban.site = row.Str(0);
		ban.by = row.Str(1);
		ban.reason = row.Str(2);
		ban.date = row.Str(3);
		ban.duration = row.I64(4);
		ban.ban_type = row.I32(5);
		ban.host_type = row.I32(6);
		return ban;
	});
}

/// Creates a ban entry in the database.
/// @param ban: The ban to add to the database.
/// @returns true on success; false otherwise.
bool BanRepository::Add(const Ban &ban)
{
	auto stmt = db.Prepare(
		"INSERT INTO bans (site, `by`, reason, date, duration, ban_type, host_type) "
		"VALUES (?, ?, ?, ?, ?, ?, ?)");

	stmt->Bind(ban.site)
		.Bind(ban.by)
		.Bind(ban.reason)
		.Bind(ban.date)
		.Bind(ban.duration)
		.Bind(ban.ban_type)
		.Bind(ban.host_type);

	return stmt->Execute() >= 0;
}

/// Removes every ban whose site contains the fragment (ban.c:197).
/// @param fragment: The substring to match against the site column.
/// @returns The number of rows removed.
int BanRepository::RemoveBySiteContaining(const std::string &fragment)
{
	auto stmt = db.Prepare("DELETE FROM bans WHERE site LIKE ?");

	stmt->Bind("%" + fragment + "%");

	auto affected = stmt->Execute();
	return affected < 0 ? 0 : (int)affected;
}
