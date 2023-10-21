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
 *      Russ Taylor (rtaylor@pacinfo.com)                                  *
 *      Gabrielle Taylor (gtaylor@pacinfo.com)                             *
 *      Brian Moore (rom@rom.efn.org)                                      *
 *  By using this code, you have agreed to follow the terms of the         *
 *  ROM license, in the file Tartarus/doc/rom.license                      *
 ***************************************************************************/

/***************************************************************************
 *       Tartarus code is copyright (C) 1997-1998 by Daniel Graham         *
 *  In using this code you agree to comply with the Tartarus license       *
 *       found in the file /Tartarus/doc/tartarus.doc                      *
 ***************************************************************************/

#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mysql.h>
#include <iterator>
#include "dioextra.h"
#include "handler.h"
#include "recycle.h"
#include "tables.h"
#include "lookup.h"
#include "save.h"
#include "act_comm.h"
#include "act_info.h"
#include "fight.h"
#include "mspec.h"
#include "comm.h"
#include "act_wiz.h"
#include "interp.h"
#include "skills.h"
#include "db.h"
#include "devextra.h"
#include "magic.h"
#include "quest.h"
#include "chardef.h"
#include "const.h"
#include "utility.h"
#include "newmem.h"
#include "./include/fmt/format.h"
#include "./include/fmt/printf.h"

