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
#include "tables.h"


//Steal the flag table, we're not storing bitvectors, but shhh, we like their functions!
const struct flag_type aftype_table[] =
    {
        {	"AFT_SPELL",	AFT_SPELL,		TRUE	},
        { 	"AFT_SKILL",	AFT_SKILL,		TRUE	},
        {	"AFT_POWER",	AFT_POWER,		TRUE	},
        {	"AFT_MALADY",	AFT_MALADY,		TRUE	},
        {	"AFT_COMMUNE",	AFT_COMMUNE,	TRUE	},
        {	"AFT_INVIS",	AFT_INVIS,		TRUE	},
        {	"AFT_RUNE",		AFT_RUNE,		TRUE	},
        {	"AFT_TIMER",	AFT_TIMER,		TRUE	},
        {	NULL,			0,				FALSE	},
    };

const struct display_type apply_locations[] =
    {
        {   "none",			APPLY_NONE,			"none"				},
        {	"str",			APPLY_STR,     		"strength"			},
        {	"dex",   		APPLY_DEX,     		"dexterity"			},
        { 	"int",	 		APPLY_INT,    	 	"intelligence"		},
        {   "wis",   		APPLY_WIS,     		"wisdom"			},
        {   "con",   		APPLY_CON,     		"constitution"		},
        {	"sex",   		APPLY_SEX,     		"sex"				},
        {	"class", 		APPLY_CLASS,   		"class"				},
        {	"luck",			APPLY_LUCK,			"luck"				},
        {   "age",  	 	APPLY_AGE,     		"age"				},
        {   "height",		APPLY_HEIGHT,     	"none"				},
        {   "weight",		APPLY_WEIGHT,     	"none"				},
        {   "mana",  		APPLY_MANA,     	"mana"				},
        {   "hp",			APPLY_HIT,     		"health"			},
        {   "move",  		APPLY_MOVE,     	"moves"				},
        {   "gold",	 		APPLY_GOLD,     	"none"				},
        {   "exp",	 		APPLY_EXP,     		"none"				},
        {	"ac",			APPLY_AC,     		"armor class"		},
        {   "hitroll",		APPLY_HITROLL,   	"hitroll"  			},
        {	"damroll",		APPLY_DAMROLL,      "damroll"			},
        {	"saves",		APPLY_SAVES,        "saves"				},
        {	"spara",		APPLY_SAVING_PARA,  "save vs paralysis"	},
        {	"srod",			APPLY_SAVING_ROD,   "save vs rod"		},
        {	"spetri",		APPLY_SAVING_PETRI, "save vs petrification"},
        {	"sbreath",		APPLY_SAVING_BREATH,"save vs breath"	},
        {   "svspell",		APPLY_SAVING_SPELL, "save vs spell" 	},
        {	"affect",		APPLY_SPELL_AFFECT, "none"				},
        {	"carry_weight", APPLY_CARRY_WEIGHT, "weight carried"	},
        {	"defense",		APPLY_DEFENSE,		"none"				},
        {	"regen",		APPLY_REGENERATION, "regeneration"  	},
        {	"size",			APPLY_SIZE,     	"size"				},
        {   "energystate",	APPLY_ENERGYSTATE,	"energy state"		},
        {   "dammod",		APPLY_DAM_MOD,		"none"				},
        {   "legs",			APPLY_LEGS,			"none"				},
        {   "arms",			APPLY_ARMS,			"none"				},
        {	"beauty",		APPLY_BEAUTY,		"none"				},
        {	"alignment",	APPLY_ALIGNMENT,	"none"				},
        {	"ethos",		APPLY_ETHOS,		"none"				},
        {   NULL,			0,					FALSE				},
    };

const struct mod_name_type mod_names[] =
    {
        {	"vision",		MOD_VISION		},
        {	"movement",		MOD_MOVEMENT	},
        {	"toughness",	MOD_TOUGHNESS	},
        {	"speed",		MOD_SPEED		},
        {	"levitation",	MOD_LEVITATION	},
        {	"visibility",	MOD_VISIBILITY	},
        {	"phase",		MOD_PHASE		},
        {	"consciousness",MOD_CONC		},
        {	"protection",	MOD_PROTECTION	},
        {	"appearance",	MOD_APPEARANCE	},
        {	"hearing",		MOD_HEARING		},
        {	"perception",	MOD_PERCEPTION	},
        {	"resistance",	MOD_RESISTANCE	},
        {	"energy state",	MOD_ENERGY_STATE},
        {	"speech",		MOD_SPEECH		},
        {	"regeneration",	MOD_REGEN		},
	{	"tasteful wardrobe",MOD_WARDROBE		},
    };

const struct cabal_type cabal_table[MAX_CABAL] =
    {
        //  name,    who entry,	extitle, 	cabal name, 			independent, 	item vnum, 	limit (pfiles / limit)
        /* independent should be FALSE if is a real cabal */
        {"none",    "",     	"",		"",				TRUE,		1,		1, 		1},
        {"guild",   "[GUILD] ", "",         	"Common Guild, Protectors of Shalar",FALSE,  	0,		1, 		1},
        {"scion",   "[SCION] ", "",     	"Scions of Knowledge",		FALSE,  	23652,		30, 		1},
        {"horde",   "[HORDE] ", "",        	"Barbarian Horde, Pillagers of Shalar", FALSE,	23625,		30, 		1},
        {"bounty",  "[BOUNTY] ", ", Neophyte Hunter",    "Bounty Hunters, Mercenaries of Shalar", FALSE,23601,	40, 		1},
        {"theatre", "[THEATRE] ","",		"Regal Theatre",		FALSE,		23712,		1, 		1},
        {"phalanx", "[PHALANX] ","",		"Phalanx of Fire", 		FALSE,		23877,		30,		1},
    };

const struct cabal_message cabal_messages[MAX_CABAL] =
    {
        // login, logout, entry message //
        { 	"", "", "" },
        { 	"%s has joined the Hunt.",
           "%s has left from the Hunt.",
           "May the darkness conceal you." },
        { 	"Our wisdom grows as %s enters the lands.",
           "The search for knowledge continues, though %s leaves the lands.",
           "Greetings, seeker of knowledge." },
        { 	"Welcome %s. You bring us strength.",
           "Farewell %s. May your strength return to us soon.",
           "Welcome, great warrior." },
        { 	"%s enters the Bounty Guild Halls.",
           "%s departs the Bounty Guild Halls.",
           "Greetings, Hunter." },
        { 	"%s enters the Guild.",
           "%s departs the Guild.",
           "Hail, brother." },
        {   "The Flame of Purity blazes brighter as %s enters the Temple.",
            "The Flame of Purity dims as %s departs the Temple.",
            "Greetings, child of the Light." },
    };

/* for position */
const struct position_type position_table[] =
    {
        {	"dead",				"dead"	},
        {	"mortally wounded",	"mort"	},
        {	"incapacitated",	"incap"	},
        {	"stunned",			"stun"	},
        {	"sleeping",			"sleep"	},
        {	"resting",			"rest"	},
        {   "sitting",			"sit"   },
        {	"fighting",			"fight"	},
        {	"standing",			"stand"	},
        {	NULL,				NULL	}
    };

/* for sex */
const struct sex_type sex_table[] =
    {
        {	"asexual"	},
        {	"male"		},
        {	"female"	},
        {	"either"	},
        {	NULL		}
    };

/* for sizes */
const struct size_type size_table[] =
    {
        {	"tiny"		},
        {	"small" 	},
        {	"medium"	},
        {	"large"		},
        {	"huge", 	},
        {	"giant" 	},
        {   "immense"	},
        {	NULL		}
    };

/* various flag tables */
const struct flag_type act_flags[] =
    {
        {	"npc",				A,	FALSE	},
        {	"sentinel",			B,	TRUE	},
        {	"scavenger",		C,	TRUE	},
        {	"ward_mob",			D,	TRUE	},
        {	"wander",			E,	TRUE	},
        {	"aggressive",		F,	TRUE	},
        {	"stay_area",		G,	TRUE	},
        {	"wimpy",			H,	TRUE	},
        {	"pet",				I,	TRUE	},
        {	"train",			J,	TRUE	},
        {	"practice",			K,	TRUE	},
        {	"smarttrack",		L,	TRUE	},
        {	"undead",			O,	TRUE	},
        {	"inner_guardian",	P,	TRUE	},
        {	"cleric",			Q,	TRUE	},
        {	"mage",				R,	TRUE	},
        {	"intelligent",		S,	TRUE	},
        {	"fast_track",		T,	TRUE	},
        {	"noalign",			U,	TRUE	},
        {	"nopurge",			V,	TRUE	},
        {	"outdoors",			W,	TRUE	},
        {	"indoors",			Y,	TRUE	},
        {   	"guildguard",		Z,	TRUE	},
        {	"healer",			aa,	TRUE	},
        {	"gain",				bb,	TRUE	},
        {	"update_always",	cc,	TRUE	},
        {	"detect_special",	dd,	TRUE	},
        {	"banker",			ee,	TRUE	},
        {	"nocturnal",		ff,	TRUE	},
        {	"diurnal",			gg, TRUE	},
        {	"fastwander",		hh, TRUE	},
        {	"law",				ii, TRUE	},
        {	NULL,				0,	FALSE	}
    };

const struct flag_type plr_flags[] =
    {
        {	"npc",				A,	FALSE	},
        {	"autoabort",			B,	FALSE	},
        {	"autoassist",			C,	FALSE	},
        {	"autoexit",			D,	FALSE	},
        {	"autoloot",			E,	FALSE	},
        {	"autosac",			F,	FALSE	},
        {	"autogold",			G,	FALSE	},
        {	"autosplit",			H,	FALSE	},
	{	"ignorant",			PLR_IGNORANT,	TRUE	},
        {	"betrayer",			K,	TRUE	},
        {	"coder",			PLR_CODER,	TRUE	},
        {	"heroimm",			M,	TRUE	},
        {	"holylight",			N,	FALSE	},
        {	"empowered",			O,	FALSE	},
        {	"no_void",			P,	FALSE	},
        {	"nosummon",			Q,	FALSE	},
        {	"nofollow",			R,	FALSE	},
        {	"no_transfer",		T,	TRUE	},
        {	"permit",			U,	TRUE	},
        {	"moron",			V,	TRUE	},
        {	"log",				W,	FALSE	},
        {	"deny",				X,	TRUE	},
        {	"freeze",			Y,	FALSE	},
        {	"thief",			Z,	FALSE	},
        {	"killer",			aa,	FALSE	},
        {	"criminal",			bb,	FALSE	},
        {	NULL,				0,	FALSE	}
    };

