/**************************************************************************r
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
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "magic.h"
#include "recycle.h"
#include "tables.h"
#include "spec.h"

/* command procedures needed */
DECLARE_DO_FUN(do_return	);
DECLARE_DO_FUN(do_wake          );

/*
 * Local functions.
 */
void	affect_modify	args( ( CHAR_DATA *ch, AFFECT_DATA *paf, bool add ) );
void	object_modify	args( ( OBJ_DATA *obj, AFFECT_DATA *paf, bool fAdd ) );
bool	is_safe_rspell	args( ( int level, CHAR_DATA *victim) );
void    modify_location args( (CHAR_DATA *ch, int location, int mod, bool add) );
/* friend stuff -- for NPC's mostly */
bool is_friend(CHAR_DATA *ch,CHAR_DATA *victim)
{
    if (is_same_group(ch,victim))
	return TRUE;


    if (!IS_NPC(ch))
	return FALSE;

    if (!IS_NPC(victim))
    {
	if (IS_SET(ch->off_flags,ASSIST_PLAYERS))
	    return TRUE;
	else
	    return FALSE;
    }

    if (IS_AFFECTED(ch,AFF_CHARM))
	return FALSE;

    if (IS_SET(ch->off_flags,ASSIST_ALL))
	return TRUE;

    if (ch->group && ch->group == victim->group)
	return TRUE;

    if (IS_SET(ch->off_flags,ASSIST_VNUM)
    &&  ch->pIndexData == victim->pIndexData)
	return TRUE;

    if (IS_SET(ch->off_flags,ASSIST_RACE) && ch->race == victim->race)
	return TRUE;

    if (IS_SET(ch->off_flags,ASSIST_ALIGN)
    &&  !IS_SET(ch->act,ACT_NOALIGN) && !IS_SET(victim->act,ACT_NOALIGN)
    &&  ((IS_GOOD(ch) && IS_GOOD(victim))
    ||	 (IS_EVIL(ch) && IS_EVIL(victim))
    ||   (IS_NEUTRAL(ch) && IS_NEUTRAL(victim))))
	return TRUE;

    return FALSE;
}

/* returns number of people on an object */
int count_users(OBJ_DATA *obj)
{
    CHAR_DATA *fch;
    int count = 0;

    if (obj->in_room == NULL)
	return 0;

    for (fch = obj->in_room->people; fch != NULL; fch = fch->next_in_room)
	if (fch->on == obj)
	    count++;

    return count;
}

/* returns race number */
int race_lookup (const char *name)
{
   int race;

   for ( race = 0; race_table[race].name != NULL; race++)
   {
	if (LOWER(name[0]) == LOWER(race_table[race].name[0])
	&&  !str_prefix( name,race_table[race].name))
	    return race;
   }

   return -1;
}
int act_lookup (const char *name)
{
   int act;

   for ( act = 0; act_flags[act].name != NULL; act++)
   {
        if (LOWER(name[0]) == LOWER(act_flags[act].name[0])
        &&  !str_prefix( name,act_flags[act].name))
            return act;
   }
   return -1;
}
int liq_lookup (const char *name)
{
    int liq;

    for ( liq = 0; liq_table[liq].liq_name != NULL; liq++)
    {
	if (LOWER(name[0]) == LOWER(liq_table[liq].liq_name[0])
	&& !str_prefix(name,liq_table[liq].liq_name))
	    return liq;
    }


    return 0;
}

int weapon_lookup (const char *name)
{
    int type;

    for (type = 0; weapon_table[type].name != NULL; type++)
    {
	if (LOWER(name[0]) == LOWER(weapon_table[type].name[0])
	&&  !str_prefix(name,weapon_table[type].name))
	    return type;
    }

    return -1;
}

int weapon_num_lookup (const char *name)
{
    int type;

    for (type = 0; weapon_table[type].name != NULL; type++)
    {
	if (LOWER(name[0]) == LOWER(weapon_table[type].name[0])
	&&  !str_prefix(name,weapon_table[type].name))
	    return weapon_table[type].type;
    }

    return -1;
}


char * weapon_name_lookup (int type)
{
    int count;

    for (count = 0; weapon_table[count].name != NULL; count++)
    {
	if (weapon_table[count].type == type)
	    return weapon_table[count].name;
    }

    return "weapon";
}

int weapon_type (const char *name)
{
    int type;

    for (type = 0; weapon_table[type].name != NULL; type++)
    {
        if (LOWER(name[0]) == LOWER(weapon_table[type].name[0])
        &&  !str_prefix(name,weapon_table[type].name))
            return weapon_table[type].type;
    }

    return WEAPON_EXOTIC;
}


int item_lookup(const char *name)
{
    int type;

    for (type = 0; item_table[type].name != NULL; type++)
    {
        if (LOWER(name[0]) == LOWER(item_table[type].name[0])
        &&  !str_prefix(name,item_table[type].name))
            return item_table[type].type;
    }

    return -1;
}

char *item_name(int item_type)
{
    int type;

    for (type = 0; item_table[type].name != NULL; type++)
	if (item_type == item_table[type].type)
	    return item_table[type].name;
    return "none";
}

char *weapon_name( int weapon_type)
{
    int type;

    for (type = 0; weapon_table[type].name != NULL; type++)
        if (weapon_type == weapon_table[type].type)
            return weapon_table[type].name;
    return "exotic";
}

int attack_lookup  (const char *name)
{
    int att;

    for ( att = 0; attack_table[att].name != NULL; att++)
    {
	if (LOWER(name[0]) == LOWER(attack_table[att].name[0])
	&&  !str_prefix(name,attack_table[att].name))
	    return att;
    }

    return 0;
}

/* returns a flag for wiznet */
long wiznet_lookup (const char *name)
{
    int flag;

    for (flag = 0; wiznet_table[flag].name != NULL; flag++)
    {
	if (LOWER(name[0]) == LOWER(wiznet_table[flag].name[0])
	&& !str_prefix(name,wiznet_table[flag].name))
	    return flag;
    }

    return -1;
}

char *color_value_string( int color, bool bold, bool flash )
{
    static char buf[64];
    static sh_int COLOR_INDEX[8] = { 30, 34, 32, 31, 36, 35, 33, 37 };

    if ( flash && bold )
    	sprintf( buf, "\x01B[1;%dm%s", COLOR_INDEX[color%8], ANSI_BLINK );
    else if ( flash )
    	sprintf( buf, "\x01B[0;%dm%s", COLOR_INDEX[color%8], ANSI_BLINK );
    else if ( bold )
    	sprintf( buf, "\x01B[1;%dm", COLOR_INDEX[color%8] );
    else
    	sprintf( buf, "\x01B[0;%dm", COLOR_INDEX[color%8] );

    return buf;
}

int strlen_color( char *argument )
{
    char 	*str;
    sh_int	length;

    if ( argument==NULL || argument[0]=='\0' )
 	return 0;

    length=0;
    str=argument;

    while ( *str != '\0' )
    {
    	if ( *str != '{' )
    	{
    	    str++;
    	    length++;
    	    continue;
    	}

    	switch ( *(++str) )
    	{
    	    default:    length+=2; 	break;
    	    case '-': 	
    	    case 'x': 	length++; 	break;
    	    case '<': 	
    	    case '0':	
    	    case '1':	
    	    case '2':	
    	    case '3':	
    	    case '4':	
    	    case '5':	
    	    case '6':	
    	    case '7':	
    	    case 'B':	
    	    case 'b':	
    	    case 'F':	
    	    case 'f':	
    	    case 'n': 			break;
    	}
    	
    	str++;
    }

    return length;
}

/* for immunity, vulnerabiltiy, and resistant
   the 'globals' (magic and weapons) may be overriden
   three other cases -- wood, silver, and iron -- are checked in fight.c */

int check_immune(CHAR_DATA *ch, int dam_type)
{
    int immune, def;
    int bit;

    immune = -1;
    def = IS_NORMAL;

    if (dam_type == DAM_NONE)
	return immune;

    if (dam_type <= 3)
    {
	if (IS_SET(ch->imm_flags,IMM_WEAPON))
	    def = IS_IMMUNE;
	else if (IS_SET(ch->res_flags,RES_WEAPON))
	    def = IS_RESISTANT;
	else if (IS_SET(ch->vuln_flags,VULN_WEAPON))
	    def = IS_VULNERABLE;
    }
    else /* magical attack */
    {	
	if (IS_SET(ch->imm_flags,IMM_MAGIC))
	    def = IS_IMMUNE;
	else if (IS_SET(ch->res_flags,RES_MAGIC))
	    def = IS_RESISTANT;
	else if (IS_SET(ch->vuln_flags,VULN_MAGIC))
	    def = IS_VULNERABLE;
    }

    /* set bits to check -- VULN etc. must ALL be the same or this will fail */
    switch (dam_type)
    {
	case(DAM_BASH):		bit = IMM_BASH;		break;
	case(DAM_PIERCE):	bit = IMM_PIERCE;	break;
	case(DAM_SLASH):	bit = IMM_SLASH;	break;
	case(DAM_FIRE):		bit = IMM_FIRE;		break;
	case(DAM_COLD):		bit = IMM_COLD;		break;
	case(DAM_LIGHTNING):	bit = IMM_LIGHTNING;	break;
	case(DAM_ACID):		bit = IMM_ACID;		break;
	case(DAM_POISON):	bit = IMM_POISON;	break;
	case(DAM_NEGATIVE):	bit = IMM_NEGATIVE;	break;
	case(DAM_HOLY):		bit = IMM_HOLY;		break;
	case(DAM_ENERGY):	bit = IMM_ENERGY;	break;
	case(DAM_MENTAL):	bit = IMM_MENTAL;	break;
	case(DAM_DISEASE):	bit = IMM_DISEASE;	break;
	case(DAM_DROWNING):	bit = IMM_DROWNING;	break;
	case(DAM_LIGHT):	bit = IMM_LIGHT;	break;
	case(DAM_CHARM):	bit = IMM_CHARM;	break;
	case(DAM_SOUND):	bit = IMM_SOUND;	break;
	case(DAM_INTERNAL):	bit = IMM_INTERNAL;	break;
	default:		return def;
    }


    if (IS_SET(ch->imm_flags,bit))
	immune = IS_IMMUNE;
    else if (IS_SET(ch->res_flags,bit) && immune != IS_IMMUNE)
	immune = IS_RESISTANT;
    else if (IS_SET(ch->vuln_flags,bit))
    {
	if (immune == IS_IMMUNE)
	    immune = IS_RESISTANT;
	else if (immune == IS_RESISTANT)
	    immune = IS_NORMAL;
	else
	    immune = IS_VULNERABLE;
    }

    if (immune == -1)
	return def;
    else
      	return immune;
}

bool is_cabal(CHAR_DATA *ch)
{
    return ch->cabal;
}

bool is_same_cabal(CHAR_DATA *ch, CHAR_DATA *victim)
{
    if (cabal_table[ch->cabal].independent)
	return FALSE;
    else
        return (ch->cabal == victim->cabal);
}

/* checks mob format */
bool is_old_mob(CHAR_DATA *ch)
{
    if (ch->pIndexData == NULL)
	return FALSE;
    else if (ch->pIndexData->new_format)
	return FALSE;
    return TRUE;
}

/* for returning skill information */
int get_skill(CHAR_DATA *ch, int sn)
{
    int skill = 0, gn = 0;
    AFFECT_DATA *af;
    bool using_switched = FALSE;
    CHAR_DATA *original = ch;

    if(IS_NPC(ch) && ch->desc && ch->desc->original && IS_SET(ch->comm, COMM_SWITCHSKILLS))
  	using_switched = TRUE;
    
    if(using_switched)
	ch = ch->desc->original;
 
    if (sn == -1) /* shorthand for level based skills */
		skill = ch->level * 5 / 2;
    else if (sn < -1 || sn > MAX_SKILL) {
		bug("Bad sn %d in get_skill.",sn);
		skill = 0;
    } else if (!IS_NPC(ch)) {
		if (ch->level < skill_table[sn].skill_level[ch->Class()->GetIndex()] && !IS_IMMORTAL(ch))
			skill = 0;
		else
			skill = ch->pcdata->learned[sn];
		if (skill == -2)
			skill = 0;
	} else if (ch->pIndexData->Class()->GetIndex() > CLASS_NONE) {
		if (ch->level < skill_table[sn].skill_level[ch->pIndexData->Class()->GetIndex()])
			skill = 0;
		else if (ch->pIndexData->Class()->GetIndex() == CLASS_WARRIOR) {
			if (!style_check(sn,ch->mobstyle))	
				skill = 0;
			else
				skill = URANGE(0, 50 + 2 * (ch->level - skill_table[sn].skill_level[ch->pIndexData->Class()->GetIndex()]), 100);
		} else if (ch->pIndexData->Class()->GetIndex() == CLASS_SORCERER) {
			;
		} else
			skill = URANGE(0, 50 + 2 * (ch->level - skill_table[sn].skill_level[ch->pIndexData->Class()->GetIndex()]), 100);
	} else {
		if (skill_table[sn].spell_fun != spell_null)
	    		skill = 40 + 2 * ch->level;
		else if (sn == gsn_sneak)
			skill = ch->level * 2 + 20;
		else if ((sn == gsn_dodge && IS_SET(ch->off_flags,OFF_DODGE))
			|| (sn == gsn_parry && IS_SET(ch->off_flags,OFF_PARRY)))
			skill = ch->level * 2;
		else if (sn == gsn_shield_block || sn == gsn_throw || sn == gsn_hand_to_hand || sn == gsn_kick || sn == gsn_rescue
	  	        || sn == gsn_ground_control || sn == gsn_throw || sn == gsn_blindness_dust || sn == gsn_poison_dust)
			skill = 25 + 2 * ch->level;
		else if ((sn == gsn_trip && IS_SET(ch->off_flags,OFF_TRIP)) ||
			(sn == gsn_bash && IS_SET(ch->off_flags,OFF_BASH)) ||
			(sn == gsn_disarm && IS_SET(ch->off_flags,OFF_DISARM)) ||
			(sn == gsn_berserk && IS_SET(ch->off_flags,OFF_BERSERK)) ||
			(sn == gsn_tail && IS_SET(ch->off_flags,OFF_TAIL)))
			skill = 35 + 2 * ch->level;
		else if (sn == gsn_sword ||  sn == gsn_dagger ||  sn == gsn_spear ||  sn == gsn_mace ||  sn == gsn_axe
			||  sn == gsn_flail ||  sn == gsn_whip ||  sn == gsn_polearm)
			skill = 50 + 5 * ch->level / 2;
		else if ((gn = gn_skill_lookup(sn)) > 1
			&& IS_SET(ch->progtypes,MPROG_FIGHT) ) {
		
			if ((!str_cmp(group_table[gn].name,"gladiator") && !str_cmp(ch->pIndexData->mprogs->fight_name,"fight_prog_gladiator"))
				||  (!str_cmp(group_table[gn].name,"barbarian") && !str_cmp(ch->pIndexData->mprogs->fight_name,"fight_prog_barbarian"))
				||  (!str_cmp(group_table[gn].name,"duelist") && !str_cmp(ch->pIndexData->mprogs->fight_name,"fight_prog_duelist"))
				||  (!str_cmp(group_table[gn].name,"skirmisher") && !str_cmp(ch->pIndexData->mprogs->fight_name,"fight_prog_skirmisher"))
				||  (!str_cmp(group_table[gn].name,"dragoon") && !str_cmp(ch->pIndexData->mprogs->fight_name,"fight_prog_dragoon"))
				||  (!str_cmp(group_table[gn].name,"tactician") && !str_cmp(ch->pIndexData->mprogs->fight_name,"fight_prog_tactician")))
				skill = ch->level + 50;
		} else
			skill = 0;
    }

	if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK]  > 10 )
		skill = 9 * skill / 10;

	//note: AFTER THIS NPCS WITH SWITCHED SKILLS RETURN TO THEIR NPC STATUS
	if(using_switched)
		ch = original;

	if (is_affected(ch,gsn_forget)) 
		if (number_percent() > skill/2)
			skill /= 3;

	if (skill > 5 && is_affected(ch,gsn_synaptic_enhancement))
		skill+= 5;

	if (skill > 5 && is_affected(ch,gsn_synaptic_impairment))
		skill-= 5;
	
	if (is_affected(ch,gsn_leadership))
		skill+=(int)(skill * .1);
	
	if(is_affected_room(ch->in_room,gsn_infidels_fate) && IS_GOOD(ch))
		skill +=(int)(skill * .1);

	if (ch->fighting && is_affected(ch->fighting,gsn_traitors_luck)) 
	{
		af = affect_find(ch->fighting->affected,gsn_traitors_luck);
		if (ch == af->owner)
			skill += 20;
	}
	
	if(ch->fighting && (IS_EVIL(ch) && is_affected(ch->fighting,gsn_awe)) 
			&& (ch->level > ch->fighting->level) && number_percent() > 96)
	{
		skill = 0;
		send_to_char("Distracted by your opponent's aura of holiness, you lose your concentration.\n\r",ch);
	}
	
	return URANGE(0,skill,100);
}

int get_weapon_sn_new(CHAR_DATA *ch, int type);

