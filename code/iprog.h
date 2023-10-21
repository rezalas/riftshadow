#ifndef IPROG_H
#define IPROG_H

#include "merc.h"

extern const struct improg_type iprog_table[];
extern const struct improg_type verb_prog_table[];

//
// TODO: UNKNOWN FUNCTIONS
//

// extern void fight_prog_tattoo_sceptre (OBJ_DATA *obj, CHAR_DATA *ch);


//
// LOCAL FUNCTIONS
//

void iprog_set (OBJ_INDEX_DATA *obj,const char *progtype, const char *name);
bool iprog_unset (OBJ_INDEX_DATA *obj,const char *progtype, const char *name);
/* ITEM FUNCTIONS */
void fight_prog_drow_talisman (OBJ_DATA *obj, CHAR_DATA *ch);
void fight_prog_devils_eye (OBJ_DATA *obj, CHAR_DATA *ch);
void fight_prog_skean (OBJ_DATA *obj, CHAR_DATA *ch);
void fight_prog_cankerworm (OBJ_DATA *obj, CHAR_DATA *ch);
void fight_prog_axe_trelaran (OBJ_DATA *obj, CHAR_DATA *ch);
void fight_prog_cure_critical (OBJ_DATA *obj, CHAR_DATA *ch);
void pulse_prog_steal (OBJ_DATA *obj, bool isTick);
void invoke_prog_tattoo_dioxide (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);
void invoke_prog_tattoo_jackass (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);
void invoke_prog_tattoo_morglum (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);
void invoke_prog_tattoo_calenduil (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);
void invoke_prog_tattoo_sceptre (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);
void invoke_prog_tattoo_zethus (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);
void entry_prog_explosives (OBJ_DATA *obj);
bool death_prog_explosives (OBJ_DATA *obj, CHAR_DATA *ch);
/* explosive packs -- Dioxide */
void invoke_prog_explosives (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);
void speech_prog_ice_dragon_statue (OBJ_DATA *obj, CHAR_DATA *ch, char *speech);
void fight_prog_scales (OBJ_DATA *obj, CHAR_DATA *ch);
void fight_prog_amber_medallion (OBJ_DATA *obj, CHAR_DATA *ch);
bool give_prog_cabal_item (OBJ_DATA *obj, CHAR_DATA *from, CHAR_DATA *to);
void get_prog_bad_idea (OBJ_DATA *obj, CHAR_DATA *ch);
void greet_prog_corpse_explode (OBJ_DATA *obj, CHAR_DATA *ch);
void fight_prog_horde_bull (OBJ_DATA *obj, CHAR_DATA *ch);
void fight_prog_horde_bear (OBJ_DATA *obj, CHAR_DATA *ch);
void fight_prog_horde_lion (OBJ_DATA *obj, CHAR_DATA *ch);
void fight_prog_horde_wolf (OBJ_DATA *obj, CHAR_DATA *ch);
void fight_prog_horde_hawk (OBJ_DATA *obj, CHAR_DATA *ch);
void pulse_prog_horde_jackal (OBJ_DATA *obj, bool isTick);
bool death_prog_trophy_belt (OBJ_DATA *belt, CHAR_DATA *ch);
void fight_prog_ruins_sword (OBJ_DATA *obj, CHAR_DATA *ch);
void verb_prog_check_bounties (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);
void verb_prog_tilt_scales (OBJ_DATA *obj,CHAR_DATA *ch,char *argument);
void verb_prog_ilopheth_bush (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);
void verb_prog_ilopheth_climb_tree (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);
void verb_prog_antava_touch_hand (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);
void verb_prog_sidhe_climb_vine (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);
void verb_prog_listen_conversation (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);
void verb_prog_rub_ball (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);
void verb_prog_twist_ring (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);
void verb_prog_twist_two_faced (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);
void verb_prog_energize_tattoo (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);
void verb_prog_evoke_stone (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);
void verb_prog_harness_crystal (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);
void verb_prog_throw_net (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);
void verb_prog_fire_pistol (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);
void verb_prog_kneel_guillotine (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);
void hit_prog_blade_truth (OBJ_DATA *obj, CHAR_DATA *ch, CHAR_DATA *victim, int damage);
void hit_prog_essence_light (OBJ_DATA *obj, CHAR_DATA *ch, CHAR_DATA *victim, int damage);
void hit_prog_essence_darkness (OBJ_DATA *obj, CHAR_DATA *ch, CHAR_DATA *victim, int damage);
void fight_prog_essence_darkness (OBJ_DATA *obj, CHAR_DATA *ch);
void verb_prog_rub_talisman (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);
void verb_prog_gate_talisman (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);
void speech_prog_vnum_talisman (OBJ_DATA *obj, CHAR_DATA *ch, char *speech);
void pulse_prog_pillar_zap (OBJ_DATA *obj, bool isTick);
bool loot_prog_shelf (OBJ_DATA *shelf, OBJ_DATA *obj, CHAR_DATA *ch);
bool open_prog_sewer_casket (OBJ_DATA *obj, CHAR_DATA *ch);
void verb_prog_pull_hook (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);
void verb_prog_turn_spindle (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);
void verb_prog_touch_obelisk (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);
void communion_handler(CHAR_DATA *ch);
void pulse_prog_cimar_babies (OBJ_DATA *obj, bool isTick);
void verb_prog_feed_baby (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);
void baby_end (OBJ_DATA *obj, OBJ_AFFECT_DATA *af);
void baby_burp (OBJ_DATA *obj, OBJ_AFFECT_DATA *af);
void verb_prog_pull_book (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);
/* Fix0red by Morglum */
void trapdoor_end (ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *af);
bool open_prog_beef_balls (OBJ_DATA *obj, CHAR_DATA *ch);
void verb_prog_look_topbounties (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);
void verb_prog_pour_wine (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);
void wine_pulse (OBJ_DATA *obj, OBJ_AFFECT_DATA *af);
void verb_prog_attach_weapon (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);
void verb_prog_join_guild (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);
void verb_prog_pull_lever (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);
void verb_prog_tie_rope (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);
void verb_prog_turn_wheel (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);
void rope_end (OBJ_DATA *obj, OBJ_AFFECT_DATA *af);
void verb_prog_tilt_bust (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);
void verb_prog_roll_tablet (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);
void close_elevator (ROOM_INDEX_DATA *pRoom);
void open_elevator (ROOM_INDEX_DATA *eleRoom, ROOM_INDEX_DATA *toRoom);
void act_to_room( void *vo1, void *vo2);
void verb_prog_iseldheim_lever_pull (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);
void fight_prog_bugzapper (OBJ_DATA *obj, CHAR_DATA *ch);
void fight_prog_arms_light (OBJ_DATA *obj, CHAR_DATA *ch);
void drop_prog_elven_gem (OBJ_DATA *obj, CHAR_DATA *ch);
void speech_prog_elven_mirror (OBJ_DATA *obj, CHAR_DATA *ch, char *speech);
void verb_prog_turn_wyntran (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);
void verb_prog_place_star (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);
// Fallen Desert Progs
void verb_prog_fallendesert_climb_ladder (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);
void verb_prog_fallendesert_ (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);

#endif /* IPROG_H */
