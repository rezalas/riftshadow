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


/* command procedures needed */
DECLARE_DO_FUN(do_quit		);
DECLARE_DO_FUN(do_yell		);
DECLARE_DO_FUN(do_follow	);
DECLARE_DO_FUN(do_murder        );
DECLARE_AFF_FUN(poison_tick);
CHAR_DATA *get_random_ch(CHAR_DATA *ch, ROOM_INDEX_DATA *room);
char * flag_name_lookup args((long bitv, const struct flag_type *flag_table));

int save_number = 0;

/*
 * Advancement stuff.
 */
void advance_level( CHAR_DATA *ch, bool hide )
{
    char buf[MAX_STRING_LENGTH];
	float tmp;
    int add_hp;
    int add_mana;
    int add_move;
    int add_prac;
    int int_mod;
	int sn;
	int i;
	int wis_mod;
    if(IS_NPC(ch))
	return;
    ch->pcdata->last_level =
	( ch->played + (int) (current_time - ch->logon) ) / 3600;

    sprintf( buf, "the %s",
	title_table [ch->Class()->GetIndex()] [ch->level] [ch->sex == SEX_FEMALE ? 1 : 0] );
    set_title( ch, buf );

    int_mod = get_curr_stat(ch,STAT_INT) - 2;	
	
	wis_mod = get_curr_stat(ch,STAT_WIS) - 2;

    add_move	= number_range((get_curr_stat(ch,STAT_CON)
				  + get_curr_stat(ch,STAT_DEX))/5,(get_curr_stat(ch,STAT_CON)
				  + get_curr_stat(ch,STAT_DEX))/3);
    add_prac	= wis_app[get_curr_stat(ch,STAT_WIS)].practice;

	tmp			= -1.3333 + .5333 * get_curr_stat(ch, STAT_CON) + 1.3333 * ch->Class()->gainconst + number_range(-1,2);
	add_hp		= (int)tmp;
	if(get_curr_stat(ch,STAT_CON) >= 24)
		add_hp += get_curr_stat(ch,STAT_CON) - 23;
	tmp			= 8.0278 + 0.5667 * get_curr_stat(ch, STAT_INT) - 2.0833 * ch->Class()->gainconst + number_range(-1,2);
	add_mana	= (int)tmp;
	if(ch->Class()->GetIndex() == CLASS_WARRIOR && number_percent() > 75)
		add_hp += 1;
	if(ch->Class()->GetIndex() == CLASS_NECROMANCER)
		add_mana -= number_range(2,5);
	if(ch->Class()->GetIndex() == CLASS_SORCERER)
		add_mana -= number_range(1,2);
    add_move	= UMAX(  6, add_move );

    ch->max_hit 	+= add_hp;
    ch->max_mana	+= add_mana;
    ch->max_move	+= add_move;
    ch->practice	+= add_prac;
    if(ch->level % 5 == 0)
         ch->train		+= 1;
    if(ch->Class()->GetIndex() == CLASS_WARRIOR && ch->level % 20 == 0)
		ch->pcdata->special++;
    ch->pcdata->perm_hit	+= add_hp;
    ch->pcdata->perm_mana	+= add_mana;
    ch->pcdata->perm_move	+= add_move;

	for (sn = 0; sn < MAX_SKILL; sn++) {
		if (ch->pcdata->learned[sn] > 10 && skill_table[sn].ctype != CMD_POWER){
			ch->pcdata->learned[sn]++;
			ch->pcdata->learned[sn] = UMIN(ch->pcdata->learned[sn],100);
		}
	}

	for (i=0;i<100;i++) {
		if (ch->pcdata->recentkills[i] && ch->pcdata->recentkills[i][0] != '\0' && strcmp(ch->pcdata->recentkills[i],""))
		{
			free_pstring(ch->pcdata->recentkills[i]);
			ch->pcdata->recentkills[i] = NULL;
		}
	}
	
    if (!hide) {
	sprintf(buf,
		"%sYou gain %d/%d hp, %d/%d mana, %d/%d move, and %d/%d practices.%s\n\r",
		get_char_color(ch,"lightgreen"),
		add_hp,ch->max_hit, add_mana, ch->max_mana,
		add_move, ch->max_move, add_prac,ch->practice,
		END_COLOR(ch));
    send_to_char(buf,ch);
    }
	
	save_char_obj(ch);
	
    return;
}



void gain_exp( CHAR_DATA *ch, int gain )
{
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) )
	return;

    if (ch->level < LEVEL_HERO)
	ch->exp += gain;

    while ( ch->level < LEVEL_HERO && ch->exp >=
        exp_per_level(ch) * (ch->level) )
    {
	sprintf(buf,"%sYou raise a level!! %s",get_char_color(ch,"lightgreen"),
		END_COLOR(ch));
	send_to_char( buf, ch );
	ch->level += 1;
	sprintf(buf,"%s gained level %d",ch->name,ch->level);
	log_string(buf);
	sprintf(buf,"$N has attained level %d!",ch->level);
	wiznet(buf,ch,NULL,WIZ_LEVELS,0,0);
	advance_level(ch,FALSE);
	//ch->hit = ch->max_hit;
	//ch->mana = ch->max_mana;
	//ch->move = ch->max_move;
	save_char_obj(ch);
    }

    return;
}



/*
 * Regeneration stuff.
 */
int hit_gain( CHAR_DATA *ch )
{
    int gain;
    int number;

    if (ch->in_room == NULL)
	return 0;

    if (is_affected(ch,gsn_atrophy) || is_affected(ch,gsn_prevent_healing))
		return 0;

	if ( IS_NPC(ch) ) {
		gain =  5 + ch->level;
 	
		if (IS_AFFECTED(ch,AFF_REGENERATION))
	    	gain *= 2;
		
		if(is_affected_area(ch->in_room->area,gsn_infidels_fate))
		{
			if(IS_GOOD(ch))
				gain = (int)((float)gain * 1.75);
			if(IS_EVIL(ch))
				gain = (int)((float)gain * .75);
		}

		switch(ch->position)
		{
	    	default :
				gain /= 2;
			break;
	    	case POS_SLEEPING:
				gain = 3 * gain/2;
			break;
	    	case POS_RESTING:
			break;
	    	case POS_FIGHTING:
				gain /= 3;
			break;
 		}
	}
    else
    {
		gain = UMAX(3,(get_curr_stat(ch,STAT_CON) - 3)/2 + ch->level/4);
		gain += (ch->Class()->gainconst + 3) * 3;
 		number = number_percent();
	
		if (number < get_skill(ch,gsn_fast_healing))
		{
	    	gain += number * gain / 100;
	    	if (ch->hit < ch->max_hit)
				check_improve(ch,gsn_fast_healing,TRUE,8);
		}

		switch ( ch->position )
		{
	    	default:
				gain /= 4;
			break;
	    	case POS_SLEEPING:
			break;
	    	case POS_RESTING:
				gain /= 2;
			break;
	    	case POS_FIGHTING:
				gain /= 6;
			break;
		}
        
		if (IS_HUNGRY(ch))
            gain /= 2;
        if (IS_THIRSTY(ch))
            gain /= 2;
	}

    if (ch->on != NULL && ch->on->item_type == ITEM_FURNITURE)
		gain = (gain * 7/5);
    if (is_affected(ch, gsn_bleeding))
    		gain = (gain * 3/5);
		
    if ( IS_AFFECTED(ch, AFF_POISON) )
		gain /= 4;

    if (IS_AFFECTED(ch, AFF_PLAGUE))
		gain /= 8;

    if (IS_AFFECTED(ch,AFF_HASTE) )
		gain /=2;

	if ( IS_AFFECTED(ch,AFF_SLOW))
	{
		gain *= 17;
		gain /= 10 ;
	}

    if (is_affected(ch,gsn_camp))
    {
		if (number_percent() < ch->pcdata->learned[gsn_camp])
			gain *=2;
    }
    
	gain *= 2;

    if (is_affected(ch,gsn_regeneration))
    	gain *=2;

	if (!IS_NPC(ch) && IS_SET(ch->act,PLR_MORON))
		gain /= 2;
    if (!IS_NPC(ch))
        for(OBJ_DATA *fire = ch->in_room->contents; fire; fire = fire->next_content)
                if(fire->item_type == ITEM_CAMPFIRE)
                        gain = (int)((float)gain * ((float)ch->Profs()->ProfEffect("firestarting", fire->value[0])));

    return UMIN(gain, ch->max_hit - ch->hit);
}



int mana_gain( CHAR_DATA *ch )
{
    int gain;
    int number;

    if (ch->in_room == NULL)
	return 0;
    if (is_affected(ch,gsn_atrophy) || is_affected(ch,gsn_prevent_healing))
	return 0;

/*    if (!IS_NPC(ch))
    {
	if (ch->pcdata->condition[COND_STARVING] > 6)
		return 0;
	if (ch->pcdata->condition[COND_DEHYDRATED] > 4)
		return 0;
    }
*/
    if ( IS_NPC(ch) )
    {
	gain = 5 + ch->level/2;
	switch (ch->position)
	{
	    default:		gain /= 2;		break;
	    case POS_SLEEPING:	gain = 3 * gain/2;	break;
   	    case POS_RESTING:				break;
	    case POS_FIGHTING:	gain /= 3;		break;
    	}
    }
    else
    {
	gain = (get_curr_stat(ch,STAT_WIS)/4 - 4
	      + get_curr_stat(ch,STAT_INT) + ch->level/2);
	number = number_percent();
	if (number < get_skill(ch,gsn_meditation))
	{
	    gain += number * gain / 100;
	    if (ch->mana < ch->max_mana)
	        check_improve(ch,gsn_meditation,TRUE,4);
	}
	number = number_percent();
	if (number < get_skill(ch,gsn_trance))
	{
	    gain += number * gain / 100;
	    if (ch->mana < ch->max_mana)
		check_improve(ch,gsn_trance,TRUE,4);
	}

	switch ( ch->position )
	{
	    default:		gain /= 4;			break;
	    case POS_SLEEPING: 					break;
	    case POS_RESTING:	gain /= 2;			break;
	    case POS_FIGHTING:	gain /= 6;			break;
	}

	if (IS_HUNGRY(ch))
	    gain /= 2;
	if (IS_THIRSTY(ch))
		 gain /= 2;

    }

    if (ch->on != NULL && ch->on->item_type == ITEM_FURNITURE)
	gain = gain * 7/5;

    if ( IS_AFFECTED( ch, AFF_POISON ) )
	gain /= 4;

    if (IS_AFFECTED(ch, AFF_PLAGUE))
        gain /= 8;

    if (IS_AFFECTED(ch,AFF_HASTE) )
	gain /=2;
    if ( IS_AFFECTED(ch,AFF_SLOW))
	gain += (7*gain/10);


    if (is_affected(ch,gsn_camp))
    {
	if (number_percent() < ch->pcdata->learned[gsn_camp])
	gain *=2;
    }


    if (is_affected(ch,gsn_mana_conduit))
    {
	gain *=2;
    }

    gain *= 2;
    return UMIN(gain, ch->max_mana - ch->mana);
}



