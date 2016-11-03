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
*	ROM 2.4 is copyright 1993-1996 Russ Taylor			   				   *
*	ROM has been brought to you by the ROM consortium		 			   *
*	    Russ Taylor (rtaylor@pacinfo.com)				  				   *
*	    Gabrielle Taylor (gtaylor@pacinfo.com)			  				   *
*	    Brian Moore (rom@rom.efn.org)									   *
*	By using this code, you have agreed to follow the terms of the		   *
*	ROM license, in the file Tartarus/doc/rom.license                  	   *
***************************************************************************/


#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "magic.h"
#include "interp.h"
#include "recycle.h"
#include "tables.h"

DECLARE_MPROG_FUN_BRIBE( bribe_prog_cityguard );
DECLARE_MPROG_FUN_FIGHT( fight_prog_priest  );

DECLARE_MPROG_FUN_GREET( greet_prog_profteacher );
DECLARE_MPROG_FUN_GREET( greet_prog_knight );
DECLARE_MPROG_FUN_GREET( greet_prog_arangird);
DECLARE_MPROG_FUN_AGGRESS( aggress_prog_arangird_demon);
DECLARE_MPROG_FUN_AGGRESS( aggress_prog_arangird_regthor);
DECLARE_MPROG_FUN_AGGRESS( aggress_prog_ilopheth_hermit);
DECLARE_MPROG_FUN_GREET( greet_prog_ruins_spirit);
DECLARE_MPROG_FUN_GREET( greet_prog_ruins_mouth);
DECLARE_MPROG_FUN_GREET( greet_prog_cimar_dollgirl);
DECLARE_MPROG_FUN_GREET( greet_prog_cimar_sorcgm);
DECLARE_MPROG_FUN_GREET(greet_prog_drow_scribe);
DECLARE_MPROG_FUN_GREET(greet_prog_tower_shopkeeper);
DECLARE_MPROG_FUN_GREET(greet_prog_headmaster);
DECLARE_MPROG_FUN_GREET(greet_prog_opening_greet);
DECLARE_MPROG_FUN_GREET(greet_prog_pay_char);
DECLARE_MPROG_FUN_GREET(greet_prog_starvin_pete);

DECLARE_MPROG_FUN_SPEECH( speech_prog_ruins_mouth);
DECLARE_MPROG_FUN_SPEECH( speech_prog_testmob);
DECLARE_MPROG_FUN_SPEECH( speech_prog_cimar_dollgirl);
DECLARE_MPROG_FUN_SPEECH( speech_prog_ilopheth_hermit);

DECLARE_MPROG_FUN_GIVE( give_prog_cimar_dollgirl);
DECLARE_MPROG_FUN_GIVE( give_prog_ilopheth_hermit);
DECLARE_MPROG_FUN_GIVE( give_prog_starvin_pete);

DECLARE_MPROG_FUN_SPEECH( speech_prog_sailor );
DECLARE_MPROG_FUN_GREET( greet_prog_sailor );

DECLARE_MPROG_FUN_PULSE(pulse_prog_demon);
DECLARE_MPROG_FUN_PULSE(pulse_prog_tahlu_mist_ward);
DECLARE_MPROG_FUN_PULSE(pulse_prog_arangird_patrol);
DECLARE_MPROG_FUN_PULSE(pulse_prog_cornogun);
DECLARE_MPROG_FUN_PULSE(pulse_prog_ilopheth_hermit);
DECLARE_MPROG_FUN_PULSE(pulse_prog_ilopheth_piranha);
DECLARE_MPROG_FUN_PULSE(pulse_prog_ilopheth_weatherbeaten);
DECLARE_MPROG_FUN_PULSE(pulse_prog_alstea_ehrlouge);
DECLARE_MPROG_FUN_PULSE(pulse_prog_formian_queen);


/* Law stuff */
DECLARE_MPROG_FUN_BEAT(beat_prog_law_track);
DECLARE_MPROG_FUN_FIGHT(fight_prog_law_subdue);
DECLARE_MPROG_FUN_PULSE(pulse_prog_troopers);

/* AP Demon Summoning Progs */
DECLARE_MPROG_FUN_ATTACK(attack_prog_lesser_demon);
DECLARE_MPROG_FUN_BEAT(beat_prog_barbas);
DECLARE_MPROG_FUN_DEATH(death_prog_barbas);
DECLARE_MPROG_FUN_SPEECH(speech_prog_aamon);
DECLARE_MPROG_FUN_GREET(greet_prog_furcas);
DECLARE_MPROG_FUN_GIVE(give_prog_malaphar);
DECLARE_MPROG_FUN_GIVE(give_prog_drow_scribe);
DECLARE_MPROG_FUN_BRIBE(bribe_prog_malaphar);
DECLARE_MPROG_FUN_SPEECH(speech_prog_ipos);
DECLARE_MPROG_FUN_SPEECH(speech_prog_oze);
DECLARE_MPROG_FUN_SPEECH(speech_prog_gamygyn);
DECLARE_MPROG_FUN_SPEECH(speech_prog_orobas);
DECLARE_MPROG_FUN_SPEECH(speech_prog_geryon);
DECLARE_MPROG_FUN_SPEECH(speech_prog_cimeries);
DECLARE_MPROG_FUN_SPEECH(speech_prog_drow_scribe);

//DECLARE_MPROG_FUN_FIGHT(fight_prog_warrior_mercenary);
DECLARE_MPROG_FUN_GREET(greet_prog_outer_guardian);
DECLARE_MPROG_FUN_GREET(greet_prog_inner_guardian);
DECLARE_MPROG_FUN_GREET(greet_prog_guild_recruiter);

DECLARE_MPROG_FUN_DEATH(death_prog_outer_guardian);
DECLARE_MPROG_FUN_DEATH(death_prog_inner_guardian);
DECLARE_MPROG_FUN_ATTACK(attack_prog_outer_guardian);

DECLARE_MPROG_FUN_MOVE(move_prog_horde_shrine_ward);
DECLARE_MPROG_FUN_MOVE(move_prog_theatre_guard);
DECLARE_MPROG_FUN_AGGRESS(aggress_prog_anchor);
DECLARE_MPROG_FUN_DEATH(death_prog_glass);
DECLARE_MPROG_FUN_GREET(greet_prog_necro_skull);
DECLARE_MPROG_FUN_DEATH(death_prog_necro_skull);
DECLARE_MPROG_FUN_PULSE(pulse_prog_necro_skull);
DECLARE_MPROG_FUN_PULSE(pulse_prog_shopkeeper);
DECLARE_MPROG_FUN_PULSE(pulse_prog_being_watched);
DECLARE_MPROG_FUN_PULSE(pulse_prog_imp);

DECLARE_MPROG_FUN_ENTRY(entry_prog_cornogun);

DECLARE_MPROG_FUN_FIGHT(fight_prog_geulgon);
DECLARE_MPROG_FUN_FIGHT(fight_prog_gking);
DECLARE_MPROG_FUN_FIGHT(fight_prog_ilopheth_druid);

DECLARE_MPROG_FUN_GIVE(give_prog_minotaur);

DECLARE_MPROG_FUN_PULSE(pulse_prog_diurnal_mob);
DECLARE_MPROG_FUN_PULSE(pulse_prog_nocturnal_mob);
DECLARE_MPROG_FUN_PULSE(pulse_prog_wizard_summon);
DECLARE_MPROG_FUN_PULSE(pulse_prog_fisherman);
DECLARE_MPROG_FUN_PULSE(pulse_prog_area_echo_ward);
DECLARE_MPROG_FUN_PULSE(pulse_prog_shade);
DECLARE_MPROG_FUN_PULSE(pulse_prog_banshee);
DECLARE_MPROG_FUN_PULSE(pulse_prog_phantasm);
DECLARE_MPROG_FUN_PULSE(pulse_prog_ravghoul);
DECLARE_MPROG_FUN_PULSE(pulse_prog_behemoth);
DECLARE_MPROG_FUN_PULSE(pulse_prog_glass);
DECLARE_MPROG_FUN_PULSE(pulse_prog_night_creeps);

DECLARE_MPROG_FUN_GREET(greet_prog_face_sucker);
DECLARE_MPROG_FUN_DEATH(death_prog_cim);

const   struct  improg_type   mprog_table[] =
{
	{	"bribe_prog",	"bribe_prog_cityguard",			(void*)bribe_prog_cityguard,			"None"},
	{	"bribe_prog",	"bribe_prog_malaphar",			(void*)bribe_prog_malaphar,			"None"},
	{	"greet_prog",	"greet_prog_knight",			(void*)greet_prog_knight,				"None"},
	{	"greet_prog",	"greet_prog_sailor",			(void*)greet_prog_sailor,				"None"},
	{	"greet_prog",	"greet_prog_outer_guardian",		(void*)greet_prog_outer_guardian,	"None"},
	{	"greet_prog",	"greet_prog_inner_guardian",		(void*)greet_prog_inner_guardian,	"None"},
	{	"greet_prog",	"greet_prog_ruins_spirit",		(void*)greet_prog_ruins_spirit,		"None"},
	{	"greet_prog",	"greet_prog_ruins_mouth",		(void*)greet_prog_ruins_mouth,			"None"},
	{	"greet_prog",	"greet_prog_furcas",			(void*)greet_prog_furcas,				"None"},
	{	"greet_prog",	"greet_prog_cimar_dollgirl",	(void*)greet_prog_cimar_dollgirl,		"None"},
	{	"greet_prog",	"greet_prog_cimar_sorcgm",		(void*)greet_prog_cimar_sorcgm,		"None"},
	{	"greet_prog",	"greet_prog_drow_scribe",		(void*)greet_prog_drow_scribe,			"None"},
	{	"greet_prog",	"greet_prog_necro_skull",		(void*)greet_prog_necro_skull,			"None"},
	{	"greet_prog",	"greet_prog_arangird",			(void*)greet_prog_arangird,			"None"},
	{	"greet_prog",	"greet_prog_tower_shopkeeper",		(void*)greet_prog_tower_shopkeeper,"None"},
	{	"greet_prog",	"greet_prog_guild_recruiter",		(void*)greet_prog_guild_recruiter,	"Cimar"},
	{	"greet_prog",	"greet_prog_headmaster",		(void*)greet_prog_headmaster,			"None"},
	{	"greet_prog",	"greet_prog_pay_char",			(void*)greet_prog_pay_char,			"None"},
	{	"greet_prog",	"greet_prog_opening_greet",		(void*)greet_prog_opening_greet,		"None"},
	{	"greet_prog",	"greet_prog_starvin_pete",		(void*)greet_prog_starvin_pete,		"None"},
	{	"greet_prog",	"greet_prog_face_sucker",		(void*)greet_prog_face_sucker,			"None"},
	{	"greet_prog",	"greet_prog_profteacher",		(void*)greet_prog_profteacher,		"None"},
	{	"speech_prog",	"speech_prog_ruins_mouth",		(void*)speech_prog_ruins_mouth,		"None"},
	{	"speech_prog",	"speech_prog_testmob",			(void*)speech_prog_testmob,
		"None"},
	{	"speech_prog",	"speech_prog_cimar_dollgirl",	(void*)speech_prog_cimar_dollgirl,		"None"},
	{	"speech_prog",	"speech_prog_ilopheth_hermit",	(void*)speech_prog_ilopheth_hermit,	"None"},
	{	"speech_prog",	"speech_prog_drow_scribe",	(void*)speech_prog_drow_scribe,			"None"},
	{	"fight_prog",	"fight_prog_law_subdue",	(void*)fight_prog_law_subdue,				"None"},
	{	"fight_prog",	"fight_prog_priest",			(void*)fight_prog_priest,				"None"},
	{	"fight_prog",	"fight_prog_ilopheth_druid",	(void*)fight_prog_ilopheth_druid,		"Valley of Ilopheth"},
	{	"fight_prog",	"fight_prog_geulgon",			(void*)fight_prog_geulgon,				"None"},
	{	"fight_prog",	"fight_prog_gking",				(void*)fight_prog_gking,				"None"},
	{	"death_prog",	"death_prog_inner_guardian",		(void*)death_prog_inner_guardian,	"None"},
	{	"death_prog",	"death_prog_outer_guardian",		(void*)death_prog_outer_guardian,	"None"},
	{	"death_prog",	"death_prog_glass",				(void*)death_prog_glass,				"None"},
	{	"death_prog",	"death_prog_necro_skull",		(void*)death_prog_necro_skull,			"None"},
	{	"death_prog",	"death_prog_barbas",			(void*)death_prog_barbas,				"None"},
	{	"death_prog",	"death_prog_cim",				(void*)death_prog_cim,					"Cimar"},
	{	"pulse_prog",	"pulse_prog_demon",				(void*)pulse_prog_demon,				"None"},
	{	"pulse_prog",	"pulse_prog_tahlu_mist_ward",	(void*)pulse_prog_tahlu_mist_ward,		"None"},
	{	"pulse_prog",	"pulse_prog_arangird_patrol",	(void*)pulse_prog_arangird_patrol,		"None"},
	{	"pulse_prog",	"pulse_prog_shopkeeper",		(void*)pulse_prog_shopkeeper,			"None"},
	{	"pulse_prog",	"pulse_prog_being_watched",		(void*)pulse_prog_being_watched,		"None"},
	{	"pulse_prog",	"pulse_prog_ilopheth_piranha",	(void*)pulse_prog_ilopheth_piranha,	"None"},
	{	"pulse_prog",	"pulse_prog_ilopheth_weatherbeaten", (void*)pulse_prog_ilopheth_weatherbeaten, "None" },
	{	"pulse_prog",	"pulse_prog_alstea_ehrlouge",	(void*)pulse_prog_alstea_ehrlouge,	"None"},
	{	"pulse_prog",	"pulse_prog_necro_skull",		(void*)pulse_prog_necro_skull,			"None"},
	{	"pulse_prog",	"pulse_prog_cornogun",			(void*)pulse_prog_cornogun,			"None"},
	{	"pulse_prog",	"pulse_prog_imp",				(void*)pulse_prog_imp,					"None"},
	{	"pulse_prog",	"pulse_prog_nocturnal_mob",		(void*)pulse_prog_nocturnal_mob,		"None"},
	{	"pulse_prog",	"pulse_prog_diurnal_mob",		(void*)pulse_prog_diurnal_mob,			"None"},
	{	"pulse_prog",	"pulse_prog_ilopheth_hermit",	(void*)pulse_prog_ilopheth_hermit,		"None"},
	{	"pulse_prog",	"pulse_prog_wizard_summon",	(void*)pulse_prog_wizard_summon,		"Cimar"},
	{	"pulse_prog",	"pulse_prog_fisherman",		(void*)pulse_prog_fisherman,			"Cimar"},
	{	"pulse_prog",	"pulse_prog_troopers",			(void*)pulse_prog_troopers,			"Cimar"},
	{	"pulse_prog",	"pulse_prog_area_echo_ward",		(void*)pulse_prog_area_echo_ward,		"None"},
	{	"pulse_prog",	"pulse_prog_formian_queen",		(void*)pulse_prog_formian_queen,		"Cim Caverns"},
	{	"pulse_prog",	"pulse_prog_shade",			(void*)pulse_prog_shade,				"None"},
	{	"pulse_prog",	"pulse_prog_banshee",			(void*)pulse_prog_banshee,			"None"},
	{	"pulse_prog",	"pulse_prog_phantasm",			(void*)pulse_prog_phantasm,			"None"},
	{	"pulse_prog",	"pulse_prog_ravghoul",		(void*)	pulse_prog_ravghoul,			"None"},
	{	"pulse_prog",	"pulse_prog_behemoth",			(void*)pulse_prog_behemoth,			"None"},
	{	"pulse_prog",	"pulse_prog_glass",			(void*)pulse_prog_glass,				"None"},
	{	"pulse_prog",	"pulse_prog_night_creeps",		(void*)pulse_prog_night_creeps,		"None"},
	{	"beat_prog",	"beat_prog_barbas",			(void*)beat_prog_barbas,				"None"},
	{	"beat_prog",	"beat_prog_law_track",			(void*)beat_prog_law_track,			"None"},
	{	"speech_prog",	"speech_prog_sailor",			(void*)speech_prog_sailor,				"None"},
	{	"speech_prog",	"speech_prog_aamon",			(void*)speech_prog_aamon,				"Caelumaedan"},
	{	"speech_prog",	"speech_prog_ipos",				(void*)speech_prog_ipos,				"Caelumaedan"},
	{	"speech_prog",	"speech_prog_oze",				(void*)speech_prog_oze,				"Caelumaedan"},
	{	"speech_prog",	"speech_prog_gamygyn",			(void*)speech_prog_gamygyn,			"Caelumaedan"},
	{	"speech_prog",	"speech_prog_orobas",			(void*)speech_prog_orobas,				"Caelumaedan"},
	{	"speech_prog",	"speech_prog_geryon",			(void*)speech_prog_geryon,				"Caelumaedan"},
	{	"speech_prog",	"speech_prog_cimeries",			(void*)speech_prog_cimeries,			"Caelumaedan"},
	{	"speech_prog",	"speech_prog_drow_scribe",		(void*)speech_prog_drow_scribe,		"None"},
	{	"give_prog",	"give_prog_malaphar",			(void*)give_prog_malaphar,				"None"},
	{	"give prog",	"give_prog_minotaur",			(void*)give_prog_minotaur,				"None"},
	{	"give_prog",	"give_prog_cimar_dollgirl",		(void*)give_prog_cimar_dollgirl,		"None"},
	{	"give_prog",	"give_prog_ilopheth_hermit",	(void*)give_prog_ilopheth_hermit,		"None"},
	{	"give_prog",	"give_prog_drow_scribe",	(void*)give_prog_drow_scribe,			"None"},
	{	"give_prog",	"give_prog_starvin_pete",	(void*)give_prog_starvin_pete,			"None"},
	{	"attack_prog",	"attack_prog_lesser_demon",		(void*)attack_prog_lesser_demon,		"None"},
	{	"attack_prog",	"attack_prog_outer_guardian",	(void*)attack_prog_outer_guardian,		"None"},
	{   "aggress_prog",  "aggress_prog_anchor",        	(void*)aggress_prog_anchor,			"None"},
	{	"aggress_prog",	"aggress_prog_arangird_demon",	(void*)aggress_prog_arangird_demon,	"None"},
	{	"aggress_prog",	"aggress_prog_arangird_regthor",(void*)aggress_prog_arangird_regthor,	"None"},
	{	"aggress_prog",	"aggress_prog_ilopheth_hermit",	(void*)aggress_prog_ilopheth_hermit,	"None"},
	{	"move_prog",	"move_prog_horde_shrine_ward",	(void*)move_prog_horde_shrine_ward,	"None"},
	{	"move_prog",	"move_prog_theatre_guard",		(void*)move_prog_theatre_guard,		"None"},
	{	"entry_prog",	"entry_prog_cornogun",			(void*)entry_prog_cornogun,			"None"},
	{	NULL,			NULL,							NULL,							NULL},
};	

