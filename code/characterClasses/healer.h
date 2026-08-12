#ifndef HEALER_H
#define HEALER_H

#include "../entity/fwd.h"
#include "../entity/spell_target.h"

//
// LOCAL FUNCTIONS
//

void spell_healing_sleep (int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode);
void healing_sleep_end (CHAR_DATA *ch, AFFECT_DATA *af);

#endif /* HEALER_H */
