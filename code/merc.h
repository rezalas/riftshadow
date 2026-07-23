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
 *  ROM license, in the file Rom24/doc/rom.license                         *
 ***************************************************************************/

/***************************************************************************
 *       Tartarus code is copyright (C) 1997-1998 by Daniel Graham         *
 *  In using this code you agree to comply with the Tartarus license       *
 *       found in the file /Tartarus/doc/tartarus.doc                      *
 ***************************************************************************/

#ifndef MERC_H
#define MERC_H

//
// system calls
//

int remove();

#ifndef __cplusplus
	#include <stdbool.h>
#endif

#ifdef _WIN32
	#include <windows.h>
#endif

#ifndef RIFTINCLD
#define RIFTINCLD
	#include "mud.h"
#endif /* RIFTINCLD */

/*
 * OLC
 * Use these macros to load any new area formats that you choose to
 * support on your MUD.  See the new_load_area format below for
 * a short example.
 */
#ifndef KEY
#define KEY(literal, field, value)	\
	if (!str_cmp(word, literal))	\
	{								\
		field = value;				\
		fMatch = true;				\
		break;						\
	}
#endif

/* provided to free strings */
#ifndef KEYS
#define KEYS(literal, field, value)	\
	if (!str_cmp(word, literal))	\
	{								\
		free_pstring(field);		\
		field  = value;				\
		fMatch = true;				\
		break;						\
	}
#endif

#ifndef KEYV
#define KEYV(literal, field)		\
	if (!str_cmp(word, literal))	\
	{								\
		fread_flag_new(field, fp);	\
		fMatch = true;				\
		break;						\
	}
#endif

#ifndef SKEY
#define SKEY(string, field)			\
	if (!str_cmp(word, string))		\
	{								\
		free_pstring(field);		\
		field = fread_string(fp);	\
		fMatch = true;				\
		break;						\
	}
#endif


//
// Entity forward declarations, typedef aliases, and function types.
//

#include "entity/fwd.h"
#include "entity/limits.h"

//
// String and memory management parameters.
//

#define MAX_KEY_HASH				1024

#ifdef MAX_STRING_LENGTH
#undef MAX_STRING_LENGTH
#define MAX_STRING_LENGTH			4608
#else
#define MAX_STRING_LENGTH			4608
#endif

#define PAGELEN						20
#define MSL							MAX_STRING_LENGTH
#define VERSION						6
#define MAX_STORED_ITEMS			5

#ifndef RSINCLUDES
#define RSINCLUDES
#include <math.h>
#include <unistd.h>
#include <signal.h>
#include <stdarg.h>
#endif /* RSINCLUDES */

//char *crypt(const char *key, const char *salt);

#define SIGSEGV						11

//
// Game parameters.
// Increase the max'es if you add more of something.
// Adjust the pulse numbers to suit yourself.
//

#define MAX_SOCIALS					256
#define MAX_IN_GROUP				354
#define MAX_AREAS					500
#define MAX_CLASS					12
#define MAX_PC_RACE					13
#define MAX_CABAL					7
#define MAX_DAMAGE_MESSAGE			62
#define MAX_LEVEL					60
#define MAX_WEAPON					10
#define MAX_HOMETOWN				5
#define MAX_SITETRACK				5
#define MAX_ORDERS					9

#define START_LEVEL					1
#define LEVEL_HERO					(MAX_LEVEL - 9)
#define LEVEL_IMMORTAL				(MAX_LEVEL - 8)

#define CHARACTER_TIME_DEATH		20736000  //characters die 8 months after being rolled.  yar.
#define PULSE_PER_SECOND			4
#define PULSE_PER_MOVE				2
#define PULSE_VIOLENCE				(3 * PULSE_PER_SECOND)
#define PULSE_MOBILE				(4 * PULSE_PER_SECOND)
#define PULSE_TICK					(30 * PULSE_PER_SECOND)
#define PULSE_AREA					(60 * PULSE_PER_SECOND)
#define PULSE_TRACK					(15 * PULSE_PER_SECOND)
#define PULSE_RAFFECT				(3 * PULSE_PER_SECOND)
#define PULSE_IPROG_PULSE			PULSE_PER_SECOND
#define PULSE_WEATHER				(937 * PULSE_PER_SECOND)
#define PULSE_BERUS					(90 * PULSE_PER_SECOND)
#define PULSE_CALABREN				(150 * PULSE_PER_SECOND)

#define IMPLEMENTOR					MAX_LEVEL
#define	CREATOR						(MAX_LEVEL - 1)
#define SUPREME						(MAX_LEVEL - 2)
#define DEITY						(MAX_LEVEL - 3)
#define GOD							(MAX_LEVEL - 4)
#define IMMORTAL					(MAX_LEVEL - 5)
#define DEMI						(MAX_LEVEL - 6)
#define ANGEL						(MAX_LEVEL - 7)
#define AVATAR						(MAX_LEVEL - 8)
#define HERO						LEVEL_HERO

