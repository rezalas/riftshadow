#include "stdlibs/rstring.h"
#include "stdlibs/tstring.h"

#include <string.h>

#include <stdio.h>
#include <stdlib.h>

//#define RSTRDEBUG

RString::RString(void)
{
	myString = NULL;
}

RString::~RString(void)
{
	SafeDealloc();
}

RString::RString(const RString& nStr)
{   
	/* copy constructor called by RString new = me*/
	myString = nStr.myString;
	SetRefCount(RefCount()+1);
}

RString::operator char const *(void) const
{
	return myString;
}

char RString::operator [](const int nIndex)
{
	return GetChar(nIndex);
}

bool RString::operator ==(const char *str)
{
	return Matches(str, DEFAULTCASE);
}

bool RString::operator !=(const char *str)
{
	return !Matches(str, DEFAULTCASE);
}

RString& RString::operator =(RString &str)
{
	SafeDealloc();
	str.Clone(*this);
	return *this;
}

RString& RString::operator =(const char *str)
{
	SafeDealloc();
	if(str != NULL)
		AllocString(str);
	return *this;
}

RString& RString::operator =(TString &str)
{
	SafeDealloc();
	if(*(str.myString) != '\0')
		AllocString(str.myString);
	return *this;
}

RString RString::operator +(RString& str)
{
	RString nStr = *this;
	nStr += str;			//there is a special hell for me for doing this
	return nStr;
}

RString RString::operator +(const char *str)
{
	RString nStr = *this;
	nStr += str;
	return nStr;
}

RString RString::operator +(const char str)
{
	RString nStr = *this;
	nStr += str;
	return nStr;
}

RString& RString::operator +=(RString& str)
{
	if(!str.myString)
		return *this;
	char *nstring = new char[StrLen() + str.StrLen() + 2];
	nstring++;
	*nstring = '\0';
	if(myString)
		strcpy(nstring, myString);

	strcat(nstring, str.myString);
	SafeDealloc();
	myString = nstring;
	SetRefCount(0);
	return *this;
}

RString& RString::operator +=(const char *str)
{
	if(!str)
		return *this;
	char *nstring = new char[StrLen() + strlen(str) + 2];
	nstring++;
	*nstring = '\0';
	if(myString)
		strcpy(nstring, myString);
	
	strcat(nstring, str);
	SafeDealloc();
	myString = nstring;
	SetRefCount(0);
	return *this;
}

RString& RString::operator +=(const char str)
{
	if(str == '\0')
		return *this;
	//behold, the power of cheese!
	char buf[2];
	buf[0] = str; buf[1] = '\0';
	*this += (char *)&buf;
	return *this;
}

inline bool RString::IsTString(void)
{
	/* the worst hack ever */
	int i;
	if(!myString)
		return FALSE;
	return ((long)myString > (long)&i);
}
inline int RString::RefCount(void)
{
	return (int)*(myString-1);
}

inline void RString::SetRefCount(int nRef)
{
	*(myString - 1) = nRef;
	return;
}

inline bool RString::IsShared(void)
{
	return (myString && RefCount() > 0);
}

void RString::PrepForMod(void)
{
	if(IsShared())
		CloneString();
}

void RString::CloneString(void)
{
	//takes the current string (presumably shared) and makes a new copy of it,
	//setting _this_ rstring to it
	SetRefCount(RefCount()-1);
	char *nstring = new char[StrLen()+2];
	nstring++;

	strcpy(nstring,myString);

	myString = nstring;
	SetRefCount(0);
}

void RString::AllocString(const char *newstr)
{
	//alloc: inits mystring/rstring to newstr
	char *nstring = new char[strlen(newstr)+2];	
//	fprintf(stderr, "Allocated %s.", newstr);
	nstring++;
	
	strcpy(nstring,newstr);
	
	myString = nstring;
	SetRefCount(0);
	return;
}

void RString::SafeDealloc(void)
{
	if(!myString)
		return;
	if(RefCount() > 0)
	{
		SetRefCount(RefCount() - 1);
		myString = NULL;
		return;
	}
	//hack
	int i;
	if((int *)myString > &i)
		return;
	
	myString--;
	delete[] myString;
}

