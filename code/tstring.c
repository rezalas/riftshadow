#include "stdlibs/tstring.h"
#include <string.h>

TString::TString()
{
	myBuf[0] = '0';
	myString = &myBuf[1];
}

TString::~TString()
{
	//nothing ..
}
#include <stdio.h>
void TString::SafeDealloc()
{
	//nothing ..
	fprintf(stderr,"Hi safed");
	return;
}

char *TString::GetString()
{
	return &myBuf[1];
}

void TString::PrepForMod()
{
}

void TString::CloneString()
{
}

TString& TString::operator =(const char *str)
{
	if(!str)
		return *this;
	strcpy(&myBuf[1], str);
	return *this;
}

TString& TString::operator =(RString &str)
{
	if(!str.myString)
		return *this;
	strcpy(myString, str.myString);
	return *this;
}

TString TString::operator +(const char *str)
{
	*this += str;
	return *this;
}

TString& TString::operator +=(const char *str)
{
	strcat(&myBuf[1], str);
	return *this;
}

