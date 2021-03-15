#ifndef HEALER_H
#define HEALER_H

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "merc.h"
#include "handler.h"
#include "interp.h"
#include "tables.h"
#include "recycle.h"
#include "magic.h"
#include "db.h"
#include "lookup.h"
#include "comm.h"
#include "devextra.h"
#include "const.h"

//
// LOCAL FUNCTIONS
//

void spell_healing_sleep (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void healing_sleep_end (CHAR_DATA *ch, AFFECT_DATA *af);

#endif /* HEALER_H */
