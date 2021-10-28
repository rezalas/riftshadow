#ifndef STRING_H
#define STRING_H

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
#include "newmem.h"
#include "comm.h"
#include "interp.h"
#include "db.h"
#include "./include/fmt/format.h"

//
// LOCAL FUNCTIONS
//

/*****************************************************************************
 Name:		string_edit
 Purpose:	Clears string and puts player into editing mode.
 Called by:	none
 ****************************************************************************/
void string_edit(CHAR_DATA *ch, char **pString);
/*****************************************************************************
 Name:		string_append
 Purpose:	Puts player into append mode for given string.
 Called by:	(many)olc_act.c
 ****************************************************************************/
void string_append(CHAR_DATA *ch, char **pString);
/*****************************************************************************
 Name:		string_replace
 Purpose:	Substitutes one string for another.
 Called by:	string_add(string.c) (aedit_builder)olc_act.c.
 ****************************************************************************/
char *string_replace(char *orig, char *old, char *newstr);
/*****************************************************************************
 Name:		string_add
 Purpose:	Interpreter for string editing.
 Called by:	game_loop_xxxx(comm.c).
 ****************************************************************************/
void string_add(CHAR_DATA *ch, char *argument);
/*
 * Thanks to Kalgen for the new procedure (no more bug!)
 * Original wordwrap() written by Surreality.
 */
/*****************************************************************************
 Name:		format_string
 Purpose:	Special string formating and word-wrapping.
 Called by:	string_add(string.c) (many)olc_act.c
 ****************************************************************************/
char *format_string(char *oldstring /*, bool fSpace */);
/*
 * Used above in string_add.  Because this function does not
 * modify case if fCase is false and because it understands
 * parenthesis, it would probably make a nice replacement
 * for one_argument.
 */
/*****************************************************************************
 Name:		first_arg
 Purpose:	Pick off one argument from a string and return the rest.
		Understands quates, parenthesis (barring ) ('s) and
		percentages.
 Called by:	string_add(string.c)
 ****************************************************************************/
char *first_arg(char *argument, char *arg_first, bool fCase);
/*
 * Used in olc_act.c for aedit_builders.
 */
char *string_unpad(char *argument);
/*
 * Same as capitalize but changes the pointer's data.
 * Used in olc_act.c in aedit_builder.
 */
char *string_proper(char *argument);

#endif /* STRING_H */
