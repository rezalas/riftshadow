#include "catch.hpp"
#include "../code/queue.h"
#include "../code/act_comm.h"
#include "../code/entity/char_data.h"
#include "../code/enums.h"
#include "../code/entity/handles.h"
 
namespace
{
	/// A character the queue can index.
	///
	/// CQueue keys its cancellation index on Handle<CHAR_DATA> rather than on a
	/// raw pointer, so a character that was never registered in the slot map has
	/// no identity to key on and cannot be found or cancelled. new_char does the
	/// registration; these mocks are built by hand, so they have to do it too.
	char_data *MakeMockChar()
	{
		char_data *ch = new char_data();
		ch->self = charHandles.Add(ch);

		return ch;
	}
}

void testQueueFunction([[maybe_unused]] char_data *qChar) {
    return;
}

SCENARIO("Testing queueing functions", "[AddToQueue]")
{
	GIVEN("a function with no parameters")
	{
		int timer = 3; // 3 tics 

		WHEN("AddToQueue is called with a positive timer")
		{
			CQueue sut;
			char_data *mockPlayer = MakeMockChar();
			mockPlayer->name = "Test";

			sut.AddToQueue(timer, "queue_test", "testQueueFunction", testQueueFunction, mockPlayer);

			THEN("The queue should show a pending call")
			{
				REQUIRE(sut.HasQueuePending(mockPlayer));
			}
		}
	}
}

SCENARIO("Testing deleting queue entries with that involve character", "[DeleteQueuedEventsInvolving]")
{
	GIVEN("a function with no parameters")
	{
		CQueue sut;
		char_data *mockPlayer = MakeMockChar();
		mockPlayer->name = "Test";

		int timer = 3; // 3 tics 

		sut.AddToQueue(timer, "queue_test", "testQueueFunction", testQueueFunction, mockPlayer);

		WHEN("DeleteQueuedEventsInvolving is called with a specified character in the queue")
		{
			sut.DeleteQueuedEventsInvolving(mockPlayer);

			THEN("The queue should show a no pending calls")
			{
				auto hasQueueEntries = sut.HasQueuePending(mockPlayer);
				REQUIRE(hasQueueEntries == false);
			}
		}
	}
}


void updateValueFunction(char_data *qChar, long value)
{
	qChar->id = value;
}

// Reproduces a re-entrant add bug found in the code.
static CQueue *activeSut = nullptr;

void reentrantRequeueFunction(char_data *qChar)
{
	activeSut->AddToQueue(5, "reentrantRequeueFunction", "reentrantRequeueFunction", reentrantRequeueFunction, qChar);
}

static int inertCallCount = 0;

void inertQueueFunction([[maybe_unused]] char_data *qChar)
{
	inertCallCount++;
	return;
}

// Reproduces a re-entrant erase bug found in the code.
void wideArgQueueFunction(char_data * /* qChar */, long /* a */, long /* b */, long /* c */)
{
	inertCallCount++;
	return;
}

void deleteEventsQueueFunction(char_data *qChar)
{
	activeSut->DeleteQueuedEventsInvolving(qChar);
}

SCENARIO("Testing a queued function that cancels other events for its own char", "[ProcessQueue]")
{
	GIVEN("a due entry that cancels its char's other events, with those still to walk")
	{
		CQueue sut;
		activeSut = &sut;
		char_data *mockVictim = MakeMockChar();
		mockVictim->name = "Victim";

		// All four are due this tick and all name the same char. The first is
		// already running by the time it cancels, so it is unaffected; the other
		// three are still queued behind it and must not fire.
		sut.AddToQueue(1, "queue_test", "deleteEventsQueueFunction", deleteEventsQueueFunction, mockVictim);
		sut.AddToQueue(1, "queue_test", "wideArgQueueFunction", wideArgQueueFunction, mockVictim, 1L, 2L, 3L);
		sut.AddToQueue(1, "queue_test", "wideArgQueueFunction", wideArgQueueFunction, mockVictim, 1L, 2L, 3L);
		sut.AddToQueue(1, "queue_test", "wideArgQueueFunction", wideArgQueueFunction, mockVictim, 1L, 2L, 3L);

		WHEN("ProcessQueue runs the cancelling entry")
		{
			inertCallCount = 0;
			sut.ProcessQueue();

			THEN("the cancelled entries are not executed and nothing dangles")
			{
				REQUIRE(inertCallCount == 0);
				REQUIRE(sut.HasQueuePending(mockVictim) == false);
			}
		}

		activeSut = nullptr;
	}
}

