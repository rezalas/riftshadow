#include "stdlibs/file.h"
//#include "autogen/fundefs.h"
#include "autogen/coldefs.h"

#include <stdlib.h>
//#include "mud.h"
#include "area.h"
#include "room.h"
#include "exit.h"
#include "merc.h"
#include "db.h"
#include "recycle.h"
#include "lookup.h"
#include "tables.h"
#include "update.h"
#include "interp.h"
#include "olc.h"
#include "spec.h"
#include <string.h>
/*
 *  * Local booting procedures.
 *  */
void    init_mm         args( ( void ) );
void    load_area       args( ( FILE *fp ) );
void    new_load_area   args( ( FILE *fp ) );   /* OLC */
void    load_mobs  args( ( FILE *fp ) );
void    load_objs       args( ( FILE *fp ) );
void    load_resets     args( ( FILE *fp ) );
void    load_newresets    args( ( FILE *fp ) );
void    load_rooms  args( ( FILE *fp ) );
void    load_socials    args( ( FILE *fp ) );
void    load_bans       args( ( void ) );
void    load_votes      args( ( void ) );
void    load_cabal_items        args( ( void ) );
void    fix_exits       args( ( void ) );
void    find_adjacents  args( ( void ) );
void    clean_notes     args( ( void ) );
void    load_improgs    args( ( FILE *fp ) );
void    load_specs      args( ( FILE *fp ) );
void    reset_area      args( ( AREA_DATA * pArea ) );

extern char strArea[MAX_INPUT_LENGTH];
extern FILE *                  fpArea;
CRoom *CRoom::first = NULL;

//Loads all the object limits and related info
void CMud::LoadObjLimits()
{
	FILE *fpChar_list;
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
        sprintf(pbuf,"ls %s%s > %s",PLAYER_DIR,"*.plr",PLAYER_LIST);
        system(pbuf);

        if ((fpChar_list = fopen( PLAYER_LIST, "r")) == NULL)
        {
                perror(PLAYER_LIST);
                exit(1);
        }

        //for(i=0;i<=MAX_TOP_BOUNTY;i++)
        //      top_bounty_value[i]=0;
        sprintf(chkbuf,"%s%s",PLAYER_DIR,"Zzz.plr");
        for (; ;)
        {
                strcpy(strPlr, fread_word(fpChar_list) );
                if(bDebug)
                        log_string(strPlr);

                if (!str_cmp(strPlr,chkbuf))
                        break;

                if ( (  fpChar = fopen(strPlr, "r") ) == NULL)
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
					//NOTE: Be careful that occurances of this word INSIDE role/desc don't fuck it up!
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
				char tbuf[MSL];
				sprintf(tbuf, "Autodeleting %s.", temp_player_name);
				log_string(tbuf);
				breakout = TRUE;
				delete_char(temp_player_name, TRUE);
				break;
			}
                        if ( !str_cmp( word, "O") || !str_cmp( word, "OBJECT"))
                        {
                                word = fread_word(fpChar);

                                if (!str_cmp(word, "Vnum"))
                                {
                                        vnum = fread_number(fpChar);
                                        if ( (get_obj_index(vnum)) != NULL)
                                        {
                                                pObjIndex = get_obj_index(vnum);
                                                pObjIndex->limcount++;
                                        }
                                }
                        }
                }
                fclose(fpChar);
                fpChar = NULL;
        }
        fclose( fpChar_list);
        RS.Log("Object Limits loaded");

        /* CABAL LIMITS */
        for(i=1;i<MAX_CABAL;i++)
        {
                tempfloat = num_pfiles / cabal_table[i].max_members;
                cabal_max[i] = (short)tempfloat<=15 ? 15 : (short)tempfloat;
        }
        RS.Log("Cabal Limits loaded");
}


// Sets all the gsn's for the skills and spells
void CMud::LoadGsn()
{
	int sn;

	for ( sn = 0; sn < MAX_SKILL; sn++ )
		if ( skill_table[sn].pgsn != NULL )
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
		sun = SUN_DARK;
	else if ( time_info.hour <  6 )
		sun = SUN_RISE;
	else if ( time_info.hour < 19 )
		sun = SUN_LIGHT;
	else if ( time_info.hour < 20 )
		sun = SUN_SET;
	else
		sun = SUN_DARK;

	berus_pos = ((current_time-RS_EPOCH)%(BERUS_SPD*360))/BERUS_SPD;
	calabren_pos = ((current_time-RS_EPOCH)%(CALABREN_SPD*360))/CALABREN_SPD;
}

