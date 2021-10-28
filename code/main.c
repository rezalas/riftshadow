#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "merc.h"
#include "mud.h"
#include "comm.h"
#include "interp.h"
#include "db.h"

int main(int argc, char **argv)
{
	struct timeval now_time;
	char buf[MSL];
	int port = 0;
	int control;

	/*
	 * Memory debugging if needed.
	 */
#ifdef MALLOC_DEBUG
	malloc_debug(2);
#endif

	/*
	 * Init time.
	 */
	gettimeofday(&now_time, NULL);
	current_time = (time_t)now_time.tv_sec;
	strcpy(str_boot_time, ctime(&current_time));

	/*
	 * Reserve one channel for our use.
	 */
	if ((fpReserve = fopen(NULL_FILE, "r")) == NULL)
	{
		perror(NULL_FILE);
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
	RS.Bootup();

	sprintf(buf, "Riftshadow booted, binding on port %d.", port);
	log_string(buf);

	game_loop_unix(control);
	close(control);

	/*
	 * That's all, folks.
	 */
	log_string("Normal termination of game.");
	exit(0);
	return 0;
}
