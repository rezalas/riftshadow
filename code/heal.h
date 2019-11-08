#ifndef HEAL_H
#define HEAL_H

#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "merc.h"
#include "magic.h"
#include "devextra.h"

DECLARE_DO_FUN(do_say);

float get_mod_cost(CHAR_DATA *ch, int cost);
/* This is all from Rom except the restoration spell */
void do_heal (CHAR_DATA *ch, char *argument);

#endif /* HEAL_H */
