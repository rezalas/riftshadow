/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *      *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.              *
 *      *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.              *
 *      *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.*
 ***************************************************************************/

/***************************************************************************
*	ROM 2.4 is copyright 1993-1996 Russ Taylor			   *
*	ROM has been brought to you by the ROM consortium		   *
*	    Russ Taylor (rtaylor@pacinfo.com)				   *
*	    Gabrielle Taylor (gtaylor@pacinfo.com)			   *
*	    Brian Moore (rom@rom.efn.org)				   *
*	By using this code, you have agreed to follow the terms of the	   *
*	ROM license, in the file Tartarus/doc/rom.license  *
***************************************************************************/

/***************************************************************************
*       Tartarus code is copyright (C) 1997-1998 by Daniel Graham          *
*	In using this code you agree to comply with the Tartarus license   *
*       found in the file /Tartarus/doc/tartarus.doc       *
***************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <ctype.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>
/* #include <sys/resource.h> */
#include "merc.h"
#include "db.h"
#include "recycle.h"
#include "lookup.h"
#include "tables.h"
#include "update.h"
#include "interp.h"
#include "olc.h"
#include "spec.h"

#if !defined(OLD_RAND)
//long random();
void srandom(unsigned int);
int getpid();
time_t time(time_t *tloc);
#endif

/* externals for counting purposes */
extern	OBJ_DATA	*obj_free;
extern	CHAR_DATA	*char_free;
extern  DESCRIPTOR_DATA *descriptor_free;
extern	PC_DATA		*pcdata_free;
extern  AFFECT_DATA	*affect_free;
char *top_bounty_name[MAX_TOP_BOUNTY];
long top_bounty_value[MAX_TOP_BOUNTY];
/*
 * Globals.
 */
char * flag_name_lookup args((long bitv, const struct flag_type *flag_table));
int flag_lookup args((const char *name, const struct flag_type *flag_table));
char * bitvector_to_flag args((long bitvect[]));
int flag_index_ilookup args((int i, const struct flag_type *flag_table));
void reset_chessboard ( void );
void sort_areas(void);
void load_race_info(void);
HELP_DATA *		help_first;
HELP_DATA *		help_last;

SHOP_DATA *		shop_first;
SHOP_DATA *		shop_last;

extern NOTE_DATA *		note_free;
char * display_name_lookup args((long bitv, const struct display_type *flag_table));
char * restrict_name_lookup args((long bitv));
int sex_lookup args((const char *name));
char			bug_buf		[2*MAX_INPUT_LENGTH];
CHAR_DATA *		char_list;
char *			help_greeting;
char			log_buf		[2*MAX_INPUT_LENGTH];
KILL_DATA		kill_table	[MAX_LEVEL];
NOTE_DATA *		note_list;
OBJ_DATA *		object_list;
TIME_INFO_DATA	time_info;
sh_int			sun;
sh_int			moon_berus;
sh_int			berus_pos;
sh_int			moon_calabren;
sh_int			calabren_pos;
ROOM_INDEX_DATA *	room_list = NULL;
ROOM_INDEX_DATA *       top_affected_room;
RUNE_DATA *		rune_list = NULL;
MOB_INDEX_DATA *	mindex_list = NULL;
OBJ_INDEX_DATA *	oIndex_list = NULL;
extern	AREA_DATA *	area_first;
long			gold_constant = 0;
long			total_gold = 0;
long			player_gold = 0;
long			total_wealth = 0;
RACE_DATA * race_list;
sh_int			chessboard[8][8];
CHAR_DATA *		chess_white;
CHAR_DATA *		chess_black;

sh_int			gsn_timer;
sh_int			gsn_repose;
sh_int			gsn_medicine;
sh_int			gsn_somatic_casting;
sh_int			gsn_frigidaura;
sh_int			gsn_stasis_wall;
sh_int			gsn_integrate;
sh_int			gsn_sensevibrations;
sh_int			gsn_diamondskin;
sh_int			gsn_hardenfist;
sh_int			gsn_jackal;
sh_int			gsn_mangled;
sh_int			gsn_rage;
sh_int			gsn_horde_communion;
sh_int			gsn_bleeding;
sh_int			gsn_calm;
sh_int			gsn_agitate;
sh_int			gsn_immolate;
sh_int			gsn_enervate;
sh_int			gsn_corona;
sh_int			gsn_infravision;
sh_int			gsn_faerie_fire;
sh_int			gsn_faerie_fog;
sh_int			gsn_backstab;
sh_int         	gsn_detect_hidden;
sh_int		gsn_detect_movement;
sh_int		gsn_bluff;
sh_int			gsn_dodge;
sh_int			gsn_envenom;
sh_int			gsn_hide;
sh_int			gsn_peek;
sh_int			gsn_pick_lock;
sh_int			gsn_sneak;
sh_int			gsn_steal;
sh_int			gsn_silence;
sh_int			gsn_disarm;
sh_int			gsn_enhanced_damage;
sh_int  gsn_kick;
sh_int			gsn_parry;
sh_int			gsn_rescue;
sh_int			gsn_second_attack;
sh_int			gsn_third_attack;
sh_int			gsn_fourth_attack;
sh_int			gsn_fifth_attack;

sh_int			gsn_blindness;
sh_int			gsn_chill_touch;
sh_int			gsn_burning_hands;
sh_int			gsn_charm_person;
sh_int			gsn_curse;
sh_int			gsn_invis;
sh_int			gsn_mass_invis;
sh_int			gsn_poison;
sh_int			gsn_plague;
sh_int			gsn_sleep;
sh_int			gsn_sanctuary;
sh_int			gsn_stoneskin;
sh_int			gsn_shield;
sh_int			gsn_fly;
/* new gsn's */

sh_int  		gsn_axe;
sh_int  		gsn_dagger;
sh_int  		gsn_flail;
sh_int  		gsn_mace;
sh_int  		gsn_polearm;
sh_int			gsn_shield_block;
sh_int  		gsn_spear;
sh_int  		gsn_sword;
sh_int  		gsn_whip;

sh_int  		gsn_bash;
sh_int  		gsn_berserk;
sh_int  		gsn_dirt;
sh_int  		gsn_hand_to_hand;
sh_int  		gsn_trip;

sh_int  		gsn_fast_healing;
sh_int  		gsn_haggle;
sh_int  		gsn_lore;
sh_int  		gsn_meditation;

sh_int  		gsn_scrolls;
sh_int  		gsn_staves;
sh_int  		gsn_wands;
sh_int  		gsn_recall;

sh_int			gsn_crush;
sh_int			gsn_ground_control;

sh_int			gsn_absorb;
sh_int 			gsn_camouflage;
sh_int 			gsn_acute_vision;
sh_int			gsn_ambush;
sh_int 			gsn_laying_hands;
sh_int 			gsn_circle_stab;
sh_int 			gsn_circle;
sh_int			gsn_dual_backstab;
sh_int			gsn_skin;
sh_int			gsn_camp;

sh_int			gsn_morph_red;
sh_int			gsn_morph_black;
sh_int			gsn_morph_blue;
sh_int			gsn_morph_green;
sh_int			gsn_morph_white;

sh_int 			gsn_morph_dragon;
sh_int			gsn_mimic;
sh_int			gsn_morph_weapon;
sh_int			gsn_liquify;
sh_int			gsn_breath_morph;
sh_int			gsn_animate_dead;
sh_int			gsn_barkskin;
sh_int			gsn_animal_call;
sh_int			gsn_aura_of_sustenance;
sh_int			gsn_shadowgate;
sh_int			gsn_eye_of_the_predator;
sh_int			gsn_blackjack;
sh_int			gsn_dual_wield;
sh_int			gsn_trance;
sh_int			gsn_wanted;
sh_int			gsn_cleave;
sh_int			gsn_herb;
sh_int			gsn_protective_shield;
sh_int			gsn_timestop;
sh_int			gsn_timestop_done;
sh_int			gsn_true_sight;
sh_int			gsn_butcher;
sh_int			gsn_vanish;
sh_int			gsn_embalm;
sh_int			gsn_counter;
sh_int			gsn_acid_spit;
sh_int			gsn_bear_call;
sh_int			gsn_trophy;
sh_int			gsn_tail;
sh_int			gsn_spellbane;
sh_int			gsn_request;
sh_int			gsn_endure;
sh_int			gsn_nerve;
sh_int			gsn_throw;
sh_int			gsn_poison_dust;
sh_int			gsn_blindness_dust;
sh_int			gsn_strangle;
sh_int			gsn_warcry;
sh_int			gsn_chimera_lion;
sh_int			gsn_chimera_goat;
sh_int			gsn_strange_form;
sh_int			gsn_enlist;
sh_int			gsn_shadowplane;
sh_int			gsn_blackjack_timer;
sh_int			gsn_strangle_timer;
sh_int			gsn_tame;
sh_int			gsn_find_water;
sh_int			gsn_track;
sh_int			gsn_shield_cleave;
sh_int			gsn_spellcraft;
sh_int			gsn_cloak_form;
sh_int			gsn_demand;
sh_int			gsn_shapeshift;
sh_int			gsn_parrot;
sh_int			gsn_door_bash;
sh_int			gsn_headbutt;
sh_int			gsn_gore;
sh_int			gsn_scorch;
sh_int			gsn_shattered_bone;
sh_int			gsn_forget;
sh_int			gsn_earthbind;
sh_int			gsn_divine_touch;
sh_int			gsn_forage;
sh_int			gsn_assassinate;
sh_int			gsn_defend;
sh_int			gsn_intimidate;
sh_int			gsn_escape;
sh_int			gsn_moving_ambush;
sh_int			gsn_pugil;
sh_int			gsn_staff;
sh_int			gsn_evaluation;
sh_int			gsn_protection_heat_cold;
sh_int			gsn_rally;
sh_int 			gsn_tactics;
sh_int			gsn_lash;
sh_int			gsn_prevent_healing;
sh_int			gsn_regeneration;
sh_int			gsn_iron_resolve;
sh_int			gsn_quiet_movement;
sh_int			gsn_atrophy;
sh_int			gsn_knife;
sh_int			gsn_disperse;
sh_int			gsn_lightwalk;
sh_int gsn_starvation;
sh_int gsn_dehydrated;
sh_int gsn_spore;
sh_int gsn_spiritual_hammer;
sh_int gsn_barbarian_strike;
sh_int gsn_barbarian_bone_tear;
sh_int gsn_hold_person;
sh_int gsn_kinetic_shield;
sh_int gsn_sanguine_ward;
sh_int gsn_impale;
sh_int gsn_retribution;
sh_int gsn_shroud_of_light;
sh_int gsn_bind;
sh_int gsn_cheap_shot;
sh_int gsn_deafen;
sh_int gsn_divine_intervention;
sh_int gsn_creep;
sh_int gsn_unholy_timer;
sh_int gsn_parting_blow;
sh_int gsn_mysterious_dream;
sh_int gsn_pincer;
sh_int gsn_devour;
sh_int gsn_keen_vision;
sh_int gsn_false_motives;
sh_int gsn_incinerate;
sh_int gsn_vampiric_touch;
sh_int gsn_mana_drain;
sh_int gsn_snare;
sh_int gsn_snaretimer;
sh_int gsn_ironhands;
sh_int gsn_charge;
sh_int gsn_overhead;
sh_int gsn_legsweep;
sh_int gsn_entwine;
sh_int gsn_whirl;
sh_int gsn_soften;
sh_int gsn_barrier;
sh_int gsn_consecrate;
sh_int gsn_blade_barrier;
sh_int gsn_aura;
sh_int gsn_sunray;
sh_int gsn_enlarge;
sh_int gsn_worldbind;
sh_int gsn_sceptre;
sh_int gsn_sceptret;
sh_int gsn_dark_blessing;
sh_int gsn_web;
sh_int gsn_displacement;
sh_int gsn_volley;
sh_int gsn_hex;
sh_int gsn_visceral;
sh_int gsn_conflagration;
sh_int gsn_gravity_well;
sh_int gsn_ultradiffusion;
sh_int gsn_molecular_disjunction;
sh_int gsn_vacuum;
sh_int gsn_cyclone_forming;
sh_int gsn_cyclone;
sh_int gsn_incandescense;
sh_int gsn_diuretic;
sh_int gsn_earthquake;
sh_int gsn_mana_conduit;
sh_int gsn_synaptic_enhancement;
sh_int gsn_synaptic_impairment;
sh_int gsn_elecshield;
sh_int gsn_interference;
sh_int gsn_abite;
sh_int gsn_lbite;
sh_int gsn_hydroperception;
sh_int gsn_decrepify;
sh_int gsn_watershield;
sh_int gsn_flood;
sh_int gsn_tidalwave;
sh_int gsn_riptide;
sh_int gsn_watermeld;
sh_int gsn_sense_disturbance;
sh_int gsn_travelease;
sh_int gsn_stability;
sh_int gsn_earthshield;
sh_int gsn_heatshield;
sh_int gsn_coldshield;
sh_int gsn_chill;
sh_int gsn_reduce;
sh_int gsn_airshield;
sh_int gsn_coagulate;
sh_int gsn_entrap;
sh_int gsn_drive;
sh_int gsn_hypothermia;
sh_int gsn_imprisonvoice;
sh_int gsn_dash;
sh_int gsn_howl;
sh_int gsn_fend;
sh_int gsn_gouge;
sh_int gsn_hobble;
sh_int gsn_crippling_blow;
sh_int gsn_maneuvering;
sh_int gsn_bleed;
sh_int gsn_catch;
sh_int gsn_posture;
sh_int gsn_unbalance;
sh_int gsn_sidestep;
sh_int gsn_concuss;
sh_int gsn_retreat;
sh_int gsn_unholy_bond;
sh_int gsn_uppercut;
sh_int gsn_disrupt_formation;
sh_int gsn_dart;
sh_int gsn_brutality;
sh_int gsn_shatter;
sh_int gsn_batter;
sh_int gsn_bloody_eyes;
sh_int gsn_broken_arm;
sh_int gsn_cracked_sternum;
sh_int gsn_hurl;
sh_int gsn_exchange;
sh_int gsn_deflect;
sh_int gsn_ease;
sh_int gsn_shieldbash;
sh_int gsn_brace;
sh_int gsn_evasion;
sh_int gsn_whirlwind;
sh_int gsn_leadership;
sh_int gsn_assess;
sh_int gsn_exploit;
sh_int gsn_outflank;
sh_int gsn_terrain_mastery;
sh_int gsn_analyze;
sh_int gsn_pull;
sh_int gsn_scramble_neurons;
sh_int gsn_acid_stream;
sh_int gsn_duelist_skill;
sh_int gsn_gladiator_skill;   sh_int gsn_barbarian_skill;
sh_int gsn_dragoon_skill;
sh_int gsn_skirmisher_skill;
sh_int gsn_tactician_skill;

sh_int gsn_talismanic;

sh_int gsn_acid_vein;
sh_int gsn_corrode_lock;
sh_int gsn_attract;
sh_int gsn_call_lightning;
sh_int gsn_grounding;
sh_int gsn_neutralize;
sh_int gsn_caustic_vapor;
sh_int gsn_noxious_fumes;
sh_int gsn_drain;
sh_int gsn_smokescreen;
sh_int gsn_shroud_of_secrecy;
sh_int gsn_smother;
sh_int gsn_noxious_ward;

