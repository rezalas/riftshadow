#include "catch.hpp"
#include "../code/fight.h"
#include "../code/entity/char_data.h"
#include "../code/entity/handles.h"
#include "../code/enums.h"
#include "../code/handler.h"
#include "../code/db.h"
#include "../code/recycle.h"
#include "../code/interp.h"

SCENARIO("testing updating victim position", "[update_pos]")
{
	GIVEN("a victim who is incapacitated with 1 or more hit points")
	{
		CHAR_DATA* victim = new CHAR_DATA;
		victim->hit = 12;
		victim->position = POS_STUNNED;

		WHEN("update_pos function is called")
		{
			update_pos(victim);

			THEN("their position is updated to POS_STANDING")
			{
				REQUIRE(victim->position == POS_STANDING);
				delete victim;
			}
		}
	}
}

//
// ch->fighting -- what it means for the reference to go away.
//
// stop_fighting is the only thing that clears the field, and it is easy to
// read it as lifetime bookkeeping because extract_char calls it. It is not.
// Of its ~70 callers exactly one -- extract_char -- is about death. The rest
// leave both characters completely alive: fleeing, `stop`, a wiznet freeze, a
// mob submerging, a spell breaking combat. So the rule it encodes is "the
// fight ended", which is a question about combat state, not about whether the
// other character still exists.
//
// These pin that behavior, because it is the specification against which any
// change to the field's type is judged.
//
// Fighting() is the single point that has to change if the field's type
// changes; every assertion below is written through it and stays as-is.
//

namespace
{
	CHAR_DATA *Fighting(CHAR_DATA *ch)
	{
		return Deref(ch->fighting);
	}

	/// stop_fighting walks char_list, so a character has to be on it to be
	/// reached at all. Characters come from new_char so that they are
	/// registered in the slot map and their liveness is a thing a test can
	/// assert on.
	CHAR_DATA *MakeCombatant()
	{
		CHAR_DATA *ch = new_char();
		ch->hit = 100;
		ch->position = POS_FIGHTING;

		char_list.push_front(std::unique_ptr<CHAR_DATA>(ch));
		ch->globalNode = char_list.begin();

		return ch;
	}

	void ClearCombatants()
	{
		// Erasing the node is what destroys the character now, so clearing the
		// list is the whole teardown.
		char_list.clear();
	}
}

SCENARIO("stop_fighting with fBoth ends the fight on both sides", "[stop_fighting]")
{
	GIVEN("two characters fighting each other")
	{
		CHAR_DATA *ch = MakeCombatant();
		CHAR_DATA *victim = MakeCombatant();

		ch->fighting = victim->self;
		victim->fighting = ch->self;

		WHEN("one of them stops fighting with fBoth set")
		{
			stop_fighting(ch, true);

			THEN("neither is fighting any more")
			{
				REQUIRE(Fighting(ch) == nullptr);
				REQUIRE(Fighting(victim) == nullptr);
			}

			THEN("both are still perfectly live characters")
			{
				// This is the whole reason the clear cannot be replaced by a
				// liveness check: nobody died, the fight just ended.
				REQUIRE(Deref(ch->self) == ch);
				REQUIRE(Deref(victim->self) == victim);
			}
		}

		ClearCombatants();
	}
}

SCENARIO("stop_fighting without fBoth clears only the character named", "[stop_fighting]")
{
	GIVEN("two characters fighting each other")
	{
		CHAR_DATA *ch = MakeCombatant();
		CHAR_DATA *victim = MakeCombatant();

		ch->fighting = victim->self;
		victim->fighting = ch->self;

		WHEN("one of them stops fighting with fBoth clear")
		{
			stop_fighting(ch, false);

			THEN("the other keeps swinging at it")
			{
				// The asymmetry is load-bearing: this is how a character
				// disengages while remaining a target.
				REQUIRE(Fighting(ch) == nullptr);
				REQUIRE(Fighting(victim) == ch);
			}
		}

		ClearCombatants();
	}
}

SCENARIO("stop_fighting leaves a fight it is not part of alone", "[stop_fighting]")
{
	GIVEN("two separate fights in progress")
	{
		CHAR_DATA *ch = MakeCombatant();
		CHAR_DATA *victim = MakeCombatant();
		CHAR_DATA *bystander = MakeCombatant();
		CHAR_DATA *itsTarget = MakeCombatant();

		ch->fighting = victim->self;
		victim->fighting = ch->self;
		bystander->fighting = itsTarget->self;
		itsTarget->fighting = bystander->self;

		WHEN("the first fight is stopped")
		{
			stop_fighting(ch, true);

			THEN("the unrelated fight is untouched")
			{
				REQUIRE(Fighting(bystander) == itsTarget);
				REQUIRE(Fighting(itsTarget) == bystander);
			}
		}

		ClearCombatants();
	}
}

