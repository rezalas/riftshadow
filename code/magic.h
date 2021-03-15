/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik Stærfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/
 
/***************************************************************************
 *  ROM 2.4 is copyright 1993-1996 Russ Taylor                             *
 *  ROM has been brought to you by the ROM consortium                      *
 *      Russ Taylor (rtaylor@efn.org)                                      *
 *      Gabrielle Taylor (gtaylor@pacinfo.com)                             *
 *      Brian Moore (zump@rom.org)                                         *
 *  By using this code, you have agreed to follow the terms of the         *
 *  ROM license, in the file Tartarus/doc/rom.license                      *
 ***************************************************************************/

/***************************************************************************
 *       Tartarus code is copyright (C) 1997-1998 by Daniel Graham         *
 *  In using this code you agree to comply with the Tartarus license       *
 *       found in the file /Tartarus/doc/tartarus.doc                      *
 ***************************************************************************/

#ifndef MAGIC_H
#define MAGIC_H

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "merc.h"
#include "necro.h"
#include "magic.h"
#include "recycle.h"
#include "tables.h"
#include "act_obj.h"
#include "act_ente.h"
#include "act_comm.h"
#include "warrior.h"
#include "dioextra.h"
#include "sorcerer.h"
#include "act_move.h"
#include "fight.h"
#include "act_info.h"
#include "newmem.h"
#include "handler.h"
#include "effects.h"
#include "comm.h"
#include "interp.h"
#include "skill.h"
#include "devextra.h"
#include "db.h"
#include "chardef.h"
#include "const.h"
#include "material.h"

//
// TODO: UNKNOWN FUNCTIONS
//

// extern void spell_hand_of_vengeance(int sn, int level, CHAR_DATA *ch, void *vo, int target);
// extern void spell_earthquake(int sn, int level, CHAR_DATA *ch, void *vo, int target);
// extern void spell_floating_disc(int sn, int level, CHAR_DATA *ch, void *vo, int target);
// extern void spell_infernal_aid(int sn, int level, CHAR_DATA *ch, void *vo, int target);
// extern void spell_fortitude(int sn, int level, CHAR_DATA *ch, void *vo, int target);
// extern void spell_spiritblade(int sn, int level, CHAR_DATA *ch, void *vo, int target);
// extern void spell_regenerate(int sn, int level, CHAR_DATA *ch, void *vo, int target);
// extern void spell_decoy(int sn, int level, CHAR_DATA *ch, void *vo, int target);
// extern void spell_decay_corpse(int sn, int level, CHAR_DATA *ch, void *vo, int target);
// extern void spell_life_transfer(int sn, int level, CHAR_DATA *ch, void *vo, int target);
// extern void spell_power_word_stun(int sn, int level, CHAR_DATA *ch, void *vo, int target);
// extern void spell_trace(int sn, int level, CHAR_DATA *ch, void *vo, int target);
// extern void spell_flame_scorch(int sn, int level, CHAR_DATA *ch, void *vo, int target);
// extern void spell_soulbind(int sn, int level, CHAR_DATA *ch, void *vo, int target);
// extern void spell_globe_of_darkness(int sn, int level, CHAR_DATA *ch, void *vo, int target);
// extern void spell_drain(int sn, int level, CHAR_DATA *ch, void *vo, int target);
// extern void spell_slave_of_purgatory(int sn, int level, CHAR_DATA *ch, void *vo, int target);	/* Spell for Ancient Only -Detlef */
// extern void spell_mental_knife(int sn, int level, CHAR_DATA *ch, void *vo, int target);			/* Spell for Kyr's tattoo -Dio */
// extern void spell_unholy_bless(int sn, int level, CHAR_DATA *ch, void *vo, int target);
// extern void spell_confuse(int sn, int level, CHAR_DATA *ch, void *vo, int target);				/* Confuse for Outlaw -Detlef */
// extern void spell_scrying(int sn, int level, CHAR_DATA *ch, void *vo, int target);
// extern void spell_nova(int sn, int level, CHAR_DATA *ch, void *vo, int target);
// extern void spell_randomizer(int sn, int level, CHAR_DATA *ch, void *vo, int target);
// extern void spell_dark_shade(int sn, int level, CHAR_DATA *ch, void *vo, int target);			/* for ancient --dioxide */
// extern void spell_lightwalk(int sn, int level, CHAR_DATA *ch, void *vo, int target);
// extern void spell_tsunami(int sn, int level, CHAR_DATA *ch, void *vo, int target);
// extern void spell_familiar(int sn, int level, CHAR_DATA *ch, void *vo, int target);
// extern void spell_fire_and_ice(int sn, int level, CHAR_DATA *ch, void *vo, int target);
// extern void spell_lifeline(int sn, int level, CHAR_DATA *ch, void *vo, int target);
// extern void spell_shadowself(int sn, int level, CHAR_DATA *ch, void *vo, int target);
// extern void spell_true_sight(int sn, int level, CHAR_DATA *ch, void *vo, int target);
// extern void spell_guard_call(int sn, int level, CHAR_DATA *ch, void *vo, int target);
// extern void spell_call_slayer(int sn, int level, CHAR_DATA *ch, void *vo, int target);
// extern void spell_acid_spit(int sn, int level, CHAR_DATA *ch, void *vo, int target);
// extern void spell_unholy_fire(int sn, int level, CHAR_DATA *ch, void *vo, int target);
// extern void spell_general_purpose(int sn, int level, CHAR_DATA *ch, void *vo, int target);
// extern void spell_nexus(int sn, int level, CHAR_DATA *ch, void *vo, int target);
// extern void spell_portal(int sn, int level, CHAR_DATA *ch, void *vo, int target);
// extern void false_motives_end(CHAR_DATA *ch, AFFECT_DATA *af);

