#ifndef PERSISTED_ENUM_H
#define PERSISTED_ENUM_H

#include "enums.h"

//
// The glue between the saved files and the typed enumerations.
//
// A player file and an area file store these values as the plain integers they
// have always been, so reading one is a cast and writing one is a cast back.
// What this adds is a report when a stored value is not one the code has
// a name for.
//
// It reports and keeps. It does not drop, clamp or default, because a save
// writes back what the load produced, and a value that the load quietly
// replaced would be a change to somebody's file that nobody asked for. Only an
// edit command should alter what a file holds. Keeping is possible because
// these enumerations fix their underlying type as int, so every int is a
// representable value of the type and an unnamed one round trips unchanged.
//
// A family opts in by specializing PersistedEnum with the values it names.
//

/// Reports a stored value with no enumerator, once per read.
void report_unnamed_persisted(const char *family, const char *field, int raw);

/// The values a family names. Specialize per family; there is deliberately no
/// generic definition, so a family that has not been reviewed will not compile
/// through this seam by accident.
template <class E>
struct PersistedEnum;

template <>
struct PersistedEnum<Sex>
{
	static constexpr const char *Family = "Sex";
	static constexpr Sex Named[] = {SEX_NEUTRAL, SEX_MALE, SEX_FEMALE, SEX_EITHER};
};

template <>
struct PersistedEnum<Size>
{
	static constexpr const char *Family = "Size";
	static constexpr Size Named[] = {SIZE_TINY, SIZE_SMALL, SIZE_MEDIUM, SIZE_LARGE,
									 SIZE_HUGE, SIZE_GIANT, SIZE_IMMENSE};
};

template <>
struct PersistedEnum<SectorType>
{
	static constexpr const char *Family = "SectorType";
	static constexpr SectorType Named[] = {
		SECT_NONE, SECT_CITY, SECT_FIELD, SECT_FOREST, SECT_HILLS, SECT_MOUNTAIN,
		SECT_WATER, SECT_INSIDE, SECT_UNDERWATER, SECT_AIR, SECT_DESERT, SECT_ROAD,
		SECT_CONFLAGRATION, SECT_BURNING, SECT_TRAIL, SECT_SWAMP, SECT_PARK,
		SECT_VERTICAL, SECT_ICE, SECT_SNOW, SECT_CAVE};
};

template <>
struct PersistedEnum<CharClass>
{
	static constexpr const char *Family = "CharClass";
	static constexpr CharClass Named[] = {
		CLASS_NONE, CLASS_WARRIOR, CLASS_THIEF, CLASS_ZEALOT, CLASS_PALADIN,
		CLASS_ANTI_PALADIN, CLASS_RANGER, CLASS_ASSASSIN, CLASS_SHAPESHIFTER,
		CLASS_HEALER, CLASS_NECROMANCER, CLASS_SORCERER};
};

template <>
struct PersistedEnum<AffectWhere>
{
	static constexpr const char *Family = "AffectWhere";
	static constexpr AffectWhere Named[] = {TO_AFFECTS, TO_OBJECT, TO_IMMUNE,
											TO_RESIST, TO_VULN, TO_WEAPON};
};

template <>
struct PersistedEnum<RoomAffectWhere>
{
	static constexpr const char *Family = "RoomAffectWhere";
	static constexpr RoomAffectWhere Named[] = {TO_ROOM_AFFECTS, TO_ROOM_CONST, TO_ROOM_FLAGS};
};

template <>
struct PersistedEnum<ObjAffectWhere>
{
	static constexpr const char *Family = "ObjAffectWhere";
	static constexpr ObjAffectWhere Named[] = {TO_OBJ_AFFECTS, TO_OBJ_APPLY};
};

template <>
struct PersistedEnum<ApplyLocation>
{
	static constexpr const char *Family = "ApplyLocation";
	static constexpr ApplyLocation Named[] = {
		APPLY_NONE, APPLY_STR, APPLY_DEX, APPLY_INT, APPLY_WIS, APPLY_CON,
		APPLY_SEX, APPLY_CLASS, APPLY_LUCK, APPLY_AGE, APPLY_HEIGHT,
		APPLY_WEIGHT, APPLY_MANA, APPLY_HIT, APPLY_MOVE, APPLY_GOLD, APPLY_EXP,
		APPLY_AC, APPLY_HITROLL, APPLY_DAMROLL, APPLY_SAVES, APPLY_SAVING_PARA,
		APPLY_SAVING_ROD, APPLY_SAVING_PETRI, APPLY_SAVING_BREATH,
		APPLY_SAVING_SPELL, APPLY_SPELL_AFFECT, APPLY_CARRY_WEIGHT,
		APPLY_DEFENSE, APPLY_REGENERATION, APPLY_SIZE, APPLY_ENERGYSTATE,
		APPLY_DAM_MOD, APPLY_LEGS, APPLY_ARMS, APPLY_BEAUTY, APPLY_ALIGNMENT,
		APPLY_ETHOS, APPLY_OBJ_PROPERTIES};
};

