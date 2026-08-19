#include <cstring>
#include <limits>
#include <type_traits>
#include <vector>

#include "catch.hpp"
#include "../code/stdlibs/handle.h"

//
// These tests are deliberately standalone: nothing here touches CHAR_DATA,
// OBJ_DATA or the game at all. The point is to pin down the handle semantics
// on their own terms before any entity depends on them.
//
// The entity type below stands in for a recycled game struct. The only thing
// that matters is that it is an ordinary struct the slot map does not own.
//

namespace
{
	struct Widget
	{
		int id = 0;
	};

	// A slot map with an 8-bit generation counter. Identical behavior to the
	// production uint32_t width, but a generation counter that can actually be
	// exhausted inside a test.
	using NarrowMap = SlotMap<Widget, unsigned char>;
	using NarrowHandle = Handle<Widget, unsigned char>;
}

TEST_CASE("a default-constructed handle is null and derefs to nothing", "[handle]")
{
	SlotMap<Widget> map;
	Handle<Widget> handle;

	REQUIRE(handle.IsNull());
	REQUIRE(map.Deref(handle) == nullptr);
	REQUIRE(!map.IsLive(handle));
}

//
// This is the property that makes handles safe to embed in the recycled entity
// structs. Those structs are routinely reset by value-initializing a fresh
// temporary over them, and a few places still clear memory wholesale. Both
// produce all-zero bytes, so all-zero MUST mean "null", never "slot 0,
// generation 0". Generation 0 is reserved for exactly this reason.
//
TEST_CASE("an all-zero handle is null, so zeroing a struct clears it", "[handle]")
{
	SlotMap<Widget> map;
	Widget widget;

	auto handle = map.Add(&widget);
	REQUIRE(!handle.IsNull());

	// Handle's default member initializers make it a non-trivial type, so a
	// plain memset here draws -Wclass-memaccess. Casting the destination to
	// void * is the documented way to say the byte write is deliberate.
	//
	// That cast silences the diagnostic unconditionally though, including for a
	// Handle that had gained a vtable or a member owning heap memory. In either
	// of those cases this test would be memsetting something it must not, and
	// nothing would say so. The assertion states the property the test actually
	// depends on, so it fails loudly instead.
	static_assert(std::is_trivially_copyable_v<Handle<Widget>>,
		"These tests pin Handle's byte representation, so it must stay trivially copyable.");

	std::memset(static_cast<void *>(&handle), 0, sizeof(handle));

	REQUIRE(handle.IsNull());
	REQUIRE(map.Deref(handle) == nullptr);
}

TEST_CASE("a handle derefs to the entity it was made from", "[handle]")
{
	SlotMap<Widget> map;
	Widget first{11};
	Widget second{22};

	auto firstHandle = map.Add(&first);
	auto secondHandle = map.Add(&second);

	REQUIRE(map.Deref(firstHandle) == &first);
	REQUIRE(map.Deref(secondHandle) == &second);
	REQUIRE(map.Deref(firstHandle)->id == 11);
	REQUIRE(map.Deref(secondHandle)->id == 22);
	REQUIRE(firstHandle != secondHandle);
}

TEST_CASE("removing an entity expires its handle", "[handle]")
{
	SlotMap<Widget> map;
	Widget widget{7};

	auto handle = map.Add(&widget);
	REQUIRE(map.IsLive(handle));

	map.Remove(handle);

	REQUIRE(!map.IsLive(handle));
	REQUIRE(map.Deref(handle) == nullptr);

	// The entity itself is untouched -- the slot map never owned it.
	REQUIRE(widget.id == 7);
}

//
// The reason this whole mechanism exists. new_char() hands back recycled
// addresses, so a raw back-reference to a dead character can silently start
// pointing at whichever character next lands on that address. A stale handle
// must expire instead -- it must never resolve to the slot's new occupant.
//
TEST_CASE("a stale handle does not resolve to the entity that reuses its slot", "[handle]")
{
	SlotMap<Widget> map;
	Widget original{1};
	Widget replacement{2};

	auto staleHandle = map.Add(&original);
	map.Remove(staleHandle);

	auto freshHandle = map.Add(&replacement);

	// The slot was genuinely recycled -- this is not passing by accident.
	REQUIRE(map.SlotCount() == 1);
	REQUIRE(staleHandle != freshHandle);

	REQUIRE(map.Deref(staleHandle) == nullptr);
	REQUIRE(map.Deref(freshHandle) == &replacement);
}

