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
*	ROM 2.4 is copyright 1993-1995 Russ Taylor			   *
*	ROM has been brought to you by the ROM consortium		   *
*	    Russ Taylor (rtaylor@pacinfo.com)				   *
*	    Gabrielle Taylor (gtaylor@pacinfo.com)			   *
*	    Brian Moore (rom@rom.efn.org)				   *
*	By using this code, you have agreed to follow the terms of the	   *
*	ROM license, in the file Rom24/doc/rom.license			   *
***************************************************************************/

#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "merc.h"
#include "recycle.h"
#include "tables.h"
#include "lookup.h"
#include "magic.h"


DECLARE_DO_FUN(do_goto);
DECLARE_DO_FUN(do_yell);
DECLARE_DO_FUN(do_murder);
DECLARE_DO_FUN(do_look);
DECLARE_DO_FUN(do_flee);

bool check_horde(CHAR_DATA *ch)
{
	if(ch->cabal == CABAL_HORDE)
	{
		send_to_char("Huh?\n\r", ch);
		return TRUE;
	}
	else
		return FALSE;
}

void update_cskills(CHAR_DATA *ch)
{
	int i;
	
	if(ch->cabal)
	{
		for(i=0; cabal_skills[i].skill; i++)
		{
			if(ch->cabal != cabal_skills[i].cabal)
				continue;
			
			if(ch->pcdata->cabal_level < cabal_skills[i].level && cabal_skills[i].level != 10) 
				continue;

			if(ch->pcdata->cabal_level != cabal_skills[i].level && cabal_skills[i].specific)
				continue;
			
			if(cabal_skills[i].level == 10 && ch->pcdata->induct != CABAL_LEADER)
				continue;

			if(ch->pcdata->learned[skill_lookup(cabal_skills[i].skill)] < 2)
				ch->pcdata->learned[skill_lookup(cabal_skills[i].skill)] = 70;
		}
	}
	else
	{
		for(i=0; cabal_skills[i].skill; i++)
		{
			ch->pcdata->learned[skill_lookup(cabal_skills[i].skill)] = 0;
		}
	}
}

void do_storytell(CHAR_DATA *ch, char *argument)
{
	char arg1[MSL], arg2[MSL], buf[MSL];
	bool color = FALSE;
	argument = one_argument(argument, arg1);
	if(get_skill(ch,skill_lookup("story tell")) < 3)
		return send_to_char("Huh?\n\r",ch);
	
	if(ch->in_room->cabal != ch->cabal && !IS_IMMORTAL(ch))
		return send_to_char("You don't feel comfortable telling stories here.\n\r",ch);

	if(arg1[0] != '\0' && strcmp(get_char_color(ch,arg1),""))
	{
		argument = one_argument(argument, arg2);
		color = TRUE;
	}
	if(!strcmp(argument,"") || arg1[0] == '\0' || (arg2[0] == '\0' && color))
	{
		send_to_char("Syntax:  storytell color emote message\n\r",ch);
		send_to_char("         storytell emote message\n\r",ch);
		return send_to_char("Use the 'color list' command to see available colors.\n\r",ch);
	}
        sprintf(buf,"%s %s '%s%s%s'",ch->name, color ? arg2 : arg1, color ? get_char_color(ch,arg1) : "",
        argument, color ? END_COLOR(ch) : "");
	area_echo(ch,buf);
}
/*
void spell_fervor(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	if(is_affected(ch,sn))
		return send_to_char("You are already filled with religious fervor.");
	init_affect(&af);
	af.where	= TO_AFFECTS;
	af.type		= sn;
	af.level	= level;
	af.aftype	= AFT_POWER;
	af.location	= APPLY_NONE;
	af.modifier	= 0;
	af.bitvector	= protection;
	affect_to_char(ch,&af);
	af.location	= APPLY_NONE;
	af.modifier = 0;
	af.bitvector = detect_evil;
	affect_to_char(ch,&af);
	act("echo");
	return;
}
*/
void spell_epic(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
	AFFECT_DATA af;
	if(is_affected(ch,sn))
		return send_to_char("You try, but cannot find any more strength in your limbs.\n\r",ch);
	init_affect(&af);
	af.where	= TO_AFFECTS;
	af.type		= sn;
	af.level	= level;
	af.aftype	= AFT_POWER;
	af.location	= APPLY_STR;
	af.modifier	= level / 6 + 1;
	af.duration	= level / 3;
	affect_to_char(ch, &af);
	send_to_char("Strength surges through your body as you prepare for battle.\n\r",ch);
	act("$n concentrates intently for a moment, getting a wild look in $s eyes.",ch,0,0,TO_ROOM);
}
void spell_calm( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    AFFECT_DATA af;

    if(!is_affected(ch, gsn_rage) || is_affected(ch,sn))
        return send_to_char("Calm yourself?  You don't feel very angry.\n\r",ch);
    if(number_percent()>get_skill(ch,sn))
        return send_to_char("You try to calm yourself but your rage overcomes you.\n\r",ch);

        affect_strip(ch,skill_lookup("rage"));
        init_affect(&af);
        af.where        = TO_AFFECTS;
        af.type         = sn;
        af.aftype       = AFT_POWER;
        af.modifier     = -8;
        af.level        = level;
        af.duration     = 3;
        af.location     = APPLY_HITROLL;
        affect_to_char(ch, &af);
        af.location     = APPLY_DAMROLL;
        affect_to_char(ch, &af);
        send_to_char("You manage to snap out of your rage.\n\r",ch);
}