#define CON_DIE_BOUND				3

//
// COLOR
//

#define CLEAR						"[0m"		// Resets Color
#define C_RED						"[0;31m"	// Normal Colors
#define C_GREEN						"[0;32m"
#define C_YELLOW					"[0;33m"
#define C_BLUE						"[0;34m"
#define C_MAGENTA					"[0;35m"
#define C_CYAN						"[0;36m"
#define C_WHITE						"[0;37m"
#define C_D_GREY					"[1;30m"	// Light Colors
#define C_B_RED						"[1;31m"
#define C_B_GREEN					"[1;32m"
#define C_B_YELLOW					"[1;33m"
#define C_B_BLUE					"[1;34m"
#define C_B_MAGENTA					"[1;35m"
#define C_B_CYAN					"[1;36m"
#define C_B_WHITE					"[1;37m"
#define MAX_COLORS					16

//
// mprog stuff
//

#include "enums.h"  // scoped constant enums (phase-02 3.3)

//
// iprog stuff
//


//
// rprog stuff
//


//
// aprog stuff
//


//
// Quests!
//

#define DOLL_QUEST					1
#define TALISMANIC_QUEST			2
#define SCRIBE_QUEST				3
#define MUD_SCHOOL					4
#define PETE_QUEST					5
#define SMITH_QUEST					6
#define RING_QUEST					7


#define NOTE_UNSTARTED				0
#define NOTE_IN_PROGRESS			1

struct buf_type
{
	BUFFER *next;
	bool valid;
	short state;	// error state of the buffer
	short size;		// size in k
	char *string;	// buffer's string
};




#define RS_EPOCH					972370801


struct time_info_data
{
	bool half;
	int hour;
	int day;
	int month;
	int season;
	int year;
};


//
// Connected state for a channel.
//


//
// Descriptor (channel) structure.
//

struct improg_type
{
	char *type;
	char *name;
	void *function;
	char *owner;
};

#define MAX_ARG						6

#define FUN_GENERIC					0
#define FUN_ACT						1

typedef void FUNC 	  (void*, void*, void*, void*, void*, void*);
typedef bool RUNE_FUN (void*, void*, void*, void*);
typedef void RUNE_END (RUNE_DATA *rune);

struct rune_data
{
	RUNE_FUN *function;
	void *placed_on;
	CHAR_DATA *owner;
	int target_type;
	int trigger_type;
	int level;
	int duration;
	int type;
	int extra;
	int drawn_in;
	RUNE_DATA *next;
	RUNE_DATA *next_content;
	RUNE_END *end_fun;
};

struct queue_data
{
	int timer;
	int funtype;
	FUNC *function;
	void *arg[MAX_ARG];
	int act_to;
	QUEUE_DATA *next;
};

struct descriptor_data
{
	DESCRIPTOR_DATA *next;
	DESCRIPTOR_DATA *snoop_by;
	CHAR_DATA *character;
	CHAR_DATA *original;
	bool valid;
	char *host;
	short descriptor;
	short connected;
	bool fcommand;
	char inbuf[4 * MAX_INPUT_LENGTH];
	char incomm[MAX_INPUT_LENGTH];
	char inlast[MAX_INPUT_LENGTH];
	int repeat;
	char *outbuf;
	int outsize;
	int outtop;
	char *showstr_head;
	char *showstr_point;
	void *pEdit;	// OLC
	char **pString;	// OLC
	int editor;		// OLC
	short type;
};

struct bounty
{
	char *name;
	long bounty;
};

//
// TO types for act.
//


//
// Help table types.
//

struct help_data
{
	HELP_DATA *next;
	short level;
	char *keyword;
	char *text;
};


#include "entity/shop_data.h"




struct barred_data
{
	short type;			// Bar on basis of guild.. or?
	short comparison;	// less than greater than = to
	short value;		// value allowed in
	short vnum;			// Vnum it bars entry to
	short msg_type;		// echo/say/emote
	char *message;		// message when access is barred
	char *message_two;	// room message for echo type
};


//
// Per-class stuff.
//

#define MAX_GUILD					8
#define MAX_SPECS					8
#define MAX_SPEC_SKILLS				1
#define MAX_ZOMBIE					10
#define MAX_BEAUTY					10

#include "entity/spec_func.h"