int move_gain( CHAR_DATA *ch )
{
    int gain;

    if (ch->in_room == NULL)
	return 0;
    if (is_affected(ch,gsn_atrophy) || is_affected(ch,gsn_prevent_healing))
	return 0;

	if (!IS_NPC(ch))
    {
	if (ch->pcdata->condition[COND_STARVING] > 6)
		return 5;
	if (ch->pcdata->condition[COND_DEHYDRATED] > 4)
		return 5;
    }

	if ( IS_NPC(ch) )
    {
	gain = ch->level;
    }
    else
    {
	gain = UMAX( 15, ch->level/2 );

	switch ( ch->position )
	{
	case POS_SLEEPING: gain += get_curr_stat(ch,STAT_DEX);		break;
	case POS_RESTING:  gain += get_curr_stat(ch,STAT_DEX) / 2;	break;
	}
        if (IS_HUNGRY(ch))
            gain /= 2;
        if (IS_THIRSTY(ch))
            gain /= 2;
    }

    gain *= ch->in_room->heal_rate/100;

    if (ch->on != NULL && ch->on->item_type == ITEM_FURNITURE)
	gain = gain * 6/5;


    if ( IS_AFFECTED(ch, AFF_POISON) )
	gain /= 4;

    if (IS_AFFECTED(ch, AFF_PLAGUE))
        gain /= 8;

    if (IS_AFFECTED(ch,AFF_HASTE) || IS_AFFECTED(ch,AFF_SLOW))
        gain *=2 ;

    if (is_affected(ch,gsn_camp))
    {
	if (number_percent() < ch->pcdata->learned[gsn_camp])
	gain *= 2;
    }
    gain *= 2;
    return UMIN(gain, ch->max_move - ch->move);
}


/* If you don't want starvation/dehydration in your mud then simply return
from function before the if (ch->level > 10) check...(Ceran)
*/
void gain_condition( CHAR_DATA *ch, int iCond, int value )
{
    int condition;
    int counter;

    if ( value == 0 || IS_NPC(ch) || IS_IMMORTAL(ch) || IS_HEROIMM(ch) || IS_SET(ch->act, PLR_NOVOID))
	return;
    if (!ch->desc)
	return;
    condition				= ch->pcdata->condition[iCond];
    if (condition == -1)
        return;
    ch->pcdata->condition[iCond]	= UMAX(0, condition + value);
    if (ch->level > 10)
    {
	if (number_percent() > 50) {
    if (ch->pcdata->condition[iCond] == 0 && iCond == COND_HUNGER)
		ch->pcdata->condition[COND_HUNGER]++;
    if (ch->pcdata->condition[iCond] == 0 && iCond == COND_THIRST
		&& !is_affected(ch,gsn_cooling_mist))
		ch->pcdata->condition[COND_THIRST]++;
	}
	else if (ch->pcdata->condition[iCond] == 0 && iCond == COND_THIRST && is_affected(ch,gsn_accumulate_heat))
		ch->pcdata->condition[COND_THIRST]++;
    }
	else if (iCond == COND_DRUNK && condition)
	{
		ch->pcdata->condition[COND_DRUNK]--;
	}
    else
    {
		ch->pcdata->condition[COND_THIRST] = 0;
		ch->pcdata->condition[COND_HUNGER] = 0;
    }
	ch->pcdata->condition[COND_THIRST] = UMIN(ch->pcdata->condition[COND_THIRST],75);
	ch->pcdata->condition[COND_HUNGER] = UMIN(ch->pcdata->condition[COND_HUNGER],75);
    if (is_affected(ch,gsn_aura_of_sustenance))
    {
		ch->pcdata->condition[COND_HUNGER] = 0;
		ch->pcdata->condition[COND_THIRST] = 0;
    }


	switch ( iCond )
	{
	case COND_HUNGER:
	    if (ch->pcdata->condition[COND_HUNGER] > COND_HUNGRY)
	    send_to_char( "You are hungry.\n\r",  ch );
	    break;

	case COND_THIRST:
	    if (ch->pcdata->condition[COND_THIRST] > COND_HUNGRY)
	    send_to_char( "You are thirsty.\n\r", ch );
	    break;

	case COND_DRUNK:
	    if (ch->pcdata->condition[COND_DRUNK] == 10)
			send_to_char( "You are sober.\n\r", ch );
	    break;
	default:
	    break;
	}

    if (ch->pcdata->condition[COND_HUNGER] > COND_HUNGRY + 5 && iCond == COND_HUNGER)
    {
		counter = ch->pcdata->condition[COND_HUNGER] - COND_HUNGRY;
		send_to_char("You are starving!\n\r",ch);
		if (ch->level > 10)
			damage_new(	ch,
					ch,
					UMIN(20,number_range(counter,2*counter)),
					TYPE_UNDEFINED,
					DAM_OTHER,
					TRUE,
					HIT_UNBLOCKABLE,
					HIT_NOADD,
					HIT_NOMULT,
					"hunger");
    }
	
    if (ch->pcdata->condition[COND_THIRST] > COND_HUNGRY + 5 && iCond == COND_THIRST)
    {
		counter = ch->pcdata->condition[COND_THIRST] - COND_HUNGRY;
		send_to_char("You are dying of thirst!\n\r",ch);
		if (ch->level > 10)
			damage_new(	ch,
						ch,
						UMIN(20,number_range(counter,2*counter)),
						TYPE_UNDEFINED,
						DAM_INTERNAL,
						TRUE,
						HIT_UNBLOCKABLE,
						HIT_NOADD,
						HIT_NOMULT,
						"thirst");
    } 

    return;
}


/*
 * Mob autonomous action.
 * This function takes 25% to 35% of ALL Merc cpu time.
 * -- Furey
 */
void mobile_update( void )
{
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    EXIT_DATA *pexit = NULL;
    int door;

    /* Examine all mobs. */
    for ( ch = char_list; ch != NULL; ch = ch_next )
    {
	ch_next = ch->next;

	if ( !IS_NPC(ch) || ch->in_room == NULL)
	    continue;

	if (ch->in_room->area->empty && !IS_SET(ch->act,ACT_UPDATE_ALWAYS) && !IS_SET(ch->act,ACT_WARD_MOB))
	    continue;

	if(ch->position==POS_FIGHTING && ch->pIndexData->cast_spell[0]!=NULL && number_percent()>50)
		if(do_mob_cast(ch))
			continue;

	if(TRAPS_MEVENT(ch,TRAP_MPULSE))
		CALL_MEVENT(ch,TRAP_MPULSE,ch);

	/* That's all for sleeping / busy monster, and empty zones */
	if ( IS_SET(ch->progtypes,MPROG_PULSE) &&
		(ch->in_room->area->nplayer > 0
		|| IS_SET(ch->act,ACT_UPDATE_ALWAYS)
		|| IS_SET(ch->act,ACT_WARD_MOB)))
          (ch->pIndexData->mprogs->pulse_prog) (ch);

        if ( ch->position != POS_STANDING )
            continue;

	/* Scavenge */
	if ( IS_SET(ch->act, ACT_SCAVENGER)
	&&   ch->in_room->contents != NULL
	&&   number_bits( 6 ) == 0 )
	{
	    OBJ_DATA *obj;
	    OBJ_DATA *obj_best;
	    int max;

	    max         = 1;
	    obj_best    = 0;
	    for ( obj = ch->in_room->contents; obj; obj = obj->next_content )
	    {
		if ( CAN_WEAR(obj, ITEM_TAKE) && can_loot(ch, obj)
		     && obj->cost > max  && obj->cost > 0)
		{
		    obj_best    = obj;
		    max         = obj->cost;
		}
	    }

	    if ( obj_best )
	    {
/*		get_obj(ch,obj,NULL, FALSE);*/
		obj_from_room( obj_best );
		obj_to_char( obj_best, ch );
		act( "$n gets $p.", ch, obj_best, NULL, TO_ROOM );
	    }
	}

	/* Wander */
	if(IS_SET(ch->act, ACT_WANDER)
	&& !ch->pIndexData->pShop
	&& (number_bits(3) == 0))
	{
		door = number_range(0, 5);
/*		if(IS_SET(ch->act, ACT_FASTWANDER) && 
				(ch->in_room->exit[0]  
				|| ch->in_room->exit[1] 
				|| ch->in_room->exit[2]
				|| ch->in_room->exit[3]
				|| ch->in_room->exit[4]
				|| ch->in_room->exit[5]))
			while((pexit = ch->in_room->exit[door]) == NULL || pexit->u1.to_room == NULL)
				door = number_range(0, 5);
*/
		if((pexit = ch->in_room->exit[door]) != NULL
		&& pexit->u1.to_room != NULL
		&& !IS_SET(pexit->exit_info, EX_CLOSED)
		&& !IS_SET(pexit->u1.to_room->room_flags, ROOM_NO_MOB)
    	&& !( pexit->u1.to_room->vnum < ch->pIndexData->restrict_low
    	||    pexit->u1.to_room->vnum > ch->pIndexData->restrict_high)
    	&& ( !IS_SET(ch->act, ACT_STAY_AREA)
    	||   pexit->u1.to_room->area == ch->in_room->area )
    	&& !( IS_SET(ch->act, ACT_OUTDOORS)
    	&&    IS_SET(pexit->u1.to_room->room_flags,ROOM_INDOORS))
    	&& !( IS_SET(ch->act, ACT_INDOORS)
		&&   !IS_SET(pexit->u1.to_room->room_flags,ROOM_INDOORS))
		&& !( pexit->u1.to_room->sector_type == SECT_UNDERWATER 
		&& !(IS_SET(ch->form, FORM_AQUATIC) 
		|| IS_SET(ch->form,FORM_AMPHIBIAN)))
    	&& !( pexit->u1.to_room->sector_type == SECT_WATER 
		&& !IS_SET(ch->form, FORM_AQUATIC) 
		&& !IS_SET(ch->form, FORM_FISH) 
		&& !IS_AFFECTED(ch,AFF_FLYING)))
    		move_char( ch, door, FALSE, TRUE );
	}
    }

    return;
}

void clear_cabal_leaders(CHAR_DATA *imm, int cabal, char *mesg)
{
	return;
}

void update_guild_leader(void)
{
	return;
}
/*
 * Update the time.
 */
void time_update( void )
{
	char buf[MSL], colbuf[MSL];
	char bw1[MSL], bw2[MSL], bw3[MSL], bw4[MSL];
	DESCRIPTOR_DATA *d;
	AREA_DATA *area;

	buf[0] = '\0';

	sprintf(bw1,"{rA ruddy glow fills the eastern sky as dawn approaches.{x");
	sprintf(bw2,"{YThe sky brightens as the great flaming disc peers above the eastern horizon.{x");
	sprintf(bw3,"{MThe sun slowly disappears below the western horizon.{x");
	sprintf(bw4,"{mThe violet glow leaves the western sky as evening gives way to night.{x");

	if (time_info.half)  {
		time_info.half = FALSE;

			/* Season affects sunrise/sunset! */
		switch (time_info.season) {
			case(0):
				switch (++time_info.hour) {
					case(7):
						sun = SUN_RISE;
						sprintf(buf,bw1);
						break;
					case(8):
						sun = SUN_LIGHT;
						sprintf(buf,bw2);
						break;
					case(16):
						sun = SUN_SET;
						sprintf(buf,bw3);
						break;
					case(17):
						sun = SUN_DARK;
						sprintf(buf,bw4);
						break;
					case(24):
						time_info.hour = 0;
						time_info.day++;
						break;
				}
				break;
			case(1):
				switch (++time_info.hour) {
					case(6):
						sun = SUN_RISE;
						sprintf(buf,bw1);
						break;
					case(7):
						sun = SUN_LIGHT;
						sprintf(buf,bw2);
						break;
					case(18):
						sun = SUN_SET;
						sprintf(buf,bw3);
						break;
					case(19):
						sun = SUN_DARK;
						sprintf(buf,bw4);
						break;
					case(24):
						time_info.hour = 0;
						time_info.day++;
						break;
				}
				break;
			case(2):
				switch (++time_info.hour) {
					case(5):
						sun = SUN_RISE;
						sprintf(buf,bw1);
						break;
					case(6):
						sun = SUN_LIGHT;
						sprintf(buf,bw2);
						break;
					case(20):
						sun = SUN_SET;
						sprintf(buf,bw3);
						break;
					case(21):
						sun = SUN_DARK;
						sprintf(buf,bw4);
						break;
					case(24):
						time_info.hour = 0;
						time_info.day++;
						break;
				}
				break;
			case(3):
				switch (++time_info.hour) {
					case(6):
						sun = SUN_RISE;
						sprintf(buf,bw1);
						break;
					case(7):
						sun = SUN_LIGHT;
						sprintf(buf,bw2);
						break;
					case(18):
						sun = SUN_SET;
						sprintf(buf,bw3);
						break;
					case(19):
						sun = SUN_DARK;
						sprintf(buf,bw4);
						break;
					case(24):
						time_info.hour = 0;
						time_info.day++;
						break;
				}
				break;
		}
	
		if ( time_info.day   >= 30 ) {
			time_info.day = 0;
			time_info.month++;
			if ( (time_info.month + 1) % 3 == 0) {
	    	    time_info.season++;
				if(time_info.season>3)
					time_info.season = 0;
			}
		}
	
		if ( time_info.month >= 12 )
		{
			time_info.month = 0;
			time_info.year++;
			//update_guild_leader();
		}
	} else {
		time_info.half = TRUE;
	}

	if (buf[0] != '\0') {
		for (d = descriptor_list;d != NULL; d = d->next) {
			if (d->connected == CON_PLAYING &&
				d->character->in_room &&
				d->character->in_room->sector_type != SECT_UNDERWATER &&
				IS_OUTSIDE(d->character) &&
				IS_AWAKE(d->character)  &&
				!is_editing(d->character) &&
				!(is_affected_area(d->character->in_room->area,gsn_whiteout)) &&
				!(is_affected_area(d->character->in_room->area,gsn_cyclone)) &&
				!(IS_AFFECTED(d->character,AFF_BLIND))) {
				colorconv(colbuf, buf, d->character);
				send_to_char(colbuf,d->character);
				send_to_char("\n\r",d->character);
			}
		}
	}

	if(buf[0] != '\0') {
		for(area = area_first; area != NULL; area = area->next) {
			if(IS_SET(area->progtypes, APROG_SUN))
				(area->aprogs->sun_prog) (area);
			}
	}
	
    return;
}

