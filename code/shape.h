#ifndef SHAPE_H
#define SHAPE_H

#include "merc.h"

#define MAX_FORM			22

#define FORM_QUEST			0
#define FORM_PLANT			1
#define FORM_MYTH_ANIMAL	2
#define FORM_EXTRAPLANAR	3

#define FORM_QUEST			0
#define FORM_OFFENSE		1
#define FORM_DEFENSE		2
#define FORM_UTILITY		3

// TODO: FIND OUT WHERE THIS IS IMPLEMENTED
extern const struct form_type form_table[MAX_FORM];

struct form_type
{
	char *name;							// Form name, i.e. lion
	char *short_descr;					// Form short, i.e. a grizzled ram
	char *long_descr;					// form long: A lion whips around to glare at you threateningly. */
	short form_stats[5];				// form stats, increase this if a new stat is added
	short armor[4];					// form ACs
	short dam_mod;						// form dam mod, 100 is normal dam
	short attacks;						// max attacks form gets per round
	char *attack_type;					// damage name, i.e. claw, must be on attack_table
	short parry_modifier;				// how difficult is it to parry? (% modifier, 50 is 50% easier, -50 is 50% harder)
	short dice_number;					// damage dice number, in 5d4+35 the 5
	short dice_type;					// damage dice type, in 5d4+35 the 4
	short dice_add;					// damage dice addition, the 35
	short hit_roll;					// form hitroll
	short saves_spell;					// form saves
	long imm_flags[MAX_BITVECTOR];		// form immunities
	long res_flags[MAX_BITVECTOR];		// form resistances
	long vuln_flags[MAX_BITVECTOR];		// form vulns
	long affected_by[MAX_BITVECTOR];	// form affected_by	
	long specials[MAX_BITVECTOR];		// form special bits, like juzam terror or whatever
	short form_category;				// quest, plant, extraplanar, animal
	short form_foci;					// offense, quest, utility, etc.
	char *yell;							// what the form echoes when it's attacked
	char *description;					// form description
	char *spec_one;						// name of first special, like "rake"
	char *spec_two;						// second special
	char *spec_three;					// third special
};

#endif /* SHAPE_H */
