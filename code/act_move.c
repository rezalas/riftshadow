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
#include <math.h>
#include "act_move.h"
#include "handler.h"
#include "magic.h"
#include "recycle.h"
#include "interp.h"
#include "tables.h"
#include "spec.h"
#include "act_comm.h"
#include "cabal.h"
#include "devextra.h"
#include "dioextra.h"
#include "fight.h"
#include "newmem.h"
#include "comm.h"
#include "act_wiz.h"
#include "update.h"
#include "db.h"
#include "act_obj.h"
#include "act_info.h"
#include "characterClasses/sorcerer.h"
#include "characterClasses/chrono.h"
#include "update.h"
#include "chardef.h"
#include "const.h"
#include "utility.h"
#include "effects.h"
#include "room.h"
#include "skills.h"
#include "lookup.h"
#include "characterClasses/thief.h"

char *const dir_name[] =
{
	"north",
	"east",
	"south",
	"west",
	"up",
	"down"
};

const short rev_dir[] =
{
	2,
	3,
	0,
	1,
	5,
	4
};

/* globals */

PATHFIND_DATA *best_path;
int iterations;

void drowning_tick(CHAR_DATA *ch, AFFECT_DATA *af)
{
	if ((is_affected_room(ch->in_room, gsn_airy_water) || ch->in_room->sector_type != SECT_UNDERWATER)
		&& is_affected(ch, gsn_drowning))
	{
		affect_strip(ch, gsn_drowning);
		return;
	}

	if (is_immortal(ch))
		return;

	switch (af->duration)
	{
		case 0:
			act("You drift into unconsciousness, and a sea of blackness swiftly overtakes you.", ch, 0, 0, TO_CHAR);
			raw_kill(ch, ch);
			break;
		case 1:
		case 2:
			act("Gasping involuntarily, you take huge amounts of water into your lungs!", ch, 0, 0, TO_CHAR);
			damage_new(ch, ch, (int)((ch->max_hit / 100) * 75), gsn_drowning, DAM_DROWNING, true, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "drowning");
			break;
		case 3:
			act("Your heart leaps in your chest, protesting the lack of oxygen!", ch, 0, 0, TO_CHAR);
			damage_new(ch, ch, (int)((ch->max_hit / 100) * 25), gsn_drowning, DAM_DROWNING, true, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "asphyxiation");
			break;
		case 4:
			act("Your face grows a deeper shade of red as you struggle to hold your breath!", ch, 0, 0, TO_CHAR);
			act("$n's face grows a deeper shade of red as $e struggles to hold $s breath!", ch, 0, 0, TO_ROOM);
			break;
	}
}

void check_waterbreath(CHAR_DATA *ch, ROOM_INDEX_DATA *to_room)
{
	if (!is_affected(ch, gsn_drowning)
		&& to_room->sector_type == SECT_UNDERWATER
		&& !is_affected_room(to_room, gsn_airy_water)
		&& !is_immortal(ch)
		&& !is_heroimm(ch))
	{
		AFFECT_DATA af;
		init_affect(&af);
		af.where = TO_AFFECTS;
		af.aftype = AFT_INVIS;
		af.type = gsn_drowning;
		af.level = 60;
		af.duration = 5;
		af.tick_fun = drowning_tick;

		new_affect_to_char(ch, &af);

		act("You take a deep breath and hold it tight as you descend into the water.", ch, 0, 0, TO_CHAR);
		act("$n takes a deep breath as $e decends into the water.", ch, 0, 0, TO_ROOM);
	}
	else if (is_affected(ch, gsn_drowning)
		&& (to_room->sector_type != SECT_UNDERWATER || is_affected_room(to_room, gsn_airy_water)))
	{
		affect_strip(ch, gsn_drowning);
	}
}

