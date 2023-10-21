/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik Stærfeldt, Tom Madsen, and Katja Nyboe.   *
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
 *  ROM 2.4 is copyright 1993-1996 Russ Taylor                             *
 *  ROM has been brought to you by the ROM consortium                      *
 *      Russ Taylor (rtaylor@efn.org)                                      *
 *      Gabrielle Taylor (gtaylor@pacinfo.com)                             *
 *      Brian Moore (zump@rom.org)                                         *
 *  By using this code, you have agreed to follow the terms of the         *
 *  ROM license, in the file Tartarus/doc/rom.license                      *
 ***************************************************************************/

/***************************************************************************
 *       Tartarus code is copyright (C) 1997-1998 by Daniel Graham         *
 *  In using this code you agree to comply with the Tartarus license       *
 *       found in the file /Tartarus/doc/tartarus.doc                      *
 ***************************************************************************/

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "alias.h"
#include "newmem.h"
#include "comm.h"
#include "interp.h"
#include "db.h"
#include "utility.h"
#include "./include/fmt/format.h"

/* does aliasing and other fun stuff */
void substitute_alias(DESCRIPTOR_DATA *d, char *argument)
{
	CHAR_DATA *ch;
	char buf[MAX_STRING_LENGTH], name[MAX_INPUT_LENGTH];
	char *point;
	int alias;

	ch = d->original ? d->original : d->character;

	if (is_npc(ch)
		|| ch->pcdata->alias[0] == nullptr
		|| !str_prefix("alias", argument)
		|| !str_prefix("una", argument)
		|| !str_prefix("prefix", argument))
	{
		interpret(d->character, argument);
		return;
	}

	sprintf(buf, "%s", argument);

	for (alias = 0; alias < MAX_ALIAS; alias++) /* go through the aliases */
	{
		if (ch->pcdata->alias[alias] == nullptr)
			break;

		if (!str_prefix(ch->pcdata->alias[alias], argument))
		{
			point = one_argument(argument, name);

			if (!strcmp(ch->pcdata->alias[alias], name))
			{
				buf[0] = '\0';

				strcat(buf, ch->pcdata->alias_sub[alias]);
				strcat(buf, " ");
				strcat(buf, point);
				break;
			}

			if (strlen(buf) > MAX_INPUT_LENGTH)
			{
				send_to_char("Alias substitution too long. Truncated.\r\n", ch);
				buf[MAX_INPUT_LENGTH - 1] = '\0';
			}
		}
	}

	interpret(d->character, buf);
}

void do_alia(CHAR_DATA *ch, char *argument)
{
	send_to_char("I'm sorry, alias must be entered in full.\n\r", ch);
}

void do_alias(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *rch;
	char arg[MAX_INPUT_LENGTH], buf[MAX_STRING_LENGTH];
	std::string buffer;
	int pos;
	smash_tilde(argument);

	if (ch->desc == nullptr)
		rch = ch;
	else
		rch = ch->desc->original ? ch->desc->original : ch;

	if (is_npc(rch))
		return;

	argument = one_argument(argument, arg);

	if (arg[0] == '\0')
	{
		if (rch->pcdata->alias[0] == nullptr)
		{
			send_to_char("You have no aliases defined.\n\r", ch);
			return;
		}

		send_to_char("Your current aliases are:\n\r", ch);

		for (pos = 0; pos < MAX_ALIAS; pos++)
		{
			if (rch->pcdata->alias[pos] == nullptr || rch->pcdata->alias_sub[pos] == nullptr)
				break;

			sprintf(buf, "    %s:  %s\n\r", rch->pcdata->alias[pos], rch->pcdata->alias_sub[pos]);
			send_to_char(buf, ch);
		}

		return;
	}

	if (!str_prefix("una", arg) || !str_cmp("alias", arg))
	{
		send_to_char("Sorry, that word is reserved.\n\r", ch);
		return;
	}

	if (argument[0] == '\0')
	{
		for (pos = 0; pos < MAX_ALIAS; pos++)
		{
			if (rch->pcdata->alias[pos] == nullptr || rch->pcdata->alias_sub[pos] == nullptr)
				break;

			if (!str_cmp(arg, rch->pcdata->alias[pos]))
			{
				sprintf(buf, "%s aliases to '%s'.\n\r", rch->pcdata->alias[pos], rch->pcdata->alias_sub[pos]);
				send_to_char(buf, ch);
				return;
			}
		}

		send_to_char("That alias is not defined.\n\r", ch);
		return;
	}

	if (!str_prefix(argument, "delete") || !str_prefix(argument, "prefix"))
	{
		send_to_char("That shall not be done!\n\r", ch);
		return;
	}

	for (pos = 0; pos < MAX_ALIAS; pos++)
	{
		if (rch->pcdata->alias[pos] == nullptr)
			break;

		if (!str_cmp(arg, rch->pcdata->alias[pos])) /* redefine an alias */
		{
			free_pstring(rch->pcdata->alias_sub[pos]);
			rch->pcdata->alias_sub[pos] = palloc_string(argument);

			buffer = fmt::format("{} is now realiased to '{}'.\n\r", arg, argument); //TODO: change the rest of the sprintf calls to format
			send_to_char(buffer.c_str(), ch);
			return;
		}
	}

	if (pos >= MAX_ALIAS)
	{
		send_to_char("Sorry, you have reached the alias limit.\n\r", ch);
		return;
	}

	/* make a new alias */
	rch->pcdata->alias[pos] = palloc_string(arg);
	rch->pcdata->alias_sub[pos] = palloc_string(argument);

	buffer = fmt::format("{} is now aliased to '{}'.\n\r", arg, argument);
	send_to_char(buffer.c_str(), ch);
}

void do_unalias(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *rch;
	char arg[MAX_INPUT_LENGTH];
	int pos;
	bool found = false;

	if (ch->desc == nullptr)
		rch = ch;
	else
		rch = ch->desc->original ? ch->desc->original : ch;

	if (is_npc(rch))
		return;

	argument = one_argument(argument, arg);

	if (arg == (char *)'\0')
	{
		send_to_char("Unalias what?\n\r", ch);
		return;
	}

	for (pos = 0; pos < MAX_ALIAS; pos++)
	{
		if (rch->pcdata->alias[pos] == nullptr)
			break;

		if (found)
		{

			rch->pcdata->alias[pos - 1] = rch->pcdata->alias[pos];
			rch->pcdata->alias_sub[pos - 1] = rch->pcdata->alias_sub[pos];
			rch->pcdata->alias[pos] = nullptr;
			rch->pcdata->alias_sub[pos] = nullptr;
			continue;
		}

		if (!strcmp(arg, rch->pcdata->alias[pos]))
		{
			send_to_char("Alias removed.\n\r", ch);

			free_pstring(rch->pcdata->alias[pos]);
			free_pstring(rch->pcdata->alias_sub[pos]);
			rch->pcdata->alias[pos] = nullptr;
			rch->pcdata->alias_sub[pos] = nullptr;

			found = true;
		}
	}

	if (!found)
		send_to_char("No alias of that name to remove.\n\r", ch);
}
