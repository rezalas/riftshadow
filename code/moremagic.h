#ifndef MOREMAGIC_H
#define MOREMAGIC_H

#include "merc.h"


//
// LOCAL FUNCTIONS
//

void spell_enlarge (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_sunray (int sn,int level, CHAR_DATA *ch, void *vo, int target);
void spell_cleanse (int sn,int level, CHAR_DATA *ch, void *vo, int target);
bool cleansed (CHAR_DATA *ch, CHAR_DATA *victim,int diffmodifier, int sn);
int get_affect_level (CHAR_DATA *ch, int sn);
void spell_cure_deafness (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_remove_paralysis (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_awaken (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_resist_heat (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_resist_cold (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_resist_lightning (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_resist_mental (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_resist_acid (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_resist_negative (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_group_teleport (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_soften (int sn, int level, CHAR_DATA *ch, void *vo,int target);
void spell_rejuvenate (int sn, int level, CHAR_DATA *ch, void *vo,int target);				/* Cleric Rejuvenate -Dioxide */
void spell_fatigue (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_strength (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_remove_taint (int sn,int level,CHAR_DATA *ch,void *vo,int target);
void spell_worldbind (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void waterbreath_end (CHAR_DATA *ch, AFFECT_DATA *af);
void spell_waterbreath (int sn, int level, CHAR_DATA *ch, void *vo, int target);

#endif /* MOREMAGIC_H */