void gold_update(void)
{
	CHAR_DATA *mob;
	extern long total_wealth;
	long mob_gold;
	long gold;

	if (total_wealth == 0)
		return;

	for (mob = char_list; mob; mob = mob->next) 
	{
		if (!IS_NPC(mob) || mob->stolen_from)
			continue;
		mob_gold = UMAX(total_gold - player_gold, 100000);
		gold = mob->pIndexData->wealth * (mob_gold / total_wealth);
		if (gold > 9)
			gold = number_range((int)((float)gold * 0.75), (int)((float)gold * 1.25));
		if (gold < 0)
			gold = 0;
		mob->gold = gold;
	}

	return;
}

/*
 * Update the weather -- Eladrian's l33t regional weather system
 */

void weather_update(void)
{
	char buf[MSL];
	int i, random, season = time_info.season;
	int average = 0, skytot = 0, temptot = 0;
	AREA_DATA *area;


	/* Randomize sky, temperature, wind for each area, based on climate */
	for(area = area_first; area; area = area->next) {
		random = number_percent();
		for (i=0;i<MAX_SKY;i++) {
			if (random <= climate_table[area->climate].skyfreqs[season][i]) {
				area->sky = i;
				break;
			}
		}

		random = number_percent();
		for (i=0;i<MAX_TEMP;i++) {
			if (random <= climate_table[area->climate].tempfreqs[season][i]) {
				area->temp = i;
				break;
			}
		}
		
		area->wind = number_range(0,2);
	}
	
	/* Adjacent areas have a moderating influence on weather.  If there's
	 * a blizzard going on next door, it won't be perfectly sunny.  Similarly,
	 * it won't be hot and snowing at the same time, etc.  Just make sure that
	 * illogical combinations are eliminated. */
	for (area = area_first; area; area = area->next) {
		skytot = 0;
		temptot = 0;
		
		for (i=0;i<MAX_ADJACENT;i++) {
			if (!area->adjacent[i])
				break;
			skytot += area->adjacent[i]->sky;
			temptot += area->adjacent[i]->temp;
		}
	
		if (i > 0) {
			average = skytot / (i + 1);

			if (abs(average - area->sky) >= 4)
				area->sky = (area->sky + average) / 2;

			average = temptot / (i + 1);

			if (abs(average - area->temp) >= 2)
				area->temp = (area->sky + average) / 2;
		}

		if (area->sky >= SKY_FLURRY && area->temp < TEMP_COOL)
			area->temp = TEMP_COOL;

		if (area->sky == SKY_DOWNPOUR || area->sky == SKY_TSTORM ||
			area->sky == SKY_BLIZZARD) {
			area->wind++;
		}

		if (area->climate == CLIMATE_NONE) {
			area->sky = SKY_PCLOUDY;
			area->temp = TEMP_WARM;
			area->wind = WIND_CALM;
		}
	}

	/* Echo the changing weather to everyone who's outdoors. */
	for (area = area_first; area; area = area->next) {
		switch(area->sky) {
			case(SKY_CLEAR):
				sprintf(buf,"There is not a cloud to be seen in the sky above.");
				break;
			case(SKY_PCLOUDY):
				sprintf(buf,"A few clouds dot the skies above.");
				break;
			case(SKY_OVERCAST):
				sprintf(buf,"A thick grey mass of clouds obscures the sky.");
				break;
			case(SKY_DRIZZLE):
				sprintf(buf,"A light drizzle falls from the sky.");
				break;
			case(SKY_DOWNPOUR):
				sprintf(buf,"Sheets of rain pour down from the skies above.");
				break;
			case(SKY_TSTORM):
				sprintf(buf,"Lightning flashes in the distance as a booming peal of thunder approaches.");
				break;
			case(SKY_FLURRY):
				sprintf(buf,"Scattered snowflakes drift down from the skies above.");
				break;
			case(SKY_BLIZZARD):
				sprintf(buf,"Driving snow sweeps down from the skies as a chill fills the air.");
				break;
			case(SKY_HAIL):
				sprintf(buf,"Pebble-sized hailstones begin to fall from the skies.");
				break;
			default:
				buf[0] = '\0';
				break;
		}

		outdoors_echo(area,buf);
	}
	
	return;
}

void berus_update( void )
{
	if (++berus_pos >= 360)	berus_pos = 0;
	
	if (berus_pos >= 0 && berus_pos < 45)
		moon_berus = MOON_NEW;
	else if (berus_pos >= 45 && berus_pos < 90)
		moon_berus = MOON_CWAXING;
	else if (berus_pos >= 90 && berus_pos < 135)
		moon_berus = MOON_HWAXING;
	else if (berus_pos >= 135 && berus_pos < 180)
		moon_berus = MOON_GWAXING;
	else if (berus_pos >= 180 && berus_pos < 225)
		moon_berus = MOON_FULL;
	else if (berus_pos >= 225 && berus_pos < 270)
		moon_berus = MOON_GWANING;
	else if (berus_pos >= 270 && berus_pos < 315)
		moon_berus = MOON_HWANING;
	else if (berus_pos >= 315 && berus_pos < 360)
		moon_berus = MOON_CWANING;
}

void calabren_update( void )
{
	if (++calabren_pos >= 360)	calabren_pos = 0;
	
	if (calabren_pos >= 0 && calabren_pos < 45)
		moon_calabren = MOON_NEW;
	else if (calabren_pos >= 45 && calabren_pos < 90)
		moon_calabren = MOON_CWAXING;
	else if (calabren_pos >= 90 && calabren_pos < 135)
		moon_calabren = MOON_HWAXING;
	else if (calabren_pos >= 135 && calabren_pos < 180)
		moon_calabren = MOON_GWAXING;
	else if (calabren_pos >= 180 && calabren_pos < 225)
		moon_calabren = MOON_FULL;
	else if (calabren_pos >= 225 && calabren_pos < 270)
		moon_calabren = MOON_GWANING;
	else if (calabren_pos >= 270 && calabren_pos < 315)
		moon_calabren = MOON_HWANING;
	else if (calabren_pos >= 315 && calabren_pos < 360)
		moon_calabren = MOON_CWANING;
}

