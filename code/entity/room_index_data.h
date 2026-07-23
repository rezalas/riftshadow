#ifndef ENTITY_ROOM_INDEX_DATA_H
#define ENTITY_ROOM_INDEX_DATA_H

#include "fwd.h"
#include "limits.h"

//
// Room type.
//

struct room_index_data
{
	ROOM_INDEX_DATA *next;
	ROOM_INDEX_DATA *next_room;
	ROOM_INDEX_DATA *aff_next;
	CHAR_DATA *people;
	OBJ_DATA *contents;
	EXTRA_DESCR_DATA *extra_descr;
	AREA_DATA *area;
	EXIT_DATA *exit[6];
	TRACK_DATA *tracks[20];
	PATHFIND_DATA *path;				// For smart tracking
	char *name;
	char *alt_name;
	char *description;
	char *alt_description;
	char *owner;
	short alt_description_cond;
	short vnum;
	long room_flags[MAX_BITVECTOR];
	short sector_type;
	short heal_rate;
	short mana_rate;
	short cabal;
	short guild;
	ROOM_AFFECT_DATA *affected;
	long affected_by[MAX_BITVECTOR];
	RPROG_DATA *rprogs;
	long progtypes[MAX_BITVECTOR];
	bool move_progs;
	TRAP_DATA *trap;
	RUNE_DATA *rune;
	bool has_rune;
	short light;
	RESET_DATA *reset_first;			// OLC
	RESET_DATA *reset_last; 			// OLC
};

#endif /* ENTITY_ROOM_INDEX_DATA_H */
