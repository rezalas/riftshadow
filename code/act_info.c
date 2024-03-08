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
#include <ctype.h>
#include <time.h>

#ifdef _WIN32
	#include <windows.h>
	#include <wincrypt.h>
#else
	#include <crypt.h>
#endif

#include "act_info.h"
#include "rift.h"
#include "weather_enums.h"
#include "handler.h"
#include "magic.h"
#include "recycle.h"
#include "tables.h"
#include "lookup.h"
#include "act_comm.h"
#include "act_move.h"
#include "cabal.h"
#include "devextra.h"
#include "dioextra.h"
#include "fight.h"
#include "newmem.h"
#include "comm.h"
#include "act_wiz.h"
#include "update.h"
#include "interp.h"
#include "db.h"
#include "db2.h"
#include "characterClasses/zealot.h"
#include "help.h"
#include "misc.h"
#include "chardef.h"
#include "const.h"
#include "material.h"
#include "utility.h"
#include "skills.h"
#include "save.h"
#include "./include/spdlog/fmt/bundled/format.h"
#include "./include/spdlog/fmt/bundled/printf.h"

char *const where_name[] = {
	"<used as light>     ",
	"<worn on finger>    ",
	"<worn on finger>    ",
	"<worn around neck>  ",
	"<worn around neck>  ",
	"<worn on torso>     ",
	"<worn on head>      ",
	"<worn on legs>      ",
	"<worn on feet>      ",
	"<worn on hands>     ",
	"<worn on arms>      ",
	"<worn as shield>    ",
	"<worn on body>      ",
	"<worn on waist>     ",
	"<worn around wrist> ",
	"<worn around wrist> ",
	"<wielded>           ",
	"<held>              ",
	"<dual wielded>      ",
	"<marked>            ",
	"<strapped on>       ",
	"<cosmetic gear>     "
};

static char *const moon_look[LunarCyclePosition::MaxLunarCycles] = 
{
	"new",
	"crescent waxing",
	"half waxing",
	"gibbous waxing",
	"full",
	"gibbous waning",
	"half waning",
	"crescent waning"
};

static char *const sky_look[WeatherCondition::MaxWeatherCondition] = 
{
	"The sky is cloudless",
	"The sky is partly cloudy",
	"The sky is overcast",
	"A light drizzle falls from the sky",
	"A drenching rain pours from the sky",
	"The sky is lit by flashes of lightning",
	"A light flurry of snowflakes falls from the sky",
	"A ferocious blizzard blots out the sky",
	"Pebble-sized hailstones fall from the sky"
};

static char *const temp_look[Temperature::MaxTemperature] = 
{
	"hot",
	"warm",
	"cool",
	"cold"
};

static char *const wind_look[Windspeed::MaxWindspeed] = 
{
	"is perfectly still.", 
	"wafts on a gentle breeze.", 
	"swirls briskly.",
	 "whips across the area with gale force!"
};

/* for do_count */
int max_on = 0;

char *format_obj_to_char(OBJ_DATA *obj, CHAR_DATA *ch, bool fShort)
{
	static char buf[MAX_STRING_LENGTH];

	buf[0] = '\0';

	if ((fShort && (obj->short_descr == nullptr || obj->short_descr[0] == '\0'))
		|| (obj->description == nullptr || obj->description[0] == '\0'))
	{
		return buf;
	}

	if (is_affected_by(obj, AFF_OBJ_BURNING))
		strcat(buf, "(Burning) ");

	if (is_obj_stat(obj, ITEM_INVIS))
		strcat(buf, "(Invis) ");

	if (is_affected_by(ch, AFF_DETECT_EVIL) && is_obj_stat(obj, ITEM_EVIL))
		strcat(buf, "(Red Aura) ");

	if (is_affected_by(ch, AFF_DETECT_GOOD) && is_obj_stat(obj, ITEM_BLESS))
		strcat(buf, "(Blue Aura) ");

	if (is_affected_by(ch, AFF_DETECT_MAGIC) && is_obj_stat(obj, ITEM_MAGIC))
		strcat(buf, "(Magical) ");

	if (is_obj_stat(obj, ITEM_GLOW))
		strcat(buf, "(Glowing) ");

	if (is_obj_stat(obj, ITEM_DARK))
		strcat(buf, "(Dark) ");

	if (is_obj_stat(obj, ITEM_HUM))
		strcat(buf, "(Humming) ");

	if (is_affected_obj(obj, gsn_stash) && is_immortal(ch))
	{
		auto oaf = obj->affected;
		for (; oaf != nullptr; oaf = oaf->next)
		{
			if (oaf->type == gsn_stash)
				break;
		}

		sprintf(buf, "(Stashed by %s) ", oaf->owner->name);
	}

	if (is_obj_stat(obj, ITEM_NOSHOW) && is_immortal(ch))
		strcat(buf, "(Nonobvious) ");

	if (fShort)
	{
		if (obj->short_descr != nullptr)
			strcat(buf, obj->short_descr);
	}
	else
	{
		if (obj->description != nullptr)
			strcat(buf, obj->description);
	}

	return buf;
}

/*
 * Show a list to a character.
 * Can coalesce duplicated items.
 */
void show_list_to_char(OBJ_DATA *list, CHAR_DATA *ch, bool fShort, bool fShowNothing)
{
	if (ch->desc == nullptr)
		return;

	/*
	 * Alloc space for output lines.
	 */
	auto line = ch->lines;
	auto count = 0;

	for (auto obj = list; obj != nullptr; obj = obj->next_content)
	{
		count++;
	}

	if (count == 0)
		return;

	ch->lines = 0;

	auto output = new_buf();
	auto prgpstrShow = (char **)talloc_struct((count * sizeof(char *)));
	auto prgnShow = (int *)talloc_struct((count * sizeof(int)));
	auto nShow = 0;

	/*
	 * Format the list of objects.
	 */
	for (auto obj = list; obj != nullptr; obj = obj->next_content)
	{
		if (IS_SET(obj->extra_flags, ITEM_NOSHOW) && !is_immortal(ch))
			continue;

		if (obj->wear_loc == WEAR_NONE && can_see_obj(ch, obj))
		{
			auto pstrShow = format_obj_to_char(obj, ch, fShort);
			auto fCombine= false;

			if (is_npc(ch) || IS_SET(ch->comm, COMM_COMBINE))
			{
				/*
				 * Look for duplicates, case sensitive.
				 * Matches tend to be near end so run loop backwards.
				 */
				for (auto iShow = nShow - 1; iShow >= 0; iShow--)
				{
					if (!strcmp(prgpstrShow[iShow], pstrShow))
					{
						prgnShow[iShow]++;
						fCombine = true;
						break;
					}
				}
			}

			/*
			 * Couldn't combine, or didn't want to.
			 */
			if (!fCombine)
			{
				prgpstrShow[nShow] = talloc_string(pstrShow);
				prgnShow[nShow] = 1;
				nShow++;
			}
		}
	}

	/*
	 * Output the formatted list.
	 */
	for (auto iShow = 0; iShow < nShow; iShow++)
	{
		if (prgpstrShow[iShow][0] == '\0')
			continue;

		if (is_npc(ch) || IS_SET(ch->comm, COMM_COMBINE))
		{
			if (prgnShow[iShow] != 1)
			{
				char buf[MAX_STRING_LENGTH];
				sprintf(buf, "(%2d) ", prgnShow[iShow]);
				add_buf(output, buf);
			}
			else
			{
				add_buf(output, "     ");
			}
		}
		add_buf(output, prgpstrShow[iShow]);
		add_buf(output, "\n\r");
	}

	if (fShowNothing && nShow == 0)
	{
		if (is_npc(ch) || IS_SET(ch->comm, COMM_COMBINE))
			send_to_char("     ", ch);

		send_to_char("Nothing.\n\r", ch);
	}

	page_to_char(buf_string(output), ch);

	/*
	 * Clean up.
	 */
	free_buf(output);
	ch->lines = line;
}

void show_char_to_char_0(CHAR_DATA *victim, CHAR_DATA *ch)
{
	char buf[MAX_STRING_LENGTH];
	buf[0] = '\0';

	auto obj = get_eq_char(victim, WEAR_HEAD);

	auto sn_faerie_fog = skill_lookup("faerie fog");

	if (!is_npc(victim))
	{
		if (IS_SET(victim->comm, COMM_AFK))
			strcat(buf, "[AFK] ");

		if (victim->ghost)
			strcat(buf, "(Ghost) ");
	}

	if (is_affected_by(victim, AFF_INVISIBLE))
		strcat(buf, "(Invis) ");

	if (victim->invis_level >= LEVEL_HERO)
		strcat(buf, "(Wizi) ");

	if (is_affected_by(victim, AFF_HIDE))
		strcat(buf, "(Hide) ");

	if (is_affected_by(victim, AFF_CAMOUFLAGE))
		strcat(buf, "(Camouflage) ");

	if (is_affected_by(victim, AFF_CHARM) && (!is_npc(victim) || victim->pIndexData->vnum != 80))
		strcat(buf, "(Charmed) ");

	if (is_affected_by(victim, AFF_PASS_DOOR))
		strcat(buf, "(Translucent) ");

	if (is_affected(victim, gsn_faerie_fire))
		strcat(buf, "(Pink Aura) ");

	if (is_affected(victim, gsn_mana_sickness))
		strcat(buf, "(Glowing) ");

	if (is_affected(victim, gsn_incandescense))
		strcat(buf, "(Glowing) ");

	if (is_affected(victim, sn_faerie_fog))
		strcat(buf, "(Purple Aura) ");

	if (is_evil(victim) && is_affected_by(ch, AFF_DETECT_EVIL))
		strcat(buf, "(Red Aura) ");

	if (is_good(victim) && is_affected_by(ch, AFF_DETECT_GOOD))
		strcat(buf, "(Golden Aura) ");

	if (is_affected_by(ch, AFF_DETECT_MAGIC))
	{
		auto spellaffs = 0;
		for (auto af = victim->affected; af; af = af->next)
		{
			if (af->aftype == AFT_SPELL)
				spellaffs++;
		}

		if (spellaffs >= 20)
		{
			strcat(buf, "(Blinding Aura) ");
		}
		else if (spellaffs >= 15)
		{
			strcat(buf, "(Blazing Aura) ");
		}
		else if (spellaffs >= 10)
		{
			strcat(buf, "(Luminous Aura) ");
		}
		else if (spellaffs >= 6)
		{
			strcat(buf, "(Bright Aura) ");
		}
		else if (spellaffs >= 3)
		{
			strcat(buf, "(Flickering Aura) ");
		}
		else if (spellaffs >= 1)
		{
			strcat(buf, "(Dim Aura) ");
		}
	}

	if (is_affected_by(victim, AFF_NOSHOW))
	{
		if (IS_SET(victim->act, ACT_NOCTURNAL))
			strcat(buf, "(Nocturnal) ");
		else
			strcat(buf, "(Diurnal) ");
	}

	if (is_affected_by(victim, AFF_SANCTUARY))
	{
		if (is_evil(victim))
			strcat(buf, "(Dark Aura) ");
		else
			strcat(buf, "(White Aura) ");
	}

	if (is_affected(victim, gsn_cloak_of_mist))
		strcat(buf, "(Cloaked in Mist) ");

	if (is_affected(victim, gsn_sanguine_ward))
		strcat(buf, "(Crimson Mist) ");

	if (is_affected(victim, gsn_snare))
		strcat(buf, "(Ensnared) ");

	if (is_affected(victim, gsn_blade_barrier))
		strcat(buf, "(Blade Barrier) ");

	if (is_affected(victim, gsn_hold_person))
		strcat(buf, "(Rigid) ");

	auto af = affect_find(victim->affected, gsn_creeping_tomb);
	if (is_affected(victim, gsn_creeping_tomb) && (af != nullptr && af->duration <= 2))
		strcat(buf, "(Entombed) ");

	if (!is_npc(victim) && victim->pcdata->energy_state == -5)
		strcat(buf, "(Frozen) ");

	if (is_affected(victim, gsn_earthfade))
		strcat(buf, "(Earthfaded) ");

	if (is_affected(victim, gsn_ultradiffusion))
		strcat(buf, "(Ultradiffuse) ");

	if (is_affected(victim, gsn_watermeld))
		strcat(buf, "(Watermeld) ");

	if (is_affected(victim, gsn_sphere_of_plasma))
		strcat(buf, "(Plasma Sphere) ");

	if (is_affected(victim, gsn_deny_magic))
		strcat(buf, "(Blue Aura) ");

	if ((is_npc(victim) || is_shifted(victim) || (str_cmp(victim->long_descr, "") && victim->long_descr[0] != '\0')) && !is_affected(ch, gsn_plasma_arc))
	{
		strcat(buf, victim->long_descr);
		send_to_char(buf, ch);
		return;
	}

	strcat(buf, get_descr_form(victim, ch, false));

	if (!is_npc(victim) && !IS_SET(ch->comm, COMM_BRIEF))
		strcat(buf, victim->pcdata->title);

	char message[MAX_STRING_LENGTH];
	switch (victim->position)
	{
		case POS_DEAD:
			strcat(buf, " is DEAD!!");
			break;
		case POS_MORTAL:
			strcat(buf, " is mortally wounded.");
			break;
		case POS_INCAP:
			strcat(buf, " is incapacitated.");
			break;
		case POS_STUNNED:
			strcat(buf, " is lying here stunned.");
			break;
		case POS_SLEEPING:
			if (victim->on != nullptr)
			{
				if (IS_SET_OLD(victim->on->value[2], SLEEP_AT))
				{
					sprintf(message, " is here, sleeping at %s.", victim->on->short_descr);
					strcat(buf, message);
				}
				else if (IS_SET_OLD(victim->on->value[2], SLEEP_ON))
				{
					sprintf(message, " is here, sleeping on %s.", victim->on->short_descr);
					strcat(buf, message);
				}
				else
				{
					sprintf(message, " is here, sleeping in %s.", victim->on->short_descr);
					strcat(buf, message);
				}
			}
			else
			{
				strcat(buf, " is sleeping here.");
			}
			break;
		case POS_RESTING:
			if (victim->on != nullptr)
			{
				if (IS_SET_OLD(victim->on->value[2], REST_AT))
				{
					sprintf(message, " is here, resting at %s.", victim->on->short_descr);
					strcat(buf, message);
				}
				else if (IS_SET_OLD(victim->on->value[2], REST_ON))
				{
					sprintf(message, " is here, resting on %s.", victim->on->short_descr);
					strcat(buf, message);
				}
				else if (IS_SET_OLD(victim->on->value[2], LOUNGE_ON))
				{
					sprintf(message, " is here, lounging on %s.", victim->on->short_descr);
					strcat(buf, message);
				}
				else
				{
					sprintf(message, " is here, resting in %s.", victim->on->short_descr);
					strcat(buf, message);
				}
			}
			else
			{
				strcat(buf, " is resting here.");
			}
			break;
		case POS_SITTING:
			if (victim->on != nullptr)
			{
				if (IS_SET_OLD(victim->on->value[2], SIT_AT))
				{
					sprintf(message, " is here, sitting at %s.", victim->on->short_descr);
					strcat(buf, message);
				}
				else if (IS_SET_OLD(victim->on->value[2], SIT_ON))
				{
					sprintf(message, " is here, sitting on %s.", victim->on->short_descr);
					strcat(buf, message);
				}
				else
				{
					sprintf(message, " is here, sitting in %s.", victim->on->short_descr);
					strcat(buf, message);
				}
			}
			else
			{
				strcat(buf, " is sitting here.");
			}
			break;
		case POS_STANDING:
			if (victim->on != nullptr)
			{
				if (IS_SET_OLD(victim->on->value[2], STAND_AT))
				{
					sprintf(message, " is here, standing at %s.", victim->on->short_descr);
					strcat(buf, message);
				}
				else if (IS_SET_OLD(victim->on->value[2], STAND_ON))
				{
					sprintf(message, " is here, standing on %s.", victim->on->short_descr);
					strcat(buf, message);
				}
				else
				{
					sprintf(message, " is here, standing in %s.", victim->on->short_descr);
					strcat(buf, message);
				}
			}
			else
			{
				strcat(buf, " is here.");
			}
			break;
		case POS_FIGHTING:
			strcat(buf, " is here, fighting ");

			if (victim->fighting == nullptr)
			{
				strcat(buf, "thin air??");
			}
			else if (victim->fighting == ch)
			{
				strcat(buf, "YOU!");
			}
			else if (victim->in_room == victim->fighting->in_room)
			{
				strcat(buf, pers(victim->fighting, ch));
				strcat(buf, ".");
			}
			else
			{
				strcat(buf, "someone who left??");
			}
			break;
	}

	strcat(buf, "\n\r");
	buf[0] = UPPER(buf[0]);

	send_to_char(buf, ch);
}