void mprog_tell(int inc, char *arg, CHAR_DATA *mob, CHAR_DATA *ch)
{
    char buf[MSL];
    RS.Queue.AddToQueue(inc, 5, act, buf, ch, 0, 0, TO_CHAR);
    RS.Queue.AddToQueue(inc, 6, sprintf,buf, (char*)"%s tells you '%s%s%s'",  mob->short_descr,  get_char_color(ch, "tells"),  arg,  END_COLOR(ch));
}

void mprog_say(int inc, char *arg, CHAR_DATA *mob, CHAR_DATA *ch)
{
	char buf[MSL];
	RS.Queue.AddToQueue(inc, 5, act, buf, ch, 0, 0, TO_CHAR);
	RS.Queue.AddToQueue(inc, 6, sprintf,buf, (char*)"%s says '%s%s%s'",  mob->short_descr,  get_char_color(ch, "speech"),  arg,  END_COLOR(ch));
}

void mprog_emote(int inc, char *arg, CHAR_DATA *mob, CHAR_DATA *ch)
{
	char buf[MSL];
	RS.Queue.AddToQueue(inc, 5, act, buf, ch, 0, 0, TO_CHAR);
	RS.Queue.AddToQueue(inc, 4, sprintf, buf, (char*)"%s %s", mob->short_descr,  arg);
}

int mprog_drop(int inc, char *arg, OBJ_DATA *obj, CHAR_DATA *mob, CHAR_DATA *ch)
{
	char buf[MSL];
	if(arg)
	{
	RS.Queue.AddToQueue(inc, 5, act, buf, ch, 0, 0, TO_CHAR);
    RS.Queue.AddToQueue(inc, 6, sprintf,buf, (char*)"%s says '%s%s%s'",  mob->short_descr,  get_char_color(ch, "speech"),  arg,  END_COLOR(ch));
	}
	RS.Queue.AddToQueue(inc, 2, obj_to_room, obj, mob->in_room);
	RS.Queue.AddToQueue(inc, 1, obj_from_char, obj);
	RS.Queue.AddToQueue(inc, 5, act, "$n drops $p.", mob, obj, 0, TO_ROOM);
	return 0;
}

int mprog_give(int inc, char *arg, OBJ_DATA *obj, CHAR_DATA *mob, CHAR_DATA *ch)
{
	char buf[MSL];
	if(arg)
	{
		RS.Queue.AddToQueue(inc, 5, act, buf, ch, 0, 0, TO_CHAR);
		RS.Queue.AddToQueue(inc, 6, sprintf,buf, (char*)"%s says '%s%s%s'",  mob->short_descr,  get_char_color(ch, "speech"),  arg,  END_COLOR(ch));
	}
	RS.Queue.AddToQueue(inc, 2, obj_to_char, obj, ch);
	RS.Queue.AddToQueue(inc, 1, obj_from_char, obj);
	RS.Queue.AddToQueue(inc, 5, act, "$n gives you $p.", mob, obj, ch, TO_VICT);
	return 0;
}

bool mprog_unset(MOB_INDEX_DATA *mobindex,const char *progtype, const char *name)
{
	int i;
	for(i = 0; mprog_table[i].name != NULL; i++)
		if(!str_cmp(mprog_table[i].name, name))
			break;

	if(!mprog_table[i].name)
		return FALSE;

	//IF YOU ADD A NEW TYPE ALSO PUT IT IN DB.C DO_ADUMP!
   	if (!str_cmp(progtype, "bribe_prog"))
     	{
		mobindex->mprogs->bribe_prog = NULL;
		free_pstring(mobindex->mprogs->bribe_name);
	    REMOVE_BIT(mobindex->progtypes, MPROG_BRIBE);
       	return TRUE;
     	}

   	if (!str_cmp(progtype, "entry_prog"))
     	{
		mobindex->mprogs->entry_prog = NULL;
		free_pstring(mobindex->mprogs->entry_name);
	    REMOVE_BIT(mobindex->progtypes, MPROG_ENTRY);
		return TRUE;
	}
   	if (!str_cmp(progtype, "greet_prog"))
     	{
		mobindex->mprogs->greet_prog = NULL;
		free_pstring(mobindex->mprogs->greet_name);
		mobindex->mprogs->greet_name = palloc_string(name);
       	REMOVE_BIT(mobindex->progtypes, MPROG_GREET);
       	return TRUE;
     	}
   	if (!str_cmp(progtype, "fight_prog"))
     	{
		mobindex->mprogs->fight_prog = NULL;
		free_pstring(mobindex->mprogs->fight_name);
		REMOVE_BIT(mobindex->progtypes, MPROG_FIGHT);
		return TRUE;
     	}
   	if (!str_cmp(progtype, "death_prog")) /* returning TRUE prevents death */
     	{
		mobindex->mprogs->death_prog = NULL;
		free_pstring(mobindex->mprogs->death_name);
       	REMOVE_BIT(mobindex->progtypes, MPROG_DEATH);
       	return TRUE;
     	}
	if (!str_cmp(progtype, "beat_prog")) {
		mobindex->mprogs->beat_prog = NULL;
		free_pstring(mobindex->mprogs->beat_name);
		REMOVE_BIT(mobindex->progtypes, MPROG_BEAT);
		return TRUE;
	}
   	if (!str_cmp(progtype, "pulse_prog"))
     	{
		mobindex->mprogs->pulse_prog = NULL;
		free_pstring(mobindex->mprogs->pulse_name);
       	REMOVE_BIT(mobindex->progtypes, MPROG_PULSE);
       	return TRUE;
     	}
   	if (!str_cmp(progtype, "speech_prog"))
     	{
		mobindex->mprogs->speech_prog = NULL;
		free_pstring(mobindex->mprogs->speech_name);
		REMOVE_BIT(mobindex->progtypes, MPROG_SPEECH);
		return TRUE;
     	}
   	if (!str_cmp(progtype, "give_prog"))
     	{
		mobindex->mprogs->give_prog = NULL;
       		free_pstring(mobindex->mprogs->give_name);
	       	REMOVE_BIT(mobindex->progtypes, MPROG_GIVE);
       		return TRUE;
     	}
	if (!str_cmp(progtype, "attack_prog"))
	{
		mobindex->mprogs->attack_prog = NULL;
		free_pstring(mobindex->mprogs->attack_name);
		REMOVE_BIT(mobindex->progtypes, MPROG_ATTACK);
		return TRUE;
	}
   	if (!str_cmp(progtype, "aggress_prog")) /* Returning TRUE stops attack */
    {
		mobindex->mprogs->aggress_prog = NULL;
		free_pstring(mobindex->mprogs->aggress_name);
		REMOVE_BIT(mobindex->progtypes, MPROG_AGGRESS);
		return TRUE;
	}
	if (!str_cmp(progtype, "move_prog"))
	{
		mobindex->mprogs->move_prog = NULL;
		free_pstring(mobindex->mprogs->move_name);
		REMOVE_BIT(mobindex->progtypes, MPROG_MOVE);
		return TRUE;
	}

	return FALSE;
}

void mprog_set(MOB_INDEX_DATA *mobindex,const char *progtype, const char *name)
{
	int i;
	for(i = 0; mprog_table[i].name != NULL; i++)
		if(!str_cmp(mprog_table[i].name, name))
			break;

	if(!mprog_table[i].name)
		return bug("Load_improgs: 'M': Function not found for vnum %d",mobindex->vnum);

	//IF YOU ADD A NEW TYPE ALSO PUT IT IN DB.C DO_ADUMP!
   	if (!str_cmp(progtype, "bribe_prog"))
     	{
		mobindex->mprogs->bribe_prog = (MPROG_FUN_BRIBE *)mprog_table[i].function;
		free_pstring(mobindex->mprogs->bribe_name);
		mobindex->mprogs->bribe_name = palloc_string(name);
	       	SET_BIT(mobindex->progtypes, MPROG_BRIBE);
       		return;
     	}

   	if (!str_cmp(progtype, "entry_prog"))
     	{
		mobindex->mprogs->entry_prog = (MPROG_FUN_ENTRY *)mprog_table[i].function;
		free_pstring(mobindex->mprogs->entry_name);
		mobindex->mprogs->entry_name = palloc_string(name);
	       	SET_BIT(mobindex->progtypes, MPROG_ENTRY);
		return;
	}
   	if (!str_cmp(progtype, "greet_prog"))
     	{
		mobindex->mprogs->greet_prog = (MPROG_FUN_GREET *)mprog_table[i].function;
		free_pstring(mobindex->mprogs->greet_name);
		mobindex->mprogs->greet_name = palloc_string(name);
       		SET_BIT(mobindex->progtypes, MPROG_GREET);
       		return;
     	}
   	if (!str_cmp(progtype, "fight_prog"))
     	{
		mobindex->mprogs->fight_prog = (MPROG_FUN_FIGHT *)mprog_table[i].function;
		free_pstring(mobindex->mprogs->fight_name);
		mobindex->mprogs->fight_name = palloc_string(name);
		SET_BIT(mobindex->progtypes, MPROG_FIGHT);
		return;
     	}
   	if (!str_cmp(progtype, "death_prog")) /* returning TRUE prevents death */
     	{
		mobindex->mprogs->death_prog = (MPROG_FUN_DEATH *)mprog_table[i].function;
		free_pstring(mobindex->mprogs->death_name);
		mobindex->mprogs->death_name = palloc_string(name);
       		SET_BIT(mobindex->progtypes, MPROG_DEATH);
       		return;
     	}
	if (!str_cmp(progtype, "beat_prog")) {
		mobindex->mprogs->beat_prog = (MPROG_FUN_BEAT *)mprog_table[i].function;
		free_pstring(mobindex->mprogs->beat_name);
		mobindex->mprogs->beat_name = palloc_string(name);
		SET_BIT(mobindex->progtypes, MPROG_BEAT);
		return;
	}
   	if (!str_cmp(progtype, "pulse_prog"))
     	{
		mobindex->mprogs->pulse_prog = (MPROG_FUN_PULSE *)mprog_table[i].function;
		free_pstring(mobindex->mprogs->pulse_name);
		mobindex->mprogs->pulse_name = palloc_string(name);
       		SET_BIT(mobindex->progtypes, MPROG_PULSE);
       		return;
     	}
   	if (!str_cmp(progtype, "speech_prog"))
     	{
		mobindex->mprogs->speech_prog = (MPROG_FUN_SPEECH *)mprog_table[i].function;
		free_pstring(mobindex->mprogs->speech_name);
		mobindex->mprogs->speech_name = palloc_string(name);
		SET_BIT(mobindex->progtypes, MPROG_SPEECH);
		return;
     	}
   	if (!str_cmp(progtype, "give_prog"))
     	{
		mobindex->mprogs->give_prog = (MPROG_FUN_GIVE *)mprog_table[i].function;
       		free_pstring(mobindex->mprogs->give_name);
		mobindex->mprogs->give_name = palloc_string(name);	
	       	SET_BIT(mobindex->progtypes, MPROG_GIVE);
       		return;
     	}
	if (!str_cmp(progtype, "attack_prog"))
	{
		mobindex->mprogs->attack_prog = (MPROG_FUN_ATTACK *)mprog_table[i].function;
		free_pstring(mobindex->mprogs->attack_name);
		mobindex->mprogs->attack_name = palloc_string(name);
		SET_BIT(mobindex->progtypes, MPROG_ATTACK);
		return;
	}
   	if (!str_cmp(progtype, "aggress_prog")) /* Returning TRUE stops attack */
    {
		mobindex->mprogs->aggress_prog = (MPROG_FUN_AGGRESS *)mprog_table[i].function;
		free_pstring(mobindex->mprogs->aggress_name);
		mobindex->mprogs->aggress_name = palloc_string(name);
		SET_BIT(mobindex->progtypes, MPROG_AGGRESS);
		return;
	}
	if (!str_cmp(progtype, "move_prog"))
	{
		mobindex->mprogs->move_prog = (MPROG_FUN_MOVE *)mprog_table[i].function;
		free_pstring(mobindex->mprogs->move_name);
		mobindex->mprogs->move_name = palloc_string(name);
		SET_BIT(mobindex->progtypes, MPROG_MOVE);
		return;
	}
   bug( "Load_improgs: 'M': invalid program type for vnum %d",mobindex->vnum);
   exit(1);
}

/******************  Functions *********************/


void bribe_prog_cityguard(CHAR_DATA *mob, CHAR_DATA *ch, int amount)
{
  if (amount < 100)
    {
      do_say(mob, "You cheapskate!!!");
      do_murder(mob, ch->name);
      return;
    }
  else if (amount >= 5000)
    {
      interpret(mob, "wink");
      do_sleep(mob, "");
      return;
    }
  else
    {
      do_say(mob, "Trying to bribe me, eh? It'll cost ya more than that.");
      return;
    }
}

void greet_prog_arangird(CHAR_DATA *mob, CHAR_DATA *ch)
{
	OBJ_DATA *obj;
	bool found=FALSE;

	if(IS_NPC(ch))
		found=TRUE;

	if (!IS_NPC(ch) && check_stealth(ch,mob))
		found=TRUE;

	if ((obj = get_eq_char(ch,WEAR_ABOUT)) != NULL
		&& (obj->pIndexData->vnum == 12219
		|| obj->pIndexData->vnum == 21800))
		found = TRUE;

	if(!found && str_cmp(race_table[ch->race].name, "duergar")) {
		do_say(mob,"Intruder! You shall pay for this trespass!");
		do_murder(mob, ch->name);
	}

}

