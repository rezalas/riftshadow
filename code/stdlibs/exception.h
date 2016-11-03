#ifndef EXCEPTION
#define EXCEPTION
#ifndef BEENHERE
	#include "tstring.h"
#endif
extern void bug(const char *bugstr, ...);

class Exception
{
public:
	Exception(char *pFile, int nLine, char *pMsg, ...)
	{
		TString tbug, ebug;
		MUNCH_VARARG(pMsg, tbug);
		ebug.Format("Bug at %s:%d - %s",pFile,nLine,(const char *)tbug);
		bug(ebug);
	}
};
#endif
