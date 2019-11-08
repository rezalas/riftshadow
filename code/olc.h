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
#define VERSION	"ILAB Online Creation [Beta 1.0, ROM 2.3 modified]\n\r" \
				"     Port a ROM 2.4 v1.00\n\r"
#define AUTHOR	"     By Jason(jdinkel@mines.colorado.edu)\n\r" \
				"     Modified for use with ROM 2.3\n\r"        \
				"     By Hans Birkeland (hansbi@ifi.uio.no)\n\r" \
				"     Modified for use with Riftshadow\n\r" \
				"     By Chad Sorrell (gabe@qhcf.net)\n\r" 
#define DATE	"     (Apr. 7, 1995 - ROM mod, Apr 16, 1995)\n\r" \
				"     (Port a ROM 2.4 - Nov 2, 1996)\n\r" \
				"     (Riftshadow Mod - June 1, 2001)\n\r"
#define CREDITS "     Original by Surreality(cxw197@psu.edu) and Locke(locke@lm.com)"


/*
 * New typedefs.
 */
typedef	bool OLC_FUN (CHAR_DATA *ch, char *argument);


/*
 * Macros
 */

#define DECLARE_OLC_FUN( fun )	OLC_FUN    fun

#define IS_BUILDER(ch, Area)	( !IS_SWITCHED( ch ) && ( (ch->pcdata->security <= Area->security && \
				strstr( Area->builders, ch->name )) || (ch->pcdata->security > Area->security) \
				|| (strstr( Area->builders, "All" ))))

//#define IS_BUILDER(ch, Area)	( 1 == 1 )

/* Return pointers to what is being edited. */
#define EDIT_MOB(Ch, Mob)	( Mob = (MOB_INDEX_DATA *)Ch->desc->pEdit )
#define EDIT_OBJ(Ch, Obj)	( Obj = (OBJ_INDEX_DATA *)Ch->desc->pEdit )
#define EDIT_ROOM(Ch, Room)	( Room = Ch->in_room )
#define EDIT_AREA(Ch, Area)	( Area = (AREA_DATA *)Ch->desc->pEdit )


/*
 * Connected states for editor.
 */
#define ED_AREA		1
#define ED_ROOM		2
#define ED_OBJECT	3
#define ED_MOBILE	4

/*
 * OLC Constants
 */
#define MAX_MOB		1	/* Default maximum number for resetting mobs */


/*
 * Interpreter Prototypes
 */
void aedit (CHAR_DATA *ch, char *argument);
void redit (CHAR_DATA *ch, char *argument);
void medit (CHAR_DATA *ch, char *argument);
void oedit (CHAR_DATA *ch, char *argument);


/*
 * Structure for an OLC editor command.
 */
struct olc_cmd_type
{
	const char * name;
	OLC_FUN * olc_fun;
};

/*
 * Structure for an OLC editor startup command.
 */
struct editor_cmd_type
{
	const char * name;
	DO_FUN * do_fun;
};


/*
 * Command procedures needed ROM OLC
 */
DECLARE_DO_FUN (do_help);
DECLARE_SPELL_FUN (spell_null);


/*
 * Editor Commands.
 */
DECLARE_DO_FUN (do_aedit);
DECLARE_DO_FUN (do_redit);
DECLARE_DO_FUN (do_oedit);
DECLARE_DO_FUN (do_medit);


/*
 * General Functions
 */
bool show_commands (CHAR_DATA *ch, char *argument);
bool show_help (CHAR_DATA *ch, char *argument);
bool edit_done (CHAR_DATA *ch);
bool show_version (CHAR_DATA *ch, char *argument);



/*
 * Area Editor Prototypes
 */
DECLARE_OLC_FUN (aedit_show);
DECLARE_OLC_FUN (aedit_create);
DECLARE_OLC_FUN (aedit_name);
DECLARE_OLC_FUN (aedit_file);
DECLARE_OLC_FUN (aedit_age);
DECLARE_OLC_FUN (aedit_security);
DECLARE_OLC_FUN (aedit_reset);
DECLARE_OLC_FUN (aedit_builder);
DECLARE_OLC_FUN (aedit_vnum);
DECLARE_OLC_FUN (aedit_lvnum);
DECLARE_OLC_FUN (aedit_uvnum);
DECLARE_OLC_FUN (aedit_climate);
DECLARE_OLC_FUN (aedit_prog);
DECLARE_OLC_FUN (aedit_credits);
DECLARE_OLC_FUN (aedit_level);
DECLARE_OLC_FUN (aedit_type);

/*
 * Room Editor Prototypes
 */
DECLARE_OLC_FUN (redit_show);
DECLARE_OLC_FUN (redit_create);
DECLARE_OLC_FUN (redit_name);
DECLARE_OLC_FUN (redit_desc);
DECLARE_OLC_FUN (redit_ed);
DECLARE_OLC_FUN (redit_format);
DECLARE_OLC_FUN (redit_north);
DECLARE_OLC_FUN (redit_south);
DECLARE_OLC_FUN (redit_east);
DECLARE_OLC_FUN (redit_west);
DECLARE_OLC_FUN (redit_up);
DECLARE_OLC_FUN (redit_down);
DECLARE_OLC_FUN (redit_mreset);
DECLARE_OLC_FUN (redit_oreset);
DECLARE_OLC_FUN (redit_mlist);
DECLARE_OLC_FUN (redit_rlist);
DECLARE_OLC_FUN (redit_olist);
DECLARE_OLC_FUN (redit_mshow);
DECLARE_OLC_FUN (redit_oshow);
DECLARE_OLC_FUN (redit_heal);
DECLARE_OLC_FUN (redit_mana);
DECLARE_OLC_FUN (redit_clan);
DECLARE_OLC_FUN (redit_prog);
DECLARE_OLC_FUN (redit_cabal);
DECLARE_OLC_FUN (redit_owner);
DECLARE_OLC_FUN (redit_trap);
DECLARE_OLC_FUN (redit_mview);
DECLARE_OLC_FUN (redit_altdesc);
/*
 * Object Editor Prototypes
 */
