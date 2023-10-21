#ifndef QUEST_H
#define QUEST_H

/* Eladrian's Kickin' Quest Tracking System -- ph34r */

#include "merc.h"


extern const struct quest_type quest_table[];

//
// LOCAL FUNCTIONS
//

void do_clearquests(CHAR_DATA *ch, char *argument);
int agrees(CHAR_DATA *ch, char *speech);
bool can_do_quest(CHAR_DATA *ch, int quest);
void store_quest_val(CHAR_DATA *ch, char *valname, short value);
void setbit_quest_val(CHAR_DATA *ch, char *valname, long value);
int get_quest_val(CHAR_DATA *ch, char *valname);
void delete_quest_val(CHAR_DATA *ch, char *valname);

/* QUEST #1 -- Return the doll (2298) to the red-eyed girl (2481) in Cimar */
void greet_prog_cimar_dollgirl(CHAR_DATA *mob, CHAR_DATA *ch);
void speech_prog_cimar_dollgirl(CHAR_DATA *mob, CHAR_DATA *ch, char *speech);
void give_prog_cimar_dollgirl(CHAR_DATA *mob, CHAR_DATA *ch, OBJ_DATA *obj);

/* QUEST #2 -- Talismanic aura quest for sorcerers, at level 35+ */
void greet_prog_cimar_sorcgm(CHAR_DATA *mob, CHAR_DATA *ch);
void speech_prog_ilopheth_shack(ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *speech);
void speech_prog_ilopheth_hermit(CHAR_DATA *mob, CHAR_DATA *ch, char *speech);
void give_prog_ilopheth_hermit(CHAR_DATA *mob, CHAR_DATA *ch, OBJ_DATA *obj);
void talismanic_reward(CHAR_DATA *ch);
void pulse_prog_talismanic_page(OBJ_DATA *obj, bool isTick);
void pulse_prog_talismanic_scrap(OBJ_DATA *obj, bool isTick);
bool aggress_prog_ilopheth_hermit(CHAR_DATA *mob, CHAR_DATA *attacker);
void pulse_prog_ilopheth_hermit(CHAR_DATA *mob);

/* QUEST 3 - Bring the drow scribe (23811) fruit from island in the Crimson Sea */
void greet_prog_drow_scribe(CHAR_DATA *mob, CHAR_DATA *ch);
void speech_prog_drow_scribe(CHAR_DATA *mob, CHAR_DATA *ch, char *speech);
void give_prog_drow_scribe(CHAR_DATA *mob, CHAR_DATA *ch, OBJ_DATA *obj);
void greet_prog_opening_greet(CHAR_DATA *mob, CHAR_DATA *ch);
void greet_prog_headmaster(CHAR_DATA *mob, CHAR_DATA *ch);
void greet_prog_pay_char(CHAR_DATA *mob, CHAR_DATA *ch);
void greet_prog_starvin_pete(CHAR_DATA *mob, CHAR_DATA *ch);
void give_prog_starvin_pete(CHAR_DATA *mob, CHAR_DATA *ch, OBJ_DATA *obj);

/* academy blacksmith quest - bring him components for a better weapon -cal */
int mspec_academy_smith (long event_vector, ...);
int mspec_scared_soldier (long event_vector, ...);

#endif /* QUEST_H */