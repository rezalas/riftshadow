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
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "merc.h"
#include "recycle.h"
#include "tables.h"
#include "lookup.h"
#include "olc.h"
#include "spec.h"
#include "interp.h"

typedef struct multdata MULTDATA;

struct multdata {
   DESCRIPTOR_DATA *des;
};

/* command procedures needed */
DECLARE_DO_FUN(do_rstat		);
DECLARE_DO_FUN(do_mstat		);
DECLARE_DO_FUN(do_ostat		);
DECLARE_DO_FUN(do_astat		);
DECLARE_DO_FUN(do_rset		);
DECLARE_DO_FUN(do_mset		);
DECLARE_DO_FUN(do_oset		);
DECLARE_DO_FUN(do_sset		);
DECLARE_DO_FUN(do_mfind		);
DECLARE_DO_FUN(do_ofind		);
DECLARE_DO_FUN(do_slookup	);
DECLARE_DO_FUN(do_mload		);
DECLARE_DO_FUN(do_oload		);
DECLARE_DO_FUN(do_quit		);
DECLARE_DO_FUN(do_look		);
DECLARE_DO_FUN(do_stand		);
DECLARE_DO_FUN(do_force		);
DECLARE_DO_FUN(do_vmstat        );
DECLARE_DO_FUN(do_vostat        );
DECLARE_DO_FUN(buglist_end_fun	);


DECLARE_MPROG_FUN_PULSE(pulse_prog_area_echo_ward);

void log_naughty args(( CHAR_DATA *ch, char *argument, int logtype));
void do_sgset( CHAR_DATA *ch, char *argument );
char * flag_name_lookup args((long bitv, const struct flag_type *flag_table));
extern bool wizlock;
AREA_DATA *             area_first;

#define GOD_LOG_FILE	"glog.txt"
/*
 * Local functions.
 */
ROOM_INDEX_DATA *	find_location	args( ( CHAR_DATA *ch, char *arg ) );

void do_wiznet( CHAR_DATA *ch, char *argument )
{
    int flag;
    char buf[MAX_STRING_LENGTH];

    if ( argument[0] == '\0' )
    {
      	if (IS_SET(ch->wiznet,WIZ_ON))
      	{
            send_to_char("Signing off of Wiznet.\n\r",ch);
            REMOVE_BIT(ch->wiznet,WIZ_ON);
      	}
      	else
      	{
            send_to_char("Welcome to Wiznet!\n\r",ch);
            SET_BIT(ch->wiznet,WIZ_ON);
      	}
      	return;
    }

    if (!str_prefix(argument,"on"))
    {
	send_to_char("Welcome to Wiznet!\n\r",ch);
	SET_BIT(ch->wiznet,WIZ_ON);
	return;
    }

    if (!str_prefix(argument,"off"))
    {
	send_to_char("Signing off of Wiznet.\n\r",ch);
	REMOVE_BIT(ch->wiznet,WIZ_ON);
	return;
    }

    /* show wiznet status */
    if (!str_prefix(argument,"status"))
    {
	buf[0] = '\0';

	if (!IS_SET(ch->wiznet,WIZ_ON))
	    strcat(buf,"off ");

	for (flag = 0; wiznet_table[flag].name != NULL; flag++)
	    if (IS_SET(ch->wiznet,wiznet_table[flag].flag))
	    {
		strcat(buf,wiznet_table[flag].name);
		strcat(buf," ");
	    }

	strcat(buf,"\n\r");

	send_to_char("Wiznet status:\n\r",ch);
	send_to_char(buf,ch);
	return;
    }

    if (!str_prefix(argument,"show"))
    /* list of all wiznet options */
    {
	buf[0] = '\0';

	for (flag = 0; wiznet_table[flag].name != NULL; flag++)
	{
	    if (wiznet_table[flag].level <= get_trust(ch))
	    {
	    	strcat(buf,wiznet_table[flag].name);
	    	strcat(buf," ");
	    }
	}

	strcat(buf,"\n\r");

	send_to_char("Wiznet options available to you are:\n\r",ch);
	send_to_char(buf,ch);
	return;
    }

    flag = wiznet_lookup(argument);

    if (flag == -1 || get_trust(ch) < wiznet_table[flag].level)
    {
	send_to_char("No such option.\n\r",ch);
	return;
    }

    if (IS_SET(ch->wiznet,wiznet_table[flag].flag))
    {
	sprintf(buf,"You will no longer see %s on wiznet.\n\r",
	        wiznet_table[flag].name);
	send_to_char(buf,ch);
	REMOVE_BIT(ch->wiznet,wiznet_table[flag].flag);
    	return;
    }
    else
    {
    	sprintf(buf,"You will now see %s on wiznet.\n\r",
		wiznet_table[flag].name);
	send_to_char(buf,ch);
    	SET_BIT(ch->wiznet,wiznet_table[flag].flag);
	return;
    }

}

void wiznet(char *string, CHAR_DATA *ch, OBJ_DATA *obj, long flag, long flag_skip, int min_level)
{
    DESCRIPTOR_DATA *d;
    char str[MAX_STRING_LENGTH];
    for ( d = descriptor_list; d != NULL; d = d->next )
    {
        if (d->connected == CON_PLAYING
		&&  d->character
		&&  IS_IMMORTAL(d->character)
		&&  IS_SET(d->character->wiznet,WIZ_ON)
		&&  (!flag || IS_SET(d->character->wiznet,flag))
		&&  (!flag_skip || !IS_SET(d->character->wiznet,flag_skip))
		&&  get_trust(d->character) >= min_level
		&&  d->character != ch)
        {
	    if (IS_SET(d->character->wiznet,WIZ_PREFIX))
	  	send_to_char("--> ",d->character);
	    sprintf(str,"%s%s%s",get_char_color(d->character,"wiznet"),string,END_COLOR(d->character));
            act_new(str,d->character,obj,ch,TO_CHAR,POS_DEAD);
        }
    }

    return;
}

void do_leader( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
	int cres=0;

    if (ch->level < 54 || IS_NPC(ch))
    {
        send_to_char("Huh?\n\r", ch);
        return;
    }

    argument = one_argument( argument, arg1 );

    if ( arg1[0] == '\0' )
    {
        send_to_char( "Syntax: leader <char>\n\r",ch);
        return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
        send_to_char("They aren't playing.\n\r",ch);
        return;
    }

    if (IS_NPC(victim))
    {
	send_to_char("Can't make mobs leaders.\n\r",ch);
	return;
    }

    if (victim->pcdata->induct == CABAL_LEADER)
    {
        send_to_char("Your power to induct has been taken away!\n\r",victim);
        sprintf(buf, "You have taken away %s's power to induct.\n\r",victim->name);
        send_to_char(buf, ch);
        victim->pcdata->induct = 0;
		sprintf(buf,"AUTO: %s removed leadership.\n\r",ch->true_name);
		add_history(NULL,victim,buf);
		return;
    }
    else
    {
        send_to_char("You have been given the power to induct!\n\r",victim);
        sprintf(buf, "You have given %s the power to induct.\n\r",victim->name);
        send_to_char(buf, ch);
        victim->pcdata->induct = CABAL_LEADER;
		update_cskills(victim);
		sprintf(buf,"AUTO: %s gave leadership.\n\r",ch->true_name);
    	add_history(NULL,victim,buf);
		return;
    }
	RS.SQL.Update("players SET induct=%d WHERE name = '%s'", ch->pcdata->induct, ch->true_name);
    return;
}

unsigned int *gDebug; 
void do_smite( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg1[MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) )
    {
	send_to_char( "Mobs can't smite.\n\r", ch );
	return;
    }

    argument = one_argument( argument, arg1 );

    if ( arg1[0] == '\0' )
    {
        send_to_char("Syntax: smite <char>\n\r", ch);
        return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
        send_to_char( "They aren't playing.\n\r", ch );
        return;
    }
	
    if ( IS_NPC(victim) )
    {
	send_to_char( "Trying to smite a mob?\n\r", ch );
	return;
    }

    if (ch == victim)
    {
        send_to_char( "Trying to smite yourself?\n\r", ch );
        return;
    }

    act( "A bolt from the heavens smites $N!", ch, NULL, victim, TO_NOTVICT );
    act( "A bolt from the heavens smites you!", ch, NULL, victim, TO_VICT );
    act( "You smite $N!", ch, NULL, victim, TO_CHAR );
    victim->hit /= 2;
    return;
}


/* Induction in a Cabal. When inducted a player gains skills or spells
specific for that Cabal. You can add or remove abilities very easily by
just following the basic format used here.
-Ceran
*/
void do_induct( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH],arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH], tbuf[MAX_STRING_LENGTH];
    char tstr[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    int cabal;
    int i;
	char query[MSL];
	
    if (IS_NPC(ch))
	return;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ((ch->level < 54 && ch->pcdata->induct != CABAL_LEADER)
	|| IS_NPC(ch) || (ch->cabal == CABAL_HORDE && !IS_IMMORTAL(ch)))
    {
        send_to_char("Huh?\n\r", ch);
        return;
    }

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
		if(get_trust(ch)>53)
		{
			send_to_char("INDUCT:   Members / Max:\n\r",ch);
			send_to_char("--------------------------\n\r",ch);
        	for(i=1; i < MAX_CABAL; i++)
        	{
				sprintf(tbuf,"%d",cabal_max[i]);
            	sprintf(buf, "%-12s %-4d / %s\n\r",
					capitalize(cabal_table[i].name),
					cabal_members[i],(cabal_table[i].max_members != 1) ? tbuf : "none");
				send_to_char(buf,ch);
			}
			send_to_char("--------------------------\n\r",ch);
        	send_to_char( "Syntax: induct <char> <cabal name>\n\r",ch);
			return;
		}
		
		buf[0] = '\0';
        send_to_char( "Syntax: induct <char> <cabal name>\n\r",ch);
/*        send_to_char("Valid Cabals are: ",ch);
        for (i = 1; i < (MAX_CABAL -1); i++)
		{
        	sprintf(buf,"%s, ", cabal_table[i].name);
			send_to_char(buf,ch);
		}
        sprintf(buf,"%s.\n\r",cabal_table[MAX_CABAL -1].name);
		send_to_char(buf,ch);
*/
        return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
        send_to_char( "They aren't playing.\n\r", ch );
        return;
    }


    if (IS_NPC(victim) && !IS_IMMORTAL(ch))
        return send_to_char( "Mobs can't be inducted into Cabals.\n\r", ch );


    if (!str_prefix(arg2,"none"))
    {
        if (ch->cabal != victim->cabal && get_trust(ch) < 56)
        {
            send_to_char("You have no power over that person's affiliations!\n\r",ch);
            return;
        }
        else
        {
        	if (!victim->cabal)
            	return send_to_char("They are already not in a cabal.\n\r",ch);

			if (victim != ch)
	    	{
	        	send_to_char("They have been inducted into None.\n\r",ch);
            	send_to_char("You have been inducted into None.\n\r",victim);
	    	}
	    	else
            	send_to_char("You have been inducted into None.\n\r",victim);

		cabal = victim->cabal;
        /* take away cabal skills */
		cabal_members[victim->cabal]--;
        victim->cabal = 0;
		victim->pcdata->cabal_level = 0;
		update_cskills(victim);                                                                                       
		sprintf(buf,"AUTO: Uninducted from %s by %s.\n\r",cabal ? capitalize(cabal_table[cabal].name) : "-",ch->true_name);
		if (!IS_IMMORTAL(victim))
			add_history(NULL,victim,buf);
        return;
        }
    }

    if ( victim->cabal != 0 )
		return send_to_char("That person is already in a cabal!\n\r", ch);

    if ((cabal = cabal_lookup(arg2)) == 0)
		return send_to_char("No such cabal exists.\n\r",ch);

    if ((cabal = cabal_lookup(arg2)) != ch->cabal && get_trust(ch)<54)
    {
        send_to_char("You may only induct into the cabal which you belong.\n\r", ch);
        return;
    }

    if(cabal_table[cabal].max_members>1 && cabal_max[cabal]<=cabal_members[cabal] && get_trust(ch)<54)
    {
		//Is cabal full? If so, so solly cholly.
		send_to_char("That cabal is full.\n\r",ch);
		return;
    }

    sprintf(buf,"%s has been inducted into the %s.", victim->name, cabal_table[cabal].long_name);
    act(buf,victim,0,ch,TO_NOTVICT);
    strcat(buf,"\n\r");
    send_to_char(buf,ch);
    sprintf(buf,"You have been inducted into the %s.\n\r", cabal_table[cabal].long_name);
    send_to_char(buf, victim);
    victim->cabal = cabal;
    sprintf(tstr,"%s into %s.",victim->name, capitalize(cabal_table[cabal].name));
    log_naughty(ch,tstr,5);

	sprintf(buf,"AUTO: Inducted into %s by %s.\n\r",capitalize(cabal_table[cabal].name),ch->true_name);
	if (!IS_IMMORTAL(victim))
		add_history(NULL,victim,buf);

	if (IS_IMMORTAL(ch) && IS_IMMORTAL(victim))
	{
		sprintf(query,"insert into inductions(ch, victim, cabal, ctime, chsite, victimsite) values('%s','%s',%d,%ld,'%s','%s')",
				ch->true_name,
				victim->true_name,
				cabal,
				current_time,
				ch->pcdata->host,
				victim->pcdata->host);
		one_query(query);
	}

    cabal_members[cabal]++;
    if(str_cmp(cabal_table[victim->cabal].extitle,""))
		set_extitle(victim,cabal_table[victim->cabal].extitle);
	victim->pcdata->cabal_level = cabal_table[victim->cabal].start_level;
	update_cskills(victim);
}

/* equips a character */
void do_outfit ( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    int i,body,arms,legs,hands,feet,shield,sn,vnum;
    AFFECT_DATA af;

	if(is_affected(ch, skill_lookup("outfit")))
		return send_to_char("You have outfitted too recently and cannot do so again.\n\r",ch);

	if(ch->level > 25 && !IS_IMMORTAL(ch))
		return send_to_char("You are far too experienced to outfit yourself.\n\r", ch);
	
	ch->pcdata->condition[COND_THIRST] = 1;
	ch->pcdata->condition[COND_HUNGER] = 1;
	if(ch->Class()->GetIndex() == CLASS_SORCERER || ch->Class()->GetIndex() == CLASS_NECROMANCER)
	{
		body = 24570;
		arms = 24572;
		legs = 24574;
		hands = 24576;
		feet = 24578;
		shield = 24580;
	}
	else
	{
		body = 24571;
		arms = 24573;
		legs = 24575;
		hands = 24577;
		feet = 24579;
		shield = 24580;
	}

    if ( ( obj = get_eq_char( ch, WEAR_BODY ) ) == NULL )
    {
	obj = create_object( get_obj_index(body), 0 );
	obj->cost = 0;
      obj_to_char( obj, ch );
      wear_obj( ch, obj, FALSE );
    }
/*
    if ( ( obj = get_eq_char( ch, WEAR_HEAD ) ) == NULL )
    {
	obj = create_object( get_obj_index(3706), 0 );
	obj->cost = 0;
      obj_to_char( obj, ch );
      wear_obj( ch, obj, FALSE );
    }

    if ( ( obj = get_eq_char( ch, WEAR_NECK_1 ) ) == NULL )
    {
	obj = create_object( get_obj_index(3705), 0 );
	obj->cost = 0;
      obj_to_char( obj, ch );
      wear_obj( ch, obj, FALSE );
    }

   if ( ( obj = get_eq_char( ch, WEAR_NECK_2 ) ) == NULL )
    {
	obj = create_object( get_obj_index(3705), 0 );
	obj->cost = 0;
      obj_to_char( obj, ch );
      wear_obj( ch, obj, FALSE );
    } */

    if ( ( obj = get_eq_char( ch, WEAR_LEGS ) ) == NULL )
    {
	obj = create_object( get_obj_index(legs), 0 );
	obj->cost = 0;
      obj_to_char( obj, ch );
      wear_obj( ch, obj, FALSE );
    }

    if ( ( obj = get_eq_char( ch, WEAR_FEET ) ) == NULL )
    {
	obj = create_object( get_obj_index(feet), 0 );
	obj->cost = 0;
      obj_to_char( obj, ch );
      wear_obj( ch, obj, FALSE );
    }

    if ( ( obj = get_eq_char( ch, WEAR_HANDS ) ) == NULL )
    {
	obj = create_object( get_obj_index(hands), 0 );
	obj->cost = 0;
      obj_to_char( obj, ch );
      wear_obj( ch, obj, FALSE );
    }

    if ( ( obj = get_eq_char( ch, WEAR_ARMS ) ) == NULL )
    {
	obj = create_object( get_obj_index(arms), 0 );
	obj->cost = 0;
      obj_to_char( obj, ch );
      wear_obj( ch, obj, FALSE );
    }

/*
    if ( ( obj = get_eq_char( ch, WEAR_ABOUT ) ) == NULL )
    {
	obj = create_object( get_obj_index(3711), 0 );
	obj->cost = 0;
      obj_to_char( obj, ch );
      wear_obj( ch, obj, FALSE );
    }

    if ( ( obj = get_eq_char( ch, WEAR_WAIST ) ) == NULL )
    {
	obj = create_object( get_obj_index(3712), 0 );
	obj->cost = 0;
      obj_to_char( obj, ch );
      wear_obj( ch, obj, FALSE );
    }

    if ( ( obj = get_eq_char( ch, WEAR_WRIST_L ) ) == NULL )
    {
	obj = create_object( get_obj_index(3713), 0 );
	obj->cost = 0;
      obj_to_char( obj, ch );
      wear_obj( ch, obj, FALSE );
    }

    if ( ( obj = get_eq_char( ch, WEAR_WRIST_R ) ) == NULL )
    {
	obj = create_object( get_obj_index(3713), 0 );
	obj->cost = 0;
      obj_to_char( obj, ch );
      wear_obj( ch, obj, FALSE );
    }
*/
    /* do the weapon thing*/
    if ((obj = get_eq_char(ch,WEAR_WIELD)) == NULL)
    {
    	sn = 0;
    	vnum = OBJ_VNUM_SCHOOL_SWORD;

    	for (i = 0; weapon_table[i].name != NULL; i++)
    	{
	    if (ch->Class()->GetIndex() == CLASS_WARRIOR && ch->pcdata->start_weapon == i)
		vnum = weapon_table[i].vnum;
	    else if (ch->pcdata->learned[sn] <
		ch->pcdata->learned[*weapon_table[i].gsn])
	    {
	    	sn = *weapon_table[i].gsn;
	    	vnum = weapon_table[i].vnum;
	    }
    	}

    	obj = create_object(get_obj_index(vnum),0);
     	obj_to_char(obj,ch);
    	wear_obj(ch,obj,FALSE);
    }

    if (((obj = get_eq_char(ch,WEAR_WIELD)) == NULL
    ||   !IS_WEAPON_STAT(obj,WEAPON_TWO_HANDS))
    &&  (obj = get_eq_char( ch, WEAR_LIGHT ) ) == NULL
    &&  (obj = get_eq_char( ch, WEAR_SHIELD ) ) == NULL )
    {
        obj = create_object( get_obj_index(OBJ_VNUM_SCHOOL_SHIELD), 0 );
	  	obj->cost = 0;
        obj_to_char( obj, ch );
        wear_obj( ch, obj, FALSE );
    }

/*	for(i=24580; i < 24590; i++)
	{
		obj = create_object(get_obj_index( i ), 0);
		obj->cost = 0;
		obj_to_char( obj, ch);
	}
*/
	obj = create_object(get_obj_index( 311 ), 0);
	obj->cost = 0;
	obj_to_char( obj, ch);

	obj = create_object(get_obj_index( 2794 ), 0);
	obj->cost = 0;
	obj_to_char( obj, ch );

	obj = create_object(get_obj_index( 24534 ), 0);
	obj->cost = 0;
	obj_to_char( obj, ch );
	
    send_to_char("You have been equipped by the gods.\n\r",ch);
	init_affect(&af);
	af.where	=TO_AFFECTS;
	af.aftype 	= AFT_POWER;
	af.type		= skill_lookup("outfit");
	af.location 	= 0;
	af.duration 	= 80;
	SET_BIT(af.bitvector, AFF_DARK_VISION);
	af.modifier = 0;
	af.level=ch->level;
	af.mod_name = MOD_WARDROBE;
	affect_to_char(ch,&af);
}

/* RT nochannels command, for those spammers */
void do_nochannels( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH], buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
        send_to_char( "Nochannel whom?\n\r", ch );
        return;
    }
    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }

    if ( ch->level <= victim->level )
    {
        send_to_char( "You failed.\n\r", ch );
        return;
    }

    if ( IS_SET(victim->comm, COMM_NOCHANNELS) )
    {
        REMOVE_BIT(victim->comm, COMM_NOCHANNELS);
        send_to_char( "The gods have restored your channel priviliges.\n\r",
		      victim );
        send_to_char( "NOCHANNELS removed.\n\r", ch );
	sprintf(buf,"$N restores channels to %s",victim->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }
    else
    {
        SET_BIT(victim->comm, COMM_NOCHANNELS);
        send_to_char( "The gods have revoked your channel priviliges.\n\r",
		       victim );
        send_to_char( "NOCHANNELS set.\n\r", ch );
	sprintf(buf,"$N revokes %s's channels.",victim->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }

    return;
}



void do_smote(CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    char *letter,*name;
    char last[MAX_INPUT_LENGTH], temp[MAX_STRING_LENGTH];
    unsigned int matches = 0;

    if ( !IS_NPC(ch) && IS_SET(ch->comm, COMM_NOEMOTE) )
    {
        send_to_char( "You can't show your emotions.\n\r", ch );
        return;
    }

    if ( argument[0] == '\0' )
    {
        send_to_char( "Emote what?\n\r", ch );
        return;
    }

    if (strstr(argument,ch->name) == NULL && !IS_NPC(ch))
		return send_to_char("You must include your name in an smote.\n\r",ch);

    send_to_char(argument,ch);
    send_to_char("\n\r",ch);

    for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
    {
        if (vch->desc == NULL || vch == ch)
            continue;

        if ((letter = strstr(argument,vch->name)) == NULL)
        {
	    send_to_char(argument,vch);
	    send_to_char("\n\r",vch);
            continue;
        }

        strcpy(temp,argument);
        temp[strlen(argument) - strlen(letter)] = '\0';
        last[0] = '\0';
        name = vch->name;

        for (; *letter != '\0'; letter++)
        {
            if (*letter == '\'' && matches == strlen(vch->name))
            {
                strcat(temp,"r");
                continue;
            }

            if (*letter == 's' && matches == strlen(vch->name))
            {
                matches = 0;
                continue;
            }

            if (matches == strlen(vch->name))
            {
                matches = 0;
            }

            if (*letter == *name)
            {
                matches++;
                name++;
                if (matches == strlen(vch->name))
                {
                    strcat(temp,"you");
                    last[0] = '\0';
                    name = vch->name;
                    continue;
                }
                strncat(last,letter,1);
                continue;
            }

            matches = 0;
            strcat(temp,last);
            strncat(temp,letter,1);
            last[0] = '\0';
            name = vch->name;
        }

	send_to_char(temp,vch);
	send_to_char("\n\r",vch);
    }

    return;
}

void do_bamfin( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    if ( !IS_NPC(ch) )
    {
	smash_tilde( argument );

	if (argument[0] == '\0')
	{
	    sprintf(buf,"Your poofin is %s\n\r",ch->pcdata->bamfin);
	    send_to_char(buf,ch);
	    return;
	}

	if ( strstr(argument,ch->name) == NULL)
	{
	    send_to_char("You must include your name.\n\r",ch);
	    return;
	}
	
	free_pstring( ch->pcdata->bamfin );
	ch->pcdata->bamfin = palloc_string( argument );

        sprintf(buf,"Your poofin is now %s\n\r",ch->pcdata->bamfin);
        send_to_char(buf,ch);
    }
    return;
}



void do_bamfout( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    if ( !IS_NPC(ch) )
    {
        smash_tilde( argument );

        if (argument[0] == '\0')
        {
            sprintf(buf,"Your poofout is %s\n\r",ch->pcdata->bamfout);
            send_to_char(buf,ch);
            return;
        }

        if ( strstr(argument,ch->name) == NULL)
        {
            send_to_char("You must include your name.\n\r",ch);
            return;
        }

        free_pstring( ch->pcdata->bamfout );
        ch->pcdata->bamfout = palloc_string( argument );

        sprintf(buf,"Your poofout is now %s\n\r",ch->pcdata->bamfout);
        send_to_char(buf,ch);
    }
    return;
}



void do_deny( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH],buf[MAX_STRING_LENGTH], *cname;
    char query[MSL];
    CHAR_DATA *victim;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Deny whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    SET_BIT(victim->act, PLR_DENY);
    send_to_char( "You are denied access!\n\r", victim );
    sprintf(buf,"$N denies access to %s",victim->name);
    wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    send_to_char( "OK.\n\r", ch );
    save_char_obj(victim);
    stop_fighting(victim,TRUE);
    victim->pause = 0;

    //add to denial #
    if(victim->desc)
    {
    	sprintf(query,"UPDATE sitetracker SET denials=denials+1 WHERE '%s' RLIKE site_name",victim->desc->host);
    	one_query(query);
    }
    sprintf(buf,"AUTO: Denied by %s.\n\r",ch->true_name);
    add_history(NULL,victim,buf);
    cname	= palloc_string(victim->true_name);
    do_quit_new(victim,"",TRUE);
    delete_char(cname,TRUE);
    return;
}



