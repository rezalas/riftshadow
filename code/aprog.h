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

DECLARE_APROG_FUN_RESET(reset_prog_cimsewer);
DECLARE_APROG_FUN_PULSE(pulse_prog_ruins_shark);
DECLARE_APROG_FUN_SUN(sun_prog_caelumaedan);
DECLARE_APROG_FUN_SUN(sun_prog_glasstower);
DECLARE_APROG_FUN_TICK(tick_prog_ilopheth);
DECLARE_APROG_FUN_SUN(sun_prog_ilopheth);
DECLARE_APROG_FUN_MYELL(myell_prog_lawful_city);
DECLARE_APROG_FUN_TICK(tick_prog_academy_reset);

const struct improg_type aprog_table[] = 
{
	{ "reset_prog", "reset_prog_cimsewer", (void*)reset_prog_cimsewer, "Sewers of Cimar"},
	{ "pulse_prog", "pulse_prog_ruins_shark", (void*)pulse_prog_ruins_shark, "Ruins of the Deep"},
	{ "sun_prog", "sun_prog_caelumaedan", (void*)sun_prog_caelumaedan, "None" },
	{ "sun_prog", "sun_prog_glasstower", (void*)sun_prog_glasstower, "None" },
	{ "sun_prog", "sun_prog_ilopheth", (void*)sun_prog_ilopheth, "Valley of Ilopheth"},
	{ "tick_prog", "tick_prog_academy_reset", (void *)tick_prog_academy_reset, "None" },
	{ "tick_prog", "tick_prog_ilopheth", (void*)tick_prog_ilopheth, "Valley of Ilopheth"},
	{ "myell_prog", "myell_prog_lawful_city", (void*)myell_prog_lawful_city, "Cimar"},
	{ NULL, NULL, NULL, NULL},
};


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
