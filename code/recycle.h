/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik Stærfeldt , Tom Madsen, and Katja Nyboe.  *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/
 
/****************************************************************************
 *  ROM 2.4 is copyright 1993-1996 Russ Taylor                              *
 *  ROM has been brought to you by the ROM consortium                       *
 *      Russ Taylor (rtaylor@efn.org)                                       *
 *      Gabrielle Taylor (gtaylor@pacinfo.com)                              *
 *      Brian Moore (zump@rom.org)                                          *
 *  By using this code, you have agreed to follow the terms of the          *
 *  ROM license, in the file Tartarus/doc/rom.license                       *
 ****************************************************************************/

/***************************************************************************
 *       Tartarus code is copyright (C) 1997-1998 by Daniel Graham         *
 *  In using this code you agree to comply with the Tartarus license       *
 *       found in the file /Tartarus/doc/tartarus.doc                      *
 ***************************************************************************/

#ifndef RECYCLE_H
#define RECYCLE_H

#ifdef macintosh
#include <types.h>
#include <time.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "merc.h"
#include "handler.h"
#include "newmem.h"

/* externs */
extern char str_empty[1];
extern int mobile_count;


/* stuff for providing a crash-proof buffer */

#define MAX_BUF  	16384
#define MAX_BUF_LIST 	10
#define BASE_BUF 	1024

/* valid states */
#define BUFFER_SAFE	0
#define BUFFER_OVERFLOW	1
#define BUFFER_FREED 	2

RUNE_DATA *new_rune args ( ( void ) );
void free_rune args( ( RUNE_DATA *rune) );

/* note recycling */
NOTE_DATA *new_note args( (void) );
void free_note args( (NOTE_DATA *note) );

/* ban data recycling */
BAN_DATA *new_ban args( (void) );
void free_ban args( (BAN_DATA *ban) );

/* descriptor recycling */
DESCRIPTOR_DATA *new_descriptor args( (void) );
void free_descriptor args( (DESCRIPTOR_DATA *d) );

/* char gen data recycling */
GEN_DATA *new_gen_data args( (void) );
void free_gen_data args( (GEN_DATA * gen) );

/* extra descr recycling */
EXTRA_DESCR_DATA *new_extra_descr args( (void) );
void free_extra_descr args( (EXTRA_DESCR_DATA *ed) );

/* trophy recycling */
TROPHY_DATA *new_trophy_data args( (char *victname) ) ;
void free_trophy args( ( TROPHY_DATA *trophy ) );

/* race_data recycling (never gonna happen!) */
RACE_DATA *new_race_data args( (void) );
void free_race_data args( ( RACE_DATA *race_specs ) );

/* tracks recycling */
TRACK_DATA *new_track_data args( (void) ) ;
void free_track args( ( TRACK_DATA *tracks ) );

/* pathfind recycling */
PATHFIND_DATA *new_path_data args( (void) );
void free_path args( ( PATHFIND_DATA *path) );

/* trap recycling */
TRAP_DATA *new_trap args( (void) );
void free_trap args( ( TRAP_DATA *trap) );

/* apply recycling */
OBJ_APPLY_DATA *new_apply_data args( (void) );
void free_apply args( ( OBJ_APPLY_DATA *apply) );

/* affect recycling */
AFFECT_DATA *new_affect args( (void) );
void free_affect args( (AFFECT_DATA *af) );
OBJ_AFFECT_DATA *new_affect_obj args ((void));
void free_affect_obj args ((OBJ_AFFECT_DATA *af));
AREA_AFFECT_DATA *new_affect_area args ((void));
void free_affect_area args ((AREA_AFFECT_DATA *af));
ROOM_AFFECT_DATA *new_affect_room(void);
void free_affect_room(ROOM_AFFECT_DATA *af);

/* object recycling */
OBJ_DATA *new_obj args( (void) );
void free_obj args( (OBJ_DATA *obj) );

/* character recyling */
CHAR_DATA *new_char args( (void) );
void free_char args( (CHAR_DATA *ch) );
PC_DATA *new_pcdata args( (void) );
void free_pcdata args( (PC_DATA *pcdata) );
OLD_CHAR *new_oldchar args((void));
void free_oldchar args( (OLD_CHAR *old) );

/* speech recycling */
SPEECH_DATA *new_speech_data args( (void) );
void free_speech args( (SPEECH_DATA *speech) );
void free_line args( (LINE_DATA *line) );
LINE_DATA *new_line_data args( (void) );

/* mob id and memory procedures */
long get_pc_id args( (void) );
long get_mob_id args( (void) );
MEM_DATA *new_mem_data args( (void) );
void free_mem_data args( ( MEM_DATA *memory) );
MEM_DATA *find_memory args( (MEM_DATA *memory, long id) );

/* buffer procedures */
BUFFER *new_buf args( (void) );
void free_buf args( (BUFFER *buffer) );
bool add_buf args( (BUFFER *buffer, char *string) );
void clear_buf args( (BUFFER *buffer) );
char *buf_string args( (BUFFER *buffer) );

#endif /* RECYCLE_H */