void do_disconnect( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    DESCRIPTOR_DATA *d;
    CHAR_DATA *victim;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Disconnect whom?\n\r", ch );
	return;
    }

    if (is_number(arg))
    {
	int desc;

	desc = atoi(arg);
    	for ( d = descriptor_list; d != NULL; d = d->next )
    	{
            if ( d->descriptor == desc )
            {
            	close_socket( d );
            	send_to_char( "Ok.\n\r", ch );
            	return;
            }
	}
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
	if(ch->level <= get_trust(victim))
		return send_to_char("Not at your level!\n\r",ch);

    if ( victim->desc == NULL )
    {
	act( "$N doesn't have a descriptor.", ch, NULL, victim, TO_CHAR );
	return;
    }

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d == victim->desc )
	{
	    close_socket( d );
	    send_to_char( "Ok.\n\r", ch );
	    return;
	}
    }

    bug( "Do_disconnect: desc not found.", 0 );
    send_to_char( "Descriptor not found!\n\r", ch );
    return;
}



void do_pardon( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Syntax: pardon <character> <killer|thief>.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "killer" ) )
    {
	if ( IS_SET(victim->act, PLR_KILLER) )
	{
	    REMOVE_BIT( victim->act, PLR_KILLER );
	    send_to_char( "Killer flag removed.\n\r", ch );
	    send_to_char( "You are no longer a KILLER.\n\r", victim );
	}
	return;
    }

    if ( !str_cmp( arg2, "thief" ) )
    {
	if ( IS_SET(victim->act, PLR_THIEF) )
	{
	    REMOVE_BIT( victim->act, PLR_THIEF );
	    send_to_char( "Thief flag removed.\n\r", ch );
	    send_to_char( "You are no longer a THIEF.\n\r", victim );
	}
	return;
    }

    send_to_char( "Syntax: pardon <character> <killer|thief>.\n\r", ch );
    return;
}



void do_echo( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    char buffer[MAX_STRING_LENGTH*2];

    if ( argument[0] == '\0' )
    {
	send_to_char( "Global echo what?\n\r", ch );
	return;
    }

    for ( d = descriptor_list; d; d = d->next )
    {
	if ( d->connected == CON_PLAYING )
	{
	    colorconv(buffer, argument, d->character);
	    if (get_trust(d->character) >= get_trust(ch))
	    {
		send_to_char(ch->name,d->character);
		send_to_char( " globals: ",d->character);
	    }
	    send_to_char( buffer, d->character );
	    send_to_char( "\n\r",   d->character );
	}
    }

    return;
}

void do_immecho( CHAR_DATA *ch, char *argument )
{
	DESCRIPTOR_DATA *d;
	char buffer[MAX_STRING_LENGTH*2];

	if ( argument[0] == '\0' )
		return send_to_char( "Global echo what?\n\r", ch );

	for ( d = descriptor_list; d; d = d->next )
	{
		if ( d->connected == CON_PLAYING && (d->character->level > 51))
		{
			colorconv(buffer, argument, d->character);
			send_to_char( buffer, d->character );
			send_to_char( "\n\r",   d->character );
		}
	}
	return;
}

void do_recho( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    char buffer[MAX_STRING_LENGTH*2];

    if ( argument[0] == '\0' )
    {
	send_to_char( "Local echo what?\n\r", ch );

	return;
    }

    for ( d = descriptor_list; d; d = d->next )
    {
	if ( d->connected == CON_PLAYING
	&&   d->character->in_room == ch->in_room )
	{
            colorconv(buffer, argument, d->character);
            if (get_trust(d->character) >= get_trust(ch) && !IS_NPC(ch))
                send_to_char( "local> ",d->character);
	    send_to_char( buffer, d->character );
	    send_to_char( "\n\r",   d->character );
	}
    }

    return;
}

void do_zecho(CHAR_DATA *ch, char *argument)
{
	char buffer[MAX_STRING_LENGTH*2];
    DESCRIPTOR_DATA *d;

    if (argument[0] == '\0')
    {
	send_to_char("Zone echo what?\n\r",ch);
	return;
    }

    for (d = descriptor_list; d; d = d->next)
    {
	if (d->connected == CON_PLAYING
	&&  d->character->in_room != NULL && ch->in_room != NULL
	&&  d->character->in_room->area == ch->in_room->area)
	{
		colorconv(buffer, argument, d->character);
	    if (get_trust(d->character) >= get_trust(ch) && !IS_NPC(ch))
		send_to_char("zone> ",d->character);
	    send_to_char(buffer,d->character);
	    send_to_char("\n\r",d->character);
	}
    }
}

void do_pecho( CHAR_DATA *ch, char *argument )
{
	char buffer[MAX_STRING_LENGTH*2];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    argument = one_argument(argument, arg);

    if ( argument[0] == '\0' || arg[0] == '\0' )
    {
	send_to_char("Personal echo what?\n\r", ch);
	return;
    }

    if  ( (victim = get_char_world(ch, arg) ) == NULL )
    {
	send_to_char("Target not found.\n\r",ch);
	return;
    }


    if (get_trust(victim) >= get_trust(ch) && get_trust(ch) != MAX_LEVEL)
        send_to_char( "personal> ",victim);
	colorconv(buffer, argument, victim);
    send_to_char(buffer,victim);
    send_to_char("\n\r",victim);
	colorconv(buffer, argument, ch);
    send_to_char( "personal> ",ch);
    send_to_char(buffer,ch);
    send_to_char("\n\r",ch);
}


ROOM_INDEX_DATA *find_location( CHAR_DATA *ch, char *arg )
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;

    if ( is_number(arg) )
	return get_room_index( atoi( arg ) );

    if ( ( victim = get_char_world( ch, arg ) ) != NULL )
	return victim->in_room;

    if ( ( obj = get_obj_world( ch, arg ) ) != NULL )
	return obj->in_room;

    return NULL;
}



void do_transfer( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *location;
    DESCRIPTOR_DATA *d;
    CHAR_DATA *victim;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' )
    {
	send_to_char( "Transfer whom (and where)?\n\r", ch );
	return;
    }

    if ( !str_cmp( arg1, "all" ) )
    {
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( d->connected == CON_PLAYING
	    &&   d->character != ch
	    &&   d->character->in_room != NULL
	    &&   can_see( ch, d->character ) )
	    {
		char buf[MAX_STRING_LENGTH];
		sprintf( buf, "%s %s", d->character->name, arg2 );
		do_transfer( ch, buf );
	    }
	}
	return;
    }

    /*
     * Thanks to Grodyn for the optional location parameter.
     */
    if ( arg2[0] == '\0' )
    {
	location = ch->in_room;
    }
    else
    {
	if ( ( location = find_location( ch, arg2 ) ) == NULL )
	{
	    send_to_char( "No such location.\n\r", ch );
	    return;
	}
    if (IS_SET(location->room_flags, ROOM_IMP_ONLY) && ch->level<60)
    {
	send_to_char("Access denied.\n\r",ch);
	return;
    }

	if ( !is_room_owner(ch,location) && room_is_private( location )
	&&  ch->level < MAX_LEVEL)
	{
	    send_to_char( "Access denied.\n\r", ch );
	    return;
	}
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim->in_room == NULL )
    {
	send_to_char( "They are in limbo.\n\r", ch );
	return;
    }

    if (get_trust(victim) >= get_trust(ch) && !IS_NPC(victim) && ch->level!=60)
    {
	send_to_char( "They are too high for you to mess with.\n\r", ch );
	return;
    }

    if ( victim->fighting != NULL )
	stop_fighting( victim, TRUE );
    act( "$n disappears in a mushroom cloud.", victim, NULL, NULL, TO_ROOM );
    char_from_room( victim );
    char_to_room( victim, location );
    act( "$n arrives from a puff of smoke.", victim, NULL, NULL, TO_ROOM );
    if ( ch != victim )
	act( "$n has transferred you.", ch, NULL, victim, TO_VICT );
    do_look( victim, "auto" );
    send_to_char( "Ok.\n\r", ch );
}



void do_at( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *location;
    ROOM_INDEX_DATA *original;
    OBJ_DATA *on;
    CHAR_DATA *wch;

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "At where what?\n\r", ch );
	return;
    }

    if ( ( location = find_location( ch, arg ) ) == NULL )
    {
	send_to_char( "No such location.\n\r", ch );
	return;
    }

    if (!is_room_owner(ch,location) && room_is_private( location )
    &&  ch->level < MAX_LEVEL)
    {
	send_to_char( "Access denied.\n\r", ch );
	return;
    }
    if (IS_SET(location->room_flags, ROOM_IMP_ONLY) && ch->level<60)
    {
	send_to_char("Access denied.\n\r",ch);
	return;
    }
    original = ch->in_room;
    on = ch->on;
    char_from_room( ch );
    char_to_room( ch, location );
    interpret( ch, argument );

    /*
     * See if 'ch' still exists before continuing!
     * Handles 'at XXXX quit' case.
     */
    for ( wch = char_list; wch != NULL; wch = wch->next )
    {
	if ( wch == ch )
	{
	    char_from_room( ch );
	    char_to_room( ch, original );
	    ch->on = on;
	    break;
	}
    }

    return;
}



void do_goto( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *location;
    CHAR_DATA *rch;
    int count = 0;

    if ( argument[0] == '\0' )
    {
	send_to_char( "Goto where?\n\r", ch );
	return;
    }

    if ( ( location = find_location( ch, argument ) ) == NULL )
    {
	send_to_char( "No such location.\n\r", ch );
	return;
    }

    count = 0;
    for ( rch = location->people; rch != NULL; rch = rch->next_in_room )
        count++;

    if (!is_room_owner(ch,location) && room_is_private(location)
    &&  (count > 1 || ch->level < MAX_LEVEL))
    {
	send_to_char( "Access denied.\n\r", ch );
	return;
    }
    if (IS_SET(location->room_flags, ROOM_IMP_ONLY) && get_trust(ch)<60)
    {
	send_to_char("Access denied.\n\r",ch);
	return;
    }
    if ( ch->fighting != NULL )
	stop_fighting( ch, TRUE );

    for (rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room)
    {
	if (get_trust(rch) >= ch->invis_level)
	{
		if (get_trust(rch) == LEVEL_HERO && ch->invis_level == LEVEL_HERO && !IS_HEROIMM(rch))
			continue;
	    if (ch->pcdata != NULL && ch->pcdata->bamfout[0] != '\0' && !IS_SWITCHED(ch))
		act("$t",ch,ch->pcdata->bamfout,rch,TO_VICT);
	    else
		act("$n leaves in a swirling mist.",ch,NULL,rch,TO_VICT);
	}
    }

    char_from_room( ch );
    char_to_room( ch, location );


    for (rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room)
    {
        if (get_trust(rch) >= ch->invis_level)
        {
		if (get_trust(rch) == LEVEL_HERO && ch->invis_level == LEVEL_HERO && !IS_HEROIMM(rch))
			continue;
            if (ch->pcdata != NULL && ch->pcdata->bamfin[0] != '\0' && !IS_SWITCHED(ch))
                act("$t",ch,ch->pcdata->bamfin,rch,TO_VICT);
            else
                act("$n appears in a swirling mist.",ch,NULL,rch,TO_VICT);
        }
    }

    do_look( ch, "auto" );
    return;
}

void do_violate( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *location;
    CHAR_DATA *rch;

    if ( argument[0] == '\0' )
    {
        send_to_char( "Goto where?\n\r", ch );
        return;
    }

    if ( ( location = find_location( ch, argument ) ) == NULL )
    {
        send_to_char( "No such location.\n\r", ch );
        return;
    }

    if (!room_is_private( location ))
    {
        send_to_char( "That room isn't private, use goto.\n\r", ch );
        return;
    }

    if ( ch->fighting != NULL )
        stop_fighting( ch, TRUE );

    for (rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room)
    {
        if (get_trust(rch) >= ch->invis_level)
        {
		if (get_trust(rch) == LEVEL_HERO && ch->invis_level == LEVEL_HERO && !IS_HEROIMM(rch))
			continue;
            if (ch->pcdata != NULL && ch->pcdata->bamfout[0] != '\0')
                act("$t",ch,ch->pcdata->bamfout,rch,TO_VICT);
            else
                act("$n leaves in a swirling mist.",ch,NULL,rch,TO_VICT);
        }
    }

    char_from_room( ch );
    char_to_room( ch, location );


    for (rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room)
    {
        if (get_trust(rch) >= ch->invis_level)
        {
            if (ch->pcdata != NULL && ch->pcdata->bamfin[0] != '\0')
                act("$t",ch,ch->pcdata->bamfin,rch,TO_VICT);
            else
                act("$n appears in a swirling mist.",ch,NULL,rch,TO_VICT);
        }
    }

    do_look( ch, "auto" );
    return;
}

/* RT to replace the 3 stat commands */

void do_stat ( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   char *string;
   OBJ_DATA *obj;
   ROOM_INDEX_DATA *location;
   CHAR_DATA *victim;

   string = one_argument(argument, arg);
   if ( arg[0] == '\0')
   {
	send_to_char("Syntax:\n\r",ch);
	send_to_char("  stat <name>\n\r",ch);
	send_to_char("  stat obj <name>\n\r",ch);
	send_to_char("  stat mob <name>\n\r",ch);
 	send_to_char("  stat room <number>\n\r",ch);
	send_to_char("  stat form <name>\n\r",ch);
	send_to_char("  stat area\n\r",ch);
	return;
   }

   if (!str_cmp(arg,"room"))
   {
	do_rstat(ch,string);
	return;
   }

   if (!str_cmp(arg,"area"))
   {
	   do_astat(ch,string);
	   return;
   }

   if (!str_cmp(arg,"obj"))
   {
	do_ostat(ch,string);
	return;
   }

   if(!str_cmp(arg,"char")  || !str_cmp(arg,"mob"))
   {
	do_mstat(ch,string);
	return;
   }
   /* do it the old way */
  victim = get_char_world(ch,argument);
  if (victim != NULL)
  {
    do_mstat(ch,argument);
    return;
  }

   obj = get_obj_world(ch,argument);
   if (obj != NULL)
   {
     do_ostat(ch,argument);
     return;
   }

  location = find_location(ch,argument);
  if (location != NULL)
  {
    do_rstat(ch,argument);
    return;
  }

  send_to_char("Nothing by that name found anywhere.\n\r",ch);
}





void do_rstat( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *location;
    OBJ_DATA *obj;
    CHAR_DATA *rch;
	char *direction;
    int door, i = 0, counter = 0, time = 0;

    one_argument( argument, arg );
    location = ( arg[0] == '\0' ) ? ch->in_room : find_location( ch, arg );
    if ( location == NULL )
    {
	send_to_char( "No such location.\n\r", ch );
	return;
    }

    if (!is_room_owner(ch,location) && ch->in_room != location
    &&  room_is_private( location ) && !IS_TRUSTED(ch,IMPLEMENTOR))
    {
	send_to_char( "That room is private right now.\n\r", ch );
	return;
    }

    sprintf( buf, "Name: '%s'\n\rArea: '%s'\n\r",
	location->name,
	location->area->name );
    send_to_char( buf, ch );

    sprintf( buf,
    "Vnum: %d  Sector: %s  Healing: %d%%  mana: %d%%\n\r",
	location->vnum,
	capitalize(sect_table[sect_numlookup(location->sector_type)].name),
	location->heal_rate,
    location->mana_rate );
    send_to_char( buf, ch );

    sprintf( buf, "Description:\n\r%s",
	location->description );
    send_to_char( buf, ch );

    if ( location->extra_descr != NULL )
    {
	EXTRA_DESCR_DATA *ed;

	send_to_char( "Extra description keywords: '", ch );
	for ( ed = location->extra_descr; ed; ed = ed->next )
	{
	    send_to_char( ed->keyword, ch );
	    if ( ed->next != NULL )
		send_to_char( " ", ch );
	}
	send_to_char( "'.\n\r", ch );
    }

    send_to_char( "Characters:", ch );
    for ( rch = location->people; rch; rch = rch->next_in_room )
    {
	if (can_see(ch,rch))
        {
	    send_to_char( " ", ch );
	    one_argument( rch->name, buf );
	    send_to_char( buf, ch );
	}
    }

    send_to_char( ".\n\rObjects:   ", ch );
    for ( obj = location->contents; obj; obj = obj->next_content )
    {
	send_to_char( " ", ch );
	one_argument( obj->name, buf );
	send_to_char( buf, ch );
    }
    send_to_char( ".\n\r", ch );

    for ( door = 0; door <= 5; door++ )
    {
	EXIT_DATA *pexit;
	char ebuf[500];
	int i;
	if ( ( pexit = location->exit[door] ) != NULL )
	{
	    ebuf[0]='\0';
	    for(i=0; exit_flags[i].name!=NULL; i++)
	    {
		if(IS_SET(pexit->exit_info,exit_flags[i].bit))
			{
                                strcat(ebuf," ");
				strcat(ebuf,exit_flags[i].name);
			}
	    }
	    sprintf(buf,"Door: %d.  To: %d.  Key: %d.  Exit flags:%s.\n\rKeyword: '%s'.  Description: %s",
		door,
		(pexit->u1.to_room == NULL ? -1 : pexit->u1.to_room->vnum),
	    	pexit->key,
	    	ebuf,
	    	pexit->keyword,
	    	pexit->description[0] != '\0' ? pexit->description : "(none).\n\r" );
	    send_to_char( buf, ch );
	}
    }

	send_to_char("Characters who have passed through:\n\r",ch);
	for(i=0;i < MAX_TRACKS;i++)
	{
		if (location->tracks[i] && location->tracks[i]->prey)
		{
			time = (time_info.hour >= location->tracks[i]->time.hour) ?
				(time_info.hour - location->tracks[i]->time.hour) :
				(24 +  time_info.hour - location->tracks[i]->time.hour) +
				(time_info.day >= location->tracks[i]->time.day) ?
				24 * (time_info.day - location->tracks[i]->time.day) :
				24 * (30 + time_info.day - location->tracks[i]->time.day) +
				(time_info.month >= location->tracks[i]->time.month) ?
				24*30*(time_info.month - location->tracks[i]->time.month) :
				24*30*(12 + time_info.month - location->tracks[i]->time.month)+
				24*30*12*(time_info.year - location->tracks[i]->time.year);
			direction = (char *)flag_name_lookup(location->tracks[i]->direction,direction_table);
			sprintf(buf,"%s exited to the %s %d hours ago.\n\r",
				location->tracks[i]->prey->name, direction, time);
			send_to_char(buf,ch);
			counter++;
		}
	}

	if (counter == 0)
		send_to_char("None!\n\r",ch);
	
	do_raffects(ch,"");

	return;
}



void do_ostat( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH], arg[MAX_INPUT_LENGTH], buf2[MSL];
    OBJ_AFFECT_DATA *paf;
	OBJ_APPLY_DATA *app;
	AFFECT_DATA *paf2;
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
		return send_to_char( "Stat what?\n\r", ch );

    if ( ( obj = get_obj_world( ch, argument ) ) == NULL || (obj && obj->carried_by && !can_see(ch,obj->carried_by)))
	return send_to_char("Nothing like that in hell, earth, or heaven.\n\r",ch);

    sprintf( buf, "Name(s): %s\n\r", obj->name );
    send_to_char( buf, ch );

    sprintf( buf, "Vnum: %d  Format: %s  Type: %s  Resets: %d\n\r",
	obj->pIndexData->vnum, obj->pIndexData->new_format ? "new" : "old",
        item_name(obj->item_type), obj->pIndexData->reset_num );
    send_to_char( buf, ch );

    sprintf( buf, "Short description: %s\n\rLong description: %s\n\r",
	obj->short_descr, obj->description );
    send_to_char( buf, ch );

	sprintf( buf, "Owner: %s\n\r", obj->owner);
    send_to_char(buf,ch);

    sprintf( buf, "Wear bits: %s\n\rExtra bits: %s\n\r",
	wear_bit_name(obj->wear_flags), extra_bit_name( obj->extra_flags ) );
    send_to_char( buf, ch );

    sprintf( buf, "Restrict_flags: %s\n\r",
	restrict_bit_name(obj->pIndexData->restrict_flags));
    send_to_char(buf,ch);

    sprintf( buf, "Number: %d/%d  Weight: %d/%d/%d pounds  Material: %s  Mat Index: %d\n\r",
	1,           get_obj_number( obj ),
	obj->weight, get_obj_weight( obj ),get_true_weight(obj), obj->material,
	obj->pIndexData->material_index );
    send_to_char( buf, ch );
    sprintf( buf, "Level: %d  Cost: %d  Condition: %d  Timer: %d\n\r",
	obj->level, obj->cost, obj->condition, obj->timer);
    send_to_char( buf, ch );

    sprintf( buf,
	"In room: %d  In object: %s  Carried by: %s  Wear_loc: %d\n\r",
	obj->in_room    == NULL    ?        0 : obj->in_room->vnum,
	obj->in_obj     == NULL    ? "(none)" : obj->in_obj->short_descr,
	obj->carried_by == NULL    ? "(none)" :
	    can_see(ch,obj->carried_by) ? obj->carried_by->name
				 	: "someone",
	obj->wear_loc );
    send_to_char( buf, ch );

    sprintf( buf, "Values: %d %d %d %d %d\n\r",
	obj->value[0], obj->value[1], obj->value[2], obj->value[3],
	obj->value[4] );
    send_to_char( buf, ch );

/* Report object limit and count and say if maxxed */
    sprintf( buf, "COUNT_DATA: Limit is %d, Count is %d -->%s.\n\r",obj->pIndexData->limtotal,obj->pIndexData->limcount,
(obj->pIndexData->limcount < obj->pIndexData->limtotal) ? "Not maxxed" : (obj->pIndexData->limtotal == 0) ? "Non-limited" : "Maxxed");
     send_to_char(buf,ch);


    /* now give out vital statistics as per identify */

    switch ( obj->item_type )
    {
    	case ITEM_SCROLL:
    	case ITEM_POTION:
    	case ITEM_PILL:
	    sprintf( buf, "Level %d spells of:", obj->value[0] );
	    send_to_char( buf, ch );

	    if ( obj->value[1] >= 0 && obj->value[1] < MAX_SKILL )
	    {
	    	send_to_char( " '", ch );
	    	send_to_char( skill_table[obj->value[1]].name, ch );
	    	send_to_char( "'", ch );
	    }

	    if ( obj->value[2] >= 0 && obj->value[2] < MAX_SKILL )
	    {
	    	send_to_char( " '", ch );
	    	send_to_char( skill_table[obj->value[2]].name, ch );
	    	send_to_char( "'", ch );
	    }

	    if ( obj->value[3] >= 0 && obj->value[3] < MAX_SKILL )
	    {
	    	send_to_char( " '", ch );
	    	send_to_char( skill_table[obj->value[3]].name, ch );
	    	send_to_char( "'", ch );
	    }

	    if (obj->value[4] >= 0 && obj->value[4] < MAX_SKILL)
	    {
		send_to_char(" '",ch);
		send_to_char(skill_table[obj->value[4]].name,ch);
		send_to_char("'",ch);
	    }

	    send_to_char( ".\n\r", ch );
	break;

    	case ITEM_WAND:
    	case ITEM_STAFF:
	    sprintf( buf, "Has %d(%d) charges of level %d",
	    	obj->value[1], obj->value[2], obj->value[0] );
	    send_to_char( buf, ch );

	    if ( obj->value[3] >= 0 && obj->value[3] < MAX_SKILL )
	    {
	    	send_to_char( " '", ch );
	    	send_to_char( skill_table[obj->value[3]].name, ch );
	    	send_to_char( "'", ch );
	    }

	    send_to_char( ".\n\r", ch );
	break;

	case ITEM_DRINK_CON:
	    sprintf(buf,"It holds %s-colored %s.\n\r",
		liq_table[obj->value[2]].liq_color,
		liq_table[obj->value[2]].liq_name);
	    send_to_char(buf,ch);
	    break;
		

    	case ITEM_WEAPON:
 	    send_to_char("Weapon type is ",ch);
	    switch (obj->value[0])
	    {
	    	case(WEAPON_EXOTIC):
		    send_to_char("exotic\n\r",ch);
		    break;
	    	case(WEAPON_SWORD):
		    send_to_char("sword\n\r",ch);
		    break;	
	    	case(WEAPON_DAGGER):
		    send_to_char("dagger\n\r",ch);
		    break;
	    	case(WEAPON_SPEAR):
		    send_to_char("spear\n\r",ch);
		    break;
	    	case(WEAPON_MACE):
		    send_to_char("mace\n\r",ch);	
		    break;
	   	case(WEAPON_AXE):
		    send_to_char("axe\n\r",ch);	
		    break;
	    	case(WEAPON_FLAIL):
		    send_to_char("flail\n\r",ch);
		    break;
	    	case(WEAPON_WHIP):
		    send_to_char("whip\n\r",ch);
		    break;
	    	case(WEAPON_POLEARM):
		    send_to_char("polearm\n\r",ch);
		    break;
		case (WEAPON_STAFF):
		send_to_char("staff.\n\r",ch);
		break;
	    	default:
		    send_to_char("unknown\n\r",ch);
		    break;
 	    }
	    if (obj->pIndexData->new_format)
	    	sprintf(buf,"Damage is %dd%d (average %d)\n\r",
		    obj->value[1],obj->value[2],
		    (1 + obj->value[2]) * obj->value[1] / 2);
	    else
	    	sprintf( buf, "Damage is %d to %d (average %d)\n\r",
	    	    obj->value[1], obj->value[2],
	    	    ( obj->value[1] + obj->value[2] ) / 2 );
	    send_to_char( buf, ch );

	    sprintf(buf,"Damage noun is %s.\n\r",
		(obj->value[3] > 0 && obj->value[3] < MAX_DAMAGE_MESSAGE) ?
		    attack_table[obj->value[3]].noun : "undefined");
	    send_to_char(buf,ch);
	
	    if (obj->value[4])  /* weapon flags */
	    {
	        sprintf(buf,"Weapons flags: %s\n\r",
		    weapon_bit_name(obj->value[4]));
	        send_to_char(buf,ch);
            }
	break;

    	case ITEM_ARMOR:
	    sprintf( buf,
	    "Armor class is %d pierce, %d bash, %d slash, and %d vs. magic\n\r",
	        obj->value[0], obj->value[1], obj->value[2], obj->value[3] );
	    send_to_char( buf, ch );
	break;
	case ITEM_CORPSE_NPC:
	case ITEM_CORPSE_PC:
	    sprintf(buf,"Original HP: %d  Owner: %s  Killed by: %s\n\r",
		obj->ohp,
		obj->owner,
		obj->talked);
	    send_to_char(buf,ch);
 	break;
        case ITEM_CONTAINER:
            sprintf(buf,"Capacity: %d#  Maximum weight: %d#  flags: %s\n\r",
                obj->value[0], obj->value[3], cont_bit_name(obj->value[1]));
            send_to_char(buf,ch);
        break;
    }

	if ( obj->pIndexData->notes) {
		sprintf(buf,"NOTES: %s\n\r",obj->pIndexData->notes);
		send_to_char(buf,ch);
	}

    if ( obj->extra_descr != NULL || obj->pIndexData->extra_descr != NULL )
    {
	EXTRA_DESCR_DATA *ed, *ed2;
	bool duplicate = FALSE; /* Don't duplicate extradescs. */

	send_to_char( "Extra description keywords: '", ch );

	for ( ed = obj->extra_descr; ed != NULL; ed = ed->next )
	{
	    send_to_char( ed->keyword, ch );
	    if ( ed->next != NULL )
	    	send_to_char( " ", ch );
	}

	for ( ed = obj->pIndexData->extra_descr; ed != NULL; ed = ed->next )
	{
		for (ed2 = obj->extra_descr; ed2; ed2 = ed2->next)
			if (!str_cmp(ed->keyword,ed2->keyword))
				duplicate = TRUE;

		if (duplicate)
			continue;
		
	    send_to_char( ed->keyword, ch );
	    if ( ed->next != NULL )
			send_to_char( " ", ch );
	}

	send_to_char( "'\n\r", ch );
    }

