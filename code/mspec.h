#ifndef MSPEC_H
#define MSPEC_H

#include "entity/fwd.h"
#include "spec.h"

// Handlers defined in quest.c. The program table in mspec.c refers to them.
int academy_smith_greet(CHAR_DATA *ch, CHAR_DATA *mob);
int academy_smith_speech(CHAR_DATA *ch, CHAR_DATA *mob, char *argument);
int academy_smith_give(CHAR_DATA *ch, CHAR_DATA *mob, OBJ_DATA *obj);
int scared_soldier_greet(CHAR_DATA *ch, CHAR_DATA *mob);

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

void create_academy_pet(CHAR_DATA *ch);
void apet_force(CHAR_DATA *ch, const char *cmd, int delay);
void apet_at_room(CHAR_DATA *ch, int vnum);
void apet_walk_to_room(CHAR_DATA *ch, int vnum);

#endif /* MSPEC_H */
