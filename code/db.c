/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik Stærfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

/***************************************************************************
 *  ROM 2.4 is copyright 1993-1996 Russ Taylor                             *
 *  ROM has been brought to you by the ROM consortium                      *
 *      Russ Taylor (rtaylor@pacinfo.com)                                  *
 *      Gabrielle Taylor (gtaylor@pacinfo.com)                             *
 *      Brian Moore (rom@rom.efn.org)                                      *
 *  By using this code, you have agreed to follow the terms of the         *
 *  ROM license, in the file Tartarus/doc/rom.license                      *
 ***************************************************************************/

/***************************************************************************
 *       Tartarus code is copyright (C) 1997-1998 by Daniel Graham         *
 *  In using this code you agree to comply with the Tartarus license       *
 *       found in the file /Tartarus/doc/tartarus.doc                      *
 ***************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <climits>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>
#include <fstream>
#include <iterator>
#include <algorithm>
#include "merc.h"
#include "db.h"
#include "db2.h"
#include "entity/handles.h"
#include "rift.h"
#include "recycle.h"
#include "lookup.h"
#include "tables.h"
#include "update.h"
#include "interp.h"
#include "olc.h"
#include "spec.h"
#include "act_comm.h"
#include "pstring.h"
#include "comm.h"
#include "act_wiz.h"
#include "devextra.h"
#include "magic.h"
#include "handler.h"
#include "misc.h"
#include "dioextra.h"
#include "./repositories/noterepository.h"
#include "./repositories/playerrepository.h"
#include "chardef.h"
#include "const.h"
#include "utility.h"
#include "./stdlibs/cdirectory.h"
#include "./include/spdlog/fmt/bundled/format.h"
#include "./include/spdlog/fmt/bundled/printf.h"

/* LOAD CABAL ITEMS */
char *top_bounty_name[MAX_TOP_BOUNTY];
long top_bounty_value[MAX_TOP_BOUNTY];
/*
 * Globals.
 */
HELP_DATA *help_first;
HELP_DATA *help_last;

SHOP_DATA *shop_first;
SHOP_DATA *shop_last;

char bug_buf[2 * MAX_INPUT_LENGTH];
CharacterList char_list;
// The same entities as char_list/object_list, indexed so a cross-reference can
// be checked for liveness instead of trusted. See entity/handles.h.
SlotMap<CHAR_DATA> charHandles;
SlotMap<OBJ_DATA> objectHandles;
SlotMap<DESCRIPTOR_DATA> descriptorHandles;
char *help_greeting;
char log_buf[2 * MAX_INPUT_LENGTH];
KILL_DATA kill_table[MAX_LEVEL];
NOTE_DATA *note_list;
ObjectList object_list;
TIME_INFO_DATA time_info;
short sun;
short moon_berus;
short berus_pos;
short moon_calabren;
short calabren_pos;
ROOM_INDEX_DATA *room_list = nullptr;
ROOM_INDEX_DATA *top_affected_room;
std::list<std::unique_ptr<RUNE_DATA>> rune_list;
MOB_INDEX_DATA *mindex_list = nullptr;
OBJ_INDEX_DATA *oIndex_list = nullptr;
long gold_constant = 0;
long total_gold = 0;
long player_gold = 0;
long total_wealth = 0;
RACE_DATA *race_list;

#define GSN(name, state) short name;
#include "gsn_list.h"
#undef GSN

//
// The same list again, as data this time, so that boot can check the names
// against what the skill table actually assigned.  LoadGsn walks the table and
// writes each row's index through its pgsn pointer, which means it can only see
// the names the table happens to mention: a name no row wires is invisible to
// it and keeps the zero it was born with.  Zero is skill_table[0], "reserved",
// so nothing downstream can tell the difference either.
//
// The state column is the part no scan could supply.  Whether a name IS wired
// is observable; whether it SHOULD be is a decision, and it lives with the name
// in gsn_list.h.
//
namespace
{

enum class GsnState
{
	Wired,
	Pending,
	Tag
};

struct GsnEntry
{
	const char *name;
	short *value;
	GsnState state;
};

#define GSN(name, state) { #name, &name, GsnState::state },
const GsnEntry gsn_registry[] =
{
#include "gsn_list.h"
};
#undef GSN

} // namespace

//
// Reports names that were supposed to be assigned and were not.  Called once,
// after LoadGsn.
//
// There is no separate "unset" sentinel to write first: skill_table[0] is the
// reserved row and carries no pgsn, so no row can ever assign zero, and a gsn
// still holding zero here has therefore never been assigned.  A sentinel was
// the original design and would have been a bug -- these values are used to
// index skill_table and are stored in AFFECT_DATA::type, so an out-of-range
// marker would have been read back as an array subscript.
//
void check_gsn_registry()
{
	int unassigned = 0;

	for (const GsnEntry &entry : gsn_registry)
	{
		if (entry.state == GsnState::Wired && *entry.value == 0)
		{
			RS.Logger.Warn("{} is declared Wired but no skill_table row assigned it, so it reads as skill 0 (\"reserved\").", entry.name);
			unassigned++;
		}
	}

	if (unassigned > 0)
		RS.Logger.Warn("{} skill number(s) are read but never assigned. Give each one a skill_table row, or mark it Pending in gsn_list.h.", unassigned);

	// A Tag names an affect and has no skill of its own, so it needs a value
	// nothing else uses.  Sharing one means is_affected() cannot tell the two
	// apart, and zero is the worst value to share: every affect that never sets
	// a type has it.
	for (const GsnEntry &entry : gsn_registry)
	{
		if (entry.state != GsnState::Tag)
			continue;

		if (*entry.value == 0)
			RS.Logger.Warn("{} is an affect tag sharing type 0 with every untyped affect.", entry.name);
	}
}

/* GSNS */
short cabal_members[MAX_CABAL];
short cabal_max[MAX_CABAL];
/*
 * Locals.
 */
MOB_INDEX_DATA *mob_index_hash[MAX_KEY_HASH];
OBJ_INDEX_DATA *obj_index_hash[MAX_KEY_HASH];
ROOM_INDEX_DATA *room_index_hash[MAX_KEY_HASH];
char *string_hash[MAX_KEY_HASH];

AREA_DATA *area_last;

char *string_space;
char *top_string;
char str_empty[1];

short count_data[30000];

int top_affect;
int top_area;
int top_ed;
int top_exit;
int top_help;
int top_mob_index;
int top_obj_index;
int top_reset;
int top_room;
int top_shop;
int top_vnum_room; /* OLC */
int top_vnum_mob;  /* OLC */
int top_vnum_obj;  /* OLC */
int mobile_count = 0;
int newmobs = 0;
int newobjs = 0;

/*
 * Semi-locals.
 */
