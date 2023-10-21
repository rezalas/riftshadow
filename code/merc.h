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
// Structure types.
//

typedef struct affect_data		AFFECT_DATA;
typedef struct area_data		AREA_DATA;
typedef struct buf_type			BUFFER;
typedef struct char_data		CHAR_DATA;
typedef struct descriptor_data	DESCRIPTOR_DATA;
typedef struct exit_data		EXIT_DATA;
typedef struct extra_descr_data	EXTRA_DESCR_DATA;
typedef struct help_data		HELP_DATA;
typedef struct kill_data		KILL_DATA;
typedef struct mem_data			MEM_DATA;
typedef struct mob_index_data	MOB_INDEX_DATA;
typedef struct speech_data		SPEECH_DATA;
typedef struct line_data		LINE_DATA;
typedef struct obj_data			OBJ_DATA;
typedef struct obj_index_data	OBJ_INDEX_DATA;
typedef struct pc_data			PC_DATA;
typedef struct gen_data			GEN_DATA;
typedef struct reset_data		RESET_DATA;
typedef struct room_index_data	ROOM_INDEX_DATA;
typedef struct trap_data		TRAP_DATA;
typedef struct shop_data		SHOP_DATA;
typedef struct time_info_data	TIME_INFO_DATA;
typedef struct iprog_data		IPROG_DATA;
typedef struct mprog_data		MPROG_DATA;
typedef struct rprog_data		RPROG_DATA;
typedef struct aprog_data		APROG_DATA;
typedef struct bounty			BOUNTY_DATA;
typedef struct old_char			OLD_CHAR;
typedef struct barred_data		BARRED_DATA;
typedef struct room_affect_data	ROOM_AFFECT_DATA;
typedef struct area_affect_data	AREA_AFFECT_DATA;
typedef struct obj_affect_data	OBJ_AFFECT_DATA;
typedef struct obj_apply_data	OBJ_APPLY_DATA;
typedef struct queue_data		QUEUE_DATA;
typedef struct rune_data		RUNE_DATA;
typedef struct trophy_data		TROPHY_DATA;
typedef struct track_data		TRACK_DATA;
typedef struct pathfind_data	PATHFIND_DATA;
typedef struct race_data		RACE_DATA;
typedef struct special_prog		SPECIAL_PROG;

//
// Function types.
//

typedef void DO_FUN (CHAR_DATA *ch, char *argument);
typedef void SPELL_FUN (int sn, int level, CHAR_DATA *ch, void *vo, int target);
typedef void GAME_FUN (CHAR_DATA *ch, CHAR_DATA *croupier, char *argument);
typedef void OAFF_FUN (OBJ_DATA *obj, OBJ_AFFECT_DATA *af);
typedef void AAFF_FUN (AREA_DATA *area, AREA_AFFECT_DATA *af);
typedef void RAFF_FUN (ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *af);
typedef void AFF_FUN (CHAR_DATA *ch, AFFECT_DATA *af);

typedef bool MPROG_FUN_AGGRESS (CHAR_DATA *mob, CHAR_DATA *attacker);
typedef void MPROG_FUN_ATTACK (CHAR_DATA *mob, CHAR_DATA *attacker);
typedef void MPROG_FUN_BRIBE (CHAR_DATA *mob, CHAR_DATA *ch, int amount);
typedef void MPROG_FUN_ENTRY (CHAR_DATA *mob);
typedef void MPROG_FUN_GREET (CHAR_DATA *mob, CHAR_DATA *ch);
typedef void MPROG_FUN_GIVE (CHAR_DATA *mob, CHAR_DATA *ch, OBJ_DATA *obj);
typedef void MPROG_FUN_FIGHT (CHAR_DATA *mob, CHAR_DATA *victim);
typedef bool MPROG_FUN_DEATH (CHAR_DATA *mob, CHAR_DATA *killer);
typedef void MPROG_FUN_BEAT  (CHAR_DATA *mob);
typedef void MPROG_FUN_PULSE (CHAR_DATA *mob);
typedef void MPROG_FUN_SPEECH (CHAR_DATA *mob, CHAR_DATA *ch, char *speech);
typedef bool MPROG_FUN_MOVE (CHAR_DATA *ch, CHAR_DATA *mob, ROOM_INDEX_DATA *from, int direction);

typedef void IPROG_FUN_WEAR (OBJ_DATA *obj, CHAR_DATA *ch);
typedef void IPROG_FUN_REMOVE (OBJ_DATA *obj, CHAR_DATA *ch);
typedef void IPROG_FUN_GET (OBJ_DATA *obj, CHAR_DATA *ch);
typedef bool IPROG_FUN_LOOT (OBJ_DATA *container, OBJ_DATA *obj, CHAR_DATA *ch);
typedef void IPROG_FUN_DROP (OBJ_DATA *obj, CHAR_DATA *ch);
typedef bool IPROG_FUN_SAC (OBJ_DATA *obj, CHAR_DATA *ch);
typedef void IPROG_FUN_ENTRY (OBJ_DATA *obj);
typedef bool IPROG_FUN_GIVE (OBJ_DATA *obj, CHAR_DATA *from, CHAR_DATA *to);
typedef void IPROG_FUN_GREET (OBJ_DATA *obj, CHAR_DATA *ch);
typedef void IPROG_FUN_FIGHT (OBJ_DATA *obj, CHAR_DATA *ch);
typedef bool IPROG_FUN_DEATH (OBJ_DATA *obj, CHAR_DATA *ch);
typedef void IPROG_FUN_SPEECH (OBJ_DATA *obj, CHAR_DATA *ch, char *speech);
typedef void IPROG_FUN_PULSE (OBJ_DATA *obj, bool isTick);
typedef void IPROG_FUN_INVOKE (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);
typedef bool IPROG_FUN_OPEN (OBJ_DATA *obj, CHAR_DATA *ch);
typedef void IPROG_FUN_LOOK (OBJ_DATA *obj, CHAR_DATA *ch);
typedef void IPROG_FUN_VERB (OBJ_DATA *obj, CHAR_DATA *ch, char *argument);
typedef void IPROG_FUN_HIT (OBJ_DATA *obj, CHAR_DATA *ch, CHAR_DATA *victim, int damage);

typedef void RPROG_FUN_PULSE (ROOM_INDEX_DATA *room);
typedef void RPROG_FUN_ENTRY (ROOM_INDEX_DATA *room, CHAR_DATA *ch);
typedef bool RPROG_FUN_MOVE (ROOM_INDEX_DATA *room, CHAR_DATA *ch, int dir);
typedef void RPROG_FUN_DROP (ROOM_INDEX_DATA *room, CHAR_DATA *ch, OBJ_DATA *obj);
typedef void RPROG_FUN_SPEECH (ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *speech);
typedef bool RPROG_FUN_OPEN (ROOM_INDEX_DATA *room, CHAR_DATA *ch, EXIT_DATA *exit);

