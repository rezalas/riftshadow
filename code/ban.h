#ifndef BAN_H
#define BAN_H

#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "merc.h"
#include "recycle.h"
#include "time.h"
#include "comm.h"
#include "act_wiz.h"
#include "interp.h"
#include "db.h"

//
// LOCAL FUNCTIONS
//

bool check_ban (char *usite, int type, int host);
void do_ban (CHAR_DATA *ch, char *argument);
void do_unban (CHAR_DATA *ch, char *argument);

#endif /* BAN_H */
