#ifndef CHARDEF_H
#define CHARDEF_H

#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include "merc.h"
#include "magic.h"
#include "interp.h"

struct pc_race_type // additional data for pc races
{
	char *name;						// MUST be in race_type
	char who_name[8];
	char *who_name_five;
	char *race_time;		
	float agemod;
	int death_modifier;				// how soon they die
	sh_int align;					// alignments race can have: 0=gne 1=gn 2=ne 3=g 4=n 5=e
	sh_int xpadd;					// extra xp per lvl needed
	char *skills[5];				// bonus skills for the race
	sh_int classes[MAX_CLASS]; 		// class restricts for races
	sh_int max_stats[MAX_STATS];	// maximum stats
	sh_int size;					// aff bits for the race
	sh_int racePulse;				// combat pulse for this race
	float racialDam;				// racial dam modifier
	sh_int status;
};

struct race_type
{
	char *name;		// call name of the race
	bool pc_race;	// can be chosen by pcs
	long act[10];	// act bits for the race
	long aff[10];	// aff bits for the race
	long off[10];	// off bits for the race
	long imm[10];	// imm bits for the race
	long res[10];	// res bits for the race
	long vuln[10];	// vuln bits for the race
	long form[15];	// default form flag for the race
	long parts[15];	// default parts for the race
	int arms;
	int legs;
};

//
// Attribute bonus structures.
//

struct str_app_type
{
	sh_int topenetrate;
	sh_int todam;
	sh_int carry;
	sh_int wield;
};

struct int_app_type
{
	sh_int learn;
};

struct wis_app_type
{
	sh_int practice;
};

struct dex_app_type
{
	sh_int defensive;
	sh_int carry;
	sh_int tohit;
	sh_int armor;
};

struct con_app_type
{
	sh_int hitp;
	sh_int shock;
};

extern const struct pc_race_type pc_race_table[];
extern const std::vector<race_type> race_table;
extern const struct str_app_type str_app[26];
extern const struct int_app_type int_app[26];
extern const struct	wis_app_type wis_app[26];
extern const struct dex_app_type dex_app[26];
extern const struct con_app_type con_app[26];

#endif /* CHARDEF_H */