void spell_rage(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
	AFFECT_DATA af;
	OBJ_DATA *obj;
	char buf[MSL];
	if(IS_NPC(ch))
		return;
	if(is_affected(ch,sn) || is_affected(ch, gsn_rage))
		return send_to_char("You are already in a battle rage.\n\r",ch);
	if(is_affected(ch,gsn_calm))
		return send_to_char("You're feeling too calm to build up a battle rage.\n\r",ch);
	init_affect(&af);
	af.where	= TO_AFFECTS;
	af.type		= sn;
	af.level	= level;
	af.aftype	= AFT_POWER;
	af.location	= APPLY_NONE;
	af.modifier	= 0;
	af.duration	= 18 - (level / 5);
	SET_BIT(af.bitvector, AFF_DARK_VISION);
	SET_BIT(af.bitvector, AFF_RAGE);
	affect_to_char(ch, &af);
	af.aftype = AFT_INVIS;
	zero_vector(af.bitvector);
	
	sprintf(buf,"You feel your rage grow as the spirit of the %s surges through you!\n\r",
		capitalize(tribe_table[ch->pcdata->tribe].name));
	send_to_char(buf,ch);
	act("$n starts growling and frothing as $s battle rage overtakes $m!",ch,0,0,TO_ROOM);
	if(is_affected(ch,gsn_sanctuary) || is_affected(ch,gsn_protective_shield)) {
		send_to_char("As your rage washes over you, you lose control of your protective supplications.",ch);
		if(is_affected(ch,gsn_sanctuary))
			affect_strip(ch,gsn_sanctuary);
		if(is_affected(ch,gsn_protective_shield))
			affect_strip(ch,gsn_protective_shield);
	}
	
	for(obj = ch->carrying; obj; obj = obj->next_content)
		if(obj->wear_loc != WEAR_NONE && obj->wear_loc != WEAR_BRAND &&
		obj->wear_loc != WEAR_SHIELD && obj->wear_loc != WEAR_WIELD &&
		obj->wear_loc != WEAR_DUAL_WIELD && obj->wear_loc != WEAR_WAIST && obj->wear_loc != WEAR_ABOUT)
			unequip_char(ch,obj,TRUE);
	
	switch(ch->pcdata->tribe)
	{
		case TRIBE_BULL:
			af.location	= APPLY_STR;
			af.modifier	= 4;
			affect_to_char(ch, &af);
			af.location	= APPLY_CON;
			affect_to_char(ch, &af);
			af.location	= APPLY_DAMROLL;
			af.modifier	= (short)(60 * (float) ((float)ch->level /(float)51));
			affect_to_char(ch, &af);
			af.location	= APPLY_HITROLL;
			af.modifier	= (short)(20 * (float) ((float)ch->level /(float)51));
			affect_to_char(ch, &af);
			af.location	= APPLY_SAVING_SPELL;
			af.modifier	= (short)(-25 * (float) ((float)ch->level /(float)51));
			affect_to_char(ch, &af);
			af.location	= APPLY_SAVING_PARA;
			af.modifier	= (short)(-20 * (float) ((float)ch->level /(float)51));
			affect_to_char(ch, &af);
			af.location	= APPLY_DEFENSE;
			af.modifier	= 20;
			affect_to_char(ch, &af);
			af.location	= APPLY_DAM_MOD;
			af.modifier	= -25;
			affect_to_char(ch, &af);
		break;
		case TRIBE_BOAR:
			af.location	= APPLY_STR;
			af.modifier	= 2;
			affect_to_char(ch, &af);
			af.location	= APPLY_CON;
			af.modifier	= 2;
			affect_to_char(ch, &af);
			af.location	= APPLY_DAMROLL;
			af.modifier	= (short)(55 * (float) ((float)ch->level /(float)51));
			affect_to_char(ch, &af);
			af.location	= APPLY_HITROLL;
			af.modifier	= (short)(20 * (float) ((float)ch->level /(float)51));
			affect_to_char(ch, &af);
			af.location	= APPLY_SAVING_SPELL;
			af.modifier	= (short)(-20 * (float) ((float)ch->level /(float)51));
			affect_to_char(ch, &af);
			af.location	= APPLY_SAVING_PARA;
			af.modifier	= (short)(-15 * (float) ((float)ch->level /(float)51));
			affect_to_char(ch, &af);
			af.location	= APPLY_DEFENSE;
			af.modifier	= 10;
			affect_to_char(ch, &af);
			af.location	= APPLY_DAM_MOD;
			af.modifier	= -15;
			affect_to_char(ch, &af);
		break;
		case TRIBE_BEAR:
			af.location	= APPLY_STR;
			af.modifier	= 3;
			affect_to_char(ch, &af);
			af.location	= APPLY_CON;
			af.modifier	= 6;
			affect_to_char(ch, &af);
			af.location	= APPLY_DAMROLL;
			af.modifier	= (short)(55 * (float) ((float)ch->level /(float)51));
			affect_to_char(ch, &af);
			af.location	= APPLY_HITROLL;
			af.modifier	= (short)(20 * (float) ((float)ch->level /(float)51));
			affect_to_char(ch, &af);
			af.location	= APPLY_SAVING_SPELL;
			af.modifier	= (short)(-25 * (float) ((float)ch->level /(float)51));
			affect_to_char(ch, &af);
			af.location	= APPLY_SAVING_PARA;
			af.modifier	= (short)(-15 * (float) ((float)ch->level /(float)51));
			affect_to_char(ch, &af);
			af.location	= APPLY_DEFENSE;
			af.modifier	= 5;
			affect_to_char(ch, &af);
			af.location	= APPLY_DAM_MOD;
			af.modifier	= -20;
			affect_to_char(ch, &af);
		break;
		case TRIBE_LION:
			af.location	= APPLY_STR;
			af.modifier	= 3;
			affect_to_char(ch, &af);
			af.location	= APPLY_DEX;
			af.modifier	= 2;
			affect_to_char(ch, &af);
			af.location	= APPLY_DAMROLL;
			af.modifier	= (short)(45 * (float) ((float)ch->level /(float)51));
			affect_to_char(ch, &af);
			af.location	= APPLY_HITROLL;
			af.modifier	= (short)(20 * (float) ((float)ch->level /(float)51));
			affect_to_char(ch, &af);
			af.location	= APPLY_SAVING_SPELL;
			af.modifier	= (short)(-20 * (float) ((float)ch->level /(float)51));
			affect_to_char(ch, &af);
			af.location	= APPLY_SAVING_PARA;
			af.modifier	= (short)(-20 * (float) ((float)ch->level /(float)51));
			affect_to_char(ch, &af);
			af.location	= APPLY_DAM_MOD;
			af.modifier	= -20;
			affect_to_char(ch, &af);
		break;
		case TRIBE_PANTHER:
			af.location	= APPLY_STR;
			af.modifier	= 3;
			affect_to_char(ch, &af);
			af.location	= APPLY_DEX;
			af.modifier	= 3;
			affect_to_char(ch, &af);
			af.location	= APPLY_DAMROLL;
			af.modifier	= (short)(40 * (float) ((float)ch->level /(float)51));
			affect_to_char(ch, &af);
			af.location	= APPLY_HITROLL;
			af.modifier	= (short)(20 * (float) ((float)ch->level /(float)51));
			affect_to_char(ch, &af);
			af.location	= APPLY_SAVING_SPELL;
			af.modifier	= (short)(-20 * (float) ((float)ch->level /(float)51));
			affect_to_char(ch, &af);
			af.location	= APPLY_SAVING_PARA;
			af.modifier	= (short)(-20 * (float) ((float)ch->level /(float)51));
			affect_to_char(ch, &af);
			af.location	= APPLY_DAM_MOD;
			af.modifier	= -20;
			affect_to_char(ch, &af);
		break;
		case TRIBE_ELK:
			af.location	= APPLY_DEX;
			af.modifier	= 3;
			affect_to_char(ch, &af);
			af.location	= APPLY_CON;
			af.modifier	= 3;
			affect_to_char(ch, &af);
			af.location	= APPLY_DAMROLL;
			af.modifier	= (short)(40 * (float) ((float)ch->level /(float)51));
			affect_to_char(ch, &af);
			af.location	= APPLY_HITROLL;
			af.modifier	= (short)(20 * (float) ((float)ch->level /(float)51));
			affect_to_char(ch, &af);
			af.location	= APPLY_SAVING_SPELL;
			af.modifier	= (short)(-20 * (float) ((float)ch->level /(float)51));
			affect_to_char(ch, &af);
			af.location	= APPLY_SAVING_PARA;
			af.modifier	= (short)(-30 * (float) ((float)ch->level /(float)51));
			affect_to_char(ch, &af);
			af.location	= APPLY_DAM_MOD;
			af.modifier	= -15;
			affect_to_char(ch, &af);
		break;
		case TRIBE_JACKAL:
			af.location	= APPLY_STR;
			af.modifier	= 2;
			affect_to_char(ch, &af);
			af.location	= APPLY_DEX;
			af.modifier	= 4;
			affect_to_char(ch, &af);
			af.location	= APPLY_DAMROLL;
			af.modifier	= (short)(20 * (float) ((float)ch->level /(float)51));
			affect_to_char(ch, &af);
			af.location	= APPLY_HITROLL;
			af.modifier	= (short)(50 * (float) ((float)ch->level /(float)51));
			affect_to_char(ch, &af);
			af.location	= APPLY_SAVING_SPELL;
			af.modifier	= (short)(-30 * (float) ((float)ch->level /(float)51));
			affect_to_char(ch, &af);
			af.location	= APPLY_SAVING_PARA;
			af.modifier	= (short)(-20 * (float) ((float)ch->level /(float)51));
			affect_to_char(ch, &af);
			af.location	= APPLY_DAM_MOD;
			af.modifier	= -15;
			affect_to_char(ch, &af);
		break;
		case TRIBE_WOLF:
			af.location	= APPLY_STR;
			af.modifier	= 2;
			affect_to_char(ch, &af);
			zero_vector(af.bitvector);
			af.location	= APPLY_DEX;
			af.modifier	= 3;
			affect_to_char(ch, &af);
			af.location	= APPLY_DAMROLL;
			af.modifier	= (short)(40 * (float) ((float)ch->level /(float)51));
			affect_to_char(ch, &af);
			af.location	= APPLY_HITROLL;
			af.modifier	= (short)(50 * (float) ((float)ch->level /(float)51));
			affect_to_char(ch, &af);
			af.location	= APPLY_SAVING_SPELL;
			af.modifier	= (short)(-20 * (float) ((float)ch->level /(float)51));
			affect_to_char(ch, &af);
			af.location	= APPLY_SAVING_PARA;
			af.modifier	= (short)(-20 * (float) ((float)ch->level /(float)51));
			affect_to_char(ch, &af);
			af.location	= APPLY_DEFENSE;
			af.modifier	= -10;
			affect_to_char(ch, &af);
			af.location	= APPLY_DAM_MOD;
			af.modifier	= -20;
			affect_to_char(ch, &af);
		break;
		case TRIBE_HAWK:
			af.location	= APPLY_STR;
			af.modifier	= 2;
			affect_to_char(ch, &af);
			af.location	= APPLY_DEX;
			af.modifier	= 4;
			affect_to_char(ch, &af);
			af.location	= APPLY_DAMROLL;
			af.modifier	= (short)(30 * (float) ((float)ch->level /(float)51));
			affect_to_char(ch, &af);
			af.location	= APPLY_HITROLL;
			af.modifier	= (short)(50 * (float) ((float)ch->level /(float)51));
			affect_to_char(ch, &af);
			af.location	= APPLY_SAVING_SPELL;
			af.modifier	= (short)(-20 * (float) ((float)ch->level /(float)51));
			affect_to_char(ch, &af);
			af.location = APPLY_SAVING_PARA;
			af.modifier = (short)(-30 * (float) ((float)ch->level /(float)51));
			affect_to_char(ch, &af);
			af.location	= APPLY_DAM_MOD;
			af.modifier	= -15;
			affect_to_char(ch, &af);
		break;
		case TRIBE_FOX:
			af.location	= APPLY_STR;
			af.modifier	= 2;
			affect_to_char(ch, &af);
			af.location	= APPLY_DEX;
			af.modifier	= 5;
			affect_to_char(ch, &af);
			af.location	= APPLY_DAMROLL;
			af.modifier	= (short)(20 * (float) ((float)ch->level /(float)51));
			affect_to_char(ch, &af);
			af.location	= APPLY_HITROLL;
			af.modifier	= (short)(55 * (float) ((float)ch->level /(float)51));
			affect_to_char(ch, &af);
			af.location	= APPLY_SAVING_SPELL;
			af.modifier	= (short)(-30 * (float) ((float)ch->level /(float)51));
			affect_to_char(ch, &af);
			af.location	= APPLY_SAVING_PARA;
			af.modifier	= (short)(-20 * (float) ((float)ch->level /(float)51));
			affect_to_char(ch, &af);
			af.location	= APPLY_DAM_MOD;
			af.modifier	= -10;
			affect_to_char(ch, &af);
		break;
		default:
		break;
	}
}
void spell_scourge(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
        CHAR_DATA *vch;
        char buf[MAX_STRING_LENGTH];

        for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
        {
        if (is_same_group(vch,ch) || is_safe(ch,vch) || is_same_cabal(ch,vch))
            continue;
        if (!IS_NPC(ch) && !IS_NPC(vch) && (ch->fighting == NULL || vch->fighting == NULL))
        {
                sprintf(buf,"Die, %s you scourging dog!",PERS(ch,vch));
                do_myell(vch,buf, ch);
        }
        if(number_percent()>30)
                spell_blindness(skill_lookup("blindness"),level-4,ch,vch,TAR_CHAR_OFFENSIVE);
        if(number_percent()>30)
                spell_poison(skill_lookup("poison"),level-4,ch,vch,TAR_CHAR_OFFENSIVE);
        if(number_percent()>30)
                spell_weaken(skill_lookup("weaken"),level-4,ch,vch,TAR_CHAR_OFFENSIVE);
        }
        spell_fireball(sn,level-2,ch,NULL,TAR_CHAR_OFFENSIVE);
        return;
}

