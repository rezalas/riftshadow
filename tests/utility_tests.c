#include "catch.hpp"
#include "../code/utility.h"
#include "../code/db.h"

// TEST_CASE("Test capitalization", "[string]" )
// {
// 	const char* sample = "this string is lower case.";
// 	char* test = capitalize(sample);
// 	char* shouldBe = "This string is lower case.";
// 	REQUIRE(strcmp(test, shouldBe) == 0);
// }

char_data* TestHelperCreateNPC()
{
	auto npc = new char_data();
	SET_BIT(npc->act, ACT_IS_NPC);
	npc->pIndexData = new mob_index_data();
	return npc;
}

SCENARIO("checking if a player character is a cabal guard", "[is_cabal_guard]")
{
	GIVEN("an uninitialized player character")
	{
		WHEN("is_cabal_guard is called")
		{
			auto isGuard = is_cabal_guard(nullptr);

			THEN("is_cabal_guard should return false")
			{
				REQUIRE(isGuard == false);
			}
		}
	}

	GIVEN("a player character who is a NPC")
	{
		WHEN("is_cabal_guard is called")
		{
			auto npc = TestHelperCreateNPC();
			auto isGuard = is_cabal_guard(npc);

			THEN("is_cabal_guard should return false")
			{
				REQUIRE(isGuard == false);
			}
		}
	}

	GIVEN("a player character who is a NPC but not a guardian")
	{
		WHEN("is_cabal_guard is called")
		{
			auto npc = TestHelperCreateNPC();
			auto isGuard = is_cabal_guard(npc);

			THEN("is_cabal_guard should return false")
			{
				REQUIRE(isGuard == false);
			}
		}
	}

	GIVEN("a player character who is a NPC and a guardian")
	{
		WHEN("is_cabal_guard is called")
		{
			auto npc = TestHelperCreateNPC();
			SET_BIT(npc->act, ACT_INNER_GUARDIAN);
			auto isGuard = is_cabal_guard(npc);

			THEN("is_cabal_guard should return true")
			{
				REQUIRE(isGuard == true);
			}
		}
	}

	GIVEN("a regular player character")
	{
		WHEN("is_cabal_guard is called")
		{
			auto pc = new char_data();
			auto isGuard = is_cabal_guard(pc);

			THEN("is_cabal_guard should return false")
			{
				REQUIRE(isGuard == false);
			}
		}
	}
}

SCENARIO("checking if a player character is a NPC", "[is_npc]")
{
	GIVEN("an uninitialized player character")
	{
		WHEN("is_npc is called")
		{
			auto isNPC = is_npc(nullptr);

			THEN("is_npc should return false")
			{
				REQUIRE(isNPC == false);
			}
		}
	}

	GIVEN("a player character who is a NPC")
	{
		WHEN("is_npc is called")
		{
			auto npc = TestHelperCreateNPC();
			auto isNPC = is_npc(npc);

			THEN("is_npc should return true")
			{
				REQUIRE(isNPC == true);
			}
		}
	}

	GIVEN("a regular player character")
	{
		WHEN("is_npc is called")
		{
			auto pc = new char_data();
			auto isNPC = is_npc(pc);

			THEN("is_npc should return false")
			{
				REQUIRE(isNPC == false);
			}
		}
	}
}

SCENARIO("checking if a player character is an immortal", "[is_immortal]")
{
	GIVEN("an uninitialized player character")
	{
		WHEN("is_immortal is called")
		{
			auto isImmortal = is_immortal(nullptr);

			THEN("is_immortal should return false")
			{
				REQUIRE(isImmortal == false);
			}
		}
	}

	GIVEN("a player character who is not an immortal")
	{
		WHEN("is_immortal is called")
		{
			auto pc = new char_data();
			pc->level = LEVEL_HERO;
			auto isImmortal = is_immortal(pc);

			THEN("is_immortal should return false")
			{
				REQUIRE(isImmortal == false);
			}
		}
	}

	GIVEN("a player character who is not an immortal (by level) but is an immortal (by trust)")
	{
		WHEN("is_immortal is called")
		{
			auto pc = new char_data();
			pc->level = LEVEL_HERO;
			pc->trust = MAX_LEVEL - 8;
			auto isImmortal = is_immortal(pc);

			THEN("is_immortal should return true")
			{
				REQUIRE(isImmortal == true);
			}
		}
	}

	GIVEN("a player character who is an immortal")
	{
		WHEN("is_immortal is called")
		{
			auto pc = new char_data();
			pc->level = LEVEL_IMMORTAL;
			auto isImmortal = is_immortal(pc);

			THEN("is_immortal should return true")
			{
				REQUIRE(isImmortal == true);
			}
		}
	}

	GIVEN("a player character who is a NPC and an immortal")
	{
		WHEN("is_immortal is called")
		{
			auto npc = TestHelperCreateNPC();
			npc->level = LEVEL_IMMORTAL;
			auto isImmortal = is_immortal(npc);

			THEN("is_immortal should return false")
			{
				REQUIRE(isImmortal == false);
			}
		}
	}
}

