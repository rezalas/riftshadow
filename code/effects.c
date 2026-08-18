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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "entity/handles.h"
#include "effects.h"
#include "handler.h"
#include "recycle.h"
#include "comm.h"
#include "update.h"
#include "db.h"
#include "magic.h"
#include "utility.h"

/// Applies an acid effect to whatever the target holds.
/// @param target What the effect is aimed at. May be a room, a character or an object.
/// @param level The effective level of the effect.
/// @param dam The damage driving the effect.
/// @details A room passes the effect on to every object lying in it, and a character passes it on to everything carried. An object may be damaged or destroyed.
void acid_effect(SpellTarget target, int level, int dam)
{
	if (target.IsRoom()) /* nail objects on the floor */
	{
		ROOM_INDEX_DATA *room = target.AsRoom();
		OBJ_DATA *obj, *obj_next;

		for (obj = room->contents; obj != nullptr; obj = obj_next)
		{
			obj_next = obj->next_content;
			acid_effect(obj, level, dam);
		}

		return;
	}

	if (target.IsChar()) /* do the effect on a victim */
	{
		CHAR_DATA *victim = target.AsChar();
		OBJ_DATA *obj, *obj_next;

		/* let's toast some gear */
		for (obj = victim->carrying; obj != nullptr; obj = obj_next)
		{
			obj_next = obj->next_content;
			acid_effect(obj, level, dam);
		}

		return;
	}

	if (target.IsObj()) /* toast an object */
	{
		OBJ_DATA *obj = target.AsObj();
		OBJ_DATA *t_obj, *n_obj;
		int chance;
		char *msg;

		if (is_obj_stat(obj, ITEM_BURN_PROOF) || is_obj_stat(obj, ITEM_NOPURGE) || number_range(0, 4) == 0)
			return;

		chance = level / 4 + dam / 10;

		if (chance > 25)
			chance = (chance - 25) / 2 + 25;

		if (chance > 50)
			chance = (chance - 50) / 2 + 50;

		if (is_obj_stat(obj, ITEM_BLESS))
			chance -= 5;

		chance -= obj->level * 2;

		switch (obj->item_type)
		{
			case ITEM_CONTAINER:
			case ITEM_CORPSE_PC:
			case ITEM_CORPSE_NPC:
				msg = "$p fumes and dissolves.";
				break;
			case ITEM_ARMOR:
				msg = "$p is pitted and etched.";
				break;
			case ITEM_CLOTHING:
				msg = "$p is corroded into scrap.";
				break;
			case ITEM_STAFF:
			case ITEM_WAND:
				chance -= 10;
				msg = "$p corrodes and breaks.";
				break;
			case ITEM_SCROLL:
				chance += 10;
				msg = "$p is burned into waste.";
				break;
			default:
				return;
		}

		chance = URANGE(5, chance, 95);

		if (number_percent() > chance)
			return;

		if (CHAR_DATA *carrier = Deref(obj->carried_by))
			act(msg, carrier, obj, nullptr, TO_ALL);
		else if (obj->in_room != nullptr && obj->in_room->people != nullptr)
			act(msg, obj->in_room->people, obj, nullptr, TO_ALL);

		if (obj->item_type == ITEM_ARMOR) /* etch it */
		{
			bool af_found = false;
			int i;

			for (auto &paf : obj->affected)
			{
				if (paf.location == APPLY_AC)
				{
					af_found = true;
					paf.type = -1;
					paf.modifier += 1;
					paf.level = std::max((int)paf.level, level);
					break;
				}
			}

			/* needs a new affect */
			if (!af_found)
			{
				OBJ_AFFECT_DATA paf;

				paf.type = -1;
				paf.level = level;
				paf.duration = -1;
				paf.location = APPLY_AC;
				paf.modifier = 1;
				obj->affected.push_front(paf);
			}

			CHAR_DATA *carrier = Deref(obj->carried_by);

			if (carrier != nullptr && obj->wear_loc != WEAR_NONE)
			{
				for (i = 0; i < 4; i++)
				{
					carrier->armor[i] += 1;
				}
			}

			return;
		}

		/* get rid of the object */
		if (obj->contains) /* dump contents */
		{
			for (t_obj = obj->contains; t_obj != nullptr; t_obj = n_obj)
			{
				n_obj = t_obj->next_content;
				obj_from_obj(t_obj);

				if (obj->in_room != nullptr)
				{
					obj_to_room(t_obj, obj->in_room);
				}
				else if (CHAR_DATA *carrier = Deref(obj->carried_by))
				{
					obj_to_room(t_obj, carrier->in_room);
				}
				else
				{
					extract_obj(t_obj);
					continue;
				}

				acid_effect(t_obj, level / 2, dam / 2);
			}
		}

		extract_obj(obj);
	}
}

