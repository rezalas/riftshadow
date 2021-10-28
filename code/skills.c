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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <algorithm>
#include "skills.h"
#include "handler.h"
#include "magic.h"
#include "recycle.h"
#include "tables.h"
#include "update.h"
#include "devextra.h"
#include "act_comm.h"
#include "chardef.h"
#include "const.h"
#include "utility.h"
#include "db.h"
#include "comm.h"
#include "interp.h"

/* used to get new skills */
void do_gain(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *trainer;

	if (is_npc(ch))
		return;

	/* find a trainer */
	for (trainer = ch->in_room->people; trainer != NULL; trainer = trainer->next_in_room)
	{
		if (is_npc(trainer) && (IS_SET(trainer->act, ACT_GAIN) || IS_SET(trainer->act, ACT_TRAIN)))
			break;
	}

	if (trainer == NULL || !can_see(ch, trainer))
	{
		send_to_char("You can't do that here.\n\r", ch);
		return;
	}

	one_argument(argument, arg);

	if (arg[0] == '\0')
	{
		do_say(trainer, "Use gain convert to convert 10 practices into 1 train.");
		do_say(trainer, "Or use gain revert to revert 1 train into 10 practices.");
		return;
	}

	if (!str_prefix(arg, "convert"))
	{
		if (ch->practice < 10)
		{
			act("$N tells you 'You are not yet ready.'", ch, NULL, trainer, TO_CHAR);
			return;
		}

		act("$N helps you apply your practice to training", ch, NULL, trainer, TO_CHAR);
		ch->practice -= 10;
		ch->train += 1;
		return;
	}

	/* 'gain revert' converts one train into 10 pracs */
	if (!str_prefix(arg, "revert"))
	{
		if (ch->train < 1)
		{
			act("$N tells you 'You do not have the training to apply to your skill practices yet.'", ch, NULL, trainer, TO_CHAR);
			return;
		}

		act("$N helps you apply your training to skills practices.", ch, NULL, trainer, TO_CHAR);
		ch->practice += 10;
		ch->train -= 1;
		return;
	}

	act("$N tells you 'I do not understand...'", ch, NULL, trainer, TO_CHAR);
}

/* RT spells and skills show the players spells (or skills) */

void do_spells(CHAR_DATA *ch, char *argument)
{
	BUFFER *buffer;
	char arg[MAX_INPUT_LENGTH];
	char spell_list[LEVEL_HERO + 1][MAX_STRING_LENGTH];
	char spell_columns[LEVEL_HERO + 1];
	int sn, level, min_lev = 1, max_lev = LEVEL_HERO, mana;
	bool fAll= false, found= false;
	char buf[MAX_STRING_LENGTH];

	if (is_npc(ch))
		return;

	if (ch->Class()->ctype != CLASS_CASTER)
	{
		send_to_char("Your class knows no spells.\n\r", ch);
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
		spell_columns[level] = 0;
		spell_list[level][0] = '\0';
	}

	for (sn = 0; sn < MAX_SKILL; sn++)
	{
		if (skill_table[sn].name == NULL)
			break;

		if ((level = skill_table[sn].skill_level[ch->Class()->GetIndex()]) < LEVEL_HERO + 1
			&& level >= min_lev
			&& level <= max_lev
			&& skill_table[sn].spell_fun != spell_null
			&& ch->pcdata->learned[sn] > 0
			&& (skill_table[sn].ctype == CMD_SPELL || skill_table[sn].ctype == CMD_BOTH))
		{
			found = true;
			level = skill_table[sn].skill_level[ch->Class()->GetIndex()];

			if (ch->level < level)
			{
				sprintf(buf, "%-18s n/a      ", skill_table[sn].name);
			}
			else
			{
				mana = std::max((int)skill_table[sn].min_mana, 100 / (2 + ch->level - level));
				sprintf(buf, "%-18s  %3d mana  ", skill_table[sn].name, mana);
			}

			if (spell_list[level][0] == '\0')
			{
				sprintf(spell_list[level], "\n\rLevel %2d: %s", level, buf);
			}
			else /* append */
			{
				if (++spell_columns[level] % 2 == 0)
					strcat(spell_list[level], "\n\r          ");

				strcat(spell_list[level], buf);
			}
		}
	}

	/* return results */

	if (!found)
	{
		send_to_char("No spells found.\n\r", ch);
		return;
	}

	buffer = new_buf();

	for (level = 0; level < LEVEL_HERO + 1; level++)
	{
		if (spell_list[level][0] != '\0')
			add_buf(buffer, spell_list[level]);
	}

	add_buf(buffer, "\n\r");
	page_to_char(buf_string(buffer), ch);
	free_buf(buffer);
}

