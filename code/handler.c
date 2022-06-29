/**************************************************************************r
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
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <algorithm>
#include "handler.h"
#include "magic.h"
#include "recycle.h"
#include "tables.h"
#include "spec.h"
#include "act_comm.h"
#include "act_info.h"
#include "act_obj.h"
#include "act_wiz.h"
#include "warrior.h"
#include "fight.h"
#include "skill.h"
#include "newmem.h"
#include "comm.h"
#include "interp.h"
#include "db.h"
#include "misc.h"
#include "chardef.h"
#include "const.h"
#include "material.h"
#include "utility.h"
#include "sorcerer.h"
#include "skills.h"

/* friend stuff -- for NPC's mostly */
bool is_friend(CHAR_DATA *ch, CHAR_DATA *victim)
{
	if (is_same_group(ch, victim))
		return true;

	if (!is_npc(ch))
		return false;

	if (!is_npc(victim))
	{
		if (IS_SET(ch->off_flags, ASSIST_PLAYERS))
			return true;
		else
			return false;
	}

	if (is_affected_by(ch, AFF_CHARM))
		return false;

	if (IS_SET(ch->off_flags, ASSIST_ALL))
		return true;

	if (ch->group && ch->group == victim->group)
		return true;

	if (IS_SET(ch->off_flags, ASSIST_VNUM) && ch->pIndexData == victim->pIndexData)
		return true;

	if (IS_SET(ch->off_flags, ASSIST_RACE) && ch->race == victim->race)
		return true;

	if (IS_SET(ch->off_flags, ASSIST_ALIGN)
		&& !IS_SET(ch->act, ACT_NOALIGN)
		&& !IS_SET(victim->act, ACT_NOALIGN)
		&& ((is_good(ch) && is_good(victim))
			|| (is_evil(ch) && is_evil(victim))
			|| (is_neutral(ch) && is_neutral(victim))))
	{
		return true;
	}

	return false;
}

/* returns number of people on an object */
int count_users(OBJ_DATA *obj)
{
	CHAR_DATA *fch;
	int count = 0;

	if (obj->in_room == nullptr)
		return 0;

	for (fch = obj->in_room->people; fch != nullptr; fch = fch->next_in_room)
	{
		if (fch->on == obj)
			count++;
	}

	return count;
}

/// Queries the race table for the index of the given race.
/// @param name: The name of the race to query.
/// @returns The table index of the given race. (Default: -1)
int race_lookup(const char *name)
{
	auto it = std::find_if(race_table.begin(), race_table.end(), [name] (auto race) {
		return race.name != nullptr && !str_prefix(name, race.name);
	});

	return it != race_table.end()
		? it - race_table.begin()
		: -1;
}

int act_lookup(const char *name)
{
	int act;

	// TODO: change act_flags from array to vector. requires detailed refactoring.
	for (act = 0; act_flags[act].name != nullptr; act++)
	{
		if (LOWER(name[0]) == LOWER(act_flags[act].name[0]) && !str_prefix(name, act_flags[act].name))
			return act;
	}

	return -1;
}

/// Queries the liquid table for the index of the given liquid.
/// @param name: The name of the liquid to query.
/// @returns The table index of the given liquid. (Default: 0)
int liq_lookup(const char *name)
{
	auto it = std::find_if(liq_table.begin(), liq_table.end(), [name] (auto liquid) {
		return liquid.liq_name != nullptr && !str_prefix(name, liquid.liq_name);
	});

	return it != liq_table.end()
		? it - liq_table.begin()
		: 0;
}

/// Queries the weapon table for the index of the given weapon.
/// @param name: The name of the weapon to query.
/// @returns The table index of the given weapon. (Default: -1)
int weapon_lookup(const char *name)
{
	auto it = std::find_if(weapon_table.begin(), weapon_table.end(), [name] (auto weapon) {
		return weapon.name != nullptr && !str_prefix(name, weapon.name);
	});

	return it != weapon_table.end()
		? it - weapon_table.begin()
		: -1;
}

/// Queries the weapon table for the type of the given weapon.
/// @param name: The name of the weapon to query.
/// @returns The type of the given weapon. (Default: WEAPON_EXOTIC)
int weapon_type_lookup(const char *name)
{
	auto idx = weapon_lookup(name);
	return idx > -1 || idx < weapon_table.size()
		? weapon_table[idx].type
		: WEAPON_EXOTIC;
}

/// Queries the weapon table for the name of the given weapon type.
/// @param type: The type of the weapon to query.
/// @param default_name (optional) If the weapon is not found, the default_name is returned. (Default: weapon).
/// @returns The type of the given weapon. (Default: default_name)
char *weapon_name_lookup(int type, char* default_name)
{
	auto it = std::find_if(weapon_table.begin(), weapon_table.end(), [type] (auto weapon) {
		return weapon.name != nullptr && type == weapon.type;
	});

	if (it == weapon_table.end())
		return default_name;

	auto idx = it - weapon_table.begin();
	return weapon_table[idx].name;
}

/// Queries the item table for the type of the given item.
/// @param name: The name of the item to query.
/// @returns The type of the given item. (Default: -1)
int item_lookup(const char *name)
{
	auto it = std::find_if(item_table.begin(), item_table.end(), [name] (auto item) {
		return item.name != nullptr && !str_prefix(name, item.name);
	});

	if (it == item_table.end())
		return -1;

	auto idx = it - item_table.begin();
	return item_table[idx].type;
}

/// Queries the item table for the name of the given item type.
/// @param item_type: The type of the item to query.
/// @returns The name of the given item type. (Default: none)
char *item_name_lookup(int item_type)
{
	auto it = std::find_if(item_table.begin(), item_table.end(), [item_type] (auto item) {
		return item.name != nullptr && item_type == item.type;
	});

	if (it == item_table.end())
		return "none";

	auto idx = it - item_table.begin();
	return item_table[idx].name;
}

/// Queries the attack table for the index of the given attack.
/// @param name: The name of the attack to query.
/// @returns The table index of the given attack. (Default: 0)
int attack_lookup(const char *name)
{
	auto it = std::find_if(attack_table.begin(), attack_table.end(), [name] (auto attack) {
		return attack.name != nullptr && !str_prefix(name, attack.name);
	});

	return it != attack_table.end()
		? it - attack_table.begin()
		: 0;
}

/// Queries the wiznet table for the index of the given wiznet flag.
/// @param name: The name of the wiznet flag to query.
/// @returns The table index of the given wiznet flag. (Default: -1)
long wiznet_lookup(const char *name)
{
	auto it = std::find_if(wiznet_table.begin(), wiznet_table.end(), [name] (auto wiznet) {
		return wiznet.name != nullptr && !str_prefix(name, wiznet.name);
	});

	return it != wiznet_table.end()
		? it - wiznet_table.begin()
		: -1;
}

char *color_value_string(int color, bool bold, bool flash)
{
	static char buf[64];
	static short COLOR_INDEX[8] = {30, 34, 32, 31, 36, 35, 33, 37};

	if (flash && bold)
		sprintf(buf, "\x01B[1;%dm%s", COLOR_INDEX[color % 8], ANSI_BLINK);
	else if (flash)
		sprintf(buf, "\x01B[0;%dm%s", COLOR_INDEX[color % 8], ANSI_BLINK);
	else if (bold)
		sprintf(buf, "\x01B[1;%dm", COLOR_INDEX[color % 8]);
	else
		sprintf(buf, "\x01B[0;%dm", COLOR_INDEX[color % 8]);

	return buf;
}

int strlen_color(char *argument)
{
	char *str;
	short length;

	if (argument == nullptr || argument[0] == '\0')
		return 0;

	length = 0;
	str = argument;

	while (*str != '\0')
	{
		if (*str != '{')
		{
			str++;
			length++;
			continue;
		}

		switch (*(++str))
		{
			case '-':
			case 'x':
				length++;
				break;
			case '<':
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case 'B':
			case 'b':
			case 'F':
			case 'f':
			case 'n':
				break;
			default:
				length += 2;
				break;
		}

		str++;
	}

	return length;
}

/* for immunity, vulnerabiltiy, and resistant
   the 'globals' (magic and weapons) may be overriden
   three other cases -- wood, silver, and iron -- are checked in fight.c */

int check_immune(CHAR_DATA *ch, int dam_type)
{
	int immune, def;
	int bit;

	immune = -1;
	def = IS_NORMAL;

	if (dam_type == DAM_NONE)
		return immune;

	if (dam_type <= 3)
	{
		if (IS_SET(ch->imm_flags, IMM_WEAPON))
			def = IS_IMMUNE;
		else if (IS_SET(ch->res_flags, RES_WEAPON))
			def = IS_RESISTANT;
		else if (IS_SET(ch->vuln_flags, VULN_WEAPON))
			def = IS_VULNERABLE;
	}
	else /* magical attack */
	{
		if (IS_SET(ch->imm_flags, IMM_MAGIC))
			def = IS_IMMUNE;
		else if (IS_SET(ch->res_flags, RES_MAGIC))
			def = IS_RESISTANT;
		else if (IS_SET(ch->vuln_flags, VULN_MAGIC))
			def = IS_VULNERABLE;
	}

	/* set bits to check -- VULN etc. must ALL be the same or this will fail */
	switch (dam_type)
	{
		case DAM_BASH:
			bit = IMM_BASH;
			break;
		case DAM_PIERCE:
			bit = IMM_PIERCE;
			break;
		case DAM_SLASH:
			bit = IMM_SLASH;
			break;
		case DAM_FIRE:
			bit = IMM_FIRE;
			break;
		case DAM_COLD:
			bit = IMM_COLD;
			break;
		case DAM_LIGHTNING:
			bit = IMM_LIGHTNING;
			break;
		case DAM_ACID:
			bit = IMM_ACID;
			break;
		case DAM_POISON:
			bit = IMM_POISON;
			break;
		case DAM_NEGATIVE:
			bit = IMM_NEGATIVE;
			break;
		case DAM_HOLY:
			bit = IMM_HOLY;
			break;
		case DAM_ENERGY:
			bit = IMM_ENERGY;
			break;
		case DAM_MENTAL:
			bit = IMM_MENTAL;
			break;
		case DAM_DISEASE:
			bit = IMM_DISEASE;
			break;
		case DAM_DROWNING:
			bit = IMM_DROWNING;
			break;
		case DAM_LIGHT:
			bit = IMM_LIGHT;
			break;
		case DAM_CHARM:
			bit = IMM_CHARM;
			break;
		case DAM_SOUND:
			bit = IMM_SOUND;
			break;
		case DAM_INTERNAL:
			bit = IMM_INTERNAL;
			break;
		default:
			return def;
	}

	if (IS_SET(ch->imm_flags, bit))
	{
		immune = IS_IMMUNE;
	}
	else if (IS_SET(ch->res_flags, bit) && immune != IS_IMMUNE)
	{
		immune = IS_RESISTANT;
	}
	else if (IS_SET(ch->vuln_flags, bit))
	{
		if (immune == IS_IMMUNE)
			immune = IS_RESISTANT;
		else if (immune == IS_RESISTANT)
			immune = IS_NORMAL;
		else
			immune = IS_VULNERABLE;
	}

	if (immune == -1)
		return def;
	else
		return immune;
}

bool is_cabal(CHAR_DATA *ch)
{
	return ch->cabal;
}

bool is_same_cabal(CHAR_DATA *ch, CHAR_DATA *victim)
{
	if (cabal_table[ch->cabal].independent)
		return false;
	else
		return (ch->cabal == victim->cabal);
}

/* checks mob format */
bool is_old_mob(CHAR_DATA *ch)
{
	if (ch->pIndexData == nullptr)
		return false;
	else if (ch->pIndexData->new_format)
		return false;

	return true;
}

///
/// Returns the skill level of a character given a skill GSN. 
/// @param ch: The character with the skill
/// @param sn: The GSN for a skill 
/// @returns A skill between 0 and 100, defaulting to 0 if the character does not have the skill.
///
int get_skill(CHAR_DATA *ch, int sn)
{
	int skill = 0, gn = 0;
	AFFECT_DATA *af;
	bool using_switched= false;
	CHAR_DATA *original = ch;

	if (is_npc(ch) && ch->desc && ch->desc->original && IS_SET(ch->comm, COMM_SWITCHSKILLS))
		using_switched = true;

	if (using_switched)
		ch = ch->desc->original;

	if (sn == -1) /* shorthand for level based skills */
	{
		skill = ch->level * 5 / 2;
	}
	else if (sn < -1 || sn > MAX_SKILL)
	{
		bug("Bad sn %d in get_skill.", sn);
		skill = 0;
	}
	else if (!is_npc(ch))
	{
		if (ch->level < skill_table[sn].skill_level[ch->Class()->GetIndex()] && !is_immortal(ch))
			skill = 0;
		else
			skill = ch->pcdata->learned[sn];

		if (skill == -2)
			skill = 0;
	}
	else if (ch->pIndexData->Class()->GetIndex() > CLASS_NONE)
	{
		if (ch->level < skill_table[sn].skill_level[ch->pIndexData->Class()->GetIndex()])
		{
			skill = 0;
		}
		else if (ch->pIndexData->Class()->GetIndex() == CLASS_WARRIOR)
		{
			if (!style_check(sn, ch->mobstyle))
				skill = 0;
			else
				skill = URANGE(0, 50 + 2 * (ch->level - skill_table[sn].skill_level[ch->pIndexData->Class()->GetIndex()]), 100);
		}
		else if (ch->pIndexData->Class()->GetIndex() == CLASS_SORCERER)
		{
			//TODO: what logic for sorcerers goes here?
		}
		else
		{
			skill = URANGE(0, 50 + 2 * (ch->level - skill_table[sn].skill_level[ch->pIndexData->Class()->GetIndex()]), 100);
		}
	}
	else
	{
		if (skill_table[sn].spell_fun != spell_null)
		{
			skill = 40 + 2 * ch->level;
		}
		else if (sn == gsn_sneak)
		{
			skill = ch->level * 2 + 20;
		}
		else if ((sn == gsn_dodge && IS_SET(ch->off_flags, OFF_DODGE))
			|| (sn == gsn_parry && IS_SET(ch->off_flags, OFF_PARRY)))
		{
			skill = ch->level * 2;
		}
		else if (sn == gsn_shield_block
			|| sn == gsn_throw
			|| sn == gsn_hand_to_hand
			|| sn == gsn_kick
			|| sn == gsn_rescue
			|| sn == gsn_ground_control
			|| sn == gsn_throw
			|| sn == gsn_blindness_dust
			|| sn == gsn_poison_dust)
		{
			skill = 25 + 2 * ch->level;
		}
		else if ((sn == gsn_trip && IS_SET(ch->off_flags, OFF_TRIP))
			|| (sn == gsn_bash && IS_SET(ch->off_flags, OFF_BASH))
			|| (sn == gsn_disarm && IS_SET(ch->off_flags, OFF_DISARM))
			|| (sn == gsn_berserk && IS_SET(ch->off_flags, OFF_BERSERK))
			|| (sn == gsn_tail && IS_SET(ch->off_flags, OFF_TAIL)))
		{
			skill = 35 + 2 * ch->level;
		}
		else if (sn == gsn_sword
			|| sn == gsn_dagger
			|| sn == gsn_spear
			|| sn == gsn_mace
			|| sn == gsn_axe
			|| sn == gsn_flail
			|| sn == gsn_whip
			|| sn == gsn_polearm)
		{
			skill = 50 + 5 * ch->level / 2;
		}
		else if ((gn = gn_skill_lookup(sn)) > 1 && IS_SET(ch->progtypes, MPROG_FIGHT))
		{

			if ((!str_cmp(group_table[gn].name, "gladiator") && !str_cmp(ch->pIndexData->mprogs->fight_name, "fight_prog_gladiator"))
				|| (!str_cmp(group_table[gn].name, "barbarian") && !str_cmp(ch->pIndexData->mprogs->fight_name, "fight_prog_barbarian"))
				|| (!str_cmp(group_table[gn].name, "duelist") && !str_cmp(ch->pIndexData->mprogs->fight_name, "fight_prog_duelist"))
				|| (!str_cmp(group_table[gn].name, "skirmisher") && !str_cmp(ch->pIndexData->mprogs->fight_name, "fight_prog_skirmisher"))
				|| (!str_cmp(group_table[gn].name, "dragoon") && !str_cmp(ch->pIndexData->mprogs->fight_name, "fight_prog_dragoon"))
				|| (!str_cmp(group_table[gn].name, "tactician") && !str_cmp(ch->pIndexData->mprogs->fight_name, "fight_prog_tactician")))
			{
				skill = ch->level + 50;
			}
		}
		else
		{
			skill = 0;
		}
	}

	if (!is_npc(ch) && ch->pcdata->condition[COND_DRUNK] > 10)
		skill = 9 * skill / 10;

	// note: AFTER THIS NPCS WITH SWITCHED SKILLS RETURN TO THEIR NPC STATUS
	if (using_switched)
		ch = original;

	if (is_affected(ch, gsn_forget))
		if (number_percent() > skill / 2)
			skill /= 3;

	if (skill > 5 && is_affected(ch, gsn_synaptic_enhancement))
		skill += 5;

	if (skill > 5 && is_affected(ch, gsn_synaptic_impairment))
		skill -= 5;

	if (is_affected(ch, gsn_leadership))
		skill += (int)(skill * .1);

	if (is_affected_room(ch->in_room, gsn_infidels_fate) && is_good(ch))
		skill += (int)(skill * .1);

	if (ch->fighting && is_affected(ch->fighting, gsn_traitors_luck))
	{
		af = affect_find(ch->fighting->affected, gsn_traitors_luck);

		if (ch == af->owner)
			skill += 20;
	}

	if (ch->fighting
		&& (is_evil(ch) && is_affected(ch->fighting, gsn_awe))
		&& ch->level > ch->fighting->level
		&& number_percent() > 96)
	{
		skill = 0;
		send_to_char("Distracted by your opponent's aura of holiness, you lose your concentration.\n\r", ch);
	}

	return URANGE(0, skill, 100);
}