struct class_type
{
	char *name;				// the full name of the class
	char who_name[4];		// Three-letter name for 'who'
	short attr_prime;		// Prime attribute/Prime Stat
	short align;			// see pc_race_type
	short xpadd;			// see pc_race_type
	short weapon;			// First weapon
	short skill_adept;		// Maximum skill level
	short thac0_00;			// Thac0 for level 0
	short thac0_32;			// Thac0 for level 32
	short hp_min;			// Min hp gained on leveling
	short hp_max;			// Max hp gained on leveling
	bool fMana;				// Class gains mana on level
	char *base_group;		// base skills gained
	char *default_group;	// default skills gained
	short ctype;			// communer/caster?
	short status;
};

struct race_data
{
	RACE_DATA *first;
	RACE_DATA *next;
	short number;
	char *name;
	bool pc_race;
	long act[MAX_BITVECTOR];
	long aff[MAX_BITVECTOR];
	long off[MAX_BITVECTOR];
	long imm[MAX_BITVECTOR];
	long res[MAX_BITVECTOR];
	long vuln[MAX_BITVECTOR];
	long form[MAX_BITVECTOR];
	long parts[MAX_BITVECTOR];
	int arms;
	int legs;
};

struct old_char
{
	char *name;
	char *short_descr;
	char *long_descr;
	char *description;
	short perm_stats[MAX_STATS];
	short armor[4];
	float dam_mod;
	short carry_weight;
	short carry_number;
	short saving_throw;
	long affected_by[MAX_BITVECTOR];
	long res_flags[MAX_BITVECTOR];
	long vuln_flags[MAX_BITVECTOR];
	long imm_flags[MAX_BITVECTOR];
	OLD_CHAR *next;
	OBJ_DATA *carrying;
};

#include "entity/affect_data.h"

//
// affect types
//


//
// class types
//


//
// open or closed
//


//
// open or closed  race
//

#define RACE_OPEN					1
#define RACE_CLOSED					0

//
// const.c skill table types
//


#define CAN_DISPEL					(1 << ASCII_A)
#define CAN_CANCEL					(1 << ASCII_B)
#define CAN_CLEANSE					(1 << ASCII_C)
#define CAN_PURIFY					(1 << ASCII_D)

//
// wear definitions
//


//
// runes only
//



//
// where definitions for room
//


//
// where definitions for objs
//


//
// where definitions for area
//

#define TO_AREA_AFFECTS				0

//
/// room applies
//


//
// obj applies
//


//
// area applies
//

#define	APPLY_AREA_NONE				0
#define	APPLY_AREA_TEMP				1
#define	APPLY_AREA_WIND				2
#define	APPLY_AREA_SKY				3

//
// Cabal definitions
//


#define MAX_EMPIRE					8
#define MAX_BOUNTY					5
#define CABAL_LEADER				5
#define MAX_TOP_BOUNTY				14
#define MAX_TOP_BOUNTY_SHOWN		9

//
// Tracking data
//

#define	MAX_TRACKS					4	// Max # of tracks recorded per room.

//
// A kill structure (indexed by level).
//
struct kill_data
{
	short number;
	short killed;
};



//*********************************************
//     VALUES OF INTEREST TO AREA BUILDERS
//      (Start of section ... start here)
//*********************************************

//
// Well known mob virtual numbers.
// Defined in #MOBILES.
//

#define MOB_VNUM_FIDO				3090
#define MOB_VNUM_CITYGUARD			3060
#define MOB_VNUM_VAMPIRE			3404

#define ACADEMY_PET					80

//
// skills.are mobs
//

