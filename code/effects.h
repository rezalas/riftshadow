#ifndef EFFECTS_H
#define EFFECTS_H

#include "entity/spell_target.h"


//
// LOCAL FUNCTIONS
//

// Each applies its element to whatever the target holds. A room spreads the
// effect to the objects lying in it. The kind of target is carried by the
// target itself, so there is no separate tag to keep in agreement with it.
// Each is documented at its definition in effects.c.
void acid_effect (SpellTarget target, int level, int dam);
void cold_effect (SpellTarget target, int level, int dam);
void fire_effect (SpellTarget target, int level, int dam);
void poison_effect (SpellTarget target, int level, int dam);
void shock_effect (SpellTarget target, int level, int dam);

#endif /* EFFECTS_H */