sh_int gsn_blanket;
sh_int gsn_whiteout;

sh_int gsn_accumulate_heat;
sh_int gsn_earthsembrace;
sh_int gsn_melt_rock;
sh_int gsn_magma_tunnel;
sh_int gsn_heat_earth;

sh_int gsn_unbreakable; 	
sh_int gsn_freeze_door;
sh_int gsn_frost_growth;
sh_int gsn_bind_feet;

sh_int gsn_glaciate;
sh_int gsn_stalactites;
sh_int gsn_ice_blast;
sh_int gsn_icy_carapace;
sh_int gsn_sheath_of_ice;

sh_int gsn_ironskin;
sh_int gsn_burden;

sh_int gsn_cloak_of_mist;
sh_int gsn_rust;
sh_int gsn_airy_water;
sh_int gsn_cooling_mist;

sh_int gsn_creeping_tomb;
sh_int gsn_pass_without_trace;
sh_int gsn_earthfade;
sh_int gsn_quicksand;
sh_int gsn_quicksand_sinking;

sh_int gsn_puddle;
sh_int gsn_indom;
sh_int gsn_unholy_communion;
sh_int gsn_lesser_demon;
sh_int gsn_greater_demon;
sh_int gsn_gamygyn_soul;
sh_int gsn_orobas_soul;
sh_int gsn_dark_familiar;
sh_int gsn_traitors_luck;
sh_int gsn_darksight;
sh_int gsn_word_of_command;
sh_int gsn_bloodlust;
sh_int gsn_mark_of_wrath;
sh_int gsn_leech;
sh_int gsn_baals_mastery;
sh_int gsn_living_blade;

sh_int gsn_plasma_arc;
sh_int gsn_sphere_of_plasma;
sh_int gsn_essence_of_plasma;
sh_int gsn_plasma_cube;
sh_int gsn_plasma_thread;

sh_int gsn_fashion_crystal;
sh_int gsn_farsee;
sh_int gsn_rotating_ward;

sh_int gsn_plant;
sh_int gsn_gag;
sh_int gsn_drag;
sh_int gsn_palm;
sh_int gsn_trail;
sh_int gsn_tripwire;
sh_int gsn_sign;
sh_int gsn_slash;
sh_int gsn_stash;
sh_int gsn_avoid;
sh_int gsn_disguise;
sh_int gsn_shadow_cloak;
sh_int gsn_stealth;
sh_int gsn_counterfeit;
sh_int gsn_strip;
sh_int gsn_silent_movement;
sh_int gsn_infidels_weight;
sh_int gsn_burning_vision;
sh_int gsn_divine_ward;
sh_int gsn_scribe;
sh_int gsn_healing_sleep;
sh_int gsn_divine_frenzy;
sh_int gsn_awe;
sh_int gsn_strike_of_virtue;
sh_int gsn_arms_of_light;
sh_int gsn_arms_of_wrath;
sh_int gsn_arms_of_purity;
sh_int gsn_arms_of_judgement;
sh_int gsn_group_retreat;
sh_int gsn_holy_shroud;
sh_int gsn_seraphic_mantle;
sh_int gsn_shield_of_faith;
sh_int gsn_deny_magic;
sh_int gsn_nullify;
sh_int gsn_bane;
sh_int gsn_gking;
sh_int gsn_lesser_golem;
sh_int gsn_greater_golem;
sh_int gsn_aggressor;
sh_int gsn_subdue;
sh_int gsn_drowning;
sh_int gsn_hunters_awareness;
sh_int gsn_hunters_stealth;
sh_int gsn_hunters_net;
sh_int gsn_severed;
sh_int gsn_rites_of_preparation;
sh_int gsn_turn_undead;
sh_int gsn_benevolence;
sh_int gsn_intercept;
sh_int gsn_blinding_orb;
sh_int gsn_channeling;
sh_int gsn_empathy;
sh_int gsn_altruism;
sh_int gsn_tower_of_fortitude;
sh_int gsn_shadowsense;
sh_int gsn_champions_defense;
sh_int gsn_indomitable_spirit;
sh_int gsn_fervor;
sh_int gsn_piety;
sh_int gsn_spiritual_healing;
sh_int gsn_crimson_martyr;
sh_int gsn_infidels_fate;
sh_int gsn_mana_sickness;
sh_int gsn_mana_infusion;
sh_int gsn_tangleroot;
sh_int gsn_rprog_elven_down;

/* GSNS */
sh_int cabal_members[MAX_CABAL];
sh_int cabal_max[MAX_CABAL];
/*
 * Locals.
 */
MOB_INDEX_DATA *	mob_index_hash		[MAX_KEY_HASH];
OBJ_INDEX_DATA *	obj_index_hash		[MAX_KEY_HASH];
ROOM_INDEX_DATA *	room_index_hash		[MAX_KEY_HASH];
char *			string_hash		[MAX_KEY_HASH];

AREA_DATA *		area_last;

BALLOT_DATA *		ballot_first;
VOTE_DATA *		vote_first;

char *			string_space;
char *			top_string;
char			str_empty	[1];

sh_int			count_data[30000];

int			top_affect;
int			top_area;
int			top_ed;
int			top_exit;
int			top_help;
int			top_mob_index;
int			top_obj_index;
int			top_reset;
int			top_room;
int			top_shop;
int			top_vnum_room;  /* OLC */
int         top_vnum_mob;   /* OLC */
int         top_vnum_obj;   /* OLC */
int 		mobile_count = 0;
int			newmobs = 0;
int			newobjs = 0;


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
int			nAllocString;
int			sAllocString;
int			nAllocPerm;
int			sAllocPerm;