const struct flag_type affect_flags[] =
    {
        {	"blind",			A,	TRUE	},
        {	"invisible",		B,	TRUE	},
        {	"detect_evil",		C,	TRUE	},
        {	"detect_invis",		D,	TRUE	},
        {	"detect_magic",		E,	TRUE	},
        {	"detect_hidden",	F,	TRUE	},
        {	"detect_good",		G,	TRUE	},
        {	"sanctuary",		H,	TRUE	},
        {	"detect_camo",		I,	TRUE	},
        {	"infrared",			J,	TRUE	},
        {	"curse",			K,	TRUE	},
        {   "camouflage",		L,	TRUE	},
        {	"poison",			M,	TRUE	},
        {	"protection",		N,	TRUE	},
        {	"rage",				O,	TRUE	},
        {	"sneak",			P,	TRUE	},
        {	"hide",				Q,	TRUE	},
        {	"sleep",			R,	TRUE	},
        {	"charm",			S,	TRUE	},
        {	"flying",			T,	TRUE	},
        {	"pass_door",		U,	TRUE	},
        {	"haste",			V,	TRUE	},
        {	"calm",				W,	TRUE	},
        {	"plague",			X,	TRUE	},
        {	"permanent",		Y,	TRUE	},
        {	"dark_vision",		Z,	TRUE	},
        {	"berserk",			aa,	TRUE	},
        {	"waterbreath",		bb,	TRUE	},
        {	"regeneration",		cc,	TRUE	},
        {	"slow",				dd,	TRUE	},
        {	"noshow",			ee,	TRUE	},
        {	"darksanc",			ff, TRUE	},
        {	NULL,				0,	FALSE	}
    };

const struct flag_type off_flags[] =
    {
        {	"area_attack",		A,	TRUE	},
        {	"backstab",			B,	TRUE	},
        {	"bash",				C,	TRUE	},
        {	"berserk",			D,	TRUE	},
        {	"disarm",			E,	TRUE	},
        {	"dodge",			F,	TRUE	},
        {	"fade",				G,	TRUE	},
        {	"fast",				H,	TRUE	},
        {	"kick",				I,	TRUE	},
        {	"dirt_kick",		J,	TRUE	},
        {	"parry",			K,	TRUE	},
        {	"rescue",			L,	TRUE	},
        {	"tail",				M,	TRUE	},
        {	"trip",				N,	TRUE	},
        {	"crush",			O,	TRUE	},
        {	"assist_all",		P,	TRUE	},
        {	"assist_align",		Q,	TRUE	},
        {	"assist_race",		R,	TRUE	},
        {	"assist_players",	S,	TRUE	},
        {	"assist_guard",		T,	TRUE	},
        {	"assist_vnum",		U,	TRUE	},
        {   "no_track",			V,	TRUE	},
        {	"static_track",		W,	TRUE	},
        {	"spam_murder",		X,	TRUE	},
        {	"intimidated",		Y,	TRUE	},
        {	"unused",			Z,	TRUE	},
        {	"assist_group",		aa, TRUE	},
        {	NULL,				0,	0		}
    };

const struct flag_type imm_flags[] =
    {
        {	"summon",		A,	TRUE	},
        {	"charm",		B,	TRUE	},
        {	"magic",		C,	TRUE	},
        {	"weapon",		D,	TRUE	},
        {	"bash",			E,	TRUE	},
        {	"pierce",		F,	TRUE	},
        {	"slash",		G,	TRUE	},
        {	"fire",			H,	TRUE	},
        {	"cold",			I,	TRUE	},
        {	"lightning",	J,	TRUE	},
        {	"acid",			K,	TRUE	},
        {	"poison",		L,	TRUE	},
        {	"negative",		M,	TRUE	},
        {	"holy",			N,	TRUE	},
        {	"energy",		O,	TRUE	},
        {	"mental",		P,	TRUE	},
        {	"disease",		Q,	TRUE	},
        {	"drowning",		R,	TRUE	},
        {	"light",		S,	TRUE	},
        {	"sound",		T,	TRUE	},
        {	"mithril",		X,	TRUE	},
        {	"silver",		Y,	TRUE	},
        {	"iron",			Z,	TRUE	},
        {   "sleep",		aa,	TRUE	},
        {	NULL,			0,	0		}
    };

const struct flag_type extra_flags[] =
    {
        {	"glow",			ITEM_GLOW,			TRUE	},
        {	"hum",			ITEM_HUM,			TRUE	},
        {	"dark",			ITEM_DARK,			TRUE	},
        {	"noshow",		ITEM_NOSHOW,		TRUE	},
        {	"evil",			ITEM_EVIL,			TRUE	},
        {	"invis",		ITEM_INVIS,			TRUE	},
        {	"magic",		ITEM_MAGIC,			TRUE	},
        {	"nodrop",		ITEM_NODROP,		TRUE	},
        {	"bless",		ITEM_BLESS,			TRUE	},
        {	"antigood",		ITEM_ANTI_GOOD,		TRUE	},
        {	"antievil",		ITEM_ANTI_EVIL,		TRUE	},
        {	"antineutral",	ITEM_ANTI_NEUTRAL,	TRUE	},
        {	"noremove",		ITEM_NOREMOVE,		TRUE	},
        {	"inventory",	ITEM_INVENTORY,		TRUE	},
        {	"nopurge",		ITEM_NOPURGE,		TRUE	},
        {	"rotdeath",		ITEM_ROT_DEATH,		TRUE	},
        {	"visdeath",		ITEM_VIS_DEATH,		TRUE	},
        {	"fixed",		ITEM_FIXED,			TRUE	},
        {	"nodisarm",		ITEM_NODISARM,		TRUE	},
        {	"nolocate",		ITEM_NOLOCATE,		TRUE	},
        {	"meltdrop",		ITEM_MELT_DROP,		TRUE	},
        {	"under_clothes",ITEM_UNDER_CLOTHES,	TRUE	},
        {	"sellextract",	ITEM_SELL_EXTRACT,	TRUE	},
        {	"burnproof",	ITEM_BURN_PROOF,	TRUE	},
        {	"nouncurse",	ITEM_NOUNCURSE,		TRUE	},
        {	"brand",		ITEM_BRAND,			TRUE	},
        {	"noanimate",	CORPSE_NO_ANIMATE,	TRUE	},
        {	"antilawful",	ITEM_ANTI_LAWFUL,	TRUE	},
        {	"antineutral",	ITEM_ANTI_NEUT,		TRUE	},
        {	"antichaotic",	ITEM_ANTI_CHAOTIC,	TRUE	},
        {	"nostash",		ITEM_NO_STASH,		TRUE	},
        {	"nosac",		ITEM_NO_SAC,		TRUE	},
        {	"donation",		ITEM_DONATION_PIT,	TRUE	},
        {	NULL,			0,					0		}
    };

const struct flag_type wear_locations[] =
    {
        {	"light",		WEAR_LIGHT,		TRUE	},
        {	"lfinger",		WEAR_FINGER_L,	TRUE	},
        {	"rfinger",		WEAR_FINGER_R,	TRUE	},
        {	"neckone",		WEAR_NECK_1,	TRUE	},
        {	"necktwo",		WEAR_NECK_2,	TRUE	},
        {	"body",			WEAR_BODY,		TRUE	},
        {	"head",			WEAR_HEAD,		TRUE	},
        {	"legs",			WEAR_LEGS,		TRUE	},
        {	"feet",			WEAR_FEET,		TRUE	},
        {	"hands",		WEAR_HANDS,		TRUE	},
        {	"arms",			WEAR_ARMS,		TRUE	},
        {	"shield",		WEAR_SHIELD,	TRUE	},
        {	"about",		WEAR_ABOUT,		TRUE	},
        {	"waist",		WEAR_WAIST,		TRUE	},
        {	"wristone",		WEAR_WRIST_R,	TRUE	},
        {	"wristtwo",		WEAR_WRIST_L,	TRUE	},
        {	"wield",		WEAR_WIELD,		TRUE	},
        {	"dualwield",	WEAR_DUAL_WIELD,TRUE	},
        {	"tattooed",		WEAR_BRAND,		TRUE	},
        {	"hold",			WEAR_HOLD,		TRUE	},
    };

const struct flag_type wear_flags[] =
    {
        {	"take",			A,	TRUE	},
        {	"finger",		B,	TRUE	},
        {	"neck",			C,	TRUE	},
        {	"body",			D,	TRUE	},
        {	"head",			E,	TRUE	},
        {	"legs",			F,	TRUE	},
        {	"feet",			G,	TRUE	},
        {	"hands",		H,	TRUE	},
        {	"arms",			I,	TRUE	},
        {	"shield",		J,	TRUE	},
        {	"about",		K,	TRUE	},
        {	"waist",		L,	TRUE	},
        {	"wrist",		M,	TRUE	},
        {	"wield",		N,	TRUE	},
        {	"hold",			O,	TRUE	},
        {	"float",		P,	TRUE	},
        {	"brand",		Q,	TRUE	},
        {	"strapped",		R,	TRUE	},
        {	"cosmetic",		S,	TRUE	},
        {	NULL,			0,	0		}
    };