void do_skills(CHAR_DATA *ch, char *argument)
{
	BUFFER *buffer;
	char arg[MAX_INPUT_LENGTH];
	char skill_list[LEVEL_HERO + 1][MAX_STRING_LENGTH];
	char skill_columns[LEVEL_HERO + 1];
	int sn, level, min_lev = 1, max_lev = LEVEL_HERO, p, gn, style_skill;
	bool fAll= false, found= false, hide_skill= false;
	char buf[MAX_STRING_LENGTH];

	if (is_npc(ch))
		return;

	if (argument[0] != '\0')
	{
		fAll = true;

		argument = one_argument(argument, arg);

		if (is_number(arg))
		{
			if (!is_number(arg))
			{
				send_to_char("Arguments must be numerical or name of skill.\n\r", ch);
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
					send_to_char("Arguments must be numerical or name of skill.\n\r", ch);
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
		else
		{
			if ((sn = skill_lookup(arg)) < 0)
			{
				return send_to_char("No such skill or spell.\n\r", ch);
			}

			if (ch->pcdata->learned[sn] == 0
				|| ch->level < skill_table[sn].skill_level[ch->Class()->GetIndex()]
				|| ((sn == gsn_gladiator_skill
							|| sn == gsn_barbarian_skill
							|| sn == gsn_duelist_skill
							|| sn == gsn_skirmisher_skill
							|| sn == gsn_dragoon_skill
							|| sn == gsn_tactician_skill)
						&& ch->level < AVATAR))
			{
				send_to_char("You are not learned in that.\n\r", ch);
			}
			else
			{
				sprintf(buf, "Your skill in '%s' is %d%%.\n\r", skill_table[sn].name, ch->pcdata->learned[sn]);
				send_to_char(buf, ch);
			}

			return;
		}
	}

	/* initialize data */
	for (level = 0; level < LEVEL_HERO + 1; level++)
	{
		skill_columns[level] = 0;
		skill_list[level][0] = '\0';
	}

	for (sn = 0; sn < MAX_SKILL; sn++)
	{
		if (skill_table[sn].name == NULL)
			break;

		hide_skill= false;
		/*
		if ((level = skill_table[sn].skill_level[ch->Class()->GetIndex()]) < LEVEL_HERO + 1
			&&  (fAll || level <= ch->level)
			&&  level >= min_lev
			&& level <= max_lev
			&&  skill_table[sn].spell_fun == spell_null
			&&  ch->pcdata->learned[sn] > 0)
		*/
		if ((level = skill_table[sn].skill_level[ch->Class()->GetIndex()]) < LEVEL_HERO + 1
			&& level >= min_lev
			&& level <= max_lev
			&& skill_table[sn].spell_fun == spell_null
			&& ch->pcdata->learned[sn] > 0)
		{
			for (p = 0; p < MAX_STYLE_SKILL; p++)
			{
				if (!str_prefix(style_percent[p].name, skill_table[sn].name))
				{
					gn = gn_skill_lookup(sn);

					if (gn > 1)
					{
						style_skill = skill_lookup(group_table[gn].name);

						if (ch->pcdata->learned[style_skill] < style_percent[p].percent)
							hide_skill = true;

						if (hide_skill)
							break;
					}
				}
			}

			if (sn == gsn_gladiator_skill
				|| sn == gsn_duelist_skill
				|| sn == gsn_barbarian_skill
				|| sn == gsn_dragoon_skill
				|| sn == gsn_skirmisher_skill
				|| sn == gsn_tactician_skill)
			{
				hide_skill = true;
			}

			found = true;
			level = skill_table[sn].skill_level[ch->Class()->GetIndex()];

			if (!hide_skill)
			{
				if (ch->level < level)
					sprintf(buf, "%-18s n/a      ", skill_table[sn].name);
				else
					sprintf(buf, "%-18s %3d%%      ", skill_table[sn].name, ch->pcdata->learned[sn]);

				if (skill_list[level][0] == '\0')
				{
					sprintf(skill_list[level], "\n\rLevel %2d: %s", level, buf);
				}
				else /* append */
				{
					if (++skill_columns[level] % 2 == 0)
						strcat(skill_list[level], "\n\r          ");

					strcat(skill_list[level], buf);
				}
			}
		}
	}

	/* return results */

	if (!found)
	{
		send_to_char("No skills found.\n\r", ch);
		return;
	}

	buffer = new_buf();

	for (level = 0; level < LEVEL_HERO + 1; level++)
	{
		if (skill_list[level][0] != '\0')
			add_buf(buffer, skill_list[level]);
	}

	add_buf(buffer, "\n\r");
	page_to_char(buf_string(buffer), ch);
	free_buf(buffer);
}

/* shows skills, groups and costs (only if not bought) */
void list_group_costs(CHAR_DATA *ch)
{
	return;
}

void list_group_chosen(CHAR_DATA *ch)
{
	if (is_npc(ch))
		return;

	return;
}

int exp_per_level(CHAR_DATA *ch)
{
	float epl;

	if (is_npc(ch))
		return 1500;

	epl = pc_race_table[ch->race].xpadd + 1500;
	epl += ((float)ch->level - 1) * epl * .08;
	return (int)epl;
}

/* this procedure handles the input parsing for the skill generator */
/* commented out for being lame
bool parse_gen_groups(CHAR_DATA *ch,char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	char buf[100];
	int gn,sn,i;

	if (argument[0] == '\0')
		return false;

	argument = one_argument(argument,arg);

	if (!str_prefix(arg,"help"))
	{
		if (argument[0] == '\0')
		{
			do_help(ch,"group help");
			return true;
		}

		do_help(ch,argument);
		return true;
	}

	if (!str_prefix(arg,"add"))
	{
		if (argument[0] == '\0')
		{
			send_to_char("You must provide a skill name.\n\r",ch);
			return true;
		}

		gn = group_lookup(argument);
		if (gn != -1)
		{
			if (ch->gen_data->group_chosen[gn] || ch->pcdata->group_known[gn])
			{
				send_to_char("You already know that group!\n\r",ch);
				return true;
			}

			sprintf(buf,"%s group added\n\r",group_table[gn].name);
			send_to_char(buf,ch);

			ch->gen_data->group_chosen[gn] = true;
			gn_add(ch,gn);
			return true;
		}

		sn = skill_lookup(argument);

		if (sn != -1)
		{
			if (ch->gen_data->skill_chosen[sn] || ch->pcdata->learned[sn] > 0)
			{
				send_to_char("You already know that skill!\n\r",ch);
				return true;
			}

			if (skill_table[sn].spell_fun != spell_null)
			{
				send_to_char("That skill is not available.\n\r",ch);
				return true;
			}

			sprintf(buf, "%s skill added\n\r",skill_table[sn].name);
			send_to_char(buf,ch);

			ch->gen_data->skill_chosen[sn] = true;
			ch->gen_data->points_chosen += skill_table[sn].rating[ch->Class()->GetIndex()];
			ch->pcdata->learned[sn] = 1;
			return true;
		}

		send_to_char("No skills or groups by that name...\n\r",ch);
		return true;
	}

	if (!strcmp(arg,"drop"))
	{
		if (argument[0] == '\0')
		{
			send_to_char("You must provide a skill to drop.\n\r",ch);
			return true;
		}

		gn = group_lookup(argument);
		if (gn != -1 && ch->gen_data->group_chosen[gn])
		{
			send_to_char("Group dropped.\n\r",ch);
			ch->gen_data->group_chosen[gn]= false;
			ch->gen_data->points_chosen -= group_table[gn].rating[ch->Class()->GetIndex()];
			gn_remove(ch,gn);

			for (i = 0; i < MAX_GROUP; i++)
			{
				if (ch->gen_data->group_chosen[gn])
					gn_add(ch,gn);
			}

			return true;
		}

		sn = skill_lookup(argument);

		if (sn != -1 && ch->gen_data->skill_chosen[sn])
		{
			send_to_char("Skill dropped.\n\r",ch);
			ch->gen_data->skill_chosen[sn]= false;
			ch->gen_data->points_chosen -= skill_table[sn].rating[ch->Class()->GetIndex()];
			ch->pcdata->learned[sn] = 0;
			return true;
		}

		send_to_char("You haven't bought any such skill or group.\n\r",ch);
		return true;
	}

	if (!str_prefix(arg,"premise"))
	{
		do_help(ch,"premise");
		return true;
	}

	if (!str_prefix(arg,"list"))
	{
		list_group_costs(ch);
		return true;
	}

	if (!str_prefix(arg,"learned"))
	{
		list_group_chosen(ch);
		return true;
	}

	if (!str_prefix(arg,"info"))
	{
		do_groups(ch,argument);
		return true;
	}

	return false;
}
*/

/* shows all groups, or the sub-members of a group */
void do_groups(CHAR_DATA *ch, char *argument)
{
	if (is_npc(ch))
		return;

	return;
}

/* check improve for styles */
void check_style_improve(CHAR_DATA *ch, int style, int multiplier)
{
	int chance, sn, skill, gn, i;
	char test[MSL];

	if (style == 0 || style >= 100)
		return;

	sn = skill_lookup(style_table[style].name);
	chance = 10 * int_app[get_curr_stat(ch, STAT_INT)].learn;
	chance /= (multiplier * 4);
	chance += ch->level * 2;

	if (number_range(1, 1000) > chance)
		return;

	ch->pcdata->learned[sn]++;

	if (ch->pcdata->learned[sn] > 100)
		ch->pcdata->learned[sn] = 100;

	/* check for newly learned style skills */
	for (i = 0; i < MAX_STYLE_SKILL; i++)
	{
		skill = skill_lookup(style_percent[i].name);

		if (skill > 0)
		{
			gn = gn_skill_lookup(skill);

			if (gn > 1)
			{
				if (!str_prefix(group_table[gn].name, style_table[style].name))
				{
					if (ch->pcdata->learned[sn] == style_percent[i].percent)
					{
						sprintf(test, "%sYou make a breakthrough in your understanding of the %s style!%s",
							get_char_color(ch, "lightyellow"),
							style_table[style].name,
							END_COLOR(ch));
						act(test, ch, 0, 0, TO_CHAR);

						sprintf(test, "You feel ready to learn the %s skill.", style_percent[i].name);
						act(test, ch, 0, 0, TO_CHAR);
					}
				}
			}
		}
	}
}

///
/// checks for skill improvement 
/// @param ch: The character with the skill
/// @param sn: The GSN of the skill used
/// @param success: Whether or not the use was successful
/// @param multiplier: A difficulty multiplier. Higher is harder to improve. Set between 1 and int[max]
///
void check_improve(CHAR_DATA *ch, int sn, bool success, int multiplier)
{
	int inum;
	float chance;
	char buf[100];

	if (is_npc(ch))
		return;

	if (is_affected_by(ch, AFF_CALM))
		return;

	if (ch->level < skill_table[sn].skill_level[ch->Class()->GetIndex()]
		|| ch->pcdata->learned[sn] <= 10
		|| ch->pcdata->learned[sn] >= 100)
	{
		return; /* skill is not known */
	}

	/* check to see if the character has a chance to learn */
	chance = 8 * int_app[get_curr_stat(ch, STAT_INT)].learn;
	chance /= (multiplier * 4);
	chance += ch->level * 2;

	if (IS_SET(ch->act, PLR_MORON))
		chance /= 4;

	if (is_affected(ch, gsn_synaptic_enhancement))
		chance *= 1.5;

	if (is_affected(ch, gsn_synaptic_impairment))
		chance /= 2;

	if (number_range(1, 1000) > chance)
		return;

	/* now that the character has a CHANCE to learn, see if they really have */

	if (success)
	{
		chance = URANGE(5, 100 - ch->pcdata->learned[sn], 95);

		if (number_percent() < chance)
		{
			if (IS_SET(ch->comm, COMM_ANSI) && ch->pcdata->learned[sn] < 99)
				sprintf(buf, "[1;33mYou have become better at %s![0m\n\r", skill_table[sn].name);
			else if (!IS_SET(ch->comm, COMM_ANSI) && ch->pcdata->learned[sn] < 99)
				sprintf(buf, "You have become better at %s!\n\r", skill_table[sn].name);
			else if (IS_SET(ch->comm, COMM_ANSI) && ch->pcdata->learned[sn] == 99)
				sprintf(buf, "[1;33mYou have perfected %s![0m\n\r", skill_table[sn].name);
			else if (!IS_SET(ch->comm, COMM_ANSI) && ch->pcdata->learned[sn] == 99)
				sprintf(buf, "You have perfected %s!\n\r", skill_table[sn].name);

			send_to_char(buf, ch);
			ch->pcdata->learned[sn]++;
			gain_exp(ch, 4);
		}
	}
	else
	{
		chance = URANGE(5, ch->pcdata->learned[sn] / 2, 30);
		inum = number_range(1, 3);

		if (number_percent() < chance)
		{
			if (IS_SET(ch->comm, COMM_ANSI) && (ch->pcdata->learned[sn] + inum) < 100)
				sprintf(buf, "[1;33mYou learn from your mistakes, and your %s skill improves.[0m\n\r", skill_table[sn].name);
			else if (!IS_SET(ch->comm, COMM_ANSI) && (ch->pcdata->learned[sn] + inum) < 100)
				sprintf(buf, "You learn from your mistakes, and your %s skill improves.\n\r", skill_table[sn].name);
			else if (IS_SET(ch->comm, COMM_ANSI) && ch->pcdata->learned[sn] + inum == 100)
				sprintf(buf, "[1;33mYou learn from your mistakes, and manage to perfect %s![0m\n\r", skill_table[sn].name);
			else if (!IS_SET(ch->comm, COMM_ANSI) && ch->pcdata->learned[sn] + inum == 100)
				sprintf(buf, "You learn from your mistakes, and manage to perfect %s!\n\r", skill_table[sn].name);

			send_to_char(buf, ch);
			ch->pcdata->learned[sn] += inum;
			ch->pcdata->learned[sn] = std::min((int)ch->pcdata->learned[sn], 100);
			gain_exp(ch, 4);
		}
	}
}

/* returns a group index number given the name */
int group_lookup(const char *name)
{
	for (int gn = 0; gn < MAX_GROUP; gn++)
	{
		if (group_table[gn].name == NULL)
			break;

		if (LOWER(name[0]) == LOWER(group_table[gn].name[0]) && !str_prefix(name, group_table[gn].name))
			return gn;
	}

	return -1;
}

int gn_skill_lookup(int sn)
{
	int i, j;

	for (i = 0; i < MAX_GROUP; i++)
	{
		if (group_table[i].name == NULL)
			break;

		for (j = 0; j < MAX_IN_GROUP; j++)
		{
			if (group_table[i].spells[j] == NULL)
				break;

			if (!str_prefix(skill_table[sn].name, group_table[i].spells[j]))
				return i;
		}
	}

	return -1;
}

bool style_check(int sn, int style)
{
	int gn;

	if ((gn = gn_skill_lookup(sn)) > 1)
	{
		if (str_cmp(group_table[gn].name, style_table[style].name))
			return false;
		else
			return true;
	}
	else
	{
		return false;
	}
}

bool group_skill_lookup(int gn, int sn)
{
	for (int i = 0; i < MAX_IN_GROUP; i++)
	{
		if (group_table[gn].spells[i] == NULL)
			break;

		if (sn < 1)
			break;

		if (str_cmp(skill_table[sn].name, group_table[gn].spells[i]))
			return true;
	}

	return false;
}

/* recursively adds a group given its number -- uses group_add */
void gn_add(CHAR_DATA *ch, int gn)
{
	ch->pcdata->group_known[gn] = true;

	for (int i = 0; i < MAX_IN_GROUP; i++)
	{
		if (group_table[gn].spells[i] == NULL)
			break;

		group_add(ch, group_table[gn].spells[i], false);
	}
}

/* recusively removes a group given its number -- uses group_remove */
void gn_remove(CHAR_DATA *ch, int gn)
{
	ch->pcdata->group_known[gn]= false;

	for (int i = 0; i < MAX_IN_GROUP; i++)
	{
		if (group_table[gn].spells[i] == NULL)
			break;

		group_remove(ch, group_table[gn].spells[i]);
	}
}

/* use for processing a skill or group for addition  */
void group_add(CHAR_DATA *ch, const char *name, bool deduct)
{
	if (is_npc(ch)) /* NPCs do not have skills */
		return;

	int sn = skill_lookup(name);

	if (sn != -1
		&& sn != gsn_gladiator_skill
		&& sn != gsn_duelist_skill
		&& sn != gsn_barbarian_skill
		&& sn != gsn_dragoon_skill
		&& sn != gsn_skirmisher_skill
		&& sn != gsn_tactician_skill)
	{
		if (ch->pcdata->learned[sn] == 0)
			ch->pcdata->learned[sn] = 1;

		return;
	}

	/* now check groups */
	int gn = group_lookup(name);

	if (gn != -1)
	{
		if (ch->pcdata->group_known[gn] == false)
			ch->pcdata->group_known[gn] = true;

		gn_add(ch, gn); /* make sure all skills in the group are known */
	}
}

/* used for processing a skill or group for deletion -- no points back! */

void group_remove(CHAR_DATA *ch, const char *name)
{
	int sn = skill_lookup(name);

	if (sn != -1)
	{
		ch->pcdata->learned[sn] = 0;
		return;
	}

	/* now check groups */

	int gn = group_lookup(name);

	if (gn != -1 && ch->pcdata->group_known[gn] == true)
	{
		ch->pcdata->group_known[gn]= false;
		gn_remove(ch, gn); /* be sure to call gn_add on all remaining groups */
	}
}
