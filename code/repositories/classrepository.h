#ifndef CLASSREPOSITORY_H
#define CLASSREPOSITORY_H

#include <string>
#include <vector>
#include "../stdlibs/dbsession.h"

/// Model used to represent one `class_table` row (a static class definition loaded
/// once at boot; drives the in-memory `CClass` list via CClass::LoadClassTable).
/// @note Column order in the schema --
///   id, name, who_name, attr_prime, align, weapon, gainconst, base_group,
///   default_group, ctype, status
/// @note `id` is the ordering key only; it maps to `CClass::index` positionally
///   (0-based, in id-ascending order) rather than being stored on the row.
/// @note The `class_table` table lives in the `rift_core` database, so construct
///   this repository with RS.Db (like players/bans/theft), never RS.DbRift.
struct ClassRow
{
	std::string	name;			// class name ("paladin")
	std::string	who_name;		// 3-letter tag shown on who
	int			attr_prime = 0;	// prime attribute
	int			align = 0;
	int			weapon = 0;		// starting weapon
	int			gainconst = 0;
	std::string	base_group;		// default skill groups granted at creation
	std::string	default_group;
	int			ctype = 0;		// neither / caster / communer
	int			status = 0;		// open for players to choose?
};

/// Repository used to read the `class_table` table (boot-time load only).
class ClassRepository
{
public:
	explicit ClassRepository(IDbSession &db) : db(db) {}

	// Reads
	std::vector<ClassRow> FindAllOrdered();

private:
	IDbSession &	db;
};

#endif /* CLASSREPOSITORY_H */