void spell_hunters_vision(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	if(is_affected(ch,sn))
		return send_to_char("You already see with the guile of a predator.\n\r",ch);
	init_affect(&af);
	af.where	= TO_AFFECTS;
	af.aftype	= AFT_POWER;
	af.type		= sn;
	af.modifier	= 0;
	af.location	= 0;
	SET_BIT(af.bitvector, AFF_DETECT_INVIS);
	SET_BIT(af.bitvector, AFF_DETECT_HIDDEN);
	SET_BIT(af.bitvector, AFF_DETECT_CAMO);
	af.level	= level;
	af.duration	= 10;
	af.mod_name = MOD_VISION;
	affect_to_char(ch,&af);
	send_to_char("You begin to see the world through the undeceiving eyes of a hunter.\n\r",ch);
}
void spell_hire_mercenary(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	CHAR_DATA *merc;
	char buf[350];
	int merc_vnum=-1, amount = 0;
	if(is_affected(ch,sn))
		return send_to_char("You cannot hire another mercenary yet.\n\r",ch);
	if(!str_cmp(target_name,""))
	{
		send_to_char("You can call the following mercenaries:\n\r",ch);
		if(ch->pcdata->bounty_credits>=bounty_table[0].credits_required)
			send_to_char("warrior       - 2500 gold\n\r",ch);
                if(ch->pcdata->bounty_credits>=bounty_table[1].credits_required)
                        send_to_char("thief         - 7500 gold\n\r",ch);
                if(ch->pcdata->bounty_credits>=bounty_table[2].credits_required)
                        send_to_char("assassin      - 25000 gold\n\r",ch);
                if(ch->pcdata->bounty_credits>=bounty_table[3].credits_required)
                        send_to_char("healer        - 10000 gold\n\r",ch);
		return;
	}
	for(merc=char_list;merc!=NULL;merc=merc->next)
		if(IS_NPC(merc) && merc->pIndexData->vnum>=MOB_VNUM_WARRIOR_MERCENARY &&
		merc->pIndexData->vnum<=MOB_VNUM_SHAMAN_MERCENARY && merc->master==ch)
			break;
	if(merc)
		return send_to_char("You already have a mercenary.\n\r",ch);
	if(!str_cmp(capitalize(target_name),"Warrior") && ch->pcdata->bounty_credits>=bounty_table[0].credits_required) {
		merc_vnum=MOB_VNUM_WARRIOR_MERCENARY;
		amount = 2500;
	}
        if(!str_cmp(capitalize(target_name),"Thief") && ch->pcdata->bounty_credits>=bounty_table[1].credits_required){
			merc_vnum=MOB_VNUM_THIEF_MERCENARY;
			amount = 7500;
		}
        if(!str_cmp(capitalize(target_name),"Assassin") && ch->pcdata->bounty_credits>=bounty_table[2].credits_required) {
			merc_vnum=MOB_VNUM_ASSASSIN_MERCENARY;
			amount = 25000;
		}
        if(!str_cmp(capitalize(target_name),"Healer") && ch->pcdata->bounty_credits>=bounty_table[3].credits_required) {
			merc_vnum=MOB_VNUM_SHAMAN_MERCENARY;
			amount = 10000;
		}
	if(merc_vnum<0)
		return send_to_char("Invalid mercenary choice.\n\r",ch);

	merc=create_mobile(get_mob_index(merc_vnum));

	if(amount>ch->gold)
		return send_to_char("You don't have enough gold to hire that mercenary.\n\r",ch);

	sprintf(buf,"The mercenary's guild charges you %i gold for your hired mercenary.\n\r",amount);	
	act("$n hires $N.",ch,0,merc,TO_ROOM);

	send_to_char(buf,ch);

	ch->gold-=amount;

	char_to_room(merc,ch->in_room);
	add_follower(merc, ch );

	merc->alignment=ch->alignment;
	merc->level = ch->level;
	merc->max_hit = ch->max_hit * 2;
	merc->hit = merc->max_hit;
	merc->damage[DICE_NUMBER] = ch->level/4;
	merc->damage[DICE_TYPE] = ch->level/10;
	merc->damage[DICE_BONUS] = ch->level/2;

	if(merc_vnum!=MOB_VNUM_SHAMAN_MERCENARY)
		merc->leader=ch;

	SET_BIT(merc->affected_by,AFF_CHARM);
	do_gtell(merc,"Reporting for duty.");

	init_affect(&af);
	af.where	= TO_AFFECTS;
	af.aftype	= AFT_POWER;
	af.type		= sn;
	af.location	= APPLY_NONE;
	af.modifier	= 0;
	af.duration	= 50;
	af.level	= level;
	affect_to_char(ch,&af);
}

