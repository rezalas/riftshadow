#include "stdlibs/strings.h"
#define UPCHAR(c)       ((c) >= 'a' && (c) <= 'z' ? (c) + 'A' - 'a' : (c))

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int vsnprintf(char *str, size_t size, const char *format, va_list ap);
char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, size_t n);
size_t strlen(const char *s);
char *strcat(char *dest, const char *src);

RString::RString()
{
	myString = NULL;
}

TString::TString()
{
	myBuf[0] = '\0';
	myString = &myBuf[0];
}

RString::~RString()
{
	SafeDealloc();
}

TString::~TString()
{
	return;
}

int RString::RefCount(void)
{
	return (int)*(myString-1);
}

int TString::RefCount(void)
{
	return 0;
}

void RString::SetRefCount(int nRef)
{
	*(myString - 1) = nRef;
	return;
}

void TString::SetRefCount(int nRef)
{
	return;
}

bool RString::IsShared(void)
{
	return (myString && RefCount() > 0);
}

bool TString::IsShared(void)
{
	return FALSE;
}

void RString::PrepForMod(void)
{
	if(IsShared())
		CloneString();
}

void TString::PrepForMod(void)
{
	return;
}

char * TString::GetBuffer(void)
{
	return myString; 
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

void TString::SetBuffer(const char *nbuffer)
{
	AllocString(nbuffer);	
	return;
}

void TString::NoDeallocSetTo(const char *str)
{
	AllocString(str);
}

void RString::NoDeallocSetTo(const char *str)
{
	AllocString(str);
}

char * RString::LowLevelAccess()
{
	return myString;
}

char * TString::LowLevelAccess()
{
	return myString;
}

void RString::Clone(RString& clone_to)
{
	/* makes shared copy */
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

inline int TString::StrLen(void)
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

inline char TString::GetChar(int nPos)
{
	if(nPos >= TSTR_LEN)
		return '@';
	return (char)*(myString + nPos);
}
/* Find occurence of letter c inside myString */
signed int RString::Find(const char c, int cStartPos, bool fIgnoreCase)
{
	char *cptr = myString + cStartPos;
	while(*cptr != '\0' && *cptr != c)
		cptr++;
	if(*cptr == '\0')
		return -1;
	return (cptr - myString);
}

signed int TString::Find(const char c, int cStartPos, bool fIgnoreCase)
{
	char *cptr = myString + cStartPos;
	while(*cptr != '\0' && *cptr != c)
		cptr++;
	if(*cptr == '\0')
		return -1;
	return (cptr - myString);
}

/* find occurence of cstr inside myString */
inline signed int RString::Find(const char *cstr, int cStartPos, bool fIgnoreCase)
{
	char *cptr = strstr(myString + cStartPos, cstr);
	if(!cptr)
		return -1;
	return (cptr - myString);
}

inline signed int TString::Find(const char *cstr, int cStartPos, bool fIgnoreCase)
{
	char *cptr = strstr(myString + cStartPos, cstr);
	if(!cptr)
		return -1;
	return (cptr - myString);
}

inline bool RString::Contains(const char *substr, bool fIgnoreCase)
{
	return (Find(substr, 0, fIgnoreCase) > -1);
}

inline bool TString::Contains(const char *substr, bool fIgnoreCase)
{
	return (Find(substr, 0, fIgnoreCase) > -1);
}

inline bool RString::Matches(const char *nstr, bool fIgnoreCase)
{
	if(fIgnoreCase)
		return !strcasecmp(myString, nstr);
	return !strcmp(myString, nstr);
}

inline bool TString::Matches(const char *nstr, bool fIgnoreCase)
{
	if(fIgnoreCase)
		return !strcasecmp(myString, nstr);
	return !strcmp(myString, nstr);
}

bool RString::PrefixMatch(const char *pref)
{
	const char *mystr = myString;
	for(; *pref && *mystr; pref++, mystr++)
		if(*pref && (!(*mystr) || UPCHAR(*pref) != UPCHAR(*mystr)))
			return FALSE;
	return TRUE;
}

bool TString::PrefixMatch(const char *pref)
{
	const char *mystr = myString;
	for(; *pref && *mystr; pref++, mystr++)
		if(*pref && (!(*mystr) || UPCHAR(*pref) != UPCHAR(*mystr)))
			return FALSE;
	return TRUE;
}

int RString::ToNumber(void)
{
	return atoi(myString);
}

int TString::ToNumber(void)
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

void TString::ToUpperCase(void)
{
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

void TString::ToLowerCase(void)
{
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

void TString::Capitalize(void)
{
	PrepForMod();
	if(*myString >= 'a' && *myString <= 'z')
		*myString += 'A' - 'a';
	for(int i = 1; *(myString + i) != '\0'; i++)
		if(*(myString + i) >= 'A' && *(myString + i) <= 'Z')
			*(myString + i) += 'a' - 'A';
}

void RString::CloneString(void)
{
	//takes the current string (presumably shared) and makes a new copy of it,
	//setting _this_ CBaseStr to it
	SetRefCount(RefCount()-1);
	char *nstring = new char[StrLen()+2];
	nstring++;

	strcpy(nstring,myString);

	myString = nstring;
	SetRefCount(0);
}

void TString::CloneString()
{
	return;
}

void RString::AllocString(const char *newstr)
{
	//alloc: inits mystring/CBaseStr to newstr
	char *nstring = new char[strlen(newstr)+2];	
	nstring++;
	
	strcpy(nstring,newstr);
	
	myString = nstring;
	RString::SetRefCount(0);
	return;
}

void TString::AllocString(const char *newstr)
{
	strncpy(myString, newstr, TSTR_LEN);
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

void TString::SafeDealloc(void)
{
	return;
}

RString::RString(const RString& nStr)
{   
	/* copy constructor called by RString new = me*/
	myString = nStr.myString;
	SetRefCount(RefCount()+1);
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
	
	SafeDealloc();
	vsnprintf(&buf[0], MAX_FORMATTED_SIZE, msg, arglist);
	AllocString(&buf[0]);
	//fprintf(stderr,"Formatting RString");
	return *this;
}


TString& TString::Format(const char *msg, ...)
{
	va_list arglist;
	va_start(arglist, msg);
	*this->Format(arglist, msg);
	va_end(arglist);
	return *this;
}

TString& TString::Format(va_list arglist, const char *msg, ...)
{
	if(msg == NULL)
		return *this;
	
	vsnprintf(myString, TSTR_LEN, msg, arglist);
	//fprintf(stderr,"Formatting TString");
	return *this;
}

RString::operator char const *(void) const
{
	return myString;
}

TString::operator char const *(void) const
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

bool TString::operator ==(const char *str)
{
	return Matches(str, DEFAULTCASE);
}
    
bool TString::operator !=(const char *str)
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

TString::TString(const TString& nStr)
{   
	strcpy(nStr.myString, myString);
}

TString& TString::operator =(const char *str)
{
	if(!str)
		return *this;
	strcpy(&myBuf[0], str);
	return *this;
}

TString& TString::operator =(TString &str)
{
	if(!str.myString)
		return *this;
	strcpy(myString, str.LowLevelAccess());
	return *this;
}

TString TString::operator +(const char *str)
{
	*this += str;
	return *this;
}

TString& TString::operator +=(const char *str)
{
	strcat(&myBuf[0], str);
	return *this;
}

char TString::operator [](const int nIndex)
{
	return GetChar(nIndex);
}

