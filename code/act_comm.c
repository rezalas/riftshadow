/**************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license._doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 **************************************************************************/

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
#include <mysql.h>
#include "merc.h"
#include "recycle.h"
#include "tables.h"
#include "olc.h"
#include "spec.h"
/* command procedures needed */
DECLARE_DO_FUN(do_quit	);
DECLARE_DO_FUN(do_look);
DECLARE_DO_FUN(do_buglist);
DECLARE_DO_FUN(do_undisguise);
DECLARE_AFF_FUN(traitor_pulse);
#define PDEATH_LOG_FILE	"permdeaths.txt"
#define DEATH_LOG_FILE	"tempdeaths.txt"
#define MOB_LOG_FILE	"mobdeaths.txt"

void obj_say_lordly args((CHAR_DATA *ch,OBJ_DATA *obj,char *argument));
void obj_say_heavenly_sceptre args((CHAR_DATA *ch,OBJ_DATA *obj,char *argument));
void mob_say_lady args((CHAR_DATA *ch, char *argument));
void say_prog_summon_demon args((CHAR_DATA *ch,char *argument));
void say_prog_raise_shadowlord args((CHAR_DATA *ch,char *argument));
void say_prog_karguska args((CHAR_DATA *ch,char *argument));
bool check_ooc args((CHAR_DATA *ch, char *argument, char*type));
const char *upstring(const char *i);
const char *lowstring(const char *i);

/* RT code to delete yourself */

void do_delet( CHAR_DATA *ch, char *argument)
{
    send_to_char("You must type the full command to delete yourself.\n\r",ch);
}

void do_delete( CHAR_DATA *ch, char *argument)
{
   char strsave[MAX_INPUT_LENGTH];
   char *cname;
   if (IS_NPC(ch))
	return;

   if (ch->pcdata->confirm_delete)
   {
	if (argument[0] != '\0')
	{
	    send_to_char("Delete status removed.\n\r",ch);
	    ch->pcdata->confirm_delete = FALSE;
	    return;
	}
	else
	{
	    if(ch->level > 30)
		perm_death_log(ch,1);
    	    sprintf( strsave, "%s%s.plr", PLAYER_DIR, capitalize( ch->name ) );
	    wiznet("$N turns $Mself into line noise.",ch,NULL,0,0,0);
	    ch->pause = 0;
	    while (ch->affected) {
	         affect_remove(ch,ch->affected);
          }
	stop_fighting(ch,TRUE);
	act( "$n has left Shalar, never to return.", ch, NULL, NULL, TO_ROOM );	
	cname	= palloc_string(ch->true_name);
	plug_graveyard(ch, 1);
	do_quit_new(ch,"",TRUE);
 	if( ((ch->played + (current_time - ch->logon)) / 3600) >= 15)
		delete_char(cname,TRUE);	// >= 15 hours. Make name unusable.
	else
		delete_char(cname,FALSE);
	free_pstring(cname);
	return;
 	}
    }

    if (argument[0] != '\0')
    {
	send_to_char("Just type delete. No argument.\n\r",ch);
	return;
    }

    send_to_char("Type delete again to confirm this command.\n\r",ch);
    send_to_char("WARNING: this command is irreversible.\n\r",ch);
    send_to_char("Typing delete with an argument will undo delete status.\n\r",
	ch);
    ch->pcdata->confirm_delete = TRUE;

    if( ((ch->played + (current_time - ch->logon)) / 3600) >= 15) {
	send_to_char("ALERT: Deletion will render this name unusable.\n\r",ch);
    }
    wiznet("$N is contemplating deletion.",ch,NULL,0,0,get_trust(ch));
}
	

/* RT code to display channel status */

void do_channels( CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];

    /* lists all channels and their status */
    send_to_char("   channel     status\n\r",ch);
    send_to_char("---------------------\n\r",ch);

    send_to_char("auction        ",ch);
    if (!IS_SET(ch->comm,COMM_NOAUCTION))
      send_to_char("ON\n\r",ch);
    else
      send_to_char("OFF\n\r",ch);

    send_to_char("Q/A            ",ch);
    if (!IS_SET(ch->comm,COMM_NOQUESTION))
      send_to_char("ON\n\r",ch);
    else
      send_to_char("OFF\n\r",ch);

    if (IS_IMMORTAL(ch))
    {
      send_to_char("god channel    ",ch);
      if(!IS_SET(ch->comm,COMM_NOWIZ))
        send_to_char("ON\n\r",ch);
      else
        send_to_char("OFF\n\r",ch);
    }

    send_to_char("tells          ",ch);
    if (!IS_SET(ch->comm,COMM_DEAF))
	send_to_char("ON\n\r",ch);
    else
	send_to_char("OFF\n\r",ch);

    send_to_char("quiet mode     ",ch);
    if (IS_SET(ch->comm,COMM_QUIET))
      send_to_char("ON\n\r",ch);
    else
      send_to_char("OFF\n\r",ch);

    if (IS_SET(ch->comm,COMM_SNOOP_PROOF))
	send_to_char("You are immune to snooping.\n\r",ch);

    if (ch->lines != PAGELEN)
    {
	if (ch->lines)
	{
	    sprintf(buf,"You display %d lines of scroll.\n\r",ch->lines+2);
	    send_to_char(buf,ch);
        }
	else
	    send_to_char("Scroll buffering is off.\n\r",ch);
    }

    if (IS_SET(ch->comm,COMM_NOTELL))
      send_to_char("You cannot use tell.\n\r",ch);

    if (IS_SET(ch->comm,COMM_NOCHANNELS))
     send_to_char("You cannot use channels.\n\r",ch);

    if (IS_SET(ch->comm,COMM_NOEMOTE))
      send_to_char("You cannot show emotions.\n\r",ch);

}

/* RT deaf blocks out all shouts */

void do_deaf( CHAR_DATA *ch, char *argument)
{

   if (IS_SET(ch->comm,COMM_DEAF))
   {
     send_to_char("You can now hear tells again.\n\r",ch);
     REMOVE_BIT(ch->comm,COMM_DEAF);
   }
   else
   {
     send_to_char("From now on, you won't hear tells.\n\r",ch);
     SET_BIT(ch->comm,COMM_DEAF);
   }
}

/* RT quiet blocks out all communication */

void do_quiet ( CHAR_DATA *ch, char * argument)
{
    if (IS_SET(ch->comm,COMM_QUIET))
    {
      send_to_char("Quiet mode removed.\n\r",ch);
      REMOVE_BIT(ch->comm,COMM_QUIET);
    }
   else
   {
     send_to_char("From now on, you will only hear says and emotes.\n\r",ch);
     SET_BIT(ch->comm,COMM_QUIET);
   }
}


void do_replay (CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
    {
	send_to_char("You can't replay.\n\r",ch);
	return;
    }

    if (!ch->pcdata->buffer->string || ch->pcdata->buffer->string[0] == '\0')
    {
	send_to_char("You have no tells to replay.\n\r",ch);
	return;
    }

    page_to_char(buf_string(ch->pcdata->buffer),ch);
    clear_buf(ch->pcdata->buffer);
}

void do_cb( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;

	if (is_affected_area(ch->in_room->area,gsn_interference))
		return send_to_char("The electrical currents in the area block your attempt to communicate.\n\r",ch);

    if (!is_cabal(ch) || cabal_table[ch->cabal].independent)
    {
        send_to_char("You aren't in a cabal.\n\r",ch);
	return;
    }

    if ( argument[0] == '\0' )
    {
      if (IS_SET(ch->comm,COMM_NOCABAL))
      {
        send_to_char("Cabal channel is now ON\n\r",ch);
        REMOVE_BIT(ch->comm,COMM_NOCABAL);
      }
      else
      {
        send_to_char("Cabal channel is now OFF\n\r",ch);
        SET_BIT(ch->comm,COMM_NOCABAL);
      }
      return;
    }
    if(IS_SHIFTED(ch) && !IS_IMMORTAL(ch))
	return send_to_char("You can't seem to form the words.\n\r",ch);

    if(is_affected(ch,gsn_silence))
    {
	send_to_char("You cannot find the words within you.\n\r",ch);
	return;
    }
    if (IS_SET(ch->in_room->room_flags, ROOM_SILENCE))
	return send_to_char("You cannot find the words within you.\n\r",ch);

        if (IS_SET(ch->comm,COMM_NOCHANNELS))
        {
         send_to_char("The gods have revoked your channel priviliges.\n\r",ch);
          return;
        }

	if (is_affected(ch,gsn_imprisonvoice)) {
	    send_to_char("Your vocal cords are frozen solid!\n\r", ch);
	    return;
	}

	if (is_affected(ch, gsn_gag))
		return send_to_char("You are gagged and cannot speak!\n\r", ch);

	check_ooc(ch,argument,"CB");
	
	if (is_affected(ch,gsn_deafen)) {
		sprintf(argument,"%s", upstring(argument));
	}

        REMOVE_BIT(ch->comm,COMM_NOCABAL);
      if (IS_SET(ch->comm,COMM_ANSI)) {
		sprintf( buf, "%s%s: %s%s%s\n\r",
		cabal_table[ch->cabal].who_name, IS_NPC(ch) ? ch->short_descr : ch->true_name, get_char_color(ch,"channels"),
		argument, END_COLOR(ch));
      } else {
		sprintf( buf, "%s%s: %s\n\r", cabal_table[ch->cabal].who_name,
		IS_NPC(ch) ? ch->short_descr : ch->name, argument );
      }
      send_to_char( buf, ch );


    for ( d = descriptor_list; d != NULL; d = d->next )
    {
        if ( d->connected == CON_PLAYING &&
             d->character != ch &&
            ( ( is_same_cabal(ch,d->character) &&
             !IS_SET(d->character->comm,COMM_NOCABAL)
	     && !IS_SET(d->character->in_room->room_flags, ROOM_SILENCE))
        || IS_SET(d->character->comm,COMM_ALL_CABALS) ) )
        {
	    if (IS_SET(d->character->comm,COMM_ANSI)) {
            sprintf( buf, "%s%s: %s%s%s\n\r",
		cabal_table[ch->cabal].who_name, !IS_NPC(ch) && can_see(d->character,ch) ? ch->true_name : PERS(ch,d->character),
		get_char_color(d->character,"channels"), argument,
			END_COLOR(d->character));
	    } else {
            sprintf( buf, "%s%s: %s\n\r", cabal_table[ch->cabal].who_name,
	    	!IS_NPC(ch) && can_see(d->character,ch) ? ch->true_name : PERS(ch,d->character), argument );
	    }
	    send_to_char( buf, d->character );
        }
    }
    return;
}

