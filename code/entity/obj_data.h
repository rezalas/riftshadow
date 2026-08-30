#ifndef ENTITY_OBJ_DATA_H
#define ENTITY_OBJ_DATA_H

#include <list>
#include <memory>

#include "fwd.h"
#include "limits.h"
#include "extra_descr.h"
#include "affect_data.h"		// OBJ_APPLY_DATA held by value
#include "../stdlibs/handle.h"	// self, and the handle-typed cross-references

//
// One object.
//

/// The global object list owns every object in the world. Declared here rather
/// than in db.h because an object caches its own node in it.
using ObjectList = std::list<std::unique_ptr<OBJ_DATA>>;

struct obj_data
{
	// Frees the owned strings and expires every handle to this object. Runs when
	// object_list erases the node (which is how a real object dies) or when
	// free_obj deletes an object that was never linked.
	~obj_data();

	// A handle naming this object, issued by new_obj and retired by free_obj.
	// It is how another entity refers to this one without risking a pointer
	// into a recycled slot; null for an object that never came from new_obj.
	Handle<OBJ_DATA> self;
	// This object's node in the global object list, valid while it is on it.
	// Caching it is what makes extraction O(1) instead of a scan for the
	// predecessor, and erasing it is what destroys the object.
	ObjectList::iterator globalNode {};
	OBJ_DATA *next_content;
	OBJ_DATA *contains;
	// The container this object sits inside, if any. Kept in step with in_room
	// and carried_by by the obj_to_*/obj_from_* pair -- an object is in exactly
	// one of the three at a time.
	Handle<OBJ_DATA> in_obj;
	Handle<CHAR_DATA> carried_by;
	std::list<EXTRA_DESCR_DATA> extra_descr;
	std::list<OBJ_APPLY_DATA> apply;
	std::list<OBJ_AFFECT_DATA> affected;
	std::list<AFFECT_DATA> charaffs;
	OBJ_INDEX_DATA *pIndexData;
	ROOM_INDEX_DATA *in_room;
	RUNE_DATA *rune;
	char *talked;
	char *owner;
	char *name;
	char *short_descr;
	char *description;
	// Zero rather than a named type, which is what this field held before it
	// had a type: it is what an object that has not been given one has, and
	// the item table reads it back as "none". Naming it would stop the save
	// seam reporting a stored zero, which is a real anomaly.
	ItemType item_type = static_cast<ItemType>(0);
	long extra_flags[MAX_BITVECTOR];
	long wear_flags[MAX_BITVECTOR];
	long imm_flags[MAX_BITVECTOR];
	long res_flags[MAX_BITVECTOR];
	long vuln_flags[MAX_BITVECTOR];
	long affected_by[MAX_BITVECTOR];
	WearLocation wear_loc = WEAR_NONE;
	short weight;
	int cost;
	short level;
	short condition;
	char *material;
	short timer;
	int value[5];
	long progtypes[MAX_BITVECTOR];
	int ohp;
	int mob_vnum;
	bool moved;
	char *wear_loc_name;
};

#endif /* ENTITY_OBJ_DATA_H */
