#ifndef RPROG_H
#define RPROG_H

/* IMPROGS Expansion: ROOM PROGS (Eladrian) */

#include "merc.h"

extern const struct improg_type rprog_table[];

//
// LOCAL FUNCTIONS
//

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
