#ifndef ALIAS_H
#define ALIAS_H

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "newmem.h"
#include "comm.h"
#include "interp.h"
#include "db.h"
#include "utility.h"

//
// LOCAL FUNCTIONS
//

/* does aliasing and other fun stuff */
void substitute_alias(DESCRIPTOR_DATA *d, char *argument);
void do_alia(CHAR_DATA *ch, char *argument);
void do_alias(CHAR_DATA *ch, char *argument);
void do_unalias(CHAR_DATA *ch, char *argument);

#endif /* ALIAS_H */