#define MOB_VNUM_ZOMBIE				2901
#define MOB_VNUM_FIRE_ELEMENTAL		2902
#define MOB_VNUM_WATER_ELEMENTAL	2903
#define MOB_VNUM_AIR_ELEMENTAL		2904
#define MOB_VNUM_EARTH_ELEMENTAL	2905
#define MOB_VNUM_FALCON				2906
#define MOB_VNUM_WOLF				2907
#define MOB_VNUM_BEAR				2908
#define MOB_VNUM_LION				2909
#define MOB_VNUM_ENFORCER			2910
#define MOB_VNUM_L_GOLEM			2911
#define MOB_VNUM_UNICORN			2912
#define MOB_VNUM_DRAGON				2913
#define MOB_VNUM_SHADOW				2914
#define MOB_VNUM_G_GOLEM			2915
#define MOB_VNUM_DECOY				2916
#define MOB_VNUM_ARMOR				2917
#define MOB_VNUM_WEAPON				2918
#define MOB_VNUM_KEY				2919
#define MOB_VNUM_SLAYER				2920
#define MOB_VNUM_SKELETON			2921
#define MOB_VNUM_MUMMY				2922
#define MOB_VNUM_NIGHTFALL			2923
#define MOB_VNUM_SLAVE				2924
#define MOB_VNUM_STALKER			2925
#define MOB_VNUM_BARBAS				2963
#define MOB_VNUM_AAMON				2964
#define MOB_VNUM_MALAPHAR			2965
#define MOB_VNUM_FURCAS				2966
#define MOB_VNUM_IPOS				2967
#define MOB_VNUM_OZE				2968
#define MOB_VNUM_GAMYGYN			2969
#define MOB_VNUM_OROBAS				2970
#define MOB_VNUM_GERYON				2971
#define MOB_VNUM_CIMERIES			2972
#define MOB_VNUM_MEPHISTOPHELES		2973
#define MOB_VNUM_BAAL				2974
#define MOB_VNUM_MOLOCH				2975
#define MOB_VNUM_DISPATER			2976
#define MOB_VNUM_ASMODEUS			2977
#define MOB_VNUM_GEULGON_FAM		2978
#define MOB_VNUM_MINOTAUR_FAM		2979
#define MOB_VNUM_CORNOGUN_FAM		2980
#define MOB_VNUM_IMP_FAM			2981
#define MOB_VNUM_HAMATULA_FAM		2982
#define MOB_VNUM_PITFIEND_FAM		2983
#define MOB_VNUM_PROTECTOR			23600
#define MOB_VNUM_TEMPLAR			4505
#define MOB_VNUM_PATROLMAN			2106
#define MOB_VNUM_HYDRA 				13724
#define MOB_VNUM_BONES				13725
#define MOB_VNUM_DRANNOR_LADY		13726
#define MOB_VNUM_DRANNOR_KNIGHT		13730
#define MOB_VNUM_PLRMOB				1201
#define MOB_VNUM_CENTURION			8102
#define MOB_VNUM_WARRIOR_MERCENARY	2930
#define MOB_VNUM_THIEF_MERCENARY	2931
#define MOB_VNUM_ASSASSIN_MERCENARY	2932
#define MOB_VNUM_SHAMAN_MERCENARY	2933
#define MOB_VNUM_LESSER_DEMON		2935
#define MOB_VNUM_GREATER_DEMON		2936
#define MOB_VNUM_LESSER_DEVIL		2937
#define MOB_VNUM_ANCHOR				2958

#define GROUP_VNUM_TROLLS			2100
#define GROUP_VNUM_OGRES			2101

//
// RT ASCII conversions -- used so we can have letters in this file
//

#define ASCII_A							0
#define ASCII_B							1
#define ASCII_C							2
#define ASCII_D							3
#define ASCII_E							4
#define ASCII_F							5
#define ASCII_G							6
#define ASCII_H							7
#define ASCII_I							8
#define ASCII_J							9
#define ASCII_K							10
#define ASCII_L							11
#define ASCII_M							12
#define ASCII_N							13
#define ASCII_O							14
#define ASCII_P							15
#define ASCII_Q							16
#define ASCII_R							17
#define ASCII_S							18
#define ASCII_T							19
#define ASCII_U							20
#define ASCII_V							21
#define ASCII_W							22
#define ASCII_X							23
#define ASCII_Y							24
#define ASCII_Z							25
#define ASCII_aa						26
#define ASCII_bb						27
#define ASCII_cc						28
#define ASCII_dd						29
#define ASCII_ee						30
#define ASCII_ff						31
#define ASCII_gg						32
#define ASCII_hh						33
#define ASCII_ii						34
#define ASCII_jj						35
#define ASCII_kk						36
#define ASCII_ll						37
#define ASCII_mm						38
#define ASCII_nn						39
#define ASCII_oo						40
#define ASCII_pp						41
#define MAX_BIT							42

//
// ACT bits for mobs.
// Used in #MOBILES.
//


//
// damage classes
//

#define	DAM_INTERNAL				20

//
// OFF bits for mobiles
//


//
// return values for check_imm
//


//
// IMM bits for mobs
//


//
// RES bits for mobs
//

 
//
// VULN bits for mobs
//


//
// body form
//


//
// body parts
//


//
// Bits for 'affected_by'.
// Used in #MOBILES.
//


//
// AFF bits for rooms
//


// Aff bits for.. AREAS!
// -- None currently. --
//

//
// Aff bits for OBJS
//


//
// Sex.
// Used in #MOBILES.
//


//
// AC types
//

#define MAX_AC						4

//
// dice
//


//
// size
//


//
// Well known object virtual numbers.
// Defined in #OBJECTS.
//

