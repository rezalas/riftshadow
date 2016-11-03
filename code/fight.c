/* test 1 */
/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *  										                               *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                              										   *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.    										   *
 *                              										   *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.       										   *
 ***************************************************************************/

/***************************************************************************
*       ROM 2.4 is copyright 1993-1996 Russ Taylor                         *
*       ROM has been brought to you by the ROM consortium                  *
*           Russ Taylor (rtaylor@pacinfo.com)                              *
*           Gabrielle Taylor (gtaylor@pacinfo.com)                         *
*           Brian Moore (rom@rom.efn.org)                                  *
*       By using this code, you have agreed to follow the terms of the     *
*       ROM license, in the file Tartarus/doc/rom.license                  *
***************************************************************************/

/***************************************************************************
*       Tartarus code is copyright (C) 1997-1998 by Daniel Graham          *
*	In using this code you agree to comply with the Tartarus license   	   *
*       found in the file /Tartarus/doc/tartarus.doc                       *
***************************************************************************/

/***************************************************************************
*            Riftshadow MUD code copyright (c) 2000-2001 and is			   *
*        	 of course for exclusive use by the Riftshadow MUD.            *
***************************************************************************/

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "interp.h"
#include "tables.h"
#include "magic.h"
#include "recycle.h"
//#include "math.h"
#include "spec.h"

#define HITS (dt==gsn_kick||dt==gsn_throw||dt==gsn_bash)

/* command procedures needed */
DECLARE_DO_FUN(do_quit			);
DECLARE_DO_FUN(do_say			);
DECLARE_DO_FUN(do_backstab      );
DECLARE_DO_FUN(do_emote         );
DECLARE_DO_FUN(do_ambush		);
DECLARE_DO_FUN(do_berserk       );
DECLARE_DO_FUN(do_deathstyle    );
DECLARE_DO_FUN(do_bash          );
DECLARE_DO_FUN(do_trip          );
DECLARE_DO_FUN(do_dirt          );
DECLARE_DO_FUN(do_flee          );
DECLARE_DO_FUN(do_kick          );
DECLARE_DO_FUN(do_disarm        );
DECLARE_DO_FUN(do_get           );
DECLARE_DO_FUN(do_recall        );
DECLARE_DO_FUN(do_sacrifice     );
DECLARE_DO_FUN(do_circle_stab	);
DECLARE_DO_FUN(do_look);
DECLARE_DO_FUN(do_tail	);
DECLARE_DO_FUN(do_bind);
DECLARE_DO_FUN(do_unbind);
DECLARE_SPELL_FUN(spell_power_word_kill);

bool    is_wielded              args((CHAR_DATA *ch, int weapon, int type));

#define PEER_BALANCE_DISTANCE	8
#define BASE_PEER_FACTOR	1.75
#define SUMMONED_XP_PENALTY	.55
/*
 * Local functions.
 */
int		check_arms	(CHAR_DATA *ch, OBJ_DATA *wield, bool bOncePerRound);
void    check_assist    args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool    check_dodge     args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
bool	check_piety	args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
bool	check_avoid		args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
int		check_evasion	args( (CHAR_DATA *ch, int chance) );
bool    check_parry     args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
bool    check_shield_block     args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
bool 	check_deflect (CHAR_DATA *ch, CHAR_DATA *victim, int dt);
bool    check_fend     args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
bool	check_mist		args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt) );
int     check_armor     args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt, int dam_type, int dam ) );
bool    check_maneuvering     args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
void	check_batter  (CHAR_DATA *ch);
void	check_analyze (CHAR_DATA *ch, CHAR_DATA *victim);
bool	check_catch	args( ( CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *obj));
void    dam_message     args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam,
                            int dt, bool immune, char *dnoun, bool reduced) );
void    death_cry       args( ( CHAR_DATA *ch ) );
void    group_gain      args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void	pk_record		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
int		xp_compute_pk	args( ( CHAR_DATA *ch, CHAR_DATA *victim, int members));
int     xp_compute      args( ( CHAR_DATA *gch, CHAR_DATA *victim,
                            int group_amount, int glevel ) );
bool    is_safe         args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void    make_corpse     args( ( CHAR_DATA *killer, CHAR_DATA *ch ) );
int     one_hit         args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ));
void    mob_hit         args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
void    raw_kill        args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void    set_fighting    args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void    disarm          args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void 	check_ground_control args( (CHAR_DATA *ch,CHAR_DATA *victim,float chance,int dam) );
bool	check_counter	args( (CHAR_DATA *ch,CHAR_DATA *victim, int dam,int dt) );
bool check_parting_blow args((CHAR_DATA *ch, CHAR_DATA *victim));
int one_hit_new args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt, bool specials, bool blockable, int addition, int multiplier, char *dnoun));
int damage_new args((CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int dam_type, bool show, bool blockable, int addition, int multiplier, char *dnoun));
char *get_dam_message args((CHAR_DATA *ch, int dt));
bool check_blade_barrier args((CHAR_DATA *ch, CHAR_DATA *victim));
char * get_attack_noun args((CHAR_DATA *ch, int dt));
int get_attack_number args((CHAR_DATA *ch, int dt));
void trophy_corpse args((CHAR_DATA *ch, CHAR_DATA *victim));
void warrior_ai (CHAR_DATA *mob, CHAR_DATA *victim);
void thief_ai (CHAR_DATA *mob, CHAR_DATA *victim);
bool can_bash (CHAR_DATA *ch, CHAR_DATA *victim);
int skirmisher_max_weapweight(CHAR_DATA *ch);
extern char *const dir_name[];
extern int get_weapon_sn_new(CHAR_DATA *ch, int type);
/*
 * Control the fights going on.
 * Called periodically by update_handler.
 */
void violence_update( void )
{
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    CHAR_DATA *victim;
    int regen = 0;
    OBJ_DATA *obj;
    AFFECT_DATA *af;

    for ( ch = char_list; ch != NULL; ch = ch->next )
    {
        ch_next = ch->next;
	if (ch->regen_rate != 0)
	{
		if (ch->regen_rate > 0)
			ch->hit = UMIN(ch->hit + number_range(1,ch->regen_rate),ch->max_hit);
		else
			ch->hit = UMIN(ch->hit - number_range(1,ch->regen_rate),ch->max_hit);
	}
	
	if (ch->fighting != NULL) {
		check_analyze(ch,ch->fighting);
		if(!IS_NPC(ch))
			check_style_improve(ch,ch->pcdata->style,6);
	}
		
	if (IS_NPC(ch))
		ch->balance = UMAX(ch->balance - 4,0);
	else
		ch->balance = UMAX(ch->balance - 3,0);

	if (is_affected(ch,gsn_unbalance))
	{
		af = affect_find(ch->affected, gsn_unbalance);
		af->modifier--;
		if (af->modifier < 0)
			affect_strip(ch,gsn_unbalance);
	}

        if ( ( victim = ch->fighting ) == NULL || ch->in_room == NULL )
            continue;

	if (IS_NPC(ch) && (!IS_NPC(ch->fighting))
	&& !IS_SET(ch->off_flags,NO_TRACK)
	&& !IS_AFFECTED(ch,AFF_CHARM)
	&& !IS_SET(ch->act,ACT_IS_HEALER)
	&& !IS_SET(ch->act,ACT_BANKER)) {
		ch->last_fought = ch->fighting;
		ch->tracktimer = 144;
	}

	update_pc_last_fight(ch,ch->fighting);
/*
        if ( IS_AWAKE(ch) && ch->in_room == victim->in_room )
            multi_hit( ch, victim, TYPE_UNDEFINED );
        else
            stop_fighting( ch, FALSE );

        if ( ( victim = ch->fighting ) == NULL )
            continue;
			*/

	check_leech(ch,victim);
	check_baals_mastery(ch,victim);
		
	for (obj = ch->carrying;obj != NULL; obj = obj->next_content)
	{  	
	   if(ch->Class()->GetIndex() == CLASS_PALADIN && obj->item_type == ITEM_WEAPON && is_worn(obj))
			check_arms(ch,obj,TRUE);	//true = once per round
   	   if (IS_SET(obj->progtypes,IPROG_FIGHT))
		(obj->pIndexData->iprogs->fight_prog) (obj,ch);
	   
	   if(TRAPS_IEVENT(obj,TRAP_IFIGHT))
		   CALL_IEVENT(obj,TRAP_IFIGHT,ch,obj);
	}

	if (IS_SET(ch->progtypes,MPROG_FIGHT) && (ch->wait <= 0))
	   	(ch->pIndexData->mprogs->fight_prog) (ch,victim);

        /*
         * Fun for the whole family!
         */
        check_assist(ch,victim);
    }

    return;
}

/* for auto assisting */
void check_assist(CHAR_DATA *ch,CHAR_DATA *victim)
{
    CHAR_DATA *rch, *rch_next;
	char store[MSL];

    for (rch = ch->in_room->people; rch != NULL; rch = rch_next)
    {
        rch_next = rch->next_in_room;

	if (!IS_NPC(rch) && rch->ghost > 0)
		continue;
	if (IS_NPC(rch) && rch->pIndexData->vnum == ACADEMY_PET && ch->ghost > 0)
		continue;
        if (IS_AWAKE(rch) && rch->fighting == NULL)
        {
			/* NPC assisting group (for charm, zombies, elementals..added by Ceran */
		if (IS_NPC(rch) && (IS_AFFECTED(rch,AFF_CHARM)) && is_same_group(rch,ch) )
 		{
			if((rch->pIndexData->vnum == 2979) && (number_percent() > 50)) {
				do_gore(rch,victim->name);
				continue;
			} 
			else 
			{
				set_fighting(rch,victim);
				continue;
			}
		}


		/* quick check for ASSIST_PLAYER */
       	if (!IS_NPC(ch) && IS_NPC(rch)
       	&& IS_SET(rch->off_flags,ASSIST_PLAYERS)
       	&&  rch->level + 6 > victim->level)
       	{
       		do_emote(rch,"screams and attacks!");
            multi_hit(rch,victim,TYPE_UNDEFINED);
            continue;
       	}

      	/* PCs next */
		if ( !IS_NPC(ch) || IS_AFFECTED(ch,AFF_CHARM))
		{
			/* First check defending */
			if (rch->defending != NULL)
			{
				CHAR_DATA *fch;
		
				fch = ch->fighting;
				if (rch->defending != NULL
				&& rch->defending == ch
				&& fch != NULL
				&& fch->fighting == ch
				&& number_percent() < get_skill(rch,gsn_defend)
				&& number_percent() > 40
				&& number_percent() < get_skill(rch,gsn_rescue))
				{
					act("$n leaps to $N's rescue!",rch,0,ch,TO_NOTVICT);
					act("$n leaps to your rescue!",rch,0,ch,TO_VICT);
					act("You leap to $N's rescue!",rch,0,ch,TO_CHAR);

    				if (is_safe(rch,fch))
						return;

    				WAIT_STATE(rch,PULSE_VIOLENCE);
    				WAIT_STATE(ch,2*PULSE_VIOLENCE);
    				check_improve(rch,gsn_defend,TRUE,1);
					stop_fighting( fch, FALSE );
    				stop_fighting( ch, FALSE );
					set_fighting( rch, fch );
					set_fighting( fch, rch );
					
					if (IS_NPC(fch) && (IS_CABAL_GUARD(fch)))
						set_fighting(ch,fch);
					continue;
				}
			}
		

            if ( ( (!IS_NPC(rch) && IS_SET(rch->act,PLR_AUTOASSIST))
            ||     IS_AFFECTED(rch,AFF_CHARM))
            &&   is_same_group(ch,rch)
            &&   !is_safe(rch, victim))
			{
				if(!IS_NPC(ch) && !IS_NPC(rch) && !IS_NPC(victim))
				{
					sprintf(store,"Help!  %s is assisting %s!", rch->name, ch->name);
					victim->law_pass = TRUE;
					do_myell(victim, store, rch);
				}
				set_fighting(rch,victim);
			}
            continue;
            }

            /* now check the NPC cases */

            if (IS_NPC(ch) && !IS_AFFECTED(ch,AFF_CHARM))
            {
                if ( (IS_NPC(rch) && IS_SET(rch->off_flags,ASSIST_ALL))
                ||   (IS_NPC(rch) && rch->group && rch->group == ch->group
				     && IS_SET(rch->off_flags, ASSIST_GROUP))
                ||   (IS_NPC(rch) && rch->race == ch->race
                   		     && IS_SET(rch->off_flags,ASSIST_RACE))
                ||   (IS_NPC(rch) && IS_SET(rch->off_flags,ASSIST_ALIGN)
				&&   ((IS_GOOD(rch)    && IS_GOOD(ch))
	                 	     ||  (IS_EVIL(rch)    && IS_EVIL(ch))
        		             ||  (IS_NEUTRAL(rch) && IS_NEUTRAL(ch))))
                ||   (rch->pIndexData == ch->pIndexData
		                     && IS_SET(rch->off_flags,ASSIST_VNUM)))

                {
                    CHAR_DATA *vch;
                    CHAR_DATA *target = NULL;
		    char buf[300], buf2[300], *pbuf;
                    int number = 0, tnumber;

                    if (number_bits(1) == 0)
                        continue;

		    for(vch = ch->in_room->people; vch; vch = vch->next)
			if (can_see(rch,vch) && is_same_group(vch,victim))
				number++;
		    tnumber = number_range(1,number);
		    number = 1;

                    for (vch = ch->in_room->people; vch; vch = vch->next)
                        if (can_see(rch,vch)
                              &&  is_same_group(vch,victim)
          	              &&  number++ == tnumber)
	              	              target = vch;

                    if (target != NULL)
                    {
			sprintf(buf,rch->short_descr);
			pbuf = &buf[0];
			if(!str_prefix("a ",rch->short_descr))
				pbuf += 2;
			else if(!str_prefix("the ",rch->short_descr))
				pbuf += 4;
			sprintf(buf2,"%s%s",pbuf != &buf[0] ? "Another " : "",pbuf);
			act("$t comes to the aid of $n, attacking $N!",ch,buf2,target,TO_ROOM);
                        multi_hit(rch,target,TYPE_UNDEFINED);
                    }
                }
            }
        }
    }
}


/*
 * Do one group of attacks.
 */
void multi_hit( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
    float     chance;
    float	    dual_chance;
    int	i, attacks;
    char buf[500];
    bool noprimary = FALSE;

    /* decrement the wait */
    if (ch->desc == NULL)
        ch->wait = UMAX(0,ch->wait - PULSE_VIOLENCE);

	if (ch->level == MAX_LEVEL) 
	{
		ch->wait = 0;
	}

    if(ch->in_room!=victim->in_room)
	{
		ch->fighting=NULL;
		victim->fighting=NULL;
		ch->position=POS_STANDING;
		victim->position=POS_STANDING;
		update_pos(ch);
		update_pos(victim);
		return;
	}

    if(!IS_NPC(victim) && victim->pcdata->entering_text && IS_SET(victim->act, PLR_AUTOABORT)) {
    	send_to_char("Saving changes and exiting due to combat.\n\r",victim);
     	if(victim->pcdata->end_fun)
     		(*victim->pcdata->end_fun) (victim, victim->pcdata->entered_text);
           	victim->pcdata->entering_text = FALSE;
        	victim->pcdata->end_fun = NULL;
	}
	
    check_blade_barrier(ch,victim);

	/* no attacks for stunnies -- just a check */
    if (ch->position < POS_RESTING)
        return;

    if (IS_NPC(ch)) {
        mob_hit(ch,victim,dt);
        return;
    }
	
    if (IS_NPC(victim) && IS_CABAL_GUARD(victim) && !IS_NPC(ch) &&
		ch->cabal==victim->cabal) {
		sprintf(buf,"How DARE you attack me, %s!",ch->name);
		do_cb(victim,buf);
	}
	
/*    if(IS_SHIFTED(ch)) {
	attacks = number_range(1,form_table[ch->pcdata->shifted].attacks);
	if(IS_AFFECTED(ch,AFF_HASTE) && number_percent()>30)
		attacks++;
	if(IS_AFFECTED(ch,AFF_SLOW))
		attacks--;
	attacks	= URANGE(1,attacks,form_table[ch->pcdata->shifted].attacks);
	for(i=0; i<attacks; i++)
		one_hit(ch,victim,dt);

	return;
    }
*/
	if (check_entwine(ch,0)) {
		noprimary = TRUE;
	}

    if (!noprimary)
    one_hit( ch, victim, dt );

    if ( ch->fighting != victim || dt == gsn_backstab || (dt ==  gsn_ambush) )
        return;

    if (IS_AFFECTED(ch,AFF_HASTE) && (IS_NPC(ch) ||
		get_skill(ch,gsn_third_attack)>5)) {
		if (!noprimary)	one_hit(ch,victim,dt);
	}

    chance = get_skill(ch,gsn_second_attack)/2;
    dual_chance = get_skill(ch,gsn_dual_wield)/2;

	if (IS_AFFECTED(ch,AFF_HASTE)) {
		chance*=1.25;
		dual_chance*=1.25;
    }

	if (IS_AFFECTED(ch,AFF_SLOW)) {
   		chance /= 2;
		dual_chance /= 2;
	}

	if (check_entwine(victim,1)) {
		dual_chance /= 4;
	}

	if(!IS_NPC(ch)) {
		if (check_posture(ch) == POSTURE_DEFENSE && style_check(gsn_posture, ch->pcdata->style)) {
			chance /= 2.5;
			dual_chance /= 2.5;
		}
	}

	
    if ( number_percent( ) < chance) {
		if (!noprimary) {
	        one_hit( ch, victim, dt );
    	    check_improve(ch,gsn_second_attack,TRUE,5);
       		if ( ch->fighting != victim ) return;
		}
    }


    if (get_eq_char(ch,WEAR_DUAL_WIELD) != NULL
	&& ch->pcdata->learned[gsn_second_attack] > 10) {
		if (number_percent( ) < dual_chance) {
			one_hit(ch,victim,gsn_dual_wield);
			check_improve(ch,gsn_dual_wield,TRUE,3);
			if (ch->fighting != victim) return;
		} else {
			check_improve(ch,gsn_dual_wield,FALSE,3);
		}
	}

    if (ch->pcdata->learned[gsn_second_attack] > 10) {
		chance = get_skill(ch,gsn_third_attack)/4;
		if (IS_AFFECTED(ch,AFF_SLOW))	chance*=0.5;
    	if (IS_AFFECTED(ch,AFF_HASTE))	chance*=1.3;

		if ( number_percent( ) < chance ) {
			if (!noprimary)	one_hit( ch, victim, dt );
       		check_improve(ch,gsn_third_attack,TRUE,6);
        	if ( ch->fighting != victim)	return;
		}
    	
		dual_chance = get_skill(ch,gsn_dual_wield)/4;

	    if (IS_AFFECTED(ch,AFF_HASTE))	dual_chance*=1.2;
		
	    if (get_eq_char(ch,WEAR_DUAL_WIELD) != NULL
		&& ch->pcdata->learned[gsn_third_attack] > 10) {
			if (number_percent( ) < dual_chance) {
				one_hit(ch,victim,gsn_dual_wield);
				check_improve(ch,gsn_dual_wield,TRUE,3);
				if (ch->fighting != victim)	return;
			} else {
				check_improve(ch,gsn_dual_wield,FALSE,3);
			}
		}
    }	

	if (ch->pcdata->learned[gsn_third_attack] > 10) {
	    chance = get_skill(ch,gsn_fourth_attack)/5;
		if (IS_AFFECTED(ch,AFF_SLOW))	chance *= 0.5;
   		if (IS_AFFECTED(ch,AFF_HASTE))	chance *= 1.3;
		if ( number_percent() < chance ) {
			if (!noprimary)	one_hit( ch, victim, dt );
			check_improve(ch,gsn_fourth_attack,TRUE,6);
			if (ch->fighting != victim ) return;
		}

		dual_chance = get_skill(ch,gsn_dual_wield)/4;
    	if (IS_AFFECTED(ch,AFF_HASTE))	dual_chance*=1.2;

		if (get_eq_char(ch,WEAR_DUAL_WIELD) != NULL &&
			ch->pcdata->learned[gsn_fourth_attack] > 10) {
			if (number_percent( ) < dual_chance) {
				one_hit(ch,victim,gsn_dual_wield);
				check_improve(ch,gsn_dual_wield,TRUE,6);
				if (ch->fighting != victim) return;
			} else {
				check_improve(ch,gsn_dual_wield,FALSE,5);
			}	
		}
	}
		
	if (!IS_NPC(ch)) {
		if (style_check(gsn_fifth_attack,ch->pcdata->style)) {
		    chance = get_skill(ch,gsn_fifth_attack)/4 + (get_curr_stat(ch,STAT_DEX) - 18) * 3;

   			if (IS_AFFECTED(ch,AFF_SLOW)) chance /= 2;
			if (is_affected(ch,skill_lookup("haste"))) chance*=1.3;
			if ( number_percent() < chance ) {
				if (!noprimary)	 one_hit( ch, victim, dt );
				check_improve(ch,gsn_fifth_attack,TRUE,6);
				if (ch->fighting != victim )	return;
		 	}
			
	    	dual_chance = get_skill(ch,gsn_dual_wield)/4;
			if (is_affected(ch,skill_lookup("haste"))) 	dual_chance*=1.2;
			if (get_eq_char(ch,WEAR_DUAL_WIELD) != NULL &&
			 ch->pcdata->learned[gsn_fifth_attack] > 10) {
				if (number_percent( ) < dual_chance) {
					one_hit(ch,victim,gsn_dual_wield);
					check_improve(ch,gsn_dual_wield,TRUE,6);
					if (ch->fighting != victim) return;
				} else {
					check_improve(ch,gsn_dual_wield,FALSE,6);
				}
			}
		}
	}
	
/* posture */

	if (check_posture(ch) == POSTURE_OFFENSE &&
		get_skill(ch,gsn_third_attack)>80 &&
		style_check(gsn_posture, ch->pcdata->style)) {
			chance = number_range(1,2);
			for (i=0; i<chance; i++)
				if (!noprimary)
					one_hit(ch, victim, dt);
		}
	return;
}

/* procedure for all mobile attacks */

void mob_hit (CHAR_DATA *ch, CHAR_DATA *victim, int dt)
{
    int chance,number;
    int dual_chance;
    CHAR_DATA *vch, *vch_next;

    one_hit(ch,victim,dt);

    if (ch->fighting != victim)
        return;

    /* Area attack -- BALLS nasty! */
    if (IS_SET(ch->off_flags,OFF_AREA_ATTACK))
    {
        for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
        {
            vch_next = vch->next;
            if ((vch != victim && vch->fighting == ch))
                one_hit(ch,vch,dt);
        }
    }

    if (IS_AFFECTED(ch,AFF_HASTE) || (IS_SET(ch->off_flags,OFF_FAST) &&
		!IS_AFFECTED(ch,AFF_SLOW)))
        one_hit(ch,victim,dt);

    if (ch->fighting != victim || dt == gsn_backstab)
        return;

    chance = UMAX(70,get_skill(ch,gsn_second_attack));
    dual_chance = ch->level*2/3;

    if (IS_AFFECTED(ch,AFF_SLOW) && !IS_SET(ch->off_flags,OFF_FAST))
    {
	dual_chance /= 2;
        chance /= 2;
    }

    if (number_percent() < chance)
    {
        one_hit(ch,victim,dt);
        if (ch->fighting != victim)
            return;
    }

    if (IS_AFFECTED(ch,AFF_HASTE))
	dual_chance *= 6/4;

    if (number_percent() < dual_chance && get_eq_char(ch,WEAR_DUAL_WIELD) != NULL)
    {
	one_hit(ch,victim,gsn_dual_wield);
	if (ch->fighting != victim)
		return;
    }

    chance = get_skill(ch,gsn_third_attack)/4;
     dual_chance *= 2/3;

    if (IS_AFFECTED(ch,AFF_SLOW)) {
		dual_chance = 0;
        chance = 0;
    }

    if (IS_AFFECTED(ch,AFF_HASTE))
		dual_chance *= 3/2;

    if (number_percent() < chance)
    {
        one_hit(ch,victim,dt);
        if (ch->fighting != victim)
            return;
    }

    if (number_percent() < dual_chance && get_eq_char(ch,WEAR_DUAL_WIELD) != NULL)
    {
	one_hit(ch,victim,gsn_dual_wield);
	if (ch->fighting != victim)
		return;
    }

    /* oh boy!  Fun stuff! */

    if (ch->wait > 0)
        return;

	if (IS_SET(ch->act,ACT_INTELLIGENT))
		switch (ch->pIndexData->Class()->GetIndex()) {
			case (CLASS_NONE):
				break;
			case (CLASS_WARRIOR):
				warrior_ai(ch, ch->fighting);
				return;
			case (CLASS_THIEF):
				thief_ai(ch, ch->fighting);
				return;
			default:
				break;
		}

	number = number_range(0,9);

    switch(number)
    {
    case (0) :
        if (IS_SET(ch->off_flags,OFF_BASH))
            do_bash(ch,"");
	else if (IS_SET(ch->off_flags,OFF_TAIL))
	    do_tail(ch,"");
        break;

    case (1) :
        if (IS_SET(ch->off_flags,OFF_BERSERK) && !IS_AFFECTED(ch,AFF_BERSERK))
            do_berserk(ch,"");
        break;

    case (2) :
        if (IS_SET(ch->off_flags,OFF_DISARM))
            do_disarm(ch,"");
        break;

    case (3) :
        if (IS_SET(ch->off_flags,OFF_KICK))
            do_kick(ch,"");
        break;

    case (4) :
        if (IS_SET(ch->off_flags,OFF_KICK_DIRT))
            do_dirt(ch,"");
        break;

    case (5) :
        if (IS_SET(ch->off_flags,OFF_TAIL))
	    do_tail(ch,"");
        break;

    case (6) :
        if (IS_SET(ch->off_flags,OFF_TRIP))
            do_trip(ch,"");
        break;

    case (7) :
        break;
    case (8) :
	break;
    case (9) :
	break;
    }
}


/*
 * Hit one guy once.
 */
int one_hit(CHAR_DATA *ch, CHAR_DATA *victim, int dt)
{
	return one_hit_new(ch,victim,dt,HIT_SPECIALS,HIT_BLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);
}

/* De-Ceranized by Cal on 10/24/01 
* return of -1 = Death			*/
int one_hit_new( CHAR_DATA *ch, CHAR_DATA *victim, int dt, bool specials, bool blockable, int addition, int multiplier, char *dnoun)
{
	OBJ_DATA *wield = NULL, *dualw = NULL;
	AFFECT_DATA *af;
	int mdam, diceroll, diceroll2 = 0, sn, skill, dam_type, tmp_dt, result, rdt;
	float dam;
	bool truestrike = FALSE;

	if (victim == ch || ch == NULL || victim == NULL
	   || victim->position == POS_DEAD
	   || (ch->in_room != victim->in_room && dt != gsn_hurl)
	   || is_safe(ch,victim))
		return 0;
	
	if(dt == TYPE_TRUESTRIKE)
	{
		truestrike = TRUE;
		dt = TYPE_UNDEFINED;
	}

	wield = get_eq_char( ch, WEAR_WIELD );
	if(dt == gsn_dual_wield)
		wield = get_eq_char( ch, WEAR_DUAL_WIELD );

	if (dt == TYPE_UNDEFINED || dt == gsn_dual_wield)
	{
		dt = TYPE_HIT;
		if ( wield != NULL && wield->item_type == ITEM_WEAPON )
			dt += wield->value[3];
		else
			dt += ch->dam_type;
	}

	if (wield != NULL)
		dam_type = attack_table[wield->value[3]].damage;
	else
		dam_type = attack_table[ch->dam_type].damage;

	if (dam_type == -1)
		dam_type = DAM_BASH;

	sn = get_weapon_sn_new(ch, dt);
	skill = 20 + get_weapon_skill(ch,sn);

	if(truestrike)
		dam_type = DAM_TRUESTRIKE;

	if (IS_NPC(ch) && wield == NULL )
	{
            dam = dice(ch->damage[DICE_NUMBER],ch->damage[DICE_TYPE]);
	}
	else
	{
		if (sn != -1)
			check_improve(ch,sn,TRUE,5);
		if ( wield != NULL )
		{
			dam = dice(wield->value[1],wield->value[2]) * skill/100;
			if (wield->level - ch->level >= 35)
				dam = (dam*6)/10;
			else if (wield->level - ch->level >= 25)
				dam = (dam*7)/10;
			else if (wield->level - ch->level >= 15)
				dam = (dam*8)/10;
			else if (wield->level - ch->level >= 5)
				dam = (dam*9)/10;

			if (IS_WEAPON_STAT(wield,WEAPON_SHARP))
				if (number_percent() <= (skill / 8))
					dam *= 1.2;
		}
		else
			dam = number_range( 1 + 4 * skill/100, 2 * ch->level/3 * skill/100);
	}

	if ( get_skill(ch,gsn_enhanced_damage) > 0 )
	{
		if (number_percent() <= get_skill(ch,gsn_enhanced_damage))
		{
			check_improve(ch,gsn_enhanced_damage,TRUE,6);
			dam *= 3 + (number_percent()/200);
			dam /= 2;
		}

		if (ch->Class()->name == "thief")
			dam *= 0.75;

		if (get_skill(ch,gsn_brutality) > 0
		   && (!IS_NPC(ch)) && number_percent() < get_skill(ch,gsn_brutality)
		   && style_check(gsn_brutality, ch->pcdata->style)
		   && ((wield != NULL) && (wield->weight >= 15)))
		{
			check_improve(ch,gsn_brutality,TRUE,5);
			dam *= 5;
			dam /= 4;
		}
	}
	else
		dam *= (float).7;

	if (( wield == NULL ) && is_affected(ch, gsn_hardenfist)
	    && (af = affect_find(ch->affected, gsn_hardenfist)))
			dam = (dice((af->level/4), 4) + 10) * skill/100;

	mdam = GET_DAMROLL(ch);
	dam += mdam * UMIN(100,skill) /100;

	if(!IS_NPC(ch))
		dam *= pc_race_table[ch->race].racialDam;
	
	if(!IS_NPC(ch) && is_affected(ch,gsn_rage)
	   && ch->pcdata->tribe == TRIBE_BOAR && number_percent()<10)
	{
		send_to_char("The fury of the Boar enrages you as you deliver a mighty blow!\n\r",ch);
		act("$n gets an enraged look in $s eyes and delivers a mighty blow!",ch,0,0,TO_ROOM);
		dam *= ch->level/20;
	}

	if ( dam <= 0 )
		dam = 1;

	if(!IS_NPC(ch) && wield == NULL && (dt >= TYPE_HIT || dt == gsn_dual_wield))
		dt = TYPE_HIT+17; //hth = #17

	if(wield && ch->Class()->GetIndex() == CLASS_PALADIN && ch->fighting 
			&& dt >= TYPE_HIT 
			&& (rdt = check_arms(ch,wield,FALSE)) > 0)	//check_arms returns 0 if normal
		dt = rdt + TYPE_HIT;
	if(wield && TRAPS_IEVENT(wield,TRAP_IONEHIT))
		if(CALL_IEVENT(wield,TRAP_IONEHIT, ch, victim, wield, &dam, &dt, &dam_type) > 0)
			return 0;

	//mob onehit [mhit] fires when mob is hit, but item onehit fires when item does the hitting

	if(IS_NPC(victim) && TRAPS_MEVENT(victim,TRAP_MONEHIT))
		if(CALL_IEVENT(victim, TRAP_MONEHIT, ch, victim, wield, &dam, &dt, &dam_type) > 0)
			return 0;

	result = damage_new( ch, victim, (int)dam, dt, dam_type, TRUE, blockable, addition, multiplier,dnoun);
	if(result == -1) //death
		return -1;

	if (wield && IS_SET(wield->progtypes, IPROG_HIT))
		(wield->pIndexData->iprogs->hit_prog) (wield, ch, victim, result);
	if (result && wield != NULL)
	{
		if (ch->fighting == victim && IS_WEAPON_STAT(wield,WEAPON_POISON))
		{
            	int level = 10;
            	OBJ_AFFECT_DATA *poison;
			AFFECT_DATA af;

            	if ((poison = affect_find_obj(wield->affected,gsn_poison)) == NULL)
                		level = wield->level;
            	else
                		level = poison->level;

            	if (!saves_spell(level / 2,victim,DAM_POISON)
				&& !IS_AFFECTED(victim,AFF_POISON))
			{
				send_to_char("You feel poison coursing through your veins.\n\r",victim);
                		act("$n is poisoned by the venom on $p.",victim,wield,NULL,TO_ROOM);
                		init_affect(&af);
                		af.where     = TO_AFFECTS;
				af.aftype     = AFT_SPELL;
                		af.type      = gsn_poison;
                		af.level     = level * 3/4;
                		af.duration  = level / 10;
                		af.location  = APPLY_STR;
                		af.modifier  = -3;
                		SET_BIT(af.bitvector, AFF_POISON);
                		affect_join( victim, &af );
          	}

			if (poison != NULL)
			{
				poison->level = UMAX(0,poison->level - 2);
				poison->duration = UMAX(0,poison->duration - 1);
				if (poison->level == 0 || poison->duration == 0)
					act("The poison on $p has worn off.",ch,wield,NULL,TO_CHAR);
			}
		}
        }
    return result;
}

/*
 * Inflict damage from a hit.
 */
