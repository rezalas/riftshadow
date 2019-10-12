#ifndef WARRIOR_H
#define WARRIOR_H

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "merc.h"
#include "magic.h"
#include "recycle.h"
#include "db.h"
#include "lookup.h"
#include "tables.h"

extern bool	is_wielded (CHAR_DATA *ch, int weapon, int type);
extern char * flag_name_lookup (long bitv, const struct flag_type *flag_table);
extern CHAR_DATA *get_char_room	(CHAR_DATA *ch, char *argument);

int style_lookup (char *argument);
int style_count (long bits[]);
void do_specialize (CHAR_DATA *ch, char *argument);
void do_style (CHAR_DATA *ch, char *argument);
void do_entrap (CHAR_DATA *ch, char *argument);
void do_hobble (CHAR_DATA *ch, char *argument);
void do_crippling_blow (CHAR_DATA *ch, char *argument);
void do_gouge (CHAR_DATA *ch, char *argument);
void do_bleed (CHAR_DATA *ch, char *argument);
int check_posture (CHAR_DATA *ch);
void do_posture (CHAR_DATA *ch, char *argument);
void posture_pulse (CHAR_DATA *ch, AFFECT_DATA *af);
void do_unbalance (CHAR_DATA *ch, char *argument);
void do_uppercut (CHAR_DATA *ch, char *argument);
int skirmisher_max_weapweight (CHAR_DATA *ch);
void do_dart (CHAR_DATA *ch, char *argument);
void do_impale (CHAR_DATA *ch, char *argument);
void do_hurl (CHAR_DATA *ch, char *argument);
void do_overhead (CHAR_DATA *ch, char *argument);
void do_extract (CHAR_DATA *ch, char *argument);
void do_exchange( CHAR_DATA *ch, char *argument);
/* Returns total weight of PC's armor for use with dragoon skills. */
int	armor_weight (CHAR_DATA *ch);
void do_charge (CHAR_DATA *ch, char *argument);
bool check_ease (CHAR_DATA *ch);
void do_shieldbash (CHAR_DATA *ch, char *argument);
void do_brace (CHAR_DATA *ch, char *arg);
void brace_helper_undo (CHAR_DATA *ch, float *braceptr);
void do_shatter (CHAR_DATA *ch, char *argument);
void do_whirlwind (CHAR_DATA *ch, char *argument);
void check_batter (CHAR_DATA *ch);
bool check_entwine (CHAR_DATA *ch, int type);
void do_entwine (CHAR_DATA *ch, char *argument);
void do_uncoil (CHAR_DATA *ch, char *argument);
void do_pull (CHAR_DATA *ch, char *argument);
char *get_assess_line (long where[], int percent);
void do_assess (CHAR_DATA *ch, char *argument);
bool check_exploit_armor_break (CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *exposed);
void do_exploit (CHAR_DATA *ch, char *argument);
void do_offhand (CHAR_DATA *ch, char *arg);
void do_drive (CHAR_DATA *ch, char *argument);
void do_dash (CHAR_DATA *ch, char *argument);
void do_concuss (CHAR_DATA *ch,char *argument);
void do_retreat (CHAR_DATA *ch, char *arg);
void execute_retreat (CHAR_DATA *ch, int *direction);
void do_disrupt_formation (CHAR_DATA *ch, char *arg);
void do_leadership (CHAR_DATA *ch,char *argument);
bool check_leadership_save (CHAR_DATA *ch, int skill);
void check_leadership_affect (CHAR_DATA *ch);
int check_outflank (CHAR_DATA *ch);
void do_outflank (CHAR_DATA *ch, char *argument);
bool outflank_me (CHAR_DATA *ch, int dir);

#endif /* WARRIOR_H */
