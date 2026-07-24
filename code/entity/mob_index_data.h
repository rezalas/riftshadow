#ifndef ENTITY_MOB_INDEX_DATA_H
#define ENTITY_MOB_INDEX_DATA_H

#include "fwd.h"
#include "limits.h"
#include "spec_func.h"
#include "../characterClasses/class.h"		// CClass, used by the inline accessors

//
// Prototype for a mob.
// This is the in-memory version of #MOBILES.
//
class mob_index_data
{
public:
	MOB_INDEX_DATA *next;
	SHOP_DATA *pShop;
	GAME_FUN *game_fun;
	BARRED_DATA *barred_entry; //dev
	MPROG_DATA *mprogs;
	AREA_DATA *area;
	SPEECH_DATA *speech;
	long progtypes[MAX_BITVECTOR];
	short vnum;
	short group;
	bool new_format;
	short count;
	short killed;
	char *player_name;
	char *short_descr;
	char *long_descr;
	char *description;
	long act[MAX_BITVECTOR];
	long affected_by[MAX_BITVECTOR];
	short alignment;
	short level;
	short hitroll;
	short hit[3];
	short mana[3];
	short damage[3];
	short ac[4];
	short dam_type;
	long off_flags[MAX_BITVECTOR];
	long imm_flags[MAX_BITVECTOR];
	long res_flags[MAX_BITVECTOR];
	long vuln_flags[MAX_BITVECTOR];
	long mprog_flags[MAX_BITVECTOR];
	short xp_mod;
	float dam_mod;
	short start_pos;
	short sex;
	short race;
	int wealth;
	long form[MAX_BITVECTOR];
	long parts[MAX_BITVECTOR];
	short size;

	// restrict mobs to a specific region

	int restrict_low;	// Low end of vnum range 
	int restrict_high;	// High end of vnum range

	short affect_sn[MAX_MOB_AFFECT];
	long affect_bit[MAX_MOB_AFFECT];
	char *cast_spell[MAX_MOB_CAST];

	CClass *Class()
	{
		return my_class;
	}

	void SetClass(int nClassIndex)
	{
		my_class = CClass::GetClass(nClassIndex);
	}

	short cabal;
	long styles[MAX_BITVECTOR];
	short ele_major;
	short ele_para;
	short arms;
	short legs;
	char *notes;
	int swealth;
	char *attack_yell;
	short profs_taught[MAX_PROFS_TAUGHT_BY_MOB];
	SPEC_FUNC spec_prog;
private:
	CClass *my_class;
};

#endif /* ENTITY_MOB_INDEX_DATA_H */