typedef void APROG_FUN_PULSE (AREA_DATA *area);
typedef void APROG_FUN_RESET (AREA_DATA *area);
typedef void APROG_FUN_SUN (AREA_DATA *area);
typedef void APROG_FUN_TICK (AREA_DATA *area);
typedef bool APROG_FUN_AGGRESS (AREA_DATA *area, CHAR_DATA *ch, CHAR_DATA *victim);
typedef void APROG_FUN_MYELL (AREA_DATA *area, CHAR_DATA *ch, CHAR_DATA *victim);



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

#define MAX_INPUT_LENGTH			4608
#define PAGELEN						20
#define MSL							MAX_STRING_LENGTH
#define VERSION						6
#define MAX_MOB_AFFECT				10
#define MAX_MOB_CAST				10
#define MAX_STORED_ITEMS			5

#ifndef RSINCLUDES
#define RSINCLUDES
#include <math.h>
#include <mysql.h>
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
#define MAX_SKILL					800
#define MAX_GROUP					50
#define MAX_IN_GROUP				354
#define MAX_AREAS					500
#define MAX_ADJACENT				25
#define MAX_ALIAS					10
#define MAX_CLASS					12
#define MAX_PC_RACE					13
#define MAX_CABAL					7
#define MAX_DAMAGE_MESSAGE			62
#define MAX_LEVEL					60
#define MAX_WEAPON					10
#define MAX_HOMETOWN				5
#define MAX_SITETRACK				5
#define MAX_QUESTS					8
#define MAX_ORDERS					9
#define MAX_BITVECTOR				2
#define MAX_PROFS_TAUGHT_BY_MOB		4

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
#define MAX_EVENTS					13

//
// mprog stuff
//

#define MPROG_BRIBE					0
#define MPROG_ENTRY					1
#define MPROG_GREET					2
#define MPROG_GIVE					3
#define MPROG_FIGHT					4
#define MPROG_DEATH					5
#define MPROG_PULSE					6
#define MPROG_SPEECH				7
#define MPROG_ATTACK				8
#define MPROG_MOVE					9
#define MPROG_BEAT					10
#define MPROG_AGGRESS				11

//
// iprog stuff
//

#define IPROG_WEAR					0
#define IPROG_REMOVE				1
#define IPROG_DROP					2
#define IPROG_SAC					3
#define IPROG_GIVE					4
#define IPROG_GREET					5
#define IPROG_FIGHT					6
#define IPROG_DEATH					7
#define IPROG_SPEECH				8
#define IPROG_ENTRY					9
#define IPROG_GET					10
#define IPROG_PULSE					11
#define IPROG_INVOKE				12
#define IPROG_VERB					13
#define IPROG_LOOT					14
#define IPROG_OPEN					15
#define IPROG_LOOK					17
#define IPROG_HIT					18

//
// rprog stuff
//

#define RPROG_PULSE					0
#define RPROG_ENTRY					1
#define RPROG_MOVE					2
#define RPROG_DROP					3
#define RPROG_SPEECH				4
#define RPROG_OPEN					5

//
// aprog stuff
//

#define APROG_PULSE					0
#define APROG_RESET					1
#define APROG_SUN					2
#define APROG_TICK					3
#define APROG_AGGRESS				4
#define APROG_MYELL					5

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

#define REPLY_YES					1
#define REPLY_NO					2
#define REPLY_NEITHER				3

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

#define CON_PLAYING					0
#define CON_GET_NAME				1
#define CON_GET_OLD_PASSWORD		2
#define CON_CONFIRM_NEW_NAME		3
#define CON_GET_NEW_PASSWORD		4
#define CON_CONFIRM_NEW_PASSWORD	5
#define CON_GET_NEW_RACE			6
#define CON_GET_NEW_SEX				7
#define CON_GET_NEW_CLASS			8
#define CON_ALLOCATE_STATS			9
#define CON_GET_ALIGNMENT			10
#define CON_GET_ETHOS				11
#define CON_DEFAULT_CHOICE			12
#define CON_GEN_GROUPS				13
#define CON_NEW_CHAR				14
#define CON_READ_IMOTD				15
#define CON_READ_MOTD				16
#define CON_BREAK_CONNECT			17
#define CON_GET_CABAL				18
#define CON_GET_SPEC_ONE			19
#define CON_GET_SPEC_TWO			20
#define	CON_GET_THERMAL				21
#define	CON_GET_MATERIAL			22
#define CON_GET_DYNAMIC				23
#define	CON_CHOOSE_ELE				24
#define CON_LEGIT_NAME				25
#define CON_GET_BEAUTY				26
#define CON_CHOOSE_WEAPON			27

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

#define TO_ROOM						0
#define TO_NOTVICT					1
#define TO_VICT						2
#define TO_CHAR						3
#define TO_ALL						4
#define TO_IMMINROOM				5
#define TO_GROUP					6
#define TO_NOTGROUP					7
#define TO_AREA						8

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

#define BAR_CLASS					0
#define BAR_CABAL					1
#define BAR_SIZE					2
#define BAR_TATTOO					3
#define BAR_LEVEL					4

#define BAR_EQUAL_TO				0
#define BAR_LESS_THAN				1
#define BAR_GREATER_THAN			2

#define BAR_SAY						0
#define BAR_ECHO					1
#define BAR_EMOTE					2

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
#define MAX_STATS					5
#define STAT_STR					0
#define STAT_INT					1
#define STAT_WIS					2
#define STAT_DEX					3
#define STAT_CON					4
#define MAX_SPECS					8
#define MAX_SPEC_SKILLS				1
#define MAX_ZOMBIE					10
#define MAX_BEAUTY					10

typedef int SPECFUN (long event_vector, ...);

struct spec_func
{
	SPECFUN *func;
	long trapvector;
};

typedef struct spec_func SPEC_FUNC;

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

//
// An affect.
//

struct affect_data
{
	AFFECT_DATA *next;
	CHAR_DATA *owner;
	char *name;
	bool valid;
	short where;
	short type;
	short level;
	short duration;
	short location;
	short modifier;
	short mod_name;
	long bitvector[MAX_BITVECTOR];
	int aftype;
	AFF_FUN *tick_fun;		// goes off every tick that char is affected
	AFF_FUN *pulse_fun;
	AFF_FUN *end_fun;		// when the affect wears off this is called
	short init_duration;
	AFF_FUN *beat_fun;		// goes off every beat
};

//
// A room affect.
//

struct room_affect_data
{
	ROOM_AFFECT_DATA *next;
	CHAR_DATA *owner;
	bool valid;
	short where;
	short type;
	short level;
	short duration;
	short location;
	short modifier;
	long bitvector[MAX_BITVECTOR];
	int aftype;
	RAFF_FUN *pulse_fun;
	RAFF_FUN *tick_fun;	// goes off every tick
	RAFF_FUN *end_fun;	// when the affect wears off this is called
};

//
// An area affect.
//

struct area_affect_data
{
	AREA_AFFECT_DATA *next;
	CHAR_DATA *owner;
	bool valid;
	short where;
	short type;
	short level;
	short duration;
	short location;
	short modifier;
	long bitvector[MAX_BITVECTOR];
	int aftype;
	AAFF_FUN *pulse_fun;
	AAFF_FUN *tick_fun;
	AAFF_FUN *end_fun;
};

