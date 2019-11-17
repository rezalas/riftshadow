/***************************************************************************
 *  File: bit.c                                                            *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 *                                                                         *
 *  This code was written by Jason Dinkel and inspired by Russ Taylor,     *
 *  and has been used here for OLC - OLC would not be what it is without   *
 *  all the previous coders who released their source code.                *
 *                                                                         *
 ***************************************************************************/
/*
 The code below uses a table lookup system that is based on suggestions
 from Russ Taylor.  There are many routines in handler.c that would benefit
 with the use of tables.  You may consider simplifying your code base by
 implementing a system like below with such functions. -Jason Dinkel
 */

#include "bit.h"

/*****************************************************************************
 Name:		flag_stat_table
 Purpose:	This table catagorizes the tables following the lookup
 		functions below into stats and flags.  Flags can be toggled
 		but stats can only be assigned.  Update this table when a
 		new set of flags is installed.
 ****************************************************************************/
const struct flag_stat_type flag_stat_table[] =
{
	{ area_flags, false	},
	{ sex_flags, true	},
	{ exit_flags, false	},
	{ door_resets, true	},
	{ room_flags, false	},
	{ sector_flags, true },
	{ type_flags, true },
	{ extra_flags, false },
	{ wear_flags, false },
	{ act_flags, false },
	{ affect_flags, false },
	{ apply_flags, true },
	{ wear_loc_flags, true },
	{ wear_loc_strings, true },
	{ weapon_flags, true },
	{ container_flags, false },

	/* ROM specific flags: */

	{ material_type, true },
	{ form_flags, false },
	{ part_flags, false },
	{ ac_type, true },
	{ size_flags, true },
	{ position_flags, true },
	{ off_flags, false },
	{ imm_flags, false },
	{ res_flags, false },
	{ vuln_flags, false },
	{ weapon_class, true },
	{ weapon_type2, false },
	{ 0, 0 }
};

/*****************************************************************************
 Name:		is_stat( table )
 Purpose:	Returns true if the table is a stat table and FALSE if flag.
 Called by:	flag_value and flag_string.
 Note:		This function is local and used only in bit.c.
 ****************************************************************************/
bool is_stat(const struct flag_type *flag_table)
{
	int flag;

	for (flag = 0; flag_stat_table[flag].structure; flag++)
	{
		if (flag_stat_table[flag].structure == flag_table && flag_stat_table[flag].stat)
			return true;
	}

	return false;
}

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
int flag_lookup2(const char *name, const struct flag_type *flag_table)
{
	int flag;

	for (flag = 0; flag_table[flag].name != NULL; flag++)
	{
		if (!str_cmp(name, flag_table[flag].name) && flag_table[flag].settable)
			return flag_table[flag].bit;
	}

	return NO_FLAG;
}

/*****************************************************************************
 Name:		flag_value( table, flag )
 Purpose:	Returns the value of the flags entered.  Multi-flags accepted.
 Called by:	olc.c and olc_act.c.
 ****************************************************************************/
int flag_value(const struct flag_type *flag_table, char *argument)
{
	char word[MAX_INPUT_LENGTH];
	int bit;

	one_argument(argument, word);

	if ((bit = flag_lookup2(word, flag_table)) != NO_FLAG)
		return bit;
	else
		return NO_FLAG;
}

/*****************************************************************************
 Name:		flag_string( table, flags/stat )
 Purpose:	Returns string with name(s) of the flags or stat entered.
 Called by:	act_olc.c, olc.c, and olc_save.c.
 ****************************************************************************/
char *flag_string(const struct flag_type *flag_table, long bits[])
{
	static char buf[512];
	int flag;

	buf[0] = '\0';

	for (flag = 0; flag_table[flag].name != NULL; flag++)
	{
		if (!is_stat(flag_table) && IS_SET(bits, flag_table[flag].bit))
		{
			strcat(buf, " ");
			strcat(buf, flag_table[flag].name);
		}
		else if (IS_SET(bits, flag_table[flag].bit))
		{
			strcat(buf, " ");
			strcat(buf, flag_table[flag].name);
			break;
		}
	}

	return (buf[0] != '\0') ? buf + 1 : (char *)"none";
}

char *style_string(const struct style_type *style_table, long bits[])
{
	static char buf[512];
	int flag;

	buf[0] = '\0';

	for (flag = 0; flag < MAX_STYLE; flag++)
	{
		if (IS_SET(bits, style_table[flag].bit))
		{
			if (flag > 0)
				strcat(buf, " ");

			strcat(buf, style_table[flag].name);
		}
	}

	return (buf[0] != '\0') ? buf + 1 : (char *)"none";
}

char *restrict_string(const struct restrict_type *restrict_table, long bits[])
{
	static char buf[512];
	int flag;

	buf[0] = '\0';

	for (flag = 0; restrict_table[flag].name != NULL; flag++)
	{
		if (IS_SET(bits, restrict_table[flag].bit))
		{
			if (flag > 0)
				strcat(buf, " ");

			strcat(buf, restrict_table[flag].name);
		}
	}

	return (buf[0] != '\0') ? buf + 1 : (char *)"none";
}

char *flag_string_old(const struct flag_type *flag_table, int bits)
{
	static char buf[512];
	int flag;

	buf[0] = '\0';

	for (flag = 0; flag_table[flag].name != NULL; flag++)
	{
		if (!is_stat(flag_table) && IS_SET_OLD(bits, flag_table[flag].bit))
		{
			strcat(buf, " ");
			strcat(buf, flag_table[flag].name);
		}
		else if (flag_table[flag].bit == bits)
		{
			strcat(buf, " ");
			strcat(buf, flag_table[flag].name);
			break;
		}
	}

	return (buf[0] != '\0') ? buf + 1 : (char *)"none";
}
