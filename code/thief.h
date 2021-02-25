#ifndef THIEF_H
#define THIEF_H

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "merc.h"
#include "interp.h"
#include "tables.h"
#include "recycle.h"
#include "magic.h"
#include "db.h"
#include "lookup.h"
#include "act_move.h"
#include "act_comm.h"
#include "dioextra.h"
#include "fight.h"
#include "db.h"
#include "const.h"
#include "material.h"


//
// LOCAL FUNCTIONS
//

void do_backstab (CHAR_DATA *ch, char *argument);
void do_circle_stab (CHAR_DATA *ch, char *argument);
void do_blackjack (CHAR_DATA *ch,char *argument);
void do_ghetto_bind (CHAR_DATA *ch,char *argument);
void do_ghetto_unbind (CHAR_DATA *ch,char *argument);
void do_plant (CHAR_DATA *ch, char *argument);
void do_ungag (CHAR_DATA *ch,char *argument);
void do_gag (CHAR_DATA *ch, char *argument);
bool check_drag (CHAR_DATA *ch, ROOM_INDEX_DATA *to_room, int where);
void do_drag (CHAR_DATA *ch, char *argument);
void do_tripwire (CHAR_DATA *ch, char *argument);
void do_sign (CHAR_DATA *ch, char *argument);
void do_slash (CHAR_DATA *ch, char *argument);
void do_stash (CHAR_DATA *ch, char *argument);
void do_disguise (CHAR_DATA *ch, char *argument);
void disguise_end (CHAR_DATA *ch, AFFECT_DATA *af);
void disguise_pulse (CHAR_DATA *ch, AFFECT_DATA *af);
void disguise_remove (CHAR_DATA *ch);
void do_undisguise (CHAR_DATA *ch, char *argument);
void do_search (CHAR_DATA *ch, char *argument);
void do_counterfeit (CHAR_DATA *ch, char *argument);
void counterfeit_end (OBJ_DATA *obj, OBJ_AFFECT_DATA *af);
void do_shadow_cloak (CHAR_DATA *ch, char *argument);
bool check_stealth (CHAR_DATA *ch, CHAR_DATA *mob);
void do_strip (CHAR_DATA *ch, char *argument);
void thief_yell (CHAR_DATA *ch, CHAR_DATA *victim);
AFFECT_DATA *check_bind (CHAR_DATA *ch,char *type);
void do_bind (CHAR_DATA *ch,char *argument);
void do_unbind (CHAR_DATA *ch,char *argument);
void do_knife (CHAR_DATA *ch, char *argument);
void check_detect_movement (CHAR_DATA *ch, char *in_out);
void do_bluff (CHAR_DATA *ch, char *argument);
void do_false_motives (CHAR_DATA *ch, char *argument);

#endif /* THIEF_H */
