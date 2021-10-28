#ifndef HEALER_H
#define HEALER_H

#include "merc.h"

//
// LOCAL FUNCTIONS
//

void spell_healing_sleep (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void healing_sleep_end (CHAR_DATA *ch, AFFECT_DATA *af);

#endif /* HEALER_H */
