#ifndef HANDLER_H
#define HANDLER_H

#include "merc.h"

//
// TODO: UNKNOWN FUNCTIONS
//
// void object_modify (OBJ_DATA *obj, AFFECT_DATA *paf, bool fAdd);


//
// LOCAL FUNCTIONS
//


/* friend stuff -- for NPC's mostly */
bool is_friend (CHAR_DATA *ch, CHAR_DATA *victim);
/* returns number of people on an object */
int count_users (OBJ_DATA *obj);
/* returns race number */
int race_lookup (const char *name);
int act_lookup (const char *name);
int liq_lookup (const char *name);
int weapon_lookup (const char *name);
int weapon_type_lookup (const char *name);
char *weapon_name_lookup (int type, char* default_name = "weapon");
int item_lookup (const char *name);
char *item_name_lookup (int item_type);
int attack_lookup  (const char *name);
/* returns a flag for wiznet */
long wiznet_lookup (const char *name);
char *color_value_string (int color, bool bold, bool flash);
int strlen_color (char *argument);
/* for immunity, vulnerabiltiy, and resistant
   the 'globals' (magic and weapons) may be overriden
   three other cases -- wood, silver, and iron -- are checked in fight.c */
int check_immune (CHAR_DATA *ch, int dam_type);
bool is_cabal (CHAR_DATA *ch);
bool is_same_cabal (CHAR_DATA *ch, CHAR_DATA *victim);
/* checks mob format */
bool is_old_mob (CHAR_DATA *ch);
/* for returning skill information */
int get_skill (CHAR_DATA *ch, int sn);
/* for returning weapon information */
int get_weapon_sn (CHAR_DATA *ch);
int get_weapon_sn_new (CHAR_DATA *ch, int type);
int get_weapon_skill (CHAR_DATA *ch, int sn);
/* used to de-screw characters */
void reset_char (CHAR_DATA *ch);
/*
 * Retrieve a character's trusted level for permission checking.
 */
int get_trust (CHAR_DATA *ch);
/*
 * Retrieve a character's age.
 */
/* command for retrieving stats */
int get_curr_stat (CHAR_DATA *ch, int stat);
/* command for returning max training score */
int get_max_train (CHAR_DATA *ch, int stat);
/*
 * Retrieve a character's carry capacity.
 */
int can_carry_n (CHAR_DATA *ch);
/*
 * Retrieve a character's carry capacity.
 */
int can_carry_w (CHAR_DATA *ch);
/* Calculate if players are in pk range to attack each other */
bool can_pk (CHAR_DATA *ch, CHAR_DATA *victim);
/*
 * See if a string is one of the names of an object.
 */

bool is_name (const char *str, const char *namelist);
bool is_exact_name (char *str, char *namelist);
void init_affect (AFFECT_DATA *paf);
/*
 * Apply or remove an affect to a character.
 */
void affect_modify (CHAR_DATA *ch, AFFECT_DATA *paf, bool fAdd);
/* find an effect in an affect list */
AFFECT_DATA *affect_find (AFFECT_DATA *paf, int sn);
OBJ_AFFECT_DATA *affect_find_obj (OBJ_AFFECT_DATA *paf, int sn);
ROOM_AFFECT_DATA *affect_find_room (ROOM_AFFECT_DATA *paf, int sn);
AREA_AFFECT_DATA *affect_find_area (AREA_AFFECT_DATA *paf, int sn);
/* fix object affects when removing one */
void affect_check (CHAR_DATA *ch, int where, long vector[]);
/*
 * Give an affect to a char.
 */
void affect_to_char (CHAR_DATA *ch, AFFECT_DATA *paf);
void new_affect_to_char (CHAR_DATA *ch, AFFECT_DATA *paf);
/*
 * Remove an affect from a char.
 */
void affect_remove (CHAR_DATA *ch, AFFECT_DATA *paf);
/*
 * Strip all affects of a given sn.
 */
void affect_strip (CHAR_DATA *ch, int sn);
/*
 * Return true if a char is affected by a spell.
 */
bool is_affected (CHAR_DATA *ch, int sn);
/*
bool is_affected_obj (OBJ_DATA *obj, int sn);
*/
/*
 * Add or enhance an affect.
 */

void affect_join (CHAR_DATA *ch, AFFECT_DATA *paf);
void new_affect_join (CHAR_DATA *ch, AFFECT_DATA *paf);
/*
 * Move a char out of a room.
 */
void char_from_room (CHAR_DATA *ch);
/*
 * Move a char into a room.
 */
