#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <cerrno>
#include <cstring>
#include <algorithm>

#include "merc.h"
#include "entity/handles.h"
#include "entity/list_cursor.h"
#include "gameloop.h"
#include "mud.h"
#include "comm.h"
#include "olc.h"
#include "interp.h"
#include "alias.h"
#include "save.h"
#include "editor.h"
#include "update.h"

CGameLoop::CGameLoop()
{
	control = -1;
	last_time.tv_sec = 0;
	last_time.tv_usec = 0;

	FD_ZERO(&in_set);
	FD_ZERO(&out_set);
	FD_ZERO(&exc_set);
}

CGameLoop::~CGameLoop()
{
}

void CGameLoop::BeginGameLoop(int controlSocket)
{
	control = controlSocket;

	// A player who disconnects mid-write would otherwise take the server down
	// with SIGPIPE.  The write paths check for the error themselves.
	signal(SIGPIPE, SIG_IGN);

	gettimeofday(&last_time, nullptr);
	current_time = (time_t)last_time.tv_sec;
}

void CGameLoop::GameLoop()
{
	while (!merc_down)
	{
#ifdef MALLOC_DEBUG
		if (malloc_verify() != 1)
			abort();
#endif

		PollDescriptors();

		/*
		 * New connection?
		 */
		if (FD_ISSET(control, &in_set))
			init_descriptor(control);

		DropExceptionalDescriptors();
		ProcessInput();

		/*
		 * Autonomous game motion.
		 */
		update_handler();

		ProcessOutput();
		SleepUntilSynchronized();

		gettimeofday(&last_time, nullptr);
		current_time = (time_t)last_time.tv_sec;
	}
}

void CGameLoop::EndGameLoop()
{
	if (control >= 0)
		close(control);

	control = -1;
}

void CGameLoop::PollDescriptors()
{
	// select() may write to its timeout, so this starts at zero every pulse
	// rather than being carried across them.  Zero means poll and return.
	timeval null_time;
	int maxdesc;

	null_time.tv_sec = 0;
	null_time.tv_usec = 0;

	FD_ZERO(&in_set);
	FD_ZERO(&out_set);
	FD_ZERO(&exc_set);
	FD_SET(control, &in_set);

	maxdesc = control;

	for (auto &owned : descriptor_list)
	{
		// Nothing here can close a connection, so a plain walk is enough.
		maxdesc = std::max(maxdesc, (int)owned->descriptor);
		FD_SET(owned->descriptor, &in_set);
		FD_SET(owned->descriptor, &out_set);
		FD_SET(owned->descriptor, &exc_set);
	}

	if (select(maxdesc + 1, &in_set, &out_set, &exc_set, &null_time) < 0)
	{
		RS.Logger.Error("Game_loop: select poll: {}", std::strerror(errno));
		exit(0);
	}
}

void CGameLoop::DropExceptionalDescriptors()
{
	/*
	 * Kick out the freaky folks.
	 */
	for (OwningListWalk<DESCRIPTOR_DATA> walk(descriptor_list); !walk.Done(); walk.Step())
	{
		DESCRIPTOR_DATA *d = walk.Current();

		if (FD_ISSET(d->descriptor, &exc_set))
		{
			FD_CLR(d->descriptor, &in_set);
			FD_CLR(d->descriptor, &out_set);

			CHAR_DATA *player = Deref(d->character);

			if (player && player->level > 1)
				save_char_obj(player);

			d->outtop = 0;
			close_socket(d);
		}
	}
}