#define OBJ_VNUM_GOLD_ONE			2
#define OBJ_VNUM_GOLD_SOME			3
#define OBJ_VNUM_COINS				5
#define OBJ_VNUM_SACK				2901
#define OBJ_VNUM_STEAK				2902
#define OBJ_VNUM_POTION				2903
#define OBJ_VNUM_CRYSTAL			2904
#define OBJ_VNUM_CHESSBOARD			23713
#define OBJ_VNUM_PROTECTORATE		2912
#define OBJ_VNUM_BADGE				2913
#define OBJ_VNUM_SKELETON			2925
#define OBJ_VNUM_URN				2930
#define OBJ_VNUM_CHAOS_BLADE		9903
#define OBJ_VNUM_RUSTY_IRON			2931
#define OBJ_VNUM_PUDDLE				2936
#define OBJ_VNUM_SCRIBE				23653
#define OBJ_VNUM_CORPSE_NPC			10
#define OBJ_VNUM_CORPSE_PC			11
#define OBJ_VNUM_SEVERED_HEAD		12
#define OBJ_VNUM_TORN_HEART			13
#define OBJ_VNUM_SLICED_ARM			14
#define OBJ_VNUM_SLICED_LEG			15
#define OBJ_VNUM_GUTS				16
#define OBJ_VNUM_BRAINS				17
#define OBJ_VNUM_MUSHROOM			20
#define OBJ_VNUM_LIGHT_BALL			21
#define OBJ_VNUM_SPRING				22
#define OBJ_VNUM_DISC				23
#define OBJ_VNUM_PORTAL				25
#define OBJ_VNUM_ROSE				1001
#define OBJ_VNUM_ARENA_TROPHY		26010
#define OBJ_VNUM_FORGE				2905
#define OBJ_VNUM_HORDE_TROPHY		23640
#define OBJ_VNUM_PIT				2796

#define PIT_NEWTHALOS				9603
#define PIT_OFCOL					671
#define PIT_ARKHAM					765
#define PIT_DEFAULT					3010

#define OBJ_VNUM_SCHOOL_MACE		24586
#define OBJ_VNUM_SCHOOL_DAGGER		24588
#define OBJ_VNUM_SCHOOL_SWORD		24589
#define OBJ_VNUM_SCHOOL_SPEAR		24583
#define OBJ_VNUM_SCHOOL_STAFF		24582
#define OBJ_VNUM_SCHOOL_AXE			24585
#define OBJ_VNUM_SCHOOL_FLAIL		24581
#define OBJ_VNUM_SCHOOL_WHIP		24587
#define OBJ_VNUM_SCHOOL_POLEARM		24584
#define OBJ_VNUM_SCHOOL_VEST		3703
#define OBJ_VNUM_SCHOOL_SHIELD		24580
#define OBJ_VNUM_SCHOOL_BANNER		3704
#define OBJ_VNUM_MAP				3162
#define OBJ_VNUM_MAP_THERA			3163
#define OBJ_VNUM_WHISTLE			2116
#define OBJ_VNUM_RING_EMPOWER 		3810
#define OBJ_VNUM_ARCPOTION			2920
#define OBJ_VNUM_ARCSCROLL			2921
#define OBJ_VNUM_ARCWAND			2922
#define OBJ_VNUM_ARCSTAFF			2923
#define OBJ_VNUM_BERRY				2924
#define OBJ_VNUM_HAIR_BRACELET		13725
#define OBJ_VNUM_BLOOD_BRACELET		13720
#define OBJ_VNUM_TIARA				23231
#define OBJ_VNUM_WIZARDRY_1			23236
#define OBJ_VNUM_WIZARDRY_2			23238
#define OBJ_VNUM_SWORD_PLANES		13714
#define OBJ_VNUM_HEAVENLY_SCEPTRE	23263
#define OBJ_VNUM_MACE_DISRUPTION	23261
#define OBJ_VNUM_TORQUE				13715
#define OBJ_VNUM_ROD_LORDLY			13717
#define OBJ_VNUM_CROWN_BLOOD		13716
#define OBJ_VNUM_RED_IOUN			1
#define OBJ_VNUM_BLUE_IOUN			1
#define OBJ_VNUM_GREEN_IOUN			1
#define OBJ_VNUM_WINGS				13711
#define OBJ_VNUM_HYDRA_SLAYER		13738
#define OBJ_VNUM_HYDRA_HEAD			13737
#define OBJ_VNUM_BLACK_KEY			13736
#define OBJ_VNUM_HORN_VALERE		13739
#define OBJ_VNUM_EXCALIBUR_KEY		23151
#define OBJ_VNUM_TROPHY_BELT		23645

//
// TATTOOS
//

#define OBJ_VNUM_TATTOO_ASS			49
#define OBJ_VNUM_TATTOO_DIO			50
#define OBJ_VNUM_TATTOO_JHYR		51
#define OBJ_VNUM_TATTOO_XANTH		52
#define OBJ_VNUM_TATTOO_SERA		53
#define OBJ_VNUM_TATTOO_DEV			54
#define OBJ_VNUM_TATTOO_BUT			55
#define OBJ_VNUM_TATTOO_DAE			56
#define OBJ_VNUM_TATTOO_DET			57
#define OBJ_VNUM_TATTOO_IOM			58
#define OBJ_VNUM_TATTOO_LES			59
#define OBJ_VNUM_TATTOO_KYR			60
#define OBJ_VNUM_TATTOO_GAB			61
#define OBJ_VNUM_TATTOO_LIC			63
#define OBJ_VNUM_TATTOO_DEO			64

