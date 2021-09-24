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

//
// Short scalar types.
// Diavolo reports AIX compiler has bugs with short types.
//

#ifdef _AIX
	#ifndef const
	#define const
	#endif /* const */

	typedef int sh_int;
	typedef int bool;

	#define unix
#else
	typedef short int sh_int;
#endif /* _AIX */


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




extern CMud RS;

//
// Structure types.
//

typedef struct affect_data		AFFECT_DATA;
typedef struct area_data		AREA_DATA;
typedef struct ban_data			BAN_DATA;
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
typedef struct note_data		NOTE_DATA;
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
#define MAX_STRING_LENGTH			4608
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

#define MPROG_BRIBE					(A)
#define MPROG_ENTRY					(B)
#define MPROG_GREET					(C)
#define MPROG_GIVE					(D)
#define MPROG_FIGHT					(E)
#define MPROG_DEATH					(F)
#define MPROG_PULSE					(G)
#define MPROG_SPEECH				(H)
#define MPROG_ATTACK				(I)
#define MPROG_MOVE					(J)
#define MPROG_BEAT					(K)
#define MPROG_AGGRESS				(L)

//
// iprog stuff
//

#define IPROG_WEAR					(A)
#define IPROG_REMOVE				(B)
#define IPROG_DROP					(C)
#define IPROG_SAC					(D)
#define IPROG_GIVE					(E)
#define IPROG_GREET					(F)
#define IPROG_FIGHT					(G)
#define IPROG_DEATH					(H)
#define IPROG_SPEECH				(I)
#define IPROG_ENTRY					(J)
#define IPROG_GET					(K)
#define IPROG_PULSE					(L)
#define IPROG_INVOKE				(M)
#define IPROG_VERB					(N)
#define IPROG_LOOT					(O)
#define IPROG_OPEN					(P)
#define IPROG_LOOK					(R)
#define IPROG_HIT					(S)

//
// rprog stuff
//

#define RPROG_PULSE					(A)
#define RPROG_ENTRY					(B)
#define RPROG_MOVE					(C)
#define RPROG_DROP					(D)
#define RPROG_SPEECH				(E)
#define RPROG_OPEN					(F)

//
// aprog stuff
//

#define APROG_PULSE					(A)
#define APROG_RESET					(B)
#define APROG_SUN					(C)
#define APROG_TICK					(D)
#define APROG_AGGRESS				(E)
#define APROG_MYELL					(F)

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

//
// Site ban structure.
//

#define BAN_SUFFIX					A
#define BAN_PREFIX					B
#define BAN_NEWBIES					C
#define BAN_ALL						D
#define BAN_PERMIT					E
#define BAN_PERMANENT				F

//
// New ban defines
//

#define NBAN_ALL					0
#define NBAN_NEWBIE					1

#define NBAN_IP						1
#define NBAN_HOST					0

struct ban_data
{
	BAN_DATA *next;
	bool valid;
	long ban_flags[MAX_BITVECTOR];
	sh_int level;
	char *name;
};

struct buf_type
{
	BUFFER *next;
	bool valid;
	sh_int state; // error state of the buffer
	sh_int size;  // size in k
	char *string; // buffer's string
};



//
// Time and weather stuff.
///

#define SUN_DARK					0
#define SUN_RISE					1
#define SUN_LIGHT					2
#define SUN_SET						3

#define MOON_NEW					0
#define MOON_CWAXING				1
#define	MOON_HWAXING				2
#define MOON_GWAXING				3
#define MOON_FULL					4
#define	MOON_GWANING				5
#define	MOON_HWANING				6
#define	MOON_CWANING				7
#define MAX_MOON					8

#define BERUS_SPD					90
#define CALABREN_SPD				150

#define RS_EPOCH					972370801

#define NUM_SEASONS					4

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
	sh_int descriptor;
	sh_int connected;
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
	sh_int type;
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
	sh_int level;
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
	sh_int open_hour;			// First opening hour
	sh_int close_hour;			// First closing hour
	sh_int direction;			// exit dir
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
	sh_int type;		// Bar on basis of guild.. or?
	sh_int comparison;	// less than greater than = to
	sh_int value;		// value allowed in
	sh_int vnum;		// Vnum it bars entry to
	sh_int msg_type;	// echo/say/emote
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
	sh_int attr_prime;		// Prime attribute/Prime Stat
	sh_int align;			// see pc_race_type
	sh_int xpadd;			// see pc_race_type
	sh_int weapon;			// First weapon
	sh_int skill_adept;		// Maximum skill level
	sh_int thac0_00;		// Thac0 for level 0
	sh_int thac0_32;		// Thac0 for level 32
	sh_int hp_min;			// Min hp gained on leveling
	sh_int hp_max;			// Max hp gained on leveling
	bool fMana;				// Class gains mana on level
	char *base_group;		// base skills gained
	char *default_group;	// default skills gained
	sh_int ctype;			// communer/caster?
	sh_int status;
};

struct race_data
{
	RACE_DATA *first;
	RACE_DATA *next;
	sh_int number;
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
	sh_int perm_stats[MAX_STATS];
	sh_int armor[4];
	float dam_mod;
	sh_int carry_weight;
	sh_int carry_number;
	sh_int saving_throw;
	long affected_by[MAX_BITVECTOR];
	long res_flags[MAX_BITVECTOR];
	long vuln_flags[MAX_BITVECTOR];
	long imm_flags[MAX_BITVECTOR];
	OLD_CHAR *next;
	OBJ_DATA *carrying;
};

//
// Data structure for notes.
//

#define NOTE_NOTE					0
#define NOTE_IDEA					1
#define NOTE_PENALTY				2
#define NOTE_NEWS					3
#define NOTE_CHANGES				4

