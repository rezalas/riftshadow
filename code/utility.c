#include "utility.h"
#include "db.h"
#include "chardef.h"
#include "handler.h"

/// Checks if the given character is a cabal guard.
/// @param ch: The character to check for guard status.
/// @returns true if the character is a cabal guard; false otherwise.
bool is_cabal_guard (CHAR_DATA *ch)
{
	if (ch == nullptr)
		return false;

	return is_npc(ch) && IS_SET(ch->act, ACT_INNER_GUARDIAN);
}

/// Checks if the given character is an NPC.
/// @param ch: The character to check for NPC status.
/// @returns true if the character is an NPC; false otherwise.
bool is_npc (CHAR_DATA *ch)
{
	if (ch == nullptr)
		return false;

	return IS_SET(ch->act, ACT_IS_NPC);
}

/// Checks if the given character is an immortal.
/// @param ch: The character to check for immortal status.
/// @returns true if the character is an immortal; false otherwise.
bool is_immortal (CHAR_DATA *ch)
{
	if (ch == nullptr)
		return false;

	auto trust = get_trust(ch);
	return trust >= LEVEL_IMMORTAL;
}

/// Checks if the given character is a hero.
/// @param ch: The character to check for hero status.
/// @returns true if the character is a hero; false otherwise.
bool is_hero (CHAR_DATA *ch)
{
	if (ch == nullptr)
		return false;

	auto trust = get_trust(ch);
	return trust >= LEVEL_HERO;
}

bool is_heroimm (CHAR_DATA *ch)
{
	if (ch == nullptr)
		return false;

	return IS_SET(ch->act, PLR_HEROIMM);
}

/// Checks if the given character is at or above the given trust level.
/// @param ch: The character whose trust level to check.
/// @param level: The trust level to check.
/// @returns true if the character meets the trust level; false otherwise.
bool is_trusted (CHAR_DATA *ch, int level)
{
	if (ch == nullptr)
		return false;

	auto trust = get_trust(ch);
	return trust >= level;
}

/// Checks if the given character is affected by the given affect gsn.
/// @param ch: The character to check for the applied affect.
/// @param sn: The affect gsn to check.
/// @returns true if the character is affected; false otherwise.
bool is_affected_by (CHAR_DATA *ch, int sn)
{
	if (ch == nullptr)
		return false;

	return IS_SET(ch->affected_by, sn);
}

/// Checks if the given object is affected by the given affect gsn.
/// @param ch: The object to check for the applied affect.
/// @param sn: The affect gsn to check.
/// @returns true if the object is affected; false otherwise.
bool is_affected_by (OBJ_DATA *obj, int sn)
{
	if (obj == nullptr)
		return false;

	return IS_SET(obj->affected_by, sn);
}

bool is_ansi (CHAR_DATA *ch)
{
	if (ch == nullptr)
		return false;

	return !is_npc(ch) && IS_SET(ch->comm, COMM_ANSI);
}

/// Checks if the given character's alignment is good.
/// @param ch: The character whose alignment to check.
/// @returns true if the character's alignment is good; false otherwise.
bool is_good (CHAR_DATA *ch)
{
	if (ch == nullptr)
		return false;

	auto alignment = get_align(ch);
	return alignment > 333;
}

/// Checks if the given character's alignment is evil.
/// @param ch: The character whose alignment to check.
/// @returns true if the character's alignment is evil; false otherwise.
bool is_evil (CHAR_DATA *ch)
{
	if (ch == nullptr)
		return false;

	auto alignment = get_align(ch);
	return alignment < -333;
}

/// Checks if the given character's alignment is neutral.
/// @param ch: The character whose alignment to check.
/// @returns true if the character's alignment is neutral; false otherwise.
bool is_neutral (CHAR_DATA *ch)
{
	if (ch == nullptr)
		return false;

	return !is_good(ch) && !is_evil(ch);
}

/// Checks if the given character's ethos is lawful.
/// @param ch: The character whose ethos to check.
/// @returns true if the character's ethos is lawful; false otherwise.
bool is_lawful (CHAR_DATA *ch)
{
	if (ch == nullptr)
		return false;

	auto ethos = get_ethos(ch);
	return ethos > 333;
}