const struct flag_type form_flags[] =
    {
        {	"edible",		FORM_EDIBLE,		TRUE	},
        {	"poison",		FORM_POISON,		TRUE	},
        {	"magical",		FORM_MAGICAL,		TRUE	},
        {	"instant_decay",FORM_INSTANT_DECAY,	TRUE	},
        {	"other",		FORM_OTHER,			TRUE	},
        {	"animal",		FORM_ANIMAL,		TRUE	},
        {	"sentient",		FORM_SENTIENT,		TRUE	},
        {	"undead",		FORM_UNDEAD,		TRUE	},
        {	"construct",	FORM_CONSTRUCT,		TRUE	},
        {	"mist",			FORM_MIST,			TRUE	},
        {	"intangible",	FORM_INTANGIBLE,	TRUE	},
        {	"biped",		FORM_BIPED,			TRUE	},
        {	"aquatic",		FORM_AQUATIC,		TRUE	},
        {	"insect",		FORM_INSECT,		TRUE	},
        {	"spider",		FORM_SPIDER,		TRUE	},
        {	"crustacean",	FORM_CRUSTACEAN,	TRUE	},
        {	"worm",			FORM_WORM,			TRUE	},
        {	"blob",			FORM_BLOB,			TRUE	},
        {	"mammal",		FORM_MAMMAL,		TRUE	},
        {	"bird",			FORM_BIRD,			TRUE	},
        {	"reptile",		FORM_REPTILE,		TRUE	},
        {	"snake",		FORM_SNAKE,			TRUE	},
        {	"dragon",		FORM_DRAGON,		TRUE	},
        {	"amphibian",	FORM_AMPHIBIAN,		TRUE	},
        {	"fish",			FORM_FISH ,			TRUE	},
        {	"cold_blood",	FORM_COLD_BLOOD,	TRUE	},
        {   "nospeech",		FORM_NOSPEECH,		TRUE	},
        {	NULL,			0,					0		}
    };

const struct flag_type part_flags[] =
    {
        {	"head",			PART_HEAD,			TRUE	},
        {	"arms",			PART_ARMS,			TRUE	},
        {	"legs",			PART_LEGS,			TRUE	},
        {	"heart",		PART_HEART,			TRUE	},
        {	"brains",		PART_BRAINS,		TRUE	},
        {	"guts",			PART_GUTS,			TRUE	},
        {	"hands",		PART_HANDS,			TRUE	},
        {	"feet",			PART_FEET,			TRUE	},
        {	"fingers",		PART_FINGERS,		TRUE	},
        {	"ear",			PART_EAR,			TRUE	},
        {	"eye",			PART_EYE,			TRUE	},
        {	"long_tongue",	PART_LONG_TONGUE,	TRUE	},
        {	"eyestalks",	PART_EYESTALKS,		TRUE	},
        {	"tentacles",	PART_TENTACLES,		TRUE	},
        {	"fins",			PART_FINS,			TRUE	},
        {	"wings",		PART_WINGS,			TRUE	},
        {	"tail",			PART_TAIL,			TRUE	},
        {	"claws",		PART_CLAWS,			TRUE	},
        {	"fangs",		PART_FANGS,			TRUE	},
        {	"horns",		PART_HORNS,			TRUE	},
        {	"scales",		PART_SCALES,		TRUE	},
        {	"tusks",		PART_TUSKS,			TRUE	},
        {	NULL,			0,					0		}
    };

const struct flag_type comm_flags[] =
    {
        {	"quiet",		COMM_QUIET,			TRUE	},
        {   "deaf",			COMM_DEAF,			TRUE	},
        {   "nowiz",		COMM_NOWIZ,			TRUE	},
        {   "noauction",    COMM_NOAUCTION,     TRUE    },
        {   "nogossip",		COMM_NOGOSSIP,		TRUE	},
        {   "noquestion",	COMM_NOQUESTION,	TRUE	},
        {   "nonewbie",		COMM_NONEWBIE,		TRUE	},
        {   "nocabal",      COMM_NOCABAL,       TRUE    },
        {   "noquote",		COMM_NOQUOTE,		TRUE	},
        {   "shoutsoff",	COMM_SHOUTSOFF,		TRUE	},
        {   "compact",		COMM_COMPACT,		TRUE	},
        {   "brief",		COMM_BRIEF,			TRUE	},
        {   "prompt",		COMM_PROMPT,		TRUE	},
        {   "combine",		COMM_COMBINE,		TRUE	},
        {   "telnet_ga",	COMM_TELNET_GA,		TRUE	},
        {   "show_affects",	COMM_SHOW_AFFECTS,	TRUE	},
        {   "noemote",		COMM_NOEMOTE,		TRUE	},
        {   "noshout",		COMM_NOSHOUT,		TRUE	},
        {   "notell",		COMM_NOTELL,		FALSE	},
        {   "nochannels",	COMM_NOCHANNELS,	FALSE	},
        {   "snoop_proof",	COMM_SNOOP_PROOF,	FALSE	},
        {   "afk",			COMM_AFK,			TRUE	},
        {	"nosocket",		COMM_NOSOCKET,		FALSE	},
        {	"switchskills",	COMM_SWITCHSKILLS,	TRUE   },
        {	"lots_o_color",		COMM_LOTS_O_COLOR,	TRUE   },
        {	NULL,			0,					0	}
    };

const struct flag_type criterion_flags[] =
    {
        { "class",			BAR_CLASS,		TRUE	},
        { "cabal",			BAR_CABAL,		TRUE	},
        { "size",			BAR_SIZE,		TRUE	},
        { "tattoo",			BAR_TATTOO,		TRUE	},
        { "level",			BAR_LEVEL,		TRUE	},
        { NULL,				0,				TRUE	},
    };

const struct flag_type trap_table[] =
    {
        { "none",			TRAP_NONE,		TRUE	},
        { "dart",			TRAP_DART,		TRUE	},
        { "poison dart",	TRAP_PDART,		TRUE	},
        { "fireball",		TRAP_FIREBALL,	TRUE	},
        { "lightning",		TRAP_LIGHTNING,	TRUE	},
        { "sleep gas",		TRAP_SLEEPGAS,	TRUE	},
        { "poison gas",		TRAP_POISONGAS,	TRUE	},
        { "acid",			TRAP_ACID,		TRUE	},
        { "concealed pit",	TRAP_PIT,		TRUE	},
        { "boulder",		TRAP_BOULDER,	TRUE	},
        { "mana drain",		TRAP_DRAIN,		TRUE	},
        { NULL,				0,				TRUE	},
    };

const struct flag_type speech_table[] =
    {
        { "say",			SPEECH_SAY,		TRUE	},
        { "sayto",			SPEECH_SAYTO,	TRUE	},
        { "tell",			SPEECH_TELL,	TRUE	},
        { "whisper",		SPEECH_WHISPER,	TRUE	},
        { "yell",			SPEECH_YELL,	TRUE	},
        { "emote",			SPEECH_EMOTE,	TRUE	},
        { "echo",			SPEECH_ECHO,	TRUE	},
        { "sing",			SPEECH_SING,	TRUE	},
        { NULL,				0,				TRUE	},

    };

const struct bounty_type bounty_table[MAX_BOUNTY] =
    {
        /* rank name,	extitle,						credits required */
        {   "neophyte",	"Neophyte Hunter", 			0		},
        {   "hunter", 	"Bounty Hunter", 			100		},
        {   "veteran",	"Veteran Hunter",			300	},
        {    "elder",	"Elder Hunter",				500	},
        {    "master",	"Master Hunter",			1000	},
    };

const struct hometown_type hometown_table[MAX_HOMETOWN] =
    {
        /* City Name	Recall	Pit	Align Restrict	Ethos Restrict	Race/Class Restrict */
        {"",		0,	0,	ALIGN_NONE,	ETHOS_NONE,		0	},
        {"Cimar",	2538,	2796,	ALIGN_ANY,	ETHOS_ANY,		0	},
        {"Melcene",    	1084,   1099,   ALIGN_ANY,      ALIGN_ANY,      	0   	},
        {"Iseldheim",	3874,	3875,	ALIGN_ANY,	ALIGN_ANY,		0	},
        {"Asgard",	1213,	1200,	ALIGN_NONE,	ETHOS_NONE,		0	},
        //REMEMBER TO INCREMENT/DECREMENT MAX_HOMETOWN WHEN YOU FUCK AROUND HERE
        //Yes, this means you Zethus.						-Cal
    };

const struct color_type color_table[MAX_COLORS] =
    {
        /* Color name	Associated Ascii Value */
        {"uncolored",	"\x01B[0m" 		},
        {"red",			"\x01B[0;31m" 	},
        {"green",		"\x01B[0;32m" 	},
        {"yellow",		"\x01B[0;33m" 	},
        {"blue",		"\x01B[0;34m" 	},
        {"magenta",		"\x01B[0;35m" 	},
        {"cyan",		"\x01B[0;36m" 	},
        {"lightgrey",	"\x01B[0;37m" 	},
        {"darkgrey",	"\x01B[1;30m" 	},
        {"lightred",	"\x01B[1;31m" 	},
        {"lightgreen",	"\x01B[1;32m" 	},
        {"lightyellow",	"\x01B[1;33m" 	},
        {"lightblue",	"\x01B[1;34m" 	},
        {"lightmagenta","\x01B[1;35m" 	},
        {"lightcyan",	"\x01B[1;36m" 	},
        {"white",		"\x01B[1;37m" 	},
    };
const struct color_event_type color_event[MAX_EVENTS] =
    {
        /* Event Name		Default Color   Minimum Level */
        {"prompt",		"uncolored",	1  },
        {"channels",	"darkgrey",		1  },
        {"tells",		"lightgreen",	1  },
        {"grouptells",	"magenta",		1  },
        {"speech",		"lightyellow",	1  },
        {"damage",		"lightred",		1  },
        {"yells",		"lightcyan",	1  },
        {"song",		"blue",			1  },
        {"immtalk",		"cyan",			52 },
        {"wiznet",		"uncolored",	52 },
        {"prays",		"lightred",		53 },
        {"builder",		"lightgrey",	51 },
        {"newbie",		"lightblue",	1  },
    };