/*
 * Update all chars, including mobs.
*/
void char_update( void )
{
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    CHAR_DATA *ch_quit;
    int hgain;
	bool ghost = FALSE;

    ch_quit	= NULL;

    /* update save counter */
    save_number++;

    if (save_number > 2)
		save_number = 0;

    for ( ch = char_list; ch != NULL; ch = ch_next ) {
	CHAR_DATA *master;
	AFFECT_DATA *paf;
	AFFECT_DATA *paf_next;
	bool charm_gone;

	ch_next = ch->next;
	master = NULL;
	if (IS_NPC(ch) && 
		(sun == SUN_RISE || sun == SUN_LIGHT) && 
		ch->in_room &&
		number_percent() < 90 && 
		!IS_AFFECTED(ch,AFF_SLEEP) && 
		ch->fighting == NULL)
	{
		if(IS_SET(ch->act, ACT_DIURNAL) && IS_AFFECTED(ch, AFF_NOSHOW))
		{
			REMOVE_BIT(ch->affected_by, AFF_NOSHOW);
		}
		else if(IS_SET(ch->act, ACT_NOCTURNAL) && !IS_AFFECTED(ch, AFF_NOSHOW))
		{
			SET_BIT(ch->affected_by, AFF_NOSHOW);
		}
	}
	else if (IS_NPC(ch) && 
			 sun >= SUN_SET && 
			 ch->in_room &&
        	 number_percent() < 90 && 
			 ch->fighting == NULL)
	{
		if(IS_SET(ch->act, ACT_NOCTURNAL) && IS_AFFECTED(ch, AFF_NOSHOW))
			REMOVE_BIT(ch->affected_by, AFF_NOSHOW);
		else if(IS_SET(ch->act, ACT_DIURNAL) && !IS_AFFECTED(ch, AFF_NOSHOW))
			SET_BIT(ch->affected_by, AFF_NOSHOW);
	}
	
	if ( ch->pause > 0 )
	    ch->pause--;
	if (IS_NPC(ch) && ch->hit < 0 && ch->in_room)
	{
		char buf[MSL];
		sprintf(buf,"%s in %d has HP %d", ch->name, ch->in_room->vnum, ch->hit);
		bug(buf,0);
	}
	if ( ch->ghost > 0 )
	{
		ch->ghost--;
		switch(ch->ghost)
		{
			case 4:
				send_to_char("Your body starts to collect itself.\n\r", ch);
			break;
			case 2:
				send_to_char("Your body begins to solidify.\n\r", ch);
			break;
			case 0:
				send_to_char("You are no longer a ghost.\n\r", ch);
			break;
		}
	}

	if (ch->bounty_timer > 0)
		ch->bounty_timer--;
	
	if (ch->ghost > 0)
		ghost = TRUE;
	/* LOWBIE LEVELLING */
	if (!IS_NPC(ch) && ch->level < 20 && (ch->in_room->vnum > 24599 || ch->in_room->vnum < 24500))
	{
		char buf1[MSL];
		while(ch->level < 20)
		{
			ch->level++;
                	advance_level( ch, TRUE);
		}
        	ch->exp = exp_per_level(ch) * (ch->level - 1);
		colorconv(buf1,"{RYou feel more experienced as you depart the Adventurer's Academy!{x\n\r",ch);
		send_to_char(buf1,ch);
		colorconv(buf1,"{GYou are now level 20.{x\n\r",ch);
		send_to_char(buf1,ch);
		if(ch->pet)
		{
			sprintf(buf1,"Remember, you can ask your familiar questions.  For example, 'say %s, how do I get to my guild?'.\n\r",ch->pet->short_descr);
			send_to_char(buf1,ch);
		}
	}
	if (!IS_NPC(ch))
		ch->Profs()->UpdateProfPoints();
	if (!IS_AFFECTED(ch, AFF_SLEEP) && is_affected(ch, gsn_bleeding) && ch->position == POS_SLEEPING)
		ch->position = POS_STANDING;
		
	if ( ch->position >= POS_STUNNED )
	{
	    if ( ch->hit  < ch->max_hit ) /* Some regen stuff */
	    {
			hgain = hit_gain(ch);
			ch->hit  += hgain;
	    }
	    else
			ch->hit = ch->max_hit;

	    if ( ch->mana < ch->max_mana )
			ch->mana += mana_gain(ch);
	    else
			ch->mana = ch->max_mana;

	    if ( ch->move < ch->max_move )
			ch->move += (is_affected(ch, gsn_repose)) ? (move_gain(ch)*2) : (move_gain(ch));
	    else
			ch->move = ch->max_move;
	}

	if ( ch->position == POS_STUNNED )
	    update_pos( ch );

	if (ch->position < POS_STUNNED)
		damage_new(ch,ch,2,gsn_bleeding,DAM_TRUESTRIKE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"slow bleeding");

	ch->talismanic = UMAX(0, ch->talismanic - 0.0625);

	if(!IS_NPC(ch) && ch->in_room)
	{
		ch->pcdata->sect_time[0]++;
		ch->pcdata->sect_time[ch->in_room->sector_type]++;
	}
	if(!IS_NPC(ch) && ch->pcdata->save_timer)
		ch->pcdata->save_timer--;
	if ( !IS_NPC(ch) /* && !IS_IMMORTAL(ch) */)
	{
	    OBJ_DATA *obj;

	    if ( ( obj = get_eq_char( ch, WEAR_LIGHT ) ) != NULL
	    &&   obj->item_type == ITEM_LIGHT
	    &&   obj->value[2] > 0 )
	    {
		if ( --obj->value[2] == 0 && ch->in_room != NULL )
		{
			ch->in_room->light = UMAX(0,ch->in_room->light - 3);
		    act( "$p goes out.", ch, obj, NULL, TO_ROOM );
		    act( "$p flickers and goes out.", ch, obj, NULL, TO_CHAR );
		    extract_obj( obj );
		}
	 	else if ( obj->value[2] <= 5 && ch->in_room != NULL)
		    act("$p flickers.",ch,obj,NULL,TO_CHAR);
	    }

		ch->timer++;
	    if (ch->timer >= 25 && !IS_HEROIMM(ch) && !IS_IMMORTAL(ch) && !IS_SET(ch->act, PLR_NOVOID))
	    {
		if ( ch->was_in_room == NULL && ch->in_room != NULL )
		{
		    ch->was_in_room = ch->in_room;
		    if ( ch->fighting != NULL )
			stop_fighting( ch, TRUE );
		    act( "$n disappears into the void.",
			ch, NULL, NULL, TO_ROOM );
		    send_to_char( "You disappear into the void.\n\r", ch );
		    if (ch->level > 1)
		        save_char_obj( ch );
		    ch->pause = 0;
		    do_quit_new(ch,"",TRUE);
		    continue;
		}
	    }

	    gain_condition( ch, COND_DRUNK,  -1 );
	    gain_condition( ch, COND_THIRST, 1  );
	    gain_condition( ch, COND_HUNGER, 1  );
	}

	if ( !IS_NPC(ch) && ch->desc == NULL ) {
		/* nothing */
	} else {
		for ( paf = ch->affected; paf != NULL; paf = paf_next ) {
		    paf_next	= paf->next;
		    charm_gone = FALSE;
			if (!ghost && ch->ghost > 0)
				break;

	    	if ( paf->duration > 0 )
		{
				if (paf->tick_fun)
					(*paf->tick_fun) (ch, paf);
				if (!ch || (!ghost && ch->ghost > 0))
					break;
				if (!paf)
					continue;
				paf->duration--;
				if (number_range(0,4) == 0 && paf->level > 0)
		 			paf->level--;  /* spell strength fades with time */
            	}
			else if ( paf->duration < 0 ) 
			{
				if (paf->tick_fun)
					(*paf->tick_fun) (ch,paf);
				if(!ghost && ch->ghost > 0)
					break;
			} 
			else if ((paf->type == gsn_entwine) && (paf->owner == NULL
				|| (paf->owner && ch->in_room != paf->owner->in_room)))
				affect_remove(ch,paf);
			else 
			{
				if(((paf->owner && paf->owner->Class()->GetIndex() == CLASS_PALADIN) || (!paf->owner && ch->Class()->GetIndex() == CLASS_PALADIN) && trusts(ch, paf->owner ? paf->owner : ch)) && paf->aftype == AFT_COMMUNE)
				{
					if(number_percent() < (get_skill(paf->owner ? paf->owner : ch,gsn_channeling)*.85) 
							&& !(skill_table[paf->type].dispel & CAN_CLEANSE))
					{
						check_improve(paf->owner ? paf->owner : ch, gsn_channeling, TRUE, 1);
						if(!paf->owner || ch == paf->owner)
							act("You feel invigorated as your $t supplication is renewed by your deity.", 
									ch, skill_table[paf->type].name, 0, TO_CHAR);
						else
							act("You feel invigorated as $N renews your $t supplication.",
									ch, skill_table[paf->type].name, paf->owner, TO_CHAR);
						paf->duration = paf->init_duration;
						ch->mana = UMAX(ch->mana - skill_table[paf->type].min_mana, 0);
						continue;
					}
				}

				if ( paf_next == NULL
					||   paf_next->type != paf->type
					||   paf_next->duration > 0 ) 
				{
		    		if ( paf->type > 0 && str_cmp(skill_table[paf->type].msg_off,"") ) 
					{
						send_to_char( skill_table[paf->type].msg_off, ch );
						send_to_char( "\n\r", ch );
		    		}
		    	
					if (paf->type && str_cmp(skill_table[paf->type].room_msg_off,"")
						&& IS_AWAKE(ch))
						act(skill_table[paf->type].room_msg_off,ch,0,0,TO_ROOM);
				}
				affect_remove( ch, paf );
			}
		}
	}

	if (!ch)
		continue;
	if (!ghost && ch->ghost > 0)
		continue;

	/*
	 * Careful with the damages here,
	 *   MUST NOT refer to ch after damage taken,
	 *   as it may be lethal damage (on NPC).
	 *
	 *   You must add a line to the poison check in damage_new in fight.c!
	 */

	if(!IS_NPC(ch) && ch->in_room && ch->in_room->sector_type == SECT_BURNING)
		damage_new(ch,ch,dice(ch->level/2,2),gsn_bleeding,DAM_FIRE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"the searing heat*");
	
	}
    /*
     * Autosave and autoquit.
     * Check that these chars still exist.
     */
    for ( ch = char_list; ch != NULL; ch = ch_next ) {
        ch_next = ch->next;

		if (ch->desc != NULL && ch->desc->descriptor % 3 == save_number)
	    	save_char_obj(ch);

    }

    return;
}

/*
 * Update all objs.
 * This function is performance sensitive.
 */
void obj_update( void )
{
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    OBJ_AFFECT_DATA *paf, *paf_next;
    CHAR_DATA *owner, *cguard;

	for ( obj = object_list; obj != NULL; obj = obj_next )
    {
		CHAR_DATA *rch;
		char *message;

		obj_next = obj->next;

		if(obj->moved)
			obj->moved	= FALSE;
	
		if((IS_AFFECTED(obj,AFF_OBJ_BURNING)
			&& (obj->carried_by
			&& (obj->carried_by->in_room->sector_type == SECT_WATER
			|| obj->carried_by->in_room->sector_type == SECT_UNDERWATER)))
			|| (obj->in_room && (obj->in_room->sector_type == SECT_WATER
			|| obj->in_room->sector_type == SECT_UNDERWATER)))
		{
			act("The water extinguishes $p.",obj->carried_by,obj,0,TO_CHAR);
			if (is_affected_obj(obj,gsn_immolate))
				affect_strip_obj(obj,gsn_immolate);
		}

		/* go through affects and decrement */
        for ( paf = obj->affected; paf != NULL; paf = paf_next )
        {
        	paf_next    = paf->next;
			if (paf->duration != 0)
				if (paf->tick_fun)
					(*paf->tick_fun) (obj,paf);
            if ( paf->duration > 0 )
            {
                paf->duration--;
                if (number_range(0,4) == 0 && paf->level > 0)
                  paf->level--;  /* spell strength fades with time */
            }
            else if ( paf->duration < 0 )
				continue;
            else
            {
                affect_remove_obj( obj, paf, TRUE );
            }
        }

		/* crumbling stuff */
		if ( obj->timer <= 0 || --obj->timer > 0 )
	    	continue;

		switch ( obj->item_type )
		{
			default: 				message = "$p crumbles into dust."; 	break;
			case ITEM_FOUNTAIN: 	message = "$p dries up."; 				break;
			case ITEM_CORPSE_NPC: 	message = "$p decays into dust."; break;
			case ITEM_CORPSE_PC:  message = "$p decays into dust."; break;
			case ITEM_FOOD:       message = "$p decomposes.";	break;
			case ITEM_POTION:     message = "$p has evaporated from disuse."; break;
			case ITEM_PORTAL:     message = "$p fades out of existence."; break;
			case ITEM_CAMPFIRE:   message = "$p dies out."; break;
			case ITEM_CONTAINER:
	    		if (CAN_WEAR(obj,ITEM_WEAR_FLOAT))
					if (obj->contains)
		    			message = "$p flickers and vanishes, spilling its contents on the floor.";
					else
		    			message = "$p flickers and vanishes.";
	    		else
					message = "$p crumbles into dust.";
	    	break;
		}
	
		/* Check explosives */
		if (obj && obj->pIndexData->vnum == OBJ_EXPLOSIVES && obj->carried_by)
		{
			bag_explode(obj->carried_by,obj,1);
			continue;
		}
	
		/* Alright. Is this a cabal item? */
		if(isCabalItem(obj) && IS_NPC(obj->carried_by))
		{
			obj->timer=0;
			continue;
		}
		
		if(isCabalItem(obj))
		{
			//If so, then find its home. Don't you want to go to your home?!?!
			cguard=get_cabal_guardian(obj->pIndexData->cabal);
			//If he's too good for his home, try again later.
			if(!cguard)
			{
				obj->timer=15;
				continue;
			}
			act( message, obj->carried_by, obj, NULL, TO_CHAR );
			obj_from_char(obj);
			obj_to_char(obj,cguard);
			obj->timer=0;
			continue;
		}

		if ( obj->carried_by != NULL )
		{
	    	if (IS_NPC(obj->carried_by) && obj->carried_by->pIndexData->pShop != NULL)
				obj->carried_by->gold++;
			else
	    	{
	    		act( message, obj->carried_by, obj, NULL, TO_CHAR );
				if ( obj->wear_loc == WEAR_FLOAT)
		    		act(message,obj->carried_by,obj,NULL,TO_ROOM);
	    	}
		}
		else if ( obj->in_room != NULL && ( rch = obj->in_room->people ) != NULL )
		{
	    	if (! (obj->in_obj && obj->in_obj->pIndexData->vnum == OBJ_VNUM_PIT
	           	&& !CAN_WEAR(obj->in_obj,ITEM_TAKE)))
	    	{
	    		act( message, rch, obj, NULL, TO_ROOM );
	    		act( message, rch, obj, NULL, TO_CHAR );
	    	}
		}

		if (obj->item_type == ITEM_CORPSE_PC && obj->contains)
        {
			/* objects go to pit */
            OBJ_DATA *t_obj, *next_obj, *pit_obj;

			/* Put items into the pit that the player recalls to */
	    	if(obj->in_room != NULL && IS_EXPLORE(obj->in_room))
	    	{ //Gear to char
				for(owner=char_list;owner!=NULL;owner=owner->next)
				{
					if(!IS_NPC(owner) && !str_cmp(owner->true_name,obj->owner))
					{
						if(obj->contains)
						{
                        	send_to_char("Your corpse decays.\n\r",owner);
							//It's their corpse.
            				for (t_obj = obj->contains; t_obj != NULL; t_obj = next_obj)
            				{
               					next_obj = t_obj->next_content;
								act_new("$p returns to you.",owner,t_obj,0,TO_CHAR,POS_DEAD);
								if(t_obj->item_type==ITEM_MONEY)
								{
				        			owner->gold += t_obj->value[1];
									extract_obj(t_obj);
									continue;
								}
                				obj_from_obj(t_obj);
                				obj_to_char(t_obj,owner);
            				}
						}
						extract_obj(obj);
						break;
					}
				}
				continue;
	    	}
	    
			if (obj->value[4])
				pit_obj = get_obj_type(get_obj_index(hometown_table[obj->value[4]].pit));
	    	else
	            pit_obj = get_obj_type(get_obj_index(OBJ_VNUM_PIT));

	    	for (t_obj = obj->contains; t_obj != NULL; t_obj = next_obj)
	    	{
				next_obj = t_obj->next_content;
				obj_from_obj(t_obj);
                obj_to_obj(t_obj,pit_obj);
	    	}
		}
		extract_obj( obj );
	}
	return;
}


