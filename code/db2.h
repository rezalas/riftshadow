#ifndef DB2_H
#define DB2_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>
#include <iterator>
#include "merc.h"
#include "handler.h"
#include "db.h"
#include "lookup.h"
#include "tables.h"
#include "recycle.h"
#include "spec.h"
#include "warrior.h"
#include "misc.h"
#include "recycle.h"
#include "iprog.h"
#include "mprog.h"
#include "rprog.h"
#include "aprog.h"

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