if ( obj->pIndexData->progtypes != 0 )  {
    if ( IS_SET(obj->progtypes, IPROG_WEAR ) ) {
	sprintf(buf, "Item has wear prog: %s\n\r", obj->pIndexData->iprogs->wear_name);
	send_to_char( buf, ch );
    }
    if ( IS_SET(obj->progtypes, IPROG_REMOVE ) ) {
	sprintf(buf, "Item has remove prog: %s\n\r", obj->pIndexData->iprogs->remove_name);
	send_to_char( buf, ch );
    }
    if ( IS_SET(obj->progtypes, IPROG_GET ) ) {
	sprintf(buf, "Item has get prog: %s\n\r", obj->pIndexData->iprogs->get_name);
	send_to_char( buf, ch );
    }
    if ( IS_SET(obj->progtypes, IPROG_DROP ) ) {
	sprintf(buf, "Item has drop prog: %s\n\r", obj->pIndexData->iprogs->drop_name);
	send_to_char( buf, ch );
    }
    if ( IS_SET(obj->progtypes, IPROG_SAC ) ) {
	sprintf(buf, "Item has sacrifice prog: %s\n\r", obj->pIndexData->iprogs->sac_name);
	send_to_char( buf, ch );
    }
    if ( IS_SET(obj->progtypes, IPROG_GIVE ) ) {
	sprintf(buf, "Item has give prog: %s\n\r", obj->pIndexData->iprogs->give_name);
	send_to_char( buf, ch );
    }
    if ( IS_SET(obj->progtypes, IPROG_FIGHT ) ) {
	sprintf(buf, "Item has fight prog: %s\n\r", obj->pIndexData->iprogs->fight_name);
	send_to_char( buf, ch );
    }
    if ( IS_SET(obj->progtypes, IPROG_DEATH ) ) {
	sprintf(buf, "Item has death prog: %s\n\r", obj->pIndexData->iprogs->death_name);
	send_to_char( buf, ch );
    }
    if ( IS_SET(obj->progtypes, IPROG_SPEECH ) ) {
	sprintf(buf, "Item has speech prog: %s\n\r", obj->pIndexData->iprogs->speech_name);
	send_to_char( buf, ch );
    }
    if ( IS_SET(obj->progtypes, IPROG_PULSE ) ) {
	sprintf(buf, "Item has pulse prog: %s\n\r", obj->pIndexData->iprogs->pulse_name);
	send_to_char( buf, ch );
    }
    if ( IS_SET(obj->progtypes, IPROG_ENTRY ) ) {
        sprintf(buf, "Item has entry prog: %s\n\r", obj->pIndexData->iprogs->entry_name);
        send_to_char( buf, ch );
    }
    if ( IS_SET(obj->progtypes, IPROG_INVOKE) ) {
        sprintf(buf, "Item has invoke prog: %s\n\r", obj->pIndexData->iprogs->invoke_name);
        send_to_char( buf, ch );
    }
    if ( IS_SET(obj->progtypes, IPROG_GREET) ) {
        sprintf(buf, "Item has greet prog: %s\n\r", obj->pIndexData->iprogs->greet_name);
        send_to_char( buf, ch );
    }
	if ( IS_SET(obj->progtypes, IPROG_VERB) ) {
		sprintf(buf, "Item has verb prog: %s\n\r", obj->pIndexData->iprogs->verb_name);
		send_to_char(buf,ch);
	}
}
	if(obj->pIndexData->spec_prog.func)
	{
		int i;
		long x;
		for(i=0; ispec_table[i].spec_name; i++)
			if(ispec_table[i].spec_func == obj->pIndexData->spec_prog.func)
				break;
		if(!ispec_table[i].spec_name)
			return (void)bug("Error: Invalid ispec.",0);
		sprintf(buf,"Item has special prog: %s\n\r", ispec_table[i].spec_name);
		send_to_char(buf,ch);
		send_to_char("Item traps on: ",ch);
		for(x=1; x<1073741824; x = x*2)
			if(ispec_table[i].spec_events & x)
			{
				send_to_char(flag_name_lookup(x, ievent_table),ch);
				send_to_char(" ",ch);
			}
		send_to_char("\n\r",ch);
	}
if(obj->pIndexData->wear_echo[0]!=NULL)
{
	sprintf(buf,"Item echos to wearer: %s\n\r", obj->pIndexData->wear_echo[0]);
	send_to_char(buf,ch);
}
if(obj->pIndexData->wear_echo[1]!=NULL)
{
	sprintf(buf,"Item echos to room on wear: %s\n\r", obj->pIndexData->wear_echo[1]);
	send_to_char(buf,ch);
}
if(obj->pIndexData->remove_echo[0]!=NULL)
{
        sprintf(buf,"Item echos to remover: %s\n\r", obj->pIndexData->remove_echo[0]);
        send_to_char(buf,ch);
}
if(obj->pIndexData->remove_echo[1]!=NULL)
{
        sprintf(buf,"Item echos to room on remove: %s\n\r", obj->pIndexData->remove_echo[1]);
        send_to_char(buf,ch);
}
if(obj->pIndexData->wear_loc_name!=NULL)
{
		sprintf(buf,"Item has cosmetic wear location: worn \"%s\"\n\r",obj->pIndexData->wear_loc_name);
		send_to_char(buf,ch);
}

	for ( app = obj->apply; app; app = app->next) {
		sprintf(buf, "Modifies %s by %d.\n\r",
			affect_loc_name(app->location),	app->modifier );
		send_to_char(buf,ch);
	}

	if ( !IS_ZERO_VECTOR(obj->imm_flags)) {
		sprintf(buf, "Grants immunity to: %s\n\r",imm_bit_name(obj->imm_flags));
		send_to_char(buf,ch);
	}

	if ( !IS_ZERO_VECTOR(obj->res_flags)) {
		sprintf(buf, "Grants resistance to: %s\n\r",
			imm_bit_name(obj->res_flags));
		send_to_char(buf,ch);
	}

	if ( !IS_ZERO_VECTOR(obj->vuln_flags)) {
		sprintf(buf, "Afflicts with vulnerability to: %s\n\r",
			imm_bit_name(obj->vuln_flags));
		send_to_char(buf,ch);
	}
	if(obj->wear_loc_name)
	{
		sprintf(buf,"Item is <worn %s>.\n\r",obj->wear_loc_name);
		send_to_char(buf,ch);
	}
	for ( paf2 = obj->charaffs; paf2; paf2 = paf2->next )
		if (paf2->bitvector) {
			sprintf(buf,"Imbues wearer with %s.\n\r",
				affect_bit_name(paf2->bitvector));
			send_to_char(buf,ch);
		}

    for ( paf = obj->affected; paf != NULL; paf = paf->next )
    {
		if (paf->aftype == AFT_SKILL)
			sprintf(buf2, "Skill");
		else if (paf->aftype == AFT_POWER)
			sprintf(buf2, "Power");
		else if (paf->aftype == AFT_COMMUNE)
			sprintf(buf2, "Commune");
		else if (paf->aftype == AFT_RUNE)
			sprintf(buf2, "Rune");
		else
			sprintf(buf2, "Spell");

		sprintf(buf,
			"%s: '%s' modifies %s by %d for %d%s hours with %s-bits %s, owner %s, level %d.\n\r",
			buf2,
			skill_table[(int)paf->type].name,
			(paf->where == TO_OBJ_AFFECTS) ?
			str_cmp(oaffect_loc_name(paf->location),"none") ?
			oaffect_loc_name(paf->location) : (paf->where == TO_OBJ_APPLY) ?
			affect_loc_name(paf->location) :
			apply_locations[paf->location].name : "none",
			paf->modifier,
			(paf->duration == -1) ? -1 : (paf->duration/2) + 1,
			(paf->duration == 0) ? "" : (paf->duration == -1) ? "" : ".5",
			(paf->where == TO_OBJ_AFFECTS) ? "aff" :
			(paf->where == TO_OBJ_APPLY) ? "apply" : "?",
			oaffect_bit_name(paf->bitvector),
			(paf->owner) ? paf->owner->name : "none",
			paf->level
			);
		send_to_char(buf,ch);
		
    }

    return;
}

void do_astat( CHAR_DATA *ch, char* argument)
{
	char buf[MSL];
	int count = 0;
	AREA_DATA *area, *pArea;
	AREA_AFFECT_DATA *paf;

	area = ch->in_room->area;

	if (!area)
		return send_to_char("Error: No current area (!!)", ch);

	sprintf( buf, "Area Name: %s\n\r", area->name);
	send_to_char(buf,ch);

	sprintf( buf, "Author: %s\n\r", area->credits);
	send_to_char(buf,ch);

	sprintf( buf, "Filename: %s\n\r", area->file_name);
	send_to_char(buf,ch);

	sprintf( buf, "Vnum range: %d to %d\n\r", area->min_vnum, area->max_vnum);
	send_to_char(buf,ch);

	sprintf( buf, "Recommended levels: %d to %d\n\r", area->low_range,
		area->high_range);
	send_to_char(buf,ch);

	sprintf( buf, "Area 'age': %d  [Area repops when age = 30, or 6 if empty.]\n\r", area->age);
	send_to_char(buf,ch);
	
	sprintf( buf, "Number of players in area: %d\n\r", area->nplayer);
	send_to_char(buf,ch);
	
	sprintf( buf, "Climate: %s\n\r", climate_table[area->climate].name);
	send_to_char(buf, ch);

	sprintf( buf, "Current temperature: ");
	send_to_char(buf, ch);
	switch (area->temp) {
		case(TEMP_HOT):
			sprintf( buf, "Hot\n\r");
			break;
		case(TEMP_WARM):
			sprintf( buf, "Warm\n\r");
			break;
		case(TEMP_COOL):
			sprintf( buf, "Cool\n\r");
			break;
		case(TEMP_COLD):
			sprintf( buf, "Cold\n\r");
			break;
		default:
			sprintf( buf, "None (error)\n\r");
	}
	send_to_char(buf, ch);

	sprintf( buf, "Current wind conditions: ");
	send_to_char(buf,ch);
	switch(area->wind) {
		case(WIND_CALM):
			sprintf( buf, "Calm\n\r");
			break;
		case(WIND_BREEZE):
			sprintf( buf, "Breeze\n\r");
			break;
		case(WIND_WINDY):
			sprintf( buf, "Windy\n\r");
			break;
		case(WIND_GALE):
			sprintf( buf, "Gale\n\r");
			break;
		default:
			sprintf( buf, "None (error)\n\r");
	}
	send_to_char(buf, ch);

	sprintf( buf, "Current sky conditions: ");
	send_to_char(buf,ch);
	switch(area->sky) {
		case(SKY_CLEAR):
			sprintf( buf, "Clear\n\r");
			break;
		case(SKY_PCLOUDY):
			sprintf( buf, "Partly cloudy\n\r");
			break;
		case(SKY_OVERCAST):
			sprintf( buf, "Overcast\n\r");
			break;
		case(SKY_DRIZZLE):
			sprintf( buf, "Drizzling\n\r");
			break;
		case(SKY_DOWNPOUR):
			sprintf( buf, "Pouring rain\n\r");
			break;
		case(SKY_TSTORM):
			sprintf( buf, "Thunderstorm\n\r");
			break;
		case(SKY_FLURRY):
			sprintf( buf, "Snow flurries\n\r");
			break;
		case(SKY_BLIZZARD):
			sprintf( buf, "Blizzard\n\r");
			break;
		case(SKY_HAIL):
			sprintf( buf, "Hail\n\r");
			break;
		default:
			sprintf( buf, "None (error)\n\r");
	}
	send_to_char(buf, ch);
		
	send_to_char("Adjacent areas:",ch);

	for (pArea = area_first; pArea; pArea = pArea->next) {
		if (is_adjacent_area(area,pArea)) {
			send_to_char(" ",ch);
			send_to_char(pArea->name,ch);
			count++;
		}
	}

	if (count == 0) {
		send_to_char(" None.\n\r",ch);
	} else {
		send_to_char(".\n\r",ch);
	}

	if (area->affected) {
		send_to_char("The area is affected by:\n\r",ch);
		for (paf = area->affected; paf; paf = paf->next) {
			if (paf->aftype == AFT_SKILL)
				sprintf( buf, "Skill: '%s' ",skill_table[paf->type].name);
			else if (paf->aftype == AFT_POWER)
				sprintf( buf, "Power: '%s' ",skill_table[paf->type].name);
			else if (paf->aftype == AFT_COMMUNE)
				sprintf( buf, "Commune: '%s' ",skill_table[paf->type].name);
			else
				sprintf( buf, "Spell: '%s' ",skill_table[paf->type].name);
			send_to_char(buf,ch);

			sprintf(buf,"modifies %s by %d for %d hours with %s-bits %s, owner %s, level %d.\n\r",
				aaffect_loc_name(paf->location),
				paf->modifier,
				(paf->duration == -1) ? paf->duration : paf->duration/2,
				"flag",
				"NULL",
				paf->owner != NULL ? paf->owner->name : "none",
				paf->level);
			send_to_char(buf,ch);
		}
	} else {
		send_to_char("The area is not affected by anything.\n\r",ch);
	}

	return;
}
	