void move_char(CHAR_DATA *ch, int door, bool automatic, bool fcharm)
{
	AFFECT_DATA *imaf, *paf, cvaf, cvaf2;
	int distance, oppdir, dam, twchance, ss_chance;
	float move, wait;

	if (door < 0 || door > 5)
	{
		bug("Do_move: bad door %d.", door);
		return;
	}

	if (!ch->in_room)
		return;

	auto invnum = ch->in_room->vnum;

	if (is_affected_room(ch->in_room, gsn_smokescreen))
	{
		auto raf = ch->in_room->affected;
		for (; raf != nullptr; raf = raf->next)
		{
			if (raf->type == gsn_smokescreen)
				break;
		}

		if (raf != nullptr)
		{
			if (raf->owner == ch)
			{
				act("The smoke parts for you allowing you passage.", ch, 0, 0, TO_CHAR);
			}
			else if (raf->owner == ch->master && automatic)
			{
				act("You follow $N through the smoke.", ch, 0, ch->master, TO_CHAR);
			}
		}
		else
		{
			act("You stumble around directionless in the smoke.", ch, 0, 0, TO_CHAR);
			door = number_range(0, 5);
		}
	}

	/*
	 * Exit trigger, if activated, bail out. Only PCs are triggered.
	 */

	auto in_room = ch->in_room;

	auto pexit = in_room->exit[door];
	if (pexit == nullptr || pexit->u1.to_room == nullptr || !can_see_room(ch,pexit->u1.to_room))
	{
		send_to_char( "Alas, you cannot go that way.\n\r", ch );
		return;
	}

	auto to_room = pexit->u1.to_room;

	/*
		if(pexit->has_rune == true)
		{
			while((rune = find_rune(pexit, RUNE_TO_PORTAL, RUNE_TRIGGER_EXIT, rune)))
			{
				if(((*rune->function) (rune, ch, &door, nullptr)) == true)
					return;
			}
		}
	* Crashing..*/

	if (is_room_affected(in_room, AFF_ROOM_RANDOMIZER))
	{
		for (auto i = 0; i < 1000; i++)
		{
			auto d0 = number_range(0,5);
			pexit = in_room->exit[d0];
			to_room = pexit->u1.to_room;

			if (pexit == nullptr || to_room == nullptr || !can_see_room(ch,pexit->u1.to_room))
				continue;

			door = d0;
			break;
		}
	}

	if (is_npc(ch) && !is_affected_by(ch, AFF_PASS_DOOR))
	{
		if (IS_SET(pexit->exit_info, EX_LOCKED) && ch->pIndexData->Class()->GetIndex() == CLASS_THIEF)
			do_pick(ch, dir_name[door]);

		if (IS_SET(pexit->exit_info, EX_CLOSED) && IS_SET(ch->parts, PART_HANDS))
			do_open(ch, dir_name[door]);

		if (IS_SET(pexit->exit_info, EX_CLOSED) && ch->size >= 3)
			do_door_bash(ch, dir_name[door]);
	}

	if (IS_SET(pexit->exit_info, EX_CLOSED)
		&& (!is_affected_by(ch, AFF_PASS_DOOR) || IS_SET(pexit->exit_info, EX_NOPASS))
		&& !is_trusted(ch, ANGEL))
	{
		act("Alas, you cannot go that way.", ch, nullptr, pexit->keyword, TO_CHAR);
		return;
	}

	if (is_affected_room(in_room, gsn_glaciate) 
		&& in_room->sector_type == SECT_ICE 
		&& door == Directions::Down 
		&& to_room->sector_type == SECT_UNDERWATER)
	{
		send_to_char("You can't break through the thick layer of ice!\n\r", ch);
		return;
	}

	if (is_affected_by(ch, AFF_CHARM)
		&& ch->master != nullptr
		&& in_room == ch->master->in_room)
	{
		send_to_char("What?  And leave your beloved master?\n\r", ch);
		return;
	}

	if (!is_room_owner(ch, to_room) && room_is_private(to_room))
	{
		send_to_char("That room is private right now.\n\r", ch);
		return;
	}

	if (!is_npc(ch)
		&& isNewbie(ch)
		&& (IS_SET(to_room->room_flags, ROOM_AREA_EXPLORE) || IS_SET(to_room->area->area_flags, AREA_NO_NEWBIES)))
	{
		send_to_char("You are too young to go there.\n\r", ch);
		return;
	}

	if (is_npc(ch) && IS_SET(to_room->room_flags, ROOM_NO_MOB))
		return;

	if (!is_npc(ch) && ch->ghost > 0 && IS_SET(to_room->area->area_flags, AREA_UNGHOST))
		ch->ghost = 0;

	auto found = false, swimmer = false, mountaineer = false;
	if (!is_npc(ch))
	{
		if ((to_room->sector_type == SECT_VERTICAL || to_room->sector_type == SECT_AIR)
			&& !is_affected_by(ch, AFF_FLYING)
			&& !is_immortal(ch))
		{
			send_to_char("You can't fly.\n\r", ch);
			return;
		}

		if (to_room->sector_type == SECT_WATER
			&& (!is_affected_by(ch, AFF_FLYING) && !is_affected_by(ch, AFF_WATERBREATH)))
		{
			for (auto obj = ch->carrying; obj != nullptr; obj = obj->next_content)
			{
				if (obj->item_type == ITEM_BOAT)
					found = true;
			}

			if (!found && /*!is_immortal(ch)) &&*/ !ch->Profs()->HasProf(psn_swimming))
			{
				send_to_char("You need a boat to go there.\n\r", ch);
				return;
			}
			else if (!found) // swimmer
			{
				swimmer = true;
			}
		}

		/* Waterbreath check */
		if ((to_room->sector_type == SECT_UNDERWATER && !is_affected_by(ch, AFF_WATERBREATH))
			|| (in_room->sector_type == SECT_UNDERWATER && to_room->sector_type != SECT_UNDERWATER))
		{
			check_waterbreath(ch, to_room);
		}

		if (check_barred(ch, to_room))
			return;

		if (is_affected(ch, gsn_quicksand_sinking) && is_affected_room(in_room, gsn_quicksand))
		{
			paf = affect_find(ch->affected, gsn_quicksand_sinking);
			if (number_percent() > ((get_curr_stat(ch, STAT_STR) * 4) - 5 * paf->modifier))
			{
				act("You struggle to escape from the quicksand, but fail to pull yourself free!", ch, 0, 0, TO_CHAR);
				act("$n struggles to escape from the quicksand, but fails to pull $mself free!", ch, 0, 0, TO_ROOM);
				WAIT_STATE(ch, 2 * PULSE_VIOLENCE);
				return;
			}

			act("You manage to pull yourself out of the quicksand.", ch, 0, 0, TO_CHAR);
			act("$n manages to pull $mself out of the quicksand.", ch, 0, 0, TO_ROOM);
			affect_strip(ch, gsn_quicksand_sinking);
		}

		if (IS_SET(in_room->progtypes, RPROG_MOVE))
		{
			if (((in_room->rprogs->move_prog)(in_room, ch, door)) == false)
				return;
		}

		for (auto moveprog = ch->in_room->people; moveprog; moveprog = moveprog->next_in_room)
		{
			if (IS_SET(moveprog->progtypes, MPROG_MOVE))
			{
				if (!moveprog->pIndexData->mprogs->move_prog(ch, moveprog, ch->in_room, door))
					return;
			}

			if (TRAPS_MEVENT(moveprog, TRAP_MMOVE))
			{
				if (CALL_MEVENT(moveprog, TRAP_MMOVE, ch, moveprog, door) > 0)
					return;
			}
		}

		move = sect_table[to_room->sector_type].move_cost + sect_table[in_room->sector_type].move_cost;
		move /= 2; /* i.e. the average */

		/* conditional effects */
		if (is_affected_by(ch, AFF_FLYING) || is_affected_by(ch, AFF_HASTE))
			move *= .75;

		if (is_affected_by(ch, AFF_SLOW))
			move *= 2;

		wait = sect_table[to_room->sector_type].wait;
		if (ch->pcdata->energy_state < -2)
			wait += 6;

		if (!is_npc(ch) && is_affected(ch, gsn_rage) && ch->pcdata->tribe == TRIBE_ELK)
		{
			wait = 0;
			move = 1;
		}

		if (is_affected_area(ch->in_room->area, gsn_gravity_well))
		{
			oppdir = reverse_d(door);

			auto gravroom = to_room;

			auto well = object_list;
			for (; well; well = well->next)
			{
				if (well->item_type == ITEM_GRAVITYWELL && well->in_room && well->in_room->area == in_room->area)
					break;
			}

			if (!well)
				return;

			for (auto distance = 0; distance <= get_grav_distance(well); distance++)
			{
				if (!gravroom->exit[oppdir] || !gravroom->exit[oppdir]->u1.to_room)
					break;

				gravroom = gravroom->exit[oppdir]->u1.to_room;

				if (is_affected_room(gravroom, gsn_gravity_well))
					break;
			}

			if (is_affected_room(gravroom, gsn_gravity_well))
			{
				wait = std::max((float)(8 - distance), wait);
				move = (32 * move) / (pow(2, distance));
			}
		}

		if (is_affected(ch, gsn_travelease))
			move /= 3;

		if (is_affected(ch, gsn_decrepify))
		{
			move *= 1.5;
			wait *= 2;
			wait = std::max(wait, (float)3);
		}

		if (is_affected(ch, gsn_creeping_tomb))
		{
			move *= 1.75;
			wait *= 1.75;
		}

		if (is_affected(ch, gsn_hunters_net))
			move *= 3;

		if (ch->legs < 1)
		{
			move *= 4;
			wait *= 4;
		}
		else if (ch->legs < 2)
		{
			move *= 2;
			wait *= 2;
		}

		if (swimmer)
		{
			auto pval = ch->Profs()->GetProf(psn_swimming);
			move *= pval > 2 ? pval > 5 ? .6 : 1.2 : 2;
			wait *= pval > 3 ? pval > 7 ? .5 : 1.2 : 2;
			ch->Profs()->CheckImprove(psn_swimming, 6);
		}

		if ((to_room->sector_type == SECT_MOUNTAIN || to_room->sector_type == SECT_HILLS)
			&& !is_npc(ch)
			&& ch->Profs()->HasProf(psn_mountaineering))
		{
			mountaineer = true;
			move = std::max(2, move - ch->Profs()->GetProf(psn_mountaineering) > 3 ? 3 : 2);
			wait = std::max(1, wait - ch->Profs()->GetProf(psn_mountaineering) > 6 ? 3 : 2);
			ch->Profs()->CheckImprove(psn_mountaineering, 6);
		}

		if (ch->move < (int)move)
		{
			send_to_char("You are too exhausted.\n\r", ch);
			return;
		}

		if (get_trust(ch) > 53)
			wait = 0;

		WAIT_STATE(ch, ((int)wait * PULSE_PER_MOVE));
		ch->move -= (int)move;
	}

	if (is_affected_by(ch, AFF_CAMOUFLAGE) && !is_affected(ch, gsn_creep))
		un_camouflage(ch, nullptr);

	un_blade_barrier(ch, nullptr);

	if (is_affected(ch, gsn_outflank))
		affect_strip(ch, gsn_outflank);

	if (to_room->sector_type != SECT_UNDERWATER || ch->in_room->sector_type != SECT_UNDERWATER)
		un_watermeld(ch, nullptr);

	if (to_room->sector_type == SECT_UNDERWATER)
	{
		if (is_affected(ch, gsn_travelease))
		{
			affect_strip(ch, gsn_travelease);
			send_to_char("The gentle tailwind dissipates.\n\r", ch);
		}

		if (is_affected(ch, gsn_airshield))
		{
			affect_strip(ch, gsn_airshield);
			send_to_char("Your air shield dissipates.\n\r", ch);
		}

		un_shroud(ch, nullptr);
	}

	if (to_room->sector_type == SECT_AIR
		|| to_room->sector_type == SECT_VERTICAL
		|| to_room->sector_type == SECT_WATER
		|| to_room->sector_type == SECT_UNDERWATER)
	{
		if (is_affected(ch, gsn_sneak))
			un_sneak(ch, nullptr);
	}

	if (is_affected_by(ch, AFF_HIDE)
		&& to_room->sector_type != SECT_CITY
		&& to_room->sector_type != SECT_INSIDE
		&& !is_affected(ch, gsn_creep))
	{
		un_hide(ch, nullptr);
	}

	if (is_affected(ch, gsn_silent_movement) && !check_silent_movement(ch, to_room))
	{
		affect_strip(ch, gsn_silent_movement);
		send_to_char("Due to the unfamiliar area you stop moving around silently.\n\r", ch);
	}

	char exbuf[MAX_STRING_LENGTH];
	if (!is_affected_by(ch, AFF_SNEAK)
		&& !check_silent_movement(ch, ch->in_room)
		&& ch->invis_level < LEVEL_HERO
		&& !is_affected_by(ch, AFF_NOSHOW)
		&& !is_affected(ch, gsn_creep)
		&& !is_affected(ch, gsn_watermeld))
	{
		un_hide(ch, nullptr);

		if (is_affected(ch, gsn_door_bash))
			act("$n goes crashing through the door $T.", ch, 0, dir_name[door], TO_ROOM);
		else
			sprintf(exbuf, "leave");

		if (to_room->sector_type == SECT_UNDERWATER || ch->in_room->sector_type == SECT_UNDERWATER || swimmer)
			sprintf(exbuf, "swim");
		else if (is_affected_by(ch, AFF_FLYING))
			sprintf(exbuf, "float");
		else if (ch->legs < 1)
			sprintf(exbuf, "crawl");
		else if (ch->legs < 2)
			sprintf(exbuf, "limp");
		else if (mountaineer)
			sprintf(exbuf, "climb");

		if (mountaineer || swimmer || (ch->legs < 2 && is_land(ch->in_room) && is_land(to_room)))
			act("You $t $T.", ch, exbuf, dir_name[door], TO_CHAR);

		act("$n $ts $T.", ch, exbuf, dir_name[door], TO_ROOM);
	}
	else
	{
		check_detect_movement(ch, dir_name[door]);
		act("IMM: $n sneaks $T.", ch, nullptr, dir_name[door], TO_IMMINROOM);
	}

	if (to_room->sector_type != SECT_FOREST
		&& to_room->sector_type != SECT_MOUNTAIN
		&& to_room->sector_type != SECT_HILLS)
	{
		un_camouflage(ch, nullptr);
	}

	if (!is_npc(ch))
	{
		add_tracks(in_room, ch, door);
	}

	if (ch->fighting)
		stop_fighting(ch, true);

	char_from_room(ch);
	char_to_room(ch, to_room);

	if (in_room->area != to_room->area)
	{
		for (auto d = descriptor_list; d; d = d->next)
		{
			auto victim = d->character;
			if (d->connected == CON_PLAYING
				&& victim != nullptr
				&& !is_npc(victim)
				&& victim->in_room != nullptr
				&& victim->in_room->area == ch->in_room->area
				&& !is_immortal(ch)
				&& get_skill(victim, gsn_shadowsense) > 1
				&& victim != ch
				&& is_evil(ch))
			{
				if (!can_see(ch, victim))
					ss_chance = (int)((get_skill(victim, gsn_shadowsense) * .85) / 2);
				else
					ss_chance = (int)(get_skill(victim, gsn_shadowsense) * .85);

				if (number_percent() < ss_chance)
				{
					check_improve(victim, gsn_shadowsense, true, 1);
					send_to_char("You feel darkness enter the vicinity.\n\r", victim);
				}
				else
				{
					check_improve(victim, gsn_shadowsense, false, 1);
				}
			}
		}
	}

	if (!is_affected_by(ch, AFF_SNEAK) 
		&& !check_silent_movement(ch, ch->in_room)
		&& ch->invis_level < LEVEL_HERO
		&& !is_affected(ch, gsn_creep)
		&& !is_affected(ch, gsn_watermeld)
		&& !is_affected_by(ch, AFF_NOSHOW))
	{
		sprintf(exbuf, "has arrived");

		if (!is_npc(ch))
		{
			if (to_room->sector_type == SECT_UNDERWATER || ch->in_room->sector_type == SECT_UNDERWATER)
				sprintf(exbuf, "swims in");
			else if (is_affected_by(ch, AFF_FLYING))
				sprintf(exbuf, "floats in");
			else if (ch->legs < 1)
				sprintf(exbuf, "crawls in");
			else if (ch->legs < 2)
				sprintf(exbuf, "limps in");
		}

		if (is_affected(ch, gsn_door_bash))
			act("The $T door bursts open and $n comes crashing in!", ch, 0, dir_name[door], TO_ROOM);
		else
			act("$n $t.", ch, exbuf, nullptr, TO_ROOM);
	}
	else
	{
		check_detect_movement(ch, "in");
		act("IMM: $n sneaks in.", ch, nullptr, nullptr, TO_IMMINROOM);
	}

	do_look(ch, "auto");
	if (is_affected(ch, gsn_impale))
	{
		send_to_char("The jagged shaft twists painfully in your wound as you move.\n\r", ch);

		auto owner = ch;
		imaf = affect_find(ch->affected, gsn_impale);
		if (imaf && imaf->owner)
			owner = imaf->owner;

		damage_new(owner, ch, dice(3, 3), TYPE_UNDEFINED, DAM_NONE, true, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "Your gaping wound*");

		if (!ch->in_room || ch->ghost > 0)
			return;
	}

	if (is_affected_room(in_room, gsn_tripwire) && is_affected_room(to_room, gsn_tripwire))
	{
		auto raf = in_room->affected;
		for (; raf != nullptr; raf = raf->next)
		{
			if (raf->modifier == door && raf->type == gsn_tripwire)
				break;
		}

		auto raf_two = to_room->affected;
		for (; raf_two != nullptr; raf_two = raf_two->next)
		{
			if (raf_two->modifier == reverse_d(door) && raf->type == gsn_tripwire)
				break;
		}

		if (is_affected_by(ch, AFF_FLYING))
			return;

		if (raf->owner == ch)
		{
			act("You gracefully step over your tripwire.", ch, 0, 0, TO_CHAR);
		}
		else
		{
			twchance = (get_skill(ch, gsn_tripwire) / 5);
			twchance += (get_curr_stat(ch, STAT_DEX) + get_curr_stat(ch, STAT_INT) - 30) * 2;

			if (!is_safe(raf->owner, ch) && (number_percent() > twchance))
			{
				act("You trip over a wire and fall flat on your face!", ch, 0, 0, TO_CHAR);
				act("$n trips over a wire and falls flat on $s face!", ch, 0, 0, TO_ROOM);

				ch->position = POS_RESTING;
				WAIT_STATE(ch, PULSE_VIOLENCE * 2);

				affect_remove_room(in_room, raf);
				affect_remove_room(to_room, raf_two);
			}
		}
	}

	if (is_affected_room(to_room, gsn_riptide))
	{
		for (auto raf = to_room->affected; raf != nullptr; raf = raf->next)
		{
			if (raf->type == gsn_riptide && raf->location == APPLY_ROOM_NONE && raf->modifier == 1)
			{
				if (is_safe_new(raf->owner, ch, false) || raf->owner == ch)
					break;

				ROOM_INDEX_DATA *riptideroom;
				for (auto room = top_affected_room; room; room = room->aff_next)
				{
					if (is_affected_room(room, gsn_riptide))
					{
						for (auto raf_two = room->affected; raf_two != nullptr; raf_two = raf_two->next)
						{
							if (raf_two->type == gsn_riptide && raf_two->owner == raf->owner &&
								raf_two->location == APPLY_ROOM_NONE && raf_two->modifier == 2)
								riptideroom = room;
						}
					}
				}

				if (riptideroom != nullptr)
				{
					sprintf(exbuf, "%sAs you enter, a violent current sucks you under the surface of the ocean!\n\rYou are pulled through the water at a dizzying pace, unable to resist!%s\n\r",
						get_char_color(ch, "cyan"),
						END_COLOR(ch));
					send_to_char(exbuf, ch);

					act("$n is suddenly pulled under the surface by a violent riptide!", ch, 0, 0, TO_ROOM);

					char_from_room(ch);
					char_to_room(ch, riptideroom);

					act("$n is suddenly pulled into the room by the powerful riptide!", ch, 0, 0, TO_ROOM);

					do_look(ch, "auto");

					WAIT_STATE(ch, PULSE_VIOLENCE);
					break;
				}
			}
		}
	}

	while (is_affected_room(to_room, gsn_frost_growth))
	{
		auto raf = to_room->affected;
		for (; raf != nullptr; raf = raf->next)
		{
			if (raf->type == gsn_frost_growth)
				break;
		}

		if (is_affected_by(ch, AFF_FLYING))
			break;

		if (is_same_cabal(ch, raf->owner) || is_same_group(ch, raf->owner))
			break;

		if (is_safe(ch, raf->owner))
			break;

		if (number_percent() <= (5 * (get_curr_stat(ch, STAT_DEX) - 15)))
		{
			act("You almost lose your footing on the surprisingly-slick ground, but steady yourself.", ch, 0, 0, TO_CHAR);
			act("$n almost loses $s footing on the frost-covered ground, but remains standing.", ch, 0, 0, TO_ROOM);
			break;
		}

		act("You slip on an unnoticed patch of ice and crash heavily to the ground!", ch, 0, 0, TO_CHAR);
		act("$n slips on the icy ground as he enters, collapsing in a heap!", ch, 0, 0, TO_ROOM);

		WAIT_STATE(ch, (ch->carry_weight / 150) * PULSE_VIOLENCE);

		damage_new(raf->owner, ch, 5 + ch->carry_weight / 6, TYPE_UNDEFINED, DAM_BASH, true, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "the fall*");

		stop_fighting(ch, true);

		ch->position = POS_RESTING;
		break;
	}

	while (is_affected_room(to_room, gsn_quicksand))
	{
		if (is_npc(ch))
			break;

		auto raf = to_room->affected;
		for (; raf != nullptr; raf = raf->next)
		{
			if (raf->type == gsn_quicksand)
				break;
		}

		if (is_safe_new(raf->owner, ch, false) || is_same_group(raf->owner, ch) || is_same_cabal(raf->owner, ch))
			break;

		if (is_affected(ch, gsn_ultradiffusion))
			break;

		if (is_affected_by(ch, AFF_FLYING))
		{
			send_to_char("As you float into the room, you spy a pool of quicksand concealed beneath you!\n\r", ch);
			break;
		}

		act("As you enter, the ground gives way beneath you and you sink into quicksand!", ch, 0, 0, TO_CHAR);
		act("As $n enters, the ground gives way beneath $m and $e sinks into quicksand!", ch, 0, 0, TO_ROOM);
		do_myell(ch, "Help!  I'm sinking in quicksand!", nullptr);

		AFFECT_DATA qs;
		init_affect(&qs);
		qs.where = TO_AFFECTS;
		qs.aftype = AFT_INVIS;
		qs.type = gsn_quicksand_sinking;
		qs.owner = ch;
		qs.level = raf->level;
		qs.duration = -1;
		qs.modifier = 1;
		qs.pulse_fun = quicksand_pulse_sink;
		affect_to_char(ch, &qs);

		WAIT_STATE(ch, 2 * PULSE_VIOLENCE);

		break;
	}

	while (is_affected_room(to_room, gsn_stalactites))
	{
		auto raf = to_room->affected;
		for (; raf != nullptr; raf = raf->next)
		{
			if (raf->type == gsn_stalactites)
				break;
		}

		if (is_safe_new(raf->owner, ch, false) || is_same_group(raf->owner, ch) || is_same_cabal(raf->owner, ch))
			break;

		if (is_affected(ch, gsn_ultradiffusion))
			break;

		act("An icy stalactite plummets from the ceiling above as you enter!", ch, 0, 0, TO_CHAR);
		act("An icy stalactite plummets from the ceiling above!", ch, 0, 0, TO_ROOM);

		do_visible(ch, "");

		if (number_percent() > (10 * get_curr_stat(ch, STAT_DEX) - 175))
		{
			damage_new(raf->owner, ch, dice(raf->level, 3), gsn_stalactites, DAM_PIERCE, true, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "the falling stalactite*");
		}
		else
		{
			act("The mass of ice shatters on the ground as you leap out of the way!", ch, 0, 0, TO_CHAR);
			act("The mass of ice shatters on the ground as $n leaps out of the way!", ch, 0, 0, TO_ROOM);
			damage_new(raf->owner, ch, 0, gsn_stalactites, DAM_PIERCE, true, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "the falling stalactite*");
		}

		if (!is_npc(ch))
			stop_fighting(ch, true);

		if (--raf->modifier <= 0)
			affect_remove_room(to_room, raf);

		if (!ch->in_room || ch->ghost > 0)
			return;

		break;
	}

	if (is_affected_room(to_room, gsn_caustic_vapor))
	{
		auto raf = to_room->affected;
		for (; raf != nullptr; raf = raf->next)
		{
			if (raf->type == gsn_caustic_vapor)
				break;
		}

		if (is_immortal(ch))
			return;

		if (!is_safe(raf->owner, ch) && !is_affected(ch, gsn_neutralize))
		{
			send_to_char("You choke and gasp for air as caustic vapors fill your lungs!\n\r", ch);

			WAIT_STATE(ch, PULSE_VIOLENCE);

			if (!is_affected(ch, gsn_noxious_fumes))
			{
				init_affect(&cvaf);
				cvaf.where = TO_AFFECTS;
				cvaf.type = gsn_noxious_fumes;
				cvaf.aftype = AFT_MALADY;
				cvaf.level = raf->level;
				cvaf.duration = 4;
				cvaf.location = 0;
				cvaf.modifier = 0;
				cvaf.owner = raf->owner;
				new_affect_to_char(ch, &cvaf);
			}

			for (auto paf = ch->affected; paf != nullptr; paf = paf->next)
			{
				if (paf->type == gsn_noxious_fumes)
					break;
			}

			paf->modifier = URANGE(0, paf->modifier, 5);

			paf->modifier++;

			init_affect(&cvaf2);
			cvaf2.where = TO_AFFECTS;
			cvaf2.type = gsn_noxious_fumes;
			cvaf2.aftype = AFT_MALADY;
			cvaf2.level = raf->level;
			cvaf2.duration = 1;
			cvaf2.location = 0;
			cvaf2.modifier = 0;
			cvaf2.owner = raf->owner;

			switch (paf->modifier)
			{
				case 1:
					dam = dice(raf->level / 3, 2);
					break;
				case 2:
					dam = dice(raf->level / 3, 3);
					break;
				case 3:
					dam = dice(raf->level / 3, 3);
					send_to_char("A wave of nausea washes over you as you inhale the vapors.\n\r", ch);
					SET_BIT(cvaf2.bitvector, AFF_POISON);
					break;
				case 4:
					dam = dice(raf->level / 2, 2);
					SET_BIT(cvaf2.bitvector, AFF_POISON);
					break;
				case 5:
					dam = dice(raf->level / 2, 2);
					send_to_char("The fumes sear your eyes, blinding you!\n\r", ch);
					SET_BIT(cvaf2.bitvector, AFF_POISON);
					SET_BIT(cvaf2.bitvector, AFF_BLIND);
					break;
				default:
					dam = dice(raf->level / 2, 3);
					SET_BIT(cvaf2.bitvector, AFF_POISON);
					SET_BIT(cvaf2.bitvector, AFF_BLIND);
					break;
			}

			if (paf->modifier >= 3)
				cvaf2.tick_fun = poison_tick;

			new_affect_join(ch, &cvaf2);

			damage_new(raf->owner, ch, dam, raf->type, DAM_POISON, true, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "the noxious fumes*$");

			if (!ch->in_room || ch->ghost > 0)
				return;
		}

		if (!is_safe(ch, raf->owner) && is_affected(ch, gsn_neutralize))
			send_to_char("The noxious fumes surround you, but you are unaffected.\n\r", ch);
	}

	if (in_room == to_room) /* no circular follows */
		return;

	if (!is_npc(ch) && ch->Profs()->HasProf("trap detecting"))
	{
		for (auto i = 0; i < MAX_EXITS; i++)
		{
			if (to_room->exit[i])
			{
				auto td = to_room->exit[i]->u1.to_room;

				if (td != nullptr && td->trap && td->trap->armed && ch->Profs()->ProfEffect("trap detecting") >= td->trap->quality)
				{
					RS.Queue.AddToQueue(1, 2, send_to_char, "Something about your surroundings suddenly makes you feel wary.\n\r", ch);
					break;
				}
			}
		}
	}

	if (to_room->has_rune == true && to_room == ch->in_room)
	{
		auto rune = find_rune(to_room, RUNE_TO_ROOM, RUNE_TRIGGER_ENTRY, nullptr);
		while (rune != nullptr)
		{
			rune->function(rune, ch, &door, nullptr);
		}
	}

	if (to_room->trap && to_room->trap->armed)
		trip_trap(ch, to_room, to_room->trap);

	auto room_has_pc = false;
	for (auto fch = to_room->people; fch != nullptr; fch = fch->next_in_room)
	{
		if (!is_npc(fch))
			room_has_pc = true;
	}

	CHAR_DATA *fch_next;
	for (auto fch = to_room->people; fch != nullptr; fch = fch_next)
	{
		fch_next = fch->next_in_room;
		/* greet trigger for items carried by people in room */
		for (auto obj = fch->carrying; room_has_pc && obj != nullptr; obj = obj->next_content)
		{
			if (IS_SET(obj->progtypes, IPROG_GREET))
				obj->pIndexData->iprogs->greet_prog(obj, ch);

			if (TRAPS_IEVENT(obj, TRAP_IGREET))
				CALL_IEVENT(obj, TRAP_IGREET, ch, obj);
		}

		if (is_npc(fch) && fch->last_fought == ch && number_percent() > 60)
		{
			track_attack(fch, ch);
		}
		else if (is_npc(fch) && fch->last_fought == ch)
		{
			RS.Queue.AddToQueue((number_percent() > 25) ? 1 : 2, 2, track_attack, fch, ch);
		}

		/* Greet trigger for mobs  */
		if (room_has_pc && IS_SET(fch->progtypes, MPROG_GREET))
			fch->pIndexData->mprogs->greet_prog(fch, ch);

		if (room_has_pc && TRAPS_MEVENT(fch, TRAP_MGREET))
			CALL_MEVENT(fch, TRAP_MGREET, ch, fch);
	}

	for (auto obj = ch->carrying; room_has_pc && obj != nullptr; obj = obj->next_content)
	{
		if (IS_SET(obj->progtypes, IPROG_ENTRY))
			(obj->pIndexData->iprogs->entry_prog)(obj);
	}

	if (IS_SET(to_room->progtypes, RPROG_ENTRY))
		to_room->rprogs->entry_prog(to_room, ch);

	if (fcharm)
	{
		for (auto fch = in_room->people; fch != nullptr; fch = fch_next)
		{
			fch_next = fch->next_in_room;

			if (fch->master == ch && is_affected_by(fch, AFF_CHARM) && fch->position < POS_STANDING)
				do_stand(fch, "");

			if (fch->master == ch && fch->position == POS_STANDING && can_see_room(fch, to_room))
			{
				if (IS_SET(ch->in_room->room_flags, ROOM_LAW) && (is_npc(fch) && IS_SET(fch->act, ACT_AGGRESSIVE)))
				{
					act("You can't bring $N into the city.", ch, nullptr, fch, TO_CHAR);
					act("You aren't allowed in the city.", fch, nullptr, nullptr, TO_CHAR);
					continue;
				}

				act("You follow $N.", fch, nullptr, ch, TO_CHAR);
				move_char(fch, door, true, true);
			}
		}
	}

	check_plasma_thread(ch, door);

	for (auto obj = ch->in_room->contents; obj != nullptr; obj = obj->next_content)
	{
		if (IS_SET(obj->progtypes, IPROG_GREET))
			(obj->pIndexData->iprogs->greet_prog)(obj, ch);
	}

	if (IS_SET(ch->progtypes, MPROG_ENTRY))
		ch->pIndexData->mprogs->entry_prog(ch);

	if (!is_npc(ch) && to_room->tracks[0] != nullptr)
	{
		auto i = 0;
		for (; i < MAX_TRACKS; i++)
		{
			if (to_room->tracks[i]->prey == ch)
				break;
		}

		if (i == MAX_TRACKS)
			i--;

		if (to_room->tracks[i]->prey == ch)
		{
			for (; i < MAX_TRACKS - 1; i++)
			{
				to_room->tracks[i]->prey = to_room->tracks[i + 1]->prey;
				to_room->tracks[i]->time = to_room->tracks[i + 1]->time;
				to_room->tracks[i]->direction = to_room->tracks[i + 1]->direction;
				to_room->tracks[i]->flying = to_room->tracks[i + 1]->flying;
				to_room->tracks[i]->sneaking = to_room->tracks[i + 1]->sneaking;
				to_room->tracks[i]->bleeding = to_room->tracks[i + 1]->bleeding;
				to_room->tracks[i]->legs = to_room->tracks[i + 1]->legs;
			}

			to_room->tracks[MAX_TRACKS - 1]->prey = nullptr;
			to_room->tracks[MAX_TRACKS - 1]->direction = -1;
		}
	}

	/*
	 * If someone is following the char, these triggers get activated
	 * for the followers before the char, but it's safer this way...
	 */
}

