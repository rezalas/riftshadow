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
 *     Russ Taylor (rtaylor@efn.org)                                       *
 *     Gabrielle Taylor (gtaylor@pacinfo.com)                              *
 *     Brian Moore (zump@rom.org)                                          *
 *  By using this code, you have agreed to follow the terms of the         *
 *  ROM license, in the file Tartarus/doc/rom.license                      *
 ***************************************************************************/

/****************************************************************************
 *       Tartarus code is copyright (C) 1997-1998 by Daniel Graham          *
 *  In using this code you agree to comply with the Tartarus license        *
 *       found in the file /Tartarus/doc/tartarus.doc                       *
 ****************************************************************************/

#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "act_ente.h"
#include "handler.h"
#include "magic.h"
#include "comm.h"
#include "db.h"
#include "act_info.h"
#include "act_move.h"
#include "utility.h"
#include "magic.h"

/* random room generation procedure */
ROOM_INDEX_DATA *get_random_room(CHAR_DATA *ch)
{
	auto getRoomIndex = []()
	{
		return get_room_index(number_range(0, 65535));
	};
	
	for (auto room = getRoomIndex(); room != nullptr; room = getRoomIndex())
	{
		if (can_see_room(ch, room)
			&& !room_is_private(room)
			&& !IS_SET(room->room_flags, ROOM_PRIVATE) 
			&& !IS_SET(room->room_flags, ROOM_SOLITARY)
			&& !IS_SET(room->room_flags, ROOM_SAFE)
			&& !IS_SET(room->room_flags, ROOM_HEROES_ONLY)
			&& !IS_SET(room->room_flags, ROOM_GODS_ONLY)
			&& !IS_SET(room->room_flags, ROOM_NO_GATE))
		{
			if (!is_npc(ch))
				return room;

			if (!(IS_SET(ch->act, ACT_AGGRESSIVE) && !IS_SET(room->room_flags, ROOM_LAW)))
				return room;
		}
	}

	return nullptr;
}

