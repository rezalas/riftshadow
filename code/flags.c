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
#include "tables.h"

int flag_lookup args( ( const char *name, const struct flag_type *flag_table) );

void do_flag(CHAR_DATA *ch, char *argument)
{
    char arg1[MAX_INPUT_LENGTH],arg2[MAX_INPUT_LENGTH],arg3[MAX_INPUT_LENGTH];
    char word[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    long *flag = 0;
    long pos, pos2 = NO_FLAG;
    char type;
    const struct flag_type *flag_table;

    argument = one_argument(argument,arg1);
    argument = one_argument(argument,arg2);
    argument = one_argument(argument,arg3);

    type = argument[0];

    if (type == '=' || type == '-' || type == '+')
        argument = one_argument(argument,word);

    if (arg1[0] == '\0')
    {
	send_to_char("Syntax:\n\r",ch);
	send_to_char("  flag mob  <name> <field> <+/-/=> <flags>\n\r",ch);
	send_to_char("  flag char <name> <field> <+/-/=> <flags>\n\r",ch);
	send_to_char("  flag obj  <name> <field> <+/-/=> <flags>\n\r",ch);
	send_to_char("  mob  flags: act,aff,off,imm,res,vuln,form,part\n\r",ch);
	send_to_char("  char flags: plr,comm,aff,imm,res,vuln\n\r",ch);
	send_to_char("  obj flags:  wear,extra\n\r",ch);
	send_to_char("  +: add flag, -: remove flag, = set equal to\n\r",ch);
	send_to_char("  otherwise flag toggles the flags listed.\n\r",ch);
	return;
    }

    if (arg2[0] == '\0')
    {
	send_to_char("What do you wish to set flags on?\n\r",ch);
	return;
    }

    if (arg3[0] == '\0')
    {
	send_to_char("You need to specify a flag to set.\n\r",ch);
	return;
    }

    if (argument[0] == '\0')
    {
	send_to_char("Which flags do you wish to change?\n\r",ch);
	return;
    }

    if (!str_prefix(arg1,"mob") || !str_prefix(arg1,"char"))
    {
	victim = get_char_world(ch,arg2);
	if (victim == NULL)
	{
	    send_to_char("You can't find them.\n\r",ch);
	    return;
	}

        /* select a flag to set */
	if (!str_prefix(arg3,"act"))
	{
	    if (!IS_NPC(victim))
	    {
		send_to_char("Use plr for PCs.\n\r",ch);
		return;
	    }

	    flag = &victim->act[0];
	    flag_table = act_flags;
	}

	else if (!str_prefix(arg3,"plr"))
	{
	    if (IS_NPC(victim))
	    {
		send_to_char("Use act for NPCs.\n\r",ch);
		return;
	    }

	    flag = &victim->act[0];
	    flag_table = plr_flags;
	}

 	else if (!str_prefix(arg3,"aff"))
	{
            flag = &victim->affected_by[0];
	    flag_table = affect_flags;
	}

  	else if (!str_prefix(arg3,"immunity"))
	{
            flag = &victim->imm_flags[0];
	    flag_table = imm_flags;
	}

	else if (!str_prefix(arg3,"resist"))
	{
            flag = &victim->res_flags[0];
	    flag_table = imm_flags;
	}

	else if (!str_prefix(arg3,"vuln"))
	{
            flag = &victim->vuln_flags[0];
	    flag_table = imm_flags;
	}

	else if (!str_prefix(arg3,"off"))
	{
	    if (!IS_NPC(victim))
	    {
	 	send_to_char("OFF can't be set on PCs.\n\r",ch);
		return;
	    }
            flag = &victim->off_flags[0];
	    flag_table = off_flags;
	}

	else if (!str_prefix(arg3,"form"))
	{
	    if (!IS_NPC(victim))
	    {
	 	send_to_char("Form can't be set on PCs.\n\r",ch);
		return;
	    }
            flag = &victim->form[0];
	    flag_table = form_flags;
	}

	else if (!str_prefix(arg3,"parts"))
	{
	    if (!IS_NPC(victim))
	    {
		send_to_char("Parts can't be set on PCs.\n\r",ch);
		return;
	    }

            flag = &victim->parts[0];
		flag_table = part_flags;
	}

	else if (!str_prefix(arg3,"comm"))
	{
	    if (IS_NPC(victim))
	    {
		send_to_char("Comm can't be set on NPCs.\n\r",ch);
		return;
	    }
            flag = &victim->comm[0];
	    flag_table = comm_flags;
	}

	else
	{
	    send_to_char("That's not an acceptable flag.\n\r",ch);
	    return;
	}
    }
    else if (!str_prefix(arg1,"obj"))
    {
	if ( ( obj = get_obj_world( ch, arg2 ) ) == NULL )
	{
	send_to_char( "Nothing like that anywhere.\n\r", ch );
	return;
	}

	/* select a flag to set */
	if (!str_prefix(arg3,"wear"))
	{
            	flag = &obj->wear_flags[0];
		flag_table = wear_flags;
	}
	else if (!str_prefix(arg3,"extra"))
	{
		flag = &obj->extra_flags[0];
		flag_table = extra_flags;
	}
	else
	{
		send_to_char("That's not an acceptable flag.\n\r",ch);
		return;
	}
    } else {
	send_to_char("Syntax:\n\r",ch);
	send_to_char("  flag mob  <name> <field> <+/-/=> <flags>\n\r",ch);
	send_to_char("  flag char <name> <field> <+/-/=> <flags>\n\r",ch);
	send_to_char("  flag obj  <name> <field> <+/-/=> <flags>\n\r",ch);
	send_to_char("  mob  flags: act,aff,off,imm,res,vuln,form,part\n\r",ch);
	send_to_char("  char flags: plr,comm,aff,imm,res,vuln\n\r",ch);
	send_to_char("  obj flags:  wear,extra\n\r",ch);
	send_to_char("  +: add flag, -: remove flag, = set equal to\n\r",ch);
	send_to_char("  otherwise flag toggles the flags listed.\n\r",ch);
	return;
    }

        /* mark the words */
        for (; ;)
        {
	    argument = one_argument(argument,word);

	    if (word[0] == '\0')
		break;

	    pos2 = flag_lookup(word,flag_table);
	    if (pos2 == NO_FLAG)
	    {
		send_to_char("That flag doesn't exist!\n\r",ch);
		return;
	    }
	}

	for (pos = 0; flag_table[pos].name != NULL; pos++)
	{
	    if ((get_trust(ch) == MAX_LEVEL || flag_table[pos].settable) && flag_table[pos].bit == pos2)
	    {
		switch(type)
		{
		    case '=':
		    case '+':
			SET_BIT(flag,flag_table[pos].bit);
			send_to_char("You set that flag.\n\r",ch);
			break;
		    case '-':
			REMOVE_BIT(flag,flag_table[pos].bit);
			send_to_char("You removed that flag.\n\r",ch);
			break;
		    default:
			if (IS_SET(flag,flag_table[pos].bit))
			{
			    REMOVE_BIT(flag,flag_table[pos].bit);
			    send_to_char("You removed that flag.\n\r",ch);
			}
			else
			{
			    SET_BIT(flag,flag_table[pos].bit);
			    send_to_char("You set that flag.\n\r",ch);
			}
		}
	    } else {
		continue;
	    }
	}
	return;
}
