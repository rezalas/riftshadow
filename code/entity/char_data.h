#ifndef ENTITY_CHAR_DATA_H
#define ENTITY_CHAR_DATA_H

#include <time.h>

#include "fwd.h"
#include "limits.h"
#include "../enums.h"					// CLASS_NONE
#include "../characterClasses/class.h"	// CClass, used by the inline accessors
#include "mob_index_data.h"				// Class() falls back through pIndexData
#include "pc_data.h"					// Profs() reaches into pcdata

//
// One character (PC or NPC).
//


extern CProficiencies prof_none; //empty proficiencies for jackasses who are going to ref ch->Profs() without checking IS_NPC

class	char_data
{
public:
	CHAR_DATA *next;
	CHAR_DATA *next_in_room;
	CHAR_DATA *master;
	CHAR_DATA *leader;
	CHAR_DATA *fighting;
	CHAR_DATA *reply;
	CHAR_DATA *pet;
	CHAR_DATA *last_fought;
	int tracktimer;
	time_t last_fight_time;
	char * last_fight_name;
	CHAR_DATA *hunting;
	CHAR_DATA *defending;
	MEM_DATA *memory;
	GAME_FUN *game_fun;
	MOB_INDEX_DATA *pIndexData;
	DESCRIPTOR_DATA *desc;
	AFFECT_DATA *affected;
	NOTE_DATA *pnote;
	OBJ_DATA *carrying;
	OBJ_DATA *on;
	ROOM_INDEX_DATA *in_room;
	ROOM_INDEX_DATA *was_in_room;
	AREA_DATA *zone;
	PC_DATA *pcdata;
	GEN_DATA *gen_data;
	PATHFIND_DATA *path;	// For smart pathfinding/tracking.  Mob only.
	PATHFIND_DATA *best;	// Stores best direction thus far.  Mob only.
	bool valid;
	char *name;
	char *true_name;
	long id;
	short version;
	char *short_descr;
	char *long_descr;
	char *description;
	char *prompt;
	char *prefix;
	short group;
	short cabal;
	short sex;

	CClass *Class()
	{
		return !my_class
			? (pIndexData && pIndexData->Class()
				? pIndexData->Class()
				:  CClass::GetClass(CLASS_NONE))
			: my_class;
	}

	void		SetClass(int nClassIndex)
	{
		my_class = CClass::GetClass(nClassIndex);
	}

	short race;
	short level;
	short trust;
	int played;
	int lines;							// for the pager
	time_t logon;
	short timer;
	short wait;
	short regen_rate;					// For imbue regeneration spell
	int hit;
	short max_hit;
	short mana;
	short max_mana;
	short move;
	short max_move;
	long gold;
	long gold_bank;
	int exp;
	long act[MAX_BITVECTOR];
	long comm[MAX_BITVECTOR];			// RT added to pad the vector
	long wiznet[MAX_BITVECTOR];			// wiz stuff
	long imm_flags[MAX_BITVECTOR];
	long res_flags[MAX_BITVECTOR];
	long vuln_flags[MAX_BITVECTOR];
	short invis_level;
	short incog_level;
	long affected_by[MAX_BITVECTOR];
	short position;
	short practice;
	short train;
	short carry_weight;
	short carry_number;
	short saving_throw;				//svs
	short alignment;
	short hitroll;
	short damroll;
	short armor[4];
	short wimpy;
	char *speechbuf[10];				// Buffer for various mob speech, progs, etc...

	//
	// stats
	//

	short perm_stat[MAX_STATS];
	short mod_stat[MAX_STATS];

	//
	// parts stuff
	//

	long form[MAX_BITVECTOR];
	long parts[MAX_BITVECTOR];
	short size;
	char *material;

	//
	// mobile stuff
	//

	ROOM_INDEX_DATA *home_room;
	int mobstyle;
	long off_flags[MAX_BITVECTOR];
	short damage[3];
	short dam_type;
	short start_pos;
	short pause;
	short ghost;
	int status;
	long progtypes[MAX_BITVECTOR];
	int hometown;
	float dam_mod;
	short defense_mod;
	bool moved;

	//
	// skill/spell-specific stuff
	//

	short arms;
	short legs;
	short balance;
	short batter;
	CHAR_DATA *analyzePC;
	int analyze;
	short pulseTimer;					// counter for racial combat pulse
	char *backup_true_name;				// Dev's SUPAR CLEVAR CORRUPTION FIX!!!
	float talismanic;
	bool disrupted;						// Has queue-using spell been disrupted?
	int bounty_timer;
	bool law_pass;
	bool stolen_from;
	CProficiencies *Profs() { return pcdata ? &pcdata->profs : &prof_none; }
private:
	CClass *	my_class;
};

#endif /* ENTITY_CHAR_DATA_H */