void char_to_room (CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex);
/*
 * Give an obj to a char.
 */
void obj_to_char (OBJ_DATA *obj, CHAR_DATA *ch);
/*
 * Take an obj from its character.
 */
void obj_from_char (OBJ_DATA *obj);
/*
 * Find the ac value of an obj, including position effect.
 */
int apply_ac (OBJ_DATA *obj, int iWear, int type);
/*
 * Find a piece of eq on a character.
 */
OBJ_DATA *get_eq_char (CHAR_DATA *ch, int iWear);
bool is_worn (OBJ_DATA *obj);
/*
 * Equip a char with an obj.
 */
void equip_char (CHAR_DATA *ch, OBJ_DATA *obj, int iWear, bool show);
/*
 * Unequip a char with an obj.
 */
void unequip_char (CHAR_DATA *ch, OBJ_DATA *obj, bool show);
/*
 * Count occurrences of an obj in a list.
 */
int count_obj_list (OBJ_INDEX_DATA *pObjIndex, OBJ_DATA *list);
/*
 * Move an obj out of a room.
 */
void obj_from_room (OBJ_DATA *obj);
/*
 * Move an obj into a room.
 */
void obj_to_room (OBJ_DATA *obj, ROOM_INDEX_DATA *pRoomIndex);
/*
 * Move an object into an object.
 */
void obj_to_obj (OBJ_DATA *obj, OBJ_DATA *obj_to);
/*
 * Move an object out of an object.
 */
void obj_from_obj (OBJ_DATA *obj);
/*
 * Extract an obj from the world.
 */
void extract_obj (OBJ_DATA *obj);
void delay_extract (CHAR_DATA *ch);
/*
 * Extract a char from the world.
 */
void extract_char (CHAR_DATA *ch, bool fPull);
/*
 * Find a char in the room.
 */
/* SEE act_info.c for new one that handles morph
CHAR_DATA *get_char_room (CHAR_DATA *ch, char *argument);
*/
/*
 * Find a char in the world.
 */
CHAR_DATA *get_char_world (CHAR_DATA *ch, char *argument);
/*
 * Find some object with a given index data.
 * Used by area-reset 'P' command.
 */
OBJ_DATA *get_obj_type (OBJ_INDEX_DATA *pObjIndex);
/*
 * Find an obj in a list.
 */
OBJ_DATA *get_obj_list (CHAR_DATA *ch, char *argument, OBJ_DATA *list);
/*
 * Find an obj in a room.
 */
OBJ_DATA *get_obj_room (OBJ_DATA *obj, ROOM_INDEX_DATA *room);
/*
 * Find an obj in player's inventory.
 */
OBJ_DATA *get_obj_carry (CHAR_DATA *ch, char *argument, CHAR_DATA *viewer);
/*
 * Find an obj in player's equipment.
 */
OBJ_DATA *get_obj_wear (CHAR_DATA *ch, char *argument);
/*
 * Find an obj in the room or in inventory.
 */
OBJ_DATA *get_obj_here (CHAR_DATA *ch, char *argument);
/*
 * Find an obj in the world.
 */
OBJ_DATA *get_obj_world (CHAR_DATA *ch, char *argument);
/* deduct cost from a character */
void deduct_cost (CHAR_DATA *ch, int cost);
void deduct_cost(CHAR_DATA *ch, long cost);
/*
 * Create a 'money' obj.
 */
OBJ_DATA *create_money (int gold);
/*
 * Return # of objects which an object counts as.
 * Thanks to Tony Chamberlain for the correct recursive code here.
 */
int get_obj_number (OBJ_DATA *obj);
/*
 * Return weight of an object, including weight of contents.
 */
int get_obj_weight (OBJ_DATA *obj);
int get_true_weight (OBJ_DATA *obj);
int get_contents_weight (OBJ_DATA *obj);
int get_contents_number (OBJ_DATA *obj);
/*
 * True if room is dark.
 */
bool room_is_dark (ROOM_INDEX_DATA *pRoomIndex);
bool is_room_owner (CHAR_DATA *ch, ROOM_INDEX_DATA *room);
/*
 * True if room is private.
 */
bool room_is_private (ROOM_INDEX_DATA *pRoomIndex);
/* visibility on a room -- for entering and exits */
bool can_see_room (CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex);
/*
 * True if char can see victim.
 */
bool can_see (CHAR_DATA *ch, CHAR_DATA *victim);
/*
 * True if char can see obj.
 */
bool can_see_obj (CHAR_DATA *ch, OBJ_DATA *obj);
/*
 * True if char can drop obj.
 */