struct obj_affect_data
{
	OBJ_AFFECT_DATA *next;
	CHAR_DATA *owner;
	bool valid;
	short where;
	short type;
	short level;
	short duration;
	short location;
	short modifier;
	long bitvector[MAX_BITVECTOR];
	int aftype;
	OAFF_FUN *pulse_fun;
	OAFF_FUN *tick_fun;
	OAFF_FUN *end_fun;
};

//
// affect types
//

#define AFT_SPELL					0
#define AFT_SKILL					1
#define AFT_POWER					2
#define AFT_MALADY					3
#define AFT_COMMUNE					4
#define AFT_INVIS					5
#define AFT_RUNE					6
#define AFT_TIMER					7

//
// class types
//

#define CLASS_NEITHER				0
#define CLASS_CASTER				1
#define CLASS_COMMUNER				2

//
// open or closed
//

#define CLASS_OPEN					1
#define CLASS_CLOSED				0

//
// open or closed  race
//

#define RACE_OPEN					1
#define RACE_CLOSED					0

//
// const.c skill table types
//

#define CMD_NONE 					0
#define CMD_SPELL					1
#define CMD_COMMUNE					2
#define CMD_POWER					3
#define CMD_BOTH					4
#define CMD_RUNE					5

#define CAN_DISPEL					(1 << ASCII_A)
#define CAN_CANCEL					(1 << ASCII_B)
#define CAN_CLEANSE					(1 << ASCII_C)
#define CAN_PURIFY					(1 << ASCII_D)

//
// wear definitions
//

#define TO_AFFECTS					0
#define TO_OBJECT					1
#define TO_IMMUNE					2
#define TO_RESIST					3
#define TO_VULN						4
#define TO_WEAPON					5

//
// runes only
//

#define RUNE_TO_WEAPON				0
#define RUNE_TO_ARMOR				1
#define RUNE_TO_PORTAL				2
#define RUNE_TO_ROOM				3

#define RUNE_TRIGGER_ENTRY			0
#define RUNE_TRIGGER_EXIT			1

//
// where definitions for room
//

#define TO_ROOM_AFFECTS				0
#define TO_ROOM_CONST				1
#define TO_ROOM_FLAGS				2

//
// where definitions for objs
//

#define TO_OBJ_AFFECTS				0
#define TO_OBJ_APPLY				1

//
// where definitions for area
//

#define TO_AREA_AFFECTS				0

//
/// room applies
//

#define APPLY_ROOM_NONE				0
#define APPLY_ROOM_HEAL				1
#define APPLY_ROOM_MANA				2
#define APPLY_ROOM_SECT				3
#define APPLY_ROOM_NOPE				4

//
// obj applies
//

#define	APPLY_OBJ_NONE				0
#define	APPLY_OBJ_V0				1
#define	APPLY_OBJ_V1				2
#define APPLY_OBJ_V2				3
#define APPLY_OBJ_V3				4
#define APPLY_OBJ_V4				5
#define APPLY_OBJ_WEIGHT			6

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

#define CABAL_NONE					0
#define CABAL_GUILD					1
#define CABAL_SCION					2
#define CABAL_HORDE					3
#define CABAL_BOUNTY				4
#define CABAL_THEATRE				5
#define CABAL_PHALANX				6
#define CABAL_NEGATIVE				8

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

#define ACT_IS_NPC					0	// Auto set for mobs
#define ACT_SENTINEL				1	// Stays in one room
#define ACT_SCAVENGER				2	// Picks up objects
#define ACT_WARD_MOB				3	// Ward mobs
#define ACT_WANDER					4	// wanders
#define ACT_AGGRESSIVE				5	// Attacks PC's
#define ACT_STAY_AREA				6	// Won't leave area
#define ACT_WIMPY					7
#define ACT_PET						8	// Auto set for pets
#define ACT_TRAIN					9	// Can train PC's
#define ACT_PRACTICE				10	// Can practice PC's
#define ACT_SMARTTRACK				11	// Will use pathfinding
#define ACT_UNDEAD					14
#define ACT_INNER_GUARDIAN			15	// yay.
#define ACT_CLERIC					16
#define ACT_MAGE					17
#define ACT_INTELLIGENT 			18
#define ACT_FAST_TRACK				19
#define ACT_NOALIGN					20
#define ACT_NOPURGE					21
#define ACT_OUTDOORS				22
#define ACT_INDOORS					24
#define ACT_GUILDGUARD				25
#define ACT_IS_HEALER				26
#define ACT_GAIN					27
#define ACT_UPDATE_ALWAYS			28
#define ACT_DETECT_SPECIAL			29
#define ACT_BANKER					30
#define ACT_NOCTURNAL				31
#define ACT_DIURNAL					32
#define ACT_FASTWANDER				33
#define ACT_LAW						34

//
// damage classes
//

#define DAM_NONE					0
#define DAM_BASH					1
#define DAM_PIERCE					2
#define DAM_SLASH					3
#define DAM_FIRE					4
#define DAM_COLD					5
#define DAM_LIGHTNING				6
#define DAM_ACID					7
#define DAM_POISON					8
#define DAM_NEGATIVE				9
#define DAM_HOLY					10
#define DAM_ENERGY					11
#define DAM_MENTAL					12
#define DAM_DISEASE					13
#define DAM_DROWNING				14
#define DAM_LIGHT					15
#define DAM_OTHER					16
#define DAM_CHARM					17
#define DAM_SOUND					18
#define DAM_TRUESTRIKE				19
#define	DAM_INTERNAL				20

//
// OFF bits for mobiles
//

#define OFF_AREA_ATTACK				0
#define OFF_BACKSTAB				1
#define OFF_BASH					2
#define OFF_BERSERK					3
#define OFF_DISARM					4
#define OFF_DODGE					5
#define OFF_FADE					6 // UNUSED
#define OFF_FAST					7
#define OFF_KICK					8
#define OFF_KICK_DIRT				9
#define OFF_PARRY					10
#define OFF_RESCUE					11
#define OFF_TAIL					12
#define OFF_TRIP					13
#define OFF_CRUSH					14
#define ASSIST_ALL					15
#define ASSIST_ALIGN				16
#define ASSIST_RACE					17
#define ASSIST_PLAYERS				18
#define ASSIST_GUARD				19
#define ASSIST_VNUM					20
#define NO_TRACK					21
#define STATIC_TRACKING				22
#define SPAM_MURDER					23
#define OFF_INTIMIDATED				24
#define OFF_UNDEAD_DRAIN			25	// True undead drain, very powerful
#define ASSIST_GROUP				26

//
// return values for check_imm
//

#define IS_NORMAL					0
#define IS_IMMUNE					1
#define IS_RESISTANT				2
#define IS_VULNERABLE				3

//
// IMM bits for mobs
//

#define IMM_SUMMON					0
#define IMM_CHARM					1
#define IMM_MAGIC					2
#define IMM_WEAPON					3
#define IMM_BASH					4
#define IMM_PIERCE					5
#define IMM_SLASH					6
#define IMM_FIRE					7
#define IMM_COLD					8
#define IMM_LIGHTNING				9
#define IMM_ACID					10
#define IMM_POISON					11
#define IMM_NEGATIVE				12
#define IMM_HOLY					13
#define IMM_ENERGY					14
#define IMM_MENTAL					15
#define IMM_DISEASE					16
#define IMM_DROWNING				17
#define IMM_LIGHT					18
#define IMM_SOUND					19
#define IMM_INTERNAL				20
#define IMM_MITHRIL					23
#define IMM_SILVER					24
#define IMM_IRON					25
#define IMM_SLEEP					26

