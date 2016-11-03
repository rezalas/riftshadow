/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
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
*	ROM 2.4 is copyright 1993-1996 Russ Taylor			   *
*	ROM has been brought to you by the ROM consortium		   *
*	    Russ Taylor (rtaylor@pacinfo.com)				   *
*	    Gabrielle Taylor (gtaylor@pacinfo.com)			   *
*	    Brian Moore (rom@rom.efn.org)				   *
*	By using this code, you have agreed to follow the terms of the	   *
*	ROM license, in the file Tartarus/doc/rom.license                  *
***************************************************************************/

/***************************************************************************
*       Tartarus code is copyright (C) 1997-1998 by Daniel Graham          *
*	In using this code you agree to comply with the Tartarus license   *
*       found in the file /Tartarus/doc/tartarus.doc                       *
***************************************************************************/

#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include "merc.h"
#include "magic.h"
#include "interp.h"


/* item type list */
const struct item_type		item_table	[]	=
{
    {	ITEM_LIGHT,		"light"		},
    {	ITEM_SCROLL,	"scroll"	},
    {	ITEM_WAND,		"wand"		},
    {   ITEM_STAFF,		"staff"		},
    {   ITEM_WEAPON,	"weapon"	},
    {   ITEM_NULL6,     "null6"   	},
    {   ITEM_DICE,     	"dice" 		},
    {	ITEM_STONE,		"stone"		},
    {   ITEM_TREASURE,	"treasure"	},
    {   ITEM_ARMOR,	"armor"		},
    {	ITEM_POTION,	"potion"	},
    {	ITEM_CLOTHING,	"clothing"	},
    {   ITEM_FURNITURE,	"furniture"	},
    {	ITEM_TRASH,	"trash"		},
    {	ITEM_CONTAINER,	"container"	},
    {	ITEM_DRINK_CON, "drink"		},
    {	ITEM_KEY,	"key"		},
    {	ITEM_FOOD,	"food"		},
    {	ITEM_MONEY,	"money"		},
    {	ITEM_BOAT,	"boat"		},
    {	ITEM_CORPSE_NPC,"npc_corpse"},
    {	ITEM_CORPSE_PC,	"pc_corpse"	},
    {   ITEM_FOUNTAIN,	"fountain"	},
    {	ITEM_PILL,	"pill"		},
    {	ITEM_PROTECT,	"protect"	},
    {	ITEM_MAP,	"map"		},
    {	ITEM_PORTAL,	"portal"	},
    {	ITEM_WARP_STONE,"warp_stone"},
    {	ITEM_ROOM_KEY,	"room_key"	},
    {	ITEM_GEM,	"gem"		},
    {	ITEM_JEWELRY,	"jewelry"	},
    {  	ITEM_SKELETON,	"skeleton"	},
    { 	ITEM_URN,	"urn"		},
    {   ITEM_GRAVITYWELL, "gravitywell" },
    {	ITEM_BOOK,	"book"		},
    {	ITEM_PEN,	"pen"		},
    {	ITEM_ALTAR,	"altar"		},
    {	ITEM_CAMPFIRE,	"campfire"	},
    {   0,		NULL		}
};


/* weapon selection table */
const	struct	weapon_type	weapon_table	[]	=
{
   { "sword",		 OBJ_VNUM_SCHOOL_SWORD,	WEAPON_SWORD,	&gsn_sword	},
   { "mace",		 OBJ_VNUM_SCHOOL_MACE,	WEAPON_MACE,	&gsn_mace 	},
   { "dagger",		 OBJ_VNUM_SCHOOL_DAGGER,	WEAPON_DAGGER,	&gsn_dagger	},
   { "axe",			 OBJ_VNUM_SCHOOL_AXE,	WEAPON_AXE,		&gsn_axe	},
   { "spear",		 OBJ_VNUM_SCHOOL_SPEAR,	WEAPON_SPEAR,	&gsn_spear	},
   { "flail",		 OBJ_VNUM_SCHOOL_FLAIL,	WEAPON_FLAIL,	&gsn_flail	},
   { "whip",		 OBJ_VNUM_SCHOOL_WHIP,	WEAPON_WHIP,	&gsn_whip	},
   { "polearm",		 OBJ_VNUM_SCHOOL_POLEARM,WEAPON_POLEARM,	&gsn_polearm},
   { "staff", 		 OBJ_VNUM_SCHOOL_STAFF, 	WEAPON_STAFF, 	&gsn_staff	},
   { "hand to hand", OBJ_VNUM_SCHOOL_SWORD, WEAPON_HAND,&gsn_sword	},
   { NULL,		0,						0,				NULL		}
};



/* wiznet table and prototype for future flag setting */
const   struct wiznet_type      wiznet_table    []              =
{
   {    "on",           WIZ_ON,         IM },
   {    "prefix",		WIZ_PREFIX,		IM },
   {    "ticks",        WIZ_TICKS,      IM },
   {    "logins",       WIZ_LOGINS,     IM },
   {    "sites",        WIZ_SITES,      L4 },
   {    "links",        WIZ_LINKS,      L7 },
   {	"newbies",		WIZ_NEWBIE,		IM },
   {	"spam",			WIZ_SPAM,		L5 },
   {    "deaths",       WIZ_DEATHS,     IM },
   {    "resets",       WIZ_RESETS,     L4 },
   {    "mobdeaths",    WIZ_MOBDEATHS,  L4 },
   {    "flags",		WIZ_FLAGS,		L5 },
   {	"penalties",	WIZ_PENALTIES,	L5 },
   {	"saccing",		WIZ_SACCING,	L5 },
   {	"levels",		WIZ_LEVELS,		IM },
   {	"load",			WIZ_LOAD,		L2 },
   {	"restore",		WIZ_RESTORE,	L2 },
   {	"snoops",		WIZ_SNOOPS,		L2 },
   {	"switches",		WIZ_SWITCHES,	L2 },
   {	"secure",		WIZ_SECURE,		L1 },
   {	"cabal",		WIZ_CABAL,		IM },
   {    "percentages",	WIZ_PERCENT,	L3 },
   {	"logging",		WIZ_LOG,		IM },
   {	"ooc",			WIZ_OOC,		IM },
   {	"debug",		WIZ_DEBUG,		L5 },
   {	NULL,			0,				0  }
};

/* attack table  -- not very organized :( */
const 	struct attack_type	attack_table	[MAX_DAMAGE_MESSAGE]	=
{
    { 	"none",			"hit",				-1,				0 	},  /*  0 */
    {	"slice",		"slice",	 		DAM_SLASH,		0 	},	
    {   "stab",			"stab",				DAM_PIERCE,		0 	},
    {	"slash",		"slash",			DAM_SLASH,		10 	},
    {	"whip",			"whip",				DAM_SLASH,		10 	},
    {   "claw",			"claw",				DAM_SLASH,		0 	},  /*  5 */
    {	"blast",		"blast",			DAM_BASH,		0 	},
    {   "pound",		"pound",			DAM_BASH,		0 	},
    {	"crush",		"crush",			DAM_BASH,		0 	},
    {   "grep",			"grep",				DAM_SLASH,		-10 },
    {	"bite",			"bite",				DAM_PIERCE,		0 	},  /* 10 */
    {   "pierce",		"pierce",			DAM_PIERCE,		0  	},
    {   "suction",		"suction",			DAM_BASH,		-20	},
    {	"beating",		"beating",			DAM_BASH,		0 	},
    {   "digestion",	"digestion",		DAM_ACID,		0 	},
    {	"charge",		"charge",			DAM_BASH,		0 	},  /* 15 */
    { 	"slap",			"slap",				DAM_BASH,		0 	},
    {	"punch",		"punch",			DAM_BASH,		0 	},
    {	"wrath",		"wrath",			DAM_HOLY,		0 	},
    {	"magic",		"magic",			DAM_ENERGY,		0 	},
    {   "divine",		"divine power",		DAM_HOLY,		0 	},  /* 20 */
    {	"cleave",		"cleave",			DAM_SLASH,		0 	},
    {	"scratch",		"scratch",			DAM_PIERCE,		0 	},
    {   "peck",			"peck",				DAM_PIERCE,		0 	},
    {   "peckb",		"peck",				DAM_BASH,		0 	},
    {   "chop",			"chop",				DAM_SLASH,		0 	},  /* 25 */
    {   "sting",		"sting",			DAM_PIERCE,		0 	},
    {   "smash",		 "smash",			DAM_BASH,		0 	},
    {   "shbite",		"shocking bite",	DAM_LIGHTNING,	0 	},
    {	"flbite",		"flaming bite", 	DAM_FIRE,		0 	},
    {	"frbite",		"freezing bite",	DAM_COLD,		0 	},  /* 30 */
    {	"acbite",		"acidic bite", 		DAM_ACID,		0 	},
    {	"chomp",		"chomp",			DAM_PIERCE,		0 	},
    {  	"drain",		"life drain",		DAM_NEGATIVE,	0 	},
    {   "thrust",		"thrust",			DAM_PIERCE,		0 	},
    {   "slime",		"slime",			DAM_ACID,		0 	}, 	/* 35 */
    {	"shock",		"shock",			DAM_LIGHTNING,	0 	},
    {   "thwack",		"thwack",			DAM_BASH,		0 	},
    {   "flame",		"flame",			DAM_FIRE,		0 	},
    {   "chill",		"chill",			DAM_COLD,		0 	},
    { 	"light",		"blast of light",	DAM_LIGHT,		0 	}, 	/* 40 */
    { 	"nature", 		"force of nature",	DAM_ENERGY, 	0 	},
    { 	"defilement", 	"defilement", 		DAM_NEGATIVE, 	0 	},
    { 	"mental",		"mental assault", 	DAM_MENTAL, 	0 	},
    { 	"sound", 		"sonic blast",		DAM_SOUND, 		0 	},
    { 	"infinite", 	"infinite darkness",DAM_NEGATIVE,	-30	},	/* 45 */
    { 	"searing", 		"searing light",	DAM_LIGHT, 		0 	},
    {	"drowning", 	"drowning", 		DAM_DROWNING, 	0 	},
    {   "chaotic",		"chaotic blast",	DAM_ENERGY, 	0 	},
    {   "sflame",		"searing flame",	DAM_ENERGY,		0 	},
    {   "terror",		"terror",			DAM_MENTAL,		0	},	/* 50 */
    {	"water",		"hydrous onslaught",DAM_DROWNING,	0 	},
    {	"thorn",		"thorn",			DAM_PIERCE,		0	},
    {	"decomp",		"decomposition",	DAM_NEGATIVE,	0	},
    {	"ptouch",		"poisonous touch",	DAM_POISON,		0	},
    {	"spike",		"spike",			DAM_PIERCE,		0	},	/* 55 */
    {	"ooze",			"ooze",				DAM_ENERGY,		0	},
    {	"asphyxiation",	"asphyxiation",		DAM_INTERNAL,	0	},
    {	"sblast",		"searing blast",	DAM_FIRE,		0	},
    {	"disease",		"poisonous bite",	DAM_POISON,		0	},	
	{   "grasp",        "foul grasp",       DAM_NEGATIVE,   0   },
    {   NULL,			NULL,				0,				0 	}
};

/*
 * The skill and spell table.
 * Slot numbers must never be changed as they appear in #OBJECTS sections.
 */
#define SLOT(n)	n

