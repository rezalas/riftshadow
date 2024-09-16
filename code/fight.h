#ifndef FIGHT_H
#define FIGHT_H

#include "merc.h"

#define HITS 					(dt==gsn_kick||dt==gsn_throw||dt==gsn_bash)
#define PEER_BALANCE_DISTANCE	8
#define BASE_PEER_FACTOR		1.75
#define SUMMONED_XP_PENALTY		0.55

//
// TODO: UNKNOWN FUNCTIONS
//
//bool check_counter (CHAR_DATA *ch,CHAR_DATA *victim, int dam,int dt);

//
// LOCAL FUNCTIONS
//

/*
 * Control the fights going on.
 * Called periodically by update_handler.
 */
void violence_update (void);
/* for auto assisting */
void check_assist (CHAR_DATA *ch, CHAR_DATA *victim);
/*
 * Do one group of attacks.
 */
void multi_hit (CHAR_DATA *ch, CHAR_DATA *victim, int dt);
/* procedure for all mobile attacks */
void mob_hit (CHAR_DATA *ch, CHAR_DATA *victim, int dt);
/*
 * Hit one guy once.
 */
int one_hit (CHAR_DATA *ch, CHAR_DATA *victim, int dt);
/* De-Ceranized by Cal on 10/24/01 
* return of -1 = Death			*/
int one_hit_new (CHAR_DATA *ch, CHAR_DATA *victim, int dt, bool specials, bool blockable, int addition, int multiplier, char *dnoun);
/*
 * Inflict damage from a hit.
 */
int damage_new (CHAR_DATA *ch, CHAR_DATA *victim, int idam, int dt, int dam_type, bool show, bool blockable, int addition, int multiplier, char *dnoun);
int damage (CHAR_DATA *ch,CHAR_DATA *victim, int dam,int dt,int dam_type, bool show);
/*
 * Inflict damage from a hit.
 */
int damage_old (CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int dam_type, bool show);
bool is_safe (CHAR_DATA *ch, CHAR_DATA *victim);
bool is_safe_new (CHAR_DATA *ch, CHAR_DATA *victim, bool show);
bool is_safe_spell (CHAR_DATA *ch, CHAR_DATA *victim, bool area);
int check_armor (CHAR_DATA *ch, CHAR_DATA *victim, int dt, int dam_type, int dam);
bool check_parry (CHAR_DATA *ch, CHAR_DATA *victim, int dt);
/*
 * Check for shield block.
 */
bool check_shield_block (CHAR_DATA *ch, CHAR_DATA *victim, int dt);
/*
 * Check for a piety deflect
 */
bool check_piety (CHAR_DATA *ch, CHAR_DATA *victim, int dt);
/*
 * Check for dodge.
 */
bool check_dodge (CHAR_DATA *ch, CHAR_DATA *victim, int dt);
bool check_avoid (CHAR_DATA *ch, CHAR_DATA *victim, int dt);
int	check_evasion (CHAR_DATA *ch, int chance);
bool check_fend (CHAR_DATA *ch, CHAR_DATA *victim, int dt);
bool check_deflect (CHAR_DATA *ch, CHAR_DATA *victim, int dt);
bool check_mist (CHAR_DATA *ch, CHAR_DATA *victim, int dt);
/*
 * Tactician skill, decide whether to increment analyze counter
 */
void check_analyze (CHAR_DATA *ch, CHAR_DATA *victim);
/*
 * Set position of a victim.
 */
void update_pos (CHAR_DATA *victim);
/*
 * Start fights.
 */
void set_fighting (CHAR_DATA *ch, CHAR_DATA *victim);
void combat_alert (CHAR_DATA *victim, int type, CHAR_DATA *ch);
/*
 * Stop fights.
 */
void stop_fighting (CHAR_DATA *ch, bool fBoth);
/*
 * Make a corpse out of a character.
 */
void make_corpse (CHAR_DATA *killer, CHAR_DATA *ch);
/*
 * Improved Death_cry contributed by Diavolo.
 */
void death_cry (CHAR_DATA *ch, bool infidels);
void raw_kill (CHAR_DATA *ch, CHAR_DATA *victim);
void pk_record (CHAR_DATA *ch, CHAR_DATA *victim);
void group_gain (CHAR_DATA *ch, CHAR_DATA *victim);
int xp_compute_pk (CHAR_DATA *ch, CHAR_DATA *victim, int members);
/*
 * Compute xp for a kill.
 * Also adjust alignment of killer.
 * Edit this function to change xp computations.
 */
int xp_compute (CHAR_DATA *gch, CHAR_DATA *victim, int group_amount, int glevel);
void dam_message (CHAR_DATA *ch, CHAR_DATA *victim,int dam,int dt,bool immune, char *dnoun, bool reduced);
void warrior_ai (CHAR_DATA *mob, CHAR_DATA *victim);
void thief_ai (CHAR_DATA *mob, CHAR_DATA *victim);
bool can_bash (CHAR_DATA *ch, CHAR_DATA *victim);
/*
 * Disarm a creature.
 * Caller must check for successful attack.
 */
