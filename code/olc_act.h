#ifndef OLC_ACT_H
#define OLC_ACT_H

/***************************************************************************
 *  File: olc_act.c                                                        *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 *                                                                         *
 *  This code was freely distributed with the The Isles 1.1 source code,   *
 *  and has been used here for OLC - OLC would not be what it is without   *
 *  all the previous coders who released their source code.                *
 *                                                                         *
 ***************************************************************************/

#ifdef macintosh
#include <types.h>
#else
#include <sys/types.h>
#endif

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "handler.h"
#include "olc.h"
#include "tables.h"
#include "recycle.h"
#include "lookup.h"
#include "spec.h"
#include "act_comm.h"
#include "misc.h"
#include "act_move.h"
#include "warrior.h"
#include "comm.h"
#include "db.h"
#include "bit.h"
#include "string.h"
#include "aprog.h"
#include "iprog.h"
#include "mprog.h"
#include "rprog.h"
#include "chardef.h"
#include "const.h"
#include "material.h"

struct olc_help_type
{
	char *command;
	const void *structure;
	char *desc;
};

struct wear_type
{
	int	wear_loc;
	int	wear_bit;
};

extern const struct olc_help_type help_table[];
extern const struct wear_type wear_table[];

//#include "iprog.h"
//#include "mprog.h"
//#include "rprog.h"
//#include "aprog.h"
extern void iprog_set (OBJ_INDEX_DATA *, const char *, const char *);
extern void mprog_set (MOB_INDEX_DATA *, const char *, const char *);
extern void rprog_set (ROOM_INDEX_DATA *, const char *, const char *);
extern void aprog_set (AREA_DATA *, const char *, const char *);
extern bool mprog_unset (MOB_INDEX_DATA *, const char *, const char *);
extern bool aprog_unset (AREA_DATA *, const char *, const char *);
extern bool iprog_unset (OBJ_INDEX_DATA *, const char *, const char *);
extern bool rprog_unset (ROOM_INDEX_DATA *, const char *, const char *);

//
// LOCAL FUNCTIONS
//


bool show_version (CHAR_DATA *ch, char *argument);
/*****************************************************************************
 Name:		show_flag_cmds
 Purpose:	Displays settable flags and stats.
 Called by:	show_help(olc_act.c).
 ****************************************************************************/
void show_flag_cmds (CHAR_DATA *ch, const struct flag_type *flag_table);
/*****************************************************************************
 Name:		show_skill_cmds
 Purpose:	Displays all skill functions.
 		Does remove those damn immortal commands from the list.
 		Could be improved by:
 		(1) Adding a check for a particular class.
 		(2) Adding a check for a level range.
 Called by:	show_help(olc_act.c).
 ****************************************************************************/
void show_skill_cmds (CHAR_DATA *ch, int tar);
/*****************************************************************************
 Name:		show_spec_cmds
 Purpose:	Displays settable special functions.
 Called by:	show_help(olc_act.c).
 ****************************************************************************/

/*
void show_spec_cmds (CHAR_DATA *ch);
*/

/*****************************************************************************
 Name:		show_help
 Purpose:	Displays help for many tables used in OLC.
 Called by:	olc interpreters.
 ****************************************************************************/
bool show_help (CHAR_DATA *ch, char *argument);
bool redit_rlist (CHAR_DATA *ch, char *argument);
bool redit_mview (CHAR_DATA *ch, char *argument);
bool redit_mlist (CHAR_DATA *ch, char *argument);
bool redit_olist (CHAR_DATA *ch, char *argument);
bool redit_mshow (CHAR_DATA *ch, char *argument);
bool redit_oshow (CHAR_DATA *ch, char *argument);
/*****************************************************************************
 Name:		check_range( lower vnum, upper vnum )
 Purpose:	Ensures the range spans only one area.
 Called by:	aedit_vnum(olc_act.c).
 ****************************************************************************/