int damage_new(CHAR_DATA *ch, CHAR_DATA *victim, int idam, int dt, int dam_type, bool show, bool blockable, int addition, int multiplier, char *dnoun)
{
	char buf[MSL];
    OBJ_DATA *corpse, *wield;
    bool immune;
	AFFECT_DATA *af,*laf;
    int kineticdam;
	int originaldam;
    CHAR_DATA *owner = NULL;
	CHAR_DATA *gch;
    bool nosafe = FALSE, reduced = FALSE;
    AFFECT_DATA *daf;
	float dam = (float)idam;
	
    if ( victim->position == POS_DEAD )
        return 0;

	/* Cheaphack to have poison et al count for PKs */
	if ((dt == gsn_poison || dt == gsn_plague || dt == gsn_impale
		|| dt == gsn_agitate || dt == gsn_bleeding || dt == gsn_enervate
		|| dt == gsn_retribution
		|| dt == gsn_burning_hands || dt == gsn_attract
		|| dt == skill_lookup("burning touch")
		|| dt == gsn_mana_sickness) && (ch != victim)) {
		owner = ch;
		ch = victim;
		nosafe = TRUE;
	}

    if (!dnoun && !nosafe && is_safe(ch,victim))
		return 0;

    originaldam=(int)dam;
    /*
     * Stop up any residual loopholes.
     */

    if ( dam > 3000 && dt >= TYPE_HIT)
        dam = 3000;
	
	dam += addition;

	if (multiplier != HIT_NOMULT)
		dam *= multiplier/100.1;

	/*SCALED DAMAGE BY LEVEL*/

	if (!IS_NPC(ch)) {
		dam *= (ch->level + 49);
		dam /= 100;
	}

    if ( victim != ch && ch->in_room == victim->in_room)
    {
        /*
         * Certain attacks are forbidden.
         * Most other attacks are returned.
         */

        if ( victim->position > POS_STUNNED &&
			 victim != ch && !((is_same_group(ch,victim) ||
			 is_safe_new(ch,victim,FALSE)) && dnoun &&
			 dnoun[strlen(dnoun)-1] == '*')) {
            if ( victim->fighting == NULL )
                set_fighting( victim, ch );
            if (victim->timer <= 4)
                victim->position = POS_FIGHTING;
        }

        if ( victim->position > POS_STUNNED && victim != ch) {
            if ( ch->fighting == NULL &&
				 !((is_same_group(ch,victim) ||
				 is_safe_new(ch,victim,FALSE)) &&
				 dnoun && dnoun[strlen(dnoun)-1] == '*'))
                set_fighting( ch, victim );
        }

        /*
         * More charm stuff.
         */
        if ( victim->master == ch && !(dnoun && dnoun[strlen(dnoun)-1] == '*'))
            stop_follower( victim );
    }

    if ( (dt >= TYPE_HIT || dt == gsn_dual_wield) &&
		 ch != victim && blockable==HIT_BLOCKABLE &&
		 ch->in_room == victim->in_room && dam>0)  {

		if (check_maneuvering(ch,victim,dt)) 	
			return 0;
		if (check_fend(ch,victim,dt))			
			return 0;
		if (check_parry(ch,victim,dt))			
			return 0;
	    if (check_shield_block(ch,victim,dt))	
			return 0;
		for (gch = ch->in_room->people; gch; gch = gch->next_in_room)
			if (check_intercept(ch,victim,gch,dt))		
				return 0;
		if (check_deflect(ch,victim,dt))		
			return 0;
		if (check_dodge(ch,victim,dt))			
			return 0;
		if (check_avoid(ch,victim,dt))          
			return 0;
		if (check_piety(ch,victim,dt))
			return 0;
		if (is_affected(victim, gsn_cloak_of_mist))
			if (check_mist(ch,victim,dt))
				return 0;
		
		if ((dam=check_armor(ch, victim, dt, dam_type, (int)dam)) == 0)
		    return 0;
		
		int sanguine = check_sanguine(victim);
		if (dam && sanguine
		      &&  ((sanguine >= 3 && sanguine < 5 && number_percent() > 85)
		      || (sanguine == 5 && number_percent() > 80)))
		{
 		     char *attackmsg = get_dam_message(ch, dt);
 		     act("Your ward becomes opaque briefly as it slows $N's $t.", victim, attackmsg, ch, TO_CHAR);
 		     act("$n's ward becomes opaque briefly as it slows $N's $t.",victim,attackmsg,ch, TO_NOTVICT);
		     act("$n's ward becomes opaque briefly as it slows your $t.",victim,attackmsg,ch, TO_VICT);
		     dam /= 2;
		}
		if(dam && sanguine == 5 && number_percent()>96 && !is_affected(victim, gsn_blindness))
			sanguine_blind(victim, ch);
		ch->batter = 0;
   }

	/* empathy */
	if(is_affected(victim, gsn_empathy))
	{
		for(laf = victim->affected; laf != NULL; laf = laf->next)
		{
			if(laf->type == gsn_empathy)
				break;
		}
		if(ch != victim)
			damage_new(ch,laf->owner,(int)((float)dam*.25),gsn_empathy,DAM_INTERNAL,
					TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"spiritual link"); 
		dam *= .75;
	}
	
	/* damage reduction */
	
	if ((is_affected(victim,gsn_absorb)) && (dam_type == DAM_LIGHTNING)) {
		victim->mana += (short)dam / 10;
	}
	
    if (is_affected(victim,gsn_diamondskin) && (daf = affect_find(victim->affected, gsn_diamondskin))) 
    {
	int diamondskin;
	diamondskin = daf->modifier;
	switch (dam_type) {
	    case DAM_LIGHT:
	    case DAM_DISEASE:
	    case DAM_DROWNING:
	    case DAM_POISON:
	    case DAM_MENTAL:
		case DAM_INTERNAL:
	    case DAM_TRUESTRIKE:
			break;
	    default:
		if (diamondskin > 500 && diamondskin - dam < 500)
		    send_to_char("Your diamond skin begins to fracture - it may not hold much longer.\n\r",victim);

		daf->modifier -= (short)dam;
		diamondskin -= (short)dam;
		if (diamondskin <= 0) {
		    diamondskin = 0;
		    act("$n howls in agony as the diamond shell that was once $s skin shatters!",victim,0,0,TO_ROOM);
		    act("Unspeakable agony wracks your entire frame as your very skin shatters!",victim,0,0,TO_CHAR);
		    damage_new(victim,victim,victim->max_hit/2,gsn_diamondskin,DAM_TRUESTRIKE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);
		    affect_strip(victim, gsn_diamondskin);
			if (victim->ghost > 0)
				return 0;
		    break;
		}
		if (dam_type != DAM_ACID)
		    dam *= .05;
		else
		    dam = 0;
		break;
	}
    }
    
    if (is_affected(victim,gsn_seraphic_mantle)
		&& dam_type != DAM_TRUESTRIKE
		&& dam_type != DAM_SLASH
		&& dam_type != DAM_BASH
		&& dam_type != DAM_PIERCE
		&& dam_type != DAM_INTERNAL)
	    	dam *= .75;
    if (is_affected(victim,gsn_shield_of_faith)
		&& (dam_type == DAM_SLASH
		|| dam_type == DAM_BASH
		|| dam_type == DAM_PIERCE))
		dam *= .75;		
    if (is_affected(victim,gsn_holy_shroud)
	    && dam_type != DAM_TRUESTRIKE)
   	{
		af = affect_find(victim->affected,gsn_holy_shroud);
		if(af->owner == victim && IS_EVIL(ch))
			dam *=.5;
		if(af->owner == victim && !IS_EVIL(ch))
			dam *=.8;
		if(af->owner != victim && IS_EVIL(ch))
			dam *=.75;
		if(af->owner != victim && !IS_EVIL(ch))
			dam *= .9;
   	} 
    if (is_affected(victim,gsn_sanguine_ward)
		&& dam_type != DAM_TRUESTRIKE)
		dam *= .8;
    if (is_affected(victim,gsn_stoneskin)
		&& dam_type != DAM_TRUESTRIKE
		&& dam_type != DAM_MENTAL
		&& dam_type != DAM_INTERNAL)
		dam *= .8;
	if (ch != NULL
		&& (af = affect_find(ch->affected,gsn_divine_ward)) != NULL
		&& af->owner == victim
		&& dam_type != DAM_TRUESTRIKE) {
		if (af->modifier == 1)
			dam *= 0.55;
		else if (af->modifier == 2)
			dam *= 0.45;
	}
	if (!IS_NPC(victim) && victim->pcdata->greaterdata[GREATER_CIMERIES] >=2)
		dam *= .85;
    if (is_affected(victim,gsn_visceral)
		&& dam_type != DAM_TRUESTRIKE)
		dam *= .875;
    if (is_affected(victim,gsn_soften)
		&& dam_type != DAM_INTERNAL)
		dam *= 1.3;
	if (is_affected(victim,gsn_ironskin)
		&& dam_type != DAM_TRUESTRIKE
		&& dam_type != DAM_MENTAL
		&& dam_type != DAM_INTERNAL)
		dam *= .7;

    if (!IS_NPC(victim)
		&& IS_SET(victim->act,PLR_BETRAYER)
		&& dam_type==DAM_HOLY)
		dam *= 1.5;

    if (dam_type!=DAM_TRUESTRIKE) {
		dam *= victim->dam_mod;
		dam /= 100;
	}

    un_camouflage( ch, NULL);
	un_camouflage(victim,NULL);
    un_hide(ch, NULL);
	un_hide(victim,NULL);
    un_invis(ch, NULL);
	un_invis(victim, NULL);
    un_sneak(ch, NULL);
	un_sneak(victim, NULL);
	un_earthfade(ch, NULL);
	un_earthfade(victim,NULL);
	un_ultradiffusion(ch,NULL);
	un_ultradiffusion(victim, NULL);
	un_watermeld(ch,NULL);
	un_watermeld(victim, NULL);
	un_shroud(ch,NULL);
	un_shroud(victim,NULL);
	
    /*
     * Damage modifiers.
     */
    if(dam > 1 && !IS_NPC(victim) && victim->pcdata->condition[COND_DRUNK] > 10)
		dam *= 0.9;
		
    if (dam > 1 && 
		(IS_AFFECTED(victim, AFF_SANCTUARY))
		&& dam_type!=DAM_TRUESTRIKE)
		dam /= 2;

    if ( dam > 1 && IS_AFFECTED(victim, AFF_PROTECTION) && IS_OPP_ALIGN(ch,victim) && dam_type!=DAM_TRUESTRIKE)
        dam -= dam / 4;

    immune = FALSE;

	/*
     * Check for parry, and dodge.
     */

    switch(check_immune(victim,dam_type))
    {
        case(IS_IMMUNE):
            immune = TRUE;
            dam = 0;
            break;
        case(IS_RESISTANT):
	    if (IS_NPC(victim))
        	dam -= dam/3;
	    else
			dam -= dam/3;
            break;
        case(IS_VULNERABLE):
            dam += dam/3;
            break;
    }

    if (dt == gsn_dual_wield)
		wield = get_eq_char(ch,WEAR_DUAL_WIELD);
    else
		wield = get_eq_char(ch,WEAR_WIELD);

    if(wield && IS_SET(victim->vuln_flags, VULN_IRON) && !str_cmp(wield->material,"iron"))
		dam += dam / 3;
    if(wield && IS_SET(victim->vuln_flags, VULN_MITHRIL) && !str_cmp(wield->material,"mithril"))
        dam += dam / 3;

	if (originaldam > 0 && dam == 0 && !immune)
		reduced = TRUE;

    if (show)
        dam_message( ch, victim, (int)dam, dt, immune, dnoun, reduced);
	
	if ( (dt >= TYPE_HIT || dt == gsn_dual_wield) && ch != victim) {
    	if (is_affected(victim,gsn_corona) && dam>0 && dt!=DAM_COLD) {
			kineticdam = dice(victim->level / 12, 6);
			damage_old(victim, ch, kineticdam, gsn_corona, DAM_FIRE, TRUE);
		}

		if (is_affected(victim,gsn_frigidaura) && dam>0 && dt!=DAM_FIRE) {
			kineticdam = dice(victim->level / 12, 6);
			damage_old(victim, ch, kineticdam, gsn_frigidaura,DAM_COLD,TRUE);
		}
    }

    if (dam == 0)
        return 0;
	
	/* retribution */
	AFFECT_DATA *paf = affect_find(ch->affected, gsn_retribution);
	if(paf && paf->owner == victim)
		paf->modifier = UMIN((int)dam + paf->modifier, paf->owner->level * 8);

    /*
     * Hurt the victim.
     * Inform the victim of his new state.
     */
    victim->hit -= (short)dam;

    if ( !IS_NPC(victim)
    &&   victim->level >= LEVEL_IMMORTAL
    &&   victim->hit < 1 )
        victim->hit = 1;
    if(IS_NPC(victim) && victim->pIndexData->vnum == 23630)
	victim->hit = 1;

    update_pos( victim );

    switch( victim->position )
    {
    case POS_MORTAL:
        act( "$n is mortally wounded, and will die soon, if not aided.",
            victim, NULL, NULL, TO_ROOM );
        send_to_char(
            "You are mortally wounded, and will die soon, if not aided.\n\r",
            victim );
        break;

    case POS_INCAP:
        act( "$n is incapacitated and will slowly die, if not aided.",
            victim, NULL, NULL, TO_ROOM );
        send_to_char(
            "You are incapacitated and will slowly die, if not aided.\n\r",
            victim );
        break;

    case POS_STUNNED:
        act( "$n is stunned, but will probably recover.",
            victim, NULL, NULL, TO_ROOM );
        send_to_char("You are stunned, but will probably recover.\n\r",
            victim );
        break;

    case POS_DEAD:
        act( "$n is DEAD!!", victim, 0, 0, TO_ROOM );
        send_to_char( "You have been KILLED!!\n\r\n\r", victim );
        break;

    default:
        if ( dam > victim->max_hit / 4 ) {
			sprintf(buf, "%sThat really did HURT!%s\n\r",
				get_char_color(victim,"white"),END_COLOR(victim));
            send_to_char( buf, victim );
		}
        if ( victim->hit < victim->max_hit / 4 ) {
			sprintf(buf, "%sYou sure are BLEEDING!%s\n\r",
				get_char_color(victim,"red"),END_COLOR(victim));
			send_to_char(buf,victim);
		}
        break;
    }

    /*
     * Sleep spells and extremely wounded folks.
     */
    if ( !IS_AWAKE(victim) && ch != victim)
        stop_fighting( victim, FALSE );

    /*
     * Payoff for killing things.
     */
    if ( victim->position == POS_DEAD )
    {
        group_gain( ch, victim );

        if ( !IS_NPC(victim) )
        {
            sprintf( log_buf, "%s killed by %s at %d",
                victim->name,
                (IS_NPC(ch) ? ch->short_descr : ch->name),
                ch->in_room->vnum );
            log_string( log_buf );
	  }

            /*
             * Dying penalty:
             */

	if (owner)
        raw_kill(owner, victim);
	else
        raw_kill(ch, victim);

        /*
         * Death trigger
         */
      if (!IS_NPC(ch)
        &&  (corpse = get_obj_list(ch,"corpse",ch->in_room->contents)) != NULL
        &&  corpse->item_type == ITEM_CORPSE_NPC && can_see_obj(ch,corpse))
        {
            OBJ_DATA *coins;

            corpse = get_obj_list( ch, "corpse", ch->in_room->contents );
            if ( IS_SET(ch->act, PLR_AUTOLOOT) &&
                 corpse && corpse->contains) /* exists and not empty */
                do_get( ch, "all corpse" );

            if (IS_SET(ch->act,PLR_AUTOGOLD) &&
                corpse && corpse->contains  && /* exists and not empty */
                !IS_SET(ch->act,PLR_AUTOLOOT))
                if ((coins = get_obj_list(ch,"gcash",corpse->contains)) != NULL)
                    do_get(ch, "all.gcash corpse");

            if ( IS_SET(ch->act, PLR_AUTOSAC) )
	    {
              if ( IS_SET(ch->act,PLR_AUTOLOOT) && corpse && corpse->contains)
                return (int)dam;  /* leave if corpse has treasure */
              else
                do_sacrifice( ch, "corpse" );
	    }
        }
    
	return -1; //death
    }

    if ( victim == ch )
        return (int)dam;

    /*
     * Wimp out?
     */
    if(ch->in_room != victim->in_room || dt == gsn_parting_blow)
		return (int)dam;
	
    if ( IS_NPC(victim) && dam > 0 && victim->wait < 1 /* same thing on pcs...is this right? PULSE_VIOLENCE / 2 */)
    {
        if ( (( IS_SET(victim->act, ACT_WIMPY) && number_bits( 2 ) == 0
        &&   victim->hit < victim->max_hit / 5))
        ||   (( IS_AFFECTED(victim, AFF_CHARM) && victim->master != NULL
        &&     victim->master->in_room != victim->in_room)
        && dt != gsn_parting_blow))
            do_flee( victim, "" );
    }
    if ( !IS_NPC(victim)
    &&   victim->hit > 0
    &&   victim->hit <= victim->wimpy
    &&   victim->wait < 1 
    && dt != gsn_parting_blow)
        do_flee( victim, "" );

    tail_chain( );

    return (int)dam;
}

int damage(CHAR_DATA *ch,CHAR_DATA *victim, int dam,int dt,int dam_type,
            bool show)
{
	return damage_new(ch, victim, dam, dt, dam_type, show, HIT_BLOCKABLE, HIT_NOADD, HIT_NOMULT, NULL);
}



/*
 * Inflict damage from a hit.
 */
int damage_old( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int
dam_type, bool show )
{
	return damage_new(ch, victim, dam, dt, dam_type, show, HIT_BLOCKABLE, HIT_NOADD, HIT_NOMULT, NULL);
}

bool is_safe(CHAR_DATA *ch, CHAR_DATA *victim)
{
	return is_safe_new(ch,victim,TRUE);
}
bool is_safe_new(CHAR_DATA *ch, CHAR_DATA *victim, bool show)
{

    if(victim==NULL) {
		wiznet("is_safe(): VICTIM IS NULL!",NULL,NULL,WIZ_LOG,0,0);
		return TRUE;
	}

    if(ch==NULL) {
		wiznet("is_safe(): CH IS NULL!",NULL,NULL,WIZ_LOG,0,0);
		return TRUE;
    }

    if (victim->invis_level >= LEVEL_HERO)
		show = FALSE;
	if(IS_AFFECTED(victim,AFF_NOSHOW)) {
		show = FALSE;
		return TRUE;
		}

    if (victim->in_room == NULL)
        return TRUE;

    if (is_affected(ch, gsn_shroud_of_light) && !ch->fighting)
	{
		affect_strip(ch, gsn_shroud_of_light);
		send_to_char("As you act aggressively your shroud of the light vanishes!\n\r", ch);
	}	

    if ((IS_NPC(ch) && IS_SET(ch->act,ACT_WARD_MOB))
		|| (IS_NPC(victim) && IS_SET(victim->act,ACT_WARD_MOB)))
		return TRUE;

    if (IS_NPC(ch) && victim->invis_level >= LEVEL_HERO)
		return TRUE;

	if (IS_NPC(victim) && IS_SET(victim->act, ACT_IS_HEALER))
		return TRUE;
	
    if (!IS_NPC(ch))
		if (ch->pcdata->death_status == HAS_DIED)
			return TRUE;
    if (!IS_NPC(victim))
		if (victim->pcdata->death_status == HAS_DIED)
			return TRUE;
	
/*    if (IS_NPC(ch) && IS_NPC(victim) && !IS_AFFECTED(ch,AFF_CHARM)
		&& !IS_AFFECTED(victim,AFF_CHARM) && !ch->desc
		&& !ch->hunting && !victim->hunting && victim!=ch
		&& ch->fighting != victim
		&& victim->fighting != ch
		&& !ch->desc && !victim->desc)
		return TRUE;
*/
    if ((IS_NPC(ch) && IS_AFFECTED(ch, AFF_NOSHOW)) || (IS_NPC(victim) && IS_AFFECTED(victim, AFF_NOSHOW)))
	return TRUE;

/* Handle Link dead players , only saves from PKS, not mobs -Ceran */
    if (!IS_NPC(victim) && victim->desc == NULL && !IS_NPC(ch) && get_trust(ch) < 58
		&& victim->fighting != ch && ch->fighting != victim) {
		if (show)  {
			act("The gods protect $N from $n.",ch,0,victim,TO_NOTVICT);
			act("The gods protect $N from you.",ch,0,victim,TO_CHAR);
		}
		return TRUE;
 	}

    if (IS_IMMORTAL(ch) && !victim->ghost)
		return FALSE;

    if (is_affected(ch, gsn_calm)) {
		if(show)
			act("You're feeling too relaxed to hurt $N.",ch,0,victim,TO_CHAR);
		return TRUE;
    }

    if (victim->fighting == ch || victim == ch)
        return FALSE;

    if (IS_NPC(ch) && ch->last_fought == victim)
		return FALSE;


    if ((victim->ghost > 0)
		|| is_affected(victim,gsn_ultradiffusion)) {
		if (show) {
			act("The gods protect $N from $n.",ch,0,victim,TO_NOTVICT);
			act("The gods protect you from $n.",ch,0,victim,TO_VICT);
			act("The gods protect $N from you.",ch,0,victim,TO_CHAR);
		}
		return TRUE;
    }

    if (ch->ghost > 0) {
		if (!IS_NPC(victim)) {
			if (show) {
				act("The gods protect $N from $n.",ch,0,victim,TO_NOTVICT);
				act("The gods protect you from $n.",ch,0,victim,TO_VICT);
				act("The gods protect $N from you.",ch,0,victim,TO_CHAR);
			}
			return TRUE;
		}
		
        if (IS_AFFECTED(victim,AFF_CHARM) && victim->master != NULL
			&& !IS_NPC(victim->master)) {
  			if (show) {
				act("The gods protect $N from $n.",ch,0,victim,TO_NOTVICT);
				act("The gods protect you from $n.",ch,0,victim,TO_VICT);
				act("The gods protect $N from you.",ch,0,victim,TO_CHAR);
			}
            return TRUE;
		}
    }


    /* killing mobiles */
    if (IS_NPC(victim))
	{
        if (!IS_NPC(ch))
		{
            /* no charmed creatures unless in pk of master */
            if (IS_AFFECTED(victim,AFF_CHARM)
				&& victim->master
				&& !can_pk(ch,victim->master))
			{
  				if (show)
				{
					act("The gods protect $N from $n.",ch,0,victim,TO_NOTVICT);
					act("The gods protect you from $n.",ch,0,victim,TO_VICT);
					act("The gods protect $N from you.",ch,0,victim,TO_CHAR);
				}
                return TRUE;
            }
        }
		else 
		{
			if (IS_AFFECTED(ch,AFF_CHARM) && IS_AFFECTED(victim,AFF_CHARM)
				&& !can_pk(ch->master,victim->master))
				return TRUE;
		}
    }
	else 
	{	/* killing players */
        if (IS_NPC(ch))
		{	/* NPC doing the killing */
            /* charmed mobs and pets cannot attack players while owned */
            if (IS_AFFECTED(ch,AFF_CHARM) && ch->master != NULL
            	&&  ch->master->fighting != victim) 
			{
                send_to_char("Players are your friends!\n\r",ch);
                return TRUE;
            }
        } 
		else 
		{ /* player doing the killing */
	    	if (!can_pk(ch, victim))
			{
  					if (show) 
					{
						act("The gods protect $N from $n.",ch,0,victim,TO_NOTVICT);
						act("The gods protect you from $n.",ch,0,victim,TO_VICT);
						act("The gods protect $N from you.",ch,0,victim,TO_CHAR);
					}
					return TRUE; 
				}
    	    }
        }
    

	if(victim && IS_SET(victim->in_room->area->progtypes, APROG_AGGRESS))
		((victim->in_room->area->aprogs->aggress_prog) (victim->in_room->area, ch, victim));
	
	if(victim && IS_SET(victim->progtypes,MPROG_AGGRESS))
		if (((victim->pIndexData->mprogs->aggress_prog) (victim,ch)) == TRUE)
			return TRUE;

    return FALSE;
}

bool is_safe_spell(CHAR_DATA *ch, CHAR_DATA *victim, bool area )
{
    if (victim->in_room == NULL || ch->in_room == NULL)
        return TRUE;

    if (victim == ch && area)
        return TRUE;

	if(IS_AFFECTED(victim,AFF_NOSHOW))
		return TRUE;

    if (victim->fighting == ch || victim == ch)
        return FALSE;

    if (IS_IMMORTAL(ch) && ch->level > LEVEL_IMMORTAL && !area)
        return FALSE;

    if (!IS_NPC(victim) && victim->pcdata->newbie==TRUE && !IS_NPC(ch))
	  return TRUE;

    if (!IS_NPC(ch) && ch->pcdata->newbie==TRUE && !IS_NPC(victim))
	  return TRUE;

    /* killing mobiles */
    if (IS_NPC(victim))
    {
        /* safe room? */
        if (IS_SET(victim->in_room->room_flags,ROOM_SAFE))
            return TRUE;

        if (victim->pIndexData->pShop != NULL)
            return TRUE;

        /* no killing healers, trainers, etc */
        if (IS_SET(victim->act,ACT_TRAIN)
        ||  IS_SET(victim->act,ACT_PRACTICE)
        ||  IS_SET(victim->act,ACT_IS_HEALER))
            return TRUE;

        if (!IS_NPC(ch))
        {
            /* no pets */
            if (IS_SET(victim->act,ACT_PET))
                return TRUE;

            /* no charmed creatures unless owner */
            if (IS_AFFECTED(victim,AFF_CHARM) && (area || ch != victim->master))
                return TRUE;

            /* legal kill? -- cannot hit mob fighting non-group member */
            if (victim->fighting != NULL && !is_same_group(ch,victim->fighting))
                return TRUE;
        }
        else
        {
            /* area effect spells do not hit other mobs */
            if (area && !is_same_group(victim,ch->fighting))
                return TRUE;
        }
    }
    /* killing players */
    else
    {
        if (area && IS_IMMORTAL(victim) && victim->level > LEVEL_IMMORTAL)
            return TRUE;

        /* NPC doing the killing */
        if (IS_NPC(ch))
        {
            /* charmed mobs and pets cannot attack players while owned */
            if (IS_AFFECTED(ch,AFF_CHARM) && ch->master != NULL
            &&  ch->master->fighting != victim)
                return TRUE;

            /* safe room? */
            if (IS_SET(victim->in_room->room_flags,ROOM_SAFE))
                return TRUE;

            /* legal kill? -- mobs only hit players grouped with opponent*/
            if (ch->fighting != NULL && !is_same_group(ch->fighting,victim))
                return TRUE;
        }

        /* player doing the killing */
        else
        {
            if (!is_cabal(ch))
                return TRUE;

            if (IS_SET(victim->act,PLR_KILLER) || IS_SET(victim->act,PLR_THIEF))
                return FALSE;

            if (!is_cabal(victim))
                return TRUE;

            if (ch->level > victim->level + 8)
                return TRUE;
        }

    }
    return FALSE;
}

int check_armor(CHAR_DATA *ch, CHAR_DATA *victim, int dt, int dam_type, int dam)
{
	int armor;
	float chance;
	char *attack;
	OBJ_DATA *wield;
	
	switch(dam_type) {
		case DAM_BASH:
			armor = victim->armor[AC_BASH]; break;
		case DAM_SLASH:
			armor = victim->armor[AC_SLASH]; break;
		case DAM_PIERCE:
			armor = victim->armor[AC_PIERCE]; break;
		default:
			armor = victim->armor[AC_EXOTIC]; break;
	}

	if (armor < 0)	
		armor = 0;		/* No negative armor! */
	
	chance  = -((.00075 * pow(armor,3) - .275 * pow(armor,2) + 1) / 100);

	if(armor>250)
		chance = .0011 * armor * 100;

	chance *= 1 + ((float) dex_app[get_curr_stat(victim,STAT_DEX)].armor / 100.001);
	
	chance *= 1 + ((float) str_app[get_curr_stat(ch,STAT_STR)].topenetrate / 100.001);

	if((wield = get_eq_char(ch,WEAR_WIELD)) && wield->weight>=10)
		chance *= 1 - ((float)wield->weight/100.001);
	
	attack = get_dam_message(ch,dt);
	
	if(wield && (is_affected_obj(wield,gsn_acid_vein)))
		return dam;
	
	if((armor < 251 && number_percent() < chance * .5)
		|| (armor > 250 && number_percent() < chance)) {
		act("$N's armor deflects your $t.",ch,attack,victim,TO_CHAR);
		act("Your armor deflects $n's $t.",ch,attack,victim,TO_VICT);
		return 0;
	} else if((armor < 251 && number_percent() < chance)
		|| (armor>250 && number_percent() < chance * 2)) {
		act("$N's armor absorbs the brunt of your $t.",ch,attack,victim,TO_CHAR);
		act("Your armor absorbs the brunt of $n's $t.",ch,attack,victim,TO_VICT);
		dam /= 2;
		return dam;
	}
	return dam;
}

bool check_parry( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
    int cd_sk=0;
	float chance=0;
	OBJ_DATA *wield;
    char buf1[MAX_STRING_LENGTH], buf2[MAX_STRING_LENGTH];
    char *attack;
    OBJ_DATA *victimwield;
	int roll, diff;

    if ( !IS_AWAKE(victim) )
        return FALSE;

    chance = get_skill(victim,gsn_parry) / 2;

    if ( get_eq_char( victim, WEAR_WIELD ) == NULL ) {
        if (IS_NPC(victim)) {
            chance /= 2;
		} else if (!is_affected(victim, gsn_hardenfist))
            return FALSE;
    }

    if (IS_AFFECTED(victim, AFF_HASTE))	chance+=20;
    if (IS_AFFECTED(ch, AFF_HASTE))		chance-=15;

	if((cd_sk = get_skill(victim, gsn_champions_defense)) > 1)
	{
		if(number_percent() < (cd_sk * .85))
		{
			chance+= 10;
			check_improve(victim, gsn_champions_defense, TRUE, 1);
		}
		else
			check_improve(victim, gsn_champions_defense, FALSE, 1);
	}
	
    chance += ch->defense_mod;
	
    if (!IS_NPC(ch) && abs(ch->pcdata->energy_state)>1)
		chance-= pow(3,abs(ch->pcdata->energy_state));

	chance-=5*abs((ch->size-victim->size));

	if (dt == gsn_dual_wield)
    	wield = get_eq_char(ch,WEAR_DUAL_WIELD);
    else
	wield = get_eq_char(ch,WEAR_WIELD);

    if (wield != NULL) {
		switch (wield->value[0]) {
			case (WEAPON_SWORD): 	chance+=5;		break;
			case (WEAPON_DAGGER): 	chance-=5;		break;
			case (WEAPON_SPEAR):	chance+=0;		break;
			case (WEAPON_STAFF):	chance+=0;		break;
			case (WEAPON_MACE):		chance-=10;		break;
			case (WEAPON_AXE):		chance-=10;		break;
			case (WEAPON_FLAIL):	chance+=10;		break;
			case (WEAPON_WHIP):		chance+=10;		break;
			case (WEAPON_POLEARM):	chance-=5;		break;
			case (WEAPON_EXOTIC):					break;
			default: 				chance+=40;		break;
	    }
    }

    if((victimwield=get_eq_char(victim,WEAR_WIELD)) != NULL) {
	    switch (victimwield->value[0]) {
			case (WEAPON_SWORD): 	chance+=10;		break;
			case (WEAPON_DAGGER): 	chance-=20;		break;
			case (WEAPON_SPEAR):	chance+=15;		break;
			case (WEAPON_STAFF):	chance+=15;		break;
			case (WEAPON_MACE):		chance-=10;		break;
			case (WEAPON_AXE):		chance-=10;		break;
			case (WEAPON_FLAIL):	chance-=10;		break;
			case (WEAPON_WHIP):		chance-=20;		break;
			case (WEAPON_POLEARM):	chance+=15;		break;
			case (WEAPON_EXOTIC):					break;
			default: 				chance-=20;		break;
    	}
    }
	
	/* posture */
	if (!IS_NPC(ch)){
		if (check_posture(ch) == POSTURE_DEFENSE && style_check(gsn_posture, ch->pcdata->style))
			chance += 20;
	}

	if (!IS_NPC(victim)){
		if (check_posture(victim) == POSTURE_DEFENSE && style_check(gsn_posture, victim->pcdata->style))
			chance += 20;
		if (check_posture(victim) == POSTURE_OFFENSE && style_check(gsn_posture, victim->pcdata->style))
			chance -= 20;
	}
	if (IS_EVIL(victim) && (ch->level > victim->level) && is_affected(ch, gsn_awe))
		chance -= 10;	
	chance -= victim->balance;
	chance += ch->balance;

	chance -= ch->batter;
	
	chance -= ch->analyze;
	if (victim->analyzePC == ch) 	chance += victim->analyze;
	
	if (!IS_NPC(victim) && IS_SET(victim->act,PLR_MORON))
		chance /= 2;

	if (check_entwine(victim,1))
		chance /= 2;

    	chance += attack_table[get_attack_number(ch,dt)].modifier;
    	chance *= (1-((float)GET_HITROLL(ch) / (float) 250));

	if(!IS_NPC(victim) && victim->Class()->GetIndex() == CLASS_NECROMANCER)
		chance *= .7;
	else if(!IS_NPC(victim) && victim->Class()->GetIndex() == CLASS_SORCERER)
		chance *= .85;

    	chance = UMIN(chance, 95);
	
    if ((roll = number_percent( )) >= chance)
	return FALSE;

    diff = (int)((float)chance - (float)roll);
	
    attack = get_dam_message(ch,dt);
    sprintf(buf1,"You %sparry $n's %s.",(diff < 15) ? "manage to " :
			(diff > 50) ? "confidently " : "", attack);
    sprintf(buf2,"$N %s your %s.",(diff < 15) ? "manages to parry" :
			(diff < 50) ? "parries" : "confidently parries", attack);
    if(IS_SET(victim->wiznet,WIZ_PERCENT))
	sprintf(buf1,"You parry $n's %s. (%d%%)",attack,(int)chance);
    if(IS_SET(ch->wiznet,WIZ_PERCENT))
	sprintf(buf2,"$N parries your %s. (%d%%)",attack,(int)chance);
    act(buf1,ch,0,victim,TO_VICT);
    act(buf2,ch,0,victim,TO_CHAR);
    check_improve(victim,gsn_parry,TRUE,2);

	/* Barbarian batter check */

	 if(!IS_NPC(ch)) {
		if (style_check(gsn_batter,ch->pcdata->style)) {
			check_batter(ch);
		}
	} else if (IS_SET(ch->progtypes,MPROG_FIGHT) && !str_cmp(ch->pIndexData->mprogs->fight_name,"fight_prog_barbarian")) {
		check_batter(ch);
	}

	return TRUE;
}

/*
 * Check for shield block.
 */