TEST_CASE("handles compare by identity, including a handle to itself", "[handle]")
{
	SlotMap<Widget> map;
	Widget first;
	Widget second;

	auto firstHandle = map.Add(&first);
	auto copyOfFirst = firstHandle;
	auto secondHandle = map.Add(&second);

	REQUIRE(firstHandle == copyOfFirst);
	REQUIRE(firstHandle != secondHandle);

	// A follower whose leader is itself is load-bearing game logic, not a
	// dangling reference: self-comparison must hold and must stay live.
	REQUIRE(map.Deref(firstHandle) == map.Deref(copyOfFirst));
	REQUIRE(map.IsLive(copyOfFirst));

	Handle<Widget> nullHandle;
	Handle<Widget> anotherNullHandle;
	REQUIRE(nullHandle == anotherNullHandle);
	REQUIRE(nullHandle != firstHandle);
}

TEST_CASE("assigning nullptr clears a handle", "[handle]")
{
	SlotMap<Widget> map;
	Widget widget;

	auto handle = map.Add(&widget);
	REQUIRE(!handle.IsNull());

	handle = nullptr;

	REQUIRE(handle.IsNull());
	REQUIRE(map.Deref(handle) == nullptr);
}

//
// The distinction the deleted `operator==(nullptr_t)` protects. Assigning
// nullptr has to keep working, which means the converting constructor has to
// exist, which would otherwise make `handle == nullptr` compile and answer a
// different question than the raw-pointer null check it replaced.
//
TEST_CASE("a handle to a freed entity is not null, but derefs to null", "[handle]")
{
	SlotMap<Widget> map;
	Widget widget;

	auto handle = map.Add(&widget);
	map.Remove(handle);

	// "Was never assigned" and "no longer resolves" are different states, and
	// only the second one is what a null check on the old raw pointer meant.
	REQUIRE(!handle.IsNull());
	REQUIRE(map.Deref(handle) == nullptr);

	// So these two disagree, which is why asking the wrong one is a compile
	// error rather than a silent wrong answer.
	REQUIRE(handle.IsNull() != (map.Deref(handle) == nullptr));
}

//
// Removing something twice is a silent no-op, which the free_X functions now
// depend on directly: their double-free guard *is* `Deref(p->self) != p`, so
// the second Remove must not bump the generation again or it would burn a
// generation per stray call.
//
TEST_CASE("removing an already-expired handle is a no-op", "[handle]")
{
	SlotMap<Widget> map;
	Widget widget;
	Widget replacement;

	auto handle = map.Add(&widget);
	map.Remove(handle);
	map.Remove(handle);
	map.Remove(handle);

	REQUIRE(map.LiveCount() == 0);
	REQUIRE(map.SlotCount() == 1);

	auto freshHandle = map.Add(&replacement);
	REQUIRE(map.Deref(freshHandle) == &replacement);
	REQUIRE(map.Deref(handle) == nullptr);
}

TEST_CASE("a handle with an out-of-range slot derefs to nothing", "[handle]")
{
	SlotMap<Widget> map;
	Widget widget;

	auto handle = map.Add(&widget);
	auto garbage = handle;

	// Same reasoning as the all-zero test above: the cast is what silences
	// -Wclass-memaccess, and the assertion is what keeps the cast honest.
	static_assert(std::is_trivially_copyable_v<Handle<Widget>>,
		"These tests pin Handle's byte representation, so it must stay trivially copyable.");

	std::memset(static_cast<void *>(&garbage), 0xFF, sizeof(garbage));

	REQUIRE(map.Deref(garbage) == nullptr);
	REQUIRE(!map.IsLive(garbage));

	// The real handle is unaffected.
	REQUIRE(map.Deref(handle) == &widget);
}

TEST_CASE("adding null is rejected rather than occupying a slot", "[handle]")
{
	SlotMap<Widget> map;

	auto handle = map.Add(nullptr);

	REQUIRE(handle.IsNull());
	REQUIRE(map.SlotCount() == 0);
	REQUIRE(map.LiveCount() == 0);
}