void pulse_prog_arangird_patrol(CHAR_DATA *mob)
{
	CHAR_DATA *vch;
	OBJ_DATA *robes;
	int dir_next = -1;
	
	if (mob->fighting)
		return;

	if (!mob->in_room)
		return;

	if (number_percent() < 70)
		return;

	switch(mob->in_room->vnum) {
		case(21847):
			do_south(mob,"");
			dir_next = DIR_WEST;
			break;
		case(21848):
			do_west(mob,"");
			dir_next = DIR_WEST;
			break;
		case(21849):
			do_west(mob,"");
			dir_next = DIR_WEST;
			break;
		case(21850):
			do_west(mob,"");
			dir_next = DIR_NORTH;
			break;
		case(21851):
			do_north(mob,"");
			dir_next = DIR_WEST;
			break;
		case(21852):
			do_west(mob,"");
			dir_next = DIR_NORTH;
			break;
		case(21853):
			do_north(mob,"");
			dir_next = DIR_NORTH;
			break;
		case(21854):
			do_north(mob,"");
			dir_next = DIR_EAST;
			break;
		case(21855):
			do_east(mob,"");
			dir_next = DIR_SOUTH;
			break;
		case(21856):
			do_south(mob,"");
			dir_next = DIR_EAST;
			break;
		case(21857):
			do_east(mob,"");
			dir_next = DIR_EAST;
			break;
		case(21858):
			do_east(mob,"");
			dir_next = DIR_EAST;
			break;
		case(21859):
			do_east(mob,"");
			dir_next = DIR_SOUTH;
			break;
		case(21860):
			do_south(mob,"");
			dir_next = DIR_SOUTH;
			break;
		default:
			return;
	}

	for (vch = mob->in_room->people; vch; vch = vch->next_in_room) {
		if(IS_NPC(vch))
			continue;
		if (IS_IMMORTAL(vch))
			continue;
		if ((robes = get_eq_char(vch,WEAR_ABOUT)) != NULL
			&& robes->pIndexData->vnum == 21800)
			continue;
		do_yell(mob,"Intruder!  Sound the alarm!");
		do_murder(mob,vch->name);
		return;
	}

	if (mob->in_room->exit[dir_next]->u1.to_room->people)
		act("You hear the heavy footsteps of an approaching patrol echo through the caves.",mob->in_room->exit[dir_next]->u1.to_room->people,0,0,TO_ALL);
	
	return;
}

void greet_prog_profteacher(CHAR_DATA *mob, CHAR_DATA *ch)
{
	if(number_percent() > 50)
		RS.Queue.AddToQueue(2, 5, act, "$N beckons you to approach $M.", ch, 0, mob, TO_CHAR);
	else
		RS.Queue.AddToQueue(2, 5, act, "$N meets your eyes thoughtfully, nodding slightly.", ch, 0, mob, TO_CHAR);
}
void greet_prog_ruins_spirit(CHAR_DATA *mob, CHAR_DATA *ch)
{
	char buf[MSL];

	if (mob->fighting || IS_IMMORTAL(ch))
		return;

	if (check_stealth(ch,mob))
		return;
	
	if (IS_EVIL(ch) && can_see(mob,ch)) {
		sprintf(buf,"How dare you enter this place of holiness, %s?  You shall suffer the consequences!",ch->name);
		do_say(mob,buf);
		do_murder(mob,ch->name);
	}

	if (IS_NEUTRAL(ch) && can_see(mob,ch)) {
		sprintf(buf,"You may go about your business, %s, but touch nothing!",
			ch->name);
		do_say(mob,buf);
	}

	return;
}

void greet_prog_ruins_mouth(CHAR_DATA *mob, CHAR_DATA *ch)
{
	do_say(mob,"Welcome to the Ministry of Magic.  Shall I direct you to the Room of Testing, the Office of the Master of the Art, or the Room of Inventory?");

	return;
}

void speech_prog_testmob(CHAR_DATA *mob, CHAR_DATA *ch, char *speech)
{

	if (strstr(speech,"test"))
		execute_speech (ch, mob, mob->pIndexData->speech);

	return;
}

void speech_prog_ruins_mouth(CHAR_DATA *mob, CHAR_DATA *ch, char *speech)
{
	ROOM_INDEX_DATA *to_room = NULL;
	
	if (strstr(speech,"testing") || strstr(speech,"Testing"))
		to_room = get_room_index(20159);
	else if (strstr(speech,"office") || strstr(speech,"Office"))
		to_room = get_room_index(20158);
	else if (strstr(speech,"inventory") || strstr(speech,"Inventory"))
		to_room = get_room_index(20161);

	if (!to_room)
		return;

	send_to_char("A magic mouth impossibly beckons for you to follow and strange magicks force you through the shimmering portal.\n\r",ch);
	act("The magic mouth impossibly beckons for $n to follow and $e vanishes into the portal.",ch,0,0,TO_ROOM);

	char_from_room(ch);
	char_to_room(ch, to_room);
	do_look(ch,"auto");

	return;
}

void greet_prog_sailor(CHAR_DATA *mob, CHAR_DATA *ch)
{
  if (!IS_NPC(ch))
    {
      do_say(mob,
	   "Hey, want to dice?  I could use some of that gold in your pouch.");
    }
}
	
void speech_prog_sailor(CHAR_DATA *mob, CHAR_DATA *ch, char *speech)
{
  int sroll;
  int proll;
  int amount;
  char buf[100];
  char arg[100];

  if (!str_cmp(speech, "yes"))
    do_say(mob, "How much?");

  else if (is_number(speech))
    {
      amount = atoi(speech);
      if (amount < 0)
	{
  	  sprintf(buf, "You win %d gold!!\n\r", 2*amount);
	  send_to_char(buf, ch);
	  act("$n does a very foolish thing....",ch, NULL, NULL, TO_ROOM);
	  ch->gold = UMAX(0, (ch->gold + (2*amount)));
	  return;
	}
      if (ch->gold < amount)
	{
	  do_say(mob, "You don't have that much gold!");
	  do_say(mob, "I won't dice with the likes of you!");
	}
      else
	{
	  act("$n rolls $s dice.", mob, NULL, NULL, TO_ROOM);
	  act("You roll your dice.", mob, NULL, NULL, TO_CHAR);
	  sroll = dice(5, 6);
	  act("$n rolls $s dice.", ch, NULL, NULL, TO_ROOM);
	  act("You roll your dice.", ch, NULL, NULL, TO_CHAR);
	  proll = dice(5, 6);
	
	  if (sroll > proll)
	    {
	      sprintf(buf, "$N lost %d gold!", amount);
	      sprintf(arg, "You lost %d gold!!", amount);
	      act("$n has won some gold!!", mob, NULL, ch, TO_NOTVICT);
	      act(buf, mob, NULL, ch, TO_CHAR);
	      act(arg, mob, NULL, ch, TO_VICT);
	      ch->gold -= amount;
	      do_say(mob, "Would you like to try again?");
	      interpret(mob, "grin");
	    }
	
	  else if (sroll == proll)
	    {
	      act("You tied with $N!  You don't lose any money.",
		  ch, NULL, mob, TO_CHAR);
	      act("You tied with $n!  You don't lose any money.",
		  ch, NULL, mob, TO_VICT);
	      act("$n tied with $N!  Neither wins the game.",
		  ch, NULL, mob, TO_NOTVICT);
	      do_say(mob, "Would you like to try that again?");
	    }
	  else
	    {
	      sprintf(buf, "$n won %d gold!", amount);
	      sprintf(arg, "You won %d gold!!", amount);
	      act("$n has won some gold!!", ch, NULL, mob, TO_NOTVICT);
	      act(buf, ch, NULL, mob, TO_VICT);
	      act(arg, ch, NULL, mob, TO_CHAR);
	      ch->gold += amount;
	      do_say(mob, "Next time you won't be so lucky.");
	    }
	}
    }
}

void greet_prog_knight(CHAR_DATA *mob, CHAR_DATA *ch)
{
  if ( IS_NPC(ch) )
    return;

  if (ch->ghost > 0)
    {
      do_say(mob, "Ghosts are not allowed in this place.");
      do_slay(mob, ch->name);
      return;
    }

  do_cb(mob, "Intruder! Intruder!");
  do_murder(mob, ch->name);
}

void fight_prog_priest(CHAR_DATA *mob, CHAR_DATA *ch)
{
  switch(number_bits(6)) {
  case 0:
    do_say(mob, "Xiang Long Shi Ba Zhang!");
    break;
  case 1:
    do_say(mob, "Qian Kun Da Nuo Yi!");
    break;
  case 2:
    do_say(mob, "Da Li Jing Kang Zi !");
    break;
  case 3:
    do_say(mob, "Ni Jing Gan Gen Wo Tou");
    break;
  case 4:
    do_say(mob, "Wo Gei Ni Yan Se Kan");
    break;
  case 5:
    do_say(mob, "Wan Xiang Shen Gong");
    break;
  }
}

void pulse_prog_formian_queen(CHAR_DATA *mob)
{
	int rand, vnum = 0, incubation = 0;
	AFFECT_DATA af;
	OBJ_AFFECT_DATA oaf;
	OBJ_DATA *egg;

	if (!mob->in_room)
		return;
	
	if (mob->fighting)
		return;

	if (is_affected(mob,gsn_timer))
		return;

	rand = number_range(1,10);

	switch(rand) {
		case(1):
		case(2):
		case(3):
		case(4):
			vnum = 3212;	//worker
			incubation = 35;
			break;
		case(5):
		case(6):
		case(7):
			vnum = 3213;	//warrior
			incubation = 45;
			break;
		case(8):
		case(9):
			vnum = 3214;	//taskmaster
			incubation = 55;
			break;
		case(10):
			vnum = 3215;	//queen guard
			incubation = 65;
			break;
		default:
			vnum = 3212;
	}

	egg = create_object(get_obj_index(vnum),0);
	obj_to_room(egg,mob->in_room);

	init_affect(&af);
	af.where		=	TO_AFFECTS;
	af.aftype		=	AFT_TIMER;
	af.type			=	gsn_timer;
	af.duration		=	20;
	affect_to_char(mob,&af);

	init_affect_obj(&oaf);
	oaf.where		=	TO_OBJ_AFFECTS;
	oaf.type		=	gsn_timer;
	oaf.aftype		=	AFT_TIMER;
	oaf.duration	=	incubation;
	oaf.end_fun		=	formian_egg_hatch;
	affect_to_obj(egg,&oaf);
	
	return;
}

/* Not really an mprog, but hey, it MAKES a mob, so.... */

void formian_egg_hatch(OBJ_DATA *obj, OBJ_AFFECT_DATA *af)
{
	int egg_vnum = 0;

	egg_vnum = obj->pIndexData->vnum;

	if (egg_vnum < 3212 || egg_vnum > 3215)
		return;

	return;
}
	
void greet_prog_outer_guardian(CHAR_DATA *mob, CHAR_DATA *ch)
{
	if(IS_NPC(ch) || ch->invis_level>LEVEL_HERO-1)
		return;
	if(mob->cabal==ch->cabal)
		do_say(mob,cabal_messages[ch->cabal].entrygreeting);
}

void pulse_prog_demon(CHAR_DATA *mob)
{
    CHAR_DATA *elemental=mob;
    CHAR_DATA *victim=elemental->hunting;
    char buf[250];
    if(!victim || !elemental->in_room || !victim->in_room || victim->ghost>0)
    {
	if(mob->master && victim->ghost>0) {
		sprintf(buf,"%s I have taken the soul of %s. Your debt will one day be collected.",mob->master->name,victim->name);
		do_tell(mob,buf); }
        act("$n returns to the Hells.",elemental,0,0,TO_ALL);
       	extract_char(mob,TRUE);
        return;
    }
    if(number_percent()>50)
        return;
    if(elemental->fighting)
        return;
    if(mob->position!=POS_STANDING)
        mob->position=POS_STANDING;
    if(victim->in_room->vnum!=elemental->in_room->vnum)
    {
        act("$n steps through a portal, seeking $s prey.",elemental,0,0,TO_ALL);
        char_from_room(elemental);
        char_to_room(elemental,victim->in_room);
        act("$n arrives through a portal, seeking $N's blood!",elemental,0,victim,TO_NOTVICT);
	act("$n arrives through a portal, seeking your blood!",elemental,0,victim,TO_VICT);
    }
    act("$n gestures at $N, uttering curses.",elemental,0,victim,TO_NOTVICT);
    act("$n gestures at you, uttering curses.",elemental,0,victim,TO_VICT);
    multi_hit(elemental,victim,TYPE_UNDEFINED);
}

bool aggress_prog_arangird_demon(CHAR_DATA *mob, CHAR_DATA *attacker)
{
	CHAR_DATA *regthor;
	
	if((regthor = get_char_room(mob, "regthor")) != NULL)
	{
		act("An invisible barrier flares around $n, protecting $m from harm.", mob, 0, attacker, TO_VICT);
		act("An invisible barrier flares around $n as $N approaches.", mob, 0, attacker, TO_NOTVICT);
		if(attacker->fighting == mob)
			stop_fighting(attacker,mob);
		return TRUE;
	}

	return FALSE;
}

bool aggress_prog_arangird_regthor(CHAR_DATA *mob, CHAR_DATA *attacker)
{
	CHAR_DATA *demon;

	if((demon = get_char_room(mob, "ertruza")) == NULL)
        return FALSE;

	act("A look of horror crosses $n's face as $s concentration falters.",mob,0,demon,TO_ALL);
	act("The barrier surrounding $N flickers and fades, and $n shrieks!",mob,0,demon,TO_ALL);
	act("Roaring with anger, $n grabs his former captor and devours $M whole!",demon,0,mob,TO_ALL);
	
	extract_char(mob, TRUE);

	act("$n then turns you and strikes!", demon, 0, attacker, TO_CHAR);
	act("$n then turns to $N and strikes!", demon, 0, attacker, TO_ROOM);
	do_murder(demon, attacker->name);

	return TRUE;
}

void fight_prog_ilopheth_druid(CHAR_DATA *mob, CHAR_DATA *victim)
{
	int rand;
	ROOM_INDEX_DATA *outside;

	rand = number_percent();
	
	if (rand > 10)
		return;

	act("With a wave of $s hand, the druid summons a gale that batters the area!",mob,0,0,TO_ROOM);
	
	if (rand == 1) {
		act("The powerful winds send you tumbling headlong through the bushes!",victim,0,0,TO_CHAR);
		act("The powerful winds send $n tumbling headling through the bushes!",victim,0,0,TO_ROOM);
		LAG_CHAR(victim,2*PULSE_VIOLENCE);
		outside = get_room_index(9096);
		char_from_room(victim);
		char_to_room(victim,outside);
		do_look(victim,"auto");
	} else {
		act("The powerful winds send you sprawling in a heap!",victim,0,0,TO_CHAR);
		act("The powerful winds send $n sprawling in a heap!",victim,0,0,TO_ROOM);
		victim->position = POS_RESTING;
		LAG_CHAR(victim,2*PULSE_VIOLENCE);
	}

	return;
}

void pulse_prog_ilopheth_piranha(CHAR_DATA *mob)
{
	ROOM_INDEX_DATA *room, *surface, *other;
	CHAR_DATA *victim, *adjmob, *adjmob_next;
	int rindex;

	if ((room = mob->in_room) == NULL)
		return;

	if (mob->fighting)
		return;

	if (mob->position <= POS_RESTING)
		return;

	if (room->vnum < 9141 || room->vnum > 9146)
		return;

	if (!room->exit[DIR_UP] || ((surface = room->exit[DIR_UP]->u1.to_room) == NULL))
		return;

	if (is_affected_room(surface,gsn_glaciate))
		return; 

	if ((victim = get_rand_from_room(surface)) == NULL) {
		if (mob->in_room != mob->home_room)
			walk_to_room(mob,mob->home_room);
		return;
	}

	if (number_percent() > 25)
		return;

	char_from_room(mob);
	char_to_room(mob,surface);
	act("$n leaps from the water, razor teeth flashing!",mob,0,0,TO_ROOM);
	damage_new(mob,victim,(20 + dice(5,6)),TYPE_UNDEFINED,DAM_PIERCE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"chomp");
	stop_fighting(mob,TRUE);
	char_from_room(mob);
	char_to_room(mob,room);

	for (rindex = 9141; rindex <= 9146; rindex++) {
		other = get_room_index(rindex);
		if (other == room)
			continue;
		for (adjmob = other->people; adjmob; adjmob = adjmob_next) {
			adjmob_next = adjmob->next_in_room;
			if (!IS_NPC(adjmob))
				continue;
			if (adjmob->pIndexData->vnum != 9011)
				continue;
			if (adjmob->fighting)
				continue;
			if (number_percent() > 50)
				continue;
			walk_to_room(adjmob,room);
			if (adjmob->in_room == room)
				act("The water here ripples, and there appears to be increased movement below the surface.",victim,0,0,TO_ALL);
		}
	}

	return;
}

void pulse_prog_ilopheth_weatherbeaten(CHAR_DATA *mob)
{
	ROOM_INDEX_DATA *forest = get_room_index(9121);
	ROOM_INDEX_DATA *limbo = get_room_index(2);
	
	if (!mob->in_room || mob->pIndexData->vnum != 9010)
		return;

	if (mob->fighting || mob->position < POS_RESTING)
		return;

	if (sun == SUN_SET && mob->in_room->vnum == 9121) {
		act("The old man glances up at the darkening sky, and trudges off into the forest to the south.",mob,0,0,TO_ROOM);
		char_from_room(mob);
		char_to_room(mob, limbo);
	} else if (sun == SUN_RISE && mob->in_room->vnum == 2) {
		char_from_room(mob);
		char_to_room(mob, forest);
		act("An aged man trudges in from the south, and sits down heavily on a large rock.",mob,0,0,TO_ROOM);
	}
	
	return;
}