void do_mstat( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
	char cred[MSL],tbuf[MSL];
	char arg[MAX_INPUT_LENGTH];
    AFFECT_DATA *paf;
    CHAR_DATA *victim;
    ROOM_INDEX_DATA *barred;
    int i, x;
    one_argument( argument, arg );

    if ( arg[0] == '\0' )
		return send_to_char( "Stat whom?\n\r", ch );

    if ( ( victim = get_char_world( ch, argument ) ) == NULL )
		return send_to_char( "They aren't here.\n\r", ch );

    if(IS_NPC(victim))
		sprintf(buf, "Vnum:   %-12d", victim->pIndexData->vnum);
    else if(str_cmp(victim->name, victim->true_name))
		sprintf(buf, "True Name: %-10s",  victim->true_name);
	if(IS_NPC(victim) || str_cmp(victim->name, victim->true_name))
    	send_to_char( buf, ch );
	sprintf( buf, "Name: %s%s (%s)\n\r", !IS_NPC(victim) ? "  " : "", victim->name, IS_NPC(victim) ? "NPC" : "PC");
	send_to_char(buf,ch);
    /* begin shared PC/NPC stuff that fits on one line */

    send_to_char("-----------------------------------------------------------------------------------------\n\r",ch);

    sprintf(buf, "Str:    %2d(%2d)      Int:    %2d(%2d)      Wis:    %2d(%2d)     Dex:  %2d(%2d)    Con:   %2d(%2d)\n\r",
		victim->perm_stat[STAT_STR], get_curr_stat(victim,STAT_STR),
		victim->perm_stat[STAT_INT], get_curr_stat(victim,STAT_INT),
		victim->perm_stat[STAT_WIS], get_curr_stat(victim,STAT_WIS),
		victim->perm_stat[STAT_DEX], get_curr_stat(victim,STAT_DEX),
		victim->perm_stat[STAT_CON], get_curr_stat(victim,STAT_CON));
    send_to_char(buf, ch);

	if(victim->Class()->GetIndex() == CLASS_NECROMANCER)
		sprintf(tbuf," (%d Souls)", victim->pcdata->souls);
	else
		tbuf[0] = '\0';
	
    sprintf(buf, "Level:  %-11d Race:   %-10s  Sex:    %-10s Room: %-9d Class: %s\n\r", victim->level, 
		race_table[victim->race].name, sex_table[victim->sex].name, !victim->in_room ? 0 : victim->in_room->vnum,
		(RSTR)victim->Class()->name);
	send_to_char(buf, ch);
		
   	sprintf(buf, "HP:     %-5d/%-5d Mana:   %-5d/%-5d Move:   %-5d/%-5dHit:  %-9d Dam:   %-11d\n\r",
				victim->hit, victim->max_hit, victim->mana, victim->max_mana, victim->move, victim->max_move,
				GET_HITROLL(victim), GET_DAMROLL(victim));
	send_to_char(buf,ch);

	sprintf(buf, "Align:  %-11s Ethos:  %-10s  Gold:   %-10ld Size: %-9s Pos:   %s\n\r",
		   		victim->alignment > ALIGN_NEUTRAL ? "good" : victim->alignment < ALIGN_NEUTRAL ? "evil" : "neutral",
				IS_NPC(victim) ? "neutral" : victim->pcdata->ethos > ETHOS_NEUTRAL ? "lawful" : 
				victim->pcdata->ethos < ETHOS_NEUTRAL ? "chaotic" : "neutral",
				victim->gold, size_table[victim->size].name, position_table[victim->position].name); 
	send_to_char(buf,ch);

	sprintf(buf, "Saves:  %-11d Dammod: %-8.4f%%   Hunt:   %-10s Fight: %-11s\n\r",
				victim->saving_throw, victim->dam_mod, !victim->hunting ? "(none)" : victim->hunting->name, !victim->fighting ? "(none)"
				: victim->fighting->name);
	send_to_char(buf,ch);
	
	sprintf(buf, "Master: %-11s Leader: %-10s  Pet:    %-10s Reply: %s\n\r",
			victim->master      ? victim->master->name   : "(none)",
			victim->leader      ? victim->leader->name   : "(none)",
			victim->pet         ? victim->pet->name      : "(none)",
			victim->reply       ? victim->reply->name    : "(none)");
    send_to_char( buf, ch );
	
	sprintf(buf, "Pierce: %-10d  Bash:   %-8d    Slash:  %-8d   Exotic: %-10d\n\r",
				GET_AC(victim, AC_PIERCE), GET_AC(victim, AC_BASH), GET_AC(victim, AC_SLASH), GET_AC(victim, AC_EXOTIC));			
	send_to_char(buf,ch);
	
    send_to_char("-----------------------------------------------------------------------------------------\n\r",ch);

	if(IS_NPC(victim))
	{
		sprintf(buf,"Count:  %-10d  XPMod:  %d%%%-5s   Wealth: %-8s   Damage: %2dd%2d+%-1d Attack: %s\n\r",
			victim->pIndexData->count, victim->pIndexData->xp_mod, "", wealth_lookup(victim->pIndexData->wealth),
			victim->damage[DICE_NUMBER],victim->damage[DICE_TYPE], GET_DAMROLL(victim), attack_table[victim->dam_type].noun);
		send_to_char(buf,ch);
		sprintf(buf,"Short:  %-30s  ", victim->short_descr);
		send_to_char(buf,ch);
		if(victim->last_fought != NULL)
		{
        	sprintf(buf,"Tracking: Player %s.  Remaining Timer: %d\n\r",victim->last_fought->name,victim->tracktimer);
	        send_to_char(buf,ch);
		} else
			send_to_char("Tracking: Nobody\n\r",ch);	
	}
	else //player
	{
		sprintf(buf,"Hours:  %-10d  Age:    %-10d  XP:     %-8d   XPTNL:  %-6d  LLevel: %d\n\r",
		(int) (victim->played + current_time - victim->logon) / 3600, get_age(victim),
		victim->exp, victim->level * exp_per_level(victim) - victim->exp, victim->pcdata->last_level);
		send_to_char(buf,ch);
		
		sprintf(buf,"Pracs:  %-10d  Trains: %-10d  Beauty: %-8s   Wimpy:  %-6d  Home:   %s\n\r",
			victim->practice, victim->train, victim->sex == 2 ? beauty_table[victim->pcdata->beauty].female : beauty_table[victim->pcdata->beauty].male,
			victim->wimpy, hometown_table[victim->hometown].name);
		send_to_char(buf,ch);

		sprintf(buf,"Items:  %-10d  Weight: %-10ld  Thirst: %-8d   Hunger: %-6d  Drunk:  %d\n\r",
				victim->carry_number, get_carry_weight(victim), victim->pcdata->condition[COND_THIRST],
				victim->pcdata->condition[COND_HUNGER], victim->pcdata->condition[COND_DRUNK]);
		send_to_char(buf,ch);

		sprintf(cred,"%d", victim->pcdata->bounty_credits);
		sprintf(buf,"Limbs:  %-2d/%-2d       DefMod: %-3d%%        Cabal:  %-9s  CLevel: %-7d %s %s%s%s\n\r",
				victim->arms, victim->legs, victim->defense_mod,
				cabal_table[victim->cabal].name,
				victim->pcdata->cabal_level,
				victim->pcdata->tribe ? "Tribe: " : "",
				victim->pcdata->tribe ?
				tribe_table[victim->pcdata->tribe].name : "",
				(victim->pcdata->bounty_credits > 0) ? "Credits: " : "",
				(victim->pcdata->bounty_credits > 0) ? cred : "");
		send_to_char(buf,ch);
		
		sprintf(buf, "PKDs:   %-4.1f (%-2d)   MobDs:  %-10d  PKWins: %.1f(%-3d)   G/N/E:  %.1f(%-2d)/%.1f(%-2d)/%.1f(%-2d)\n\r",
			victim->pcdata->fragged, victim->pcdata->killed[PK_KILLED], victim->pcdata->killed[MOB_KILLED],
			victim->pcdata->frags[PK_KILLS], victim->pcdata->kills[PK_KILLS], victim->pcdata->frags[PK_GOOD],
			victim->pcdata->kills[PK_GOOD], victim->pcdata->frags[PK_NEUTRAL], victim->pcdata->kills[PK_NEUTRAL],
			victim->pcdata->frags[PK_EVIL], victim->pcdata->kills[PK_EVIL]);
		send_to_char(buf, ch);

		if(victim->ghost > 0)
		{
			sprintf(buf,"Ghost:   %-2d hours\n\r", victim->ghost);
			send_to_char(buf, ch);
		}
		
		if(victim->Class()->GetIndex() == CLASS_ANTI_PALADIN)
		{
			sprintf(buf,"%s\n\r", get_demon_names(victim));
			send_to_char(buf, ch);
		}
	}
	
	send_to_char("-----------------------------------------------------------------------------------------\n\r",ch);

    sprintf(buf, "%s: %s\n\r",IS_NPC(victim) ? "Act" : "Plr", act_bit_name(victim->act));
    send_to_char(buf,ch);

    if (victim->comm[0]) {
        sprintf(buf,"Comm: %s\n\r",comm_bit_name(victim->comm));
    	send_to_char(buf,ch);
    }
	
    if(!IS_NPC(victim) && victim->trust)
    {
	sprintf(buf,"Trust: %s%s%s\n\r",
		IS_SET(victim->pcdata->trust,TRUST_GROUP) ? "group " : "",
		IS_SET(victim->pcdata->trust,TRUST_CABAL) ? "cabal " : "",
		ch->pcdata->trusting ? ch->pcdata->trusting->true_name : "");
	send_to_char(buf,ch);
    }

    if (IS_NPC(victim) && victim->off_flags) {
        sprintf(buf, "Offense: %s\n\r",off_bit_name(victim->off_flags));
		send_to_char(buf,ch);
    }

    if (victim->imm_flags[0]) {
        sprintf(buf, "Immune: %s\n\r",imm_bit_name(victim->imm_flags));
		send_to_char(buf,ch);
    }

    if (victim->res_flags[0]) {
        sprintf(buf, "Resist: %s\n\r", imm_bit_name(victim->res_flags));
		send_to_char(buf,ch);
    }

    if (victim->vuln_flags[0]) {
        sprintf(buf, "Vulnerable: %s\n\r", imm_bit_name(victim->vuln_flags));
		send_to_char(buf,ch);
    }

	if (IS_NPC(victim)) {
    	sprintf(buf, "Form: %s\n\rParts: %s\n\r",
			form_bit_name(victim->form), part_bit_name(victim->parts));
    	send_to_char(buf,ch);
	}

    	if (victim->affected_by[0])
	{
        	sprintf(buf, "Affected by: %s\n\r",
			affect_bit_name(victim->affected_by));
		send_to_char(buf,ch);
    	}


	if (!IS_NPC(victim))
	{
    	sprintf(buf, "Last PC fought: %s, %d seconds ago\n\r",
			victim->last_fight_name!=NULL ? victim->last_fight_name : "(none)",
			victim->last_fight_time ? (int)(current_time -
			victim->last_fight_time) : -1);
		send_to_char(buf,ch);
	}
	if(!IS_NPC(victim) && victim->Class()->GetIndex() == CLASS_SORCERER)
	{
		sprintf(buf,"Major Focus: %s\tPara-Elemental Focus: %s\n\r",
			sphere_table[victim->pcdata->ele_major].name,
			sphere_table[victim->pcdata->ele_para].name);
		send_to_char(buf,ch);
	}
	if(!IS_NPC(victim))
		victim->Profs()->DisplayProfsForStat(ch);
	if(!IS_NPC(victim) && victim->pcdata->styles[0]) {
		sprintf(buf,"Styles: ");
	
		for(i=1; i<MAX_STYLE; i++)
			if(IS_SET(victim->pcdata->styles, style_table[i].bit)) {
				strcat(buf,style_table[i].name);
				strcat(buf," ");
			}

		strcat(buf,"\n\r");
		send_to_char(buf,ch);
	}

	if (IS_NPC(victim) && victim->pIndexData->styles[0]) {
		sprintf(buf,"Styles: ");
		for (i=1; i < MAX_STYLE; i++)
			if (IS_SET(victim->pIndexData->styles, style_table[i].bit)) {
				strcat(buf,style_table[i].name);
				strcat(buf," ");
			}

		strcat(buf,"\n\r");
		send_to_char(buf,ch);
	}

	if (!IS_NPC(victim) && victim->pcdata->security)
	{
		sprintf( buf, "Security: %d\n\r", victim->pcdata->security ); /* OLC */
		send_to_char( buf, ch );
	}
	
	if (IS_NPC(victim) && victim->pIndexData->notes) {
		sprintf(buf,"NOTES: %s\n\r",victim->pIndexData->notes);
		send_to_char(buf,ch);
	}
	if(IS_NPC(victim) && victim->pIndexData->spec_prog.func)
	{
		int i;
		long x;
		for(i=0; mspec_table[i].spec_name; i++)
			if(mspec_table[i].spec_func == victim->pIndexData->spec_prog.func)
				break;
		if(!mspec_table[i].spec_name)
			return (void)bug("Error: Invalid mspec.",0);
		sprintf(buf,"Mob has special prog: %s\n\r", mspec_table[i].spec_name);
		send_to_char(buf,ch);
		send_to_char("Mob traps on: ",ch);
		for(x=1; x<1073741824; x = x*2)
			if(mspec_table[i].spec_events & x)
			{
				send_to_char(flag_name_lookup(x, mevent_table),ch);
				send_to_char(" ",ch);
			}
		send_to_char("\n\r",ch);
	}
	
    if (IS_NPC(victim) && victim->pIndexData->progtypes != 0 )
	{
    if ( IS_SET(victim->progtypes, MPROG_ATTACK ) ) 
	{
        	sprintf(buf, "Mobile has attack prog: %s\n\r",
				victim->pIndexData->mprogs->attack_name);
        	send_to_char( buf, ch );
    }
    if ( IS_SET(victim->progtypes, MPROG_BRIBE ) ) {
        sprintf(buf, "Mobile has bribe prog: %s\n\r", victim->pIndexData->mprogs->bribe_name);
        send_to_char( buf, ch );
    }
    if ( IS_SET(victim->progtypes, MPROG_ENTRY ) ) {
        sprintf(buf, "Mobile has entry prog: %s\n\r", victim->pIndexData->mprogs->entry_name);
        send_to_char( buf, ch );
    }
    if ( IS_SET(victim->progtypes, MPROG_GREET ) ) {
        sprintf(buf, "Mobile has greet prog: %s\n\r", victim->pIndexData->mprogs->greet_name);
        send_to_char( buf, ch );
    }
    if ( IS_SET(victim->progtypes, MPROG_GIVE ) ) {
        sprintf(buf, "Mobile has give prog: %s\n\r", victim->pIndexData->mprogs->give_name);
        send_to_char( buf, ch );
    }
    if ( IS_SET(victim->progtypes, MPROG_FIGHT ) ) {
        sprintf(buf, "Mobile has fight prog: %s\n\r", victim->pIndexData->mprogs->fight_name);
        send_to_char( buf, ch );
    }
    if ( IS_SET(victim->progtypes, MPROG_DEATH ) ) {
        sprintf(buf, "Mobile has death prog: %s\n\r", victim->pIndexData->mprogs->death_name);
        send_to_char( buf, ch );
    }
    if ( IS_SET(victim->progtypes, MPROG_PULSE ) ) {
        sprintf(buf, "Mobile has pulse prog: %s\n\r", victim->pIndexData->mprogs->pulse_name);
        send_to_char( buf, ch );
    }
    if ( IS_SET(victim->progtypes, MPROG_SPEECH ) ) {
        sprintf(buf, "Mobile has speech prog: %s\n\r", victim->pIndexData->mprogs->speech_name);
        send_to_char( buf, ch );
    }
    if ( IS_SET(victim->progtypes, MPROG_MOVE) ) {
        sprintf(buf, "Mobile has movement prog: %s\n\r", victim->pIndexData->mprogs->move_name);
        send_to_char( buf, ch );
    }
    }
    if(IS_NPC(victim) && IS_SET(victim->act, ACT_WARD_MOB) && IS_SET(victim->progtypes, MPROG_PULSE) 
	&& victim->pIndexData->mprogs->pulse_prog == pulse_prog_area_echo_ward)
    {
	char tbuf[MSL];
	colorconv(tbuf,victim->short_descr,ch);
	strcat(tbuf,"\n\r");
	sprintf(buf, "Ward does the following area echo every %d seconds between %d%s and %d%s to the vnum range %d-%d:\n\r%s",
		victim->level, victim->armor[2] % 12 > 0 ? victim->armor[2] % 12 : 12, 
		victim->armor[2] >= 12 ? "pm" : "am", victim->armor[3] % 12 > 0 ? victim->armor[3] % 12 : 12, 
		victim->armor[3] >= 12 ? "pm" : "am", victim->armor[0], victim->armor[1], tbuf);
	send_to_char(buf,ch);
    }
    if(IS_NPC(victim))
	for(i = 0; i < MAX_PROFS_TAUGHT_BY_MOB; i++)
		if(victim->pIndexData->profs_taught[i] > -1)
		{
			sprintf(buf,"Mobile teaches proficiency '%s'.\n\r", prof_table[victim->pIndexData->profs_taught[i]].name);
			send_to_char(buf, ch);
		}
    if(IS_NPC(victim))
    {
    for(i=0;i<=9;i++)
    {
	if(victim->pIndexData->cast_spell[i]==NULL)
		break;
	sprintf(buf,"Mobile casts spell '%s'.\n\r",victim->pIndexData->cast_spell[i]);
	send_to_char(buf,ch);
    }
    }
    if(IS_NPC(victim) && victim->pIndexData->barred_entry)
    {
	barred 	= get_room_index(victim->pIndexData->barred_entry->vnum);
	i	= victim->pIndexData->barred_entry->comparison;
	if(!barred)
		bugout("Error: No room for barred entry data was found.");
	sprintf(buf,"Mobile bars entry to %s (%d) if %s is not %s %d.\n\r",
	barred->name,
	barred->vnum,
	criterion_flags[victim->pIndexData->barred_entry->type].name,
	i == BAR_EQUAL_TO ? "equal to" : i == BAR_LESS_THAN ? "less than" : i == BAR_GREATER_THAN ? "greater than" :"unknown",
	victim->pIndexData->barred_entry->value);
	send_to_char(buf,ch);
	i	= victim->pIndexData->barred_entry->msg_type;
	sprintf(buf,"Mobile forbids access by %s %s%s\n\r",
	i == BAR_SAY ? "saying" : i == BAR_ECHO ? "echoing" : i == BAR_EMOTE ? "emoting" : "unknown",
	victim->pIndexData->barred_entry->message,
	i == BAR_ECHO ? " to the person." : "");
	send_to_char(buf,ch);
	if(i == BAR_ECHO && victim->pIndexData->barred_entry->message_two)
	{
		sprintf(buf,"Mobile forbids access by echoing %s to the room excluding the person.\n\r",
		victim->pIndexData->barred_entry->message_two);
		send_to_char(buf,ch);
	}
    }
    if(IS_NPC(victim) && victim->pIndexData->pShop)
    {
	i = victim->pIndexData->pShop->open_hour;
	x = victim->pIndexData->pShop->close_hour;
	sprintf(buf,"Mob is a shopkeeper that opens at %d%s and closes at %d%s, exiting to the %s.\n\r",
		i > 12 ? i - 12 : i, i > 12 ? "pm" : "am", x > 12 ? x - 12 : x, x > 12 ? "pm" : "am",
		direction_table[victim->pIndexData->pShop->direction].name);
	send_to_char(buf,ch);
    }
	if ( IS_NPC(victim) && victim->pIndexData->restrict_low != -1) {
		sprintf(buf,"Mob will only wander within vnum range %d to %d.\n\r",
			victim->pIndexData->restrict_low,
			victim->pIndexData->restrict_high);
		send_to_char(buf,ch);
	}
		
	for ( paf = victim->affected; paf != NULL; paf = paf->next )
    {
	if (paf->aftype == AFT_SPELL)
		sprintf( buf2, "Spell");
	else if (paf->aftype == AFT_SKILL)
	    	sprintf( buf2, "Skill");
	else if (paf->aftype == AFT_POWER)
	    	sprintf( buf2, "Power");
	else if (paf->aftype == AFT_MALADY)
	    	sprintf( buf2, "Malady");
	else if (paf->aftype == AFT_COMMUNE)
	    	sprintf( buf2, "Commune");
	else if (paf->aftype == AFT_INVIS)
		sprintf( buf2, "Hidden Affect");
	else if (paf->aftype == AFT_RUNE)
		sprintf( buf2, "Rune");
	else if (paf->aftype == AFT_TIMER)
		sprintf( buf2, "Timer");
	else
 		sprintf( buf2, "Spell");
	sprintf( buf,
	"%s: '%s' %s%s%smodifies %s by %d for %d%s hours with %s-bits %s, owner %s, level %d.\n\r",
	buf2,
	skill_table[(int) paf->type].name,
	paf->name ? "(" : "", paf->name ? paf->name : "", paf->name ? ") " : "",
	str_cmp(affect_loc_name( paf->location ), "none") ? affect_loc_name(paf->location) :
		apply_locations[paf->location].name,
	paf->modifier,
	(paf->duration == -1) ? -1 : (paf->duration/2) + 1,
	(paf->duration % 2 == 0) ? "" : (paf->duration == -1) ? "" : ".5",
	paf->where == TO_IMMUNE ? "imm" : paf->where == TO_RESIST ? "res" : paf->where == TO_VULN ? "vuln" : "aff",
	paf->where == TO_IMMUNE || paf->where == TO_RESIST || paf->where == TO_VULN ? imm_bit_name(paf->bitvector) :
	affect_bit_name( paf->bitvector ),
	paf->owner != NULL ? paf->owner->true_name : "none",
	paf->level
	);
	send_to_char( buf, ch );
    }

	if(is_affected(victim,gsn_word_of_command)) {
		sprintf(buf,"*** Will '%s' upon hearing '%s' ***\n\r",
			victim->pcdata->command[1],victim->pcdata->command[0]);
		send_to_char(buf,ch);
	}

	if (!IS_NPC(victim)) {
		for (i=0;i<20;i++) {
			//sprintf(buf,"Queue %d: '%s'.\n\r", i, victim->pcdata->queue[i]);
			//send_to_char(buf,ch);
		}
	}
		
    return;
}

/* ofind and mfind replaced with vnum, vnum skill also added */

void do_vnum(CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    char *string;

    string = one_argument(argument,arg);

    if (arg[0] == '\0')
    {
	send_to_char("Syntax:\n\r",ch);
	send_to_char("  vnum obj <name>\n\r",ch);
	send_to_char("  vnum mob <name>\n\r",ch);
	send_to_char("  vnum skill <skill or spell>\n\r",ch);
	return;
    }

    if (!str_cmp(arg,"obj"))
    {
	do_ofind(ch,string);
 	return;
    }

    if (!str_cmp(arg,"mob") || !str_cmp(arg,"char"))
    {
	do_mfind(ch,string);
	return;
    }

    if (!str_cmp(arg,"skill") || !str_cmp(arg,"spell"))
    {
	do_slookup(ch,string);
	return;
    }
    /* do both */
    do_mfind(ch,argument);
    do_ofind(ch,argument);
}


void do_mfind( CHAR_DATA *ch, char *argument )
{
    extern int top_mob_index;
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    MOB_INDEX_DATA *pMobIndex;
    int vnum;
    int nMatch;
    bool fAll;
    bool found;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Find whom?\n\r", ch );
	return;
    }

    fAll	= FALSE; /* !str_cmp( arg, "all" ); */
    found	= FALSE;
    nMatch	= 0;

    /*
     * Yeah, so iterating over all vnum's takes 10,000 loops.
     * Get_mob_index is fast, and I don't feel like threading another link.
     * Do you?
     * -- Furey
     */
    for ( vnum = 0; nMatch < top_mob_index; vnum++ )
    {
	if ( ( pMobIndex = get_mob_index( vnum ) ) != NULL )
	{
	    nMatch++;
	    if ( fAll || is_name( argument, pMobIndex->player_name ) )
	    {
		found = TRUE;
		sprintf( buf, "[%5d] %s\n\r",
		    pMobIndex->vnum, pMobIndex->short_descr );
		send_to_char( buf, ch );
	    }
	}
    }

    if ( !found )
	send_to_char( "No mobiles by that name.\n\r", ch );

    return;
}



void do_ofind( CHAR_DATA *ch, char *argument )
{
    extern int top_obj_index;
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    OBJ_INDEX_DATA *pObjIndex;
    int vnum;
    int nMatch;
    bool fAll;
    bool found;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Find what?\n\r", ch );
	return;
    }

    fAll	= FALSE; /* !str_cmp( arg, "all" ); */
    found	= FALSE;
    nMatch	= 0;

    /*
     * Yeah, so iterating over all vnum's takes 10,000 loops.
     * Get_obj_index is fast, and I don't feel like threading another link.
     * Do you?
     * -- Furey
     */
    for ( vnum = 0; nMatch < top_obj_index; vnum++ )
    {
	if ( ( pObjIndex = get_obj_index( vnum ) ) != NULL )
	{
	    nMatch++;
	    if ( fAll || is_name( argument, pObjIndex->name ) )
	    {
		found = TRUE;
		sprintf( buf, "[%5d] %s\n\r",
		    pObjIndex->vnum, pObjIndex->short_descr );
		send_to_char( buf, ch );
	    }
	}
    }

    if ( !found )
	send_to_char( "No objects by that name.\n\r", ch );

    return;
}


void do_owhere(CHAR_DATA *ch, char *argument )
{
    char buf[MAX_INPUT_LENGTH];
    BUFFER *buffer;
    OBJ_DATA *obj;
    OBJ_DATA *in_obj;
    bool found;
    int number = 0, max_found;

    found = FALSE;
    number = 0;
    max_found = 200;

    buffer = new_buf();

    if (argument[0] == '\0')
    {
	send_to_char("Find what?\n\r",ch);
	return;
    }

    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
        if ( !can_see_obj( ch, obj ) || !is_name( argument, obj->name )
        ||   ch->level < obj->level)
            continue;

        number++;

        for ( in_obj = obj; in_obj->in_obj != NULL; in_obj = in_obj->in_obj )
            ;

        if ( in_obj->carried_by != NULL && can_see(ch,in_obj->carried_by)
	&&   in_obj->carried_by->in_room != NULL)
            sprintf( buf, "%3d) %s is carried by %s [Room %d]\n\r",
                number, obj->short_descr,PERS(in_obj->carried_by, ch),
		in_obj->carried_by->in_room->vnum );
        else if (in_obj->in_room != NULL && can_see_room(ch,in_obj->in_room))
            sprintf( buf, "%3d) %s is in %s [Room %d]\n\r",
                number, obj->short_descr,get_room_name(in_obj->in_room),
	   	in_obj->in_room->vnum);
	else
	{
		number--;
 		continue;
	}

        found = TRUE;

        buf[0] = UPPER(buf[0]);
        add_buf(buffer,buf);

        if (number >= max_found)
            break;
    }

    if ( !found )
        send_to_char( "Nothing like that in heaven or earth.\n\r", ch );
    else
        page_to_char(buf_string(buffer),ch);

    free_buf(buffer);
}


void do_mwhere( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    BUFFER *buffer;
    CHAR_DATA *victim;
    bool found;
    int count = 0;

    if ( argument[0] == '\0' )
    {
	DESCRIPTOR_DATA *d;

	/* show characters logged */

	buffer = new_buf();
	for (d = descriptor_list; d != NULL; d = d->next)
	{
	    if (d->character != NULL && d->connected == CON_PLAYING
	    &&  d->character->in_room != NULL && can_see(ch,d->character)
	    &&  can_see_room(ch,d->character->in_room))
	    {
		victim = d->character;
		count++;
		if (d->original != NULL)
		    sprintf(buf,"%3d) %s (in the body of %s) is in %s [%d]\n\r",
			count, d->original->true_name,victim->short_descr,
			get_room_name(victim->in_room),victim->in_room->vnum);
		else
		    sprintf(buf,"%3d) %s is in %s [%d]\n\r",
			count, IS_NPC(victim) ? victim->name : victim->true_name,get_room_name(victim->in_room),
			victim->in_room->vnum);
		add_buf(buffer,buf);
	    }
	}

        page_to_char(buf_string(buffer),ch);
	free_buf(buffer);
	return;
    }

    found = FALSE;
    buffer = new_buf();
    for ( victim = char_list; victim != NULL; victim = victim->next )
    {
	if ( victim->in_room != NULL
	&&   can_see(ch,victim)
	&&   is_name( argument, victim->name ) )
	{
	    found = TRUE;
	    count++;
	    sprintf( buf, "%3d) [%5d] %-28s [%5d] %s\n\r", count,
		IS_NPC(victim) ? victim->pIndexData->vnum : 0,
		IS_NPC(victim) ? victim->short_descr : victim->name,
		victim->in_room->vnum,
		get_room_name(victim->in_room));
	    add_buf(buffer,buf);
	}
    }

    if ( !found )
	act( "You didn't find any $T.", ch, NULL, argument, TO_CHAR );
    else
    	page_to_char(buf_string(buffer),ch);

    free_buf(buffer);

    return;
}



void do_reboo( CHAR_DATA *ch, char *argument )
{
    send_to_char( "If you want to REBOOT, spell it out.\n\r", ch );
    send_to_char( "Use: 'reboot nosave', if you don't want players saved at reboot.\n\r",ch);
    return;
}

void reboot_now( CHAR_DATA *ch )
{
	DESCRIPTOR_DATA *d,*d_next;
	extern bool merc_down;
	CHAR_DATA *vch;
//	OBJ_DATA *t_obj, *obj;

	/*
	for ( obj = object_list; obj != NULL; obj = obj->next )
	{
		if(obj->item_type != ITEM_CORPSE_PC || !obj->contains)
			continue;

		for(owner=char_list;owner!=NULL;owner=owner->next)
		{
			if(!IS_NPC(owner) && !str_cmp(owner->true_name,obj->owner))
			{
				send_to_char("Your corpse decays.\n\r",owner);
				for (t_obj = obj->contains; t_obj != NULL; t_obj = t_obj->next_content)
				{
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
				extract_obj(obj);
				break;
			}
		}
	}	*/
	
	do_force(ch, "all save");
	do_echo(ch, "*** REBOOTING NOW ***");
	for ( d = descriptor_list; d != NULL; d = d_next )
	{
		d_next = d->next;
		vch = d->original ? d->original : d->character;
		if (vch != NULL && d->connected == 0)
		{
			if (is_affected(vch,gsn_cloak_form))
				do_cloak(vch,"auto");
			save_char_obj(vch);
		}
		close_socket(d);
	}
	merc_down = TRUE;
	return;
}

void start_reboot( CHAR_DATA *ch)
{
	char buf[MSL];
	extern int reboot_num;
	extern bool rebooting;

	if(reboot_num == -1)
	{
		rebooting = FALSE;
		do_echo(ch,"*** REBOOT CANCELLED ***");
	}
	else if(reboot_num == 0)
		reboot_now(ch);
	else
	{
		if(reboot_num < 6 || (reboot_num % 5 == 0))
		{
			sprintf(buf, "*** REBOOT IN %s MINUTE%s ***", int_to_cap_string(reboot_num), (reboot_num == 1) ? "" : "S");
			do_echo(ch, buf);
			reboot_num--;
			RS.Queue.AddToQueue(60, 1, (void *)start_reboot, ch );
		}
	}
}

void do_reboot( CHAR_DATA *ch, char *argument )
{
	extern bool rebooting;
	extern int reboot_num;
	int mins=0;
	char send[MSL];
	char arg1[MSL],area[MSL]="";
	AREA_DATA *pArea = NULL;
	bool found=FALSE;

	argument = one_argument(argument, arg1);
	mins = atoi(arg1);

	for( pArea = area_first; pArea != NULL; pArea = pArea->next )
		if ( IS_SET(pArea->area_flags, AREA_CHANGED) || IS_SET(pArea->area_flags, AREA_ADDED))
		{
			found=TRUE;
			sprintf(area,"%s%24s\n\r", area, pArea->name);
		}
	
	if((str_cmp(argument,"now") && str_cmp(arg1,"now")) && found)
	{
		send_to_char("The following areas have had changes made and need to be saved.  If you want\n\r"\
					 "to override, specify 'now' after your reboot command.\n\r", ch);
		send_to_char(area, ch);
		return;
	}
	
	if(rebooting && mins != -1)
		return send_to_char("Someone has already started a reboot.\n\r", ch);

	if(!rebooting && mins < 0)
		return send_to_char("There is no reboot in progress to cancel.\n\r", ch);

	rebooting = TRUE;

	if(mins == 0)
	{
		reboot_now(ch);
	}
	else if(mins == -1)
	{
		sprintf(send,"{R%s has cancelled the reboot.{x", ch->true_name);
		do_immecho(ch, send);
		reboot_num = -1;
		rebooting = FALSE;
	}
	else
	{
		reboot_num = mins;
		start_reboot(ch);
	}
}

void do_protect( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;

    if (argument[0] == '\0')
    {
	TOGGLE_BIT(ch->in_room->room_flags, ROOM_IMP_ONLY);
	send_to_char("Ok.\n\r",ch);
	return;
    }

    if ((victim = get_char_world(ch,argument)) == NULL)
    {
	send_to_char("You can't find them.\n\r",ch);
	return;
    }

    if (IS_SET(victim->comm,COMM_SNOOP_PROOF))
    {
	act_new("$N is no longer snoop-proof.",ch,NULL,victim,TO_CHAR,POS_DEAD);
	/*send_to_char("Your snoop-proofing was just removed.\n\r",victim);*/
	REMOVE_BIT(victim->comm,COMM_SNOOP_PROOF);
    }
    else
    {
	act_new("$N is now snoop-proof.",ch,NULL,victim,TO_CHAR,POS_DEAD);
	/*send_to_char("You are now immune to snooping.\n\r",victim);*/
	SET_BIT(victim->comm,COMM_SNOOP_PROOF);
    }
}



void do_snoop( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    DESCRIPTOR_DATA *d;
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Snoop whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim->desc == NULL )
    {
	send_to_char( "No descriptor to snoop.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "Cancelling all snoops.\n\r", ch );
	wiznet("$N stops being such a snoop.",
		ch,NULL,WIZ_SNOOPS,WIZ_SECURE,get_trust(ch));
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( d->snoop_by == ch->desc )
		d->snoop_by = NULL;
	}
	return;
    }

    if ( victim->desc->snoop_by != NULL )
    {
	send_to_char( "Busy already.\n\r", ch );
	return;
    }
    if (IS_SET(victim->in_room->room_flags, ROOM_IMP_ONLY) && ch->level<60)
    {
	send_to_char("Access denied.\n\r",ch);
	return;
    }

    if (!is_room_owner(ch,victim->in_room) && ch->in_room != victim->in_room
    &&  room_is_private(victim->in_room) && ch->level<60)
    {
        send_to_char("That character is in a private room.\n\r",ch);
        return;
    }

    if ( victim->level >= ch->level
    ||   IS_SET(victim->comm,COMM_SNOOP_PROOF))
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( ch->desc != NULL )
    {
	for ( d = ch->desc->snoop_by; d != NULL; d = d->snoop_by )
	{
	    if ( d->character == victim || d->original == victim )
	    {
		send_to_char( "No snoop loops.\n\r", ch );
		return;
	    }
	}
    }

    victim->desc->snoop_by = ch->desc;
    sprintf(buf,"$N starts snooping on %s",
	(IS_NPC(ch) ? victim->short_descr : victim->name));
    wiznet(buf,ch,NULL,WIZ_SNOOPS,WIZ_SECURE,get_trust(ch));
    send_to_char( "Ok.\n\r", ch );
    return;
}



void do_switch( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH], buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Switch into whom?\n\r", ch );
	return;
    }

    if ( ch->desc == NULL )
	return;

    if ( ch->desc->original != NULL )
    {
	send_to_char( "You are already switched.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "Ok.\n\r", ch );
	return;
    }

    if (!IS_NPC(victim))
    {
	send_to_char("You can only switch into mobiles.\n\r",ch);
	return;
    }

    if (!is_room_owner(ch,victim->in_room) && ch->in_room != victim->in_room
    &&  room_is_private(victim->in_room) && !IS_TRUSTED(ch,IMPLEMENTOR))
    {
	send_to_char("That character is in a private room.\n\r",ch);
	return;
    }

    if ( victim->desc != NULL )
    {
	send_to_char( "Character in use.\n\r", ch );
	return;
    }

    sprintf(buf,"$N switches into %s.",victim->short_descr);
    wiznet(buf,ch,NULL,WIZ_SWITCHES,WIZ_SECURE,get_trust(ch));

    ch->desc->character = victim;
    ch->desc->original  = ch;
    victim->desc        = ch->desc;
    victim->pcdata	= ch->pcdata;
    ch->desc            = NULL;
    /* change communications to match */
//    if (ch->prompt != NULL)
//        victim->prompt = palloc_string(ch->prompt);
    copy_vector(victim->comm, ch->comm);
    victim->lines = ch->lines;
    send_to_char( "Ok.\n\r", victim );
    return;
}