void spell_hunters_strength(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	float bcr;

	if(is_affected(ch,sn) || IS_NPC(ch))
		return send_to_char("You already have the enhanced strength of a hunter.\n\r",ch);

	init_affect(&af);
	af.where	= TO_AFFECTS;
	af.aftype	= AFT_POWER;
	af.type		= sn;
	af.location	= APPLY_STR;
	af.modifier	= ch->pcdata->cabal_level + 2;
	af.level	= level;
	af.duration	= (get_skill(ch,sn)-60);
	affect_to_char(ch,&af);
	af.location	= APPLY_DAMROLL;
	bcr = sqrt(ch->pcdata->bounty_credits) / 2;
	af.modifier	= (int)(ch->level/7 + URANGE(1,bcr,31));
	affect_to_char(ch,&af);
	send_to_char("You feel your body surge with the newfound strength of a hunter seeking his prey!\n\r",ch);
	act("$n seems to move with a newfound strength and agility.",ch,0,0,TO_ROOM);
}
char * get_room_description(ROOM_INDEX_DATA *room);

void spell_hunters_awareness(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim;
	AFFECT_DATA af;

	if(IS_NPC(ch) || IS_SWITCHED(ch))
		return;

	if(is_affected(ch, sn) || (ch->pcdata->cabal_level == 5 && ch->mana < 100))
		return send_to_char("You are too drained to look for your prey again.\n\r", ch);
	
	if(!str_cmp(target_name,""))
		return send_to_char("Syntax: call 'hunters awareness' <target>\n\r",ch);

	if(!(victim = get_char_world(ch,target_name)) || IS_NPC(victim))  
		return send_to_char("You are unable to find them.\n\r",ch);

	if(ch->in_room == victim->in_room)
		return send_to_char("They are right in front of you!\n\r", ch);
	
	if(is_adj_range(victim->in_room->area, ch->in_room->area, UMIN(ch->pcdata->cabal_level + 1,4)))
	{
		if(ch->pcdata->cabal_level == 5)
		{
			ch->mana -= 100;
			act("You stalk $N relentlessly, persuing $M with all the tools at your disposal.",ch,0,victim,TO_CHAR);
			if(saves_spell(level + 8,victim,DAM_OTHER))
				return act("You are unable to get an accurate fix on $N.",ch,0,victim,TO_CHAR);
			act("You manage to get an accurate perception of $N's surroundings.",ch,0,victim,TO_CHAR);
			send_to_char(get_room_description(victim->in_room),ch);
			send_to_char("\n\r",ch);
		}
		else
		{
			send_to_char("You can smell the sweat from your prey nearby.\n\r", ch);
			if(number_percent() > 85)
			{
				char *rtitle = get_room_name(victim->in_room);
				ROOM_INDEX_DATA *dRoom = get_room_index(number_range(victim->in_room->vnum + 30, 
										     victim->in_room->vnum - 30));
				if(number_percent() > 30 && dRoom)
					rtitle = get_room_name(dRoom);
				act("Your instincts lead you to believe $N might be in $t.", ch, rtitle, victim, TO_CHAR);
			}
		}
	}
	else
		send_to_char("You gather no scent of your prey in the immediate area.\n\r", ch);

	init_affect(&af);
	af.where    = TO_AFFECTS;
	af.aftype   = AFT_INVIS;
	af.type     = sn;
	af.level    = level;
	af.duration = (6 - ch->pcdata->cabal_level) * 2;
	if(ch->pcdata->cabal_level == 5)
		af.duration = 8;
	affect_to_char(ch,&af);	
	return;
}