void pulse_prog_alstea_ehrlouge(CHAR_DATA *mob)
{
	ROOM_INDEX_DATA *study = get_room_index(105);
	int rnd;

	if(mob->fighting || mob->position > POS_RESTING)
		return;
	if(mob->in_room->vnum != 105)
		return;

	rnd = number_percent();
	
	if(rnd < 4)
	{
		act("Ehrlouge scribbles something down violently, his teeth gritting and chattering.",mob,0,0,TO_ROOM);
		return;
	}
	if(rnd > 97)
	{
		act("The skeletal figure of Ehrlouge begins to shudder and tremble as he dryly and raspily whimpers.",mob,0,0,TO_ROOM);
		return;
	}
	return;
}

void attack_prog_outer_guardian(CHAR_DATA *mob, CHAR_DATA *attacker)
{
	char buf[MSL];
	
	if(IS_NPC(attacker))
		return;
	if(attacker->ghost)
	{
		do_say(mob, "Ghosts are not allowed in this place!");
		damage_new(mob, attacker,attacker->hit - 1, TYPE_HIT, DAM_HOLY, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "divine wrath");
		attacker->hit = 25;
		attacker->ghost = 0;
	}
	sprintf(buf,"Intruder! Intruder! %s is raiding the cabal!",attacker->true_name);
	do_cb(mob,buf);

	return;
}

void greet_prog_inner_guardian(CHAR_DATA *mob, CHAR_DATA *ch)
{
	char buf[MSL];

	if(IS_NPC(ch) || IS_IMMORTAL(ch))
		return;

	if(ch->cabal == mob->cabal)
		return;

	sprintf(buf,"Intruder! Intruder! %s is raiding the cabal!",ch->true_name);
	do_cb(mob,buf);

	if(ch->ghost>0)
	{
		do_say(mob,"Ghosts aren't allowed in here!");
		raw_kill(mob,ch);
		return;
	}

	multi_hit(mob,ch,TYPE_UNDEFINED);	
}

bool death_prog_outer_guardian(CHAR_DATA *mob, CHAR_DATA *killer)
{
	CHAR_DATA *cabalguardian, *othercabalguardian, *vch;
	char buf[MSL];
	OBJ_DATA *obj, *obj_next;
	bool present;

	do_cb(mob,"Alas, my life has come to an end.");

	if ((cabalguardian = get_cabal_guardian(mob->cabal)) != NULL)
	{
		for (obj = cabalguardian->carrying; obj != NULL; obj = obj_next)
		{
			present = FALSE;
			obj_next = obj->next_content;

			if (!isCabalItem(obj))
				continue;

			if (obj->pIndexData->cabal != cabalguardian->cabal)
			{
				for (vch = killer->in_room->people; vch != NULL; vch = vch->next_in_room)
				{
					if (is_same_group(vch,killer)) {
						if (obj->pIndexData->cabal == vch->cabal) {
							present = TRUE;
						}
					}
				}
				if (present && (othercabalguardian = get_cabal_guardian(obj->pIndexData->cabal)) != NULL)
				{
					sprintf(buf,"As %s falls, %s returns to %s!",
					mob->short_descr,obj->short_descr,othercabalguardian->short_descr);
					obj_from_char(obj);
					obj_to_char(obj,othercabalguardian);
					act(buf,mob,0,0,TO_ALL);
					cabal_shudder(obj->pIndexData->cabal, FALSE);
				}
			}
		}
		save_cabal_items();
	}

	return FALSE;
}

bool death_prog_inner_guardian(CHAR_DATA *mob, CHAR_DATA *killer)
{
	OBJ_DATA *obj, *obj_next;
	char buf[MSL];
	bool found = FALSE;
	CHAR_DATA *cabalguardian;

	do_cb(mob,"Alas, my life has come to an end.");

   	for (obj = mob->carrying; obj != NULL; obj = obj_next)
   	{
		found = FALSE;
		obj_next = obj->next_content;

		if (!isCabalItem(obj))
			continue;

        if(obj->pIndexData->cabal!=mob->cabal)
        {
        	cabalguardian = get_cabal_guardian(obj->pIndexData->cabal);
            if(cabalguardian)
            {
            	sprintf(buf,"As %s falls, %s returns to %s!",
				mob->short_descr,obj->short_descr,cabalguardian->short_descr);
                obj_from_char(obj);
                obj_to_char(obj,cabalguardian);
                found=TRUE;
                act(buf,mob,0,0,TO_ALL);
                cabal_shudder(obj->pIndexData->cabal, FALSE);
            }
		}

		save_cabal_items();
		if(obj->pIndexData->cabal==mob->cabal || !found)
		{
			act("As $N falls, $p appears in $n's hands!",killer,obj,mob,TO_ROOM);
			act("As $N falls, $p appears in your hands!",killer,obj,mob,TO_CHAR);
			obj->timer=21;
			obj_from_char(obj);
			obj_to_char(obj,killer);
        }
	}
	return FALSE;
}

void pulse_prog_tahlu_mist_ward(CHAR_DATA *mob)
{
	DESCRIPTOR_DATA *d;
	CHAR_DATA *mist, *ch;
	if(!mob->in_room->area->nplayer || number_percent()<=90)
		return;
    	for ( d = descriptor_list; d != NULL; d = d->next )
    	{
		if ( d->connected == CON_PLAYING
		&&   d->character->in_room != NULL
		&&   d->character->in_room->area == mob->in_room->area
		&&   IS_EVIL(d->character))
		{
			ch = d->character;
			mist = create_mobile(get_mob_index(1616));
			char_to_room(mist, ch->in_room);
			act("A strange cloud of mist rises from the floor, stretching tendrils towards you!",ch,0,0,TO_CHAR);
			act("A strange cloud of mist rises from the floor, stretching tendrils towards $n.",ch,0,0,TO_ROOM);
			multi_hit(mist,ch,TYPE_UNDEFINED);
			return;
		}
	}
}

bool move_prog_horde_shrine_ward(CHAR_DATA *ch, CHAR_DATA *mob, ROOM_INDEX_DATA *from, int direction)
{
	if(direction != DIR_SOUTH)
		return TRUE;

	if(IS_NPC(ch))
		return FALSE;

	if (!is_affected(ch,gsn_horde_communion)) {
		send_to_char("Alas, you cannot go that way.\n\r",ch);
		return FALSE;
	}
	send_to_char("With a muted hush, the bushes part easily, allowing you passage to the south.\n\r",ch);
	return TRUE;
}

bool aggress_prog_anchor(CHAR_DATA *mob, CHAR_DATA *attacker)
{
	act("$n dissipates harmlessly as you turn your attention towards it.",mob,0,0,TO_ROOM);
	stop_fighting(mob, TRUE);
	extract_char(mob,TRUE);
	stop_fighting(attacker,TRUE);
	return TRUE;
}

bool death_prog_glass(CHAR_DATA *mob, CHAR_DATA *killer)
{

	CHAR_DATA *ch, *vch, *vch_next;
	ch = mob->leader;

	if(ch == NULL)
		return FALSE;

	for(vch=mob->in_room->people;vch!=NULL;vch=vch_next)	
	{
		vch_next = vch->next_in_room;	
		
		if(is_safe(ch,vch) || ch == vch)
			continue;
		
		act("Some of the exploding glass from $n strikes you!",mob,0,vch,TO_VICT);
		damage_new(ch,vch,dice(mob->level,6),TYPE_UNDEFINED,DAM_PIERCE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"the shattered glass*");

		if((vch != NULL) && (number_percent() > 60))
			damage_new(ch,vch,dice(mob->level,2),TYPE_UNDEFINED,DAM_PIERCE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"the shattered glass*");
		if((vch != NULL) && (number_percent() > 70))
			damage_new(ch,vch,dice(mob->level,2),TYPE_UNDEFINED,DAM_PIERCE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"the shattered glass*");
		if((vch != NULL) && (number_percent() > 80))
			damage_new(ch,vch,dice(mob->level,2),TYPE_UNDEFINED,DAM_PIERCE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"the shattered glass*");
		if((vch != NULL) && (number_percent() > 90))
			damage_new(ch,vch,dice(mob->level,2),TYPE_UNDEFINED,DAM_PIERCE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"the shattered glass*");
	}

	return FALSE;
}

void pulse_prog_being_watched(CHAR_DATA *mob)
{
	CHAR_DATA *vch;
	for(vch = mob->in_room->people; vch; vch = vch->next_in_room)
		if(vch != mob && number_percent()>92)
			send_to_char("You feel as though you are being watched.\n\r",vch);
}
void pulse_prog_shopkeeper(CHAR_DATA *mob)
{
	SHOP_DATA *pShop = mob->pIndexData->pShop;
	CHAR_DATA *vch, *vch_next;
	EXIT_DATA *exit, *extwo;
	ROOM_INDEX_DATA *room = mob->in_room;

	if(!pShop)
		return;
	if(pShop->close_hour == pShop->open_hour || !(exit = mob->in_room->exit[pShop->direction]) || !exit->u1.to_room)
		return;
	if(time_info.hour == pShop->open_hour && mob->exp!=20)
	{
		REMOVE_BIT(exit->exit_info, EX_LOCKED);
		REMOVE_BIT(exit->exit_info, EX_CLOSED);
		char_from_room(mob);
		char_to_room(mob,exit->u1.to_room);
		act("$n unlocks the door to the $T and opens for business.",
			mob,0,direction_table[reverse_d(pShop->direction)].name,TO_ROOM);
		if((exit = exit->u1.to_room->exit[reverse_d(pShop->direction)]))
		{
			REMOVE_BIT(exit->exit_info, EX_LOCKED);
			REMOVE_BIT(exit->exit_info, EX_CLOSED);
		}
		char_from_room(mob);
		char_to_room(mob,room);
		mob->exp = 20;
		return;
	}
	if(time_info.hour < pShop->close_hour && time_info.hour >= pShop->open_hour)
		return;
	if(time_info.hour == pShop->close_hour && mob->exp != 10)
	{
		do_say(mob,"I'm closing for now, you'll have to come back later.");
		char_from_room(mob);
		char_to_room(mob,exit->u1.to_room);
		act("$n locks the door to the $T as $e closes $s business.",
			mob,0,direction_table[reverse_d(pShop->direction)].name,TO_ROOM);

		char_from_room(mob);
		char_to_room(mob,room);
		if (IS_SET(exit->exit_info,EX_ISDOOR)) {
			SET_BIT(exit->exit_info,EX_LOCKED);
			SET_BIT(exit->exit_info,EX_CLOSED);
		}
		if((extwo = exit->u1.to_room->exit[reverse_d(pShop->direction)])) {
			if (IS_SET(extwo->exit_info, EX_ISDOOR)) {
				SET_BIT(extwo->exit_info, EX_LOCKED);
				SET_BIT(extwo->exit_info, EX_CLOSED);
			}
		}
	}
	for(vch = mob->in_room->people; vch; vch = vch_next)
	{
		vch_next = vch->next_in_room;
		if(vch == mob || IS_IMMORTAL(vch))
			continue;
		if(IS_NPC(vch) && IS_SET(vch->act,ACT_SENTINEL))
			continue;
		act("$n hustles you out the door.",mob,0,vch,TO_VICT);
		act("$n hustles $N out the door.",mob,0,vch,TO_NOTVICT);
		char_from_room(vch);
		char_to_room(vch,exit->u1.to_room);
		act("$t tosses $n out of $s shop.",vch,mob->short_descr,0,TO_ROOM);
		if(mob->fighting)
			stop_fighting(mob,mob->fighting);
		do_look(vch,"auto");
	}
	mob->exp = 10;
}

bool move_prog_theatre_guard(CHAR_DATA *ch, CHAR_DATA *mob, ROOM_INDEX_DATA *from, int direction)
{
	if(direction != DIR_SOUTH || IS_IMMORTAL(ch))
		return TRUE;

	if(ch->pause)
	{
		act("$n moves to block $N.",mob,0,ch,TO_ROOM);
		act("$n moves to block you.",mob,0,ch,TO_VICT);
		do_say(mob,"You'd better clean yourself up a little before you go in.");
		return FALSE;
	}
	return TRUE;
}

void greet_prog_necro_skull(CHAR_DATA *mob, CHAR_DATA *ch)
{
	char buf[MSL];
	CHAR_DATA *master = mob->leader;
	if(ch == master)
		return;
	if(!can_see(master,ch))
		return;
	if (IS_NPC(ch))
		return;
	act("$n rises into the air and opens its mouth in a silent shriek!",mob,0,ch,TO_ROOM);
	sprintf(buf,"A skull-sentinel whispers to you '%sMaster, %s has arrived at my location.%s'",get_char_color(ch,"lightred"),palloc_string(ch->name),END_COLOR(ch));
	act(buf,master,0,0,TO_CHAR);
	return;
}

bool death_prog_necro_skull(CHAR_DATA *mob, CHAR_DATA *killer)
{
	act("$n crumbles into dust.",mob,0,0,TO_ROOM);
	extract_char(mob,TRUE);
	return TRUE;
}	

void pulse_prog_necro_skull(CHAR_DATA *mob)
{
        if(mob->leader == NULL) {
                act("$n crumbles into a pile of dust.",mob,0,0,TO_ROOM);
                extract_char(mob,TRUE);
        }
}

void attack_prog_lesser_demon(CHAR_DATA *mob, CHAR_DATA *attacker)
{
	char buf[MSL];
	AFFECT_DATA *af = affect_find(mob->affected,gsn_lesser_demon);

	if (!af)
		return;
	
	if (mob->pIndexData->vnum == MOB_VNUM_BARBAS && attacker != af->owner && af->owner) {
		do_say(mob,"Bah, ya bloody fool, you'll pay for this!");
		sprintf(buf,"%s I'll be back for ye another time, weakling.",
			af->owner->name);
		do_tell(mob,buf);
		act("$n vanishes in a crimson flash!",mob,0,0,TO_ROOM);
		stop_fighting(mob,TRUE);
		char_from_room(mob);
		char_to_room(mob,get_room_index(3));
		RS.Queue.AddToQueue(1, 1, delay_extract, mob);
		af->owner->pcdata->lesserdata[LESSER_BARBAS] = FAVOR_NONE;
		return;
	}

	if (mob->pIndexData->vnum == MOB_VNUM_BARBAS)
		return;

	if (attacker == af->owner) {
		switch (mob->pIndexData->vnum) {
			case (MOB_VNUM_BARBAS):
				af->owner->pcdata->lesserdata[LESSER_BARBAS] = FAVOR_FAILED;
				break;
			case (MOB_VNUM_FURCAS):
				af->owner->pcdata->lesserdata[LESSER_FURCAS] = FAVOR_FAILED;
				break;
			case (MOB_VNUM_MALAPHAR):
				af->owner->pcdata->lesserdata[LESSER_MALAPHAR] = FAVOR_FAILED;
				break;
			case (MOB_VNUM_AAMON):
				af->owner->pcdata->lesserdata[LESSER_AAMON] = FAVOR_FAILED;
				break;
			case (MOB_VNUM_IPOS):
				af->owner->pcdata->lesserdata[LESSER_IPOS] = FAVOR_FAILED;
				break;
			default:
				return;
		}
		do_emote(mob,"shakes his head angrily.");
		do_say(mob,"You could have had powers beyond your dreams, fool.  Beyond your dreams....");
		act("$n vanishes in a crimson flash!",mob,0,0,TO_ROOM);
		stop_fighting(mob,TRUE);
		char_from_room(mob);
		char_to_room(mob,get_room_index(3));
		RS.Queue.AddToQueue(1, 1, delay_extract, mob);
		return;
	}

	if (attacker != af->owner) {
		switch (mob->pIndexData->vnum) {
			case (MOB_VNUM_BARBAS):
				af->owner->pcdata->lesserdata[LESSER_BARBAS] = FAVOR_NONE;
				break;
			case (MOB_VNUM_FURCAS):
				af->owner->pcdata->lesserdata[LESSER_FURCAS] = FAVOR_NONE;
				break;
			case (MOB_VNUM_MALAPHAR):
				af->owner->pcdata->lesserdata[LESSER_MALAPHAR] = FAVOR_NONE;
				break;
			case (MOB_VNUM_AAMON):
				af->owner->pcdata->lesserdata[LESSER_AAMON] = FAVOR_NONE;
				break;
			case (MOB_VNUM_IPOS):
				af->owner->pcdata->lesserdata[LESSER_IPOS] = FAVOR_NONE;
				break;
			default:
				return;
		}
		sprintf(buf,"%s, you fool, you'll pay for this!",attacker->name);
		do_say(mob,buf);
		sprintf(buf,"%s I'll be back for ye another time, weakling.",
			af->owner->name);
		do_tell(mob,buf);
		act("$n vanishes in a crimson flash!",mob,0,0,TO_ROOM);
		stop_fighting(mob,TRUE);
		char_from_room(mob);
		char_to_room(mob,get_room_index(3));
		RS.Queue.AddToQueue(1, 1, delay_extract, mob);
		return;
	}
		
	
	return;
}

