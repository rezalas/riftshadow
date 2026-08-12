#ifndef MOREMAGIC_H
#define MOREMAGIC_H

#include "entity/fwd.h"
#include "entity/spell_target.h"


//
// LOCAL FUNCTIONS
//

void spell_enlarge (int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode);
void spell_sunray (int sn,int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode);
void spell_cleanse (int sn,int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode);
bool cleansed (CHAR_DATA *ch, CHAR_DATA *victim,int diffmodifier, int sn);
int get_affect_level (CHAR_DATA *ch, int sn);
void spell_cure_deafness (int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode);
void spell_remove_paralysis (int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode);
void spell_awaken (int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode);
void spell_resist_heat (int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode);
void spell_resist_cold (int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode);
void spell_resist_lightning (int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode);
void spell_resist_mental (int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode);
void spell_resist_acid (int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode);
void spell_resist_negative (int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode);
void spell_group_teleport (int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode);
void spell_soften (int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode);
void spell_rejuvenate (int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode);				/* Cleric Rejuvenate -Dioxide */
void spell_fatigue (int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode);
void spell_strength (int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode);
void spell_remove_taint (int sn,int level,CHAR_DATA *ch,SpellTarget vo, CastMode mode);
void spell_worldbind (int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode);
void waterbreath_end (CHAR_DATA *ch, AFFECT_DATA *af);
void spell_waterbreath (int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode);

#endif /* MOREMAGIC_H */
