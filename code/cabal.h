#ifndef CABAL_H
#define CABAL_H

#include "entity/fwd.h"
#include "entity/spell_target.h"

//
// LOCAL FUNCTIONS
//

bool check_horde (CHAR_DATA *ch);
void update_cskills (CHAR_DATA *ch);
void do_storytell (CHAR_DATA *ch, char *argument);
/*
void spell_fervor(int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode)
*/
void spell_epic (int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode);
void spell_calm (int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode);
void spell_rage (int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode);
void spell_scourge (int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode);		/* Scourge for Arcanas -Dev */
void spell_hunters_vision (int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode);
void spell_hire_mercenary (int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode);
void spell_hunters_strength (int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode);
void spell_hunters_awareness (int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode);
void spell_web (int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode);
void spell_informant (int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode);
void do_howl (CHAR_DATA *ch, char *argument);
void spell_mana_transfer (int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode );
void spell_scribe (int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode );
void spell_deny_magic (int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode );
bool check_deny_magic (CHAR_DATA *ch);
void spell_bane (int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode );
void spell_repose (int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode );
void spell_medicine (int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode );
void spell_horde_communion (int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode);
void communion_tick (CHAR_DATA *ch, AFFECT_DATA *af);
void do_exile (CHAR_DATA *ch, char *argument );
void spell_piety (int sn,int level,CHAR_DATA *ch,SpellTarget vo, CastMode mode);
void spell_fervor (int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode);
void spell_spiritual_healing (int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode);
void spell_shroud_of_light (int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode);
void spell_crimson_martyr (int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode);
void retribution_tick (CHAR_DATA *ch, AFFECT_DATA *af);
void spell_retribution (int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode);
void do_phalanx (CHAR_DATA *ch, char *argument);
void spell_safehaven (int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode);

#endif /* CABAL_H */
