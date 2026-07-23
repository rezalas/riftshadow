#ifndef ENTITY_AREA_DATA_H
#define ENTITY_AREA_DATA_H

#include "fwd.h"
#include "limits.h"

//
// Area definition.
//

struct area_data
{
	AREA_DATA *next;
	AREA_DATA *prev;
	RESET_DATA *reset_first;
	RESET_DATA *reset_last;
	APROG_DATA *aprogs;
	long progtypes[MAX_BITVECTOR];
	char *file_name;
	char *name;
	char *credits;
	short age;
	short nplayer;
	short low_range;
	short high_range;
	short min_vnum;
	short max_vnum;
	short area_type;
	long area_flags[MAX_BITVECTOR];
	bool empty;
	long affected_by[MAX_BITVECTOR];
	AREA_AFFECT_DATA *affected;
	AREA_DATA *adjacent[MAX_ADJACENT];
	short climate;
	short sky;
	short temp;
	short wind;
	char *builders;						// OLC - Listing of
	int vnum;							// OLC - Area vnum
	int security;						// OLC - Value 1-9
};

#endif /* ENTITY_AREA_DATA_H */