void do_return( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    if ( ch->desc == NULL )
	return;

    if ( ch->desc->original == NULL )
    {
	send_to_char( "You aren't switched.\n\r", ch );
	return;
    }

    send_to_char("You return to your original body. Type replay to see any missed tells.\n\r", ch );
    if (ch->prompt != NULL)
    {
	free_pstring(ch->prompt);
	ch->prompt = NULL;
    }

    sprintf(buf,"$N returns from %s.",ch->short_descr);
    wiznet(buf,ch->desc->original,0,WIZ_SWITCHES,WIZ_SECURE,get_trust(ch));
    ch->pcdata		      = NULL;
    ch->desc->character       = ch->desc->original;
    ch->desc->original        = NULL;
    ch->desc->character->desc = ch->desc;
    ch->desc                  = NULL;
    return;
}

/* trust levels for load and clone */
bool obj_check (CHAR_DATA *ch, OBJ_DATA *obj)
{
    if (IS_TRUSTED(ch,GOD)
	|| (IS_TRUSTED(ch,IMMORTAL) && obj->level <= 20 && obj->cost <= 1000)
	|| (IS_TRUSTED(ch,DEMI)	    && obj->level <= 10 && obj->cost <= 500)
	|| (IS_TRUSTED(ch,ANGEL)    && obj->level <=  5 && obj->cost <= 250)
	|| (IS_TRUSTED(ch,AVATAR)   && obj->level ==  0 && obj->cost <= 100))
	return TRUE;
    else
	return FALSE;
}

/* for clone, to insure that cloning goes many levels deep */
void recursive_clone(CHAR_DATA *ch, OBJ_DATA *obj, OBJ_DATA *clone)
{
    OBJ_DATA *c_obj, *t_obj;


    for (c_obj = obj->contains; c_obj != NULL; c_obj = c_obj->next_content)
    {
	if (obj_check(ch,c_obj))
	{
	    t_obj = create_object(c_obj->pIndexData,0);
	    clone_object(c_obj,t_obj);
	    obj_to_obj(t_obj,clone);
	    recursive_clone(ch,c_obj,t_obj);
	}
    }
}

/* command that is similar to load */
void do_clone(CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char *rest;
    CHAR_DATA *mob;
    OBJ_DATA  *obj;

    rest = one_argument(argument,arg);

    if (arg[0] == '\0')
    {
	send_to_char("Clone what?\n\r",ch);
	return;
    }

    if (!str_prefix(arg,"object"))
    {
	mob = NULL;
	obj = get_obj_here(ch,rest);
	if (obj == NULL)
	{
	    send_to_char("You don't see that here.\n\r",ch);
	    return;
	}
    }
    else if (!str_prefix(arg,"mobile") || !str_prefix(arg,"character"))
    {
	obj = NULL;
	mob = get_char_room(ch,rest);
	if (mob == NULL)
	{
	    send_to_char("You don't see that here.\n\r",ch);
	    return;
	}
    }
    else /* find both */
    {
	mob = get_char_room(ch,argument);
	obj = get_obj_here(ch,argument);
	if (mob == NULL && obj == NULL)
	{
	    send_to_char("You don't see that here.\n\r",ch);
	    return;
	}
    }

    /* clone an object */
    if (obj != NULL)
    {
	OBJ_DATA *clone;

	if (!obj_check(ch,obj))
	{
	    send_to_char(
		"Your powers are not great enough for such a task.\n\r",ch);
	    return;
	}

	clone = create_object(obj->pIndexData,0);
	clone_object(obj,clone);
	if (obj->carried_by != NULL)
	    obj_to_char(clone,ch);
	else
	    obj_to_room(clone,ch->in_room);
 	recursive_clone(ch,obj,clone);

	act("$n has created $p.",ch,clone,NULL,TO_ROOM);
	act("You clone $p.",ch,clone,NULL,TO_CHAR);
	wiznet("$N clones $p.",ch,clone,WIZ_LOAD,WIZ_SECURE,get_trust(ch));
	return;
    }
    else if (mob != NULL)
    {
	CHAR_DATA *clone;
	OBJ_DATA *new_obj;
	char buf[MAX_STRING_LENGTH];

	if (!IS_NPC(mob))
	{
	    send_to_char("You can only clone mobiles.\n\r",ch);
	    return;
	}

	if ((mob->level > 20 && !IS_TRUSTED(ch,GOD))
	||  (mob->level > 10 && !IS_TRUSTED(ch,IMMORTAL))
	||  (mob->level >  5 && !IS_TRUSTED(ch,DEMI))
	||  (mob->level >  0 && !IS_TRUSTED(ch,ANGEL))
	||  !IS_TRUSTED(ch,AVATAR))
	{
	    send_to_char(
		"Your powers are not great enough for such a task.\n\r",ch);
	    return;
	}

	clone = create_mobile(mob->pIndexData);
	clone_mobile(mob,clone);
	
	for (obj = mob->carrying; obj != NULL; obj = obj->next_content)
	{
	    if (obj_check(ch,obj))
	    {
		new_obj = create_object(obj->pIndexData,0);
		clone_object(obj,new_obj);
		recursive_clone(ch,obj,new_obj);
		obj_to_char(new_obj,clone);
		new_obj->wear_loc = obj->wear_loc;
	    }
	}
	char_to_room(clone,ch->in_room);
        act("$n has created $N.",ch,NULL,clone,TO_ROOM);
        act("You clone $N.",ch,NULL,clone,TO_CHAR);
	sprintf(buf,"$N clones %s.",clone->short_descr);
	wiznet(buf,ch,NULL,WIZ_LOAD,WIZ_SECURE,get_trust(ch));
        return;
    }
}

/* RT to replace the two load commands */

void do_load(CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
    log_naughty(ch,argument,1);
    argument = one_argument(argument,arg);

    if (arg[0] == '\0')
    {
	send_to_char("Syntax:\n\r",ch);
	send_to_char("  load mob <vnum>\n\r",ch);
	send_to_char("  load obj <vnum> <level>\n\r",ch);
	return;
    }

    if (!str_cmp(arg,"mob") || !str_cmp(arg,"char"))
    {
	do_mload(ch,argument);
	return;
    }

    if (!str_cmp(arg,"obj"))
    {
	do_oload(ch,argument);
	return;
    }
    /* echo syntax */
    do_load(ch,"");
}


void do_mload( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    MOB_INDEX_DATA *pMobIndex;
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];

    one_argument( argument, arg );

    if ( arg[0] == '\0' || !is_number(arg) )
    {
	send_to_char( "Syntax: load mob <vnum>.\n\r", ch );
	return;
    }

    if ( ( pMobIndex = get_mob_index( atoi( arg ) ) ) == NULL )
    {
	send_to_char( "No mob has that vnum.\n\r", ch );
	return;
    }

    victim = create_mobile( pMobIndex );
    char_to_room( victim, ch->in_room );
    act( "$n has created $N!", ch, NULL, victim, ch->invis_level > 50 ? TO_IMMINROOM : TO_ROOM );
    sprintf(buf,"$N loads %s.",victim->short_descr);
    wiznet(buf,ch,NULL,WIZ_LOAD,WIZ_SECURE,get_trust(ch));
    send_to_char( "Ok.\n\r", ch );
    return;
}



void do_oload( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH] ,arg2[MAX_INPUT_LENGTH];
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    int level;

    argument = one_argument( argument, arg1 );
    one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || !is_number(arg1))
    {
	send_to_char( "Syntax: load obj <vnum> <level>.\n\r", ch );
	return;
    }

    level = get_trust(ch); /* default */

    if ( arg2[0] != '\0')  /* load with a level */
    {
	if (!is_number(arg2))
        {
	  send_to_char( "Syntax: oload <vnum> <level>.\n\r", ch );
	  return;
	}
        level = atoi(arg2);
        if (level < 0 || level > get_trust(ch))
	{
	  send_to_char( "Level must be be between 0 and your level.\n\r",ch);
  	  return;
	}
    }

    if ( ( pObjIndex = get_obj_index( atoi( arg1 ) ) ) == NULL )
    {
	send_to_char( "No object has that vnum.\n\r", ch );
	return;
    }

    obj = create_object( pObjIndex, level );
    if ( CAN_WEAR(obj, ITEM_TAKE) )
	obj_to_char( obj, ch );
    else
	obj_to_room( obj, ch->in_room );
    act( "$n has created $p!", ch, obj, NULL, ch->invis_level > 50 ? TO_IMMINROOM : TO_ROOM );
    wiznet("$N loads $p.",ch,obj,WIZ_LOAD,WIZ_SECURE,get_trust(ch));
    send_to_char( "Ok.\n\r", ch );
    return;
}



void do_purge( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[100];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    DESCRIPTOR_DATA *d;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
		/* 'purge' */
		CHAR_DATA *vnext;
		OBJ_DATA  *obj_next;

		for ( victim = ch->in_room->people; victim != NULL; victim = vnext )
		{
	    	vnext = victim->next_in_room;
	    	if ( IS_NPC(victim) && !IS_SET(victim->act,ACT_NOPURGE)
	    	&&   victim != ch /* safety precaution */ )
				extract_char( victim, TRUE );
		}

	for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;
	    if (!IS_OBJ_STAT(obj,ITEM_NOPURGE))
	      extract_obj( obj );
	}

	act( "$n purges the room!", ch, NULL, NULL, TO_ROOM);
	send_to_char( "Ok.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( !IS_NPC(victim) )
    {

	if (ch == victim)
	{
	  send_to_char("Ho ho ho.\n\r",ch);
	  return;
	}

	if (get_trust(ch) <= get_trust(victim))
	{
	  send_to_char("Maybe that wasn't a good idea...\n\r",ch);
	  sprintf(buf,"%s tried to purge you!\n\r",ch->name);
	  send_to_char(buf,victim);
	  return;
	}

	act("$n disintegrates $N!",ch,0,victim,TO_NOTVICT);

    	if (victim->level > 1)
	    save_char_obj( victim );
    	d = victim->desc;
    	extract_char( victim, TRUE );
    	if ( d != NULL )
          close_socket( d );

	return;
    }

    act( "$n purges $N.", ch, NULL, victim, TO_NOTVICT );
    extract_char( victim, TRUE );
    return;
}



