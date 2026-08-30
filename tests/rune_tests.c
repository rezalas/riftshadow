#include "catch.hpp"
#include "../code/merc.h"
#include "../code/handler.h"
#include "../code/characterClasses/chrono.h"
#include "../code/entity/char_data.h"
#include "../code/entity/obj_data.h"
#include "../code/entity/exit_data.h"
#include "../code/entity/room_index_data.h"
#include "../code/entity/handles.h"
#include "../code/enums.h"
#include "../code/db.h"
#include "../code/recycle.h"
#include "../code/mem.h"

//
// RUNE_DATA, and the two lists every rune is on at once.
//
// apply_rune puts a rune on the global `rune_list`, which owns it, AND on a
// non-owning per-container chain hanging off obj->rune / exit->rune / room->rune
// (via `next_content`). update.c ticks runes off the global list and calls
// extract_rune when one expires; find_rune walks the per-container chain to
// decide whether entering a room or opening a door should trigger anything.
//
// So the global list decides *when* a rune fires and holds it alive, and the
// container chain decides *whether*. extract_rune has to unlink from both, and
// the container chain strictly first, since leaving rune_list destroys the rune.
// These scenarios are about the container half, because that is the half it
// gets wrong.
//

namespace
{
	// The one reader and the one writer of rune->owner, so the rest of this
	// file compiles unchanged either side of the field's type flip.
	CHAR_DATA *RuneOwner(const RUNE_DATA *rune)
	{
		return Deref(rune->owner);
	}

	void SetRuneOwner(RUNE_DATA &rune, CHAR_DATA *owner)
	{
		rune.owner = owner->self;
	}

	// apply_rune copies its argument into a rune rune_list owns, so the caller's
	// struct is a template rather than the thing that ends up on the lists.
	RUNE_DATA *PlaceRuneOnObj(OBJ_DATA *obj, int type, RuneTrigger trigger, CHAR_DATA *owner = nullptr)
	{
		RUNE_DATA temp;

		temp.function = nullptr;
		temp.placed_on = obj;
		temp.owner = nullptr;

		if (owner != nullptr)
			SetRuneOwner(temp, owner);

		temp.target_type = RUNE_TO_WEAPON;
		temp.trigger_type = trigger;
		temp.level = 30;
		temp.duration = 5;
		temp.type = type;
		temp.extra = 0;
		temp.drawn_in = 0;
		temp.next_content = nullptr;
		temp.end_fun = nullptr;

		apply_rune(&temp);

		// apply_rune pushes onto both heads, so the rune it just made is the
		// front of the global list.
		return rune_list.front().get();
	}

	// The container chain as a list of rune `type`s, read the way find_rune
	// reads it. A rune that has been unlinked correctly does not appear here.
	std::vector<int> ChainTypes(OBJ_DATA *obj)
	{
		std::vector<int> types;

		for (RUNE_DATA *rune = obj->rune; rune != nullptr; rune = rune->next_content)
			types.push_back(rune->type);

		return types;
	}

	int GlobalListLength()
	{
		return (int)rune_list.size();
	}

	void ClearRunes(OBJ_DATA *obj)
	{
		while (!rune_list.empty())
			extract_rune(rune_list.front().get());

		obj->rune = nullptr;
	}
}

SCENARIO("a rune is reachable from both of the lists it is placed on", "[rune]")
{
	GIVEN("an object with one rune on it")
	{
		OBJ_DATA *obj = new_obj();

		PlaceRuneOnObj(obj, 11, RUNE_TRIGGER_EXIT);

		THEN("it is on the global tick list and on the object's own chain")
		{
			REQUIRE(GlobalListLength() == 1);
			REQUIRE(ChainTypes(obj) == std::vector<int>{11});
		}

		THEN("find_rune matches it by trigger")
		{
			REQUIRE(find_rune(obj, RUNE_TO_WEAPON, RUNE_TRIGGER_EXIT, nullptr) != nullptr);
			REQUIRE(find_rune(obj, RUNE_TO_WEAPON, RUNE_TRIGGER_ENTRY, nullptr) == nullptr);
		}

		ClearRunes(obj);
		free_obj(obj);
	}
}