bool check_shield_block( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
	int cd_sk=0;
	float chance;
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char *attack;

	if (get_eq_char(victim, WEAR_SHIELD) == NULL)
		return FALSE;
	
    if ( !IS_AWAKE(victim) )
        return FALSE;

	if ((chance = get_skill(victim, gsn_shield_block)) < 2)
		return FALSE;
	
    chance /= 4;
    chance += 15;

	if((cd_sk = get_skill(victim, gsn_champions_defense)) > 1)
	{
		if(number_percent() < (cd_sk * .85))
		{
			chance += 25;
			check_improve(victim, gsn_champions_defense, TRUE, 5);
		}
		else
			check_improve(victim, gsn_champions_defense, FALSE, 5);
	}
	
	/* posture */
	if(!IS_NPC(ch))
	{
		if (check_posture(ch) == POSTURE_DEFENSE && style_check(gsn_posture, ch->pcdata->style))
			chance += 20;
	}

	if(!IS_NPC(victim))
	{
		if (check_posture(victim) == POSTURE_DEFENSE && style_check(gsn_posture, victim->pcdata->style))
			chance += 10;
		if (check_posture(victim) == POSTURE_OFFENSE && style_check(gsn_posture, victim->pcdata->style))
			chance -= 20;
	}

	if(IS_EVIL(victim) && (ch->level > victim->level) && is_affected(ch, gsn_awe))
		chance -= 20;
    	
    if (IS_NPC(victim) || IS_NPC(ch)) 
		chance += (victim->level - ch->level);
	
	chance *= (float)((float)1 - ((float)GET_HITROLL(ch) / (float)250));
	chance -= victim->balance;
	chance += ch->balance;
	chance -= ch->batter;
	
	chance -= ch->analyze;
	if (victim->analyzePC == ch)
		chance += victim->analyze;
	
	if (check_entwine(victim,1))
		chance /= 2;

	chance = UMIN(55, chance);
	
    if ( number_percent( ) >= chance)
		return FALSE;


    attack=get_dam_message(ch,dt);

    sprintf(buf1,"You block $n's %s with your shield.",attack);
    sprintf(buf2,"$N blocks your %s with $S shield.",attack);
    if(IS_SET(victim->wiznet,WIZ_PERCENT))
        sprintf(buf1,"You block $n's %s with your shield. (%d%%)",attack,(int)chance);
    if(IS_SET(ch->wiznet,WIZ_PERCENT))
        sprintf(buf2,"$N blocks your %s with $S shield. (%d%%)",attack,(int)chance);
    act(buf1,ch,0,victim,TO_VICT);
    act(buf2,ch,0,victim,TO_CHAR);
    check_improve(victim,gsn_shield_block,TRUE,4);

	/* Barbarian batter check */

	if(!IS_NPC(ch)) {
		if (style_check(gsn_batter,ch->pcdata->style)) 
			check_batter(ch);
	} else if (IS_SET(ch->progtypes,MPROG_FIGHT) && !str_cmp(ch->pIndexData->mprogs->fight_name,"fight_prog_barbarian")) {
		check_batter(ch);
	}
	return TRUE;
}

/*
 * Check for a piety deflect
 */
bool check_piety( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
	char *attack;

	if(!is_affected(victim, gsn_piety))
		return FALSE;
	
	if(number_percent() <= 6)
	{
		attack=get_dam_message(ch,dt);
		act("In your righteous fervor, you easily deflect $n's $t.",ch,attack,victim,TO_VICT);
		act("$N easily deflects your $t.",ch,attack,victim,TO_CHAR);
		if(number_percent() <= 50)
		{
			act("While you recover from your deflected attack, $N quickly strikes you!", ch, 0, victim, TO_CHAR);
			act("While $n recovers from your deflection, you quickly strike $m!", ch, 0, victim, TO_VICT);
			act("While $n recovers from $s deflected blow, $N quickly strikes $m!", ch, 0, victim, TO_NOTVICT);
			one_hit_new(victim,ch,TYPE_UNDEFINED,HIT_SPECIALS,HIT_BLOCKABLE,HIT_NOADD,100,NULL);
		}
		return TRUE;
	}
	return FALSE;
}

/*
 * Check for dodge.
 */
bool check_dodge( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
    float chance;
    char buf1[MAX_STRING_LENGTH], buf2[MAX_STRING_LENGTH];
    char *attack;
    int dex, dexa, str;
	int encumb;
	int roll, diff;

    if ( !IS_AWAKE(victim) )
        return FALSE;

	chance = ( 3 * get_skill(victim,gsn_dodge) / 10);

    dex = get_curr_stat(victim,STAT_DEX);
    dexa= get_curr_stat(ch,STAT_DEX);
    if (dex <= 5)
	chance += 0;
    else if (dex <= 10)
	chance += dex/2;
    else if (dex <=15)
	chance += (2*dex/3);
    else if (dex <= 20)
	chance += (8*dex/10);
    else
	chance += dex;
    chance+=(dex-dexa) * 2;

	chance-=check_terrain_mastery(ch);
	chance+=check_terrain_mastery(victim);

	if (is_affected_room(ch->in_room,gsn_blanket))
		chance *= 0.8;
	
	/* posture */
	if(!IS_NPC(ch)) {
        if (check_posture(ch) == POSTURE_DEFENSE && style_check(gsn_posture, ch->pcdata->style))
			chance += 20;
	}

	if(IS_EVIL(victim) && (ch->level > victim->level) && is_affected(ch,gsn_awe))
		chance -=20;
	if(!IS_NPC(victim)) {
		if (check_posture(victim) == POSTURE_DEFENSE && style_check(gsn_posture, victim->pcdata->style))
			chance += 20;
		if (check_posture(victim) == POSTURE_OFFENSE && style_check(gsn_posture, victim->pcdata->style))
			chance -= 20;
	}

	if (!IS_NPC(victim) && IS_SET(victim->act,PLR_MORON))
		chance /= 2;

	if (check_entwine(victim,2))
		chance /= 2;

	if (is_affected(victim,gsn_creeping_tomb))
		chance *= 0.75;
	
    chance+=ch->defense_mod;
    chance *= (1-((float)GET_HITROLL(ch) / (float) 250));

	if(!IS_NPC(victim)) {
		encumb = armor_weight(victim);
		if (check_ease(victim)) {
			encumb -= (int)((float)encumb * (float) 0.005 * (float)get_skill(victim,gsn_ease));
		}

		str = get_curr_stat(victim,STAT_STR);
		chance *= (float)(300 + (5 * (str - 18)) - encumb)/(float)250;
		if (chance < 0) chance = 0;
	}
	
	if (IS_NPC(victim) || IS_NPC(ch))
		chance += (victim->level - ch->level);
	
	//chance -= 5*abs((ch->size-victim->size));

	chance -= victim->balance;
	chance += ch->balance;

	chance -= ch->analyze;
	if (victim->analyzePC == ch)	chance += victim->analyze;
	
	if (!IS_NPC(victim) && style_check(gsn_evasion,victim->pcdata->style))
			chance += check_evasion(victim, (int)chance);
	
	chance = UMIN(chance, 95);

	if ((roll = number_percent( )) >= chance)
        return FALSE;
	
	diff = (int)chance - roll;
	
    attack=get_dam_message(ch,dt);
	
    sprintf(buf1,"You %sdodge $n's %s%s.",(diff < 15) ? "barely " : "",
			attack, (diff > 50) ? " with ease" : "");
    sprintf(buf2,"$N %sdodges your %s%s.",(diff < 15) ? "barely " : "",
			attack, (diff > 50) ? " with ease" : "");
    if(IS_SET(victim->wiznet,WIZ_PERCENT))
        sprintf(buf1,"You dodge $n's %s. (%d%% -- %d%% needed)",attack,roll,(int)chance);
    if(IS_SET(ch->wiznet,WIZ_PERCENT))
        sprintf(buf2,"$N dodges your %s. (%d%% -- %d%% needed)",attack,roll,(int)chance);
	
    act(buf1,ch,0,victim,TO_VICT);
    act(buf2,ch,0,victim,TO_CHAR);
	
    check_improve(victim,gsn_dodge,TRUE,5);

    return TRUE;
}

bool check_avoid( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
    float chance;
    char buf1[MAX_STRING_LENGTH], buf2[MAX_STRING_LENGTH];
    char *attack;
    int dex, dexa, str;
	int encumb;
	int roll, diff;

    if ( !IS_AWAKE(victim) )
        return FALSE;

	if (get_skill(ch, gsn_avoid) == 0)
		return FALSE;

	chance = ( 3 * get_skill(victim,gsn_avoid) / 10);

    dex = get_curr_stat(victim,STAT_DEX);
    dexa= get_curr_stat(ch,STAT_DEX);
    if (dex <= 5)
		chance += 0;
    else if (dex <= 10)
		chance += dex/2;
    else if (dex <=15)
		chance += (2*dex/3);
    else if (dex <= 20)
		chance += (8*dex/10);
    else
		chance += dex;

    chance+=dex-dexa;
    chance+=(ch->size-victim->size)*5;

	if (is_affected_room(ch->in_room,gsn_blanket))
		chance *= 0.8;
	
	/* posture */
	if(!IS_NPC(ch)) {
        if (check_posture(ch) == POSTURE_DEFENSE && style_check(gsn_posture, ch->pcdata->style))
			chance += 20;
	}

	if(!IS_NPC(victim)) {
		if (check_posture(victim) == POSTURE_DEFENSE && style_check(gsn_posture, victim->pcdata->style))
			chance += 20;
		if (check_posture(victim) == POSTURE_OFFENSE && style_check(gsn_posture, victim->pcdata->style))
			chance -= 20;
	}

	if (check_entwine(victim,2))
		chance /= 2;

	if (is_affected(victim,gsn_creeping_tomb))
		chance *= 0.75;

    if(IS_SHIFTED(victim)) {
		chance = get_curr_stat(victim,STAT_DEX) * 2.5;
		if(IS_AFFECTED(victim,AFF_HASTE))
			chance*=1.1;
		if(IS_AFFECTED(victim,AFF_SLOW))
			chance*=.65;
    }
	
    chance+=ch->defense_mod;
	
    chance*=(1-((float)GET_HITROLL(ch) / (float) 250));

	if(!IS_NPC(victim)) {
		encumb = armor_weight(victim);
		str = get_curr_stat(victim,STAT_STR);
		chance *= (float)(300 + (10 * (str - 18)) - encumb)/(float)250;
		if (chance < 0)
			chance = 0;
	}
	
	if (IS_NPC(victim) || IS_NPC(ch)) chance += (victim->level - ch->level);
	
    chance -= 5*abs((ch->size-victim->size));

	chance -= victim->balance;
	chance += ch->balance;

	chance -= ch->analyze;
	if (victim->analyzePC == ch)	chance += victim->analyze;
	
	if (!IS_NPC(victim)) {
		if (style_check(gsn_evasion,victim->pcdata->style))
			chance += check_evasion(victim, (int)chance);
	}
	
	chance = UMIN(chance, 75);
	
	if ((roll = number_percent( )) >= chance)
        return FALSE;
	
	diff = (int)chance - roll;
	
    attack=get_dam_message(ch,dt);
	
    sprintf(buf1,"You %savoid $n's %s%s.",(diff < 15) ? "barely " : "",
			attack, (diff > 50) ? " with ease" : "");
    sprintf(buf2,"$N %savoids your %s%s.",(diff < 15) ? "barely " : "",
			attack, (diff > 50) ? " with ease" : "");
	
    act(buf1,ch,0,victim,TO_VICT);
    act(buf2,ch,0,victim,TO_CHAR);
	
    check_improve(victim,gsn_avoid,TRUE,5);

    return TRUE;
}

int	check_evasion (CHAR_DATA *ch, int chance)
{
	int skill, dex;
	float nododge = 100 - chance;		// chance of NOT dodging
	OBJ_DATA *weapon, *dual;
	
	weapon = get_eq_char(ch,WEAR_WIELD);
	dual = get_eq_char(ch,WEAR_DUAL_WIELD);
	
	if ((skill = get_skill(ch,gsn_evasion)) <= 1) 	
		return chance;
	
	if ((dex = get_curr_stat(ch,STAT_DEX)) <= 18)	
		return chance;
	
	if (weapon != NULL)
	{
		if (weapon->weight > skirmisher_max_weapweight(ch))	
			return chance;
	}

	if (dual != NULL)
	{
		if (dual->weight > skirmisher_max_weapweight(ch))		
			return chance;
	}
	
	if (number_percent() > skill)
	{
		return chance;
	}
	else
	{
		nododge *= ((float)1 - 0.05 * (float)(dex - 18));
		check_improve(ch,gsn_evasion,TRUE,5);
		chance = 100 - (int)nododge;
		if(ch->size > SIZE_MEDIUM)
			chance -= (ch->size - SIZE_MEDIUM) * 10;
	}

	return chance;
}

bool check_fend( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
	float skill;
	OBJ_DATA *wield, *wield2;
	char buf1[MAX_STRING_LENGTH];
	char buf2[MAX_STRING_LENGTH];
	char *attack;

    if(!IS_NPC(victim))
		if(!style_check(gsn_fend, victim->pcdata->style))
			return FALSE;

	if (!IS_AWAKE(victim))
		return FALSE;

	if ((skill = get_skill(victim,gsn_fend)) < 5)
		return FALSE;

	if (!is_wielded(victim,WEAPON_SPEAR,WIELD_PRIMARY)
		&& !is_wielded(victim,WEAPON_POLEARM,WIELD_PRIMARY)
		&& !is_wielded(victim,WEAPON_STAFF,WIELD_PRIMARY))
		return FALSE;

	wield = get_eq_char(victim,WEAR_WIELD);

	wield2 = get_eq_char(ch,WEAR_WIELD);
	if(!wield)
		return FALSE;
	if (wield2 == NULL)
	{
		skill *= .65;
	}
	else
	{
		switch (wield2->value[0])
		{
		case (WEAPON_SWORD): 	skill*=.45;		break;
		case (WEAPON_DAGGER): 	skill*=.65;		break;
		case (WEAPON_SPEAR):	skill*=.25;		break;
		case (WEAPON_STAFF):	skill*=.25;		break;
		case (WEAPON_MACE):		skill*=.55;		break;
		case (WEAPON_AXE):		skill*=.45;		break;
		case (WEAPON_FLAIL):	skill*=.45;		break;
		case (WEAPON_WHIP):		skill*=.45;		break;
		case (WEAPON_POLEARM):	skill*=.25;		break;
		default:				skill*=.35;		break;
		}
	}

	if (is_wielded(victim,WEAPON_POLEARM,WIELD_PRIMARY))
		skill *= 1.2;

	if (ch->size > victim->size + 1)
		skill /= pow(2,(ch->size - (victim->size - 1)));

	if (victim->analyzePC == ch)    
		skill += victim->analyze;
	
	skill = URANGE(5,skill,85);
	
	if (number_percent() >= skill)
		return FALSE;

	attack = get_dam_message(ch,dt);

	sprintf(buf1,"You keep $n's %s at bay with your $t.",attack);
	sprintf(buf2,"$N keeps your %s at bay with $S $t.",attack);
	if(IS_SET(victim->wiznet,WIZ_PERCENT))
		sprintf(buf1,"You keep $n's %s at bay with your $t. (%d%%)",attack,(int)skill);
	if(IS_SET(ch->wiznet,WIZ_PERCENT))
		sprintf(buf2,"$N keeps your %s at bay with $S $t. (%d%%)",attack,(int)skill);
	act(buf1,ch,weapon_name_lookup(wield->value[0]),victim,TO_VICT);
	act(buf2,ch,weapon_name_lookup(wield->value[0]),victim,TO_CHAR);

	check_improve(victim,gsn_fend,TRUE,4);
	return TRUE;
}

bool check_deflect (CHAR_DATA *ch, CHAR_DATA *victim, int dt)
{
	float chance;
	int skill;
	char *attack;
	char buf1[MSL], buf2[MSL];
	OBJ_DATA *sleeves;

	if (!IS_NPC(victim))
		if(!style_check(gsn_deflect,victim->pcdata->style))
			return FALSE;

	if ((skill = get_skill(victim,gsn_deflect)) <= 1)
		return FALSE;
	
	if (!IS_AWAKE(victim))
		return FALSE;
	
	if ((sleeves = get_eq_char(victim,WEAR_ARMS)) == NULL)
		return FALSE;

	if (material_table[sleeves->pIndexData->material_index].mat_hardness <= 2)
		return FALSE;
	
	chance = ((get_skill(victim,gsn_deflect) / 2));

	/* posture */
	
	if(!IS_NPC(ch))
		if (check_posture(ch) == POSTURE_DEFENSE
			&& style_check(gsn_posture, ch->pcdata->style))
			chance += 20;

	if (IS_NPC(victim) || IS_NPC(ch))
		chance += (victim->level - ch->level);

	chance *= (float)(1-(float)((float)GET_HITROLL(ch) / (float) 250));
	chance -= victim->balance;
	chance += ch->balance;

	if (victim->analyzePC == ch)
		chance += victim->analyze;

	if (chance > 95)
		chance = 95;
	
	if (number_percent() >= chance)
		return FALSE;

	attack = get_dam_message(ch,dt);
	
	sprintf(buf1,"You deflect $n's %s with your armored forearm.",attack);
	sprintf(buf2,"$N deflects your %s with $S armored forearm.",attack);
	
    act(buf1,ch,0,victim,TO_VICT);
    act(buf2,ch,0,victim,TO_CHAR);

	check_improve(victim,gsn_deflect,TRUE,4);

	return TRUE;
}

bool check_mist (CHAR_DATA *ch, CHAR_DATA *victim, int dt)
{
	char buf1[MSL], buf2[MSL];
	float chance = 50;
	char *attack;
	
	if (!IS_AWAKE(victim))
		return FALSE;

	chance *= (float)((float) 1- (float)((float)GET_HITROLL(ch) / (float) 250));

	if (number_percent() > chance)
		return FALSE;

	attack = get_dam_message(ch,dt);

	sprintf(buf1,"$n's %s passes harmlessly through your cloak of mist.",attack);
	sprintf(buf2,"Your %s passes harmlessly through $N's cloak of mist.",attack);

	act(buf1,ch,0,victim,TO_VICT);
	act(buf2,ch,0,victim,TO_CHAR);
	return TRUE;
}

/*
 * Tactician skill, decide whether to increment analyze counter
 */

void check_analyze (CHAR_DATA *ch, CHAR_DATA *victim)
{
	int intel, skill;
    if (IS_NPC(ch)) { /*
		skill = 30 + ch->level;
		if (number_percent () > (skill - 125 + (5 * get_curr_stat(ch,STAT_INT)))) return;
		if (victim != ch->analyzePC) ch->analyze = 0;
		ch->analyzePC = ch->fighting;
		if (ch->analyze < 50)   ch->analyze++; */
	 	return;
	}
	if (!style_check(gsn_analyze, ch->pcdata->style)) return;
	
	if ((skill = get_skill(ch,gsn_analyze)) == 0) return;

	intel = get_curr_stat(ch,STAT_INT);
	
	if (number_percent () > (skill - 125 + (5 * intel))) return;

	if (victim != ch->analyzePC) {
		ch->analyze = 0;
	}
	
	ch->analyzePC = ch->fighting;
	
	if (IS_NPC(victim)) {
		if (number_percent() > 40)	return;
	}
	
	if (ch->analyze < 50)	ch->analyze++;
	check_improve(ch,gsn_analyze,TRUE,6);
}

/*
 * Set position of a victim.
 */
void update_pos( CHAR_DATA *victim )
{
	int diff=0;

    if ( victim->hit > 0 )
    {
        if ( victim->position <= POS_STUNNED )
            victim->position = POS_STANDING;
        return;
    }

    if(is_affected(victim, gsn_indomitable_spirit) ) {
	if(victim->hit > (victim->mana * -1))
		return;
    }
	
    if ( IS_NPC(victim) && victim->hit < 1)
		victim->position = POS_DEAD;
	else if ( victim->hit <= -11 )
        victim->position = POS_DEAD;
	else if ( victim->hit <= -6 ) 
		victim->position = POS_MORTAL;
    else if ( victim->hit <= -3 ) 
		victim->position = POS_INCAP;
    else
		victim->position = POS_STUNNED;

    return;
}

/*
 * Start fights.
 */
void set_fighting( CHAR_DATA *ch, CHAR_DATA *victim )
{
	char buf[MSL];
	
	if ( ch->fighting != NULL )
		return;

	if (ch == victim)
		return;

    if ( IS_AFFECTED(ch, AFF_SLEEP) )
        affect_strip( ch, gsn_sleep );

    if ( IS_AFFECTED(ch, gsn_hypothermia) )
        affect_strip( ch, gsn_hypothermia );

	if (is_affected(victim,gsn_bind_feet)) {
		affect_strip(victim, gsn_bind_feet);
		send_to_char("The assault upon you breaks the magic binding your legs to the earth.\n\r",victim);
		}

	if ((IS_NPC(victim) && IS_SET(victim->act,ACT_GUILDGUARD))) {
		sprintf(buf,"Help!  %s is attacking the guild!",PERS(ch,victim));
		do_myell(victim,buf, ch);
	}

	un_shroud(ch,NULL);
	un_shroud(victim,NULL);

    ch->fighting = victim;
    ch->position = POS_FIGHTING;

	if (IS_NPC(victim))
		ch->pause = UMAX(ch->pause, 3);
	else
		ch->pause = UMAX(ch->pause, 7);

	if (IS_NPC(ch))
		victim->pause = UMAX(victim->pause, 3);
	else
		victim->pause = UMAX(ch->pause, 7);

	if(victim && IS_SET(victim->progtypes,MPROG_ATTACK))
		(victim->pIndexData->mprogs->attack_prog) (victim,ch);

    return;
}

void combat_alert(CHAR_DATA *victim, int type, CHAR_DATA *ch)
{
	
	
	return;
}

/*
 * Stop fights.
 */
void stop_fighting( CHAR_DATA *ch, bool fBoth )
{
    CHAR_DATA *fch;

    for ( fch = char_list; fch != NULL; fch = fch->next )
    {
        if ( fch == ch || ( fBoth && fch->fighting == ch ) )
        {
            fch->fighting       = NULL;
            fch->position       = POS_STANDING;
	    if(is_affected(fch, gsn_entwine))
		affect_strip(fch, gsn_entwine);
            update_pos( fch );
        }
    }

    return;
}

/*
 * Make a corpse out of a character.
 */
void make_corpse( CHAR_DATA *killer,CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *corpse;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    char *name;
    OBJ_AFFECT_DATA oaf;

    if ( IS_NPC(ch) )
    {
        name            = ch->short_descr;
        corpse          = create_object(get_obj_index(OBJ_VNUM_CORPSE_NPC), 0);
        corpse->timer   = number_range( 5, 8 );
		//free_pstring(corpse->talked);
	
		if (IS_NPC(killer))
			corpse->talked = palloc_string(killer->short_descr);
		else
			corpse->talked = palloc_string(killer->true_name);

		if ( ch->gold > 0 )
        {
            obj_to_obj( create_money( ch->gold ), corpse );
            ch->gold = 0;
        }
        corpse->cost = 0;
		corpse->mob_vnum = ch->pIndexData->vnum;
    }
    else
    {
        name            = ch->name;
        corpse          = create_object(get_obj_index(OBJ_VNUM_CORPSE_PC), 0);
        corpse->timer   = number_range( 20, 35 );
    	if(ch->in_room && IS_EXPLORE(ch->in_room))
			corpse->timer   = number_range(6,9);
		corpse->value[4] = ch->hometown;
		//free_pstring(corpse->talked);
		if (IS_NPC(killer))
			corpse->talked = palloc_string(killer->short_descr);
		else
			corpse->talked = palloc_string(killer->true_name);
      	corpse->owner = palloc_string(ch->true_name);
		if (ch->gold > 1)
		{
			obj_to_obj(create_money(ch->gold), corpse);
			ch->gold = 0;
		}

        corpse->cost = 0;
    }

    corpse->level = URANGE(1,ch->level,100);
     if (IS_NPC(ch) && (IS_SET(ch->act,ACT_UNDEAD) || IS_SET(ch->form,FORM_UNDEAD)))
	SET_BIT(corpse->extra_flags,CORPSE_NO_ANIMATE);

    corpse->value[2] = ch->size;

    sprintf( buf, corpse->short_descr, name );
    free_pstring( corpse->short_descr );
    corpse->short_descr = palloc_string( buf );

    sprintf( buf, corpse->description, name );
    free_pstring( corpse->description );
    corpse->description = palloc_string( buf );
    corpse->ohp	= ch->max_hit;
    for ( obj = ch->carrying; obj != NULL; obj = obj_next )
    {
        bool floating = FALSE;

        obj_next = obj->next_content;
		if (IS_SET(obj->extra_flags,ITEM_BRAND))
			continue;

		if (!IS_NPC(ch) && IS_SET(obj->extra_flags,ITEM_FIXED)) {
			unequip_char(ch,obj,FALSE);
			equip_char(ch,obj,obj->wear_loc,FALSE);
			continue;
		}

		obj_from_char( obj );

        if (obj->item_type == ITEM_POTION)
            obj->timer = number_range(500,1000);
        if (obj->item_type == ITEM_SCROLL)
            obj->timer = number_range(1000,2500);

        if (IS_SET(obj->extra_flags,ITEM_ROT_DEATH) && !floating)
        {
            obj->timer = 1;
            REMOVE_BIT(obj->extra_flags,ITEM_ROT_DEATH);
        }
	if (obj->pIndexData->start_timer > 0 && IS_NPC(ch))
		obj->timer = obj->pIndexData->start_timer;
        REMOVE_BIT(obj->extra_flags,ITEM_VIS_DEATH);

	if (obj->pIndexData->limcount > obj->pIndexData->limtotal
		 && obj->pIndexData->limtotal > 0)
	{
	    act("$p flashes brightly and vaporizes.",ch,obj,NULL,TO_ROOM);
            extract_obj( obj );
		continue;
	}

        else if (floating)
        {
            if (obj!=NULL && IS_OBJ_STAT(obj,ITEM_ROT_DEATH)) /* get rid of it! */
            {
                if (obj->contains != NULL)
                {
                    OBJ_DATA *in, *in_next;

                    act("$p evaporates, scattering its contents.",
                        ch,obj,NULL,TO_ROOM);
                    for (in = obj->contains; in != NULL; in = in_next)
                    {
                        in_next = in->next_content;
                        obj_from_obj(in);
                        obj_to_room(in,ch->in_room);
                    }
                 }
                 else
                    act("$p evaporates.",
                        ch,obj,NULL,TO_ROOM);
                 extract_obj(obj);
            }
            else
            {
                act("$p falls to the floor.",ch,obj,NULL,TO_ROOM);
                obj_to_room(obj,ch->in_room);
            }
	}
//	else if (obj->wear_loc == WEAR_BRAND)
	else if (IS_SET(obj->extra_flags,ITEM_BRAND))
	{
		/* nothing */
	} else if (!IS_NPC(ch) && IS_SET(obj->extra_flags,ITEM_FIXED))
	{
		/* nothing */
	}
	else
	{
		obj_to_obj( obj, corpse );
	}
    }
    obj_to_room( corpse, ch->in_room );

	if (is_affected(ch,gsn_noxious_ward))
	{
		act( "A cloud of thick smoke billows out of your corpse!", ch, corpse, NULL, TO_CHAR );
		act( "A cloud of thick smoke billows out of $p!", ch, corpse, NULL, TO_ROOM );
		init_affect_obj(&oaf);
		oaf.where        = TO_OBJ_AFFECTS;
		oaf.type         = gsn_noxious_ward;
		oaf.duration     = -1;
		affect_to_obj(corpse,&oaf);
	}

	if (is_affected(ch,gsn_ultradiffusion))
		extract_obj(corpse);

    return;
}



/*
 * Improved Death_cry contributed by Diavolo.
 */
void death_cry( CHAR_DATA *ch, bool infidels )
{
    ROOM_INDEX_DATA *was_in_room;
    char *msg;
    int door;
    int vnum;

    vnum = 0;
    msg = "You hear $n's death cry.";

	if (!is_affected(ch,gsn_ultradiffusion))
	{
    switch ( number_bits(4))
    {
    case  0: msg  = "$n hits the ground ... DEAD.";                     break;
    case  1:
        if (ch->material == 0)
        {
            msg  = "$n splatters blood on your armor.";
            break;
        }
    case  2:
        if (IS_SET(ch->parts,PART_GUTS))
        {
            msg = "$n spills $s guts all over the floor.";
            vnum = OBJ_VNUM_GUTS;
        }
        break;
    case  3:
        if (IS_SET(ch->parts,PART_HEAD) && !infidels)
        {
            msg  = "$n's severed head plops on the ground.";
            vnum = OBJ_VNUM_SEVERED_HEAD;
        }
        break;
    case  4:
        if (IS_SET(ch->parts,PART_HEART))
        {
            msg  = "$n's heart is torn from $s chest.";
            vnum = OBJ_VNUM_TORN_HEART;
        }
        break;
    case  5:
        if (IS_SET(ch->parts,PART_ARMS))
        {
            msg  = "$n's arm is sliced from $s dead body.";
            vnum = OBJ_VNUM_SLICED_ARM;
        }
        break;
    case  6:
        if (IS_SET(ch->parts,PART_LEGS))
        {
            msg  = "$n's leg is sliced from $s dead body.";
            vnum = OBJ_VNUM_SLICED_LEG;
        }
        break;
    case 7:
        if (IS_SET(ch->parts,PART_BRAINS))
        {
            msg = "$n's head is shattered, and $s brains splash all over you.";
            vnum = OBJ_VNUM_BRAINS;
        }
    }

    act( msg, ch, NULL, NULL, TO_ROOM );

    if ( vnum != 0 )
    {
        char buf[MAX_STRING_LENGTH];
        OBJ_DATA *obj;
        char *name;

        name            = IS_NPC(ch) ? ch->short_descr : ch->name;
        obj             = create_object( get_obj_index( vnum ), 0 );
        obj->timer      = number_range( 4, 7 );
		obj->level      = ch->level;
        sprintf( buf, obj->short_descr, name );
        free_pstring( obj->short_descr );
        obj->short_descr = palloc_string( buf );

        sprintf( buf, obj->description, name );
        free_pstring( obj->description );
        obj->description = palloc_string( buf );

        if (obj->item_type == ITEM_FOOD)
        {
            if (IS_SET(ch->form,FORM_POISON))
                obj->value[3] = 1;
            else if (!IS_SET(ch->form,FORM_EDIBLE))
                obj->item_type = ITEM_TRASH;
        }

        obj_to_room( obj, ch->in_room );
    }

    if ( IS_NPC(ch) )
        msg = "You hear something's death cry.";
    else
        msg = "You hear someone's death cry.";

    was_in_room = ch->in_room;
    for ( door = 0; door <= 5; door++ )
    {
        EXIT_DATA *pexit;

        if ( ( pexit = was_in_room->exit[door] ) != NULL
        &&   pexit->u1.to_room != NULL
        &&   pexit->u1.to_room != was_in_room )
        {
            ch->in_room = pexit->u1.to_room;
            act( msg, ch, NULL, NULL, TO_ROOM );
        }
    }
    ch->in_room = was_in_room;
	}

    return;
}



