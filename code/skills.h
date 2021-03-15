#ifndef SKILLS_H
#define SKILLS_H

#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "merc.h"
#include "handler.h"
#include "magic.h"
#include "recycle.h"
#include "tables.h"
#include "update.h"
#include "devextra.h"
#include "act_comm.h"
#include "chardef.h"
#include "const.h"

//
// LOCAL FUNCTIONS
//

void do_gain (CHAR_DATA *ch, char *argument);
/* RT spells and skills show the players spells (or skills) */
void do_spells (CHAR_DATA *ch, char *argument);
void do_skills (CHAR_DATA *ch, char *argument);
/* shows skills, groups and costs (only if not bought) */
void list_group_costs (CHAR_DATA *ch);
void list_group_chosen (CHAR_DATA *ch);
int exp_per_level (CHAR_DATA *ch);
/* this procedure handles the input parsing for the skill generator */
/* commented out for being lame
bool parse_gen_groups (CHAR_DATA *ch,char *argument);
*/	
/* shows all groups, or the sub-members of a group */
void do_groups (CHAR_DATA *ch, char *argument);
/* check improve for styles */
void check_style_improve (CHAR_DATA *ch, int style, int multiplier);
/* checks for skill improvement */
void check_improve (CHAR_DATA *ch, int sn, bool success, int multiplier);
/* returns a group index number given the name */
int group_lookup (const char *name);
int gn_skill_lookup (int sn);
bool style_check (int sn, int style);
bool group_skill_lookup (int gn, int sn);
/* recursively adds a group given its number -- uses group_add */
void gn_add (CHAR_DATA *ch, int gn);
/* recusively removes a group given its number -- uses group_remove */
void gn_remove (CHAR_DATA *ch, int gn);
/* use for processing a skill or group for addition  */
void group_add (CHAR_DATA *ch, const char *name, bool deduct);
/* used for processing a skill or group for deletion -- no points back! */
void group_remove (CHAR_DATA *ch, const char *name);

#endif /* SKILLS_H */
