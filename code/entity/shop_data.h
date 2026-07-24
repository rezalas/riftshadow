#ifndef ENTITY_SHOP_DATA_H
#define ENTITY_SHOP_DATA_H

#include "fwd.h"

//
// Shop types.
//

#define MAX_TRADE					5

struct shop_data
{
	bool fIsPetShop;
	int pet_cage_vnum;
	SHOP_DATA *next;			// Next shop in list
	MOB_INDEX_DATA *pMobIndex;
	short open_hour;			// First opening hour
	short close_hour;			// First closing hour
	short direction;			// exit dir
};

#endif /* ENTITY_SHOP_DATA_H */