/// Applies a cold effect to whatever the target holds.
/// @param target What the effect is aimed at. May be a room, a character or an object.
/// @param level The effective level of the effect.
/// @param dam The damage driving the effect.
/// @details A room passes the effect on to every object lying in it. A character may be chilled and carried containers may shatter.
void cold_effect(SpellTarget target, int level, int dam)
{
	if (target.IsRoom()) /* nail objects on the floor */
	{
		ROOM_INDEX_DATA *room = target.AsRoom();
		OBJ_DATA *obj, *obj_next;

		for (obj = room->contents; obj != nullptr; obj = obj_next)
		{
			obj_next = obj->next_content;
			cold_effect(obj, level, dam);
		}

		return;
	}

	if (target.IsChar()) /* whack a character */
	{
		CHAR_DATA *victim = target.AsChar();
		OBJ_DATA *obj, *obj_next;

		/* chill touch effect */
		if (!saves_spell(level / 4 + dam / 20, victim, DAM_COLD))
		{
			AFFECT_DATA af;

			act("$n turns blue and shivers.", victim, nullptr, nullptr, TO_ROOM);
			act("A chill sinks deep into your bones.", victim, nullptr, nullptr, TO_CHAR);
			init_affect(&af);
			af.where = TO_AFFECTS;
			af.aftype = AFT_SPELL;
			af.type = skill_lookup("chill");
			af.level = level;
			af.duration = 6;
			af.location = APPLY_STR;
			af.modifier = -1;
			affect_join(victim, &af);
		}

		/* hunger! (warmth sucked out */
		if (!is_npc(victim))
			gain_condition(victim, COND_HUNGER, dam / 20);

		/* let's toast some gear */
		for (obj = victim->carrying; obj != nullptr; obj = obj_next)
		{
			obj_next = obj->next_content;
			cold_effect(obj, level, dam);
		}

		return;
	}

	if (target.IsObj()) /* toast an object */
	{
		OBJ_DATA *obj = target.AsObj();
		int chance;
		char *msg;

		if (is_obj_stat(obj, ITEM_BURN_PROOF) || is_obj_stat(obj, ITEM_NOPURGE) || number_range(0, 4) == 0)
			return;

		chance = level / 4 + dam / 10;

		if (chance > 25)
			chance = (chance - 25) / 2 + 25;
		if (chance > 50)
			chance = (chance - 50) / 2 + 50;

		if (is_obj_stat(obj, ITEM_BLESS))
			chance -= 5;

		chance -= obj->level * 2;

		switch (obj->item_type)
		{
			case ITEM_POTION:
				msg = "$p freezes and shatters!";
				chance += 25;
				break;
			case ITEM_DRINK_CON:
				msg = "$p freezes and shatters!";
				chance += 5;
				break;
			default:
				return;
		}

		chance = URANGE(5, chance, 95);

		if (number_percent() > chance)
			return;

		if (CHAR_DATA *carrier = Deref(obj->carried_by))
			act(msg, carrier, obj, nullptr, TO_ALL);
		else if (obj->in_room != nullptr && obj->in_room->people != nullptr)
			act(msg, obj->in_room->people, obj, nullptr, TO_ALL);

		extract_obj(obj);
	}
}