SCENARIO("checking if a player character is a hero", "[is_hero]")
{
	GIVEN("an uninitialized player character")
	{
		WHEN("is_hero is called")
		{
			auto isHero = is_hero(nullptr);

			THEN("is_hero should return false")
			{
				REQUIRE(isHero == false);
			}
		}
	}

	GIVEN("a player character who is not a hero")
	{
		WHEN("is_hero is called")
		{
			auto pc = new char_data();
			pc->level = 25;
			auto isHero = is_hero(pc);

			THEN("is_hero should return false")
			{
				REQUIRE(isHero == false);
			}
		}
	}

	GIVEN("a player character who is not a hero (by level) but is a hero (by trust)")
	{
		WHEN("is_hero is called")
		{
			auto pc = new char_data();
			pc->level = 25;
			pc->trust = MAX_LEVEL - 9;
			auto isHero = is_hero(pc);

			THEN("is_hero should return true")
			{
				REQUIRE(isHero == true);
			}
		}
	}

	GIVEN("a player character who is a hero")
	{
		WHEN("is_hero is called")
		{
			auto pc = new char_data();
			pc->level = LEVEL_HERO;
			auto isHero = is_hero(pc);

			THEN("is_hero should return true")
			{
				REQUIRE(isHero == true);
			}
		}
	}

	GIVEN("a player character who is a NPC and a hero")
	{
		WHEN("is_hero is called")
		{
			auto npc = TestHelperCreateNPC();
			npc->level = LEVEL_HERO;
			auto isHero = is_hero(npc);

			THEN("is_hero should return false")
			{
				REQUIRE(isHero == false);
			}
		}
	}
}

SCENARIO("checking if a player character is a hero/imm", "[is_heroimm]")
{
	GIVEN("an uninitialized player character")
	{
		WHEN("is_heroimm is called")
		{
			auto isHero = is_heroimm(nullptr);

			THEN("is_heroimm should return false")
			{
				REQUIRE(isHero == false);
			}
		}
	}

	GIVEN("a player character who is not a hero")
	{
		WHEN("is_heroimm is called")
		{
			auto pc = new char_data();
			auto isHero = is_heroimm(pc);

			THEN("is_heroimm should return false")
			{
				REQUIRE(isHero == false);
			}
		}
	}

	GIVEN("a player character who is a hero")
	{
		WHEN("is_heroimm is called")
		{
			auto pc = new char_data();
			SET_BIT(pc->act, PLR_HEROIMM);
			auto isHero = is_heroimm(pc);

			THEN("is_heroimm should return true")
			{
				REQUIRE(isHero == true);
			}
		}
	}
}

