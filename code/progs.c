/* Dev's r33t new progs system.  Bow.  */

#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "magic.h"
#include "interp.h"
#include "recycle.h"
#include "tables.h"
#include "lookup.h"

char * flag_name_lookup args((long bitv, const struct flag_type *flag_table));