/* for returning weapon information */
int get_weapon_sn(CHAR_DATA *ch)
{
	return get_weapon_sn_new(ch,0);
}
int get_weapon_sn_new(CHAR_DATA *ch, int type)
{
    OBJ_DATA *wield;
    int sn;

    wield = get_eq_char( ch, WEAR_WIELD );
    if(type == gsn_dual_wield)
	wield = get_eq_char( ch, WEAR_DUAL_WIELD );
    if (wield == NULL || wield->item_type != ITEM_WEAPON)
        sn = gsn_hand_to_hand;
    else switch (wield->value[0])
    {
        default :               sn = -1;                break;
        case(WEAPON_SWORD):     sn = gsn_sword;         break;
        case(WEAPON_DAGGER):    sn = gsn_dagger;        break;
        case(WEAPON_SPEAR):     sn = gsn_spear;         break;
        case(WEAPON_MACE):      sn = gsn_mace;          break;
        case(WEAPON_AXE):       sn = gsn_axe;           break;
        case(WEAPON_FLAIL):     sn = gsn_flail;         break;
        case(WEAPON_WHIP):      sn = gsn_whip;          break;
        case(WEAPON_POLEARM):   sn = gsn_polearm;       break;
	case(WEAPON_STAFF):	sn = gsn_staff;	break;
   }
   return sn;
}

int get_weapon_skill(CHAR_DATA *ch, int sn)
{
     int skill;

     /* -1 is exotic */
    if (IS_NPC(ch))
    {
	if (sn == -1)
	    skill = 3 * ch->level;
	else if (sn == gsn_hand_to_hand)
	    skill = 40 + 2 * ch->level;
	else
	    skill = 40 + 5 * ch->level / 2;
    }

    else
    {
	if (sn == -1)
	    skill = 3 * ch->level;
	else
	    skill = ch->pcdata->learned[sn];
    }

    return URANGE(0,skill,100);
}


/* used to de-screw characters */
void reset_char(CHAR_DATA *ch)
{
     int loc,stat;
     OBJ_DATA *obj;
     AFFECT_DATA *af;
	 OBJ_APPLY_DATA *app;
     int i;

     if (IS_NPC(ch))
	return;

    if (ch->pcdata->perm_hit == 0
    ||	ch->pcdata->perm_mana == 0
    ||  ch->pcdata->perm_move == 0
    ||	ch->pcdata->last_level == 0)
    {
    /* do a FULL reset */
	for (loc = 0; loc < MAX_WEAR; loc++)
	{
	    obj = get_eq_char(ch,loc);
	    if (obj == NULL)
			continue;
	    for ( af = obj->charaffs; af != NULL; af = af->next )
			affect_strip(ch, af->type);
		for ( app = obj->apply; app != NULL; app = app->next )
			modify_location(ch, app->location, app->modifier, FALSE);
	}
	/* now reset the permanent stats */
	ch->pcdata->perm_hit 	= ch->max_hit;
	ch->pcdata->perm_mana 	= ch->max_mana;
	ch->pcdata->perm_move	= ch->max_move;
	ch->pcdata->last_level	= ch->played/3600;
	if (ch->pcdata->true_sex < 0 || ch->pcdata->true_sex > 2)
	{
		if (ch->sex > 0 && ch->sex < 3)
	    	    ch->pcdata->true_sex	= ch->sex;
		else
		    ch->pcdata->true_sex 	= 0;
	}
    }

    /* now restore the character to his/her true condition */
    for (stat = 0; stat < MAX_STATS; stat++)
	ch->mod_stat[stat] = 0;

    if (ch->pcdata->true_sex < 0 || ch->pcdata->true_sex > 2)
	ch->pcdata->true_sex = 0;
    ch->sex		= ch->pcdata->true_sex;
    ch->max_hit 	= ch->pcdata->perm_hit;
    ch->max_mana	= ch->pcdata->perm_mana;
    ch->max_move	= ch->pcdata->perm_move;

    for (i = 0; i < 4; i++)
    	ch->armor[i]	= 0;

    ch->hitroll		= 0;
    ch->damroll		= 0;
    ch->saving_throw	= 0;
    /* now start adding back the effects */
    for (loc = 0; loc < MAX_WEAR; loc++)
    {
        obj = get_eq_char(ch,loc);
        if (obj == NULL)
            continue;
	for (i = 0; i < 4; i++)
	    ch->armor[i] += apply_ac( obj, loc, i );

	for ( af = obj->charaffs; af != NULL; af = af->next )
		affect_modify(ch, af, TRUE);
	for ( app = obj->apply; app != NULL; app = app->next )
		modify_location(ch, app->location, app->modifier, TRUE);
    }

    /* now add back spell effects */
    for (af = ch->affected; af != NULL; af = af->next)
	modify_location(ch, af->location, af->modifier, TRUE);
    /* make sure sex is RIGHT!!!! */
    if (ch->sex < 0 || ch->sex > 2)
	ch->sex = ch->pcdata->true_sex;
}


/*
 * Retrieve a character's trusted level for permission checking.
 */
int get_trust( CHAR_DATA *ch )
{
    if ( ch->desc != NULL && ch->desc->original != NULL )
		ch = ch->desc->original;

    if (ch->trust)
		return ch->trust;

    if ( IS_NPC(ch) && ch->level >= LEVEL_HERO )
		return LEVEL_HERO - 1;
    else
		return ch->level;
}


/*
 * Retrieve a character's age.
 */
/* command for retrieving stats */
int get_curr_stat( CHAR_DATA *ch, int stat )
{
	AFFECT_DATA *af;
    int max;
    int iClass;
	int mod = 0;

    iClass = ch->Class()->GetIndex();

    if (IS_NPC(ch) || ch->level > LEVEL_IMMORTAL)
	max = 25;

    else
    {
	max = pc_race_table[ch->race].max_stats[stat];

    if (!str_cmp(race_table[ch->race].name,"human"))
    {
	switch(ch->Class()->attr_prime) {
		case(STAT_STR):
			if(stat == STAT_STR && 
			(ch->Class()->GetIndex() == CLASS_WARRIOR ||
			ch->Class()->GetIndex() == CLASS_ANTI_PALADIN ||
			ch->Class()->GetIndex() == CLASS_RANGER ||
			ch->Class()->GetIndex() == CLASS_PALADIN))
				max = 23;
			else
				max = 20;
			break;
                case(STAT_INT):                                  
                        if(stat == STAT_INT &&
			(ch->Class()->GetIndex() == CLASS_SORCERER ||
			ch->Class()->GetIndex() == CLASS_NECROMANCER ||
			ch->Class()->GetIndex() == CLASS_SHAPESHIFTER))             
                                max = 23;
                        else                                    
                                max = 20;
                        break;
                case(STAT_WIS):
                        if(stat == STAT_WIS &&  
                        (ch->Class()->GetIndex() == CLASS_HEALER || 
                        ch->Class()->GetIndex() == CLASS_ZEALOT)) 
                                max = 23;
                        else
                                max = 20;
                        break;
                case(STAT_DEX):
                        if(stat == STAT_DEX &&   
                        (ch->Class()->GetIndex() == CLASS_ASSASSIN || 
                        ch->Class()->GetIndex() == CLASS_THIEF))   
                                max = 23;                                     
                        else
                                max = 20;
                        break;
	}
    }			



 	max = UMIN(max,25);
    }

	if (ch->fighting && is_affected(ch->fighting,gsn_traitors_luck)) {
		af = affect_find(ch->fighting->affected,gsn_traitors_luck);
		if (ch == af->owner)
			mod = 2;
	}

    return UMIN(URANGE(3,ch->perm_stat[stat] + ch->mod_stat[stat], max) + mod,25);
}

/* command for returning max training score */
int get_max_train( CHAR_DATA *ch, int stat )
{
    int max;
    int iClass;

    iClass = (ch->Class()->GetIndex() + 1);

    if (IS_NPC(ch) || ch->level > LEVEL_IMMORTAL)
	return 25;

    max = pc_race_table[ch->race].max_stats[stat];

    if (!str_cmp(race_table[ch->race].name,"human"))
    {
        switch(ch->Class()->attr_prime) {
                case(STAT_STR):
                        if(stat == STAT_STR &&
                        (ch->Class()->name == "warrior" ||
                        ch->Class()->name == "anti-paladin" ||
                        ch->Class()->name == "ranger" ||
                        ch->Class()->name == "paladin"))
                                max = 23;
                        else
                                max = 20;
                        break;
                case(STAT_INT):
                        if(stat == STAT_INT &&
                        (ch->Class()->name == "sorcerer" ||
                        ch->Class()->name == "necromancer" ||
                        ch->Class()->name == "shapeshifter"))
                                max = 23;
                        else
                                max = 20;
                        break;
                case(STAT_WIS):
                        if(stat == STAT_WIS &&
                        (ch->Class()->name == "healer" ||
                        ch->Class()->name == "zealot"))
                                max = 23;
                        else
                                max = 20;
                        break;
                case(STAT_DEX):
                        if(stat == STAT_DEX &&
                        (ch->Class()->name == "assassin" ||
                        ch->Class()->name == "thief"))
                                max = 23;
                        else
                                max = 20;
                        break;
        }
    }			


    return UMIN(max,25);
}

	
/*
 * Retrieve a character's carry capacity.
 */
int can_carry_n( CHAR_DATA *ch )
{
    if ( !IS_NPC(ch) && ch->level >= LEVEL_IMMORTAL )
	return 1000;

    if ( IS_NPC(ch) && IS_SET(ch->act, ACT_PET) )
	return 0;

    return MAX_WEAR + ch->level/6 + dex_app[get_curr_stat(ch,STAT_DEX)].carry;
}



/*
 * Retrieve a character's carry capacity.
 */
int can_carry_w( CHAR_DATA *ch )
{
    if ( !IS_NPC(ch) && ch->level >= LEVEL_IMMORTAL )
		return 10000000;

    if ( IS_NPC(ch) && IS_SET(ch->act, ACT_PET) )
		return 0;

    return MAX_WEAR + ch->level*2 + str_app[get_curr_stat(ch,STAT_STR)].carry;
}

/* Calculate if players are in pk range to attack each other */
bool can_pk( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int xp, vxp;

    if (ch == victim)
        return TRUE;

	if( ch == NULL || victim == NULL)
		return FALSE;

    if (IS_NPC(ch)
	|| IS_NPC(victim)
	|| (ch->ghost > 0)
	|| (victim->ghost > 0)
	|| (ch->level < MIN_LEVEL_TO_PK)
	|| (victim->level < MIN_LEVEL_TO_PK)
	|| abs(victim->level - ch->level) > 9)
	return FALSE;

	if (!IS_NPC(ch) 
		&& !IS_NPC(victim) 
		&& ch->level == LEVEL_HERO 
		&& victim->level == LEVEL_HERO)
	return TRUE;
	
    xp = ch->exp;
    vxp = victim->exp;

     if (xp > MIN_PK_XP && vxp > MIN_PK_XP)
    {
        if (ch->level - victim->level < MIN_LEVEL_TO_PK
            && ch->level - victim->level > -MIN_LEVEL_TO_PK)
        {
            if ( ( (xp *.7) < vxp) && ((xp * 1.3) > vxp))
                return TRUE;
            else if ( ((vxp * .7) < xp) && ((vxp * 1.3) > xp))
                return TRUE;
            else
                return FALSE;
        }
        else
            return FALSE;
    }
    else
        return FALSE;
}

/*
 * See if a string is one of the names of an object.
 */

bool is_name ( const char *str, const char *namelist )
{
    char name[MAX_INPUT_LENGTH], part[MAX_INPUT_LENGTH];
    char *list, *string;

    /* fix crash on NULL namelist */
    if (namelist == NULL || namelist[0] == '\0')
    	return FALSE;

    /* fixed to prevent is_name on "" returning TRUE */
    if (str == NULL || str[0] == '\0')
        return FALSE;

    string = (char *)str;
    /* we need ALL parts of string to match part of namelist */
    for ( ; ; )  /* start parsing string */
    {
	str = one_argument((char *)str,part);

	if (part[0] == '\0' )
	    return TRUE;

	/* check to see if this is part of namelist */
	list = (char *)namelist;
	for ( ; ; )  /* start parsing namelist */
	{
	    list = one_argument(list,name);
	    if (name[0] == '\0')  /* this name was not found */
		return FALSE;

	    if (!str_cmp(string,name))
		return TRUE; /* full pattern match */

	    if (!str_cmp(part,name))
		break;
	}
    }
}

bool is_exact_name(char *str, char *namelist )
{
    char name[MAX_INPUT_LENGTH];

    if (namelist == NULL)
	return FALSE;

    for ( ; ; )
    {
	namelist = one_argument( namelist, name );
	if ( name[0] == '\0' )
	    return FALSE;
	if ( !str_cmp( str, name ) )
	    return TRUE;
    }
}

void init_affect( AFFECT_DATA *paf )
{
	paf->where		= TO_AFFECTS;
	paf->name		= NULL;
	paf->type		= 0;
	paf->aftype		= AFT_SPELL;
	paf->level		= 0;
	paf->init_duration = 0;
	paf->duration	= 0;
	paf->location	= 0;
	paf->modifier	= 0;
	paf->mod_name	= MOD_NONE;
	zero_vector(paf->bitvector);
	paf->tick_fun	= NULL;
	paf->pulse_fun	= NULL;
	paf->end_fun	= NULL;
	paf->owner		= NULL;
	paf->beat_fun	= NULL;
	return;
}

/*
 * Apply or remove an affect to a character.
 */
void affect_modify( CHAR_DATA *ch, AFFECT_DATA *paf, bool fAdd )
{
    OBJ_DATA *wield;
    int mod;
	bool disarmed = FALSE;

    mod = paf->modifier;

    if ( fAdd )
    {
	switch (paf->where)
	{
	case TO_AFFECTS:
	    BITWISE_OR(ch->affected_by, paf->bitvector);
	    break;
	case TO_IMMUNE:
	    BITWISE_OR(ch->imm_flags,paf->bitvector);
	    break;
	case TO_RESIST:
	    BITWISE_OR(ch->res_flags,paf->bitvector);
	    break;
	case TO_VULN:
	    BITWISE_OR(ch->vuln_flags,paf->bitvector);
	    break;
	}
    }
    else
    {
        switch (paf->where)
        {
        case TO_AFFECTS:
            BITWISE_XAND(ch->affected_by, paf->bitvector);
            break;
        case TO_IMMUNE:
            BITWISE_XAND(ch->imm_flags, paf->bitvector);
            break;
        case TO_RESIST:
            BITWISE_XAND(ch->res_flags, paf->bitvector);
            break;
        case TO_VULN:
            BITWISE_XAND(ch->vuln_flags, paf->bitvector);
            break;
        }
    }
    modify_location(ch, paf->location, paf->modifier, fAdd);
    /*
     * Check for weapon wielding.
     * Guard against recursion (for weapons with affects).
     */
    if ( !IS_NPC(ch) && ( wield = get_eq_char( ch, WEAR_WIELD ) ) != NULL
    &&   get_obj_weight(wield) > (str_app[get_curr_stat(ch,STAT_STR)].wield)
    &&   !IS_OBJ_STAT(wield,ITEM_NODISARM) && !IS_IMMORTAL(ch))
    {
	static int depth;

	if ( depth == 0 )
	{
	    depth++;
	    act( "You can no longer find the strength to wield $p.", ch, wield, NULL, TO_CHAR );
	    act( "$n can no longer find the strength to wield $p.", ch, wield, NULL, TO_ROOM );
		disarmed = TRUE;
	    obj_from_char( wield );
	    obj_to_char( wield, ch );
	    depth--;
	}
    }
    if (!IS_NPC(ch) && ( wield = get_eq_char(ch,WEAR_DUAL_WIELD)) != NULL
    && get_obj_weight(wield) > (str_app[get_curr_stat(ch,STAT_STR)].wield)
    &&   !IS_OBJ_STAT(wield,ITEM_NODISARM))
    {
	static int depth;

	if ( depth == 0 )
	{
	    depth++;
	    act( "You can no longer find the strength to wield $p.", ch, wield, NULL, TO_CHAR );
	    act( "$n can no longer find the strength to wield $p.", ch, wield, NULL, TO_ROOM );
		disarmed = TRUE;
	    obj_from_char( wield );
	    obj_to_char( wield, ch );
	    depth--;
	}
    }

	if (!IS_NPC(ch)) {
	
	/* Arms */
	if ((wield = get_eq_char(ch,WEAR_WIELD)) != NULL && ch->arms < 1)
	{
		static int depth;

		if (depth == 0)
		{
			depth++;
			act("You are no longer able to grasp $p.", ch, wield, NULL, TO_CHAR );
			act("$n is no longer able to grasp $p.", ch, wield, NULL, TO_ROOM );
			disarmed = TRUE;
			obj_from_char(wield);
			obj_to_char(wield, ch);
			depth--;
		}
	}

	if ((wield = get_eq_char(ch,WEAR_DUAL_WIELD)) != NULL && ch->arms < 2)
	{
		static int depth;

		if (depth == 0)
		{
			depth++;
			act("You are no longer able to grasp $p.", ch, wield, NULL, TO_CHAR );
			act("$n is no longer able to grasp $p.", ch, wield, NULL, TO_ROOM );
			disarmed = TRUE;
			obj_from_char(wield);
			obj_to_char(wield, ch);
			depth--;
		}
	}

	if ((wield = get_eq_char(ch,WEAR_WIELD)) != NULL && ch->arms < 2 && ch->size < SIZE_LARGE
		&& (IS_WEAPON_STAT(wield,WEAPON_TWO_HANDS)
		|| wield->value[0] == WEAPON_STAFF
		|| wield->value[0] == WEAPON_POLEARM
		|| wield->value[0] == WEAPON_SPEAR) )
	{
		static int depth;

		if (depth == 0)
		{
			depth++;
			act("You are no longer able to grasp $p.", ch, wield, NULL, TO_CHAR );
			act("$n is no longer able to grasp $p.", ch, wield, NULL, TO_ROOM );
			disarmed = TRUE;
			obj_from_char(wield);
			obj_to_char(wield, ch);
			depth--;
		}
	}
	}

    if ((wield = get_eq_char(ch,WEAR_DUAL_WIELD)) != NULL)
    {
	if (disarmed && get_eq_char(ch,WEAR_WIELD) == NULL)
	{
		unequip_char(ch,wield,FALSE);
		act("You swap $p into your primary hand.",ch,wield,0,TO_CHAR);
		act("$n swaps $p into $s primary hand.",ch,wield,0,TO_ROOM);
		equip_char(ch,wield,WEAR_WIELD,FALSE);
	}
    }

    return;
}