void beat_prog_barbas(CHAR_DATA *mob)
{
	CHAR_DATA *ch = mob->last_fought;
	ROOM_INDEX_DATA *old_room = mob->in_room;
	char buf[MSL];

	if (!ch)
		return;

	if (mob->fighting)
		return;

	if (ch->ghost > 0) {
		sprintf(buf,"%s I'll be takin' me leave now, weakling.",ch->name);
		do_tell(mob,buf);
		extract_char(mob,TRUE);
		return;
	}

	if (ch->in_room->area == mob->in_room->area && ch->in_room != mob->in_room) {
		if (number_range(1,4) == 1)
			smart_track(ch,mob);
		return;
	}
	
	if (ch->in_room != mob->in_room) {
		if (number_range(1,3) == 1)
			track_char(ch,mob);
		else
			return;
		if (mob->in_room == old_room && number_range(1,30) == 1) {
			char_from_room(mob);
			char_to_room(mob, ch->in_room);
			act("$n has arrived through a gate.",mob,0,0,TO_ROOM);
		}
		return;
	}

	do_say(mob,"You think you can escape me?  Pathetic whelp!");
	one_hit(mob,ch,TYPE_UNDEFINED);
	
	return;
}

bool death_prog_barbas(CHAR_DATA *mob, CHAR_DATA *killer)
{
	AFFECT_DATA *af;

	af = affect_find(mob->affected,gsn_lesser_demon);
	if (!af)
		return FALSE;
	
	act("Emitting a ghastly grunt, Barbas falls to the ground in a heap and disappears.",mob,0,0,TO_ROOM);

	if (killer != af->owner)
		return FALSE;

	send_to_char("You feel a gluttonous lust enter your body as you stand victorious!\n\r",killer);

	killer->pcdata->lesserdata[LESSER_BARBAS] = FAVOR_GRANTED;
	killer->pcdata->learned[skill_lookup("consume")] = 1;

	extract_char(mob,TRUE);

	return TRUE;
}

void speech_prog_aamon(CHAR_DATA *mob, CHAR_DATA *ch, char *speech)
{
	char *lowspeech;
	AFFECT_DATA *af = affect_find(mob->affected,gsn_lesser_demon);

	if (!af)
		return;

	if (ch != af->owner)
		return;

	lowspeech = talloc_string(lowstring(speech));

	if (strstr(lowspeech,"repeat") || strstr(lowspeech,"again")) {
		do_say(mob,"Most often by hoes and by gardeners I'm chased,");
		do_say(mob,"They cut off my head and then smash it to paste;");
		do_say(mob,"Against them, however, my body is braced,");
		do_say(mob,"For always I'm growing due south of the waist.");
		do_emote(mob,"peers at you expectantly.");
		return;
	}

	if (strstr(lowspeech,"weed")) {
		do_emote(mob,"appears to be crestfallen as the answer to his riddle rings out.");
		do_say(mob,"Very well... defeated once more... take this and let me leave.");
		ch->pcdata->lesserdata[LESSER_AAMON] = FAVOR_GRANTED;
		ch->pcdata->learned[skill_lookup("dark insight")] = 1;
		send_to_char("You stagger under a sudden mental assault, but when it passes your mind seems sharper.\n\r",ch);
		do_emote(mob,"disappears in a cloud of smoke, a frown on his face.");
		extract_char(mob,TRUE);
		return;
	}

	af->modifier++;

	switch(af->modifier) {
		case(1):
			do_emote(mob,"cackles madly, a gleam in his eyes.");
			do_say(mob,"Wrong, wrong, wrong!  You've but two guesses remaining!");
			break;
		case(2):
			do_say(mob,"Ha!  Wrong.");
			do_emote(mob,"'s eyes glisten as he watches you squirm.");
			break;
		default:
			do_emote(mob,"laughs giddily and whirls to leave.");
			RS.Queue.AddToQueue(1, 2, do_say,mob, (char*)"I'll be leaving now,  and leaving you to your thoughts!  You'll never know the answer to my riddle.");
			RS.Queue.AddToQueue(2, 5, act, "With a puff of hazy purple smoke and a sound like a cough, $n disapears.", mob, 0, 0, TO_ROOM);
			RS.Queue.AddToQueue(3, 1, delay_extract, mob);
			ch->pcdata->lesserdata[LESSER_AAMON] = FAVOR_FAILED;
	}
	
	return;
}

void give_prog_malaphar(CHAR_DATA *mob, CHAR_DATA *ch, OBJ_DATA *obj)
{
	AFFECT_DATA *af = affect_find(mob->affected,gsn_lesser_demon);

	if (!af)
		return;

	if (obj->cost < 5000 && obj->pIndexData->limtotal > 8) {
		do_say(mob,"Surely, friend, you do not believe me to be a pauper?");
		do_say(mob,"Entirely unacceptable, I'm afraid.");
		act("$n wraps his cloak around him and vanishes into the shadows.",ch,0,0,TO_ROOM);
		extract_char(mob,TRUE);
		ch->pcdata->lesserdata[LESSER_MALAPHAR] = FAVOR_FAILED;
		return;
	}

	act("A thin smile crosses Malaphar's face as he tips his hat to you and vanishes.",mob,0,0,TO_ROOM);	
	extract_char(mob,TRUE);
	send_to_char("A wave of greed flashes over you as your fingers seem somehow more nimble.\n\r",ch);
	ch->pcdata->lesserdata[LESSER_MALAPHAR] = FAVOR_GRANTED;
	ch->pcdata->learned[skill_lookup("steal")] = 1;

	return;
}

void bribe_prog_malaphar(CHAR_DATA *mob, CHAR_DATA *ch, int amount)
{
	AFFECT_DATA	*af = affect_find(mob->affected,gsn_lesser_demon);
	
	if (!af)
		return;

	if (amount < 10000) {
		do_say(mob,"Surely, friend, you do not believe me to be a pauper?");
		do_say(mob,"Entirely unacceptable, I'm afraid.");
		act("$n wraps his cloak around him and vanishes into the shadows.",ch,0,0,TO_ROOM);
		ch->pcdata->lesserdata[LESSER_MALAPHAR] = FAVOR_FAILED;
		return;
	}

	act("A thin smile crosses Malaphar's face as he tips his hat to you and vanishes.",ch,0,0,TO_ROOM);	
	send_to_char("A wave of greed flashes over you as your fingers seem somehow more nimble.\n\r",ch);

	ch->pcdata->lesserdata[LESSER_MALAPHAR] = FAVOR_GRANTED;
	ch->pcdata->learned[skill_lookup("steal")] = 1;

	return;
}

void greet_prog_furcas(CHAR_DATA *mob, CHAR_DATA *ch)
{
	AFFECT_DATA *af = affect_find(mob->affected,gsn_lesser_demon);

	if (!af)
		return;

	if (ch != af->owner)
		return;

	act("Eyes wide with amazement and fear, $n leaps backward away from you!",mob,0,ch,TO_VICT);
	act("Eyes wide with amazement and fear, $n leaps backward away from $N!",mob,0,ch,TO_NOTVICT);

	do_say(mob,"Do not harm us, we hope it will not!  Here!  Here, take it!");

	ch->pcdata->lesserdata[LESSER_FURCAS] = FAVOR_GRANTED;
	ch->pcdata->learned[skill_lookup("darksight")] = 1;
	send_to_char("Your eyes burn for a moment, and your vision seems somehow sharper.\n\r",ch);
	
	act("$n shrinks back into nearby shadows and is gone.",mob,0,0,TO_ROOM);
	extract_char(mob,TRUE);

	return;
}

void speech_prog_ipos(CHAR_DATA *mob, CHAR_DATA *ch, char *speech)
{
	char buf[MSL];
	AFFECT_DATA *af = affect_find(mob->affected,gsn_lesser_demon);
	int i;

	if (!af)
		return;

	if (!af->owner)
		return;

	if(ch != af->owner)
		return;
	
	if(af->modifier > 4)
		return;

	mob->speechbuf[af->modifier] = palloc_string(speech);

	do_emote(mob,"nods solemnly, furiously scribbling on his hand with an invisible pen.");
	af->modifier++;

	if (af->modifier == 4) {
		do_emote(mob,"nearly loses his balance, doubled over with laughter.");
		act("Ipos recites: $t",mob,mob->speechbuf[0],0,TO_ROOM);
		act("              $t",mob,mob->speechbuf[1],0,TO_ROOM);
		act("              $t",mob,mob->speechbuf[2],0,TO_ROOM);
		act("              $t",mob,mob->speechbuf[3],0,TO_ROOM);
		RS.Queue.AddToQueue(2, 2, do_say,mob, (char*)"Quite the worst bit of garbage I've ever had the joyless task of committing to memory.  Good day,  my rhythmless friend.");
		do_note(mob,"to anti-paladin");
		do_note(mob,"subject Verse, the Worst");
		do_note(mob,"+ Greetings, terrifying and horrible anti-paladins of the realm!");
		do_note(mob,"+");
		sprintf(buf,"+ %s has something he'd like to share with you all, and who am I to", ch->name);
		do_note(mob,buf);
		do_note(mob,"+ avoid facilitating such a request?  Here's a sampling of this most heartfelt");
		do_note(mob,"+ poetry, straight from the lips of your guildmate:");
		do_note(mob,"+");
		for (i=0;i < 4; i++) {
			sprintf(buf,"+ %s",mob->speechbuf[i]);
			do_note(mob,buf);
		}
		do_note(mob,"+");
		do_note(mob,"+");
		do_note(mob,"+ [at the bottom of the scroll, in florid script, is a single 'I']");
		do_note(mob,"send");
		RS.Queue.AddToQueue(4, 2, send_to_char, (char*)"You feel suddenly overcome with a desire to lash out at someone verbally.\n\r", ch);
		ch->pcdata->lesserdata[LESSER_IPOS] = FAVOR_GRANTED;
		ch->pcdata->learned[skill_lookup("taunt")] = 1;
		for (i=0;i < 4; i++)
			free_pstring(mob->speechbuf[i]);
		RS.Queue.AddToQueue(5, 5, act, "With a derisive snort, $n steps through a gate and vanishes without a trace.", mob, 0, 0, TO_ROOM);
		RS.Queue.AddToQueue(6, 1, delay_extract, mob);
	}

	return;
}

void speech_prog_oze(CHAR_DATA *mob, CHAR_DATA *ch, char *speech)
{
	AFFECT_DATA *af = affect_find(mob->affected,gsn_greater_demon);
	char buf[MSL];

	if (!af || !af->owner)
		return;

	if (ch != af->owner)
		return;

	if (!str_prefix("yes", speech)) {
		WAIT_STATE(ch,3*PULSE_VIOLENCE);
		act("Eyes flickering with nearly extinguished flame, Oze incants a phrase.",mob,0,0,TO_ROOM);
		sprintf(buf,"'siphon' %s",ch->name);
		do_cast(mob,buf);
		stop_fighting(mob,TRUE);
		RS.Queue.AddToQueue(3, 5, act, "Suddenly rejuvenated, Oze begins to growl deep in his torso of exposed organs.", mob, 0, 0, TO_ROOM);
		RS.Queue.AddToQueue(5, 2, do_say,mob, (char*)"I would say that your deed would be remembered... but as you were the one who put me here in the first place,  I doubt very much that you would wish it so.  I take my leave.");
		RS.Queue.AddToQueue(7, 5, act, "With a squelching sound, Oze thins into a gout of blood and shoots into the sky.", mob, 0, 0, TO_ROOM);
		RS.Queue.AddToQueue(8, 1, delay_extract, mob);
		RS.Queue.AddToQueue(8, 1, save_char_obj, ch);
		ch->pcdata->greaterdata[GREATER_OZE] = FAVOR_GRANTED;
		ch->pcdata->learned[skill_lookup("leech")] = 1;
		ch->pcdata->perm_hit -= 70;
		ch->max_hit -= 70;
		ch->hit = ch->max_hit;
		return;
	}
	
	if (!str_prefix("no",speech)) {
		sprintf(buf,"The abyss will not forget this treachery, %s.",ch->name);
		do_whisper(mob,buf);
		RS.Queue.AddToQueue(1, 5, act, "The puddle of gore before you that was once a greater demon dissolves into the earth.", mob, 0, 0, TO_ROOM);
		RS.Queue.AddToQueue(2, 1, delay_extract, mob);
		ch->pcdata->greaterdata[GREATER_OZE] = FAVOR_FAILED;
		return;
	}

	do_say(mob,"The choice is a simple one, mortal.  Yes or no?");
	return;
}

void speech_prog_gamygyn(CHAR_DATA *mob, CHAR_DATA *ch, char *speech)
{
	AFFECT_DATA *paf = affect_find(mob->affected,gsn_greater_demon), af;
	char buf[MSL];

	if (!paf || !paf->owner)
		return;

	if (ch != paf->owner)
		return;

	if (!str_prefix("yes", speech)) {
		WAIT_STATE(ch,PULSE_VIOLENCE);
		act("Roaring suddenly with a thunder not unlike a thousand horses, $n rises.",mob,0,0,TO_ROOM);
		RS.Queue.AddToQueue(2, 5, act, "Plunging suddenly headlong, the demon dissolves into your midsection as you scream involuntarily.", mob, 0, ch, TO_VICT);
		RS.Queue.AddToQueue(2, 5, act, "Plunging suddenly headlong, the demon dissolves into $N's midsection as $E screams.", mob, 0, ch, TO_NOTVICT);
		RS.Queue.AddToQueue(3, 1, delay_extract, mob);
		RS.Queue.AddToQueue(3, 2, send_to_char, buf, ch);
		RS.Queue.AddToQueue(3, 4, sprintf,buf,(char*)"%sA dark soul writhes within you, discomfited,  and you remember your bargain.%s\n\r", get_char_color(ch, "blue"), END_COLOR(ch));

		init_affect(&af);
		af.where		=	TO_AFFECTS;
		af.aftype		=	AFT_INVIS;
		af.type			=	gsn_gamygyn_soul;
		af.owner		=	ch;
		af.level		=	60;
		af.duration		=	1000;
		af.modifier		=	0;
		af.location		=	0;
		SET_BIT(af.bitvector, AFF_PERMANENT);
		af.tick_fun		=	gamygyn_tick;
		affect_to_char(ch,&af);
		
	} else if (!str_prefix("no",speech)) {
		WAIT_STATE(ch,PULSE_VIOLENCE);
		act("With a flourishing fanfare which seems nonetheless foreboding, $n rises.",mob,0,0,TO_ROOM);
		RS.Queue.AddToQueue(1, 2, do_tell, mob, buf);
		RS.Queue.AddToQueue(1, 3, sprintf, buf, (char*)"%s You may yet live to regret your rash decision.  Doubtless the seeds of realization sprout within you now.", ch->name);
		RS.Queue.AddToQueue(2, 5, act, "Flashing a great dark light, $n vanishes from sight.", mob, 0, 0, TO_ROOM);
		RS.Queue.AddToQueue(3, 1, delay_extract, mob);
		ch->pcdata->greaterdata[GREATER_GAMYGYN] = FAVOR_FAILED;
	}

	return;
}
		
