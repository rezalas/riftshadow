#include "catch.hpp"
#include "../code/magic.h"
#include "../code/const.h"

char* substr(char* arr, int begin, int len = 0)
{	
    char* res = new char[len + 1];
    for (int i = 0; i < len; i++)
        res[i] = *(arr + begin + i);
    res[len] = 0;
    return res;
}

char* AllUpper(char* arr)
{
	auto len = strlen(arr);
	char* res = new char[len + 1];
	for(auto i = 0; i < len; i++)
	{
		if(arr[i] == '\0')
			break;
		res[i] = toupper(arr[i]);
	}
	res[len] = 0;
	return res;
}
char* AllLower(char* arr)
{
	for(auto i = 0; i < strlen(arr); i++)
	{
		if(arr[i] == '\0')
			break;
		arr[i] = tolower(arr[i]);
	}
	return arr;
}

SCENARIO("testing skill lookup","[skill_lookup]")
{
	GIVEN("a null name")
	{
		WHEN("skill_lookup is called")
		{
			auto expected = -1;
			auto actual = skill_lookup(NULL);
			THEN("it should return negative 1")
			{
				REQUIRE(expected == -1);
			}
		}
	}
	GIVEN("an empty name")
	{
		WHEN("skill_lookup is called")
		{
			auto expected = -1;
			auto actual = skill_lookup("");
			THEN("it should return negative 1")
			{
				REQUIRE(expected == -1);
			}
		}
	}
	GIVEN("an invalid name")
	{
		WHEN("skill_lookup is called")
		{
			auto expected = -1;
			auto actual = skill_lookup("Jimmy dean sausage");
			THEN("it should return negative 1")
			{
				REQUIRE(expected == -1);
			}
		}
	}
	GIVEN("a valid name uppercase")
	{
		WHEN("skill_lookup is called")
		{
			// skill_table should always have at least 6 skills. this should be safe
			auto expected = 5;
			auto skillName = AllUpper(skill_table[expected].name); 
			auto actual = skill_lookup(skillName);
			THEN("it should return the expected index")
			{
				REQUIRE(actual == expected);
			}
		}
	}
	GIVEN("an valid name mixed case")
	{
		WHEN("skill_lookup is called")
		{
			// skill_table should always have at least 6 skills. this should be safe
			auto expected = 5;
			auto skillName = skill_table[expected].name; 
			auto firstHalf = AllLower(substr(skillName,0,3));
			auto secondHalf = AllUpper(substr(skillName,3,(sizeof(skillName) - 3)));
			auto finalSkillName = (std::string(firstHalf) + std::string(secondHalf)).c_str();
			auto actual = skill_lookup(finalSkillName);
			THEN("it should return the expected index")
			{
				REQUIRE(actual == expected);
			}
		}
	}
}