/* find an effect in an affect list */
AFFECT_DATA  *affect_find(AFFECT_DATA *paf, int sn)
{
    AFFECT_DATA *paf_find;

    for ( paf_find = paf; paf_find != NULL; paf_find = paf_find->next )
    {
        if ( paf_find->type == sn )
	return paf_find;
    }

    return NULL;
}

OBJ_AFFECT_DATA *affect_find_obj(OBJ_AFFECT_DATA *paf, int sn)
{
	OBJ_AFFECT_DATA *paf_find;

	for (paf_find = paf; paf_find != NULL; paf_find = paf_find->next ) {
		if (paf_find->type == sn )
			return paf_find;
	}
	return NULL;
}

ROOM_AFFECT_DATA  *affect_find_room(ROOM_AFFECT_DATA *paf, int sn)
{
    ROOM_AFFECT_DATA *paf_find;

    for ( paf_find = paf; paf_find != NULL; paf_find = paf_find->next )
    {
        if ( paf_find->type == sn )
	return paf_find;
    }

    return NULL;
}

AREA_AFFECT_DATA *affect_find_area(AREA_AFFECT_DATA *paf, int sn)
{
	AREA_AFFECT_DATA *paf_find;

	for (paf_find = paf; paf_find != NULL; paf_find = paf_find->next)
		if (paf_find->type == sn)
			return paf_find;

	return NULL;
}

/* fix object affects when removing one */
void affect_check(CHAR_DATA *ch,int where,long vector[])
{
    AFFECT_DATA *paf;

    if (where == TO_OBJECT || where == TO_WEAPON || vector == 0)
		return;

    for (paf = ch->affected; paf != NULL; paf = paf->next)
		if (paf->where == where && vector_equal(paf->bitvector, vector)) {
	    switch (where) {
	        case TO_AFFECTS:
		    BITWISE_OR(ch->affected_by,vector);
		    break;
	        case TO_IMMUNE:
		    BITWISE_OR(ch->imm_flags,vector);
		    break;
	        case TO_RESIST:
		    BITWISE_OR(ch->res_flags,vector);
		    break;
	        case TO_VULN:
		    BITWISE_OR(ch->vuln_flags,vector);
		    break;
	    }
	    return;
	}
}


/*
 * Give an affect to a char.
 */
void affect_to_char( CHAR_DATA *ch, AFFECT_DATA *paf )
{
	new_affect_to_char(ch,paf);
	return;
}

void new_affect_to_char( CHAR_DATA *ch, AFFECT_DATA *paf )
{
    AFFECT_DATA *paf_new;

    if(IS_SET(ch->imm_flags,IMM_SLEEP) && IS_SET(paf->bitvector,AFF_SLEEP) && paf->where==TO_AFFECTS)
	{
		send_to_char("You are unaffected.\n\r",ch);
		act("$n is unaffected.",ch,0,0,TO_ROOM);
		return;
    }
	
    if(IS_AFFECTED(ch,AFF_SLEEP)
		&& IS_SET(paf->bitvector,AFF_SLEEP)
		&& ch->position==POS_SLEEPING)
		return;
	
	if(is_affected(ch,gsn_indom) && paf->aftype != AFT_TIMER) 
		return send_to_char("Your body's state is maintained by the force of your will.\n\r",ch);

	// set initial duration
	paf->init_duration = paf->duration;
    paf_new = new_affect();
    *paf_new		= *paf;
    paf_new->next	= ch->affected;
    ch->affected	= paf_new;

	/* Morg - Valgrind fix */
	paf_new->valid	= TRUE;

	
    affect_modify( ch, paf_new, TRUE );
    return;
}


/*
 * Remove an affect from a char.
 */
void affect_remove( CHAR_DATA *ch, AFFECT_DATA *paf )
{
    int where;
    long vector[MAX_BITVECTOR];
	
    if ( ch->affected == NULL )
		return bug( "Affect_remove: no affect on %s.", ch->name );

    if(paf->end_fun != NULL)
		(*paf->end_fun) (ch, paf);

    affect_modify( ch, paf, FALSE );
    where = paf->where;
    copy_vector(vector, paf->bitvector);

    if ( paf == ch->affected )
		ch->affected	= paf->next;
    else
    {
		AFFECT_DATA *prev;

		for ( prev = ch->affected; prev != NULL; prev = prev->next )
		{
	    	if ( prev->next == paf )
	    	{
				prev->next = paf->next;
				break;
	    	}
		}

		if ( prev == NULL )
			return bug( "Affect_remove: cannot find paf.", 0 );
    }

    free_affect(paf);

    affect_check(ch,where,vector);
    return;
}

/*
 * Strip all affects of a given sn.
 */
void affect_strip( CHAR_DATA *ch, int sn )
{
    AFFECT_DATA *paf;
    AFFECT_DATA *paf_next;

    for ( paf = ch->affected; paf != NULL; paf = paf_next )
    {
	paf_next = paf->next;
	if ( paf->type == sn ) {
	    affect_remove( ch, paf );
	}
    }

    return;
}


/*
 * Return true if a char is affected by a spell.
 */
bool is_affected( CHAR_DATA *ch, int sn )
{
    AFFECT_DATA *paf;

	if (!ch)
		return FALSE;

    for ( paf = ch->affected; paf != NULL; paf = paf->next )
    {
	if ( paf->type == sn )
	    return TRUE;
    }

    return FALSE;
}
/*
bool is_affected_obj( OBJ_DATA *obj, int sn )
{
    AFFECT_DATA *paf;

    for ( paf = obj->affected; paf != NULL; paf = paf->next )
    {
	if ( paf->type == sn )
	    return TRUE;
    }

    return FALSE;
}
*/


/*
 * Add or enhance an affect.
 */

void affect_join( CHAR_DATA *ch, AFFECT_DATA *paf )
{
	paf->owner = NULL;
	paf->end_fun = NULL;
	new_affect_join(ch,paf);
	return;
}
void new_affect_join( CHAR_DATA *ch, AFFECT_DATA *paf )
{
    AFFECT_DATA *paf_old;

	for ( paf_old = ch->affected; paf_old != NULL; paf_old = paf_old->next )
    {
	if ( paf_old->type == paf->type )
	{
	    paf->level = (paf->level + paf_old->level) / 2;
	    paf->duration += paf_old->duration;
	    paf->modifier += paf_old->modifier;
	    affect_remove( ch, paf_old );
	    break;
	}
    }

    new_affect_to_char( ch, paf );
    return;
}



/*
 * Move a char out of a room.
 */
void char_from_room( CHAR_DATA *ch )
{
    ROOM_INDEX_DATA *prev_room = ch->in_room;
    ROOM_AFFECT_DATA *af;
	OBJ_DATA *obj;
    CHAR_DATA *mob;
    bool other = FALSE;
    AFFECT_DATA *aaf;

    if ( ch->in_room == NULL )
		return bug( "Char_from_room: NULL.", 0 );

    if ( !IS_NPC(ch) )
		--ch->in_room->area->nplayer;

	for (obj = ch->carrying; obj; obj = obj->next_content)
		if (obj->wear_loc != WEAR_NONE)
			if ((obj->item_type == ITEM_LIGHT || IS_OBJ_STAT(obj,ITEM_GLOW))
				&& ch->in_room)
				ch->in_room->light = UMAX(0,ch->in_room->light - 3);

	if(IS_SET(ch->progtypes,MPROG_MOVE))
    {
	for(mob = ch->in_room->people; mob; mob = mob->next_in_room)
		if(mob != ch && IS_SET(mob->progtypes,MPROG_MOVE))
			other = TRUE;
	if(!other)
		ch->in_room->move_progs = FALSE;
    }
    if ( ch == ch->in_room->people )
    {
	ch->in_room->people = ch->next_in_room;
    }
    else
    {
	CHAR_DATA *prev;

	for ( prev = ch->in_room->people; prev; prev = prev->next_in_room )
	{
	    if ( prev->next_in_room == ch )
	    {
		prev->next_in_room = ch->next_in_room;
		break;
	    }
	}
/*	if ( prev == NULL )
	    bug( "Char_from_room: ch not found.", 0 ); */
    }

    ch->in_room      = NULL;
    ch->next_in_room = NULL;
    ch->on 	     = NULL;  /* sanity check! */
   if(is_affected_room(prev_room,gsn_gravity_well))
    {
        for ( af = prev_room->affected; af != NULL; af = af->next )
             if (af->type == gsn_gravity_well)
                  break;
        if(ch == af->owner)
	{
                gravity_well_explode(prev_room, af);
	}	
    }
	if (!is_affected(ch,gsn_pull) && (check_entwine(ch,1) || check_entwine(ch,2)))
	{
		for (aaf = ch->affected; aaf != NULL; aaf = aaf->next)
		{
			if (aaf->type == gsn_entwine && (aaf->modifier == 1 || aaf->location == APPLY_DEX))
				break;
		}
		do_uncoil(aaf->owner,"automagic");
	}
	else if (!is_affected(ch,gsn_pull) && check_entwine(ch,0))
	{
		do_uncoil(ch,"automagic");
	}
    return;
}



/*
 * Move a char into a room.
 */
void char_to_room( CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex )
{
	OBJ_DATA *obj;

    if ( pRoomIndex == NULL ) {
		ROOM_INDEX_DATA *room;

		bug( "Char_to_room: NULL.", 0 );
	
		if ((room = get_room_index(ROOM_VNUM_TEMPLE)) != NULL)
	    	char_to_room(ch,room);
	
		return;
    }

    ch->in_room		= pRoomIndex;
    ch->next_in_room	= pRoomIndex->people;
    pRoomIndex->people	= ch;

    if ( !IS_NPC(ch) )
    {
		/* this is crashing us */
		if (ch->in_room->area->empty)
	    		ch->in_room->area->empty = FALSE;
		++ch->in_room->area->nplayer;
    }
    else
    {
	if (ch->home_room == NULL)
		ch->home_room = pRoomIndex;
    }

	for (obj = ch->carrying; obj; obj = obj->next_content)
		if (obj->wear_loc != WEAR_NONE)
			if ((obj->item_type == ITEM_LIGHT || IS_OBJ_STAT(obj,ITEM_GLOW))
				&& ch->in_room)
				ch->in_room->light += 3;

    if(IS_SET(ch->progtypes,MPROG_MOVE))
	ch->in_room->move_progs = TRUE;
	
    if (IS_AFFECTED(ch,AFF_PLAGUE)) {
        AFFECT_DATA *af;
		
		for ( af = ch->affected; af; af = af->next)
            if (af->tick_fun == plague_tick) {
				break;
			}

        if (!af) {
            REMOVE_BIT(ch->affected_by,AFF_PLAGUE);
            return;
        }

    }

    if ((!IS_ZERO_VECTOR(ch->in_room->affected_by) || ch->in_room->has_rune) && IS_IMMORTAL(ch))
		do_raffects(ch,"");

    return;
}



/*
 * Give an obj to a char.
 */
void obj_to_char( OBJ_DATA *obj, CHAR_DATA *ch )
{
    obj->next_content	 = ch->carrying;
    ch->carrying	 = obj;
    obj->carried_by	 = ch;
    obj->in_room	 = NULL;
    obj->in_obj		 = NULL;
    ch->carry_number	+= get_obj_number( obj );
    ch->carry_weight	+= get_obj_weight( obj );
}



/*
 * Take an obj from its character.
 */
void obj_from_char( OBJ_DATA *obj )
{
    CHAR_DATA *ch;

    if ( ( ch = obj->carried_by ) == NULL )
    {
	bug( "Obj_from_char: null ch.", 0 );
	return;
    }

    if ( obj->wear_loc != WEAR_NONE )
	unequip_char( ch, obj,TRUE);

    if ( ch->carrying == obj )
    {
	ch->carrying = obj->next_content;
    }
    else
    {
	OBJ_DATA *prev;

	for ( prev = ch->carrying; prev != NULL; prev = prev->next_content )
	{
	    if ( prev->next_content == obj )
	    {
		prev->next_content = obj->next_content;
		break;
	    }
	}

	if ( prev == NULL )
	    bug( "Obj_from_char: obj not in list.", 0 );
    }

    obj->carried_by	 = NULL;
    obj->next_content	 = NULL;
    ch->carry_number	-= get_obj_number( obj );
    ch->carry_weight	-= get_obj_weight( obj );
    return;
}



/*
 * Find the ac value of an obj, including position effect.
 */
int apply_ac( OBJ_DATA *obj, int iWear, int type )
{
    if ( obj->item_type != ITEM_ARMOR )
	return 0;

    switch ( iWear )
    {
    case WEAR_STRAPPED: return     obj->value[type];
    case WEAR_BODY:	return 3 * obj->value[type];
    case WEAR_HEAD:	return     obj->value[type];
    case WEAR_LEGS:	return     obj->value[type];
    case WEAR_FEET:	return     obj->value[type];
    case WEAR_HANDS:	return     obj->value[type];
    case WEAR_ARMS:	return     obj->value[type];
    case WEAR_SHIELD:	return     obj->value[type];
    case WEAR_NECK_1:	return     obj->value[type];
    case WEAR_NECK_2:	return     obj->value[type];
    case WEAR_ABOUT:	return 2 * obj->value[type];
    case WEAR_WAIST:	return     obj->value[type];
    case WEAR_WRIST_L:	return     obj->value[type];
    case WEAR_WRIST_R:	return     obj->value[type];
    case WEAR_HOLD:	return     obj->value[type];
    case WEAR_FINGER_L: return     obj->value[type];
    case WEAR_FINGER_R: return     obj->value[type];
    }

    return 0;
}



/*
 * Find a piece of eq on a character.
 */
OBJ_DATA *get_eq_char( CHAR_DATA *ch, int iWear )
{
    OBJ_DATA *obj;

    if (ch == NULL)
	return NULL;

    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if ( obj->wear_loc == iWear )
	    return obj;
    }

    return NULL;
}

bool is_worn(OBJ_DATA *obj)
{
    if(obj->wear_loc > WEAR_NONE)
		return TRUE;
    return FALSE;
}

/*
 * Equip a char with an obj.
 */
void equip_char( CHAR_DATA *ch, OBJ_DATA *obj, int iWear, bool show )
{
	char buf[MSL];
    AFFECT_DATA *paf, *fmaf;
	OBJ_APPLY_DATA *app;
    int i;
    bool status;

    status = FALSE;
    if ( iWear != WEAR_COSMETIC && get_eq_char( ch, iWear ) != NULL )
    {
	sprintf(buf,"Equip_char: already equipped (%d) -- %s -- %s.",iWear,ch->name, ch->in_room->area->file_name);
	bug(buf, 0);
	return;
    }
    int palign = ch->alignment, pethos = get_ethos(ch);
    if(!IS_NPC(ch) && is_affected(ch, gsn_false_motives))
    {
	AFFECT_DATA *pal;
	for(pal = ch->affected; pal; pal = pal->next)
		if(pal->type == gsn_false_motives && pal->location == APPLY_ALIGNMENT)
			palign -= pal->modifier;
	for(pal = ch->affected; pal; pal = pal->next)
		if(pal->type == gsn_false_motives && pal->location == APPLY_ETHOS)
			pethos -= pal->modifier;
    }
    if ( !IS_NPC(ch) && ( IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)    && palign < 0    )
    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)    && palign > 0    )
    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_NEUTRAL) && palign == 0 	)
    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_LAWFUL)  && pethos > 0	)
    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_NEUT)    && pethos == 0	)
    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_CHAOTIC) && pethos < 0	)
    || 	is_restricted(ch,obj))
    {
	/*
	 * Thanks to Morgenes for the bug fix here!
	 */
	act( "$p rejects you, sliding from your hands despite your best efforts.", ch, obj, NULL, TO_CHAR );
	act( "$n drops $p onto the ground.",  ch, obj, NULL, TO_ROOM );
	obj_from_char( obj );
	obj_to_room( obj, ch->in_room );
	return;
    }

	obj->wear_loc = iWear;
	if (show && IS_SET(obj->progtypes,IPROG_WEAR))
		(obj->pIndexData->iprogs->wear_prog) (obj,ch);
	if(show && TRAPS_IEVENT(obj,TRAP_IWEAR))
		CALL_IEVENT(obj,TRAP_IWEAR,ch,obj);

	if(iWear == WEAR_COSMETIC)
		return;
	BITWISE_OR(ch->imm_flags, obj->imm_flags);
	BITWISE_OR(ch->res_flags, obj->res_flags);
	BITWISE_OR(ch->vuln_flags, obj->vuln_flags);
	
    for (i = 0; i < 4; i++)
    	ch->armor[i]      	+= apply_ac( obj, iWear,i );

	for ( app = obj->apply; app ; app = app->next)
		modify_location( ch, app->location, app->modifier, TRUE);
	
	for ( paf = obj->charaffs; paf != NULL; paf = paf->next )
		affect_to_char(ch,paf);

	if ((obj->item_type == ITEM_LIGHT || IS_OBJ_STAT(obj,ITEM_GLOW))
		&& ch->in_room)
		ch->in_room->light += 3;

    if(show && obj->pIndexData->wear_echo[0]!=NULL)
        act(palloc_string(obj->pIndexData->wear_echo[0]),ch,obj,0,TO_CHAR);
    if(show && obj->pIndexData->wear_echo[1]!=NULL)
        act(palloc_string(obj->pIndexData->wear_echo[1]),ch,obj,0,TO_ROOM);
	
    return;
}