bool can_drop_obj (CHAR_DATA *ch, OBJ_DATA *obj);
/*
 * Return ascii name of an affect location.
 */
char *affect_loc_name (int location);
/*
 * Return ascii name of an affect bit vector.
 */
char *affect_bit_name (long vector[]);
/*
 * Return ascii name of extra flags vector.
 */
char *extra_bit_name (long extra_flags[]);
/* Return ascii name for restrict flags */
char * restrict_bit_name (long restrict_flags[]);
/* return ascii name of an act vector */
char *act_bit_name (long act_flags[]);
char *comm_bit_name (long comm_flags[]);
char *imm_bit_name (long imm_flags[]);
char *wear_bit_name (long wear_flags[]);
char *form_bit_name (long form_flags[]);
char *part_bit_name (long part_flags[]);
char *weapon_bit_name (int weapon_flags);
char *cont_bit_name (int cont_flags);
char *off_bit_name (long off_flags[]);
/* Room Affects */
/*
 * Apply or remove an affect to a room.
 */

void init_affect_room (ROOM_AFFECT_DATA *paf);
void affect_modify_room (ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *paf, bool fAdd);
/*
 * Give an affect to a room.
 */
void affect_to_room (ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *paf);
void new_affect_to_room (ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *paf);
void affect_check_room (ROOM_INDEX_DATA *room,int where, long vector[]);
/*
 * Remove an affect from a room.
 */
void affect_remove_room (ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *paf);
/*
 * Strip all affects of a given sn.
 */
void affect_strip_room (ROOM_INDEX_DATA *room, int sn);
/*
 * Return true if a room is affected by a spell.
 */
bool is_affected_room (ROOM_INDEX_DATA *room, int sn);
/*
 * Add or enhance an affect.
 */
void affect_join_room (ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *paf);
/*
 * Return ascii name of an raffect location.
 */
char *raffect_loc_name (int location);
/*
 * Return ascii name of an affect bit vector.
 */
char *raffect_bit_name (long vector[]);
void charaff_to_obj (OBJ_DATA *obj, AFFECT_DATA *paf);
void charaff_to_obj_index (OBJ_INDEX_DATA *obj, AFFECT_DATA *paf);
void charaff_from_obj_index (OBJ_INDEX_DATA *obj, AFFECT_DATA *paf);
/*
 * Apply or remove an affect to an object.
 */
void init_affect_obj (OBJ_AFFECT_DATA *paf);
void affect_modify_obj (OBJ_DATA *obj, OBJ_AFFECT_DATA *paf, bool fAdd);
void affect_to_obj (OBJ_DATA *obj, OBJ_AFFECT_DATA *paf);
void affect_check_obj (OBJ_DATA *obj, int where, long vector[]);
void affect_remove_obj (OBJ_DATA *obj, OBJ_AFFECT_DATA *paf, bool show);
void affect_strip_obj (OBJ_DATA *obj, int sn);
bool is_affected_obj (OBJ_DATA *obj, int sn);
void affect_join_obj (OBJ_DATA *obj, OBJ_AFFECT_DATA *paf);
char *oaffect_loc_name (int location);
char *oaffect_bit_name (long vector[]);
/*
 * Apply or remove an affect to an area.
 */
void init_affect_area (AREA_AFFECT_DATA *paf);
void affect_modify_area (AREA_DATA *area, AREA_AFFECT_DATA *paf, bool fAdd);
void affect_to_area (AREA_DATA *area, AREA_AFFECT_DATA *paf);
void affect_check_area (AREA_DATA *area, int where, long vector[]);
void affect_remove_area (AREA_DATA *area, AREA_AFFECT_DATA *paf);
void affect_strip_area (AREA_DATA *area, int sn);
bool is_affected_area (AREA_DATA *area, int sn);
void affect_join_area (AREA_DATA *area, AREA_AFFECT_DATA *paf);
char *aaffect_loc_name (int location);
bool is_safe_rspell_nom (int level, CHAR_DATA *victim);
bool is_safe_rspell (int level, CHAR_DATA *victim);
/*
 * Return ascii name of an affect bit vector.
 */
char *flag_room_name (int vector);
void modify_location (CHAR_DATA *ch, int location, int mod, bool add);
int get_align (CHAR_DATA *ch);
int get_ethos (CHAR_DATA *ch);
int damage_queue (CHAR_DATA *ch, CHAR_DATA *victim, int dam, int damtype, bool blockable, int add, int mult, char *dnoun);

#endif /* HANDLER_H */
