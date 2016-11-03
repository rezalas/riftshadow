/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
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
*	ROM 2.4 is copyright 1993-1996 Russ Taylor			   *
*	ROM has been brought to you by the ROM consortium		   *
*	    Russ Taylor (rtaylor@pacinfo.com)				   *
*	    Gabrielle Taylor (gtaylor@pacinfo.com)			   *
*	    Brian Moore (rom@rom.efn.org)				   *
*	By using this code, you have agreed to follow the terms of the	   *
*	ROM license, in the file Tartarus/doc/rom.license                  *
***************************************************************************/

/***************************************************************************
*       Tartarus code is copyright (C) 1997-1998 by Daniel Graham          *
*	In using this code you agree to comply with the Tartarus license   *
*       found in the file /Tartarus/doc/tartarus.doc                       *
***************************************************************************/

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

char * flag_name_lookup args((long bitv, const struct flag_type *flag_table));

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

const   struct  improg_type   iprog_table[] =
{
    {	"get_prog",	"get_prog_bad_idea",		  (void *)get_prog_bad_idea,		"None"},
    {   "greet_prog",   "greet_prog_corpse_explode",     (void *)greet_prog_corpse_explode,	"None"},
    {   "fight_prog",   "fight_prog_cure_critical",	  (void *)fight_prog_cure_critical,			"None"},
    {   "fight_prog",	"fight_prog_amber_medallion",	  (void *)fight_prog_amber_medallion,	"None"},
    {   "fight_prog",	"fight_prog_horde_bull",	 (void *)fight_prog_horde_bull,			"None"},
    {   "fight_prog",	"fight_prog_horde_bear",	  (void *)fight_prog_horde_bear,			"None"},
    {   "fight_prog",   "fight_prog_horde_lion",	  (void *)fight_prog_horde_lion,			"None"},
    {   "fight_prog",   "fight_prog_horde_wolf",	  (void *)fight_prog_horde_wolf,			"None"},
    {   "fight_prog",   "fight_prog_horde_hawk",	  (void *)fight_prog_horde_hawk,			"None"},
	{	"fight_prog",	"fight_prog_axe_trelaran",	  (void *)fight_prog_axe_trelaran,			"None"},
	{	"fight_prog",	"fight_prog_ruins_sword",	(void *)fight_prog_ruins_sword,				"None"},
	{	"fight_prog",	"fight_prog_essence_darkness",	(void *)fight_prog_essence_darkness,	"None"},
	{	"fight_prog",	"fight_prog_cankerworm",		(void *)fight_prog_cankerworm,			"Cimar"},
	{	"fight_prog",	"fight_prog_skean",				(void *)fight_prog_skean,				"Cimar"},
	{	"fight_prog",	"fight_prog_scales",			(void *)fight_prog_scales,	"None"},
	{	"fight_prog",	"fight_prog_arms_light",		(void *)fight_prog_arms_light,	"None"},
	{	"fight_prog",	"fight_prog_devils_eye",		(void *)fight_prog_devils_eye,			"Cimar"},
	{	"fight_prog",	"fight_prog_drow_talisman",		(void *)fight_prog_drow_talisman,	"None"},
	{   "death_prog",   "death_prog_explosives",	  (void *)death_prog_explosives,			"None"},
    {   "death_prog",	"death_prog_trophy_belt",	  (void *)death_prog_trophy_belt,			"None"},
    {	"drop_prog",	"drop_prog_elven_gem",	  (void *)drop_prog_elven_gem,				"None"},
	{	"loot_prog",	"loot_prog_shelf",			(void *)loot_prog_shelf,					"None"},
	{	"speech_prog",	"speech_prog_vnum_talisman",	(void *)speech_prog_vnum_talisman,		"None"},
    {   "speech_prog",  "speech_prog_ice_dragon_statue", (void *)speech_prog_ice_dragon_statue,	"None"},
	{	"speech_prog",	"speech_prog_elven_mirror",	(void *)speech_prog_elven_mirror,		"None"},
    {   "pulse_prog",   "pulse_prog_steal",		  (void *)pulse_prog_steal,						"None"},
	{	"pulse_prog",	"pulse_prog_pillar_zap",		(void *)pulse_prog_pillar_zap,			"None"},
    {   "pulse_prog",   "pulse_prog_horde_jackal",        (void *)pulse_prog_horde_jackal,		"None"},
	{	"pulse_prog",	"pulse_prog_talismanic_page",	(void *)pulse_prog_talismanic_scrap,	"None"},
	{	"pulse_prog",	"pulse_prog_talismanic_scrap",	(void *)pulse_prog_talismanic_scrap, 	"None"},
	{	"pulse_prog",	"pulse_prog_cimar_babies",	(void *)pulse_prog_cimar_babies,	"Cimar"},
	{	"pulse_prog",	"pulse_prog_repop_container", (void *)pulse_prog_repop_container,		"None"},
    {   "invoke_prog",  "invoke_prog_tattoo_dioxide",     (void *)invoke_prog_tattoo_dioxide,	"None"},
    {   "invoke_prog",  "invoke_prog_tattoo_jackass",     (void *)invoke_prog_tattoo_jackass,	"None"},
	{	"invoke_prog",	"invoke_prog_tattoo_morglum",	  (void *)invoke_prog_tattoo_morglum,	"None"},
	{	"invoke_prog",	"invoke_prog_tattoo_calenduil",	  (void *)invoke_prog_tattoo_calenduil, "None"},
	{	"invoke_prog",	"invoke_prog_tattoo_zethus",	(void *)invoke_prog_tattoo_zethus,	"None"},
	{   "invoke_prog",  "invoke_prog_explosives",         (void *)invoke_prog_explosives,		"None"},
    {   "give_prog",    "give_prog_cabal_item",           (void *)give_prog_cabal_item,			"None"},
    {   "entry_prog",   "entry_prog_explosives",          (void *)entry_prog_explosives,		"None"},
	{	"open_prog",	"open_prog_sewer_casket",		(void *)open_prog_sewer_casket,			"None"},
	{	"open_prog",	"open_prog_beef_balls",			(void *)open_prog_beef_balls,			"Cimar"},
	{	"verb_prog",	"verb_prog_rub_ball",			 (void *)verb_prog_rub_ball,			"None"},
	{	"verb_prog",	"verb_prog_ilopheth_bush",		(void *)verb_prog_ilopheth_bush,		"None"},
	{	"verb_prog",	"verb_prog_ilopheth_climb_tree", (void *)verb_prog_ilopheth_climb_tree,	"Valley of Ilopheth" },
	{	"verb_prog",	"verb_prog_sidhe_climb_vine",	(void *)verb_prog_sidhe_climb_vine,		"Sidhe Village" },
	{	"verb_prog",	"verb_prog_antava_touch_hand",	(void *)verb_prog_antava_touch_hand,	"Antava Marsh"	},
	{	"verb_prog",	"verb_prog_twist_ring",			(void *)verb_prog_twist_ring,			"None"},
	{	"verb_prog",	"verb_prog_twist_two_faced",		(void *)verb_prog_twist_two_faced,		"None"},
	{	"verb_prog",	"verb_prog_energize_tattoo",		(void *)verb_prog_energize_tattoo,	"None"},
	{	"verb_prog",	"verb_prog_harness_crystal",	(void *)verb_prog_harness_crystal,		"None"},
	{	"verb_prog",	"verb_prog_fire_pistol",		(void *)verb_prog_fire_pistol,			"None"},
	{	"verb prog",	"verb_prog_kneel_guillotine",	(void *)verb_prog_kneel_guillotine,		"None"},
	{	"verb_prog",	"verb_prog_gate_talisman",		(void *)verb_prog_gate_talisman,		"None"},
	{	"verb_prog",	"verb_prog_rub_talisman",		(void *)verb_prog_rub_talisman,			"None"},
	{	"verb_prog",	"verb_prog_touch_obelisk",		(void *)verb_prog_touch_obelisk,		"None"},
	{	"verb_prog",	"verb_prog_pull_hook",			(void *)verb_prog_pull_hook,			"None"},
	{	"verb_prog",	"verb_prog_turn_spindle",		(void *)verb_prog_turn_spindle,			"None"},
	{	"verb_prog",	"verb_prog_listen_conversation",(void *)verb_prog_listen_conversation,	"Cimar"},
	{	"verb_prog",	"verb_prog_feed_baby",		(void *)verb_prog_feed_baby,	"Cimar"},
	{	"verb_prog",	"verb_prog_pull_book",		(void *)verb_prog_pull_book,	"Cimar"},
	{	"verb_prog",	"verb_prog_look_topbounties",	(void *)verb_prog_look_topbounties,	"Cimar"},
	{	"verb_prog",	"verb_prog_pour_wine",		(void *)verb_prog_pour_wine,		"Cimar"},
	{	"verb_prog",	"verb_prog_attach_weapon",	(void *)verb_prog_attach_weapon,	"Cimar"},
	{	"verb_prog",	"verb_prog_join_guild",		(void *)verb_prog_join_guild,		"Cimar"},
	{	"verb_prog",	"verb_prog_pull_lever",		(void *)verb_prog_pull_lever,		"None"},
	{	"verb_prog",	"verb_prog_turn_wheel",		(void *)verb_prog_turn_wheel,		"None"},
	{	"verb_prog",	"verb_prog_tie_rope",		(void *)verb_prog_tie_rope,		"None"},
	{	"verb_prog",	"verb_prog_tilt_bust",		(void *)verb_prog_tilt_bust,		"None"},
	{	"verb_prog",	"verb_prog_tilt_scales",	(void *)verb_prog_tilt_scales,		"None"},
	{	"verb_prog",	"verb_prog_roll_tablet",	(void *)verb_prog_roll_tablet,		"None"},
	{	"verb_prog",	"verb_prog_throw_net",		(void *)verb_prog_throw_net,		"None"},
	{	"verb_prog",	"verb_prog_check_bounties",	(void *)verb_prog_check_bounties,	"None"},
	{	"verb_prog",	"verb_prog_iseldheim_lever_pull",(void *)verb_prog_iseldheim_lever_pull,"None"},
	{	"verb_prog",	"verb_prog_place_star",		(void *)verb_prog_place_star,		"The Elven Stronghold"},
	{	"verb_prog",	"verb_prog_turn_wyntran",	(void *)verb_prog_turn_wyntran,		"The Elven Stronghold"},
	{       "verb_prog",    "verb_prog_fallendesert_climb_ladder", (void *)verb_prog_fallendesert_climb_ladder, "Fallen Desert" },
	{	"hit_prog",		"hit_prog_blade_truth",			(void *)hit_prog_blade_truth,			"None"},
	{	"hit_prog",		"hit_prog_essence_light",		(void *)hit_prog_essence_light,			"None"},
	{	"hit_prog",		"hit_prog_essence_darkness",	(void *)hit_prog_essence_darkness,		"None"},
	{	"fight_prog",	"fight_prog_bugzapper",			(void *)fight_prog_bugzapper,	"None"},
	{   NULL,		NULL,					  NULL		        ,						NULL},
};
#undef fight_prog
const struct improg_type verb_prog_table[] =
{
	{	"rub",			"verb_prog_rub_ball",		(void *)verb_prog_rub_ball,			"None"},
	{	"twist",		"verb_prog_twist_ring",		(void *)verb_prog_twist_ring,		"Melcene"},
	{	"twist",		"verb_prog_twist_two_faced",	(void *)verb_prog_twist_two_faced,	"None"},
	{	"energize",		"verb_prog_energize_tattoo",	(void *)verb_prog_energize_tattoo,	"None"},
	{	"harness",		"verb_prog_harness_crystal",(void *)verb_prog_harness_crystal,	"None"},
	{	"fire",			"verb_prog_fire_pistol",	(void *)verb_prog_fire_pistol,		"None"},
	{ 	"kneel",		"verb_prog_kneel_guillotine",(void *)verb_prog_kneel_guillotine,"None"},
	{	"rub",			"verb_prog_rub_talisman",	(void *)verb_prog_rub_talisman,		"None"},
	{	"gate",			"verb_prog_gate_talisman",	(void *)verb_prog_gate_talisman,	"None"},
	{	"touch",		"verb_prog_touch_obelisk",	(void *)verb_prog_touch_obelisk,	"None"},
	{   "pull",			"verb_prog_pull_hook",		(void *)verb_prog_pull_hook,		"None"},
	{	"turn",			"verb_prog_turn_spindle",	(void *)verb_prog_turn_spindle,		"None"},
	{	"listen",		"verb_prog_listen_conversation", (void *)verb_prog_listen_conversation, "Cimar"},
	{	"feed",			"verb_prog_feed_baby",		(void *)verb_prog_feed_baby,		"Cimar"},
	{	"push",			"verb_prog_ilopheth_bush",	(void *)verb_prog_ilopheth_bush,			"Valley of Ilopheth"},
	{	"climb",		"verb_prog_ilopheth_climb_tree",	(void *)verb_prog_ilopheth_climb_tree,	"Valley of Ilopheth"},
	{	"climb",		"verb_prog_sidhe_climb_vine",	(void *)verb_prog_sidhe_climb_vine,	"Sidhe Village" },
	{	"touch",		"verb_prog_antava_touch_hand",	(void *)verb_prog_antava_touch_hand,	"Antava Marsh"},
	{	"pull",			"verb_prog_pull_book",			(void *)verb_prog_pull_book,		"Cimar"},
	{	"look",			"verb_prog_look_topbounties",		(void *)verb_prog_look_topbounties,	"Cimar"},
	{	"l",			"verb_prog_look_topbounties",		(void *)verb_prog_look_topbounties,	"Cimar"},
	{	"lo",                    "verb_prog_look_topbounties",           (void *)verb_prog_look_topbounties,     "Cimar"},
	{   "loo",                    "verb_prog_look_topbounties",           (void *)verb_prog_look_topbounties,     "Cimar"}, 
	{	"pour",			"verb_prog_pour_wine",			(void *)verb_prog_pour_wine,		"Cimar"},
	{	"attach",		"verb_prog_attach_weapon",		(void *)verb_prog_attach_weapon,	"Cimar"},
	{	"join",			"verb_prog_join_guild",			(void *)verb_prog_join_guild,		"Cimar"},
	{	"pull",			"verb_prog_pull_lever",			(void *)verb_prog_pull_lever,		"None"},
	{	"pull",			"verg_prog_iseldheim_lever_pull",	(void *)verb_prog_iseldheim_lever_pull, "None"},
	{	"turn",			"verb_prog_turn_wheel",			(void *)verb_prog_turn_wheel,		"None"},
	{	"tie",			"verb_prog_tie_rope",			(void *)verb_prog_tie_rope,		"None"},
	{	"tilt",			"verb_prog_tilt_bust",			(void *)verb_prog_tilt_bust,		"None"},
	{	"tilt",			"verb_prog_tilt_scales",		(void *)verb_prog_tilt_scales,		"None"},
	{	"roll",			"verb_prog_roll_tablet",		(void *)verb_prog_roll_tablet,		"None"},
	{	"throw",		"verb_prog_throw_net",			(void *)verb_prog_throw_net,		"None"},
	{	"check",		"verb_prog_check_bounties",		(void *)verb_prog_check_bounties,	"None"},
	{	"place",		"verb_prog_place_star",			(void *)verb_prog_place_star,		"None"},
	{	"turn",			"verb_prog_turn_wyntran",		(void *)verb_prog_turn_wyntran,		"The Elven Stronghold"},
	{	"climb",		"verb_prog_fallendesert_climb_ladder",	(void *)verb_prog_fallendesert_climb_ladder,"Fallen Desert" },
	{	NULL,			NULL,						NULL,					NULL},
};