void CGameLoop::ProcessInput()
{
	for (OwningListWalk<DESCRIPTOR_DATA> walk(descriptor_list); !walk.Done(); walk.Step())
	{
		DESCRIPTOR_DATA *d = walk.Current();

		d->fcommand= false;

		// Nothing between here and the interpret() below can free the
		// character: read_from_descriptor only touches the socket, and the
		// branch that does close the connection continues out of the loop.
		CHAR_DATA *player = Deref(d->character);

		if (FD_ISSET(d->descriptor, &in_set))
		{
			if (player != nullptr)
				player->timer = 0;

			if (!read_from_descriptor(d))
			{
				FD_CLR(d->descriptor, &out_set);

				if (player != nullptr && player->level > 1)
					save_char_obj(player);

				d->outtop = 0;
				close_socket(d);
				continue;
			}
		}

		if (player != nullptr && player->wait > 0)
			--player->wait;

		if (player != nullptr && player->wait <= 0 && player->pcdata->pending)
		{
			int i = 0;

			interpret(player, player->pcdata->queue[0]);

			// The queued command can be "quit", which extracts the
			// character out from under us, so read the connection again
			// here rather than before the call. Everything below this point
			// only shuffles the command queue and cannot free anything, so
			// the one read covers all of it.
			player = Deref(d->character);

			if (!player)
				continue;

			for (i = 1; i < player->pcdata->write_next; i++)
			{
				if (i > MAX_QUEUE - 1) break;
				strcpy(player->pcdata->queue[(i - 1)], player->pcdata->queue[i]);
			}

			player->pcdata->write_next--;

			for (i = player->pcdata->write_next; i < MAX_QUEUE; i++)
			{
				player->pcdata->queue[i][0] = '\0';
			}

			if (player->pcdata->write_next == 0)
				player->pcdata->pending = false;

			continue;
		}

		read_from_buffer(d);

		if (d->incomm[0] != '\0')
		{
			d->fcommand = true;
			stop_idling(player);

			// stop_idling moves the character back out of limbo and fires a
			// room act, so read the connection again rather than carrying
			// the value above across it.
			player = Deref(d->character);

			/* OLC */
			if (d->showstr_point)
			{
				show_string(d, d->incomm);
			}
			else if (d->pString)
			{
				string_add(player, d->incomm);
			}
			else if (d->connected == CON_PLAYING && player->pcdata && player->pcdata->entering_text)
			{
				process_text(player, d->incomm);
			}
			else
			{
				switch (d->connected)
				{
					case CON_PLAYING:
						if (!run_olc_editor(d))
							substitute_alias(d, d->incomm);
						break;
					default:
						nanny(d, d->incomm);
						break;
				}
			}

			d->incomm[0] = '\0';
		}
	}
}

void CGameLoop::ProcessOutput()
{
	for (OwningListWalk<DESCRIPTOR_DATA> walk(descriptor_list); !walk.Done(); walk.Step())
	{
		DESCRIPTOR_DATA *d = walk.Current();

		if ((d->fcommand || d->outtop > 0) && FD_ISSET(d->descriptor, &out_set))
		{
			if (!process_output(d, true))
			{
				// Read after process_output, not before: its write path can
				// overflow the output buffer and close the connection.
				CHAR_DATA *player = Deref(d->character);

				if (player != nullptr && player->level > 1)
					save_char_obj(player);

				d->outtop = 0;
				close_socket(d);
			}
		}
	}
}

void CGameLoop::SleepUntilSynchronized()
{
	/*
	 * Synchronize to a clock.
	 * Sleep( last_time + 1/PULSE_PER_SECOND - now ).
	 * Careful here of signed versus unsigned arithmetic.
	 */
	struct timeval now_time;
	long secDelta;
	long usecDelta;

	gettimeofday(&now_time, nullptr);

	usecDelta = ((int)last_time.tv_usec) - ((int)now_time.tv_usec) + 1000000 / PULSE_PER_SECOND;
	secDelta = ((int)last_time.tv_sec) - ((int)now_time.tv_sec);

	while (usecDelta < 0)
	{
		usecDelta += 1000000;
		secDelta -= 1;
	}

	while (usecDelta >= 1000000)
	{
		usecDelta -= 1000000;
		secDelta += 1;
	}

	if (secDelta > 0 || (secDelta == 0 && usecDelta > 0))
	{
		struct timeval stall_time;

		stall_time.tv_usec = usecDelta;
		stall_time.tv_sec = secDelta;

		if (select(0, nullptr, nullptr, nullptr, &stall_time) < 0)
		{
			RS.Logger.Error("Game_loop: select stall: {}", std::strerror(errno));
			exit(1);
		}
	}
}
