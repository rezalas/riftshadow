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

#include "tables.h"


//Steal the flag table, we're not storing bitvectors, but shhh, we like their functions!
const struct flag_type aftype_table[] =
{
	//	 name			bit				settable
	{	"AFT_SPELL",	AFT_SPELL,		true	},
	{	"AFT_SKILL",	AFT_SKILL,		true	},
	{	"AFT_POWER",	AFT_POWER,		true	},
	{	"AFT_MALADY",	AFT_MALADY,		true	},
	{	"AFT_COMMUNE",	AFT_COMMUNE,	true	},
	{	"AFT_INVIS",	AFT_INVIS,		true	},
	{	"AFT_RUNE",		AFT_RUNE,		true	},
	{	"AFT_TIMER",	AFT_TIMER,		true	},
	{	NULL,			0,				false	}
};

const struct display_type apply_locations[] =
{
	//	name				bit						display
	{	"none",				APPLY_NONE,				"none"					},
	{	"str",				APPLY_STR,				"strength"				},
	{	"dex",				APPLY_DEX,				"dexterity"				},
	{	"int",				APPLY_INT,				"intelligence"			},
	{	"wis",				APPLY_WIS,				"wisdom"				},
	{	"con",				APPLY_CON,				"constitution"			},
	{	"sex",				APPLY_SEX,				"sex"					},
	{	"class",			APPLY_CLASS,			"class"					},
	{	"luck",				APPLY_LUCK,				"luck"					},
	{	"age",				APPLY_AGE,				"age"					},
	{	"height",			APPLY_HEIGHT,			"none"					},
	{	"weight",			APPLY_WEIGHT,			"none"					},
	{	"mana",				APPLY_MANA,				"mana"					},
	{	"hp",				APPLY_HIT,				"health"				},
	{	"move",				APPLY_MOVE,				"moves"					},
	{	"gold",				APPLY_GOLD,				"none"					},
	{	"exp",				APPLY_EXP,				"none"					},
	{	"ac",				APPLY_AC,				"armor class"			},
	{	"hitroll",			APPLY_HITROLL,			"hitroll"				},
	{	"damroll",			APPLY_DAMROLL,			"damroll"				},
	{	"saves",			APPLY_SAVES,			"saves"					},
	{	"spara",			APPLY_SAVING_PARA,		"save vs paralysis"		},
	{	"srod",				APPLY_SAVING_ROD,		"save vs rod"			},
	{	"spetri",			APPLY_SAVING_PETRI,		"save vs petrification"	},
	{	"sbreath",			APPLY_SAVING_BREATH,	"save vs breath"		},
	{	"svspell",			APPLY_SAVING_SPELL,		"save vs spell"			},
	{	"affect",			APPLY_SPELL_AFFECT,		"none"					},
	{	"carry_weight", 	APPLY_CARRY_WEIGHT,		"weight carried"		},
	{	"defense",			APPLY_DEFENSE,			"none"					},
	{	"regen",			APPLY_REGENERATION,		"regeneration"			},
	{	"size",				APPLY_SIZE,				"size"					},
	{	"energystate",		APPLY_ENERGYSTATE,		"energy state"			},
	{	"dammod",			APPLY_DAM_MOD,			"none"					},
	{	"legs",				APPLY_LEGS,				"none"					},
	{	"arms",				APPLY_ARMS,				"none"					},
	{	"beauty",			APPLY_BEAUTY,			"none"					},
	{	"alignment",		APPLY_ALIGNMENT,		"none"					},
	{	"ethos",			APPLY_ETHOS,			"none"					},
	{	NULL,				0,						0						} // NULL, 0, FALSE
};

const struct mod_name_type mod_names[] =
{
	//	name					type
	{	"vision",				MOD_VISION			},
	{	"movement",				MOD_MOVEMENT		},
	{	"toughness",			MOD_TOUGHNESS		},
	{	"speed",				MOD_SPEED			},
	{	"levitation",			MOD_LEVITATION		},
	{	"visibility",			MOD_VISIBILITY		},
	{	"phase",				MOD_PHASE			},
	{	"consciousness",		MOD_CONC			},
	{	"protection",			MOD_PROTECTION		},
	{	"appearance",			MOD_APPEARANCE		},
	{	"hearing",				MOD_HEARING			},
	{	"perception",			MOD_PERCEPTION		},
	{	"resistance",			MOD_RESISTANCE		},
	{	"energy state",			MOD_ENERGY_STATE	},
	{	"speech",				MOD_SPEECH			},
	{	"regeneration",			MOD_REGEN			},
	{	"tasteful wardrobe",	MOD_WARDROBE		}
};

const struct cabal_type cabal_table[MAX_CABAL] =
{
	/* independent should be false if is a real cabal */
	//  name,    	who_name,		extitle,				long_name,									indep,		vnum,		max,	start
	{	"none",		"",				"",						"",											true,			1,		 1, 	1	},
	{	"guild",	"[GUILD] ", 	"",						"Common Guild, Protectors of Shalar",		false,			0,		 1, 	1	},
	{	"scion",	"[SCION] ", 	"",						"Scions of Knowledge",						false,		23652,		30, 	1	},
	{	"horde",	"[HORDE] ",		"",						"Barbarian Horde, Pillagers of Shalar", 	false,		23625,		30, 	1	},
	{	"bounty",	"[BOUNTY] ", 	", Neophyte Hunter",	"Bounty Hunters, Mercenaries of Shalar",	false,		23601,		40, 	1	},
	{	"theatre",	"[THEATRE] ",	"",						"Regal Theatre",							false,		23712,		 1, 	1	},
	{	"phalanx",	"[PHALANX] ",	"",						"Phalanx of Fire",							false,		23877,		30,		1	}
};

const struct cabal_message cabal_messages[MAX_CABAL] =
{
	// login, logout, entry message //
	{
		"",
		"",
		""
	},
	{
		"%s has joined the Hunt.",
		"%s has left from the Hunt.",
		"May the darkness conceal you."
	},
	{
		"Our wisdom grows as %s enters the lands.",
		"The search for knowledge continues, though %s leaves the lands.",
		"Greetings, seeker of knowledge."
	},
	{
		"Welcome %s. You bring us strength.",
		"Farewell %s. May your strength return to us soon.",
		"Welcome, great warrior."
	},
	{
		"%s enters the Bounty Guild Halls.",
		"%s departs the Bounty Guild Halls.",
		"Greetings, Hunter."
	},
	{
		"%s enters the Guild.",
		"%s departs the Guild.",
		"Hail, brother."
	},
	{
		"The Flame of Purity blazes brighter as %s enters the Temple.",
		"The Flame of Purity dims as %s departs the Temple.",
		"Greetings, child of the Light."
	}
};

const struct position_type position_table[] =
{
	//	name					short_name
	{	"dead",					"dead"		},
	{	"mortally wounded",		"mort"		},
	{	"incapacitated",		"incap"		},
	{	"stunned",				"stun"		},
	{	"sleeping",				"sleep"		},
	{	"resting",				"rest"		},
	{	"sitting",				"sit"		},
	{	"fighting",				"fight"		},
	{	"standing",				"stand"		},
	{	NULL,					NULL		}
};

const struct sex_type sex_table[] =
{
	//	name
	{	"asexual"	},
	{	"male"		},
	{	"female"	},
	{	"either"	},
	{	NULL		}
};

const struct size_type size_table[] =
{
	//	name
	{	"tiny"		},
	{	"small" 	},
	{	"medium"	},
	{	"large"		},
	{	"huge", 	},
	{	"giant" 	},
	{	"immense"	},
	{	NULL		}
};

const struct flag_type act_flags[] =
{
	//	name				bit			settable
	{	"npc",				ASCII_A,	false	},
	{	"sentinel",			ASCII_B,	true	},
	{	"scavenger",		ASCII_C,	true	},
	{	"ward_mob",			ASCII_D,	true	},
	{	"wander",			ASCII_E,	true	},
	{	"aggressive",		ASCII_F,	true	},
	{	"stay_area",		ASCII_G,	true	},
	{	"wimpy",			ASCII_H,	true	},
	{	"pet",				ASCII_I,	true	},
	{	"train",			ASCII_J,	true	},
	{	"practice",			ASCII_K,	true	},
	{	"smarttrack",		ASCII_L,	true	},
	{	"undead",			ASCII_O,	true	},
	{	"inner_guardian",	ASCII_P,	true	},
	{	"cleric",			ASCII_Q,	true	},
	{	"mage",				ASCII_R,	true	},
	{	"intelligent",		ASCII_S,	true	},
	{	"fast_track",		ASCII_T,	true	},
	{	"noalign",			ASCII_U,	true	},
	{	"nopurge",			ASCII_V,	true	},
	{	"outdoors",			ASCII_W,	true	},
	{	"indoors",			ASCII_Y,	true	},
	{	"guildguard",		ASCII_Z,	true	},
	{	"healer",			ASCII_aa,	true	},
	{	"gain",				ASCII_bb,	true	},
	{	"update_always",	ASCII_cc,	true	},
	{	"detect_special",	ASCII_dd,	true	},
	{	"banker",			ASCII_ee,	true	},
	{	"nocturnal",		ASCII_ff,	true	},
	{	"diurnal",			ASCII_gg, 	true	},
	{	"fastwander",		ASCII_hh, 	true	},
	{	"law",				ASCII_ii, 	true	},
	{	NULL,				0,			false	}
};