void iprog_set(OBJ_INDEX_DATA *obj,const char *progtype, const char *name)
{
    int i;
    for(i = 0; iprog_table[i].name != NULL; i++)
        if(!str_cmp(iprog_table[i].name, name))
            break;
    if(!iprog_table[i].name)
        return bug("Load_improgs: 'I': Function not found for vnum %d",obj->vnum);

    if (!str_cmp(progtype, "wear_prog"))
    {
        obj->iprogs->wear_prog = (IPROG_FUN_WEAR *)iprog_table[i].function;
	if(obj->iprogs->wear_name)
        free_pstring(obj->iprogs->wear_name);
        obj->iprogs->wear_name = palloc_string(name);
        SET_BIT(obj->progtypes, IPROG_WEAR);
        return;
    }
    if (!str_cmp(progtype, "remove_prog"))
    {
        obj->iprogs->remove_prog = (IPROG_FUN_REMOVE *)iprog_table[i].function;
	if(obj->iprogs->remove_name)
        free_pstring(obj->iprogs->remove_name);
        obj->iprogs->remove_name = palloc_string(name);
        SET_BIT(obj->progtypes, IPROG_REMOVE);
        return;
    }
    if (!str_cmp(progtype, "get_prog"))
    {
        obj->iprogs->get_prog = (IPROG_FUN_GET *)iprog_table[i].function;
	if(obj->iprogs->get_name)
        free_pstring(obj->iprogs->get_name);
        obj->iprogs->get_name = palloc_string(name);
        SET_BIT(obj->progtypes, IPROG_GET);
        return;
    }
    if (!str_cmp(progtype, "loot_prog")) /* Returning FALSE prevents taking */
    {
        obj->iprogs->loot_prog = (IPROG_FUN_LOOT *)iprog_table[i].function;
	if(obj->iprogs->loot_name)
        free_pstring(obj->iprogs->loot_name);
        obj->iprogs->loot_name = palloc_string(name);
        SET_BIT(obj->progtypes, IPROG_LOOT);
        return;
    }
    if (!str_cmp(progtype, "drop_prog"))
    {
        obj->iprogs->drop_prog = (IPROG_FUN_DROP *)iprog_table[i].function;
	if(obj->iprogs->drop_name)
        free_pstring(obj->iprogs->drop_name);
        obj->iprogs->drop_name = palloc_string(name);
        SET_BIT(obj->progtypes, IPROG_DROP);
        return;
    }
    if (!str_cmp(progtype, "sac_prog"))
    {
        obj->iprogs->sac_prog = (IPROG_FUN_SAC *)iprog_table[i].function;
        free_pstring(obj->iprogs->sac_name);
        obj->iprogs->sac_name = palloc_string(name);
        SET_BIT(obj->progtypes, IPROG_SAC);
        return;
    }
    if (!str_cmp(progtype, "entry_prog"))
    {
        obj->iprogs->entry_prog = (IPROG_FUN_ENTRY *)iprog_table[i].function;
	if(obj->iprogs->entry_name)
        free_pstring(obj->iprogs->entry_name);
        obj->iprogs->entry_name = palloc_string(name);
        SET_BIT(obj->progtypes, IPROG_ENTRY);
        return;
    }
    if (!str_cmp(progtype, "give_prog"))
    {
        obj->iprogs->give_prog = (IPROG_FUN_GIVE *)iprog_table[i].function;
	if(obj->iprogs->give_name)
        free_pstring(obj->iprogs->give_name);
        obj->iprogs->give_name = palloc_string(name);
        SET_BIT(obj->progtypes, IPROG_GIVE);
        return;
    }
    if (!str_cmp(progtype, "greet_prog"))
    {
        obj->iprogs->greet_prog = (IPROG_FUN_GREET *)iprog_table[i].function;
	if(obj->iprogs->greet_name)
        free_pstring(obj->iprogs->greet_name);
        obj->iprogs->greet_name = palloc_string(name);
        SET_BIT(obj->progtypes, IPROG_GREET);
        return;
    }
    if (!str_cmp(progtype, "fight_prog"))
    {
        obj->iprogs->fight_prog = (IPROG_FUN_FIGHT *)iprog_table[i].function;
	if(obj->iprogs->fight_name)
        free_pstring(obj->iprogs->fight_name);
        obj->iprogs->fight_name = palloc_string(name);
        SET_BIT(obj->progtypes, IPROG_FIGHT);
        return;
    }
    if (!str_cmp(progtype, "death_prog")) /* returning TRUE prevents death */
    {
        obj->iprogs->death_prog = (IPROG_FUN_DEATH *)iprog_table[i].function;
	if(obj->iprogs->death_name)
        free_pstring(obj->iprogs->death_name);
        obj->iprogs->death_name = palloc_string(name);
        SET_BIT(obj->progtypes, IPROG_DEATH);
        return;
    }
    if (!str_cmp(progtype, "speech_prog"))
    {
        obj->iprogs->speech_prog = (IPROG_FUN_SPEECH *)iprog_table[i].function;
	if(obj->iprogs->speech_name)
        free_pstring(obj->iprogs->speech_name);
        obj->iprogs->speech_name = palloc_string(name);
        SET_BIT(obj->progtypes, IPROG_SPEECH);
        return;
    }
    if (!str_cmp(progtype, "pulse_prog"))
    {
        obj->iprogs->pulse_prog = (IPROG_FUN_PULSE *)iprog_table[i].function;
	if(obj->iprogs->pulse_name)
        free_pstring(obj->iprogs->pulse_name);
        obj->iprogs->pulse_name = palloc_string(name);
        SET_BIT(obj->progtypes, IPROG_PULSE);
        return;
    }
    if (!str_cmp(progtype, "invoke_prog"))
    {
        obj->iprogs->invoke_prog = (IPROG_FUN_INVOKE *)iprog_table[i].function;
	if(obj->iprogs->invoke_name)
        free_pstring(obj->iprogs->invoke_name);
        obj->iprogs->invoke_name = palloc_string(name);
        SET_BIT(obj->progtypes, IPROG_INVOKE);
        return;
    }
    if (!str_cmp(progtype, "open_prog"))
    {
        obj->iprogs->open_prog = (IPROG_FUN_OPEN *)iprog_table[i].function;
	if(obj->iprogs->open_name)
	        free_pstring(obj->iprogs->open_name);
        obj->iprogs->open_name = palloc_string(name);
        SET_BIT(obj->progtypes, IPROG_OPEN);
        return;
    }
    if (!str_cmp(progtype, "look_prog"))
    {
        obj->iprogs->look_prog = (IPROG_FUN_LOOK *)iprog_table[i].function;
	if(obj->iprogs->look_name)
	        free_pstring(obj->iprogs->look_name);
        obj->iprogs->look_name = palloc_string(name);
        SET_BIT(obj->progtypes, IPROG_OPEN);
        return;
    }
    if (!str_cmp(progtype, "verb_prog"))
    {
        obj->iprogs->verb_prog = (IPROG_FUN_VERB *)iprog_table[i].function;
	if(obj->iprogs->verb_name)
	        free_pstring(obj->iprogs->verb_name);
        obj->iprogs->verb_name = palloc_string(name);
        SET_BIT(obj->progtypes, IPROG_VERB);
        return;
    }
	if (!str_cmp(progtype, "hit_prog"))
	{
		obj->iprogs->hit_prog = (IPROG_FUN_HIT *)iprog_table[i].function;
		free_pstring(obj->iprogs->hit_name);
		obj->iprogs->hit_name = palloc_string(name);
		SET_BIT(obj->progtypes, IPROG_HIT);
		return;
	}
    bug( "Load_improgs: 'I': invalid program type for vnum %d",obj->vnum);
    exit(1);
 }

bool iprog_unset(OBJ_INDEX_DATA *obj,const char *progtype, const char *name)
{
	int i;
	for(i = 0; iprog_table[i].name != NULL; i++)
		if(!str_cmp(iprog_table[i].name, name))
			break;

	if(!iprog_table[i].name)
		return FALSE;

	if (!str_cmp(progtype, "wear_prog"))
	{
		obj->iprogs->wear_prog = NULL;
		free_pstring(obj->iprogs->wear_name);
		REMOVE_BIT(obj->progtypes, IPROG_WEAR);
	}
	else if (!str_cmp(progtype, "remove_prog"))
	{
		obj->iprogs->remove_prog = NULL;
		free_pstring(obj->iprogs->remove_name);
		REMOVE_BIT(obj->progtypes, IPROG_REMOVE);
	}
	else if (!str_cmp(progtype, "get_prog"))
	{
		obj->iprogs->get_prog = NULL;
		free_pstring(obj->iprogs->get_name);
		REMOVE_BIT(obj->progtypes, IPROG_GET);
	}
	else if (!str_cmp(progtype, "loot_prog")) /* Returning FALSE prevents taking */
	{
		obj->iprogs->loot_prog = NULL;
		free_pstring(obj->iprogs->loot_name);
		REMOVE_BIT(obj->progtypes, IPROG_LOOT);
	}
	else if (!str_cmp(progtype, "drop_prog"))
	{
		obj->iprogs->drop_prog = NULL;
		free_pstring(obj->iprogs->drop_name);
		REMOVE_BIT(obj->progtypes, IPROG_DROP);
	}
	else if (!str_cmp(progtype, "sac_prog"))
	{
		obj->iprogs->sac_prog = NULL;
		free_pstring(obj->iprogs->sac_name);
		REMOVE_BIT(obj->progtypes, IPROG_SAC);
	}
	else if (!str_cmp(progtype, "entry_prog"))
	{
		obj->iprogs->entry_prog = NULL;
		free_pstring(obj->iprogs->entry_name);
		REMOVE_BIT(obj->progtypes, IPROG_ENTRY);
	}
	else if (!str_cmp(progtype, "give_prog"))
	{
		obj->iprogs->give_prog = NULL;
		free_pstring(obj->iprogs->give_name);
		REMOVE_BIT(obj->progtypes, IPROG_GIVE);
	}
	else if (!str_cmp(progtype, "greet_prog"))
	{
		obj->iprogs->greet_prog = NULL;
		free_pstring(obj->iprogs->greet_name);
		REMOVE_BIT(obj->progtypes, IPROG_GREET);
	}
	else if (!str_cmp(progtype, "fight_prog"))
	{
		obj->iprogs->fight_prog = NULL;
		free_pstring(obj->iprogs->fight_name);
		REMOVE_BIT(obj->progtypes, IPROG_FIGHT);
	}
	else if (!str_cmp(progtype, "death_prog")) /* returning TRUE prevents death */
	{
		obj->iprogs->death_prog = NULL;
		free_pstring(obj->iprogs->death_name);
		REMOVE_BIT(obj->progtypes, IPROG_DEATH);
	}
	else if (!str_cmp(progtype, "speech_prog"))
	{
		obj->iprogs->speech_prog = NULL;
		free_pstring(obj->iprogs->speech_name);
		REMOVE_BIT(obj->progtypes, IPROG_SPEECH);
	}
	else if (!str_cmp(progtype, "pulse_prog"))
	{
		obj->iprogs->pulse_prog = NULL;
		free_pstring(obj->iprogs->pulse_name);
		REMOVE_BIT(obj->progtypes, IPROG_PULSE);
	}
	else if (!str_cmp(progtype, "invoke_prog"))
	{
		obj->iprogs->invoke_prog = NULL;
		free_pstring(obj->iprogs->invoke_name);
		REMOVE_BIT(obj->progtypes, IPROG_INVOKE);
	}
	else if (!str_cmp(progtype, "open_prog"))
	{
		obj->iprogs->open_prog = NULL;
		free_pstring(obj->iprogs->open_name);
		REMOVE_BIT(obj->progtypes, IPROG_OPEN);
	}
	if (!str_cmp(progtype, "look_prog"))
	{
		obj->iprogs->look_prog = NULL;
		free_pstring(obj->iprogs->look_name);
		REMOVE_BIT(obj->progtypes, IPROG_OPEN);
	}
	if (!str_cmp(progtype, "verb_prog"))
	{
		obj->iprogs->verb_prog = NULL;
		free_pstring(obj->iprogs->verb_name);
		REMOVE_BIT(obj->progtypes, IPROG_VERB);
	}
	if (!str_cmp(progtype, "hit_prog"))
	{
		obj->iprogs->hit_prog = NULL;
		free_pstring(obj->iprogs->hit_name);
		REMOVE_BIT(obj->progtypes, IPROG_HIT);
	}
	return TRUE;
}

/* ITEM FUNCTIONS */
void fight_prog_drow_talisman(OBJ_DATA *obj, CHAR_DATA *ch)
{
	if(!is_worn(obj) || number_percent() > 7 || !ch->fighting)
		return;
	act("$p pulses and glows a sickly shade of green!", ch, obj, 0, TO_ALL);
	if(number_percent()>50)
		obj_cast_spell(skill_lookup("frost breath"),ch->level,ch,ch->fighting,obj);
	else
		obj_cast_spell(skill_lookup("acid blast"),60,ch,ch->fighting,obj);
}

void fight_prog_devils_eye(OBJ_DATA *obj, CHAR_DATA *ch)
{

	if(!is_worn(obj))
		return;

	if(number_percent() < 5)
	{
		act("The Devil's Eye suddenly pivots and stares directly into $n's face!", ch, 0, ch->fighting, TO_NOTVICT);
		act("The Devil's Eye suddenly pivots and stares directly into your face!", ch, 0, ch->fighting, TO_VICT);
		damage_new(ch,ch->fighting,dice(12,12),TYPE_UNDEFINED,DAM_SLASH,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"heartquake");
	}

	return;
}

void fight_prog_skean(OBJ_DATA *obj, CHAR_DATA *ch)
{
	int percent;
	AFFECT_DATA af;
	
	if(!is_worn(obj))
		return;

	if(number_percent() < 6)
	{
		percent = number_percent();
		if(ch->fighting->Class()->ctype == CLASS_COMMUNER && !is_affected(ch->fighting, gsn_severed))
		{
			act("An enchanted obsidian skean blazes through the air, leaving a trail of fire!", ch, 0, ch->fighting, TO_ROOM);
			act("Your vital ties to the gods' empowerment have been severed!", ch, 0, ch->fighting, TO_VICT);
			init_affect(&af);
			af.where = TO_AFFECTS;
			af.aftype = AFT_SKILL;
			af.type = gsn_severed;
			af.level = ch->level;
			af.modifier = 10 + dice(1,8);
			af.duration = dice(1,2);
			new_affect_to_char( ch->fighting, &af );
			return;
		}
	
		if(!IS_SET(ch->affected_by, AFF_BLIND))
		{
			act("An enchanted obsidian skean darts toward your eyes, gouging you painfully!", ch, 0, ch->fighting, TO_VICT);
			act("An enchanted obsidian skean darts toward $N's eyes, gouging $M painfully!", ch, 0, ch->fighting, TO_NOTVICT);
			init_affect(&af);
			af.where = TO_AFFECTS;
			af.aftype = AFT_SKILL;
			af.level = ch->level;
			af.type = gsn_bleeding;
			SET_BIT(af.bitvector, AFF_BLIND);
			af.duration = dice(1, 4);
			new_affect_to_char( ch->fighting, &af );
			return;
		}

		if(IS_GOOD(ch->fighting))
		{
			act("You swoon for a moment, and the world goes slightly hazy...", ch->fighting, 0, 0, TO_CHAR);
			WAIT_STATE(ch->fighting, PULSE_VIOLENCE * 2);
			return;
		}
	}

	return;
}

void fight_prog_cankerworm(OBJ_DATA *obj, CHAR_DATA *ch)
{
	int dam;
	
	if(!is_worn(obj))
		return;

	if(number_percent() < 10)
	{
		dam = dice((ch->level), 3);
		act("The Cankerworm leaps from your hands momentarily!", ch, 0, 0, TO_CHAR);
		act("The Cankerworm seems to leap from $n's hands momentarily!", ch, 0, 0, TO_ROOM);
		damage_new(ch,ch->fighting,dam,TYPE_UNDEFINED,DAM_SLASH,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"burrowing axe");
	}
	return;
}

void fight_prog_axe_trelaran(OBJ_DATA *obj, CHAR_DATA *ch)
{
	OBJ_DATA *crown;

	if (!is_worn(obj))
		return;
	
	if(str_cmp(race_table[ch->race].name,"duergar"))
	{
		act("Your hand twists in agony as $p glows midnight black!", ch, obj, 0, TO_CHAR);
		act("$n's hand twists in agony as $p tries to writhe free!", ch, obj, 0, TO_ROOM);
		obj_cast_spell(skill_lookup("curse"),60,ch,ch,obj);
		obj_cast_spell(skill_lookup("plague"),60,ch,ch,obj);
		obj_cast_spell(skill_lookup("poison"),60,ch,ch,obj);
	}
	else if((crown = get_eq_char(ch, WEAR_HEAD)) != NULL)
	{
		if(number_percent() < 6 && (crown->pIndexData->vnum == 21825))
		{
			act("$p suddenly shifts to the likeness of a gaping dragon head, as it spits out a blast of acid!", ch, obj, 0, TO_ALL);
			obj_cast_spell(skill_lookup("acid blast"), ch->level, ch, ch->fighting, obj);
		}
	}
}

void fight_prog_cure_critical(OBJ_DATA *obj, CHAR_DATA *ch)
{
    if (number_percent() < 8 && is_worn(obj))
        {
        act("$n's $p glows bright blue!",ch,obj,0,TO_ROOM);
        act("$p glows bright blue!",ch,obj,0,TO_CHAR);
        obj_cast_spell(skill_lookup("cure critical"), obj->level, ch, ch, obj);
        }
}

void pulse_prog_steal(OBJ_DATA *obj, bool isTick)
{
	CHAR_DATA *vch, *ch = obj->carried_by;
	OBJ_DATA *stolen=NULL;
	bool dbreak=FALSE;
	int invnum = number_range(0,5), i = 0;
	if(!ch || !is_worn(obj) || ch->position!=POS_STANDING)
		return;
	for(vch=ch->in_room->people; vch!=NULL; vch = vch->next_in_room)
	{
		if(number_percent()<98 || vch==ch)
			continue;
    		for ( stolen = vch->carrying; stolen != NULL; stolen = stolen->next_content )
    		{
			if ( stolen->wear_loc == WEAR_NONE
			&&   (can_see_obj( ch, stolen))
			&& (invnum==i || !stolen->next_content))
			{
			dbreak=TRUE;
            		break;
			}
			i++;
    		}
	if(dbreak)
		break;
	}
	if(!stolen || !vch)
		return;
	send_to_char("You grin evilly as a sudden urge takes hold of you.\n\r",ch);
	act("Slipping into $N's pockets, you carefully slide $p out.",ch,stolen,vch,TO_CHAR);
	if(number_percent()>95 && vch->position!=POS_SLEEPING)
	{
		do_myell(vch,"Keep your hands out of there!", ch);
		send_to_char("Oops.\n\r",ch);
		multi_hit(vch,ch,TYPE_UNDEFINED);
		return;
	}
	obj_from_char(stolen);
	obj_to_char(stolen,ch);
}
	
void invoke_prog_tattoo_dioxide(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;

        return;

	obj = get_eq_char(ch, WEAR_BRAND);
      act("$n's $p glows blue.",ch,obj,0,TO_ROOM);
      act("Your $p glows blue.",ch,obj,0,TO_CHAR);
	
	ch->hit += (ch->level*2) * 8;
      ch->hit = UMIN(ch->hit,ch->max_hit);
      send_to_char("You feel MUCH better.\n\r",ch);
      act("$n looks much better.",ch,NULL,NULL,TO_ROOM);

	if (check_dispel(ch->level+3,ch,skill_lookup("blindness")))
	{
		act("$n is no longer blinded.",ch,NULL,NULL,TO_ROOM);
	}
	if (check_dispel(ch->level+3,ch,skill_lookup("curse")))
	{
		act("$n is no longer cursed.",ch,NULL,NULL,TO_ROOM);
	}
	if (check_dispel(ch->level+3,ch,skill_lookup("faerie fire")))
	{
		act("$n's pink outline fades.",ch,NULL,NULL,TO_ROOM);
	}
	if (check_dispel(ch->level+3,ch,skill_lookup("slow")))
	{
		act("$n is no longer moving so slowly.",ch,NULL,NULL,TO_ROOM);
	}
	if (check_dispel(ch->level+3,ch,skill_lookup("weaken")))
	{
		act("$n looks stronger.",ch,NULL,NULL,TO_ROOM);
	}
	if (check_dispel(ch->level+3,ch,skill_lookup("poison")))
	{
		act("$n looks less ill.",ch,NULL,NULL,TO_ROOM);
	}
	if (check_dispel(ch->level+3,ch,skill_lookup("plague")))
	{
		act("$n's sores vanish.",ch,NULL,NULL,TO_ROOM);
	}
	if (check_dispel(ch->level+3,ch,skill_lookup("windwall")))
	{
		act("$n's is no longer blinded by dust and dirt.",ch,NULL,NULL,TO_ROOM);
	}

      send_to_char("Your pulse races fiercely as you burn with raging fury!\n\r",ch);
      act("$n gets a furious look in $s eyes as $e burns with raging fury!",ch,NULL,NULL,TO_ROOM);
      init_affect(&af);
      af.where        = TO_AFFECTS;
	af.aftype       = AFT_SKILL;
//      af.type         = gsn_rage_of_dioxide;
      af.level        = ch->level;
      af.duration     = 10;
      af.modifier     = ch->level/2;
      af.location     = APPLY_HITROLL;
      affect_to_char(ch,&af);
      af.location     = APPLY_DAMROLL;
      affect_to_char(ch,&af);
      return;

}

