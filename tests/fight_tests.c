#include <string.h>
#include <stddef.h>
#include "catch.hpp"
#include "../code/merc.h"
#include "../code/fight.h"

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