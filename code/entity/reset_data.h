#ifndef ENTITY_RESET_DATA_H
#define ENTITY_RESET_DATA_H

#include "fwd.h"

//
// Reset commands:
//   '*': comment
//   'M': read a mobile
//   'O': read an object
//   'P': put object in object
//   'G': give object to mobile
//   'E': equip object to mobile
//   'D': set state of door
//   'R': randomize room exits
//   'S': stop (end of list)
//

//
// Area-reset definition.
//

struct reset_data
{
	RESET_DATA *next;
	char command;
	short arg1;
	short arg2;
	short arg3;
	short arg4;
};

#endif /* ENTITY_RESET_DATA_H */