void invoke_prog_tattoo_jackass(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	obj = get_eq_char(ch, WEAR_BRAND);
      act("$n's $p doesn't glow much due to $s stupidity.",ch,obj,0,TO_ROOM);
      act("Your $p doesn't glow much, probably because you're a jackass.",ch,obj,0,TO_CHAR);
      send_to_char("You smite yourself! What a jackass!\n\r",ch);
      act("$n smites $mself! What a jackass!",ch,NULL,NULL,TO_ROOM);
	ch->hit /= 2;
	return;
}

void invoke_prog_tattoo_morglum(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	return;
}

void invoke_prog_tattoo_calenduil(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	if(strstr(argument,"jackal"))
	{
	//ZHERE
        	AFFECT_DATA af;
        	CHAR_DATA *vch;
        	int found = 0;
        	if(is_affected(ch,gsn_jackal))
                	return send_to_char("Greed taken too far can bring unpleasant results.\n\r",ch);
        	init_affect(&af);
        	af.where        = TO_AFFECTS;
        	af.aftype       = AFT_INVIS;
        	af.type         = gsn_jackal;
        	af.level        = ch->level;
        	af.location     = APPLY_DAM_MOD;
        	af.modifier     = (ch->level / (short)2.5);
        	af.duration     = ch->level + 10;
        	for(vch = ch->in_room->people; vch; vch = vch->next_in_room)
                if(vch != ch && is_same_group(ch,vch) && !is_affected(vch,gsn_jackal))
                {
                        found++;
                        affect_to_char(vch,&af);
			vch->hit = vch->max_hit;
                }
        	if(!found)
        	        return send_to_char("The Jester will only grant you power at the expense of others.\n\r",ch);
		af.location = APPLY_HIT;
        	af.modifier = (int)(af.modifier * found * (float).7);
        	af.duration = ch->level + 10;
       		affect_to_char(ch,&af);
		ch->hit = ch->hit + af.modifier;
        	send_to_char("Power courses through you as the Jester strengthens you at the expense of your companions!\n\r",ch);
	        WAIT_STATE(ch, PULSE_VIOLENCE);
	}
}

void invoke_prog_tattoo_sceptre(OBJ_DATA *obj, CHAR_DATA *ch, char *argument) {
AFFECT_DATA af;
if (is_affected(ch,gsn_sceptret)) {
		send_to_char("You can't heal yourself again yet.\n\r",ch);
		return;
}
		send_to_char("Your body heals at great cost to your abilities.\n\r",ch);
		act("$n looks better, but weakened.",ch,0,0,TO_ROOM);
		ch->hit = ch->hit + 500;
		init_affect(&af);
		af.where = TO_AFFECTS;
		af.type = gsn_sceptret;
		af.aftype = AFT_POWER;
		af.level = ch->level;
		af.duration = 3;
		af.location = APPLY_DAMROLL;
		af.modifier = -50;
		SET_BIT(af.bitvector, AFF_SLOW);
		affect_to_char(ch, &af);
		af.location = APPLY_MANA;
		af.modifier = -500;
		affect_to_char(ch, &af);
		WAIT_STATE(ch,PULSE_VIOLENCE);
}
void invoke_prog_tattoo_zethus(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;
	obj = get_eq_char(ch,WEAR_BRAND);
	if(!is_worn(obj))
		return;
	if(ch->position != POS_FIGHTING)
		return send_to_char("You must be fighting to invoke the mandala.",ch);
	if(obj->value[0] < 1)
		return send_to_char("Your tattoo is not energized.\n\r",ch);
	act("$n concentrates intently for a moment.",ch,0,0,TO_ROOM);
	obj->value[0] -= 1;
	if(!saves_spell(ch->level + dice(1,5),ch->fighting,DAM_NEGATIVE))
	{
		act("A flash of darkness arcs between $n and $N, leaving $N looking hopeless!",ch,0,ch->fighting,TO_NOTVICT);
		act("A flash of darkness arcs between you and $N as you sever $N's link with $S god.",ch,0,ch->fighting,TO_CHAR);
		act("Negative energy crackles in the air around you, temporarily severing your link with your god!",ch,0,ch->fighting,TO_VICT);
		damage_new(ch,ch->fighting,ch->level + 15,TYPE_UNDEFINED,DAM_NEGATIVE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT, "energy wave");
		damage_new(ch,ch,ch->level - 15,TYPE_UNDEFINED,DAM_INTERNAL,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT, "exertion");
		init_affect(&af);
		af.where = TO_AFFECTS;
		af.type = gsn_severed;
		af.duration = ch->level/12;
		af.aftype = AFT_POWER;
		af.location = APPLY_WIS;
		af.modifier = -(ch->level/5);
		new_affect_to_char(ch->fighting,&af);
	}
ch->mana = ch->mana - 50;
WAIT_STATE(ch,PULSE_VIOLENCE * 2);
return;
}


void entry_prog_explosives(OBJ_DATA *obj)
{
	if (!is_worn(obj))
		return;

	if (obj->timer == 0)
		return;

	if (obj->carried_by != NULL)
	{
		act("You hear soft ticking from somewhere.",obj->carried_by, NULL, NULL,TO_ROOM);
	}
	return;
}

bool death_prog_explosives(OBJ_DATA *obj, CHAR_DATA *ch)
{
	if(!is_worn(obj))
		return FALSE;

	bag_explode(ch,obj,2);

	return FALSE;
}

/* explosive packs -- Dioxide */

void invoke_prog_explosives(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	int fuse;

	if (obj->timer > 0)
	{
		send_to_char("The fuse is already set.\n\r",ch);
		return;
	}
	if (is_number(argument))
	{
		fuse = atoi(argument);
		if (fuse < 1 || fuse > 48) {
			if (fuse == 0)
			{
				act("$n sets $p and stuffs it in $s mouth!",ch,obj,0,TO_ROOM);
				act("You set $p and stuff it in your mouth!",ch,obj,0,TO_CHAR);
				unequip_char(ch,obj,TRUE);
				bag_explode(ch,obj,3);
				return;
			}
			act("$n fumbles awkwardly with $p, and it begins to tick.",ch,obj,0,TO_ROOM);
			act("You fumble awkwardly with $p, and it begins to tick.",ch,obj,0,TO_CHAR);
		} else {
			act("$n sets the fuse of $p, and it begins to tick.",ch,obj,0,TO_ROOM);
			act("You set the fuse of $p, and it begins to tick.",ch,obj,0,TO_CHAR);
			obj->timer = fuse;
			return;
		}
	} else {
		act("$n fumbles awkwardly with $p, and it begins to tick.",ch,obj,0,TO_ROOM);
		act("You fumble awkwardly with $p, and it begins to tick.",ch,obj,0,TO_CHAR);
	}

	fuse = number_range(2,48);
	obj->timer = fuse;

	return;
}

void speech_prog_ice_dragon_statue(OBJ_DATA *obj, CHAR_DATA *ch, char *speech)
{
	CHAR_DATA *dragon;
	if(str_cmp(speech,"Valnian"))
		return;
	dragon = create_mobile(get_mob_index(1617));
	if(!dragon || !is_worn(obj))
		return;
	char_to_room(dragon, ch->in_room);
	if(ch->level<40)
	{
		do_say(dragon,"You dare to call upon me, weakling?!");
		multi_hit(dragon,ch,TYPE_UNDEFINED);
		extract_obj(obj);
		return;
	}
	act("$p shatters violently in a cloud of ice. As it clears, a dragon of pure ice hovers over you.",
		dragon,obj,0,TO_ROOM);
	add_follower(dragon, ch);
	dragon->leader = ch;
	SET_BIT(dragon->affected_by, AFF_CHARM);
	extract_obj(obj);
}

void fight_prog_scales(OBJ_DATA *obj, CHAR_DATA *ch)
{
	CHAR_DATA *victim = ch->fighting;

	switch(obj->value[2])
	{
		case(0):
			if(number_percent() > 91)
			{
				act("Ethereal green tendrils reach out of your tattoo and lash out at $N!",ch,obj,victim,TO_CHAR);
				act("Ethereal green tendrils reach out of $n's tattoo and lash out at you!",ch,obj,victim,TO_VICT);
				act("Ethereal green tendrils reach out of $n's tattoo and lash out at $N!",ch,obj,victim,TO_ROOM);
				damage_new(ch,victim,ch->level,TYPE_UNDEFINED,DAM_TRUESTRIKE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"lashing tendrils");
			}
			break;
		case(1):
			if(number_percent() > 85 && !IS_NPC(victim) && !IS_IMMORTAL(victim))
			{
				act("$N's $p begins to glow, and you feel drained.",victim,obj,ch,TO_VICT);				       act("Your $p begins to glow as it drains energy from $n.",ch,obj,victim,TO_CHAR);
				victim->pcdata->condition[COND_HUNGER]=5;
				victim->pcdata->condition[COND_THIRST]=5;
			}
			break;
		case(2):
			if(number_percent() > 85)
			{
				act("$p throbs momentarily, and you feel invigorated!",ch,obj,0,TO_CHAR);
				act("$n looks a bit more refreshed.",ch,obj,0,TO_ROOM);
				if(number_percent() > 50)
					ch->hit += ch->level;
				else
					ch->mana += ch->level;
			}
			break;
			
	}
return;
}
void fight_prog_amber_medallion(OBJ_DATA *obj, CHAR_DATA *ch)
{
	int intel;
        if(!is_worn(obj) || number_percent()<92 || ch->mana>=ch->max_mana || IS_NPC(ch))
                return;
	intel = get_curr_stat(ch, STAT_INT);
	act("$p glows softly and you feel refreshed.",ch,obj,0,TO_CHAR);
	act("$p glows softly.",ch,obj,0,TO_ROOM);
        ch->mana = UMIN(ch->mana + number_range(intel,intel * 2), ch->max_mana);
}

bool give_prog_cabal_item(OBJ_DATA *obj, CHAR_DATA *from, CHAR_DATA *to)
{
	char buf[MSL];
	if(!IS_CABAL_GUARD(to) || IS_IMMORTAL(to))
		return FALSE;
	if(from->cabal!=to->cabal)
	{
		do_say(to,"I want nothing from the likes of you.");
		return TRUE;
	}
	cabal_shudder(obj->pIndexData->cabal, TRUE);
        sprintf(buf,"%s has given me %s.",PERS(from,to),obj->short_descr);
	do_cb(to,buf);
	obj_from_char(obj);
	obj_to_char(obj, to);
	save_cabal_items();
	return TRUE;
}

void get_prog_bad_idea(OBJ_DATA *obj, CHAR_DATA *ch)
{
	AFFECT_DATA af;
	EXIT_DATA *pexit = ch->in_room->exit[DIR_UP];
	CHAR_DATA *vch;
	act("As you pick up $p, a booming voice can be heard.",ch,obj,0,TO_CHAR);
	act("As $n picks up $p, a booming voice can be heard.",ch,obj,0,TO_ROOM);
	act("'Stealing from the Gods, are we?  Ah, yes.  I like that.  Very daring.  Good taste.  Pity I like $p.'\n\r",ch,obj,0,TO_CHAR);
	act("'Stealing from the Gods, are we?  Ah, yes.  I like that.  Very daring.  Good taste.  Pity I like $p.'",ch,obj,0,TO_ROOM);
	act("$p grows searing hot and you are forced to drop it!",ch,obj,0,TO_CHAR);
	act("$p glows searing white and $n is forced to drop it!",ch,obj,0,TO_ROOM);
	obj_from_char(obj);
	obj_to_room(obj,ch->in_room);
	if(!pexit->u1.to_room)
	{
	init_affect(&af);
	af.where	= TO_AFFECTS;
	af.aftype	= AFT_MALADY;
	af.type		= gsn_plague;
	af.owner	= ch;
	af.level	= 50;
	af.duration	= 30;
	af.location	= APPLY_STR;
	af.modifier	= -100;
	af.tick_fun	= plague_tick;
	af.end_fun	= NULL;
	SET_BIT(af.bitvector, AFF_PLAGUE);
	affect_to_char(ch, &af);
	send_to_char("You feel very sick.  Clearly you've made someone angry.\n\r",ch);
	act("$n looks extremely ill.",ch,0,0,TO_ROOM);
	}
	else
	{
		act("'I believe you've earned a rest from your hard exertions, and a chance to enjoy the spoils of your victory.'",ch,0,0,TO_ALL);
		pexit->u1.to_room = NULL;
		init_affect(&af);
	        af.where        = TO_AFFECTS;
        	af.aftype       = AFT_MALADY;
        	af.type         = gsn_imprisonvoice;
        	af.owner        = ch;
        	af.level        = 99;
        	af.duration     = -1;
        	af.location     = APPLY_NONE;
        	af.modifier     = 0;
        	af.tick_fun     = NULL;
        	af.end_fun      = NULL;
 		SET_BIT(af.bitvector, AFF_DETECT_GOOD);
		for(vch = ch->in_room->people; vch; vch = vch->next_in_room)
			affect_to_char(vch, &af);
	}
}

void greet_prog_corpse_explode(OBJ_DATA *obj, CHAR_DATA *ch)
{
	CHAR_DATA *owner;
	
	for(owner=char_list;owner != NULL;owner=owner->next)
		if(!IS_NPC(owner) && (!str_cmp(owner->true_name,obj->owner)))
			break;
	
	if(owner == NULL)
		return;
	
	if (owner == ch)
		return;
	
	if(is_safe_new(ch,owner,FALSE))
		return;
	act("As $n enters, $p starts to shudder violently, then explodes!",ch,obj,0,TO_NOTVICT);
	act("As you enter, $p starts to shudder violently, then explodes!",ch,obj,0,TO_CHAR);
	damage_new(owner,ch,dice(obj->level,2),TYPE_UNDEFINED,DAM_ENERGY,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,
		"the exploding corpse*");
	extract_obj(obj);
}

void fight_prog_horde_bull(OBJ_DATA *obj, CHAR_DATA *ch)
{
	if(!is_affected(ch, gsn_rage) || !ch->fighting || number_percent()> (.1 * get_skill(ch,gsn_rage)))
		return;
	if(ch->hit >= ch->max_hit)
		return;
	send_to_char("The fortitude of the Bull fills you, inspiring you to ignore your pain and fight on.\n\r",ch);
	act("$n's muscles ripple with fresh vigor as $s eyes harden with new resolve.",ch,0,0,TO_ROOM);
	ch->hit=UMIN(ch->max_hit,ch->hit+number_range(ch->level*(short)1.2, ch->level * (short)2.2));
}

void fight_prog_horde_bear(OBJ_DATA *obj, CHAR_DATA *ch)
{
        if(!is_affected(ch, gsn_rage) || !ch->fighting || number_percent()> (.15 * get_skill(ch,gsn_rage)))
                return;
	act("The rage of the Bear roars through you as you charge into $N, sending $m sprawling!",ch,0,ch->fighting,TO_CHAR);
	act("$n gets a wild look in $s eyes, charging into you and sending you sprawling!",ch,0,ch->fighting,TO_VICT);
	act("$n gets a wild look in $s eyes, charging into $N and sending $m sprawling!",ch,0,ch->fighting,TO_NOTVICT);
	WAIT_STATE(ch->fighting, (int)(PULSE_VIOLENCE * 1.9));
	damage_new(ch,ch->fighting,dice(8,8),TYPE_UNDEFINED,DAM_BASH,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"charge");
}

void fight_prog_horde_lion(OBJ_DATA *obj, CHAR_DATA *ch)
{
	AFFECT_DATA af;
	if(!is_affected(ch, gsn_rage) || !ch->fighting || number_percent()> (.18 * get_skill(ch,gsn_rage)))
		return;
	act("The spirit of the Lion surges through you as you claw at $N's flesh!",ch,0,ch->fighting,TO_CHAR);
	act("$n gets a wild look in $s eyes, clawing viciously at $N!",ch,0,ch->fighting,TO_NOTVICT);
	act("$n gets a wild look in $s eyes, clawing viciously at you!",ch,0,ch->fighting,TO_VICT);
	damage_new(ch,ch->fighting,dice(ch->level - 2,4),TYPE_UNDEFINED,DAM_BASH,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,
		"raking claws$");
	if(!ch->fighting || is_affected(ch->fighting,gsn_bleeding))
		return;
	init_affect(&af);
	af.where	= TO_AFFECTS;
	af.type		= gsn_bleeding;
	af.aftype	= AFT_INVIS;
	af.location	= APPLY_NONE;
	af.modifier	= 0;
	af.level	= ch->level - 5;
	af.duration	= ch->level / 7;
	af.owner	= ch;
	af.tick_fun	= bleeding_tick;
	af.end_fun	= NULL;
	new_affect_to_char(ch->fighting, &af);
	send_to_char("Blood begins to gush from your vicious wounds.\n\r",ch->fighting);
	act("Bright red blood begins to gush from $n's wounds.",ch->fighting,0,0,TO_ROOM);
}