void do_north(CHAR_DATA *ch, char *argument)
{
	if (is_affected(ch, gsn_bind_feet))
	{
		send_to_char("Your feet are rooted to the ground and you cannot move!\n\r", ch);
		return;
	}

	move_char(ch, Directions::North, false, true);
}

void do_east(CHAR_DATA *ch, char *argument)
{
	if (is_affected(ch, gsn_bind_feet))
	{
		send_to_char("Your feet are rooted to the ground and you cannot move!\n\r", ch);
		return;
	}

	move_char(ch, Directions::East, false, true);
}

void do_south(CHAR_DATA *ch, char *argument)
{
	if (is_affected(ch, gsn_bind_feet))
	{
		send_to_char("Your feet are rooted to the ground and you cannot move!\n\r", ch);
		return;
	}

	move_char(ch, Directions::South, false, true);
}

void do_west(CHAR_DATA *ch, char *argument)
{
	if (is_affected(ch, gsn_bind_feet))
	{
		send_to_char("Your feet are rooted to the ground and you cannot move!\n\r", ch);
		return;
	}

	move_char(ch, Directions::West, false, true);
}

void do_up(CHAR_DATA *ch, char *argument)
{
	if (is_affected(ch, gsn_bind_feet))
	{
		send_to_char("Your feet are rooted to the ground and you cannot move!\n\r", ch);
		return;
	}

	move_char(ch, Directions::Up, false, true);
}

void do_down(CHAR_DATA *ch, char *argument)
{
	if (is_affected(ch, gsn_bind_feet))
	{
		send_to_char("Your feet are rooted to the ground and you cannot move!\n\r", ch);
		return;
	}

	move_char(ch, Directions::Down, false, true);
}

void trip_trap(CHAR_DATA *ch, ROOM_INDEX_DATA *room, TRAP_DATA *trap)
{
	if (!trap->armed)
		return;

	if (!is_npc(ch) && ch->Profs()->HasProf("trap detecting"))
	{
		if (ch->Profs()->GetProf("trap detecting") / 2 >= trap->quality)
		{
			ch->Profs()->CheckImprove("trap detecting", 400);
			RS.Queue.AddToQueue(1, 2, send_to_char, "As you carefully advance, you spot a trap and quickly sidestep it!\n\r", ch);
			return;
		}
		else
		{
			ch->Profs()->CheckImprove("trap detecting", 200);
		}
	}

	trap->armed = false;

	if (trap->timer)
	{
		act(trap->trig_echo, ch, 0, 0, TO_CHAR);
		RS.Queue.AddToQueue(trap->timer, 3, trap_execute, nullptr, room, trap);
	}
	else
	{
		trap_execute(ch, room, trap);
	}
}

void trap_execute(CHAR_DATA *victim, ROOM_INDEX_DATA *room, TRAP_DATA *trap)
{
	if (!victim)
		victim = get_rand_from_room(room);

	if (!victim)
		return;

	act(trap->exec_echo, victim, 0, 0, TO_ALL);
	switch (trap->type)
	{
		case TRAP_DART:
			damage_new(victim, victim, (dice(trap->quality * 10, 7)), TYPE_UNDEFINED, DAM_PIERCE, true, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "the razor-tipped dart*");
			break;
		case TRAP_PDART:
			damage_new(victim, victim, (dice(trap->quality * 5, 7)), TYPE_UNDEFINED, DAM_PIERCE, true, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "the poison-tipped dart*");
			if (saves_spell(8 * trap->quality, victim, DAM_POISON))
			{
				send_to_char("You feel a burning sensation in your veins, but it fades away.\n\r", victim);
				break;
			}

			if (IS_SET(victim->affected_by, AFF_POISON))
				break;

			send_to_char("Your blood burns as a deadly venom seeps into your body!\n\r", victim);

			AFFECT_DATA af;
			init_affect(&af);
			af.where = TO_AFFECTS;
			af.aftype = AFT_MALADY;
			af.type = gsn_poison;
			af.level = trap->quality * 7;
			af.location = APPLY_STR;
			af.modifier = -5;
			af.duration = trap->quality * 2;
			af.owner = victim;
			SET_BIT(af.bitvector, AFF_POISON);
			af.tick_fun = poison_tick;
			affect_to_char(victim, &af);
			break;
		case TRAP_FIREBALL:
			for (auto vch = room->people; vch; vch = vch->next_in_room)
			{
				damage_new(vch, vch, dice(trap->quality * 9, 9), TYPE_UNDEFINED, DAM_FIRE, true, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "the fireball*");
			}
			break;
		case TRAP_LIGHTNING:
			{
				auto dam = damage_new(victim, victim, dice(trap->quality * 10, 9), TYPE_UNDEFINED, DAM_LIGHTNING, true, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "the lightning bolt*");

				if (trap->quality > 5)
					shock_effect(victim, trap->quality * 7, dam, TARGET_CHAR);
			}
			break;
		case TRAP_SLEEPGAS:
			{
				auto mod = trap->quality < 5
					? (trap->quality * 3) - 6
					: trap->quality * 2;

				for (auto vch = room->people; vch; vch = vch->next_in_room)
				{
					if (saves_spell(vch->level + mod, vch, DAM_OTHER))
					{
						send_to_char("A wave of drowsiness washes over you, but it passes.\n\r", vch);
						continue;
					}
					else
					{
						send_to_char("As you inhale the vapors, you drift off into a deep blackness....\n\r", vch);

						AFFECT_DATA af;
						init_affect(&af);
						af.where = TO_AFFECTS;
						af.aftype = AFT_MALADY;
						af.type = gsn_sleep;
						af.level = trap->quality * 6;
						af.owner = vch;
						af.location = 0;
						af.modifier = 0;
						af.duration = trap->quality * 4;

						affect_to_char(vch, &af);
					}
				}
			}
			break;
		case TRAP_POISONGAS:
			break;
		case TRAP_ACID:
			spell_acid_stream(gsn_acid_stream, trap->quality * 7, victim, victim, TARGET_CHAR);
			break;
		case TRAP_DRAIN:
			for (auto vch = room->people; vch; vch = vch->next_in_room)
			{
				send_to_char("You feel drained.\n\r", vch);

				if (trap->quality <= 4)
				{
					vch->mana = vch->mana / (trap->quality + 1);
				}
				else if (trap->quality == 5)
				{
					vch->mana = 0;
				}
				else
				{
					vch->mana = 0;

					AFFECT_DATA af;
					init_affect(&af);
					af.where = TO_AFFECTS;
					af.level = vch->level;
					af.aftype = AFT_MALADY;
					af.type = gsn_mana_drain;
					af.duration = trap->quality * 4;
					af.owner = vch;
					af.modifier = 0;
					af.location = 0;
					af.pulse_fun = mana_drain_pulse;
					affect_to_char(vch, &af);
				}
			}
			break;
		case TRAP_PIT:
			break;
		case TRAP_BOULDER:
			break;
		default:
			break;
	}
}

int find_door(CHAR_DATA *ch, char *arg)
{
	EXIT_DATA *pexit;
	int door = 0;

	if (!str_cmp(arg, "n") || !str_cmp(arg, "north"))
	{
		door = 0;
	}
	else if (!str_cmp(arg, "e") || !str_cmp(arg, "east"))
	{
		door = 1;
	}
	else if (!str_cmp(arg, "s") || !str_cmp(arg, "south"))
	{
		door = 2;
	}
	else if (!str_cmp(arg, "w") || !str_cmp(arg, "west"))
	{
		door = 3;
	}
	else if (!str_cmp(arg, "u") || !str_cmp(arg, "up"))
	{
		door = 4;
	}
	else if (!str_cmp(arg, "d") || !str_cmp(arg, "down"))
	{
		door = 5;
	}
	else
	{
		for (auto exit : ch->in_room->exit)
		{
			pexit = exit;

			if (pexit != nullptr
				&& IS_SET(pexit->exit_info, EX_ISDOOR)
				&& pexit->keyword != nullptr
				&& is_name(arg, pexit->keyword))
			{
				return door;
			}

			door++;
		}

		act("You don't see a door $T here.", ch, nullptr, arg, TO_CHAR);
		return -1;
	}

	pexit = ch->in_room->exit[door];

	if (pexit == nullptr)
	{
		act("You see no door $T here.", ch, nullptr, arg, TO_CHAR);
		return -1;
	}

	if (!IS_SET(pexit->exit_info, EX_ISDOOR))
	{
		send_to_char("There's no door there!\n\r", ch);
		return -1;
	}

	return door;
}

void do_open(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	one_argument(argument, arg);

	if (arg[0] == '\0')
	{
		send_to_char("Open what?\n\r", ch);
		return;
	}

	auto obj = get_obj_here(ch, arg);
	if (obj != nullptr)
	{
		/* open portal */
		if (obj->item_type == ITEM_PORTAL)
		{
			if (!IS_SET_OLD(obj->value[1], EX_ISDOOR))
			{
				send_to_char("You can't do that.\n\r", ch);
				return;
			}

			if (!IS_SET_OLD(obj->value[1], EX_CLOSED))
			{
				send_to_char("It's already open.\n\r", ch);
				return;
			}

			if (IS_SET_OLD(obj->value[1], EX_LOCKED))
			{
				send_to_char("It's locked.\n\r", ch);
				return;
			}

			REMOVE_BIT_OLD(obj->value[1], EX_CLOSED);

			act("You open $p.", ch, obj, nullptr, TO_CHAR);
			act("$n opens $p.", ch, obj, nullptr, TO_ROOM);
			return;
		}

		/* 'open object' */
		if (obj->item_type != ITEM_CONTAINER)
		{
			send_to_char("That's not a container.\n\r", ch);
			return;
		}

		if (is_affected_obj(obj, gsn_ice_blast))
		{
			act("$p is frozen shut!", ch, obj, 0, TO_CHAR);
			return;
		}

		if (!IS_SET_OLD(obj->value[1], CONT_CLOSED))
		{
			send_to_char("It's already open.\n\r", ch);
			return;
		}

		if (IS_SET_OLD(obj->value[1], CONT_LOCKED))
		{
			send_to_char("It's locked.\n\r", ch);
			return;
		}

		if (IS_SET(obj->progtypes, IPROG_OPEN))
		{
			if (!obj->pIndexData->iprogs->open_prog(obj, ch))
				return;
		}

		if (TRAPS_IEVENT(obj, TRAP_IOPEN))
		{
			if (CALL_IEVENT(obj, TRAP_IOPEN, ch, obj))
				return;
		}

		REMOVE_BIT_OLD(obj->value[1], CONT_CLOSED);
		
		act("You open $p.", ch, obj, nullptr, TO_CHAR);
		act("$n opens $p.", ch, obj, nullptr, TO_ROOM);
		return;
	}

	auto door = find_door(ch, arg);
	if (door < 0)
		return;

	/* 'open door' */
	auto pexit = ch->in_room->exit[door];

	if (IS_SET(ch->in_room->progtypes, RPROG_OPEN))
	{
		if (!ch->in_room->rprogs->open_prog(ch->in_room, ch, pexit))
			return;
	}

	if (!IS_SET(pexit->exit_info, EX_CLOSED))
	{
		send_to_char("It's already open!\n\r", ch);
		return;
	}

	if (IS_SET(pexit->exit_info, EX_LOCKED))
	{
		send_to_char("It's locked.\n\r", ch);
		return;
	}

	if (IS_SET(pexit->exit_info, EX_JAMMED))
	{
		send_to_char("You can't seem to pull the door open.\n\r", ch);
		return;
	}

	REMOVE_BIT(pexit->exit_info, EX_CLOSED);

	act("$n opens the $T.", ch, nullptr, pexit->keyword != nullptr ? pexit->keyword : "door", TO_ROOM);
	send_to_char("Ok.\n\r", ch);

	/* open the other side */
	auto to_room = pexit->u1.to_room;
	if(to_room != nullptr)
	{
		auto pexit_rev = to_room->exit[rev_dir[door]];
		if (pexit_rev != nullptr && pexit_rev->u1.to_room == ch->in_room)
		{
			REMOVE_BIT(pexit_rev->exit_info, EX_CLOSED);

			for (auto rch = to_room->people; rch != nullptr; rch = rch->next_in_room)
			{
				act("The $T opens.", rch, nullptr, pexit_rev->keyword != nullptr ? pexit_rev->keyword : "door", TO_CHAR);
			}
		}
	}
}

