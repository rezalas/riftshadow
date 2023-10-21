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

#include "merc.h"

/* externals for counting purposes */
extern DESCRIPTOR_DATA *descriptor_free;
extern EXTRA_DESCR_DATA	*extra_descr_free;
extern AFFECT_DATA *affect_free;
extern OBJ_DATA *obj_free;
extern CHAR_DATA *char_free;
extern PC_DATA *pcdata_free;


/* stuff for providing a crash-proof buffer */

#define MAX_BUF					16384
#define MAX_BUF_LIST			10
#define BASE_BUF				1024

/* valid states */
#define BUFFER_SAFE				0
#define BUFFER_OVERFLOW			1
#define BUFFER_FREED			2

//
// LOCAL FUNCTIONS
//

/* descriptor recycling */
DESCRIPTOR_DATA *new_descriptor(void);
void free_descriptor(DESCRIPTOR_DATA *d);
/* char gen data recycling */
GEN_DATA *new_gen_data(void);
void free_gen_data(GEN_DATA * gen);
/* trophy recycling */
TROPHY_DATA *new_trophy_data(char *victname);
void free_trophy(TROPHY_DATA *trophy);
/* speech recycling */
SPEECH_DATA *new_speech_data(void);
void free_speech(SPEECH_DATA *speech);
void free_line(LINE_DATA *line);
LINE_DATA *new_line_data(void);
IPROG_DATA *new_iprog(void);
/* race_data recycling (never gonna happen!) */
RACE_DATA *new_race_data(void);
void free_race_data(RACE_DATA *race_specs);
/* tracks recycling */
TRACK_DATA *new_track_data(void);
void free_track(TRACK_DATA *tracks);
/* pathfind recycling */
PATHFIND_DATA *new_path_data(void);
void free_path(PATHFIND_DATA *path);
/* extra descr recycling */
EXTRA_DESCR_DATA *new_extra_descr(void);
void free_extra_descr(EXTRA_DESCR_DATA *ed);
/* apply recycling */
OBJ_APPLY_DATA *new_apply_data(void);
void free_apply(OBJ_APPLY_DATA *apply);
/* stuff for recycling affects */
AFFECT_DATA *new_affect(void);
TRAP_DATA *new_trap(void);
void free_trap(TRAP_DATA *trap);
RUNE_DATA *new_rune(void);
void free_rune(RUNE_DATA *rune);
QUEUE_DATA *new_queue(void);
void free_queue(QUEUE_DATA *queue);
void free_affect(AFFECT_DATA *af);
ROOM_AFFECT_DATA *new_affect_room(void);
void free_affect_room(ROOM_AFFECT_DATA *af);
OBJ_AFFECT_DATA *new_affect_obj(void);
void free_affect_obj(OBJ_AFFECT_DATA *af);
AREA_AFFECT_DATA *new_affect_area(void);
void free_affect_area(AREA_AFFECT_DATA *af);
/* object recycling */
OBJ_DATA *new_obj(void);
void free_obj(OBJ_DATA *obj);
/* character recycling */
CHAR_DATA *new_char(void);
void free_char(CHAR_DATA *ch);
PC_DATA *new_pcdata(void);
OLD_CHAR *new_oldchar(void);
void free_oldchar(OLD_CHAR *old);
void free_pcdata(PC_DATA *pcdata);
/* mob id and memory procedures */
long get_pc_id(void);
long get_mob_id(void);
MEM_DATA *new_mem_data(void);
void free_pstruct_data(MEM_DATA *memory);
/* local procedure for finding the next acceptable size */
/* -1 indicates out-of-boundary error */
int get_size(int val);
/* buffer procedures */
BUFFER *new_buf(void);
void free_buf(BUFFER *buffer);
bool add_buf(BUFFER *buffer, char *string);
void clear_buf(BUFFER *buffer);
char *buf_string(BUFFER *buffer);

#endif /* RECYCLE_H */