const struct flag_type plr_flags[] =
{
	//	name				bit				settable
	{	"npc",				ASCII_A,		false	},
	{	"autoabort",		ASCII_B,		false	},
	{	"autoassist",		ASCII_C,		false	},
	{	"autoexit",			ASCII_D,		false	},
	{	"autoloot",			ASCII_E,		false	},
	{	"autosac",			ASCII_F,		false	},
	{	"autogold",			ASCII_G,		false	},
	{	"autosplit",		ASCII_H,		false	},
	{	"ignorant",			PLR_IGNORANT,	true	},
	{	"betrayer",			ASCII_K,		true	},
	{	"coder",			PLR_CODER,		true	},
	{	"heroimm",			ASCII_M,		true	},
	{	"holylight",		ASCII_N,		false	},
	{	"empowered",		ASCII_O,		false	},
	{	"no_void",			ASCII_P,		false	},
	{	"nosummon",			ASCII_Q,		false	},
	{	"nofollow",			ASCII_R,		false	},
	{	"no_transfer",		ASCII_T,		true	},
	{	"permit",			ASCII_U,		true	},
	{	"moron",			ASCII_V,		true	},
	{	"log",				ASCII_W,		false	},
	{	"deny",				ASCII_X,		true	},
	{	"freeze",			ASCII_Y,		false	},
	{	"thief",			ASCII_Z,		false	},
	{	"killer",			ASCII_aa,		false	},
	{	"criminal",			ASCII_bb,		false	},
	{	NULL,				0,				false	}
};

const struct flag_type affect_flags[] =
{
	//	name				bit			settable
	{	"blind",			ASCII_A,	true	},
	{	"invisible",		ASCII_B,	true	},
	{	"detect_evil",		ASCII_C,	true	},
	{	"detect_invis",		ASCII_D,	true	},
	{	"detect_magic",		ASCII_E,	true	},
	{	"detect_hidden",	ASCII_F,	true	},
	{	"detect_good",		ASCII_G,	true	},
	{	"sanctuary",		ASCII_H,	true	},
	{	"detect_camo",		ASCII_I,	true	},
	{	"infrared",			ASCII_J,	true	},
	{	"curse",			ASCII_K,	true	},
	{   "camouflage",		ASCII_L,	true	},
	{	"poison",			ASCII_M,	true	},
	{	"protection",		ASCII_N,	true	},
	{	"rage",				ASCII_O,	true	},
	{	"sneak",			ASCII_P,	true	},
	{	"hide",				ASCII_Q,	true	},
	{	"sleep",			ASCII_R,	true	},
	{	"charm",			ASCII_S,	true	},
	{	"flying",			ASCII_T,	true	},
	{	"pass_door",		ASCII_U,	true	},
	{	"haste",			ASCII_V,	true	},
	{	"calm",				ASCII_W,	true	},
	{	"plague",			ASCII_X,	true	},
	{	"permanent",		ASCII_Y,	true	},
	{	"dark_vision",		ASCII_Z,	true	},
	{	"berserk",			ASCII_aa,	true	},
	{	"waterbreath",		ASCII_bb,	true	},
	{	"regeneration",		ASCII_cc,	true	},
	{	"slow",				ASCII_dd,	true	},
	{	"noshow",			ASCII_ee,	true	},
	{	"darksanc",			ASCII_ff, 	true	},
	{	NULL,				0,			false	}
};

const struct flag_type off_flags[] =
{
	//	name				bit			settable
	{	"area_attack",		ASCII_A,	true	},
	{	"backstab",			ASCII_B,	true	},
	{	"bash",				ASCII_C,	true	},
	{	"berserk",			ASCII_D,	true	},
	{	"disarm",			ASCII_E,	true	},
	{	"dodge",			ASCII_F,	true	},
	{	"fade",				ASCII_G,	true	},
	{	"fast",				ASCII_H,	true	},
	{	"kick",				ASCII_I,	true	},
	{	"dirt_kick",		ASCII_J,	true	},
	{	"parry",			ASCII_K,	true	},
	{	"rescue",			ASCII_L,	true	},
	{	"tail",				ASCII_M,	true	},
	{	"trip",				ASCII_N,	true	},
	{	"crush",			ASCII_O,	true	},
	{	"assist_all",		ASCII_P,	true	},
	{	"assist_align",		ASCII_Q,	true	},
	{	"assist_race",		ASCII_R,	true	},
	{	"assist_players",	ASCII_S,	true	},
	{	"assist_guard",		ASCII_T,	true	},
	{	"assist_vnum",		ASCII_U,	true	},
	{	"no_track",			ASCII_V,	true	},
	{	"static_track",		ASCII_W,	true	},
	{	"spam_murder",		ASCII_X,	true	},
	{	"intimidated",		ASCII_Y,	true	},
	{	"unused",			ASCII_Z,	true	},
	{	"assist_group",		ASCII_aa,	true	},
	{	NULL,				0,			0		}
};

const struct flag_type imm_flags[] =
{
	//	name				bit			settable
	{	"summon",			ASCII_A,	true	},
	{	"charm",			ASCII_B,	true	},
	{	"magic",			ASCII_C,	true	},
	{	"weapon",			ASCII_D,	true	},
	{	"bash",				ASCII_E,	true	},
	{	"pierce",			ASCII_F,	true	},
	{	"slash",			ASCII_G,	true	},
	{	"fire",				ASCII_H,	true	},
	{	"cold",				ASCII_I,	true	},
	{	"lightning",		ASCII_J,	true	},
	{	"acid",				ASCII_K,	true	},
	{	"poison",			ASCII_L,	true	},
	{	"negative",			ASCII_M,	true	},
	{	"holy",				ASCII_N,	true	},
	{	"energy",			ASCII_O,	true	},
	{	"mental",			ASCII_P,	true	},
	{	"disease",			ASCII_Q,	true	},
	{	"drowning",			ASCII_R,	true	},
	{	"light",			ASCII_S,	true	},
	{	"sound",			ASCII_T,	true	},
	{	"mithril",			ASCII_X,	true	},
	{	"silver",			ASCII_Y,	true	},
	{	"iron",				ASCII_Z,	true	},
	{   "sleep",			ASCII_aa,	true	},
	{	NULL,				0,			0		}
};

const struct flag_type extra_flags[] =
{
	//	name				bit						settable
	{	"glow",				ITEM_GLOW,				true	},
	{	"hum",				ITEM_HUM,				true	},
	{	"dark",				ITEM_DARK,				true	},
	{	"noshow",			ITEM_NOSHOW,			true	},
	{	"evil",				ITEM_EVIL,				true	},
	{	"invis",			ITEM_INVIS,				true	},
	{	"magic",			ITEM_MAGIC,				true	},
	{	"nodrop",			ITEM_NODROP,			true	},
	{	"bless",			ITEM_BLESS,				true	},
	{	"antigood",			ITEM_ANTI_GOOD,			true	},
	{	"antievil",			ITEM_ANTI_EVIL,			true	},
	{	"antineutral",		ITEM_ANTI_NEUTRAL,		true	},
	{	"noremove",			ITEM_NOREMOVE,			true	},
	{	"inventory",		ITEM_INVENTORY,			true	},
	{	"nopurge",			ITEM_NOPURGE,			true	},
	{	"rotdeath",			ITEM_ROT_DEATH,			true	},
	{	"visdeath",			ITEM_VIS_DEATH,			true	},
	{	"fixed",			ITEM_FIXED,				true	},
	{	"nodisarm",			ITEM_NODISARM,			true	},
	{	"nolocate",			ITEM_NOLOCATE,			true	},
	{	"meltdrop",			ITEM_MELT_DROP,			true	},
	{	"under_clothes",	ITEM_UNDER_CLOTHES,		true	},
	{	"sellextract",		ITEM_SELL_EXTRACT,		true	},
	{	"burnproof",		ITEM_BURN_PROOF,		true	},
	{	"nouncurse",		ITEM_NOUNCURSE,			true	},
	{	"brand",			ITEM_BRAND,				true	},
	{	"noanimate",		CORPSE_NO_ANIMATE,		true	},
	{	"antilawful",		ITEM_ANTI_LAWFUL,		true	},
	{	"antineutral",		ITEM_ANTI_NEUT,			true	},
	{	"antichaotic",		ITEM_ANTI_CHAOTIC,		true	},
	{	"nostash",			ITEM_NO_STASH,			true	},
	{	"nosac",			ITEM_NO_SAC,			true	},
	{	"donation",			ITEM_DONATION_PIT,		true	},
	{	NULL,				0,						0		}
};