void do_crumble(CHAR_DATA *ch, char *argument)
{
	char arg1[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	char arg3[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	OBJ_DATA *obj;

	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);
	argument = one_argument(argument, arg3);

	if (arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0')
	{
		send_to_char("Syntax: crumble <char> <inv/eq> <item>\n\r", ch);
		return;
	}

	if ((victim = get_char_world(ch, arg1)) == nullptr)
	{
		send_to_char("They aren't here.\n\r", ch);
		return;
	}

	if (get_trust(victim) > get_trust(ch))
	{
		send_to_char("You can't do that.\n\r", ch);
		return;
	}

	if (!str_prefix(arg2, "inventory"))
	{
		if ((obj = get_obj_carry(victim, arg3, ch)) == nullptr)
		{
			send_to_char("You can't find it.\n\r", ch);
			return;
		}
	}
	else if (!str_prefix(arg2, "equipment"))
	{
		if ((obj = get_obj_wear(victim, arg3)) == nullptr)
		{
			send_to_char("You can't find it.\n\r", ch);
			return;
		}
		unequip_char(ch, obj, true);
	}
	else
	{
		send_to_char("Syntax: crumble <char> <inv/eq> <item>\n\r", ch);
		return;
	}

	act("$p crumbles into dust.", victim, obj, nullptr, TO_CHAR);
	act("$N: $p crumbles into dust.", ch, obj, victim, TO_CHAR);
	obj_from_char(obj);
	extract_obj(obj);
}

void do_ghost(CHAR_DATA *ch, char *argument)
{
	char arg1[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	DESCRIPTOR_DATA *d;

	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);

	if (arg1[0] == '\0' || arg2[0] == '\0')
	{
		send_to_char("Syntax:\n\r", ch);
		send_to_char("  ghost <name> <yes/no>\n\r", ch);
		send_to_char("  ghost all <yes/no>\n\r", ch);
		send_to_char("  ghost zone <yes/no>\n\r", ch);
		send_to_char("Ghosting persons YES makes them a ghost, out of PK range.\n\r", ch);
		send_to_char("Ghosting persons NO unghosts them, and puts them back into range.\n\r", ch);
		send_to_char("Ghosting ALL changes EVERYONE to/from a ghost.\n\r", ch);
		send_to_char("Ghosting ZONE changes everyone in your AREA to/from a ghost.\n\r", ch);
		send_to_char("NOTE: Ghosting TO a ghost lasts for 15 ticks, then wears off.\n\r", ch);
		return;
	}

	if (!str_cmp(arg1, "all"))
	{
		for (d = descriptor_list; d; d = d->next)
		{
			if (d->connected == CON_PLAYING
				&& d->character != ch
				&& d->character->in_room != nullptr
				&& can_see(ch, d->character))
			{
				if (!str_cmp(arg2, "yes"))
				{
					d->character->ghost = 15;
					send_to_char("You have turned into an invincible ghost for several minutes.\n\r", d->character);
				}

				if (!str_cmp(arg2, "no"))
				{
					d->character->ghost = 0;
					send_to_char("You are no longer an invincible ghost.\n\r", d->character);
				}
			}
		}

		return;
	}

	if (!str_cmp(arg1, "zone"))
	{
		for (d = descriptor_list; d; d = d->next)
		{
			if (d->connected == CON_PLAYING 
				&& d->character != ch
				&& d->character->in_room != nullptr
				&& can_see(ch, d->character)
				&& d->character->in_room->area == ch->in_room->area)
			{
				if (!str_cmp(arg2, "yes"))
				{
					d->character->ghost = 15;
					send_to_char("You have turned into an invincible ghost for a several minutes.\n\r", d->character);
				}

				if (!str_cmp(arg2, "no"))
				{
					d->character->ghost = 0;
					send_to_char("You are no longer an invincible ghost.\n\r", d->character);
				}
			}
		}

		return;
	}

	victim = get_char_world(ch, arg1);

	if (victim == nullptr)
	{
		send_to_char("They aren't here.\n\r", ch);
		return;
	}

	if (!str_cmp(arg2, "yes"))
	{
		victim->ghost = 15;
		send_to_char("You have turned into an invincible ghost for a several minutes.\n\r", victim);
	}

	if (!str_cmp(arg2, "no"))
	{
		victim->ghost = 0;
		send_to_char("You are no longer an invincible ghost.\n\r", victim);
	}
}

void do_praclist(CHAR_DATA *ch, char *argument)
{
	char arg1[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
	int sn;
	CHAR_DATA *victim;
	int col;

	col = 0;
	argument = one_argument(argument, arg1);

	if (arg1[0] == '\0')
	{
		send_to_char("See whose praclist?\n\r", ch);
		return;
	}

	if ((victim = get_char_world(ch, arg1)) == nullptr)
	{
		send_to_char("They aren't here.\n\r", ch);
		return;
	}

	if (is_npc(victim))
		return;

	for (sn = 0; sn < MAX_SKILL; sn++)
	{
		if (skill_table[sn].name == nullptr)
			break;

		if (skill_table[sn].skill_level[victim->Class()->GetIndex()] > 52)
			continue;

		if (victim->level < skill_table[sn].skill_level[victim->Class()->GetIndex()]
			|| victim->pcdata->learned[sn] < 1 /* skill is not known */)
		{
			continue;
		}

		sprintf(buf, "%-18s %3d%%  ", skill_table[sn].name, victim->pcdata->learned[sn]);
		send_to_char(buf, ch);

		if (++col % 3 == 0)
			send_to_char("\n\r", ch);
	}

	if (col % 3 != 0)
		send_to_char("\n\r", ch);

	sprintf(buf, "%d practice sessions left.\n\r", victim->practice);
	send_to_char(buf, ch);
}

void do_affrem(CHAR_DATA *ch, char *argument)
{
	char arg1[MAX_STRING_LENGTH];
	char arg2[MAX_STRING_LENGTH];
	char buf[MAX_STRING_LENGTH];
	CHAR_DATA *victim;
	int sn;
	AFFECT_DATA *af, *af_next;
	bool remove = true;
	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);

	if (arg1[0] == '\0')
	{
		send_to_char("Syntax: affrem <char> <affect>\n\r", ch);
		return;
	}

	if ((victim = get_char_world(ch, arg1)) == nullptr)
	{
		send_to_char("They aren't playing.\n\r", ch);
		return;
	}

	if (arg2[0] == '\0')
	{
		send_to_char("Syntax: affrem <char> <affect>\n\r", ch);
		return;
	}

	sn = 0;
	if ((sn = skill_lookup(arg2)) < 0)
	{
		send_to_char("No such skill or spell.\n\r", ch);
		return;
	}

	if (!(is_affected(victim, sn)))
	{
		send_to_char("Target is not affected by that.\n\r", ch);
		return;
	}

	for (af = victim->affected; af != nullptr; af = af_next)
	{
		af_next = af->next;

		if (IS_SET(af->bitvector, AFF_PERMANENT) && get_trust(ch) < MAX_LEVEL && af->type == sn)
		{
			send_to_char("You cannot remove a permanent affect.\n\r", ch);
			remove = false;
			continue;
		}

		if (af->type == sn)
			affect_remove(victim, af);
	}

	if (skill_table[sn].msg_off && remove)
	{
		send_to_char(skill_table[sn].msg_off, victim);
		send_to_char("\n\r", victim);
	}

	if (skill_table[sn].room_msg_off && remove)
		act(skill_table[sn].room_msg_off, victim, 0, 0, TO_ROOM);

	sprintf(buf, "The %s affect is removed from %s.\n\r", skill_table[sn].name, victim->name);

	if (remove)
		send_to_char(buf, ch);
}

void do_divine_intervention(CHAR_DATA *ch, char *argument)
{
	char arg1[MAX_STRING_LENGTH];
	char arg2[MAX_STRING_LENGTH];
	char arg3[MAX_STRING_LENGTH];
	char arg4[MAX_STRING_LENGTH];
	CHAR_DATA *victim;
	int enchant_type, value, duration;
	bool isgoodaffect;
	AFFECT_DATA af;
	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);
	argument = one_argument(argument, arg3);
	argument = one_argument(argument, arg4);

	if (((arg1[0] == '\0') || (arg2[0] == '\0') || (arg3[0] == '\0') || arg4[0] == '\0') && str_cmp(arg2, "pet"))
	{
		send_to_char("Syntax:  intervention <char> <affect> <value> <duration>\n\r", ch);
		send_to_char("Affects: hp mana move ac hitroll damroll saves\n\r", ch);
		return;
	}

	if ((victim = get_char_world(ch, arg1)) == nullptr)
	{
		send_to_char("They aren't playing.\n\r", ch);
		return;
	}

	if (!str_cmp(arg2, "pet"))
	{
		create_academy_pet(victim);
		return;
	}

	if (!(is_number(arg3)))
	{
		send_to_char("Value must be numeric.\n\r", ch);
		return;
	}

	if (!(is_number(arg4)))
	{
		send_to_char("Duration must be numeric.\n\r", ch);
		return;
	}

	isgoodaffect = false;

	value = atoi(arg3);
	duration = atoi(arg4);

	if (!str_prefix(arg2, "hp"))
	{
		if (value > 0)
			isgoodaffect = true;

		enchant_type = APPLY_HIT;
	}
	else if (!str_prefix(arg2, "mana"))
	{
		if (value > 0)
			isgoodaffect = true;

		enchant_type = APPLY_MANA;
	}
	else if (!str_prefix(arg2, "move"))
	{
		if (value > 0)
			isgoodaffect = true;

		enchant_type = APPLY_MOVE;
	}
	else if (!str_prefix(arg2, "ac"))
	{
		if (value < 0)
			isgoodaffect = true;

		enchant_type = APPLY_AC;
	}
	else if (!str_prefix(arg2, "hitroll"))
	{
		if (value > 0)
			isgoodaffect = true;

		enchant_type = APPLY_HITROLL;
	}
	else if (!str_prefix(arg2, "damroll"))
	{
		if (value > 0)
			isgoodaffect = true;

		enchant_type = APPLY_DAMROLL;
	}
	else if (!str_prefix(arg2, "saves"))
	{
		if (value < 0)
			isgoodaffect = true;

		enchant_type = APPLY_SAVING_SPELL;
	}
	else
	{
		send_to_char("No affect of that name.\n\r", ch);
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_POWER;
	af.type = gsn_divine_intervention;
	af.level = ch->level;
	af.duration = duration;
	af.modifier = value;
	af.location = enchant_type;
	af.aftype = AFT_COMMUNE;
	affect_to_char(victim, &af);

	std::string buffer;
	if (isgoodaffect == true)
	{
		buffer = fmt::format("{} is blessed with {}.\n\r", victim->name, arg2);
		send_to_char(buffer.c_str(), ch);
		send_to_char("You have been blessed by the Immortals.\n\r", victim);
	}
	else
	{
		buffer = fmt::format("{} is cursed with {}.\n\r", victim->name, arg2);
		send_to_char(buffer.c_str(), ch);
		send_to_char("You have been cursed by the Immortals.\n\r", victim);
	}
}

void do_ccb(CHAR_DATA *ch, char *argument)
{
	char arg1[MAX_STRING_LENGTH];
	char arg2[MAX_STRING_LENGTH];
	int cabal;
	DESCRIPTOR_DATA *d;

	/*
	if (!is_immortal(ch))
	{
		send_to_char("Huh?\n\r",ch);
		return;
	}
	*/

	argument = one_argument(argument, arg1);
	strcpy(arg2, argument);

	if (arg1[0] == '\0')
	{
		send_to_char("Syntax: ccb <cabal> <message>\n\r", ch);
		return;
	}

	if (arg2[0] == '\0')
	{
		send_to_char("Syntax: ccb <cabal> <message>\n\r", ch);
		return;
	}

	cabal = cabal_lookup(arg1);
	if (!(cabal))
	{
		send_to_char("That's not a valid cabal.\n\r", ch);
		return;
	}

	auto buffer = fmt::format("{}{}: {}{}{}\n\r",
		cabal_table[cabal].who_name,
		is_npc(ch) ? ch->short_descr : ch->name,
		get_char_color(ch, "channels"),
		arg2,
		END_COLOR(ch));

	send_to_char(buffer.c_str(), ch);

	for (d = descriptor_list; d != nullptr; d = d->next)
	{
		if (d->connected == CON_PLAYING)
		{
			if ((d->character != ch
					&& !IS_SET(d->character->comm, COMM_NOCABAL)
					&& d->character->cabal
					&& d->character->cabal == cabal)
				|| (IS_SET(d->character->comm, COMM_ALL_CABALS)
					&& d->character != ch))
			{
				buffer = fmt::format("{}{}: {}{}{}\n\r",
					cabal_table[cabal].who_name,
					pers(ch, d->character),
					get_char_color(d->character, "channels"),
					arg2,
					END_COLOR(ch));
				send_to_char(buffer.c_str(), d->character);
			}
		}
	}
}

void do_powers(CHAR_DATA *ch, char *argument)
{
	BUFFER *buffer;
	char arg[MAX_INPUT_LENGTH];
	char skill_list[LEVEL_HERO + 1][MAX_STRING_LENGTH];
	char skill_columns[LEVEL_HERO + 1];
	int gns, sn, level, min_lev = 1, max_lev = LEVEL_HERO;
	bool fAll = false, found = false;
	char buf[MAX_STRING_LENGTH];

	if (is_npc(ch))
		return;

	if (!ch->cabal)
	{
		send_to_char("You are homeless; you have no powers.\n\r", ch);
		return;
	}

	if (argument[0] != '\0')
	{
		fAll = true;

		if (str_prefix(argument, "all"))
		{
			argument = one_argument(argument, arg);

			if (!is_number(arg))
			{
				send_to_char("Arguments must be numerical or all.\n\r", ch);
				return;
			}

			max_lev = atoi(arg);

			if (max_lev < 1 || max_lev > LEVEL_HERO)
			{
				sprintf(buf, "Levels must be between 1 and %d.\n\r", LEVEL_HERO);
				send_to_char(buf, ch);
				return;
			}

			if (argument[0] != '\0')
			{
				argument = one_argument(argument, arg);

				if (!is_number(arg))
				{
					send_to_char("Arguments must be numerical or all.\n\r", ch);
					return;
				}

				min_lev = max_lev;
				max_lev = atoi(arg);

				if (max_lev < 1 || max_lev > LEVEL_HERO)
				{
					sprintf(buf, "Levels must be between 1 and %d.\n\r", LEVEL_HERO);
					send_to_char(buf, ch);
					return;
				}

				if (min_lev > max_lev)
				{
					send_to_char("That would be silly.\n\r", ch);
					return;
				}
			}
		}
	}

	/* initialize data */
	for (level = 0; level < LEVEL_HERO + 1; level++)
	{
		skill_columns[level] = 0;
		skill_list[level][0] = '\0';
	}

	for (gns = 0; cabal_skills[gns].skill; gns++)
	{
		sn = skill_lookup(cabal_skills[gns].skill);

		if ((level = skill_table[sn].skill_level[ch->Class()->GetIndex()]) < LEVEL_HERO + 1
			&& level >= min_lev
			&& level <= max_lev && ch->pcdata->learned[sn] > 0)
		{
			found = true;
			level = skill_table[sn].skill_level[ch->Class()->GetIndex()];

			if (ch->level < level)
				sprintf(buf, "%-18s n/a      ", skill_table[sn].name);
			else
				sprintf(buf, "%-18s %3d%%      ", skill_table[sn].name, ch->pcdata->learned[sn]);

			if (skill_list[level][0] == '\0')
			{
				sprintf(skill_list[level], "\n\rLevel %2d: %s", level, buf);
			}
			else
			{
				if (++skill_columns[level] % 2 == 0)
					strcat(skill_list[level], "\n\r          ");

				strcat(skill_list[level], buf);
			}
		}
	}

	/* return results */

	if (!found)
	{
		send_to_char("No powers found.\n\r", ch);
		return;
	}

	buffer = new_buf();

	for (level = 0; level < LEVEL_HERO + 1; level++)
	{
		if (skill_list[level][0] != '\0')
		{
			add_buf(buffer, skill_list[level]);
		}
	}

	add_buf(buffer, "\n\r");
	page_to_char(buf_string(buffer), ch);
	free_buf(buffer);
}

bool check_shroud_of_light(CHAR_DATA *ch, CHAR_DATA *victim)
{
	AFFECT_DATA af;

	if (get_skill(ch, gsn_shroud_of_light) < 2)
		return false;

	if (number_percent() <= 35 && is_affected(ch, gsn_shroud_of_light))
	{
		act("As you approach $M, $N's shroud of light sears you!", ch, 0, victim, TO_CHAR);
		act("As $n approaches you, your shroud of light sears $m!", ch, 0, victim, TO_VICT);
		act("$n is seared by $N's shroud of light!", ch, 0, victim, TO_NOTVICT);

		init_affect(&af);
		af.where = TO_AFFECTS;
		af.aftype = AFT_POWER;
		af.type = gsn_blindness;
		af.level = victim->level;
		af.duration = victim->level / 10;
		SET_BIT(af.bitvector, AFF_BLIND);
		affect_to_char(ch, &af);

		damage_old(victim, ch, (victim->level + 10), gsn_shroud_of_light, DAM_HOLY, true);
		return true;
	}

	return false;
}

void do_heroimm(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim;
	char arg[MAX_STRING_LENGTH];

	if (is_npc(ch))
		return;

	one_argument(argument, arg);

	if (get_trust(ch) < MAX_LEVEL - 2)
		return;

	if (arg[0] == '\0')
	{
		send_to_char("Heroimm who?\n\r", ch);
		return;
	}

	if ((victim = get_char_world(ch, arg)) == nullptr)
	{
		send_to_char("They aren't here.\n\r", ch);
		return;
	}

	if (victim == ch)
	{
		send_to_char("Uh, no?\n\r", ch);
		return;
	}

	if (is_heroimm(victim))
	{
		send_to_char("They are already a Hero Imm.\n\r", ch);
		return;
	}

	SET_BIT(victim->act, PLR_HEROIMM);
	SET_BIT(victim->act, PLR_HOLYLIGHT);

	victim->max_hit = 10000;
	victim->hit = victim->max_hit;
	victim->max_mana = 10000;
	victim->mana = victim->max_mana;
	victim->max_move = 10000;
	victim->move = victim->max_move;

	send_to_char("They are now a Hero Immortal.\n\r", ch);
	send_to_char("You are now a Hero Immortal.\n\r", victim);
}

void check_keen_vision(CHAR_DATA *ch, CHAR_DATA *victim)
{
	char buf[MAX_STRING_LENGTH];
	CHAR_DATA *gch;
	int chance;

	for (gch = ch->in_room->people; gch != nullptr; gch = gch->next_in_room)
	{
		if (gch == ch || gch == victim)
			continue;

		if (!can_see(gch, ch))
			continue;

		chance = get_skill(gch, gsn_keen_vision);
		if (chance == 0)
			continue;

		if (number_percent() < chance)
		{
			sprintf(buf, "You notice %s slipping into %s's inventory.\n\r", pers(ch, gch), pers(victim, gch));
			send_to_char(buf, gch);
			check_improve(gch, gsn_keen_vision, true, 1);
		}
	}
}

void report_cabal_items(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];
	char pbuf[MAX_STRING_LENGTH];
	CHAR_DATA *gch;
	CHAR_DATA *guardian;
	OBJ_DATA *obj;
	OBJ_DATA *obj_next;
	bool found;
	bool afound;
	int total = 0;
	char buf1[MSL];

	if (strstr(argument, "which items") || strstr(argument, "Which items"))
	{
		if (is_npc(ch))
			return;

		found = false;
		afound = false;
		guardian = nullptr;

		for (gch = ch->in_room->people; gch != nullptr; gch = gch->next_in_room)
		{
			if (is_cabal_guard(gch))
			{
				if (ch->cabal != gch->cabal && !is_immortal(ch))
					continue;

				sprintf(buf, "I hold ");

				for (obj = gch->carrying; obj != nullptr; obj = obj_next)
				{
					obj_next = obj->next_content;

					if ((obj->pIndexData->vnum) && isCabalItem(obj))
					{
						total++;
						found = true;

						if (obj_next != nullptr)
						{
							sprintf(pbuf, "%s, ", obj->short_descr);
							strcat(buf, pbuf);
						}
						else
						{
							if (total == 1)
							{
								sprintf(pbuf, "%s.", obj->short_descr);
								strcat(buf, pbuf);
							}
							else
							{
								sprintf(pbuf, "and %s.", obj->short_descr);
								strcat(buf, pbuf);
							}
						}
					}
				}

				guardian = gch;

				if (!found)
					strcat(buf, "nothing.");

				afound = true;
			}
		}

		if (afound && guardian != nullptr)
			do_say(guardian, buf);

		return;
	}

	guardian = nullptr;
	if (strstr(argument, "where is") || strstr(argument, "Where is"))
	{
		if (is_npc(ch))
			return;

		for (gch = ch->in_room->people; gch != nullptr; gch = gch->next_in_room)
		{
			if (is_cabal_guard(gch))
			{
				if (ch->cabal == gch->cabal || is_immortal(ch))
					guardian = gch;
			}
		}

		if (guardian == nullptr)
			return;

		sprintf(buf1, "%s is the guardian.", guardian->short_descr);
		wiznet(buf1, 0, nullptr, WIZ_DEBUG, 0, 0);

		for (obj = object_list; obj != nullptr; obj = obj->next)
		{
			if (obj->pIndexData->cabal != guardian->cabal)
				continue;

			if (obj->carried_by != nullptr)
			{
				sprintf(pbuf, "%s is carried by %s.",
					obj->short_descr,
					is_npc(obj->carried_by) ? obj->carried_by->short_descr : obj->carried_by->name);
				break;
			}
		}

		if (guardian != nullptr && pbuf != nullptr && obj != nullptr)
			do_say(guardian, pbuf);
	}
}

void easy_uninduct(CHAR_DATA *ch)
{
	int gn, gns, sn;

	if (!ch->cabal)
		return;

	gn = group_lookup(cabal_table[ch->cabal].name);

	for (gns = 0; gns < MAX_SKILL; gns++)
	{
		if (group_table[gn].spells[gns] == nullptr)
			break;

		sn = skill_lookup(group_table[gn].spells[gns]);

		if (skill_table[sn].skill_level[ch->Class()->GetIndex()] < LEVEL_HERO + 1 && ch->pcdata->learned[sn] > 0)
		{
			ch->pcdata->learned[sn] = -2;
		}
	}

	group_remove(ch, cabal_table[ch->cabal].name);

	ch->cabal = 0;
}

void do_lag(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim;
	char arg1[MAX_STRING_LENGTH];
	char arg2[MAX_STRING_LENGTH];
	int pulses, rounds;

	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);

	if (arg1[0] == '\0' || arg2[0] == '\0')
	{
		send_to_char("Syntax: lag <char> <rounds>\n\r", ch);
		send_to_char("0 resets target's lag to nothing.\n\r", ch);
		return;
	}

	if ((victim = get_char_world(ch, arg1)) == nullptr)
	{
		send_to_char("They're not here.", ch);
		return;
	}

	if (!is_npc(victim) && victim->level >= get_trust(ch))
	{
		send_to_char("Respect your elders.\n\r", ch);
		return;
	}

	if (is_number(arg2))
	{
		rounds = atoi(arg2);
		pulses = rounds * 12;

		if (pulses <= 0)
		{
			victim->wait = 0;
			send_to_char("Lag removed.\n\r", ch);
		}
		else
		{
			LAG_CHAR(victim, pulses);
			send_to_char("Lag added.\n\r", ch);
		}
	}
	else
	{
		send_to_char("Rounds has to be a numerical value.\n\r", ch);
	}
}