void speech_prog_orobas(CHAR_DATA *mob, CHAR_DATA *ch, char *speech)
{
	AFFECT_DATA *paf = affect_find(mob->affected,gsn_greater_demon), af;
	char buf[MSL];

	if (!paf || !paf->owner)
		return;

	if (ch != paf->owner)
		return;

	if (!str_prefix("yes",speech)) {
		WAIT_STATE(ch,4*PULSE_VIOLENCE);
		do_emote(mob,"smiles deviously, and you grow confused attempting to locate his lips.");
		RS.Queue.AddToQueue(1, 2, do_say, mob, buf);
		RS.Queue.AddToQueue(1, 3, sprintf,buf, (char*)"Very well.  I shall watch over you.  I require the corpse of another anti-paladin to warm my feet,  %s... I trust you shall not disappoint.", ch->name);
		RS.Queue.AddToQueue(2, 5, act, "Before you realize what's happening, $n has planted a firm grip on your arm.", mob, 0, ch, TO_VICT);
		RS.Queue.AddToQueue(3, 5, act, "The terrible demon's many hands begin grappling with your frame.", mob, 0, ch, TO_VICT);
		RS.Queue.AddToQueue(5, 5, act, "A multitude of taloned fingers dig deeply into your flesh.", mob, 0, ch, TO_VICT);
		RS.Queue.AddToQueue(6, 5, act, "$n draws $mself toward you and you scream involuntarily!", mob, 0, ch, TO_VICT);
		RS.Queue.AddToQueue(8, 5, act, "You writhe in agony and pure horror as $n reaches down your throat...", mob, 0, ch, TO_VICT);
		RS.Queue.AddToQueue(10, 5, act, "In a few moments, $n has vanished entirely into your body.", mob, 0, ch, TO_VICT);
		RS.Queue.AddToQueue(11, 1, delay_extract, mob);
		RS.Queue.AddToQueue(12, 2, send_to_char, buf, ch);
		RS.Queue.AddToQueue(12, 4, sprintf,buf,(char*)"%sA dark soul writhes within you, discomfited,  and you remember your bargain.%s\n\r", get_char_color(ch, "red"), END_COLOR(ch));

		init_affect(&af);
		af.where		=	TO_AFFECTS;
		af.aftype		=	AFT_INVIS;
		af.type			=	gsn_orobas_soul;
		af.owner		=	ch;
		af.level		=	60;
		af.duration		=	1000;
		af.modifier		=	0;
		af.location		=	0;
		SET_BIT(af.bitvector, AFF_PERMANENT);
		af.tick_fun		=	orobas_tick;
		affect_to_char(ch,&af);
		
	} else if (!str_prefix("no",speech)) {
		WAIT_STATE(ch,PULSE_VIOLENCE);
		act("Shrieks of outrage rise up from $n, arms and legs kicking out toward you.",mob,0,0,TO_ROOM);
		RS.Queue.AddToQueue(1, 2, do_say, mob, (char*)"I expected nothing else.");
		RS.Queue.AddToQueue(2, 5, act, "With a fierce snarl and many assorted whispers, $n streaks into the darkness.", mob, 0, 0, TO_ROOM);
		RS.Queue.AddToQueue(3, 1, delay_extract, mob);
		ch->pcdata->greaterdata[GREATER_OROBAS] = FAVOR_FAILED;
	}

	return;
}

void speech_prog_geryon(CHAR_DATA *mob, CHAR_DATA *ch, char *speech)
{

	AFFECT_DATA *paf = affect_find(mob->affected,gsn_greater_demon);
	char buf[MSL];
	
	if (!paf || !paf->owner)
		return;

	if (ch != paf->owner)
		return;

	if (!str_prefix("eye",speech)) {
		WAIT_STATE(ch,4*PULSE_VIOLENCE);
		sprintf(buf,"A wise choice, %s.",ch->name);
		do_say(mob,buf);
		do_emote(mob,"smiles pleasantly once more, reaching to his belt for a small metal tool.");
		RS.Queue.AddToQueue(2, 5, act, "Cupping one hand around the back of your head, Geryon plunges with the tool....", mob, 0, ch, TO_VICT);
		RS.Queue.AddToQueue(4, 5, act, "The greater demon's even, pearly teeth are the last thing your left eye sees.", mob, 0, ch, TO_VICT);
		RS.Queue.AddToQueue(5, 5, act, "The next thing your right eye sees is a hand moving away, holding a bloody orb.", mob, 0, ch, TO_VICT);
		RS.Queue.AddToQueue(6, 5, act, "You stifle a scream as the suddenness of the operation becomes realization.", mob, 0, ch, TO_VICT);
		RS.Queue.AddToQueue(8, 5, act, "Geryon drops your left eye into a bag at his belt, still smiling.", mob, 0, ch, TO_VICT);
		RS.Queue.AddToQueue(10, 2, do_say,mob,(char*)"That's a good lad!  Do try not to cry just yet,  there's nothing quite like the sting of tears in an open eye socket.  Now,  then....");
		RS.Queue.AddToQueue(11, 2, send_to_char, (char*)"The warmth of suffused power tingles through your fingers.\n\r", ch);
		RS.Queue.AddToQueue(12, 2, do_emote,mob, (char*)"bows deeply and disappears,  leaving you alone with your dark thoughts.");
		RS.Queue.AddToQueue(13, 1, delay_extract, mob);
		ch->pcdata->greaterdata[GREATER_GERYON] = GERYON_EYE;
		return;
	}

	if (!str_prefix("finger",speech)) {
		WAIT_STATE(ch,4*PULSE_VIOLENCE);
		sprintf(buf,"A wise choice, %s.",ch->name);
		do_say(mob,buf);
		do_emote(mob,"smiles pleasantly once more, reaching to his belt for a very sharp knife.");
		RS.Queue.AddToQueue(2, 5, act, "Seizing your hand abruptly, Geryon places the blade of his knife against it.", mob, 0, ch, TO_VICT);
		RS.Queue.AddToQueue(4, 5, act, "With a quick sawing motion, the greater demon looses your left index finger!", mob, 0, ch, TO_VICT);
		RS.Queue.AddToQueue(5, 5, act, "Blood pounding in your ears, you're unsure whether the scream you hear is yours.", mob, 0, ch, TO_VICT);
		RS.Queue.AddToQueue(6, 5, act, "Catching the flying finger in his hand, Geryon places it into a bag at his belt.", mob, 0, ch, TO_VICT);
		RS.Queue.AddToQueue(7, 5, act, "Geryon releases your wrist, and gouts of blood begin to pour from the wound.", mob, 0, ch, TO_VICT);
		RS.Queue.AddToQueue(8, 5, act, "Geryon flashes his handsome smile and pushes stray hair back from his face.", mob, 0, ch, TO_VICT);
		RS.Queue.AddToQueue(10, 2, do_say,mob,(char*)"Well done, my boy.  Just stop up that stump you've got,  or you'll bleed to death before I've gone!  Now,  then...");
		RS.Queue.AddToQueue(11, 2, send_to_char, (char*)"The warmth of suffused power tingles through your eyes.\n\r", ch);
		RS.Queue.AddToQueue(12, 2, do_emote,mob, (char*)"bows deeply and disappears,  leaving you alone with your dark thoughts.");
		RS.Queue.AddToQueue(13, 1, delay_extract, mob);
		ch->pcdata->greaterdata[GREATER_GERYON] = GERYON_FINGER;
		return;
	}

	return;
}

void speech_prog_cimeries(CHAR_DATA *mob, CHAR_DATA *ch, char *speech)
{
	AFFECT_DATA *paf = affect_find(mob->affected,gsn_greater_demon);

	if (!paf || !paf->owner)
		return;
	
	if (ch != paf->owner)
		return;

	if (!str_prefix("ear",speech)) {
		WAIT_STATE(ch,PULSE_VIOLENCE*3);
		send_to_char("The sound of Cimeries' claw grinding against your skull fills your soul...",ch);
		RS.Queue.AddToQueue(2, 5, act, "Slowly, with a disgusting grin, Cimeries scrapes your ear away from your head.", mob, 0, ch, TO_VICT);
		RS.Queue.AddToQueue(3, 5, act, "The sound of tearing flesh nearly makes you vomit, and pain buckles your knees.", mob, 0, ch, TO_VICT);
		RS.Queue.AddToQueue(5, 5, act, "Finally, having finished his hideous operation, Cimeries strikes your chest.", mob, 0, ch, TO_VICT);
		RS.Queue.AddToQueue(6, 5, act, "You seem to absorb the blow more readily than you'd have thought possible.", mob, 0, ch, TO_VICT);
		RS.Queue.AddToQueue(8, 5, act, "Suddenly, the monstrous demon fades into shadows and dissipates.", mob, 0, 0, TO_ROOM);
		RS.Queue.AddToQueue(9, 1, delay_extract, mob);
		ch->pcdata->greaterdata[GREATER_CIMERIES] = CIMERIES_EAR;
		ch->pcdata->beauty = UMAX(1,ch->pcdata->beauty - 4);
		return;
	}
		
	if (!str_prefix("nose",speech)) {
		WAIT_STATE(ch,PULSE_VIOLENCE*3);
		send_to_char("You feel Cimeries' claw dig into the bridge of your nose and pull...",ch);
		RS.Queue.AddToQueue(2, 5, act, "Slowly, with a disgusting grin, Cimeries scrapes your nose away from your head.", mob, 0, ch, TO_VICT);
		RS.Queue.AddToQueue(3, 5, act, "The sound of tearing flesh nearly makes you vomit, and pain buckles your knees.", mob, 0, ch, TO_VICT);
		RS.Queue.AddToQueue(5, 5, act, "Finally, having finished his hideous operation, Cimeries strikes your chest.", mob, 0, ch, TO_VICT);
		RS.Queue.AddToQueue(6, 5, act, "You seem to absorb the blow more readily than you'd have thought possible.", mob, 0, ch, TO_VICT);
		RS.Queue.AddToQueue(8, 5, act, "Suddenly, the monstrous demon fades into shadows and dissipates.", mob, 0, 0, TO_ROOM);
		RS.Queue.AddToQueue(9, 1, delay_extract, mob);
		ch->pcdata->greaterdata[GREATER_CIMERIES] = CIMERIES_NOSE;
		ch->pcdata->beauty = UMAX(1,ch->pcdata->beauty - 4);
		return;
	}

	return;
}

void pulse_prog_cornogun(CHAR_DATA *mob)
{
	if(number_percent() != 1 || mob->position != POS_STANDING)
		return;
	
	switch(number_range(0,10)) {
		case(0):
		act("$n chitters annoyingly at everyone in the room.",mob,0,0,TO_ROOM);
		break;
		case(1):
		act("$n pauses momentarily to spit, leaving a disgusting substance all over the floor.",mob,0,0,TO_ROOM);
		break;
		case(2):
		act("$n ceases flapping its wings incessantly to let out a mournful screech.",mob,0,0,TO_ROOM);
		break;
		default:
		spell_faerie_fog( skill_lookup("faerie fog"), mob->level, mob, NULL, TAR_IGNORE);
		break;
	}
}

void entry_prog_cornogun(CHAR_DATA *mob)
{
	act("$n flaps in on its tiny wings.",mob,0,0,TO_ROOM);
	spell_faerie_fog(skill_lookup("faerie fog"),mob->level,mob,NULL,TAR_IGNORE);
	return;
}	

void pulse_prog_imp(CHAR_DATA *mob)
{
	CHAR_DATA *victim;
	OBJ_DATA *steal;	

	if(number_percent() > 40 || mob->position != POS_STANDING)
		return;
	
	for(victim = mob->in_room->people; victim; victim = victim->next_in_room) {
		if ((number_percent() < 98)
		|| ((mob->master) && ((victim == mob->master)
		|| is_safe_new(mob->master,victim,FALSE)))
		|| mob == victim)
			continue;

		
		for(steal = victim->carrying; steal; steal = steal->next_content) {
			if(steal->wear_loc == WEAR_NONE && can_see_obj(mob,steal))  {
				if (number_percent() > 75) {
					if (IS_NPC(victim))
						multi_hit(victim,mob,TYPE_UNDEFINED);
				} else {
					obj_from_char(steal);
					obj_to_char(steal,mob);
				}
				return;
			}
		}
	}

	return;
}


void fight_prog_geulgon(CHAR_DATA *mob, CHAR_DATA *victim)
{
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;
	CHAR_DATA *ch=mob->master;
	if(mob->fighting == NULL)
		return;

	if(!victim)
		return;
	
	if(number_percent() < 75)
		return;	
	
	switch(number_range(1,5)) {
		
		case(1):
			spell_energy_drain(skill_lookup("energy drain"),mob->level,mob,victim,TAR_CHAR_OFFENSIVE);
			break;
		case(2):
		case(3):
			spell_vampiric_touch(skill_lookup("vampiric touch"),mob->level,mob,victim,TAR_CHAR_OFFENSIVE);
			break;
		case(4):
		case(5):
			        for (vch = mob->in_room->people; vch != NULL; vch = vch_next)
        			{
       				 vch_next = vch->next_in_room;
        			if (is_same_group(vch,ch))
                			continue;
       				 if (ch->cabal>0 && ch->cabal==vch->cabal)
                			continue;
        			if (is_safe(ch,vch) && vch->fighting!=NULL && vch->fighting!=ch)
            			continue;
				spell_iceball(skill_lookup("iceball"),mob->level,mob,vch,TAR_IGNORE);
				}
		break;
		
	}

}

void give_prog_minotaur(CHAR_DATA *mob, CHAR_DATA *ch, OBJ_DATA *obj)
{
	
	if(ch != mob->master) {
		act("$N snarls angrily at $n's offering, taking $p and breaking it over his knee!",ch,obj,mob,TO_ROOM);
		act("$n snarls angrily at your offering, taking $p and breaking it over his knee!",mob,obj,ch,TO_VICT);
		obj_from_char(obj);
		extract_obj(obj);
		return;
	}

	if(obj->item_type != ITEM_WEAPON) {
		act("$n growls and throws $p to the ground.",mob,obj,ch,TO_ROOM);
		obj_from_char(obj);
		obj_to_room(obj,mob->in_room);
		return;
		}
	
	act("$n lets out a fierce warcry as he wields $p.",mob,obj,ch,TO_ROOM);
	
	if(get_eq_char(mob,WEAR_WIELD) == NULL) {
		equip_char(mob,obj,WEAR_WIELD,TRUE);
		return;
	} else {
		equip_char(mob,obj,WEAR_DUAL_WIELD,TRUE);
		return;
	}
}
/*
void fight_prog_minotaur(CHAR_DATA *mob, CHAR_DATA *victim)
{
	OBJ_DATA *obj = get_eq_char(mob,WEAR_WIELD),obj2 = get_eq_char(mob,WEAR_DUAL_WIELD);

	switch(obj->value[0]) {
		
		case(WEAPON_SWORD):
			
		case(WEAPON_AXE):
		
		
		case(WEAPON_SPEAR):
		
	
		case(WEAPON_STAFF):
		
		
		case(WEAPON_FLAIL):

		case(WEAPON_WHIP):

		
		case(WEAPON_DAGGER):

		case(WEAPON_POLEARM):

	}
return;
}		
*/
void fight_prog_gking(CHAR_DATA *mob, CHAR_DATA *victim)
{
	CHAR_DATA *gar1, *gar2;
	AFFECT_DATA af;

	if((mob->hit*100/mob->max_hit) < 75 && !is_affected(mob, gsn_gking))
	{
		init_affect(&af);
		af.where    = TO_AFFECTS;
		af.type     = gsn_gking;
		af.aftype   = AFT_SKILL;
		af.level    = 51;
		af.duration = 15;
		affect_to_char(mob, &af);
		gar1 = create_mobile(get_mob_index(2233));
		gar2 = create_mobile(get_mob_index(2233));
		act("$n gestures slightly animating the two mithril statues next to the throne!",mob,0,0,TO_ROOM);
		char_to_room(gar1, mob->in_room);
		char_to_room(gar2, mob->in_room);
		act("$n screams and attacks you!", gar1, 0, victim, TO_VICT);
		act("$n screams and attacks $N!", gar1, 0, victim, TO_NOTVICT);
		do_murder(gar1, victim->name);
		act("$n screams and attacks you!", gar2, 0, victim, TO_VICT);
		act("$n screams and attacks $N!", gar2, 0, victim, TO_NOTVICT);
		do_murder(gar2, victim->name);
	}
}

void pulse_prog_nocturnal_mob(CHAR_DATA *mob)
{
	if ((sun == SUN_RISE || sun == SUN_LIGHT) && number_percent() < 30 && mob->fighting == NULL)
	{
		extract_char(mob,TRUE);
	}
	return;
}

void pulse_prog_diurnal_mob(CHAR_DATA *mob)
{
	if (sun == SUN_DARK && number_percent() < 30 && mob->fighting == NULL)
	{
		extract_char(mob,TRUE);	
	}
	return;
}

bool death_prog_cim(CHAR_DATA *mob, CHAR_DATA *killer)
{
	CHAR_DATA *extract;
	
	for(extract = mob->in_room->people; extract != NULL; extract = extract->next_in_room) {
		if(IS_NPC(extract) && extract->pIndexData->vnum == 2499)
			break;
		continue;
	}

	extract_char(extract,TRUE);
	return TRUE;
}

void greet_prog_tower_shopkeeper(CHAR_DATA *mob, CHAR_DATA *ch)
{
	AFFECT_DATA af;
	if(is_affected(mob,gsn_bash))
		return;

	if(IS_NPC(ch))
		return;
	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = gsn_bash;
	af.duration = -1;
	affect_to_char(mob,&af);
	act("$N glances over his shoulder at $n and grumbles.",ch,0,mob,TO_ROOM);
	act("$n glances over his shoulder at you and grumbles.",mob,0,ch,TO_VICT);
	RS.Queue.AddToQueue(4, 2, do_say,mob, (char*)"Well,  are ye goin' to buy somethin' or are ye just going t'stand there and stare at me!?");
	RS.Queue.AddToQueue(24, 2, do_astrip, mob, (char*)"");	
	return;
}

