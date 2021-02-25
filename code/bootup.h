#ifndef BOOTUP_H
#define BOOTUP_H

#include "stdlibs/file.h"
#include "autogen/coldefs.h"

#include <stdlib.h>
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
#include "const.h"

/*
 *  * Local booting procedures.
 *  */
void init_mm (void);
void load_area (FILE *fp);
void new_load_area (FILE *fp);   /* OLC */
void load_mobs (FILE *fp);
void load_objs (FILE *fp);
void load_resets (FILE *fp);
void load_newresets (FILE *fp);
void load_rooms (FILE *fp);
void load_socials (FILE *fp);
void load_bans (void);
void load_votes (void);
void load_cabal_items (void);
void fix_exits (void);
void find_adjacents (void);
void clean_notes (void);
void load_improgs (FILE *fp);
void load_specs (FILE *fp);
void reset_area (AREA_DATA * pArea);

/*
* this macro loads SQL table data into a linked list
* superfast kung fu style!
* -cal 
*/
#define DEFINE_SQLTABLELOAD(classname, funcname, loaddef)						\
void classname::funcname(const char *query)										\
{																				\
	CRow row;																	\
	classname *stackcopy;														\
	int i = 0;																	\
	int nResults = RS.SQL.Select(query), nSize = nResults * sizeof(classname);	\
	classname *lastp = NULL, *memptr = NULL;									\
	while(!RS.SQL.End())														\
	{																			\
		row = RS.SQL.GetRow();													\
		stackcopy = new classname;												\
		if(!classname::first)													\
		{																		\
			classname::first = stackcopy;										\
		}																		\
		else																	\
			((classname *)lastp)->next 	= stackcopy;							\
		stackcopy->index = i++;													\
		loaddef(stackcopy)														\
		stackcopy->next = NULL;													\
		lastp = stackcopy;														\
	}																			\
	RS.SQL.FreeResults();														\
} 

/* associates rooms with their area in an appallingly fast way by avoiding lookups */
#define ASSOCIATE_AREA(troom, aid) troom.SetArea((CArea *)(CArea::first+aid*sizeof(CArea)))

/* allocates exits for rooms */
#define ALLOCATE_EXITS(troom)													\
	int numexits = 0;															\
	for(int jloop = 0; jloop < MAX_EXITS; jloop++)								\
		if(row[jloop*4 + COL_WORLD_ROOMS_0_TO_ROOM][0] != -1)					\
			numexits++;															\
	void *jptr = (void *)new char[numexits * sizeof(CExit)];					\
	for(int jloop = 0; jloop < MAX_EXITS; jloop++)								\
	{																			\
		if(row[jloop*4 + COL_WORLD_ROOMS_0_TO_ROOM][0] == -1)					\
			continue;															\
		CExit *eptr = (CExit *)((char *)jptr + jloop * sizeof(CExit));			\
		LOAD_EXIT_DATA(eptr);													\
		troom.exits[i] = eptr;													\
	}

DEFINE_SQLTABLELOAD(CClass, LoadClassTable, LOAD_CLASS_TABLE)

#endif /* BOOTUP_H */