void raw_kill( CHAR_DATA *ch,CHAR_DATA *victim )
{
	int i, sn;
	ROOM_INDEX_DATA *location;
	AFFECT_DATA af, *paf, *paf_next;
	ROOM_AFFECT_DATA raf;
	AREA_AFFECT_DATA aaf;
	CHAR_DATA *gch, *gch_next;
	OBJ_DATA *obj, *obj2, *corpse;
	char wizbuf[MSL], *cname,buf[MSL],buf2[MSL];
	bool infidels=FALSE;

    stop_fighting( victim, TRUE );
    if(!IS_NPC(ch) && !IS_NPC(victim) && IS_EVIL(victim) && get_skill(ch,gsn_infidels_fate) > 0)
    	if(number_percent() < (10 + (get_skill(ch,gsn_infidels_fate) - 30 )))
			infidels = TRUE;
	death_cry(victim,infidels);

    if (IS_NPC(ch) && ch->pIndexData->vnum == 23630) 
 	{
		victim->hit = 1;
		communion_handler(victim);
		return;
    }

    if (!IS_NPC(victim) && IS_IMMORTAL(victim))     	/* Imm deaths */
    {
		if (victim->pcdata->imm_death) 
		{
			act("$T",victim,NULL,victim->pcdata->imm_death,TO_ROOM);
			act("$T",victim,NULL,victim->pcdata->imm_death,TO_CHAR);
        } 
		else
		{
         	act("$n's skull glows brightly and $n rises from $s ashes!",victim,0,0,TO_ROOM);
            send_to_char("Your skull glows brightly and you rise from your ashes!\n\r",victim);
        }
		return;
    }


	for (obj = victim->carrying;obj != NULL;obj = obj->next_content) 
	{
		if (IS_SET(obj->progtypes,IPROG_DEATH))
			if ((obj->pIndexData->iprogs->death_prog) (obj,victim)) 
				return;
	  	if(TRAPS_IEVENT(obj,TRAP_IDEATH))
			if(CALL_IEVENT(obj,TRAP_IDEATH,victim,obj))
				return;
	}
	

	if(IS_NPC(ch) && IS_AFFECTED(ch,AFF_CHARM) && ch->master
		&& ch->master->in_room && ch->master->in_room == ch->in_room)
			ch = ch->master;


	if ( IS_SET(victim->progtypes,MPROG_DEATH))
		if ((victim->pIndexData->mprogs->death_prog) (victim,ch)) 
		        return;
	if (IS_NPC(victim) && TRAPS_MEVENT(victim, TRAP_MDEATH))
                  if(CALL_MEVENT(victim,TRAP_MDEATH,ch,victim) > 0)
			return;
	temp_death_log(ch,victim);
	mob_death_log(ch,victim);

	make_corpse( ch,victim );

	if(!IS_NPC(ch) && !IS_NPC(victim) && victim!=ch) 	/* BOUNTY AND BLOODLUST! */
	{
		if(ch->pcdata->bounty && victim->cabal == CABAL_BOUNTY)
			ch->pcdata->bounty_killed++;
      	pay_bounty(ch, victim);
		if(IS_GOOD(victim) && get_skill(ch, gsn_bloodlust) > 1)
			check_bloodlust(ch,victim);
		if(is_affected(ch,gsn_orobas_soul) || is_affected(ch,gsn_gamygyn_soul))
			check_orobas_gamygyn(ch,victim);
	}


	if(!IS_NPC(victim) && ch->in_room == victim->in_room && !IS_NPC(ch) && get_skill(ch,gsn_trophy) > 20)
		trophy_corpse(ch,victim);

	if (ch && victim)
	{
		sprintf( wizbuf, "%s died to %s at %s [room %d]",
            	(IS_NPC(victim) ? victim->short_descr : victim->name),
            	(IS_NPC(ch) ? ch->short_descr : ch->name),
            	get_room_name(ch->in_room), ch->in_room->vnum);
		if (IS_NPC(victim))
	            wiznet(wizbuf,NULL,NULL,WIZ_MOBDEATHS,0,0);
		else
            	wiznet(wizbuf,NULL,NULL,WIZ_DEATHS,0,0);
	}

	for (gch = char_list; gch != NULL; gch = gch_next)
	{
		gch_next = gch->next;
		if (IS_NPC(gch) && gch->master == victim && gch->pIndexData->vnum != ACADEMY_PET && !IS_NPC(victim))
			extract_char(gch,TRUE);
    	}

	if (!IS_NPC(ch))
		pk_record(ch,victim);
	else if (!IS_NPC(victim))
			victim->pcdata->killed[MOB_KILLED]++;

    	if ( IS_NPC(victim) || IS_SWITCHED(victim))
    	{
	 	 victim->pIndexData->killed++;
	  	kill_table[URANGE(0, victim->level, MAX_LEVEL-1)].killed++;
	  	extract_char( victim, TRUE );
	  	return;
    	}

    	if (is_affected(victim,gsn_disguise))
		do_undisguise(victim,"");
	
    	for (paf = victim->affected; paf != NULL; paf = paf_next)
	{
		paf_next = paf->next;
		if(IS_SET(paf->bitvector,AFF_PERMANENT))
			continue;
		affect_remove( victim, paf );
    	}

	extract_char( victim, FALSE );
	
	for (paf = victim->affected; paf != NULL; paf = paf_next)
	{
		paf_next = paf->next;
		if(IS_SET(paf->bitvector,AFF_PERMANENT))
			continue;
		affect_remove( victim, paf );
    	}

    	victim->talismanic = 0;
    	BITWISE_OR(victim->affected_by, race_data_lookup(victim->race)->aff);
    	for (i = 0; i < 4; i++)
        	victim->armor[i]= 0;
    	victim->position    					= 	POS_RESTING;
    	victim->hit         					= 	1;
    	victim->mana        					= 	1;
    	victim->move        					= 	victim->max_move;
    	victim->pcdata->condition[COND_THIRST]		=	0;
    	victim->pcdata->condition[COND_HUNGER]		=	0;
    	victim->pcdata->condition[COND_DRUNK] 		= 	0;
    	victim->ghost = 20;
    	victim->bounty_timer = 100;

    	send_to_char("You have died and turned into an invincible ghost for several minutes.\n\r",victim);

    	if (victim->hometown)
		location = get_room_index(hometown_table[victim->hometown].recall);
    	else
		location = get_room_index(ROOM_VNUM_TEMPLE);

   	char_from_room(victim);
    	char_to_room(victim,location);
    	init_affect(&af);
    	af.where = TO_AFFECTS;
    	af.aftype = AFT_SKILL;
    	af.type = skill_lookup("infravision");
    	af.name = palloc_string("ghost");
    	af.duration = 10;
    	af.location = 0;
	if(!IS_AFFECTED(victim, AFF_WATERBREATH))
	    	SET_BIT(af.bitvector, AFF_WATERBREATH);
	if(!IS_AFFECTED(victim, AFF_DARK_VISION))
		SET_BIT(af.bitvector, AFF_DARK_VISION);
	if(!IS_AFFECTED(victim, AFF_PASS_DOOR))
		SET_BIT(af.bitvector, AFF_PASS_DOOR);
	if(!IS_AFFECTED(victim, AFF_FLYING))
		SET_BIT(af.bitvector, AFF_FLYING);
    	af.modifier = 0;
    	af.level = victim->level;
   	affect_to_char(victim,&af);

	victim->last_fight_name = NULL;
	victim->last_fight_time = 0;

	if (!IS_NPC(victim))  /* con loss */
	{
		victim->pcdata->death_count++;
		if ((victim->pcdata->death_count >= 4)
		     && !IS_IMMORTAL(victim)
		     && !(victim->perm_stat[STAT_CON] < 3))
		{
			victim->perm_stat[STAT_CON] -= 1;
			victim->pcdata->death_count = 0;
			send_to_char("With this death you feel your vitality draining away from you.\n\r",victim);
		}
		if (victim->perm_stat[STAT_CON] < CON_DIE_BOUND)
		{
			SET_BIT(victim->act, PLR_DENY);
			perm_death_log(victim,2);
			send_to_char("You gasp your final breaths. Your vitality has faded.\n\r",victim);
			wiznet("$N dies of constitution loss.",victim,NULL,0,0,0);
			stop_fighting(victim,TRUE);
			victim->pause = 0;
			save_char_obj(victim);
			cname = palloc_string(victim->true_name);
			plug_graveyard(victim, 2);
			do_quit_new(victim,"",TRUE);
			delete_char(cname, TRUE);
			return;
		}
		if (victim->perm_stat[STAT_CON] < 5)
			send_to_char("The banshee wails as you feel closer to a permanent death.\n\r",victim);
	}

    	if(IS_NPC(ch) && IS_AFFECTED(ch,AFF_CHARM) && ch->master)
		ch = ch->master;

	if(infidels == TRUE)
	{
			act("You impale $N's head on a ceremonial spear, making an example of $M.",ch,0,victim,TO_CHAR);
			act("$n impales $N's head on a ceremonial spear!",ch,0,victim,TO_ROOM);
			obj2 = create_object(get_obj_index(2999),ch->level);
			obj2->timer = ch->level;
			obj_to_room(obj2,ch->in_room);
				
			init_affect_room(&raf);
			raf.where = TO_ROOM_AFFECTS;
			raf.type = gsn_infidels_fate;
			raf.duration = ch->level;
			raf.modifier = 0;
			raf.location = 0;
			affect_to_room(ch->in_room,&raf);
			init_affect_area(&aaf);
			aaf.where = TO_AREA_AFFECTS;
			aaf.type = gsn_infidels_fate;
			aaf.duration = ch->level;
			affect_to_area(ch->in_room->area,&aaf);
		
			sprintf(buf,obj2->short_descr,victim->name);
			free_pstring(obj2->short_descr);
			obj2->short_descr = palloc_string(buf);
			sprintf(buf,obj2->description,victim->name);
			free_pstring(obj2->description);
			obj2->description = palloc_string(buf);
			sprintf(buf2,"{RA hush falls over your surroundings as %s is raised.{x",obj2->short_descr);
			zone_echo(ch->in_room->area,buf2);
	}

	/* Skill decay upon mobdeath. */
	if(!IS_NPC(victim) && IS_NPC(ch) && victim->level < 51)
		for(sn = 0; sn < MAX_SKILL; sn++)
			if (victim->pcdata->learned[sn] > 10 &&
			   (number_percent() > (8 * get_curr_stat(victim,STAT_WIS) - 120)))
				  victim->pcdata->learned[sn]--;
	
	act("$n appears in the room.",victim,0,0,TO_ROOM);
    	do_look(victim,"auto");

	return;
}

void pk_record(CHAR_DATA *ch, CHAR_DATA *victim)
{
	char buf[MSL];
	CHAR_DATA *wch;
	int killer_group = 0, victim_group = 0;
	int killer_in_room = 0;
	float killer_credit, victim_credit;
	int i;

	if (IS_NPC(victim) || IS_NPC(ch))
		return;
	
	for (wch = char_list; wch; wch = wch->next) {
		if (is_same_group(wch,ch)
			&& !IS_NPC(wch)
			&& ((wch->in_room == ch->in_room)
			|| (wch->ghost > 0))) {
			sprintf(buf,"Adding %s to killer_group",wch->name);
			log_string(buf);
			killer_group++;
		}
		if (is_same_group(wch,ch) && wch->in_room == ch->in_room && !IS_NPC(wch)) {
			sprintf(buf,"Adding %s to killer_in_room",wch->name);
			log_string(buf);
			killer_in_room++;
		}
		if (is_same_group(wch,victim)
			&& ((wch->in_room == victim->in_room)
			|| (wch->ghost > 0))) {
			sprintf(buf,"Adding %s to victim_group.",wch->name);
			log_string(buf);
			victim_group++;
		}
	}

	victim_credit = (float) ((float)victim_group / (float)killer_group);
	
	if (victim_credit > 1)
		victim_credit = 1;
	killer_credit = victim_credit;
	
	killer_credit = (float)(killer_credit/(float)killer_in_room);

	victim->pcdata->fragged += victim_credit;

	for (wch = char_list; wch; wch = wch->next) {
		if (is_same_group(wch,ch)
			&& can_pk (wch,victim)
			&& !IS_NPC(wch)
			&& wch->in_room == ch->in_room) {
			wch->pcdata->frags[PK_KILLS] += killer_credit;

			if (IS_GOOD(victim))
				wch->pcdata->frags[PK_GOOD] += killer_credit;
			else if (IS_NEUTRAL(victim))
				wch->pcdata->frags[PK_NEUTRAL] += killer_credit;
			else if (IS_EVIL(victim))
				wch->pcdata->frags[PK_EVIL] += killer_credit;
			
			if (wch->level < 51)
				{
				for (i=0;i<100;i++) {
					if (wch->pcdata->recentkills[i] == NULL)
						break;
					if (*(wch->pcdata->recentkills[i]) == '\0')
						break;
				}
				if(i<95)
					wch->pcdata->recentkills[i] = palloc_string(victim->name);
			}
		}
	}
	
	ch->pcdata->kills[PK_KILLS]++;
	
	if (victim->alignment > 0)
		ch->pcdata->kills[PK_GOOD]++;
	else if (victim->alignment == 0)
		ch->pcdata->kills[PK_NEUTRAL]++;
	else if (victim->alignment < 0)
		ch->pcdata->kills[PK_EVIL]++;
	
	victim->pcdata->killed[PK_KILLED]++;
	
	if (ch->level < 51) {
		for (i=0;i<100;i++) {
			if (ch->pcdata->recentkills[i] == NULL)
				break;
			if (*(ch->pcdata->recentkills[i]) == '\0')
				break;
			}
		if(i < 95)
			ch->pcdata->recentkills[i] = palloc_string(victim->name);
	}

	return;
}

void group_gain( CHAR_DATA *ch, CHAR_DATA *victim )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *gch;
    CHAR_DATA *lch;
    int xp;
    int members;
    int group_levels;
	bool ispk = FALSE;

    /*
     * Monsters don't get kill xp's or alignment changes.
     * P-killing doesn't help either.
     * Dying of mortal wounds or poison doesn't give xp to anyone!
     */

    if ( victim == ch )
        return;
    if (!IS_NPC(victim))
		ispk = TRUE;

    members = 0;
    group_levels = 0;
    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room ) {
        if ( is_same_group( gch, ch ) ) {
            //if(!IS_NPC(gch))
	    members++;
            group_levels += (IS_NPC(gch) && gch->master) ? gch->master->level : gch->level;
	    //note: the preceding code skews the group average a bit in favor of the group leader if there's npcs in the group
	    //but i don't really care
        }
    }

    if ( members == 0 )
        members = 1;

    lch = (ch->leader != NULL) ? ch->leader : ch;

	if (ch->in_room == NULL)	return;

    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room ) {
        if ( !is_same_group( gch, ch ))// || IS_NPC(gch))
            continue;
        if ( gch->level - lch->level > 8 ) {
            send_to_char( "You are too high for this group.\n\r", gch );
            continue;
        }

        if ( gch->level - lch->level < -8 )
        {
            send_to_char( "You are too low for this group.\n\r", gch );
            continue;
        }
	
		if (!ispk)
	        xp = xp_compute(gch, victim, members, group_levels );
		else
			xp = xp_compute_pk(gch, victim, members);

		if (xp < 0)
			xp = 0;

		if (gch->level < 51) {
			if (xp > 0)
				sprintf(buf, "%sYou receive %d experience points.%s\n\r",
					get_char_color(gch,"white"),xp,END_COLOR(gch));
			else
				sprintf(buf, "%sYou receive no experience from this victory.%s\n\r",
					get_char_color(gch,"white"),END_COLOR(gch));
		} else {
sprintf(buf,"%sYou have already reached the pinnacle of your learning.%s\n\r",get_char_color(gch,"white"),END_COLOR(gch));
		}
        send_to_char( buf, gch );
        gain_exp( gch, xp );
    }

    return;
}

int xp_compute_pk (CHAR_DATA *ch, CHAR_DATA *victim, int members)
{
	int i;
	float xp, netpk, totalpk;
	bool recentpk = FALSE;

	if (IS_NPC(ch) || IS_NPC(victim))
		return 0;
	
	for (i=0; ch->pcdata->recentkills[i] && *(ch->pcdata->recentkills[i]) != '\0' && i<95;i++)
		if (!str_cmp(ch->pcdata->recentkills[i],victim->name)) {
			recentpk = TRUE;
			break;
		}

	/* We don't want to encourage low-level PKing */
	
	if (victim->level < 15)
		xp = 0;
	else if (victim->level < 20)
		xp = victim->level;
	else if (victim->level < 25)
		xp = victim->level * 2;
	else if (victim->level < 30)
		xp = victim->level * 4;
	else if (victim->level < 35)
		xp = victim->level * 8;
	else if (victim->level < 40)
		xp = victim->level * 12;
	else if (victim->level < 45)
		xp = victim->level * 16;
	else
		xp = victim->level * 20;

	/* Level mods */

	xp *= (float) (1 + (float) (0.05 * (victim->level - ch->level)));
	
	/* Alignment mods */
	
	if (IS_GOOD(ch)) {
		if (IS_GOOD(victim)) {
			send_to_char("With the final strike, you feel a twinge in your soul.\n\r",ch);
			xp = 0;
		}
		if (IS_NEUTRAL(victim))
			xp /= 2;
		if (IS_EVIL(victim))
			xp *= 1.25;
		xp += 5 * victim->pcdata->kills[PK_GOOD];
	}

	if (IS_NEUTRAL(ch)) {
		if (IS_GOOD(victim)) {
			if (ch->pcdata->kills[PK_GOOD] < ch->pcdata->kills[PK_EVIL])
				xp *= 1.2;
			if (ch->pcdata->kills[PK_GOOD] > ch->pcdata->kills[PK_EVIL])
				xp *= 0.8;
		}
		
		if (IS_EVIL(victim)) {
			if (ch->pcdata->kills[PK_GOOD] > ch->pcdata->kills[PK_EVIL])
				xp *= 1.2;
			if (ch->pcdata->kills[PK_GOOD] < ch->pcdata->kills[PK_EVIL])
				xp *= 0.8;
		}
	}

	/* Cabal mods */

	if (ch->cabal && !victim->cabal)
		xp *= 0.8;
	if (!ch->cabal && victim->cabal)
		xp *= 1.2;

	/* Victim PK mods */

	netpk = victim->pcdata->frags[PK_KILLS] - victim->pcdata->fragged;
	totalpk = victim->pcdata->frags[PK_KILLS] + victim->pcdata->fragged;

	if (totalpk > 5)
		xp *= (1 + (netpk / totalpk));

	/* We do not reward gangbanging */

	xp /= pow(members,2);
	
	/* We do not reward multikilling */

	if (recentpk) {
		xp = 0;
		send_to_char("You have little more to learn from slaying this foe repeatedly.\n\r",ch);
	}
	
	return (int)xp;
}

/*
 * Compute xp for a kill.
 * Also adjust alignment of killer.
 * Edit this function to change xp computations.
 */
int xp_compute( CHAR_DATA *gch, CHAR_DATA *victim, int group_amount, int glevel )
{
    float xp,base_exp;
    int level_range;
    float mult, peer_factor = BASE_PEER_FACTOR, gavg = glevel / group_amount;
    CHAR_DATA *cPeers = NULL;
	mult = (100 - (gavg > gch->level ? gavg - gch->level : gch->level - gavg) * 6) / 100;
    level_range = victim->level - gch->level;

    /* compute the base exp */
    switch (level_range) {
        default :       base_exp =   0;         break;
        case -9 :       base_exp =   2;         break;
        case -8 :       base_exp =   4;         break;
        case -7 :       base_exp =   7;         break;
        case -6 :       base_exp =  12;         break;
        case -5 :       base_exp =  14;         break;
        case -4 :       base_exp =  25;         break;
        case -3 :       base_exp =  36;         break;
        case -2 :       base_exp =  55;         break;
        case -1 :       base_exp =  85;         break;
        case  0 :       base_exp = 100;         break;
        case  1 :       base_exp = 150;         break;
        case  2 :       base_exp = 200;         break;
        case  3 :       base_exp = 250;         break;
        case  4 :       base_exp = 300;         break;
    }

    if (level_range > 4)
        base_exp = 200 + 20 * level_range;

    if (IS_GOOD(gch))
	{
        if (IS_EVIL(victim))
            xp = (base_exp *4)/3;
        else if (IS_GOOD(victim))
            xp = 0;
        else
            xp = base_exp;
    }
    else if (IS_EVIL(gch))
	{
        if (IS_GOOD(victim))
            xp = (base_exp * 4)/3;
        else if (IS_EVIL(victim))
            xp = base_exp/2;
        else
            xp = base_exp;
    }
    else /* neutral */
		xp = base_exp;

    xp = number_range ((int)(xp * 3/4), (int)(xp * 5/4));
    xp *= mult;
    
//    if(IS_NPC(victim))
//	xp *= (float)victim->pIndexData->xp_mod / (float)100;
    if(IS_NPC(victim) && victim->law_pass)
		xp *= SUMMONED_XP_PENALTY;
    /* adjust for grouping */
 
    //if (group_amount == 2)
    //    xp = (xp * 4)/3;
    //if (group_amount == 3)
    //
	  xp = (xp * 12)/10;
    if (group_amount > 3)
		xp /= (group_amount - 2);
	
    if(!IS_NPC(gch))
	for(cPeers = char_list; cPeers; cPeers = cPeers->next)
		if(!IS_NPC(cPeers) && cPeers != gch &&
			cPeers->level > gch->level - PEER_BALANCE_DISTANCE &&
			cPeers->level < gch->level + PEER_BALANCE_DISTANCE)
				peer_factor = UMAX(1, peer_factor - .25);
    if(group_amount == 2)
	peer_factor = UMAX(1, peer_factor * .66);
    if(group_amount == 3)
	peer_factor = 1;
    xp *= peer_factor;
/*    sprintf(buf,"(WIZ_LOGGING) %s gains experience in a group of %d: peer modifier is %f%%.",  
		gch->name,group_amount,peer_factor*100);
    wiznet(buf,NULL,NULL,WIZ_LOG,0,0); */
    return (int)xp;
}


void dam_message( CHAR_DATA *ch, CHAR_DATA *victim,int dam,int dt,bool immune, char *dnoun, bool reduced)
{
    char buf1[256], buf2[256], buf3[256],buf97[256];
    OBJ_DATA *wield;
    CHAR_DATA *gch;
    const char *vs;
    const char *vp;
    char *attack = NULL, *attack2;
    char punct;
	int prop;
    bool asterisk = FALSE, duped = FALSE;

    if(dam < 0) {
	    prop = 0;
    } else {
	    prop = dam;
    }

    
    if (ch == NULL || victim == NULL || !victim->in_room || !ch->in_room)
        return;

	 if (reduced) { vs = "\x01B[0mfail to harm"; vp = "\x01B[0mfails to harm"; }
	else if ( prop == 0 ) { vs = "miss";       vp = "misses";          }
    else if ( prop <=  2 ) { vs = "scratch";    vp = "scratches";       }
    else if ( prop <=  4 ) { vs = "graze";      vp = "grazes";          }
    else if ( prop <=  7 ) { vs = "hit";        vp = "hits";            }
    else if ( prop <=  11 ) { vs = "injure";     vp = "injures";         }
    else if ( prop <=  15 ) { vs = "wound";      vp = "wounds";          }
    else if ( prop <=  20 ) { vs = "maul";       vp = "mauls";           }
    else if ( prop <=  25 ) { vs = "decimate";   vp = "decimates";       }
    else if ( prop <=  30 ) { vs = "devastate";  vp = "devastates";      }
    else if ( prop <=  37 ) { vs = "maim";       vp = "maims";           }
    else if ( prop <=  45 ) { vs = "MUTILATE";   vp = "MUTILATES";       }
    else if ( prop <=  55 ) { vs = "EVISCERATE"; vp = "EVISCERATES";     }
    else if ( prop <=  65 ) { vs = "DISMEMBER";  vp = "DISMEMBERS";   	 }
    else if ( prop <=  85 ) { vs = "MASSACRE";   vp = "MASSACRES";       }
    else if ( prop <=  100 ) { vs = "MANGLE";     vp = "MANGLES";         }
    else if ( prop <=  135 ) { vs = "*** DEMOLISH ***";
                              vp = "*** DEMOLISHES ***";                 }
    else if ( prop <=  160 ) { vs = "*** DEVASTATE ***";
                              vp = "*** DEVASTATES ***";                 }
    else if ( prop <= 250 ) { vs = "=== OBLITERATE ===";
                              vp = "=== OBLITERATES ===";                }
    else if ( prop <= 330 ) { vs = ">>> ANNIHILATE <<<";
                              vp = ">>> ANNIHILATES <<<";                }
    else if ( prop <= 380 ) { vs = "<<< ERADICATE >>>";
                              vp = "<<< ERADICATES >>>";                 }
    else { vs = "do UNSPEAKABLE things to";
           vp = "does UNSPEAKABLE things to";       	 }

    punct   = (prop <= 33) ? '.' : '!';


/* Now quickly set dual wield weapon dt to real weapon dt - this allows
dual wielded weapons to be kept track of until this point, now we no
longer need to know if the weapon doing the dam is the dual one, just what
it's actual dt is
-Ceran
*/
    if (dt  == gsn_dual_wield) {
		if (dt == gsn_dual_wield)
    		wield = get_eq_char(ch,WEAR_DUAL_WIELD);
		dt = TYPE_HIT;
		if (wield != NULL)
			dt += wield->value[3];
		else
			dt += ch->dam_type;
	}


    if ( dt == TYPE_HIT ) {
        if (ch == victim) {
			if (prop < victim->max_hit) {
            	sprintf( buf1, "$n %s $mself%c",vp,punct);
            	sprintf( buf2, "You %s%s%s yourself%c",
					get_char_color(ch,"damage"), vs, END_COLOR(ch), punct);
			} else {
				sprintf( buf1, "$n's assault slays $mself outright!");
				sprintf( buf2, "Your assault slays you outright!");
			}
        } else {
			for (gch = victim->in_room->people; gch != NULL;
				gch = gch->next_in_room) {
			    if (gch != ch && gch != victim) {
					if (prop < victim->max_hit) {
						sprintf( buf1, "$n %s%s%s %s%c",
							get_char_color(gch,"damage"), vp, END_COLOR(gch),
							get_descr_form(victim,gch,FALSE),punct);
						act(buf1,ch,NULL,gch,TO_VICT);
					} else {
						sprintf( buf1, "$n's assault slays %s outright!",
							get_descr_form(victim,gch,FALSE));
						act(buf1,ch,NULL,gch,TO_VICT);
					}
			    }
			}
			
			sprintf( buf2, "You %s%s%s $N%c", get_char_color(ch,"damage"),
				vs, END_COLOR(ch), punct );
			sprintf( buf3, "$n %s%s%s you%c", get_char_color(victim,"damage"),
				vp, END_COLOR(victim), punct );
        }
    } else {
		if ( dt >= 0 && dt < MAX_SKILL )
            attack      = skill_table[dt].noun_damage;
        else if ( dt >= TYPE_HIT && dt < TYPE_HIT + MAX_DAMAGE_MESSAGE)
            attack      = attack_table[dt - TYPE_HIT].noun;
        else {
            dt  = TYPE_HIT+17;
            attack  = attack_table[0].noun;
        }
		
		if (dnoun) {
			duped = TRUE;
			attack = palloc_string(dnoun);
		}

		if (attack && attack[strlen(attack)-1] == '$') {
			vp = vs;
			attack2 = palloc_string(attack);
			chop(attack2);
			if (duped)
				free_pstring(attack);
			attack = palloc_string(attack2);
			free_pstring(attack2);
		} else if (attack && attack[strlen(attack)-1] == '#') {
			vs = vp;
			attack2 = palloc_string(attack);
			chop(attack2);
			if (duped)
				free_pstring(attack);
			attack = palloc_string(attack2);
			free_pstring(attack2);
		}
		
		if (immune) {
		    if (attack && attack[strlen(attack)-1] == '*') {
				attack2 = palloc_string(attack);
				chop(attack2);
				if (duped)
					free_pstring(attack);
				attack = palloc_string(attack2);
				duped = TRUE;
				free_pstring(attack2);
				asterisk = TRUE;
                sprintf(buf1,"$N is unaffected by %s!",attack);
				if (ch == victim)
					sprintf(buf2,"You are unaffected by %s!",attack);
				else
	                sprintf(buf2,"$N is unaffected by %s!",attack);
                sprintf(buf3,"You are unaffected by %s!",attack);
            } else {
				if (ch == victim) {
       	       		sprintf(buf1,"$n is unaffected by $s %s.",attack);
               		sprintf(buf2,"You are unaffected by your %s.",attack);
            	} else {
                	sprintf(buf1,"$N is unaffected by $n's %s!",attack);
                	sprintf(buf2,"$N is unaffected by your %s!",attack);
                	sprintf(buf3,"You are unaffected by $n's %s!",attack);
            	}
	    	}
        } else {
	    	if (attack && attack[strlen(attack)-1] == '*') {
				attack2 = palloc_string(attack);
				chop(attack2);
				if (duped)
					free_pstring(attack);
				attack = palloc_string(attack2);
				duped = TRUE;
				free_pstring(attack2);
				asterisk = TRUE;
				for (gch = victim->in_room->people; gch != NULL;
					gch = gch->next_in_room) {
				    if (gch != ch && gch != victim) {
						if (prop < victim->max_hit)						
							sprintf( buf1, "%s %s%s%s %s%c", attack,
								get_char_color(gch,"damage"),vp, END_COLOR(gch),
				    			get_descr_form(victim,gch,FALSE),punct);
						else
							sprintf(buf1,"%s slays %s outright!", attack,
								get_descr_form(victim,gch,FALSE));
						act(buf1,ch,NULL,gch,TO_VICT);
			    	}
				}
			
				if (ch == victim) {
					if (prop < victim->max_hit)
						sprintf( buf2, "%s %s%s%s you%c", attack,
							get_char_color(ch,"damage"),vp,END_COLOR(ch),punct);
					else
						sprintf( buf2, "%s slays you outright!",attack);
				} else {
					if (prop < victim->max_hit)
		   	      		sprintf( buf2, "%s %s%s%s $N%c", attack,
							get_char_color(ch,"damage"),vp,END_COLOR(ch),punct);
					else
						sprintf( buf2, "%s slays $N outright!",attack);
				}

				if (prop < victim->max_hit)
	       	 		sprintf( buf3, "%s %s%s%s you%c", attack,
						get_char_color(victim,"damage"), vp, END_COLOR(victim),
						punct);
				else
					sprintf (buf3, "%s slays you outright!",attack);
			} else if (ch == victim) {
				for (gch = victim->in_room->people; gch != NULL;
					gch = gch->next_in_room) {
		   		 	if (gch != ch && gch != victim) {
						if (prop < victim->max_hit)
							sprintf( buf1, "$n's %s %s%s%s $m%c", attack,
								get_char_color(gch,"damage"), vp,
								END_COLOR(gch), punct);
						else
							sprintf( buf1, "$n's %s slays $m outright!",attack);
						act(buf1,ch,NULL,gch,TO_VICT);
		   		 	}
				}
		
				if (prop < victim->max_hit)
					sprintf( buf2, "Your %s %s%s%s you%c", attack,
						get_char_color(ch,"damage"), vp, END_COLOR(ch), punct);
				else
					sprintf( buf2, "Your %s slays you outright!", attack);
			} else {
				for (gch = victim->in_room->people; gch != NULL;
					gch = gch->next_in_room) {
			    	if (gch != ch && gch != victim) {
						if (prop < victim->max_hit)
							sprintf( buf1, "$n's %s %s%s%s %s%c", attack,
								get_char_color(gch,"damage"),vp,END_COLOR(gch),
					    		get_descr_form(victim,gch,FALSE),punct);
						else
							sprintf( buf1, "$n's %s slays %s outright!", attack,
								get_descr_form(victim,gch,FALSE));
						act(buf1,ch,NULL,gch,TO_VICT);
					}
				}
			
				if (prop < victim->max_hit)
					sprintf( buf2, "Your %s %s%s%s $N%c", attack,
						get_char_color(ch,"damage"), vp, END_COLOR(ch), punct);
				else
					sprintf( buf2, "Your %s slays $N outright!",attack);
       	
				if (prop < victim->max_hit)
					sprintf( buf3, "$n's %s \%s%s%s you%c", attack,
						get_char_color(victim,"damage"), vp,
						END_COLOR(victim), punct);
				else
					sprintf( buf3, "$n's %s slays you outright!",attack);
			}
		}
	}

	if(!str_cmp(attack,"")) {
		sprintf(buf97,"Log: Damage without noun. Char: %s, Victim: %s, DT: %d",ch->true_name,victim->true_name,dt);
 		wiznet(buf97,NULL,NULL,WIZ_LOG,0,0);
		}

   if (ch == victim) {
        act_new(buf2,ch,NULL,NULL,TO_CHAR,POS_INCAP);
    } else {
		if (!(asterisk && ch->in_room != victim->in_room))
			act( buf2, ch, NULL, victim, TO_CHAR );
        act( buf3, ch, NULL, victim, TO_VICT );
    }
	
	if (duped)
		free_pstring(attack);

    return;
}

void warrior_ai (CHAR_DATA *mob, CHAR_DATA *victim)
{
	int health = 0;
	int style, count, dir;
	OBJ_DATA *mobweap, *victweap;
	CHAR_DATA *gch;

	for (style = 1; style < MAX_STYLE; style++) {
		if (IS_SET(mob->pIndexData->styles,style_table[style].bit)
			&& mob->mobstyle != style
			&& number_percent() < 15) {
			mob->mobstyle = style;
			do_emote(mob,"shifts into a new combat stance.");
			return;
		}
	}

	health = victim->hit * 100 / victim->max_hit;
	
	switch(mob->mobstyle) {
		case (3):			/* DUELIST */
			if ((victim->Class()->GetIndex() == CLASS_SORCERER
				|| victim->Class()->GetIndex() == CLASS_NECROMANCER
				|| victim->Class()->GetIndex() == CLASS_HEALER
				|| victim->Class()->GetIndex() == CLASS_ZEALOT
				|| victim->Class()->GetIndex() == CLASS_ANTI_PALADIN)
				&& (mobweap = get_eq_char(mob,WEAR_WIELD)) != NULL
				&& (mobweap->value[0] != WEAPON_AXE
					&& mobweap->value[0] != WEAPON_WHIP
					&& mobweap->value[0] != WEAPON_EXOTIC
					&& mobweap->value[0] != WEAPON_POLEARM)
				&& !is_affected(victim,gsn_concuss))
				do_concuss(mob,"");
			else if(health < 50 && can_bash(mob,victim))
				do_bash(mob,"");
			else if (health < 50 && !IS_AFFECTED(victim,AFF_FLYING))
				do_trip(mob,"");
			else
				do_unbalance(mob,"");
			break;
		case (5):			/* DRAGOON */
			if(health < 50 && can_bash(mob,victim))
				do_bash(mob,"");
			else if (health < 50 && !IS_AFFECTED(victim,AFF_FLYING))
				do_trip(mob,"");
			else if (health < 50 && get_eq_char(mob,WEAR_SHIELD))
				do_shieldbash(mob,"");
			else
				do_exchange(mob,"");
			break;
		case (6):			/* TACTICIAN */
			mobweap = get_eq_char(mob,WEAR_WIELD);
			if(get_eq_char(victim,WEAR_HEAD) == NULL && mobweap
				&& str_cmp(race_table[victim->race].name,"minotaur"))
				do_exploit(mob,"head");
			else if (get_eq_char(victim,WEAR_BODY) == NULL && mobweap)
				do_exploit(mob,"torso");
			else if (get_eq_char(victim,WEAR_ARMS) == NULL && mobweap)
				do_exploit(mob,"arm");
			else if (get_eq_char(victim,WEAR_LEGS) == NULL && mobweap)
				do_exploit(mob,"leg");
			else if(health < 50 && can_bash(mob,victim))
				do_bash(mob,"");
			else if (health < 50 && !IS_AFFECTED(victim,AFF_FLYING))
				do_trip(mob,"");
			else if (!is_affected(mob,gsn_outflank)) {
				for (dir = 0; dir < 6; dir++)
					if (mob->in_room->exit[dir]
						&& mob->in_room->exit[dir]->u1.to_room) {
						do_outflank(mob,dir_name[dir]);
						break;
					}
			} else if (can_see(victim,mob) && IS_SET(mob->off_flags, OFF_KICK_DIRT))
				do_dirt(mob,"");
			else if ((mobweap = get_eq_char(mob,WEAR_WIELD)) != NULL
				&& ((victweap = get_eq_char(victim,WEAR_WIELD)) != NULL)
				&& !IS_OBJ_STAT(victweap,ITEM_NOREMOVE)
				&& !IS_OBJ_STAT(victweap,ITEM_NODISARM))
				do_disarm(mob,"");
			else if (!is_affected(mob,gsn_berserk))
				do_berserk(mob,"");
			else if (!is_affected(mob,gsn_warcry))
				do_warcry(mob,"");
			else
				do_kick(mob,"");
			break;
		case (2):			/* BARBARIAN */
			mobweap = get_eq_char(mob,WEAR_WIELD);
			if ((victim->Class()->GetIndex() == CLASS_SORCERER
				|| victim->Class()->GetIndex() == CLASS_NECROMANCER)
				&& mobweap
				&& mobweap->weight >= 15
				&& (mobweap->value[0] == WEAPON_MACE
					|| mobweap->value[0] == WEAPON_AXE
					|| mobweap->value[0] == WEAPON_FLAIL
					|| mobweap->value[0] == WEAPON_STAFF)
				&& !is_affected(victim,gsn_uppercut))
				do_uppercut(mob,"");
			else if(health < 50 && can_bash(mob,victim))
				do_bash(mob,"");
			else if (health < 50 && !IS_AFFECTED(victim,AFF_FLYING))
				do_trip(mob,"");
			else if (health < 50
				&& mobweap
				&& mobweap->weight >= 15
				&& mobweap->value[0] != WEAPON_WHIP
				&& mobweap->value[0] != WEAPON_POLEARM
				&& mobweap->value[0] != WEAPON_DAGGER
				&& mobweap->value[0] != WEAPON_EXOTIC
				&& mobweap->value[0] != WEAPON_SPEAR)
				do_overhead(mob,"");
			else if (mobweap && mobweap->weight >= 15
				&& (mobweap->value[0] == WEAPON_SPEAR
					|| mobweap->value[0] == WEAPON_POLEARM)
				&& !is_affected(victim,gsn_impale))
				do_impale(mob,"");
			else if (mobweap && mobweap->weight >= 15
				&& mobweap->value[0] != WEAPON_DAGGER
				&& mobweap->value[0] != WEAPON_WHIP
				&& mobweap->value[0] != WEAPON_EXOTIC
				&& get_eq_char(victim,WEAR_WIELD)) {
				if (number_percent() > 50)
					do_shatter(mob,"primary");
				else
					do_shatter(mob,"offhand");
			} else if (can_see(victim,mob) && IS_SET(mob->off_flags, OFF_KICK_DIRT))
				do_dirt(mob,"");
			else if ((mobweap = get_eq_char(mob,WEAR_WIELD)) != NULL
				&& ((victweap = get_eq_char(victim,WEAR_WIELD)) != NULL)
				&& !IS_OBJ_STAT(victweap,ITEM_NOREMOVE)
				&& !IS_OBJ_STAT(victweap,ITEM_NODISARM))
				do_disarm(mob,"");
			else if (!is_affected(mob,gsn_berserk))
				do_berserk(mob,"");
			else if (!is_affected(mob,gsn_warcry))
				do_warcry(mob,"");
			else
				do_kick(mob,"");
			break;
		case (4):			/* SKIRMISHER */
			count = 0;
			for (gch = mob->in_room->people; gch; gch = gch->next_in_room) {
				if (is_same_group(victim,gch))
					count++;
			}
			if ((mobweap = get_eq_char(mob,WEAR_WIELD)) != NULL
				&& mobweap->value[0] == WEAPON_WHIP
				&& !is_affected(mob,gsn_entwine))
				do_entwine(mob,"arm");
			else if (is_affected(mob,gsn_entwine)) {
				if (number_percent() > 50) {
					for (dir = 0; dir < 6; dir++) {
						if (mob->in_room->exit[dir]
							&& mob->in_room->exit[dir]->u1.to_room
							&& !IS_SET(mob->in_room->exit[dir]->exit_info,EX_CLOSED)) {
							do_pull(mob,dir_name[dir]);
							break;
						}
					}
					break;
				} else {
					do_uncoil(mob,"");
				}
			} else if (health < 50 && can_bash(mob,victim))
				do_bash(mob,"");
			else if (health < 50 && !IS_AFFECTED(victim,AFF_FLYING))
				do_trip(mob,"");
			else if (count > 1)
				do_whirlwind(mob,"");
			else if (mobweap && mobweap->weight < 10
				&& (mobweap->value[0] == WEAPON_DAGGER
				|| mobweap->value[0] == WEAPON_AXE
				|| mobweap->value[0] == WEAPON_MACE
				|| mobweap->value[0] == WEAPON_SWORD))
				do_dart(mob,"");
			else if (can_see(victim,mob) && IS_SET(mob->off_flags, OFF_KICK_DIRT))
				do_dirt(mob,"");
			else if ((mobweap = get_eq_char(mob,WEAR_WIELD)) != NULL
				&& ((victweap = get_eq_char(victim,WEAR_WIELD)) != NULL)
				&& !IS_OBJ_STAT(victweap,ITEM_NOREMOVE)
				&& !IS_OBJ_STAT(victweap,ITEM_NODISARM))
				do_disarm(mob,"");
			else if (!is_affected(mob,gsn_berserk))
				do_berserk(mob,"");
			else if (!is_affected(mob,gsn_warcry))
				do_warcry(mob,"");
			else
				do_kick(mob,"");
			break;
		case (1):			/* GLADIATOR */
			mobweap = get_eq_char(mob,WEAR_WIELD);
			if(health < 50 && can_bash(mob,victim))
				do_bash(mob,"");
			else if (health < 50 && !IS_AFFECTED(victim,AFF_FLYING))
				do_trip(mob,"");
			else if (number_percent() > 30) {
				switch (number_range(1,4)) {
					case(1):
						do_bleed(mob,"");
						break;
					case(2):
						do_hobble(mob,"");
						break;
					case(3):
						do_crippling_blow(mob,"");
						break;
					case(4):
						do_gouge(mob,"");
						break;
				}
				break;
			} else if (get_eq_char(victim,WEAR_WIELD)
				&& mobweap
				&& (get_eq_char(mob,WEAR_DUAL_WIELD)
					|| (mobweap->value[0] == WEAPON_STAFF
						|| mobweap->value[0] == WEAPON_POLEARM
						|| mobweap->value[0] == WEAPON_SPEAR)))
				do_entrap(mob,"");
			else if (!is_affected(mob,gsn_berserk))
				do_berserk(mob,"");
			else if (!is_affected(mob,gsn_warcry))
				do_warcry(mob,"");
			else
				do_kick(mob,"");
			break;
		default:
			if(health < 50 && can_bash(mob,victim))
				do_bash(mob,"");
			else if (health < 50 && !IS_AFFECTED(victim,AFF_FLYING))
				do_trip(mob,"");
			else if (can_see(victim,mob) && IS_SET(mob->off_flags, OFF_KICK_DIRT))
				do_dirt(mob,"");
			else if ((mobweap = get_eq_char(mob,WEAR_WIELD)) != NULL
				&& ((victweap = get_eq_char(victim,WEAR_WIELD)) != NULL)
				&& !IS_OBJ_STAT(victweap,ITEM_NOREMOVE)
				&& !IS_OBJ_STAT(victweap,ITEM_NODISARM))
				do_disarm(mob,"");
			else if (!is_affected(mob,gsn_berserk))
				do_berserk(mob,"");
			else if (!is_affected(mob,gsn_warcry))
				do_warcry(mob,"");
			else
				do_kick(mob,"");
			break;
	}

	return;
}

