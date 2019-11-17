#ifndef ACT_ENTE_H
#define ACT_ENTE_H

#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "merc.h"
#include "handler.h"
#include "comm.h"
#include "db.h"
#include "act_info.h"
#include "act_move.h"

//
// LOCAL FUNCTIONS
//

ROOM_INDEX_DATA *get_random_room(CHAR_DATA *ch);
void do_enter(CHAR_DATA *ch, char *argument);

#endif /* ACT_ENTE_H */