DECLARE_OLC_FUN (oedit_show);
DECLARE_OLC_FUN (oedit_create);
DECLARE_OLC_FUN (oedit_name);
DECLARE_OLC_FUN (oedit_short);
DECLARE_OLC_FUN (oedit_long);
DECLARE_OLC_FUN (oedit_addapply);
DECLARE_OLC_FUN (oedit_delapply);
DECLARE_OLC_FUN (oedit_value0);
DECLARE_OLC_FUN (oedit_value1);
DECLARE_OLC_FUN (oedit_value2);
DECLARE_OLC_FUN (oedit_value3);
DECLARE_OLC_FUN (oedit_value4);		/* ROM */
DECLARE_OLC_FUN (oedit_weight);
DECLARE_OLC_FUN (oedit_cost);
DECLARE_OLC_FUN (oedit_ed);
DECLARE_OLC_FUN (oedit_verb);
DECLARE_OLC_FUN (oedit_extra);		/* ROM */
DECLARE_OLC_FUN (oedit_wear);		/* ROM */
DECLARE_OLC_FUN (oedit_type);		/* ROM */
DECLARE_OLC_FUN (oedit_affect);		/* ROM */
DECLARE_OLC_FUN (oedit_material);	/* ROM */
DECLARE_OLC_FUN (oedit_level);		/* ROM */
DECLARE_OLC_FUN (oedit_condition);	/* ROM */
DECLARE_OLC_FUN (oedit_liqlist);
DECLARE_OLC_FUN (oedit_prog);
DECLARE_OLC_FUN (oedit_msg);
DECLARE_OLC_FUN (oedit_notes);
DECLARE_OLC_FUN (oedit_restrict);
DECLARE_OLC_FUN (oedit_limit);
DECLARE_OLC_FUN (oedit_timer);
DECLARE_OLC_FUN (oedit_cabal);
DECLARE_OLC_FUN (oedit_flag);
DECLARE_OLC_FUN (oedit_spec);
DECLARE_OLC_FUN (oedit_wear_name);

/*
 * Mobile Editor Prototypes
 */
DECLARE_OLC_FUN (medit_show);
DECLARE_OLC_FUN (medit_create);
DECLARE_OLC_FUN (medit_name);
DECLARE_OLC_FUN (medit_short);
DECLARE_OLC_FUN (medit_long);
DECLARE_OLC_FUN (medit_shop);
DECLARE_OLC_FUN (medit_desc);
DECLARE_OLC_FUN (medit_level);
DECLARE_OLC_FUN (medit_align);
DECLARE_OLC_FUN (medit_spec);
DECLARE_OLC_FUN (medit_cabal);
DECLARE_OLC_FUN (medit_sex);		/* ROM */
DECLARE_OLC_FUN (medit_act);		/* ROM */
DECLARE_OLC_FUN (medit_affect);		/* ROM */
DECLARE_OLC_FUN (medit_ac);			/* ROM */
DECLARE_OLC_FUN (medit_form);		/* ROM */
DECLARE_OLC_FUN (medit_part);		/* ROM */
DECLARE_OLC_FUN (medit_imm);		/* ROM */
DECLARE_OLC_FUN (medit_res);		/* ROM */
DECLARE_OLC_FUN (medit_vuln);		/* ROM */
DECLARE_OLC_FUN (medit_material);	/* ROM */
DECLARE_OLC_FUN (medit_off);		/* ROM */
DECLARE_OLC_FUN (medit_size);		/* ROM */
DECLARE_OLC_FUN (medit_hitdice);	/* ROM */
DECLARE_OLC_FUN (medit_manadice);	/* ROM */
DECLARE_OLC_FUN (medit_damdice);	/* ROM */
DECLARE_OLC_FUN (medit_race);		/* ROM */
DECLARE_OLC_FUN (medit_position);	/* ROM */
DECLARE_OLC_FUN (medit_gold);		/* ROM */
DECLARE_OLC_FUN (medit_tarifa);		/* ROM */
DECLARE_OLC_FUN (medit_hitroll);	/* ROM */
DECLARE_OLC_FUN (medit_camino);		/* ROM */
DECLARE_OLC_FUN (medit_damtype);	/* ROM */
DECLARE_OLC_FUN (medit_prog);
DECLARE_OLC_FUN (medit_speech );
DECLARE_OLC_FUN (medit_class);
DECLARE_OLC_FUN (medit_notes);
DECLARE_OLC_FUN (medit_limit);
DECLARE_OLC_FUN (medit_optional);
DECLARE_OLC_FUN (medit_group);
DECLARE_OLC_FUN (medit_vnum);
DECLARE_OLC_FUN (medit_yell);

/*
 * Prototypes
 */
AREA_DATA *get_area_data (int vnum);
void add_reset (ROOM_INDEX_DATA *room, RESET_DATA *pReset, int index);
bool check_security(CHAR_DATA *ch);
int get_security(CHAR_DATA *ch);

bool run_olc_editor (DESCRIPTOR_DATA *d);
bool is_editing (CHAR_DATA *ch);
char *olc_ed_name (CHAR_DATA *ch);
char *olc_ed_vnum (CHAR_DATA *ch);

#endif /* OLC_H */