const struct flag_type wear_locations[] =
{
	//	name			bit					settable
	{	"light",		WEAR_LIGHT,			true	},
	{	"lfinger",		WEAR_FINGER_L,		true	},
	{	"rfinger",		WEAR_FINGER_R,		true	},
	{	"neckone",		WEAR_NECK_1,		true	},
	{	"necktwo",		WEAR_NECK_2,		true	},
	{	"body",			WEAR_BODY,			true	},
	{	"head",			WEAR_HEAD,			true	},
	{	"legs",			WEAR_LEGS,			true	},
	{	"feet",			WEAR_FEET,			true	},
	{	"hands",		WEAR_HANDS,			true	},
	{	"arms",			WEAR_ARMS,			true	},
	{	"shield",		WEAR_SHIELD,		true	},
	{	"about",		WEAR_ABOUT,			true	},
	{	"waist",		WEAR_WAIST,			true	},
	{	"wristone",		WEAR_WRIST_R,		true	},
	{	"wristtwo",		WEAR_WRIST_L,		true	},
	{	"wield",		WEAR_WIELD,			true	},
	{	"dualwield",	WEAR_DUAL_WIELD,	true	},
	{	"tattooed",		WEAR_BRAND,			true	},
	{	"hold",			WEAR_HOLD,			true	},
};

const struct flag_type wear_flags[] =
{
	//	name			bit			settable
	{	"take",			ASCII_A,	true	},
	{	"finger",		ASCII_B,	true	},
	{	"neck",			ASCII_C,	true	},
	{	"body",			ASCII_D,	true	},
	{	"head",			ASCII_E,	true	},
	{	"legs",			ASCII_F,	true	},
	{	"feet",			ASCII_G,	true	},
	{	"hands",		ASCII_H,	true	},
	{	"arms",			ASCII_I,	true	},
	{	"shield",		ASCII_J,	true	},
	{	"about",		ASCII_K,	true	},
	{	"waist",		ASCII_L,	true	},
	{	"wrist",		ASCII_M,	true	},
	{	"wield",		ASCII_N,	true	},
	{	"hold",			ASCII_O,	true	},
	{	"float",		ASCII_P,	true	},
	{	"brand",		ASCII_Q,	true	},
	{	"strapped",		ASCII_R,	true	},
	{	"cosmetic",		ASCII_S,	true	},
	{	NULL,			0,			0		}
};


const struct flag_type form_flags[] =
{
	//	name			bit					settable
	{	"edible",		FORM_EDIBLE,		true	},
	{	"poison",		FORM_POISON,		true	},
	{	"magical",		FORM_MAGICAL,		true	},
	{	"instant_decay",FORM_INSTANT_DECAY,	true	},
	{	"other",		FORM_OTHER,			true	},
	{	"animal",		FORM_ANIMAL,		true	},
	{	"sentient",		FORM_SENTIENT,		true	},
	{	"undead",		FORM_UNDEAD,		true	},
	{	"construct",	FORM_CONSTRUCT,		true	},
	{	"mist",			FORM_MIST,			true	},
	{	"intangible",	FORM_INTANGIBLE,	true	},
	{	"biped",		FORM_BIPED,			true	},
	{	"aquatic",		FORM_AQUATIC,		true	},
	{	"insect",		FORM_INSECT,		true	},
	{	"spider",		FORM_SPIDER,		true	},
	{	"crustacean",	FORM_CRUSTACEAN,	true	},
	{	"worm",			FORM_WORM,			true	},
	{	"blob",			FORM_BLOB,			true	},
	{	"mammal",		FORM_MAMMAL,		true	},
	{	"bird",			FORM_BIRD,			true	},
	{	"reptile",		FORM_REPTILE,		true	},
	{	"snake",		FORM_SNAKE,			true	},
	{	"dragon",		FORM_DRAGON,		true	},
	{	"amphibian",	FORM_AMPHIBIAN,		true	},
	{	"fish",			FORM_FISH ,			true	},
	{	"cold_blood",	FORM_COLD_BLOOD,	true	},
	{   "nospeech",		FORM_NOSPEECH,		true	},
	{	NULL,			0,					0		}
};

const struct flag_type part_flags[] =
{
	//	name			bit					settable
	{	"head",			PART_HEAD,			true	},
	{	"arms",			PART_ARMS,			true	},
	{	"legs",			PART_LEGS,			true	},
	{	"heart",		PART_HEART,			true	},
	{	"brains",		PART_BRAINS,		true	},
	{	"guts",			PART_GUTS,			true	},
	{	"hands",		PART_HANDS,			true	},
	{	"feet",			PART_FEET,			true	},
	{	"fingers",		PART_FINGERS,		true	},
	{	"ear",			PART_EAR,			true	},
	{	"eye",			PART_EYE,			true	},
	{	"long_tongue",	PART_LONG_TONGUE,	true	},
	{	"eyestalks",	PART_EYESTALKS,		true	},
	{	"tentacles",	PART_TENTACLES,		true	},
	{	"fins",			PART_FINS,			true	},
	{	"wings",		PART_WINGS,			true	},
	{	"tail",			PART_TAIL,			true	},
	{	"claws",		PART_CLAWS,			true	},
	{	"fangs",		PART_FANGS,			true	},
	{	"horns",		PART_HORNS,			true	},
	{	"scales",		PART_SCALES,		true	},
	{	"tusks",		PART_TUSKS,			true	},
	{	NULL,			0,					0		}
};

const struct flag_type comm_flags[] =
{
	//	name			bit					settable
	{	"quiet",		COMM_QUIET,			true	},
	{	"deaf",			COMM_DEAF,			true	},
	{	"nowiz",		COMM_NOWIZ,			true	},
	{	"noauction",	 COMM_NOAUCTION,	true	},
	{	"nogossip",		COMM_NOGOSSIP,		true	},
	{	"noquestion",	COMM_NOQUESTION,	true	},
	{	"nonewbie",		COMM_NONEWBIE,		true	},
	{	"nocabal",		COMM_NOCABAL,		true	},
	{	"noquote",		COMM_NOQUOTE,		true	},
	{	"shoutsoff",	COMM_SHOUTSOFF,		true	},
	{	"compact",		COMM_COMPACT,		true	},
	{	"brief",		COMM_BRIEF,			true	},
	{	"prompt",		COMM_PROMPT,		true	},
	{	"combine",		COMM_COMBINE,		true	},
	{	"telnet_ga",	COMM_TELNET_GA,		true	},
	{	"show_affects",	COMM_SHOW_AFFECTS,	true	},
	{	"noemote",		COMM_NOEMOTE,		true	},
	{	"noshout",		COMM_NOSHOUT,		true	},
	{	"notell",		COMM_NOTELL,		false	},
	{	"nochannels",	COMM_NOCHANNELS,	false	},
	{	"snoop_proof",	COMM_SNOOP_PROOF,	false	},
	{	"afk",			COMM_AFK,			true	},
	{	"nosocket",		COMM_NOSOCKET,		false	},
	{	"switchskills",	COMM_SWITCHSKILLS,	true	},
	{	"lots_o_color",	COMM_LOTS_O_COLOR,	true	},
	{	NULL,			0,					0		}
};

const struct flag_type criterion_flags[] =
{
	//	name			bit				settable
	{	"class",		BAR_CLASS,		true	},
	{	"cabal",		BAR_CABAL,		true	},
	{	"size",			BAR_SIZE,		true	},
	{	"tattoo",		BAR_TATTOO,		true	},
	{	"level",		BAR_LEVEL,		true	},
	{	NULL,			0,				true	}
};

const struct flag_type trap_table[] =
{
	//	name				bit				settable
	{	"none",				TRAP_NONE,		true	},
	{	"dart",				TRAP_DART,		true	},
	{	"poison dart",		TRAP_PDART,		true	},
	{	"fireball",			TRAP_FIREBALL,	true	},
	{	"lightning",		TRAP_LIGHTNING,	true	},
	{	"sleep gas",		TRAP_SLEEPGAS,	true	},
	{	"poison gas",		TRAP_POISONGAS,	true	},
	{	"acid",				TRAP_ACID,		true	},
	{	"concealed pit",	TRAP_PIT,		true	},
	{	"boulder",			TRAP_BOULDER,	true	},
	{	"mana drain",		TRAP_DRAIN,		true	},
	{	NULL,				0,				true	}
};

const struct flag_type speech_table[] =
{
	//	name			bit				settable
	{	"say",			SPEECH_SAY,		true	},
	{	"sayto",		SPEECH_SAYTO,	true	},
	{	"tell",			SPEECH_TELL,	true	},
	{	"whisper",		SPEECH_WHISPER,	true	},
	{	"yell",			SPEECH_YELL,	true	},
	{	"emote",		SPEECH_EMOTE,	true	},
	{	"echo",			SPEECH_ECHO,	true	},
	{	"sing",			SPEECH_SING,	true	},
	{	NULL,			0,				true	}
};

const struct bounty_type bounty_table[MAX_BOUNTY] =
{
	//	rank name,		extitle,					credits required
	{	"neophyte",		"Neophyte Hunter",			   0	},
	{	"hunter", 		"Bounty Hunter", 			 100	},
	{	"veteran",		"Veteran Hunter",			 300	},
	{	"elder",		"Elder Hunter",				 500	},
	{	"master",		"Master Hunter",			1000	}
};

