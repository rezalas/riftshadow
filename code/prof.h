#define PFLAGS_NONE 0
#define PFLAGS_BASIC 1

#define MAX_PROFS 25
#define START_PROF_COMMANDS	const struct prof_cmd_type prof_cmd_table[] = {
#define PROF_COMMAND(x, y, z)	{ #x, y, #z },
#define END_PROF_COMMANDS	};
#define DECL_PROF_CMD(x)	void x (CHAR_DATA *ch, char *argument);

typedef short int sh_int;
struct proficiency_type
{
        sh_int *ppsn;
        char *  name;
        int     cost;
        int     minlevel;
	char *	requires;
        long    flags; //note: old school bitvector
};
struct prof_level_type
{
	char *	name;
	int	level;
};
struct char_data;

class CProficiencies
{
friend class char_data;
public:
			CProficiencies() { ch = 0; for(int j = 0; j < MAX_PROFS; j++) profs[j] = -1; points = 0;}
	static void 	AssignPsns(void);
	static sh_int   ProfIndexLookup(const char *profname);
	bool		HasProf(int psn) { return profs[psn] > -1; }
	bool		HasProf(char *profname) { return HasProf(ProfIndexLookup(profname)); }
	int		GetProf(int psn) { return profs[psn]; }
	int		GetProf(char *profname) { return GetProf(ProfIndexLookup(profname)); }
	float		ProfEffect(char *profname, float nArg = 0);
	float		ProfEffect(int psn);
	void		SetChar(char_data *nch) { ch = nch; }
	void		SetProf(int profindex, int proflevel) { profs[profindex] = proflevel; }
	void		WriteProfs(void *vfp);
	void		DisplayProfsForStat(char_data *imm);
	const char *	GetSkillLevelName(int ind);
	void		ShowProfsToChar(void);
	void		SetPoints(int newpoints) { points = newpoints; }
	void		DeductPoints(int dpoints) { points -= dpoints; }
	int		GetPoints(void) { return points; }
	void		SetPATime(time_t newtime) { pawardedtime = newtime; }
	void		UpdateProfPoints(void);
	void		CheckImprove(int pindex, int chance);
	void		CheckImprove(char *profname, int chance) { return CheckImprove(ProfIndexLookup(profname), chance); }
	bool		InterpCommand(char *command, char *argument);
private:
	time_t		pawardedtime;
	char_data	*ch;
	int		points;
	int		profs[MAX_PROFS];
};

extern sh_int psn_none;
extern sh_int psn_swimming;
extern sh_int psn_mountaineering;