const struct sect_type sect_table[] =
    {
        /* keyword			number			moves	wait */
        {"unused",			0,					1,	1 },
        {"city",			SECT_CITY,			1,	1 },
        {"field",			SECT_FIELD,			2,	1 },
        {"forest",			SECT_FOREST,			3,	2 },
        {"hills",			SECT_HILLS,			5,	3 },
        {"mountains",			SECT_MOUNTAIN,			6,	4 },
        {"water",			SECT_WATER,			3,	3 },
        {"inside",   			SECT_INSIDE,    		2,  	2 },
        {"underwater",			SECT_UNDERWATER,		4,	2 },
        {"air",				SECT_AIR,			3,	2 },
        {"desert",			SECT_DESERT,			4,	2 },
        {"road",			SECT_ROAD,			1,	1 },
        {"burning",			SECT_BURNING,			5,	2 },
        {"conflagration",		SECT_CONFLAGRATION,		5,	2 },
        {"trail", 			SECT_TRAIL, 			2,  	1 },
        {"swamp",			SECT_SWAMP,			4,	3 },
        {"park",			SECT_PARK,			2,	1 },
        {"vertical",			SECT_VERTICAL,			8, 	4 },
        {"ice",				SECT_ICE,			2,	3 },
        {"snow",			SECT_SNOW,			6,	2 },
        {"cave",			SECT_CAVE,			2,  	2 },
        {NULL,				0,				0,	0 },
    };

const struct flag_type room_flags[] =
    {
        {"dark",			ROOM_DARK,				TRUE},
        {"no_mob",			ROOM_NO_MOB,    		TRUE},
        {"indoors",			ROOM_INDOORS,			TRUE},
        {"private",			ROOM_PRIVATE,			TRUE},
        {"safe",			ROOM_SAFE,				TRUE},
        {"solitary",		ROOM_SOLITARY,			TRUE},
        {"pet_shop",		ROOM_PET_SHOP,			TRUE},
        {"norecall",		ROOM_NO_RECALL,			TRUE},
        {"imp_only",		ROOM_IMP_ONLY,			TRUE},
        {"god_only",		ROOM_GODS_ONLY,			TRUE},
        {"hero_only",		ROOM_HEROES_ONLY,		TRUE},
        {"newbie_only",		ROOM_NEWBIES_ONLY,		TRUE},
        {"law",				ROOM_LAW,				TRUE},
        {"nowhere",			ROOM_NOWHERE,			TRUE},
        {"no_gate",			ROOM_NO_GATE,			TRUE},
        {"silence",			ROOM_SILENCE,			TRUE},
        {"no_summon_to",	ROOM_NO_SUMMON_TO,		TRUE},
        {"no_summon_from",	ROOM_NO_SUMMON_FROM,	TRUE},
        {"no_alarm",		ROOM_NO_ALARM,			TRUE},
        {"force_duel",		ROOM_FORCE_DUEL,		TRUE},
        {"no_magic",		ROOM_NO_MAGIC,			TRUE},
        {"explore",			ROOM_AREA_EXPLORE,		TRUE},
        {"no_commune",		ROOM_NO_COMMUNE,		TRUE},
        {NULL,				0,						TRUE},
    };

const struct flag_type direction_table [] =
    {
        {"north",	DIR_NORTH,	TRUE },
        {"east",	DIR_EAST,	TRUE },
        {"south",  	DIR_SOUTH, 	TRUE },
        {"west",	DIR_WEST,	TRUE },
        {"up",		DIR_UP,		TRUE },
        {"down",	DIR_DOWN,	TRUE },
        {NULL,		0,			TRUE },
    };

const struct flag_type exit_flags [] =
    {
        {"isdoor",		EX_ISDOOR,		TRUE },
        {"closed",		EX_CLOSED,		TRUE },
        {"locked",		EX_LOCKED,		TRUE },
        {"nopick",		EX_PICKPROOF,	TRUE },
        {"nopass",		EX_NOPASS,		TRUE },
        {"noclose",		EX_NOCLOSE,		TRUE },
        {"nolock",		EX_NOLOCK,		TRUE },
        {"nobash",		EX_NOBASH,		TRUE },
        {"nonobvious",	EX_NONOBVIOUS,	TRUE },
        {"translucent",	EX_TRANSLUCENT,	TRUE },
        {"jammed",		EX_JAMMED,		TRUE },
        {NULL,			0,				TRUE },
    };

const struct flag_type area_type_table[] =
    {
        {"normal",		ARE_NORMAL,		TRUE },
        {"road_river",	ARE_ROAD_RIVER, TRUE },
        {"cabal",		ARE_CABAL,		TRUE },
        {"quest",		ARE_QUEST,		TRUE },
        {"city",		ARE_CITY,		TRUE },
        {"unopened",	ARE_UNOPENED,	TRUE },
        {"shrine",		ARE_SHRINE,		TRUE },
        {NULL,			0,				TRUE },
    };

const struct flag_type sky_table[MAX_SKY] =
    {
        {"clear",		SKY_CLEAR,		TRUE },
        {"partlycloudy",SKY_PCLOUDY,	TRUE },
        {"overcast",	SKY_OVERCAST,	TRUE },
        {"drizzle",		SKY_DRIZZLE,	TRUE },
        {"downpour",	SKY_DOWNPOUR,	TRUE },
        {"thunderstorm",SKY_TSTORM,		TRUE },
        {"flurry",		SKY_FLURRY,		TRUE },
        {"blizzard",	SKY_BLIZZARD,	TRUE },
        {"hail",		SKY_HAIL,		TRUE },
    };

const struct flag_type temp_table[MAX_TEMP] =
    {
        {"hot",			TEMP_HOT,		TRUE },
        {"warm",		TEMP_WARM,		TRUE },
        {"cool",		TEMP_COOL,		TRUE },
        {"cold",		TEMP_COLD,		TRUE },
    };

const struct flag_type wind_table[MAX_WIND] =
    {
        {"calm",		WIND_CALM,		TRUE },
        {"breeze",		WIND_BREEZE,	TRUE },
        {"windy",		WIND_WINDY,		TRUE },
        {"gale",		WIND_GALE,		TRUE },
    };

const struct restrict_type restrict_table[] =
    {
        {"human_only",			A,	RESTRICT_RACE,	"human"			},
        {"dwarf_only",			B,	RESTRICT_RACE,	"dwarf"			},
        {"elf_only",			C,	RESTRICT_RACE,	"elf"			},
        {"drow_only",			D,	RESTRICT_RACE,	"drow"			},
        {"duergar_only",		E,	RESTRICT_RACE,	"duergar"		},
        {"imp_only",			F,	RESTRICT_RACE,	"imp"			},
        {"halfling_only",		G,	RESTRICT_RACE,	"halfling"		},
        {"titan_only",			H,	RESTRICT_RACE,	"titan"			},
        {"minotaur_only",		I,	RESTRICT_RACE,	"minotaur"		},
        {"sidhe_only",			J,	RESTRICT_RACE,	"sidhe"			},
        {"warrior_only",		K,	RESTRICT_CLASS,	"warrior"		},
        {"thief_only",			L,	RESTRICT_CLASS,	"thief"			},
        {"zealot_only",			M,	RESTRICT_CLASS,	"zealot"		},
        {"paladin_only",		N,	RESTRICT_CLASS,	"paladin"		},
        {"anti_paladin_only",	O,	RESTRICT_CLASS,	"anti-paladin"	},
        {"ranger_only",			P,	RESTRICT_CLASS,	"ranger"		},
        {"monk_only",			Q,	RESTRICT_CLASS,	"monk"			},
        {"shapeshifter_only",	R,	RESTRICT_CLASS,	"shapeshifter"	},
        {"assassin_only",		S,	RESTRICT_CLASS,	"assassin"		},
        {"necromancer_only",	T,	RESTRICT_CLASS,	"necromancer"	},
        {"sorcerer_only",		U,	RESTRICT_CLASS,	"sorcerer"		},
        {"scion_only",			W,	RESTRICT_CABAL,	"scion"			},
        {"horde_only",			X,	RESTRICT_CABAL,	"horde"			},
        {"theatre_only",		Y,	RESTRICT_CABAL,	"theatre"		},
        {"bounty_only",			Z,	RESTRICT_CABAL,	"bounty"		},
        {"mage_only",			aa,	RESTRICT_OTHER,	""				},
        {"newbie_only",			bb,	RESTRICT_OTHER,	""				},
        {"female_only",			cc,	RESTRICT_OTHER,	""				},
        {"male_only",			dd,	RESTRICT_OTHER,	""				},
        {NULL,					0,	0,				""				},
    };

const struct tribe_type tribe_table[] =
    {
        {"none",	"none",						TRIBE_NONE		},
        {"boar",	"Tribe of the Boar",		TRIBE_BOAR,		},
        {"wolf",	"Tribe of the Wolf",		TRIBE_WOLF,		},
        {"bear",	"Tribe of the Bear",		TRIBE_BEAR,		},
        {"hawk",	"Tribe of the Hawk",		TRIBE_HAWK,		},
        {"lion",	"Tribe of the Lion",		TRIBE_LION,		},
        {"elk",		"Tribe of the Elk",			TRIBE_ELK,		},
        {"jackal",	"Tribe of the Jackal",		TRIBE_JACKAL,	},
        {"fox",		"Tribe of the Fox",			TRIBE_FOX,		},
        {"bull",	"Tribe of the Bull",		TRIBE_BULL,		},
        {"panther",	"Tribe of the Panther",		TRIBE_PANTHER,	},
        {NULL,		NULL,						TRIBE_NONE,		},
    };

char *  const   day_name        [] =
    {
        "of Toil", "of Grain", "of Iron", "of Hunting", "of Feasting", "of Rest",
    };

char *  const   month_name      [] =
    {
        "of the Glacier", "of the Arrival", "of the Torrent", "of the Mountain Stand",
        "of the Quickening", "of the Great Siege", "of the Molten Stone", "of the Holy Descent",
        "of the Ashen Earth", "of the Crumbling Wall", "of the Whirling Wind",
        "of the Triumph"
    };

char *  const   season_name	[] =
    {
        "of Darkness", "of Birth", "of Light", "of Death",
    };