int get_spell_aftype(CHAR_DATA *ch)
{
	if (ch->Class()->ctype == CLASS_COMMUNER)
		return AFT_COMMUNE;
	else
		return AFT_SPELL;

	return -1;
}

void do_finger(CHAR_DATA *ch, char *argument)
{
	char arg1[MAX_STRING_LENGTH];
	char arg2[MAX_STRING_LENGTH];
	char buf1[MAX_STRING_LENGTH];
	char buf2[MAX_STRING_LENGTH];
	char buf3[MAX_STRING_LENGTH];
	char buf97[MAX_STRING_LENGTH], *tbuf = nullptr;
	FILE *fp;
	int align = -1, class_index = -1, ethos = -1, kills = -1, gkills = -1, nkills = -1, ekills = -1;
	int pkilled = -1, mkilled = -1, level = -1, played = -1, cabal = 0, induct = -1, race = -1;
	int con = 0, stat = 25, died = 0, agemod = 0, bcredits = 0, hometown = 0, aobj = -1, lobj = -1, timeplayed = -1;
	int vnum = 0, room = 0;
	char *history, *name, *login, *title, *extitle, *sex, *allsites, *desc;
	bool fMatch = false;
	char *word;
	bool end = false;
	bool dead = false;
	char eqbuf[MAX_STRING_LENGTH];
	long sect_time[SECT_MAX];
	short quests[MAX_QUESTS];
	char questtext[MSL];
	BUFFER *output;
	long act[MAX_BITVECTOR];
	long sectval[SECT_MAX], comm = 0;
	float perc[SECT_MAX];
	int i, x = 0, born = 0, dtime = 0, souls = -1;
	int reputation = 0;
	OBJ_INDEX_DATA *pObjIndex;

	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);

	if (arg1[0] == '\0')
	{
		send_to_char("Syntax: finger <char>\n\r", ch);
		send_to_char("        finger <char> history\n\r", ch);
		send_to_char("        finger <char> equipment\n\r", ch);
		send_to_char("        finger <char> description\n\r", ch);
		send_to_char("        finger <char> quests\n\r", ch);
		send_to_char("        finger <char> role\n\r", ch);
		return;
	}

	arg1[0] = UPPER(arg1[0]);
	questtext[0] = '\0';
	for (i = 0; i < MAX_QUESTS; i++)
	{
		quests[i] = 0;
	}

	sect_time[0] = 0;

	sprintf(buf3, "(none)");

	history = nullptr;

	name = palloc_string(buf3);
	login = palloc_string(buf3);
	sex = palloc_string(buf3);
	title = nullptr;
	extitle = nullptr;
	allsites = nullptr;
	desc = nullptr;

	sprintf(eqbuf, "Sorry, that is unavailable.\n\r");
	auto dir = fmt::format("{}/{}{}", RIFT_PLAYER_DIR, arg1, ".plr");
	auto dir2 = fmt::format("{}/dead_char/{}{}", RIFT_PLAYER_DIR, arg1, ".plr");

	if (!str_cmp(arg1, "Zzz"))
	{
		send_to_char("Invalid player.\n\r", ch);
		return;
	}

	if (((fp = fopen(dir.c_str(), "r")) != nullptr))
	{
		dead = false;
	}
	else if (((fp = fopen(dir2.c_str(), "r")) != nullptr))
	{
		dead = true;
	}
	else
	{
		send_to_char("No such character.\n\r", ch);
		return;
	}

	for (;;)
	{
		word = feof(fp) ? (char *)"End" : fread_word(fp);
		fMatch = false;

		switch (UPPER(word[0]))
		{
			case '*':
				fMatch = true;
				fread_to_eol(fp);
				break;
			case 'A':
				KEY("Alig", align, fread_number(fp))
				KEYV("Act", act)
				KEY("Agemod", agemod, fread_number(fp))

				if (!str_cmp(word, "Attr"))
				{
					for (stat = 0; stat < MAX_STATS; stat++)
					{
						con = fread_number(fp);
						if (stat == STAT_CON)
							break;
					}

					break;
				}

				break;
			case 'B':
				KEY("BCredits", bcredits, fread_number(fp))
				KEY("Born", born, fread_number(fp))
				break;
			case 'C':
				if (!str_cmp(word, "Cla"))
				{
					class_index = CClass::Lookup(fread_string(fp));
					fMatch = true;
					break;
				}

				KEY("Cabal", cabal, cabal_lookup(fread_string(fp)))
				KEY("Comm", comm, fread_flag(fp))
				break;
			case 'D':
				KEY("Died", died, fread_number(fp))
				KEY("Desc", desc, fread_string(fp))
				KEY("DeathTime", dtime, fread_number(fp))
				break;
			case 'E':
				KEY("Etho", ethos, fread_number(fp))
				KEY("EXTitl", extitle, fread_string(fp))

				if (!str_cmp(word, "End"))
				{
					end = true;
				}

				break;
			case 'F':
				if (!str_cmp(word, "FingEQ"))
				{
					eqbuf[0] = '\0';

					for (i = 0; i < MAX_WEAR; i++)
					{
						vnum = fread_number(fp);
						if (vnum == 0)
							continue;

						pObjIndex = get_obj_index(vnum);
						sprintf(buf2, "%s%s (%d)\n\r", get_where_name(i), pObjIndex ? pObjIndex->short_descr : "# NON-EXISTANT #", vnum);
						strcat(eqbuf, buf2);
					}
				}
				break;
			case 'H':
				KEY("History", history, fread_string(fp))
				KEY("HomeTown", hometown, fread_number(fp))
				break;
			case 'I':
				KEY("Indu", induct, fread_number(fp))
				break;
			case 'K':
				if (!str_cmp(word, "kls"))
				{
					kills = fread_number(fp);
					gkills = fread_number(fp);
					nkills = fread_number(fp);
					ekills = fread_number(fp);
				}

				if (!str_cmp(word, "kld"))
				{
					pkilled = fread_number(fp);
					mkilled = fread_number(fp);
				}

				fMatch = true;
				break;
			case 'L':
				KEY("Levl", level, fread_number(fp))
				KEY("LogonTime", login, fread_string(fp))
				break;
			case 'N':
				KEY("Name", name, fread_string(fp))
				break;
			case 'P':
				KEY("Plyd", played, fread_number(fp))
				break;
			case 'Q':
				if (!str_cmp(word, "Quest"))
					quests[fread_number(fp)] = fread_number(fp);

				break;
			case 'R':
				KEY("Race", race, race_lookup(fread_string(fp)))
				KEY("Rep", reputation, fread_number(fp))
				KEY("Room", room, fread_number(fp))
				KEY("Role", tbuf, fread_string(fp))
				break;
			case 'S':
				KEY("Sex", sex, sex_table[fread_number(fp)].name)
				KEY("SiteTrack", allsites, fread_string(fp))
				KEY("Souls", souls, fread_number(fp))

				if (!str_cmp(word, "Sect"))
				{
					while ((i = fread_number(fp)) != -1)
					{
						sect_time[x] = i;
						x++;
					}
				}

				break;
			case 'T':
				KEY("Titl", title, fread_string(fp))
				KEY("TimePlayed", timeplayed, fread_number(fp))
				KEY("TrackAObj", aobj, fread_number(fp))
				KEY("TrackLObj", lobj, fread_number(fp))
				break;
		}

		if (!fMatch)
			fread_to_eol(fp);

		if (arg2[0] != '\0' && !str_prefix(arg2, "history") && history)
			break;

		if (end)
			break;
	}

	fclose(fp);

	if (arg2[0] != '\0' && !str_prefix(arg2, "history"))
	{
		output = new_buf();
		if (level >= ch->level)
		{
			send_to_char("Player is too high to access their history.\n\r", ch);
			return;
		}

		sprintf(buf2, "FINGER: %s's player history:\n\r", name);
		send_to_char(buf2, ch);

		if (history == nullptr || history[0] == '\0')
		{
			send_to_char("No pfile history available.\n\r", ch);
		}
		else
		{
			add_buf(output, history);
			page_to_char(buf_string(output), ch);
			free_buf(output);
		}
	}
	else if (arg2[0] != '\0' && !str_prefix(arg2, "equipment"))
	{
		sprintf(buf2, "FINGER: %s's equipment:\n\r(Objects: %d total, %d limited)\n\r\n\r", name, aobj, lobj);
		send_to_char(buf2, ch);

		if (eqbuf[0] != '\0')
			send_to_char(eqbuf, ch);
		else
			send_to_char("Nothing.\n\r", ch);
	}
	else if (arg2[0] != '\0' && !str_prefix(arg2, "description"))
	{
		sprintf(buf2, "FINGER: %s's description:\n\r", name);
		send_to_char(buf2, ch);

		if (desc != nullptr)
			send_to_char(desc, ch);
		else
			send_to_char("No description available.\n\r", ch);
	}
	else if (arg2[0] != '\0' && !str_prefix(arg2, "quests"))
	{
		sprintf(buf2, " *** Quest history for %s (reputation %d) ***\n\r", name, reputation);
		send_to_char(buf2, ch);

		fMatch = false;

		for (i = 0; i < MAX_QUESTS; i++)
		{
			if (quests[i] > 0)
			{
				fMatch = true;
				strcat(questtext, quest_table[i].name);
				strcat(questtext, " -- ");
				strcat(questtext, quest_table[i].description);
				strcat(questtext, "\n\r    Current progress: ");
				strcat(questtext, quest_table[i].stages[quests[i] - 1]);
				strcat(questtext, "\n\r");
			}
		}

		if (fMatch)
			send_to_char(questtext, ch);
		else
			send_to_char("No quests completed.\n\r", ch);
	}
	else if (arg2[0] != '\0' && !str_prefix(arg2, "role"))
	{
		send_to_char("*** ROLE ***\n\r", ch);

		if (!tbuf)
		{
			sprintf(buf2, "%s has no role.\n\r", name);
			send_to_char(buf2, ch);
			return;
		}
		else
		{
			output = new_buf();
			add_buf(output, tbuf);
			page_to_char(buf_string(output), ch);
			free_buf(output);
			return;
		}
	}
	else
	{
		send_to_char("FINGER INFO:\n\r", ch);
		sprintf(buf2, "%s",
			dead && IS_SET(act, PLR_DENY) && con < CON_DIE_BOUND ? "  *** CON-DEAD CHARACTER: ***\n\r" :
			dead && IS_SET(act, PLR_DENY) 						 ? "  *** DENIED CHARACTER: ***\n\r"   :
			dead && died == HAS_DIED							 ? "  *** AGE-DEAD CHARACTER: ***\n\r" :
			dead												 ? "  *** DELETED CHARACTER: ***\n\r"  : "");
		send_to_char(buf2, ch);

		sprintf(buf2, "  [%2d %5s %s%s] %s%s %s%s\n\r",
			level,
			pc_race_table[race].who_name,
			(RSTR)CClass::GetClass(class_index)->who_name, history ? " *" : "  ",
			cabal_table[cabal].who_name,
			name,
			title ? title : "",
			extitle ? extitle : "");
		send_to_char(buf2, ch);

		sprintf(buf2, "  Align: %-16s Ethos: %-20s Sex:  %s\n\r",
			align < 0 ? "evil" : align == 0 ? "neutral" : align > 0 ? "good" : "(none)",
			ethos < 0 ? "chaotic" : ethos == 0 ? "neutral" : ethos > 0 ? "lawful" : "(none)",
			pc_race_table[race].name);
		send_to_char(buf2, ch);

		sprintf(buf3, "%d (b%d)", time_info.year - born, born);
		sprintf(buf1, "%d (d%d)", played / 3600, dtime);
		auto buffer2 = fmt::sprintf("  Hours: %-16s Age:   %-20s Objs: %d (%d L)\n\r", buf1, buf3, aobj, lobj);
		send_to_char(buffer2.c_str(), ch);

		sprintf(buf3, "%s%s%s%s",
			IS_SET(act, PLR_HEROIMM) ? " heroimm" : "",
			IS_SET(act, PLR_EMPOWERED) ? " empowered" : "",
			IS_SET(act, PLR_BETRAYER) ? " betrayer" : "",
			IS_SET(act, PLR_MORON) ? " moron" : "");

		if (!str_cmp(buf3, ""))
			sprintf(buf3, " none");

		buffer2 = fmt::sprintf("  Town:  %-16s Flag: %-21s Room: %d\n\r",
			hometown ? hometown_table[hometown].name : "(none)",
			buf3,
			room);
		send_to_char(buffer2.c_str(), ch);

		sprintf(buf3, "%dk/%dd", kills, pkilled);
		buffer2 = fmt::sprintf("  PKr:   %-16s ", buf3);
		send_to_char(buffer2.c_str(), ch);

		sprintf(buf3, "%dg, %dn, %de", gkills, nkills, ekills);
		buffer2 = fmt::sprintf("PKs:   %-20s MOBd: %d\n\r", buf3, mkilled);
		send_to_char(buffer2.c_str(), ch);

		if (sect_time[0] > 0)
		{
			sectval[0] = sect_time[SECT_CITY] + sect_time[SECT_INSIDE] + sect_time[SECT_ROAD];
			perc[0] = (((float)sectval[0] / (float)sect_time[0]) * (float)100);
			sectval[1] = sect_time[SECT_FIELD] + sect_time[SECT_FOREST] + sect_time[SECT_HILLS] + sect_time[SECT_MOUNTAIN] + sect_time[SECT_DESERT];
			perc[1] = (((float)sectval[1] / (float)sect_time[0]) * (float)100);
			perc[2] = 100 - perc[1] - perc[0];
			sprintf(buf2, "  Sect:  Civilized %2.1f%% %sWilderness %2.1f%%             Other %2.1f%%\n\r",
				perc[0],
				perc[0] > 99.9 ? "" : perc[0] < 10 ? "  " : " ",
				perc[1],
				perc[2]);
			send_to_char(buf2, ch);
		}

		if (souls > 1)
		{
			sprintf(buf97, "  Souls: %d\n\r", souls);
			send_to_char(buf97, ch);
		}

		send_to_char("\n\rLOGIN INFO:\n\r", ch);

		sprintf(buf2, "  Last:  %s (%d minutes played)\n\r", login, timeplayed);
		send_to_char(buf2, ch);

		// dev
		show_database_info(ch, arg1);

		if (tbuf)
			free_pstring(tbuf);
	}
}

