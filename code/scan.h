#ifndef SCAN_H
#define SCAN_H

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

extern void show_char_to_char_0 (CHAR_DATA *victim, CHAR_DATA *ch);

void scan_list (ROOM_INDEX_DATA *scan_room, CHAR_DATA *ch, sh_int depth, sh_int door);
void do_scan(CHAR_DATA *ch, char *argument);

#endif /* SCAN_H */