struct note_data
{
	NOTE_DATA *next;
	bool valid;
	sh_int type;
	char *sender;
	char *date;
	char *to_list;
	char *subject;
	char *text;
	time_t date_stamp;
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
	sh_int where;
	sh_int type;
	sh_int level;
	sh_int duration;
	sh_int location;
	sh_int modifier;
	sh_int mod_name;
	long bitvector[MAX_BITVECTOR];
	int aftype;
	AFF_FUN *tick_fun;		// goes off every tick that char is affected
	AFF_FUN *pulse_fun;
	AFF_FUN *end_fun;		// when the affect wears off this is called
	sh_int init_duration;
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
	sh_int where;
	sh_int type;
	sh_int level;
	sh_int duration;
	sh_int location;
	sh_int modifier;
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
	sh_int where;
	sh_int type;
	sh_int level;
	sh_int duration;
	sh_int location;
	sh_int modifier;
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
	sh_int where;
	sh_int type;
	sh_int level;
	sh_int duration;
	sh_int location;
	sh_int modifier;
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

#define CAN_DISPEL					(1 << A)
#define CAN_CANCEL					(1 << B)
#define CAN_CLEANSE					(1 << C)
#define CAN_PURIFY					(1 << D)

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
	sh_int number;
	sh_int killed;
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

#define A							0
#define B							1
#define C							2
#define D							3
#define E							4
#define F							5
#define G							6
#define H							7
#define I							8
#define J							9
#define K							10
#define L							11
#define M							12
#define N							13
#define O							14
#define P							15
#define Q							16
#define R							17
#define S							18
#define T							19
#define U							20
#define V							21
#define W							22
#define X							23
#define Y							24
#define Z							25
#define aa							26
#define bb							27
#define cc							28
#define dd							29
#define ee							30
#define ff							31
#define gg							32
#define hh							33
#define ii							34
#define jj							35
#define kk							36
#define ll							37
#define mm							38
#define nn							39
#define oo							40
#define pp							41
#define MAX_BIT						42

//
// ACT bits for mobs.
// Used in #MOBILES.
//

#define ACT_IS_NPC					(A)	// Auto set for mobs
#define ACT_SENTINEL				(B)	// Stays in one room
#define ACT_SCAVENGER				(C)	// Picks up objects
#define ACT_WARD_MOB				(D)	// Ward mobs
#define ACT_WANDER					(E)	// wanders
#define ACT_AGGRESSIVE				(F)	// Attacks PC's
#define ACT_STAY_AREA				(G)	// Won't leave area
#define ACT_WIMPY					(H)
#define ACT_PET						(I)	// Auto set for pets
#define ACT_TRAIN					(J)	// Can train PC's
#define ACT_PRACTICE				(K)	// Can practice PC's
#define ACT_SMARTTRACK				(L)	// Will use pathfinding
#define ACT_UNDEAD					(O)
#define ACT_INNER_GUARDIAN			(P)	// yay.
#define ACT_CLERIC					(Q)
#define ACT_MAGE					(R)
#define ACT_INTELLIGENT 			(S)
#define ACT_FAST_TRACK				(T)
#define ACT_NOALIGN					(U)
#define ACT_NOPURGE					(V)
#define ACT_OUTDOORS				(W)
#define ACT_INDOORS					(Y)
#define ACT_GUILDGUARD				(Z)
#define ACT_IS_HEALER				(aa)
#define ACT_GAIN					(bb)
#define ACT_UPDATE_ALWAYS			(cc)
#define ACT_DETECT_SPECIAL			(dd)
#define ACT_BANKER					(ee)
#define ACT_NOCTURNAL				(ff)
#define ACT_DIURNAL					(gg)
#define ACT_FASTWANDER				(hh)
#define ACT_LAW						(ii)

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

#define OFF_AREA_ATTACK				(A)
#define OFF_BACKSTAB				(B)
#define OFF_BASH					(C)
#define OFF_BERSERK					(D)
#define OFF_DISARM					(E)
#define OFF_DODGE					(F)
#define OFF_FADE					(G) // UNUSED
#define OFF_FAST					(H)
#define OFF_KICK					(I)
#define OFF_KICK_DIRT				(J)
#define OFF_PARRY					(K)
#define OFF_RESCUE					(L)
#define OFF_TAIL					(M)
#define OFF_TRIP					(N)
#define OFF_CRUSH					(O)
#define ASSIST_ALL					(P)
#define ASSIST_ALIGN				(Q)
#define ASSIST_RACE					(R)
#define ASSIST_PLAYERS				(S)
#define ASSIST_GUARD				(T)
#define ASSIST_VNUM					(U)
#define NO_TRACK					(V)
#define STATIC_TRACKING				(W)
#define SPAM_MURDER					(X)
#define OFF_INTIMIDATED				(Y)
#define OFF_UNDEAD_DRAIN			(Z)	// True undead drain, very powerful
#define ASSIST_GROUP				(aa)

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

#define IMM_SUMMON					(A)
#define IMM_CHARM					(B)
#define IMM_MAGIC					(C)
#define IMM_WEAPON					(D)
#define IMM_BASH					(E)
#define IMM_PIERCE					(F)
#define IMM_SLASH					(G)
#define IMM_FIRE					(H)
#define IMM_COLD					(I)
#define IMM_LIGHTNING				(J)
#define IMM_ACID					(K)
#define IMM_POISON					(L)
#define IMM_NEGATIVE				(M)
#define IMM_HOLY					(N)
#define IMM_ENERGY					(O)
#define IMM_MENTAL					(P)
#define IMM_DISEASE					(Q)
#define IMM_DROWNING				(R)
#define IMM_LIGHT					(S)
#define IMM_SOUND					(T)
#define IMM_INTERNAL				(U)
#define IMM_MITHRIL					(X)
#define IMM_SILVER					(Y)
#define IMM_IRON					(Z)
#define IMM_SLEEP					(aa)

//
// RES bits for mobs
//

#define RES_SUMMON					(A)
#define RES_CHARM					(B)
#define RES_MAGIC					(C)
#define RES_WEAPON					(D)
#define RES_BASH					(E)
#define RES_PIERCE					(F)
#define RES_SLASH					(G)
#define RES_FIRE					(H)
#define RES_COLD					(I)
#define RES_LIGHTNING				(J)
#define RES_ACID					(K)
#define RES_POISON					(L)
#define RES_NEGATIVE				(M)
#define RES_HOLY					(N)
#define RES_ENERGY					(O)
#define RES_MENTAL					(P)
#define RES_DISEASE					(Q)
#define RES_DROWNING				(R)
#define RES_LIGHT					(S)
#define RES_SOUND					(T)
#define RES_INTERNAL				(U)
#define RES_MITHRIL					(X)
#define RES_SILVER					(Y)
#define RES_IRON					(Z)
 
//
// VULN bits for mobs
//

#define VULN_SUMMON					(A)
#define VULN_CHARM					(B)
#define VULN_MAGIC					(C)
#define VULN_WEAPON					(D)
#define VULN_BASH					(E)
#define VULN_PIERCE					(F)
#define VULN_SLASH					(G)
#define VULN_FIRE					(H)
#define VULN_COLD					(I)
#define VULN_LIGHTNING				(J)
#define VULN_ACID					(K)
#define VULN_POISON					(L)
#define VULN_NEGATIVE				(M)
#define VULN_HOLY					(N)
#define VULN_ENERGY					(O)
#define VULN_MENTAL					(P)
#define VULN_DISEASE				(Q)
#define VULN_DROWNING				(R)
#define VULN_LIGHT					(S)
#define VULN_SOUND					(T)
#define VULN_INTERNAL				(U)
#define VULN_MITHRIL				(X)
#define VULN_SILVER					(Y)
#define VULN_IRON					(Z)

//
// body form
//

#define FORM_EDIBLE					(A)
#define FORM_POISON					(B)
#define FORM_MAGICAL				(C)
#define FORM_INSTANT_DECAY			(D)
#define FORM_OTHER					(E) // defined by material bit

//
// actual form
//

#define FORM_ANIMAL					(G)
#define FORM_SENTIENT				(H)
#define FORM_UNDEAD					(I)
#define FORM_CONSTRUCT				(J)
#define FORM_MIST					(K)
#define FORM_INTANGIBLE				(L)

#define FORM_BIPED					(M)
#define FORM_AQUATIC				(N)
#define FORM_INSECT					(O)
#define FORM_SPIDER					(P)
#define FORM_CRUSTACEAN				(Q)
#define FORM_WORM					(R)
#define FORM_BLOB					(S)

#define FORM_MAMMAL					(V)
#define FORM_BIRD					(W)
#define FORM_REPTILE				(X)
#define FORM_SNAKE					(Y)
#define FORM_DRAGON					(Z)
#define FORM_AMPHIBIAN				(aa)
#define FORM_FISH					(bb)
#define FORM_COLD_BLOOD				(cc)
#define FORM_NOSPEECH				(dd)

//
// body parts
//

#define PART_HEAD					(A)
#define PART_ARMS					(B)
#define PART_LEGS					(C)
#define PART_HEART					(D)
#define PART_BRAINS					(E)
#define PART_GUTS					(F)
#define PART_HANDS					(G)
#define PART_FEET					(H)
#define PART_FINGERS				(I)
#define PART_EAR					(J)
#define PART_EYE					(K)
#define PART_LONG_TONGUE			(L)
#define PART_EYESTALKS				(M)
#define PART_TENTACLES				(N)
#define PART_FINS					(O)
#define PART_WINGS					(P)
#define PART_TAIL					(Q)

//
// for combat
//

#define PART_CLAWS					(U)
#define PART_FANGS					(V)
#define PART_HORNS					(W)
#define PART_SCALES					(X)
#define PART_TUSKS					(Y)

//
// Bits for 'affected_by'.
// Used in #MOBILES.
//

#define AFF_BLIND					(A)
#define AFF_INVISIBLE				(B)
#define AFF_DETECT_EVIL				(C)
#define AFF_DETECT_INVIS			(D)
#define AFF_DETECT_MAGIC			(E)
#define AFF_DETECT_HIDDEN			(F)
#define AFF_DETECT_GOOD				(G)
#define AFF_SANCTUARY				(H)
#define AFF_DETECT_CAMO				(I)
#define AFF_INFRARED				(J) // unused!
#define AFF_CURSE					(K)
#define AFF_CAMOUFLAGE				(L)
#define AFF_POISON					(M)
#define AFF_PROTECTION				(N)
#define AFF_RAGE					(O)
#define AFF_SNEAK					(P)
#define AFF_HIDE					(Q)
#define AFF_SLEEP					(R)
#define AFF_CHARM					(S)
#define AFF_FLYING					(T)
#define AFF_PASS_DOOR				(U)
#define AFF_HASTE					(V)
#define AFF_CALM					(W)
#define AFF_PLAGUE					(X)
#define AFF_PERMANENT				(Y)
#define AFF_DARK_VISION				(Z)
#define AFF_BERSERK					(aa)
#define AFF_WATERBREATH				(bb)
#define AFF_REGENERATION			(cc)
#define AFF_SLOW					(dd)
#define AFF_NOSHOW					(ee)

//
// AFF bits for rooms
//

#define AFF_ROOM_RANDOMIZER			(A)
#define AFF_ROOM_CURSE				(K)
#define AFF_ROOM_POISON				(M)
#define AFF_ROOM_SLEEP				(R)
#define AFF_ROOM_PLAGUE				(X)
#define AFF_ROOM_SLOW				(dd)

// Aff bits for.. AREAS!
// -- None currently. --
//

//
// Aff bits for OBJS
//

#define	AFF_OBJ_BURNING				(A)

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

#define ITEM_GLOW					(A)
#define ITEM_HUM					(B)
#define ITEM_DARK					(C)
#define ITEM_NOSHOW					(D)
#define ITEM_EVIL					(E)
#define ITEM_INVIS					(F)
#define ITEM_MAGIC					(G)
#define ITEM_NODROP					(H)
#define ITEM_BLESS					(I)
#define ITEM_ANTI_GOOD				(J)
#define ITEM_ANTI_EVIL				(K)
#define ITEM_ANTI_NEUTRAL			(L)
#define ITEM_NOREMOVE				(M)
#define ITEM_INVENTORY				(N)
#define ITEM_NOPURGE				(O)
#define ITEM_ROT_DEATH				(P)
#define ITEM_VIS_DEATH				(Q)
#define ITEM_FIXED					(R)
#define ITEM_NODISARM				(S)
#define ITEM_NOLOCATE				(T)
#define ITEM_MELT_DROP				(U)
#define ITEM_UNDER_CLOTHES			(V)
#define ITEM_SELL_EXTRACT			(W)
#define ITEM_BURN_PROOF				(Y)
#define ITEM_NOUNCURSE				(Z)
#define ITEM_BRAND					(aa)
#define CORPSE_NO_ANIMATE			(bb)
#define ITEM_ANTI_LAWFUL			(cc)
#define ITEM_ANTI_NEUT				(dd)
#define ITEM_ANTI_CHAOTIC			(ee)
#define ITEM_NO_STASH				(ff)
#define ITEM_NO_SAC					(gg)
#define ITEM_DONATION_PIT			(hh)

//
// Wear flags.
// Used in #OBJECTS.
//

#define ITEM_TAKE					(A)
#define ITEM_WEAR_FINGER			(B)
#define ITEM_WEAR_NECK				(C)
#define ITEM_WEAR_BODY				(D)
#define ITEM_WEAR_HEAD				(E)
#define ITEM_WEAR_LEGS				(F)
#define ITEM_WEAR_FEET				(G)
#define ITEM_WEAR_HANDS				(H)
#define ITEM_WEAR_ARMS				(I)
#define ITEM_WEAR_SHIELD			(J)
#define ITEM_WEAR_ABOUT				(K)
#define ITEM_WEAR_WAIST				(L)
#define ITEM_WEAR_WRIST				(M)
#define ITEM_WEAR_WIELD				(N)
#define ITEM_WEAR_HOLD				(O)
#define ITEM_WEAR_FLOAT				(P)
#define ITEM_WEAR_BRAND				(Q)
#define ITEM_WEAR_STRAPPED			(R)
#define ITEM_WEAR_COSMETIC			(S) //cosmetic, misc, up to 5/person

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
#define STYLE_GLADIATOR				(A)
#define STYLE_BARBARIAN				(B)
#define STYLE_DUELIST				(C)
#define STYLE_SKIRMISHER			(D)
#define STYLE_DRAGOON				(E)
#define STYLE_TACTICIAN				(F)
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

#define WEAPON_FLAMING				(A)
#define WEAPON_FROST				(B)
#define WEAPON_VAMPIRIC				(C)
#define WEAPON_SHARP				(D)
#define WEAPON_VORPAL				(E)
#define WEAPON_TWO_HANDS			(F)
#define WEAPON_SHOCKING				(G)
#define WEAPON_POISON				(H)
#define WEAPON_AVENGER				(I)
#define WEAPON_SHADOWBANE			(J)
#define WEAPON_LIGHTBRINGER			(K)

//
// gate flags
//

#define GATE_NORMAL_EXIT			(A)
#define GATE_NOCURSE				(B)
#define GATE_GOWITH					(C)
#define GATE_BUGGY					(D)
#define GATE_RANDOM					(E)

//
// furniture flags
//

#define STAND_AT					(A)
#define STAND_ON					(B)
#define STAND_IN					(C)
#define SIT_AT						(D)
#define SIT_ON						(E)
#define SIT_IN						(F)
#define REST_AT						(G)
#define REST_ON						(H)
#define REST_IN						(I)
#define SLEEP_AT					(J)
#define SLEEP_ON					(K)
#define SLEEP_IN					(L)
#define PUT_AT						(M)
#define PUT_ON						(N)
#define PUT_IN						(O)
#define PUT_INSIDE					(P)
#define LOUNGE_ON					(Q)
	
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

#define CONT_CLOSEABLE				(A)
#define CONT_PICKPROOF				(B)
#define CONT_CLOSED					(C)
#define CONT_LOCKED					(D)
#define CONT_PUT_ON					(E)

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

#define ROOM_DARK					(A)
#define ROOM_NO_MOB					(C)
#define ROOM_INDOORS				(D)
#define ROOM_NO_CONSECRATE			(E)
#define ROOM_PRIVATE				(J)
#define ROOM_SAFE					(K)
#define ROOM_SOLITARY				(L)
#define ROOM_PET_SHOP				(M)
#define ROOM_NO_RECALL				(N)
#define ROOM_IMP_ONLY				(O)
#define ROOM_GODS_ONLY				(P)
#define ROOM_HEROES_ONLY			(Q)
#define ROOM_NEWBIES_ONLY			(R)
#define ROOM_LAW					(S)
#define ROOM_NOWHERE				(T)
#define ROOM_NO_GATE				(U)
#define ROOM_SILENCE				(V)
#define ROOM_NO_SUMMON_TO			(W)
#define ROOM_NO_SUMMON_FROM			(X)
#define ROOM_NO_ALARM				(Z)
#define ROOM_FORCE_DUEL				(bb)
#define ROOM_NO_MAGIC				(cc)
#define ROOM_AREA_EXPLORE			(dd) // Don't use - Use area flags instead
#define ROOM_NO_COMMUNE				(ee)

//
// Directions.
// Used in #ROOMS.
//

#define DIR_NORTH					0
#define DIR_EAST					1
#define DIR_SOUTH					2
#define DIR_WEST					3
#define DIR_UP						4
#define DIR_DOWN					5


//
// Exit flags.
// Used in #ROOMS.
//

#define EX_ISDOOR					(A)
#define EX_CLOSED					(B)
#define EX_LOCKED					(C)
#define EX_PICKPROOF				(D)
#define EX_NOPASS					(E)
#define EX_NOCLOSE					(F)
#define EX_NOLOCK					(G)
#define EX_NOBASH					(H)
#define EX_NONOBVIOUS				(I)
#define EX_TRANSLUCENT				(J)
#define EX_JAMMED					(K)

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
// Equpiment wear locations.
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

//
// Climate types
//

#define CLIMATE_NONE				0
#define CLIMATE_TEMPERATE			1
#define CLIMATE_DESERT				2
#define CLIMATE_TROPICAL			3
#define CLIMATE_ALPINE				4
#define CLIMATE_TUNDRA				5
#define CLIMATE_SUBTROPICAL			6
#define CLIMATE_ARID				7
#define CLIMATE_ENGLISH				8
#define MAX_CLIMATE					9

#define SKY_CLEAR					0
#define SKY_PCLOUDY					1
#define SKY_OVERCAST				2
#define SKY_DRIZZLE					3
#define SKY_DOWNPOUR				4
#define SKY_TSTORM					5
#define SKY_FLURRY					6
#define SKY_BLIZZARD				7
#define SKY_HAIL					8
#define MAX_SKY						9

#define TEMP_HOT					0
#define TEMP_WARM					1
#define TEMP_COOL					2
#define TEMP_COLD					3
#define MAX_TEMP					4

#define WIND_CALM					0
#define WIND_BREEZE					1
#define WIND_WINDY					2
#define WIND_GALE					3
#define MAX_WIND					4

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

#define PLR_IS_NPC					(A) // Don't EVER set.

//
// RT auto flags
//

#define PLR_AUTOABORT				(B)
#define PLR_AUTOASSIST				(C)
#define PLR_AUTOEXIT				(D)
#define PLR_AUTOLOOT				(E)
#define PLR_AUTOSAC					(F)
#define PLR_AUTOGOLD				(G)
#define PLR_AUTOSPLIT				(H)
#define PLR_COLOR					(I)
#define PLR_IGNORANT				(J)
#define PLR_BETRAYER				(K)

//
// RT personal flags
//

#define PLR_CODER					(L)
#define PLR_HEROIMM					(M)
#define PLR_HOLYLIGHT				(N)
#define PLR_EMPOWERED				(O)
#define PLR_NOVOID					(P)
#define PLR_NOSUMMON				(Q)
#define PLR_NOFOLLOW				(R)

//
// 2 bits reserved, S-T
//

#define PLR_NO_TRANSFER				(T)

//
// Bad flags
//

#define PLR_PERMIT					(U)
#define PLR_MORON					(V)
#define PLR_LOG						(W)
#define PLR_DENY					(X)
#define PLR_FREEZE					(Y)
#define PLR_THIEF					(Z)
#define PLR_KILLER					(aa)
#define PLR_CRIMINAL				(bb)

//
// RT comm flags -- may be used on both mobs and chars
//

#define COMM_QUIET					(A)
#define COMM_DEAF					(B)
#define COMM_NOWIZ					(C)
#define COMM_NOAUCTION				(D)
#define COMM_NOGOSSIP				(E)
#define COMM_NOQUESTION				(F)
#define COMM_NONEWBIE				(G)
#define COMM_NOCABAL				(H)
#define COMM_NOQUOTE				(I)
#define COMM_SHOUTSOFF				(J)
#define COMM_ANSI					(K)

//
// display flags
//

#define COMM_COMPACT				(L)
#define COMM_BRIEF					(M)
#define COMM_PROMPT					(N)
#define COMM_COMBINE				(O)
#define COMM_TELNET_GA				(P)
#define COMM_SHOW_AFFECTS			(Q)
#define COMM_IMMORTAL				(S)

//
// penalties
//

#define COMM_NOEMOTE				(T)
#define COMM_NOSHOUT				(U)
#define COMM_NOTELL					(V)
#define COMM_NOCHANNELS				(W) 
#define COMM_BUILDER				(X)
#define COMM_SNOOP_PROOF			(Y)
#define COMM_AFK					(Z)
#define COMM_ALL_CABALS				(aa)
#define COMM_NOSOCKET				(bb)
#define COMM_SWITCHSKILLS			(cc)
#define COMM_NOBUILDER				(dd)
#define COMM_LOTS_O_COLOR			(ee)

//
// Trust flags
//

#define TRUST_GROUP					(A)
#define TRUST_CABAL					(B)

//
// WIZnet flags
//

#define WIZ_ON						(A)
#define WIZ_TICKS					(B)
#define WIZ_LOGINS					(C)
#define WIZ_SITES					(D)
#define WIZ_LINKS					(E)
#define WIZ_DEATHS					(F)
#define WIZ_RESETS					(G)
#define WIZ_MOBDEATHS				(H)
#define WIZ_FLAGS					(I)
#define WIZ_PENALTIES				(J)
#define WIZ_SACCING					(K)
#define WIZ_LEVELS					(L)
#define WIZ_SECURE					(M)
#define WIZ_SWITCHES				(N)
#define WIZ_SNOOPS					(O)
#define WIZ_RESTORE					(P)
#define WIZ_LOAD					(Q)
#define WIZ_NEWBIE					(R)
#define WIZ_PREFIX					(S)
#define WIZ_SPAM					(T)
#define WIZ_CABAL					(U)
#define WIZ_PERCENT					(V)
#define WIZ_LOG						(W)
#define	WIZ_OOC						(X)
#define WIZ_DEBUG					(Y)

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

#define MAT_TRANSLUCENT				(A)
#define MAT_TRANSPARENT				(B)
#define MAT_EDIBLE					(C)

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
	sh_int vnum;
	sh_int group;
	bool new_format;
	sh_int count;
	sh_int killed;
	char *player_name;
	char *short_descr;
	char *long_descr;
	char *description;
	long act[MAX_BITVECTOR];
	long affected_by[MAX_BITVECTOR];
	sh_int alignment;
	sh_int level;
	sh_int hitroll;
	sh_int hit[3];
	sh_int mana[3];
	sh_int damage[3];
	sh_int ac[4];
	sh_int dam_type;
	long off_flags[MAX_BITVECTOR];
	long imm_flags[MAX_BITVECTOR];
	long res_flags[MAX_BITVECTOR];
	long vuln_flags[MAX_BITVECTOR];
	long mprog_flags[MAX_BITVECTOR];
	sh_int xp_mod;
	float dam_mod;
	sh_int start_pos;
	sh_int sex;
	sh_int race;
	int wealth;
	long form[MAX_BITVECTOR];
	long parts[MAX_BITVECTOR];
	sh_int size;

