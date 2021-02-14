#ifndef ACT_WIZ_H
#define ACT_WIZ_H

#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <iterator>
#include "merc.h"
#include "handler.h"
#include "recycle.h"
#include "tables.h"
#include "lookup.h"
#include "olc.h"
#include "spec.h"
#include "interp.h"
#include "act_info.h"
#include "save.h"
#include "dioextra.h"
#include "help.h"
#include "devextra.h"
#include "newmem.h"
#include "update.h"
#include "cabal.h"
#include "comm.h"
#include "act_comm.h"
#include "fight.h"
#include "skills.h"
#include "db.h"
#include "magic.h"
#include "mprog.h"

#define GOD_LOG_FILE	"glog.txt"

typedef struct multdata MULTDATA;

struct multdata
{
	DESCRIPTOR_DATA *des;
};

//
// LOCAL FUNCTIONS
//

void do_wiznet (CHAR_DATA *ch, char *argument);
void wiznet (char *string, CHAR_DATA *ch, OBJ_DATA *obj, long flag, long flag_skip, int min_level);
void do_leader (CHAR_DATA *ch, char *argument);
void do_smite (CHAR_DATA *ch, char *argument);
/* Induction in a Cabal. When inducted a player gains skills or spells
specific for that Cabal. You can add or remove abilities very easily by
just following the basic format used here.
-Ceran
*/
void do_induct (CHAR_DATA *ch, char *argument);
/* equips a character */
void do_outfit (CHAR_DATA *ch, char *argument);
/* RT nochannels command, for those spammers */
void do_nochannels (CHAR_DATA *ch, char *argument);
void do_smote (CHAR_DATA *ch, char *argument);
void do_bamfin (CHAR_DATA *ch, char *argument);
void do_bamfout (CHAR_DATA *ch, char *argument);
void do_deny (CHAR_DATA *ch, char *argument);
void do_disconnect (CHAR_DATA *ch, char *argument);
void do_pardon (CHAR_DATA *ch, char *argument);
void do_echo (CHAR_DATA *ch, char *argument);
void do_immecho (CHAR_DATA *ch, char *argument);
void do_recho (CHAR_DATA *ch, char *argument);
void do_zecho (CHAR_DATA *ch, char *argument);
void do_pecho (CHAR_DATA *ch, char *argument);
ROOM_INDEX_DATA *find_location (CHAR_DATA *ch, char *arg);
void do_transfer (CHAR_DATA *ch, char *argument);
void do_at (CHAR_DATA *ch, char *argument);
void do_goto (CHAR_DATA *ch, char *argument);
void do_violate (CHAR_DATA *ch, char *argument);
/* RT to replace the 3 stat commands */
void do_stat  (CHAR_DATA *ch, char *argument);
void do_rstat (CHAR_DATA *ch, char *argument);
void do_ostat (CHAR_DATA *ch, char *argument);
void do_astat (CHAR_DATA *ch, char* argument);
void do_mstat (CHAR_DATA *ch, char *argument);
/* ofind and mfind replaced with vnum, vnum skill also added */
void do_vnum (CHAR_DATA *ch, char *argument);
void do_mfind (CHAR_DATA *ch, char *argument);
void do_ofind (CHAR_DATA *ch, char *argument);
void do_owhere(CHAR_DATA *ch, char *argument);
void do_mwhere (CHAR_DATA *ch, char *argument);
void do_reboo (CHAR_DATA *ch, char *argument);
void reboot_now (CHAR_DATA *ch);
void start_reboot (CHAR_DATA *ch);
void do_reboot (CHAR_DATA *ch, char *argument);
void do_protect (CHAR_DATA *ch, char *argument);
void do_snoop (CHAR_DATA *ch, char *argument);
void do_switch (CHAR_DATA *ch, char *argument);
void do_return (CHAR_DATA *ch, char *argument);
/* trust levels for load and clone */
bool obj_check (CHAR_DATA *ch, OBJ_DATA *obj);
/* for clone, to insure that cloning goes many levels deep */
void recursive_clone (CHAR_DATA *ch, OBJ_DATA *obj, OBJ_DATA *clone);
/* command that is similar to load */
void do_clone (CHAR_DATA *ch, char *argument);
/* RT to replace the two load commands */
void do_load(CHAR_DATA *ch, char *argument);
void do_mload (CHAR_DATA *ch, char *argument);
void do_oload (CHAR_DATA *ch, char *argument);
void do_purge (CHAR_DATA *ch, char *argument);
void do_advance (CHAR_DATA *ch, char *argument);
void do_trust (CHAR_DATA *ch, char *argument);
void do_restore (CHAR_DATA *ch, char *argument);
void do_freeze (CHAR_DATA *ch, char *argument);
void do_log (CHAR_DATA *ch, char *argument);
void do_noemote (CHAR_DATA *ch, char *argument);
void do_noshout (CHAR_DATA *ch, char *argument);
void do_notell (CHAR_DATA *ch, char *argument);
void do_peace (CHAR_DATA *ch, char *argument);
void do_wizlock (CHAR_DATA *ch, char *argument);
/* RT anti-newbie code */
void do_newlock (CHAR_DATA *ch, char *argument);
void do_slookup (CHAR_DATA *ch, char *argument);
/* RT set replaces sset, mset, oset, and rset */
void do_set (CHAR_DATA *ch, char *argument);
void do_sgset (CHAR_DATA *ch, char *argument);
void do_sset (CHAR_DATA *ch, char *argument);
void do_mset (CHAR_DATA *ch, char *argument);
void do_string (CHAR_DATA *ch, char *argument);
void do_oset (CHAR_DATA *ch, char *argument);
void do_rset (CHAR_DATA *ch, char *argument);
void do_sockets (CHAR_DATA *ch, char *argument);
char* get_end_host (char* host);
int host_comp (MULTDATA* d1, MULTDATA* d2);
void do_multicheck (CHAR_DATA *ch, char *argument);
/* Use this to lag out spammers and force them to stop moving */
void do_lagout (CHAR_DATA *ch,char *argument);
/*
 * Thanks to Grodyn for pointing out bugs in this function.
 */