template <>
struct PersistedEnum<WearLocation>
{
	static constexpr const char *Family = "WearLocation";
	static constexpr WearLocation Named[] = {
		WEAR_NONE, WEAR_LIGHT, WEAR_FINGER_L, WEAR_FINGER_R, WEAR_NECK_1,
		WEAR_NECK_2, WEAR_BODY, WEAR_HEAD, WEAR_LEGS, WEAR_FEET, WEAR_HANDS,
		WEAR_ARMS, WEAR_SHIELD, WEAR_ABOUT, WEAR_WAIST, WEAR_WRIST_L,
		WEAR_WRIST_R, WEAR_WIELD, WEAR_HOLD, WEAR_DUAL_WIELD, WEAR_BRAND,
		WEAR_STRAPPED, WEAR_COSMETIC};
};

template <>
struct PersistedEnum<Position>
{
	static constexpr const char *Family = "Position";
	static constexpr Position Named[] = {
		POS_DEAD, POS_MORTAL, POS_INCAP, POS_STUNNED, POS_SLEEPING,
		POS_RESTING, POS_SITTING, POS_FIGHTING, POS_STANDING};
};

template <>
struct PersistedEnum<ItemType>
{
	static constexpr const char *Family = "ItemType";
	static constexpr ItemType Named[] = {
		ITEM_LIGHT, ITEM_SCROLL, ITEM_WAND, ITEM_STAFF, ITEM_WEAPON, ITEM_NULL6,
		ITEM_DICE, ITEM_TREASURE, ITEM_ARMOR, ITEM_POTION, ITEM_CLOTHING,
		ITEM_FURNITURE, ITEM_TRASH, ITEM_CONTAINER, ITEM_DRINK_CON, ITEM_KEY,
		ITEM_FOOD, ITEM_MONEY, ITEM_BOAT, ITEM_CORPSE_NPC, ITEM_CORPSE_PC,
		ITEM_FOUNTAIN, ITEM_PILL, ITEM_PROTECT, ITEM_MAP, ITEM_PORTAL,
		ITEM_WARP_STONE, ITEM_ROOM_KEY, ITEM_GEM, ITEM_JEWELRY, ITEM_CAMPFIRE,
		ITEM_CABAL_ITEM, ITEM_SKELETON, ITEM_URN, ITEM_GRAVITYWELL, ITEM_BOOK,
		ITEM_PEN, ITEM_ALTAR, ITEM_STONE};
};

template <>
struct PersistedEnum<DamageType>
{
	static constexpr const char *Family = "DamageType";
	static constexpr DamageType Named[] = {
		DAM_NONE, DAM_BASH, DAM_PIERCE, DAM_SLASH, DAM_FIRE, DAM_COLD,
		DAM_LIGHTNING, DAM_ACID, DAM_POISON, DAM_NEGATIVE, DAM_HOLY, DAM_ENERGY,
		DAM_MENTAL, DAM_DISEASE, DAM_DROWNING, DAM_LIGHT, DAM_OTHER, DAM_CHARM,
		DAM_SOUND, DAM_TRUESTRIKE, DAM_INTERNAL};
};

/// Whether the stored integer is one of the values the family names.
template <class E>
bool is_named_value(int raw)
{
	for (E value : PersistedEnum<E>::Named)
	{
		if (static_cast<int>(value) == raw)
			return true;
	}

	return false;
}

/// Reads a stored integer as a typed value, reporting one that has no name.
template <class E>
E read_persisted(int raw, const char *field)
{
	if (!is_named_value<E>(raw))
		report_unnamed_persisted(PersistedEnum<E>::Family, field, raw);

	return static_cast<E>(raw);
}

/// The integer a saved file holds for this value.
template <class E>
constexpr int write_persisted(E value)
{
	return static_cast<int>(value);
}

#endif /* PERSISTED_ENUM_H */