/* Mobile tracking. I implemented the most stupid, basic mob tracking you
could possibly come up with, but it works ok from a player point of view.
If you want something more sophisticated just hack that into here
instead...(Ceran)
*/

// Rewrote tracking -Cal
void track_attack(CHAR_DATA *mob, CHAR_DATA *victim)
{
	char buf[MSL];
	if(mob->in_room != victim->in_room || !can_see(mob, victim) || mob->fighting || IS_AFFECTED(mob, AFF_NOSHOW))
		return;
	if(mob->pIndexData->attack_yell)
		sprintf(buf,"%s", act_msg(mob->pIndexData->attack_yell, victim));
	else
		sprintf(buf,"%s, now you die!", victim->name);
	do_yell(mob, buf);
	multi_hit(mob, victim, TYPE_UNDEFINED);
}

void track_update( void )
{
    CHAR_DATA *tch;
    CHAR_DATA *tch_next;
    char buf[MAX_STRING_LENGTH];

    for (tch = char_list; tch != NULL; tch = tch_next) {
		tch_next = tch->next;
		if (!IS_NPC(tch))
			continue;

		if (!tch->last_fought) {
			if (tch->position > POS_RESTING
				&& number_range(1,10) == 1
				&& !tch->fighting
				&& tch->home_room
				&& tch->in_room != tch->home_room
				&& (tch->in_room->vnum < tch->pIndexData->restrict_low
				|| tch->in_room->vnum > tch->pIndexData->restrict_high))
				walk_to_room(tch,tch->home_room);
			continue;
		}

		if (tch->fighting || IS_AFFECTED(tch,AFF_NOSHOW))
			continue;
		if(tch->in_room == tch->last_fought->in_room)
		{
			track_attack(tch, tch->last_fought);
			continue;
		}

		if (!IS_SET(tch->act,ACT_FAST_TRACK) && number_range(1,5) != 1)
			continue;

		if (IS_SET(tch->act,ACT_SMARTTRACK))
			smart_track(tch->last_fought,tch);
		else
			track_char(tch->last_fought,tch);
		
		track_attack(tch, tch->last_fought);

		if (!tch->fighting)
			tch->tracktimer--;

		if (tch->tracktimer == 0)
			tch->last_fought = NULL;
    }

	return;
}

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
void aggr_update( void )
{
    CHAR_DATA *wch;
    CHAR_DATA *wch_next;
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    CHAR_DATA *victim;
	AFFECT_DATA *paf;
	int timer;
    char buf[MAX_STRING_LENGTH];

    for ( wch = char_list; wch != NULL; wch = wch_next )
    {
		wch_next = wch->next;
		if(!wch->name)
			continue;

		if(!wch->in_room)
			continue;

		if (IS_NPC(wch) && IS_SET(wch->progtypes,MPROG_BEAT))
			(wch->pIndexData->mprogs->beat_prog) (wch);

		for (paf = wch->affected; paf; paf = paf->next)
			if (paf->beat_fun)
				(*paf->beat_fun)(wch,paf);

		if((!IS_NPC(wch) && wch->pulseTimer <= pc_race_table[wch->race].racePulse)
			|| (IS_NPC(wch) && wch->pulseTimer <= 12))
			wch->pulseTimer++;

		timer = wch->pulseTimer;

		if (IS_AFFECTED(wch,AFF_HASTE))
			timer++;

		if (IS_AFFECTED(wch,AFF_SLOW))
			timer--;

		if (wch->fighting && 
			((!IS_NPC(wch) && timer >= pc_race_table[wch->race].racePulse)
			 || (IS_NPC(wch) && timer >= 12)))
		{
			update_pc_last_fight(wch,wch->fighting);

			if ( IS_AWAKE(wch) && wch->in_room == wch->fighting->in_room )
				multi_hit( wch, wch->fighting, TYPE_UNDEFINED );
			else
				stop_fighting( wch, FALSE );

			wch->pulseTimer = 0;
		}
		
	if(wch->position==POS_SLEEPING && IS_SET(wch->imm_flags,IMM_SLEEP))
		wch->position=POS_STANDING;
	
	if (IS_AFFECTED(wch,AFF_RAGE) && IS_AWAKE(wch) && !wch->fighting &&
		!(wch->desc == NULL && !IS_NPC(wch)))
	{
	for ( vch = wch->in_room->people; vch != NULL; vch = vch_next)
	{
		vch_next = vch->next_in_room;
		if (wch != vch && can_see(wch,vch) && !IS_IMMORTAL(vch)) {
		if (is_affected(vch,skill_lookup("radiance")))
			continue;
		if (check_stealth(vch, wch))
			continue;
		if (is_safe_new(wch,vch,FALSE))
			continue;
		if (is_same_cabal(wch,vch) && number_range(1,10000) > 1)
			continue;
		if (is_same_group(wch,vch) && IS_NPC(vch))
			continue;
        if (IS_NPC(wch) && IS_AFFECTED(wch, AFF_NOSHOW))
            continue;
		if (is_same_cabal(wch,vch) && wch->in_room->cabal != CABAL_HORDE)
			sprintf(buf, "%sYour thirst for blood overcomes you and you leap at the nearest target!%s\n\r",
				get_char_color(wch,"lightred"),END_COLOR(wch));
		else
			sprintf(buf, "%sYou charge into frenzied battle!%s\n\r",
				get_char_color(wch,"lightred"),END_COLOR(wch));
		send_to_char(buf,wch);
		do_murder(wch,vch->name);
		break;
		}
	}
	}
	if (!IS_NPC(wch) && is_affected(wch,gsn_divine_frenzy) && IS_AWAKE(wch) && !wch->fighting)
	{
		for(vch = wch->in_room->people;vch != NULL; vch = vch_next)
		{
			vch_next = vch->next_in_room;
			
			if(!can_see(wch,vch) || wch == vch)
				continue;
			if(check_stealth(vch,wch))
				continue;
			if(is_safe_new(wch,vch,FALSE))
				continue;
			if (is_affected(vch,skill_lookup("radiance")))
				continue;
			if(!IS_EVIL(vch))
				continue;			
			send_to_char("Righteous anger swells within you as you lash out at the wicked!\n\r",wch);
			do_murder(wch,vch->name);
			break;
		}
	}
	if (is_affected(wch,gsn_mark_of_wrath) && IS_AWAKE(wch) && !wch->fighting
		&& !(wch->desc == NULL && !IS_NPC(wch))) {
		paf = affect_find(wch->affected,gsn_mark_of_wrath);
		for (vch = wch->in_room->people; vch; vch = vch_next) {
			vch_next = vch->next_in_room;
			if (paf->owner->ghost > 0)
				affect_remove(wch,paf);
			if (wch == vch || !can_see(wch,vch))
				continue;
			if (is_affected(vch,skill_lookup("radiance")))
				continue;
			if (is_safe_new(wch,vch,FALSE))
				continue;
			if (vch == paf->owner) {
				sprintf(buf, "%sCatching sight of the mark upon %s's brow, you are consumed with wrath!%s\n\r",get_char_color(wch,"lightred"),vch->name,END_COLOR(wch));
				send_to_char(buf,wch);
				do_murder(wch,vch->name);
				break;
			}
		}
	}
	if ( IS_NPC(wch)
	||   wch->level >= LEVEL_IMMORTAL
	||   wch->in_room == NULL
	||   wch->in_room->area->empty)
	    continue;

	for ( ch = wch->in_room->people; ch != NULL; ch = ch_next )
	{
	    int count;

	    ch_next	= ch->next_in_room;

	    if ( !IS_NPC(ch)
	    ||   !IS_SET(ch->act, ACT_AGGRESSIVE)
	    ||   IS_SET(ch->in_room->room_flags,ROOM_SAFE)
	    ||   (IS_NPC(ch) && IS_AFFECTED(ch, AFF_NOSHOW))
	    ||   IS_AFFECTED(ch,AFF_CALM)
	    ||   ch->fighting != NULL
	    ||   IS_AFFECTED(ch, AFF_CHARM)
	    ||   !IS_AWAKE(ch)
	    ||   ( IS_SET(ch->act, ACT_WIMPY) && IS_AWAKE(wch) )
	    ||   !can_see( ch, wch )
	    ||   number_bits(1) == 0)
		continue;

	    /*
	     * Ok we have a 'wch' player character and a 'ch' npc aggressor.
	     * Now make the aggressor fight a RANDOM pc victim in the room,
	     *   giving each 'vch' an equal chance of selection.
	     */
	    count	= 0;
	    victim	= NULL;
	    for ( vch = wch->in_room->people; vch != NULL; vch = vch_next )
	    {
		vch_next = vch->next_in_room;

		if ( !IS_NPC(vch)
		&&   vch->level < LEVEL_IMMORTAL
		&&   !IS_HEROIMM(vch)
		&&   ch->level >= vch->level - 5
		&&   ( !IS_SET(ch->act, ACT_WIMPY) || !IS_AWAKE(vch) )
		&&   can_see( ch, vch )
		&&  (!IS_SET(ch->act,ACT_DIURNAL) || sun >= SUN_SET)
                &&  (!IS_SET(ch->act,ACT_NOCTURNAL) || (sun != SUN_RISE && sun != SUN_LIGHT))
		&& !is_safe_new(ch,vch,FALSE)
		&& !is_affected(vch,skill_lookup("radiance")) )
		{
		    if ( number_range( 0, count ) == 0 )
			victim = vch;
		    count++;
		}
	    }

	    if ( victim == NULL )
			continue;
	    multi_hit( ch, victim, TYPE_UNDEFINED );
    }
	}
    return;
}


int get_age(CHAR_DATA *ch) /* returns age in years. */
{
	float result;
	if(IS_NPC(ch))
		return 1;
	//long time = ch->played + (int) (current_time - ch->logon));
	result  = 20 + (float) (( (float)time_info.month + (float)1) / (float)12) + time_info.year - ch->pcdata->birth_date;
	result *= pc_race_table[ch->race].agemod;
	return (int)result;
}
int get_hours(CHAR_DATA *ch)
{
	return (ch->played + (int) (current_time - ch->logon)) / 3600;
}
/* returns the age name type */
char * get_age_name_new(int age,int racenumber)
{
    char *name;

	age /= 3600;

	/*
		0 - 20:	young
		21 - 120:	mature
		121 - 220:	middle aged
		221 - 300:	old
		301 - 350:	very old
		351 - 1000:	ancient
		1001 -  ~ :	worm-food
	*/

	if (age <= 20)
	{
		name = "young";
	}
	else if (age <= 120)
	{
		name = "mature";
	}
	else if (age <= 220)
	{
		name = "middle aged";
	}
	else if (age <= 300)
	{
		name = "old";
	}
	else if (age <= 350)
	{
		name = "very old";
	}
	else if (age <= 1000)
	{
		name = "ancient";
	}
	else
	{
		name = "worm-food";
	}

    return name;
}

