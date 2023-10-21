#ifndef UTILITY_H
#define UTILITY_H

#include "merc.h"

#ifdef __cplusplus
extern "C" {
#endif

bool is_npc (CHAR_DATA *ch);

#ifdef __cplusplus
}
#endif

bool is_cabal_guard (CHAR_DATA *ch);
bool is_immortal (CHAR_DATA *ch);
bool is_hero (CHAR_DATA *ch);
bool is_heroimm (CHAR_DATA *ch);
bool is_trusted (CHAR_DATA *ch, int level);
bool is_affected_by (CHAR_DATA *ch, int sn);
// NOTE: function overloading fails using a plain C compiler, but since we're using a C++ compiler all is good :)
bool is_affected_by (OBJ_DATA *ch, int sn);

bool is_ansi (CHAR_DATA *ch);

bool is_good (CHAR_DATA *ch);
bool is_evil (CHAR_DATA *ch);
bool is_neutral (CHAR_DATA *ch);

bool is_lawful (CHAR_DATA *ch);
bool is_chaotic (CHAR_DATA *ch);
bool is_eneutral (CHAR_DATA *ch);

bool is_same_align (short one, short two);
bool is_opp_align (CHAR_DATA *ch, CHAR_DATA *vch);
bool is_num_good (short num);
bool is_num_evil (short num);
bool is_num_neutral (short num);

bool is_num_lawful (short num);
bool is_num_chaotic (short num);
bool is_num_eneutral (short num);

bool is_awake (CHAR_DATA *ch);
short get_ac (CHAR_DATA *ch, short type);
short get_hitroll (CHAR_DATA *ch);
short get_damroll (CHAR_DATA *ch);

bool is_outside (CHAR_DATA *ch);

long get_carry_weight (CHAR_DATA *ch);

bool is_shifted (CHAR_DATA *ch);
bool is_hungry (CHAR_DATA *ch);
bool is_thirsty (CHAR_DATA *ch);

bool is_room_affected (ROOM_INDEX_DATA *room, int sn);
bool is_ground (ROOM_INDEX_DATA *room);
bool is_water (ROOM_INDEX_DATA *room);

bool can_wear (OBJ_DATA *obj, int part);
bool is_obj_stat (OBJ_DATA *obj, int stat);
bool is_weapon_stat (OBJ_DATA *obj, int stat);
int weight_mult (OBJ_DATA *obj);
bool is_explore (ROOM_INDEX_DATA *room);
bool is_metal (OBJ_DATA *obj);

bool is_stone (OBJ_DATA *obj);
bool can_rust (OBJ_DATA *obj);

char* pers (CHAR_DATA *ch, CHAR_DATA *looker);

bool is_switched (CHAR_DATA *ch);


#endif /* UTILITY_H */
