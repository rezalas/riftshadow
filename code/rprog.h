#ifndef RPROG_H
#define RPROG_H

/* IMPROGS Expansion: ROOM PROGS (Eladrian) */

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "magic.h"
#include "interp.h"
#include "recycle.h"
#include "tables.h"
#include "room.h"
#include "rift.h"

DECLARE_RPROG_FUN_PULSE( pulse_prog_cimar_sewergrate );
DECLARE_RPROG_FUN_SPEECH( speech_prog_ilopheth_shack );
DECLARE_RPROG_FUN_PULSE(pulse_prog_cim_conv);
DECLARE_RPROG_FUN_ENTRY(entry_prog_ilopheth_flute);
DECLARE_RPROG_FUN_ENTRY(entry_prog_sidhe_ankle);
DECLARE_RPROG_FUN_OPEN(open_prog_mudschool_key);
DECLARE_RPROG_FUN_MOVE(move_prog_mudschool_key);
DECLARE_RPROG_FUN_MOVE(move_prog_door_close);
DECLARE_RPROG_FUN_PULSE(pulse_prog_mudschool_snake);
DECLARE_RPROG_FUN_PULSE(pulse_prog_elven_star);
DECLARE_RPROG_FUN_OPEN(open_prog_bust_room);
DECLARE_RPROG_FUN_OPEN(open_prog_nodoor);
DECLARE_RPROG_FUN_MOVE(move_prog_stone_roll);
DECLARE_RPROG_FUN_MOVE(move_prog_horde_shrine);
DECLARE_RPROG_FUN_ENTRY(entry_prog_iseldheim_lift);
DECLARE_RPROG_FUN_DROP(drop_prog_elven_star);
DECLARE_RPROG_FUN_SPEECH(speech_prog_elven_down);

bool rprog_unset(ROOM_INDEX_DATA *room, const char *progtype, const char *name);
void rprog_set(ROOM_INDEX_DATA *room, const char *progtype, const char *name);
void pulse_prog_cimar_sewergrate(ROOM_INDEX_DATA *room);
void pulse_prog_cim_conv(ROOM_INDEX_DATA *room);
void entry_prog_ilopheth_flute(ROOM_INDEX_DATA *room, CHAR_DATA *ch);
void entry_prog_sidhe_ankle(ROOM_INDEX_DATA *room, CHAR_DATA *ch);
bool open_prog_mudschool_key(ROOM_INDEX_DATA *room, CHAR_DATA *ch, EXIT_DATA *exit);
bool move_prog_mudschool_key(ROOM_INDEX_DATA *room, CHAR_DATA *ch, int dir);
bool move_prog_door_close(ROOM_INDEX_DATA *room, CHAR_DATA *ch, int dir);
void pulse_prog_mudschool_snake(ROOM_INDEX_DATA *room);
bool open_prog_bust_room(ROOM_INDEX_DATA *room, CHAR_DATA *ch, EXIT_DATA *exit);
bool open_prog_nodoor(ROOM_INDEX_DATA *room, CHAR_DATA *ch, EXIT_DATA *exit);
bool move_prog_stone_roll(ROOM_INDEX_DATA *room, CHAR_DATA *ch, int dir);
bool move_prog_horde_shrine(ROOM_INDEX_DATA *room, CHAR_DATA *ch, int dir);
void entry_prog_iseldheim_lift(ROOM_INDEX_DATA *room, CHAR_DATA *ch);
void drop_prog_elven_star(ROOM_INDEX_DATA *room, CHAR_DATA *ch, OBJ_DATA *obj);
void speech_prog_elven_down(ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *speech);
void rprog_elven_down_end (CHAR_DATA *ch, AFFECT_DATA *af);
void pulse_prog_elven_star(ROOM_INDEX_DATA *room);

#endif /* RPROG_H */