void pulse_prog_wizard_summon(CHAR_DATA *mob)
{
	ROOM_INDEX_DATA *room = get_room_index(2402);
	ROOM_INDEX_DATA *room2 = mob->in_room;
	CHAR_DATA *vch, *vch_next;
	bool found = FALSE;
	AFFECT_DATA af, *paf = NULL;
	
	if(!room->people && is_affected(mob,gsn_bash)) {
		affect_strip(mob,gsn_bash);
		return;
	}

	for (vch = room->people; vch; vch = vch_next) {
		vch_next = vch->next_in_room;

		if (IS_NPC(vch))
			continue;

		found = FALSE;

		if(is_affected(mob,gsn_bash)) {
			for(paf = mob->affected; paf; paf = paf->next) {
				if (paf->type == gsn_bash && paf->owner == vch) {
					found = TRUE;
					break;
				}
			}
		}

		if (found) {
			paf->modifier--;
			if(paf->modifier == 0) {
				do_emote(mob,"utters the words, 'gjwwai'.");
				char_from_room(vch);
				char_to_room(vch,room2);
				act("$n has summoned you!",mob,0,vch,TO_VICT);
				act("$N arrives suddenly.",mob,0,vch,TO_NOTVICT);
				do_look(vch,"auto");
				RS.Queue.AddToQueue(1, 2, do_say,mob, (char*)"Please be more careful, and examine your surroundings.  You'll live longer.");
				affect_remove(mob,paf);
			}
		} else {
			init_affect(&af);
			af.where = TO_AFFECTS;
			af.modifier = 10;
			af.duration = -1;
			af.owner = vch;
			af.type = gsn_bash;
			affect_to_char(mob,&af);
		}
	}

	return;
}

void greet_prog_notescribe(CHAR_DATA *mob, CHAR_DATA *ch)
{
	AFFECT_DATA af;	

	mprog_say(1,"Greetings!  I am the Scribe of Melcene.",mob,ch);
	mprog_say(4,"I will write scrolls for those less literate than I to be sent to all of Shalar!",mob,ch);
	mprog_say(5,"For a price, of course.",mob,ch);
	mprog_say(8,"Read the sign for details.",mob,ch);
	mprog_say(9,"Interested?",mob,ch);

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = gsn_bash;
	af.aftype = AFT_INVIS;
	af.modifier = 0;
	af.duration = -1;
	af.end_fun = 0;
	af.pulse_fun = 0;
	affect_to_char(ch,&af);
}
/* I blame a loss of inspiration 
void speech_prog_notescribe(CHAR_DATA *mob, CHAR_DATA *ch, char *speech)
{
	AFFECT_DATA *af=affect_find(ch->affected,gsn_bash);
	int price;

	if(af->modifier == 0 && agrees(ch,speech) == REPLY_YES) {
		mprog_say(0,"Excellent.  Please state who I should this to.",mob,ch);
		af->modifier = 1;
	}
	
	if(af->modifier == 1) {
		sprintf(buf,"I have this addressed to %s.  Is this correct?",speech);
		af->modifier = 2);
	}

	if(af->modifier == 2 && agrees(ch,speech) == REPLY_YES) {
		do_emote(mob,"nods quickly and carefully addresses the scroll.")
		mprog_say(2,"And what subject will this scroll address?",mob,ch);
		sprintf(buf,"to %s",speech);
		do_note(mob,buf); 
		af->modifier = 3;
	}

	if(af->modifier == 2 && agrees(ch,speech) == REPLY_NO) {
		do_emote(mob,"sighs.");
		
	if(af->modifier == 1) {
		if(!check_ooc(ch,speech)) {
			mprog_say(0,"Bah, I'm not writing that!",mob,ch);
			return;
		}
		
		
		*/

CHAR_DATA* check_sector( int min, int max);
void set_aggressor_hunt(CHAR_DATA *mob);
bool verify_aggressor( CHAR_DATA *mob, CHAR_DATA *ch, int min, int max);

void beat_prog_law_track(CHAR_DATA *mob)
{
	CHAR_DATA *ch = mob->last_fought;
	char buf[MSL];
	int min=0,max=0;

	switch(mob->pIndexData->vnum)
	{
		case 2410:
			min = 2400;
			max = 2427;
		break;
		case 2700:
			min = 2428;
			max = 2461;
		break;
		case 2703:
			min = 2462;
			max = 2492;
		break;
		case 2706:
			min = 2493;
			max = 2522;
		break;
		case 2709:
			min = 2523;
			max = 2555;
		break;
		case 2712:
			min = 2556;
			max = 2587;
		break;
		case 2715:
			min = 2588;
			max = 2616;
		break;
		case 2718:
			min = 2617;
			max = 2634;
		break;
		case 2721:
			min = 2635;
			max = 2652;
		break;
	}

	if(min == 0 || max == 0)
		return;

	if(IS_AFFECTED(mob, AFF_NOSHOW))
		return;

	if(ch == NULL)
	{
		mob->last_fought = check_sector( min, max );                                                            
		if(mob->last_fought != NULL)
		{
			set_aggressor_hunt(mob);
			ch = mob->last_fought;
		}
		else
		{
			mob->last_fought = NULL;
			mob->tracktimer = 0;
			return;
		}
	}

	if(is_affected(ch, gsn_disguise))
		return;
	
	if(!verify_aggressor( mob, ch, min, max))
	{
		mob->last_fought = NULL;
		mob->tracktimer = 0;
		return;
	}

	if(mob->fighting)
		return;

	if(ch->ghost > 0)
		return;

	if(!is_affected(ch, gsn_aggressor))
	{
		mob->last_fought = NULL;
		mob->tracktimer = 0;
		return;
	}

	if(!can_see(mob, ch) || !IS_AWAKE(mob))
	{
		mob->last_fought = NULL;
		mob->tracktimer = 0;
		return;
	}
	
	if (ch->in_room == mob->in_room)
	{
		if(mob->pIndexData->attack_yell)
			sprintf(buf,"%s", act_msg(mob->pIndexData->attack_yell, ch));
		else
			sprintf(buf,"%s, now you die!", ch->name);
		do_yell(mob, buf);
		multi_hit(mob,mob->last_fought,TYPE_UNDEFINED);
		return;
	}
	
	if (number_range(1,6) == 1)
		smart_track(ch,mob);

	return;
}

bool verify_aggressor( CHAR_DATA *mob, CHAR_DATA *ch, int min, int max)
{
	ROOM_INDEX_DATA *pRoom;
	CHAR_DATA *tch;
	int i;
	bool found=FALSE;

	for ( i = min; i <= max; i++ )
	{
		if ( ( pRoom = get_room_index(i) ) )
		{
			for( tch = pRoom->people; tch != NULL; tch = tch->next_in_room )
			{
				if(ch == tch)
					found=TRUE;
			}
		}
	}

	return found;
}
	
CHAR_DATA* check_sector( int min, int max)
{
	ROOM_INDEX_DATA *pRoom;
	CHAR_DATA *ch;
	int i;

	for ( i = min; i <= max; i++ )
	{
		if ( ( pRoom = get_room_index(i) ) )
		{
			for( ch = pRoom->people; ch != NULL; ch = ch->next_in_room )
			{
				if(IS_NPC(ch))
					continue;

				if(is_affected(ch, gsn_aggressor))
					return ch;
			}
		}
	}

	return NULL;
}

void set_aggressor_hunt(CHAR_DATA *mob)
{
	char store[MSL];
	if(!can_see(mob,mob->last_fought) || 
			mob->last_fought->ghost > 0 ||
			is_affected(mob, gsn_gag))
		return;
	if(mob->in_room != mob->last_fought->in_room)
	{
		sprintf(store,"Halt, %s, you murderous scum!  You will bleed from there to the gates!", mob->last_fought->name);
		do_yell(mob,store);
	}
	mob->tracktimer = 12;
	return;
}

void pulse_prog_fisherman(CHAR_DATA *mob)
{
	OBJ_DATA *obj=NULL, *obj_next=NULL, *fish=NULL;
	AFFECT_DATA af;

	for(obj=mob->in_room->contents; obj != NULL; obj=obj_next) {
		obj_next = obj->next_content;

		if(obj->pIndexData->vnum == 2473)
			break;
	}

	if(!obj)
		return;
	
	fish = create_object(get_obj_index(2474),1);
	
	 if (get_obj_weight( fish ) + get_contents_weight( obj )
             > (obj->value[0]) || is_affected(mob,gsn_bash)) {
		 extract_obj(fish);
		 return;	
	 }

	act("$n calmly reels in his catch and deposits it into the basket at his side.",mob,0,0,TO_ROOM);
	obj_to_obj(fish,obj);

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = gsn_bash;
	af.duration = 0;
	af.modifier = 0;
	af.aftype = AFT_TIMER;
	affect_to_char(mob,&af);

	return;	
}

void greet_prog_guild_recruiter(CHAR_DATA *mob, CHAR_DATA *ch)
{
	if(ch->level != 1)
		return;

	act("$N smiles warmly at you.",ch,0,mob,TO_CHAR);
	mprog_say(1,"Take the time to read the sign here, friend Shalaran... and if you should decide you would like to join me in the Guild, take heed of its advice.",mob,ch);
	return;
}

void wake_up(CHAR_DATA *ch)
{
	if(ch->position == POS_SLEEPING)
		ch->position = POS_RESTING;
	if(is_affected(ch, gsn_subdue))
		affect_strip(ch, gsn_subdue);
}

char *get_insult(CHAR_DATA *ch)
{
	if(!str_cmp(race_table[ch->race].name,"duergar"))
	{
		if(number_range(1,2) == 1)
			return("coal-faced dirtbag");
		else
			return("brush-bearded mud eater");
	}
	else if(!str_cmp(race_table[ch->race].name,"dwarf"))
	{
		if(number_range(1,3) == 1)
			return("stumpy scump-head");
		else if(number_range(1,2) == 1)
			return("bearded rock-head");
		else
			return("ugly half-pint ball of scump");
	}
	else if(!str_cmp(race_table[ch->race].name,"elf"))
	{
		if(number_range(1,2) == 1)
			return("pale-faced tenderfoot");
		else
			return("lanky-limbed pile of scump");
	}
	else if(!str_cmp(race_table[ch->race].name,"drow"))
	{
		if(number_range(1,2) == 1)
			return("skulking cavedweller");
		else
			return("slug-eating sneak");
	}
	else if(!str_cmp(race_table[ch->race].name,"planar"))
	{
		if(number_range(1,2) == 1)
			return("bloody goon");
		else
			return("sparking sideshow menace");
	}
	else if(!str_cmp(race_table[ch->race].name,"celestial"))
	{
		if(number_range(1,2) == 1)
			return("bloody goon");
		else
			return("stinking mountain of lard");
	}
	else if(!str_cmp(race_table[ch->race].name,"abyss"))
	{
		if(number_range(1,2) == 1)
			return("bloody goon");
		else
			return("titanic mutton chop");
	}
	else if(!str_cmp(race_table[ch->race].name,"imp"))
	{
		if(number_range(1,2) == 1)
			return("brainless scump");
		else
			return("scrawny bootfodder");
	}
	else if(!str_cmp(race_table[ch->race].name,"minotaur"))
	{
		if(number_range(1,2) == 1)
			return("flaming cow");
		else
			return("horn-faced bull brain");
	}
	else if(!str_cmp(race_table[ch->race].name,"halfling"))
	{
		if(number_range(1,2) == 1)
			return("knee-high bootlicker");
		else
			return("hairless pugger");
	}
	else if(!str_cmp(race_table[ch->race].name,"sidhe"))
	{
		if(number_range(1,2) == 1)
			return("tree-hugging faerie featherbrain");
		else
			return("bloody insect");
	}
	else
	{
		if(number_range(1,2) == 1)
			return("dimwitted scump-bucket");
		else
			return("pugger loving gate goon");
	}
}

void fight_prog_law_subdue(CHAR_DATA *mob, CHAR_DATA *ch)
{
	CHAR_DATA *hydra, *dragon;
	int hydra_vnum, dragon_vnum, vnum = 0, num;
	ROOM_INDEX_DATA *pRoom;
	AFFECT_DATA af;
	char buf[MSL];
	float chance;

	switch(mob->pIndexData->vnum)
	{
		case 2410:
			hydra_vnum = 2409;
			dragon_vnum = 2408;
		break;
		default:
			dragon_vnum = mob->pIndexData->vnum + 1;
			hydra_vnum = mob->pIndexData->vnum + 2;
	}

	if(((hydra = get_char_room(mob,"hydra trooper soldier")) == NULL) ||
	   ((dragon = get_char_room(mob,"dragon trooper soldier")) == NULL))	
		return;

	if(!is_affected(ch, gsn_aggressor))
		return;
	chance = ((float)ch->hit / (float)ch->max_hit) * 100 + 10;
	if(number_percent() > chance)
	{
		stop_fighting(ch, TRUE);
		stop_fighting(mob, TRUE);
		mob->last_fought = NULL;
		mob->tracktimer = 0;
		if(number_range(1,2) == 1)
			act("The Hydra trooper dives at your legs, binding you, and you topple to the ground!", mob, 0, ch, TO_VICT);
		else
			act("You crumple as the Dragon trooper lands a hefty blow on the side of your head!", mob, 0, ch, TO_VICT);
		act("The troopers leap on top of you, and you find yourself quickly immobilized!", mob, 0, ch, TO_VICT);
		act("The troopers leap on top of $N, and $N finds $Mself quickly immobilized!", mob, 0, ch, TO_NOTVICT);
		act("You hear the wet sounds of impact against your skull, and everything goes black.", mob, 0, ch, TO_VICT);
		act("$N's inert form is dragged away toward the gates.", mob, 0, ch, TO_NOTVICT);
		ch->position = POS_SLEEPING;
		init_affect(&af);
	    af.where    = TO_AFFECTS;
		af.type     = gsn_subdue;
	    af.aftype   = AFT_INVIS;
		SET_BIT(af.bitvector, AFF_SLEEP);
		af.level    = 60;
	    af.duration = 12;
	    affect_to_char(ch, &af);

		switch(mob->pIndexData->vnum)
		{
			case 2410:
				num = number_range( 0, 4 );
				switch(num)
				{
					case 0: vnum = 3502; break;
					case 1: vnum = 3507; break;
					case 2: vnum = 3509; break;
					case 3: vnum = 3516; break;
					case 4: vnum = 3517; break;
				}
				break;
			case 2700:
				num = number_range( 0, 3 );
				switch(num)
				{
					case 0: vnum = 533; break;
					case 1: vnum = 534; break;
					case 2: vnum = 542; break;
					case 3: vnum = 551; break;
				}
				break;
			case 2703:
				num = number_range( 0, 4 );
				switch(num)
				{
					case 0: vnum = 528; break;
					case 1: vnum = 529; break;
					case 2: vnum = 522; break;
					case 3: vnum = 523; break;
					case 4: vnum = 524; break;
				}
				break;
			case 2706:
				num = number_range( 0, 3 );
				switch(num)
				{
					case 0: vnum = 2326; break;
					case 1: vnum = 2327; break;
					case 2: vnum = 2328; break;
					case 3: vnum = 2329; break;
				}
				break;
			case 2709:                                                                                                
				num = number_range( 0, 3 );
				switch(num)
				{
					case 0: vnum = 201; break;
					case 1: vnum = 202; break;
					case 2: vnum = 203; break;
					case 3: vnum = 204; break;
				}
				break;                                                                                                    
			case 2712:                                                                                                
				num = number_range( 0, 3 );
				switch(num)
				{
					case 0: vnum = 2319; break;
					case 1: vnum = 2320; break;
					case 2: vnum = 2321; break;
					case 3: vnum = 2323; break;
				}
				break;
			case 2715:                                                                                                        
				num = number_range( 0, 3 );
				switch(num)
				{
					case 0: vnum = 2331; break;
					case 1: vnum = 2332; break;
					case 2: vnum = 2333; break;
					case 3: vnum = 2334; break;
				}
				break;
			case 2718:                                                                                                        
				num = number_range( 0, 3 );
				switch(num)
				{
					case 0: vnum = 6008; break;
					case 1: vnum = 6009; break;
					case 2: vnum = 6011; break;
					case 3: vnum = 6014; break;
				}
				break;
			case 2721:                                                                                                        
				num = number_range( 0, 2 );
				switch(num)
				{
					case 0: vnum = 14257; break;
					case 1: vnum = 14267; break;
					case 2: vnum = 14268; break;
				}
				break;
		}
		
		pRoom = get_room_index(vnum);
		if(is_affected(ch,gsn_plasma_thread))
			affect_strip(ch,gsn_plasma_thread);

		char_from_room(ch);
		char_to_room(ch, get_room_index(5));
		RS.Queue.AddToQueue(10, 2, (void*)send_to_char,(char*)"You are dimly aware of being dragged along the ground...\n\r", ch);
		RS.Queue.AddToQueue(19, 1, (void*)wake_up, ch);
		RS.Queue.AddToQueue(19, 2, (void*)affect_strip, ch, &gsn_subdue);
		RS.Queue.AddToQueue(19, 2, (void*)char_to_room, ch, pRoom);
		RS.Queue.AddToQueue(19, 1, (void*)char_from_room, ch);
		RS.Queue.AddToQueue(20, 2, (void*)send_to_char,(char*)"You are shaken awake by a fuzzy man, as seen through your own personal haze.\n\r", ch);
		RS.Queue.AddToQueue(20, 5, act, (char*)"$N comes to $S knees holding the back of $S head.", 0, 0, ch, TO_NOTVICT);
		RS.Queue.AddToQueue(21, 2, (void*)send_to_char, buf, ch);
		RS.Queue.AddToQueue(21, 5, (void*)sprintf, buf,
						(char*)"A Hydra trooper tells you '%sAnd stay out of Cimar, ye %s!%s'\n\r", 
						get_char_color(ch,"tells"), get_insult(ch), END_COLOR(ch));
	}
	else
	{
		act("The troopers maneuver around you, waiting for an opening.", mob, 0, ch, TO_VICT);
		act("The troopers maneuver around $N, waiting for an opening.", mob, 0, ch, TO_NOTVICT);
	}
	return;
}