char * get_age_name(CHAR_DATA *ch)
{
	if (IS_NPC(ch))
		return "young";

	return get_age_name_new((ch->played + (int) (current_time - ch->logon)),ch->race);

}

void age_update(void)
{

    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    bool timedied = FALSE;
    char *cname;

    for (ch = char_list; ch != NULL; ch = ch_next)
    {
	ch_next = ch->next;
	if (IS_NPC(ch))
		continue;
	if (IS_IMMORTAL(ch))
		continue;
	if (IS_HEROIMM(ch))
		continue;

	if (ch->pcdata->death_status == HAS_DIED)
	{
		ch->pcdata->death_timer -= 1;
		if (ch->pcdata->death_timer > 0)
			continue;
		else
		{
	    	act("$n slowly fades away as $s souls departs the mortal planes.",ch,0,0,TO_ROOM);
	    	send_to_char("Your soul finally departs the mortal planes.\n\r",ch);
            wiznet("$N has finally died of old age.",ch,NULL,0,0,0);
            perm_death_log(ch,3);
			plug_graveyard(ch, 2);
			ch->pause = 0;
            stop_fighting(ch,TRUE);
	    	cname = talloc_string(ch->true_name);
	    	do_quit_new(ch,"",TRUE);
	    	delete_char(cname,TRUE);
		}
		continue;
	}
	
	if(ch->pcdata->roll_time + CHARACTER_TIME_DEATH < current_time)
	{
		timedied = TRUE;
		send_to_char("You breathe your last as the long years you have lived finally take their toll.\n\r",ch);
	}
	
	if (get_hours(ch) < ch->pcdata->death_time && !timedied)
		continue;
	act("$n closes $s eyes for the final time as age catches up with $m at last.",ch,0,0,TO_ROOM);
	send_to_char("You close your eyes for the final time as age catches up with you at last.\n\r",ch);
	age_death(ch);	
	ch->pcdata->death_timer = 50;
	ch->pcdata->death_status = HAS_DIED;
    wiznet("$N has become an old age ghost.",ch,NULL,0,0,0);
	timedied = FALSE;
	}
return;
}


/*
 * Handle all kinds of updates.
 * Called once per pulse from game loop.
 * Random times to defeat tick-timing clients and players.
 */

void update_handler( void )
{
    static  int     pulse_area;
    static  int     pulse_mobile;
    static  int     pulse_violence;
    static  int     pulse_point;
    static  int	    pulse_track;
    static  int     pulse_iprog_pulse;
	static	int		pulse_room;
	static	int		pulse_weather;
	static	int		pulse_berus;
	static	int		pulse_calabren;

    int pepe;

    if ( --pulse_iprog_pulse <= 0) {
		pepe=pulse_point;
		pulse_iprog_pulse = PULSE_IPROG_PULSE;
        if(--pepe<=0)
                iprog_pulse_update(TRUE);
        else
                iprog_pulse_update(FALSE);
		
		RS.Queue.ProcessQueue();
		//execute_queue();	//execute queue every second.	
    }

    if ( --pulse_area     <= 0 ) {
		pulse_area	= PULSE_AREA;
//		area_update	( );
    }

    if ( --pulse_track <= 0) {
		pulse_track = PULSE_TRACK;
		track_update	( );
    }
	
    if ( --pulse_mobile   <= 0 ) {
		pulse_mobile	= PULSE_MOBILE;
		mobile_update	( );
		affect_update	( );
    }

	if ( --pulse_room <= 0 ) {
		pulse_room = PULSE_RAFFECT;
		room_affect_update( );
	}
	
	if ( --pulse_weather <= 0) {
		pulse_weather = PULSE_WEATHER;
		weather_update( );
	}

	if ( --pulse_berus <= 0) {
		pulse_berus = PULSE_BERUS;
		berus_update ( );
	}

	if ( --pulse_calabren <= 0) {
		pulse_calabren = PULSE_CALABREN;
		calabren_update ( );
	}
	
    if ( --pulse_violence <= 0 ) {
		pulse_violence	= PULSE_VIOLENCE;
		violence_update	( );
    }

    if ( --pulse_point    <= 0 ) {
		wiznet("TICK!",NULL,NULL,WIZ_TICKS,0,0);
		pulse_point     = number_range( PULSE_TICK - 5, PULSE_TICK + 5);
		time_update	( );
		char_update	( );
		rune_update	( );
		area_update ( );
		obj_update	( );
		age_update	( );
//		save_demos	( );
		room_update ( );
		update_db_gold();
/* This re-adjusts current mobs gold they carry.  No good. */
		gold_update ( );
    }

    aggr_update( );
	tail_chain( );
    return;
}

void do_forcetick(CHAR_DATA *ch, char *argument)
{
	wiznet("TICK!",NULL,NULL,WIZ_TICKS,0,0);
	time_update	( );
	char_update	( );
	rune_update	( );
	area_update ( );
	weather_update ( );
	obj_update	( );
	age_update	( );
	track_update( );
//	save_demos	( );
	room_update     ( );
	gold_update ( );
	
	return;
}

void affect_update ( void )
{
	CHAR_DATA *ch, *ch_next;
	OBJ_DATA *obj, *obj_next;
	ROOM_INDEX_DATA *room;
	AREA_DATA *area;
	AFFECT_DATA *paf, *paf_next;
	ROOM_AFFECT_DATA *raf, *raf_next;
	OBJ_AFFECT_DATA *oaf, *oaf_next;
	AREA_AFFECT_DATA *aaf, *aaf_next;

	for (ch = char_list; ch; ch = ch_next) {
		ch_next = ch->next;
        	//Dev's super-fly cheap name hax!
        	if(!IS_NPC(ch) && strcmp(ch->true_name,ch->backup_true_name))
	        {
			char buf[MSL];
                	free_pstring(ch->true_name);
                	free_pstring(ch->name);
                	ch->name = palloc_string(ch->backup_true_name);
                	ch->true_name = palloc_string(ch->backup_true_name);
                	wiznet("ERROR: $N had corrupt name!  Name has been properly repaired.  Check for other errors.",ch,NULL,0,0,0);
			sprintf(buf,"Error: %s has corrupt name.  Repaired.\n\r",ch->true_name);
			bug(buf,0);
			log_string(buf);
        	}
		for (paf = ch->affected; paf; paf = paf_next) {
			paf_next = paf->next;
			if (paf->pulse_fun)
				(*paf->pulse_fun)(ch,paf);
		}
	}

	for (obj = object_list; obj; obj = obj_next) {
		obj_next = obj->next;
		for (oaf = obj->affected; oaf; oaf = oaf_next) {
			oaf_next = oaf->next;
			if (oaf->pulse_fun)
				(*oaf->pulse_fun)(obj,oaf);
		}
	}

	for (room = top_affected_room; room; room = room->aff_next) {
		for (raf = room->affected; raf; raf = raf_next) {
			raf_next = raf->next;
			if (raf->pulse_fun)
				(*raf->pulse_fun)(room,raf);
		}
		if (IS_SET(room->progtypes, RPROG_PULSE))
			(room->rprogs->pulse_prog) (room);
	}
	
	for (area = area_first; area; area = area->next) {
		for (aaf = area->affected; aaf; aaf = aaf_next) {
			aaf_next = aaf->next;
			if (aaf->pulse_fun)
				(*aaf->pulse_fun)(area,aaf);
		}
		if (IS_SET(area->progtypes, APROG_PULSE))
			(area->aprogs->pulse_prog) (area);

	}

	return;
}

void room_update( void )
{
    ROOM_INDEX_DATA *room;
    ROOM_INDEX_DATA *room_next;

    for ( room = top_affected_room; room ; room = room_next )
    {
	ROOM_AFFECT_DATA *paf;
	ROOM_AFFECT_DATA *paf_next;

	room_next = room->aff_next;

	for ( paf = room->affected; paf != NULL; paf = paf_next )
	{
	    paf_next	= paf->next;
		if ( paf->duration != 0)
			if (paf->tick_fun)
				(*paf->tick_fun) (room,paf);
	    if ( paf->duration > 0 )
			paf->duration--;
        else if ( paf->duration < 0 )
			;
	    else
			affect_remove_room( room, paf );
	}

    }
    return;
}