SCENARIO("checking if a player character is trusted", "[is_trusted]")
{
	GIVEN("an uninitialized player character")
	{
		WHEN("is_trusted is called")
		{
			auto trust = MAX_LEVEL - 8;
			auto trusted = is_trusted(nullptr, trust);

			THEN("is_trusted should return false")
			{
				REQUIRE(trusted == false);
			}
		}
	}

	GIVEN("a player character who has no trust level defined and their character level does not meet the specified trust level")
	{
		WHEN("is_trusted is called")
		{
			auto pc = new char_data();
			pc->level = 25;

			auto trust = MAX_LEVEL - 8;
			auto trusted = is_trusted(pc, trust);

			THEN("is_trusted should return false")
			{
				REQUIRE(trusted == false);
			}
		}
	}

	GIVEN("a player character who has no trust level defined but their character level meets the specified trust level")
	{
		WHEN("is_hero is called")
		{
			auto pc = new char_data();
			pc->level =  MAX_LEVEL - 8;

			auto trust = MAX_LEVEL - 8;
			auto trusted = is_trusted(pc, trust);

			THEN("is_trusted should return true")
			{
				REQUIRE(trusted == true);
			}
		}
	}

	GIVEN("a player character who does not meet the specified trust level")
	{
		WHEN("is_trusted is called")
		{
			auto pc = new char_data();
			pc->trust = 25;

			auto trust = MAX_LEVEL - 8;
			auto trusted = is_trusted(pc, trust);

			THEN("is_trusted should return false")
			{
				REQUIRE(trusted == false);
			}
		}
	}

	GIVEN("a player character who meets the specified trust level")
	{
		WHEN("is_trusted is called")
		{
			auto pc = new char_data();
			pc->trust = MAX_LEVEL - 8;

			auto trust = MAX_LEVEL - 8;
			auto trusted = is_trusted(pc, trust);

			THEN("is_true should return true")
			{
				REQUIRE(trusted == true);
			}
		}
	}
}

SCENARIO("checking if a player character is affected by an affect", "[is_affected_by]")
{
	GIVEN("an uninitialized player character")
	{
		WHEN("is_affected_by is called")
		{
			auto gsn = gsn_poison;
			auto pc = (CHAR_DATA *)nullptr;
			auto isAffected = is_affected_by(pc, gsn);

			THEN("is_affected_by should return false")
			{
				REQUIRE(isAffected == false);
			}
		}
	}

	GIVEN("a player character who is not affected by the affect")
	{
		WHEN("is_affected_by is called")
		{
			auto gsn = gsn_poison;
			auto pc = new char_data();
			auto isAffected = is_affected_by(pc, gsn);

			THEN("is_affected_by should return false")
			{
				REQUIRE(isAffected == false);
			}
		}
	}

	GIVEN("a player character who meets the specified trust level")
	{
		WHEN("is_trusted is called")
		{
			auto gsn = gsn_poison;
			auto pc = new char_data();
			SET_BIT(pc->affected_by, gsn);

			auto isAffected = is_affected_by(pc, gsn);

			THEN("is_affected_by should return true")
			{
				REQUIRE(isAffected == true);
			}
		}
	}
}

SCENARIO("checking if an object is affected by an affect", "[is_affected_by]")
{
	GIVEN("an uninitialized object")
	{
		WHEN("is_affected_by is called")
		{
			auto gsn = gsn_poison;
			auto obj = (OBJ_DATA *)nullptr;
			auto isAffected = is_affected_by(obj, gsn);

			THEN("is_affected_by should return false")
			{
				REQUIRE(isAffected == false);
			}
		}
	}

	GIVEN("a player character who is not affected by the affect")
	{
		WHEN("is_affected_by is called")
		{
			auto gsn = gsn_poison;
			auto obj = new OBJ_DATA();

			auto isAffected = is_affected_by(obj, gsn);

			THEN("is_affected_by should return false")
			{
				REQUIRE(isAffected == false);
			}
		}
	}

	GIVEN("a player character who meets the specified trust level")
	{
		WHEN("is_affected_by is called")
		{
			auto gsn = gsn_poison;
			auto obj = new OBJ_DATA();
			SET_BIT(obj->affected_by, gsn);

			auto isAffected = is_affected_by(obj, gsn);

			THEN("is_affected_by should return true")
			{
				REQUIRE(isAffected == true);
			}
		}
	}
}

