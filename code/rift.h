//#define NULL			0
#define TRUE			1
#define FALSE			0

#define STATE_INVALID	0
#define STATE_VALID		1

#define MAX_EXITS		6

#ifndef BEENHERE //!
#define BEENHERE
#include "stdlibs/bitvector.h"
#include "stdlibs/strings.h" //includes rstring.h
//#include "stdlibs/exception.h"
#include "stdlibs/funcptr.h"

#include "classlist.h"

extern CMud RS;
extern RString EmptyString;
void bug(const char *bugstr, ...); //wraps onto RS.Bug

//the following is for typecasting rstrings when the func is weak typing
#define RSTR	const char *

#define RIFT_DIR	/home/rift/
#define GREET_FILE	"../area/login.txt"

#define throwbug(...) throw Exception(__FILE__, __LINE__, __VA_ARGS__);
#define ERROR_TRAP()	try {
#define CATCH_ERRORS(j)	} catch (const char *msg) { bug("%s error: %s",j,msg); } catch (...) { bug("%s exception: unhandled",j); }

#define begin_benchmark 	struct timeval bttime; long bsec, esec; gettimeofday(&bttime, NULL); bsec = bttime.tv_usec;
#define end_benchmark(name)	gettimeofday(&bttime, NULL); esec = bttime.tv_usec; float res = (float)(esec - bsec) / 1000; \
							RS.Log("%s took %f ms.", name, res);
/////// smallmacros ///////
#define LOWCHAR(c) 		((c) >= 'A' && (c) <= 'Z' ? (c) + 'a' - 'A' : (c))
#define UPCHAR(c)		((c) >= 'a' && (c) <= 'z' ? (c) + 'A' - 'a' : (c))
template <class TYPE>
inline const TYPE& UMIN(const TYPE& a, const TYPE& b)
{
	return a < b ? a : b;
}
template <class TYPE>
inline const TYPE& UMAX(const TYPE& a, const TYPE& b)
{
	return a > b ? a : b;
}

inline long URANGE(long a, long b, long c)
{
	return 	b < a ? a : /* if b is less than a, a*/
			b > c ? c : /* b greater than c, then c */
	   		b;			/* otherwise b */
}
/* NOTE: strprefix returns TRUE if it IS a prefix, opposite of ROM behavior */
inline bool strprefix(const char *astr, const char *bstr)
{
	for(; *astr != '\0'; astr++, bstr++)
		if(LOWCHAR(*astr) != LOWCHAR(*bstr))
		   return FALSE;
	return TRUE;	
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

#endif
