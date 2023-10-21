#ifndef MATERIAL_H
#define MATERIAL_H

#include <vector>
#include "merc.h"

//
// Liquids.
//

#define LIQ_WATER					0

struct liq_type
{
	char * liq_name;
	char * liq_color;
	short liq_affect[5];
};

struct material_type
{
	char * mat_name;
	short mat_phase;
	short mat_hardness;
	short mat_rarity;
	short mat_conductivity;
	short mat_flammability;
	short mat_density;
	long mat_flags;
};

extern const struct material_type material_table[];
extern const std::vector<liq_type> liq_table;


#endif /* MATERIAL_H */