//
// LOCAL FUNCTIONS
//

int skill_lookup (const char *name);
int find_spell(CHAR_DATA *ch, const char *name);
void say_spell (CHAR_DATA *ch, int sn);
bool saves_spell (int level, CHAR_DATA *victim, int dam_type);
bool saves_dispel(int dis_level, int spell_level, int duration);
bool check_dispel(int dis_level, CHAR_DATA *victim, int sn);
bool check_dispel_cancellation(int dis_level, CHAR_DATA *victim, CHAR_DATA *ch, int sn, bool commune, bool cancel);
int mana_cost(CHAR_DATA *ch, int min_mana, int level);
void do_barkskin(CHAR_DATA *ch, char *argument);
void do_laying_hands(CHAR_DATA *ch, char *argument);
void cast_myell(CHAR_DATA *ch, CHAR_DATA *victim);
void do_cast(CHAR_DATA *ch, char *argument);
void obj_cast_spell (int sn, int level, CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *obj);
void spell_acid_blast(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_armor(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_bless(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_blindness(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_burning_hands(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_cancellation(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_cause_light(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_cause_critical(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_cause_serious(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_chain_lightning(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_change_sex(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_charm_person(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_chill_touch(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_color_spray(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_wrath(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_continual_light(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_control_weather(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_create_food(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_create_rose(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_create_spring(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_create_water(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_cure_blindness(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_cure_critical(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_cure_disease(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_cure_light(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_cure_poison(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_cure_serious(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_curse(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_dark_wrath(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_demonfire(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_detect_evil(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_detect_good(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void do_detect_hidden(CHAR_DATA *ch, char *argument);
void do_detect_movement(CHAR_DATA *ch, char *argument);
void spell_detect_invis(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_detect_magic(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_detect_poison(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_dispel_evil(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_dispel_good(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_dispel_magic(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_fireball(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_fireproof(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_flamestrike(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_faerie_fire(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_faerie_fog(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_fly(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_frenzy(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_gate(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_giant_strength(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_harm(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_haste(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_heal(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_holy_word(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_identify(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_invis(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_know_alignment(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_lightning_bolt(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_locate_object(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_magic_missile(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_mass_healing(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_mass_invis(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_null(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_pass_door(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_plague(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void plague_tick(CHAR_DATA *ch, AFFECT_DATA *af);
void spell_poison(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void poison_tick(CHAR_DATA *ch, AFFECT_DATA *af);
void spell_protection(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_ray_of_truth(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_recharge(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_refresh(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_remove_curse(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_sanctuary(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_shield(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_shocking_grasp(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_sleep(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_slow(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_stone_skin(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_summon(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void summon_char(CHAR_DATA *ch, CHAR_DATA *victim);

//
// Modified teleport -- only within radius of one area now.
//
void spell_teleport(int sn, int level, CHAR_DATA *ch, void *vo, int target);

//
// Modified turn undead spell. Good and neutral clerics attempt to destroy
// undead, with good aligns getting a bonus. Evil try to either subdue aggro
// undead or control them, depending on the difference in their level.
//
void spell_turn_undead(int sn, int level, CHAR_DATA *ch, void *vo, int target);

void spell_ventriloquate(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_weaken(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_word_of_recall(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void recall_execute(CHAR_DATA *ch, ROOM_INDEX_DATA *location);
void spell_acid_breath(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_fire_breath(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_frost_breath(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_gas_breath(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_nether_breath(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_lightning_breath(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_iceball(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_protective_shield(int sn, int level, CHAR_DATA *ch, void *vo, int target);
int check_sanguine(CHAR_DATA *ch);
void sanguine_blind(CHAR_DATA *ch, CHAR_DATA *victim);
void spell_sanguine_ward(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_consecrate(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_safety(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_blade_barrier(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_old_blade_barrier(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_holy_fire(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_talk_to_dead(int sn, int level, CHAR_DATA *ch, void *vo, int target);

//
// New energy drain spell due to anti-paladin's always complaining how
// useless the old one was.
// -Ceran
//
void spell_energy_drain(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_dark_shield(int sn, int level, CHAR_DATA *ch, void *vo, int target);
int check_spellcraft(CHAR_DATA *ch, int sn);
void spell_deathspell(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_lifebane(int sn, int level, CHAR_DATA *ch, void *vo, int target);

//
// sceptre of heavens...say 'i am the wrath of god'
//
void spell_heavenly_sceptre_frenzy(int sn, int level, CHAR_DATA *ch, void *vo, int target);

//
// sceptre of heavens, say 'Feel the force of god'
//
void spell_heavenly_sceptre_fire(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_lightshield(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_forget(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_earthbind(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_cremate(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_divine_touch(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_transfer_object(int sn, int level, CHAR_DATA *ch, void *vo, int target);

//
// Necros use this to keep body parts longer...for lesser golems
//
void spell_preserve(int sn, int level, CHAR_DATA *ch, void *vo, int target);

//
// Make a victim flee...if they really fail their saves big time it can
// kill them outright..(Ceran)
//
void spell_power_word_fear(int sn, int level, CHAR_DATA *ch, void *vo, int target);

//
// Causes a target's hp to regenerate at a constant rate, plus removes the
// effects of a wither prevent healing spell
//
void spell_imbue_regeneration(int sn, int level, CHAR_DATA *ch, void *vo, int target);

//
//	Restores forget, wither, prevent healing, atrophy.
//	Also restores lost levels due to energy drain by powerful undead
//
void spell_restoration(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_prevent_healing(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_atrophy(int sn, int level, CHAR_DATA *ch, void *vo, int target);

//
//	Heal spell, but also for only slightly more cost can cure poison and disease.
//	Can stop wasting, but no heal benefit is gained if used this way. Won't
//	restore undead_drains or wither etc.
//
void spell_utter_heal(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_hold_person(int sn, int level, CHAR_DATA *ch, void *vo, int target);			/* Hold person -Detlef */
void spell_crushing_hand(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_deafen(int sn, int level, CHAR_DATA *ch, void *vo, int target);				/* Deafen for AP -Detlef */
void spell_talismanic_aura(int sn, int level, CHAR_DATA *ch, void *vo, int target);
void talismanic_end(CHAR_DATA *ch, AFFECT_DATA *af);
void puddle_evaporate(OBJ_DATA *obj, OBJ_AFFECT_DATA *af);
bool check_somatic(CHAR_DATA *ch);
void mana_drain_pulse(CHAR_DATA *ch, AFFECT_DATA *af);
void colorful_end(CHAR_DATA *ch, AFFECT_DATA *af);
void spell_colorful(int sn, int level, CHAR_DATA *ch, void *vo, int target);

#endif /* MAGIC_H */