void thief_ai (CHAR_DATA *mob, CHAR_DATA *victim)
{
	OBJ_DATA *mobweap;
	OBJ_DATA *victweap;
	
	if ((((mobweap = get_eq_char(mob,WEAR_WIELD)) != NULL
		&& mobweap->value[0] == WEAPON_DAGGER)
		|| ((mobweap = get_eq_char(mob,WEAR_DUAL_WIELD)) != NULL
		&& mobweap->value[0] == WEAPON_DAGGER))
		&& victim->fighting != mob)
		do_circle_stab(mob,"");
	else if (can_see(victim,mob))
		do_dirt(mob,"");
	else if ((mobweap = get_eq_char(mob,WEAR_WIELD)) != NULL
		&& ((victweap = get_eq_char(victim,WEAR_WIELD)) != NULL))
		do_disarm(mob,"");
	else if (!IS_AFFECTED(victim,AFF_FLYING))
		do_trip(mob,"");
	else
		do_kick(mob,"");

	return;
}
	

bool can_bash (CHAR_DATA *ch, CHAR_DATA *victim)
{
	if (    (IS_NPC(ch) && !IS_SET(ch->off_flags, OFF_BASH))
		|| victim->position < POS_FIGHTING
		|| ch->size + 1 < victim->size
		|| ch->size - 1 > victim->size
		|| is_affected(victim,gsn_protective_shield)
		|| is_affected(victim,gsn_sanguine_ward)
		|| is_affected(victim,gsn_heatshield)
		|| is_affected(victim,gsn_elecshield)
		|| is_affected(victim,gsn_airshield)
		|| is_affected(victim,gsn_earthshield)
		|| is_affected(victim,gsn_watershield)
		|| is_affected(victim,gsn_coldshield))
		return FALSE;

	return TRUE;
}

/*
 * Disarm a creature.
 * Caller must check for successful attack.
 */
void disarm( CHAR_DATA *ch, CHAR_DATA *victim )
{
    OBJ_DATA *obj;
    OBJ_DATA *secondary;


    if ( ( obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL )
        return;

    if ( IS_OBJ_STAT(obj,ITEM_NOREMOVE) || IS_OBJ_STAT(obj,ITEM_NODISARM))
    {
        act("$S weapon won't budge!",ch,NULL,victim,TO_CHAR);
        act("$n tries to disarm you, but your weapon won't budge!",
            ch,NULL,victim,TO_VICT);
        act("$n tries to disarm $N, but fails.",ch,NULL,victim,TO_NOTVICT);
        return;
    }

    act( "$n disarms you and sends your weapon flying!",
         ch, NULL, victim, TO_VICT    );
    act( "You disarm $N!",  ch, NULL, victim, TO_CHAR    );
    act( "$n disarms $N!",  ch, NULL, victim, TO_NOTVICT );

    obj_from_char( obj );
    if ( IS_OBJ_STAT(obj,ITEM_NODROP) || IS_OBJ_STAT(obj,ITEM_INVENTORY) )
        obj_to_char( obj, victim );
    else
    {
		if(check_catch(ch,victim,obj))
			obj_to_char( obj, ch);
		else
		{
	    	obj_to_room( obj, victim->in_room );
			if (IS_NPC(victim) && victim->wait == 0 && can_see_obj(victim,obj))
            	get_obj(victim,obj,NULL, TRUE);
		}
    }

    if ((secondary = get_eq_char(victim,WEAR_DUAL_WIELD)) != NULL)
    {
		unequip_char(victim,secondary,FALSE);
		act("You quickly swap $p into your primary hand.",victim,secondary,0,TO_CHAR);
		act("$n quickly swaps $p into $s primary hand.",victim,secondary,0,TO_ROOM);
		equip_char(victim,secondary,WEAR_WIELD,FALSE);
    }
    return;
}

void do_berserk( CHAR_DATA *ch, char *argument)
{
    float chance;
    int	hp_percent;

    if ((chance = get_skill(ch,gsn_berserk)) == 0
    ||  (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_BERSERK))
    ||  (!IS_NPC(ch)
    &&   ch->level < skill_table[gsn_berserk].skill_level[ch->Class()->GetIndex()]))
    {
        send_to_char("You turn red in the face, but nothing happens.\n\r",ch);
        return;
    }

    if (IS_AFFECTED(ch,AFF_BERSERK) || is_affected(ch,gsn_berserk)
    ||  is_affected(ch,skill_lookup("frenzy")))
    {
        send_to_char("You get a little madder.\n\r",ch);
        return;
    }

    if (IS_AFFECTED(ch,AFF_CALM))
    {
        send_to_char("You're feeling too mellow to berserk.\n\r",ch);
        return;
    }

    if (ch->mana < 50)
    {
        send_to_char("You can't get up enough energy.\n\r",ch);
        return;
    }

    /* modifiers */

    /* fighting */
    if (ch->position == POS_FIGHTING)
        chance += 10;

    /* damage -- below 50% of hp helps, above hurts */
    if (ch->max_hit == 0)
	hp_percent = 0;
     else
    hp_percent = 100 * ch->hit/ch->max_hit;
    chance += 25 - hp_percent/2;

    if (number_percent() < chance)
    {
        AFFECT_DATA af;

        WAIT_STATE(ch,PULSE_VIOLENCE);
        ch->mana -= 50;
        ch->move = (short)(ch->move / 1.2);

        /* heal a little damage */
        ch->hit += ch->level*2;
        ch->hit = UMIN(ch->hit,ch->max_hit);

        send_to_char("Your pulse races as you are consumed by rage!\n\r",ch);
        act("$n gets a wild look in $s eyes.",ch,NULL,NULL,TO_ROOM);
        check_improve(ch,gsn_berserk,TRUE,2);

        init_affect(&af);
        af.where        = TO_AFFECTS;
        af.type         = gsn_berserk;
        af.level        = ch->level;
		af.duration 	= (ch->level / 10);
        af.modifier     = UMAX(1,ch->level/5);
        SET_BIT(af.bitvector, AFF_BERSERK);
		af.aftype		= AFT_SKILL;
        af.location     = APPLY_HITROLL;
        affect_to_char(ch,&af);
        af.location     = APPLY_DAMROLL;
        affect_to_char(ch,&af);

    }

    else
    {
        WAIT_STATE(ch,PULSE_VIOLENCE);
        ch->mana -= 25;

        send_to_char("Your pulse speeds up, but nothing happens.\n\r",ch);
        check_improve(ch,gsn_berserk,FALSE,2);
    }
}

void do_bash( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    float chance;
	int dam, avoid;
    char buf[MAX_STRING_LENGTH];
    AFFECT_DATA af;

    one_argument(argument,arg);

    if ( (chance = get_skill(ch,gsn_bash)) == 0)
        return send_to_char("Bashing? What's that?\n\r",ch);

    if (IS_NPC(ch) && IS_AFFECTED(ch,AFF_CHARM))
		return;

    if (arg[0] == '\0')
    {
        victim = ch->fighting;
        if (victim == NULL)
			return send_to_char("But you aren't fighting anyone!\n\r",ch);
    }

    else if ((victim = get_char_room(ch,arg)) == NULL)
        return send_to_char("They aren't here.\n\r",ch);

    if (check_sidestep(ch,victim,gsn_bash,95))
        return;

    if (victim->position < POS_FIGHTING)
    {
        act("You'll have to let $M get back up first.",ch,NULL,victim,TO_CHAR);
        return;
    }

    if (victim == ch)
        return send_to_char("You try to bash your brains out, but fail.\n\r",ch);

    if (is_safe(ch,victim))
        return;

    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
    {
        act("But $N is your friend!",ch,NULL,victim,TO_CHAR);
        return;
    }

	if (ch->size + 1 < victim->size)
		return send_to_char("They are too large to bash.\n\r",ch);

	if (ch->size - 1 > victim->size)
		return send_to_char("They are too small to properly aim a bash at.\n\r",ch);

	if ((avoid = get_skill(victim, gsn_avoid)) > 0)
	{
		avoid /= 2;

		avoid += 5 * (get_curr_stat(ch,STAT_DEX) - 20);
		
		if(number_percent() < avoid)
		{
			act("You deftly avoid $N's bash and $E topples to the ground.",victim,0,ch,TO_CHAR);
			act("$n deftly avoids $N's bash and $E topples to the ground.",victim,0,ch,TO_NOTVICT);
			act("Your bash is deftly avoided by $n, and you go crashing to the ground.",victim,0,ch,TO_VICT);
			WAIT_STATE(ch,3 * PULSE_VIOLENCE);
			return;
		}
	}

    if (is_affected(victim,gsn_protective_shield))
    {
		act("$N's bash seems to slide around you.",victim,0,ch,TO_CHAR);
		act("$N's bash seems to slide around $n.",victim,0,ch,TO_NOTVICT);
		act("Your bash seems to slide around $n.",victim,0,ch,TO_VICT);
		WAIT_STATE(ch,(int)(PULSE_VIOLENCE*.5));
		return;
    }

    if (is_affected(victim,gsn_sanguine_ward))
    {
	act("$N tries to bash you but your sanguine ward keeps $M at bay.",victim,0,ch,TO_CHAR);
	act("$n's ward flares brightly as $N tries to bash $m, deflecting the attack!",victim,0,ch,TO_NOTVICT);
	act("$n's ward flares brightly as you try to bash $m, sending you flying back!",victim,0,ch,TO_VICT);
	WAIT_STATE(ch,PULSE_VIOLENCE*1);
	return;
    }
    if (is_affected(victim,gsn_heatshield))
    {
	act("$N tries to bash you but is stopped by a wave of searing heat.",victim,0,ch,TO_CHAR);
	act("$N tries to bash $n but a wave of searing heat stops him.",victim,0,ch,TO_NOTVICT);
	act("You try to bash $n but a wave of searing heat stops you!",victim,0,ch,TO_VICT);
	WAIT_STATE(ch,PULSE_VIOLENCE*1);
        damage_old(victim, ch, dice(8,10),gsn_heatshield, DAM_FIRE,TRUE);
	return;
    }
    if (is_affected(victim,gsn_elecshield))
    {
	act("$N attempts to bash you, but collides with your electric shield.",victim,0,ch,TO_CHAR);
	act("$N attempts to bash $n, but collides with $s electric shield.",victim,0,ch,TO_NOTVICT);
	act("You are stopped by an electric field, shocking you and sapping your energy!",victim,0,ch,TO_VICT);
	WAIT_STATE(ch,PULSE_VIOLENCE*1);
        damage_old(victim, ch, dice(2,10),gsn_elecshield, DAM_LIGHTNING,TRUE);
	ch->hit = UMAX(ch->hit - dice(5,10),0);
	return;
    }
    if (is_affected(victim,gsn_airshield))
    {
	act("$N attempts to bash you, but a curtain of swirling winds throws $M back!",victim,0,ch,TO_CHAR);
	act("$N attempts to bash $n, but a curtain of swirling winds throws $M back!",victim,0,ch,TO_NOTVICT);
	act("You try to bash $n but a curtain of swirling winds throw you back!",victim,0,ch,TO_VICT);
	dam = dice(2,10);
        damage_old(victim, ch, dam,gsn_airshield, DAM_BASH,TRUE);
	WAIT_STATE(ch,PULSE_VIOLENCE*3);
	return;
    }
    if (is_affected(victim,gsn_earthshield))
    {
        act("$N attempts to bash you, but collides with your earthshield.",victim,0,ch,TO_CHAR);
        act("$N attempts to bash $n, but instead collides with a solid barrier.",victim,0,ch,TO_NOTVICT);
        act("Your hear your bones crack as you slam into a rock-hard shield around $n.",victim,0,ch,TO_VICT);
        damage_old(victim, ch, dice(2,10),gsn_earthshield, DAM_BASH,TRUE);
	init_affect(&af);
	af.where     = TO_AFFECTS;
	af.aftype     = AFT_MALADY;
	af.type      = gsn_shattered_bone;
	af.name      = palloc_string("broken bones");
	af.level     = ch->level;
	af.duration  = ch->level/5;
	af.location  = APPLY_STR;
	af.modifier  = -1;
	affect_to_char(ch,&af);
	af.location  = APPLY_DEX;
	af.modifier  = -1;
	affect_to_char(ch,&af);
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
        return;
    }
    if (is_affected(victim,gsn_watershield))
    {
        act("$N attempts to bash you, and you feel slightly invigorated as $E crashes into your watershield.",victim,0,ch,TO_CHAR);
        act("$n looks slightly invigorated as $N crashes into $s watershield.",victim,0,ch,TO_NOTVICT);
        act("You feel drained as your mass collides with $n's shield of swirling water.",victim,0,ch,TO_VICT);
        dam = dice(3,10);
        damage_old(victim, ch, dam,gsn_watershield, DAM_DROWNING,TRUE);
        victim->hit = UMIN(victim->hit + dam,victim->max_hit);
        if (!IS_NPC(ch)) {
                ch->pcdata->condition[COND_THIRST] += 8;
                ch->pcdata->condition[COND_THIRST] = UMIN(ch->pcdata->condition[COND_THIRST],COND_HUNGRY);
        }
        return;
    }
    if (is_affected(victim,gsn_coldshield))
    {
        act("$N attempts to bash you, but is stopped by a wave of freezing air.",victim,0,ch,TO_CHAR);
        act("$N attempts to bash $n, but is stopped by a wave of freezing air.",victim,0,ch,TO_NOTVICT);
        act("You attempt to bash $n, but are seared by a wave of freezing air.",victim,0,ch,TO_VICT);
    (*skill_table[gsn_chill].spell_fun) ( gsn_chill, ch->level, victim,ch,TAR_CHAR_OFFENSIVE);
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
        return;
    }
    /* modifiers */

    /* size  and weight */
    chance += ch->carry_weight / 250;
    chance -= victim->carry_weight / 200;

    if (ch->size < victim->size)
        chance += (ch->size - victim->size) * 20;
    else
        chance += (ch->size - victim->size) * 10;


    /* stats */
    chance += get_curr_stat(ch,STAT_STR);
    chance -= (get_curr_stat(victim,STAT_DEX) * 4)/3;
    chance -= GET_AC(victim,AC_BASH) /25;
    /* speed */
    if (IS_SET(ch->off_flags,OFF_FAST) || is_affected(ch,skill_lookup("haste")))
        chance += 10;
    if (IS_SET(victim->off_flags,OFF_FAST) || is_affected(victim,skill_lookup("haste")))
        chance -= 40;

    /* level */
    chance += (ch->level - victim->level);
	if (!IS_NPC(ch) && !IS_NPC(victim)
	&& (victim->fighting == NULL || ch->fighting == NULL))
	{
	sprintf(buf,"Help! %s is bashing me!",PERS(ch,victim));
	do_myell(victim,buf,ch);
	}


    if (!IS_NPC(victim)
        && chance < get_skill(victim,gsn_dodge) )
        chance -= 3 * (get_skill(victim,gsn_dodge) - chance);
	
	chance = UMIN(chance, 75);
    /* now the attack */
    if (number_percent() < chance )
    {
        act("$n sends you sprawling with a powerful bash!",ch,NULL,victim,TO_VICT);
        act("You slam into $N, and send $M flying!",ch,NULL,victim,TO_CHAR);
        act("$n sends $N sprawling with a powerful bash.",ch,NULL,victim,TO_NOTVICT);
        check_improve(ch,gsn_bash,TRUE,1);
        damage_old(ch,victim,number_range(2,(int)(2 + 2 * ch->size + chance/20)),gsn_bash,DAM_BASH,TRUE);
		LAG_CHAR(victim,UMIN(2,number_range(1,2) + ch->size - victim->size) * PULSE_VIOLENCE);
		WAIT_STATE(ch, PULSE_VIOLENCE *2);
        victim->position = POS_RESTING;
		victim->disrupted = TRUE;
	if (!IS_NPC(ch) && (ch->Class()->GetIndex() == CLASS_WARRIOR)
	&& (number_percent() < ch->level))
		ch->pcdata->sp++;
    }
    else
    {
        damage_old(ch,victim,0,gsn_bash,DAM_BASH,FALSE);
        act("You fall flat on your face!",ch,NULL,victim,TO_CHAR);
        act("$n falls flat on $s face.",ch,NULL,victim,TO_NOTVICT);
        act("You evade $n's bash, causing $m to fall flat on $s face.",ch,NULL,victim,TO_VICT);
        check_improve(ch,gsn_bash,FALSE,1);

        ch->position = POS_RESTING;
        WAIT_STATE(ch,3 * PULSE_VIOLENCE);
    }
}

void do_dirt( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    float chance;
    char buf[MAX_STRING_LENGTH];

    one_argument(argument,arg);

    if ( (chance = get_skill(ch,gsn_dirt)) == 0)
        return send_to_char("You get your feet dirty.\n\r",ch);

    if (arg[0] == '\0')
    {
        victim = ch->fighting;
        if (victim == NULL)
        {
            send_to_char("But you aren't in combat!\n\r",ch);
            return;
        }
    }

    else if ((victim = get_char_room(ch,arg)) == NULL)
    {
        send_to_char("They aren't here.\n\r",ch);
        return;
    }

    if (IS_AFFECTED(victim,AFF_BLIND))
    {
        act("$E's already been blinded.",ch,NULL,victim,TO_CHAR);
        return;
    }

    if(victim->position==POS_SLEEPING) {
	send_to_char("Their eyes are closed.\n\r",ch);
	return;
    }

    if (victim == ch)
    {
        send_to_char("Very funny.\n\r",ch);
        return;
    }

    if (is_safe(ch,victim))
        return;

    if (check_sidestep(ch,victim,gsn_dirt,95))
	return;

    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
    {
        act("But $N is such a good friend!",ch,NULL,victim,TO_CHAR);
        return;
    }

    /* modifiers */

    /* dexterity */
    chance += get_curr_stat(ch,STAT_DEX);
    chance -= 2 * get_curr_stat(victim,STAT_DEX);

    /* speed  */
    if (IS_SET(ch->off_flags,OFF_FAST) || is_affected(ch,skill_lookup("haste")))
        chance += 10;
    if (IS_SET(victim->off_flags,OFF_FAST) || is_affected(victim,skill_lookup("haste")))
        chance -= 30;

    /* level */
    chance += (ch->level - victim->level) * 2;

    /* sloppy hack to prevent false zeroes */
    if ((int)chance % 5 == 0)
        chance += 1;

    /* terrain */

    switch(ch->in_room->sector_type)
    {
        case(SECT_INSIDE):              chance -= 20;   break;
        case(SECT_CITY):                chance -= 10;   break;
        case(SECT_FIELD):               chance +=  5;   break;
        case(SECT_FOREST):                              break;
        case(SECT_HILLS):                               break;
        case(SECT_MOUNTAIN):            chance -= 10;   break;
        case(SECT_WATER):          	chance  =  0;   break;
        case(SECT_UNDERWATER):        	chance  =  0;   break;
        case(SECT_AIR):                 chance  =  0;   break;
        case(SECT_DESERT):              chance += 10;   break;
    }

    if (chance == 0)
    {
        send_to_char("There isn't any dirt to kick.\n\r",ch);
        return;
    }

	if (IS_SET(victim->act,ACT_UNDEAD) || IS_SET(victim->form,FORM_UNDEAD) ) {
		chance = 0;
	}
	
    /* now the attack */
    if (number_percent() < chance)
    {
        AFFECT_DATA af;
        act("$n is blinded by the dirt in $s eyes!",victim,NULL,NULL,TO_ROOM);
        act("$n kicks dirt in your eyes!",ch,NULL,victim,TO_VICT);
	if (!IS_NPC(ch) && !IS_NPC(victim) &&
	( (victim->fighting == NULL) || (ch->fighting == NULL)))
		do_myell(victim,"Help! Someone just kicked dirt in my eyes!", ch);

	damage(ch,victim,number_range(2,5),gsn_dirt,DAM_NONE,TRUE);
        send_to_char("You can't see a thing!\n\r",victim);
        check_improve(ch,gsn_dirt,TRUE,2);
        WAIT_STATE(ch,skill_table[gsn_dirt].beats);

        init_affect(&af);
        af.where        = TO_AFFECTS;
        af.type         = gsn_dirt;
		af.aftype = AFT_MALADY;
        af.level        = ch->level;
        af.duration     = 0;
        af.location     = APPLY_HITROLL;
        af.modifier     = -4;
        SET_BIT(af.bitvector, AFF_BLIND);
	af.mod_name	= MOD_VISION;
        affect_to_char(victim,&af);
    }
    else
    {
/* PK yells....add these to most attacks that can initiate PKs.
-Ceran
*/
	if (!IS_NPC(ch) && !IS_NPC(victim)
	&& (victim->fighting == NULL || ch->fighting == NULL))
	{
	switch(number_range(0,1))
		{
		case (0):
		sprintf(buf,"Help! %s just tried to kick dirt in my eyes!",PERS(ch,victim));
		break;
		case (1):
		sprintf(buf,"Die, %s you dirty fool!",PERS(ch,victim));
		break;
		}
	do_myell(victim,buf,ch);
	}
        damage(ch,victim,0,gsn_dirt,DAM_NONE,TRUE);

        check_improve(ch,gsn_dirt,FALSE,2);
        WAIT_STATE(ch,skill_table[gsn_dirt].beats);
    }
}

void do_trip( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    float chance;
    char buf[MAX_STRING_LENGTH];

    one_argument(argument,arg);


    if ( (chance = get_skill(ch,gsn_trip)) == 0
    	|| (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_TRIP))
    	||   (!IS_NPC(ch)
        && ch->level < skill_table[gsn_trip].skill_level[ch->Class()->GetIndex()]))
    {
        send_to_char("Tripping?  What's that?\n\r",ch);
        return;
    }


    if (arg[0] == '\0')
    {
        victim = ch->fighting;
        if (victim == NULL)
        {
            send_to_char("But you aren't fighting anyone!\n\r",ch);
            return;
        }
    }

    else if ((victim = get_char_room(ch,arg)) == NULL)
    {
        send_to_char("They aren't here.\n\r",ch);
        return;
    }

    if (check_sidestep(ch,victim,gsn_trip,95))
	return;

    if (is_safe(ch,victim))
        return;

    if (IS_AFFECTED(victim,AFF_FLYING)
	&& !is_affected(victim,gsn_earthbind))
    {
        act("$S feet aren't on the ground.",ch,NULL,victim,TO_CHAR);
        return;
    }

    if (victim->position < POS_FIGHTING)
    {
        act("$N is already down.",ch,NULL,victim,TO_CHAR);
        return;
    }

    if (victim == ch)
    {
        send_to_char("You fall flat on your face!\n\r",ch);
        WAIT_STATE(ch,2 * skill_table[gsn_trip].beats);
        act("$n trips over $s own feet!",ch,NULL,NULL,TO_ROOM);
        return;
    }

    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
    {
        act("$N is your beloved master.",ch,NULL,victim,TO_CHAR);
        return;
    }

    /* modifiers */

    /* size */
    if (ch->size < victim->size)
        chance += (ch->size - victim->size) * 10;  /* bigger = harder to trip */

    /* dex */
    chance += get_curr_stat(ch,STAT_DEX);
    chance -= get_curr_stat(victim,STAT_DEX) * 3 / 2;

    /* speed */
    if (IS_SET(ch->off_flags,OFF_FAST) || is_affected(ch,skill_lookup("haste")))
        chance += 10;
    if (IS_SET(victim->off_flags,OFF_FAST) || is_affected(victim,skill_lookup("haste")))
        chance -= 40;
    if(is_affected(victim,gsn_cloak_form))
	chance+=10;

    /* level */
    chance += (ch->level - victim->level) * 2;


    /* now the attack */
    if (number_percent() < chance)
    {
		if (!IS_NPC(ch) 
			&& !IS_NPC(victim)
			&& (ch->fighting == NULL 
			|| victim->fighting == NULL))
		{
			sprintf(buf,"Help! %s just tripped me!",PERS(ch,victim));
			do_myell(victim,buf,ch);
		}

		if(is_affected(victim, gsn_stability))
		{
			act("$n tries to trip you, but you do not go down!",ch,NULL,victim,TO_VICT);
			act("You trip $N and $N doesn't go down!",ch,NULL,victim,TO_CHAR);
			act("$n trips $N, but $E doesn't go down.",ch,NULL,victim,TO_NOTVICT);
		}
		else
		{
			act("$n trips you and you go down!",ch,NULL,victim,TO_VICT);
			act("You trip $N and $E goes down!",ch,NULL,victim,TO_CHAR);
			act("$n trips $N, sending $M to the ground.",ch,NULL,victim,TO_NOTVICT);
			LAG_CHAR(victim,2 * PULSE_VIOLENCE);
			victim->position = POS_RESTING;
			victim->disrupted = TRUE;
		}

		check_improve(ch,gsn_trip,TRUE,1); 
		WAIT_STATE(ch,skill_table[gsn_trip].beats);
        damage(ch,victim,number_range(2, 2 +  2 * victim->size),gsn_trip, DAM_BASH,TRUE);
    }
    else
    {
		if (!IS_NPC(ch) && !IS_NPC(victim)
			&& (ch->fighting == NULL || victim->fighting == NULL))
		{
			sprintf(buf,"Help! %s just tried to trip me!",PERS(ch,victim));
			do_myell(victim,buf,ch);
		}
        damage(ch,victim,0,gsn_trip,DAM_BASH,TRUE);
		if (!is_affected(ch,gsn_stability))
		{
        	WAIT_STATE(ch,skill_table[gsn_trip].beats*2/3);
		}
        check_improve(ch,gsn_trip,FALSE,1);
    }
}

void do_hit( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( ch->position != POS_FIGHTING )
    {
	send_to_char( "You're not fighting anyone.\n\r", ch );
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Switch targets to whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }

    if ( victim == ch )
    {
        send_to_char( "Are you crazy?\n\r", ch );
        return;
    }

    if ( victim->fighting != ch )
    {
	send_to_char( "That person isn't fighting you.\n\r", ch );
	return;
    }

	if (victim == ch->fighting) {
		send_to_char( "You're trying as hard as you can!\n\r",ch);
		return;
	}
	
    ch->fighting = victim;
    ch->batter = 0;
	return;
}

void do_kill( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
        send_to_char( "Kill whom?\n\r", ch );
        return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }
/*  Allow player killing   */
    if ( !IS_NPC(victim) )
    {
        send_to_char( "You must MURDER a player.\n\r", ch );
        return;
    }
    if ( victim == ch )
    {
        send_to_char( "You hit yourself.  Ouch!\n\r", ch );
        multi_hit( ch, ch, TYPE_UNDEFINED );
        return;
    }

    if ( is_safe( ch, victim ) )
        return;

    if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
    {
        act( "$N is your beloved master.", ch, NULL, victim, TO_CHAR );
        return;
    }

    if ( ch->position == POS_FIGHTING )
    {
        send_to_char( "You do the best you can!\n\r", ch );
        return;
    }

    if (check_shroud_of_light(ch,victim)) {return;}

    if (check_sidestep(ch,victim,0,50))
        return;

    WAIT_STATE( ch, 1 * PULSE_VIOLENCE );
    multi_hit( ch, victim, TYPE_UNDEFINED );
    return;
}



void do_murde( CHAR_DATA *ch, char *argument )
{
    send_to_char( "If you want to MURDER, spell it out.\n\r", ch );
    return;
}



void do_murder( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    AFFECT_DATA af;
    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
        send_to_char( "Murder whom?\n\r", ch );
        return;
    }

    if ((IS_AFFECTED(ch,AFF_CHARM) || (IS_NPC(ch) && IS_SET(ch->act,ACT_PET))))
	{
//        return;
	}
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }

    if ( victim == ch )
    {
        send_to_char( "Suicide is a mortal sin.\n\r", ch );
        return;
    }

    if ( is_safe( ch, victim ) )
        return;

    if (check_shroud_of_light(ch,victim)) {return;}

    if (check_sidestep(ch,victim,0,50)) {return;}

    if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
    {
        act( "$N is your beloved master.", ch, NULL, victim, TO_CHAR );
        return;
    }

    if ( ch->position == POS_FIGHTING )
    {
        send_to_char( "You do the best you can!\n\r", ch );
        return;
    }
    if (is_affected(ch, gsn_rage) && !IS_NPC(ch) && ch->pcdata->tribe == TRIBE_FOX)
    {
	act("Charging towards $N, with the cunning of the Fox, you circle around and strike at $S exposed flank!",ch,0,victim,TO_CHAR);
	act("Charging towards you, $n abruptly circles around and strikes at your exposed flank!",ch,0,victim,TO_VICT);
	act("Charging towards $N, $n abruptly circles around and strikes at $S exposed flank!",ch,0,victim,TO_NOTVICT);
    if(number_percent()>25 && get_curr_stat(victim,STAT_STR)>13 && get_curr_stat(victim,STAT_DEX)>9)
    {
	act("A chilling tearing sound heralds the destruction of flesh and bone.",ch,0,0,TO_ALL);
	init_affect(&af);
	af.where	= TO_AFFECTS;
	af.type		= skill_lookup("torn muscles");
	af.aftype	= AFT_MALADY;
	af.duration	= ch->level / 20;
	af.level	= ch->level;
	af.location	= APPLY_STR;
	af.modifier	= -2;
	affect_to_char(victim, &af);
	af.location	= APPLY_DEX;
	affect_to_char(victim, &af);
    }
    }
    WAIT_STATE( ch, 1 * PULSE_VIOLENCE );
    un_blade_barrier(ch,NULL);
    if (IS_NPC(victim) && IS_AFFECTED(victim,AFF_CHARM))
        sprintf(buf, "Help!  I am being attacked by %s!",ch->short_descr);
	else if (IS_NPC(ch) && !IS_NPC(victim))
		sprintf(buf, "Help!  I am being attacked by %s!",PERS(ch,victim));
    else if (!IS_NPC(victim))
    {
        switch(number_range(1,4))
        {
	default:
            sprintf(buf, "Help!  I am being attacked by %s!",PERS(ch,victim));
            break;
        case (1):
            sprintf(buf, "Help!  I am being attacked by %s!",PERS(ch,victim));
            break;
        case (2):
            sprintf(buf, "Die, %s, you treacherous fool!", PERS(ch,victim));
            break;
        case (3):
            sprintf(buf, "Help!  %s is attacking me!", PERS(ch,victim));
            break;
	case (4):
            sprintf(buf, "Die, %s, you foul dog!", PERS(ch,victim));
            break;
        }
    }

/* Added a myell hack of yell so Enforcer cabal members can see if a PK
yell is faked or not (see myell in act_comm.c for this).
-Ceran
*/
    if(!IS_NPC(victim))
    	do_myell( victim, buf ,ch);
    check_blade_barrier(ch,victim);
    one_hit( ch, victim, TYPE_UNDEFINED );
    if(get_skill(ch,gsn_third_attack) && number_percent()>50)
	one_hit(ch,victim,TYPE_UNDEFINED);
    return;
}