const struct para_sphere para_list[] =
    {
        {	"heat",		"air",			"smoke"		},
        {	"heat",		"earth",		"magma"		},
        {	"heat",		"electricity",	"plasma"	},
        {	"heat",		"water",		"acid"		},
        {	"cold",		"air",			"blizzard"	},
        {	"cold",		"earth",		"frost"		},
        {	"cold",		"electricity",	"crystal"	},
        {	"cold",		"water",		"glacier"	},
        {	"air",		"electricity",	"lightning"	},
        {	"air",		"water",		"mist"		},
        {	"earth",	"electricity",	"metal"		},
        {	"earth",	"water",		"ooze"		},
    };

const struct ele_sphere sphere_table[] =
    {
        {	ELE_HEAT,		ELE_TYPE_PRIMARY,	"Heat"	},
        {	ELE_COLD,		ELE_TYPE_PRIMARY,	"Cold"	},
        {	ELE_AIR,		ELE_TYPE_PRIMARY,	"Air"	},
        {	ELE_EARTH,		ELE_TYPE_PRIMARY,	"Earth"	},
        {	ELE_WATER,		ELE_TYPE_PRIMARY,	"Water"	},
        {	ELE_ELECTRICITY,ELE_TYPE_PRIMARY,	"Electricity" },
        {	ELE_SMOKE,		ELE_TYPE_PARA,		"Smoke"	},
        {	ELE_MAGMA,		ELE_TYPE_PARA,		"Magma"	},
        {	ELE_PLASMA,		ELE_TYPE_PARA,		"Plasma"},
        {	ELE_ACID,		ELE_TYPE_PARA,		"Acid"	},
        {	ELE_BLIZZARD,	ELE_TYPE_PARA,		"Blizzard"},
        {	ELE_FROST,		ELE_TYPE_PARA,		"Frost"	},
        {	ELE_CRYSTAL,	ELE_TYPE_PARA,		"Crystal"},
        {	ELE_ICE,		ELE_TYPE_PARA,		"Ice"},
        {	ELE_LIGHTNING,	ELE_TYPE_PARA,		"Lightning"},
        {	ELE_MIST,		ELE_TYPE_PARA,		"Mist"	},
        {	ELE_METAL,		ELE_TYPE_PARA,		"Metal"	},
        {	ELE_OOZE,		ELE_TYPE_PARA,		"Ooze"	},
        {	ELE_NONE,		ELE_TYPE_NONE,		"none"	},
    };

const struct style_type style_table[] =
    {
        {	STYLE_NONE,			"none"},
        {	STYLE_GLADIATOR,	"Gladiator"},
        {	STYLE_BARBARIAN,	"Barbarian"},
        {	STYLE_DUELIST,		"Duelist"},
        {	STYLE_SKIRMISHER,	"Skirmisher"},
        {	STYLE_DRAGOON,		"Dragoon"},
        {	STYLE_TACTICIAN,	"Tactician"},
    };

const struct style_list style_percent[] =
    {
        {	"none",					0},
        {	"fend",					5},
        {	"entrap",				15},
        {	"gouge",				30},
        {	"bleed",				50},
        {	"hobble",				60},
        {	"crippling blow",		80},
        {	"brutality",			5},
        {	"shatter",				15},
        {	"overhead",				30},
        {	"impale",				45},
        {	"uppercut",				60},
        {	"batter",				80},
        {	"maneuvering",			5},
        {	"catch",				15},
        {	"unbalance",			30},
        {	"posture",				45},
        {	"sidestep",				55},
        {	"concuss",				65},
        {	"disrupt formation",	80},
        {	"evasion",				5},
        {	"hurl",					15},
        {	"dart",					35},
        {	"entwine",				50},
        {	"pull",					55},
        {	"fifth attack",			65},
        {	"whirlwind",			80},
        {	"ease",					5},
        {	"deflect",				15},
        {	"exchange",				30},
        {	"charge",				40},
        {	"shieldbash",			55},
        {	"brace",				75},
        {	"leadership",			5},
        {	"assess",				20},
        {	"exploit",				35},
        {	"outflank",				50},
        {	"terrain mastery",		60},
        {	"analyze",				85}

    };

const struct cabal_list cabal_skills[] =
    {
        { CABAL_SCION,	"mana transfer",	1, FALSE},
        { CABAL_SCION, 	"scribe",			1, FALSE},
        { CABAL_SCION,	"deny magic",		1, FALSE},
        { CABAL_SCION,	"nullify",			1, FALSE},
        { CABAL_SCION,	"bane of ignorance",1, FALSE},
        { CABAL_HORDE,	"howl",				1, FALSE},
        { CABAL_HORDE,	"epic strength",	1, FALSE},
        { CABAL_HORDE,	"trophy",			1, FALSE},
        { CABAL_HORDE,	"rage",				1, FALSE},
        { CABAL_HORDE,	"calm",				1, FALSE},
        { CABAL_HORDE,	"communion",		10, FALSE},
        { CABAL_BOUNTY,	"hunters strength",	1, FALSE},
        { CABAL_BOUNTY, "hunters awareness",1, FALSE},
        { CABAL_BOUNTY, "hunters stealth",	4, FALSE},
        { CABAL_BOUNTY, "isolation",		5, FALSE},
        { CABAL_THEATRE,"story tell",		1, FALSE},
        { CABAL_GUILD,	"medicine",			1, FALSE},
        { CABAL_GUILD,	"repose",			1, FALSE},
        { CABAL_PHALANX,"fervor",			1, FALSE},
        { CABAL_PHALANX,"piety",			1, FALSE},
        { CABAL_PHALANX,"spiritual healing",2, TRUE},
        { CABAL_PHALANX,"shroud of light",	2, TRUE},
        { CABAL_PHALANX,"safehaven",		2, TRUE},
        { CABAL_PHALANX,"crimson martyr",	3, TRUE},
        { CABAL_PHALANX,"retribution",		3, TRUE},
        { CABAL_PHALANX,"infidels fate",	3, TRUE},
        { CABAL_NONE,	NULL,				0, FALSE},
    };

const struct flag_type wealth_table[] =
    {
        {	"none",		0,		FALSE	},
        {	"poor",		1,		FALSE	},
        {	"moderate",	10,		FALSE	},
        {	"affluent",	100,	FALSE	},
        {	"rich",		1000,	FALSE	},
        {	"peerless",	10000,	FALSE	},
        {	NULL,		0,		FALSE	},
    };


const struct demon_type demon_table[] =
    {
        {	"barbas",		LESSER_BARBAS,		LESSER_DEMON	},
        {	"aamon",		LESSER_AAMON,		LESSER_DEMON	},
        {	"malaphar",		LESSER_MALAPHAR,	LESSER_DEMON	},
        {	"furcas",		LESSER_FURCAS,		LESSER_DEMON	},
        {	"ipos",			LESSER_IPOS,		LESSER_DEMON	},
        {	"oze",			GREATER_OZE,		GREATER_DEMON	},
        {	"gamygyn",		GREATER_GAMYGYN,	GREATER_DEMON	},
        {	"orobas",		GREATER_OROBAS,		GREATER_DEMON	},
        {	"geryon",		GREATER_GERYON,		GREATER_DEMON	},
        {	"cimeries",		GREATER_CIMERIES, 	GREATER_DEMON	},
        {	NULL,			0,					0				},
    };
/* Sky:
 * 0 - Clear
 * 1 - Partly Cloudy
 * 2 - Overcast
 * 3 - Drizzle
 * 4 - Downpour
 * 5 - Thunderstorm
 * 6 - Snow Flurry
 * 7 - Blizzard
 * 8 - Hail
 *
 *
 * Temperature:
 * 0 - Hot
 * 1 - Warm
 * 2 - Cool
 * 3 - Cold
 *
 *
 * Ok, here's how the tables work.  The weather_update () function scans
 * through the relevant arrays for sky/temp, depending on the current season
 * and the regional climate.  It randomizes a number from 1-100, and starts
 * with element 0 of the array and keeps incrementing until it finds a number
 * bigger than the random one, and then it stops... and presto, that's your
 * weather.  So, what does this mean?  The tables contain cumulative
 * probabilities.  The last element of each array MUST be 100.  Weather_update
 * will handle errors properly, but the result isn't going to be what you
 * want.
 *
 * Example: you want to define a pattern that's 20% sunny, 20% overcast, 10%
 * flurries, and 50% blizzard.  It would look like this:
 * {20,20,40,40,40,40,50,100,100}
 *
 * Now say it picks a random number... 44.  It'll compare 44 with each
 * successive element of the array.  20... nope, too small.  Move on to the
 * next.  20 again... nope, too small... 40... still too small... etc... until
 * it reaches 50.  And it'll stop there, since 50 > 44.  And sky = 6, or in
 * other words, snow flurries.
 *
 *                                                    --Eladrian
 */

