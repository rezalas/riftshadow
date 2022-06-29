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
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include "moremagic.h"
#include "weather_enums.h"
#include "recycle.h"
#include "tables.h"
#include "lookup.h"
#include "magic.h"
#include "act_move.h"
#include "comm.h"
#include "const.h"
#include "utility.h"
#include "dioextra.h"
#include "fight.h"
#include "devextra.h"
#include "act_info.h"
#include "act_comm.h"
#include "skills.h"
#include "db.h"

#define LOGIN_LOG_FILE			"logins.txt"
//#define TEMP_GREP_RESULTS_TWO	"../temp/tempgrepresults2.tmp"


void spell_enlarge(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;
	char buf[MSL];

	if (is_affected(victim, gsn_enlarge) || victim->size >= SIZE_IMMENSE)
	{
		if (victim != ch)
			send_to_char("They are already enlarged.\n\r", ch);

		if (victim == ch)
			send_to_char("You are already enlarged.\n\r", ch);

		return;
	}

	if (!trusts(ch, victim) && is_safe(ch, victim))
		return;

	if (is_affected(victim, gsn_reduce))
	{
		send_to_char("You grow to your normal size.\n\r", ch);
		act("$n grows to $s normal size!", victim, 0, ch, TO_ROOM);
		affect_strip(victim, gsn_reduce);
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = gsn_enlarge;
	af.level = level;
	af.duration = level / 8;
	af.location = APPLY_SIZE;
	af.modifier = 1;
	affect_to_char(victim, &af);

	af.aftype = AFT_INVIS;
	af.location = APPLY_DAM_MOD;
	af.modifier = 20;
	affect_to_char(victim, &af);

	send_to_char("Your entire body and gear rapidly grow in size, but you feel somewhat more frail afterward.\n\r", victim);
	act("$n suddenly swells in all directions, expanding to half again $s normal size!", victim, 0, 0, TO_ROOM);

	if (!is_npc(victim) && !trusts(ch, victim) && (!ch->fighting || !victim->fighting))
	{
		sprintf(buf, "Die, %s, you sorcerous dog!", pers(ch, victim));
		do_myell(victim, buf, ch);
		multi_hit(victim, ch, TYPE_UNDEFINED);
	}
}

void spell_sunray(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	int dam = 0;
	AFFECT_DATA af;

	if (ch->in_room->sector_type == SECT_INSIDE)
	{
		send_to_char("You can't see the sun to use it to burn people.\n\r", ch);
		return;
	}

	if (sun == SolarPosition::Dark)
		dam = 65;

	if (sun == SolarPosition::Sunrise)
		dam = 110;

	if (sun == SolarPosition::Daylight)
		dam = 150;

	if (sun == SolarPosition::Sunset)
		dam = 100;

	if (ch->in_room->area->sky == WeatherCondition::Clear)
		dam = (int)((float)dam * 1.7);

	if (ch->in_room->area->sky == WeatherCondition::PartlyCloudy)
		dam *= 1;

	if (ch->in_room->area->sky == WeatherCondition::Drizzle)
		dam = (int)((float)dam * .8);

	if (ch->in_room->area->sky == WeatherCondition::ThunderStorm)
		dam *= 1;

	// Fuzz it up.
	act("$n burns $N with the light of the sun!", ch, 0, victim, TO_NOTVICT);
	act("$n burns you with the light of the sun!", ch, 0, victim, TO_VICT);
	act("You burn $N with the light of the sun!", ch, 0, victim, TO_CHAR);

	if (dam > 120 && number_percent() > 80)
	{
		act("$n appears to be blinded by the sun in $s eyes!", victim, 0, 0, TO_ROOM);
		act("You are blinded by the sun in your eyes!", victim, 0, 0, TO_CHAR);

		init_affect(&af);
		af.where = TO_AFFECTS;
		af.type = sn;
		af.aftype = AFT_COMMUNE;
		af.duration = level / 12;
		af.location = APPLY_HITROLL;
		af.modifier = -6;
		af.level = level;

		SET_BIT(af.bitvector, AFF_BLIND);

		af.mod_name = MOD_VISION;
		affect_to_char(victim, &af);
	}

	damage_old(ch, victim, dam, sn, DAM_LIGHT, true);
}

void spell_cleanse(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;

	// Difficulty modifier: 1 easy 2 medium 3 hard 4 very hard
	sn = skill_lookup("blindness");

	if (is_affected(victim, sn) && cleansed(ch, victim, 1, sn))
	{
		if (victim != ch)
			act("$n is no longer blinded.", victim, 0, ch, TO_ROOM);

		send_to_char(skill_table[sn].msg_off, victim);
		send_to_char("\n\r", victim);
		affect_strip(victim, sn);
	}

	sn = skill_lookup("poison");

	if (is_affected(victim, sn) && cleansed(ch, victim, 2, sn))
	{
		if (victim != ch)
			act("$n looks less ill.", victim, 0, ch, TO_ROOM);

		send_to_char(skill_table[sn].msg_off, victim);
		send_to_char("\n\r", victim);
		affect_strip(victim, sn);
	}

	sn = skill_lookup("weaken");

	if (is_affected(victim, sn) && cleansed(ch, victim, 1, sn))
	{
		if (victim != ch)
			act("$n looks stronger.", victim, 0, ch, TO_ROOM);

		send_to_char(skill_table[sn].msg_off, victim);
		send_to_char("\n\r", victim);
		affect_strip(victim, sn);
	}

	sn = skill_lookup("plague");

	if (is_affected(victim, sn) && cleansed(ch, victim, 2, sn))
	{
		if (victim != ch)
			act("$n looks less diseased.", victim, 0, ch, TO_ROOM);
		send_to_char(skill_table[sn].msg_off, victim);
		send_to_char("\n\r", victim);
		affect_strip(victim, sn);
	}

	sn = skill_lookup("slow");

	if (is_affected(victim, sn) && cleansed(ch, victim, 2, sn))
	{
		if (victim != ch)
			act("$n starts moving at normal speed again.", victim, 0, ch, TO_ROOM);

		send_to_char(skill_table[sn].msg_off, victim);
		send_to_char("\n\r", victim);
		affect_strip(victim, sn);
	}

	sn = skill_lookup("forget");

	if (is_affected(victim, sn) && cleansed(ch, victim, 4, sn))
	{
		if (victim != ch)
			act("$n looks like $e's just had a rush of memory.", victim, 0, ch, TO_ROOM);
		send_to_char(skill_table[sn].msg_off, victim);
		send_to_char("\n\r", victim);
		affect_strip(victim, sn);
	}

	sn = skill_lookup("rot");

	if (is_affected(victim, sn) && cleansed(ch, victim, 3, sn))
	{
		if (victim != ch)
			act("$n's body stops wasting away.", victim, 0, ch, TO_ROOM);

		send_to_char(skill_table[sn].msg_off, victim);
		send_to_char("\n\r", victim);
		affect_strip(victim, sn);
	}

	sn = skill_lookup("curse");

	if (is_affected(victim, sn) && cleansed(ch, victim, 2, sn))
	{
		if (victim != ch)
			act("$n's curse wears off.", victim, 0, ch, TO_ROOM);

		send_to_char(skill_table[sn].msg_off, victim);
		send_to_char("\n\r", victim);
		affect_strip(victim, sn);
	}

	sn = skill_lookup("impale");

	if (is_affected(victim, sn) && cleansed(ch, victim, 4, sn))
	{
		if (victim != ch)
			act("$n stops bleeding.", victim, 0, ch, TO_ROOM);

		send_to_char(skill_table[sn].msg_off, victim);
		send_to_char("\n\r", victim);
		affect_strip(victim, sn);
	}

	check_improve(ch, skill_lookup("cleanse"), true, 1);
}

bool cleansed(CHAR_DATA *ch, CHAR_DATA *victim, int diffmodifier, int sn)
{
	int chance = 0, snlevel;

	chance = 100 / diffmodifier;
	snlevel = get_affect_level(victim, sn);

	if (snlevel < 0)
		return false;

	chance -= (snlevel - ch->level) * 2;

	if (number_percent() > chance)
		return false;

	return true;
}

int get_affect_level(CHAR_DATA *ch, int sn)
{
	// Go through all affects on ch and return the level of the one that matches sn.
	for (AFFECT_DATA *paf = ch->affected; paf != nullptr; paf = paf->next)
	{
		if (paf->type == sn)
			return paf->level;
	}

	return -1;
}

void spell_cure_deafness(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;

	if (!is_affected(victim, gsn_deafen))
	{
		if (victim == ch)
			send_to_char("You aren't deaf.\n\r", ch);
		else
			act("$N doesn't appear to be deafened.", ch, nullptr, victim, TO_CHAR);

		return;
	}

	if (check_dispel(level, victim, gsn_deafen))
	{
		send_to_char("Your hearing returns!\n\r", victim);
		act("$n is no longer deafened.", victim, nullptr, nullptr, TO_ROOM);
	}
	else
	{
		send_to_char("You failed.\n\r", ch);
	}
}

void spell_remove_paralysis(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;

	if (!is_affected(victim, skill_lookup("hold person")))
	{
		if (victim == ch)
			send_to_char("You aren't paralyzed.\n\r", ch);
		else
			act("$N doesn't appear to be paralyzed.", ch, nullptr, victim, TO_CHAR);

		return;
	}

	if (check_dispel(level, victim, skill_lookup("hold person")))
	{
		send_to_char("The feeling in your body returns!\n\r", victim);
		act("$n is no longer paraylzed.", victim, nullptr, nullptr, TO_ROOM);
	}
	else
	{
		send_to_char("You failed.\n\r", ch);
	}
}

void spell_awaken(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA *laf;

	if (is_awake(victim))
	{
		act("$N is already awake.", ch, nullptr, victim, TO_CHAR);
		return;
	}

	for (laf = victim->affected; laf != nullptr; laf = laf->next)
	{
		if (IS_SET(laf->bitvector, AFF_SLEEP))
			break;
	}

	if (laf == nullptr)
	{
		send_to_char("They don't seem to be in deep sleep.\n\r", ch);
		return;
	}

	if (check_dispel(level, victim, laf->type))
	{
		send_to_char("You suddenly awaken.\n\r", victim);
		act("$n suddenly awakens.", victim, nullptr, nullptr, TO_ROOM);
		do_stand(victim, "");
	}
	else
	{
		send_to_char("You failed.\n\r", ch);
	}
}

void spell_resist_heat(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected(victim, sn))
	{
		if (victim == ch)
			send_to_char("You are already resisting heat.\n\r", ch);
		else
			act("$E is already resisting heat.", ch, nullptr, victim, TO_CHAR);

		return;
	}

	send_to_char("You feel resistant to heat.\n\r", victim);
	act("$n is resistant to heat.", victim, nullptr, nullptr, TO_ROOM);

	init_affect(&af);
	af.where = TO_RESIST;
	af.aftype = AFT_COMMUNE;
	af.type = sn;
	af.modifier = 0;
	af.level = level;
	af.duration = 4 + ch->level / 5;

	SET_BIT(af.bitvector, RES_FIRE);

	af.mod_name = MOD_RESISTANCE;
	af.location = 0;
	affect_to_char(victim, &af);
}

