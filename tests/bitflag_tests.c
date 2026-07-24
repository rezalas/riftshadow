#include <math.h>
#include "catch.hpp"
#include "../code/macros.h"
#include "../code/entity/limits.h"
#include "../code/stdlibs/bitvector.h"

// Equivalence proofs for the bitflag operator rewrite.
//
// The pre-rewrite operators are reproduced below *verbatim*
// renamed with a LEGACY_ prefix. They are copied as text on
// purpose: asserting the new shift-based macros against a 
// hand-written pow() expression would only prove that expression
// equal to itself. Comparing against the actual retired macro body 
// is what makes this a proof rather than a restatement.

// array flags
#define LEGACY_IS_SET(flag, bit)		((flag[(int)(bit/32)]) & ((long)pow(2,(bit % 32))))
#define LEGACY_SET_BIT(var, bit)		((var[(int)bit/32]) |= ((long)pow(2,bit % 32)))
#define LEGACY_REMOVE_BIT(var, bit)		((var[bit/32]) &= ~((long)pow(2,bit % 32)))

// scalar flags
#define LEGACY_IS_SET_OLD(flag, bit)	(flag & ((long)pow(2,bit)))
#define LEGACY_SET_BIT_OLD(var, bit)	(var |= (long)pow(2,bit))
#define LEGACY_REMOVE_BIT_OLD(var,bit)	(var &= ~((long)pow(2,bit)))

// Word patterns exercised against every bit position. Exhaustive over *flag
// values* is 2^128 for the array family, so the domain is (every bit) x (a set
// of patterns chosen to cover both polarities, alternating neighbours, and the
// isolated-bit case). Every bit position is covered totally; that is the axis
// the rewrite could plausibly get wrong.
static const long WORD_PATTERNS[] = {
	0L,
	~0L,
	0x55555555L,
	0xAAAAAAAAL,
	0x00000001L,
	0x80000000L,
	0x0000FFFFL,
	0xFFFF0000L,
};

static const int PATTERN_COUNT = sizeof(WORD_PATTERNS) / sizeof(WORD_PATTERNS[0]);

// The array operators divide the bit by 32 and index, so bits 0..63 span both
// words of a long[MAX_BITVECTOR]. The shift is always (bit % 32), i.e. 0..31,
// where both (long)pow(2,n) and 1L << n are well defined.
static const int ARRAY_BIT_MAX = 64;

// The scalar operators shift by the absolute bit, so 63 would be a 1L << 63 on
// a signed long, undefined in both the old and new spelling, and not a domain
// the codebase uses (these overwhelmingly operate on obj->value[N], an int).
// The proof therefore runs 0..62, which is total over every defined input.
static const int SCALAR_BIT_MAX = 63;

TEST_CASE("IS_SET matches the retired pow() macro for every bit", "[bitflag]")
{
	for (int bit = 0; bit < ARRAY_BIT_MAX; bit++)
	{
		for (int i = 0; i < PATTERN_COUNT; i++)
		{
			for (int j = 0; j < PATTERN_COUNT; j++)
			{
				long flag[MAX_BITVECTOR] = { WORD_PATTERNS[i], WORD_PATTERNS[j] };

				INFO("bit=" << bit << " word0=" << flag[0] << " word1=" << flag[1]);
				REQUIRE(!!IS_SET(flag, bit) == !!LEGACY_IS_SET(flag, bit));
				REQUIRE(IS_SET(flag, bit) == LEGACY_IS_SET(flag, bit));
			}
		}
	}
}

TEST_CASE("SET_BIT matches the retired pow() macro for every bit", "[bitflag]")
{
	for (int bit = 0; bit < ARRAY_BIT_MAX; bit++)
	{
		for (int i = 0; i < PATTERN_COUNT; i++)
		{
			for (int j = 0; j < PATTERN_COUNT; j++)
			{
				long fresh[MAX_BITVECTOR] = { WORD_PATTERNS[i], WORD_PATTERNS[j] };
				long legacy[MAX_BITVECTOR] = { WORD_PATTERNS[i], WORD_PATTERNS[j] };

				SET_BIT(fresh, bit);
				LEGACY_SET_BIT(legacy, bit);

				INFO("bit=" << bit << " word0=" << WORD_PATTERNS[i] << " word1=" << WORD_PATTERNS[j]);
				REQUIRE(fresh[0] == legacy[0]);
				REQUIRE(fresh[1] == legacy[1]);
			}
		}
	}
}