void fight_prog_horde_wolf(OBJ_DATA *obj, CHAR_DATA *ch)
{
	AFFECT_DATA af;

	if(!is_affected(ch, gsn_rage) || !ch->fighting || number_percent()> (.18 * get_skill(ch,gsn_rage)))
		return;
	act("The spirit of the Wolf enrages you as you leap at $N and sink your fangs into $S throat!",ch,0,ch->fighting,TO_CHAR);
	act("$n gets a wild look in $s eyes and leaps at $N, sinking $s teeth into $S throat!",ch,0,ch->fighting,TO_NOTVICT);
	act("$n gets a wild look in $s eyes and leaps at you, sinking $s teeth into your throat!",ch,0,ch->fighting,TO_VICT);
	damage_new(ch,ch->fighting,dice(ch->level,2),TYPE_UNDEFINED,DAM_BASH,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"bite");

	if (!ch->fighting)
		return;

	if(!is_affected(ch->fighting,gsn_bleeding))
	{
        	init_affect(&af);
        	af.where        = TO_AFFECTS;
        	af.type         = gsn_bleeding;
        	af.aftype       = AFT_INVIS;
        	af.location     = APPLY_NONE;
        	af.modifier     = 0;
        	af.level        = ch->level - 5;
        	af.duration     = ch->level / 10;
        	af.owner        = ch;
        	af.end_fun      = NULL;
        	new_affect_to_char(ch->fighting, &af);
	}
	if(!is_affected(ch->fighting,gsn_mangled) && number_percent() < (get_skill(ch,gsn_rage) * .28))
	{
                init_affect(&af);
                af.where        = TO_AFFECTS;
		af.type		= gsn_mangled;
		af.aftype	= AFT_MALADY;
                af.location     = APPLY_NONE;
                af.modifier     = 0;
                af.level        = ch->level - 5;
		af.duration	= ch->level / 18;
		af.owner	= ch;
		af.end_fun	= NULL;
		new_affect_to_char(ch->fighting, &af);
		act("You hear the satisfying crunch of bone as you tear deeply into $N's throat.",ch,0,ch->fighting,TO_CHAR);
		act("You hear the crunch of bone as $n tears deeply into your throat.",ch,0,ch->fighting,TO_VICT);
		act("You hear the crunch of bone as $n tears deeply into $N's throat.",ch,0,ch->fighting,TO_NOTVICT);
	}
}

void fight_prog_horde_hawk(OBJ_DATA *obj, CHAR_DATA *ch)
{
	if(!is_affected(ch, gsn_rage) || !ch->fighting || number_percent()> (.2 * get_skill(ch,gsn_rage)))
		return;
	act("The spirit of the Hawk flows through you as you sense a weakness in $N's defenses and strike!",ch,0,ch->fighting,TO_CHAR);
	act("$n pauses for a moment before unleashing a well aimed blow at you.",ch,0,ch->fighting,TO_VICT);
	act("$n pauses for a moment before unleashing a well aimed blow at $N.",ch,0,ch->fighting,TO_NOTVICT);
	one_hit_new(ch,ch->fighting,TYPE_TRUESTRIKE,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);
}

void pulse_prog_horde_jackal(OBJ_DATA *obj, bool isTick)
{
	CHAR_DATA *victim, *ch = obj->carried_by;
	AFFECT_DATA af;
	if(!ch)
		return;
	victim = ch->fighting;
	if(is_affected(ch,gsn_jackal))
		affect_strip(ch,gsn_jackal);
	if(!ch->fighting || !is_affected(ch, gsn_rage))
		return;
	init_affect(&af);
	af.where	= TO_AFFECTS;
	af.type		= gsn_jackal;
	af.aftype	= AFT_INVIS;
	af.level	= ch->level;
	af.duration	= 2;
	af.location	= APPLY_DAMROLL;
	af.modifier	= (short)((100-(((float) victim->hit / (float) victim->max_hit) * 100)) / 2) * (ch->level / 51);
	affect_to_char(ch, &af);
	af.location	= APPLY_HITROLL;
	affect_to_char(ch, &af);
}

bool death_prog_trophy_belt(OBJ_DATA *belt, CHAR_DATA *ch)
{
	OBJ_DATA *newbelt;
	
	if(!belt->value[4])
		return FALSE;

	extract_obj(belt);
	
	newbelt = create_object(get_obj_index(23645),0);
	obj_to_char(newbelt,ch);
	equip_char(ch,newbelt,WEAR_WAIST,FALSE);

	free_trophy(ch->pcdata->trophy);

	newbelt->value[4] = 0;
	return FALSE;
}

void fight_prog_ruins_sword(OBJ_DATA *obj, CHAR_DATA *ch)
{
	if (!ch->fighting || number_percent() > 12)
		return;

	if (!is_worn(obj))
		return;

	act("Rivulets of water suddenly flow over $p as the blade lashes out at $N!",ch,obj,ch->fighting,TO_CHAR);
	act("Rivulets of water suddenly flow over $p as the blade lashes out at you!",ch,obj,ch->fighting,TO_VICT);
	act("Rivulets of water suddenly flow over $p as the blade lashes out at $N!",ch,obj,ch->fighting,TO_NOTVICT);
	obj_cast_spell(skill_lookup("drown"), ch->level / 2, ch, ch->fighting, obj);

	return;
}

void mprog_say(int inc, char *arg, CHAR_DATA *mob, CHAR_DATA *ch);
void mprog_emote(int inc, char *arg, CHAR_DATA *mob, CHAR_DATA *ch);

void verb_prog_check_bounties(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	DESCRIPTOR_DATA *d;
	char buf[MSL];
	bool found=FALSE;
	CHAR_DATA *mob;
	static char * const his_her [] = { "it",  "his", "her" };


	sprintf(buf,"You can not decipher the words in the book.\n\r");

	if(ch->cabal != CABAL_BOUNTY && !IS_IMMORTAL(ch))
		return send_to_char(buf, ch);
		
	if((mob = get_char_room(ch,"dargor")) == NULL)
		return send_to_char(buf, ch);

	if(mob->pIndexData->vnum != 23604)
		return send_to_char(buf, ch);	

	if(ch->gold < 50)
		return do_say(mob,"Perhaps I can help you another time, when you have a bit more gold.");
	
	ch->gold -= 50;
	
	act("You hand $N 50 gold pieces.", ch, 0, mob, TO_CHAR);
	act("$n hands $N some gold.", ch, 0, mob, TO_ROOM);
	do_emote(mob,"slips the gold into his pouch.");
	do_say(mob,"Now then..");
	do_emote(mob,"studies his list of bounties.");
	for ( d = descriptor_list; d != NULL; d = d->next )
		if ( d->connected == CON_PLAYING && d->character && d->character->in_room != NULL)
			if(d->character->pcdata && d->character->pcdata->bounty)
			{
				found = TRUE;
				if(d->character == ch)
					sprintf(buf,"Hm, I see your name on this list, do you think it's wise for you to be here?");
				else
					sprintf(buf,"%s has %ld gold on %s head.", 
							d->character->name, 
							d->character->pcdata->bounty, 
							his_her[URANGE(0, d->character->sex, 2)]);
				do_say(mob,buf);
			}
                                                                                                     
	if(!found)                                                                                                    
		do_say(mob,"Sorry, I don't see anyone in the realms with a bounty on their head.");                       
                                                                                                 
return;  
	
}

void verb_prog_tilt_scales(OBJ_DATA *obj,CHAR_DATA *ch,char *argument)
{
	OBJ_AFFECT_DATA oaf;
	int difference = obj->value[2];
	if(obj->value[1] == 2)
		return send_to_char("You cannot tilt the scales again yet.\n\r",ch);
	if(!argument[0] || (strcmp(argument,"left") && strcmp(argument,"right")))
		return send_to_char("You can tilt the scales to the left or right.\n\r",ch);
	if(!strcmp(argument,"left"))
	{
		if(obj->value[2] == 0)
			send_to_char("The scales are already tilted to the left.\n\r",ch);
		if(obj->value[2] == 1)
		{
			obj->value[2]--;
			send_to_char("You tilt the scales so that the left side is weighted...\n\r",ch);
		}
		if(obj->value[2] == 2)
		{
			obj->value[2]--;
			send_to_char("You tilt the scales to the left, balancing them...\n\r",ch);
		}
	}
	if(!strcmp(argument,"right"))
	{
		if(obj->value[2] == 2)
			send_to_char("The scales are already tilted to the right.\n\r",ch);
		if(obj->value[2] == 1)
		{
			obj->value[2]++;
		
			send_to_char("You tilt the scales so that the right side is weighted...\n\r",ch);
		}
		if(obj->value[2] == 0)
		{
			obj->value[2]++;
			send_to_char("You tilt the scales to the right, balancing them...\n\r",ch);
		}
	}
	if(difference != obj->value[2])
	{
		
		init_affect_obj(&oaf);
		oaf.where = TO_OBJ_AFFECTS;
		oaf.duration = 12;
		oaf.level = ch->level;
		oaf.modifier = 1;
		oaf.location = APPLY_OBJ_V1;
		oaf.type = gsn_bash;
		oaf.aftype = AFT_SKILL;
		oaf.owner = ch;
		affect_to_obj(obj,&oaf);

	}
return;
}

void verb_prog_ilopheth_bush(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	ROOM_INDEX_DATA *room;
	
	if (ch->fighting)
		return send_to_char("You're otherwise occupied right now.\n\r",ch);

	send_to_char("With a mighty effort, you duck your head and push through the bushes to the west.\n\r",ch);
	act("With a mighty effort, $n ducks $s head and pushes through the bushes to the west.",ch,0,0,TO_ROOM);
	
	room = get_room_index(9096);

	char_from_room(ch);
	char_to_room(ch,room);
	do_look(ch,"auto");

	WAIT_STATE(ch,PULSE_VIOLENCE * 2);

	return;
}

void verb_prog_ilopheth_climb_tree(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	ROOM_INDEX_DATA *to_room = get_room_index(4100);

	if (ch->fighting)
		return send_to_char("You're somewhat distracted right now.\n\r",ch);

	if (ch->position <= POS_SITTING)
		return send_to_char("You need to be standing up to try that!\n\r",ch);

	act("You pull yourself up into the branches of the massive tree and ascend.",ch,0,0,TO_CHAR);
	act("$n carefully climbs the massive tree, disappearing into the canopy.",ch,0,0,TO_ROOM);

	char_from_room(ch);
	char_to_room(ch,to_room);
	do_look(ch,"auto");

	return;
}

void verb_prog_antava_touch_hand(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	ROOM_INDEX_DATA *tomb = get_room_index(102);
	
	if(ch->in_room->vnum != 462)
		return;
	if(ch->fighting)
		return send_to_char("You're too busy fighting to do that!\n\r",ch);
	if(ch->position <= POS_SITTING)
		return send_to_char("You can't reach it while you're sitting down.\n\r",ch);
	
	act("You touch the hand of the decaying statue.",ch,0,0,TO_CHAR);
	act("$n reaches up and touches the hand of the decaying statue.",ch,0,0,TO_ROOM);
	act("A rusty clicking noise begins to sound from within the statue.",0,0,0,TO_ROOM);
	
	if(ch->in_room->vnum != 462)
		return;
	
	RS.Queue.AddToQueue(14,5,act,"Darkness closes back in upon the antechamber as the stone above you shuts abruptly!",ch,0,0,TO_CHAR);
	RS.Queue.AddToQueue(9,2,do_look,ch,"auto");
	RS.Queue.AddToQueue(9,5,act,"$n suddenly slides down from above!",ch,0,0,TO_ROOM);
	RS.Queue.AddToQueue(9,2,char_to_room,ch,tomb);
	RS.Queue.AddToQueue(9,1,char_from_room,ch);
	RS.Queue.AddToQueue(9,5,act,"You suddenly feel the ground drop out from under you!",ch,0,0,TO_CHAR);
	RS.Queue.AddToQueue(9,5,act,"The sound of grinding granite fills the air as $n suddenly drops out of sight!",ch,0,0,TO_ROOM);
	RS.Queue.AddToQueue(2,2,WAIT_STATE,ch,PULSE_VIOLENCE*5);
	
	return;
}

void verb_prog_sidhe_climb_vine(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	ROOM_INDEX_DATA *up_room = NULL, *down_room = NULL;

   	if (ch->in_room->vnum == 4100)
	{
		up_room = get_room_index(4101);
		down_room = get_room_index(9023);
	} else if (ch->in_room->vnum == 4101)
		down_room = get_room_index(4100);
	else
		return;
		

	if (ch->fighting)
		return send_to_char("You're somewhat distracted right now.\n\r",ch);
	if (ch->position <= POS_SITTING)
		return send_to_char("Try standing up first.\n\r",ch);

	if (ch->in_room->vnum == 4101)
	{
		act("You slowly lower yourself down the vines, descending halfway down the tree.",ch,0,0,TO_CHAR);
		act("$n carefully climbs up the vines hanging here, disappearing into the foliage.",ch,0,0,TO_ROOM);
		char_from_room(ch);
		char_to_room(ch,down_room);
		do_look(ch,"auto");
		return;
	}

	if (!str_cmp(argument,"up"))
	{
		act("You test the vines, and then climb carefully upward into the tree.",ch,0,0,TO_CHAR);
		act("$n carefully climbs up the vines hanging here, disappearing into the foliage.",ch,0,0,TO_ROOM);
		char_from_room(ch);
		char_to_room(ch,up_room);
		do_look(ch,"auto");
	}
	else if (!str_cmp(argument,"down"))
	{
		act("You slowly lower yourself down the vines, descending to the floor below.",ch,0,0,TO_CHAR);
		act("$n slowly lowers $mself down the vines, descending to the floor below.",ch,0,0,TO_ROOM);
		char_from_room(ch);
		char_to_room(ch,down_room);
		do_look(ch,"auto");
	}
	else
		return send_to_char("Climb up or down?\n\r",ch);

	return;
}

void verb_prog_listen_conversation(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	int i=0, rand, inc=2, tc=0, cres=0, ccount[MAX_CABAL];
	CHAR_DATA *fat, *minotaur, *violet;
	char buf[MSL];
	char *temp = '\0';
	CRow row;
	
	rand = dice(1,4);
	if((violet = get_char_room(ch,"violet")) == NULL ||
		(fat = get_char_room(ch, "fat")) == NULL ||
		(minotaur = get_char_room(ch, "minotaur")) == NULL)
		return;

	if(violet->pIndexData->vnum != 2407 ||
	   fat->pIndexData->vnum != 2529 ||
	   minotaur->pIndexData->vnum != 2525)
		return;

	send_to_char("You try and listen to what is being talked about at the bar....\n\r", ch);

	if(rand == 1)
	{
		mprog_say(inc,"...an then he cut their heads off!  All fifty of them!", fat, ch);
		mprog_emote(inc*2,"snorts derisively.", minotaur, ch);
		mprog_say(inc*3,"It's true, I swear it!", fat, ch);
		mprog_say(inc*4,"That story gets better every time you tell it, Boll.", violet, ch);
		cres = RS.SQL.Select("name FROM players ORDER BY pks DESC LIMIT 1");
		if(cres)
		{
			row = RS.SQL.GetRow();
			temp = palloc_string(row[0]);
			RS.Queue.AddToQueue(inc*5, 1, (void*)free,temp);
			RS.Queue.AddToQueue(inc*5, 4, (void*)say_to, fat, ch, (char*)"Just do yourself a favor, and watch out for $t.", temp);

		}
	}
	else if(rand == 2)
	{
		mprog_say(inc,"Time to pay up, Georg.  Your tab's getting massive.", violet, ch);
		mprog_emote(inc*2, "reaches for his purse, then yelps in surprise.", minotaur, ch);
		mprog_emote(inc*3, "leans over and examines the minotaur customer.", fat, ch);

/*
		cres = RS.SQL.Select("DISTINCT players.name FROM players,theft WHERE players.class=1 AND theft.ch=players.name ORDER BY COUNT(theft.ch) DESC LIMIT 1");
		if(cres)
		{
			while(!RS.SQL.End())
			{
				row = RS.SQL.GetRow();
				cres2 = RS.SQL.Select("COUNT(ch) FROM theft WHERE ch='%s'", row[0]);
				row2 = RS.SQL.GetRow();
			if(atoi(row2[0]) > tc)
			{
				tc = atoi(row2[0]);
				temp = palloc_string(row[0]);
				mysql_free_result(res2);
			}
		}
		RS.Queue.AddToQueue(inc*4, 1, (void*)free,temp);
		RS.Queue.AddToQueue(inc*4, 4, (void*)say_to, fat, ch, (char*)"Haha, $t got him again!  By Rygath's teeth, that's a fine thief...", temp);
*/
	}
/*	else if(rand == 3)
	{
		mprog_say(inc,"What do you mean, hiring?", violet, ch);
		mprog_emote(inc*2, "raises an eyebrow, peering sideways at a fat man.", minotaur, ch);
		mprog_say(inc*3,"Don't look at me like that, Georg.  I only know what I've heard...", fat, ch);
		res = one_query_res("SELECT name,cabal FROM players WHERE cabal>0");
		if(!res)
			return mysql_free_result(res);
		while((row = mysql_fetch_row(res)))
		{
			sprintf(buf,"SELECT COUNT(ch) FROM inductions WHERE ch='%s'", row[0]);
			res2 = one_query_res(buf);
			if(!res2)
				continue;
			row2 = mysql_fetch_row(res2);
			if(atoi(row2[0]) > tc)
			{
				tc = atoi(row2[0]);
				temp = talloc_string(row[0]);
				mysql_free_result(res2);
			}
		}
		add_one_to_queue(inc*4,free,temp);
		add_four_to_queue(inc*4,say_to,fat,ch,"$t has been inducting people like mad, and I think I've got a shot.", temp);
		mprog_emote(inc*5,"gurgles quietly into his drink.", minotaur, ch);
	}*/
	else
	{
		mprog_emote(inc, "gesticulates wildly, nearly spilling his drink.", minotaur, ch);
		mprog_emote(inc*2, "gasps in astonishment.", violet, ch);
		mprog_say(inc*3,"Wow... hadn't heard that one yet, Georg.  Was this recently?", fat, ch);
		mprog_emote(inc*4, "nods violently.", minotaur, ch);
		mprog_emote(inc*5, "chuckles politely.", violet, ch);	
/*		res = one_query_res("SELECT killercabal FROM pklogs");
		if(!res)
			return mysql_free_result(res);
		while((row = mysql_fetch_row(res)))
		{
			ccount[atoi(row[0])]++;
		}
		tc =0;
		for(i=0; i < MAX_CABAL; i++)                                                                          
			if(ccount[i] > tc)                                                                                
				tc = i;         
		RS.Queue.AddToQueue(inc*6, 4, (void*)say_to,violet,ch,(char*)"Well then, I suppose it's only a matter of time before we're all members of $t",
				cabal_table[tc].name);
		mprog_say(inc*7,"They're really cleaning house out there.", violet, ch);
*/
	}

	WAIT_STATE(ch, PULSE_VIOLENCE * 3);
	return;
}

void verb_prog_rub_ball(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	act("You rub the BALL OF DEATH.",ch,obj,0,TO_CHAR);
	act("$n rubs the BALL OF DEATH.",ch,obj,0,TO_ROOM);
	
	raw_kill(ch,ch);
	
	return;
}

