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
