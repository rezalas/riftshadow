/*
 * Stores all the functions for necromancer spells and handling of blood
 * charges, urns, and so forth.  Set up by Eladrian, 3/5/01.
 */

#ifndef NECRO_H
#define NECRO_H

#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <algorithm>
#include "merc.h"
#include "necro.h"
#include "recycle.h"
#include "tables.h"
#include "lookup.h"
#include "magic.h"
#include "interp.h"
#include "fight.h"
#include "db.h"
#include "misc.h"
#include "utility.h"

//
// LOCAL FUNCTIONS
//

void spell_dark_vessel (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void make_urn (CHAR_DATA *ch, OBJ_DATA *corpse);
OBJ_DATA *find_urn (CHAR_DATA *ch);
bool drain_urn (CHAR_DATA *ch, int charges);
void power_urn (CHAR_DATA *ch, int charges);
void spell_siphon (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_hex (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_animate_dead (int sn, int level, CHAR_DATA *ch, void *vo, int target );
void animate_two (CHAR_DATA *ch, OBJ_DATA *corpse);
void animate_three (CHAR_DATA *ch, OBJ_DATA *corpse);
void animate_four (CHAR_DATA *ch, OBJ_DATA *corpse);
void spell_vampiric_touch (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_black_circle (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_visceral (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void visceral_two (CHAR_DATA *ch);
void visceral_three (CHAR_DATA *ch);
void visceral_four (CHAR_DATA *ch);
void spell_ritual_soul (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void ritual_two (CHAR_DATA *ch, CHAR_DATA *victim);
void ritual_three (CHAR_DATA *ch, CHAR_DATA *victim);
void ritual_four (CHAR_DATA* ch, CHAR_DATA *victim);
void spell_ritual_flesh (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void flesh_two (CHAR_DATA *ch, CHAR_DATA *victim);
void flesh_three (CHAR_DATA *ch, CHAR_DATA *victim);
void flesh_four (CHAR_DATA *ch, CHAR_DATA *victim);
void spell_decrepify (int sn,int level,CHAR_DATA *ch, void *vo, int target);
void spell_corrupt_flesh (int sn, int level,CHAR_DATA *ch, void *vo, int target);
void spell_corpse_trap (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_lesser_golem (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_greater_golem (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void do_drain (CHAR_DATA *ch, char *argument);
bool check_bond (CHAR_DATA *ch, CHAR_DATA *mob);
bool check_zombie_summon (CHAR_DATA *ch);
void soul_add (CHAR_DATA *ch, int souls);

#endif /* NECRO_H */
