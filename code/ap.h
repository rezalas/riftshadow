#ifndef AP_H
#define AP_H

/* Home for everything Anti */
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <time.h>
#include <math.h>
#include "merc.h"
#include "interp.h"
#include "tables.h"
#include "recycle.h"
#include "magic.h"
#include "db.h"
#include "lookup.h"
#include "fight.h"
#include "act_info.h"
#include "newmem.h"
#include "act_comm.h"
#include "act_info.h"
#include "const.h"
#include "utility.h"

//
// LOCAL FUNCTIONS
//

void check_bloodlust (CHAR_DATA *ch, CHAR_DATA *victim);
void check_leech (CHAR_DATA *ch, CHAR_DATA *victim);
void spell_indomitability (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void do_taunt (CHAR_DATA *ch, char *argument);
void spell_wrack (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_radiance (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void wrack_tick (CHAR_DATA *ch, AFFECT_DATA *af);
void spell_inspire_lust (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void lust_pulse (CHAR_DATA *ch, AFFECT_DATA *af);
void spell_dispaters (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void do_consume (CHAR_DATA *ch, char *argument);
void spell_baals_mastery (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void check_baals_mastery (CHAR_DATA *ch, CHAR_DATA *victim);
void spell_word_of_command (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void command_execute (CHAR_DATA *ch);
void command_execute_delay (CHAR_DATA *ch, char* command);
void spell_mark_of_wrath (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_living_blade (int sn, int level, CHAR_DATA *ch, void *vo, int target);
/* Note: this is fugly. */
void living_blade_pulse (OBJ_DATA *obj, OBJ_AFFECT_DATA *af);
void living_blade_end (OBJ_DATA *obj, OBJ_AFFECT_DATA *af);
void traitor_pulse (CHAR_DATA *ch, AFFECT_DATA *af);
void spell_dark_familiar (int sn, int level, CHAR_DATA *ch, void *vo, int target);
/* The spell used to begin the demon-summoning rituals for AP favors. */
void spell_unholy_communion (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void communion_end (CHAR_DATA *ch, AFFECT_DATA *af);
void check_unholy_communion (CHAR_DATA *ch, char *argument);
void demon_appear (CHAR_DATA *ch, int *demonptr, int *typeptr);
void lesser_demon_tick (CHAR_DATA *mob, AFFECT_DATA *af);
void greater_demon_tick (CHAR_DATA *mob, AFFECT_DATA *af);
void furcas_vanish (CHAR_DATA *ch, CHAR_DATA *mob);
void spell_insanity (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void insanity_pulse (CHAR_DATA *ch, AFFECT_DATA *af);
void insanity_two (CHAR_DATA *ch, int room);
void insanity_end (CHAR_DATA *ch, AFFECT_DATA *af);
void insanity_fight (CHAR_DATA *ch);
void gamygyn_tick (CHAR_DATA *ch, AFFECT_DATA *af);
void orobas_tick (CHAR_DATA *ch, AFFECT_DATA *af);
void do_breath_mephisto (CHAR_DATA *ch, char *argument);
void mephisto_two (CHAR_DATA *ch, CHAR_DATA *victim, char *argument);
void do_touch (CHAR_DATA *ch, char *argument);
void check_orobas_gamygyn (CHAR_DATA *ch,CHAR_DATA *victim);
void burning_pulse (CHAR_DATA *ch, AFFECT_DATA *af);
void do_darksight (CHAR_DATA *ch, char *argument);
void darksight_end (CHAR_DATA *ch, AFFECT_DATA *af);
/* Dark Insight -- Completely ripped from assess */
char *get_insight_line (long where[]);
void spell_dark_insight (int sn, int level, CHAR_DATA *ch, void *vo, int target);

#endif /* AP_H */