void verb_prog_twist_ring(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	if(!is_worn(obj))
		return send_to_char("Huh?\n\r",ch);

	act("As you twist a tarnished gold ring on your finger, you feel lighter on your feet.",ch,0,0,TO_CHAR);
	act("$n twists $p on his finger.",ch,obj,0,TO_ROOM);
	obj_cast_spell(skill_lookup("refresh"),obj->level,ch,ch,NULL);
	WAIT_STATE(ch,PULSE_VIOLENCE);
	return;

}

void verb_prog_twist_two_faced(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	int nChance = 0;
	EXIT_DATA *pexit;
	if(!argument[0] || (strcmp(argument,"clockwise") && strcmp(argument,"counterclockwise")))
		return send_to_char("Which way do you want to turn it?  You could twist it clockwise or counterclockwise.\n\r",ch);
	if(!strcmp(argument,"clockwise"))
	{
		send_to_char("You twist the sundial clockwise.\n\r",ch);
		act("$n twists the sundial clockwise.",ch,0,0,TO_ROOM);
		nChance = 35;
	}
	else
	{
		send_to_char("You twist the sundial counterclockwise.\n\r",ch);
		act("$n twists the sundial counterclockwise.",ch,0,0,TO_ROOM);
		nChance = 65;
	}
	if(number_percent()>nChance)
	{
		act("The faces of the statue seem to grin maniacally.",ch,0,0,TO_ALL);
		send_to_char("A stream of acid shoots out of the mouth of the statue at you!\n\r",ch);
		act("A stream of acid shoots out of the mouth of the statue at $n!",ch,0,0,TO_ROOM);
		spell_acid_stream(gsn_acid_stream, ch->level + 1, ch, ch, TARGET_CHAR);
		if(ch->ghost)
			return;
		send_to_char("A small dart flies out of the eye of the statue, barely piercing your skin.\n\r",ch);
		act("A small dart flies out of the eye of the statue, barely piercing $n's skin.",ch,0,0,TO_ROOM);
		spell_plague(gsn_plague, ch->level + 5, ch, ch, TARGET_CHAR);
	}
	else
	{
		send_to_char("As you rotate the sundial, a trap door beneath the table slowly swings open!\n\r",ch);
		pexit = ch->in_room->exit[DIR_DOWN];
		REMOVE_BIT(pexit->exit_info, EX_LOCKED);
		REMOVE_BIT(pexit->exit_info, EX_CLOSED);
	}
}

void verb_prog_energize_tattoo(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	OBJ_AFFECT_DATA oaf;
	obj = get_eq_char(ch,WEAR_BRAND);
	if(!is_worn(obj))
		return;
	if(ch->position != POS_FIGHTING)
		return send_to_char("You must be fighting to sap your opponent's strength.\n\r",ch);
	if(IS_NPC(ch->fighting))
		return send_to_char("You must find a purer source of power.\n\r",ch);
	if(is_affected_obj(obj,gsn_bash))
		return send_to_char("Your tattoo must fully absorb the energy that you have sapped so recently.\n\r",ch);
	act("You attempt to siphon some energy from $N to power your tattoo.",ch,0,ch->fighting,TO_CHAR);
	act("You feel a cold sensation run through your body.",ch,0,ch->fighting,TO_VICT);
	
	if(!saves_spell(ch->level,ch->fighting,DAM_NEGATIVE))
	{
		damage_new(ch,ch->fighting,dice(1,10),TYPE_UNDEFINED,DAM_NEGATIVE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT, "energy sapping");
		ch->fighting->mana -= (int)(2.8*ch->level);
		obj->value[0]++;
		init_affect_obj(&oaf);
		oaf.where = TO_OBJ_AFFECTS;
		oaf.aftype = AFT_SKILL;
		oaf.duration = 5;
		oaf.modifier = 0;
		oaf.location = APPLY_NONE;
		oaf.type = gsn_bash;
		affect_to_obj(obj,&oaf);
	}
	
WAIT_STATE(ch, PULSE_VIOLENCE * 2);
return;
}

void verb_prog_evoke_stone(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	/* Morg - Leave the code compilable.  Please.
	OBJ_AFFECT_DATA *oaf;
	int chance;
	
	oaf = affect_find_obj(obj->affected,gsn_imbue_stone);

	if(!oaf)
		return send_to_char("This stone is not powered by natural energy.\n\r");

	if(ch->Class()->name != "druid")
		return send_to_char("This stone is useless to you.\n\r");
	
	if(obj->value[1] < 1)
		return send_to_char("The energy of this stone has been depleted.\n\r");
	
	if(obj->value[0] == 0)
		return send_to_char("This stone has not been imbued.\n\r");
	
	if(obj->value[0] == 1)
	{
		chance = number_percent();

		if(chance < 23)
		{
		}
	}
*/
	return;




		
	
}

void verb_prog_harness_crystal(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	OBJ_AFFECT_DATA *oaf;
	int mana, efficiency;

	oaf = affect_find_obj(obj->affected,gsn_fashion_crystal);

	if (!oaf)
		return send_to_char("This crystal is devoid of magical energy.\n\r",ch);

	if (oaf->owner == ch)
		efficiency = 50;
	else if (ch->Class()->name == "sorcerer"
		|| ch->Class()->name == "shapeshifter"
		|| ch->Class()->name == "necromancer"
		|| ch->Class()->name == "chronomancer")
		efficiency = number_range(25,40);
	else
		efficiency = number_range(5,20);

	act("Focusing intently on the charged crystal, you harness the energy stored within.",ch,0,0,TO_CHAR);
	act("$n focuses intently on a glowing crystal in $s hands.",ch,0,0,TO_ROOM);

	mana = oaf->modifier;
	
	extract_obj(obj);

	if (efficiency <= 20 && number_percent() > 50) {
		act("The crystal crumbles into dust and the energy dissipates as your focus falters.",ch,0,0,TO_CHAR);
		return;
	}

	act("The crystal crumbles to dust as you harness the mana stored within.",ch,0,0,TO_CHAR);

	mana *= efficiency;
	mana /= 100;
	ch->mana += mana;

	if (ch->mana > (1.25 * ch->max_mana))
		ch->mana = (short)(ch->max_mana * 1.25);

	return;
}

void verb_prog_throw_net(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim;
	AFFECT_DATA af;
	char buf[MSL];

	if (argument[0] == '\0')
	{
		if(!ch->fighting)
			return send_to_char("Throw the net at who?\n\r", ch);
		else
			victim = ch->fighting;
	}
	else
	{
		if((victim = get_char_room(ch,argument)) == NULL)
			return send_to_char("They aren't here.\n\r",ch);
	}

	if(!is_worn(obj))
		return send_to_char("You must have the net in your hands to entrap someone with it.\n\r", ch);

	if(is_affected(victim, gsn_hunters_net))
		return send_to_char("They are already ensnared in a net.\n\r",ch);
	if(is_safe(ch,victim))
		return;
	act("You throw $p over $N, entangling $M in the thick netting!", ch, obj, victim, TO_CHAR);
	act("$n throws $p over $N, entangling $M in the thick netting!", ch, obj, victim, TO_NOTVICT);
	act("$n throws $p over you, entangling you in the thick netting!", ch, obj, victim, TO_VICT);

	init_affect(&af);
	af.where    = TO_AFFECTS;
	af.type     = gsn_hunters_net;
	af.aftype   = AFT_POWER;
	af.level    = ch->level;
	af.location	= APPLY_DEX;
	af.modifier	= -1 * (ch->level / 10);
	af.duration = 12;
	affect_to_char(victim, &af);

	obj_from_char(obj);
	extract_obj(obj);
	
	if(!ch->fighting)
	{
		sprintf(buf,"Help!  %s just threw a net on me!", ch->name);
		do_yell(victim, buf);
		set_fighting(ch,victim);
	}
	WAIT_STATE(ch,PULSE_VIOLENCE*number_range(1,2));	
	return;
}

void verb_prog_fire_pistol(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim;

	if (argument[0] == '\0') {
		if (!ch->fighting)
			return send_to_char("Bust a cap in whose ass?\n\r",ch);
		else
			victim = ch->fighting;
	} else {
		if ((victim = get_char_room(ch,argument)) == NULL)
			return send_to_char("They aren't here.\n\r",ch);
	}

	if (obj->wear_loc != WEAR_HOLD)
		return send_to_char("Can't bust no caps if that shit ain't cocked and ready, foo!\n\r",ch);

	if(ch == victim)
	{
		act("Oh no!  $n puts $p in $s mouth and pulls the trigger!",ch,obj,0,TO_ROOM);
		send_to_char("Don't do it, man!\n\r",ch);
		act("You put $p in your mouth and pull the trigger!",ch,obj,0,TO_CHAR);
	}
	else
	{
		act("You pull the trigger and bust a cap in $N's ass!",ch,0,victim,TO_CHAR);
		act("$n turns $p sideways and busts a cap in yo ass!",ch,obj,victim,TO_VICT);
		act("$n turns $p sideways and busts a cap in $N's ass!",ch,obj,victim,TO_NOTVICT);
	}
	damage_new(ch,victim,number_range(200,500),gsn_bash,DAM_TRUESTRIKE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"ass cappin'");

	WAIT_STATE(ch,PULSE_VIOLENCE);

	return;

}

void verb_prog_kneel_guillotine(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	ROOM_INDEX_DATA *old_room = ch->in_room;
	
	act("You place your neck across the wooden bar of the guillotine.",ch,0,0,TO_CHAR);
	act("$n places $s neck across the wooden bar of the guillotine.",ch,0,0,TO_ROOM);

	act("The whistle of metal slicing through the air is the last thing you hear.",ch,0,0,TO_CHAR);
	act("The razor-sharp blade drops rapidly, slicing $n's head clean from $s body!",ch,0,0,TO_ROOM);

	raw_kill(ch,ch);

	if(!IS_NPC(ch))
	{
		char_from_room(ch);
		char_to_room(ch,old_room);
	}
	return;
}

void hit_prog_blade_truth(OBJ_DATA *obj, CHAR_DATA *ch, CHAR_DATA *victim, int damage)
{
	bool dual = FALSE;
	
	if (number_percent() > 20)
		return;

	if ((get_eq_char(ch,WEAR_DUAL_WIELD)) == obj)
		dual = TRUE;

	act("$p blurs into motion as it whistles through the air.",ch,obj,0,TO_ALL);

	if (dual)
		one_hit(ch,victim,gsn_dual_wield);
	else
		one_hit(ch,victim,TYPE_UNDEFINED);

	return;
}

void hit_prog_essence_light(OBJ_DATA *obj, CHAR_DATA *ch, CHAR_DATA *victim, int damage)
{
	if (damage <= 0)
		return;

	if (!(IS_NPC(victim) && IS_SET(victim->act,ACT_UNDEAD))
		&& !IS_SET(victim->form,FORM_UNDEAD)) {
		if (IS_GOOD(victim)) {
			if (victim->hit < victim->max_hit && number_percent() <= 50) {
			act("A pearly luminescence flows into $n's wounds.",victim,obj,ch,TO_ROOM);
			act("A pearly luminescence flows into your wounds.",victim,obj,ch,TO_CHAR);
			send_to_char("You feel healed!\n\r",victim);
			victim->hit = UMIN(victim->max_hit,victim->hit + dice(8,8));
			}
		} else if (IS_NEUTRAL(victim)) {
			if (number_percent() <= 50) {
				act("A reluctant light flickers in $n's hands and lashes out!",ch,obj,victim,TO_ROOM);
				act("A reluctant light flickers in your hands and lashes out!",ch,obj,victim,TO_CHAR);
				damage_new(ch,victim,dice(3,ch->level),TYPE_UNDEFINED,DAM_HOLY,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"torrent of light");
			}
		} else {
			act("Waves of brilliant white light cascade from your hands, searing $n!",victim,obj,ch,TO_VICT);
			act("Waves of brilliant white light cascade from $N's hands, searing $n!",victim,obj,ch,TO_NOTVICT);
			act("Waves of brilliant white light cascade from $N's hands, searing you!",victim,obj,ch,TO_CHAR);
			damage_new(ch,victim,dice(ch->level,5),TYPE_UNDEFINED,DAM_TRUESTRIKE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"blinding fury");
			if (number_percent() <= 20) {
				switch(number_range(1,4)) {
					case(1):
					case(2):
						act("You reel in agony as the raw energy engulfs you!",victim,0,ch,TO_CHAR);
						act("$n staggers as the raw energy engulfs $m!",victim,0,ch,TO_ROOM);
						LAG_CHAR(victim,PULSE_VIOLENCE);
						break;
				}
			}
		}
	} else {
		act("$p erupts with an unfathomable radiance, engulfing $N!",ch,obj,victim,TO_CHAR);
		act("A blinding radiance billows forth from $n, engulfing $N!",ch,obj,victim,TO_NOTVICT);
		act("$N lets out a terrifying preternatural wail, and crumbles to dust!",ch,obj,victim,TO_ALL);
		raw_kill(ch,victim);
	}
	return;
}

void hit_prog_essence_darkness(OBJ_DATA *obj, CHAR_DATA *ch, CHAR_DATA *victim, int damage)
{
	AFFECT_DATA af;
	int leech;
	bool dual = FALSE;

	if (damage <= 0)
		return;

	leech = damage/3;

	if (IS_AFFECTED(victim,AFF_SANCTUARY))
		leech *= 2;

	ch->hit += leech;

	if (!is_affected(victim,gsn_curse) && number_percent() <= 25) {
		act("A black aura briefly envelops $n as the darkness touches $s flesh.",victim,0,ch,TO_ROOM);
		act("You feel a taint wash over you as the darkness touches your flesh.",victim,0,ch,TO_CHAR);
		init_affect(&af);
		af.where		=	TO_AFFECTS;
		af.type			=	gsn_curse;
		af.aftype		=	AFT_SPELL;
		af.location		=	APPLY_HITROLL;
		af.modifier		=	-(ch->level/7);
		af.duration		=	ch->level / 4;
		SET_BIT(af.bitvector, AFF_CURSE);
		af.owner		=	ch;
		affect_to_char(victim,&af);
		af.location		=	APPLY_SAVES;
		af.modifier		=	ch->level / 7;
		affect_to_char(victim,&af);
		return;
	}

	if (IS_GOOD(victim) && number_percent() <= 40) {
		if ((get_eq_char(ch,WEAR_DUAL_WIELD)) == obj)
			dual = TRUE;
		act("$p writhes in your hands and strikes anew, thirsty for blood!",ch,obj,victim,TO_CHAR);
		act("A mournful wail is audible as $n's black hands lash out at $N!",ch,0,victim,TO_NOTVICT);
		act("A mournful wail is audible as $n's black hands lash out at you!",ch,0,victim,TO_VICT);
		if (dual)
			one_hit(ch,victim,gsn_dual_wield);
		else
			one_hit(ch,victim,TYPE_UNDEFINED);
	}

	return;
}

void fight_prog_essence_darkness(OBJ_DATA *obj, CHAR_DATA *ch)
{
	CHAR_DATA *victim;

	if (!is_worn(obj))
		return;

	if (number_percent() > 35)
		return;
	
	if ((victim = ch->fighting) == NULL)
		return;

	ch->alignment = UMAX(-1000, ch->alignment - 10);
	
	switch(number_range(1,3)) {
		case(1):
			act("A cloud of putrefaction issues forth from your hands!",ch,0,victim,TO_CHAR);
			act("A cloud of putrefaction issues forth from $n's hands!",ch,0,victim,TO_ROOM);
			obj_cast_spell(skill_lookup("plague"), ch->level, ch, victim,obj);
			break;
		case(2):
			act("Your eyes cloud suddenly with a black haze!",victim,0,ch,TO_CHAR);
			act("A swirling black cloud encircles $n's face!",victim,0,ch,TO_ROOM);
			obj_cast_spell(skill_lookup("blindness"),ch->level, ch, victim,obj);
			break;
		case(3):
			act("Darkness lashes out from $N's hands to engulf you!",victim,0,ch,TO_CHAR);
			act("Darkness lashes out from $N's hands to engulf $n!",victim,0,ch,TO_NOTVICT);
			act("Darkness lashes out from your hands to engulf $n!",victim,0,ch,TO_VICT);
			obj_cast_spell(skill_lookup("energy drain"), ch->level, ch, victim,obj);
			break;
	}

	return;
}


void verb_prog_rub_talisman(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	int sn = skill_lookup("word of recall");
	int target = TAR_CHAR_DEFENSIVE;
	void *vo = (CHAR_DATA *)ch;
	
	act("You rub a worn patch of the wooden talisman.",ch,0,0,TO_CHAR);
	act("$n vigorously rubs $p.",ch,obj,0,TO_ROOM);

	WAIT_STATE(ch,PULSE_VIOLENCE);
	if(check_deny_magic(ch))
		return;
	
	if (ch->fighting)
		return send_to_char("Nothing seems to happen.\n\r",ch);
	
	(*skill_table[sn].spell_fun) ( sn, ch->level, ch, vo, target);

	act("$p crumbles into dust as the magic drains from it.",ch,obj,0,TO_CHAR);

	extract_obj(obj);
	
	return;
}

void verb_prog_gate_talisman(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	int vnum;
	ROOM_INDEX_DATA *to_room;
	
	if(!is_number(argument))
		return send_to_char("Go where?\n\r",ch);

	vnum = atoi(argument);

	if (((to_room = get_room_index(vnum)) == NULL) || (str_cmp(to_room->area->builders,ch->name) && vnum != 1200))
		return send_to_char("You can't go there!\n\r",ch);

	send_to_char("The talisman flickers and feels warm for a moment, and you are elsewhere.\n\r",ch);

	act("A rift opens in midair beside $n and $e steps in and vanishes.",ch,0,0,TO_ROOM);

	char_from_room(ch);
	char_to_room(ch,to_room);

	do_look(ch,"auto");

	return;
}

void speech_prog_vnum_talisman(OBJ_DATA *obj, CHAR_DATA *ch, char *speech)
{
	char buf[MSL];
	
	if (!str_cmp(speech, "vnum")
		&& !str_cmp(ch->in_room->area->builders,ch->name)) {
		sprintf(buf,"Your talisman whispers to you: you are currently in room #%d.\n\r",ch->in_room->vnum);
		send_to_char(buf,ch);
	}
	
	return;
}