void do_global(CHAR_DATA *ch,char *argument)
{
    if (IS_SET(ch->comm,COMM_ALL_CABALS))
    {
        send_to_char("You will no longer hear all Cabal channels.\n\r",ch);
        REMOVE_BIT(ch->comm,COMM_ALL_CABALS);
    }
    else
    {
        send_to_char("You will now hear all Cabal channels.\n\r",ch);
        SET_BIT(ch->comm,COMM_ALL_CABALS);
    }
    return;
}


void do_newbie( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *wch;
    int trust;
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];

	trust = ch->level;

	if(ch->level > 25 && !IS_IMMORTAL(ch) && !IS_SET(ch->act,PLR_HEROIMM))
		return send_to_char("Huh?\n\r",ch);
	else
    	if ( argument[0] == '\0' )
		{
    		if (IS_SET(ch->comm,COMM_NONEWBIE))
			{
				send_to_char("Newbie channel is now ON\n\r",ch);
				REMOVE_BIT(ch->comm,COMM_NONEWBIE);
      		} 
			else 
			{
				send_to_char("Newbie channel is now OFF\n\r",ch);
				SET_BIT(ch->comm,COMM_NONEWBIE);
      		}
      		return;
    	}

	if (argument[0] == '\0' )
		return send_to_char("Syntax: newbie <message>\n\r",ch);

	sprintf(buf,": %s", argument);

    REMOVE_BIT(ch->comm,COMM_NONEWBIE);
    
	if (IS_SET(ch->comm,COMM_ANSI)) 
	{
    	sprintf(buf2,"%s[NEWBIE] $n$t%s",get_char_color(ch,"newbie"),END_COLOR(ch));
    	act_new(buf2,ch,buf,0,TO_CHAR,POS_DEAD);
	}
	else
    	act_new("[NEWBIE] $n$t",ch,buf,0,TO_CHAR,POS_DEAD);

    for (wch = char_list; wch != NULL; wch = wch->next)
    {
		if (IS_NPC(wch) && wch->desc == NULL)
			continue;

		if ( (wch->level <= 25 && !IS_SET(wch->comm, COMM_NONEWBIE))
				|| IS_IMMORTAL(wch)
				|| IS_HEROIMM(wch)
				|| (IS_NPC(wch) && (wch->desc != NULL) && IS_IMMORTAL(wch->desc->original)))
		{
			if (IS_SET(wch->comm,COMM_ANSI))
	    	{
            	sprintf(buf2,"%s[NEWBIE] $n$t%s",get_char_color(wch,"newbie"),END_COLOR(wch));
	    		act_new(buf2,ch,buf,wch,TO_VICT,POS_DEAD);
	    	}
	    	else
            	act_new("[NEWBIE] $n$t",ch,buf,wch,TO_VICT,POS_DEAD);
		}
    }

    return;
}


void do_builder( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *wch;
    int trust;
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];

	trust = ch->level;

	if(trust < 52 && !IS_SET(ch->act,PLR_HEROIMM))
		return send_to_char("Huh?\n\r",ch);
	else
    	if ( argument[0] == '\0' )
		{
    		if (IS_SET(ch->comm,COMM_NOBUILDER))
			{
				send_to_char("Builder channel is now ON\n\r",ch);
				REMOVE_BIT(ch->comm,COMM_NOBUILDER);
      		} 
			else 
			{
				send_to_char("Builder channel is now OFF\n\r",ch);
				SET_BIT(ch->comm,COMM_NOBUILDER);
      		}
      		return;
    	}

	if (argument[0] == '\0' )
		return send_to_char("Syntax: builder <message>\n\r",ch);

	sprintf(buf,": %s", argument);

    REMOVE_BIT(ch->comm,COMM_NOBUILDER);
    
	if (IS_SET(ch->comm,COMM_ANSI)) 
	{
    	sprintf(buf2,"%s[BUILDER] $n$t%s",get_char_color(ch,"builder"),END_COLOR(ch));
    	act_new(buf2,ch,buf,0,TO_CHAR,POS_DEAD);
	}
	else
    	act_new("[BUILDER] $n$t",ch,buf,0,TO_CHAR,POS_DEAD);

    for (wch = char_list; wch != NULL; wch = wch->next)
    {
		if (IS_NPC(wch) && wch->desc == NULL)
			continue;

		if ( ( IS_IMMORTAL(wch)) || IS_SET(wch->comm,COMM_BUILDER) || IS_HEROIMM(wch) ||
			(IS_NPC(wch) && (wch->desc != NULL) && IS_IMMORTAL(wch->desc->original)) )
		{
			if (IS_SET(wch->comm,COMM_ANSI))
	    	{
            	sprintf(buf2,"%s[BUILDER] $n$t%s",get_char_color(wch,"builder"),END_COLOR(wch));
	    		act_new(buf2,ch,buf,wch,TO_VICT,POS_DEAD);
	    	}
	    	else
            	act_new("[BUILDER] $n$t",ch,buf,wch,TO_VICT,POS_DEAD);
		}
    }

    return;
}

