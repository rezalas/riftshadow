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

/* command procedures needed */
DECLARE_DO_FUN(do_look);
DECLARE_DO_FUN(do_stand);
DECLARE_SPELL_FUN(spell_teleport);

/* random room generation procedure */
ROOM_INDEX_DATA *get_random_room(CHAR_DATA *ch);
/* RT Enter portals */
void do_enter( CHAR_DATA *ch, char *argument);

#endif /* ACT_ENTE_H */