void do_close(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	one_argument(argument, arg);

	if (arg[0] == '\0')
	{
		send_to_char("Close what?\n\r", ch);
		return;
	}

	auto obj = get_obj_here(ch, arg);
	if (obj != nullptr)
	{
		/* portal stuff */
		if (obj->item_type == ITEM_PORTAL)
		{

			if (!IS_SET_OLD(obj->value[1], EX_ISDOOR) || IS_SET_OLD(obj->value[1], EX_NOCLOSE))
			{
				send_to_char("You can't do that.\n\r", ch);
				return;
			}

			if (IS_SET_OLD(obj->value[1], EX_CLOSED))
			{
				send_to_char("It's already closed.\n\r", ch);
				return;
			}

			SET_BIT_OLD(obj->value[1], EX_CLOSED);
			act("You close $p.", ch, obj, nullptr, TO_CHAR);
			act("$n closes $p.", ch, obj, nullptr, TO_ROOM);
			return;
		}

		/* 'close object' */
		if (obj->item_type != ITEM_CONTAINER)
		{
			send_to_char("That's not a container.\n\r", ch);
			return;
		}

		if (is_affected_obj(obj, gsn_ice_blast))
		{
			act("$p is frozen shut!", ch, obj, 0, TO_CHAR);
			return;
		}

		if (IS_SET_OLD(obj->value[1], CONT_CLOSED))
		{
			send_to_char("It's already closed.\n\r", ch);
			return;
		}

		if (!IS_SET_OLD(obj->value[1], CONT_CLOSEABLE))
		{
			send_to_char("You can't do that.\n\r", ch);
			return;
		}

		SET_BIT_OLD(obj->value[1], CONT_CLOSED);
		act("You close $p.", ch, obj, nullptr, TO_CHAR);
		act("$n closes $p.", ch, obj, nullptr, TO_ROOM);
		return;
	}

	auto door = find_door(ch, arg);
	if (door < 0)
		return;

	/* 'close door' */
	auto pexit = ch->in_room->exit[door];
	if (IS_SET(pexit->exit_info, EX_CLOSED))
	{
		send_to_char("It's already closed.\n\r", ch);
		return;
	}

	if (IS_SET(pexit->exit_info, EX_NOCLOSE))
	{
		send_to_char("You can't close it.\n\r", ch);
		return;
	}

	SET_BIT(pexit->exit_info, EX_CLOSED);

	act("$n closes the $T.", ch, nullptr, pexit->keyword, TO_ROOM);
	send_to_char("Ok.\n\r", ch);

	WAIT_STATE(ch, 2 * PULSE_PER_SECOND);

	/* close the other side */
	auto to_room = pexit->u1.to_room;
	if(to_room != nullptr)
	{
		auto pexit_rev = to_room->exit[rev_dir[door]];
		if (pexit_rev != 0 && pexit_rev->u1.to_room == ch->in_room)
		{
			CHAR_DATA *rch;

			SET_BIT(pexit_rev->exit_info, EX_CLOSED);
			for (rch = to_room->people; rch != nullptr; rch = rch->next_in_room)
			{
				act("The $T closes.", rch, nullptr, pexit_rev->keyword, TO_CHAR);
			}
		}
	}
}

bool has_key(CHAR_DATA *ch, int key)
{
	for (auto *obj = ch->carrying; obj != nullptr; obj = obj->next_content)
	{
		if (obj->pIndexData->vnum == key)
			return true;
	}

	return false;
}

void do_lock(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	one_argument(argument, arg);

	if (arg[0] == '\0')
	{
		send_to_char("Lock what?\n\r", ch);
		return;
	}

	auto obj = get_obj_here(ch, arg);
	if (obj != nullptr)
	{
		/* portal stuff */
		if (obj->item_type == ITEM_PORTAL)
		{
			if (!IS_SET_OLD(obj->value[1], EX_ISDOOR) || IS_SET_OLD(obj->value[1], EX_NOCLOSE))
			{
				send_to_char("You can't do that.\n\r", ch);
				return;
			}

			if (!IS_SET_OLD(obj->value[1], EX_CLOSED))
			{
				send_to_char("It's not closed.\n\r", ch);
				return;
			}

			if (obj->value[4] < 0 || IS_SET_OLD(obj->value[1], EX_NOLOCK))
			{
				send_to_char("It can't be locked.\n\r", ch);
				return;
			}

			if (!has_key(ch, obj->value[4]))
			{
				send_to_char("You lack the key.\n\r", ch);
				return;
			}

			if (IS_SET_OLD(obj->value[1], EX_LOCKED))
			{
				send_to_char("It's already locked.\n\r", ch);
				return;
			}

			SET_BIT_OLD(obj->value[1], EX_LOCKED);
			act("You lock $p.", ch, obj, nullptr, TO_CHAR);
			act("$n locks $p.", ch, obj, nullptr, TO_ROOM);
			return;
		}

		/* 'lock object' */
		if (obj->item_type != ITEM_CONTAINER)
		{
			send_to_char("That's not a container.\n\r", ch);
			return;
		}

		if (!IS_SET_OLD(obj->value[1], CONT_CLOSED))
		{
			send_to_char("It's not closed.\n\r", ch);
			return;
		}

		if (obj->value[2] < 0)
		{
			send_to_char("It can't be locked.\n\r", ch);
			return;
		}

		if (!has_key(ch, obj->value[2]))
		{
			send_to_char("You lack the key.\n\r", ch);
			return;
		}

		if (IS_SET_OLD(obj->value[1], CONT_LOCKED))
		{
			send_to_char("It's already locked.\n\r", ch);
			return;
		}

		SET_BIT_OLD(obj->value[1], CONT_LOCKED);
		act("You lock $p.", ch, obj, nullptr, TO_CHAR);
		act("$n locks $p.", ch, obj, nullptr, TO_ROOM);
		return;
	}

	auto door = find_door(ch, arg);
	if (door < 0)
		return;

	/* 'lock door' */
	auto pexit = ch->in_room->exit[door];
	if (!IS_SET(pexit->exit_info, EX_CLOSED))
	{
		send_to_char("It's not closed.\n\r", ch);
		return;
	}

	if (pexit->key < 0)
	{
		send_to_char("It can't be locked.\n\r", ch);
		return;
	}

	if (!has_key(ch, pexit->key))
	{
		send_to_char("You lack the key.\n\r", ch);
		return;
	}

	if (IS_SET(pexit->exit_info, EX_LOCKED))
	{
		send_to_char("It's already locked.\n\r", ch);
		return;
	}

	SET_BIT(pexit->exit_info, EX_LOCKED);
	send_to_char("*Click*\n\r", ch);
	act("$n locks the $T.", ch, nullptr, pexit->keyword, TO_ROOM);

	/* lock the other side */
	auto to_room = pexit->u1.to_room;
	auto pexit_rev = to_room->exit[rev_dir[door]];
	if (to_room != nullptr && pexit_rev != nullptr && pexit_rev->u1.to_room == ch->in_room)
	{
		SET_BIT(pexit_rev->exit_info, EX_LOCKED);
	}
}

void do_unlock(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	one_argument(argument, arg);

	if (arg[0] == '\0')
	{
		send_to_char("Unlock what?\n\r", ch);
		return;
	}

	auto obj = get_obj_here(ch, arg);
	if (obj != nullptr)
	{
		/* portal stuff */
		if (obj->item_type == ITEM_PORTAL)
		{
			if (!IS_SET_OLD(obj->value[1], EX_ISDOOR))
			{
				send_to_char("You can't do that.\n\r", ch);
				return;
			}

			if (!IS_SET_OLD(obj->value[1], EX_CLOSED))
			{
				send_to_char("It's not closed.\n\r", ch);
				return;
			}

			if (obj->value[4] < 0)
			{
				send_to_char("It can't be unlocked.\n\r", ch);
				return;
			}

			if (!has_key(ch, obj->value[4]))
			{
				send_to_char("You lack the key.\n\r", ch);
				return;
			}

			if (!IS_SET_OLD(obj->value[1], EX_LOCKED))
			{
				send_to_char("It's already unlocked.\n\r", ch);
				return;
			}

			REMOVE_BIT_OLD(obj->value[1], EX_LOCKED);
			act("You unlock $p.", ch, obj, nullptr, TO_CHAR);
			act("$n unlocks $p.", ch, obj, nullptr, TO_ROOM);
			return;
		}

		/* 'unlock object' */
		if (obj->item_type != ITEM_CONTAINER)
		{
			send_to_char("That's not a container.\n\r", ch);
			return;
		}

		if (!IS_SET_OLD(obj->value[1], CONT_CLOSED))
		{
			send_to_char("It's not closed.\n\r", ch);
			return;
		}

		if (obj->value[2] < 0)
		{
			send_to_char("It can't be unlocked.\n\r", ch);
			return;
		}

		if (!has_key(ch, obj->value[2]))
		{
			send_to_char("You lack the key.\n\r", ch);
			return;
		}

		if (!IS_SET_OLD(obj->value[1], CONT_LOCKED))
		{
			send_to_char("It's already unlocked.\n\r", ch);
			return;
		}

		REMOVE_BIT_OLD(obj->value[1], CONT_LOCKED);

		act("You unlock $p.", ch, obj, nullptr, TO_CHAR);
		act("$n unlocks $p.", ch, obj, nullptr, TO_ROOM);
		return;
	}

	auto door = find_door(ch, arg);
	if (door < 0)
		return;

	/* 'unlock door' */
	auto pexit = ch->in_room->exit[door];
	if (!IS_SET(pexit->exit_info, EX_CLOSED))
	{
		send_to_char("It's not closed.\n\r", ch);
		return;
	}

	if (pexit->key < 0)
	{
		send_to_char("It can't be unlocked.\n\r", ch);
		return;
	}

	if (!has_key(ch, pexit->key))
	{
		send_to_char("You lack the key.\n\r", ch);
		return;
	}

	if (!IS_SET(pexit->exit_info, EX_LOCKED))
	{
		send_to_char("It's already unlocked.\n\r", ch);
		return;
	}

	REMOVE_BIT(pexit->exit_info, EX_LOCKED);

	send_to_char("*Click*\n\r", ch);
	act("$n unlocks the $T.", ch, nullptr, pexit->keyword, TO_ROOM);

	/* unlock the other side */
	auto to_room = pexit->u1.to_room;
	auto pexit_rev = to_room->exit[rev_dir[door]];
	if (to_room != nullptr && pexit_rev != nullptr && pexit_rev->u1.to_room == ch->in_room)
	{
		REMOVE_BIT(pexit_rev->exit_info, EX_LOCKED);
	}
}

void do_pick(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	one_argument(argument, arg);

	if (arg[0] == '\0')
	{
		send_to_char("Pick what?\n\r", ch);
		return;
	}

	WAIT_STATE(ch, skill_table[gsn_pick_lock].beats);

	/* look for guards */
	for (auto gch = ch->in_room->people; gch; gch = gch->next_in_room)
	{
		if (is_npc(gch) && is_awake(gch) && ch->level + 5 < gch->level)
		{
			act("$N is standing too close to the lock.", ch, nullptr, gch, TO_CHAR);
			return;
		}
	}

	if (!is_npc(ch) && number_percent() > get_skill(ch, gsn_pick_lock))
	{
		send_to_char("You failed.\n\r", ch);
		check_improve(ch, gsn_pick_lock, false, 2);
		return;
	}

	auto obj = get_obj_here(ch, arg);
	if (obj != nullptr)
	{
		/* portal stuff */
		if (obj->item_type == ITEM_PORTAL)
		{
			if (!IS_SET_OLD(obj->value[1], EX_ISDOOR))
			{
				send_to_char("You can't do that.\n\r", ch);
				return;
			}

			if (!IS_SET_OLD(obj->value[1], EX_CLOSED))
			{
				send_to_char("It's not closed.\n\r", ch);
				return;
			}

			if (obj->value[4] < 0)
			{
				send_to_char("It can't be unlocked.\n\r", ch);
				return;
			}

			if (IS_SET_OLD(obj->value[1], EX_PICKPROOF))
			{
				send_to_char("You failed.\n\r", ch);
				return;
			}

			REMOVE_BIT_OLD(obj->value[1], EX_LOCKED);

			act("You pick the lock on $p.", ch, obj, nullptr, TO_CHAR);
			act("$n picks the lock on $p.", ch, obj, nullptr, TO_ROOM);
			check_improve(ch, gsn_pick_lock, true, 2);
			return;
		}

		/* 'pick object' */
		if (obj->item_type != ITEM_CONTAINER)
		{
			send_to_char("That's not a container.\n\r", ch);
			return;
		}

		if (!IS_SET_OLD(obj->value[1], CONT_CLOSED))
		{
			send_to_char("It's not closed.\n\r", ch);
			return;
		}

		if (obj->value[2] < 0)
		{
			send_to_char("It can't be unlocked.\n\r", ch);
			return;
		}

		if (!IS_SET_OLD(obj->value[1], CONT_LOCKED))
		{
			send_to_char("It's already unlocked.\n\r", ch);
			return;
		}

		if (IS_SET_OLD(obj->value[1], CONT_PICKPROOF))
		{
			send_to_char("You failed.\n\r", ch);
			return;
		}

		REMOVE_BIT_OLD(obj->value[1], CONT_LOCKED);

		act("You pick the lock on $p.", ch, obj, nullptr, TO_CHAR);
		act("$n picks the lock on $p.", ch, obj, nullptr, TO_ROOM);
		check_improve(ch, gsn_pick_lock, true, 2);
		return;
	}

	auto door = find_door(ch, arg);
	if (door < 0)
		return;

	/* 'pick door' */
	auto pexit = ch->in_room->exit[door];
	if (!IS_SET(pexit->exit_info, EX_CLOSED) && !is_immortal(ch))
	{
		send_to_char("It's not closed.\n\r", ch);
		return;
	}

	if (pexit->key < 0 && !is_immortal(ch))
	{
		send_to_char("It can't be picked.\n\r", ch);
		return;
	}

	if (!IS_SET(pexit->exit_info, EX_LOCKED))
	{
		send_to_char("It's already unlocked.\n\r", ch);
		return;
	}

	if (IS_SET(pexit->exit_info, EX_PICKPROOF) && !is_immortal(ch))
	{
		send_to_char("You failed.\n\r", ch);
		return;
	}

	REMOVE_BIT(pexit->exit_info, EX_LOCKED);

	send_to_char("*Click*\n\r", ch);
	act("$n picks the $T.", ch, nullptr, pexit->keyword, TO_ROOM);
	check_improve(ch, gsn_pick_lock, true, 2);

	/* pick the other side */
	auto to_room = pexit->u1.to_room;
	auto pexit_rev = to_room->exit[rev_dir[door]];
	if (to_room != nullptr && pexit_rev != nullptr && pexit_rev->u1.to_room == ch->in_room)
	{
		REMOVE_BIT(pexit_rev->exit_info, EX_LOCKED);
	}
}

void do_stand(CHAR_DATA *ch, char *argument)
{
	OBJ_DATA *obj = nullptr;

	if (argument[0] != '\0')
	{
		if (ch->position == POS_FIGHTING)
		{
			send_to_char("Maybe you should finish fighting first?\n\r", ch);
			return;
		}

		obj = get_obj_list(ch, argument, ch->in_room->contents);
		if (obj == nullptr)
		{
			send_to_char("You don't see that here.\n\r", ch);
			return;
		}

		if (obj->item_type != ITEM_FURNITURE
			|| (!IS_SET_OLD(obj->value[2], STAND_AT)
				&& !IS_SET_OLD(obj->value[2], STAND_ON)
				&& !IS_SET_OLD(obj->value[2], STAND_IN)))
		{
			send_to_char("You can't seem to find a place to stand.\n\r", ch);
			return;
		}

		if (ch->on != obj && count_users(obj) >= obj->value[0])
		{
			act_new("There's no room to stand on $p.", ch, obj, nullptr, TO_CHAR, POS_DEAD);
			return;
		}

		ch->on = obj;
	}

	switch (ch->position)
	{
		case POS_SLEEPING:
			if (is_affected_by(ch, AFF_SLEEP))
			{
				send_to_char("You can't wake up!\n\r", ch);
				return;
			}

			if (obj == nullptr)
			{
				if (is_affected(ch, gsn_camp))
				{
					send_to_char("You break camp feeling more refreshed.\n\r", ch);
					affect_strip(ch, gsn_camp);
				}

				send_to_char("You wake and stand up.\n\r", ch);
				act("$n wakes and stands up.", ch, nullptr, nullptr, TO_ROOM);
				ch->on = nullptr;
			}
			else if (IS_SET_OLD(obj->value[2], STAND_AT))
			{
				act_new("You wake and stand at $p.", ch, obj, nullptr, TO_CHAR, POS_DEAD);
				act("$n wakes and stands at $p.", ch, obj, nullptr, TO_ROOM);
			}
			else if (IS_SET_OLD(obj->value[2], STAND_ON))
			{
				act_new("You wake and stand on $p.", ch, obj, nullptr, TO_CHAR, POS_DEAD);
				act("$n wakes and stands on $p.", ch, obj, nullptr, TO_ROOM);
			}
			else
			{
				act_new("You wake and stand in $p.", ch, obj, nullptr, TO_CHAR, POS_DEAD);
				act("$n wakes and stands in $p.", ch, obj, nullptr, TO_ROOM);
			}

			ch->position = POS_STANDING;
			break;
		case POS_RESTING:
		case POS_SITTING:
			if (obj == nullptr)
			{
				send_to_char("You stand up.\n\r", ch);
				act("$n stands up.", ch, nullptr, nullptr, TO_ROOM);
				ch->on = nullptr;
			}
			else if (IS_SET_OLD(obj->value[2], STAND_AT))
			{
				act("You stand at $p.", ch, obj, nullptr, TO_CHAR);
				act("$n stands at $p.", ch, obj, nullptr, TO_ROOM);
			}
			else if (IS_SET_OLD(obj->value[2], STAND_ON))
			{
				act("You stand on $p.", ch, obj, nullptr, TO_CHAR);
				act("$n stands on $p.", ch, obj, nullptr, TO_ROOM);
			}
			else
			{
				act("You stand in $p.", ch, obj, nullptr, TO_CHAR);
				act("$n stands on $p.", ch, obj, nullptr, TO_ROOM);
			}

			ch->position = POS_STANDING;
			break;
		case POS_STANDING:
			send_to_char("You are already standing.\n\r", ch);
			break;
		case POS_FIGHTING:
			send_to_char("You are already fighting!\n\r", ch);
			break;
	}
}