	// restrict mobs to a specific region

	int restrict_low;	// Low end of vnum range 
	int restrict_high;	// High end of vnum range

	sh_int affect_sn[MAX_MOB_AFFECT];
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

	sh_int cabal;
	long styles[MAX_BITVECTOR];
	sh_int ele_major;
	sh_int ele_para;
	sh_int arms;
	sh_int legs;
	char *notes;
	int swealth;
	char *attack_yell;
	sh_int profs_taught[MAX_PROFS_TAUGHT_BY_MOB];
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
	sh_int type;
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

extern CProficiencies prof_none; //empty proficiencies for jackasses who are going to ref ch->Profs() without checking IS_NPC

//
// One character (PC or NPC).
//
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
	sh_int version;
	char *short_descr;
	char *long_descr;
	char *description;
	char *prompt;
	char *prefix;
	sh_int group;
	sh_int cabal;
	sh_int sex;

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

	sh_int race;
	sh_int level;
	sh_int trust;
	int played;
	int lines;							// for the pager
	time_t logon;
	sh_int timer;
	sh_int wait;
	sh_int regen_rate;					// For imbue regeneration spell
	int hit;
	sh_int max_hit;
	sh_int mana;
	sh_int max_mana;
	sh_int move;
	sh_int max_move;
	long gold;
	long gold_bank;
	int exp;
	long act[MAX_BITVECTOR];
	long comm[MAX_BITVECTOR];			// RT added to pad the vector
	long wiznet[MAX_BITVECTOR];			// wiz stuff
	long imm_flags[MAX_BITVECTOR];
	long res_flags[MAX_BITVECTOR];
	long vuln_flags[MAX_BITVECTOR];
	sh_int invis_level;
	sh_int incog_level;
	long affected_by[MAX_BITVECTOR];
	sh_int position;
	sh_int practice;
	sh_int train;
	sh_int carry_weight;
	sh_int carry_number;
	sh_int saving_throw;				//svs
	sh_int alignment;
	sh_int hitroll;
	sh_int damroll;
	sh_int armor[4];
	sh_int wimpy;
	char *speechbuf[10];				// Buffer for various mob speech, progs, etc...

