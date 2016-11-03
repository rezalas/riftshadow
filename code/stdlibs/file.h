#ifndef CFILE
#define CFILE

#define STRING_END		'~'

#include <stdio.h>
#include "../rift.h"
#ifndef BEENHERE
//	#define throwbug(...) throw Exception(__FILE__, __LINE__, __VA_ARGS__);
#endif

#define MAX_WORD_LENGTH 2048
#define MAX_STRING_LENGTH 4096

#define MODE_READ 	0
#define MODE_WRITE 	1
#define MODE_APPEND 2

class CFile
{
public:
	CFile();
	~CFile();

	bool LoadFile(const char *file,		 //returns false for failure
				  int mode = MODE_READ); //false to not append to file
	void CloseFile();
	bool End();							//end of file
	
	char ReadLetter();
	long ReadNumber();
	void ReadBitvector(CBitvector &vec);
	void ReadToEOL();
	TString& ReadWord();			//note: this and readstring share the same str!  return string is INVALID after the next read
	TString& ReadString(bool fToEOL = FALSE);  //so assign to a rstring result.  readstring is terminated by STRING_END.
	TString& ReadStringToEOL(); 		//just calls readstring(true)
private:
	FILE * fp;
	bool endoffile;

	void UpdateEOF();

	TString contents;

	TString result;
};
#endif