const struct hometown_type hometown_table[MAX_HOMETOWN] =
{
	//	City Name,		Recall,	Pit,	Alignment,		Ethos,			Race/Class Restrict
	{	"",				   0,	   0,	ALIGN_NONE,		ETHOS_NONE,		0	},
	{	"Cimar",		2538,	2796,	ALIGN_ANY,		ETHOS_ANY,		0	},
	{	"Melcene",		1084,	1099,   ALIGN_ANY,		ETHOS_ANY,		0	},
	{	"Iseldheim",	3874,	3875,	ALIGN_ANY,		ETHOS_ANY,		0	},
	{	"Asgard",		1213,	1200,	ALIGN_NONE,		ETHOS_NONE,		0	}
};

const struct color_type color_table[MAX_COLORS] =
{
	// 	Color name,		Associated Ascii Value
	{	"uncolored",	"\x01B[0m"		},
	{	"red",			"\x01B[0;31m"	},
	{	"green",		"\x01B[0;32m"	},
	{	"yellow",		"\x01B[0;33m"	},
	{	"blue",			"\x01B[0;34m"	},
	{	"magenta",		"\x01B[0;35m"	},
	{	"cyan",			"\x01B[0;36m"	},
	{	"lightgrey",	"\x01B[0;37m"	},
	{	"darkgrey",		"\x01B[1;30m"	},
	{	"lightred",		"\x01B[1;31m"	},
	{	"lightgreen",	"\x01B[1;32m"	},
	{	"lightyellow",	"\x01B[1;33m"	},
	{	"lightblue",	"\x01B[1;34m"	},
	{	"lightmagenta",	"\x01B[1;35m"	},
	{	"lightcyan",	"\x01B[1;36m"	},
	{	"white",		"\x01B[1;37m"	}
};

const struct color_event_type color_event[MAX_EVENTS] =
{
	//	Event Name,		Default Color,	Minimum Level
	{	"prompt",		"uncolored",	 1	},
	{	"channels",		"darkgrey",		 1	},
	{	"tells",		"lightgreen",	 1	},
	{	"grouptells",	"magenta",		 1	},
	{	"speech",		"lightyellow",	 1	},
	{	"damage",		"lightred",		 1	},
	{	"yells",		"lightcyan",	 1	},
	{	"song",			"blue",			 1	},
	{	"immtalk",		"cyan",			52	},
	{	"wiznet",		"uncolored",	52	},
	{	"prays",		"lightred",		53	},
	{	"builder",		"lightgrey",	51	},
	{	"newbie",		"lightblue",	 1	}
};

const struct sect_type sect_table[] =
{
	//	keyword,			number				moves	wait
	{	"unused",			0,					1,		1 },
	{	"city",				SECT_CITY,			1,		1 },
	{	"field",			SECT_FIELD,			2,		1 },
	{	"forest",			SECT_FOREST,		3,		2 },
	{	"hills",			SECT_HILLS,			5,		3 },
	{	"mountains",		SECT_MOUNTAIN,		6,		4 },
	{	"water",			SECT_WATER,			3,		3 },
	{	"inside",   		SECT_INSIDE,		2,		2 },
	{	"underwater",		SECT_UNDERWATER,	4,		2 },
	{	"air",				SECT_AIR,			3,		2 },
	{	"desert",			SECT_DESERT,		4,		2 },
	{	"road",				SECT_ROAD,			1,		1 },
	{	"burning",			SECT_BURNING,		5,		2 },
	{	"conflagration",	SECT_CONFLAGRATION,	5,		2 },
	{	"trail",			SECT_TRAIL, 		2,		1 },
	{	"swamp",			SECT_SWAMP,			4,		3 },
	{	"park",				SECT_PARK,			2,		1 },
	{	"vertical",			SECT_VERTICAL,		8,		4 },
	{	"ice",				SECT_ICE,			2,		3 },
	{	"snow",				SECT_SNOW,			6,		2 },
	{	"cave",				SECT_CAVE,			2,		2 },
	{	NULL,				0,					0,		0 }
};

const struct flag_type room_flags[] =
{
	//	name				bit						settable
	{	"dark",				ROOM_DARK,				true	},
	{	"no_mob",			ROOM_NO_MOB,			true	},
	{	"indoors",			ROOM_INDOORS,			true	},
	{	"private",			ROOM_PRIVATE,			true	},
	{	"safe",				ROOM_SAFE,				true	},
	{	"solitary",			ROOM_SOLITARY,			true	},
	{	"pet_shop",			ROOM_PET_SHOP,			true	},
	{	"norecall",			ROOM_NO_RECALL,			true	},
	{	"imp_only",			ROOM_IMP_ONLY,			true	},
	{	"god_only",			ROOM_GODS_ONLY,			true	},
	{	"hero_only",		ROOM_HEROES_ONLY,		true	},
	{	"newbie_only",		ROOM_NEWBIES_ONLY,		true	},
	{	"law",				ROOM_LAW,				true	},
	{	"nowhere",			ROOM_NOWHERE,			true	},
	{	"no_gate",			ROOM_NO_GATE,			true	},
	{	"silence",			ROOM_SILENCE,			true	},
	{	"no_summon_to",		ROOM_NO_SUMMON_TO,		true	},
	{	"no_summon_from",	ROOM_NO_SUMMON_FROM,	true	},
	{	"no_alarm",			ROOM_NO_ALARM,			true	},
	{	"force_duel",		ROOM_FORCE_DUEL,		true	},
	{	"no_magic",			ROOM_NO_MAGIC,			true	},
	{	"explore",			ROOM_AREA_EXPLORE,		true	},
	{	"no_commune",		ROOM_NO_COMMUNE,		true	},
	{  NULL,				0,						true	}
};

const struct flag_type direction_table [] =
{
	//	name		bit				settable
	{	"north",	DIR_NORTH,		true	},
	{	"east",		DIR_EAST,		true	},
	{	"south",	DIR_SOUTH, 		true	},
	{	"west",		DIR_WEST,		true	},
	{	"up",		DIR_UP,			true	},
	{	"down",		DIR_DOWN,		true	},
	{	NULL,		0,				true	}
};

const struct flag_type exit_flags [] =
{
	//	name			bit				settable
	{	"isdoor",		EX_ISDOOR,		true	},
	{	"closed",		EX_CLOSED,		true	},
	{	"locked",		EX_LOCKED,		true	},
	{	"nopick",		EX_PICKPROOF,	true	},
	{	"nopass",		EX_NOPASS,		true	},
	{	"noclose",		EX_NOCLOSE,		true	},
	{	"nolock",		EX_NOLOCK,		true	},
	{	"nobash",		EX_NOBASH,		true	},
	{	"nonobvious",	EX_NONOBVIOUS,	true	},
	{	"translucent",	EX_TRANSLUCENT,	true	},
	{	"jammed",		EX_JAMMED,		true	},
	{	NULL,			0,				true	}
};

const struct flag_type area_type_table[] =
{
	//	name			bit				settable
	{	"normal",		ARE_NORMAL,		true	},
	{	"road_river",	ARE_ROAD_RIVER, true	},
	{	"cabal",		ARE_CABAL,		true	},
	{	"quest",		ARE_QUEST,		true	},
	{	"city",			ARE_CITY,		true	},
	{	"unopened",		ARE_UNOPENED,	true	},
	{	"shrine",		ARE_SHRINE,		true	},
	{	NULL,			0,				true	}
};

const struct flag_type sky_table[MAX_SKY] =
{
	//	name			bit				settable
	{	"clear",		SKY_CLEAR,		true	},
	{	"partlycloudy",	SKY_PCLOUDY,	true	},
	{	"overcast",		SKY_OVERCAST,	true	},
	{	"drizzle",		SKY_DRIZZLE,	true	},
	{	"downpour",		SKY_DOWNPOUR,	true	},
	{	"thunderstorm",	SKY_TSTORM,		true	},
	{	"flurry",		SKY_FLURRY,		true	},
	{	"blizzard",		SKY_BLIZZARD,	true	},
	{	"hail",			SKY_HAIL,		true	}
};

const struct flag_type temp_table[MAX_TEMP] =
{
	//	name		bit				settable
	{	"hot",		TEMP_HOT,		true	},
	{	"warm",		TEMP_WARM,		true	},
	{	"cool",		TEMP_COOL,		true	},
	{	"cold",		TEMP_COLD,		true	}
};

const struct flag_type wind_table[MAX_WIND] =
{
	//	name		bit				settable
	{	"calm",		WIND_CALM,		true	},
	{	"breeze",	WIND_BREEZE,	true	},
	{	"windy",	WIND_WINDY,		true	},
	{	"gale",		WIND_GALE,		true	}
};