void do_advance( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int level, res=0;
    int iLevel;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || !is_number( arg2 ) )
    {
	send_to_char( "Syntax: advance <char> <level>.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "That player is not here.\n\r", ch);
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( ( level = atoi( arg2 ) ) < 1 || level > 60 )
    {
	send_to_char( "Level must be 1 to 60.\n\r", ch );
	return;
    }

    if ( level > get_trust( ch ) )
    {
	send_to_char( "Limited to your trust level.\n\r", ch );
	return;
    }

    /*
     * Lower level:
     *   Reset to level 1.
     *   Then raise again.
     *   Currently, an imp can lower another imp.
     *   -- Swiftest
     */
    if ( level <= victim->level )
    {
        int temp_prac;

	send_to_char( "Lowering a player's level!\n\r", ch );
	send_to_char( "**** OOOOHHHHHHHHHH  NNNNOOOO ****\n\r", victim );
	temp_prac = victim->practice;
	victim->level    = 1;
    	victim->exp      = exp_per_level(victim);
	victim->max_hit  = 10;
    	victim->max_mana = 100;
	victim->max_move = 100;
	victim->pcdata->perm_hit = 10;
    	victim->pcdata->perm_mana = 100;
	victim->pcdata->perm_move = 100;
	victim->practice = 0;
	victim->train 	 = 0;
	victim->hit      = victim->max_hit;
    	victim->mana     = victim->max_mana;
	victim->move     = victim->max_move;
	advance_level( victim, TRUE );
    }
    else
    {
	send_to_char( "Raising a player's level!\n\r", ch );
	send_to_char( "**** OOOOHHHHHHHHHH  YYYYEEEESSS ****\n\r", victim );
    }

    for ( iLevel = victim->level ; iLevel < level; iLevel++ )
    {
	victim->level += 1;
	advance_level( victim,FALSE);
    }
    sprintf(buf,"You are now level %d.\n\r",victim->level);
    send_to_char(buf,victim);
    if(victim->level >= 52)
		res = RS.SQL.Delete("players WHERE name = '%s'",victim->true_name);

	victim->exp   = exp_per_level(victim) * UMAX( 1, victim->level );
    save_char_obj(victim);
    return;
}



void do_trust( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int level;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    if ( arg1[0] == '\0' || arg2[0] == '\0' || !is_number( arg2 ) )
    {
	send_to_char( "Syntax: trust <char> <level>.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "That player is not here.\n\r", ch);
	return;
    }

    if ( ( level = atoi( arg2 ) ) < 0 || level > 60 )
    {
	send_to_char( "Level must be 0 (reset) or 1 to 60.\n\r", ch );
	return;
    }

    if ( level > get_trust( ch ) )
    {
	send_to_char( "Limited to your trust.\n\r", ch );
	return;
    }

    victim->trust = level;
    return;
}



void do_restore( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH], buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *vch;
    DESCRIPTOR_DATA *d;

    argument = one_argument( argument, arg );
    if (arg[0] == '\0' || !str_cmp(arg,"room"))
    {
    /* cure room */
    	
        for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
        {
            affect_strip(vch,gsn_plague);
            affect_strip(vch,gsn_poison);
            affect_strip(vch,gsn_blindness);
            affect_strip(vch,gsn_sleep);
            affect_strip(vch,gsn_curse);

            vch->hit 	= vch->max_hit;
            vch->mana   = vch->max_mana;
            vch->move	= vch->max_move;
            if (!IS_NPC(vch))	update_pos( vch);
            act("$n has restored you.",ch,NULL,vch,TO_VICT);
        }

        sprintf(buf,"$N restored room %d.",ch->in_room->vnum);
        wiznet(buf,ch,NULL,WIZ_RESTORE,WIZ_SECURE,get_trust(ch));

        send_to_char("Room restored.\n\r",ch);
        return;

    }

    if ( get_trust(ch) >=  MAX_LEVEL - 2 && !str_cmp(arg,"all"))
    {
    /* cure all */
    	
        for (d = descriptor_list; d != NULL; d = d->next)
        {
	    victim = d->character;

	    if (victim == NULL || IS_NPC(victim))
		continue;

            affect_strip(victim,gsn_plague);
            affect_strip(victim,gsn_poison);
            affect_strip(victim,gsn_blindness);
            affect_strip(victim,gsn_sleep);
            affect_strip(victim,gsn_curse);

            victim->hit 	= victim->max_hit;
            victim->mana    = victim->max_mana;
            victim->move	= victim->max_move;
            if (!IS_NPC(victim))	update_pos( victim);
	    if (victim->in_room != NULL)
	    {
		if(!argument || !str_cmp(argument,""))
                act("$n has restored you.",ch,NULL,victim,TO_VICT);
		else
		act("$t has restored you.",ch,argument,victim,TO_VICT);
	    }
        }
	send_to_char("All active players restored.\n\r",ch);
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    affect_strip(victim,gsn_plague);
    affect_strip(victim,gsn_poison);
    affect_strip(victim,gsn_blindness);
    affect_strip(victim,gsn_sleep);
    affect_strip(victim,gsn_curse);
    victim->hit  = victim->max_hit;
    victim->mana = victim->max_mana;
    victim->move = victim->max_move;
    if (!IS_NPC(victim)) 	update_pos( victim );
    act( "$n has restored you.", ch, NULL, victim, TO_VICT );
    sprintf(buf,"$N restored %s",
	IS_NPC(victim) ? victim->short_descr : victim->name);
    wiznet(buf,ch,NULL,WIZ_RESTORE,WIZ_SECURE,get_trust(ch));
    send_to_char( "Ok.\n\r", ch );
    return;
}

 	
void do_freeze( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH],buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Freeze whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( IS_SET(victim->act, PLR_FREEZE) )
    {
	REMOVE_BIT(victim->act, PLR_FREEZE);
	send_to_char( "You can play again.\n\r", victim );
	send_to_char( "FREEZE removed.\n\r", ch );
	sprintf(buf,"$N thaws %s.",victim->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }
    else
    {
	SET_BIT(victim->act, PLR_FREEZE);
	send_to_char( "You can't do ANYthing!\n\r", victim );
	send_to_char( "FREEZE set.\n\r", ch );
	sprintf(buf,"$N puts %s in the deep freeze.",victim->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }

    save_char_obj( victim );

    return;
}



void do_log( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Log whom?\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "all" ) )
    {
	if ( fLogAll )
	{
	    fLogAll = FALSE;
	    send_to_char( "Log ALL off.\n\r", ch );
	}
	else
	{
	    fLogAll = TRUE;
	    send_to_char( "Log ALL on.\n\r", ch );
	}
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    /*
     * No level check, gods can log anyone.
     */
    if ( IS_SET(victim->act, PLR_LOG) )
    {
	REMOVE_BIT(victim->act, PLR_LOG);
	send_to_char( "LOG removed.\n\r", ch );
    }
    else
    {
	SET_BIT(victim->act, PLR_LOG);
	send_to_char( "LOG set.\n\r", ch );
    }

    return;
}



void do_noemote( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH],buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Noemote whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }


    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( IS_SET(victim->comm, COMM_NOEMOTE) )
    {
	REMOVE_BIT(victim->comm, COMM_NOEMOTE);
	send_to_char( "You can emote again.\n\r", victim );
	send_to_char( "NOEMOTE removed.\n\r", ch );
	sprintf(buf,"$N restores emotes to %s.",victim->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }
    else
    {
	SET_BIT(victim->comm, COMM_NOEMOTE);
	send_to_char( "You can't emote!\n\r", victim );
	send_to_char( "NOEMOTE set.\n\r", ch );
	sprintf(buf,"$N revokes %s's emotes.",victim->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }

    return;
}



void do_noshout( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH],buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Noshout whom?\n\r",ch);
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( IS_SET(victim->comm, COMM_NOSHOUT) )
    {
	REMOVE_BIT(victim->comm, COMM_NOSHOUT);
	send_to_char( "You can shout again.\n\r", victim );
	send_to_char( "NOSHOUT removed.\n\r", ch );
	sprintf(buf,"$N restores shouts to %s.",victim->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }
    else
    {
	SET_BIT(victim->comm, COMM_NOSHOUT);
	send_to_char( "You can't shout!\n\r", victim );
	send_to_char( "NOSHOUT set.\n\r", ch );
	sprintf(buf,"$N revokes %s's shouts.",victim->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }

    return;
}



void do_notell( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH],buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Notell whom?", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( IS_SET(victim->comm, COMM_NOTELL) )
    {
	REMOVE_BIT(victim->comm, COMM_NOTELL);
	send_to_char( "You can tell again.\n\r", victim );
	send_to_char( "NOTELL removed.\n\r", ch );
	sprintf(buf,"$N restores tells to %s.",victim->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }
    else
    {
	SET_BIT(victim->comm, COMM_NOTELL);
	send_to_char( "You can't tell!\n\r", victim );
	send_to_char( "NOTELL set.\n\r", ch );
	sprintf(buf,"$N revokes %s's tells.",victim->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }

    return;
}



void do_peace( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *rch;

    for ( rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room )
    {
	if ( rch->fighting != NULL )
	    stop_fighting( rch, TRUE );
	if (IS_NPC(rch) && IS_SET(rch->act,ACT_AGGRESSIVE))
	    REMOVE_BIT(rch->act,ACT_AGGRESSIVE);
	if (IS_NPC(rch))
	    rch->last_fought = NULL;
    }

    send_to_char( "Ok.\n\r", ch );
    return;
}

void do_wizlock( CHAR_DATA *ch, char *argument )
{
    extern bool wizlock;
    wizlock = !wizlock;

    if ( wizlock )
    {
	wiznet("$N has wizlocked the game.",ch,NULL,0,0,0);
	send_to_char( "Game wizlocked.\n\r", ch );
    }
    else
    {
	wiznet("$N removes wizlock.",ch,NULL,0,0,0);
	send_to_char( "Game un-wizlocked.\n\r", ch );
    }

    return;
}

/* RT anti-newbie code */

void do_newlock( CHAR_DATA *ch, char *argument )
{
    extern bool newlock;
    newlock = !newlock;

    if ( newlock )
    {
	wiznet("$N locks out new characters.",ch,NULL,0,0,0);
        send_to_char( "New characters have been locked out.\n\r", ch );
    }
    else
    {
	wiznet("$N allows new characters back in.",ch,NULL,0,0,0);
        send_to_char( "Newlock removed.\n\r", ch );
    }

    return;
}


void do_slookup( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    int sn;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Lookup which skill or spell?\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "all" ) )
    {
	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
	    if ( skill_table[sn].name == NULL )
		break;
	    sprintf( buf, "Sn: %3d  Skill/spell: '%s'\n\r",
		sn, skill_table[sn].name );
	    send_to_char( buf, ch );
	}
    }
    else
    {
	if ( ( sn = skill_lookup( arg ) ) < 0 )
	{
	    send_to_char( "No such skill or spell.\n\r", ch );
	    return;
	}

	sprintf( buf, "Sn: %3d  Skill/spell: '%s'\n\r",
	    sn, skill_table[sn].name );
	send_to_char( buf, ch );
    }

    return;
}

/* RT set replaces sset, mset, oset, and rset */

void do_set( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    log_naughty(ch,argument,2);
    argument = one_argument(argument,arg);

    if (arg[0] == '\0')
    {
	send_to_char("Syntax:\n\r",ch);
	send_to_char("  set mob    <name> <field> <value>\n\r",ch);
	send_to_char("  set obj    <name> <field> <value>\n\r",ch);
	send_to_char("  set room   <room> <field> <value>\n\r",ch);
    send_to_char("  set skill  <name> <spell or skill> <value>\n\r",ch);
	send_to_char("	set sgroup <name> <group name>\n\r",ch);
	send_to_char("  set form   <name> <form/all> <%>\n\r",ch);
	return;
    }

    if (!str_prefix(arg,"mobile") || !str_prefix(arg,"character"))
    {
	do_mset(ch,argument);
	return;
    }

	if (!str_prefix(arg,"sgroup"))
	{
		do_sgset(ch,argument);
		return;
	}

    if (!str_prefix(arg,"skill") || !str_prefix(arg,"spell"))
    {
	do_sset(ch,argument);
	return;
    }

    if (!str_prefix(arg,"object"))
    {
	do_oset(ch,argument);
	return;
    }

    if (!str_prefix(arg,"room"))
    {
	do_rset(ch,argument);
	return;
    }
    /* echo syntax */
    do_set(ch,"");
}

void do_sgset( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
	int gn,i;
	char mesg[MSL];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0')
   		return send_to_char( "Syntax:\n\r  set sgroup <name> <group name>\n\r", ch);

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    	return send_to_char( "They aren't here.\n\r", ch );

    if ( IS_NPC(victim) )
    	return send_to_char( "Not on NPC's.\n\r", ch );
	
	gn = group_lookup(arg2);

	if(gn != -1)
	{
		if(ch->pcdata->group_known[gn] == TRUE)
		{
			ch->pcdata->group_known[gn] = FALSE;
			gn_remove(victim, gn);
			send_to_char("Group removed.\n\r", ch);
		}
		else
		{
			ch->pcdata->group_known[gn] = TRUE;
			/* Check for sorc focus and adjust acordingly */
			for(i = 0; i < (MAX_ELE); i++)
			{
				if(!str_cmp(arg2,sphere_table[i].name))
				{
					sprintf(mesg,"%s now has the elemental sphere %s.\n\r",victim->name,sphere_table[i].name);
					send_to_char(mesg,ch);
					SET_BIT(ch->pcdata->ele_sphere, sphere_table[i].element);
					break;
				}
			}
			gn_add(victim, gn);
			send_to_char("Group added.\n\r", ch);
		}
	}
    return;
}

void do_sset( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int value;
    int sn;
    bool fAll;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
	send_to_char( "Syntax:\n\r",ch);
	send_to_char( "  set skill <name> <spell or skill> <value>\n\r", ch);
	send_to_char( "  set skill <name> all <value>\n\r",ch);
	send_to_char("   (use the name of the skill, not the number)\n\r",ch);
	return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    fAll = !str_cmp( arg2, "all" );
    sn   = 0;
    if ( !fAll && ( sn = skill_lookup( arg2 ) ) < 0 )
    {
	send_to_char( "No such skill or spell.\n\r", ch );
	return;
    }

    /*
     * Snarf the value.
     */
    if ( !is_number( arg3 ) )
    {
	send_to_char( "Value must be numeric.\n\r", ch );
	return;
    }

    value = atoi( arg3 );
    if ( (value < 0 || value > 100) && (value != -2) )
    {
	send_to_char( "Value range is 0 to 100 (-2 for permanent strip).\n\r", ch );
	return;
    }

    if ( fAll )
    {
		for ( sn = 0; sn < MAX_SKILL; sn++ )
		{
	    	if ( skill_table[sn].name != NULL )
				victim->pcdata->learned[sn]	= value;
		}
    }
    else
    {
		victim->pcdata->learned[sn] = value;
    }

    return;
}



void do_mset( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    char buf[300];
    CHAR_DATA *victim;
    int value, i;
	AFFECT_DATA af;
	
    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    strcpy( arg3, argument );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
	send_to_char("Syntax:\n\r",ch);
	send_to_char("  set char <name> <field> <value>\n\r",ch);
	send_to_char( "  Field being one of:\n\r",			ch );
	send_to_char( "    str int wis dex con sex class level\n\r",	ch );
    send_to_char( "    race group gold hp mana move prac\n\r",ch);
	send_to_char( "    align train thirst hunger drunk home\n\r",	ch );
	send_to_char("    xptotal size dam_mod tribe special pause\n\r",ch);
	send_to_char("    beauty security aggressor proficiency profpoints\n\r", ch);
	return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    /* clear zones for mobs */
    victim->zone = NULL;

    /*
     * Snarf the value (which need not be numeric).
     */
    value = is_number( arg3 ) ? atoi( arg3 ) : -1;

    /*
     * Set something.
     */
	if ( !str_cmp(arg2,"aggressor"))
	{
		init_affect(&af);
		af.where    = TO_AFFECTS;
		af.type     = gsn_aggressor;
		af.aftype   = AFT_INVIS;
		af.level    = 60;
		af.duration = 12;
		affect_to_char(victim, &af);
		send_to_char("Aggressor status set.\n\r", ch);
		return;
	}
	else if ( !str_cmp(arg2,"beauty"))
	{
		if(value>0 && value<11)
		{
			victim->pcdata->beauty = value;
			return;
		}
		else
			return send_to_char("Invalid beauty modifier.\n\r", ch);
	}
    if ( !str_cmp(arg2,"size"))
    {
	if(value>-1 && value <= SIZE_IMMENSE)
	{
		victim->size=value;
		return;
	}
	send_to_char("Invalid size value.\n\r",ch);
	return;
    }
    if ( !str_cmp( arg2, "xptotal"))
    {
	if ( value < 0)
	{
	send_to_char("Xp total must be greater than 0.\n\r",ch);
	return;
	}
	victim->exp = value;
	return;
    }
    if (!str_cmp( arg2, "home"))
    {
	if (hometown_lookup(arg3))
	{
	victim->hometown = hometown_lookup(arg3);
	return;
	}
	else
	{
	send_to_char("No such hometown.\n\r",ch);
	return;
	}
    }
    if ( !str_cmp( arg2,"sp" ) )
    {
	if (value < 0 || value > 20000)
	{
	send_to_char("Specialization points are between 0 and 20000.\n\r",ch);	
	return;
	}
	victim->pcdata->sp = value;
	return;
    }

    if ( !str_cmp( arg2, "str" ) )
    {
	if ( value < 3 || value > get_max_train(victim,STAT_STR) )
	{
	    sprintf(buf,
		"Strength range is 3 to %d\n\r.",
		get_max_train(victim,STAT_STR));
	    send_to_char(buf,ch);
	    return;
	}

	victim->perm_stat[STAT_STR] = value;
	return;
    }

    if ( !str_cmp( arg2, "security" ) )	/* OLC */
    {
        if ( IS_NPC( victim ) )
        {
            send_to_char( "Not on NPC's.\n\r", ch );
            return;
        }

	if ( value < 0 || value > 10)
	{
		send_to_char( "Valid security is 0-10 only.\n\r", ch );
	}
	else
		victim->pcdata->security = value;
	return;
    }

    if ( !str_cmp( arg2, "int" ) )
    {
        if ( value < 3 || value > get_max_train(victim,STAT_INT) )
        {
            sprintf(buf,
		"Intelligence range is 3 to %d.\n\r",
		get_max_train(victim,STAT_INT));
            send_to_char(buf,ch);
            return;
        }

        victim->perm_stat[STAT_INT] = value;
        return;
    }

    if ( !str_cmp( arg2, "wis" ) )
    {
	if ( value < 3 || value > get_max_train(victim,STAT_WIS) )
	{
	    sprintf(buf,
		"Wisdom range is 3 to %d.\n\r",get_max_train(victim,STAT_WIS));
	    send_to_char( buf, ch );
	    return;
	}

	victim->perm_stat[STAT_WIS] = value;
	return;
    }

    if ( !str_cmp( arg2, "dex" ) )
    {
	if ( value < 3 || value > get_max_train(victim,STAT_DEX) )
	{
	    sprintf(buf,
		"Dexterity ranges is 3 to %d.\n\r",
		get_max_train(victim,STAT_DEX));
	    send_to_char( buf, ch );
	    return;
	}

	victim->perm_stat[STAT_DEX] = value;
	return;
    }

    if ( !str_cmp( arg2, "con" ) )
    {
	if ( value < 3 || value > get_max_train(victim,STAT_CON) )
	{
	    sprintf(buf,
		"Constitution range is 3 to %d.\n\r",
		get_max_train(victim,STAT_CON));
	    send_to_char( buf, ch );
	    return;
	}

	victim->perm_stat[STAT_CON] = value;
	return;
    }

    if ( !str_prefix( arg2, "sex" ) )
    {
	if ( value < 0 || value > 2 )
	{
	    send_to_char( "Sex range is 0 to 2.\n\r", ch );
	    return;
	}
	victim->sex = value;
	if (!IS_NPC(victim))
	    victim->pcdata->true_sex = value;
	return;
    }

    if ( !str_prefix( arg2, "class" ) )
    {
	int sclass;

	if (IS_NPC(victim))
	{
	    send_to_char("Mobiles have no class.\n\r",ch);
	    return;
	}

	sclass = CClass::Lookup(arg3);
	if ( sclass == -1 )
	{
	    char buf[MAX_STRING_LENGTH];

        	strcpy( buf, "Possible classes are: " );
        	for ( sclass = 0; sclass < MAX_CLASS; sclass++ )
        	{
            	    if ( sclass > 0 )
                    	strcat( buf, " " );
            	    strcat( buf, CClass::GetClass(sclass)->name );
        	}
            strcat( buf, ".\n\r" );

	    send_to_char(buf,ch);
	    return;
	}

	victim->SetClass(sclass);
	return;
    }

    if ( !str_prefix( arg2, "level" ) )
    {
	if ( !IS_NPC(victim) )
	{
	    send_to_char( "Not on PC's.\n\r", ch );
	    return;
	}

	if ( value < 0 || value > 60 )
	{
	    send_to_char( "Level range is 0 to 60.\n\r", ch );
	    return;
	}
	victim->level = value;
	return;
    }

    if ( !str_prefix( arg2, "gold" ) )
    {
	victim->gold = value;
	return;
    }

    if ( !str_prefix( arg2, "hp" ) )
    {
	if ( value < -10 || value > 30000 )
	{
	    send_to_char( "Hp range is -10 to 30,000 hit points.\n\r", ch );
	    return;
	}
	victim->max_hit = value;
        if (!IS_NPC(victim))
            victim->pcdata->perm_hit = value;
	return;
    }

    if ( !str_prefix( arg2, "mana" ) )
    {
	if ( value < 0 || value > 30000 )
	{
        send_to_char( "mana range is 0 to 30,000 mana points.\n\r", ch );
	    return;
	}
    victim->max_mana = value;
        if (!IS_NPC(victim))
            victim->pcdata->perm_mana = value;
	return;
    }

    if ( !str_prefix( arg2, "move" ) )
    {
	if ( value < 0 || value > 30000 )
	{
	    send_to_char( "Move range is 0 to 30,000 move points.\n\r", ch );
	    return;
	}
	victim->max_move = value;
        if (!IS_NPC(victim))
            victim->pcdata->perm_move = value;
	return;
    }

    if ( !str_prefix( arg2, "practice" ) )
    {
	if ( value < 0 || value > 250 )
	{
	    send_to_char( "Practice range is 0 to 250 sessions.\n\r", ch );
	    return;
	}
	victim->practice = value;
	return;
    }

    if ( !str_prefix( arg2, "train" ))
    {
	if (value < 0 || value > 50 )
	{
	    send_to_char("Training session range is 0 to 50 sessions.\n\r",ch);
	    return;
	}
	victim->train = value;
	return;
    }
    if ( !str_prefix( arg2, "align" ) )
    {
	if ( value < -1000 || value > 1000 )
	{
	    send_to_char( "Alignment range is -1000 to 1000.\n\r", ch );
	    return;
	}
	victim->alignment = value;
	return;
    }

    if ( !str_prefix( arg2, "ethos" ) )
    {
	if ( value < -1000 || value > 1000 )
	{
	    send_to_char( "Ethos range is -1000 to 1000.\n\r", ch );
	    return;
	}
	if (!IS_NPC(victim))
	    victim->pcdata->ethos = value;
	return;
    }

    if ( !str_prefix( arg2, "thirst" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < -1 || value > 100 )
	{
	    send_to_char( "Thirst range is -1 to 100.\n\r", ch );
	    return;
	}

	victim->pcdata->condition[COND_THIRST] = value;
	return;
    }

    if ( !str_prefix( arg2, "drunk" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < -1 || value > 100 )
	{
	    send_to_char( "Drunk range is -1 to 100.\n\r", ch );
	    return;
	}

	victim->pcdata->condition[COND_DRUNK] = value;
	return;
    }


    if ( !str_prefix( arg2, "hunger" ) )
    {
        if ( IS_NPC(victim) )
        {
            send_to_char( "Not on NPC's.\n\r", ch );
            return;
        }

        if ( value < -1 || value > 100 )
        {
            send_to_char( "Full range is -1 to 100.\n\r", ch );
            return;
        }

        victim->pcdata->condition[COND_HUNGER] = value;
        return;
    }

    if (!str_prefix( arg2, "race" ) )
    {
	int race;

	race = race_lookup(arg3);

	if ( race == 0)
	{
	    send_to_char("That is not a valid race.\n\r",ch);
	    return;
	}

	if (!IS_NPC(victim) && !race_table[race].pc_race)
	{
	    send_to_char("That is not a valid player race.\n\r",ch);
	    return;
	}

	victim->race = race;
	victim->size = pc_race_table[race].size;
	return;
    }

    if (!str_prefix(arg2,"group"))
    {
	if (!IS_NPC(victim))
	{
	    send_to_char("Only on NPCs.\n\r",ch);
	    return;
	}
	victim->group = value;
	return;
    }
    if(!str_prefix(arg2,"dam_mod"))
    {
	if(!is_number(arg3))
		return send_to_char("Invalid dam_mod.\n\r",ch);
	victim->dam_mod = atoi(arg3);
	sprintf(buf,"%s's dam_mod set to %f%%.\n\r",IS_NPC(victim) ? victim->short_descr : victim->name, victim->dam_mod);
	send_to_char(buf,ch);
	return;
    }
    if(!str_prefix(arg2,"tribe") && get_trust(ch)>=59)
    {
	for(i=0; tribe_table[i].name; i++)
		if(!str_prefix(arg3,tribe_table[i].name))
			break;
	if(!tribe_table[i].name)
		return send_to_char("There is no tribe by that name.\n\r",ch);
	victim->pcdata->tribe	= i;
	sprintf(buf,"%s is now in the %s tribe.\n\r",victim->name,capitalize(tribe_table[i].name));
	send_to_char(buf,ch);
	return;
    }
    if(!str_prefix(arg2,"specializations"))
    {
	if(IS_NPC(victim) || !is_number(arg3))
		return send_to_char("You can't do that.\n\r",ch);
	victim->pcdata->special = atoi(arg3);
	sprintf(buf,"%s now has %d specializations.\n\r",victim->name,victim->pcdata->special);
	send_to_char(buf,ch);
	return;
    }
    if(!str_prefix(arg2,"pause"))
    {
	if (IS_NPC(victim))
		return send_to_char("You can't do that.\n\r",ch);
	if(!is_number(arg3))
		return send_to_char("Invalid pause.\n\r",ch);
	victim->pause = atoi(arg3);
	sprintf(buf,"%s's pause set to %d.\n\r", victim->true_name, victim->pause);
	send_to_char(buf,ch);
	return;
    }
    if(!str_prefix(arg2,"proficiency"))
    {   
        char buf4[MSL];
        argument = one_argument(argument, buf4);
        int ind = CProficiencies::ProfIndexLookup(buf4);
        if (IS_NPC(victim))
                return send_to_char("Not on mobiles.\n\r",ch);
        if(ind < 0 || !is_number(argument) || atoi(argument) > 10 || atoi(argument) < -1)
                return send_to_char("Invalid input.\n\r",ch);
        victim->Profs()->SetProf(ind, atoi(argument));
	return send_to_char("Proficiency set.\n\r",ch);
    }
    if(!str_prefix(arg2,"profpoints"))
    {
	if(IS_NPC(victim) || !is_number(arg3))
		return send_to_char("You can't do that.\n\r",ch);
	victim->Profs()->SetPoints(atoi(arg3));
	sprintf(buf,"%s now has %d proficiency points.\n\r",victim->name,victim->Profs()->GetPoints());
	return send_to_char(buf,ch);
    }
    /*
     * Generate usage message.
     */
    do_mset( ch, "" );
    return;
}

void do_string( CHAR_DATA *ch, char *argument )
{
    char type [MAX_INPUT_LENGTH];
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    log_naughty(ch,argument,3);
    smash_tilde( argument );
    argument = one_argument( argument, type );
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    strcpy( arg3, argument );

    if ( type[0] == '\0' || arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
	send_to_char("Syntax:\n\r",ch);
	send_to_char("  string char <name> <field> <string>\n\r",ch);
	send_to_char("    fields: name short long desc title spec\n\r",ch);
	send_to_char("  string obj  <name> <field> <string>\n\r",ch);
	send_to_char("    fields: name short long extended\n\r",ch);
	return;
    }

    if (!str_prefix(type,"character") || !str_prefix(type,"mobile"))
    {
    	if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    	{
	    send_to_char( "They aren't here.\n\r", ch );
	    return;
    	}

	/* clear zone for mobs */
	victim->zone = NULL;

	/* string something */

     	if ( !str_prefix( arg2, "name" ) )
    	{
	    free_pstring( victim->name );
	    victim->name = palloc_string( arg3 );
	    return;
    	}
    	
    	if ( !str_prefix( arg2, "description" ) )
    	{
    	    free_pstring(victim->description);
    	    victim->description = palloc_string(arg3);
    	    return;
    	}

    	if ( !str_prefix( arg2, "short" ) )
    	{
	    free_pstring( victim->short_descr );
	    victim->short_descr = palloc_string( arg3 );
	    return;
    	}

    	if ( !str_prefix( arg2, "long" ) )
    	{
	    free_pstring( victim->long_descr );
	    strcat(arg3,"\n\r");
	    victim->long_descr = palloc_string( arg3 );
	    return;
    	}

    	if ( !str_prefix( arg2, "title" ) )
    	{
	    if ( IS_NPC(victim) )
	    {
	    	send_to_char( "Not on NPC's.\n\r", ch );
	    	return;
	    }

	    set_title( victim, arg3 );
	    return;
    	}

    }

    if (!str_prefix(type,"object"))
    {
    	/* string an obj */
    	
   	if ( ( obj = get_obj_world( ch, arg1 ) ) == NULL )
    	{
	    send_to_char( "Nothing like that in heaven or earth.\n\r", ch );
	    return;
    	}
    	
        if ( !str_prefix( arg2, "name" ) )
    	{
	    free_pstring( obj->name );
	    obj->name = palloc_string( arg3 );
	    return;
    	}

    	if ( !str_prefix( arg2, "short" ) )
    	{
	    free_pstring( obj->short_descr );
	    obj->short_descr = palloc_string( arg3 );
	    return;
    	}

    	if ( !str_prefix( arg2, "long" ) )
    	{
	    free_pstring( obj->description );
	    obj->description = palloc_string( arg3 );
	    return;
    	}

    	if ( !str_prefix( arg2, "ed" ) || !str_prefix( arg2, "extended"))
    	{
	    EXTRA_DESCR_DATA *ed;

	    argument = one_argument( argument, arg3 );
	    if ( argument == NULL )
	    {
	    	send_to_char( "Syntax: oset <object> ed <keyword> <string>\n\r",
		    ch );
	    	return;
	    }

 	    strcat(argument,"\n\r");

	    ed = new_extra_descr();

	    ed->keyword		= palloc_string( arg3     );
	    ed->description	= palloc_string( argument );
	    ed->next		= obj->extra_descr;
	    obj->extra_descr	= ed;
	    return;
    	}
    }

    	
    /* echo bad use message */
    do_string(ch,"");
}



void do_oset( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int value;

    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    strcpy( arg3, argument );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
	send_to_char("Syntax:\n\r",ch);
	send_to_char("  set obj <object> <field> <value>\n\r",ch);
	send_to_char("  Field being one of:\n\r",				ch );
	send_to_char("    value0 value1 value2 value3 value4 (v1-v4)\n\r",	ch );
	send_to_char("    extra wear level weight cost timer\n\r",		ch );
	return;
    }

    if ( ( obj = get_obj_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "Nothing like that in heaven or earth.\n\r", ch );
	return;
    }

    /*
     * Snarf the value (which need not be numeric).
     */
    value = atoi( arg3 );

    /*
     * Set something.
     */
    if ( !str_cmp( arg2, "value0" ) || !str_cmp( arg2, "v0" ) )
    {
/*	obj->value[0] = UMIN(50,value); */
	obj->value[0] = value;
	return;
    }

    if ( !str_cmp( arg2, "value1" ) || !str_cmp( arg2, "v1" ) )
    {
	obj->value[1] = value;
	return;
    }

    if ( !str_cmp( arg2, "value2" ) || !str_cmp( arg2, "v2" ) )
    {
	obj->value[2] = value;
	return;
    }

    if ( !str_cmp( arg2, "value3" ) || !str_cmp( arg2, "v3" ) )
    {
	obj->value[3] = value;
	return;
    }

    if ( !str_cmp( arg2, "value4" ) || !str_cmp( arg2, "v4" ) )
    {
	obj->value[4] = value;
	return;
    }

    if ( !str_prefix( arg2, "level" ) )
    {
	obj->level = value;
	return;
    }
	
    if ( !str_prefix( arg2, "weight" ) )
    {
	obj->weight = value;
	return;
    }

    if ( !str_prefix( arg2, "cost" ) )
    {
	obj->cost = value;
	return;
    }

    if ( !str_prefix( arg2, "timer" ) )
    {
	obj->timer = value;
	return;
    }

    /*
     * Generate usage message.
     */
    do_oset( ch, "" );
    return;
}



void do_rset( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *location;
    int value;

    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    strcpy( arg3, argument );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
	send_to_char( "Syntax:\n\r",ch);
	send_to_char( "  set room <location> <field> <value>\n\r",ch);
	send_to_char( "  Field being one of:\n\r",			ch );
	send_to_char( "    flags sector\n\r",				ch );
	return;
    }

    if ( ( location = find_location( ch, arg1 ) ) == NULL )
    {
	send_to_char( "No such location.\n\r", ch );
	return;
    }

    if (!is_room_owner(ch,location) && ch->in_room != location
    &&  room_is_private(location) && !IS_TRUSTED(ch,IMPLEMENTOR))
    {
        send_to_char("That room is private right now.\n\r",ch);
        return;
    }

    /*
     * Snarf the value.
     */
    if ( !is_number( arg3 ) )
    {
	send_to_char( "Value must be numeric.\n\r", ch );
	return;
    }
    value = atoi( arg3 );

    /*
     * Set something.
     */

    if ( !str_prefix( arg2, "sector" ) )
    {
	location->sector_type	= value;
	return;
    }

    /*
     * Generate usage message.
     */
    do_rset( ch, "" );
    return;
}



void do_sockets( CHAR_DATA *ch, char *argument )
{
    char buf[2 * MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    DESCRIPTOR_DATA *d;
    int count;
    bool bDis = FALSE; //displinary

    count	= 0;
    buf[0]	= '\0';
    if(!strcmp(argument,"show"))
	bDis = TRUE;
    one_argument(argument,arg);
    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->character != NULL && ((!d->original && can_see( ch, d->character )) || (d->original && can_see(ch,d->original)) )
	&& (!IS_SET(d->character->comm, COMM_NOSOCKET) || get_trust(ch) == MAX_LEVEL) && (arg[0] == '\0' ||
		is_name(arg,d->character->true_name) || bDis))
	{
	    count++;
/* Changed this so that socket connection state is listed in plain
english. Any imm can read socket connection status in order to see who's
rolling multiple chars (etc), but only imms 55+ can see IP and
hostnames.
 */
	    sprintf( buf + strlen(buf), "[%3d %14s %4d] %s@%s\n\r",
		d->descriptor,
	d->connected == (CON_PLAYING) ? "Playing" :
	d->connected == (CON_GET_NAME) ? "Getting name" :
	d->connected == (CON_GET_OLD_PASSWORD) ? "Getting password" :
	d->connected == (CON_CONFIRM_NEW_NAME) ? "Confirming name" :
	d->connected == (CON_GET_NEW_PASSWORD) ? "Getting new pwd" :
	d->connected == (CON_CONFIRM_NEW_PASSWORD) ? "Confirm new pwd" :
	d->connected == (CON_GET_NEW_RACE) ? "Getting race" :
	d->connected == (CON_GET_NEW_SEX) ? "Getting sex" :
	d->connected == (CON_GET_NEW_CLASS) ? "Getting class" :
	d->connected == (CON_ALLOCATE_STATS) ? "Allocating stats" :
    	d->connected == (CON_DEFAULT_CHOICE) ? "Get hometown" :
	d->connected == (CON_GET_ALIGNMENT) ? "Getting alignment" :
	d->connected == (CON_GET_ETHOS) ? "Getting ethos" :
	d->connected == (CON_READ_IMOTD) ? "Getting Imotd":
	d->connected == (CON_READ_MOTD) ? "Getting motd" :
	d->connected == (CON_CHOOSE_WEAPON) ? "Choosing weapon" :
	d->connected == (CON_GET_CABAL) ? "Getting cabal" : "null",
	(int)((current_time - d->character->logon) / 60 ) ,
		d->original  ? (d->original->true_name) ?
		d->original->true_name : d->original->name  :
		d->character ? (d->character->true_name) ?
		d->character->true_name : d->character->name : "(none)",
	bDis && get_trust(ch)>58 ? d->host : "unknown");
	}
    }
    if (count == 0)
    {
	send_to_char("No one by that name is connected.\n\r",ch);
	return;
    }

    sprintf( buf2, "%d user%s.\n\r",
count, count == 1 ? "" : "s" );
    strcat(buf,buf2);
    page_to_char( buf, ch );
    return;
}

char* get_end_host(char* host) {
   int i=0;
   while(host[i]!='.'&&host[i]!='\0') {
      i++;
   }
   if(host[i]!='\0')
   i++;
   return &host[i];
}

int host_comp(MULTDATA* d1, MULTDATA* d2) {
    return strcmp(get_end_host(d1->des->host),get_end_host(d2->des->host));
}

void do_multicheck( CHAR_DATA *ch, char *argument) {
    char buf[2 * MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    int count=0,i=0,j=0;
    MULTDATA CHARLIST[80];

    buf[0]      = '\0';

      for(d=descriptor_list;d!=NULL;d=d->next) {
	if(d->character!=NULL&&can_see(ch,d->character)) {
	   CHARLIST[i].des=d;
	   i++;
	}
	if(i>80) {
	   send_to_char("Max 80 characters, have coder fix.\n\r",ch);
	   return;
	}
      }
	return send_to_char("Fix later.\n\r",ch);
     // qsort(CHARLIST, i, sizeof(MULTDATA), (void*)&host_comp);
  
    if(i<2) {
	send_to_char("This command is not usable with only 1 player.\n\r",ch);
	return;
    }

    for(j=0;j<i-1;j++) {
/*      if((!strcmp(get_end_host(CHARLIST[j].des->host),
         	get_end_host(CHARLIST[j+1].des->host))) ||
		(j!=0&&!strcmp(get_end_host(CHARLIST[j].des->host),
		get_end_host(CHARLIST[j-1].des->host)))) {*/
	if(!strcmp(CHARLIST[j].des->host,CHARLIST[j+1].des->host) ||
		(j!=0 && !strcmp(CHARLIST[j].des->host, CHARLIST[j-1].des->host))) {
          count++;
          sprintf( buf + strlen(buf), "[%3d %2d] %s@%s\n\r",
                CHARLIST[j].des->descriptor,
                CHARLIST[j].des->connected,
                CHARLIST[j].des->original  ?
		CHARLIST[j].des->original->true_name ?
		CHARLIST[j].des->original->true_name :
		CHARLIST[j].des->original->name  :
                CHARLIST[j].des->character ?
		CHARLIST[j].des->character->true_name ?
		CHARLIST[j].des->character->true_name :
		CHARLIST[j].des->character->name : "(none)",
                (get_trust(ch) >= 55) ? CHARLIST[j].des->host : "unknown"
         	);
      }
    }

//    if(!strcmp(get_end_host(CHARLIST[j].des->host),
//		get_end_host(CHARLIST[j-1].des->host))) {
      if(!strcmp(CHARLIST[j].des->host,CHARLIST[j-1].des->host)) {
	count++;
	sprintf(buf+strlen(buf), "[%3d %2d] %s@%s\n\r",
		CHARLIST[j].des->descriptor, CHARLIST[j].des->connected,
		CHARLIST[j].des->original ? CHARLIST[j].des->original->name :
		CHARLIST[j].des->character ? CHARLIST[j].des->character->name : "(none)",
		(get_trust(ch) >= 55) ? CHARLIST[j].des->host : "unknown");
    }

    if (count == 0) {
       send_to_char("No matches were found.\n\r",ch);
       return;
    }

    sprintf( buf2, "%d user%s\n\r", count, count == 1 ? "" : "s" );
    strcat(buf,buf2);
    page_to_char( buf, ch );
    return;
}


/* Use this to lag out spammers and force them to stop moving */
void do_lagout(CHAR_DATA *ch,char *argument)
{
	char arg[MAX_STRING_LENGTH];
	CHAR_DATA *victim;

    one_argument(argument,arg);
    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	return;
    }
    	victim = get_char_world(ch,arg);
	if (victim == NULL)
	{
	return;
 	}
	WAIT_STATE(victim,24);
	return;
}


