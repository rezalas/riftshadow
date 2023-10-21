#ifndef SCAN_H
#define SCAN_H

#include "merc.h"

//
// LOCAL FUNCTIONS
//

void scan_list (ROOM_INDEX_DATA *scan_room, CHAR_DATA *ch, short depth, short door);
void do_scan(CHAR_DATA *ch, char *argument);

#endif /* SCAN_H */