#define OBJ_VNUM_DRAGONSWORD		4503
#define OBJ_VNUM_DRAGONPLATE		4506
#define OBJ_VNUM_EMPERORTHRONE		8102

#define OBJ_EXPLOSIVES				29

//
// Item types.
// Used in #OBJECTS.
//


//
// Extra flags.
// Used in #OBJECTS.
///


//
// Wear flags.
// Used in #OBJECTS.
//





#define MAX_STYLE					7
#define MAX_STYLE_SKILL				38

//
// weapon class
//


//
// weapon types
//


//
// gate flags
//


//
// furniture flags
//

	
//
// Apply types (for affects).
// Used in #OBJECTS.
//


// APPLY_OBJ_PROPERTIES (== 100) moved into enum ApplyLocation above -- it was
// always numbered in that family despite its name.

//
// Modifier Names
//


//
// Values for containers (value[1]).
// Used in #OBJECTS.
//



#define HAS_DIED					8

#define AD_COND_NIGHT				0

//
// Well known room virtual numbers.
// Defined in #ROOMS.
//

#define ROOM_VNUM_DEATH_GOOD		1212
#define ROOM_VNUM_DEATH_NEUTRAL		1212
#define ROOM_VNUM_DEATH_EVIL		1212

//
// These are the cabal rooms where if you put another Cabal's item down
// that cabal will lose it's powers. Tailor these for you cabals then make
// cabal items for the cabal (these are made by setting the object with the
// cabal item setting).
//

#define ROOM_VNUM_NIGHTWALK			2900
#define ROOM_VNUM_LIMBO				2
#define ROOM_VNUM_CHAT				1200
#define ROOM_VNUM_TEMPLE			2538
#define ROOM_VNUM_ALTAR				2538
#define ROOM_VNUM_SCHOOL			3700
#define ROOM_VNUM_START				3758
#define ROOM_VNUM_VENUEPORT			9302

//
// Set HOMETOWNS as the room in which you want the 'hometown' command to
// function. This will be the only room in the game where players can change
// their hometown after choosing it at creation.
//
//#define ROOM_VNUM_HOMETOWNS			3008

//
// Room flags.
// Used in #ROOMS.
//


//
// Exit flags.
// Used in #ROOMS.
//



//
// Sector types.
// Used in #ROOMS.
//


//
// Trap types
//

#define MAX_TRAP					11

#define ALIGN_NEUTRAL				0
#define ETHOS_NEUTRAL				0

//
// Alignment selections
//


//
// Ethos selections
//



//
// Class guild used in the room 'G'  flags
//




//
// Equipment wear locations.
// Used in #RESETS.
//

#define MAX_WEAR					22


#define BLOCK_YES					1
#define BLOCK_NO					0

#define NORM						1







//*********************************************
//     VALUES OF INTEREST TO AREA BUILDERS
//     (End of this section ... stop here)
//*********************************************

//
// Conditions.
//


//
// Positions.
//


#define MIN_PK_XP					9999	// min xp players need to start PKing
#define MIN_LEVEL_TO_PK				23		// Minimum level for players to pk



//
// ACT bits for players.
//


//
// RT comm flags -- may be used on both mobs and chars
//


//
// Trust flags
//

#define TRUST_GROUP					0
#define TRUST_CABAL					1

//
// WIZnet flags
//


//
// AP Demonic Favors Defines
//






#define GERYON_EYE					2
#define GERYON_FINGER				3
#define CIMERIES_EAR				2
#define CIMERIES_NOSE				3

//
// Sorcerer Elemental Groups
//

#define MAX_ELE						19




#include "entity/mob_index_data.h"

//
// Speech info for progs
//


struct speech_data
{
	MOB_INDEX_DATA *mob;
	SPEECH_DATA *next;
	SPEECH_DATA *prev;
	char *name;
	LINE_DATA *first_line;
	LINE_DATA *current_line;
};

struct line_data
{
	SPEECH_DATA *speech;
	LINE_DATA *next;
	LINE_DATA *prev;
	int number;
	int delay;
	short type;
	char *text;
};
	
//
// Color codes
//

#define ANSI_NORMAL					"\x01B[m"
#define ANSI_BOLD					"\x01B[1m"
#define ANSI_BLINK					"\x01B[5m"
#define ANSI_REVERSE				"\x01B[7m"

//
// memory for mobs
//

struct mem_data
{
	MEM_DATA *next;
	bool valid;
	int id; 
	int reaction;
	time_t when;
};


