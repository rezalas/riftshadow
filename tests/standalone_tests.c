#include <string>
#include "catch.hpp"
#include "../code/merc.h"
#include "../code/include/fmt/format.h"

TEST_CASE("test between format and sprintf", "[standalone]")
{
	char buf[MAX_STRING_LENGTH];
	
	char site[] = "den.megapath.net";

	sprintf(buf, "BANNED - %s just tried to connect.", site);
	auto buf2 = fmt::format("BANNED - {} just tried to connect.", site);
	// sprintf(), data() and c_str() are null terminated 
	// data() returns char*
	// c_str() returns const char*
	REQUIRE(strcmp(buf, buf2.data()) == 0); 
	REQUIRE(strcmp(buf, buf2.c_str()) == 0);
}

TEST_CASE("test multiple format arguments", "[standalone]")
{
	char buf[MAX_STRING_LENGTH];
	
	char arg[] = "up";
	char argument[] = "forward";

	sprintf(buf, "%s is now realiased to '%s'.\n\r", arg, argument);
	auto buf2 = fmt::format("{} is now realiased to '{}'.\n\r", arg, argument); 
	// sprintf(), data() and c_str() are null terminated 
	// data() returns char*
	// c_str() returns const char*
	REQUIRE(strcmp(buf, buf2.data()) == 0); 
	REQUIRE(strcmp(buf, buf2.c_str()) == 0);
}

TEST_CASE("test overwrite", "[standalone]")
{
	auto buf = fmt::format("{} is now realiased to '{}'.\n\r", "up", "forward");
	buf = fmt::format("{} is now realiased to '{}'.\n\r", "down", "back"); 
	auto buf2 = std::string("down is now realiased to 'back'.\n\r");
	REQUIRE(strcmp(buf.data(), buf2.data()) == 0); 
}

TEST_CASE("test mixed types", "[standalone]")
{
	auto flag = false;

	auto buf = fmt::format("the meaning of life is {}", flag ? "unknown" : std::to_string(42));
	auto buf2 = std::string("the meaning of life is unknown");
	auto buf3 = std::string("the meaning of life is 42");
	REQUIRE(strcmp(buf.data(), buf3.data()) == 0); 

	flag = true;
	buf = fmt::format("the meaning of life is {}", flag ? "unknown" : std::to_string(42));
	REQUIRE(strcmp(buf.data(), buf2.data()) == 0); 
}

TEST_CASE("test extra placeholders", "[standalone]")
{
	try
	{
		auto buf = fmt::format("the meaning of life is {}{}", "unknown");
		auto buf2 = std::string("the meaning of life is unknown");
		REQUIRE(strcmp(buf.data(), buf2.data()) != 0); // these strings arent equal
	}catch(const std::exception& e){} // ignore the exception, we know that there is a missing argument
}

TEST_CASE("test missing placeholders", "[standalone]")
{
	auto buf = fmt::format("the meaning of life is ", "unknown");
	auto buf2 = std::string("the meaning of life is unknown");
	REQUIRE(strcmp(buf.data(), buf2.data()) != 0); // these strings arent equal
}

TEST_CASE("test if null and nullptr are both valid for uninstantiated types", "[standalone]")
{

	class Test
	{
		Test()
		{
		}
	};

	Test* test = nullptr;

	REQUIRE(test == NULL);
	REQUIRE(test == nullptr);

	test = NULL;

	REQUIRE(test == NULL);
	REQUIRE(test == nullptr);

	int* test2 = nullptr;

	REQUIRE(test2 == NULL);
	REQUIRE(test2 == nullptr);

	test2 = nullptr;

	REQUIRE(test2 == NULL);
	REQUIRE(test2 == nullptr);

}
