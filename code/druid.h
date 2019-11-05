#ifndef DRUID_H
#define DRUID_H

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

/*
void spell_imbue_stone(int sn, int level, CHAR_DATA *ch, void *vo, int target);
*/

void spell_tangleroot(int sn, int level, CHAR_DATA *ch, void *vo, int target);

#endif /* DRUID_H */