//
// RES bits for mobs
//

#define RES_SUMMON					0
#define RES_CHARM					1
#define RES_MAGIC					2
#define RES_WEAPON					3
#define RES_BASH					4
#define RES_PIERCE					5
#define RES_SLASH					6
#define RES_FIRE					7
#define RES_COLD					8
#define RES_LIGHTNING				9
#define RES_ACID					10
#define RES_POISON					11
#define RES_NEGATIVE				12
#define RES_HOLY					13
#define RES_ENERGY					14
#define RES_MENTAL					15
#define RES_DISEASE					16
#define RES_DROWNING				17
#define RES_LIGHT					18
#define RES_SOUND					19
#define RES_INTERNAL				20
#define RES_MITHRIL					23
#define RES_SILVER					24
#define RES_IRON					25
 
//
// VULN bits for mobs
//

#define VULN_SUMMON					0
#define VULN_CHARM					1
#define VULN_MAGIC					2
#define VULN_WEAPON					3
#define VULN_BASH					4
#define VULN_PIERCE					5
#define VULN_SLASH					6
#define VULN_FIRE					7
#define VULN_COLD					8
#define VULN_LIGHTNING				9
#define VULN_ACID					10
#define VULN_POISON					11
#define VULN_NEGATIVE				12
#define VULN_HOLY					13
#define VULN_ENERGY					14
#define VULN_MENTAL					15
#define VULN_DISEASE				16
#define VULN_DROWNING				17
#define VULN_LIGHT					18
#define VULN_SOUND					19
#define VULN_INTERNAL				20
#define VULN_MITHRIL				23
#define VULN_SILVER					24
#define VULN_IRON					25

//
// body form
//

#define FORM_EDIBLE					0
#define FORM_POISON					1
#define FORM_MAGICAL				2
#define FORM_INSTANT_DECAY			3
#define FORM_OTHER					4 // defined by material bit

//
// actual form
//

#define FORM_ANIMAL					6
#define FORM_SENTIENT				7
#define FORM_UNDEAD					8
#define FORM_CONSTRUCT				9
#define FORM_MIST					10
#define FORM_INTANGIBLE				11

#define FORM_BIPED					12
#define FORM_AQUATIC				13
#define FORM_INSECT					14
#define FORM_SPIDER					15
#define FORM_CRUSTACEAN				16
#define FORM_WORM					17
#define FORM_BLOB					18

#define FORM_MAMMAL					21
#define FORM_BIRD					22
#define FORM_REPTILE				23
#define FORM_SNAKE					24
#define FORM_DRAGON					25
#define FORM_AMPHIBIAN				26
#define FORM_FISH					27
#define FORM_COLD_BLOOD				28
#define FORM_NOSPEECH				29

//
// body parts
//

#define PART_HEAD					0
#define PART_ARMS					1
#define PART_LEGS					2
#define PART_HEART					3
#define PART_BRAINS					4
#define PART_GUTS					5
#define PART_HANDS					6
#define PART_FEET					7
#define PART_FINGERS				8
#define PART_EAR					9
#define PART_EYE					10
#define PART_LONG_TONGUE			11
#define PART_EYESTALKS				12
#define PART_TENTACLES				13
#define PART_FINS					14
#define PART_WINGS					15
#define PART_TAIL					16

//
// for combat
//

#define PART_CLAWS					20
#define PART_FANGS					21
#define PART_HORNS					22
#define PART_SCALES					23
#define PART_TUSKS					24

//
// Bits for 'affected_by'.
// Used in #MOBILES.
//

#define AFF_BLIND					0
#define AFF_INVISIBLE				1
#define AFF_DETECT_EVIL				2
#define AFF_DETECT_INVIS			3
#define AFF_DETECT_MAGIC			4
#define AFF_DETECT_HIDDEN			5
#define AFF_DETECT_GOOD				6
#define AFF_SANCTUARY				7
#define AFF_DETECT_CAMO				8
#define AFF_INFRARED				9 // unused!
#define AFF_CURSE					10
#define AFF_CAMOUFLAGE				11
#define AFF_POISON					12
#define AFF_PROTECTION				13
#define AFF_RAGE					14
#define AFF_SNEAK					15
#define AFF_HIDE					16
#define AFF_SLEEP					17
#define AFF_CHARM					18
#define AFF_FLYING					19
#define AFF_PASS_DOOR				20
#define AFF_HASTE					21
#define AFF_CALM					22
#define AFF_PLAGUE					23
#define AFF_PERMANENT				24
#define AFF_DARK_VISION				25
#define AFF_BERSERK					26
#define AFF_WATERBREATH				27
#define AFF_REGENERATION			28
#define AFF_SLOW					29
#define AFF_NOSHOW					30

//
// AFF bits for rooms
//

#define AFF_ROOM_RANDOMIZER			0
#define AFF_ROOM_CURSE				10
#define AFF_ROOM_POISON				12
#define AFF_ROOM_SLEEP				17
#define AFF_ROOM_PLAGUE				23
#define AFF_ROOM_SLOW				29

// Aff bits for.. AREAS!
// -- None currently. --
//

//
// Aff bits for OBJS
//

#define	AFF_OBJ_BURNING				0

//
// Sex.
// Used in #MOBILES.
//

#define SEX_NEUTRAL					0
#define SEX_MALE					1
#define SEX_FEMALE					2

//
// AC types
//

#define AC_PIERCE					0
#define AC_BASH						1
#define AC_SLASH					2
#define AC_EXOTIC					3
#define MAX_AC						4

//
// dice
//

#define DICE_NUMBER					0
#define DICE_TYPE					1
#define DICE_BONUS					2

//
// size
//

#define SIZE_TINY					0
#define SIZE_SMALL					1
#define SIZE_MEDIUM					2
#define SIZE_LARGE					3
#define SIZE_HUGE					4
#define SIZE_GIANT					5
#define SIZE_IMMENSE				6

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

#define ITEM_LIGHT					1
#define ITEM_SCROLL					2
#define ITEM_WAND					3
#define ITEM_STAFF					4
#define ITEM_WEAPON					5
#define ITEM_NULL6					6
#define	ITEM_DICE					7
#define ITEM_TREASURE				8
#define ITEM_ARMOR					9
#define ITEM_POTION					10
#define ITEM_CLOTHING				11
#define ITEM_FURNITURE				12
#define ITEM_TRASH					13
#define ITEM_CONTAINER				15
#define ITEM_DRINK_CON				17
#define ITEM_KEY					18
#define ITEM_FOOD					19
#define ITEM_MONEY					20
#define ITEM_BOAT					22
#define ITEM_CORPSE_NPC				23
#define ITEM_CORPSE_PC				24
#define ITEM_FOUNTAIN				25
#define ITEM_PILL					26
#define ITEM_PROTECT				27
#define ITEM_MAP					28
#define ITEM_PORTAL					29
#define ITEM_WARP_STONE				30
#define ITEM_ROOM_KEY				31
#define ITEM_GEM					32
#define ITEM_JEWELRY				33
#define ITEM_CAMPFIRE				34
#define ITEM_CABAL_ITEM				35
#define ITEM_SKELETON				36
#define ITEM_URN					37
#define ITEM_GRAVITYWELL			38
#define ITEM_BOOK					39
#define ITEM_PEN					40
#define ITEM_ALTAR					41
//#define ITEM_DONATION_PIT			42
#define ITEM_STONE					43