const	struct	skill_type	skill_table	[MAX_SKILL]	=
{

/*
 * Magic spells.
 */

    {
        "reserved", {60, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99},
	0,			TAR_IGNORE,		POS_STANDING,
	NULL,			0,	 0,	 0,
	"",			"",		"", CMD_NONE
    },

	{
		"generic timer", {60, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61},
		spell_null,		TAR_IGNORE,		POS_STANDING,
		&gsn_timer,		0,	0,	0,
		"",		"",		"",		CMD_NONE
	},

	{
		"mana transfer", {60, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15},
		spell_mana_transfer,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
		NULL,	0, 2, 24,
		"", "", "", CMD_POWER
	},

	{
		"scribe", {60, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25},
		spell_scribe,	TAR_IGNORE,	POS_STANDING,
		&gsn_scribe , 0, 50, 24,
		"", "", "", CMD_POWER
	},

	{
		"deny magic", {60, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25},
		spell_deny_magic,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
		&gsn_deny_magic,	0,	50,	24,
		"", "The light blue aura surrounding you fades away.", "", CMD_POWER
	},

	{
		"nullify", {60, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30},
		spell_null,	TAR_IGNORE,	POS_STANDING,
		&gsn_nullify,	0,	25,	24,
		"", "", "", CMD_POWER
	},

	{
		"bane of ignorance", {60, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20},
		spell_bane, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
		&gsn_bane,	0,	50,	24,
		"", "", "", CMD_POWER
	},

	{
        "acid blast", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_acid_blast,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			0,	20,	12,
	"acid blast",		"!Acid Blast!", "", CMD_SPELL
    },

    {
	"embalm", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 15, 53},
	spell_null,	TAR_IGNORE,	POS_STANDING,
	&gsn_embalm,	0,	20,	12,
	"",	"","", CMD_NONE
    },

    {
	"talk to dead", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 21, 53},
	spell_talk_to_dead,	TAR_IGNORE,	POS_STANDING,
	NULL,		0,	20,	12,
	"",	"",	"", CMD_BOTH
    },


	{
        "deathspell", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_deathspell,       TAR_IGNORE, POS_STANDING,
        NULL,  		0,      150,    12,
        "explosion of negative energy",  "", "", CMD_SPELL
	},

	{
		"door bash", {60, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18},
		spell_null,	TAR_IGNORE,	POS_FIGHTING,
		&gsn_door_bash,	0,	0,	0,
		"failed door bashing",	"",	"", CMD_NONE
	},

	{
		"headbutt", {60, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15},
		spell_null,		TAR_IGNORE,		POS_FIGHTING,
		&gsn_headbutt,	0,	0,	0,
		"headbutt",		"The throbbing in your head subsides.", "", CMD_NONE
	},

	{
		"gore", {60, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25},
		spell_null,		TAR_IGNORE,		POS_STANDING,
		&gsn_gore,		0,	0,	0,
		"goring",		"",	"", CMD_NONE
	},

	{
		"silent movement", {60, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		spell_null,		TAR_IGNORE,		POS_STANDING,
		&gsn_silent_movement,	0,	0,	0,
		"", "", "", CMD_NONE
	},

{
	"silence", {60, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61},
	spell_null,	TAR_IGNORE,	POS_DEAD,
	&gsn_silence,	0,	0,	0,
	"",	"You feel the words come back to you.", "", CMD_SPELL
},

{
	"brand", {60, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61},
	spell_null,	TAR_IGNORE,	POS_DEAD,
	&gsn_parrot,	0,	0,	0,
	"unholy fury",	"You feel once again ready to call upon your god.", "", CMD_NONE
},

{
	"hunger", {60, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61},
	spell_null,	TAR_IGNORE,	POS_DEAD,
	&gsn_starvation,	0,	0,	0,
	"starvation",	"You no longer feel the pangs of famine.", "", CMD_NONE
},

{
	"dehydration", {60, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61},
	spell_null,	TAR_IGNORE,	POS_DEAD,
	&gsn_dehydrated,	0,	0,	0,
	"dehydration",	"You no longer feel so thirsty.", "", CMD_NONE
},

{
	"shapeshift", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_null,	TAR_IGNORE, POS_STANDING,
	&gsn_shapeshift,	0,	0,	0,
	"",	"You resume your natural form.", "", CMD_NONE
},

{
        "lifebane", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_lifebane,        TAR_IGNORE,     POS_FIGHTING,
        NULL,   	0,      45,     12,
        "lifebane",     "", "", CMD_SPELL
},

{
        "demand", {60, 53, 53, 53, 53, 30, 53, 53, 53, 53, 53, 53},
        spell_null,     TAR_IGNORE,     POS_STANDING,
        &gsn_demand,    0,        0,      12,
        "",     "", "", CMD_NONE
},



    {
	"animate dead", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 30, 53},
	spell_animate_dead,	TAR_IGNORE,		POS_STANDING,
	&gsn_animate_dead,			0,	100, 24,
	"",			"Your power to raise dead returns.",	"", CMD_SPELL
    },

    {
	"lesser golem", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 21, 53},
	spell_lesser_golem, TAR_IGNORE, POS_STANDING,
	&gsn_lesser_golem,	0,	70,	24,
	"",	"You feel up to fashioning another lesser golem.", "", CMD_SPELL
    },

	{
	"greater golem", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 33, 53},
	spell_greater_golem, TAR_IGNORE, POS_STANDING,
	&gsn_greater_golem,	0,	70,	24,
	"",	"You feel up to fashioning another greater golem.", "", CMD_SPELL
    },

    {
        "armor", {60, 53, 53, 53, 2, 5, 7, 1, 1, 1, 3, 1},
	spell_armor,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			CAN_CANCEL|CAN_DISPEL,	 5,	12,
	"",			"You feel less armored.",	"", CMD_BOTH
    },

    {
        "bless", {60, 53, 53, 53, 5, 53, 53, 53, 53, 7, 53, 53},
	spell_bless,		TAR_OBJ_CHAR_DEF,	POS_STANDING,
	NULL,			CAN_CANCEL|CAN_DISPEL,	 5,	12,
	"",			"You feel less righteous.",
	"$n's holy aura fades.", CMD_COMMUNE
    },

    {
        "blindness", {60, 53, 53, 11, 53, 9, 53, 53, 53, 53, 12, 53},
	spell_blindness,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_blindness,		CAN_CLEANSE|CAN_DISPEL|CAN_CANCEL,	 5,	12,
	"",			"You can see again.",	"", CMD_BOTH
    },

    {
        "burning hands", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_burning_hands,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_burning_hands,	0,	15,	12,
	"burning hands$",	"!Burning Hands!", 	"", CMD_SPELL
    },

    {   "calm", {60, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30},
	spell_calm,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_calm,		0,	150,	24,
	"",			"You no longer feel so peaceful.",	"", CMD_POWER
    },

    {
        "cancellation", {60, 53, 53, 19, 53, 53, 53, 53, 16, 24, 18, 19},
	spell_cancellation,	TAR_IGNORE,	POS_FIGHTING,
	NULL,			0,	20,	12,
	"",			"!cancellation!",	"", CMD_BOTH
    },

    {
        "cause critical", {60, 53, 53, 18, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_cause_critical,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			0,	20,	12,
	"spell",		"!Cause Critical!",	"", CMD_COMMUNE
    },

    {
        "cause light", {60, 53, 53, 3, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_cause_light,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			0,	15,	12,
	"spell",		"!Cause Light!",	"", CMD_COMMUNE
    },

    {
        "cause serious", {60, 53, 53, 12, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_cause_serious,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			0,	17,	12,
	"spell",		"!Cause Serious!",	"", CMD_COMMUNE
    },

    {
        "chain lightning", {60, 53, 53, 53, 53, 53, 53, 53, 26, 53, 53, 53},
	spell_chain_lightning,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			0,	25,	12,
	"lightning",		"!Chain Lightning!",	"", CMD_SPELL
    },

    {
        "change sex", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_change_sex,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			CAN_CLEANSE|CAN_CANCEL|CAN_DISPEL,	15,	12,
	"",			"Your body feels familiar again.",	"", CMD_SPELL
    },

    {
        "charm person", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_charm_person,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_charm_person,	CAN_CLEANSE|CAN_CANCEL|CAN_DISPEL,	 80,	12,
	"",			"You feel more self-confident.",	"", CMD_SPELL
    },

    {
        "chill", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 1},
	spell_chill,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_chill,			0,	15,	12,
	"chilling touch",	"You feel less cold.",	"", CMD_SPELL
    },

    {
        "color spray", {60, 53, 53, 53, 53, 53, 53, 53, 13, 53, 53, 53},
	spell_color_spray,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			0,	15,	12,
	"color spray",		"!Colour Spray!",	"", CMD_SPELL
    },

	{
		"sanguine ward", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 38, 53},
		spell_sanguine_ward,	TAR_IGNORE,	POS_STANDING,
		&gsn_sanguine_ward,	CAN_DISPEL,	50,	24,
		"sanguine ward",	"Your sanguine ward dissipates.", "", CMD_SPELL
	},

    {
	"heavenly wrath", {60, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61},
	spell_heavenly_sceptre_frenzy,	TAR_CHAR_SELF,	POS_FIGHTING,
	NULL,	0,	0,	0,
	"",	"The heavenly wrath bestowed upon you fades away.",	"", CMD_NONE
    },

    {
	"heavenly fire", {60, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61},
	spell_heavenly_sceptre_fire,	TAR_IGNORE,	POS_FIGHTING,
	NULL,	0,	0,	0,
	"heavenly fire",	"You feel the power drained from you by the scetpre's heavenly fire return.",	"", CMD_NONE
    },

    {
	"wrath", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_wrath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			0,	20,	16,
	"heavenly wrath",	"!wrath!",	"", CMD_COMMUNE
    },

    {
        "continual light", {60, 53, 53, 53, 53, 53, 53, 53, 12, 4, 53, 53},
	spell_continual_light,	TAR_IGNORE,		POS_STANDING,
	NULL,			0,	 7,	12,
	"",			"!Continual Light!",	"", CMD_BOTH
    },

    {
        "control weather", {60, 53, 53, 53, 53, 53, 20, 53, 53, 53, 53, 53},
	spell_control_weather,	TAR_IGNORE,		POS_STANDING,
	NULL,			0,	25,	12,
	"",			"!Control Weather!",	"", CMD_SPELL
    },

    {
        "create food", {60, 53, 53, 53, 4, 53, 53, 53, 53, 1, 53, 53},
	spell_create_food,	TAR_IGNORE,		POS_STANDING,
	NULL,			0,	 5,	12,
	"",			"!Create Food!",	"", CMD_BOTH
    },

    {
        "bitten arm", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_null,	TAR_IGNORE,		POS_FIGHTING,
	&gsn_abite,			0,	30, 	12,
	"",			"You feel you can use your arm again.",	"", CMD_NONE
    },

    {
        "create spring", {60, 53, 53, 53, 53, 53, 53, 53, 53, 17, 53, 9},
	spell_create_spring,	TAR_IGNORE,		POS_STANDING,
	NULL,			0,	20,	12,
	"",			"!Create Spring!",	"", CMD_BOTH
    },

    {
        "create water", {60, 53, 53, 53, 4, 53, 53, 53, 53, 53, 53, 53},
	spell_create_water,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			0,	 5,	12,
	"",			"!Create Water!",	"", CMD_BOTH
    },

    {
        "cure blindness", {60, 53, 53, 13, 11, 53, 53, 53, 53, 7, 53, 53},
	spell_cure_blindness,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			0,	 5,	12,
	"",			"!Cure Blindness!",	"", CMD_COMMUNE
    },

    {
        "cure critical", {60, 53, 53, 18, 10, 53, 53, 35, 53, 9, 53, 53},
	spell_cure_critical,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			0,	30,	12,
	"",			"!Cure Critical!",	"", CMD_BOTH
    },

    {
        "cure disease", {60, 53, 53, 15, 16, 53, 53, 28, 53, 11, 53, 53},
	spell_cure_disease,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			0,	20,	12,
	"",			"!Cure Disease!",	"", CMD_BOTH
    },

    {
        "cure light", {60, 53, 53, 3, 1, 53, 53, 53, 53, 1, 53, 53},
	spell_cure_light,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			0,	10,	12,
	"",			"!Cure Light!",		"", CMD_COMMUNE
    },

    {
        "cure poison", {60, 53, 53, 13, 18, 53, 53, 24, 53, 10, 53, 53},
	spell_cure_poison,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			0,	 5,	12,
	"",			"!Cure Poison!",	"", CMD_BOTH
    },

    {
        "cure serious", {60, 53, 53, 9, 53, 53, 53, 53, 53, 2, 53, 53},
	spell_cure_serious,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			0,	15,	12,
	"",			"!Cure Serious!",	"", CMD_COMMUNE
    },

    {
        "curse", {60, 53, 53, 20, 53, 17, 53, 53, 53, 14, 16, 53},
	spell_curse,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_curse,			CAN_CLEANSE,		20,		12,
	"curse",		"The curse wears off.",
	"$n is no longer impure.", CMD_BOTH
    },

    {
	"dark wrath", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_dark_wrath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			0,	20,	12,
	"unholy dark wrath",	"!DarkWrath!", "", CMD_BOTH
    },

    {
        "demonfire", {60, 53, 53, 33, 53, 53, 53, 40, 53, 53, 53, 53},
	spell_demonfire,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			0,	30,	12,
	"torments$",		"!Demonfire!",		"", CMD_BOTH
    },	

	{
		"infidels weight", {60, 53, 53, 40, 53, 53, 53, 53, 53, 53, 53, 53},
		spell_infidels_weight,	TAR_CHAR_OFFENSIVE, POS_FIGHTING,
		&gsn_infidels_weight,	CAN_CLEANSE,	50,	12,
		"",	"",	"", CMD_COMMUNE
	},

	{
		"burning vision", {60, 53, 53, 40, 53, 53, 53, 53, 53, 53, 53, 53},
		spell_burning_vision,	TAR_CHAR_OFFENSIVE, POS_STANDING,
		&gsn_burning_vision,   CAN_CLEANSE,  90, 24,
		"", "Your burning vision subsides!", "", CMD_COMMUNE
	},

	{	"divine malison", {60, 53, 53, 22, 53, 53, 53, 53, 53, 53, 53, 53},
		spell_divine_malison,	TAR_CHAR_OFFENSIVE, POS_FIGHTING,
		&gsn_divine_ward,		0,			70,  24,
		"",	"You no longer suffer from a divine malison.",	"",	CMD_COMMUNE
	},

    {
        "detect evil", {60, 53, 53, 53, 1, 53, 53, 53, 53, 5, 53, 53},
	spell_detect_evil,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			CAN_CANCEL|CAN_DISPEL,	 5,	12,
	"",			"The red in your vision disappears.",	"", CMD_COMMUNE
    },

    {
        "detect good", {60, 53, 53, 53, 53, 10, 53, 53, 53, 5, 53, 53},
        spell_detect_good,      TAR_CHAR_SELF,          POS_STANDING,
        NULL,      	CAN_CANCEL|CAN_DISPEL,        5,     12,
        "",                     "The gold in your vision disappears.",	"", CMD_BOTH
    },

    {
        "detect hidden", {60, 53, 10, 53, 53, 53, 53, 18, 53, 53, 53, 53},
	spell_null,	 	TAR_IGNORE,		POS_STANDING,
	&gsn_detect_hidden,	0,	 5,	12,
	"",			"You feel less aware of your surroundings.",
	"", CMD_NONE
    },

    {
       "detect movement", {60, 53, 15, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_null,             TAR_IGNORE,             POS_STANDING,
	&gsn_detect_movement,	0,	5,	12,
	"",			"You slowly stop listening to your surroundings.",
	"", CMD_NONE
    },

    {
	"bluff", {60, 53, 38, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_bluff,		0,	25,	12,
	"",			"Your blood begins to dry up and your true wounds show through.",
	"", CMD_NONE
    },

    {
	"false motives", {60, 53, 47, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_null,             TAR_IGNORE,             POS_STANDING,
	&gsn_false_motives,	0,	25,	12,
	"",			"You begin acting like yourself again.",
	"", CMD_NONE
    },

    {
        "detect invis", {60, 53, 53, 9, 8, 10, 53, 7, 8, 8, 8, 53},
	spell_detect_invis,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			CAN_DISPEL|CAN_CANCEL,	 5,	12,
	"",			"You no longer see invisible objects.",
	"", CMD_BOTH
    },

    {
        "detect magic", {60, 53, 53, 4, 53, 53, 53, 53, 53, 53, 5, 1},
	spell_detect_magic,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			CAN_DISPEL|CAN_CANCEL,	 5,	12,
	"",			"The detect magic wears off.",	"", CMD_BOTH
    },

    {
        "detect poison", {60, 53, 53, 8, 53, 53, 53, 53, 53, 8, 53, 53},
	spell_detect_poison,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			CAN_DISPEL|CAN_CANCEL,	 5,	12,
	"",			"!Detect Poison!",	"", CMD_BOTH
    },

    {
        "dispel evil", {60, 53, 53, 15, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_dispel_evil,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			0,	15,	12,
	"dispel evil",		"!Dispel Evil!",	"", CMD_COMMUNE
    },

    {
        "dispel good", {60, 53, 53, 15, 53, 15, 53, 53, 53, 53, 53, 53},
        spell_dispel_good,      TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,                   0,      15,     12,
        "dispel good",          "!Dispel Good!",	"", CMD_BOTH
    },

    {
        "dispel magic", {60, 53, 53, 20, 53, 53, 53, 53, 15, 53, 18, 18},
	spell_dispel_magic,	TAR_IGNORE,	POS_FIGHTING,
	NULL,			0,	15,	12,
	"",			"!Dispel Magic!",	"", CMD_BOTH
    },

	{
	"energy drain", {60, 53, 53, 25, 53, 27, 53, 53, 53, 53, 19, 53},
	spell_energy_drain, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
	NULL,           0,  30, 12,
	"energy drain",     "The cold chill in your body fades away.", "", CMD_BOTH
	},

	{
   	"bitten leg", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_null,		TAR_IGNORE,	POS_FIGHTING,
	&gsn_lbite,			0,	150,	24,
	"",		"Your leg feels better.",		"", CMD_NONE
    },

    {
        "faerie fire", {60, 53, 53, 11, 53, 53, 53, 3, 5, 3, 3, 53},
	spell_faerie_fire,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_faerie_fire,		0,	 5,	12,
	"faerie fire",		"The pink aura around you fades away.",
	"The pink aura around $n fades away.", CMD_BOTH
    },

    {
        "faerie fog", {60, 53, 53, 14, 53, 53, 53, 53, 16, 18, 17, 53},
	spell_faerie_fog,	TAR_IGNORE,		POS_STANDING,
	&gsn_faerie_fog,			0,	12,	12,
	"faerie fog",		"The purple fog on your body fades away.", "The purple fog on $n fades away.", CMD_BOTH
    },

    {
        "fireball", {60, 53, 53, 53, 53, 22, 53, 53, 27, 53, 53, 53},
	spell_fireball,		TAR_IGNORE,	POS_FIGHTING,
	NULL,			0,	15,	12,
	"fireball",		"!Fireball!",		"", CMD_SPELL
    },

    {
	"voice of damnation", {60, 53, 53, 53, 27, 53, 53, 53, 53, 53, 53, 53},
	spell_voice_of_damnation,	TAR_IGNORE,	POS_STANDING,
	NULL,			0,	45,	24,
	"booming voice",	"","",CMD_COMMUNE
    },
    
    {
	"blinding orb", {60, 53, 53, 53, 20, 53, 53, 53, 53, 53, 53, 53},
	spell_blinding_orb,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_blinding_orb, CAN_CANCEL, 25, 24,
	"radiant light",	"Your vision slowly begins to return.","$n seems to be able to see once again.", CMD_COMMUNE
    },

    {
        "fireproof", {60, 53, 53, 53, 53, 53, 53, 53, 25, 53, 53, 53},
	spell_fireproof,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			0,	10,	12,
	"",			"",	"$p's protective aura fades.", CMD_SPELL
    },

    {
        "flamestrike", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_flamestrike,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			0,	20,	12,
	"flamestrike",		"!Flamestrike!",		"", CMD_BOTH
    },

    {
        "fly", {60, 53, 53, 19, 53, 53, 53, 53, 18, 17, 53, 16},
	spell_fly,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			CAN_DISPEL|CAN_CANCEL,	10,	18,
	"",			"You slowly float to the ground.",	"$n slowly floats to the ground.", CMD_BOTH
    },

    {
        "frenzy", {60, 53, 53, 25, 53, 53, 53, 53, 53, 22, 53, 53},
        spell_frenzy,           TAR_CHAR_DEFENSIVE,     POS_STANDING,
        NULL,       CAN_CANCEL|CAN_DISPEL,      30,     24,
        "",                     "Your rage ebbs.",	"", CMD_COMMUNE
    },

    {
        "gate", {60, 53, 53, 53, 53, 53, 53, 53, 53, 10, 53, 53},
	spell_gate,		TAR_IGNORE,		POS_FIGHTING,
	NULL,			0,	80,	12,
	"",			"!Gate!",		"", CMD_COMMUNE
    },

    {
        "giant strength", {60, 53, 53, 53, 53, 53, 53, 53, 24, 53, 53, 53},
	spell_giant_strength,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			CAN_DISPEL|CAN_CANCEL,	20,	12,
	"",			"You feel weaker.",	"", CMD_BOTH
    },

    {
        "harm", {60, 53, 53, 24, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_harm,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			0,	35,	12,
	"harm spell",		"!Harm!",		"", CMD_COMMUNE
    },

    {
        "haste", {60, 53, 53, 53, 53, 53, 53, 53, 23, 53, 53, 53},
	spell_haste,		TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			CAN_DISPEL|CAN_CANCEL,	30,	12,
	"",			"You feel yourself slow down.",	"", CMD_SPELL
    },

    {
        "heal", {60, 53, 53, 53, 27, 53, 53, 53, 53, 17, 53, 53},
	spell_heal,		TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			0,	50,	12,
	"",			"!Heal!",		"", CMD_COMMUNE
    },

    {
        "holy word", {60, 53, 53, 35, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_holy_word,	TAR_IGNORE,	POS_FIGHTING,
	NULL,			0, 	200,	24,
	"divine wrath",		"!Holy Word!",		"", CMD_COMMUNE
    },

    {
        "identify", {60, 53, 53, 14, 53, 53, 53, 19, 15, 21, 14, 15},
	spell_identify,		TAR_OBJ_INV,		POS_STANDING,
	NULL,			0,	12,	18,
	"",			"!Identify!",		"", CMD_BOTH
    },

    {
        "infravision", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 6},
	spell_infravision,	TAR_CHAR_SELF,		POS_STANDING,
	&gsn_infravision,	CAN_DISPEL|CAN_CANCEL,	 10,	12,
	"",			"You can no longer detect nearby heat variations.",	"", CMD_SPELL
    },

    {
        "invisibility", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_invis,		TAR_OBJ_CHAR_DEF,	POS_STANDING,
	&gsn_invis,		CAN_DISPEL|CAN_CANCEL,	 5,	12,
	"",			"You are no longer invisible.",		
	"$n fades into view.", CMD_SPELL
    },

    {
        "know alignment", {60, 53, 53, 53, 3, 53, 53, 53, 53, 8, 53, 53},
	spell_know_alignment,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			0,	 9,	12,
	"",			"!Know Alignment!",	"", CMD_COMMUNE
    },

    {
        "locate object", {60, 53, 53, 20, 53, 53, 53, 53, 20, 24, 20, 20},
	spell_locate_object,	TAR_IGNORE,		POS_STANDING,
	NULL,			0,	20,	18,
	"",			"!Locate Object!",	"", CMD_BOTH
    },

    {
        "magic missile", {60, 53, 53, 53, 53, 1, 53, 53, 1, 53, 53, 1},
	spell_magic_missile,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			0,	15,	12,
	"magic missiles",	"!Magic Missile!",	"", CMD_SPELL
    },

    {
        "mass healing", {60, 53, 53, 53, 53, 53, 53, 53, 53, 35, 53, 53},
	spell_mass_healing,	TAR_IGNORE,		POS_FIGHTING,
	NULL,			0,	100,	36,
	"",			"!Mass Healing!",	"", CMD_COMMUNE
    },

    {
        "mass invis", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_mass_invis,	TAR_IGNORE,		POS_STANDING,
	&gsn_mass_invis,	CAN_DISPEL|CAN_CANCEL,	20,	24,
	"",			"You are no longer invisible.",		"", CMD_BOTH
    },

    {
        "pass door", {60, 53, 53, 36, 53, 53, 53, 38, 30, 26, 53, 53},
	spell_pass_door,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			CAN_DISPEL|CAN_CANCEL,	20,	12,
	"",			"You feel solid again.",	"", CMD_BOTH
    },

    {
        "plague", {60, 53, 53, 21, 53, 25, 53, 53, 53, 53, 25, 53},
	spell_plague,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_plague,		CAN_CLEANSE,	20,	12,
	"sickness",		"Your sores begin to heal.",	"", CMD_BOTH
    },

    {
        "poison", {60, 53, 53, 10, 53, 14, 53, 53, 53, 53, 17, 53},
	spell_poison,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_poison,		CAN_PURIFY|CAN_CLEANSE,	10,	12,
	"poison",		"You feel less sick.",	
	"$n looks less ill.", CMD_BOTH
    },

	{
		"mana drain", {60, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61},
		spell_null,		TAR_IGNORE,			POS_DEAD,
		&gsn_mana_drain,		0,				0,	0,
		"mana drain",	"You feel more energetic.",		"",		CMD_NONE
	},


    {
	"power word kill", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_power_word_kill,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	NULL,			0,	150,	24,
	"word of power",	"PowerWordKill!",	"", CMD_SPELL
    },

    {
        "protection", {60, 53, 53, 53, 6, 15, 53, 53, 53, 8, 53, 53},
        spell_protection,  	TAR_CHAR_SELF,          POS_STANDING,
        NULL,             CAN_CANCEL|CAN_DISPEL,       5,     12,
        "",                     "You feel less protected.",	"", CMD_BOTH
    },

    {
        "ray of truth", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_ray_of_truth,     TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,                   0,      20,     12,
        "ray of truth",         "!Ray of Truth!",	"", CMD_COMMUNE
    },

    {
        "recharge", {60, 53, 53, 53, 53, 53, 53, 53, 27, 53, 53, 53},
	spell_recharge,		TAR_OBJ_INV,		POS_STANDING,
	NULL,			0,	60,	24,
	"",			"!Recharge!",		"", CMD_SPELL
    },

    {
        "refresh", {60, 53, 53, 7, 53, 53, 53, 53, 53, 6, 53, 53},
	spell_refresh,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			0,	12,	18,
	"refresh",		"!Refresh!",		"", CMD_BOTH
    },

    {
        "remove curse", {60, 53, 53, 20, 15, 53, 53, 53, 53, 12, 53, 53},
	spell_remove_curse,	TAR_OBJ_CHAR_DEF,	POS_STANDING,
	NULL,			0,	 5,	12,
	"",			"!Remove Curse!",	"", CMD_COMMUNE
    },

    {
        "sanctuary", {60, 53, 53, 53, 53, 53, 53, 53, 29, 21, 53, 53},
	spell_sanctuary,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_sanctuary,		CAN_CANCEL|CAN_DISPEL,	70,	12,
	"",			"The protective aura around your body fades.",
	"The protective aura around $n's body fades.", CMD_BOTH
    },

    {
        "shield", {60, 53, 53, 53, 53, 53, 27, 53, 25, 15, 53, 53},
	spell_shield,		TAR_CHAR_SELF,	POS_STANDING,
	&gsn_shield,			CAN_DISPEL|CAN_CANCEL,	12,	12,
	"",			"Your force shield shimmers then fades away.",
	"", CMD_BOTH
    },

    {
        "shocking grasp", {60, 53, 53, 53, 53, 53, 53, 53, 5, 53, 53, 53},
	spell_shocking_grasp,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			0,	15,	12,
	"shocking grasp",	"!Shocking Grasp!",	"", CMD_SPELL
    },

    {
        "sleep", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_sleep,		TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_sleep,		CAN_CANCEL,	15,	12,
	"",			"You feel less tired.",	"", CMD_SPELL
    },

    {
        "slow", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_slow,             TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,                   CAN_CLEANSE|CAN_DISPEL|CAN_CANCEL,      30,     12,
        "",                     "You feel yourself speed up.",	"", CMD_SPELL
    },

    {
        "stoneskin", {60, 53, 53, 53, 53, 53, 53, 30, 29, 20, 53, 26},
	spell_stone_skin,	TAR_CHAR_SELF,		POS_STANDING,
	&gsn_stoneskin,			CAN_DISPEL|CAN_CANCEL,	50,	12,
	"",			"Your skin softens and returns to its normal state.",	"", CMD_SPELL
    },

    {
        "summon", {60, 53, 53, 15, 21, 21, 53, 53, 53, 53, 27, 53},
	spell_summon,		TAR_IGNORE,		POS_STANDING,
	NULL,			0,	50,	27,
	"",			"!Summon!",		"", CMD_BOTH
    },

    {
        "teleport", {60, 53, 53, 53, 53, 22, 53, 53, 15, 19, 15, 16},
	spell_teleport,		TAR_CHAR_SELF,		POS_STANDING,
	NULL,	 		0,	35,	12,
	"",			"!Teleport!",		"", CMD_BOTH
    },

    {
        "ventriloquate", {60, 53, 53, 53, 53, 53, 53, 53, 4, 53, 53, 53},
	spell_ventriloquate,	TAR_IGNORE,		POS_STANDING,
	NULL,			0,	 5,	12,
	"",			"!Ventriloquate!",	"", CMD_BOTH
    },

    {
        "weaken", {60, 53, 53, 8, 53, 27, 53, 53, 53, 53, 13, 53},
	spell_weaken,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			CAN_CLEANSE|CAN_DISPEL|CAN_CANCEL,	20,	12,
	"spell",		"You feel stronger.",	"", CMD_BOTH
    },

    {
	"web", {60, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30},
	spell_web,		TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_web,			0,	75,	12,
	"corrosive web",	"The sticky strands about your body dissolve.",	"", CMD_POWER
    },


    {
        "word of recall", {60, 53, 53, 28, 30, 53, 53, 53, 29, 27, 33, 30},
		spell_word_of_recall,	TAR_CHAR_DEFENSIVE, POS_STANDING,
		NULL,			0,	 5,	24,
		"",			"!Word of Recall!",	"", CMD_BOTH
    },

    {
        "protective shield", {60, 53, 53, 16, 53, 53, 53, 53, 17, 13, 53, 53},
        spell_protective_shield,      TAR_CHAR_SELF,     POS_STANDING,
        &gsn_protective_shield,       CAN_DISPEL|CAN_CANCEL,      75,     12,
        "",          "Your protective shield fades away.","", CMD_BOTH
    },

    {
        "summon nephilim", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_summon_nephilim,      TAR_IGNORE,     POS_STANDING,
        NULL,                   0,      400,     36,
        "",          "","", CMD_SPELL
    },

    {
        "consecrate", {60, 53, 53, 53, 53, 53, 53, 53, 53, 35, 53, 53},
        spell_consecrate,      TAR_IGNORE,     POS_STANDING,
        &gsn_consecrate,                   0,      90,     60,
        "",          "You feel holy enough to consecrate more ground.","", CMD_COMMUNE
    },

    {
	"utter heal", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_utter_heal, TAR_CHAR_DEFENSIVE, POS_STANDING,
	NULL, 0,	75, 12,
	"", "", "", CMD_COMMUNE
    },

    {
	"strike of virtue", {60, 53, 53, 53, 23, 53, 53, 53, 53, 53, 53, 53},
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_strike_of_virtue,	0,0,24,
	"strike of virtue","","",CMD_NONE
    },

    {
     	"valiant charge", {60, 53, 53, 53, 45, 53, 53, 53, 53, 53, 53, 53},
	spell_null,	TAR_IGNORE,	POS_STANDING,
	NULL,0,0,36,
	"","","",CMD_NONE
    },
    
    {
	"awe", {60, 53, 53, 53, 33, 53, 53, 53, 53, 53, 53, 53},
	spell_awe,	TAR_IGNORE,	POS_STANDING,
	&gsn_awe,0,90,36,
	"","Your awe-inspiring aura fades away.","$n seems less powerful.", CMD_COMMUNE
    },

    {
	"divine frenzy", {60, 53, 53, 53, 51, 53, 53, 53, 53, 53, 53, 53},
	spell_divine_frenzy,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_divine_frenzy,	CAN_DISPEL|CAN_CANCEL,200,24,
	"","Your righteous rage begins to fade.", "$n seems to calm down.", CMD_COMMUNE
    },

	{
		"indomitable spirit", {60, 53, 53, 53, 41, 53, 53, 53, 53, 53, 53, 53},
		spell_indomitable_spirit,	TAR_CHAR_SELF,	POS_STANDING,
		&gsn_indomitable_spirit,	CAN_DISPEL|CAN_CANCEL,	100, 36,
		"","Your indomitable spirit fades.","", CMD_COMMUNE
	},

    {
	"arms of wrath", {60, 53, 53, 53, 25, 53, 53, 53, 53, 53, 53, 53},
	spell_arms_of_wrath,	TAR_OBJ_INV,	POS_STANDING,
	&gsn_arms_of_wrath,	0,45,24,
	"","You feel able to imbue another weapon with holy wrath once again.","",CMD_COMMUNE
    },

    {
	"arms of purity", {60, 53, 53, 53, 35, 53, 53, 53, 53, 53, 53, 53},
	spell_arms_of_purity,	TAR_OBJ_INV,	POS_STANDING,
	&gsn_arms_of_purity,	0,45,24,
	"","You feel able to imbue another weapon with purity.","",CMD_COMMUNE
    },

    {
	"arms of judgement", {60, 53, 53, 53, 45, 53, 53, 53, 53, 53, 53, 53},
	spell_arms_of_judgement,	TAR_OBJ_INV,	POS_STANDING,
	&gsn_arms_of_judgement,	0,45,24,
	"","You feel to imbue another weapon with divine judgement.","",CMD_COMMUNE
    },

    {
	"arms of light", {60, 53, 53, 53, 15, 53, 53, 53, 53, 53, 53, 53},
	spell_arms_of_light, TAR_OBJ_INV, POS_STANDING,
	&gsn_arms_of_light, 0, 45, 24,
	"","You feel able to imbue a weapon with light once again.","", CMD_COMMUNE
    },

    {
	"seraphic mantle", {60, 53, 53, 53, 38, 53, 53, 53, 53, 53, 53, 53},
	spell_seraphic_mantle, TAR_CHAR_DEFENSIVE, POS_STANDING,
	&gsn_seraphic_mantle, CAN_DISPEL|CAN_CANCEL, 65, 12,
	"","The ward of light surrounding you fades.", "The ward of light surrounding $n fades.",CMD_COMMUNE
    },

    {
	"shield of faith", {60, 53, 53, 53, 16, 53, 53, 53, 53, 53, 53, 53},
	spell_shield_of_faith,	TAR_CHAR_DEFENSIVE, POS_STANDING,
	&gsn_shield_of_faith,	CAN_DISPEL|CAN_CANCEL, 45, 12,
	"","The pearly shield slowly breaks apart and fades into nothingness.", "The pearly shield surrounding $n slowly breaks apart and fades into nothingness.", CMD_COMMUNE
    },

    {
	"holy shroud", {60, 53, 53, 53, 33, 53, 53, 53, 53, 53, 53, 53},
	spell_holy_shroud,	TAR_CHAR_DEFENSIVE, POS_STANDING,
	&gsn_holy_shroud,	CAN_DISPEL|CAN_CANCEL,	55,24,
	"","The veil of mist surrounding you silently dissipates.", "The veil of luminescent mist surrounding $n silently dissipates.", CMD_COMMUNE
    },

	{
		"tower of fortitude", {60, 53, 53, 53, 44, 53, 53, 53, 53, 53, 53, 53},
		spell_tower_of_fortitude,	TAR_CHAR_SELF, POS_STANDING,
		&gsn_tower_of_fortitude,	CAN_DISPEL|CAN_CANCEL, 75, 24,
		"","Your tower of fortitude subsides.", "", CMD_COMMUNE
	},

    {
	"atrophy", {60, 53, 53, 38, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_atrophy, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
	&gsn_atrophy, 0,	25, 12,
	"atrophy",	"",	"", CMD_COMMUNE
    },

	{
		"healing sleep", {60, 53, 53, 53, 53, 53, 53, 53, 53, 30, 53, 53},
		spell_healing_sleep, TAR_CHAR_DEFENSIVE, POS_STANDING,
		&gsn_healing_sleep,	0,	25,	12,
		"", "", "", CMD_COMMUNE
	},
/*
 * Dragon breath
 */
    {
        "acid breath", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_acid_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			0,	100,	24,
	"blast of acid",	"!Acid Breath!",	"", CMD_SPELL
    },

    {
        "fire breath", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_fire_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			0,	200,	24,
	"blast of flame",	"The smoke leaves your eyes.",	"", CMD_SPELL
    },

    {
        "frost breath", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_frost_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			0,	125,	24,
	"blast of frost",	"!Frost Breath!",	"", CMD_SPELL
    },

    {
        "gas breath", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_gas_breath,	TAR_IGNORE,		POS_FIGHTING,
	NULL,			0,	175,	24,
	"blast of gas",		"!Gas Breath!",		"", CMD_SPELL
    },

    {
        "lightning breath", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_lightning_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			0,	150,	24,
	"blast of lightning",	"!Lightning Breath!",	"", CMD_SPELL
    },

	{
		"nether breath", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
		spell_nether_breath,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
		NULL,		0,		100,		24,
		"blast of nether",	"!Nether Breath!",		"",	CMD_SPELL
	},

    {
        "hold person", {60, 53, 53, 53, 53, 53, 53, 53, 30, 53, 53, 53},
        spell_hold_person,      TAR_CHAR_OFFENSIVE,   POS_STANDING,
        &gsn_hold_person,       CAN_CLEANSE,       100,     24,
        "",          "The stiffness in your body wears off.","", CMD_SPELL
    },

/* combat and weapons skills */


    {
        "axe", {60, 1, 53, 1, 10, 1, 1, 53, 53, 53, 53, 53},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_axe,            	0,       0,      0,
        "",                     "!Axe!",		"", CMD_NONE
    },

    {
        "dagger", {60, 1, 1, 53, 53, 1, 1, 1, 1, 53, 1, 1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_dagger,            0,       0,      0,
        "",                     "!Dagger!",		"", CMD_NONE
    },

    {
        "flail", {60, 1, 53, 1, 1, 1, 53, 53, 1, 1, 53, 53},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_flail,            	0,       0,      0,
        "",                     "!Flail!",		"", CMD_NONE
    },

    {
	"mind sear", {60, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61},		
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	NULL,	0,	0,	0,
	"mind sear",	"The burning in your mind fades away.",	"", CMD_NONE
    },

    {
        "mace", {60, 1, 13, 53, 1, 1, 53, 53, 53, 1, 53, 53},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_mace,            	0,       0,      0,
        "",                     "!Mace!",		"", CMD_NONE
    },

    {
        "polearm", {60, 1, 53, 1, 1, 53, 1, 53, 53, 53, 53, 53},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_polearm,           0,       0,      0,
        "",                     "!Polearm!",		"", CMD_NONE
    },

    {
        "shield block", {60, 10, 53, 18, 10, 53, 12, 53, 53, 17, 53, 53},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_shield_block,	0,	0,	0,
	"",			"!Shield!",		"", CMD_NONE
    },

	{
		"intercept",			{ 53, 53, 53, 35, 53, 53, 53, 53, 53, 53 },
		spell_null,				TAR_IGNORE,				POS_FIGHTING,
		&gsn_intercept,			0,		0,		0,
		"",	"",	"", CMD_NONE
	},

	{
		"champions defense",	{ 53, 53, 53, 53, 27, 53, 53, 53, 53, 53 },
		spell_null,				TAR_IGNORE,				POS_FIGHTING,
		&gsn_champions_defense,	0,		0,		0,
		"", "", "", CMD_NONE
	},

    {
        "spear", {60, 1, 53, 53, 53, 1, 1, 53, 53, 53, 53, 53},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_spear,            	0,       0,      0,
        "",                     "!Spear!",		"", CMD_NONE
    },

    {
	"dual wield", {60, 12, 30, 53, 17, 18, 16, 28, 53, 53, 53, 53},
	spell_null,		TAR_IGNORE,	POS_FIGHTING,
	&gsn_dual_wield,	0,	0,	0,
	"", "", "", CMD_NONE
    },

    {
        "sword", {60, 1, 22, 1, 1, 1, 1, 1, 53, 53, 1, 53},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_sword,            	0,       0,      0,
        "",                     "!sword!",		"", CMD_NONE
    },

    {
        "whip", {60, 1, 53, 53, 53, 53, 1, 1, 1, 1, 1, 1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_whip,            	0,       0,      0,
        "",                     "!Whip!",	"", CMD_NONE
    },

    {
	"counter", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_null,	TAR_IGNORE,	POS_STANDING,
	&gsn_counter,	0,	0,	0,
	"counter strike",	"",	"", CMD_NONE
    },

    {
        "tame", {60, 53, 53, 53, 53, 53, 25, 53, 53, 53, 53, 53},
        spell_null,     TAR_IGNORE,     POS_FIGHTING,
        &gsn_tame,      0,        0,      0,
        "",     "", CMD_NONE
    },

    {
        "find water", {60, 53, 53, 53, 53, 53, 18, 53, 53, 53, 53, 53},
        spell_null,     TAR_IGNORE,     POS_STANDING,
        &gsn_find_water,        0,        0,      0,
        "",      "", CMD_NONE
     },

     {
        "shield cleave", {60, 18, 53, 53, 53, 22, 26, 53, 53, 53, 53, 53},
        spell_null,     TAR_IGNORE,     POS_FIGHTING,
        &gsn_shield_cleave, 0,    0,      0,
        "",     "", CMD_NONE
     },

     {
        "darkshield", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_dark_shield,     TAR_CHAR_DEFENSIVE,     POS_STANDING,
        NULL,   0,      20,     12,
        "",     "The dark shield surrounding you fades away.", "", CMD_SPELL
    },

	{
		"somatic casting", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 45, 45},
		spell_null,				TAR_IGNORE,				POS_DEAD,
		&gsn_somatic_casting,		0,	0,	0,
		"",		"",		"",		CMD_NONE
	},

    {
        "spellcraft", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 1, 1},
        spell_null,     TAR_IGNORE,     POS_SLEEPING,
        &gsn_spellcraft,        0,        0,      0,
        "",     "", "", CMD_NONE
    },



    {
        "power word fear", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_power_word_fear,  TAR_CHAR_OFFENSIVE, POS_FIGHTING,
        NULL,   0,  20, 12,
        "", "You feel the fear plaguing your mind fade away.",
        "", CMD_SPELL
    },

	{
        "preserve", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 20, 53},
        spell_preserve, TAR_OBJ_INV,    POS_STANDING,
        NULL,   0,  10, 12,
        "", "", "", CMD_SPELL
    },

    {
        "imbue regeneration", {60, 53, 53, 53, 53, 53, 53, 53, 53, 35, 53, 53},
        spell_imbue_regeneration, TAR_CHAR_DEFENSIVE, POS_STANDING,
        NULL,   0,  50, 12,
        "", "Your body stops regenerating.",    "", CMD_COMMUNE
    },

    {
        "restoration", {60, 53, 53, 53, 53, 53, 53, 53, 53, 19, 53, 53},
        spell_restoration,  TAR_CHAR_DEFENSIVE, POS_STANDING,
        NULL,   0,  50, 12,
        "", "", "", CMD_COMMUNE
    },


    {
	"quiet movement", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_null,	TAR_IGNORE, POS_STANDING,
	&gsn_quiet_movement, 0, 0, 0,
	"", "You stop moving quietly.", "", CMD_NONE
    },
    {
	"iron resolve", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_null,	TAR_IGNORE, POS_STANDING,
	&gsn_iron_resolve, 0, 0, 0,
	"", "Your mind wanders from its resolve.", "", CMD_NONE
    },

    {
	"ambush", {60, 53, 53, 53, 53, 53, 12, 53, 53, 53, 53, 53},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_ambush,		0,	0,	24,
	"surprise attack",	"!Ambush!",	"", CMD_NONE
    },

    {
	"moving ambush", {60, 53, 53, 53, 53, 53, 35, 53, 53, 53, 53, 53},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_moving_ambush,		0,	0,	0,
	"surprise attack",	"!Ambush!",	"", CMD_NONE
    },


        {
        "pugil", {60, 22, 53, 53, 53, 53, 24, 20, 53, 18, 53, 53},
        spell_null,     TAR_IGNORE,     POS_FIGHTING,
        &gsn_pugil,     0,        0,      12,
        "pugil",        "",     "", CMD_NONE
        },

        {
        "lash", {60, 15, 53, 24, 53, 53, 19, 25, 17, 19, 53, 25},
        spell_null,     TAR_IGNORE,     POS_FIGHTING,
        &gsn_lash,      0,        0,      12,
        "lash", "",     "", CMD_NONE
        },

        {
        "protection heat cold", {60, 53, 53, 53, 53, 53, 18, 53, 53, 53, 53, 53},
        spell_null,     TAR_IGNORE,     POS_FIGHTING,
        &gsn_protection_heat_cold,      0,        0,      12,
        "",     "You no longer feel protected from the elements.", "", CMD_NONE
        },

    {
        "enhanced damage", {60, 1, 40, 29, 5, 1, 15, 18, 53, 53, 53, 53},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_enhanced_damage,   0,        0,     0,
        "",                     "!Enhanced Damage!",	"", CMD_NONE
    },

    {
	"group retreat", {60, 53, 53, 53, 38, 53, 53, 53, 53, 53, 53, 53},
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_group_retreat,	0,	0,	12,
	"","","",CMD_NONE
    },
        {
        "brutality", {60, 20, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,     TAR_IGNORE,     POS_FIGHTING,
        &gsn_brutality,       0,        0,      0,
        "",     "",     "", CMD_NONE
        },

        {
        "staff", {60, 1, 53, 53, 53, 53, 1, 1, 53, 1, 1, 1},
        spell_null,     TAR_IGNORE,     POS_FIGHTING,
        &gsn_staff,     0,        0,      0,
        "",     "",                "", CMD_NONE
        },

        {
        "tactics", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,     TAR_IGNORE,     POS_FIGHTING,
        &gsn_tactics,   0,        0,      0,
        "",     "",     "", CMD_NONE
        },

        {
        "rally", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,     TAR_IGNORE,     POS_FIGHTING,
        &gsn_rally,     0,        0,      0,
        "",     "Your confidence to incite others has returned.",       "", CMD_NONE
        },


    {
        "backstab", {60, 53, 53, 53, 53, 53, 53, 10, 53, 53, 53, 53},
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_backstab,          0,        0,     24,
        "backstab",             "!Backstab!",		"", CMD_NONE
    },

    {
	"strange form", {60, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61},
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_strange_form,	0,	0,	0,
	"",	"You feel your normal form returning.",	"You feel your normal form returning.", CMD_NONE
    },

    {
	"cloak", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_null,	TAR_IGNORE,	POS_STANDING,
	&gsn_cloak_form,	0, 	0,	0,
	"",	"The wind pulls your cloak away from your face.", "", CMD_POWER
    },
			
    {
        "bite", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null, TAR_IGNORE, POS_FIGHTING,
        &gsn_chimera_lion,  0,   0,  0,
        "crushing bite",    "", "", CMD_NONE
    },

    {
        "ram", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null, TAR_IGNORE, POS_FIGHTING,
        &gsn_chimera_goat,  0,   0,  0,
        "ramming",  "", "", CMD_NONE
    },


    {
	"tail", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_tail,	0,	0,	12,
	"tail attack",	"", "", CMD_NONE
    },

    {
	"throw", {60, 53, 53, 53, 53, 53, 53, 15, 53, 53, 53, 53},
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_throw,	0,	0,	12,
	"throw",	"",	"", CMD_NONE
    },

    {
	"endure", {60, 53, 53, 53, 53, 53, 53, 11, 53, 53, 53, 53},
	spell_null,	TAR_IGNORE,	POS_STANDING,
	&gsn_endure,	0,	0,	12,
	"",	"You feel more susceptible to magic.",	"", CMD_NONE
    },

    {
	"poison dust", {60, 53, 53, 53, 53, 53, 53, 14, 53, 53, 53, 53},
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_poison_dust,	0,	0,	0,
	"poison",	"The poison in your blood is neutralised.", "", CMD_NONE
    },

    {
	"blindness dust", {60, 53, 53, 53, 53, 53, 53, 23, 53, 53, 53, 53},
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_blindness_dust,	0,	0,	0,
	"",	"You rub the dust in your eyes away.", "", CMD_NONE
    },

/*
    {
        "strangle timer", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,  TAR_IGNORE, POS_STANDING,
        &gsn_strangle_timer,    0,0,  0,
        "", "You stop guarding your neck so carefully.",    ""
    },
*/
    {
	"strangle", {60, 53, 53, 53, 53, 53, 53, 25, 53, 53, 53, 53},
	spell_null,	TAR_IGNORE, POS_STANDING,
	&gsn_strangle,	0,	0,	0,
	"strangulation attempt",	"Your neck feels better.", "", CMD_NONE
    },


    {
	"nerve", {60, 53, 53, 53, 53, 53, 53, 13, 53, 53, 53, 53},
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_nerve,	0,	0,	0,
	"nerve",	"Your arm loses the numbness within it.",	"", CMD_NONE
    },

    {
        "bash", {60, 15, 53, 53, 53, 13, 53, 15, 53, 53, 53, 53},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_bash,            	0,       0,      24,
        "bash",                 "!Bash!",		"", CMD_NONE
    },

    {
        "berserk", {60, 18, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_berserk,        	0,       0,      24,
        "",                     "You feel your pulse slow down.",	"", CMD_NONE
    },

    {
	"ground control", {60, 53, 53, 53, 53, 53, 53, 27, 53, 53, 53, 53},
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_ground_control,	0,	0,	0,
	"ground control",	"",	"", CMD_NONE
    },

    {
        "circle stab", {60, 53, 15, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_circle,            	0,       0,      18,
        "circle stab",                 "!Circlestab!",		"", CMD_NONE
    },

    {
	"warcry", {60, 20, 53, 20, 53, 53, 20, 53, 53, 53, 53, 53},
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_warcry,	0,	0, 0,
	"",	"The effects of your warcry die out.",	"", CMD_NONE
    },


    {
        "dirt kicking", {60, 3, 2, 53, 53, 4, 5, 4, 53, 53, 53, 53},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_dirt,		0,	0,	24,
	"kicked dirt",		"You rub the dirt out of your eyes.",	"$n rubs the dirt out of $s eyes.", CMD_NONE
    },

    {
        "disarm", {60, 10, 18, 53, 15, 16, 17, 15, 53, 53, 53, 53},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_disarm,            0,        0,     24,
        "",                     "!Disarm!",		"", CMD_NONE
    },

    {
        "dodge", {60, 13, 1, 53, 53, 20, 14, 1, 53, 53, 53, 53},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_dodge,             0,        0,     0,
        "",                     "!Dodge!",		"", CMD_NONE
    },

    {
	"dual backstab", {60, 53, 53, 53, 53, 53, 53, 30, 53, 53, 53, 53},
    	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_dual_backstab,	0,	0,	0,
	"second backstab",	"!DualBackstab!",	"", CMD_NONE
    },

    {
        "envenom", {60, 53, 53, 53, 53, 53, 53, 25, 53, 53, 53, 53},
	spell_null,		TAR_IGNORE,	  	POS_RESTING,
	&gsn_envenom,		0,	0,	36,
	"",			"!Envenom!",		"", CMD_NONE
    },

    {
        "hand to hand", {60, 1, 1, 11, 10, 6, 9, 3, 15, 13, 12, 5},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_hand_to_hand,	0,	0,	0,
	"",			"!Hand to Hand!",	"", CMD_NONE
    },

    {
        "kick", {60, 8, 6, 12, 9, 10, 11, 6, 53, 15, 53, 53},
        spell_null,             TAR_IGNORE,     POS_FIGHTING,
        &gsn_kick,              0,        0,     12,
        "kick",                 "!Kick!",		"", CMD_NONE
    },

    {
        "parry", {60, 1, 13, 7, 1, 1, 1, 20, 18, 21, 18, 16},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_parry,             0,        0,     0,
        "",                     "!Parry!",		"", CMD_NONE
    },

    {
        "rescue", {60, 1, 53, 53, 1, 53, 53, 53, 53, 53, 53, 53},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_rescue,            0,        0,     12,
        "",                     "!Rescue!",		"", CMD_NONE
    },

    {
        "trip", {60, 15, 8, 53, 53, 13, 11, 10, 53, 53, 53, 53},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_trip,		0,	0,	24,
	"trip",			"!Trip!",		"", CMD_NONE
    },

    {
        "second attack", {60, 6, 14, 24, 7, 7, 8, 7, 28, 25, 21, 26},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_second_attack,     0,        0,     0,
        "",                     "!Second Attack!",	"", CMD_NONE
    },

    {
        "third attack", {60, 12, 35, 53, 18, 17, 25, 25, 53, 53, 53, 53},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_third_attack,      0,        0,     0,
        "",                     "!Third Attack!",	"", CMD_NONE
    },

    {
        "fourth attack", {60, 30, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_fourth_attack,      0,        0,     0,
        "",                     "!Fourth Attack!",	"", CMD_NONE
    },

	{
		"fifth attack", {60, 36, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
		spell_null,				TAR_IGNORE,				POS_FIGHTING,
		&gsn_fifth_attack,		0,		0, 		0,
		"",						"!Fifth Attack!",	"",	CMD_NONE
	},

/* non-combat skills */

    {
	"acute vision", {60, 53, 53, 53, 53, 53, 11, 53, 53, 53, 53, 53},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_acute_vision,	0,	0,	12,
	"",			"Your sight dulls as your awareness fades.",	"", CMD_NONE

    },

    {
	"barkskin", {60, 53, 53, 53, 53, 53, 18, 53, 53, 53, 53, 53},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_barkskin,		0,	0,	12,
	"",	"The bark on your skin peels off and falls aways.", "", CMD_NONE
    },

    {
	"enlist", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_null,	TAR_IGNORE,	POS_STANDING,
	&gsn_enlist,	0,	0,	0,
	"",	"You decide to follow your own destiny and leave your company.", "", CMD_NONE
    },

    {
	"animal call", {60, 53, 53, 53, 53, 53, 20, 53, 53, 53, 53, 53},
	spell_null,	TAR_IGNORE,	POS_STANDING,
	&gsn_animal_call,	0,	0,	36,
	"", "You feel up to calling more animals.", "", CMD_NONE
    },

    {
	"bear call", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_null,	TAR_IGNORE,	POS_STANDING,
	&gsn_bear_call,	0,	0,	12,
	"", "You feel up to calling more bears.", "", CMD_NONE
    },

    {
	"camouflage", {60, 53, 53, 53, 53, 53, 8, 53, 53, 53, 53, 53},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_camouflage,	0,	0,	12,
	"",			"!Camouflage!",		"", CMD_NONE
    },

    {
	"camp", {60, 53, 53, 53, 53, 53, 20, 53, 53, 53, 53, 53},
	spell_null,		TAR_IGNORE,		POS_RESTING,
	&gsn_camp,		0,	0,	48,
	"",			"!Camp!",		"", CMD_NONE
    },

    {
        "fast healing", {60, 7, 12, 9, 8, 14, 15, 8, 17, 10, 14, 8},
	spell_null,		TAR_IGNORE,		POS_SLEEPING,
	&gsn_fast_healing,	0,	0,	0,
	"",			"!Fast Healing!",	"", CMD_NONE
    },

    {
        "haggle",               { 14, 7, 11, 53, 53, 14, 13, 17, 9, 10},
	spell_null,		TAR_IGNORE,		POS_RESTING,
	&gsn_haggle,		0,	0,	0,
	"",			"!Haggle!",		"", CMD_NONE
    },

	{
		"benevolence", {60, 53, 53, 53, 14, 53, 53, 53, 53, 53, 53, 53},
		spell_null,	TAR_IGNORE,		POS_STANDING,
		&gsn_benevolence,	0,	0,	0,
		"",		"!Benevolence!", "", CMD_NONE
	},

    {
        "hide", {60, 53, 3, 53, 53, 53, 53, 10, 53, 53, 53, 53},
	spell_null,		TAR_IGNORE,		POS_RESTING,
	&gsn_hide,		0,	 0,	12,
	"",			"!Hide!",		"", CMD_NONE
    },

    {
	"laying hands", {60, 53, 53, 53, 13, 53, 53, 53, 53, 53, 53, 53},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_laying_hands,	0,	35, 	36,
	"searing hands",	"You feel able to heal with your touch again.",	"", CMD_NONE
    },

    {
	"turn undead", {60, 53, 53, 53, 25, 53, 53, 53, 53, 53, 53, 53},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_turn_undead, 	0,	25,	25,
	"holy flames",		"You feel able to turn the undead once again.","", CMD_NONE
    },

    {
        "lore", {60, 21, 18, 53, 17, 20, 18, 53, 53, 53, 53, 53},
	spell_null,		TAR_IGNORE,		POS_RESTING,
	&gsn_lore,		0,	0,	36,
	"",			"!Lore!",		"", CMD_NONE
    },

    {
        "meditation", {60, 16, 53, 6, 8, 8, 13, 2, 7, 6, 11, 7},
	spell_null,		TAR_IGNORE,		POS_SLEEPING,
	&gsn_meditation,	0,	0,	0,
	"",			"Meditation",		"", CMD_NONE
    },

    {
	"trance", {60, 53, 53, 15, 53, 53, 53, 14, 15, 16, 14, 14},
	spell_null,	TAR_IGNORE,	POS_SLEEPING,
	&gsn_trance,	0,	0,	0,
	"",	"",	"", CMD_NONE
    },

	{
		"channeling", {60, 53, 53, 53, 51, 53, 53, 14, 15, 16, 14, 14},
		spell_null, TAR_IGNORE, POS_SLEEPING,
		&gsn_channeling,	0,	0,	0,
		"", "", "", CMD_NONE
	},

	{
		"shadowsense", {60, 53, 53, 53, 32, 53, 53, 14, 15, 16, 14, 14},
		spell_null, TAR_IGNORE, POS_SLEEPING,
		&gsn_shadowsense,	0,	0,	0,
		"", "", "", CMD_NONE
	},

	{
        "black guard", {60, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61},
        spell_null,     TAR_IGNORE,     POS_SLEEPING,
        &gsn_blackjack_timer,   0,	0,  0,
        "", "You stop guarding your head so carefully.",    "", CMD_NONE
    },
    {
        "blackjack", {60, 53, 21, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,     TAR_IGNORE,     POS_STANDING,
        &gsn_blackjack,  0,       0,      24,
        "blackjack",    "You come to with a pounding headache.", "", CMD_NONE
    },

	{
        "peek", {60, 53, 11, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_peek,		0,	 0,	 0,
	"",			"!Peek!",		"", CMD_NONE
    },

    {
        "pick lock", {60, 53, 11, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_pick_lock,		0,	 0,	12,
	"",			"!Pick!",		"", CMD_NONE
    },

    {
	"skin", {60, 53, 53, 53, 53, 53, 22, 53, 53, 53, 53, 53},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_skin,		0,	0,	24,
	"",			"You feel up to skinning and fashioning another sack.", "", CMD_NONE
    },

    {
        "sneak", {60, 53, 4, 53, 53, 53, 53, 5, 53, 53, 53, 53},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_sneak,		0,	 0,	12,
	"",			"You no longer feel stealthy.",	"", CMD_NONE
    },

    {
        "steal", {60, 53, 5, 53, 53, 25, 53, 53, 53, 53, 53, 53},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_steal,		0,	 0,	24,
	"",			"!Steal!",		"", CMD_NONE
    },

	{
		"plant", {60, 53, 17, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_plant,		0,	0,	24,
	"",			"!Plant!",		"",	CMD_NONE
	},

	{
		"palm", {60, 53, 8, 53, 53, 53, 53, 53, 53, 53, 53, 53},
		spell_null,	TAR_IGNORE,	POS_STANDING,
		&gsn_palm,	0,	0,	24,
		"",	"!Palm!",	"", CMD_NONE
	},

	{
		"trail", {60, 53, 33, 53, 53, 53, 53, 53, 53, 53, 53, 53},
		spell_null, TAR_IGNORE, POS_STANDING,
		&gsn_trail, 0, 0, 24,
		"", "!Trail!", "", CMD_NONE
	},

	{
		"drag", {60, 53, 30, 53, 53, 53, 53, 53, 53, 53, 53, 53},
		spell_null, 	TAR_IGNORE,	POS_STANDING,
		&gsn_drag,	0,	0,	24,
		"",	"!Drag!",	"", CMD_NONE
	},


	{
		"tripwire", {60, 53, 35, 53, 53, 53, 53, 53, 53, 53, 53, 53},
		spell_null,		TAR_DIR,	POS_STANDING,
		&gsn_tripwire,	0,	0,	24,
		"", "You are able to place another tripwire now.", "", CMD_NONE
	},

	{
		"stash", {60, 53, 24, 53, 53, 53, 53, 53, 53, 53, 53, 53},
		spell_null,		TAR_IGNORE,	POS_STANDING,
		&gsn_stash,	0,	0,	24,
		"", "", "", CMD_NONE
	},

	{
		"disguise", {60, 53, 37, 53, 53, 53, 53, 53, 53, 53, 53, 53},
		spell_null,	TAR_IGNORE, POS_STANDING,
		&gsn_disguise, 0, 0, 24,
		"", "", "", CMD_NONE
	},

	{
		"avoid", {60, 53, 35, 53, 53, 53, 53, 53, 53, 53, 53, 53},
		spell_null,		TAR_IGNORE,	POS_STANDING,
		&gsn_avoid, 0, 0, 24,
		"", "", "", CMD_NONE
	},

	{
		"shadow cloak", {60, 53, 32, 53, 53, 53, 53, 53, 53, 53, 53, 53},
		spell_null, 	TAR_IGNORE, POS_STANDING,
		&gsn_shadow_cloak, 0, 50, 24,
		"", "Your concentration dwindles and you no longer feel cloaked in shadow.", "", CMD_NONE
	},

	{
		"stealth", {60, 53, 41, 53, 53, 53, 53, 53, 53, 53, 53, 53},
		spell_null,	TAR_IGNORE, POS_STANDING,
		&gsn_stealth, 0, 0, 24,
		"", "", "", CMD_NONE
	},

	{
		"counterfeit", {60, 53, 32, 53, 53, 53, 53, 53, 53, 53, 53, 53},
		spell_null, TAR_IGNORE, POS_STANDING,
		&gsn_counterfeit, 0, 0, 24,
		"", "", "", CMD_NONE
	},

	{
		"strip", {60, 53, 45, 53, 53, 53, 53, 53, 53, 53, 53, 53},
		spell_null, TAR_IGNORE, POS_STANDING,
		&gsn_strip, 0, 0, 24,
		"", "", "", CMD_NONE
	},

	{
        "scrolls", {60, 53, 25, 53, 53, 20, 1, 53, 1, 53, 1, 1},
		spell_null,		TAR_IGNORE,		POS_STANDING,
		&gsn_scrolls,		0,	0,	24,
		"",			"!Scrolls!",		"", CMD_NONE
    },

    {
        "staves", {60, 53, 53, 53, 53, 53, 1, 5, 1, 53, 1, 1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_staves,		0,	0,	12,
	"",			"!Staves!",		"", CMD_NONE
    },

    {
        "wands", {60, 53, 53, 53, 53, 28, 53, 53, 1, 53, 1, 1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_wands,		0,	0,	12,
	"",			"!Wands!",		"", CMD_NONE
    },

    {
        "recall", {60, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_recall,		0,	0,	12,
	"",			"You are able to recall again.",
"", CMD_POWER
    },

    {
	"holy fire", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_holy_fire,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,	0,	20,	12,
	"holy fire",	"",	"", CMD_SPELL
    },

    {
	"blade barrier", {60, 53, 53, 53, 53, 53, 53, 53, 53, 37, 53, 53},
	spell_blade_barrier,	TAR_IGNORE,	POS_STANDING,
	&gsn_blade_barrier,	0,	80,	12,
	"blade barrier",	"The whirling blades around you vanish.",	"", CMD_COMMUNE
    },
	
    {
        "prevent healing", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_prevent_healing, TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
        &gsn_prevent_healing,   0,  20, 0,
        "", "You feel your health returning.",  "", CMD_NONE
    },

    {
        "iceball", {60, 53, 53, 53, 53, 29, 53, 53, 53, 53, 53, 53},
        spell_iceball,  TAR_IGNORE,     POS_FIGHTING,
        NULL,   0,      20,     12,
        "iceball",      "", "", CMD_SPELL
    },

    {
		"trophy", {60, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21},
		spell_null,	TAR_IGNORE,	POS_RESTING,
		&gsn_trophy,	0,	0,	0,
		"",	"You feel up to making a new trophy.",	"", CMD_NONE
    },

    {
	"request", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_null,	TAR_IGNORE,	POS_STANDING,	
	&gsn_request,	0,	0,	0,
	"",	"Your contemplation of the good you have seen is over.",
	"", CMD_NONE
    },

    {
        "herb", {60, 53, 53, 53, 53, 53, 10, 53, 53, 53, 53, 53},
        spell_null,     TAR_IGNORE,     POS_STANDING,
        &gsn_herb,       0,       0,      12,
        "", "The herbs look more plentiful.", "", CMD_NONE
    },

    {
        "cleave", {60, 53, 53, 53, 53, 30, 53, 53, 53, 53, 53, 53},
        spell_null,  TAR_IGNORE,     POS_STANDING,
        &gsn_cleave,   0,      0,     12,
        "cleave",      "", "", CMD_NONE
    },

    {
	"vanish", {60, 53, 53, 53, 53, 53, 53, 18, 53, 53, 53, 53},
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_vanish,	0,	0,	12,
	"",	"You feel ready to vanish once again.",	"", CMD_NONE
    },


    {
        "earthbind",    {53,53,53,53,53,53,53,53,53,53},
        spell_earthbind,    TAR_CHAR_OFFENSIVE, POS_FIGHTING,
        &gsn_earthbind, 0,  20, 12,
        "", "Your feet no longer feel bound to the ground.",    "", CMD_SPELL
    },

    {
        "forget", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_forget,   TAR_CHAR_OFFENSIVE, POS_STANDING,
        &gsn_forget,    CAN_CLEANSE,  125, 12,
        "", "Your memories return.",    "", CMD_SPELL
    },

    {
        "divine touch", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_divine_touch,     TAR_CHAR_SELF,  POS_STANDING,
        &gsn_divine_touch,   0, 30, 12,
        "", "Your hands lose their divine touch.",  "", CMD_SPELL
    },

    {
        "transfer object", {60, 53, 53, 53, 53, 53, 53, 53, 32, 53, 53, 53},
        spell_transfer_object,  TAR_IGNORE,    POS_STANDING,
        NULL,   0,  30, 12,
        "", "", "", CMD_SPELL
    },

    {
        "forage", {60, 53, 53, 53, 53, 53, 25, 53, 53, 53, 53, 53},
        spell_null, TAR_IGNORE, POS_STANDING,
        &gsn_forage,    0,    0,  0,
        "", "", "", CMD_NONE
    },

    {
        "assassinate", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null, TAR_IGNORE, POS_STANDING,
        &gsn_assassinate,    0,    0,  0,
        "assassination attempt", "", "", CMD_NONE
    },

    {
        "defend", {60, 53, 53, 53, 22, 53, 53, 53, 53, 53, 53, 53},
        spell_null, TAR_IGNORE, POS_STANDING,
        &gsn_defend,    0,    0,  0,
        "", "", "", CMD_NONE
    },

    {
        "intimidate", {60, 53, 53, 53, 53, 36, 53, 53, 53, 53, 53, 53},
        spell_null, TAR_IGNORE, POS_STANDING,
        &gsn_intimidate,    0,    0,  0,
        "", "", "", CMD_NONE
    },

    {
        "escape", {60, 53, 27, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null, TAR_IGNORE, POS_STANDING,
        &gsn_escape,    0,    0,  0,
        "", "", "", CMD_NONE
    },

    {
        "knife", {60, 53, 28, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_knife,          0,        0,     20,
        "knifing",             "!Knife!",           "", CMD_NONE
    },

    {
        "disperse", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,     TAR_IGNORE,     POS_STANDING,
        &gsn_disperse,      0,       0,      0,
        "",       "",     "", CMD_POWER
    },

    {
        "bind", {60, 53, 23, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_null,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_bind, 0,	0,	0,
	"",	"You break free of the bindings on your head and legs.",	"", CMD_NONE
    },

	{
		"gag", {60, 53, 28, 53, 53, 53, 53, 53, 53, 53, 53, 53},
		spell_null, TAR_CHAR_OFFENSIVE, POS_STANDING,
		&gsn_gag, 0, 0, 0,
		"", "You tear away the gag from your mouth.",	"", CMD_NONE
	},

    {
        "deafen", {60, 53, 53, 53, 53, 24, 53, 53, 53, 53, 53, 53},
        spell_deafen,       TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        &gsn_deafen,        CAN_CLEANSE|CAN_DISPEL|CAN_CANCEL,      20,     12,
        "",          "The ringing in your ears finally stops.","", CMD_SPELL
    },

    {
        "divine intervention", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,       TAR_IGNORE,     POS_DEAD,
        &gsn_divine_intervention,                   0,      0,     0,
        "",          "You are no longer intervened by the Immortals.","", CMD_NONE
    },

	{
	"creep", {60, 53, 53, 53, 53, 53, 15, 53, 53, 53, 53, 53},
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_creep,	0,	0,	0,
	"",	"You feel less stealthy in the forest.",	"", CMD_NONE
    },
   /*
    {
        "prevent", {60, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51},
	spell_null,	TAR_IGNORE,	POS_STANDING,
	NULL,			0,	50,	0,
	"",	"",	"", CMD_SPELL
    },*/
    {
        "unholy guard", {60, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61},
        spell_null,     TAR_IGNORE,     POS_SLEEPING,
        &gsn_unholy_timer,   0,	0,  0,
        "unholy torments$", "Your corpse can be drained for power again.",    "", CMD_NONE
    },
    {
        "truestrike", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,     TAR_IGNORE,     POS_FIGHTING,
        NULL,   0,	0,  0,
        "", "",    "", CMD_NONE
    },

    {
        "parting blow", {60, 27, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_parting_blow,      0,        0,     23,
        "parting blow",         0,       "", CMD_NONE
    },

	{
        "rejuvenate", {60, 53, 53, 53, 53, 53, 53, 53, 53, 40, 53, 53},
	spell_rejuvenate,		TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			0,	80,	12,
	"",			"!Rejuvenate!",		"", CMD_COMMUNE
    },

    {
        "pincer", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,     TAR_IGNORE,     POS_FIGHTING,
        &gsn_pincer,    0,       0,     24,
        "brutal attack",       "!pincer!",     "", CMD_NONE
    },

    {
	"devour", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,             TAR_IGNORE,             POS_RESTING,
        &gsn_devour,            0,        0,     12,
        "",                     "!Devour!",		"", CMD_NONE
    },


	{
        "keen vision", {60, 53, 44, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,       TAR_IGNORE,     POS_FIGHTING,
        &gsn_keen_vision,                   0,     0,     0,
        "",          "Your eyes feel less sharp.","", CMD_BOTH
    },

    {
        "vampiric touch", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 30, 53},
        spell_vampiric_touch,             TAR_CHAR_OFFENSIVE,             POS_FIGHTING,
        &gsn_vampiric_touch,       0,       60,      24,
        "deathly touch",                     "",       "", CMD_SPELL
    },

    {
	"snare", {60, 53, 53, 53, 53, 53, 40, 53, 53, 53, 53, 53},
	spell_null,	TAR_IGNORE, POS_STANDING,
	&gsn_snare,	0,	75,	12,
	"",	"You free yourself from the snare.", "", CMD_NONE
    },
    {
	"snaretimer", {60, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55},
	spell_null,	TAR_IGNORE, POS_STANDING,
	&gsn_snaretimer,	0,	75,	12,
	"flames of the apocalypse",	"You feel able to construct a new snare.", "", CMD_NONE
    },

    {
	"outfit", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	NULL,	0,	0,	24,
	"outfit",		NULL,	"", CMD_NONE
    },

    {
        "soften", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_soften,            TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        &gsn_soften,          CAN_CLEANSE|CAN_DISPEL|CAN_CANCEL,      70,     12,
        "",                     "You no longer feel so frail.",  "", CMD_SPELL
    },

    {
	"cure deafness", {60, 53, 53, 53, 53, 53, 53, 53, 53, 11, 53, 53},
	spell_cure_deafness,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,	0,	10,	12,
	"",	"",	"", CMD_COMMUNE
    },

    {
	"awaken", {60, 53, 53, 53, 53, 53, 53, 53, 53, 20, 53, 53},
	spell_awaken,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,	0,	40,	24,
	"",	"",	"", CMD_COMMUNE
    },

    {
	"resist heat", {60, 53, 53, 53, 53, 53, 53, 53, 53, 10, 53, 53},
	spell_resist_heat,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,	CAN_DISPEL|CAN_CANCEL,	50,	12,
	"",	"You are no longer resistant to heat.",	"", CMD_COMMUNE
    },

    {
	"resist cold", {60, 53, 53, 53, 53, 53, 53, 53, 53, 12, 53, 53},
	spell_resist_cold,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,	CAN_DISPEL|CAN_CANCEL,	50,	12,
	"",	"You are no longer resistant to cold.",	"", CMD_COMMUNE
    },
    {
	"resist lightning", {60, 53, 53, 53, 53, 53, 53, 53, 53, 14, 53, 53},
	spell_resist_lightning,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,	CAN_DISPEL|CAN_CANCEL,	50,	12,
	"",	"You are no longer resistant to lightning.",	"", CMD_COMMUNE
    },
    {
	"resist mental", {60, 53, 53, 53, 53, 53, 53, 53, 53, 16, 53, 53},
	spell_resist_mental,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,	CAN_DISPEL|CAN_CANCEL,	50,	12,
	"",	"You are no longer resistant to mental attacks.",	"", CMD_COMMUNE
    },
    {
	"resist acid", {60, 53, 53, 53, 53, 53, 53, 53, 53, 16, 53, 53},
	spell_resist_acid,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,	CAN_DISPEL|CAN_CANCEL,	50,	12,
	"",	"You are no longer resistant to acid.",	"", CMD_COMMUNE
    },
    {
	"resist negative", {60, 53, 53, 53, 53, 53, 53, 53, 53, 20, 53, 53},
	spell_resist_negative,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,	CAN_DISPEL|CAN_CANCEL,	50,	12,
	"",	"You are no longer resistant to negative damage.",	"", CMD_COMMUNE
    },

    {
	"remove paralysis", {60, 53, 53, 53, 53, 53, 53, 53, 53, 30, 53, 53},
	spell_remove_paralysis,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,	0,	70,	12,
	"",	"",	"", CMD_COMMUNE
    },

    {
	"group teleport", {60, 53, 53, 53, 53, 53, 53, 53, 53, 33, 53, 53},
	spell_group_teleport,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,	0,	90,	12,
	"",	"",	"", CMD_COMMUNE
    },

	{
	"offhand disarm", {60, 25, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	NULL,	0,	0,	24,
	"",	"",	"", CMD_NONE
    },
    {
	"cleanse", {60, 53, 53, 53, 53, 53, 53, 53, 53, 35, 53, 53},
	spell_cleanse,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,	0,	75,	12,
	"",	"",	"", CMD_COMMUNE
    },
    {
        "strength of faith", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_strength,          TAR_CHAR_SELF,     POS_STANDING,
        NULL,   0,        250,     80,
        "",     "The absolute certainty in your faith begins to waver.",     "", CMD_COMMUNE
    },

	{
        "sunray", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_sunray,       TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        &gsn_sunray,   		0,      25,     12,
        "sunray",   		"The bright spot fades from your vision and you can see again.",     "", CMD_COMMUNE
    },
    {
        "stupidity", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,         TAR_IGNORE,             POS_DEAD,
        NULL,                   0,        0,      0,
        "",                     "You feel marginally, although not all that much, less stupid.",     "", CMD_NONE
    },
    {
        "fatigue", {60, 53, 53, 26, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_fatigue, TAR_CHAR_OFFENSIVE,        POS_FIGHTING,
        NULL,                   0,        30,      12,
        "fatigue",                     "",     "", CMD_COMMUNE
    },

    {
	"remove taint", {60, 53, 53, 53, 21, 53, 53, 53, 53, 53, 53, 53},
	spell_remove_taint,	TAR_OBJ_INV,	POS_STANDING,
	NULL,	0,	50,	12,
	"",	"",	"", CMD_COMMUNE
    },

    {
        "worldbind", {60, 53, 53, 53, 53, 40, 53, 53, 53, 53, 53, 53},
        spell_worldbind,      TAR_CHAR_OFFENSIVE,     POS_STANDING,
        &gsn_worldbind,           CAN_CLEANSE,      75,     18,
        "",                     "Your ties to the spiritual world recover.", "", CMD_SPELL
    },

    {
        "sceptre drain", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_null,	TAR_IGNORE,		POS_STANDING,
	&gsn_sceptret,			0,	0, 	0,
	"Rape Me",		"You feel less drained.",	"", CMD_NONE
    },

 {
        "sceptre", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_null,	TAR_IGNORE,		POS_STANDING,
	&gsn_sceptre,			0,	0, 	0,
	"Rape Me",		"You no longer feel drained.",	"", CMD_NONE
    },
    {
        "dark blessing", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,     TAR_IGNORE,             POS_STANDING,
        &gsn_dark_blessing,                   0,        0,      0,
        "",              "Your unholy power fades away.",  "", CMD_NONE
    },
    {
        "informant", {60, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38},
        spell_informant,     TAR_IGNORE,     POS_STANDING,
        NULL,   0,       80,  24,
        "", "",    "", CMD_POWER
    },
    {
        "hunters strength", {60, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15},
        spell_hunters_strength,     TAR_IGNORE,     POS_STANDING,
        NULL,   0,     60,  12,
        "", "Your hunter's strength wanes as your concentration wavers.",    "", CMD_POWER
    },
	{
		"hunters awareness", {60, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20},
		spell_hunters_awareness,     TAR_IGNORE,     POS_STANDING,
		NULL,   0,     60,  12,
		"", "You feel ready to call upon your hunter's instincts again.",    "", CMD_POWER
	},
	{
		"hunters net", {60, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30},
		spell_null,	TAR_IGNORE,	POS_STANDING,
		&gsn_hunters_net,	0, 0, 0,
		"", "You finally break free of the net encumbering you.", "", CMD_POWER
	},
    {
        "hire mercenary", {60, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31},
        spell_hire_mercenary,     TAR_IGNORE,     POS_STANDING,
        NULL,   0,      100,  12,
        "", "The guild will put you in touch with a new mercenary-for-hire.",    "", CMD_POWER
    },
    {
        "hunters vision", {60, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40},
        spell_hunters_vision,     TAR_IGNORE,     POS_STANDING,
        NULL,   0,      75,  12,
        "", "Your predatorial sight dulls as your concentration wavers.",    "", CMD_POWER
    },

    {
        "displacement", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,     TAR_IGNORE,     POS_STANDING,
        &gsn_displacement,   0,       0,  0,
        "", "",    "", CMD_NONE
    },
    {
        "reflect", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,     TAR_IGNORE,     POS_STANDING,
        &gsn_volley,   0,       0,  0,
        "", "",    "", CMD_NONE
    },
    {
	"water breathing", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 1},
	spell_waterbreath, TAR_CHAR_DEFENSIVE, POS_STANDING,
	NULL,		CAN_DISPEL|CAN_CANCEL,	35,	12,
	"",	"You can no longer breathe under water.",	"", CMD_BOTH
    },
    {
		"dark vessel", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 20, 53},
		spell_dark_vessel, TAR_IGNORE, POS_STANDING,
		NULL,		0,	300,	24,
		"",	"",	"", CMD_SPELL
    },
    {
	"siphon", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 29, 53},
	spell_siphon, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
	NULL,		0,	30,	24,
	"siphoning",	"",	"", CMD_SPELL
    },
    {
	"hex", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 47, 53},
	spell_hex, TAR_CHAR_OFFENSIVE, POS_STANDING,
	&gsn_hex,		CAN_CLEANSE,	100,	24,
	"hex",	"",	"", CMD_SPELL
    },
    {
        "visceral divination", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 51, 53},
        spell_visceral, TAR_CHAR_SELF, 	POS_STANDING,
        &gsn_visceral,               0,        400,    72,
        "",  "",     "", CMD_SPELL
    },
    {
        "dark summons", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 49, 53},
        spell_black_circle, 	TAR_IGNORE,  POS_STANDING,
        NULL,               0,        250,    24,
        "",  "You feel able to call your minions to you again.",     "", CMD_SPELL
    },
    {
        "ritual of soul", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 51, 53},
        spell_ritual_soul,     TAR_CHAR_DEFENSIVE,  POS_STANDING,
        NULL,               0,        200,    36,
        "",  "",     "", CMD_SPELL
    },
    {
        "ritual of flesh", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 45, 53},
        spell_ritual_flesh,     TAR_CHAR_DEFENSIVE,  POS_STANDING,
        NULL,               0,        200,    36,
        "",  "",     "", CMD_SPELL
    },
    {
        "conflagration", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 51},
        spell_conflagration,     TAR_IGNORE,  POS_STANDING,
        &gsn_conflagration,               0,        200,    24,
        "conflagration",  "You feel fully recovered from your last conflagration.",     "", CMD_SPELL
    },
    {
        "gravity well", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 51},
        spell_gravity_well,     TAR_IGNORE,  POS_STANDING,
        &gsn_gravity_well,               0,        200,    24,
        "",  "You feel able to create a new gravity well.",     "", CMD_SPELL
    },
    {
        "ultradiffusion", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 45},
        spell_ultradiffusion,     TAR_CHAR_DEFENSIVE,  POS_STANDING,
        &gsn_ultradiffusion,               0,        200,    36,
        "molecular disjunction",  NULL,     "", CMD_SPELL
    },
    {
        "disjunction", {60, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61},
        NULL,     TAR_IGNORE,  POS_DEAD,
        &gsn_molecular_disjunction,               0,        0,    0,
        "",  "",     "", CMD_SPELL
    },
    {
        "cyclone", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 51},
        spell_cyclone,     TAR_IGNORE,  POS_STANDING,
        &gsn_cyclone,               0,        200,    24,
        "",  "You feel able to create another cyclone.",     "", CMD_SPELL
    },

	{
		"forming cyclone", {60, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61},
		NULL,	TAR_IGNORE,	POS_DEAD,
		&gsn_cyclone_forming,	0,	0,	0,
		"", "", "", CMD_SPELL
	},

    {
        "knock", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 29},
        spell_knock,     TAR_IGNORE,  POS_STANDING,
        NULL,               0,        20,    24,
        "",  "",     "", CMD_SPELL
    },
    {
        "vacuum", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 40},
        spell_vacuum,     TAR_IGNORE,  POS_STANDING,
        &gsn_vacuum,               0,        100,    36,
        "vacuum",  "",     "", CMD_SPELL
    },

    {
	"scorch", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 15},
	spell_scorch,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_scorch,	0,	20,	12,
	"scorching heat",	"",	"", CMD_SPELL
    },

    {
        "heat metal", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 21},
	spell_heat_metal,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			0, 	25,	24,
	"",		"!Heat Metal!",		"", CMD_SPELL
    },

    {
        "incandescense", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 7},
        spell_incandescense,      TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        &gsn_incandescense,       CAN_DISPEL|CAN_CANCEL,        5,     12,
        "", 	"The glowing aura around you shimmers and fades away.", "", CMD_SPELL
    },
    {
        "diuretic warmth", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 25},
        spell_diuretic,     TAR_CHAR_AMBIGUOUS,  POS_FIGHTING,
        &gsn_diuretic,               0,        	25,    24,
        "",  "",     "", CMD_SPELL
    },
    {
        "corona", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 35},
        spell_corona,     TAR_IGNORE,  POS_STANDING,
        &gsn_corona,      CAN_DISPEL|CAN_CANCEL,           40,    24,
        "fiery corona",  "The flames around you flicker and die out.",     "", CMD_SPELL
    },

    {
		"heatshield", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 38},
		spell_heatshield,	TAR_IGNORE,	POS_STANDING,
		&gsn_heatshield,	CAN_DISPEL|CAN_CANCEL,	50,	24,
		"heatshield",	"Your heat shield dissipates.",	"", CMD_SPELL
   	},

    {
        "scathing wind", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 40},
        spell_scathing,     TAR_IGNORE,  POS_FIGHTING,
        NULL,               0,           30,    24,
        "scathing wind",  "You can see again.",     "", CMD_SPELL
    },
    {
        "immolate", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 44},
        spell_immolate,       TAR_CHAR_OFFENSIVE,     POS_STANDING,
        &gsn_immolate,                   0,        120,     24,
        "",             "",         "", CMD_SPELL
    },
    {
        "agitate", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 48},
        spell_agitate,     TAR_CHAR_AMBIGUOUS,  POS_STANDING,
        &gsn_agitate,               0,           80,    24,
        "",  "You breathe a sigh of relief as the excess heat within your body dissipates.",     "", CMD_SPELL
    },
    {
        "earthquake", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 45},
        spell_earthquake,     TAR_IGNORE,  POS_FIGHTING,
        &gsn_earthquake,               0,           80,    24,
	"earthquake", "", "", CMD_SPELL
    },

    {
        "electrocute", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 28},
        spell_electrocute,     TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
        NULL,               0,           30,    24,
	"electrocution", "", "", CMD_SPELL
    },
    {
        "induce pain", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 1},
        spell_induce_pain,     TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
        NULL,               0,           20,    12,
	"induced pain", "", "", CMD_SPELL
    },

    {
        "disrupt vision", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 13},
        spell_disrupt_vision,     TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
        NULL,         CAN_DISPEL|CAN_CANCEL,           20,    12,
	"", "You can see again.", "", CMD_SPELL
    },

    {
        "mana conduit", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 20},
        spell_mana_conduit,     TAR_CHAR_DEFENSIVE,  POS_STANDING,
        &gsn_mana_conduit,              CAN_DISPEL|CAN_CANCEL,   35,    12,
	"", "You no longer feel energized to channel mana.", "", CMD_SPELL
    },

    {
        "enhance synaptics", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 16},
        spell_synaptic_enhancement,     TAR_CHAR_DEFENSIVE,  POS_STANDING,
        &gsn_synaptic_enhancement,      CAN_DISPEL|CAN_CANCEL,      40,    12,
	"", "Your mind clouds slightly as your synaptic enhancement wears off.", "", CMD_SPELL
    },

    {
        "impair synaptics", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 32},
        spell_synaptic_impairment,     TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
        &gsn_synaptic_impairment,   CAN_DISPEL|CAN_CANCEL,           30,    12,
	"", "The haze lifts from your mind, and you can think clearly once again.", "", CMD_SPELL
    },

    {
	"elecshield", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 38},
	spell_elecshield,	TAR_IGNORE,	POS_STANDING,
	&gsn_elecshield,	CAN_DISPEL|CAN_CANCEL,	50,	24,
	"electricity shield",	"Your shield of electricity dissipates.",	"", CMD_SPELL
    },

    {
        "scramble neurons", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 41},
        spell_scramble_neurons,     TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
        &gsn_scramble_neurons,         CAN_DISPEL|CAN_CANCEL,           100,    24,
	"", "You can once again focus properly.", "", CMD_SPELL
    },

    {
        "mana leech", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 45},
        spell_mana_leech,     TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
        NULL,               0,           5,    24,
	"mana leech", "", "", CMD_SPELL
    },

    {
	"interference", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 51},
	spell_interference,	TAR_IGNORE,	POS_STANDING,
	&gsn_interference,	0,	200,	36,
	"",	"You feel able to create another interference field.",	"", CMD_SPELL
    },
    {
        "rage", {60, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20},
        spell_rage,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_rage,       0,       0,      12,
        "",                     "Your barbaric rage slowly subsides.",       "", CMD_POWER
    },

	{
		"communion", {60, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20},
		spell_horde_communion,	TAR_CHAR_GENERAL,		POS_STANDING,
		&gsn_horde_communion,		0,		100,		24,
		"",				"",			"",		CMD_POWER
	},

    {
	"hydroperception", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 12},
	spell_hydroperception,	TAR_CHAR_SELF,		POS_STANDING,
	&gsn_hydroperception,	CAN_DISPEL|CAN_CANCEL,	 15,	12,
	"",			"Your senses of changes in water subside.",
	"", CMD_SPELL
    },

    {
        "dehydrate", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 17},
        spell_dehydrate,     TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
        NULL,               0,           30,    24,
	"dehydration", "", "", CMD_SPELL
    },

    {
        "drown", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 21},
        spell_drown,     TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
        NULL,               0,          25,    12,
	"drowning", "", "", CMD_SPELL
    },
    {
        "hydration", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 31},
        spell_hydration,     TAR_CHAR_DEFENSIVE,  POS_STANDING,
        NULL,               0,           50,    24,
	"", "", "", CMD_SPELL
    },
    {
	"decrepify", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 33, 53},
	spell_decrepify,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_decrepify,		0,	50,	24,
	"",	"You feel less sluggish.", "", CMD_SPELL
    },
    {
	"story tell", {60, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10},
        spell_null,        TAR_CHAR_OFFENSIVE,     POS_STANDING,
        NULL,         0,        0,     0,
        "",     "", "", CMD_NONE
    },
    {
        "epic strength", {60, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28},
        spell_epic,        TAR_IGNORE,     POS_STANDING,
        NULL,         0,        50,     12,
        "",     "Your increased stamina fades as your concentration wavers.", "", CMD_POWER
    },
    {	
	"corrupt flesh", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 36, 53},
	spell_corrupt_flesh,	TAR_IGNORE,	POS_STANDING,
	NULL,	0,	35,	24,
	"",	"",	"", CMD_SPELL
    },
    {
	"corpse trap", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 26, 53},
	spell_corpse_trap,	TAR_IGNORE,	POS_STANDING,
	NULL,	0,	100,	24,
	"",	"You feel ready to make another trap.",	"", CMD_SPELL
    },
    {
        "mangled throat", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,      TAR_IGNORE,     POS_STANDING,
        &gsn_mangled,   0,        0,    0,
        "", "Your throat stops bleeding.", "", CMD_NONE
    },
    {
        "torn muscles", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,      TAR_IGNORE,     POS_STANDING,
        NULL,   0,        0,    0,
        "", "Your muscles slowly begin to mend.", "", CMD_NONE
    },
    {
        "predatorial hunger", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,      TAR_IGNORE,     POS_STANDING,
        &gsn_jackal,   0,        0,    0,
        "", "", "", CMD_NONE
    },
    {
	"watershield", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 38},
	spell_watershield,	TAR_IGNORE,	POS_STANDING,
	&gsn_watershield,	CAN_DISPEL|CAN_CANCEL,	50,	24,
	"watershield",	"Your water shield dissipates.",	"", CMD_SPELL
    },

    {
        "flood", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 40},
        spell_flood,     TAR_IGNORE,  POS_STANDING,
        &gsn_flood,               0,       70,    36,
        "",  "",     "", CMD_SPELL
    },

    {
        "regeneration", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 24},
        spell_regeneration, TAR_CHAR_DEFENSIVE, POS_STANDING,
        NULL,   CAN_DISPEL|CAN_CANCEL,  75, 24,
        "", "The soothing coolness of your regeneration wanes.",    "", CMD_SPELL
    },

    {
        "tidal wave", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 44},
        spell_tidalwave,     TAR_IGNORE,  POS_STANDING,
        &gsn_tidalwave,               0,       150,    24,
        "tidal wave",  "You feel ready to create another tidal wave.",     "", CMD_SPELL
    },

    {
        "riptide", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 51},
        spell_riptide,     TAR_IGNORE,  POS_STANDING,
        &gsn_riptide,               0,       100,    24,
        "",  "You feel ready to create another riptide.",     "", CMD_SPELL
    },

    {
        "watermeld", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 28},
        spell_watermeld,     TAR_CHAR_SELF,  POS_STANDING,
        &gsn_watermeld,               0,       70,    24,
        "",  "The water swirls around you as your concealment fails you.",     "", CMD_SPELL
    },
    {
        "sense disturbance", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 10},
        spell_sense_disturbance,     TAR_CHAR_SELF,  POS_STANDING,
        &gsn_sense_disturbance,     CAN_DISPEL|CAN_CANCEL,       10,    12,
        "",  "Your sense of disturbances in the air wavers.",     "", CMD_SPELL
    },
    {
        "travelease", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 21},
        spell_travelease,     TAR_CHAR_SELF,  POS_STANDING,
        &gsn_travelease,     CAN_DISPEL|CAN_CANCEL,       25,    12,
        "",  "The favorable winds aiding your travels subside.",     "", CMD_SPELL
    },

    {
        "diffusion", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 27},
        spell_diffusion, TAR_CHAR_DEFENSIVE, POS_STANDING,
        NULL,   CAN_DISPEL|CAN_CANCEL,  30, 12,
        "", "You feel solid again.",    "", CMD_SPELL
    },

    {
        "disruption", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 1},
        spell_disruption,     TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
        NULL,               0,           25,    12,
	"disruption", "", "", CMD_SPELL
    },

    {
	"enlarge", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 37},
	spell_enlarge,	    TAR_CHAR_DEFENSIVE,	    POS_FIGHTING,
	&gsn_enlarge,		CAN_DISPEL|CAN_CANCEL,      40,     24,
	"",			"You shrink back to your normal size.",	"", CMD_SPELL
    },
    {
        "harden fist", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 28},
        spell_hardenfist,     TAR_CHAR_SELF,  POS_STANDING,
        &gsn_hardenfist,               0,       10,    12,
        "",  "Your hands return to normal.",     "", CMD_SPELL
    },

    {
        "stability", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 15},
        spell_stability,      TAR_CHAR_SELF,     POS_STANDING,
        &gsn_stability,                   0,      20,     12,
        "stability",          "You lose control of your stability.","", CMD_SPELL
    },

    {
        "crush", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 26},
        spell_crush,     TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
        NULL,               0,           25,    12,
        "crush", "", "", CMD_SPELL
    },
    {
        "sense vibrations", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 9},
        spell_sensevibrations,     TAR_CHAR_SELF,  POS_STANDING,
        &gsn_sensevibrations,     CAN_DISPEL|CAN_CANCEL,       15,    12,
        "",  "You no longer feel attuned to the ground.",     "", CMD_SPELL
    },

	{
		"overbear", {60, 53, 53, 53, 53, 53, 53, 35, 53, 53, 53, 31},
		spell_overbear,			TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,			0,			35,		24,
		"overbear",	"",	"", CMD_SPELL
	},

    {
        "diamondskin", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 40},
        spell_diamondskin,     TAR_CHAR_SELF,  POS_STANDING,
        &gsn_diamondskin,      CAN_DISPEL,       100,    36,
        "shattered skin",  "Your skin softens and returns to its normal state.",
	"", CMD_SPELL
    },

    {
        "earthshield", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 38},
        spell_earthshield,       TAR_IGNORE,     POS_STANDING,
        &gsn_earthshield,    CAN_DISPEL|CAN_CANCEL,      50,     24,
        "earthshield",   "Your earth shield dissipates.", "", CMD_SPELL
    },
    {
        "coldshield", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 38},
        spell_coldshield,       TAR_IGNORE,     POS_STANDING,
        &gsn_coldshield,     CAN_DISPEL|CAN_CANCEL,      50,     24,
        "coldshield",   "Your cold shield dissipates.", "", CMD_SPELL
    },
    {
        "reduce", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 35},
        spell_reduce,      TAR_CHAR_DEFENSIVE,     POS_FIGHTING,
        &gsn_reduce,       CAN_DISPEL|CAN_CANCEL,      40,     24,
        "",                     "You return to your normal size.", "", CMD_SPELL
    },

    {
        "anchor", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 36},
        spell_anchor,     TAR_IGNORE,  POS_STANDING,
        NULL,               0,       60,    24,
        "",  "",     "", CMD_SPELL
    },
    {
        "transferrence", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 36},
        spell_aerial_transferrence,     TAR_IGNORE,  POS_STANDING,
        NULL,               0,       30,    24,
        "rough landing",  "",     "", CMD_SPELL
    },

    {
	"airshield", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 38},
	spell_airshield,	TAR_IGNORE,	POS_STANDING,
	&gsn_airshield,	CAN_DISPEL|CAN_CANCEL,	50, 24,
	"airshield",	"Your air shield dissipates.",	"", CMD_SPELL
    },

    {
	"frigid aura", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 35},
	spell_frigidaura,	TAR_IGNORE,	POS_STANDING,
	&gsn_frigidaura,	CAN_DISPEL | CAN_CANCEL, 40, 12,
	"frigid aura",	"The cold aura about you dissipates.", "",CMD_SPELL
    },
    {
        "enervate", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 48},
        spell_enervate,     TAR_CHAR_AMBIGUOUS,  POS_STANDING,
        &gsn_enervate,               0,           80,    24,
        "",  "You breathe a sigh of relief as warmth returns to your body.", "", CMD_SPELL
    },

    {
        "coagulate", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 25},
        spell_coagulate,     TAR_CHAR_AMBIGUOUS,  POS_FIGHTING,
        &gsn_coagulate,               0,           20,    12,
        "",  "",     "", CMD_SPELL
    },
    {
        "integrate style", {60, 45, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,     TAR_IGNORE,  POS_FIGHTING,
        &gsn_integrate,               0,           0,    0,
        "",  "",     "", CMD_NONE
    },

    {
        "chill metal", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 18},
	spell_chillmetal,	TAR_OBJ_CHAR_DEF, POS_FIGHTING,
	NULL,			0,	30,	24,
	"",	"",	"", CMD_SPELL
    },

    {
        "freeze metal", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 21},
	spell_freezemetal,	TAR_CHAR_OFFENSIVE, POS_FIGHTING,
	NULL,			0,	30,	24,
	"",	"",	"", CMD_SPELL
    },

    {
        "entrap", {60, 24, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_entrap,            0,        0,     0,
        "",                     "!Entrap!",		"", CMD_NONE
    },

    {
        "drive", {60, 25, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_drive,            0,        0,     0,
        "",                     "!Drive!",             "", CMD_NONE
    },

    {
        "hypothermia", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 30},
	spell_hypothermia,		TAR_CHAR_OFFENSIVE, POS_STANDING,
	&gsn_hypothermia,		CAN_CANCEL,	20,	12,
	"",	"You regain consciousness as warmth creeps back into your body.", "", CMD_SPELL
    },

    {
        "frostbite", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 33},
	spell_frostbite,		TAR_CHAR_OFFENSIVE, POS_FIGHTING,
	NULL,		0,	40,	24,
	"",	"A dull aching replaces the terrible numbness as warmth creeps back into your limbs.", "", CMD_SPELL
    },

    {
        "imprison voice", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 42},
        spell_imprisonvoice,       TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        &gsn_imprisonvoice,      CAN_CANCEL|CAN_DISPEL,      60,     24,
        "",          "You feel relief as you realize that you can speak again.","", CMD_SPELL
    },

	{
		"acid stream", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 21},
		spell_acid_stream,			TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
		&gsn_acid_stream,					0, 		30,		12,
		"acid stream",	"The searing pain from the acid abates.", "", CMD_SPELL
	},

	{
		"caustic vapor", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 41},
		spell_caustic_vapor,	TAR_IGNORE,	POS_FIGHTING,
		&gsn_caustic_vapor,	CAN_PURIFY,	50,	24,
		"caustic vapor",	"You feel able to once again create caustic vapors.",	"", CMD_SPELL
	},

	{
        "dash", {60, 25, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_dash,            0,        0,     0,
        "",                     "!Dash!",             "", CMD_NONE
    },

    {
	"stasis wall", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_stasis_wall,	TAR_DIR,	POS_STANDING,
	&gsn_stasis_wall,	RUNE_CAST|RUNE_DOOR,	50,	48,
	"",			"bright blue before fading away",		      "", CMD_SPELL
    },

    {
        "fend", {60, 20, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_fend,            0,        0,     	0,
        "",                     "!Fend!",		"", CMD_NONE
    },

    {
        "howl", {60, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_howl,            0,        0,     0,
        "",                     "!Howl!",             "", CMD_NONE
    },

    {
        "hobble", {60, 38, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_hobble,            0,        0,     0,
        "hobbling strike", "Your leg feels strong enough for you to rest weight on it once more.", "",  CMD_NONE
    },

    {
        "crippling blow", {60, 42, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_crippling_blow,            0,        0,     0,
        "crippling blow", "Your once-mangled arm feels stronger, despite a lingering stiffness.",	"",   CMD_NONE
    },

    {
        "maneuvering", {60, 20, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_maneuvering,             0,        0,     0,
        "",                     "!Maneuvering!",              "", CMD_NONE
    },
    {
        "gouge", {60, 28, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_gouge,            0,        0,     0,
        "gouge", "The swelling around your eyes subsides and your vision clears.",	"",   CMD_NONE
    },

    {
        "bleed", {60, 35, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_bleed,            CAN_CLEANSE,        0,     0,
        "bleeding strike", "",	"",   CMD_NONE
    },

    {
        "catch", {60, 24, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_catch,             0,        0,     0,
        "",                     "!Catch!",              "", CMD_NONE
    },

    {
        "posture", {60, 32, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_posture,            0,        0,     0,
        "",       "",		"", CMD_NONE
    },

    {
        "unbalance", {60, 26, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_unbalance,            0,        0,     0,
        "",       "",		"", CMD_NONE
    },

    {
        "sidestep", {60, 33, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_sidestep,             0,        0,     0,
        "",                     "!Sidestep!",              "", CMD_NONE
    },

    {
        "concuss", {60, 36, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,     TAR_IGNORE,     POS_FIGHTING,
        &gsn_concuss,  0,       0,      0,
        "concuss",    "Your mind clears as your temples cease throbbing.", "", CMD_NONE
    },

    {
        "retreat", {60, 20, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,     TAR_IGNORE,     POS_FIGHTING,
        &gsn_retreat,  0,       0,      0,
        "retreat",    "!Retreat!", "", CMD_NONE
    },
    {
	"unholy bond", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 40, 53},
	spell_null,	TAR_IGNORE,	POS_STANDING,
	&gsn_unholy_bond, 0, 0, 0,
	"unholy bond", "", "", CMD_NONE
    },

	{
	"chill touch", {60, 53, 53, 53, 53, 7, 53, 53, 53, 53, 1, 53},
	spell_chill_touch,	TAR_CHAR_OFFENSIVE, POS_FIGHTING,
	&gsn_chill_touch,	0, 20, 12,
	"chill touch", "You feel less cold.", "", CMD_SPELL
    },

	{
	"uppercut", {60, 38, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_uppercut,	0, 0, 0,
	"uppercut",	"You feel your jaw heal.", "", CMD_NONE
    },

    {
        "overhead", {60, 29, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,     TAR_IGNORE,     POS_FIGHTING,
        &gsn_overhead,  0,       0,      0,
        "overhead strike",    "Your head stops throbbing.", "", CMD_NONE
    },

	{
        "bleeding", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,      TAR_IGNORE,     POS_STANDING,
        &gsn_bleeding,   0,        0,    0,
        "bleeding", "You finally stop bleeding.", "", CMD_NONE
    },	

    {
        "disrupt formation", {60, 45, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,      TAR_IGNORE,     POS_STANDING,
        &gsn_disrupt_formation,   0,        0,    0,
        "", "!Disrupt!", "", CMD_NONE
    },
    {
	"dart", {60, 28, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_dart,	0,	0,	0,
	"darting strike",	"Blah, I say, blah.", "", CMD_NONE
    },
    {
	"bloodied eyes", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_null,	TAR_IGNORE,	POS_STANDING,
	&gsn_bloody_eyes,	0,0,0,
	"", "You wipe the blood out of your eyes.", "", CMD_NONE
    },
    {
	"cracked sternum", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_null,	TAR_IGNORE,	POS_STANDING,
	&gsn_cracked_sternum, 0,0,0,
	"",	"Your feel your sternum mend.","", CMD_NONE
    },
    {
	"broken arm", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_null, 	TAR_IGNORE,	POS_STANDING,
	&gsn_broken_arm, 0,0,0,
	"",	"Your arm feels stronger.", "", CMD_NONE
    },

    {
        "hurl", {60, 26, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_hurl,            0,        0,     0,
        "hurled weapon", "",	"",   CMD_NONE
    },
    {
	"exchange", {60, 29, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_exchange,	0,0,0,
	"",	"",	"", CMD_NONE
    },

    {
        "charge", {60, 33, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,    		 TAR_IGNORE,	 	    POS_STANDING,
        &gsn_charge, 		 0,       0,      0,
        "charge",    "", 	"", 	CMD_NONE
    },

	{
		"deflect", {60, 26, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
		spell_null,				TAR_IGNORE,				POS_FIGHTING,
		&gsn_deflect,		  0,		0,		0,
		"deflect",						"!Deflect!",	"",	CMD_NONE
	},

	{
		"ease", {60, 20, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
		spell_null,			TAR_IGNORE,				POS_SLEEPING,
		&gsn_ease,			  0,		0,		0,
		"ease",						"!Ease!",		"",	CMD_NONE
	},

	{
	"shieldbash", {60, 35, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_shieldbash,       	0,       0,      18,
        "shieldbash",           "!Shieldbash!",		"", CMD_NONE
    },

	{
	"brace", {60, 29, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_brace,	0,	0,	0,
	"brace",	"",	"", CMD_NONE
    },

	{
	"shatter", {60, 25, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_null, TAR_IGNORE, POS_FIGHTING,
	&gsn_shatter, 0, 0, 0,
	"shatter", "!shatter!", "", CMD_NONE
	},

	{
	"evasion", {60, 20, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_null, TAR_IGNORE, POS_FIGHTING,
	&gsn_evasion, 0, 0, 0,
	"evasion", "!evasion!", "", CMD_NONE
	},

	{
	"whirlwind", {60, 44, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_null, TAR_IGNORE, POS_FIGHTING,
	&gsn_whirlwind, 0, 0, 0,
	"whirlwind", "!whirlwind!", "", CMD_NONE
	},

	{
	"batter", {60, 42, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_null, TAR_IGNORE, POS_FIGHTING,
	&gsn_batter, 0, 0, 0,
	"batter", "!batter!", "", CMD_NONE
	},

	{
	"analyze", {60, 42, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_null, TAR_IGNORE, POS_FIGHTING,
	&gsn_analyze, 0, 0, 0,
	"analyze", "!analyze!", "", CMD_NONE
	},

	{
	"leadership", {60, 20, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_null, TAR_IGNORE, POS_STANDING,
	&gsn_leadership, 0, 0, 0,
	"leadership", "You feel a little disorganized suddenly.", "", CMD_NONE
	},
	{
	"rites of preparation", {60, 53, 53, 53, 38, 53, 53, 53, 53, 53, 53, 53},
	spell_rites_of_preparation, TAR_IGNORE, POS_STANDING,
	&gsn_rites_of_preparation, 0, 0, 0,
	"rites of preparation", "You feel less bold and confident.", "", CMD_COMMUNE
	},
	{
	"spiritual hammer", {60, 53, 53, 53, 20, 53, 53, 53, 53, 53, 53, 53},
	spell_spiritual_hammer, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
	&gsn_spiritual_hammer, 0, 25, 24,
	"spiritual hammer", "", "", CMD_COMMUNE
	},
	{
	"shattered bone", {60, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61},
  	spell_null,     TAR_IGNORE,     POS_DEAD,
  	&gsn_shattered_bone,    0,0,      0,
  	"fragments of blasted bone",    "",     "", CMD_NONE
   	},

	{
	"noxious fumes", {60, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61},
	spell_null,		TAR_IGNORE,		POS_DEAD,
	&gsn_noxious_fumes,		CAN_PURIFY,	0,	0,
	"noxious fumes", "You cough one last time, recovering from the effects of the caustic vapors.",	"", CMD_NONE
	},

	{
	"entwine", {60, 33, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_null, TAR_IGNORE, POS_FIGHTING,
	&gsn_entwine, 0, 0, 0,
	"", "!Entwine!", "", CMD_NONE
	},
	{
	"pull", {60, 33, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_null, TAR_IGNORE, POS_FIGHTING,
	&gsn_pull, 0, 0, 0,
	"", "!Pull!", "", CMD_NONE
	},

	{
	"assess", {60, 26, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_null, TAR_IGNORE, POS_FIGHTING,
	&gsn_assess, 0, 0, 0,
	"", "!assess!", "", CMD_NONE
	},

	{
	"exploit", {60, 28, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_null, TAR_IGNORE, POS_FIGHTING,
	&gsn_exploit, 0, 0, 0,
	"", "!exploit!", "", CMD_NONE
	},

	{
	"terrain mastery", {60, 35, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_null, TAR_IGNORE, POS_FIGHTING,
	&gsn_terrain_mastery, 0, 0, 0,
	"",	"!terrain mastery!", "", CMD_NONE
	},

	{
	"acid vein", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 27},
	spell_acid_vein, TAR_OBJ_INV, POS_STANDING,
	&gsn_acid_vein,	0,25, 24,
	"",	"",	"",	CMD_SPELL
	},

	{
		"duelist", {60, 1, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
		spell_null, TAR_IGNORE, POS_FIGHTING,
		&gsn_duelist_skill, 0, 0, 0,
		"","duelist", "", CMD_NONE
	},

	{
		"gladiator", {60, 1, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
		spell_null, TAR_IGNORE, POS_FIGHTING,
		&gsn_gladiator_skill, 0, 0, 0,
		"", "gladiator", "", CMD_NONE
	},

	{
		"barbarian", {60, 1, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
		spell_null, TAR_IGNORE, POS_FIGHTING,
		&gsn_barbarian_skill, 0, 0, 0,
		"", "barbarian", "", CMD_NONE
	},

	{
		"dragoon", {60, 1, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
		spell_null, TAR_IGNORE, POS_FIGHTING,
		&gsn_dragoon_skill, 0, 0, 0,
		"", "dragoon", "", CMD_NONE
	},

	{
		"skirmisher", {60, 1, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
		spell_null, TAR_IGNORE, POS_FIGHTING,
		&gsn_skirmisher_skill, 0, 0, 0,
		"", "skirmisher", "", CMD_NONE
	},

	{
		"tactician", {60, 1, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
		spell_null, TAR_IGNORE, POS_FIGHTING,
		&gsn_tactician_skill, 0, 0, 0,
		"", "tactician", "", CMD_NONE
	},

	{
	    "corrode lock", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 27},
	    spell_corrode_lock, TAR_IGNORE, POS_STANDING,
	    &gsn_corrode_lock, 0,30,12,
	    "", "", "", CMD_SPELL
    },

	{
		"attract", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 26},
		spell_attract, TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
		&gsn_attract,	CAN_DISPEL|CAN_CANCEL, 	50,		12,
		"lightning strike", "The tingling leaves you.", "", CMD_SPELL,
	},

    {
		"absorb", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 35},
		spell_absorb,		TAR_CHAR_SELF,		POS_STANDING,
		&gsn_absorb,	CAN_CANCEL|CAN_DISPEL,	100,	12,
		"",			"You are no longer absorbing electricity.", "",
		CMD_SPELL
    },

    {
        "lightning bolt", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 15},
	spell_lightning_bolt,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			0,	15,	12,
	"lightning bolt",	"!Lightning Bolt!",	"", CMD_SPELL
    },

    {
        "call lightning", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 40},
	spell_call_lightning,	TAR_IGNORE,		POS_FIGHTING,
	&gsn_call_lightning,			0,	15,	12,
	"lightning strike",	"You feel ready to call lightning again.",	"", CMD_SPELL
    },

    {
        "grounding", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 29},
        spell_grounding,    TAR_IGNORE,  POS_STANDING,
        &gsn_grounding, 	CAN_DISPEL|CAN_CANCEL,  70, 12,
        "", "You lose your grounding against electricty.",  "", CMD_SPELL
    },

	{
		"thunderclap", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 32},
		spell_thunderclap,	TAR_IGNORE,	POS_FIGHTING,
		NULL,		0,	60,	24,
		"thunderclap",	"!Thunderclap!",	"", CMD_SPELL
	},

    {
        "outflank", {60, 32, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_outflank,            0,        0,     0,
        "",       "",		"", CMD_NONE
    },

	{
		"neutralize", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 35},
		spell_neutralize,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
		&gsn_neutralize,	0,	30,	12,
		"neutralize",	"Your resistance to acid fades.",	"",	CMD_SPELL
	},

	{
		"drain", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 20, 53},
		spell_null,	TAR_IGNORE,	POS_STANDING,
		&gsn_drain,	0,	0,	0,
		"",	"",	"",	CMD_NONE
	},

	{
		"talismanic aura", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 35, 35},
		spell_talismanic_aura, TAR_IGNORE, POS_STANDING,
		&gsn_talismanic, 0, 60, 24,
		"", "", "", CMD_SPELL
	},

    {
		"impale", {60, 34, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
		spell_null,     TAR_IGNORE,     POS_FIGHTING,
		&gsn_impale,      0,  0,  24,
		"impale",         "!Impale!",       "", CMD_NONE
	},

	{
		"smokescreen", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 34},
		spell_smokescreen,	TAR_IGNORE,	POS_FIGHTING,
		&gsn_smokescreen,		CAN_PURIFY,	50,	24,
		"",	"You feel as if you can produce another smokescreen.",	"", CMD_SPELL
	},

    {
        "smother", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 29},
	spell_smother,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_smother,			CAN_PURIFY,	25,	12,
	"",	"The thick cloud of putrid smoke around your head dissipates.",	"", CMD_SPELL
    },

    {
        "asphyxiate", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 13},
	spell_asphyxiate,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			0,	40,	12,
	"asphyxiation",	"",	"", CMD_SPELL
    },

	{
		"putrid air", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 18},
		spell_putrid_air,	TAR_IGNORE,	POS_FIGHTING,
		NULL,		0,	50,	24,
		"",	"!putrid air!",	"", CMD_SPELL
	},

    {
        "shroud of secrecy", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 43},
	spell_shroud_of_secrecy,	TAR_CHAR_SELF,	POS_STANDING,
	&gsn_shroud_of_secrecy,		CAN_PURIFY|CAN_DISPEL|CAN_CANCEL,	75,	24,
	"",	"",	"", CMD_SPELL
    },
    {
        "noxious ward", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 38},
		spell_noxious_ward,	TAR_CHAR_SELF,	POS_STANDING,
		&gsn_noxious_ward,			CAN_PURIFY,	75,	24,
		"",	"",	"", CMD_SPELL
	},

	{
		"molten stones", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 17},
		spell_molten_stones,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,	0,	20,	12,
		"",	"",	"", CMD_SPELL
    },

	{
		"melt rock", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 31},
		spell_melt_rock,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
		&gsn_melt_rock,	0,	25,	24,
		"", "",  "", CMD_SPELL
	},

	{
		"heat earth", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 29},
		spell_heat_earth,		TAR_IGNORE,			POS_FIGHTING,
		NULL,		0,			30,			12,
		"heat earth",		"",		"",		CMD_SPELL
	},
	{
		"magma tunnel", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 36},
		spell_magma_tunnel, TAR_IGNORE, POS_STANDING,
		&gsn_magma_tunnel, 0, 50, 24,
		"", "", "", CMD_SPELL
	},

	{
		"blanket", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 38},
		spell_blanket,			TAR_IGNORE,			POS_STANDING,
		&gsn_blanket,				0,	50, 24,
		"blanket", "You feel ready to blanket another room with snow.", "", CMD_SPELL
	},

	{
		"boreal wind", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 15},
		spell_boreal_wind,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,	0,	20, 12,
		"boreal wind", "", "", CMD_SPELL
	},

	{
		"concave shell", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 30},
		spell_concave_shell,	TAR_IGNORE,			POS_FIGHTING,
		NULL,	0,	40,	24,
		"", "", "", CMD_SPELL
	},
	{
		"frost glaze", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 20},
		spell_frost_glaze,	TAR_CHAR_SELF,	POS_STANDING,
		NULL,	CAN_DISPEL|CAN_CANCEL,	15,	24,
		"",	"The frost on your armor melts away.", "", CMD_SPELL
	},
	{
		"unbreakable grip", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 25},
		spell_unbreakable,	TAR_CHAR_AMBIGUOUS,	POS_STANDING,
		&gsn_unbreakable,	CAN_DISPEL|CAN_CANCEL,	50,	24,
		"",	"The frost securing your grip melts away.", "", CMD_SPELL
	},

	{
		"earthen embrace", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 25},
		spell_earthsembrace,   TAR_CHAR_SELF,      POS_STANDING,
		&gsn_earthsembrace,         CAN_DISPEL|CAN_CANCEL,  50, 12,
		"",         "The living stone encasing you melts away.",  "", CMD_SPELL
	},

	{
		"accumulate heat", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 25},
		spell_accumulate_heat,	TAR_CHAR_SELF,	POS_STANDING,
		&gsn_accumulate_heat, 		CAN_DISPEL|CAN_CANCEL, 	50, 12,
		"",			"The heat radiating around you dissipates.", "", CMD_SPELL
	},

	{
		"frigid breeze", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 21},
		spell_frigid_breeze,	TAR_IGNORE,			POS_STANDING,
		NULL,		CAN_DISPEL|CAN_CANCEL,	20,	12,
		"",	"",	"",	CMD_SPELL
	},

	{
		"whiteout", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 40},
		spell_whiteout,			TAR_IGNORE,			POS_STANDING,
		&gsn_whiteout,				0, 150, 24,
		"",			"You feel ready to create another whiteout.", "", CMD_SPELL
	},

	{
		"pure air", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 29},
		spell_pure_air,			TAR_IGNORE,			POS_STANDING,
		NULL,		0, 		40,		24,
		"", "", "", CMD_SPELL
	},

	{
		"icelance", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 25},
		spell_icelance,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,	0,		20,		12,
		"icelance", "", "", CMD_SPELL
	},

	{
		"freeze door", {60, 53, 53, 53, 53, 53, 53, 53, 53, 35, 53, 31},
		spell_freeze_door,	TAR_IGNORE,		POS_STANDING,
		&gsn_freeze_door,		0,		35,		24,
		"",		"",		"",		CMD_SPELL
	},

	{
		"frost growth", {60, 53, 53, 53, 53, 53, 53, 53, 35, 53, 53, 41},
		spell_frost_growth,		TAR_IGNORE,		POS_STANDING,
		&gsn_frost_growth,		0,		80,		24,
		"",		"",		"",		CMD_SPELL
	},

	{
		"bind feet", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 37},
		spell_bind_feet,		TAR_CHAR_OFFENSIVE,	POS_STANDING,
		&gsn_bind_feet,			0,		50,		24,
		"",		"You are able to move your feet again.",		"",		CMD_SPELL
	},


	{
		"glaciate", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 39},
		spell_glaciate,			TAR_IGNORE,			POS_STANDING,
		&gsn_glaciate,			0,		125,	36,
		"",		"",	"",		CMD_SPELL
	},

	{
		"hailstorm", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 23},
		spell_hailstorm,		TAR_IGNORE,			POS_FIGHTING,
		NULL,			0,		25,		12,
		"hailstorm",	"",	"",	CMD_SPELL
	},

	{
		"stalactites", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 45},
		spell_stalactites,		TAR_IGNORE,			POS_STANDING,
		&gsn_stalactites,		0,		200,			24,
		"",		"",		"",		CMD_SPELL
	},

	{
		"ice blast", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 31},
		spell_ice_blast,		TAR_CHAR_OFFENSIVE,	POS_STANDING,
		&gsn_ice_blast,			0,		50,				24,
		"ice blast",		"",		"",		CMD_SPELL
	},

	{
		"puddle", {60, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61},
		spell_null,			TAR_IGNORE,			POS_DEAD,
		&gsn_puddle,		0,		0,	0,
		"",		"",		"",		CMD_NONE
	},

	{
		"icy carapace", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 9},
		spell_icy_carapace,		TAR_CHAR_SELF,	POS_STANDING,
		&gsn_icy_carapace,	CAN_DISPEL|CAN_CANCEL,		35,		12,
		"",		"The icy carapace surrounding you melts away.",		"",		CMD_SPELL
	},
	
	{
		"sheath of ice", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 28},
		spell_sheath_of_ice,		TAR_CHAR_AMBIGUOUS,	POS_FIGHTING,
		&gsn_sheath_of_ice,	0,		40,		24,
		"",		"",		"",		CMD_SPELL
	},

	{
		"ironskin", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 27},
		spell_ironskin,			TAR_CHAR_SELF,		POS_STANDING,
		&gsn_ironskin,		CAN_CANCEL|CAN_DISPEL,	50,	24,
		"",		"Your skin softens and returns to is normal state.",	"", 	CMD_SPELL
	},

	{
		"metal shards", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 15},
		spell_metal_shards,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,				0,		25,		12,
		"metal shard",		"",		"",		CMD_SPELL
	},

	{
		"burden", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 23},
		spell_burden,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		&gsn_burden,	CAN_CANCEL|CAN_DISPEL,		40,		12,
		"burden",	"The invisible weight lifts from your shoulders.", 	"", CMD_SPELL
	},

	{
		"fortify weapon", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 35},
		spell_fortify_weapon,	TAR_OBJ_INV,	POS_STANDING,
		NULL,				0,		100,	24,
		"",		"",		"",		CMD_SPELL
	},

	{
		"fortify armor", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 30},
		spell_fortify_armor,	TAR_OBJ_INV,	POS_STANDING,
		NULL,				0,		100,	24,
		"",		"",		"",		CMD_SPELL
	},

	{
		"alter metal", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 42},
		spell_alter_metal,		TAR_IGNORE,		POS_STANDING,
		NULL,				0,		150,	24,
		"",		"",		"",		CMD_SPELL
	},

	{
		"cloak of mist", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 40},
		spell_cloak_of_mist,	TAR_CHAR_SELF,	POS_STANDING,
		&gsn_cloak_of_mist,	CAN_CANCEL|CAN_DISPEL,	80,		24,
		"",		"The cloak of mist surrounding you dissipates.",
		"The cloak of mist surrounding $n dissipates.", CMD_SPELL
	},

	{
		"vigorize", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 11},
		spell_vigorize,			TAR_IGNORE,		POS_STANDING,
		NULL,			0,		20,		24,
		"",		"",		"",		CMD_SPELL
	},

	{
		"creeping tomb", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 39},
		spell_creeping_tomb,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
		&gsn_creeping_tomb,	CAN_CANCEL,		70,		24,
		"","The ooze surrounding you crumbles to a fine powder and falls away.",
		"The ooze surrounding $n crumbles to dust.",   CMD_SPELL
	},

	{
		"pass without trace", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 34},
		spell_pass_without_trace, TAR_CHAR_SELF,	POS_STANDING,
		&gsn_pass_without_trace, CAN_CANCEL|CAN_DISPEL,  60,  24,
		"", "Your tracks are no longer magically concealed.", "", CMD_SPELL
	},

	{
		"quicksand", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 45},
		spell_quicksand,			TAR_IGNORE,		POS_STANDING,
		&gsn_quicksand,			0,				200,		36,
		"",	"You feel able to create another pool of quicksand.", "", CMD_SPELL
	},

	{
		"sinking", {60, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61},
		spell_null,			TAR_IGNORE,				POS_DEAD,
		&gsn_quicksand_sinking, 0, 0, 0,
		"", "", "", CMD_NONE
	},

	{
		"sap endurance", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 18},
		spell_sap_endurance,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,					0,			30,				12,
		"",	"", "", CMD_SPELL
	},

	{
		"earthfade", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 32},
		spell_earthfade,			TAR_CHAR_SELF,		POS_STANDING,
		&gsn_earthfade,			0,			65,				36,
		"",		"",		"",		CMD_SPELL
	},

	{
		"emulsify", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 28},
		spell_emulsify,				TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,					0,			25,				12,
		"emulsification",	"",	"",	CMD_SPELL
	},
	{
		"rust", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 27},
		spell_rust,					TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		&gsn_rust,				0,			35,				24,
		"",		"",		"",	CMD_SPELL
	},

	{
		"airy water", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 36},
		spell_airy_water,			TAR_IGNORE,		POS_STANDING,
		&gsn_airy_water,		0,			70,				24,
		"",		"",		"",	CMD_SPELL
	},

	{
		"cooling mist", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 25},
		spell_cooling_mist,			TAR_CHAR_DEFENSIVE,	POS_STANDING,
		&gsn_cooling_mist,	CAN_CANCEL|CAN_DISPEL,	40,		24,
		"",		"The cooling mist surrounding you dissipates.",
		"The cooling mist surrounding $n dissipates.", CMD_SPELL
	},
	
	{
		"prismatic spray", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 31},
		spell_prismatic_spray,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,			0,			35,			24,
		"",	"",	"",	CMD_SPELL
	},

    {
        "plasma arc", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 13},
        spell_plasma_arc,     TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
        &gsn_plasma_arc,         CAN_DISPEL|CAN_CANCEL,           20,    24,
		"", "You can see properly again.", "", CMD_SPELL
    },

    {
		"plasma bolt", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 21},
		spell_plasma_bolt,	TAR_CHAR_SELF,		POS_FIGHTING,
		NULL,	0,	 	25,		24,
		"plasma bolt",			"", 	"", CMD_SPELL
    },
    {
		"sphere of plasma", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 28},
		spell_sphere_of_plasma,	TAR_CHAR_SELF,		POS_FIGHTING,
		&gsn_sphere_of_plasma,	CAN_CANCEL,	 20,	24,
		"sphere of plasma",			"",
		"", CMD_SPELL
    },
	{
		"plasma cube", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 36},
		spell_plasma_cube,			TAR_IGNORE,		POS_STANDING,
		&gsn_plasma_cube,		0,	50,	24,
		"",		"",		"",	CMD_SPELL
	},
	{
		"essence of plasma", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 30},
		spell_essence_of_plasma,	TAR_IGNORE,	POS_STANDING,
		&gsn_essence_of_plasma,		0,		50,		24,
		"",		"You feel ready to call upon the essence of plasma once again.",
		"",		CMD_SPELL
	},
    {
        "plasma thread", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 35},
        spell_plasma_thread,     TAR_CHAR_AMBIGUOUS,  POS_STANDING,
        &gsn_plasma_thread,         0,           100,    24,
		"", "", "", CMD_SPELL
    },
	{
		"fashion crystal", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 15},
		spell_fashion_crystal,		TAR_IGNORE,	POS_STANDING,
		&gsn_fashion_crystal,		0,		200,		36,
		"",		"",		"",		CMD_SPELL
	},
	{
		"farsee", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 32},
		spell_farsee, 				TAR_CHAR_SELF,	POS_STANDING,
		&gsn_farsee,			CAN_CANCEL|CAN_DISPEL,		50,			24,
		"",		"Your vision clouds once again.", "", CMD_SPELL
	},
	{
		"mana beam", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 22},
		spell_mana_beam,			TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
		NULL,					0,		30,		24,
		"",		"",		"",		CMD_SPELL
	},
	{
		"detonation", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 27},
		spell_detonation,			TAR_IGNORE,			POS_FIGHTING,
		NULL,					0,		40,		24,
		"",		"",		"",		CMD_SPELL
	},
	{
		"fortify crystal", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 36},
		spell_fortify_crystal,		TAR_OBJ_INV,		POS_STANDING,
		NULL,					0,		100,	24,
		"",		"",		"",		CMD_SPELL
	},
	{
		"rotating ward", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 43},
		spell_rotating_ward,		TAR_CHAR_SELF,		POS_STANDING,
		&gsn_rotating_ward,		0,		60,		24,
		"",		"A crystal orbiting around you crumbles to dust.", "", CMD_SPELL
	},

			/* Beginning of AP revamp */
	{
		"unholy communion", {60, 53, 53, 53, 53, 25, 53, 53, 53, 53, 53, 53},
		spell_unholy_communion,	TAR_CHAR_SELF,	POS_STANDING,
		&gsn_unholy_communion,	0,	150,	48,
		"",	"The lingering effects of your unholy communion fade away.", "",
		CMD_SPELL
	},
	{
		"lesser demon", {60, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61},
		spell_null,			TAR_IGNORE,		POS_DEAD,
		&gsn_lesser_demon,		0,	0,	0,
		"",	"",	"",	CMD_NONE
	},
	{
		"greater demon", {60, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61},
		spell_null,			TAR_IGNORE,		POS_DEAD,
		&gsn_greater_demon,		0, 0, 0,
		"",	"",	"",	CMD_NONE
	},
	{
		"gamygyns pact", {60, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61},
		spell_null,			TAR_IGNORE,		POS_DEAD,
		&gsn_gamygyn_soul,		0, 0, 0,
		"", "", "", CMD_NONE
	},
	{
		"orobas pact", {60, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61},
		spell_null,			TAR_IGNORE,		POS_DEAD,
		&gsn_orobas_soul, 0, 0, 0,
		"",	"",	"",	CMD_NONE
	},
	{
		"dark familiar", {60, 53, 53, 53, 53, 40, 53, 53, 53, 53, 53, 53},
		spell_dark_familiar,	TAR_IGNORE,		POS_STANDING,
		&gsn_dark_familiar,	0,	60,		24,
		"",	"You feel ready to summon a new demonic familiar.", "", CMD_SPELL
	},
	{
		"indomitability", {60, 53, 53, 53, 53, 51, 53, 53, 53, 53, 53, 53},
		spell_indomitability,	TAR_CHAR_SELF,	POS_STANDING,
		&gsn_indom,			0,		150,	24,
		"",	"Your indomitable will fades.",	"", CMD_SPELL
	},
	{
		"darksight", {60, 53, 53, 53, 53, 25, 53, 53, 53, 53, 53, 53},
		spell_null,				TAR_IGNORE,		POS_STANDING,
		&gsn_darksight,		0,		0,		0,
		"",	"Your insight into the shadows fades.",	"",	CMD_SPELL
	},
	{
		"bloodlust", {60, 53, 53, 53, 53, 35, 53, 53, 53, 53, 53, 53},
		spell_null,				TAR_IGNORE,		POS_DEAD,
		&gsn_bloodlust,		0,		0,		0,
		"",		"",		"",		CMD_NONE
	},
	{
		"traitors luck", {60, 53, 53, 53, 53, 35, 53, 53, 53, 53, 53, 53},
		spell_null,				TAR_IGNORE,		POS_DEAD,
		&gsn_traitors_luck,	0,		0,		0,
		"",		"",		"",		CMD_NONE
	},
	{
		"taunt", {60, 53, 53, 53, 53, 25, 53, 53, 53, 53, 53, 53},
		spell_null,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
		NULL,	0,	0,	24,
		"",	"",	"",	CMD_NONE
	},
	{	"wrack", {60, 53, 53, 53, 53, 47, 53, 53, 53, 53, 53, 53},
		spell_wrack,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
		NULL,	0,	50,	24,
		"",	"You feel your body strengthen.", "",	CMD_SPELL
	},
	{
		"radiance", {60, 53, 53, 53, 53, 47, 53, 53, 53, 53, 53, 53},
		spell_radiance,	TAR_CHAR_SELF,	POS_STANDING,
		NULL,	0, 100, 24,
		"",	"You no longer feel so beautiful.",	"",	CMD_SPELL
	},
	{
		"inspire lust", {60, 53, 53, 53, 53, 47, 53, 53, 53, 53, 53, 53},
		spell_inspire_lust,	TAR_CHAR_GENERAL,	POS_STANDING,
		NULL,	0, 50, 24,
		"",	"Your thoughts of riches fade away.",	"", CMD_SPELL
	},
	{
		"dispaters watch", {60, 53, 53, 53, 53, 51, 53, 53, 53, 53, 53, 53},
		spell_dispaters,	TAR_IGNORE,	POS_STANDING,
		NULL,	0, 100, 24,
		"",	"",	"",	 CMD_SPELL
	},

	{
		"mark of wrath", {60, 53, 53, 53, 53, 51, 53, 53, 53, 53, 53, 53},
		spell_mark_of_wrath,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
		&gsn_mark_of_wrath,		0,		200,		24,
		"",		"The smouldering anger within your dark heart fades.", "",
		CMD_SPELL
	},

	{
		"consume", {60, 53, 53, 53, 53, 25, 53, 53, 53, 53, 53, 53},
		spell_null,	TAR_IGNORE,	POS_STANDING,
		NULL,	0, 0, 24,
		"",	"",	"",	CMD_NONE
	},
	{
		"baals mastery", {60, 53, 53, 53, 53, 43, 53, 53, 53, 53, 53, 53},
		spell_baals_mastery,	TAR_IGNORE,		POS_STANDING,
		&gsn_baals_mastery,		0, 	250,		36,
		"", "", "", CMD_SPELL
	},
	{
		"living blade", {60, 53, 53, 53, 53, 51, 53, 53, 53, 53, 53, 53},
		spell_living_blade,		TAR_OBJ_INV,	POS_STANDING,
		&gsn_living_blade,	   0,	200,		24,
		"",	"",	"",	CMD_SPELL
	},
	{
		"leech", {60, 53, 53, 53, 53, 35, 53, 53, 53, 53, 53, 53},
		spell_null,				TAR_IGNORE,		POS_FIGHTING,
		&gsn_leech,			0,		0,		0,
		"",	"",	"",	CMD_NONE
	},
	{
		"word of command", {60, 53, 53, 53, 53, 51, 53, 53, 53, 53, 53, 53},
		spell_word_of_command,	TAR_IGNORE,		POS_STANDING,
		&gsn_word_of_command,	0,		150,		24,
		"", "", "", CMD_SPELL
	},

	{
		"insanity", {60, 53, 53, 53, 53, 51, 53, 53, 53, 53, 53, 53},
		spell_insanity,	TAR_CHAR_SELF,	POS_FIGHTING,
		NULL,	0,	100, 24,
		"", "", "",	CMD_SPELL
	},

	{
		"sign", {60, 53, 5, 53, 53, 53, 53, 53, 53, 53, 53, 53},
		spell_null, TAR_IGNORE, POS_STANDING,
		&gsn_sign, 0, 0, 12,
		"", "",	"", CMD_NONE
	},

    {
        "slash", {60, 53, 23, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_slash,		0,	 0,	24,
	"",			"!slash!",		"", CMD_NONE
    },
    {
	"breath of mephisto", {60, 53, 53, 53, 53, 43, 53, 53, 53, 53, 53, 53},
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	NULL,	0,	0, 	0,
	"",	"",	"", CMD_NONE
    },
    {
	"burning touch", {60, 53, 53, 53, 53, 43, 53, 53, 53, 53, 53, 53},
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	NULL,	0,0,0,
	"",	"The burning stops.", "", CMD_NONE
    },
    {
	"dark insight", {60, 53, 53, 53, 53, 25, 53, 53, 53, 53, 53, 53},
	spell_dark_insight,	TAR_CHAR_DEFENSIVE, POS_STANDING,
	NULL, 0,	60, 24,
	"",	"",	"",	CMD_SPELL
    },	
	{
		"medicine", {60, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15},
		spell_medicine, TAR_IGNORE, POS_STANDING,
		&gsn_medicine,	0,	40,	24,
		"", "The medicine in your system wears off.", "", CMD_POWER
	},
	{
		"repose", {60, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15},
		spell_repose,	TAR_CHAR_SELF,	POS_STANDING,
		&gsn_repose,	0,	30,	24,
		"",	"You feet begin to ache again as you walk.", "", CMD_POWER
	},
    {
        "aggressor", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        spell_null,             TAR_IGNORE,     POS_FIGHTING,
        &gsn_aggressor,         0,      0,      0,
        "", "", "", CMD_NONE
    },

	{
		"subdue", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
		spell_null,             TAR_IGNORE,     POS_FIGHTING,
		&gsn_subdue,         0,      0,      0,
		"", "", "", CMD_NONE
	},

	{
		"drowning", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
		spell_null,             TAR_IGNORE,     POS_FIGHTING,
		&gsn_drowning,		0,		0,		0,
		"", "", "", CMD_NONE
	},
	{
		"colorful blessings", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
		spell_colorful,		TAR_IGNORE, POS_FIGHTING,
		NULL,			0,		0,		0,
		"",	"Your colorful blessings fade away and the world returns to its drab state.", 
		"$n stops staggering around like a drunk.", CMD_NONE
	},
	{
		"severed", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
		spell_null,		TAR_IGNORE,		POS_FIGHTING,
		&gsn_severed,		0,		0,		0,
		"",	"You feel your ties to your god restored.",	"",	CMD_NONE
	},
	{
		"empathy", {60, 53, 53, 53, 24, 53, 53, 53, 53, 53, 53, 53},
		spell_empathy,	TAR_CHAR_DEFENSIVE,		POS_STANDING,
		&gsn_empathy,		0,		50,		24,
		"", "Your spiritual link is severed.", "", CMD_COMMUNE
	},
	{
		"altruism", {60, 53, 53, 53, 29, 53, 53, 53, 53, 53, 53, 53},
		spell_altruism,	TAR_CHAR_DEFENSIVE,		POS_STANDING,
		&gsn_altruism,		0,		25,		24,
		"", "", "", CMD_COMMUNE
	},
	{
		"fervor", {60, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15},
		spell_fervor,	TAR_CHAR_SELF,		POS_STANDING,
		&gsn_fervor,	0,	30,	24,
		"","You feel your holy fervor fade.","", CMD_POWER
	},

	{	"piety", {60, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25},
		spell_piety,	TAR_CHAR_SELF,		POS_STANDING,
		&gsn_piety,	0,	50,	24,
		"","Your wrath fades away.","", CMD_POWER
	},

	{
		"spiritual healing", {60, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30},
		spell_spiritual_healing,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
		&gsn_spiritual_healing,	0,	50,	24,
		"",	"You feel able to heal again.", "", CMD_POWER
	},

	{
		"crimson martyr", {60, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30},
		spell_crimson_martyr,		TAR_IGNORE,			POS_STANDING,
		&gsn_crimson_martyr,		0,		100,		12,
		"righteous vengeance","Your righteous anger slowly begins to fade away.", "", CMD_POWER
	},

	{
		"retribution", {60, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40}, 
		spell_retribution,			TAR_CHAR_OFFENSIVE,	POS_STANDING,
		&gsn_retribution,			0,		60,			24,
		"retribution",			"You feel the weight of your sins lifted.",	"", CMD_POWER
	},

	{
		"shroud of light", {60, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35},
		spell_shroud_of_light,		TAR_CHAR_DEFENSIVE, POS_STANDING,
		&gsn_shroud_of_light,		0,		45,			24,
		"",	"Your shroud of light trickles away.", "", CMD_POWER
	},

	{   
		"safehaven", {60, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40},
		spell_safehaven,        TAR_CHAR_DEFENSIVE, POS_STANDING,
		NULL,               0,      50,     12,
		"", "", "", CMD_POWER
	},

	{   
		"infidels fate", {60, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40},
		spell_null,     TAR_IGNORE, POS_STANDING,
		&gsn_infidels_fate, 0,  0,  0,
		"", "", "", CMD_NONE
	},

	{
		"mana infusion", {53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 5},
		spell_mana_infusion, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
		&gsn_mana_infusion,	0, 20, 24,
		"", "", "", CMD_SPELL
	},
	{
		"tangleroot",	{53,53,53,53,53,53,53,53,53,53,53,53},
		spell_tangleroot, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
		&gsn_tangleroot,	0,	35,	24,
		"", "", "", CMD_COMMUNE
	}, 
	{
		"mana sickness", {53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
		spell_null,	TAR_IGNORE,	POS_STANDING,
		&gsn_mana_sickness,	0, 0, 0,
		"", "As the last of the mana drains from you, you feel decidedly better.", "", CMD_NONE
	},

 	{
		"end", {60, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
                spell_null,             TAR_END,             POS_FIGHTING,
                NULL,           	    0,              0,              0,
                "",     "THIS MUST BE LAST!",     "",     CMD_NONE
	},
	//END MUST BE LAST!!!!
	/* ZZZ */
};

const   struct  group_type      group_table     [MAX_GROUP]     =
{

    {
        "class basics",
        { "sword", "axe", "dagger", "flail", "mace", "polearm","staves","wands",
        "shield block", "spear", "whip", "bash", "berserk", "dirt kicking",
        "disarm", "dodge", "enhanced damage", "hand to hand", "kick",
        "parry", "rescue", "trip", "second attack", "third attack", "embalm",
		"corpse trap", "barkskin", "blade barrier", "holy fire", "warcry",
		"lash", "circle stab", "turn undead", "camouflage", "ambush", "forget",
		"divine touch", "transfer object", "forage", "assassinate", "staff",
		"pugil", "protection heat cold", "intimidate", "escape",
		"moving ambush", "wrath", "dark wrath", "acute vision", "laying hands",
		"dual backstab", "power word kill", "power word stun", "animate dead",
		"skin", "camp", "fourth attack", "demand", "lifebane", "deathspell",
		"ground control", "endure", "throw", "nerve", "poison dust",
		"blindness dust", "strangle", "drain", "tsunami", "dual wield",
		"butcher", "bark skin", "utter heal", "consecrate", "fatigue",
		"detect movement", "protective shield", "animal call", "blackjack", "sign", "trance", "herb",
		"cleave", "bluff", "false motives", "keen vision", "iceball", "lesser golem", "ranger staff", "greater golem",
		"preserve", "power word fear", "prevent healing", "decay corpse",
		"imbue regeneration", "restoration", "atrophy",  "vanish",
		"fire and ice", "counter", "bear call", "enlist",
		"spellcraft", "tame", "darkshield", "find water", "shield cleave",
		"curse weapon", "talk to dead", "focus","rejuvenate", "recharge",
        "fast healing", "haggle", "meditation", "recall", "backstab",
		"envenom", "camouflage", "hide", "peek", "pick lock", "sneak",
		"scrolls", "acid blast", "armor", "bless", "blindness", "burning hands",
		"cancellation", "color spray", "continual light", "control weather",
		"create food", "create water", "cure blindness", "cure critical",
		"cure disease", "cure light", "cure poison", "cure serious", "curse",
		"demonfire", "detect evil", "detect good", "detect hidden",
		"detect invis","vampiric touch", "unholy bond", "detect magic",
		"detect poison", "dispel evil", "dispel good", "dispel magic",
		"enchant armor", "enchant weapon", "energy drain", "faerie fire",
		"faerie fog", "farsight", "fireball", "fireproof", "flamestrike",
		"frenzy", "gate", "giant strength", "harm", "haste", "heal",
		"holy word", "identify", "invisibility", "know alignment",
		"locate object", "magic missile", "mass healing", "mass invis",
		"pass door", "plague", "poison", "protection evil", "protection good",
		"ray of truth", "refresh", "remove curse", "sanctuary", "shield",
		"shocking grasp", "sleep", "slow", "summon", "teleport",
		"ventriloquate", "weaken", "word of recall", "cause critical",
		"cause light", "cause serious", "chain lightning", "change sex",
		"charm person", "ice pox", "hold person", "lore", "sanguine ward",
		"knife", "unholy bless", "cheap shot", "bind", "deafen", "creep",
		"stun", "parting blow", "mysterious dream", "snare", "bearcharge",
		"cure deafness", "awaken", "resist heat", "resist cold",
		"resist lightning", "resist mental", "resist acid", "resist negative",
		"resist paralysis", "group teleport", "remove paralysis", "cleanse",
		"remove taint", "strength of faith", "worldbind", "dark vessel", "hex",
		"siphon", "dark summons", "divine malison",
		"decrepify", "integrate style", "drain", "dash", "drive", "retreat",
		"corrupt flesh", "offhand disarm",
		"plant","palm","trail","drag","tripwire","stash","disguise","avoid",
		"shadow cloak","stealth","counterfeit","strip","unholy communion",
		"infidels weight", "burning vision", "somatic casting", "protection",
		"healing sleep", "chill touch", "gag","steal","spiritual hammer","benevolence",
		"slash", "mana infusion"}
    },

	{
		"warrior",
		{ "" }
	},

	{
		"thief",
		{ "" }
	},
	
	{
		"assassin",
		{ "" }
	},

	{
		"paladin",
		{ "" }
	},

	{
		"zealot",
		{ "" }
	},

	{
		"healer",
		{ "create spring", "fly" }
	},

	{
		"anti-paladin",	
		{ "" }
	},

	{
		"ranger",
		{ "" }
	},

	{
		"necromancer",
		{ "" }
	},

	{
		"shapeshifter",	
		{ "" }
	},

	{
		"sorcerer",	
		{ "" }
	},
	
    {
        "class default",
        { "" }
    },

	/* Racial groups */

	{
		"minotaur",			
		{ "headbutt", "gore" }
	},

	{
		"elf",			
		{ "silent movement" }
	},

	{
		"drow",	
		{ "silent movement" }
	},

	/* Add new groups to the _END_ or it will break mob specs! */

    {
	 "protector",
	 {"intercept", "champions defense", "defend", "group retreat", "valiant charge",
	  "channeling", "shield of faith", "empathy", "altruism", "holy shroud", "rites of preparation",
	  "tower of fortitude" }
    },

    {
	 "crusader",
	 {"strike of virtue", "shadowsense", "arms of light", "blinding orb", "arms of wrath",
	  "voice of damnation", "awe", "arms of purity", "seraphic mantle", "indomitable spirit",
	  "arms of judgement", "divine frenzy" }
    },

	/* new warrior specializations */
    {
        "gladiator",
        { "fend", "entrap", "gouge", "bleed", "hobble", "crippling blow" }
    },

    {
		"duelist",
		{ "maneuvering", "catch", "unbalance", "posture", "concuss",
		  "sidestep", "disrupt formation" }
    },

    {
        "barbarian",
        { "brutality", "shatter", "uppercut","impale","overhead","batter"}
    },

	{
		"skirmisher",
		{ "evasion", "hurl", "dart", "fifth attack", "entwine", "pull",
		  "whirlwind" }
	},
	
	{
		"dragoon", 
		{ "ease", "deflect", "exchange", "charge", "shieldbash", "brace"}
	},
	
	{
		"tactician",
		{ "leadership", "assess", "exploit", "analyze", "terrain mastery",
		  "outflank" },
	},

	{
		"cold",
		{ "chill", "chill metal", "infravision", "freeze metal", "coagulate",
		  "hypothermia", "frostbite", "frigid aura", "coldshield",
		  "imprison voice", "enervate" },
	},

	{
		"heat",
		{ "incandescense", "infravision", "scorch", "heat metal",
		  "diuretic warmth", "knock", "corona", "heatshield", "scathing wind",
		  "immolate", "agitate", "conflagration" }
	},

	{
		"air",
		{ "sense disturbance", "fly", "travelease", "diffusion", "disruption",
		  "enlarge", "anchor", "transferrence", "airshield", "vacuum",
		  "ultradiffusion", "cyclone" }
	},

	{
		"earth",
		{ "harden fist", "sense vibrations", "stability", "stoneskin", "crush",
		  "overbear", "reduce", "earthshield", "diamondskin", "earthquake",
		  "gravity well" }
	},

	{
		"electricity",
		{ "induce pain", "detect invis", "disrupt vision", "enhance synaptics",
		  "mana conduit", "electrocute", "impair synaptics", "elecshield",
		  "scramble neurons", "mana leech", "interference" }
	},

	{
		"water",
		{ "water breathing", "create spring", "hydroperception", "dehydrate",
		  "drown", "regeneration", "watermeld", "hydration", "watershield",
		  "flood", "tidal wave", "riptide" }
	},

	{
		"acid",
		{ "acid stream", "acid vein", "corrode lock", "neutralize",
		  "caustic vapor" },
	},

	{
		"smoke",
		{ "smokescreen", "smother", "shroud of secrecy", "putrid air",
		  "noxious ward", "asphyxiate" }
	},

	{
		"lightning",
		{ "call lightning", "thunderclap", "grounding", "lightning bolt",
		  "attract", "absorb" }
	},

	{
		"magma",
		{ "earthen embrace", "accumulate heat", "melt rock", "magma tunnel",
		  "molten stones", "heat earth" }
	},

	{
		"plasma",
		{ "plasma arc", "sphere of plasma", "plasma thread", "plasma bolt",
		  "plasma cube", "essence of plasma" }
	},
	
	{
		"blizzard",
		{ "blanket", "frigid breeze", "boreal wind", "pure air",
		  "concave shell", "whiteout" }
	},

	{
		"frost",
		{ "frost glaze", "frost growth", "bind feet", "freeze door",
		  "unbreakable grip", "icelance" }
	},

	{
		"crystal",
		{ "fashion crystal", "mana beam", "detonation", "farsee", "rotating ward"},
	},

	{
		"ice",
		{ "glaciate", "stalactites", "ice blast", "hailstorm", "sheath of ice",
		  "icy carapace" }
	},

	{
		"mist",
		{ "vigorize", "cloak of mist", "rust", "cooling mist", "airy water",
		  "prismatic spray" }
	},

	{
		"metal",
		{ "alter metal", "fortify armor", "fortify weapon", "burden",
		  "ironskin", "metal shards" }
	},

	{
		"ooze",
		{ "sap endurance", "quicksand", "creeping tomb", "pass without trace",
		  "emulsify"}
	},

	{
	"end",
	{ "" }
    },
};

