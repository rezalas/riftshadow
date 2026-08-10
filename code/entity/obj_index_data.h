#ifndef ENTITY_OBJ_INDEX_DATA_H
#define ENTITY_OBJ_INDEX_DATA_H

#include <list>

#include "fwd.h"
#include "limits.h"
#include "extra_descr.h"
#include "affect_data.h"		// OBJ_APPLY_DATA held by value

//
// Prototype for an object.
//

struct obj_index_data
{
	OBJ_INDEX_DATA *next;
	std::list<EXTRA_DESCR_DATA> extra_descr;
	std::list<AFFECT_DATA> affected;
	std::list<AFFECT_DATA> charaffs;
	std::list<OBJ_APPLY_DATA> apply;
	AREA_DATA *area;
	bool new_format;
	char *name;
	char *short_descr;
	char *description;
	short vnum;
	short reset_num;
	short cabal;
	char *material;
	short material_index;
	short item_type;
	long extra_flags[MAX_BITVECTOR];
	long wear_flags[MAX_BITVECTOR];
	long restrict_flags[MAX_BITVECTOR];
	long imm_flags[MAX_BITVECTOR];
	long res_flags[MAX_BITVECTOR];
	long vuln_flags[MAX_BITVECTOR];
	short start_timer;
	short level;
	short condition;
	short limtotal;
	short limcount;
	short count;
	short weight;
	int cost;
	int value[5];
	IPROG_DATA *iprogs;
	long progtypes[MAX_BITVECTOR];
	char *wear_echo[2];
	char *remove_echo[2];
	char *verb;
	char *notes;
	char *wear_loc_name;

	// Attached object special program, or null. Owned by the static table in
	// ispec.c, so this is a borrowed pointer and is never freed.
	const struct ISpec *spec;
};

#endif /* ENTITY_OBJ_INDEX_DATA_H */
