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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>
#include <iterator>
#include <algorithm>
#include "merc.h"
#include "db2.h"
#include "recycle.h"
#include "lookup.h"
#include "tables.h"
#include "update.h"
#include "interp.h"
#include "olc.h"
#include "spec.h"
#include "act_comm.h"
#include "newmem.h"
#include "comm.h"
#include "act_wiz.h"
#include "devextra.h"
#include "magic.h"
#include "handler.h"
#include "misc.h"
#include "dioextra.h"
#include "chardef.h"
#include "const.h"
#include "./include/fmt/format.h"
#include "./include/fmt/printf.h"

#ifndef OLD_RAND
#define OLD_RAND
	void srandom(unsigned int);
	int getpid();
	time_t time(time_t *tloc);
#endif


/* externals for counting purposes */
extern OBJ_DATA *obj_free;
extern CHAR_DATA *char_free;
extern DESCRIPTOR_DATA *descriptor_free;
extern PC_DATA *pcdata_free;
extern AFFECT_DATA *affect_free;

extern bool fBootDb;
extern int newmobs;
extern int newobjs;
extern MOB_INDEX_DATA * mob_index_hash[MAX_KEY_HASH];
extern OBJ_INDEX_DATA * obj_index_hash[MAX_KEY_HASH];
extern int top_mob_index;
extern int top_obj_index;
extern int top_affect;
extern int top_ed; 
extern AREA_DATA * area_first;
extern NOTE_DATA *note_free;
extern AREA_DATA *area_first;


/* from db2.c */
extern int social_count;

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
void fix_exits (void);
void find_adjacents (void);
void area_update(void);
void reset_room (ROOM_INDEX_DATA *pRoom);	/* OLC */
void reset_area (AREA_DATA * pArea);		/* OLC */
CHAR_DATA *create_mobile (MOB_INDEX_DATA *pMobIndex);
void clone_mobile (CHAR_DATA *parent, CHAR_DATA *clone);
OBJ_DATA *create_object (OBJ_INDEX_DATA *pObjIndex, int level);
void clone_object (OBJ_DATA *parent, OBJ_DATA *clone);
void clear_char(CHAR_DATA *ch);
char *get_extra_descr (const char *name, EXTRA_DESCR_DATA *ed);
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
void append_file (CHAR_DATA *ch, char *file, char *str);
/*
 * Reports a bug.
 */
void bug(const char *str, int param);
void log_string (const char *str);
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

#endif /* DB_H */
