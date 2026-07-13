#include "graveyardrepository.h"

/// Creates a tombstone for a deceased character on the graveyard board.
/// @param grave: The memorial row to record (ID is assigned by the server's
///   auto_increment, so it is omitted from the explicit column list).
/// @returns true on success; false otherwise.
bool GraveyardRepository::Add(const Graveyard &grave)
{
	auto stmt = db.Prepare(
		"INSERT INTO graveyard "
		"(Pname, Pfrags, Pfgood, Pfneutral, Pfevil, Pfdeaths, Pmdeaths, Phours) "
		"VALUES (?, ?, ?, ?, ?, ?, ?, ?)");

	stmt->Bind(grave.Pname)
		.Bind(grave.Pfrags)
		.Bind(grave.Pfgood)
		.Bind(grave.Pfneutral)
		.Bind(grave.Pfevil)
		.Bind(grave.Pfdeaths)
		.Bind(grave.Pmdeaths)
		.Bind(grave.Phours);

	return stmt->Execute() >= 0;
}