void show_char_to_char_1(CHAR_DATA *victim, CHAR_DATA *ch)
{
	char buf[MAX_STRING_LENGTH], buf2[MAX_STRING_LENGTH], buf3[MAX_STRING_LENGTH];

	if (victim->description[0] != '\0')
		send_to_char(victim->description, ch);
	else
		act("You see nothing special about $M.", ch, nullptr, victim, TO_CHAR);

	auto percent = victim->max_hit > 0
		? (100 * victim->hit) / victim->max_hit
		: -1;

	strcpy(buf, get_descr_form(victim, ch, false));

	if (!is_npc(victim))
	{
		sprintf(buf2, ", a%s %s %s %s%s,",
				(victim->pcdata->beauty == 1 || victim->pcdata->beauty == 3 || victim->pcdata->beauty == 4) ? "n" : "",
				(victim->sex == 2) ? beauty_table[victim->pcdata->beauty].female
								   : beauty_table[victim->pcdata->beauty].male,
				sex_table[victim->sex].name, pc_race_table[victim->race].name,
				(!str_cmp(pc_race_table[victim->race].name, "celestial") ||
				 !str_cmp(pc_race_table[victim->race].name, "planar") ||
				 !str_cmp(pc_race_table[victim->race].name, "abyss"))
					? " titan"
					: "");

		strcat(buf, buf2);
	}

	sprintf(buf2, " %s\n", get_battle_condition(victim, percent));
	strcat(buf, buf2);
	buf[0] = UPPER(buf[0]);

	send_to_char(buf, ch);

	if (is_affected_by(victim, AFF_FLYING))
	{
		sprintf(buf, "%s$E is hovering in mid-air!%s", 
			get_char_color(ch, "white"),
			END_COLOR(ch));

		act(buf, ch, 0, victim, TO_CHAR);
	}

	if (is_affected(victim, gsn_corona))
	{
		sprintf(buf, "%s$E is surrounded by a faint corona of flickering flames!%s",
			get_char_color(ch, "lightred"),
			END_COLOR(ch));

		act(buf, ch, 0, victim, TO_CHAR);
	}

	if (is_affected(victim, gsn_frigidaura))
	{
		sprintf(buf, "%s$E is surrounded by a swirling aura of crackling frost!%s",
			get_char_color(ch, "lightblue"),
			END_COLOR(ch));

		act(buf, ch, 0, victim, TO_CHAR);
	}

	if (is_affected(victim, gsn_rotating_ward))
	{
		auto counter = 0;

		for (auto paf = victim->affected; paf; paf = paf->next)
		{
			if (paf->type == gsn_rotating_ward)
				counter++;
		}

		sprintf(buf, "%s%s rotating around $S body!%s",
			get_char_color(ch, "white"),
			counter == 1
				? "A multifaceted crystal is"
				: "Multifaceted crystals are",
			END_COLOR(ch));

		act(buf, ch, 0, victim, TO_CHAR);
	}

	if (is_affected(victim, gsn_cloak_form) && !is_immortal(ch))
		return;

	auto found = false;
	for (auto iWear = 0; iWear < MAX_WEAR; iWear++)
	{
		if (iWear == WEAR_WIELD) // cosmetics come right before "wield" on display
		{
			for (auto tObj = victim->carrying; tObj; tObj = tObj->next_content)
			{
				if (tObj->wear_loc != WEAR_COSMETIC)
					continue;

				auto worn_over = victim->carrying;
				for (; worn_over; worn_over = worn_over->next_content)
				{
					if (worn_over != tObj
						&& IS_SET(tObj->extra_flags, ITEM_UNDER_CLOTHES)
						&& is_worn(worn_over))
					{
						if (worn_over->wear_flags[0] + pow(2, ITEM_WEAR_COSMETIC) == tObj->wear_flags[0])
							break;
					}
				}

				if (worn_over)
					continue;

				if (!found)
					act("\n\r$N is using:", ch, nullptr, victim, TO_CHAR);

				auto buffer = fmt::format("{}>", tObj->wear_loc_name ? tObj->wear_loc_name : "bug: unset wear_loc on cosmetic");
				buffer = fmt::sprintf("<worn %-14s", buffer);

				send_to_char(buffer.c_str(), ch);

				if (can_see_obj(ch, tObj))
					send_to_char(format_obj_to_char(tObj, ch, true), ch);
				else
					send_to_char("something.", ch);

				send_to_char("\n\r", ch);
				found = true;
			}
		}

		auto obj = get_eq_char(victim, iWear);
		if (iWear != WEAR_COSMETIC && obj != nullptr && can_see_obj(ch, obj))
		{
			if (!found)
			{
				send_to_char("\n\r", ch);
				act("$N is using:", ch, nullptr, victim, TO_CHAR);
				found = true;
			}

			send_to_char(where_name[iWear], ch);
			send_to_char(format_obj_to_char(obj, ch, true), ch);
			send_to_char("\n\r", ch);
		}
	}

	auto belt = get_eq_char(victim, WEAR_WAIST);
	if (!is_npc(victim)
		&& victim->cabal == CABAL_HORDE
		&& belt != nullptr
		&& belt->pIndexData->vnum == OBJ_VNUM_TROPHY_BELT
		&& victim->pcdata->trophy && belt->value[4] >= 1)
	{
		auto placeholder = victim->pcdata->trophy;

		act("\n\r$p catches your eye, meriting closer examination.", ch, belt, 0, TO_CHAR);
		send_to_char(belt->extra_descr->description, ch);
		send_to_char("\n\rAttached to the belt are:\n\r", ch);

		for (auto i = 0; i < MAX_STRING_LENGTH; i++)
		{
			buf2[i] = '\0';
			buf3[i] = '\0';
		}

		for (auto counter = 1; counter <= belt->value[4]; counter++)
		{
			sprintf(buf2, "%s%s%c scalp of %s",
				counter > 1 ? ", " : "",
				counter % 4 == 0 ? "\n\r" : "",
				counter > 1 ? 'a' : 'A', victim->pcdata->trophy->victname);

			strcat(buf3, buf2);

			for (auto i = 0; i < MAX_STRING_LENGTH; i++)
			{
				buf2[i] = '\0';
			}

			if (counter >= belt->value[4])
				break;

			if (!victim->pcdata->trophy->next)
				break;

			victim->pcdata->trophy = victim->pcdata->trophy->next;
		}

		strcat(buf3, ".\n\r");

		send_to_char(buf3, ch);

		buf3[0] = '\0';
		victim->pcdata->trophy = placeholder;
	}

	if (victim != ch
		&& !is_npc(ch)
		&& (is_immortal(ch) || !is_immortal(victim))
		&& number_percent() < get_skill(ch, gsn_peek))
	{
		send_to_char("\n\rYou peek at the inventory:\n\r", ch);

		check_improve(ch, gsn_peek, true, 4);

		show_list_to_char(victim->carrying, ch, true, true);

		if (!is_npc(victim))
		{
			act("\n\rYou check for gold coins on $N's person:", ch, 0, victim, TO_CHAR);

			if (victim->gold <= 0)
			{
				act("$N does not seem to be carrying any gold.", ch, 0, victim, TO_CHAR);
			}
			else if (victim->gold < 10)
			{
				act("Scant few gold pieces jingle softly in $N's purse.", ch, 0, victim, TO_CHAR);
			}
			else if (victim->gold < 100)
			{
				act("$N's coinpurse appears fairly empty.", ch, 0, victim, TO_CHAR);
			}
			else if (victim->gold < 1000)
			{
				act("$N's coinpurse has a healthy bulge to it.", ch, 0, victim, TO_CHAR);
			}
			else if (victim->gold < 10000)
			{
				act("$N's coinpurse hangs heavily at $S waist, laden with gold.", ch, 0, victim, TO_CHAR);
			}
			else if (victim->gold < 100000)
			{
				act("$N's coinpurse bulges at the seams, overflowing with gold!", ch, 0, victim, TO_CHAR);
			}
			else
			{
				act("Not only does $N have the largest coinpurse you've ever laid eyes upon, but it can scarcely contain the veritable motherlode $E is toting around.  Thieves the world over are having wet dreams about $N at this very moment.", ch, 0, victim, TO_CHAR);
			}
		}
	}
}

void show_char_to_char_2(CHAR_DATA *victim, CHAR_DATA *ch)
{
	auto percent = victim->max_hit > 0
		? (100 * victim->hit) / victim->max_hit
		: -1;

	char buf[MAX_STRING_LENGTH];
	strcpy(buf, get_descr_form(victim, ch, false));

	if (!is_npc(victim))
	{
		char buf2[MAX_STRING_LENGTH];
		sprintf(buf2, ", a%s %s %s %s,",
				(victim->pcdata->beauty == 1 || victim->pcdata->beauty == 3 || victim->pcdata->beauty == 4) ? "n" : "",
				(victim->sex == 2) ? beauty_table[victim->pcdata->beauty].female
								   : beauty_table[victim->pcdata->beauty].male,
				sex_table[victim->sex].name, pc_race_table[victim->race].name);

		strcat(buf, buf2);
	}

	sprintf(buf, "%s %s", victim->name, get_battle_condition(victim, percent));
	buf[0] = UPPER(buf[0]);

	send_to_char(buf, ch);

	auto found = false;
	for (auto iWear = 0; iWear < MAX_WEAR; iWear++)
	{
		auto obj = get_eq_char(victim, iWear);
		if (obj != nullptr && can_see_obj(ch, obj))
		{
			if (!found)
			{
				send_to_char("\n\r", ch);
				act("$N is using:", ch, nullptr, victim, TO_CHAR);
				found = true;
			}

			send_to_char(where_name[iWear], ch);
			send_to_char(format_obj_to_char(obj, ch, true), ch);
			send_to_char("\n\r", ch);
		}
	}

	if (victim != ch
		&& !is_npc(ch)
		&& (is_immortal(ch) || !is_immortal(victim))
		&& number_percent() < get_skill(ch, gsn_peek))
	{
		send_to_char("\n\rYou peek at the inventory:\n\r", ch);
		check_improve(ch, gsn_peek, true, 4);
		show_list_to_char(victim->carrying, ch, true, true);
	}
}

void show_char_to_char_3(CHAR_DATA *victim, CHAR_DATA *ch)
{
	if (can_see(victim, ch))
	{
		if (ch == victim)
		{
			act("$n looks closely at $mself.", ch, nullptr, nullptr, TO_ROOM);
		}
		else
		{
			act("$n looks closely at you.", ch, nullptr, victim, TO_VICT);
			act("$n looks closely at $N.", ch, nullptr, victim, TO_NOTVICT);
		}
	}

	if (victim->description[0] != '\0')
		send_to_char(victim->description, ch);
	else
		act("You see nothing special about $M.", ch, nullptr, victim, TO_CHAR);

	auto percent = victim->max_hit > 0
		? (100 * victim->hit) / victim->max_hit
		: -1;

	char buf[MAX_STRING_LENGTH];
	strcpy(buf, get_descr_form(victim, ch, false));

	if (!is_npc(victim))
	{
		char buf2[MAX_STRING_LENGTH];
		sprintf(buf2, ", a%s %s %s %s,",
				(victim->pcdata->beauty == 1 || victim->pcdata->beauty == 3 || victim->pcdata->beauty == 4) ? "n" : "",
				(victim->sex == 2) ? beauty_table[victim->pcdata->beauty].female
								   : beauty_table[victim->pcdata->beauty].male,
				sex_table[victim->sex].name, pc_race_table[victim->race].name);

		strcat(buf, buf2);
	}

	sprintf(buf, "%s %s", victim->name, get_battle_condition(victim, percent));
	buf[0] = UPPER(buf[0]);

	send_to_char(buf, ch);
}

void show_char_to_char(CHAR_DATA *list, CHAR_DATA *ch)
{
	for (auto rch = list; rch != nullptr; rch = rch->next_in_room)
	{
		if (rch == ch)
			continue;

		if (get_trust(ch) < rch->invis_level)
			continue;

		if (is_npc(rch) && IS_SET(rch->act, ACT_WARD_MOB))
			continue;

		if (can_see(ch, rch))
			show_char_to_char_0(rch, ch);
	}
}

bool check_blind(CHAR_DATA *ch)
{
	if (!is_npc(ch) && IS_SET(ch->act, PLR_HOLYLIGHT))
		return true;

	if (is_affected_by(ch, AFF_BLIND))
	{
		send_to_char("You can't see a thing!\n\r", ch);
		return false;
	}

	if (is_affected_area(ch->in_room->area, gsn_whiteout) && is_outside(ch))
	{
		auto paf = ch->in_room->area->affected;
		for (; paf != nullptr; paf = paf->next)
		{
			if (paf->type == gsn_whiteout)
				break;
		}

		if (paf->owner != ch)
		{
			send_to_char("You can't see a thing through the snow!\n\r", ch);
			return false;
		}
	}

	return true;
}

/* changes your scroll */
void do_scroll(CHAR_DATA *ch, char *argument)
{
	char buf[100], arg[MAX_INPUT_LENGTH];
	one_argument(argument, arg);

	if (arg[0] == '\0')
	{
		if (ch->lines == 0)
		{
			send_to_char("You do not page long messages.\n\r", ch);
		}
		else
		{
			sprintf(buf, "You currently display %d lines per page.\n\r", ch->lines + 2);
			send_to_char(buf, ch);
		}

		return;
	}

	if (!is_number(arg))
	{
		send_to_char("You must provide a number.\n\r", ch);
		return;
	}

	auto lines = atoi(arg);
	if (lines == 0)
	{
		send_to_char("Paging disabled.\n\r", ch);
		ch->lines = 0;
		return;
	}

	if (lines < 10 || lines > 100)
	{
		send_to_char("You must provide a reasonable number.\n\r", ch);
		return;
	}

	sprintf(buf, "Scroll set to %d lines.\n\r", lines);

	send_to_char(buf, ch);

	ch->lines = lines - 2;
}

/* RT does socials */
void do_socials(CHAR_DATA *ch, char *argument)
{
	auto col = 0;
	for (auto iSocial = 0; social_table[iSocial].name[0] != '\0'; iSocial++)
	{
		char buf[MAX_STRING_LENGTH];
		sprintf(buf, "%-12s", social_table[iSocial].name);
		send_to_char(buf, ch);

		if (++col % 2 == 0)
		{
			send_to_char("\n\r", ch);
			return;
		}
	}

	if (col % 6 != 0)
		send_to_char("\n\r", ch);
}

/* RT Commands to replace news, motd, imotd, etc from ROM */

void do_motd(CHAR_DATA *ch, char *argument)
{
	do_help(ch, "motd");
}

void do_imotd(CHAR_DATA *ch, char *argument)
{
	do_help(ch, "imotd");
}

void do_rules(CHAR_DATA *ch, char *argument)
{
	do_help(ch, "rules");
}

void do_story(CHAR_DATA *ch, char *argument)
{
	do_help(ch, "story");
}

void do_wizlist(CHAR_DATA *ch, char *argument)
{
	do_help(ch, "wizlist");
}

/* RT this following section holds all the auto commands from ROM, as well as replacements for config */
void do_autolist(CHAR_DATA *ch, char *argument)
{
	/* lists most player flags */
	if (is_npc(ch))
		return;

	send_to_char("   action     status\n\r", ch);
	send_to_char("---------------------\n\r", ch);

	auto sendToChar = [](bool isSet, CHAR_DATA *ch)
	{
		auto txt = isSet ? "ON\n\r" : "OFF\n\r";
		send_to_char(txt, ch);
	};

	send_to_char("autoabort      ", ch);
	sendToChar(IS_SET(ch->act, PLR_AUTOABORT), ch);

	send_to_char("autoassist     ", ch);
	sendToChar(IS_SET(ch->act, PLR_AUTOASSIST), ch);

	send_to_char("autoexit       ", ch);
	sendToChar(IS_SET(ch->act, PLR_AUTOEXIT), ch);

	send_to_char("autogold       ", ch);
	sendToChar(IS_SET(ch->act, PLR_AUTOGOLD), ch);

	send_to_char("autoloot       ", ch);
	sendToChar(IS_SET(ch->act, PLR_AUTOLOOT), ch);

	send_to_char("autosac        ", ch);
	sendToChar(IS_SET(ch->act, PLR_AUTOSAC), ch);

	send_to_char("autosplit      ", ch);
	sendToChar(IS_SET(ch->act, PLR_AUTOSPLIT), ch);

	send_to_char("compact mode   ", ch);
	sendToChar(IS_SET(ch->comm, COMM_COMPACT), ch);

	send_to_char("prompt         ", ch);
	sendToChar(IS_SET(ch->comm, COMM_PROMPT), ch);

	send_to_char("color          ", ch);
	sendToChar(IS_SET(ch->comm, COMM_ANSI), ch);

	send_to_char("combine items  ", ch);
	sendToChar(IS_SET(ch->comm, COMM_COMBINE), ch);

	if (IS_SET(ch->act, PLR_NOSUMMON))
		send_to_char("You cannot be summoned by anyone out of PK.\n\r", ch);
	else
		send_to_char("You can be summoned by anyone.\n\r", ch);

	if (IS_SET(ch->act, PLR_NOFOLLOW))
		send_to_char("You do not welcome followers.\n\r", ch);
	else
		send_to_char("You accept followers.\n\r", ch);
}

void do_color(CHAR_DATA *ch, char *argument)
{
	if (is_npc(ch))
		return;

	char buf[MAX_STRING_LENGTH];
	if (!str_prefix(argument, "list") && str_cmp(argument, ""))
	{
		send_to_char("The following colors are available:\n\r", ch);

		for (auto i = 0; i < MAX_COLORS; i++)
		{
			sprintf(buf, "%i) %s%s%s\n\r",
				i + 1,
				color_table[i].color_ascii,
				color_table[i].color_name,
				END_COLOR(ch));

			send_to_char(buf, ch);
		}

		return;
	}

	if (!str_prefix(argument, "toggle") && str_cmp(argument, ""))
	{
		if (IS_SET(ch->comm, COMM_ANSI))
		{
			send_to_char("Color disabled.\n\r", ch);
			REMOVE_BIT(ch->comm, COMM_ANSI);
		}
		else
		{
			send_to_char("Color enabled.\n\r", ch);
			SET_BIT(ch->comm, COMM_ANSI);
		}

		return;
	}

	if (!str_cmp(argument, ""))
	{
		send_to_char("Your color scheme is:\n\r", ch);

		for (auto i = 0; i < MAX_EVENTS; i++)
		{
			if (get_trust(ch) < color_event[i].min_level)
				continue;

			sprintf(buf, "%i) %-20s (currently %s%s%s)\n\r",
				i + 1,
				capitalize(color_event[i].event_name),
				get_char_color(ch, ch->pcdata->color_scheme[i]),
				ch->pcdata->color_scheme[i],
				END_COLOR(ch));

			send_to_char(buf, ch);
		}

		sprintf(buf, "Your color is %s.  Use color TOGGLE to switch color on/off.\n\r", 
			IS_SET(ch->comm, COMM_ANSI) ? "ON" : "OFF");

		send_to_char(buf, ch);
		send_to_char("Use color <event> <color> to change the color of a given event.\n\r", ch);
		send_to_char("Use color list to see a list of available colors.\n\r", ch);
		return;
	}

	char arg1[MAX_STRING_LENGTH], arg2[MAX_STRING_LENGTH];
	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);

	if (arg1[0] == '\0' || arg2[0] == '\0')
	{
		send_to_char("Invalid choices.\n\r", ch);
		return;
	}

	auto eventnum = get_event_number(arg1);
	if (eventnum < 0)
	{
		send_to_char("Invalid event.\n\r", ch);
		return;
	}

	auto color = get_color_name(arg2);
	if (color == nullptr)
	{
		send_to_char("Invalid color selection.\n\r", ch);
		return;
	}

	ch->pcdata->color_scheme[eventnum] = color;

	sprintf(buf, "%s has been color remapped to %s.\n\r", 
		capitalize(color_event[eventnum].event_name),
		color);

	send_to_char(buf, ch);
}

void do_autoabort(CHAR_DATA *ch, char *argument)
{
	if (is_npc(ch))
		return;

	if (IS_SET(ch->act, PLR_AUTOABORT))
	{
		send_to_char("Autoabort removed.\n\r", ch);
		REMOVE_BIT(ch->act, PLR_AUTOABORT);
	}
	else
	{
		send_to_char("You will now automatically exit edit mode when attacked.\n\r", ch);
		SET_BIT(ch->act, PLR_AUTOABORT);
	}
}

void do_autoassist(CHAR_DATA *ch, char *argument)
{
	if (is_npc(ch))
		return;

	if (IS_SET(ch->act, PLR_AUTOASSIST))
	{
		send_to_char("Autoassist removed.\n\r", ch);
		REMOVE_BIT(ch->act, PLR_AUTOASSIST);
	}
	else
	{
		send_to_char("You will now assist when needed.\n\r", ch);
		SET_BIT(ch->act, PLR_AUTOASSIST);
	}
}

void do_autoexit(CHAR_DATA *ch, char *argument)
{
	if (is_npc(ch))
		return;

	if (IS_SET(ch->act, PLR_AUTOEXIT))
	{
		send_to_char("Exits will no longer be displayed.\n\r", ch);
		REMOVE_BIT(ch->act, PLR_AUTOEXIT);
	}
	else
	{
		send_to_char("Exits will now be displayed.\n\r", ch);
		SET_BIT(ch->act, PLR_AUTOEXIT);
	}
}

void do_autogold(CHAR_DATA *ch, char *argument)
{
	if (is_npc(ch))
		return;

	if (IS_SET(ch->act, PLR_AUTOGOLD))
	{
		send_to_char("Autogold removed.\n\r", ch);
		REMOVE_BIT(ch->act, PLR_AUTOGOLD);
	}
	else
	{
		send_to_char("Automatic gold looting set.\n\r", ch);
		SET_BIT(ch->act, PLR_AUTOGOLD);
	}
}

void do_autoloot(CHAR_DATA *ch, char *argument)
{
	if (is_npc(ch))
		return;

	if (IS_SET(ch->act, PLR_AUTOLOOT))
	{
		send_to_char("Autolooting removed.\n\r", ch);
		REMOVE_BIT(ch->act, PLR_AUTOLOOT);
	}
	else
	{
		send_to_char("Automatic corpse looting set.\n\r", ch);
		SET_BIT(ch->act, PLR_AUTOLOOT);
	}
}

void do_autosac(CHAR_DATA *ch, char *argument)
{
	if (is_npc(ch))
		return;

	if (IS_SET(ch->act, PLR_AUTOSAC))
	{
		send_to_char("Autosacrificing removed.\n\r", ch);
		REMOVE_BIT(ch->act, PLR_AUTOSAC);
	}
	else
	{
		send_to_char("Automatic corpse sacrificing set.\n\r", ch);
		SET_BIT(ch->act, PLR_AUTOSAC);
	}
}

void do_autosplit(CHAR_DATA *ch, char *argument)
{
	if (is_npc(ch))
		return;

	if (IS_SET(ch->act, PLR_AUTOSPLIT))
	{
		send_to_char("Autosplitting removed.\n\r", ch);
		REMOVE_BIT(ch->act, PLR_AUTOSPLIT);
	}
	else
	{
		send_to_char("Automatic gold splitting set.\n\r", ch);
		SET_BIT(ch->act, PLR_AUTOSPLIT);
	}
}

void do_brief(CHAR_DATA *ch, char *argument)
{
	if (IS_SET(ch->comm, COMM_BRIEF))
	{
		send_to_char("Full descriptions activated.\n\r", ch);
		REMOVE_BIT(ch->comm, COMM_BRIEF);
	}
	else
	{
		send_to_char("Short descriptions activated.\n\r", ch);
		SET_BIT(ch->comm, COMM_BRIEF);
	}
}

