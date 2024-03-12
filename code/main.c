#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "merc.h"
#include "mud.h"
#include "comm.h"
#include "interp.h"
#include "db.h"
#include "./include/spdlog/spdlog.h"

int main(int argc, char **argv)
{
	struct timeval now_time;
	int port = 0;
	int control;

	RS.Logger.SetLevel(spdlog::level::info);

	/*
	 * Memory debugging if needed.
	 */
#ifdef MALLOC_DEBUG
	malloc_debug(2);
#endif

	/*
	 * Init time.
	 */
	gettimeofday(&now_time, nullptr);
	current_time = (time_t)now_time.tv_sec;
	strcpy(str_boot_time, ctime(&current_time));

	/*
	 * Reserve one channel for our use.
	 */
	if ((fpReserve = fopen(NULL_FILE, "r")) == nullptr)
	{
		RS.Logger.Error("Main: fopen {}: {}", NULL_FILE, std::strerror(errno));
		exit(0);
	}

	/*
	 * Get the port number.
	 */
	port = atoi(RS.SQL.Settings.GetValue("Port").c_str());
	if (argc > 1)
	{
		if (!is_number(argv[1]))
		{
			fprintf(stderr, "Usage: %s [port #]\n", argv[0]);
			exit(0);
		}
		else if ((port = atoi(argv[1])) == 666)
		{
			RS.Logger.SetLevel(spdlog::level::debug);
			bDebug = true;
		}
		else if (port <= 1024)
		{
			fprintf(stderr, "Port number must be above 1024.\n");
			exit(0);
		}
	}

	mPort = port;

	/*
	 * Run the game.
	 */
	control = init_socket(port);

	// boot_db( );
	if (!RS.Bootup())
	{
		RS.Logger.Error("Riftshadow failed to boot, aborting.");
		exit(0);
		return 0;
	}

	RS.Logger.Info("Riftshadow booted, binding on port {}.", port);

	game_loop_unix(control);
	close(control);

	/*
	 * That's all, folks.
	 */
	RS.Logger.Info("Normal termination of game.");
	exit(0);
	return 0;
}
