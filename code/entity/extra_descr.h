#ifndef ENTITY_EXTRA_DESCR_H
#define ENTITY_EXTRA_DESCR_H

#include "fwd.h"

//
// Extra description data for a room or object.
//

struct extra_descr_data
{
	EXTRA_DESCR_DATA *next;				// Next in list
	bool valid;
	char *keyword;						// Keyword in look/examine
	char *description;					// What to see
};

#endif /* ENTITY_EXTRA_DESCR_H */