/* for returning weapon information */
int get_weapon_sn(CHAR_DATA *ch)
{
	return get_weapon_sn_new(ch, 0);
}

int get_weapon_sn_new(CHAR_DATA *ch, int type)
{
	OBJ_DATA *wield;
	int sn;

	wield = get_eq_char(ch, WEAR_WIELD);
	if (type == gsn_dual_wield)
		wield = get_eq_char(ch, WEAR_DUAL_WIELD);

	if (wield == nullptr || wield->item_type != ITEM_WEAPON)
	{
		sn = gsn_hand_to_hand;
	}
	else
	{
		switch (wield->value[0])
		{
			case WEAPON_SWORD:
				sn = gsn_sword;
				break;
			case WEAPON_DAGGER:
				sn = gsn_dagger;
				break;
			case WEAPON_SPEAR:
				sn = gsn_spear;
				break;
			case WEAPON_MACE:
				sn = gsn_mace;
				break;
			case WEAPON_AXE:
				sn = gsn_axe;
				break;
			case WEAPON_FLAIL:
				sn = gsn_flail;
				break;
			case WEAPON_WHIP:
				sn = gsn_whip;
				break;
			case WEAPON_POLEARM:
				sn = gsn_polearm;
				break;
			case WEAPON_STAFF:
				sn = gsn_staff;
				break;
			default:
				sn = -1;
				break;
		}
	}

	return sn;
}

int get_weapon_skill(CHAR_DATA *ch, int sn)
{
	int skill;

	/* -1 is exotic */
	if (is_npc(ch))
	{
		if (sn == -1)
			skill = 3 * ch->level;
		else if (sn == gsn_hand_to_hand)
			skill = 40 + 2 * ch->level;
		else
			skill = 40 + 5 * ch->level / 2;
	}
	else
	{
		if (sn == -1)
			skill = 3 * ch->level;
		else
			skill = ch->pcdata->learned[sn];
	}

	return URANGE(0, skill, 100);
}

/* used to de-screw characters */
void reset_char(CHAR_DATA *ch)
{
	int loc, stat;
	OBJ_DATA *obj;
	AFFECT_DATA *af;
	OBJ_APPLY_DATA *app;
	int i;

	if (is_npc(ch))
		return;

	if (ch->pcdata->perm_hit == 0
		|| ch->pcdata->perm_mana == 0
		|| ch->pcdata->perm_move == 0
		|| ch->pcdata->last_level == 0)
	{
		/* do a FULL reset */
		for (loc = 0; loc < MAX_WEAR; loc++)
		{
			obj = get_eq_char(ch, loc);

			if (obj == nullptr)
				continue;

			for (af = obj->charaffs; af != nullptr; af = af->next)
			{
				affect_strip(ch, af->type);
			}

			for (app = obj->apply; app != nullptr; app = app->next)
			{
				modify_location(ch, app->location, app->modifier, false);
			}
		}

		/* now reset the permanent stats */
		ch->pcdata->perm_hit = ch->max_hit;
		ch->pcdata->perm_mana = ch->max_mana;
		ch->pcdata->perm_move = ch->max_move;
		ch->pcdata->last_level = ch->played / 3600;

		if (ch->pcdata->true_sex < 0 || ch->pcdata->true_sex > 2)
		{
			if (ch->sex > 0 && ch->sex < 3)
				ch->pcdata->true_sex = ch->sex;
			else
				ch->pcdata->true_sex = 0;
		}
	}

	/* now restore the character to his/her true condition */
	for (stat = 0; stat < MAX_STATS; stat++)
	{
		ch->mod_stat[stat] = 0;
	}

	if (ch->pcdata->true_sex < 0 || ch->pcdata->true_sex > 2)
		ch->pcdata->true_sex = 0;

	ch->sex = ch->pcdata->true_sex;
	ch->max_hit = ch->pcdata->perm_hit;
	ch->max_mana = ch->pcdata->perm_mana;
	ch->max_move = ch->pcdata->perm_move;

	for (i = 0; i < 4; i++)
	{
		ch->armor[i] = 0;
	}

	ch->hitroll = 0;
	ch->damroll = 0;
	ch->saving_throw = 0;

	/* now start adding back the effects */
	for (loc = 0; loc < MAX_WEAR; loc++)
	{
		obj = get_eq_char(ch, loc);

		if (obj == nullptr)
			continue;

		for (i = 0; i < 4; i++)
		{
			ch->armor[i] += apply_ac(obj, loc, i);
		}

		for (af = obj->charaffs; af != nullptr; af = af->next)
		{
			affect_modify(ch, af, true);
		}

		for (app = obj->apply; app != nullptr; app = app->next)
		{
			modify_location(ch, app->location, app->modifier, true);
		}
	}

	/* now add back spell effects */
	for (af = ch->affected; af != nullptr; af = af->next)
	{
		modify_location(ch, af->location, af->modifier, true);
	}

	/* make sure sex is RIGHT!!!! */
	if (ch->sex < 0 || ch->sex > 2)
		ch->sex = ch->pcdata->true_sex;
}

/*
 * Retrieve a character's trusted level for permission checking.
 */
int get_trust(CHAR_DATA *ch)
{
	if (ch->desc != nullptr && ch->desc->original != nullptr)
		ch = ch->desc->original;

	if (ch->trust)
		return ch->trust;

	if (is_npc(ch) && ch->level >= LEVEL_HERO)
		return LEVEL_HERO - 1;
	else
		return ch->level;
}

/*
 * Retrieve a character's age.
 */
/* command for retrieving stats */
int get_curr_stat(CHAR_DATA *ch, int stat)
{
	AFFECT_DATA *af;
	int max;
	int mod = 0;

	if (is_npc(ch) || ch->level > LEVEL_IMMORTAL)
		max = 25;

	else
	{
		max = pc_race_table[ch->race].max_stats[stat];

		if (!str_cmp(race_table[ch->race].name, "human"))
		{
			int iClass;
			iClass = ch->Class()->GetIndex();
			switch (ch->Class()->attr_prime)
			{
				case STAT_STR:
					if (stat == STAT_STR &&
						(ch->Class()->GetIndex() == CLASS_WARRIOR || ch->Class()->GetIndex() == CLASS_ANTI_PALADIN ||
						ch->Class()->GetIndex() == CLASS_RANGER || ch->Class()->GetIndex() == CLASS_PALADIN))
						max = 23;
					else
						max = 20;
					break;
				case STAT_INT:
					if (stat == STAT_INT &&
						(ch->Class()->GetIndex() == CLASS_SORCERER || ch->Class()->GetIndex() == CLASS_NECROMANCER ||
						ch->Class()->GetIndex() == CLASS_SHAPESHIFTER))
						max = 23;
					else
						max = 20;
					break;
				case STAT_WIS:
					if (stat == STAT_WIS &&
						(ch->Class()->GetIndex() == CLASS_HEALER || ch->Class()->GetIndex() == CLASS_ZEALOT))
						max = 23;
					else
						max = 20;
					break;
				case STAT_DEX:
					if (stat == STAT_DEX &&
						(ch->Class()->GetIndex() == CLASS_ASSASSIN || ch->Class()->GetIndex() == CLASS_THIEF))
						max = 23;
					else
						max = 20;
					break;
			}
		}

		max = std::min(max, 25);
	}

	if (ch->fighting && is_affected(ch->fighting, gsn_traitors_luck))
	{
		af = affect_find(ch->fighting->affected, gsn_traitors_luck);

		if (ch == af->owner)
			mod = 2;
	}

	return std::min(URANGE(3, ch->perm_stat[stat] + ch->mod_stat[stat], max) + mod, 25);
}

/* command for returning max training score */
int get_max_train(CHAR_DATA *ch, int stat)
{
	int max;
	int iClass;

	iClass = (ch->Class()->GetIndex() + 1);

	if (is_npc(ch) || ch->level > LEVEL_IMMORTAL)
		return 25;

	max = pc_race_table[ch->race].max_stats[stat];

	if (!str_cmp(race_table[ch->race].name, "human"))
	{
		switch (ch->Class()->attr_prime)
		{
			case STAT_STR:
				if (stat == STAT_STR && (ch->Class()->name == "warrior" || ch->Class()->name == "anti-paladin" ||
										ch->Class()->name == "ranger" || ch->Class()->name == "paladin"))
					max = 23;
				else
					max = 20;

				break;
			case STAT_INT:
				if (stat == STAT_INT && (ch->Class()->name == "sorcerer" || ch->Class()->name == "necromancer" ||
										ch->Class()->name == "shapeshifter"))
					max = 23;
				else
					max = 20;

				break;
			case STAT_WIS:
				if (stat == STAT_WIS && (ch->Class()->name == "healer" || ch->Class()->name == "zealot"))
					max = 23;
				else
					max = 20;

				break;
			case STAT_DEX:
				if (stat == STAT_DEX && (ch->Class()->name == "assassin" || ch->Class()->name == "thief"))
					max = 23;
				else
					max = 20;

				break;
		}
	}

	return std::min(max, 25);
}

/*
 * Retrieve a character's carry capacity.
 */
int can_carry_n(CHAR_DATA *ch)
{
	if (!is_npc(ch) && ch->level >= LEVEL_IMMORTAL)
		return 1000;

	if (is_npc(ch) && IS_SET(ch->act, ACT_PET))
		return 0;

	return MAX_WEAR + ch->level / 6 + dex_app[get_curr_stat(ch, STAT_DEX)].carry;
}

/*
 * Retrieve a character's carry capacity.
 */
int can_carry_w(CHAR_DATA *ch)
{
	if (!is_npc(ch) && ch->level >= LEVEL_IMMORTAL)
		return 10000000;

	if (is_npc(ch) && IS_SET(ch->act, ACT_PET))
		return 0;

	return MAX_WEAR + ch->level * 2 + str_app[get_curr_stat(ch, STAT_STR)].carry;
}