/*
 * Unequip a char with an obj.
 */
void unequip_char( CHAR_DATA *ch, OBJ_DATA *obj, bool show )
{
    AFFECT_DATA *paf = NULL;
	OBJ_APPLY_DATA *app;
    int i, wearloc = obj->wear_loc;

    if ( obj->wear_loc == WEAR_NONE )
    {
	bug( "Unequip_char: already unequipped.", 0 );
	return;
    }

	if ( obj->wear_loc == WEAR_WIELD && check_entwine(obj->carried_by,0))
	{
		do_uncoil(obj->carried_by,"automagic");
	}
	
	obj->wear_loc = -1;
	if (show && IS_SET(obj->progtypes,IPROG_REMOVE))
        (obj->pIndexData->iprogs->remove_prog) (obj,ch);
    if(show && TRAPS_IEVENT(obj,TRAP_IREMOVE))
		CALL_IEVENT(obj,TRAP_IREMOVE,ch,obj);

	if(obj->wear_loc == WEAR_COSMETIC)
		return;
	
    for (i = 0; i < 4; i++)
    	ch->armor[i]	-= apply_ac( obj, wearloc, i );
	BITWISE_XOR(ch->imm_flags, obj->imm_flags);
	BITWISE_XOR(ch->res_flags, obj->res_flags);
	BITWISE_XOR(ch->vuln_flags, obj->vuln_flags);
	
	for (app = obj->apply; app; app = app->next)
		modify_location(ch, app->location, app->modifier, FALSE);
	
	for ( paf = obj->charaffs; paf != NULL; paf = paf->next )
		affect_strip(ch, paf->type);

	if ((obj->item_type == ITEM_LIGHT || IS_OBJ_STAT(obj, ITEM_GLOW))
		&& ch->in_room)
		ch->in_room->light = UMAX(0, ch->in_room->light - 3);

    if(show && obj->pIndexData->remove_echo[0]!=NULL)
        act(palloc_string(obj->pIndexData->remove_echo[0]),ch,obj,0,TO_CHAR);
    if(show && obj->pIndexData->remove_echo[1]!=NULL)
        act(palloc_string(obj->pIndexData->remove_echo[1]),ch,obj,0,TO_ROOM);

    return;
}



/*
 * Count occurrences of an obj in a list.
 */
int count_obj_list( OBJ_INDEX_DATA *pObjIndex, OBJ_DATA *list )
{
    OBJ_DATA *obj;
    int nMatch;

    nMatch = 0;
    for ( obj = list; obj != NULL; obj = obj->next_content )
    {
	if ( obj->pIndexData == pObjIndex )
	    nMatch++;
    }

    return nMatch;
}



/*
 * Move an obj out of a room.
 */
void obj_from_room( OBJ_DATA *obj )
{
    ROOM_INDEX_DATA *in_room;
    CHAR_DATA *ch;

    if ((in_room = obj->in_room) == NULL)
		return bug( "obj_from_room: NULL.", 0 );

    if(obj->item_type == ITEM_CAMPFIRE)
	in_room->light = UMAX(in_room->light - obj->value[0], 0);

    for (ch = in_room->people; ch != NULL; ch = ch->next_in_room)
		if (ch->on == obj)
	    	ch->on = NULL;

    if ( obj == in_room->contents ) {
		in_room->contents = obj->next_content;
    } else {
		OBJ_DATA *prev;

		for ( prev = in_room->contents; prev; prev = prev->next_content ) {
		    if ( prev->next_content == obj ) {
				prev->next_content = obj->next_content;
				break;
		    }
		}

		if ( prev == NULL )
	    	return bug( "Obj_from_room: obj not found.", 0 );
    }
	
    obj->in_room      = NULL;
    obj->next_content = NULL;
    return;
}



/*
 * Move an obj into a room.
 */
void obj_to_room( OBJ_DATA *obj, ROOM_INDEX_DATA *pRoomIndex )
{
    if(pRoomIndex==NULL || obj==NULL)
		return;
    if(obj->item_type == ITEM_CAMPFIRE)
	pRoomIndex->light += obj->value[0];

    obj->next_content		= pRoomIndex->contents;
    pRoomIndex->contents	= obj;
    obj->in_room		= pRoomIndex;
    obj->carried_by		= NULL;
    obj->in_obj			= NULL;
    return;
}



/*
 * Move an object into an object.
 */
void obj_to_obj( OBJ_DATA *obj, OBJ_DATA *obj_to )
{
    if(obj==NULL || obj_to==NULL || obj == obj_to)
		return;

	obj->next_content		= obj_to->contains;
    obj_to->contains		= obj;
    obj->in_obj			= obj_to;
    obj->in_room		= NULL;
    obj->carried_by		= NULL;

	if (obj_to->pIndexData->vnum == OBJ_VNUM_PIT)
        obj->cost = 0;

    for ( ; obj_to != NULL; obj_to = obj_to->in_obj )
    {
		if ( obj_to->carried_by != NULL )
		{
	    	obj_to->carried_by->carry_weight += get_obj_weight( obj );
		}
    }

    return;
}



/*
 * Move an object out of an object.
 */
void obj_from_obj( OBJ_DATA *obj )
{
    OBJ_DATA *obj_from;

    if ( ( obj_from = obj->in_obj ) == NULL )
    {
	bug( "Obj_from_obj: null obj_from.", 0 );
	return;
    }
    if ( obj == obj_from->contains )
    {
	obj_from->contains = obj->next_content;
    }
    else
    {
	OBJ_DATA *prev;

	for ( prev = obj_from->contains; prev; prev = prev->next_content )
	{
	    if ( prev->next_content == obj )
	    {
		prev->next_content = obj->next_content;
		break;
	    }
	}

	if ( prev == NULL )
	{
	    bug( "Obj_from_obj: obj not found.", 0 );
	    return;
	}
    }

    obj->next_content = NULL;
    obj->in_obj       = NULL;

    for ( ; obj_from != NULL; obj_from = obj_from->in_obj )
    {
	if ( obj_from->carried_by != NULL )
	{
	    obj_from->carried_by->carry_weight -= get_obj_weight( obj );
	}
    }

    return;
}



/*
 * Extract an obj from the world.
 */
void extract_obj( OBJ_DATA *obj )
{
    OBJ_DATA *obj_content;
    OBJ_DATA *obj_next;

    if ( obj->in_room != NULL )
	obj_from_room( obj );
    else if ( obj->carried_by != NULL )
	obj_from_char( obj );
    else if ( obj->in_obj != NULL )
	obj_from_obj( obj );

    obj->pIndexData->limcount -= 1;

    for ( obj_content = obj->contains; obj_content; obj_content = obj_next )
    {
	obj_next = obj_content->next_content;
	extract_obj( obj_content );
    }

    if ( object_list == obj )
    {
	object_list = obj->next;
    }
    else
    {
	OBJ_DATA *prev;

	for ( prev = object_list; prev != NULL; prev = prev->next )
	{
	    if ( prev->next == obj )
	    {
		prev->next = obj->next;
		break;
	    }
	}

	if ( prev == NULL )
	{
	    bug( "Extract_obj: obj %d not found.", obj->pIndexData->vnum );
	    return;
	}
    }


    free_obj(obj);
    return;
}

void delay_extract( CHAR_DATA *ch)
{
	extract_char(ch, TRUE);

	return;
}


/*
 * Extract a char from the world.
 */
void extract_char( CHAR_DATA *ch, bool fPull )
{
    CHAR_DATA *wch;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    CHAR_DATA *tch;
    ROOM_INDEX_DATA *room;
    ROOM_AFFECT_DATA *raf;
    AFFECT_DATA *af;
    extern long total_wealth;

    if ( ch->in_room == NULL )
    {
	char buf[MSL], vn[50];
	if(IS_NPC(ch))
		sprintf(vn,"%d",ch->pIndexData->vnum);
	sprintf(buf,"Extract_char: in_room is NULL.  %s%s.",
		IS_NPC(ch) ? "Vnum is " : "Name is ", IS_NPC(ch) ? vn : ch->name);
	bug(buf,0);
    }
	/* remove all tracking */
    for (tch = char_list; tch != NULL; tch = tch->next)
    {
		if (tch->last_fought == ch)
			tch->last_fought = NULL;
		if (!IS_NPC(ch) && !IS_NPC(tch) && tch->last_fight_name == ch->true_name)
			tch->last_fight_name = NULL;
		if (!IS_NPC(tch) && !IS_NPC(ch) && tch->pcdata->trusting == ch)
			tch->pcdata->trusting = NULL;
		for(af = tch->affected; af; af = af->next)
			if(af->owner == ch)
				af->owner = NULL;
    }

    nuke_pets(ch);
    ch->pet = NULL; /* just in case */

    if ( fPull )
		die_follower( ch );

    stop_fighting( ch, TRUE );

    for ( obj = ch->carrying; obj != NULL; obj = obj_next ) {
		obj_next = obj->next_content;
        if (!(obj->wear_loc == WEAR_BRAND
			|| IS_SET(obj->extra_flags,ITEM_FIXED)) && !IS_NPC(ch)) {
			if (!IS_NPC(ch) && (fPull))
				obj->pIndexData->limcount++;
			extract_obj( obj );
		}
    }

    char_from_room( ch );

    if(IS_NPC(ch) && CQueue::HasQueuePending(ch))
    {
	bug("Attempt at extracting mob %d while it has queue events pending.  Deleting events.",ch->pIndexData->vnum);
	CQueue::DeleteQueuedEventsInvolving(ch);
    }

    /* Death room is set in the cabal table now */
    if ( !fPull && !IS_NPC(ch))
        return char_to_room(ch,get_room_index(ROOM_VNUM_ALTAR));

    if ( IS_NPC(ch) ) {
		--ch->pIndexData->count;
		total_wealth -= ch->pIndexData->wealth;
	}

    if ( ch->desc != NULL && ch->desc->original != NULL ) {
		do_return( ch, "" );
		ch->desc = NULL;
    }

    for ( wch = char_list; wch != NULL; wch = wch->next )
		if ( wch->reply == ch )
			wch->reply = NULL;

	for (room = top_affected_room; room ; room = room->aff_next ) {
		for ( raf = room->affected; raf != NULL; raf = raf->next ) {
			if (raf->owner == ch) {
				affect_remove_room(room,raf);
			}
		}
	}

    if ( ch == char_list )
       char_list = ch->next;
    else {
		CHAR_DATA *prev;
		for ( prev = char_list; prev != NULL; prev = prev->next ) {
			if ( prev->next == ch ) {
				prev->next = ch->next;
				break;
			}
		}

		if ( prev == NULL ) 
	    	return bug( "Extract_char: char not found.", 0 );
    }

    if ( ch->desc != NULL )
		ch->desc->character = NULL;

    free_char( ch );
    return;
}



/*
 * Find a char in the room.
 */
/* SEE act_info.c for new one that handles morph
CHAR_DATA *get_char_room( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *rch;
    int number;
    int count;

    number = number_argument( argument, arg );
    count  = 0;
    if ( !str_cmp( arg, "self" ) )
	return ch;
    for ( rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room )
    {
        if ( !can_see( ch, rch ) || (arg[0] != '\0' && !is_name( arg, rch->name )))
	    continue;
	if ( ++count == number )
	    return rch;
    }

    return NULL;
}
*/



/*
 * Find a char in the world.
 */
CHAR_DATA *get_char_world( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *wch;
    int number;
    int count;
    char name[MAX_STRING_LENGTH];

    if ( ( wch = get_char_room( ch, argument ) ) != NULL )
	return wch;

    number = number_argument( argument, arg );
    count  = 0;
    for ( wch = char_list; wch != NULL ; wch = wch->next )
    {
    	if(IS_IMMORTAL(ch) && !IS_NPC(wch))
		sprintf(name,wch->true_name);
    	else
		sprintf(name,wch->name);

	if ( wch->in_room == NULL || !can_see( ch, wch ) || !is_name( arg, name ) )
	    continue;
	if (IS_NPC(wch) && wch->pIndexData->vnum == MOB_VNUM_DECOY && IS_IMMORTAL(ch))
	    continue;
	if ( ++count == number )
	    return wch;
    }

    return NULL;
}



/*
 * Find some object with a given index data.
 * Used by area-reset 'P' command.
 */
OBJ_DATA *get_obj_type( OBJ_INDEX_DATA *pObjIndex )
{
    OBJ_DATA *obj;

    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
	if ( obj->pIndexData == pObjIndex )
	    return obj;
    }

    return NULL;
}


/*
 * Find an obj in a list.
 */
OBJ_DATA *get_obj_list( CHAR_DATA *ch, char *argument, OBJ_DATA *list )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int number;
    int count;

    number = number_argument( argument, arg );
    count  = 0;
    for ( obj = list; obj != NULL; obj = obj->next_content )
    {
        if ( (can_see_obj( ch, obj ))
			 && (arg[0] == '\0' || is_name( arg, obj->name )) )
		{
	    	if ( ++count == number )
				return obj;
		}
    }

    return NULL;
}


/*
 * Find an obj in a room.
 */
OBJ_DATA *get_obj_room( OBJ_DATA *obj, ROOM_INDEX_DATA *room)
{
	OBJ_DATA *obj2;
	
	for( obj2 = room->contents; obj2 != NULL; obj2 = obj2->next_content)
	{
		if(obj->pIndexData->vnum == obj2->pIndexData->vnum)
			return obj2;
	}

	return NULL;
}

/*
 * Find an obj in player's inventory.
 */
OBJ_DATA *get_obj_carry( CHAR_DATA *ch, char *argument, CHAR_DATA *viewer )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int number;
    int count;

    number = number_argument( argument, arg );
    count  = 0;
    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if ( obj->wear_loc == WEAR_NONE
	&&   (can_see_obj( viewer, obj ) )
        &&   (arg[0] == '\0' || is_name( arg, obj->name )) )
	{
            if ( ++count == number )
		return obj;
	}
    }

    return NULL;
}



/*
 * Find an obj in player's equipment.
 */
OBJ_DATA *get_obj_wear( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int number;
    int count;

    number = number_argument( argument, arg );
    count  = 0;
    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if ( obj->wear_loc != WEAR_NONE
	&&   can_see_obj( ch, obj )
        &&   (arg[0] == '\0' || is_name( arg, obj->name )) )
	{
	    if ( ++count == number )
		return obj;
	}
    }

    return NULL;
}



/*
 * Find an obj in the room or in inventory.
 */
OBJ_DATA *get_obj_here( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;

	if (!str_cmp(argument,""))
		return NULL;

    obj = get_obj_list( ch, argument, ch->in_room->contents );
    if ( obj != NULL )
	return obj;

    if ( ( obj = get_obj_carry( ch, argument, ch ) ) != NULL )
	return obj;

    if ( ( obj = get_obj_wear( ch, argument ) ) != NULL )
	return obj;

    return NULL;
}



/*
 * Find an obj in the world.
 */
OBJ_DATA *get_obj_world( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int number;
    int count;

    if ( ( obj = get_obj_here( ch, argument ) ) != NULL )
	return obj;

    number = number_argument( argument, arg );
    count  = 0;
    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
	if ( can_see_obj( ch, obj ) && is_name( arg, obj->name ) )
	{
	    if ( ++count == number )
		return obj;
	}
    }

    return NULL;
}

/* deduct cost from a character */

void deduct_cost(CHAR_DATA *ch, int cost)
{
    int gold = 0;

    gold = UMIN(ch->gold,cost);

    ch->gold -= gold;

    if (ch->gold < 0)
    {
	bug("deduct costs: gold %d < 0",ch->gold);
	ch->gold = 0;
    }
}
/*
 * Create a 'money' obj.
 */
