#include <type_traits>

#include "catch.hpp"
#include "../code/persisted_enum.h"
#include "../code/enums.h"
#include "../code/utility.h"
#include "../code/merc.h"
#include "../code/lookup.h"
#include "../code/tables.h"
#include "../code/const.h"

SCENARIO("reading a stored value into a typed enumeration", "[persisted_enum]")
{
	GIVEN("a value the enumeration names")
	{
		THEN("it reads back as that value, and writes back as the same integer")
		{
			REQUIRE(read_persisted<Sex>(0, "Sex") == SEX_NEUTRAL);
			REQUIRE(read_persisted<Sex>(1, "Sex") == SEX_MALE);
			REQUIRE(read_persisted<Sex>(2, "Sex") == SEX_FEMALE);
			REQUIRE(read_persisted<Sex>(3, "Sex") == SEX_EITHER);

			REQUIRE(write_persisted(SEX_NEUTRAL) == 0);
			REQUIRE(write_persisted(SEX_MALE) == 1);
			REQUIRE(write_persisted(SEX_FEMALE) == 2);
			REQUIRE(write_persisted(SEX_EITHER) == 3);
		}
	}

	GIVEN("a stored value the enumeration does not name")
	{
		// It should report one of these. It must not change it: a save writes
		// back what the load produced, so replacing the value here would edit
		// somebody's file without anybody asking for it.
		WHEN("it is read and written back")
		{
			THEN("the same integer comes out")
			{
				REQUIRE(write_persisted(read_persisted<Sex>(7, "Sex")) == 7);
				REQUIRE(write_persisted(read_persisted<Sex>(-4, "Sex")) == -4);
				REQUIRE(write_persisted(read_persisted<Sex>(9999, "Sex")) == 9999);
			}
		}
	}

	GIVEN("the values a family names")
	{
		THEN("only those are recognised")
		{
			REQUIRE(is_named_value<Sex>(0));
			REQUIRE(is_named_value<Sex>(3));
			REQUIRE_FALSE(is_named_value<Sex>(4));
			REQUIRE_FALSE(is_named_value<Sex>(-1));
		}
	}
}

SCENARIO("telling a character's sex from a prototype's instruction", "[is_character_sex]")
{
	GIVEN("the three a character can be")
	{
		THEN("each is accepted")
		{
			REQUIRE(is_character_sex(SEX_NEUTRAL));
			REQUIRE(is_character_sex(SEX_MALE));
			REQUIRE(is_character_sex(SEX_FEMALE));
		}
	}

	GIVEN("a prototype asking for one to be picked, and a value with no name")
	{
		THEN("neither is a sex a character can be")
		{
			REQUIRE_FALSE(is_character_sex(SEX_EITHER));
			REQUIRE_FALSE(is_character_sex(static_cast<Sex>(11)));
		}
	}
}

SCENARIO("reading a stored size", "[persisted_enum]")
{
	GIVEN("the sizes the enumeration names, and one it does not")
	{
		THEN("the named ones read back, and the unnamed one survives the trip")
		{
			REQUIRE(read_persisted<Size>(0, "size") == SIZE_TINY);
			REQUIRE(read_persisted<Size>(2, "size") == SIZE_MEDIUM);
			REQUIRE(read_persisted<Size>(6, "size") == SIZE_IMMENSE);
			REQUIRE(write_persisted(read_persisted<Size>(9, "size")) == 9);
		}
	}
}

SCENARIO("measuring the gap between two sizes", "[size_difference]")
{
	// Sizes are an ascending ordinal and the skills that care about size were
	// written as arithmetic on it: victim->size > ch->size + 1, ch->size <
	// victim->size - 1, and differences scaled by a constant. Those became
	// calls to size_difference, so what is checked here is that the rewriting
	// rule holds for every pair of sizes rather than for the pairs that
	// happened to come up.
	const int Smallest = 0;
	const int Largest = 6;

	GIVEN("every pair of sizes")
	{
		THEN("the gap is the difference of the ordinals, and the comparisons agree")
		{
			for (int a = Smallest; a <= Largest; a++)
			{
				for (int b = Smallest; b <= Largest; b++)
				{
					Size first = static_cast<Size>(a);
					Size second = static_cast<Size>(b);

					REQUIRE(size_difference(first, second) == a - b);
					REQUIRE((size_difference(first, second) > 1) == (a > b + 1));
					REQUIRE((size_difference(second, first) > 1) == (a + 1 < b));
					REQUIRE((size_difference(second, first) > 1) == (a < b - 1));
				}
			}
		}
	}
}