void update_pc_last_fight(CHAR_DATA *ch, CHAR_DATA *ch2)
{
	if (is_npc(ch) || is_npc(ch2) || ch == ch2)
		return;

	ch->last_fight_time = current_time;
	ch->last_fight_name = ch2->true_name;

	ch2->last_fight_time = current_time;
	ch2->last_fight_name = ch->true_name;
}

/* horrid Cabal track code */
void do_ctrack(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_STRING_LENGTH];
	char buf[MAX_STRING_LENGTH];
	char buf2[MAX_STRING_LENGTH];
	char newbuf[MAX_STRING_LENGTH];
	char results[MAX_STRING_LENGTH];
	char *login = nullptr;
	FILE *fpChar;
	FILE *fpChar2;
	int numMatches = 0, cabal, counter;
	BUFFER *output;

	one_argument(argument, arg);

	output = new_buf();
	return;

	if (arg[0] == '\0')
	{
		send_to_char("Syntax: ctrack: <cabal name>\n\r", ch);
		send_to_char("Tracks members of a certain cabal and their last login.\n\r", ch);
		return;
	}

	if ((cabal = cabal_lookup(arg)) == 0)
	{
		send_to_char("No such cabal exists.\n\r", ch);
		return;
	}

	sprintf(arg, "%s", lowstring(arg));
	sprintf(buf, "grep 'Cabal %s~' %s/%s > %s", arg, RIFT_PLAYER_DIR, "*.plr", TEMP_GREP_RESULTS);

	auto returnCode = system(buf);
	if(returnCode != 0) // grep returns 0 on SUCCESS, > 0 on ERROR. system returns -1 on ERROR
		bug("Command [%s] failed with exit code [%d]", buf, returnCode);

	fpChar = fopen(TEMP_GREP_RESULTS, "r");
	if (fpChar == nullptr)
	{
		send_to_char("Error opening results.\n\r", ch);
		return;
	}

	while (fgets(results, MAX_INPUT_LENGTH, fpChar))
	{
		free_pstring(newbuf);

		for (counter = 0; counter < (int)(strlen(results) - 10); counter++)
		{
			if (results[counter + 10] == '.')
			{
				newbuf[counter] = '\0';
				break;
			}

			newbuf[counter] = results[counter + 10];
		}

		sprintf(buf2, "%s/%s.plr", RIFT_PLAYER_DIR, newbuf);
		fpChar2 = fopen(buf2, "r");
		login = get_login(ch, fpChar2);
		close(*((int *)fpChar2));

		numMatches++;

		sprintf(buf, "%3d) %s: %s\n\r", numMatches, newbuf, login ? login : "(none)");
		add_buf(output, buf);

		free_pstring(results);
	}

	fclose(fpChar);

	if (!numMatches)
	{
		sprintf(buf, "No players found in %s.\n\r", arg);
		send_to_char(buf, ch);
	}
	else
	{
		sprintf(buf, "%d players found in %s:\n\r", numMatches, arg);
		send_to_char(buf, ch);
		page_to_char(buf_string(output), ch);
	}

	free_buf(output);
}

