#ifndef QUEST_H
#define QUEST_H

/* Eladrian's Kickin' Quest Tracking System -- ph34r */

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "magic.h"
#include "recycle.h"
#include "db.h"
#include "lookup.h"
#include "tables.h"
#include "interp.h"
#include "spec.h"
#include "mprog.h"
#include "act_obj.h"

#define STAGE(ch)		ch->pcdata->quests[nQuestIndex]
#define SET_STAGE(ch, i) ch->pcdata->quests[nQuestIndex] = i

#define SMITH_QUEST_LASTCOMP "Smith Quest Last Component"
#define SMITH_QUEST_COMP_ACQUIRED "Smith Quest Components Used"


void do_clearquests(CHAR_DATA *ch, char *argument);
int agrees(CHAR_DATA *ch, char *speech);
bool can_do_quest(CHAR_DATA *ch, int quest);
void store_quest_val(CHAR_DATA *ch, char *valname, sh_int value);
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