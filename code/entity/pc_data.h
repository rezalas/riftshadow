#ifndef ENTITY_PC_DATA_H
#define ENTITY_PC_DATA_H

#include <time.h>

#include "fwd.h"
#include "limits.h"
#include "../enums.h"		// SECT_MAX
#include "../prof.h"		// CProficiencies is embedded by value

//
// Data which only PC's have.
//

struct pc_data
{
	PC_DATA *next;
	BUFFER *buffer;
	bool valid;
	bool newbie;
	char *pwd;
	char *bamfin;
	char *bamfout;
	char *title;
	char *extitle;
	char *imm_death;
	time_t last_note;
	time_t last_idea;
	time_t last_penalty;
	time_t last_news;
	time_t last_changes;
	short perm_hit;
	short perm_mana;
	short perm_move;
	short true_sex;
	short age_mod;
	short death_timer;
	short death_status;
	short ethos;
	short oalign;
	short oethos;
	short sp;
	short special;
	short wanteds;
	short death_count;
	short induct;
	short start_weapon;
	short killed[2]; 			// 0 = PK_KILLED, 1 = MOB_KILLED
	short kills[4];
	float frags[4];
	float fragged;
	char *recentkills[100];
	short old_room;
	int last_level;
	short beauty;
	short reputation;
	short condition[6];
	short learned[MAX_SKILL];
	bool group_known[MAX_GROUP];
	bool confirm_delete;
	char *alias[MAX_ALIAS];
	char *alias_sub[MAX_ALIAS];

	//
	// bounties - dev
	//

	long bounty;
	short bounty_credits;

	short helpid;
	char *temp_history_buffer;
	char *history_buffer;
	char *role;
	char *temp_role;
	char *logon_time;
	char *color_scheme[MAX_EVENTS];
	long shifted;
	OLD_CHAR *old;
	bool entering_text;
	char *entered_text;
	DO_FUN *end_fun;
	long trust[MAX_BITVECTOR];
	CHAR_DATA *trusting;
	short energy_state;
	char *host;
	short quests[MAX_QUESTS];
	time_t roll_time;
	short birth_date;
	short death_time;
	short tribe;
	long sect_time[SECT_MAX];
	short save_timer;
	short style;
	long styles[MAX_BITVECTOR];
	long ele_sphere[MAX_BITVECTOR];
	TROPHY_DATA *trophy;
	char *command[2];
	
	//
	// Sorcerer elements data
	//

	short ele_temp[4];					// used only for creation
	short ele_major;
	short ele_para;

	//
	// AP devil favors data
	//

	int devildata[MAX_DEVIL];
	int lesserdata[MAX_LESSER];
	int greaterdata[MAX_GREATER];

	int editing_item;
	int security;						// OLC - Builder security
	short souls;
	int cabal_level;
	int bounty_killed;
 	char *temp_str;
	short deposited_items[5];
	short paladin_path;
	short temp_quest;

	//
	// CMD_QUEUE data
	//

	char queue[MAX_QUEUE][MAX_INPUT_LENGTH];
	short read_next;
	short write_next;
	bool pending;

	//
	// proficiency data
	//

	CProficiencies	profs;
};

#endif /* ENTITY_PC_DATA_H */
