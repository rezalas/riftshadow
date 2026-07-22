#ifndef ENTITY_FWD_H
#define ENTITY_FWD_H


// Forward declarations of the entity types and the typedef aliases the
// codebase uses for them.  This header is the single source of truth for
// these names: headers that only *mention* an entity (pointers, references,
// prototypes) include this and nothing more; only code that dereferences an
// entity includes its full definition.


//
// Structure types.
//

struct	affect_data;		typedef struct	affect_data			AFFECT_DATA;
struct	area_data;			typedef struct	area_data			AREA_DATA;
struct	buf_type;			typedef struct	buf_type			BUFFER;
class	char_data;			typedef class	char_data			CHAR_DATA;
struct	descriptor_data;	typedef struct	descriptor_data		DESCRIPTOR_DATA;
struct	exit_data;			typedef struct	exit_data			EXIT_DATA;
struct	extra_descr_data;	typedef struct	extra_descr_data	EXTRA_DESCR_DATA;
struct	help_data;			typedef struct	help_data			HELP_DATA;
struct	kill_data;			typedef struct	kill_data			KILL_DATA;
struct	mem_data;			typedef struct	mem_data			MEM_DATA;
class	mob_index_data;		typedef class	mob_index_data		MOB_INDEX_DATA;
struct	speech_data;		typedef struct	speech_data			SPEECH_DATA;
struct	line_data;			typedef struct	line_data			LINE_DATA;
struct	obj_data;			typedef struct	obj_data			OBJ_DATA;
struct	obj_index_data;		typedef struct	obj_index_data		OBJ_INDEX_DATA;
struct	pc_data;			typedef struct	pc_data				PC_DATA;
struct	gen_data;			typedef struct	gen_data			GEN_DATA;
struct	reset_data;			typedef struct	reset_data			RESET_DATA;
struct	room_index_data;	typedef struct	room_index_data		ROOM_INDEX_DATA;
struct	trap_data;			typedef struct	trap_data			TRAP_DATA;
struct	shop_data;			typedef struct	shop_data			SHOP_DATA;
struct	time_info_data;		typedef struct	time_info_data		TIME_INFO_DATA;
struct	iprog_data;			typedef struct	iprog_data			IPROG_DATA;
struct	mprog_data;			typedef struct	mprog_data			MPROG_DATA;
struct	rprog_data;			typedef struct	rprog_data			RPROG_DATA;
struct	aprog_data;			typedef struct	aprog_data			APROG_DATA;
struct	bounty;				typedef struct	bounty				BOUNTY_DATA;
struct	old_char;			typedef struct	old_char			OLD_CHAR;
struct	barred_data;		typedef struct	barred_data			BARRED_DATA;
struct	room_affect_data;	typedef struct	room_affect_data	ROOM_AFFECT_DATA;
struct	area_affect_data;	typedef struct	area_affect_data	AREA_AFFECT_DATA;
struct	obj_affect_data;	typedef struct	obj_affect_data		OBJ_AFFECT_DATA;
struct	obj_apply_data;		typedef struct	obj_apply_data		OBJ_APPLY_DATA;
struct	queue_data;			typedef struct	queue_data			QUEUE_DATA;
struct	rune_data;			typedef struct	rune_data			RUNE_DATA;
struct	trophy_data;		typedef struct	trophy_data			TROPHY_DATA;
struct	track_data;			typedef struct	track_data			TRACK_DATA;
struct	pathfind_data;		typedef struct	pathfind_data		PATHFIND_DATA;
struct	race_data;			typedef struct	race_data			RACE_DATA;
struct	special_prog;		typedef struct	special_prog		SPECIAL_PROG;

//
// Function types.
//
// These live here rather than in merc.h because they are built purely on the
// incomplete types above -- a function *type* needs no complete parameter
// types.  Headers declaring prototypes can therefore include fwd.h alone.
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

#endif /* ENTITY_FWD_H */
