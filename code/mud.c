#ifdef _WIN32
	#include <windows.h>
	#include <winsock.h>
	#include <winsock2.h>
#else
	#include "socket.h"
#endif
#include "stdlibs/file.h"
//#include "autogen/fundefs.h"
//#include "autogen/coldefs.h"

#include <string.h>
#include "mud.h"
#include "merc.h"
#include "update.h"
#include "dioextra.h"
#include "db.h"
#include "db2.h"
#include "misc.h"
#include "./stdlibs/clogger.h"


CMud RS;

CMud::CMud()
{
	Settings = Config(CONFIG_FILE);
	Logger = CLogger();
	Logger.Initialize();

	if (!Settings.isLoaded())
	{
		Logger.Error("Unable to load configuration!\r\n");
		exit(0);
	}
}

CMud::~CMud()
{
	if(game_up)
		RS.Shutdown();

	Logger.Shutdown();
}

bool CMud::Bootup()
{
	Logger.Info("*** Beginning RIFTSHADOW MUD server ***");

	top_string = nullptr;
	fBootDb = true;

	Logger.Info("Creating persistent SQL connection...");
	DbConnection riftCore = RS.SQL.Settings.GetDbConnection("rift_core");
	if (!RS.SQL.StartSQLServer(riftCore.Host.c_str(),
	riftCore.Db.c_str(), riftCore.User.c_str(), riftCore.Pwd.c_str()))
	{
		Logger.Warn("Failed to create a SQL connection.");
		return false;
	}

	game_up = true;

	Logger.Info("Loading options and greeting screen...");
	RS.LoadGreetingScreen();

	RS.LoadOptions();

	InitializeTables();

	Logger.Info("Initialize random number generator...");
	init_mm();

	Logger.Info("Setting time, weather, and berus/calabren pos...");
	RS.LoadTime();
	
	Logger.Info("Assigning gsns and psns to skills and spells...");
	RS.LoadGsn();
	CProficiencies::AssignPsns();

	Logger.Info("Reading the race info...");
	load_race_info();

	Logger.Info("Sorting area list..");
	sort_areas();

	Logger.Info("Loading area files..");
	RS.LoadAreas();

	Logger.Info("Starting player file parsing..");
	RS.LoadObjLimits();

	fix_exits( );
	Logger.Info("Exits fixed");
	find_adjacents( );
	Logger.Info("Adjacent areas found");
	clean_notes( );
	Logger.Info("Notes cleaned");
	fBootDb= false;
	area_update( );
	Logger.Info("Area update");
	gold_update( );
	Logger.Info("Allocating gold");
	load_cabal_items();
	Logger.Info("Cabal Items loaded");
	weather_update();
	Logger.Info("Priming weather");
	reset_chessboard();
	gold_constant = std::stol(Settings.GetValue("Gold"));
	update_db_gold();

	//CSocket::InitializeSockets();

	/* 
	*      load up the "tables" - linked lists containing most
	*      of the static game data.  Races, classes, lookups, etc etc.
	*/

#ifndef NOLOAD
	/*RS.SQL.IQuery("LOCK TABLES class_table READ, race_table READ, interp_table READ, 
			world_areas READ, world_rooms READ");
	*/	

	/*
	* Load areas, rooms, object templates, and mob templates
	*/
	
	//RS.LoadAreas();

	//RS.SQL.IQuery("UNLOCK TABLES");
#endif
	//RS.GameEngine.GameLoop();
	return true;
}

inline bool CMud::RunGame()
{
	return game_up;
}

void CMud::Shutdown()
{
	game_up= false;
}

bool CMud::IsBanned(int desc, const char *tIP)
{
	return false;
}

char * CMud::GetError()
{
	return "add errno handling";
}

/* the following functions are so incredibly slick, they're illegal in connecticut */

void bug(const char *bugstr, ...)
{
	RS.Bug(bugstr);
}

/// Reports a bug 
/// @param tError: The message to report as a bug.
/// @param  ... - A list of values as related to the format specifiers in tError.
/// @note Format specifiers are in the printf-family style.
void CMud::Bug(const char *tError, ...)
{
	TString bug;
	MUNCH_VARARG(tError, bug) //mmm, chewy.
	fprintf(stderr,"[*****] BUG: %s\n\r", (RSTR)bug);

	std::string str = std::string((RSTR)bug);
	wiznet(str.data(), 0, 0, WIZ_DEBUG, 0, 0);
}
