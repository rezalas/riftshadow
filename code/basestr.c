#include "stdlibs/strings.h"

#include <string.h>

#include <stdio.h>
#include <stdlib.h>

//#define RSTRDEBUG
#define UPCHAR(c)       ((c) >= 'a' && (c) <= 'z' ? (c) + 'A' - 'a' : (c))

CBaseStr::CBaseStr(void)
{
}

CBaseStr::~CBaseStr(void)
{
}

char * CBaseStr::GetBuffer(void)
{
	if(RefCount() > 0)
	{
		CloneString();
		return myString;
	}
	return myString; //are we sure this is safe?
}

void CBaseStr::SetBuffer(const char *nbuffer)
{
	SafeDealloc();
	
	AllocString(nbuffer);	
	return;
}

void CBaseStr::NoDeallocSetTo(const char *str)
{
	AllocString(str);
}

char * CBaseStr::LowLevelAccess()
{
	return myString;
}

/*
bool CBaseStr::operator ==(const char *str)
{
	return Matches(str, DEFAULTCASE);
}

bool CBaseStr::operator !=(const char *str)
{
	return !Matches(str, DEFAULTCASE);
}
*/
void CBaseStr::Clone(CBaseStr& clone_to)
{
	clone_to.SafeDealloc();
	if(!myString)
		return (void)(clone_to.myString = NULL);
	
	if(RefCount() > 250)
		CloneString();

	SetRefCount(RefCount() == 0 ? 2 : RefCount() + 1);
	clone_to.myString = myString;
	return;
}

void CBaseStr::CopyFrom(CBaseStr &src)
{
	SafeDealloc();

	AllocString(src.myString);
	
	return;
}

inline int CBaseStr::StrLen(void)
{
	char *i;
	if(!myString)
		return 0;
	for(i = myString; *i != '\0'; i++)
		;
	return (int)(i-myString);
}

inline char CBaseStr::GetChar(int nPos)
{
	#ifdef BOUNDS_CHECK_GETCHAR
	if(StrLen() < nPos)
		throw("ERROR: GetChar attempted to access beyond end of string");
	#endif
	return (char)*(myString + nPos);	//blindingly fast, but no bounds checking.
}

signed int CBaseStr::Find(const char c, int cStartPos = 0, bool fIgnoreCase = DEFAULTCASE)
{
	char *cptr = myString + cStartPos;
	while(*cptr != '\0' && *cptr != c)
		cptr++;
	if(*cptr == '\0')
		return -1;
	return (cptr - myString);
}

inline signed int CBaseStr::Find(const char *cstr, int cStartPos = 0, bool fIgnoreCase = DEFAULTCASE)
{
	char *cptr = strstr(myString + cStartPos, cstr);
	if(!cptr)
		return -1;
	return (cptr - myString);
}

inline bool CBaseStr::Contains(const char *substr, bool fIgnoreCase = DEFAULTCASE)
{
	return (Find(substr, 0, fIgnoreCase) > -1);
}

inline bool CBaseStr::Matches(const char *nstr, bool fIgnoreCase = DEFAULTCASE)
{
	if(fIgnoreCase)
		return !strcasecmp(myString, nstr);
	return !strcmp(myString, nstr);
}

bool CBaseStr::PrefixMatch(const char *pref)
{
	const char *mystr = myString;
	for(; *pref && *mystr; pref++, mystr++)
		if(*pref && (!(*mystr) || UPCHAR(*pref) != UPCHAR(*mystr)))
			return FALSE;
	return TRUE;
}
int CBaseStr::ToNumber(void)
{
	return atoi(myString);
}

void CBaseStr::ToUpperCase(void)
{
	PrepForMod();
	
	for(int i = 0; *(myString + i) != '\0'; i++)
		if(*(myString + i) >= 'a' && *(myString + i) <= 'z')
			*(myString + i) += 'A' - 'a';
}

void CBaseStr::ToLowerCase(void)
{
	PrepForMod();

	for(int i = 0; *(myString + i) != '\0'; i++)
		if(*(myString + i) >= 'A' && *(myString + i) <= 'Z')
			*(myString + i) += 'a' - 'A';
}

void CBaseStr::Capitalize(void)
{
	PrepForMod();
	if(*myString >= 'a' && *myString <= 'z')
		*myString += 'A' - 'a';
	for(int i = 1; *(myString + i) != '\0'; i++)
		if(*(myString + i) >= 'A' && *(myString + i) <= 'Z')
			*(myString + i) += 'a' - 'A';
}

#ifdef RSTRDEBUG
#include <unistd.h>
int main()
{
	CBaseStr newstr;
	CBaseStr fuck, dood, party;
	long wtf;
	int j,k,i;
	newstr.SetBuffer("moo!");

	fuck = "up yours, pal";
	party = "yes I'm going to a...";

	dood.Format("%s party party! %s %s! I would like you to %cance",
			(const char *)party, (const char *)newstr, (const char *)fuck, 'd');

	sleep(5);

	fprintf(stderr,"done");
	sleep(5);
	//fprintf(stderr,"%s\n%s\n%s\n%s", (const char *)dood, (const char *)newstr, (const char *)fuck, (const char *)party);
}
#endif
