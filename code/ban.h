#ifndef BAN_H
#define BAN_H

#include "merc.h"

//
// LOCAL FUNCTIONS
//

bool check_ban (char *usite, int type, int host);
void do_ban (CHAR_DATA *ch, char *argument);
void do_unban (CHAR_DATA *ch, char *argument);

#endif /* BAN_H */
