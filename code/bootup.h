#ifndef BOOTUP_H
#define BOOTUP_H

#include "autogen/coldefs.h"
#include "merc.h"

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
	classname *lastp = nullptr, *memptr = nullptr;								\
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
		stackcopy->next = nullptr;												\
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
