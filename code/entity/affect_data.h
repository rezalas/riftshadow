#ifndef ENTITY_AFFECT_DATA_H
#define ENTITY_AFFECT_DATA_H

#include "fwd.h"
#include "limits.h"

//
// An affect.
//

struct affect_data
{
	AFFECT_DATA *next;
	CHAR_DATA *owner;
	char *name;
	bool valid;
	short where;
	short type;
	short level;
	short duration;
	short location;
	short modifier;
	short mod_name;
	long bitvector[MAX_BITVECTOR];
	int aftype;
	AFF_FUN *tick_fun;		// goes off every tick that char is affected
	AFF_FUN *pulse_fun;
	AFF_FUN *end_fun;		// when the affect wears off this is called
	short init_duration;
	AFF_FUN *beat_fun;		// goes off every beat
};

//
// A room affect.
//

struct room_affect_data
{
	ROOM_AFFECT_DATA *next;
	CHAR_DATA *owner;
	bool valid;
	short where;
	short type;
	short level;
	short duration;
	short location;
	short modifier;
	long bitvector[MAX_BITVECTOR];
	int aftype;
	RAFF_FUN *pulse_fun;
	RAFF_FUN *tick_fun;	// goes off every tick
	RAFF_FUN *end_fun;	// when the affect wears off this is called
};

//
// An area affect.
//

struct area_affect_data
{
	AREA_AFFECT_DATA *next;
	CHAR_DATA *owner;
	bool valid;
	short where;
	short type;
	short level;
	short duration;
	short location;
	short modifier;
	long bitvector[MAX_BITVECTOR];
	int aftype;
	AAFF_FUN *pulse_fun;
	AAFF_FUN *tick_fun;
	AAFF_FUN *end_fun;
};

struct obj_affect_data
{
	OBJ_AFFECT_DATA *next;
	CHAR_DATA *owner;
	bool valid;
	short where;
	short type;
	short level;
	short duration;
	short location;
	short modifier;
	long bitvector[MAX_BITVECTOR];
	int aftype;
	OAFF_FUN *pulse_fun;
	OAFF_FUN *tick_fun;
	OAFF_FUN *end_fun;
};

struct obj_apply_data
{
	bool valid;
	short location;
	short modifier;
	short type;						// For gsns, if relevant.
	OBJ_APPLY_DATA *next;
};

#endif /* ENTITY_AFFECT_DATA_H */
