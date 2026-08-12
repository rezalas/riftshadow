/****************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,         *
 *  Michael Seifert, Hans Henrik Stærfeldt, Tom Madsen, and Katja Nyboe.    *
 *                                                                          *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael           *
 *  Chastain, Michael Quan, and Mitchell Tse.                               *
 *                                                                          *
 *  In order to use any part of this Merc Diku Mud, you must comply with    *
 *  both the original Diku license in 'license.doc' as well the Merc        *
 *  license in 'license.txt'.  In particular, you may not remove either of  *
 *  these copyright notices.                                                *
 *                                                                          *
 *  Much time and thought has gone into this software and you are           *
 *  benefitting.  We hope that you share your changes too.  What goes       *
 *  around, comes around.                                                   *
 ****************************************************************************/
 
/****************************************************************************
 *  ROM 2.4 is copyright 1993-1996 Russ Taylor                              *
 *  ROM has been brought to you by the ROM consortium                       *
 *      Russ Taylor (rtaylor@efn.org)                                       *
 *      Gabrielle Taylor (gtaylor@pacinfo.com)                              *
 *      Brian Moore (zump@rom.org)                                          *
 *  By using this code, you have agreed to follow the terms of the          *
 *  ROM license, in the file Tartarus/doc/rom.license                       *
 ****************************************************************************/

/****************************************************************************
 *       Tartarus code is copyright (C) 1997-1998 by Daniel Graham          *
 *  In using this code you agree to comply with the Tartarus license        *
 *       found in the file /Tartarus/doc/tartarus.doc                       *
 ****************************************************************************/

#ifndef DB_H
#define DB_H

#include <cstddef>
#include <list>
#include <memory>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>		// srandom
#include <unistd.h>		// getpid -- replaces the OLD_RAND hand declarations

#include "entity/obj_data.h"		// ObjectList: object_list owns its objects
#include "entity/char_data.h"		// CharacterList: char_list owns its characters
#include "entity/list_cursor.h"	// walking char_list while characters are extracted
#include "entity/extra_descr.h"		// get_extra_descr takes a std::list<EXTRA_DESCR_DATA>&

#include "entity/fwd.h"
#include "entity/limits.h"

#define MAX_KEY_HASH				1024
#define MAX_TOP_BOUNTY				14
#define MAX_TOP_BOUNTY_SHOWN		9

// OLD_RAND selects the legacy Mitchell-Moore generator in db.c.  The system
// prototypes it used to hand-declare now come from the real headers above.
#ifndef OLD_RAND
#define OLD_RAND
#endif

extern char *top_bounty_name[MAX_TOP_BOUNTY];
extern long top_bounty_value[MAX_TOP_BOUNTY];

extern HELP_DATA *help_first;
extern HELP_DATA *help_last;
extern SHOP_DATA *shop_first;
extern SHOP_DATA *shop_last;
extern char bug_buf[];
extern char *help_greeting;
extern char log_buf[];
// Owns every character in the world. A character is put here by create_mobile
// and by the login handing over at CON_READ_MOTD, and destroyed by extract_char
// erasing its node. room->people and ch->carrying are containment, and stay raw.
extern CharacterList char_list;
extern KILL_DATA kill_table[];
extern MOB_INDEX_DATA *mindex_list;
extern OBJ_INDEX_DATA *oIndex_list;
// Owns every object in the world. An object is put here by create_object (and by
// fread_obj for the legacy format) and destroyed by extract_obj erasing its
// node; obj_data::contains and ch->carrying are containment, and stay raw.
extern ObjectList object_list;
extern TIME_INFO_DATA time_info;
extern short sun;
extern short moon_berus;
extern short berus_pos;
extern short moon_calabren;
extern short calabren_pos;
extern ROOM_INDEX_DATA *room_list;
extern ROOM_INDEX_DATA *top_affected_room;
// Owns every applied rune. A rune is on two lists at once: this one, which
// decides when it expires and holds it alive, and a per-container chain off
// obj->rune / exit->rune / room->rune, which decides whether it triggers and is
// non-owning. Erasing from here is the destruction point, so extract_rune has
// to unlink the container chain strictly first.
extern std::list<std::unique_ptr<RUNE_DATA>> rune_list;
extern long gold_constant;
extern long total_gold;
extern long player_gold;
extern long total_wealth;
extern RACE_DATA * race_list;

//
// The skill numbers.  The names, and what each one is for, live in
// gsn_list.h so that the declarations here and the definitions in db.c come
// from one list and cannot drift apart.
//
#define GSN(name, state) extern short name;
#include "gsn_list.h"
#undef GSN