void do_compact(CHAR_DATA *ch, char *argument)
{
	if (IS_SET(ch->comm, COMM_COMPACT))
	{
		send_to_char("Compact mode removed.\n\r", ch);
		REMOVE_BIT(ch->comm, COMM_COMPACT);
	}
	else
	{
		send_to_char("Compact mode set.\n\r", ch);
		SET_BIT(ch->comm, COMM_COMPACT);
	}
}

void do_show(CHAR_DATA *ch, char *argument)
{
	if (IS_SET(ch->comm, COMM_SHOW_AFFECTS))
	{
		send_to_char("Affects will no longer be shown in score.\n\r", ch);
		REMOVE_BIT(ch->comm, COMM_SHOW_AFFECTS);
	}
	else
	{
		send_to_char("Affects will now be shown in score.\n\r", ch);
		SET_BIT(ch->comm, COMM_SHOW_AFFECTS);
	}
}

void do_prompt(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];

	if (!str_cmp(argument, ""))
	{
		send_to_char("Syntax: prompt toggle\n\r", ch);
		send_to_char("Syntax: prompt default\n\r", ch);
		send_to_char("Syntax: prompt (string)\n\r\n\r", ch);
		send_to_char("The following key phrases may be used in the (string) and will be translated:\n\r", ch);
		send_to_char("%h: Current HP      %H: Maximum HP\n\r", ch);
		send_to_char("%m: Current Mana    %M: Maximum Mana\n\r", ch);
		send_to_char("%v: Current Moves   %V: Maximum Moves\n\r", ch);
		send_to_char("%1: % of Max HP     %2: % of Max Mana\n\r", ch);
		send_to_char("%3: % of Max MV\n\r", ch);
		send_to_char("%x: Total XP        %X: XP TNL\n\r", ch);
		send_to_char("%g: Total Gold      %r: Room Name\n\r", ch);
		send_to_char("%e: Room Exits      %c: Newline (for long prompts)\n\r", ch);
		send_to_char("%L: Lag Status (+ is lagged, and - is not)\n\r", ch);

		if (is_immortal(ch))
			send_to_char("%z: Area Name       %R: Room Vnum\n\r", ch);

		if (is_immortal(ch))
			send_to_char("%p: Mortals in Area %P: All PCs in area\n\r", ch);

		if (is_immortal(ch))
			send_to_char("%C: Player Count\n\r", ch);

		send_to_char("Example: prompt <%hhp %mm %vmv> will set your prompt to <10hp 100m 100mv>.\n\r", ch);
		send_to_char("Prompt toggle will toggle whether you see prompts or not.\n\r", ch);
		send_to_char("Prompt default will set your prompt to the default <hp mana moves> format.\n\r", ch);
		return;
	}

	if (!str_prefix(argument, "toggle"))
	{
		if (IS_SET(ch->comm, COMM_PROMPT))
		{
			send_to_char("Prompts disabled.\n\r", ch);
			REMOVE_BIT(ch->comm, COMM_PROMPT);
		}
		else
		{
			send_to_char("Prompts enabled.\n\r", ch);
			SET_BIT(ch->comm, COMM_PROMPT);
		}

		return;
	}

	if (!strcmp(argument, "default"))
	{
		strcpy(buf, "<%hhp %mm %vmv> ");
	}
	else
	{
		if (strlen(argument) > 75)
			argument[75] = '\0';

		strcpy(buf, argument);
		smash_tilde(buf);

		if (str_suffix("%c", buf))
			strcat(buf, " ");
	}

	free_pstring(ch->prompt);
	ch->prompt = palloc_string(buf);

	sprintf(buf, "Your prompt is now set to %s.\n\r", ch->prompt);
	send_to_char(buf, ch);
}

void do_combine(CHAR_DATA *ch, char *argument)
{
	if (IS_SET(ch->comm, COMM_COMBINE))
	{
		send_to_char("Long inventory selected.\n\r", ch);
		REMOVE_BIT(ch->comm, COMM_COMBINE);
	}
	else
	{
		send_to_char("Combined inventory selected.\n\r", ch);
		SET_BIT(ch->comm, COMM_COMBINE);
	}
}

void do_nofollow(CHAR_DATA *ch, char *argument)
{
	if (is_npc(ch))
		return;

	if (is_affected_by(ch, AFF_CHARM))
	{
		send_to_char("Now why would you want to leave your master?\n\r", ch);
		return;
	}

	if (IS_SET(ch->act, PLR_NOFOLLOW))
	{
		send_to_char("You now accept followers.\n\r", ch);
		REMOVE_BIT(ch->act, PLR_NOFOLLOW);
	}
	else
	{
		send_to_char("You no longer accept followers.\n\r", ch);
		SET_BIT(ch->act, PLR_NOFOLLOW);
		die_follower(ch);
	}
}

void do_nosummon(CHAR_DATA *ch, char *argument)
{
	if (is_npc(ch))
	{
		if (IS_SET(ch->imm_flags, IMM_SUMMON))
		{
			send_to_char("You may now be summoned by anyone.\n\r", ch);
			REMOVE_BIT(ch->imm_flags, IMM_SUMMON);
		}
		else
		{
			send_to_char("You may no longer be summoned.\n\r", ch);
			SET_BIT(ch->imm_flags, IMM_SUMMON);
		}
	}
	else
	{
		if (IS_SET(ch->act, PLR_NOSUMMON))
		{
			send_to_char("You may now be summoned by anyone.\n\r", ch);
			REMOVE_BIT(ch->act, PLR_NOSUMMON);
		}
		else
		{
			send_to_char("You may only be summoned by those in PK.\n\r", ch);
			SET_BIT(ch->act, PLR_NOSUMMON);
		}
	}
}

void do_glance(CHAR_DATA *ch, char *argument)
{
	if (ch->desc == nullptr)
		return;

	if (argument[0] == '\0')
	{
		send_to_char("Glance at whom?\n\r", ch);
		return;
	}

	if (ch->position < POS_SLEEPING)
	{
		send_to_char("You can't see anything but stars!\n\r", ch);
		return;
	}

	if (ch->position == POS_SLEEPING)
	{
		send_to_char("You can't see anything, you're sleeping!\n\r", ch);
		return;
	}

	if (!check_blind(ch))
		return;

	auto victim = get_char_room(ch, argument); 
	if (victim == nullptr)
	{
		send_to_char("That person isn't here.\n\r", ch);
		return;
	}

	if (is_affected(victim, gsn_cloak_form))
	{
		send_to_char("The figure is buried deep within a dark colored cloak.\n\r", ch);
		return;
	}

	show_char_to_char_2(victim, ch);
}

void do_examine(CHAR_DATA *ch, char *argument)
{
	if (ch->desc == nullptr)
		return;

	char arg[MAX_INPUT_LENGTH];
	one_argument(argument, arg);

	if (arg[0] == '\0')
	{
		send_to_char("Examine what?\n\r", ch);
		return;
	}

	if (ch->position < POS_SLEEPING)
	{
		send_to_char("You can't see anything but stars!\n\r", ch);
		return;
	}

	if (ch->position == POS_SLEEPING)
	{
		send_to_char("You can't see anything, you're sleeping!\n\r", ch);
		return;
	}

	if (!check_blind(ch))
		return;

	std::string buffer;

	auto obj = get_obj_here(ch, arg);
	if (obj != nullptr)
	{
		switch (obj->item_type)
		{
			case ITEM_DRINK_CON:
			case ITEM_CONTAINER:
			case ITEM_CORPSE_NPC:
			case ITEM_CORPSE_PC:
				buffer = fmt::format("in {}", arg);
				break;
			default:
				buffer = std::string(arg);
				break;
		}

		do_look(ch, buffer.data());
	}
	else
	{
		do_look(ch, arg);
	}
}

bool show_altdesc(ROOM_INDEX_DATA *room)
{
	if (!room->alt_description)
		return false;

	if (room->alt_description_cond == AD_COND_NIGHT && sun == SolarPosition::Dark)
		return true;

	return false;
}

char *get_room_description(ROOM_INDEX_DATA *room)
{
	if (!room->alt_description)
		return room->description;

	if (show_altdesc(room))
		return room->alt_description;

	return room->description;
}

char *get_room_name(ROOM_INDEX_DATA *room)
{
	if (!room->alt_description)
		return room->name;

	if (show_altdesc(room))
		return room->alt_name;
		
	return room->name;
}

void do_look(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];
	char *pdesc, *direction;
	int door;
	int i;

	if (ch->desc == nullptr)
		return;

	if (ch->position < POS_SLEEPING)
	{
		send_to_char("You can't see anything but stars!\n\r", ch);
		return;
	}

	if (ch->position == POS_SLEEPING)
	{
		send_to_char("You can't see anything, you're sleeping!\n\r", ch);
		return;
	}

	if (!check_blind(ch))
		return;

	if (!is_npc(ch)
		&& !IS_SET(ch->act, PLR_HOLYLIGHT)
		&& room_is_dark(ch->in_room)
		&& !is_affected_by(ch, AFF_DARK_VISION))
	{
		send_to_char("It is pitch black ... \n\r", ch);
		show_char_to_char(ch->in_room->people, ch);
		return;
	}

	char arg1[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	char arg3[MAX_INPUT_LENGTH];
	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);

	auto number = number_argument(arg1, arg3);
	auto count = 0;

	if (arg1[0] == '\0' || !str_cmp(arg1, "auto"))
	{
		/* 'look' or 'look auto' */

		if (get_bv_stage(ch) < 2)
		{
			sprintf(buf, "%s", get_room_name(ch->in_room));
			buf[0] = UPPER(buf[0]);
			send_to_char(buf, ch);

			if ((is_immortal(ch) && (is_npc(ch) || IS_SET(ch->act, PLR_HOLYLIGHT)))
				|| (!is_npc(ch) && is_heroimm(ch) && strstr(ch->in_room->area->builders, ch->name)))
			{
				sprintf(buf, " [Room %d]", ch->in_room->vnum);
				send_to_char(buf, ch);

				if (show_altdesc(ch->in_room))
					send_to_char(" [Alt Desc]", ch);
			}

			if (is_immortal(ch))
			{
				for (auto victim = ch->in_room->people; victim; victim = victim->next_in_room)
				{
					if (is_npc(victim) && IS_SET(victim->act, ACT_WARD_MOB))
					{
						send_to_char(" [Ward Mob]", ch);
						break;
					}
				}
			}

			send_to_char("\n\r", ch);
		}

		if ((arg1[0] == '\0' || (!is_npc(ch) && !IS_SET(ch->comm, COMM_BRIEF)))
			&& !is_affected(ch, gsn_plasma_arc)
			&& 	(get_bv_stage(ch) < 2))
		{
			send_to_char("  ", ch);
			send_to_char(get_room_description(ch->in_room), ch);
		}

		if ((is_npc(ch) || (!is_npc(ch) && IS_SET(ch->act, PLR_AUTOEXIT)))
			&& (get_bv_stage(ch) < 1))
		{
			send_to_char("\n\r", ch);
			do_exits(ch, "auto");
		}

		if (is_affected_room(ch->in_room, gsn_conflagration) && ch->in_room->sector_type == SECT_CONFLAGRATION)
		{
			sprintf(buf, "%sA wave of heat washes over you as a raging inferno blazes throughout the area!%s\n\r",
				get_char_color(ch, "lightred"),
				END_COLOR(ch));

			send_to_char(buf, ch);
		}

		if (is_affected_room(ch->in_room, gsn_essence_of_plasma))
		{
			sprintf(buf, "%sAn orb of pure plasma whizzes erratically through the area!%s\n\r",
				get_char_color(ch, "lightmagenta"),
				END_COLOR(ch));

			send_to_char(buf, ch);
		}

		if (is_affected_room(ch->in_room, gsn_glaciate) && ch->in_room->sector_type == SECT_ICE)
		{
			sprintf(buf, "%sThe water here has congealed into a thick glacial ice floe!%s\n\r",
				get_char_color(ch, "white"),
				END_COLOR(ch));

			send_to_char(buf, ch);
		}

		if (is_affected_room(ch->in_room, gsn_caustic_vapor))
		{
			sprintf(buf, "%sA thick cloud of noxious vapors hangs over the area.%s\n\r",
				get_char_color(ch, "lightgreen"),
				END_COLOR(ch));

			send_to_char(buf, ch);
		}

		if (is_affected_room(ch->in_room, gsn_blanket))
		{
			sprintf(buf, "%sA layer of glimmering snow blankets the area.%s\n\r",
				get_char_color(ch, "white"),
				END_COLOR(ch));

			send_to_char(buf, ch);
		}

		if (is_affected_room(ch->in_room, gsn_flood))
		{
			sprintf(buf, "%sThe area is entirely submerged beneath a massive flood!%s\n\r",
				get_char_color(ch, "cyan"),
				END_COLOR(ch));

			send_to_char(buf, ch);
		}

		if (is_affected_room(ch->in_room, gsn_riptide))
		{
			for (auto raf = ch->in_room->affected; raf != nullptr; raf = raf->next)
			{
				if (raf->type == gsn_riptide && raf->owner == ch && raf->location == APPLY_ROOM_NONE &&
					raf->modifier == 1)
				{
					sprintf(buf, "%sThe calm surface of the water belies the deadly riptide churning beneath!%s\n\r",
						get_char_color(ch, "cyan"),
						END_COLOR(ch));

					send_to_char(buf, ch);
					break;
				}
				else if (raf->type == gsn_riptide && raf->owner == ch && raf->location == APPLY_ROOM_NONE &&
						 raf->modifier == 2)
				{
					sprintf(buf, "%sThe waters swirl menacingly, ready to receive the riptide's prey.%s\n\r",
						get_char_color(ch, "cyan"),
						END_COLOR(ch));

					send_to_char(buf, ch);
					break;
				}
			}
		}

		if (is_affected(ch, gsn_mark_of_wrath))
		{
			auto af = affect_find(ch->affected, gsn_mark_of_wrath);

			for (i = 0; i < MAX_TRACKS; i++)
			{
				if (!ch->in_room->tracks[i])
					break;

				if (ch->in_room->tracks[i]->prey != af->owner)
					continue;

				direction = (char *)flag_name_lookup(ch->in_room->tracks[i]->direction, direction_table);

				sprintf(buf, "%sThrough the veil of your rage, you sense %s's tracks leading %s.%s\n\r",
					get_char_color(ch, "lightred"),
					af->owner->name,
					direction,
					END_COLOR(ch));

				send_to_char(buf, ch);
			}
		}

		if (ch->in_room->sector_type == SECT_SNOW)
		{
			for (i = 0; i <= 2; i++)
			{
				if (!ch->in_room->tracks[i])
					continue;

				if (!ch->in_room->tracks[i]->prey)
					continue;

				if (ch->in_room->tracks[i]->flying)
					continue;

				direction = flag_name_lookup(ch->in_room->tracks[i]->direction, direction_table);

				sprintf(buf, "%sYou see %s footprints leading %s through the snow.%s\n\r",
					get_char_color(ch, "white"),
					(ch->in_room->tracks[i]->prey->race == 4)
						? "elven"
						: pc_race_table[ch->in_room->tracks[i]->prey->race].race_time,
					direction,
					END_COLOR(ch));
					
				send_to_char(buf, ch);
			}
		}

		show_list_to_char(ch->in_room->contents, ch, false, false);
		show_char_to_char(ch->in_room->people, ch);
		return;
	}

	pdesc = get_extra_descr(arg3, ch->in_room->extra_descr);

	if (pdesc != nullptr)
	{
		if (++count == number)
		{
			send_to_char(pdesc, ch);
			return;
		}
	}

	if (!str_cmp(arg1, "i") || !str_cmp(arg1, "in") || !str_cmp(arg1, "on"))
	{
		/* 'look in' */
		if (arg2[0] == '\0')
		{
			send_to_char("Look in what?\n\r", ch);
			return;
		}

		if (is_affected(ch, gsn_plasma_arc))
		{
			send_to_char("You can't seem to see that!\n\r", ch);
			return;
		}

		auto obj = get_obj_here(ch, arg2);
		if (obj == nullptr)
		{
			send_to_char("You do not see that here.\n\r", ch);
			return;
		}

		switch (obj->item_type)
		{
			case ITEM_DRINK_CON:
				if (obj->value[1] <= 0)
				{
					send_to_char("It is empty.\n\r", ch);
					break;
				}

				sprintf(buf, "It's %sfilled with a %s liquid.\n\r",
						obj->value[1] < obj->value[0] / 4
							? "less than half-"
							: obj->value[1] < 3 * obj->value[0] / 4 ? "about half-" : "more than half-",
						liq_table[obj->value[2]].liq_color);

				send_to_char(buf, ch);
				break;

			case ITEM_CONTAINER:
			case ITEM_CORPSE_NPC:
			case ITEM_CORPSE_PC:
				if (IS_SET_OLD(obj->value[1], CONT_CLOSED))
				{
					send_to_char("It is closed.\n\r", ch);
					break;
				}

				if (is_affected_obj(obj, gsn_ice_blast))
				{
					act("$p is frozen shut!", ch, obj, 0, TO_CHAR);
					break;
				}

				act("$p holds:", ch, obj, nullptr, TO_CHAR);

				if (obj->contains)
				{
					show_list_to_char(obj->contains, ch, true, true);
				}
				else
				{
					send_to_char("Nothing.\n\r", ch);
				}
				break;
			default:
				send_to_char("That is not a container.\n\r", ch);
				break;
		}

		return;
	}

	if (!str_cmp(arg1, "berus") || !str_cmp(arg1, "calabren"))
	{
		if (!is_outside(ch))
		{
			send_to_char("You cannot see the sky, much less the moons!\n\r", ch);
			return;
		}

		if (sun == SolarPosition::Daylight)
		{
			send_to_char("You cannot make out the moons during the day.\n\r", ch);
			return;
		}

		if (ch->in_room->area->sky == WeatherCondition::Overcast || ch->in_room->area->sky >= WeatherCondition::Downpour)
		{
			send_to_char("You cannot make out the moons through the cloud cover.\n\r", ch);
			return;
		}

		char buf2[MAX_STRING_LENGTH];
		auto isCalabren = !str_cmp(arg1, "calabren");

		sprintf(buf2, "The %s moon of %s orbits above you in the night sky.\n\r",
			moon_look[isCalabren ? moon_calabren : moon_berus],
			isCalabren ? "Calabren" : "Berus");

		send_to_char(buf2, ch);
		return;
	}

	auto victim = get_char_room(ch, arg1);
	if (victim != nullptr)
	{
		if (is_affected(ch, gsn_plasma_arc))
		{
			send_to_char("You can't seem to see that!\n\r", ch);
			return;
		}

		if (is_immortal(victim))
			act("$n looks at you.", ch, 0, victim, TO_VICT);

		show_char_to_char_1(victim, ch);
		return;
	}

	auto found = false;
	auto obj = ch->in_room->contents;
	for (; obj != nullptr; obj = obj->next_content)
	{
		if (!can_see_obj(ch, obj))
			continue;

		pdesc = get_extra_descr(arg3, obj->extra_descr);
		if (pdesc != nullptr)
		{
			if (++count == number)
			{
				found = true;
				send_to_char(pdesc, ch);
				break;
			}

			continue;
		}

		pdesc = get_extra_descr(arg3, obj->pIndexData->extra_descr);
		if (pdesc != nullptr)
		{
			if (++count == number)
			{
				found = true;
				send_to_char(pdesc, ch);
				break;
			}

			continue;
		}

		if (is_name(arg3, obj->name))
		{
			if (++count == number)
			{
				found = true;
				send_to_char(obj->description, ch);
				send_to_char("\n\r", ch);
				break;
			}
		}
	}

	for (obj = ch->carrying; obj != nullptr; obj = obj->next_content)
	{
		if (!can_see_obj(ch, obj))
			continue;

		pdesc = get_extra_descr(arg3, obj->extra_descr);
		if (pdesc != nullptr)
		{
			if (++count == number)
			{
				found = true;
				send_to_char(pdesc, ch);
				break;
			}

			continue;
		}

		pdesc = get_extra_descr(arg3, obj->pIndexData->extra_descr);
		if (pdesc != nullptr)
		{
			if (++count == number)
			{
				found = true;
				send_to_char(pdesc, ch);
				break;
			}

			continue;
		}

		if (is_name(arg3, obj->name))
		{
			if (++count == number)
			{
				found = true;
				send_to_char(obj->description, ch);
				send_to_char("\n\r", ch);
				break;
			}
		}
	}

	if (obj)
	{
		if (obj->item_type == ITEM_URN)
		{
			sprintf(buf, "Peering into the urn, you estimate that it currently holds %s ounce%s of blood.\n\r",
				int_to_string(obj->value[4]),
				obj->value[4] == 1 ? "" : "s");

			send_to_char(buf, ch);
		}

		if (obj->item_type == ITEM_SCROLL
			&& obj->pIndexData->vnum == OBJ_VNUM_SCRIBE
			&& ch->cabal == CABAL_SCION)
		{
			sprintf(buf, "Deciphering the symbols, you realize this scroll contains the spell %s.\n\r", skill_table[obj->value[1]].name);
			send_to_char(buf, ch);
		}
	}

	if (found)
		return;

	if (obj)
	{
		if (obj->pIndexData->vnum == OBJ_VNUM_CHESSBOARD)
			print_chessboard(ch);

		if (obj->item_type == ITEM_SCROLL
			&& obj->pIndexData->vnum == OBJ_VNUM_SCRIBE
			&& ch->cabal == CABAL_SCION)
		{
			sprintf(buf, "Deciphering the symbols, you realize this scroll contains the spell %s.\n\r", skill_table[obj->value[1]].name);
			send_to_char(buf, ch);
		}

		if (obj->item_type == ITEM_URN)
		{
			sprintf(buf, "Peering into the urn, you estimate that it currently holds %s ounce%s of blood.\n\r",
				int_to_string(obj->value[4]),
				obj->value[4] == 1 ? "" : "s");

			send_to_char(buf, ch);
		}
	}

	if (found)
		return;

	if (count > 0 && count != number)
	{
		auto buffer = count == 1
			? fmt::format("You only see one {} here.\n\r", arg3)
			: fmt::format("You only see {} of those here.\n\r", count); //TODO: change the rest of the sprintf calls to format

		send_to_char(buffer.c_str(), ch);
		return;
	}

	if (!str_cmp(arg1, "n") || !str_cmp(arg1, "north"))
	{
		door = 0;
	}
	else if (!str_cmp(arg1, "e") || !str_cmp(arg1, "east"))
	{
		door = 1;
	}
	else if (!str_cmp(arg1, "s") || !str_cmp(arg1, "south"))
	{
		door = 2;
	}
	else if (!str_cmp(arg1, "w") || !str_cmp(arg1, "west"))
	{
		door = 3;
	}
	else if (!str_cmp(arg1, "u") || !str_cmp(arg1, "up"))
	{
		door = 4;
	}
	else if (!str_cmp(arg1, "d") || !str_cmp(arg1, "down"))
	{
		door = 5;
	}
	else
	{
		send_to_char("You do not see that here.\n\r", ch);
		return;
	}

	/* 'look direction' */

	/* if the current room is outside, up should show weather.. */

	auto pexit = ch->in_room->exit[door];
	if (pexit == nullptr)
	{
		if (door == 4 && is_outside(ch))
		{
			do_weather(ch, "");
			return;
		}

		send_to_char("Nothing special there.\n\r", ch);
		return;
	}

	if (pexit->u1.to_room && is_affected_room(pexit->u1.to_room, gsn_conflagration))
	{
		send_to_char("Billowing smoke and towering flames obscure your ability to see anything in that direction.\n\r", ch);
		return;
	}

	if (is_affected_room(pexit->u1.to_room, gsn_smokescreen) || is_affected_room(ch->in_room, gsn_smokescreen))
	{
		send_to_char("Smoke disrupts your vision and prevents you from looking around there.\n\r", ch);
		return;
	}

	if (pexit->description != nullptr && pexit->description[0] != '\0')
		send_to_char(pexit->description, ch);
	else
		send_to_char("Nothing special there.\n\r", ch);

	if (pexit 
		&& !IS_SET(pexit->exit_info, EX_CLOSED)
		&& is_affected(ch, gsn_farsee)
		&& pexit->u1.to_room)
	{
		auto crystal = get_eq_char(ch, WEAR_HOLD);
		if (crystal != nullptr && crystal->pIndexData->vnum == OBJ_VNUM_CRYSTAL)
		{
			send_to_char("Focusing through your crystal, you extend your sights....\n\r", ch);

			auto in_room = ch->in_room;

			char_from_room(ch);
			char_to_room(ch, pexit->u1.to_room);
			do_look(ch, "auto");
			char_from_room(ch);
			char_to_room(ch, in_room);
		}
	}

	if (pexit->keyword != nullptr && pexit->keyword[0] != '\0' && pexit->keyword[0] != ' ')
	{
		if (IS_SET(pexit->exit_info, EX_CLOSED) && !IS_SET(pexit->exit_info, EX_NONOBVIOUS))
			act("The $T is closed.", ch, nullptr, pexit->keyword, TO_CHAR);
		else if (IS_SET(pexit->exit_info, EX_ISDOOR) && !IS_SET(pexit->exit_info, EX_NONOBVIOUS))
			act("The $T is open.", ch, nullptr, pexit->keyword, TO_CHAR);
	}
}

