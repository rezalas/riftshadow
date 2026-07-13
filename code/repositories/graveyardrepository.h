#ifndef GRAVEYARDREPOSITORY_H
#define GRAVEYARDREPOSITORY_H

#include <string>
#include "../stdlibs/dbsession.h"

/// Model used to represent the `graveyard` table (one memorial row recorded when a
/// high-level character dies, capturing their final PK/frag stats; the death-board
/// archive written by `plug_graveyard`).
/// @note Column order in the schema --
///   ID, Pname, Pfrags, Pfgood, Pfneutral, Pfevil, Pfdeaths, Pmdeaths, Phours
/// @note `ID` is an auto_increment PRIMARY KEY, so Add supplies only the other
///   columns and lets the server assign it.
/// @note Columns are PascalCase in the schema and are mirrored verbatim here
///   (same convention as `buglist`, Step 7). No column is a MySQL keyword, so
///   nothing needs backticking.
/// @note The `graveyard` table lives in the `rift` database, not `rift_core`.
///   Construct this repository with RS.DbRift, never RS.Db.
struct Graveyard
{
	int			ID = 0;				// auto_increment primary key
	std::string	Pname;				// the character's name (varchar(50))
	float		Pfrags = 0.0f;		// PK kills
	float		Pfgood = 0.0f;		// good-aligned frags
	float		Pfneutral = 0.0f;	// neutral-aligned frags
	float		Pfevil = 0.0f;		// evil-aligned frags
	float		Pfdeaths = 0.0f;	// times fragged (PK deaths)
	int			Pmdeaths = 0;		// mob-inflicted deaths
	int			Phours = 0;			// hours played
};

/// Repository used to manage the `graveyard` table.
class GraveyardRepository
{
public:
	explicit GraveyardRepository(IDbSession &db) : db(db) {}

	bool Add(const Graveyard &grave);

private:
	IDbSession &	db;
};

#endif /* GRAVEYARDREPOSITORY_H */
