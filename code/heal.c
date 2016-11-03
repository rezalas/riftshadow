/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,	   *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *									   *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael	   *
 *  Chastain, Michael Quan, and Mitchell Tse.				   *
 *									   *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc	   *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.						   *
 *									   *
 *  Much time and thought has gone into this software and you are	   *
 *  benefitting.  We hope that you share your changes too.  What goes	   *
 *  around, comes around.						   *
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
#include "magic.h"

DECLARE_DO_FUN(do_say);

float get_mod_cost(CHAR_DATA *ch, int cost)
{

    return cost * calculate_inflation();
}


/* This is all from Rom except the restoration spell */
void do_heal(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *mob;
    char arg[MAX_INPUT_LENGTH];
    char buf[MSL];
    int cost, sn, mod_cost,level=0;
    SPELL_FUN *spell;
    char *words;
    OBJ_DATA *cursed = NULL;


    /* check for healer */
    for ( mob = ch->in_room->people; mob; mob = mob->next_in_room )
        if ( IS_NPC(mob) && IS_SET(mob->act, ACT_IS_HEALER) )
            break;

    if ( mob == NULL )
        return send_to_char( "You can't do that here.\n\r", ch );

    if(mob->position==POS_SLEEPING)
        return send_to_char("The healer must be awake to heal you.\n\r",ch);

    argument = one_argument(argument,arg);

    if (mob->cabal != 0)
        if (mob->cabal != ch->cabal)
            return do_say(mob,"Begone, intruder!  I'll not heal you!");

    if (arg[0] == '\0') {
        /* display price list */
        act("$N says 'I offer the following spells:'",ch,NULL,mob,TO_CHAR);
        mod_cost = (int)get_mod_cost(mob, 2500);
        sprintf(buf,"  heal   : healing spell            %6d gold\n\r",mod_cost);
        send_to_char(buf,ch);
        mod_cost = (int)get_mod_cost(mob, 2000);
        sprintf(buf,"  uncurse: remove curse             %6d gold\n\r",mod_cost);
        send_to_char(buf,ch);
        sprintf(buf,"  uncurse <name>: uncurse object    %6d gold\n\r",mod_cost);
        send_to_char(buf,ch);
        mod_cost = (int)get_mod_cost(mob, 1000);
        sprintf(buf,"  critic : cure critical wounds     %6d gold\n\r",mod_cost);
        send_to_char(buf,ch);
        mod_cost = (int)get_mod_cost(mob, 500);
        sprintf(buf,"  poison : cure poison              %6d gold\n\r",mod_cost);
        send_to_char(buf,ch);
        mod_cost = (int)get_mod_cost(mob, 500);
        sprintf(buf,"  blind  : cure blindness           %6d gold\n\r",mod_cost);
        send_to_char(buf,ch);
        mod_cost = (int)get_mod_cost(mob, 1000);
        sprintf(buf,"  disease: cure disease             %6d gold\n\r",mod_cost);
        send_to_char(buf,ch);
        mod_cost = (int)get_mod_cost(mob, 500);
        sprintf(buf,"  serious: cure serious wounds      %6d gold\n\r",mod_cost);
        send_to_char(buf,ch);
        mod_cost = (int)get_mod_cost(mob, 100);
        sprintf(buf,"  light  : cure light wounds        %6d gold\n\r",mod_cost);
        send_to_char(buf,ch);
        mod_cost = (int)get_mod_cost(mob, 500);
        sprintf(buf,"  mana   : restore mana             %6d gold\n\r",mod_cost);
        send_to_char(buf,ch);
        mod_cost = (int)get_mod_cost(mob, 100);
        sprintf(buf,"  refresh: restore movement         %6d gold\n\r",mod_cost);
        send_to_char(buf,ch);
        send_to_char(" Type heal <type> to be healed.\n\r",ch);
        return;
    }

    if (!str_prefix(arg,"light")) {
        spell = spell_cure_light;
        sn    = skill_lookup("cure light");
        words = "cure light";
        cost  = 100;
    } else if (!str_prefix(arg,"serious")) {
        spell = spell_cure_serious;
        sn    = skill_lookup("cure serious");
        words = "cure serious";
        cost  = 500;
    } else if (!str_prefix(arg,"critical")) {
        spell = spell_cure_critical;
        sn    = skill_lookup("cure critical");
        words = "cure critical";
        cost  = 1000;
    } else if (!str_prefix(arg,"heal")) {
        spell = spell_heal;
        sn = skill_lookup("heal");
        words = "heal";
        cost  = 2500;
    } else if (!str_prefix(arg,"blindness")) {
        spell = spell_cure_blindness;
        sn    = skill_lookup("cure blindness");
        words = "cure blindness";
        cost  = 500;
    } else if (!str_prefix(arg,"disease")) {
        spell = spell_cure_disease;
        sn    = skill_lookup("cure disease");
        words = "cure disease";
        cost = 1000;
    } else if (!str_prefix(arg,"poison")) {
        spell = spell_cure_poison;
        sn    = skill_lookup("cure poison");
        words = "cure poison";
        cost  = 500;
    } else if (!str_prefix(arg,"uncurse") || !str_prefix(arg,"curse")) {
        spell = spell_remove_curse;
        sn    = skill_lookup("remove curse");
        argument = one_argument(argument, buf);
        if(buf[0] != '\0')
            if(!(cursed = get_obj_wear(ch, buf)) && !(cursed = get_obj_carry(ch, buf, ch)))
                return send_to_char("You don't have that.\n\r",ch);
        words = "remove curse";
        cost  = 2000;
    } else if (!str_prefix(arg,"mana") || !str_prefix(arg,"energize")) {
        spell = NULL;
        sn = -1;
        words = "energizer";
        cost = 500;
    } else if (!str_prefix(arg,"refresh") || !str_prefix(arg,"moves")) {
        spell =  spell_refresh;
        sn    = skill_lookup("refresh");
        words = "refresh";
        cost  = 100;
    } else {
        act("$N says 'Type 'heal' for a list of spells.'",
            ch,NULL,mob,TO_CHAR);
        return;
    }

    cost = (int)get_mod_cost(mob, cost);

    if (cost > ch->gold) {
        do_say(mob,"You do not have enough gold for my services.");
        return;
    }

    WAIT_STATE(ch, (ch->cabal == CABAL_HORDE) ? (PULSE_VIOLENCE*3) : PULSE_VIOLENCE);



    deduct_cost(ch,cost);
    act("$n utters the words '$T'.",mob,NULL,words,TO_ROOM);

    if (spell == NULL) {
        ch->mana += dice(4,8) + mob->level / 3;
        ch->mana = UMIN(ch->mana,ch->max_mana);
        send_to_char("A warm glow passes through you.\n\r",ch);
        return;
    }

    if (sn == -1)
        return;

    level = mob->level;

    if(ch->cabal == CABAL_HORDE)
        level /= 2;

    spell(sn, level, !cursed ? mob : ch, !cursed ? (void *)ch : (void *)cursed, !cursed ? TARGET_CHAR : TARGET_OBJ);
}