/* RT added back for the hell of it */
void do_read(CHAR_DATA *ch, char *argument)
{
	do_look(ch, argument);
}

/*
 * Thanks to Zrin for auto-exit part.
 */
void do_exits(CHAR_DATA *ch, char *argument)
{
	if (!check_blind(ch))
		return;

	char buf[MAX_STRING_LENGTH];
	if (is_affected_room(ch->in_room, gsn_smokescreen))
	{
		sprintf(buf, "%sDense smoke prevents any vision beyond this room!%s\n\r",
			get_char_color(ch, "darkgrey"),
			END_COLOR(ch));

		send_to_char(buf, ch);
	}

	auto fAuto = !str_cmp(argument, "auto");
	if (fAuto)
		sprintf(buf, "[Exits:");
	else if (is_immortal(ch))
		sprintf(buf, "Obvious exits from room %d:\n\r", ch->in_room->vnum);
	else
		sprintf(buf, "Obvious exits:\n\r");

	auto found = false;
	for (auto door = 0; door <= 5; door++)
	{
		auto pexit = ch->in_room->exit[door];

		if (pexit != nullptr
			&& pexit->u1.to_room != nullptr
			&& can_see_room(ch, pexit->u1.to_room)
			&& (!IS_SET(pexit->exit_info, EX_NONOBVIOUS) || is_immortal(ch))
			&& !is_affected_room(ch->in_room, gsn_smokescreen))
		{
			found = true;

			if (fAuto)
			{
				strcat(buf, " ");
				if (IS_SET(pexit->exit_info, EX_CLOSED) || IS_SET(pexit->exit_info, EX_NONOBVIOUS))
					strcat(buf, "(");

				strcat(buf, dir_name[door]);
				if (IS_SET(pexit->exit_info, EX_CLOSED) || IS_SET(pexit->exit_info, EX_NONOBVIOUS))
					strcat(buf, ")");
			}
			else
			{
				sprintf(buf + strlen(buf), "%-5s - %s",
					capitalize(dir_name[door]),
					room_is_dark(pexit->u1.to_room) ? "Too dark to tell" : get_room_name(pexit->u1.to_room));

				if (is_immortal(ch))
					sprintf(buf + strlen(buf), " %s%s(room %d)\n\r",
						IS_SET(pexit->exit_info, EX_CLOSED) ? "(CLOSED) " : "",
						IS_SET(pexit->exit_info, EX_NONOBVIOUS) ? "(NONOBVIOUS) " : "", pexit->u1.to_room->vnum);
				else
					sprintf(buf + strlen(buf), "\n\r");
			}
		}
	}

	if (!found)
		strcat(buf, fAuto ? " none" : "None.\n\r");

	if (fAuto)
		strcat(buf, "]\n\r");

	send_to_char(buf, ch);
}

void do_worth(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];

	if (is_npc(ch))
	{
		sprintf(buf, "You have %ld gold.\n\r", ch->gold);
		send_to_char(buf, ch);
		return;
	}

	sprintf(buf, "You have %ld gold and %d experience (%d exp to level).\n\r",
		ch->gold,
		ch->exp,
		ch->level * exp_per_level(ch) - ch->exp);

	send_to_char(buf, ch);
}

void do_score(CHAR_DATA *ch, char *argument)
{
	if (is_npc(ch))
	{
		send_to_char("Nope, not for NPC's. Try stat <mob>.\n\r", ch);
		return;
	}

	char buf[MAX_STRING_LENGTH];
	sprintf(buf, "You are %s%s, level %d (%d hours).\n\r",
		ch->true_name,
		ch->pcdata->title,
		ch->level,
		get_hours(ch));
	send_to_char(buf, ch);

	sprintf(buf, "You are %d %s years old, born in the Caelumaedani year %d.\n\r",
		get_age(ch),
		pc_race_table[ch->race].race_time,
		ch->pcdata->birth_date);
	send_to_char(buf, ch);

	auto tmp = get_hours(ch) / ch->pcdata->death_time * 100.001f;
	auto i = (int)tmp;
	char *state;

	if (i < 15)
		state = "You feel energetic and ready to take on the world!";
	else if (i < 40)
		state = "You feel vigorous and hale.";
	else if (i < 65)
		state = "Your arduous life is beginning to take its toll upon your body.";
	else if (i < 90)
		state = "You have lived a full life, but your body grows weary of the strain.";
	else
		state = "Your bones ache from a lifetime's toil, and you feel the end approaching.";

	sprintf(buf, "%s\n\r", state);
	send_to_char(buf, ch);

	sprintf(buf, "Race: %s  Sex: %s  Class: %s\n\r",
		race_table[ch->race].name,
		sex_table[ch->sex].name,
		is_npc(ch) ? "mobile" : ch->Class()->name);
	send_to_char(buf, ch);

	if (get_trust(ch) != ch->level)
	{
		sprintf(buf, "You are trusted at level %d.\n\r", get_trust(ch));
		send_to_char(buf, ch);
	}

	sprintf(buf, "You have %d/%d hit, %d/%d mana, %d/%d movement.\n\r",
		ch->hit,
		ch->max_hit,
		ch->mana,
		ch->max_mana,
		ch->move,
		ch->max_move);
	send_to_char(buf, ch);

	if (ch->Class()->GetIndex() == CLASS_WARRIOR || ch->pcdata->special > 0)
	{
		sprintf(buf, "You have %d practices and %d training sessions and %d specializations.\n\r",
			ch->practice,
			ch->train,
			ch->pcdata->special);
		send_to_char(buf, ch);
	}
	else
	{
		sprintf(buf, "You have %d practices and %d training sessions.\n\r", ch->practice, ch->train);
		send_to_char(buf, ch);
	}

	sprintf(buf, "You have %d proficiency points.\n\r", ch->Profs()->GetPoints());
	send_to_char(buf, ch);

	sprintf(buf, "You are carrying %d/%d items with weight %ld/%d pounds.\n\r",
		ch->carry_number,
		can_carry_n(ch),
		get_carry_weight(ch),
		can_carry_w(ch));
	send_to_char(buf, ch);

	sprintf(buf, "Str: %d(%d)  Int: %d(%d)  Wis: %d(%d)  Dex: %d(%d)  Con: %d(%d)\n\r",
		ch->perm_stat[STAT_STR],
		get_curr_stat(ch, STAT_STR),
		ch->perm_stat[STAT_INT],
		get_curr_stat(ch, STAT_INT),
		ch->perm_stat[STAT_WIS],
		get_curr_stat(ch, STAT_WIS),
		ch->perm_stat[STAT_DEX],
		get_curr_stat(ch, STAT_DEX),
		ch->perm_stat[STAT_CON],
		get_curr_stat(ch, STAT_CON));
	send_to_char(buf, ch);

	sprintf(buf, "You have scored %d exp, and have %ld gold.\n\r", ch->exp, ch->gold);
	send_to_char(buf, ch);

	/* RT shows exp to level */
	if (!is_npc(ch) && ch->level < LEVEL_HERO)
	{
		sprintf(buf, "You need %d exp to level.\n\r", (ch->level * exp_per_level(ch) - ch->exp));
		send_to_char(buf, ch);
	}

	sprintf(buf, "Wimpy set to %d hit points.  Hometown is %s.\n\r", ch->wimpy, hometown_table[ch->hometown].name);
	send_to_char(buf, ch);

	if (!is_npc(ch) && ch->pcdata->condition[COND_DRUNK] > 10)
		send_to_char("You are drunk.\n\r", ch);
	if (!is_npc(ch) && ch->pcdata->condition[COND_THIRST] > 35)
		send_to_char("You are thirsty.\n\r", ch);
	if (!is_npc(ch) && ch->pcdata->condition[COND_HUNGER] > 35)
		send_to_char("You are hungry.\n\r", ch);

	switch (ch->position)
	{
		case POS_DEAD:
			send_to_char("You are DEAD!!\n\r", ch);
			break;
		case POS_MORTAL:
			send_to_char("You are mortally wounded.\n\r", ch);
			break;
		case POS_INCAP:
			send_to_char("You are incapacitated.\n\r", ch);
			break;
		case POS_STUNNED:
			send_to_char("You are stunned.\n\r", ch);
			break;
		case POS_SLEEPING:
			send_to_char("You are sleeping.\n\r", ch);
			break;
		case POS_RESTING:
			send_to_char("You are resting.\n\r", ch);
			break;
		case POS_SITTING:
			send_to_char("You are sitting.\n\r", ch);
			break;
		case POS_STANDING:
			send_to_char("You are standing.\n\r", ch);
			break;
		case POS_FIGHTING:
			send_to_char("You are fighting.\n\r", ch);
			break;
	}

	/* print AC values -- print negative AC as 0 */
	if (ch->level >= 30)
	{
		sprintf(buf, "Armor: pierce: %d  bash: %d  slash: %d  magic: %d\n\r",
			std::max(0, (int)get_ac(ch, AC_PIERCE)),
			std::max(0, (int)get_ac(ch, AC_BASH)),
			std::max(0, (int)get_ac(ch, AC_SLASH)),
			std::max(0, (int)get_ac(ch, AC_EXOTIC)));
		send_to_char(buf, ch);
	}

	for (i = 0; i < 4; i++)
	{
		char *temp;

		switch (i)
		{
			case AC_PIERCE:
				temp = "piercing";
				break;
			case AC_BASH:
				temp = "bashing";
				break;
			case AC_SLASH:
				temp = "slashing";
				break;
			case AC_EXOTIC:
				temp = "magic";
				break;
			default:
				temp = "error";
				break;
		}

		send_to_char("You are ", ch);

		if (get_ac(ch, i) <= 20)
			sprintf(buf, "defenseless against %s.\n\r", temp);
		else if (get_ac(ch, i) <= 40)
			sprintf(buf, "barely protected from %s.\n\r", temp);
		else if (get_ac(ch, i) <= 60)
			sprintf(buf, "slightly armored against %s.\n\r", temp);
		else if (get_ac(ch, i) <= 80)
			sprintf(buf, "somewhat armored against %s.\n\r", temp);
		else if (get_ac(ch, i) <= 100)
			sprintf(buf, "armored against %s.\n\r", temp);
		else if (get_ac(ch, i) <= 120)
			sprintf(buf, "well-armored against %s.\n\r", temp);
		else if (get_ac(ch, i) <= 140)
			sprintf(buf, "very well-armored against %s.\n\r", temp);
		else if (get_ac(ch, i) <= 160)
			sprintf(buf, "heavily armored against %s.\n\r", temp);
		else if (get_ac(ch, i) <= 180)
			sprintf(buf, "superbly armored against %s.\n\r", temp);
		else if (get_ac(ch, i) <= 200)
			sprintf(buf, "impenetrably armored against %s.\n\r", temp);
		else if (get_ac(ch, i) <= 220)
			sprintf(buf, "nigh-invulnerable to %s.\n\r", temp);
		else
			sprintf(buf, "divinely armored against %s.\n\r", temp);

		send_to_char(buf, ch);
	}

	/* RT wizinvis and holy light */
	if (is_immortal(ch))
	{
		send_to_char("Holy Light: ", ch);

		if (IS_SET(ch->act, PLR_HOLYLIGHT))
			send_to_char("on", ch);
		else
			send_to_char("off", ch);

		if (ch->invis_level)
		{
			sprintf(buf, "  Invisible: level %d", ch->invis_level);
			send_to_char(buf, ch);
		}

		if (ch->incog_level)
		{
			sprintf(buf, "  Incognito: level %d", ch->incog_level);
			send_to_char(buf, ch);
		}

		send_to_char("\n\r", ch);
	}

	if (ch->level >= 15)
	{
		sprintf(buf, "Hitroll: %d  Damroll: %d.\n\r", get_hitroll(ch), get_damroll(ch));
		send_to_char(buf, ch);
	}

	send_to_char("You are ", ch);
	if (ch->alignment == 1000)
		send_to_char("good,", ch);
	else if (ch->alignment == 0)
		send_to_char("neutral,", ch);
	else
		send_to_char("evil,", ch);

	send_to_char(" and have a ", ch);
	if (ch->pcdata->ethos == 1000)
		send_to_char("lawful ethos.\n\r", ch);
	else if (ch->pcdata->ethos == 0)
		send_to_char("neutral ethos.\n\r", ch);
	else
		send_to_char("chaotic ethos.\n\r", ch);

	if (ch->pcdata->trusting)
	{
		sprintf(buf, "You are trusting %s.\n\r", ch->pcdata->trusting->name);
		send_to_char(buf, ch);
	}
	if (ch->Class()->GetIndex() == CLASS_SORCERER)
	{
		sprintf(buf, "Your major elemental focus is %s and your para-elemental focus is %s.\n\r",
				sphere_table[ch->pcdata->ele_major].name, sphere_table[ch->pcdata->ele_para].name);
		send_to_char(buf, ch);
	}
	if (ch->Class()->GetIndex() == CLASS_ANTI_PALADIN)
	{
		for (i = 0; demon_table[i].name; i++)
		{
			if (demon_table[i].type == LESSER_DEMON && ch->pcdata->lesserdata[i] >= FAVOR_GRANTED)
				act("You have been granted the favor of the lesser demon $t.", ch, capitalize(demon_table[i].name), 0, TO_CHAR);
			if (demon_table[i].type == GREATER_DEMON && i >= MAX_LESSER && ch->pcdata->greaterdata[i - MAX_LESSER] >= FAVOR_GRANTED)
				act("You have been granted the favor of the greater demon $t.", ch, capitalize(demon_table[i].name), 0, TO_CHAR);
		}
	}

	for (i = 1; i < MAX_STYLE; i++)
	{
		if (IS_SET(ch->pcdata->styles, style_table[i].bit))
		{
			sprintf(buf, "You are %s in the fighting style of the %s.\n\r",
				ch->pcdata->style && ch->pcdata->style == i ? "currently" : "versed",
				style_table[i].name);
			send_to_char(buf, ch);
		}
	}

	if (IS_SET(ch->comm, COMM_SHOW_AFFECTS))
		do_affects(ch, "");
}

