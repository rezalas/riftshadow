#include "rift.h"
class CPCData
{
friend class CCharacter;
public:
	CPCData()
	{
	};
	~CPCData()
	{
	};
	
	static bool				IsValidPlayer(const char *name, bool lDead = FALSE);
	static bool				IsDeadPlayer(const char *name);
	static bool				IsValidPassword(const char *name, const char *pwd);
	CPCData *				next;
	int						scroll_lines;
private:
	float					age_mod;

	int						beauty;
};
