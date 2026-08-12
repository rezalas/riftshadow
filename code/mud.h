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

#include <string>

#include "rift.h"
#include "gameloop.h"
#include "stdlibs/clogger.h"
#include "stdlibs/dbsession.h"
#include "queue.h"
#include "characterClasses/class.h"
#include "prof.h"
#include "config.h"

#define CONFIG_FILE rift_path("/config.json")

#include "entity/fwd.h"

extern void wiznet(char *string, CHAR_DATA *ch, OBJ_DATA *obj, long flag, long flag_skip, int min_level);

extern CMud RS;

//
// The MUD itself: the connections it owns, the services it runs on, and the
// state that outlives any one player.
//
// Command dispatch is deliberately not a member.  Commands live in a const
// table of free functions in interp.c, looked up by name, and a handler is an
// entry point rather than behaviour that belongs to this object.  Wrapping
// that table in a class here would add an owner without adding an invariant.
//
class CMud
{
public:
	CMud();
	virtual ~CMud();
	CGameLoop			GameEngine;
	DbSession			Db;		// `rift_core` database connection
	DbSession			DbRift;	// `rift` database connection
	CLogger				Logger;
//	CInterpreter		Interpreter;
	CQueue				Queue;
	Config				Settings;
	bool				Initialize();
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

	void				Wiznet(const char *tText, int iWizType, int iMinLevel = 0, ...);

	char *				GetError();				//returns text of last error	
	/*
	* random mud-wide functions
	*/

	bool				IsBanned(int desc, const char *tIP);

	bool				RunGame();	//are we up?

	std::string			greeting_screen;
	std::string			motd;
	std::string			base_directory;
	std::string			player_dir;
	
	int					game_port;
	int					build_port;

	int					debug_mode;
private:
	bool				game_up;
};

#endif /* MUD_H */