const struct restrict_type restrict_table[] =
{
	//	name, 					bit, 			type,				value
	{	"human_only",			ASCII_A,		RESTRICT_RACE,		"human"			},
	{	"dwarf_only",			ASCII_B,		RESTRICT_RACE,		"dwarf"			},
	{	"elf_only",				ASCII_C,		RESTRICT_RACE,		"elf"			},
	{	"drow_only",			ASCII_D,		RESTRICT_RACE,		"drow"			},
	{	"duergar_only",			ASCII_E,		RESTRICT_RACE,		"duergar"		},
	{	"imp_only",				ASCII_F,		RESTRICT_RACE,		"imp"			},
	{	"halfling_only",		ASCII_G,		RESTRICT_RACE,		"halfling"		},
	{	"titan_only",			ASCII_H,		RESTRICT_RACE,		"titan"			},
	{	"minotaur_only",		ASCII_I,		RESTRICT_RACE,		"minotaur"		},
	{	"sidhe_only",			ASCII_J,		RESTRICT_RACE,		"sidhe"			},
	{	"warrior_only",			ASCII_K,		RESTRICT_CLASS,		"warrior"		},
	{	"thief_only",			ASCII_L,		RESTRICT_CLASS,		"thief"			},
	{	"zealot_only",			ASCII_M,		RESTRICT_CLASS,		"zealot"		},
	{	"paladin_only",			ASCII_N,		RESTRICT_CLASS,		"paladin"		},
	{	"anti_paladin_only",	ASCII_O,		RESTRICT_CLASS,		"anti-paladin"	},
	{	"ranger_only",			ASCII_P,		RESTRICT_CLASS,		"ranger"		},
	{	"monk_only",			ASCII_Q,		RESTRICT_CLASS,		"monk"			},
	{	"shapeshifter_only",	ASCII_R,		RESTRICT_CLASS,		"shapeshifter"	},
	{	"assassin_only",		ASCII_S,		RESTRICT_CLASS,		"assassin"		},
	{	"necromancer_only",		ASCII_T,		RESTRICT_CLASS,		"necromancer"	},
	{	"sorcerer_only",		ASCII_U,		RESTRICT_CLASS,		"sorcerer"		},
	{	"scion_only",			ASCII_W,		RESTRICT_CABAL,		"scion"			},
	{	"horde_only",			ASCII_X,		RESTRICT_CABAL,		"horde"			},
	{	"theatre_only",			ASCII_Y,		RESTRICT_CABAL,		"theatre"		},
	{	"bounty_only",			ASCII_Z,		RESTRICT_CABAL,		"bounty"		},
	{	"mage_only",			ASCII_aa,		RESTRICT_OTHER,		""				},
	{	"newbie_only",			ASCII_bb,		RESTRICT_OTHER,		""				},
	{	"female_only",			ASCII_cc,		RESTRICT_OTHER,		""				},
	{	"male_only",			ASCII_dd,		RESTRICT_OTHER,		""				},
	{	NULL,					0,				0,					""				}
};

const struct tribe_type tribe_table[] =
{
	//	name,		long name,					tribe number
	{	"none",		"none",						TRIBE_NONE		},
	{	"boar",		"Tribe of the Boar",		TRIBE_BOAR,		},
	{	"wolf",		"Tribe of the Wolf",		TRIBE_WOLF,		},
	{	"bear",		"Tribe of the Bear",		TRIBE_BEAR,		},
	{	"hawk",		"Tribe of the Hawk",		TRIBE_HAWK,		},
	{	"lion",		"Tribe of the Lion",		TRIBE_LION,		},
	{	"elk",		"Tribe of the Elk",			TRIBE_ELK,		},
	{	"jackal",	"Tribe of the Jackal",		TRIBE_JACKAL,	},
	{	"fox",		"Tribe of the Fox",			TRIBE_FOX,		},
	{	"bull",		"Tribe of the Bull",		TRIBE_BULL,		},
	{	"panther",	"Tribe of the Panther",		TRIBE_PANTHER,	},
	{	NULL,		NULL,						TRIBE_NONE,		},
};

const char* day_name[] =
{
	"of Toil",
	"of Grain",
	"of Iron",
	"of Hunting",
	"of Feasting",
	"of Rest"
};

const char* month_name[] =
{
	"of the Glacier",
	"of the Arrival",
	"of the Torrent",
	"of the Mountain Stand",
	"of the Quickening",
	"of the Great Siege",
	"of the Molten Stone",
	"of the Holy Descent",
	"of the Ashen Earth",
	"of the Crumbling Wall",
	"of the Whirling Wind",
	"of the Triumph"
};

const char* season_name[] =
{
	"of Darkness",
	"of Birth",
	"of Light",
	"of Death"
};

const struct para_sphere para_list[] =
{
	//	first,		second,			para
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
	{	"earth",	"water",		"ooze"		}
};

const struct ele_sphere sphere_table[] =
{
	//	element,			type,				name
	{	ELE_HEAT,			ELE_TYPE_PRIMARY,	"Heat"			},
	{	ELE_COLD,			ELE_TYPE_PRIMARY,	"Cold"			},
	{	ELE_AIR,			ELE_TYPE_PRIMARY,	"Air"			},
	{	ELE_EARTH,			ELE_TYPE_PRIMARY,	"Earth"			},
	{	ELE_WATER,			ELE_TYPE_PRIMARY,	"Water"			},
	{	ELE_ELECTRICITY,	ELE_TYPE_PRIMARY,	"Electricity"	},
	{	ELE_SMOKE,			ELE_TYPE_PARA,		"Smoke"			},
	{	ELE_MAGMA,			ELE_TYPE_PARA,		"Magma"			},
	{	ELE_PLASMA,			ELE_TYPE_PARA,		"Plasma"		},
	{	ELE_ACID,			ELE_TYPE_PARA,		"Acid"			},
	{	ELE_BLIZZARD,		ELE_TYPE_PARA,		"Blizzard"		},
	{	ELE_FROST,			ELE_TYPE_PARA,		"Frost"			},
	{	ELE_CRYSTAL,		ELE_TYPE_PARA,		"Crystal"		},
	{	ELE_ICE,			ELE_TYPE_PARA,		"Ice"			},
	{	ELE_LIGHTNING,		ELE_TYPE_PARA,		"Lightning"		},
	{	ELE_MIST,			ELE_TYPE_PARA,		"Mist"			},
	{	ELE_METAL,			ELE_TYPE_PARA,		"Metal"			},
	{	ELE_OOZE,			ELE_TYPE_PARA,		"Ooze"			},
	{	ELE_NONE,			ELE_TYPE_NONE,		"none"			}
};

const struct style_type style_table[] =
{
	//	style,				name
	{	STYLE_NONE,			"none"			},
	{	STYLE_GLADIATOR,	"Gladiator"		},
	{	STYLE_BARBARIAN,	"Barbarian"		},
	{	STYLE_DUELIST,		"Duelist"		},
	{	STYLE_SKIRMISHER,	"Skirmisher"	},
	{	STYLE_DRAGOON,		"Dragoon"		},
	{	STYLE_TACTICIAN,	"Tactician"		}
};

const struct style_list style_percent[] =
{
	//	name,					percent
	{	"none",					 0		},
	{	"fend",					 5		},
	{	"entrap",				15		},
	{	"gouge",				30		},
	{	"bleed",				50		},
	{	"hobble",				60		},
	{	"crippling blow",		80		},
	{	"brutality",			 5		},
	{	"shatter",				15		},
	{	"overhead",				30		},
	{	"impale",				45		},
	{	"uppercut",				60		},
	{	"batter",				80		},
	{	"maneuvering",			 5		},
	{	"catch",				15		},
	{	"unbalance",			30		},
	{	"posture",				45		},
	{	"sidestep",				55		},
	{	"concuss",				65		},
	{	"disrupt formation",	80		},
	{	"evasion",				 5		},
	{	"hurl",					15		},
	{	"dart",					35		},
	{	"entwine",				50		},
	{	"pull",					55		},
	{	"fifth attack",			65		},
	{	"whirlwind",			80		},
	{	"ease",					 5		},
	{	"deflect",				15		},
	{	"exchange",				30		},
	{	"charge",				40		},
	{	"shieldbash",			55		},
	{	"brace",				75		},
	{	"leadership",			 5		},
	{	"assess",				20		},
	{	"exploit",				35		},
	{	"outflank",				50		},
	{	"terrain mastery",		60		},
	{	"analyze",				85		}
};

const std::vector<cabal_list> cabal_skills =
{
	//	cable			skill,					level,	specific
	{	CABAL_SCION,	"mana transfer",		 1,		false	},
	{	CABAL_SCION, 	"scribe",				 1,		false	},
	{	CABAL_SCION,	"deny magic",			 1,		false	},
	{	CABAL_SCION,	"nullify",				 1,		false	},
	{	CABAL_SCION,	"bane of ignorance",	 1,		false	},
	{	CABAL_HORDE,	"howl",					 1,		false	},
	{	CABAL_HORDE,	"epic strength",		 1,		false	},
	{	CABAL_HORDE,	"trophy",				 1,		false	},
	{	CABAL_HORDE,	"rage",					 1,		false	},
	{	CABAL_HORDE,	"calm",					 1,		false	},
	{	CABAL_HORDE,	"communion",			10,		false	},
	{	CABAL_BOUNTY,	"hunters strength",		 1,		false	},
	{	CABAL_BOUNTY,	"hunters awareness",	 1,		false	},
	{	CABAL_BOUNTY,	"hunters stealth",		 4,		false	},
	{	CABAL_BOUNTY,	"isolation",			 5,		false	},
	{	CABAL_THEATRE,	"story tell",			 1,		false	},
	{	CABAL_GUILD,	"medicine",				 1,		false	},
	{	CABAL_GUILD,	"repose",				 1,		false	},
	{	CABAL_PHALANX,	"fervor",				 1,		false	},
	{	CABAL_PHALANX,	"piety",				 1,		false	},
	{	CABAL_PHALANX,	"spiritual healing",	 2,		true	},
	{	CABAL_PHALANX,	"shroud of light",		 2,		true	},
	{	CABAL_PHALANX,	"safehaven",			 2,		true	},
	{	CABAL_PHALANX,	"crimson martyr",		 3,		true	},
	{	CABAL_PHALANX,	"retribution",			 3,		true	},
	{	CABAL_PHALANX,	"infidels fate",		 3,		true	},
	{	CABAL_NONE,		NULL,					 0,		false	}
};