	//
	// stats
	//

	sh_int perm_stat[MAX_STATS];
	sh_int mod_stat[MAX_STATS];

	//
	// parts stuff
	//

	long form[MAX_BITVECTOR];
	long parts[MAX_BITVECTOR];
	sh_int size;
	char *material;

	//
	// mobile stuff
	//

	ROOM_INDEX_DATA *home_room;
	int mobstyle;
	long off_flags[MAX_BITVECTOR];
	sh_int damage[3];
	sh_int dam_type;
	sh_int start_pos;
	sh_int pause;
	sh_int ghost;
	int status;
	long progtypes[MAX_BITVECTOR];
	int hometown;
	float dam_mod;
	sh_int defense_mod;
	bool moved;

	//
	// skill/spell-specific stuff
	//

	sh_int arms;
	sh_int legs;
	sh_int balance;
	sh_int batter;
	CHAR_DATA *analyzePC;
	int analyze;
	sh_int pulseTimer;					// counter for racial combat pulse
	char *backup_true_name;				// Dev's SUPAR CLEVAR CORRUPTION FIX!!!
	float talismanic;
	bool disrupted;						// Has queue-using spell been disrupted?
	int bounty_timer;
	bool law_pass;
	bool stolen_from;
	CProficiencies *Profs();
private:
	CClass *	my_class;
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
	sh_int perm_hit;
	sh_int perm_mana;
	sh_int perm_move;
	sh_int true_sex;
	sh_int age_mod;
	sh_int death_timer;
	sh_int death_status;
	sh_int ethos;
	sh_int oalign;
	sh_int oethos;
	sh_int sp;
	sh_int special;
	sh_int wanteds;
	sh_int death_count;
	sh_int induct;
	sh_int start_weapon;
	sh_int killed[2]; 			// 0 = PK_KILLED, 1 = MOB_KILLED
	sh_int kills[4];
	float frags[4];
	float fragged;
	char *recentkills[100];
	sh_int old_room;
	int last_level;
	sh_int beauty;
	sh_int reputation;
	sh_int condition[6];
	sh_int learned[MAX_SKILL];
	bool group_known[MAX_GROUP];
	bool confirm_delete;
	char *alias[MAX_ALIAS];
	char *alias_sub[MAX_ALIAS];