TEST_CASE("REMOVE_BIT matches the retired pow() macro for every bit", "[bitflag]")
{
	for (int bit = 0; bit < ARRAY_BIT_MAX; bit++)
	{
		for (int i = 0; i < PATTERN_COUNT; i++)
		{
			for (int j = 0; j < PATTERN_COUNT; j++)
			{
				long fresh[MAX_BITVECTOR] = { WORD_PATTERNS[i], WORD_PATTERNS[j] };
				long legacy[MAX_BITVECTOR] = { WORD_PATTERNS[i], WORD_PATTERNS[j] };

				REMOVE_BIT(fresh, bit);
				LEGACY_REMOVE_BIT(legacy, bit);

				INFO("bit=" << bit << " word0=" << WORD_PATTERNS[i] << " word1=" << WORD_PATTERNS[j]);
				REQUIRE(fresh[0] == legacy[0]);
				REQUIRE(fresh[1] == legacy[1]);
			}
		}
	}
}

TEST_CASE("IS_SET_OLD matches the retired pow() macro for every bit", "[bitflag]")
{
	for (int bit = 0; bit < SCALAR_BIT_MAX; bit++)
	{
		for (int i = 0; i < PATTERN_COUNT; i++)
		{
			long flag = WORD_PATTERNS[i];

			INFO("bit=" << bit << " flag=" << flag);
			REQUIRE(!!IS_SET_OLD(flag, bit) == !!LEGACY_IS_SET_OLD(flag, bit));
			REQUIRE(IS_SET_OLD(flag, bit) == LEGACY_IS_SET_OLD(flag, bit));
		}
	}
}

TEST_CASE("SET_BIT_OLD matches the retired pow() macro for every bit", "[bitflag]")
{
	for (int bit = 0; bit < SCALAR_BIT_MAX; bit++)
	{
		for (int i = 0; i < PATTERN_COUNT; i++)
		{
			long fresh = WORD_PATTERNS[i];
			long legacy = WORD_PATTERNS[i];

			SET_BIT_OLD(fresh, bit);
			LEGACY_SET_BIT_OLD(legacy, bit);

			INFO("bit=" << bit << " flag=" << WORD_PATTERNS[i]);
			REQUIRE(fresh == legacy);
		}
	}
}

TEST_CASE("REMOVE_BIT_OLD matches the retired pow() macro for every bit", "[bitflag]")
{
	for (int bit = 0; bit < SCALAR_BIT_MAX; bit++)
	{
		for (int i = 0; i < PATTERN_COUNT; i++)
		{
			long fresh = WORD_PATTERNS[i];
			long legacy = WORD_PATTERNS[i];

			REMOVE_BIT_OLD(fresh, bit);
			LEGACY_REMOVE_BIT_OLD(legacy, bit);

			INFO("bit=" << bit << " flag=" << WORD_PATTERNS[i]);
			REQUIRE(fresh == legacy);
		}
	}
}

// TOGGLE_BIT / TOGGLE_BIT_OLD (macros.h) are defined in terms of the six
// operators above, so they inherit the rewrite rather than being rewritten. They
// are covered here because phase-02 §2 did not enumerate them and they are the
// operators most exposed to the double-evaluation hazard: TOGGLE_BIT expands its
// `bit` argument six times.
TEST_CASE("TOGGLE_BIT flips exactly the requested bit", "[bitflag]")
{
	for (int bit = 0; bit < ARRAY_BIT_MAX; bit++)
	{
		for (int i = 0; i < PATTERN_COUNT; i++)
		{
			for (int j = 0; j < PATTERN_COUNT; j++)
			{
				long flag[MAX_BITVECTOR] = { WORD_PATTERNS[i], WORD_PATTERNS[j] };
				bool wasSet = !!IS_SET(flag, bit);

				TOGGLE_BIT(flag, bit);

				INFO("bit=" << bit << " word0=" << WORD_PATTERNS[i] << " word1=" << WORD_PATTERNS[j]);
				REQUIRE(!!IS_SET(flag, bit) == !wasSet);

				// every other bit is untouched
				long expected[MAX_BITVECTOR] = { WORD_PATTERNS[i], WORD_PATTERNS[j] };
				expected[bit / 32] ^= (1L << (bit % 32));
				REQUIRE(flag[0] == expected[0]);
				REQUIRE(flag[1] == expected[1]);
			}
		}
	}
}