void do_ambush( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];
    float chance;


    one_argument( argument, arg);


    if (arg[0] == '\0')
    {
	send_to_char("Ambush whom?\n\r",ch);
	return;
    }

    if (ch->fighting != NULL)
    {
	send_to_char("But you're still fighting!\n\r",ch);
	return;
    }
    else if ((victim = get_char_room(ch,arg)) == NULL)
    {
	send_to_char("They aren't here.\n\r",ch);
	return;
    }
    else if (!IS_AFFECTED(ch, AFF_CAMOUFLAGE) || can_see(victim, ch) )
    {
	send_to_char("But they can still see you.\n\r",ch);
	return;
    }

    if ( victim == ch )
    {
	send_to_char("That's just not possible.\n\r",ch);
	return;
    }
    if ( is_safe( ch, victim ) )
      return;

    if (check_shroud_of_light(ch,victim)) {return;}

    if (check_sidestep(ch,victim,gsn_ambush,95))
        return;

    chance = get_skill(ch,gsn_moving_ambush);
    if (victim->fighting != NULL
	&& ( chance < 3
	|| ch->level < skill_table[gsn_moving_ambush].skill_level[ch->Class()->GetIndex()]))
    {
	send_to_char("They are moving around too much to ambush.\n\r",ch);
	return;
    }

    if (victim->fighting != NULL
	&& number_percent() >= chance)
	{
	send_to_char("You can't quite pin them down for your ambush.\n\r",ch);
	return;
	}

	if (!IS_NPC(ch) && !IS_NPC(victim) && victim->fighting == NULL)
	{
	sprintf(buf,"Help! I've been ambushed by %s!",PERS(ch,victim));
	do_myell(victim,buf,ch);
	}

   WAIT_STATE( ch, skill_table[gsn_ambush].beats );
   if ( (number_percent( ) < get_skill(ch, gsn_ambush) )
   || ( get_skill(ch,gsn_ambush) >= 2 && !IS_AWAKE(victim) ) )
    {
	check_improve(ch,gsn_ambush, TRUE,1);
	multi_hit( ch, victim, gsn_ambush );
    }
    else
    {
	check_improve(ch,gsn_ambush, FALSE,1);
	damage( ch, victim, 0, gsn_ambush, DAM_NONE, TRUE);
    }

    return;
}

void do_rescue( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *fch;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
        send_to_char( "Rescue whom?\n\r", ch );
        return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }

    if ( victim == ch )
    {
        send_to_char( "What about fleeing instead?\n\r", ch );
        return;
    }

    if ( !IS_NPC(ch) && IS_NPC(victim) )
    {
        send_to_char( "Doesn't need your help!\n\r", ch );
        return;
    }
    if ( ( fch = victim->fighting ) == NULL )
    {
        send_to_char( "That person is not fighting right now.\n\r", ch );
        return;
    }


    if (is_safe(ch,victim->fighting))
	return;

    WAIT_STATE( ch, skill_table[gsn_rescue].beats );
    if ( number_percent( ) > get_skill(ch,gsn_rescue))
    {
        send_to_char( "You fail the rescue.\n\r", ch );
        check_improve(ch,gsn_rescue,FALSE,1);
        return;
    }

    LAG_CHAR(victim,PULSE_VIOLENCE);
    act( "You rescue $N!",  ch, NULL, victim, TO_CHAR    );
    act( "$n rescues you!", ch, NULL, victim, TO_VICT    );
    act( "$n rescues $N!",  ch, NULL, victim, TO_NOTVICT );
    check_improve(ch,gsn_rescue,TRUE,1);

    stop_fighting( fch, FALSE );
    stop_fighting( victim, FALSE );

    set_fighting( ch, fch );
    set_fighting( fch, ch );
    return;
}



void do_kick( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    int dam, skill;

    if ((skill = get_skill(ch, gsn_kick)) == 0)
		return send_to_char("You better leave the martial arts to fighters.\n\r", ch );

    if ( ( victim = ch->fighting ) == NULL )
    {
        send_to_char( "You aren't fighting anyone.\n\r", ch );
        return;
    }

    WAIT_STATE( ch, skill_table[gsn_kick].beats );
    if ( get_skill(ch,gsn_kick) > number_percent())
    {
	dam = ch->level/2;
	dam += number_range(0,ch->level/6);
	dam += number_range(0,ch->level/6);
	dam += number_range(0,ch->level/6);
	dam += number_range(0,ch->level/6);
	dam += number_range(0,ch->level/6);
	dam += number_range(0,ch->level/6);
	dam += number_range(ch->level/5,ch->level/4);

	if (!IS_NPC(ch))
	{
	if (ch->Class()->name == "shapeshifter")
	if (ch->Class()->name == "necromancer")
	if (ch->Class()->name == "sorcerer")
		{
		dam *= 3;
		dam /= 4;
		}
	}
        damage(ch,victim,dam, gsn_kick,DAM_BASH,TRUE);
        check_improve(ch,gsn_kick,TRUE,1);
    }
    else
    {
        damage( ch, victim, 0, gsn_kick,DAM_BASH,TRUE);
        check_improve(ch,gsn_kick,FALSE,1);
    }
    return;
}




void do_disarm( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    int hth,ch_weapon,vict_weapon,ch_vict_weapon;
	float chance;

    hth = 0;

    if ((chance = get_skill(ch,gsn_disarm)) == 0)
    {
        send_to_char( "You don't know how to disarm opponents.\n\r", ch );
        return;
    }

    if ( get_eq_char( ch, WEAR_WIELD ) == NULL
    &&   ((hth = get_skill(ch,gsn_hand_to_hand)) == 0
    ||    (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_DISARM) && ch->pIndexData->vnum!=MOB_VNUM_TEMPLAR)))
    {
        send_to_char( "You must wield a weapon to disarm.\n\r", ch );
        return;
    }

    if ( ( victim = ch->fighting ) == NULL )
    {
        send_to_char( "You aren't fighting anyone.\n\r", ch );
        return;
    }

    if ( ( obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL )
    {
        send_to_char( "Your opponent is not wielding a weapon.\n\r", ch );
        return;
    }

	if (obj && !can_see_obj(ch,obj)) {
		send_to_char("You can't see their weapon!\n\r",ch);
		return;
	}
	
	if (is_affected(victim,gsn_unbreakable) ||
		(!IS_NPC(victim) && victim->pcdata->greaterdata[GREATER_GERYON] == GERYON_EYE))
	{
		act("You try to disarm $N, but $s hands stick to his weapon.",
			ch,NULL,victim,TO_CHAR);
		act("$n tries to disarm you, but your hands stick to your weapon.",
			ch,NULL,victim,TO_VICT);
		act("$n tries to disarm $N, but their hands stick to their weapon.",
			ch,NULL,victim,TO_NOTVICT);
		return;
	}

    /* find weapon skills */
    ch_weapon = get_weapon_skill(ch,get_weapon_sn(ch));
    vict_weapon = get_weapon_skill(victim,get_weapon_sn(victim));
    ch_vict_weapon = get_weapon_skill(ch,get_weapon_sn(victim));

    /* modifiers */
    chance *= 8;
    chance /=10;

    /* skill */
    if ( get_eq_char(ch,WEAR_WIELD) == NULL)
        chance = chance * hth/150;
    else
        chance = chance * ch_weapon/100;

    chance += (ch_vict_weapon/2 - vict_weapon) / 2;

    /* dex vs. strength */
    chance += get_curr_stat(ch,STAT_DEX);
    chance -= 1.25 * get_curr_stat(victim,STAT_STR);

    /* level */
    chance += (ch->level - victim->level) * 2;

    /* and now the attack */
    if (number_percent() < chance)
    {
        WAIT_STATE( ch, skill_table[gsn_disarm].beats );
        disarm( ch, victim );
        check_improve(ch,gsn_disarm,TRUE,1);
    }
    else
    {
        WAIT_STATE(ch,skill_table[gsn_disarm].beats);
        act("You fail to disarm $N.",ch,NULL,victim,TO_CHAR);
        act("$n tries to disarm you, but fails.",ch,NULL,victim,TO_VICT);
        act("$n tries to disarm $N, but fails.",ch,NULL,victim,TO_NOTVICT);
        check_improve(ch,gsn_disarm,FALSE,1);
    }
    return;
}

void do_surrender( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *mob;
    if ( (mob = ch->fighting) == NULL )
    {
        send_to_char( "But you're not fighting!\n\r", ch );
        return;
    }
    act( "You surrender to $N!", ch, NULL, mob, TO_CHAR );
    act( "$n surrenders to you!", ch, NULL, mob, TO_VICT );
    act( "$n tries to surrender to $N!", ch, NULL, mob, TO_NOTVICT );
    stop_fighting( ch, TRUE );

}


void do_sla( CHAR_DATA *ch, char *argument )
{
    send_to_char( "If you want to SLAY, spell it out.\n\r", ch );
    return;
}



void do_slay( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
        send_to_char( "Slay whom?\n\r", ch );
        return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }

    if ( ch == victim )
    {
        send_to_char( "Suicide is a mortal sin.\n\r", ch );
        return;
    }

    if ( !IS_NPC(victim) && victim->level >= get_trust(ch))
    {
        send_to_char( "You failed.\n\r", ch );
        return;
    }

    act( "You point at $M and $S heart stops!",  ch, NULL, victim, TO_CHAR    );
    act( "$n points at you and your heart suddenly stops!", ch, NULL, victim, TO_VICT    );
    act( "$n points at $N and $E falls over clenching $s chest!",  ch, NULL, victim, TO_NOTVICT );

    raw_kill(ch, victim );
    return;
}

void spell_power_word_kill(int sn,int level,CHAR_DATA *ch,void *vo, int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam,saves,modify;

    saves = 0;

    act("$n points a finger at $N and utters the word, 'Die'.",ch,NULL,victim,TO_NOTVICT);
    act("$n points a finger at you and utters the word, 'Die'.",ch,NULL,victim,TO_VICT);
    send_to_char("You intone a word of unholy power.\n\r",ch);

    for ( modify = 0; modify < 5; modify++)
    {
	if (saves_spell(level-1,victim,DAM_NEGATIVE) )
		saves++;
    }

    if(IS_IMMORTAL(ch) && (victim->level<ch->level)) { saves=0; }
    if (saves == 0)
    {
	act("$N shudders in shock as $S heart explodes!", ch, NULL,victim,TO_NOTVICT);
	send_to_char("You feel your heart rupture in a violent explosion of pain!\n\r",victim);
	act("Your word of power vaporises $N's heart, killing $M instantly!",ch,NULL,victim,TO_CHAR);
	raw_kill(ch,victim);
    	return;
    }
    dam = dice(level,14);
    dam /= saves;
    damage_old(ch,victim,dam,sn,DAM_NEGATIVE,TRUE);

	return;
}

void do_herb(CHAR_DATA *ch,char *argument)
{
        CHAR_DATA *victim;
        char arg[MAX_INPUT_LENGTH];
        AFFECT_DATA af;

        one_argument(argument,arg);

        if ( (get_skill(ch,gsn_herb) == 0)
        || (ch->level < skill_table[gsn_herb].skill_level[ch->Class()->GetIndex()]) )
        {
        send_to_char("Huh?\n\r",ch);
        return;
        }
        if (is_affected(ch,gsn_herb))
        {
        send_to_char("You can't find any more herbs.\n\r",ch);
        return;
        }
        if ((ch->in_room->sector_type != SECT_FOREST)
        && (ch->in_room->sector_type != SECT_HILLS)
        && (ch->in_room->sector_type != SECT_MOUNTAIN) )
        {
        send_to_char("You can't find any herbs here.\n\r",ch);
        return;
        }

	if (arg[0] == '\0')
		victim = ch;
       else   if ((victim = get_char_room(ch,arg)) == NULL)
        {
        send_to_char("They aren't here.\n\r",ch);
        return;
        }

        if (number_percent() > get_skill(ch,gsn_herb))
        {
        send_to_char("You search for herbs but fail to find any.\n\r",ch);
        act("$n looks about in the bushes but finds nothing.",ch,0,0,TO_ROOM);
        check_improve(ch,gsn_herb,FALSE,4);
        return;
        }

        if (victim != ch)
        {
        act("$n applies herbs to $N.",ch,0,victim,TO_NOTVICT);
        act("You apply herbs to $N.",ch,0,victim,TO_CHAR);
        act("$n applies herbs to you.",ch,0,victim,TO_VICT);
        }

	if (victim == ch)
	{
        act("$n applies herbs to $mself.",ch,0,0,TO_ROOM);
        send_to_char("You find herbs and apply them to yourself.\n\r",ch);
        }

	send_to_char("You feel better.\n\r",victim);

	if (IS_AFFECTED(victim,AFF_PLAGUE) && number_percent() > 30)
	{
	affect_strip(victim,gsn_plague);
	act("The sores on $n's body vanish.\n\r",victim,0,0,TO_ROOM);
	send_to_char("The sores on your body vanish.\n\r",victim);
	}

        check_improve(ch,gsn_herb,TRUE,4);

        victim->hit = UMIN(victim->hit + (4*ch->level), victim->max_hit);
        init_affect(&af);
        af.where = TO_AFFECTS;
        af.type = gsn_herb;
        af.location = 0;
        af.duration = 4;
        af.modifier = 0;
	af.aftype = AFT_SKILL;
        af.level = ch->level;
        affect_to_char(ch,&af);
        return;
}

void do_cleave(CHAR_DATA *ch,char *argument)
{
	OBJ_DATA *weapon;
        CHAR_DATA *victim;
        char arg[MAX_INPUT_LENGTH];
        int dam, chance, dam_type;
        char buf[MAX_STRING_LENGTH];
        int sn;
	int skill;

        if ( ( ( chance = get_skill(ch,gsn_cleave)) == 0)
        || (ch->level < skill_table[gsn_cleave].skill_level[ch->Class()->GetIndex()]) )
        {
        send_to_char("You don't know how to cleave.\n\r",ch);
        return;
        }

        one_argument(argument,arg);
        weapon = get_eq_char(ch,WEAR_WIELD);

    	if (weapon == NULL)
	{
        send_to_char("You need to wield a sword or axe to cleave.\n\r",ch);
        return;
        }

        if ((weapon->value[0] != WEAPON_SWORD)
        && (weapon->value[0] != WEAPON_AXE) )
        {
        send_to_char("You need to wield a sword or axe to cleave.\n\r",ch);
        return;
        }
        if ( (victim = get_char_room(ch,arg)) == NULL )
        {
        send_to_char("They aren't here.\n\r",ch);
        return;
        }
        if (victim == ch)
        {
        send_to_char("You can't do such a thing.\n\r",ch);
        return;
        }
        if (victim->fighting != NULL)
        {
        send_to_char("They are moving too much to cleave.\n\r",ch);
        return;
        }
        if (victim->hit > 10000)
        {
        send_to_char("They are much too massive to cut through in a single cleave.\n\r",ch);
        return;
        }

        if (is_safe(ch,victim))
                return;

	if (check_shroud_of_light(ch,victim)) {return;}
    	if (check_sidestep(ch,victim,gsn_cleave,95))
        	return;


        chance /= 5;
        if ((ch->level - victim->level) < 0)
                chance -= (ch->level - victim->level)*3;
        else
                chance += (ch->level - victim->level);
        chance += get_curr_stat(ch,STAT_STR)/2;
        chance -= get_curr_stat(victim,STAT_DEX)/3;  /* Improve evasion */
        chance -= get_curr_stat(victim,STAT_STR)/4;  /* Improve repelling */
        chance -= get_curr_stat(victim,STAT_CON)/4;  /* Shock survival */
        dam_type = attack_table[weapon->value[3]].damage;


        if (!can_see(victim,ch))
                chance += 5;
        chance -= dice(2,6);
        chance = URANGE(1,chance,6);
        if (number_percent() < 50)
                sprintf(buf,"Die, %s you butchering fool!",PERS(ch,victim));
        else
                sprintf(buf,"Help! %s just tried to cleave me in half!",PERS(ch,victim));

        sn = get_weapon_sn(ch);
        skill = get_weapon_skill(ch,sn) + get_skill(ch,gsn_cleave) + 10;
        skill = URANGE(0,skill/2,100);

	if (IS_IMP(ch))
		chance += 100;

        act("You make a brutal swing at $N in an attempt to cleave them in half.",ch,0,victim,TO_CHAR);
        act("$n attempts to cleave you in half with a brutal slice.",ch,0,victim,TO_VICT);
        act("$n makes an attempt to cleave $N in half.",ch,0,victim,TO_NOTVICT);

    if (IS_NPC(victim))
	victim->last_fought = ch;

        if (number_percent() > chance)
        {
        check_improve(ch,gsn_cleave,FALSE,5);
        if (weapon->pIndexData->new_format)
                dam = dice(weapon->value[1],weapon->value[2])*skill/100;
        else
                dam = number_range(weapon->value[1]*skill/100,weapon->value[2]*skill/100);
        dam *= number_range(ch->level/10,ch->level/7);
        damage_old(ch,victim,dam,gsn_cleave,dam_type,TRUE);
        if (!IS_NPC(victim) && !IS_NPC(ch) && victim->hit > 1)
                do_myell(victim,buf,ch);
	WAIT_STATE(ch,PULSE_VIOLENCE*3);
	LAG_CHAR(victim,PULSE_VIOLENCE*2);
        return;
        }

        act("Your cleave slices $S body in half with a clean cut!",ch,0,victim,TO_CHAR);
        act("$n cleaves you in half, tearing your body into two bloody bits!",ch,0,victim,TO_VICT);
        act("$n cleaves $N into bits of bloody flesh!",ch,0,victim,TO_NOTVICT);
        check_improve(ch,gsn_cleave,TRUE,5);
        raw_kill(ch,victim);
        return;
}

void check_ground_control(CHAR_DATA *ch,CHAR_DATA *victim,float chance,int dam)
{
	if (chance < 10)
		return;
	if (dam == 0)
		return;

        chance += get_skill(ch,gsn_ground_control)/2;
        chance /= 3;

  	chance -= number_range(5,15);
	chance = URANGE(5,chance,60);

        if (ch->level < skill_table[gsn_ground_control].skill_level[ch->Class()->GetIndex()])
                return;
        if (number_percent() > chance)
	{
                check_improve(ch,gsn_ground_control,FALSE,3);
                return;

	}
        act("With brutal skill you grind $N against the ground with your weight.",ch,0,victim,TO_CHAR);
        act("$n takes hold of $N and grinds $M against the ground.",ch,0,victim,TO_NOTVICT);
        act("$n grinds you against the ground with brutal skill.",ch,0,victim,TO_VICT);
        dam -= 10;
        check_improve(ch,gsn_ground_control,TRUE,2);
        damage_old(ch,victim,dam,gsn_ground_control,DAM_BASH,TRUE);
        LAG_CHAR(victim,2*PULSE_VIOLENCE);
        return;

}

void do_tail(CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    float chance, dam;
    int knock;

    one_argument(argument,arg);

    if (IS_NPC(ch) && IS_AFFECTED(ch,AFF_CHARM))
	return;

    if ( (chance = get_skill(ch,gsn_tail)) == 0)
	return send_to_char("You don't have a tail to strike with like that.\n\r",ch);

    if (arg[0] == '\0')
    {
        victim = ch->fighting;
        if (victim == NULL)
            return send_to_char("But you aren't fighting anyone!\n\r",ch);
    }

    else if ((victim = get_char_room(ch,arg)) == NULL)
    {
        send_to_char("They aren't here.\n\r",ch);
        return;
    }

    chance = URANGE(5,chance,90);

    chance += (ch->level - victim->level);
    chance -= get_curr_stat(victim,STAT_DEX);
    chance += get_curr_stat(ch,STAT_DEX)/3;
    chance += get_curr_stat(ch,STAT_STR)/3;

	act("$n violently lashes out with $s tail.",ch,0,0,TO_ROOM);
	send_to_char("You violently lash out with your tail!\n\r",ch);

    if (number_percent() > chance)
    {
	damage_old(ch,victim,0,gsn_tail,DAM_BASH,TRUE);
	return;
    }

    dam = dice(ch->level, 3);
    knock = (ch->level + 15);

    damage_old(ch,victim,(int)dam,gsn_tail,DAM_BASH,TRUE);
    if (number_percent() < knock)
    {
	act("$n is sent crashing to the ground by the force of the blow!",victim,0,0,TO_ROOM);
	send_to_char("The tail strike sends you crashing to the ground!\n\r",victim);
	LAG_CHAR(victim,( 3 * PULSE_VIOLENCE/2));
    }
    WAIT_STATE(ch,2*PULSE_VIOLENCE);
    return;
}

/* Monk skills */

void do_throw(CHAR_DATA *ch,char *argument)
{
    CHAR_DATA *victim;
	AFFECT_DATA af;
    int dam, chance;
    char arg[MAX_INPUT_LENGTH];

    if ( ((chance = get_skill(ch,gsn_throw)) == 0
    || ch->level < skill_table[gsn_throw].skill_level[ch->Class()->GetIndex()]))
    {
        send_to_char("Throwing? What's that?\n\r",ch);
        return;
    }

    one_argument(argument,arg);
    if (arg[0] == '\0')
        victim = ch->fighting;
    else
        victim = get_char_room(ch,arg);

    if (victim == NULL)
    {
        send_to_char("They aren't here.\n\r",ch);
        return;
    }
    if (victim == ch)
    {
        send_to_char("Now that would be a bit stupid.\n\r",ch);
        return;
    }
    if ((victim->fighting != ch) && (ch->fighting != victim))
    {
        send_to_char("But you aren't engaged in combat with them.\n\r",ch);
        return;
    }

	if (is_affected(victim,gsn_protective_shield))
    {
	act("$N's throw seems to slide around you.",victim,0,ch,TO_CHAR);
	act("$N's throw seems to slide around $n.",victim,0,ch,TO_NOTVICT);
	act("Your throw seems to slide around $n.",victim,0,ch,TO_VICT);
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	return;
    }

    if (is_affected(victim,gsn_sanguine_ward))
    {
	act("$N tries to throw you but your sanguine ward keeps $M at bay.",victim,0,ch,TO_CHAR);
	act("$n's ward flares brightly as $N tries to throw $m, deflecting the attack!",victim,0,ch,TO_NOTVICT);
	act("$n's ward flares brightly as you try to throw $m, sending you flying back!",victim,0,ch,TO_VICT);
	WAIT_STATE(ch,PULSE_VIOLENCE*1);
	return;
    }
    if (is_affected(victim,gsn_heatshield))
    {
	act("$N tries to throw you but is stopped by a wave of searing heat.",victim,0,ch,TO_CHAR);
	act("$N tries to throw $n but a wave of searing heat stops him.",victim,0,ch,TO_NOTVICT);
	act("You try to throw $n but a wave of searing heat stops you!",victim,0,ch,TO_VICT);
	WAIT_STATE(ch,PULSE_VIOLENCE*1);
        damage_old(victim, ch, dice(8,10),gsn_heatshield, DAM_FIRE,TRUE);
	return;
    }
    if (is_affected(victim,gsn_elecshield))
    {
	act("$N attempts to throw you, but collides with your electric shield.",victim,0,ch,TO_CHAR);
	act("$N attempts to throw $n, but collides with $s electric shield.",victim,0,ch,TO_NOTVICT);
	act("You are stopped by an electric field, shocking you and sapping your energy!",victim,0,ch,TO_VICT);
	WAIT_STATE(ch,PULSE_VIOLENCE*1);
        damage_old(victim, ch, dice(2,10),gsn_elecshield, DAM_LIGHTNING,TRUE);
	ch->hit = UMAX(ch->hit - dice(5,10),0);
	return;
    }
    if (is_affected(victim,gsn_airshield))
    {
	act("$N attempts to throw you, but a curtain of swirling winds throws $M back!",victim,0,ch,TO_CHAR);
	act("$N attempts to throw $n, but a curtain of swirling winds throws $M back!",victim,0,ch,TO_NOTVICT);
	act("You try to throw $n but a curtain of swirling winds throw you back!",victim,0,ch,TO_VICT);
	dam = dice(2,10);
        damage_old(victim, ch, dam,gsn_airshield, DAM_DROWNING,TRUE);
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	return;
    }
    if (is_affected(victim,gsn_earthshield))
    {
        act("$N attempts to throw you, but collides with your earthshield.",victim,0,ch,TO_CHAR);
        act("$N attempts to throw $n, but instead collides with a solid barrier.",victim,0,ch,TO_NOTVICT);
        act("Your hear your bones crack as you slam into a rock-hard shield around $n.",victim,0,ch,TO_VICT);
        damage_old(victim, ch, dice(2,10),gsn_earthshield, DAM_BASH,TRUE);
	init_affect(&af);
	af.where     = TO_AFFECTS;
	af.aftype     = AFT_MALADY;
	af.type      = gsn_shattered_bone;
	af.name      = palloc_string("broken bones");
	af.level     = ch->level;
	af.duration  = ch->level/5;
	af.location  = APPLY_STR;
	af.modifier  = -1;
	affect_to_char(ch,&af);
	af.location  = APPLY_DEX;
	af.modifier  = -1;
	affect_to_char(ch,&af);
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
        return;
    }
    if (is_affected(victim,gsn_watershield))
    {
        act("$N attempts to throw you, and you feel slightly invigorated as $E crashes into your watershield.",victim,0,ch,TO_CHAR);
        act("$n looks slightly invigorated as $N crashes into $s watershield.",victim,0,ch,TO_NOTVICT);
        act("You feel drained as your mass collides with $n's shield of swirling water.",victim,0,ch,TO_VICT);
        dam = dice(3,10);
        damage_old(victim, ch, dam,gsn_watershield, DAM_DROWNING,TRUE);
        victim->hit = UMIN(victim->hit + dam,victim->max_hit);
        if (!IS_NPC(ch)) {
                ch->pcdata->condition[COND_THIRST] += 8;
                ch->pcdata->condition[COND_THIRST] = UMIN(ch->pcdata->condition[COND_THIRST],COND_HUNGRY);
        }
        return;
    }
    if (is_affected(victim,gsn_coldshield))
    {
        act("$N attempts to throw you, but is stopped by a wave of freezing air.",victim,0,ch,TO_CHAR);
        act("$N attempts to throw $n, but is stopped by a wave of freezing air.",victim,0,ch,TO_NOTVICT);
        act("You attempt to throw $n, but are seared by a wave of freezing air.",victim,0,ch,TO_VICT);
    (*skill_table[gsn_chill].spell_fun) ( gsn_chill, ch->level, victim,ch,TAR_CHAR_OFFENSIVE);
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
        return;
    }
    chance += (ch->level - victim->level);
    chance -= get_curr_stat(victim,STAT_DEX);
    chance += get_curr_stat(ch,STAT_DEX)/2;
    chance *= 9;
    chance /= 10;
    chance = URANGE(5, chance, 90);

    if (number_percent() > chance)
    {
        dam = 0;
	check_improve(ch,gsn_throw,FALSE,3);
    }
    else
    {
        if (ch->level <= 18)    dam = 30;
        else if (ch->level <= 22)   dam = 35;
        else if (ch->level <= 25)   dam = 40;
        else if (ch->level <= 28)   dam = 45;
        else if (ch->level <= 32)   dam = 48;
        else if (ch->level <= 35)   dam = 52;
        else if (ch->level <= 38)   dam = 56;
        else if (ch->level <= 40)   dam = 60;
        else if (ch->level <= 43)   dam = 64;
        else if (ch->level <= 45)   dam = 68;
        else if (ch->level <= 48)   dam = 70;
        else                        dam = 72;

	act("$n grabs $N and throws $M to the ground with stunning force!",ch,0,victim,TO_NOTVICT);
	act("You grab $N and throw $M to the ground with stunning force!",ch,0,victim,TO_CHAR);
	act("$n grabs you and throws you to the ground with stunning force!",ch,0,victim,TO_VICT);

        dam += str_app[get_curr_stat(ch,STAT_STR)].todam;
        check_improve(ch,gsn_throw,TRUE,3);
        LAG_CHAR(victim,2*PULSE_VIOLENCE);
    }

    damage_old(ch,victim,dam,gsn_throw,DAM_BASH,TRUE);
    WAIT_STATE(ch,2*PULSE_VIOLENCE);
    if (ch->fighting == victim)
        check_ground_control(ch,victim,chance,dam);

    return;
}

void do_nerve(CHAR_DATA *ch,char *argument)
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    AFFECT_DATA af;
    float chance;

    if ( (chance = get_skill(ch,gsn_nerve)) == 0
    || ch->level < skill_table[gsn_nerve].skill_level[ch->Class()->GetIndex()])
    {
        send_to_char("You don't know how to use nerve pressure tactics.\n\r",ch);
        return;
    }

    one_argument(argument,arg);
    if (arg[0] == '\0')
        victim = ch->fighting;
    else
        victim = get_char_room(ch,arg);

    if (victim == NULL)
    {
        send_to_char("Attempt to put pressure on who's nerves?\n\r",ch);
        return;
    }
    if (victim == ch)
    {
        send_to_char("You can't do that.\n\r",ch);
        return;
    }
    if (is_safe(ch,victim))
	return;

    if (is_affected(victim,gsn_nerve))
    {
	send_to_char("They have already been weakened using nerve pressure.\n\r",ch);
	return;
    }


    chance += (ch->level - victim->level)*3;
    chance -= get_curr_stat(victim,STAT_DEX)/3;
    chance += get_curr_stat(ch,STAT_DEX)/2;
    chance -= get_curr_stat(victim,STAT_CON)/3;

    if (number_percent() > chance)
    {
        act("$n grasps $N's arm but fails to apply the right pressure point.",ch,0,victim,TO_NOTVICT);
        act("You grasp $N's arm but fail to apply the right pressure point.",ch,0,victim,TO_CHAR);
        act("$n grasps your arm but fails to apply the right pressure point.",ch,0,victim,TO_VICT);
        check_improve(ch,gsn_nerve,FALSE,3);
        WAIT_STATE(ch,PULSE_VIOLENCE);
        return;
    }
    else
    {
    act("$n grasps $N's arm and weakens $m with pressure points.",ch,0,victim,TO_NOTVICT);
    act("You grasp $N's arm and weaken $m with pressure points.",ch,0,victim,TO_CHAR);
    act("$n grasps your arm and weakens you with pressure point.",ch,0,victim,TO_VICT);
    check_improve(ch,gsn_nerve,TRUE,3);
    init_affect(&af);
    af.where = TO_AFFECTS;
    af.type = gsn_nerve;
    af.location = APPLY_STR;
    af.duration = (ch->level/5);
    af.modifier = -3;
    af.aftype = AFT_SKILL;
    af.level = ch->level;
    affect_to_char(victim,&af);
    WAIT_STATE(ch,PULSE_VIOLENCE);
    }

	if (!IS_NPC(ch) && !IS_NPC(victim)
	&& (ch->fighting == NULL
	|| victim->fighting == NULL) )
	{
sprintf(buf,"Help, %s is attacking me!",PERS(ch,victim));
do_myell(victim,buf,ch);
	}
    if (victim->fighting == NULL)
    {
	multi_hit(victim,ch,TYPE_UNDEFINED);
    }
		
    return;
}

void do_endure(CHAR_DATA *ch,char *argument)
{
    AFFECT_DATA af;

    if ( (get_skill(ch,gsn_endure) == 0)
    || ch->level < skill_table[gsn_endure].skill_level[ch->Class()->GetIndex()])
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if (is_affected(ch,gsn_endure))
    {
        send_to_char("You already have the mental resolve to resist magic.\n\r",ch);
        return;
    }
    if (ch->mana < 30)
    {
        send_to_char("You don't have the mana.\n\r",ch);
        return;
    }


    if (number_percent() > get_skill(ch,gsn_endure))
    {
        send_to_char("You try to strengthen your mental resolve but fail.\n\r",ch);
        check_improve(ch,gsn_endure,FALSE,1);
        ch->mana -= 15;
        return;
    }

    send_to_char("You build up the mental resolve to better resist magic.\n\r",ch);
    check_improve(ch,gsn_endure,TRUE,1);
    init_affect(&af);
    af.where = TO_AFFECTS;
    af.type = gsn_endure;
    af.location = APPLY_SAVING_SPELL;
    af.aftype = AFT_SKILL;
    af.modifier = -20;
    af.level = ch->level;
    af.duration = ch->level;
    affect_to_char(ch,&af);

    ch->mana -= 30;
    return;
}

void do_blindness_dust(CHAR_DATA *ch,char *argument)
{
        CHAR_DATA *vch;
        CHAR_DATA *vch_next;
        AFFECT_DATA af;
        float chance;
        bool fighting = FALSE;
        char buf[MAX_STRING_LENGTH];

        if ( (chance = get_skill(ch,gsn_blindness_dust)) == 0
        || ch->level < skill_table[gsn_blindness_dust].skill_level[ch->Class()->GetIndex()])
        {
        send_to_char("You don't know how to make blindness dust to throw.\n\r",ch);
        return;
        }
        if (ch->mana < 18)
        {
        send_to_char("You don't have the mana.\n\r",ch);
        return;
        }
        if (number_percent() > chance)
        {
        act("$n hurls some dust into the air but it is blown away.",ch,0,0,TO_ROOM);
        send_to_char("You throw out some dust but it is blown away.\n\r",ch);
        ch->mana -= 9;
        check_improve(ch,gsn_blindness_dust,FALSE,2);
        WAIT_STATE(ch,12);
        return;
        }
        act("$n hurls a handful of dust into the room!",ch,0,0,TO_ROOM);
        send_to_char("You throw a handful of blindness dust into the room!\n\r",ch);
        check_improve(ch,gsn_blindness_dust,TRUE,2);
        if (ch->fighting != NULL)
                fighting = TRUE;

        init_affect(&af);
        af.where = TO_AFFECTS;
        af.type = gsn_blindness_dust;
        af.level = ch->level;
        af.duration = ch->level/10;
        SET_BIT(af.bitvector, AFF_BLIND);
        af.location = APPLY_HITROLL;
        af.modifier = -3;
	af.aftype = AFT_SKILL;
        ch->mana -= 18;
	af.mod_name = MOD_VISION;
        WAIT_STATE(ch,12);
        for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
        {
        vch_next = vch->next_in_room;
        if (is_safe(ch,vch) || IS_IMMORTAL(vch))
		 continue;
        if (is_same_group(ch,vch) || is_same_cabal(ch,vch))
		continue;
        if (!IS_AFFECTED(vch,AFF_BLIND)
        && !saves_spell(ch->level,vch,DAM_OTHER))
	{
	act("$n appears blinded.",vch,0,0,TO_ROOM);
	send_to_char("You get dust in your eyes.\n\r",vch);
                affect_to_char(vch,&af);
	}
        if (!IS_NPC(vch) && !IS_NPC(ch)
        && (vch->fighting == NULL || (!fighting)))
                {
        sprintf(buf,"Help! %s just threw dust in my eyes!",PERS(ch,vch));
                do_myell(vch,buf,ch);
                }

                if (vch->fighting == NULL)
                        multi_hit(vch,ch,TYPE_UNDEFINED);

        }

        return;
}