void pulse_prog_pillar_zap(OBJ_DATA *obj, bool isTick)
{
	char buf[MSL];
	CHAR_DATA *mob, *mob_next;
	ROOM_INDEX_DATA *room;
	bool found = FALSE;

	if ((room = obj->in_room) == NULL)
		return;

	if (!room->people)
		return;
	
	for (mob = room->people; mob; mob = mob->next_in_room)
		if (IS_NPC(mob) && (IS_SET(mob->act, ACT_UNDEAD) || IS_SET(mob->form, FORM_UNDEAD))) {
			found = TRUE;
			break;
		}

	if (!found)
		return;

	sprintf(buf,"\x01B[1;34mThe pillars crackle suddenly with raw energy and emit a beam of blinding light!\x01B[0m");
	
	act(buf,room->people,0,0,TO_ALL);

	for (mob = room->people; mob; mob = mob_next) {
		mob_next = mob->next_in_room;

		if (IS_NPC(mob) && (IS_SET(mob->act, ACT_UNDEAD) || IS_SET(mob->form,FORM_UNDEAD))) {
			act("$n dissolves into dust as the shining blue light sweeps over it!",mob,0,0,TO_ROOM);
			extract_char(mob, TRUE);
		}
	}

	return;
}

bool loot_prog_shelf(OBJ_DATA *shelf, OBJ_DATA *obj, CHAR_DATA *ch)
{
	ROOM_INDEX_DATA *room;
	CHAR_DATA *mob;
	int mobvnum = 0;
	bool found = FALSE;

	if ((room = shelf->in_room) == NULL)
		return TRUE;

	switch (shelf->pIndexData->vnum) {
		case(2241):
			if (room->vnum != 2375)
				return TRUE;
			mobvnum = 2222;
			break;
		case(2242):
			if (room->vnum != 2377)
				return TRUE;
			mobvnum = 2220;
			break;
		case(2243):
			if (room->vnum != 2379)
				return TRUE;
			mobvnum = 2221;
			break;
		default:
			return TRUE;
	}

	for (mob = room->people; mob; mob = mob->next_in_room) {
		if (!IS_NPC(mob))
			continue;
		if (mob->pIndexData->vnum == mobvnum) {
			found = TRUE;
			break;
		}
	}

	if (!found)
		return TRUE;

	act("$n bars your access to $p, and you are unable to do that.",mob,shelf,ch,TO_VICT);
	act("$N moves toward $p as though to get something from it, but $n blocks $m.",mob,shelf,ch,TO_NOTVICT);
	
	return FALSE;
}

bool open_prog_sewer_casket(OBJ_DATA *obj, CHAR_DATA *ch)
{
	CHAR_DATA *skeleton;

	REMOVE_BIT_OLD(obj->value[1], CONT_CLOSED);
	
	send_to_char("You slide the heavy lid off the stone burial casket.\n\r",ch);
	act("$n slides the heavy lid off the stone burial casket.",ch,0,0,TO_ROOM);

	skeleton = create_mobile(get_mob_index(2200));

	char_to_room(skeleton,ch->in_room);
	act("Dust fills the burial chamber, and as it clears....", ch, 0, 0, TO_ALL);
	act("$N rises out of the casket and lunges at you!", ch, 0, skeleton, TO_CHAR);
	act("$N rises out of the casket and lunges at $n!", ch, 0, skeleton, TO_ROOM);

	do_murder(skeleton, ch->name);

	return FALSE;
}

void verb_prog_pull_hook(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit = ch->in_room->exit[2];

	if ( !IS_SET(pexit->exit_info, EX_CLOSED) || !IS_SET(pexit->exit_info, EX_LOCKED) )
	{
		act("You tug on a metal hook inside a notch in the marble and nothing happens.", ch, 0, 0, TO_CHAR );
		act("$n tugs on a metal hook inside a notch in the marble.", ch, 0, 0, TO_ROOM);
		return;
	}
	REMOVE_BIT(pexit->exit_info, EX_CLOSED);
	REMOVE_BIT(pexit->exit_info, EX_LOCKED);
    send_to_char( "You tug on a hook of metal inside this notch.\n\r",ch);
	act( "$n tugs on a small metal hook inside a notch in the marble.", ch, NULL, pexit->keyword, TO_ROOM );
	act("A soft click is audible and then the southern wall slides noisily open!",ch,0,0,TO_ALL);

	if ((to_room = pexit->u1.to_room) != NULL
		 && (pexit_rev=to_room->exit[0]) != NULL
		 &&   pexit_rev->u1.to_room == ch->in_room ) {
		 REMOVE_BIT( pexit_rev->exit_info, EX_LOCKED );
		 REMOVE_BIT( pexit_rev->exit_info, EX_LOCKED );
	}
}

void verb_prog_turn_spindle(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	ROOM_INDEX_DATA *room = obj->in_room;
	EXIT_DATA *pexit;
	int dir;
	char *direction, *revdir;

	if (!room)
		return send_to_char("Huh?\n\r",ch);

	WAIT_STATE(ch,2*PULSE_VIOLENCE);

	switch (room->vnum) {
		case(2277):
			dir = DIR_DOWN;
			break;
		case(22600):
			dir = DIR_UP;
			break;
		default:
			act("You turn the wheel and it spins rapidly, seemingly unattached to anything.",ch,0,0,TO_CHAR);
			act("$n turns the wheel and it spins rapidly, seemingly unattached to anything.",ch,0,0,TO_ROOM);
			return;
	}

	pexit = room->exit[dir];
		
	act("The spindle wheel creaks loudly as you turn it....",ch,0,0,TO_CHAR);
	act("The spindle wheel creaks loudly as $n turns it....",ch,0,0,TO_ROOM);

	direction = (char *)flag_name_lookup(dir,direction_table);

	if (IS_SET(pexit->exit_info,EX_CLOSED)) {
		act("A massive rusted grate slides noisily open, revealing a tunnel $tward!",ch,direction,0,TO_ALL);
		REMOVE_BIT(pexit->exit_info,EX_CLOSED);
		REMOVE_BIT(pexit->exit_info,EX_LOCKED);
		REMOVE_BIT(pexit->u1.to_room->exit[reverse_d(dir)]->exit_info,EX_CLOSED);
		REMOVE_BIT(pexit->u1.to_room->exit[reverse_d(dir)]->exit_info,EX_LOCKED);
		if (pexit->u1.to_room->people) {
			revdir = (char *)flag_name_lookup(reverse_d(dir),direction_table);
			act("A massive rusted grate slides noisily open, revealing a tunnel $tward!",ch,revdir,0,TO_ALL);
		}
	} else {
		act("A massive rusted grates slides noisily shut, barring the tunnal $tward!",ch,direction,0,TO_ALL);
		SET_BIT(pexit->exit_info,EX_CLOSED);
		SET_BIT(pexit->exit_info,EX_LOCKED);
		SET_BIT(pexit->u1.to_room->exit[reverse_d(dir)]->exit_info,EX_CLOSED);
		SET_BIT(pexit->u1.to_room->exit[reverse_d(dir)]->exit_info,EX_LOCKED);
		if (pexit->u1.to_room->people) {
			revdir = (char *)flag_name_lookup(reverse_d(dir),direction_table);
			act("A massive rusted grate slides noisily shut, revealing a tunnel $tward!",ch,revdir,0,TO_ALL);
		}
	}

	return;
}

void verb_prog_touch_obelisk(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	char buf[MSL];
	AFFECT_DATA *af;

	af = affect_find(ch->affected,gsn_horde_communion);

	if (!af)
		return;

	af->modifier = 1;

	REMOVE_BIT(ch->comm,COMM_PROMPT);

	sprintf(buf,"%sA sharp blow to the back of your head sends you tumbling forward into an inky abyss.%s\n\r",
		get_char_color(ch,"red"),END_COLOR(ch));
	send_to_char(buf,ch);

	ch->position = POS_SLEEPING;
	SET_BIT(ch->affected_by,AFF_SLEEP);
	die_follower(ch);
	ch->pcdata->condition[COND_HUNGER] = 1;
	ch->pcdata->condition[COND_THIRST] = 1;

	RS.Queue.AddToQueue(5, 1, communion_handler, ch);

	return;
}

void communion_handler(CHAR_DATA *ch)
{
	char buf[MSL], short_desc[MSL], long_desc[MSL], name[MSL], query[MSL];
	OBJ_DATA *obj, *remove;
	AFFECT_DATA *af;
	CHAR_DATA *animal;
	int tribe, pelt;

	af = affect_find(ch->affected,gsn_horde_communion);

	if (!af) {
		ch->position = POS_STANDING;
		REMOVE_BIT(ch->affected_by,AFF_SLEEP);
		send_to_char("Something has gone wrong with the communion.  Please pray if you see this.\n\r",ch);
		return;
	}
	
	switch (af->modifier) {
		case (1):
			sprintf(buf,"%sEnveloped in the blackness, you fall, tumbling wildly, without any sense of your bearings.%s\n\r",get_char_color(ch,"lightgrey"),END_COLOR(ch));
			send_to_char(buf,ch);
			af->modifier++;
			RS.Queue.AddToQueue(5, 1, communion_handler, ch);
			break;
		case (2):
			sprintf(buf,"%sThe sonorous tone of a ramshorn booms through the void surrounding you.%s\n\r",get_char_color(ch,"lightgrey"),END_COLOR(ch));
			send_to_char(buf,ch);
			af->modifier++;
			RS.Queue.AddToQueue(5, 1, communion_handler, ch);
			break;
		case (3):
			sprintf(buf,"%sA pearly luminescence begins to coalesce before your eyes, suspended in the darkness that rushes past as you plummet.%s\n\r",get_char_color(ch,"white"),END_COLOR(ch));
			send_to_char(buf,ch);
			af->modifier++;
			RS.Queue.AddToQueue(5, 1, communion_handler, ch);
			break;
		case (4):
			sprintf(buf,"%sWithin the light, humanoid outlines begin to take shape.%s\n\r",get_char_color(ch,"lightgrey"),END_COLOR(ch));
			send_to_char(buf,ch);
			af->modifier++;
			RS.Queue.AddToQueue(5, 1, communion_handler, ch);
			break;
		case (5):
			sprintf(buf,"%sYou see a behemoth of a man, gird in naught but animal hides and a belt adorned with human scalps.%s\n\r",get_char_color(ch,"lightgrey"),END_COLOR(ch));
			send_to_char(buf,ch);
			af->modifier++;
			RS.Queue.AddToQueue(5, 1, communion_handler, ch);
			break;
		case (6):
			sprintf(buf,"%sThe stillness is shattered by a deafening battlecry, as the figures blur into motion.%s\n\r",get_char_color(ch,"white"),END_COLOR(ch));
			send_to_char(buf,ch);
			af->modifier++;
			RS.Queue.AddToQueue(5, 1, communion_handler, ch);
			break;
		case (7):
			sprintf(buf,"%sA spray of blood follows his axe's arcing path through the air, light glinting off the silver blade.%s\n\r",get_char_color(ch,"red"),END_COLOR(ch));
			send_to_char(buf,ch);
			af->modifier++;
			RS.Queue.AddToQueue(5, 1, communion_handler, ch);
			break;
		case (8):
			sprintf(buf,"%sYou have the sensation of being lifted higher, as your field of vision broadens.%s\n\r",get_char_color(ch,"lightgrey"),END_COLOR(ch));
			send_to_char(buf,ch);
			af->modifier++;
			RS.Queue.AddToQueue(5, 1, communion_handler, ch);
			break;
		case (9):
			sprintf(buf,"%sA battlefield is spread out beneath you, the combat just witnessed but a part of the larger picture.%s\n\r",get_char_color(ch,"lightgrey"),END_COLOR(ch));
			send_to_char(buf,ch);
			af->modifier++;
			RS.Queue.AddToQueue(5, 1, communion_handler, ch);
			break;
		case (10):
			sprintf(buf,"%sA small band of barbarians appear to have been trapped by a well-armored force of vastly superior numbers, a sheer cliffside at their backs.%s\n\r",get_char_color(ch,"lightgrey"),END_COLOR(ch));
			send_to_char(buf,ch);
			af->modifier++;
			RS.Queue.AddToQueue(5, 1, communion_handler, ch);
			break;
		case (11):
			sprintf(buf,"%sFierce men and women, of all the races of Shalar, the barbarian horde raise their voices in a single outcry.%s\n\r",get_char_color(ch,"lightgrey"),END_COLOR(ch));
			send_to_char(buf,ch);
			af->modifier++;
			RS.Queue.AddToQueue(5, 1, communion_handler, ch);
			break;
		case (12):
			sprintf(buf,"%sTheir bellowed warcry rippling out over the plains, they launch themselves into the heart of the enemy formation.%s\n\r",get_char_color(ch,"lightgrey"),END_COLOR(ch));
			send_to_char(buf,ch);
			af->modifier++;
			RS.Queue.AddToQueue(5, 1, communion_handler, ch);
			break;
		case (13):
			sprintf(buf,"%sThe clash of steel upon steel and the horrible cries of dying men rise from the melee below...%s\n\r",get_char_color(ch,"lightgrey"),END_COLOR(ch));
			send_to_char(buf,ch);
			af->modifier++;
			RS.Queue.AddToQueue(5, 1, communion_handler, ch);
			break;
		case (14):
			sprintf(buf,"%s...and the pungent odor of blood and death fills the air as the earth is stained a deep crimson.%s\n\r",get_char_color(ch,"lightred"),END_COLOR(ch));
			send_to_char(buf,ch);
			af->modifier++;
			RS.Queue.AddToQueue(5, 1, communion_handler, ch);
			break;
		case (15):
			sprintf(buf,"%sThe barbarian horde cleaves a swath of terrible carnage through the heart of the fray, destruction trailing in their wake.%s\n\r",get_char_color(ch,"lightgrey"),END_COLOR(ch));
			send_to_char(buf,ch);
			af->modifier++;
			RS.Queue.AddToQueue(5, 1, communion_handler, ch);
			break;
		case (16):
			sprintf(buf,"%sThe once-superior force, now decimated, turns to flee, a cacophony of shouts from the barbarians at their backs.%s\n\r",get_char_color(ch,"lightgrey"),END_COLOR(ch));
			send_to_char(buf,ch);
			af->modifier++;
			RS.Queue.AddToQueue(5, 1, communion_handler, ch);
			break;
		case (17):
			sprintf(buf,"%sAs suddenly as the scene appeared, it is gone, the figures sinking into the earth and fading from sight entirely.%s\n\r",get_char_color(ch,"white"),END_COLOR(ch));
			send_to_char(buf,ch);
			af->modifier++;
			RS.Queue.AddToQueue(5, 1, communion_handler, ch);
			break;
		case (18):
			SET_BIT(ch->comm,COMM_PROMPT);
			sprintf(buf,"%sYou float to the ground, coming to rest in a soft grassy meadow at the edge of a forest.%s\n\r",get_char_color(ch,"lightgrey"),END_COLOR(ch));
			send_to_char(buf,ch);
			tribe = number_range(1,10);
			ch->pcdata->tribe = tribe;
			af->modifier++;
			RS.Queue.AddToQueue(5, 1, communion_handler, ch);
			break;
		case (19):
			af->modifier++;
			char_from_room(ch);
			char_to_room(ch,get_room_index(23633));
			ch->position = POS_STANDING;
			if (IS_SET(ch->comm,COMM_BRIEF)) {
				REMOVE_BIT(ch->comm,COMM_BRIEF);
				do_look(ch,"auto");
				SET_BIT(ch->comm,COMM_BRIEF);
			} else {
				do_look(ch,"auto");
			}
			RS.Queue.AddToQueue(2, 1, communion_handler, ch);
			break;
		case (20):
			sprintf(buf,"%sYou see a blur of motion streaking towards you from the edge of the forest.%s\n\r",get_char_color(ch,"white"),END_COLOR(ch));
			send_to_char(buf,ch);
			af->modifier++;
			RS.Queue.AddToQueue(2, 1, communion_handler, ch);
			break;
		case (21):
			af->modifier++;
			animal = create_mobile(get_mob_index(23630));
			switch (ch->pcdata->tribe) {
				case(1):
					sprintf(name,"boar ferocious horde");
					sprintf(short_desc,"a ferocious boar");
					sprintf(long_desc,"A ferocious long-tusked boar charges madly toward you!");
					break;
				case(2):
					sprintf(name,"wolf feral horde");
					sprintf(short_desc,"a feral wolf");
					sprintf(long_desc,"A massive wolf with silvery fur howls a challenge and leaps at you!");
					break;
				case(3):
					sprintf(name,"bear massive horde");
					sprintf(short_desc,"a massive bear");
					sprintf(long_desc,"A brown bear, spotted with fresh blood, roars a challenge and charges!");
					break;
				case(4):
					sprintf(name,"hawk steely-eyed horde");
					sprintf(short_desc,"a steely-eyed hawk");
					sprintf(long_desc,"An unusually large steely-eyed hawk screeches and dives towards you!");
					break;
				case(5):
					sprintf(name,"lion fierce horde");
					sprintf(short_desc,"a fierce lion");
					sprintf(long_desc,"A fierce golden lion roars a mighty challenge and bounds towards you!");
					break;
				case(6):
					sprintf(name,"elk majestic horde");
					sprintf(short_desc,"a majestic elk");
					sprintf(long_desc,"A majestic elk looms before you, his massive antlers tinged with blood.");
					break;
				case(7):
					sprintf(name,"jackal spotted horde");
					sprintf(short_desc,"a spotted jackal");
					sprintf(long_desc,"A spotted jackal circles you warily and darts in to strike!");
					break;
				case(8):
					sprintf(name,"fox wily fox");
					sprintf(short_desc,"a wily fox");
					sprintf(long_desc,"A wily fox darts rapidly in and out of reach, biting and clawing at you!");
					break;
				case(9):
					sprintf(name,"bull mighty horde");
					sprintf(short_desc,"a mighty bull");
					sprintf(long_desc,"A mighty bull's fetid breath mists in the air as he charges towards you!");
					break;
				case(10):
					sprintf(name,"panther savage horde");
					sprintf(short_desc,"a savage panther");
					sprintf(long_desc,"A savage panther roars and streaks fluidly towards you!");
					break;
				default:
					send_to_char("Something is awry with the communion.  Please pray and ask for help.\n\r",ch);
					affect_strip(ch,gsn_horde_communion);
					return;
			}

			free_pstring(animal->name);
			animal->name = palloc_string(name);
			free_pstring(animal->short_descr);
			animal->short_descr = palloc_string(short_desc);
			free_pstring(animal->long_descr);
			animal->long_descr = palloc_string(long_desc);

			animal->level = ch->level + 10;

			animal->damage[DICE_NUMBER] = ch->level;
			animal->damage[DICE_TYPE] = 3;
			animal->damage[DICE_BONUS] = ch->level;

			animal->max_hit = ch->level * 200;
			animal->hit = animal->max_hit;

			char_to_room(animal,get_room_index(23633));
			one_hit(animal,ch,TYPE_UNDEFINED);
			break;
		case (22):
			REMOVE_BIT(ch->comm,COMM_PROMPT);
			sprintf(buf,"%sBlood streaming from your wounds, your vision clouds as you slump to the ground.%s\n\r",get_char_color(ch,"lightred"),END_COLOR(ch));
			send_to_char(buf,ch);
			char_from_room(ch);
			char_to_room(ch,get_room_index(23628));
			ch->position = POS_SLEEPING;
			for (animal = (get_room_index(23633))->people; animal; animal = animal->next_in_room) {
				if (IS_NPC(animal) && animal->pIndexData->vnum == 23630) {
					extract_char(animal,TRUE);
					break;
				}
			}
				
			af->modifier++;
			RS.Queue.AddToQueue(8, 1, communion_handler, ch);
			break;
		case (23):
			sprintf(buf,"%sRays of light pierce your eyelids as they flutter open, and you groggily stagger to your feet.%s\n\r",get_char_color(ch,"white"),END_COLOR(ch));
			send_to_char(buf,ch);
			REMOVE_BIT(ch->affected_by,AFF_SLEEP);
			ch->position = POS_STANDING;
			af->modifier++;
			RS.Queue.AddToQueue(5, 1, communion_handler, ch);
			break;
		case (24):
			sprintf(buf,"%sA trophy belt is fastened securely about your waist, and you sense the weight of a massive pelt upon your back.%s\n\r",get_char_color(ch,"lightblue"),END_COLOR(ch));
			send_to_char(buf,ch);
			af->modifier++;
			ch->cabal = CABAL_HORDE;
			ch->pcdata->cabal_level = cabal_table[ch->cabal].start_level;
			update_cskills(ch);
			cabal_members[CABAL_HORDE]++;
			if (IS_IMMORTAL(af->owner) && IS_IMMORTAL(ch))
			{
				sprintf(query,
						"insert into inductions(ch, victim, cabal, ctime, chsite, victimsite) values('%s','%s',%d,%ld,'%s','%s')", 
						af->owner->true_name,                                                                                    
					    ch->true_name,                                                                                
					    CABAL_HORDE,                                                                                            
					    current_time,                                                                                     
					    af->owner->pcdata->host,                                                                                 
					    ch->pcdata->host);                                                                            
		        one_query(query);
			}
			obj = create_object(get_obj_index(OBJ_VNUM_TROPHY_BELT), 60);
			obj_to_char(obj,ch);
			if ((remove = get_eq_char(ch,WEAR_WAIST)) != NULL)
				unequip_char(ch,remove,FALSE);
			equip_char(ch,obj,WEAR_WAIST,FALSE);
			
			switch (ch->pcdata->tribe) {
				case (1):
					pelt = 23634;
					break;
				case (2):
					pelt = 23640;
					break;
				case (3):
					pelt = 23635;
					break;
				case (4):
					pelt = 23641;
					break;
				case (5):
					pelt = 23636;
					break;
				case (6):
					pelt = 23638;
					break;
				case (7):
					pelt = 23639;
					break;
				case (8):
					pelt = 23642;
					break;
				case (9):
					pelt = 23633;
					break;
				case (10):
					pelt = 23637;
					break;
				default:
					send_to_char("Something is awry with the communion.  Please pray and ask for help.\n\r",ch);
					affect_strip(ch,gsn_horde_communion);
					return;
			}
			
			obj = create_object(get_obj_index(pelt),60);
			obj_to_char(obj,ch);
			if ((remove = get_eq_char(ch,WEAR_ABOUT)) != NULL)
				unequip_char(ch,remove,FALSE);
			equip_char(ch,obj,WEAR_ABOUT,FALSE);


			
			RS.Queue.AddToQueue(5, 1, communion_handler, ch);
			break;
		case (25):
			sprintf(buf,"%sEven as your mind reels, trying to make sense of all you have seen, you feel infused with newfound vigor.%s\n\r",get_char_color(ch,"lightblue"),END_COLOR(ch));
			send_to_char(buf,ch);
			ch->hit = ch->max_hit;
			ch->mana = ch->max_mana;
			af->modifier++;
			RS.Queue.AddToQueue(5, 1, communion_handler, ch);
			break;
		case (26):
			sprintf(buf,"%sRays of light stream from the path to the north.  As you turn to leave, you are struck with the thought that what you have witnessed in this sacred glade was meant for you and you alone, and should not be shared with others who have yet to be tested.%s\n\r",get_char_color(ch,"lightblue"),END_COLOR(ch));
			SET_BIT(ch->comm,COMM_PROMPT);
			send_to_char(buf,ch);
			affect_strip(ch,gsn_horde_communion);
			return;
		default:
			break;
	}

	return;
}