SCENARIO("extracting the newest rune leaves the older ones in place", "[rune]")
{
	GIVEN("an object with three runes, newest first")
	{
		OBJ_DATA *obj = new_obj();

		PlaceRuneOnObj(obj, 11, RUNE_TRIGGER_EXIT);
		PlaceRuneOnObj(obj, 22, RUNE_TRIGGER_EXIT);
		RUNE_DATA *newest = PlaceRuneOnObj(obj, 33, RUNE_TRIGGER_EXIT);

		REQUIRE(ChainTypes(obj) == std::vector<int>({33, 22, 11}));

		WHEN("the newest expires")
		{
			// The head case, and the only one extract_rune has ever handled.
			extract_rune(newest);

			THEN("the other two are still on the chain, in order")
			{
				REQUIRE(ChainTypes(obj) == std::vector<int>({22, 11}));
				REQUIRE(GlobalListLength() == 2);
			}
		}

		ClearRunes(obj);
		free_obj(obj);
	}
}

SCENARIO("extracting the oldest rune does not hide the newer ones", "[rune]")
{
	GIVEN("an object with two runes on it")
	{
		OBJ_DATA *obj = new_obj();

		RUNE_DATA *oldest = PlaceRuneOnObj(obj, 11, RUNE_TRIGGER_EXIT);

		PlaceRuneOnObj(obj, 22, RUNE_TRIGGER_EXIT);

		WHEN("the oldest expires")
		{
			// It is the tail, so its next_content is null -- the case the old
			// unlink treated as "this container has no runes left", whatever
			// else was still on the chain.
			extract_rune(oldest);

			THEN("the newer rune is still there and still visible")
			{
				REQUIRE(ChainTypes(obj) == std::vector<int>{22});
				REQUIRE(find_rune(obj, RUNE_TO_WEAPON, RUNE_TRIGGER_EXIT, nullptr) != nullptr);
			}
		}

		ClearRunes(obj);
		free_obj(obj);
	}
}

SCENARIO("extracting a rune from the middle of the chain keeps the rest", "[rune]")
{
	GIVEN("an object with three runes on it")
	{
		OBJ_DATA *obj = new_obj();

		PlaceRuneOnObj(obj, 11, RUNE_TRIGGER_EXIT);

		RUNE_DATA *middle = PlaceRuneOnObj(obj, 22, RUNE_TRIGGER_EXIT);

		PlaceRuneOnObj(obj, 33, RUNE_TRIGGER_EXIT);

		WHEN("the middle one expires")
		{
			extract_rune(middle);

			THEN("the newest is still the head and the oldest still follows it")
			{
				REQUIRE(ChainTypes(obj) == std::vector<int>({33, 11}));
				REQUIRE(GlobalListLength() == 2);
			}
		}

		ClearRunes(obj);
		free_obj(obj);
	}
}

SCENARIO("extracting the last rune leaves the container naming nothing", "[rune]")
{
	GIVEN("an object with a single rune on it")
	{
		OBJ_DATA *obj = new_obj();

		RUNE_DATA *only = PlaceRuneOnObj(obj, 11, RUNE_TRIGGER_EXIT);

		WHEN("it expires")
		{
			extract_rune(only);

			THEN("the object names no rune at all")
			{
				// Leaving rune_list destroys the rune, so a container left pointing
				// at an extracted one is pointing at freed memory.
				REQUIRE(obj->rune == nullptr);
				REQUIRE(GlobalListLength() == 0);
			}
		}

		ClearRunes(obj);
		free_obj(obj);
	}
}

//
// rune->owner -- the caster. §0.6 found it late: it is a fifth `owner`, on a
// type nobody counted as part of the entity graph, and it is the only one that
// was scrubbed by literally nothing. Runes are placed on doors and rooms
// precisely so they outlive the moment they were cast in, so a caster leaving
// while their rune stands is the ordinary case rather than an edge one.
//