void do_immtalk( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *wch;
    int trust;
    char arg1[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    int level;

    trust = get_trust(ch);

    if(!IS_IMMORTAL(ch))
		return;
	
    if ( argument[0] == '\0' ) {
    	if (IS_SET(ch->comm,COMM_NOWIZ)) {
			send_to_char("Immortal channel is now ON\n\r",ch);
			REMOVE_BIT(ch->comm,COMM_NOWIZ);
      	} else {
			send_to_char("Immortal channel is now OFF\n\r",ch);
			SET_BIT(ch->comm,COMM_NOWIZ);
      	}
      	return;
    }


	sprintf(buf2,"%s",argument);
	argument = one_argument(argument,arg1);

	if (arg1[0] == '\0' )
		return send_to_char("Syntax: immtalk <level?> <message>\n\r",ch);

	if (!is_number(arg1)) {
		level = 51;
	} else {
		level = atoi(arg1);
		if (level > trust || level < 51) {
			level = 51;
		} else {
			sprintf(buf2,"%s",argument);
		}
	}

	if (buf2[0] == '\0' )
    	return send_to_char("Syntax: immtalk <level?> <message>\n\r",ch);

	if (level > LEVEL_HERO)
		sprintf(buf," [%d]: %s",level,buf2);
	else
		sprintf(buf,": %s",buf2);

    REMOVE_BIT(ch->comm,COMM_NOWIZ);
    if (IS_SET(ch->comm,COMM_ANSI))
	{
    	sprintf(buf2,"%s[IMM] $n$t%s",get_char_color(ch,"immtalk"),END_COLOR(ch));
    	act_new(buf2,ch,buf,0,TO_CHAR,POS_DEAD);
	} 
	else
    	act_new("[IMM] $n$t",ch,buf,0,TO_CHAR,POS_DEAD);

    for (wch = char_list; wch != NULL; wch = wch->next)
    {
		if (IS_NPC(wch) && wch->desc == NULL)
			continue;

		if ( (IS_IMMORTAL(wch) || IS_SET(wch->comm,COMM_IMMORTAL) ||
			(IS_NPC(wch) && (wch->desc != NULL) && IS_IMMORTAL(wch->desc->original))) &&
			get_trust(wch) >= UMAX(level,52)) 
		{
			if (IS_SET(wch->comm,COMM_ANSI))
	    	{
            	sprintf(buf2,"%s[IMM] $n$t%s",get_char_color(wch,"immtalk"),END_COLOR(wch));
	    		act_new(buf2,ch,buf,wch,TO_VICT,POS_DEAD);
	    	}
	    	else
            	act_new("[IMM] $n$t",ch,buf,wch,TO_VICT,POS_DEAD);
		}
    }

    return;
}

/* Used for an object...take this out if you don't want it */
void obj_say_heavenly_sceptre(CHAR_DATA *ch,OBJ_DATA *obj,char *argument)
{
/*int sn;
	sn = -1;

	if (strstr(argument,"i am the wrath of god")
	|| strstr(argument,"I am the wrath of god"))
		sn = skill_lookup("heavenly wrath");

	if (strstr(argument,"feel the force of god")
	|| strstr(argument,"Feel the force of god"))
		sn = skill_lookup("heavenly fire");


	if (sn == -1)
		return;
	
	(*skill_table[sn].spell_fun) (sn,obj->level,ch,NULL,TAR_IGNORE);
	return;*/
}

const char *upstring(const char *i)
{

	static char buf[MAX_STRING_LENGTH];
	char *o;
	char *ori;

	if(i == NULL) {
		return '\0';
	}	
	
	for (o = buf; *i && (unsigned int)(o - buf) < sizeof(buf) - 1; i++, o++)
		*o = UPPER(*i);

	*o = '\0';

	return buf;
}

const char *lowstring(const char *i)
{
	static char lowbuf[MAX_STRING_LENGTH];
	char *o;

	for (o = lowbuf; *i && (unsigned int)(o - lowbuf) < sizeof(lowbuf)-1; i++, o++)
		*o = LOWER(*i);
	
	*o = '\0';
	return lowbuf;
}

void do_say( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *room_char;
    CHAR_DATA *victim;
    OBJ_DATA *char_obj;
	char saymsg[MSL];
    char buf[MAX_STRING_LENGTH];

    if ( argument[0] == '\0' )
		return send_to_char( "Say what?\n\r", ch );

	check_ooc(ch,argument,"SAY");
	
    if(is_affected(ch,gsn_silence))
		return send_to_char("You cannot find the words within you.\n\r",ch);

	if(is_affected(ch,gsn_uppercut))
		return send_to_char("Your jaw is too painful to move.\n\r",ch);

	if (IS_SET(ch->in_room->room_flags, ROOM_SILENCE))
		return send_to_char("You open your mouth, but no sound comes out.\n\r",ch);

    if(IS_SHIFTED(ch) && !IS_IMMORTAL(ch))
		return send_to_char("You can't talk while you're in form.\n\r",ch);
	
    if(is_affected(ch,gsn_mangled))
		return send_to_char("Your torn throat prevents you from getting the words out.\n\r",ch);
	
	if (is_affected(ch,gsn_imprisonvoice))
	    return send_to_char("Your vocal cords are frozen solid!\n\r", ch);

	if (is_affected(ch, gsn_gag))
	{
		act("You are gagged and cannot speak!", ch, NULL, NULL, TO_CHAR);
		act("Muffled sounds emanate from $n's direction as $e attempts in vain to speak through $s gag.", ch, NULL, NULL, TO_ROOM);
		return;
	}

	if (is_affected(ch,gsn_deafen))
		return do_yell(ch,argument);

	if (!str_cmp(ch->name,"Karsus") && strstr(argument,"eing")
		&& !strstr(argument,"being")) {
		send_to_char("***It's 'ING', not 'EING' you fooker!!!***\n\r",ch);
		send_to_char("***It's 'ING', not 'EING' you fooker!!!***\n\r",ch);
		send_to_char("***It's 'ING', not 'EING' you fooker!!!***\n\r",ch);
	}

	if ( argument[strlen(argument) - 1] == '!')
		sprintf(saymsg,"exclaim");
	else if (argument[strlen(argument) - 1] == '?')
		sprintf(saymsg,"ask");
	else
		sprintf(saymsg,"say");

	
	sprintf(buf,"You %s '%s$T%s'",saymsg,get_char_color(ch,"speech"),END_COLOR(ch));
	
	act(buf, ch, NULL, argument, TO_CHAR );

    for ( victim = ch->in_room->people; victim != NULL; victim = victim->next_in_room )
    {

        if ( IS_AWAKE(victim) )
		{
			if (!(IS_IMMORTAL(ch)) && is_affected(victim,gsn_deafen))
			{
				act("$n says something you can't quite hear.",ch,0,victim,TO_VICT);
			}
			else
			{
   				sprintf(buf,"$n %ss '%s$t%s'",saymsg,get_char_color(victim,"speech"),END_COLOR(victim));
				act(buf,ch,argument,victim,TO_VICT);
				if (is_affected(victim,gsn_word_of_command) && strstr(argument,victim->pcdata->command[0]))
					command_execute(victim);
			}
		}
    }

	if(IS_SET(ch->in_room->progtypes, RPROG_SPEECH))
		(ch->in_room->rprogs->speech_prog) (ch->in_room, ch, argument);

  for (room_char = ch->in_room->people; room_char != NULL;
         room_char = room_char->next_in_room)
    {
      if (IS_SET(room_char->progtypes,MPROG_SPEECH) && room_char != ch) {
        (room_char->pIndexData->mprogs->speech_prog) (room_char,ch,argument);
      }
	  if (TRAPS_MEVENT(room_char, TRAP_MSPEECH))
	  	  CALL_MEVENT(room_char,TRAP_MSPEECH,ch,room_char,argument);
    }

	report_cabal_items(ch, argument);

	if (is_affected(ch,gsn_unholy_communion)
		&& (ch->Class()->name == "anti-paladin"))
		check_unholy_communion(ch,argument);

	for (char_obj = ch->carrying; char_obj != NULL;
		char_obj = char_obj->next_content)
	{
		if (IS_SET(char_obj->progtypes,IPROG_SPEECH) && char_obj->pIndexData->iprogs)
			(char_obj->pIndexData->iprogs->speech_prog) (char_obj,ch,argument);
		if(TRAPS_IEVENT(char_obj,TRAP_ISPEECH))
			CALL_IEVENT(char_obj,TRAP_ISPEECH,ch,char_obj,argument);
	}

	for (char_obj = ch->in_room->contents; char_obj != NULL;
		char_obj = char_obj->next_content)
	{
		if (IS_SET(char_obj->progtypes,IPROG_SPEECH) && char_obj->pIndexData->iprogs)
			(char_obj->pIndexData->iprogs->speech_prog) (char_obj,ch,argument);
		if(TRAPS_IEVENT(char_obj,TRAP_ISPEECH))
			CALL_IEVENT(char_obj,TRAP_ISPEECH,ch,char_obj,argument);
	}
    return;
}

void say_to( CHAR_DATA *ch, CHAR_DATA *victim, char *argument, char *extra)
{
	char saymsg[MSL];
    char buf[MAX_STRING_LENGTH];

    if ( argument[0] == '\0' )
		return send_to_char( "Say what?\n\r", ch );

    if(is_affected(ch,gsn_silence))
		return send_to_char("You cannot find the words within you.\n\r",ch);

	if(is_affected(ch,gsn_uppercut))
		return send_to_char("Your jaw is too painful to move.\n\r",ch);

	if (IS_SET(ch->in_room->room_flags, ROOM_SILENCE))
		return send_to_char("You open your mouth, but no sound comes out.\n\r",ch);

    if(IS_SHIFTED(ch) && !IS_IMMORTAL(ch))
		return send_to_char("You can't talk while you're in form.\n\r",ch);
	
    if(is_affected(ch,gsn_mangled))
		return send_to_char("Your torn throat prevents you from getting the words out.\n\r",ch);
	
	if (is_affected(ch,gsn_imprisonvoice))
	    return send_to_char("Your vocal cords are frozen solid!\n\r", ch);

	if (is_affected(ch, gsn_gag))
	{
		act("You are gagged and cannot speak!", ch, NULL, NULL, TO_CHAR);
		act("Muffled sounds emanate from $n's direction as $e attempts in vain to speak through $s gag.", ch, NULL, NULL, TO_ROOM);
		return;
	}

	if ( argument[strlen(argument) - 1] == '!')
		sprintf(saymsg,"exclaim");
	else if (argument[strlen(argument) - 1] == '?')
		sprintf(saymsg,"ask");
	else
		sprintf(saymsg,"say");
	
	if ( IS_AWAKE(victim) ) {
		if (!(IS_IMMORTAL(ch)) && is_affected(victim,gsn_deafen)) 
			act("$n says something you can't quite hear.",ch,0,victim,TO_VICT);
		else {
			sprintf(buf,"$n %ss '%s%s%s'",saymsg,get_char_color(victim,"speech"),argument,END_COLOR(victim));
			act(buf,ch,extra,victim,TO_VICT);
			if (is_affected(victim,gsn_word_of_command) && strstr(argument,victim->pcdata->command[0]))
				command_execute(victim);

		}
    }

    return;
}

void do_whisper( CHAR_DATA *ch, char *argument )
{
	char buf[MSL];
	CHAR_DATA *victim;

	if (!IS_NPC(ch))

	if ( argument[0] == '\0' )
	{
		send_to_char( "Whisper what?\n\r", ch );
		return;
	}

	if(is_affected(ch,gsn_silence) || IS_SET(ch->in_room->room_flags, ROOM_SILENCE))
	{
		send_to_char("You cannot find the words within you.\n\r",ch);
		return;
	}
	if(IS_SHIFTED(ch) && !IS_IMMORTAL(ch))
		return send_to_char("You can't whisper while shapeshifted.\n\r",ch);

	if (is_affected(ch,gsn_imprisonvoice)) {
	    send_to_char("Your vocal cords are frozen solid!\n\r", ch);
	    return;
	}

	if (is_affected(ch, gsn_gag))
	{
		act("You are gagged and cannot speak!", ch, NULL, NULL, TO_CHAR);
		act("Muffled sounds emanate from $n's direction as $e attempts in vain to speak through $s gag.", ch, NULL, NULL, TO_ROOM);
		return;
	}
	
	check_ooc(ch,argument,"WHISPER");
	
	if (is_affected(ch,gsn_deafen)) {
		do_yell(ch,argument);
		return;
	}

	sprintf(buf,"You whisper '%s%s%s'",get_char_color(ch,"red"),argument,END_COLOR(ch));
		
	act(buf, ch, NULL, argument, TO_CHAR );

	if(IS_SET(ch->in_room->progtypes, RPROG_SPEECH))
		(ch->in_room->rprogs->speech_prog) (ch->in_room, ch, argument);

	for ( victim = ch->in_room->people; victim != NULL; victim = victim->next_in_room )
	{
		if ( IS_AWAKE(victim) )
		{
			if (!(IS_IMMORTAL(ch)) && is_affected(victim,gsn_deafen)) {
				act("$n says something you can't quite hear.",ch,0,victim,TO_VICT);
			} else {
				sprintf(buf,"$n whispers '%s%s%s'",get_char_color(victim,"red"),argument,END_COLOR(victim));
				act(buf,ch,0,victim,TO_VICT);
				if (IS_SET(victim->progtypes,MPROG_SPEECH) && victim != ch)
					(victim->pIndexData->mprogs->speech_prog) (victim,ch,argument);
				if (TRAPS_MEVENT(victim, TRAP_MSPEECH))
					CALL_MEVENT(victim,TRAP_MSPEECH,ch,victim,argument);
				if (is_affected(victim,gsn_word_of_command) && strstr(argument,victim->pcdata->command[0]))
					command_execute(victim);
			}
		}
	}

	if (is_affected(ch,gsn_unholy_communion)
		&& (ch->Class()->name == "anti-paladin"))
		check_unholy_communion(ch,argument);

	return;
}

void do_sing( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim;
	char buf[MSL], buf2[MSL], buf3[MSL];
	char *copy;
	int i, j, lines = 1, length;

	if (argument[0] == '\0')
		return send_to_char("Sing what?\n\r",ch);

	if(is_affected(ch,gsn_silence) 
		|| IS_SET(ch->in_room->room_flags, ROOM_SILENCE))
		return send_to_char("You cannot find the words within you.\n\r",ch);
	
	if(IS_SHIFTED(ch) && !IS_IMMORTAL(ch))
		return send_to_char("You can't sing while shapeshifted.\n\r",ch);

	if (is_affected(ch,gsn_imprisonvoice))
	    return send_to_char("Your vocal cords are frozen solid!\n\r", ch);

	if(is_affected(ch,gsn_uppercut))
		return send_to_char("Your jaw is too painful to move.\n\r",ch);

	if (is_affected(ch, gsn_gag)) {
		act("You are gagged and cannot speak!", ch, NULL, NULL, TO_CHAR);
		act("Muffled sounds emanate from $n's direction as $e attempts in vain to speak through $s gag.", ch, NULL, NULL, TO_ROOM);
		return;
	}
	
	if (is_affected(ch,gsn_deafen))
		return send_to_char("You can't carry a tune when you can't even hear yourself!\n\r",ch);

	check_ooc(ch,argument,"SING");

	copy = argument;
	
	for (i = 0; i < MAX_STRING_LENGTH; i++) {
		buf[i] = '\0';
		buf2[i] = '\0';
	}

	if (!IS_NPC(ch))
		length = strlen(ch->name);
	else
		length = strlen(ch->short_descr);

	for (i = 0; *copy && *copy != '\0'; i++) {
		if (*copy == '/') {
			lines++;
			strcat(buf,"\n\r");
			for (j = 0; j <= length; j++)
				strcat(buf," ");
			strcat(buf,"       ");
			i += (j + 8);
		} else {
			buf[i] = *copy;
		}
		copy++;
	}

	if (lines > 9)
		return send_to_char("You can't sing all that in one breath!\n\r",ch);

	WAIT_STATE(ch,lines * 2);

	buf[i] = '\0';

	copy = argument;

	for (i = 0; *copy && *copy != '\0'; i++) {
		if (*copy == '/') {
			lines++;
			strcat(buf2,"\n\r          ");
			i += 11;
		} else {
			buf2[i] = *copy;
		}
		copy++;
	}

	buf2[i] = '\0';

	for (victim = ch->in_room->people; victim; victim = victim->next_in_room ) {
		if ( IS_AWAKE(victim) ) {
			if (!(IS_IMMORTAL(ch)) && is_affected(victim,gsn_deafen)) {
				act("$n sings something you can't quite hear.",ch,0,victim,TO_VICT);
			} else {
				sprintf(buf3,"$n sings '%s%s%s'",get_char_color(victim,"song"),buf,END_COLOR(victim));
				act(buf3,ch,0,victim,TO_VICT);
				if (is_affected(victim,gsn_word_of_command) && strstr(argument,victim->pcdata->command[0]))
					command_execute(victim);
			}
		}
	}

	sprintf(buf3,"You sing '%s%s%s'",get_char_color(ch,"song"),buf2,END_COLOR(ch));
	act(buf3,ch,0,0,TO_CHAR);

	return;
}

void do_pray( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    FILE *fp;
    char buf[MSL];

    if (argument[0] == '\0' )
    {
        send_to_char("Pray what?\n\r",ch);
      	return;
    }

    if ( IS_SET(ch->comm, COMM_NOCHANNELS) )
    {
        send_to_char( "You can't pray.\n\r", ch );
        return;
    }

    WAIT_STATE( ch, 10 );
    send_to_char("You pray to the heavens!\n\r",ch);
    sprintf(buf,"%s: %s [%d] is PRAYing for: %s\n", log_time(), ch->true_name, ch->in_room->vnum,argument);
    fp = fopen("/home/rift/area/prays.txt","a");
    if(fp)
	{
		fprintf(fp,"%s",buf);
    	fclose(fp);
	}
    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	CHAR_DATA *victim;

	victim = d->original ? d->original : d->character;

	if ( d->connected == CON_PLAYING &&
	     d->character != ch &&
	     !IS_SET(victim->comm, COMM_SHOUTSOFF) &&
	     !IS_SET(victim->comm, COMM_QUIET) &&
	     victim->level >= 52 )
	{
	    sprintf(buf,"%s%s [%d] is PRAYing for: %s%s\n\r",
		get_char_color(victim,"prays"),ch->true_name,ch->in_room->vnum,argument,END_COLOR(victim));
	    send_to_char(buf,d->character);
	}
    }

    return;
}



