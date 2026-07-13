#ifdef _WIN32
	#include <windows.h>
	#include <winsock.h>
	#include <winsock2.h>
#else
	#include "socket.h"
#endif
#include "stdlibs/file.h"
//#include "autogen/fundefs.h"

#include <stdlib.h>
#include <string.h>
#include <fstream>
#include "autogen/coldefs.h"
#include "mud.h"
#include "merc.h"
#include "rift.h"
#include "newmem.h"
#include "area.h"
#include "room.h"
#include "exit.h"
#include "bootup.h"
#include "update.h"
#include "dioextra.h"
#include "db.h"
#include "db2.h"
#include "misc.h"
#include "recycle.h"
#include "lookup.h"
#include "tables.h"
#include "interp.h"
#include "olc.h"
#include "spec.h"
#include "const.h"
#include "devextra.h"
#include "comm.h"
#include "./stdlibs/cdirectory.h"
#include "./stdlibs/clogger.h"
#include "./include/spdlog/fmt/bundled/format.h"


CMud RS;

CRoom *CRoom::first = nullptr;

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

	// NOTE: Do not call Logger.Shutdown() (spdlog::shutdown()) here. 
	// It causes a use after free error due to internal teardown order. 
	// In other words, the spdlog singleton was that backs the logger is
	// destroyed before this destructor runs, so calling shutdown() here
	// writes into already-freed memory. 
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
		Logger.Error("Failed to create a SQL connection.");
		return false;
	}

	// Setup connection to the `rift_core` database.
	if (!RS.Db.Connect(riftCore))
	{
		Logger.Error("Failed to create the DbSession connection to the rift_core database.");
		return false;
	}

	// Setup connection to the `rift` database.
	DbConnection rift = RS.SQL.Settings.GetDbConnection("rift");
	if (!RS.DbRift.Connect(rift))
	{
		Logger.Error("Failed to create the DbSession connection to the rift database.");
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
	//TODO: The linker errors out if this is deleted. Need to find what references this.
}

//Loads all the object limits and related info
void CMud::LoadObjLimits()
{
	FILE *fpChar;
	char strPlr[MAX_INPUT_LENGTH];
	char chkbuf[MAX_STRING_LENGTH];
	char temp_player_name[MSL];
	char pbuf[100];
	int i;
	long temp_bounty;
	long min_bounty = 0;
	float tempfloat;

	Logger.Info("Loading object counts off players now...");

	auto dir = CDirectory(RIFT_PLAYER_DIR);
	auto files = dir.GetFiles(".plr");

	std::ofstream player_list(PLAYER_LIST, std::ios::trunc);
	for(auto file : files) player_list << file << std::endl;
	player_list.close();

	//for(i=0;i<=MAX_TOP_BOUNTY;i++)
	//      top_bounty_value[i]=0;
	sprintf(chkbuf,"%s/%s", RIFT_PLAYER_DIR,"Zzz.plr");
	for (auto file : files)
	{
		strcpy(strPlr, file.data());
		if(bDebug)
			Logger.Debug(strPlr);

		if (!str_cmp(strPlr,chkbuf))
			break;

		fpChar = fopen(strPlr, "r");
		if (fpChar == nullptr)
		{
			RS.Logger.Error("LoadObjLimits: fopen {}: {}", strPlr, std::strerror(errno));
			exit(1);
		}

		num_pfiles++;
		temp_bounty=0;

		for (; ;)
		{
			int vnum, lastlogin = 0, plevel = 0;
			bool breakout;
			char letter;
			char *word;
			OBJ_INDEX_DATA *pObjIndex;

			temp_player_name[0] = '\0';
			letter = fread_letter(fpChar);

			if (letter != '#')
				continue;

			word = fread_word(fpChar);

			if (!str_cmp(word,"End"))
				break;

			if ( !str_cmp( word, "PLAYER"))
			{
				for ( ; ; )
				{
					word   = fread_word( fpChar );
					//NOTE: Be careful that occurrences of this word INSIDE role/desc don't fuck it up!
					//read before/after role/desc and test accordingly
					if(!str_cmp(word,"Name") && temp_player_name[0] == '\0')
						sprintf(temp_player_name,"%s",fread_string( fpChar ));
					if(!str_cmp(word,"Cabal"))
						cabal_members[cabal_lookup(fread_string(fpChar))]++;
					if(!str_cmp(word,"LogO") && lastlogin == 0)
						lastlogin = fread_number(fpChar);
					if(!str_cmp(word,"Levl"))
						plevel = fread_number(fpChar);
					if(!str_cmp(word,"End"))
						break;
				}
			}

			if(lastlogin && plevel && plevel < 52 && lastlogin + 3456000 < current_time)
			{
				Logger.Info("Autodeleting {}.", temp_player_name);
				breakout = true;
				delete_char(temp_player_name, true);
				break;
			}

			if ( !str_cmp( word, "O") || !str_cmp( word, "OBJECT"))
			{
				word = fread_word(fpChar);

				if (!str_cmp(word, "Vnum"))
				{
					vnum = fread_number(fpChar);
					if ( (get_obj_index(vnum)) != nullptr)
					{
						pObjIndex = get_obj_index(vnum);
						pObjIndex->limcount++;
					}
				}
			}
		}

		fclose(fpChar);
		fpChar = nullptr;
	}

	Logger.Info("Object Limits loaded");

	/* CABAL LIMITS */
	for(i=1;i<MAX_CABAL;i++)
	{
		tempfloat = num_pfiles / cabal_table[i].max_members;
		cabal_max[i] = (short)tempfloat<=15 ? 15 : (short)tempfloat;
	}

	Logger.Info("Cabal Limits loaded");
}