void spell_web(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
   	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	if(is_safe(ch,victim))
		return;

	if(saves_spell(level+5,victim,DAM_OTHER) || number_range(20,35)<get_curr_stat(victim,STAT_DEX))
	{
		act("$n tries to entangle you in a sticky web, but you avoid it.",ch,0,victim,TO_VICT);
		act("$n tries to entangle $N in a sticky web, but $E avoids it.",ch,0,victim,TO_NOTVICT);
		act("You try to entangle $N, but $E avoids it.",ch,0,victim,TO_CHAR);
		return;
	}
	act("$n releases a sticky web at you, impeding your mobility!",ch,0,victim,TO_VICT);
	act("$n releases a sticky web at $N!",ch,0,victim,TO_NOTVICT);
	act("You release a sticky web at $N!",ch,0,victim,TO_CHAR);
	victim->move/=2;
	damage(ch,victim,number_range(30,50),sn,DAM_ACID,TRUE);
	init_affect(&af);
	af.where	= TO_AFFECTS;
	af.type		= sn;
	af.aftype	= AFT_MALADY;
	af.level	= level;
	af.location	= APPLY_DEX;
	af.modifier	= -(number_range(7,15));
	af.duration	= 3;
	if(!is_affected(victim,sn))
		affect_to_char(victim,&af);
}
void spell_informant(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim;
	int amount;
	char info[500];

	if(!str_cmp(target_name,"") || !(victim=get_char_world(ch,target_name)))
		return send_to_char("Your informants are unable to find them.\n\r",ch);

	if(saves_spell(level+8,victim,DAM_OTHER) || IS_NPC(victim) || IS_IMMORTAL(victim))
		return act("Your spies are unable to locate $N.",ch,0,victim,TO_CHAR);

	amount=number_range(25,50);

	if(victim->pcdata->bounty)
		amount/=2;

	if(ch->gold<amount)
		return send_to_char("An informant scoffs at your meager payment.\n\r",ch);

	ch->gold-=amount;
	
	if(IS_SET(ch->comm,COMM_ANSI))
		sprintf(info,"An informant whispers to you '\x01B[1;31mI saw %s in %s.\x01B[0;37m'\n\r",victim->name,get_room_name(victim->in_room));
	else
		sprintf(info,"An informant whispers to you 'I saw %s in %s.'\n\r",victim->name,get_room_name(victim->in_room));

	send_to_char(info,ch);

	sprintf(info,"An informant requires %i gold for his report.\n\r",amount);
	send_to_char(info,ch);
}


void do_howl(CHAR_DATA *ch,char *argument)
{
	CHAR_DATA *vch,*vch_next;
	int exits = 0,dir;
	EXIT_DATA *pexit;
        if (get_skill(ch,gsn_howl) == 0)
        {
                send_to_char("Huh?\n\r",ch);
                return;
        }
	if(cabal_down(ch,CABAL_HORDE))
		return;
	if (ch->mana < 30)
	 return send_to_char("You do not have the strength to howl.\n\r", ch);
	else
	 ch->mana -= 30;

	if (!ch->fighting)
	  return send_to_char("You must be fighting to howl!\n\r",ch);

	for (dir = 0; dir <= 5; dir++)
	{
		if ((pexit = ch->in_room->exit[dir]) != NULL && !IS_SET(pexit->exit_info,EX_CLOSED))
			exits++;
	}

        if (number_percent() > get_skill(ch,gsn_howl) || exits == 0)
        {
        	act("$n opens $s mouth to scream, but his voice sounds stripped.",ch,0,0,TO_ROOM);
        	send_to_char("Your howl seems to be rather weak.\n\r",ch);
        	check_improve(ch,gsn_howl,FALSE,2);
        }
	else
	{
		act("$n opens $s mouth and startles you with an animal-like howl!",ch,0,0,TO_ROOM);
		send_to_char("You scream at the top of your lungs like an animal!\n\r",ch);
		check_improve(ch,gsn_howl,TRUE,2);
		/* Loop through all the people in the room */
		for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
		{
			vch_next = vch->next_in_room;
			if(vch!=ch && !check_leadership_save(vch, gsn_howl) && 
					vch->fighting==ch && (!IS_NPC(vch) || (IS_NPC(vch) && !IS_SET(vch->act,ACT_SENTINEL))))
			{
				act("$n looks frightened and tries to run!",vch,0,0,TO_ROOM);
				do_flee(vch,"");
				
			}
		}
	}

        WAIT_STATE(ch,PULSE_VIOLENCE*2);
}

void spell_mana_transfer( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int mod = (ch->level * 2);

    if(ch == victim)
        return send_to_char("You can't cast this on yourself.\n\r", ch);
	
	act("You transfer part of your energy to $N!", ch, 0, victim, TO_CHAR);
	act("$n transfers part of $s energy to you!", ch, 0, victim, TO_VICT);
	act("$n tranfers part of $s energy to $N!", ch, 0, victim, TO_NOTVICT);

	damage_new(ch,ch,mod,sn,DAM_MENTAL,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"mana tranfer");

	victim->mana += mod;
	if(victim->mana > victim->max_mana)
		victim->mana = victim->max_mana;

	return;
}

