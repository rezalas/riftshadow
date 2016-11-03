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
#include "merc.h"
#include "recycle.h"
#include "tables.h"
#include "lookup.h"

void do_invoke_jackass args((CHAR_DATA *ch));
void do_invoke_dioxide args((CHAR_DATA *ch));
void do_invoke_seracuz args((CHAR_DATA *ch));
void do_invoke_butter args((CHAR_DATA *ch));
void do_invoke_jhyrbian args((CHAR_DATA *ch));
void do_invoke_dev args((CHAR_DATA *ch, char *argument));
void do_invoke_daedelex args((CHAR_DATA *ch));
void do_invoke_xanthrailles args((CHAR_DATA *ch));
void do_invoke_iomahk args((CHAR_DATA *ch));
void do_invoke_detlef args((CHAR_DATA *ch));
void do_invoke_lestregus args((CHAR_DATA *ch));
void do_invoke_kyrissaean args((CHAR_DATA *ch));
void do_invoke_gabe args((CHAR_DATA *ch));
void do_invoke_lichen  args((CHAR_DATA *ch));

void do_brands( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    OBJ_DATA *objj;

    if (ch->level < 53 || IS_NPC(ch))
    {
        send_to_char("Huh?\n\r", ch);
        return;
    }
    argument = one_argument( argument, arg1 );

    if ( arg1[0] == '\0' )
    {
        send_to_char("Syntax: tattoo <char>\n\r", ch);
        return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
        send_to_char( "They aren't playing.\n\r", ch );
        return;
    }

/*    if ( IS_NPC(victim) )
    {
	send_to_char( "Trying to tattoo a mob?\n\r", ch );
	return;
    }
*/
    if (ch == victim)
    {
        send_to_char( "Trying to tattoo yourself?\n\r", ch );
        return;
    }
    if ( (obj = get_eq_char(victim, WEAR_BRAND) ) != NULL )
    {
        send_to_char("That person already has a tattoo.\n\r", ch);
        return;
    }

    if ( (obj = get_eq_char(ch, WEAR_BRAND) ) != NULL )
    {
	objj = create_object(obj->pIndexData, 0);
        clone_object(obj, objj);
        obj_to_char( objj, victim );
        equip_char( victim, objj, WEAR_BRAND, TRUE);
        act("You mark $N with $p!",ch,objj,victim,TO_CHAR);
	act("$n marks you with $p!",ch,objj,victim,TO_VICT);
	act("$n marks $N with $p!",ch,objj,victim,TO_NOTVICT);
        return;
    }
    send_to_char("You don't have a brand yourself.\n\r", ch);
    return;
}

void do_unbrands( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;

    if (ch->level < 53 || IS_NPC(ch))
    {
        send_to_char("Huh?\n\r", ch);
        return;
    }
    argument = one_argument( argument, arg1 );

    if ( arg1[0] == '\0' )
    {
        send_to_char("Syntax: untattoo <char>\n\r", ch);
        return;
    }
    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
        send_to_char( "They aren't playing.\n\r", ch );
        return;
    }
    if (IS_NPC(victim))
    {
	send_to_char( "Trying to untattoo a mob?\n\r", ch );
	return;
    }
    if (ch == victim)
    {
        send_to_char( "Trying to untattoo yourself?\n\r", ch );
        return;
    }
    if ((obj = get_eq_char(victim, WEAR_BRAND)) == NULL)
    {
	send_to_char( "That person doesn't have a tattoo.\n\r", ch );
	return;
    }
    unequip_char(victim, obj, TRUE);
    obj_from_char(obj);
    extract_obj(obj);
    send_to_char("You have untattooed that person.\n\r", ch);
    send_to_char("Your mark is painfully removed!\n\r", victim);
    return;
}

void do_invoke( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj;
	char arg[MAX_STRING_LENGTH];

	argument = one_argument(argument,arg);

	if (arg[0] == '\0')
	{
		send_to_char("Invoke what?\n\r",ch);
		return;
	}

	if ((obj = get_obj_wear(ch,arg)) == NULL)
	{
		send_to_char("You're not wearing that.\n\r",ch);
		return;
	}

	if (IS_SET(obj->progtypes,IPROG_INVOKE))
	{
		(obj->pIndexData->iprogs->invoke_prog) (obj,ch,argument);
	} else {
		send_to_char("You can't invoke that.\n\r",ch);
		return;
	}
}

void do_invoke_jackass( CHAR_DATA *ch )
{
	OBJ_DATA *obj;

	obj = get_eq_char(ch, WEAR_BRAND);
      act("$n's $p doesn't glow much due to $s stupidity.",ch,obj,0,TO_ROOM);
      act("Your $p doesn't glow much, probably because you're a jackass.",ch,obj,0,TO_CHAR);
      send_to_char("You smite yourself! What a jackass!\n\r",ch);
      act("$n smites $mself! What a jackass!",ch,NULL,NULL,TO_ROOM);
	ch->hit /= 2;
	return;
}

/*
void do_invoke_detlef( CHAR_DATA *ch )
{
	OBJ_DATA *obj;
	AFFECT_DATA af;

	if (is_affected(ch,gsn_phat_blunt))
	{
        send_to_char("You cannot smoke phat blunt too much!\n\r",ch);
        return;
	}

	obj = get_eq_char(ch, WEAR_BRAND);
        act("$n's $p glows green.",ch,obj,0,TO_ROOM);
        act("Your $p glows green.",ch,obj,0,TO_CHAR);
	
        ch->hit += (ch->level*2) * 10;
        ch->hit = UMIN(ch->hit,ch->max_hit);
        send_to_char("You smoke a phat blunt and feel MUCH better.\n\r",ch);
        act("$n smokes $s phat blunt and feels much better.",ch,NULL,NULL,TO_ROOM);

	if (check_dispel(ch->level+3,ch,skill_lookup("blindness")))
	{
		act("$n is no longer blinded.",ch,NULL,NULL,TO_ROOM);
	}
	if (check_dispel(ch->level+3,ch,skill_lookup("poison")))
	{
		act("$n looks less ill.",ch,NULL,NULL,TO_ROOM);
	}
	if (check_dispel(ch->level+3,ch,skill_lookup("plague")))
	{
		act("$n's sores vanish.",ch,NULL,NULL,TO_ROOM);
	}
	
      init_affect(&af);
      af.where        = TO_AFFECTS;
af.aftype        = AFT_SPELL;
      af.type         = gsn_phat_blunt;
      af.level        = ch->level;
      af.duration     = 20;
      af.modifier     = 20;
      af.bitvector    = 0;
      af.location     = APPLY_HITROLL;
      affect_to_char(ch,&af);
      af.location     = APPLY_DAMROLL;
      affect_to_char(ch,&af);
      return;
}
*/

void do_invoke_gabe ( CHAR_DATA *ch )
{
        OBJ_DATA *obj;

        if(ch->position==POS_FIGHTING) {
                act("You can not invoke your lords power during a fight!",ch,0,0,TO_CHAR);
                return;
        }

        obj = get_eq_char(ch, WEAR_BRAND);

        act("$n's $p glows a bright gold.",ch,obj,0,TO_ROOM);
        act("Your $p glows a bright gold.",ch,obj,0,TO_CHAR);

}

