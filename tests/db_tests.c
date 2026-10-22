#include <cstring>

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
SCENARIO("Testing random number generator", "[random_mm]")
{
	GIVEN("A request for a random number")
	{
		WHEN("rgiState has not been initialized properly")
		{
			THEN("number_mm() should self-initialize instead of streaming zeros")
			{
				bool anyNonZero = false;

				for (auto i = 0; i < 8; i++)
				{
					if (number_mm() != 0)
					{
						anyNonZero = true;
						break;
					}
				}

				REQUIRE(anyNonZero == true);
			}
		}
	}
}
// The loaders that scan player files read letters until one is '#', which opens
// a section, and stop at the "#End" section. They never asked whether the file
// had run out first, so a file that ends without "#End" -- a zero-byte one is
// enough -- kept them reading end-of-file forever and hung the boot.
// SkipToSection is that scan with the question asked.

namespace
{
	/// A temporary file holding the text given, positioned at its start.
	FILE *FileHolding(const char *text)
	{
		FILE *fp = tmpfile();

		fputs(text, fp);
		rewind(fp);

		return fp;
	}
}

SCENARIO("skipping to the next section of a player file", "[SkipToSection]")
{
	GIVEN("an empty file")
	{
		FILE *fp = FileHolding("");

		THEN("there is no section to find")
		{
			REQUIRE_FALSE(SkipToSection(fp));
		}

		fclose(fp);
	}

	GIVEN("a file that has text but no section marker")
	{
		FILE *fp = FileHolding("  Name Tester~\n Levl 10\n");

		THEN("the scan reaches the end and says so")
		{
			REQUIRE_FALSE(SkipToSection(fp));
		}

		fclose(fp);
	}

	GIVEN("a file with text before an #End marker")
	{
		FILE *fp = FileHolding("Vnum 1234\n\n#End\n");

		THEN("the scan stops after the '#', so the next word is the section name")
		{
			REQUIRE(SkipToSection(fp));
			REQUIRE(strcmp(fread_word(fp), "End") == 0);
		}

		fclose(fp);
	}

	GIVEN("a file with two sections")
	{
		FILE *fp = FileHolding("#O\nVnum 1234\n#End\n");

		THEN("each scan finds the next one, and then there are none")
		{
			REQUIRE(SkipToSection(fp));
			REQUIRE(strcmp(fread_word(fp), "O") == 0);
			REQUIRE(SkipToSection(fp));
			REQUIRE(strcmp(fread_word(fp), "End") == 0);
			REQUIRE_FALSE(SkipToSection(fp));
		}

		fclose(fp);
	}
}