void pulse_prog_cimar_babies(OBJ_DATA *obj, bool isTick)
{
	CHAR_DATA *ch = obj->carried_by;

	if(ch == NULL || IS_NPC(ch))
		return;
	
	if(number_percent() <= 98 || is_affected_obj(obj,gsn_bash))
		return;

	act("$p begins wailing, its squalling face turning bright red.",ch,obj,0,TO_CHAR);
	act("$n's baby begins wailing...",ch,obj,0,TO_ROOM);

	return;
}

void verb_prog_feed_baby(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	OBJ_DATA *milk;
	OBJ_AFFECT_DATA af,af2;
	bool found = FALSE;

	if(!is_worn(obj))
		return send_to_char("You must first hold the baby in order to feed it.\n\r",ch);

	for(milk = ch->carrying; milk != NULL; milk=milk->next_content) {
		if(milk->pIndexData->vnum == 2454) {
			found = TRUE;
			break;
		}
	}

	if(!found || milk->value[1] < 5)
		return send_to_char("You don't have any milk to feed it!\n\r",ch);
	
	act("$p greedily begins to feed from a bladder of milk.",ch,obj,0,TO_CHAR);
	act("$n's baby greedily begins to feed.",ch,0,0,TO_ROOM);
	milk->value[1] = 0;
	
	RS.Queue.AddToQueue(12, 5, act, "$p drifts off to sleep.", ch, obj, 0, TO_CHAR);
	
	init_affect_obj(&af);
	af.where = TO_OBJ_APPLY;
	af.aftype = AFT_SPELL;
	af.type = gsn_bash;
	af.modifier = 0;
	af.duration = 6;
	af.end_fun = baby_end;
	affect_to_obj(obj,&af);

	init_affect_obj(&af2);
	af2.where = TO_OBJ_APPLY;
	af2.aftype = AFT_SPELL;
	af2.type = gsn_trip;
	af2.modifier = 0;
	af2.duration = 1;
	af2.end_fun = baby_burp;
	affect_to_obj(obj,&af2);

}	

void baby_end(OBJ_DATA *obj, OBJ_AFFECT_DATA *af)
{
	CHAR_DATA *ch = obj->carried_by;

	if(ch == NULL)
		return;

	act("$p suddenly wakens and looks around curiously.",ch,obj,0,TO_CHAR);
	act("$n's baby wakes up.",ch,0,0,TO_ROOM);

	return;
}

void baby_burp(OBJ_DATA *obj, OBJ_AFFECT_DATA *af)
{
	CHAR_DATA *ch = obj->carried_by;

	if(ch == NULL)
		return;
	
	act("$p burps loudly.",ch,obj,0,TO_CHAR);
	act("$n's baby burps loudly.",ch,0,0,TO_ROOM);

	return;
}

void verb_prog_pull_book(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	EXIT_DATA *pexit = ch->in_room->exit[DIR_DOWN];
	ROOM_AFFECT_DATA raf;
		
	if(!IS_SET(pexit->exit_info,EX_CLOSED)) {
		act("The book snaps back into the shelf.",ch,0,0,TO_ALL);
		return;
	}

	REMOVE_BIT(pexit->exit_info,EX_LOCKED);
	REMOVE_BIT(pexit->exit_info,EX_CLOSED);
	REMOVE_BIT(pexit->exit_info,EX_NONOBVIOUS);
	
	act("You pull The Tome of the Caverns from the shelf.",ch,0,0,TO_CHAR);
	act("$n pulls a book from the shelf.",ch,0,0,TO_ROOM);	
	act("The book snaps back into the shelf, and a creaking sound fills the air!",ch,0,0,TO_ALL);
	act("A trap door in the floor swings open.",ch,0,0,TO_ALL);	

	init_affect_room(&raf);
	raf.where = TO_ROOM_AFFECTS;
	raf.type = gsn_kick;
	raf.aftype = AFT_INVIS;
	raf.location = 0;
	raf.modifier = 0;
	raf.duration = 1;
	raf.end_fun = trapdoor_end;
	new_affect_to_room(ch->in_room,&raf);

	return;
}

/* Fix0red by Morglum */
void trapdoor_end(ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *af)
{
	EXIT_DATA *pexit = room->exit[DIR_DOWN];
	CHAR_DATA *ch, *ch_next;

	SET_BIT(pexit->exit_info,EX_CLOSED);
	SET_BIT(pexit->exit_info,EX_LOCKED);
	SET_BIT(pexit->exit_info,EX_NONOBVIOUS);

	for(ch = char_list; ch != NULL; ch=ch_next) {
		ch_next = ch->next;
		if(ch->in_room == room)
			act("A creaking sound fills the air, and a trap door in the floor swings shut.",ch,0,0,TO_CHAR);
			
		}
	return;	
}


bool open_prog_beef_balls(OBJ_DATA *obj, CHAR_DATA *ch)
{
	CHAR_DATA *mob;
	bool found = FALSE;

	for(mob = char_list; mob != NULL; mob = mob->next)
	{
		if(IS_NPC(mob))
		{
			if(mob->pIndexData->vnum == 24549) 
			{
				found = TRUE;
				break;
			}
		}
	}

	act("The trapped aroma of the beef balls wafts through the air like a brick.",ch,0,0,TO_CHAR);

    if(found && ch->pcdata->quests[PETE_QUEST] == 1)
	{
		act("$N's eyes go wild at the odor, and he leaps to his feet!",ch,0,mob,TO_ALL);
        mprog_say(2,"Please!  Please, please, please, Pete must have that bag, please!  Will please give you information you like!"
				,mob,ch);
		ch->pcdata->quests[PETE_QUEST] = 2;
	}
	return TRUE;
}

void verb_prog_look_topbounties(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	do_topbounties(ch,"");

	return;
}

void verb_prog_pour_wine(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	OBJ_DATA *obj2;
	bool found = FALSE;	
	OBJ_AFFECT_DATA af;

	for(obj2 = ch->carrying; obj2 != NULL; obj2 = obj2->next_content) {
		if(obj2->pIndexData->vnum == 2412) {
			found = TRUE;
			break;
			}
		}

	if(!found)
		return send_to_char("You don't have anything to pour the wine into!\n\r",ch);

	if(obj->value[1] < 1)
		return send_to_char("The skin is already empty.\n\r",ch);

	if(obj2->value[1] > 0)
		return send_to_char("The glass is full.\n\r",ch);

	obj->value[1] -= 1;
	obj2->value[1] += 1;

	SET_BIT(obj2->extra_flags,ITEM_GLOW);

	send_to_char("Your pour rose colored wine into a crystal wine flute.\n\r",ch);
	send_to_char("A crystal wine flute begins to glow with a soft, mysterious light!\n\r",ch);

	init_affect_obj(&af);
	af.where = TO_OBJ_AFFECTS;
	af.duration = -1;
	af.type = gsn_bash;
	af.modifier = 0;
	af.location = 0;
	af.pulse_fun = wine_pulse;
	affect_to_obj(obj2,&af);
	return;
}

void wine_pulse(OBJ_DATA *obj, OBJ_AFFECT_DATA *af)
{
	CHAR_DATA *ch = obj->carried_by;
	
	if(obj->value[1] == 0) {
		REMOVE_BIT(obj->extra_flags,ITEM_GLOW);
		send_to_char("As the wine drains from the flute, the light from within it fades.\n\r",ch);
		affect_strip_obj(obj,gsn_bash);
		return;
	}

return;

}

void verb_prog_attach_weapon(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	OBJ_DATA *pole, *finished;
	bool found = FALSE;

	for(pole = ch->carrying; pole != NULL; pole = pole->next_content) {
		if(pole->pIndexData->vnum == 2431) {
			found = TRUE;
			break;
		}
	}
	
	if(!found)
		return send_to_char("You don't have anything to attach it to!\n\r",ch);
	
	act("You attach $p to a long wooden pole.",ch,obj,0,TO_CHAR);

	switch(obj->pIndexData->vnum) {
		case(2432):
			obj_from_char(obj);
			obj_from_char(pole);
			extract_obj(obj);
			extract_obj(pole);
			finished = create_object(get_obj_index(2461),18);
			obj_to_char(finished,ch);
			break;
		case(2433):
			obj_from_char(obj);
			obj_from_char(pole);
			extract_obj(obj);
			extract_obj(pole);
			finished = create_object(get_obj_index(2462),18);
			obj_to_char(finished,ch);
			break;
		case(2434):
			obj_from_char(obj);
			obj_from_char(pole);
			extract_obj(obj);
			extract_obj(pole);
			finished = create_object(get_obj_index(2463),18);
			obj_to_char(finished,ch);
			break;
		case(2435):
			obj_from_char(obj);
			obj_from_char(pole);
			extract_obj(obj);
			extract_obj(pole);
			finished = create_object(get_obj_index(2464),18);
			obj_to_char(finished,ch);
			break;
	}

	return;
}

void verb_prog_join_guild(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	if(ch->cabal)
		return send_to_char("You are already in a cabal!\n\r",ch);

	ch->cabal = CABAL_GUILD;
	ch->pcdata->cabal_level = 1;
	update_cskills(ch);
	send_to_char("You have been inducted into the Common Guild of Shalar.\n\r",ch);
	return;
}

void verb_prog_pull_lever(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *mob, *check;
	int vnum=0;
	char buf[MSL],buf2[MSL];
	bool found = FALSE;

	for(check = char_list; check != NULL; check=check->next) {
		if(IS_NPC(check) && 
		check->in_room == ch->in_room && 
		(check->pIndexData->vnum == 24538 || 
		check->pIndexData->vnum == 24539 || 
		check->pIndexData->vnum == 24540)) {
			found = TRUE;
			break;
		}
	}
	
	if(found)
		return send_to_char("There is already a monster on the loose in this room!\n\r",ch);

	switch(obj->pIndexData->vnum) {
		case(24538):
			vnum = 24538;
			sprintf(buf2,"As $n pulls the blue lever, the steel cage door creaks open.");
			sprintf(buf,"As you pull the blue lever, the steel cage door creaks open, and a gibberling leaps forth!");
			break;
		case(24539):
			vnum = 24539;
			sprintf(buf2,"As $n pulls the red lever, the steel cage door creaks open.");
			sprintf(buf,"As you pull the red lever, the steel cage door creaks open, and a goblin leaps forth!");
			break;
		case(24540):
			vnum = 24540;
			sprintf(buf2,"As $n pulls the black lever, the steel cage door creaks open.");
			sprintf(buf,"As you pull the black lever, the steel cage door creaks open, and a mastiff leaps forth!");
			break;
		}

	mob = create_mobile(get_mob_index(vnum));
	char_to_room(mob,ch->in_room);
	act(buf2,ch,0,0,TO_ROOM);
	act(buf,ch,0,0,TO_CHAR);
	one_hit(ch,mob,TYPE_UNDEFINED);
	WAIT_STATE(ch,PULSE_VIOLENCE);
	return;
}

void verb_prog_tie_rope(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	OBJ_DATA *obj2;
	OBJ_AFFECT_DATA oaf;

	if(ch->in_room->vnum != 24548)
		return send_to_char("There is nothing to tie the rope to here!\n\r",ch);
	for(obj2 = ch->in_room->contents; obj2 != NULL; obj2 = obj2->next_content) {
		if(obj2->pIndexData->vnum == 24548)
			break;
	}

	if(is_affected_obj(obj2,gsn_trip))
		return send_to_char("There is already a rope tied to that wheel!\n\r",ch);

	act("You tie a length of rope between the wheel and the metal plate.",ch,0,0,TO_CHAR);
	act("$n ties a length of rope between the wheel and the metal plate.",ch,0,0,TO_ROOM);
	
	init_affect_obj(&oaf);
	oaf.where = TO_OBJ_AFFECTS;
	oaf.duration = 12;
	oaf.type = gsn_trip;
	oaf.end_fun = rope_end;
	affect_to_obj(obj2,&oaf);
	
	obj_from_char(obj);
	extract_obj(obj);
	return;
	
}

void verb_prog_turn_wheel(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	EXIT_DATA *exit = obj->in_room->exit[DIR_DOWN];

	if(!is_affected_obj(obj,gsn_trip))
		return send_to_char("The wheel spins uselessly, connected to nothing.\n\r",ch);

	REMOVE_BIT(exit->exit_info,EX_NONOBVIOUS);
	REMOVE_BIT(exit->exit_info,EX_LOCKED);
	REMOVE_BIT(exit->exit_info,EX_NOPASS);
	REMOVE_BIT(exit->exit_info,EX_CLOSED);

	act("You spin the wheel like a crank, and the rope hauls the metal plate up easily!",ch,0,0,TO_CHAR);
	act("$n spins the wheel and a metal plate in the floor opens!",ch,0,0,TO_ROOM);

	return;
}

void rope_end(OBJ_DATA *obj, OBJ_AFFECT_DATA *af)
{
	EXIT_DATA *exit = obj->in_room->exit[DIR_DOWN];
	ROOM_INDEX_DATA *room = obj->in_room;

	SET_BIT(exit->exit_info,EX_NONOBVIOUS);
	SET_BIT(exit->exit_info,EX_LOCKED);
	SET_BIT(exit->exit_info,EX_NOPASS);
	SET_BIT(exit->exit_info,EX_LOCKED);

	if(!room->people)
		return;
	act("As the rope breaks, the metal plate slams back into the floor.",room->people,0,0,TO_ALL);
	return;
}