char * RString::GetBuffer(void)
{
	if(RefCount() > 0)
	{
		CloneString();
		return myString;
	}
	return myString; //are we sure this is safe?
}

void RString::SetBuffer(const char *nbuffer)
{
	SafeDealloc();
	
	AllocString(nbuffer);	
	return;
}

void RString::NoDeallocSetTo(const char *str)
{
	AllocString(str);
}

char * RString::LowLevelAccess()
{
	return myString;
}

void RString::Clone(RString& clone_to)
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

void RString::CopyFrom(RString &src)
{
	SafeDealloc();

	AllocString(src.myString);
	
	return;
}

inline int RString::StrLen(void)
{
	char *i;
	if(!myString)
		return 0;
	for(i = myString; *i != '\0'; i++)
		;
	return (int)(i-myString);
}

inline char RString::GetChar(int nPos)
{
	#ifdef BOUNDS_CHECK_GETCHAR
	if(StrLen() < nPos)
		throw("ERROR: GetChar attempted to access beyond end of string");
	#endif
	return (char)*(myString + nPos);	//blindingly fast, but no bounds checking.
}

signed int RString::Find(const char c, int cStartPos = 0, bool fIgnoreCase = DEFAULTCASE)
{
	char *cptr = myString + cStartPos;
	while(*cptr != '\0' && *cptr != c)
		cptr++;
	if(*cptr == '\0')
		return -1;
	return (cptr - myString);
}

inline signed int RString::Find(const char *cstr, int cStartPos = 0, bool fIgnoreCase = DEFAULTCASE)
{
	char *cptr = strstr(myString + cStartPos, cstr);
	if(!cptr)
		return -1;
	return (cptr - myString);
}

inline bool RString::Contains(const char *substr, bool fIgnoreCase = DEFAULTCASE)
{
	return (Find(substr, 0, fIgnoreCase) > -1);
}

inline bool RString::Matches(const char *nstr, bool fIgnoreCase = DEFAULTCASE)
{
	if(fIgnoreCase)
		return !strcasecmp(myString, nstr);
	return !strcmp(myString, nstr);
}

int RString::ToNumber(void)
{
	return atoi(myString);
}

void RString::ToUpperCase(void)
{
	PrepForMod();
	
	for(int i = 0; *(myString + i) != '\0'; i++)
		if(*(myString + i) >= 'a' && *(myString + i) <= 'z')
			*(myString + i) += 'A' - 'a';
}

void RString::ToLowerCase(void)
{
	PrepForMod();

	for(int i = 0; *(myString + i) != '\0'; i++)
		if(*(myString + i) >= 'A' && *(myString + i) <= 'Z')
			*(myString + i) += 'a' - 'A';
}

void RString::Capitalize(void)
{
	PrepForMod();
	if(*myString >= 'a' && *myString <= 'z')
		*myString += 'A' - 'a';
	for(int i = 1; *(myString + i) != '\0'; i++)
		if(*(myString + i) >= 'A' && *(myString + i) <= 'Z')
			*(myString + i) += 'a' - 'A';
}

RString& RString::Format(const char *msg, ...)
{
	va_list arglist;
	va_start(arglist, msg);
	*this->Format(arglist, msg);
	va_end(arglist);
	return *this;
}

RString& RString::Format(va_list arglist, const char *msg, ...)
{
	if(msg == NULL)
		return *this;
	char buf[MAX_FORMATTED_SIZE]; //format can be used to stack overflow if you're a dumbass.  so don't be one.	
	int nLen;
	
	SafeDealloc();
	
	if(IsTString())
	{
		fprintf(stderr,"Formatting TString");
		vsprintf(myString, msg, arglist);
		return *this;
	}	
	nLen = vsprintf(&buf[0], msg, arglist);
	AllocString(&buf[0]);
	fprintf(stderr,"Formatting RString");
	return *this;
}

#ifdef RSTRDEBUG
#include <unistd.h>
int main()
{
	RString newstr;
	RString fuck, dood, party;
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
