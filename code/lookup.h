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
 ***************************************************************************/

/***************************************************************************
 *       Tartarus code is copyright (C) 1997-1998 by Daniel Graham         *
 *  In using this code you agree to comply with the Tartarus license       *
 *       found in the file /Tartarus/doc/tartarus.doc                      *
 ***************************************************************************/

#ifndef LOOKUP_H
#define LOOKUP_H

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "merc.h"
#include "tables.h"
#include "newmem.h"
#include "db.h"

int flag_lookup (const char *name, const struct flag_type *flag_table);
int climate_lookup (const char *name);
int restrict_lookup (const char *name);
RACE_DATA *race_data_lookup (const int race);
int display_lookup (const char *name, const struct display_type *flag_table);
char * get_demon_names (CHAR_DATA *ch);
char * display_name_lookup (long bitv, const struct display_type *flag_table);
char * restrict_name_lookup (long bitv);
char * tribe_name_lookup (long bitv);
int ele_name_lookup (const char *name);
char * wealth_lookup (int number);
char * flag_name_lookup (long bitv, const struct flag_type *flag_table);
int flag_index_ilookup (int i, const struct flag_type *flag_table);
int flag_index_lookup (const char *name, const struct flag_type *flag_table);
int material_lookup (const char *name);
int sect_lookup (const char *name);
int sect_numlookup (int number);
int cabal_lookup  (const char *name);
int position_lookup	(const char *name);
int sex_lookup (const char *name);
int size_lookup	(const char *name);
int hometown_lookup (const char *name);
/* Function to generate the number for the direction by passing the word, GABE */
int direction_lookup (char *dir);
/* chop the string into two variables */
void half_chop (const char *string, char *arg1, char *arg2);
char * chaldir (int dir);

#endif /* LOOKUP_H */
