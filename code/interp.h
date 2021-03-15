/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik Stærfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/
 
/***************************************************************************
 *  ROM 2.4 is copyright 1993-1996 Russ Taylor                             *
 *  ROM has been brought to you by the ROM consortium                      *
 *      Russ Taylor (rtaylor@pacinfo.com)                                  *
 *      Gabrielle Taylor (gtaylor@pacinfo.com)                             *
 *      Brian Moore (rom@rom.efn.org)                                      *
 *  By using this code, you have agreed to follow the terms of the         *
 *  ROM license, in the file Tartarus/doc/rom.license                      *
****************************************************************************/

/***************************************************************************
 *       Tartarus code is copyright (C) 1997-1998 by Daniel Graham         *
 *  In using this code you agree to comply with the Tartarus license       *
 *       found in the file /Tartarus/doc/tartarus.doc                      *
 ***************************************************************************/

/* this is a listing of all the commands and command related data */

#ifndef INTERP_H
#define INTERP_H

#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "handler.h"
#include "magic.h"
#include "tables.h"
#include "spec.h"
#include "act_move.h"
#include "devextra.h"
#include "vote.h"
#include "skills.h"
#include "comm.h"
#include "act_wiz.h"
#include "fight.h"
#include "db.h"
#include "db2.h"
#include "note.h"
#include "olc.h"
#include "olc_act.h"
#include "olc_save.h"
#include "magic.h"
#include "heal.h"
#include "tattoo.h"
#include "quest.h"
#include "flags.h"
#include "scan.h"
#include "act_info.h"
#include "act_wiz.h"
#include "iprog.h"
#include "const.h"

/* for command types */
#define ML				MAX_LEVEL		/* implementor */
#define L1				MAX_LEVEL - 1	/* creator */
#define L2				MAX_LEVEL - 2	/* supreme being */
#define L3				MAX_LEVEL - 3	/* deity */
#define L4				MAX_LEVEL - 4	/* god */
#define L5				MAX_LEVEL - 5	/* immortal */
#define L6				MAX_LEVEL - 6	/* demigod */
#define L7				MAX_LEVEL - 7	/* angel */
#define L8				MAX_LEVEL - 8	/* avatar */
#define IM				LEVEL_IMMORTAL	/* avatar */
#define HE				LEVEL_HERO		/* hero */

#define COM_INGORE		1

/*
 * Command logging types.
 */
#define LOG_NORMAL		0
#define LOG_ALWAYS		1
#define LOG_NEVER		2

#define LAST_COMMAND 	"lastcommand.txt"

/*
 * Structure for a command in the command lookup table.
 */
struct cmd_type
{
	char * const name;
	DO_FUN *do_fun;
	sh_int position;
	sh_int level;
	sh_int log;
	sh_int show;
	sh_int hide;
	char * const skill_name;
};

/* the command table itself */
extern const struct cmd_type cmd_table[];

// TODO: where is this implemented
extern void do_wizhelp(CHAR_DATA *ch, char *argument);


extern void do_silent_movement(CHAR_DATA *ch, char *argument); // race.c
extern void do_proficiencies(CHAR_DATA *ch, char *argument); // prof.c
extern void do_bandage(CHAR_DATA *ch, char *argument); // temp_interep.c

//
// TODO: UNKNOWN FUNCTIONS
//

// extern void do_answer(CHAR_DATA *ch, char *argument);
// extern void do_auction(CHAR_DATA *ch, char *argument);
// extern void do_shadowgate(CHAR_DATA *ch, char *argument);
// extern void do_eye_of_the_predator(CHAR_DATA *ch, char *argument);
// extern void do_lunge(CHAR_DATA *ch, char *argument);
// extern void do_noloot(CHAR_DATA *ch, char *argument);
// extern void do_moron(CHAR_DATA *ch, char *argument);
// extern void do_revert(CHAR_DATA *ch, char *argument);
// extern void do_deathstyle(CHAR_DATA *ch, char *argument);
// extern void do_bearcharge(CHAR_DATA *ch, char *argument);
// extern void do_skullcrush(CHAR_DATA *ch, char *argument);
// extern void do_mpstat(CHAR_DATA *ch, char *argument);
// extern void do_mpdump(CHAR_DATA *ch, char *argument);
// extern void do_tsunami(CHAR_DATA *ch, char *argument);
// extern void do_ram(CHAR_DATA *ch, char *argument);
// extern void do_trunksmash(CHAR_DATA *ch, char *argument);
// extern void do_shapeshift(CHAR_DATA *ch, char *argument);
// extern void do_retribution(CHAR_DATA *ch, char *argument);
// extern void do_legsweep(CHAR_DATA *ch, char *argument);
// extern void do_question(CHAR_DATA *ch, char *argument);
// extern void do_notransfer(CHAR_DATA *ch, char *argument);
// extern void do_choke(CHAR_DATA *ch, char *argument);
// extern void do_evaluation(CHAR_DATA *ch, char *argument);
// extern void do_weaponbreaker(CHAR_DATA *ch, char *argument);
// extern void do_cranial(CHAR_DATA *ch, char *argument);
// extern void do_disperse(CHAR_DATA *ch, char *argument);
// extern void do_quest(CHAR_DATA *ch, char *argument);
// extern void do_rally(CHAR_DATA *ch, char *argument);
// extern void do_devour(CHAR_DATA *ch, char *argument);
// extern void do_play(CHAR_DATA *ch, char *argument);
// extern void do_quote(CHAR_DATA *ch, char *argument);
// extern void do_stab(CHAR_DATA *ch, char *argument);
// extern void do_whirl(CHAR_DATA *ch, char *argument);
// extern void do_forms(CHAR_DATA *ch, char *argument);
// extern void do_blast(CHAR_DATA *ch, char *argument);
// extern void do_pay(CHAR_DATA *ch, char *argument);

//
// LOCAL FUNCTIONS
//

void list_cmd_queue(CHAR_DATA *ch);
void clear_cmd_queue(CHAR_DATA *ch);
void interpret(CHAR_DATA *ch, char *argument);
bool knows_command(CHAR_DATA *ch, int cmd);
bool check_social(CHAR_DATA *ch, char *command, char *argument);
bool is_number(char *arg);
bool is_alphanum(char *arg);
int have_space(char *arg);
bool have_schar(char *arg);
int number_argument(char *argument, char *arg);
int mult_argument(char *argument, char *arg);
char *one_argument(char *argument, char *arg_first);
void do_commands(CHAR_DATA *ch, char *argument);
void do_wizhelp(CHAR_DATA *ch, char *argument);

#endif /* INTERP_H */