struct mprog_data
{
	MPROG_FUN_BRIBE *bribe_prog;
	MPROG_FUN_ENTRY *entry_prog;
	MPROG_FUN_GIVE *give_prog;
	MPROG_FUN_GREET *greet_prog;
	MPROG_FUN_FIGHT *fight_prog;
	MPROG_FUN_DEATH *death_prog;
	MPROG_FUN_PULSE *pulse_prog;
	MPROG_FUN_BEAT *beat_prog;
	MPROG_FUN_SPEECH *speech_prog;
	MPROG_FUN_ATTACK *attack_prog;
	MPROG_FUN_AGGRESS *aggress_prog;
	MPROG_FUN_MOVE *move_prog;
	char *attack_name;
	char *aggress_name;
	char *bribe_name;
	char *entry_name;
	char *give_name;
	char *greet_name;
	char *fight_name;
	char *death_name;
	char *pulse_name;
	char *beat_name;
	char *speech_name;
	char *move_name;
};

struct iprog_data
{
	IPROG_FUN_WEAR *wear_prog;
	IPROG_FUN_REMOVE *remove_prog;
	IPROG_FUN_GET *get_prog;
	IPROG_FUN_LOOT *loot_prog;
	IPROG_FUN_DROP *drop_prog;
	IPROG_FUN_SAC *sac_prog;
	IPROG_FUN_ENTRY *entry_prog;
	IPROG_FUN_GIVE *give_prog;
	IPROG_FUN_GREET *greet_prog;
	IPROG_FUN_FIGHT *fight_prog;
	IPROG_FUN_DEATH *death_prog;
	IPROG_FUN_SPEECH *speech_prog;
	IPROG_FUN_PULSE *pulse_prog;
	IPROG_FUN_INVOKE *invoke_prog;
	IPROG_FUN_OPEN *open_prog;
	IPROG_FUN_LOOK *look_prog;
	IPROG_FUN_VERB *verb_prog;
	IPROG_FUN_HIT *hit_prog;
	char *wear_name;
	char *remove_name;
	char *get_name;
	char *loot_name;
	char *drop_name;
	char *sac_name;
	char *entry_name;
	char *give_name;
	char *greet_name;
	char *fight_name;
	char *death_name;
	char *speech_name;
	char *pulse_name;
	char *invoke_name;
	char *open_name;
	char *look_name;
	char *verb_name;
	char *hit_name;
};

struct rprog_data
{
	RPROG_FUN_PULSE *pulse_prog;
	RPROG_FUN_ENTRY *entry_prog;
	RPROG_FUN_MOVE *move_prog;
	RPROG_FUN_DROP *drop_prog;
	RPROG_FUN_SPEECH *speech_prog;
	RPROG_FUN_OPEN *open_prog;
	char *pulse_name;
	char *entry_name;
	char *move_name;
	char *drop_name;
	char *speech_name;
	char *open_name;
};

struct aprog_data
{
	APROG_FUN_PULSE *pulse_prog;
	APROG_FUN_RESET *reset_prog;
	APROG_FUN_SUN *sun_prog;
	APROG_FUN_TICK *tick_prog;
	APROG_FUN_AGGRESS *aggress_prog;
	APROG_FUN_MYELL	*myell_prog;
	char *pulse_name;
	char *reset_name;
	char *sun_name;
	char *tick_name;
	char *aggress_name;
	char *myell_name;
};

#include "entity/pc_data.h"

//
// Data for generating characters -- only used during generation
//

struct gen_data
{
	GEN_DATA *next;
	bool valid;
	bool skill_chosen[MAX_SKILL];
	bool group_chosen[MAX_GROUP];
	int points_chosen;
};

#include "entity/extra_descr.h"


#include "entity/obj_index_data.h"

//
// One object.
//

struct obj_data
{
	OBJ_DATA *next;
	OBJ_DATA *next_content;
	OBJ_DATA *contains;
	OBJ_DATA *in_obj;
	OBJ_DATA *on;
	CHAR_DATA *carried_by;
	EXTRA_DESCR_DATA *extra_descr;
	OBJ_APPLY_DATA *apply;
	OBJ_AFFECT_DATA *affected;
	AFFECT_DATA *charaffs;
	OBJ_INDEX_DATA *pIndexData;
	ROOM_INDEX_DATA *in_room;
	RUNE_DATA *rune;
	bool has_rune;
	bool valid;
	char *talked;
	char *owner;
	char *name;
	char *short_descr;
	char *description;
	short item_type;
	long extra_flags[MAX_BITVECTOR];
	long wear_flags[MAX_BITVECTOR];
	long imm_flags[MAX_BITVECTOR];
	long res_flags[MAX_BITVECTOR];
	long vuln_flags[MAX_BITVECTOR];
	long affected_by[MAX_BITVECTOR];
	short wear_loc;
	short weight;
	int cost;
	short level;
	short condition;
	char *material;
	short timer;
	int value[5];
	long progtypes[MAX_BITVECTOR];
	int ohp;
	int mob_vnum;
	bool moved;
	char *wear_loc_name;
};