/* Calculate if players are in pk range to attack each other */
bool can_pk(CHAR_DATA *ch, CHAR_DATA *victim)
{
	int xp, vxp;

	if (ch == victim)
		return true;

	if (ch == nullptr || victim == nullptr)
		return false;

	int levelDif = victim->level - ch->level;

	if (levelDif <= -1)
		levelDif *= -1;

	if (is_npc(ch)
		|| is_npc(victim)
		|| ch->ghost > 0
		|| victim->ghost > 0
		|| ch->level < MIN_LEVEL_TO_PK
		|| victim->level < MIN_LEVEL_TO_PK
		|| levelDif > 9) // abs(victim->level - ch->level)
	{
		return false;
	}

	if (!is_npc(ch)
		&& !is_npc(victim)
		&& ch->level == LEVEL_HERO
		&& victim->level == LEVEL_HERO)
	{
		return true;
	}

	xp = ch->exp;
	vxp = victim->exp;

	if (xp > MIN_PK_XP && vxp > MIN_PK_XP)
	{
		if (ch->level - victim->level < MIN_LEVEL_TO_PK && ch->level - victim->level > -MIN_LEVEL_TO_PK)
		{
			if (((xp * .7) < vxp) && ((xp * 1.3) > vxp))
				return true;
			else if (((vxp * .7) < xp) && ((vxp * 1.3) > xp))
				return true;
			else
				return false;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

/*
 * See if a string is one of the names of an object.
 */

bool is_name(const char *str, const char *namelist)
{
	char name[MAX_INPUT_LENGTH], part[MAX_INPUT_LENGTH];
	char *list, *string;

	/* fix crash on nullptr namelist */
	if (namelist == nullptr || namelist[0] == '\0')
		return false;

	/* fixed to prevent is_name on "" returning true */
	if (str == nullptr || str[0] == '\0')
		return false;

	string = (char *)str;
	/* we need ALL parts of string to match part of namelist */
	for (;;) /* start parsing string */
	{
		str = one_argument((char *)str, part);

		if (part[0] == '\0')
			return true;

		/* check to see if this is part of namelist */
		list = (char *)namelist;
		for (;;) /* start parsing namelist */
		{
			list = one_argument(list, name);

			if (name[0] == '\0') /* this name was not found */
				return false;

			if (!str_cmp(string, name))
				return true; /* full pattern match */

			if (!str_cmp(part, name))
				break;
		}
	}
}

bool is_exact_name(char *str, char *namelist)
{
	char name[MAX_INPUT_LENGTH];

	if (namelist == nullptr)
		return false;

	for (;;)
	{
		namelist = one_argument(namelist, name);

		if (name[0] == '\0')
			return false;
		if (!str_cmp(str, name))
			return true;
	}
}

void init_affect(AFFECT_DATA *paf)
{
	paf->where = TO_AFFECTS;
	paf->name = nullptr;
	paf->type = 0;
	paf->aftype = AFT_SPELL;
	paf->level = 0;
	paf->init_duration = 0;
	paf->duration = 0;
	paf->location = 0;
	paf->modifier = 0;
	paf->mod_name = MOD_NONE;

	zero_vector(paf->bitvector);
	paf->tick_fun = nullptr;
	paf->pulse_fun = nullptr;
	paf->end_fun = nullptr;
	paf->owner = nullptr;
	paf->beat_fun = nullptr;
}

/*
 * Apply or remove an affect to a character.
 */
void affect_modify(CHAR_DATA *ch, AFFECT_DATA *paf, bool fAdd)
{
	OBJ_DATA *wield;
	int mod;
	bool disarmed= false;

	mod = paf->modifier;

	if (fAdd)
	{
		switch (paf->where)
		{
			case TO_AFFECTS:
				BITWISE_OR(ch->affected_by, paf->bitvector);
				break;
			case TO_IMMUNE:
				BITWISE_OR(ch->imm_flags, paf->bitvector);
				break;
			case TO_RESIST:
				BITWISE_OR(ch->res_flags, paf->bitvector);
				break;
			case TO_VULN:
				BITWISE_OR(ch->vuln_flags, paf->bitvector);
				break;
		}
	}
	else
	{
		switch (paf->where)
		{
			case TO_AFFECTS:
				BITWISE_XAND(ch->affected_by, paf->bitvector);
				break;
			case TO_IMMUNE:
				BITWISE_XAND(ch->imm_flags, paf->bitvector);
				break;
			case TO_RESIST:
				BITWISE_XAND(ch->res_flags, paf->bitvector);
				break;
			case TO_VULN:
				BITWISE_XAND(ch->vuln_flags, paf->bitvector);
				break;
		}
	}

	modify_location(ch, paf->location, paf->modifier, fAdd);

	/*
	 * Check for weapon wielding.
	 * Guard against recursion (for weapons with affects).
	 */
	if (!is_npc(ch)
		&& (wield = get_eq_char(ch, WEAR_WIELD)) != nullptr
		&& get_obj_weight(wield) > str_app[get_curr_stat(ch, STAT_STR)].wield
		&& !is_obj_stat(wield, ITEM_NODISARM)
		&& !is_immortal(ch))
	{
		static int depth;

		if (depth == 0)
		{
			depth++;
			act("You can no longer find the strength to wield $p.", ch, wield, nullptr, TO_CHAR);
			act("$n can no longer find the strength to wield $p.", ch, wield, nullptr, TO_ROOM);
			disarmed = true;
			obj_from_char(wield);
			obj_to_char(wield, ch);
			depth--;
		}
	}

	if (!is_npc(ch)
		&& (wield = get_eq_char(ch, WEAR_DUAL_WIELD)) != nullptr
		&& get_obj_weight(wield) > str_app[get_curr_stat(ch, STAT_STR)].wield
		&& !is_obj_stat(wield, ITEM_NODISARM))
	{
		static int depth;

		if (depth == 0)
		{
			depth++;
			act("You can no longer find the strength to wield $p.", ch, wield, nullptr, TO_CHAR);
			act("$n can no longer find the strength to wield $p.", ch, wield, nullptr, TO_ROOM);
			disarmed = true;
			obj_from_char(wield);
			obj_to_char(wield, ch);
			depth--;
		}
	}

	if (!is_npc(ch))
	{
		/* Arms */
		if ((wield = get_eq_char(ch, WEAR_WIELD)) != nullptr && ch->arms < 1)
		{
			static int depth;

			if (depth == 0)
			{
				depth++;
				act("You are no longer able to grasp $p.", ch, wield, nullptr, TO_CHAR);
				act("$n is no longer able to grasp $p.", ch, wield, nullptr, TO_ROOM);
				disarmed = true;
				obj_from_char(wield);
				obj_to_char(wield, ch);
				depth--;
			}
		}

		if ((wield = get_eq_char(ch, WEAR_DUAL_WIELD)) != nullptr && ch->arms < 2)
		{
			static int depth;

			if (depth == 0)
			{
				depth++;
				act("You are no longer able to grasp $p.", ch, wield, nullptr, TO_CHAR);
				act("$n is no longer able to grasp $p.", ch, wield, nullptr, TO_ROOM);
				disarmed = true;
				obj_from_char(wield);
				obj_to_char(wield, ch);
				depth--;
			}
		}

		if ((wield = get_eq_char(ch, WEAR_WIELD)) != nullptr
			&& ch->arms < 2
			&& ch->size < SIZE_LARGE
			&& (is_weapon_stat(wield, WEAPON_TWO_HANDS)
				|| wield->value[0] == WEAPON_STAFF
				|| wield->value[0] == WEAPON_POLEARM
				|| wield->value[0] == WEAPON_SPEAR))
		{
			static int depth;

			if (depth == 0)
			{
				depth++;
				act("You are no longer able to grasp $p.", ch, wield, nullptr, TO_CHAR);
				act("$n is no longer able to grasp $p.", ch, wield, nullptr, TO_ROOM);
				disarmed = true;
				obj_from_char(wield);
				obj_to_char(wield, ch);
				depth--;
			}
		}
	}

	if ((wield = get_eq_char(ch, WEAR_DUAL_WIELD)) != nullptr)
	{
		if (disarmed && get_eq_char(ch, WEAR_WIELD) == nullptr)
		{
			unequip_char(ch, wield, false);
			act("You swap $p into your primary hand.", ch, wield, 0, TO_CHAR);
			act("$n swaps $p into $s primary hand.", ch, wield, 0, TO_ROOM);
			equip_char(ch, wield, WEAR_WIELD, false);
		}
	}
}

/* find an effect in an affect list */
AFFECT_DATA *affect_find(AFFECT_DATA *paf, int sn)
{
	AFFECT_DATA *paf_find;

	for (paf_find = paf; paf_find != nullptr; paf_find = paf_find->next)
	{
		if (paf_find->type == sn)
			return paf_find;
	}

	return nullptr;
}

OBJ_AFFECT_DATA *affect_find_obj(OBJ_AFFECT_DATA *paf, int sn)
{
	OBJ_AFFECT_DATA *paf_find;

	for (paf_find = paf; paf_find != nullptr; paf_find = paf_find->next)
	{
		if (paf_find->type == sn)
			return paf_find;
	}

	return nullptr;
}

ROOM_AFFECT_DATA *affect_find_room(ROOM_AFFECT_DATA *paf, int sn)
{
	ROOM_AFFECT_DATA *paf_find;

	for (paf_find = paf; paf_find != nullptr; paf_find = paf_find->next)
	{
		if (paf_find->type == sn)
			return paf_find;
	}

	return nullptr;
}

AREA_AFFECT_DATA *affect_find_area(AREA_AFFECT_DATA *paf, int sn)
{
	AREA_AFFECT_DATA *paf_find;

	for (paf_find = paf; paf_find != nullptr; paf_find = paf_find->next)
	{
		if (paf_find->type == sn)
			return paf_find;
	}

	return nullptr;
}

/* fix object affects when removing one */
void affect_check(CHAR_DATA *ch, int where, long vector[])
{
	AFFECT_DATA *paf;

	if (where == TO_OBJECT || where == TO_WEAPON || vector == 0)
		return;

	for (paf = ch->affected; paf != nullptr; paf = paf->next)
	{
		if (paf->where == where && vector_equal(paf->bitvector, vector))
		{
			switch (where)
			{
				case TO_AFFECTS:
					BITWISE_OR(ch->affected_by, vector);
					break;
				case TO_IMMUNE:
					BITWISE_OR(ch->imm_flags, vector);
					break;
				case TO_RESIST:
					BITWISE_OR(ch->res_flags, vector);
					break;
				case TO_VULN:
					BITWISE_OR(ch->vuln_flags, vector);
					break;
			}

			return;
		}
	}
}

/*
 * Give an affect to a char.
 */
void affect_to_char(CHAR_DATA *ch, AFFECT_DATA *paf)
{
	new_affect_to_char(ch, paf);
}

void new_affect_to_char(CHAR_DATA *ch, AFFECT_DATA *paf)
{
	AFFECT_DATA *paf_new;

	if (IS_SET(ch->imm_flags, IMM_SLEEP) && IS_SET(paf->bitvector, AFF_SLEEP) && paf->where == TO_AFFECTS)
	{
		send_to_char("You are unaffected.\n\r", ch);
		act("$n is unaffected.", ch, 0, 0, TO_ROOM);
		return;
	}

	if (is_affected_by(ch, AFF_SLEEP) && IS_SET(paf->bitvector, AFF_SLEEP) && ch->position == POS_SLEEPING)
		return;

	if (is_affected(ch, gsn_indom) && paf->aftype != AFT_TIMER)
	{
		send_to_char("Your body's state is maintained by the force of your will.\n\r", ch);
		return;
	}

	// set initial duration
	paf->init_duration = paf->duration;

	paf_new = new_affect();
	*paf_new = *paf;

	paf_new->next = ch->affected;
	ch->affected = paf_new;

	/* Morg - Valgrind fix */
	paf_new->valid = true;

	affect_modify(ch, paf_new, true);
}

/*
 * Remove an affect from a char.
 */
void affect_remove(CHAR_DATA *ch, AFFECT_DATA *paf)
{
	int where;
	long vector[MAX_BITVECTOR];

	if (ch->affected == nullptr)
	{
		bug("Affect_remove: no affect on %s.", ch->name);
		return;
	}

	if (paf->end_fun != nullptr)
		(*paf->end_fun)(ch, paf);

	affect_modify(ch, paf, false);
	where = paf->where;
	copy_vector(vector, paf->bitvector);

	if (paf == ch->affected)
	{
		ch->affected = paf->next;
	}
	else
	{
		AFFECT_DATA *prev;

		for (prev = ch->affected; prev != nullptr; prev = prev->next)
		{
			if (prev->next == paf)
			{
				prev->next = paf->next;
				break;
			}
		}

		if (prev == nullptr)
		{
			bug("Affect_remove: cannot find paf.", 0);
			return;
		}
	}

	free_affect(paf);
	affect_check(ch, where, vector);
}

/*
 * Strip all affects of a given sn.
 */
void affect_strip(CHAR_DATA *ch, int sn)
{
	AFFECT_DATA *paf;
	AFFECT_DATA *paf_next;

	for (paf = ch->affected; paf != nullptr; paf = paf_next)
	{
		paf_next = paf->next;

		if (paf->type == sn)
		{
			affect_remove(ch, paf);
		}
	}
}

/*
 * Return true if a char is affected by a spell.
 */
bool is_affected(CHAR_DATA *ch, int sn)
{
	AFFECT_DATA *paf;

	if (!ch)
		return false;

	for (paf = ch->affected; paf != nullptr; paf = paf->next)
	{
		if (paf->type == sn)
			return true;
	}

	return false;
}
/*
bool is_affected_obj( OBJ_DATA *obj, int sn )
{
	AFFECT_DATA *paf;

	for ( paf = obj->affected; paf != nullptr; paf = paf->next )
	{
		if ( paf->type == sn )
			return true;
	}

	return false;
}
*/

/*
 * Add or enhance an affect.
 */

void affect_join(CHAR_DATA *ch, AFFECT_DATA *paf)
{
	paf->owner = nullptr;
	paf->end_fun = nullptr;
	new_affect_join(ch, paf);
}

void new_affect_join(CHAR_DATA *ch, AFFECT_DATA *paf)
{
	AFFECT_DATA *paf_old;

	for (paf_old = ch->affected; paf_old != nullptr; paf_old = paf_old->next)
	{
		if (paf_old->type == paf->type)
		{
			paf->level = (paf->level + paf_old->level) / 2;
			paf->duration += paf_old->duration;
			paf->modifier += paf_old->modifier;

			affect_remove(ch, paf_old);
			break;
		}
	}

	new_affect_to_char(ch, paf);
}

/*
 * Move a char out of a room.
 */
void char_from_room(CHAR_DATA *ch)
{
	ROOM_INDEX_DATA *prev_room = ch->in_room;
	ROOM_AFFECT_DATA *af;
	OBJ_DATA *obj;
	CHAR_DATA *mob;
	bool other= false;
	AFFECT_DATA *aaf;

	if (ch->in_room == nullptr)
	{
		bug("Char_from_room: nullptr.", 0);
		return;
	}

	if (!is_npc(ch))
		--ch->in_room->area->nplayer;

	for (obj = ch->carrying; obj; obj = obj->next_content)
	{
		if (obj->wear_loc != WEAR_NONE)
		{
			if ((obj->item_type == ITEM_LIGHT || is_obj_stat(obj, ITEM_GLOW)) && ch->in_room)
				ch->in_room->light = std::max(0, ch->in_room->light - 3);
		}
	}

	if (IS_SET(ch->progtypes, MPROG_MOVE))
	{
		for (mob = ch->in_room->people; mob; mob = mob->next_in_room)
		{
			if (mob != ch && IS_SET(mob->progtypes, MPROG_MOVE))
				other = true;
		}

		if (!other)
			ch->in_room->move_progs = false;
	}

	if (ch == ch->in_room->people)
	{
		ch->in_room->people = ch->next_in_room;
	}
	else
	{
		CHAR_DATA *prev;

		for (prev = ch->in_room->people; prev; prev = prev->next_in_room)
		{
			if (prev->next_in_room == ch)
			{
				prev->next_in_room = ch->next_in_room;
				break;
			}
		}
		/*
		if ( prev == nullptr )
			bug( "Char_from_room: ch not found.", 0 );
		*/
	}

	ch->in_room = nullptr;
	ch->next_in_room = nullptr;
	ch->on = nullptr; /* sanity check! */

	if (is_affected_room(prev_room, gsn_gravity_well))
	{
		for (af = prev_room->affected; af != nullptr; af = af->next)
		{
			if (af->type == gsn_gravity_well)
				break;
		}

		if (ch == af->owner)
			gravity_well_explode(prev_room, af);
	}
	if (!is_affected(ch, gsn_pull) && (check_entwine(ch, 1) || check_entwine(ch, 2)))
	{
		for (aaf = ch->affected; aaf != nullptr; aaf = aaf->next)
		{
			if (aaf->type == gsn_entwine && (aaf->modifier == 1 || aaf->location == APPLY_DEX))
				break;
		}

		do_uncoil(aaf->owner, "automagic");
	}
	else if (!is_affected(ch, gsn_pull) && check_entwine(ch, 0))
	{
		do_uncoil(ch, "automagic");
	}
}

/*
 * Move a char into a room.
 */
void char_to_room(CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex)
{
	OBJ_DATA *obj;

	if (pRoomIndex == nullptr)
	{
		ROOM_INDEX_DATA *room;

		bug("Char_to_room: nullptr.", 0);

		if ((room = get_room_index(ROOM_VNUM_TEMPLE)) != nullptr)
			char_to_room(ch, room);

		return;
	}

	ch->in_room = pRoomIndex;
	ch->next_in_room = pRoomIndex->people;
	pRoomIndex->people = ch;

	if (!is_npc(ch))
	{
		/* this is crashing us */
		if (ch->in_room->area->empty)
			ch->in_room->area->empty= false;

		++ch->in_room->area->nplayer;
	}
	else
	{
		if (ch->home_room == nullptr)
			ch->home_room = pRoomIndex;
	}

	for (obj = ch->carrying; obj; obj = obj->next_content)
	{
		if (obj->wear_loc != WEAR_NONE)
		{
			if ((obj->item_type == ITEM_LIGHT || is_obj_stat(obj, ITEM_GLOW)) && ch->in_room)
				ch->in_room->light += 3;
		}
	}

	if (IS_SET(ch->progtypes, MPROG_MOVE))
		ch->in_room->move_progs = true;

	if (is_affected_by(ch, AFF_PLAGUE))
	{
		AFFECT_DATA *af;

		for (af = ch->affected; af; af = af->next)
		{
			if (af->tick_fun == plague_tick)
			{
				break;
			}
		}

		if (!af)
		{
			REMOVE_BIT(ch->affected_by, AFF_PLAGUE);
			return;
		}
	}

	if ((!IS_ZERO_VECTOR(ch->in_room->affected_by) || ch->in_room->has_rune) && is_immortal(ch))
		do_raffects(ch, "");
}

///
/// Give an item (object) to a specified character and iterate carried count.
///
/// @param obj: Any item in the game
/// @param ch: The character to receive the item
///
void obj_to_char(OBJ_DATA *obj, CHAR_DATA *ch)
{
	obj->next_content = ch->carrying;
	ch->carrying = obj;
	obj->carried_by = ch;
	obj->in_room = nullptr;
	obj->in_obj = nullptr;
	ch->carry_number += get_obj_number(obj);
	ch->carry_weight += get_obj_weight(obj);
}

/*
 * Take an obj from its character.
 */
void obj_from_char(OBJ_DATA *obj)
{
	CHAR_DATA *ch;

	if ((ch = obj->carried_by) == nullptr)
	{
		bug("Obj_from_char: null ch.", 0);
		return;
	}

	if (obj->wear_loc != WEAR_NONE)
		unequip_char(ch, obj, true);

	if (ch->carrying == obj)
	{
		ch->carrying = obj->next_content;
	}
	else
	{
		OBJ_DATA *prev;

		for (prev = ch->carrying; prev != nullptr; prev = prev->next_content)
		{
			if (prev->next_content == obj)
			{
				prev->next_content = obj->next_content;
				break;
			}
		}

		if (prev == nullptr)
			bug("Obj_from_char: obj not in list.", 0);
	}

	obj->carried_by = nullptr;
	obj->next_content = nullptr;
	ch->carry_number -= get_obj_number(obj);
	ch->carry_weight -= get_obj_weight(obj);
}

/*
 * Find the ac value of an obj, including position effect.
 */
int apply_ac(OBJ_DATA *obj, int iWear, int type)
{
	if (obj->item_type != ITEM_ARMOR)
		return 0;

	switch (iWear)
	{
		case WEAR_STRAPPED:
			return obj->value[type];
		case WEAR_BODY:
			return 3 * obj->value[type];
		case WEAR_HEAD:
			return obj->value[type];
		case WEAR_LEGS:
			return obj->value[type];
		case WEAR_FEET:
			return obj->value[type];
		case WEAR_HANDS:
			return obj->value[type];
		case WEAR_ARMS:
			return obj->value[type];
		case WEAR_SHIELD:
			return obj->value[type];
		case WEAR_NECK_1:
			return obj->value[type];
		case WEAR_NECK_2:
			return obj->value[type];
		case WEAR_ABOUT:
			return 2 * obj->value[type];
		case WEAR_WAIST:
			return obj->value[type];
		case WEAR_WRIST_L:
			return obj->value[type];
		case WEAR_WRIST_R:
			return obj->value[type];
		case WEAR_HOLD:
			return obj->value[type];
		case WEAR_FINGER_L:
			return obj->value[type];
		case WEAR_FINGER_R:
			return obj->value[type];
	}

	return 0;
}

/*
 * Find a piece of eq on a character.
 */
OBJ_DATA *get_eq_char(CHAR_DATA *ch, int iWear)
{
	OBJ_DATA *obj;

	if (ch == nullptr)
		return nullptr;

	for (obj = ch->carrying; obj != nullptr; obj = obj->next_content)
	{
		if (obj->wear_loc == iWear)
			return obj;
	}

	return nullptr;
}

bool is_worn(OBJ_DATA *obj)
{
	if (obj->wear_loc > WEAR_NONE)
		return true;

	return false;
}

/*
 * Equip a char with an obj.
 */
void equip_char(CHAR_DATA *ch, OBJ_DATA *obj, int iWear, bool show)
{
	char buf[MSL];
	AFFECT_DATA *paf, *fmaf;
	OBJ_APPLY_DATA *app;
	int i;
	bool status;

	status= false;
	if (iWear != WEAR_COSMETIC && get_eq_char(ch, iWear) != nullptr)
	{
		sprintf(buf, "Equip_char: already equipped (%d) -- %s -- %s.", iWear, ch->name, ch->in_room->area->file_name);
		bug(buf, 0);
		return;
	}

	int palign = ch->alignment, pethos = get_ethos(ch);

	if (!is_npc(ch) && is_affected(ch, gsn_false_motives))
	{
		AFFECT_DATA *pal;
		for (pal = ch->affected; pal; pal = pal->next)
		{
			if (pal->type == gsn_false_motives && pal->location == APPLY_ALIGNMENT)
				palign -= pal->modifier;
		}

		for (pal = ch->affected; pal; pal = pal->next)
		{
			if (pal->type == gsn_false_motives && pal->location == APPLY_ETHOS)
				pethos -= pal->modifier;
		}
	}

	if (!is_npc(ch)
		&& (is_obj_stat(obj, ITEM_ANTI_EVIL) && palign < 0)
		|| (is_obj_stat(obj, ITEM_ANTI_GOOD) && palign > 0)
		|| (is_obj_stat(obj, ITEM_ANTI_NEUTRAL) && palign == 0)
		|| (is_obj_stat(obj, ITEM_ANTI_LAWFUL) && pethos > 0)
		|| (is_obj_stat(obj, ITEM_ANTI_NEUT) && pethos == 0)
		|| (is_obj_stat(obj, ITEM_ANTI_CHAOTIC) && pethos < 0)
		|| is_restricted(ch, obj))
	{
		/*
		 * Thanks to Morgenes for the bug fix here!
		 */
		act("$p rejects you, sliding from your hands despite your best efforts.", ch, obj, nullptr, TO_CHAR);
		act("$n drops $p onto the ground.", ch, obj, nullptr, TO_ROOM);
		obj_from_char(obj);
		obj_to_room(obj, ch->in_room);
		return;
	}

	obj->wear_loc = iWear;

	if (show && IS_SET(obj->progtypes, IPROG_WEAR))
		(obj->pIndexData->iprogs->wear_prog)(obj, ch);

	if (show && TRAPS_IEVENT(obj, TRAP_IWEAR))
		CALL_IEVENT(obj, TRAP_IWEAR, ch, obj);

	if (iWear == WEAR_COSMETIC)
		return;

	BITWISE_OR(ch->imm_flags, obj->imm_flags);
	BITWISE_OR(ch->res_flags, obj->res_flags);
	BITWISE_OR(ch->vuln_flags, obj->vuln_flags);

	for (i = 0; i < 4; i++)
	{
		ch->armor[i] += apply_ac(obj, iWear, i);
	}

	for (app = obj->apply; app; app = app->next)
	{
		modify_location(ch, app->location, app->modifier, true);
	}

	for (paf = obj->charaffs; paf != nullptr; paf = paf->next)
	{
		affect_to_char(ch, paf);
	}

	if ((obj->item_type == ITEM_LIGHT || is_obj_stat(obj, ITEM_GLOW)) && ch->in_room)
		ch->in_room->light += 3;

	if (show && obj->pIndexData->wear_echo[0] != nullptr)
		act(palloc_string(obj->pIndexData->wear_echo[0]), ch, obj, 0, TO_CHAR);

	if (show && obj->pIndexData->wear_echo[1] != nullptr)
		act(palloc_string(obj->pIndexData->wear_echo[1]), ch, obj, 0, TO_ROOM);
}

/*
 * Unequip a char with an obj.
 */
void unequip_char(CHAR_DATA *ch, OBJ_DATA *obj, bool show)
{
	AFFECT_DATA *paf = nullptr;
	OBJ_APPLY_DATA *app;
	int i, wearloc = obj->wear_loc;

	if (obj->wear_loc == WEAR_NONE)
	{
		bug("Unequip_char: already unequipped.", 0);
		return;
	}

	if (obj->wear_loc == WEAR_WIELD && check_entwine(obj->carried_by, 0))
	{
		do_uncoil(obj->carried_by, "automagic");
	}

	obj->wear_loc = -1;

	if (show && IS_SET(obj->progtypes, IPROG_REMOVE))
		(obj->pIndexData->iprogs->remove_prog)(obj, ch);

	if (show && TRAPS_IEVENT(obj, TRAP_IREMOVE))
		CALL_IEVENT(obj, TRAP_IREMOVE, ch, obj);

	if (obj->wear_loc == WEAR_COSMETIC)
		return;

	for (i = 0; i < 4; i++)
	{
		ch->armor[i] -= apply_ac(obj, wearloc, i);
	}

	BITWISE_XOR(ch->imm_flags, obj->imm_flags);
	BITWISE_XOR(ch->res_flags, obj->res_flags);
	BITWISE_XOR(ch->vuln_flags, obj->vuln_flags);

	for (app = obj->apply; app; app = app->next)
	{
		modify_location(ch, app->location, app->modifier, false);
	}

	for (paf = obj->charaffs; paf != nullptr; paf = paf->next)
	{
		affect_strip(ch, paf->type);
	}

	if ((obj->item_type == ITEM_LIGHT || is_obj_stat(obj, ITEM_GLOW)) && ch->in_room)
		ch->in_room->light = std::max(0, ch->in_room->light - 3);

	if (show && obj->pIndexData->remove_echo[0] != nullptr)
		act(palloc_string(obj->pIndexData->remove_echo[0]), ch, obj, 0, TO_CHAR);

	if (show && obj->pIndexData->remove_echo[1] != nullptr)
		act(palloc_string(obj->pIndexData->remove_echo[1]), ch, obj, 0, TO_ROOM);
}

/*
 * Count occurrences of an obj in a list.
 */
int count_obj_list(OBJ_INDEX_DATA *pObjIndex, OBJ_DATA *list)
{
	OBJ_DATA *obj;
	int nMatch;

	nMatch = 0;

	for (obj = list; obj != nullptr; obj = obj->next_content)
	{
		if (obj->pIndexData == pObjIndex)
			nMatch++;
	}

	return nMatch;
}

/*
 * Move an obj out of a room.
 */
void obj_from_room(OBJ_DATA *obj)
{
	ROOM_INDEX_DATA *in_room;
	CHAR_DATA *ch;

	if ((in_room = obj->in_room) == nullptr)
	{
		bug("obj_from_room: nullptr.", 0);
		return;
	}

	if (obj->item_type == ITEM_CAMPFIRE)
		in_room->light = std::max(in_room->light - obj->value[0], 0);

	for (ch = in_room->people; ch != nullptr; ch = ch->next_in_room)
	{
		if (ch->on == obj)
			ch->on = nullptr;
	}

	if (obj == in_room->contents)
	{
		in_room->contents = obj->next_content;
	}
	else
	{
		OBJ_DATA *prev;

		for (prev = in_room->contents; prev; prev = prev->next_content)
		{
			if (prev->next_content == obj)
			{
				prev->next_content = obj->next_content;
				break;
			}
		}

		if (prev == nullptr)
		{
			bug("Obj_from_room: obj not found.", 0);
			return;
		}
	}

	obj->in_room = nullptr;
	obj->next_content = nullptr;
}

/*
 * Move an obj into a room.
 */
void obj_to_room(OBJ_DATA *obj, ROOM_INDEX_DATA *pRoomIndex)
{
	if (pRoomIndex == nullptr || obj == nullptr)
		return;

	if (obj->item_type == ITEM_CAMPFIRE)
		pRoomIndex->light += obj->value[0];

	obj->next_content = pRoomIndex->contents;
	pRoomIndex->contents = obj;
	obj->in_room = pRoomIndex;
	obj->carried_by = nullptr;
	obj->in_obj = nullptr;
}

/*
 * Move an object into an object.
 */
void obj_to_obj(OBJ_DATA *obj, OBJ_DATA *obj_to)
{
	if (obj == nullptr || obj_to == nullptr || obj == obj_to)
		return;

	obj->next_content = obj_to->contains;
	obj_to->contains = obj;
	obj->in_obj = obj_to;
	obj->in_room = nullptr;
	obj->carried_by = nullptr;

	if (obj_to->pIndexData->vnum == OBJ_VNUM_PIT)
		obj->cost = 0;

	for (; obj_to != nullptr; obj_to = obj_to->in_obj)
	{
		if (obj_to->carried_by != nullptr)
		{
			obj_to->carried_by->carry_weight += get_obj_weight(obj);
		}
	}
}

/*
 * Move an object out of an object.
 */
void obj_from_obj(OBJ_DATA *obj)
{
	OBJ_DATA *obj_from;

	if ((obj_from = obj->in_obj) == nullptr)
	{
		bug("Obj_from_obj: null obj_from.", 0);
		return;
	}

	if (obj == obj_from->contains)
	{
		obj_from->contains = obj->next_content;
	}
	else
	{
		OBJ_DATA *prev;

		for (prev = obj_from->contains; prev; prev = prev->next_content)
		{
			if (prev->next_content == obj)
			{
				prev->next_content = obj->next_content;
				break;
			}
		}

		if (prev == nullptr)
		{
			bug("Obj_from_obj: obj not found.", 0);
			return;
		}
	}

	obj->next_content = nullptr;
	obj->in_obj = nullptr;

	for (; obj_from != nullptr; obj_from = obj_from->in_obj)
	{
		if (obj_from->carried_by != nullptr)
		{
			obj_from->carried_by->carry_weight -= get_obj_weight(obj);
		}
	}
}

/*
 * Extract an obj from the world.
 */
void extract_obj(OBJ_DATA *obj)
{
	OBJ_DATA *obj_content;
	OBJ_DATA *obj_next;

	if (obj->in_room != nullptr)
		obj_from_room(obj);
	else if (obj->carried_by != nullptr)
		obj_from_char(obj);
	else if (obj->in_obj != nullptr)
		obj_from_obj(obj);

	obj->pIndexData->limcount -= 1;

	for (obj_content = obj->contains; obj_content; obj_content = obj_next)
	{
		obj_next = obj_content->next_content;
		extract_obj(obj_content);
	}

	if (object_list == obj)
	{
		object_list = obj->next;
	}
	else
	{
		OBJ_DATA *prev;

		for (prev = object_list; prev != nullptr; prev = prev->next)
		{
			if (prev->next == obj)
			{
				prev->next = obj->next;
				break;
			}
		}

		if (prev == nullptr)
		{
			bug("Extract_obj: obj %d not found.", obj->pIndexData->vnum);
			return;
		}
	}

	free_obj(obj);
}

void delay_extract(CHAR_DATA *ch)
{
	extract_char(ch, true);
}

/*
 * Extract a char from the world.
 */
void extract_char(CHAR_DATA *ch, bool fPull)
{
	CHAR_DATA *wch;
	OBJ_DATA *obj;
	OBJ_DATA *obj_next;
	CHAR_DATA *tch;
	ROOM_INDEX_DATA *room;
	ROOM_AFFECT_DATA *raf;
	AFFECT_DATA *af;

	if (ch->in_room == nullptr)
	{
		char buf[MSL], vn[50];

		if (is_npc(ch))
			sprintf(vn, "%d", ch->pIndexData->vnum);

		sprintf(buf, "Extract_char: in_room is nullptr.  %s%s.",
			is_npc(ch) ? "Vnum is " : "Name is ",
			is_npc(ch) ? vn : ch->name);
		bug(buf, 0);
	}
	/* remove all tracking */
	for (tch = char_list; tch != nullptr; tch = tch->next)
	{
		if (tch->last_fought == ch)
			tch->last_fought = nullptr;

		if (!is_npc(ch) && !is_npc(tch) && tch->last_fight_name == ch->true_name)
			tch->last_fight_name = nullptr;

		if (!is_npc(tch) && !is_npc(ch) && tch->pcdata->trusting == ch)
			tch->pcdata->trusting = nullptr;

		for (af = tch->affected; af; af = af->next)
		{
			if (af->owner == ch)
				af->owner = nullptr;
		}
	}

	nuke_pets(ch);
	ch->pet = nullptr; /* just in case */

	if (fPull)
		die_follower(ch);

	stop_fighting(ch, true);

	for (obj = ch->carrying; obj != nullptr; obj = obj_next)
	{
		obj_next = obj->next_content;

		if (!(obj->wear_loc == WEAR_BRAND || IS_SET(obj->extra_flags, ITEM_FIXED)) && !is_npc(ch))
		{
			if (!is_npc(ch) && (fPull))
				obj->pIndexData->limcount++;

			extract_obj(obj);
		}
	}

	char_from_room(ch);

	if (is_npc(ch) && CQueue::HasQueuePending(ch))
	{
		bug("Attempt at extracting mob %d while it has queue events pending.  Deleting events.", ch->pIndexData->vnum);
		CQueue::DeleteQueuedEventsInvolving(ch);
	}

	/* Death room is set in the cabal table now */
	if (!fPull && !is_npc(ch))
		return char_to_room(ch, get_room_index(ROOM_VNUM_ALTAR));

	if (is_npc(ch))
	{
		--ch->pIndexData->count;
		total_wealth -= ch->pIndexData->wealth;
	}

	if (ch->desc != nullptr && ch->desc->original != nullptr)
	{
		do_return(ch, "");
		ch->desc = nullptr;
	}

	for (wch = char_list; wch != nullptr; wch = wch->next)
	{
		if (wch->reply == ch)
			wch->reply = nullptr;
	}

	for (room = top_affected_room; room; room = room->aff_next)
	{
		for (raf = room->affected; raf != nullptr; raf = raf->next)
		{
			if (raf->owner == ch)
			{
				affect_remove_room(room, raf);
			}
		}
	}

	if (ch == char_list)
	{
		char_list = ch->next;
	}
	else
	{
		CHAR_DATA *prev;
		for (prev = char_list; prev != nullptr; prev = prev->next)
		{
			if (prev->next == ch)
			{
				prev->next = ch->next;
				break;
			}
		}

		if (prev == nullptr)
		{
			bug("Extract_char: char not found.", 0);
			return;
		}
	}

	if (ch->desc != nullptr)
		ch->desc->character = nullptr;

	free_char(ch);
}

/*
 * Find a char in the room.
 */
/* SEE act_info.c for new one that handles morph
CHAR_DATA *get_char_room( CHAR_DATA *ch, char *argument )
{
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *rch;
	int number;
	int count;

	number = number_argument( argument, arg );
	count  = 0;
	if ( !str_cmp( arg, "self" ) )
	return ch;
	for ( rch = ch->in_room->people; rch != nullptr; rch = rch->next_in_room )
	{
		if ( !can_see( ch, rch ) || (arg[0] != '\0' && !is_name( arg, rch->name )))
			continue;

		if ( ++count == number )
			return rch;
	}

	return nullptr;
}
*/

/*
 * Find a char in the world.
 */
CHAR_DATA *get_char_world(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *wch;
	int number;
	int count;
	char name[MAX_STRING_LENGTH];

	if ((wch = get_char_room(ch, argument)) != nullptr)
		return wch;

	number = number_argument(argument, arg);
	count = 0;

	for (wch = char_list; wch != nullptr; wch = wch->next)
	{
		if (is_immortal(ch) && !is_npc(wch))
			sprintf(name, "%s", wch->true_name);
		else
			sprintf(name, "%s", wch->name);

		if (wch->in_room == nullptr || !can_see(ch, wch) || !is_name(arg, name))
			continue;

		if (is_npc(wch) && wch->pIndexData->vnum == MOB_VNUM_DECOY && is_immortal(ch))
			continue;

		if (++count == number)
			return wch;
	}

	return nullptr;
}

/*
 * Find some object with a given index data.
 * Used by area-reset 'P' command.
 */
OBJ_DATA *get_obj_type(OBJ_INDEX_DATA *pObjIndex)
{
	OBJ_DATA *obj;

	for (obj = object_list; obj != nullptr; obj = obj->next)
	{
		if (obj->pIndexData == pObjIndex)
			return obj;
	}

	return nullptr;
}

/*
 * Find an obj in a list.
 */
OBJ_DATA *get_obj_list(CHAR_DATA *ch, char *argument, OBJ_DATA *list)
{
	char arg[MAX_INPUT_LENGTH];
	OBJ_DATA *obj;
	int number;
	int count;

	number = number_argument(argument, arg);
	count = 0;
	for (obj = list; obj != nullptr; obj = obj->next_content)
	{
		if ((can_see_obj(ch, obj)) && (arg[0] == '\0' || is_name(arg, obj->name)))
		{
			if (++count == number)
				return obj;
		}
	}

	return nullptr;
}

/*
 * Find an obj in a room.
 */
OBJ_DATA *get_obj_room(OBJ_DATA *obj, ROOM_INDEX_DATA *room)
{
	OBJ_DATA *obj2;

	for (obj2 = room->contents; obj2 != nullptr; obj2 = obj2->next_content)
	{
		if (obj->pIndexData->vnum == obj2->pIndexData->vnum)
			return obj2;
	}

	return nullptr;
}

/*
 * Find an obj in player's inventory.
 */
OBJ_DATA *get_obj_carry(CHAR_DATA *ch, char *argument, CHAR_DATA *viewer)
{
	char arg[MAX_INPUT_LENGTH];
	OBJ_DATA *obj;
	int number;
	int count;

	number = number_argument(argument, arg);
	count = 0;
	for (obj = ch->carrying; obj != nullptr; obj = obj->next_content)
	{
		if (obj->wear_loc == WEAR_NONE && (can_see_obj(viewer, obj)) && (arg[0] == '\0' || is_name(arg, obj->name)))
		{
			if (++count == number)
				return obj;
		}
	}

	return nullptr;
}

/*
 * Find an obj in player's equipment.
 */
OBJ_DATA *get_obj_wear(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	OBJ_DATA *obj;
	int number;
	int count;

	number = number_argument(argument, arg);
	count = 0;
	for (obj = ch->carrying; obj != nullptr; obj = obj->next_content)
	{
		if (obj->wear_loc != WEAR_NONE && can_see_obj(ch, obj) && (arg[0] == '\0' || is_name(arg, obj->name)))
		{
			if (++count == number)
				return obj;
		}
	}

	return nullptr;
}

/*
 * Find an obj in the room or in inventory.
 */
OBJ_DATA *get_obj_here(CHAR_DATA *ch, char *argument)
{
	OBJ_DATA *obj;

	if (!str_cmp(argument, ""))
		return nullptr;

	obj = get_obj_list(ch, argument, ch->in_room->contents);
	if (obj != nullptr)
		return obj;

	if ((obj = get_obj_carry(ch, argument, ch)) != nullptr)
		return obj;

	if ((obj = get_obj_wear(ch, argument)) != nullptr)
		return obj;

	return nullptr;
}

/*
 * Find an obj in the world.
 */
OBJ_DATA *get_obj_world(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	OBJ_DATA *obj;
	int number;
	int count;

	if ((obj = get_obj_here(ch, argument)) != nullptr)
		return obj;

	number = number_argument(argument, arg);
	count = 0;

	for (obj = object_list; obj != nullptr; obj = obj->next)
	{
		if (can_see_obj(ch, obj) && is_name(arg, obj->name))
		{
			if (++count == number)
				return obj;
		}
	}

	return nullptr;
}

///
/// Deduct a sum from a character's currency pool. If this would cause
/// the character to have negative currency, it sets the value to 0 
/// instead. 
/// @param ch: The character to take from
/// @param cost: The sum to deduct
void deduct_cost(CHAR_DATA *ch, int cost)
{
	int gold = 0;

	gold = std::min(ch->gold, (long)cost);

	ch->gold -= gold;

	if (ch->gold < 0)
	{
		bug("deduct costs: gold %d < 0", ch->gold);
		ch->gold = 0;
	}
}
/*
 * Create a 'money' obj.
 */
OBJ_DATA *create_money(int gold)
{
	char buf[MAX_STRING_LENGTH];
	OBJ_DATA *obj = nullptr;

	if (gold <= 0)
	{
		bug("Create_money: zero or negative money.", gold);
		gold = std::max(1, gold);
	}
	else if (gold == 1)
	{
		obj = create_object(get_obj_index(OBJ_VNUM_GOLD_ONE), 0);
	}
	else if (gold > 1)
	{
		obj = create_object(get_obj_index(OBJ_VNUM_GOLD_SOME), 0);
		sprintf(buf, obj->short_descr, gold);
		free_pstring(obj->short_descr);
		obj->short_descr = palloc_string(buf);
		obj->value[1] = gold;
		obj->cost = gold;
		obj->weight = gold / 1000;
	}

	return obj;
}

/*
 * Return # of objects which an object counts as.
 * Thanks to Tony Chamberlain for the correct recursive code here.
 */
int get_obj_number(OBJ_DATA *obj)
{
	int number;

	if (obj->wear_loc == WEAR_BRAND || obj->item_type == ITEM_MONEY || obj->item_type == ITEM_GEM ||
		obj->item_type == ITEM_JEWELRY)
		number = 0;
	else
		number = 1;

	return number;
}

/*
 * Return weight of an object, including weight of contents.
 */
int get_obj_weight(OBJ_DATA *obj)
{
	OBJ_DATA *tobj;

	int weight = obj->weight;
	for (tobj = obj->contains; tobj != nullptr; tobj = tobj->next_content)
	{
		weight += get_obj_weight(tobj);
	}

	return weight;
}

int get_true_weight(OBJ_DATA *obj)
{
	int weight = obj->weight;
	for (obj = obj->contains; obj != nullptr; obj = obj->next_content)
	{
		weight += get_obj_weight(obj);
	}

	return weight;
}

int get_contents_weight(OBJ_DATA *obj)
{
	OBJ_DATA *stuff;
	int weight = 0;

	for (stuff = obj->contains; stuff != nullptr; stuff = stuff->next_content)
		weight += get_obj_weight(stuff);

	return weight;
}

int get_contents_number(OBJ_DATA *obj)
{
	OBJ_DATA *stuff;
	int number = 0;

	for (stuff = obj->contains; stuff != nullptr; stuff = stuff->next_content)
		number++;

	return number;
}

/*
 * True if room is dark.
 */
bool room_is_dark(ROOM_INDEX_DATA *pRoomIndex)
{

	if (pRoomIndex->light > 2)
		return false;

	if (IS_SET(pRoomIndex->room_flags, ROOM_DARK))
		return true;

	if (pRoomIndex->sector_type == SECT_INSIDE || pRoomIndex->sector_type == SECT_CITY)
		return false;

	if (sun == SolarPosition::Sunset || sun == SolarPosition::Dark)
		return true;

	return false;
}

bool is_room_owner(CHAR_DATA *ch, ROOM_INDEX_DATA *room)
{
	if (room->owner == nullptr || room->owner[0] == '\0')
		return false;

	return is_name(ch->name, room->owner);
}

/*
 * True if room is private.
 */
bool room_is_private(ROOM_INDEX_DATA *pRoomIndex)
{
	CHAR_DATA *rch;
	int count;

	return false;

	if (pRoomIndex->owner != nullptr && pRoomIndex->owner[0] != '\0')
		return true;

	count = 0;
	for (rch = pRoomIndex->people; rch != nullptr; rch = rch->next_in_room)
		count++;

	if (IS_SET(pRoomIndex->room_flags, ROOM_PRIVATE) && count >= 2)
		return true;

	if (IS_SET(pRoomIndex->room_flags, ROOM_SOLITARY) && count >= 1)
		return true;

	if (IS_SET(pRoomIndex->room_flags, ROOM_IMP_ONLY))
		return true;

	return false;
}

/* visibility on a room -- for entering and exits */
bool can_see_room(CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex)
{
	if (IS_SET(pRoomIndex->room_flags, ROOM_IMP_ONLY) && get_trust(ch) < MAX_LEVEL)
		return false;

	if (IS_SET(pRoomIndex->room_flags, ROOM_GODS_ONLY) && !is_immortal(ch))
		return false;

	if (IS_SET(pRoomIndex->room_flags, ROOM_HEROES_ONLY) && !is_immortal(ch))
		return false;

	if (IS_SET(pRoomIndex->room_flags, ROOM_NEWBIES_ONLY) && ch->level > 10 && !is_immortal(ch) &&
		(!is_npc(ch) || ch->pIndexData->vnum != ACADEMY_PET))
		return false;

	return true;
}

/*
 * True if char can see victim.
 */
bool can_see(CHAR_DATA *ch, CHAR_DATA *victim)
{
	AREA_AFFECT_DATA *paf;
	AFFECT_DATA *af;

	if (is_npc(ch) && IS_SET(ch->act, ACT_DETECT_SPECIAL))
		return true;

	if (ch == victim)
		return true;

	if (!is_immortal(ch) && is_affected_by(victim, AFF_NOSHOW))
		return false;

	if (is_cabal_guard(ch) && !ch->desc)
		return true;

	if (get_trust(ch) < victim->invis_level)
		return false;

	if (get_trust(ch) < victim->incog_level && ch->in_room != victim->in_room)
		return false;

	if (get_trust(ch) == LEVEL_HERO && victim->invis_level == LEVEL_HERO && !is_heroimm(ch))
		return false;

	if ((!is_npc(ch) && IS_SET(ch->act, PLR_HOLYLIGHT)) || (is_npc(ch) && is_immortal(ch)))
		return true;

	if (is_npc(ch) && victim->invis_level >= LEVEL_HERO)
		return false;

	if (!is_npc(ch) && ch->pcdata->death_status == HAS_DIED)
		return true;

	/* Make sure cabal guardians can always see */
	if (is_cabal_guard(ch))
		return true;

	if (is_affected_by(ch, AFF_BLIND))
		return false;

	if (is_affected(victim, gsn_ultradiffusion))
		return false;

	if (is_affected_area(ch->in_room->area, gsn_whiteout) && is_outside(ch))
	{
		for (paf = ch->in_room->area->affected; paf; paf = paf->next)
		{
			if (paf->type == gsn_whiteout)
				break;
		}

		if (paf->owner != ch)
			return false;
	}

	if (is_affected(victim, gsn_watermeld) && !is_affected(ch, gsn_hydroperception))
		return false;

	if (is_affected(victim, gsn_earthfade))
		return false;

	if (is_affected_by(victim, AFF_INVISIBLE)
		&& !is_affected_by(ch, AFF_DETECT_INVIS)
		&& (!(is_affected(ch, gsn_hydroperception)
			&& (ch->in_room->sector_type == SECT_WATER
				|| ch->in_room->sector_type == SECT_UNDERWATER
				|| (ch->in_room->sector_type != SECT_INSIDE && victim->in_room->area->sky >= WeatherCondition::Drizzle))
			&& ch->in_room == victim->in_room))
		&& !(is_affected(ch, gsn_sense_disturbance)
			&& ch->in_room->sector_type != SECT_UNDERWATER
			&& victim->in_room->sector_type != SECT_UNDERWATER)
		&& (!(is_affected(ch, gsn_sensevibrations)
			&& victim->in_room->sector_type != SECT_UNDERWATER
			&& victim->in_room->sector_type != SECT_AIR
			&& ch->in_room->sector_type != SECT_UNDERWATER
			&& ch->in_room->sector_type != SECT_AIR
			&& !is_affected_by(victim, AFF_SNEAK)
			&& !is_affected_by(victim, AFF_FLYING))))
	{
		return false;
	}

	if (is_affected_by(victim, AFF_HIDE)
		&& !is_affected_by(ch, AFF_DETECT_HIDDEN)
		&& victim->fighting == nullptr
		&& !(is_affected(ch, gsn_darksight)
			&& (af = affect_find(ch->affected, gsn_darksight))
			&& af->aftype == AFT_SKILL
			&& room_is_dark(victim->in_room))
		&& !(is_affected(ch, gsn_hydroperception)
			&& (ch->in_room->sector_type == SECT_WATER
				|| ch->in_room->sector_type == SECT_UNDERWATER
				|| (ch->in_room->sector_type != SECT_INSIDE && victim->in_room->area->sky >= WeatherCondition::Drizzle))
			&& ch->in_room == victim->in_room)
		&& !(is_affected(ch, gsn_sensevibrations)
			&& victim->in_room->sector_type != SECT_UNDERWATER
			&& victim->in_room->sector_type != SECT_AIR
			&& ch->in_room->sector_type != SECT_UNDERWATER
			&& ch->in_room->sector_type != SECT_AIR
			&& !is_affected_by(victim, AFF_SNEAK)
			&& !is_affected_by(victim, AFF_FLYING)))
	{
		return false;
	}

	if (is_affected_by(victim, AFF_CAMOUFLAGE) && !is_affected_by(ch, AFF_DETECT_CAMO))
		return false;

	if (is_npc(ch))
		return true;

	if (room_is_dark(ch->in_room) && !is_affected_by(ch, AFF_DARK_VISION))
		return false;

	return true;
}

/*
 * True if char can see obj.
 */
bool can_see_obj(CHAR_DATA *ch, OBJ_DATA *obj)
{
	AREA_AFFECT_DATA *paf;
	OBJ_AFFECT_DATA *oaf;
	OBJ_DATA *tat;

	if (!ch->in_room)
	{
		bug("ERROR IN CANSEEOBJ!", 0);
		return false;
	}

	if (is_affected_obj(obj, gsn_stash))
	{
		for (oaf = obj->affected; oaf != nullptr; oaf = oaf->next)
		{
			if (oaf->type == gsn_stash)
				break;
		}

		if (oaf->owner != ch && !is_immortal(ch))
			return false;
	}

	if (is_obj_stat(obj, ITEM_NOSHOW) && !is_immortal(ch) && is_worn(obj) && (tat = get_eq_char(ch, WEAR_BRAND)) != obj)
		return false;

	if (!is_npc(ch) && IS_SET(ch->act, PLR_HOLYLIGHT))
		return true;

	if (IS_SET(obj->extra_flags, ITEM_VIS_DEATH))
		return false;

	if ((is_affected_by(ch, AFF_BLIND) && obj->item_type != ITEM_POTION))
		return false;

	if (is_affected_area(ch->in_room->area, gsn_whiteout) && is_outside(ch) && obj->item_type != ITEM_POTION)
	{
		for (paf = ch->in_room->area->affected; paf; paf = paf->next)
		{
			if (paf->type == gsn_whiteout)
				break;
		}

		if (paf && paf->owner != ch)
			return false;
	}

	if (obj->item_type == ITEM_LIGHT && obj->value[2] != 0)
		return true;

	if (IS_SET(obj->extra_flags, ITEM_INVIS) && !is_affected_by(ch, AFF_DETECT_INVIS))
		return false;

	if (is_obj_stat(obj, ITEM_GLOW) || is_affected_by(obj, AFF_OBJ_BURNING))
		return true;

	if (room_is_dark(ch->in_room) && !is_affected_by(ch, AFF_DARK_VISION))
		return false;

	return true;
}

/*
 * True if char can drop obj.
 */
bool can_drop_obj(CHAR_DATA *ch, OBJ_DATA *obj)
{
	if (!IS_SET(obj->extra_flags, ITEM_NODROP))
		return true;

	if (!is_npc(ch) && ch->level >= LEVEL_IMMORTAL)
		return true;

	return false;
}

/*
 * Return ascii name of an affect location.
 */
char *affect_loc_name(int location)
{
	for (int i = 0; apply_locations[i].name != nullptr; i++)
	{
		if (apply_locations[i].bit == location)
			return apply_locations[i].display;
	}

	bug("Affect_location_name: unknown location %d.", location);
	return "(unknown)";
}

/*
 * Return ascii name of an affect bit vector.
 */
char *affect_bit_name(long vector[])
{
	static char buf[512];

	buf[0] = '\0';
	if (IS_SET(vector, AFF_BLIND))
		strcat(buf, " blind");

	if (IS_SET(vector, AFF_INVISIBLE))
		strcat(buf, " invisible");

	if (IS_SET(vector, AFF_DETECT_EVIL))
		strcat(buf, " detect_evil");

	if (IS_SET(vector, AFF_DETECT_GOOD))
		strcat(buf, " detect_good");

	if (IS_SET(vector, AFF_DETECT_INVIS))
		strcat(buf, " detect_invis");

	if (IS_SET(vector, AFF_DETECT_MAGIC))
		strcat(buf, " detect_magic");

	if (IS_SET(vector, AFF_DETECT_HIDDEN))
		strcat(buf, " detect_hidden");

	if (IS_SET(vector, AFF_SANCTUARY))
		strcat(buf, " sanctuary");

	if (IS_SET(vector, AFF_DETECT_CAMO))
		strcat(buf, " detect_camo");

	if (IS_SET(vector, AFF_CURSE))
		strcat(buf, " curse");

	if (IS_SET(vector, AFF_POISON))
		strcat(buf, " poison");

	if (IS_SET(vector, AFF_PROTECTION))
		strcat(buf, " protection");

	if (IS_SET(vector, AFF_SLEEP))
		strcat(buf, " sleep");

	if (IS_SET(vector, AFF_SNEAK))
		strcat(buf, " sneak");

	if (IS_SET(vector, AFF_HIDE))
		strcat(buf, " hide");

	if (IS_SET(vector, AFF_CHARM))
		strcat(buf, " charm");

	if (IS_SET(vector, AFF_FLYING))
		strcat(buf, " flying");

	if (IS_SET(vector, AFF_WATERBREATH))
		strcat(buf, " waterbreath");

	if (IS_SET(vector, AFF_PASS_DOOR))
		strcat(buf, " pass_door");

	if (IS_SET(vector, AFF_BERSERK))
		strcat(buf, " berserk");

	if (IS_SET(vector, AFF_CALM))
		strcat(buf, " calm");

	if (IS_SET(vector, AFF_HASTE))
		strcat(buf, " haste");

	if (IS_SET(vector, AFF_SLOW))
		strcat(buf, " slow");

	if (IS_SET(vector, AFF_PLAGUE))
		strcat(buf, " plague");

	if (IS_SET(vector, AFF_DARK_VISION))
		strcat(buf, " dark_vision");

	if (IS_SET(vector, AFF_CAMOUFLAGE))
		strcat(buf, " camouflage");

	if (IS_SET(vector, AFF_PERMANENT))
		strcat(buf, " permanent");

	if (IS_SET(vector, AFF_RAGE))
		strcat(buf, " rage");

	if (IS_SET(vector, AFF_NOSHOW))
		strcat(buf, " noshow");

	return (buf[0] != '\0') ? buf + 1 : (char *)"none";
}

/*
 * Return ascii name of extra flags vector.
 */
char *extra_bit_name(long extra_flags[])
{
	static char buf[512];

	buf[0] = '\0';

	if (IS_SET(extra_flags, ITEM_GLOW))
		strcat(buf, " glow");

	if (IS_SET(extra_flags, ITEM_HUM))
		strcat(buf, " hum");

	if (IS_SET(extra_flags, ITEM_DARK))
		strcat(buf, " dark");

	if (IS_SET(extra_flags, ITEM_NOSHOW))
		strcat(buf, " noshow");

	if (IS_SET(extra_flags, ITEM_EVIL))
		strcat(buf, " evil");

	if (IS_SET(extra_flags, ITEM_INVIS))
		strcat(buf, " invis");

	if (IS_SET(extra_flags, ITEM_MAGIC))
		strcat(buf, " magic");

	if (IS_SET(extra_flags, ITEM_NODROP))
		strcat(buf, " nodrop");

	if (IS_SET(extra_flags, ITEM_BLESS))
		strcat(buf, " bless");

	if (IS_SET(extra_flags, ITEM_ANTI_GOOD))
		strcat(buf, " anti-good");

	if (IS_SET(extra_flags, ITEM_ANTI_EVIL))
		strcat(buf, " anti-evil");

	if (IS_SET(extra_flags, ITEM_ANTI_NEUTRAL))
		strcat(buf, " anti-neutral");

	if (IS_SET(extra_flags, ITEM_ANTI_LAWFUL))
		strcat(buf, " anti-lawful");

	if (IS_SET(extra_flags, ITEM_ANTI_NEUT))
		strcat(buf, " anti-neutrale");

	if (IS_SET(extra_flags, ITEM_ANTI_CHAOTIC))
		strcat(buf, " anti-chaotic");

	if (IS_SET(extra_flags, ITEM_NOREMOVE))
		strcat(buf, " noremove");

	if (IS_SET(extra_flags, ITEM_INVENTORY))
		strcat(buf, " inventory");

	if (IS_SET(extra_flags, ITEM_NOPURGE))
		strcat(buf, " nopurge");

	if (IS_SET(extra_flags, ITEM_VIS_DEATH))
		strcat(buf, " vis_death");

	if (IS_SET(extra_flags, ITEM_ROT_DEATH))
		strcat(buf, " rot_death");

	if (IS_SET(extra_flags, ITEM_NOLOCATE))
		strcat(buf, " no_locate");

	if (IS_SET(extra_flags, ITEM_MELT_DROP))
		strcat(buf, " melt_drop");

	if (IS_SET(extra_flags, ITEM_SELL_EXTRACT))
		strcat(buf, " sell_extract");

	if (IS_SET(extra_flags, ITEM_BURN_PROOF))
		strcat(buf, " burn_proof");

	if (IS_SET(extra_flags, ITEM_NOUNCURSE))
		strcat(buf, " no_uncurse");

	if (IS_SET(extra_flags, CORPSE_NO_ANIMATE))
		strcat(buf, " no_animate");

	if (IS_SET(extra_flags, ITEM_BRAND))
		strcat(buf, " branded");

	if (IS_SET(extra_flags, ITEM_FIXED))
		strcat(buf, " fixed");

	if (IS_SET(extra_flags, ITEM_NODISARM))
		strcat(buf, " nodisarm");

	if (IS_SET(extra_flags, ITEM_NO_SAC))
		strcat(buf, " no_sac");

	return (buf[0] != '\0') ? buf + 1 : (char *)"none";
}

/* Return ascii name for restrict flags */
char *restrict_bit_name(long restrict_flags[])
{
	static char buf[512];
	buf[0] = '\0';

	for (int i = 0; restrict_table[i].name != nullptr; i++)
	{
		if (IS_SET(restrict_flags, restrict_table[i].bit))
		{
			strcat(buf, " ");
			strcat(buf, palloc_string(restrict_table[i].name));
		}
	}

	return (buf[0] != '\0') ? buf + 1 : (char *)"none";
}

/* return ascii name of an act vector */
char *act_bit_name(long act_flags[])
{
	static char buf[512];

	buf[0] = '\0';

	if (IS_SET(act_flags, ACT_IS_NPC))
	{
		strcat(buf, " npc");

		if (IS_SET(act_flags, ACT_SENTINEL))
			strcat(buf, " sentinel");

		if (IS_SET(act_flags, ACT_SCAVENGER))
			strcat(buf, " scavenger");

		if (IS_SET(act_flags, ACT_AGGRESSIVE))
			strcat(buf, " aggressive");

		if (IS_SET(act_flags, ACT_STAY_AREA))
			strcat(buf, " stay_area");

		if (IS_SET(act_flags, ACT_WARD_MOB))
			strcat(buf, " ward_mob");

		if (IS_SET(act_flags, ACT_WIMPY))
			strcat(buf, " wimpy");

		if (IS_SET(act_flags, ACT_PET))
			strcat(buf, " pet");

		if (IS_SET(act_flags, ACT_TRAIN))
			strcat(buf, " train");

		if (IS_SET(act_flags, ACT_PRACTICE))
			strcat(buf, " practice");

		if (IS_SET(act_flags, ACT_UNDEAD))
			strcat(buf, " undead");

		if (IS_SET(act_flags, ACT_CLERIC))
			strcat(buf, " cleric");

		if (IS_SET(act_flags, ACT_MAGE))
			strcat(buf, " mage");

		if (IS_SET(act_flags, ACT_FAST_TRACK))
			strcat(buf, " fast_track");

		if (IS_SET(act_flags, ACT_INTELLIGENT))
			strcat(buf, " intelligent");

		if (IS_SET(act_flags, ACT_NOALIGN))
			strcat(buf, " no_align");

		if (IS_SET(act_flags, ACT_NOPURGE))
			strcat(buf, " no_purge");

		if (IS_SET(act_flags, ACT_IS_HEALER))
			strcat(buf, " healer");

		if (IS_SET(act_flags, ACT_GUILDGUARD))
			strcat(buf, " guildguard");

		if (IS_SET(act_flags, ACT_GAIN))
			strcat(buf, " skill_train");

		if (IS_SET(act_flags, ACT_UPDATE_ALWAYS))
			strcat(buf, " update_always");

		if (IS_SET(act_flags, ACT_BANKER))
			strcat(buf, " banker");

		if (IS_SET(act_flags, ACT_INNER_GUARDIAN))
			strcat(buf, " inner_guardian");

		if (IS_SET(act_flags, ACT_NOCTURNAL))
			strcat(buf, " nocturnal");

		if (IS_SET(act_flags, ACT_DIURNAL))
			strcat(buf, " diurnal");

		if (IS_SET(act_flags, ACT_FASTWANDER))
			strcat(buf, " fastwander");

		if (IS_SET(act_flags, ACT_LAW))
			strcat(buf, " law");
	}
	else
	{
		strcat(buf, " player");

		if (IS_SET(act_flags, PLR_AUTOABORT))
			strcat(buf, " autoabort");

		if (IS_SET(act_flags, PLR_AUTOASSIST))
			strcat(buf, " autoassist");

		if (IS_SET(act_flags, PLR_AUTOEXIT))
			strcat(buf, " autoexit");

		if (IS_SET(act_flags, PLR_AUTOLOOT))
			strcat(buf, " autoloot");

		if (IS_SET(act_flags, PLR_AUTOSAC))
			strcat(buf, " autosac");

		if (IS_SET(act_flags, PLR_AUTOGOLD))
			strcat(buf, " autogold");

		if (IS_SET(act_flags, PLR_AUTOSPLIT))
			strcat(buf, " autosplit");

		if (IS_SET(act_flags, PLR_BETRAYER))
			strcat(buf, " betrayer");

		if (IS_SET(act_flags, PLR_HOLYLIGHT))
			strcat(buf, " holy_light");

		if (IS_SET(act_flags, PLR_EMPOWERED))
			strcat(buf, " empowered");

		if (IS_SET(act_flags, PLR_NOVOID))
			strcat(buf, " no_void");

		if (IS_SET(act_flags, PLR_NOSUMMON))
			strcat(buf, " no_summon");

		if (IS_SET(act_flags, PLR_NOFOLLOW))
			strcat(buf, " no_follow");

		if (IS_SET(act_flags, PLR_MORON))
			strcat(buf, " moron");

		if (IS_SET(act_flags, PLR_FREEZE))
			strcat(buf, " frozen");

		if (IS_SET(act_flags, PLR_THIEF))
			strcat(buf, " thief");

		if (IS_SET(act_flags, PLR_KILLER))
			strcat(buf, " killer");

		if (IS_SET(act_flags, PLR_CRIMINAL))
			strcat(buf, " criminal");

		if (IS_SET(act_flags, PLR_HEROIMM))
			strcat(buf, " heroimm");
	}

	return (buf[0] != '\0') ? buf + 1 : (char *)"none";
}

char *comm_bit_name(long comm_flags[])
{
	static char buf[512];

	buf[0] = '\0';

	if (IS_SET(comm_flags, COMM_QUIET))
		strcat(buf, " quiet");

	if (IS_SET(comm_flags, COMM_DEAF))
		strcat(buf, " deaf");

	if (IS_SET(comm_flags, COMM_NOWIZ))
		strcat(buf, " no_wiz");

	if (IS_SET(comm_flags, COMM_NOAUCTION))
		strcat(buf, " no_auction");

	if (IS_SET(comm_flags, COMM_NOGOSSIP))
		strcat(buf, " no_gossip");

	if (IS_SET(comm_flags, COMM_NOQUESTION))
		strcat(buf, " no_question");

	if (IS_SET(comm_flags, COMM_NONEWBIE))
		strcat(buf, " no_newbie");

	if (IS_SET(comm_flags, COMM_NOQUOTE))
		strcat(buf, " no_quote");

	if (IS_SET(comm_flags, COMM_COMPACT))
		strcat(buf, " compact");

	if (IS_SET(comm_flags, COMM_BRIEF))
		strcat(buf, " brief");

	if (IS_SET(comm_flags, COMM_PROMPT))
		strcat(buf, " prompt");

	if (IS_SET(comm_flags, COMM_COMBINE))
		strcat(buf, " combine");

	if (IS_SET(comm_flags, COMM_NOEMOTE))
		strcat(buf, " no_emote");

	if (IS_SET(comm_flags, COMM_NOSHOUT))
		strcat(buf, " no_shout");

	if (IS_SET(comm_flags, COMM_NOTELL))
		strcat(buf, " no_tell");

	if (IS_SET(comm_flags, COMM_NOCHANNELS))
		strcat(buf, " no_channels");

	if (IS_SET(comm_flags, COMM_ANSI))
		strcat(buf, " color");

	if (IS_SET(comm_flags, COMM_SWITCHSKILLS))
		strcat(buf, " switchskills");

	if (IS_SET(comm_flags, COMM_LOTS_O_COLOR))
		strcat(buf, " lots o color!");

	return (buf[0] != '\0') ? buf + 1 : (char *)"none";
}

char *imm_bit_name(long imm_flags[])
{
	static char buf[512];

	buf[0] = '\0';

	if (IS_SET(imm_flags, IMM_SUMMON))
		strcat(buf, " summon");

	if (IS_SET(imm_flags, IMM_CHARM))
		strcat(buf, " charm");

	if (IS_SET(imm_flags, IMM_MAGIC))
		strcat(buf, " magic");

	if (IS_SET(imm_flags, IMM_WEAPON))
		strcat(buf, " weapon");

	if (IS_SET(imm_flags, IMM_BASH))
		strcat(buf, " blunt");

	if (IS_SET(imm_flags, IMM_PIERCE))
		strcat(buf, " piercing");

	if (IS_SET(imm_flags, IMM_SLASH))
		strcat(buf, " slashing");

	if (IS_SET(imm_flags, IMM_FIRE))
		strcat(buf, " fire");

	if (IS_SET(imm_flags, IMM_COLD))
		strcat(buf, " cold");

	if (IS_SET(imm_flags, IMM_LIGHTNING))
		strcat(buf, " lightning");

	if (IS_SET(imm_flags, IMM_ACID))
		strcat(buf, " acid");

	if (IS_SET(imm_flags, IMM_POISON))
		strcat(buf, " poison");

	if (IS_SET(imm_flags, IMM_NEGATIVE))
		strcat(buf, " negative");

	if (IS_SET(imm_flags, IMM_HOLY))
		strcat(buf, " holy");

	if (IS_SET(imm_flags, IMM_ENERGY))
		strcat(buf, " energy");

	if (IS_SET(imm_flags, IMM_MENTAL))
		strcat(buf, " mental");

	if (IS_SET(imm_flags, IMM_DISEASE))
		strcat(buf, " disease");

	if (IS_SET(imm_flags, IMM_DROWNING))
		strcat(buf, " drowning");

	if (IS_SET(imm_flags, IMM_LIGHT))
		strcat(buf, " light");

	if (IS_SET(imm_flags, IMM_IRON))
		strcat(buf, " iron");

	if (IS_SET(imm_flags, IMM_MITHRIL))
		strcat(buf, " mithril");

	if (IS_SET(imm_flags, IMM_SILVER))
		strcat(buf, " silver");

	if (IS_SET(imm_flags, IMM_SLEEP))
		strcat(buf, " sleep");

	if (IS_SET(imm_flags, IMM_SOUND))
		strcat(buf, " sound");

	if (IS_SET(imm_flags, IMM_INTERNAL))

		strcat(buf, " internal");
	return (buf[0] != '\0') ? buf + 1 : (char *)"none";
}

char *wear_bit_name(long wear_flags[])
{
	static char buf[512];

	buf[0] = '\0';

	if (IS_SET(wear_flags, ITEM_TAKE))
		strcat(buf, " take");

	if (IS_SET(wear_flags, ITEM_WEAR_FINGER))
		strcat(buf, " finger");

	if (IS_SET(wear_flags, ITEM_WEAR_NECK))
		strcat(buf, " neck");

	if (IS_SET(wear_flags, ITEM_WEAR_BODY))
		strcat(buf, " torso");

	if (IS_SET(wear_flags, ITEM_WEAR_HEAD))
		strcat(buf, " head");

	if (IS_SET(wear_flags, ITEM_WEAR_LEGS))
		strcat(buf, " legs");

	if (IS_SET(wear_flags, ITEM_WEAR_FEET))
		strcat(buf, " feet");

	if (IS_SET(wear_flags, ITEM_WEAR_HANDS))
		strcat(buf, " hands");

	if (IS_SET(wear_flags, ITEM_WEAR_ARMS))
		strcat(buf, " arms");

	if (IS_SET(wear_flags, ITEM_WEAR_SHIELD))
		strcat(buf, " shield");

	if (IS_SET(wear_flags, ITEM_WEAR_ABOUT))
		strcat(buf, " body");

	if (IS_SET(wear_flags, ITEM_WEAR_WAIST))
		strcat(buf, " waist");

	if (IS_SET(wear_flags, ITEM_WEAR_WRIST))
		strcat(buf, " wrist");

	if (IS_SET(wear_flags, ITEM_WEAR_WIELD))
		strcat(buf, " wield");

	if (IS_SET(wear_flags, ITEM_WEAR_HOLD))
		strcat(buf, " hold");

	if (IS_SET(wear_flags, ITEM_WEAR_FLOAT))
		strcat(buf, " float");

	if (IS_SET(wear_flags, ITEM_WEAR_BRAND))
		strcat(buf, " brand");

	if (IS_SET(wear_flags, ITEM_WEAR_STRAPPED))
		strcat(buf, " strapped");

	if (IS_SET(wear_flags, ITEM_WEAR_COSMETIC))
		strcat(buf, " cosmetic");

	return (buf[0] != '\0') ? buf + 1 : (char *)"none";
}

char *form_bit_name(long form_flags[])
{
	static char buf[512];

	buf[0] = '\0';

	if (IS_SET(form_flags, FORM_POISON))
		strcat(buf, " poison");
	else if (IS_SET(form_flags, FORM_EDIBLE))
		strcat(buf, " edible");

	if (IS_SET(form_flags, FORM_MAGICAL))
		strcat(buf, " magical");

	if (IS_SET(form_flags, FORM_INSTANT_DECAY))
		strcat(buf, " instant_rot");

	if (IS_SET(form_flags, FORM_OTHER))
		strcat(buf, " other");

	if (IS_SET(form_flags, FORM_ANIMAL))
		strcat(buf, " animal");

	if (IS_SET(form_flags, FORM_SENTIENT))
		strcat(buf, " sentient");

	if (IS_SET(form_flags, FORM_UNDEAD))
		strcat(buf, " undead");

	if (IS_SET(form_flags, FORM_CONSTRUCT))
		strcat(buf, " construct");

	if (IS_SET(form_flags, FORM_MIST))
		strcat(buf, " mist");

	if (IS_SET(form_flags, FORM_INTANGIBLE))
		strcat(buf, " intangible");

	if (IS_SET(form_flags, FORM_BIPED))
		strcat(buf, " biped");

	if (IS_SET(form_flags, FORM_AQUATIC))
		strcat(buf, " aquatic");

	if (IS_SET(form_flags, FORM_INSECT))
		strcat(buf, " insect");

	if (IS_SET(form_flags, FORM_SPIDER))
		strcat(buf, " spider");

	if (IS_SET(form_flags, FORM_CRUSTACEAN))
		strcat(buf, " crustacean");

	if (IS_SET(form_flags, FORM_WORM))
		strcat(buf, " worm");

	if (IS_SET(form_flags, FORM_BLOB))
		strcat(buf, " blob");

	if (IS_SET(form_flags, FORM_MAMMAL))
		strcat(buf, " mammal");

	if (IS_SET(form_flags, FORM_BIRD))
		strcat(buf, " bird");

	if (IS_SET(form_flags, FORM_REPTILE))
		strcat(buf, " reptile");

	if (IS_SET(form_flags, FORM_SNAKE))
		strcat(buf, " snake");

	if (IS_SET(form_flags, FORM_DRAGON))
		strcat(buf, " dragon");

	if (IS_SET(form_flags, FORM_AMPHIBIAN))
		strcat(buf, " amphibian");

	if (IS_SET(form_flags, FORM_FISH))
		strcat(buf, " fish");

	if (IS_SET(form_flags, FORM_COLD_BLOOD))
		strcat(buf, " cold_blooded");

	return (buf[0] != '\0') ? buf + 1 : (char *)"none";
}

char *part_bit_name(long part_flags[])
{
	static char buf[512];

	buf[0] = '\0';

	if (IS_SET(part_flags, PART_HEAD))
		strcat(buf, " head");

	if (IS_SET(part_flags, PART_ARMS))
		strcat(buf, " arms");

	if (IS_SET(part_flags, PART_LEGS))
		strcat(buf, " legs");

	if (IS_SET(part_flags, PART_HEART))
		strcat(buf, " heart");

	if (IS_SET(part_flags, PART_BRAINS))
		strcat(buf, " brains");

	if (IS_SET(part_flags, PART_GUTS))
		strcat(buf, " guts");

	if (IS_SET(part_flags, PART_HANDS))
		strcat(buf, " hands");

	if (IS_SET(part_flags, PART_FEET))
		strcat(buf, " feet");

	if (IS_SET(part_flags, PART_FINGERS))
		strcat(buf, " fingers");

	if (IS_SET(part_flags, PART_EAR))
		strcat(buf, " ears");

	if (IS_SET(part_flags, PART_EYE))
		strcat(buf, " eyes");

	if (IS_SET(part_flags, PART_LONG_TONGUE))
		strcat(buf, " long_tongue");

	if (IS_SET(part_flags, PART_EYESTALKS))
		strcat(buf, " eyestalks");

	if (IS_SET(part_flags, PART_TENTACLES))
		strcat(buf, " tentacles");

	if (IS_SET(part_flags, PART_FINS))
		strcat(buf, " fins");

	if (IS_SET(part_flags, PART_WINGS))
		strcat(buf, " wings");

	if (IS_SET(part_flags, PART_TAIL))
		strcat(buf, " tail");

	if (IS_SET(part_flags, PART_CLAWS))
		strcat(buf, " claws");

	if (IS_SET(part_flags, PART_FANGS))
		strcat(buf, " fangs");

	if (IS_SET(part_flags, PART_HORNS))
		strcat(buf, " horns");

	if (IS_SET(part_flags, PART_SCALES))
		strcat(buf, " scales");

	return (buf[0] != '\0') ? buf + 1 : (char *)"none";
}

char *weapon_bit_name(int weapon_flags)
{
	static char buf[512];

	buf[0] = '\0';

	if (IS_SET_OLD(weapon_flags, WEAPON_FLAMING))
		strcat(buf, " flaming");

	if (IS_SET_OLD(weapon_flags, WEAPON_FROST))
		strcat(buf, " frost");

	if (IS_SET_OLD(weapon_flags, WEAPON_VAMPIRIC))
		strcat(buf, " vampiric");

	if (IS_SET_OLD(weapon_flags, WEAPON_SHARP))
		strcat(buf, " sharp");

	if (IS_SET_OLD(weapon_flags, WEAPON_VORPAL))
		strcat(buf, " vorpal");

	if (IS_SET_OLD(weapon_flags, WEAPON_TWO_HANDS))
		strcat(buf, " two-handed");

	if (IS_SET_OLD(weapon_flags, WEAPON_SHOCKING))
		strcat(buf, " shocking");

	if (IS_SET_OLD(weapon_flags, WEAPON_POISON))
		strcat(buf, " poison");

	return (buf[0] != '\0') ? buf + 1 : (char *)"none";
}

char *cont_bit_name(int cont_flags)
{
	static char buf[512];

	buf[0] = '\0';

	if (IS_SET_OLD(cont_flags, CONT_CLOSEABLE))
		strcat(buf, " closable");

	if (IS_SET_OLD(cont_flags, CONT_PICKPROOF))
		strcat(buf, " pickproof");

	if (IS_SET_OLD(cont_flags, CONT_CLOSED))
		strcat(buf, " closed");

	if (IS_SET_OLD(cont_flags, CONT_LOCKED))
		strcat(buf, " locked");

	return (buf[0] != '\0') ? buf + 1 : (char *)"none";
}

char *off_bit_name(long off_flags[])
{
	static char buf[512];

	buf[0] = '\0';

	if (IS_SET(off_flags, OFF_AREA_ATTACK))
		strcat(buf, " area_attack");

	if (IS_SET(off_flags, OFF_BACKSTAB))
		strcat(buf, " backstab");

	if (IS_SET(off_flags, OFF_BASH))
		strcat(buf, " bash");

	if (IS_SET(off_flags, OFF_BERSERK))
		strcat(buf, " berserk");

	if (IS_SET(off_flags, OFF_DISARM))
		strcat(buf, " disarm");

	if (IS_SET(off_flags, OFF_DODGE))
		strcat(buf, " dodge");

	if (IS_SET(off_flags, OFF_FADE))
		strcat(buf, " fade");

	if (IS_SET(off_flags, OFF_FAST))
		strcat(buf, " fast");

	if (IS_SET(off_flags, OFF_KICK))
		strcat(buf, " kick");

	if (IS_SET(off_flags, OFF_KICK_DIRT))
		strcat(buf, " kick_dirt");

	if (IS_SET(off_flags, OFF_PARRY))
		strcat(buf, " parry");

	if (IS_SET(off_flags, OFF_TAIL))
		strcat(buf, " tail");

	if (IS_SET(off_flags, OFF_TRIP))
		strcat(buf, " trip");

	if (IS_SET(off_flags, OFF_CRUSH))
		strcat(buf, " crush");

	if (IS_SET(off_flags, ASSIST_ALL))
		strcat(buf, " assist_all");

	if (IS_SET(off_flags, ASSIST_ALIGN))
		strcat(buf, " assist_align");

	if (IS_SET(off_flags, ASSIST_RACE))
		strcat(buf, " assist_race");

	if (IS_SET(off_flags, ASSIST_PLAYERS))
		strcat(buf, " assist_players");

	if (IS_SET(off_flags, ASSIST_GUARD))
		strcat(buf, " assist_guard");

	if (IS_SET(off_flags, ASSIST_VNUM))
		strcat(buf, " assist_vnum");

	if (IS_SET(off_flags, NO_TRACK))
		strcat(buf, " no_track");

	if (IS_SET(off_flags, STATIC_TRACKING))
		strcat(buf, " static_track");

	if (IS_SET(off_flags, SPAM_MURDER))
		strcat(buf, " spam_murder");

	if (IS_SET(off_flags, ASSIST_GROUP))
		strcat(buf, " assist_group");

	return (buf[0] != '\0') ? buf + 1 : (char *)"none";
}

/* Room Affects */

/*
 * Apply or remove an affect to a room.
 */

void init_affect_room(ROOM_AFFECT_DATA *paf)
{
	paf->where = TO_ROOM_AFFECTS;
	paf->type = 0;
	paf->aftype = AFT_SPELL;
	paf->level = 0;
	paf->duration = 0;
	paf->location = 0;
	paf->modifier = 0;

	zero_vector(paf->bitvector);

	paf->pulse_fun = nullptr;
	paf->tick_fun = nullptr;
	paf->end_fun = nullptr;
	paf->owner = nullptr;
	/* Morg - Valgrind fix */
	paf->valid= false;
}

void affect_modify_room(ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *paf, bool fAdd)
{
	int mod = paf->modifier;

	if (fAdd)
	{
		switch (paf->where)
		{
			case TO_ROOM_AFFECTS:
				BITWISE_OR(room->affected_by, paf->bitvector);
				break;
			case TO_ROOM_FLAGS:
				BITWISE_OR(room->room_flags, paf->bitvector);
				break;
			case TO_ROOM_CONST:
				break;
		}
	}
	else
	{
		switch (paf->where)
		{
			case TO_ROOM_AFFECTS:
				BITWISE_XAND(room->affected_by, paf->bitvector);
				break;
			case TO_ROOM_FLAGS:
				BITWISE_XAND(room->room_flags, paf->bitvector);
				break;
			case TO_ROOM_CONST:
				break;
		}

		mod = 0 - mod;
	}

	switch (paf->location)
	{
		case APPLY_ROOM_NONE:
			break;
		case APPLY_ROOM_NOPE:
			break;
		case APPLY_ROOM_HEAL:
			room->heal_rate += mod;
			break;
		case APPLY_ROOM_MANA:
			room->mana_rate += mod;
			break;
		case APPLY_ROOM_SECT:
			room->sector_type += mod;
			break;
		default:
			bug("Affect_modify_room: unknown location %d.", paf->location);
			break;
	}
}

/*
 * Give an affect to a room.
 */
void affect_to_room(ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *paf)
{
	paf->owner = nullptr;
	paf->end_fun = nullptr;
	paf->tick_fun = nullptr;
	new_affect_to_room(room, paf);
}

void new_affect_to_room(ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *paf)
{
	ROOM_AFFECT_DATA *paf_new;
	ROOM_INDEX_DATA *pRoomIndex;

	if (!room->affected)
	{
		if (top_affected_room)
		{

			for (pRoomIndex = top_affected_room; pRoomIndex->aff_next != nullptr; pRoomIndex = pRoomIndex->aff_next)
			{
				continue;
			}

			pRoomIndex->aff_next = room;
		}
		else
		{
			top_affected_room = room;
		}

		room->aff_next = nullptr;
	}

	paf_new = new_affect_room();

	*paf_new = *paf;
	paf_new->next = room->affected;
	room->affected = paf_new;

	affect_modify_room(room, paf_new, true);
}

void affect_check_room(ROOM_INDEX_DATA *room, int where, long vector[])
{
	ROOM_AFFECT_DATA *paf;

	if (IS_ZERO_VECTOR(vector))
		return;

	for (paf = room->affected; paf != nullptr; paf = paf->next)
	{
		if (paf->where == where && vector_equal(paf->bitvector, vector))
		{
			switch (where)
			{
				case TO_ROOM_AFFECTS:
					BITWISE_OR(room->affected_by, vector);
					break;
				case TO_ROOM_FLAGS:
					BITWISE_OR(room->room_flags, vector);
					break;
				case TO_ROOM_CONST:
					break;
			}

			return;
		}
	}
}

/*
 * Remove an affect from a room.
 */
void affect_remove_room(ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *paf)
{
	int where;
	long vector[MAX_BITVECTOR];

	if (room->affected == nullptr)
	{
		bug("Affect_remove_room: no affect.", 0);
		return;
	}

	if (paf->end_fun != nullptr)
		(*paf->end_fun)(room, paf);

	affect_modify_room(room, paf, false);
	where = paf->where;
	copy_vector(vector, paf->bitvector);

	if (paf == room->affected)
	{
		room->affected = paf->next;
	}
	else
	{
		ROOM_AFFECT_DATA *prev;

		for (prev = room->affected; prev != nullptr; prev = prev->next)
		{
			if (prev->next == paf)
			{
				prev->next = paf->next;
				break;
			}
		}

		if (prev == nullptr)
		{
			bug("Affect_remove_room: cannot find paf.", 0);
			return;
		}
	}

	if (!room->affected)
	{
		ROOM_INDEX_DATA *prev;

		if (top_affected_room == room)
		{
			top_affected_room = room->aff_next;
		}
		else
		{
			for (prev = top_affected_room; prev->aff_next; prev = prev->aff_next)
			{
				if (prev->aff_next == room)
				{
					prev->aff_next = room->aff_next;
					break;
				}
			}

			if (prev == nullptr)
			{
				bug("Affect_remove_room: cannot find room.", 0);
				return;
			}
		}

		room->aff_next = nullptr;
	}

	free_affect_room(paf);

	affect_check_room(room, where, vector);
}

/*
 * Strip all affects of a given sn.
 */
void affect_strip_room(ROOM_INDEX_DATA *room, int sn)
{
	ROOM_AFFECT_DATA *paf;
	ROOM_AFFECT_DATA *paf_next;

	for (paf = room->affected; paf != nullptr; paf = paf_next)
	{
		paf_next = paf->next;

		if (paf->type == sn)
			affect_remove_room(room, paf);
	}
}

/*
 * Return true if a room is affected by a spell.
 */
bool is_affected_room(ROOM_INDEX_DATA *room, int sn)
{
	ROOM_AFFECT_DATA *paf;

	if (!room)
		return false;

	for (paf = room->affected; paf != nullptr; paf = paf->next)
	{
		if (paf->type == sn)
			return true;
	}

	return false;
}

/*
 * Add or enhance an affect.
 */
void affect_join_room(ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *paf)
{
	ROOM_AFFECT_DATA *paf_old;

	for (paf_old = room->affected; paf_old != nullptr; paf_old = paf_old->next)
	{
		if (paf_old->type == paf->type)
		{
			paf->level = (paf->level + paf_old->level) / 2;
			paf->duration += paf_old->duration;
			paf->modifier += paf_old->modifier;
			affect_remove_room(room, paf_old);
			break;
		}
	}

	affect_to_room(room, paf);
}

/*
 * Return ascii name of an raffect location.
 */
char *raffect_loc_name(int location)
{
	switch (location)
	{
		case APPLY_ROOM_NONE:
			return "none";
		case APPLY_ROOM_HEAL:
			return "heal rate";
		case APPLY_ROOM_MANA:
			return "mana rate";
		case APPLY_ROOM_SECT:
			return "sector";
		case APPLY_ROOM_NOPE:
			return "nope";
	}

	bug("raffect_location_name: unknown location %d.", location);
	return "(unknown)";
}

/*
 * Return ascii name of an affect bit vector.
 */
char *raffect_bit_name(long vector[])
{
	static char buf[512];

	buf[0] = '\0';

	if (IS_SET(vector, AFF_ROOM_CURSE))
		strcat(buf, " curse");

	if (IS_SET(vector, AFF_ROOM_POISON))
		strcat(buf, " poison");

	if (IS_SET(vector, AFF_ROOM_PLAGUE))
		strcat(buf, " plague");

	if (IS_SET(vector, AFF_ROOM_SLEEP))
		strcat(buf, " sleep");

	if (IS_SET(vector, AFF_ROOM_SLOW))
		strcat(buf, " slow");

	return (buf[0] != '\0') ? buf + 1 : (char *)"none";
}

void charaff_to_obj(OBJ_DATA *obj, AFFECT_DATA *paf)
{
	AFFECT_DATA *paf_new;

	paf_new = new_affect();
	*paf_new = *paf;
	paf_new->next = obj->charaffs;
	obj->charaffs = paf_new;
}

void charaff_to_obj_index(OBJ_INDEX_DATA *obj, AFFECT_DATA *paf)
{
	AFFECT_DATA *paf_new;

	paf_new = new_affect();
	*paf_new = *paf;
	paf_new->next = obj->charaffs;
	obj->charaffs = paf_new;
}

void charaff_from_obj_index(OBJ_INDEX_DATA *obj, AFFECT_DATA *paf)
{
	if (obj->charaffs == nullptr)
	{
		bug("charaff_from_obj_index: no affect.", 0);
		return;
	}

	if (paf == obj->charaffs)
	{
		obj->charaffs = paf->next;
	}
	else
	{
		AFFECT_DATA *prev;
		for (prev = obj->charaffs; prev != nullptr; prev = prev->next)
		{
			if (prev->next == paf)
			{
				prev->next = paf->next;
				break;
			}
		}

		if (prev == nullptr)
		{
			bug("charaff_from_obj_index: cannot find paf.", 0);
			return;
		}
	}

	free_affect(paf);
}

/*
 * Apply or remove an affect to an object.
 */

void init_affect_obj(OBJ_AFFECT_DATA *paf)
{
	paf->where = TO_OBJ_AFFECTS;
	paf->type = 0;
	paf->aftype = AFT_SPELL;
	paf->level = 0;
	paf->duration = 0;
	paf->location = 0;
	paf->modifier = 0;
	zero_vector(paf->bitvector);
	paf->tick_fun = nullptr;
	paf->pulse_fun = nullptr;
	paf->end_fun = nullptr;
	paf->owner = nullptr;
}

void affect_modify_obj(OBJ_DATA *obj, OBJ_AFFECT_DATA *paf, bool fAdd)
{
	CHAR_DATA *ch;
	int mod, wear;
	OBJ_APPLY_DATA *app, *p_app;

	mod = paf->modifier;

	if (fAdd)
	{
		switch (paf->where)
		{
			case TO_OBJ_AFFECTS:
				BITWISE_OR(obj->affected_by, paf->bitvector);
				break;
			case TO_OBJ_APPLY:
				ch = obj->carried_by;
				wear = obj->wear_loc;

				if (ch && (wear != WEAR_NONE))
					unequip_char(ch, obj, false);

				app = new_apply_data();
				app->location = paf->location;
				app->modifier = paf->modifier;
				app->type = paf->type;
				app->next = obj->apply;
				obj->apply = app;

				if (ch && (wear != WEAR_NONE))
					equip_char(ch, obj, wear, false);

				break;
		}
	}
	else
	{
		switch (paf->where)
		{
		case TO_OBJ_AFFECTS:
			BITWISE_XAND(obj->affected_by, paf->bitvector);
			break;
		case TO_OBJ_APPLY:
			ch = obj->carried_by;
			wear = obj->wear_loc;

			if (ch && (wear != WEAR_NONE))
				unequip_char(ch, obj, false);

			for (app = obj->apply; app; app = app->next)
			{
				if (app->type == paf->type && app->location == paf->location)
					break;
			}

			if (!app)
			{
				if (ch && (wear != WEAR_NONE))
					equip_char(ch, obj, wear, false);

				break;
			}
			if (obj->apply != app)
			{
				for (p_app = obj->apply; p_app; p_app = p_app->next)
				{
					if (p_app->next == app)
						break;
				}

				p_app->next = app->next;
				free_apply(app);
			}
			else
			{
				obj->apply = app->next;
				free_apply(app);
			}

			if (ch && (wear != WEAR_NONE))
				equip_char(ch, obj, wear, false);

			break;
		}

		mod = 0 - mod;
	}

	/* TO_OBJ_APPLY, as seen above, lets an item affect modify the apply
	 * portion of the object.  For example, a rust spell cast on a target
	 * could cause some of the target's metal eq to rust temporarily, giving
	 * them a -dex apply for the duration.  This should properly add and
	 * remove applies due to affects.
	 */

	if (paf->where == TO_OBJ_AFFECTS)
	{
		switch (paf->location)
		{
			case APPLY_OBJ_NONE:
				break;
			case APPLY_OBJ_V0:
				obj->value[0] += mod;
				break;
			case APPLY_OBJ_V1:
				obj->value[1] += mod;
				break;
			case APPLY_OBJ_V2:
				obj->value[2] += mod;
				break;
			case APPLY_OBJ_V3:
				obj->value[3] += mod;
				break;
			case APPLY_OBJ_V4:
				obj->value[4] += mod;
				break;
			case APPLY_OBJ_WEIGHT:
				obj->weight += mod;
				break;
			default:
				bug("affect_modify_obj: unknown location %d.", paf->location);
		}
	}
}

void affect_to_obj(OBJ_DATA *obj, OBJ_AFFECT_DATA *paf)
{
	OBJ_AFFECT_DATA *paf_new;

	paf_new = new_affect_obj();

	*paf_new = *paf;
	paf_new->next = obj->affected;
	obj->affected = paf_new;

	affect_modify_obj(obj, paf_new, true);
}

void affect_check_obj(OBJ_DATA *obj, int where, long vector[])
{
	OBJ_AFFECT_DATA *paf;

	if (IS_ZERO_VECTOR(vector))
		return;

	for (paf = obj->affected; paf != nullptr; paf = paf->next)
	{
		if (paf->where == where && vector_equal(paf->bitvector, vector))
		{
			switch (where)
			{
				case TO_OBJ_AFFECTS:
					BITWISE_OR(obj->affected_by, vector);
					break;
			}

			return;
		}
	}
}

void affect_remove_obj(OBJ_DATA *obj, OBJ_AFFECT_DATA *paf, bool show)
{
	int where;
	long vector[MAX_BITVECTOR];

	if (!obj->affected)
	{
		bug("affect_remove_obj: no affect!", 0);
		return;
	}

	if (paf->end_fun != nullptr && show)
		(*paf->end_fun)(obj, paf);

	affect_modify_obj(obj, paf, false);
	where = paf->where;
	copy_vector(vector, paf->bitvector);

	if (paf == obj->affected)
	{
		obj->affected = paf->next;
	}
	else
	{
		OBJ_AFFECT_DATA *prev;

		for (prev = obj->affected; prev != nullptr; prev = prev->next)
		{
			if (prev->next == paf)
			{
				prev->next = paf->next;
				break;
			}
		}

		if (!prev)
		{
			bug("affect_remove_obj: cannot find paf on obj #%d!", obj->pIndexData->vnum);
			return;
		}
	}

	free_affect_obj(paf);
	affect_check_obj(obj, where, vector);
}

void affect_strip_obj(OBJ_DATA *obj, int sn)
{
	OBJ_AFFECT_DATA *paf;
	OBJ_AFFECT_DATA *paf_next;

	for (paf = obj->affected; paf != nullptr; paf = paf_next)
	{
		paf_next = paf->next;

		if (paf->type == sn)
			affect_remove_obj(obj, paf, true);
	}
}

bool is_affected_obj(OBJ_DATA *obj, int sn)
{
	OBJ_AFFECT_DATA *paf;

	for (paf = obj->affected; paf != nullptr; paf = paf->next)
	{
		if (paf->type == sn)
			return true;
	}

	return false;
}

void affect_join_obj(OBJ_DATA *obj, OBJ_AFFECT_DATA *paf)
{
	OBJ_AFFECT_DATA *paf_old;

	for (paf_old = obj->affected; paf_old != nullptr; paf_old = paf_old->next)
	{
		if (paf_old->type == paf->type)
		{
			paf->level = (paf->level + paf_old->level) / 2;
			paf->duration += paf_old->duration;
			paf->modifier += paf_old->modifier;
			affect_remove_obj(obj, paf_old, true);
			break;
		}
	}

	affect_to_obj(obj, paf);
}

char *oaffect_loc_name(int location)
{
	switch (location)
	{
		case APPLY_OBJ_NONE:
			return "none";
		case APPLY_OBJ_V0:
			return "value0";
		case APPLY_OBJ_V1:
			return "value1";
		case APPLY_OBJ_V2:
			return "value2";
		case APPLY_OBJ_V3:
			return "value3";
		case APPLY_OBJ_V4:
			return "value4";
		case APPLY_OBJ_WEIGHT:
			return "weight";
	}

	bug("oaffect_loc_name: unknown  location %d.", location);
	return "(unknown)";
}

char *oaffect_bit_name(long vector[])
{
	static char buf[512];

	buf[0] = '\0';

	if (IS_SET(vector, AFF_OBJ_BURNING))
		strcat(buf, " burning");

	return (buf[0] != '\0') ? buf + 1 : (char *)"none";
}

/*
 * Apply or remove an affect to an area.
 */

void init_affect_area(AREA_AFFECT_DATA *paf)
{
	paf->where = TO_AREA_AFFECTS;
	paf->type = 0;
	paf->aftype = AFT_SPELL;
	paf->level = 0;
	paf->duration = 0;
	paf->location = 0;
	paf->modifier = 0;
	zero_vector(paf->bitvector);
	paf->pulse_fun = nullptr;
	paf->tick_fun = nullptr;
	paf->end_fun = nullptr;
	paf->owner = nullptr;
}

void affect_modify_area(AREA_DATA *area, AREA_AFFECT_DATA *paf, bool fAdd)
{
	int mod;

	mod = paf->modifier;

	if (fAdd)
	{
		switch (paf->where)
		{
			case TO_AREA_AFFECTS:
				BITWISE_OR(area->affected_by, paf->bitvector);
				break;
		}
	}
	else
	{
		switch (paf->where)
		{
			case TO_AREA_AFFECTS:
				BITWISE_XAND(area->affected_by, paf->bitvector);
				break;
		}

		mod = 0 - mod;
	}

	switch (paf->location)
	{
		case APPLY_AREA_NONE:
			break;
		case APPLY_AREA_TEMP:
			area->temp += paf->modifier;

			if (area->temp >= Temperature::MaxTemperature)
				area->temp = Temperature::MaxTemperature - 1;

			break;
		case APPLY_AREA_WIND:
			area->wind += paf->modifier;

			if (area->wind >= Windspeed::MaxWindspeed)
				area->wind = Windspeed::MaxWindspeed - 1;

			break;
		case APPLY_AREA_SKY:
			area->sky += paf->modifier;

			if (area->sky >= WeatherCondition::MaxWeatherCondition)
				area->sky = WeatherCondition::MaxWeatherCondition - 1;

			break;
		default:
			bug("affect_modify_area: unknown location %d.", paf->location);
	}
}

void affect_to_area(AREA_DATA *area, AREA_AFFECT_DATA *paf)
{
	AREA_AFFECT_DATA *paf_new;

	paf_new = new_affect_area();

	*paf_new = *paf;
	paf_new->next = area->affected;
	area->affected = paf_new;

	affect_modify_area(area, paf_new, true);
}

void affect_check_area(AREA_DATA *area, int where, long vector[])
{
	AREA_AFFECT_DATA *paf;

	if (vector == 0)
		return;

	for (paf = area->affected; paf != nullptr; paf = paf->next)
	{
		if (paf->where == where && vector_equal(paf->bitvector, vector))
		{
			switch (where)
			{
				case TO_AREA_AFFECTS:
					BITWISE_OR(area->affected_by, vector);
					break;
			}

			return;
		}
	}
}

void affect_remove_area(AREA_DATA *area, AREA_AFFECT_DATA *paf)
{
	int where;
	long vector[MAX_BITVECTOR];

	if (!area->affected)
	{
		bug("affect_remove_area: no affect!", 0);
		return;
	}

	if (paf->end_fun != nullptr)
		(*paf->end_fun)(area, paf);

	affect_modify_area(area, paf, false);
	where = paf->where;
	copy_vector(vector, paf->bitvector);

	if (paf == area->affected)
	{
		area->affected = paf->next;
	}
	else
	{
		AREA_AFFECT_DATA *prev;

		for (prev = area->affected; prev != nullptr; prev = prev->next)
		{
			if (prev->next == paf)
			{
				prev->next = paf->next;
				break;
			}
		}

		if (!prev)
		{
			bug("affect_remove_area: cannot find paf!", 0);
			return;
		}
	}

	free_affect_area(paf);

	affect_check_area(area, where, vector);
}

void affect_strip_area(AREA_DATA *area, int sn)
{
	AREA_AFFECT_DATA *paf;
	AREA_AFFECT_DATA *paf_next;

	for (paf = area->affected; paf != nullptr; paf = paf_next)
	{
		paf_next = paf->next;

		if (paf->type == sn)
			affect_remove_area(area, paf);
	}
}

bool is_affected_area(AREA_DATA *area, int sn)
{
	if (area == nullptr)
		return false;

	for (auto paf = area->affected; paf != nullptr; paf = paf->next)
	{
		if (paf->type == sn)
			return true;
	}

	return false;
}

void affect_join_area(AREA_DATA *area, AREA_AFFECT_DATA *paf)
{
	AREA_AFFECT_DATA *paf_old;

	for (paf_old = area->affected; paf_old != nullptr; paf_old = paf_old->next)
	{
		if (paf_old->type == paf->type)
		{
			paf->level = (paf->level + paf_old->level) / 2;
			paf->duration += paf_old->duration;
			paf->modifier += paf_old->modifier;
			affect_remove_area(area, paf_old);
			break;
		}
	}

	affect_to_area(area, paf);
}

char *aaffect_loc_name(int location)
{
	switch (location)
	{
		case APPLY_AREA_NONE:
			return "none";
		case APPLY_AREA_SKY:
			return "sky";
		case APPLY_AREA_TEMP:
			return "temperature";
		case APPLY_AREA_WIND:
			return "wind";
	}

	bug("aaffect_loc_name: unknown  location %d.", location);
	return "(unknown)";
}

bool is_safe_rspell_nom(int level, CHAR_DATA *victim)
{
	/* ghosts are safe */
	if (!is_npc(victim) && (victim->ghost))
		return true;

	if (victim->level < 5 && !is_npc(victim))
		return true;

	if (!is_npc(victim) && ((level >= victim->level + 10) || (victim->level >= level + 10)))
		return true;

	return false;
}

bool is_safe_rspell(int level, CHAR_DATA *victim)
{
	if (is_safe_rspell_nom(level, victim))
	{
		act("The gods protect $n.", victim, nullptr, nullptr, TO_CHAR);
		act("The gods protect $n from the spell of room.", victim, nullptr, nullptr, TO_ROOM);

		return true;
	}
	else
	{
		return false;
	}
}

/*
 * Return ascii name of an affect bit vector.
 */
char *flag_room_name(int vector)
{
	static char buf[512];

	buf[0] = '\0';

	if (vector & ROOM_DARK)
		strcat(buf, " dark");

	if (vector & ROOM_NO_MOB)
		strcat(buf, " nomob");

	if (vector & ROOM_INDOORS)
		strcat(buf, " indoors");

	if (vector & ROOM_PRIVATE)
		strcat(buf, " private");

	if (vector & ROOM_SAFE)
		strcat(buf, " safe");

	if (vector & ROOM_SOLITARY)
		strcat(buf, " solitary");

	if (vector & ROOM_PET_SHOP)
		strcat(buf, " petshop");

	if (vector & ROOM_NO_RECALL)
		strcat(buf, " norecall");

	if (vector & ROOM_IMP_ONLY)
		strcat(buf, " imp_only");

	if (vector & ROOM_GODS_ONLY)
		strcat(buf, " god_only");

	if (vector & ROOM_HEROES_ONLY)
		strcat(buf, " heroes");

	if (vector & ROOM_NEWBIES_ONLY)
		strcat(buf, " newbies");

	if (vector & ROOM_LAW)
		strcat(buf, " law");

	if (vector & ROOM_NOWHERE)
		strcat(buf, " nowhere");

	if (vector & ROOM_NO_MAGIC)
		strcat(buf, " nomagic");

	return (buf[0] != '\0') ? buf + 1 : (char *)"none";
}

void modify_location(CHAR_DATA *ch, int location, int mod, bool add)
{
	int i;
	if (!add)
		mod = 0 - mod;

	switch (location)
	{
		case APPLY_NONE:
			break;
		case APPLY_STR:
			ch->mod_stat[STAT_STR] += mod;
			break;
		case APPLY_DEX:
			ch->mod_stat[STAT_DEX] += mod;
			break;
		case APPLY_INT:
			ch->mod_stat[STAT_INT] += mod;
			break;
		case APPLY_WIS:
			ch->mod_stat[STAT_WIS] += mod;
			break;
		case APPLY_CON:
			ch->mod_stat[STAT_CON] += mod;
			break;
		case APPLY_SEX:
			ch->sex += mod;
			break;
		case APPLY_CLASS:
			break;
		case APPLY_AGE:
			break;
		case APPLY_HEIGHT:
			break;
		case APPLY_WEIGHT:
			break;
		case APPLY_MANA:
			ch->max_mana += mod;
			break;
		case APPLY_HIT:
			ch->max_hit += mod;
			break;
		case APPLY_MOVE:
			ch->max_move += mod;
			break;
		case APPLY_GOLD:
			break;
		case APPLY_EXP:
			break;
		case APPLY_AC:
			for (i = 0; i < 4; i++)
				ch->armor[i] += mod;
			break;
		case APPLY_HITROLL:
			ch->hitroll += mod;
			break;
		case APPLY_DAMROLL:
			ch->damroll += mod;
			break;
		case APPLY_SAVES:
			ch->saving_throw += mod;
			break;
		case APPLY_SAVING_PARA:
			ch->saving_throw += mod;
			break;
		case APPLY_SAVING_ROD:
			ch->saving_throw += mod;
			break;
		case APPLY_SAVING_PETRI:
			ch->saving_throw += mod;
			break;
		case APPLY_SAVING_BREATH:
			ch->saving_throw += mod;
			break;
		case APPLY_SAVING_SPELL:
			ch->saving_throw += mod;
			break;
		case APPLY_SPELL_AFFECT:
			break;
		case APPLY_CARRY_WEIGHT:
			ch->carry_weight += mod;
			break;
		case APPLY_DEFENSE:
			ch->defense_mod += mod;
			break;
		case APPLY_SIZE:
			ch->size += mod;
			break;
		case APPLY_REGENERATION:
			ch->regen_rate += mod;
			break;
		case APPLY_ENERGYSTATE:
			if (!is_npc(ch))
				ch->pcdata->energy_state += mod;
			break;
		case APPLY_ARMS:
			ch->arms += mod;
			break;
		case APPLY_LEGS:
			ch->legs += mod;
			break;
		case APPLY_ALIGNMENT:
			ch->alignment += mod;
			break;
		case APPLY_ETHOS:
			if (!is_npc(ch))
				ch->pcdata->ethos += mod;
			break;
		case APPLY_BEAUTY:
			if (ch->pcdata->beauty + mod > MAX_BEAUTY)
				ch->pcdata->beauty = MAX_BEAUTY;
			else
				ch->pcdata->beauty += mod;
			break;
		case APPLY_DAM_MOD:
			if (add)
				ch->dam_mod *= (1.00f + (float)mod / 100.00f);
			else
			{
				mod = 0 - mod;
				ch->dam_mod *= 1.00f / (1.00f + (float)mod / 100.00f);
			}
			break;
	}
}

int get_align(CHAR_DATA *ch)
{
	return ch->alignment;
}

int get_ethos(CHAR_DATA *ch)
{
	if (!is_npc(ch))
		return ch->pcdata->ethos;

	return 0;
}

int damage_queue(CHAR_DATA *ch, CHAR_DATA *victim, int dam, int damtype, bool blockable, int add, int mult, char *dnoun)
{
	return damage_new(ch, victim, dam, gsn_bash, damtype, true, blockable, add, mult, dnoun);
}
