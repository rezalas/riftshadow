#ifndef ACT_INFO_H
#define ACT_INFO_H

#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#ifdef _WIN32
    #include <windows.h>
    #include <wincrypt.h>
#else
    #include <crypt.h>
#endif

#include "merc.h"
#include "handler.h"
#include "magic.h"
#include "recycle.h"
#include "tables.h"
#include "lookup.h"
#include "act_comm.h"
#include "cabal.h"
#include "devextra.h"
#include "dioextra.h"
#include "fight.h"
#include "newmem.h"
#include "comm.h"
#include "act_wiz.h"
#include "update.h"
#include "interp.h"
#include "db.h"
#include "zealot.h"

/* command procedures needed */
DECLARE_DO_FUN(do_exits);
DECLARE_DO_FUN(do_look);
DECLARE_DO_FUN(do_help);
DECLARE_DO_FUN(do_affects);
DECLARE_DO_FUN(do_play);
DECLARE_DO_FUN(do_tell);
DECLARE_DO_FUN(descr_end_fun);
DECLARE_DO_FUN(do_weather);

#define DEBUG_LOG_FILE	"debug.txt"
#define GOD_LOG_FILE	"glog.txt"



/*
 * Local functions.
 */
char * format_obj_to_char (OBJ_DATA *obj, CHAR_DATA *ch, bool fShort);
void show_list_to_char (OBJ_DATA *list, CHAR_DATA *ch, bool fShort, bool fShowNothing);
void show_char_to_char_0 (CHAR_DATA *victim, CHAR_DATA *ch);
void show_char_to_char_1 (CHAR_DATA *victim, CHAR_DATA *ch);
void show_char_to_char_2 (CHAR_DATA *victim, CHAR_DATA *ch);
void show_char_to_char_3 (CHAR_DATA *victim, CHAR_DATA *ch);
void show_char_to_char (CHAR_DATA *list, CHAR_DATA *ch);
bool check_blind (CHAR_DATA *ch);

bool can_shapeshift (CHAR_DATA *ch, int form_num);
void shapeshift_revert (CHAR_DATA *ch);
bool isCabalItem (OBJ_DATA *obj);
bool isNewbie (CHAR_DATA *ch);
void print_chessboard (CHAR_DATA *ch);
char * flag_name_lookup (long bitv, const struct flag_type *flag_table);

char *format_obj_to_char (OBJ_DATA *obj, CHAR_DATA *ch, bool fShort);
/*
 * Show a list to a character.
 * Can coalesce duplicated items.
 */
void show_list_to_char (OBJ_DATA *list, CHAR_DATA *ch, bool fShort, bool fShowNothing);
void show_char_to_char_0 (CHAR_DATA *victim, CHAR_DATA *ch);
void show_char_to_char_1 (CHAR_DATA *victim, CHAR_DATA *ch);
void show_char_to_char_2 (CHAR_DATA *victim, CHAR_DATA *ch);
void show_char_to_char_3 (CHAR_DATA *victim, CHAR_DATA *ch);
void show_char_to_char (CHAR_DATA *list, CHAR_DATA *ch);
bool check_blind (CHAR_DATA *ch);
/* changes your scroll */
void do_scroll (CHAR_DATA *ch, char *argument);
/* RT does socials */
void do_socials (CHAR_DATA *ch, char *argument);
/* RT Commands to replace news, motd, imotd, etc from ROM */
void do_motd (CHAR_DATA *ch, char *argument);
void do_imotd (CHAR_DATA *ch, char *argument);
void do_rules (CHAR_DATA *ch, char *argument);
void do_story (CHAR_DATA *ch, char *argument);
void do_wizlist (CHAR_DATA *ch, char *argument);
/* RT this following section holds all the auto commands from ROM, as well as
   replacements for config */
