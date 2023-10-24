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


CMud RS;

CMud::CMud()
{
	Settings = Config(CONFIG_FILE);
	
	if (!Settings.isLoaded())
	{
		RS.Bug("Unable to load configuration!\r\n");
		exit(0);
	}		
}

CMud::~CMud()
{
	if(game_up)
		RS.Shutdown();
}

bool CMud::Bootup()
{
	FILE *fp;
	char tempbuf[MSL], buf[MSL];
		RS.Log("\n\r*** Beginning RIFTSHADOW MUD server ***");
		
		top_string = nullptr;
		fBootDb = true;

		RS.Log("Creating persistent SQL connection...");
		DbConnection riftCore = RS.SQL.Settings.GetDbConnection("rift_core");
		if (!RS.SQL.StartSQLServer(riftCore.Host.c_str(),
		riftCore.Db.c_str(), riftCore.User.c_str(), riftCore.Pwd.c_str()))
		{
			RS.Log("Failed to create a SQL connection.");
			return false;
		}
		
		game_up = true;
		
		RS.Log("Loading options and greeting screen...");
		RS.LoadGreetingScreen();
		
		RS.LoadOptions();

		InitializeTables();

        RS.Log("Initialize random number generator...");
        init_mm();
				
		RS.Log("Setting time, weather, and berus/calabren pos...");
		RS.LoadTime();
		
		RS.Log("Assigning gsns and psns to skills and spells...");
		RS.LoadGsn();
		CProficiencies::AssignPsns();

		RS.Log("Reading the race info...");
		load_race_info();

		RS.Log("Sorting area list..");
		sort_areas();

		RS.Log("Loading area files..");
		RS.LoadAreas();

		RS.Log("Starting player file parsing..");
		RS.LoadObjLimits();
		

		fix_exits( );
		RS.Log("Exits fixed");
		find_adjacents( );
		RS.Log("Adjacent areas found");
		clean_notes( );
		RS.Log("Notes cleaned");
		fBootDb= false;
		area_update( );
		RS.Log("Area update");
		gold_update( );
		RS.Log("Allocating gold");
		load_cabal_items();
		RS.Log("Cabal Items loaded");
		weather_update();
		RS.Log("Priming weather");
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

void CMud::Bug(const char *tError, ...)
{
	TString bug;
	MUNCH_VARARG(tError, bug) //mmm, chewy.
	fprintf(stderr,"%s", (RSTR)bug);
}

void CMud::Debug(const char *tDebug, ...)
{
	return;
//TODO: why are these instructions after the return?
//	TString debug;
//	MUNCH_VARARG(tDebug, (RSTR)debug) //burp	
}

void CMud::Log(const char *tEvent, ...)
{
	TString log;
	MUNCH_VARARG(tEvent, log) //varargs are always better with a little gray poupon
	fprintf(stderr,"%s\n\r", (RSTR)log); //temporary
}
