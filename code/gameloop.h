#include "rift.h"

#define PULSE_PER_SECOND 4

#define ONCE_PER_SECOND 	if(mtime % 4 == 0)

class CGameLoop
{
friend class CMud;
public:
	CGameLoop();
	~CGameLoop();

	void BeginGameLoop();
	void GameLoop();
	void EndGameLoop();

	int	 GetTime() { return ctime; }
private:
	void SleepUntilSynchronized();
	unsigned long ctime;
};