char * munch(char *str)
{
	static char buf[MSL];
	int i;
	char *copy;

	if (!str)
		return "";

	copy = str;

	for (i = 0; *copy && *copy != '\0'; i++) {
		if (*copy == '\r') {
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

/*
 * Semi-locals.
 */
bool			fBootDb;
FILE *			fpArea;
char			strArea[MAX_INPUT_LENGTH];
int race_occurance[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int cabal_occurance[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int class_occurance[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int sex_occurance[] = { 0, 0, 0 };
int num_pfiles = 0;
int average_played = 0;
int tempalign = 0;
int align_occurance[] = { 0, 0, 0 };
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
 * Local booting procedures.
*/
void    init_mm         args( ( void ) );
void	load_area	args( ( FILE *fp ) );
void    new_load_area   args( ( FILE *fp ) );   /* OLC */
void  	load_mobs  args( ( FILE *fp ) );
void    load_objs	args( ( FILE *fp ) );
void	load_resets	args( ( FILE *fp ) );
void  	load_newresets    args( ( FILE *fp ) );
void	load_rooms  args( ( FILE *fp ) );
void 	load_socials	args( ( FILE *fp ) );
void    load_votes	args( ( void ) );
void	load_cabal_items	args( ( void ) );
void	fix_exits	args( ( void ) );
void	find_adjacents	args( ( void ) );
void    clean_notes	args( ( void ) );
void    load_improgs    args( ( FILE *fp ) );
void	load_specs	args( ( FILE *fp ) );
void	reset_area	args( ( AREA_DATA * pArea ) );

void update_db_gold()
{
	char buf[MSL];
	CRow row;
	int cres;

	// Changed from > 15 to > 30 to tighten the gold up.
	cres = RS.SQL.Select("COUNT(gold) FROM players WHERE level > 30");
	if(cres)
	{
		row = RS.SQL.GetRow();
		num_pfiles  = atoi(row[0]);
	}
	else
		num_pfiles = 1;

	cres = RS.SQL.Select("SUM(gold) FROM players");
	if(cres)
	{
		row = RS.SQL.GetRow();
		player_gold = atoll(row[0]);
	}
	else
		player_gold = 1;
	
	total_gold  = gold_constant * num_pfiles;
}

/*
 * Snarf an 'area' header line.
 */
void load_area( FILE *fp )
{
    AREA_DATA *pArea;

    pArea		 = new AREA_DATA;
/*  pArea->reset_first	= NULL;
    pArea->reset_last	= NULL; */
    pArea->file_name	= fread_string(fp);

	pArea->prev			= NULL;

	zero_vector(pArea->area_flags);
	SET_BIT(pArea->area_flags, AREA_LOADING);	/* OLC */
    pArea->vnum         = top_area;             /* OLC */

	pArea->affected		= NULL;
    pArea->name		= fread_string( fp );
	pArea->credits	= fread_string( fp );
	pArea->low_range = fread_number(fp);
    pArea->high_range = fread_number(fp);
	pArea->min_vnum	= fread_number(fp);
    pArea->max_vnum	= fread_number(fp);
    pArea->area_type	= flag_lookup(fread_word(fp),area_type_table);
	pArea->climate		= climate_lookup(fread_word(fp));
    fread_flag_new(pArea->area_flags,fp);
	pArea->security = fread_number(fp);
	pArea->builders	= fread_string( fp );
	pArea->age		= 15;
    pArea->nplayer	= 0;
    pArea->empty	= FALSE;
	pArea->nplayer = 0;
    zero_vector(pArea->affected_by);

	/* Morg - Valgrind fix. */
	zero_vector(pArea->progtypes);
	pArea->aprogs = NULL;
	pArea->temp	= 0;
	pArea->sky = 0;	
	
    if ( !area_first )
		area_first = pArea;

    if ( area_last ) {
		area_last->next = pArea;
		pArea->prev = area_last;
        REMOVE_BIT(area_last->area_flags, AREA_LOADING);        /* OLC */
    }

    area_last	= pArea;
    pArea->next	= NULL;

    top_area++;
    return;
}

void sort_areas(void)
{
	AREA_DATA *placeholder;
	AREA_DATA *pArea;
	bool sorted = FALSE;
	bool first = FALSE;
	int vnum = 0;

	while (!sorted) {
		sorted = TRUE;
		for(pArea = area_first; pArea; pArea = pArea->next) {
			first = FALSE;
			
			if (!pArea->next)
				break;

			if (pArea == area_first)
				first = TRUE;

			if (pArea->min_vnum > pArea->next->min_vnum) {
				sorted = FALSE;
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

	for (pArea = area_first; pArea; pArea = pArea->next) {
		pArea->vnum = vnum;
		vnum++;
	}
		
				

	return;
}

/*
 * OLC
 * Use these macros to load any new area formats that you choose to
 * support on your MUD.  See the new_load_area format below for
 * a short example.
 */
#if defined(KEY)
#undef KEY
#endif

#define KEY( literal, field, value )                \
                if ( !str_cmp( word, literal ) )    \
                {  \
    field  = value;\
    fMatch = TRUE; \
    break;         \
                }

#define SKEY( string, field )       \
                if ( !str_cmp( word, string ) )     \
                {  \
    free_pstring( field );           \
    field = fread_string( fp );     \
    fMatch = TRUE; \
    break;         \
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
void new_load_area( FILE *fp )
{
    AREA_DATA *pArea;
    char      *word;
    bool      fMatch;

    pArea                = new AREA_DATA;
    pArea->age          = 15;
    pArea->nplayer      = 0;
    pArea->file_name     = palloc_string( strArea );
    pArea->vnum         = top_area;
    pArea->name         = palloc_string( "New Area" );
    pArea->builders     = palloc_string( "" );
	pArea->security		= 1;
	pArea->min_vnum        = 0;
    pArea->max_vnum        = 0;
	pArea->nplayer		= 0;
    zero_vector(pArea->area_flags);
/*  pArea->recall       = ROOM_VNUM_TEMPLE;        ROM OLC */

    for ( ; ; )
    {
       word   = feof( fp ) ? (char*)"End" : fread_word( fp );
       fMatch = FALSE;

       switch ( UPPER(word[0]) )
       {
           case 'N':
            SKEY( "Name", pArea->name );
            break;
           case 'S':
             KEY( "Security", pArea->security, fread_number( fp ) );
            break;
           case 'V':
            if ( !str_cmp( word, "VNUMs" ) )
            {
                pArea->min_vnum = fread_number( fp );
                pArea->max_vnum = fread_number( fp );
            }
            break;
           case 'E':
             if ( !str_cmp( word, "End" ) )
             {
 fMatch = TRUE;
 if ( area_first == NULL )
    area_first = pArea;
 if ( area_last  != NULL )
    area_last->next = pArea;
 area_last   = pArea;
 pArea->next = NULL;
 top_area++;
 return;
            }
            break;
           case 'B':
            SKEY( "Builders", pArea->builders );
            break;
	   case 'C':
	    SKEY( "Credits", pArea->credits );
	    break;
        }
    }
}

/*
 * Sets vnum range for area using OLC protection features.
 */
void assign_area_vnum( int vnum )
{
    if ( area_last->min_vnum == 0 || area_last->max_vnum == 0 )
        area_last->min_vnum = area_last->max_vnum = vnum;
    if ( vnum != URANGE( area_last->min_vnum, vnum, area_last->max_vnum ) )
	{
		if ( vnum < area_last->min_vnum )
            area_last->min_vnum = vnum;
        else
			area_last->max_vnum = vnum;
	}
    return;
}

/*
 * Adds a reset to a room.  OLC
 * Similar to add_reset in olc.c
 */
void new_reset( ROOM_INDEX_DATA *pR, RESET_DATA *pReset )
{
    RESET_DATA *pr;

    if ( !pR )
       return;

    pr = pR->reset_last;

    if ( !pr )
    {
        pR->reset_first = pReset;
        pR->reset_last  = pReset;
    }
    else
    {
        pR->reset_last->next = pReset;
        pR->reset_last       = pReset;
        pR->reset_last->next = NULL;
    }

    top_reset++;
    return;
}

/*
 * Snarf a reset section.
 */
void load_resets( FILE *fp )
{
    RESET_DATA *pReset;
    int         iLastRoom = 0;
    int         iLastObj  = 0;

    if ( !area_last )
    {
	bug( "Load_resets: no #AREA seen yet.", 0 );
	exit( 1 );
    }

    for ( ; ; )
    {
	ROOM_INDEX_DATA *pRoomIndex;
	EXIT_DATA *pexit = NULL;
	char letter;
/*	OBJ_INDEX_DATA *temp_index;
	int temp; */

	if ( ( letter = fread_letter( fp ) ) == 'S' )
	    break;

	if ( letter == '*' )
	{
	    fread_to_eol( fp );
	    continue;
	}

	pReset		 = new RESET_DATA;
	pReset->command	= letter;
	/* if_flag */	  fread_number( fp );
	pReset->arg1	= fread_number( fp );
	pReset->arg2	= fread_number( fp );
	pReset->arg3	= (letter == 'G' || letter == 'R')
			    ? 0 : fread_number( fp );
	pReset->arg4	= (letter == 'P' || letter == 'M')
			    ? fread_number(fp) : 0;
/*	pReset->arg4	= (letter == 'P' || letter == 'M')
			    ? ((temp == 0)? 1 : fread_number(fp)) : 0; */
			  fread_to_eol( fp );

	/*
	 * Validate parameters.
	 * We're calling the index functions for the side effect.
	 */
	switch ( letter )
	{
	default:
	    bug( "Load_resets: bad command '%c'.", letter );
	    exit( 1 );
	    break;

	case 'M':
	    get_mob_index  ( pReset->arg1 );
/*	    get_room_index ( pReset->arg3 ); */
            if ( ( pRoomIndex = get_room_index ( pReset->arg3 ) ) )
            {
                new_reset( pRoomIndex, pReset );
                iLastRoom = pReset->arg3;
            }
	    break;

	case 'O':
/*	    temp_index = get_obj_index  ( pReset->arg1 );
	    temp_index->reset_num++;
	    get_room_index ( pReset->arg3 ); */
	    get_obj_index ( pReset->arg1 );
            if ( ( pRoomIndex = get_room_index ( pReset->arg3 ) ) )
            {
                new_reset( pRoomIndex, pReset );
                iLastObj = pReset->arg3;
            }
	    break;

	case 'P':
/*	    temp_index = get_obj_index  ( pReset->arg1 );
	    temp_index->reset_num++;
	    get_obj_index  ( pReset->arg3 ); */
	    get_obj_index  ( pReset->arg1 );
            if ( ( pRoomIndex = get_room_index ( iLastObj ) ) )
            {
                new_reset( pRoomIndex, pReset );
            }
	    break;

	case 'G':
	case 'E':
/*	    temp_index = get_obj_index  ( pReset->arg1 );
	    temp_index->reset_num++; */
	    get_obj_index (pReset->arg1);
            if ( ( pRoomIndex = get_room_index ( iLastRoom ) ) )
            {
                new_reset( pRoomIndex, pReset );
                iLastObj = iLastRoom;
            }
	    break;

	case 'D':
	    pRoomIndex = get_room_index( pReset->arg1 );

	    if ( pReset->arg2 < 0
	    ||   pReset->arg2 > 5
        || !pRoomIndex
	    || !( pexit = pRoomIndex->exit[pReset->arg2] )
	    || !IS_SET( pexit->exit_info, EX_ISDOOR ) )
	    {
		bug( "Load_resets: 'D': exit %d not door.", pReset->arg2 );
		exit( 1 );
	    }

/*	    if ( pReset->arg3 < 0 || pReset->arg3 > 2 )
	    {
		bug( "Load_resets: 'D': bad 'locks': %d.", pReset->arg3 );
		exit( 1 );
	    } */

            switch ( pReset->arg3 )
            {
                default:
					bug( "Load_resets: 'D': bad 'locks': %d." , pReset->arg3);
                case 0: break;
				case 1:
						SET_BIT( pexit->exit_info, EX_CLOSED );
						break;
                case 2:
                	SET_BIT( pexit->exit_info, EX_CLOSED);
					SET_BIT( pexit->exit_info, EX_LOCKED );
					break;
            }

	    break;

	case 'R':
	    pRoomIndex		= get_room_index( pReset->arg1 );

	    if ( pReset->arg2 < 0 || pReset->arg2 > 6 )
	    {
		bug( "Load_resets: 'R': bad exit %d.", pReset->arg2 );
		exit( 1 );
	    }

            if ( pRoomIndex )
                new_reset( pRoomIndex, pReset );

	    break;
	}

/*	if ( area_last->reset_first == NULL )
	    area_last->reset_first	= pReset;
	if ( area_last->reset_last  != NULL )
	    area_last->reset_last->next	= pReset;
	
	area_last->reset_last	= pReset;*/
	pReset->next		= NULL;
	top_reset++;
    }

    return;
}


/*
 * Snarf a shop section.
 */
void load_shops( FILE *fp )
{/*
    SHOP_DATA *pShop;

    for ( ; ; )
    {
	MOB_INDEX_DATA *pMobIndex;
	int iTrade;

	pShop			= palloc_struct( sizeof(*pShop) );
	pShop->keeper		= fread_number( fp );
	if ( pShop->keeper == 0 )
	    break;
	for ( iTrade = 0; iTrade < MAX_TRADE; iTrade++ )
	    pShop->buy_type[iTrade]	= fread_number( fp );
	pShop->profit_buy	= fread_number( fp );
	pShop->profit_sell	= fread_number( fp );
	pShop->open_hour	= fread_number( fp );
	pShop->close_hour	= fread_number( fp );
				  fread_to_eol( fp );
	pMobIndex		= get_mob_index( pShop->keeper );
	pMobIndex->pShop	= pShop;

	if ( shop_first == NULL )
	    shop_first = pShop;
	if ( shop_last  != NULL )
	    shop_last->next = pShop;

	shop_last	= pShop;
	pShop->next	= NULL;
	top_shop++;
    }*/

    return;
}


/* LOAD CABAL ITEMS */
extern int mPort;

void load_cabal_items(void)
{
	FILE *fp;
	OBJ_DATA *obj;
	CHAR_DATA *mob;

	int inum, vnum;

	if(mPort != 9999)
		return;

	fp=fopen("citems.txt","r");
	if(!fp)
		return;
	while((inum = fread_number(fp)) != -1)
	{
		vnum = fread_number(fp);
		obj = create_object(get_obj_index(cabal_table[inum].item_vnum),0);
		if(!vnum)
		{
			for(mob = char_list; mob; mob = mob->next)
				if(mob->cabal == inum && IS_CABAL_GUARD(mob))
					obj_to_char(obj, mob);
			continue;
		}
		for(mob = char_list; mob; mob = mob->next)
			if(mob->pIndexData->vnum == vnum)
				obj_to_char(obj, mob);
	}
	fclose(fp);
}
/*
 * Translate all room exits from virtual to real.
 * Has to be done after all rooms are read in.
 * Check for bad reverse exits.
 */
void fix_exits( void )
{
    ROOM_INDEX_DATA *pRoomIndex;
    EXIT_DATA *pexit;
    int iHash;
    int door;

    for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
    {
	for ( pRoomIndex  = room_index_hash[iHash];
	      pRoomIndex != NULL;
	      pRoomIndex  = pRoomIndex->next )
	{
	    bool fexit;

	    fexit = FALSE;
	    for ( door = 0; door <= 5; door++ )
	    {
		if ( ( pexit = pRoomIndex->exit[door] ) != NULL )
		{
		    if ( pexit->u1.vnum <= 0
		    || get_room_index(pexit->u1.vnum) == NULL)
			pexit->u1.to_room = NULL;
		    else
		    {
		   	fexit = TRUE;
			pexit->u1.to_room = get_room_index( pexit->u1.vnum );
			if(IS_SET(pexit->exit_info, EX_CLOSED) && !IS_SET(pexit->exit_info, EX_ISDOOR))
				SET_BIT(pexit->exit_info, EX_ISDOOR);
		    }
		}
	    }
	    if (!fexit)
		SET_BIT(pRoomIndex->room_flags,ROOM_NO_MOB);
	}
    }
/*
    for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
    {
	for ( pRoomIndex  = room_index_hash[iHash];
	      pRoomIndex != NULL;
	      pRoomIndex  = pRoomIndex->next )
	{
	    for ( door = 0; door <= 5; door++ )
	    {
		if ( ( pexit     = pRoomIndex->exit[door]       ) != NULL
		&&   ( to_room   = pexit->u1.to_room            ) != NULL
		&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
		&&   pexit_rev->u1.to_room != pRoomIndex
		&&   (pRoomIndex->vnum < 1200 || pRoomIndex->vnum > 1299))
		{
		    sprintf( buf, "Fix_exits: %d:%d -> %d:%d -> %d.",
			pRoomIndex->vnum, door,
			to_room->vnum,    rev_dir[door],
			(pexit_rev->u1.to_room == NULL)
			    ? 0 : pexit_rev->u1.to_room->vnum );
		    bug( buf, 0 );
		}
	    }
	}
    }
*/
    return;
}

void find_adjacents( void )
{
	AREA_DATA *area;
	int count, dir, i;
	ROOM_INDEX_DATA *room, *to_room;
	EXIT_DATA *pexit;
	bool in;

	for (area = area_first; area; area = area->next)
	{
		count = 0;

		for (i = 0; i < MAX_ADJACENT; i++)
		{
			area->adjacent[i] = NULL;
		}

		for (room = room_list; room != NULL; room = room->next_room)
		{
			if (room->area != area)
				continue;

			for (dir = 0; dir <= 5; dir++)
			{
				if ((pexit = room->exit[dir]) != NULL)
				{
					if ((to_room = pexit->u1.to_room) != NULL)
					{
						if (to_room->area != area)
						{
							in = FALSE;
							for (i = 0; i < MAX_ADJACENT; i++)
							{
								if (area->adjacent[i] == to_room->area)
									in = TRUE;
							}
							if (in == FALSE)
							{
								area->adjacent[count] = to_room->area;
								count++;
							}
						}
					}
				}
			}
		}
	}
}

/*
 * Repopulate areas periodically.
 */
void area_update( void )
{
    AREA_DATA *pArea;
	AREA_AFFECT_DATA *paf, *paf_next;
    char buf[MAX_STRING_LENGTH];
	
    for ( pArea = area_first; pArea != NULL; pArea = pArea->next ){

		if (IS_SET(pArea->progtypes, APROG_TICK))
			(pArea->aprogs->tick_prog) (pArea);

		for (paf = pArea->affected; paf; paf = paf_next) {
			paf_next = paf->next;
			if (paf->duration > 0 ) {
				if (paf->tick_fun)
					(*paf->tick_fun) (pArea,paf);
				paf->duration--;
			} else if (paf->duration < 0)
				;
			else
				affect_remove_area(pArea,paf);
		}
		
		if (pArea->nplayer == 0)  {
			pArea->empty = TRUE;
		} else {
			pArea->empty = FALSE;
		}
	
		if ( ++pArea->age < 6 )
		    continue;

		/*
		 * Check age and reset.
		 */

		if ( (pArea->empty || pArea->age >= 30)) {
	   	 	reset_area( pArea );
			if (IS_SET(pArea->progtypes, APROG_RESET))
				(pArea->aprogs->reset_prog) (pArea);
	   		sprintf(buf,"%s has just been reset.",pArea->name);
	   	 	wiznet(buf,NULL,NULL,WIZ_RESETS,0,0);
	    	pArea->age = number_range( 0, 2 );
		}

    }

    return;
}

/* OLC
 * Reset one room.  Called by reset_area and olc.
 */
//void reset_area( AREA_DATA *pArea )
void reset_room( ROOM_INDEX_DATA *pRoom )
{
    RESET_DATA *pReset;
	CHAR_DATA *pMob;
    CHAR_DATA *mob;
	OBJ_DATA    *pObj, *pObj2;
	CHAR_DATA   *LastMob = NULL;
	OBJ_DATA    *LastObj = NULL;
	EXIT_DATA *pexit = NULL;
	bool last;
	int level = 0;
	char buf[MSL];
	CHAR_DATA *rch;
	bool found;
	
	if ( !pRoom )
		return;

    pMob = NULL;
    last = TRUE;

	if (pRoom->trap)
		pRoom->trap->armed = TRUE;

	for ( pReset = pRoom->reset_first; pReset != NULL; pReset = pReset->next )
	{
		ROOM_INDEX_DATA *pRoomIndex = NULL;
		MOB_INDEX_DATA *pMobIndex = NULL;
		OBJ_INDEX_DATA *pObjIndex = NULL;
		OBJ_INDEX_DATA *pObjToIndex = NULL;
		OBJ_DATA *secondary = NULL;
		int count, limit;

		switch ( pReset->command )
		{
			default:
	    		bug( "Reset_area: bad command %c.", pReset->command );
	    	break;

			case 'M':
	    		if ( ( pMobIndex = get_mob_index( pReset->arg1 ) ) == NULL )
	    		{
					bug( "Reset_area: 'M': bad vnum %d.", pReset->arg1 );
					continue;
	    		}

	    		if ( ( pRoomIndex = get_room_index( pReset->arg3 ) ) == NULL )
	    		{
					bug( "Reset_area: 'R': bad vnum %d.", pReset->arg3 );
					continue;
	    		}

	    		if ( pMobIndex->count >= pReset->arg2 )
	    		{
					last = FALSE;
					break;
	    		}

	    		count = 0;
	    		for (mob = pRoomIndex->people; mob != NULL; mob = mob->next_in_room)
					if (mob->pIndexData == pMobIndex)
					{
		    			count++;
		    			if (count >= pReset->arg4)
		    			{
		    				last = FALSE;
		    				break;
		    			}
					}

	    		if (count >= pReset->arg4)
					break;

				pMob = create_mobile( pMobIndex );

				/*
 				 * Some more hard coding.
 				 */
				if ( room_is_dark( pRoom ) )
					SET_BIT(pMob->affected_by, AFF_INFRARED);

				/*
 				 * Pet shop mobiles get ACT_PET set.
 				 */
				{
					ROOM_INDEX_DATA *pRoomIndexPrev;

					pRoomIndexPrev = get_room_index( pRoom->vnum - 1 );
					if ( pRoomIndexPrev
     				&& IS_SET( pRoomIndexPrev->room_flags, ROOM_PET_SHOP ) )
     				SET_BIT( pMob->act, ACT_PET);
				}

				pMob->zone = pRoomIndex->area;
				
				char_to_room( pMob, pRoom );

				LastMob = pMob;
				level  = URANGE( 0, pMob->level - 2, LEVEL_HERO - 1 ); /* -1 ROM */
				last = TRUE;
			break;

			case 'O':
				if ( !( pObjIndex = get_obj_index( pReset->arg1 ) ) )
				{
					bug( "Reset_room: 'O' 1 : bad vnum %d", pReset->arg1 );
					sprintf (buf,"%d %d %d %d",pReset->arg1, pReset->arg2, pReset->arg3, pReset->arg4 );
					bug(buf,1);
					continue;
				}

				if ( !( pRoomIndex = get_room_index( pReset->arg3 ) ) )
				{
					bug( "Reset_room: 'O' 2 : bad vnum %d.", pReset->arg3 );
					sprintf (buf,"%d %d %d %d",pReset->arg1, pReset->arg2, pReset->arg3, pReset->arg4 );
					bug(buf,1);
					continue;
				}

				for(pObj2 = pRoomIndex->contents; pObj2; pObj2 = pObj2->next_content)
					if(pObj2->pIndexData->vnum == pObjIndex->vnum && 
					   !IS_OBJ_STAT(pObj2, ITEM_DONATION_PIT) && !IS_OBJ_STAT(pObj2, ITEM_CORPSE_PC) && 
					   !IS_OBJ_STAT(pObj2, ITEM_CORPSE_NPC) &&
					   pObj2->item_type == ITEM_CONTAINER && !pObj2->contains)
						extract_obj(pObj2);
				
				if ((pObjIndex->limcount >= pObjIndex->limtotal                                                   
					 && pObjIndex->limtotal > 0) 
					 || count_obj_list( pObjIndex, pRoomIndex->contents ) > 0 )
				{
					last = FALSE;
					break;
				}
				pObj = create_object( pObjIndex, UMIN(number_fuzzy( level ), LEVEL_HERO -1) );
				pObj->cost = 0;
				pObj->owner = palloc_string("none");
				pObj->talked = palloc_string("someone a long long time ago");
				obj_to_room( pObj, pRoom );
				last = TRUE;
			break;

			case 'P':
				if ( !( pObjIndex = get_obj_index( pReset->arg1 ) ) )
				{
					bug( "Reset_room: 'P': bad vnum %d.", pReset->arg1 );
					continue;
				}

				if ( !( pObjToIndex = get_obj_index( pReset->arg3 ) ) )
				{
					bug( "Reset_room: 'P': bad vnum %d.", pReset->arg3 );
					continue;
				}

				if (pReset->arg2 > 50)
					limit = 6;
				else if (pReset->arg2 == -1)
					limit = 999;
				else
					limit = pReset->arg2;

				if ( pRoom->area->nplayer > 1
     				|| ( LastObj = get_obj_type( pObjToIndex ) ) == NULL
     				|| ( LastObj->in_room == NULL && !last)
				|| ( pObjIndex->limtotal > 0 && pObjIndex->limcount >= pObjIndex->limtotal )
     				|| ( pObjIndex->count >= limit /* && number_range(0,4) != 0 */ )
      				|| ( count = count_obj_list( pObjIndex, LastObj->contains ) ) > pReset->arg4  )
				{
					last = FALSE;
					break;
				}


				while (count < pReset->arg4) {
					pObj = create_object( pObjIndex, number_fuzzy( LastObj->level ) );
					pObj->owner = palloc_string("none");
					obj_to_obj( pObj, LastObj );
					count++;
					if (pObjIndex->limcount >= pObjIndex->limtotal && pObjIndex->limtotal > 0)
						break;
				}
	
				LastObj->value[1] = LastObj->pIndexData->value[1];
				last = TRUE;
			break;

			case 'G':
			case 'E':
				if ( !( pObjIndex = get_obj_index( pReset->arg1 ) ) )
				{
					bug( "Reset_room: 'E' or 'G': bad vnum %d.", pReset->arg1 );
					continue;
				}

				if (!last)
					break;

				if ( !LastMob )
				{
					bug( "Reset_room: 'E' or 'G': null mob for vnum %d.", pReset->arg1 );
					last = FALSE;
					break;
				}
					
				if ( LastMob->pIndexData->pShop )
				{
					int olevel=0,i,j;

					if (!pObjIndex->new_format)
						switch ( pObjIndex->item_type )
						{
							default:
								olevel = 0;
							break;

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
											olevel = UMIN(olevel,
				         							skill_table[pObjIndex->value[i]].
						     						skill_level[j]);
			    						}
									}
		    					}
								olevel = UMAX(0,(olevel * 3 / 4) - 2);
							break;
							
							case ITEM_WAND:
								olevel = number_range( 10, 20 );
							break;
							case ITEM_STAFF:
								olevel = number_range( 15, 25 );
							break;
							case ITEM_ARMOR:
								olevel = number_range(  5, 15 );
							break;
							case ITEM_WEAPON:
								olevel = number_range(  5, 15 );
							break;
							case ITEM_TREASURE:
								olevel = number_range( 10, 20 );
							break;
						}

					pObj = create_object( pObjIndex, olevel );
					pObj->owner = palloc_string("none");
					if (pObj->pIndexData->limtotal == 0)
						SET_BIT( pObj->extra_flags, ITEM_INVENTORY );
				}
				else   /* ROM OLC else version */
				{
					int limit;
					if (pReset->arg2 > 50 )  /* old format */
						limit = 6;
					else if ( pReset->arg2 == -1 || pReset->arg2 == 0 )  /* no limit */
						limit = 999;
					else
						limit = pReset->arg2;
					
					if (pObjIndex->limcount >= pObjIndex->limtotal && pObjIndex->limtotal > 0)
					{
						break;
					}
					else
					{
			            pObj=create_object(pObjIndex,UMIN(number_fuzzy(level), LEVEL_HERO - 1));

					}
					pObj->owner = palloc_string("none");
				}

				obj_to_char( pObj, LastMob );

				if ( pReset->command == 'E' )
				{
					if (pReset->arg3 == 16)
					{
						if ( (secondary = get_eq_char(LastMob,WEAR_WIELD)) != NULL)
						{
							unequip_char(LastMob,secondary,TRUE);
							equip_char(LastMob,secondary,18,TRUE);
							equip_char(LastMob,pObj,16,TRUE);
						}
						else
							equip_char( LastMob, pObj, pReset->arg3, TRUE);       					}
					else if  ( pReset->arg3 == 18)
					{
						if (get_eq_char(LastMob,WEAR_WIELD) == NULL)
							equip_char(LastMob, pObj, 16, TRUE);
						else
							equip_char(LastMob,pObj,pReset->arg3, TRUE);
					}
					else
					{
						equip_char( LastMob, pObj, pReset->arg3, TRUE);
					}
				}
				last = TRUE;
			break;
	
			case 'F':
				found = FALSE;

				if(!LastMob)
					continue;

				for (rch = LastMob->in_room->people; rch != NULL; rch = rch->next_in_room)
				{
					if (IS_NPC(rch) && (pReset->arg2 == rch->pIndexData->vnum))
					{
						found = TRUE;
						break;
					}
				}

				if (!last || !found )
					break;

				add_follower(LastMob,rch);
				LastMob->leader = rch;
			break;

			case 'D':
				if ( ( pRoomIndex = get_room_index( pReset->arg1 ) ) == NULL )                                            
				{                                                                                                         
					bug( "Reset_area: 'D': bad vnum %d.", pReset->arg1 );                                                     
					continue;                                                                                                 
				}                                                                                                         
                                                                                                                  
				if ( ( pexit = pRoomIndex->exit[pReset->arg2] ) == NULL )                                                 
					break;                                                                                                    
	                                                                                                                  
				SET_BIT( pexit->exit_info, EX_ISDOOR );                                                                   
				switch ( pReset->arg3 )                                                                                   
				{                                                                                                         
					case 0:                                                                                                   
						REMOVE_BIT( pexit->exit_info, EX_CLOSED );                                                                
						REMOVE_BIT( pexit->exit_info, EX_LOCKED );                                                                
					break;                                                                                                    
                                                                                                        
					case 1:                                                                                                   
						SET_BIT(    pexit->exit_info, EX_CLOSED );                                                                
						REMOVE_BIT( pexit->exit_info, EX_LOCKED );                                                                
					break;                                                                                                    
                                                                                                 
					case 2:                                                                                                   
						SET_BIT(    pexit->exit_info, EX_CLOSED );                                                                
						SET_BIT(    pexit->exit_info, EX_LOCKED );                                                                
					break;                                                                                                    
				}                                                                                                         
                                                                                   
				last = TRUE;                                                                                              
			break;

			case 'R':
if ( !( pRoomIndex = get_room_index( pReset->arg1 ) ) )
{
bug( "Reset_room: 'R': bad vnum %d.", pReset->arg1 );
continue;
}

{
EXIT_DATA *pExit;
int d0;
int d1;

for ( d0 = 0; d0 < pReset->arg2 - 1; d0++ )
{
d1   = number_range( d0, pReset->arg2-1 );
pExit                = pRoomIndex->exit[d0];
pRoomIndex->exit[d0] = pRoomIndex->exit[d1];
pRoomIndex->exit[d1] = pExit;
}
}
break;
}
}
return;
}

/* OLC
 * Reset one area.
 */
void reset_area( AREA_DATA *pArea )
{
	ROOM_INDEX_DATA *pRoom;
	int  vnum;

	for ( vnum = pArea->min_vnum; vnum <= pArea->max_vnum; vnum++ )
	{
		if ( ( pRoom = get_room_index(vnum) ) )
			reset_room(pRoom);
	}

	return;
}

/*
 * Create an instance of a mobile.
 */
CHAR_DATA *create_mobile( MOB_INDEX_DATA *pMobIndex )
{
    int i;
	CHAR_DATA *mob;
    AFFECT_DATA af;
	int style;

    mobile_count++;

    if ( pMobIndex == NULL )
    {
	bug( "Create_mobile: NULL pMobIndex.", 0 );
	exit( 1 );
    }

    mob = new_char();

    mob->pIndexData	= pMobIndex;

    mob->name		= palloc_string( pMobIndex->player_name );    /* OLC */
    mob->short_descr	= palloc_string( pMobIndex->short_descr );    /* OLC */
    mob->long_descr	= palloc_string( pMobIndex->long_descr );     /* OLC */
	if(pMobIndex->description)
    mob->description	= palloc_string( pMobIndex->description );    /* OLC */
    mob->id		= get_mob_id();
    mob->prompt		= NULL;
    copy_vector(mob->progtypes, pMobIndex->progtypes);
	total_wealth += pMobIndex->wealth;

    if (pMobIndex->new_format)
    /* load in new style */
    {
	/* read from prototype */
 	mob->group		= pMobIndex->group;
	copy_vector(mob->act, pMobIndex->act);
	copy_vector(mob->affected_by, pMobIndex->affected_by);
	mob->alignment		= pMobIndex->alignment;
	mob->level		= pMobIndex->level;
	mob->hitroll		= pMobIndex->hitroll;
	mob->damroll		= pMobIndex->damage[DICE_BONUS];
	mob->max_hit		= dice(pMobIndex->hit[DICE_NUMBER],
				       pMobIndex->hit[DICE_TYPE])
				  + pMobIndex->hit[DICE_BONUS];
	mob->hit		= mob->max_hit;
	mob->max_mana		= dice(pMobIndex->mana[DICE_NUMBER],
				       pMobIndex->mana[DICE_TYPE])
				  + pMobIndex->mana[DICE_BONUS];
	mob->mana		= mob->max_mana;
	mob->damage[DICE_NUMBER]= pMobIndex->damage[DICE_NUMBER];
	mob->damage[DICE_TYPE]	= pMobIndex->damage[DICE_TYPE];
	mob->dam_type		= pMobIndex->dam_type;
        if (mob->dam_type == 0)
    	    switch(number_range(1,3))
            {
                case (1): mob->dam_type = 3;        break;  /* slash */
                case (2): mob->dam_type = 7;        break;  /* pound */
                case (3): mob->dam_type = 11;       break;  /* pierce */
            }
	for (i = 0; i < 4; i++)
	    mob->armor[i]	= pMobIndex->ac[i];
	copy_vector(mob->off_flags, pMobIndex->off_flags);
	copy_vector(mob->imm_flags, pMobIndex->imm_flags);
	copy_vector(mob->res_flags, pMobIndex->res_flags);
	copy_vector(mob->vuln_flags, pMobIndex->vuln_flags);
	mob->start_pos		= pMobIndex->start_pos;
	mob->sex		= pMobIndex->sex;
        if (mob->sex == 3) /* random sex */
            mob->sex = number_range(1,2);
	mob->race		= pMobIndex->race;
	copy_vector(mob->form, pMobIndex->form);
	copy_vector(mob->parts, pMobIndex->parts),
	mob->size		= pMobIndex->size;
	copy_vector(mob->progtypes, pMobIndex->progtypes);
	mob->dam_mod		= pMobIndex->dam_mod;
	mob->defense_mod	= 0;
	mob->arms		= pMobIndex->arms;
	mob->legs		= pMobIndex->legs;
	mob->regen_rate = 0;
	mob->mobstyle	=	0;
	mob->stolen_from	= FALSE;
	if (pMobIndex->Class()->GetIndex() == CLASS_WARRIOR) {
		for (style = 1; style <= 32; style *= 2)
			if (IS_SET(pMobIndex->styles,style)) {
				mob->mobstyle = style;
				break;
			}
	}

	if(IS_SET(mob->act,ACT_WARD_MOB))
		mob->invis_level = LEVEL_HERO + 1;
	/* computed on the spot */

    	for (i = 0; i < MAX_STATS; i ++)
            mob->perm_stat[i] = UMIN(25,11 + mob->level/4);

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

        if (IS_SET(mob->act,ACT_CLERIC))
        {
            mob->perm_stat[STAT_WIS] += 3;
            mob->perm_stat[STAT_DEX] -= 1;
            mob->perm_stat[STAT_STR] += 1;
        }

        if (IS_SET(mob->act,ACT_MAGE))
        {
            mob->perm_stat[STAT_INT] += 3;
            mob->perm_stat[STAT_STR] -= 1;
            mob->perm_stat[STAT_DEX] += 1;
        }

        if (IS_SET(mob->off_flags,OFF_FAST))
            mob->perm_stat[STAT_DEX] += 2;

        mob->perm_stat[STAT_STR] += mob->size - SIZE_MEDIUM;
        mob->perm_stat[STAT_CON] += (mob->size - SIZE_MEDIUM) / 2;

	/* let's get some spell action */

		init_affect(&af);
		af.where	 = TO_AFFECTS;
		af.type      = skill_lookup("sanctuary");
		af.level     = mob->level;
		af.duration  = -1;
		af.location  = APPLY_NONE;

		if (IS_AFFECTED(mob,AFF_SANCTUARY))
		{
			SET_BIT(af.bitvector, AFF_SANCTUARY);
			affect_to_char( mob, &af );
	    }

		for(i=0;i<10;i++)
		{
		if(pMobIndex->affect_sn[i]<0)
			break;
		if(pMobIndex->affect_sn[i]>0)
		{
			af.modifier 	= 0;
			af.location 	= APPLY_NONE;
			af.type 	= pMobIndex->affect_sn[i];
			SET_BIT(af.bitvector, pMobIndex->affect_bit[i]);
			affect_to_char(mob,&af);
		}
	}
    }
    else /* read in old format and convert */
    {
	copy_vector(mob->act, pMobIndex->act);
	copy_vector(mob->affected_by, pMobIndex->affected_by);
	mob->alignment		= pMobIndex->alignment;
	mob->level		= pMobIndex->level;
	mob->hitroll		= pMobIndex->hitroll;
	mob->damroll		= 0;
	mob->max_hit		= mob->level * 8 + number_range(
					mob->level * mob->level/4,
					mob->level * mob->level);
	mob->hit		= mob->max_hit;
	mob->max_mana		= 100 + dice(mob->level,10);
	mob->mana		= mob->max_mana;
	switch(number_range(1,3))
	{
	    case (1): mob->dam_type = 3; 	break;  /* slash */
	    case (2): mob->dam_type = 7;	break;  /* pound */
	    case (3): mob->dam_type = 11;	break;  /* pierce */
	}
	for (i = 0; i < 3; i++)
	    mob->armor[i]	= 0;
	mob->armor[3]		= 0;
	mob->race		= pMobIndex->race;
	copy_vector(mob->off_flags, pMobIndex->off_flags);
	copy_vector(mob->imm_flags, pMobIndex->imm_flags);
	copy_vector(mob->res_flags, pMobIndex->res_flags);
	copy_vector(mob->vuln_flags, pMobIndex->vuln_flags);
	mob->start_pos		= pMobIndex->start_pos;
	mob->sex		= pMobIndex->sex;
	copy_vector(mob->form, pMobIndex->form);
	copy_vector(mob->parts, pMobIndex->parts);
	mob->size		= SIZE_MEDIUM;
	mob->material		= "";
	mob->arms		= pMobIndex->arms;
	mob->legs		= pMobIndex->legs;
	mob->regen_rate = 0;
	mob->timer		= 0;
        for (i = 0; i < MAX_STATS; i ++)
            mob->perm_stat[i] = 11 + mob->level/4;
    }

    mob->position = mob->start_pos;

    mob->cabal		= mob->pIndexData->cabal;
    mob->home_room	= NULL;
    /* link the mob to the world list */
    mob->next		= char_list;
    char_list		= mob;
    pMobIndex->count++;
    return mob;
}

/* duplicate a mobile exactly -- except inventory */
void clone_mobile(CHAR_DATA *parent, CHAR_DATA *clone)
{
    int i;
    AFFECT_DATA *paf;

    if ( parent == NULL || clone == NULL || !IS_NPC(parent))
	return;

    /* start fixing values */
    clone->name 	= palloc_string(parent->name);
    clone->version	= parent->version;
    clone->short_descr	= palloc_string(parent->short_descr);
    clone->long_descr	= palloc_string(parent->long_descr);
    clone->description	= palloc_string(parent->description);
    clone->group	= parent->group;
    clone->sex		= parent->sex;
    clone->SetClass(parent->Class()->GetIndex());
    clone->race		= parent->race;
    clone->level	= parent->level;
    clone->trust	= 0;
    clone->timer	= parent->timer;
    clone->wait		= parent->wait;
    clone->hit		= parent->hit;
    clone->max_hit	= parent->max_hit;
    clone->mana		= parent->mana;
    clone->max_mana	= parent->max_mana;
    clone->move		= parent->move;
    clone->max_move	= parent->max_move;
    clone->gold		= parent->gold;
    clone->exp		= parent->exp;
    copy_vector(clone->act, parent->act);
    copy_vector(clone->comm, parent->comm);
    copy_vector(clone->imm_flags, parent->imm_flags);
    copy_vector(clone->res_flags, parent->res_flags);
    copy_vector(clone->vuln_flags, parent->vuln_flags);
    clone->invis_level	= parent->invis_level;
    copy_vector(clone->affected_by, parent->affected_by);
    clone->position	= parent->position;
    clone->practice	= parent->practice;
    clone->train	= parent->train;
    clone->saving_throw	= parent->saving_throw;
    clone->alignment	= parent->alignment;
    clone->hitroll	= parent->hitroll;
    clone->damroll	= parent->damroll;
    clone->wimpy	= parent->wimpy;
    copy_vector(clone->form, parent->form);
    copy_vector(clone->parts, parent->parts);
    clone->size		= parent->size;
    copy_vector(clone->off_flags, parent->off_flags);
    clone->dam_type	= parent->dam_type;
    clone->start_pos	= parent->start_pos;
    clone->dam_mod	= parent->dam_mod;
    clone->defense_mod	= parent->defense_mod;
    clone->cabal	= parent->cabal;
    clone->arms		= parent->arms;
    clone->legs		= parent->legs;

    for (i = 0; i < 4; i++)
    	clone->armor[i]	= parent->armor[i];

    for (i = 0; i < MAX_STATS; i++)
    {
	clone->perm_stat[i]	= parent->perm_stat[i];
	clone->mod_stat[i]	= parent->mod_stat[i];
    }

    for (i = 0; i < 3; i++)
	clone->damage[i]	= parent->damage[i];

    /* now add the affects */
    for (paf = parent->affected; paf != NULL; paf = paf->next)
        affect_to_char(clone,paf);

}




/*
 * Create an instance of an object.
 */
OBJ_DATA *create_object( OBJ_INDEX_DATA *pObjIndex, int level )
{
    AFFECT_DATA *paf;
	EXTRA_DESCR_DATA *ed;
    OBJ_DATA *obj;
    int i;

    if ( pObjIndex == NULL )
    {
	bug( "Create_object: NULL pObjIndex." );
	exit( 1 );
    }

    obj = new_obj();

    obj->pIndexData	= pObjIndex;
    obj->in_room	= NULL;
    obj->ohp		= 0;
    if (pObjIndex->new_format)
	obj->level = pObjIndex->level;
    else
	obj->level		= UMAX(0,level);
    obj->wear_loc	= -1;

	obj->name     = palloc_string( pObjIndex->name );           /* OLC */
	obj->short_descr  = palloc_string( pObjIndex->short_descr );    /* OLC */
	obj->description  = palloc_string( pObjIndex->description );    /* OLC */
	obj->material	= palloc_string(pObjIndex->material);
	if(pObjIndex->wear_loc_name)
		obj->wear_loc_name = palloc_string(pObjIndex->wear_loc_name);
	else
		obj->wear_loc_name = NULL;
    obj->item_type	= pObjIndex->item_type;
    copy_vector(obj->extra_flags, pObjIndex->extra_flags);
	obj->apply		= pObjIndex->apply;
	obj->charaffs	= pObjIndex->charaffs;
    copy_vector(obj->wear_flags, pObjIndex->wear_flags);
	copy_vector(obj->imm_flags, pObjIndex->imm_flags);
	copy_vector(obj->res_flags, pObjIndex->res_flags);
	copy_vector(obj->vuln_flags, pObjIndex->vuln_flags);
	obj->value[0]	= pObjIndex->value[0];
    obj->value[1]	= pObjIndex->value[1];
    obj->value[2]	= pObjIndex->value[2];
    obj->value[3]	= pObjIndex->value[3];
    obj->value[4]	= pObjIndex->value[4];
    obj->weight		= pObjIndex->weight;
	copy_vector(obj->progtypes, pObjIndex->progtypes);

	/* Morg - Valgrind Fix */
	obj->contains	= NULL;

	if (pObjIndex->extra_descr)
	{
		ed = new_extra_descr();
	
		ed->description		=	palloc_string(pObjIndex->extra_descr->description);
		ed->keyword			=	palloc_string(pObjIndex->extra_descr->keyword);
		ed->next			=	obj->extra_descr;
		obj->extra_descr	=	ed;
	}

    if (level == -1 || pObjIndex->new_format)
	obj->cost	= pObjIndex->cost;
    else
    	obj->cost	= number_fuzzy( 10 )
			* number_fuzzy( level ) * number_fuzzy( level );

    /*
     * Mess with object properties.
     */
    switch ( obj->item_type )
    {
    default:
	bug( "Read_object: vnum %d bad type.", pObjIndex->vnum );
	break;

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
    case ITEM_CAMPFIRE:
	break;

    case ITEM_SCROLL:
	if (level != -1 && !pObjIndex->new_format)
	    obj->value[0]	= number_fuzzy( obj->value[0] );
	break;

    case ITEM_WAND:
    case ITEM_STAFF:
	if (level != -1 && !pObjIndex->new_format)
	{
	    obj->value[0]	= number_fuzzy( obj->value[0] );
	    obj->value[1]	= number_fuzzy( obj->value[1] );
	    obj->value[2]	= obj->value[1];
	}
	if (!pObjIndex->new_format)
	    obj->cost *= 2;
	break;

    case ITEM_WEAPON:
	if (level != -1 && !pObjIndex->new_format)
	{
	    obj->value[1] = number_fuzzy( number_fuzzy( 1 * level / 4 + 2 ) );
	    obj->value[2] = number_fuzzy( number_fuzzy( 3 * level / 4 + 6 ) );
	}
	break;

    case ITEM_ARMOR:
	if (level != -1 && !pObjIndex->new_format)
	{
	    obj->value[0]	= number_fuzzy( level / 5 + 3 );
	    obj->value[1]	= number_fuzzy( level / 5 + 3 );
	    obj->value[2]	= number_fuzzy( level / 5 + 3 );
	}
	break;

    case ITEM_POTION:
    case ITEM_PILL:
	if (level != -1 && !pObjIndex->new_format)
	    obj->value[0] = number_fuzzy( number_fuzzy( obj->value[0] ) );
	break;

    case ITEM_MONEY:
	if (!pObjIndex->new_format)
	    obj->value[0]	= obj->cost;
	break;
    }

    for (paf = pObjIndex->affected; paf != NULL; paf = paf->next)
	if ( paf->location == APPLY_SPELL_AFFECT ) {
		OBJ_AFFECT_DATA oaf;

		init_affect_obj(&oaf);
		oaf.where = paf->where;
		oaf.type = paf->type;
		oaf.level = paf->level;
		oaf.duration = paf->duration;
		oaf.location = paf->location;
		oaf.modifier = paf->modifier;
		copy_vector(oaf.bitvector, paf->bitvector);
		affect_to_obj(obj,&oaf);
	}

    obj->owner = palloc_string("none");
    obj->next		= object_list;
    object_list		= obj;
    pObjIndex->limcount += 1;

    return obj;
}

/* duplicate an object exactly -- except contents */
void clone_object(OBJ_DATA *parent, OBJ_DATA *clone)
{
    int i;
    OBJ_AFFECT_DATA *paf;
    EXTRA_DESCR_DATA *ed,*ed_new;

    if (parent == NULL || clone == NULL)
	return;

    /* start fixing the object */
    clone->name 	= palloc_string(parent->name);
    clone->short_descr 	= palloc_string(parent->short_descr);
    clone->description	= palloc_string(parent->description);
    clone->item_type	= parent->item_type;
    copy_vector(clone->extra_flags, parent->extra_flags);
	clone->apply		= parent->apply;
    copy_vector(clone->wear_flags, parent->wear_flags);
	copy_vector(clone->imm_flags, parent->imm_flags);
	copy_vector(clone->res_flags, parent->res_flags);
	copy_vector(clone->vuln_flags, parent->vuln_flags);
    clone->weight	= parent->weight;
    clone->cost		= parent->cost;
    clone->level	= parent->level;
    clone->condition	= parent->condition;
    clone->material	= palloc_string(parent->material);
    clone->timer	= parent->timer;
    clone->ohp		= parent->ohp;
	if(parent->wear_loc_name)
		clone->wear_loc_name = palloc_string(parent->wear_loc_name);
    for (i = 0;  i < 5; i ++)
	clone->value[i]	= parent->value[i];

    /* affects */

    for (paf = parent->affected; paf != NULL; paf = paf->next)
		affect_to_obj(clone,paf);

    /* extended desc */
    for (ed = parent->extra_descr; ed != NULL; ed = ed->next)
    {
        ed_new  = new_extra_descr();
        ed_new->keyword    	= palloc_string( ed->keyword);
        ed_new->description     = palloc_string( ed->description );
        ed_new->next           	= clone->extra_descr;
        clone->extra_descr  	= ed_new;
    }

}



/*
 * Clear a new character.
 */
void clear_char( CHAR_DATA *ch )
{
    static CHAR_DATA ch_zero;
    int i;

    *ch				= ch_zero;
    ch->name			= &str_empty[0];
    ch->short_descr		= &str_empty[0];
    ch->long_descr		= &str_empty[0];
    ch->description		= &str_empty[0];
    ch->prompt  = &str_empty[0];
    ch->logon			= current_time;
    ch->lines			= PAGELEN;
    for (i = 0; i < 4; i++)
    	ch->armor[i]		= 0;
    ch->position		= POS_STANDING;
    ch->hit			= 20;
    ch->max_hit			= 20;
    ch->mana			= 100;
    ch->max_mana		= 100;
    ch->move			= 100;
    ch->max_move		= 100;
    ch->last_fought             = NULL;
    ch->last_fight_time         = NULL;
    ch->last_fight_name         = NULL;
    ch->on			= NULL;
    ch->hometown		= 0;
    ch->arms			= 2;
    ch->legs			= 2;
    ch->balance			= 0;
	ch->regen_rate 		= 0;
	ch->ghost		= 0;
    return;
}

/*
 * Get an extra description from a list.
 */
char *get_extra_descr( const char *name, EXTRA_DESCR_DATA *ed )
{
    for ( ; ed != NULL; ed = ed->next )
    {
	if ( is_name( (char *) name, ed->keyword ) )
	    return ed->description;
    }
    return NULL;
}



/*
 * Translates mob virtual number to its mob index struct.
 * Hash table lookup.
 */
MOB_INDEX_DATA *get_mob_index( int vnum )
{
    MOB_INDEX_DATA *pMobIndex;

    for ( pMobIndex  = mob_index_hash[vnum % MAX_KEY_HASH];
	  pMobIndex != NULL;
	  pMobIndex  = pMobIndex->next )
    {
	if ( pMobIndex->vnum == vnum )
	    return pMobIndex;
    }

    if ( fBootDb )
    {
	bug( "Get_mob_index: funky vnum %d.", vnum );
	exit( 1 );
    }

    return NULL;
}



/*
 * Translates mob virtual number to its obj index struct.
 * Hash table lookup.
 */
OBJ_INDEX_DATA *get_obj_index( int vnum )
{
    OBJ_INDEX_DATA *pObjIndex;

    for ( pObjIndex  = obj_index_hash[vnum % MAX_KEY_HASH];
	  pObjIndex != NULL;
	  pObjIndex  = pObjIndex->next )
    {
	if ( pObjIndex->vnum == vnum )
	    return pObjIndex;
    }

    if ( fBootDb )
    {
	bug( "Get_obj_index: bad vnum %d.", vnum );
	exit( 1 );
    }

    return NULL;
}



/*
 * Translates mob virtual number to its room index struct.
 * Hash table lookup.
 */
ROOM_INDEX_DATA *get_room_index( int vnum )
{
    ROOM_INDEX_DATA *pRoomIndex;

    for ( pRoomIndex  = room_index_hash[vnum % MAX_KEY_HASH];
	  pRoomIndex != NULL;
	  pRoomIndex  = pRoomIndex->next )
    {
	if ( pRoomIndex->vnum == vnum )
	    return pRoomIndex;
    }

    if ( fBootDb )
    {
	bug( "Get_room_index: bad vnum %d.", vnum );
	exit( 1 );
    }

    return NULL;
}



/*
 * Read a letter from a file.
 */
char fread_letter( FILE *fp )
{
    char c;

    do
    {
	c = getc( fp );
    }
    while ( isspace(c) );

    return c;
}



/*
 * Read a number from a file.
 */
int fread_number( FILE *fp )
{
    int number;
    bool sign;
    char c;
    char buf[MAX_STRING_LENGTH], dbuf[MAX_STRING_LENGTH];

    do
    {
	c = getc( fp );
    }
    while ( isspace(c) );

    number = 0;

    sign   = FALSE;
    if ( c == '+' )
    {
	c = getc( fp );
    }
    else if ( c == '-' )
    {
	sign = TRUE;
	c = getc( fp );
    }

    if ( !isdigit(c) )
    {
	sprintf(buf,"Fread_number: bad format: %c",c);
        for(number=0;(number<25 && c!=0);number++)
	{
		c=getc(fp);
		sprintf(dbuf,"%c",c);
		strcat(buf,dbuf);
	}
	bug( buf, 0 );
	exit( 1 );
    }

    while ( isdigit(c) )
    {
	number = number * 10 + c - '0';
	c      = getc( fp );
    }

    if ( sign )
	number = 0 - number;

    if ( c == '|' )
	number += fread_number( fp );
    else if ( c != ' ' )
	ungetc( c, fp );

    return number;
}

long fread_flag( FILE *fp)
{
    int number;
    char c;
    bool negative = FALSE;

    do
    {
	c = getc(fp);
    }
    while ( isspace(c));

    if (c == '-')
    {
	negative = TRUE;
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
	number = number * 10 + c - '0';
	c = getc(fp);
    }

    if (c == '|')
	number += fread_flag(fp);

    else if  ( c != ' ')
	ungetc(c,fp);

    if (negative)
	return -1 * number;

    return number;
}

void fread_flag_new (long vector[], FILE *fp)
{
	char c;
	zero_vector(vector);
	do {
		c = getc(fp);
	} while(isspace(c));

	if (!isdigit(c)) {
		while (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z')) {
			SET_BIT(vector, vector_convert(c));
			c = getc(fp);
		}
	} else if (c == '0')
		return;

	if (c == '|')
		fread_flag_new(vector, fp);
	else if (c != ' ')
		ungetc(c,fp);

	return;
}

long vector_convert(char letter)
{
	long bit = 0;

	if ('A' <= letter && letter <= 'Z')
		bit = letter - 'A';
	else if ('a' <= letter && letter <='z')
		bit = letter - ('a' - 26);

	return bit;
}
	

long flag_convert(char letter )
{
    long bitsum = 0;
    char i;

    if ('A' <= letter && letter <= 'Z')
    {
	bitsum = 1;
	for (i = letter; i > 'A'; i--)
	    bitsum *= 2;
    }
    else if ('a' <= letter && letter <= 'z')
    {
	bitsum = 67108864; /* 2^26 */
	for (i = letter; i > 'a'; i --)
	    bitsum *= 2;
    }

    return bitsum;
}

void fread_vector(long vector[], FILE *fp)
{
	int i;

	for (i = 0; i < MAX_BITVECTOR; i++) {
		vector[i] = fread_number(fp);
	}

	return;
}

/*
 * Read and allocate space for a string from a file.
 * These strings are read-only and shared.
 * Strings are hashed:
 *   each string prepended with hash pointer to prev string,
 *   hash code is simply the string length.
 *   this function takes 40% to 50% of boot-up time.
 */
char *fread_string( FILE *fp )
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
	c = getc( fp );
    }
    while ( isspace(c) );

    if ( ( *plast++ = c ) == '~' )
	return &str_empty[0];

    for ( ;; )
    {
        /*
         * Back off the char type lookup,
         *   it was too dirty for portability.
         *   -- Furey
         */

	switch ( *plast = getc(fp) )
	{
        default:
            plast++;
            break;

        case EOF:
	/* temp fix */
            bug( "Fread_string: EOF", 0 );
	    return NULL;
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
/*	    {
		union
		{
		    char *	pc;
		    char	rgc[sizeof(char *)];
		} u1;
		int ic;
		int iHash;
		char *pHash;
		char *pHashPrev;
		char *pString;
		
		plast[-1] = '\0';

		iHash     = UMIN( MAX_KEY_HASH - 1, plast - 1 - top_string );
		for ( pHash = string_hash[iHash]; pHash; pHash = pHashPrev )
		{
		    for ( ic = 0; ic < sizeof(char *); ic++ )
			u1.rgc[ic] = pHash[ic];
		    pHashPrev = u1.pc;
		    pHash    += sizeof(char *);

		    if ( top_string[sizeof(char *)] == pHash[0]
		    &&   !strcmp( top_string+sizeof(char *)+1, pHash+1 ) )
			return pHash;
		}

		if ( fBootDb )
		{
		    pString		= top_string;
		    top_string		= plast;
		    u1.pc		= string_hash[iHash];
		    for ( ic = 0; ic < sizeof(char *); ic++ )
			pString[ic] = u1.rgc[ic];
		    string_hash[iHash]	= pString;

		    nAllocString += 1;
		    sAllocString += top_string - pString;
		    return pString + sizeof(char *);
		}
		else
		{
		    return palloc_string( top_string + sizeof(char *) );
		}
	    }
	*/
	}
    }
}

char *fread_string_eol( FILE *fp )
{
    static bool char_special[256-EOF];
    char *plast;
    char buf[MSL], c;

    if ( char_special[EOF-EOF] != TRUE )
    {
        char_special[EOF -  EOF] = TRUE;
        char_special['\n' - EOF] = TRUE;
        char_special['\r' - EOF] = TRUE;
    }
    /*
     * Skip blanks.
     * Read first char.
     */
    plast = &buf[0];

    do
    {
        c = getc( fp );
    }
    while ( isspace(c) );

    if ( ( *plast++ = c ) == '\n')
        return &str_empty[0];

    for ( ;; )
    {
        if ( !char_special[ ( *plast++ = getc( fp ) ) - EOF ] )
            continue;

        switch ( plast[-1] )
        {
        default:
            break;

        case EOF:
            bug( "Fread_string_eol  EOF", 0 );
            exit( 1 );
            break;

        case '\n':  case '\r':
            {
		plast[-1] = '\0';
		return palloc_string(buf);
            }
        }
    }
}



/*
 * Read to end of line (for comments).
 */
void fread_to_eol( FILE *fp )
{
    char c;

    do
    {
	c = getc( fp );
    }
    while ( c != '\n' && c != '\r' );

    do
    {
	c = getc( fp );
    }
    while ( c == '\n' || c == '\r' );

    ungetc( c, fp );
    return;
}



/*
 * Read one word (into static buffer).
 */
char *fread_word( FILE *fp )
{
    static char word[MAX_INPUT_LENGTH];
    char *pword;
    char cEnd;

    do
    {
	cEnd = getc( fp );
    }
    while ( isspace( cEnd ) );

    if ( cEnd == '\'' || cEnd == '"' )
    {
	pword   = word;
    }
    else
    {
	word[0] = cEnd;
	pword   = word+1;
	cEnd    = ' ';
    }

    for ( ; pword < word + MAX_INPUT_LENGTH; pword++ )
    {
	*pword = getc( fp );
	if ( cEnd == ' ' ? isspace(*pword) : *pword == cEnd )
	{
	    if ( cEnd == ' ' )
		ungetc( *pword, fp );
	    *pword = '\0';
	    return word;
	}
    }

    bug( "Fread_word: word too long.", 0 );
    bug( word, 0 );
    exit( 1 );
    return NULL;
}

/*
 * Allocate some ordinary memory,
 *   with the expectation of freeing it someday.
 */
void *iamlame( int sMem )
{
/*
    void *pMem;
    int *magic;
    int iList;

    sMem += sizeof(*magic);

    for ( iList = 0; iList < MAX_MEM_LIST; iList++ )
        if ( sMem <= rgSizeList[iList] )
            break;

    if ( iList == MAX_MEM_LIST ) {
        bug( "Alloc_mem: size %d too large.", sMem );
        exit( 1 );
    }

    if ( rgFreeList[iList] == NULL ) {
        pMem              = palloc_struct( rgSizeList[iList] );
    } else {
        pMem              = rgFreeList[iList];
        rgFreeList[iList] = * ((void **) rgFreeList[iList]);
    }

    magic = (int *) pMem;
    *magic = MAGIC_NUM;
    pMem += sizeof(*magic);

    return pMem;
*/
    return NULL;
}



/*
 * Free some memory.
 * Recycle it back onto the free list for blocks of that size.
 */
void soami( void *pMem, int sMem )
{
/*
    int iList;
    int *magic;

    pMem -= sizeof(*magic);
    magic = (int *) pMem;

    if (*magic != MAGIC_NUM) {
        //bug("Attempt to recycle invalid memory of size %d.",sMem);
        //bug((char*) pMem + sizeof(*magic),0);
        return;
    }

    *magic = 0;
    sMem += sizeof(*magic);

    for ( iList = 0; iList < MAX_MEM_LIST; iList++ )
        if ( sMem <= rgSizeList[iList] )
            break;

    if ( iList == MAX_MEM_LIST ) {
        bug( "Free_mem: size %d too large.", sMem );
        exit( 1 );
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
void *crappyold( int sMem )
{
/*
    static char *pMemPerm;
    static int iMemPerm;
    void *pMem;

    while ( sMem % sizeof(long) != 0 )
		sMem++;

    if ( sMem > MAX_PERM_BLOCK ) {
		bug( "Alloc_perm: %d too large.", sMem );
		exit( 1 );
    }

    if ( pMemPerm == NULL || iMemPerm + sMem > MAX_PERM_BLOCK ) {
		iMemPerm = 0;
		if ( ( pMemPerm = calloc( 1, MAX_PERM_BLOCK ) ) == NULL ) {
		    perror( "Alloc_perm" );
			exit( 1 );
		}
    }

    pMem        = pMemPerm + iMemPerm;
    iMemPerm   += sMem;
    nAllocPerm += 1;
    sAllocPerm += sMem;
    return pMem;
*/
	return NULL;
}



/*
 * Duplicate a string into dynamic memory.
 * Fread_strings are read-only and shared.
 */
char *iamnotereet( const char *str )
{
/*    char *str_new;

    if ( str[0] == '\0' )
		return &str_empty[0];

    if ( str >= string_space && str < top_string )
		return (char *) str;

    str_new = palloc_struct( strlen(str) + 1 );
    strcpy( str_new, str );
    return str_new;
*/
	return NULL;
}



void do_areas( CHAR_DATA *ch, char *argument )
{
	char buf[MAX_STRING_LENGTH];
	AREA_DATA *pArea;
	int iArea = 0;
	
	if(strcmp(argument,"") && (!is_number(argument) || (iArea = atoi(argument)) < 1 || iArea > 51))
		return send_to_char("The area's level range must be between 1 and 51.\n\r",ch);
	for(pArea = area_first; pArea; pArea = pArea->next)
	{
		if(pArea->area_type == ARE_UNOPENED || pArea->area_type == ARE_CABAL || pArea->area_type == ARE_SHRINE)
			continue;
		if(iArea && (pArea->low_range>iArea || pArea->high_range<iArea))
			continue;
		if (pArea->low_range == 1 && pArea->high_range == 51)
			sprintf(buf,"[  ALL  ] %-20s - %s\n\r",pArea->credits, pArea->name);
		else
			sprintf(buf,"[%2d - %2d] %-20s - %s\n\r", pArea->low_range, pArea->high_range, pArea->credits, pArea->name);
		send_to_char( buf, ch );
	}
}



void do_memory( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    sprintf( buf, "Affects %5d\n\r", top_affect    ); send_to_char( buf, ch );
    sprintf( buf, "Areas   %5d\n\r", top_area      ); send_to_char( buf, ch );
    sprintf( buf, "ExDes   %5d\n\r", top_ed        ); send_to_char( buf, ch );
    sprintf( buf, "Exits   %5d\n\r", top_exit      ); send_to_char( buf, ch );
    sprintf( buf, "Helps   %5d\n\r", top_help      ); send_to_char( buf, ch );
    sprintf( buf, "Socials %5d\n\r", social_count  ); send_to_char( buf, ch );
    sprintf( buf, "Mobs    %5d(%d new format)\n\r", top_mob_index,newmobs );
    send_to_char( buf, ch );
    sprintf( buf, "(in use)%5d\n\r", mobile_count  ); send_to_char( buf, ch );
    sprintf( buf, "Objs    %5d(%d new format)\n\r", top_obj_index,newobjs );
    send_to_char( buf, ch );
    sprintf( buf, "Resets  %5d\n\r", top_reset     ); send_to_char( buf, ch );
    sprintf( buf, "Rooms   %5d\n\r", top_room      ); send_to_char( buf, ch );
    sprintf( buf, "Shops   %5d\n\r", top_shop      ); send_to_char( buf, ch );

    sprintf( buf, "Strings %5d strings of %7d bytes.\n\r",
	nAllocString, sAllocString);
    send_to_char( buf, ch );

    sprintf( buf, "Perms   %5d blocks  of %7d bytes.\n\r",
	nAllocPerm, sAllocPerm );
    send_to_char( buf, ch );

    return;
}

void do_dump( CHAR_DATA *ch, char *argument )
{
    int count,count2,num_pcs,aff_count;
    CHAR_DATA *fch;
    MOB_INDEX_DATA *pMobIndex;
    PC_DATA *pc;
    OBJ_DATA *obj;
    OBJ_INDEX_DATA *pObjIndex;
    ROOM_INDEX_DATA *room;
    EXIT_DATA *exit;
    DESCRIPTOR_DATA *d;
    AFFECT_DATA *af;
	OBJ_AFFECT_DATA *oaf;
    FILE *fp;
    int vnum,nMatch = 0;

    fclose(fpReserve);
    fp = fopen("mem.dmp","w");

    /* report use of data structures */

    num_pcs = 0;
    aff_count = 0;

    /* mobile prototypes */
    fprintf(fp,"MobProt	%4d (%8d bytes)\n",
	top_mob_index, top_mob_index * (sizeof(*pMobIndex)));

    /* mobs */
    count = 0;  count2 = 0;
    for (fch = char_list; fch != NULL; fch = fch->next)
    {
	count++;
	if (fch->pcdata != NULL)
	    num_pcs++;
	for (af = fch->affected; af != NULL; af = af->next)
	    aff_count++;
    }
    for (fch = char_free; fch != NULL; fch = fch->next)
	count2++;

    fprintf(fp,"Mobs	%4d (%8d bytes), %2d free (%d bytes)\n",
	count, count * (sizeof(*fch)), count2, count2 * (sizeof(*fch)));

    /* pcdata */
    count = 0;
    for (pc = pcdata_free; pc != NULL; pc = pc->next)
	count++;

    fprintf(fp,"Pcdata	%4d (%8d bytes), %2d free (%d bytes)\n",
	num_pcs, num_pcs * (sizeof(*pc)), count, count * (sizeof(*pc)));

    /* descriptors */
    count = 0; count2 = 0;
    for (d = descriptor_list; d != NULL; d = d->next)
	count++;
    for (d= descriptor_free; d != NULL; d = d->next)
	count2++;

    fprintf(fp, "Descs	%4d (%8d bytes), %2d free (%d bytes)\n",
	count, count * (sizeof(*d)), count2, count2 * (sizeof(*d)));

    /* object prototypes */
    for ( vnum = 0; nMatch < top_obj_index; vnum++ )
        if ( ( pObjIndex = get_obj_index( vnum ) ) != NULL )
        {
	    for (af = pObjIndex->affected; af != NULL; af = af->next)
		aff_count++;
            nMatch++;
        }

    fprintf(fp,"ObjProt	%4d (%8d bytes)\n",
	top_obj_index, top_obj_index * (sizeof(*pObjIndex)));


    /* objects */
    count = 0;  count2 = 0;
    for (obj = object_list; obj != NULL; obj = obj->next)
    {
	count++;
	for (oaf = obj->affected; oaf != NULL; oaf = oaf->next)
	    aff_count++;
    }
    for (obj = obj_free; obj != NULL; obj = obj->next)
	count2++;

    fprintf(fp,"Objs	%4d (%8d bytes), %2d free (%d bytes)\n",
	count, count * (sizeof(*obj)), count2, count2 * (sizeof(*obj)));

    /* affects */
    count = 0;
    for (af = affect_free; af != NULL; af = af->next)
	count++;

    fprintf(fp,"Affects	%4d (%8d bytes), %2d free (%d bytes)\n",
	aff_count, aff_count * (sizeof(*af)), count, count * (sizeof(*af)));

    /* rooms */
    fprintf(fp,"Rooms	%4d (%8d bytes)\n",
	top_room, top_room * (sizeof(*room)));

     /* exits */
    fprintf(fp,"Exits	%4d (%8d bytes)\n",
	top_exit, top_exit * (sizeof(*exit)));

    fclose(fp);

    /* start printing out mobile data */
    fp = fopen("mob.dmp","w");

    fprintf(fp,"\nMobile Analysis\n");
    fprintf(fp,  "---------------\n");
    nMatch = 0;
    for (vnum = 0; nMatch < top_mob_index; vnum++)
	if ((pMobIndex = get_mob_index(vnum)) != NULL)
	{
	    nMatch++;
	    fprintf(fp,"#%-4d %3d active %3d killed     %s\n",
		pMobIndex->vnum,pMobIndex->count,
		pMobIndex->killed,pMobIndex->short_descr);
	}
    fclose(fp);

    /* start printing out object data */
    fp = fopen("obj.dmp","w");

    fprintf(fp,"\nObject Analysis\n");
    fprintf(fp,  "---------------\n");
    nMatch = 0;
    for (vnum = 0; nMatch < top_obj_index; vnum++)
	if ((pObjIndex = get_obj_index(vnum)) != NULL)
	{
	    nMatch++;
	    fprintf(fp,"#%-4d %3d active %3d reset      %s\n",
		pObjIndex->vnum,pObjIndex->count,
		pObjIndex->reset_num,pObjIndex->short_descr);
	}

    /* close file */
    fclose(fp);
    fpReserve = fopen( NULL_FILE, "r" );
}



/*
 * Stick a little fuzz on a number.
 */
int number_fuzzy( int number )
{
    switch ( number_bits( 2 ) )
    {
    case 0:  number -= 1; break;
    case 3:  number += 1; break;
    }

    return UMAX( 1, number );
}

/*
 * Generate a random number.
 */
int number_range( int from, int to )
{
    int power;
    int number;

    if (from == 0 && to == 0)
	return 0;

    if ( ( to = to - from + 1 ) <= 1 )
	return from;

    for ( power = 2; power < to; power <<= 1 )
	;

    while ( ( number = number_mm() & (power -1 ) ) >= to )
	;

    return from + number;
}



/*
 * Generate a percentile roll.
 */
int number_percent( void )
{
    int percent;

    while ( (percent = number_mm() & (128-1) ) > 99 )
	;

    return 1 + percent;
}



/*
 * Generate a random door.
 */
int number_door( void )
{
    int door;

    while ( ( door = number_mm() & (8-1) ) > 5)
	;

    return door;
}

int number_bits( int width )
{
    return number_mm( ) & ( ( 1 << width ) - 1 );
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

#if defined (OLD_RAND)
static  int     rgiState[2+55];
#endif

void init_mm( )
{
#if defined (OLD_RAND)
    int *piState;
    int iState;

    piState     = &rgiState[2];

    piState[-2] = 55 - 55;
    piState[-1] = 55 - 24;

    piState[0]  = ((int) current_time) & ((1 << 30) - 1);
    piState[1]  = 1;
    for ( iState = 2; iState < 55; iState++ )
    {
        piState[iState] = (piState[iState-1] + piState[iState-2])
        & ((1 << 30) - 1);
    }
#else
    srandom(time(NULL)^getpid());
#endif
    return;
}



long number_mm( void )
{
#if defined (OLD_RAND)
    int *piState;
    int iState1;
    int iState2;
    int iRand;

    piState             = &rgiState[2];
    iState1             = piState[-2];
    iState2             = piState[-1];
    iRand               = (piState[iState1] + piState[iState2])
        & ((1 << 30) - 1);
    piState[iState1]    = iRand;
    if ( ++iState1 == 55 )
        iState1 = 0;
    if ( ++iState2 == 55 )
        iState2 = 0;
    piState[-2]         = iState1;
    piState[-1]         = iState2;
    return iRand >> 6;
#else
    return random() >> 6;
#endif
}


/*
 * Roll some dice.
 */
int dice( int number, int size )
{
    int idice;
    int sum;

    switch ( size )
    {
    case 0: return 0;
    case 1: return number;
    }

    for ( idice = 0, sum = 0; idice < number; idice++ )
	sum += number_range( 1, size );

    return sum;
}



/*
 * Simple linear interpolation.
 */
int interpolate( int level, int value_00, int value_32 )
{
    return value_00 + level * (value_32 - value_00) / 32;
}



/*
 * Removes the tildes from a string.
 * Used for player-entered strings that go into disk files.
 */
void smash_tilde( char *str )
{
    for ( ; *str != '\0'; str++ )
    {
	if ( *str == '~' )
	    *str = '-';
    }

    return;
}



/*
 * Compare strings, case insensitive.
 * Return TRUE if different
 *   (compatibility with historical functions).
 */
bool str_cmp( const char *astr, const char *bstr )
{

    if ( astr == NULL )
    {
//	sprintf(buf,"Str_cmp: null astr, bstr: %s (%s:%d dev=pimp)",gLastFile, gLastLine, bstr);
//	bug( buf, 0 );
	return TRUE;
    }

    if ( bstr == NULL )
    {
//	sprintf(buf,"Str_cmp: null bstr, astr: %s",astr);
//	bug( buf, 0 );
	return TRUE;
    }

    for ( ; *astr || *bstr; astr++, bstr++ )
    {
	if ( LOWER(*astr) != LOWER(*bstr) )
	    return TRUE;
    }

    return FALSE;
}



/*
 * Compare strings, case insensitive, for prefix matching.
 * Return TRUE if astr not a prefix of bstr
 *   (compatibility with historical functions).
 */
bool str_prefix( const char *astr, const char *bstr )
{
    if ( astr == NULL )
    {
	bug( "Strn_cmp: null astr.", 0 );
	return TRUE;
    }

    if ( bstr == NULL )
    {
	bug( "Strn_cmp: null bstr.", 0 );
	return TRUE;
    }

    for ( ; *astr; astr++, bstr++ )
    {
	if ( LOWER(*astr) != LOWER(*bstr) )
	    return TRUE;
    }

    return FALSE;
}



/*
 * Compare strings, case insensitive, for match anywhere.
 * Returns TRUE is astr not part of bstr.
 *   (compatibility with historical functions).
 */
bool str_infix( const char *astr, const char *bstr )
{
    int sstr1;
    int sstr2;
    int ichar;
    char c0;

    if ( ( c0 = LOWER(astr[0]) ) == '\0' )
	return FALSE;

    sstr1 = strlen(astr);
    sstr2 = strlen(bstr);

    for ( ichar = 0; ichar <= sstr2 - sstr1; ichar++ )
    {
	if ( c0 == LOWER(bstr[ichar]) && !str_prefix( astr, bstr + ichar ) )
	    return FALSE;
    }

    return TRUE;
}



/*
 * Compare strings, case insensitive, for suffix matching.
 * Return TRUE if astr not a suffix of bstr
 *   (compatibility with historical functions).
 */
bool str_suffix( const char *astr, const char *bstr )
{
    int sstr1;
    int sstr2;

    sstr1 = strlen(astr);
    sstr2 = strlen(bstr);
    if ( sstr1 <= sstr2 && !str_cmp( astr, bstr + sstr2 - sstr1 ) )
	return FALSE;
    else
	return TRUE;
}



/*
 * Returns an initial-capped string.
 */
char *capitalize( const char *str )
{
    static char strcap[MAX_STRING_LENGTH];
    int i;

    for ( i = 0; str[i] != '\0'; i++ )
	strcap[i] = LOWER(str[i]);
    strcap[i] = '\0';
    strcap[0] = UPPER(strcap[0]);
    return strcap;
}


/*
 * Append a string to a file.
 */
void append_file( CHAR_DATA *ch, char *file, char *str )
{
    FILE *fp;
	char buf[MSL];

    if ( IS_NPC(ch) || str[0] == '\0' )
	return;

    fclose( fpReserve );
    if ( ( fp = fopen( file, "a" ) ) == NULL )
    {
	perror( file );
	send_to_char( "Could not open the file!\n\r", ch );
    }
    else
    {
	//free_pstring(buf);
	strcpy(buf,ctime(&current_time));
	chop(buf);
	fprintf( fp, "[%5d] %s: %s: %s\n",
	    ch->in_room ? ch->in_room->vnum : 0, ch->true_name, buf, str );
	fclose( fp );
    }

    fpReserve = fopen( NULL_FILE, "r" );
    return;
}



/*
 * Reports a bug.
 */
void bug( const char *str, int param )
{
    char buf[MAX_STRING_LENGTH];

    if ( fpArea != NULL )
    {
	int iLine;
	int iChar;

	if ( fpArea == stdin )
	{
	    iLine = 0;
	}
	else
	{
	    iChar = ftell( fpArea );
	    fseek( fpArea, 0, 0 );
	    for ( iLine = 0; ftell( fpArea ) < iChar; iLine++ )
	    {
		while ( getc( fpArea ) != '\n' )
		    ;
	    }
	    fseek( fpArea, iChar, 0 );
	}

	sprintf( buf, "[*****] FILE: %s LINE: %d", strArea, iLine );
	log_string( buf );
    }

    strcpy( buf, "[*****] BUG: " );
    sprintf( buf + strlen(buf), str, param );
    log_string( buf );
    wiznet(buf,0,0,WIZ_DEBUG,0,0);
    return;
}

/*
 * Writes a string to the log.
 */
void log_string( const char *str )
{
    char *strtime;

    strtime    = ctime( &current_time );
    strtime[strlen(strtime)-1] = '\0';
    fprintf( stderr, "%s :: %s\n", strtime, str );
    return;
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
void tail_chain( void )
{
    return;
}


void do_force_reset(CHAR_DATA *ch,char *argument)
{
    AREA_DATA *pArea;
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *ich, *ich_next;
    if(get_trust(ch) < LEVEL_IMMORTAL && !IS_HEROIMM(ch))
	return send_to_char("Huh?\n\r",ch);

    if(get_trust(ch) < 56 && !IS_BUILDER(ch, ch->in_room->area))
	return send_to_char("You don't have access to reset this area.\n\r",ch);

    pArea = ch->in_room->area;
    for(ich = char_list; ich; ich = ich_next)
    {
	ich_next = ich->next;
	if(IS_NPC(ich) && ich->in_room && ich->in_room->area == pArea && pArea->area_type == ARE_UNOPENED)
		extract_char(ich, TRUE);
    }
    reset_area(pArea);
    pArea->age = number_range(0,3);
    sprintf(buf,"Area: %s reset.\n\r",pArea->name);
    send_to_char(buf,ch);
    sprintf(buf,"Area: %s reset by %s.\n\r",pArea->name,ch->name);

    return;
}


/* void do_alist(CHAR_DATA *ch,char *argument)
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
	for(pArea = area_first; pArea != NULL; pArea = pArea->next)
		if(pArea->min_vnum<=i && pArea->max_vnum>=i)
    			sprintf(buf,"%-25s: %4d %4d\n\r",pArea->name,pArea->min_vnum,pArea->max_vnum);
	if(buf[0]!='\0')
	{
		sprintf(buf2,"The following area contains vnum %i:\n\r%s",i,buf);
		return send_to_char(buf2,ch);
	}
	else
		return send_to_char("No areas containing that vnum were found.\n\r",ch);
    }
    for (pArea = area_first; pArea != NULL; pArea = pArea->next)
    {
    i++;
    sprintf(buf,"%-25s: %4d %4d",pArea->name,pArea->min_vnum,pArea->max_vnum);
    list[i] = palloc_string(buf);
    avnum[i] = pArea->min_vnum;
    }
    for(a = 1; a<i-1; a++)
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
    for(x = 1; x<=i; x = x+2)
    {
	sprintf(buf,"%-40s %s\n\r",list[x],x+1<=i ? list[x+1] : "");
	send_to_char(buf,ch);
    }
    send_to_char("Use alist <vnum> to see what area occupies the given vnum.\n\r",ch);
}
*/


void do_llimit(CHAR_DATA *ch, char *argument)
    {
	FILE *fpChar_list;
	FILE *fpChar;
	char strPlr[MAX_INPUT_LENGTH];
	char catplr[MAX_INPUT_LENGTH];
	char chkbuf[MAX_STRING_LENGTH];
	char pbuf[100];
  	OBJ_INDEX_DATA *lObjIndex;
	OBJ_DATA *obj;
	int i;
	CHAR_DATA *carrier;

/* reset limit counts and recount items in game , do NOT count items on
players loaded */
	send_to_char("Reseting all counts to zero now.\n\r",ch);
	for (i = 0; i < 30000; i++)
	{
	lObjIndex = get_obj_index(i);
	if (lObjIndex == NULL)
		continue;
	lObjIndex->limcount = 0;
	}
	send_to_char("Counts set to zero....\n\r",ch);
	send_to_char("Loading all in-game counts now (excluding PC objects).\n\r",ch);
    	for (obj = object_list; obj != NULL; obj = obj->next)
	{
	carrier = obj->carried_by;
	if (carrier != NULL && !IS_NPC(carrier))
		continue;
	obj->pIndexData->limcount++;
	}
	send_to_char("Loading all pfile object counts now.\n\r",ch);

	sprintf(catplr,"%s%s",PLAYER_DIR,"*.plr");
	sprintf(pbuf,"ls %s > %s",catplr,PLAYER_LIST);
	system(pbuf);

	if ((fpChar_list = fopen( PLAYER_LIST, "r")) == NULL)
	{
	perror(PLAYER_LIST);
	exit(1);
	}

	for (; ;)
	{
	 strcpy(strPlr, fread_word(fpChar_list) );
//     log_string(strPlr);
 	 sprintf(chkbuf,"%s%s",PLAYER_DIR,"Zzz.plr");
	 if (!str_cmp(strPlr,"../player/Zzz.plr"))
		break;	/* Exit if == Zzz.plr file */		

	 if ( (	fpChar = fopen(strPlr, "r") ) == NULL)
	 {
		perror(strPlr);
		exit(1);
	 }

	for (; ;)
	{
	int vnum;
	char letter;
	char *word;
	OBJ_INDEX_DATA *pObjIndex;

	letter = fread_letter(fpChar);
	if (letter != '#')
		continue;
	word = fread_word(fpChar);

	if (!str_cmp(word,"End"))
		break;

	if ( !str_cmp( word, "O") || !str_cmp( word, "OBJECT"))
		{
		word = fread_word(fpChar);
		if (!str_cmp(word, "Vnum"))
			{
			vnum = fread_number(fpChar);
			if ( (get_obj_index(vnum)) == NULL)
			{
		/*	bug("Bad obj vnum in limits: %d",vnum); */
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
	fpChar = NULL;
	}

	fclose( fpChar_list);
	send_to_char("All object counts reset.\n\r",ch);
	return;
}			

/*
 * Snarf a room section.
 */
void load_rooms( FILE *fp )
{
    ROOM_INDEX_DATA *pRoomIndex;
    char error_v[MAX_STRING_LENGTH];
	int i;

    if ( area_last == NULL )
        bugout( "Load_resets: no #AREA seen yet.");

    for ( ; ; )
    {
        sh_int vnum=0;
        char letter, str[20];
	char *bword;
	long bitvect;
        int door;
        int iHash;

        letter          = fread_letter( fp );
        if ( letter != '#' )
	{
	    sprintf(str,"Letter is %c.",letter);
	    bug(str,0);
            bug( "Load_newrooms: # not found, last vnum %d.", vnum );
	    exit(0);
	}


        vnum            = fread_number( fp );
        if ( vnum == 0 )
            break;

        fBootDb = FALSE;
        if ( get_room_index( vnum ) != NULL )
	{
	    sprintf(error_v,"Load_rooms: vnum %d duplicated.", vnum);
            bugout( error_v );
	}
        fBootDb = TRUE;

        pRoomIndex       = new ROOM_INDEX_DATA;
		pRoomIndex->reset_first			= NULL;
		pRoomIndex->reset_last			= NULL;
        pRoomIndex->owner               = palloc_string("");
		pRoomIndex->move_progs			= FALSE;
        pRoomIndex->people              = NULL;
        pRoomIndex->contents            = NULL;
        pRoomIndex->extra_descr         = NULL;
	pRoomIndex->alt_description	= NULL;
	pRoomIndex->alt_description_cond = 0;
	pRoomIndex->alt_name		= NULL;
	pRoomIndex->trap				= NULL;
	pRoomIndex->affected			= NULL;
	zero_vector(pRoomIndex->room_flags);
	for (i=0;i<MAX_TRACKS;i++)
		pRoomIndex->tracks[i] = new_track_data();
        pRoomIndex->area                = area_last;
        pRoomIndex->vnum                = vnum;
        pRoomIndex->name                = fread_string( fp );
		if (pRoomIndex->name[strlen(pRoomIndex->name) - 1] == '.')
			chop(pRoomIndex->name);
        pRoomIndex->description         = fread_string( fp );
        pRoomIndex->sector_type         = sect_table[sect_lookup(fread_word( fp ))].value;
		
		/* Morg - Valgrind fix. */
		pRoomIndex->rprogs				= NULL;
		zero_vector(pRoomIndex->progtypes);
		pRoomIndex->cabal				= 0;
		pRoomIndex->has_rune			= FALSE;

		zero_vector(pRoomIndex->affected_by);
		if(pRoomIndex->area->area_type == ARE_SHRINE) {
			SET_BIT(pRoomIndex->room_flags, ROOM_NO_SUMMON_TO);
			SET_BIT(pRoomIndex->room_flags, ROOM_NO_SUMMON_FROM);
			SET_BIT(pRoomIndex->room_flags, ROOM_NO_GATE);
		}

        for ( door = 0; door <= 5; door++ )
            pRoomIndex->exit[door] = NULL;

        /* defaults */
        pRoomIndex->heal_rate = 100;
        pRoomIndex->mana_rate = 100;
		pRoomIndex->light = 0;

        for ( ; ; )
        {
            letter = fread_letter( fp );
	    letter = UPPER(letter);
            if ( letter == 'S' )
                break;

            if ( letter == 'H') /* healing room */
                pRoomIndex->heal_rate = fread_number(fp);

            else if ( letter == 'M') /* mana room */
                pRoomIndex->mana_rate = fread_number(fp);

            else if ( letter == 'G') /* Guilds...added by Ceran */
                pRoomIndex->guild = fread_number(fp);

	    else if ( letter == 'A') //ALTDESC
	    {
		fread_word(fp);
		pRoomIndex->alt_description_cond = fread_number(fp);
		pRoomIndex->alt_name		= fread_string(fp);
		pRoomIndex->alt_description = fread_string(fp);
	    }

           else if ( letter == 'C') /* Cabal */
           {
		fread_word(fp);
                if (pRoomIndex->cabal)
    			bugout("Load_rooms: duplicate cabal fields.");
                pRoomIndex->cabal = cabal_lookup(fread_word(fp));
            }


            else if ( letter == 'D' )
            {
                EXIT_DATA *pexit;

                door = flag_lookup(fread_word( fp ),direction_table);
                if ( door < 0 || door > 5 )
    			{ bug( "Fread_rooms: vnum %d has bad door number.", vnum ); exit(1); }

                pexit    = new EXIT_DATA;
                pexit->u1.vnum          = fread_number( fp );
                fread_flag_new(pexit->exit_info, fp);
                pexit->key              = fread_number( fp );
                pexit->keyword          = fread_string( fp );
                pexit->description      = fread_string( fp );

                pRoomIndex->exit[door]  = pexit;
                top_exit++;
            }
            else if ( letter == 'E' )
            {
                EXTRA_DESCR_DATA *ed;

                ed       = new EXTRA_DESCR_DATA;
                ed->keyword             = fread_string( fp );
                ed->description         = fread_string( fp );
                ed->next                = pRoomIndex->extra_descr;
                pRoomIndex->extra_descr = ed;
                top_ed++;
            } else if ( letter == 'T' )
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
				trap->armed = TRUE;
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
		bword	= fread_word(fp);
		bitvect = flag_lookup(bword,room_flags);
		if (bitvect == NO_FLAG && str_cmp(bword,"dark"))
			bugout("Invalid bitvector specified for room flags.");
		SET_BIT(pRoomIndex->room_flags,	bitvect);
	    }
            else
	    {
		ungetc(letter,fp);
		break;
	    }

        }
        iHash   = vnum % MAX_KEY_HASH;
        pRoomIndex->next        = room_index_hash[iHash];
        room_index_hash[iHash]  = pRoomIndex;
        pRoomIndex->next_room   = room_list;
        room_list               = pRoomIndex;
        top_room++;
    }

    return;
}

void load_votes()
{
	FILE *fp;
	BALLOT_DATA *ballot;
	VOTE_DATA *vote;
	char discard, *word;
	if(!(fp=fopen(VOTE_FILE,"r")))
		return;
	return;
	vote_first	= NULL;
	ballot_first	= NULL;
	for(;;)
	{
	//Discard the first '#'
		discard		= fread_letter(fp);
		if(discard=='$')
			break;
        	ballot  	 = new BALLOT_DATA;
		ballot->name	= fread_string(fp);
		ballot->next	= ballot_first;
		for( ;; )
		{
			//votez
			word		= fread_word(fp);
			if(!str_cmp(word,"ENDVOTES"))
				break;
        	vote    	 = new VOTE_DATA;
			vote->voter 	= palloc_string(word);
			vote->vote_for	= palloc_string(fread_word(fp));
			vote->time	= fread_string(fp);
			vote->ip	= fread_string(fp);
			vote->next	= vote_first;
			vote_first	= vote;
			ballot->first_vote = vote;
		}
		ballot_first	= ballot;
	}
	fclose(fp);
}

void load_newresets( FILE *fp )
{
    RESET_DATA *pReset;
	int iLastRoom = 0;
    int iLastObj  = 0;

	if ( area_last == NULL )
    {
	bug( "Load_resets: no #AREA seen yet.", 0 );
	exit( 1 );
    }

    for ( ; ; )
    {
	ROOM_INDEX_DATA *pRoomIndex=NULL;
	char letter, tletter;
	OBJ_INDEX_DATA *temp_index;
	char *word;
	if ( ( letter = fread_letter( fp ) ) == 'S' )
	    break;

	if ( letter == '*' )
	{
	    fread_to_eol( fp );
	    continue;
	}

	pReset		 = new RESET_DATA;
	pReset->arg1 = 0;
	pReset->arg2 = 0;
	pReset->arg3 = 0;
	pReset->arg4 = 0;
	pReset->command	= letter;
	fread_word(fp);  //get rid of rest of word, i.e. OB for MOB
	/*
	 * Validate parameters.
	 * We're calling the index functions for the side effect.
	 */
	switch ( letter )
	{
	default:
	    bug( "Load_resets: bad command '%c'.", letter );
	    exit( 1 );
	    break;

	case 'F':
	    pReset->arg1		= fread_number(fp);	//leader vnum
	    pReset->arg2		= fread_number(fp);	//leader mob number
		pReset->arg3		= 0; 
	    pReset->arg4		= 0;
		pRoomIndex = get_room_index( iLastRoom );
	    new_reset( pRoomIndex , pReset );
	    break;

	case 'M':
		pReset->arg1	= fread_number( fp );  	//mob vnum
		fread_word( fp ); 				//the word TO
		pReset->arg3	= fread_number( fp ); 	//to room vnum
		fread_word( fp ); 				//the word GLIMIT
		pReset->arg2	= fread_number( fp ); 	// global limit
		fread_word( fp ); 			    	//the word LLIMIT
		pReset->arg4	= fread_number( fp ); 	// local limit
		if ( ( pRoomIndex = get_room_index ( pReset->arg3 ) ) )
		{
			new_reset( pRoomIndex, pReset );
			iLastRoom = pReset->arg3;
		}
		if(area_last->area_type == ARE_UNOPENED)  //debugging for unopened areas
	    {
	    	get_mob_index  ( pReset->arg1 );
	    	get_room_index ( pReset->arg3 );
		}
	    break;

	case 'O':
	    pReset->arg1		= fread_number(fp);	//object vnum
	    pReset->arg2		= 0;				//crappy placeholder
	    fread_word(fp);					//get rid of the word TO
	    pReset->arg3		= fread_number(fp); //room vnum
		iLastRoom = pReset->arg3;
	    pReset->arg4		= 0;				//crappy placeholder
	    temp_index = get_obj_index  ( pReset->arg1 );
	    temp_index->reset_num++;
		if ( ( pRoomIndex = get_room_index ( pReset->arg3 ) ) )
		{
			new_reset( pRoomIndex, pReset );
			iLastObj = pReset->arg3;
		}
		if(area_last->area_type == ARE_UNOPENED)
	    	get_room_index ( pReset->arg3 );
	    break;

	case 'P':
	    pReset->arg1		= fread_number(fp);	//object being put in container
	    pReset->arg2		= -1;				//crappy placeholder
	    fread_word(fp);					//disregard IN
	    pReset->arg3		= fread_number(fp);	//container vnum
	    fread_word(fp);					//disregard COUNT
	    pReset->arg4		= fread_number(fp);	//number of copies to put in cont.
	    temp_index = get_obj_index  ( pReset->arg1 );
	    temp_index->reset_num++;
		if ( ( pRoomIndex = get_room_index ( iLastRoom ) ) )
			new_reset( pRoomIndex, pReset );
		if(area_last->area_type == ARE_UNOPENED)
	    	get_obj_index  ( pReset->arg3 );
	    break;

	case 'G':
	    pReset->arg1		= fread_number(fp);	//object to give
	    pReset->arg2		= 0;
	    pReset->arg3		= 0;
	    pReset->arg4		= 0;
	    temp_index = get_obj_index  ( pReset->arg1 );
	    temp_index->reset_num++;
		if ( ( pRoomIndex = get_room_index ( iLastRoom ) ) )
		{
			new_reset( pRoomIndex, pReset );
			iLastObj = iLastRoom;
		}
		break;

	case 'E':
	    pReset->arg1		= fread_number(fp);	//object to give
	    pReset->arg2		= 0;
	    word			= talloc_string(fread_word(fp));
	    pReset->arg3	= flag_lookup(word,wear_locations);  //location of obj
		if (pReset->arg3 == NO_FLAG)
			bugout("Invalid wear location in resets.");
	    pReset->arg4		= 0;
	    temp_index = get_obj_index  ( pReset->arg1 );
	    temp_index->reset_num++;
		if ( ( pRoomIndex = get_room_index ( iLastRoom ) ) )
		{
			new_reset( pRoomIndex, pReset );
			iLastObj = iLastRoom;
		}
		break;

	case 'D':
	    pReset->arg1		= fread_number(fp);	//room vnum
	    pReset->arg2		= flag_lookup(fread_word( fp ),direction_table); //direction
		if (pReset->arg2 == NO_FLAG)
			bugout("Invalid direction in door reset.");
	    tletter			= fread_letter(fp);
	    tletter			= UPPER(tletter);
	    if(tletter == 'O')
		pReset->arg3	= 0;
	    else if(tletter == 'C')
		pReset->arg3	= 1;
	    else if(tletter == 'L')
		pReset->arg3	= 2;
	    else
		bugout("Invalid door reset state.");
	    pReset->arg4		= 0;
		if ( ( pRoomIndex = get_room_index ( pReset->arg1 ) ) )
			new_reset( pRoomIndex, pReset);
		break;

	case 'R':
	    pReset->arg1	= fread_number(fp);
	    pReset->arg2	= fread_number(fp);
	    pReset->arg3	= 0;
	    pReset->arg4	= 0;
		if ( ( pRoomIndex = get_room_index ( pReset->arg1 ) ) )
			new_reset( pRoomIndex, pReset );
		if ( pReset->arg2 < 0 || pReset->arg2 > 6 )
	    {
		bug( "Load_resets: 'R': bad exit %d.", pReset->arg2 );
		exit( 1 );
	    }
	    break;
	}
	fread_to_eol( fp );

	pReset->next		= NULL;
	top_reset++;
    }

    return;
}

void clean_notes(void)
{
	char query[MSL];
	//if the difference is over 14 days, delete it.
	sprintf(query,"DELETE FROM notes WHERE %ld-timestamp>1209600",current_time);
	one_query(query);
}

void load_race_info(void)
{
	int i, j;

	race_list = new_race_data();

	race_list->first = race_list;

	for (i = 0; race_table[i].name != NULL; i++) {
		race_list->name = palloc_string(race_table[i].name);
		race_list->pc_race = race_table[i].pc_race;
		race_list->number = i;
		for (j = 0; j < 10; j++)
			if (race_table[i].act[j] != NO_FLAG)
				SET_BIT(race_list->act, race_table[i].act[j]);
		for (j = 0; j < 10; j++)
			if (race_table[i].aff[j] != NO_FLAG)
				SET_BIT(race_list->aff, race_table[i].aff[j]);
		for (j = 0; j < 10; j++)
			if (race_table[i].off[j] != NO_FLAG)
				SET_BIT(race_list->off, race_table[i].off[j]);
		for (j = 0; j < 10; j++)
			if (race_table[i].imm[j] != NO_FLAG)
				SET_BIT(race_list->imm, race_table[i].imm[j]);
		for (j = 0; j < 10; j++)
			if (race_table[i].res[j] != NO_FLAG)
				SET_BIT(race_list->res, race_table[i].res[j]);
		for (j = 0; j < 10; j++)
			if (race_table[i].vuln[j] != NO_FLAG)
				SET_BIT(race_list->vuln, race_table[i].vuln[j]);
		for (j = 0; j < 15; j++)
			if (race_table[i].form[j] != NO_FLAG)
				SET_BIT(race_list->form, race_table[i].form[j]);
		for (j = 0; j < 15; j++)
			if (race_table[i].parts[j] != NO_FLAG)
				SET_BIT(race_list->parts, race_table[i].parts[j]);
		race_list->arms = race_table[i].arms;
		race_list->legs = race_table[i].legs;

		race_list->next = new_race_data();
		race_list->next->first = race_list->first;
	
		race_list = race_list->next;
	}

	race_list->next = NULL;

	race_list = race_list->first;

	return;
}

char intflag_to_char(int i) {
    if (i >= 0 && i <= 25)
        return (char)((int)('A') + i);
    else
		return (char)((int)('a') + i - 26);
}

char * bitvector_to_flag(long bitvect[]) {
    int i, ilen = 0;
    static char buf[64];
    char *a = buf;

	if (IS_ZERO_VECTOR(bitvect))
		return("0");

	for(i = 0; i < MAX_BIT; i++) {
		if (IS_SET(bitvect, i)) {
			ilen++;
			*a++ = intflag_to_char(i);
		}
	}

    if (ilen == 0)
		*a++ = '0';

    *a = '\0';
    return buf;
}