SCENARIO("reading a stored sector", "[persisted_enum]")
{
	GIVEN("the value 1,523 rooms in the area files carry")
	{
		THEN("it names the absence of a terrain rather than nothing at all")
		{
			REQUIRE(read_persisted<SectorType>(0, "sector") == SECT_NONE);
			REQUIRE(is_named_value<SectorType>(0));
		}
	}

	GIVEN("a stored sector the enumeration does not name")
	{
		THEN("it survives the trip")
		{
			REQUIRE(write_persisted(read_persisted<SectorType>(30, "sector")) == 30);
		}
	}
}

SCENARIO("changing a room's terrain and putting it back", "[sector_offset]")
{
	// A room affect that changes terrain stores an offset, not a terrain, and
	// the room reverts by having the same number taken off again. Conflagration
	// and glaciate both rely on it, and one of them computes the original
	// terrain by subtracting the offset before working out the next one.
	const int Highest = 20;

	GIVEN("every pair of terrains")
	{
		THEN("the stored offset turns one into the other, and undoes itself")
		{
			for (int from = 0; from <= Highest; from++)
			{
				for (int to = 0; to <= Highest; to++)
				{
					SectorType was = static_cast<SectorType>(from);
					SectorType becomes = static_cast<SectorType>(to);
					int offset = sector_offset(becomes, was);

					REQUIRE(sector_shifted(was, offset) == becomes);
					REQUIRE(sector_shifted(sector_shifted(was, offset), -offset) == was);
				}
			}
		}
	}
}

SCENARIO("reading a stored character class", "[persisted_enum]")
{
	GIVEN("the classes the enumeration names")
	{
		THEN("each reads back and writes back as its own number")
		{
			REQUIRE(read_persisted<CharClass>(0, "class") == CLASS_NONE);
			REQUIRE(read_persisted<CharClass>(1, "class") == CLASS_WARRIOR);
			REQUIRE(read_persisted<CharClass>(11, "class") == CLASS_SORCERER);
			REQUIRE(write_persisted(CLASS_SORCERER) == 11);
		}
	}

	GIVEN("a stored class the enumeration does not name")
	{
		THEN("it survives the trip")
		{
			REQUIRE(write_persisted(read_persisted<CharClass>(12, "class")) == 12);
			REQUIRE(write_persisted(read_persisted<CharClass>(-1, "class")) == -1);
		}
	}

	GIVEN("a slot number and the slot it names")
	{
		THEN("each is the other's inverse over every slot a character has")
		{
			for (int slot = 0; slot < MAX_WEAR; slot++)
			{
				REQUIRE(wear_index(wear_slot(slot)) == slot);
			}

			REQUIRE(wear_index(WEAR_NONE) == -1);
			REQUIRE(wear_slot(0) == WEAR_LIGHT);
			REQUIRE(wear_slot(21) == WEAR_COSMETIC);
		}

		THEN("the two names for slot eighteen are the same slot")
		{
			REQUIRE(WEAR_DUAL_WIELD == WEAR_FLOAT);
		}
	}

	GIVEN("a stored slot the enumeration does not name")
	{
		THEN("it survives the trip")
		{
			REQUIRE(write_persisted(read_persisted<WearLocation>(64, "Wear")) == 64);
			REQUIRE(write_persisted(read_persisted<WearLocation>(-7, "Wear")) == -7);
		}
	}

	GIVEN("a position and its row number")
	{
		THEN("each is the other's inverse over the whole family")
		{
			for (int row = position_index(POS_DEAD); row <= position_index(POS_STANDING); row++)
			{
				REQUIRE(position_index(position_at(row)) == row);
			}
		}

		THEN("the row lookup answers for every position and for one that is not")
		{
			REQUIRE(std::string(position_row(POS_STANDING)->name) == "standing");
			REQUIRE(std::string(position_row(POS_DEAD)->name) == "dead");

			// An area file with a start position word this does not know is
			// stored below dead, and both callers used to subscript the table
			// with it.
			REQUIRE(std::string(position_row(position_at(-1))->name) == "dead");
			REQUIRE(std::string(position_row(position_at(99))->name) == "dead");
		}
	}

	GIVEN("a stored position the enumeration does not name")
	{
		THEN("it survives the trip")
		{
			REQUIRE(write_persisted(read_persisted<Position>(-1, "Pos")) == -1);
			REQUIRE(write_persisted(read_persisted<Position>(42, "Pos")) == 42);
		}
	}

	GIVEN("a stored item type the enumeration does not name")
	{
		THEN("it survives the trip")
		{
			// Zero is what an object that has never been given a type holds,
			// and it is deliberately not a named value, so a player file that
			// carries one gets reported rather than passed over.
			REQUIRE(write_persisted(read_persisted<ItemType>(0, "Ityp")) == 0);
			REQUIRE(write_persisted(read_persisted<ItemType>(-1, "Ityp")) == -1);
			REQUIRE(write_persisted(read_persisted<ItemType>(14, "Ityp")) == 14);
		}
	}

	GIVEN("the damage class that used to be a define in merc.h")
	{
		THEN("the family names it, so the seam stops reporting it")
		{
			REQUIRE(is_named_value<DamageType>(20));
			REQUIRE(read_persisted<DamageType>(20, "damage class") == DAM_INTERNAL);
		}
	}

	GIVEN("a stored damage class the enumeration does not name")
	{
		THEN("it survives the trip")
		{
			REQUIRE(write_persisted(read_persisted<DamageType>(-1, "damage class")) == -1);
			REQUIRE(write_persisted(read_persisted<DamageType>(77, "damage class")) == 77);
		}
	}

	GIVEN("the attack table")
	{
		THEN("every attack but the no-attack row carries a damage class the family names")
		{
			// Row zero is the mob with no attack, and its damage class is
			// deliberately not one. Every other row has to be a real class, or
			// the immunity check falls through to its default and the attack
			// silently ignores resistances.
			for (size_t row = 1; row < attack_table.size(); row++)
			{
				if (attack_table[row].name == nullptr)
					continue;

				INFO("attack row " << row);
				REQUIRE(is_named_value<DamageType>(write_persisted(attack_table[row].damage)));
			}
		}

		THEN("the no-attack row is the one exception")
		{
			REQUIRE(write_persisted(attack_table[0].damage) == -1);
		}
	}

	GIVEN("a per-class array")
	{
		THEN("a class subscripts it by its own number")
		{
			REQUIRE(class_index(CLASS_NONE) == 0);
			REQUIRE(class_index(CLASS_SORCERER) == 11);
		}
	}
}