//
// Extra flags.
// Used in #OBJECTS.
///

#define ITEM_GLOW					0
#define ITEM_HUM					1
#define ITEM_DARK					2
#define ITEM_NOSHOW					3
#define ITEM_EVIL					4
#define ITEM_INVIS					5
#define ITEM_MAGIC					6
#define ITEM_NODROP					7
#define ITEM_BLESS					8
#define ITEM_ANTI_GOOD				9
#define ITEM_ANTI_EVIL				10
#define ITEM_ANTI_NEUTRAL			11
#define ITEM_NOREMOVE				12
#define ITEM_INVENTORY				13
#define ITEM_NOPURGE				14
#define ITEM_ROT_DEATH				15
#define ITEM_VIS_DEATH				16
#define ITEM_FIXED					17
#define ITEM_NODISARM				18
#define ITEM_NOLOCATE				19
#define ITEM_MELT_DROP				20
#define ITEM_UNDER_CLOTHES			21
#define ITEM_SELL_EXTRACT			22
#define ITEM_BURN_PROOF				24
#define ITEM_NOUNCURSE				25
#define ITEM_BRAND					26
#define CORPSE_NO_ANIMATE			27
#define ITEM_ANTI_LAWFUL			28
#define ITEM_ANTI_NEUT				29
#define ITEM_ANTI_CHAOTIC			30
#define ITEM_NO_STASH				31
#define ITEM_NO_SAC					32
#define ITEM_DONATION_PIT			33

//
// Wear flags.
// Used in #OBJECTS.
//

#define ITEM_TAKE					0
#define ITEM_WEAR_FINGER			1
#define ITEM_WEAR_NECK				2
#define ITEM_WEAR_BODY				3
#define ITEM_WEAR_HEAD				4
#define ITEM_WEAR_LEGS				5
#define ITEM_WEAR_FEET				6
#define ITEM_WEAR_HANDS				7
#define ITEM_WEAR_ARMS				8
#define ITEM_WEAR_SHIELD			9
#define ITEM_WEAR_ABOUT				10
#define ITEM_WEAR_WAIST				11
#define ITEM_WEAR_WRIST				12
#define ITEM_WEAR_WIELD				13
#define ITEM_WEAR_HOLD				14
#define ITEM_WEAR_FLOAT				15
#define ITEM_WEAR_BRAND				16
#define ITEM_WEAR_STRAPPED			17
#define ITEM_WEAR_COSMETIC			18 //cosmetic, misc, up to 5/person

#define RESTRICT_OTHER				0
#define RESTRICT_CLASS				1
#define RESTRICT_RACE				2
#define RESTRICT_CABAL				3

#define TRIBE_NONE					0
#define TRIBE_BOAR					1
#define TRIBE_WOLF					2
#define TRIBE_BEAR					3
#define TRIBE_HAWK					4
#define TRIBE_LION					5
#define TRIBE_ELK					6
#define TRIBE_JACKAL				7
#define TRIBE_FOX					8
#define TRIBE_BULL					9
#define TRIBE_PANTHER				10

#define PALADIN_NONE				0
#define PALADIN_PROTECTOR			1
#define PALADIN_CRUSADER			2

#define STYLE_NONE					-1
#define STYLE_GLADIATOR				0
#define STYLE_BARBARIAN				1
#define STYLE_DUELIST				2
#define STYLE_SKIRMISHER			3
#define STYLE_DRAGOON				4
#define STYLE_TACTICIAN				5
#define MAX_STYLE					7
#define MAX_STYLE_SKILL				38

//
// weapon class
//

#define WEAPON_EXOTIC				0
#define WEAPON_SWORD				1
#define WEAPON_DAGGER				2
#define WEAPON_SPEAR				3
#define WEAPON_MACE					4
#define WEAPON_AXE					5
#define WEAPON_FLAIL				6
#define WEAPON_WHIP					7
#define WEAPON_POLEARM				8
#define WEAPON_STAFF				9
#define WEAPON_HAND					10

//
// weapon types
//

#define WEAPON_FLAMING				0
#define WEAPON_FROST				1
#define WEAPON_VAMPIRIC				2
#define WEAPON_SHARP				3
#define WEAPON_VORPAL				4
#define WEAPON_TWO_HANDS			5
#define WEAPON_SHOCKING				6
#define WEAPON_POISON				7
#define WEAPON_AVENGER				8
#define WEAPON_SHADOWBANE			9
#define WEAPON_LIGHTBRINGER			10

//
// gate flags
//

#define GATE_NORMAL_EXIT			0
#define GATE_NOCURSE				1
#define GATE_GOWITH					2
#define GATE_BUGGY					3
#define GATE_RANDOM					4

//
// furniture flags
//

#define STAND_AT					0
#define STAND_ON					1
#define STAND_IN					2
#define SIT_AT						3
#define SIT_ON						4
#define SIT_IN						5
#define REST_AT						6
#define REST_ON						7
#define REST_IN						8
#define SLEEP_AT					9
#define SLEEP_ON					10
#define SLEEP_IN					11
#define PUT_AT						12
#define PUT_ON						13
#define PUT_IN						14
#define PUT_INSIDE					15
#define LOUNGE_ON					16
	
//
// Apply types (for affects).
// Used in #OBJECTS.
//

#define APPLY_NONE					0
#define APPLY_STR					1
#define APPLY_DEX					2
#define APPLY_INT					3
#define APPLY_WIS					4
#define APPLY_CON					5
#define APPLY_SEX					6
#define APPLY_CLASS					7
#define APPLY_LUCK					8 // UNUSED!!
#define APPLY_AGE					9
#define APPLY_HEIGHT				10
#define APPLY_WEIGHT				11
#define APPLY_MANA					12
#define APPLY_HIT					13
#define APPLY_MOVE					14
#define APPLY_GOLD					15
#define APPLY_EXP					16
#define APPLY_AC					17
#define APPLY_HITROLL				18
#define APPLY_DAMROLL				19
#define APPLY_SAVES					20
#define APPLY_SAVING_PARA			21
#define APPLY_SAVING_ROD			22
#define APPLY_SAVING_PETRI			23
#define APPLY_SAVING_BREATH			24
#define APPLY_SAVING_SPELL			25
#define APPLY_SPELL_AFFECT			26
#define APPLY_CARRY_WEIGHT			27
#define APPLY_DEFENSE				28
#define APPLY_REGENERATION			29
#define APPLY_SIZE					30
#define APPLY_ENERGYSTATE			31
#define APPLY_DAM_MOD				32
#define APPLY_LEGS					33
#define APPLY_ARMS					34
#define APPLY_BEAUTY				35
#define APPLY_ALIGNMENT				36
#define APPLY_ETHOS					37

