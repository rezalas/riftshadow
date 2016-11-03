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
#include "merc.h"
#include "db.h"
#include "recycle.h"
#include "lookup.h"
#include "tables.h"
#include "update.h"
#include "interp.h"

//#define free public_fREe
//tweakable parameters
#define TEMP_STRING_MEM_SIZE	320000
#define TEMP_STRUCT_MEM_SIZE	100000
#define PERM_STRING_LIST_CHUNK	30000
#define PERM_STRING_CHUNK_SIZE	131072
#define PERM_STRING_MAX_SIZE	3500000
#define EMERGENCY_DISABLE_TALLOC FALSE
typedef struct string_entry STRING_ENTRY;
typedef struct mallinfo MALLOC_INFO;
void *init_temp_struct_mem(void);
void *init_temp_string_mem(void);

extern int                     nAllocString; 
extern int                     sAllocString;
extern int                     nAllocPerm;
extern int                     sAllocPerm;

char *talloc_string(const char *str)
{
	static long tStrMem = 0;
	static void *tStrPtr = NULL;
	static void *tStrBase = NULL;
	static int nIterations = 0;
	unsigned int tStrLen;
	char *tStrNew;

	if(!tStrPtr) {
		tStrPtr = init_temp_string_mem();
		tStrBase = tStrPtr;
	}
	if(!tStrPtr)
		return NULL;
	tStrLen = strlen(str) + 1; //+1 for the null we add at the end
	if(tStrLen > TEMP_STRING_MEM_SIZE)
	{
		bug("Error: Temp string too large.",0);
		return NULL;
	}
	if(tStrMem + tStrLen >= TEMP_STRING_MEM_SIZE)
	{	//current loc + new string is over limit, go back around the mulberry bush
		char buf[MSL];
		nIterations++;
		sprintf(buf,"Temporary string allocation space allotted (%d times since boot), returning to origination.",
			nIterations);
		wiznet(buf,NULL,NULL,WIZ_LOG,0,0);
		tStrPtr = tStrBase;
		tStrMem = 0;
	}
//	else if(tStrMem + tStrLen >= TEMP_STRING_WARN_SIZE)
//		wiznet("95% tstring saturation reached.",NULL,NULL,WIZ_DEBUG,0,0);
	tStrNew = (char *)tStrPtr;
	strcpy(tStrNew, str);
	tStrPtr = (void *)((char *)tStrPtr + tStrLen);
	tStrMem = tStrMem + tStrLen;
	return tStrNew;
}

void *talloc_struct(long nStructSize)
{
	static long tStrMem = 0;
	static void *tStrPtr = NULL, *tStrBase;
	char *tStrNew; 
 
	if(!tStrPtr) {
		tStrPtr = init_temp_struct_mem();
		tStrBase = tStrPtr;
	}
	if(!tStrPtr)
		return NULL;
	if(nStructSize > TEMP_STRUCT_MEM_SIZE)
	{
		bug("Error: Temp struct too large.",0);
		return NULL;
	}
	if(tStrMem + nStructSize >= TEMP_STRUCT_MEM_SIZE)
	{       //current loc + new struct is over limit, go back around the mulberry bush
		wiznet("Temporary struct allocation space allotted, returning to origination.",NULL,NULL,WIZ_LOG,0,0);
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
	if(str == NULL)
	{
		bug("Bad code form that'll lead to free_pstr() crashes: attempting to palloc_string a NULL.",0);
                return NULL;
	}
	nAllocString++;
//	sAllocString += slen;
	tstr = new char[slen];
	if(!tstr)
	{
		bug("Unable to allocate pstring.",0);
		return NULL;
	}
	strcpy(tstr,str);
	return tstr;
}
/*
void *palloc_struct(long nStructSize)
{
	if(nStructSize < 1)
	{
		bug("Attempting to palloc_struct of 0 or less size.  Bad idea.",0);
		return NULL;
	}
	nAllocPerm++;
	sAllocPerm += nStructSize;
	return (void *)new char[nStructSize];
}
*/
void free_pstring(char *pstr)
{
//	char buf[MSL];
//	sprintf(buf,"Trying to free %s : %ld.",pstr,(long)pstr);
//	bug(buf,0);
//	if(pstr == NULL)
//		bug("Error: Trying to free null pstring.",0);
	if(pstr == NULL 
         ||   pstr == &str_empty[0]) 
                return;
	nAllocString--;
//	sAllocString -= strlen(pstr);
	delete[] pstr;
}
/*void free_pstruct(void *pstruct)
{
	if(pstruct == NULL)
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
		return NULL;
	for(i=0; i<nStructSize; i++)
		*((char *)nPtr+i) = '\0';
	return nPtr;
}*/

//Internal functions.  DO NOT CALL ANYWHERE ELSE [or die!].

void *init_temp_struct_mem(void)
{
	void *tStrAlloc = (void *)new char [TEMP_STRUCT_MEM_SIZE];
	if(!tStrAlloc)
                bug("Error: Unable to dlmalloc() temporary struct memory.",0);
        return tStrAlloc;
}
void *init_temp_string_mem(void)
{
	void *tStrAlloc = new char[TEMP_STRING_MEM_SIZE];
	if(!tStrAlloc)
		bug("Error: Unable to dlmalloc() temporary string memory.",0);
	return tStrAlloc;
}

char *temp;
extern bool bDebug;
void do_memtest(CHAR_DATA *ch, char *argument)
{
	char buf[MSL];
	CHAR_DATA *qch;
	argument = one_argument(argument, buf);
	CQueue j;
	RS.Queue.AddToQueue(6,2,(void*)do_bash,ch,"Calenduil");
	return;
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
			if(IS_NPC(qch) && qch->pIndexData->vnum == 3001)
				qch->dam_mod = atoi(argument);
		send_to_char("Dammod changed.\n\r",ch);
	}
	if(!str_cmp(buf,"hp"))
	{
		for(qch = char_list; qch; qch = qch->next)
			if(IS_NPC(qch) && qch->pIndexData->vnum == 3001)
				qch->max_hit = UMAX(qch->max_hit - atoi(argument), 100);
		send_to_char("Maxhit changed.\n\r",ch);
	}
	if(!str_cmp(buf,"begin"))
	{
		for(qch = char_list; qch; qch = qch->next)
			if(IS_NPC(qch) && qch->pIndexData->vnum == 3001 && !IS_AFFECTED(qch, AFF_DETECT_MAGIC))
				SET_BIT(qch->affected_by, AFF_DETECT_MAGIC);
		send_to_char("It HAS BEGUN!\n\r",ch);
	}
	if(!str_cmp(buf,"end"))
	{
		for(qch = char_list; qch; qch = qch->next)
			if(IS_NPC(qch) && qch->pIndexData->vnum == 3001 && qch->in_room->vnum > 2399 &&
			   qch->in_room->vnum < 2801)
				extract_char(qch, TRUE);
		send_to_char("It has.. ENDED!\n\r",ch);
	}
}

#undef free