	//
	// bounties - dev
	//

	long bounty;
	sh_int bounty_credits;

	sh_int helpid;
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
	sh_int energy_state;
	char *host;
	sh_int quests[MAX_QUESTS];
	time_t roll_time;
	sh_int birth_date;
	sh_int death_time;
	sh_int tribe;
	long sect_time[SECT_MAX];
	sh_int save_timer;
	sh_int style;
	long styles[MAX_BITVECTOR];
	long ele_sphere[MAX_BITVECTOR];
	TROPHY_DATA *trophy;
	char *command[2];
	
	//
	// Sorcerer elements data
	//

	sh_int ele_temp[4];					// used only for creation
	sh_int ele_major;
	sh_int ele_para;

	//
	// AP devil favors data
	//

	int devildata[MAX_DEVIL];
	int lesserdata[MAX_LESSER];
	int greaterdata[MAX_GREATER];

	int editing_item;
	int security;						// OLC - Builder security
	sh_int souls;
	int cabal_level;
	int bounty_killed;
 	char *temp_str;
	sh_int deposited_items[5];
	sh_int paladin_path;
	sh_int temp_quest;

	//
	// CMD_QUEUE data
	//

	char queue[20][MAX_INPUT_LENGTH];
	sh_int read_next;
	sh_int write_next;
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
	sh_int vnum;
	sh_int reset_num;
	sh_int cabal;
	char *material;
	sh_int material_index;
	sh_int item_type;
	long extra_flags[MAX_BITVECTOR];
	long wear_flags[MAX_BITVECTOR];
	long restrict_flags[MAX_BITVECTOR];
	long imm_flags[MAX_BITVECTOR];
	long res_flags[MAX_BITVECTOR];
	long vuln_flags[MAX_BITVECTOR];
	sh_int start_timer;
	sh_int level;
	sh_int condition;
	sh_int limtotal;
	sh_int limcount;
	sh_int count;
	sh_int weight;
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
	sh_int item_type;
	long extra_flags[MAX_BITVECTOR];
	long wear_flags[MAX_BITVECTOR];
	long imm_flags[MAX_BITVECTOR];
	long res_flags[MAX_BITVECTOR];
	long vuln_flags[MAX_BITVECTOR];
	long affected_by[MAX_BITVECTOR];
	sh_int wear_loc;
	sh_int weight;
	int cost;
	sh_int level;
	sh_int condition;
	char *material;
	sh_int timer;
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
	sh_int location;
	sh_int modifier;
	sh_int type;						// For gsns, if relevant.
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
		sh_int vnum;
	} u1;

	long exit_info[MAX_BITVECTOR];
	sh_int key;
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
	sh_int arg1;
	sh_int arg2;
	sh_int arg3;
	sh_int arg4;
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
	sh_int age;
	sh_int nplayer;
	sh_int low_range;
	sh_int high_range;
	sh_int min_vnum;
	sh_int max_vnum;
	sh_int area_type;
	long area_flags[MAX_BITVECTOR];
	bool empty;
	long affected_by[MAX_BITVECTOR];
	AREA_AFFECT_DATA *affected;
	AREA_DATA *adjacent[MAX_ADJACENT];
	sh_int climate;
	sh_int sky;
	sh_int temp;
	sh_int wind;
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
	sh_int alt_description_cond;
	sh_int vnum;
	long room_flags[MAX_BITVECTOR];
	sh_int sector_type;
	sh_int heal_rate;
	sh_int mana_rate;
	sh_int cabal;
	sh_int guild;
	ROOM_AFFECT_DATA *affected;
	long affected_by[MAX_BITVECTOR];
	RPROG_DATA *rprogs;
	long progtypes[MAX_BITVECTOR];
	bool move_progs;
	TRAP_DATA *trap;
	RUNE_DATA *rune;
	bool has_rune;
	sh_int light;
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
	sh_int direction;					// Which way they went.
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


extern int nAllocPerm;
extern int sAllocPerm;
extern char *target_name;

//
// These are skill_lookup return values for common skills and spells.
//