void room_affect_update( void )
{
	ROOM_INDEX_DATA *room, *to_room;
	CHAR_DATA *victim, *v_next, *vch, *ch;
	int i, dam,chance, roomcount = 0;
	AREA_AFFECT_DATA *aaf;
	ROOM_AFFECT_DATA *af, *af2;
	AFFECT_DATA *paf, cvaf, cvaf2;
	OBJ_DATA *obj;
	char buf[MAX_STRING_LENGTH];

	for (room = top_affected_room; room ; room = room->aff_next ) {

	if(is_affected_room(room,gsn_gravity_well))
	{
		int distance, direction;
		ROOM_INDEX_DATA *droom = room, *prevroom;
		OBJ_DATA *well;
		char *dirname;

		for ( af = room->affected; af != NULL; af = af->next )
			if (af->type == gsn_gravity_well)
				break;

		for(well = object_list; well!=NULL; well = well->next)
			if(well->item_type == ITEM_GRAVITYWELL && well->in_room && well->in_room == room)
				break;
		if(!well)
			continue;
		for(direction = 0; direction<=5; direction++)
		{
			if(!room->exit[direction])
				continue;
			droom = room->exit[direction]->u1.to_room;
			prevroom = room;
			for(distance = 0; distance <= get_grav_distance(well); distance++)
			{
				if(droom == prevroom || droom == room || droom == NULL)
					break;
				if(droom->people)
				{
					for(victim = droom->people; victim != NULL; victim = v_next)
					{
						v_next = victim->next_in_room;
						if(IS_NPC(victim) && IS_SET(victim->act,ACT_SENTINEL))
							continue;
						if(!IS_NPC(victim) && is_safe_new(af->owner,victim,FALSE))
							continue;
						if(victim->invis_level>LEVEL_HERO)
							continue;
						if(IS_NPC(victim))
							SET_BIT(victim->act, ACT_WANDER);
						char_from_room(victim);
						char_to_room(victim, prevroom);
						dirname = (char*)flag_name_lookup(reverse_d(direction),direction_table);
						act("A powerful force drags you inexorably $T.",victim,0,dirname,TO_CHAR);
						do_look(victim,"auto");
						act("An invisible force drags $n into the room from the $T.",
							victim,0,flag_name_lookup(direction,direction_table),TO_ROOM);
					}
				}
                                if(!droom->exit[direction] || !droom->exit[direction]->u1.to_room ||
					droom->exit[direction]->u1.to_room->area!=droom->area ||
					IS_SET(droom->exit[direction]->exit_info, EX_CLOSED))
                                        break;
				prevroom = droom;
				droom = droom->exit[direction]->u1.to_room;
							
			}
		}
	}

	if (is_affected_room(room,gsn_vacuum))
	{
		for ( af = room->affected; af != NULL; af = af->next )
		{
			if (af->type == gsn_vacuum)
				break;
		}
		af->modifier++;
		if (af->modifier > 2)
		{
			roomcount = 0;
			for (i=0; i<=5; i++)
				if (room->exit[i] && !IS_SET(room->exit[i]->exit_info, EX_CLOSED))
					roomcount++;

			if (roomcount == 0)
			{
				for (vch = room->people; vch != NULL; vch = v_next)
				{
					v_next = vch->next_in_room;
					if (!IS_NPC(vch) && is_safe_new(af->owner,vch,FALSE))
						continue;
					send_to_char("Your lungs burn furiously, desperate for air!\n\r",vch);
					damage_new(vch,vch,dice(10,5)+5,TYPE_HIT + attack_lookup("asphyxiation"),DAM_INTERNAL,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);
				}
			} else {
				affect_remove_room(room,af);
			}
		}
	}

	if (is_affected_room(room,gsn_earthquake))
	{

		for ( af = room->affected; af != NULL; af = af->next )
		{
			if (af->type == gsn_earthquake)
				break;
		}
		if (number_range(1,4) == 1) {
		for (vch = room->people; vch != NULL; vch = v_next)
		{
			v_next = vch->next_in_room;

			if (is_safe_new(af->owner,vch,FALSE))
				continue;
			dam = dice(10,15);
			if (is_affected(vch,gsn_earthshield)) {
				dam /= 2;
			}
			send_to_char("Large chunks of rubble tumble down from the ceiling, crushing you!\n\r",vch);
			damage_new(af->owner,vch,dam,TYPE_UNDEFINED,DAM_BASH,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"the falling debris*");
		}
		}
	}

	if (is_affected_room(room,gsn_tidalwave))
	{
		for ( af = room->affected; af != NULL; af = af->next )
		{
			if (af->type == gsn_tidalwave && af->location == APPLY_ROOM_NONE)
				break;
		}

		if (af->modifier == 1)
		{
			for (vch = room->people; vch != NULL; vch = vch->next_in_room)
			{
				sprintf(buf,"%sA towering tidal wave obscures the entire horizon, approaching rapidly!%s\n\r", get_char_color(vch,"cyan"), END_COLOR(vch));
				send_to_char(buf,vch);
			}
			af->modifier--;
		}
		else if (af->modifier == 0)
		{
			for ( af2 = room->affected; af2 != NULL; af2 = af2->next )
			{
				if (af2->type == gsn_tidalwave && af2->location == APPLY_ROOM_NOPE)
					break;
			}
			for (vch = room->people; vch != NULL; vch = vch->next_in_room)
			{
				sprintf(buf,"%sA massive tidal wave rolls in, engulfing the area!%s\n\r", get_char_color(vch,"cyan"), END_COLOR(vch));
				send_to_char(buf,vch);
			}
			for (vch = room->people; vch != NULL; vch = v_next)
			{
				v_next = vch->next_in_room;
				if (vch == af->owner) continue;
				if (!IS_NPC(vch) && is_safe_new(af->owner,vch,FALSE))
					continue;
				if (!IS_NPC(vch))
				{
				if (vch->in_room == af->owner->in_room)
				{
					sprintf(buf,"Help! I'm being drowned by %s's tidal wave!",PERS(af->owner,vch));
					do_myell(vch,buf, NULL);
				} else {
					sprintf(buf,"Help! I'm being drowned by a tidal wave!");
					do_myell(vch,buf, NULL);
				}
				}
				damage_new(af->owner,vch,dice(af2->modifier,10),gsn_tidalwave,DAM_DROWNING,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"the tidal wave*");
			}
			if (room->sector_type != SECT_WATER)
			{
			for (vch = room->people; vch != NULL; vch = vch->next_in_room)
			{
				sprintf(buf,"%sAs the tidal wave reaches land, the accumulated mass of water comes crashing down upon you!%s\n\r", get_char_color(vch,"cyan"), END_COLOR(vch));
				send_to_char(buf,vch);
			}
			for (vch = room->people; vch != NULL; vch = v_next)
			{
				v_next = vch->next_in_room;
				if (vch == af->owner) continue;
				if (!IS_NPC(vch) && is_safe_new(af->owner,vch,FALSE))
					continue;
				damage_new(af->owner,vch,dice(af2->modifier,10),gsn_tidalwave,DAM_BASH,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"the crashing wave*");
				WAIT_STATE(vch,PULSE_VIOLENCE*2);
			}
			}

			affect_remove_room(room,af);
			affect_remove_room(room,af2);
		}
		else
			af->modifier--;
	}

	if (is_affected_room(room, gsn_caustic_vapor)) {
		if (room->people) {
		for (vch = room->people; vch != NULL; vch = v_next) {
			v_next = vch->next_in_room;
			for (af = vch->in_room->affected; af != NULL; af = af->next) {
				if (af->type == gsn_caustic_vapor)	break;
			}

			if (IS_IMMORTAL(vch))
				continue;

			if (!is_safe(vch,af->owner) && !is_affected(vch,gsn_neutralize)) {
				if (!is_affected(vch,gsn_noxious_fumes)) {
					init_affect(&cvaf);
					cvaf.where      =   TO_AFFECTS;
					cvaf.type       =   gsn_noxious_fumes;
					cvaf.aftype     =   AFT_MALADY;
					cvaf.level      =   af->level;
					cvaf.duration   =   4;
					cvaf.location	=	0;
					cvaf.modifier	=	0;
					cvaf.owner		=	af->owner;
					new_affect_to_char(vch,&cvaf);
				}
	
				for (paf = vch->affected; paf != NULL; paf = paf->next) {
					if (paf->type == gsn_noxious_fumes) break;
				}
	
				paf->modifier = URANGE(0,paf->modifier,5);
	
				paf->modifier++;
	
				init_affect(&cvaf2);
				cvaf2.where     =   TO_AFFECTS;
				cvaf2.type      =   gsn_noxious_fumes;
				cvaf2.aftype    =   AFT_MALADY;
				cvaf.level      =   af->level;
				cvaf.duration   =   1;
				cvaf.location	=	0;
				cvaf.modifier	=	0;
				cvaf.owner		=	af->owner;
			
				switch(paf->modifier) {
					case(1):
						dam = dice(af->level/3,2);
						break;
					case(2):
						dam = dice(af->level/3,3);
						break;
					case(3):
						dam = dice(af->level/3,3);
						send_to_char("A wave of nausea washes over you as you inhale the vapors.\n\r",vch);
						SET_BIT(cvaf2.bitvector, AFF_POISON);
						break;
					case(4):
						dam = dice(af->level/2,2);
						SET_BIT(cvaf2.bitvector, AFF_POISON);
						break;
					case(5):
						dam = dice(af->level/2,2);
						send_to_char("The fumes sear your eyes, blinding you!\n\r",vch);
						SET_BIT(cvaf2.bitvector, AFF_POISON);
						SET_BIT(cvaf2.bitvector, AFF_BLIND);
						break;
					default:
						dam = dice(af->level/2,3);
						SET_BIT(cvaf2.bitvector, AFF_POISON);
						SET_BIT(cvaf2.bitvector, AFF_BLIND);
						break;
				}

				if (paf->modifier >= 3)
					cvaf2.tick_fun = poison_tick;

				new_affect_join(vch,&cvaf2);
				
				damage_new(af->owner,vch,dam,af->type,DAM_POISON,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"the noxious fumes*$");
			}
		}
		}
	}
	
	}

	for(room = room_list; room != NULL; room = room->next_room)
	{
	//cyke
	
	if(is_affected_area(room->area, gsn_cyclone))	
	{
		for(aaf = room->area->affected;aaf;aaf = aaf->next) {
			if (aaf->type == gsn_cyclone)	break;
		}

		for(obj = room->contents; obj!=NULL; obj = obj->next_content)
		{
			if(!obj->in_room || obj->in_room != room)
				continue;
			if(obj->weight>5 || number_percent()>((obj->weight+1)*3) || !IS_SET(obj->wear_flags,ITEM_TAKE))
				continue;
                        to_room = get_random_exit(room);
                        if(!to_room || (!room->people && !to_room->people))
                                break;
			if(room->people)
				act("The violent winds blow $p away!",room->people,obj,0,TO_ALL);
			obj_from_room(obj);
			obj_to_room(obj,to_room);
			obj->moved = TRUE;
			if(!to_room->people)
				continue;
			act("The violent winds blow $p in!",to_room->people,obj,0,TO_ALL);
			victim = (CHAR_DATA*)get_random_ch(aaf->owner, to_room);
			if(!victim)
				continue;
			if(victim == aaf->owner)
				continue;
			dam = dice(obj->weight+1,8);
			damage_new(aaf->owner,victim,dam,skill_lookup("cyclone"),DAM_BASH,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"the flying debris*");
		}
		for(victim = room->people; victim!=NULL; victim = v_next)
		{
			v_next = victim->next_in_room;
			chance = 8;
			if(IS_AFFECTED(victim, AFF_FLYING))
				chance = 35;
			if(is_affected(victim,gsn_airshield))
				chance = 0;
			if(number_percent()>=chance || (IS_NPC(victim) && IS_SET(victim->act, ACT_SENTINEL)) ||
			victim->fighting || victim->invis_level>LEVEL_HERO)
				continue;
			to_room = get_random_exit(room);
		
			if(!to_room)
				break;
			if ((to_room->sector_type == SECT_AIR && !IS_AFFECTED(victim,AFF_FLYING)) || to_room->area->area_type == ARE_UNOPENED) break;
			send_to_char("The violent winds buffet you out of the room!\n\r",victim);
			act("The violent winds buffet $n out of the room!",victim,0,0,TO_ROOM);
			char_from_room(victim);
			char_to_room(victim, to_room);
			do_look(victim,"auto");
			act("The violent winds buffet $n into the room!",victim,0,0,TO_ROOM);
		}
	}
	}
	for(ch = char_list; ch!=NULL; ch=ch->next)
	{
		dam = 0;
		for(obj = ch->carrying; obj!=NULL; obj=obj->next_content)
			if (IS_AFFECTED(obj, AFF_OBJ_BURNING))
				dam += is_worn(obj) ? 8 : 4;
	if(dam && !ch->ghost)
	{
		send_to_char("You scream in pain as your burning equipment sears your flesh!\n\r",ch);
		damage_new(ch,ch,dam,gsn_immolate,DAM_FIRE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"burning armor");
	}
	}
	return;
}


void ayell_update( void )
{
	return;
}