/*
 * Thanks to Grodyn for pointing out bugs in this function.
 */
void do_force( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "Force whom to do what?\n\r", ch );
	return;
    }

    one_argument(argument,arg2);

    if (!str_cmp(arg2,"delete") || !str_prefix(arg2,"mob"))
    {
	send_to_char("That will NOT be done.\n\r",ch);
	return;
    }

    sprintf( buf, "$n forces you to '%s'.", argument );

    if ( !str_cmp( arg, "all" ) )
    {
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;

	if (get_trust(ch) < MAX_LEVEL - 3 && str_cmp(arg2,"save"))
	{
	    send_to_char("Not at your level!\n\r",ch);
	    return;
	}

	for ( vch = char_list; vch != NULL; vch = vch_next )
	{
	    vch_next = vch->next;

	    if ( !IS_NPC(vch) && get_trust( vch ) < get_trust( ch ) )
	    {
		if(!can_see(vch,ch))
			sprintf(buf,"An irresistable urge forces you to '%s'.",argument);
		act( buf, ch, NULL, vch, TO_VICT );
		interpret( vch, argument );
	    }
	}
    }
    else if (!str_cmp(arg,"players"))
    {
        CHAR_DATA *vch;
        CHAR_DATA *vch_next;

        if (get_trust(ch) < MAX_LEVEL - 2)
        {
            send_to_char("Not at your level!\n\r",ch);
            return;
        }

        for ( vch = char_list; vch != NULL; vch = vch_next )
        {
            vch_next = vch->next;

            if ( !IS_NPC(vch) && get_trust( vch ) < get_trust( ch )
	    &&	 vch->level < LEVEL_HERO)
            {
                act( buf, ch, NULL, vch, TO_VICT );
                interpret( vch, argument );
            }
        }
    }
    else if (!str_cmp(arg,"gods"))
    {
        CHAR_DATA *vch;
        CHAR_DATA *vch_next;

        if (get_trust(ch) < MAX_LEVEL - 2)
        {
            send_to_char("Not at your level!\n\r",ch);
            return;
        }

        for ( vch = char_list; vch != NULL; vch = vch_next )
        {
            vch_next = vch->next;

            if ( !IS_NPC(vch) && get_trust( vch ) < get_trust( ch )
            &&   vch->level >= LEVEL_HERO)
            {
                act( buf, ch, NULL, vch, TO_VICT );
                interpret( vch, argument );
            }
        }
    }
    else
    {
	CHAR_DATA *victim;

	if ( ( victim = get_char_world( ch, arg ) ) == NULL )
	{
	    send_to_char( "They aren't here.\n\r", ch );
	    return;
	}

	if ( victim == ch )
	{
	    send_to_char( "Aye aye, right away!\n\r", ch );
	    return;
	}

//	if(victim->desc == NULL)
//		return send_to_char("They have no descriptor!\n\r",ch);

    	if (!is_room_owner(ch,victim->in_room)
	&&  ch->in_room != victim->in_room
        &&  room_is_private(victim->in_room) && !IS_TRUSTED(ch,IMPLEMENTOR))
    	{
            send_to_char("That character is in a private room.\n\r",ch);
            return;
        }

	if ( get_trust( victim ) >= get_trust( ch ) )
	{
	    send_to_char( "Do it yourself!\n\r", ch );
	    return;
	}

	if ( !IS_NPC(victim) && get_trust(ch) < MAX_LEVEL -3)
	{
	    send_to_char("Not at your level!\n\r",ch);
	    return;
	}

	act( buf, ch, NULL, victim, TO_VICT );
	interpret( victim, argument );
    }

    send_to_char( "Ok.\n\r", ch );
    return;
}



/*
 * New routines by Dionysos.
 */
void do_invis( CHAR_DATA *ch, char *argument )
{
    int level;
    char arg[MAX_STRING_LENGTH];

    /* RT code for taking a level argument */
    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    /* take the default path */

      if ( ch->invis_level)
      {
	  ch->invis_level = 0;
	  act( "$n uncloaks $s presence.", ch, NULL, NULL, TO_ROOM );
	  send_to_char( "You uncloak your presence.\n\r", ch );
      }
      else
      {
	  	act( "$n cloaks $s presence.", ch, NULL, NULL, TO_ROOM );
	  	send_to_char( "You cloak your presence.\n\r", ch );
		ch->invis_level = get_trust(ch);
	  }
    else
    /* do the level thing */
    {
      level = atoi(arg);
      if (level < 2 || level > get_trust(ch))
      {
		send_to_char("Invis level must be between 2 and your level.\n\r",ch);
        return;
      }
      else
      {
	  ch->reply = NULL;
          act( "$n cloaks $s presence.", ch, NULL, NULL, TO_ROOM );
          send_to_char( "You cloak your presence.\n\r", ch );
ch->invis_level = level;
	  }
    }

    return;
}


void do_incognito( CHAR_DATA *ch, char *argument )
{
    int level;
    char arg[MAX_STRING_LENGTH];

    /* RT code for taking a level argument */
    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    /* take the default path */

      if ( ch->incog_level)
      {
          ch->incog_level = 0;
          act( "$n is no longer cloaked.", ch, NULL, NULL, TO_ROOM );
          send_to_char( "You are no longer cloaked.\n\r", ch );
      }
      else
      {
          ch->incog_level = get_trust(ch);
          act( "$n cloaks $s presence.", ch, NULL, NULL, TO_ROOM );
          send_to_char( "You cloak your presence.\n\r", ch );
      }
    else
    /* do the level thing */
    {
      level = atoi(arg);
      if (level < 2 || level > get_trust(ch))
      {
        send_to_char("Incog level must be between 2 and your level.\n\r",ch);
        return;
      }
      else
      {
          ch->reply = NULL;
          ch->incog_level = level;
          act( "$n cloaks $s presence.", ch, NULL, NULL, TO_ROOM );
          send_to_char( "You cloak your presence.\n\r", ch );
      }
    }

    return;
}



void do_holylight( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    if ( IS_SET(ch->act, PLR_HOLYLIGHT) )
    {
	REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
	send_to_char( "Holy light mode off.\n\r", ch );
    }
    else
    {
	SET_BIT(ch->act, PLR_HOLYLIGHT);
	send_to_char( "Holy light mode on.\n\r", ch );
    }

    return;
}

/* prefix command: it will put the string typed on each line typed */

void do_prefi (CHAR_DATA *ch, char *argument)
{
    send_to_char("You cannot abbreviate the prefix command.\r\n",ch);
    return;
}

void do_prefix (CHAR_DATA *ch, char *argument)
{
    char buf[MAX_INPUT_LENGTH];

    if (argument[0] == '\0')
    {
	if (ch->prefix[0] == '\0')
	{
	    send_to_char("You have no prefix to clear.\r\n",ch);
	    return;
	}

	send_to_char("Prefix removed.\r\n",ch);
	free_pstring(ch->prefix);
	ch->prefix = palloc_string("");
	return;
    }

    if (ch->prefix[0] != '\0')
    {
	sprintf(buf,"Prefix changed to %s.\r\n",argument);
	free_pstring(ch->prefix);
    }
    else
    {
	sprintf(buf,"Prefix set to %s.\r\n",argument);
    }

    ch->prefix = palloc_string(argument);
}

void do_astrip(CHAR_DATA *ch,char *argument)
{
	CHAR_DATA *victim;
	AFFECT_DATA *af, *af_next;
	char arg[MAX_STRING_LENGTH];

   	one_argument(argument,arg);
	if (arg[0] == '\0')
		victim = ch;
	else
		victim = get_char_world(ch,arg);

	if (victim == NULL)
	{
	send_to_char("They aren't here.\n\r",ch);
	return;
	}

            for ( af = victim->affected; af != NULL; af = af_next )
            {
		af_next=af->next;
		if(IS_SET(af->bitvector,AFF_PERMANENT))
			continue;
		affect_remove(victim,af);
	    }
	if (victim != ch)
		act("All affects stripped from $N.",ch,0,victim,TO_CHAR);
	else
		send_to_char("All affects stripped from yourself.\n\r",ch);
	return;
}
	

void do_limcounter(CHAR_DATA *ch,char *argument)
{
    OBJ_INDEX_DATA *pObjIndex;
    char buf[200];
    int vnum;

    if (!is_number(argument))
    {
	send_to_char("Only limstat by vnums.\n\r",ch);
	return;
    }

    vnum = atoi(argument);

    pObjIndex = get_obj_index(vnum);
    if (pObjIndex == NULL)
    {
	send_to_char("Not found.\n\r",ch);
	return;
    }

    sprintf(buf,"Obj vnum %d,Max: %d, Count %d.",vnum, pObjIndex->limtotal,
pObjIndex->limcount);
    send_to_char(buf,ch);
    send_to_char("\n\r",ch);
    return;
}


void do_classes(CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    int iRace;
    int iClass;

    for (iRace = 1;  race_table[iRace].name != NULL; iRace++)
	{
	if (!race_table[iRace].pc_race)
		break;
	sprintf(buf,"\n\r%s:\n\r",race_table[iRace].name);
	send_to_char(buf,ch);
	for (iClass = 1; iClass < MAX_CLASS; iClass++)
	  {
	  if (pc_race_table[iRace].classes[iClass] == 1)
		{
		sprintf(buf,"%s ", (RSTR)CClass::GetClass(iClass)->name);
		send_to_char(buf,ch);
		}
	  }
	}
	send_to_char("\n\r",ch);
  	return;
}


void do_rinfo(CHAR_DATA *ch,char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    ROOM_INDEX_DATA *location;
    int i;
    one_argument( argument, arg );
    location = ( arg[0] == '\0' ) ? ch->in_room : find_location( ch, arg );
    if ( location == NULL )
    {
	send_to_char( "No such location.\n\r", ch );
	return;
    }
    sprintf(buf,"Room %d flags (%s):\n\r",location->vnum,location->name);
    if (IS_SET(location->area->area_flags, AREA_EXPLORE))
	strcat(buf,"AREA_EXPLORE\n\r");
    if (IS_SET(location->area->area_flags, AREA_NO_NEWBIES))
        strcat(buf,"AREA_NO_NEWBIES\n\r");
    if (IS_SET(location->area->area_flags, AREA_UNGHOST))
        strcat(buf,"AREA_UNGHOST\n\r");
    for(i = 0; room_flags[i].name!=NULL; i++) {
	if(IS_SET(location->room_flags,room_flags[i].bit))
	{
		strcat(buf,"ROOM ");
		strcat(buf,upstring(room_flags[i].name));
		strcat(buf,"\n\r");
	} }
    if(location->room_flags==0 && location->area->area_flags==0)
	strcat(buf,"(no flags)\n\r");
    send_to_char(buf,ch);
}


void do_deathmessage(CHAR_DATA *ch,char *argument)
{
    char buf[MAX_STRING_LENGTH];

    if ( !IS_NPC(ch) ) {
		smash_tilde( argument );

		if (argument[0] == '\0') {
		    sprintf(buf,"Your death message is %s\n\r",ch->pcdata->imm_death);
		    send_to_char(buf,ch);
		    return;
		}

		if ( strstr(argument,ch->name) == NULL)
			return send_to_char("You must include your name.\n\r",ch);
	
		free_pstring( ch->pcdata->imm_death );
		ch->pcdata->imm_death = palloc_string( argument );

        sprintf(buf,"Your death message is now %s\n\r",ch->pcdata->imm_death);
        send_to_char(buf,ch);
    }

    return;
}


void do_max_limits(CHAR_DATA *ch,char *argument)
{
/*
    OBJ_DATA *obj;
    OBJ_DATA *o_next;
    int count; */
    char arg[MAX_INPUT_LENGTH];
    OBJ_INDEX_DATA *pobj;
     int vnum;
    one_argument(argument,arg);

    if (arg[0] == '\0')
    {
	send_to_char("Max limit on which items?\n\r",ch);
	return;
    }
    vnum = 0;

    if (is_number(arg))
    vnum = atoi(arg);

    if (vnum != 0)
    {
    pobj = get_obj_index(vnum);
    if (pobj == NULL)
	{
	send_to_char("No object by that vnum exists.\n\r",ch);
	return;
	}
	if (pobj->limtotal == 0)
	{
	send_to_char("That item is not limited.\n\r",ch);
	return;
	}
/*
    count = 0;
    for (obj = object_list; obj != NULL; obj = o_next)
	{
	o_next = obj->next;
	if (obj->pIndexData->vnum == pobj->vnum)
		count++;
	}
*/

    if (pobj->limcount >= pobj->limtotal )
	{
send_to_char("That item is already at it's max count.\n\r",ch);
	return;
	}

	pobj->limcount = pobj->limtotal;
	send_to_char("Item is now maxxed.\n\r",ch);
	return;
    }
    else
	{
	send_to_char("You must give the vnum of the object to be maxxed.\n\r",ch);
	return;
	}

    return;
}

/* Add Apply */
void do_addapply(CHAR_DATA *ch, char *argument)
{
	OBJ_DATA *obj;
	AFFECT_DATA *paf, *paf_next, af;
	OBJ_APPLY_DATA *app, *app_next;
	char arg1[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	char arg3[MAX_INPUT_LENGTH];
	int modifier, location;
	
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
	argument = one_argument( argument, arg3 );
	
	if ( arg1[0] == '\0' || arg2[0] == '\0')
	{
		send_to_char("Syntax for applies: addapply <object> <apply type> <value>\n\r",ch);
		send_to_char("                    addapply <object> clear\n\r",ch);
		send_to_char("Apply Types: hp str dex int wis con mana\n\r", ch);
		send_to_char("             ac move hitroll damroll saves\n\r\n\r", ch);
		send_to_char("Syntax for affects: addapply <object> affect <affect name>\n\r",ch);
		send_to_char("Affect Names: blind invisible detect_evil detect_invis detect_magic\n\r",ch);
		send_to_char("              detect_hidden detect_good sanctuary infrared\n\r",ch);
		send_to_char("              curse poison protect_evil protect_good sneak hide sleep charm\n\r", ch);
		send_to_char("              flying pass_door haste calm plague weaken dark_vision berserk\n\r", ch);
		send_to_char("              waterbreath regeneration slow\n\r", ch);
		return;
	}
	
	if ((obj = get_obj_world(ch,arg1)) == NULL)
		return send_to_char("No such object exists!\n\r",ch);
	
	if(!str_prefix(arg2,"clear")) {
		for (paf = obj->charaffs; paf; paf = paf_next) {
	    	paf_next = paf->next;
		   	free_affect(paf);
		}
		obj->charaffs = NULL;

		for (app = obj->apply; app; app = app_next) {
			app_next = app->next;
		}
		obj->apply = NULL;
		
		act("All affects removed from $p.",ch,obj,0,TO_CHAR);
		return;
	}
	if(arg3[0]=='\0')
		return;
	if (!str_prefix(arg2,"hp"))
		location=APPLY_HIT;
	else if (!str_prefix(arg2,"str"))
		location=APPLY_STR;
	else if (!str_prefix(arg2,"dex"))
		location=APPLY_DEX;
	else if (!str_prefix(arg2,"int"))
		location=APPLY_INT;
	else if (!str_prefix(arg2,"wis"))
		location=APPLY_WIS;
	else if (!str_prefix(arg2,"con"))
		location=APPLY_CON;
	else if (!str_prefix(arg2,"mana"))
		location=APPLY_MANA;
	else if (!str_prefix(arg2,"move"))
		location=APPLY_MOVE;
	else if (!str_prefix(arg2,"ac"))
		location=APPLY_AC;
	else if (!str_prefix(arg2,"hitroll"))
		location=APPLY_HITROLL;
	else if (!str_prefix(arg2,"damroll"))
		location=APPLY_DAMROLL;
	else if (!str_prefix(arg2,"saves"))
		location=APPLY_SAVING_SPELL;
	else if (!str_prefix(arg2,"affect"))
		location=APPLY_SPELL_AFFECT;
	else
	{
		send_to_char("That apply is not possible!\n\r",ch);
		return;
	}
	
	if (location!=APPLY_SPELL_AFFECT) {
		app = new_apply_data();
		app->location = location;
		if (is_number(arg3))
			modifier = atoi(arg3);
		else
			return send_to_char("Applies require a numerical value.\n\r",ch);
		app->modifier = modifier;
		app->next = obj->apply;
		obj->apply = app;
	} else {
		init_affect(&af);
		af.where = TO_AFFECTS;
		af.type = skill_lookup(arg3);
		if (af.type == -1)
			return send_to_char("Invalid affect.\n\r",ch);
		af.level = obj->level;
		af.duration = -1;
		af.location = 0;
		af.modifier = 0;
		af.owner = NULL;
		charaff_to_obj(obj,&af);
	}

	send_to_char("Ok.\n\r", ch);
}

void log_naughty( CHAR_DATA *ch, char *argument, int logtype )
{
	FILE *fp;
   	char *strtime;

    	strtime                    = ctime( &current_time );
    	strtime[strlen(strtime)-1] = '\0';	
   if ( ( fp = fopen( GOD_LOG_FILE, "a" ) ) != NULL )
    {
		if(logtype==1)
			fprintf(fp,"%s: LOAD- %s is loading %s.\n",strtime,ch->name,argument);
		if(logtype==2)
			fprintf(fp,"%s: SET- %s is setting %s.\n",strtime,ch->name,argument);
		if(logtype==3)
			fprintf(fp,"%s: STRING- %s is stringing %s.\n",strtime,ch->name,argument);
		if(logtype==5)
			fprintf(fp,"%s: INDUCT- %s is inducting %s.\n",strtime,ch->name,argument);
      fclose(fp);
    }
}

void do_vstat(CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   char *choppedarg;
    choppedarg = one_argument(argument,arg);

    if (arg[0] == '\0')
    {
        send_to_char("Syntax:\n\r",ch);
        send_to_char("  vstat mob [vnum]\n\r",ch);
        send_to_char("  vstat obj [vnum]\n\r",ch);
	send_to_char("Default is obj.\n\r",ch);
        return;
    }

    if (!str_cmp(arg,"mob"))
    {
        do_vmstat(ch,choppedarg);
        return;
    }

    if (!str_cmp(arg,"obj"))
    {
        do_vostat(ch,choppedarg);
        return;
    }

    /* default = obj */
    	do_vostat(ch,argument);
}

void do_vmstat( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    MOB_INDEX_DATA *pMobIndex;
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];

    one_argument( argument, arg );

    if ( arg[0] == '\0' || !is_number(arg) )
    {
        send_to_char( "Syntax: vstat mob [vnum]\n\r", ch );
        return;
    }

    if ( ( pMobIndex = get_mob_index( atoi( arg ) ) ) == NULL )
    {
        send_to_char( "No mob has that vnum.\n\r", ch );
        return;
    }

    victim = create_mobile( pMobIndex );
    char_to_room( victim, ch->in_room );
    sprintf( buf, "%s", victim->name );
    do_mstat( ch, buf );
    extract_char( victim, TRUE);
    return;
}

void do_vostat( CHAR_DATA *ch, char *argument )
{
    extern int top_obj_index;
    char buf[MAX_STRING_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    OBJ_INDEX_DATA *pObjIndex=NULL;
    OBJ_DATA *obj;
    bool found = FALSE;
    int vnum, nMatch=0;
    char *blah;
    blah = one_argument( argument, arg1 );

    if ( arg1[0] == '\0')
    {
        send_to_char( "Syntax: vostat [vnum] or vostat [name]\n\r", ch );
        return;
    }
    if(is_number(arg1))
    {
    if ( ( pObjIndex = get_obj_index( atoi( arg1 ) ) ) == NULL )
    {
        send_to_char( "No object has that vnum.\n\r", ch );
        return;
    }
	found=TRUE;
    }
    if(!is_number(arg1))
	{
	    for ( vnum = 0; nMatch < top_obj_index; vnum++ )
	    {
        	if ( ( pObjIndex = get_obj_index( vnum ) ) != NULL )
	        {
        	nMatch++;
	            if ( is_name( argument, pObjIndex->name ) )
        	    {
                	found=TRUE;
	                break;
        	    }
            }
    	}
    }
    if(!found || !pObjIndex)
    {
	send_to_char("No objects by that name were found.\n\r",ch);
	return;
    }
    pObjIndex->limcount--;
    obj = create_object( pObjIndex, 0 );
    obj_to_room( obj, ch->in_room );
    sprintf( buf, "%s", obj->name );
    do_ostat( ch, buf );
    extract_obj( obj );
    pObjIndex->limcount++;
return;
}

void do_history(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];
	char arg1[MAX_STRING_LENGTH];
	char arg2[MAX_STRING_LENGTH];
	char obuf[MAX_STRING_LENGTH];
	CHAR_DATA *victim;
	int len;
	bool found = FALSE;

	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );

	if (arg1[0] == '\0')
	{
		send_to_char("Syntax: history <character>          = read a character's history\n\r", ch);
		send_to_char("        history <character> + <info> = add to the temporary buffer\n\r", ch);
		send_to_char("        history <character> -        = remove the last line from the temporary buffer\n\r", ch);
		send_to_char("        history <character> show     = shows the temporary buffer\n\r", ch);
		send_to_char("        history <character> add      = write the temporary buffer to their pfile\n\r", ch);
		send_to_char("        history <character> clear    = clear a player's history (IMP ONLY)\n\r",ch);
		return;
	}

	if ((victim = get_char_world(ch,arg1)) == NULL)
	{
		send_to_char("They aren't here, attempting offline history...\n\r",ch);
		sprintf(buf,"finger %s history",arg1);
		interpret(ch,buf);
		return;
	}

	if (IS_NPC(victim))
	{
		send_to_char("You can't history a mob.\n\r",ch);
		return;
	}
	
	if(victim->level>=ch->level && get_trust(ch)<60)
	{
		send_to_char("Access denied.\n\r",ch);
		return;	
	}

	if (arg2[0] == '\0')
	{
		show_history(ch,victim);
		return;
	}
	if (!strcmp(arg2,"clear"))
	{
		if(!IS_IMP(ch))
		{
			send_to_char("You are not permitted to complete that operation.\n\r",ch);
			return;
		}
		victim->pcdata->history_buffer=NULL;
	}
	if (!strcmp(arg2,"add")){
		if (victim->pcdata->temp_history_buffer == NULL || victim->pcdata->temp_history_buffer[0] == '\0')
		{
			send_to_char("The temporary buffer for that character is empty.\n\r",ch);
			return;
		} else {
			add_history(ch,victim,victim->pcdata->temp_history_buffer);
			free_pstring(victim->pcdata->temp_history_buffer);
			victim->pcdata->temp_history_buffer = NULL;
			show_history(ch,victim);
		}
		return;
	}

	if (!strcmp(arg2,"show"))
	{
		show_temp_history(ch,victim);
		return;
	}

	if (!strcmp(arg2,"+"))
	{
		if (argument[0] != '\0')
		{
			obuf[0] = '\0';
			if (victim->pcdata->temp_history_buffer) {
			strcat(obuf,victim->pcdata->temp_history_buffer);
			}
			smash_tilde(argument);
			strcat(obuf,argument);
			strcat(obuf,"\n\r");

			free_pstring(victim->pcdata->temp_history_buffer);
			victim->pcdata->temp_history_buffer = palloc_string(obuf);
		} else {
			send_to_char("Add what?\n\r",ch);
			return;
		}
		show_temp_history(ch,victim);
	}


	if (!strcmp(arg2,"-"))
	{

		if (victim->pcdata->temp_history_buffer == NULL || victim->pcdata->temp_history_buffer[0] == '\0')
		{
			send_to_char("No lines left to remove.\n\r",ch);
			return;
		}

		strcpy(obuf,victim->pcdata->temp_history_buffer);

		for (len = strlen(obuf); len > 0; len--)
		{
			if (obuf[len] == '\r')
			{
				if (!found)
				{
					if (len > 0)
					{
						len--;
					}
					found = TRUE;
				}
				else
				{
					obuf[len + 1] = '\0';
					free_pstring(victim->pcdata->temp_history_buffer);
					victim->pcdata->temp_history_buffer = palloc_string(obuf);
					show_temp_history(ch,victim);
					return;
				}
			}
		}
		obuf[0] = '\0';
		free_pstring(victim->pcdata->temp_history_buffer);
		victim->pcdata->temp_history_buffer = palloc_string(obuf);
		show_temp_history(ch,victim);
		return;
	}
	return;
}

void add_history(CHAR_DATA *ch, CHAR_DATA *victim, char *string)
{
	char buf[MAX_STRING_LENGTH];
	char obuf[MAX_STRING_LENGTH];
	char *strtime;

	strtime = ctime( &current_time );
	chomp(strtime);


	if (victim->pcdata->history_buffer)
		sprintf(buf,victim->pcdata->history_buffer);
	else
		strcpy(buf,"");

	sprintf(obuf,"Added by %s (%s at %d hours)\n\r",
		ch ? ch->name : "AUTO", strtime, (int)((victim->played + current_time - victim->logon) / 3600));
	strcat(buf,obuf);
	strcat(buf,string);
	strcat(buf,"\n\r");

	free_pstring(victim->pcdata->history_buffer);
	victim->pcdata->history_buffer = palloc_string(buf);

	return;
}

void show_temp_history(CHAR_DATA *ch, CHAR_DATA *victim)
{
	BUFFER *output;

	output = new_buf();
	send_to_char(victim->name,ch);
	send_to_char("'s temporary history buffer:\n\r",ch);

	if (victim->pcdata->temp_history_buffer == NULL || victim->pcdata->temp_history_buffer[0] == '\0')
	{
		send_to_char("Temporary buffer is clear.\n\r", ch );
	} else {
		add_buf(output,victim->pcdata->temp_history_buffer);
		page_to_char(buf_string(output),ch);
		free_buf(output);
	}
	return;
}