extern sh_int gsn_timer;
extern sh_int gsn_repose;
extern sh_int gsn_medicine;
extern sh_int gsn_somatic_casting;
extern sh_int gsn_frigidaura;
extern sh_int gsn_stasis_wall;
extern sh_int gsn_integrate;
extern sh_int gsn_hardenfist;
extern sh_int gsn_sensevibrations;
extern sh_int gsn_diamondskin;
extern sh_int gsn_jackal;
extern sh_int gsn_strike_of_virtue;
extern sh_int gsn_awe;
extern sh_int gsn_divine_frenzy;
extern sh_int gsn_mangled;
extern sh_int gsn_bleeding;
extern sh_int gsn_calm;
extern sh_int gsn_decrepify;
extern sh_int gsn_rage;
extern sh_int gsn_horde_communion;
extern sh_int gsn_agitate;
extern sh_int gsn_immolate;
extern sh_int gsn_enervate;
extern sh_int gsn_corona;
extern sh_int gsn_infravision;
extern sh_int gsn_faerie_fire;
extern sh_int gsn_faerie_fog;
extern sh_int gsn_displacement;
extern sh_int gsn_backstab;
extern sh_int gsn_detect_hidden;
extern sh_int gsn_detect_movement;
extern sh_int gsn_bluff;
extern sh_int gsn_false_motives;
extern sh_int gsn_dodge;
extern sh_int gsn_envenom;
extern sh_int gsn_hide;
extern sh_int gsn_peek;
extern sh_int gsn_pick_lock;
extern sh_int gsn_sneak;
extern sh_int gsn_steal;
extern sh_int gsn_silence;
extern sh_int gsn_disarm;
extern sh_int gsn_enhanced_damage;
extern sh_int gsn_kick;
extern sh_int gsn_parry;
extern sh_int gsn_rescue;
extern sh_int gsn_second_attack;
extern sh_int gsn_third_attack;
extern sh_int gsn_fourth_attack;

extern sh_int gsn_blindness;
extern sh_int gsn_burning_hands;
extern sh_int gsn_chill_touch;
extern sh_int gsn_charm_person;
extern sh_int gsn_curse;
extern sh_int gsn_invis;
extern sh_int gsn_mass_invis;
extern sh_int gsn_plague;
extern sh_int gsn_poison;
extern sh_int gsn_sleep;
extern sh_int gsn_fly;
extern sh_int gsn_sanctuary;

/* new gsn's */
extern sh_int gsn_axe;
extern sh_int gsn_dagger;
extern sh_int gsn_flail;
extern sh_int gsn_mace;
extern sh_int gsn_polearm;
extern sh_int gsn_shield_block;
extern sh_int gsn_spear;
extern sh_int gsn_sword;
extern sh_int gsn_whip;
 
extern sh_int gsn_bash;
extern sh_int gsn_berserk;
extern sh_int gsn_dirt;
extern sh_int gsn_hand_to_hand;
extern sh_int gsn_trip;
 
extern sh_int gsn_fast_healing;
extern sh_int gsn_haggle;
extern sh_int gsn_lore;
extern sh_int gsn_meditation;
 
extern sh_int gsn_scrolls;
extern sh_int gsn_staves;
extern sh_int gsn_wands;
extern sh_int gsn_recall;

extern sh_int gsn_mimic;
extern sh_int gsn_liquify;
extern sh_int gsn_morph_weapon;

extern sh_int gsn_ground_control;

extern sh_int gsn_absorb;
extern sh_int gsn_camouflage;
extern sh_int gsn_acute_vision;
extern sh_int gsn_ambush;
extern sh_int gsn_laying_hands;
extern sh_int gsn_battlecry;
extern sh_int gsn_circle;
extern sh_int gsn_empower;
extern sh_int gsn_dual_backstab;
extern sh_int gsn_power_word_fear;
extern sh_int gsn_skin;
extern sh_int gsn_camp;
extern sh_int gsn_steel_nerves;
extern sh_int gsn_animate_dead;
extern sh_int gsn_barkskin;
extern sh_int gsn_animal_call;
extern sh_int gsn_aura_of_sustenance;
extern sh_int gsn_shadowgate;
extern sh_int gsn_eye_of_the_predator;
extern sh_int gsn_blackjack;
extern sh_int gsn_dual_wield;
extern sh_int gsn_trance;
extern sh_int gsn_wanted;
extern sh_int gsn_cleave;
extern sh_int gsn_herb;
extern sh_int gsn_bandage;
extern sh_int gsn_protective_shield;
extern sh_int gsn_consecrate;
extern sh_int gsn_true_sight;
extern sh_int gsn_butcher;
extern sh_int gsn_vanish;
extern sh_int gsn_embalm;
extern sh_int gsn_counter;
extern sh_int gsn_acid_spit;
extern sh_int gsn_bear_call;
extern sh_int gsn_trophy;
extern sh_int gsn_tail;
extern sh_int gsn_spellbane;
extern sh_int gsn_request;
extern sh_int gsn_endure;
extern sh_int gsn_throw;
extern sh_int gsn_nerve;
extern sh_int gsn_poison_dust;
extern sh_int gsn_blindness_dust;
extern sh_int gsn_strangle;
extern sh_int gsn_warcry;
extern sh_int gsn_chimera_lion;
extern sh_int gsn_chimera_goat;
extern sh_int gsn_strange_form;
extern sh_int gsn_enlist;
extern sh_int gsn_shadowplane;
extern sh_int gsn_blackjack_timer;
extern sh_int gsn_strangle_timer;
extern sh_int gsn_tame;
extern sh_int gsn_track;
extern sh_int gsn_find_water;
extern sh_int gsn_shield_cleave;
extern sh_int gsn_spellcraft;
extern sh_int gsn_cloak_form;
extern sh_int gsn_demand;
extern sh_int gsn_shapeshift;
extern sh_int gsn_regeneration;
extern sh_int gsn_parrot;
extern sh_int gsn_door_bash;
extern sh_int gsn_headbutt;
extern sh_int gsn_gore;
extern sh_int gsn_scorch;
extern sh_int gsn_turn_undead;
extern sh_int gsn_spiritual_hammer;
extern sh_int gsn_crimson_martyr;
extern sh_int gsn_benevolence;
extern sh_int gsn_intercept;
extern sh_int gsn_champions_defense;
extern sh_int gsn_indomitable_spirit;
extern sh_int gsn_shattered_bone;
extern sh_int gsn_forget;
extern sh_int gsn_earthbind;
extern sh_int gsn_divine_touch;
extern sh_int gsn_forage;
extern sh_int gsn_assassinate;
extern sh_int gsn_defend;
extern sh_int gsn_intimidate;
extern sh_int gsn_escape;
extern sh_int gsn_moving_ambush;
extern sh_int gsn_pugil;
extern sh_int gsn_staff;
extern sh_int gsn_protection_heat_cold;
extern sh_int gsn_rally;
extern sh_int gsn_tactics;
extern sh_int gsn_lash;
extern sh_int gsn_prevent_healing;
extern sh_int gsn_regeneration;
extern sh_int gsn_quiet_movement;
extern sh_int gsn_iron_resolve;
extern sh_int gsn_atrophy;
extern sh_int gsn_stoneskin;
extern sh_int gsn_shield;
extern sh_int gsn_starvation;
extern sh_int gsn_spore;
extern sh_int gsn_barbarian_strike;
extern sh_int gsn_barbarian_bone_tear;
extern sh_int gsn_dehydrated;			// For starve/thirst
extern sh_int gsn_hold_person;			// Hold person -Detlef
extern sh_int gsn_flurry;				// Quest flurry -Dioxide
extern sh_int gsn_drum;					// Quest drum -Dioxide
extern sh_int gsn_knife;				// Thief knife -Dioxide
extern sh_int gsn_disperse;				// Disperse for Outlaw -Dioxide
extern sh_int gsn_lightwalk;			// Quest lightwalk -Dioxide
extern sh_int gsn_sanguine_ward;
extern sh_int gsn_executioner;
extern sh_int gsn_bind;
extern sh_int gsn_deafen;				// Deafen for AP -Detlef
extern sh_int gsn_divine_intervention;	// divine intervention -Dioxide
extern sh_int gsn_creep;				// creep skill -Dioxide
extern sh_int gsn_unholy_timer;
extern sh_int gsn_phat_blunt;
extern sh_int gsn_parting_blow;
extern sh_int gsn_mysterious_dream;
extern sh_int gsn_pincer;
extern sh_int gsn_devour;
extern sh_int gsn_keen_vision;
extern sh_int gsn_vampiric_touch;
extern sh_int gsn_snare;
extern sh_int gsn_snaretimer;
extern sh_int gsn_soften;
extern sh_int gsn_consecrate;
extern sh_int gsn_blade_barrier;
extern sh_int gsn_sunray;
extern sh_int gsn_enlarge;
extern sh_int gsn_mana_drain;
extern sh_int cabal_members[MAX_CABAL];
extern sh_int cabal_max[MAX_CABAL];
extern sh_int gsn_worldbind;
extern sh_int gsn_sceptre;
extern sh_int gsn_sceptret;
extern sh_int gsn_dark_blessing;
extern sh_int gsn_web;
extern sh_int gsn_volley;