char *get_login(CHAR_DATA *ch, FILE *fpChar2)
{
	bool fMatch, end = false;
	char *login = nullptr, *word;

	if (fpChar2 == nullptr)
	{
		send_to_char("Error opening pfile.\n\r", ch);
		return "";
	}

	for (;;)
	{
		fMatch = false;
		word = feof(fpChar2) ? (char *)"End" : fread_word(fpChar2);

		switch (UPPER(word[0]))
		{
			case 'E':
				if (!str_cmp(word, "End"))
				{
					end = true;
				}
				break;
			case 'L':
				KEY("LogonTime", login, fread_string(fpChar2))
				break;
		}

		if (!fMatch)
			fread_to_eol(fpChar2);

		if (end)
			break;
	}

	return login;
}

void chomp(char *str)
{
	for (; *str != '\0'; str++)
	{
		if (*str == '\n' || *str == '\r')
			*str = '\0';
	}
}

void chompr(char *str)
{
	for (; *str != '\0'; str++)
	{
		if (*str == '\r')
			*str = '\0';
	}
}

void chop(char *str)
{
	str[strlen(str) - 1] = '\0';
}

bool can_live_in(CHAR_DATA *ch, int hometown)
{
	if (hometown > MAX_HOMETOWN || hometown <= 0)
		return false;

	if (hometown_table[hometown].align == ALIGN_NONE
		|| hometown_table[hometown].align == ALIGN_GN && is_evil(ch)
		|| hometown_table[hometown].align == ALIGN_NE && is_good(ch)
		|| hometown_table[hometown].align == ALIGN_G && !is_good(ch)
		|| hometown_table[hometown].align == ALIGN_N && !is_neutral(ch)
		|| hometown_table[hometown].align == ALIGN_E && !is_evil(ch)
		|| hometown_table[hometown].align == ALIGN_GE && is_neutral(ch))
	{
		return false;
	}

	if (hometown_table[hometown].ethos == ETHOS_NONE
		|| hometown_table[hometown].ethos == ETHOS_LN && is_chaotic(ch)
		|| hometown_table[hometown].ethos == ETHOS_NC && is_lawful(ch)
		|| hometown_table[hometown].ethos == ETHOS_L && !is_lawful(ch)
		|| hometown_table[hometown].ethos == ETHOS_N && !is_eneutral(ch)
		|| hometown_table[hometown].ethos == ETHOS_C && !is_chaotic(ch)
		|| hometown_table[hometown].ethos == ETHOS_LC && is_eneutral(ch))
	{
		return false;
	}

	return true;
}