void spell_scribe( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	int skill, cost, ipower;
	OBJ_DATA *obj;
	char spell_name[MSL],power[MSL];
	char *argument;

	argument = one_argument(target_name, spell_name);
	argument = one_argument(argument, power);

	if((skill = skill_lookup(spell_name)) < 1)
		return send_to_char("That isn't a spell you can scribe!",ch);

	if(skill_table[skill].ctype != CMD_SPELL && skill_table[skill].ctype != CMD_BOTH)
		return send_to_char("You can't scribe a skill!", ch);

	for (obj = ch->carrying; obj != NULL; obj = obj->next_content)
		if(obj->pIndexData->vnum == OBJ_VNUM_SCRIBE && obj->item_type == ITEM_TRASH)
			break;

	if(obj == NULL)
		return act("You do not have a blank scroll to scribe on.", ch, 0, 0, TO_CHAR);

	if(!is_number(power))
		ipower = 0;
	else
		ipower = atoi(power);

	cost = skill_table[skill].min_mana + ipower;

	if((ch->mana - cost) < 0)
		return act("You do not have the mana to scribe a scroll.", ch, 0, 0, TO_CHAR);
	else
		ch->mana -= cost;

	if(get_skill(ch, skill) == 0)
		return act("You are not even learned in $t.", ch, skill_table[skill].name, 0, TO_CHAR);

	if(get_skill(ch, skill) < 90)
		return act("You are not sufficiently learned in $t to properly scribe it.",ch,skill_table[skill].name,0,TO_CHAR);

	if(skill_table[skill].skill_level[ch->Class()->GetIndex()] >= 30)
		return act("That spell is too high level to be scribed!", ch, 0, 0, TO_CHAR);

	obj->level = 4 + (level/2) + (ipower / 200);
	obj->item_type = ITEM_SCROLL;
	obj->value[0] = 4 + (level/2) + (ipower / 100);
	obj->value[1] = skill;
	obj->value[2] = -1;
	obj->value[3] = -1;
	obj->value[4] = -1;
	free_pstring(obj->short_descr);
	obj->short_descr = palloc_string("a scroll with arcane markings");
	free_pstring(obj->description);
	obj->description = palloc_string("You see a scroll marked with some strange arcane symbols.");
	act("You scribe the $t spell onto the scroll!", ch, skill_table[skill].name,0, TO_CHAR);
	act("$n makes detailed inscriptions on a scroll in $s possession.", ch, skill_table[skill].name, 0, TO_ROOM);
	return;
}

void spell_deny_magic( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	if(ch == victim)
		return send_to_char("You can't cast this on yourself.\n\r", ch);

	act("You gesture at $N and $E is surrounded by a faint blue aura.", ch, 0, victim, TO_CHAR);
	act("$n gestures at you, and a faint blue aura forms around you.", ch, 0, victim, TO_VICT);
	act("$n gestures at $N, and a faint blue aura forms around $M.", ch, 0, victim, TO_NOTVICT);

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.duration = ch->level / 5;
	af.type = gsn_deny_magic;
	af.level = level;
	af.aftype = AFT_POWER;
	af.owner = ch;
	new_affect_to_char(victim,&af);

	return;
}

bool check_deny_magic(CHAR_DATA *ch)
{
	if(!is_affected(ch, gsn_deny_magic))
		return FALSE;

	act("The light blue aura around you flares and your magic does nothing.", ch, 0, 0, TO_CHAR);
	act("The light blue aura around $n flares.", ch, 0, 0, TO_ROOM);

	return TRUE;
}

void spell_bane( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int damage;

	if(IS_SET(victim->act, PLR_IGNORANT)) {
		damage = 3 * dice(level,3);
		act("Your mental assault tears at $N's ignorance!", ch, 0, victim, TO_CHAR);
		act("$n's mental assault tears at your ignorance!", ch, 0, victim, TO_VICT);
		act("$n's mental assault tears at $N's ignorance!", ch, 0, victim, TO_NOTVICT);
	}
	else
		damage = 2 * dice(level,2);

	damage_new(ch, victim, damage, gsn_bane, DAM_MENTAL, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT,"mental assault");
	
}

void spell_repose( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	AFFECT_DATA af;
	
	if(is_affected(ch, gsn_repose))
	{
		act("You are already able to rest quickly.", ch, 0, 0, TO_CHAR);
	}
	else
	{
		init_affect(&af);
		af.where = TO_AFFECTS;
		af.aftype = AFT_POWER;
		af.type = gsn_repose;
		af.duration = ch->level;
		af.mod_name = MOD_MOVEMENT;
		new_affect_to_char(ch, &af);
		act("You feel lighter on your feet.", ch, 0, 0, TO_CHAR);
	}
}

void spell_medicine( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	CHAR_DATA *vch;
	AFFECT_DATA af;
	
	if(is_affected(ch, gsn_medicine))
	{
		act("You cannot apply medicine again so soon.", ch, 0, 0, TO_CHAR);
		return;
	}

	for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
	{

		if(is_affected(vch, gsn_medicine) || !is_same_group(ch, vch))
			continue;

		init_affect(&af);
    	af.where = TO_AFFECTS;                                                                               
    	af.aftype = AFT_POWER;                                                                                
    	af.type = gsn_medicine;                                                                      
    	af.duration = 4;                                                                                       
		af.owner = ch;                                                                                       
    	af.level = level;                                                                                    
    	new_affect_to_char(vch,&af);    
	
		if(vch == ch)
		{
			act("You apply medicine to your wounds.", ch, 0, 0, TO_CHAR);
			act("$n applies medicine to $s wounds.", ch, 0, 0, TO_ROOM);
		}
		else
		{
			act("You apply medicine to $N's wounds.", ch, 0, vch, TO_CHAR);
			act("$n applies medicine to your wounds.", ch, 0, vch, TO_VICT);
			act("$n applies medicine to $N's wounds.", ch, 0, vch, TO_NOTVICT);
		}
	}
	
}

void spell_horde_communion(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	if (IS_NPC(ch) && (!ch->desc || !ch->desc->original))
		return;
	if (IS_NPC(ch))
		ch = ch->desc->original;

	if (is_affected(victim, gsn_horde_communion))
		return send_to_char("They cannot try the communion ritual again right now.\n\r",ch);

	if (ch == victim)
		return send_to_char("You have already undergone the sacred communion!\n\r",ch);

	if ((ch->cabal != CABAL_HORDE || ch->pcdata->induct != CABAL_LEADER) && !IS_IMMORTAL(ch))
		return send_to_char("It is not your place to call upon the spirits thus!\n\r",ch);

	if (IS_NPC(victim))
		return send_to_char("You profane the spirits with such notions.\n\r",ch);

	if (victim->cabal == CABAL_HORDE)
		return send_to_char("The spirits will only answer the call once in each person's lifetime.\n\r",ch);
	
	if (victim->cabal != CABAL_NONE)
		return send_to_char("That one has other allegiances already!",ch);

	if (victim->level < 15
		|| IS_LAWFUL(victim)
		|| (victim->Class()->name == "sorcerer")
		|| (victim->Class()->name == "paladin")
		|| (victim->Class()->name == "necromancer")
		|| (victim->Class()->name == "healer")
		|| (victim->Class()->name == "shapeshifter")
		|| (victim->Class()->name == "chronomancer"))
		return act("$N is not fit to partake in the rites of communion.",ch,0,victim,TO_CHAR);

	if(cabal_table[CABAL_HORDE].max_members > 1 
	   && cabal_max[CABAL_HORDE] <= cabal_members[CABAL_HORDE] 
	   && get_trust(ch)<54)
		return send_to_char("The Horde is as large as it can possibly be.\n\r", ch);	
	
	act("You prepare $N for communion with the animal spirits of Horde.",ch,0,victim,TO_CHAR);
	act("A tranquil calm washes over you as your skin tingles slightly.",ch,0,victim,TO_VICT);
	act("A gentle breeze sweeps through, as an eerie silence falls over the area.",ch,0,victim,TO_NOTVICT);

	init_affect(&af);
	af.where		=	TO_AFFECTS;
	af.aftype		=	AFT_POWER;
	af.type			=	gsn_horde_communion;
	af.location		=	0;
	af.modifier		=	0;
	af.duration		=	-1;
	af.owner		=	ch;
	af.level		=	level;
	af.tick_fun		=	communion_tick;
	new_affect_to_char(victim,&af);
	af.duration		=	24;
	af.tick_fun		=	NULL;
	new_affect_to_char(ch,&af);
	return;
}