void do_affects(CHAR_DATA *ch, char *argument)
{
	if (ch->affected == nullptr || !(ch->affected->aftype != AFT_INVIS || ch->affected->next != nullptr))
	{
		send_to_char("You are not affected by anything.\n\r", ch);
		return;
	}

	send_to_char("You are affected by:\n\r", ch);

	AFFECT_DATA *paf_last = nullptr;
	char buf[MAX_STRING_LENGTH];
	for (auto paf = ch->affected; paf != nullptr; paf = paf->next)
	{
		if (paf->aftype == AFT_INVIS)
			continue;

		for (auto i = 0; i < MAX_STRING_LENGTH; i++)
		{
			buf[i] = '\0';
		}

		if (paf_last != nullptr
			&& (paf->type == paf_last->type && ((paf->name == nullptr && paf_last->name == nullptr) || !str_cmp(paf->name, paf_last->name))))
		{
			if (ch->level >= 20)
				sprintf(buf, "                          ");
			else
				continue;
		}
		else
		{
			if (paf->aftype == AFT_SKILL)
				sprintf(buf, "Skill  : %-17s", paf->name ? paf->name : skill_table[paf->type].name);
			else if (paf->aftype == AFT_POWER)
				sprintf(buf, "Power  : %-17s", paf->name ? paf->name : skill_table[paf->type].name);
			else if (paf->aftype == AFT_MALADY)
				sprintf(buf, "Malady : %-17s", paf->name ? paf->name : skill_table[paf->type].name);
			else if (paf->aftype == AFT_COMMUNE)
				sprintf(buf, "Commune: %-17s", paf->name ? paf->name : skill_table[paf->type].name);
			else if (paf->aftype == AFT_RUNE)
				sprintf(buf, "Rune   : %-17s", paf->name ? paf->name : skill_table[paf->type].name);
			else if (paf->aftype == AFT_TIMER)
				sprintf(buf, "Timer  : %-17s", paf->name ? paf->name : skill_table[paf->type].name);
			else if (paf->aftype != AFT_INVIS)
				sprintf(buf, "Spell  : %-17s", paf->name ? paf->name : skill_table[paf->type].name);
		}

		send_to_char(buf, ch);

		if (ch->level >= 20)
		{
			auto showdur = paf->duration + 1;

			auto buffer = fmt::format("| modifies {}{}{} ",
					(paf->mod_name > -1) ? mod_names[paf->mod_name].name : affect_loc_name(paf->location),
					(paf->mod_name > -1) ? "" : " by ",
					(paf->mod_name > -1) ? "" : std::to_string(paf->modifier)); //TODO: change the rest of the sprintf calls to format

			if (paf->aftype != AFT_INVIS)
				send_to_char(buffer.c_str(), ch);

			if (paf->duration == -1)
			{
				buffer = "permanently";
			}
			else
			{
				buffer = fmt::format("for%s%s%s hour%s.",
					(showdur == 1) ? "" : std::to_string(showdur / 2),
					(showdur % 2 == 0) ? "" : (showdur == 1) ? "" : " and", (showdur % 2 == 0) ? "" : " a half",
					(showdur == 1 || showdur == 2) ? "" : "s");
			}

			if (paf->aftype != AFT_INVIS)
				send_to_char(buffer.c_str(), ch);
		}

		if (paf->aftype != AFT_INVIS)
			send_to_char("\n\r", ch);

		paf_last = paf;
	}
}

void do_time(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];
	char *suf;

	auto day = time_info.day + 1;

	if (day > 4 && day < 20)
		suf = "th";
	else if (day % 10 == 1)
		suf = "st";
	else if (day % 10 == 2)
		suf = "nd";
	else if (day % 10 == 3)
		suf = "rd";
	else
		suf = "th";

	sprintf(buf, "It is %s%d o'clock %s, Day %s, %d%s of the Month %s.\n\rIt is the Season %s in the Year %d.\n\r",
		(time_info.half) ? "half past " : "",
		(time_info.hour % 12 == 0) ? 12 : time_info.hour % 12,
		time_info.hour >= 12 ? "pm" : "am",
		day_name[day % 6],
		day,
		suf,
		month_name[time_info.month],
		season_name[time_info.season],
		time_info.year);
	send_to_char(buf, ch);

	auto time = talloc_string(str_boot_time);
	chomp(time);

	sprintf(buf, "Riftshadow started up at %s.\n\r", time);
	send_to_char(buf, ch);

	time = talloc_string(ctime(&current_time));
	chomp(time);

	sprintf(buf, "The system time is %s.\n\r", time);
	send_to_char(buf, ch);
}

void do_weather(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH], buf2[MAX_STRING_LENGTH];

	if (!is_outside(ch))
	{
		sprintf(buf, "You can't see the sky from here, but the air around you is %s.\n\r", temp_look[ch->in_room->area->temp]);
		send_to_char(buf, ch);
		return;
	}

	sprintf(buf, "%s and the %s air %s\n\r",
		sky_look[ch->in_room->area->sky],
		temp_look[ch->in_room->area->temp],
		wind_look[ch->in_room->area->wind]);
	send_to_char(buf, ch);

	if (sun != SolarPosition::Daylight)
	{
		if (ch->in_room->area->sky == WeatherCondition::Overcast || ch->in_room->area->sky >= WeatherCondition::Downpour)
		{
			send_to_char("You cannot make out the twin moons through the thick cloud cover.\n\r", ch);
			return;
		}

		sprintf(buf2, "Berus is %s and Calabren is %s.\n\r", moon_look[moon_berus], moon_look[moon_calabren]);
		send_to_char(buf2, ch);
	}
}

void do_oldhelp(CHAR_DATA *ch, char *argument)
{
	bool found= false;
	char argall[MAX_INPUT_LENGTH], argone[MAX_INPUT_LENGTH];

	if (argument[0] == '\0')
		argument = "summary";

	/* this parts handles help a b so that it returns help 'a b' */
	argall[0] = '\0';
	while (argument[0] != '\0')
	{
		argument = one_argument(argument, argone);

		if (argall[0] != '\0')
			strcat(argall, " ");

		strcat(argall, argone);
	}

	auto output = new_buf();
	for (auto pHelp = help_first; pHelp != nullptr; pHelp = pHelp->next)
	{
		auto level = (pHelp->level < 0) ? -1 * pHelp->level - 1 : pHelp->level;

		if (level > get_trust(ch))
		{
			if (pHelp->level < 61)
				continue;
		}

		if (is_name(argall, pHelp->keyword))
		{
			/* add separator if found */
			if (found)
				add_buf(output, "\n\r============================================================\n\r\n\r");

			if (pHelp->level >= 0 && str_cmp(argall, "imotd"))
			{
				add_buf(output, pHelp->keyword);
				add_buf(output, "\n\r");
			}

			/*
			 * Strip leading '.' to allow initial blanks.
			 */
			if (pHelp->text[0] == '.')
				add_buf(output, pHelp->text + 1);
			else
				add_buf(output, pHelp->text);

			found = true;

			/* small hack :) */
			if (ch->desc != nullptr && ch->desc->connected != CON_PLAYING && ch->desc->connected != CON_GEN_GROUPS)
				break;
		}
	}

	if (!found)
		send_to_char("No help on that word.\n\r", ch);
	else
		page_to_char(buf_string(output), ch);

	free_buf(output);
}

/* whois command */
void do_whois(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	one_argument(argument, arg);

	if (arg[0] == '\0')
	{
		send_to_char("Whois who?\n\r", ch);
		return;
	}

	std::string buffer;

	auto output = new_buf();
	auto found = false;
	for (auto d = descriptor_list; d != nullptr; d = d->next)
	{
		char const *class_name;
		char const *imm_lvl;

		if (d->connected != CON_PLAYING || !can_see(ch, d->character))
			continue;

		auto wch = (d->original != nullptr) ? d->original : d->character;

		if (!can_see(ch, wch))
			continue;

		if (is_affected(d->character, gsn_disguise))
			continue;

		if (str_cmp(wch->name, wch->true_name)
			? is_immortal(ch)
				? !str_prefix(arg, wch->true_name) : 0
			: !str_prefix(arg, wch->name))
		{
			found = true;

			/* work out the printing */
			class_name = wch->Class()->who_name;
			switch (wch->level)
			{
				case MAX_LEVEL - 0:
					class_name = "IMP";
					imm_lvl = "Creator";
					break;
				case MAX_LEVEL - 1:
					class_name = "CRE";
					imm_lvl = " Elder ";
					break;
				case MAX_LEVEL - 2:
					class_name = "SUP";
					imm_lvl = " Elder ";
					break;
				case MAX_LEVEL - 3:
					class_name = "DEI";
					imm_lvl = "Greater";
					break;
				case MAX_LEVEL - 4:
					class_name = "GOD";
					imm_lvl = "Greater";
					break;
				case MAX_LEVEL - 5:
					class_name = "IMM";
					imm_lvl = "Greater";
					break;
				case MAX_LEVEL - 6:
					class_name = "DEM";
					imm_lvl = " Minor ";
					break;
				case MAX_LEVEL - 7:
					class_name = "ANG";
					imm_lvl = " Minor ";
					break;
				case MAX_LEVEL - 8:
					class_name = "AVA";
					imm_lvl = "Avatar ";
					break;
				default:
					imm_lvl = "";
					break;
			}

			char rbuf[MAX_STRING_LENGTH], disp[MAX_STRING_LENGTH];

			/* a little formatting */
			if (is_immortal(ch) || (is_heroimm(wch) && wch->level < 52))
			{
				if (get_trust(ch) >= 52)
					strcpy(rbuf, "  ");
				else
					strcpy(rbuf, "");

				buffer = fmt::sprintf("[%2d %-5s %s%s] %s%s%s%s%s%s%s%s%s%s%s%s\n\r",
					wch->level,
					pc_race_table[wch->race].who_name_five,
					class_name,
					get_trust(ch) >= 52 && wch->pcdata->history_buffer != nullptr ? " *" : rbuf,
					can_pk(ch, wch) ? "(PK) " : "",
					wch->incog_level >= LEVEL_HERO ? "(Incog) " : "",
					wch->invis_level >= LEVEL_HERO ? "(Wizi) " : "",
					cabal_table[wch->cabal].who_name,
					get_trust(ch) >= 57 && IS_SET(wch->act, PLR_MORON) ? "(MORON) " : "",
					IS_SET(wch->comm, COMM_AFK) && ch->level > 51 ? "[AFK] " : "",
					IS_SET(wch->act, PLR_CRIMINAL) ? "(WANTED) " : "",
					IS_SET(wch->act, PLR_THIEF) ? "(THIEF) " : "",
					isNewbie(wch) ? "(NEWBIE) " : "",
					wch->true_name,
					is_npc(wch) ? "" : wch->pcdata->title,
					is_npc(wch) ? "" : wch->pcdata->extitle ? wch->pcdata->extitle : "");
				add_buf(output, buffer.data());
			}
			else if (get_trust(wch) >= 52 && !is_immortal(ch))
			{
				if (wch->pcdata->shifted > -1)
					continue;

				if (is_immortal(wch))
					sprintf(disp, "*   %s  *", imm_lvl);
				else
					sprintf(disp, "[%2d %-5s %s]",
						wch->level,
						pc_race_table[wch->race].who_name_five,
						class_name);

				buffer = fmt::format("{} {}{}{}{}\n\r",
					disp,
					wch->cabal == ch->cabal ? cabal_table[wch->cabal].who_name : "",
					wch->name, 
					is_npc(wch) ? "" : wch->pcdata->title,
					is_npc(wch) ? "" : (wch->pcdata->extitle) ? wch->pcdata->extitle : ""); //TODO: change the rest of the sprintf calls to format

				add_buf(output, buffer.data());
			}
			else
			{
				if (wch->pcdata->shifted > -1)
					continue;

				if (is_immortal(wch))
					sprintf(disp, "*   %s  *", imm_lvl);
				else
					sprintf(disp, "[%2d %-5s %s]",
						wch->level,
						pc_race_table[wch->race].who_name_five,
						class_name);

				// TODO: originally had 13 placeholders, but it only has 12 arguments. could be a bug.
				buffer = fmt::format("{} {}{}{}{}{}{}{}{}{}{}{}\n\r",
					disp,
					can_pk(ch, wch) ? "(PK) " : "",
					wch->incog_level >= LEVEL_HERO ? "(Incog) " : "",
					wch->invis_level >= LEVEL_HERO ? "(Wizi) " : "",
					wch->cabal == ch->cabal || is_immortal(wch) ? cabal_table[wch->cabal].who_name : "", "",
					IS_SET(wch->comm, COMM_AFK) ? "[AFK] " : "",
					IS_SET(wch->act, PLR_CRIMINAL) ? "(WANTED) " : "",
					IS_SET(wch->act, PLR_THIEF) ? "(THIEF) " : "",
					isNewbie(wch) ? "(NEWBIE) " : "",
					wch->name,
					is_npc(wch) ? "" : wch->pcdata->title,
					is_npc(wch) ? "" : (wch->pcdata->extitle) ? wch->pcdata->extitle : "");
				add_buf(output, buffer.data());
			}
		}
	}

	if (!found)
	{
		send_to_char("No one of that name is playing.\n\r", ch);
		return;
	}

	page_to_char(buf_string(output), ch);
	free_buf(output);
}

/*
 * New 'who' command originally by Alander of Rivers of Mud.
 */
void do_who(CHAR_DATA *ch, char *argument)
{
	std::string buffer;
	char buf[MAX_STRING_LENGTH];
	char buf2[MAX_STRING_LENGTH];
	char rbuf[MAX_STRING_LENGTH];
	char disp[MAX_STRING_LENGTH];
	int fAlign = 0, fEthos = 0;
	bool rgfClass[MAX_CLASS];
	bool rgfRace[MAX_PC_RACE];
	bool rgfCabal[MAX_CABAL];
	bool fCriminal= false;
	bool fClassRestrict= false;
	bool fCabalRestrict= false;
	bool fCabal= false;
	bool fRaceRestrict= false;
	bool fImmortalOnly= false;
	bool fPkOnly= false;
	bool fBuilder= false;
	bool fNewbie= false;
	bool fNoDesc= false;
	bool fRole= false;
	/*
	 * Set default arguments.
	 */
	auto iLevelLower = 0;
	auto iLevelUpper = MAX_LEVEL;

	for (auto iClass = 0; iClass < MAX_CLASS; iClass++)
	{
		rgfClass[iClass] = false;
	}

	for (auto iRace = 0; iRace < MAX_PC_RACE; iRace++)
	{
		rgfRace[iRace] = false;
	}

	for (auto iCabal = 0; iCabal < MAX_CABAL; iCabal++)
	{
		rgfCabal[iCabal] = false;
	}

	/*
	 * Parse arguments.
	 */
	for (;;)
	{
		char arg[MAX_STRING_LENGTH], arg2[500];

		argument = one_argument(argument, arg);
		if (arg[0] == '\0')
			break;

		if (is_number(arg) && is_immortal(ch))
		{
			auto tempnum = atoi(arg) ;
			if (tempnum < 1 || tempnum > MAX_LEVEL)
			{
				send_to_char("Invalid who level range.\n\r", ch);
				return;
			}

			iLevelLower = tempnum;
			one_argument(argument, arg2);

			if (arg2[0] != '\0' && is_number(arg2))
			{
				tempnum = atoi(arg2);
				if (tempnum <= iLevelLower || tempnum > MAX_LEVEL)
				{
					send_to_char("Invalid who level range.\n\r", ch);
					return;
				}
				iLevelUpper = tempnum;
			}
		}
		else
		{
			/*
			 * Look for classes to turn on.
			 */
			if (!str_prefix(arg, "immortal"))
			{
				fImmortalOnly = true;
			}
			else if (!str_prefix(arg, "pk"))
			{
				fPkOnly = true;
			}
			else if (!str_prefix(arg, "wanted") || !str_prefix(arg, "criminal"))
			{
				fCriminal = true;
			}
			else if (!str_prefix(arg, "builder") && is_immortal(ch))
			{
				fBuilder = true;
			}
			else if (!str_prefix(arg, "newbie"))
			{
				fNewbie = true;
			}
			else if (!str_prefix(arg, "good") && is_immortal(ch))
			{
				fAlign = 3;
			}
			else if (!str_prefix(arg, "neutral") && is_immortal(ch))
			{
				fAlign = 2;
			}
			else if (!str_prefix(arg, "evil") && is_immortal(ch))
			{
				fAlign = 1;
			}
			else if (!str_prefix(arg, "lawful") && is_immortal(ch))
			{
				fEthos = 3;
			}
			else if (!str_prefix(arg, "eneutral") && is_immortal(ch))
			{
				fEthos = 2;
			}
			else if (!str_prefix(arg, "chaotic") && is_immortal(ch))
			{
				fEthos = 1;
			}
			else if (!str_prefix(arg, "nodesc") && is_immortal(ch))
			{
				fNoDesc = true;
			}
			else if (!str_prefix(arg, "role") && is_immortal(ch))
			{
				fRole = true;
			}
			else
			{
				auto iClass = CClass::Lookup(arg);
				if (iClass != -1)
				{
					if (is_immortal(ch))
					{
						fClassRestrict = true;
						rgfClass[iClass] = true;
						continue;
					}

					send_to_char("That's not a valid race or cabal.\n\r", ch);
					return;
				}

				auto iRace = race_lookup(arg);
				if (iRace != 0 && iRace <= MAX_PC_RACE)
				{
					fRaceRestrict = true;
					rgfRace[iRace] = true;
					continue;
				}

				auto iCabal = cabal_lookup(arg);
				if (!iCabal)
				{
					send_to_char("That's not a valid race or cabal.\n\r", ch);
					return;
				}

				if (iCabal != ch->cabal && is_immortal(ch))
				{
					send_to_char("You're not a member of that cabal.\n\r", ch);
					return;
				}

				fCabalRestrict = true;
				rgfCabal[iCabal] = true;
			}
		}
	}

	/*
	 * Now show matching chars.
	 */
	buf[0] = '\0';

	auto nMatch = 0;
	auto output = new_buf();
	for (auto d = descriptor_list; d != nullptr; d = d->next)
	{
		/*
		 * Check for match against restrictions.
		 * Don't use trust as that exposes trusted mortals.
		 */
		if (d->connected != CON_PLAYING || !can_see(ch, d->character))
			continue;

		auto wch = d->original != nullptr
			? d->original
			: d->character;

		if (!can_see(ch, wch))
			continue;

		auto fOChar = wch->pcdata->old && !is_immortal(ch);

		if (!is_immortal(ch) && is_affected(wch, gsn_cloak_form) && ch != wch && ch->pcdata->induct == 5)
		{
			// TODO: anc is currently unused, delete section?
			// 	if (can_pk(ch, wch))
			// 		anc = 1;
		 	continue;
		}

		if (wch->level < iLevelLower
			|| wch->level > iLevelUpper
			|| (fImmortalOnly && (wch->level < LEVEL_IMMORTAL && !is_heroimm(wch)))
			|| (fPkOnly && !can_pk(ch, wch))
			|| (fClassRestrict && !rgfClass[wch->Class()->GetIndex()])
			|| (fRaceRestrict && !rgfRace[wch->race])
			|| (fCriminal && !IS_SET(wch->act, PLR_CRIMINAL))
			|| (fBuilder && !IS_SET(wch->comm, COMM_BUILDER))
			|| (fNewbie && wch->level > 10)
			|| (fCabal && !is_cabal(wch) && !is_immortal(ch))
			|| (fCabalRestrict && !rgfCabal[wch->cabal])
			|| (fAlign == 3 && !is_good(wch))
			|| (fAlign == 2 && !is_neutral(wch))
			|| (fAlign == 1 && !is_evil(wch))
			|| (fEthos == 3 && !is_lawful(wch))
			|| (fEthos == 2 && !is_eneutral(wch))
			|| (fEthos == 1 && !is_chaotic(wch))
			|| (fRole && !wch->pcdata->role)
			|| (fNoDesc && wch->description != nullptr && strlen(wch->description) > 144)
			|| fOChar)
		{
			continue;
		}

		nMatch++;

		/*
		 * If the character is disguised, do not show him on who, except to immortals
		 */
		if (is_affected(wch, gsn_disguise) && !is_immortal(ch))
			continue;

		/*
		 * Figure out what to print for class.
		 */
		char const *class_name;
		char const *imm_lvl;
		switch (wch->level)
		{
			case MAX_LEVEL - 0:
				class_name = "IMP";
				imm_lvl = "Creator";
				break;
			case MAX_LEVEL - 1:
				class_name = "CRE";
				imm_lvl = " Elder ";
				break;
			case MAX_LEVEL - 2:
				class_name = "SUP";
				imm_lvl = " Elder ";
				break;
			case MAX_LEVEL - 3:
				class_name = "DEI";
				imm_lvl = "Greater";
				break;
			case MAX_LEVEL - 4:
				class_name = "GOD";
				imm_lvl = "Greater";
				break;
			case MAX_LEVEL - 5:
				class_name = "IMM";
				imm_lvl = "Greater";
				break;
			case MAX_LEVEL - 6:
				class_name = "DEM";
				imm_lvl = " Minor ";
				break;
			case MAX_LEVEL - 7:
				class_name = "ANG";
				imm_lvl = " Minor ";
				break;
			case MAX_LEVEL - 8:
				class_name = "AVA";
				imm_lvl = "Avatar ";
				break;
			default:
				class_name = wch->Class()->who_name;
				imm_lvl = "";
				break;
		}

		/*
		 * Format it up.
		 */
		if (is_immortal(ch) || (wch->level < 52 && is_heroimm(wch)))
		{
			auto trust = get_trust(ch);
			strcpy(rbuf, trust >= 52 ? "  " : "");

			buffer = fmt::sprintf("[%2d %-5s %s%s] %s%s%s%s%s%s%s%s%s%s%s%s%s\n\r",
				wch->level,
				pc_race_table[wch->race].who_name_five,
				class_name,
				trust >= 52 && wch->pcdata->history_buffer != nullptr ? " *" : rbuf,
				can_pk(ch, wch) ? "(PK) " : "",
				wch->incog_level >= LEVEL_HERO ? "(Incog) " : "",
				wch->invis_level >= LEVEL_HERO ? "(Wizi) " : "",
				cabal_table[wch->cabal].who_name,
				trust >= 57 && IS_SET(wch->act, PLR_MORON) ? "(MORON) " : "",
				(IS_SET(wch->comm, COMM_AFK) && ch->level > 51) ? "[AFK] " : "",
				IS_SET(wch->act, PLR_CRIMINAL) ? "(WANTED) " : "",
				IS_SET(wch->act, PLR_THIEF) ? "(THIEF) " : "",
				isNewbie(wch) ? "(NEWBIE) " : "",
				is_affected(wch, gsn_disguise) ? "(Disguised) " : "",
				wch->true_name,
				is_npc(wch) ? "" : wch->pcdata->title,
				is_npc(wch) ? "" : wch->pcdata->extitle ? wch->pcdata->extitle : "");
			add_buf(output, buffer.data());
		}
		else if (get_trust(wch) >= 52 && !is_immortal(ch))
		{
			if (wch->pcdata->shifted > -1)
				continue;

			if (is_immortal(wch))
			{
				sprintf(disp, "*   %s  *", imm_lvl);
			}
			else
			{
				sprintf(disp, "[%2d %-5s %s]",
					wch->level,
					pc_race_table[wch->race].who_name_five,
					class_name);
			}

			buffer = fmt::format("{} {}{}{}{}\n\r", 
				disp,
				wch->cabal == ch->cabal ? cabal_table[wch->cabal].who_name : "",
				wch->true_name,
				is_npc(wch) ? "" : wch->pcdata->title,
				is_npc(wch) ? "" : (wch->pcdata->extitle) ? wch->pcdata->extitle : ""); //TODO: change the rest of the sprintf calls to format
			add_buf(output, buffer.data());
		}
		else
		{
			if (wch->pcdata->shifted > -1)
				continue;

			if (is_immortal(wch))
			{
				sprintf(disp, "*   %s  *", imm_lvl);
			}
			else
			{
				sprintf(disp, "[%2d %-5s %s]",
					wch->level,
					pc_race_table[wch->race].who_name_five,
					class_name);
			}

			buffer = fmt::format("{} {}{}{}{}{}{}{}{}{}{}{}\n\r",
				disp,
				can_pk(ch, wch) ? "(PK) " : "",
				wch->incog_level >= LEVEL_HERO ? "(Incog) " : "",
				wch->invis_level >= LEVEL_HERO ? "(Wizi) " : "",
				wch->cabal == ch->cabal || is_immortal(wch) ? cabal_table[wch->cabal].who_name : "",
				IS_SET(wch->comm, COMM_AFK) ? "[AFK] " : "",
				IS_SET(wch->act, PLR_CRIMINAL) ? "(WANTED) " : "",
				IS_SET(wch->act, PLR_THIEF) ? "(THIEF) " : "",
				isNewbie(wch) ? "(NEWBIE) " : "",
				wch->true_name,
				is_npc(wch) ? "" : wch->pcdata->title,
				is_npc(wch) ? "" : wch->pcdata->extitle ? wch->pcdata->extitle : "");
			add_buf(output, buffer.data());
		}
	}

	sprintf(buf2, "\n\rPlayers found: %d\n\r", nMatch);
	add_buf(output, buf2);
	page_to_char(buf_string(output), ch);
	free_buf(output);
}