const struct flag_type wealth_table[] =
{
	//	name			bit			settable
	{	"none",			    0,		false	},
	{	"poor",			    1,		false	},
	{	"moderate",		   10,		false	},
	{	"affluent",		  100,		false	},
	{	"rich",			 1000,		false	},
	{	"peerless",		10000,		false	},
	{	NULL,			    0,		false	},
};


const struct demon_type demon_table[] =
{
	//	name,			number,				type
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
{	
	{	
		"none",											// Climate name
		CLIMATE_NONE,									// Climate number
		{												// Sky frequencies by season
			{	0,	0,	0,	0,	0,	0,	0,	0,	0	},	//		Winter
			{	0,	0,	0,	0,	0,	0,	0,	0,	0	},	//		Spring
			{	0,	0,	0,	0,	0,	0,	0,	0,	0	},	//		Summer
			{	0,	0,	0,	0,	0,	0,	0,	0,	0	}	//		Autumn
		},
		{												// Temperature
			{	0,	0,	0,	0	},						//		Winter
			{	0,	0,	0,	0	},						//		Spring
			{	0,	0,	0,	0	},						//		Summer
			{	0,	0,	0,	0	}						//		Autumn
		}
	},
	{
		"temperate",
		CLIMATE_TEMPERATE,
		{
			{	20,		40,		60,		60,		60,		 65,	 90,	 95,	100		},
			{	20,		40,		60,		70,		80,		 90,	 97,	 98,	100		},
			{	20,		40,		60,		70,		90,		100,	100,	100,	100		},
			{	20,		40,		60,		70,		80,		 90,	 97,	 98,	100		}
		},
		{
			{	 0,		  0,	 40,	100		},
			{	25,		 75,	100,	100		},
			{	60,		100,	100,	100		},
			{	25,		 75,	100,	100		}
		}
	},
	{
		"desert",
		CLIMATE_DESERT,
		{
			{	70,		 80,	 80,	 80,	 95,	100,	100,	100,	100		},
			{	80,		 90,	 90,	 90,	 98,	100,	100,	100,	100		},
			{	90,		100,	100,	100,	100,	100,	100,	100,	100		},
			{	80,		 90,	 90,	 90,	 98,	100,	100,	100,	100		}
		},
		{
			{	50,		 50,	100,	100		},
			{	70,		 80,	100,	100		},
			{	90,		100,	100,	100		},
			{	70,		 80,	100,	100		}
		}
	},
	{
		"tropical",
		CLIMATE_TROPICAL,
		{
			{	5,		10,		30,		50,		80,		100,	100,	100,	100		},
			{	5,		10,		30,		50,		80,		100,	100,	100,	100		},
			{	5,		10,		30,		50,		80,		100,	100,	100,	100		},
			{	5,		10,		30,		50,		80,		100,	100,	100,	100		}
		},
		{
			{	50,		100,	100,	100		},
			{	80,		100,	100,	100		},
			{  100,		100,	100,	100		},
			{	80,		100,	100,	100		}
		}
	},
	{
		"alpine",
		CLIMATE_ALPINE,
		{
			{	20,		40,		60,		60,		60,		60,		70,		95,		100		},
			{	20,		40,		60,		60,		60,		60,		80,		90,		100		},
			{	20,		40,		60,		70,		75,		85,		95,		97,		100		},
			{	20,		40,		60,		60,		60,		60,		80,		90,		100		}
		},
		{
			{	0,		 0,		 30,	100		},
			{	0,		15,		 60,	100		},
			{	0,		60,		100,	100		},
			{	0,		15,		 60,	100		}
		}
	},
	{
		"tundra",
		CLIMATE_TUNDRA,
		{
			{	30,		40,		50,		50,		50,		50,		70,		95,		100		},
			{	30,		40,		50,		50,		50,		50,		80,		90,		100		},
			{	30,		40,		50,		55,		55,		60,		90,		93,		100		},
			{	30,		40,		50,		50,		50,		50,		80,		90,		100		}
		},
		{
			{	0,		 0,		 0,		100		},
			{	0,		 0,		10,		100		},
			{	0,		10,		50,		100		},
			{	0,		 0,		10,		100		}
		}
	},
	{
		"subtropical",
		CLIMATE_SUBTROPICAL,
		{
			{	0,	0,	0,	0,	0,	0,	0,	0,	0	},
			{	0,	0,	0,	0,	0,	0,	0,	0,	0	},
			{	0,	0,	0,	0,	0,	0,	0,	0,	0	},
			{	0,	0,	0,	0,	0,	0,	0,	0,	0	}
		},
		{
			{	0,	0,	0,	0	},
			{	0,	0,	0,	0	},
			{	0,	0,	0,	0	},
			{	0,	0,	0,	0	}
		}
	},
	{
		"arid",
		CLIMATE_ARID,
		{
			{	30,		40,		 50,	 60,	 80,	 96,	 98,	 98,	100		},
			{	50,		75,		 85,	 90,	 95,	100,	100,	100,	100		},
			{	70,		95,		100,	100,	100,	100,	100,	100,	100		},
			{	50,		75,		 85,	 90,	 95,	100,	100,	100,	100	}
		},
		{
			{	0,		  0,	 75,	100		},
			{	0,		 50,	100,	100		},
			{  80,		100,	100,	100		},
			{	0,		 50,	100,	100		}
		}
	},
	{
		"english",
		CLIMATE_ENGLISH,
		{
			{	0,	0,	0,	0,	0,	0,	0,	0,	0	},
			{	0,	0,	0,	0,	0,	0,	0,	0,	0	},
			{	0,	0,	0,	0,	0,	0,	0,	0,	0	},
			{	0,	0,	0,	0,	0,	0,	0,	0,	0	}
		},
		{
			{	0,	0,	0,	0	},
			{	0,	0,	0,	0	},
			{	0,	0,	0,	0	},
			{	0,	0,	0,	0	}
		}
	}
};

const struct beauty_type beauty_table[] =
{
	//	male,						female
	{	"", 						""							},
	{	"unspeakably grotesque",	"unspeakably grotesque" 	},
	{	"disfigured",				"disfigured"				},
	{	"ugly",						"ugly"						},
	{	"unattractive",				"unattractive"				},
	{	"plain",		 			"homely"					},
	{	"pleasant-looking",			"pleasant-looking"			},
	{	"handsome",					"pretty"					},
	{	"striking",					"beautiful",				},
	{	"stunning",					"stunning",					},
	{	"dazzlingly handsome",		"dazzlingly beautiful"		}
};

const struct order_list order_table[] =
{
	//command
	{	"hit" 		},
	{	"murder" 	},
	{	"get" 		},
	{	"drop"		},
	{	"put" 		},
	{	"give" 		},
	{	"kill" 		},
	{	"rescue" 	},
	{	"disengage" },
	{	"say"		},
	{	"emote" 	},
	{	"pmote" 	}
};

const struct flag_type area_flags[] =
{
	//	name,			bit,				settable
	{	"none",			AREA_NONE,			false	},
	{	"explore",		AREA_EXPLORE,		true	},
	{	"no_newbies",	AREA_NO_NEWBIES,	true	},
	{	"unghost",		AREA_UNGHOST,		true	},
	{	"changed",		AREA_CHANGED,		true	},
	{	"added",		AREA_ADDED,			true	},
	{	"loading",		AREA_LOADING,		false	},
	{	NULL,			0,					0		}
};

const struct flag_type sex_flags[] =
{
	//	name,			bit,			settable
	{	"male",			SEX_MALE,		true	},
	{	"female",		SEX_FEMALE,		true	},
	{	"neutral",		SEX_NEUTRAL,	true	},
	{	"random",		3,				true	},   /* ROM */
	{	"none",			SEX_NEUTRAL,	true	},
	{	NULL,			0,				0		}
};

const struct flag_type door_resets[] =
{
	//	name,					bit,	settable
	{	"open and unlocked",	0,		true	},
	{	"closed and unlocked",	1,		true	},
	{	"closed and locked",	2,		true	},
	{	NULL,					0,		0		}
};

const struct flag_type sector_flags[] =
{
	//	name,				bit,				settable
	{	"inside",			SECT_INSIDE,		true	},
	{	"city",				SECT_CITY,			true	},
	{	"field",			SECT_FIELD,			true	},
	{	"forest",			SECT_FOREST,		true	},
	{	"hills",			SECT_HILLS,			true	},
	{	"mountain",			SECT_MOUNTAIN,		true	},
	{	"water",			SECT_WATER,			true	},
	{	"unused",			SECT_UNUSED,		true	},
	{	"underwater",		SECT_UNDERWATER,	true	},
	{	"air",				SECT_AIR,			true	},
	{	"desert",			SECT_DESERT,		true	},
	{	"road",				SECT_ROAD,			true	},
	{	"conflagration",	SECT_CONFLAGRATION,	true	},
	{	"burning",			SECT_BURNING,		true	},
	{	"trail",			SECT_TRAIL,			true	},
	{	"swamp",			SECT_SWAMP,			true	},
	{	"park",				SECT_PARK,			true	},
	{	"vertical",			SECT_VERTICAL,		true	},
	{	"ice",				SECT_ICE,			true	},
	{	"snow",				SECT_SNOW,			true	},
	{	"cave",				SECT_CAVE,			true	},
	{	NULL,				0,					0		}
};

const struct flag_type type_flags[] =
{
	//	name,				bit,				settable
	{	"light",			ITEM_LIGHT,			true	},
	{	"scroll",			ITEM_SCROLL,		true	},
	{	"wand",				ITEM_WAND,			true	},
	{	"staff",			ITEM_STAFF,			true	},
	{	"weapon",			ITEM_WEAPON,		true	},
	{	"treasure",			ITEM_TREASURE,		true	},
	{	"stone",			ITEM_STONE,			true	},
	{	"armor",			ITEM_ARMOR,			true	},
	{	"potion",			ITEM_POTION,		true	},
	{	"furniture",		ITEM_FURNITURE,		true	},
	{	"trash",			ITEM_TRASH,			true	},
	{	"container",		ITEM_CONTAINER,		true	},
	{	"drinkcontainer",	ITEM_DRINK_CON,		true	},
	{	"key",				ITEM_KEY,			true	},
	{	"food",				ITEM_FOOD,			true	},
	{	"money",			ITEM_MONEY,			true	},
	{	"boat",				ITEM_BOAT,			true	},
	{	"npccorpse",		ITEM_CORPSE_NPC,	true	},
	{	"pc corpse",		ITEM_CORPSE_PC,		false	},
	{	"fountain",			ITEM_FOUNTAIN,		true	},
	{	"pill",				ITEM_PILL,			true	},
	{	"protect",			ITEM_PROTECT,		true	},
	{	"map",				ITEM_MAP,			true	},
	{	"portal",			ITEM_PORTAL,		true	},
	{	"warpstone",		ITEM_WARP_STONE,	true	},
	{	"roomkey",			ITEM_ROOM_KEY,		true	},
	{	"gem",				ITEM_GEM,			true	},
	{	"jewelry",			ITEM_JEWELRY,		true	},
	{	"campfire",			ITEM_CAMPFIRE,		true	},
	{	"altar",			ITEM_ALTAR,			true	},
	{	NULL,				0,		  			0		}
};


/*
 * Used when adding an affect to tell where it goes.
 * See addaffect and delaffect in act_olc.c
 */
const struct flag_type apply_flags[] =
{
	//	name,				bit,					settable
	{	"none",				APPLY_NONE,				true	},
	{	"strength",			APPLY_STR,				true	},
	{	"dexterity",		APPLY_DEX,				true	},
	{	"intelligence",		APPLY_INT,				true	},
	{	"wisdom",			APPLY_WIS,				true	},
	{	"constitution",		APPLY_CON,				true	},
	{	"sex",				APPLY_SEX,				true	},
	{	"class",			APPLY_CLASS,			true	},
	{	"age",				APPLY_AGE,				true	},
	{	"height",			APPLY_HEIGHT,			true	},
	{	"weight",			APPLY_WEIGHT,			true	},
	{	"mana",				APPLY_MANA,				true	},
	{	"hp",				APPLY_HIT,				true	},
	{	"move",				APPLY_MOVE,				true	},
	{	"gold",				APPLY_GOLD,				true	},
	{	"experience",		APPLY_EXP,				true	},
	{	"ac",				APPLY_AC,				true	},
	{	"hitroll",			APPLY_HITROLL,			true	},
	{	"damroll",			APPLY_DAMROLL,			true	},
	{	"saves",			APPLY_SAVES,			true	},
	{	"savingpara",		APPLY_SAVING_PARA,		true	},
	{	"savingrod",		APPLY_SAVING_ROD,		true	},
	{	"savingpetri",		APPLY_SAVING_PETRI,		true	},
	{	"savingbreath",		APPLY_SAVING_BREATH,	true	},
	{	"savingspell",		APPLY_SAVING_SPELL,		true	},
	{	"spellaffect",		APPLY_SPELL_AFFECT,		false	},
	{	"beauty",			APPLY_BEAUTY,			true,	},
	{	NULL,				0,						0		}
};

/*
 * What is seen.
 */
const struct flag_type wear_loc_strings[] =
{
	//	name,					bit,			settable
	{	"in the inventory",		WEAR_NONE,		true	},
	{	"as a light",			WEAR_LIGHT,		true	},
	{	"on the left finger",	WEAR_FINGER_L,	true	},
	{	"on the right finger",	WEAR_FINGER_R,	true	},
	{	"around the neck (1)",	WEAR_NECK_1,	true	},
	{	"around the neck (2)",	WEAR_NECK_2,	true	},
	{	"on the body",			WEAR_BODY,		true	},
	{	"over the head",		WEAR_HEAD,		true	},
	{	"on the legs",			WEAR_LEGS,		true	},
	{	"on the feet",			WEAR_FEET,		true	},
	{	"on the hands",			WEAR_HANDS,		true	},
	{	"on the arms",			WEAR_ARMS,		true	},
	{	"as a shield",			WEAR_SHIELD,	true	},
	{	"about the shoulders",	WEAR_ABOUT,		true	},
	{	"around the waist",		WEAR_WAIST,		true	},
	{	"on the left wrist",	WEAR_WRIST_L,	true	},
	{	"on the right wrist",	WEAR_WRIST_R,	true	},
	{	"wielded",				WEAR_WIELD,		true	},
	{	"held in the hands",	WEAR_HOLD,		true	},
	{	"floating nearby",		WEAR_FLOAT,		true	},
	{	NULL,					0,				0		}
};


const struct flag_type wear_loc_flags[] =
{
	//	name,		bit,			settable
	{	"none",		WEAR_NONE,		true	},
	{	"light",	WEAR_LIGHT,		true	},
	{	"lfinger",	WEAR_FINGER_L,	true	},
	{	"rfinger",	WEAR_FINGER_R,	true	},
	{	"neck1",	WEAR_NECK_1,	true	},
	{	"neck2",	WEAR_NECK_2,	true	},
	{	"body",		WEAR_BODY,		true	},
	{	"head",		WEAR_HEAD,		true	},
	{	"legs",		WEAR_LEGS,		true	},
	{	"feet",		WEAR_FEET,		true	},
	{	"hands",	WEAR_HANDS,		true	},
	{	"arms",		WEAR_ARMS,		true	},
	{	"shield",	WEAR_SHIELD,	true	},
	{	"about",	WEAR_ABOUT,		true	},
	{	"waist",	WEAR_WAIST,		true	},
	{	"lwrist",	WEAR_WRIST_L,	true	},
	{	"rwrist",	WEAR_WRIST_R,	true	},
	{	"wielded",	WEAR_WIELD,		true	},
	{	"hold",		WEAR_HOLD,		true	},
	{	"floating",	WEAR_FLOAT,		true	},
	{	"tattoo",	WEAR_BRAND,		true	},
	{	NULL,		0,				0		}
};

const struct flag_type weapon_flags[]=
{
	//	name,				bit,	settable
	{	"hit",				 0,		true	},  /*  0 */
	{	"slice",			 1,		true	},
	{	"stab",				 2,		true	},
	{	"slash",			 3,		true	},
	{	"whip",				 4,		true	},
	{	"claw",				 5,		true	},  /*  5 */
	{	"blast",			 6,		true	},
	{	"pound", 			 7,		true	},
	{	"crush",			 8,		true	},
	{	"grep",				 9,		true	},
	{	"bite",				10,		true	},  /* 10 */
	{	"pierce",			11,		true	},
	{	"suction",			12,		true	},
	{	"beating",			13,		true	},
	{	"digestion",		14,		true	},
	{	"charge",			15,		true	},  /* 15 */
	{	"slap",				16,		true	},
	{	"punch",			17,		true	},
	{	"wrath",			18,		true	},
	{	"magic",			19,		true	},
	{	"divinepower",		20,		true	},  /* 20 */
	{	"cleave",			21,		true	},
	{	"scratch",			22,		true	},
	{	"peckpierce",		23,		true	},
	{	"peckbash",			24,		true	},
	{	"chop",				25,		true	},  /* 25 */
	{	"sting",			26,		true	},
	{	"smash",			27,		true	},
	{	"shockingbite",		28,		true	},
	{	"flamingbite",		29,		true	},
	{	"freezingbite",		30,		true	},  /* 30 */
	{	"acidicbite",		31,		true	},
	{	"chomp",			32,		true	},
	{	"lifedrain",		33,		true	},
	{	"thrust",			34,		true	},
	{	"slime",			35,		true	},	/* 35 */
	{	"shock",			36,		true	},
	{	"thwack",			37,		true	},
	{	"flame",			38,		true	},
	{	"chill",			39,		true	},
	{	"lightblast",		40,		true	},	/* 40 */
	{	"forcenature",		41,		true	},
	{	"defilement",		42,		true	},
	{	"mentalassault",	43,		true	},
	{	"sonicblast",		44,		true	},
	{	"infdarkness",		45,		true	},	/* 45 */
	{	"searing",			46,		true	},
	{	"drowning",			47,		true	},
	{	"chaotic",			48,		true	},
	{	"sflame",			49,		true	},
	{	"terror",			50,		true	}, 	/* 50 */
	{	"hydrous",			51,		true	},
	{	"thorn",			52,		true	},
	{	"decomposition",	53,		true	},
	{	"ptouch",			54,		true	},
	{	"spike",			55,		true	},	/* 55 */
	{	"ooze",				56,		true	},
	{	"asphyx",			57,		true	},
	{	"sblast",			58,		true	},
	{	"poisbite",			59,		true	},
	{	"foulgrasp",		60,		true	},	/* 60 */
	{	NULL,				0,		0		}
};

const struct flag_type container_flags[] =
{
	//	name,			bit,		settable
	{	"closeable",	ASCII_A,	true	},
	{	"pickproof",	ASCII_B,	true	},
	{	"closed",		ASCII_C,	true	},
	{	"locked",		ASCII_D,	true	},
	{	"puton",		ASCII_E,	true	},
	{	NULL,			0,			0		}
};

/*****************************************************************************
                 ROM - specific tables:
 ****************************************************************************/

const struct flag_type ac_type[] =
{
	//	name,			bit,			settable
	{	"pierce",		AC_PIERCE,		true	},
	{	"bash",			AC_BASH,		true	},
	{	"slash",		AC_SLASH,		true	},
	{	"exotic",		AC_EXOTIC,		true	},
	{	NULL,			0,				0		}
};


const struct flag_type size_flags[] =
{
	//	name,			bit,			settable
	{	"tiny",			SIZE_TINY,		true	},
	{	"small",		SIZE_SMALL,		true	},
	{	"medium",		SIZE_MEDIUM,	true	},
	{	"large",		SIZE_LARGE,		true	},
	{	"huge",			SIZE_HUGE,		true	},
	{	"giant",		SIZE_GIANT,		true	},
	{	NULL,			0,				0		}
};


const struct flag_type weapon_class[] =
{
	//	name,			bit,		settable
	{	"none",			 0,			true	},
	{	"exotic",		 1,			true	},
	{	"sword",		 2,			true	},
	{	"dagger",		 3,			true	},
	{	"spear",		 4,			true	},
	{	"mace",			 5,			true	},
	{	"axe",			 6,			true	},
	{	"flail",		 7,			true	},
	{	"whip",			 8,			true	},
	{	"polearm",		 9,			true	},
	{	"staff",		10,			true	},
	{	"hand",			11,			true	},
	{	NULL,			 0,			0		}
};

const struct flag_type weapon_type2[] =
{
	//	name,			bit,				settable
	{	"none",			0,					true	},
	{	"flaming",		WEAPON_FLAMING,		true	},
	{	"frost",		WEAPON_FROST,		true	},
	{	"vampiric",		WEAPON_VAMPIRIC,	true	},
	{	"sharp",		WEAPON_SHARP,		true	},
	{	"vorpal",		WEAPON_VORPAL,		true	},
	{	"twohands",		WEAPON_TWO_HANDS,	true	},
	{	"shocking",		WEAPON_SHOCKING,	true	},
	{	"poison",		WEAPON_POISON,		true	},
	{	NULL,			0,					0		}
};

const struct flag_type res_flags[] =
{
	//	name,			bit,				settable
	{	"summon",		RES_SUMMON,			true	},
	{	"charm",		RES_CHARM,			true	},
	{	"magic",		RES_MAGIC,			true	},
	{	"weapon",		RES_WEAPON,			true	},
	{	"bash",			RES_BASH,			true	},
	{	"pierce",		RES_PIERCE,			true	},
	{	"slash",		RES_SLASH,			true	},
	{	"fire",			RES_FIRE,			true	},
	{	"cold",			RES_COLD,			true	},
	{	"lightning",	RES_LIGHTNING,		true	},
	{	"acid",			RES_ACID,			true	},
	{	"poison",		RES_POISON,			true	},
	{	"negative",		RES_NEGATIVE,		true	},
	{	"holy",			RES_HOLY,			true	},
	{	"energy",		RES_ENERGY,			true	},
	{	"mental",		RES_MENTAL,			true	},
	{	"disease",		RES_DISEASE,		true	},
	{	"drowning",		RES_DROWNING,		true	},
	{	"light",		RES_LIGHT,			true	},
	{	"sound",		RES_SOUND,		 	true	},
	{	"silver",		RES_SILVER,			true	},
	{	"iron",			RES_IRON,			true	},
	{	NULL,			0,					0		}
};

const struct flag_type vuln_flags[] =
{
	//	name,			bit,				settable
	{	"summon",		VULN_SUMMON,		true	},
	{	"charm",		VULN_CHARM,			true	},
	{	"magic",		VULN_MAGIC,			true	},
	{	"weapon",		VULN_WEAPON,		true	},
	{	"bash",			VULN_BASH,			true	},
	{	"pierce",		VULN_PIERCE,		true	},
	{	"slash",		VULN_SLASH,			true	},
	{	"fire",			VULN_FIRE,			true	},
	{	"cold",			VULN_COLD,			true	},
	{	"lightning",	VULN_LIGHTNING,		true	},
	{	"acid",			VULN_ACID,			true	},
	{	"poison",		VULN_POISON,		true	},
	{	"negative",		VULN_NEGATIVE,		true	},
	{	"holy",			VULN_HOLY,			true	},
	{	"energy",		VULN_ENERGY,		true	},
	{	"mental",		VULN_MENTAL,		true	},
	{	"disease",		VULN_DISEASE,		true	},
	{	"drowning",		VULN_DROWNING,		true	},
	{	"light",		VULN_LIGHT,			true	},
	{	"sound",		VULN_SOUND,			true	},
	{	"silver",		VULN_SILVER,		true	},
	{	"iron",			VULN_IRON,			true	},
	{	NULL,			0,					0		}
};

const struct flag_type material_type[] =    /* not yet implemented */
{
	//	name,		bit,	settable
	{	"none",		0,		true	},
	{	NULL,		0,		0		}
};

const struct flag_type position_flags[] =
{
	//	name,			bit,				settable
	{	"dead",			POS_DEAD,			false	},
	{	"mortal",		POS_MORTAL,			false	},
	{	"incap",		POS_INCAP,			false	},
	{	"stunned",		POS_STUNNED,		false	},
	{	"sleeping",		POS_SLEEPING,		true	},
	{	"resting",		POS_RESTING,		true	},
	{	"sitting",		POS_SITTING,		true	},
	{	"fighting",		POS_FIGHTING,		false	},
	{	"standing",		POS_STANDING,		true	},
	{	NULL,			0,					0		}
};

const struct flag_type portal_flags[]=
{
	//	name,			bit,				settable
	{	"normal_exit",	GATE_NORMAL_EXIT,	true	},
	{	"no_curse",		GATE_NOCURSE,		true	},
	{	"go_with",		GATE_GOWITH,		true	},
	{	"buggy",		GATE_BUGGY,			true	},
	{	"random",		GATE_RANDOM,		true	},
	{	NULL,		 	 0,					0		}
};

const struct flag_type furniture_flags[]=
{
	//	name,			bit,			settable
	{	"none",			0,				true	},
	{	"stand_at",		STAND_AT,		true	},
	{	"stand_on",		STAND_ON,		true	},
	{	"stand_in",		STAND_IN,		true	},
	{	"sit_at",		SIT_AT,			true	},
	{	"sit_on",		SIT_ON,			true	},
	{	"sit_in",		SIT_IN,			true	},
	{	"rest_at",		REST_AT,		true	},
	{	"rest_on",		REST_ON,		true	},
	{	"rest_in",		REST_IN,		true	},
	{	"sleep_at",		SLEEP_AT,		true	},
	{	"sleep_on",		SLEEP_ON,		true	},
	{	"sleep_in",		SLEEP_IN,		true	},
	{	"put_at",		PUT_AT,			true	},
	{	"put_on",		PUT_ON,			true	},
	{	"put_in",		PUT_IN,			true	},
	{	"put_inside", 	PUT_INSIDE,		true	},
	{	"lounge_on",	LOUNGE_ON,		true	},
	{	NULL,			0,				0		}
};

const struct display_type altdesc_condtable[] =
{
	//	name,			bit,			display
	{	"nighttime",	AD_COND_NIGHT,	"shown while sun is down"	},
	{	NULL,			0,				NULL						}
};
