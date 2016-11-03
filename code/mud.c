#include "socket.h"
#include "stdlibs/file.h"
//#include "autogen/fundefs.h"
//#include "autogen/coldefs.h"

#include <string.h>
//#include "mud.h"
#include "merc.h"
#include "update.h"
extern FILE *                  fpArea;
extern char                    strArea[MAX_INPUT_LENGTH];
extern char *			top_string;
extern bool			fBootDb;
extern char * help_greeting;


void    init_mm         args( ( void ) );
void sort_areas(void);
void load_race_info(void);
void    fix_exits       args( ( void ) );
void    find_adjacents  args( ( void ) );
void    clean_notes     args( ( void ) );
void    load_improgs    args( ( FILE *fp ) );
void    load_specs      args( ( FILE *fp ) );
void    reset_area      args( ( AREA_DATA * pArea ) );
long    process_bounty  args( ( char *name, long bounty));
void    load_socials    args( ( FILE *fp ) );
void    load_votes      args( ( void ) );
void    load_cabal_items        args( ( void ) );
void reset_chessboard ( void );

CMud RS;

CMud::CMud()
{
}

CMud::~CMud()
{
	if(game_up)
		RS.Shutdown();
}

void CMud::Bootup()
{
	FILE *fp;
	char tempbuf[MSL], buf[MSL];
		RS.Log("\n\r*** Beginning RIFTSHADOW MUD server ***");
		
		top_string = NULL;
		fBootDb = TRUE;

		RS.Log("Creating persistent SQL connection...");
		RS.SQL.StartSQLServer();
		
		game_up = TRUE;
		
		RS.Log("Loading options and greeting screen...");
		RS.LoadGreetingScreen();
		
		RS.LoadOptions();

		InitializeTables();
		
		RS.Log("Initializing host sockets...");

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
		fBootDb = FALSE;
		area_update( );
		RS.Log("Area update");
		gold_update( );
		RS.Log("Allocating gold");
		load_cabal_items();
		RS.Log("Cabal Items loaded");
		weather_update();
		RS.Log("Priming weather");
		reset_chessboard();
		fp = fopen("login.txt", "r");
		while(fgets(tempbuf,200,fp))
		{
			strcat(buf,tempbuf);
			strcat(buf,"\r");
		}
		fclose(fp);
		fp = fopen("gold.txt", "r");
		gold_constant = fread_number(fp);
		fclose(fp);
		chop(buf);
		chop(buf);
		help_greeting = palloc_string(buf);
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
}

inline bool CMud::RunGame()
{
	return game_up;
}

void CMud::Shutdown()
{
	game_up = FALSE;
}

bool CMud::IsBanned(int desc, const char *tIP)
{
	return FALSE;
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
	MUNCH_VARARG(tError, bug); //mmm, chewy.
	fprintf(stderr,"%s", (RSTR)bug);
}

void CMud::Debug(const char *tDebug, ...)
{
	return;
	TString debug;
	MUNCH_VARARG(tDebug, (RSTR)debug); //burp	
}

void CMud::Log(const char *tEvent, ...)
{
	TString log;
	MUNCH_VARARG(tEvent, log); //varargs are always better with a little gray poupon
	fprintf(stderr,"%s\n\r", (RSTR)log); //temporary
}