void pulse_prog_troopers(CHAR_DATA *mob)
{
	CHAR_DATA *gryphon;

	if((gryphon = get_char_room(mob,"gryphon trooper soldier")) == NULL) 
	{
		act("$n looks around hurriedly then retreats to call in reinforcements.", mob, 0, 0, TO_ROOM);
		extract_char(mob, TRUE);
	}

	return;
}

void pulse_prog_area_echo_ward(CHAR_DATA *mob)
{
	DESCRIPTOR_DATA *d;

	//hightime = armor[3] lowtime = armor[0]
	if(!mob->in_room->area->nplayer || time_info.hour < mob->armor[2] || time_info.hour > mob->armor[3])
		return;
	if(!mob->timer)
	{
		mob->timer = 1;
		mob->regen_rate = mob->level;   //interval in sec = mob level
	}
	mob->regen_rate = UMAX(0,mob->regen_rate - 4); //(PULSE_MOBILE / PULSE_PER_SECOND), one mpulse per 4 sec
	if(mob->regen_rate)
		return;
	for(d = descriptor_list; d; d = d->next)
	{
		if(d->connected != CON_PLAYING || !d->character->in_room || d->character->in_room->area != mob->in_room->area 
			   || d->character->in_room->vnum < mob->armor[0] || d->character->in_room->vnum > mob->armor[1])
			continue;

		{
			char buf[MSL];
			colorconv(buf,mob->short_descr,mob);
			write_to_buffer(d,buf,0);
			write_to_buffer(d,"\n\r",0);
		}
	}
	mob->regen_rate = mob->level;
				
}

void pulse_prog_shade(CHAR_DATA *mob)
{
      	CHAR_DATA *victim = mob->fighting;
      	int sn;
	char *spell = NULL;
	if(!victim || number_percent() > 13)
		return;
	
	if(number_percent()>80) {
			act("$n plunges an icy hand into the $N's heart!",mob,0,victim,TO_NOTVICT);
			act("$n plunges an icy hand into your heart!",mob,0,victim,TO_VICT);
			spell = "chill";
	}
	else {
			act("$n creates a conduit into $N's soul and siphons $S life away!",mob,0,victim,TO_NOTVICT);
			act("$n creates a conduit into your soul and siphons your life away!!",mob,0,victim,TO_VICT);
			spell = "vampiric touch";
	}
	if ( ( sn = skill_lookup( spell ) ) < 0 )
		return;
	(*skill_table[sn].spell_fun) ( sn, mob->level+1, mob, victim, TARGET_CHAR );
	return;
}

void pulse_prog_banshee(CHAR_DATA *mob)
{
	CHAR_DATA *victim = mob->fighting;
	int sn, dam;
	char *spell = NULL;
	if(!victim || number_percent() > 20)
		return;
	if(number_percent()>70) {
			act("$n lets out a terrible wail that forces you to cover your ears in pain!",mob,0,victim,TO_VICT);
			act("$n lets out a terrible wail as $e draws near to $N.",mob,0,victim,TO_NOTVICT);
			dam = dice(20,7) + 20;
			damage_new(mob,victim,dam,
				TYPE_UNDEFINED,DAM_SOUND,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"piercing scream");
	}
	else if(number_percent()>40)
	{
			act("$n screeches gleefully, mimicing a laugh.",mob,0,victim,TO_ALL);
			act("$n's face contorts in terror.",victim,0,mob,TO_ALL);
			act("$N's wailing causes your muscles to contort wildly in pain!",mob,0,victim,TO_CHAR);
			spell = "weaken";
	}
	else {
			act("$n begins to howl mournfully, the unnatural notes piercing your mind.",mob,0,victim,TO_ALL);
			spell = "deafen";
	}
	if (!spell || ( sn = skill_lookup( spell ) ) < 0 )
		return;
	(*skill_table[sn].spell_fun) ( sn, mob->level+1, mob, victim, TARGET_CHAR);
	return;	
}

void pulse_prog_phantasm(CHAR_DATA *mob)
{
	CHAR_DATA *victim = mob->fighting;
	int sn=0;
	char *spell=NULL;
	if(!victim || number_percent() > 10)
		return;
	
	switch(number_range(0,5)) {
		case 0:
			act("$n shows you the face of your worst nightmare, paralyzing you with fear!",mob,0,victim,TO_VICT);
			act("$n's ghostly form flickers briefly as $N flinches away in terror.",mob,0,victim,TO_NOTVICT);
			LAG_CHAR(victim,(int)(PULSE_VIOLENCE*3));
			break;
		case 1:
		case 2:
		case 3:
			act("Your death flashes before your eyes.",0,0,victim,TO_VICT);
			act("You feel hope slip away.",0,0,victim,TO_VICT);
			act("$N gets a look of hopelessness on $S face.",mob,0,victim,TO_NOTVICT);
			spell = "curse";
			break;
		case 4:
		case 5:
			act("$n runs $s fingers down $N's face gently.",mob,0,victim,TO_NOTVICT);
			act("$n runs $s fingers down your face gently.",mob,0,victim,TO_VICT);
			act("$n plunges $s spectral hand into $N's head, leaving $M looking confused.",mob,0,victim,TO_NOTVICT);
			act("$n plunges $s spectral hand into your head, leaving you feeling confused.",mob,0,victim,TO_VICT);
			spell = "forget";
			break;
	}
	if (!spell || ( sn = skill_lookup( spell ) ) < 0 )
		return;
	(*skill_table[sn].spell_fun) ( sn, mob->level+1, mob, victim, TARGET_CHAR );
	return;
}

void pulse_prog_ravghoul(CHAR_DATA *mob)
{
	CHAR_DATA *victim = mob->fighting;
	AFFECT_DATA af;

	if(victim==NULL || victim->in_room != mob->in_room || number_percent() > 10)
		return;

	switch(number_range(0,2)) {
		case (0):
		if(is_affected(victim,gsn_abite))
			return;
		act("$n grabs $N's arm and takes a bite out of it!",mob,0,victim,TO_NOTVICT);
		act("$n grabs your arm and takes a bite out of it!",mob,0,victim,TO_VICT);
		one_hit_new(mob,victim,TYPE_UNDEFINED,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"bite");
		init_affect(&af);
		af.where = TO_AFFECTS;
		af.type = gsn_abite;
		af.aftype = AFT_MALADY;
		af.location = APPLY_STR;
		af.duration = mob->level/6;
		af.modifier = -mob->level/9;
		af.level = mob->level;
		affect_to_char(victim,&af);
		break;
		case (1):
		if(is_affected(victim,gsn_lbite))
			return;
		act("$n grabs $N's leg and takes a bite out of it!",mob,0,victim,TO_NOTVICT);
		act("$n grabs your leg and takes a bite out of it!",mob,0,victim,TO_VICT);
		one_hit_new(mob,victim,TYPE_UNDEFINED,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"bite");
		init_affect(&af);
		af.where = TO_AFFECTS;
		af.type = gsn_lbite;
		af.aftype = AFT_MALADY;
		af.location = APPLY_DEX;
		af.duration = mob->level/6;
		af.modifier = -mob->level/6;
		af.level = mob->level;
		affect_to_char(victim,&af);
		break;	
		case(2):
		act("$n moves to $N's flank and takes a bite from the back of $S head!",mob,0,victim,TO_NOTVICT);
		act("$n moves to your flank and takes a bite from the back of your head!",mob,0,victim,TO_VICT);
		one_hit_new(mob,victim,TYPE_UNDEFINED,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"bite");
		LAG_CHAR(victim,PULSE_VIOLENCE*number_range(1,2));
		break;
	}
	return;
}

void pulse_prog_behemoth(CHAR_DATA *mob)
{
	CHAR_DATA *victim = mob->fighting;
	int dam;
	if(victim==NULL ||  victim->in_room != mob->in_room || number_percent() > 15)
		return;
	
	dam = dice(14,14);
	act("Bones clattering in a terrible cacophony, $N charges at $n clumsily!",victim,0,mob,TO_NOTVICT);
	act("$N is trampled to the ground!",mob,0,victim,TO_NOTVICT);
	act("Bones clattering in a terrible cacophony, $n charges at you!",mob,0,victim,TO_VICT);
	act("You are trampled to the ground!",mob,0,victim,TO_VICT);
	damage_new(mob,victim,dam,TYPE_UNDEFINED,DAM_BASH,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"trampling");
	if(is_affected(victim,skill_lookup("protective shield")) && number_percent() < 70)
		return;
	LAG_CHAR(victim,PULSE_VIOLENCE*number_range(2,3));
	return;
}

void pulse_prog_glass(CHAR_DATA *mob)
{
	CHAR_DATA *victim = mob->fighting;
	AFFECT_DATA af, caf;

	if(victim && victim->in_room == mob->in_room
	   && number_percent() > 90
	   && !is_affected(mob,gsn_trophy)
	   && !is_affected(victim,gsn_impale))
	{
		act("$n raises one of its massive arms and impales $N on it!",mob,0,victim,TO_NOTVICT);
		act("$n raises one of its massive arms and impales you on it!",mob,0,victim,TO_VICT);
		damage_new(mob,victim,dice(16,15),
			TYPE_UNDEFINED,DAM_PIERCE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"impalement");
		init_affect(&af);
		af.where = TO_AFFECTS;
		af.location = APPLY_STR;
		af.modifier = -mob->level/7;
		af.duration = mob->level/4;
		af.type = gsn_bleeding;
		af.aftype = AFT_MALADY;
		af.level = mob->level;
		af.owner = mob->leader;
		af.end_fun = NULL;
		new_affect_to_char(victim,&af);
		af.location = APPLY_DEX;
		affect_to_char(victim,&af);	
		init_affect(&caf);
		caf.where = TO_AFFECTS;
		caf.location = 0;
		caf.modifier = 0;
		caf.duration = 1;
		caf.type = gsn_trophy;
		caf.level = mob->level;
		caf.aftype = AFT_SKILL;
		caf.owner = mob->leader;
		caf.end_fun = NULL;
		new_affect_to_char(mob,&caf);
		return;
	}
	return;
}

void pulse_prog_night_creeps(CHAR_DATA *mob)
{
	CHAR_DATA *victim;
	if(!IS_AFFECTED(mob, AFF_NOSHOW) && !IS_AFFECTED(mob, AFF_DETECT_MAGIC))
		return (void)SET_BIT(mob->affected_by, AFF_NOSHOW);
	if((sun == SUN_RISE || sun == SUN_LIGHT) && !IS_AFFECTED(mob, AFF_NOSHOW))
	{
		if(mob->fighting)
			stop_fighting(mob,TRUE);
		act("As the first rays of the sun emerge, $n scuttles away with sickening speed.",mob,0,0,TO_ROOM);
		SET_BIT(mob->affected_by, AFF_NOSHOW);
		return;
	}
	else if((sun == SUN_SET || sun == SUN_DARK) && IS_AFFECTED(mob, AFF_NOSHOW) && IS_AFFECTED(mob, AFF_DETECT_MAGIC))
	{
		REMOVE_BIT(mob->affected_by, AFF_NOSHOW);
		act("Chittering with mindless malice, $n bursts from the cover of shadow and attacks!",mob,0,0,TO_ROOM);
		for(victim = mob->in_room->people; victim; victim = victim->next_in_room)
			if(victim != mob && !is_safe_new(mob,victim,FALSE))
			{
				one_hit(mob,victim,TYPE_UNDEFINED);
				break;
			}
		return;
	}
	else if(mob->fighting && mob->pIndexData->vnum == 3001)
	{
		AFFECT_DATA af;
		if(number_percent() < 91)
			return;
		act("With a vicious clacking noise, $n sinks its razor-sharp pincers into $N!",mob,0,mob->fighting,TO_NOTVICT);
		act("With a vicious clacking noise, $n sinks its razor-sharp pincers into you!",mob,0,mob->fighting,TO_VICT);
		damage_new(mob,mob->fighting,dice(mob->fighting->level/3,3),TYPE_UNDEFINED,DAM_PIERCE,TRUE,
			HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"pincering claws$");
		init_affect(&af);
		af.where	= TO_AFFECTS;
		af.aftype 	= AFT_MALADY;
		af.level 	= (short)(mob->fighting->level / 2.5);
		af.duration 	= mob->fighting->level / 10;
		if(number_range(1,2) == 1)
			af.location = APPLY_STR;
		else
			af.location = APPLY_DEX;
		af.modifier = 0 - mob->fighting->level / 11;
		af.type = af.location == APPLY_STR ? gsn_abite : gsn_lbite;
		if(!is_affected(mob->fighting, gsn_lbite) && !is_affected(mob->fighting, gsn_abite))
			af.tick_fun = bleeding_tick;
		affect_to_char(mob->fighting, &af);
	}
}
void sucker_pulse(CHAR_DATA *ch, AFFECT_DATA *af)
{
	CHAR_DATA *owner;
	for(owner = char_list; owner; owner = owner->next)
		if(IS_NPC(owner) && owner->pIndexData->vnum == 3002 && owner->hunting == ch)
			break;
	if(!owner)
	{
		act("$n sighs in relief as the remains of the flatworm fall away from $s face.",ch,0,0,TO_ROOM);
		send_to_char("You sigh in relief as the remains of the flatworm fall away from your face.\n\r",ch);
		affect_strip(ch, gsn_blindness);
		return;
	}
	if(number_range(1,3) != 1)
		return;
	act("$n swells slightly as it drains the bodily fluids from $N.",owner,0,ch,TO_NOTVICT);
	act("$n swells slightly as it drains your bodily fluids!",owner,0,ch,TO_VICT);
	damage_new(owner,ch,dice(8,5),
		TYPE_UNDEFINED,DAM_ACID,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"suction");
	owner->hit += number_range(20,40);
}
void greet_prog_face_sucker(CHAR_DATA *mob, CHAR_DATA *ch)
{
	char buf[MSL];
	AFFECT_DATA af;
	if(mob->fighting || IS_NPC(ch) || mob == ch || mob->hunting == ch || !can_see(mob,ch) || is_safe_new(mob,ch,FALSE))
		return;
	mob->hunting = ch;
	sprintf(buf,"%sSuddenly, something pale and slimy falls from the ceiling, landing on your face!%s",
		get_char_color(ch,"red"),END_COLOR(ch));
	act(buf,mob,0,ch,TO_VICT);
	sprintf(buf,"%sSuddenly, something small and pale falls from the ceiling, landing on $N's face!%s",
		get_char_color(ch,"red"),END_COLOR(ch));
	act(buf,mob,0,ch,TO_NOTVICT);
	init_affect(&af);
	af.where	= TO_AFFECTS;
	af.level	= 90;
	af.type		= gsn_blindness;
	af.location	= APPLY_HITROLL;
	af.modifier	= -10;
	af.owner	= mob;
	af.duration	= 6;
	af.pulse_fun	= sucker_pulse;
	SET_BIT(af.bitvector, AFF_BLIND);
	act("You scream in horror as $n latches itself onto your face!",mob,0,ch,TO_VICT);
	act("$N lets out a scream of pure horror as $n latches itself onto $S face!",mob,0,ch,TO_NOTVICT);
	affect_to_char(ch, &af);
	multi_hit(mob,ch,TYPE_UNDEFINED);
}

