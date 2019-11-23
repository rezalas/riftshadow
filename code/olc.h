/***************************************************************************
 *  File: olc.h                                                            *
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
/*
 * This is a header file for all the OLC files.  Feel free to copy it into
 * merc.h if you wish.  Many of these routines may be handy elsewhere in
 * the code.  -Jason Dinkel
 */

#ifndef OLC_H
#define OLC_H

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
#include "tables.h"
#include "mem.h"
#include "olc_act.h"
#include "lookup.h"
#include "bit.h"
#include "comm.h"
#include "interp.h"
#include "db.h"
#include "act_comm.h"
#include "help.h"
#include "magic.h"

/*
 * Interpreter Table Prototypes
 */
extern const struct olc_cmd_type aedit_table[];
extern const struct olc_cmd_type redit_table[];
extern const struct olc_cmd_type oedit_table[];
extern const struct olc_cmd_type medit_table[];

/*
 * The version info.  Please use this info when reporting bugs.
 * It is displayed in the game by typing 'version' while editing.
 * Do not remove these from the code - by request of Jason Dinkel
 */
#define SHOW_VERSION	"ILAB Online Creation [Beta 1.0, ROM 2.3 modified]\n\r" \
						"     Port a ROM 2.4 v1.00\n\r"
#define SHOW_AUTHOR		"     By Jason(jdinkel@mines.colorado.edu)\n\r" \
						"     Modified for use with ROM 2.3\n\r"        \
						"     By Hans Birkeland (hansbi@ifi.uio.no)\n\r" \
						"     Modified for use with Riftshadow\n\r" \
						"     By Chad Sorrell (gabe@qhcf.net)\n\r" 
#define SHOW_DATE		"     (Apr. 7, 1995 - ROM mod, Apr 16, 1995)\n\r" \
						"     (Port a ROM 2.4 - Nov 2, 1996)\n\r" \
						"     (Riftshadow Mod - June 1, 2001)\n\r"
#define SHOW_CREDITS	"     Original by Surreality(cxw197@psu.edu) and Locke(locke@lm.com)"


/*
 * New typedefs.
 */
typedef	bool OLC_FUN (CHAR_DATA *ch, char *argument);


/*
 * Macros
 */

#define IS_BUILDER(ch, area)	(!IS_SWITCHED(ch) &&																\
									((ch->pcdata->security <= area->security && strstr(area->builders, ch->name))	\
										|| ch->pcdata->security > area->security									\
										|| strstr(area->builders, "All")))


/* Return pointers to what is being edited. */
#define EDIT_MOB(ch, mob)		(mob = (MOB_INDEX_DATA *)ch->desc->pEdit)
#define EDIT_OBJ(ch, obj)		(obj = (OBJ_INDEX_DATA *)ch->desc->pEdit)
#define EDIT_ROOM(ch, room)		(room = ch->in_room)
#define EDIT_AREA(ch, area)		(area = (AREA_DATA *)ch->desc->pEdit)


/*
 * Connected states for editor.
 */
#define ED_AREA					1
#define ED_ROOM					2
#define ED_OBJECT				3
#define ED_MOBILE				4

/*
 * OLC Constants
 */
#define MAX_MOB					1	/* Default maximum number for resetting mobs */


/*
 * Structure for an OLC editor command.
 */
struct olc_cmd_type
{
	const char *name;
	OLC_FUN *olc_fun;
};

/*
 * Structure for an OLC editor startup command.
 */
struct editor_cmd_type
{
	const char *name;
	DO_FUN *do_fun;
};

//
// TODO: UNKNOWN FUNCTIONS
//

//bool medit_tarifa(CHAR_DATA *ch, char *argument);		/* ROM */
//bool medit_camino(CHAR_DATA *ch, char *argument);		/* ROM */
//bool medit_spec(CHAR_DATA *ch, char *argument);
//bool oedit_affect(CHAR_DATA *ch, char *argument);		/* ROM */


//
// LOCAL FUNCTIONS
//


/* Executed from comm.c.  Minimizes compiling when changes are made. */
bool run_olc_editor(DESCRIPTOR_DATA *d);
bool is_editing(CHAR_DATA *ch);
char *olc_ed_name(CHAR_DATA *ch);
char *olc_ed_vnum(CHAR_DATA *ch);
/*****************************************************************************
 Name:		show_olc_cmds
 Purpose:	Format up the commands from given table.
 Called by:	show_commands(olc_act.c).
 ****************************************************************************/
void show_olc_cmds(CHAR_DATA *ch, const struct olc_cmd_type *olc_table);
/*****************************************************************************
 Name:		show_commands
 Purpose:	Display all olc commands.
 Called by:	olc interpreters.
 ****************************************************************************/
bool show_commands(CHAR_DATA *ch, char *argument);
/*****************************************************************************
 Name:		get_area_data
 Purpose:	Returns pointer to area with given vnum.
 Called by:	do_aedit(olc.c).
 ****************************************************************************/
AREA_DATA *get_area_data(int vnum);
/*****************************************************************************
 Name:		edit_done
 Purpose:	Resets builder information on completion.
 Called by:	aedit, redit, oedit, medit(olc.c)
 ****************************************************************************/
bool edit_done(CHAR_DATA *ch);
/* Area Interpreter, called by do_aedit. */
void aedit(CHAR_DATA *ch, char *argument);
/* Room Interpreter, called by do_redit. */
void redit(CHAR_DATA *ch, char *argument);
/* Object Interpreter, called by do_oedit. */
void oedit(CHAR_DATA *ch, char *argument);
/* Mobile Interpreter, called by do_medit. */
void medit(CHAR_DATA *ch, char *argument);
int get_security(CHAR_DATA *ch);
bool check_security(CHAR_DATA *ch);
/* Entry point for all editors. */
void do_olc(CHAR_DATA *ch, char *argument);
/* Entry point for editing area_data. */
void do_aedit(CHAR_DATA *ch, char *argument);
/* Entry point for editing room_index_data. */
void do_redit(CHAR_DATA *ch, char *argument);
/* Entry point for editing obj_index_data. */
void do_oedit(CHAR_DATA *ch, char *argument);
/* Entry point for editing mob_index_data. */
void do_medit(CHAR_DATA *ch, char *argument);
void display_resets(CHAR_DATA *ch);
/*****************************************************************************
 Name:		add_reset
 Purpose:	Inserts a new reset in the given index slot.
 Called by:	do_resets(olc.c).
 ****************************************************************************/
void add_reset(ROOM_INDEX_DATA *room, RESET_DATA *pReset, int index);
void do_resets(CHAR_DATA *ch, char *argument);
/*****************************************************************************
 Name:		do_alist
 Purpose:	Normal command to list areas and display area information.
 Called by:	interpreter(interp.c)
 ****************************************************************************/
void do_alist(CHAR_DATA *ch, char *argument);

#endif /* OLC_H */