SCENARIO("checking if a player character has ansi set", "[is_ansi]")
{
	GIVEN("an uninitialized object")
	{
		WHEN("is_ansi is called")
		{
			auto isANSI = is_ansi(nullptr);

			THEN("is_ansi should return false")
			{
				REQUIRE(isANSI == false);
			}
		}
	}

	GIVEN("a player character who is a NPC")
	{
		WHEN("is_ansi is called")
		{
			auto npc = TestHelperCreateNPC();

			auto isANSI = is_ansi(npc);

			THEN("is_ansi should return false")
			{
				REQUIRE(isANSI == false);
			}
		}
	}

	GIVEN("a player character who is not an NPC and is not ANSI")
	{
		WHEN("is_ansi is called")
		{
			auto pc = new char_data();
			auto isANSI = is_ansi(pc);

			THEN("is_ansi should return false")
			{
				REQUIRE(isANSI == false);
			}
		}
	}

	GIVEN("a player character who is not an NPC and is ANSI")
	{
		WHEN("is_ansi is called")
		{
			auto pc = new char_data();
			SET_BIT(pc->comm, COMM_ANSI);
			auto isANSI = is_ansi(pc);

			THEN("is_ansi should return true")
			{
				REQUIRE(isANSI == true);
			}
		}
	}
}

SCENARIO("checking if a player character is aligned good", "[is_good]")
{
	GIVEN("an uninitialized object")
	{
		WHEN("is_good is called")
		{
			auto isGood = is_good(nullptr);

			THEN("is_good should return false")
			{
				REQUIRE(isGood == false);
			}
		}
	}

	GIVEN("a player character who is aligned good")
	{
		WHEN("is_good is called")
		{
			auto pc = new char_data();
			pc->alignment = 1000;
			auto isGood = is_good(pc);

			THEN("is_good should return true")
			{
				REQUIRE(isGood == true);
			}
		}
	}

	GIVEN("a player character who is not aligned good")
	{
		WHEN("is_good is called")
		{
			auto pc = new char_data();
			pc->alignment = 100;
			auto isGood = is_good(pc);

			THEN("is_good should return false")
			{
				REQUIRE(isGood == false);
			}
		}
	}
}

SCENARIO("checking if a player character is aligned evil", "[is_evil]")
{
	GIVEN("an uninitialized object")
	{
		WHEN("is_evil is called")
		{
			auto isEvil = is_evil(nullptr);

			THEN("is_evil should return false")
			{
				REQUIRE(isEvil == false);
			}
		}
	}

	GIVEN("a player character who is aligned evil")
	{
		WHEN("is_evil is called")
		{
			auto pc = new char_data();
			pc->alignment = -1000;
			auto isEvil = is_evil(pc);

			THEN("is_evil should return true")
			{
				REQUIRE(isEvil == true);
			}
		}
	}

	GIVEN("a player character who is not aligned evil")
	{
		WHEN("is_evil is called")
		{
			auto pc = new char_data();
			pc->alignment = 1000;
			auto isEvil = is_evil(pc);

			THEN("is_evil should return false")
			{
				REQUIRE(isEvil == false);
			}
		}
	}
}

SCENARIO("checking if a player character is aligned neutral", "[is_neutral]")
{
	GIVEN("an uninitialized object")
	{
		WHEN("is_neutral is called")
		{
			auto isNeutral = is_neutral(nullptr);

			THEN("is_neutral should return false")
			{
				REQUIRE(isNeutral == false);
			}
		}
	}

	GIVEN("a player character who is aligned neutral")
	{
		WHEN("is_neutral is called")
		{
			auto pc = new char_data();
			pc->alignment = 0;
			auto isNeutral = is_neutral(pc);

			THEN("is_neutral should return true")
			{
				REQUIRE(isNeutral == true);
			}
		}
	}

	GIVEN("a player character who is not aligned neutral")
	{
		WHEN("is_neutral is called")
		{
			auto pc = new char_data();
			pc->alignment = 1000;
			auto isNeutral = is_neutral(pc);

			THEN("is_neutral should return false")
			{
				REQUIRE(isNeutral == false);
			}
		}
	}
}

SCENARIO("checking if a player character is ethicallly lawful", "[is_lawful]")
{
	GIVEN("an uninitialized object")
	{
		WHEN("is_lawful is called")
		{
			auto isLawful = is_lawful(nullptr);

			THEN("is_lawful should return false")
			{
				REQUIRE(isLawful == false);
			}
		}
	}

	GIVEN("a player character who is ethically lawful")
	{
		WHEN("is_lawful is called")
		{
			auto pc = new char_data();
			pc->pcdata = new PC_DATA();
			pc->pcdata->ethos = 1000;
			auto isLawful = is_lawful(pc);

			THEN("is_lawful should return true")
			{
				REQUIRE(isLawful == true);
			}
		}
	}

	GIVEN("a player character who is not ethically lawful")
	{
		WHEN("is_lawful is called")
		{
			auto pc = new char_data();
			pc->pcdata = new PC_DATA();
			pc->pcdata->ethos = 0;
			auto isLawful = is_lawful(pc);

			THEN("is_lawful should return false")
			{
				REQUIRE(isLawful == false);
			}
		}
	}
}