void CMud::LoadAreas()
{
	FILE *fpList;
	//	CArea::LoadAreaData();	//loads the data about each area - name, uid, etc


	log_string("Loading area files now...");

	if ( ( fpList = fopen( AREA_LIST, "r" ) ) == NULL )
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
			log_string(strArea);

		if ( strArea[0] == '-' )
		{
			fpArea = stdin;
		}
		else
		{
			if ( ( fpArea = fopen( strArea, "r" ) ) == NULL )
			{
				perror( strArea );
				exit( 1 );
			}
		}

		for ( ; ; )
		{
			char *word;

			if ( fread_letter( fpArea ) != '#' )
			{
				bug( "Boot_db: # not found.", 0 );
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
				bug( "Boot_db: bad section name.", 0 );
			}
		}

		if ( fpArea != stdin )
			fclose( fpArea );
		fpArea = NULL;
	}
	fclose( fpList );

	fBootDb = FALSE;
	
}

/*
* this macro loads SQL table data into a linked list
* superfast kung fu style!
* -cal 
*/

#define DEFINE_SQLTABLELOAD(classname, funcname, loaddef) \
void classname::funcname(const char *query) \
{ \
	CRow row; \
	classname *stackcopy; \
	int i = 0; \
	int nResults = RS.SQL.Select(query), nSize = nResults * sizeof(classname); \
	classname *lastp = NULL, *memptr = NULL; \
	while(!RS.SQL.End()) \
	{ \
		row = RS.SQL.GetRow(); \
		stackcopy                               = new classname; \
		if(!classname::first) \
		{ \
			classname::first 			= stackcopy; \
		}\
		else \
			((classname *)lastp)->next 	= stackcopy; \
		stackcopy->index						= i++; \
		loaddef(stackcopy) \
		stackcopy->next 						= NULL; \
		lastp 							   		 = stackcopy; \
	} \
	RS.SQL.FreeResults(); \
} 
/* associates rooms with their area in an appallingly fast way by avoiding lookups */
#define ASSOCIATE_AREA(troom, aid)	troom.SetArea((CArea *)(CArea::first+aid*sizeof(CArea)))
/* allocates exits for rooms */
#define ALLOCATE_EXITS(troom)	\
	int numexits = 0; \
	for(int jloop = 0; jloop < MAX_EXITS; jloop++) \
		if(row[jloop*4 + COL_WORLD_ROOMS_0_TO_ROOM][0] != -1) \
			numexits++; \
	void *jptr = (void *)new char[numexits * sizeof(CExit)]; \
	for(int jloop = 0; jloop < MAX_EXITS; jloop++) \
	{\
		if(row[jloop*4 + COL_WORLD_ROOMS_0_TO_ROOM][0] == -1) continue;\
		CExit *eptr = (CExit *)((char *)jptr + jloop * sizeof(CExit)); \
		LOAD_EXIT_DATA(eptr); \
		troom.exits[i] = eptr; \
	}

DEFINE_SQLTABLELOAD(CClass, LoadClassTable, LOAD_CLASS_TABLE)
//DEFINE_SQLTABLELOAD(CRace, LoadRaceTable, LOAD_RACE_TABLE)
//DEFINE_SQLTABLELOAD(CCommand, LoadCmdTable, LOAD_INTERP_TABLE)
//DEFINE_SQLTABLELOAD(CArea, LoadAreaTable, LOAD_WORLD_AREAS)

//DEFINE_SQLTABLELOAD(CRoom, LoadRoomTable, LOAD_WORLD_ROOMS)
	
/* remove this */
//#include <sys/time.h>
void CArea::LoadAreaData()
{
	/* NOTE: class macro weapon is disabled 'cause it's a double for some
	* reason: fix0r */
	RS.Log("Loading area data...");
//	CArea::LoadAreaTable("* FROM world_areas ORDER BY min_vnum ASC");
//	begin_benchmark
//	CRoom::LoadRoomTable("* FROM world_rooms ORDER BY vnum ASC");
//	end_benchmark("Loading rooms");
}

void CMud::LoadOptions()
{
	CFile myFile;
	TString word;
	
	if(myFile.LoadFile(OPTION_FILE) == FALSE)
		return;
	
	for(word = myFile.ReadWord(); !myFile.End(); word = myFile.ReadWord())
	{
		if(word[0] == '#')
		{
			myFile.ReadToEOL();
			continue;
		}
		
		if(word == "game_port")
		{
			game_port = atoi(myFile.ReadWord());
			continue;
		}
		
		if(word == "base_directory")
		{
			base_directory = (const char *)(myFile.ReadWord());
			continue;
		}

		if(word == "player_dir")
		{
			player_dir = (const char *)(myFile.ReadWord());
			continue;
		}

		RS.Bug("LoadOptions Error: Unrecognized option '%s'", (RSTR)word);
	}

	myFile.CloseFile();
}

void CMud::LoadGreetingScreen()
{
	//can't use cfile because of weird \r action
	FILE *fp = fopen(GREET_FILE, "r");
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