const struct climate_type climate_table[MAX_CLIMATE] =
    {	/*Climate name*/	/*Climate number*/
        {	"none",				CLIMATE_NONE,
            {	/*Sky frequencies by season*/
                { 0, 0, 0, 0, 0, 0, 0, 0, 0},		/* Winter */
                { 0, 0, 0, 0, 0, 0, 0, 0, 0},		/* Spring */
                { 0, 0, 0, 0, 0, 0, 0, 0, 0},		/* Summer */
                { 0, 0, 0, 0, 0, 0, 0, 0, 0}		/* Autumn */
            },

            {	/*Temperature*/
                { 0, 0, 0, 0},						/* Winter */
                { 0, 0, 0, 0},						/* Spring */
                { 0, 0, 0, 0},						/* Summer */
                { 0, 0, 0, 0}						/* Autumn */
            }
        },

        {	"temperate",		CLIMATE_TEMPERATE,
          {
              {20,40,60,60,60,65,90,95,100},
              {20,40,60,70,80,90,97,98,100},
              {20,40,60,70,90,100,100,100,100},
              {20,40,60,70,80,90,97,98,100}
          },

          {
              { 0, 0,40,100},
              {25,75,100,100},
              {60,100,100,100},
              {25,75,100,100}
          }
        },

        {	"desert",			CLIMATE_DESERT,
          {
              {70,80,80,80,95,100,100,100,100},
              {80,90,90,90,98,100,100,100,100},
              {90,100,100,100,100,100,100,100,100},
              {80,90,90,90,98,100,100,100,100}
          },

          {
              {50,50,100,100},
              {70,80,100,100},
              {90,100,100,100},
              {70,80,100,100}
          }
        },

        {	"tropical",			CLIMATE_TROPICAL,
          {
              { 5,10,30,50,80,100,100,100,100},
              { 5,10,30,50,80,100,100,100,100},
              { 5,10,30,50,80,100,100,100,100},
              { 5,10,30,50,80,100,100,100,100}
          },

          {
              {50,100,100,100},
              {80,100,100,100},
              {100,100,100,100},
              {80,100,100,100}
          }
        },

        {	"alpine",			CLIMATE_ALPINE,
          {
              {20,40,60,60,60,60,70,95,100},
              {20,40,60,60,60,60,80,90,100},
              {20,40,60,70,75,85,95,97,100},
              {20,40,60,60,60,60,80,90,100}
          },

          {
              { 0, 0,30,100},
              { 0,15,60,100},
              { 0,60,100,100},
              { 0,15,60,100}
          }
        },

        {	"tundra",	CLIMATE_TUNDRA,
          {
              {30,40,50,50,50,50,70,95,100},
              {30,40,50,50,50,50,80,90,100},
              {30,40,50,55,55,60,90,93,100},
              {30,40,50,50,50,50,80,90,100}
          },

          {
              { 0, 0, 0,100},
              { 0, 0,10,100},
              { 0,10,50,100},
              { 0, 0,10,100}
          }
        },

        {	"subtropical",			CLIMATE_SUBTROPICAL,
          {
              { 0, 0, 0, 0, 0, 0, 0, 0, 0},
              { 0, 0, 0, 0, 0, 0, 0, 0, 0},
              { 0, 0, 0, 0, 0, 0, 0, 0, 0},
              { 0, 0, 0, 0, 0, 0, 0, 0, 0}
          },

          {
              { 0, 0, 0, 0},
              { 0, 0, 0, 0},
              { 0, 0, 0, 0},
              { 0, 0, 0, 0}
          }
        },


        {	"arid",					CLIMATE_ARID,
          {
              {30,40,50,60,80,96,98,98,100},
              {50,75,85,90,95,100,100,100,100},
              {70,95,100,100,100,100,100,100,100},
              {50,75,85,90,95,100,100,100,100}
          },

          {
              { 0, 0,75,100},
              { 0,50,100,100},
              {80,100,100,100},
              { 0,50,100,100}
          }
        },

        {	"english",				CLIMATE_ENGLISH,
          {
              { 0, 0, 0, 0, 0, 0, 0, 0, 0},
              { 0, 0, 0, 0, 0, 0, 0, 0, 0},
              { 0, 0, 0, 0, 0, 0, 0, 0, 0},
              { 0, 0, 0, 0, 0, 0, 0, 0, 0}
          },

          {
              { 0, 0, 0, 0},
              { 0, 0, 0, 0},
              { 0, 0, 0, 0},
              { 0, 0, 0, 0}
          }
        }

    };

const struct piece_type piece_table[2][MAX_PIECE] =
    {
        {
            {"::::::::",
                ":::\x01B[1;37m()\x01B[0m:::",				/* White pawn */
                "::\x01B[1;37m/__\\\x01B[0m::"},

            {"::\x01B[1;37m^_>>\x01B[0m::",
             "::\x01B[1;37m| \\\x01B[0m:::",				/* White knight */
             "::\x01B[1;37m|__\\\x01B[0m::"},

            {":::\x01B[1;37m/+\x01B[0m:::",
             ":::\x01B[1;37m()\x01B[0m:::",				/* White bishop */
             "::\x01B[1;37m/__\\\x01B[0m::"},

            {"::\x01B[1;37m___\x01B[0m:::",
             ":\x01B[1;37m[ _ ]\x01B[0m::",				/* White Rook */
             ":\x01B[1;37m[_ _]\x01B[0m::"},

            {"::\x01B[1;37m(QQ)\x01B[0m::",
             ":\x01B[1;37m(_  _)\x01B[0m:",				/* White Queen */
             ":\x01B[1;37m\\____/\x01B[0m:"},

            {"::\x01B[1;37m|KK|\x01B[0m::",
             ":\x01B[1;37m|_  _|\x01B[0m:",				/* White King */
             ":\x01B[1;37m\\____/\x01B[0m:"},

            {"::::::::",
             ":::\x01B[0;31m()\x01B[0m:::",				/* Black pawn */
             "::\x01B[0;31m/__\\\x01B[0m::"},

            {"::\x01B[0;31m^_>>\x01B[0m::",
             "::\x01B[0;31m| \\\x01B[0m:::",				/* Black knight */
             "::\x01B[0;31m|__\\\x01B[0m::"},

            {":::\x01B[0;31m/+\x01B[0m:::",
             ":::\x01B[0;31m()\x01B[0m:::",				/* Black bishop */
             "::\x01B[0;31m/__\\\x01B[0m::"},

            {"::\x01B[0;31m___\x01B[0m:::",
             ":\x01B[0;31m[ _ ]\x01B[0m::",				/* Black Rook */
             ":\x01B[0;31m[_ _]\x01B[0m::"},

            {"::\x01B[0;31m(QQ)\x01B[0m::",
             ":\x01B[0;31m(_  _)\x01B[0m:",				/* Black Queen */
             ":\x01B[0;31m\\____/\x01B[0m:"},

            {"::\x01B[0;31m|KK|\x01B[0m::",
             ":\x01B[0;31m|_  _|\x01B[0m:",				/* Black King */
             ":\x01B[0;31m\\____/\x01B[0m:"}
        },

        /* ON BLACK SQUARES */

        {
            {"        ",
             "   \x01B[1;37m()\x01B[0m   ",				/* White pawn */
             "  \x01B[1;37m/__\\\x01B[0m  "},

            {"  \x01B[1;37m^_>>\x01B[0m  ",
             "  \x01B[1;37m| \\\x01B[0m   ",				/* White knight */
             "  \x01B[1;37m|__\\\x01B[0m  "},

            {"   \x01B[1;37m/+\x01B[0m   ",
             "   \x01B[1;37m()\x01B[0m   ",				/* White bishop */
             "  \x01B[1;37m/__\\\x01B[0m  "},

            {"  \x01B[1;37m___\x01B[0m   ",
             " \x01B[1;37m[ _ ]\x01B[0m  ",				/* White Rook */
             " \x01B[1;37m[_ _]\x01B[0m  "},

            {"  \x01B[1;37m(QQ)\x01B[0m  ",
             " \x01B[1;37m(_  _)\x01B[0m ",				/* White Queen */
             " \x01B[1;37m\\____/\x01B[0m "},

            {"  \x01B[1;37m|KK|\x01B[0m  ",
             " \x01B[1;37m|_  _|\x01B[0m ",				/* White King */
             " \x01B[1;37m\\____/\x01B[0m "},

            {"        ",
             "   \x01B[0;31m()\x01B[0m   ",				/* Black pawn */
             "  \x01B[0;31m/__\\\x01B[0m  "},

            {"  \x01B[0;31m^_>>\x01B[0m  ",
             "  \x01B[0;31m| \\\x01B[0m   ",				/* Black knight */
             "  \x01B[0;31m|__\\\x01B[0m  "},

            {"   \x01B[0;31m/+\x01B[0m   ",
             "   \x01B[0;31m()\x01B[0m   ",				/* Black bishop */
             "  \x01B[0;31m/__\\\x01B[0m  "},

            {"  \x01B[0;31m___\x01B[0m   ",
             " \x01B[0;31m[ _ ]\x01B[0m  ",				/* Black Rook */
             " \x01B[0;31m[_ _]\x01B[0m  "},

            {"  \x01B[0;31m(QQ)\x01B[0m  ",
             " \x01B[0;31m(_  _)\x01B[0m ",				/* Black Queen */
             " \x01B[0;31m\\____/\x01B[0m "},

            {"  \x01B[0;31m|KK|\x01B[0m  ",
             " \x01B[0;31m|_  _|\x01B[0m ",				/* Black King */
             " \x01B[0;31m\\____/\x01B[0m "}
        }
    };

const struct beauty_type beauty_table[] =
    {
        { "", 						""						},
        { "unspeakably grotesque",	"unspeakably grotesque" },
        { "disfigured",				"disfigured"			},
        { "ugly",					"ugly"					},
        { "unattractive",			"unattractive"			},
        { "plain",		 			"homely"				},
        { "pleasant-looking",		"pleasant-looking"		},
        { "handsome",				"pretty"				},
        { "striking",				"beautiful",			},
        { "stunning",				"stunning",				},
        { "dazzlingly handsome",	"dazzlingly beautiful"	},
    };

const struct order_list order_table[] =
    {
        { "hit" },
        { "murder" },
        { "get" },
        { "drop"},
        { "put" },
        { "give" },
        { "kill" },
        { "rescue" },
        { "disengage" },
        { "say"	},
        { "emote" },
        { "pmote" },
    };

const struct flag_type area_flags[] =
    {
        { "none",         AREA_NONE,      FALSE   },
        { "explore",	  AREA_EXPLORE,	  TRUE	  },
        { "no_newbies",	  AREA_NO_NEWBIES,TRUE   },
        { "unghost",	  AREA_UNGHOST,	  TRUE   },
        { "changed",      AREA_CHANGED,   TRUE    },
        { "added",        AREA_ADDED,     TRUE    },
        { "loading",      AREA_LOADING,   FALSE   },
        { NULL,           0,          0   }
    };



const struct flag_type sex_flags[] =
    {
        { "male",         SEX_MALE,       TRUE    },
        { "female",       SEX_FEMALE,     TRUE    },
        { "neutral",      SEX_NEUTRAL,        TRUE    },
        {   "random",               3,                      TRUE    },   /* ROM */
        { "none",         SEX_NEUTRAL,        TRUE    },
        { NULL,           0,          0   }
    };