void show_history(CHAR_DATA *ch, CHAR_DATA *victim)
{
	BUFFER *output;

	output = new_buf();	
	send_to_char(victim->name,ch);
	send_to_char("'s player history:\n\r",ch);
	if (victim->pcdata->history_buffer == NULL || victim->pcdata->history_buffer[0] == '\0') {
		send_to_char("No pfile history available.\n\r",ch);
	} else {
		add_buf(output,victim->pcdata->history_buffer);
		page_to_char(buf_string(output),ch);
		free_buf(output);
	}
	return;
}

void do_afk(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];

	if (IS_SET(ch->comm, COMM_AFK))
	{
		REMOVE_BIT(ch->comm, COMM_AFK);
		send_to_char("AFK removed.\n\r",ch);
		wiznet("$N is no longer AFK.",ch,NULL,0,0,0);
		do_replay(ch,"");
	} else {
		SET_BIT(ch->comm, COMM_AFK);
		send_to_char("AFK set.\n\r",ch);
		if (argument[0] != '\0')
			sprintf(buf,"$N is now AFK (%s).",argument);
		else
			sprintf(buf,"$N is now AFK.");
		wiznet(buf,ch,NULL,0,0,0);
 	}

	return;
}


void do_empower(CHAR_DATA *ch, char *argument)
{
    char arg[MAX_STRING_LENGTH];
    char arg2[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    argument = one_argument( argument, arg );
    argument = one_argument( argument, arg2 );

    if ( arg[0] == '\0' || arg2[0] == '\0')
    {
        send_to_char( "Syntax: empower <char> <yes/no>\n\r", ch );
        return;
    }

	if (IS_SWITCHED(ch))
		return send_to_char("You can't do that while switched!\n\r", ch);
	
    if ( ( victim = get_char_world( ch, arg ) ) == NULL || IS_NPC(victim))
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }


    if (!str_prefix(arg2,"no"))
    {
		if (IS_SET(victim->act, PLR_EMPOWERED))
		{	
        	REMOVE_BIT(victim->act, PLR_EMPOWERED);
        	send_to_char( "The Immortals have revoked your empowerment!\n\r",victim );
			act("You have UNEMPOWERED $N, until you notify $M, $E doesn't know.", ch, 0, victim, TO_CHAR);
			sprintf(buf,"$N revokes %s's empowerment.",victim->name);
			wiznet(buf,ch,NULL,WIZ_PENALTIES,0,0);
			sprintf(buf,"AUTO: Unempowered by %s.\n\r",ch->true_name);
			add_history(NULL,victim,buf);
			return;
		}
		else
			return send_to_char( "They are not empowered in the first place.\n\r", ch );
    }
    else if (!str_prefix(arg2,"yes"))
    {
		if (!IS_SET(victim->act, PLR_EMPOWERED))
		{
        	SET_BIT(victim->act, PLR_EMPOWERED);
			act("You have EMPOWERED $N, until you notify $M, $E doesn't know.", ch, 0, victim, TO_CHAR);
			sprintf(buf,"$N empowers %s.",victim->name);
			wiznet(buf,ch,NULL,WIZ_PENALTIES,0,0);
			sprintf(buf,"AUTO: Empowered by %s.\n\r",ch->true_name);
			add_history(NULL,victim,buf);
			return;
		}
		else
			return send_to_char( "They are already empowered.\n\r", ch );
    }
    else
		return send_to_char( "Syntax: empower <char> <yes/no>\n\r", ch );

    return;
}

void do_raffects(CHAR_DATA *ch, char *argument)
{
	ROOM_AFFECT_DATA *paf;
	RUNE_DATA *rune;
	char buf[MAX_STRING_LENGTH];
	bool found = FALSE;
	int i = 0;
	if (ch->in_room->affected != NULL)
	{
		found = TRUE;
		send_to_char("The room is affected by:\n\r", ch);

		for (paf = ch->in_room->affected; paf != NULL; paf = paf->next)
		{
			if (paf->aftype == AFT_SPELL)
				sprintf( buf, "Spell: '%s' ", skill_table[paf->type].name);
			if (paf->aftype == AFT_SKILL)
				sprintf( buf, "Skill: '%s' ", skill_table[paf->type].name);
			if (paf->aftype == AFT_POWER)
				sprintf( buf, "Power: '%s' ", skill_table[paf->type].name);
			if (paf->aftype == AFT_COMMUNE)
				sprintf( buf, "Commune: '%s' ", skill_table[paf->type].name);
			if (paf->aftype != AFT_SPELL && paf->aftype!=AFT_SKILL && paf->aftype!=AFT_POWER && paf->aftype!=AFT_MALADY && paf->aftype!=AFT_COMMUNE)
				sprintf( buf, "Spell: '%s' ", skill_table[paf->type].name);
			send_to_char(buf, ch);

			sprintf(buf,"modifies %s by %d for %d hours with %s-bits %s, owner %s, level %d.\n\r",
			raffect_loc_name(paf->location),
			paf->modifier,
			paf->duration/2,
			paf->where == TO_ROOM_FLAGS ? "flag" : paf->where == TO_ROOM_CONST? "const" : "aff",
			"NULL",
        		paf->owner != NULL ? paf->owner->name : "none",
			paf->level
			);
			send_to_char(buf,ch);
		}
	}
	if(ch->in_room->has_rune)
	{
		for(rune = ch->in_room->rune; rune; rune = rune->next_content)
		{
			sprintf(buf,"Rune '%s' placed in room by %s, level %d, duration %d hours.\n\r",
				skill_table[rune->type].name,
				!IS_NPC(rune->owner) ? rune->owner->true_name : rune->owner->name,
				rune->level, rune->duration);
			send_to_char(buf,ch);
		}
	}
	for(i = 0; i<6; i++)
	{
		if(ch->in_room->exit[i] && ch->in_room->exit[i]->has_rune == TRUE)
		{
			rune = ch->in_room->exit[i]->rune;
			sprintf(buf,"Rune '%s' placed on %s door by %s, level %d, duration %d hours.\n\r",
			skill_table[rune->type].name, direction_table[i].name,
			!IS_NPC(rune->owner) ? rune->owner->true_name : rune->owner->name,
			rune->level, rune->duration);
			send_to_char(buf,ch);
			found = TRUE;
		}
	}
	if(!found)
		send_to_char("The room is not affected by anything.\n\r",ch);
	return;
}

void do_rastrip(CHAR_DATA *ch,char *argument)
{
	ROOM_AFFECT_DATA *af, *af_next;
	ROOM_INDEX_DATA *location = NULL;

	if (argument[0] == '\0')
	{
		location = ch->in_room;
	}

	if (location == NULL && (location = find_location(ch, argument)) == NULL)
	{
		send_to_char("No such room exists.\n\r", ch);
		return;
	}


	for (af = location->affected; af != NULL; af = af_next)
	{
		af_next = af->next;
		affect_remove_room(location,af);
	}

	act("All affects stripped from '$t'.",ch,location->name,0,TO_CHAR);

	return;
}

void do_aastrip(CHAR_DATA *ch, char *argument)
{
	AREA_AFFECT_DATA *af, *af_next;
	AREA_DATA *area = ch->in_room->area;

	for (af = area->affected; af; af= af_next) {
		af_next = af->next;
		affect_remove_area(area,af);
	}

	act("All affects stripped from '$t'.",ch,area->name,0,TO_CHAR);

	return;
}

void do_oastrip(CHAR_DATA *ch, char *argument)	
{
	OBJ_DATA *obj;
	OBJ_AFFECT_DATA *af, *af_next;
	char arg[MSL];
	
	one_argument(argument,arg);

	if (arg[0] == '\0')
		return send_to_char("Strip affects from which object?\n\r",ch);
	else
		obj = get_obj_world(ch, arg);

	if (!obj)
		return send_to_char("Strip affects from which object?\n\r",ch);

	for (af = obj->affected; af != NULL; af = af_next) {
		af_next = af->next;
		if (IS_SET(af->bitvector, AFF_PERMANENT))
			continue;
		affect_remove_obj (obj, af, TRUE);
	}

	act("All affects stripped from $p.",ch,obj,0,TO_CHAR);

	return;
}

void do_givexp(CHAR_DATA *ch, char *argument)
{
	char arg1[MSL],arg2[MSL],temp[MSL];
	CHAR_DATA *victim;
	int xp;
	
	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);

	if(!*argument)
		return send_to_char("Syntax:   givexp <character> <# xp> <message they receive>\n\r", ch);

	if((victim = get_char_world(ch,arg1)) == NULL)
		return send_to_char("You can't find them.\n\r", ch);

	if(!is_number(arg2))
		return do_givexp(ch,"");

	xp = atoi(arg2);
	
	if((xp > 3000) && (get_trust(ch) < 56))
		return send_to_char("You may not give more than 3000 experience.\n\r",ch);

	if(!str_cmp(argument,""))
		sprintf(temp,"You recieve %d experience points for outstanding performance.\n\r", xp);
	else
		sprintf(temp,"%s\n\r", argument);

	send_to_char(temp,victim);
	gain_exp(victim,xp);

	send_to_char("Ok.\n\r", ch);
	sprintf(temp,"Log added: %s receives %d experience from %s.\n\r",victim->name,xp,ch->name);
	send_to_char(temp,ch);
	sprintf(temp,"AUTO: %s gave %d experience.\n\r",ch->true_name,xp);
	add_history(NULL,victim,temp);
	return;
}

void do_setpath(CHAR_DATA *ch, char *argument)
{
	char arg1[MSL], temp[MSL];	
	CHAR_DATA *victim;

	argument = one_argument(argument, arg1);

	if (argument[0] == '\0' || arg1[0] == '\0')
		return send_to_char("Syntax: setpath <character> <protector/crusader>\n\r",ch);

	if((victim = get_char_world(ch,arg1)) == NULL)
		return send_to_char("You cannot find that person.\n\r", ch);
	
	if(!strcmp(argument, "protector"))
	{
		victim->pcdata->paladin_path = PALADIN_PROTECTOR;
		group_add(victim,"protector",FALSE);
		send_to_char("You have set that character's path to Protector.\n\r",ch);
		send_to_char("Your path has been set, you will follow that of the Protector of Shalar.\n\r", victim);
		sprintf(temp,"AUTO: %s inducted them into Protector path.\n\r",ch->true_name);
	}
	else if(!strcmp(argument, "crusader"))
	{
		victim->pcdata->paladin_path = PALADIN_CRUSADER;
		group_add(victim,"crusader",FALSE);
		send_to_char("You have set that character's path to Crusader.\n\r",ch);
		send_to_char("Your path has been set, you will follow that of the Crusader of Shalar.\n\r", victim);
		sprintf(temp,"AUTO: %s inducted them into Crusader path.\n\r",ch->true_name);		
	}
	else
		return send_to_char("That's not a paladin path.\n\r",ch);
	add_history(NULL,victim,temp);
}
void do_clearfavors(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim;
	int i;
	
	if (argument[0] == '\0')
		victim = ch;
	else
		victim = get_char_world(ch,argument);

	if (!victim)
		return send_to_char("They aren't here.\n\r",ch);

	for (i = 0; i < MAX_DEVIL; i++) {
		victim->pcdata->devildata[i] = FAVOR_NONE;
		victim->pcdata->greaterdata[i] = FAVOR_NONE;
		victim->pcdata->lesserdata[i] = FAVOR_NONE;
	}

	act("$N's favors cleared.",ch,0,victim,TO_CHAR);

	return;
}

void do_gsnlist(CHAR_DATA *ch,char *argument)
{
	int sn, col=0;
	char buf[MSL];
	BUFFER *output;

	output = new_buf();

	for (sn = 0; sn < MAX_SKILL; sn++)
	{
		if (skill_table[sn].name == NULL) continue;
		sprintf( buf, "%-18s %3d  ", skill_table[sn].name, sn );
		add_buf(output,buf);
		if ( ++col % 3 == 0 ) add_buf(output,"\n\r");
	}

	page_to_char(buf_string(output),ch);
	free_buf(output);

	return;
}

void do_ccl(CHAR_DATA *ch, char *argument)
{
	clear_cabal_leaders( ch, cabal_lookup(argument), "You are gone.");
	return;
}

void do_noskills(CHAR_DATA *ch, char *argument)
{
	if(!IS_SET(ch->comm, COMM_SWITCHSKILLS))
	{
		SET_BIT(ch->comm, COMM_SWITCHSKILLS);
		send_to_char("You will now have every skill when switched into a mob.\n\r",ch);
	}
	else
	{
		REMOVE_BIT(ch->comm, COMM_SWITCHSKILLS);
		send_to_char("You will no longer have your normal immortal skills when switched into a mob.\n\r",ch);
	}
}

void do_buglist(CHAR_DATA *ch, char *argument)
{
	char arg1[MSL], arg2[MSL], query[MSL], str[200];
	MYSQL *conn;
	MYSQL_RES *res_set = NULL;
	MYSQL_ROW row = NULL;
	if(argument[0] == '\0') {
		send_to_char("Syntax:  buglist <number>                          - detailed information about bug <number>\n\r",ch);
		send_to_char("         buglist last <number>                     - list <number> most recent bugs\n\r",ch);
		send_to_char("         buglist add <brief one line summary>      - report bug (it prompts for detailed description)\n\r",ch);
		send_to_char("         buglist done <number>                     - mark bug <number> as resolved\n\r",ch);
		return;
	}
	if(IS_NPC(ch))
		return send_to_char("Die.\n\r",ch);
	argument = one_argument(argument, arg1);
	//status 0 = unresolved, 1 = resolved, 2 = my head is on fire
	if(!strcmp(arg1,"auto"))
	{
		sprintf(query,"SELECT * from buglist WHERE Status = 0 ORDER BY ID desc LIMIT 8");
		sprintf(str,"Displaying last 8 unresolved bugs.\n\r");
	}
	else if(is_number(arg1))
	{
        	conn = open_conn();
	        if(!conn)
                	return send_to_char("Error opening bug database.\n\r",ch);
		sprintf(query,"SELECT * from buglist WHERE ID = %d", atoi(arg1));
		sprintf(str,"Displaying bug #%d:\n\r",atoi(arg1));
		send_to_char(str,ch);
		mysql_query(conn,query);
		res_set = mysql_store_result(conn);
		if(!res_set || !(row = mysql_fetch_row(res_set)))
			send_to_char("Unable to find that bug.\n\r",ch);
		else {
		sprintf(query,"Added by: %-15s Date: %-15s\n\rStatus: %s\n\rSummary: %s\n\rDescription: %s\n\r",
			row[1], row[2], atoi(row[5]) == 1 ? "Resolved" : "Unresolved", row[3], row[4]);
		send_to_char(query,ch);
		}
		mysql_free_result(res_set);
		do_disc(conn);
		return;
	}
	else if(!strcmp(arg1,"last"))
	{
		argument = one_argument(argument, arg2);
		if(!is_number(arg2))
			return send_to_char("Invalid number of last bugs to display.\n\r",ch);
		sprintf(query,"SELECT * from buglist ORDER BY ID desc LIMIT %d",atoi(arg2));
		sprintf(str,"Displaying last %d bugs.\n\r",atoi(arg2));
	}
	else if(!strcmp(arg1,"add"))
	{
		ch->pcdata->temp_str = palloc_string(argument);
		send_to_char("Please enter a description of the bug with as much information as possible:\n\r",ch);
		enter_text(ch, buglist_end_fun);
		return;
	}
	else if(!strcmp(arg1,"done"))
	{
		argument = one_argument(argument, arg2);
                if(!is_number(arg2))
                        return send_to_char("Invalid number of bug to complete.\n\r",ch);
		sprintf(query,"UPDATE buglist SET Status = 1 WHERE ID = %d",atoi(arg2));
		one_query(query);
		send_to_char("Bug marked as resolved.\n\r",ch);
		return;
	}
	else return send_to_char("Invalid option.\n\r",ch);
        conn = open_conn();
        if(!conn)
                return send_to_char("Error opening bug database.\n\r",ch);
        send_to_char(str,ch);
        mysql_query(conn,query);
        res_set = mysql_store_result(conn);
        if(!res_set)
                send_to_char("No bugs found.\n\r",ch);
        else {
		send_to_char("ID    Added By    Summary\n\r",ch);
		send_to_char("---   ---------   ------------\n\r",ch);
                while((row = mysql_fetch_row (res_set)) != NULL)
                {
	                sprintf(query,"%-5s %-11s %s %s\n\r",
        	            row[0], row[1], row[3], atoi(row[5]) == 1 ? "(SOLVED)" : "");
                	send_to_char(query,ch);
		}
        }
        mysql_free_result(res_set);
        do_disc(conn);
}

void buglist_end_fun(CHAR_DATA *ch, char *argument)
{
	char query[MSL];
	sprintf(query, "INSERT INTO buglist VALUES(NULL, '%s', '%s', '%s', '%s', 0)",
		ch->true_name, log_time(), ch->pcdata->temp_str, argument);
	one_query(query);
	free_pstring(ch->pcdata->temp_str);
	send_to_char("Bug has been reported.\n\r",ch);
}

void do_constdump(CHAR_DATA *ch, char *argument)
{
	FILE *fp = fopen("../code/constdump.txt","w"), *fp2;
	int sn, i;
	if(!fp)
		return;

	for(sn = 0; ; sn++)
	{
		fprintf(fp, "\t{\n");
		fprintf(fp, "\t\t\"%s\"\t\t{ ",skill_table[sn].name);
		for(i = 0; i<MAX_CLASS; i++)
			fprintf(fp, "%i, ", skill_table[sn].skill_level[i]);
		fprintf(fp, "},\n");
		fprintf(fp, "\t\t{ ");
		fprintf(fp, "},\n");
		fprintf(fp, "\t\tspell_null,\t\t%s,", skill_table[sn].target == TAR_CHAR_OFFENSIVE ? "TAR_CHAR_OFFENSIVE" : 
			skill_table[sn].target == TAR_CHAR_DEFENSIVE ? "TAR_CHAR_DEFENSIVE" :
			skill_table[sn].target == TAR_CHAR_AMBIGUOUS ? "TAR_CHAR_AMBIGUOUS" :
			skill_table[sn].target == TAR_IGNORE ? "TAR_IGNORE" :
			skill_table[sn].target == TAR_CHAR_SELF ? "TAR_CHAR_SELF" :
			skill_table[sn].target == TAR_CHAR_GENERAL ? "TAR_CHAR_GENERAL" :
			skill_table[sn].target == TAR_OBJ_INV ? "TAR_OBJ_INV" :
			skill_table[sn].target == TAR_OBJ_CHAR_DEF ? "TAR_OBJ_CHAR_DEF" :
			skill_table[sn].target == TAR_OBJ_CHAR_OFF ? "TAR_OBJ_CHAR_OFF" :
			skill_table[sn].target == TAR_DIR ? "TAR_DIR" :
			skill_table[sn].target == TAR_END ? "TAR_END" : "ERROR");
		fprintf(fp, "\t\t%s,\n", skill_table[sn].minimum_position == POS_DEAD ? "POS_DEAD" :
		skill_table[sn].minimum_position == POS_MORTAL ? "POS_MORTAL" :
		skill_table[sn].minimum_position == POS_INCAP ? "POS_INCAP" :
		skill_table[sn].minimum_position == POS_STUNNED ? "POS_STUNNED" :
		skill_table[sn].minimum_position == POS_SLEEPING ? "POS_SLEEPING" :
		skill_table[sn].minimum_position == POS_RESTING ? "POS_RESTING" :
		skill_table[sn].minimum_position == POS_SITTING ? "POS_SITTING" :
		skill_table[sn].minimum_position == POS_FIGHTING ? "POS_FIGHTING" :
		skill_table[sn].minimum_position == POS_STANDING ? "POS_STANDING" : "ERROR");
		fprintf(fp,"\t\t&gsn_none,\t%s%s%s%s,\t%d,\t%d,\n", skill_table[sn].dispel & CAN_DISPEL ? "CAN_DISPEL" : "",
			skill_table[sn].dispel & CAN_CANCEL ? "CAN_CANCEL" : "",
			skill_table[sn].dispel & CAN_CLEANSE ? "CAN_CLEANSE" : "",
			skill_table[sn].dispel & CAN_PURIFY ? "CAN_PURIFY" : "",
			skill_table[sn].min_mana, skill_table[sn].beats);
		fprintf(fp,"\t\t\"%s\",\t\"%s\",\t\"%s\",\t%s\n", skill_table[sn].noun_damage, skill_table[sn].msg_off, 
			skill_table[sn].room_msg_off, skill_table[sn].ctype == CMD_SPELL ? "CMD_SPELL" : 
			skill_table[sn].ctype == CMD_COMMUNE ? "CMD_COMMUNE" :
			skill_table[sn].ctype == CMD_POWER ? "CMD_POWER" :
			skill_table[sn].ctype == CMD_BOTH ? "CMD_BOTH" :
			skill_table[sn].ctype == CMD_RUNE ? "CMD_RUNE" :
			skill_table[sn].ctype == CMD_NONE ? "CMD_NONE" : "ERROR");
		fprintf(fp,"\t},\n\n");
		if(skill_table[sn].target == TAR_END)
			break;
	}
	fclose(fp);
	send_to_char("Const.c dump completed and successful.\n\r",ch); 
	return;
}

void do_interpdump(CHAR_DATA *ch, char *argument)
{
	FILE *fp = fopen("/home/rift/code/interpdump.txt","w");
	int i=0,j=0,k=0;
	int bit=0;	
	
	for( i = 0; cmd_table[i].do_fun != NULL; i++)
	{
		bit=0;
		if(cmd_table[i].show == 0)
			bit+=2;
		if(cmd_table[i].hide == 1)
			bit+=1;
		fprintf(fp,";%s;0;do_%s;%d;%d;%d;\n",
				cmd_table[i].name,
				cmd_table[i].name,
				cmd_table[i].level,
				cmd_table[i].log,
				bit);
	}
	fclose(fp);

	fp = fopen("/home/rift/code/climate-dump.txt","w");

	for(i = 0; climate_table[i].number!=CLIMATE_ENGLISH ; i++)
	{
		fprintf(fp,"%s;%d;", climate_table[i].name, climate_table[i].number);
		for(j=0; j<MAX_SKY; j++)
		{
			if(j != 0)
				fprintf(fp, ",");

			for(k=0; k<NUM_SEASONS; k++)
			{
				if(k != 0)
					fprintf(fp, "|");
				fprintf(fp, "%d", climate_table[i].skyfreqs[k][j]);
			}
		}
		fprintf(fp,";");
		for(j=0; j<MAX_TEMP; j++)
		{
			if(j != 0)
				fprintf(fp, ",");

			for(k=0; k<NUM_SEASONS; k++)
			{
				if(k != 0)
					fprintf(fp, "|");
				fprintf(fp, "%d", climate_table[i].tempfreqs[k][j]);
			}
		}
		fprintf(fp,";\n");
	}

	fclose(fp);
}
	

void do_racedump(CHAR_DATA *ch, char *argument)
{
	FILE *fp = fopen("/home/rift/code/racdump.txt","w");
	int race=0,i=0;
	long temp_bit=0;
	char buf[MSL];

	for ( race = 0; race_table[race].name != NULL; race++ )
	{
		fprintf(fp,";%s;%s;%s;%s;", race_table[race].name , race_table[race].name , race_table[race].name, race_table[race].name);
	   	fprintf(fp," %d;", race_table[race].pc_race);
		temp_bit=0;
		for( i=0; i<10; i++)
			if(race_table[race].act[i] != NO_FLAG)
				SET_BIT_OLD( temp_bit, race_table[race].act[i]);
		fprintf(fp," %ld;", temp_bit);
		temp_bit=0;
		for( i=0; i<10; i++)
            if(race_table[race].aff[i] != NO_FLAG)
                SET_BIT_OLD( temp_bit, race_table[race].aff[i]);
        fprintf(fp," %ld;", temp_bit);
		temp_bit=0;
		for( i=0; i<10; i++)
			if(race_table[race].off[i] != NO_FLAG)
				SET_BIT_OLD( temp_bit, race_table[race].off[i]);
		fprintf(fp," %ld;", temp_bit);
		temp_bit=0;
		for( i=0; i<10; i++)
			if(race_table[race].imm[i] != NO_FLAG)
				SET_BIT_OLD( temp_bit, race_table[race].imm[i]);
		fprintf(fp," %ld;", temp_bit);
		temp_bit=0;
		for( i=0; i<10; i++)
			if(race_table[race].res[i] != NO_FLAG)
				SET_BIT_OLD( temp_bit, race_table[race].res[i]);
		fprintf(fp," %ld;", temp_bit);
		temp_bit=0;
		for( i=0; i<10; i++)
			if(race_table[race].vuln[i] != NO_FLAG)
				SET_BIT_OLD( temp_bit, race_table[race].vuln[i]);
		fprintf(fp," %ld;", temp_bit);
		temp_bit=0;
		for( i=0; i<15; i++)
            if(race_table[race].form[i] != NO_FLAG && race_table[race].form[i] < cc)
			{
				temp_bit += (long int)pow(2, race_table[race].form[i]);
				sprintf(buf,"(FORM) %d-%d = %ld\n\r", race, i, (long int)pow(2, race_table[race].parts[i]));
				send_to_char(buf, ch);
			}
		fprintf(fp," %ld;", temp_bit);
		temp_bit=0;
		for( i=0; i<15; i++)
            if(race_table[race].parts[i] != NO_FLAG && race_table[race].parts[i] < cc)
			{
				temp_bit += (long int)pow(2, race_table[race].parts[i]);
				sprintf(buf,"(PARTS) %d-%d = %ld\n\r", race, i, (long int)pow(2, race_table[race].parts[i]));
				send_to_char(buf, ch);
			}
		fprintf(fp," %ld;", temp_bit);
		fprintf(fp," %d; %d; 0; 0; 0; 0; 0; 25; 25; 25; 25; 25;\n", race_table[race].arms, race_table[race].legs);
	}
	fclose(fp);
}
