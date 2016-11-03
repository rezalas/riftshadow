#define TRUE		1
#define FALSE		0
#define NULL		0
#define size_t		unsigned int

#define DEFAULTCASE TRUE
#define MAX_FORMATTED_SIZE 4096

#include <stdarg.h>

/* Cal's basic string class
*  RString and TString inheirit it
*  and handle the allocation issues
*/
class CBaseStr
{
public:
	CBaseStr(void);
	virtual ~CBaseStr();

	/* ACCESS FUNCTIONS */
	
	void		CopyFrom(CBaseStr& src);	/* makes a new, unshared copy of mr stringy string (src) */	
	virtual char * GetBuffer(void);		/* REALLOCS STRING IF SHARED and returns a char ptr you can molest */
	void		SetBuffer(const char *newstr);	/* makes a new, alloced separate copy of newstr */
	void		Clone(CBaseStr& clone_to);	/* create cloned CBaseStr. SHARED with parent! */
	char *		LowLevelAccess(void);		/* low level editing of shared strings.  Don't use if you're not me.  							   YOU AREN'T. */
	void		NoDeallocSetTo(const char *str);/* sets to str without freeing current string.  ME ONLY.  */

	/* INFORMATION FUNCTIONS */
	
	int			StrLen(void);		/* returns length of string AS CALCULATED BY looping until the first 							null */
	int			ToNumber(void);		/* atoi */
	char		GetChar(int nPos);		/* character at position nPos in string, starting at 0 */
	bool		Matches(const char *nstr,
				bool fIgnoreCase = DEFAULTCASE); /* is nstr an exact match for me? */
	signed int	Find(const char c, 
				int cStartPos = 0,
				bool fIgnoreCase = DEFAULTCASE); /* find first instance of letter, -1 for no match */
	signed int	Find(const char *cstr,
				int cStartPos = 0,
				bool fIgnoreCase = DEFAULTCASE); /* returns # of first instance of match inside string, -1 								 for no match */
	bool		Contains(const char *substr,
				bool fIgnoreCase = DEFAULTCASE); /* TRUE if the CBaseStr contains substr */
	virtual bool		IsShared(void) = 0;		 /* TRUE if CBaseStr is currently shared */
	bool 		CBaseStr::PrefixMatch(const char *pref);
	
	/* MODIFICATION FUNCTIONS */
	
	void 		ToLowerCase(void);		/* convert string to lower case */
	void		ToUpperCase(void);		/* convert string to upper case */
	void		Capitalize(void);		/* turns tOoblaH into Tooblah 	*/
protected:
	virtual void	SafeDealloc() = 0;
	virtual void	AllocString(const char *newstr) = 0;
	virtual void	CloneString(void) = 0;
	virtual void	PrepForMod(void) = 0;			/* if I'm shared, make me a unique snowflake */
	char *			myString;					/* the actual string */
	virtual int		RefCount(void) = 0;
	virtual void	SetRefCount(int nRef) = 0;
};