bool fBootDb;
FILE *fpArea;
char strArea[MAX_INPUT_LENGTH];
int race_occurance[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int cabal_occurance[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int class_occurance[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int sex_occurance[] = {0, 0, 0};
int num_pfiles = 0;
int average_played = 0;
int tempalign = 0;
int align_occurance[] = {0, 0, 0};
int disregard = 0;
int char_hours = 0;
int char_max_hp = 0;
int most_hours = 0;
char most_hours_player[MAX_STRING_LENGTH];
char *plr_act_flags;
int totalhrsplayed = 0;
int temp_level = 0;
float temp_percentage = 0;

/*
 * Memory management.
 * Increase MAX_STRING if you have too.
 * Tune the others only if you understand what you're doing.
 */
/*#define     	MAX_STRING      3593120
#define			MAX_PERM_BLOCK	131072
#define			MAX_MEM_LIST	12

void *			rgFreeList	[MAX_MEM_LIST];
const int		rgSizeList	[MAX_MEM_LIST]	=
{
	16, 32, 64, 128, 256, 1024, 2048, 4096, 8192, 16384, 32768-64
};
*/
int nAllocString;
int sAllocString;
int nAllocPerm;
int sAllocPerm;

char *munch(char *str)
{
	static char buf[MSL];
	int i;
	char *copy;

	if (!str)
		return "";

	copy = str;

	for (i = 0; *copy != '\0'; i++)
	{
		if (*copy == '\r')
		{
			copy++;
			i--;
			continue;
		}

		buf[i] = *copy;
		copy++;
	}

	buf[i] = '\0';
	return buf;
}

void update_db_gold()
{
	auto players = PlayerRepository(RS.Db);

	// Changed from > 15 to > 30 to tighten the gold up.
	num_pfiles = players.CountGoldAboveLevel(30);
	player_gold = players.SumGold();

	total_gold = gold_constant * num_pfiles;
}

/*
 * Snarf an 'area' header line.
 */
void load_area(FILE *fp)
{
	AREA_DATA *pArea;

	pArea = new AREA_DATA;
	/*
	pArea->reset_first	= nullptr;
	pArea->reset_last	= nullptr;
	*/
	pArea->file_name = fread_string(fp);

	pArea->prev = nullptr;

	zero_vector(pArea->area_flags);
	SET_BIT(pArea->area_flags, AREA_LOADING); /* OLC */
	pArea->vnum = top_area;					  /* OLC */
	pArea->affected.clear();
	pArea->name = fread_string(fp);
	pArea->credits = fread_string(fp);
	pArea->low_range = fread_number(fp);
	pArea->high_range = fread_number(fp);
	pArea->min_vnum = fread_number(fp);
	pArea->max_vnum = fread_number(fp);
	pArea->area_type = flag_lookup(fread_word(fp), area_type_table);
	pArea->climate = climate_lookup(fread_word(fp));
	fread_flag_new(pArea->area_flags, fp);

	pArea->security = fread_number(fp);
	pArea->builders = fread_string(fp);
	pArea->age = 15;
	pArea->nplayer = 0;
	pArea->empty= false;
	pArea->nplayer = 0;
	zero_vector(pArea->affected_by);

	/* Morg - Valgrind fix. */
	zero_vector(pArea->progtypes);
	pArea->aprogs = nullptr;
	pArea->temp = 0;
	pArea->sky = 0;

	if (!area_first)
		area_first = pArea;

	if (area_last)
	{
		area_last->next = pArea;
		pArea->prev = area_last;

		REMOVE_BIT(area_last->area_flags, AREA_LOADING); /* OLC */
	}

	area_last = pArea;
	pArea->next = nullptr;

	top_area++;
}

void sort_areas(void)
{
	AREA_DATA *placeholder;
	AREA_DATA *pArea;
	bool sorted= false;
	bool first= false;
	int vnum = 0;

	while (!sorted)
	{
		sorted = true;

		for (pArea = area_first; pArea; pArea = pArea->next)
		{
			first= false;

			if (!pArea->next)
				break;

			if (pArea == area_first)
				first = true;

			if (pArea->min_vnum > pArea->next->min_vnum)
			{
				sorted= false;

				if (pArea->next->next)
					pArea->next->next->prev = pArea;

				pArea->next->prev = pArea->prev;

				if (!first)
					pArea->prev->next = pArea->next;

				pArea->prev = pArea->next;
				placeholder = pArea->next->next;
				pArea->next->next = pArea;
				pArea->next = placeholder;

				if (first)
					area_first = pArea->prev;
			}
		}
	}

	for (pArea = area_first; pArea; pArea = pArea->next)
	{
		pArea->vnum = vnum;
		vnum++;
	}
}

/* OLC
 * Snarf an 'area' header line.   Check this format.  MUCH better.  Add fields
 * too.
 *
 * #AREAFILE
 * Name   { All } Locke    Newbie School~
 * Repop  A teacher pops in the room and says, 'Repop coming!'~
 * Recall 3001
 * End
 */
void new_load_area(FILE *fp)
{
	AREA_DATA *pArea;
	char *word;
	bool fMatch;

	pArea = new AREA_DATA;
	pArea->age = 15;
	pArea->nplayer = 0;
	pArea->file_name = palloc_string(strArea);
	pArea->vnum = top_area;
	pArea->name = palloc_string("New Area");
	pArea->builders = palloc_string("");
	pArea->security = 1;
	pArea->min_vnum = 0;
	pArea->max_vnum = 0;
	pArea->nplayer = 0;
	zero_vector(pArea->area_flags);
	/*  pArea->recall       = ROOM_VNUM_TEMPLE;        ROM OLC */

	for (;;)
	{
		word = feof(fp) ? (char *)"End" : fread_word(fp);
		fMatch= false;

		switch (UPPER(word[0]))
		{
			case 'N':
				SKEY("Name", pArea->name)
				break;
			case 'S':
				KEY("Security", pArea->security, fread_number(fp))
				break;
			case 'V':
				if (!str_cmp(word, "VNUMs"))
				{
					pArea->min_vnum = fread_number(fp);
					pArea->max_vnum = fread_number(fp);
				}
				break;
			case 'E':
				if (!str_cmp(word, "End"))
				{
					fMatch = true;
					if (area_first == nullptr)
						area_first = pArea;
					if (area_last != nullptr)
						area_last->next = pArea;
					area_last = pArea;
					pArea->next = nullptr;
					top_area++;
					return;
				}
				break;
			case 'B':
				SKEY("Builders", pArea->builders)
				break;
			case 'C':
				SKEY("Credits", pArea->credits)
				break;
		}
	}
}

/*
 * Sets vnum range for area using OLC protection features.
 */
void assign_area_vnum(int vnum)
{
	if (area_last->min_vnum == 0 || area_last->max_vnum == 0)
		area_last->min_vnum = area_last->max_vnum = vnum;

	if (vnum != URANGE(area_last->min_vnum, vnum, area_last->max_vnum))
	{
		if (vnum < area_last->min_vnum)
			area_last->min_vnum = vnum;
		else
			area_last->max_vnum = vnum;
	}
}

/*
 * Adds a reset to a room.  OLC
 * Similar to add_reset in olc.c
 */
void new_reset(ROOM_INDEX_DATA *pR, RESET_DATA *pReset)
{
	RESET_DATA *pr;

	if (!pR)
		return;

	pr = pR->reset_last;

	if (!pr)
	{
		pR->reset_first = pReset;
		pR->reset_last = pReset;
	}
	else
	{
		pR->reset_last->next = pReset;
		pR->reset_last = pReset;
		pR->reset_last->next = nullptr;
	}

	top_reset++;
	return;
}

/*
 * Snarf a reset section.
 */
void load_resets(FILE *fp)
{
	RESET_DATA *pReset;
	int iLastRoom = 0;
	int iLastObj = 0;

	if (!area_last)
	{
		RS.Logger.Error("Load_resets: no #AREA seen yet.");
		exit(1);
	}

	for (;;)
	{
		ROOM_INDEX_DATA *pRoomIndex;
		EXIT_DATA *pexit = nullptr;
		char letter;
		/*
		OBJ_INDEX_DATA *temp_index;
		int temp;
		*/

		letter = fread_letter(fp);

		if (letter == 'S')
			break;

		if (letter == '*')
		{
			fread_to_eol(fp);
			continue;
		}

		pReset = new RESET_DATA;
		pReset->command = letter;

		/* if_flag */ fread_number(fp);

		pReset->arg1 = fread_number(fp);
		pReset->arg2 = fread_number(fp);
		pReset->arg3 = (letter == 'G' || letter == 'R') ? 0 : fread_number(fp);
		pReset->arg4 = (letter == 'P' || letter == 'M') ? fread_number(fp) : 0;

		/*
		pReset->arg4 = (letter == 'P' || letter == 'M')
			? ((temp == 0) ? 1 : fread_number(fp))
			: 0;
		*/

		fread_to_eol(fp);

		/*
		 * Validate parameters.
		 * We're calling the index functions for the side effect.
		 */
		switch (letter)
		{
			case 'M':
				get_mob_index(pReset->arg1);

				/*
				get_room_index(pReset->arg3);
				*/

				pRoomIndex = get_room_index(pReset->arg3);

				if (pRoomIndex)
				{
					new_reset(pRoomIndex, pReset);
					iLastRoom = pReset->arg3;
				}

				break;
			case 'O':
				/*
				temp_index = get_obj_index(pReset->arg1);
				temp_index->reset_num++;
				get_room_index(pReset->arg3);
				*/

				get_obj_index(pReset->arg1);

				pRoomIndex = get_room_index(pReset->arg3);

				if (pRoomIndex)
				{
					new_reset(pRoomIndex, pReset);
					iLastObj = pReset->arg3;
				}

				break;
			case 'P':
				/*
				temp_index = get_obj_index(pReset->arg1);
				temp_index->reset_num++;
				get_obj_index(pReset->arg3);
				*/

				get_obj_index(pReset->arg1);
				
				pRoomIndex = get_room_index(iLastObj);

				if (pRoomIndex)
					new_reset(pRoomIndex, pReset);

				break;
			case 'G':
			case 'E':
				/*
				temp_index = get_obj_index(pReset->arg1);
				temp_index->reset_num++;
				*/

				get_obj_index(pReset->arg1);

				pRoomIndex = get_room_index(iLastRoom);

				if (pRoomIndex)
				{
					new_reset(pRoomIndex, pReset);
					iLastObj = iLastRoom;
				}

				break;
			case 'D':
				pRoomIndex = get_room_index(pReset->arg1);

				if (pReset->arg2 < 0
					|| pReset->arg2 > 5
					|| !pRoomIndex
					|| !(pexit = pRoomIndex->exit[pReset->arg2])
					|| !IS_SET(pexit->exit_info, EX_ISDOOR))
				{
					RS.Logger.Error("Load_resets: 'D': exit {} not door.", pReset->arg2);
					exit(1);
				}

				/*
				if (pReset->arg3 < 0 || pReset->arg3 > 2)
				{
					RS.Logger.Error("Load_resets: 'D': bad 'locks': %d.", pReset->arg3);
					exit(1);
				}
				*/

				switch (pReset->arg3)
				{
					case 0:
						break;
					case 1:
						SET_BIT(pexit->exit_info, EX_CLOSED);
						break;
					case 2:
						SET_BIT(pexit->exit_info, EX_CLOSED);
						SET_BIT(pexit->exit_info, EX_LOCKED);
						break;
					default:
						RS.Logger.Warn("Load_resets: 'D': bad 'locks': {}.", pReset->arg3);
						break;
				}

				break;
			case 'R':
				pRoomIndex = get_room_index(pReset->arg1);

				if (pReset->arg2 < 0 || pReset->arg2 > 6)
				{
					RS.Logger.Error("Load_resets: 'R': bad exit {}.", pReset->arg2);
					exit(1);
				}

				if (pRoomIndex)
					new_reset(pRoomIndex, pReset);

				break;
			default:
				RS.Logger.Error("Load_resets: bad command '{}'.", letter);
				exit(1);
				break;
		}

		/*
		if (area_last->reset_first == nullptr)
			area_last->reset_first = pReset;
	
		if (area_last->reset_last != nullptr)
			area_last->reset_last->next	= pReset;

		area_last->reset_last = pReset;
		*/
		pReset->next = nullptr;
		top_reset++;
	}
}

/*
 * Snarf a shop section.
 */
void load_shops(FILE *fp)
{
	/*
	SHOP_DATA *pShop;

	for (;;)
	{
		MOB_INDEX_DATA *pMobIndex;
		int iTrade;

		pShop = palloc_struct(sizeof(*pShop));
		pShop->keeper = fread_number(fp);
		if(pShop->keeper == 0)
			break;

		for(iTrade = 0; iTrade < MAX_TRADE; iTrade++)
		{
			pShop->buy_type[iTrade]	= fread_number(fp);
		}

		pShop->profit_buy = fread_number(fp);
		pShop->profit_sell = fread_number(fp);
		pShop->open_hour = fread_number(fp);
		pShop->close_hour = fread_number(fp);
		fread_to_eol(fp);

		pMobIndex = get_mob_index(pShop->keeper);
		pMobIndex->pShop = pShop;

		if (shop_first == nullptr)
			shop_first = pShop;
		if (shop_last != nullptr)
			shop_last->next = pShop;

		shop_last = pShop;
		pShop->next	= nullptr;
		top_shop++;
	}
	*/
}

void load_cabal_items(void)
{
	FILE *fp;
	OBJ_DATA *obj;

	int inum, vnum;

	if (mPort != 9999)
		return;

	fp = fopen(CABAL_ITEMS_FILE, "r");

	if (!fp)
		return;

	while ((inum = fread_number(fp)) != -1)
	{
		vnum = fread_number(fp);
		obj = create_object(get_obj_index(cabal_table[inum].item_vnum), 0);

		if (!vnum)
		{
			for (OwningListWalk<CHAR_DATA> walk(char_list); !walk.Done(); walk.Step())
			{
				CHAR_DATA *mob = walk.Current();

				if (mob->cabal == inum && is_cabal_guard(mob))
					obj_to_char(obj, mob);
			}

			continue;
		}

		for (OwningListWalk<CHAR_DATA> walk(char_list); !walk.Done(); walk.Step())
		{
			CHAR_DATA *mob = walk.Current();

			if (mob->pIndexData->vnum == vnum)
				obj_to_char(obj, mob);
		}
	}

	fclose(fp);
}

/*
 * Translate all room exits from virtual to real.
 * Has to be done after all rooms are read in.
 * Check for bad reverse exits.
 */
void fix_exits(void)
{
	ROOM_INDEX_DATA *pRoomIndex;
	EXIT_DATA *pexit;
	int door;

	for (auto iHash = 0; iHash < MAX_KEY_HASH; iHash++)
	{
		for (pRoomIndex = room_index_hash[iHash]; pRoomIndex != nullptr; pRoomIndex = pRoomIndex->next)
		{
			bool fexit = false;

			const int pRoomIndex_exit_size = std::size(pRoomIndex->exit);
			for (door = 0; door < pRoomIndex_exit_size; door++)
			{
				pexit = pRoomIndex->exit[door];

				if (pexit != nullptr)
				{
					if (pexit->u1.vnum <= 0 || get_room_index(pexit->u1.vnum) == nullptr)
					{
						pexit->u1.to_room = nullptr;
					}
					else
					{
						fexit = true;
						pexit->u1.to_room = get_room_index(pexit->u1.vnum);

						if (IS_SET(pexit->exit_info, EX_CLOSED) && !IS_SET(pexit->exit_info, EX_ISDOOR))
							SET_BIT(pexit->exit_info, EX_ISDOOR);
					}
				}
			}

			if (!fexit)
				SET_BIT(pRoomIndex->room_flags, ROOM_NO_MOB);
		}
	}
	/*
	for (iHash = 0; iHash < MAX_KEY_HASH; iHash++)
	{
		for (pRoomIndex  = room_index_hash[iHash]; pRoomIndex != nullptr; pRoomIndex  = pRoomIndex->next)
		{
			for (door = 0; door <= 5; door++)
			{
				if ((pexit = pRoomIndex->exit[door]) != nullptr
					&& (to_room = pexit->u1.to_room) != nullptr
					&& (pexit_rev = to_room->exit[rev_dir[door]]) != nullptr
					&& pexit_rev->u1.to_room != pRoomIndex
					&& (pRoomIndex->vnum < 1200 || pRoomIndex->vnum > 1299))
				{
					RS.Logger.Warn("Fix_exits: {}:{} -> {}:{} -> {}.",
						pRoomIndex->vnum,
						door,
						to_room->vnum,
						rev_dir[door],
						pexit_rev->u1.to_room == nullptr ? 0 : pexit_rev->u1.to_room->vnum);
				}
			}
		}
	}
	*/
}

void find_adjacents(void)
{
	int count, i;
	ROOM_INDEX_DATA *room, *to_room;
	bool in;

	for (auto area = area_first; area; area = area->next)
	{
		count = 0;

		for (i = 0; i < MAX_ADJACENT; i++)
		{
			area->adjacent[i] = nullptr;
		}

		for (room = room_list; room != nullptr; room = room->next_room)
		{
			if (room->area != area)
				continue;

			for (auto pexit : room->exit)
			{
				if (pexit == nullptr)
					continue;

				to_room = pexit->u1.to_room;
				if (to_room != nullptr && to_room->area != area)
				{
					in = false;

					for (i = 0; i < MAX_ADJACENT; i++)
					{
						if (area->adjacent[i] == to_room->area)
							in = true;
					}

					if (in == false)
					{
						area->adjacent[count] = to_room->area;
						count++;
					}
				}
			}
		}
	}
}

/*
 * Repopulate areas periodically.
 */
void area_update(void)
{
	AREA_DATA *pArea;
	char buf[MAX_STRING_LENGTH];

	for (pArea = area_first; pArea != nullptr; pArea = pArea->next)
	{
		if (IS_SET(pArea->progtypes, APROG_TICK))
			(pArea->aprogs->tick_prog)(pArea);

		for (auto it = pArea->affected.begin(); it != pArea->affected.end(); )
		{
			auto next = std::next(it);

			if (it->duration > 0)
			{
				if (it->tick_fun)
					(*it->tick_fun)(pArea, &*it);

				it->duration--;
			}
			else if (it->duration < 0);
			else
			{
				affect_remove_area(pArea, &*it);
			}

			it = next;
		}

		if (pArea->nplayer == 0)
			pArea->empty = true;
		else
			pArea->empty= false;

		if (++pArea->age < 6)
			continue;

		/*
		 * Check age and reset.
		 */

		if ((pArea->empty || pArea->age >= 30))
		{
			reset_area(pArea);

			if (IS_SET(pArea->progtypes, APROG_RESET))
				(pArea->aprogs->reset_prog)(pArea);

			sprintf(buf, "%s has just been reset.", pArea->name);
			wiznet(buf, nullptr, nullptr, WIZ_RESETS, 0, 0);
			pArea->age = number_range(0, 2);
		}
	}
}

/* OLC
 * Reset one room.  Called by reset_area and olc.
 */
// void reset_area( AREA_DATA *pArea )
void reset_room(ROOM_INDEX_DATA *pRoom)
{
	RESET_DATA *pReset;
	CHAR_DATA *pMob;
	CHAR_DATA *mob;
	OBJ_DATA *pObj, *pObj2, *pObj2_next;
	CHAR_DATA *LastMob = nullptr;
	OBJ_DATA *LastObj = nullptr;
	EXIT_DATA *pexit = nullptr;
	bool last;
	int level = 0;
	CHAR_DATA *rch;
	bool found;

	if (!pRoom)
		return;

	pMob = nullptr;
	last = true;

	if (pRoom->trap)
		pRoom->trap->armed = true;

	for (pReset = pRoom->reset_first; pReset != nullptr; pReset = pReset->next)
	{
		ROOM_INDEX_DATA *pRoomIndex = nullptr;
		MOB_INDEX_DATA *pMobIndex = nullptr;
		OBJ_INDEX_DATA *pObjIndex = nullptr;
		OBJ_INDEX_DATA *pObjToIndex = nullptr;
		OBJ_DATA *secondary = nullptr;
		int count, limit;

		switch (pReset->command)
		{
			case 'M':
				pMobIndex = get_mob_index(pReset->arg1);

				if (pMobIndex == nullptr)
				{
					RS.Logger.Warn("Reset_area: 'M': bad vnum {}.", pReset->arg1);
					continue;
				}

				pRoomIndex = get_room_index(pReset->arg3);

				if (pRoomIndex == nullptr)
				{
					RS.Logger.Warn("Reset_area: 'R': bad vnum {}.", pReset->arg3);
					continue;
				}

				if (pMobIndex->count >= pReset->arg2)
				{
					last= false;
					break;
				}

				count = 0;
				for (mob = pRoomIndex->people; mob != nullptr; mob = mob->next_in_room)
				{
					if (mob->pIndexData == pMobIndex)
					{
						count++;
						if (count >= pReset->arg4)
						{
							last= false;
							break;
						}
					}
				}

				if (count >= pReset->arg4)
					break;

				pMob = create_mobile(pMobIndex);

				/*
				* Some more hard coding.
				*/
				if (room_is_dark(pRoom))
					SET_BIT(pMob->affected_by, AFF_INFRARED);

				/*
				* Pet shop mobiles get ACT_PET set.
				*/
				{
					ROOM_INDEX_DATA *pRoomIndexPrev;

					pRoomIndexPrev = get_room_index(pRoom->vnum - 1);
					if (pRoomIndexPrev && IS_SET(pRoomIndexPrev->room_flags, ROOM_PET_SHOP))
						SET_BIT(pMob->act, ACT_PET);
				}

				pMob->zone = pRoomIndex->area;

				char_to_room(pMob, pRoom);

				LastMob = pMob;
				level = URANGE(0, pMob->level - 2, LEVEL_HERO - 1); /* -1 ROM */
				last = true;
				break;
			case 'O':
				pObjIndex = get_obj_index(pReset->arg1);

				if (!pObjIndex)
				{
					RS.Logger.Warn("Reset_room: 'O' 1 : bad vnum {}", pReset->arg1);
					RS.Logger.Warn("{} {} {} {}", pReset->arg1, pReset->arg2, pReset->arg3, pReset->arg4);
					continue;
				}

				pRoomIndex = get_room_index(pReset->arg3);

				if (!pRoomIndex)
				{
					RS.Logger.Warn("Reset_room: 'O' 2 : bad vnum {}.", pReset->arg3);
					RS.Logger.Warn("{} {} {} {}", pReset->arg1, pReset->arg2, pReset->arg3, pReset->arg4);
					continue;
				}

				// The successor has to be read before the body: extract_obj frees
				// pObj2, and the loop advances through it.
				for (pObj2 = pRoomIndex->contents; pObj2; pObj2 = pObj2_next)
				{
					pObj2_next = pObj2->next_content;

					if (pObj2->pIndexData->vnum == pObjIndex->vnum
						&& !is_obj_stat(pObj2, ITEM_DONATION_PIT)
						&& !is_obj_stat(pObj2, ITEM_CORPSE_PC)
						&& !is_obj_stat(pObj2, ITEM_CORPSE_NPC)
						&& pObj2->item_type == ITEM_CONTAINER
						&& !pObj2->contains)
					{
						extract_obj(pObj2);
					}
				}

				if ((pObjIndex->limcount >= pObjIndex->limtotal && pObjIndex->limtotal > 0)
					|| count_obj_list(pObjIndex, pRoomIndex->contents) > 0)
				{
					last= false;
					break;
				}

				pObj = create_object(pObjIndex, std::min(number_fuzzy(level), LEVEL_HERO - 1));
				pObj->cost = 0;
				pObj->owner = palloc_string("none");
				pObj->talked = palloc_string("someone a long long time ago");

				obj_to_room(pObj, pRoom);

				last = true;
				break;
			case 'P':
				pObjIndex = get_obj_index(pReset->arg1);

				if (!pObjIndex)
				{
					RS.Logger.Warn("Reset_room: 'P': bad vnum {}.", pReset->arg1);
					continue;
				}

				pObjToIndex = get_obj_index(pReset->arg3);

				if (!pObjToIndex)
				{
					RS.Logger.Warn("Reset_room: 'P': bad vnum {}.", pReset->arg3);
					continue;
				}

				if (pReset->arg2 > 50)
					limit = 6;
				else if (pReset->arg2 == -1)
					limit = 999;
				else
					limit = pReset->arg2;

				if (pRoom->area->nplayer > 1
					|| (LastObj = get_obj_type(pObjToIndex)) == nullptr
					|| (LastObj->in_room == nullptr && !last)
					|| (pObjIndex->limtotal > 0 && pObjIndex->limcount >= pObjIndex->limtotal)
					|| (pObjIndex->count >= limit /* && number_range(0,4) != 0 */)
					|| (count = count_obj_list(pObjIndex, LastObj->contains)) > pReset->arg4)
				{
					last= false;
					break;
				}

				while (count < pReset->arg4)
				{
					pObj = create_object(pObjIndex, number_fuzzy(LastObj->level));
					pObj->owner = palloc_string("none");
					obj_to_obj(pObj, LastObj);

					count++;

					if (pObjIndex->limcount >= pObjIndex->limtotal && pObjIndex->limtotal > 0)
						break;
				}

				LastObj->value[1] = LastObj->pIndexData->value[1];
				last = true;
				break;
			case 'G':
			case 'E':
				pObjIndex = get_obj_index(pReset->arg1);

				if (!pObjIndex)
				{
					RS.Logger.Warn("Reset_room: 'E' or 'G': bad vnum {}.", pReset->arg1);
					continue;
				}

				if (!last)
					break;

				if (!LastMob)
				{
					RS.Logger.Warn("Reset_room: 'E' or 'G': null mob for vnum {}.", pReset->arg1);
					last= false;
					break;
				}

				if (LastMob->pIndexData->pShop)
				{
					int olevel = 0, i, j;

					if (!pObjIndex->new_format)
					{
						switch (pObjIndex->item_type)
						{
							case ITEM_PILL:
							case ITEM_POTION:
							case ITEM_SCROLL:
								olevel = 53;
								for (i = 1; i < 5; i++)
								{
									if (pObjIndex->value[i] > 0)
									{
										for (j = 0; j < MAX_CLASS; j++)
										{
											olevel = std::min(olevel, (int)skill_table[pObjIndex->value[i]].skill_level[j]);
										}
									}
								}
								olevel = std::max(0, (olevel * 3 / 4) - 2);
								break;
							case ITEM_WAND:
								olevel = number_range(10, 20);
								break;
							case ITEM_STAFF:
								olevel = number_range(15, 25);
								break;
							case ITEM_ARMOR:
								olevel = number_range(5, 15);
								break;
							case ITEM_WEAPON:
								olevel = number_range(5, 15);
								break;
							case ITEM_TREASURE:
								olevel = number_range(10, 20);
								break;
							default:
								olevel = 0;
								break;
						}
					}

					pObj = create_object(pObjIndex, olevel);
					pObj->owner = palloc_string("none");

					if (pObj->pIndexData->limtotal == 0)
						SET_BIT(pObj->extra_flags, ITEM_INVENTORY);
				}
				else /* ROM OLC else version */
				{
					int limit;

					if (pReset->arg2 > 50) /* old format */
						limit = 6;
					else if (pReset->arg2 == -1 || pReset->arg2 == 0) /* no limit */
						limit = 999;
					else
						limit = pReset->arg2;

					if (pObjIndex->limcount >= pObjIndex->limtotal && pObjIndex->limtotal > 0)
					{
						break;
					}
					else
					{
						pObj = create_object(pObjIndex, std::min(number_fuzzy(level), LEVEL_HERO - 1));
					}

					pObj->owner = palloc_string("none");
				}

				obj_to_char(pObj, LastMob);

				if (pReset->command == 'E')
				{
					if (pReset->arg3 == 16)
					{
						secondary = get_eq_char(LastMob, WEAR_WIELD);

						if (secondary != nullptr)
						{
							unequip_char(LastMob, secondary, true);
							equip_char(LastMob, secondary, 18, true);
							equip_char(LastMob, pObj, 16, true);
						}
						else
						{
							equip_char(LastMob, pObj, pReset->arg3, true);
						}
					}
					else if (pReset->arg3 == 18)
					{
						if (get_eq_char(LastMob, WEAR_WIELD) == nullptr)
							equip_char(LastMob, pObj, 16, true);
						else
							equip_char(LastMob, pObj, pReset->arg3, true);
					}
					else
					{
						equip_char(LastMob, pObj, pReset->arg3, true);
					}
				}
				last = true;
				break;

			case 'F':
				found= false;

				if (!LastMob)
					continue;

				for (rch = LastMob->in_room->people; rch != nullptr; rch = rch->next_in_room)
				{
					if (is_npc(rch) && (pReset->arg2 == rch->pIndexData->vnum))
					{
						found = true;
						break;
					}
				}

				if (!last || !found)
					break;

				add_follower(LastMob, rch);
				LastMob->leader = rch->self;
				break;
			case 'D':
				pRoomIndex = get_room_index(pReset->arg1);

				if (pRoomIndex == nullptr)
				{
					RS.Logger.Warn("Reset_area: 'D': bad vnum {}.", pReset->arg1);
					continue;
				}

				pexit = pRoomIndex->exit[pReset->arg2];

				if (pexit == nullptr)
					break;

				SET_BIT(pexit->exit_info, EX_ISDOOR);

				switch (pReset->arg3)
				{
					case 0:
						REMOVE_BIT(pexit->exit_info, EX_CLOSED);
						REMOVE_BIT(pexit->exit_info, EX_LOCKED);
						break;
					case 1:
						SET_BIT(pexit->exit_info, EX_CLOSED);
						REMOVE_BIT(pexit->exit_info, EX_LOCKED);
						break;
					case 2:
						SET_BIT(pexit->exit_info, EX_CLOSED);
						SET_BIT(pexit->exit_info, EX_LOCKED);
						break;
				}

				last = true;
				break;
			case 'R':
				pRoomIndex = get_room_index(pReset->arg1);

				if (!pRoomIndex)
				{
					RS.Logger.Warn("Reset_room: 'R': bad vnum {}.", pReset->arg1);
					continue;
				}

				{
					EXIT_DATA *pExit;
					int d0;
					int d1;

					int dmax = pReset->arg2;
					if (dmax > 6)
						dmax = 6;

					for (d0 = 0; d0 < dmax - 1; d0++)
					{
						d1 = number_range(d0, dmax - 1);
						pExit = pRoomIndex->exit[d0];
						pRoomIndex->exit[d0] = pRoomIndex->exit[d1];
						pRoomIndex->exit[d1] = pExit;
					}
				}

				break;
			default:
				RS.Logger.Warn("Reset_area: bad command {}.", pReset->command);
				break;
		}
	}
}

/* OLC
 * Reset one area.
 */
void reset_area(AREA_DATA *pArea)
{
	ROOM_INDEX_DATA *pRoom;
	int vnum;

	for (vnum = pArea->min_vnum; vnum <= pArea->max_vnum; vnum++)
	{
		pRoom = get_room_index(vnum);

		if (pRoom)
			reset_room(pRoom);
	}
}

/*
 * Create an instance of a mobile.
 */
CHAR_DATA *create_mobile(MOB_INDEX_DATA *pMobIndex)
{
	int i;
	CHAR_DATA *mob;
	AFFECT_DATA af;
	int style;

	mobile_count++;

	if (pMobIndex == nullptr)
	{
		RS.Logger.Error("Create_mobile: nullptr pMobIndex.");
		exit(1);
	}

	mob = new_char();
	mob->pIndexData = pMobIndex;
	mob->name = palloc_string(pMobIndex->player_name);		  /* OLC */
	mob->short_descr = palloc_string(pMobIndex->short_descr); /* OLC */
	mob->long_descr = palloc_string(pMobIndex->long_descr);	  /* OLC */

	if (pMobIndex->description)
		mob->description = palloc_string(pMobIndex->description); /* OLC */

	mob->id = get_mob_id();
	mob->prompt = nullptr;

	copy_vector(mob->progtypes, pMobIndex->progtypes);
	total_wealth += pMobIndex->wealth;

	/* load in new style */
	if (pMobIndex->new_format)
	{
		/* read from prototype */
		mob->group = pMobIndex->group;

		copy_vector(mob->act, pMobIndex->act);
		copy_vector(mob->affected_by, pMobIndex->affected_by);

		mob->alignment = pMobIndex->alignment;
		mob->level = pMobIndex->level;
		mob->hitroll = pMobIndex->hitroll;
		mob->damroll = pMobIndex->damage[DICE_BONUS];
		mob->max_hit = dice(pMobIndex->hit[DICE_NUMBER], pMobIndex->hit[DICE_TYPE]) + pMobIndex->hit[DICE_BONUS];
		mob->hit = mob->max_hit;
		mob->max_mana = dice(pMobIndex->mana[DICE_NUMBER], pMobIndex->mana[DICE_TYPE]) + pMobIndex->mana[DICE_BONUS];
		mob->mana = mob->max_mana;
		mob->damage[DICE_NUMBER] = pMobIndex->damage[DICE_NUMBER];
		mob->damage[DICE_TYPE] = pMobIndex->damage[DICE_TYPE];
		mob->dam_type = pMobIndex->dam_type;

		if (mob->dam_type == 0)
		{
			switch (number_range(1, 3))
			{
				case 1: /* slash */
					mob->dam_type = 3;
					break;
				case 2: /* pound */
					mob->dam_type = 7;
					break; 
				case 3: /* pierce */
					mob->dam_type = 11;
					break; 
			}
		}

		for (i = 0; i < 4; i++)
		{
			mob->armor[i] = pMobIndex->ac[i];
		}

		copy_vector(mob->off_flags, pMobIndex->off_flags);
		copy_vector(mob->imm_flags, pMobIndex->imm_flags);
		copy_vector(mob->res_flags, pMobIndex->res_flags);
		copy_vector(mob->vuln_flags, pMobIndex->vuln_flags);

		mob->start_pos = pMobIndex->start_pos;
		mob->sex = pMobIndex->sex;

		if (mob->sex == 3) /* random sex */
			mob->sex = number_range(1, 2);

		mob->race = pMobIndex->race;

		copy_vector(mob->form, pMobIndex->form);
		copy_vector(mob->parts, pMobIndex->parts), mob->size = pMobIndex->size;
		copy_vector(mob->progtypes, pMobIndex->progtypes);

		mob->dam_mod = pMobIndex->dam_mod;
		mob->defense_mod = 0;
		mob->arms = pMobIndex->arms;
		mob->legs = pMobIndex->legs;
		mob->regen_rate = 0;
		mob->mobstyle = 0;
		mob->stolen_from= false;

		if (pMobIndex->Class()->GetIndex() == CLASS_WARRIOR)
		{
			for (style = 1; style <= 32; style *= 2)
			{
				if (IS_SET(pMobIndex->styles, style))
				{
					mob->mobstyle = style;
					break;
				}
			}
		}

		if (IS_SET(mob->act, ACT_WARD_MOB))
			mob->invis_level = LEVEL_HERO + 1;

		/* computed on the spot */
		for (i = 0; i < MAX_STATS; i++)
		{
			mob->perm_stat[i] = std::min(25, 11 + mob->level / 4);
		}

		if (mob->pIndexData->Class()->GetIndex() == CLASS_WARRIOR)
		{
			mob->perm_stat[STAT_STR] += 3;
			mob->perm_stat[STAT_INT] -= 1;
			mob->perm_stat[STAT_CON] += 2;
		}

		if (mob->pIndexData->Class()->GetIndex() == CLASS_THIEF)
		{
			mob->perm_stat[STAT_DEX] += 3;
			mob->perm_stat[STAT_INT] += 1;
			mob->perm_stat[STAT_WIS] -= 1;
		}

		if (IS_SET(mob->act, ACT_CLERIC))
		{
			mob->perm_stat[STAT_WIS] += 3;
			mob->perm_stat[STAT_DEX] -= 1;
			mob->perm_stat[STAT_STR] += 1;
		}

		if (IS_SET(mob->act, ACT_MAGE))
		{
			mob->perm_stat[STAT_INT] += 3;
			mob->perm_stat[STAT_STR] -= 1;
			mob->perm_stat[STAT_DEX] += 1;
		}

		if (IS_SET(mob->off_flags, OFF_FAST))
			mob->perm_stat[STAT_DEX] += 2;

		mob->perm_stat[STAT_STR] += mob->size - SIZE_MEDIUM;
		mob->perm_stat[STAT_CON] += (mob->size - SIZE_MEDIUM) / 2;

		/* let's get some spell action */

		init_affect(&af);
		af.where = TO_AFFECTS;
		af.type = skill_lookup("sanctuary");
		af.level = mob->level;
		af.duration = -1;
		af.location = APPLY_NONE;

		if (is_affected_by(mob, AFF_SANCTUARY))
		{
			SET_BIT(af.bitvector, AFF_SANCTUARY);
			affect_to_char(mob, &af);
		}

		for (i = 0; i < 10; i++)
		{
			if (pMobIndex->affect_sn[i] < 0)
				break;

			if (pMobIndex->affect_sn[i] > 0)
			{
				af.modifier = 0;
				af.location = APPLY_NONE;
				af.type = pMobIndex->affect_sn[i];
				// affect_bit is NO_FLAG (-99) when the mob's affect flag name was not found
				// in affect_flags. SET_BIT would then index bitvector[(int)-99/32] = [-3].
				if (pMobIndex->affect_bit[i] >= 0)
					SET_BIT(af.bitvector, pMobIndex->affect_bit[i]);
				affect_to_char(mob, &af);
			}
		}
	}
	else /* read in old format and convert */
	{
		copy_vector(mob->act, pMobIndex->act);
		copy_vector(mob->affected_by, pMobIndex->affected_by);

		mob->alignment = pMobIndex->alignment;
		mob->level = pMobIndex->level;
		mob->hitroll = pMobIndex->hitroll;
		mob->damroll = 0;
		mob->max_hit = mob->level * 8 + number_range(mob->level * mob->level / 4, mob->level * mob->level);
		mob->hit = mob->max_hit;
		mob->max_mana = 100 + dice(mob->level, 10);
		mob->mana = mob->max_mana;

		switch (number_range(1, 3))
		{
			case 1: /* slash */
				mob->dam_type = 3;
				break;
			case 2: /* pound */
				mob->dam_type = 7;
				break;
			case 3: /* pierce */
				mob->dam_type = 11;
				break;
		}

		for (i = 0; i < 3; i++)
		{
			mob->armor[i] = 0;
		}

		mob->armor[3] = 0;
		mob->race = pMobIndex->race;

		copy_vector(mob->off_flags, pMobIndex->off_flags);
		copy_vector(mob->imm_flags, pMobIndex->imm_flags);
		copy_vector(mob->res_flags, pMobIndex->res_flags);
		copy_vector(mob->vuln_flags, pMobIndex->vuln_flags);

		mob->start_pos = pMobIndex->start_pos;
		mob->sex = pMobIndex->sex;

		copy_vector(mob->form, pMobIndex->form);
		copy_vector(mob->parts, pMobIndex->parts);

		mob->size = SIZE_MEDIUM;
		mob->material = "";
		mob->arms = pMobIndex->arms;
		mob->legs = pMobIndex->legs;
		mob->regen_rate = 0;
		mob->timer = 0;

		for (i = 0; i < MAX_STATS; i++)
		{
			mob->perm_stat[i] = 11 + mob->level / 4;
		}
	}

	mob->position = mob->start_pos;
	mob->cabal = mob->pIndexData->cabal;
	mob->home_room = nullptr;

	/* link the mob to the world list */
	// Ownership moves to the list here, not in new_char. A mob exists unlinked
	// between the two, and the test fixtures build ones that never link at all.
	char_list.push_front(std::unique_ptr<CHAR_DATA>(mob));
	mob->globalNode = char_list.begin();

	pMobIndex->count++;

	return mob;
}

/* duplicate a mobile exactly -- except inventory */
void clone_mobile(CHAR_DATA *parent, CHAR_DATA *clone)
{
	int i;

	if (parent == nullptr || clone == nullptr || !is_npc(parent))
		return;

	/* start fixing values */
	clone->name = palloc_string(parent->name);
	clone->version = parent->version;
	clone->short_descr = palloc_string(parent->short_descr);
	clone->long_descr = palloc_string(parent->long_descr);
	clone->description = palloc_string(parent->description);
	clone->group = parent->group;
	clone->sex = parent->sex;
	clone->SetClass(parent->Class()->GetIndex());
	clone->race = parent->race;
	clone->level = parent->level;
	clone->trust = 0;
	clone->timer = parent->timer;
	clone->wait = parent->wait;
	clone->hit = parent->hit;
	clone->max_hit = parent->max_hit;
	clone->mana = parent->mana;
	clone->max_mana = parent->max_mana;
	clone->move = parent->move;
	clone->max_move = parent->max_move;
	clone->gold = parent->gold;
	clone->exp = parent->exp;

	copy_vector(clone->act, parent->act);
	copy_vector(clone->comm, parent->comm);
	copy_vector(clone->imm_flags, parent->imm_flags);
	copy_vector(clone->res_flags, parent->res_flags);
	copy_vector(clone->vuln_flags, parent->vuln_flags);

	clone->invis_level = parent->invis_level;

	copy_vector(clone->affected_by, parent->affected_by);

	clone->position = parent->position;
	clone->practice = parent->practice;
	clone->train = parent->train;
	clone->saving_throw = parent->saving_throw;
	clone->alignment = parent->alignment;
	clone->hitroll = parent->hitroll;
	clone->damroll = parent->damroll;
	clone->wimpy = parent->wimpy;

	copy_vector(clone->form, parent->form);
	copy_vector(clone->parts, parent->parts);

	clone->size = parent->size;

	copy_vector(clone->off_flags, parent->off_flags);

	clone->dam_type = parent->dam_type;
	clone->start_pos = parent->start_pos;
	clone->dam_mod = parent->dam_mod;
	clone->defense_mod = parent->defense_mod;
	clone->cabal = parent->cabal;
	clone->arms = parent->arms;
	clone->legs = parent->legs;

	for (i = 0; i < 4; i++)
	{
		clone->armor[i] = parent->armor[i];
	}

	for (i = 0; i < MAX_STATS; i++)
	{
		clone->perm_stat[i] = parent->perm_stat[i];
		clone->mod_stat[i] = parent->mod_stat[i];
	}

	for (i = 0; i < 3; i++)
	{
		clone->damage[i] = parent->damage[i];
	}

	/* now add the affects */
	for (auto &paf : parent->affected)
		affect_to_char(clone, &paf);
}

/*
 * Create an instance of an object.
 */
OBJ_DATA *create_object(OBJ_INDEX_DATA *pObjIndex, int level)
{
	OBJ_DATA *obj;

	if (pObjIndex == nullptr)
	{
		RS.Logger.Error("Create_object: nullptr pObjIndex.");
		exit(1);
	}

	obj = new_obj();
	obj->pIndexData = pObjIndex;
	obj->in_room = nullptr;
	obj->ohp = 0;

	if (pObjIndex->new_format)
		obj->level = pObjIndex->level;
	else
		obj->level = std::max(0, level);

	obj->wear_loc = -1;
	obj->name = palloc_string(pObjIndex->name);				  /* OLC */
	obj->short_descr = palloc_string(pObjIndex->short_descr); /* OLC */
	obj->description = palloc_string(pObjIndex->description); /* OLC */
	obj->material = palloc_string(pObjIndex->material);

	if (pObjIndex->wear_loc_name)
		obj->wear_loc_name = palloc_string(pObjIndex->wear_loc_name);
	else
		obj->wear_loc_name = nullptr;

	obj->item_type = pObjIndex->item_type;

	copy_vector(obj->extra_flags, pObjIndex->extra_flags);

	obj->apply = pObjIndex->apply;		// deep-copies the prototype's applies (was a shared pointer)
	obj->charaffs = pObjIndex->charaffs;	// deep-copies the prototype's charaffs (was a shared pointer)

	copy_vector(obj->wear_flags, pObjIndex->wear_flags);
	copy_vector(obj->imm_flags, pObjIndex->imm_flags);
	copy_vector(obj->res_flags, pObjIndex->res_flags);
	copy_vector(obj->vuln_flags, pObjIndex->vuln_flags);

	obj->value[0] = pObjIndex->value[0];
	obj->value[1] = pObjIndex->value[1];
	obj->value[2] = pObjIndex->value[2];
	obj->value[3] = pObjIndex->value[3];
	obj->value[4] = pObjIndex->value[4];
	obj->weight = pObjIndex->weight;
	copy_vector(obj->progtypes, pObjIndex->progtypes);

	/* Morg - Valgrind Fix */
	obj->contains = nullptr;

	if (!pObjIndex->extra_descr.empty())
		obj->extra_descr.push_back(pObjIndex->extra_descr.front());

	if (level == -1 || pObjIndex->new_format)
		obj->cost = pObjIndex->cost;
	else
		obj->cost = number_fuzzy(10) * number_fuzzy(level) * number_fuzzy(level);

	/*
	 * Mess with object properties.
	 */
	switch (obj->item_type)
	{
		case ITEM_LIGHT:
			if (obj->value[2] == 999)
				obj->value[2] = -1;

			break;
		case ITEM_FURNITURE:
		case ITEM_TRASH:
		case ITEM_CONTAINER:
		case ITEM_DRINK_CON:
		case ITEM_KEY:
		case ITEM_FOOD:
		case ITEM_BOAT:
		case ITEM_CORPSE_NPC:
		case ITEM_CORPSE_PC:
		case ITEM_FOUNTAIN:
		case ITEM_MAP:
		case ITEM_CLOTHING:
		case ITEM_PORTAL:
			if (!pObjIndex->new_format)
				obj->cost /= 5;

			break;
		case ITEM_TREASURE:
		case ITEM_WARP_STONE:
		case ITEM_ROOM_KEY:
		case ITEM_GEM:
		case ITEM_JEWELRY:
		case ITEM_SKELETON:
		case ITEM_DICE:
		case ITEM_URN:
		case ITEM_GRAVITYWELL:
		case ITEM_BOOK:
		case ITEM_PEN:
		case ITEM_ALTAR:
		case ITEM_STONE:
		case ITEM_CAMPFIRE:
			break;
		case ITEM_SCROLL:
			if (level != -1 && !pObjIndex->new_format)
				obj->value[0] = number_fuzzy(obj->value[0]);
			break;

		case ITEM_WAND:
		case ITEM_STAFF:
			if (level != -1 && !pObjIndex->new_format)
			{
				obj->value[0] = number_fuzzy(obj->value[0]);
				obj->value[1] = number_fuzzy(obj->value[1]);
				obj->value[2] = obj->value[1];
			}

			if (!pObjIndex->new_format)
				obj->cost *= 2;

			break;
		case ITEM_WEAPON:
			if (level != -1 && !pObjIndex->new_format)
			{
				obj->value[1] = number_fuzzy(number_fuzzy(1 * level / 4 + 2));
				obj->value[2] = number_fuzzy(number_fuzzy(3 * level / 4 + 6));
			}

			break;
		case ITEM_ARMOR:
			if (level != -1 && !pObjIndex->new_format)
			{
				obj->value[0] = number_fuzzy(level / 5 + 3);
				obj->value[1] = number_fuzzy(level / 5 + 3);
				obj->value[2] = number_fuzzy(level / 5 + 3);
			}

			break;
		case ITEM_POTION:
		case ITEM_PILL:
			if (level != -1 && !pObjIndex->new_format)
				obj->value[0] = number_fuzzy(number_fuzzy(obj->value[0]));

			break;
		case ITEM_MONEY:
			if (!pObjIndex->new_format)
				obj->value[0] = obj->cost;

			break;
		default:
			RS.Logger.Warn("Read_object: vnum {} bad type.", pObjIndex->vnum);
			break;
	}

	for (auto &paf : pObjIndex->affected)
	{
		if (paf.location == APPLY_SPELL_AFFECT)
		{
			OBJ_AFFECT_DATA oaf;

			init_affect_obj(&oaf);
			oaf.where = paf.where;
			oaf.type = paf.type;
			oaf.level = paf.level;
			oaf.duration = paf.duration;
			oaf.location = paf.location;
			oaf.modifier = paf.modifier;
			copy_vector(oaf.bitvector, paf.bitvector);
			affect_to_obj(obj, &oaf);
		}
	}

	obj->owner = palloc_string("none");

	// The list takes ownership here, not in new_obj: an object exists unlinked
	// between the two, and the test fixtures build objects that never get linked
	// at all.
	object_list.push_front(std::unique_ptr<OBJ_DATA>(obj));
	obj->globalNode = object_list.begin();

	pObjIndex->limcount += 1;

	return obj;
}

/* duplicate an object exactly -- except contents */
void clone_object(OBJ_DATA *parent, OBJ_DATA *clone)
{
	int i;

	if (parent == nullptr || clone == nullptr)
		return;

	/* start fixing the object */
	clone->name = palloc_string(parent->name);
	clone->short_descr = palloc_string(parent->short_descr);
	clone->description = palloc_string(parent->description);
	clone->item_type = parent->item_type;

	copy_vector(clone->extra_flags, parent->extra_flags);

	clone->apply = parent->apply;		// deep-copies the applies (was a shared pointer)

	copy_vector(clone->wear_flags, parent->wear_flags);
	copy_vector(clone->imm_flags, parent->imm_flags);
	copy_vector(clone->res_flags, parent->res_flags);
	copy_vector(clone->vuln_flags, parent->vuln_flags);

	clone->weight = parent->weight;
	clone->cost = parent->cost;
	clone->level = parent->level;
	clone->condition = parent->condition;
	clone->material = palloc_string(parent->material);
	clone->timer = parent->timer;
	clone->ohp = parent->ohp;

	if (parent->wear_loc_name)
		clone->wear_loc_name = palloc_string(parent->wear_loc_name);

	for (i = 0; i < 5; i++)
	{
		clone->value[i] = parent->value[i];
	}

	/* affects */

	for (auto &paf : parent->affected)
		affect_to_obj(clone, &paf);

	/* extended desc — prepend each (reverses order), matching the old intrusive list */
	for (const auto &ed : parent->extra_descr)
		clone->extra_descr.insert(clone->extra_descr.begin(), ed);
}

/*
 * Get an extra description from a list.
 */
char *get_extra_descr(const char *name, const std::list<EXTRA_DESCR_DATA> &eds)
{
	for (const auto &ed : eds)
	{
		if (is_name((char *)name, ed.keyword))
			return ed.description;
	}

	return nullptr;
}

/*
 * Translates mob virtual number to its mob index struct.
 * Hash table lookup.
 */
MOB_INDEX_DATA *get_mob_index(int vnum)
{
	MOB_INDEX_DATA *pMobIndex;

	for (pMobIndex = mob_index_hash[vnum % MAX_KEY_HASH]; pMobIndex != nullptr; pMobIndex = pMobIndex->next)
	{
		if (pMobIndex->vnum == vnum)
			return pMobIndex;
	}

	if (fBootDb)
	{
		RS.Logger.Error("Get_mob_index: funky vnum {}.", vnum);
		exit(1);
	}

	return nullptr;
}

/*
 * Translates mob virtual number to its obj index struct.
 * Hash table lookup.
 */
OBJ_INDEX_DATA *get_obj_index(int vnum)
{
	OBJ_INDEX_DATA *pObjIndex;

	for (pObjIndex = obj_index_hash[vnum % MAX_KEY_HASH]; pObjIndex != nullptr; pObjIndex = pObjIndex->next)
	{
		if (pObjIndex->vnum == vnum)
			return pObjIndex;
	}

	if (fBootDb)
	{
		RS.Logger.Error("Get_obj_index: bad vnum {}.", vnum);
		exit(1);
	}

	return nullptr;
}

/*
 * Translates mob virtual number to its room index struct.
 * Hash table lookup.
 */
ROOM_INDEX_DATA *get_room_index(int vnum)
{
	ROOM_INDEX_DATA *pRoomIndex;

	for (pRoomIndex = room_index_hash[vnum % MAX_KEY_HASH]; pRoomIndex != nullptr; pRoomIndex = pRoomIndex->next)
	{
		if (pRoomIndex->vnum == vnum)
			return pRoomIndex;
	}

	if (fBootDb)
	{
		RS.Logger.Error("Get_room_index: bad vnum %d.", vnum);
		exit(1);
	}

	return nullptr;
}

/*
 * Read a letter from a file.
 */
char fread_letter(FILE *fp)
{
	char c;

	do
	{
		c = getc(fp);
	} while (isspace(c));

	return c;
}

/*
 * Read a number from a file.
 */
int fread_number(FILE *fp)
{
	int number;
	bool sign;
	char c;
	char buf[MAX_STRING_LENGTH], dbuf[MAX_STRING_LENGTH];

	do
	{
		c = getc(fp);
	} while (isspace(c));

	number = 0;

	sign= false;

	if (c == '+')
	{
		c = getc(fp);
	}
	else if (c == '-')
	{
		sign = true;
		c = getc(fp);
	}

	if (!isdigit(c))
	{
		sprintf(buf, "Fread_number: bad format: %c", c);

		for (number = 0; (number < 25 && c != 0); number++)
		{
			c = getc(fp);
			sprintf(dbuf, "%c", c);
			strcat(buf, dbuf);
		}

		RS.Logger.Error(buf);
		exit(1);
	}

	while (isdigit(c))
	{
		/* Saturate instead of overflowing on malformed/oversized input. */
		if (number > (INT_MAX - (c - '0')) / 10)
			number = INT_MAX;
		else
			number = number * 10 + c - '0';

		c = getc(fp);
	}

	if (sign)
		number = 0 - number;

	if (c == '|')
		number += fread_number(fp);
	else if (c != ' ')
		ungetc(c, fp);

	return number;
}

long fread_flag(FILE *fp)
{
	int number;
	char c;
	bool negative= false;

	do
	{
		c = getc(fp);
	} while (isspace(c));

	if (c == '-')
	{
		negative = true;
		c = getc(fp);
	}

	number = 0;

	if (!isdigit(c))
	{
		while (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z'))
		{
			number += flag_convert(c);
			c = getc(fp);
		}
	}

	while (isdigit(c))
	{
		if (number > (INT_MAX - (c - '0')) / 10)
			number = INT_MAX;
		else
			number = number * 10 + c - '0';

		c = getc(fp);
	}

	if (c == '|')
		number += fread_flag(fp);
	else if (c != ' ')
		ungetc(c, fp);

	if (negative)
		return -1 * number;

	return number;
}

void fread_flag_new(long vector[], FILE *fp)
{
	char c;
	zero_vector(vector);
	do
	{
		c = getc(fp);
	} while (isspace(c));

	if (!isdigit(c))
	{
		while (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z'))
		{
			SET_BIT(vector, vector_convert(c));
			c = getc(fp);
		}
	}
	else if (c == '0')
		return;

	if (c == '|')
		fread_flag_new(vector, fp);
	else if (c != ' ')
		ungetc(c, fp);
}

long vector_convert(char letter)
{
	long bit = 0;

	if ('A' <= letter && letter <= 'Z')
		bit = letter - 'A';
	else if ('a' <= letter && letter <= 'z')
		bit = letter - ('a' - 26);

	return bit;
}

long flag_convert(char letter)
{
	long bitsum = 0;
	char i;

	if ('A' <= letter && letter <= 'Z')
	{
		bitsum = 1;

		for (i = letter; i > 'A'; i--)
		{
			bitsum *= 2;
		}
	}
	else if ('a' <= letter && letter <= 'z')
	{
		bitsum = 67108864; /* 2^26 */

		for (i = letter; i > 'a'; i--)
		{
			bitsum *= 2;
		}
	}

	return bitsum;
}

void fread_vector(long vector[], FILE *fp)
{
	for (int i = 0; i < MAX_BITVECTOR; i++)
	{
		vector[i] = fread_number(fp);
	}
}

/*
 * Read and allocate space for a string from a file.
 * These strings are read-only and shared.
 * Strings are hashed:
 *   each string prepended with hash pointer to prev string,
 *   hash code is simply the string length.
 *   this function takes 40% to 50% of boot-up time.
 */
char *fread_string(FILE *fp)
{
	char *plast;
	char buf[MAX_BUF], c;

	plast = &buf[0];
	/*
	 * Skip blanks.
	 * Read first char.
	 */
	do
	{
		c = getc(fp);
	} while (isspace(c));

	if ((*plast++ = c) == '~')
		return &str_empty[0];

	for (;;)
	{
		/*
		 * Back off the char type lookup,
		 *   it was too dirty for portability.
		 *   -- Furey
		 */

		switch (*plast = getc(fp))
		{
			case EOF:
				/* temp fix */
				RS.Logger.Warn("Fread_string: EOF");
				return nullptr;
				/* exit( 1 ); */
				break;
			case '\n':
				plast++;
				*plast++ = '\r';
				break;
			case '\r':
				break;
			case '~':
				plast++;
				plast[-1] = '\0';
				return palloc_string(buf);
				/*
				{
					union
					{
						char * pc;
						char rgc[sizeof(char *)];
					} u1;
					int ic;
					int iHash;
					char *pHash;
					char *pHashPrev;
					char *pString;

					plast[-1] = '\0';

					iHash = std::min(MAX_KEY_HASH - 1, plast - 1 - top_string);
					for (pHash = string_hash[iHash]; pHash; pHash = pHashPrev)
					{
						for (ic = 0; ic < sizeof(char *); ic++)
						{
							u1.rgc[ic] = pHash[ic];
						}

						pHashPrev = u1.pc;
						pHash    += sizeof(char *);

						if (top_string[sizeof(char *)] == pHash[0]
							&& !strcmp( top_string+sizeof(char *)+1, pHash+1))
						{
							return pHash;
						}
					}

					if (fBootDb)
					{
						pString	= top_string;
						top_string	= plast;
						u1.pc = string_hash[iHash];
						for (ic = 0; ic < sizeof(char *); ic++)
						{
							pString[ic] = u1.rgc[ic];
						}

						string_hash[iHash]	= pString;

						nAllocString += 1;
						sAllocString += top_string - pString;
						return pString + sizeof(char *);
					}
					else
					{
						return palloc_string(top_string + sizeof(char *));
					}
				}
				*/
			default:
				plast++;
				break;
		}
	}
}

char *fread_string_eol(FILE *fp)
{
	static bool char_special[256 - EOF];
	char *plast;
	char buf[MSL], c;

	if (char_special[EOF - EOF] != true)
	{
		char_special[EOF - EOF] = true;
		char_special['\n' - EOF] = true;
		char_special['\r' - EOF] = true;
	}

	/*
	 * Skip blanks.
	 * Read first char.
	 */
	plast = &buf[0];

	do
	{
		c = getc(fp);
	} while (isspace(c));

	if ((*plast++ = c) == '\n')
		return &str_empty[0];

	for (;;)
	{
		if (!char_special[(*plast++ = getc(fp)) - EOF])
			continue;

		switch (plast[-1])
		{
			case EOF:
				RS.Logger.Error("Fread_string_eol  EOF");
				exit(1);
				break;
			case '\n':
			case '\r':
				plast[-1] = '\0';
				return palloc_string(buf);
			default:
				break;
		}
	}
}

/*
 * Read to end of line (for comments).
 */
void fread_to_eol(FILE *fp)
{
	char c;

	do
	{
		c = getc(fp);
	} while (c != '\n' && c != '\r');

	do
	{
		c = getc(fp);
	} while (c == '\n' || c == '\r');

	ungetc(c, fp);
}

/*
 * Read one word (into static buffer).
 */
char *fread_word(FILE *fp)
{
	static char word[MAX_INPUT_LENGTH];
	char *pword;
	char cEnd;

	do
	{
		cEnd = getc(fp);
	} while (isspace(cEnd));

	if (cEnd == '\'' || cEnd == '"')
	{
		pword = word;
	}
	else
	{
		word[0] = cEnd;
		pword = word + 1;
		cEnd = ' ';
	}

	for (; pword < word + MAX_INPUT_LENGTH; pword++)
	{
		*pword = getc(fp);
		if (cEnd == ' ' ? isspace(*pword) : *pword == cEnd)
		{
			if (cEnd == ' ')
				ungetc(*pword, fp);

			*pword = '\0';
			return word;
		}
	}

	RS.Logger.Error("Fread_word: word too long [{}].", word);
	exit(1);
	return nullptr;
}

/*
 * Allocate some ordinary memory,
 *   with the expectation of freeing it someday.
 */
void *iamlame(int sMem)
{
	/*
	void *pMem;
	int *magic;
	int iList;

	sMem += sizeof(*magic);

	for (iList = 0; iList < MAX_MEM_LIST; iList++)
	{
		if (sMem <= rgSizeList[iList])
			break;
	}

	if (iList == MAX_MEM_LIST)
	{
		RS.Logger.Error("Alloc_mem: size {} too large.", sMem);
		exit(1);
	}

	if (rgFreeList[iList] == nullptr)
	{
		pMem = palloc_struct(rgSizeList[iList]);
	}
	else
	{
		pMem = rgFreeList[iList];
		rgFreeList[iList] = * ((void **) rgFreeList[iList]);
	}

	magic = (int *) pMem;
	*magic = MAGIC_NUM;
	pMem += sizeof(*magic);

	return pMem;
	*/
	return nullptr;
}

/*
 * Free some memory.
 * Recycle it back onto the free list for blocks of that size.
 */
void soami(void *pMem, int sMem)
{
	/*
	int iList;
	int *magic;

	pMem -= sizeof(*magic);
	magic = (int *) pMem;

	if (*magic != MAGIC_NUM)
	{
		//RS.Logger.Warn("Attempt to recycle invalid memory of size {}.",sMem);
		//RS.Logger.Warn((char*) pMem + sizeof(*magic));
		return;
	}

	*magic = 0;
	sMem += sizeof(*magic);

	for (iList = 0; iList < MAX_MEM_LIST; iList++)
	{
		if (sMem <= rgSizeList[iList])
			break;
	}

	if (iList == MAX_MEM_LIST)
	{
		RS.Logger.Error("Free_mem: size {} too large.", sMem);
		exit(1);
	}

	* ((void **) pMem) = rgFreeList[iList];
	rgFreeList[iList]  = pMem;
	*/
}

/*
 * Allocate some permanent memory.
 * Permanent memory is never freed,
 *   pointers into it may be copied safely.
 */
void *crappyold(int sMem)
{
	/*
	static char *pMemPerm;
	static int iMemPerm;
	void *pMem;

	while (sMem % sizeof(long) != 0)
	{
		sMem++;
	}

	if (sMem > MAX_PERM_BLOCK)
	{
		RS.Logger.Error("Alloc_perm: {} too large.", sMem);
		exit(1);
	}

	if (pMemPerm == nullptr || iMemPerm + sMem > MAX_PERM_BLOCK)
	{
		iMemPerm = 0;

		if ((pMemPerm = calloc( 1, MAX_PERM_BLOCK ) ) == nullptr)
		{
			RS.Logger.Error("Crappyold: calloc: {}", std::strerror(errno));
			exit(1);
		}
	}

	pMem = pMemPerm + iMemPerm;
	iMemPerm += sMem;
	nAllocPerm += 1;
	sAllocPerm += sMem;
	return pMem;
	*/
	return nullptr;
}

/*
 * Duplicate a string into dynamic memory.
 * Fread_strings are read-only and shared.
 */
char *iamnotereet(const char *str)
{
	/*
	char *str_new;

	if (str[0] == '\0')
		return &str_empty[0];

	if (str >= string_space && str < top_string)
		return (char *) str;

	str_new = palloc_struct(strlen(str) + 1);
	strcpy(str_new, str);
	return str_new;
	*/
	return nullptr;
}

void do_areas(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];
	AREA_DATA *pArea;
	int iArea = 0;

	if (strcmp(argument, "") && (!is_number(argument) || (iArea = atoi(argument)) < 1 || iArea > 51))
	{
		send_to_char("The area's level range must be between 1 and 51.\n\r", ch);
		return;
	}

	for (pArea = area_first; pArea; pArea = pArea->next)
	{
		if (pArea->area_type == ARE_UNOPENED || pArea->area_type == ARE_CABAL || pArea->area_type == ARE_SHRINE)
			continue;

		if (iArea && (pArea->low_range > iArea || pArea->high_range < iArea))
			continue;

		if (pArea->low_range == 1 && pArea->high_range == 51)
			sprintf(buf, "[  ALL  ] %-20s - %s\n\r", pArea->credits, pArea->name);
		else
			sprintf(buf, "[%2d - %2d] %-20s - %s\n\r", pArea->low_range, pArea->high_range, pArea->credits, pArea->name);

		send_to_char(buf, ch);
	}
}

void do_memory(CHAR_DATA *ch, [[maybe_unused]] char *argument)
{
	char buf[MAX_STRING_LENGTH];

	sprintf(buf, "Affects %5d\n\r", top_affect);
	send_to_char(buf, ch);

	sprintf(buf, "Areas   %5d\n\r", top_area);
	send_to_char(buf, ch);

	sprintf(buf, "ExDes   %5d\n\r", top_ed);
	send_to_char(buf, ch);

	sprintf(buf, "Exits   %5d\n\r", top_exit);
	send_to_char(buf, ch);

	sprintf(buf, "Helps   %5d\n\r", top_help);
	send_to_char(buf, ch);

	sprintf(buf, "Socials %5d\n\r", social_count);
	send_to_char(buf, ch);

	sprintf(buf, "Mobs    %5d(%d new format)\n\r", top_mob_index, newmobs);
	send_to_char(buf, ch);

	sprintf(buf, "(in use)%5d\n\r", mobile_count);
	send_to_char(buf, ch);

	sprintf(buf, "Objs    %5d(%d new format)\n\r", top_obj_index, newobjs);
	send_to_char(buf, ch);

	sprintf(buf, "Resets  %5d\n\r", top_reset);
	send_to_char(buf, ch);

	sprintf(buf, "Rooms   %5d\n\r", top_room);
	send_to_char(buf, ch);

	sprintf(buf, "Shops   %5d\n\r", top_shop);
	send_to_char(buf, ch);

	sprintf(buf, "Strings %5d strings of %7d bytes.\n\r", nAllocString, sAllocString);
	send_to_char(buf, ch);

	sprintf(buf, "Perms   %5d blocks  of %7d bytes.\n\r", nAllocPerm, sAllocPerm);
	send_to_char(buf, ch);
}

void do_dump([[maybe_unused]] CHAR_DATA *ch, [[maybe_unused]] char *argument)
{
	int count, count2, num_pcs, aff_count;
	MOB_INDEX_DATA *pMobIndex;
	OBJ_DATA *obj;
	OBJ_INDEX_DATA *pObjIndex;
	ROOM_INDEX_DATA *room;
	EXIT_DATA *exit;
	FILE *fp;
	int vnum, nMatch = 0;

	fp = fopen(MEM_DUMP_FILE, "w");
	if (fp == nullptr)
	{
		RS.Logger.Warn("Unable to open mem dump file: fopen {}: {}", MEM_DUMP_FILE, std::strerror(errno));
		return;
	}


	/* report use of data structures */

	num_pcs = 0;
	aff_count = 0;

	/* mobile prototypes */
	fprintf(fp, "MobProt	%4d (%8lu bytes)\n", top_mob_index, top_mob_index * (sizeof(*pMobIndex)));

	/* mobs */
	count = 0;
	count2 = 0;

	for (auto &owned : char_list)
	{
		count++;

		if (owned->pcdata != nullptr)
			num_pcs++;

		aff_count += owned->affected.size();
	}

	// No free list any more. char_list owns its characters, so an extracted one
	// is destroyed rather than parked for reuse.
	fprintf(fp, "Mobs	%4d (%8lu bytes), %2d free (%lu bytes)\n", count, count * (sizeof(CHAR_DATA)), count2, count2 * (sizeof(CHAR_DATA)));

	/* pcdata — no free list any more (pcdata is owned by unique_ptr) */
	count = 0;

	fprintf(fp, "Pcdata	%4d (%8lu bytes), %2d free (%lu bytes)\n", num_pcs, num_pcs * (sizeof(PC_DATA)), count, count * (sizeof(PC_DATA)));

	/* descriptors */
	// No free list any more. descriptor_list owns its connections, so a closed
	// one is destroyed rather than parked for reuse.
	count = descriptor_list.size();
	count2 = 0;

	fprintf(fp, "Descs	%4d (%8lu bytes), %2d free (%lu bytes)\n", count, count * (sizeof(DESCRIPTOR_DATA)), count2, count2 * (sizeof(DESCRIPTOR_DATA)));

	/* object prototypes */
	for (vnum = 0; nMatch < top_obj_index; vnum++)
	{
		pObjIndex = get_obj_index(vnum);

		if (pObjIndex != nullptr)
		{
			aff_count += pObjIndex->affected.size();

			nMatch++;
		}
	}

	fprintf(fp, "ObjProt	%4d (%8lu bytes)\n", top_obj_index, top_obj_index * (sizeof(*pObjIndex)));

	/* objects */
	count = 0;
	count2 = 0;

	for (auto &owned : object_list)
	{
		count++;

		aff_count += owned->affected.size();
	}

	// No free list any more -- object_list owns its objects, so a freed one is
	// returned to the allocator rather than parked for reuse.
	fprintf(fp, "Objs	%4d (%8lu bytes), %2d free (%lu bytes)\n", count, count * (sizeof(OBJ_DATA)), count2, count2 * (sizeof(OBJ_DATA)));

	/* affects — no free list any more (affects are owned by value) */
	count = 0;

	fprintf(fp, "Affects	%4d (%8lu bytes), %2d free (%lu bytes)\n", aff_count, aff_count * (sizeof(AFFECT_DATA)), count, count * (sizeof(AFFECT_DATA)));

	/* rooms */
	fprintf(fp, "Rooms	%4d (%8lu bytes)\n", top_room, top_room * (sizeof(*room)));

	/* exits */
	fprintf(fp, "Exits	%4d (%8lu bytes)\n", top_exit, top_exit * (sizeof(*exit)));

	fclose(fp);

	/* start printing out mobile data */
	fp = fopen(MOB_DUMP_FILE, "w");
	if (fp == nullptr)
	{
		RS.Logger.Warn("Unable to open mob dump file: fopen {}: {}", MOB_DUMP_FILE, std::strerror(errno));
		return;
	}

	fprintf(fp, "\nMobile Analysis\n");
	fprintf(fp, "---------------\n");

	nMatch = 0;

	for (vnum = 0; nMatch < top_mob_index; vnum++)
	{
		pMobIndex = get_mob_index(vnum);

		if (pMobIndex != nullptr)
		{
			nMatch++;
			fprintf(fp, "#%-4d %3d active %3d killed     %s\n", pMobIndex->vnum, pMobIndex->count, pMobIndex->killed, pMobIndex->short_descr);
		}
	}

	fclose(fp);

	/* start printing out object data */
	fp = fopen(OBJ_DUMP_FILE, "w");
	if (fp == nullptr)
	{
		RS.Logger.Warn("Unable to open obj dump file: fopen {}: {}", OBJ_DUMP_FILE, std::strerror(errno));
		return;
	}

	fprintf(fp, "\nObject Analysis\n");
	fprintf(fp, "---------------\n");

	nMatch = 0;

	for (vnum = 0; nMatch < top_obj_index; vnum++)
	{
		pObjIndex = get_obj_index(vnum);

		if (pObjIndex != nullptr)
		{
			nMatch++;
			fprintf(fp, "#%-4d %3d active %3d reset      %s\n", pObjIndex->vnum, pObjIndex->count, pObjIndex->reset_num, pObjIndex->short_descr);
		}
	}

	/* close file */
	fclose(fp);
}

/*
 * Stick a little fuzz on a number.
 */
int number_fuzzy(int number)
{
	switch (number_bits(2))
	{
		case 0:
			number -= 1;
			break;
		case 3:
			number += 1;
			break;
	}

	return std::max(1, number);
}

/*
 * Generate a random number.
 */
int number_range(int from, int to)
{
	int power;
	int number;

	if (from == 0 && to == 0)
		return 0;

	to = to - from + 1;

	if (to <= 1)
		return from;

	for (power = 2; power < to; power <<= 1);
	while ((number = number_mm() & (power - 1)) >= to);

	return from + number;
}

///
/// Generate a percentile roll.
///
/// @return a random integer from 1 - 99
///
int number_percent(void)
{
	int percent;
	while ((percent = number_mm() & 127) > 99);

	return 1 + percent;
}

/*
 * Generate a random door.
 */
int number_door(void)
{
	int door;

	while ((door = number_mm() & (8 - 1)) > 5);

	return door;
}

int number_bits(int width)
{
	return number_mm() & ((1 << width) - 1);
}

/*
 * I've gotten too many bad reports on OS-supplied random number generators.
 * This is the Mitchell-Moore algorithm from Knuth Volume II.
 * Best to leave the constants alone unless you've read Knuth.
 * -- Furey
 */

/* I noticed streaking with this random number generator, so I switched
   back to the system srandom call.  If this doesn't work for you,
   define OLD_RAND to use the old system -- Alander */

#ifdef OLD_RAND
static int rgiState[2 + 55];
#endif

///
/// This is mandatory for the random number generator to work, and MUST run 
/// during the startup phase for randomization to function properly in the game.
///
void init_mm()
{
#ifdef OLD_RAND
	int *piState;
	int iState;

	piState = &rgiState[2];

	piState[-2] = 55 - 55;
	piState[-1] = 55 - 24;

	piState[0] = ((int)current_time) & ((1 << 30) - 1);
	piState[1] = 1;
	for (iState = 2; iState < 55; iState++)
	{
		piState[iState] = (piState[iState - 1] + piState[iState - 2]) & ((1 << 30) - 1);
	}
#else
	srandom(time(nullptr) ^ getpid());
#endif
}

///
/// Generates a random number using Mitchell-Moore algorithm from Knuth Volume II.
///
/// @return a randomly generated long.
///
long number_mm(void)
{
#ifdef OLD_RAND
	
	if(rgiState[0] == 0) // check if they're all zero, if so call init_mm()
	{
		bool isAnyrgiStateNumberNotZero = false;
		const int rgiState_size = std::size(rgiState);
		for(auto i = 0; i < rgiState_size; i++)
		{
			if(rgiState[i] != 0)
			{
				isAnyrgiStateNumberNotZero = true;
				break;
			}
		}
		if(isAnyrgiStateNumberNotZero == false)
		{
			init_mm();
		}
	}
	int *piState;
	int iState1;
	int iState2;
	int iRand;

	piState = &rgiState[2];
	iState1 = piState[-2];
	iState2 = piState[-1];
	iRand = (piState[iState1] + piState[iState2]) & ((1 << 30) - 1);
	piState[iState1] = iRand;
	if (++iState1 == 55)
		iState1 = 0;
	if (++iState2 == 55)
		iState2 = 0;
	piState[-2] = iState1;
	piState[-1] = iState2;
	return iRand >> 6;
#else
	return rand() >> 6;
#endif
}

/*
 * Roll some dice.
 */
int dice(int number, int size)
{
	int idice;
	int sum;

	switch (size)
	{
		case 0:
			return 0;
		case 1:
			return number;
	}

	for (idice = 0, sum = 0; idice < number; idice++)
	{
		sum += number_range(1, size);
	}

	return sum;
}

/*
 * Simple linear interpolation.
 */
int interpolate(int level, int value_00, int value_32)
{
 	return value_00 + level * (value_32 - value_00) / 32;
}

/*
 * Removes the tildes from a string.
 * Used for player-entered strings that go into disk files.
 */
void smash_tilde(char *str)
{
	for (; *str != '\0'; str++)
	{
		if (*str == '~')
			*str = '-';
	}
}

/*
 * Compare strings, case insensitive.
 * Return true if different
 *   (compatibility with historical functions).
 */
bool str_cmp(const char *astr, const char *bstr)
{
	if (astr == nullptr)
	{
		//	RS.Logger.Warn("Str_cmp: null astr, bstr: {} ({}:{} dev=pimp)",gLastFile, gLastLine, bstr);
		return true;
	}

	if (bstr == nullptr)
	{
		//	RS.Logger.Warn("Str_cmp: null bstr, astr: {}", astr);
		return true;
	}

	for (; *astr || *bstr; astr++, bstr++)
	{
		if (LOWER(*astr) != LOWER(*bstr))
			return true;
	}

	return false;
}

/*
 * Compare strings, case insensitive, for prefix matching.
 * Return true if astr not a prefix of bstr
 *   (compatibility with historical functions).
 */
bool str_prefix(const char *astr, const char *bstr)
{
	if (astr == nullptr)
	{
		RS.Logger.Debug("Strn_cmp: null astr.");
		return true;
	}

	if (bstr == nullptr)
	{
		RS.Logger.Debug("Strn_cmp: null bstr.");
		return true;
	}

	for (; *astr; astr++, bstr++)
	{
		if (LOWER(*astr) != LOWER(*bstr))
			return true;
	}

	return false;
}

/*
 * Compare strings, case insensitive, for match anywhere.
 * Returns true is astr not part of bstr.
 *   (compatibility with historical functions).
 */
bool str_infix(const char *astr, const char *bstr)
{
	int sstr1;
	int sstr2;
	int ichar;
	char c0;

	c0 = LOWER(astr[0]);

	if (c0 == '\0')
		return false;

	sstr1 = strlen(astr);
	sstr2 = strlen(bstr);

	for (ichar = 0; ichar <= sstr2 - sstr1; ichar++)
	{
		if (c0 == LOWER(bstr[ichar]) && !str_prefix(astr, bstr + ichar))
			return false;
	}

	return true;
}

/*
 * Compare strings, case insensitive, for suffix matching.
 * Return true if astr not a suffix of bstr
 *   (compatibility with historical functions).
 */
bool str_suffix(const char *astr, const char *bstr)
{
	int sstr1 = strlen(astr);
	int sstr2 = strlen(bstr);

	if (sstr1 <= sstr2 && !str_cmp(astr, bstr + sstr2 - sstr1))
		return false;
	else
		return true;
}

/*
 * Returns an initial-capped string.
 */
char *capitalize(const char *str)
{
	static char strcap[MAX_STRING_LENGTH];
	int i;

	for (i = 0; str[i] != '\0'; i++)
	{
		strcap[i] = LOWER(str[i]);
	}

	strcap[i] = '\0';
	strcap[0] = UPPER(strcap[0]);
	return strcap;
}

/*
 * Append a string to a file.
 */
void append_file(CHAR_DATA *ch, const char *file, char *str)
{
	FILE *fp;
	char buf[MSL];

	if (is_npc(ch) || str[0] == '\0')
		return;

	fp = fopen(file, "a");

	if (fp == nullptr)
	{
		RS.Logger.Warn("Append_file: fopen {}: {}", file, std::strerror(errno));
		send_to_char("Could not open the file!\n\r", ch);
	}
	else
	{
		// free_pstring(buf);
		strcpy(buf, ctime(&current_time));
		chop(buf);
		fprintf(fp, "[%5d] %s: %s: %s\n", ch->in_room ? ch->in_room->vnum : 0, ch->true_name, buf, str);
		fclose(fp);
	}
}

/*
 * This function is here to aid in debugging.
 * If the last expression in a function is another function call,
 *   gcc likes to generate a JMP instead of a CALL.
 * This is called "tail chaining."
 * It hoses the debugger call stack for that call.
 * So I make this the last call in certain critical functions,
 *   where I really need the call stack to be right for debugging!
 *
 * If you don't understand this, then LEAVE IT ALONE.
 * Don't remove any calls to tail_chain anywhere.
 *
 * -- Furey
 */
void tail_chain(void)
{
	return;
}

void do_force_reset(CHAR_DATA *ch, [[maybe_unused]] char *argument)
{
	AREA_DATA *pArea;
	char buf[MAX_STRING_LENGTH];

	if (get_trust(ch) < LEVEL_IMMORTAL && !is_heroimm(ch))
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	if (get_trust(ch) < 56 && !IS_BUILDER(ch, ch->in_room->area))
	{
		send_to_char("You don't have access to reset this area.\n\r", ch);
		return;
	}

	pArea = ch->in_room->area;

	for (OwningListWalk<CHAR_DATA> walk(char_list); !walk.Done(); walk.Step())
	{
		CHAR_DATA *ich = walk.Current();

		if (is_npc(ich) && ich->in_room && ich->in_room->area == pArea && pArea->area_type == ARE_UNOPENED)
			extract_char(ich, true);
	}

	reset_area(pArea);
	pArea->age = number_range(0, 3);

	sprintf(buf, "Area: %s reset.\n\r", pArea->name);
	send_to_char(buf, ch);


	sprintf(buf, "Area: %s reset by %s.\n\r", pArea->name, ch->name);
}

/*
void do_alist(CHAR_DATA *ch,char *argument)
{
	//Alist spifferized by Dev.
	AREA_DATA *pArea;
	char buf[MAX_STRING_LENGTH], buf2[200];
	char *list[300], *temp;
	int i = 0, a, b, x, avnum[300], itemp;

	if (ch->level < 52 && !IS_SET(ch->comm,COMM_BUILDER))
		return send_to_char("Huh?\n\r",ch);

	if(str_cmp(argument,"") && is_number(argument))
	{
	i = atoi(argument);
	buf[0]='\0';

	for(pArea = area_first; pArea != nullptr; pArea = pArea->next)
	{
		if(pArea->min_vnum<=i && pArea->max_vnum>=i)
				sprintf(buf,"%-25s: %4d %4d\n\r",pArea->name,pArea->min_vnum,pArea->max_vnum);
	}

	if(buf[0]!='\0')
	{
		sprintf(buf2,"The following area contains vnum %i:\n\r%s",i,buf);
		return send_to_char(buf2,ch);
	}
	else
		return send_to_char("No areas containing that vnum were found.\n\r",ch);
	}

	for (pArea = area_first; pArea != nullptr; pArea = pArea->next)
	{
		i++;
		sprintf(buf,"%-25s: %4d %4d",pArea->name,pArea->min_vnum,pArea->max_vnum);
		list[i] = palloc_string(buf);
		avnum[i] = pArea->min_vnum;
	}

	for(a = 1; a<i-1; a++)
	{
		for(b = a+1; b<i; b++)
		{
			if(avnum[a] > avnum[b])
			{
				temp = list[a];
				list[a] = list[b];
				list[b] = temp;
				itemp = avnum[a];
				avnum[a] = avnum[b];
				avnum[b] = itemp;
			}
		}
	}

	for(x = 1; x<=i; x = x+2)
	{
		sprintf(buf,"%-40s %s\n\r",list[x],x+1<=i ? list[x+1] : "");
		send_to_char(buf,ch);
	}

	send_to_char("Use alist <vnum> to see what area occupies the given vnum.\n\r",ch);
}
*/

void do_llimit(CHAR_DATA *ch, [[maybe_unused]] char *argument)
{
	FILE *fpChar;
	char strPlr[MAX_INPUT_LENGTH];
//	char catplr[MAX_INPUT_LENGTH];
	char chkbuf[MAX_STRING_LENGTH];
//	char pbuf[100];
	OBJ_INDEX_DATA *lObjIndex;
	OBJ_DATA *obj;
	int i;
	CHAR_DATA *carrier;

	/* reset limit counts and recount items in game , do NOT count items on
	players loaded */
	send_to_char("Reseting all counts to zero now.\n\r", ch);
	for (i = 0; i < 30000; i++)
	{
		lObjIndex = get_obj_index(i);

		if (lObjIndex == nullptr)
			continue;

		lObjIndex->limcount = 0;
	}

	send_to_char("Counts set to zero....\n\r", ch);
	send_to_char("Loading all in-game counts now (excluding PC objects).\n\r", ch);

	for (auto &owned : object_list)
	{
		obj = owned.get();
		carrier = Deref(obj->carried_by);

		if (carrier != nullptr && !is_npc(carrier))
			continue;

		obj->pIndexData->limcount++;
	}

	send_to_char("Loading all pfile object counts now.\n\r", ch);

	auto dir = CDirectory(RIFT_PLAYER_DIR);
	auto files = dir.GetFiles(".plr");

	std::ofstream player_list(PLAYER_LIST, std::ios::trunc);
	for(auto file : files) player_list << file << std::endl;
	player_list.close();

	sprintf(chkbuf, "%s/%s", RIFT_PLAYER_DIR, "Zzz.plr");
	for (auto file : files)
	{
		strcpy(strPlr, file.data());
		if(bDebug)
			RS.Logger.Debug(strPlr);

		if (!str_cmp(strPlr, chkbuf))
			break; /* Exit if == Zzz.plr file */

		fpChar = fopen(strPlr, "r");
		if (fpChar == nullptr)
		{
			RS.Logger.Error("Do_llimit: fopen {}: {}", strPlr, std::strerror(errno));
			exit(1);
		}

		for (;;)
		{
			int vnum;
			char letter;
			char *word;
			OBJ_INDEX_DATA *pObjIndex;

			letter = fread_letter(fpChar);

			if (letter != '#')
				continue;

			word = fread_word(fpChar);

			if (!str_cmp(word, "End"))
				break;

			if (!str_cmp(word, "O") || !str_cmp(word, "OBJECT"))
			{
				word = fread_word(fpChar);

				if (!str_cmp(word, "Vnum"))
				{
					vnum = fread_number(fpChar);
					if (get_obj_index(vnum) == nullptr)
					{
						/*	RS.Logger.Warn("Bad obj vnum in limits: {}",vnum); */
					}
					else
					{
						pObjIndex = get_obj_index(vnum);
						pObjIndex->limcount++;
					}
				}
			}
		}

		fclose(fpChar);
		fpChar = nullptr;
	}

	send_to_char("All object counts reset.\n\r", ch);
}

/*
 * Snarf a room section.
 */
void load_rooms(FILE *fp)
{
	ROOM_INDEX_DATA *pRoomIndex;
	char error_v[MAX_STRING_LENGTH];

	if (area_last == nullptr)
		bugout("Load_resets: no #AREA seen yet.");

	for (;;)
	{
		short vnum = 0;
		char letter;
		char *bword;
		long bitvect;
		int door;
		int iHash;

		letter = fread_letter(fp);
		if (letter != '#')
		{
			RS.Logger.Error("Letter is {}.", letter);
			RS.Logger.Error("Load_newrooms: # not found, last vnum {}.", vnum);
			exit(0);
		}

		vnum = fread_number(fp);
		if (vnum == 0)
			break;

		fBootDb= false;

		if (get_room_index(vnum) != nullptr)
		{
			sprintf(error_v, "Load_rooms: vnum %d duplicated.", vnum);
			bugout(error_v);
		}

		fBootDb = true;

		// Value-init, not default-init: the scalar members are read before
		// anything assigns them -- `rune` in particular, which is now the only
		// record of whether this room carries one.
		pRoomIndex = new ROOM_INDEX_DATA();
		pRoomIndex->reset_first = nullptr;
		pRoomIndex->reset_last = nullptr;
		pRoomIndex->owner = palloc_string("");
		pRoomIndex->move_progs= false;
		pRoomIndex->people = nullptr;
		pRoomIndex->contents = nullptr;
		pRoomIndex->extra_descr.clear();
		pRoomIndex->alt_description = nullptr;
		pRoomIndex->alt_description_cond = 0;
		pRoomIndex->alt_name = nullptr;
		pRoomIndex->trap = nullptr;
		pRoomIndex->affected.clear();
		zero_vector(pRoomIndex->room_flags);

		pRoomIndex->area = area_last;
		pRoomIndex->vnum = vnum;
		pRoomIndex->name = fread_string(fp);

		if (pRoomIndex->name[strlen(pRoomIndex->name) - 1] == '.')
			chop(pRoomIndex->name);

		pRoomIndex->description = fread_string(fp);
		pRoomIndex->sector_type = sect_table[sect_lookup(fread_word(fp))].value;

		/* Morg - Valgrind fix. */
		pRoomIndex->rprogs = nullptr;
		zero_vector(pRoomIndex->progtypes);
		pRoomIndex->cabal = 0;

		zero_vector(pRoomIndex->affected_by);
		if (pRoomIndex->area->area_type == ARE_SHRINE)
		{
			SET_BIT(pRoomIndex->room_flags, ROOM_NO_SUMMON_TO);
			SET_BIT(pRoomIndex->room_flags, ROOM_NO_SUMMON_FROM);
			SET_BIT(pRoomIndex->room_flags, ROOM_NO_GATE);
		}

		const int pRoomIndex_exit_size = std::size(pRoomIndex->exit);
		for (door = 0; door < pRoomIndex_exit_size; door++)
		{
			pRoomIndex->exit[door] = nullptr;
		}

		/* defaults */
		pRoomIndex->heal_rate = 100;
		pRoomIndex->mana_rate = 100;
		pRoomIndex->light = 0;

		for (;;)
		{
			letter = fread_letter(fp);
			letter = UPPER(letter);
			if (letter == 'S')
				break;

			if (letter == 'H') /* healing room */
				pRoomIndex->heal_rate = fread_number(fp);
			else if (letter == 'M') /* mana room */
				pRoomIndex->mana_rate = fread_number(fp);
			else if (letter == 'G') /* Guilds...added by Ceran */
				pRoomIndex->guild = fread_number(fp);
			else if (letter == 'A') // ALTDESC
			{
				fread_word(fp);
				pRoomIndex->alt_description_cond = fread_number(fp);
				pRoomIndex->alt_name = fread_string(fp);
				pRoomIndex->alt_description = fread_string(fp);
			}
			else if (letter == 'C') /* Cabal */
			{
				fread_word(fp);
				if (pRoomIndex->cabal)
					bugout("Load_rooms: duplicate cabal fields.");
				pRoomIndex->cabal = cabal_lookup(fread_word(fp));
			}
			else if (letter == 'D')
			{
				EXIT_DATA *pexit;

				door = flag_lookup(fread_word(fp), direction_table);
				if (door < 0 || door > 5)
				{
					RS.Logger.Error("Fread_rooms: vnum {} has bad door number.", vnum);
					exit(1);
				}

				// Value-init for the same reason as the room above. This one
				// was the worse of the two: nothing zeroed the exit's rune
				// fields at all, so every exit in the world booted with an
				// indeterminate rune pointer behind an indeterminate flag.
				pexit = new EXIT_DATA();
				pexit->u1.vnum = fread_number(fp);
				fread_flag_new(pexit->exit_info, fp);
				pexit->key = fread_number(fp);
				pexit->keyword = fread_string(fp);
				pexit->description = fread_string(fp);

				pRoomIndex->exit[door] = pexit;
				top_exit++;
			}
			else if (letter == 'E')
			{
				EXTRA_DESCR_DATA ed;

				ed.keyword = fread_string(fp);
				ed.description = fread_string(fp);
				pRoomIndex->extra_descr.insert(pRoomIndex->extra_descr.begin(), std::move(ed));
				top_ed++;
			}
			else if (letter == 'T')
			{
				TRAP_DATA *trap;
				fread_word(fp);

				trap = new_trap();
				trap->room = pRoomIndex;
				trap->type = flag_lookup(fread_word(fp), trap_table);
				trap->quality = fread_number(fp);

				if (trap->quality > 10)
					trap->quality = 10;

				trap->complexity = fread_number(fp);
				trap->timer = fread_number(fp);

				if (trap->timer < 0)
					trap->timer = 0;

				trap->trig_echo = fread_string(fp);
				trap->exec_echo = fread_string(fp);
				trap->armed = true;
				pRoomIndex->trap = trap;
			}
			else if (letter == 'O')
			{
				fread_word(fp);

				if (pRoomIndex->owner[0] != '\0')
					bugout("Load_rooms: duplicate owner.");

				pRoomIndex->owner = fread_string(fp);
			}
			else if (letter == 'R')
			{
				fread_word(fp);
				bword = fread_word(fp);
				bitvect = flag_lookup(bword, room_flags);
				if (bitvect == NO_FLAG && str_cmp(bword, "dark"))
					bugout("Invalid bitvector specified for room flags.");
				SET_BIT(pRoomIndex->room_flags, bitvect);
			}
			else
			{
				ungetc(letter, fp);
				break;
			}
		}

		iHash = vnum % MAX_KEY_HASH;
		pRoomIndex->next = room_index_hash[iHash];
		room_index_hash[iHash] = pRoomIndex;
		pRoomIndex->next_room = room_list;
		room_list = pRoomIndex;
		top_room++;
	}
}

void load_newresets(FILE *fp)
{
	RESET_DATA *pReset;
	int iLastRoom = 0;
	int iLastObj = 0;

	if (area_last == nullptr)
	{
		RS.Logger.Error("Load_resets: no #AREA seen yet.");
		exit(1);
	}

	for (;;)
	{
		ROOM_INDEX_DATA *pRoomIndex = nullptr;
		char letter, tletter;
		OBJ_INDEX_DATA *temp_index;

		letter = fread_letter(fp);

		if (letter == 'S')
			break;

		if (letter == '*')
		{
			fread_to_eol(fp);
			continue;
		}

		pReset = new RESET_DATA;
		pReset->arg1 = 0;
		pReset->arg2 = 0;
		pReset->arg3 = 0;
		pReset->arg4 = 0;
		pReset->command = letter;
		fread_word(fp); // get rid of rest of word, i.e. OB for MOB
		/*
		 * Validate parameters.
		 * We're calling the index functions for the side effect.
		 */
		switch (letter)
		{
			case 'F':
				pReset->arg1 = fread_number(fp); // leader vnum
				pReset->arg2 = fread_number(fp); // leader mob number
				pReset->arg3 = 0;
				pReset->arg4 = 0;
				pRoomIndex = get_room_index(iLastRoom);
				new_reset(pRoomIndex, pReset);

				break;
			case 'M':
				pReset->arg1 = fread_number(fp); // mob vnum
				fread_word(fp);					 // the word TO
				pReset->arg3 = fread_number(fp); // to room vnum
				fread_word(fp);					 // the word GLIMIT
				pReset->arg2 = fread_number(fp); // global limit
				fread_word(fp);					 // the word LLIMIT
				pReset->arg4 = fread_number(fp); // local limit

				pRoomIndex = get_room_index(pReset->arg3);

				if (pRoomIndex)
				{
					new_reset(pRoomIndex, pReset);
					iLastRoom = pReset->arg3;
				}

				if (area_last->area_type == ARE_UNOPENED) // debugging for unopened areas
				{
					get_mob_index(pReset->arg1);
					get_room_index(pReset->arg3);
				}

				break;
			case 'O':
				pReset->arg1 = fread_number(fp); // object vnum
				pReset->arg2 = 0;				 // crappy placeholder
				fread_word(fp);					 // get rid of the word TO

				pReset->arg3 = fread_number(fp); // room vnum
				iLastRoom = pReset->arg3;
				pReset->arg4 = 0; // crappy placeholder
				temp_index = get_obj_index(pReset->arg1);
				temp_index->reset_num++;

				pRoomIndex = get_room_index(pReset->arg3);

				if (pRoomIndex)
				{
					new_reset(pRoomIndex, pReset);
					iLastObj = pReset->arg3;
				}

				if (area_last->area_type == ARE_UNOPENED)
					get_room_index(pReset->arg3);

				break;
			case 'P':
				pReset->arg1 = fread_number(fp); // object being put in container
				pReset->arg2 = -1;				 // crappy placeholder
				fread_word(fp);					 // disregard IN
				pReset->arg3 = fread_number(fp); // container vnum
				fread_word(fp);					 // disregard COUNT
				pReset->arg4 = fread_number(fp); // number of copies to put in cont.
				temp_index = get_obj_index(pReset->arg1);
				temp_index->reset_num++;

				pRoomIndex = get_room_index(iLastRoom);

				if (pRoomIndex)
					new_reset(pRoomIndex, pReset);

				if (area_last->area_type == ARE_UNOPENED)
					get_obj_index(pReset->arg3);

				break;
			case 'G':
				pReset->arg1 = fread_number(fp); // object to give
				pReset->arg2 = 0;
				pReset->arg3 = 0;
				pReset->arg4 = 0;
				temp_index = get_obj_index(pReset->arg1);
				temp_index->reset_num++;

				pRoomIndex = get_room_index(iLastRoom);

				if (pRoomIndex)
				{
					new_reset(pRoomIndex, pReset);
					iLastObj = iLastRoom;
				}

				break;
			case 'E':
				pReset->arg1 = fread_number(fp); // object to give
				pReset->arg2 = 0;
				pReset->arg3 = flag_lookup(fread_word(fp), wear_locations); // location of obj

				if (pReset->arg3 == NO_FLAG)
					bugout("Invalid wear location in resets.");

				pReset->arg4 = 0;
				temp_index = get_obj_index(pReset->arg1);
				temp_index->reset_num++;

				pRoomIndex = get_room_index(iLastRoom);
				if (pRoomIndex)
				{
					new_reset(pRoomIndex, pReset);
					iLastObj = iLastRoom;
				}

				break;
			case 'D':
				pReset->arg1 = fread_number(fp);							 // room vnum
				pReset->arg2 = flag_lookup(fread_word(fp), direction_table); // direction

				if (pReset->arg2 == NO_FLAG)
					bugout("Invalid direction in door reset.");

				tletter = fread_letter(fp);
				tletter = UPPER(tletter);

				if (tletter == 'O')
					pReset->arg3 = 0;
				else if (tletter == 'C')
					pReset->arg3 = 1;
				else if (tletter == 'L')
					pReset->arg3 = 2;
				else
					bugout("Invalid door reset state.");

				pReset->arg4 = 0;

				pRoomIndex = get_room_index(pReset->arg1);

				if (pRoomIndex)
					new_reset(pRoomIndex, pReset);

				break;
			case 'R':
				pReset->arg1 = fread_number(fp);
				pReset->arg2 = fread_number(fp);
				pReset->arg3 = 0;
				pReset->arg4 = 0;

				pRoomIndex = get_room_index(pReset->arg1);

				if (pRoomIndex)
					new_reset(pRoomIndex, pReset);

				if (pReset->arg2 < 0 || pReset->arg2 > 6)
				{
					RS.Logger.Error("Load_resets: 'R': bad exit {}.", pReset->arg2);
					exit(1);
				}

				break;
			default:
				RS.Logger.Error("Load_resets: bad command '{}'.", letter);
				exit(1);
				break;
		}

		fread_to_eol(fp);

		pReset->next = nullptr;
		top_reset++;
	}
}

void clean_notes(void)
{
	// if the note is over 14 days old, delete it.
	auto notes = NoteRepository(RS.DbRift);
	auto removed = notes.RemoveOlderThan(current_time - 1209600);

	if (removed > 0)
		RS.Logger.Info("Clean_notes: removed {} notes older than 14 days.", removed);
}

void load_race_info(void)
{
	int i, j;

	race_list = new_race_data();
	race_list->first = race_list;

	for (i = 0; race_table[i].name != nullptr; i++)
	{
		race_list->name = palloc_string(race_table[i].name);
		race_list->pc_race = race_table[i].pc_race;
		race_list->number = i;

		for (j = 0; j < 10; j++)
		{
			if (race_table[i].act[j] != NO_FLAG)
				SET_BIT(race_list->act, race_table[i].act[j]);
		}

		for (j = 0; j < 10; j++)
		{
			if (race_table[i].aff[j] != NO_FLAG)
				SET_BIT(race_list->aff, race_table[i].aff[j]);
		}

		for (j = 0; j < 10; j++)
		{
			if (race_table[i].off[j] != NO_FLAG)
				SET_BIT(race_list->off, race_table[i].off[j]);
		}

		for (j = 0; j < 10; j++)
		{
			if (race_table[i].imm[j] != NO_FLAG)
				SET_BIT(race_list->imm, race_table[i].imm[j]);
		}

		for (j = 0; j < 10; j++)
		{
			if (race_table[i].res[j] != NO_FLAG)
				SET_BIT(race_list->res, race_table[i].res[j]);
		}

		for (j = 0; j < 10; j++)
		{
			if (race_table[i].vuln[j] != NO_FLAG)
				SET_BIT(race_list->vuln, race_table[i].vuln[j]);
		}

		for (j = 0; j < 15; j++)
		{
			if (race_table[i].form[j] != NO_FLAG)
				SET_BIT(race_list->form, race_table[i].form[j]);
		}

		for (j = 0; j < 15; j++)
		{
			if (race_table[i].parts[j] != NO_FLAG)
				SET_BIT(race_list->parts, race_table[i].parts[j]);
		}

		race_list->arms = race_table[i].arms;
		race_list->legs = race_table[i].legs;
		race_list->next = new_race_data();
		race_list->next->first = race_list->first;
		race_list = race_list->next;
	}

	race_list->next = nullptr;
	race_list = race_list->first;
}

char intflag_to_char(int i)
{
	if (i >= 0 && i <= 25)
		return (char)((int)('A') + i);
	else
		return (char)((int)('a') + i - 26);
}

char *bitvector_to_flag(long bitvect[])
{
	int i, ilen = 0;
	static char buf[64];
	char *a = buf;

	if (IS_ZERO_VECTOR(bitvect))
		return ("0");

	for (i = 0; i < MAX_BIT; i++)
	{
		if (IS_SET(bitvect, i))
		{
			ilen++;
			*a++ = intflag_to_char(i);
		}
	}

	if (ilen == 0)
		*a++ = '0';

	*a = '\0';
	return buf;
}
