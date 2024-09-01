/*

        These are Dev's new, unified memory functions.
        Abandon hope, all ye who enter here, cause this shit is cryptic.

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <ctype.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>
#include "newmem.h"
#include "db.h"
#include "comm.h"
#include "recycle.h"
#include "lookup.h"
#include "tables.h"
#include "update.h"
#include "interp.h"
#include "utility.h"
#include "act_wiz.h"
#include "handler.h"
#include "fight.h"

//#define free public_fREe
//tweakable parameters
#define TEMP_STRING_MEM_SIZE		320000
#define TEMP_STRUCT_MEM_SIZE		100000
#define PERM_STRING_LIST_CHUNK		30000
#define PERM_STRING_CHUNK_SIZE		131072
#define PERM_STRING_MAX_SIZE		3500000
#define EMERGENCY_DISABLE_TALLOC	FALSE

typedef struct string_entry STRING_ENTRY;
typedef struct mallinfo MALLOC_INFO;

char *temp;

char *talloc_string(const char *str)
{
	static long tStrMem = 0;
	static void *tStrPtr = nullptr;
	static void *tStrBase = nullptr;
	static int nIterations = 0;
	unsigned int tStrLen;
	char *tStrNew;

	if(!tStrPtr) {
		tStrPtr = init_temp_string_mem();
		tStrBase = tStrPtr;
	}
	if(!tStrPtr)
		return nullptr;
	tStrLen = strlen(str) + 1; //+1 for the null we add at the end
	if(tStrLen > TEMP_STRING_MEM_SIZE)
	{
		RS.Logger.Warn("Error: Temp string too large.");
		return nullptr;
	}
	if(tStrMem + tStrLen >= TEMP_STRING_MEM_SIZE)
	{	//current loc + new string is over limit, go back around the mulberry bush
		char buf[MSL];
		nIterations++;
		sprintf(buf,"Temporary string allocation space allotted (%d times since boot), returning to origination.",
			nIterations);
		wiznet(buf,nullptr,nullptr,WIZ_LOG,0,0);
		tStrPtr = tStrBase;
		tStrMem = 0;
	}
//	else if(tStrMem + tStrLen >= TEMP_STRING_WARN_SIZE)
//		wiznet("95% tstring saturation reached.",nullptr,nullptr,WIZ_DEBUG,0,0);
	tStrNew = (char *)tStrPtr;
	strcpy(tStrNew, str);
	tStrPtr = (void *)((char *)tStrPtr + tStrLen);
	tStrMem = tStrMem + tStrLen;
	return tStrNew;
}

void *talloc_struct(long nStructSize)
{
	static long tStrMem = 0;
	static void *tStrPtr = nullptr, *tStrBase;
	char *tStrNew; 
 
	if(!tStrPtr) {
		tStrPtr = init_temp_struct_mem();
		tStrBase = tStrPtr;
	}
	if(!tStrPtr)
		return nullptr;
	if(nStructSize > TEMP_STRUCT_MEM_SIZE)
	{
		RS.Logger.Warn("Error: Temp struct too large.");
		return nullptr;
	}
	if(tStrMem + nStructSize >= TEMP_STRUCT_MEM_SIZE)
	{       //current loc + new struct is over limit, go back around the mulberry bush
		wiznet("Temporary struct allocation space allotted, returning to origination.",nullptr,nullptr,WIZ_LOG,0,0);
		tStrPtr = tStrBase;
		tStrMem = 0;
	}
	tStrNew = (char *)tStrPtr;
	tStrPtr = (void *)((char *)tStrPtr + nStructSize);
	tStrMem = tStrMem + nStructSize;
	return tStrNew;
}


char *palloc_string(const char *str)
{
	char *tstr;
	int slen = strlen(str) + 1;
	if(str == nullptr)
	{
		RS.Logger.Warn("Bad code form that'll lead to free_pstr() crashes: attempting to palloc_string a nullptr.");
		return nullptr;
	}
	nAllocString++;
//	sAllocString += slen;
	tstr = new char[slen];
	if(!tstr)
	{
		RS.Logger.Warn("Unable to allocate pstring.");
		return nullptr;
	}
	strcpy(tstr,str);
	return tstr;
}
/*
void *palloc_struct(long nStructSize)
{
	if(nStructSize < 1)
	{
		RS.Logger.Warn("Attempting to palloc_struct of 0 or less size.  Bad idea.");
		return nullptr;
	}
	nAllocPerm++;
	sAllocPerm += nStructSize;
	return (void *)new char[nStructSize];
}
*/
void free_pstring(char *pstr)
{
//	RS.Logger.Info("Trying to free {} : {}.",pstr,(long)pstr);
//	if(pstr == nullptr)
//		RS.Logger.Debug("Error: Trying to free null pstring.");
	if(pstr == nullptr 
         ||   pstr == &str_empty[0]) 
                return;
	nAllocString--;
//	sAllocString -= strlen(pstr);
	delete[] pstr;
}
/*void free_pstruct(void *pstruct)
{
	if(pstruct == nullptr)
		return;
	nAllocPerm--;
	sAllocPerm -= sizeof(pstruct);
	delete pstruct;
}
void *calloc_struct(long nStructSize)
{
	void *nPtr;
	int i;
	nPtr = palloc_struct(nStructSize);
	if(!nPtr)
		return nullptr;
	for(i=0; i<nStructSize; i++)
		*((char *)nPtr+i) = '\0';
	return nPtr;
}*/

