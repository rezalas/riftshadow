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
#include "effects.h"
#include "handler.h"
#include "recycle.h"
#include "comm.h"
#include "update.h"
#include "db.h"
#include "magic.h"
#include "utility.h"

void acid_effect(void *vo, int level, int dam, int target)
{
	if (target == TARGET_ROOM) /* nail objects on the floor */
	{
		ROOM_INDEX_DATA *room = (ROOM_INDEX_DATA *)vo;
		OBJ_DATA *obj, *obj_next;

		for (obj = room->contents; obj != nullptr; obj = obj_next)
		{
			obj_next = obj->next_content;
			acid_effect(obj, level, dam, TARGET_OBJ);
		}

		return;
	}

	if (target == TARGET_CHAR) /* do the effect on a victim */
	{
		CHAR_DATA *victim = (CHAR_DATA *)vo;
		OBJ_DATA *obj, *obj_next;

		/* let's toast some gear */
		for (obj = victim->carrying; obj != nullptr; obj = obj_next)
		{
			obj_next = obj->next_content;
			acid_effect(obj, level, dam, TARGET_OBJ);
		}

		return;
	}

	if (target == TARGET_OBJ) /* toast an object */
	{
		OBJ_DATA *obj = (OBJ_DATA *)vo;
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

		if (obj->carried_by != nullptr)
			act(msg, obj->carried_by, obj, nullptr, TO_ALL);
		else if (obj->in_room != nullptr && obj->in_room->people != nullptr)
			act(msg, obj->in_room->people, obj, nullptr, TO_ALL);

		if (obj->item_type == ITEM_ARMOR) /* etch it */
		{
			OBJ_AFFECT_DATA *paf;
			bool af_found = false;
			int i;

			for (paf = obj->affected; paf != nullptr; paf = paf->next)
			{
				if (paf->location == APPLY_AC)
				{
					af_found = true;
					paf->type = -1;
					paf->modifier += 1;
					paf->level = std::max((int)paf->level, level);
					break;
				}
			}

			/* needs a new affect */
			if (!af_found)
			{
				paf = new_affect_obj();

				paf->type = -1;
				paf->level = level;
				paf->duration = -1;
				paf->location = APPLY_AC;
				paf->modifier = 1;
				paf->next = obj->affected;
				obj->affected = paf;
			}

			if (obj->carried_by != nullptr && obj->wear_loc != WEAR_NONE)
			{
				for (i = 0; i < 4; i++)
				{
					obj->carried_by->armor[i] += 1;
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
				else if (obj->carried_by != nullptr)
				{
					obj_to_room(t_obj, obj->carried_by->in_room);
				}
				else
				{
					extract_obj(t_obj);
					continue;
				}

				acid_effect(t_obj, level / 2, dam / 2, TARGET_OBJ);
			}
		}

		extract_obj(obj);
	}
}

void cold_effect(void *vo, int level, int dam, int target)
{
	if (target == TARGET_ROOM) /* nail objects on the floor */
	{
		ROOM_INDEX_DATA *room = (ROOM_INDEX_DATA *)vo;
		OBJ_DATA *obj, *obj_next;

		for (obj = room->contents; obj != nullptr; obj = obj_next)
		{
			obj_next = obj->next_content;
			cold_effect(obj, level, dam, TARGET_OBJ);
		}

		return;
	}

	if (target == TARGET_CHAR) /* whack a character */
	{
		CHAR_DATA *victim = (CHAR_DATA *)vo;
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
			cold_effect(obj, level, dam, TARGET_OBJ);
		}

		return;
	}

	if (target == TARGET_OBJ) /* toast an object */
	{
		OBJ_DATA *obj = (OBJ_DATA *)vo;
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

		if (obj->carried_by != nullptr)
			act(msg, obj->carried_by, obj, nullptr, TO_ALL);
		else if (obj->in_room != nullptr && obj->in_room->people != nullptr)
			act(msg, obj->in_room->people, obj, nullptr, TO_ALL);

		extract_obj(obj);
	}
}

void fire_effect(void *vo, int level, int dam, int target)
{
	if (target == TARGET_ROOM) /* nail objects on the floor */
	{
		ROOM_INDEX_DATA *room = (ROOM_INDEX_DATA *)vo;
		OBJ_DATA *obj, *obj_next;

		for (obj = room->contents; obj != nullptr; obj = obj_next)
		{
			obj_next = obj->next_content;
			fire_effect(obj, level, dam, TARGET_OBJ);
		}

		return;
	}

	if (target == TARGET_CHAR) /* do the effect on a victim */
	{
		CHAR_DATA *victim = (CHAR_DATA *)vo;
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

			fire_effect(obj, level, dam, TARGET_OBJ);
		}

		return;
	}

	if (target == TARGET_OBJ) /* toast an object */
	{
		OBJ_DATA *obj = (OBJ_DATA *)vo;
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

		if (obj->carried_by != nullptr)
			act(msg, obj->carried_by, obj, nullptr, TO_ALL);
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
				else if (obj->carried_by != nullptr)
				{
					obj_to_room(t_obj, obj->carried_by->in_room);
				}
				else
				{
					extract_obj(t_obj);
					continue;
				}

				fire_effect(t_obj, level / 2, dam / 2, TARGET_OBJ);
			}
		}

		extract_obj(obj);
	}
}

void poison_effect(void *vo, int level, int dam, int target)
{
	if (target == TARGET_ROOM) /* nail objects on the floor */
	{
		ROOM_INDEX_DATA *room = (ROOM_INDEX_DATA *)vo;
		OBJ_DATA *obj, *obj_next;

		for (obj = room->contents; obj != nullptr; obj = obj_next)
		{
			obj_next = obj->next_content;
			poison_effect(obj, level, dam, TARGET_OBJ);
		}

		return;
	}

	if (target == TARGET_CHAR) /* do the effect on a victim */
	{
		CHAR_DATA *victim = (CHAR_DATA *)vo;
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
			poison_effect(obj, level, dam, TARGET_OBJ);
		}

		return;
	}

	if (target == TARGET_OBJ) /* do some poisoning */
	{
		OBJ_DATA *obj = (OBJ_DATA *)vo;
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

void shock_effect(void *vo, int level, int dam, int target)
{
	if (target == TARGET_ROOM)
	{
		ROOM_INDEX_DATA *room = (ROOM_INDEX_DATA *)vo;
		OBJ_DATA *obj, *obj_next;

		for (obj = room->contents; obj != nullptr; obj = obj_next)
		{
			obj_next = obj->next_content;
			shock_effect(obj, level, dam, TARGET_OBJ);
		}

		return;
	}

	if (target == TARGET_CHAR)
	{
		CHAR_DATA *victim = (CHAR_DATA *)vo;
		OBJ_DATA *obj, *obj_next;

		/* toast some gear */
		for (obj = victim->carrying; obj != nullptr; obj = obj_next)
		{
			obj_next = obj->next_content;
			shock_effect(obj, level, dam, TARGET_OBJ);
		}

		return;
	}

	if (target == TARGET_OBJ)
	{
		OBJ_DATA *obj = (OBJ_DATA *)vo;
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
			default:
				return;
		}

		chance = URANGE(5, chance, 95);

		if (number_percent() > chance)
			return;

		if (obj->carried_by != nullptr)
			act(msg, obj->carried_by, obj, nullptr, TO_ALL);
		else if (obj->in_room != nullptr && obj->in_room->people != nullptr)
			act(msg, obj->in_room->people, obj, nullptr, TO_ALL);

		extract_obj(obj);
	}
}