void do_tell( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH],buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    int count, number;
	bool deaf = FALSE;
    char arg2[MAX_STRING_LENGTH];


    if  (is_affected(ch,gsn_silence))
    {
	send_to_char("You cannot find the words within you.\n\r",ch);
	return;
    }


    if (IS_SET(ch->comm, COMM_NOTELL) || IS_SET(ch->comm,COMM_DEAF))
    {
	send_to_char("Your message didn't get through.\n\r", ch );
	return;
    }

    if ( IS_SET(ch->comm, COMM_QUIET) )
    {
	send_to_char("You must turn off quiet mode first.\n\r", ch);
	return;
    }

    if (IS_SET(ch->comm,COMM_DEAF))
    {
	send_to_char("You must turn off deaf mode first.\n\r",ch);
	return;
    }

	if (is_affected_area(ch->in_room->area,gsn_interference))
		return send_to_char("The electrical currents in the area block your attempt to communicate.\n\r",ch);

	check_ooc(ch,argument,"TELL");
	
    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "Tell whom what?\n\r", ch );
	return;
    }

/* This added for decoy tells..without this you have to locate the number
of the person you are trying to talk to when their decoys occupy 1.<name>,
and that's mega annoying...(Ceran)
*/
    arg2[0] = '\0';
    sprintf(arg2,"%s",arg);

    /*
     * Can tell to PC's anywhere, but NPC's only in same room.
     * -- Furey
     */

   number = number_argument(arg2, arg );
	count = 0;

    for (victim = char_list; victim != NULL; victim = victim->next)
    {
	if (is_name(victim->name,arg)
	&& (IS_NPC(victim) && victim->pIndexData->vnum == MOB_VNUM_DECOY))
		continue;
	if (victim->in_room == NULL || (IS_IMMORTAL(ch) ? !is_name(arg,(victim->true_name ? victim->true_name : victim->name)):
			!is_name(arg,victim->name)) || !can_see( ch,victim ))
		continue;
        if ( ++count == number )
		break;
    }

    if (victim == NULL || ( IS_NPC(victim) && victim->in_room != ch->in_room ) )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
    if (IS_SET(ch->in_room->room_flags, ROOM_SILENCE) || IS_SET(victim->in_room->room_flags,ROOM_SILENCE))
	return send_to_char("Your message didn't get through.\n\r",ch);
    if ( victim->desc == NULL && !IS_NPC(victim))
    {
	act("$N seems to have lost consciousness...try again later.",
	    ch,NULL,victim,TO_CHAR);
        sprintf(buf,"%s tells you '%s'\n\r",PERS(ch,victim),argument);
        buf[0] = UPPER(buf[0]);
        add_buf(victim->pcdata->buffer,buf);
	return;
    }
    if ( !(IS_IMMORTAL(ch) && ch->level > LEVEL_IMMORTAL) && (!IS_AWAKE(victim)  ||
	is_affected(victim,gsn_deafen)))
	return act( "$E can't hear you.", ch, 0, victim, TO_CHAR );

    if ((IS_SET(victim->comm,COMM_QUIET) || IS_SET(victim->comm,COMM_DEAF))
	&& !IS_IMMORTAL(ch) ) /* Let Imms send tells to deaf players */
	return act( "$E is not receiving tells.", ch, 0, victim, TO_CHAR );

	if (is_affected(ch,gsn_deafen)) {
		deaf = TRUE;
		argument = palloc_string(upstring(argument));
	}


	sprintf(buf,"You tell $%s '%s$t%s'",IS_IMMORTAL(ch) && !IS_NPC(victim) ? "F" : "N", get_char_color(ch,"tells"),END_COLOR(ch));
	act(buf, ch, argument,victim,TO_CHAR);

	sprintf(buf,"$%s tells you '%s$t%s'",IS_IMMORTAL(victim) && !IS_NPC(ch) ? "f" :"n",get_char_color(victim,"tells"),END_COLOR(victim));
	act_new(buf,ch,argument,victim,TO_VICT,POS_DEAD);

	if (IS_SET(victim->progtypes,MPROG_SPEECH) && victim != ch)
		(victim->pIndexData->mprogs->speech_prog) (victim,ch,argument);

	if (is_affected(victim,gsn_word_of_command) && strstr(argument,victim->pcdata->command[0]))
		command_execute(victim);

    victim->reply	= ch;

	if (deaf)
		free_pstring(argument);
	
    return;
}


void do_noreply(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *vch;
	send_to_char("You concentrate and momentarily close your ears to the replies of others.\n\r",ch);
	for(vch = char_list; vch; vch = vch->next)
		if(!IS_NPC(vch) && vch->reply == ch)
			vch->reply = NULL;
}

void do_reply( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];
	bool deaf = FALSE;

    if ( IS_SET(ch->comm, COMM_NOTELL) )
    {
	send_to_char( "Your message didn't get through.\n\r", ch );
	return;
    }

    if ( ( victim = ch->reply ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim->desc == NULL && !IS_NPC(victim) && !IS_SWITCHED(victim) && victim->pcdata)
    {
        act("$N seems to have lost consciousness...try again later.",
            ch,NULL,victim,TO_CHAR);
        sprintf(buf,"%s tells you '%s'\n\r",PERS(ch,victim),argument);
        buf[0] = UPPER(buf[0]);
        add_buf(victim->pcdata->buffer,buf);
        return;
    }

    if ( !IS_IMMORTAL(ch) && (!IS_AWAKE(victim) || is_affected(victim,gsn_deafen)))
    {
	act( "$E can't hear you.", ch, 0, victim, TO_CHAR );
	return;
    }

    if ((IS_SET(victim->comm,COMM_QUIET) || IS_SET(victim->comm,COMM_DEAF))
    &&  !IS_IMMORTAL(ch) && !IS_IMMORTAL(victim))
    {
        act_new( "$E is not receiving tells.", ch, 0, victim, TO_CHAR,POS_DEAD);
        return;
    }
    if (!IS_IMMORTAL(victim) && !IS_AWAKE(ch))
    {
	send_to_char( "In your dreams, or what?\n\r", ch );
	return;
    }
	
	if (is_affected(ch,gsn_imprisonvoice)) {
	    send_to_char("Your vocal cords are frozen solid!\n\r", ch);
	    return;
	}

    if (is_affected(ch, gsn_gag))
	    return send_to_char("You are gagged and cannot speak!\n\r", ch);

	if (!IS_IMMORTAL(victim))
		check_ooc(ch,argument,"TELL");

	if (is_affected(ch,gsn_deafen)) {
		deaf = TRUE;
		argument = palloc_string(upstring(argument));
	}

	sprintf(buf,"You tell $%s '%s$t%s'",IS_IMMORTAL(ch) && !IS_NPC(victim) ? "F" : "N", get_char_color(ch,"tells"),END_COLOR(ch));
	act(buf, ch, argument,victim,TO_CHAR);

	sprintf(buf,"$%s tells you '%s$t%s'",IS_IMMORTAL(victim) && !IS_NPC(ch) ? "f" :"n",get_char_color(victim,"tells"),END_COLOR(victim));
	act_new(buf,ch,argument,victim,TO_VICT,POS_DEAD);

	if (IS_SET(victim->progtypes,MPROG_SPEECH) && victim != ch)
		(victim->pIndexData->mprogs->speech_prog) (victim,ch,argument);
	
	victim->reply	= ch;

	if (deaf)
		free_pstring(argument);

    return;
}