/* RT Enter portals */
void do_enter(CHAR_DATA *ch, char *argument)
{
	if (ch->fighting != nullptr)
		return;

	/* nifty portal stuff */
	if (argument[0] == '\0')
	{
		send_to_char("Nope, can't do it.\n\r", ch);
		return;
	}

	auto old_room = ch->in_room;

	auto portal = get_obj_list(ch, argument, ch->in_room->contents);
	if (portal == nullptr)
	{
		send_to_char("You don't see that here.\n\r", ch);
		return;
	}

	if (portal->item_type != ITEM_PORTAL
		|| (IS_SET_OLD(portal->value[1], EX_CLOSED) && !is_trusted(ch, ANGEL)))
	{
		send_to_char("You can't seem to find a way in.\n\r", ch);
		return;
	}

	if (!is_trusted(ch, ANGEL)
		&& !IS_SET_OLD(portal->value[2], GATE_NOCURSE) 
		&& (is_affected_by(ch, AFF_CURSE) || IS_SET(old_room->room_flags, ROOM_NO_RECALL)))
	{
		send_to_char("Something prevents you from leaving...\n\r", ch);
		return;
	}

	if (IS_SET_OLD(portal->value[2], GATE_RANDOM) 
		|| portal->value[3] == -1 
		|| (IS_SET_OLD(portal->value[2], GATE_BUGGY) && number_percent() < 5))
	{
		send_to_char("You enter the portal and find yourself elsewhere.\n\r", ch);
		act("$n steps into $p.", ch, portal, 0, TO_ROOM);
		spell_teleport(skill_lookup("teleport"), ch->level, ch, ch, TAR_CHAR_SELF);
		return;
	}

	auto location = get_room_index(portal->value[3]);
	if (location == nullptr 
		|| location == old_room 
		|| !can_see_room(ch, location) 
		|| (room_is_private(location) && !is_trusted(ch, IMPLEMENTOR)))
	{
		act("$p doesn't seem to go anywhere.", ch, portal, nullptr, TO_CHAR);
		return;
	}

	if (is_npc(ch) 
		&& IS_SET(ch->act, ACT_AGGRESSIVE) 
		&& IS_SET(location->room_flags, ROOM_LAW))
	{
		send_to_char("Something prevents you from leaving...\n\r", ch);
		return;
	}

	act("$n steps into $p.", ch, portal, nullptr, TO_ROOM);

	if (IS_SET_OLD(portal->value[2], GATE_NORMAL_EXIT))
		act("The world bends around you as you enter $p.", ch, portal, nullptr, TO_CHAR);
	else
		act("You walk through $p and find yourself somewhere else...", ch, portal, nullptr, TO_CHAR);

	char_from_room(ch);
	char_to_room(ch, location);

	if (IS_SET_OLD(portal->value[2], GATE_GOWITH)) /* take the gate along */
	{
		obj_from_room(portal);
		obj_to_room(portal, location);
	}

	if (IS_SET_OLD(portal->value[2], GATE_NORMAL_EXIT))
		act("$n has arrived.", ch, portal, nullptr, TO_ROOM);
	else
		act("$n has arrived through $p.", ch, portal, nullptr, TO_ROOM);

	do_look(ch, "auto");

	/* charges */
	if (portal->value[0] > 0)
		portal->value[0]--;

	/* protect against circular follows */
	if (old_room == location)
		return;

	auto to_room = location;

	for (auto fch = to_room->people; fch != nullptr; fch = fch->next_in_room)
	{
		/* greet trigger for items carried by people in room */
		for (auto obj = fch->carrying; obj != nullptr; obj = obj->next_content)
		{
			if (IS_SET(obj->progtypes, IPROG_GREET))
				obj->pIndexData->iprogs->greet_prog(obj, ch);
		}

		/* Greet trigger for mobs  */
		if (is_npc(fch) && IS_SET(fch->progtypes, MPROG_GREET))
			fch->pIndexData->mprogs->greet_prog(fch, ch);
	}

	for (auto obj = ch->carrying; obj != nullptr; obj = obj->next_content)
	{
		if (IS_SET(obj->progtypes, IPROG_ENTRY))
			obj->pIndexData->iprogs->entry_prog(obj);
	}

	if (IS_SET(to_room->progtypes, RPROG_ENTRY))
		to_room->rprogs->entry_prog(to_room, ch);

	for (auto fch = old_room->people; fch != nullptr; fch = fch->next_in_room)
	{
		/* no following through dead portals */
		if (portal == nullptr || portal->value[0] == 0)
			continue;

		if (fch->master == ch && is_affected_by(fch, AFF_CHARM) && fch->position < POS_STANDING)
			do_stand(fch, "");

		if (fch->master == ch && fch->position == POS_STANDING)
		{
			if (IS_SET(ch->in_room->room_flags, ROOM_LAW) && (is_npc(fch) && IS_SET(fch->act, ACT_AGGRESSIVE)))
			{
				act("You can't bring $N into the city.", ch, nullptr, fch, TO_CHAR);
				act("You aren't allowed in the city.", fch, nullptr, nullptr, TO_CHAR);
				continue;
			}

			act("You follow $N.", fch, nullptr, ch, TO_CHAR);
			do_enter(fch, argument);
		}
	}

	if (portal != nullptr && portal->value[0] == 0)
	{
		act("$p shimmers and rapidly closes behind you.", ch, portal, nullptr, TO_CHAR);

		if (ch->in_room == old_room)
		{
			act("$p flares brilliantly and fades away.", ch, portal, nullptr, TO_ROOM);
		}
		else if (old_room->people != nullptr)
		{
			act("$p flares brilliantly and fades away.", old_room->people, portal, nullptr, TO_CHAR);
			act("$p flares brilliantly and fades away.", old_room->people, portal, nullptr, TO_ROOM);
		}

		extract_obj(portal);
	}
}
