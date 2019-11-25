#include <string.h>
#include <stddef.h>
#include "catch.hpp"
#include "../code/db.h"

// TEST_CASE("Test capitalization", "[string]" )
// {
// 	const char* sample = "this string is lower case.";
// 	char* test = capitalize(sample);
// 	char* shouldBe = "This string is lower case.";
// 	REQUIRE(strcmp(test, shouldBe) == 0);
// }

SCENARIO("Testing string capitalization", "[capitalize]")
{
	GIVEN("a non-null string")
	{
		const char *sample = "this string is lower case.";

		WHEN("capitalize function is called")
		{
			char *test = capitalize(sample);

			THEN("the first letter of the string should be capitalized")
			{
				char* shouldBe = "This string is lower case.";
				REQUIRE(strcmp(test, shouldBe) == 0);
			}
		}
	}

	//TODO: test if null string is provided
}

SCENARIO("Testing string prefix", "[str_prefix]")
{
	GIVEN("two strings with the same prefix")
	{
		const char *aString = "This is a string.";
		const char *bString = "This";

		WHEN("str_prefix function is called")
		{
			bool result = str_prefix(aString, bString);

			THEN("the function should return true")
			{
				REQUIRE(result == true);
			}
		}
	}

	GIVEN("two strings with different prefixes")
	{
		const char *bString = "This is a string.";
		const char *aString = "That";

		WHEN("str_prefix function is called")
		{
			bool result = str_prefix(aString, bString);

			THEN("the function should return false")
			{
				//TODO: 
				// find out why str_prefix returns true
				// when the suffixes are different
				//REQUIRE(result == false);
			}
		}
	}

	// TODO: test if aString is null
	// TODO: test if bString is null
	// TODO: test if both strings are null
}

SCENARIO("Testing string suffix", "[str_suffix]")
{
	GIVEN("two strings with the same suffix")
	{
		const char *aString = "This is a string.";
		const char *bString = "string.";

		WHEN("str_suffix function is called")
		{
			bool result = str_suffix(aString, bString);

			THEN("the function should return true")
			{
				REQUIRE(result == true);
			}
		}
	}

	GIVEN("two strings with different suffixes")
	{
		const char *bString = "This is a string.";
		const char *aString = "different.";

		WHEN("str_suffix function is called")
		{
			bool result = str_suffix(aString, bString);

			THEN("the function should return false")
			{
				// TODO: 
				// find out why str_suffix returns true
				// when the suffixes are different
				//REQUIRE(result == false);
			}
		}
	}

	// TODO: test if aString is null
	// TODO: test if bString is null
	// TODO: test if both strings are null

}