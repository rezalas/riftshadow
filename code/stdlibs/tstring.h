/* tstrings are rstrings that use stack allocation instead of heap alloc
*  therefore they should ONLY be used for temporary (hence the t) strings */

#ifndef TSTRING
#define TSTRING

#include <string.h>
#include <stdio.h>

#ifndef RSTRING
	#include "rstring.h"
#endif


#define TSTR_LEN		2048

class TString : public RString
{
public:
	TString();
	~TString();
	TString& operator =(const char *str);
	TString& operator =(RString &str);
	TString operator +(const char *str);
	TString operator +(TString& str);
	TString& operator +=(const char *str);
	TString& operator +=(const char str);

	void PrepForMod();
	void CloneString();
	void SafeDealloc();	
	char *GetString();

private:
	char myBuf[TSTR_LEN];
};

#endif /* TSTRING_H */