/// Checks if the given character's ethos is chaotic.
/// @param ch: The character whose ethos to check.
/// @returns true if the character's ethos is chaotic; false otherwise.
bool is_chaotic (CHAR_DATA *ch)
{
	if (ch == nullptr)
		return false;

	auto ethos = get_ethos(ch);
	return ethos  < -333;
}

/// Checks if the given character's ethos is neutral.
/// @param ch: The character whose ethos to check.
/// @returns true if the character's ethos is neutral; false otherwise.
bool is_eneutral (CHAR_DATA *ch)
{
	if (ch == nullptr)
		return false;

	return !is_lawful(ch) && !is_chaotic(ch);
}

/// Checks if the two given numbers represent the same alignment.
/// @param one: The first number whose alignment to check.
/// @param two: The second number whose alignment to check.
/// @returns true if the two characters are of opposite alignment; false otherwise.
bool is_same_align (short one, short two)
{
	return (one < 333 && two < 333)
		|| (one > -333 && one < 333 && two > -333 && two < 333)
		|| (one > 333 && two > 333);
}

/// Checks if the two given characters are of opposite alignments.
/// @param ch: The first character whose alignment to check.
/// @param vch: The second character whose alignment to check.
/// @returns true if the two characters are of opposite alignment; false otherwise.
bool is_opp_align (CHAR_DATA *ch, CHAR_DATA *vch)
{
	if(ch == nullptr || vch == nullptr)
		return false;

	return (is_good(ch) && is_evil(vch)) || (is_evil(ch) && is_good(vch));
}

/// Checks if the given number corresponds to the good alignment.
/// @param num: The number whose alignment to check.
/// @returns true if the number corresponds to the good alignment; false otherwise.
bool is_num_good (short num)
{
	return num > 333;
}

/// Checks if the given number corresponds to the evil alignment.
/// @param num: The number whose alignment to check.
/// @returns true if the number corresponds to the evil alignment; false otherwise.
bool is_num_evil (short num)
{
	return num < -333;
}

/// Checks if the given number corresponds to the neutral alignment.
/// @param num: The number whose alignment to check.
/// @returns true if the number corresponds to the neutral alignment; false otherwise.
bool is_num_neutral (short num)
{
	return !is_num_good(num) && !is_num_evil(num);
}

/// Checks if the given number corresponds to the lawful ethos.
/// @param num: The number whose ethos to check.
/// @returns true if the number corresponds to the lawful ethos; false otherwise.
bool is_num_lawful (short num)
{
	return num > 333;
}

/// Checks if the given number corresponds to the chaotic ethos.
/// @param num: The number whose ethos to check.
/// @returns true if the number corresponds to the chaotic ethos; false otherwise.
bool is_num_chaotic (short num)
{
	return num < -333;
}

/// Checks if the given number corresponds to the neutral ethos.
/// @param num: The number whose ethos to check.
/// @returns true if the number corresponds to the neutral ethos; false otherwise.
bool is_num_eneutral (short num)
{
	return !is_num_lawful(num) && !is_num_chaotic(num);
}

/// Checks if the given character's state is awake.
/// @param ch: The character whose state to check.
/// @returns true if the character's state is awake; false otherwise.
bool is_awake(CHAR_DATA *ch)
{
	if (ch == nullptr)
		return false;

	return ch->position > POS_SLEEPING;
}

/// Retrieves the given character's armor class of the given type.
/// @param ch: The character whose armor class to retrieve.
/// @param type: The armor type the character is wearing.
/// @returns The armor class of the piece of armor the character is wearing.
short get_ac (CHAR_DATA *ch, short type)
{
	if (ch == nullptr || type > std::size(ch->armor))
		return false;

	return ch->armor[type];
}

/// Calculates the given character's hit roll with modifiers applied.
/// @param ch: The character whose hit roll to calculate.
/// @returns The character's tabulated hit roll.
short get_hitroll (CHAR_DATA *ch)
{
	if (ch == nullptr)
		return 0;

	auto stat = get_curr_stat(ch, STAT_DEX);
	return ch->hitroll + dex_app[stat].tohit;
}

