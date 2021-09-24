#ifndef APROG_H
#define APROG_H

/* IMPROGS Expansion: AREA PROGS (Eladrian) */

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "magic.h"
#include "interp.h"
#include "recycle.h"
#include "tables.h"
#include "dioextra.h"
#include "db.h"
#include "devextra.h"
#include "utility.h"

extern const struct improg_type aprog_table[];

//
// LOCAL FUNCTIONS
//

bool aprog_unset (AREA_DATA *area, const char *progtype, const char *name);
void aprog_set (AREA_DATA *area, const char *progtype, const char *name);
void tick_prog_academy_reset (AREA_DATA *area);
void tick_prog_ilopheth (AREA_DATA *area);
void reset_prog_cimsewer (AREA_DATA *area);
void pulse_prog_ruins_shark (AREA_DATA *area);
void sun_prog_ilopheth (AREA_DATA *area);
void sun_prog_caelumaedan (AREA_DATA *area);
void sun_prog_glasstower (AREA_DATA *area);
void myell_prog_lawful_city (AREA_DATA *area, CHAR_DATA *ch, CHAR_DATA *victim);

#endif /* APROG_H */
