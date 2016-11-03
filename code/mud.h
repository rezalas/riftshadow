/*					R I F T S H A D O W
 * 						   M U D
 *
 *		All code contained herein is the exclusive property
 *			of Riftshadow.  Blah blah, legalese for Elad!
 *
 *
 */

#include "rift.h"
#include "gameloop.h"
#include "stdlibs/sql.h"
#include "queue.h"
#include "interp2.h"
#include "skill.h"
#include "class.h"
#include "race.h"
#include "prof.h"

#define OPTION_FILE "../area/rs.conf"

class CMud
{	
public:
	CMud();
	virtual ~CMud();
	
//	CGameLoop 			GameEngine;
	CSQLInterface		SQL;
//	CInterpreter		Interpreter;
	CQueue				Queue;

	void				Bootup();
	void				Shutdown();

	void				LoadGreetingScreen();
	void				LoadOptions();
	void				LoadAreas();
	void				LoadGsn();
	void				LoadObjLimits();
	void				LoadTime();
	void				InitializeTables();		//load up the big old lists of classes, races, etc etc etc

	/* warning, log and debug reporting functions
	 * 0 - errors [bugs] & logged events only
	 * 1 - errors, logged & warnings
	 * 2 - heavy debug info
	 */
	int					GetDebugMode();
	void				SetDebugMode(int fDebugMode = 1);

	void				Bug(const char *tError, ...);
	void				Log(const char *tEvent, ...);
	void				Debug(const char *tDebug, ...);
	void				Wiznet(const char *tText, int iWizType, int iMinLevel = 0, ...);

	char *				GetError();				//returns text of last error	
	/*
	* random mud-wide functions
	*/

	bool				IsBanned(int desc, const char *tIP);
	
	//long				CurrentTime() { return GameEngine.GetTime(); }

	bool				RunGame();	//are we up?

	/*
	 * What follows are THE coolest hacks in all recorded history,
	 * null member classes with [] overloaded so it seems like an array.
	 * 		-Cal
	 */
//	CSkill				Skills;
//	CRace				Races;
//	CClass				Classes;
//	CCommand			Commands;
	/* mud options arei
	 * read off a plaintext - #define OPTION_FILE "rs.conf"
	 */
	
	RString				greeting_screen;
	RString				motd;
	RString				base_directory;
	RString				player_dir;
	
	int					game_port;
	int					build_port;

	int					debug_mode;
private:
	void *				AllocPerm(int nBytes);
	bool				game_up;
};