/// Calculates the given character's damage roll with modifiers applied.
/// @param ch: The character whose damage roll to calculate.
/// @returns The character's tabulated damage roll.
short get_damroll (CHAR_DATA *ch)
{
	if (ch == nullptr)
		return 0;

	auto stat = get_curr_stat(ch, STAT_STR);
	return ch->damroll + str_app[stat].todam;
}

/// Checks if the given character is outside.
/// @param ch: The character whose location to check.
/// @returns true if the character is outside; false otherwise.
bool is_outside (CHAR_DATA *ch)
{
	if (ch == nullptr)
		return false;

	return !IS_SET(ch->in_room->room_flags, ROOM_INDOORS)
		&& ch->in_room->sector_type != SECT_INSIDE
		&& ch->in_room->sector_type != SECT_UNDERWATER;
}

/// Retrieves the current weight that the given character's is carrying.
/// @param ch: The character whose current carry weight to retrieve.
/// @returns The amount of weight the character is currently carrying.
long get_carry_weight(CHAR_DATA *ch)
{
	if (ch == nullptr)
		return 0;

	return ch->carry_weight + ch->gold/10000;
}

/// Checks if the given character's state is shifted.
/// @param ch: The character whose state to check.
/// @returns true if the character's state is shifted; false otherwise.
bool is_shifted (CHAR_DATA *ch)
{
	return false;
}

/// Checks if the given character's state is hungry.
/// @param ch: The character whose state to check.
/// @returns true if the character's state is hungry; false otherwise.
bool is_hungry (CHAR_DATA *ch)
{
	if (is_npc(ch))
		return false;
	
	return ch->pcdata->condition[COND_HUNGER] > COND_HUNGRY;
}

/// Checks if the given character's state is thirsty.
/// @param ch: The character whose state to check.
/// @returns true if the character's state is thirsty; false otherwise.
bool is_thirsty (CHAR_DATA *ch)
{
	if (is_npc(ch))
		return false;
		
	return ch->pcdata->condition[COND_THIRST] > COND_HUNGRY;
}

/// Checks if the given room is affected by the given affect.
/// @param room: The room to check for the given affect.
/// @param sn: The affect to check against.
/// @returns true if the room is affected by the affect; false otherwise.
bool is_room_affected (ROOM_INDEX_DATA *room, int sn)
{
	if (room == nullptr)
		return false;

	return IS_SET(room->affected_by, sn);
}

/// Checks if the given room's sector type is ground.
/// @param room: The room whose sector type to check.
/// @returns true if the room's sector type is ground; false otherwise.
bool is_ground (ROOM_INDEX_DATA *room)
{
	if (room == nullptr)
		return false;

	return room->sector_type != SECT_AIR
		&& room->sector_type != SECT_WATER
		&& room->sector_type != SECT_UNDERWATER
		&& room->sector_type != SECT_VERTICAL;
}

/// Checks if the given room's sector type is water.
/// @param room: The room whose sector type to check.
/// @returns true if the room's sector type is water; false otherwise.
bool is_water (ROOM_INDEX_DATA *room)
{
	if (room == nullptr)
		return false;

	return room->sector_type == SECT_WATER
		|| room->sector_type == SECT_UNDERWATER;
}

/// Checks if the given object is wearable on the given part.
/// @param obj: The object to check for wearability.
/// @param part: The part to place the object if it's wearable.
/// @returns true if the object can be worn on part; false otherwise.
bool can_wear (OBJ_DATA *obj, int part)
{
	if (obj == nullptr)
		return false;

	return IS_SET(obj->wear_flags, part);
}

bool is_obj_stat (OBJ_DATA *obj, int stat)
{
	if (obj == nullptr)
		return false;

	return IS_SET(obj->extra_flags, stat);
}

bool is_weapon_stat (OBJ_DATA *obj, int stat)
{
	if (obj == nullptr)
		return false;

	return IS_SET_OLD(obj->value[4], stat);
}

