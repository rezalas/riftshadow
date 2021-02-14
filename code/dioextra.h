#ifndef DIOEXTRA_H
#define DIOEXTRA_H

#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mysql.h>
#include <iterator>
#include "merc.h"
#include "handler.h"
#include "recycle.h"
#include "tables.h"
#include "lookup.h"
#include "save.h"
#include "act_comm.h"
#include "act_info.h"
#include "fight.h"
#include "mspec.h"
#include "comm.h"
#include "act_wiz.h"
#include "interp.h"
#include "skills.h"
#include "db.h"
#include "devextra.h"
#include "magic.h"


#define TEMP_PKTRACK_FILE "../temp/temppktrack.tmp"

//
// LOCAL FUNCTIONS
//

void do_crumble (CHAR_DATA *ch, char *argument);
void do_ghost (CHAR_DATA *ch, char *argument);
void do_praclist (CHAR_DATA *ch, char *argument);
void do_affrem (CHAR_DATA *ch, char *argument);
void do_divine_intervention (CHAR_DATA *ch, char *argument);
void do_ccb (CHAR_DATA *ch, char *argument);
void do_powers (CHAR_DATA *ch, char *argument);
bool check_shroud_of_light (CHAR_DATA *ch, CHAR_DATA *victim);
void do_heroimm (CHAR_DATA *ch, char *argument);
void check_keen_vision (CHAR_DATA *ch, CHAR_DATA *victim);
void report_cabal_items (CHAR_DATA *ch, char *argument);
void easy_uninduct (CHAR_DATA *ch);
void do_lag (CHAR_DATA *ch, char *argument);
int get_spell_aftype (CHAR_DATA *ch);
void do_finger (CHAR_DATA *ch, char *argument);
void update_pc_last_fight (CHAR_DATA *ch,CHAR_DATA *ch2);
/* horrid Cabal track code */
void do_ctrack (CHAR_DATA *ch, char *argument);
char *get_login (CHAR_DATA *ch, FILE *fpChar2);
void chomp (char *str);
void chompr (char *str);
void chop (char *str);
bool can_live_in (CHAR_DATA *ch,int hometown);
int count_carried (CHAR_DATA *ch, bool limited);
bool auto_check_multi (DESCRIPTOR_DATA *d_check, char *host);
void do_pload (CHAR_DATA *ch,char *argument);
void do_damage (CHAR_DATA *ch,char *argument);
void zone_echo (AREA_DATA *area, char *echo);
bool old_is_adjacent_area (AREA_DATA *area, AREA_DATA *area2);
bool is_adjacent_area (AREA_DATA *area, AREA_DATA *area2);
bool is_adj_range (AREA_DATA *area, AREA_DATA *area2, int range);

#endif /* DIOEXTRA_H */
