#ifndef HEAL_H
#define HEAL_H

#include "merc.h"

//
// LOCAL FUNCTIONS
//

float get_mod_cost(CHAR_DATA *ch, int cost);
/* This is all from Rom except the restoration spell */
void do_heal (CHAR_DATA *ch, char *argument);

#endif /* HEAL_H */
