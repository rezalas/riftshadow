#ifndef MSPEC_H
#define MSPEC_H

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <algorithm>
#include "merc.h"
#include "handler.h"
#include "spec.h"
#include "tables.h"
#include "mprog.h"
#include "act_comm.h"
#include "act_move.h"
#include "fight.h"
#include "newmem.h"
#include "comm.h"
#include "interp.h"
#include "db.h"
#include "devextra.h"

extern va_list arglist;

DECLARE_SPEC(mspec_academy_smith);
DECLARE_SPEC(mspec_academy_greeters);
DECLARE_SPEC(mspec_horde_tanner);
DECLARE_SPEC(mspec_scared_soldier);
DECLARE_SPEC(mspec_academy_pet);
DECLARE_SPEC(mspec_minotaur_zombie);
DECLARE_SPEC(mspec_fallendesert_spirits);

DO_FUN(do_give);

#define CIM_FOOD 2605
#define CIM_WATER 2538
#define CIM_BOAT 2625
#define apet_find_guild() 	vnum = \
								cclass == CLASS_WARRIOR ? 2400 : \
								cclass == CLASS_THIEF ? 2581 : \
								cclass == CLASS_ZEALOT ? 2542 : \
								cclass == CLASS_PALADIN ? 2540 : \
								cclass == CLASS_ANTI_PALADIN ? 2502 : \
								cclass == CLASS_HEALER ? 2544 : \
								cclass == CLASS_NECROMANCER ? 2421 : \
								cclass == CLASS_SORCERER ? 2489 : \
								cclass == CLASS_ASSASSIN ? 2580 : 0

// Fallen Desert Progs
int mspec_fallendesert_spirits (long event_vector, ...);
int mspec_minotaur_zombie (long event_vector, ...);
int mspec_academy_greeters (long event_vector, ...);
int mspec_horde_tanner (long event_vector, ...);
int mspec_academy_pet (long event_vector, ...);

void create_academy_pet(CHAR_DATA *ch);
void apet_force(CHAR_DATA *ch, const char *cmd, int delay);
void apet_at_room(CHAR_DATA *ch, int vnum);
void apet_walk_to_room(CHAR_DATA *ch, int vnum);

#endif /* MSPEC_H */