void communion_tick (CHAR_DATA *ch, AFFECT_DATA *af)
{
	if (af->modifier > 0)
		return;

	if (ch->in_room->vnum != 23628)
		return send_to_char("You feel a deep and powerful calling toward the sacred shrine of the Horde.\n\r",ch);

	send_to_char("You feel an urge to reach out and touch the obelisk before you.\n\r",ch);
	
	return;
}

void do_exile( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *remove;

    if (IS_NPC(ch))
	    return;

    argument = one_argument( argument, arg1 );

	if ((ch->level < 54 && ch->pcdata->induct != CABAL_LEADER) || IS_NPC(ch)
		|| ch->cabal != CABAL_HORDE)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	if((victim = get_char_world(ch, arg1)) == NULL)
		return send_to_char("You can't seem to find them.\n\r", ch);

	if(victim->cabal != CABAL_HORDE)
		return send_to_char("They aren't in the Horde.\n\r", ch);

	act("You exile $N from the Horde and mark $M as a betrayer to you and your brethren!", ch, 0, victim, TO_CHAR);
	act("You have been exiled from the Longhouse and marked a betrayer of your brethren!", ch, 0, victim, TO_VICT);
	victim->cabal = CABAL_NONE;
	update_cskills(victim);
	if ((remove = get_eq_char(victim,WEAR_WAIST)) != NULL)
	{
		obj_from_char(remove);
		extract_obj(remove);
	}
	if((remove = get_eq_char(victim,WEAR_ABOUT)) != NULL)
	{
		obj_from_char(remove);
		extract_obj(remove);
	}
	free_pstring(victim->pcdata->extitle);
	victim->pcdata->extitle = palloc_string(", Exiled from the Horde");
	
	return;
}

void spell_piety(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
	AFFECT_DATA af;
	char buf[MSL];

	
	if(IS_NPC(ch))
		return;

	if(is_affected(ch,gsn_piety))
		return send_to_char("Your righteous wrath already infuses you with vigor.\n\r",ch);
	
	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = gsn_piety;
	af.level = level;
	af.aftype = AFT_POWER;
	af.location = APPLY_AC;
	af.modifier = level / 4;
	af.duration = level / 5;
	affect_to_char(ch, &af);

	act("Your righteous wrath at the impure surges through you.", ch, 0, 0, TO_CHAR);
	act("$n stands taller as $s righteous wrath infuses $m.",ch,0,0,TO_ROOM);
}

void spell_fervor(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	
	if(is_affected(ch,gsn_fervor))
		return send_to_char("You are already filled with holy fervor!\n\r",ch);

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.level = level;
	af.duration = level/5;
	af.type = gsn_fervor;
	af.aftype = AFT_POWER;
	af.modifier = -level/6;
	af.location = APPLY_SAVING_SPELL;
	SET_BIT(af.bitvector,AFF_DETECT_EVIL);
	SET_BIT(af.bitvector,AFF_DETECT_GOOD);
	af.mod_name = MOD_VISION;
	new_affect_to_char(ch,&af);
	af.location = 0;
	af.modifier = 0;
	zero_vector(af.bitvector);
	af.mod_name = MOD_PROTECTION;
	SET_BIT(af.bitvector,AFF_PROTECTION);
	new_affect_to_char(ch,&af);

	act("$n's eyes blaze with light for a moment before fading to a dull glow.",ch,0,0,TO_ROOM);
	act("You feel your spirit rejoice as holy fervor enters you.",ch,0,0,TO_CHAR);
	return;
}
	