void spell_resist_cold(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected(victim, sn))
	{
		if (victim == ch)
			send_to_char("You are already resisting cold.\n\r", ch);
		else
			act("$E is already resisting cold.", ch, nullptr, victim, TO_CHAR);

		return;
	}

	send_to_char("You feel resistant to cold.\n\r", victim);
	act("$n is resistant to cold.", victim, nullptr, nullptr, TO_ROOM);

	init_affect(&af);
	af.where = TO_RESIST;
	af.aftype = AFT_COMMUNE;
	af.type = sn;
	af.modifier = 0;
	af.level = level;
	af.duration = 4 + ch->level / 5;

	SET_BIT(af.bitvector, RES_COLD);

	af.location = 0;
	af.mod_name = MOD_RESISTANCE;
	affect_to_char(victim, &af);
}

void spell_resist_lightning(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected(victim, sn))
	{
		if (victim == ch)
			send_to_char("You are already resisting lightning.\n\r", ch);
		else
			act("$E is already resisting lightning.", ch, nullptr, victim, TO_CHAR);

		return;
	}

	send_to_char("You feel resistant to lightning.\n\r", victim);
	act("$n is resistant to lightning.", victim, nullptr, nullptr, TO_ROOM);

	init_affect(&af);
	af.where = TO_RESIST;
	af.aftype = AFT_COMMUNE;
	af.type = sn;
	af.modifier = 0;
	af.level = level;
	af.duration = 4 + ch->level / 5;

	SET_BIT(af.bitvector, RES_LIGHTNING);

	af.location = 0;
	af.mod_name = MOD_RESISTANCE;
	affect_to_char(victim, &af);
}

