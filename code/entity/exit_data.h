#ifndef ENTITY_EXIT_DATA_H
#define ENTITY_EXIT_DATA_H

#include "fwd.h"
#include "limits.h"

//
// Exit data.
//

struct exit_data
{
	union
	{
		ROOM_INDEX_DATA *to_room;
		short vnum;
	} u1;

	long exit_info[MAX_BITVECTOR];
	short key;
	char *keyword;
	char *description;
	EXIT_DATA *next;					// OLC
	int orig_door;						// OLC
	RUNE_DATA *rune;
	bool has_rune;
};

#endif /* ENTITY_EXIT_DATA_H */