/* iprog pulse update, also drift/sink code plugged in */
void iprog_pulse_update(bool isTick)
{
	char *direction;
	OBJ_DATA *obj, *obj_next;
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	int door;
	
	for ( obj = object_list; obj != NULL; obj = obj_next )
	{
		obj_next = obj->next;

		/* items drifting/sinking in water -- Dioxide */
		if (obj->in_room && obj->in_room->sector_type == SECT_WATER && material_table[obj->pIndexData->material_index].mat_density == 0 && IS_SET(obj->wear_flags, ITEM_TAKE) && !IS_OBJ_STAT(obj,ITEM_NOPURGE))
		{
			if (number_range(0,60) == 0)
			{
			if (!((obj->item_type == ITEM_CORPSE_PC || obj->item_type == ITEM_CORPSE_NPC) && obj->contains))
			{
			door = number_range(0,3);
			pexit = obj->in_room->exit[door];
			if (pexit != NULL && (to_room = pexit->u1.to_room) != NULL && to_room->sector_type == SECT_WATER)
			{
				if (obj->in_room->people) {
					direction = (char*) flag_name_lookup(door,direction_table);
					act("$p drifts $Tward.",obj->in_room->people,obj,direction,TO_ALL);
				}
				if (to_room->people) {
					act("$p drifts in.",to_room->people,obj,0,TO_ALL);
				}
				
				obj_from_room(obj);
				obj_to_room(obj,to_room);
			}
			}
			}
		}
		if (obj->in_room && (obj->in_room->sector_type == SECT_WATER || obj->in_room->sector_type == SECT_UNDERWATER) && (material_table[obj->pIndexData->material_index].mat_density > 0 || obj->item_type == ITEM_CORPSE_PC || obj->item_type == ITEM_CORPSE_NPC) && obj->item_type != ITEM_BOAT && IS_SET(obj->wear_flags, ITEM_TAKE) && !IS_OBJ_STAT(obj,ITEM_NOPURGE))
		{
			if (number_range(0,5) == 0)
			{
				if ((obj->item_type == ITEM_CORPSE_PC || obj->item_type == ITEM_CORPSE_NPC) && obj->contains == NULL)
				{
					door = DIR_UP;
				} else {
					door = DIR_DOWN;
				}
				pexit = obj->in_room->exit[door];
				if (pexit != NULL && (to_room = pexit->u1.to_room) != NULL)
				{
					if (door == DIR_UP)
					{
						if (obj->in_room->people && obj->in_room->sector_type == SECT_UNDERWATER) {
							direction = (char*)flag_name_lookup(door,direction_table);
							act("$p floats $Tward.",obj->in_room->people,obj,direction,TO_ALL);
						}

						if (to_room->sector_type == SECT_WATER && to_room->people)
							act("$p suddenly bobs up and surfaces.",to_room->people,obj,0,TO_ALL);
						else if (to_room->people)
							act("$p floats in from below.",to_room->people,obj,0,TO_ALL);
					} else {
						if (obj->in_room->people && obj->in_room->sector_type == SECT_WATER)
							act("$p sinks beneath the surface.",obj->in_room->people,obj,0,TO_ALL);
						else if (obj->in_room->people && obj->in_room->sector_type == SECT_UNDERWATER) {
							direction = (char*)flag_name_lookup(door,direction_table);
							act("$p floats $Tward.",obj->in_room->people,obj,direction,TO_ALL);
						}

						if (to_room->people)
							act("$p sinks in from above.",to_room->people,obj,0,TO_ALL);
					}
					obj_from_room(obj);
					obj_to_room(obj,to_room);
				}
			}
		}

		
		if ((obj->in_room != NULL && obj->in_room->area->nplayer > 0) || 
				(obj->carried_by && obj->carried_by->in_room && obj->carried_by->in_room->area->nplayer > 0))
		{
			if(IS_SET(obj->progtypes, IPROG_PULSE))
				(obj->pIndexData->iprogs->pulse_prog) (obj,isTick);
			if(TRAPS_IEVENT(obj,TRAP_IPULSE))
				CALL_IEVENT(obj,TRAP_IPULSE,obj,isTick);
		}
	}
}

bool do_mob_cast(CHAR_DATA *ch)
{
	sh_int i, sn, rnd, in_room = 0, room_occupant = 0;
	CHAR_DATA *vch, *victim;

	if(!IS_NPC(ch) || !ch->fighting || ch->pIndexData->cast_spell[0]==NULL)
		return FALSE;

	for(;;)
	{
		i = number_range(0,9);
		if(ch->pIndexData->cast_spell[i]!=NULL)
			break;
	}

	sn = skill_lookup(ch->pIndexData->cast_spell[i]);
	
	if(sn<1)
		return FALSE;

	victim = ch->fighting;
	//Share the wealth.
	//
	if(skill_table[sn].target == TAR_CHAR_OFFENSIVE)
	{
		for(victim = ch->in_room->people; victim != NULL; victim = victim->next_in_room)
			if(victim && !IS_NPC(victim) && victim->fighting && victim->fighting==ch)
				in_room++;
		rnd = number_range(1,in_room);
		for(vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
		{
			if(!IS_NPC(vch) && vch->fighting == ch)
				room_occupant++;
			if(rnd == room_occupant)
				break;
		}
		victim = vch != NULL ? vch : ch->fighting;
	}
	if(skill_table[sn].target == TAR_CHAR_DEFENSIVE)
		victim = ch;

	if(IS_SET(ch->act, ACT_CLERIC))
	{
		if(skill_table[sn].target == TAR_CHAR_DEFENSIVE)
			act("$n closes $s eyes with a look of concentration for a moment.",ch,0,0,TO_ROOM);
		else
		{
			act("You narrow your eyes and glare in $N's direction.",ch,0,victim,TO_CHAR);
        	act("$n narrows $s eyes and glares in $N's direction.",ch,0,victim,TO_NOTVICT);
			act("$n narrows $s eyes and glares in your direction.",ch,0,victim,TO_VICT);
		}
	}
	else if (!IS_SET(ch->form,FORM_NOSPEECH))
		say_spell(ch,sn);
	
	(*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim, TAR_CHAR_OFFENSIVE);

	return TRUE;
}

void save_votes( )
{
	FILE *fp;
	BALLOT_DATA *ballot;
	VOTE_DATA *vote;
        if(!(fp=fopen(VOTE_FILE,"w")))
                return;
	for(ballot=ballot_first; ballot!=NULL; ballot=ballot->next)
	{
		fprintf(fp,"#%s~\n",ballot->name);
		for(vote=ballot_first->first_vote; vote!=NULL; vote=vote->next)
			fprintf(fp,"%s %s %s~ %s~\n",vote->voter,vote->vote_for,vote->time,vote->ip);
		fprintf(fp,"ENDVOTES\n");
	}
	fprintf(fp,"$");
	fclose(fp);
}

int reverse_d(int dir)
{
	if(dir == DIR_NORTH)
		return DIR_SOUTH;
        if(dir == DIR_SOUTH)
                return DIR_NORTH;
        if(dir == DIR_WEST)
                return DIR_EAST;
        if(dir == DIR_EAST)
                return DIR_WEST;
        if(dir == DIR_UP)
                return DIR_DOWN;
        if(dir == DIR_DOWN)
                return DIR_UP;
	return 0;
}

int get_grav_distance(OBJ_DATA *obj)
{
	if(obj->weight < 10)
		return 0;

	if(obj->weight < 20)
		return 1;

	if(obj->weight < 30)
		return 2;

	if(obj->weight < 40)
		return 3;

	return 4;
}

ROOM_INDEX_DATA *get_random_exit(ROOM_INDEX_DATA *room)
{
	int i, exit;
	for(i = 0; i < 100; i++)
		if(room->exit[(exit=number_range(0,5))])
			return room->exit[exit]->u1.to_room;
	return NULL;
}

CHAR_DATA *get_random_ch(CHAR_DATA *ch, ROOM_INDEX_DATA *room)
{
    CHAR_DATA *vch, *victim = NULL;
    int now = 0, highest = 0;
    for( vch = room->people; vch; vch = vch->next_in_room )
    {
        if (!IS_NPC(vch) && !is_safe_new(ch,vch,FALSE) && vch->invis_level<LEVEL_HERO+1
	&& ( now = number_percent() ) > highest)
        {
            victim = vch;
            highest = now;
        }
    }
    return victim;
}

CHAR_DATA *get_rand_from_room(ROOM_INDEX_DATA *room)
{
	CHAR_DATA *vch, *victim = NULL;
	int now = 0, highest = 0;
	
	for( vch = room->people; vch; vch = vch->next_in_room ) {
		if ((now = number_percent() ) > highest) {
			victim = vch;
			highest = now;
		}
	}
	return victim;
}

void save_demos()
{
/*	MYSQL *conn;
	MYSQL_RES *res_set;
	FILE *fp;
	char query[MSL];
	int i, total, affected, affected2;
	float perc, perc2, perc3;
	//gold constant....
*/
	FILE *fp;
	if(!(fp=fopen("gold.txt","w")))
		return;
	fprintf(fp,"%ld",gold_constant);
	fclose(fp);
/*
	if(!(fp=fopen("demo.txt","w")))
		return bug("Error opening demographics.",0);
	conn = open_conn();
	//racial crap
	fprintf(fp,"Riftshadow Demographics\n\r");
	fprintf(fp,"Racial Statistics:\n\r");
	mysql_query(conn,"SELECT count(level) FROM players where level>10");
	res_set = mysql_store_result(conn);
	total = mysql_num_rows(res_set);
	mysql_free_result(res_set);
	for(i=1;i<MAX_PC_RACE;i++)
	{
		sprintf(query,"SELECT level FROM players WHERE race=%d and level>10",i);
		mysql_query(conn,query);
		res_set = mysql_store_result(conn);
		affected = mysql_num_rows(res_set);
		mysql_free_result(res_set);
		perc = (affected * 100) / total;
		fprintf(fp,"%-12s %-4d %3.0f%%   ",capitalize(pc_race_table[i].name),affected,perc);
		if(i % 3 == 0 || i==MAX_PC_RACE-1)
			fprintf(fp,"\n\r")
	}
	fprintf(fp,"Class Statistics:\n\r");
	for(i=0;i<MAX_CLASS;i++)
	{
		sprintf(query,"SELECT level FROM players WHERE class=%d and level>10",i);
		mysql_query(conn,query);
		res_set = mysql_store_result(conn);
		affected = mysql_num_rows(res_set);
		mysql_free_result(res_set);
		perc = (affected * 100) / total;
		fprintf(fp,"%-12s %-4d %3.0f%%   ",capitalize(class_table[i].name),affected,perc);

		if(i>0 && ((i+1) % 3 == 0 || i==MAX_CLASS-1))
			fprintf(fp,"\n\r");
	}
	fprintf(fp,"Cabal Statistics:\n\r");
        for(i=1;i<MAX_CABAL;i++)
        {
                sprintf(query,"SELECT level FROM players WHERE cabal=%d and level>10",i);
                mysql_query(conn,query);
                res_set = mysql_store_result(conn);
                affected = mysql_num_rows(res_set);
				mysql_free_result(res_set);
				perc = (affected * 100) / total;
				fprintf(fp,"%-12s %-4d %3.0f%%   ",capitalize(cabal_table[i].name),affected,perc);
                if(i % 3 == 0 || i==MAX_CABAL-1)
                        fprintf(fp,"\n\r");
        }
	mysql_query(conn,"SELECT level FROM players WHERE sex=1 and level>10");
	res_set = mysql_store_result(conn);
	affected = mysql_num_rows(res_set);
    mysql_free_result(res_set);
	perc = (affected * 100)/ total;
	perc2 = ((total-affected) * 100)/total;
	fprintf(fp,"Sex Statistics:\n\r");
	fprintf(fp,"Male         %-4d %3.0f%%   Female       %-4d %3.0f%%\n\r",affected,perc,(total-affected),perc2);
	fprintf(fp,"Alignment Statistics:\n\r");
    mysql_query(conn,"SELECT level FROM players WHERE align>0 and level>10");
    res_set = mysql_store_result(conn);
    affected = mysql_num_rows(res_set);
	mysql_free_result(res_set);
	perc = (affected * 100)/ total;
    mysql_query(conn,"SELECT level FROM players WHERE align<0 and level>10");
    res_set = mysql_store_result(conn);
    affected2 = mysql_num_rows(res_set);
   	perc2 = (affected2 * 100) / total;
	perc3 = ((total-affected-affected2) * 100)/total;
	fprintf(fp,"Good         %-4d %3.0f%%   Neutral      %-4d %3.0f%%   Evil         %-4d %3.0f%%\n\r",
	affected, perc, (total-affected-affected2), perc3, affected2, perc2);
	mysql_free_result(res_set);
	fprintf(fp,"\n\rTotal        %d\n\r",total);
	fclose(fp);
	do_disc(conn);*/
}

void rune_update()
{
	RUNE_DATA *rune, *rune_next;

	for(rune = rune_list; rune; rune = rune_next)
	{
		rune_next = rune->next;
		rune->duration--;
		if(rune->duration<=0)
		{
			if(rune->end_fun)
				(*rune->end_fun) (rune);
			extract_rune(rune);
		}
		
	}
}