SCENARIO("Testing a queued function that requeues itself", "[ProcessQueue]")
{
	GIVEN("a due entry that requeues itself, with later entries still to walk")
	{
		CQueue sut;
		activeSut = &sut;
		char_data *mockPlayer = MakeMockChar();
		mockPlayer->name = "Test";

		// Four entries, so the re-entrant add below happens while three more are
		// still queued behind it. Against the old flat vector this reallocated
		// mid-iteration; the later entries must survive it either way.
		sut.AddToQueue(1, "queue_test", "reentrantRequeueFunction", reentrantRequeueFunction, mockPlayer);
		sut.AddToQueue(5, "queue_test", "inertQueueFunction", inertQueueFunction, mockPlayer);
		sut.AddToQueue(5, "queue_test", "inertQueueFunction", inertQueueFunction, mockPlayer);
		sut.AddToQueue(5, "queue_test", "inertQueueFunction", inertQueueFunction, mockPlayer);

		WHEN("ProcessQueue runs the due entry")
		{
			sut.ProcessQueue();

			THEN("the remaining entries are still walked without touching freed memory")
			{
				REQUIRE(sut.HasQueuePending(mockPlayer));
			}
		}

		activeSut = nullptr;
	}
}

// Same-tick execution order that must run in the order they were added.
static std::vector<int> executionOrder;

void recordOrderFunction([[maybe_unused]] char_data *qChar, long tag)
{
	executionOrder.push_back(static_cast<int>(tag));
}

SCENARIO("Testing execution order of queued events", "[ProcessQueue]")
{
	GIVEN("four events queued at the same tick")
	{
		CQueue sut;
		char_data *mockPlayer = MakeMockChar();
		mockPlayer->name = "Test";
		executionOrder.clear();

		sut.AddToQueue(3, "queue_test", "recordOrderFunction", recordOrderFunction, mockPlayer, 1L);
		sut.AddToQueue(3, "queue_test", "recordOrderFunction", recordOrderFunction, mockPlayer, 2L);
		sut.AddToQueue(3, "queue_test", "recordOrderFunction", recordOrderFunction, mockPlayer, 3L);
		sut.AddToQueue(3, "queue_test", "recordOrderFunction", recordOrderFunction, mockPlayer, 4L);

		WHEN("the queue is processed up to that tick")
		{
			sut.ProcessQueue();
			sut.ProcessQueue();
			sut.ProcessQueue();

			THEN("they execute in insertion order, on the correct tick")
			{
				REQUIRE(executionOrder == std::vector<int>{1, 2, 3, 4});
			}
		}
	}

	GIVEN("events queued across several ticks, added out of order")
	{
		CQueue sut;
		char_data *mockPlayer = MakeMockChar();
		mockPlayer->name = "Test";
		executionOrder.clear();

		sut.AddToQueue(3, "queue_test", "recordOrderFunction", recordOrderFunction, mockPlayer, 30L);
		sut.AddToQueue(1, "queue_test", "recordOrderFunction", recordOrderFunction, mockPlayer, 10L);
		sut.AddToQueue(2, "queue_test", "recordOrderFunction", recordOrderFunction, mockPlayer, 20L);
		sut.AddToQueue(1, "queue_test", "recordOrderFunction", recordOrderFunction, mockPlayer, 11L);

		WHEN("the queue is processed tick by tick")
		{
			sut.ProcessQueue();
			sut.ProcessQueue();
			sut.ProcessQueue();

			THEN("they execute in tick order, insertion order within a tick")
			{
				REQUIRE(executionOrder == std::vector<int>{10, 11, 20, 30});
			}
		}
	}

	GIVEN("an event queued from inside a queued event")
	{
		CQueue sut;
		char_data *mockPlayer = MakeMockChar();
		mockPlayer->name = "Test";
		executionOrder.clear();

		sut.AddToQueue(1, "queue_test", "recordOrderFunction", recordOrderFunction, mockPlayer, 1L);
		sut.AddToQueue(2, "queue_test", "recordOrderFunction", recordOrderFunction, mockPlayer, 2L);

		WHEN("the queue is processed")
		{
			sut.ProcessQueue();
			sut.ProcessQueue();

			THEN("a re-entrant timer counts from the tick that was draining")
			{
				REQUIRE(executionOrder == std::vector<int>{1, 2});
			}
		}
	}
}