//Internal functions.  DO NOT CALL ANYWHERE ELSE [or die!].

void *init_temp_struct_mem(void)
{
	void *tStrAlloc = (void *)new char [TEMP_STRUCT_MEM_SIZE];
	if(!tStrAlloc)
		RS.Logger.Warn("Error: Unable to dlmalloc() temporary struct memory.");
	return tStrAlloc;
}
void *init_temp_string_mem(void)
{
	void *tStrAlloc = new char[TEMP_STRING_MEM_SIZE];
	if(!tStrAlloc)
		RS.Logger.Warn("Error: Unable to dlmalloc() temporary string memory.");
	return tStrAlloc;
}

void do_memtest(CHAR_DATA *ch, char *argument)
{
	char buf[MSL];
	CHAR_DATA *qch;
	argument = one_argument(argument, buf);
	RS.Queue.AddToNewQueue(6, do_bash_queue, ch, "Calenduil");
	return;

	//TODO: what the what???

	if(buf[0]=='\0' || !argument || !is_number(argument))
		return;
	if(number_percent()> (.2 * get_skill(ch,gsn_rage)))
	{
		sprintf(buf,"%f is the value.\n\r",.2 * get_skill(ch,gsn_rage));
		send_to_char(buf,ch);
	}
	return;
	if(!str_cmp(buf,"dammod"))
	{
		for(qch = char_list; qch; qch = qch->next)
			if(is_npc(qch) && qch->pIndexData->vnum == 3001)
				qch->dam_mod = atoi(argument);
		send_to_char("Dammod changed.\n\r",ch);
	}
	if(!str_cmp(buf,"hp"))
	{
		for(qch = char_list; qch; qch = qch->next)
			if(is_npc(qch) && qch->pIndexData->vnum == 3001)
				qch->max_hit = std::max(qch->max_hit - atoi(argument), 100);
		send_to_char("Maxhit changed.\n\r",ch);
	}
	if(!str_cmp(buf,"begin"))
	{
		for(qch = char_list; qch; qch = qch->next)
			if(is_npc(qch) && qch->pIndexData->vnum == 3001 && !is_affected_by(qch, AFF_DETECT_MAGIC))
				SET_BIT(qch->affected_by, AFF_DETECT_MAGIC);
		send_to_char("It HAS BEGUN!\n\r",ch);
	}
	if(!str_cmp(buf,"end"))
	{
		for(qch = char_list; qch; qch = qch->next)
			if(is_npc(qch) && qch->pIndexData->vnum == 3001 && qch->in_room->vnum > 2399 &&
			   qch->in_room->vnum < 2801)
				extract_char(qch, true);
		send_to_char("It has.. ENDED!\n\r",ch);
	}
}

#undef free
