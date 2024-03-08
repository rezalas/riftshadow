/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik Stærfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license._doc' as well the Merc      *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

/***************************************************************************
 *   ROM 2.4 is copyright 1993-1996 Russ Taylor                             *
 *   ROM has been brought to you by the ROM consortium                      *
 *       Russ Taylor (rtaylor@pacinfo.com)                                  *
 *       Gabrielle Taylor (gtaylor@pacinfo.com)                             *
 *       Brian Moore (rom@rom.efn.org)                                      *
 *                                                                          *
 *   By using this code, you have agreed to follow the terms of the         *
 *   ROM license, in the file Tartarus/doc/rom.license                      *
 ****************************************************************************/

/***************************************************************************
 *       Tartarus code is copyright (C) 1997-1998 by Daniel Graham          *
 *    In using this code you agree to comply with the Tartarus license      *
 *       found in the file /Tartarus/doc/tartarus.doc                       *
 ****************************************************************************/

#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <mysql.h>
#include "act_comm.h"
#include "rift.h"
#include "recycle.h"
#include "tables.h"
#include "olc.h"
#include "spec.h"
#include "comm.h"
#include "devextra.h"
#include "characterClasses/warrior.h"
#include "save.h"
#include "act_info.h"
#include "newmem.h"
#include "act_wiz.h"
#include "interp.h"
#include "fight.h"
#include "skills.h"
#include "db.h"
#include "devextra.h"
#include "magic.h"
#include "dioextra.h"
#include "act_move.h"
#include "handler.h"
#include "characterClasses/ap.h"
#include "characterClasses/thief.h"
#include "chardef.h"
#include "update.h"
#include "./include/spdlog/fmt/bundled/format.h"
#include "./include/spdlog/fmt/bundled/printf.h"

/* RT code to delete yourself */
void do_delet(CHAR_DATA *ch, char *argument)
{
	send_to_char("You must type the full command to delete yourself.\n\r", ch);
}

void do_delete(CHAR_DATA *ch, char *argument)
{
	if (is_npc(ch))
		return;

	if (ch->pcdata->confirm_delete)
	{
		if (argument[0] != '\0')
		{
			send_to_char("Delete status removed.\n\r", ch);
			ch->pcdata->confirm_delete = false;
			return;
		}
		else
		{
			if (ch->level > 30)
				perm_death_log(ch, 1);

			char strsave[MAX_INPUT_LENGTH];
			sprintf(strsave, "%s/%s.plr", RIFT_PLAYER_DIR, capitalize(ch->name));
			wiznet("$N turns $Mself into line noise.", ch, nullptr, 0, 0, 0);
			ch->pause = 0;

			while (ch->affected)
			{
				affect_remove(ch, ch->affected);
			}

			stop_fighting(ch, true);
			act("$n has left Shalar, never to return.", ch, nullptr, nullptr, TO_ROOM);

			plug_graveyard(ch, 1);
			do_quit_new(ch, "", true);

			auto cname = palloc_string(ch->true_name);
			auto save_pfile = ((ch->played + (current_time - ch->logon)) / 3600) >= 15;
			delete_char(cname, true); // >= 15 hours. Make name unusable.

			free_pstring(cname);
			return;
		}
	}

	if (argument[0] != '\0')
	{
		send_to_char("Just type delete. No argument.\n\r", ch);
		return;
	}

	send_to_char("Type delete again to confirm this command.\n\r", ch);
	send_to_char("WARNING: this command is irreversible.\n\r", ch);
	send_to_char("Typing delete with an argument will undo delete status.\n\r", ch);

	ch->pcdata->confirm_delete = true;

	if (((ch->played + (current_time - ch->logon)) / 3600) >= 15)
		send_to_char("ALERT: Deletion will render this name unusable.\n\r", ch);

	wiznet("$N is contemplating deletion.", ch, nullptr, 0, 0, get_trust(ch));
}

/* RT code to display channel status */
void do_channels(CHAR_DATA *ch, char *argument)
{
	/* lists all channels and their status */
	send_to_char("   channel     status\n\r", ch);
	send_to_char("---------------------\n\r", ch);

	auto sendToChar = [](bool isSet, CHAR_DATA *ch) {
		auto txt = isSet ? "ON\n\r" : "OFF\n\r";
		send_to_char(txt, ch);
	};

	send_to_char("auction        ", ch);
	sendToChar(!IS_SET(ch->comm, COMM_NOAUCTION), ch);

	send_to_char("Q/A            ", ch);
	sendToChar(!IS_SET(ch->comm, COMM_NOQUESTION), ch);

	if (is_immortal(ch))
	{
		send_to_char("god channel    ", ch);
		sendToChar(!IS_SET(ch->comm, COMM_NOWIZ), ch);
	}

	send_to_char("tells          ", ch);
	sendToChar(!IS_SET(ch->comm, COMM_DEAF), ch);

	send_to_char("quiet mode     ", ch);
	sendToChar(IS_SET(ch->comm, COMM_QUIET), ch);

	if (IS_SET(ch->comm, COMM_SNOOP_PROOF))
		send_to_char("You are immune to snooping.\n\r", ch);

	if (ch->lines != PAGELEN)
	{
		if (ch->lines)
		{
			char buf[MAX_STRING_LENGTH];
			sprintf(buf, "You display %d lines of scroll.\n\r", ch->lines + 2);
			send_to_char(buf, ch);
		}
		else
		{
			send_to_char("Scroll buffering is off.\n\r", ch);
		}
	}

	if (IS_SET(ch->comm, COMM_NOTELL))
		send_to_char("You cannot use tell.\n\r", ch);

	if (IS_SET(ch->comm, COMM_NOCHANNELS))
		send_to_char("You cannot use channels.\n\r", ch);

	if (IS_SET(ch->comm, COMM_NOEMOTE))
		send_to_char("You cannot show emotions.\n\r", ch);
}

/* RT deaf blocks out all shouts */
void do_deaf(CHAR_DATA *ch, char *argument)
{
	if (IS_SET(ch->comm, COMM_DEAF))
	{
		send_to_char("You can now hear tells again.\n\r", ch);
		REMOVE_BIT(ch->comm, COMM_DEAF);
		return;
	}

	send_to_char("From now on, you won't hear tells.\n\r", ch);
	SET_BIT(ch->comm, COMM_DEAF);
}

/* RT quiet blocks out all communication */
void do_quiet(CHAR_DATA *ch, char *argument)
{
	if (IS_SET(ch->comm, COMM_QUIET))
	{
		send_to_char("Quiet mode removed.\n\r", ch);
		REMOVE_BIT(ch->comm, COMM_QUIET);
		return;
	}

	send_to_char("From now on, you will only hear says and emotes.\n\r", ch);
	SET_BIT(ch->comm, COMM_QUIET);
}

void do_replay(CHAR_DATA *ch, char *argument)
{
	if (is_npc(ch))
	{
		send_to_char("You can't replay.\n\r", ch);
		return;
	}

	if (!ch->pcdata->buffer->string || ch->pcdata->buffer->string[0] == '\0')
	{
		send_to_char("You have no tells to replay.\n\r", ch);
		return;
	}

	page_to_char(buf_string(ch->pcdata->buffer), ch);
	clear_buf(ch->pcdata->buffer);
}

