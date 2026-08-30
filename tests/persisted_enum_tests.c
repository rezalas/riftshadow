#include <type_traits>

#include "catch.hpp"
#include "../code/persisted_enum.h"
#include "../code/enums.h"
#include "../code/utility.h"

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
