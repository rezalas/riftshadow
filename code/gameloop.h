#ifndef GAMELOOP_H
#define GAMELOOP_H

#include <sys/select.h>
#include <sys/time.h>

//
// The main game loop.  It owns the listening socket, the descriptor poll and
// the pulse clock, and it runs until merc_down is set.
//
// The pulse is the unit everything else is measured in.  One pass of GameLoop
// polls every descriptor, accepts a new connection, reads and interprets one
// command per player, advances the world by one pulse and writes the output
// back out, then sleeps for whatever is left of 1/PULSE_PER_SECOND.  The sleep
// is what makes the pulse a real time interval rather than a busy spin, so a
// step that runs long shortens the sleep rather than delaying the next pulse.
//
class CGameLoop
{
public:
	CGameLoop();
	~CGameLoop();

	void BeginGameLoop(int controlSocket);
	void GameLoop();
	void EndGameLoop();

private:
	void PollDescriptors();
	void DropExceptionalDescriptors();
	void ProcessInput();
	void ProcessOutput();
	void SleepUntilSynchronized();

	// The poll sets are rebuilt from scratch every pulse.  They are members
	// rather than locals only so the steps above can share one pulse's answer.
	fd_set in_set;
	fd_set out_set;
	fd_set exc_set;

	int control;
	timeval last_time;
};

#endif /* GAMELOOP_H */