extern sh_int gsn_talismanic;
extern sh_int gsn_hex;
extern sh_int gsn_visceral;
extern sh_int gsn_conflagration;
extern sh_int gsn_gravity_well;
extern sh_int gsn_ultradiffusion;
extern sh_int gsn_molecular_disjunction;
extern sh_int gsn_vacuum;
extern sh_int gsn_cyclone_forming;
extern sh_int gsn_cyclone;
extern sh_int gsn_incandescense;
extern sh_int gsn_diuretic;
extern sh_int gsn_corona;
extern sh_int gsn_earthquake;
extern sh_int gsn_mana_conduit;
extern sh_int gsn_synaptic_enhancement;
extern sh_int gsn_synaptic_impairment;
extern sh_int gsn_interference;
extern sh_int gsn_elecshield;
extern sh_int gsn_abite;
extern sh_int gsn_lbite;
extern sh_int gsn_hydroperception;
extern sh_int gsn_watershield;
extern sh_int gsn_flood;
extern sh_int gsn_tidalwave;
extern sh_int gsn_riptide;
extern sh_int gsn_watermeld;
extern sh_int gsn_sense_disturbance;
extern sh_int gsn_travelease;
extern sh_int gsn_stability;
extern sh_int gsn_crush;
extern sh_int gsn_earthshield;
extern sh_int gsn_coldshield;
extern sh_int gsn_heatshield;
extern sh_int gsn_chill;
extern sh_int gsn_reduce;
extern sh_int gsn_airshield;
extern sh_int gsn_coagulate;
extern sh_int gsn_unholy_bond;
extern sh_int gsn_drive;
extern sh_int gsn_hypothermia;
extern sh_int gsn_retreat;
extern sh_int gsn_dash;
extern sh_int gsn_imprisonvoice;
extern sh_int gsn_howl;
extern sh_int gsn_scramble_neurons;
extern sh_int gsn_acid_vein;
extern sh_int gsn_acid_stream;
extern sh_int gsn_corrode_lock;
extern sh_int gsn_attract;
extern sh_int gsn_call_lightning;
extern sh_int gsn_grounding;
extern sh_int gsn_neutralize;
extern sh_int gsn_caustic_vapor;
extern sh_int gsn_noxious_fumes;
extern sh_int gsn_drain;
extern sh_int gsn_smokescreen;
extern sh_int gsn_shroud_of_secrecy;
extern sh_int gsn_smother;
extern sh_int gsn_noxious_ward;
extern sh_int gsn_blanket;
extern sh_int gsn_accumulate_heat;
extern sh_int gsn_earthsembrace;
extern sh_int gsn_melt_rock;
extern sh_int gsn_magma_tunnel;
extern sh_int gsn_heat_earth;
extern sh_int gsn_whiteout;
extern sh_int gsn_unbreakable;
extern sh_int gsn_freeze_door;
extern sh_int gsn_frost_growth;
extern sh_int gsn_bind_feet;
extern sh_int gsn_glaciate;
extern sh_int gsn_stalactites;
extern sh_int gsn_ice_blast;
extern sh_int gsn_icy_carapace;
extern sh_int gsn_sheath_of_ice;
extern sh_int gsn_ironskin;
extern sh_int gsn_burden;
extern sh_int gsn_cloak_of_mist;
extern sh_int gsn_airy_water;
extern sh_int gsn_cooling_mist;
extern sh_int gsn_creeping_tomb;
extern sh_int gsn_pass_without_trace;
extern sh_int gsn_quicksand;
extern sh_int gsn_quicksand_sinking;
extern sh_int gsn_earthfade;
extern sh_int gsn_rust;
extern sh_int gsn_puddle;
extern sh_int gsn_indom;
extern sh_int gsn_traitors_luck;
extern sh_int gsn_darksight;
extern sh_int gsn_bloodlust;
extern sh_int gsn_unholy_communion;
extern sh_int gsn_lesser_demon;
extern sh_int gsn_greater_demon;
extern sh_int gsn_gamygyn_soul;
extern sh_int gsn_orobas_soul;
extern sh_int gsn_dark_familiar;
extern sh_int gsn_word_of_command;
extern sh_int gsn_mark_of_wrath;
extern sh_int gsn_baals_mastery;
extern sh_int gsn_living_blade;
extern sh_int gsn_leech;
extern sh_int gsn_plasma_arc;
extern sh_int gsn_sphere_of_plasma;
extern sh_int gsn_essence_of_plasma;
extern sh_int gsn_plasma_cube;
extern sh_int gsn_plasma_thread;
extern sh_int gsn_fashion_crystal;
extern sh_int gsn_farsee;
extern sh_int gsn_blinding_orb;
extern sh_int gsn_rotating_ward;
extern sh_int gsn_plant;
extern sh_int gsn_gag;
extern sh_int gsn_drag;
extern sh_int gsn_palm;
extern sh_int gsn_trail;
extern sh_int gsn_tripwire;
extern sh_int gsn_sign;
extern sh_int gsn_slash;
extern sh_int gsn_stash;
extern sh_int gsn_avoid;
extern sh_int gsn_disguise;
extern sh_int gsn_shadow_cloak;
extern sh_int gsn_stealth;
extern sh_int gsn_counterfeit;
extern sh_int gsn_strip;
extern sh_int gsn_silent_movement;
extern sh_int gsn_infidels_weight;
extern sh_int gsn_burning_vision;
extern sh_int gsn_divine_ward;
extern sh_int gsn_scribe;
extern sh_int gsn_healing_sleep;
extern sh_int gsn_deny_magic;
extern sh_int gsn_nullify;
extern sh_int gsn_bane;
extern sh_int gsn_gking;
extern sh_int gsn_lesser_golem;
extern sh_int gsn_greater_golem;
extern sh_int gsn_aggressor;
extern sh_int gsn_subdue;
extern sh_int gsn_drowning;
extern sh_int gsn_hunters_awareness;
extern sh_int gsn_hunters_stealth;
extern sh_int gsn_hunters_net;
extern sh_int gsn_severed;

extern sh_int gsn_group_retreat;
extern sh_int gsn_arms_of_light;
extern sh_int gsn_arms_of_wrath;
extern sh_int gsn_arms_of_purity;
extern sh_int gsn_arms_of_judgement;
extern sh_int gsn_holy_shroud;
extern sh_int gsn_seraphic_mantle;
extern sh_int gsn_shield_of_faith;
extern sh_int gsn_rites_of_preparation;
extern sh_int gsn_channeling;
extern sh_int gsn_empathy;
extern sh_int gsn_altruism;
extern sh_int gsn_tower_of_fortitude;
extern sh_int gsn_shadowsense;
extern sh_int gsn_fervor;
extern sh_int gsn_piety;
extern sh_int gsn_spiritual_healing;
extern sh_int gsn_retribution;
extern sh_int gsn_shroud_of_light;
extern sh_int gsn_infidels_fate;
extern sh_int gsn_mana_sickness;
extern sh_int gsn_mana_infusion;
extern sh_int gsn_tangleroot;

//
// GSNS
//

//
// Gladiator
//

