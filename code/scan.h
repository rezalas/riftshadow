#ifndef SCAN_H
#define SCAN_H

#include "merc.h"

//
// LOCAL FUNCTIONS
//

void scan_list (ROOM_INDEX_DATA *scan_room, CHAR_DATA *ch, sh_int depth, sh_int door);
void do_scan(CHAR_DATA *ch, char *argument);

#endif /* SCAN_H */