void spell_resist_mental(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected(victim, sn))
	{
		if (victim == ch)
			send_to_char("You are already resisting mental attacks.\n\r", ch);
		else
			act("$E is already resisting mental attacks.", ch, nullptr, victim, TO_CHAR);

		return;
	}

	send_to_char("You feel resistant to mental attacks.\n\r", victim);
	act("$n is resistant to mental attacks.", victim, nullptr, nullptr, TO_ROOM);

	init_affect(&af);
	af.where = TO_RESIST;
	af.aftype = AFT_COMMUNE;
	af.type = sn;
	af.modifier = 0;
	af.level = level;
	af.duration = 4 + ch->level / 5;

	SET_BIT(af.bitvector, RES_MENTAL);

	af.location = 0;
	af.mod_name = MOD_RESISTANCE;
	affect_to_char(victim, &af);
}

void spell_resist_acid(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected(victim, sn))
	{
		if (victim == ch)
			send_to_char("You are already resisting acid.\n\r", ch);
		else
			act("$E is already resisting acid.", ch, nullptr, victim, TO_CHAR);

		return;
	}

	send_to_char("You feel resistant to acid.\n\r", victim);
	act("$n is resistant to acid.", victim, nullptr, nullptr, TO_ROOM);

	init_affect(&af);
	af.where = TO_RESIST;
	af.aftype = AFT_COMMUNE;
	af.type = sn;
	af.modifier = 0;
	af.level = level;
	af.duration = 4 + ch->level / 5;

	SET_BIT(af.bitvector, RES_ACID);

	af.location = 0;
	af.mod_name = MOD_RESISTANCE;
	affect_to_char(victim, &af);
}

