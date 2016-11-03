/* tstrings are rstrings that use stack allocation instead of heap alloc
*  therefore they should ONLY be used for temporary (hence the t) strings */

#ifndef RSTRING
#include "rstring.h"
#endif
#ifndef TSTRING

#define TSTR_LEN		2048

class TString : public RString
{
public:
	TString();
	~TString();
	TString& operator  =(const char *str);
	TString& operator  =(RString &str);
	TString operator   +(const char *str);
	TString operator   +(TString& str);
	TString& operator +=(const char *str);
	TString& operator +=(const char str);

	void PrepForMod();
	void CloneString();
	void SafeDealloc();	
	char *GetString();

private:
	char myBuf[TSTR_LEN];
};
#define TSTRING
#endif
