#ifndef IPROG_H
#define IPROG_H

#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "magic.h"
#include "interp.h"
#include "recycle.h"
#include "tables.h"
#include "lookup.h"

char * flag_name_lookup (long bitv, const struct flag_type *flag_table);

DECLARE_IPROG_FUN_GREET( greet_prog_corpse_explode);

DECLARE_IPROG_FUN_LOOT(loot_prog_shelf);

DECLARE_IPROG_FUN_FIGHT(fight_prog_cure_critical);
DECLARE_IPROG_FUN_FIGHT(fight_prog_tattoo_sceptre);
DECLARE_IPROG_FUN_FIGHT(fight_prog_amber_medallion);
DECLARE_IPROG_FUN_FIGHT(fight_prog_axe_trelaran);
DECLARE_IPROG_FUN_FIGHT(fight_prog_horde_bull);
DECLARE_IPROG_FUN_FIGHT(fight_prog_horde_bear);
DECLARE_IPROG_FUN_FIGHT(fight_prog_horde_lion);
DECLARE_IPROG_FUN_FIGHT(fight_prog_horde_wolf);
DECLARE_IPROG_FUN_FIGHT(fight_prog_horde_hawk);
DECLARE_IPROG_FUN_FIGHT(fight_prog_ruins_sword);
DECLARE_IPROG_FUN_FIGHT(fight_prog_essence_darkness);
DECLARE_IPROG_FUN_FIGHT(fight_prog_cankerworm);
DECLARE_IPROG_FUN_FIGHT(fight_prog_skean);
DECLARE_IPROG_FUN_FIGHT(fight_prog_scales);
DECLARE_IPROG_FUN_FIGHT(fight_prog_arms_light);
DECLARE_IPROG_FUN_FIGHT(fight_prog_devils_eye);
DECLARE_IPROG_FUN_FIGHT(fight_prog_drow_talisman);

DECLARE_IPROG_FUN_PULSE(pulse_prog_steal);
DECLARE_IPROG_FUN_PULSE(pulse_prog_pillar_zap);
DECLARE_IPROG_FUN_PULSE(pulse_prog_talismanic_scrap);
DECLARE_IPROG_FUN_PULSE(pulse_prog_talismanic_page);
DECLARE_IPROG_FUN_PULSE(pulse_prog_cimar_babies);
DECLARE_IPROG_FUN_PULSE(pulse_prog_repop_container);

DECLARE_IPROG_FUN_OPEN(open_prog_sewer_casket);
DECLARE_IPROG_FUN_OPEN(open_prog_beef_balls);

DECLARE_IPROG_FUN_INVOKE(invoke_prog_tattoo_dioxide);
DECLARE_IPROG_FUN_INVOKE(invoke_prog_tattoo_jackass);
DECLARE_IPROG_FUN_INVOKE(invoke_prog_tattoo_morglum);
DECLARE_IPROG_FUN_INVOKE(invoke_prog_tattoo_calenduil);
DECLARE_IPROG_FUN_INVOKE(invoke_prog_tattoo_zethus);
DECLARE_IPROG_FUN_INVOKE(invoke_prog_explosives);

DECLARE_IPROG_FUN_ENTRY(entry_prog_explosives);

DECLARE_IPROG_FUN_DEATH(death_prog_explosives);
DECLARE_IPROG_FUN_DEATH(death_prog_trophy_belt);

DECLARE_IPROG_FUN_DROP(drop_prog_elven_gem);

DECLARE_IPROG_FUN_SPEECH(speech_prog_ice_dragon_statue);
DECLARE_IPROG_FUN_SPEECH(speech_prog_vnum_talisman);
DECLARE_IPROG_FUN_SPEECH(speech_prog_elven_mirror);

DECLARE_IPROG_FUN_GIVE(give_prog_cabal_item);

DECLARE_IPROG_FUN_PULSE(pulse_prog_horde_jackal);

DECLARE_IPROG_FUN_VERB(verb_prog_place_star);
DECLARE_IPROG_FUN_VERB(verb_prog_turn_wyntran);

// Fallen Desert Progs
DECLARE_IPROG_FUN_VERB(verb_prog_fallendesert_climb_ladder);

DECLARE_IPROG_FUN_VERB(verb_prog_rub_ball);
DECLARE_IPROG_FUN_VERB(verb_prog_twist_ring);
DECLARE_IPROG_FUN_VERB(verb_prog_twist_two_faced);
DECLARE_IPROG_FUN_VERB(verb_prog_energize_tattoo);
DECLARE_IPROG_FUN_VERB(verb_prog_harness_crystal);
DECLARE_IPROG_FUN_VERB(verb_prog_fire_pistol);
DECLARE_IPROG_FUN_VERB(verb_prog_kneel_guillotine);
DECLARE_IPROG_FUN_VERB(verb_prog_rub_talisman);
DECLARE_IPROG_FUN_VERB(verb_prog_gate_talisman);
DECLARE_IPROG_FUN_VERB(verb_prog_touch_obelisk);
DECLARE_IPROG_FUN_VERB(verb_prog_pull_hook);
DECLARE_IPROG_FUN_VERB(verb_prog_turn_spindle);
DECLARE_IPROG_FUN_VERB(verb_prog_listen_conversation);
DECLARE_IPROG_FUN_VERB(verb_prog_feed_baby);
DECLARE_IPROG_FUN_VERB(verb_prog_antava_touch_hand);
DECLARE_IPROG_FUN_VERB(verb_prog_ilopheth_bush);
DECLARE_IPROG_FUN_VERB(verb_prog_ilopheth_climb_tree);
DECLARE_IPROG_FUN_VERB(verb_prog_sidhe_climb_vine);
DECLARE_IPROG_FUN_VERB(verb_prog_pull_book);
DECLARE_IPROG_FUN_VERB(verb_prog_look_topbounties);
DECLARE_IPROG_FUN_VERB(verb_prog_pour_wine);
DECLARE_IPROG_FUN_VERB(verb_prog_attach_weapon);
DECLARE_IPROG_FUN_VERB(verb_prog_join_guild);
DECLARE_IPROG_FUN_VERB(verb_prog_pull_lever);
DECLARE_IPROG_FUN_VERB(verb_prog_turn_wheel);
DECLARE_IPROG_FUN_VERB(verb_prog_tie_rope);
DECLARE_IPROG_FUN_VERB(verb_prog_tilt_bust);
DECLARE_IPROG_FUN_VERB(verb_prog_tilt_scales);
DECLARE_IPROG_FUN_VERB(verb_prog_roll_tablet);
DECLARE_IPROG_FUN_VERB(verb_prog_throw_net);
DECLARE_IPROG_FUN_VERB(verb_prog_check_bounties);
DECLARE_IPROG_FUN_VERB(verb_prog_iseldheim_lever_pull);

DECLARE_IPROG_FUN_HIT(hit_prog_blade_truth);
DECLARE_IPROG_FUN_HIT(hit_prog_essence_light);
DECLARE_IPROG_FUN_HIT(hit_prog_essence_darkness);
DECLARE_IPROG_FUN_FIGHT(fight_prog_bugzapper);

DECLARE_IPROG_FUN_GET(get_prog_bad_idea);


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

extern void mprog_say (int inc, char *arg, CHAR_DATA *mob, CHAR_DATA *ch);
extern void mprog_emote (int inc, char *arg, CHAR_DATA *mob, CHAR_DATA *ch);

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
