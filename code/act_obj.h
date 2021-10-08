#ifndef ACT_OBJ_H
#define ACT_OBJ_H

#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <algorithm>
#include "merc.h"
#include "handler.h"
#include "magic.h"
#include "tables.h"
#include "spec.h"
#include "act_comm.h"
#include "cabal.h"
#include "devextra.h"
#include "dioextra.h"
#include "paladin.h"
#include "fight.h"
#include "newmem.h"
#include "comm.h"
#include "act_wiz.h"
#include "update.h"
#include "interp.h"
#include "db.h"
#include "act_move.h"
#include "chardef.h"
#include "const.h"
#include "material.h"
#include "utility.h"

#define CABAL_ITEMS_FILE	"citems.txt"
#define MINOTAUR_ONLY		ASCII_I


//
// TODO: UNKNOWN FUNCTIONS
//

/* A pile of obj progs for use in legion.are, and the drannor areas.
Remove these if you don't want to use those areas
*/
// void obj_give_hydra_head (CHAR_DATA *ch, CHAR_DATA *lady, OBJ_DATA *obj);
// void wear_obj_zhentil_robe (CHAR_DATA *ch, OBJ_DATA *obj);
// void wear_obj_symbol_magic (CHAR_DATA *ch, OBJ_DATA *obj);
// void remove_obj_symbol_magic (CHAR_DATA *ch, OBJ_DATA *obj);
// void recite_libram_conjuration (CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *scroll, OBJ_DATA *obj, bool fRead);
// void brandish_sceptre_dead (CHAR_DATA *ch, OBJ_DATA *obj);
// void wear_obj_misty_cloak (CHAR_DATA *ch, OBJ_DATA *obj);
// void remove_obj_misty_cloak (CHAR_DATA *ch, OBJ_DATA *obj);


//
// LOCAL FUNCTIONS
//

/* RT part of the corpse looting code */
bool check_arms (CHAR_DATA *ch, OBJ_DATA *obj);
bool can_loot (CHAR_DATA *ch, OBJ_DATA *obj);
void get_obj (CHAR_DATA *ch, OBJ_DATA *obj, OBJ_DATA *container, bool pcheck);
void do_get (CHAR_DATA *ch, char *argument);
void do_put (CHAR_DATA *ch, char *argument);
void do_drop (CHAR_DATA *ch, char *argument);
void do_give (CHAR_DATA *ch, char *argument);
void do_skin (CHAR_DATA *ch, char *argument);
void do_butcher (CHAR_DATA *ch, char *argument);
/* for poisoning weapons and food/drink */
void do_envenom (CHAR_DATA *ch, char *argument);
void do_fill (CHAR_DATA *ch, char *argument);
/* new pour - dio */
void do_pour (CHAR_DATA *ch, char *argument);
void do_drink (CHAR_DATA *ch, char *argument);
void do_eat (CHAR_DATA *ch, char *argument);
/*
 * Remove an object.
 */
bool remove_obj (CHAR_DATA *ch, int iWear, bool fReplace);
/*
 * Wear one object.
 * Optional replacement of existing objects.
 * Big repetitive code, ick.
 */
void wear_obj (CHAR_DATA *ch, OBJ_DATA *obj, bool fReplace);
void do_wear (CHAR_DATA *ch, char *argument);
void do_remove (CHAR_DATA *ch, char *argument);
void do_sacrifice (CHAR_DATA *ch, char *argument);
void quaff_potion(CHAR_DATA *arg1, int arg2, int arg3, int arg4, int arg5);
void do_quaff (CHAR_DATA *ch, char *argument);
void do_recite (CHAR_DATA *ch, char *argument);
void do_brandish (CHAR_DATA *ch, char *argument);
void do_zap (CHAR_DATA *ch, char *argument);
void do_steal (CHAR_DATA *ch, char *argument);
/*
 * Shopping commands.
 */
CHAR_DATA *find_keeper (CHAR_DATA *ch);
/* insert an object at the right spot for the keeper */
void obj_to_keeper (OBJ_DATA *obj, CHAR_DATA *ch);
/* get an object from a shopkeeper's list */
OBJ_DATA *get_obj_keeper (CHAR_DATA *ch, CHAR_DATA *keeper, char *argument);
int get_cost (CHAR_DATA *keeper, OBJ_DATA *obj, bool fBuy);
void do_buy (CHAR_DATA *ch, char *argument);
void do_list (CHAR_DATA *ch, char *argument);
void do_sell (CHAR_DATA *ch, char *argument);
void do_value (CHAR_DATA *ch, char *argument);
void do_request (CHAR_DATA *ch, char *argument);
bool hands_full (CHAR_DATA *ch);
void do_embalm (CHAR_DATA *ch, char *argument);
void cabal_shudder (int cabal, bool itemloss);
bool cabal_down (CHAR_DATA *ch, int cabal);
bool cabal_down_new (CHAR_DATA *ch, int cabal, bool show);
bool cant_carry (CHAR_DATA *ch, OBJ_DATA *obj);
bool is_restricted (CHAR_DATA *ch, OBJ_DATA *obj);
void do_demand (CHAR_DATA *ch, char *argument);
void reslot_weapon (CHAR_DATA *ch);
void report_weapon_skill (CHAR_DATA *ch, OBJ_DATA *obj);
void wear_obj_fallen_wings (CHAR_DATA *ch, OBJ_DATA *obj);
void remove_obj_fallen_wings (CHAR_DATA *ch, OBJ_DATA *obj);
void save_cabal_items (void);
bool is_owner (CHAR_DATA *ch, OBJ_DATA *corpse);
void do_roll (CHAR_DATA *ch, char *argument);
void do_flip (CHAR_DATA *ch, char *argument);
bool is_carrying (CHAR_DATA *ch, int vnum);
bool is_carrying_type (CHAR_DATA *ch, int type);

#endif /* ACT_OBJ_H */
