#ifndef PALADIN_H
#define PALADIN_H

#include "merc.h"

//
// LOCAL FUNCTIONS
//

void spell_rites_of_preparation (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_spiritual_hammer (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void do_turn_undead (CHAR_DATA *ch, char *argument);
bool check_benevolence (CHAR_DATA *ch, CHAR_DATA *keeper);
bool check_intercept (CHAR_DATA *ch, CHAR_DATA *victim, CHAR_DATA *paladin, int dt);
void spell_blinding_orb (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_voice_of_damnation (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_seraphic_mantle (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_arms_of_light (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_arms_of_purity (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_arms_of_wrath (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_arms_of_judgement (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void do_strike_of_virtue (CHAR_DATA *ch, char *argument);
void spell_divine_frenzy (int sn,int level, CHAR_DATA *ch, void *vo, int target);
void do_group_retreat (CHAR_DATA *ch, char *argument);
void do_valiant_charge (CHAR_DATA *ch, char *argument);
void spell_awe (int level, int sn, CHAR_DATA *ch, void *vo, int target);
void spell_shield_of_faith (int level, int sn, CHAR_DATA *ch, void *vo, int target);
void spell_holy_shroud (int level, int sn, CHAR_DATA *ch, void *vo, int target);
int check_arms (CHAR_DATA *ch, OBJ_DATA *wield, bool bOncePerRound);
void spell_empathy (int level, int sn, CHAR_DATA *ch, void *vo, int target);
void empathy_end (CHAR_DATA *ch, AFFECT_DATA *af);
void spell_tower_of_fortitude (int level, int sn, CHAR_DATA *ch, void *vo, int target);
void spell_indomitable_spirit (int level, int sn, CHAR_DATA *ch, void *vo, int target);
void ispirit_beat (CHAR_DATA *ch, AFFECT_DATA *af);
void ispirit_end (CHAR_DATA *ch, AFFECT_DATA *af);
void spell_altruism (int level, int sn, CHAR_DATA *ch, void *vo, int target);

#endif /* PALADIN_H */