OBJ_DATA *create_money( int gold)
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj=NULL;

    if ( gold <= 0)
    {
	bug( "Create_money: zero or negative money.",gold);
	gold = UMAX(1,gold);
    }

    else if (gold == 1)
    {
	obj = create_object( get_obj_index( OBJ_VNUM_GOLD_ONE), 0 );
    }
    else if (gold > 1)
    {
        obj = create_object( get_obj_index( OBJ_VNUM_GOLD_SOME ), 0 );
        sprintf( buf, obj->short_descr, gold );
        free_pstring( obj->short_descr );
        obj->short_descr        = palloc_string( buf );
        obj->value[1]           = gold;
        obj->cost               = gold;
	obj->weight		= gold/1000;
    }

    return obj;
}



/*
 * Return # of objects which an object counts as.
 * Thanks to Tony Chamberlain for the correct recursive code here.
 */
int get_obj_number( OBJ_DATA *obj )
{
    int number;

    if (obj->wear_loc == WEAR_BRAND || obj->item_type == ITEM_MONEY
    ||  obj->item_type == ITEM_GEM || obj->item_type == ITEM_JEWELRY)
        number = 0;
    else
        number = 1;

    return number;
}


/*
 * Return weight of an object, including weight of contents.
 */
int get_obj_weight( OBJ_DATA *obj )
{
    int weight;
    OBJ_DATA *tobj;

    weight = obj->weight;
    for ( tobj = obj->contains; tobj != NULL; tobj = tobj->next_content )
	weight += get_obj_weight( tobj );

    return weight;
}

int get_true_weight(OBJ_DATA *obj)
{
    int weight;

    weight = obj->weight;
    for ( obj = obj->contains; obj != NULL; obj = obj->next_content )
        weight += get_obj_weight( obj );

    return weight;
}

int get_contents_weight(OBJ_DATA *obj)
{
	OBJ_DATA *stuff;
	int weight = 0;

	for ( stuff = obj->contains; stuff != NULL; stuff = stuff->next_content )
		weight += get_obj_weight( stuff );

	return weight;
}

int get_contents_number(OBJ_DATA *obj)
{
	OBJ_DATA *stuff;
	int number = 0;

	for ( stuff = obj->contains; stuff != NULL; stuff = stuff->next_content )
		number++;

	return number;
}

/*
 * True if room is dark.
 */
bool room_is_dark( ROOM_INDEX_DATA *pRoomIndex )
{
	
	if (pRoomIndex->light > 2)
		return FALSE;
	
    if ( IS_SET(pRoomIndex->room_flags, ROOM_DARK) )
		return TRUE;

    if ( pRoomIndex->sector_type == SECT_INSIDE
    ||   pRoomIndex->sector_type == SECT_CITY )
		return FALSE;

    if ( sun == SUN_SET	|| sun == SUN_DARK )
		return TRUE;

    	return FALSE;
}


bool is_room_owner(CHAR_DATA *ch, ROOM_INDEX_DATA *room)
{
    if (room->owner == NULL || room->owner[0] == '\0')
	return FALSE;

    return is_name(ch->name,room->owner);
}

/*
 * True if room is private.
 */
bool room_is_private( ROOM_INDEX_DATA *pRoomIndex )
{
    CHAR_DATA *rch;
    int count;

    return FALSE;

    if (pRoomIndex->owner != NULL && pRoomIndex->owner[0] != '\0')
	return TRUE;

    count = 0;
    for ( rch = pRoomIndex->people; rch != NULL; rch = rch->next_in_room )
	count++;

    if ( IS_SET(pRoomIndex->room_flags, ROOM_PRIVATE)  && count >= 2 )
	return TRUE;

    if ( IS_SET(pRoomIndex->room_flags, ROOM_SOLITARY) && count >= 1 )
	return TRUE;

    if ( IS_SET(pRoomIndex->room_flags, ROOM_IMP_ONLY) )
	return TRUE;

    return FALSE;
}

/* visibility on a room -- for entering and exits */
bool can_see_room( CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex )
{
    if (IS_SET(pRoomIndex->room_flags, ROOM_IMP_ONLY)
    &&  get_trust(ch) < MAX_LEVEL)
	return FALSE;

    if (IS_SET(pRoomIndex->room_flags, ROOM_GODS_ONLY)
    &&  !IS_IMMORTAL(ch))
	return FALSE;

    if (IS_SET(pRoomIndex->room_flags, ROOM_HEROES_ONLY)
    &&  !IS_IMMORTAL(ch))
	return FALSE;

    if (IS_SET(pRoomIndex->room_flags,ROOM_NEWBIES_ONLY)
    &&  ch->level > 10 && !IS_IMMORTAL(ch) && (!IS_NPC(ch) || ch->pIndexData->vnum != ACADEMY_PET))
	return FALSE;

    return TRUE;
}



/*
 * True if char can see victim.
 */
bool can_see( CHAR_DATA *ch, CHAR_DATA *victim )
{
	AREA_AFFECT_DATA *paf;
	AFFECT_DATA *af;

	if (IS_NPC(ch) && IS_SET(ch->act,ACT_DETECT_SPECIAL))
		return TRUE;

	if ( ch == victim )
		return TRUE;

	if (!IS_IMMORTAL(ch) && IS_AFFECTED(victim, AFF_NOSHOW))
		return FALSE;
	
	if(IS_CABAL_GUARD(ch) && !ch->desc)
		return TRUE;

	if ( get_trust(ch) < victim->invis_level)
		return FALSE;


    if (get_trust(ch) < victim->incog_level && ch->in_room != victim->in_room)
	return FALSE;

	if (get_trust(ch) == LEVEL_HERO && victim->invis_level == LEVEL_HERO && !IS_HEROIMM(ch))
	return FALSE;

    if ( (!IS_NPC(ch) && IS_SET(ch->act, PLR_HOLYLIGHT))
    ||   (IS_NPC(ch) && IS_IMMORTAL(ch)))
	return TRUE;
    if(IS_NPC(ch) && victim->invis_level >= LEVEL_HERO)
	return FALSE;

    if (!IS_NPC(ch)
    && ch->pcdata->death_status == HAS_DIED)
		return TRUE;
/* Make sure cabal guardians can always see */
	if (IS_CABAL_GUARD(ch))
	return TRUE;
	
    if ( IS_AFFECTED(ch, AFF_BLIND) )
	return FALSE;

	if (is_affected(victim,gsn_ultradiffusion))
		return FALSE;

	if (is_affected_area(ch->in_room->area,gsn_whiteout) &&
		IS_OUTSIDE(ch)) {
		for (paf = ch->in_room->area->affected;paf;paf = paf->next) {
			if (paf->type == gsn_whiteout)  break;
		}
		if (paf->owner != ch)
			return FALSE;
	}

	if (is_affected(victim,gsn_watermeld) && !is_affected(ch,gsn_hydroperception))
		return FALSE;

	if (is_affected(victim,gsn_earthfade))
		return FALSE;

    if ( IS_AFFECTED(victim, AFF_INVISIBLE)
    &&   !IS_AFFECTED(ch, AFF_DETECT_INVIS)
	&& (!(is_affected(ch,gsn_hydroperception)
		&& (ch->in_room->sector_type == SECT_WATER
		|| ch->in_room->sector_type == SECT_UNDERWATER
		|| (ch->in_room->sector_type != SECT_INSIDE
		&& victim->in_room->area->sky >= SKY_DRIZZLE))
		&& ch->in_room == victim->in_room ))
	&& (!(is_affected(ch,gsn_sense_disturbance)
		&& ch->in_room->sector_type != SECT_UNDERWATER
		&& victim->in_room->sector_type != SECT_UNDERWATER))
	&& (!(is_affected(ch,gsn_sensevibrations)
		&& (victim->in_room->sector_type != SECT_UNDERWATER)
		&& (victim->in_room->sector_type != SECT_AIR)
		&& (ch->in_room->sector_type != SECT_UNDERWATER)
		&& (ch->in_room->sector_type != SECT_AIR)
		&& !(IS_AFFECTED(victim, AFF_SNEAK))
		&& !(IS_AFFECTED(victim, AFF_FLYING)) ) ))
	return FALSE;
	
    if ( IS_AFFECTED(victim, AFF_HIDE)
    &&   !IS_AFFECTED(ch, AFF_DETECT_HIDDEN)
    &&   victim->fighting == NULL
	&&	!(is_affected(ch,gsn_darksight) && (af = affect_find(ch->affected,
		gsn_darksight)) && af->aftype == AFT_SKILL
		&& room_is_dark(victim->in_room))
	&& (!(is_affected(ch,gsn_hydroperception) && (ch->in_room->sector_type == SECT_WATER || ch->in_room->sector_type == SECT_UNDERWATER || (ch->in_room->sector_type != SECT_INSIDE && victim->in_room->area->sky >= SKY_DRIZZLE)) && ch->in_room == victim->in_room ))
	&& (!(is_affected(ch,gsn_sensevibrations)
		&& (victim->in_room->sector_type != SECT_UNDERWATER)
			&& (victim->in_room->sector_type != SECT_AIR)
		&& (ch->in_room->sector_type != SECT_UNDERWATER)
			&& (ch->in_room->sector_type != SECT_AIR)
		&& !(IS_AFFECTED(victim, AFF_SNEAK))
		&& !(IS_AFFECTED(victim, AFF_FLYING))) ))
	return FALSE;

    if ( IS_AFFECTED(victim, AFF_CAMOUFLAGE) && !IS_AFFECTED(ch,AFF_DETECT_CAMO))
	return FALSE;

    if (IS_NPC(ch))
	return TRUE;

    if ( room_is_dark( ch->in_room ) && !IS_AFFECTED(ch,AFF_DARK_VISION))
	return FALSE;


    return TRUE;
}



/*
 * True if char can see obj.
 */
bool can_see_obj( CHAR_DATA *ch, OBJ_DATA *obj )
{
	AREA_AFFECT_DATA *paf;
	OBJ_AFFECT_DATA *oaf;
	OBJ_DATA *tat;
	if(!ch->in_room)
	{
		bug("ERROR IN CANSEEOBJ!",0);
		return FALSE;
	}
	if (is_affected_obj(obj, gsn_stash))
	{
		for (oaf = obj->affected; oaf != NULL; oaf = oaf->next)	
			if(oaf->type == gsn_stash)
				break;

		if(oaf->owner != ch && !IS_IMMORTAL(ch))
			return FALSE;
	}

	if (IS_OBJ_STAT(obj,ITEM_NOSHOW) && !IS_IMMORTAL(ch) && is_worn(obj) && (tat = get_eq_char(ch, WEAR_BRAND)) != obj)
		return FALSE;

	if (!IS_NPC(ch) && IS_SET(ch->act, PLR_HOLYLIGHT) )
		return TRUE;

    if (IS_SET(obj->extra_flags,ITEM_VIS_DEATH))
		return FALSE;

    if ((IS_AFFECTED( ch, AFF_BLIND ) && obj->item_type != ITEM_POTION))
		return FALSE;

	if (is_affected_area(ch->in_room->area,gsn_whiteout) &&
		IS_OUTSIDE(ch) &&
		obj->item_type != ITEM_POTION) {
		for (paf = ch->in_room->area->affected;paf;paf = paf->next) {
			if (paf->type == gsn_whiteout)	
				break;
		}
		if (paf && paf->owner != ch)
			return FALSE;
	}

    if ( obj->item_type == ITEM_LIGHT && obj->value[2] != 0 )
	return TRUE;

    if ( IS_SET(obj->extra_flags, ITEM_INVIS)
    &&   !IS_AFFECTED(ch, AFF_DETECT_INVIS) )
        return FALSE;

    if ( IS_OBJ_STAT(obj,ITEM_GLOW) || IS_AFFECTED(obj,AFF_OBJ_BURNING))
		return TRUE;

    if ( room_is_dark( ch->in_room ) && !IS_AFFECTED(ch,AFF_DARK_VISION))
		return FALSE;

    return TRUE;
}



/*
 * True if char can drop obj.
 */
bool can_drop_obj( CHAR_DATA *ch, OBJ_DATA *obj )
{
    if ( !IS_SET(obj->extra_flags, ITEM_NODROP) )
	return TRUE;

    if ( !IS_NPC(ch) && ch->level >= LEVEL_IMMORTAL )
	return TRUE;

    return FALSE;
}


/*
 * Return ascii name of an affect location.
 */
char *affect_loc_name( int location )
{
    int i;
    for(i=0; apply_locations[i].name != NULL; i++)
		if(apply_locations[i].bit == location)
			return apply_locations[i].display;
    bug( "Affect_location_name: unknown location %d.", location );
    return "(unknown)";
}



/*
 * Return ascii name of an affect bit vector.
 */
char *affect_bit_name( long vector[] )
{
    static char buf[512];

    buf[0] = '\0';
    if ( IS_SET(vector, AFF_BLIND         )) strcat( buf, " blind"         );
    if ( IS_SET(vector, AFF_INVISIBLE     )) strcat( buf, " invisible"     );
    if ( IS_SET(vector, AFF_DETECT_EVIL   )) strcat( buf, " detect_evil"   );
    if ( IS_SET(vector, AFF_DETECT_GOOD   )) strcat( buf, " detect_good"   );
    if ( IS_SET(vector, AFF_DETECT_INVIS  )) strcat( buf, " detect_invis"  );
    if ( IS_SET(vector, AFF_DETECT_MAGIC  )) strcat( buf, " detect_magic"  );
    if ( IS_SET(vector, AFF_DETECT_HIDDEN )) strcat( buf, " detect_hidden" );
    if ( IS_SET(vector, AFF_SANCTUARY     )) strcat( buf, " sanctuary"     );
    if ( IS_SET(vector, AFF_DETECT_CAMO   )) strcat( buf, " detect_camo"   );
    if ( IS_SET(vector, AFF_CURSE         )) strcat( buf, " curse"         );
    if ( IS_SET(vector, AFF_POISON        )) strcat( buf, " poison"        );
    if ( IS_SET(vector, AFF_PROTECTION    )) strcat( buf, " protection"    );
    if ( IS_SET(vector, AFF_SLEEP         )) strcat( buf, " sleep"         );
    if ( IS_SET(vector, AFF_SNEAK         )) strcat( buf, " sneak"         );
    if ( IS_SET(vector, AFF_HIDE          )) strcat( buf, " hide"          );
    if ( IS_SET(vector, AFF_CHARM         )) strcat( buf, " charm"         );
    if ( IS_SET(vector, AFF_FLYING        )) strcat( buf, " flying"        );
    if ( IS_SET(vector, AFF_WATERBREATH   )) strcat( buf, " waterbreath"   );
    if ( IS_SET(vector, AFF_PASS_DOOR     )) strcat( buf, " pass_door"     );
    if ( IS_SET(vector, AFF_BERSERK	    )) strcat( buf, " berserk"	    );
    if ( IS_SET(vector, AFF_CALM	  	  	)) strcat( buf, " calm"	    );
    if ( IS_SET(vector, AFF_HASTE	    	)) strcat( buf, " haste"	    );
    if ( IS_SET(vector, AFF_SLOW          )) strcat( buf, " slow"          );
    if ( IS_SET(vector, AFF_PLAGUE	    )) strcat( buf, " plague" 	    );
    if ( IS_SET(vector, AFF_DARK_VISION   )) strcat( buf, " dark_vision"   );
    if ( IS_SET(vector, AFF_CAMOUFLAGE    )) strcat( buf, " camouflage"    );
    if ( IS_SET(vector, AFF_PERMANENT	    )) strcat( buf, " permanent");
    if ( IS_SET(vector, AFF_RAGE	  )) strcat( buf, " rage");
	    if ( IS_SET(vector, AFF_NOSHOW))			strcat( buf, " noshow");
    return ( buf[0] != '\0' ) ? buf+1 : (char*)"none";
}

/*
 * Return ascii name of extra flags vector.
 */