SCENARIO("a rune does not remember a caster who has left the world", "[rune]")
{
	GIVEN("a rune placed by a character")
	{
		OBJ_DATA *obj = new_obj();
		CHAR_DATA *caster = new_char();
		RUNE_DATA *rune = PlaceRuneOnObj(obj, 11, RUNE_TRIGGER_EXIT, caster);

		REQUIRE(RuneOwner(rune) == caster);

		WHEN("the caster is freed while the rune still stands")
		{
			free_char(caster);

			THEN("the rune names nobody")
			{
				REQUIRE(RuneOwner(rune) == nullptr);
			}

			THEN("it still names nobody once the slot is reissued")
			{
				// The damage this does is misattribution rather than a crash:
				// the readers pass the owner to is_safe_new and damage_new, so
				// a recycled reference makes an unrelated character the author
				// of a stasis wall they never cast.
				//
				// The slot rather than the address, because a destroyed
				// character is really deleted now. SlotCount holding steady is
				// what proves the freed slot really was handed back out, which
				// is what keeps the assertion below from being trivially true.
				std::size_t slotsBefore = charHandles.SlotCount();

				CHAR_DATA *newcomer = new_char();

				REQUIRE(charHandles.SlotCount() == slotsBefore);
				REQUIRE(RuneOwner(rune) != newcomer);

				free_char(newcomer);
			}
		}

		ClearRunes(obj);
		free_obj(obj);
	}
}

SCENARIO("a recycled exit does not inherit the last one's rune", "[rune]")
{
	GIVEN("an exit that carried a rune and has been freed")
	{
		EXIT_DATA *pexit = new_exit();

		REQUIRE(pexit->rune == nullptr);

		// Stand in for a rune having been placed on the door. The pointer is
		// never followed here -- what matters is that new_exit clears it.
		pexit->rune = (RUNE_DATA *)0xdeadbeef;

		free_exit(pexit);

		WHEN("the exit allocator hands the same struct back out")
		{
			EXIT_DATA *reused = new_exit();

			THEN("it names no rune")
			{
				// new_exit resets every other field it hands back and used to
				// skip this one, so a recycled exit inherited both the stale
				// pointer and the stale flag that made it readable. The area
				// loader had the same hole from the other direction: it built
				// exits with an uninitialised rune pointer entirely.
				REQUIRE(reused == pexit);
				REQUIRE(reused->rune == nullptr);
			}

			free_exit(reused);
		}
	}
}

//
// The rune spell_stasis_wall hands to the queue.
//
// A drawn rune waits nine ticks before draw_rune finalises it. It used to live
// in the temp-struct pool, which is a bump allocator over a fixed buffer that
// wraps without regard for outstanding pointers -- so the queue entry named
// memory any other pool caller could take back. Ownership is now released into
// the queue and adopted back by draw_rune, which takes a unique_ptr by value.
//
// That last part is why this scenario no longer asserts on the freeing. It used
// to read `REQUIRE(new_rune() == drawn)` -- the free list handing the same
// address back was the only observable proof that a bail-out path had not
// stranded the struct. There is no free list to ask any more, and the property
// it was testing is now enforced by the signature rather than at runtime: every
// path out of draw_rune destroys the rune because the parameter owns it. A path
// that failed to would not compile into a leak, it would not exist.
//
// What is still worth pinning is the game-visible half: a draw that bails must
// not leave a rune applied. LeakSanitizer covers the other half.
//

SCENARIO("a draw that cannot complete applies nothing", "[rune]")
{
	GIVEN("a rune queued for drawing whose caster then leaves")
	{
		CHAR_DATA *caster = new_char();
		auto drawn = std::make_unique<RUNE_DATA>();

		drawn->owner = caster->self;
		drawn->drawn_in = 1;

		free_char(caster);

		REQUIRE(GlobalListLength() == 0);

		WHEN("the draw comes due with nobody left to complete it")
		{
			draw_rune(std::move(drawn));

			THEN("the draw is abandoned rather than applied")
			{
				// The owner handle expired with the caster, so draw_rune takes
				// its first bail-out path. Nothing reaches apply_rune, so
				// rune_list -- which owns applied runes -- stays empty.
				REQUIRE(GlobalListLength() == 0);
			}
		}
	}
}