//
// object affect applies.
//

#define APPLY_OBJ_PROPERTIES		100

//
// Modifier Names
//

#define MOD_NONE					-1
#define MOD_VISION					0
#define MOD_MOVEMENT				1
#define MOD_TOUGHNESS				2
#define MOD_SPEED					3
#define MOD_LEVITATION				4
#define MOD_VISIBILITY				5
#define MOD_PHASE					6
#define MOD_CONC					7
#define MOD_PROTECTION				8
#define MOD_APPEARANCE				9
#define MOD_HEARING					10
#define MOD_PERCEPTION				11
#define MOD_RESISTANCE				12
#define MOD_ENERGY_STATE			13
#define MOD_SPEECH					14
#define MOD_REGEN					15
#define MOD_WARDROBE				16

//
// Values for containers (value[1]).
// Used in #OBJECTS.
//

#define CONT_CLOSEABLE				0
#define CONT_PICKPROOF				1
#define CONT_CLOSED					2
#define CONT_LOCKED					3
#define CONT_PUT_ON					4

#define WIELD_ONE					1
#define WIELD_TWO					2
#define WIELD_PRIMARY				3

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

#define ROOM_DARK					0
#define ROOM_NO_MOB					2
#define ROOM_INDOORS				3
#define ROOM_NO_CONSECRATE			4
#define ROOM_PRIVATE				9
#define ROOM_SAFE					10
#define ROOM_SOLITARY				11
#define ROOM_PET_SHOP				12
#define ROOM_NO_RECALL				13
#define ROOM_IMP_ONLY				14
#define ROOM_GODS_ONLY				15
#define ROOM_HEROES_ONLY			16
#define ROOM_NEWBIES_ONLY			17
#define ROOM_LAW					18
#define ROOM_NOWHERE				19
#define ROOM_NO_GATE				20
#define ROOM_SILENCE				21
#define ROOM_NO_SUMMON_TO			22
#define ROOM_NO_SUMMON_FROM			23
#define ROOM_NO_ALARM				25
#define ROOM_FORCE_DUEL				27
#define ROOM_NO_MAGIC				28
#define ROOM_AREA_EXPLORE			29 // Don't use - Use area flags instead
#define ROOM_NO_COMMUNE				30

//
// Exit flags.
// Used in #ROOMS.
//

#define EX_ISDOOR					0
#define EX_CLOSED					1
#define EX_LOCKED					2
#define EX_PICKPROOF				3
#define EX_NOPASS					4
#define EX_NOCLOSE					5
#define EX_NOLOCK					6
#define EX_NOBASH					7
#define EX_NONOBVIOUS				8
#define EX_TRANSLUCENT				9
#define EX_JAMMED					10

#define ARE_NORMAL					0
#define ARE_ROAD_RIVER				1
#define ARE_CABAL					2
#define ARE_QUEST					3
#define ARE_CITY					4
#define ARE_UNOPENED				5
#define ARE_SHRINE					6

//
// Sector types.
// Used in #ROOMS.
//

#define SECT_INSIDE					7
#define SECT_CITY					1
#define SECT_FIELD					2
#define SECT_FOREST					3
#define SECT_HILLS					4
#define SECT_MOUNTAIN				5
#define SECT_WATER					6
#define SECT_UNUSED					7
#define SECT_UNDERWATER				8
#define SECT_AIR					9
#define SECT_DESERT					10
#define SECT_ROAD					11
#define SECT_CONFLAGRATION			12
#define SECT_BURNING				13
#define SECT_TRAIL					14
#define SECT_SWAMP					15
#define SECT_PARK					16
#define SECT_VERTICAL				17
#define	SECT_ICE					18
#define	SECT_SNOW					19
#define SECT_CAVE					20
#define SECT_MAX					21

//
// Trap types
//

#define TRAP_NONE					0
#define TRAP_DART					1
#define TRAP_PDART					2
#define TRAP_FIREBALL				3
#define TRAP_LIGHTNING				4
#define TRAP_SLEEPGAS				5
#define TRAP_POISONGAS				6
#define TRAP_ACID					7
#define TRAP_PIT					8
#define TRAP_BOULDER				9
#define TRAP_DRAIN					10
#define MAX_TRAP					11

#define ALIGN_NEUTRAL				0
#define ETHOS_NEUTRAL				0

//
// Alignment selections
//

#define ALIGN_NONE					-1
#define ALIGN_ANY					0
#define ALIGN_GN					1
#define ALIGN_NE					2
#define ALIGN_GE					3
#define ALIGN_G						4
#define ALIGN_N						5
#define ALIGN_E						6

//
// Ethos selections
//

#define ETHOS_NONE					-1
#define ETHOS_ANY					0
#define ETHOS_LN					1
#define ETHOS_NC					2
#define ETHOS_LC					3
#define ETHOS_L						4
#define ETHOS_N						5
#define ETHOS_C						6


//
// Class guild used in the room 'G'  flags
//

#define GUILD_WARRIOR				1
#define GUILD_THIEF					2
#define GUILD_CLERIC				3
#define GUILD_PALADIN				4
#define GUILD_ANTI_PALADIN			5
#define GUILD_RANGER				6
#define GUILD_MONK					7
#define GUILD_SHAPESHIFTER			8
#define GUILD_ASSASSIN				9
#define GUILD_NECROMANCER			10
#define GUILD_SORCERER				11

#define CLASS_NONE					0
#define CLASS_WARRIOR 				1
#define CLASS_THIEF 				2
#define CLASS_ZEALOT 				3
#define CLASS_PALADIN 				4
#define CLASS_ANTI_PALADIN			5
#define CLASS_RANGER 				6
#define CLASS_ASSASSIN 				7
#define CLASS_SHAPESHIFTER			8
#define CLASS_HEALER				9
#define CLASS_NECROMANCER 			10
#define CLASS_SORCERER				11


//
// Equipment wear locations.
// Used in #RESETS.
//

#define WEAR_NONE					-1
#define WEAR_LIGHT					0
#define WEAR_FINGER_L				1
#define WEAR_FINGER_R				2
#define WEAR_NECK_1					3
#define WEAR_NECK_2					4
#define WEAR_BODY					5
#define WEAR_HEAD					6
#define WEAR_LEGS					7
#define WEAR_FEET					8
#define WEAR_HANDS					9
#define WEAR_ARMS					10
#define WEAR_SHIELD					11
#define WEAR_ABOUT					12
#define WEAR_WAIST					13
#define WEAR_WRIST_L				14
#define WEAR_WRIST_R				15
#define WEAR_WIELD					16
#define WEAR_HOLD					17
#define WEAR_DUAL_WIELD				18
#define WEAR_FLOAT					18
#define WEAR_BRAND					19
#define WEAR_STRAPPED				20
#define WEAR_COSMETIC				21
#define MAX_WEAR					22


#define BLOCK_YES					1
#define BLOCK_NO					0

#define NORM						1

#define HIT_UNBLOCKABLE				0
#define HIT_BLOCKABLE				1