char *extra_bit_name( long extra_flags[] )
{
    static char buf[512];

    buf[0] = '\0';
    if ( IS_SET(extra_flags, ITEM_GLOW )        ) strcat( buf, " glow"         );
    if ( IS_SET(extra_flags, ITEM_HUM  )        ) strcat( buf, " hum"          );
    if ( IS_SET(extra_flags, ITEM_DARK  )       ) strcat( buf, " dark"         );
    if ( IS_SET(extra_flags, ITEM_NOSHOW )      ) strcat( buf, " noshow"       );
    if ( IS_SET(extra_flags, ITEM_EVIL )        ) strcat( buf, " evil"         );
    if ( IS_SET(extra_flags, ITEM_INVIS )       ) strcat( buf, " invis"        );
    if ( IS_SET(extra_flags, ITEM_MAGIC  )      ) strcat( buf, " magic"        );
    if ( IS_SET(extra_flags, ITEM_NODROP  )     ) strcat( buf, " nodrop"       );
    if ( IS_SET(extra_flags, ITEM_BLESS   )     ) strcat( buf, " bless"        );
    if ( IS_SET(extra_flags, ITEM_ANTI_GOOD)    ) strcat( buf, " anti-good"    );
    if ( IS_SET(extra_flags, ITEM_ANTI_EVIL )   ) strcat( buf, " anti-evil"    );
    if ( IS_SET(extra_flags, ITEM_ANTI_NEUTRAL)) strcat( buf, " anti-neutral" );
    if ( IS_SET(extra_flags, ITEM_ANTI_LAWFUL )) strcat( buf, " anti-lawful"  );
    if ( IS_SET(extra_flags, ITEM_ANTI_NEUT  )  ) strcat( buf, " anti-neutrale"    );
    if ( IS_SET(extra_flags, ITEM_ANTI_CHAOTIC) ) strcat( buf, " anti-chaotic" );
    if ( IS_SET(extra_flags, ITEM_NOREMOVE    ) ) strcat( buf, " noremove"     );
    if ( IS_SET(extra_flags, ITEM_INVENTORY   ) ) strcat( buf, " inventory"    );
    if ( IS_SET(extra_flags, ITEM_NOPURGE	) ) strcat( buf, " nopurge"	);
    if ( IS_SET(extra_flags, ITEM_VIS_DEATH	) ) strcat( buf, " vis_death"	);
    if ( IS_SET(extra_flags, ITEM_ROT_DEATH	) ) strcat( buf, " rot_death"	);
    if ( IS_SET(extra_flags, ITEM_NOLOCATE	) ) strcat( buf, " no_locate"	);
    if ( IS_SET(extra_flags, ITEM_MELT_DROP	) ) strcat( buf, " melt_drop"	);
    if ( IS_SET(extra_flags, ITEM_SELL_EXTRACT )) strcat( buf, " sell_extract" );
    if ( IS_SET(extra_flags, ITEM_BURN_PROOF	) ) strcat( buf, " burn_proof"	);
    if ( IS_SET(extra_flags, ITEM_NOUNCURSE	 )) strcat( buf, " no_uncurse"	);
    if ( IS_SET(extra_flags, CORPSE_NO_ANIMATE) ) strcat( buf, " no_animate" );
    if ( IS_SET(extra_flags, ITEM_BRAND       ) ) strcat( buf, " branded"      );
    if ( IS_SET(extra_flags, ITEM_FIXED       ) ) strcat( buf, " fixed"      );
    if ( IS_SET(extra_flags, ITEM_NODISARM	) ) strcat( buf, " nodisarm"	);
	if ( IS_SET(extra_flags, ITEM_NO_SAC	) ) strcat( buf, " no_sac"		);
	return ( buf[0] != '\0' ) ? buf+1 : (char*)"none";
}

/* Return ascii name for restrict flags */
char * restrict_bit_name(long restrict_flags[])
{
    static char buf[512];
    int i;
    buf[0] = '\0';
   for(i=0; restrict_table[i].name!=NULL; i++)
	if(IS_SET(restrict_flags,restrict_table[i].bit))
	{
		strcat(buf," ");
		strcat(buf,palloc_string(restrict_table[i].name));
	}
   return (buf[0] != '\0') ? buf+1 : (char*)"none";
}


/* return ascii name of an act vector */
char *act_bit_name( long act_flags[] )
{
    static char buf[512];

    buf[0] = '\0';

    if (IS_SET(act_flags,ACT_IS_NPC))
    {
 	strcat(buf," npc");
    	if (IS_SET(act_flags, ACT_SENTINEL 	)) strcat(buf, " sentinel");
    	if (IS_SET(act_flags, ACT_SCAVENGER	)) strcat(buf, " scavenger");
	if (IS_SET(act_flags, ACT_AGGRESSIVE)	) strcat(buf, " aggressive");
	if (IS_SET(act_flags, ACT_STAY_AREA	)) strcat(buf, " stay_area");
	if (IS_SET(act_flags, ACT_WARD_MOB	)) strcat(buf, " ward_mob");
	if (IS_SET(act_flags, ACT_WIMPY	)) strcat(buf, " wimpy");
	if (IS_SET(act_flags, ACT_PET)		) strcat(buf, " pet");
	if (IS_SET(act_flags, ACT_TRAIN)	) strcat(buf, " train");
	if (IS_SET(act_flags, ACT_PRACTICE)	) strcat(buf, " practice");
	if (IS_SET(act_flags, ACT_UNDEAD)	) strcat(buf, " undead");
	if (IS_SET(act_flags, ACT_CLERIC)	) strcat(buf, " cleric");
	if (IS_SET(act_flags, ACT_MAGE	)) strcat(buf, " mage");
	if (IS_SET(act_flags, ACT_FAST_TRACK)) strcat(buf, " fast_track");
	if (IS_SET(act_flags, ACT_INTELLIGENT)) strcat(buf, " intelligent");
	if (IS_SET(act_flags, ACT_NOALIGN)	) strcat(buf, " no_align");
	if (IS_SET(act_flags, ACT_NOPURGE)	) strcat(buf, " no_purge");
	if (IS_SET(act_flags, ACT_IS_HEALER)	) strcat(buf, " healer");
	if (IS_SET(act_flags, ACT_GUILDGUARD)  ) strcat(buf, " guildguard");
	if (IS_SET(act_flags, ACT_GAIN)	) strcat(buf, " skill_train");
	if (IS_SET(act_flags, ACT_UPDATE_ALWAYS)) strcat(buf," update_always");
	if (IS_SET(act_flags, ACT_BANKER))	strcat(buf," banker");
	if (IS_SET(act_flags, ACT_INNER_GUARDIAN)) strcat(buf, " inner_guardian");
	if (IS_SET(act_flags, ACT_NOCTURNAL))	strcat(buf," noctunal");
	if (IS_SET(act_flags, ACT_DIURNAL))		strcat(buf," diurnal");
	if (IS_SET(act_flags, ACT_FASTWANDER))	strcat(buf," fastwander");
	if (IS_SET(act_flags, ACT_LAW))			strcat(buf," law");
    }
    else
    {
	strcat(buf," player");
	if (IS_SET(act_flags, PLR_AUTOABORT	)) strcat(buf, " autoabort");
	if (IS_SET(act_flags, PLR_AUTOASSIST	)) strcat(buf, " autoassist");
	if (IS_SET(act_flags, PLR_AUTOEXIT	)) strcat(buf, " autoexit");
	if (IS_SET(act_flags, PLR_AUTOLOOT	)) strcat(buf, " autoloot");
	if (IS_SET(act_flags, PLR_AUTOSAC	)) strcat(buf, " autosac");
	if (IS_SET(act_flags, PLR_AUTOGOLD	)) strcat(buf, " autogold");
	if (IS_SET(act_flags, PLR_AUTOSPLIT	)) strcat(buf, " autosplit");
	if (IS_SET(act_flags, PLR_BETRAYER	)) strcat(buf, " betrayer");
	if (IS_SET(act_flags, PLR_HOLYLIGHT	)) strcat(buf, " holy_light");
	if (IS_SET(act_flags, PLR_EMPOWERED	)) strcat(buf, " empowered");
	if (IS_SET(act_flags, PLR_NOVOID	)) strcat(buf, " no_void");
	if (IS_SET(act_flags, PLR_NOSUMMON	)) strcat(buf, " no_summon");
	if (IS_SET(act_flags, PLR_NOFOLLOW	)) strcat(buf, " no_follow");
	if (IS_SET(act_flags, PLR_MORON	)) strcat(buf, " moron");
	if (IS_SET(act_flags, PLR_FREEZE	)) strcat(buf, " frozen");
	if (IS_SET(act_flags, PLR_THIEF	)) strcat(buf, " thief");
	if (IS_SET(act_flags, PLR_KILLER	)) strcat(buf, " killer");
        if (IS_SET(act_flags, PLR_CRIMINAL   )) strcat(buf, " criminal");
        if (IS_SET(act_flags, PLR_HEROIMM   )) strcat(buf, " heroimm");
    }
    return ( buf[0] != '\0' ) ? buf+1 : (char*)"none";
}

char *comm_bit_name(long comm_flags[])
{
    static char buf[512];

    buf[0] = '\0';

    if (IS_SET(comm_flags,  COMM_QUIET		)) strcat(buf, " quiet");
    if (IS_SET(comm_flags,  COMM_DEAF		)) strcat(buf, " deaf");
    if (IS_SET(comm_flags,  COMM_NOWIZ		)) strcat(buf, " no_wiz");
    if (IS_SET(comm_flags,  COMM_NOAUCTION	)) strcat(buf, " no_auction");
    if (IS_SET(comm_flags,  COMM_NOGOSSIP	)) strcat(buf, " no_gossip");
    if (IS_SET(comm_flags,  COMM_NOQUESTION	)) strcat(buf, " no_question");
    if (IS_SET(comm_flags,  COMM_NONEWBIE	)) strcat(buf, " no_newbie");
    if (IS_SET(comm_flags,  COMM_NOQUOTE	)) strcat(buf, " no_quote");
    if (IS_SET(comm_flags,  COMM_COMPACT	)) strcat(buf, " compact");
    if (IS_SET(comm_flags,  COMM_BRIEF		)) strcat(buf, " brief");
    if (IS_SET(comm_flags,  COMM_PROMPT	)) strcat(buf, " prompt");
    if (IS_SET(comm_flags,  COMM_COMBINE	)) strcat(buf, " combine");
    if (IS_SET(comm_flags,  COMM_NOEMOTE	)) strcat(buf, " no_emote");
    if (IS_SET(comm_flags,  COMM_NOSHOUT	)) strcat(buf, " no_shout");
    if (IS_SET(comm_flags,  COMM_NOTELL	)) strcat(buf, " no_tell");
    if (IS_SET(comm_flags,  COMM_NOCHANNELS	)) strcat(buf, " no_channels");
    if (IS_SET(comm_flags,  COMM_ANSI          )) strcat(buf, " color");
    if (IS_SET(comm_flags,  COMM_SWITCHSKILLS )) strcat(buf, " switchskills");
    if (IS_SET(comm_flags,  COMM_LOTS_O_COLOR   )) strcat(buf, " lots o color!");
    return ( buf[0] != '\0' ) ? buf+1 : (char*)"none";
}

char *imm_bit_name(long imm_flags[])
{
    static char buf[512];

    buf[0] = '\0';

    if (IS_SET(imm_flags, IMM_SUMMON		)) strcat(buf, " summon");
    if (IS_SET(imm_flags, IMM_CHARM		)) strcat(buf, " charm");
    if (IS_SET(imm_flags, IMM_MAGIC		)) strcat(buf, " magic");
    if (IS_SET(imm_flags, IMM_WEAPON		)) strcat(buf, " weapon");
    if (IS_SET(imm_flags, IMM_BASH		)) strcat(buf, " blunt");
    if (IS_SET(imm_flags, IMM_PIERCE		)) strcat(buf, " piercing");
    if (IS_SET(imm_flags, IMM_SLASH		)) strcat(buf, " slashing");
    if (IS_SET(imm_flags, IMM_FIRE		)) strcat(buf, " fire");
    if (IS_SET(imm_flags, IMM_COLD		)) strcat(buf, " cold");
    if (IS_SET(imm_flags, IMM_LIGHTNING	)) strcat(buf, " lightning");
    if (IS_SET(imm_flags, IMM_ACID		)) strcat(buf, " acid");
    if (IS_SET(imm_flags, IMM_POISON		)) strcat(buf, " poison");
    if (IS_SET(imm_flags, IMM_NEGATIVE	)) strcat(buf, " negative");
    if (IS_SET(imm_flags, IMM_HOLY		)) strcat(buf, " holy");
    if (IS_SET(imm_flags, IMM_ENERGY		)) strcat(buf, " energy");
    if (IS_SET(imm_flags, IMM_MENTAL		)) strcat(buf, " mental");
    if (IS_SET(imm_flags, IMM_DISEASE	)) strcat(buf, " disease");
    if (IS_SET(imm_flags, IMM_DROWNING	)) strcat(buf, " drowning");
    if (IS_SET(imm_flags, IMM_LIGHT		)) strcat(buf, " light");
    if (IS_SET(imm_flags, IMM_IRON		)) strcat(buf, " iron");
    if (IS_SET(imm_flags, IMM_MITHRIL		)) strcat(buf, " mithril");
    if (IS_SET(imm_flags, IMM_SILVER		)) strcat(buf, " silver");
    if (IS_SET(imm_flags, IMM_SLEEP		)) strcat(buf, " sleep");
	if (IS_SET(imm_flags, IMM_SOUND		)) strcat(buf, " sound");
	if (IS_SET(imm_flags, IMM_INTERNAL	)) strcat(buf, " internal");
	return ( buf[0] != '\0' ) ? buf+1 : (char*)"none";
}

char *wear_bit_name(long wear_flags[])
{
    static char buf[512];

    buf [0] = '\0';
    if (IS_SET(wear_flags,  ITEM_TAKE		)) strcat(buf, " take");
    if (IS_SET(wear_flags,  ITEM_WEAR_FINGER	)) strcat(buf, " finger");
    if (IS_SET(wear_flags,  ITEM_WEAR_NECK	)) strcat(buf, " neck");
    if (IS_SET(wear_flags,  ITEM_WEAR_BODY	)) strcat(buf, " torso");
    if (IS_SET(wear_flags,  ITEM_WEAR_HEAD	)) strcat(buf, " head");
    if (IS_SET(wear_flags,  ITEM_WEAR_LEGS	)) strcat(buf, " legs");
    if (IS_SET(wear_flags,  ITEM_WEAR_FEET	)) strcat(buf, " feet");
    if (IS_SET(wear_flags,  ITEM_WEAR_HANDS	)) strcat(buf, " hands");
    if (IS_SET(wear_flags,  ITEM_WEAR_ARMS	)) strcat(buf, " arms");
    if (IS_SET(wear_flags,  ITEM_WEAR_SHIELD	)) strcat(buf, " shield");
    if (IS_SET(wear_flags,  ITEM_WEAR_ABOUT	)) strcat(buf, " body");
    if (IS_SET(wear_flags,  ITEM_WEAR_WAIST	)) strcat(buf, " waist");
    if (IS_SET(wear_flags,  ITEM_WEAR_WRIST	)) strcat(buf, " wrist");
    if (IS_SET(wear_flags,  ITEM_WEAR_WIELD	)) strcat(buf, " wield");
    if (IS_SET(wear_flags,  ITEM_WEAR_HOLD	)) strcat(buf, " hold");
    if (IS_SET(wear_flags,  ITEM_WEAR_FLOAT	)) strcat(buf, " float");
	if (IS_SET(wear_flags,  ITEM_WEAR_BRAND    )) strcat(buf, " brand");
    if (IS_SET(wear_flags,  ITEM_WEAR_STRAPPED	)) strcat(buf, " strapped");
	if (IS_SET(wear_flags,	ITEM_WEAR_COSMETIC )) strcat(buf, " cosmetic");
    return ( buf[0] != '\0' ) ? buf+1 : (char*)"none";
}

char *form_bit_name(long form_flags[])
{
    static char buf[512];

    buf[0] = '\0';
    if (IS_SET(form_flags,  FORM_POISON	)) strcat(buf, " poison");
    else if (IS_SET(form_flags,  FORM_EDIBLE	)) strcat(buf, " edible");
    if (IS_SET(form_flags,  FORM_MAGICAL	)) strcat(buf, " magical");
    if (IS_SET(form_flags,  FORM_INSTANT_DECAY	)) strcat(buf, " instant_rot");
    if (IS_SET(form_flags,  FORM_OTHER		)) strcat(buf, " other");
    if (IS_SET(form_flags,  FORM_ANIMAL	)) strcat(buf, " animal");
    if (IS_SET(form_flags,  FORM_SENTIENT	)) strcat(buf, " sentient");
    if (IS_SET(form_flags,  FORM_UNDEAD	)) strcat(buf, " undead");
    if (IS_SET(form_flags,  FORM_CONSTRUCT	)) strcat(buf, " construct");
    if (IS_SET(form_flags,  FORM_MIST		)) strcat(buf, " mist");
    if (IS_SET(form_flags,  FORM_INTANGIBLE	)) strcat(buf, " intangible");
    if (IS_SET(form_flags,  FORM_BIPED		)) strcat(buf, " biped");
    if (IS_SET(form_flags,  FORM_AQUATIC	)) strcat(buf, " aquatic");
    if (IS_SET(form_flags,  FORM_INSECT	)) strcat(buf, " insect");
    if (IS_SET(form_flags,  FORM_SPIDER	)) strcat(buf, " spider");
    if (IS_SET(form_flags,  FORM_CRUSTACEAN	)) strcat(buf, " crustacean");
    if (IS_SET(form_flags,  FORM_WORM		)) strcat(buf, " worm");
    if (IS_SET(form_flags,  FORM_BLOB		)) strcat(buf, " blob");
    if (IS_SET(form_flags,  FORM_MAMMAL	)) strcat(buf, " mammal");
    if (IS_SET(form_flags,  FORM_BIRD		)) strcat(buf, " bird");
    if (IS_SET(form_flags,  FORM_REPTILE	)) strcat(buf, " reptile");
    if (IS_SET(form_flags,  FORM_SNAKE		)) strcat(buf, " snake");
    if (IS_SET(form_flags,  FORM_DRAGON	)) strcat(buf, " dragon");
    if (IS_SET(form_flags,  FORM_AMPHIBIAN	)) strcat(buf, " amphibian");
    if (IS_SET(form_flags,  FORM_FISH		)) strcat(buf, " fish");
    if (IS_SET(form_flags,  FORM_COLD_BLOOD 	)) strcat(buf, " cold_blooded");

    return ( buf[0] != '\0' ) ? buf+1 : (char*)"none";
}

