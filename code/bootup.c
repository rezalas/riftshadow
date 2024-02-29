#include "autogen/coldefs.h"
#include "stdlibs/file.h"

#include <stdlib.h>
#include <string.h>
#include <fstream>
#include "rift.h"
#include "newmem.h"
#include "area.h"
#include "room.h"
#include "exit.h"
#include "bootup.h"
#include "db.h"
#include "recycle.h"
#include "lookup.h"
#include "tables.h"
#include "update.h"
#include "interp.h"
#include "olc.h"
#include "spec.h"
#include "const.h"
#include "devextra.h"
#include "comm.h"
#include "./stdlibs/cdirectory.h"
#include "./include/fmt/format.h"

CRoom *CRoom::first = nullptr;

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

	RS.Log("Loading object counts off players now...");

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
			RS.Log(strPlr);

		if (!str_cmp(strPlr,chkbuf))
			break;

		fpChar = fopen(strPlr, "r");
		if (fpChar == nullptr)
		{
			perror(strPlr);
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
				auto buffer = fmt::format("Autodeleting {}.", temp_player_name);
				RS.Log(buffer.c_str());
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

	RS.Log("Object Limits loaded");

	/* CABAL LIMITS */
	for(i=1;i<MAX_CABAL;i++)
	{
		tempfloat = num_pfiles / cabal_table[i].max_members;
		cabal_max[i] = (short)tempfloat<=15 ? 15 : (short)tempfloat;
	}

	RS.Log("Cabal Limits loaded");
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

	RS.Log("Loading area files now...");

	if ( ( fpList = fopen( AREA_LIST, "r" ) ) == nullptr )
	{
		perror( AREA_LIST );
		exit( 1 );
	}

	for ( ; ; )
	{
		strcpy( strArea, fread_word( fpList ) );

		if ( strArea[0] == '$' )
			break;

		if (bDebug)
			RS.Log(strArea);

		if ( strArea[0] == '-' )
		{
			fpArea = stdin;
		}
		else
		{
			auto strAreaFullPath = fmt::format("{}/{}", RIFT_AREA_DIR, strArea);
			if ( ( fpArea = fopen(strAreaFullPath.c_str(), "r" ) ) == nullptr )
			{
				perror(strAreaFullPath.c_str());
				exit( 1 );
			}
		}

		for ( ; ; )
		{
			char *word;

			if ( fread_letter( fpArea ) != '#' )
			{
				RS.Bug("Boot_db: # not found.");
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
				RS.Bug("Boot_db: bad section name.");
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
	
	RS.Log("Loading class and race data...");
	CClass::LoadClassTable("* FROM class_table ORDER BY id ASC");
	//CRace::LoadRaceTable("* FROM race_table ORDER BY pcrace DESC, name ASC");
	
	//RS.Log("Loading commands...");
	//CCommand::LoadCmdTable("* FROM interp_table ORDER BY id ASC");

	//end_benchmark("Table initialization")
	CATCH_ERRORS("InitializeTables") //catch any leftovers
}