void spell_resist_negative(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected(victim, sn))
	{
		if (victim == ch)
			send_to_char("You are already resisting negative damage.\n\r", ch);
		else
			act("$E is already resisting negative damage.", ch, nullptr, victim, TO_CHAR);

		return;
	}

	send_to_char("You feel resistant to negative damage.\n\r", victim);
	act("$n is resistant to negative damage.", victim, nullptr, nullptr, TO_ROOM);

	init_affect(&af);
	af.where = TO_RESIST;
	af.aftype = AFT_COMMUNE;
	af.type = sn;
	af.modifier = 0;
	af.level = level;
	af.duration = 4 + ch->level / 5;

	SET_BIT(af.bitvector, RES_NEGATIVE);

	af.location = 0;
	af.mod_name = MOD_RESISTANCE;
	affect_to_char(victim, &af);
}

void spell_group_teleport(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	ROOM_INDEX_DATA *old_room;
	CHAR_DATA *group;
	CHAR_DATA *g_next;

	old_room = ch->in_room;

	spell_teleport(skill_lookup("teleport"), level, ch, ch, 0);

	if (ch->in_room != old_room)
	{
		for (group = old_room->people; group != nullptr; group = g_next)
		{
			g_next = group->next_in_room;

			if (!is_same_group(group, ch) || (group->fighting != nullptr) || group == ch)
				continue;

			if (group == ch)
				continue;

			send_to_char("You have been teleported!\n\r", group);
			act("$n vanishes!", group, nullptr, nullptr, TO_ROOM);

			char_from_room(group);
			char_to_room(group, ch->in_room);

			act("$n slowly fades into existence.", group, nullptr, nullptr, TO_ROOM);

			un_blade_barrier(group, nullptr);

			do_look(group, "auto");
		}
	}
}

void spell_soften(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected(victim, sn))
	{
		send_to_char("They are already softened.\n\r", ch);

		ch->mana -= 30;

		WAIT_STATE(ch, PULSE_VIOLENCE * 2);

		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = sn;
	af.level = level;
	af.location = APPLY_NONE;
	af.aftype = AFT_SPELL;
	af.modifier = 0;
	af.duration = 1;
	affect_to_char(victim, &af);

	act("$n looks frail.", victim, 0, 0, TO_ROOM);
	send_to_char("You feel more frail.\n\r", victim);
}