char *part_bit_name(long part_flags[])
{
    static char buf[512];

    buf[0] = '\0';
    if (IS_SET(part_flags,  PART_HEAD		)) strcat(buf, " head");
    if (IS_SET(part_flags,  PART_ARMS		)) strcat(buf, " arms");
    if (IS_SET(part_flags,  PART_LEGS		)) strcat(buf, " legs");
    if (IS_SET(part_flags,  PART_HEART		)) strcat(buf, " heart");
    if (IS_SET(part_flags,  PART_BRAINS	)) strcat(buf, " brains");
    if (IS_SET(part_flags,  PART_GUTS		)) strcat(buf, " guts");
    if (IS_SET(part_flags,  PART_HANDS		)) strcat(buf, " hands");
    if (IS_SET(part_flags,  PART_FEET		)) strcat(buf, " feet");
    if (IS_SET(part_flags,  PART_FINGERS	)) strcat(buf, " fingers");
    if (IS_SET(part_flags,  PART_EAR		)) strcat(buf, " ears");
    if (IS_SET(part_flags,  PART_EYE		)) strcat(buf, " eyes");
    if (IS_SET(part_flags,  PART_LONG_TONGUE	)) strcat(buf, " long_tongue");
    if (IS_SET(part_flags,  PART_EYESTALKS	)) strcat(buf, " eyestalks");
    if (IS_SET(part_flags,  PART_TENTACLES	)) strcat(buf, " tentacles");
    if (IS_SET(part_flags,  PART_FINS		)) strcat(buf, " fins");
    if (IS_SET(part_flags,  PART_WINGS		)) strcat(buf, " wings");
    if (IS_SET(part_flags,  PART_TAIL		)) strcat(buf, " tail");
    if (IS_SET(part_flags,  PART_CLAWS		)) strcat(buf, " claws");
    if (IS_SET(part_flags,  PART_FANGS		)) strcat(buf, " fangs");
    if (IS_SET(part_flags,  PART_HORNS		)) strcat(buf, " horns");
    if (IS_SET(part_flags,  PART_SCALES	)) strcat(buf, " scales");

    return ( buf[0] != '\0' ) ? buf+1 : (char*)"none";
}

char *weapon_bit_name(int weapon_flags)
{
    static char buf[512];

    buf[0] = '\0';
    if (IS_SET_OLD(weapon_flags, WEAPON_FLAMING)) strcat(buf, " flaming");
    if (IS_SET_OLD(weapon_flags, WEAPON_FROST)) strcat(buf, " frost");
    if (IS_SET_OLD(weapon_flags, WEAPON_VAMPIRIC)) strcat(buf, " vampiric");
    if (IS_SET_OLD(weapon_flags, WEAPON_SHARP)) strcat(buf, " sharp");
    if (IS_SET_OLD(weapon_flags, WEAPON_VORPAL)) strcat(buf, " vorpal");
    if (IS_SET_OLD(weapon_flags, WEAPON_TWO_HANDS)) strcat(buf, " two-handed");
    if (IS_SET_OLD(weapon_flags, WEAPON_SHOCKING) 	) strcat(buf, " shocking");
    if (IS_SET_OLD(weapon_flags, WEAPON_POISON	)) strcat(buf, " poison");

    return ( buf[0] != '\0' ) ? buf+1 : (char*)"none";
}

char *cont_bit_name( int cont_flags)
{
    static char buf[512];

    buf[0] = '\0';

    if (IS_SET_OLD(cont_flags, CONT_CLOSEABLE)) strcat(buf, " closable");
    if (IS_SET_OLD(cont_flags, CONT_PICKPROOF)) strcat(buf, " pickproof");
    if (IS_SET_OLD(cont_flags, CONT_CLOSED	)) strcat(buf, " closed");
    if (IS_SET_OLD(cont_flags, CONT_LOCKED	)) strcat(buf, " locked");

    return (buf[0] != '\0' ) ? buf+1 : (char*)"none";
}


char *off_bit_name(long off_flags[])
{
    static char buf[512];

    buf[0] = '\0';

    if (IS_SET(off_flags, OFF_AREA_ATTACK	)) strcat(buf, " area_attack");
    if (IS_SET(off_flags, OFF_BACKSTAB	)) strcat(buf, " backstab");
    if (IS_SET(off_flags, OFF_BASH		)) strcat(buf, " bash");
    if (IS_SET(off_flags, OFF_BERSERK		)) strcat(buf, " berserk");
    if (IS_SET(off_flags, OFF_DISARM		)) strcat(buf, " disarm");
    if (IS_SET(off_flags, OFF_DODGE		)) strcat(buf, " dodge");
    if (IS_SET(off_flags, OFF_FADE		)) strcat(buf, " fade");
    if (IS_SET(off_flags, OFF_FAST		)) strcat(buf, " fast");
    if (IS_SET(off_flags, OFF_KICK		)) strcat(buf, " kick");
    if (IS_SET(off_flags, OFF_KICK_DIRT	)) strcat(buf, " kick_dirt");
    if (IS_SET(off_flags, OFF_PARRY		)) strcat(buf, " parry");
    if (IS_SET(off_flags, OFF_RESCUE		)) strcat(buf, " rescue");
    if (IS_SET(off_flags, OFF_TAIL		)) strcat(buf, " tail");
    if (IS_SET(off_flags, OFF_TRIP		)) strcat(buf, " trip");
    if (IS_SET(off_flags, OFF_CRUSH		)) strcat(buf, " crush");
    if (IS_SET(off_flags, ASSIST_ALL		)) strcat(buf, " assist_all");
    if (IS_SET(off_flags, ASSIST_ALIGN	)) strcat(buf, " assist_align");
    if (IS_SET(off_flags, ASSIST_RACE		)) strcat(buf, " assist_race");
    if (IS_SET(off_flags, ASSIST_PLAYERS	)) strcat(buf, " assist_players");
    if (IS_SET(off_flags, ASSIST_GUARD	)) strcat(buf, " assist_guard");
    if (IS_SET(off_flags, ASSIST_VNUM		)) strcat(buf, " assist_vnum");
    if (IS_SET(off_flags, NO_TRACK		)) strcat(buf, " no_track");
    if (IS_SET(off_flags, STATIC_TRACKING	)) strcat(buf, " static_track");
    if (IS_SET(off_flags, SPAM_MURDER	)) strcat(buf," spam_murder");
	if (IS_SET(off_flags, ASSIST_GROUP	)) strcat(buf," assist_group");
    return ( buf[0] != '\0' ) ? buf+1 : (char*)"none";
}

/* Room Affects */
void	affect_modify_room	args( ( ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *paf, bool fAdd ) );

/*
 * Apply or remove an affect to a room.
 */

void init_affect_room( ROOM_AFFECT_DATA *paf )
{
	paf->where	= TO_ROOM_AFFECTS;
	paf->type	= 0;
	paf->aftype	= AFT_SPELL;
	paf->level	= 0;
	paf->duration	= 0;
	paf->location	= 0;
	paf->modifier	= 0;
	zero_vector(paf->bitvector);
	paf->pulse_fun	= NULL;
	paf->tick_fun	= NULL;
	paf->end_fun	= NULL;
	paf->owner	= NULL;
	/* Morg - Valgrind fix */
	paf->valid = FALSE;
	return;
}

void affect_modify_room(ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *paf, bool fAdd )
{
	int mod;

	mod = paf->modifier;

	if (fAdd)
	{
		switch (paf->where)
		{
		case TO_ROOM_AFFECTS:	
			BITWISE_OR(room->affected_by, paf->bitvector);	
			break;
		case TO_ROOM_FLAGS:	
			BITWISE_OR(room->room_flags, paf->bitvector);	
			break;
		case TO_ROOM_CONST:	
			break;
		}
	}
	else
	{
		switch (paf->where)
        	{
		case TO_ROOM_AFFECTS:	
			BITWISE_XAND(room->affected_by, paf->bitvector);	
			break;
		case TO_ROOM_FLAGS:	
			BITWISE_XAND(room->room_flags, paf->bitvector);	
			break;
		case TO_ROOM_CONST:	
			break;
	        }
		mod = 0 - mod;
	}

	switch (paf->location)
	{
		case APPLY_ROOM_NONE:					break;
		case APPLY_ROOM_NOPE:					break;
		case APPLY_ROOM_HEAL:	room->heal_rate += mod;		break;
		case APPLY_ROOM_MANA:	room->mana_rate += mod;		break;
		case APPLY_ROOM_SECT:	room->sector_type += mod;	break;
		default: bug("Affect_modify_room: unknown location %d.", paf->location); break;
	}

	return;
}

/*
 * Give an affect to a room.
 */
void affect_to_room( ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *paf )
{
	paf->owner = NULL;
	paf->end_fun = NULL;
	paf->tick_fun = NULL;
	new_affect_to_room(room,paf);
	return;
}
void new_affect_to_room( ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *paf )
{
    ROOM_AFFECT_DATA *paf_new;
    ROOM_INDEX_DATA *pRoomIndex;

    if ( !room->affected ){
		if ( top_affected_room ) {

			for ( pRoomIndex  = top_affected_room; pRoomIndex->aff_next != NULL;
					pRoomIndex  = pRoomIndex->aff_next )
				continue;

			pRoomIndex->aff_next = room;
		} else {
			top_affected_room = room;
		}
     	room->aff_next = NULL;
	}

    paf_new = new_affect_room();

    *paf_new		= *paf;
    paf_new->next	= room->affected;
    room->affected	= paf_new;

    affect_modify_room( room , paf_new, TRUE );
    return;
}

void affect_check_room(ROOM_INDEX_DATA *room,int where, long vector[])
{
    ROOM_AFFECT_DATA *paf;

    if (IS_ZERO_VECTOR(vector))
		return;

    for (paf = room->affected; paf != NULL; paf = paf->next)
	if (paf->where == where && vector_equal(paf->bitvector,vector))
	{
	    switch (where)
	    {
	        case TO_ROOM_AFFECTS:
				BITWISE_OR(room->affected_by,vector);
		    break;
		case TO_ROOM_FLAGS:
	      	    BITWISE_OR(room->room_flags, vector);
	    	    break;
	        case TO_ROOM_CONST:
		    break;
	    }
	    return;
	}
}

/*
 * Remove an affect from a room.
 */
void affect_remove_room( ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *paf )
{
    int where;
	long vector[MAX_BITVECTOR];

    if ( room->affected == NULL ) {
		bug( "Affect_remove_room: no affect.", 0 );
		return;
    }

    if(paf->end_fun != NULL)
		(*paf->end_fun) (room, paf);

    affect_modify_room( room, paf, FALSE );
    where = paf->where;
	copy_vector(vector, paf->bitvector);

    if ( paf == room->affected ) {
		room->affected	= paf->next;
    } else {
		ROOM_AFFECT_DATA *prev;

		for ( prev = room->affected; prev != NULL; prev = prev->next ) {
		    if ( prev->next == paf ) {
			prev->next = paf->next;
			break;
			}
		}

		if ( prev == NULL ) {
		    bug( "Affect_remove_room: cannot find paf.", 0 );
		    return;
		}
    }

    if ( !room->affected ) {
		ROOM_INDEX_DATA *prev;

		if (top_affected_room  == room) {
			top_affected_room = room->aff_next;
		} else {
			for(prev=top_affected_room;prev->aff_next;prev=prev->aff_next ) {
				if ( prev->aff_next == room ) {
				prev->aff_next = room->aff_next;
				break;
				}
			}
		
			if ( prev == NULL ) {
				bug( "Affect_remove_room: cannot find room.", 0 );
			   return;
			}
		}
	
		room->aff_next = NULL;
	}
	
    free_affect_room(paf);

    affect_check_room(room,where,vector);
    return;
}

/*
 * Strip all affects of a given sn.
 */
void affect_strip_room( ROOM_INDEX_DATA *room, int sn )
{
    ROOM_AFFECT_DATA *paf;
    ROOM_AFFECT_DATA *paf_next;

    for ( paf = room->affected; paf != NULL; paf = paf_next )
    {
	paf_next = paf->next;
	if ( paf->type == sn )
	    affect_remove_room( room, paf );
    }

    return;
}



/*
 * Return true if a room is affected by a spell.
 */
bool is_affected_room( ROOM_INDEX_DATA *room, int sn )
{
    ROOM_AFFECT_DATA *paf;

	if (!room)
		return FALSE;

    for ( paf = room->affected; paf != NULL; paf = paf->next )
    {
	if ( paf->type == sn )
	    return TRUE;
    }

    return FALSE;
}



/*
 * Add or enhance an affect.
 */
void affect_join_room( ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *paf )
{
    ROOM_AFFECT_DATA *paf_old;

	for ( paf_old = room->affected; paf_old != NULL; paf_old = paf_old->next )
    {
	if ( paf_old->type == paf->type )
	{
	    paf->level = (paf->level + paf_old->level) / 2;
	    paf->duration += paf_old->duration;
	    paf->modifier += paf_old->modifier;
	    affect_remove_room( room, paf_old );
	    break;
	}
    }

    affect_to_room( room, paf );
    return;
}

/*
 * Return ascii name of an raffect location.
 */
char *raffect_loc_name( int location )
{
    switch ( location )
    {
    case APPLY_ROOM_NONE:	return "none";
    case APPLY_ROOM_HEAL:	return "heal rate";
    case APPLY_ROOM_MANA:	return "mana rate";
    case APPLY_ROOM_SECT:	return "sector";
    case APPLY_ROOM_NOPE:	return "nope";
    }

    bug( "raffect_location_name: unknown location %d.", location );
    return "(unknown)";
}


/*
 * Return ascii name of an affect bit vector.
 */
char *raffect_bit_name( long vector[])
{
    static char buf[512];

    buf[0] = '\0';
    if ( IS_SET(vector, AFF_ROOM_CURSE)     ) strcat( buf, " curse"           );
    if ( IS_SET(vector, AFF_ROOM_POISON)    ) strcat( buf, " poison"          );
    if ( IS_SET(vector, AFF_ROOM_PLAGUE)    ) strcat( buf, " plague"          );
    if ( IS_SET(vector, AFF_ROOM_SLEEP)     ) strcat( buf, " sleep"           );
    if ( IS_SET(vector, AFF_ROOM_SLOW)      ) strcat( buf, " slow"            );
    return ( buf[0] != '\0' ) ? buf+1 : (char*)"none";
}

void charaff_to_obj (OBJ_DATA *obj, AFFECT_DATA *paf)
{
	AFFECT_DATA *paf_new;

	paf_new = new_affect();
	*paf_new = *paf;
	paf_new->next = obj->charaffs;
	obj->charaffs = paf_new;

	return;
}

void charaff_to_obj_index (OBJ_INDEX_DATA *obj, AFFECT_DATA *paf)
{
	AFFECT_DATA *paf_new;
	
	paf_new = new_affect();
	*paf_new = *paf;
	paf_new->next = obj->charaffs;
	obj->charaffs = paf_new;

	return;
}

void charaff_from_obj_index(OBJ_INDEX_DATA *obj, AFFECT_DATA *paf)
{
	if ( obj->charaffs == NULL )
	{
		bug( "charaff_from_obj_index: no affect.", 0 );
		return;
	}

	if ( paf == obj->charaffs)
	{
		obj->charaffs = paf->next;
	}
	else
	{
		AFFECT_DATA *prev;
		for ( prev = obj->charaffs; prev != NULL; prev = prev->next )                                                  
		{                                                                                                             
			if ( prev->next == paf )                                                                                  
			{                                                                                                         
				prev->next = paf->next;                                                                                   
				break;                                                                                                    
			}                                                                                                         
		}                                                                                                             
                                                                               
		if ( prev == NULL )                                                                                           
		{                                                                                                             
			bug( "charaff_from_obj_index: cannot find paf.", 0 );                                                              
			return;                                                                                                   
		}                                                                                                             
	}                                                                                                             
                                                                                    
	free_affect(paf);                                                                                             
	return;
}

/*
 * Apply or remove an affect to an object.
 */

void init_affect_obj (OBJ_AFFECT_DATA *paf)
{
	paf->where 		=	TO_OBJ_AFFECTS;
	paf->type		=	0;
	paf->aftype		=	AFT_SPELL;
	paf->level		=	0;
	paf->duration	=	0;
	paf->location	=	0;
	paf->modifier	=	0;
	zero_vector(paf->bitvector);
	paf->tick_fun 	=	NULL;
	paf->pulse_fun	=	NULL;
	paf->end_fun 	=	NULL;
	paf->owner		=	NULL;
	return;
}