void do_yell( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    char buf[MAX_STRING_LENGTH];
	bool deaf = FALSE;

    if(is_affected(ch,gsn_silence))
		return send_to_char("You cannot find the words within you.\n\r",ch);

    if ( IS_SET(ch->comm, COMM_NOSHOUT) || (IS_SHIFTED(ch) && !IS_IMMORTAL(ch)))
    {
        send_to_char( "You can't yell.\n\r", ch );
        return;
    }
    if(is_affected(ch,gsn_mangled))
        return send_to_char("Your torn throat prevents you from getting the words out.\n\r",ch);

    if ( argument[0] == '\0' )
    {
	send_to_char( "Yell what?\n\r", ch );
	return;
    }
	if (is_affected(ch,gsn_imprisonvoice)) {
	    send_to_char("Your vocal cords are frozen solid!\n\r", ch);
	    return;
	}

	if (is_affected(ch, gsn_gag))
	{
		act("You are gagged and cannot speak!", ch, NULL, NULL, TO_CHAR);
		act("Muffled sounds emanate from $n's direction as $e attempts in vain to speak through $s gag.", ch, NULL, NULL, TO_ROOM);
		return;
	}

	if (is_affected(ch,gsn_uppercut)) {
		return send_to_char("Your jaw hurts too much to move.\n\r",ch);
	}
		
	if (is_affected(ch,gsn_deafen)) {
		deaf = TRUE;
		argument = palloc_string(upstring(argument));
	}

	check_ooc(ch,argument,"YELL");


	if (!str_cmp(ch->name,"Karsus") && strstr(argument,"eing")
		&& !strstr(argument,"being")) {
		send_to_char("***It's 'ING', not 'EING' you fooker!!!***\n\r",ch);
		send_to_char("***It's 'ING', not 'EING' you fooker!!!***\n\r",ch);
		send_to_char("***It's 'ING', not 'EING' you fooker!!!***\n\r",ch);
	}

	
    if (IS_SET(ch->comm,COMM_ANSI))
    {
	sprintf(buf,"You yell '%s$t%s'",get_char_color(ch,"yells"),END_COLOR(ch));
	act(buf,ch,argument,NULL,TO_CHAR);
    }
    else
	act("You yell '$t'",ch,argument,NULL,TO_CHAR);

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->connected == CON_PLAYING
	&&   d->character != ch
	&&   d->character->in_room != NULL
	&&   d->character->in_room->area == ch->in_room->area
        &&   !IS_SET(d->character->comm,COMM_QUIET) )
	{
		if (IS_SET(d->character->in_room->room_flags, ROOM_SILENCE))
			continue;
	
		/* Can't hear yells while asleep, MORGLUM */
		if(!IS_AWAKE(d->character))
			continue;

        sprintf(buf,"$n yells '%s$t%s'",get_char_color(d->character,"yells"),END_COLOR(d->character));
	    act_new(buf,ch,argument,d->character,TO_VICT, POS_SLEEPING);
		if (is_affected(d->character,gsn_word_of_command) && strstr(argument,d->character->pcdata->command[0]))
			command_execute(d->character);
	}
    }

	if (deaf)
		free_pstring(argument);
	
    return;
}


void do_myell( CHAR_DATA *ch, char *argument, CHAR_DATA *attacker )
{
/*
	if(IS_SHIFTED(ch))
    {
        for (d = descriptor_list; d; d = d->next)
        {
                if (d->connected == CON_PLAYING
                &&  d->character->in_room != NULL && ch->in_room != NULL
                &&  d->character->in_room->area == ch->in_room->area
				&&  d->character != ch
				&&  !IS_SET(d->character->in_room->room_flags, ROOM_SILENCE))
                {
                        send_to_char(form_table[ch->pcdata->shifted].yell,d->character);
                        send_to_char("\n\r",d->character);
                }
        }
		send_to_char("You make some noise as you are attacked.\n\r",ch);
		return;
    }
*/
	if (ch->invis_level >= LEVEL_HERO)
		return;

	if(ch && ch->in_room && attacker && can_see(ch,attacker))
		if(IS_SET(ch->in_room->area->progtypes, APROG_MYELL))
			((ch->in_room->area->aprogs->myell_prog) (ch->in_room->area, attacker, ch));
	
    do_yell(ch,argument);
    return;
}


void do_emote( CHAR_DATA *ch, char *argument )
{
	char buffer[MAX_STRING_LENGTH*2];

	if ( !IS_NPC(ch) && IS_SET(ch->comm, COMM_NOEMOTE) )
        return send_to_char( "You can't show your emotions.\n\r", ch );
    if ( argument[0] == '\0' )
        return send_to_char( "Emote what?\n\r", ch );

	check_ooc(ch,argument,"EMOT");
	
	buffer[0] = '\0';
	if(argument[0] != '\'' || UPPER(argument[1]) != 'S')
		strcpy(buffer, " ");
	strcat(buffer, argument);

	    act( "$n$T", ch, NULL, buffer, TO_ALL);

}


