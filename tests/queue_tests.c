#include "catch.hpp"
#include "../code/queue.h"
#include "../code/merc.h"
#include "../code/act_comm.h"
 
void testQueueFunction(char_data *qChar) {
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
			char_data *mockPlayer = new char_data();
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
		char_data *mockPlayer = new char_data();
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

void inertQueueFunction(char_data *qChar)
{
	inertCallCount++;
	return;
}

// Reproduces a re-entrant erase bug found in the code.
void wideArgQueueFunction(char_data *qChar, long a, long b, long c)
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
		char_data *mockVictim = new char_data();
		mockVictim->name = "Victim";

		// All four are due this tick and all name the same char. The first
		// survives the cancel (its timer is already 0 by the time it runs, and
		// DeleteQueuedEventsInvolving only removes delay > 0); the other three
		// are erased out from under the loop while still due.
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
		char_data *mockPlayer = new char_data();
		mockPlayer->name = "Test";

		// Four entries leaves the vector at size == capacity == 4, so the
		// re-entrant add below is guaranteed to reallocate.
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

SCENARIO("Testing queue processing", "[ProcessQueue]")
{
	GIVEN("A queue with a ready-to-run function")
	{
		CQueue sut;
		int timer = 1;
		char_data* tmpChar = new char_data();
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
		char_data* tmpChar = new char_data();
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