#ifndef ENTITY_OBJ_INDEX_DATA_H
#define ENTITY_OBJ_INDEX_DATA_H

#include "fwd.h"
#include "limits.h"
#include "spec_func.h"

//
// Prototype for an object.
//

struct obj_index_data
{
	OBJ_INDEX_DATA *next;
	EXTRA_DESCR_DATA *extra_descr;
	AFFECT_DATA *affected;
	AFFECT_DATA *charaffs;
	OBJ_APPLY_DATA *apply;
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
	SPEC_FUNC spec_prog;
};

#endif /* ENTITY_OBJ_INDEX_DATA_H */