//
// What no runtime assertion in this file can check: the promoted families do
// not convert to each other, or to the ints and bools they used to be. A
// regression here is a compile error in this file rather than a failing
// assertion, which is the whole point of the promotion.
//
static_assert(!std::is_convertible_v<Sex, int>, "a sex is not a number");
static_assert(!std::is_convertible_v<int, Sex>, "a number is not a sex");
static_assert(!std::is_convertible_v<Size, int>, "a size is not a number");
static_assert(!std::is_convertible_v<SectorType, int>, "a terrain is not a number");
static_assert(!std::is_convertible_v<CharClass, int>, "a class is not a number");
static_assert(!std::is_convertible_v<Sex, Size>, "a sex is not a size");
static_assert(!std::is_convertible_v<Size, SectorType>, "a size is not a terrain");
static_assert(!std::is_convertible_v<CharClass, SectorType>, "a class is not a terrain");

// The two arguments that sit next to each other in one_hit_new and damage_new.
// They were both bool, so a call could pass them the wrong way round and say
// nothing.
static_assert(!std::is_convertible_v<HitSpecials, HitBlockable>, "the two hit flags are not each other");
static_assert(!std::is_convertible_v<HitBlockable, HitSpecials>, "the two hit flags are not each other");
static_assert(!std::is_convertible_v<bool, HitBlockable>, "a bool is not a blockable flag");
static_assert(!std::is_convertible_v<bool, HitSpecials>, "a bool is not a specials flag");
static_assert(!std::is_convertible_v<HitBlockable, bool>, "a blockable flag is not a bool");

// The five families that all spell their values TO_*. Four of them are stored
// in a field called "where" on four affect types, and the fifth is what act()
// takes. Their values collide from 0 upwards.
static_assert(!std::is_convertible_v<ActTarget, int>, "an act target is not a number");
static_assert(!std::is_convertible_v<AffectWhere, int>, "an affect where is not a number");
static_assert(!std::is_convertible_v<RoomAffectWhere, int>, "a room affect where is not a number");
static_assert(!std::is_convertible_v<ObjAffectWhere, int>, "an object affect where is not a number");
static_assert(!std::is_convertible_v<AreaAffectWhere, int>, "an area affect where is not a number");