void do_autolist (CHAR_DATA *ch, char *argument);
void do_color (CHAR_DATA *ch, char *argument);
void do_autoabort (CHAR_DATA *ch, char *argument);
void do_autoassist (CHAR_DATA *ch, char *argument);
void do_autoexit (CHAR_DATA *ch, char *argument);
void do_autogold (CHAR_DATA *ch, char *argument);
void do_autoloot (CHAR_DATA *ch, char *argument);
void do_autosac (CHAR_DATA *ch, char *argument);
void do_autosplit (CHAR_DATA *ch, char *argument);
void do_brief (CHAR_DATA *ch, char *argument);
void do_compact (CHAR_DATA *ch, char *argument);
void do_show (CHAR_DATA *ch, char *argument);
void do_prompt (CHAR_DATA *ch, char *argument);
void do_combine (CHAR_DATA *ch, char *argument);
void do_nofollow (CHAR_DATA *ch, char *argument);
void do_nosummon (CHAR_DATA *ch, char *argument);
void do_glance (CHAR_DATA *ch, char *argument);
void do_examine (CHAR_DATA *ch, char *argument);
bool show_altdesc (ROOM_INDEX_DATA *room);
char * get_room_description(ROOM_INDEX_DATA *room);
char * get_room_name(ROOM_INDEX_DATA *room);
void do_look (CHAR_DATA *ch, char *argument);
/* RT added back for the hell of it */
void do_read (CHAR_DATA *ch, char *argument);
/*
 * Thanks to Zrin for auto-exit part.
 */
void do_exits (CHAR_DATA *ch, char *argument);
void do_worth (CHAR_DATA *ch, char *argument);
void do_score (CHAR_DATA *ch, char *argument);
void do_affects(CHAR_DATA *ch, char *argument);
void do_time (CHAR_DATA *ch, char *argument);
void do_weather (CHAR_DATA *ch, char *argument);
void do_oldhelp (CHAR_DATA *ch, char *argument);
/* whois command */
void do_whois (CHAR_DATA *ch, char *argument);
/*
 * New 'who' command originally by Alander of Rivers of Mud.
 */
void do_who (CHAR_DATA *ch, char *argument);
void do_count  (CHAR_DATA *ch, char *argument);
void do_inventory (CHAR_DATA *ch, char *argument);
void do_equipment (CHAR_DATA *ch, char *argument);
void do_compare (CHAR_DATA *ch, char *argument);
void do_where (CHAR_DATA *ch, char *argument);
void do_consider (CHAR_DATA *ch, char *argument);
void set_title (CHAR_DATA *ch, char *title);
void do_title (CHAR_DATA *ch, char *argument);
void set_extitle(CHAR_DATA *ch, char *title);
void do_extitle (CHAR_DATA *ch, char *argument);
void descr_end_fun(CHAR_DATA *ch, char *argument);
void do_description (CHAR_DATA *ch, char *argument);
void do_report (CHAR_DATA *ch, char *argument);
void do_practice (CHAR_DATA *ch, char *argument);
/*
 * 'Wimpy' originally by Dionysos.
 */
void do_wimpy (CHAR_DATA *ch, char *argument);
void do_password (CHAR_DATA *ch, char *argument);
char * get_descr_form(CHAR_DATA *ch,CHAR_DATA *looker, bool get_long);
/* Last returns are redundancy ones in case of some failure in checks */
/* Replacement for the handler.c get_char_room to handle morph dragon */
CHAR_DATA * get_char_room (CHAR_DATA *ch, char *argument);
CHAR_DATA * get_char_from_room (CHAR_DATA *ch, ROOM_INDEX_DATA *room, char *argument);
void do_balance(CHAR_DATA *ch, char *argument);
void do_withdraw(CHAR_DATA *ch,char *argument);
void do_deposit(CHAR_DATA *ch,char *argument);
/* Some shapeshifter code */
/* do a if (can_shapeshift(ch,form_num)) check.
 * If not then return and don't revert. Otherwise shapeshift_revert()
 * and then apply new morph num to the player in the do_morph_dragon fun.
 */
void do_records(CHAR_DATA *ch,char *argument);
void do_ratings(CHAR_DATA *ch, char *argument);
void do_lore (CHAR_DATA *ch, char *argument);
bool isCabalItem( OBJ_DATA *obj);
bool isNewbie (CHAR_DATA *ch);
void debug_string( const char *str);
void do_xlook (CHAR_DATA *ch, char *argument);
char * get_where_name(int iWear);
void do_trustgroup(CHAR_DATA *ch, char *argument);
void do_trustcabal(CHAR_DATA *ch, char *argument);
void do_trustchar(CHAR_DATA *ch, char *argument);
void do_role(CHAR_DATA *ch, char *argument);
void add_role(CHAR_DATA *ch, char *string);
void show_temp_role(CHAR_DATA *ch);
void show_role(CHAR_DATA *ch);

#endif /* ACT_INFO_H */