void do_pmote( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    char *letter,*name;
    char last[MAX_INPUT_LENGTH], temp[MAX_STRING_LENGTH], tbuf[MSL];
    unsigned int matches = 0;

    if ( !IS_NPC(ch) && IS_SET(ch->comm, COMM_NOEMOTE) )
        return send_to_char( "You can't show your emotions.\n\r", ch );

    if ( argument[0] == '\0' )
        return send_to_char( "Emote what?\n\r", ch );

	check_ooc(ch,argument,"PMOT");

	sprintf(temp, "%s", argument);
    tbuf[0] = '\0';
    if(temp[0] != '\'' && UPPER(temp[1]) != 'S')
		strcpy(tbuf, " ");
    strcat(tbuf,temp);
    argument = palloc_string(tbuf);
    act( "$n$t", ch, argument, NULL, TO_CHAR );

    for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
    {
	if (vch->desc == NULL || vch == ch)
	    continue;

	if ((letter = strstr(argument,vch->name)) == NULL)
	{
	    act("$N$t",vch,argument,ch,TO_CHAR);
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

	act("$N$t",vch,temp,ch,TO_CHAR);
    }

	free_pstring(argument);
	
    return;
}

/* Simple, yet brilliant.  Notify immortals when players are using words that
 * are offensive/harrassing or commonly associated with OOC speech.  Immortals
 * can then respond accordingly if a rule violation is detected.
 * 											-El
 */

bool check_ooc (CHAR_DATA *ch, char *argument, char *type)
{
	char buf[MSL];

	/* Imms can curse and be OOC all we want! */
	if (IS_IMMORTAL(ch))	
		return TRUE;

	if (IS_NPC(ch) && !IS_AFFECTED(ch,AFF_CHARM))
		return TRUE;
	
	/* The list of naughty words.  Add more as needed. */
	
	if (strstr(argument,"pk")			||
		strstr(argument,"PK")			||
		strstr(argument,"Pk") 			||
		strstr(argument,"ooc")			||
		strstr(argument,"OOC") 			||
		strstr(argument,"Ooc") 			||
		strstr(argument,"exp ") 		||
		strstr(argument,"exp.")			||
		strstr(argument," xp") 			||
		strstr(argument," XP") 			||
		strstr(argument,"tnl")			||
		strstr(argument,"hp")			||
		strstr(argument,"HP")			||
		strstr(argument," u ") 			||
		strstr(argument," U ") 			||
		strstr(argument,"0r") 			||
		strstr(argument,"x0") 			||
		strstr(argument," 2 ") 			||
		strstr(argument,"grp") 			||
		strstr(argument,"afk")			||
		strstr(argument,"AFK")			||
		strstr(argument,"computer")		||
		strstr(argument,"quit out") 	||
		strstr(argument,"drop link")	||
		strstr(argument,"lost link")	||
		strstr(argument,"lose link")	||
		strstr(argument,"discon")		||
		strstr(argument,"relog") 		||
		strstr(argument,"logoff") 		||
		strstr(argument,"log off") 		||
		strstr(argument," lag")			||
		strstr(argument," Lag")			||
		strstr(argument,"character") 	||
		strstr(argument,"chars")	 	||
		strstr(argument,"player") 		||
		strstr(argument,"game") 		||
		strstr(argument,"MUD") 			||
		strstr(argument,"mud") 			||
		strstr(argument,"RL")			||
		strstr(argument,"RS") 			||
		strstr(argument,"Riftshadow") 	||
		strstr(argument,"play") 		||
		strstr(argument,"CF") 			||
		strstr(argument,"cf") 			||
		strstr(argument,"Carrion") 		||
		strstr(argument,"fuck") 		||
		strstr(argument,"FUCK") 		||
		strstr(argument,"Fuck") 		||
		strstr(argument,"fuk") 			||
		strstr(argument,"pussy") 		||
		strstr(argument,"cunt") 		||
		strstr(argument,"licker") 		||
		strstr(argument,"dick") 		||
		strstr(argument,"fag") 			||
		strstr(argument,"FAG") 			||
		strstr(argument,"nigg") 		||
		strstr(argument,"gook")			||
		strstr(argument,"Gook")			||
		strstr(argument,"jew") 			||
		strstr(argument,"Jew") 			||
		strstr(argument,"kike") 		||
		strstr(argument," spic") 		||
		strstr(argument,"newbie"))
	{
		sprintf(buf,"%s is %sING: ",ch->name,type);
		strcat(buf,argument);
		wiznet(buf,NULL,NULL,WIZ_OOC,0,0);
	}

	return FALSE;
}

void do_bug( CHAR_DATA *ch, char *argument )
{
    if(IS_IMMORTAL(ch))
	return do_buglist(ch,argument);
    append_file( ch, BUG_FILE, argument );
    send_to_char( "Bug logged.\n\r", ch );
    return;
}

void do_typo( CHAR_DATA *ch, char *argument )
{
    append_file( ch, TYPO_FILE, argument );
    send_to_char( "Typo logged.\n\r", ch );
    return;
}

void do_idea( CHAR_DATA *ch, char *argument )
{
	append_file( ch, IDEAS_FILE, argument );
	send_to_char( "Idea logged.  Thanks!\n\r", ch);
	return;
}

void do_rent( CHAR_DATA *ch, char *argument )
{
    send_to_char( "There is no rent here.  Just save and quit.\n\r", ch );
    return;
}


void do_qui( CHAR_DATA *ch, char *argument )
{
    send_to_char( "If you want to QUIT, you have to spell it out.\n\r", ch );
    return;
}


void do_quit(CHAR_DATA *ch, char *argument)
{
	do_quit_new(ch,argument,FALSE);
	return;
}

void do_quit_new( CHAR_DATA *ch, char *argument, bool autoq)
{
    DESCRIPTOR_DATA *d,*d_next;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    int id;
    CHAR_DATA *wch;
    CHAR_DATA *wch_next;

    if ( IS_NPC(ch) )
		return;

    if (!autoq && IS_AFFECTED(ch,AFF_CHARM))
		return send_to_char("Now why would you want to leave your master?\n\r",ch);

    if (!autoq && ch->position == POS_FIGHTING)
		return send_to_char( "No way! You are fighting.\n\r", ch );

    if (!autoq && ch->position  < POS_STUNNED)
		return send_to_char( "You're not DEAD yet.\n\r", ch );

    if (!autoq && ch->pause > 0 && !IS_IMMORTAL(ch))
		return send_to_char( "Your adrenaline is gushing too much to quit!\n\r", ch );

	if (!autoq && (is_affected(ch,gsn_blackjack) || IS_AFFECTED(ch,AFF_SLEEP)))
		return send_to_char("You can't quit!\n\r",ch);

	if (is_affected(ch,skill_lookup("diamond skin")))
		affect_strip(ch,skill_lookup("diamond skin"));
	
	if (is_affected(ch,skill_lookup("channel")))
		affect_strip(ch,skill_lookup("channel"));
	
	if (is_affected(ch,skill_lookup("fortitude")))
		affect_strip(ch,skill_lookup("fortitude"));
	
	if (is_affected(ch,gsn_cloak_form))
		do_cloak(ch,"auto");

	if (!autoq && IS_AFFECTED(ch,AFF_CHARM))
	   return send_to_char("You don't want to leave your master yet!\n\r",ch);

	if(!autoq && !IS_SET(ch->in_room->room_flags,ROOM_LAW)
		&& ch->in_room->cabal==0 && !IS_IMMORTAL(ch))
		return send_to_char("You don't feel safe enough to quit here.\n\r",ch);

	if (!autoq && ch->in_room->cabal != 0 && !IS_IMMORTAL(ch))
        if (ch->in_room->cabal != ch->cabal)
        	return send_to_char("You can't quit out in an enemy Cabal!\n\r",ch);

	if (autoq == FALSE)
		for (obj = object_list; obj != NULL; obj = obj_next) {
			obj_next = obj->next;
			if (isCabalItem(obj))
				if (obj->carried_by != NULL && obj->carried_by == ch)
					return act("You cannot quit with cabal items in your inventory!",ch,0,0,TO_CHAR);
		}
	if (CQueue::HasQueuePending(ch))
	{
		if(autoq)
			bug("Trying to autoquit char %s with pending queue.", ch->name);
		return send_to_char("You cannot quit while events requiring your presence are pending.\n\r",ch);
	}

    if(is_affected(ch,gsn_disguise))
		do_undisguise(ch,"");

	if(is_affected(ch, gsn_empathy))
		affect_strip(ch, gsn_empathy);
	
    if (ch->cabal != 0 && ch->pcdata->host)
	    announce_logout(ch);

    send_to_char("Alas, all good things must come to an end.\n\r",ch);
    act( "$n has left the realm.", ch, NULL, NULL, TO_ROOM );

    sprintf( log_buf, "%s@%s has %squit. [%ld played, %d (%d) obj]",
		ch->true_name, !IS_SET(ch->comm, COMM_NOSOCKET) ? ch->pcdata->host : "",
		autoq ? "auto" : "", ((current_time - ch->logon) / 60),
		count_carried(ch,FALSE), count_carried(ch,TRUE));

    log_string(log_buf);
    wiznet(log_buf,ch,NULL,WIZ_SITES,0,get_trust(ch));
    login_log(ch,2);
    /*
     * After extract_char the ch is no longer valid!
     */

/* Remove any Cabal items first , also owners
    and any limiteds on levels 10 and below
*/
    for (obj = object_list; obj != NULL; obj = obj_next) {
		obj_next = obj->next;
   		if (isCabalItem(obj)) {
			if (obj->carried_by != NULL && obj->carried_by == ch)
				extract_obj(obj);
		} else if (obj->pIndexData->limtotal != 0 && ch->level < 10)
			if (obj->carried_by == ch)
				extract_obj(obj);
    }

/* extract all decoys */ 
   for (wch = char_list; wch != NULL; wch = wch_next)
    {
		AFFECT_DATA *laf;
		wch_next = wch->next;

		if (wch->defending!= NULL && wch->defending == ch)
			wch->defending  =  NULL;

		if (is_affected(wch, gsn_empathy))
		{
			for(laf = wch->affected; laf != NULL; laf = laf->next)
				if(laf->type == gsn_empathy)
					break;

			if(laf->owner == ch)
				affect_strip(wch, gsn_empathy);
		}
		
		if (!IS_NPC(wch))
			continue;

		if (IS_NPC(wch) && IS_AFFECTED(wch,AFF_CHARM) && wch->master==ch && IS_SET(wch->act,ACT_UNDEAD))
		{
			extract_char(wch,TRUE);
			continue;
		}
	
		if (wch->pIndexData->vnum == MOB_VNUM_DECOY && is_name(ch->name,wch->name))
		{
			act("$n crumbles to dust.",wch,0,0,TO_ROOM);
			extract_char(wch,TRUE);
		}
    }

    save_char_obj( ch );
    id = ch->id;
    d = ch->desc;


	extract_char( ch, TRUE );

	
	if ( d != NULL )
		close_socket( d );

    /* toast evil cheating bastards */
    for (d = descriptor_list; d != NULL; d = d_next)
    {
	CHAR_DATA *tch;

	d_next = d->next;
	tch = d->original ? d->original : d->character;
	if (tch && tch->id == id)
	{
	    extract_char(tch,TRUE);
	    close_socket(d);
	}
    }

    return;
}



void do_save( CHAR_DATA *ch, char *argument )
{
    send_to_char("Saving. Remember that Riftshadow has automatic saving.\n\r", ch);
    if ( IS_NPC(ch))
        return;
    save_char_obj( ch );
    ch->pcdata->save_timer = 2;
    return;
}



void do_follow( CHAR_DATA *ch, char *argument )
{
/* RT changed to allow unlimited following and follow the NOFOLLOW rules */
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Follow whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master != NULL )
    {
	act( "But you'd rather follow $N!", ch, NULL, ch->master, TO_CHAR );
	return;
    }

    if ( victim == ch )
    {
	if ( ch->master == NULL )
	{
	    send_to_char( "You already follow yourself.\n\r", ch );
	    return;
	}
	stop_follower(ch);
	return;
    }

    if (!IS_NPC(victim) && IS_SET(victim->act,PLR_NOFOLLOW) && /*!IS_IMMORTAL(ch) && */
		(get_skill(ch, gsn_trail)==0 || can_see(victim, ch)))
    {
		if((get_skill(ch, gsn_trail)>0 && can_see(victim, ch)))
			act("You can't very well trail someone if they can see you.", ch, NULL, NULL, TO_CHAR);
		act("$N doesn't seem to want any followers.\n\r",ch,NULL,victim, TO_CHAR);
        return;
    }

	if (is_affected(victim,gsn_sphere_of_plasma))
		return send_to_char("They are encased in a sphere of plasma!\n\r",ch);
	if (is_affected(ch,gsn_sphere_of_plasma))
		return send_to_char("You are encased in a sphere of plasma!\n\r",ch);

    REMOVE_BIT(ch->act,PLR_NOFOLLOW);

    if ( ch->master != NULL )
	stop_follower( ch );

    add_follower( ch, victim );
    return;
}


void add_follower( CHAR_DATA *ch, CHAR_DATA *master )
{
	int chance;
	AFFECT_DATA af;

    if ( ch->master != NULL )
    {
	bug( "Add_follower: non-null master.", 0 );
	return;
    }

    ch->master        = master;
    ch->leader        = NULL;

	if((chance = get_skill(ch, gsn_trail))>0 && IS_SET(master->act,PLR_NOFOLLOW))
	{
		if(number_percent() < chance)
		{
    		act( "You attempt to secretly trail $N.",  ch, NULL, master, TO_CHAR );
			init_affect(&af);
			af.where = TO_AFFECTS;
			af.type = gsn_trail;
			af.level = ch->level;
			af.aftype    = AFT_INVIS;
			af.duration  = -1;
			affect_to_char(ch,&af);
			check_improve(ch, gsn_trail, TRUE, 1);
		}
		else
		{
			act("You attempt to secretly trail $N.", ch, NULL, master, TO_CHAR);
			act("You have a sudden feeling someone is following you.", ch, NULL, master, TO_VICT);
			check_improve(ch, gsn_trail, FALSE, 1);
		}
	}
	else
	{
		if ( can_see( master, ch ) )
			act( "$n now follows you.", ch, NULL, master, TO_VICT );
		act( "You now follow $N.",  ch, NULL, master, TO_CHAR );
	}
	return;
}



void stop_follower( CHAR_DATA *ch )
{
    if ( ch->master == NULL )
    {
	bug( "Stop_follower: null master.", 0 );
	return;
    }

    if ( IS_AFFECTED(ch, AFF_CHARM) )
    {
		REMOVE_BIT( ch->affected_by, AFF_CHARM );
		affect_strip( ch, gsn_charm_person );
    }

    if ( can_see( ch->master, ch ) && ch->in_room != NULL && !(IS_NPC(ch) && ch->pIndexData->vnum == MOB_VNUM_ANCHOR))
    {
		act( "$n stops following you.",     ch, NULL, ch->master, TO_VICT    );
    	act( "You stop following $N.",      ch, NULL, ch->master, TO_CHAR    );
		if(is_affected(ch, gsn_trail))
			affect_strip( ch, gsn_trail);
	}
    if (ch->master->pet == ch)
	ch->master->pet = NULL;

	check_leadership_affect(ch);
    ch->master = NULL;
    ch->leader = NULL;
    return;
}

