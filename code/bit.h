#ifndef BIT_H
#define BIT_H

#ifdef macintosh
#include <types.h>
#else
#include <sys/types.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "tables.h"


struct flag_stat_type
{
	const struct flag_type *structure;
	bool stat;
};



/*****************************************************************************
 Name:		flag_stat_table
 Purpose:	This table catagorizes the tables following the lookup
 		functions below into stats and flags.  Flags can be toggled
 		but stats can only be assigned.  Update this table when a
 		new set of flags is installed.
 ****************************************************************************/
const struct flag_stat_type flag_stat_table[] =
{
	{ area_flags, FALSE	},
	{ sex_flags, TRUE	},
	{ exit_flags, FALSE	},
	{ door_resets, TRUE	},
	{ room_flags, FALSE	},
	{ sector_flags, TRUE },
	{ type_flags, TRUE },
	{ extra_flags, FALSE },
	{ wear_flags, FALSE },
	{ act_flags, FALSE },
	{ affect_flags, FALSE },
	{ apply_flags, TRUE },
	{ wear_loc_flags, TRUE },
	{ wear_loc_strings, TRUE },
	{ weapon_flags, TRUE },
	{ container_flags, FALSE },

	/* ROM specific flags: */

	{ material_type, TRUE },
	{ form_flags, FALSE },
	{ part_flags, FALSE },
	{ ac_type, TRUE },
	{ size_flags, TRUE },
	{ position_flags, TRUE },
	{ off_flags, FALSE },
	{ imm_flags, FALSE },
	{ res_flags, FALSE },
	{ vuln_flags, FALSE },
	{ weapon_class, TRUE },
	{ weapon_type2, FALSE },
	{ 0, 0 }
};

/*****************************************************************************
 Name:		is_stat( table )
 Purpose:	Returns TRUE if the table is a stat table and FALSE if flag.
 Called by:	flag_value and flag_string.
 Note:		This function is local and used only in bit.c.
 ****************************************************************************/
bool is_stat (const struct flag_type *flag_table);
/*
 * This function is Russ Taylor's creation.  Thanks Russ!
 * All code copyright (C) Russ Taylor, permission to use and/or distribute
 * has NOT been granted.  Use only in this OLC package has been granted.
 */
/*****************************************************************************
 Name:		flag_lookup2( flag, table )
 Purpose:	Returns the value of a single, settable flag from the table.
 Called by:	flag_value and flag_string.
 Note:		This function is local and used only in bit.c.
 ****************************************************************************/
int flag_lookup2 (const char *name, const struct flag_type *flag_table);
/*****************************************************************************
 Name:		flag_value( table, flag )
 Purpose:	Returns the value of the flags entered.  Multi-flags accepted.
 Called by:	olc.c and olc_act.c.
 ****************************************************************************/
int flag_value (const struct flag_type *flag_table, char *argument);
/*****************************************************************************
 Name:		flag_string( table, flags/stat )
 Purpose:	Returns string with name(s) of the flags or stat entered.
 Called by:	act_olc.c, olc.c, and olc_save.c.
 ****************************************************************************/
char *flag_string (const struct flag_type *flag_table, long bits[]);
char *style_string (const struct style_type *style_table, long bits[]);
char *restrict_string (const struct restrict_type *restrict_table, long bits[]);
char *flag_string_old (const struct flag_type *flag_table, int bits);

#endif /* BIT_H */
