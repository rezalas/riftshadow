#ifndef HELP_H
#define HELP_H

#include "merc.h"

//
// LOCAL FUNCTIONS
//

struct Help;	// code/repositories/helprepository.h

int can_see_help (CHAR_DATA *ch, const Help &help, bool fOnlyResult);
void show_helpfile (CHAR_DATA *ch, const Help &help);
void modhelp_end_fun (CHAR_DATA *ch, char *argument);
void do_modhelp (CHAR_DATA *ch, char *argument);
void do_help (CHAR_DATA *ch, char *argument);
void do_delhelp (CHAR_DATA *ch, char *argument);
void addhelp_end_fun (CHAR_DATA *ch, char *argument);
void do_addhelp (CHAR_DATA *ch, char *argument);
void do_listhelp (CHAR_DATA *ch, char *argument);

#endif /* HELP_H */