void spell_spiritual_healing(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	
	if(is_affected(ch,gsn_spiritual_healing))
		return send_to_char("You cannot provide more healing yet.\n\r",ch);

	if(!IS_GOOD(victim)) {
		act("$N is too impure!",ch,0,victim,TO_CHAR);
		return;
	}

	victim->hit = UMIN(victim->max_hit, victim->hit + dice(ch->level,3));
	if(ch != victim) {
		act("$n utters a quiet prayer and places $s hand on your forehead.",ch,0,victim,TO_VICT);
		act("You quietly say a prayer, placing your hand on $N's forehead.",ch,0,victim,TO_CHAR);
		act("$n places a hand on $N's forehead.",ch,0,victim,TO_NOTVICT);
	} else {
		act("You pray quietly for healing.",ch,0,victim,TO_CHAR);
		act("$n utters a quiet prayer.",ch,0,0,TO_ROOM);
	}

	if(check_dispel(ch->level,victim,gsn_poison)) {
		send_to_char("Your fever subsides as the poison leaves your body.\n\r",ch);
		act("$n looks better.",ch,0,0,TO_ROOM);
	}
	if(check_dispel(ch->level,victim,gsn_plague)) {
		send_to_char("Your plague sores vanish.\n\r",ch);
		act("$n looks relieved as his sores vanish.",ch,0,0,TO_ROOM);
	}
	if(check_dispel(ch->level,victim,gsn_blindness)) {
		send_to_char("You can see again.\n\r",ch);
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = gsn_spiritual_healing;
	af.aftype = AFT_TIMER;
	af.duration = 5;
	af.level = level;
	new_affect_to_char(ch,&af);
	
	return;
}

void spell_shroud_of_light(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;


        if(is_affected(ch,sn))
                return send_to_char("You are already shrouded in light.\n\r",ch);
	
	if(ch->fighting)
		return send_to_char("You are not able to shroud yourself while you fight.\n\r", ch);

	act("A feeling of warmth fills your body as you become shrouded in crackling light.", ch, 0, 0, TO_CHAR);
	act("A veil of sparkling luminescence crackles into existence around $n.", ch, 0, 0, TO_ROOM);

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_POWER;
	af.type = gsn_shroud_of_light;
	af.level = level;
	af.duration = level/4;
	af.location = APPLY_DAM_MOD;
	af.modifier = -10;
	af.mod_name = MOD_PROTECTION;
	affect_to_char(ch,&af);

}

void spell_crimson_martyr(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = ch->fighting, *vch, *vch_next;
	AFFECT_DATA af;
	for(vch = ch->in_room->people; vch; vch = vch->next_in_room)
		if(IS_EVIL(vch) && !is_safe_new(ch,vch,FALSE))
			break;
	if(ch->hit < ch->max_hit * .06)
		return send_to_char("You are too weak to perform your sacrifice.\n\r",ch);
	if(!victim || !IS_EVIL(victim))
		victim = vch;
	if(!victim)
		return send_to_char("You can only call upon your martyrdom in the presence of evil to be purged.\n\r",ch);
	
	act("Raising $s arms to the heavens in supplication, $n calls down judgement upon the impure!",ch,0,0,TO_ROOM);
	act("$n blazes with blinding incandescence as the wrath of the Light falls upon you!",ch,0,0,TO_ROOM);
	act("Raising your arms to the heavens in supplication, you call down judgement upon the impure!",ch,0,0,TO_CHAR);
	act("You feel the infinite power of the Light surge through you, burning you alive!",ch,0,0,TO_CHAR);
	for(vch = ch->in_room->people; vch; vch = vch_next)
	{
		vch_next = vch->next_in_room;
		if(!IS_GOOD(vch) && !is_safe_new(ch,vch,FALSE))
		{
			damage_new(ch, vch, (int)((float)ch->hit * .35), sn, DAM_TRUESTRIKE, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, NULL);
			LAG_CHAR(vch, PULSE_VIOLENCE * 2);
		}
	}
	WAIT_STATE(vch, PULSE_VIOLENCE * 2);
	init_affect(&af);
	af.where	= TO_AFFECTS;
	af.type		= sn;
	af.level	= level;
	af.location = APPLY_DEX;
	af.modifier = 4;
	af.duration = 2;
	SET_BIT(af.bitvector, AFF_HASTE);
	
	for(vch = ch->in_room->people; vch; vch = vch->next_in_room)
		if(IS_GOOD(vch) && vch != ch)
		{
			affect_to_char(vch, &af);
			vch->hit = UMIN(vch->max_hit, vch->hit + (level * 10 * (ch->hit / ch->max_hit)));
			act("$n's sacrifice infuses you with newfound vigor!",ch,0,vch,TO_VICT);
		}

	act("$n slumps to the ground, crimson blood streaming from $s body.",ch,0,0,TO_ROOM);
	send_to_char("You fall to the ground as the searing ravages of your martyrdom take their toll.\n\r",ch);
	stop_fighting(ch, TRUE);
	ch->hit = (int)((float)ch->max_hit * .05);
}

void retribution_tick (CHAR_DATA *ch, AFFECT_DATA *af)
{
	if(af->modifier == 0 || !af->owner || is_safe(af->owner, ch))
		return;
	float dam = af->modifier / (af->duration == 0 ? 1 : af->duration);
	send_to_char("Pain courses through your body as zealous retribution is extracted upon you!\n\r",ch);
	damage_new(af->owner, ch, (int)dam, gsn_retribution, DAM_HOLY, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, 
			HIT_NOMULT, "the retribution of the Phalanx*");
	af->modifier -= (int)dam;
}

void spell_retribution(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;
	if(is_affected(victim,sn) || is_affected(ch,sn))
		return send_to_char("You cannot call further vengeance upon them.\n\r",ch);
	if(IS_GOOD(victim))
		return send_to_char("They are not an acceptable target for your vengeance.\n\r",ch);
	init_affect(&af);
	af.location = APPLY_NONE;
	af.aftype	= AFT_POWER;
	af.duration = 8;
	af.modifier	= 0;
	af.level	= level;
	af.type		= sn;
	af.tick_fun = retribution_tick;
	af.owner	= ch;
	affect_to_char(victim, &af);
	act("You unleash the vengeance of the Phalanx upon $N!",ch,0,victim,TO_CHAR);
	act("$n unleashes the vengeance of the Phalanx upon $N!",ch,0,victim,TO_NOTVICT);
	act("$n unleashes the vengeance of the Phalanx upon you!",ch,0,victim,TO_VICT);
	af.tick_fun = NULL;
	af.duration	= level - 5;
	af.aftype	= AFT_TIMER;
	affect_to_char(ch, &af);
}

void do_phalanx(CHAR_DATA *ch, char *argument)
{
        char arg1[MSL];
        CHAR_DATA *victim;

        argument = one_argument(argument, arg1);

		if (ch->cabal != CABAL_PHALANX && !IS_IMMORTAL(ch) && ch->pcdata->induct == CABAL_LEADER)
			return send_to_char("Huh?\n\r", ch);
		
        if (argument[0] == '\0' || arg1[0] == '\0')
                return send_to_char("Syntax: phalanx <character> <keeper/inquisitor>\n\r",ch);

        if((victim = get_char_world(ch,arg1)) == NULL)
                return send_to_char("You cannot find that person.\n\r", ch);

		if(victim->cabal != CABAL_PHALANX)
				return send_to_char("That person isn't a member of the Phalanx!\n\r", ch);
		
		if(victim->pcdata->cabal_level > 1 && get_trust(ch) < 59)
				return send_to_char("That person already has already been assigned.\n\r", ch);
		
        if(!str_cmp(argument, "keeper"))
        {
				victim->pcdata->cabal_level = 2;
				update_cskills(victim);
				act("$N is now a Keeper of the Fire.", ch, 0, victim, TO_CHAR);
				act("You are now a Keeper of the Fire.", ch, 0, victim, TO_VICT);
        }
        else if(!str_cmp(argument, "inquisitor"))
        {
				victim->pcdata->cabal_level = 3;
				update_cskills(victim);
				act("$N is now an Inquisitor of the Fire.", ch, 0, victim, TO_CHAR);
				act("You are now an Inquisitor of the Fire.", ch, 0, victim, TO_VICT);
		}
        else
                send_to_char("That's not a valid option.\n\r",ch);

        return;
}

void spell_safehaven(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	
	if(victim->hit < victim->max_hit*.7) {
		if(ch == victim) {
			return send_to_char("You must be healthier to return to the safe haven.\n\r",ch);
		} else {
			act("$N is not healthy enough to return to the safe haven.",ch,0,victim,TO_CHAR);
			return;
		}
	}

	if(ch == victim) {
		act("You call upon the power of Light to transport yourself to the safe haven.",ch,0,0,TO_CHAR);
		act("$n disappears in a flash of light.",ch,0,0,TO_ROOM);
		char_from_room(ch);
		char_to_room(ch,get_room_index(3699));
		do_look(ch,"auto");
		WAIT_STATE(ch,PULSE_VIOLENCE*6);
		return;
	} else {
		if(!trusts(ch,victim))
			return send_to_char("They do not allow you to do that.\n\r",ch);

		act("You call upon the power of Light to transport $N to the safe haven.",ch,0,victim,TO_CHAR);
		act("$n calls upon the Light and you are transported to the safe haven.",ch,0,victim,TO_VICT);
		act("$n vanishes in a flash of light.",victim,0,ch,TO_ROOM);
		char_from_room(victim);
		char_to_room(victim,get_room_index(3699));
		do_look(victim,"auto");
		WAIT_STATE(ch,PULSE_VIOLENCE*6);
		WAIT_STATE(victim,PULSE_VIOLENCE*3);
		return;
	}

return;
}