void do_count(CHAR_DATA *ch, char *argument)
{
	auto not_seen = 0;
	auto count = 0;

	for (auto d = descriptor_list; d != nullptr; d = d->next)
	{
		if (d->connected == CON_PLAYING)
		{
			if (can_see(ch, d->character) && !is_switched(d->character))
				count++;
			else
				not_seen++;
		}
	}

	if ((count + not_seen) >= count_data[0])
		count_data[0] = count + not_seen;

	char buf[MAX_STRING_LENGTH];
	sprintf(buf, "There are %d characters on that you can see.\n\r", count);
	send_to_char(buf, ch);
}

void do_inventory(CHAR_DATA *ch, char *argument)
{
	send_to_char("You are carrying:\n\r", ch);
	show_list_to_char(ch->carrying, ch, true, true);
}

void do_equipment(CHAR_DATA *ch, char *argument)
{
	send_to_char("You are using:\n\r", ch);

	auto found = false;
	for (auto iWear = 0; iWear < MAX_WEAR; iWear++)
	{
		if (iWear == WEAR_WIELD) // cosmetics come right before "wield" on display
		{
			for (auto tObj = ch->carrying; tObj; tObj = tObj->next_content)
			{
				if (tObj->wear_loc == WEAR_COSMETIC)
				{
					auto buffer = fmt::format("{}>", tObj->wear_loc_name ? tObj->wear_loc_name : "bug: unset wear_loc on cosmetic");
					buffer = fmt::sprintf("<worn %-14s", buffer);
					send_to_char(buffer.c_str(), ch);

					if (can_see_obj(ch, tObj))
						send_to_char(format_obj_to_char(tObj, ch, true), ch);
					else
						send_to_char("something.", ch);

					send_to_char("\n\r", ch);
					found = true;
				}
			}
		}

		auto obj = get_eq_char(ch, iWear);
		if (iWear == WEAR_COSMETIC || obj == nullptr)
			continue;

		send_to_char(where_name[iWear], ch);

		if (can_see_obj(ch, obj))
			send_to_char(format_obj_to_char(obj, ch, true), ch);
		else
			send_to_char("something.", ch);

		send_to_char("\n\r", ch);
		found = true;
	}

	if (!found)
		send_to_char("Nothing.\n\r", ch);
}

void do_compare(CHAR_DATA *ch, char *argument)
{
	char arg1[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);

	if (arg1[0] == '\0')
	{
		send_to_char("Compare what to what?\n\r", ch);
		return;
	}

	auto obj1 = get_obj_carry(ch, arg1, ch);
	if (obj1 == nullptr)
	{
		send_to_char("You do not have that item.\n\r", ch);
		return;
	}

	OBJ_DATA *obj2;
	if (arg2[0] == '\0')
	{
		for (obj2 = ch->carrying; obj2 != nullptr; obj2 = obj2->next_content)
		{
			if (obj2->wear_loc != WEAR_NONE 
				&& can_see_obj(ch, obj2) 
				&& obj1->item_type == obj2->item_type 
				&& obj1->wear_flags[0] == obj2->wear_flags[0]
				&& IS_SET(obj1->wear_flags, ITEM_TAKE)
				&& IS_SET(obj2->wear_flags, ITEM_TAKE))
			{
				break;
			}
		}

		if (obj2 == nullptr)
		{
			send_to_char("You aren't wearing anything comparable.\n\r", ch);
			return;
		}
	}
	else
	{
		obj2 = get_obj_carry(ch, arg2, ch);
		if (obj2 == nullptr)
		{
			send_to_char("You do not have that item.\n\r", ch);
			return;
		}
	}

	char *msg = nullptr;
	auto value1 = 0;
	auto value2 = 0;

	if (obj1 == obj2)
	{
		msg = "You compare $p to itself.  It looks about the same.";
	}
	else if (obj1->item_type != obj2->item_type)
	{
		msg = "You can't compare $p and $P.";
	}
	else
	{
		switch (obj1->item_type)
		{
			case ITEM_ARMOR:
				value1 = obj1->value[0] + obj1->value[1] + obj1->value[2];
				value2 = obj2->value[0] + obj2->value[1] + obj2->value[2];
				break;
			case ITEM_WEAPON:
				value1 = (1 + obj1->value[2]) * obj1->value[1];
				value2 = (1 + obj2->value[2]) * obj2->value[1];
				break;
			default:
				msg = "You can't compare $p and $P.";
				break;
		}
	}

	if (msg == nullptr)
	{
		if (value1 == value2)
			msg = "$p and $P look about the same.";
		else if (value1 > value2)
			msg = "$p looks better than $P.";
		else
			msg = "$p looks worse than $P.";
	}

	act(msg, ch, obj1, obj2, TO_CHAR);
}

void do_where(CHAR_DATA *ch, char *argument)
{
	if (ch == nullptr)
		return;

	if (is_affected_by(ch, AFF_BLIND) || (get_bv_stage(ch) > -1))
	{
		send_to_char("You can't see anything!\n\r", ch);
		return;
	}

	if (is_affected_area(ch->in_room->area, gsn_cyclone))
	{
		send_to_char("The swirling winds and debris prevent you from seeing anything!\n\r", ch);
		return;
	}

	if (is_affected_room(ch->in_room, gsn_smokescreen))
	{
		send_to_char("Smoke fills the room and prevents you from looking around!\n\r", ch);
		return;
	}

	auto found = false;
	char buf[MAX_STRING_LENGTH];

	char arg[MAX_INPUT_LENGTH];
	one_argument(argument, arg);
	if (arg[0] == '\0')
	{
		send_to_char("Players near you:\n\r", ch);

		for (auto d = descriptor_list; d; d = d->next)
		{
			auto victim = d->character;
			if (d->connected == CON_PLAYING
				&& victim != nullptr
				&& !is_npc(victim)
				&& victim->in_room != nullptr
				&& !IS_SET(victim->in_room->room_flags, ROOM_NOWHERE) 
				&& (!IS_SET(victim->in_room->room_flags, ROOM_IMP_ONLY) || ch->level == 60)
				&& (is_room_owner(ch, victim->in_room) || !room_is_private(victim->in_room))
				&& (victim->in_room->area == ch->in_room->area 
					|| (is_adjacent_area(victim->in_room->area, ch->in_room->area)
						&& (is_immortal(ch)
						|| ch->pcdata->greaterdata[GREATER_GERYON] == GERYON_FINGER)))
				&& can_see(ch, victim)
				&& !is_affected_room(victim->in_room, gsn_smokescreen)
				&& !is_affected(victim, gsn_disguise))
			{
				found = true;

				sprintf(buf, "%s%-28s %s%s%s\n\r",
					can_pk(ch, victim) ? "(PK) " : "     ",
					(!is_npc(ch) && ch == victim && is_affected(ch, gsn_shroud_of_secrecy)) ? ch->true_name : pers(victim, ch),
					is_immortal(ch) ? victim->in_room->area->name : "", is_immortal(ch) ? ": " : "",
					get_room_name(victim->in_room));

				send_to_char(buf, ch);
			}
		}

		if (!found)
			send_to_char("None.\n\r", ch);
	}
	else if (!str_prefix(arg, "pk"))
	{
		for (auto d = descriptor_list; d; d = d->next)
		{
			auto victim = d->character;
			if (d->connected == CON_PLAYING
				&& victim != nullptr
				&& !is_npc(victim)
				&& victim->in_room != nullptr
				&& !IS_SET(victim->in_room->room_flags, ROOM_NOWHERE)
				&& (!IS_SET(victim->in_room->room_flags, ROOM_IMP_ONLY) || ch->level == 60)
				&& (is_room_owner(ch, victim->in_room) || !room_is_private(victim->in_room))
				&& (victim->in_room->area == ch->in_room->area
					|| (is_adjacent_area(victim->in_room->area, ch->in_room->area) && is_immortal(ch)))
				&& can_pk(ch, victim)
				&& can_see(ch, victim)
				&& !is_affected_room(victim->in_room, gsn_smokescreen))
			{
				found = true;

				sprintf(buf, "%s%-28s %s%s%s\n\r",
					can_pk(ch, victim) ? "(PK) " : "     ",
					(!is_npc(ch) && ch == victim && is_affected(ch, gsn_shroud_of_secrecy)) ? ch->true_name : pers(victim, ch),
					is_immortal(ch) ? victim->in_room->area->name : "", is_immortal(ch) ? ": " : "",
					get_room_name(victim->in_room));

				send_to_char(buf, ch);
			}
		}

		if (!found)
			act("You didn't find anyone in PK.", ch, nullptr, arg, TO_CHAR);
	}
	else
	{
		for (auto victim = char_list; victim != nullptr; victim = victim->next)
		{
			if (victim->in_room != nullptr
				&& (victim->in_room->area == ch->in_room->area
					|| (is_adjacent_area(victim->in_room->area, ch->in_room->area) && is_immortal(ch)))
				&& !is_affected_by(victim, AFF_SNEAK)
				&& (!IS_SET(victim->in_room->room_flags, ROOM_IMP_ONLY) || ch->level == 60)
				&& can_see(ch, victim)
				&& is_name(arg, victim->name)
				&& !is_affected_room(victim->in_room, gsn_smokescreen))
			{
				found = true;

				sprintf(buf, "%s%-28s %s%s%s\n\r",
					can_pk(ch, victim) ? "(PK) " : "     ", pers(victim, ch),
					is_immortal(ch) ? victim->in_room->area->name : "",
					is_immortal(ch) ? ": " : "",
					get_room_name(victim->in_room));

				send_to_char(buf, ch);
				break;
			}
		}

		if (!found)
			act("You didn't find any $T.", ch, nullptr, arg, TO_CHAR);
	}
}

void do_consider(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	one_argument(argument, arg);
	if (arg[0] == '\0')
	{
		send_to_char("Consider killing whom?\n\r", ch);
		return;
	}

	auto victim = get_char_room(ch, arg);
	if (victim == nullptr)
	{
		send_to_char("They're not here.\n\r", ch);
		return;
	}

	auto diff = victim->level - ch->level;
	if (diff <= -10)
		act("Your sneeze would kill $N.", ch, 0, victim, TO_CHAR);
	else if (diff <= -5)
		act("$N wouldn't last long against you.", ch, 0, victim, TO_CHAR);
	else if (diff <= -2)
		act("$N looks like an easy kill.", ch, 0, victim, TO_CHAR);
	else if (diff <= 1)
		act("$N is about a perfect match for you!", ch, 0, victim, TO_CHAR);
	else if (diff <= 4)
		act("$N looks a little tough.", ch, 0, victim, TO_CHAR);
	else if (diff <= 9)
		act("You wouldn't last too long against $N by yourself.", ch, 0, victim, TO_CHAR);
	else
		act("You must have a fascination with death.", ch, 0, victim, TO_CHAR);

	diff = victim->size - ch->size;
	if (diff > 1)
		act("$N easily towers over you.", ch, 0, victim, TO_CHAR);
	else if (diff == 1)
		act("$N is considerably larger than you.", ch, 0, victim, TO_CHAR);
	else if (!diff)
		act("$N is about the same size as you.", ch, 0, victim, TO_CHAR);
	else if (diff == -1)
		act("$N is considerably smaller than you.", ch, 0, victim, TO_CHAR);
	else if (diff < -1)
		act("You tower over $N.  Be careful not to step on $M.", ch, 0, victim, TO_CHAR);

	if (victim->alignment >= 1)
		act("$N seems relatively benign.", ch, 0, victim, TO_CHAR);
}

void set_title(CHAR_DATA *ch, char *title)
{
	if (is_npc(ch))
	{
		RS.Logger.Debug("Set_title: NPC.");
		return;
	}

	char buf[MAX_STRING_LENGTH];

	if (title[0] != '.' && title[0] != ',' && title[0] != '!' && title[0] != '?')
	{
		buf[0] = ' ';
		strcpy(buf + 1, title);
	}
	else
	{
		strcpy(buf, title);
	}

	free_pstring(ch->pcdata->title);
	ch->pcdata->title = palloc_string(buf);
}

void do_title(CHAR_DATA *ch, char *argument)
{
	if (is_npc(ch) || ch->level < 52)
		return;

	/*
	 * Can't use one_argument here because it smashes case.
	 * So we just steal all its code.  Bleagh.
	 */
	char arg1[MAX_INPUT_LENGTH];
	auto pArg = arg1;

	while (isspace(*argument))
	{
		argument++;
	}

	auto cEnd = ' ';
	if (*argument == '\'' || *argument == '"')
		cEnd = *argument++;

	while (*argument != '\0')
	{
		if (*argument == cEnd)
		{
			argument++;
			break;
		}
		
		*pArg++ = *argument++;
	}

	*pArg = '\0';

	if (arg1[0] == '\0' || argument[0] == '\0')
	{
		send_to_char("Change who's title to what?\n\r", ch);
		return;
	}

	auto victim = get_char_world(ch, arg1);
	if (victim == nullptr)
	{
		send_to_char("Player isn't on.\n\r", ch);
		return;
	}

	if (is_npc(victim))
	{
		send_to_char("Trying to change a mob's title?\n\r", ch);
		return;
	}

	if (ch != victim && get_trust(victim) >= get_trust(ch))
	{
		send_to_char("You can't do that to that person.\n\r", ch);
		return;
	}

	if (strlen(argument) > 45)
		argument[45] = '\0';

	auto fp = fopen(GOD_LOG_FILE, "a");
	if (fp != nullptr)
	{
		auto strtime = ctime(&current_time);
		strtime[strlen(strtime) - 1] = '\0';

		fprintf(fp, "%s: TITLE- %s is titling %s.\n", strtime, ch->name, argument);
		fclose(fp);
	}

	char buf[MAX_STRING_LENGTH];
	sprintf(buf, "AUTO: Titled to (%s) by %s.\n\r", argument, ch->true_name);
	add_history(nullptr, victim, buf);

	smash_tilde(argument);
	set_title(victim, argument);

	send_to_char("Ok.\n\r", ch);
}

void set_extitle(CHAR_DATA *ch, char *title)
{
	if (is_npc(ch))
	{
		RS.Logger.Debug("Set_extitle: NPC.");
		return;
	}

	char buf[MAX_STRING_LENGTH];
	if (title[0] != '.' && title[0] != ',' && title[0] != '!' && title[0] != '?')
	{
		buf[0] = ' ';
		strcpy(buf + 1, title);
	}
	else
	{
		strcpy(buf, title);
	}

	free_pstring(ch->pcdata->extitle);
	ch->pcdata->extitle = palloc_string(buf);
}

void do_extitle(CHAR_DATA *ch, char *argument)
{
	if (is_npc(ch))
		return;

	/*
	 * Can't use one_argument here because it smashes case.
	 * So we just steal all its code.  Bleagh.
	 */
	char arg1[MAX_INPUT_LENGTH];
	auto pArg = arg1;

	while (isspace(*argument))
	{
		argument++;
	}

	auto cEnd = ' ';
	if (*argument == '\'' || *argument == '"')
		cEnd = *argument++;

	while (*argument != '\0')
	{
		if (*argument == cEnd)
		{
			argument++;
			break;
		}

		*pArg++ = *argument++;
	}

	*pArg = '\0';

	if (arg1[0] == '\0')
	{
		send_to_char("Change who's extra title to what?\n\r", ch);
		return;
	}

	auto victim = get_char_world(ch, arg1);
	if (victim == nullptr)
	{
		send_to_char("That player isn't on.\n\r", ch);
		return;
	}

	if (is_npc(victim))
	{
		send_to_char("Trying to change a mob's extra title?\n\r", ch);
		return;
	}

	if (ch != victim && get_trust(victim) >= get_trust(ch))
	{
		send_to_char("You can't do that to that person.\n\r", ch);
		return;
	}

	if (argument[0] == '\0')
	{
		free_pstring(ch->pcdata->extitle);
		ch->pcdata->extitle = nullptr;

		send_to_char("Extitle cleared.\n\r", ch);
		return;
	}

	if (strlen(argument) > 45)
		argument[45] = '\0';

	auto fp = fopen(GOD_LOG_FILE, "a");
	if (fp != nullptr)
	{
		auto strtime = ctime(&current_time);
		strtime[strlen(strtime) - 1] = '\0';

		fprintf(fp, "%s: TITLE- %s is titling %s.\n", strtime, ch->name, argument);
		fclose(fp);
	}

	char buf[MAX_STRING_LENGTH];
	sprintf(buf, "AUTO: Extitled to (%s) by %s.\n\r", argument, ch->true_name);
	add_history(nullptr, victim, buf);

	smash_tilde(argument);
	set_extitle(victim, argument);

	send_to_char("Ok.\n\r", ch);
}

void descr_end_fun(CHAR_DATA *ch, char *argument)
{
	free_pstring(ch->description);
	ch->description = palloc_string(argument);

	send_to_char("Description modified.\n\r", ch);
	ch->pcdata->entered_text[0] = '\0';
}