/// Applies a fire effect to whatever the target holds.
/// @param target What the effect is aimed at. May be a room, a character or an object.
/// @param level The effective level of the effect.
/// @param dam The damage driving the effect.
/// @details A room passes the effect on to every object lying in it, and a character passes it on to everything carried. An object may burn away.
void fire_effect(SpellTarget target, int level, int dam)
{
	if (target.IsRoom()) /* nail objects on the floor */
	{
		ROOM_INDEX_DATA *room = target.AsRoom();
		OBJ_DATA *obj, *obj_next;

		for (obj = room->contents; obj != nullptr; obj = obj_next)
		{
			obj_next = obj->next_content;
			fire_effect(obj, level, dam);
		}

		return;
	}

	if (target.IsChar()) /* do the effect on a victim */
	{
		CHAR_DATA *victim = target.AsChar();
		OBJ_DATA *obj, *obj_next;

		/* chance of blindness */
		if (!is_affected_by(victim, AFF_BLIND) && !saves_spell(level / 4 + dam / 20, victim, DAM_FIRE))
		{
			AFFECT_DATA af;
			act("$n is blinded by smoke!", victim, nullptr, nullptr, TO_ROOM);
			act("Your eyes tear up from smoke...you can't see a thing!", victim, nullptr, nullptr, TO_CHAR);

			init_affect(&af);
			af.where = TO_AFFECTS;
			af.aftype = AFT_SPELL;
			af.type = skill_lookup("fire breath");
			af.level = level;
			af.duration = number_range(0, level / 10);
			af.location = APPLY_HITROLL;
			af.modifier = -4;
			SET_BIT(af.bitvector, AFF_BLIND);

			affect_to_char(victim, &af);
		}

		/* getting thirsty */
		if (!is_npc(victim))
			gain_condition(victim, COND_THIRST, dam / 20);

		/* let's toast some gear! */
		for (obj = victim->carrying; obj != nullptr; obj = obj_next)
		{
			obj_next = obj->next_content;

			fire_effect(obj, level, dam);
		}

		return;
	}

	if (target.IsObj()) /* toast an object */
	{
		OBJ_DATA *obj = target.AsObj();
		OBJ_DATA *t_obj, *n_obj;
		int chance;
		char *msg;

		if (is_obj_stat(obj, ITEM_BURN_PROOF) || is_obj_stat(obj, ITEM_NOPURGE) || number_range(0, 4) == 0)
			return;

		chance = level / 4 + dam / 10;

		if (chance > 25)
			chance = (chance - 25) / 2 + 25;
		if (chance > 50)
			chance = (chance - 50) / 2 + 50;

		if (is_obj_stat(obj, ITEM_BLESS))
			chance -= 5;
		chance -= obj->level * 2;

		switch (obj->item_type)
		{
			case ITEM_CONTAINER:
				msg = "$p ignites and burns!";
				break;
			case ITEM_POTION:
				chance += 25;
				msg = "$p bubbles and boils!";
				break;
			case ITEM_SCROLL:
				chance += 50;
				msg = "$p crackles and burns!";
				break;
			case ITEM_STAFF:
				chance += 10;
				msg = "$p smokes and chars!";
				break;
			case ITEM_WAND:
				msg = "$p sparks and sputters!";
				break;
			case ITEM_FOOD:
				msg = "$p blackens and crisps!";
				break;
			case ITEM_PILL:
				msg = "$p melts and drips!";
				break;
			default:
				return;
		}

		chance = URANGE(5, chance, 95);

		if (number_percent() > chance)
			return;

		if (CHAR_DATA *carrier = Deref(obj->carried_by))
			act(msg, carrier, obj, nullptr, TO_ALL);
		else if (obj->in_room != nullptr && obj->in_room->people != nullptr)
			act(msg, obj->in_room->people, obj, nullptr, TO_ALL);

		if (obj->contains)
		{
			/* dump the contents */

			for (t_obj = obj->contains; t_obj != nullptr; t_obj = n_obj)
			{
				n_obj = t_obj->next_content;
				obj_from_obj(t_obj);

				if (obj->in_room != nullptr)
				{
					obj_to_room(t_obj, obj->in_room);
				}
				else if (CHAR_DATA *carrier = Deref(obj->carried_by))
				{
					obj_to_room(t_obj, carrier->in_room);
				}
				else
				{
					extract_obj(t_obj);
					continue;
				}

				fire_effect(t_obj, level / 2, dam / 2);
			}
		}

		extract_obj(obj);
	}
}