#define HIT_NOSPECIALS				0
#define HIT_SPECIALS				1

#define HIT_NOMULT					1
#define HIT_NOADD 0

#define POSTURE_OFFENSE				1
#define POSTURE_NONE				0
#define POSTURE_DEFENSE				-1

#define MAX_QUEUE					20


//*********************************************
//     VALUES OF INTEREST TO AREA BUILDERS
//     (End of this section ... stop here)
//*********************************************

//
// Conditions.
//

#define COND_DRUNK					0
#define COND_FULL					1
#define COND_THIRST					2
#define COND_HUNGER					3
#define COND_STARVING				4
#define COND_DEHYDRATED				5

#define COND_HUNGRY					50

//
// Positions.
//

#define POS_DEAD					0
#define POS_MORTAL					1
#define POS_INCAP					2
#define POS_STUNNED					3
#define POS_SLEEPING				4
#define POS_RESTING					5
#define POS_SITTING					6
#define POS_FIGHTING				7
#define POS_STANDING				8

#define MIN_PK_XP					9999	// min xp players need to start PKing
#define MIN_LEVEL_TO_PK				23		// Minimum level for players to pk

#define PK_KILLS					0
#define PK_GOOD						1
#define PK_NEUTRAL					2
#define PK_EVIL						3

#define PK_KILLED					0
#define MOB_KILLED					1

//
// ACT bits for players.
//

#define PLR_IS_NPC					0 // Don't EVER set.

//
// RT auto flags
//

#define PLR_AUTOABORT				1
#define PLR_AUTOASSIST				2
#define PLR_AUTOEXIT				3
#define PLR_AUTOLOOT				4
#define PLR_AUTOSAC					5
#define PLR_AUTOGOLD				6
#define PLR_AUTOSPLIT				7
#define PLR_COLOR					8
#define PLR_IGNORANT				9
#define PLR_BETRAYER				10

//
// RT personal flags
//

#define PLR_CODER					11
#define PLR_HEROIMM					12
#define PLR_HOLYLIGHT				13
#define PLR_EMPOWERED				14
#define PLR_NOVOID					15
#define PLR_NOSUMMON				16
#define PLR_NOFOLLOW				17

//
// 2 bits reserved, S-T
//

#define PLR_NO_TRANSFER				19

//
// Bad flags
//

#define PLR_PERMIT					20
#define PLR_MORON					21
#define PLR_LOG						22
#define PLR_DENY					23
#define PLR_FREEZE					24
#define PLR_THIEF					25
#define PLR_KILLER					26
#define PLR_CRIMINAL				27

//
// RT comm flags -- may be used on both mobs and chars
//

#define COMM_QUIET					0
#define COMM_DEAF					1
#define COMM_NOWIZ					2
#define COMM_NOAUCTION				3
#define COMM_NOGOSSIP				4
#define COMM_NOQUESTION				5
#define COMM_NONEWBIE				6
#define COMM_NOCABAL				7
#define COMM_NOQUOTE				8
#define COMM_SHOUTSOFF				9
#define COMM_ANSI					10

//
// display flags
//

#define COMM_COMPACT				11
#define COMM_BRIEF					12
#define COMM_PROMPT					13
#define COMM_COMBINE				14
#define COMM_TELNET_GA				15
#define COMM_SHOW_AFFECTS			16
#define COMM_IMMORTAL				18

//
// penalties
//

#define COMM_NOEMOTE				19
#define COMM_NOSHOUT				20
#define COMM_NOTELL					21
#define COMM_NOCHANNELS				22 
#define COMM_BUILDER				23
#define COMM_SNOOP_PROOF			24
#define COMM_AFK					25
#define COMM_ALL_CABALS				26
#define COMM_NOSOCKET				27
#define COMM_SWITCHSKILLS			28
#define COMM_NOBUILDER				29
#define COMM_LOTS_O_COLOR			30

//
// Trust flags
//

#define TRUST_GROUP					0
#define TRUST_CABAL					1

//
// WIZnet flags
//

#define WIZ_ON						0
#define WIZ_TICKS					1
#define WIZ_LOGINS					2
#define WIZ_SITES					3

#ifdef WIZ_LINKS
#undef WIZ_LINKS
#define WIZ_LINKS					4
#else
#define WIZ_LINKS					4
#endif

#define WIZ_DEATHS					5
#define WIZ_RESETS					6
#define WIZ_MOBDEATHS				7
#define WIZ_FLAGS					8
#define WIZ_PENALTIES				9
#define WIZ_SACCING					10
#define WIZ_LEVELS					11
#define WIZ_SECURE					12
#define WIZ_SWITCHES				13
#define WIZ_SNOOPS					14
#define WIZ_RESTORE					15
#define WIZ_LOAD					16
#define WIZ_NEWBIE					17
#define WIZ_PREFIX					18
#define WIZ_SPAM					19
#define WIZ_CABAL					20
#define WIZ_PERCENT					21
#define WIZ_LOG						22
#define	WIZ_OOC						23
#define WIZ_DEBUG					24

//
// AP Demonic Favors Defines
//

#define DEVIL_ASMODEUS				0
#define DEVIL_MOLOCH				1
#define DEVIL_BAAL					2
#define DEVIL_DISPATER				3
#define DEVIL_MEPHISTO				4
#define MAX_DEVIL					5

#define LESSER_BARBAS				0
#define LESSER_AAMON				1
#define LESSER_MALAPHAR				2
#define LESSER_FURCAS				3
#define LESSER_IPOS					4
#define MAX_LESSER					5

#define GREATER_OZE					0
#define GREATER_GAMYGYN				1
#define GREATER_OROBAS				2
#define GREATER_GERYON				3
#define GREATER_CIMERIES			4
#define MAX_GREATER					5

#define LESSER_DEMON				0
#define GREATER_DEMON				1

#define FAVOR_NONE					0
#define FAVOR_FAILED				-1
#define FAVOR_IN_PROGRESS			1
#define FAVOR_GRANTED				2

#define GERYON_EYE					2
#define GERYON_FINGER				3
#define CIMERIES_EAR				2
#define CIMERIES_NOSE				3

//
// Sorcerer Elemental Groups
//

#define ELE_HEAT					0
#define ELE_COLD					1
#define ELE_AIR						2
#define ELE_EARTH					3
#define ELE_WATER					4
#define ELE_ELECTRICITY				5
#define ELE_SMOKE					6
#define ELE_MAGMA					7
#define ELE_PLASMA					8
#define ELE_ACID					9
#define ELE_BLIZZARD				10
#define ELE_FROST					11
#define ELE_CRYSTAL					12
#define ELE_ICE						13
#define ELE_LIGHTNING				14
#define ELE_MIST					15
#define ELE_METAL					16
#define ELE_OOZE					17
#define ELE_NONE					18
#define MAX_ELE						19

#define ELE_TYPE_NONE				0
#define ELE_TYPE_PRIMARY			1
#define ELE_TYPE_PARA				2

#define MAT_TRANSLUCENT				0
#define MAT_TRANSPARENT				1
#define MAT_EDIBLE					2

#define MAT_SOLID					0
#define MAT_LIQUID					1
#define MAT_GAS						2
#define MAT_PLASMA					3

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