///
/// Sets all the gsn's for the skills and spells
///
void CMud::LoadGsn()
{
	int sn;

	for ( sn = 0; sn < MAX_SKILL; sn++ )
		if ( skill_table[sn].pgsn != nullptr )
			*skill_table[sn].pgsn = sn;

}


// Sets the time and weather
void CMud::LoadTime()
{
	long lhour;

	lhour = (current_time - 650336715) / (PULSE_TICK / PULSE_PER_SECOND);
	time_info.hour = ((current_time - RS_EPOCH) % (60 * 24)) / (60);
	time_info.day = ((current_time - RS_EPOCH) % (60 * 60 * 12)) / (60 * 24);
	time_info.month = ((current_time - RS_EPOCH) % (60 * 60 * 24 * 6)) / (60 * 60 * 12);
	time_info.year  = 1515 + (current_time - RS_EPOCH) / (60 * 60 * 24 * 6);
	time_info.season= ((time_info.month + 1) % 12) / 3;

	if ( time_info.hour <  5 )
		sun = SolarPosition::Dark;
	else if ( time_info.hour <  6 )
		sun = SolarPosition::Sunrise;
	else if ( time_info.hour < 19 )
		sun = SolarPosition::Daylight;
	else if ( time_info.hour < 20 )
		sun = SolarPosition::Sunset;
	else
		sun = SolarPosition::Dark;

	berus_pos = ((current_time-RS_EPOCH)%(LunarSpeed::Berus*360))/LunarSpeed::Berus;
	calabren_pos = ((current_time-RS_EPOCH)%(LunarSpeed::Calabren*360))/LunarSpeed::Calabren;
}

///
///
/// Loads all areas from the area index file declared as  @see AREA_LIST
///
/// @param someparam description
///
void CMud::LoadAreas()
{
	FILE *fpList;

	Logger.Info("Loading area files now...");

	if ( ( fpList = fopen( AREA_LIST, "r" ) ) == nullptr )
	{
		RS.Logger.Error("LoadAreas: fopen {}: {}", AREA_LIST, std::strerror(errno));
		exit( 1 );
	}

	for ( ; ; )
	{
		strcpy( strArea, fread_word( fpList ) );

		if ( strArea[0] == '$' )
			break;

		if (bDebug)
			Logger.Debug(strArea);

		if ( strArea[0] == '-' )
		{
			fpArea = stdin;
		}
		else
		{
			auto strAreaFullPath = fmt::format("{}/{}", RIFT_AREA_DIR, strArea);
			if ( ( fpArea = fopen(strAreaFullPath.c_str(), "r" ) ) == nullptr )
			{
				RS.Logger.Error("LoadAreas: fopen {}: {}", strAreaFullPath, std::strerror(errno));
				exit( 1 );
			}
		}

		for ( ; ; )
		{
			char *word;

			if ( fread_letter( fpArea ) != '#' )
			{
				RS.Logger.Error("Boot_db: # not found.");
				exit( 1 );
			}

			word = fread_word( fpArea );
			if ( word[0] == '$'               ) break;
			else if ( !str_cmp( word, "AREA"     ) ) load_area    (fpArea);
			else if ( !str_cmp( word, "AREADATA" ) ) new_load_area(fpArea); /* OLC */
			else if ( !str_cmp( word, "MOBS"         ) ) load_mobs (fpArea);
			else if ( !str_cmp( word, "OBJS"     ) ) load_objs(fpArea);
			else if ( !str_cmp( word, "OLDRESETS"   ) ) load_resets  (fpArea);
			else if ( !str_cmp( word, "RESETS") ) load_newresets(fpArea);
			else if ( !str_cmp( word, "ROOMS" ) ) load_rooms(fpArea);
			else if ( !str_cmp( word, "SOCIALS"  ) ) load_socials (fpArea);
			else if ( !str_cmp( word, "IMPROGS" ) ) load_improgs(fpArea);
			else if ( !str_cmp( word, "SPECS" ) ) load_specs(fpArea);
			else
			{
				RS.Logger.Warn("Boot_db: bad section name.");
			}
		}

		if ( fpArea != stdin )
			fclose( fpArea );
		fpArea = nullptr;
	}
	fclose( fpList );

	fBootDb = false;

}

void CMud::LoadOptions()
{
	game_port = stoi(Settings.GetValue("Port"));
	base_directory = (Settings.GetValue("BaseDir").c_str());
	player_dir = (Settings.GetValue("PlayerDir").c_str());
}

void CMud::LoadGreetingScreen()
{
	//can't use cfile because of weird \r action
	FILE *fp = fopen(LOGIN_BANNER_FILE, "r");
	if (fp == nullptr)
	{
		RS.Logger.Warn("Unable to open banner file: fopen {}: {}", LOGIN_BANNER_FILE, std::strerror(errno));
		return;
	}

	char tempbuf[210], buf[4096];
	int i;
	buf[0] = '\0';
	while(fgets(tempbuf,200,fp))
 	{
		strcat(buf,tempbuf);
		strcat(buf,"\r");
	}
	for(i=0; buf[i] != '\0'; i++)
		;
	buf[i-2] = '\0';
   	fclose(fp);
	greeting_screen = buf;
	help_greeting = palloc_string(buf);
}

void CMud::InitializeTables()
{
	ERROR_TRAP() //do this, or T will pity you

	//begin_benchmark

	Logger.Info("Loading class and race data...");
	CClass::LoadClassTable("* FROM class_table ORDER BY id ASC");
	//CRace::LoadRaceTable("* FROM race_table ORDER BY pcrace DESC, name ASC");

	//Logger.Info("Loading commands...");
	//CCommand::LoadCmdTable("* FROM interp_table ORDER BY id ASC");

	//end_benchmark("Table initialization")
	CATCH_ERRORS("InitializeTables") //catch any leftovers
}