void do_rest(CHAR_DATA *ch, char *argument)
{
	OBJ_DATA *obj = nullptr;

	if (ch->position == POS_FIGHTING)
	{
		send_to_char("You are already fighting!\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_rage))
	{
		send_to_char("You cannot rest while in a berserker rage!\n\r", ch);
		return;
	}

	/* okay, now that we know we can rest, find an object to rest on */
	if (argument[0] != '\0')
	{
		obj = get_obj_list(ch, argument, ch->in_room->contents);
		if (obj == nullptr)
		{
			send_to_char("You don't see that here.\n\r", ch);
			return;
		}
	}
	else
	{
		obj = ch->on;
	}

	if (obj != nullptr)
	{
		if (obj->item_type != ITEM_FURNITURE
			|| (!IS_SET_OLD(obj->value[2], REST_ON)
				&& !IS_SET_OLD(obj->value[2], REST_IN)
				&& !IS_SET_OLD(obj->value[2], LOUNGE_ON)
				&& !IS_SET_OLD(obj->value[2], REST_AT)))
		{
			send_to_char("You can't rest on that.\n\r", ch);
			return;
		}

		if (obj != nullptr && ch->on != obj && count_users(obj) >= obj->value[0])
		{
			act_new("There's no more room on $p.", ch, obj, nullptr, TO_CHAR, POS_DEAD);
			return;
		}

		ch->on = obj;
	}

	switch (ch->position)
	{
		case POS_SLEEPING:
			if (is_affected_by(ch, AFF_SLEEP) || is_affected(ch, gsn_blackjack))
			{
				send_to_char("You can't wake up!\n\r", ch);
				return;
			}

			if (obj == nullptr)
			{
				if (is_affected(ch, gsn_camp))
				{
					send_to_char("You break camp feeling more refreshed.\n\r", ch);
					affect_strip(ch, gsn_camp);
				}

				send_to_char("You wake up and start resting.\n\r", ch);
				act("$n wakes up and starts resting.", ch, nullptr, nullptr, TO_ROOM);
			}
			else if (IS_SET_OLD(obj->value[2], REST_AT))
			{
				act_new("You wake up and rest at $p.", ch, obj, nullptr, TO_CHAR, POS_SLEEPING);
				act("$n wakes up and rests at $p.", ch, obj, nullptr, TO_ROOM);
			}
			else if (IS_SET_OLD(obj->value[2], REST_ON))
			{
				act_new("You wake up and rest on $p.", ch, obj, nullptr, TO_CHAR, POS_SLEEPING);
				act("$n wakes up and rests on $p.", ch, obj, nullptr, TO_ROOM);
			}
			else if (IS_SET_OLD(obj->value[2], LOUNGE_ON))
			{
				act_new("You wake up and lounge on $p.", ch, obj, nullptr, TO_CHAR, POS_SLEEPING);
				act("$n wakes up and lounge on $p.", ch, obj, nullptr, TO_ROOM);
			}
			else
			{
				act_new("You wake up and rest in $p.", ch, obj, nullptr, TO_CHAR, POS_SLEEPING);
				act("$n wakes up and rests in $p.", ch, obj, nullptr, TO_ROOM);
			}

			ch->position = POS_RESTING;
			break;
		case POS_RESTING:
			send_to_char("You are already resting.\n\r", ch);
			break;
		case POS_STANDING:
			if (obj == nullptr)
			{
				send_to_char("You rest.\n\r", ch);
				act("$n sits down and rests.", ch, nullptr, nullptr, TO_ROOM);
			}
			else if (IS_SET_OLD(obj->value[2], REST_AT))
			{
				act("You sit down at $p and rest.", ch, obj, nullptr, TO_CHAR);
				act("$n sits down at $p and rests.", ch, obj, nullptr, TO_ROOM);
			}
			else if (IS_SET_OLD(obj->value[2], REST_ON))
			{
				act("You sit on $p and rest.", ch, obj, nullptr, TO_CHAR);
				act("$n sits on $p and rests.", ch, obj, nullptr, TO_ROOM);
			}
			else if (IS_SET_OLD(obj->value[2], LOUNGE_ON))
			{
				act("You lounge on $p and rest.", ch, obj, nullptr, TO_CHAR);
				act("$n lounges on $p and rests.", ch, obj, nullptr, TO_ROOM);
			}
			else
			{
				act("You rest in $p.", ch, obj, nullptr, TO_CHAR);
				act("$n rests in $p.", ch, obj, nullptr, TO_ROOM);
			}

			ch->position = POS_RESTING;
			break;
		case POS_SITTING:
			if (obj == nullptr)
			{
				send_to_char("You rest.\n\r", ch);
				act("$n rests.", ch, nullptr, nullptr, TO_ROOM);
			}
			else if (IS_SET_OLD(obj->value[2], REST_AT))
			{
				act("You rest at $p.", ch, obj, nullptr, TO_CHAR);
				act("$n rests at $p.", ch, obj, nullptr, TO_ROOM);
			}
			else if (IS_SET_OLD(obj->value[2], REST_ON))
			{
				act("You rest on $p.", ch, obj, nullptr, TO_CHAR);
				act("$n rests on $p.", ch, obj, nullptr, TO_ROOM);
			}
			else if (IS_SET_OLD(obj->value[2], LOUNGE_ON))
			{
				act("You lounge on $p and rest.", ch, obj, nullptr, TO_CHAR);
				act("$n lounge on $p and rests.", ch, obj, nullptr, TO_ROOM);
			}
			else
			{
				act("You rest in $p.", ch, obj, nullptr, TO_CHAR);
				act("$n rests in $p.", ch, obj, nullptr, TO_ROOM);
			}

			ch->position = POS_RESTING;
			break;
	}
}

void do_sit(CHAR_DATA *ch, char *argument)
{
	OBJ_DATA *obj = nullptr;

	if (ch->position == POS_FIGHTING)
	{
		send_to_char("Maybe you should finish this fight first?\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_rage))
	{
		send_to_char("You cannot sleep while in a berserker rage!\n\r", ch);
		return;
	}

	/* okay, now that we know we can sit, find an object to sit on */
	if (argument[0] != '\0')
	{
		obj = get_obj_list(ch, argument, ch->in_room->contents);

		if (obj == nullptr)
		{
			send_to_char("You don't see that here.\n\r", ch);
			return;
		}
	}
	else
	{
		obj = ch->on;
	}

	if (obj != nullptr)
	{
		if (obj->item_type != ITEM_FURNITURE ||
			(!IS_SET_OLD(obj->value[2], SIT_ON) && !IS_SET_OLD(obj->value[2], SIT_IN) &&
			 !IS_SET_OLD(obj->value[2], SIT_AT)))
		{
			send_to_char("You can't sit on that.\n\r", ch);
			return;
		}

		if (obj != nullptr && ch->on != obj && count_users(obj) >= obj->value[0])
		{
			act_new("There's no more room on $p.", ch, obj, nullptr, TO_CHAR, POS_DEAD);
			return;
		}

		ch->on = obj;
	}

	switch (ch->position)
	{
		case POS_SLEEPING:
			if (is_affected_by(ch, AFF_SLEEP) || is_affected(ch, gsn_blackjack))
			{
				send_to_char("You can't wake up!\n\r", ch);
				return;
			}

			if (obj == nullptr)
			{
				send_to_char("You wake and sit up.\n\r", ch);
				act("$n wakes and sits up.", ch, nullptr, nullptr, TO_ROOM);
			}
			else if (IS_SET_OLD(obj->value[2], SIT_AT))
			{
				act_new("You wake and sit at $p.", ch, obj, nullptr, TO_CHAR, POS_DEAD);
				act("$n wakes and sits at $p.", ch, obj, nullptr, TO_ROOM);
			}
			else if (IS_SET_OLD(obj->value[2], SIT_ON))
			{
				act_new("You wake and sit on $p.", ch, obj, nullptr, TO_CHAR, POS_DEAD);
				act("$n wakes and sits at $p.", ch, obj, nullptr, TO_ROOM);
			}
			else
			{
				act_new("You wake and sit in $p.", ch, obj, nullptr, TO_CHAR, POS_DEAD);
				act("$n wakes and sits in $p.", ch, obj, nullptr, TO_ROOM);
			}

			ch->position = POS_SITTING;
			break;
		case POS_RESTING:
			if (obj == nullptr)
				send_to_char("You stop resting.\n\r", ch);
			else if (IS_SET_OLD(obj->value[2], SIT_AT))
			{
				act("You sit at $p.", ch, obj, nullptr, TO_CHAR);
				act("$n sits at $p.", ch, obj, nullptr, TO_ROOM);
			}

			else if (IS_SET_OLD(obj->value[2], SIT_ON))
			{
				act("You sit on $p.", ch, obj, nullptr, TO_CHAR);
				act("$n sits on $p.", ch, obj, nullptr, TO_ROOM);
			}

			ch->position = POS_SITTING;
			break;
		case POS_SITTING:
			send_to_char("You are already sitting down.\n\r", ch);
			break;
		case POS_STANDING:
			if (obj == nullptr)
			{
				send_to_char("You sit down.\n\r", ch);
				act("$n sits down on the ground.", ch, nullptr, nullptr, TO_ROOM);
			}
			else if (IS_SET_OLD(obj->value[2], SIT_AT))
			{
				act("You sit down at $p.", ch, obj, nullptr, TO_CHAR);
				act("$n sits down at $p.", ch, obj, nullptr, TO_ROOM);
			}
			else if (IS_SET_OLD(obj->value[2], SIT_ON))
			{
				act("You sit on $p.", ch, obj, nullptr, TO_CHAR);
				act("$n sits on $p.", ch, obj, nullptr, TO_ROOM);
			}
			else
			{
				act("You sit down in $p.", ch, obj, nullptr, TO_CHAR);
				act("$n sits down in $p.", ch, obj, nullptr, TO_ROOM);
			}

			ch->position = POS_SITTING;
			break;
	}
}

void do_sleep(CHAR_DATA *ch, char *argument)
{
	OBJ_DATA *obj = nullptr;

	if (is_affected(ch, gsn_rage))
	{
		send_to_char("You cannot sleep while in a berserker rage!\n\r", ch);
		return;
	}

	if (ch->pause > 5)
	{
		send_to_char("You are too excited to sleep!\n\r", ch);
		return;
	}

	switch (ch->position)
	{
		case POS_SLEEPING:
			send_to_char("You are already sleeping.\n\r", ch);
			break;
		case POS_RESTING:
		case POS_SITTING:
		case POS_STANDING:
			if (argument[0] == '\0' && ch->on == nullptr)
			{
				send_to_char("You go to sleep.\n\r", ch);
				act("$n goes to sleep.", ch, nullptr, nullptr, TO_ROOM);

				ch->position = POS_SLEEPING;
			}
			else /* find an object and sleep on it */
			{
				if (argument[0] == '\0')
					obj = ch->on;
				else
					obj = get_obj_list(ch, argument, ch->in_room->contents);

				if (obj == nullptr)
				{
					send_to_char("You don't see that here.\n\r", ch);
					return;
				}

				if (obj->item_type != ITEM_FURNITURE
					|| (!IS_SET_OLD(obj->value[2], SLEEP_ON)
						&& !IS_SET_OLD(obj->value[2], SLEEP_IN)
						&& !IS_SET_OLD(obj->value[2], SLEEP_AT)))
				{
					send_to_char("You can't sleep on that!\n\r", ch);
					return;
				}

				if (ch->on != obj && count_users(obj) >= obj->value[0])
				{
					act_new("There is no room on $p for you.", ch, obj, nullptr, TO_CHAR, POS_DEAD);
					return;
				}

				ch->on = obj;
				if (IS_SET_OLD(obj->value[2], SLEEP_AT))
				{
					act("You go to sleep at $p.", ch, obj, nullptr, TO_CHAR);
					act("$n goes to sleep at $p.", ch, obj, nullptr, TO_ROOM);
				}
				else if (IS_SET_OLD(obj->value[2], SLEEP_ON))
				{
					act("You go to sleep on $p.", ch, obj, nullptr, TO_CHAR);
					act("$n goes to sleep on $p.", ch, obj, nullptr, TO_ROOM);
				}
				else
				{
					act("You go to sleep in $p.", ch, obj, nullptr, TO_CHAR);
					act("$n goes to sleep in $p.", ch, obj, nullptr, TO_ROOM);
				}
				ch->position = POS_SLEEPING;
			}
			break;
		case POS_FIGHTING:
			send_to_char("You are already fighting!\n\r", ch);
			break;
	}
}

void do_wake(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	one_argument(argument, arg);

	if (arg[0] == '\0')
	{
		do_stand(ch, argument);
		return;
	}

	if (!is_awake(ch))
	{
		send_to_char("You are asleep yourself!\n\r", ch);
		return;
	}

	auto victim = get_char_room(ch, arg);
	if (victim == nullptr)
	{
		send_to_char("They aren't here.\n\r", ch);
		return;
	}

	if (is_awake(victim))
	{
		act("$N is already awake.", ch, nullptr, victim, TO_CHAR);
		return;
	}

	if (is_affected_by(victim, AFF_SLEEP))
	{
		act("You can't wake $M!", ch, nullptr, victim, TO_CHAR);
		return;
	}

	act("You wake $N.", ch, 0, victim, TO_CHAR);
	act("$n wakes $N.", ch, 0, victim, TO_NOTVICT);
	act_new("$n wakes you.", ch, nullptr, victim, TO_VICT, POS_SLEEPING);

	do_stand(victim, "");
}

void do_sneak(CHAR_DATA *ch, char *argument)
{
	auto sn_fog = skill_lookup("faerie fog");
	auto sn_fire = skill_lookup("faerie fire");

	if (is_affected(ch, sn_fog)
		|| is_affected(ch, sn_fire)
		|| is_affected(ch, gsn_incandescense)
		|| is_affected(ch, gsn_mana_sickness))
	{
		send_to_char("You can't hide while glowing.\n\r", ch);
		return;
	}

	if (ch->in_room->sector_type == SECT_AIR
		|| ch->in_room->sector_type == SECT_VERTICAL
		|| ch->in_room->sector_type == SECT_WATER
		|| ch->in_room->sector_type == SECT_UNDERWATER)
	{
		send_to_char("You need some solid ground beneath you to sneak properly.\n\r", ch);
		return;
	}

	send_to_char("You attempt to move silently.\n\r", ch);

	if (is_affected_by(ch, AFF_SNEAK))
		return;

	if (number_percent() >= get_skill(ch, gsn_sneak))
	{
		check_improve(ch, gsn_sneak, false, 3);
		return;
	}

	check_improve(ch, gsn_sneak, true, 3);

	AFFECT_DATA af;
	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_INVIS;
	af.type = gsn_sneak;
	af.level = ch->level;
	af.duration = ch->level;
	af.location = APPLY_NONE;
	af.modifier = 0;

	SET_BIT(af.bitvector, AFF_SNEAK);

	affect_to_char(ch, &af);
}

void do_cloak(CHAR_DATA *ch, char *argument)
{
	/*
	AFFECT_DATA af;
	char buf[MAX_STRING_LENGTH];
	char arg[MAX_STRING_LENGTH];

	argument = one_argument(argument,arg);

	if (is_affected(ch,gsn_cloak_form))
	{
		if(is_affected(ch, gsn_cloak_form))
		{
			affect_strip(ch,gsn_cloak_form);
		}

		if(ch->original_name)
		{
			free_pstring(ch->name);
			ch->name=palloc_string(ch->original_name);
			free_pstring(ch->original_name);
			ch->original_name=nullptr;
		}

		if(is_affected(ch, gsn_eye_of_the_predator))
		{
			affect_strip(ch,gsn_eye_of_the_predator);
		}

		if (str_cmp(arg,"auto"))
		{
			send_to_char("You throw your cloak to the side, exposing yourself.\n\r",ch);
			return;
		}

		if(IS_SHIFTED(ch))
		{
			send_to_char("You cannot cloak while an animal.\n\r",ch);
			return;
		}

		if (number_percent() < get_skill(ch,gsn_cloak_form))
		{
			init_affect(&af);
			af.where    = TO_AFFECTS;
			af.aftype    = AFT_POWER;
			af.type     = gsn_cloak_form;
			af.level    = ch->level;
			af.location = APPLY_NONE;
			af.modifier = 0;
			af.bitvector = AFF_SNEAK;
			af.duration = -1;
			affect_to_char( ch, &af );

			if(!(ch->original_name))
			{
				free_pstring(ch->original_name);
				ch->original_name=palloc_string(ch->name);
				free_pstring(ch->name);
				sprintf(buf, "cloaked figure");
				ch->name=palloc_string(buf);
			}

			af.location = APPLY_HIT;
			af.modifier = ch->level*5;
			affect_to_char(ch,&af);

			af.location = APPLY_MOVE;
			af.modifier = ch->level*2;
			affect_to_char(ch,&af);

			af.location = APPLY_MANA;
			af.modifier = ch->level*2;
			affect_to_char(ch,&af);

			check_improve(ch,gsn_cloak_form,true,6);

			send_to_char("You cloak your presence.\n\r",ch);
			return;
		}
		else
		{
			check_improve(ch,gsn_cloak_form, false,3);
			send_to_char("You fail to cloak yourself.\n\r",ch);
			return;
		}
	}
	*/
}

/*
void do_vigilance(CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;

	if ((get_skill(ch,gsn_vigilance) == 0) || (ch->level < skill_table[gsn_vigilance].skill_level[ch->Class()->GetIndex()]))
	{
		send_to_char("Huh?\n\r",ch);
		return;
	}

	if (is_affected(ch,gsn_vigilance) )
	{
		send_to_char("You are already as vigilant as you can be.\n\r",ch);
		return;
	}

	if (ch->mana < 30)
	{
		send_to_char("You do not have the mana.\n\r",ch);
		return;
	}

	if (number_percent() > ch->pcdata->learned[gsn_vigilance])
	{
		send_to_char("You try to watch things more carefully but lose concentration.\n\r",ch);
		check_improve(ch,gsn_vigilance,false,1);
		ch->mana -= 15;
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_POWER;
	af.level = ch->level;
	af.location = 0;
	af.duration = 48;
	af.modifier = 0;
	af.type = gsn_vigilance;
	af.bitvector = AFF_DETECT_INVIS;
	affect_to_char(ch,&af);

	af.bitvector = AFF_DETECT_MAGIC;
	affect_to_char(ch,&af);

	af.bitvector = AFF_DARK_VISION;
	affect_to_char(ch,&af);

	ch->mana -= 30;

	send_to_char("Your eyes tingle.\n\r",ch);
	send_to_char("You are more sensitive to the prescence of magical auras.\n\r",ch);
	return;
}
*/

void do_acute_vision(CHAR_DATA *ch, char *argument)
{
	if (get_skill(ch, gsn_acute_vision) == 0 ||
		ch->level < skill_table[gsn_acute_vision].skill_level[ch->Class()->GetIndex()])
	{
		send_to_char("You don't know which bushes to look at.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_acute_vision))
	{
		send_to_char("Your eyes are already as sharp as they'll get.\n\r", ch);
		return;
	}

	if (ch->mana < 25)
	{
		send_to_char("You don't have the mana.\n\r", ch);
		return;
	}

	if (number_percent() > get_skill(ch, gsn_acute_vision))
	{
		send_to_char("Your vision sharpens but then dulls.\n\r", ch);
		check_improve(ch, gsn_acute_vision, false, 1);
		ch->mana -= 12;
		return;
	}

	AFFECT_DATA af;
	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SKILL;
	af.type = gsn_acute_vision;
	af.location = 0;
	af.modifier = 0;
	af.level = ch->level;
	af.duration = ch->level;

	SET_BIT(af.bitvector, AFF_DETECT_CAMO);

	af.mod_name = MOD_VISION;
	affect_to_char(ch, &af);

	ch->mana -= 25;
	send_to_char("Your vision sharpens.\n\r", ch);

	return;
}

void do_camp(CHAR_DATA *ch, char *argument)
{
	if (get_skill(ch, gsn_camp) == 0 || ch->level < skill_table[gsn_camp].skill_level[ch->Class()->GetIndex()])
	{
		send_to_char("You don't know how to effectively camp.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_camp))
	{
		send_to_char("You are already camping.\n\r", ch);
		return;
	}

	if (ch->position == POS_SLEEPING)
	{
		send_to_char("You are already asleep.", ch);
		return;
	}

	if ((ch->in_room->sector_type != SECT_FOREST)
		&& (ch->in_room->sector_type != SECT_HILLS)
		&& (ch->in_room->sector_type != SECT_MOUNTAIN))
	{
		send_to_char("This land is not wild enough for you to camp out in.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_rage))
	{
		send_to_char("You don't want rest, you want blood!\n\r", ch);
		return;
	}

	AFFECT_DATA af;
	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SKILL;
	af.location = 0;
	af.duration = 5;
	af.modifier = 0;
	af.level = ch->level;
	af.type = gsn_camp;
	affect_to_char(ch, &af);

	act("$n settles down and sets up camp.", ch, nullptr, nullptr, TO_ROOM);
	send_to_char("You settle down and set up camp.\n\r", ch);

	do_sleep(ch, "");
}

void do_camouflage(CHAR_DATA *ch, char *argument)
{
	auto sn_fog = skill_lookup("faerie fog");
	auto sn_fire = skill_lookup("faerie fire");

	if (is_affected(ch, sn_fog) || is_affected(ch, sn_fire) || is_affected(ch, gsn_incandescense))
	{
		send_to_char("You can't camouflage while glowing.\n\r", ch);
		return;
	}

	if (!is_immortal(ch) 
		&& (ch->in_room->sector_type != SECT_FOREST) 
		&& (ch->in_room->sector_type != SECT_MOUNTAIN)
		&& (ch->in_room->sector_type != SECT_HILLS))
	{
		send_to_char("There is no cover here.\n\r", ch);
		act("$n tries to cover $mself on the single leaf on the ground.", ch, nullptr, nullptr, TO_ROOM);
		return;
	}

	send_to_char("You attempt to blend in with your surroundings.\n\r", ch);

	if (number_percent() < get_skill(ch, gsn_camouflage))
	{
		SET_BIT(ch->affected_by, AFF_CAMOUFLAGE);
		check_improve(ch, gsn_camouflage, true, 2);
		WAIT_STATE(ch, 8);
	}
	else
	{
		check_improve(ch, gsn_camouflage, false, 2);
		REMOVE_BIT(ch->affected_by, AFF_CAMOUFLAGE);
		WAIT_STATE(ch, 8);
	}
}

/* Creep - Camo sneak -- Dioxide */

void do_creep(CHAR_DATA *ch, char *argument)
{
	auto chance = get_skill(ch, gsn_creep);
	if (chance == 0)
	{
		send_to_char("Creeping? What's that?\n\r", ch);
		return;
	}

	if (!is_affected_by(ch, AFF_CAMOUFLAGE))
	{
		send_to_char("You must be blended in with your surroundings to creep.\n\r", ch);
		return;
	}

	if (argument[0] == '\0')
	{
		send_to_char("Creep in which direction?\n\r", ch);
		return;
	}

	int direction = 0;
	if (!str_cmp(argument, "n") || !str_cmp(argument, "north"))
	{
		direction = 0;
	}
	else if (!str_cmp(argument, "e") || !str_cmp(argument, "east"))
	{
		direction = 1;
	}
	else if (!str_cmp(argument, "s") || !str_cmp(argument, "south"))
	{
		direction = 2;
	}
	else if (!str_cmp(argument, "w") || !str_cmp(argument, "west"))
	{
		direction = 3;
	}
	else if (!str_cmp(argument, "u") || !str_cmp(argument, "up"))
	{
		direction = 4;
	}
	else if (!str_cmp(argument, "d") || !str_cmp(argument, "down"))
	{
		direction = 5;
	}
	else
	{
		act("You can't creep there.", ch, nullptr, 0, TO_CHAR);
	}

	AFFECT_DATA af;
	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SKILL;
	af.type = gsn_creep;
	af.modifier = 0;
	af.location = 0;
	af.duration = 1;
	af.level = ch->level;
	affect_join(ch, &af);

	move_char(ch, direction, false, true);

	affect_strip(ch, gsn_creep);
	check_improve(ch, gsn_creep, true, 2);

	auto wait = 100 - chance;
	WAIT_STATE(ch, wait);
}

void do_hide(CHAR_DATA *ch, char *argument)
{
	auto sn_fog = skill_lookup("faerie fog");
	auto sn_fire = skill_lookup("faerie fire");

	if (is_affected(ch, sn_fog)
		|| is_affected(ch, sn_fire)
		|| is_affected(ch, gsn_incandescense)
		|| is_affected(ch, gsn_mana_sickness))
	{
		send_to_char("You can't hide while glowing.\n\r", ch);
		return;
	}

	if (ch->in_room->sector_type != SECT_CITY
		&& ch->in_room->sector_type != SECT_INSIDE
		&& ch->in_room->sector_type != SECT_ROAD
		&& !is_immortal(ch))
	{
		send_to_char("The shadows here are too natural to blend with.\n\r", ch);
		return;
	}

	if (ch->Class()->GetIndex() == CLASS_ASSASSIN && ch->pause > 5)
	{
		send_to_char("You're breathing too heavily to hide.\n\r", ch);
		return;
	}

	send_to_char("You attempt to hide.\n\r", ch);

	if (is_affected_by(ch, AFF_HIDE))
		return;

	if (number_percent() < get_skill(ch, gsn_hide))
	{
		SET_BIT(ch->affected_by, AFF_HIDE);
		check_improve(ch, gsn_hide, true, 3);
		return;
	}

	check_improve(ch, gsn_hide, false, 3);
}

void un_camouflage(CHAR_DATA *ch, char *argument)
{
	if (!is_affected_by(ch, AFF_CAMOUFLAGE))
		return;

	REMOVE_BIT(ch->affected_by, AFF_CAMOUFLAGE);

	act("$n steps out from their cover.", ch, nullptr, nullptr, TO_ROOM);
	send_to_char("You step out from your cover.\n\r", ch);
}

void un_blackjack(CHAR_DATA *ch, char *argument)
{
	if (is_affected(ch, gsn_blackjack))
		affect_strip(ch, gsn_blackjack);
}

void un_strangle(CHAR_DATA *ch, char *argument)
{
	if (is_affected(ch, gsn_strangle))
		affect_strip(ch, gsn_strangle);
}

void un_hide(CHAR_DATA *ch, char *argument)
{
	if (!is_affected_by(ch, AFF_HIDE))
		return;

	REMOVE_BIT(ch->affected_by, AFF_HIDE);

	act("$n steps out of the shadows.", ch, nullptr, nullptr, TO_ROOM);
	send_to_char("You step out of the shadows.\n\r", ch);
}

void un_invis(CHAR_DATA *ch, char *argument)
{
	if (!is_affected_by(ch, AFF_INVISIBLE))
		return;

	affect_strip(ch, gsn_invis);
	affect_strip(ch, gsn_mass_invis);

	REMOVE_BIT(ch->affected_by, AFF_INVISIBLE);

	act("$n fades into existence.", ch, nullptr, nullptr, TO_ROOM);
	send_to_char("You fade into existence.\n\r", ch);
}

void un_sneak(CHAR_DATA *ch, char *argument)
{
	if (IS_SET(race_table[ch->race].aff, AFF_SNEAK))
		return;

	if (!is_affected_by(ch, AFF_SNEAK) && !is_affected(ch, gsn_silent_movement))
		return;

	if (is_affected(ch, gsn_silent_movement))
	{
		affect_strip(ch, gsn_silent_movement);
	}
	else
	{
		affect_strip(ch, gsn_sneak);

		REMOVE_BIT(ch->affected_by, AFF_SNEAK);
	}

	send_to_char("You trample around loudly again.\n\r", ch);
}

void un_earthfade(CHAR_DATA *ch, char *argument)
{
	if (!is_affected(ch, gsn_earthfade))
		return;

	affect_strip(ch, gsn_earthfade);

	act("$n slowly emerges from the ground.", ch, 0, 0, TO_ROOM);
	act("You slowly emerge from the ground.", ch, 0, 0, TO_CHAR);
	
	WAIT_STATE(ch, 3 * PULSE_VIOLENCE);
}

void un_blade_barrier(CHAR_DATA *ch, char *argument)
{
	if (!is_affected(ch, gsn_blade_barrier))
		return;

	affect_strip(ch, gsn_blade_barrier);

	act("The whirling blades around $n vanish.", ch, 0, 0, TO_ROOM);
	act("The whirling blades around you vanish.", ch, 0, 0, TO_CHAR);

	WAIT_STATE(ch, PULSE_VIOLENCE);
}

void un_watermeld(CHAR_DATA *ch, char *argument)
{
	if (!is_affected(ch, gsn_watermeld))
		return;

	affect_strip(ch, gsn_watermeld);

	act("The water swirls around you as your concealment fails you.", ch, 0, 0, TO_CHAR);
}

void un_shroud(CHAR_DATA *ch, char *argument)
{
	if (is_affected(ch, gsn_shroud_of_secrecy))
	{
		affect_strip(ch, gsn_shroud_of_secrecy);
	}
}

void do_visible(CHAR_DATA *ch, char *argument)
{
	un_camouflage(ch, nullptr);
	un_hide(ch, nullptr);
	un_invis(ch, nullptr);
	un_sneak(ch, nullptr);
	un_ultradiffusion(ch, nullptr);
	un_watermeld(ch, nullptr);
	un_earthfade(ch, nullptr);
	un_shroud(ch, nullptr);
}

void do_recall(CHAR_DATA *ch, char *argument)
{
	if (is_npc(ch) && !IS_SET(ch->act, ACT_PET))
	{
		send_to_char("Only players can recall.\n\r", ch);
		return;
	}

	if (ch->level > 15 && !is_immortal(ch) && !ch->ghost)
	{
		send_to_char("Those who have passed the 15th rank cannot recall.\n\r", ch);
		return;
	}

	act("$n prays for transportation!", ch, 0, 0, TO_ROOM);

	auto location = get_room_index(hometown_table[ch->hometown].recall);
	if (location == nullptr)
	{
		send_to_char("Your temple has been destroyed...pray to the Gods for help!\n\r", ch);
		return;
	}

	if (ch->in_room == location)
		return;

	if (IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL) || is_affected_by(ch, AFF_CURSE))
	{
		send_to_char("The gods have forsaken you.\n\r", ch);
		return;
	}

	auto victim = ch->fighting;
	if (victim != nullptr)
	{
		char buf[MAX_STRING_LENGTH];
		auto skill = get_skill(ch, gsn_recall);

		if (number_percent() < 80 * skill / 100)
		{
			check_improve(ch, gsn_recall, false, 6);

			WAIT_STATE(ch, 4);

			sprintf(buf, "You failed!\n\r");
			send_to_char(buf, ch);
			return;
		}

		check_improve(ch, gsn_recall, true, 4);
		sprintf(buf, "You recall from combat!\n\r");
		send_to_char(buf, ch);
		stop_fighting(ch, true);
	}

	un_watermeld(ch, nullptr);

	ch->move /= 2;

	act("$n disappears.", ch, nullptr, nullptr, TO_ROOM);

	char_from_room(ch);
	char_to_room(ch, location);

	act("$n appears in the room.", ch, nullptr, nullptr, TO_ROOM);

	do_look(ch, "auto");

	if (ch->pet != nullptr)
		do_recall(ch->pet, "");
}

void do_train(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];

	if (is_npc(ch))
		return;

	/*
	 * Check for trainer.
	 */
	auto mob = ch->in_room->people;
	for (; mob != nullptr; mob = mob->next_in_room)
	{
		if (is_npc(mob) && IS_SET(mob->act, ACT_TRAIN))
			break;
	}

	if (mob == nullptr)
	{
		send_to_char("You can't do that here.\n\r", ch);
		return;
	}

	if (argument[0] == '\0')
	{
		sprintf(buf, "You have %d training sessions.\n\r", ch->train);
		send_to_char(buf, ch);
		argument = "foo";
	}

	auto cost = 1;
	char *pOutput = nullptr;
	short stat = -1;

	if (!str_cmp(argument, "str"))
	{
		stat = STAT_STR;
		pOutput = "strength";
	}
	else if (!str_cmp(argument, "int"))
	{
		stat = STAT_INT;
		pOutput = "intelligence";
	}
	else if (!str_cmp(argument, "wis"))
	{
		stat = STAT_WIS;
		pOutput = "wisdom";
	}
	else if (!str_cmp(argument, "dex"))
	{
		stat = STAT_DEX;
		pOutput = "dexterity";
	}
	else if (!str_cmp(argument, "con"))
	{
		stat = STAT_CON;
		pOutput = "constitution";
	}
	else if (!str_cmp(argument, "hp"))
	{
		cost = 1;
	}
	else if (!str_cmp(argument, "mana"))
	{
		cost = 1;
	}
	else
	{
		strcpy(buf, "You can train:");

		if (ch->perm_stat[STAT_STR] < get_max_train(ch, STAT_STR))
			strcat(buf, " str");

		if (ch->perm_stat[STAT_INT] < get_max_train(ch, STAT_INT))
			strcat(buf, " int");

		if (ch->perm_stat[STAT_WIS] < get_max_train(ch, STAT_WIS))
			strcat(buf, " wis");

		if (ch->perm_stat[STAT_DEX] < get_max_train(ch, STAT_DEX))
			strcat(buf, " dex");

		if (ch->perm_stat[STAT_CON] < get_max_train(ch, STAT_CON))
			strcat(buf, " con");

		strcat(buf, " hp mana");

		strcat(buf, ".\n\r");
		send_to_char(buf, ch);

		return;
	}

	if (!str_cmp("hp", argument))
	{
		if (cost > ch->train)
		{
			send_to_char("You don't have enough training sessions.\n\r", ch);
			return;
		}

		ch->train -= cost;
		ch->pcdata->perm_hit += 10;
		ch->max_hit += 10;
		ch->hit += 10;

		act("Your durability increases!", ch, nullptr, nullptr, TO_CHAR);
		act("$n's durability increases!", ch, nullptr, nullptr, TO_ROOM);
		return;
	}

	if (!str_cmp("mana", argument))
	{
		if (cost > ch->train)
		{
			send_to_char("You don't have enough training sessions.\n\r", ch);
			return;
		}

		ch->train -= cost;
		ch->pcdata->perm_mana += 10;
		ch->max_mana += 10;
		ch->mana += 10;

		act("Your power increases!", ch, nullptr, nullptr, TO_CHAR);
		act("$n's power increases!", ch, nullptr, nullptr, TO_ROOM);
		return;
	}

	if (ch->perm_stat[stat] >= get_max_train(ch, stat))
	{
		act("Your $T is already at maximum.", ch, nullptr, pOutput, TO_CHAR);
		return;
	}

	if (cost > ch->train)
	{
		send_to_char("You don't have enough training sessions.\n\r", ch);
		return;
	}

	ch->train -= cost;

	ch->perm_stat[stat] += 1;
	act("Your $T increases!", ch, nullptr, pOutput, TO_CHAR);
	act("$n's $T increases!", ch, nullptr, pOutput, TO_ROOM);
}

void do_bear_call(CHAR_DATA *ch, char *argument)
{
	char arg1[MAX_INPUT_LENGTH];
	one_argument(argument, arg1);

	if (get_skill(ch, gsn_bear_call) == 0 || ch->level < skill_table[gsn_bear_call].skill_level[ch->Class()->GetIndex()])
	{
		send_to_char("You don't know how to call bears?\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_bear_call))
	{
		send_to_char("You can't call more bears yet.\n\r", ch);
		return;
	}

	if (ch->mana <= 70)
	{
		send_to_char("You do not have the mana.\n\r", ch);
		return;
	}

	auto found = false;
	for (auto check = char_list; check != nullptr; check = check->next)
	{
		if (check->master == ch && check->pIndexData->vnum == MOB_VNUM_BEAR)
			found = true;
	}

	if (found)
	{
		send_to_char("You already have bears to care for.\n\r", ch);
		return;
	}

	if (ch->in_room->sector_type != SECT_FOREST
		&& ch->in_room->sector_type != SECT_HILLS
		&& ch->in_room->sector_type != SECT_MOUNTAIN)
	{
		act("$n calls out into the surroundings but nothing comes.", ch, 0, 0, TO_ROOM);
		send_to_char("You call a bear call but nothing responds.\n\r", ch);
		return;
	}

	if (number_percent() > ch->pcdata->learned[gsn_bear_call])
	{
		send_to_char("You call out for bears but none respond.\n\r", ch);
		check_improve(ch, gsn_bear_call, false, 1);

		ch->mana -= 35;
		return;
	}

	ch->mana -= 70;
	auto a_level = ch->level;

	act("$n calls out to the wild and is heard!", ch, 0, 0, TO_ROOM);
	send_to_char("Your call to the wild is heard!\n\r", ch);

	for (auto count = 0; count < 2; count++)
	{
		auto animal = create_mobile(get_mob_index(MOB_VNUM_BEAR));
		animal->level = a_level;
		animal->max_hit = ch->max_hit + dice(a_level, 5);
		animal->damroll += a_level * 3 / 4;
		animal->hitroll += a_level / 3;
		animal->alignment = ch->alignment;
		animal->cabal = ch->cabal;

		char_to_room(animal, ch->in_room);
		add_follower(animal, ch);

		animal->leader = ch;

		SET_BIT(animal->affected_by, AFF_CHARM);

		act("$n arrives to journey with $N.", animal, nullptr, ch, TO_NOTVICT);

		animal->hit = animal->max_hit;
	}

	AFFECT_DATA af;
	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SKILL;
	af.level = ch->level;
	af.modifier = 0;
	af.duration = 24;
	af.location = 0;
	af.type = gsn_bear_call;

	affect_to_char(ch, &af);
	check_improve(ch, gsn_bear_call, true, 1);
}

void do_animal_call(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	one_argument(argument, arg);

	auto chance = get_skill(ch, gsn_animal_call);
	if (chance == 0 || ch->level < skill_table[gsn_animal_call].skill_level[ch->Class()->GetIndex()])
	{
		send_to_char("You don't know how to call upon animals for aid.\n\r", ch);
		return;
	}

	if (arg[0] == '\0')
	{
		send_to_char("Call out for which animal?\n\r", ch);
		return;
	}

	auto type = 0;
	auto cant_call = false;
	if (!str_cmp(arg, "falcon"))
	{
		type = MOB_VNUM_FALCON;
	}
	else if (!str_cmp(arg, "wolf"))
	{
		type = MOB_VNUM_WOLF;

		if (ch->level < 25)
			cant_call = true;
	}
	else if (!str_cmp(arg, "bear"))
	{
		type = MOB_VNUM_BEAR;

		if (ch->level < 30)
			cant_call = true;
	}
	else if (!str_cmp(arg, "lion"))
	{
		type = MOB_VNUM_LION;

		if (ch->level < 35)
			cant_call = true;
	}
	else
	{
		send_to_char("call out for which animal?\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_animal_call))
	{
		send_to_char("You aren't able to call upon anymore animals yet.\n\r", ch);
		return;
	}

	if (cant_call)
	{
		send_to_char("You are not skilled enough at woodsmanship to call that animal.\n\r", ch);
		return;
	}

	if (ch->mana < 50)
	{
		send_to_char("You don't have the mana.\n\r", ch);
		return;
	}

	auto mob = char_list;
	for (; mob != nullptr; mob = mob->next)
	{
		if (is_npc(mob)
			&& is_affected_by(mob, AFF_CHARM)
			&& (mob->master == ch)
			&& ((mob->pIndexData->vnum == MOB_VNUM_FALCON)
				|| (mob->pIndexData->vnum == MOB_VNUM_WOLF)
				|| (mob->pIndexData->vnum == MOB_VNUM_BEAR)
				|| (mob->pIndexData->vnum == MOB_VNUM_LION)))
		{
			break;
		}
	}

	if (mob != nullptr)
	{
		send_to_char("You already have animals you should care for.\n\r", ch);
		return;
	}

	if (ch->in_room->sector_type != SECT_FOREST
		&& ch->in_room->sector_type != SECT_HILLS
		&& ch->in_room->sector_type != SECT_MOUNTAIN)
	{
		send_to_char("You are not within the right environment to call animals.\n\r", ch);
		return;
	}

	if (number_percent() > chance)
	{
		act("$n calls out to the wild but nothing responds.", ch, 0, 0, TO_ROOM);
		send_to_char("You call out to the wild but nothing responds.\n\r", ch);

		ch->mana -= 25;

		check_improve(ch, gsn_animal_call, false, 1);
		return;
	}

	auto pRoomIndex = ch->in_room;
	auto pMobIndex = get_mob_index(type);
	if (pMobIndex == nullptr)
	{
		bug("Animal call: Bad mob vnum call %d.", type);

		send_to_char("You call out to the wild but nothing responds.\n\r", ch);
		act("$n calls out to the wild but nothing responds.", ch, 0, 0, TO_ROOM);

		ch->mana -= 25;
		return;
	}

	auto animal1 = create_mobile(pMobIndex);
	auto animal2 = create_mobile(pMobIndex);
	animal2->dam_type = animal1->dam_type;

	for (auto i = 0; i < 4; i++)
	{
		animal1->armor[i] = 0;
		animal2->armor[i] = 0;
	}

	if (type == MOB_VNUM_FALCON)
	{
		animal1->max_hit = ch->max_hit / 3;
		animal1->hit = ch->max_hit / 3;
		animal2->max_hit = ch->max_hit / 3;
		animal2->hit = ch->max_hit / 3;
		animal1->damroll = (5 + ch->level / 3);
		animal2->damroll = (5 + ch->level / 3);
		animal1->hitroll = ch->level / 3;
		animal2->hitroll = ch->level / 3;
	}
	else if (type == MOB_VNUM_WOLF)
	{
		animal1->max_hit = ch->max_hit / 2;
		animal1->hit = ch->max_hit / 2;
		animal2->max_hit = ch->max_hit / 2;
		animal2->hit = ch->max_hit / 2;
		animal1->damroll = (8 + ch->level / 3);
		animal2->damroll = (8 + ch->level / 3);
		animal1->hitroll = ch->level / 3;
		animal2->hitroll = ch->level / 3;
	}
	else if (type == MOB_VNUM_BEAR)
	{
		animal1->max_hit = ch->max_hit;
		animal1->hit = ch->max_hit;
		animal2->max_hit = ch->max_hit;
		animal2->hit = ch->max_hit;
		animal1->damroll = (2 * ch->level / 3);
		animal2->damroll = (2 * ch->level / 3);
		animal1->hitroll = ch->level / 3;
		animal2->hitroll = ch->level / 3;
	}
	else if (type == MOB_VNUM_LION)
	{
		animal1->max_hit = (3 * ch->max_hit / 4);
		animal1->hit = (3 * ch->max_hit / 4);
		animal2->max_hit = (3 * ch->max_hit / 4);
		animal2->hit = (3 * ch->max_hit / 4);
		animal1->damroll = (-2 + ch->level);
		animal2->damroll = (-2 + ch->level);
		animal1->hitroll = ch->level / 3;
		animal2->hitroll = ch->level / 3;
	}
	else
	{
		bug("Bad animal call: %d.", type);

		send_to_char("You call out to the wild but nothing responds.\n\r", ch);
		act("$n calls out to the wild but nothing comes.\n\r", ch, 0, 0, TO_ROOM);

		ch->mana -= 25;
		return;
	}

	char_to_room(animal1, pRoomIndex);
	char_to_room(animal2, pRoomIndex);

	animal1->level = ch->level;
	animal2->level = ch->level;

	act("Animals respond to $n's call!", ch, 0, animal1, TO_ROOM);
	act("Animals respond to your call!", ch, 0, animal1, TO_CHAR);

	ch->mana -= 50;

	SET_BIT(animal1->affected_by, AFF_CHARM);
	SET_BIT(animal2->affected_by, AFF_CHARM);

	animal1->leader = ch;
	animal2->leader = ch;
	animal1->master = ch;
	animal2->master = ch;
	animal1->cabal = ch->cabal;
	animal2->cabal = ch->cabal;

	AFFECT_DATA af;
	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SKILL;
	af.type = gsn_animal_call;
	af.level = ch->level;
	af.duration = 24;
	af.location = 0;
	af.modifier = 0;
	affect_to_char(ch, &af);
}

void track_char(CHAR_DATA *ch, CHAR_DATA *mob)
{
	if (mob->in_room == nullptr)
		return;

	auto i = 0;
	auto found = false;
	for (; i < MAX_TRACKS; i++)
	{
		if (mob->in_room->tracks[i] && mob->in_room->tracks[i]->prey == ch)
		{
			found = true;
			break;
		}
	}

	if (!found)
		return;

	if (mob->in_room->tracks[i]->prey != ch)
		return;

	auto track_dir = mob->in_room->tracks[i]->direction;

	if (is_affected_by(mob, AFF_CHARM))
		return;

	if (IS_SET(mob->act, ACT_SENTINEL))
		return;

	act("$n checks the ground for tracks.", mob, 0, 0, TO_ROOM);

	if (IS_SET(mob->off_flags, STATIC_TRACKING) || track_dir == -1)
		return;

	auto pexit = mob->in_room->exit[track_dir];

	move_char(mob, track_dir, false, true);
}

void smart_track(CHAR_DATA *ch, CHAR_DATA *mob)
{
	if (!is_npc(mob) || !IS_SET(mob->act, ACT_SMARTTRACK))
		return;

	if (IS_SET(mob->act, ACT_SENTINEL) || is_affected_by(ch, AFF_NOSHOW))
		return;

	if (IS_SET(mob->off_flags, STATIC_TRACKING))
		return;

	if (ch->in_room == mob->in_room)
		return;

	if (ch->in_room->area != mob->in_room->area)
		return;

	if (mob->fighting)
		return;

	mob->path = nullptr;
	best_path = nullptr;
	iterations = 0;

	auto path = new_path_data();
	path->room = mob->in_room;
	path->steps = 0;

	mob->path = path;
	mob->in_room->path = path;

	find_path(mob->path, ch->in_room);

	auto solve = best_path;
	if (!best_path)
	{
		/*
		 * Typically this means they moved to a room not connected to the rest of the
		 * area, i.e. you get there via a special prog that moves you
		 */
		/*
		char buf[200];
		bug ("Some weird tracking shit just happened with mob vnum %d.",mob->pIndexData->vnum);
		sprintf(buf,"MOB %s tracking from ROOM %d to ROOM %d -- %d iterations.\n\r",
			mob->name, mob->in_room->vnum,
			ch->in_room->vnum,iterations);
		bug(buf,0);
		*/
		return;
	}

	for (;;)
	{
		if (!solve->prev->prev)
			break;

		solve = solve->prev;
	}

	/* Uncomment for tracking info */
	/*
	sprintf(buf,"MOB %s tracking from ROOM %d to ROOM %d -- %d iterations.\n\r",
		mob->name,
		mob->in_room->vnum,
		ch->in_room->vnum,
		iterations);
	log_string(buf);
	*/

	move_char(mob, solve->dir_from, false, true);

	free_path(mob->path);

	best_path = nullptr;

	for (auto room = room_list; room != nullptr; room = room->next_room)
	{
		if (room->path)
			room->path = nullptr;
	}
}

void walk_to_room(CHAR_DATA *mob, ROOM_INDEX_DATA *goal)
{
	if (!is_npc(mob))
		return;

	if (IS_SET(mob->act, ACT_SENTINEL))
		return;

	if (mob->in_room == goal)
		return;

	if (mob->in_room->area != goal->area)
		return;

	if (mob->fighting)
		return;

	mob->path = nullptr;
	best_path = nullptr;
	iterations = 0;

	auto path = new_path_data();
	path->room = mob->in_room;
	path->steps = 0;

	mob->path = path;
	mob->in_room->path = path;

	find_path(mob->path, goal);

	auto solve = best_path;
	if (!best_path)
	{
		bug("Some weird tracking shit just happened with mob vnum %d.", mob->pIndexData->vnum);
		return;
	}

	for (;;)
	{
		if (!solve->prev->prev)
			break;

		solve = solve->prev;
	}

	/*
	sprintf(buf,"MOB %s walking from ROOM %d to ROOM %d -- %d iterations.\n\r",
		mob->name,
		mob->in_room->vnum,
		goal->vnum,
		iterations);
	log_string(buf);
	*/

	move_char(mob, solve->dir_from, false, true);

	free_path(mob->path);

	best_path = nullptr;

	for (auto room = room_list; room != nullptr; room = room->next_room)
	{
		if (room->path)
			room->path = nullptr;
	}
}

void find_path(PATHFIND_DATA *path, ROOM_INDEX_DATA *goal)
{
	path->evaluated = true;
	iterations++;

	if (path->room == goal)
	{
		if (!best_path || best_path->steps > path->steps)
			best_path = path;

		return;
	}

	auto found = false;
	for (auto i = 0; i < 6; i++)
	{
		if ((best_path && best_path->steps < 2)
			|| (path->room->exit[i] 
				&& path->room->exit[i]->u1.to_room
				&& path->room->exit[i]->u1.to_room->area == goal->area
				&& !(path->room->exit[i]->u1.to_room->path
					&& path->room->exit[i]->u1.to_room->path->steps <= path->steps + 1
					&& path->room->exit[i]->u1.to_room->path->evaluated)
				&& !(path->room->exit[i]->u1.to_room->path
					&& path->room->exit[i]->u1.to_room->path->steps < path->steps + 1
					&& !path->room->exit[i]->u1.to_room->path->evaluated)))
		{
			found = true;
		}
	}

	if (!found)
	{
		for (auto i = 0; i < 6; i++)
		{
			path->dir_to[i] = nullptr;
		}

		return;
	}

	for (auto i = 0; i < 6; i++)
	{
		if (!path->room->exit[i]
			|| !path->room->exit[i]->u1.to_room
			|| path->room->exit[i]->u1.to_room->area != goal->area
			|| (path->room->exit[i]->u1.to_room->path 
				&& path->room->exit[i]->u1.to_room->path->steps <= path->steps + 1 
				&& path->room->exit[i]->u1.to_room->path->evaluated)
			|| (path->room->exit[i]->u1.to_room->path
				&& path->room->exit[i]->u1.to_room->path->steps < path->steps + 1
				&& !path->room->exit[i]->u1.to_room->path->evaluated))
		{
			continue;
		}

		auto next_path = new_path_data();
		next_path->room = path->room->exit[i]->u1.to_room;
		next_path->dir_from = i;
		next_path->steps = path->steps + 1;
		next_path->prev = path;

		path->dir_to[i] = next_path;
		path->room->exit[i]->u1.to_room->path = next_path;
	}

	for (auto i = 0; i < 6; i++)
	{
		if (!path->room->exit[i]
			|| !path->dir_to[i]
			|| !path->room->exit[i]->u1.to_room
			|| path->room->exit[i]->u1.to_room->area != goal->area
			|| path->room->exit[i]->u1.to_room->path->evaluated)
		{
			continue;
		}

		find_path(path->dir_to[i], goal);
	}
}

void do_aura_of_sustenance(CHAR_DATA *ch, char *argument)
{
	if (get_skill(ch, gsn_aura_of_sustenance) == 0 || ch->level < skill_table[gsn_aura_of_sustenance].skill_level[ch->Class()->GetIndex()])
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_aura_of_sustenance))
	{
		send_to_char("You are already as sustained as you can get.\n\r", ch);
		return;
	}

	if (ch->mana < 40)
	{
		send_to_char("You do not have the mana\n\r", ch);
		return;
	}

	AFFECT_DATA af;
	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SKILL;
	af.location = 0;
	af.level = ch->level;
	af.duration = ch->level;
	af.type = gsn_aura_of_sustenance;
	af.modifier = 0;
	affect_to_char(ch, &af);

	ch->pcdata->condition[COND_THIRST] = 1;
	ch->pcdata->condition[COND_HUNGER] = 1;

	send_to_char("Your mind goes into a trance as you sustain your body in a state of health.\n\r", ch);

	ch->mana -= 40;
}

void do_vanish(CHAR_DATA *ch, char *argument)
{
	auto chance = get_skill(ch, gsn_vanish);
	if (chance == 0 || ch->level < skill_table[gsn_vanish].skill_level[ch->Class()->GetIndex()])
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_vanish))
	{
		send_to_char("You cannot vanish again so soon.\n\r", ch);
		return;
	}

	if (ch->mana < 20)
	{
		send_to_char("You don't have the mana.", ch);
		return;
	}

	WAIT_STATE(ch, PULSE_VIOLENCE);

	if (number_percent() > (chance * .95)
		|| IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL)
		|| IS_SET(ch->in_room->room_flags, ROOM_NO_SUMMON_FROM))
	{
		send_to_char("You attempt to vanish without trace but fail.\n\r", ch);
		act("$n attempts to slide into the shadows but fails.", ch, 0, 0, TO_ROOM);

		check_improve(ch, gsn_vanish, false, 2);

		ch->mana -= 10;

		return;
	}

	ch->mana -= 20;

	auto found = false;
	ROOM_INDEX_DATA *pRoomIndex;
	for (auto nocrash = 0; nocrash < 100000; nocrash++)
	{
		pRoomIndex = get_room_index(number_range(0, 30000));
		if (pRoomIndex == nullptr)
			continue;

		if (pRoomIndex->area == ch->in_room->area
			&& !IS_SET(pRoomIndex->room_flags, ROOM_PRIVATE)
			&& pRoomIndex->guild == 0
			&& can_see_room(ch, pRoomIndex)
			&& !IS_SET(pRoomIndex->room_flags, ROOM_SOLITARY)
			&& ch->in_room->vnum != 1212
			&& pRoomIndex->vnum != 1212)
		{
			found = true;
			break;
		}
	}

	if (!found)
	{
		ch->mana += 20;

		send_to_char("You failed.", ch);
		return;
	}

	stop_fighting(ch, true);

	act("$n suddenly vanishes into the shadows!", ch, 0, 0, TO_ROOM);
	send_to_char("You slip into the shadows and vanish!\n\r", ch);

	char_from_room(ch);
	char_to_room(ch, pRoomIndex);

	act("$n appears from the shadows.", ch, 0, 0, TO_ROOM);

	do_look(ch, "auto");

	WAIT_STATE(ch, PULSE_VIOLENCE * 2);
}

void do_door_bash(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	one_argument(argument, arg);

	if (arg[0] == '\0')
	{
		send_to_char("Bash door which door?\n\r", ch);
		return;
	}

	auto chance = get_skill(ch, gsn_door_bash);

	if (is_npc(ch))
		chance = 50;

	if (!is_npc(ch) && (chance == 0 || ch->level < skill_table[gsn_door_bash].skill_level[ch->Class()->GetIndex()]))
	{
		send_to_char("You'd hurt yourself doing that.\n\r", ch);
		return;
	}

	if (ch->move < 5)
	{
		send_to_char("You are too exhausted.\n\r", ch);
		return;
	}

	auto door = find_door(ch, arg);
	if (door < 0)
		return;

	auto pexit = ch->in_room->exit[door];
	if (!IS_SET(pexit->exit_info, EX_CLOSED))
	{
		send_to_char("It's not closed.\n\r", ch);
		return;
	}

	if (!IS_SET(pexit->exit_info, EX_LOCKED) && !IS_SET(pexit->exit_info, EX_JAMMED))
	{
		send_to_char("It's already unlocked, why not just use the knob?\n\r", ch);
		return;
	}

	if (!is_npc(ch))
	{
		chance /= 4;
		chance += ch->carry_weight / 10;
		chance += get_curr_stat(ch, STAT_STR) / 2;
	}

	if (!is_npc(ch))
	{
		if (!str_cmp(pc_race_table[ch->race].name, "planar")
			|| !str_cmp(pc_race_table[ch->race].name, "abyss")
			|| !str_cmp(pc_race_table[ch->race].name, "celestial"))
		{
			chance += 20;
		}
	}

	if (!is_npc(ch) && is_immortal(ch))
		chance = 100;

	do_visible(ch, "");

	if (number_percent() > chance || IS_SET(pexit->exit_info, EX_NOBASH))
	{
		act("$n flies into the $T door and rebounds with a great lack of dignity!", ch, 0, dir_name[door], TO_ROOM);
		act("You fly into the door $T but simply bounce off it like a lump of rock!", ch, 0, dir_name[door], TO_CHAR);

		if (pexit->u1.to_room->people)
			act("The door buckles as a heavy weight crashes against it from the other side!", pexit->u1.to_room->people, 0, 0, TO_ALL);

		damage_new(ch, ch, dice(ch->level, 2), gsn_door_bash, DAM_BASH, true, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "the impact*");

		check_improve(ch, gsn_door_bash, false, 1);

		ch->position = POS_RESTING;

		WAIT_STATE(ch, 3 * PULSE_VIOLENCE);
		return;
	}

	act("$n slams into the $T door and throws it open with a mighty crash!", ch, 0, dir_name[door], TO_ROOM);
	act("You slam into the $T door and it cracks open with a deafening sound!", ch, 0, dir_name[door], TO_CHAR);

	check_improve(ch, gsn_door_bash, true, 1);

	WAIT_STATE(ch, 2 * PULSE_VIOLENCE);
	REMOVE_BIT(pexit->exit_info, EX_LOCKED);
	REMOVE_BIT(pexit->exit_info, EX_CLOSED);
	REMOVE_BIT(pexit->exit_info, EX_JAMMED);

	auto to_room = pexit->u1.to_room;
	auto pexit_rev = to_room->exit[rev_dir[door]];

	if (to_room != nullptr && pexit_rev != nullptr && pexit_rev->u1.to_room == ch->in_room)
	{
		REMOVE_BIT(pexit_rev->exit_info, EX_LOCKED);
		REMOVE_BIT(pexit_rev->exit_info, EX_CLOSED);
		REMOVE_BIT(pexit_rev->exit_info, EX_JAMMED);
	}

	if (number_percent() < (3 * get_curr_stat(ch, STAT_DEX)) || is_npc(ch))
		return;

	/*
	* Affect to char so in move_char you relay the right move message, then
	*  strip affect once moved.
	*/
	AFFECT_DATA af;
	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SKILL;
	af.type = gsn_door_bash;
	af.location = 0;
	af.modifier = 0;
	af.duration = -1;
	af.level = ch->level;
	affect_to_char(ch, &af);

	move_char(ch, door, false, true);
	affect_strip(ch, gsn_door_bash);
}

/* For selecting new home */

void do_hometown(CHAR_DATA *ch, char *argument)
{
	if (is_npc(ch))
		return;

	if (ch->in_room->vnum != 0) // ROOM_VNUM_HOMETOWNS)
	{
		send_to_char("You can't do that here.\n\r", ch);
		return;
	}

	char arg[MAX_STRING_LENGTH];
	char arg2[MAX_STRING_LENGTH];
	argument = one_argument(argument, arg);
	one_argument(argument, arg2);

	char buf[MAX_STRING_LENGTH];
	auto cost = 50 * (ch->level * ch->level);
	if (arg[0] == '\0')
	{
		send_to_char("Available hometowns to you are: \n\r", ch);

		for (auto i = 0; i < MAX_HOMETOWN; i++)
		{
			if (can_live_in(ch, i))
			{
				if (ch->hometown == i)
				{
					sprintf(buf, "%-18s (Current hometown)\n\r", hometown_table[i].name);
					send_to_char(buf, ch);
				}
				else
				{
					sprintf(buf, "%-18s %d gold.\n\r", hometown_table[i].name, cost);
					send_to_char(buf, ch);
				}
			}
		}

		return;
	}
	else
	{
		if (ch->gold < cost)
		{
			send_to_char("You don't have enough money.\n\r", ch);
			return;
		}

		auto hometown = hometown_lookup(arg);
		if (hometown == ch->hometown)
		{
			send_to_char("That is your current hometown already.\n\r", ch);
			return;
		}
		else
		{
			if (!can_live_in(ch, hometown))
			{
				send_to_char("You cannot live there.\n\r", ch);
				return;
			}

			sprintf(buf, "Your hometown is now %s.\n\r", hometown_table[hometown].name);
			send_to_char(buf, ch);

			deduct_cost(ch, cost);

			ch->hometown = hometown;
			return;
		}
	}

	send_to_char("That city doesn't exist for choosing as a hometown. Type 'hometown' to see list.\n\r", ch);
}

bool check_barred(CHAR_DATA *ch, ROOM_INDEX_DATA *to_room)
{
	int field = 0;

	for (auto blocker = ch->in_room->people; blocker != nullptr; blocker = blocker->next_in_room)
	{
		if (is_npc(blocker)
			&& blocker->pIndexData->barred_entry
			&& blocker->pIndexData->barred_entry->vnum == to_room->vnum)
		{

			if (blocker->pIndexData->barred_entry->type == BAR_CLASS)
				field = ch->Class()->GetIndex();

			if (blocker->pIndexData->barred_entry->type == BAR_CABAL)
				field = ch->cabal;

			if (blocker->pIndexData->barred_entry->type == BAR_SIZE)
				field = ch->size;

			if (blocker->pIndexData->barred_entry->type == BAR_LEVEL)
				field = ch->level;

			if (blocker->pIndexData->barred_entry->type == BAR_TATTOO)
			{
				auto tattoo = get_eq_char(ch, WEAR_BRAND);

				if (!tattoo && blocker->pIndexData->barred_entry->value)
					return bar_entry(ch, blocker, to_room);

				field = tattoo->pIndexData->vnum;
			}

			if (blocker->pIndexData->barred_entry->type == BAR_CLASS
				&& ch->Class()->GetIndex() == blocker->pIndexData->barred_entry->value
				&& IS_SET(blocker->act, ACT_GUILDGUARD)
				&& ch->pause > 0
				&& ch->ghost <= 0
				&& !is_immortal(ch))
			{
				send_to_char("Your guild will not offer you sanctuary yet.\n\r", ch);
				return true;
			}

			if ((blocker->pIndexData->barred_entry->comparison == BAR_EQUAL_TO && field != blocker->pIndexData->barred_entry->value)
				|| (blocker->pIndexData->barred_entry->comparison == BAR_LESS_THAN && field >= blocker->pIndexData->barred_entry->value)
				|| (blocker->pIndexData->barred_entry->comparison == BAR_GREATER_THAN && field <= blocker->pIndexData->barred_entry->value))
			{
				return bar_entry(ch, blocker, to_room);
			}
		}
	}

	return false;
}

bool bar_entry(CHAR_DATA *ch, CHAR_DATA *blocker, ROOM_INDEX_DATA *to_room)
{
	char buf[MAX_STRING_LENGTH];
	auto str = palloc_string(blocker->pIndexData->barred_entry->message);

	parse_bar(buf, str, ch, blocker, to_room);

	if (blocker->pIndexData->barred_entry->msg_type == BAR_SAY)
		do_say(blocker, buf);

	if (blocker->pIndexData->barred_entry->msg_type == BAR_EMOTE)
		do_emote(blocker, buf);

	if (blocker->pIndexData->barred_entry->msg_type == BAR_ECHO)
	{
		buf[0] = UPPER(buf[0]);

		send_to_char(buf, ch);
		send_to_char("\n\r", ch);

		if (blocker->pIndexData->barred_entry->message_two)
		{
			char buf2[MAX_STRING_LENGTH];
			auto strtwo = palloc_string(blocker->pIndexData->barred_entry->message_two);
			parse_bar(buf2, strtwo, ch, blocker, to_room);

			buf2[0] = UPPER(buf2[0]);

			if (!is_immortal(ch) || ch->invis_level < LEVEL_HERO + 1)
				mob_recho(ch, buf2);
		}
	}

	if (is_immortal(ch))
		return false;

	return true;
}

void parse_bar(char *buf, const char *str, CHAR_DATA *ch, CHAR_DATA *blocker, ROOM_INDEX_DATA *to_room)
{
	const char *i = nullptr;
	char *point;
	char buf2[MAX_STRING_LENGTH];
	point = buf;

	while (*str != '\0')
	{
		if (*str != '$')
		{
			*point++ = *str++;
			continue;
		}

		++str;
		switch (*str)
		{
			case 'n':
				sprintf(buf2, "%s", ch->name);
				i = buf2;
				break;
			case 'N':
				sprintf(buf2, "%s", blocker->short_descr);
				i = buf2;
				break;
			case 'r':
				sprintf(buf2, "%s", get_room_name(ch->in_room));
				i = buf2;
				break;

			case 'R':
				sprintf(buf2, "%s", get_room_name(to_room));
				i = buf2;
				break;
			default:
				break;
		}

		++str;

		while ((*point = *i) != '\0')
		{
			++point, ++i;
		}
	}

	*point = '\0';
}

bool is_land(ROOM_INDEX_DATA *room)
{
	if (room->sector_type == SECT_WATER || room->sector_type == SECT_UNDERWATER || room->sector_type == SECT_AIR)
		return false;

	return true;
}

void add_tracks(ROOM_INDEX_DATA *room, CHAR_DATA *ch, int direction)
{
	if (is_ground(room) && is_affected(ch, gsn_pass_without_trace))
		return;

	if (room->tracks[0] == nullptr)
		return;

	if (room->tracks[0]->prey != nullptr)
	{
		for (auto i = MAX_TRACKS - 1; i > 0; i--)
		{
			room->tracks[i]->prey = room->tracks[i - 1]->prey;
			room->tracks[i]->time = room->tracks[i - 1]->time;
			room->tracks[i]->direction = room->tracks[i - 1]->direction;
			room->tracks[i]->flying = room->tracks[i - 1]->flying;
			room->tracks[i]->sneaking = room->tracks[i - 1]->sneaking;
			room->tracks[i]->bleeding = room->tracks[i - 1]->bleeding;
			room->tracks[i]->legs = room->tracks[i - 1]->legs;
		}
	}

	room->tracks[0]->prey = ch;
	room->tracks[0]->time = time_info;
	room->tracks[0]->direction = direction;
	room->tracks[0]->flying = is_affected_by(ch, AFF_FLYING);
	room->tracks[0]->sneaking = is_affected_by(ch, AFF_SNEAK);
	room->tracks[0]->bleeding = is_affected(ch, gsn_bleeding);
	room->tracks[0]->legs = ch->legs;
}

void clear_tracks(ROOM_INDEX_DATA *room)
{
	for (auto i = 0; i < MAX_TRACKS - 1; i++)
	{
		room->tracks[i]->prey = nullptr;
		room->tracks[i]->direction = -1;
		room->tracks[i]->flying= false;
		room->tracks[i]->sneaking= false;
		room->tracks[i]->bleeding= false;
		room->tracks[i]->legs = -1;
	}
}