bool check_range( int lower, int upper);
AREA_DATA *get_vnum_area( int vnum);
bool medit_vnum (CHAR_DATA *ch, char *argument);
bool medit_group (CHAR_DATA *ch, char *argument);
bool medit_speech (CHAR_DATA *ch, char *argument);
bool medit_prog (CHAR_DATA *ch, char *argument);
bool oedit_prog (CHAR_DATA *ch, char *argument);
bool oedit_spec (CHAR_DATA *ch, char *argument);
bool redit_trap (CHAR_DATA *ch, char *argument);
bool redit_prog (CHAR_DATA *ch, char *argument);
bool redit_altdesc (CHAR_DATA *ch, char *argument);
bool aedit_type (CHAR_DATA *ch, char *argument);
bool aedit_prog (CHAR_DATA *ch, char *argument);
bool aedit_security (CHAR_DATA *ch, char *argument);
bool aedit_show (CHAR_DATA *ch, char *argument);
bool aedit_reset (CHAR_DATA *ch, char *argument);
bool aedit_create (CHAR_DATA *ch, char *argument);
bool aedit_name (CHAR_DATA *ch, char *argument);
bool aedit_credits (CHAR_DATA *ch, char *argument);
bool aedit_file (CHAR_DATA *ch, char *argument);
bool aedit_level (CHAR_DATA *ch, char *argument);
bool aedit_age (CHAR_DATA *ch, char *argument);
bool aedit_builder (CHAR_DATA *ch, char *argument);
bool aedit_vnum (CHAR_DATA *ch, char *argument);
bool aedit_lvnum (CHAR_DATA *ch, char *argument);
bool aedit_uvnum (CHAR_DATA *ch, char *argument);
int aclimate_lookup (const char *name);
bool aedit_climate (CHAR_DATA *ch, char *argument);
bool redit_cabal (CHAR_DATA *ch, char *argument);
bool redit_owner (CHAR_DATA *ch, char *argument);
bool redit_show (CHAR_DATA *ch, char *argument);
bool change_exit( CHAR_DATA *ch, char *argument, int door);
bool redit_north (CHAR_DATA *ch, char *argument);
bool redit_south (CHAR_DATA *ch, char *argument);
bool redit_east (CHAR_DATA *ch, char *argument);
bool redit_west (CHAR_DATA *ch, char *argument);
bool redit_up (CHAR_DATA *ch, char *argument);
bool redit_down (CHAR_DATA *ch, char *argument);
bool redit_ed (CHAR_DATA *ch, char *argument);
bool redit_create (CHAR_DATA *ch, char *argument);
bool redit_name (CHAR_DATA *ch, char *argument);
void redit_end_fun(CHAR_DATA *ch, char *argument);
bool redit_desc (CHAR_DATA *ch, char *argument);
bool redit_heal (CHAR_DATA *ch, char *argument);
bool redit_mana (CHAR_DATA *ch, char *argument);
bool redit_clan (CHAR_DATA *ch, char *argument);
bool redit_format (CHAR_DATA *ch, char *argument);
bool redit_mreset (CHAR_DATA *ch, char *argument);
/*****************************************************************************
 Name:		wear_loc
 Purpose:	Returns the location of the bit that matches the count.
 		1 = first match, 2 = second match etc.
 Called by:	oedit_reset(olc_act.c).
 ****************************************************************************/
int wear_loc (int bits, int count);
/*****************************************************************************
 Name:		wear_bit
 Purpose:	Converts a wear_loc into a bit.
 Called by:	redit_oreset(olc_act.c).
 ****************************************************************************/
int wear_bit (int loc);
bool redit_oreset (CHAR_DATA *ch, char *argument);
void show_obj_values (CHAR_DATA *ch, OBJ_INDEX_DATA *obj);
bool set_obj_values (CHAR_DATA *ch, OBJ_INDEX_DATA *pObj, int value_num, char *argument);
bool oedit_show (CHAR_DATA *ch, char *argument);
bool oedit_addapply (CHAR_DATA *ch, char *argument);

/*
bool oedit_addaffect (CHAR_DATA *ch, char *argument);
*/

/*
 * My thanks to Hans Hvidsten Birkeland and Noam Krendel(Walker)
 * for really teaching me how to manipulate pointers.
 */
bool oedit_delapply (CHAR_DATA *ch, char *argument);
bool oedit_msg  (CHAR_DATA *ch, char *argument);
bool oedit_limit (CHAR_DATA *ch, char *argument);
bool oedit_verb (CHAR_DATA *ch, char *argument);
bool oedit_cabal (CHAR_DATA *ch, char *argument);
bool oedit_timer (CHAR_DATA *ch, char *argument);
bool oedit_restrict (CHAR_DATA *ch, char *argument);
bool oedit_notes (CHAR_DATA *ch, char *argument);
bool oedit_wear_name  (CHAR_DATA *ch, char *argument);
bool oedit_flag (CHAR_DATA *ch, char *argument);
bool oedit_name (CHAR_DATA *ch, char *argument);
bool oedit_short (CHAR_DATA *ch, char *argument);
bool oedit_long (CHAR_DATA *ch, char *argument);
bool set_value (CHAR_DATA *ch, OBJ_INDEX_DATA *pObj, char *argument, int value);
/*****************************************************************************
 Name:		oedit_values
 Purpose:	Finds the object and sets its value.
 Called by:	The four valueX functions below. (now five -- Hugin )
 ****************************************************************************/
