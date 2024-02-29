#ifndef MUD_H
#define MUD_H

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
#include "characterClasses/class.h"
#include "race.h"
#include "prof.h"
#include "config.h"

#define CONFIG_FILE RIFT_ROOT_DIR "/config.json"

typedef struct char_data CHAR_DATA;
typedef struct obj_data	OBJ_DATA;
extern void wiznet(char *string, CHAR_DATA *ch, OBJ_DATA *obj, long flag, long flag_skip, int min_level);

extern CMud RS;

class CMud
{	
public:
	CMud();
	virtual ~CMud();
//	CGameLoop 			GameEngine;
	CSQLInterface		SQL;
//	CInterpreter		Interpreter;
	CQueue				Queue;
	Config				Settings;
	bool				Bootup();
	void				Shutdown();

	void				LoadGreetingScreen();
	void				LoadOptions();
	void				LoadAreas();
	void				LoadGsn();
	void				LoadObjLimits();
	void				LoadTime();
	void				InitializeTables();		//load up the big old lists of classes, races, etc etc etc
	Config				GetSettings();

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

#endif /* MUD_H */