/* nukes charmed monsters and pets */
void nuke_pets( CHAR_DATA *ch )
{
    CHAR_DATA *pet;

    if ((pet = ch->pet) != NULL)
    {
    	stop_follower(pet);
    	if (pet->in_room != NULL)
    	    act("$N slowly fades away.",ch,NULL,pet,TO_NOTVICT);
    	extract_char(pet,TRUE);
    }
    ch->pet = NULL;

    return;
}



void die_follower( CHAR_DATA *ch )
{
    CHAR_DATA *fch;
    CHAR_DATA *fch_next;
    if(!ch)
	return bug("Error: Die follower, invalid master.",0);
    if ( ch->master != NULL )
    {
    	if (ch->master->pet == ch)
    	    ch->master->pet = NULL;
		stop_follower( ch );
    }

    ch->leader = NULL;

    for ( fch = char_list; fch != NULL; fch = fch_next )
    {
		fch_next = fch->next;
		/*if(!fch->in_room && IS_NPC(fch))
		{
			bug("Error: Mob %d in room is NULL!",fch->pIndexData->vnum);
			break;
		}*/
		if ( (IS_NPC(fch)) && ( (is_affected(fch,gsn_animate_dead)) || (IS_AFFECTED(fch,AFF_CHARM) ) ) )
 		{
			if  (fch->master == ch)
			{
				REMOVE_BIT(fch->affected_by,AFF_CHARM);
				affect_strip(fch,gsn_animate_dead);
				act("$n slowly fades away.",fch,NULL,NULL,TO_ROOM);
				extract_char(fch,TRUE);
			}
		}
		else
		{
			if ( fch->master == ch && !is_affected(fch, gsn_trail) )
	    		stop_follower( fch );
			if ( fch->leader == ch && !is_affected(fch, gsn_trail) )
	    		fch->leader = fch;
		}
    }

    return;
}



void do_order( CHAR_DATA *ch, char *argument )
{
	char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH],arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *och;
    CHAR_DATA *och_next;
    bool found, fAll, cOK;
	int i;

    argument = one_argument( argument, arg );
    one_argument(argument,arg2);

    if ( arg[0] == '\0' || argument[0] == '\0' )
		return send_to_char( "Order whom to do what?\n\r", ch );

    if ( IS_AFFECTED( ch, AFF_CHARM ) )
		return send_to_char( "You feel like taking, not giving, orders.\n\r", ch );
    if ( !str_cmp( arg, "all" ) )
    {
		fAll   = TRUE;
		victim = NULL;
    }
    else
    {
		fAll   = FALSE;

		if ( ( victim = get_char_room( ch, arg ) ) == NULL )
	    	return send_to_char( "They aren't here.\n\r", ch );

		if ( victim == ch )
			return send_to_char( "Aye aye, right away!\n\r", ch );

		if (!IS_AFFECTED(victim, AFF_CHARM) || victim->master != ch
		||  (IS_IMMORTAL(victim) && victim->trust >= ch->trust))
	    	return send_to_char( "Do it yourself!\n\r", ch );
    }
    
	cOK = FALSE;
	for(i=0; i < MAX_ORDERS; i++)
		if(!str_prefix(arg2,order_table[i].command))
			cOK = TRUE;

	if(!cOK)
		return send_to_char("You can't order them to do that!\n\r", ch);

	found = FALSE;

    for ( och = ch->in_room->people; och != NULL; och = och_next )
    {
		och_next = och->next_in_room;

		if ( IS_AFFECTED(och, AFF_CHARM)
		&&   och->master == ch
		&& ( fAll || och == victim ) )
		{
		if(IS_NPC(och) && och->pIndexData->vnum == ACADEMY_PET)
			continue;
	    	found = TRUE;
	    	sprintf( buf, "$n orders you to '%s'.", argument );
	    	act( buf, ch, NULL, och, TO_VICT );
	    	interpret( och, argument );
		if(!str_prefix(arg2,"disengage"))
			break;
		}
    }

    if ( found )
    {
	if(str_prefix(arg2,"say") && str_prefix(arg2,"emote") && str_prefix(arg2,"pmote"))
		WAIT_STATE(ch,2*PULSE_VIOLENCE);
	send_to_char( "Ok.\n\r", ch );
    }
    else
	send_to_char( "You have no followers here.\n\r", ch );
    return;
}



void do_group( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH], buf2[MSL];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim, *vch;
	AFFECT_DATA af;
	int exp_tnl, exp_ltl, epl;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	CHAR_DATA *gch;
	CHAR_DATA *leader;

	leader = (ch->leader != NULL) ? ch->leader : ch;
	sprintf( buf, "%s's group:\n\r", PERS(leader, ch) );
	send_to_char( buf, ch );

	for ( gch = char_list; gch != NULL; gch = gch->next )
	{
	    if ( is_same_group( gch, ch ) )
	    {
			if (!IS_NPC(gch)) {
				exp_tnl = (gch->level * exp_per_level(gch) - gch->exp);
				epl = pc_race_table[gch->race].xpadd + 1500;
				epl += (int)((gch->level - 2) * epl * 0.08);
				exp_ltl = (gch->level * exp_per_level(gch) - (gch->level - 1) * epl);
                sprintf( buf,
                "[%2d %s] %-32s %3d%%hp %3d%%mana %3d%%mv %3d%%tnl\n\r",
                    gch->level,
                    IS_NPC(gch) ? "Mob" : gch->Class()->who_name,
                    capitalize( PERS(gch, ch) ),
	(gch->max_hit == 0) ? 0 : (gch->hit*100)/gch->max_hit,
	(gch->max_mana == 0) ? 0 :
                    (gch->mana*100)/gch->max_mana,
	(gch->max_move == 0) ? 0 :
                    (gch->move*100)/gch->max_move,
					URANGE(0,(exp_tnl * 100)/exp_ltl,100));
			} else {
				sprintf (buf2, PERS(gch,ch));
				if (buf2[0] != '\0')
					buf2[0] = UPPER(buf2[0]);
				sprintf (buf,
                "[Minion] %-32s %3d%%hp %3d%%mana %3d%%mv\n\r",
					buf2,
	(gch->max_hit == 0) ? 0 : (gch->hit*100)/gch->max_hit,
	(gch->max_mana == 0) ? 0 : (gch->mana*100)/gch->max_mana,
	(gch->max_move == 0) ? 0 : (gch->move*100)/gch->max_move);
			}
			send_to_char( buf, ch );
	    }
	}
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
		return send_to_char( "They aren't here.\n\r", ch );

    if ( ch->master != NULL || ( ch->leader != NULL && ch->leader != ch ) )
		return send_to_char( "But you are following someone else!\n\r", ch );

    if ( victim->master != ch && ch != victim )
		return act_new("$N isn't following you.",ch,NULL,victim,TO_CHAR,POS_SLEEPING);

    if (IS_AFFECTED(victim,AFF_CHARM))
        return send_to_char("You can't remove charmed followers from your group.\n\r",ch);

    if (IS_AFFECTED(ch,AFF_CHARM))
    	return act_new("You like your master too much to leave $m!",ch,NULL,victim,TO_VICT,POS_SLEEPING);

    if ( is_same_group( victim, ch ) && ch != victim )
    {
	victim->leader = NULL;
	act_new("$n removes $N from $s group.",
	    ch,NULL,victim,TO_NOTVICT,POS_RESTING);
	act_new("$n removes you from $s group.",
	    ch,NULL,victim,TO_VICT,POS_SLEEPING);
	act_new("You remove $N from your group.",
	    ch,NULL,victim,TO_CHAR,POS_SLEEPING);
	check_leadership_affect(victim);
	return;
    }

    victim->leader = ch;
    act_new("$N joins $n's group.",ch,NULL,victim,TO_NOTVICT,POS_RESTING);
    act_new("You join $n's group.",ch,NULL,victim,TO_VICT,POS_SLEEPING);
    act_new("$N joins your group.",ch,NULL,victim,TO_CHAR,POS_SLEEPING);

	if (!IS_NPC(ch) && !IS_NPC(victim)
		&& ch->Class()->name == "anti-paladin"
		&& ch->pcdata->learned[gsn_traitors_luck] > 1) {
		init_affect(&af);
		af.where		=	TO_AFFECTS;
		af.aftype		=	AFT_INVIS;
		af.type			=	gsn_traitors_luck;
		af.owner		=	ch;
		af.level		=	ch->level;
		af.duration		=	-1;
		SET_BIT(af.bitvector, AFF_PERMANENT);
		af.modifier		=	0;
		af.location		=	0;
		af.pulse_fun	=	traitor_pulse;
		new_affect_to_char(ch,&af);
	}

	if (!IS_NPC(ch) && !IS_NPC(victim)
		&& victim->Class()->name == "anti-paladin"
		&& victim->pcdata->learned[gsn_traitors_luck] > 1) {
		init_affect(&af);
		af.where		=	TO_AFFECTS;
		af.aftype		=	AFT_INVIS;
		af.type			=	gsn_traitors_luck;
		af.owner		=	victim;
		af.level		=	ch->level;
		af.duration		=	-1;
		SET_BIT(af.bitvector, AFF_PERMANENT);
		af.modifier		=	0;
		af.location		=	0;
		af.pulse_fun	=	traitor_pulse;

		for (vch = char_list; vch; vch = vch->next)
			if (vch != ch && is_same_group(vch,ch))
				new_affect_to_char(vch,&af);
	}
	
			
    return;
}



/*
 * 'Split' originally by Gnort, God of Chaos.
 */
