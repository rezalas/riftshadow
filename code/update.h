#ifndef UPDATE_H
#define UPDATE_H

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "merc.h"
#include "interp.h"
#include "tables.h"
#include "recycle.h"
#include "update.h"
#include "spec.h"
#include "lookup.h"
#include "chrono.h"
#include "act_info.h"
#include "act_move.h"
#include "thief.h"
#include "save.h"
#include "misc.h"
#include "act_obj.h"
#include "fight.h"
#include "skills.h"
#include "dioextra.h"
#include "olc.h"
#include "db.h"
#include "devextra.h"
#include "act_comm.h"
#include "comm.h"


//
// LOCAL FUNCTIONS
//

void advance_level (CHAR_DATA *ch, bool hide);
void gain_exp (CHAR_DATA *ch, int gain);
/*
 * Regeneration stuff.
 */
int hit_gain (CHAR_DATA *ch);
int mana_gain (CHAR_DATA *ch);
int move_gain (CHAR_DATA *ch);
/* If you don't want starvation/dehydration in your mud then simply return
from function before the if (ch->level > 10) check...(Ceran)
*/
void gain_condition (CHAR_DATA *ch, int iCond, int value);
/*
 * Mob autonomous action.
 * This function takes 25% to 35% of ALL Merc cpu time.
 * -- Furey
 */
void mobile_update (void);
void clear_cabal_leaders (CHAR_DATA *imm, int cabal, char *mesg);
void update_guild_leader (void);
/*
 * Update the time.
 */
void time_update (void);
void gold_update (void);
/*
 * Update the weather -- Eladrian's l33t regional weather system
 */
void weather_update (void);
void berus_update (void);
void calabren_update (void);
/*
 * Update all chars, including mobs.
*/
void char_update (void);
/*
 * Update all objs.
 * This function is performance sensitive.
 */
void obj_update (void);
/* Mobile tracking. I implemented the most stupid, basic mob tracking you
could possibly come up with, but it works ok from a player point of view.
If you want something more sophisticated just hack that into here
instead...(Ceran)
*/
// Rewrote tracking -Cal
void track_attack (CHAR_DATA *mob, CHAR_DATA *victim);
void track_update (void);
/*
 * Aggress.
 *
 * for each mortal PC
 *     for each mob in room
 *         aggress on some random PC
 *
 * This function takes 25% to 35% of ALL Merc cpu time.
 * Unfortunately, checking on each PC move is too tricky,
 *   because we don't the mob to just attack the first PC
 *   who leads the party into the room.
 *
 * -- Furey
 */
void aggr_update (void);
 /* returns age in years. */
int get_age (CHAR_DATA *ch);
int get_hours (CHAR_DATA *ch);
/* returns the age name type */
char * get_age_name_new (int age, int racenumber);
char * get_age_name (CHAR_DATA *ch);
void age_update (void);
/*
 * Handle all kinds of updates.
 * Called once per pulse from game loop.
 * Random times to defeat tick-timing clients and players.
 */
void update_handler (void);
void do_forcetick (CHAR_DATA *ch, char *argument);
void affect_update (void);
void room_update (void);
void room_affect_update (void);
void ayell_update (void);
/* iprog pulse update, also drift/sink code plugged in */
void iprog_pulse_update (bool isTick);
bool do_mob_cast (CHAR_DATA *ch);
void save_votes (void);
/* used for saving */
int reverse_d (int dir);
int get_grav_distance (OBJ_DATA *obj);
ROOM_INDEX_DATA *get_random_exit (ROOM_INDEX_DATA *room);
CHAR_DATA *get_random_ch (CHAR_DATA *ch, ROOM_INDEX_DATA *room);
CHAR_DATA *get_rand_from_room (ROOM_INDEX_DATA *room);
void save_demos (void);
void rune_update (void);

#endif /* UPDATE_H */
