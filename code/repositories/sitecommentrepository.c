#include "sitecommentrepository.h"

/// The explicit column list, in schema order.
/// @note `comment` is a MySQL keyword, so it is backticked.
static const std::string kSelect =
	"SELECT site_id, comment_id, comment_name, comment_date, `comment` FROM sitecomments";

/// Maps one result row onto a SiteComment, in schema column order.
static SiteComment MapSiteComment(const Row &row)
{
	SiteComment comment;
	comment.site_id = row.I32(0);
	comment.comment_id = row.I32(1);
	comment.comment_name = row.Str(2);
	comment.comment_date = row.Str(3);
	comment.comment = row.Str(4);
	return comment;
}

/// Counts the comments attached to a site (the `sitetrack` listing column).
/// @param siteId: The tracked site's id.
/// @returns The number of comments on that site (0 if none or on error).
int SiteCommentRepository::CountBySite(int siteId)
{
	auto stmt = db.Prepare("SELECT COUNT(*) FROM sitecomments WHERE site_id = ?");

	stmt->Bind(siteId);

	auto counts = stmt->Query<int>([](const Row &row) { return row.I32(0); });
	return counts.empty() ? 0 : counts[0];
}

/// Finds every comment attached to a site, in insertion order.
/// @param siteId: The tracked site's id.
/// @returns The matching comments (empty if none).
std::vector<SiteComment> SiteCommentRepository::FindBySite(int siteId)
{
	auto stmt = db.Prepare(kSelect + " WHERE site_id = ?");

	stmt->Bind(siteId);

	return stmt->Query<SiteComment>(MapSiteComment);
}

/// Creates a comment on a site in the database.
/// @param comment: The comment to record (comment_id is assigned by the server's
///   auto_increment, so only the other four columns are supplied).
/// @returns true on success; false otherwise.
bool SiteCommentRepository::Add(const SiteComment &comment)
{
	auto stmt = db.Prepare(
		"INSERT INTO sitecomments (site_id, comment_name, comment_date, `comment`) "
		"VALUES (?, ?, ?, ?)");

	stmt->Bind(comment.site_id)
		.Bind(comment.comment_name)
		.Bind(comment.comment_date)
		.Bind(comment.comment);

	return stmt->Execute() >= 0;
}

/// Removes a single comment by its primary key (the `delcomment` command).
/// @param commentId: The comment's auto_increment id.
/// @returns The number of rows removed.
int SiteCommentRepository::Remove(int commentId)
{
	auto stmt = db.Prepare("DELETE FROM sitecomments WHERE comment_id = ?");

	stmt->Bind(commentId);

	auto affected = stmt->Execute();
	return affected < 0 ? 0 : (int)affected;
}