SCENARIO("stop_fighting puts everyone it clears back on their feet", "[stop_fighting]")
{
	GIVEN("two characters fighting each other")
	{
		CHAR_DATA *ch = MakeCombatant();
		CHAR_DATA *victim = MakeCombatant();

		ch->fighting = victim->self;
		victim->fighting = ch->self;

		WHEN("the fight is stopped on both sides")
		{
			stop_fighting(ch, true);

			THEN("both are standing rather than still in a fighting stance")
			{
				REQUIRE(ch->position == POS_STANDING);
				REQUIRE(victim->position == POS_STANDING);
			}
		}

		ClearCombatants();
	}
}

SCENARIO("stop_fighting strips entwine from everyone it clears", "[stop_fighting]")
{
	GIVEN("two characters fighting each other, both entwined")
	{
		CHAR_DATA *ch = MakeCombatant();
		CHAR_DATA *victim = MakeCombatant();

		ch->fighting = victim->self;
		victim->fighting = ch->self;

		for (CHAR_DATA *each : {ch, victim})
		{
			AFFECT_DATA af;
			init_affect(&af);
			af.type = gsn_entwine;
			af.where = TO_AFFECTS;
			af.duration = 5;
			affect_to_char(each, &af);
		}

		REQUIRE(is_affected(ch, gsn_entwine) == true);
		REQUIRE(is_affected(victim, gsn_entwine) == true);

		WHEN("the fight is stopped on both sides")
		{
			stop_fighting(ch, true);

			THEN("neither is entwined any more")
			{
				// Being held in place is a property of the fight, so it goes
				// when the fight does -- for the attacker as well as the
				// character stop_fighting was called on.
				REQUIRE(is_affected(ch, gsn_entwine) == false);
				REQUIRE(is_affected(victim, gsn_entwine) == false);
			}
		}

		ClearCombatants();
	}
}

SCENARIO("stop_fighting reaches only characters on char_list", "[stop_fighting]")
{
	GIVEN("an attacker that is not on the global character list")
	{
		CHAR_DATA *ch = MakeCombatant();
		CHAR_DATA *offList = new_char();
		offList->hit = 100;
		offList->fighting = ch->self;

		WHEN("the character it is attacking stops fighting with fBoth set")
		{
			stop_fighting(ch, true);

			THEN("the off-list attacker still refers to it")
			{
				// Documents the mechanism's reach rather than a reachable game
				// state: the sweep is over char_list, so anything not on that
				// list keeps its reference. That is exactly the gap a handle
				// closes, and it is why the field's type matters.
				REQUIRE(Fighting(offList) == ch);
			}
		}

		free_char(offList);
		ClearCombatants();
	}
}

SCENARIO("a reference to a freed opponent reads as nothing", "[stop_fighting]")
{
	GIVEN("an attacker whose target the sweep cannot reach")
	{
		CHAR_DATA *attacker = MakeCombatant();
		CHAR_DATA *target = new_char();

		attacker->fighting = target->self;

		REQUIRE(Fighting(attacker) == target);

		WHEN("the target is freed without any scrub running")
		{
			// The counterpart to the test above: that one shows the sweep's
			// reach ends at char_list, this one shows what now covers the gap.
			// A raw pointer here was a reference into a character struct that
			// the free list can hand straight back out to the next new_char.
			free_char(target);

			THEN("the reference reads as nothing rather than as recycled memory")
			{
				REQUIRE(Fighting(attacker) == nullptr);
			}
		}

		ClearCombatants();
	}
}
static bool CommandExists(const char *name)
{
	for (auto cmd = 0; !cmd_table[cmd].name.empty(); cmd++)
	{
		if (cmd_table[cmd].name == name)
			return true;
	}

	return false;
}

SCENARIO("the command names exempted by movement and silence affects", "[interp][whitelist]")
{
	// interpret matches what the player typed against cmd_table by prefix, then
	// compares cmd_table[cmd].name against these literals. A literal naming no
	// command exempts nothing, and until the comparisons here were string
	// comparisons rather than pointer comparisons there was no way to notice.
	GIVEN("the command names the whitelists exempt")
	{
		WHEN("each is looked up in the command table")
		{
			THEN("the exemptions players rely on name real commands")
			{
				REQUIRE(CommandExists("astrip"));
				REQUIRE(CommandExists("immtalk"));
				REQUIRE(CommandExists("look"));
				REQUIRE(CommandExists("who"));
				REQUIRE(CommandExists("tell"));
				REQUIRE(CommandExists("reply"));
				REQUIRE(CommandExists("say"));
				REQUIRE(CommandExists("cast"));
				REQUIRE(CommandExists("gtell"));
				REQUIRE(CommandExists("score"));
			}

			THEN("the three that name nothing are still inert")
			{
				// Not commands. "gt" is harmless because typing it prefix-matches
				// "gtell", which is exempted in its own right. "examine" is
				// commented out of the table and "trustall" was never in it. If
				// any of these becomes a command, revisit the lists that name it.
				REQUIRE_FALSE(CommandExists("gt"));
				REQUIRE_FALSE(CommandExists("examine"));
				REQUIRE_FALSE(CommandExists("trustall"));
			}
		}
	}
}