extern sh_int gsn_fend;
extern sh_int gsn_entrap;
extern sh_int gsn_gouge;
extern sh_int gsn_hobble;
extern sh_int gsn_crippling_blow;
extern sh_int gsn_bleed;

//
// Duelist
//

extern sh_int gsn_maneuvering;
extern sh_int gsn_catch;
extern sh_int gsn_posture;
extern sh_int gsn_unbalance;
extern sh_int gsn_sidestep;
extern sh_int gsn_concuss;
extern sh_int gsn_disrupt_formation;

//
// Barbarian
//

extern sh_int gsn_brutality;
extern sh_int gsn_shatter;
extern sh_int gsn_uppercut;
extern sh_int gsn_impale;
extern sh_int gsn_overhead;
extern sh_int gsn_batter;

//
// Dragoon
//

extern sh_int gsn_exchange;
extern sh_int gsn_charge;
extern sh_int gsn_deflect;
extern sh_int gsn_ease;
extern sh_int gsn_brace;
extern sh_int gsn_shieldbash;

//
// Skirmisher
//

extern sh_int gsn_evasion;
extern sh_int gsn_hurl;
extern sh_int gsn_dart;
extern sh_int gsn_cracked_sternum;
extern sh_int gsn_bloody_eyes;
extern sh_int gsn_broken_arm;
extern sh_int gsn_entwine;
extern sh_int gsn_pull;
extern sh_int gsn_fifth_attack;
extern sh_int gsn_whirlwind;

//
// Tactician
//

extern sh_int gsn_leadership;
extern sh_int gsn_assess;
extern sh_int gsn_exploit;
extern sh_int gsn_outflank;
extern sh_int gsn_terrain_mastery;
extern sh_int gsn_analyze;

//
// Style skills to keep track of percents
//

extern sh_int gsn_duelist_skill;
extern sh_int gsn_gladiator_skill;
extern sh_int gsn_barbarian_skill;
extern sh_int gsn_dragoon_skill;
extern sh_int gsn_skirmisher_skill;
extern sh_int gsn_tactician_skill;

//
// Varius gsns
//

extern sh_int gsn_rprog_elven_down;

extern char *top_bounty_name[MAX_TOP_BOUNTY];
extern long top_bounty_value[MAX_TOP_BOUNTY];

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

//
// Structure for a social in the socials table.
//

struct social_type
{
	char name[20];
	char *char_no_arg;
	char *others_no_arg;
	char *char_found;
	char *others_found;
	char *vict_found;
	char *char_not_found;
	char *char_auto;
	char *others_auto;
};



//
// Global constants.
//

extern const struct class_type class_table[MAX_CLASS];

//
// Global variables.
//

extern HELP_DATA *help_first;
extern SHOP_DATA *shop_first;

extern CHAR_DATA *char_list;
extern DESCRIPTOR_DATA *descriptor_list;
extern OBJ_DATA *object_list;
extern ROOM_INDEX_DATA *room_list;
extern RUNE_DATA *rune_list;
extern long gold_constant;
extern long total_gold;
extern long player_gold;
extern int num_pfiles;
extern char bug_buf[];
extern time_t current_time;
extern bool fLogAll;
extern bool MOBtrigger;
extern FILE *fpReserve;
extern KILL_DATA kill_table[];
extern char log_buf[];
extern TIME_INFO_DATA time_info;
extern sh_int sun;
extern sh_int moon_berus;
extern sh_int berus_pos;
extern sh_int moon_calabren;
extern sh_int calabren_pos;
extern ROOM_INDEX_DATA *top_affected_room;
extern sh_int count_data[30000];

//
// Data files used by the server.
//
// AREA_LIST contains a list of areas to boot.
// All files are read in completely at bootup.
// Most output files (bug, idea, typo, shutdown) are append-only.
//
// The NULL_FILE is held open so that we have a stream handle in reserve,
//   so players can go ahead and telnet to all the other descriptors.
// Then we close it whenever we need to open a file (e.g. a save file).
//

#ifdef __unix__
	#define NULL_FILE				"/dev/null"				// To reserve one stream
#elif _WIN32
	#define NULL_FILE 				RIFT_ROOT_DIR "/nul"	// win32 equiv to /dev/null
#endif

#define TEMP_GREP_RESULTS 			RIFT_TEMP_DIR "/tempgrepresults.tmp" 		// Temporary grep results
#define TEMP_GREP_RESULTS_TWO 		RIFT_TEMP_DIR "/temp/tempgrepresults2.tmp"
#define GOD_DIR						RIFT_ROOT_DIR "/gods/"						// list of gods
#define TEMP_FILE					RIFT_PLAYER_DIR "/romtmp"
#define PLAYER_LIST					RIFT_PLAYER_DIR "/Player.lst"				// Player list for limits
#define AREA_LIST					RIFT_AREA_DIR "/area.lst"					// List of areas
#define BUG_FILE					RIFT_AREA_DIR "/bugs.txt"					// For 'bug' and bug()
#define TYPO_FILE					RIFT_AREA_DIR "/typos.txt"					// For 'typo'
#define IDEAS_FILE					RIFT_AREA_DIR "/ideas.txt"					// For ideas!
#define NOTE_FILE					RIFT_AREA_DIR "/notes.not"					// For 'notes'
#define IDEA_FILE					RIFT_AREA_DIR "/ideas.not"
#define PENALTY_FILE				RIFT_AREA_DIR "/penal.not"
#define NEWS_FILE					RIFT_AREA_DIR "/news.not"
#define CHANGES_FILE				RIFT_AREA_DIR "/chang.not"
#define BAN_FILE					RIFT_AREA_DIR "/ban.txt"
#define MULT_EXP					5


extern QUEUE_DATA *global_queue;
extern AREA_DATA *area_first;
extern PATHFIND_DATA *best_path;
extern RACE_DATA * race_list;
extern int iterations;

extern bool command_result;
extern char *command_line;

//**********************************************
//						OLC
// *********************************************

//
// Object defined in limbo.are
// Used in save.c to load objects that don't exist.
//

#define OBJ_VNUM_DUMMY				70

//
// Area flags.
//

#define AREA_NONE					0
#define AREA_EXPLORE				(A)		// So far, only that gear returns to newbies
#define AREA_NO_NEWBIES				(B)		// Newbies can't go in
#define AREA_UNGHOST				(C)		// Walking in unghosts you
#define AREA_CHANGED				(D)		// Area has been modified.
#define AREA_ADDED					(E)		// Area has been added to.
#define AREA_LOADING				(F)		// Used for counting in db.c

#define MAX_DIR						6
#define NO_FLAG						-99		// Must not be used in flags or stats.

//
// Global Constants
//

extern char * const dir_name[];
extern const sh_int rev_dir[];				// sh_int - ROM OLC

//
// Global variables
//

extern AREA_DATA *area_first;
extern AREA_DATA *area_last;
extern SHOP_DATA *shop_last;

extern int top_affect;
extern int top_area;
extern int top_ed;
extern int top_exit;
extern int top_help;
extern int top_mob_index;
extern int top_obj_index;
extern int top_reset;
extern int top_room;
extern int top_shop;

extern int top_vnum_mob;
extern int top_vnum_obj;
extern int top_vnum_room;

extern char str_empty[1];

extern MOB_INDEX_DATA *mob_index_hash[MAX_KEY_HASH];
extern OBJ_INDEX_DATA *obj_index_hash[MAX_KEY_HASH];
extern ROOM_INDEX_DATA *room_index_hash[MAX_KEY_HASH];


#define CAP(st)						(*(st) = UPPER(*(st)), st)
#define	HIGH_VNUM					65535
#define LOW_VNUM					-1


extern bool bDebug;

#define CLEAR_MEM(stru, x)		for(unsigned int clearmem = 0; clearmem < x; clearmem++) *((char *)stru + clearmem) = '\0';

#endif /* MERC_H */
