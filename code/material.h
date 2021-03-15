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
	sh_int liq_affect[5];
};

struct material_type
{
	char * mat_name;
	sh_int mat_phase;
	sh_int mat_hardness;
	sh_int mat_rarity;
	sh_int mat_conductivity;
	sh_int mat_flammability;
	sh_int mat_density;
	long mat_flags;
};

extern const struct material_type material_table[];
extern const std::vector<liq_type> liq_table;


#endif /* MATERIAL_H */

