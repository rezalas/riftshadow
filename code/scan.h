#ifndef SCAN_H
#define SCAN_H

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "handler.h"
#include "act_info.h"
#include "zealot.h"
#include "comm.h"
#include "interp.h"
#include "db.h"
#include "utility.h"

//
// LOCAL FUNCTIONS
//

void scan_list (ROOM_INDEX_DATA *scan_room, CHAR_DATA *ch, sh_int depth, sh_int door);
void do_scan(CHAR_DATA *ch, char *argument);

#endif /* SCAN_H */