void do_poison_dust(CHAR_DATA *ch,char *argument)
{
        CHAR_DATA *vch;
        CHAR_DATA *vch_next;
        AFFECT_DATA af;
        float chance;
        bool fighting = FALSE;
        char buf[MAX_STRING_LENGTH];

        if ( (chance = get_skill(ch,gsn_poison_dust)) == 0
        || ch->level < skill_table[gsn_poison_dust].skill_level[ch->Class()->GetIndex()])
        {
        send_to_char("You don't know how to make poison dust to throw.\n\r",ch);
        return;
        }
        if (ch->mana < 20)
        {
        send_to_char("You don't have the mana.\n\r",ch);
        return;
        }
        if (number_percent() > chance)
        {
        act("$n hurls some dust into the air but it is blown away.",ch,0,0,TO_ROOM);
        send_to_char("You throw out some dust but it is blown away.\n\r",ch);
        ch->mana -= 10;
        WAIT_STATE(ch,12);
        check_improve(ch,gsn_poison_dust,FALSE,2);
        return;
        }
        act("$n hurls a handful of dust into the room!",ch,0,0,TO_ROOM);
        send_to_char("You throw a handful of poison dust into the room!\n\r",ch);
        check_improve(ch,gsn_poison_dust,TRUE,2);
          if (ch->fighting != NULL)
                fighting = TRUE;
		af.aftype = AFT_SKILL;
        init_affect(&af);
        af.where = TO_AFFECTS;
        af.type = gsn_poison;
		af.aftype = AFT_MALADY;
        af.level = ch->level;
        af.duration = ch->level/5;
        SET_BIT(af.bitvector, AFF_POISON);
        af.location = APPLY_STR;
        af.modifier = -5;
		af.tick_fun = poison_tick;
        ch->mana -= 20;
        WAIT_STATE(ch,12);
        for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
        {
        vch_next = vch->next_in_room;
        if (is_safe(ch,vch) || IS_IMMORTAL(vch))    continue;
        if (is_same_group(ch,vch))      continue;
        if (!IS_AFFECTED(vch,AFF_POISON)
        && !saves_spell(ch->level,vch,DAM_POISON))
	{
	act("$n turns green and looks sick.",vch,0,0,TO_ROOM);
	send_to_char("You suddenly feel very sick.\n\r",vch);
                affect_to_char(vch,&af);
	}

        if (!IS_NPC(vch) && !IS_NPC(ch)
        && (vch->fighting == NULL || (!fighting)))
                {
        sprintf(buf,"Help! %s just threw dust in my eyes!",PERS(ch,vch));
                do_myell(vch,buf,ch);
                }
                if (vch->fighting == NULL)
                        multi_hit(vch,ch,TYPE_UNDEFINED);

        }
        return;
}

void do_warcry(CHAR_DATA *ch,char *argument)
{
        AFFECT_DATA af;
        float chance;
        if ( (chance = get_skill(ch,gsn_warcry)) == 0
        || ch->level < skill_table[gsn_warcry].skill_level[ch->Class()->GetIndex()])
        {
        send_to_char("You don't know how to warcry properly.\n\r",ch);
        return;
        }

        if (is_affected(ch,gsn_warcry))
        {
        send_to_char("You are already affected by a warcry.\n\r",ch);
        return;
        }
        if (ch->mana < 20)
        {
        send_to_char("You don't have the mana.\n\r",ch);
        return;
        }

        if (number_percent() > chance)
        {
        act("$n makes some soft grunting noises.",ch,0,0,TO_ROOM);
        send_to_char("You make soft grunting sounds but nothing happens.\n\r",ch);
        check_improve(ch,gsn_warcry,FALSE,2);
        ch->mana -= 10;
        WAIT_STATE(ch,12);
        return;
        }

        act("$n lets out a blood freezing warcry!",ch,0,0,TO_ROOM);
        send_to_char("You let out a fierce warcry!\n\r",ch);
        check_improve(ch,gsn_warcry,TRUE,2);
        init_affect(&af);
        af.where = TO_AFFECTS;
        af.type = gsn_warcry;
        af.modifier = ch->level/7;
        af.location = APPLY_HITROLL;
        af.duration = ch->level;
        af.level = ch->level;
	af.aftype = AFT_SKILL;
        affect_to_char(ch,&af);
	af.location = APPLY_SAVES;
        af.modifier = -(ch->level/5);
        affect_to_char(ch,&af);
        ch->mana -= 20;
        WAIT_STATE(ch,12);
        return;
}

void do_strangle(CHAR_DATA *ch,char *argument)
{
	CHAR_DATA *victim;
	char arg[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
	int chance;
	AFFECT_DATA af;

	one_argument(argument,arg);

    
	if ( (chance = get_skill(ch,gsn_strangle)) == 0
		|| ch->level < skill_table[gsn_strangle].skill_level[ch->Class()->GetIndex()])
        return send_to_char("You don't know how to strangle properly.\n\r",ch);


	if ((victim = get_char_room(ch,arg)) == NULL)
        return send_to_char("They aren't here.\n\r",ch);
        
	if (victim == ch)
        return send_to_char("Try using a piece of rope and a high tree branch maybe?\n\r",ch);
        
	if (is_affected(victim,gsn_strangle) && !IS_AWAKE(victim))
        return send_to_char("They are already lying on the ground unconcious.\n\r",ch);
        
	if (victim->position == POS_FIGHTING)
        return send_to_char("They are moving around too much to strangle.\n\r",ch);
    
	if (is_safe(ch,victim))
		return;

	if (check_sidestep(ch,victim,gsn_strangle,95))
		return;

	if (check_blade_barrier(ch,victim)) {return;}

	update_pc_last_fight(ch,victim);

   
	if (is_affected(victim,gsn_strangle))
        return send_to_char("They are guarding their necks too well right now.\n\r",ch);

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = gsn_strangle;
	af.level = ch->level;
	af.duration = 2;
	af.modifier = 0;
	af.location = 0;
	SET_BIT(af.bitvector, AFF_SLEEP);
	af.aftype = AFT_SKILL;
	af.mod_name = MOD_CONC;
	chance /= 2;
	chance += ( 2*ch->level - 2*victim->level);
	chance -= get_curr_stat(victim,STAT_DEX)/2;
	chance += 10;
	
	if(is_affected(victim,gsn_cloak_form))
		chance+=20;
	if (chance > 75)
		chance = 75;

	if (number_percent() < 50)
		sprintf(buf,"Help! %s is trying to strangle me!",PERS(ch,victim));
	else
		sprintf(buf,"Help! %s just tried to strangle me!",PERS(ch,victim));
		
	if (number_percent() > chance)
	{
		af.duration = 2;
		zero_vector(af.bitvector);
		af.aftype = AFT_INVIS;
		affect_to_char(victim,&af);
		damage_old(ch,victim,0,gsn_strangle,DAM_BASH,TRUE);
		check_improve(ch,gsn_strangle,FALSE,2);
		WAIT_STATE(ch,12);
		if (!IS_NPC(ch) && !IS_NPC(victim))
			do_myell(victim,buf,ch);
        multi_hit(victim,ch,TYPE_UNDEFINED);
		return;
	}
        
	act("$n grabs hold of $N's neck and puts them to sleep.",ch,0,victim,TO_NOTVICT);
	act("You grab hold of $N's neck and put them to sleep.",ch,0,victim,TO_CHAR);
	send_to_char("Someone grabs hold of your neck and puts you to sleep.\n\r",victim);
	affect_to_char(victim,&af);
	victim->position = POS_SLEEPING;
	check_improve(ch,gsn_strangle,TRUE,2);
	WAIT_STATE(ch,12);
	return;
}

void do_enlist(CHAR_DATA *ch,char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    int level;
    char buf[MAX_STRING_LENGTH];
    AFFECT_DATA af;
    int chance;
    CHAR_DATA *check;

    chance = get_skill(ch,gsn_enlist);
    if (chance <= 10
    || ch->level < skill_table[gsn_enlist].skill_level[ch->Class()->GetIndex()])
    {
        send_to_char("You do not have the skills required to enlist mercenary aid.\n\r",ch);
        return;
    }
    one_argument(argument,arg);
    if (is_affected(ch,gsn_enlist))
    {
	send_to_char("You haven't built up the nerve to recruit anyone else yet.\n\r",ch);
	return;
    }

    if (arg[0] == '\0')
    {
        send_to_char("Who do you wish to enlist as a follower?\n\r",ch);
        return;
    }
    if ((victim = get_char_room(ch,arg)) == NULL)
    {
        send_to_char("They aren't here.\n\r",ch);
        return;
    }
    if (victim == ch)
    {
        send_to_char("You can't recruit yourself.\n\r",ch);
        return;
    }
    if (!IS_NPC(victim))
    {
        send_to_char("Why not just ask them to join you?\n\r",ch);
        return;
    }
    for (check = char_list; check != NULL; check = check->next)
    {
	if (is_affected(check,gsn_enlist) && check->master == ch)
	{
	send_to_char("You already have a devoted recruit following you.\n\r",ch);
	return;
	}
    }

    level = ch->level;

    if ((IS_GOOD(ch) && IS_EVIL(victim))
       || (IS_EVIL(ch) && IS_GOOD(victim)))
       level = 0;

    if (IS_NEUTRAL(victim) || IS_NEUTRAL(ch))
        level -= 4;

    if (IS_AFFECTED(victim,AFF_CHARM))
        level = 0;
    if ( (victim->level + 8) > level)
	level = 0;

    if (saves_spell(level,victim,DAM_OTHER) || level == 0)
    {
        if (number_percent() < 50)
            do_say(victim,"I don't follow scum such as you!");
        else
            do_say(victim,"You couldn't recruit a mangy dog to your company!");
        sprintf(buf,"Help! I'm being attacked by %s!",PERS(victim,ch));
        do_myell(ch,buf, victim);
        multi_hit(victim,ch,TYPE_UNDEFINED);
        return;
    }

    victim->leader = ch;
    victim->master = ch;
    init_affect(&af);
    af.where = TO_AFFECTS;
af.aftype = AFT_SKILL;
    af.type = gsn_enlist;
    af.modifier = 0;
    af.level = ch->level;
    SET_BIT(af.bitvector, AFF_CHARM);
    af.duration = ch->level;
    af.location = 0;
    affect_to_char(victim,&af);
    af.duration = 15;
    affect_to_char(ch,&af);
    act("$N salutes you and prepares to follow into combat.",ch,0,victim,TO_CHAR);
    act("$N salutes $n and falls into line with $s.",ch,0,victim,TO_NOTVICT);
    return;
}



void do_tame(CHAR_DATA *ch, char *argument)
{
        CHAR_DATA *victim;
        char arg[MAX_INPUT_LENGTH];
        int chance;

        chance = get_skill(ch,gsn_tame);
        one_argument(argument,arg);
        if (arg[0] == '\0')
                victim = ch->fighting;
        else
                victim = get_char_room(ch,arg);
        if (victim == NULL)
        {
        send_to_char("They aren't here to tame though.\n\r",ch);
        return;
        }
        if (!IS_NPC(victim))
        {
        send_to_char("Why not just talk to them about your problems?\n\r",ch);
        return;
        }
        else if (!IS_SET(victim->act,ACT_AGGRESSIVE)
        && !IS_SET(victim->off_flags,SPAM_MURDER))
        {
        send_to_char("They are as tame as they can be.\n\r",ch);
        return;
        }

        chance += (ch->level - victim->level)*3;
        if (chance > 90)        chance = 90;
        WAIT_STATE(ch,12);
        if (number_percent() > chance)
        {
        act("$n tries to calm down $N but fails.",ch,0,victim,TO_NOTVICT);
        act("$n tries to calm you down but fails.",ch,0,victim,TO_VICT);
        act("You try to calm $N down but fail.",ch,0,victim,TO_CHAR);
        check_improve(ch,gsn_tame,FALSE,2);
        return;
        }
        act("$n calms $N down.",ch,0,victim,TO_NOTVICT);
        act("You calm $N down.",ch,0,victim,TO_CHAR);
        act("$n calms you down.",ch,0,victim,TO_VICT);
        check_improve(ch,gsn_tame,TRUE,2);
        stop_fighting(victim,TRUE);
     victim->last_fought = NULL;
        REMOVE_BIT(victim->act,ACT_AGGRESSIVE);
        REMOVE_BIT(victim->off_flags,SPAM_MURDER);
        return;
}

void do_find_water(CHAR_DATA *ch,char *argument)
{
        OBJ_DATA *spring;
        int chance;
        chance = get_skill(ch,gsn_find_water);
        if (chance > 95)        chance = 95;
        if (chance == 0
        || ch->level < skill_table[gsn_find_water].skill_level[ch->Class()->GetIndex()])
        {
        send_to_char("You poke the ground with a stick but find no water that way.\n\r",ch);
        return;
        }
        if (ch->mana < 15)
        {
        send_to_char("You don't have the mana.\n\r",ch);
        return;
        }
        if (ch->in_room->sector_type == SECT_WATER
        || ch->in_room->sector_type == SECT_UNDERWATER)
        return send_to_char("Water all around but not a drop to drink.\n\r",ch);

        if (number_percent() > chance)
        {
        act("$n pokes the ground with a stick then scratches $s head.",ch,0,0,TO_ROOM);
        send_to_char("You poke about on the ground but fail to find any water.\n\r",ch);
        check_improve(ch,gsn_find_water,FALSE,1);
        ch->mana -= 7;
        WAIT_STATE(ch,18);
        return;
        }
        act("$n pokes at the ground and digs up a spring of natural water!",ch,0,0,TO_ROOM);
        send_to_char("You poke about for a bit and eventually dig up a spring of water.\n\r",ch);
        WAIT_STATE(ch,18);
        ch->mana -= 15;
        spring = create_object(get_obj_index(OBJ_VNUM_SPRING),0);
        check_improve(ch,gsn_find_water,TRUE,1);
        obj_to_room(spring,ch->in_room);
        return;
}

void do_track(CHAR_DATA *ch,char *argument)
{
        return;
}

void do_shield_cleave(CHAR_DATA *ch,char *argument)
{
	char buf[MAX_STRING_LENGTH];
        char arg[MAX_INPUT_LENGTH];
        int chance;
        OBJ_DATA *weapon;
        OBJ_DATA *shield;
        bool using_primary = TRUE;
  	CHAR_DATA *victim;

        one_argument(argument,arg);
        if (arg[0] == '\0')
                victim = ch->fighting;
        else
                victim = get_char_room(ch,arg);

        chance = get_skill(ch,gsn_shield_cleave);
        if (chance == 0
        || ch->level < skill_table[gsn_shield_cleave].skill_level[ch->Class()->GetIndex()])
        {
        send_to_char("You don't know the methods to cleave a shield in two.\n\r",ch);
        return;
        }
        weapon = get_eq_char(ch,WEAR_WIELD);
        if (weapon == NULL)
        {
        weapon = get_eq_char(ch,WEAR_DUAL_WIELD);
        using_primary = FALSE;
        }
        if (weapon == NULL
        || (weapon->value[0] != WEAPON_SWORD
        && weapon->value[0] != WEAPON_AXE) )
        {
        send_to_char("You must be wielding an axe or sword to shield cleave.\n\r",ch);
        return;
        }
        if (victim == NULL)
        {
        send_to_char("But they aren't here.\n\r",ch);
        return;
        }
    if (victim == ch)
    {
send_to_char("That isn't possible.\n\r",ch);
return;
}
        if ((shield = get_eq_char(victim,WEAR_SHIELD)) == NULL)
        {
        send_to_char("But they aren't using a shield.\n\r",ch);
        return;
        }
        chance *= 9;
        chance /= 10;
        chance += (ch->level - victim->level)*3;
        chance -= shield->level;
        if (!using_primary)
                chance -= 15;
    

	if (!IS_NPC(victim) && ch->fighting != victim)
    {
		sprintf(buf,"Help! %s just shield cleaved me!",PERS(ch,victim));
		do_myell(victim,buf,ch);
    }

	if(IS_OBJ_STAT(shield,ITEM_NODISARM))
		chance=0;
        if (number_percent() > chance)
        {
        act("$n makes a might blow at $N's shield but fails to cleave it.",ch,0,victim,TO_NOTVICT);
        act("$n lands a mighty blow to your shield but fails to cleave it.",ch,0,victim,TO_VICT);
        act("You strike a mighty blow to $N's shield but fail to cleave it.",ch,0,victim,TO_CHAR);
        check_improve(ch,gsn_shield_cleave,FALSE,1);
        WAIT_STATE(ch,12);
    	multi_hit(victim,ch,-1);
        return;
        }
        act("$n's mighty blow cleaves $N's shield in half!",ch,0,victim,TO_NOTVICT);
        act("Your mighty blow cleaves $N's shield in half!",ch,0,victim,TO_CHAR);
        act("$n strikes your shield with powerful force, cleaving it in two!",ch,0,victim,TO_VICT);
        extract_obj(shield);
        WAIT_STATE(ch,12);
	check_improve(ch,gsn_shield_cleave,TRUE,1);
    multi_hit(victim,ch,-1);

        return;
}

void bag_explode(CHAR_DATA *ch,OBJ_DATA *obj,int everyone) {

	CHAR_DATA *vch;
	CHAR_DATA *vch_next;

	act("\x01B[1;31mA small bag of explosives detonates in a blinding flash of light!\x01B[0;37m",ch,obj,0,TO_ROOM);
	act("\x01B[1;31mA small bag of explosives detonates in a blinding flash of light!\x01B[0;37m",ch,obj,0,TO_CHAR);

	if (everyone == 3) {
		act("$n EXPLODES in an eruption of blood and fluids!",ch,0,0,TO_ROOM);
		send_to_char("You have EXPLODED!!\n\r",ch);
		unequip_char(ch,obj,TRUE);
		extract_obj(obj);
		raw_kill(ch,ch);
		return;
	} else if (everyone == 2 || everyone == 1) {
		for (vch = ch->in_room->people; vch; vch = vch_next)
		{
			vch_next = vch->next_in_room;
			if (vch == ch)
				continue;

			act("$n EXPLODES in an eruption of blood and fluids!",vch,0,0,TO_ROOM);
			send_to_char("You have EXPLODED!!\n\r", vch);
			raw_kill(ch,vch);
		}
		if (everyone == 2)
			return;

		unequip_char(ch,obj,TRUE);
		extract_obj(obj);
		act("$n EXPLODES in an eruption of blood and fluids!",ch,0,0,TO_ROOM);
		send_to_char("You have EXPLODED!!\n\r", ch);
		raw_kill(ch,ch);
		return;
	}

	return;
}

/* dying of old age */

void age_death(CHAR_DATA *ch)
{
    ROOM_INDEX_DATA *pRoomIndex;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;

    if (IS_NPC(ch))
        return;


	pRoomIndex = get_room_index(1212);

    if (pRoomIndex == NULL)
        return;

/* For now default to the players recall, or midgaard */
        pRoomIndex = get_room_index(hometown_table[ch->hometown].recall);
	if (pRoomIndex == NULL)
		pRoomIndex = get_room_index(ROOM_VNUM_TEMPLE);

    for (obj = ch->carrying; obj != NULL; obj = obj_next)
    {
        obj_next = obj->next_content;
        extract_obj(obj);
    }
/* All items are poofed at age death */
    stop_fighting(ch,TRUE);
    make_corpse(ch,ch);
    char_from_room(ch);
    char_to_room(ch,pRoomIndex);
    do_look(ch,"auto");
    send_to_char("You have died and become a permanent ghost, awaiting your final departure.\n\r",ch);
    return;
}



void do_forage(CHAR_DATA *ch,char *argument)
{
        OBJ_DATA *berry_1;
        OBJ_DATA *berry_2;
        int chance, found;

    if ( (chance = get_skill(ch,gsn_forage)) == 0
    || (ch->level < skill_table[gsn_forage].skill_level[ch->Class()->GetIndex()]) )
    {
        send_to_char("You aren't able to decide on which plants are edible.\n\r",ch);
	return;
    }
    if (ch->in_room->sector_type != SECT_FOREST)
    {
    send_to_char("You aren't in a suitable forest region where you can apply your plant lore.\n\r",ch);
    return;
    }

    if (chance > 90)
        chance = 90;

    if (number_percent() > chance)
    {
    act("$n messes about in the undergrowth but comes up looking perplexed.",ch,0,0,TO_ROOM);
    send_to_char("You search around but find nothing you can recognise as edible.\n\r",ch);
    check_improve(ch,gsn_forage,FALSE,2);
    WAIT_STATE(ch,12);
    return;
    }

    act("$n messes about in the nearby bushes and comes out with some berries.",ch,0,0,TO_ROOM);
    send_to_char("You search around and find some edible berries in the bushes.\n\r",ch);
    check_improve(ch,gsn_forage,TRUE,2);

    found = number_range(1,3);

    berry_1 = create_object(get_obj_index(OBJ_VNUM_BERRY),1);
    if (berry_1 == NULL)
        return;
    obj_to_char(berry_1,ch);

    if (found >= 2)
    {
        berry_2 = create_object(get_obj_index(OBJ_VNUM_BERRY),1);
        obj_to_char(berry_2,ch);
    }
/*
    if (found >= 3)
    {
        berry_3 = create_object(get_obj_index(OBJ_VNUM_BERRY),1);
        obj_to_char(berry_3,ch);
    }
    if (found >= 4)
    {
        berry_4 = create_object(get_obj_index(OBJ_VNUM_BERRY),1);
        obj_to_char(berry_4,ch);
    }
*/

    return;
}

void do_defend(CHAR_DATA *ch,char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];

    one_argument(argument,arg);
        if ( (get_skill(ch,gsn_defend) == 0)
        || (ch->level < skill_table[gsn_defend].skill_level[ch->Class()->GetIndex()]) )
    {
        send_to_char("You aren't able to defend other people.\n\r",ch);
	return;
    }

    if (arg[0] == '\0')
    {
    	if (ch->defending == NULL)
    		sprintf(buf,"You aren't defending anyone right now.\n\r");
    	else
    		sprintf(buf,"You are defending %s.\n\r",ch->defending->name);
    	send_to_char(buf,ch);
    	return;
    }
    victim = get_char_room(ch,arg);
    if (victim == NULL)
    {
    send_to_char("They aren't here.\n\r",ch);
    return;
    }
    if (victim == ch)
    {
    send_to_char("You no longer defend anyone.\n\r",ch);
    ch->defending = NULL;
    return;
    }
    if (IS_NPC(victim))
    {
    send_to_char("They don't need to be defended.\n\r",ch);
    return;
    }
    if (ch->defending != NULL)
    {
        act("You stop defending $N.",ch,0,ch->defending,TO_CHAR);
        act("$n stops defending you.",ch,0,ch->defending,TO_VICT);
    }
    act("You start defending $N.\n\r",ch,0,victim,TO_CHAR);
    ch->defending = victim;
    act("$n is now defending you.",ch,0,victim,TO_VICT);
    return;
}

void do_intimidate(CHAR_DATA *ch,char *argument)
{
        CHAR_DATA *victim;
        char arg[MAX_INPUT_LENGTH];
        int chance;

        chance = get_skill(ch,gsn_intimidate);
        chance *= 8;
        chance /= 10;

        one_argument(argument,arg);
        if (arg[0] == '\0')
                victim = ch->fighting;
        else
                victim = get_char_room(ch,arg);
        if (victim == NULL)
        {
        send_to_char("Trying to intimidate those imaginary friends again?\n\r",ch);
        return;
        }
        if (!IS_NPC(victim))
        {
        send_to_char("Why not just threaten them yourself?\n\r",ch);
        return;
        }
        else if (!IS_SET(victim->act,ACT_AGGRESSIVE)
        && !IS_SET(victim->off_flags,SPAM_MURDER))
        {
        send_to_char("They are pretty passive as it is, why anger them further?\n\r",ch);
        return;
        }
        else if (IS_SET(victim->off_flags,OFF_INTIMIDATED))
        {
        send_to_char("They won't be intimidated any further by anyone.\n\r",ch);
        return;
        }

        chance += (ch->level*3 - victim->level*3);
        if (IS_GOOD(victim))
            chance -= 20;

        if (chance > 70)        chance = 70;
        WAIT_STATE(ch,12);
        if (number_percent() > chance)
        {
        act("$n tries to intimidate $N into submission but just makes a fool of $mself.",ch,0,victim,TO_NOTVICT);
        act("You try to intimidate $N into submission but just make a fool of yourself.",ch,0,victim,TO_CHAR);
        check_improve(ch,gsn_intimidate,FALSE,1);
        return;
        }
        act("$n stares down $N with $s overwhelming presence.",ch,0,victim,TO_NOTVICT);
        act("You stare down $N with your overwhelming presence.",ch,0,victim,TO_CHAR);

        check_improve(ch,gsn_intimidate,TRUE,1);
        stop_fighting(victim,TRUE);
        victim->last_fought = NULL;
        SET_BIT(victim->off_flags,OFF_INTIMIDATED);

        return;
}


//New flee...this allows 'flee <direction>' for the thief skill escape.
void do_flee( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *was_in;
    ROOM_INDEX_DATA *now_in;
    CHAR_DATA *victim, *panther;
    char arg[MAX_INPUT_LENGTH];
    int attempt, chance, dir;
    bool pounced = FALSE;


    if ( ( victim = ch->fighting ) == NULL ) {
        if (ch->position == POS_FIGHTING) {
			ch->position = POS_STANDING;
		}
        send_to_char( "You aren't fighting anyone.\n\r", ch );
        return;
    }
	
    if (check_bind(ch,"legs")) {
		act("$n attempts to flee but $s bindings cause $m to trip up and fall!",ch,0,0,TO_ROOM);
		act("You attempt to flee but your bindings cause you to trip up and fall!",ch,0,0,TO_CHAR);
		WAIT_STATE(ch,PULSE_VIOLENCE);
		check_parting_blow(ch,victim);
	return;
    }

    if (check_entwine(ch,1) || check_entwine(ch,2)) {
		act("You are too tightly entwined to escape!",ch,NULL,NULL,TO_CHAR);
		return;
    }
	
    if (check_entwine(ch,0)) 		do_uncoil(ch,"");

    if (is_affected(ch,gsn_web) && number_percent()>60) {
		act("$n tries to flee, but the webs $e's entangled in cause $m to trip up and fall!",ch,0,0,TO_ROOM);
		act("You try to flee, but the webs you are entangled in cause you to trip up and fall!",ch,0,0,TO_CHAR);
		check_parting_blow(ch,victim);
		WAIT_STATE(ch,PULSE_VIOLENCE);
		return;
    }

    one_argument(argument,arg);
    chance = get_skill(ch,gsn_escape);
    if (chance > 90)	chance = 90;

    dir = 10;

    if (arg[0] == '\0')
        dir = 10;
    else if (!str_prefix(arg,"north"))
        dir = 0;
    else if (!str_prefix(arg,"east"))
        dir = 1;
    else if (!str_prefix(arg,"south"))
        dir = 2;
    else if (!str_prefix(arg,"west"))
        dir = 3;
    else if (!str_prefix(arg,"up"))
        dir = 4;
    else if (!str_prefix(arg,"down"))
        dir = 5;
    else
        dir = 10;

    was_in = ch->in_room;
    for(panther = ch->in_room->people; panther; panther=panther->next_in_room) {
		if((!IS_NPC(panther) && is_affected(panther, gsn_rage) &&
			panther->pcdata->tribe == TRIBE_PANTHER && panther->fighting == ch))
			break;
	}

    if((panther && IS_NPC(panther)) || (panther && number_percent()>30)) pounced = TRUE;
	
    for ( attempt = 0; attempt < 6; attempt++ ) {
        EXIT_DATA *pexit;
        int door;

        if (dir == 10 || number_percent() > chance)
            door = number_door( );
        else
            door = dir;
        if ( ( pexit = was_in->exit[door] ) == 0
        ||   pexit->u1.to_room == NULL
        ||   ( IS_SET(pexit->exit_info, EX_CLOSED)
		&& (!IS_AFFECTED(ch,AFF_PASS_DOOR) ))
        || ( IS_NPC(ch)
        &&   IS_SET(pexit->u1.to_room->room_flags, ROOM_NO_MOB) ) )
            continue;

		if (outflank_me(ch,door))	return;

		if(check_parting_blow(ch,victim)) //TRUE = died to parting blow
			return;

		if(!is_carrying_type(ch, ITEM_BOAT) && (ch->in_room->exit[door]->u1.to_room->sector_type == SECT_WATER) 
&& !IS_AFFECTED(ch,AFF_FLYING) && !ch->Profs()->HasProf(psn_swimming))
			return send_to_char("You need a boat to go there.\n\r",ch);

		stop_fighting(ch,TRUE);
		
		move_char( ch, door, FALSE, TRUE );
	
		if ( ( now_in = ch->in_room ) == was_in )
		{
			set_fighting(victim,ch);
			set_fighting(ch, victim); //to stop people from walking away
			return;
		}

		ch->last_fought = NULL;
		ch->batter = 0;
  		ch->in_room = was_in;
        act( "$n has fled!", ch, NULL, NULL, TO_ROOM );
        if ( !IS_NPC(ch) ) {
			send_to_char( "You flee from combat!\n\r", ch );
			if (dir != 10 && get_skill(ch,gsn_escape) > 2)
        		check_improve(ch,gsn_escape,2,TRUE);
        }
	
		if(panther && pounced && !IS_NPC(panther)) {
			act("$n unleashes a loud roar and leaps after $N!",panther,0,ch,TO_NOTVICT);
			act("Sensing $n's fear as $e turns to flee, you roar and leap after $m!",ch,0,panther,TO_VICT);
		}
		/*if(panther && IS_NPC(panther)) {
			act("$n flees the room, $N still stuck to $s face!",ch,0,panther,TO_NOTVICT);
		}*/
		ch->in_room = now_in;
	
		if(panther && pounced) {
			move_char(panther,door,FALSE, TRUE);
			if(panther->in_room != ch->in_room)
				return;
			if(!IS_NPC(panther))
				act("$n chases $N into the room, leaping upon $M!",panther,0,ch,TO_NOTVICT);
			else
				act("$n runs into the room screaming, $N latched to $s face!",ch,0,panther,TO_NOTVICT);
			send_to_char("You flee, but the worm is still attached to your face!\n\r",ch);
			WAIT_STATE(ch,PULSE_VIOLENCE);
			one_hit(panther,ch,TYPE_UNDEFINED);
		}
		
        return;
    }
    send_to_char( "PANIC! You couldn't escape!\n\r", ch );
    return;
}

/* Nasty assassinattion skill. Chance of success is very high, should be
changed if you plan on using this on a PK mud. -Ceran
*/
void do_assassinate(CHAR_DATA *ch,char *argument)
{
        CHAR_DATA *victim;
        int chance, dam;
        char arg[MAX_INPUT_LENGTH];
        char buf[MAX_INPUT_LENGTH];

        one_argument(argument,arg);
        if (IS_NPC(ch))
                return;

        if ((get_skill(ch,gsn_assassinate) == 0)
        || ch->level < skill_table[gsn_assassinate].skill_level[ch->Class()->GetIndex()])
	{
	send_to_char("Huh?\n\r",ch);
	return;
	}

        if (arg[0] == '\0')
        {
        send_to_char("Attempt to assassinate who?\n\r",ch);
        return;
        }
        if ( (victim = get_char_room(ch,arg)) == NULL)
        {
        send_to_char("They aren't here.\n\r",ch);
        return;
        }
        if (victim == ch)
        {
        send_to_char("You can't do that.\n\r",ch);
        return;
        }
        if (victim->fighting != NULL || victim->position == POS_FIGHTING)
        {
        send_to_char("They are moving around too much to get in close for the kill.\n\r",ch);
        return;
        }

        if (is_safe(ch,victim))
                return;
	if (check_blade_barrier(ch,victim))
		return;
        if (victim->hit < victim->max_hit - 50)
        {
        send_to_char("They have too much blood on them right now to locate their strike point.\n\r",ch);
        return;
        }

        chance = get_skill(ch,gsn_assassinate);

        if (number_percent() > chance)
        {
        sprintf(buf,"Help! %s just tried to assassinate me!",PERS(ch,victim));
        do_myell(victim,buf,ch);
        damage_old(ch,victim,0,gsn_assassinate,DAM_OTHER,TRUE);
        check_improve(ch,gsn_assassinate,1,TRUE);
        WAIT_STATE(ch,PULSE_VIOLENCE*2);
        return;
        }

	chance *=2;
	chance /=3;

        chance += (ch->level * 3);
        chance -= (victim->level * 4);
        if (!can_see(victim,ch))
                chance += 10;
        if (victim->position == POS_FIGHTING)
                chance -= 10;
        else if (victim->position == POS_SLEEPING)
                chance += 10;

        chance /= 2;
        chance = URANGE(2,chance,25);
	
        if (victim->level > (ch->level + 8))
            chance = 0;
	  if(chance>6)
		chance=5;
        act("$n tries to strike at $N's critical nerves!",ch,0,victim,TO_NOTVICT);
        act("You try to strike $N's critical nerves!",ch,0,victim,TO_CHAR);
/*        act("$n strikes at your critical nerves!",ch,0,victim,TO_VICT); */

	if (ch->level == MAX_LEVEL) {
		chance = 100;
	}

        if (number_percent() < chance)
        {
        act("$n +++ ASSASSINATES +++ $N!",ch,0,victim,TO_NOTVICT);
        act("You +++ ASSASSINATE +++ $N!",ch,0,victim,TO_CHAR);
        act_new("$n +++ ASSASSINATES +++ you!",ch,0,victim,TO_VICT,POS_DEAD);
        raw_kill(ch,victim);
        check_improve(ch,gsn_assassinate,TRUE,3);
        return;
        }
        else
        {
        dam = ch->damroll*2;
        if (number_percent() < get_skill(ch,gsn_enhanced_damage))
        {
            dam += (number_percent() * dam/100);
        }

        damage_old(ch,victim,dam,gsn_assassinate,DAM_ENERGY,TRUE);
        check_improve(ch,gsn_assassinate,FALSE,1);
        if (!IS_NPC(victim) && !IS_NPC(ch) && (victim->ghost == 0))
               {
                sprintf(buf,"Help! %s just tried to assassinate me!",PERS(ch,victim));
		do_myell(victim,buf,ch);
                }
        multi_hit(victim,ch,TYPE_UNDEFINED);
		WAIT_STATE(ch,PULSE_VIOLENCE*2);
        }

        return;
}


/* Some more new skills..Ceran */