// The four families that all spell their values APPLY_*. Three of them are
// stored in a field called "location" on the character, room and area affects,
// and the fourth is the object's own value slots. Their values collide from 0
// upwards.
static_assert(!std::is_convertible_v<ApplyLocation, int>, "an apply location is not a number");
static_assert(!std::is_convertible_v<ApplyRoomLocation, int>, "a room apply is not a number");
static_assert(!std::is_convertible_v<ApplyAreaLocation, int>, "an area apply is not a number");
static_assert(!std::is_convertible_v<ApplyLocation, ApplyRoomLocation>, "a character apply is not a room apply");
static_assert(!std::is_convertible_v<ApplyRoomLocation, ApplyAreaLocation>, "a room apply is not an area apply");
static_assert(!std::is_convertible_v<ApplyLocation, ApplyAreaLocation>, "a character apply is not an area apply");
static_assert(!std::is_convertible_v<ActTarget, AffectWhere>, "an act target is not an affect where");
static_assert(!std::is_convertible_v<AffectWhere, RoomAffectWhere>, "a character affect is not a room affect");
static_assert(!std::is_convertible_v<AffectWhere, ObjAffectWhere>, "a character affect is not an object affect");
static_assert(!std::is_convertible_v<RoomAffectWhere, AreaAffectWhere>, "a room affect is not an area affect");
static_assert(!std::is_convertible_v<ObjAffectWhere, AffectWhere>, "an object affect is not a character affect");

// The three families a mob's barred entry stores in three adjacent fields, all
// spelled BAR_ and all starting at zero. The criterion says which of the
// mover's numbers is being compared, so a comparison sitting in it reads as a
// perfectly plausible criterion.
static_assert(!std::is_convertible_v<BarCriterion, int>, "a bar criterion is not a number");
static_assert(!std::is_convertible_v<BarComparison, int>, "a bar comparison is not a number");
static_assert(!std::is_convertible_v<BarMessage, int>, "a bar message style is not a number");
static_assert(!std::is_convertible_v<BarCriterion, BarComparison>, "a criterion is not a comparison");
static_assert(!std::is_convertible_v<BarComparison, BarMessage>, "a comparison is not a message style");
static_assert(!std::is_convertible_v<BarMessage, BarCriterion>, "a message style is not a criterion");

// A rune's target says what kind of thing placed_on points at, so the wrong
// value here is a cast to the wrong type rather than a wrong answer. The
// trigger and the RUNE_ masks collide with it from one upwards.
static_assert(!std::is_convertible_v<RuneTarget, int>, "a rune target is not a number");
static_assert(!std::is_convertible_v<RuneTrigger, int>, "a rune trigger is not a number");
static_assert(!std::is_convertible_v<RuneTarget, RuneTrigger>, "a rune target is not a trigger");
static_assert(!std::is_convertible_v<RuneTrigger, RuneTarget>, "a rune trigger is not a target");
static_assert(!std::is_convertible_v<RuneMask, RuneTarget>, "a rune mask is not a rune target");
static_assert(!std::is_convertible_v<RuneMask, RuneTrigger>, "a rune mask is not a rune trigger");

// A worn slot is a slot, not a number. WEAR_NONE is -1, which is what the
// carried-not-worn case has always been written as, so the family has to be
// able to hold it and the loops that count slots have to say they are counting.
static_assert(!std::is_convertible_v<WearLocation, int>, "a wear location is not a number");
static_assert(!std::is_convertible_v<int, WearLocation>, "a number is not a wear location");
static_assert(!std::is_convertible_v<WearLocation, ApplyLocation>, "a wear location is not an apply location");
static_assert(!std::is_convertible_v<WearLocation, BarCriterion>, "a wear location is not a bar criterion");

// A position is ordered, and the ordering is what most of the tests on it use.
// A scoped enumeration keeps the relational operators between its own values
// and takes away the ones that would compare it with a number, which is the
// whole trade.
static_assert(!std::is_convertible_v<Position, int>, "a position is not a number");
static_assert(!std::is_convertible_v<int, Position>, "a number is not a position");
static_assert(!std::is_convertible_v<Position, WearLocation>, "a position is not a wear location");
static_assert(POS_DEAD < POS_SLEEPING, "the order is what the comparisons read");
static_assert(POS_SLEEPING < POS_RESTING, "the order is what the comparisons read");
static_assert(POS_RESTING < POS_SITTING, "the order is what the comparisons read");
static_assert(POS_SITTING < POS_FIGHTING, "the order is what the comparisons read");
static_assert(POS_FIGHTING < POS_STANDING, "the order is what the comparisons read");

