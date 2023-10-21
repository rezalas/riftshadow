#ifndef DB2_H
#define DB2_H

#include <stdio.h>
#include "merc.h"

//
// Structure for a social in the socials table.
//

struct social_type
{
	char name[20];
	char *char_no_arg;
	char *others_no_arg;
	char *char_found;
	char *others_found;
	char *vict_found;
	char *char_not_found;
	char *char_auto;
	char *others_auto;
};

extern struct social_type social_table[MAX_SOCIALS];
extern int social_count;

//
// LOCAL FUNCTIONS
//

//
// snarf a socials file
//

void load_socials (FILE *fp);

//
// Snarf an IMprog section
// format: M <mob vnum> <program type> <function name>
//

void load_improgs (FILE *fp);
void load_specs (FILE *fp);
void load_mobs (FILE *fp);
void bugout (char *reason);
void bug_exit (char *file, int nLine);


//
// Snarf an obj section. dev style
//

void load_objs (FILE *fp);

#endif /* DB2_H */