int weight_mult (OBJ_DATA *obj)
{
	if (obj == nullptr)
		return false;

	return obj->item_type == ITEM_CONTAINER
		? obj->value[4]
		: 100;
}

/// Checks if the given room is explorable.
/// @param room: The room to check for explorability.
/// @returns true if the room is explorable; false otherwise.
bool is_explore (ROOM_INDEX_DATA *room)
{
	if (room == nullptr)
		return false;

	return IS_SET(room->room_flags, ROOM_AREA_EXPLORE)
		|| IS_SET(room->area->area_flags, AREA_EXPLORE);
}

/// Checks if the given object is made of metal.
/// @param obj: The object whose composition to check.
/// @returns true if the object is made of metal; false otherwise.
bool is_metal (OBJ_DATA *obj)
{
	if (obj == nullptr)
		return false;

	return !str_cmp(obj->material, "iron")
		|| !str_cmp(obj->material, "steel")
		|| !str_cmp(obj->material, "mithril")
		|| !str_cmp(obj->material, "copper")
		|| !str_cmp(obj->material, "gold")
		|| !str_cmp(obj->material, "silver")
		|| !str_cmp(obj->material, "platinum")
		|| !str_cmp(obj->material, "brass")
		|| !str_cmp(obj->material, "bronze")
		|| !str_cmp(obj->material, "metal")
		|| !str_cmp(obj->material, "pewter")
		|| !str_cmp(obj->material, "lead")
		|| !str_cmp(obj->material, "electrum")
		|| !str_cmp(obj->material, "tin");
}

/// Checks if the given object is made of stone.
/// @param obj: The object whose composition to check.
/// @returns true if the object is made of stone; false otherwise.
bool is_stone (OBJ_DATA *obj)
{
	if (obj == nullptr)
		return false;

	return !str_cmp(obj->material, "ceramic")
		|| !str_cmp(obj->material, "porcelain")
		|| !str_cmp(obj->material, "marble")
		|| !str_cmp(obj->material, "stone")
		|| !str_cmp(obj->material, "quartz")
		|| !str_cmp(obj->material, "corundum")
		|| !str_cmp(obj->material, "flint")
		|| !str_cmp(obj->material, "lodestone")
		|| !str_cmp(obj->material, "granite")
		|| !str_cmp(obj->material, "enamel")
		|| !str_cmp(obj->material, "obsidian")
		|| !str_cmp(obj->material, "glass")
		|| !str_cmp(obj->material, "pottery")
		|| !str_cmp(obj->material, "crystal")
		|| !str_cmp(obj->material, "coral")
		|| !str_cmp(obj->material, "sandstone")
		|| !str_cmp(obj->material, "clay")
		|| !str_cmp(obj->material, "earth")
		|| !str_cmp(obj->material, "ceramic");
}

/// Checks if the given object can rust.
/// @param obj: The object whose composition to check.
/// @returns true if the object can rust; false otherwise.
bool can_rust (OBJ_DATA *obj)
{
	if (obj == nullptr)
		return false;

	return !str_cmp(obj->material, "iron")
		|| !str_cmp(obj->material, "steel")
		|| !str_cmp(obj->material, "copper")
		|| !str_cmp(obj->material, "brass")
		|| !str_cmp(obj->material, "bronze")
		|| !str_cmp(obj->material, "metal")
		|| !str_cmp(obj->material, "pewter")
		|| !str_cmp(obj->material, "lead")
		|| !str_cmp(obj->material, "tin");
}

char* pers (CHAR_DATA *ch, CHAR_DATA *looker)
{
	if (ch == nullptr || looker == nullptr)
		return "someone";

	if (can_see(looker, ch) && !is_affected(looker, gsn_plasma_arc))
		return is_npc(ch) ? ch->short_descr : ch->name;

	if (is_immortal(ch) && !is_npc(ch) && ch->invis_level > 50 && (!ch->desc || !ch->desc->original))
		return "An Immortal";

	return "someone";
}


bool is_switched (CHAR_DATA *ch)
{
	if (ch == nullptr)
		return false;

	return ch->desc && ch->desc->original;	// ROM OLC
}
