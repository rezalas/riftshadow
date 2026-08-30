#ifndef ENTITY_AFFECT_DATA_H
#define ENTITY_AFFECT_DATA_H

#include "../enums.h"
#include "fwd.h"
#include "limits.h"
#include "../stdlibs/handle.h"

//
// An affect.
//

// A character affect. Owns its `name` string (palloc_string / free_pstring) →
// rule-of-5 (bodies in recycle.c). Parents hold these by value in a
// std::list<AFFECT_DATA> (ch->affected, obj->charaffs, and the two Tier-0
// prototype lists obj_index->affected / obj_index->charaffs). `owner` is a
// non-owning CHAR_DATA back-reference and expires on its own when that
// character is freed. std::list, not vector: callers cache an element pointer
// returned by affect_find across later list mutations.
struct affect_data
{
	Handle<CHAR_DATA> owner;
	char *name = nullptr;
	AffectWhere where = TO_AFFECTS;
	short type = 0;
	short level = 0;
	short duration = 0;
	ApplyLocation location = APPLY_NONE;
	short modifier = 0;
	short mod_name = 0;
	long bitvector[MAX_BITVECTOR] = {};
	int aftype = 0;
	AFF_FUN *tick_fun = nullptr;	// goes off every tick that char is affected
	AFF_FUN *pulse_fun = nullptr;
	AFF_FUN *end_fun = nullptr;		// when the affect wears off this is called
	short init_duration = 0;
	AFF_FUN *beat_fun = nullptr;	// goes off every beat

	affect_data() = default;
	~affect_data();

	affect_data(const affect_data &other);
	affect_data &operator=(const affect_data &other);
	affect_data(affect_data &&other) noexcept;
	affect_data &operator=(affect_data &&other) noexcept;
};

//
// A room affect.
//

// A room affect. Plain value type; a room owns these by value in a
// std::list<ROOM_AFFECT_DATA> (room->affected). `owner` is a non-owning
// CHAR_DATA back-reference. Note extract_char still deletes a character's room
// affects outright: the affect keeps damaging whoever walks in, so losing the
// attribution is not the same as the effect ending.
struct room_affect_data
{
	Handle<CHAR_DATA> owner;
	RoomAffectWhere where = TO_ROOM_AFFECTS;
	short type = 0;
	short level = 0;
	short duration = 0;
	ApplyRoomLocation location = APPLY_ROOM_NONE;
	short modifier = 0;
	long bitvector[MAX_BITVECTOR] = {};
	int aftype = 0;
	RAFF_FUN *pulse_fun = nullptr;
	RAFF_FUN *tick_fun = nullptr;	// goes off every tick
	RAFF_FUN *end_fun = nullptr;	// when the affect wears off this is called
};

//
// An area affect.
//

// An area affect. Plain value type; an area owns these by value in a
// std::list<AREA_AFFECT_DATA> (area->affected). `owner` is a non-owning
// CHAR_DATA back-reference and expires on its own when that character is
// freed; nothing has ever scrubbed it, and an area affect outlives the fight
// that spawned it by design.
struct area_affect_data
{
	Handle<CHAR_DATA> owner;
	AreaAffectWhere where = TO_AREA_AFFECTS;
	short type = 0;
	short level = 0;
	short duration = 0;
	ApplyAreaLocation location = APPLY_AREA_NONE;
	short modifier = 0;
	long bitvector[MAX_BITVECTOR] = {};
	int aftype = 0;
	AAFF_FUN *pulse_fun = nullptr;
	AAFF_FUN *tick_fun = nullptr;
	AAFF_FUN *end_fun = nullptr;
};

// An object affect. Plain value type; an object owns these by value in a
// std::list<OBJ_AFFECT_DATA> (obj->affected). `owner` is a non-owning
// CHAR_DATA back-reference and stays a raw pointer.
struct obj_affect_data
{
	Handle<CHAR_DATA> owner;
	ObjAffectWhere where = TO_OBJ_AFFECTS;
	short type = 0;
	short level = 0;
	short duration = 0;
	// Two families live in this field, and which one is here is decided
	// by `where` above. TO_OBJ_AFFECTS means an ApplyObjLocation, one of
	// the object's own value slots. TO_OBJ_APPLY means an ApplyLocation,
	// a character apply that the object hands to whoever wears it. A
	// single type here would be a claim that only one of them is true.
	short location = 0;
	short modifier = 0;
	long bitvector[MAX_BITVECTOR] = {};
	int aftype = 0;
	OAFF_FUN *pulse_fun = nullptr;
	OAFF_FUN *tick_fun = nullptr;
	OAFF_FUN *end_fun = nullptr;
};

// A stat apply on an object. Plain value type; parents own these by value in a
// std::list<OBJ_APPLY_DATA> (list, not vector: spell_enchant_weapon holds element
// pointers across a second prepend, so nodes must not move).
struct obj_apply_data
{
	ApplyLocation location = APPLY_NONE;
	short modifier = 0;
	short type = 0;						// For gsns, if relevant.
};

#endif /* ENTITY_AFFECT_DATA_H */
