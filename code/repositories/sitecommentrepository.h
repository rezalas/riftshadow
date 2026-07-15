#ifndef SITECOMMENTREPOSITORY_H
#define SITECOMMENTREPOSITORY_H

#include <string>
#include <vector>
#include "../stdlibs/dbsession.h"

/// Model used to represent the `sitecomments` table (one admin comment left on a
/// tracked site, shown by the `sitetrack` command).
/// @note Column order in the schema --
///   site_id, comment_id, comment_name, comment_date, comment
/// @note `comment_id` is an auto_increment PRIMARY KEY, so Add supplies only the
///   other four columns and lets the server assign it.
/// @note `comment` is a MySQL keyword, so it is backticked wherever named (like
///   `votes.time` / `pklogs.date`). No other column needs quoting.
/// @note The `sitecomments` table lives in the `rift` database, not `rift_core`.
///   Construct this repository with RS.DbRift, never RS.Db.
struct SiteComment
{
	int			site_id = 0;		// the tracked site this comment is attached to
	int			comment_id = 0;		// auto_increment primary key
	std::string	comment_name;		// the immortal who left the comment
	std::string	comment_date;		// the display timestamp string (e.g. "Tue Nov 14,  5:53pm")
	std::string	comment;			// the comment body
};

/// Repository used to manage the `sitecomments` table.
class SiteCommentRepository
{
public:
	explicit SiteCommentRepository(IDbSession &db) : db(db) {}

	int CountBySite(int siteId);
	std::vector<SiteComment> FindBySite(int siteId);
	bool Add(const SiteComment &comment);
	int Remove(int commentId);

private:
	IDbSession &	db;
};

#endif /* SITECOMMENTREPOSITORY_H */