void do_description(CHAR_DATA *ch, char *argument)
{
	if (is_npc(ch))
		return;

	char arg1[MAX_STRING_LENGTH];
	argument = one_argument(argument, arg1);

	if (!str_cmp(arg1, "clear"))
	{
		if (ch->description)
			free_pstring(ch->description);

		ch->description = nullptr;

		send_to_char("Description cleared.\n\r", ch);
	}
	else if (!str_cmp(arg1, "+"))
	{
		smash_tilde(argument);
		auto buffer = new_buf();

		if (ch->description)
			add_buf(buffer, ch->description);

		add_buf(buffer, argument);
		add_buf(buffer, "\n\r");

		if (ch->description)
			free_pstring(ch->description);

		ch->description = palloc_string(buf_string(buffer));
		free_buf(buffer);

		send_to_char("Line added.\n\r", ch);
		return;
	}
	else if (!str_cmp(arg1, "-"))
	{
		if (ch->description == nullptr || ch->description[0] == '\0')
		{
			send_to_char("No lines left to remove.\n\r", ch);
			return;
		}

		char buf[MAX_STRING_LENGTH];
		strcpy(buf, ch->description);

		auto  found= false;
		for (auto len = strlen(buf); len > 0; len--)
		{
			if (buf[len] == '\r')
			{
				if (!found) /* back it up */
				{
					if (len > 0)
						len--;

					found = true;
				}
				else /* found the second one */
				{
					buf[len + 1] = '\0';
					free_pstring(ch->description);
					
					ch->description = palloc_string(buf);
					return;
				}
			}
		}

		buf[0] = '\0';
		free_pstring(ch->description);

		ch->description = palloc_string(buf);

		send_to_char("Ok\n\r", ch);
		return;
	}
	else if (!str_cmp(arg1, "format"))
	{
		if (!ch->description)
		{
			send_to_char("You have no description to format!\n\r", ch);
			return ;
		}

		ch->description = format_string(ch->description);

		send_to_char("Description formatted.\n\r", ch);
		return;
	}

	send_to_char("Your description is:\n\r", ch);
	send_to_char(ch->description ? ch->description : "(None)\n\r", ch);
}

void do_report(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH], buf2[MAX_STRING_LENGTH], buf3[MAX_STRING_LENGTH];

	auto percenta = ch->max_hit == 0  ? 0 : (ch->hit * 100) / ch->max_hit;
	auto percentb = ch->max_mana == 0 ? 0 : (ch->mana * 100) / ch->max_mana;
	auto percentc = ch->max_move == 0 ? 0 : (ch->move * 100) / ch->max_move;

	if (percenta >= 100)
		sprintf(buf, "in perfect health");
	else if (percenta < 100 && percenta >= 80)
		sprintf(buf, "slightly injured");
	else if (percenta < 80 && percenta >= 60)
		sprintf(buf, "somewhat wounded");
	else if (percenta < 60 && percenta >= 40)
		sprintf(buf, "seriously hurt");
	else if (percenta < 40 && percenta >= 20)
		sprintf(buf, "grievously wounded");
	else if (percenta < 20 && percenta > 0)
		sprintf(buf, "hanging on by a thread");
	else
		sprintf(buf, "dead on my feet");

	if (percentb >= 100)
		sprintf(buf2, "full");
	else if (percentb < 100 && percentb >= 80)
		sprintf(buf2, "strong");
	else if (percentb < 80 && percentb >= 60)
		sprintf(buf2, "steady");
	else if (percentb < 60 && percentb >= 40)
		sprintf(buf2, "wavering");
	else if (percentb < 40 && percentb >= 20)
		sprintf(buf2, "weak");
	else if (percentb < 20 && percentb > 0)
		sprintf(buf2, "barely any");
	else
		sprintf(buf2, "no remaining");

	if (percentc >= 100)
		sprintf(buf3, "maximum");
	else if (percentc < 100 && percentc >= 80)
		sprintf(buf3, "great");
	else if (percentc < 80 && percentc >= 60)
		sprintf(buf3, "strong");
	else if (percentc < 60 && percentc >= 40)
		sprintf(buf3, "moderate");
	else if (percentc < 40 && percentc >= 20)
		sprintf(buf3, "fading");
	else if (percentc < 20 && percentc > 0)
		sprintf(buf3, "crumbling");
	else
		sprintf(buf3, "no remaining");

	auto buffer = fmt::format("I am {}, with {} mental focus, and {} endurance.", buf, buf2, buf3); //TODO: change the rest of the sprintf calls to format
	do_say(ch, buffer.data());
}

void do_practice(CHAR_DATA *ch, char *argument)
{
	if (is_npc(ch))
		return;

	char buf[MAX_STRING_LENGTH];

	if (argument[0] == '\0')
	{
		/* Hi, you may have seen me in such films as Cheap Hack 1 and Cheap Hack
		 * 2: Electric boogaloo -cal */

		if (ch->Class()->GetIndex() == CLASS_NECROMANCER)
		{
			if (ch->pcdata->learned[skill_lookup("ritual of flesh")] > 0 && ch->pcdata->souls < 30)
				ch->pcdata->learned[skill_lookup("ritual of flesh")] = -2;
			if (ch->pcdata->learned[skill_lookup("ritual of soul")] > 0 && ch->pcdata->souls < 55)
				ch->pcdata->learned[skill_lookup("ritual of soul")] = -2;
			if (ch->pcdata->learned[skill_lookup("visceral divination")] && ch->pcdata->souls < 70)
				ch->pcdata->learned[skill_lookup("visceral divination")] = -2;
		}

		auto col = 0;
		for (auto sn = 0; sn < MAX_SKILL; sn++)
		{
			auto hide_skill= false;
			if (skill_table[sn].name == nullptr)
				break;

			for (auto p = 0; p < MAX_STYLE_SKILL; p++)
			{
				if (!str_cmp(style_percent[p].name, skill_table[sn].name))
				{
					auto gn = gn_skill_lookup(sn);
					if (gn > 1)
					{
						auto style_skill = skill_lookup(group_table[gn].name);
						if (ch->pcdata->learned[style_skill] < style_percent[p].percent)
						{
							hide_skill = true;
							break;
						}
					}
				}
			}

			if (sn == gsn_gladiator_skill
				|| sn == gsn_duelist_skill
				|| sn == gsn_barbarian_skill
				|| sn == gsn_dragoon_skill
				|| sn == gsn_skirmisher_skill
				|| sn == gsn_tactician_skill)
				hide_skill = true;

			if (skill_table[sn].skill_level[ch->Class()->GetIndex()] > 52)
				continue;

			if (ch->level < skill_table[sn].skill_level[ch->Class()->GetIndex()] || ch->pcdata->learned[sn] < 1)
				continue;

			if (!hide_skill)
			{
				sprintf(buf, "%-18s %3d%%  ", skill_table[sn].name, ch->pcdata->learned[sn]);
				send_to_char(buf, ch);

				if (++col % 3 == 0)
					send_to_char("\n\r", ch);
			}
		}

		if (col % 3 != 0)
			send_to_char("\n\r", ch);

		sprintf(buf, "You have %d practice sessions left.\n\r", ch->practice);
		send_to_char(buf, ch);

		if (is_immortal(ch) && get_trust(ch) > 58)
			send_to_char("Use 'set' to change class and see a prac list for other classes.\n\r", ch);

		return;
	}

	if (!is_awake(ch))
	{
		send_to_char("In your dreams, or what?\n\r", ch);
		return;
	}

	if (is_npc(ch))
		return;

	auto mob = ch->in_room->people;
	for (; mob != nullptr; mob = mob->next_in_room)
	{
		if (is_npc(mob) && IS_SET(mob->act, ACT_PRACTICE))
			break;
	}

	if (mob == nullptr)
	{
		send_to_char("You can't do that here.\n\r", ch);
		return;
	}

	if (ch->practice <= 0)
	{
		send_to_char("You have no practice sessions left.\n\r", ch);
		return;
	}

	auto sn = find_spell(ch, argument); 
	if (sn < 0
		|| (!is_npc(ch) 
			&& (ch->level < skill_table[sn].skill_level[ch->Class()->GetIndex()] 
			|| ch->pcdata->learned[sn] < 1))) /* skill is not known */
	{
		send_to_char("You can't practice that.\n\r", ch);
		return;
	}

	if (!str_prefix(argument, "skirmisher")
		|| !str_prefix(argument, "duelist")
		|| !str_prefix(argument, "tactician")
		|| !str_prefix(argument, "barbarian")
		|| (!str_prefix(argument, "dragoon") && str_prefix(argument, "drag"))
		|| !str_prefix(argument, "gladiator"))
	{
		send_to_char("You can't practice that.\n\r", ch);
		return;
	}

	for (auto p = 0; p < MAX_STYLE_SKILL; p++)
	{
		auto gn = gn_skill_lookup(sn);
		if (!str_cmp(style_percent[p].name, skill_table[sn].name) && gn > 1)
		{
			if (ch->pcdata->learned[skill_lookup(group_table[gn].name)] < style_percent[p].percent)
			{
				send_to_char("You can't practice that.\n\r", ch);
				return;
			}
		}
	}

	auto adept = is_npc(ch) ? 100 : 75;
	if (ch->pcdata->learned[sn] >= adept)
	{
		sprintf(buf, "You are already learned at %s.\n\r", skill_table[sn].name);
		send_to_char(buf, ch);
		return;
	}

	ch->practice--;

	auto mcap = 80;
	auto base = int_app[get_curr_stat(ch, STAT_INT)].learn;
	base *= mcap - ch->pcdata->learned[sn];
	base /= mcap;

	if (base < 1)
		base = 1;

	ch->pcdata->learned[sn] += base;

	if (skill_table[sn].ctype == CMD_SPELL || (skill_table[sn].ctype == CMD_BOTH && ch->Class()->ctype == CLASS_CASTER))
	{
		say_spell(mob, sn);
		act("As $n demonstrates the casting of $t, you gain new insight into the spell.", mob, skill_table[sn].name, ch, TO_VICT);
		act("$N watches studiously as $n demonstrates the $t spell.", mob, skill_table[sn].name, ch, TO_NOTVICT);
	}
	else if (skill_table[sn].ctype == CMD_COMMUNE || (skill_table[sn].ctype == CMD_BOTH && ch->Class()->ctype == CLASS_COMMUNER))
	{
		act("$n recites the prayer of $t.", mob, skill_table[sn].name, 0, TO_ROOM);
		send_to_char("You try to memorize the fundamentals of the holy recitation.\n\r", ch);
		act("$N watches studiously as $n demonstrates the $t prayer.", mob, skill_table[sn].name, ch, TO_NOTVICT);
	}
	else // if(skill_table[sn].ctype == CMD_NONE)
	{
		act("$n demonstrates the usage of the $t skill.", mob, skill_table[sn].name, 0, TO_ROOM);
		act("You study the fundamental aspects of $t.", ch, skill_table[sn].name, 0, TO_CHAR);
		act("$N studiously watches $n's demonstration of the $t skill.", mob, skill_table[sn].name, ch, TO_NOTVICT);
	}
	if (ch->pcdata->learned[sn] >= adept)
	{
		ch->pcdata->learned[sn] = adept;
		act("You have mastered the fundamental aspects of $T.", ch, nullptr, skill_table[sn].name, TO_CHAR);
	}
}

/*
 * 'Wimpy' originally by Dionysos.
 */
void do_wimpy(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	one_argument(argument, arg);

	auto wimpy = arg[0] == '\0'
		? ch->max_hit / 5
		: atoi(arg);

	if (wimpy < 0)
	{
		send_to_char("Your courage exceeds your wisdom.\n\r", ch);
		return;
	}

	if (wimpy > ch->max_hit / 2)
	{
		send_to_char("Such cowardice ill becomes you.\n\r", ch);
		return;
	}

	ch->wimpy = wimpy;

	char buf[MAX_STRING_LENGTH];
	sprintf(buf, "Wimpy set to %d hit points.\n\r", wimpy);
	send_to_char(buf, ch);
}

void do_password(CHAR_DATA *ch, char *argument)
{
	if (is_npc(ch))
		return;

	/*
	 * Can't use one_argument here because it smashes case.
	 * So we just steal all its code.  Bleagh.
	*/
	char arg1[MAX_INPUT_LENGTH];
	auto pArg = arg1;

	while (isspace(*argument))
	{
		argument++;
	}

	auto cEnd = *argument == '\'' || *argument == '"'
		? *argument++
		: ' ';

	while (*argument != '\0')
	{
		if (*argument == cEnd)
		{
			argument++;
			break;
		}

		*pArg++ = *argument++;
	}

	*pArg = '\0';

	char arg2[MAX_INPUT_LENGTH];
	pArg = arg2;

	while (isspace(*argument))
	{
		argument++;
	}

	cEnd = *argument == '\'' || *argument == '"'
		? *argument++
		: ' ';

	while (*argument != '\0')
	{
		if (*argument == cEnd)
		{
			argument++;
			break;
		}

		*pArg++ = *argument++;
	}

	*pArg = '\0';

	if (arg1[0] == '\0' || arg2[0] == '\0')
	{
		send_to_char( "Syntax: password <old> <new>.\n\r", ch );
		return;
	}

	if (strcmp(crypt(arg1, ch->pcdata->pwd), ch->pcdata->pwd))
	{
		WAIT_STATE(ch, 40);
		send_to_char("Wrong password.  Wait 10 seconds.\n\r", ch);
		return;
	}

	if (strlen(arg2) < 5)
	{
		send_to_char("New password must be at least five characters long.\n\r", ch);
		return;
	}

	/*
	 * No tilde allowed because of player file format.
	*/
	auto pwdnew = crypt(arg2, ch->name);
	for (auto p = pwdnew; *p != '\0'; p++)
	{
		if (*p == '~')
		{
			send_to_char("New password not acceptable, try again.\n\r", ch);
			return;
		}
	}

	free_pstring(ch->pcdata->pwd);
	ch->pcdata->pwd = palloc_string(pwdnew);

	save_char_obj(ch);
	send_to_char("Ok.\n\r", ch);
}

char *get_descr_form(CHAR_DATA *ch, CHAR_DATA *looker, bool get_long)
{
	if (get_long && !is_affected(ch, gsn_cloak_form))
		return ch->long_descr;

	return pers(ch, looker);
}

/* Last returns are redundancy ones in case of some failure in checks */

/* Replacement for the handler.c get_char_room to handle morph dragon */
CHAR_DATA *get_char_room(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	auto count = 0;
	auto number = number_argument(argument, arg);

	if (!str_cmp(arg, "self"))
		return ch;

	for (auto rch = ch->in_room->people; rch != nullptr; rch = rch->next_in_room)
	{
		if (argument[0] == '\0')
			return nullptr;

		if (!can_see(ch, rch))
			continue;

		if (arg[0] != '\0'
			&& !is_name(arg, rch->name)
			&& (!is_immortal(ch) || !rch->true_name || !is_name(arg, rch->true_name)))
			continue;

		if (++count == number)
			return rch;
	}

	return nullptr;
}

CHAR_DATA *get_char_from_room(CHAR_DATA *ch, ROOM_INDEX_DATA *room, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	auto count = 0;
	auto number = number_argument(argument, arg);

	if (!str_cmp(arg, "self"))
		return ch;

	for (auto rch = room->people; rch != nullptr; rch = rch->next_in_room)
	{
		if (!can_see(ch, rch))
			continue;

		if ((arg[0] != '\0')
			&& !is_name(arg, rch->name)
			&& (!is_immortal(ch) || !rch->true_name || !is_name(arg, rch->true_name)))
			continue;

		if (++count == number)
			return rch;
	}

	return nullptr;
}

void do_balance(CHAR_DATA *ch, char *argument)
{
	auto banker = ch->in_room->people;
	for (; banker != nullptr; banker = banker->next_in_room)
	{
		if (is_npc(banker) && IS_SET(banker->act, ACT_BANKER))
			break;
	}

	if (banker == nullptr)
	{
		send_to_char("You can't do that here.\n\r", ch);
		return;
	}

	char buf[100];
	sprintf(buf, "You have %ld gold coins in your account.\n\r", ch->gold_bank);

	if (ch->gold_bank == 0)
		sprintf(buf, "You have no account here!\n\r");

	send_to_char(buf, ch);
}

void do_withdraw(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];
	OBJ_INDEX_DATA *pIndex;

	auto banker = ch->in_room->people;
	for (; banker != nullptr; banker = banker->next_in_room)
	{
		if (IS_SET(banker->act, ACT_BANKER))
			break;
	}

	if (banker == nullptr || is_npc(ch))
	{
		send_to_char("You can't do that here.\n\r", ch);
		return;
	}

	char arg[MAX_STRING_LENGTH];
	argument = one_argument(argument, arg);

	if (arg[0] == '\0')
	{
		auto bFound = false;
		for (auto i = 0; i < MAX_STORED_ITEMS; i++)
		{
			if (ch->pcdata->deposited_items[i] > 0)
			{
				if (!bFound)
					act("$n hands you a slip of paper with a listing of your assets.", banker, 0, ch, TO_VICT);

				pIndex = get_obj_index(ch->pcdata->deposited_items[i]);
				if (!pIndex)
				{
					ch->pcdata->deposited_items[i] = 0;
					continue;
				}

				bFound = true;
				sprintf(buf, "%d) %s - estimated at %d gold\n\r",
					i + 1,
					pIndex->short_descr,
					std::max(1, pIndex->cost + number_range((int)(-.1 * pIndex->cost), (int)(.1 * pIndex->cost))));
				send_to_char(buf, ch);
			}
		}

		if (!bFound)
			mob_tell(banker, ch, "Sorry, you do not seem to have any items stored here.");

		return;
	}

	auto amount = atoi(arg);
	if (amount <= 0)
	{
		mob_tell(banker, ch, "Sorry, withdraw how much?");
		return;
	}

	if (amount > 0 && amount <= MAX_STORED_ITEMS)
	{
		auto i = ch->pcdata->deposited_items[amount - 1];
		if (i == 0 || (pIndex = get_obj_index(i)) == nullptr)
		{
			mob_tell(banker, ch, "I'm sorry, but I can't seem to find any record of that.");
			return;
		}

		act("$n nods briefly before scurrying away.", banker, 0, ch, TO_VICT);
		act("$n soon returns, followed by two guards hauling a heavy chest.", banker, 0, ch, TO_VICT);
		act("$n gets out a large set of keys and unlocks the chest.", banker, 0, ch, TO_VICT);

		auto pObj = create_object(pIndex, 0);
		if (!pObj)
			return;

		if (!can_see_obj(ch, pObj))
		{
			act("You are unable to find what you're looking for in the chest.", ch, 0, 0, TO_CHAR);
			extract_obj(pObj);
			return;
		}

		act("You retrieve $p from the storage chest.", ch, pObj, 0, TO_CHAR);
		obj_to_char(pObj, ch);
		ch->pcdata->deposited_items[amount - 1] = 0;

		for (i = 1; i < MAX_STORED_ITEMS; i++) // MUNCH.
		{
			if (ch->pcdata->deposited_items[i - 1] == 0 && ch->pcdata->deposited_items[i] > 0)
			{
				ch->pcdata->deposited_items[i - 1] = ch->pcdata->deposited_items[i];
				ch->pcdata->deposited_items[i] = 0;
			}
		}

		return;
	}

	if (ch->gold_bank < amount)
	{
		mob_tell(banker, ch, "Sorry, we don't give loans.");
		return;
	}

	auto charges = 0;

	ch->gold_bank -= amount;
	ch->gold += amount;
	ch->gold -= charges;

	sprintf(buf, "You withdraw %d gold.\n\r", amount);
	send_to_char(buf, ch);

	if (charges > 0)
	{
		sprintf(buf, "You are charged a fee of %d gold.\n\r", charges);
		send_to_char(buf, ch);
	}
}