// Three families share the ITEM_ prefix and their values collide: ITEM_LIGHT
// is the type 1, ITEM_HUM is extra flag bit 1, ITEM_WEAR_FINGER is wear bit 1.
// Only the type is an ordinal; the other two are bit numbers and are still
// plain enumerations, so this asserts what the split bought rather than
// pretending all three are done.
static_assert(!std::is_convertible_v<ItemType, int>, "an item type is not a number");
static_assert(!std::is_convertible_v<int, ItemType>, "a number is not an item type");
static_assert(!std::is_convertible_v<ItemExtraFlag, ItemType>, "an extra flag is not an item type");
static_assert(!std::is_convertible_v<ItemWearFlag, ItemType>, "a wear flag is not an item type");
static_assert(!std::is_convertible_v<ItemType, WearLocation>, "an item type is not a worn slot");

// A damage class used to be an int sitting next to a bool in the damage
// functions, and the two could be passed the wrong way round in silence: true
// is 1 and so is DAM_BASH. fight.h carried a deleted overload whose only job
// was to catch that. These two assertions are what replaced it.
static_assert(!std::is_convertible_v<bool, DamageType>, "a bool is not a damage class");
static_assert(!std::is_convertible_v<DamageType, bool>, "a damage class is not a bool");
static_assert(!std::is_convertible_v<DamageType, int>, "a damage class is not a number");
static_assert(!std::is_convertible_v<int, DamageType>, "a number is not a damage class");
static_assert(!std::is_convertible_v<DamageType, ItemType>, "a damage class is not an item type");

SCENARIO("the values behind the hit arguments", "[hit_flags]")
{
	GIVEN("the two flag families")
	{
		THEN("their values are what they were before they had types")
		{
			REQUIRE(static_cast<int>(HIT_UNBLOCKABLE) == 0);
			REQUIRE(static_cast<int>(HIT_BLOCKABLE) == 1);
			REQUIRE(static_cast<int>(HIT_NOSPECIALS) == 0);
			REQUIRE(static_cast<int>(HIT_SPECIALS) == 1);
		}
	}

	GIVEN("the two do-nothing values for the numeric arguments")
	{
		// These are not a family and are not typed: an addition of nothing is
		// zero, and the multiplier is a percentage that the damage skips
		// entirely when it is one.
		THEN("they stay numbers")
		{
			REQUIRE(HIT_NOADD == 0);
			REQUIRE(HIT_NOMULT == 1);
		}
	}
}

SCENARIO("reading a stored affect discriminator", "[persisted_enum]")
{
	GIVEN("the values each affect family names")
	{
		THEN("each reads back as itself")
		{
			REQUIRE(read_persisted<AffectWhere>(0, "affect where") == TO_AFFECTS);
			REQUIRE(read_persisted<AffectWhere>(5, "affect where") == TO_WEAPON);
			REQUIRE(read_persisted<RoomAffectWhere>(2, "room affect where") == TO_ROOM_FLAGS);
			REQUIRE(read_persisted<ObjAffectWhere>(1, "object affect where") == TO_OBJ_APPLY);
		}
	}

	GIVEN("an object affect carrying a character affect's discriminator")
	{
		// Which is what envenom stores, and what an object prototype's spell
		// affect is copied into. Five is not a value ObjAffectWhere names, and
		// the load has to hand it back unchanged rather than turning somebody's
		// saved object into a different affect.
		THEN("the value survives being read and written back")
		{
			REQUIRE(write_persisted(read_persisted<ObjAffectWhere>(5, "object affect where")) == 5);
		}
	}
}

SCENARIO("reading a stored apply location", "[persisted_enum]")
{
	GIVEN("locations the enumerations name")
	{
		THEN("each reads back as itself")
		{
			REQUIRE(read_persisted<ApplyLocation>(0, "apply") == APPLY_NONE);
			REQUIRE(read_persisted<ApplyLocation>(18, "apply") == APPLY_HITROLL);
			REQUIRE(read_persisted<ApplyLocation>(100, "apply") == APPLY_OBJ_PROPERTIES);
		}
	}

	GIVEN("the zero of each apply family")
	{
		// They are all zero, which is why the sites that were written as a
		// bare 0 could not say which family they meant, and why renaming them
		// changed no stored value.
		THEN("they agree")
		{
			REQUIRE(write_persisted(APPLY_NONE) == 0);
			REQUIRE(write_persisted(APPLY_ROOM_NONE) == 0);
			REQUIRE(write_persisted(APPLY_AREA_NONE) == 0);
			REQUIRE(obj_location(APPLY_OBJ_NONE) == 0);
		}
	}

	GIVEN("an object affect's location, which is one family or the other")
	{
		// Which one is decided by the affect's where, so the field is a plain
		// number and these say which family is going into it.
		THEN("both families reach it, and keep their own values")
		{
			REQUIRE(obj_location(APPLY_OBJ_V4) == 5);
			REQUIRE(obj_location(APPLY_HITROLL) == 18);
		}
	}
}