SCENARIO("checking if a player character is ethicallly chaotic", "[is_chaotic]")
{
	GIVEN("an uninitialized object")
	{
		WHEN("is_chaotic is called")
		{
			auto isChaotic = is_chaotic(nullptr);

			THEN("is_chaotic should return false")
			{
				REQUIRE(isChaotic == false);
			}
		}
	}

	GIVEN("a player character who is ethically chaotic")
	{
		WHEN("is_chaotic is called")
		{
			auto pc = new char_data();
			pc->pcdata = new PC_DATA();
			pc->pcdata->ethos = -1000;
			auto isChaotic = is_chaotic(pc);

			THEN("is_chaotic should return true")
			{
				REQUIRE(isChaotic == true);
			}
		}
	}

	GIVEN("a player character who is not ethically chaotic")
	{
		WHEN("is_chaotic is called")
		{
			auto pc = new char_data();
			pc->pcdata = new PC_DATA();
			pc->pcdata->ethos = 0;
			auto isChaotic = is_chaotic(pc);

			THEN("is_chaotic should return false")
			{
				REQUIRE(isChaotic == false);
			}
		}
	}
}

SCENARIO("checking if a player character is ethicallly neutral", "[is_eneutral]")
{
	GIVEN("an uninitialized object")
	{
		WHEN("is_eneutral is called")
		{
			auto isNeutral = is_eneutral(nullptr);

			THEN("is_eneutral should return false")
			{
				REQUIRE(isNeutral == false);
			}
		}
	}

	GIVEN("a player character who is ethically neutral")
	{
		WHEN("is_eneutral is called")
		{
			auto pc = new char_data();
			pc->pcdata = new PC_DATA();
			pc->pcdata->ethos = 0;
			auto isNeutral = is_eneutral(pc);

			THEN("is_eneutral should return true")
			{
				REQUIRE(isNeutral == true);
			}
		}
	}

	GIVEN("a player character who is not ethically neutral")
	{
		WHEN("is_neutral is called")
		{
			auto pc = new char_data();
			pc->pcdata = new PC_DATA();
			pc->pcdata->ethos = 1000;
			auto isNeutral = is_eneutral(pc);

			THEN("is_eneutral should return false")
			{
				REQUIRE(isNeutral == false);
			}
		}
	}
}

SCENARIO("checking if two player characters have the same alignment", "[is_same_align]")
{
	GIVEN("two player characters who have the same alignment")
	{
		WHEN("is_same_align is called")
		{
			auto one = 100;
			auto two = -100;
			auto isSameAlign = is_same_align(one, two);

			THEN("is_same_align should return true")
			{
				REQUIRE(isSameAlign == true);
			}
		}
	}

	GIVEN("two player characters who have different alignments")
	{
		WHEN("is_same_align is called")
		{
			auto one = 1000;
			auto two = -1000;
			auto isSameAlign = is_same_align(one, two);

			THEN("is_same_align should return false")
			{
				REQUIRE(isSameAlign == false);
			}
		}
	}
}

SCENARIO("checking if two player characters have the opposite alignment", "[is_opp_align]")
{
	GIVEN("the first player character is uninitialized")
	{
		WHEN("is_eneutral is called")
		{
			auto one = (CHAR_DATA *)nullptr;
			auto two = new char_data();
			auto isOppAlign = is_opp_align(one, two);

			THEN("is_opp_align should return false")
			{
				REQUIRE(isOppAlign == false);
			}
		}
	}

	GIVEN("the second player character is uninitialized")
	{
		WHEN("is_eneutral is called")
		{
			auto one = new char_data();
			auto two = (CHAR_DATA *)nullptr;
			auto isOppAlign = is_opp_align(one, two);

			THEN("is_opp_align should return false")
			{
				REQUIRE(isOppAlign == false);
			}
		}
	}

	GIVEN("two player characters who have the same alignment")
	{
		WHEN("is_opp_align is called")
		{
			auto one = new char_data();
			auto two = new char_data();

			one->alignment = 100;
			two->alignment = -100;
			auto isOppAlign = is_opp_align(one, two);

			THEN("is_opp_align should return false")
			{
				REQUIRE(isOppAlign == false);
			}
		}
	}

	GIVEN("two player characters who have different alignments")
	{
		WHEN("is_opp_align is called")
		{
			auto one = new char_data();
			auto two = new char_data();

			one->alignment = 1000;
			two->alignment = -1000;
			auto isOppAlign = is_opp_align(one, two);

			THEN("is_opp_align should return true")
			{
				REQUIRE(isOppAlign == true);
			}
		}
	}
}


