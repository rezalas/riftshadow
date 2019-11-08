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

/* vals from db.c */

#ifndef DB_H
#define DB_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <ctype.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>
/* #include <sys/resource.h> */
#include "merc.h"
#include "db.h"
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

// TODO: currently in db2.c - make db2.h
extern void bugout (char *reason);



#ifndef OLD_RAND
// long random();
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

/* conversion from db.h */
void convert_mob(MOB_INDEX_DATA *mob);
void convert_obj(OBJ_INDEX_DATA *obj);

/* macro for flag swapping */
#define GET_UNSET(flag1,flag2) (~(flag1)&((flag1)|(flag2)))

/* Magic number for memory allocation */
#define MAGIC_NUM 52571214

/*
 * OLC
 * Use these macros to load any new area formats that you choose to
 * support on your MUD.  See the new_load_area format below for
 * a short example.
 */
#ifndef KEY
#define KEY(literal, field, value)	\
	if (!str_cmp(word, literal))	\
	{								\
		field = value;				\
		fMatch = true;				\
		break;						\
	}
#endif

#ifndef SKEY
#define SKEY(string, field)			\
	if (!str_cmp(word, string))		\
	{								\
		free_pstring(field);		\
		field = fread_string(fp);	\
		fMatch = true;				\
		break;						\
	}
#endif

//#define social_debug 0

char *display_name_lookup args((long bitv, const struct display_type *flag_table));
char *restrict_name_lookup args((long bitv));
int sex_lookup args((const char *name));


/* func from db.c */
extern void assign_area_vnum( int vnum );                    /* OLC */

char *flag_name_lookup args((long bitv, const struct flag_type *flag_table));
int flag_lookup args((const char *name, const struct flag_type *flag_table));
char *bitvector_to_flag args((long bitvect[]));
int flag_index_ilookup args((int i, const struct flag_type *flag_table));
void reset_chessboard(void);
void sort_areas(void);
void load_race_info(void);

/*
 * Local booting procedures.
 */
void init_mm args((void));
void load_area args((FILE * fp));
void new_load_area args((FILE * fp)); /* OLC */
void load_mobs args((FILE * fp));
void load_objs args((FILE * fp));
void load_resets args((FILE * fp));
void load_newresets args((FILE * fp));
void load_rooms args((FILE * fp));
void load_socials args((FILE * fp));
void load_votes args((void));
void load_cabal_items args((void));
void fix_exits args((void));
void find_adjacents args((void));
void clean_notes args((void));
void load_improgs args((FILE * fp));
void load_specs args((FILE * fp));
void reset_area args((AREA_DATA * pArea));

void area_update(void);
void update_db_gold(void);
void clone_object (OBJ_DATA *parent, OBJ_DATA *clone);
void reset_room (ROOM_INDEX_DATA *pRoom);
char *munch (char *str);

int number_door (void);
void tail_chain(void);
long number_mm (void);
int number_bits (int width);
char fread_letter (FILE *fp);
int fread_number (FILE *fp);
long fread_flag (FILE *fp);
void fread_flag_new (long vector[], FILE *fp);
long vector_convert (char letter);
char *fread_string (FILE *fp);
void fread_to_eol (FILE *fp);
char *fread_word (FILE *fp);
long flag_convert (char letter);
void append_file (CHAR_DATA *ch, char *file, char *str);
void clone_mobile (CHAR_DATA *parent, CHAR_DATA *clone);
void log_string (const char *str);
int number_fuzzy (int number);
bool str_infix (const char *astr, const char *bstr);
ROOM_INDEX_DATA *get_room_index(int vnum);
MOB_INDEX_DATA *get_mob_index (int vnum);
ROOM_INDEX_DATA *get_room_index (int vnum);
int dice (int number, int size);
CHAR_DATA *create_mobile (MOB_INDEX_DATA *pMobIndex);
bool str_cmp (const char *astr, const char *bstr);
bool str_prefix (const char *astr, const char *bstr);
bool str_suffix (const char *astr, const char *bstr);
char *capitalize (const char *str);
int number_percent (void);
int number_range (int from, int to);
void smash_tilde (char *str);
OBJ_INDEX_DATA *get_obj_index(int vnum);
OBJ_DATA *create_object (OBJ_INDEX_DATA *pObjIndex, int level);
char *get_extra_descr (const char *name, EXTRA_DESCR_DATA *ed);

#endif /* DB_H */
