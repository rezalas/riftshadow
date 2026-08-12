#ifndef RIFT_H
#define RIFT_H

#define STATE_INVALID	0
#define STATE_VALID		1

// MAX_EXITS removed -- it duplicated MAX_EXIT (direction.h), which is the count
// of the Directions enum and the natural home. Both were 6.

#include "stdlibs/bitvector.h"
#include "stdlibs/funcptr.h"
#include "riftpath.h"

// Forward declaration rather than #include "mud.h": mud.h includes this header,
// so including it back would be circular. `extern CMud RS` below needs only an
// incomplete type, which is exactly what a forward declaration provides.
class CMud;

#ifndef __cplusplus
#include <stdbool.h>
#endif

extern CMud RS;
void bug(const char *bugstr, ...); //wraps onto RS.Bug

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

//
// Every path below is resolved against the game root at run time rather than
// being a literal relative to whatever directory the process was started in.
// See riftpath.h for how the root is found and why these are not string
// literals any more.  They cannot be joined to an adjacent literal: write
// rift_path("/logs/thing.txt") rather than RIFT_LOGS_DIR "/thing.txt".
//
#define RIFT_ROOT_DIR			rift_path("")
#define RIFT_ADMIN_DIR			rift_path("/admin")
#define RIFT_AREA_DIR			rift_path("/area")
#define RIFT_CODE_DIR			rift_path("/code")
#define RIFT_CONFIGS_DIR		rift_path("/configs")
#define RIFT_DOCS_DIR			rift_path("/docs")
#define RIFT_GOD_DIR			rift_path("/gods")							// list of gods
#define RIFT_PLAYER_DIR			rift_path("/player")
#define RIFT_LOGS_DIR			rift_path("/logs")
#define RIFT_TEMP_DIR			rift_path("/temp")

#define AREA_LIST				rift_path("/area/area.lst")					// List of areas
#define LOGIN_BANNER_FILE		rift_path("/configs/login_banner.txt")
#define CABAL_ITEMS_FILE		rift_path("/configs/citems.txt")
#define IDEAS_FILE				rift_path("/docs/ideas.txt")				// For ideas!
#define RS_LOG_FILE				rift_path("/logs/rs.log")
#define DEBUG_LOG_FILE			rift_path("/logs/debug.txt")
#define GOD_LOG_FILE			rift_path("/logs/glog.txt")
#define AREA_DUMP_FILE 			rift_path("/logs/area-dump.txt")
#define LOGIN_LOG_FILE			rift_path("/logs/logins.txt")
#define BOUNTY_LOG_FILE			rift_path("/logs/bounties.txt")
#define HISTORY_FILE			rift_path("/logs/history.txt")
#define PDEATH_LOG_FILE			rift_path("/logs/permdeaths.txt")
#define DEATH_LOG_FILE			rift_path("/logs/tempdeaths.txt")
#define MOB_LOG_FILE			rift_path("/logs/mobdeaths.txt")
#define PRAYER_FILE 			rift_path("/logs/prays.txt")
#define RACE_DUMP_FILE 			rift_path("/logs/racdump.txt")
#define CLIMATE_DUMP_FILE 		rift_path("/logs/climate-dump.txt")
#define INTERP_DUMP_FILE 		rift_path("/logs/interpdump.txt")
#define CONST_DUMP_FILE 		rift_path("/logs/constdump.txt")
#define MEM_DUMP_FILE			rift_path("/logs/mem.dmp")
#define MOB_DUMP_FILE			rift_path("/logs/mob.dmp")
#define OBJ_DUMP_FILE			rift_path("/logs/obj.dmp")
#define DEMO_LOG_FILE			rift_path("/logs/demo.txt")
#define BUG_FILE				rift_path("/logs/bugs.txt")					// For 'bug' and bug()
#define TYPO_FILE				rift_path("/logs/typos.txt")				// For 'typo'
#define PLAYER_LIST				rift_path("/player/Player.lst")				// Player list for limits
#define TEMP_GREP_RESULTS 		rift_path("/temp/tempgrepresults.tmp") 		// Temporary grep results
#define TEMP_GREP_RESULTS_TWO 	rift_path("/temp/tempgrepresults2.tmp")
#define BUGOUT_FILE				rift_path("/logs/bugout.txt")

#ifdef __unix__
	#define NULL_FILE				"/dev/null"				// To reserve one stream
#elif _WIN32
	#define NULL_FILE 				rift_path("/nul")		// win32 equiv to /dev/null
#endif


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
