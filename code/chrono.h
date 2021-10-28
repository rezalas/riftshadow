#ifndef CHRONO_H
#define CHRONO_H

#include "merc.h"

//
// LOCAL FUNCTIONS
//

void spell_stasis_wall (int sn, int level, CHAR_DATA *ch, void *vo,int target );
bool trigger_stasis_wall (void *vo,void *vo2,void *vo3,void *vo4);
bool activate_stasis_wall (void *vo,void *vo2,void *vo3,void *vo4);	//list your rune trigs here for good reference
void draw_rune (void *vo, void *vo2); //attempt to add a rune that's just been initiated
void do_rune (CHAR_DATA *ch, char *argument );
RUNE_DATA *find_rune (void *vo, int target_type, int trigger_type, RUNE_DATA *rune_prev);
void extract_rune (RUNE_DATA *rune);
void apply_rune (RUNE_DATA *rune); //make new copy of rune and apply it to target

#endif /* CHRONO_H */
