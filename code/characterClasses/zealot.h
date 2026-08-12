#ifndef ZEALOT_H
#define ZEALOT_H

#include "../entity/fwd.h"
#include "../entity/spell_target.h"

//
// LOCAL FUNCTIONS
//

void spell_infidels_weight (int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode);
int get_bv_stage (CHAR_DATA *ch);
void spell_burning_vision (int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode);
void burning_vision_tick (CHAR_DATA *ch, AFFECT_DATA *af);
void spell_divine_malison (int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode);

#endif /* ZEALOT_H */
