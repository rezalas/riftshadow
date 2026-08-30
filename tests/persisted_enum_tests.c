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