//
// Speech info for progs
//

#define SPEECH_SAY					0
#define SPEECH_SAYTO				1
#define SPEECH_TELL					2
#define SPEECH_WHISPER				3
#define SPEECH_YELL					4
#define SPEECH_EMOTE				5
#define SPEECH_ECHO					6
#define SPEECH_SING					7

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

	char queue[20][MAX_INPUT_LENGTH];
	short read_next;
	short write_next;
	bool pending;

	//
	// proficiency data
	//

	CProficiencies	profs;
};

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


//
// Prototype for an object.
//

struct obj_index_data
{
	OBJ_INDEX_DATA *next;
	EXTRA_DESCR_DATA *extra_descr;
	AFFECT_DATA *affected;
	AFFECT_DATA *charaffs;
	OBJ_APPLY_DATA *apply;
	AREA_DATA *area;
	bool new_format;
	char *name;
	char *short_descr;
	char *description;
	short vnum;
	short reset_num;
	short cabal;
	char *material;
	short material_index;
	short item_type;
	long extra_flags[MAX_BITVECTOR];
	long wear_flags[MAX_BITVECTOR];
	long restrict_flags[MAX_BITVECTOR];
	long imm_flags[MAX_BITVECTOR];
	long res_flags[MAX_BITVECTOR];
	long vuln_flags[MAX_BITVECTOR];
	short start_timer;
	short level;
	short condition;
	short limtotal;
	short limcount;
	short count;
	short weight;
	int cost;
	int value[5];
	IPROG_DATA *iprogs;
	long progtypes[MAX_BITVECTOR];
	char *wear_echo[2];
	char *remove_echo[2];
	char *verb;
	char *notes;
	char *wear_loc_name;
	SPEC_FUNC spec_prog;
};

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

struct obj_apply_data
{
	bool valid;
	short location;
	short modifier;
	short type;						// For gsns, if relevant.
	OBJ_APPLY_DATA *next;
};


//
// Exit data.
//

struct exit_data
{
	union
	{
		ROOM_INDEX_DATA *to_room;
		short vnum;
	} u1;

	long exit_info[MAX_BITVECTOR];
	short key;
	char *keyword;
	char *description;
	EXIT_DATA *next;					// OLC
	int orig_door;						// OLC
	RUNE_DATA *rune;
	bool has_rune;
};



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


//
// Area definition.
//

struct area_data
{
	AREA_DATA *next;
	AREA_DATA *prev;
	RESET_DATA *reset_first;
	RESET_DATA *reset_last;
	APROG_DATA *aprogs;
	long progtypes[MAX_BITVECTOR];
	char *file_name;
	char *name;
	char *credits;
	short age;
	short nplayer;
	short low_range;
	short high_range;
	short min_vnum;
	short max_vnum;
	short area_type;
	long area_flags[MAX_BITVECTOR];
	bool empty;
	long affected_by[MAX_BITVECTOR];
	AREA_AFFECT_DATA *affected;
	AREA_DATA *adjacent[MAX_ADJACENT];
	short climate;
	short sky;
	short temp;
	short wind;
	char *builders;						// OLC - Listing of
	int vnum;							// OLC - Area vnum
	int security;						// OLC - Value 1-9
};


//
// Room type.
//

struct room_index_data
{
	ROOM_INDEX_DATA *next;
	ROOM_INDEX_DATA *next_room;
	ROOM_INDEX_DATA *aff_next;
	CHAR_DATA *people;
	OBJ_DATA *contents;
	EXTRA_DESCR_DATA *extra_descr;
	AREA_DATA *area;
	EXIT_DATA *exit[6];
	TRACK_DATA *tracks[20];
	PATHFIND_DATA *path;				// For smart tracking
	char *name;
	char *alt_name;
	char *description;
	char *alt_description;
	char *owner;
	short alt_description_cond;
	short vnum;
	long room_flags[MAX_BITVECTOR];
	short sector_type;
	short heal_rate;
	short mana_rate;
	short cabal;
	short guild;
	ROOM_AFFECT_DATA *affected;
	long affected_by[MAX_BITVECTOR];
	RPROG_DATA *rprogs;
	long progtypes[MAX_BITVECTOR];
	bool move_progs;
	TRAP_DATA *trap;
	RUNE_DATA *rune;
	bool has_rune;
	short light;
	RESET_DATA *reset_first;			// OLC
	RESET_DATA *reset_last; 			// OLC
};

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

#define TAR_IGNORE					0
#define TAR_CHAR_OFFENSIVE			1
#define TAR_CHAR_DEFENSIVE			2
#define TAR_CHAR_SELF				3
#define TAR_OBJ_INV					4
#define TAR_OBJ_CHAR_DEF			5
#define TAR_OBJ_CHAR_OFF			6
#define TAR_DIR						7
#define TAR_CHAR_AMBIGUOUS			8
#define TAR_CHAR_GENERAL			9


#define TAR_END						666 //don't touch

#define TARGET_CHAR					0
#define TARGET_OBJ					1
#define TARGET_ROOM					2
#define TARGET_NONE					3
#define TARGET_RUNE					4
#define TARGET_DIR					5

//
//rune target bitvectors
//

#define RUNE_CAST					1
#define RUNE_ARMOR					2
#define RUNE_WEAPON					4
#define RUNE_DOOR					8
#define RUNE_ROOM					16


//
// Utility macros.
//

#define URANGE(a, b, c)				(b < a ? a : (b > c ? c : b))
#define LOWER(c)					(c >= 'A' && c <= 'Z' ? c + 'a' - 'A' : c)
#define UPPER(c)					(c >= 'a' && c <= 'z' ? c + 'A' - 'a' : c)
#define IS_SET(flag, bit)			((flag[(int)(bit/32)]) & ((long)pow(2,(bit % 32))))
#define IS_SET_OLD(flag, bit) 		(flag & ((long)pow(2,bit)))
#define SET_BIT(var, bit)			((var[(int)bit/32]) |= ((long)pow(2,bit % 32)))
#define SET_BIT_OLD(var, bit) 		(var |= (long)pow(2,bit))
#define REMOVE_BIT(var, bit)		((var[bit/32]) &= ~((long)pow(2,bit % 32)))
#define REMOVE_BIT_OLD(var,bit) 	(var &= ~((long)pow(2,bit)))
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

#define AREA_NONE					0
#define AREA_EXPLORE				0		// So far, only that gear returns to newbies
#define AREA_NO_NEWBIES				1		// Newbies can't go in
#define AREA_UNGHOST				2		// Walking in unghosts you
#define AREA_CHANGED				3		// Area has been modified.
#define AREA_ADDED					4		// Area has been added to.
#define AREA_LOADING				5		// Used for counting in db.c

#define MAX_DIR						6
#define NO_FLAG						-99		// Must not be used in flags or stats.


#define CAP(st)						(*(st) = UPPER(*(st)), st)
#define	HIGH_VNUM					65535
#define LOW_VNUM					-1


#define CLEAR_MEM(stru, x)		for(unsigned int clearmem = 0; clearmem < x; clearmem++) *((char *)stru + clearmem) = '\0';

#endif /* MERC_H */