TEST_CASE("freed slots are reused rather than growing the map", "[handle]")
{
	SlotMap<Widget> map;
	std::vector<Widget> widgets(4);
	std::vector<Handle<Widget>> handles;

	for (auto &widget : widgets)
		handles.push_back(map.Add(&widget));

	REQUIRE(map.SlotCount() == 4);
	REQUIRE(map.LiveCount() == 4);

	map.Remove(handles[1]);
	map.Remove(handles[2]);

	REQUIRE(map.LiveCount() == 2);
	REQUIRE(map.SlotCount() == 4);

	Widget extra;
	auto extraHandle = map.Add(&extra);

	REQUIRE(map.SlotCount() == 4);
	REQUIRE(map.LiveCount() == 3);
	REQUIRE(map.Deref(extraHandle) == &extra);

	// The survivors are untouched by all the churn around them.
	REQUIRE(map.Deref(handles[0]) == &widgets[0]);
	REQUIRE(map.Deref(handles[3]) == &widgets[3]);
	REQUIRE(map.Deref(handles[1]) == nullptr);
	REQUIRE(map.Deref(handles[2]) == nullptr);
}

//
// The one failure mode a generational handle can still have: if a slot's
// generation counter wraps, an ancient handle can start resolving to a live
// entity -- the wrong one. Silent, and no sanitizer catches it. The fix is to
// retire a slot permanently once its counter is spent rather than reissue a
// colliding generation, so the miss stays a miss forever.
//
TEST_CASE("a slot is retired rather than wrapping its generation", "[handle]")
{
	NarrowMap map;
	Widget widget;

	NarrowHandle firstHandle = map.Add(&widget);

	// Churn the single slot until its generation counter is spent. The bound
	// is deliberate: an implementation that never retires the slot should fail
	// an assertion here rather than spin forever.
	const int recycleLimit = 4 * (std::numeric_limits<unsigned char>::max() + 1);
	int recycles = 0;

	NarrowHandle handle = firstHandle;
	while (map.SlotCount() == 1 && recycles < recycleLimit)
	{
		map.Remove(handle);
		handle = map.Add(&widget);
		recycles++;
	}

	REQUIRE(recycles < recycleLimit);

	// The slot was retired instead of reissuing firstHandle's generation.
	REQUIRE(map.SlotCount() == 2);
	REQUIRE(map.LiveCount() == 1);
	REQUIRE(handle != firstHandle);
	REQUIRE(map.Deref(firstHandle) == nullptr);
	REQUIRE(map.Deref(handle) == &widget);

	// And the retired slot stays retired -- it is never handed out again.
	map.Remove(handle);
	Widget another;
	auto laterHandle = map.Add(&another);

	REQUIRE(laterHandle != firstHandle);
	REQUIRE(map.Deref(firstHandle) == nullptr);
}

TEST_CASE("every handle ever issued by a map stays distinct from every other", "[handle]")
{
	NarrowMap map;
	std::vector<NarrowHandle> issued;
	std::vector<Widget> widgets(3);

	// Enough churn to exhaust and retire several slots.
	for (int round = 0; round < 400; round++)
	{
		std::vector<NarrowHandle> live;

		for (auto &widget : widgets)
		{
			auto handle = map.Add(&widget);
			issued.push_back(handle);
			live.push_back(handle);
		}

		for (auto handle : live)
			map.Remove(handle);
	}

	REQUIRE(map.LiveCount() == 0);

	// Repopulate, so the assertion below is about collision and not merely
	// about an empty map. Every issued handle above has been removed, so none
	// of them may resolve to any of these -- a wrapped generation is exactly
	// how one would.
	std::vector<Widget> survivors(8);
	for (auto &widget : survivors)
		map.Add(&widget);

	REQUIRE(map.LiveCount() == 8);

	for (auto handle : issued)
		REQUIRE(map.Deref(handle) == nullptr);
}

//
// Handles are stored inside the recycled entity structs, which are copied and
// value-initialized wholesale. They must stay cheap and trivial to keep that
// legal, and small enough that swapping a raw pointer for a handle does not
// grow the structs.
//
TEST_CASE("a handle is a trivially copyable 64-bit value", "[handle]")
{
	REQUIRE(sizeof(Handle<Widget>) == 8);
	REQUIRE(std::is_trivially_copyable<Handle<Widget>>::value);
	REQUIRE(std::is_trivially_destructible<Handle<Widget>>::value);
	REQUIRE(std::is_standard_layout<Handle<Widget>>::value);
}