void disarm (CHAR_DATA *ch, CHAR_DATA *victim);
void do_berserk (CHAR_DATA *ch, char *argument);
void do_bash (CHAR_DATA *ch, char *argument);
void do_bash_queue (CHAR_DATA *ch, std::string argument);
void do_dirt (CHAR_DATA *ch, char *argument);
void do_trip (CHAR_DATA *ch, char *argument);
void do_hit (CHAR_DATA *ch, char *argument);
void do_kill (CHAR_DATA *ch, char *argument);
void do_murde (CHAR_DATA *ch, char *argument);
void do_murder (CHAR_DATA *ch, char *argument);
void do_ambush (CHAR_DATA *ch, char *argument);
void do_rescue (CHAR_DATA *ch, char *argument);
void do_kick (CHAR_DATA *ch, char *argument);
void do_disarm (CHAR_DATA *ch, char *argument);
void do_surrender (CHAR_DATA *ch, char *argument);
void do_sla (CHAR_DATA *ch, char *argument);
void do_slay (CHAR_DATA *ch, char *argument);
void spell_power_word_kill (int sn,int level,CHAR_DATA *ch,void *vo, int target);
void do_herb (CHAR_DATA *ch,char *argument);
void do_cleave (CHAR_DATA *ch,char *argument);
void check_ground_control (CHAR_DATA *ch, CHAR_DATA *victim, float chance, int dam);
void do_tail (CHAR_DATA *ch, char *argument);
/* Monk skills */
void do_throw (CHAR_DATA *ch, char *argument);
void do_nerve (CHAR_DATA *ch, char *argument);
void do_endure (CHAR_DATA *ch, char *argument);
void do_blindness_dust (CHAR_DATA *ch, char *argument);
void do_poison_dust (CHAR_DATA *ch,char *argument);
void do_warcry (CHAR_DATA *ch, char *argument);
void do_strangle (CHAR_DATA *ch, char *argument);
void do_enlist (CHAR_DATA *ch, char *argument);
void do_tame (CHAR_DATA *ch, char *argument);
void do_find_water (CHAR_DATA *ch, char *argument);
void do_track (CHAR_DATA *ch, char *argument);
void do_shield_cleave (CHAR_DATA *ch, char *argument);
void bag_explode (CHAR_DATA *ch, OBJ_DATA *obj, int everyone);
/* dying of old age */
void age_death (CHAR_DATA *ch);
void do_forage (CHAR_DATA *ch, char *argument);
void do_defend (CHAR_DATA *ch, char *argument);
void do_intimidate (CHAR_DATA *ch, char *argument);
//New flee...this allows 'flee <direction>' for the thief skill escape.
void do_flee (CHAR_DATA *ch, char *argument);
/* Nasty assassination skill. Chance of success is very high, should be
changed if you plan on using this on a PK mud. -Ceran
*/
void do_assassinate (CHAR_DATA *ch, char *argument);
/* Some more new skills..Ceran */
void do_lash (CHAR_DATA *ch, char *argument);
void do_pugil (CHAR_DATA *ch, char *argument);
void do_protection_heat_cold (CHAR_DATA *ch, char *argument);
void do_call_to_arms (CHAR_DATA *ch, char *arguement);
void do_iron_resolve (CHAR_DATA *ch, char *argument);
void do_quiet_movement (CHAR_DATA *ch, char *argument);
bool check_parting_blow (CHAR_DATA *ch, CHAR_DATA *victim);
CHAR_DATA *get_cabal_guardian (int cabal);
bool check_blade_barrier (CHAR_DATA *ch, CHAR_DATA *victim);
bool check_maneuvering (CHAR_DATA *ch, CHAR_DATA *victim, int dt);
bool check_catch (CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *obj);
char *get_dam_message (CHAR_DATA *ch, int dt);
char *get_attack_noun (CHAR_DATA *ch, int dt);
int get_attack_number (CHAR_DATA *ch, int dt);
void trophy_corpse (CHAR_DATA *ch, CHAR_DATA *victim);
bool check_sidestep (CHAR_DATA *ch, CHAR_DATA *victim, int skill, int mod);
int check_terrain_mastery (CHAR_DATA *ch);
void bleeding_tick (CHAR_DATA *ch, AFFECT_DATA *af);
void do_gore (CHAR_DATA *ch, char *argument);
void do_headbutt (CHAR_DATA *ch, char *argument);
void do_disengage (CHAR_DATA *ch, char *argument);
bool is_wielded (CHAR_DATA *ch, int weapon, int type);

#endif /* FIGHT_H */
