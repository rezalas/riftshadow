#ifndef DEVEXTRA_H
#define DEVEXTRA_H

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mysql.h>
#include <algorithm>
#include <iterator>
#include "merc.h"
#include "magic.h"
#include "recycle.h"
#include "db.h"
#include "lookup.h"
#include "tables.h"
#include "time.h"
#include "interp.h"
#include "help.h"
#include "save.h"
#include "cabal.h"
#include "magic.h"
#include "act_info.h"
#include "config.h"
#include "olc_act.h"
#include "bit.h"
#include "chardef.h"
#include "const.h"
#include "utility.h"

#define LOGIN_LOG_FILE			"logins.txt"
#define BOUNTY_LOG_FILE			"bounties.txt"
//#define TEMP_GREP_RESULTS_TWO	"../temp/tempgrepresults2.tmp"
#define HISTORY_FILE			"history.txt"

#define COSM_UNDER(a,b,c,d)		obj_to_obj(make_cosmetic(a,b,c,d),obj);
#define COSM_WEAR(a,b)			obj_to_obj(make_cosmetic(a,b,NULL,NULL),obj);
#define COSM_NORM(a,b,c)		obj_to_obj(make_cosmetic(a,b,NULL,c),obj);


//
// LOCAL FUNCTIONS
//

bool IS_IMP (CHAR_DATA *ch);
float calculate_inflation (void);
char *escape_string (char *string);
void do_pswitch (CHAR_DATA *ch, char *argument);
void do_gold (CHAR_DATA *ch, char *argument);
void do_clean (CHAR_DATA *ch, char *argument);
void clean_mud (void);
void do_listoffer (CHAR_DATA *ch, char *argument);
void do_offer (CHAR_DATA *ch, char *argument);
void do_sitetrack (CHAR_DATA *ch, char *argument);
void comment_end_fun (CHAR_DATA *ch, char *argument);
void show_database_info (CHAR_DATA *ch, char *argument);
void do_demo (CHAR_DATA *ch, char *name);
void delete_char (char *name, bool save_pfile);
MYSQL *open_fconn (void);
MYSQL_ROW one_query_row (char *query);
MYSQL_ROW one_fquery_row (char *query);
MYSQL_RES *one_query_res (char *query);
int one_query_count (char *query);
void one_query (char *query);
void enter_text (CHAR_DATA *ch, DO_FUN *end_fun);
char *log_time (void);
MYSQL *open_conn (void);
void one_fquery (char *query);
void plug_graveyard (CHAR_DATA *ch, int type);
void do_pktrack (CHAR_DATA *ch, char *argument);
bool trusts (CHAR_DATA *ch, CHAR_DATA *victim);
void mob_recho (CHAR_DATA *ch, char *argument );
void area_echo (CHAR_DATA *ch, char *echo);
void rarea_echo (ROOM_INDEX_DATA *room, char *echo);
void outdoors_echo (AREA_DATA *area, char *echo);
bool check_volley (CHAR_DATA *ch, CHAR_DATA *victim);
char *get_char_color (CHAR_DATA *ch, char *event);
int get_event_number (char *name);
char *get_color_name (char *name);
char *color_name_to_ascii (char *name);
char *END_COLOR (CHAR_DATA *ch);
void ADD_WAIT_STATE (CHAR_DATA *ch, int npulse);
void WAIT_STATE (CHAR_DATA *ch, int npulse);
void LAG_CHAR (CHAR_DATA *ch, int npulse);
void bounty_log (char *string);
void do_credits (CHAR_DATA *ch, char *argument);
void bounty_credit (CHAR_DATA *ch, int credit);
void bounty_cb (char *string);
void do_topbounties (CHAR_DATA *ch, char *argument);
void do_bounty (CHAR_DATA *ch, char *argument);
void pay_bounty (CHAR_DATA *ch, CHAR_DATA *victim);
void do_rchanges (CHAR_DATA *ch, char *argument);
void do_affto (CHAR_DATA *ch, char *argument);
char *flags_to_string (CHAR_DATA *ch, const struct flag_type *showflags, int flagsperline);
void do_ltrack (CHAR_DATA *ch, char *argument);
void do_assess_old (CHAR_DATA *ch, char *argument);
void do_supps (CHAR_DATA *ch, char *argument );
void do_commune (CHAR_DATA *ch, char *argument );
void do_call (CHAR_DATA *ch, char *argument );
void do_snare (CHAR_DATA *ch, char *argument );
void mob_tell (CHAR_DATA *mob, CHAR_DATA *ch, char *tell);
void do_forcewear (CHAR_DATA *ch, char *argument);
void do_forceremove (CHAR_DATA *ch, char *argument);
void do_createcosmetic (CHAR_DATA *ch, char *argument);
OBJ_DATA *make_cosmetic (char *name, char *wearloc, char *underloc, char *cosmeticloc);
void pulse_prog_repop_container (OBJ_DATA *obj, bool isTick);

#endif /* DEVEXTRA_H */