void do_deposit(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];

	auto banker = ch->in_room->people;
	for (; banker != nullptr; banker = banker->next_in_room)
	{
		if (is_npc(banker) && IS_SET(banker->act, ACT_BANKER))
			break;
	}

	if (banker == nullptr || is_npc(ch))
	{
		send_to_char("You can't do that here.\n\r", ch);
		return;
	}

	char arg[MAX_STRING_LENGTH];
	one_argument(argument, arg);

	if (arg[0] == '\0')
	{
		send_to_char("Syntax: deposit <number of coins>\n\r        deposit <item>\n\r", ch);
		return;
	}

	auto deposited = get_obj_carry(ch, arg, ch);
	if (!is_number(arg) && deposited == nullptr)
	{
		send_to_char("You don't have that.\n\r", ch);
		return;
	}

	if (is_number(arg))
	{
		auto amount = atoi(arg);
		if (amount <= 0)
		{
			mob_tell(banker, ch, "Hey genius, around here we call that a 'with-draw-al'.");
			return;
		}

		if (ch->gold < amount)
		{
			mob_tell(banker, ch, "Sorry, we don't offer loans.");
			return;
		}

		if (amount + ch->gold_bank > ch->level * 4000)
		{
			mob_tell(banker, ch, "Sorry, we can't secure a deposit of that size for you.");
			return;
		}

		ch->gold_bank += (2 * amount) / 3;
		ch->gold -= amount;
		auto charges = amount / 3;

		sprintf(buf, "You deposit %d gold, less a %d gold transaction fee.\n\r", amount, charges);
		send_to_char(buf, ch);

		sprintf(buf, "Your new balance is %ld gold.\n\r", ch->gold_bank);
		send_to_char(buf, ch);
		return;
	}

	if (deposited->pIndexData->limtotal > 0)
	{
		act("$n examines $p with an appraising eye.", banker, deposited, ch, TO_VICT);
		mob_tell(banker, ch, "Sorry, we don't insure items of this rarity.");
		return;
	}

	auto bFull = true;
	auto i = 0;
	for (; i < MAX_STORED_ITEMS; i++)
	{
		if (ch->pcdata->deposited_items[i] == 0)
		{
			bFull= false;
			break;
		}
	}

	if (bFull)
	{
		do_say(banker, "I'm sorry, but we are unable to store any more of your belongings.");
		return;
	}

	auto charges = std::max(deposited->level * 15, deposited->cost / 8);

	if (ch->gold < charges)
	{
		do_say(banker, "You don't seem to have enough money to cover storage on this.");
		return;
	}

	act("$n examines $p at length.", banker, deposited, ch, TO_VICT);
	interpret(banker, "nod");

	sprintf(buf, "%s A pleasure to do business with you.", ch->name);
	do_tell(banker, buf);

	sprintf(buf, "You deposit %s at a cost of %d gold.\n\r", deposited->short_descr, charges);
	ch->gold -= charges;
	send_to_char(buf, ch);

	ch->pcdata->deposited_items[i] = deposited->pIndexData->vnum;
	extract_obj(deposited);
}

void do_records(CHAR_DATA *ch, char *argument)
{
	if (!is_immortal(ch))
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	char buf[MAX_STRING_LENGTH];
	char buf1[MAX_STRING_LENGTH];
	buf1[0] = '\0';
	sprintf(buf, "Listing of current active players :\n\r");

	auto count = 0;
	for (auto victim = char_list; victim != nullptr; victim = victim->next)
	{
		if (is_npc(victim))
			continue;

		if (victim->pcdata->wanteds > 0)
		{
			count++;
			sprintf(buf1, "%s has %d flags.\n\r", victim->name, victim->pcdata->wanteds);
			strcat(buf, buf1);
		}
	}

	if (count == 0)
		strcat(buf, "None in the realms right now have had a flag before.\n\r");

	send_to_char(buf, ch);
	send_to_char("\n\r", ch);
}

void do_ratings(CHAR_DATA *ch, char *argument)
{
	if (is_npc(ch))
		return;

	if (!(ch->level > LEVEL_HERO))
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	char buf[MAX_STRING_LENGTH];
	argument = one_argument(argument, buf);

	if (buf[0] == '\0')
	{
		send_to_char("syntax: ratings <char>\n\r", ch);
		return;
	}
	else
	{
		auto victim = get_char_world(ch, buf);
		if (victim == nullptr || is_npc(victim))
			return;

		send_to_char("\n\r", ch);
		act("$N's PK record:", ch, nullptr, victim, TO_CHAR);

		sprintf(buf, "Total PKills: %d\n\rGood Kills: %d\n\rNeutral Kills: %d\n\rEvil Kills: %d\n\r\n\rPKDeaths: %d\n\rMob Deaths: %d",
			victim->pcdata->kills[PK_KILLS],
			victim->pcdata->kills[PK_GOOD],
			victim->pcdata->kills[PK_NEUTRAL],
			victim->pcdata->kills[PK_EVIL],
			victim->pcdata->killed[PK_KILLED],
			victim->pcdata->killed[MOB_KILLED]);
		send_to_char(buf, ch);
		send_to_char("\n\r", ch);
	}
}

void do_lore(CHAR_DATA *ch, char *argument) /* Lore by Detlef */
{
	char object_name[MAX_INPUT_LENGTH + 100];
	argument = one_argument(argument, object_name);

	if (get_skill(ch, gsn_lore) <= 1)
	{
		send_to_char("You don't know how to do that.\n\r", ch);
		return;
	}

	if (object_name[0] == '\0')
	{
		send_to_char("Lore what?\n\r", ch);
		return;
	}

	auto obj = get_obj_carry(ch, object_name, ch);
	if (obj == nullptr)
	{
		send_to_char("You are not carrying that.\n\r", ch);
		return;
	}

	if (ch->mana < std::max(obj->level - 10, 15))
	{
		send_to_char("You're too tired to examine it right now.\n\r", ch);
		return;
	}

	auto lorebonus = ch->Profs()->GetProf("forgotten lore");
	if (!is_npc(ch) && lorebonus > -1)
	{
		send_to_char("Your knowledge of forgotten lore grants you additional insight.\n\r", ch);
		ch->Profs()->CheckImprove("forgotten lore", 100);
	}

	lorebonus = std::max(lorebonus, 0);

	act("You examine $p intently.", ch, obj, 0, TO_CHAR);

	if (number_percent() < get_skill(ch, gsn_lore) + lorebonus * 15)
	{
		char buf[MAX_STRING_LENGTH];
		auto buffer = fmt::format("Object '{}' is type {}.\n\rIt weighs {} pounds.\n\r",
			obj->name, 
			item_name_lookup(obj->item_type),
			std::to_string(obj->weight)); //TODO: change the rest of the sprintf calls to format
		send_to_char(buffer.c_str(), ch);

		sprintf(buf, "It is level %d.\n\r", obj->level);

		if (lorebonus > 0)
			send_to_char(buf, ch);

		switch (obj->item_type)
		{
			case ITEM_SCROLL:
			case ITEM_POTION:
			case ITEM_PILL:
				if (lorebonus > 0)
				{
					sprintf(buf, "It is enchanted with level %d spells of:", obj->value[0]);
					send_to_char(buf, ch);

					for (auto j = 1; j < 5; j++)
					{
						if (obj->value[j] >= 0
							&& obj->value[j] < MAX_SKILL
							&& str_cmp(skill_table[obj->value[j]].name, "reserved"))
						{
							send_to_char(" '", ch);
							send_to_char(skill_table[obj->value[j]].name, ch);
							send_to_char("'", ch);
						}
					}
					send_to_char(".\n\r", ch);
				}
				break;
			case ITEM_DRINK_CON:
				sprintf(buf, "It holds %s-colored %s.\n\r", liq_table[obj->value[2]].liq_color, liq_table[obj->value[2]].liq_name);
				send_to_char(buf, ch);
				break;
			case ITEM_CONTAINER:
				sprintf(buf, "It can contain %d objects at a maximum of %d pounds.\n\r", obj->value[0], obj->value[3]);
				send_to_char(buf, ch);
				break;
			case ITEM_WEAPON:
				send_to_char("It is a", ch);
				switch (obj->value[0])
				{
					case WEAPON_EXOTIC:
						send_to_char("n exotic.\n\r", ch);
						break;
					case WEAPON_SWORD:
						send_to_char(" sword.\n\r", ch);
						break;
					case WEAPON_DAGGER:
						send_to_char(" dagger.\n\r", ch);
						break;
					case WEAPON_SPEAR:
						send_to_char(" spear.\n\r", ch);
						break;
					case WEAPON_MACE:
						send_to_char(" mace.\n\r", ch);
						break;
					case WEAPON_AXE:
						send_to_char("n axe.\n\r", ch);
						break;
					case WEAPON_FLAIL:
						send_to_char(" flail.\n\r", ch);
						break;
					case WEAPON_WHIP:
						send_to_char(" whip.\n\r", ch);
						break;
					case WEAPON_POLEARM:
						send_to_char(" polearm.\n\r", ch);
						break;
					case WEAPON_STAFF:
						send_to_char(" staff.\n\r", ch);
						break;
					default:
						send_to_char("n unknown.\n\r", ch);
						break;
				}

				sprintf(buf, "Damage is %dd%d (average %d).\n\r",
					obj->value[1],
					obj->value[2],
					(1 + obj->value[2]) * obj->value[1] / 2);
				send_to_char(buf, ch);

				if (obj->value[4] && lorebonus) /* weapon flags */
				{
					sprintf(buf, "Weapon specials: %s\n\r", weapon_bit_name(obj->value[4]));
					send_to_char(buf, ch);
				}

				break;

			case ITEM_ARMOR:
				sprintf(buf, "Armor class is %d pierce, %d bash, %d slash, and %d vs. magic.\n\r",
					obj->value[0],
					obj->value[1],
					obj->value[2],
					obj->value[3]);
				send_to_char(buf, ch);
				break;
		}

		for (auto app = obj->apply; app; app = app->next)
		{
			sprintf(buf, "It affects your %s by %d.\n\r", affect_loc_name(app->location), app->modifier);
			send_to_char(buf, ch);
		}

		check_improve(ch, gsn_lore, true, 2);
	}
	else
	{
		act("You examine $p closely but fail to gain any insight into it.", ch, obj, 0, TO_CHAR);
		check_improve(ch, gsn_lore, false, 2);
	}

	ch->mana -= std::max(obj->level - 10 - (lorebonus * 3), 15);
}

bool isCabalItem(OBJ_DATA *obj)
{
	auto ovnum = obj->pIndexData->vnum;

	for (auto i = 1; i < MAX_CABAL; i++)
	{
		if (cabal_table[i].item_vnum == ovnum)
			return true;
	}

	return false;
}

bool isNewbie(CHAR_DATA *ch)
{
	/*
	if (is_npc(ch))
		return false;

	return ch->pcdata->newbie;
	*/
	return false;
}

void debug_string(const char *str)
{
	auto fp = fopen(DEBUG_LOG_FILE, "a");

	if (fp != nullptr)
	{
		fprintf(fp, "DEBUG :: %s\n", str);
		fclose(fp);
	}
}

void do_xlook(CHAR_DATA *ch, char *argument)
{
	if (argument[0] == '\0')
	{
		send_to_char("Syntax: xlook <char>\n\r", ch);
		return;
	}

	auto victim = get_char_world(ch, argument);
	if (victim == nullptr)
	{
		send_to_char("No such player found.\n\r", ch);
		return;
	}

	if (!can_see(ch, victim))
	{
		send_to_char("No such player found.\n\r", ch);
		return;
	}

	char buf[MAX_STRING_LENGTH];
	sprintf(buf, "%s is at '%s' (%d) of area '%s'",
		is_npc(victim) ? victim->short_descr : victim->name,
		get_room_name(victim->in_room),
		victim->in_room->vnum,
		victim->in_room->area->name);

	send_to_char(buf, ch);
	send_to_char("\n\r", ch);

	sprintf(buf, "%ihp/%ihp  %im/%im  %imv/%imv\n\r",
		ch->hit,
		ch->max_hit,
		ch->mana,
		ch->max_mana,
		ch->move,
		ch->max_move);

	send_to_char("In room: ", ch);

	auto foundIR = 0;
	for (auto vch = victim->in_room->people; vch != nullptr; vch = vch->next_in_room)
	{
		if (vch == victim)
			continue;

		if (is_npc(vch))
			continue;

		if (!can_see(ch, vch))
			continue;

		if (is_same_group(vch, victim))
			send_to_char("(GROUP)", ch);

		send_to_char(vch->name, ch);
		send_to_char(" ", ch);
		foundIR++;
	}

	if (foundIR == 0)
		send_to_char("none.", ch);

	if (victim->fighting != nullptr)
	{
		send_to_char("\n\rFighting: ", ch);
		send_to_char((is_npc(victim->fighting) ? victim->fighting->short_descr : victim->fighting->name), ch);
	}
	else
	{
		send_to_char("\n\rFighting: none.", ch);
	}

	if (victim->description)
	{
		sprintf(buf, "\n\rDescription: %s", victim->description);
		send_to_char(buf, ch);
	}

	send_to_char("\n\rEquipment:\n\r", ch);

	for (auto iWear = 0; iWear < MAX_WEAR; iWear++)
	{
		auto obj = get_eq_char(victim, iWear);
		if (obj != nullptr && can_see_obj(ch, obj))
		{
			send_to_char(where_name[iWear], ch);
			send_to_char(format_obj_to_char(obj, ch, true), ch);
			send_to_char("\n\r", ch);
		}
	}

	send_to_char("\n\r", ch);
	send_to_char("Inventory:\n\r", ch);
	show_list_to_char(victim->carrying, ch, true, false);
}

char *get_where_name(int iWear)
{
	return where_name[iWear];
}

void do_trustgroup(CHAR_DATA *ch, char *argument)
{
	if (is_npc(ch))
		return;

	TOGGLE_BIT(ch->pcdata->trust, TRUST_GROUP);

	if (IS_SET(ch->pcdata->trust, TRUST_GROUP))
		send_to_char("You now trust your group with questionable actions.\n\r", ch);

	if (!IS_SET(ch->pcdata->trust, TRUST_GROUP))
		send_to_char("You no longer trust your group with questionable actions.\n\r", ch);
}

void do_trustcabal(CHAR_DATA *ch, char *argument)
{
	if (is_npc(ch))
		return;

	TOGGLE_BIT(ch->pcdata->trust, TRUST_CABAL);

	if (IS_SET(ch->pcdata->trust, TRUST_CABAL))
		send_to_char("You now trust your cabal with questionable actions.\n\r", ch);

	if (!IS_SET(ch->pcdata->trust, TRUST_CABAL))
		send_to_char("You no longer trust your cabal with questionable actions.\n\r", ch);
}

void do_trustchar(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim = get_char_room(ch, argument);

	if (is_npc(ch))
		return;

	if (!victim && str_cmp(argument, "self"))
	{
		send_to_char("Syntax: trust <character>\n\r        trust self", ch);
		return;
	}

	if (is_npc(victim))
	{
		send_to_char("They are not interested in your trust.\n\r", ch);
		return;
	}

	if (!str_cmp(argument, "self"))
	{
		if (!ch->pcdata->trusting)
		{
			send_to_char("You are not trusting anybody specific.\n\r", ch);
			return;
		}

		act("You no longer trust $N with questionable actions.", ch, 0, ch->pcdata->trusting, TO_CHAR);
		ch->pcdata->trusting = nullptr;
		return;
	}

	ch->pcdata->trusting = victim;
	act("You now trust $N with questionable actions.", ch, 0, victim, TO_CHAR);
}

void do_role(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_BUF], arg1[MAX_BUF], obuf[MAX_BUF];

	if (is_npc(ch))
		return;

	if (argument[0] != '\0')
	{
		buf[0] = '\0';
		smash_tilde(argument);
		argument = one_argument(argument, arg1);

		if (!strcmp(arg1, "show") && is_immortal(ch) && get_trust(ch) > 52)
		{
			auto victim = get_char_world(ch, argument);
			if (victim == nullptr)
			{
				send_to_char("They aren't here.\n\r", ch);
				return;
			}

			if (is_npc(victim))
			{
				send_to_char("You can't role a mob.\n\r", ch);
				return;
			}

			send_to_char(victim->true_name, ch);
			send_to_char("'s role:\n\r", ch);

			if (victim->pcdata->role == nullptr || victim->pcdata->role[0] == '\0')
			{
				send_to_char("No role available.\n\r", ch);
				return;
			}

			auto output = new_buf();
			add_buf(output, victim->pcdata->role);
			page_to_char(buf_string(output), ch);
			free_buf(output);
			return;
		}

		if (!strcmp(arg1, "-"))
		{
			if (ch->pcdata->temp_role == nullptr || ch->pcdata->temp_role[0] == '\0')
			{
				send_to_char("No lines left to remove.\n\r", ch);
				return;
			}

			strcpy(obuf, ch->pcdata->temp_role);

			auto found = false;
			for (auto len = strlen(obuf); len > 0; len--)
			{
				if (obuf[len] != '\r')
					continue;

				if (!found)
				{
					if (len > 0)
						len--;

					found = true;
				}
				else
				{
					obuf[len + 1] = '\0';
					free_pstring(ch->pcdata->temp_role);
					ch->pcdata->temp_role = palloc_string(obuf);

					send_to_char("Line deleted.\n\r", ch);
					return;
				}
			}

			obuf[0] = '\0';
			free_pstring(ch->pcdata->temp_role);
			ch->pcdata->temp_role = palloc_string(obuf);

			send_to_char("Line deleted.\n\r", ch);
			return;
		}

		if (!strcmp(arg1, "+"))
		{
			obuf[0] = '\0';

			if (ch->pcdata->temp_role)
				strcat(obuf, ch->pcdata->temp_role);

			strcat(obuf, argument[0] != '\0' ? argument : " ");
			strcat(obuf, "\n\r");

			if (strlen(obuf) + (ch->pcdata->role ? strlen(ch->pcdata->role) : 0) > MAX_BUF - 1000)
			{
				send_to_char("Sorry, your role is too long.\n\r", ch);
				return;
			}

			free_pstring(ch->pcdata->temp_role);
			ch->pcdata->temp_role = palloc_string(obuf);

			send_to_char("Ok.\n\r", ch);
			return;
		}

		if (!strcmp(arg1, "add"))
		{
			if (ch->pcdata->temp_role == nullptr || ch->pcdata->temp_role[0] == '\0')
			{
				send_to_char("You have no role in progress to add.\n\r", ch);
				return;
			}
			else
			{
				add_role(ch, ch->pcdata->temp_role);
				free_pstring(ch->pcdata->temp_role);
				ch->pcdata->temp_role = nullptr;
				show_role(ch);
			}

			return;
		}

		if (!str_cmp(arg1, "format"))
		{
			if (!ch->pcdata->temp_role)
			{
				send_to_char("You have no role in progress to format.\n\r", ch);
				return;
			}

			ch->pcdata->temp_role = format_string(ch->pcdata->temp_role);

			send_to_char("Your role in progress has been formatted.\n\r", ch);
			return;
		}
	}

	show_role(ch);

	if (ch->pcdata->temp_role != nullptr)
		show_temp_role(ch);
}

void add_role(CHAR_DATA *ch, char *string)
{
	char buf[MAX_BUF], obuf[MAX_BUF];

	auto strtime = ctime(&current_time);
	chomp(strtime);

	if (ch->pcdata->role)
		sprintf(buf, "%s", ch->pcdata->role);
	else
		strcpy(buf, "");

	sprintf(obuf, "Role entry added on %s at level %d (%d hours):\n\r",
		strtime,
		ch->level,
		(int)((ch->played + current_time - ch->logon) / 3600));

	strcat(buf, obuf);
	strcat(buf, string);
	strcat(buf, "\n\r");

	free_pstring(ch->pcdata->role);
	ch->pcdata->role = palloc_string(buf);
}

void show_temp_role(CHAR_DATA *ch)
{
	send_to_char("Role entry in progress:\n\r", ch);

	if (ch->pcdata->temp_role == nullptr || ch->pcdata->temp_role[0] == '\0')
	{
		send_to_char("You have no role in progress.\n\r", ch);
	}
	else
	{
		auto output = new_buf();
		add_buf(output, ch->pcdata->temp_role);
		page_to_char(buf_string(output), ch);
		free_buf(output);
	}
}

void show_role(CHAR_DATA *ch)
{
	send_to_char("Your role:\n\r", ch);

	if (ch->pcdata->role == nullptr || ch->pcdata->role[0] == '\0')
	{
		send_to_char("(Empty)\n\r", ch);
	}
	else
	{
		auto output = new_buf();
		add_buf(output, ch->pcdata->role);
		page_to_char(buf_string(output), ch);
		free_buf(output);
	}
}