void do_force (CHAR_DATA *ch, char *argument);
/*
 * New routines by Dionysos.
 */
void do_invis (CHAR_DATA *ch, char *argument);
void do_incognito (CHAR_DATA *ch, char *argument);
void do_holylight (CHAR_DATA *ch, char *argument);
/* prefix command: it will put the string typed on each line typed */
void do_prefi (CHAR_DATA *ch, char *argument);
void do_prefix (CHAR_DATA *ch, char *argument);
void do_astrip (CHAR_DATA *ch,char *argument);
void do_limcounter (CHAR_DATA *ch,char *argument);
void do_classes (CHAR_DATA *ch, char *argument);
void do_rinfo (CHAR_DATA *ch,char *argument);
void do_deathmessage (CHAR_DATA *ch,char *argument);
void do_max_limits (CHAR_DATA *ch,char *argument);
/* Add Apply */
void do_addapply (CHAR_DATA *ch, char *argument);
void log_naughty (CHAR_DATA *ch, char *argument, int logtype);
void do_vstat (CHAR_DATA *ch, char *argument);
void do_vmstat (CHAR_DATA *ch, char *argument);
void do_vostat (CHAR_DATA *ch, char *argument);
void do_history (CHAR_DATA *ch, char *argument);
void add_history (CHAR_DATA *ch, CHAR_DATA *victim, char *string);
void show_temp_history (CHAR_DATA *ch, CHAR_DATA *victim);
void show_history (CHAR_DATA *ch, CHAR_DATA *victim);
void do_afk (CHAR_DATA *ch, char *argument);
void do_empower (CHAR_DATA *ch, char *argument);
void do_raffects (CHAR_DATA *ch, char *argument);
void do_rastrip (CHAR_DATA *ch,char *argument);
void do_aastrip (CHAR_DATA *ch, char *argument);
void do_oastrip (CHAR_DATA *ch, char *argument);
void do_givexp (CHAR_DATA *ch, char *argument);
void do_setpath (CHAR_DATA *ch, char *argument);
void do_clearfavors (CHAR_DATA *ch, char *argument);
void do_gsnlist (CHAR_DATA *ch,char *argument);
void do_ccl (CHAR_DATA *ch, char *argument);
void do_noskills (CHAR_DATA *ch, char *argument);
void do_buglist (CHAR_DATA *ch, char *argument);
void buglist_end_fun (CHAR_DATA *ch, char *argument);
void do_constdump (CHAR_DATA *ch, char *argument);
void do_interpdump (CHAR_DATA *ch, char *argument);
void do_racedump (CHAR_DATA *ch, char *argument);

#endif /* ACT_WIZ_H */