void verb_prog_tilt_bust(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	EXIT_DATA *exit = ch->in_room->exit[DIR_SOUTH];

	if(!IS_SET(exit->exit_info, EX_CLOSED)) {
		act("$n tilts the bust backwards.",ch,0,0,TO_ROOM);
		act("You tilt the bust backwards.",ch,0,0,TO_CHAR);
		return;
	}

	act("You tilt the bust backwards.",ch,0,0,TO_CHAR);
	act("$n tilts the bust backwards.",ch,0,0,TO_ROOM);
	act("The south wall slides back silently, revealing a doorway!",ch,0,0,TO_ALL);

	REMOVE_BIT(exit->exit_info, EX_LOCKED);
	REMOVE_BIT(exit->exit_info, EX_CLOSED);
	REMOVE_BIT(exit->exit_info, EX_NONOBVIOUS);
	
	return;
}

void verb_prog_roll_tablet(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	EXIT_DATA *exit = ch->in_room->exit[DIR_EAST];

	if(ch->fighting != NULL)
		return send_to_char("You're too busy fighting to roll the tablet!\n\r",ch);

	if(!IS_SET(exit->exit_info,EX_NONOBVIOUS))
		return send_to_char("The stone is rolled as far as possible.\n\r",ch);

	REMOVE_BIT(exit->exit_info,EX_NONOBVIOUS);
	REMOVE_BIT(exit->exit_info,EX_CLOSED);
	REMOVE_BIT(exit->exit_info,EX_LOCKED);

	act("Grunting with the effort, you slowly roll the huge stone tablet to the side of the tunnel.",ch,0,0,TO_CHAR);

	return;
}

void close_elevator(ROOM_INDEX_DATA *pRoom)
{
	int i;
	ROOM_INDEX_DATA *toRoom;
	const char mmsg [] = "The lift doors close with a loud clang.";
	for(i=0;i<MAX_DIR;i++)
		if(pRoom->exit[i] && pRoom->exit[i]->u1.to_room)
		{
			act(mmsg,pRoom->exit[i]->u1.to_room->people,0,0,TO_ALL);
			act(mmsg,pRoom->people,0,0,TO_ALL);
			toRoom = pRoom->exit[i]->u1.to_room;
			if(toRoom->exit[reverse_d(i)]->u1.to_room)
				toRoom->exit[reverse_d(i)]->u1.to_room = NULL;
			pRoom->exit[i]->u1.to_room = NULL;
		}
}

void open_elevator(ROOM_INDEX_DATA *eleRoom, ROOM_INDEX_DATA *toRoom)
{
	int i;
	const char omsg[] = "The doors to the lift slide open with a loud clang.";
	act(omsg,eleRoom->people,0,0,TO_ALL);
	act(omsg,toRoom->people,0,0,TO_ALL);
	eleRoom->mana_rate = 100;
	for(i=0;i<MAX_DIR;i++)
		if(toRoom->exit[i] && !toRoom->exit[i]->u1.to_room)
		{
			toRoom->exit[i]->u1.to_room = eleRoom;
			if(!eleRoom->exit[reverse_d(i)])
				return bug("Invalid eleRoom exit in Open_Elevator",0);
			eleRoom->exit[reverse_d(i)]->u1.to_room = toRoom;
		}
}

void act_to_room(void *vo1, void *vo2)
{
	ROOM_INDEX_DATA *pRoom = (ROOM_INDEX_DATA *)vo2;
	if(!pRoom->people)
		return;

	act((char*)vo1,pRoom->people,0,0,TO_ALL);
}
void verb_prog_iseldheim_lever_pull(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{

	ROOM_INDEX_DATA *lRoom = ch->in_room, *eleRoom, *tRoom = NULL;
	int eDir = obj->value[0], i;
	bool elInTransit = TRUE;
	char *mmsg;
	eleRoom = get_room_index(obj->value[2]);

	if(!eleRoom)
		return;

	for(i=0; i<MAX_DIR; i++)
		if(eleRoom->exit[i] && eleRoom->exit[i]->u1.to_room)
		{
			tRoom = eleRoom->exit[i]->u1.to_room;
			elInTransit = FALSE;
		}
	if(elInTransit || eleRoom->mana_rate == 101)
	{
		send_to_char("You pull the lever, but nothing seems to happen.\n\r",ch);
		act("$n pulls the lever, but nothing seems to happen.",ch,0,0,TO_ROOM);
		return;
	}
	if(!tRoom)
		return bug("Whack ass shit up in dis lizzever!",0);

	mmsg = talloc_string("A loud grinding noise can be heard as the gears above the lift begin to turn.");
	eleRoom->mana_rate = 101;	//ele in use
	if(!lRoom->exit[eDir]->u1.to_room)
	{ //call elevator SIR!
		RS.Queue.AddToQueue(2, 2, act_to_room, mmsg, lRoom);
		RS.Queue.AddToQueue(2, 2, act_to_room, mmsg, tRoom);
		RS.Queue.AddToQueue(2, 2, act_to_room, mmsg, eleRoom);
		RS.Queue.AddToQueue(3, 1, close_elevator, eleRoom);
		RS.Queue.AddToQueue(5, 2, act_to_room,(char*)"With a lurch, the lift begins to accelerate.", eleRoom);
		RS.Queue.AddToQueue(9, 2, act_to_room, (char*)"The lift shudders as it slowly comes to a halt.", eleRoom);
		RS.Queue.AddToQueue(10, 2, open_elevator, eleRoom, lRoom);
		act("You pull the lever into the down position.",ch,0,0,TO_CHAR);
		act("$n pulls the lever into the down position.",ch,0,0,TO_ROOM);
		return;
	}
	else if(lRoom->exit[eDir]->u1.to_room)
	{ //elevator is here, we can send it back up
		tRoom = get_room_index(obj->value[1]);
		RS.Queue.AddToQueue(2, 2, act_to_room, mmsg, lRoom);
		RS.Queue.AddToQueue(2, 2, act_to_room, mmsg, tRoom);
		RS.Queue.AddToQueue(2, 2, act_to_room, mmsg, eleRoom);
		RS.Queue.AddToQueue(3, 1, close_elevator, eleRoom);
		RS.Queue.AddToQueue(5, 2, act_to_room,(char*)"With a lurch, the lift begins to accelerate.", eleRoom);
		RS.Queue.AddToQueue(9, 2, act_to_room, (char*)"The lift shudders as it slowly comes to a halt.", eleRoom);
		RS.Queue.AddToQueue(10, 2, open_elevator, eleRoom, tRoom);
		act("You pull the lever into the up position.",ch,0,0,TO_CHAR);
		act("$n pulls the lever into the up position.",ch,0,0,TO_ROOM);
	}
	
}

void fight_prog_bugzapper(OBJ_DATA *obj, CHAR_DATA *ch)
{
	CHAR_DATA *victim = ch->fighting;
	if(!victim || !IS_NPC(victim) || victim->pIndexData->vnum < 3000 || victim->pIndexData->vnum > 3010 || !is_worn(obj))
		return;
	act("$n shrinks back suddenly at the sight of $p.",victim,obj,0,TO_ROOM);
	act("Imbued with newfound vigor, you hack mercilessly at $n.",victim,0,ch,TO_VICT);
	act("$N suddenly strikes $n with a brutal blow.",victim,0,ch,TO_NOTVICT);
	one_hit_new(ch,victim,TYPE_UNDEFINED,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,700,NULL);	
}

void fight_prog_arms_light(OBJ_DATA *obj, CHAR_DATA *ch)
{
	AFFECT_DATA af;
	CHAR_DATA *victim = ch->fighting;
	if(!victim)
		return;
	if(!is_affected_obj(obj, gsn_arms_of_light))
		return;
	if(number_percent() > 90)
	{
		act("$p suddenly flares brightly!",victim,obj,0,TO_ROOM);
		if(!saves_spell(ch->level,victim,DAM_LIGHT))
		{
			act("$n appears to be blinded.",victim,obj,0,TO_ROOM);
			act("You are blinded!",ch,0,victim,TO_VICT);
			init_affect(&af);
			af.where = TO_AFFECTS;
			af.aftype = AFT_SKILL;
			af.type = gsn_blindness;
			af.duration = ch->level/12;
			af.modifier = -4;
			af.location = APPLY_HITROLL;
			SET_BIT(af.bitvector,AFF_BLIND);
			affect_to_char(victim,&af);
		}
	}
return;
}

void drop_prog_elven_gem(OBJ_DATA *obj, CHAR_DATA *ch)
{
	OBJ_DATA *obj2;

	for(obj2=ch->in_room->contents; obj2 != NULL; obj2=obj2->next_content) {
		if(obj->pIndexData->vnum == obj2->pIndexData->vnum) {
			act("As $p falls to the floor, it disintegrates and becomes a cloud of dust.",ch,obj,0,TO_ROOM);
			extract_obj(obj);
			break;
		}
	}

return;

}

void speech_prog_elven_mirror(OBJ_DATA *obj, CHAR_DATA *ch, char *speech)
{
	char arg1[MSL], arg2[MSL], arg3[MSL], arg4[MSL], buf[MSL];
	CHAR_DATA *victim;
	char *sect;
	ROOM_INDEX_DATA *room,*room2=ch->in_room;
	int rand;
	
	if(!is_worn(obj))
		return;

	half_chop(speech,arg1,arg2);

	half_chop(arg2,arg3,arg4);
	if(str_prefix(arg1,"show"))
		return;
 	
	if(!arg4[0] || !(victim=get_char_world(ch,arg4)))
		return;

	room = victim->in_room;
	act("$p begins to change colour as it attempts to find $N.",ch,obj,victim,TO_ROOM);
	
	switch(room->sector_type) {
		case(SECT_INSIDE):
			sect = talloc_string("inside a room");
			break;
		case(SECT_CITY):
			sect = talloc_string("in a city");		
			break;
		case(SECT_FOREST):
			sect = talloc_string("in a forest");
			break;
		case(SECT_MOUNTAIN):
			sect = talloc_string("in the mountains");
			break;
		case(SECT_FIELD):
			sect = talloc_string("in a field");
			break;
		case(SECT_HILLS):
			sect = talloc_string("among some hills");
			break;
		case(SECT_WATER):
			sect = talloc_string("on a body of water");
			break;
		case(SECT_UNDERWATER):
			sect = talloc_string("underwater");
			break;
		case(SECT_AIR):
			sect = talloc_string("in mid-air");
			break;
		case(SECT_CONFLAGRATION):
			sect = talloc_string("amidst a raging fire");
			break;
		case(SECT_BURNING):
			sect = talloc_string("amidst a raging fire");
			break;
		case(SECT_DESERT):
			sect = talloc_string("in a parched desert");
			break;
		case(SECT_ROAD):
			sect = talloc_string("on a road");
			break;
		case(SECT_TRAIL):
			sect = talloc_string("along a trail");
			break;
		case(SECT_SWAMP):
			sect = talloc_string("in a swamp");
			break;
		case(SECT_PARK):
			sect = talloc_string("in a park");
			break;
		case(SECT_VERTICAL):
			sect = talloc_string("near some cliffs");
			break;
		case(SECT_ICE):
			sect = talloc_string("amidst snow and ice");
			break;
		case(SECT_SNOW):
			sect = talloc_string("amidst snow and ice");
			break;
		case(SECT_CAVE):
			sect = talloc_string("in a cave");
			break;
		default:
			sect = talloc_string("but cannot make out their surroundings");
	}
	rand = number_percent();

	if(rand == 1) {
		act("$p suddenly stabilizes into a perfect image of $N!",ch,obj,victim,TO_CHAR);
		char_from_room(ch);
		char_to_room(ch,room);
		do_look(ch,"auto");
		char_from_room(ch);
		char_to_room(ch,room2);
		WAIT_STATE(ch,PULSE_VIOLENCE*2);
		free_pstring(obj->description);
		sprintf(buf,"A curious mirror lies here, humming softly.  Peering into the glass, you see %s standing in %s.",victim->name,get_room_name(room));
		obj->description = palloc_string(buf);
		return;
	}

	if(rand > 1 && rand < 90) {
		act("$p forms into a hazy image of $N.",ch,obj,victim,TO_CHAR);
		sprintf(buf,"Peering into the mirror, you see $N %s.",sect);
		act(buf,ch,0,victim,TO_CHAR);
		free_pstring(obj->description);
		sprintf(buf,"A curious mirror lies here, humming softly.  Peering into the glass, you see %s %s.",victim->name,sect);
		obj->description = palloc_string(buf);
		return;
	}

	if(rand > 89) {
		act("$p fails to form into a coherent image.",ch,obj,0,TO_CHAR);
		return;
	}

return;
}

void verb_prog_turn_wyntran(OBJ_DATA *obj, CHAR_DATA *ch, char *argument) {
	MOB_INDEX_DATA *pMobIndex;
	CHAR_DATA *victim;
	char buf[MAX_STRING_LENGTH];

	if((pMobIndex = get_mob_index(4627)) == NULL) {
		bug("verb_prog_turn_wyntran mob loading error");
		return;
	}

	victim = create_mobile( pMobIndex );

	act("The temperature in the room drops rapidly as a darkened portal opens overhead.", ch, 0, 0, TO_ROOM);
	act("The temperature in the room drops rapidly as a darkened portal opens overhead.", ch, 0, 0, TO_CHAR);

	RS.Queue.AddToQueue(3, 5, act, "Acompanied by peals of thunder a shrouded being descends from the portal.", ch, 0, 0, TO_CHAR);
	RS.Queue.AddToQueue(3, 5, act, "Acompanied by peals of thunder a shrouded being descends from the portal.", ch, 0, 0, TO_ROOM);
	RS.Queue.AddToQueue(4, 2, char_to_room, victim, ch->in_room);

	WAIT_STATE(ch, PULSE_VIOLENCE*2);
	
	return;
}

void verb_prog_place_star(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	OBJ_DATA *pContainer, *aStar, *aS_next;
	ROOM_INDEX_DATA *room = ch->in_room, *nroom;
	CHAR_DATA *vch, *vch_next;
	int iStars = 0;
	char buf[MSL];
	for(pContainer = room->contents; pContainer; pContainer = pContainer->next_content)
		if(pContainer->pIndexData->vnum == 4669)
			break;
	if(!pContainer)
		return send_to_char("There is nothing to place it in here.\n\r",ch);
	for(aStar = pContainer->contains; aStar; aStar = aStar->next_content)
		if(!aStar->pIndexData->iprogs || aStar->pIndexData->iprogs->verb_prog != verb_prog_place_star)
			return; //something else got into that container
		else if(aStar->pIndexData->vnum == obj->pIndexData->vnum)
			return send_to_char("That has already been placed into the symbol.\n\r",ch);
		else
			iStars++;
	act("You set $p on the surface of the symbol.",ch,obj,0,TO_CHAR);
	act("$n sets $p on the surface of the symbol.",ch,obj,0,TO_ROOM);
	sprintf(buf,"%s$p rotates rapidly before flying into %s in the symbol with an audible click!%s",
		get_char_color(ch,"yellow"), iStars >= 4 ? "the last indentation" : "one of the indentations", END_COLOR(ch));
	act(buf, ch, obj, 0, TO_ALL);
	obj_from_char(obj);
	obj_to_obj(obj, pContainer);

	if(iStars < 4)
		return;
	
	for(aStar = pContainer->contains; aStar; aStar = aS_next)
	{
		aS_next = aStar->next_content;
		extract_obj(aStar);
	}

	sprintf(buf,"%sAs the last star fills the symbol, a blinding flash fills the room and when you reopen your eyes you are elsewhere!%s", get_char_color(ch, "white"), END_COLOR(ch));
	act(buf, ch, 0, 0, TO_ALL);
	
	if(!(nroom = get_room_index(4649)))
		return;
	for(vch = room->people; vch; vch = vch_next)
	{
		vch_next = vch->next_in_room;
		char_from_room(vch);
		char_to_room(vch, nroom);
		do_look(vch, "auto");
	}
}

// Fallen Desert Progs
void verb_prog_fallendesert_climb_ladder(OBJ_DATA *obj, CHAR_DATA *ch, char *argument) {
        ROOM_INDEX_DATA *to_room = NULL;
	int vnum = 0;

	if(ch->in_room->vnum == 29176) {
		vnum = 29178;
	} else if(ch->in_room->vnum == 29182) {
		vnum = 29183;
	} else if(ch->in_room->vnum == 29192) {
		vnum = 29194;
	} else {
		bug("error climbin ladder in fallen desert, wrong vnum for room");
		return;
	}

	to_room = get_room_index(vnum);
                                                                                                                                                                                                     
        if (ch->fighting)
                return send_to_char("You're somewhat distracted right now.\n\r",ch);
                                                                                                                                                                                                     
        if (ch->position <= POS_SITTING)
                return send_to_char("You need to be standing up to try that!\n\r",ch);
                                                                                                                                                                                                     
        act("The ladder gives slightly from your weight as you climb into the tent.",ch,0,0,TO_CHAR);
        act("The ladder gives slightly from $n's weight as $e climbs into the tent.",ch,0,0,TO_ROOM);
                                                                                                                                                                                                     
        char_from_room(ch);
        char_to_room(ch,to_room);
        do_look(ch,"auto");
                                                                                                                                                                                                     
        return;
}

void verb_prog_fallendesert_(OBJ_DATA *obj, CHAR_DATA *ch, char *argument) {
        ROOM_INDEX_DATA *to_room = NULL;
        int vnum = 0;
                                                                                                                                                                                                     
        if(ch->in_room->vnum == 29176) {
                vnum = 29178;
        } else if(ch->in_room->vnum == 29182) {
                vnum = 29183;
        } else if(ch->in_room->vnum == 29192) {
                vnum = 29194;
        } else {
                bug("error climbin ladder in fallen desert, wrong vnum for room");
                return;
        }
                                                                                                                                                                                                     
        to_room = get_room_index(vnum);
                                                                                                                                                                                                     
        if (ch->fighting)
                return send_to_char("You're somewhat distracted right now.\n\r",ch);
                                                                                                                                                                                                     
        if (ch->position <= POS_SITTING)
                return send_to_char("You need to be standing up to try that!\n\r",ch);
                                                                                                                                                                                                     
        act("The ladder gives slightly from your weight as you climb into the tent.",ch,0,0,TO_CHAR);
        act("The ladder gives slightly from $n's weight as $e climbs into the tent.",ch,0,0,TO_ROOM);
                                                                                                                                                                                                     
        char_from_room(ch);
        char_to_room(ch,to_room);
        do_look(ch,"auto");
                                                                                                                                                                                                     
        return;
}