const struct flag_type door_resets[] =
    {
        { "open and unlocked",    0,      TRUE    },
        { "closed and unlocked",  1,      TRUE    },
        { "closed and locked",    2,      TRUE    },
        { NULL,           0,      0   }
    };


const struct flag_type sector_flags[] =
    {
        { "inside",   SECT_INSIDE,        TRUE    },
        { "city",     SECT_CITY,      TRUE    },
        { "field",    SECT_FIELD,     TRUE    },
        { "forest",   SECT_FOREST,        TRUE    },
        { "hills",    SECT_HILLS,     TRUE    },
        { "mountain", SECT_MOUNTAIN,      TRUE    },
        { "water",		SECT_WATER,		TRUE	},
        { "unused", SECT_UNUSED,        TRUE    },
        { "underwater",	SECT_UNDERWATER,	TRUE	},
        { "air",      SECT_AIR,       TRUE    },
        { "desert",   SECT_DESERT,        TRUE    },
        { "road",		SECT_ROAD,		TRUE	},
        { "conflagration",	SECT_CONFLAGRATION,	TRUE},
        { "burning",	SECT_BURNING,		TRUE 	},
        { "trail",		SECT_TRAIL,			TRUE	},
        { "swamp",		SECT_SWAMP,			TRUE	},
        { "park",		SECT_PARK,			TRUE	},
        { "vertical",	SECT_VERTICAL,		TRUE	},
        { "ice",		SECT_ICE,			TRUE	},
        { "snow",		SECT_SNOW,			TRUE	},
        { "cave",		SECT_CAVE,			TRUE	},
        { NULL,       0,          0   }
    };

const struct flag_type type_flags[] =
    {
        { "light",        ITEM_LIGHT,     TRUE    },
        { "scroll",       ITEM_SCROLL,        TRUE    },
        { "wand",         ITEM_WAND,      TRUE    },
        { "staff",        ITEM_STAFF,     TRUE    },
        { "weapon",       ITEM_WEAPON,        TRUE    },
        { "treasure",     ITEM_TREASURE,      TRUE    },
        { "stone",		  ITEM_STONE,		  TRUE},
        { "armor",        ITEM_ARMOR,     TRUE    },
        { "potion",       ITEM_POTION,        TRUE    },
        { "furniture",        ITEM_FURNITURE,     TRUE    },
        { "trash",        ITEM_TRASH,     TRUE    },
        { "container",        ITEM_CONTAINER,     TRUE    },
        { "drinkcontainer",   ITEM_DRINK_CON,     TRUE    },
        { "key",          ITEM_KEY,       TRUE    },
        { "food",         ITEM_FOOD,      TRUE    },
        { "money",        ITEM_MONEY,     TRUE    },
        { "boat",         ITEM_BOAT,      TRUE    },
        { "npccorpse",        ITEM_CORPSE_NPC,    TRUE    },
        { "pc corpse",        ITEM_CORPSE_PC,     FALSE   },
        { "fountain",     ITEM_FOUNTAIN,      TRUE    },
        { "pill",         ITEM_PILL,      TRUE    },
        { "protect",      ITEM_PROTECT,       TRUE    },
        { "map",          ITEM_MAP,       TRUE    },
        { "portal",     ITEM_PORTAL,        TRUE    },
        { "warpstone",      ITEM_WARP_STONE,    TRUE    },
        { "roomkey",      ITEM_ROOM_KEY,      TRUE    },
        { "gem",          ITEM_GEM,       TRUE    },
        { "jewelry",      ITEM_JEWELRY,       TRUE    },
        { "campfire",      ITEM_CAMPFIRE,       TRUE    },
        { "altar",		  ITEM_ALTAR,		  TRUE	  },
        { NULL,           0,          0   }
    };


/*
 * Used when adding an affect to tell where it goes.
 * See addaffect and delaffect in act_olc.c
 */
const struct flag_type apply_flags[] =
    {
        { "none",         APPLY_NONE,     TRUE    },
        { "strength",     APPLY_STR,      TRUE    },
        { "dexterity",        APPLY_DEX,      TRUE    },
        { "intelligence",     APPLY_INT,      TRUE    },
        { "wisdom",       APPLY_WIS,      TRUE    },
        { "constitution",     APPLY_CON,      TRUE    },
        { "sex",          APPLY_SEX,      TRUE    },
        { "class",        APPLY_CLASS,        TRUE    },
        { "age",          APPLY_AGE,      TRUE    },
        { "height",       APPLY_HEIGHT,       TRUE    },
        { "weight",       APPLY_WEIGHT,       TRUE    },
        { "mana",         APPLY_MANA,     TRUE    },
        { "hp",           APPLY_HIT,      TRUE    },
        { "move",         APPLY_MOVE,     TRUE    },
        { "gold",         APPLY_GOLD,     TRUE    },
        { "experience",       APPLY_EXP,      TRUE    },
        { "ac",           APPLY_AC,       TRUE    },
        { "hitroll",      APPLY_HITROLL,      TRUE    },
        { "damroll",      APPLY_DAMROLL,      TRUE    },
        { "saves",        APPLY_SAVES,        TRUE    },
        { "savingpara",       APPLY_SAVING_PARA,  TRUE    },
        { "savingrod",        APPLY_SAVING_ROD,   TRUE    },
        { "savingpetri",      APPLY_SAVING_PETRI, TRUE    },
        { "savingbreath",     APPLY_SAVING_BREATH,    TRUE    },
        { "savingspell",      APPLY_SAVING_SPELL, TRUE    },
        { "spellaffect",      APPLY_SPELL_AFFECT, FALSE   },
        { "beauty",			APPLY_BEAUTY,	TRUE,	},
        { NULL,           0,          0   }
    };
/*
 * What is seen.
 */
const struct flag_type wear_loc_strings[] =
    {
        { "in the inventory", WEAR_NONE,  TRUE    },
        { "as a light",       WEAR_LIGHT, TRUE    },
        { "on the left finger",   WEAR_FINGER_L,  TRUE    },
        { "on the right finger",  WEAR_FINGER_R,  TRUE    },
        { "around the neck (1)",  WEAR_NECK_1,    TRUE    },
        { "around the neck (2)",  WEAR_NECK_2,    TRUE    },
        { "on the body",      WEAR_BODY,  TRUE    },
        { "over the head",    WEAR_HEAD,  TRUE    },
        { "on the legs",      WEAR_LEGS,  TRUE    },
        { "on the feet",      WEAR_FEET,  TRUE    },
        { "on the hands",     WEAR_HANDS, TRUE    },
        { "on the arms",      WEAR_ARMS,  TRUE    },
        { "as a shield",      WEAR_SHIELD,    TRUE    },
        { "about the shoulders",  WEAR_ABOUT, TRUE    },
        { "around the waist", WEAR_WAIST, TRUE    },
        { "on the left wrist",    WEAR_WRIST_L,   TRUE    },
        { "on the right wrist",   WEAR_WRIST_R,   TRUE    },
        { "wielded",      WEAR_WIELD, TRUE    },
        { "held in the hands",    WEAR_HOLD,  TRUE    },
        { "floating nearby",  WEAR_FLOAT, TRUE    },
        { NULL,           0         , 0   }
    };


const struct flag_type wear_loc_flags[] =
    {
        { "none",     WEAR_NONE,  TRUE    },
        { "light",    WEAR_LIGHT, TRUE    },
        { "lfinger",  WEAR_FINGER_L,  TRUE    },
        { "rfinger",  WEAR_FINGER_R,  TRUE    },
        { "neck1",    WEAR_NECK_1,    TRUE    },
        { "neck2",    WEAR_NECK_2,    TRUE    },
        { "body",     WEAR_BODY,  TRUE    },
        { "head",     WEAR_HEAD,  TRUE    },
        { "legs",     WEAR_LEGS,  TRUE    },
        { "feet",     WEAR_FEET,  TRUE    },
        { "hands",    WEAR_HANDS, TRUE    },
        { "arms",     WEAR_ARMS,  TRUE    },
        { "shield",   WEAR_SHIELD,    TRUE    },
        { "about",    WEAR_ABOUT, TRUE    },
        { "waist",    WEAR_WAIST, TRUE    },
        { "lwrist",   WEAR_WRIST_L,   TRUE    },
        { "rwrist",   WEAR_WRIST_R,   TRUE    },
        { "wielded",  WEAR_WIELD, TRUE    },
        { "hold",     WEAR_HOLD,  TRUE    },
        { "floating", WEAR_FLOAT, TRUE    },
        { "tattoo",	  WEAR_BRAND, TRUE	  },
        { NULL,       0,      0   }
    };

