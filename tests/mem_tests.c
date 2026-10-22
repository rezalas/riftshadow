#include "catch.hpp"
#include "../code/entity/obj_index_data.h"
#include "../code/mem.h"

SCENARIO("a new object prototype starts with no wear or remove echoes", "[new_obj_index]")
{
	GIVEN("a freshly made object prototype")
	{
		// wear_echo and remove_echo hold two strings each. The constructor
		// cleared a third slot in both, which wrote past each array into
		// whatever followed it. The sanitizer build's bounds check reports it.
		OBJ_INDEX_DATA *proto = new_obj_index();

		THEN("both echo pairs are empty")
		{
			REQUIRE(proto->wear_echo[0] == nullptr);
			REQUIRE(proto->wear_echo[1] == nullptr);
			REQUIRE(proto->remove_echo[0] == nullptr);
			REQUIRE(proto->remove_echo[1] == nullptr);
			REQUIRE(proto->verb == nullptr);
		}

		free_obj_index(proto);
	}
}
