#include "rift.h"

#ifndef MAX_STAT
#define MAX_STAT 5
#endif
class CRace
{
friend class CMud;
public:
	CRace();
	~CRace();

	static CRace *			GetRace(int nIndex);		//get race based on race #

	static void		LoadRaceTable(const char *query);

	CRace *					next;
	static CRace *			first;
	RString					name;
	RString					short_name;
	RString					imm_name;
	RString					race_time;
	
	bool					pcrace;
	bool					race_status;

	CBitvector				act_flags;
	CBitvector				aff_flags;
	CBitvector				off_flags;
	CBitvector				imm_flags;
	CBitvector				res_flags;
	CBitvector				vuln_flags;

	CBitvector				form_flags;
	CBitvector				body_parts;
	
	int						arms;
	int						legs;
	int						size;
	
	float					age_mod;
	int						deathtime;
	int						align;
	int						xp_add;

	int						max_stats[MAX_STAT];

/*	enum
	{
		ALIGN_G,
		ALIGN_N,
		ALIGN_E,
		ALIGN_GN,
		ALIGN_GE,
		ALIGN_NE
	};*/
	int						index;
};