void do_lash(CHAR_DATA *ch,char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int chance;
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *weapon;

    one_argument(argument,arg);

    if ( (chance = get_skill(ch,gsn_lash)) == 0)
        return send_to_char("You don't have the skill to lash people's legs.\n\r",ch);

    weapon = get_eq_char(ch,WEAR_WIELD);
    if (weapon == NULL || (weapon->value[0] != WEAPON_WHIP && weapon->value[0] != WEAPON_FLAIL) )
    {
        chance -= 15;
        weapon = get_eq_char(ch,WEAR_DUAL_WIELD);
    }
    if (weapon == NULL || (weapon->value[0] != WEAPON_WHIP && weapon->value[0] != WEAPON_FLAIL))
    	return send_to_char("You aren't wielding any weapon to lash with.\n\r",ch);
    if (weapon->value[0] != WEAPON_WHIP && weapon->value[0] != WEAPON_FLAIL)
    	return send_to_char("You need to be wielding a whip or flail to lash.\n\r",ch);

    if (arg[0] == '\0')
    {
        victim = ch->fighting;
        if (victim == NULL)
            return send_to_char("But you aren't fighting anyone!\n\r",ch);
    }

    else if ((victim = get_char_room(ch,arg)) == NULL)
        return send_to_char("They aren't here.\n\r",ch);

    if (victim == ch)
        return send_to_char("You try to lash your feet and look clumsy doing it.\n\r",ch);

    if (is_safe(ch,victim))
        return;

    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
        return act("But $N is your friend!",ch,NULL,victim,TO_CHAR);

	/* speed */
    if (IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch, AFF_HASTE))
        chance += 5;
    if (IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,AFF_HASTE))
        chance -= 35;
    chance += get_curr_stat(ch,STAT_DEX) * 2;
    chance -= get_curr_stat(victim,STAT_DEX) * 2;
    if (IS_AFFECTED(victim,AFF_FLYING))
        chance -= 30;
    if (!IS_NPC(ch) && ch->Class()->GetIndex() != CLASS_WARRIOR)
		chance -= 30;
    /* level */
    if(IS_NPC(victim))
    	chance += (ch->level - victim->level)*3;
    if (!IS_NPC(ch) && !IS_NPC(victim) && (victim->fighting == NULL || ch->fighting == NULL))
    {
	sprintf(buf,"Help! %s is lashing me!",PERS(ch,victim));
	do_myell(victim,buf,ch);
    }
    if (number_percent() > chance)
    {
        act("$n lashes at $N's legs but misses.",ch,0,victim,TO_NOTVICT);
        act("$n lashes at your legs but misses.",ch,0,victim,TO_VICT);
        act("You lash at $N's legs but miss.",ch,0,victim,TO_CHAR);
        check_improve(ch,gsn_lash,FALSE,1);
	if(ch->fighting==NULL)
		set_fighting(ch, victim);
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
        return;
    }
        act("$n lashes $N's legs, sending $M crashing down.",ch,0,victim,TO_NOTVICT);
        act("$n lashes your legs, sending you crashing to the ground.",ch,0,victim,TO_VICT);
        act("You lash $N's legs, sending $M crashing to the ground.",ch,0,victim,TO_CHAR);
        check_improve(ch,gsn_lash,TRUE,1);
        LAG_CHAR(victim,(int)(PULSE_VIOLENCE*1.5));
        WAIT_STATE(ch,PULSE_VIOLENCE*2);
        damage_old(ch,victim,dice(2,7),gsn_lash,DAM_BASH,TRUE);
	if(ch->fighting==NULL)
		multi_hit(victim,ch,TYPE_UNDEFINED);
        victim->position = POS_RESTING;
        return;
}

void do_pugil(CHAR_DATA *ch,char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    int chance;
    int dam;

    one_argument(argument,arg);

    if ( (chance = get_skill(ch,gsn_pugil)) == 0
    ||  IS_NPC(ch)
    ||  (!IS_NPC(ch) && ch->level < skill_table[gsn_pugil].skill_level[ch->Class()->GetIndex()]) )
    {
        send_to_char("You're not trained in the art of pugiling.\n\r",ch);
	return;
    }

    if (arg[0] == '\0')
    {
	victim = ch->fighting;
	if (victim == NULL)
	{
	send_to_char("But you aren't fighting anyone.\n\r",ch);
	return;
	}
    }
    else if ((victim = get_char_room(ch,arg)) == NULL)
    {
	send_to_char("They aren't here.\n\r",ch);
	return;
    }

    if (ch->fighting == NULL)
	{
        send_to_char("You can't pugil someone like that.\n\r",ch);
	return;
	}

    if (victim == ch)
    {
        send_to_char("That would be a bit stupid.\n\r", ch);
	return;
    }

   obj = get_eq_char(ch,WEAR_WIELD);
   if (obj == NULL || obj->value[0] != WEAPON_STAFF)
    {
    send_to_char("You must be wielding a staff to pugil.\n\r",ch);
    return;
    }

    chance += (ch->level - victim->level);
    chance = URANGE(5, chance, 90);

    WAIT_STATE(ch,skill_table[gsn_pugil].beats);

    if (number_percent() < chance)
    {
        act("You smash $N with a bone crushing pugil!",ch,NULL,victim,TO_CHAR);
        act("$n smashes you with a bone crushing pugil!",ch,NULL,victim,TO_VICT);
        act("$n smashes $N with a bone crushing pugil!",ch,NULL,victim,TO_NOTVICT);
        check_improve(ch,gsn_pugil,TRUE,1);
	one_hit_new(ch, victim, gsn_pugil, HIT_NOSPECIALS, HIT_UNBLOCKABLE, HIT_NOADD, 170, "pugil strike");
    }	
    else
    {
        check_improve(ch,gsn_pugil,FALSE,1);

        damage_old(ch,victim,0,gsn_pugil,DAM_NONE,TRUE);
    }

    WAIT_STATE(ch,20);
    return;
}

void do_protection_heat_cold(CHAR_DATA *ch,char *argument)
{
    AFFECT_DATA af;
    int chance;
    chance = get_skill(ch,gsn_protection_heat_cold);

    if (chance == 0
	|| ch->level <skill_table[gsn_protection_heat_cold].skill_level[ch->Class()->GetIndex()])
    {
send_to_char("You don't know how to protect yourself from the elements.\n\r",ch);
	return;
    }

    if (is_affected(ch,gsn_protection_heat_cold))
    {
    send_to_char("You are already protected from the elements.\n\r",ch);
    return;
    }

    if (number_percent() > chance)
    {
	send_to_char("You try to protect yourself from the elements but fail.\n\r",ch);
	check_improve(ch,gsn_protection_heat_cold,FALSE,1);
	return;
    }

    init_affect(&af);
    af.where = TO_RESIST;
    af.aftype = AFT_SKILL;
    af.type = gsn_protection_heat_cold;
    af.level = ch->level;
    af.modifier = 0;
    af.duration = ch->level;
    SET_BIT(af.bitvector, RES_FIRE);
	SET_BIT(af.bitvector, RES_COLD);
    af.location = 0;
    af.mod_name = MOD_RESISTANCE;
	affect_to_char(ch,&af);
    send_to_char("You are protected from the elements.\n\r",ch);
	check_improve(ch,gsn_protection_heat_cold,TRUE,1);
    return;
}

void do_call_to_arms( CHAR_DATA *ch, char *arguement)
{
    CHAR_DATA *target;
    CHAR_DATA *target_next;

    if (IS_NPC(ch))
    return;

    if (ch->mana < 10)
    {
	send_to_char("You are unable to muster the force of will to lead more crusades.\n\r", ch);
	return;
    }

    if ( ch->fighting == NULL )
    {
     send_to_char("You give a stirring call to arms, but alas there is nobody to fight.\n\r", ch);
     act("$n cries out to nobody in particular to fight nobody in particular.", ch, NULL, NULL, TO_ROOM);
     return;
    }

    if (IS_SET(ch->act,PLR_CRIMINAL))
    {
        send_to_char("Do you really think people will rally to a criminal?!\n\r",ch);
        return;
    }

    if ( !IS_GOOD(ch))
    {
     send_to_char("Despite your best attempts nobody will join you.\n\r",ch);
     act("$n gives a pathetic call to arms.",ch, NULL, NULL, TO_ROOM);
     return;
    }

    if (number_percent() > get_skill(ch,gsn_rally))
    {
        send_to_char("You yell some wild rhetoric but fail to inspire anyone.\n\r",ch);
        check_improve(ch,gsn_rally,FALSE,2);
        WAIT_STATE(ch,12);
        return;
    }

    act("$n gives a stirring call to arms!",ch, NULL, NULL, TO_ROOM);
    send_to_char("You give a stirring call to arms!\n\r",ch);

   for (target = ch->in_room->people; target != NULL; target = target_next)
   {
    target_next = target->next_in_room;

    if (!IS_GOOD(target) || !IS_NPC(target) || IS_AFFECTED(target,AFF_CHARM))
        continue;
    if (is_same_group(ch,target) || target->position < POS_RESTING)
        continue;
    if (target->position == POS_FIGHTING)
        continue;
    if (ch->mana < 15)
        continue;
    if (ch->fighting == NULL)
        continue;
    if ( target->level > (ch->level + 12) )
        {
         act("$n tells you, 'Fight your own battles and prove your worth paladin'", target, NULL, ch,
TO_VICT);
         continue;
        }
       if ( number_percent() > ( ch->level - target->level + 70) )
        {
         act("Your call to arms gets little more than a passing consideration from $N.", ch, NULL, target, TO_CHAR);
         check_improve(ch,gsn_rally, FALSE,2);
         continue;
        }
       ch->mana -= 15;
       act("$N rallys to your call!", ch, NULL, target, TO_CHAR);
       act("$n screams and rushes to attack $N!", target, NULL, ch->fighting, TO_NOTVICT);
       act("$n screams and rushes forwards to attack you!", target, NULL, ch->fighting, TO_VICT);
       multi_hit(target, ch->fighting, TYPE_UNDEFINED);
       continue;
    }
    WAIT_STATE(ch,12);
 return;
}

void do_iron_resolve(CHAR_DATA *ch,char *argument)
{
    AFFECT_DATA af;

    if (is_affected(ch,gsn_iron_resolve))
    {
        send_to_char("Your mind is already firmly set on your inner being.\n\r",ch);
        return;
    }
    if (ch->mana < 100)
    {
        send_to_char("You don't have the mental concentration right now.\n\r",ch);
        return;
    }

    if (number_percent() > get_skill(ch,gsn_iron_resolve))
    {
        send_to_char("You try to concentrate on your inner being but fail to maintain it.\n\r",ch);
        ch->mana -= 50;
        WAIT_STATE(ch,12);
        return;
    }

    send_to_char("You feel yourself come together as mind and body.\n\r",ch);

    init_affect(&af);
    af.where = TO_AFFECTS;
af.aftype = AFT_SKILL;
    af.type = gsn_iron_resolve;
    af.level = ch->level;
    af.location = 0;
    af.modifier = 0;
    af.duration = ch->level/3;
    ch->mana -= 100;
    WAIT_STATE(ch,12);
}

void do_quiet_movement(CHAR_DATA *ch, char *argument)
{
    AFFECT_DATA af;
    int chance;

    chance = get_skill(ch,gsn_quiet_movement);
    if (chance == 0
    || ch->level < skill_table[gsn_quiet_movement].skill_level[ch->Class()->GetIndex()])
    {
        send_to_char("You don't know how to move with silent stealth through the wilderness.\n\r",ch);
        return;
    }

    if (is_affected(ch,gsn_quiet_movement))
    {
        send_to_char("You are already attempting to move silently through the wilderness.\n\r",ch);
        return;
    }
    if (ch->in_room->sector_type == SECT_FOREST)
    {
        send_to_char("You attempt to move quietly through the forest.\n\r",ch);
    }
    else if (ch->in_room->sector_type == SECT_MOUNTAIN)
    {
        send_to_char("You attempt to move quietly through the mountains.\n\r",ch);
    }
    else if (ch->in_room->sector_type == SECT_HILLS)
    {
        send_to_char("You attempt to move quietly through the hills.\n\r",ch);
    }
    else
    {
        send_to_char("You aren't in the right kind of wilderness to mvoe quietly.\n\r",ch);
        return;
    }

    if (number_percent() > chance)
    {
        check_improve(ch,gsn_quiet_movement,FALSE,1);
        return;
    }
    check_improve(ch,gsn_quiet_movement,TRUE,1);

    init_affect(&af);
    af.where = TO_AFFECTS;
    af.type = gsn_quiet_movement;
    af.aftype = AFT_SKILL;
    af.location = 0;
    af.modifier = 0;
    af.duration = ch->level;
    af.level = ch->level;
    affect_to_char(ch,&af);
    return;
}

bool check_parting_blow( CHAR_DATA *ch, CHAR_DATA *victim )
{
	if(ch->in_room != victim->in_room)
		return FALSE;

	if (get_skill(victim,gsn_parting_blow) <= 0)
		return FALSE;

	if (number_percent() < get_skill(victim,gsn_parting_blow)) {
		act("You get in one more shot as $N flees.",victim,0,ch,TO_CHAR);
		act("$n gets in one more shot as $N flees.",victim,0,ch,TO_NOTVICT);
		act("$n gets in one more shot as you flee.",victim,0,ch,TO_VICT);
		check_improve(victim,gsn_parting_blow,TRUE,1);
		if(one_hit_new(victim,ch,gsn_parting_blow,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,100,NULL) == -1) //death
			return TRUE;
	} else {
		check_improve(victim,gsn_parting_blow,FALSE,3);
	}

	return FALSE;
}

CHAR_DATA *get_cabal_guardian(int cabal)
{
	CHAR_DATA *cabalguardian;
	for(cabalguardian=char_list;cabalguardian!=NULL;cabalguardian=cabalguardian->next)
		if(IS_NPC(cabalguardian) && cabalguardian->cabal==cabal && IS_CABAL_GUARD(cabalguardian))
			return cabalguardian;
	return NULL;
}

bool check_blade_barrier(CHAR_DATA *ch, CHAR_DATA *victim)
{
	if(is_safe(ch,victim))
		return FALSE;
	if(is_affected(victim,gsn_blade_barrier))
        {
                act("Your blade barrier tears into $N's flesh before fading away into nothingness.",victim,0,ch,TO_CHAR);
                act("$n's blade barrier tears into $N's flesh before fading away into nothingness.",victim,0,ch,TO_NOTVICT);
                act("$n's blade barrier tears into your flesh before fading away into nothingness.",victim,0,ch,TO_VICT);
                damage_old(victim,ch,dice(victim->level,5),gsn_blade_barrier,DAM_SLASH,TRUE);
                affect_strip(victim,gsn_blade_barrier);
		return TRUE;
        }
return FALSE;
}

bool check_maneuvering( CHAR_DATA *ch, CHAR_DATA *victim, int dt)
{
	int chance, count=0;
	bool ableth=FALSE;
	CHAR_DATA *vch, *vch_next;
	char buf1[MAX_STRING_LENGTH], buf2[MAX_STRING_LENGTH];

	
    if(!IS_NPC(victim))
	{
		if(!style_check(gsn_maneuvering, victim->pcdata->style))
        	return FALSE;
	}

	if(!IS_AWAKE(victim))
		return FALSE;

	/* Check to make sure the person has the skill */
	if ((chance = get_skill(victim,gsn_maneuvering)) == 0)
		return FALSE;

		/* Loop through all the people in the room */
        for (vch = victim->in_room->people; vch != NULL; vch = vch_next)
        {
                vch_next = vch->next_in_room;
				/* if anyone in his group, is in the room..the duelist doesn't get to maneuver */
				if(vch != victim && is_same_group(victim,vch))
					return FALSE;

				/* Do the count of people fighting indirectly with the duelist..only 2 people after the
		   		person he is tanking..get to be maneuvered around */
			if(vch != victim)
			{
				if(vch->fighting == victim && victim->fighting != vch)
					count++;
				if(vch == ch && count<3)
					ableth=TRUE;
			}
		}

	/* Mob level difference */
	chance -= ch->level - victim->level;

	/* Can't be fighting directly */
	if(ch == victim->fighting || !ableth)
		return FALSE;


	/* Skill % * 65% chance of it happening */
	chance -= (int)((float) chance * .35);

	/* Roll the dice..*/
	if ( number_percent( ) >= chance)
		return FALSE;

	/* Echo the messages to the relative people */
	sprintf(buf1,"You position yourself on the far side of %s, keeping %s at bay.\n\r",
		IS_NPC(victim->fighting) ? victim->fighting->short_descr : victim->fighting->name,
		IS_NPC(ch) ? ch->short_descr : ch->name);
	send_to_char(buf1,victim);
	sprintf(buf2,"In the chaos of the fray, you are unable to get close enough to strike %s.\n\r",
		IS_NPC(victim) ? victim->short_descr : victim->name);
	send_to_char(buf2,ch);

	/* Check the improve */
	check_improve(victim,gsn_maneuvering,TRUE,2);

	return TRUE;
}

bool check_catch( CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *obj)
{
	int chance=0;

	if(!IS_NPC(ch)){
		if(!style_check(gsn_catch, ch->pcdata->style))
			return FALSE;
	}

	if ((chance = get_skill(ch,gsn_catch)) == 0)
		return FALSE;

	if((get_eq_char(ch,WEAR_SHIELD)!=NULL || get_eq_char(ch,WEAR_DUAL_WIELD)!=NULL) && (get_eq_char(ch,WEAR_WIELD)!=NULL))
		return FALSE;

	if(!can_see_obj(ch,obj))
		return FALSE;
	
	if (number_percent() > (chance*.9))
	{
		act("You attempt to grab $N's disarmed weapon, but fail to catch it.", ch,0,victim,TO_CHAR);
		act("$n tries to catch your weapon as it falls to the ground.", ch, 0, victim, TO_VICT);
		act("$n attempts to grab for $N's disarmed weapon, but fails to catch it!", ch, 0, victim, TO_NOTVICT);
		return FALSE;
	}
	else
	{
		act("As $N's $t hurtles through the air, you reach out and catch it!",
			ch,weapon_name_lookup(obj->value[0]),victim,TO_CHAR);
		act("As your $t hurtles through the air, $n reaches out and catchs it in midair!",
			ch,weapon_name_lookup(obj->value[0]),victim,TO_VICT);
		act("As $N's $t hurtles through the air, $n reaches out and catchs it in midair!",
			ch,weapon_name_lookup(obj->value[0]),victim,TO_NOTVICT);
		return TRUE;
	}
}

char *get_dam_message(CHAR_DATA *ch, int dt)
{
	return get_attack_noun(ch,dt);
}

char * get_attack_noun(CHAR_DATA *ch, int dt)
{
      OBJ_DATA *wield;
      int tmp_dt;
      char *attack;
/*      if(IS_SHIFTED(ch))
      {
	if(!attack_lookup(form_table[ch->pcdata->shifted].attack_type))
		return "Error";
	return attack_table[attack_lookup(form_table[ch->pcdata->shifted].attack_type)].noun;
      }
*/      tmp_dt = TYPE_HIT;
      if (dt  == gsn_dual_wield)
          wield = get_eq_char(ch,WEAR_DUAL_WIELD);
      else
      wield = get_eq_char(ch,WEAR_WIELD);
        if (wield != NULL)
          tmp_dt += wield->value[3];
      else
          tmp_dt += ch->dam_type;
      if (tmp_dt > TYPE_HIT && tmp_dt < TYPE_HIT + MAX_DAMAGE_MESSAGE)
          attack = attack_table[tmp_dt - TYPE_HIT].noun;
      else
      {
          attack = attack_table[17].noun;
      }
      return attack;
}

int get_attack_number(CHAR_DATA *ch, int dt)
{
      OBJ_DATA *wield;
      int tmp_dt;
//      if(IS_SHIFTED(ch))
//	return attack_lookup(form_table[ch->pcdata->shifted].attack_type);
      tmp_dt = TYPE_HIT;
      if (dt  == gsn_dual_wield)
          wield = get_eq_char(ch,WEAR_DUAL_WIELD);
      else
          wield = get_eq_char(ch,WEAR_WIELD);
        if (wield != NULL)
          tmp_dt += wield->value[3];
      else
          tmp_dt += ch->dam_type;
      if (tmp_dt > TYPE_HIT && tmp_dt < TYPE_HIT + MAX_DAMAGE_MESSAGE)
      	return (tmp_dt - TYPE_HIT);
      return 1;
}

void trophy_corpse(CHAR_DATA *ch, CHAR_DATA *victim)
{
	OBJ_DATA *corpse, *belt, *newbelt;
	OBJ_AFFECT_DATA oaf;
	TROPHY_DATA *placeholder;
	int i, scalps;
	
	for(corpse = ch->in_room->contents; corpse; corpse = corpse->next_content)
		if(!str_cmp(corpse->owner, victim->true_name))
			break;
	
	belt = get_eq_char(ch,WEAR_WAIST);
	
	if(!corpse || !belt || belt->pIndexData->vnum != OBJ_VNUM_TROPHY_BELT)
		return;

	if(number_percent()>get_skill(ch,gsn_trophy)+8) {
		act("You mangle $N's scalp beyond repair in your attempt to trophy it, and throw it away in disgust.",ch,0,victim,TO_CHAR);
		act("$n mangles $N's scalp beyond repair in $s attempt to trophy it, and throws it away in disgust.",ch,0,victim,TO_NOTVICT);
		check_improve(ch,gsn_trophy,FALSE,1);
		return;
	}

	act("With a smooth stroke, you slice $N's scalp cleanly from $S head, attaching it to your belt!",ch,0,victim,TO_CHAR);
	act("$n smoothly slices $N's scalp from $S head and attaches it to $s belt!",ch,0,victim,TO_NOTVICT);
	check_improve(ch,gsn_trophy,TRUE,1);

	if (!ch->pcdata->trophy)
		belt->value[4] = 0;
	
	/* Update the hit/dam mods on the belt. */

	scalps = belt->value[4];
	
	extract_obj(belt);
	
	newbelt = create_object(get_obj_index(23645),0);
	obj_to_char(newbelt,ch);

	newbelt->value[4] = scalps;
	
	init_affect_obj(&oaf);
	oaf.where 		= TO_OBJ_APPLY;
	oaf.type		= gsn_trophy;
	oaf.aftype		= AFT_POWER;
	oaf.level		= ch->level;
	oaf.duration	= -1;
	oaf.modifier	= (newbelt->value[4] + 1) / 2;
	oaf.location	= APPLY_HITROLL;
	affect_to_obj(newbelt, &oaf);
	oaf.location	= APPLY_DAMROLL;
	oaf.modifier	= UMAX((newbelt->value[4] + 1) / 2 - (newbelt->value[4] % 2) + 1,1);
	affect_to_obj(newbelt, &oaf);

	equip_char(ch, newbelt, WEAR_WAIST, FALSE);

	/* Handling of keeping track of the victims trophied on the belt. */

	if(newbelt->value[4] == 0)
	{
		
		ch->pcdata->trophy = new_trophy_data(victim->true_name);
	}
	else
	{
		placeholder = ch->pcdata->trophy;
	
		for (i=1;i < newbelt->value[4];i++)
		{
			if (!ch->pcdata->trophy)
				break;
			if (!ch->pcdata->trophy->next)
				break;

			ch->pcdata->trophy = ch->pcdata->trophy->next;
		}

		ch->pcdata->trophy = ch->pcdata->trophy->next = new_trophy_data(victim->true_name);
		
		ch->pcdata->trophy = placeholder;
	}
	
	newbelt->value[4]++;
	
}

bool check_sidestep(CHAR_DATA *ch, CHAR_DATA *victim, int skill, int mod)
{
	int chance;
	char type[MSL];

	if(IS_NPC(victim) || (IS_NPC(ch) && IS_AFFECTED(ch,AFF_CHARM)))
		return FALSE;
	
	if(!style_check(gsn_sidestep, victim->pcdata->style))
		return FALSE;

	if(skill == 0)
	{
		if(get_skill(victim,gsn_sidestep) < 2)
			return FALSE;
		chance=mod;
		sprintf(type,"attack");
	}
	else
	{
		if((chance = get_skill(victim,gsn_sidestep)) < 2)
			return FALSE;
		sprintf(type,"%s",skill_table[skill].name);
		chance=(int)(chance * ((float)mod / 100));
	}
	
	if (victim->fighting)
		return FALSE;

	if (!can_see(victim,ch))
		return FALSE;

	if (IS_AFFECTED(ch, AFF_HIDE))
		return FALSE;

	if (is_affected(victim,gsn_hold_person))
		return FALSE;

	if(!IS_AWAKE(victim))
		return FALSE;

	if(victim->pcdata->energy_state < -4)
		return FALSE;

	if(is_affected(victim,gsn_bind_feet))
		return FALSE;

	if(is_affected(victim,gsn_creeping_tomb))
		return FALSE;

	if (number_percent() < chance)
	{
        act("Anticipating the coming strike, $N deftly sidesteps your $t.",ch,type,victim,TO_CHAR);
		act("Anticipating the coming strike, you deftly sidestep $n's $t.",ch,type,victim,TO_VICT);
		WAIT_STATE(ch,PULSE_VIOLENCE*2);
		return TRUE;
	}
	else
	{
        act("You notice $n bearing down on you too late, and fail to sidestep his $t.",ch,type,victim,TO_VICT);
	}

	return FALSE;
}

int check_terrain_mastery(CHAR_DATA *ch)
{
	int i,skill,total=0;
	float sect_per = 0;

	if(IS_NPC(ch))
		return 0;
	
	if((skill = get_skill(ch,gsn_terrain_mastery)) > 0)
	{
		if(!style_check(gsn_terrain_mastery, ch->pcdata->style))
			return 0;

		for(i=1; i < SECT_MAX; i++)
		{	
			total+=ch->pcdata->sect_time[i];
		}
		
		if (total == 0)		return 0;
		
		sect_per=(float)(ch->pcdata->sect_time[ch->in_room->sector_type])/(float)total * 100;

		check_improve(ch,gsn_terrain_mastery,TRUE,4);
		return (int)(sect_per / (10 - 0.36*(ch->mod_stat[STAT_INT])));
	}

	return 0;
}

void bleeding_tick (CHAR_DATA *ch, AFFECT_DATA *af)
{
	if (IS_NPC(ch) &&
		(IS_SET(ch->act,ACT_UNDEAD) || IS_SET(ch->form,FORM_UNDEAD)))
		return;

	send_to_char("You continue bleeding from your wounds.\n\r",ch);
	
	if (!af->owner)
		af->owner = ch;

	damage_new(af->owner,ch,af->level,gsn_bleeding,DAM_OTHER,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);
	
	return;
}

void do_gore (CHAR_DATA *ch, char *argument)
{
	char arg[MSL], buf[MSL];
	CHAR_DATA *victim;
	AFFECT_DATA af;
	int skill=0, dam;

	one_argument(argument,arg);

	if ((!IS_NPC(ch)) && (skill = get_skill(ch,skill_lookup("gore"))) == 0)
		return send_to_char("Huh?\n\r",ch);
	
	if(IS_NPC(ch))
		skill = 100;

	if ((!IS_NPC(ch)) && str_cmp(race_table[ch->race].name,"minotaur")
		&& !IS_SET(ch->parts,PART_HORNS))
		return send_to_char("With what horns?\n\r",ch);

	if (ch->fighting)
		return send_to_char("You can't gore in combat!\n\r",ch);

	if (arg[0] == '\0')
		return send_to_char("Gore whom?\n\r",ch);
	
	if ((victim = get_char_room(ch,arg)) == NULL)
		return send_to_char("They aren't here.\n\r",ch);

	if (victim == ch)
		return send_to_char("You wrench your neck trying to poke yourself with your horns.\n\r",ch);

	if (is_safe(ch,victim))
		return;

	if (check_sidestep(ch,victim,gsn_gore,95))
		return;

	if(!IS_NPC(victim)) {
		sprintf(buf,"Help! %s is goring me!", PERS(ch,victim));
		do_myell(victim,buf,ch);
	}

	if (number_percent() < 0.7*skill) {
		act("Charging furiously, you ram into $N, goring $M with your horns!",ch,0,victim,TO_CHAR);
		act("Charging furiously, $n rams into you, goring you with $s horns!",ch,0,victim,TO_VICT);
		act("Charging furiously, $n rams into $N, goring $M with $s horns!",ch,0,victim,TO_NOTVICT);
		
		dam = dice(ch->level,5);
		
		damage_new(ch,victim,dam,gsn_gore,DAM_PIERCE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"goring");

		if (number_percent() < 0.5*skill && !is_affected(victim,gsn_bleeding)){
			act("Blood pours from two deep wounds as $n's horns pierce your flesh!",ch,0,victim,TO_VICT);
			act("Blood pours from two deep wounds as your horns pierce $N's flesh!",ch,0,victim,TO_CHAR);
			act("Blood pours from two deep wounds as $n's horns pierce $N's flesh!",ch,0,victim,TO_NOTVICT);
			init_affect(&af);
			af.where		=	TO_AFFECTS;
			af.type			=	gsn_bleeding;
			af.aftype		=	AFT_MALADY;
			af.level		=	ch->level;
			af.owner		=	ch;
			af.duration		=	4;
			af.location		=	APPLY_STR;
			af.modifier		=	-ch->level/10;
			af.tick_fun		=	bleeding_tick;
			af.end_fun		=	NULL;
			new_affect_to_char(victim,&af);
		}

		check_improve(ch,gsn_gore,TRUE,2);
		
		WAIT_STATE(ch,2*PULSE_VIOLENCE);
	} else {
		act("$N avoids your attempt to gore $M, and you go sprawling to the ground!",ch,0,victim,TO_CHAR);
		act("You avoid $n's attempt to gore you, and $e goes sprawling to the ground!",ch,0,victim,TO_VICT);
		act("$N avoids $n's attempt to gore $M, and $n goes sprawling to the ground!",ch,0,victim,TO_NOTVICT);

		damage_new(ch,victim,0,gsn_gore,DAM_PIERCE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"goring");

		WAIT_STATE(ch,3*PULSE_VIOLENCE);
		ch->position = POS_RESTING;
		check_improve(ch,gsn_gore,FALSE,2);
	}
	
	return;
}

void do_headbutt (CHAR_DATA *ch, char *argument)
{	
	CHAR_DATA *victim;
	AFFECT_DATA af;
	int skill, dam;

	if ((skill = get_skill(ch,gsn_headbutt)) == 0)
		return send_to_char("Huh?\n\r",ch);

	if ((victim = ch->fighting) == NULL)
		return send_to_char("You're not fighting anyone!\n\r",ch);

	if (ch->size < (victim->size - 1))
		return send_to_char("They're too large to headbutt.\n\r",ch);

	if (ch->size > (victim->size + 1))
		return send_to_char("They're too small to headbutt.\n\r",ch);

	if (skill > (number_percent() + 5)) {
		act("You slam your forehead into $N with bone-jarring force!",ch,0,victim,TO_CHAR);
		act("$n brings $s head crashing into yours with a ferocious headbutt!",ch,0,victim,TO_VICT);
		act("$n slams $s forehead into $N with bone-jarring force!",ch,0,victim,TO_NOTVICT);
		
		dam = dice(ch->level/2,3);
			
		damage_new(ch,victim,dam,gsn_headbutt,DAM_BASH,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"headbutt");

		if (number_percent() < 3 && !IS_SET(victim->imm_flags,IMM_BASH
			&& !IS_SET(victim->imm_flags,IMM_SLEEP))) {
			act("With the impact, everything suddenly goes black....",victim,0,0,TO_CHAR);
			act("$n staggers back from the collision and then collapses in a heap!",victim,0,0,TO_ROOM);
			init_affect(&af);
			af.where		=	TO_AFFECTS;
			af.type			=	gsn_headbutt;
			af.aftype		=	AFT_SKILL;
			af.owner		=	ch;
			af.level		=	ch->level;
			af.duration		=	2;
			SET_BIT(af.bitvector, AFF_SLEEP);
			affect_to_char(victim,&af);
			stop_fighting(victim,TRUE);
			victim->position = POS_SLEEPING;
		}

		damage_new(ch,ch,dam/4,gsn_headbutt,DAM_BASH,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"headbutt");

		if (number_percent() < 2) {
			act("With the impact, everything suddenly goes black....",ch,0,0,TO_CHAR);
			act("$n staggers back from the collision and then collapses in a heap!",ch,0,0,TO_ROOM);
			init_affect(&af);
			af.where		=	TO_AFFECTS;
			af.type			=	gsn_headbutt;
			af.aftype		=	AFT_SKILL;
			af.owner		=	ch;
			af.level		=	ch->level;
			af.duration		=	2;
			SET_BIT(af.bitvector, AFF_SLEEP);
			affect_to_char(ch,&af);
			stop_fighting(ch,TRUE);
			ch->position = POS_SLEEPING;
		}
		LAG_CHAR(victim,PULSE_VIOLENCE);
		check_improve(ch,gsn_headbutt,TRUE,3);
	} else {
		act("You look rather foolish as you attempt to headbutt $N, but miss.",ch,0,victim,TO_CHAR);
		act("$n brings $s head forward in an attempt to headbutt you, but misses.",ch,0,victim,TO_VICT);
		act("$n brings $s head forward in an attempt to headbutt $N, but misses.",ch,0,victim,TO_NOTVICT);
		damage_new(ch,victim,0,gsn_headbutt,DAM_BASH,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"headbutt");
		check_improve(ch,gsn_headbutt,FALSE,3);
	}

	WAIT_STATE(ch,2*PULSE_VIOLENCE);

	return;
}

void do_disengage(CHAR_DATA *ch, char *argument)
{
	if (!ch->fighting)
		return send_to_char("You aren't fighting anyone!\n\r",ch);

	if (ch->fighting->fighting == ch)
		return send_to_char("It's called fleeing.  Look into it.\n\r",ch);
	
	act("You cease attacking $N.",ch,0,ch->fighting,TO_CHAR);
	act("$n ceases attacking you.",ch,0,ch->fighting,TO_VICT);
	act("$n ceases attacking $N.",ch,0,ch->fighting,TO_NOTVICT);

	stop_fighting(ch,FALSE);

	WAIT_STATE(ch,PULSE_VIOLENCE);
	
	return;
}

bool is_wielded(CHAR_DATA *ch,int weapon,int type) {

	OBJ_DATA *wield;
	bool primary = FALSE;

	wield = get_eq_char(ch,WEAR_WIELD);
	if (wield == NULL)
	{
		if (weapon == WEAPON_HAND && (get_eq_char(ch,WEAR_DUAL_WIELD) == NULL))
			return TRUE;
		else
			return FALSE;
	}
	if (wield->value[0] == weapon)
	{
		if (type == WIELD_ONE || type == WIELD_PRIMARY) {
			return TRUE;
		}
		primary = TRUE;
	} else {
		if (type == WIELD_PRIMARY) {
			return FALSE;
		}
	}
	wield = get_eq_char(ch,WEAR_DUAL_WIELD);
	if (wield == NULL)
	{
		return FALSE;
	}
	if (wield->value[0] == weapon)
	{
		if (type == WIELD_TWO && primary) {
			return TRUE;
		}
	} else {
		return FALSE;
	}
	return TRUE;
}