#include "entity/exit_data.h"



#include "entity/reset_data.h"


#include "entity/area_data.h"


#include "entity/room_index_data.h"

struct trap_data
{
	ROOM_INDEX_DATA *room;				// Where is it?
	int type;							// What kind of trap is it?
	int quality;						// How badass is it?
	int complexity;						// How tough is it to disarm?
	char *trig_echo;					// Activation echo.
	char *exec_echo;					// Echo when the trap executes.
	int timer;							// Time before activation.
	bool armed;							// Is it armed?
};
	
	
	

//
// Solution to Horde trophy ugliness
//

struct trophy_data
{
	char *victname;
	TROPHY_DATA *next;
};

//
// Data stored in rooms to facilitate PC and mob tracking
//

struct track_data
{
	CHAR_DATA *prey;					// Who passed through.
	TIME_INFO_DATA time;				// When they passed through.
	short direction;					// Which way they went.
	bool flying;						// Are they?
	bool sneaking;						// Are they?
	int legs;							// How many?
	bool bleeding;						// Are they?
};

struct pathfind_data
{
	ROOM_INDEX_DATA *room;				// What room is this?
	bool evaluated;						// Has path_find() evaluated it yet?
	int dir_from;						// What direction did we go in the last node?
	int steps;							// Number of steps from parent node
	PATHFIND_DATA *prev;				// Room we came from
	PATHFIND_DATA *dir_to[6];			// Points to up to 6 rooms, NEWSUD.
};

struct note_data;
typedef struct note_data NOTE_DATA;

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

//
// Types of attacks.
// Must be non-overlapping with spell/skill types,
// but may be arbitrary beyond that.
//

#define TYPE_NOCOMBAT				-2
#define TYPE_UNDEFINED				-1
#define TYPE_HIT					1000
#define TYPE_TRUESTRIKE				-3


//
//  Target types.
//



//
//rune target bitvectors
//



//
// Utility macros.
//

#define URANGE(a, b, c)				(b < a ? a : (b > c ? c : b))
#define LOWER(c)					(c >= 'A' && c <= 'Z' ? c + 'a' - 'A' : c)
#define UPPER(c)					(c >= 'a' && c <= 'z' ? c + 'A' - 'a' : c)
// Array flags -- operate on a long[MAX_BITVECTOR]; the bit is split into a word
// index and an offset. `act`, `affected_by`, `bitvector` and friends.
#define IS_SET(flag, bit)			((flag)[(bit) / 32] &   (1L << ((bit) % 32)))
#define SET_BIT(var, bit)			((var)[(bit) / 32]  |=  (1L << ((bit) % 32)))
#define REMOVE_BIT(var, bit)		((var)[(bit) / 32]  &= ~(1L << ((bit) % 32)))

// Scalar flags -- operate on a single integer field, overwhelmingly obj->value[N]
// (the furniture flags in act_info.c among them). The bit is an absolute shift,
// with no word split. The _OLD suffix names the representation, NOT deprecation:
// these three have 134 live call sites (140 counting TOGGLE_BIT_OLD below).
#define IS_SET_OLD(flag, bit) 		((flag) &   (1L << (bit)))
#define SET_BIT_OLD(var, bit) 		((var)  |=  (1L << (bit)))
#define REMOVE_BIT_OLD(var,bit) 	((var)  &= ~(1L << (bit)))
#define TOGGLE_BIT(var, bit)		(IS_SET(var,bit) ? REMOVE_BIT(var,bit) : SET_BIT(var,bit))
#define TOGGLE_BIT_OLD(var, bit)	(IS_SET_OLD(var,bit) ? REMOVE_BIT_OLD(var,bit) : SET_BIT_OLD(var,bit))


//TODO: Find out where these are implemented
extern const struct class_type class_table[MAX_CLASS];
extern QUEUE_DATA *global_queue;

#define MULT_EXP					5

//**********************************************
//						OLC
// *********************************************
//
// Object defined in limbo.are
// Used in save.c to load objects that don't exist.
//
//

#define OBJ_VNUM_DUMMY				70

//
// Area flags.
//


#define MAX_DIR						6
#define NO_FLAG						-99		// Must not be used in flags or stats.


#define CAP(st)						(*(st) = UPPER(*(st)), st)
#define	HIGH_VNUM					65535
#define LOW_VNUM					-1


#define CLEAR_MEM(stru, x)		for(unsigned int clearmem = 0; clearmem < x; clearmem++) *((char *)stru + clearmem) = '\0';

#endif /* MERC_H */