void do_cb(CHAR_DATA *ch, char *argument)
{
	if (is_affected_area(ch->in_room->area, gsn_interference))
	{
		send_to_char("The electrical currents in the area block your attempt to communicate.\n\r", ch);
		return;
	}

	if (!is_cabal(ch) || cabal_table[ch->cabal].independent)
	{
		send_to_char("You aren't in a cabal.\n\r", ch);
		return;
	}

	if (argument[0] == '\0')
	{
		if (IS_SET(ch->comm, COMM_NOCABAL))
		{
			send_to_char("Cabal channel is now ON\n\r", ch);
			REMOVE_BIT(ch->comm, COMM_NOCABAL);
		}
		else
		{
			send_to_char("Cabal channel is now OFF\n\r", ch);
			SET_BIT(ch->comm, COMM_NOCABAL);
		}

		return;
	}

	if (is_shifted(ch) && !is_immortal(ch))
	{
		send_to_char("You can't seem to form the words.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_silence))
	{
		send_to_char("You cannot find the words within you.\n\r", ch);
		return;
	}

	if (IS_SET(ch->in_room->room_flags, ROOM_SILENCE))
	{
		send_to_char("You cannot find the words within you.\n\r", ch);
		return;
	}

	if (IS_SET(ch->comm, COMM_NOCHANNELS))
	{
		send_to_char("The gods have revoked your channel privileges.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_imprisonvoice))
	{
		send_to_char("Your vocal cords are frozen solid!\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_gag))
	{
		send_to_char("You are gagged and cannot speak!\n\r", ch);
		return;
	}

	check_ooc(ch, argument, "CB");

	if (is_affected(ch, gsn_deafen))
		sprintf(argument, "%s", upstring(argument));

	REMOVE_BIT(ch->comm, COMM_NOCABAL);

	char buf[MAX_STRING_LENGTH];
	if (IS_SET(ch->comm, COMM_ANSI))
	{
		sprintf(buf, "%s%s: %s%s%s\n\r",
				cabal_table[ch->cabal].who_name,
				is_npc(ch) ? ch->short_descr : ch->true_name,
				get_char_color(ch, "channels"),
				argument,
				END_COLOR(ch));
	}
	else
	{
		sprintf(buf, "%s%s: %s\n\r",
				cabal_table[ch->cabal].who_name,
				is_npc(ch) ? ch->short_descr : ch->name,
				argument);
	}

	send_to_char(buf, ch);

	for (auto d = descriptor_list; d != nullptr; d = d->next)
	{
		if (d->connected == CON_PLAYING && d->character != ch && ((is_same_cabal(ch, d->character) && !IS_SET(d->character->comm, COMM_NOCABAL) && !IS_SET(d->character->in_room->room_flags, ROOM_SILENCE)) || IS_SET(d->character->comm, COMM_ALL_CABALS)))
		{
			if (IS_SET(d->character->comm, COMM_ANSI))
			{
				sprintf(buf, "%s%s: %s%s%s\n\r",
						cabal_table[ch->cabal].who_name,
						!is_npc(ch) && can_see(d->character, ch) ? ch->true_name : pers(ch, d->character),
						get_char_color(d->character, "channels"),
						argument,
						END_COLOR(d->character));
			}
			else
			{
				sprintf(buf, "%s%s: %s\n\r",
						cabal_table[ch->cabal].who_name,
						!is_npc(ch) && can_see(d->character, ch) ? ch->true_name : pers(ch, d->character),
						argument);
			}

			send_to_char(buf, d->character);
		}
	}
}

void do_global(CHAR_DATA *ch, char *argument)
{
	if (IS_SET(ch->comm, COMM_ALL_CABALS))
	{
		send_to_char("You will no longer hear all Cabal channels.\n\r", ch);
		REMOVE_BIT(ch->comm, COMM_ALL_CABALS);
		return;
	}

	send_to_char("You will now hear all Cabal channels.\n\r", ch);
	SET_BIT(ch->comm, COMM_ALL_CABALS);
}

void do_newbie(CHAR_DATA *ch, char *argument)
{
	if (ch->level > 25 && !is_immortal(ch) && !IS_SET(ch->act, PLR_HEROIMM))
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	else
	{
		if (argument[0] == '\0')
		{
			if (IS_SET(ch->comm, COMM_NONEWBIE))
			{
				send_to_char("Newbie channel is now ON\n\r", ch);
				REMOVE_BIT(ch->comm, COMM_NONEWBIE);
			}
			else
			{
				send_to_char("Newbie channel is now OFF\n\r", ch);
				SET_BIT(ch->comm, COMM_NONEWBIE);
			}

			return;
		}
	}

	if (argument[0] == '\0')
	{
		send_to_char("Syntax: newbie <message>\n\r", ch);
		return;
	}

	char buf[MAX_STRING_LENGTH];
	char buf2[MAX_STRING_LENGTH];
	sprintf(buf, ": %s", argument);

	REMOVE_BIT(ch->comm, COMM_NONEWBIE);

	if (IS_SET(ch->comm, COMM_ANSI))
	{
		sprintf(buf2, "%s[NEWBIE] $n$t%s", get_char_color(ch, "newbie"), END_COLOR(ch));
		act_new(buf2, ch, buf, 0, TO_CHAR, POS_DEAD);
	}
	else
	{
		act_new("[NEWBIE] $n$t", ch, buf, 0, TO_CHAR, POS_DEAD);
	}

	for (auto wch = char_list; wch != nullptr; wch = wch->next)
	{
		if (is_npc(wch) && wch->desc == nullptr)
			continue;

		if ((wch->level <= 25 && !IS_SET(wch->comm, COMM_NONEWBIE)) || is_immortal(wch) || is_heroimm(wch) || (is_npc(wch) && (wch->desc != nullptr) && is_immortal(wch->desc->original)))
		{
			if (IS_SET(wch->comm, COMM_ANSI))
			{
				sprintf(buf2, "%s[NEWBIE] $n$t%s", get_char_color(wch, "newbie"), END_COLOR(wch));
				act_new(buf2, ch, buf, wch, TO_VICT, POS_DEAD);
			}
			else
				act_new("[NEWBIE] $n$t", ch, buf, wch, TO_VICT, POS_DEAD);
		}
	}
}

void do_builder(CHAR_DATA *ch, char *argument)
{
	int trust = ch->level;

	if (trust < 52 && !IS_SET(ch->act, PLR_HEROIMM))
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	else
	{
		if (argument[0] == '\0')
		{
			if (IS_SET(ch->comm, COMM_NOBUILDER))
			{
				send_to_char("Builder channel is now ON\n\r", ch);
				REMOVE_BIT(ch->comm, COMM_NOBUILDER);
			}
			else
			{
				send_to_char("Builder channel is now OFF\n\r", ch);
				SET_BIT(ch->comm, COMM_NOBUILDER);
			}

			return;
		}
	}

	if (argument[0] == '\0')
	{
		send_to_char("Syntax: builder <message>\n\r", ch);
		return;
	}

	char buf[MAX_STRING_LENGTH];
	char buf2[MAX_STRING_LENGTH];
	sprintf(buf, ": %s", argument);

	REMOVE_BIT(ch->comm, COMM_NOBUILDER);

	if (IS_SET(ch->comm, COMM_ANSI))
	{
		sprintf(buf2, "%s[BUILDER] $n$t%s", get_char_color(ch, "builder"), END_COLOR(ch));
		act_new(buf2, ch, buf, 0, TO_CHAR, POS_DEAD);
	}
	else
	{
		act_new("[BUILDER] $n$t", ch, buf, 0, TO_CHAR, POS_DEAD);
	}

	for (auto wch = char_list; wch != nullptr; wch = wch->next)
	{
		if (is_npc(wch) && wch->desc == nullptr)
			continue;

		if (is_immortal(wch) || IS_SET(wch->comm, COMM_BUILDER) || is_heroimm(wch) || (is_npc(wch) && (wch->desc != nullptr) && is_immortal(wch->desc->original)))
		{
			if (IS_SET(wch->comm, COMM_ANSI))
			{
				sprintf(buf2, "%s[BUILDER] $n$t%s", get_char_color(wch, "builder"), END_COLOR(wch));
				act_new(buf2, ch, buf, wch, TO_VICT, POS_DEAD);
			}
			else
			{
				act_new("[BUILDER] $n$t", ch, buf, wch, TO_VICT, POS_DEAD);
			}
		}
	}
}

void do_immtalk(CHAR_DATA *ch, char *argument)
{
	if (!is_immortal(ch))
		return;

	if (argument[0] == '\0')
	{
		if (IS_SET(ch->comm, COMM_NOWIZ))
		{
			send_to_char("Immortal channel is now ON\n\r", ch);
			REMOVE_BIT(ch->comm, COMM_NOWIZ);
		}
		else
		{
			send_to_char("Immortal channel is now OFF\n\r", ch);
			SET_BIT(ch->comm, COMM_NOWIZ);
		}

		return;
	}

	char buf2[MAX_STRING_LENGTH];
	sprintf(buf2, "%s", argument);

	char arg1[MAX_STRING_LENGTH];
	argument = one_argument(argument, arg1);

	if (arg1[0] == '\0')
	{
		send_to_char("Syntax: immtalk <level?> <message>\n\r", ch);
		return;
	}

	int level = 51;
	if (is_number(arg1))
	{
		level = atoi(arg1);
		if (level > get_trust(ch) || level < 51)
		{
			level = 51;
		}
		else
		{
			sprintf(buf2, "%s", argument);
		}
	}

	if (buf2[0] == '\0')
	{
		send_to_char("Syntax: immtalk <level?> <message>\n\r", ch);
		return;
	}

	auto buffer = level > LEVEL_HERO
		? fmt::format(" [{}]: {}", level, buf2)
		: fmt::format(": {}", buf2); //TODO: change the rest of the sprintf calls to format

	REMOVE_BIT(ch->comm, COMM_NOWIZ);

	if (IS_SET(ch->comm, COMM_ANSI))
	{
		sprintf(buf2, "%s[IMM] $n$t%s", get_char_color(ch, "immtalk"), END_COLOR(ch));
		act_new(buf2, ch, buffer.data(), 0, TO_CHAR, POS_DEAD);
	}
	else
	{
		act_new("[IMM] $n$t", ch, buffer.data(), 0, TO_CHAR, POS_DEAD);
	}

	for (auto wch = char_list; wch != nullptr; wch = wch->next)
	{
		if (is_npc(wch) && wch->desc == nullptr)
			continue;

		if ((is_immortal(wch) || IS_SET(wch->comm, COMM_IMMORTAL) || (is_npc(wch) && (wch->desc != nullptr) && is_immortal(wch->desc->original))) && get_trust(wch) >= std::max(level, 52))
		{
			if (IS_SET(wch->comm, COMM_ANSI))
			{
				sprintf(buf2, "%s[IMM] $n$t%s", get_char_color(wch, "immtalk"), END_COLOR(wch));
				act_new(buf2, ch, buffer.data(), wch, TO_VICT, POS_DEAD);
			}
			else
			{
				act_new("[IMM] $n$t", ch, buffer.data(), wch, TO_VICT, POS_DEAD);
			}
		}
	}
}

/* Used for an object...take this out if you don't want it */
void obj_say_heavenly_sceptre(CHAR_DATA *ch, OBJ_DATA *obj, char *argument)
{
	/*
	int sn;
	sn = -1;

	if (strstr(argument,"i am the wrath of god")
		|| strstr(argument,"I am the wrath of god"))
		sn = skill_lookup("heavenly wrath");

	if (strstr(argument,"feel the force of god")
		|| strstr(argument,"Feel the force of god"))
		sn = skill_lookup("heavenly fire");

	if (sn == -1)
		return;

	(*skill_table[sn].spell_fun) (sn,obj->level,ch,nullptr,TAR_IGNORE);
	return;
	*/
}

const char *upstring(const char *i)
{
	static char buf[MAX_STRING_LENGTH];
	char *o;

	if (i == nullptr)
		return (char *)'\0';

	for (o = buf; *i && (unsigned int)(o - buf) < sizeof(buf) - 1; i++, o++)
	{
		*o = UPPER(*i);
	}

	*o = '\0';

	return buf;
}

const char *lowstring(const char *i)
{
	static char lowbuf[MAX_STRING_LENGTH];
	char *o;

	for (o = lowbuf; *i && (unsigned int)(o - lowbuf) < sizeof(lowbuf) - 1; i++, o++)
	{
		*o = LOWER(*i);
	}

	*o = '\0';
	return lowbuf;
}


///
/// Allows a player or NPC to say things to others in the same room.
/// @param ch: The source of the message
/// @param argument: The message to be sent
///
void do_say(CHAR_DATA *ch, char *argument)
{
	if (argument[0] == '\0')
	{
		send_to_char("Say what?\n\r", ch);
		return;
	}
	check_ooc(ch, argument, "SAY");

#pragma region  Reasons you should not be able to talk
	if (is_affected(ch, gsn_silence))
	{
		send_to_char("You cannot find the words within you.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_uppercut))
	{
		send_to_char("Your jaw is too painful to move.\n\r", ch);
		return;
	}

	if (IS_SET(ch->in_room->room_flags, ROOM_SILENCE))
		return send_to_char("You open your mouth, but no sound comes out.\n\r", ch);

	if (is_shifted(ch) && !is_immortal(ch))
	{
		send_to_char("You can't talk while you're in form.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_mangled))
	{
		send_to_char("Your torn throat prevents you from getting the words out.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_imprisonvoice))
	{
		send_to_char("Your vocal cords are frozen solid!\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_gag))
	{
		act("You are gagged and cannot speak!", ch, nullptr, nullptr, TO_CHAR);
		act("Muffled sounds emanate from $n's direction as $e attempts in vain to speak through $s gag.", ch, nullptr, nullptr, TO_ROOM);
		return;
	}
#pragma  endregion

	if (is_affected(ch, gsn_deafen))
	{
		do_yell(ch, argument);
		return;
	}

	char saymsg[MAX_STRING_LENGTH];
	if (argument[strlen(argument) - 1] == '!')
		sprintf(saymsg, "exclaim");
	else if (argument[strlen(argument) - 1] == '?')
		sprintf(saymsg, "ask");
	else
		sprintf(saymsg, "say");

	auto buffer = fmt::format("You {} '{}$T{}'", saymsg, get_char_color(ch, "speech"), END_COLOR(ch));
	act(buffer.c_str(), ch, nullptr, argument, TO_CHAR);

	for (auto victim = ch->in_room->people; victim != nullptr; victim = victim->next_in_room)
	{
		if (is_awake(victim))
		{
			if (!(is_immortal(ch)) && is_affected(victim, gsn_deafen))
			{
				act("$n says something you can't quite hear.", ch, 0, victim, TO_VICT);
			}
			else
			{
				buffer = fmt::format("$n {}s '{}$t{}'", saymsg, get_char_color(victim, "speech"), END_COLOR(victim));
				act(buffer.c_str(), ch, argument, victim, TO_VICT);

				if (is_affected(victim, gsn_word_of_command) && strstr(argument, victim->pcdata->command[0]))
					command_execute(victim);
			}
		}
	}

	if (IS_SET(ch->in_room->progtypes, RPROG_SPEECH))
		ch->in_room->rprogs->speech_prog(ch->in_room, ch, argument);

	for (auto room_char = ch->in_room->people; room_char != nullptr; room_char = room_char->next_in_room)
	{
		if (IS_SET(room_char->progtypes, MPROG_SPEECH) && room_char != ch)
			room_char->pIndexData->mprogs->speech_prog(room_char, ch, argument);

		if (TRAPS_MEVENT(room_char, TRAP_MSPEECH))
			CALL_MEVENT(room_char, TRAP_MSPEECH, ch, room_char, argument);
	}

	report_cabal_items(ch, argument);

	if (is_affected(ch, gsn_unholy_communion) && (ch->Class()->name == "anti-paladin"))
		check_unholy_communion(ch, argument);

	for (auto char_obj = ch->carrying; char_obj != nullptr; char_obj = char_obj->next_content)
	{
		if (IS_SET(char_obj->progtypes, IPROG_SPEECH) && char_obj->pIndexData->iprogs)
			char_obj->pIndexData->iprogs->speech_prog(char_obj, ch, argument);

		if (TRAPS_IEVENT(char_obj, TRAP_ISPEECH))
			CALL_IEVENT(char_obj, TRAP_ISPEECH, ch, char_obj, argument);
	}

	for (auto char_obj = ch->in_room->contents; char_obj != nullptr; char_obj = char_obj->next_content)
	{
		if (IS_SET(char_obj->progtypes, IPROG_SPEECH) && char_obj->pIndexData->iprogs)
			char_obj->pIndexData->iprogs->speech_prog(char_obj, ch, argument);

		if (TRAPS_IEVENT(char_obj, TRAP_ISPEECH))
			CALL_IEVENT(char_obj, TRAP_ISPEECH, ch, char_obj, argument);
	}
}

void say_to(CHAR_DATA *ch, CHAR_DATA *victim, char *argument, char *extra)
{
	if (argument[0] == '\0')
	{
		send_to_char("Say what?\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_silence))
	{
		send_to_char("You cannot find the words within you.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_uppercut))
	{
		send_to_char("Your jaw is too painful to move.\n\r", ch);
		return;
	}

	if (IS_SET(ch->in_room->room_flags, ROOM_SILENCE))
	{
		send_to_char("You open your mouth, but no sound comes out.\n\r", ch);
		return;
	}

	if (is_shifted(ch) && !is_immortal(ch))
	{
		send_to_char("You can't talk while you're in form.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_mangled))
	{
		send_to_char("Your torn throat prevents you from getting the words out.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_imprisonvoice))
	{
		send_to_char("Your vocal cords are frozen solid!\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_gag))
	{
		act("You are gagged and cannot speak!", ch, nullptr, nullptr, TO_CHAR);
		act("Muffled sounds emanate from $n's direction as $e attempts in vain to speak through $s gag.", ch, nullptr, nullptr, TO_ROOM);
		return;
	}

	char saymsg[MAX_STRING_LENGTH];
	if (argument[strlen(argument) - 1] == '!')
		sprintf(saymsg, "exclaim");
	else if (argument[strlen(argument) - 1] == '?')
		sprintf(saymsg, "ask");
	else
		sprintf(saymsg, "say");

	if (is_awake(victim))
	{
		if (!(is_immortal(ch)) && is_affected(victim, gsn_deafen))
		{
			act("$n says something you can't quite hear.", ch, 0, victim, TO_VICT);
		}
		else
		{
			auto buffer = fmt::format("$n {}s '{}{}{}'", saymsg, get_char_color(victim, "speech"), argument, END_COLOR(victim));
			act(buffer.c_str(), ch, extra, victim, TO_VICT);

			if (is_affected(victim, gsn_word_of_command) && strstr(argument, victim->pcdata->command[0]))
				command_execute(victim);
		}
	}
}

void do_whisper(CHAR_DATA *ch, char *argument) /* whisper -- dioxide */
{
	if (!is_npc(ch) && argument[0] == '\0')
	{
		send_to_char("Whisper what?\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_silence) || IS_SET(ch->in_room->room_flags, ROOM_SILENCE))
	{
		send_to_char("You cannot find the words within you.\n\r", ch);
		return;
	}

	if (is_shifted(ch) && !is_immortal(ch))
	{
		send_to_char("You can't whisper while shapeshifted.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_imprisonvoice))
	{
		send_to_char("Your vocal cords are frozen solid!\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_gag))
	{
		act("You are gagged and cannot speak!", ch, nullptr, nullptr, TO_CHAR);
		act("Muffled sounds emanate from $n's direction as $e attempts in vain to speak through $s gag.", ch, nullptr, nullptr, TO_ROOM);
		return;
	}

	check_ooc(ch, argument, "WHISPER");

	if (is_affected(ch, gsn_deafen))
	{
		do_yell(ch, argument);
		return;
	}

	char buf[MAX_STRING_LENGTH];
	sprintf(buf, "You whisper '%s%s%s'", get_char_color(ch, "red"), argument, END_COLOR(ch));
	act(buf, ch, nullptr, argument, TO_CHAR);

	if (IS_SET(ch->in_room->progtypes, RPROG_SPEECH))
		ch->in_room->rprogs->speech_prog(ch->in_room, ch, argument);

	for (auto victim = ch->in_room->people; victim != nullptr; victim = victim->next_in_room)
	{
		if (is_awake(victim))
		{
			if (!is_immortal(ch) && is_affected(victim, gsn_deafen))
			{
				act("$n says something you can't quite hear.", ch, 0, victim, TO_VICT);
			}
			else
			{
				sprintf(buf, "$n whispers '%s%s%s'", get_char_color(victim, "red"), argument, END_COLOR(victim));
				act(buf, ch, 0, victim, TO_VICT);

				if (IS_SET(victim->progtypes, MPROG_SPEECH) && victim != ch)
					victim->pIndexData->mprogs->speech_prog(victim, ch, argument);

				if (TRAPS_MEVENT(victim, TRAP_MSPEECH))
					CALL_MEVENT(victim, TRAP_MSPEECH, ch, victim, argument);

				if (is_affected(victim, gsn_word_of_command) && strstr(argument, victim->pcdata->command[0]))
					command_execute(victim);
			}
		}
	}

	if (is_affected(ch, gsn_unholy_communion) && (ch->Class()->name == "anti-paladin"))
		check_unholy_communion(ch, argument);
}

void do_sing(CHAR_DATA *ch, char *argument)
{
	if (argument[0] == '\0')
	{
		send_to_char("Sing what?\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_silence) || IS_SET(ch->in_room->room_flags, ROOM_SILENCE))
	{
		send_to_char("You cannot find the words within you.\n\r", ch);
		return;
	}

	if (is_shifted(ch) && !is_immortal(ch))
	{
		send_to_char("You can't sing while shapeshifted.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_imprisonvoice))
	{
		send_to_char("Your vocal cords are frozen solid!\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_uppercut))
	{
		send_to_char("Your jaw is too painful to move.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_gag))
	{
		act("You are gagged and cannot speak!", ch, nullptr, nullptr, TO_CHAR);
		act("Muffled sounds emanate from $n's direction as $e attempts in vain to speak through $s gag.", ch, nullptr, nullptr, TO_ROOM);
		return;
	}

	if (is_affected(ch, gsn_deafen))
	{
		send_to_char("You can't carry a tune when you can't even hear yourself!\n\r", ch);
		return;
	}

	check_ooc(ch, argument, "SING");

	char buf[MAX_STRING_LENGTH], buf2[MAX_STRING_LENGTH];
	for (auto i = 0; i < MAX_STRING_LENGTH; i++)
	{
		buf[i] = '\0';
		buf2[i] = '\0';
	}

	auto length = !is_npc(ch)
					  ? strlen(ch->name)
					  : strlen(ch->short_descr);

	auto copy = argument;
	int i, j, lines = 1;

	for (i = 0; *copy != '\0'; i++)
	{
		if (*copy == '/')
		{
			lines++;
			strcat(buf, "\n\r");

			for (j = 0; j <= length; j++)
			{
				strcat(buf, " ");
			}

			strcat(buf, "       ");
			i += (j + 8);
		}
		else
		{
			buf[i] = *copy;
		}

		copy++;
	}

	if (lines > 9)
	{
		send_to_char("You can't sing all that in one breath!\n\r", ch);
		return;
	}

	WAIT_STATE(ch, lines * 2);

	buf[i] = '\0';

	copy = argument;
	for (i = 0; *copy != '\0'; i++)
	{
		if (*copy == '/')
		{
			lines++;
			strcat(buf2, "\n\r          ");
			i += 11;
		}
		else
		{
			buf2[i] = *copy;
		}

		copy++;
	}

	buf2[i] = '\0';

	std::string buffer3;
	for (auto victim = ch->in_room->people; victim; victim = victim->next_in_room)
	{
		if (is_awake(victim))
		{
			if (!is_immortal(ch) && is_affected(victim, gsn_deafen))
			{
				act("$n sings something you can't quite hear.", ch, 0, victim, TO_VICT);
			}
			else
			{
				buffer3 = fmt::format("$n sings '{}{}{}'", get_char_color(victim, "song"), buf, END_COLOR(victim));
				act(buffer3.c_str(), ch, 0, victim, TO_VICT);

				if (is_affected(victim, gsn_word_of_command) && strstr(argument, victim->pcdata->command[0]))
					command_execute(victim);
			}
		}
	}

	buffer3 = fmt::format("You sing '{}{}{}'", get_char_color(ch, "song"), buf2, END_COLOR(ch));
	act(buffer3.c_str(), ch, 0, 0, TO_CHAR);
}

void do_pray(CHAR_DATA *ch, char *argument)
{
	if (argument[0] == '\0')
	{
		send_to_char("Pray what?\n\r", ch);
		return;
	}

	if (IS_SET(ch->comm, COMM_NOCHANNELS))
	{
		send_to_char("You can't pray.\n\r", ch);
		return;
	}

	WAIT_STATE(ch, 10);

	send_to_char("You pray to the heavens!\n\r", ch);

	char buf[MAX_STRING_LENGTH];

	auto fp = fopen(PRAYER_FILE, "a");
	if (fp)
	{
		sprintf(buf, "%s: %s [%d] is PRAYing for: %s\n",
				log_time(),
				ch->true_name,
				ch->in_room->vnum,
				argument);
		fprintf(fp, "%s", buf);
		fclose(fp);
	}

	for (auto d = descriptor_list; d != nullptr; d = d->next)
	{
		auto victim = d->original ? d->original : d->character;

		if (d->connected == CON_PLAYING && d->character != ch && !IS_SET(victim->comm, COMM_SHOUTSOFF) && !IS_SET(victim->comm, COMM_QUIET) && victim->level >= 52)
		{
			sprintf(buf, "%s%s [%d] is PRAYing for: %s%s\n\r",
					get_char_color(victim, "prays"),
					ch->true_name,
					ch->in_room->vnum,
					argument,
					END_COLOR(victim));
			send_to_char(buf, d->character);
		}
	}
}

void do_tell(CHAR_DATA *ch, char *argument)
{
	if (is_affected(ch, gsn_silence))
	{
		send_to_char("You cannot find the words within you.\n\r", ch);
		return;
	}

	if (IS_SET(ch->comm, COMM_NOTELL) || IS_SET(ch->comm, COMM_DEAF))
	{
		send_to_char("Your message didn't get through.\n\r", ch);
		return;
	}

	if (IS_SET(ch->comm, COMM_QUIET))
	{
		send_to_char("You must turn off quiet mode first.\n\r", ch);
		return;
	}

	if (IS_SET(ch->comm, COMM_DEAF))
	{
		send_to_char("You must turn off deaf mode first.\n\r", ch);
		return;
	}

	if (is_affected_area(ch->in_room->area, gsn_interference))
	{
		send_to_char("The electrical currents in the area block your attempt to communicate.\n\r", ch);
		return;
	}

	check_ooc(ch, argument, "TELL");

	char arg[MAX_INPUT_LENGTH];
	argument = one_argument(argument, arg);

	if (arg[0] == '\0' || argument[0] == '\0')
	{
		send_to_char("Tell whom what?\n\r", ch);
		return;
	}

	/* This added for decoy tells..without this you have to locate the number
	of the person you are trying to talk to when their decoys occupy 1.<name>,
	and that's mega annoying...(Ceran)
	*/
	char arg2[MAX_STRING_LENGTH];
	arg2[0] = '\0';
	sprintf(arg2, "%s", arg);

	/*
	 * Can tell to PC's anywhere, but NPC's only in same room.
	 * -- Furey
	 */

	auto number = number_argument(arg2, arg);
	auto count = 0;
	auto victim = char_list;
	for (; victim != nullptr; victim = victim->next)
	{
		if (is_name(victim->name, arg) && (is_npc(victim) && victim->pIndexData->vnum == MOB_VNUM_DECOY))
			continue;

		if (victim->in_room == nullptr || (is_immortal(ch) ? !is_name(arg, (victim->true_name ? victim->true_name : victim->name)) : !is_name(arg, victim->name)) || !can_see(ch, victim))
			continue;

		if (++count == number)
			break;
	}

	if (victim == nullptr || (is_npc(victim) && victim->in_room != ch->in_room))
	{
		send_to_char("They aren't here.\n\r", ch);
		return;
	}

	if (IS_SET(ch->in_room->room_flags, ROOM_SILENCE) || IS_SET(victim->in_room->room_flags, ROOM_SILENCE))
	{
		send_to_char("Your message didn't get through.\n\r", ch);
		return;
	}

	char buf[MAX_STRING_LENGTH];
	if (victim->desc == nullptr && !is_npc(victim))
	{
		act("$N seems to have lost consciousness...try again later.", ch, nullptr, victim, TO_CHAR);
		sprintf(buf, "%s tells you '%s'\n\r", pers(ch, victim), argument);
		buf[0] = UPPER(buf[0]);
		add_buf(victim->pcdata->buffer, buf);
		return;
	}

	if (!(is_immortal(ch) && ch->level > LEVEL_IMMORTAL) && (!is_awake(victim) || is_affected(victim, gsn_deafen)))
	{
		act("$E can't hear you.", ch, 0, victim, TO_CHAR);
		return;
	}

	if ((IS_SET(victim->comm, COMM_QUIET) || IS_SET(victim->comm, COMM_DEAF)) && !is_immortal(ch)) /* Let Imms send tells to deaf players */
	{
		act("$E is not receiving tells.", ch, 0, victim, TO_CHAR);
		return;
	}

	auto deaf = false;
	if (is_affected(ch, gsn_deafen))
	{
		deaf = true;
		argument = palloc_string(upstring(argument));
	}

	sprintf(buf, "You tell $%s '%s$t%s'",
			is_immortal(ch) && !is_npc(victim) ? "F" : "N",
			get_char_color(ch, "tells"),
			END_COLOR(ch));

	act(buf, ch, argument, victim, TO_CHAR);

	sprintf(buf, "$%s tells you '%s$t%s'",
			is_immortal(victim) && !is_npc(ch) ? "f" : "n",
			get_char_color(victim, "tells"),
			END_COLOR(victim));

	act_new(buf, ch, argument, victim, TO_VICT, POS_DEAD);

	if (IS_SET(victim->progtypes, MPROG_SPEECH) && victim != ch)
		victim->pIndexData->mprogs->speech_prog(victim, ch, argument);

	if (is_affected(victim, gsn_word_of_command) && strstr(argument, victim->pcdata->command[0]))
		command_execute(victim);

	victim->reply = ch;

	if (deaf)
		free_pstring(argument);
}

void do_noreply(CHAR_DATA *ch, char *argument)
{
	send_to_char("You concentrate and momentarily close your ears to the replies of others.\n\r", ch);

	for (auto vch = char_list; vch; vch = vch->next)
	{
		if (!is_npc(vch) && vch->reply == ch)
			vch->reply = nullptr;
	}
}

void do_reply(CHAR_DATA *ch, char *argument)
{
	if (IS_SET(ch->comm, COMM_NOTELL))
	{
		send_to_char("Your message didn't get through.\n\r", ch);
		return;
	}

	auto victim = ch->reply;
	if (victim == nullptr)
	{
		send_to_char("They aren't here.\n\r", ch);
		return;
	}

	char buf[MAX_STRING_LENGTH];
	if (victim->desc == nullptr && !is_npc(victim) && !is_switched(victim) && victim->pcdata)
	{
		act("$N seems to have lost consciousness...try again later.", ch, nullptr, victim, TO_CHAR);
		sprintf(buf, "%s tells you '%s'\n\r", pers(ch, victim), argument);
		buf[0] = UPPER(buf[0]);
		add_buf(victim->pcdata->buffer, buf);
		return;
	}

	if (!is_immortal(ch) && (!is_awake(victim) || is_affected(victim, gsn_deafen)))
	{
		act("$E can't hear you.", ch, 0, victim, TO_CHAR);
		return;
	}

	if ((IS_SET(victim->comm, COMM_QUIET) || IS_SET(victim->comm, COMM_DEAF)) && !is_immortal(ch) && !is_immortal(victim))
	{
		act_new("$E is not receiving tells.", ch, 0, victim, TO_CHAR, POS_DEAD);
		return;
	}

	if (!is_immortal(victim) && !is_awake(ch))
	{
		send_to_char("In your dreams, or what?\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_imprisonvoice))
	{
		send_to_char("Your vocal cords are frozen solid!\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_gag))
	{
		send_to_char("You are gagged and cannot speak!\n\r", ch);
		return;
	}

	if (!is_immortal(victim))
		check_ooc(ch, argument, "TELL");

	auto deaf = false;
	if (is_affected(ch, gsn_deafen))
	{
		deaf = true;
		argument = palloc_string(upstring(argument));
	}

	sprintf(buf, "You tell $%s '%s$t%s'",
			is_immortal(ch) && !is_npc(victim) ? "F" : "N",
			get_char_color(ch, "tells"),
			END_COLOR(ch));

	act(buf, ch, argument, victim, TO_CHAR);

	sprintf(buf, "$%s tells you '%s$t%s'",
			is_immortal(victim) && !is_npc(ch) ? "f" : "n",
			get_char_color(victim, "tells"),
			END_COLOR(victim));

	act_new(buf, ch, argument, victim, TO_VICT, POS_DEAD);

	if (IS_SET(victim->progtypes, MPROG_SPEECH) && victim != ch)
		victim->pIndexData->mprogs->speech_prog(victim, ch, argument);

	victim->reply = ch;

	if (deaf)
		free_pstring(argument);
}

void do_yell(CHAR_DATA *ch, char *argument)
{
	if (is_affected(ch, gsn_silence))
	{
		send_to_char("You cannot find the words within you.\n\r", ch);
		return;
	}

	if (IS_SET(ch->comm, COMM_NOSHOUT) || (is_shifted(ch) && !is_immortal(ch)))
	{
		send_to_char("You can't yell.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_mangled))
	{
		send_to_char("Your torn throat prevents you from getting the words out.\n\r", ch);
		return;
	}

	if (argument[0] == '\0')
	{
		send_to_char("Yell what?\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_imprisonvoice))
	{
		send_to_char("Your vocal cords are frozen solid!\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_gag))
	{
		act("You are gagged and cannot speak!", ch, nullptr, nullptr, TO_CHAR);
		act("Muffled sounds emanate from $n's direction as $e attempts in vain to speak through $s gag.", ch, nullptr, nullptr, TO_ROOM);
		return;
	}

	if (is_affected(ch, gsn_uppercut))
	{
		send_to_char("Your jaw hurts too much to move.\n\r", ch);
		return;
	}

	auto deaf = false;
	if (is_affected(ch, gsn_deafen))
	{
		deaf = true;
		argument = palloc_string(upstring(argument));
	}

	check_ooc(ch, argument, "YELL");

	if (!str_cmp(ch->name, "Karsus") && strstr(argument, "eing") && !strstr(argument, "being"))
	{
		send_to_char("***It's 'ING', not 'EING' you fooker!!!***\n\r", ch);
		send_to_char("***It's 'ING', not 'EING' you fooker!!!***\n\r", ch);
		send_to_char("***It's 'ING', not 'EING' you fooker!!!***\n\r", ch);
	}

	char buf[MAX_STRING_LENGTH];
	if (IS_SET(ch->comm, COMM_ANSI))
	{
		sprintf(buf, "You yell '%s$t%s'", get_char_color(ch, "yells"), END_COLOR(ch));
		act(buf, ch, argument, nullptr, TO_CHAR);
	}
	else
	{
		act("You yell '$t'", ch, argument, nullptr, TO_CHAR);
	}

	for (auto d = descriptor_list; d != nullptr; d = d->next)
	{
		if (d->connected == CON_PLAYING && d->character != ch && d->character->in_room != nullptr && d->character->in_room->area == ch->in_room->area && !IS_SET(d->character->comm, COMM_QUIET))
		{
			if (IS_SET(d->character->in_room->room_flags, ROOM_SILENCE))
				continue;

			/* Can't hear yells while asleep, MORGLUM */
			if (!is_awake(d->character))
				continue;

			sprintf(buf, "$n yells '%s$t%s'", get_char_color(d->character, "yells"), END_COLOR(d->character));
			act_new(buf, ch, argument, d->character, TO_VICT, POS_SLEEPING);

			if (is_affected(d->character, gsn_word_of_command) && strstr(argument, d->character->pcdata->command[0]))
				command_execute(d->character);
		}
	}

	if (deaf)
		free_pstring(argument);
}

void do_myell(CHAR_DATA *ch, char *argument, CHAR_DATA *attacker)
{
	/*
	if(is_shifted(ch))
	{
		for (d = descriptor_list; d; d = d->next)
		{
			if (d->connected == CON_PLAYING
				&&  d->character->in_room != nullptr && ch->in_room != nullptr
				&&  d->character->in_room->area == ch->in_room->area
				&&  d->character != ch
				&& !IS_SET(d->character->in_room->room_flags, ROOM_SILENCE))
			{
					send_to_char(form_table[ch->pcdata->shifted].yell,d->character);
					send_to_char("\n\r",d->character);
			}
		}

		send_to_char("You make some noise as you are
		attacked.\n\r",ch); return;
	}
	*/

	if (ch->invis_level >= LEVEL_HERO)
		return;

	if (ch && ch->in_room && attacker && can_see(ch, attacker))
	{
		if (IS_SET(ch->in_room->area->progtypes, APROG_MYELL))
			ch->in_room->area->aprogs->myell_prog(ch->in_room->area, attacker, ch);
	}

	do_yell(ch, argument);
}

void do_emote(CHAR_DATA *ch, char *argument)
{
	if (!is_npc(ch) && IS_SET(ch->comm, COMM_NOEMOTE))
	{
		send_to_char("You can't show your emotions.\n\r", ch);
		return;
	}

	if (argument[0] == '\0')
	{
		send_to_char("Emote what?\n\r", ch);
		return;
	}

	check_ooc(ch, argument, "EMOT");

	char buffer[MAX_STRING_LENGTH * 2];
	buffer[0] = '\0';
	if (argument[0] != '\'' || UPPER(argument[1]) != 'S')
		strcpy(buffer, " ");

	strcat(buffer, argument);

	act("$n$T", ch, nullptr, buffer, TO_ALL);
}

void do_pmote(CHAR_DATA *ch, char *argument)
{
	if (!is_npc(ch) && IS_SET(ch->comm, COMM_NOEMOTE))
	{
		send_to_char("You can't show your emotions.\n\r", ch);
		return;
	}

	if (argument[0] == '\0')
	{
		send_to_char("Emote what?\n\r", ch);
		return;
	}

	check_ooc(ch, argument, "PMOT");

	char temp[MAX_STRING_LENGTH], tbuf[MAX_STRING_LENGTH];
	sprintf(temp, "%s", argument);
	tbuf[0] = '\0';

	if (temp[0] != '\'' && UPPER(temp[1]) != 'S')
		strcpy(tbuf, " ");

	strcat(tbuf, temp);
	argument = palloc_string(tbuf);

	act("$n$t", ch, argument, nullptr, TO_CHAR);

	char *letter, *name;
	unsigned int matches = 0;
	char last[MAX_INPUT_LENGTH];
	for (auto vch = ch->in_room->people; vch != nullptr; vch = vch->next_in_room)
	{
		if (vch->desc == nullptr || vch == ch)
			continue;

		letter = strstr(argument, vch->name);
		if (letter == nullptr)
		{
			act("$N$t", vch, argument, ch, TO_CHAR);
			continue;
		}

		strcpy(temp, argument);
		temp[strlen(argument) - strlen(letter)] = '\0';
		last[0] = '\0';
		name = vch->name;

		for (; *letter != '\0'; letter++)
		{
			if (*letter == '\'' && matches == strlen(vch->name))
			{
				strcat(temp, "r");
				continue;
			}

			if (*letter == 's' && matches == strlen(vch->name))
			{
				matches = 0;
				continue;
			}

			if (matches == strlen(vch->name))
				matches = 0;

			if (*letter == *name)
			{
				matches++;
				name++;

				if (matches == strlen(vch->name))
				{
					strcat(temp, "you");
					last[0] = '\0';
					name = vch->name;
					continue;
				}

				strncat(last, letter, 1);
				continue;
			}

			matches = 0;
			strcat(temp, last);
			strncat(temp, letter, 1);
			last[0] = '\0';
			name = vch->name;
		}

		act("$N$t", vch, temp, ch, TO_CHAR);
	}

	free_pstring(argument);
}

/* Simple, yet brilliant.  Notify immortals when players are using words that
 * are offensive/harassing or commonly associated with OOC speech.  Immortals
 * can then respond accordingly if a rule violation is detected.
 * 											-El
 */
bool check_ooc(CHAR_DATA *ch, char *argument, char *type)
{
	/* Imms can curse and be OOC all we want! */
	if (is_immortal(ch))
		return true;

	if (is_npc(ch) && !is_affected_by(ch, AFF_CHARM))
		return true;

	/* The list of naughty words.  Add more as needed. */

	if (strstr(argument, "pk") || strstr(argument, "PK") || strstr(argument, "Pk") || strstr(argument, "ooc") ||
		strstr(argument, "OOC") || strstr(argument, "Ooc") || strstr(argument, "exp ") || strstr(argument, "exp.") ||
		strstr(argument, " xp") || strstr(argument, " XP") || strstr(argument, "tnl") || strstr(argument, "hp") ||
		strstr(argument, "HP") || strstr(argument, " u ") || strstr(argument, " U ") || strstr(argument, "0r") ||
		strstr(argument, "x0") || strstr(argument, " 2 ") || strstr(argument, "grp") || strstr(argument, "afk") ||
		strstr(argument, "AFK") || strstr(argument, "computer") || strstr(argument, "quit out") ||
		strstr(argument, "drop link") || strstr(argument, "lost link") || strstr(argument, "lose link") ||
		strstr(argument, "discon") || strstr(argument, "relog") || strstr(argument, "logoff") ||
		strstr(argument, "log off") || strstr(argument, " lag") || strstr(argument, " Lag") ||
		strstr(argument, "character") || strstr(argument, "chars") || strstr(argument, "player") ||
		strstr(argument, "game") || strstr(argument, "MUD") || strstr(argument, "mud") || strstr(argument, "RL") ||
		strstr(argument, "RS") || strstr(argument, "Riftshadow") || strstr(argument, "play") ||
		strstr(argument, "CF") || strstr(argument, "cf") || strstr(argument, "Carrion") || strstr(argument, "fuck") ||
		strstr(argument, "FUCK") || strstr(argument, "Fuck") || strstr(argument, "fuk") || strstr(argument, "pussy") ||
		strstr(argument, "cunt") || strstr(argument, "licker") || strstr(argument, "dick") || strstr(argument, "fag") ||
		strstr(argument, "FAG") || strstr(argument, "nigg") || strstr(argument, "gook") || strstr(argument, "Gook") ||
		strstr(argument, "jew") || strstr(argument, "Jew") || strstr(argument, "kike") || strstr(argument, " spic") ||
		strstr(argument, "newbie"))
	{
		char buf[MAX_STRING_LENGTH];
		sprintf(buf, "%s is %sING: ", ch->name, type);
		strcat(buf, argument);
		wiznet(buf, nullptr, nullptr, WIZ_OOC, 0, 0);
	}

	return false;
}

void do_bug(CHAR_DATA *ch, char *argument)
{
	if (is_immortal(ch))
	{
		do_buglist(ch, argument);
		return;
	}

	append_file(ch, BUG_FILE, argument);
	send_to_char("Bug logged.\n\r", ch);
}

void do_typo(CHAR_DATA *ch, char *argument)
{
	append_file(ch, TYPO_FILE, argument);
	send_to_char("Typo logged.\n\r", ch);
}

void do_idea(CHAR_DATA *ch, char *argument)
{
	append_file(ch, IDEAS_FILE, argument);
	send_to_char("Idea logged.  Thanks!\n\r", ch);
}

void do_rent(CHAR_DATA *ch, char *argument)
{
	send_to_char("There is no rent here.  Just save and quit.\n\r", ch);
}

void do_qui(CHAR_DATA *ch, char *argument)
{
	send_to_char("If you want to QUIT, you have to spell it out.\n\r", ch);
}

void do_quit(CHAR_DATA *ch, char *argument)
{
	do_quit_new(ch, argument, false);
}

void do_quit_new(CHAR_DATA *ch, char *argument, bool autoq)
{
	if (is_npc(ch))
		return;

	if (!autoq)
	{
		if (is_affected_by(ch, AFF_CHARM))
		{
			send_to_char("Now why would you want to leave your master?\n\r", ch);
			return;
		}

		if (ch->position == POS_FIGHTING)
		{
			send_to_char("No way! You are fighting.\n\r", ch);
			return;
		}

		if (ch->position < POS_STUNNED)
		{
			send_to_char("You're not DEAD yet.\n\r", ch);
			return;
		}

		if (ch->pause > 0 && !is_immortal(ch))
		{
			send_to_char("Your adrenaline is gushing too much to quit!\n\r", ch);
			return;
		}

		if (is_affected(ch, gsn_blackjack) || is_affected_by(ch, AFF_SLEEP))
		{
			send_to_char("You can't quit!\n\r", ch);
			return;
		}
	}

	if (is_affected(ch, skill_lookup("diamond skin")))
		affect_strip(ch, skill_lookup("diamond skin"));

	if (is_affected(ch, skill_lookup("channel")))
		affect_strip(ch, skill_lookup("channel"));

	if (is_affected(ch, skill_lookup("fortitude")))
		affect_strip(ch, skill_lookup("fortitude"));

	if (is_affected(ch, gsn_cloak_form))
		do_cloak(ch, "auto");

	if (!autoq)
	{
		if (is_affected_by(ch, AFF_CHARM))
		{
			send_to_char("You don't want to leave your master yet!\n\r", ch);
			return;
		}

		if (!IS_SET(ch->in_room->room_flags, ROOM_LAW) && ch->in_room->cabal == 0 && !is_immortal(ch))
		{
			send_to_char("You don't feel safe enough to quit here.\n\r", ch);
			return;
		}

		if (ch->in_room->cabal != 0 && !is_immortal(ch) && ch->in_room->cabal != ch->cabal)
		{
			send_to_char("You can't quit out in an enemy Cabal!\n\r", ch);
			return;
		}

		for (auto obj = object_list; obj != nullptr; obj = obj->next)
		{
			if (isCabalItem(obj))
			{
				if (obj->carried_by != nullptr && obj->carried_by == ch)
				{
					act("You cannot quit with cabal items in your inventory!", ch, 0, 0, TO_CHAR);
					return;
				}
			}
		}
	}

	if (CQueue::HasQueuePending(ch))
	{
		if (autoq)
			RS.Bug("Trying to autoquit char %s with pending queue.", ch->name);

		send_to_char("You cannot quit while events requiring your presence are pending.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_disguise))
		do_undisguise(ch, "");

	if (is_affected(ch, gsn_empathy))
		affect_strip(ch, gsn_empathy);

	if (ch->cabal != 0 && ch->pcdata->host)
		announce_logout(ch);

	send_to_char("Alas, all good things must come to an end.\n\r", ch);
	act("$n has left the realm.", ch, nullptr, nullptr, TO_ROOM);

	sprintf(log_buf, "%s@%s has %squit. [%ld played, %d (%d) obj]",
			ch->true_name,
			!IS_SET(ch->comm, COMM_NOSOCKET) ? ch->pcdata->host : "",
			autoq ? "auto" : "",
			((current_time - ch->logon) / 60),
			count_carried(ch, false),
			count_carried(ch, true));

	RS.Log(log_buf);
	wiznet(log_buf, ch, nullptr, WIZ_SITES, 0, get_trust(ch));
	login_log(ch, 2);

	/*
	 * After extract_char the ch is no longer valid!
	 */

	/* Remove any Cabal items first , also owners
		and any limiteds on levels 10 and below
	*/
	OBJ_DATA *obj_next;
	for (auto obj = object_list; obj != nullptr; obj = obj_next)
	{
		obj_next = obj->next;
		if (isCabalItem(obj))
		{
			if (obj->carried_by != nullptr && obj->carried_by == ch)
				extract_obj(obj);
		}
		else if (obj->pIndexData->limtotal != 0 && ch->level < 10)
		{
			if (obj->carried_by == ch)
				extract_obj(obj);
		}
	}

	/* extract all decoys */
	CHAR_DATA *wch_next;
	for (auto wch = char_list; wch != nullptr; wch = wch_next)
	{
		wch_next = wch->next;

		if (wch->defending != nullptr && wch->defending == ch)
			wch->defending = nullptr;

		if (is_affected(wch, gsn_empathy))
		{
			auto laf = wch->affected;
			for (; laf != nullptr; laf = laf->next)
			{
				if (laf->type == gsn_empathy)
					break;
			}

			if (laf->owner == ch)
				affect_strip(wch, gsn_empathy);
		}

		if (!is_npc(wch))
			continue;

		if (is_npc(wch) && is_affected_by(wch, AFF_CHARM) && wch->master == ch && IS_SET(wch->act, ACT_UNDEAD))
		{
			extract_char(wch, true);
			continue;
		}

		if (wch->pIndexData->vnum == MOB_VNUM_DECOY && is_name(ch->name, wch->name))
		{
			act("$n crumbles to dust.", wch, 0, 0, TO_ROOM);
			extract_char(wch, true);
		}
	}

	save_char_obj(ch);
	auto id = ch->id;
	auto d = ch->desc;

	extract_char(ch, true);

	if (d != nullptr)
		close_socket(d);

	/* toast evil cheating bastards */
	for (d = descriptor_list; d != nullptr; d = d->next)
	{
		auto tch = d->original ? d->original : d->character;
		if (tch && tch->id == id)
		{
			extract_char(tch, true);
			close_socket(d);
		}
	}
}

void do_save(CHAR_DATA *ch, char *argument)
{
	send_to_char("Saving. Remember that Riftshadow has automatic saving.\n\r", ch);

	if (is_npc(ch))
		return;

	save_char_obj(ch);
	ch->pcdata->save_timer = 2;
}

void do_follow(CHAR_DATA *ch, char *argument)
{
	/* RT changed to allow unlimited following and follow the NOFOLLOW rules */
	char arg[MAX_INPUT_LENGTH];
	one_argument(argument, arg);

	if (arg[0] == '\0')
	{
		send_to_char("Follow whom?\n\r", ch);
		return;
	}

	auto victim = get_char_room(ch, arg);
	if (victim == nullptr)
	{
		send_to_char("They aren't here.\n\r", ch);
		return;
	}

	if (is_affected_by(ch, AFF_CHARM) && ch->master != nullptr)
	{
		act("But you'd rather follow $N!", ch, nullptr, ch->master, TO_CHAR);
		return;
	}

	if (victim == ch)
	{
		if (ch->master == nullptr)
		{
			send_to_char("You already follow yourself.\n\r", ch);
			return;
		}

		stop_follower(ch);
		return;
	}

	if (!is_npc(victim) && IS_SET(victim->act, PLR_NOFOLLOW) && (get_skill(ch, gsn_trail) == 0 || can_see(victim, ch)))
	{
		if (get_skill(ch, gsn_trail) > 0 && can_see(victim, ch))
			act("You can't very well trail someone if they can see you.", ch, nullptr, nullptr, TO_CHAR);

		act("$N doesn't seem to want any followers.\n\r", ch, nullptr, victim, TO_CHAR);
		return;
	}

	if (is_affected(victim, gsn_sphere_of_plasma))
	{
		send_to_char("They are encased in a sphere of plasma!\n\r", ch);
		return;
	}
	if (is_affected(ch, gsn_sphere_of_plasma))
	{
		send_to_char("You are encased in a sphere of plasma!\n\r", ch);
		return;
	}

	REMOVE_BIT(ch->act, PLR_NOFOLLOW);

	if (ch->master != nullptr)
		stop_follower(ch);

	add_follower(ch, victim);
}

void add_follower(CHAR_DATA *ch, CHAR_DATA *master)
{
	if (ch->master != nullptr)
	{
		RS.Bug("Add_follower: non-nullptr master.");
		return;
	}

	ch->master = master;
	ch->leader = nullptr;

	auto chance = get_skill(ch, gsn_trail);
	if (chance > 0 && IS_SET(master->act, PLR_NOFOLLOW))
	{
		if (number_percent() < chance)
		{
			act("You attempt to secretly trail $N.", ch, nullptr, master, TO_CHAR);

			AFFECT_DATA af;
			init_affect(&af);
			af.where = TO_AFFECTS;
			af.type = gsn_trail;
			af.level = ch->level;
			af.aftype = AFT_INVIS;
			af.duration = -1;
			affect_to_char(ch, &af);
			check_improve(ch, gsn_trail, true, 1);
		}
		else
		{
			act("You attempt to secretly trail $N.", ch, nullptr, master, TO_CHAR);
			act("You have a sudden feeling someone is following you.", ch, nullptr, master, TO_VICT);
			check_improve(ch, gsn_trail, false, 1);
		}
	}
	else
	{
		if (can_see(master, ch))
			act("$n now follows you.", ch, nullptr, master, TO_VICT);

		act("You now follow $N.", ch, nullptr, master, TO_CHAR);
	}
}

void stop_follower(CHAR_DATA *ch)
{
	if (ch->master == nullptr)
	{
		RS.Bug("Stop_follower: nullptr master.");
		return;
	}

	if (is_affected_by(ch, AFF_CHARM))
	{
		REMOVE_BIT(ch->affected_by, AFF_CHARM);
		affect_strip(ch, gsn_charm_person);
	}

	if (can_see(ch->master, ch) && ch->in_room != nullptr && !(is_npc(ch) && ch->pIndexData->vnum == MOB_VNUM_ANCHOR))
	{
		act("$n stops following you.", ch, nullptr, ch->master, TO_VICT);
		act("You stop following $N.", ch, nullptr, ch->master, TO_CHAR);

		if (is_affected(ch, gsn_trail))
			affect_strip(ch, gsn_trail);
	}
	if (ch->master->pet == ch)
		ch->master->pet = nullptr;

	check_leadership_affect(ch);
	ch->master = nullptr;
	ch->leader = nullptr;
}

/* nukes charmed monsters and pets */
void nuke_pets(CHAR_DATA *ch)
{
	auto pet = ch->pet;
	if (pet != nullptr)
	{
		stop_follower(pet);
		if (pet->in_room != nullptr)
			act("$N slowly fades away.", ch, nullptr, pet, TO_NOTVICT);

		extract_char(pet, true);
	}

	ch->pet = nullptr;
}

void die_follower(CHAR_DATA *ch)
{
	if (!ch)
	{
		RS.Bug("Error: Die follower, invalid master.");
		return;
	}

	if (ch->master != nullptr)
	{
		if (ch->master->pet == ch)
			ch->master->pet = nullptr;

		stop_follower(ch);
	}

	ch->leader = nullptr;

	CHAR_DATA *fch_next;
	for (auto fch = char_list; fch != nullptr; fch = fch_next)
	{
		fch_next = fch->next;
		/*if(!fch->in_room && is_npc(fch))
		{
			RS.Bug("Error: Mob %d in room is nullptr!",fch->pIndexData->vnum);
			break;
		}*/
		if (is_npc(fch) && (is_affected(fch, gsn_animate_dead) || is_affected_by(fch, AFF_CHARM)))
		{
			if (fch->master == ch)
			{
				REMOVE_BIT(fch->affected_by, AFF_CHARM);
				affect_strip(fch, gsn_animate_dead);
				act("$n slowly fades away.", fch, nullptr, nullptr, TO_ROOM);
				extract_char(fch, true);
			}
		}
		else
		{
			if (fch->master == ch && !is_affected(fch, gsn_trail))
				stop_follower(fch);

			if (fch->leader == ch && !is_affected(fch, gsn_trail))
				fch->leader = fch;
		}
	}
}

void do_order(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH], arg2[MAX_INPUT_LENGTH];
	argument = one_argument(argument, arg);
	one_argument(argument, arg2);

	if (arg[0] == '\0' || argument[0] == '\0')
	{
		send_to_char("Order whom to do what?\n\r", ch);
		return;
	}

	if (is_affected_by(ch, AFF_CHARM))
	{
		send_to_char("You feel like taking, not giving, orders.\n\r", ch);
		return;
	}

	CHAR_DATA *victim = nullptr;
	auto fAll = !str_cmp(arg, "all");
	if (!fAll)
	{
		victim = get_char_room(ch, arg);
		if (victim == nullptr)
		{
			send_to_char("They aren't here.\n\r", ch);
			return;
		}

		if (victim == ch)
		{
			send_to_char("Aye aye, right away!\n\r", ch);
			return;
		}

		if (!is_affected_by(victim, AFF_CHARM) || victim->master != ch || (is_immortal(victim) && victim->trust >= ch->trust))
		{
			send_to_char("Do it yourself!\n\r", ch);
			return;
		}
	}

	auto cOK = false;
	for (auto i = 0; i < MAX_ORDERS; i++)
	{
		if (!str_prefix(arg2, order_table[i].command))
			cOK = true;
	}

	if (!cOK)
	{
		send_to_char("You can't order them to do that!\n\r", ch);
		return;
	}

	auto found = false;

	CHAR_DATA *och_next;
	for (auto och = ch->in_room->people; och != nullptr; och = och_next)
	{
		och_next = och->next_in_room;

		if (is_affected_by(och, AFF_CHARM) && och->master == ch && (fAll || och == victim))
		{
			if (is_npc(och) && och->pIndexData->vnum == ACADEMY_PET)
				continue;

			found = true;

			char buf[MAX_STRING_LENGTH];
			sprintf(buf, "$n orders you to '%s'.", argument);
			act(buf, ch, nullptr, och, TO_VICT);
			interpret(och, argument);

			if (!str_prefix(arg2, "disengage"))
				break;
		}
	}

	if (found)
	{
		if (str_prefix(arg2, "say") && str_prefix(arg2, "emote") && str_prefix(arg2, "pmote"))
			WAIT_STATE(ch, 2 * PULSE_VIOLENCE);

		send_to_char("Ok.\n\r", ch);
	}
	else
	{
		send_to_char("You have no followers here.\n\r", ch);
	}
}

void do_group(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	one_argument(argument, arg);

	if (arg[0] == '\0')
	{
		std::string buffer;
		char buf2[MAX_STRING_LENGTH];
		auto leader = ch->leader != nullptr ? ch->leader : ch;

		buffer = fmt::format("{}'s group:\n\r", pers(leader, ch));
		send_to_char(buffer.c_str(), ch);

		for (auto gch = char_list; gch != nullptr; gch = gch->next)
		{
			if (is_same_group(gch, ch))
			{
				if (!is_npc(gch))
				{
					auto exp_tnl = (gch->level * exp_per_level(gch) - gch->exp);
					auto epl = pc_race_table[gch->race].xpadd + 1500;
					epl += (int)((gch->level - 2) * epl * 0.08);
					auto exp_ltl = (gch->level * exp_per_level(gch) - (gch->level - 1) * epl);
					buffer = fmt::sprintf("[%2d %s] %-32s %3d%%hp %3d%%mana %3d%%mv %3d%%tnl\n\r",
							gch->level,
							is_npc(gch) ? "Mob" : gch->Class()->who_name,
							capitalize(pers(gch, ch)),
							(gch->max_hit == 0) ? 0 : (gch->hit * 100) / gch->max_hit,
							(gch->max_mana == 0) ? 0 : (gch->mana * 100) / gch->max_mana,
							(gch->max_move == 0) ? 0 : (gch->move * 100) / gch->max_move,
							URANGE(0, (exp_tnl * 100) / exp_ltl, 100));
				}
				else
				{
					sprintf(buf2, "%s", pers(gch, ch));
					if (buf2[0] != '\0')
						buf2[0] = UPPER(buf2[0]);

					buffer = fmt::sprintf("[Minion] %-32s %3d%%hp %3d%%mana %3d%%mv\n\r",
							buf2,
							(gch->max_hit == 0) ? 0 : (gch->hit * 100) / gch->max_hit,
							(gch->max_mana == 0) ? 0 : (gch->mana * 100) / gch->max_mana,
							(gch->max_move == 0) ? 0 : (gch->move * 100) / gch->max_move);
				}

				send_to_char(buffer.c_str(), ch);
			}
		}

		return;
	}

	auto victim = get_char_room(ch, arg);
	if (victim == nullptr)
	{
		send_to_char("They aren't here.\n\r", ch);
		return;
	}

	if (ch->master != nullptr || (ch->leader != nullptr && ch->leader != ch))
	{
		send_to_char("But you are following someone else!\n\r", ch);
		return;
	}

	if (victim->master != ch && ch != victim)
	{
		act_new("$N isn't following you.", ch, nullptr, victim, TO_CHAR, POS_SLEEPING);
		return;
	}

	if (is_affected_by(victim, AFF_CHARM))
	{
		send_to_char("You can't remove charmed followers from your group.\n\r", ch);
		return;
	}

	if (is_affected_by(ch, AFF_CHARM))
	{
		act_new("You like your master too much to leave $m!", ch, nullptr, victim, TO_VICT, POS_SLEEPING);
		return;
	}

	if (is_same_group(victim, ch) && ch != victim)
	{
		victim->leader = nullptr;
		act_new("$n removes $N from $s group.", ch, nullptr, victim, TO_NOTVICT, POS_RESTING);
		act_new("$n removes you from $s group.", ch, nullptr, victim, TO_VICT, POS_SLEEPING);
		act_new("You remove $N from your group.", ch, nullptr, victim, TO_CHAR, POS_SLEEPING);
		check_leadership_affect(victim);
		return;
	}

	victim->leader = ch;
	act_new("$N joins $n's group.", ch, nullptr, victim, TO_NOTVICT, POS_RESTING);
	act_new("You join $n's group.", ch, nullptr, victim, TO_VICT, POS_SLEEPING);
	act_new("$N joins your group.", ch, nullptr, victim, TO_CHAR, POS_SLEEPING);

	AFFECT_DATA af;
	if (!is_npc(ch) && !is_npc(victim) && ch->Class()->name == "anti-paladin" && ch->pcdata->learned[gsn_traitors_luck] > 1)
	{
		init_affect(&af);
		af.where = TO_AFFECTS;
		af.aftype = AFT_INVIS;
		af.type = gsn_traitors_luck;
		af.owner = ch;
		af.level = ch->level;
		af.duration = -1;
		SET_BIT(af.bitvector, AFF_PERMANENT);
		af.modifier = 0;
		af.location = 0;
		af.pulse_fun = traitor_pulse;
		new_affect_to_char(ch, &af);
	}

	if (!is_npc(ch) && !is_npc(victim) && victim->Class()->name == "anti-paladin" && victim->pcdata->learned[gsn_traitors_luck] > 1)
	{
		init_affect(&af);
		af.where = TO_AFFECTS;
		af.aftype = AFT_INVIS;
		af.type = gsn_traitors_luck;
		af.owner = victim;
		af.level = ch->level;
		af.duration = -1;
		SET_BIT(af.bitvector, AFF_PERMANENT);
		af.modifier = 0;
		af.location = 0;
		af.pulse_fun = traitor_pulse;

		for (auto vch = char_list; vch; vch = vch->next)
		{
			if (vch != ch && is_same_group(vch, ch))
				new_affect_to_char(vch, &af);
		}
	}
}

/*
 * 'Split' originally by Gnort, God of Chaos.
 */
void do_split(CHAR_DATA *ch, char *argument)
{
	char arg1[MAX_INPUT_LENGTH];
	one_argument(argument, arg1);

	if (arg1[0] == '\0')
	{
		send_to_char("Split how much?\n\r", ch);
		return;
	}

	auto amount_gold = atoi(arg1);
	if (amount_gold < 0)
	{
		send_to_char("Your group wouldn't like that.\n\r", ch);
		return;
	}

	if (amount_gold == 0)
	{
		send_to_char("You hand out zero coins, but no one notices.\n\r", ch);
		return;
	}

	if (ch->gold < amount_gold)
	{
		send_to_char("You don't have that much to split.\n\r", ch);
		return;
	}

	auto members = 0;
	for (auto gch = ch->in_room->people; gch != nullptr; gch = gch->next_in_room)
	{
		if (is_same_group(gch, ch) && !is_affected_by(gch, AFF_CHARM))
			members++;
	}

	if (members < 2)
	{
		send_to_char("Just keep it all.\n\r", ch);
		return;
	}

	auto share_gold = amount_gold / members;
	auto extra_gold = amount_gold % members;

	if (share_gold == 0)
	{
		send_to_char("Don't even bother, cheapskate.\n\r", ch);
		return;
	}

	ch->gold -= amount_gold;
	ch->gold += share_gold + extra_gold;

	char buf[MAX_STRING_LENGTH];
	if (share_gold > 0)
	{
		sprintf(buf, "You split %d gold coins. Your share is %d gold.\n\r", amount_gold, share_gold + extra_gold);
		send_to_char(buf, ch);
	}

	sprintf(buf, "$n splits %d gold coins. Your share is %d gold.", amount_gold, share_gold);

	for (auto gch = ch->in_room->people; gch != nullptr; gch = gch->next_in_room)
	{
		if (gch != ch && is_same_group(gch, ch) && !is_affected_by(gch, AFF_CHARM))
		{
			act(buf, ch, nullptr, gch, TO_VICT);
			gch->gold += share_gold;
		}
	}
}

void do_gtell(CHAR_DATA *ch, char *argument)
{
	if (is_affected(ch, gsn_silence))
	{
		send_to_char("You cannot find the words within you.\n\r", ch);
		return;
	}

	if (is_affected_area(ch->in_room->area, gsn_interference))
	{
		send_to_char("The electrical currents in the area block your attempt to communicate.\n\r", ch);
		return;
	}

	if (argument[0] == '\0')
	{
		send_to_char("Tell your group what?\n\r", ch);
		return;
	}

	if (IS_SET(ch->comm, COMM_NOTELL))
	{
		send_to_char("Your message didn't get through!\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_imprisonvoice))
	{
		send_to_char("Your vocal cords are frozen solid!\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_gag))
	{
		send_to_char("You are gagged and cannot speak!\n\r", ch);
		return;
	}

	check_ooc(ch, argument, "GROUPTELL");

	auto deaf = false;
	if (is_affected(ch, gsn_deafen))
	{
		deaf = true;
		argument = palloc_string(upstring(argument));
	}

	char buf[MAX_STRING_LENGTH];
	sprintf(buf, "You tell the group '%s$t%s'", get_char_color(ch, "grouptells"), END_COLOR(ch));
	act_new(buf, ch, argument, nullptr, TO_CHAR, POS_SLEEPING);

	for (auto gch = char_list; gch != nullptr; gch = gch->next)
	{
		if (is_same_group(gch, ch) && !is_affected(gch, gsn_deafen))
		{
			sprintf(buf, "$n tells the group '%s$t%s'", get_char_color(gch, "grouptells"), END_COLOR(gch));
			act_new(buf, ch, argument, gch, TO_VICT, POS_SLEEPING);
		}
	}

	if (deaf)
		free_pstring(argument);
}

SPEECH_DATA *find_speech(MOB_INDEX_DATA *mob, char *name)
{
	if (!mob->speech)
		return nullptr;

	for (auto speech = mob->speech; speech; speech = speech->next)
	{
		if (!str_cmp(speech->name, name))
			return speech;
	}

	return nullptr;
}

void execute_speech(CHAR_DATA *ch, CHAR_DATA *mob, SPEECH_DATA *speech)
{
	if (!speech || !speech->first_line || !mob || !mob->in_room)
		return;

	if (speech->first_line != speech->current_line)
		return;

	speech_handler(ch, mob, speech);
}

void speech_handler(CHAR_DATA *ch, CHAR_DATA *mob, SPEECH_DATA *speech)
{
	if (!speech || !speech->first_line || !mob || !mob->in_room)
		return;

	if (mob->position < POS_RESTING)
		return;

	auto line = speech->current_line;
	if (line == nullptr)
	{
		speech->current_line = speech->first_line;
		return;
	}

	char buf[MAX_STRING_LENGTH];
	for (auto i = 0; i < MAX_STRING_LENGTH; i++)
	{
		buf[i] = '\0';
	}

	auto copy = palloc_string(line->text);
	auto point = copy;

	for (auto i = 0; *point && *point != '\0'; i++)
	{
		if (*point == '$')
		{
			point++;
			if (*point == 'N')
			{
				strcat(buf, ch->name);
				i += (strlen(ch->name) - 1);
			}
			else
			{
				point--;
				buf[i] = *point;
			}
		}
		else
		{
			buf[i] = *point;
		}
		point++;
	}

	free_pstring(copy);

	std::string buffer; // outside of switch to appease the compiler gods
	switch (line->type)
	{
	case SPEECH_SAY:
		do_say(mob, buf);
		break;
	case SPEECH_SAYTO:
		say_to(mob, ch, buf, "");
		break;
	case SPEECH_TELL:
		buffer = fmt::format("{} {}", ch->name, buf);
		do_tell(mob, buffer.data());
		break;
	case SPEECH_WHISPER:
		do_whisper(mob, buf);
		break;
	case SPEECH_YELL:
		do_yell(mob, buf);
		break;
	case SPEECH_EMOTE:
		do_emote(mob, buf);
		break;
	case SPEECH_ECHO:
		break;
	case SPEECH_SING:
		do_sing(mob, buf);
		break;
	default:
		RS.Bug("Error in speech -- missing/invalid type.");
		return;
	}

	if (!line->next)
	{
		speech->current_line = speech->first_line;
		return;
	}

	speech->current_line = line->next;

	if (speech->current_line->delay <= 0)
	{
		speech_handler(ch, mob, speech);
		return;
	}

	RS.Queue.AddToQueue(speech->current_line->delay, 3, speech_handler, ch, mob, speech);
}

/*
 * It is very important that this be an equivalence relation:
 * (1) A ~ A
 * (2) if A ~ B then B ~ A
 * (3) if A ~ B  and B ~ C, then A ~ C
 */
bool is_same_group(CHAR_DATA *ach, CHAR_DATA *bch)
{
	if (ach == nullptr || bch == nullptr)
		return false;

	/* if ( ( ach->level - bch->level > 8 || ach->level - bch->level < -8 ) && !is_npc(ach) )
		return false;*/

	if (ach->leader != nullptr)
		ach = ach->leader;

	if (bch->leader != nullptr)
		bch = bch->leader;

	return ach == bch;
}

void do_release(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	one_argument(argument, arg);

	if (arg[0] == '\0')
	{
		send_to_char("Release which follower?\n\r", ch);
		return;
	}

	auto victim = get_char_room(ch, arg);
	if (victim == nullptr)
	{
		send_to_char("They aren't here.\n\r", ch);
		return;
	}

	if (!is_affected_by(victim, AFF_CHARM) || victim->master != ch)
	{
		send_to_char("They aren't under your control.\n\r", ch);
		return;
	}

	if (is_npc(victim))
	{
		act("$n slowly fades away.", victim, 0, 0, TO_ROOM);
		extract_char(victim, true);
	}
	else
	{
		send_to_char("You can't release a player into the void..maybe nofollow them?\n\r", ch);
		return;
	}
}

void perm_death_log(CHAR_DATA *ch, int deltype)
{
	auto fp = fopen(PDEATH_LOG_FILE, "a");
	if (fp == nullptr)
		return;

	char *deletedReason; // deltype == 1 ? "DELETED" : deltype == 2 ? "CON-DIE" : deltype == 3	? "OLD-AGE"
						//		   : deltype == 4	? "AUTODELETE" : "DELETED";
	switch(deltype)
	{
		case 2:
			deletedReason = "CON-DIE";
		break;
		case 3:
			deletedReason = "OLD-AGE";
		break;
		case 4:
			deletedReason = "AUTODELETE";
		break;
		case 1:
		default:
			deletedReason = "DELETED";
		break;
	}

	fprintf(fp, "%s||[%2d %5s %s]||%s%s%s%s||Kills: %d||Deaths: %d||Age: %d||Hours: %ld||%s",
			deletedReason,
			ch->level,
			pc_race_table[ch->race].who_name,
			(RSTR)ch->Class()->who_name,
			cabal_table[ch->cabal].who_name,
			ch->true_name, is_npc(ch) ? "" : ch->pcdata->title,
			is_npc(ch) ? "" : (ch->pcdata->extitle) ? ch->pcdata->extitle
													: "",
			ch->pcdata->kills[PK_KILLS],
			ch->pcdata->killed[PK_KILLED],
			get_age(ch),
			(ch->played + current_time - ch->logon) / 3600,
			ctime(&current_time));

	fclose(fp);
}

void temp_death_log(CHAR_DATA *killer, CHAR_DATA *dead)
{
	if (is_npc(killer) && (killer->master != nullptr))
		killer = killer->master;

	if (is_npc(dead) || is_npc(killer))
		return;

	char query[MAX_STRING_LENGTH];
	sprintf(query, "INSERT INTO pklogs VALUES('%s', %d, '%s', %d, '%s', '%s (%d)',%ld)",
			killer->true_name,
			killer->cabal,
			dead->true_name,
			dead->cabal,
			log_time(),
			escape_string(get_room_name(dead->in_room)),
			dead->in_room->vnum,
			current_time);

	one_query(query);
}

void mob_death_log(CHAR_DATA *killer, CHAR_DATA *dead)
{
	if (!is_npc(killer) || is_npc(dead) || !is_explore(killer->in_room))
		return;

	auto fp = fopen(MOB_LOG_FILE, "a");
	if (fp == nullptr)
		return;

	fprintf(fp, "%s | %s | %s | %s | %s",
			dead->true_name,
			killer->short_descr,
			killer->in_room->area->name,
			get_room_name(killer->in_room),
			ctime(&current_time));

	fclose(fp);
}

/* type 0 = create, 1 = login, 2 = logout */
void login_log(CHAR_DATA *ch, int type)
{
	if (!ch->pcdata->host && !ch->desc)
		return;

	if (IS_SET(ch->comm, COMM_NOSOCKET))
		return;

	auto escape = ch->pcdata->host
					  ? escape_string(ch->pcdata->host)
					  : escape_string(ch->desc->host);

	char query[MAX_STRING_LENGTH];
	sprintf(query, "INSERT INTO logins VALUES('%s', '%s', '%s', '%ld', '%ld', '%d', '%d', '%d')",
			ch->true_name,
			escape,
			log_time(),
			current_time,
			type == 2 ? ((current_time - ch->logon) / 60) : -1,
			type > 0 ? count_carried(ch, false) : -1, type > 0 ? count_carried(ch, true) : -1,
			type);

	one_query(query);
}
