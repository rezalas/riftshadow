#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "stdlibs/file.h"

CFile::CFile()
{
	fp = NULL;
	endoffile = FALSE;
}

CFile::~CFile()
{
	CloseFile();
}

bool CFile::LoadFile(const char *file, int mode)
{
	endoffile = FALSE;
	
	if(!file)
		return FALSE;

	if((fp = fopen(file, mode == MODE_READ ? "r" : mode == MODE_WRITE ? "w+" : "a+")) == NULL)
	{
		fprintf(stderr,"LoadFile error: Unable to open %s.", file);
		return FALSE;
	}

	return TRUE;
}

void CFile::CloseFile(void)
{
	if(!fp)
		return;

	fclose(fp);
	fp = NULL;
}

bool CFile::End(void)
{
	return endoffile;
}

void CFile::UpdateEOF()
{
	if(!fp)
		return;
	char c = fgetc(fp);
	if(c == EOF)
		endoffile = TRUE;
	ungetc(c, fp);
}

TString& CFile::ReadWord(void)
{
	if(!fp)
		throw "ReadWord error: Invalid fp.";
	
	if(End())
		throw "ReadWord error: EOF.";
	
	unsigned char cEnd, tempbuf[MAX_WORD_LENGTH], *pbuf;
	for(cEnd = fgetc(fp); isspace(cEnd); cEnd = fgetc(fp))
		;
	if(cEnd == '\'' || cEnd == '"')
		pbuf = &tempbuf[0];
	else
	{
		tempbuf[0] = cEnd;
		pbuf = &tempbuf[0] + 1;
		cEnd = ' ';
	}

	for(; pbuf < tempbuf + MAX_WORD_LENGTH; pbuf++)
	{
		*pbuf = fgetc(fp);

		if(cEnd == ' ' ? isspace(*pbuf) : *pbuf == cEnd) 	//if the endchar is a  ' ' we're looking for whitespace
		{													//otherwise just a match to cEnd
			if(cEnd == ' ')
				ungetc(*pbuf, fp);							//back up!

			*pbuf = '\0';
			strcpy(result.GetBuffer(), (const char *)&tempbuf[0]);
			UpdateEOF();
			return result;
		}
	}
	UpdateEOF();
	if(!End())
		fprintf(stderr, "Bug: word too long: %s", tempbuf);
	return result;
}

void CFile::ReadToEOL()
{
	if(!fp)
		fprintf(stderr,"ReadToEOL: Null fp");
	char c;
	do
	{
		c = fgetc(fp);
	} while (c != EOF && c != '\n');
	UpdateEOF();
}

char CFile::ReadLetter()
{
	if(!fp)
		fprintf(stderr,"ReadLetter error, invalid fp.");
	char c = fgetc(fp);
	UpdateEOF();
	return c;
}

long CFile::ReadNumber()
{
	ReadWord();
	return atol(result);
}

void CFile::ReadBitvector(CBitvector &vec)
{
	ReadWord();
	vec.SetToDouble(result);
	return;
}

TString& CFile::ReadString(bool fToEOL)
{
	if(!fp)
		throw("ReadString error: Invalid fp.");
	
	char cEnd = fToEOL ? '\n' : STRING_END, c;
	char buf[MAX_STRING_LENGTH], *pbuf = &buf[0];

	do
	{
		c = fgetc(fp);
	} while (isspace(c)); //bzzt

	if((*pbuf++ = c) == cEnd)
		return result;

	while(1)
	{
		*pbuf = fgetc(fp);
		if(*pbuf == EOF || *pbuf == cEnd) //doink
		{
			if(cEnd == '\n' && (*(++pbuf) = fgetc(fp)) != '\r')
				ungetc(*pbuf--, fp); //clever!
			*(pbuf + (fToEOL ? 1 : 0)) = '\0'; //possible bux0r
			strcpy(result.GetBuffer(), (const char*)&buf[0]);
			UpdateEOF();
			return result;
			break;
		}
		pbuf++;
	}
}

inline TString& CFile::ReadStringToEOL()
{
	return ReadString(TRUE);
}
