#include <string.h>
#include <stddef.h>
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
            int numberOfArguments = 1;
            char_data *mockPlayer;

            sut.AddToQueue(timer, numberOfArguments, testQueueFunction, mockPlayer);

			THEN("The queue should show a pending call")
			{
				REQUIRE(CQueue::HasQueuePending(mockPlayer));
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
		int argCount = 2;
		char_data* tmpChar = new char_data();
		tmpChar->id = 10107;
		long expected = 1337L;
		sut.AddToQueue(timer, argCount, updateValueFunction, tmpChar, expected);
		sut.ProcessQueue();
		THEN("The function should update the value")
		{
			REQUIRE(tmpChar->id == expected);
		}
	}
}