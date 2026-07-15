#include "sitetrackerrepository.h"

/// The explicit column list, in schema order.
static const std::string kSelect =
	"SELECT site_id, site_name, denials FROM sitetracker";

/// Maps one result row onto a SiteTracker, in schema column order.
static SiteTracker MapSiteTracker(const Row &row)
{
	SiteTracker site;
	site.site_id = row.I32(0);
	site.site_name = row.Str(1);
	site.denials = row.I32(2);
	return site;
}

/// Finds every tracked site (the `sitetrack` listing).
/// @returns The tracked sites (empty if none).
std::vector<SiteTracker> SiteTrackerRepository::FindAll()
{
	auto stmt = db.Prepare(kSelect);

	return stmt->Query<SiteTracker>(MapSiteTracker);
}

/// Finds a tracked site by its primary key (the `sitetrack <id>` view).
/// @param siteId: The tracked site's auto_increment id.
/// @returns The matching site (0 or 1 row; empty on error or if none).
std::vector<SiteTracker> SiteTrackerRepository::FindById(int siteId)
{
	auto stmt = db.Prepare(kSelect + " WHERE site_id = ?");

	stmt->Bind(siteId);

	return stmt->Query<SiteTracker>(MapSiteTracker);
}

/// Creates a tracked site in the database (the `sitetrack add` command).
/// @param site: The site to record (site_id is assigned by the server's
///   auto_increment, so only site_name and denials are supplied).
/// @returns true on success; false otherwise.
bool SiteTrackerRepository::Add(const SiteTracker &site)
{
	auto stmt = db.Prepare(
		"INSERT INTO sitetracker (site_name, denials) VALUES (?, ?)");

	stmt->Bind(site.site_name)
		.Bind(site.denials);

	return stmt->Execute() >= 0;
}

/// Increments the denial count of every tracked site whose site_name matches a
/// host (the auto-deny path; site_name is treated as a regex the host must match).
/// @param host: The connecting host, matched against each site_name via RLIKE.
/// @returns The number of rows updated.
int SiteTrackerRepository::IncrementDenialsByHost(const std::string &host)
{
	auto stmt = db.Prepare(
		"UPDATE sitetracker SET denials = denials + 1 WHERE ? RLIKE site_name");

	stmt->Bind(host);

	auto affected = stmt->Execute();
	return affected < 0 ? 0 : (int)affected;
}

/// Removes a single tracked site by its primary key (the `delsite` command).
/// @param siteId: The site's auto_increment id.
/// @returns The number of rows removed.
int SiteTrackerRepository::Remove(int siteId)
{
	auto stmt = db.Prepare("DELETE FROM sitetracker WHERE site_id = ?");

	stmt->Bind(siteId);

	auto affected = stmt->Execute();
	return affected < 0 ? 0 : (int)affected;
}
