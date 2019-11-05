#ifndef ACT_COMM_H
#define ACT_COMM_H

#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <mysql.h>
#include "merc.h"
#include "recycle.h"
#include "tables.h"
#include "olc.h"
#include "spec.h"
#include "comm.h"
#include "devextra.h"
#include "warrior.h"
#include "save.h"
#include "act_info.h"
#include "newmem.h"
#include "act_wiz.h"
#include "interp.h"
#include "fight.h"
#include "skills.h"
#include "act_comm.h"
#include "db.h"
#include "devextra.h"
#include "magic.h"
#include "dioextra.h"
#include "act_move.h"
#include "handler.h"
#include "ap.h"


/* command procedures needed */
DECLARE_DO_FUN(do_quit	);
DECLARE_DO_FUN(do_look);
DECLARE_DO_FUN(do_buglist);
DECLARE_DO_FUN(do_undisguise);
DECLARE_AFF_FUN(traitor_pulse);
#define PDEATH_LOG_FILE	RIFT_AREA_DIR "/permdeaths.txt"
#define DEATH_LOG_FILE	RIFT_AREA_DIR "/tempdeaths.txt"
#define MOB_LOG_FILE	RIFT_AREA_DIR "/mobdeaths.txt"

/* TODO: unused functions? */
void obj_say_lordly (CHAR_DATA *ch,OBJ_DATA *obj,char *argument);
void mob_say_lady (CHAR_DATA *ch, char *argument);
void say_prog_summon_demon (CHAR_DATA *ch,char *argument);
void say_prog_raise_shadowlord (CHAR_DATA *ch,char *argument);
void say_prog_karguska (CHAR_DATA *ch,char *argument);
bool check_ooc (CHAR_DATA *ch, char *argument, char*type);
/* END TODO */

void do_delet (CHAR_DATA *ch, char *argument);
void do_delete (CHAR_DATA *ch, char *argument);
/* RT code to display channel status */
void do_channels (CHAR_DATA *ch, char *argument);
/* RT deaf blocks out all shouts */
void do_deaf (CHAR_DATA *ch, char *argument);
/* RT quiet blocks out all communication */
void do_quiet (CHAR_DATA *ch, char * argument);
void do_replay (CHAR_DATA *ch, char *argument);
void do_cb (CHAR_DATA *ch, char *argument);
void do_global (CHAR_DATA *ch,char *argument);
void do_newbie (CHAR_DATA *ch, char *argument);
void do_builder (CHAR_DATA *ch, char *argument);
void do_immtalk (CHAR_DATA *ch, char *argument);
const char *upstring(const char *i);
const char *lowstring(const char *i);
/* Used for an object...take this out if you don't want it */
void obj_say_heavenly_sceptre (CHAR_DATA *ch, OBJ_DATA *obj,char *argument);
void do_say (CHAR_DATA *ch, char *argument);
void say_to (CHAR_DATA *ch, CHAR_DATA *victim, char *argument, char *extra);
void do_whisper (CHAR_DATA *ch, char *argument);
void do_sing (CHAR_DATA *ch, char *argument);
void do_pray (CHAR_DATA *ch, char *argument);
void do_tell (CHAR_DATA *ch, char *argument);
void do_noreply (CHAR_DATA *ch, char *argument);
void do_reply (CHAR_DATA *ch, char *argument);
void do_yell (CHAR_DATA *ch, char *argument);
void do_myell (CHAR_DATA *ch, char *argument, CHAR_DATA *attacker);
void do_emote (CHAR_DATA *ch, char *argument);
void do_pmote (CHAR_DATA *ch, char *argument);
/* Simple, yet brilliant.  Notify immortals when players are using words that
 * are offensive/harrassing or commonly associated with OOC speech.  Immortals
 * can then respond accordingly if a rule violation is detected.
 * 											-El
 */
bool check_ooc (CHAR_DATA *ch, char *argument, char *type);
void do_bug (CHAR_DATA *ch, char *argument);
void do_typo (CHAR_DATA *ch, char *argument);
void do_idea (CHAR_DATA *ch, char *argument);
void do_rent (CHAR_DATA *ch, char *argument);
void do_qui (CHAR_DATA *ch, char *argument);
void do_quit (CHAR_DATA *ch, char *argument);
void do_quit_new (CHAR_DATA *ch, char *argument, bool autoq);
void do_save (CHAR_DATA *ch, char *argument);
void do_follow (CHAR_DATA *ch, char *argument);
void add_follower (CHAR_DATA *ch, CHAR_DATA *master);
void stop_follower (CHAR_DATA *ch);
/* nukes charmed monsters and pets */
void nuke_pets (CHAR_DATA *ch);
void die_follower (CHAR_DATA *ch);
void do_order (CHAR_DATA *ch, char *argument);
void do_group (CHAR_DATA *ch, char *argument);
/*
 * 'Split' originally by Gnort, God of Chaos.
 */
void do_split (CHAR_DATA *ch, char *argument);
void do_gtell (CHAR_DATA *ch, char *argument);
SPEECH_DATA *find_speech (MOB_INDEX_DATA *mob, char *name);
void execute_speech (CHAR_DATA *ch, CHAR_DATA *mob, SPEECH_DATA *speech);
void speech_handler (CHAR_DATA *ch, CHAR_DATA *mob, SPEECH_DATA *speech);
/*
 * It is very important that this be an equivalence relation:
 * (1) A ~ A
 * (2) if A ~ B then B ~ A
 * (3) if A ~ B  and B ~ C, then A ~ C
 */
bool is_same_group (CHAR_DATA *ach, CHAR_DATA *bch);
void do_release (CHAR_DATA *ch,char *argument);
void perm_death_log (CHAR_DATA *ch, int deltype);
void temp_death_log (CHAR_DATA *killer, CHAR_DATA *dead);
void mob_death_log (CHAR_DATA *killer, CHAR_DATA *dead);
/* type 0 = create, 1 = login, 2 = logout */
void login_log (CHAR_DATA *ch, int type);

#endif  /* ACT_COMM_H */
