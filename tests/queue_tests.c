#include "catch.hpp"
#include "../code/queue.h"
#include "../code/merc.h"
 
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

			sut.AddToQueue(timer, testQueueFunction, mockPlayer);

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
		int timer = 3; // 3 tics 

		sut.AddToQueue(timer, testQueueFunction, mockPlayer);

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

SCENARIO("Testing queue processing", "[ProcessQueue]")
{
	GIVEN("A queue with a ready-to-run function")
	{
		CQueue sut;
		int timer = 1;
		char_data* tmpChar = new char_data();
		tmpChar->id = 10107;
		long expected = 1337L;
		sut.AddToQueue(timer, updateValueFunction, tmpChar, expected);
		sut.ProcessQueue();
		THEN("The function should update the value")
		{
			REQUIRE(tmpChar->id == expected);
		}
	}
}