void spell_rejuvenate(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	victim->hit = std::min(victim->hit + 200, (int)victim->max_hit);

	update_pos(victim);

	send_to_char("A very warm feeling fills your body.\n\r", victim);

	if (ch != victim)
		send_to_char("Ok.\n\r", ch);
}

void spell_fatigue(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	int dam = dice(level, 4);

	if (saves_spell(level, victim, DAM_MENTAL))
	{
		dam /= 2;
	}
	else
	{
		act("$n looks fatigued.", victim, 0, 0, TO_ROOM);
		act("You feel very fatigued.", victim, 0, 0, TO_CHAR);
		victim->move -= dam / 2;
	}

	damage_new(ch, victim, dam, sn, DAM_MENTAL, true, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, nullptr);
}

void spell_strength(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;

	if (is_affected(ch, sn))
	{
		send_to_char("You are as sure of your faith as you will ever get.\n\r", ch);
		return;
	}

	init_affect(&af);
	af.where = TO_RESIST;
	af.aftype = AFT_COMMUNE;
	af.duration = ch->level * 2;
	af.location = 0;
	af.modifier = 0;

	SET_BIT(af.bitvector, RES_HOLY);

	af.mod_name = MOD_RESISTANCE;
	af.level = level;
	af.type = sn;
	affect_to_char(ch, &af);

	send_to_char("You fast for a period of time, building up your absolute faith in the strength of your Deity.\n\r", ch);
	act("$n meditates for a period of time, building up $s faith in $s Deity.", ch, 0, 0, TO_ROOM);
}

void spell_remove_taint(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	OBJ_DATA *obj = (OBJ_DATA *)vo;
	int chance;

	if (!IS_SET(obj->extra_flags, ITEM_EVIL))
	{
		send_to_char("It is not tainted by evil.\n\r", ch);
		return;
	}

	chance = get_skill(ch, sn) / 2;
	chance += level - obj->level;

	if (obj->level >= MAX_LEVEL)
		chance -= MAX_LEVEL;

	if (number_percent() < chance)
	{
		act("The sickly red aura of $p fades away.", ch, obj, nullptr, TO_ALL);
		REMOVE_BIT(obj->extra_flags, ITEM_EVIL);
	}
	else
	{
		act("$p shivers softly, but the sickly red aura remains.", ch, obj, nullptr, TO_ALL);
	}
}

void spell_worldbind(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected(victim, sn))
	{
		send_to_char("They are already bound to this world.\n\r", ch);
		return;
	}

	if (saves_spell(level - 5, victim, DAM_NEGATIVE))
	{
		act("You failed to bind $N to this world.", ch, 0, victim, TO_CHAR);
		return;
	}

	act("You sever $N's ties to the spiritual world!", ch, 0, victim, TO_CHAR);
	act("$n severs $N's ties to the spiritual world!", ch, 0, victim, TO_NOTVICT);
	act_new("$n severs your ties to the spiritual world!", ch, 0, victim, TO_VICT, POS_DEAD);

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = sn;
	af.aftype = AFT_SPELL;
	af.level = level;
	af.duration = 3;
	af.location = 0;
	af.modifier = 0;
	affect_to_char(victim, &af);
}

void waterbreath_end(CHAR_DATA *ch, AFFECT_DATA *af)
{
	check_waterbreath(ch, ch->in_room);
}

void spell_waterbreath(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	CHAR_DATA *victim = (CHAR_DATA *)vo;

	if (is_affected(victim, sn))
	{
		send_to_char("They can already breathe underwater.\n\r", ch);
		return;
	}

	if (is_affected(victim, gsn_drowning))
	{
		act("You feel relieved as you begin breathing water like you would air.", victim, 0, 0, TO_CHAR);
		act("$n looks relieved as $e begins breathing water like $e would air.", victim, 0, 0, TO_ROOM);
		affect_strip(victim, gsn_drowning);
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = sn;
	af.aftype = get_spell_aftype(ch);
	af.level = level;
	af.duration = level / 5;
	af.location = 0;
	af.modifier = 0;
	af.end_fun = waterbreath_end;

	SET_BIT(af.bitvector, AFF_WATERBREATH);

	affect_to_char(victim, &af);

	send_to_char("You feel more adept at breathing underwater.\n\r", victim);

	if (ch != victim)
		act("You give $N the ability to breathe underwater.", ch, 0, victim, TO_CHAR);
}