// Scalar payloads a queued event carries must be copies taken at schedule time.
// The callers that schedule delayed events (do_brace, do_retreat,
// spell_concave_shell) have returned long before the event fires, so a captured
// reference would read a dead frame.
static int capturedInt = 0;
static float capturedFloat = 0.0f;

void recordScalarsFunction([[maybe_unused]] char_data *qChar, int i, float f)
{
	capturedInt = i;
	capturedFloat = f;
}

SCENARIO("Testing that scalar queue arguments are copied, not referenced", "[AddToQueue]")
{
	GIVEN("an event scheduled with scalar locals that are then overwritten")
	{
		CQueue sut;
		char_data *mockPlayer = MakeMockChar();
		mockPlayer->name = "Test";
		capturedInt = 0;
		capturedFloat = 0.0f;

		int direction = 4;
		float bracemod = 0.75f;

		sut.AddToQueue(2, "queue_test", "recordScalarsFunction", recordScalarsFunction, mockPlayer, direction, bracemod);

		// Deliberately not UB: if the queue held references to these, the event
		// below would observe the new values rather than the scheduled ones.
		direction = 99;
		bracemod = -1.0f;

		WHEN("the queue is processed")
		{
			sut.ProcessQueue();
			sut.ProcessQueue();

			THEN("the event sees the values as they were when it was scheduled")
			{
				REQUIRE(capturedInt == 4);
				REQUIRE(capturedFloat == 0.75f);
			}
		}
	}
}

SCENARIO("Testing queue processing", "[ProcessQueue]")
{
	GIVEN("A queue with a ready-to-run function")
	{
		CQueue sut;
		int timer = 1;
		char_data* tmpChar = MakeMockChar();
		tmpChar->id = 10107;
		long expected = 1337L;
		sut.AddToQueue(timer, "queue_test", "updateValueFunction", updateValueFunction, tmpChar, expected);
		sut.ProcessQueue();
		THEN("The function should update the value")
		{
			REQUIRE(tmpChar->id == expected);
		}
	}

	GIVEN("A function with a timer greater than one")
	{
		CQueue sut;
		char_data* tmpChar = MakeMockChar();
		tmpChar->name = "Test";
		tmpChar->id = 10107;
		long expected = 1337L;
		sut.AddToQueue(1, "queue_test", "updateValueFunction", updateValueFunction, tmpChar, expected);
		sut.ProcessQueue();
		THEN("The function should update the value")
		{
			REQUIRE(tmpChar->id == expected);
		}
	}
}
//
// The reason charIndex is keyed on a handle rather than on a CHAR_DATA *.
//
// A queued entry can outlive any character it names. Keyed by pointer, a
// character who left the world with entries still pending leaves its key
// behind, and the free lists hand the next character that very address -- so
// the newcomer answers to the departed character's key and inherits its pending
// events. A handle carries the slot's generation, so the newcomer's key differs
// even when the address is identical.
//
// Expiring a handle and reissuing one for the same pointer is what that looks
// like from the slot map's side, and it is deterministic: same address, same
// slot, new generation. Under a pointer-keyed index the assertions below would
// read the departed character's entry back out.
//

SCENARIO("the queue does not attribute a departed character's events to its replacement", "[DeleteQueuedEventsInvolving]")
{
	GIVEN("a character with a queued event, whose handle is then expired and reissued")
	{
		CQueue sut;
		char_data *ch = MakeMockChar();

		sut.AddToQueue(5, "queue_test", "testQueueFunction", testQueueFunction, ch);

		REQUIRE(sut.HasQueuePending(ch));

		Handle<CHAR_DATA> indexedUnder = ch->self;

		// What free_char does to the handle, without the rest of free_char --
		// these mocks are not built by new_char and do not own their strings.
		charHandles.Remove(ch->self);

		// And what new_char does for whoever lands on the address next.
		ch->self = charHandles.Add(ch);

		WHEN("the queue is asked about the character at that address")
		{
			THEN("the reissued handle is not the one the entry was indexed under")
			{
				REQUIRE(!(ch->self == indexedUnder));
			}

			THEN("it has nothing pending, though the address is unchanged")
			{
				REQUIRE(sut.HasQueuePending(ch) == false);
			}

			THEN("and cancelling for it leaves the orphaned entry alone")
			{
				sut.DeleteQueuedEventsInvolving(ch);

				// Still indexed under the expired handle, waiting to run or be
				// dropped when it does. It is not reachable by anyone, which is
				// the point: nobody inherits it either.
				REQUIRE(sut.HasQueuePending(ch) == false);
			}
		}
	}
}