bool oedit_values (CHAR_DATA *ch, char *argument, int value);
bool oedit_value0 (CHAR_DATA *ch, char *argument);
bool oedit_value1 (CHAR_DATA *ch, char *argument);
bool oedit_value2 (CHAR_DATA *ch, char *argument);
bool oedit_value3 (CHAR_DATA *ch, char *argument);
bool oedit_value4 (CHAR_DATA *ch, char *argument);
bool oedit_weight (CHAR_DATA *ch, char *argument);
bool oedit_cost (CHAR_DATA *ch, char *argument);
bool oedit_create (CHAR_DATA *ch, char *argument);
bool oedit_ed (CHAR_DATA *ch, char *argument);
bool oedit_extra (CHAR_DATA *ch, char *argument);      /* Moved out of oedit() due to naming conflicts -- Hugin */
bool oedit_wear (CHAR_DATA *ch, char *argument);      /* Moved out of oedit() due to naming conflicts -- Hugin */
bool oedit_type (CHAR_DATA *ch, char *argument);      /* Moved out of oedit() due to naming conflicts -- Hugin */
bool oedit_material (CHAR_DATA *ch, char *argument);
bool oedit_level (CHAR_DATA *ch, char *argument);
bool oedit_condition (CHAR_DATA *ch, char *argument);
bool medit_limit (CHAR_DATA *ch, char *argument);
bool medit_optional (CHAR_DATA *ch, char *argument);
bool medit_yell (CHAR_DATA *ch, char *argument);
bool medit_notes (CHAR_DATA *ch, char *argument);
void clean_mob_class (MOB_INDEX_DATA *pMob, int class_index);
bool medit_class (CHAR_DATA *ch, char *argument);
bool medit_show (CHAR_DATA *ch, char *argument);
bool medit_create (CHAR_DATA *ch, char *argument);

/*
bool medit_spec (CHAR_DATA *ch, char *argument);
*/

bool medit_damtype (CHAR_DATA *ch, char *argument);
bool medit_align (CHAR_DATA *ch, char *argument);
bool medit_level (CHAR_DATA *ch, char *argument);
bool medit_desc (CHAR_DATA *ch, char *argument);
bool medit_long (CHAR_DATA *ch, char *argument);
bool medit_short (CHAR_DATA *ch, char *argument);
bool medit_name (CHAR_DATA *ch, char *argument);
bool medit_cabal (CHAR_DATA *ch, char *argument);
bool medit_shop (CHAR_DATA *ch, char *argument);
/* ROM medit functions: */
bool medit_sex (CHAR_DATA *ch, char *argument);          /* Moved out of medit() due to naming conflicts -- Hugin */
bool medit_act (CHAR_DATA *ch, char *argument);          /* Moved out of medit() due to naming conflicts -- Hugin */
bool medit_affect (CHAR_DATA *ch, char *argument);      /* Moved out of medit() due to naming conflicts -- Hugin */
bool medit_ac (CHAR_DATA *ch, char *argument);
bool medit_form (CHAR_DATA *ch, char *argument);
bool medit_part (CHAR_DATA *ch, char *argument);
bool medit_imm (CHAR_DATA *ch, char *argument);
bool medit_res (CHAR_DATA *ch, char *argument);
bool medit_vuln (CHAR_DATA *ch, char *argument);
bool medit_material (CHAR_DATA *ch, char *argument);
bool medit_off (CHAR_DATA *ch, char *argument);
bool medit_size (CHAR_DATA *ch, char *argument);
bool medit_hitdice (CHAR_DATA *ch, char *argument);
bool medit_manadice (CHAR_DATA *ch, char *argument);
bool medit_damdice (CHAR_DATA *ch, char *argument);
bool medit_race (CHAR_DATA *ch, char *argument);
bool medit_position (CHAR_DATA *ch, char *argument);
bool medit_gold (CHAR_DATA *ch, char *argument);
bool medit_hitroll (CHAR_DATA *ch, char *argument);
bool oedit_liqlist (CHAR_DATA *ch, char *argument);

#endif /* OLC_ACT_H */
