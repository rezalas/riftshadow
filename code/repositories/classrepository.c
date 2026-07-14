#include "classrepository.h"

/// Maps one `class_table` result row to a ClassRow. Column 0 (`id`) is the
/// ordering key only and is not stored.
/// @param row: The fetched result row.
/// @returns The mapped ClassRow.
static ClassRow MapClassRow(const Row &row)
{
	ClassRow c;
	c.name			= row.Str(1);
	c.who_name		= row.Str(2);
	c.attr_prime	= row.I32(3);
	c.align			= row.I32(4);
	c.weapon		= row.I32(5);
	c.gainconst		= row.I32(6);
	c.base_group	= row.Str(7);
	c.default_group	= row.Str(8);
	c.ctype			= row.I32(9);
	c.status		= row.I32(10);
	return c;
}

/// Reads every class definition, ordered by id ascending (the boot-time load).
/// The id-ascending order is load-bearing: it fixes each class's positional
/// `index`, which the rest of the game uses as its class id.
/// @returns All class rows in id order (empty on error or if the table is empty).
std::vector<ClassRow> ClassRepository::FindAllOrdered()
{
	auto stmt = db.Prepare(
		"SELECT id, name, who_name, attr_prime, align, weapon, gainconst, "
		"base_group, default_group, ctype, status "
		"FROM class_table ORDER BY id ASC");

	return stmt->Query<ClassRow>(MapClassRow);
}