SCENARIO("checking if a specific number is for a good alignment", "[is_num_good]")
{
	GIVEN("a number for a good alignment")
	{
		WHEN("is_num_good is called")
		{
			auto align = 1000;
			auto isNumGood = is_num_good(align);

			THEN("is_num_good should return true")
			{
				REQUIRE(isNumGood == true);
			}
		}
	}

	GIVEN("a number for a non-good alignment")
	{
		WHEN("is_num_good is called")
		{
			auto align = 0;
			auto isNumGood = is_num_good(align);

			THEN("is_num_good should return false")
			{
				REQUIRE(isNumGood == false);
			}
		}
	}
}

SCENARIO("checking if a specific number is for a evil alignment", "[is_num_evil]")
{
	GIVEN("a number for an evil alignment")
	{
		WHEN("is_num_evil is called")
		{
			auto align = -1000;
			auto isNumEvil = is_num_evil(align);

			THEN("is_num_evil should return true")
			{
				REQUIRE(isNumEvil == true);
			}
		}
	}

	GIVEN("a number for a non-evil alignment")
	{
		WHEN("is_num_evil is called")
		{
			auto align = 1000;
			auto isNumEvil = is_num_evil(align);

			THEN("is_num_evil should return false")
			{
				REQUIRE(isNumEvil == false);
			}
		}
	}
}

SCENARIO("checking if a number is for a neutral alignment", "[is_num_neutral]")
{
	GIVEN("a number for a neutral alignment")
	{
		WHEN("is_num_neutral is called")
		{
			auto align = 0;
			auto isNumNeutral = is_num_neutral(align);

			THEN("is_num_neutral should return true")
			{
				REQUIRE(isNumNeutral == true);
			}
		}
	}

	GIVEN("a number for a non-neutral alignment")
	{
		WHEN("is_num_neutral is called")
		{
			auto align = 1000;
			auto isNumNeutral = is_num_neutral(align);

			THEN("is_num_neutral should return false")
			{
				REQUIRE(isNumNeutral == false);
			}
		}
	}
}

SCENARIO("checking if a number is for a lawful ethos", "[is_num_lawful]")
{
	GIVEN("a number for a lawful ethos")
	{
		WHEN("is_num_lawful is called")
		{
			auto ethos = 1000;
			auto isNumLawful = is_num_lawful(ethos);

			THEN("is_num_lawful should return true")
			{
				REQUIRE(isNumLawful == true);
			}
		}
	}

	GIVEN("a number for a non-lawful ethos")
	{
		WHEN("is_num_lawful is called")
		{
			auto ethos = 0;
			auto isNumLawful = is_num_lawful(ethos);

			THEN("is_num_lawful should return false")
			{
				REQUIRE(isNumLawful == false);
			}
		}
	}
}

SCENARIO("checking if a number is for a chaotic ethos", "[is_num_chaotic]")
{
	GIVEN("a number for a chaotic ethos")
	{
		WHEN("is_num_chaotic is called")
		{
			auto ethos = -1000;
			auto isNumChaotic = is_num_chaotic(ethos);

			THEN("is_num_chaotic should return true")
			{
				REQUIRE(isNumChaotic == true);
			}
		}
	}

	GIVEN("a number for a non-chaotic ethos")
	{
		WHEN("is_num_chaotic is called")
		{
			auto ethos = 0;
			auto isNumChaotic = is_num_chaotic(ethos);

			THEN("is_num_chaotic should return false")
			{
				REQUIRE(isNumChaotic == false);
			}
		}
	}
}

