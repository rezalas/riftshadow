#ifndef PROF_H
#define PROF_H

#include <time.h>
#include <vector>
#include <algorithm>

// TODO: these are defined in utility.h as regular functions but for some reason the compiler freaks out
// TODO: so leaving them as macros for the time being
#define IS_NPC(ch)					(IS_SET(ch->act, ACT_IS_NPC))
#define IS_GROUND(room)				(room->sector_type != SECT_AIR && room->sector_type != SECT_WATER && room->sector_type != SECT_UNDERWATER && room->sector_type != SECT_VERTICAL)


#define PFLAGS_NONE 0
#define PFLAGS_BASIC 1

#define MAX_PROFS 25


struct char_data;										
typedef struct char_data CHAR_DATA;						// mirrored definition from merc.h
typedef void DO_FUN (CHAR_DATA *ch, char *argument);	// mirrored definition from merc.h

struct proficiency_type
{
	short *ppsn;
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

struct proficiency_msg
{
	char *learning_msgs[5];
};

struct prof_cmd_type
{
	char *name;
	DO_FUN *cmd;
	char *requires;
};


extern char *format_string (char *oldstring);
extern short psn_none;
extern short psn_swimming;
extern short psn_mountaineering;

void add_prof_affect(CHAR_DATA *ch, char *name, int duration, bool fInvis);
bool is_affected_prof(CHAR_DATA *ch, char *prof);
void do_proficiencies(CHAR_DATA *ch, char *argument);
void prof_tracking(CHAR_DATA *ch, char *argument);
void build_fire(CHAR_DATA *ch, int dur);
void prof_firestart(CHAR_DATA *ch, char *argument);
void prof_cook(CHAR_DATA *ch, char *argument);
void prof_appraise(CHAR_DATA *ch, char *argument);
void prof_butcher(CHAR_DATA *ch, char *argument);
void prof_bandage(CHAR_DATA *ch, char *argument);


class CProficiencies
{
friend class char_data;

public:
	CProficiencies();
	~CProficiencies();

	static void AssignPsns(void);
	static short ProfIndexLookup(const char *profname);
	bool HasProf(int psn);
	bool HasProf(char *profname);
	int GetProf(int psn);
	int GetProf(char *profname);
	void GetProfsTaughtByTrainer(char_data* ch, char_data* trainer);
	void TrainProficiency(char_data* ch, char_data* trainer, char* argument);
	proficiency_type GetProficiency(int psn);
	proficiency_type GetProficiency(char* profname);
	float ProfEffect(char *profname, float nArg = 0);
	float ProfEffect(int psn);
	void ListKnownProficiencies(char_data* player);
	void ListBasicProficiencies(char_data* player);
	void SetChar(char_data *nch);
	void SetProf(int profindex, int proflevel);
	void WriteProfs(void *vfp);
	void DisplayProfsForStat(char_data *imm);
	const char * GetSkillLevelName(int ind);
	void ShowProfsToChar(void);
	void SetPoints(int newpoints);
	void DeductPoints(int dpoints);
	int GetPoints(void);
	void SetPATime(time_t newtime);
	void UpdateProfPoints(void);
	void CheckImprove(int pindex, int chance);
	void CheckImprove(char *profname, int chance);
	bool InterpCommand(char *command, char *argument);

private:
	time_t pawardedtime;
	char_data *ch;
	int points;
	int profs[MAX_PROFS];
	static const std::vector<prof_level_type> prof_level_table;
	static const std::vector<proficiency_type> prof_table;
	static const std::vector<prof_cmd_type> prof_cmd_table;
	static const struct proficiency_msg prof_msg_table [MAX_PROFS];
};

#endif /* PROF_H */