int count_carried(CHAR_DATA *ch, bool limited)
{
	OBJ_DATA *obj;
	int count = 0;

	for (obj = ch->carrying; obj != nullptr; obj = obj->next_content)
	{
		if (limited && obj->pIndexData->limtotal <= 0)
			continue;

		count++;
	}

	return count;
}

bool auto_check_multi(DESCRIPTOR_DATA *d_check, char *host)
{
	for (DESCRIPTOR_DATA *d = descriptor_list; d != nullptr; d = d->next)
	{
		if (d == d_check || d->character == nullptr)
			continue;

		if (!str_cmp(host, d->host))
			return true;
	}

	return false;
}

void do_pload(CHAR_DATA *ch, char *argument)
{
	DESCRIPTOR_DATA *d;
	char name[MAX_STRING_LENGTH];
	CHAR_DATA *victim;

	argument = one_argument(argument, name);

	if (name[0] == '\0' || argument[0] == '\0')
	{
		send_to_char("Syntax: pload <char> <command>\n\r", ch);
		return;
	}

	name[0] = UPPER(name[0]);

	if (get_char_world(ch, name) != nullptr)
	{
		send_to_char("That character is already online!\n\r", ch);
		return;
	}

	d = new_descriptor();

	if (!load_char_obj(d, name))
	{
		send_to_char("No such character exists.\n\r", ch);
		return;
	}

	auto buffer = fmt::format("cp {}{}{} {}pload.txt", RIFT_PLAYER_DIR, name, ".plr", RIFT_PLAYER_DIR);

	auto returnCode = system(buffer.c_str());
	if(returnCode != 0) // cp returns 0 on SUCCESS, 1 on ERROR. system returns -1 on ERROR
		bug("Command [%s] failed with exit code [%d]", buffer.data(), returnCode);

	d->character->desc = nullptr;
	d->character->next = char_list;

	char_list = d->character;

	d->outsize = 2000;
	d->outbuf = new char[d->outsize];
	d->connected = CON_PLAYING;

	reset_char(d->character);

	victim = d->character;

	d->character->pcdata->host = palloc_string("PLOAD");

	interpret(ch, argument);

	buffer = fmt::format("{}/{}{}", RIFT_PLAYER_DIR, name, ".plr");

	if (fopen(buffer.c_str(), "r") != nullptr)
	{
		save_char_obj(victim);
		extract_char(victim, true);
	}

	free_descriptor(d);
}