/// Applies a poison effect to whatever the target holds.
/// @param target What the effect is aimed at. May be a room, a character or an object.
/// @param level The effective level of the effect.
/// @param dam The damage driving the effect.
/// @details A room passes the effect on to every object lying in it. Food and drink containers can be spoiled.
void poison_effect(SpellTarget target, int level, int dam)
{
	if (target.IsRoom()) /* nail objects on the floor */
	{
		ROOM_INDEX_DATA *room = target.AsRoom();
		OBJ_DATA *obj, *obj_next;

		for (obj = room->contents; obj != nullptr; obj = obj_next)
		{
			obj_next = obj->next_content;
			poison_effect(obj, level, dam);
		}

		return;
	}

	if (target.IsChar()) /* do the effect on a victim */
	{
		CHAR_DATA *victim = target.AsChar();
		OBJ_DATA *obj, *obj_next;

		/* chance of poisoning */
		if (!saves_spell(level / 4 + dam / 20, victim, DAM_POISON))
		{
			AFFECT_DATA af;

			send_to_char("You feel poison coursing through your veins.\n\r", victim);
			act("$n looks very ill.", victim, nullptr, nullptr, TO_ROOM);

			init_affect(&af);
			af.where = TO_AFFECTS;
			af.aftype = AFT_SPELL;
			af.type = gsn_poison;
			af.level = level;
			af.duration = level / 2;
			af.location = APPLY_STR;
			af.modifier = -1;
			SET_BIT(af.bitvector, AFF_POISON);
			affect_join(victim, &af);
		}

		/* equipment */
		for (obj = victim->carrying; obj != nullptr; obj = obj_next)
		{
			obj_next = obj->next_content;
			poison_effect(obj, level, dam);
		}

		return;
	}

	if (target.IsObj()) /* do some poisoning */
	{
		OBJ_DATA *obj = target.AsObj();
		int chance;

		if (is_obj_stat(obj, ITEM_BURN_PROOF) || is_obj_stat(obj, ITEM_BLESS) || number_range(0, 4) == 0)
			return;

		chance = level / 4 + dam / 10;

		if (chance > 25)
			chance = (chance - 25) / 2 + 25;

		if (chance > 50)
			chance = (chance - 50) / 2 + 50;

		chance -= obj->level * 2;

		switch (obj->item_type)
		{
			case ITEM_FOOD:
				break;
			case ITEM_DRINK_CON:
				if (obj->value[0] == obj->value[1])
					return;
				break;
			default:
				return;
		}

		chance = URANGE(5, chance, 95);

		if (number_percent() > chance)
			return;

		obj->value[3] = 1;
	}
}

/// Applies a shock effect to whatever the target holds.
/// @param target What the effect is aimed at. May be a room, a character or an object.
/// @param level The effective level of the effect.
/// @param dam The damage driving the effect.
/// @details A room passes the effect on to every object lying in it. A character may be stunned.
void shock_effect(SpellTarget target, int level, int dam)
{
	if (target.IsRoom())
	{
		ROOM_INDEX_DATA *room = target.AsRoom();
		OBJ_DATA *obj, *obj_next;

		for (obj = room->contents; obj != nullptr; obj = obj_next)
		{
			obj_next = obj->next_content;
			shock_effect(obj, level, dam);
		}

		return;
	}

	if (target.IsChar())
	{
		CHAR_DATA *victim = target.AsChar();
		OBJ_DATA *obj, *obj_next;

		/* toast some gear */
		for (obj = victim->carrying; obj != nullptr; obj = obj_next)
		{
			obj_next = obj->next_content;
			shock_effect(obj, level, dam);
		}

		return;
	}

	if (target.IsObj())
	{
		OBJ_DATA *obj = target.AsObj();
		int chance;
		char *msg;

		if (is_obj_stat(obj, ITEM_BURN_PROOF) || is_obj_stat(obj, ITEM_NOPURGE) || number_range(0, 4) == 0)
			return;

		chance = level / 4 + dam / 10;

		if (chance > 25)
			chance = (chance - 25) / 2 + 25;

		if (chance > 50)
			chance = (chance - 50) / 2 + 50;

		if (is_obj_stat(obj, ITEM_BLESS))
			chance -= 5;

		chance -= obj->level * 2;

		switch (obj->item_type)
		{
			case ITEM_WAND:
			case ITEM_STAFF:
				chance += 10;
				msg = "$p overloads and explodes!";
				break;
			case ITEM_JEWELRY:
				chance -= 10;
				msg = "$p is fused into a worthless lump.";
				break;
			default:
				return;
		}

		chance = URANGE(5, chance, 95);

		if (number_percent() > chance)
			return;

		if (CHAR_DATA *carrier = Deref(obj->carried_by))
			act(msg, carrier, obj, nullptr, TO_ALL);
		else if (obj->in_room != nullptr && obj->in_room->people != nullptr)
			act(msg, obj->in_room->people, obj, nullptr, TO_ALL);

		extract_obj(obj);
	}
}
