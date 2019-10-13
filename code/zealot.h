#ifndef ZEALOT_H
#define ZEALOT_H

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "merc.h"
#include "interp.h"
#include "tables.h"
#include "recycle.h"
#include "magic.h"
#include "db.h"
#include "lookup.h"

void spell_infidels_weight (int sn, int level, CHAR_DATA *ch, void *vo, int target);
int get_bv_stage (CHAR_DATA *ch);
void spell_burning_vision (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void burning_vision_tick (CHAR_DATA *ch, AFFECT_DATA *af);
void spell_divine_malison (int sn, int level, CHAR_DATA *ch, void *vo, int target);

#endif /* ZEALOT_H */
