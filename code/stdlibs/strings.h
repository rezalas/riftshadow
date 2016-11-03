#include <stdarg.h>
#define TRUE        1
#define FALSE       0
//#define NULL        0
#define size_t      unsigned int

#define DEFAULTCASE TRUE
#define MAX_FORMATTED_SIZE 4096
#define TSTR_LEN		2048
#define MUNCH_VARARG(msg, str)  va_list arglist;va_start(arglist, msg);str.Format(arglist, msg);va_end(arglist);

class RString
{
public:
	RString();
	RString(const RString& nStr);
	~RString();
	bool IsShared();

	/* OPERATORS */
	
	operator const char*() const;			/* you can use it as a const char * but MODIFY IT AND DIE!! (use 							   getbuffer to mod) */
	RString& operator   =(RString& str);		/* pointer-based copy [equiv to RString.Clone()] */
	RString& operator  =(const char *str);		/* sets RString to value */
	RString  operator  +(const char *str);
	RString  operator  +(RString& str);
	RString  operator  +(const char str);
	RString& operator +=(RString& str);		/* string concatenation via += */
	RString& operator +=(const char *str);		/* more of the same */
	RString& operator +=(const char str);		/* I'll have a bit more */
	bool 	 operator ==(const char *str);		/* string comparison based on DEFAULTCASE  */
	bool	 operator !=(const char *str);		/* ditto */
	char	 operator [](const int nIndex);		/* access nIndex'th char of the string */

	/* ACCESS FUNCTIONS */
	
	void		CopyFrom(RString& src);	/* makes a new, unshared copy of mr stringy string (src) */	
	char * 		GetBuffer(void);		/* REALLOCS STRING IF SHARED and returns a char ptr you can molest */
	void		SetBuffer(const char *newstr);	/* makes a new, alloced separate copy of newstr */
	void		Clone(RString& clone_to);	/* create cloned RString. SHARED with parent! */
	char *		LowLevelAccess(void);		/* low level editing of shared strings.  Don't use if you're not me.  							   YOU AREN'T. */

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
				bool fIgnoreCase = DEFAULTCASE); /* TRUE if the RString contains substr */
	bool 		PrefixMatch(const char *pref);
	
	/* MODIFICATION FUNCTIONS */
	
	void 		ToLowerCase(void);		/* convert string to lower case */
	void		ToUpperCase(void);		/* convert string to upper case */
	void		Capitalize(void);		/* turns tOoblaH into Tooblah 	*/
	
	void 		NoDeallocSetTo(const char *str); /* sets to str without freeing current string.  ME ONLY.  */

	RString&	Format(va_list arglist,
							   const char *msg,  
						   					   ...);
	RString&	Format(const char *msg, 
							...); /* k-rad sprintf wrapper, returns reference */	 
protected:
	char *		myString;

	void SafeDealloc();
	void AllocString(const char *newstr);
	void CloneString();
	void PrepForMod();
	int RefCount();
	void SetRefCount(int nRef);
};

class TString
{
public:
	TString();
	TString(const TString& nStr);
	~TString();
	/* OPERATORS */
	
	operator const char*() const;			/* you can use it as a const char * but MODIFY IT AND DIE!! (use 							   getbuffer to mod) */
	TString& operator  =(TString& str);		/* pointer-based copy [equiv to TString.Clone()] */
	TString& operator  =(const char *str);		/* sets TString to value */
	TString  operator  +(const char *str);
	TString  operator  +(TString& str);
	TString  operator  +(const char str);
	TString& operator +=(TString& str);		/* string concatenation via += */
	TString& operator +=(const char *str);		/* more of the same */
	TString& operator +=(const char str);		/* I'll have a bit more */
	bool 	 operator ==(const char *str);		/* string comparison based on DEFAULTCASE  */
	bool	 operator !=(const char *str);		/* ditto */
	char	 operator [](const int nIndex);		/* access nIndex'th char of the string */

	/* ACCESS FUNCTIONS */
	
	void		CopyFrom(TString& src);	/* makes a new, unshared copy of mr stringy string (src) */	
	char * 		GetBuffer(void);		/* REALLOCS STRING IF SHARED and returns a char ptr you can molest */
	void		SetBuffer(const char *newstr);	/* makes a new, alloced separate copy of newstr */
	void		Clone(TString& clone_to);	/* create cloned TString. SHARED with parent! */
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
				bool fIgnoreCase = DEFAULTCASE); /* TRUE if the TString contains substr */
	bool 		PrefixMatch(const char *pref);
	
	/* MODIFICATION FUNCTIONS */
	
	void 		ToLowerCase(void);		/* convert string to lower case */
	void		ToUpperCase(void);		/* convert string to upper case */
	void		Capitalize(void);		/* turns tOoblaH into Tooblah 	*/

	TString&	Format(va_list arglist,
					   const char *msg,  
		 			   ...);
	TString&	Format(const char *msg, 
						...); /* k-rad sprintf wrapper, returns reference so */
	bool 		IsShared();
protected:
	char *		myString;

	void SafeDealloc();
	void AllocString(const char *newstr);
	void CloneString();
	void PrepForMod();
	int RefCount();
	void SetRefCount(int nRef);
	char myBuf[TSTR_LEN];
};

