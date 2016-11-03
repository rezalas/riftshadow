#define RSTRING
#define TRUE		1
#define FALSE		0
#define NULL		0
#define size_t		unsigned int

#define MAX_FORMATTED_SIZE 4096

#include <stdarg.h>

#define DEFAULTCASE TRUE
//#define BOUNDS_CHECK_GETCHAR //define for slower getchar or [] array access w/ check to make sure it's not beyond end of str

#define MUNCH_VARARG(msg, str)	va_list arglist;va_start(arglist, msg);str.Format(arglist, msg);va_end(arglist);
class RString
{
friend class TString;
	/*			NOTE TO SELF
	 *		do global string pool chunks
	 *		make addition less fugly
	 */
public:
	RString();
	~RString();
	RString(const RString& nStr);

	/* OPERATORS */
	
	operator const char*() const;				/* you can use it as a const char * but MODIFY IT AND DIE!! (use getbuffer to mod) */
	RString& operator  =(TString& str); 
	RString& operator  =(RString& str);			/* pointer-based copy [equiv to RString.Clone()] */
	RString& operator  =(const char *str);		/* sets rstring to value */
	RString  operator  +(const char *str);
	RString  operator  +(RString& str);
	RString  operator  +(const char str);
	RString& operator +=(RString& str);			/* string concatenation via += */
	RString& operator +=(const char *str);		/* more of the same */
	RString& operator +=(const char str);		/* I'll have a bit more */
	bool 	 operator ==(const char *str);		/* string comparison based on DEFAULTCASE  */
	bool	 operator !=(const char *str);		/* ditto */
	char	 operator [](const int nIndex);		/* access nIndex'th char of the string */
	
	/* ACCESS FUNCTIONS */
	
	void		CopyFrom(RString& src);			/* makes a new, unshared copy of mr stringy string (src) */	
	char *		GetBuffer(void);				/* REALLOCS STRING IF SHARED and returns a char ptr you can molest */
	void		SetBuffer(const char *newstr);	/* makes a new, alloced separate copy of newstr */
	void		Clone(RString& clone_to);		/* create cloned RString. SHARED with parent! */
	char *		LowLevelAccess(void);			/* low level editing of shared strings.  Don't use if you're not me.  YOU AREN'T. */
	void		NoDeallocSetTo(const char *str);/* sets to str without freeing current string.  ME ONLY.  */
	/* INFORMATION FUNCTIONS */
	
	int			StrLen(void);					/* returns length of string AS CALCULATED BY looping until the first null */
	int			ToNumber(void);					/* atoi */
	char		GetChar(int nPos);				/* character at position nPos in string, starting at 0 */
	bool		Matches(const char *nstr,
					bool fIgnoreCase = DEFAULTCASE);	/* is nstr an exact match for me? */
	signed int	Find(const char c, 
					int cStartPos = 0,
					bool fIgnoreCase = DEFAULTCASE);	/* find first instance of letter, -1 for no match */
	signed int	Find(const char *cstr,
					int cStartPos = 0,
					bool fIgnoreCase = DEFAULTCASE);	/* returns # of first instance of match inside string, -1 for no match */
	bool		Contains(const char *substr,
					bool fIgnoreCase = DEFAULTCASE);	/* TRUE if the Rstring contains substr */
	bool		IsShared(void);					/* TRUE if RString is currently shared */

	/* MODIFICATION FUNCTIONS */
	
	void 		ToLowerCase(void);				/* convert string to lower case */
	void		ToUpperCase(void);				/* convert string to upper case */
	void		Capitalize(void);				/* turns tOoblaH into Tooblah 	*/
	RString&	Format(va_list arglist,			/* I'm willing to cut you, fool.  Keep that in mind. */
					   const char *msg,  
					   ...);
	RString&	Format(const char *msg, 
					...);						/* k-rad sprintf wrapper, returns reference so it can be tossed inside call */
protected:
	void 		SafeDealloc();
	void		AllocString(const char *newstr);
	void		CloneString(void);
	void		PrepForMod(void);				/* if I'm shared, make me a unique snowflake */
	char *		myString;						/* the actual string */
	int			RefCount(void);
	void		SetRefCount(int nRef);
	bool		IsTString(void);				/* horrid hack */

};

#ifndef TSTRING
#include "tstring.h"
#endif
