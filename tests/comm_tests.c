#include <cstring>
#include <fcntl.h>
#include <unistd.h>

#include "catch.hpp"
#include "../code/comm.h"
#include "../code/merc.h"
#include "world_fixture.h"

// process_output draws the prompt, and drawing the prompt is what can destroy
// the character it is drawn for: every write goes through write_to_buffer, and
// a buffer that passes 32000 bytes closes the socket and frees whoever was on
// the other end. The re-reads in that function exist for exactly that reason.
// These cover what happens when one of them comes back empty.
//
// A fixture connection carries no socket, and the flush at the end of
// process_output fails without one, so these tests hand it a writable file
// descriptor. That is what makes the return value mean "the prompt was drawn"
// rather than "there was nowhere to send it".

namespace
{
	/// A writable descriptor that discards, so write_to_descriptor succeeds and
	/// the return value of process_output reports the prompt rather than the socket.
	class NullSink
	{
	public:
		NullSink() : fd(open("/dev/null", O_WRONLY)) {}
		~NullSink() { if (fd >= 0) close(fd); }

		NullSink(const NullSink &) = delete;
		NullSink &operator=(const NullSink &) = delete;

		int fd;
	};
}

SCENARIO("the prompt is not drawn for a character that is already gone", "[process_output]")
{
	GIVEN("a playing connection whose character has been freed")
	{
		TestWorld world;
		NullSink sink;
		auto room = world.CreateRoom();
		auto player = world.CreatePlayer("Ghost", room);
		auto connection = Deref(player->desc);

		REQUIRE(sink.fd >= 0);
		REQUIRE(connection != nullptr);
		REQUIRE(Deref(connection->character) == player);

		connection->descriptor = sink.fd;

		WHEN("the character is freed while the connection is still open")
		{
			free_char(player);

			REQUIRE(Deref(connection->character) == nullptr);

			THEN("process_output reports failure rather than reading through the dead handle")
			{
				// Without the guard this dereferences a retired handle and the
				// process dies here. The return value is the visible half, and
				// not crashing is the half under test.
				REQUIRE(process_output(connection, true) == false);
			}

			THEN("nothing is written to the connection on the way out")
			{
				connection->outtop = 0;

				process_output(connection, true);

				// Every prompt write happens after the resolve, so bailing at
				// the resolve leaves the output buffer untouched.
				REQUIRE(connection->outtop == 0);
			}
		}

		connection->descriptor = -1;
	}
}

SCENARIO("a connection with a live character still reaches the prompt", "[process_output]")
{
	GIVEN("a playing connection whose character is alive")
	{
		TestWorld world;
		NullSink sink;
		auto room = world.CreateRoom();
		auto player = world.CreatePlayer("Solid", room);
		auto connection = Deref(player->desc);

		REQUIRE(sink.fd >= 0);
		REQUIRE(connection != nullptr);

		connection->descriptor = sink.fd;

		WHEN("output is processed with the prompt asked for")
		{
			connection->outtop = 0;

			bool drawn = process_output(connection, true);

			THEN("the prompt is drawn and the write succeeds")
			{
				// The reason the test above means anything: the guard has to
				// stop the dead case without also stopping the live one. Same
				// call, same connection, opposite answer.
				REQUIRE(drawn == true);
			}
		}

		connection->descriptor = -1;
	}
}