const     struct flag_type weapon_flags[]=
    {
        { 	"hit",      	0,      TRUE    },  /*  0 */
        { 	"slice",    	1,      TRUE    },
        {   "stab",       	2,      TRUE    },
        { 	"slash",    	3,      TRUE    },
        { 	"whip",     	4,      TRUE    },
        {   "claw",       	5,      TRUE    },  /*  5 */
        { 	"blast",    	6,      TRUE    },
        {   "pound",  		7,      TRUE    },
        { 	"crush",    	8,      TRUE    },
        {   "grep",       	9,      TRUE    },
        { 	"bite",     	10,     TRUE    },  /* 10 */
        {   "pierce", 		11,     TRUE    },
        {   "suction",    	12,     TRUE    },
        { 	"beating", 		13,     TRUE    },
        {   "digestion",  	14,     TRUE    },
        { 	"charge",   	15,     TRUE    },  /* 15 */
        {   "slap",     	16,     TRUE    },
        { 	"punch",    	17,     TRUE    },
        { 	"wrath",    	18,     TRUE    },
        { 	"magic",    	19,     TRUE    },
        {   "divinepower",  20,     TRUE    },  /* 20 */
        { 	"cleave",   	21,     TRUE    },
        { 	"scratch",  	22,     TRUE    },
        {   "peckpierce", 	23,     TRUE    },
        {   "peckbash",   	24,     TRUE    },
        {   "chop",       	25,     TRUE    },  /* 25 */
        {   "sting",  		26,     TRUE    },
        {   "smash",  		27,     TRUE    },
        {   "shockingbite",	28,     TRUE    },
        { 	"flamingbite", 	29,     TRUE    },
        { 	"freezingbite", 30,     TRUE    },  /* 30 */
        { 	"acidicbite",   31,     TRUE    },
        { 	"chomp",    	32,     TRUE    },
        {   "lifedrain",    33,     TRUE    },
        {   "thrust", 		34,     TRUE    },
        {   "slime",  		35,     TRUE    },	/* 35 */
        { 	"shock",    	36,     TRUE    },
        {   "thwack", 		37,     TRUE    },
        {   "flame",  		38,     TRUE    },
        {   "chill",  		39,     TRUE    },
        {	"lightblast", 	40,		TRUE	},	/* 40 */
        {	"forcenature",	41,		TRUE	},
        {	"defilement",	42,		TRUE	},
        {	"mentalassault",43,		TRUE	},
        {	"sonicblast",	44,		TRUE	},
        {	"infdarkness",	45,		TRUE	},	/* 45 */
        {	"searing",		46,		TRUE	},
        {	"drowning",		47,		TRUE	},
        {	"chaotic",		48,		TRUE	},
        {	"sflame",		49,		TRUE	},
        {	"terror",		50,		TRUE	}, 	/* 50 */
        {	"hydrous",		51,		TRUE	},
        {	"thorn",		52,		TRUE	},
        {	"decomposition",53,		TRUE	},
        {	"ptouch",		54,		TRUE	},
        {	"spike",		55,		TRUE	},	/* 55 */
        {	"ooze",			56,		TRUE	},
        {	"asphyx",		57,		TRUE	},
        {	"sblast",		58,		TRUE	},
        {	"poisbite",		59,		TRUE	},
        {	"foulgrasp",	60,		TRUE	},	/* 60 */
        {   NULL,     		0,      0   }
    };

const struct flag_type container_flags[] =
    {
        { "closeable",    A,      TRUE    },
        { "pickproof",    B,      TRUE    },
        { "closed",       C,      TRUE    },
        { "locked",       D,      TRUE    },
        { "puton",        E,     TRUE    },
        { NULL,           0,      0   }
    };

/*****************************************************************************
                 ROM - specific tables:
 ****************************************************************************/




const struct flag_type ac_type[] =
    {
        {   "pierce",        AC_PIERCE,            TRUE    },
        {   "bash",          AC_BASH,              TRUE    },
        {   "slash",         AC_SLASH,             TRUE    },
        {   "exotic",        AC_EXOTIC,            TRUE    },
        {   NULL,              0,                    0       }
    };


const struct flag_type size_flags[] =
    {
        {   "tiny",          SIZE_TINY,            TRUE    },
        {   "small",         SIZE_SMALL,           TRUE    },
        {   "medium",        SIZE_MEDIUM,          TRUE    },
        {   "large",         SIZE_LARGE,           TRUE    },
        {   "huge",          SIZE_HUGE,            TRUE    },
        {   "giant",         SIZE_GIANT,           TRUE    },
        {   NULL,              0,                    0       },
    };


const struct flag_type weapon_class[] =
    {
        {	"none",			0,			TRUE	},
        {	"exotic",		1,			TRUE    },
        {   "sword",        2,			TRUE    },
        {   "dagger",       3,			TRUE    },
        {   "spear",        4,			TRUE    },
        {   "mace",         5,			TRUE    },
        {   "axe",          6,			TRUE    },
        {   "flail",        7,			TRUE    },
        {   "whip",         8,			TRUE    },
        {   "polearm",      9,			TRUE    },
        {	"staff",		10,			TRUE	},
        {	"hand",			11,			TRUE	},
        {   NULL,           0,          0       }
    };

const struct flag_type weapon_type2[] =
    {
        {   "none",        0,         TRUE   },
        {   "flaming",       WEAPON_FLAMING,       TRUE    },
        {   "frost",         WEAPON_FROST,         TRUE    },
        {   "vampiric",      WEAPON_VAMPIRIC,      TRUE    },
        {   "sharp",         WEAPON_SHARP,         TRUE    },
        {   "vorpal",        WEAPON_VORPAL,        TRUE    },
        {   "twohands",     WEAPON_TWO_HANDS,     TRUE    },
        { "shocking",  WEAPON_SHOCKING,      TRUE    },
        { "poison",   WEAPON_POISON,      TRUE    },
        {   NULL,              0,                    0       }
    };

const struct flag_type res_flags[] =
    {
        { "summon",    RES_SUMMON,        TRUE    },
        {   "charm",         RES_CHARM,            TRUE    },
        {   "magic",         RES_MAGIC,            TRUE    },
        {   "weapon",        RES_WEAPON,           TRUE    },
        {   "bash",          RES_BASH,             TRUE    },
        {   "pierce",        RES_PIERCE,           TRUE    },
        {   "slash",         RES_SLASH,            TRUE    },
        {   "fire",          RES_FIRE,             TRUE    },
        {   "cold",          RES_COLD,             TRUE    },
        {   "lightning",     RES_LIGHTNING,        TRUE    },
        {   "acid",          RES_ACID,             TRUE    },
        {   "poison",        RES_POISON,           TRUE    },
        {   "negative",      RES_NEGATIVE,         TRUE    },
        {   "holy",          RES_HOLY,             TRUE    },
        {   "energy",        RES_ENERGY,           TRUE    },
        {   "mental",        RES_MENTAL,           TRUE    },
        {   "disease",       RES_DISEASE,          TRUE    },
        {   "drowning",      RES_DROWNING,         TRUE    },
        {   "light",         RES_LIGHT,            TRUE    },
        { "sound",    RES_SOUND,      TRUE    },
        { "silver",   RES_SILVER,     TRUE    },
        { "iron",     RES_IRON,       TRUE    },
        {   NULL,          0,            0    }
    };
const struct flag_type vuln_flags[] =
    {
        { "summon",    VULN_SUMMON,       TRUE    },
        { "charm",    VULN_CHARM,     TRUE    },
        {   "magic",         VULN_MAGIC,           TRUE    },
        {   "weapon",        VULN_WEAPON,          TRUE    },
        {   "bash",          VULN_BASH,            TRUE    },
        {   "pierce",        VULN_PIERCE,          TRUE    },
        {   "slash",         VULN_SLASH,           TRUE    },
        {   "fire",          VULN_FIRE,            TRUE    },
        {   "cold",          VULN_COLD,            TRUE    },
        {   "lightning",     VULN_LIGHTNING,       TRUE    },
        {   "acid",          VULN_ACID,            TRUE    },
        {   "poison",        VULN_POISON,          TRUE    },
        {   "negative",      VULN_NEGATIVE,        TRUE    },
        {   "holy",          VULN_HOLY,            TRUE    },
        {   "energy",        VULN_ENERGY,          TRUE    },
        {   "mental",        VULN_MENTAL,          TRUE    },
        {   "disease",       VULN_DISEASE,         TRUE    },
        {   "drowning",      VULN_DROWNING,        TRUE    },
        {   "light",         VULN_LIGHT,           TRUE    },
        { "sound",     VULN_SOUND,        TRUE    },
        {   "silver",        VULN_SILVER,          TRUE    },
        {   "iron",          VULN_IRON,            TRUE    },
        {   NULL,              0,                    0       }
    };


const struct flag_type material_type[] =    /* not yet implemented */
    {
        {   "none",          0,            TRUE    },
        {   NULL,              0,            0       }
    };

const struct flag_type position_flags[] =
    {
        {   "dead",           POS_DEAD,            FALSE   },
        {   "mortal",         POS_MORTAL,          FALSE   },
        {   "incap",          POS_INCAP,           FALSE   },
        {   "stunned",        POS_STUNNED,         FALSE   },
        {   "sleeping",       POS_SLEEPING,        TRUE    },
        {   "resting",        POS_RESTING,         TRUE    },
        {   "sitting",        POS_SITTING,         TRUE    },
        {   "fighting",       POS_FIGHTING,        FALSE   },
        {   "standing",       POS_STANDING,        TRUE    },
        {   NULL,              0,                    0       }
    };

const struct flag_type portal_flags[]=
    {
        {   "normal_exit",      GATE_NORMAL_EXIT, TRUE    },
        { "no_curse",   GATE_NOCURSE,     TRUE    },
        {   "go_with",      GATE_GOWITH,      TRUE    },
        {   "buggy",    GATE_BUGGY,       TRUE    },
        { "random",     GATE_RANDOM,      TRUE    },
        {   NULL,       0,            0   }
    };

const struct flag_type furniture_flags[]=
    {
        { "none",       0,            TRUE    },
        { "stand_at",   STAND_AT,     TRUE    },
        { "stand_on",   STAND_ON,     TRUE    },
        { "stand_in",   STAND_IN,     TRUE    },
        { "sit_at",     SIT_AT,       TRUE    },
        { "sit_on",     SIT_ON,       TRUE    },
        { "sit_in",     SIT_IN,       TRUE    },
        { "rest_at",    REST_AT,      TRUE    },
        { "rest_on",    REST_ON,      TRUE    },
        { "rest_in",    REST_IN,      TRUE    },
        { "sleep_at",   SLEEP_AT,     TRUE    },
        { "sleep_on",   SLEEP_ON,     TRUE    },
        { "sleep_in",   SLEEP_IN,     TRUE    },
        { "put_at",     PUT_AT,       TRUE    },
        { "put_on",     PUT_ON,       TRUE    },
        { "put_in",     PUT_IN,       TRUE    },
        { "put_inside", PUT_INSIDE,   TRUE    },
        { "lounge_on",	LOUNGE_ON,	  TRUE	  },
        { NULL,         0,            0   }
    };

const struct display_type altdesc_condtable[] =
{
	{ "nighttime",	AD_COND_NIGHT,	"shown while sun is down"	},
	{ NULL,		0,		NULL				},
};