void affect_modify_obj(OBJ_DATA *obj, OBJ_AFFECT_DATA *paf, bool fAdd)
{
	CHAR_DATA *ch;
	int mod, wear;
	OBJ_APPLY_DATA *app, *p_app;

	mod = paf->modifier;

	if (fAdd) {
		switch (paf->where) {
			case TO_OBJ_AFFECTS:
				BITWISE_OR(obj->affected_by, paf->bitvector);
				break;
			case TO_OBJ_APPLY:
				ch = obj->carried_by;
				wear = obj->wear_loc;
				if (ch && (wear != WEAR_NONE))
					unequip_char(ch,obj,FALSE);
				app = new_apply_data();
				app->location	=	paf->location;
				app->modifier	=	paf->modifier;
				app->type		=	paf->type;
				app->next		=	obj->apply;
				obj->apply		=	app;
				if (ch && (wear != WEAR_NONE))
					equip_char(ch,obj,wear,FALSE);
				break;
		}
	} else {
		switch (paf->where) {
			case TO_OBJ_AFFECTS:
				BITWISE_XAND(obj->affected_by, paf->bitvector);
				break;
			case TO_OBJ_APPLY:
				ch = obj->carried_by;
				wear = obj->wear_loc;
				if (ch && (wear != WEAR_NONE))
					unequip_char(ch,obj,FALSE);
				for (app = obj->apply; app; app = app->next)
					if (app->type == paf->type
						&& app->location == paf->location)
						break;
				if (!app) {
					if (ch && (wear != WEAR_NONE))
						equip_char(ch,obj,wear,FALSE);
					break;
				}
				if (obj->apply != app) {
					for (p_app = obj->apply; p_app; p_app = p_app->next)
						if (p_app->next == app)
							break;
					p_app->next = app->next;
					free_apply (app);
				} else {
					obj->apply = app->next;
					free_apply (app);
				}
				if (ch && (wear != WEAR_NONE))
					equip_char(ch, obj, wear,FALSE);
				break;
		}
		mod = 0 - mod;
	}

	/* TO_OBJ_APPLY, as seen above, lets an item affect modify the apply
	 * portion of the object.  For example, a rust spell cast on a target
	 * could cause some of the target's metal eq to rust temporarily, giving
	 * them a -dex apply for the duration.  This should properly add and
	 * remove applies due to affects.
	 */
	
	if (paf->where == TO_OBJ_AFFECTS) {
		switch (paf->location) {
			case APPLY_OBJ_NONE:
				break;
			case APPLY_OBJ_V0:
				obj->value[0] += mod;
				break;
			case APPLY_OBJ_V1:
				obj->value[1] += mod;
				break;
			case APPLY_OBJ_V2:
				obj->value[2] += mod;
				break;
			case APPLY_OBJ_V3:
				obj->value[3] += mod;
				break;
			case APPLY_OBJ_V4:
				obj->value[4] += mod;
				break;
			case APPLY_OBJ_WEIGHT:
				obj->weight += mod;
				break;
			default:
				bug("affect_modify_obj: unknown location %d.",paf->location);
		}
	}
	
	return;
}

void affect_to_obj (OBJ_DATA *obj, OBJ_AFFECT_DATA *paf)
{
	OBJ_AFFECT_DATA *paf_new;

	paf_new = new_affect_obj();
	
	*paf_new		= 	*paf;
	paf_new->next	=	obj->affected;
	obj->affected	=	paf_new;

	affect_modify_obj (obj, paf_new, TRUE);
	return;
}

void affect_check_obj(OBJ_DATA *obj, int where, long vector[])
{
	OBJ_AFFECT_DATA *paf;

	if (IS_ZERO_VECTOR(vector))
		return;

	for (paf = obj->affected; paf != NULL; paf = paf->next) {
		if (paf->where == where && vector_equal(paf->bitvector,vector)) {
			switch (where) {
				case TO_OBJ_AFFECTS:
					BITWISE_OR(obj->affected_by,vector);
					break;
			}
			return;
		}
	}

	return;
}

void affect_remove_obj (OBJ_DATA *obj, OBJ_AFFECT_DATA *paf, bool show)
{
	int where;
	long vector[MAX_BITVECTOR];

	if (!obj->affected)
		return bug("affect_remove_obj: no affect!",0);
	
	if (paf->end_fun != NULL && show)
		(*paf->end_fun) (obj,paf);
	
	affect_modify_obj (obj,paf,FALSE);
	where = paf->where;
	copy_vector(vector, paf->bitvector);

	if (paf == obj->affected) {
		obj->affected = paf->next;
	} else {
		OBJ_AFFECT_DATA *prev;
		
		for (prev = obj->affected; prev != NULL; prev = prev->next) {
			if (prev->next == paf) {
				prev->next = paf->next;
				break;
			}
		}
		
		if (!prev)
			return bug("affect_remove_obj: cannot find paf on obj #%d!",obj->pIndexData->vnum);
	}

	free_affect_obj(paf);

	affect_check_obj(obj,where,vector);
	return;
}

void affect_strip_obj (OBJ_DATA *obj, int sn)
{
	OBJ_AFFECT_DATA *paf;
	OBJ_AFFECT_DATA *paf_next;

	for (paf = obj->affected; paf != NULL; paf = paf_next ) {
		paf_next = paf->next;
		if (paf->type == sn)
			affect_remove_obj(obj,paf,TRUE);
	}

	return;
}

bool is_affected_obj (OBJ_DATA *obj, int sn)
{
	OBJ_AFFECT_DATA *paf;

	for (paf = obj->affected; paf != NULL; paf = paf->next)
		if (paf->type == sn)
			return TRUE;

	return FALSE;
}

void affect_join_obj (OBJ_DATA *obj, OBJ_AFFECT_DATA *paf)
{
	OBJ_AFFECT_DATA *paf_old;

	for (paf_old = obj->affected; paf_old != NULL; paf_old = paf_old->next) {
		if (paf_old->type == paf->type) {
			paf->level = (paf->level + paf_old->level) / 2;
			paf->duration += paf_old->duration;
			paf->modifier += paf_old->modifier;
			affect_remove_obj (obj,paf_old,TRUE);
			break;
		}
	}

	affect_to_obj (obj, paf);
	return;
}

char *oaffect_loc_name (int location )
{
	switch (location) {
		case APPLY_OBJ_NONE:		return "none";
		case APPLY_OBJ_V0:			return "value0";
		case APPLY_OBJ_V1:			return "value1";
		case APPLY_OBJ_V2:          return "value2";
		case APPLY_OBJ_V3:          return "value3";
		case APPLY_OBJ_V4:          return "value4";
		case APPLY_OBJ_WEIGHT:		return "weight";
	}
	bug ("oaffect_loc_name: unknown  location %d.",location);
	return "(unknown)";
}

char *oaffect_bit_name (long vector[] )
{
	static char buf[512];

	buf[0] = '\0';

	if (IS_SET(vector, AFF_OBJ_BURNING)		) strcat( buf, " burning"		);
	return (buf[0] != '\0' ) ? buf+1 : (char*)"none";
}

/*
 * Apply or remove an affect to an area.
 */

void init_affect_area (AREA_AFFECT_DATA *paf)
{
	paf->where 		=	TO_AREA_AFFECTS;
	paf->type		=	0;
	paf->aftype		=	AFT_SPELL;
	paf->level		=	0;
	paf->duration	=	0;
	paf->location	=	0;
	paf->modifier	=	0;
	zero_vector(paf->bitvector);
	paf->pulse_fun	=	NULL;
	paf->tick_fun 	=	NULL;
	paf->end_fun 	=	NULL;
	paf->owner		=	NULL;
	return;
}

void affect_modify_area(AREA_DATA *area, AREA_AFFECT_DATA *paf, bool fAdd)
{
	int mod;

	mod = paf->modifier;

	if (fAdd) {
		switch (paf->where) {
			case TO_AREA_AFFECTS:
				BITWISE_OR(area->affected_by, paf->bitvector);
				break;
		}
	} else {
		switch (paf->where) {
			case TO_AREA_AFFECTS:
				BITWISE_XAND(area->affected_by, paf->bitvector);
				break;
		}
		mod = 0 - mod;
	}

	switch (paf->location) {
		case APPLY_AREA_NONE:			
			break;
		case APPLY_AREA_TEMP:
			area->temp+= paf->modifier;
			if (area->temp >= MAX_TEMP)
				area->temp = MAX_TEMP - 1;
			break;
		case APPLY_AREA_WIND:
			area->wind += paf->modifier;
			if (area->wind >= MAX_WIND)
				area->wind = MAX_WIND - 1;
			break;
		case APPLY_AREA_SKY:	
			area->sky += paf->modifier;
			if (area->sky >= MAX_SKY)
				area->sky = MAX_SKY - 1;
			break;
		default:
			bug("affect_modify_area: unknown location %d.",paf->location);
	}
	
	return;
}

void affect_to_area (AREA_DATA *area, AREA_AFFECT_DATA *paf)
{
	AREA_AFFECT_DATA *paf_new;

	paf_new = new_affect_area();
	
	*paf_new		= 	*paf;
	paf_new->next	=	area->affected;
	area->affected	=	paf_new;

	affect_modify_area (area, paf_new, TRUE);
	return;
}

void affect_check_area(AREA_DATA *area, int where, long vector[])
{
	AREA_AFFECT_DATA *paf;

	if (vector == 0)
		return;

	for (paf = area->affected; paf != NULL; paf = paf->next) {
		if (paf->where == where && vector_equal(paf->bitvector, vector)) {
			switch (where) {
				case TO_AREA_AFFECTS:
					BITWISE_OR(area->affected_by,vector);
					break;
			}
			return;
		}
	}

	return;
}

void affect_remove_area (AREA_DATA *area, AREA_AFFECT_DATA *paf)
{
	int where;
	long vector[MAX_BITVECTOR];

	if (!area->affected)
		return bug("affect_remove_area: no affect!",0);
	
	if (paf->end_fun != NULL)
		(*paf->end_fun) (area,paf);

	affect_modify_area (area,paf,FALSE);
	where = paf->where;
	copy_vector(vector, paf->bitvector);

	if (paf == area->affected) {
		area->affected = paf->next;
	} else {
		AREA_AFFECT_DATA *prev;
		
		for (prev = area->affected; prev != NULL; prev = prev->next) {
			if (prev->next == paf) {
				prev->next = paf->next;
				break;
			}
		}
		
		if (!prev)
			return bug("affect_remove_area: cannot find paf!",0);
	}

	free_affect_area(paf);

	affect_check_area(area,where,vector);
	return;
}

void affect_strip_area (AREA_DATA *area, int sn)
{
	AREA_AFFECT_DATA *paf;
	AREA_AFFECT_DATA *paf_next;

	for (paf = area->affected; paf != NULL; paf = paf_next ) {
		paf_next = paf->next;
		if (paf->type == sn)
			affect_remove_area(area,paf);
	}

	return;
}

bool is_affected_area (AREA_DATA *area, int sn)
{
	AREA_AFFECT_DATA *paf;

	for (paf = area->affected; paf != NULL; paf = paf->next)
		if (paf->type == sn)
			return TRUE;

	return FALSE;
}

void affect_join_area (AREA_DATA *area, AREA_AFFECT_DATA *paf)
{
	AREA_AFFECT_DATA *paf_old;

	for (paf_old = area->affected; paf_old != NULL; paf_old = paf_old->next) {
		if (paf_old->type == paf->type) {
			paf->level = (paf->level + paf_old->level) / 2;
			paf->duration += paf_old->duration;
			paf->modifier += paf_old->modifier;
			affect_remove_area (area,paf_old);
			break;
		}
	}

	affect_to_area(area, paf);
	return;
}

char *aaffect_loc_name (int location )
{
	switch (location) {
		case APPLY_AREA_NONE:		return "none";
		case APPLY_AREA_SKY:		return "sky";
		case APPLY_AREA_TEMP:		return "temperature";
		case APPLY_AREA_WIND:		return "wind";							
	}
	bug ("aaffect_loc_name: unknown  location %d.",location);
	return "(unknown)";
}

bool is_safe_rspell_nom(int level, CHAR_DATA *victim )
{
  /* ghosts are safe */
  if ( !IS_NPC(victim) && (victim->ghost))
    return TRUE;

  if  ( victim->level < 5  && !IS_NPC(victim) )
    return TRUE;

  if ( !IS_NPC(victim) &&
      ((level >= victim->level + 10) || (victim->level >= level + 10)))
    return TRUE;

  return FALSE;
}


bool is_safe_rspell(int level, CHAR_DATA *victim)
{
  if (is_safe_rspell_nom(level,victim))
    {
      act("The gods protect $n.",victim,NULL,NULL,TO_CHAR);
      act("The gods protect $n from the spell of room.",victim,NULL,NULL,TO_ROOM);
      return TRUE;
    }
  else return FALSE;
}

	
/*
 * Return ascii name of an affect bit vector.
 */
char *flag_room_name( int vector )
{
    static char buf[512];

    buf[0] = '\0';
    if ( vector & ROOM_DARK         ) strcat( buf, " dark"         );
    if ( vector & ROOM_NO_MOB        ) strcat( buf, " nomob"        );
    if ( vector & ROOM_INDOORS      ) strcat( buf, " indoors"      );
    if ( vector & ROOM_PRIVATE      ) strcat( buf, " private"      );
    if ( vector & ROOM_SAFE         ) strcat( buf, " safe"         );
    if ( vector & ROOM_SOLITARY     ) strcat( buf, " solitary"     );
    if ( vector & ROOM_PET_SHOP     ) strcat( buf, " petshop"      );
    if ( vector & ROOM_NO_RECALL    ) strcat( buf, " norecall"     );
    if ( vector & ROOM_IMP_ONLY     ) strcat( buf, " imp_only"     );
    if ( vector & ROOM_GODS_ONLY    ) strcat( buf, " god_only"     );
    if ( vector & ROOM_HEROES_ONLY  ) strcat( buf, " heroes"       );
    if ( vector & ROOM_NEWBIES_ONLY ) strcat( buf, " newbies"      );
    if ( vector & ROOM_LAW          ) strcat( buf, " law"          );
    if ( vector & ROOM_NOWHERE      ) strcat( buf, " nowhere"      );
    if ( vector & ROOM_NO_MAGIC     ) strcat( buf, " nomagic"      );
    return ( buf[0] != '\0' ) ? buf+1 : (char*)"none";
}

void modify_location(CHAR_DATA *ch, int location, int mod, bool add)
{
  int i;
  if(!add)
	mod = 0 - mod;

  switch(location)
  {
    case APPLY_NONE:                                            break;
    case APPLY_STR:           ch->mod_stat[STAT_STR]    += mod; break;
    case APPLY_DEX:           ch->mod_stat[STAT_DEX]    += mod; break;
    case APPLY_INT:           ch->mod_stat[STAT_INT]    += mod; break;
    case APPLY_WIS:           ch->mod_stat[STAT_WIS]    += mod; break;
    case APPLY_CON:           ch->mod_stat[STAT_CON]    += mod; break;
    case APPLY_SEX:           ch->sex                   += mod; break;
    case APPLY_CLASS:                                           break;
    case APPLY_AGE:                                             break;
    case APPLY_HEIGHT:                                          break;
    case APPLY_WEIGHT:                                          break;
    case APPLY_MANA:          ch->max_mana              += mod; break;
    case APPLY_HIT:           ch->max_hit               += mod; break;
    case APPLY_MOVE:          ch->max_move              += mod; break;
    case APPLY_GOLD:                                            break;
    case APPLY_EXP:                                             break;
    case APPLY_AC:
        for (i = 0; i < 4; i ++)
            	ch->armor[i] += mod;
        break;
    case APPLY_HITROLL:       ch->hitroll               += mod; break;
    case APPLY_DAMROLL:       ch->damroll               += mod; break;
    case APPLY_SAVES:         ch->saving_throw          += mod; break;
    case APPLY_SAVING_PARA:   ch->saving_throw			+= mod; break;
    case APPLY_SAVING_ROD:    ch->saving_throw          += mod; break;
    case APPLY_SAVING_PETRI:  ch->saving_throw          += mod; break;
    case APPLY_SAVING_BREATH: ch->saving_throw          += mod; break;
    case APPLY_SAVING_SPELL:  ch->saving_throw          += mod; break;
    case APPLY_SPELL_AFFECT:                                    break;
	case APPLY_CARRY_WEIGHT:  ch->carry_weight			+= mod; break;
    case APPLY_DEFENSE:       ch->defense_mod           += mod; break;
    case APPLY_SIZE:          ch->size                  += mod; break;
    case APPLY_REGENERATION:  ch->regen_rate            += mod; break;
    case APPLY_ENERGYSTATE:   if(!IS_NPC(ch)) ch->pcdata->energy_state  += mod; break;
    case APPLY_ARMS:          ch->arms                  += mod; break;
    case APPLY_LEGS:          ch->legs                  += mod; break;
	case APPLY_ALIGNMENT:	  ch->alignment				+= mod; break;
	case APPLY_ETHOS:		  if(!IS_NPC(ch)) ch->pcdata->ethos += mod; break;
	case APPLY_BEAUTY:
		if(ch->pcdata->beauty + mod > MAX_BEAUTY)
			ch->pcdata->beauty = MAX_BEAUTY;
		else
			ch->pcdata->beauty += mod;
		break;
	case APPLY_DAM_MOD:
	if(add)
		ch->dam_mod               *= (float)(1 + (float) mod / (float)100);
	else
	{
		mod			   = 0 - mod;
		ch->dam_mod 		  *= (float) 1 / (float) (1 + (float) mod / (float)100);
	}
	break;
  }
}

int get_align(CHAR_DATA *ch)
{
	return ch->alignment;
}

int get_ethos(CHAR_DATA *ch)
{
	if(!IS_NPC(ch))
		return ch->pcdata->ethos;
	return 0;
}

int damage_queue(CHAR_DATA *ch, CHAR_DATA *victim, int dam, int damtype, bool blockable, int add, int mult, char *dnoun)
{
	        return damage_new(ch, victim, dam, gsn_bash, damtype, TRUE, blockable, add, mult, dnoun);
}