TEST_CASE("TOGGLE_BIT_OLD flips exactly the requested bit", "[bitflag]")
{
	for (int bit = 0; bit < SCALAR_BIT_MAX; bit++)
	{
		for (int i = 0; i < PATTERN_COUNT; i++)
		{
			long flag = WORD_PATTERNS[i];
			bool wasSet = !!IS_SET_OLD(flag, bit);

			TOGGLE_BIT_OLD(flag, bit);

			INFO("bit=" << bit << " flag=" << WORD_PATTERNS[i]);
			REQUIRE(!!IS_SET_OLD(flag, bit) == !wasSet);
			REQUIRE(flag == (WORD_PATTERNS[i] ^ (1L << bit)));
		}
	}
}

// The rewrite adds the parentheses the originals lacked entirely. Without them
// an argument that is an expression rather than an identifier binds wrong:
// IS_SET(f, a + b) expanded to f[(int)(a + b/32)] — b/32, not (a+b)/32.
TEST_CASE("bitflag operators accept expression arguments", "[bitflag]")
{
	long flag[MAX_BITVECTOR] = { 0L, 0L };
	int base = 30;
	int offset = 3;

	SET_BIT(flag, base + offset);
	REQUIRE(flag[1] == 0x2L);				// bit 33 -> word 1, offset 1
	REQUIRE(flag[0] == 0L);
	REQUIRE(!!IS_SET(flag, base + offset));

	REMOVE_BIT(flag, base + offset);
	REQUIRE(flag[0] == 0L);
	REQUIRE(flag[1] == 0L);

	long scalar = 0L;
	SET_BIT_OLD(scalar, base + offset);
	REQUIRE(scalar == (1L << 33));
	REQUIRE(!!IS_SET_OLD(scalar, base + offset));

	REMOVE_BIT_OLD(scalar, base + offset);
	REQUIRE(scalar == 0L);
}

// ARR(x) in stdlibs/bitvector.h had the same defect and the same fix.
//
// ARR expands a bare conditional, so it must be assigned out before Catch sees
// it. REQUIRE(ARR(x) == n) hands the `<` to Catch's expression decomposition
// and does not compile.
TEST_CASE("ARR maps a bit to its word index", "[bitflag]")
{
	int word0 = ARR(0);
	int word31 = ARR(31);
	int word32 = ARR(32);
	int word63 = ARR(63);

	REQUIRE(word0 == 0);
	REQUIRE(word31 == 0);
	REQUIRE(word32 == 1);
	REQUIRE(word63 == 1);
}

// The actual defect. ARR's body was an unparenthesized `x < 32 ? 0 : 1`, so any
// use inside a larger expression re-associated: `1 + ARR(32)` expanded to
// `1 + 32 < 32 ? 0 : 1`, i.e. `(33 < 32) ? 0 : 1` == 1, not 2.
//
// Note the argument side is *not* the hazard it looks like: `+`, `<<` and `>`
// all bind at least as tightly as `<`, so ARR(a + b) happened to work. It is the
// missing outer parentheses that bite, which is why this is the case that fails
// against the pre-fix header.
TEST_CASE("ARR binds correctly inside a larger expression", "[bitflag]")
{
	int embedded = 1 + ARR(32);
	REQUIRE(embedded == 2);

	int doubled = 2 * ARR(32);
	REQUIRE(doubled == 2);
}
