#ifndef RIFT_H
#define RIFT_H

#define STATE_INVALID	0
#define STATE_VALID		1

#define MAX_EXITS		6

#include "stdlibs/bitvector.h"
#include "stdlibs/strings.h"
#include "stdlibs/funcptr.h"

// Forward declaration rather than #include "mud.h": mud.h includes this header,
// so including it back would be circular. `extern CMud RS` below needs only an
// incomplete type, which is exactly what a forward declaration provides.
class CMud;

#ifndef __cplusplus
#include <stdbool.h>
#endif

extern CMud RS;
extern RString EmptyString;
void bug(const char *bugstr, ...); //wraps onto RS.Bug

//the following is for typecasting rstrings when the func is weak typing
#define RSTR	const char *

//
// Data files used by the server.
//
// AREA_LIST contains a list of areas to boot.
// All files are read in completely at bootup.
// Most output files (bug, idea, typo, shutdown) are append-only.
//
// The NULL_FILE is held open so that we have a stream handle in reserve,
//   so players can go ahead and telnet to all the other descriptors.
// Then we close it whenever we need to open a file (e.g. a save file).
//

#define RIFT_ROOT_DIR			".."
#define RIFT_ADMIN_DIR			RIFT_ROOT_DIR "/admin"
#define RIFT_AREA_DIR			RIFT_ROOT_DIR "/area"
#define RIFT_CODE_DIR			RIFT_ROOT_DIR "/code"
#define RIFT_CONFIGS_DIR		RIFT_ROOT_DIR "/configs"
#define RIFT_DOCS_DIR			RIFT_ROOT_DIR "/docs"
#define RIFT_GOD_DIR			RIFT_ROOT_DIR "/gods"						// list of gods
#define RIFT_PLAYER_DIR			RIFT_ROOT_DIR "/player"
#define RIFT_LOGS_DIR			RIFT_ROOT_DIR "/logs"
#define RIFT_TEMP_DIR			RIFT_ROOT_DIR "/temp"

#define AREA_LIST				RIFT_AREA_DIR "/area.lst"					// List of areas
#define LOGIN_BANNER_FILE		RIFT_CONFIGS_DIR "/login_banner.txt"
#define CABAL_ITEMS_FILE		RIFT_CONFIGS_DIR "/citems.txt"
#define IDEAS_FILE				RIFT_DOCS_DIR "/ideas.txt"					// For ideas!
#define RS_LOG_FILE				RIFT_LOGS_DIR "/rs.log"
#define DEBUG_LOG_FILE			RIFT_LOGS_DIR "/debug.txt"
#define GOD_LOG_FILE			RIFT_LOGS_DIR "/glog.txt"
#define AREA_DUMP_FILE 			RIFT_LOGS_DIR "/area-dump.txt"
#define LOGIN_LOG_FILE			RIFT_LOGS_DIR "/logins.txt"
#define BOUNTY_LOG_FILE			RIFT_LOGS_DIR "/bounties.txt"
#define HISTORY_FILE			RIFT_LOGS_DIR "/history.txt"
#define PDEATH_LOG_FILE			RIFT_LOGS_DIR "/permdeaths.txt"
#define DEATH_LOG_FILE			RIFT_LOGS_DIR "/tempdeaths.txt"
#define MOB_LOG_FILE			RIFT_LOGS_DIR "/mobdeaths.txt"
#define PRAYER_FILE 			RIFT_LOGS_DIR "/prays.txt"
#define RACE_DUMP_FILE 			RIFT_LOGS_DIR "/racdump.txt"
#define CLIMATE_DUMP_FILE 		RIFT_LOGS_DIR "/climate-dump.txt"
#define INTERP_DUMP_FILE 		RIFT_LOGS_DIR "/interpdump.txt"
#define CONST_DUMP_FILE 		RIFT_LOGS_DIR "/constdump.txt"
#define MEM_DUMP_FILE			RIFT_LOGS_DIR "/mem.dmp"
#define MOB_DUMP_FILE			RIFT_LOGS_DIR "/mob.dmp"
#define OBJ_DUMP_FILE			RIFT_LOGS_DIR "/obj.dmp"
#define DEMO_LOG_FILE			RIFT_LOGS_DIR "/demo.txt"
#define BUG_FILE				RIFT_LOGS_DIR "/bugs.txt"					// For 'bug' and bug()
#define TYPO_FILE				RIFT_LOGS_DIR "/typos.txt"					// For 'typo'
#define PLAYER_LIST				RIFT_PLAYER_DIR "/Player.lst"				// Player list for limits
#define TEMP_GREP_RESULTS 		RIFT_TEMP_DIR "/tempgrepresults.tmp" 		// Temporary grep results
#define TEMP_GREP_RESULTS_TWO 	RIFT_TEMP_DIR "/tempgrepresults2.tmp"

#ifdef __unix__
	#define NULL_FILE				"/dev/null"				// To reserve one stream
#elif _WIN32
	#define NULL_FILE 				RIFT_ROOT_DIR "/nul"	// win32 equiv to /dev/null
#endif


#define throwbug(...) throw Exception(__FILE__, __LINE__, __VA_ARGS__);
#define ERROR_TRAP()	try {
#define CATCH_ERRORS(j)	} catch (const char *msg) { bug("%s error: %s",j,msg); } catch (...) { bug("%s exception: unhandled",j); }

#define begin_benchmark 	struct timeval bttime; long bsec, esec; gettimeofday(&bttime, NULL); bsec = bttime.tv_usec;
#define end_benchmark(name)	gettimeofday(&bttime, NULL); esec = bttime.tv_usec; float res = (float)(esec - bsec) / 1000; \
							RS.Log("%s took %f ms.", name, res);
/////// smallmacros ///////
#define LOWCHAR(c) 		((c) >= 'A' && (c) <= 'Z' ? (c) + 'a' - 'A' : (c))
#define UPCHAR(c)		((c) >= 'a' && (c) <= 'z' ? (c) + 'A' - 'a' : (c))

inline long URANGE(long a, long b, long c)
{
	return 	b < a ? a : /* if b is less than a, a*/
			b > c ? c : /* b greater than c, then c */
	   		b;			/* otherwise b */
}
/* NOTE: strprefix returns true if it IS a prefix, opposite of ROM behavior */
inline bool strprefix(const char *astr, const char *bstr)
{
	for(; *astr != '\0'; astr++, bstr++)
		if(LOWCHAR(*astr) != LOWCHAR(*bstr))
		   return false;
	return true;
}
/*inline void capitalize(char *myString)
{
	if(*myString >= 'a' && *myString <= 'z')
		*myString += 'A' - 'a';
    for(int i = 1; *(myString + i) != '\0'; i++)
		if(*(myString + i) >= 'A' && *(myString + i) <= 'Z')
			*(myString + i) += 'a' - 'A';
}*/
#define ZERO_TIMEVAL(time)	time.tv_sec = 0; time.tv_usec = 0;

#endif /* RIFT_H */