void do_damage(CHAR_DATA *ch, char *argument)
{
	int dam = 0;
	char arg1[MAX_STRING_LENGTH];
	char arg2[MAX_STRING_LENGTH];
	CHAR_DATA *victim;

	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);

	if (arg1[0] == '\0' || arg2[0] == '\0' || argument[0] == '\0')
	{
		send_to_char("Syntax: damage <char> <damage> <noun>\n\r", ch);
		return;
	}

	if ((victim = get_char_room(ch, arg1)) == nullptr)
	{
		send_to_char("They aren't here.\n\r", ch);
		return;
	}

	if (!is_number(arg2))
	{
		send_to_char("Damage must be numerical.\n\r", ch);
		return;
	}

	dam = atoi(arg2);

	damage_new(ch, victim, dam, TYPE_UNDEFINED, DAM_OTHER, true, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, argument);

	if (ch->fighting == victim)
		stop_fighting(ch, false);

	if (victim->fighting == ch)
		stop_fighting(victim, false);
}

void zone_echo(AREA_DATA *area, char *echo)
{
	char buffer[MSL * 2];

	for (DESCRIPTOR_DATA *d = descriptor_list; d; d = d->next)
	{
		if (d->connected == CON_PLAYING && d->character->in_room != nullptr && d->character->in_room->area == area)
		{
			colorconv(buffer, echo, d->character);
			send_to_char(buffer, d->character);
			send_to_char("\n\r", d->character);
		}
	}
}

bool old_is_adjacent_area(AREA_DATA *area, AREA_DATA *area2)
{
	ROOM_INDEX_DATA *to_room;

	for (auto room = room_list; room != nullptr; room = room->next_room)
	{
		if (room->area != area)
			continue;

		for (auto exit : room->exit)
		{
			if (exit == nullptr)
				continue;

			to_room = exit->u1.to_room;

			if (to_room != nullptr && to_room->area == area2)
				return true;
		}
	}

	return false;
}

bool is_adjacent_area(AREA_DATA *area, AREA_DATA *area2)
{
	for (auto adjacent : area->adjacent)
	{
		if (adjacent == area2)
			return true;
	}

	return false;
}

bool is_adj_range(AREA_DATA *area, AREA_DATA *area2, int range)
{
	if (area == area2)
		return true;

	for (int i = 0; i < MAX_ADJACENT; i++)
	{
		if (range > 1 && area != nullptr && area->adjacent[i] != nullptr)
		{
			range -= 1;
			if (is_adj_range(area->adjacent[i], area2, range))
				return true;
		}

		if (area->adjacent[i] == area2)
			return true;
	}

	return false;
}