extern MOB_INDEX_DATA * mob_index_hash[MAX_KEY_HASH];
extern OBJ_INDEX_DATA * obj_index_hash[MAX_KEY_HASH];
extern ROOM_INDEX_DATA *room_index_hash[MAX_KEY_HASH];
extern AREA_DATA *area_last;
extern char *top_string;
extern char str_empty[1];
extern short count_data[30000];
extern int top_affect;
extern int top_area;
extern int top_ed; 
extern int top_exit;
extern int top_help;
extern int top_mob_index;
extern int top_obj_index;
extern int top_reset;
extern int top_room;
extern int top_shop;
extern int top_vnum_room;
extern int top_vnum_mob;
extern int top_vnum_obj;
extern int mobile_count;
extern int newmobs;
extern int newobjs;
extern bool fBootDb;
extern FILE * fpArea;
extern char strArea[MAX_INPUT_LENGTH];
extern int num_pfiles;

extern int nAllocString; 
extern int sAllocString;
extern int nAllocPerm;
extern int sAllocPerm;

extern short cabal_members[MAX_CABAL];
extern short cabal_max[MAX_CABAL];


// TODO: UNKNOWN FUNCTION
extern int _filbuf (FILE *);


// macro for flag swapping
#define GET_UNSET(flag1,flag2)		(~(flag1)&((flag1)|(flag2)))

// Magic number for memory allocation
#define MAGIC_NUM					52571214

//#define social_debug				0

//
// LOCAL FUNCTIONS
//

char *munch (char *str);
void update_db_gold(void);
void load_area (FILE * fp);
void sort_areas (void);
void new_load_area (FILE * fp);				/* OLC */
void assign_area_vnum (int vnum);			/* OLC */
/*
 * Adds a reset to a room.  OLC
 * Similar to add_reset in olc.c
 */
void new_reset(ROOM_INDEX_DATA *pR, RESET_DATA *pReset);
void load_resets (FILE * fp);
/*
 * Snarf a shop section.
 */
void load_shops(FILE *fp);
void load_cabal_items (void);
void load_bans (void);
void fix_exits (void);
void find_adjacents (void);
void area_update(void);
void reset_room (ROOM_INDEX_DATA *pRoom);	/* OLC */
void reset_area (AREA_DATA * pArea);		/* OLC */
CHAR_DATA *create_mobile (MOB_INDEX_DATA *pMobIndex);
void clone_mobile (CHAR_DATA *parent, CHAR_DATA *clone);
OBJ_DATA *create_object (OBJ_INDEX_DATA *pObjIndex, int level);
void clone_object (OBJ_DATA *parent, OBJ_DATA *clone);
char *get_extra_descr (const char *name, const std::list<EXTRA_DESCR_DATA> &eds);
MOB_INDEX_DATA *get_mob_index (int vnum);
OBJ_INDEX_DATA *get_obj_index(int vnum);
ROOM_INDEX_DATA *get_room_index (int vnum);
char fread_letter (FILE *fp);
int fread_number (FILE *fp);
long fread_flag (FILE *fp);
void fread_flag_new (long vector[], FILE *fp);
long vector_convert (char letter);
long flag_convert (char letter);
void fread_vector(long vector[], FILE *fp);
char *fread_string (FILE *fp);
char *fread_string_eol(FILE *fp);
void fread_to_eol (FILE *fp);
char *fread_word (FILE *fp);
/*
 * Allocate some ordinary memory,
 *   with the expectation of freeing it someday.
 */
void *iamlame(int sMem);
/*
 * Free some memory.
 * Recycle it back onto the free list for blocks of that size.
 */
void soami(void *pMem, int sMem);
/*
 * Allocate some permanent memory.
 * Permanent memory is never freed,
 *   pointers into it may be copied safely.
 */
void *crappyold(int sMem);
/*
 * Duplicate a string into dynamic memory.
 * Fread_strings are read-only and shared.
 */
char *iamnotereet(const char *str);
void do_areas(CHAR_DATA *ch, char *argument);
void do_memory(CHAR_DATA *ch, char *argument);
void do_dump(CHAR_DATA *ch, char *argument);
int number_fuzzy (int number);
int number_range (int from, int to);
int number_percent (void);
int number_door (void);
int number_bits (int width);
void init_mm (void);
long number_mm (void);
int dice (int number, int size);
int interpolate(int level, int value_00, int value_32);
void smash_tilde (char *str);
bool str_cmp (const char *astr, const char *bstr);
bool str_prefix (const char *astr, const char *bstr);
bool str_infix (const char *astr, const char *bstr);
bool str_suffix (const char *astr, const char *bstr);
char *capitalize (const char *str);
void append_file (CHAR_DATA *ch, const char *file, char *str);
void tail_chain(void);
void do_force_reset(CHAR_DATA *ch, char *argument);
/*
void do_alist(CHAR_DATA *ch,char *argument);
*/
void do_llimit(CHAR_DATA *ch, char *argument);
void load_rooms (FILE *fp);
void load_newresets (FILE * fp);
void clean_notes (void);
void load_race_info (void);
char intflag_to_char(int i);
char *bitvector_to_flag (long bitvect[]);

///
/// Reports every skill number that gsn_list.h says must be wired to a
/// skill_table row and that no row assigned.  Call once, after LoadGsn.
/// @note Replaces nothing.  The check did not exist: an unassigned gsn kept
///       the zero it was born with, which is a valid index into skill_table.
void check_gsn_registry();

#endif /* DB_H */