void do_split( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    CHAR_DATA *gch;
    int members;
    int amount_gold = 0;
    int share_gold;
    int extra_gold;

    argument = one_argument( argument, arg1 );

    if ( arg1[0] == '\0' )
    {
	send_to_char( "Split how much?\n\r", ch );
	return;
    }

	amount_gold = atoi(arg1);

    if ( amount_gold < 0)
    {
	send_to_char( "Your group wouldn't like that.\n\r", ch );
	return;
    }

    if ( amount_gold == 0)
    {
	send_to_char( "You hand out zero coins, but no one notices.\n\r", ch );
	return;
    }

    if ( ch->gold <  amount_gold)
    {
	send_to_char( "You don't have that much to split.\n\r", ch );
	return;
    }

    members = 0;
    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
	if ( is_same_group( gch, ch ) && !IS_AFFECTED(gch,AFF_CHARM))
	    members++;
    }

    if ( members < 2 )
    {
	send_to_char( "Just keep it all.\n\r", ch );
	return;
    }
	
    share_gold   = amount_gold / members;
    extra_gold   = amount_gold % members;

    if ( share_gold == 0 )
    {
	send_to_char( "Don't even bother, cheapskate.\n\r", ch );
	return;
    }

    ch->gold 	-= amount_gold;
    ch->gold 	+= share_gold + extra_gold;

    if (share_gold > 0)
    {
	sprintf(buf,
	    "You split %d gold coins. Your share is %d gold.\n\r",
	     amount_gold,share_gold + extra_gold);
	send_to_char(buf,ch);
    }

	sprintf(buf,"$n splits %d gold coins. Your share is %d gold.",
		amount_gold,share_gold);

    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
	if ( gch != ch && is_same_group(gch,ch) && !IS_AFFECTED(gch,AFF_CHARM))
	{
	    act( buf, ch, NULL, gch, TO_VICT );
	    gch->gold += share_gold;
	}
    }

    return;
}



void do_gtell( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *gch;
    char buf[MAX_STRING_LENGTH];
	bool deaf = FALSE;

    if(is_affected(ch,gsn_silence))
    {
	send_to_char("You cannot find the words within you.\n\r",ch);
	return;
    }

	if (is_affected_area(ch->in_room->area,gsn_interference))	
		return send_to_char("The electrical currents in the area block your attempt to communicate.\n\r",ch);

    if ( argument[0] == '\0' )
    {
	send_to_char( "Tell your group what?\n\r", ch );
	return;
    }

    if ( IS_SET( ch->comm, COMM_NOTELL ) )
    {
	send_to_char( "Your message didn't get through!\n\r", ch );
	return;
    }
	if (is_affected(ch,gsn_imprisonvoice)) {
	    send_to_char("Your vocal cords are frozen solid!\n\r", ch);
	    return;
	}
    if (is_affected(ch, gsn_gag))
		return send_to_char("You are gagged and cannot speak!\n\r", ch);
	check_ooc(ch,argument,"GROUPTELL");
	
	if (is_affected(ch,gsn_deafen)) {
		deaf = TRUE;
		argument = palloc_string(upstring(argument));
	}

    sprintf(buf,"You tell the group '%s$t%s'",get_char_color(ch,"grouptells"),END_COLOR(ch));
    act_new(buf,ch,argument,NULL,TO_CHAR,POS_SLEEPING);


    for ( gch = char_list; gch != NULL; gch = gch->next )
    {
	if ( is_same_group( gch, ch ) && !is_affected(gch, gsn_deafen))
	{
	    sprintf(buf, "$n tells the group '%s$t%s'", get_char_color(gch,"grouptells"),END_COLOR(gch));
            act_new(buf,ch,argument,gch,TO_VICT,POS_SLEEPING);
	}
    }

	if (deaf)
		free_pstring(argument);
	
    return;
}

SPEECH_DATA *find_speech(MOB_INDEX_DATA *mob, char *name)
{
	SPEECH_DATA *speech;
	bool found = FALSE;

	if (!mob->speech)
		return NULL;

	for (speech = mob->speech; speech; speech = speech->next) {
		if (!str_cmp(speech->name,name)) {
			found = TRUE;
			break;
		}
	}

	if (found)
		return speech;
	else
		return NULL;
}

void execute_speech(CHAR_DATA *ch, CHAR_DATA *mob, SPEECH_DATA *speech)
{
	if (!speech || !speech->first_line || !mob || !mob->in_room)
		return;

	if (speech->first_line != speech->current_line)
		return;

	speech_handler(ch, mob, speech);

	return;
}

void speech_handler(CHAR_DATA *ch, CHAR_DATA *mob, SPEECH_DATA *speech)
{
	LINE_DATA *line;
	char buf[MSL];
	char *point, *copy;
	int i;

	if (!speech || !speech->first_line || !mob || !mob->in_room)
		return;

	if (mob->position < POS_RESTING)
		return;

	if ((line = speech->current_line) == NULL) {
		speech->current_line = speech->first_line;
		return;
	}

	for (i = 0; i < MSL; i++)
		buf[i] = '\0';

	copy = palloc_string(line->text);

	point = copy;

	for (i = 0; *point && *point != '\0'; i++) {
		if (*point == '$') {
			point++;
			if (*point == 'N') {
				strcat(buf,ch->name);
				i += (strlen(ch->name) - 1);
			} else {
				point--;
				buf[i] = *point;
			}
		} else {
			buf[i] = *point;
		}
		point++;
	}

	free_pstring(copy);
			
	switch(line->type) {
		case(SPEECH_SAY):
			do_say(mob,buf);
			break;
		case(SPEECH_SAYTO):
			say_to(mob,ch,buf,"");
			break;
		case(SPEECH_TELL):
			sprintf(buf,"%s %s",ch->name,buf);
			do_tell(mob,buf);
			break;
		case(SPEECH_WHISPER):
			do_whisper(mob,buf);
			break;
		case(SPEECH_YELL):
			do_yell(mob,buf);
			break;
		case(SPEECH_EMOTE):
			do_emote(mob,buf);
			break;
		case(SPEECH_ECHO):
			break;
		case(SPEECH_SING):
			do_sing(mob,buf);
			break;
		default:
			return bug ("Error in speech -- missing/invalid type.",0);
	}

	if (!line->next) {
		speech->current_line = speech->first_line;
		return;
	}

	speech->current_line = line->next;

	if (speech->current_line->delay <= 0) {
		speech_handler(ch, mob, speech);
		return;
	}

	RS.Queue.AddToQueue(speech->current_line->delay, 3, speech_handler, ch, mob, speech);

	return;
}

/*
 * It is very important that this be an equivalence relation:
 * (1) A ~ A
 * (2) if A ~ B then B ~ A
 * (3) if A ~ B  and B ~ C, then A ~ C
 */
bool is_same_group( CHAR_DATA *ach, CHAR_DATA *bch )
{
    if ( ach == NULL || bch == NULL)
	return FALSE;
/*    if ( ( ach->level - bch->level > 8 || ach->level - bch->level < -8 ) && !IS_NPC(ach) )
	return FALSE;*/
    if ( ach->leader != NULL ) ach = ach->leader;
    if ( bch->leader != NULL ) bch = bch->leader;
    return ach == bch;
}

void do_release(CHAR_DATA *ch,char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    one_argument(argument,arg);
    if (arg[0] == '\0')
    {
	send_to_char("Release which follower?\n\r",ch);
	return;
    }

    victim = get_char_room(ch,arg);
    if (victim == NULL)
    {
	send_to_char("They aren't here.\n\r",ch);
    return;
    }

    if (!IS_AFFECTED(victim,AFF_CHARM)
    || victim->master != ch)
    {
	send_to_char("They aren't under your control.\n\r",ch);
	return;
    }

    if (IS_NPC(victim))
    {
    act("$n slowly fades away.",victim,0,0,TO_ROOM);
    extract_char(victim,TRUE);
    }
    else
    {
    send_to_char("You can't release a player into the void..maybe nofollow them?\n\r",ch);
    return;
    }
    return;
}

void perm_death_log( CHAR_DATA *ch, int deltype )
{
	FILE *fp;

	if ((fp = fopen( PDEATH_LOG_FILE, "a" )) != NULL)
	{

	fprintf(fp,"%s||[%2d %5s %s]||%s%s%s%s||Kills: %d||Deaths: %d||Age: %d||Hours: %d||%s",
	deltype == 1 ? "DELETED" : deltype == 2 ? "CON-DIE" : deltype == 3 ? "OLD-AGE" : deltype == 4 ? "AUTODELETE" : "DELETED",
	ch->level,
	pc_race_table[ch->race].who_name,
	(RSTR)ch->Class()->who_name,
	cabal_table[ch->cabal].who_name,
	ch->true_name,
	IS_NPC(ch) ? "" : ch->pcdata->title,
	IS_NPC(ch) ? "" : (ch->pcdata->extitle) ? ch->pcdata->extitle : "",
	ch->pcdata->kills[PK_KILLS],
	ch->pcdata->killed[PK_KILLED],
	get_age(ch),
	(int) (ch->played + current_time - ch->logon) / 3600,
	(char *) ctime( &current_time )
	);

	fclose(fp);
	}
}

void temp_death_log( CHAR_DATA *killer, CHAR_DATA *dead)
{
	char query[MSL];

	if ( IS_NPC(killer) && (killer->master != NULL))
		killer = killer->master;
	if (IS_NPC(dead) || IS_NPC(killer))
		return;
	sprintf(query,"INSERT INTO pklogs VALUES('%s', %d, '%s', %d, '%s', '%s (%d)',%ld)",
		killer->true_name, killer->cabal, dead->true_name, dead->cabal, log_time(), escape_string(get_room_name(dead->in_room)), dead->in_room->vnum, current_time);
	one_query(query);
}

void mob_death_log( CHAR_DATA *killer, CHAR_DATA *dead)
{
	FILE *fp;

	if (!IS_NPC(killer) || IS_NPC(dead) || !IS_EXPLORE(killer->in_room)) {
		return;
	}

	if ((fp = fopen( MOB_LOG_FILE, "a" )) != NULL)
	{
		fprintf(fp,"%s | %s | %s | %s | %s",
		dead->true_name,
		killer->short_descr,
		killer->in_room->area->name,
		get_room_name(killer->in_room),
		(char *) ctime( &current_time )
		);
		fclose(fp);
	}
}

/* type 0 = create, 1 = login, 2 = logout */
void login_log(CHAR_DATA *ch, int type)
{
	char query[MSL];
	char *escape;

	if(!ch->pcdata->host && !ch->desc)
		return;
	if(IS_SET(ch->comm, COMM_NOSOCKET))
		return;

	escape = (char *) ((ch->pcdata->host) ? escape_string(ch->pcdata->host) : escape_string(ch->desc->host));
	
	sprintf(query,"INSERT INTO logins VALUES('%s', '%s', '%s', '%ld', '%ld', '%d', '%d', '%d')",
		ch->true_name,
		escape,
		log_time(),
		current_time,
		type == 2 ? ((current_time - ch->logon) / 60) : -1,
		type > 0 ? count_carried(ch,FALSE) : -1,
		type > 0 ? count_carried(ch,TRUE) : -1,
		type);
	one_query(query);
}