SCENARIO("checking if a number is for a neutral ethos", "[is_num_eneutral]")
{
	GIVEN("a number for a neutral ethos")
	{
		WHEN("is_num_eneutral is called")
		{
			auto ethos = 0;
			auto isNumNeutral = is_num_eneutral(ethos);

			THEN("is_num_eneutral should return true")
			{
				REQUIRE(isNumNeutral == true);
			}
		}
	}

	GIVEN("a number for a non-neutral ethos")
	{
		WHEN("is_num_eneutral is called")
		{
			auto ethos = 1000;
			auto isNumNeutral = is_num_eneutral(ethos);

			THEN("is_num_eneutral should return false")
			{
				REQUIRE(isNumNeutral == false);
			}
		}
	}
}

SCENARIO("checking if a player character is awake", "[is_awake]")
{
	GIVEN("an uninitialized object")
	{
		WHEN("is_awake is called")
		{
			auto isAwake = is_awake(nullptr);

			THEN("is_awake should return false")
			{
				REQUIRE(isAwake == false);
			}
		}
	}

	GIVEN("a player character who is not awake")
	{
		WHEN("is_eneutral is called")
		{
			auto pc = new char_data();
			pc->position = POS_SLEEPING;

			auto isAwake = is_awake(pc);

			THEN("is_awake should return false")
			{
				REQUIRE(isAwake == false);
			}
		}
	}

	GIVEN("a player character who is awake")
	{
		WHEN("is_neutral is called")
		{
			auto pc = new char_data();
			pc->position = POS_STANDING;

			auto isAwake = is_awake(pc);

			THEN("is_awake should return true")
			{
				REQUIRE(isAwake == true);
			}
		}
	}
}

SCENARIO("retrieve a player character's armor type", "[get_ac]")
{
	GIVEN("an uninitialized object")
	{
		WHEN("get_ac is called")
		{
			auto pc = (CHAR_DATA *)nullptr;
			auto type = AC_EXOTIC;
			auto getAC = get_ac(pc, type);

			THEN("get_ac should return 0")
			{
				REQUIRE(getAC == 0);
			}
		}
	}

	GIVEN("an invalid armor type")
	{
		WHEN("get_ac is called")
		{
			auto pc = new char_data();
			auto type = 10;
			auto getAC = get_ac(pc, type);

			THEN("get_ac should return 0")
			{
				REQUIRE(getAC == 0);
			}
		}
	}

	//TODO: Find out how pc->armor[type] is filled so we can 
	//      write a valid test for get_ac with a valid armor type
}


// TODO: write tests
// short get_hitroll (CHAR_DATA *ch)

// TODO: write tests
// short get_damroll (CHAR_DATA *ch)

// TODO: write tests
// bool is_outside (CHAR_DATA *ch)

// TODO: write tests
// long get_carry_weight(CHAR_DATA *ch)

// TODO: write tests
// bool is_shifted (CHAR_DATA *ch)

// TODO: write tests
// bool is_hungry (CHAR_DATA *ch)

// TODO: write tests
// bool is_thirsty (CHAR_DATA *ch)

// TODO: write tests
// bool is_room_affected (ROOM_INDEX_DATA *room, int sn)

// TODO: write tests
// bool is_ground (ROOM_INDEX_DATA *room)

// TODO: write tests
// bool is_water (ROOM_INDEX_DATA *room)

// TODO: write tests
// bool can_wear (OBJ_DATA *obj, int part)

// TODO: write tests
// bool is_obj_stat (OBJ_DATA *obj, int stat)

// TODO: write tests
// bool is_weapon_stat (OBJ_DATA *obj, int stat)

// TODO: write tests
// int weight_mult (OBJ_DATA *obj)

// TODO: write tests
// bool is_explore (ROOM_INDEX_DATA *room)

// TODO: write tests
// bool is_metal (OBJ_DATA *obj)

// TODO: write tests
// bool is_stone (OBJ_DATA *obj)

// TODO: write tests
// bool can_rust (OBJ_DATA *obj)

// TODO: write tests
// char* pers (CHAR_DATA *ch, CHAR_DATA *looker)

// TODO: write tests
// bool is_switched (CHAR_DATA *ch)
