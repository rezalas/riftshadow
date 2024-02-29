/****************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,         *
 *  Michael Seifert, Hans Henrik Stærfeldt, Tom Madsen, and Katja Nyboe.    *
 *                                                                          *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael           *
 *  Chastain, Michael Quan, and Mitchell Tse.                               *
 *                                                                          *
 *  In order to use any part of this Merc Diku Mud, you must comply with    *
 *  both the original Diku license in 'license.doc' as well the Merc        *
 *  license in 'license.txt'.  In particular, you may not remove either of  *
 *  these copyright notices.                                                *
 *                                                                          *
 *  Much time and thought has gone into this software and you are           *
 *  benefitting.  We hope that you share your changes too.  What goes       *
 *  around, comes around.                                                   *
 ****************************************************************************/
 
/****************************************************************************
 *  ROM 2.4 is copyright 1993-1996 Russ Taylor                              *
 *  ROM has been brought to you by the ROM consortium                       *
 *      Russ Taylor (rtaylor@efn.org)                                       *
 *      Gabrielle Taylor (gtaylor@pacinfo.com)                              *
 *      Brian Moore (zump@rom.org)                                          *
 *  By using this code, you have agreed to follow the terms of the          *
 *  ROM license, in the file Tartarus/doc/rom.license                       *
 ****************************************************************************/

/****************************************************************************
 *       Tartarus code is copyright (C) 1997-1998 by Daniel Graham          *
 *  In using this code you agree to comply with the Tartarus license        *
 *       found in the file /Tartarus/doc/tartarus.doc                       *
 ****************************************************************************/

#ifndef DB_H
#define DB_H

#include <cstddef>
#include "merc.h"

#ifndef OLD_RAND
#define OLD_RAND
	void srandom(unsigned int);
	int getpid();
	time_t time(time_t *tloc);
#endif

extern char *top_bounty_name[MAX_TOP_BOUNTY];
extern long top_bounty_value[MAX_TOP_BOUNTY];

extern HELP_DATA *help_first;
extern HELP_DATA *help_last;
extern SHOP_DATA *shop_first;
extern SHOP_DATA *shop_last;
extern char bug_buf[];
extern char *help_greeting;
extern char log_buf[];
extern CHAR_DATA *char_list;
extern KILL_DATA kill_table[];
extern MOB_INDEX_DATA *mindex_list;
extern OBJ_INDEX_DATA *oIndex_list;
extern OBJ_DATA *object_list;
extern TIME_INFO_DATA time_info;
extern short sun;
extern short moon_berus;
extern short berus_pos;
extern short moon_calabren;
extern short calabren_pos;
extern ROOM_INDEX_DATA *room_list;
extern ROOM_INDEX_DATA *top_affected_room;
extern RUNE_DATA *rune_list;
extern long gold_constant;
extern long total_gold;
extern long player_gold;
extern long total_wealth;
extern RACE_DATA * race_list;

//
// These are skill_lookup return values for common skills and spells.
//

//
// GSNS
//

//TODO: UNKNOWN GSNS
//extern short gsn_battlecry;
//extern short gsn_empower;
//extern short gsn_power_word_fear;
//extern short gsn_steel_nerves;
//extern short gsn_bandage;
//extern short gsn_flurry;				// Quest flurry -Dioxide
//extern short gsn_drum;				// Quest drum -Dioxide
//extern short gsn_executioner;
//extern short gsn_phat_blunt;


extern short gsn_timer;
extern short gsn_repose;
extern short gsn_medicine;
extern short gsn_somatic_casting;
extern short gsn_frigidaura;
extern short gsn_stasis_wall;
extern short gsn_integrate;
extern short gsn_sensevibrations;
extern short gsn_diamondskin;
extern short gsn_hardenfist;
extern short gsn_jackal;
extern short gsn_mangled;
extern short gsn_rage;
extern short gsn_horde_communion;
extern short gsn_bleeding;
extern short gsn_calm;
extern short gsn_agitate;
extern short gsn_immolate;
extern short gsn_enervate;
extern short gsn_corona;
extern short gsn_infravision;
extern short gsn_faerie_fire;
extern short gsn_faerie_fog;
extern short gsn_backstab;
extern short gsn_detect_hidden;
extern short gsn_detect_movement;
extern short gsn_bluff;
extern short gsn_dodge;
extern short gsn_envenom;
extern short gsn_hide;
extern short gsn_peek;
extern short gsn_pick_lock;
extern short gsn_sneak;
extern short gsn_steal;
extern short gsn_silence;
extern short gsn_disarm;
extern short gsn_enhanced_damage;
extern short gsn_kick;
extern short gsn_parry;
extern short gsn_rescue;
extern short gsn_second_attack;
extern short gsn_third_attack;
extern short gsn_fourth_attack;

extern short gsn_divine_frenzy;
extern short gsn_awe;
extern short gsn_strike_of_virtue;

extern short gsn_decrepify;
extern short gsn_displacement;
extern short gsn_false_motives;

extern short gsn_blindness;
extern short gsn_chill_touch;
extern short gsn_burning_hands;
extern short gsn_charm_person;
extern short gsn_curse;
extern short gsn_invis;
extern short gsn_mass_invis;
extern short gsn_poison;
extern short gsn_plague;
extern short gsn_sleep;
extern short gsn_sanctuary;
extern short gsn_fly;

/* new gsn's */
extern short gsn_axe;
extern short gsn_dagger;
extern short gsn_flail;
extern short gsn_mace;
extern short gsn_polearm;
extern short gsn_shield_block;
extern short gsn_spear;
extern short gsn_sword;
extern short gsn_whip;

extern short gsn_bash;
extern short gsn_berserk;
extern short gsn_dirt;
extern short gsn_hand_to_hand;
extern short gsn_trip;

extern short gsn_fast_healing;
extern short gsn_haggle;
extern short gsn_lore;
extern short gsn_meditation;

extern short gsn_scrolls;
extern short gsn_staves;
extern short gsn_wands;
extern short gsn_recall;

extern short gsn_mimic;
extern short gsn_liquify;
extern short gsn_morph_weapon;

extern short gsn_ground_control;

extern short gsn_absorb;
extern short gsn_camouflage;
extern short gsn_acute_vision;
extern short gsn_ambush;
extern short gsn_laying_hands;
extern short gsn_circle;
extern short gsn_dual_backstab;
extern short gsn_skin;
extern short gsn_camp;
extern short gsn_animate_dead;
extern short gsn_barkskin;
extern short gsn_animal_call;
extern short gsn_aura_of_sustenance;
extern short gsn_shadowgate;
extern short gsn_eye_of_the_predator;
extern short gsn_blackjack;
extern short gsn_dual_wield;
extern short gsn_trance;
extern short gsn_wanted;
extern short gsn_cleave;
extern short gsn_herb;
extern short gsn_protective_shield;
extern short gsn_consecrate;
extern short gsn_true_sight;
extern short gsn_butcher;
extern short gsn_vanish;
extern short gsn_embalm;
extern short gsn_counter;
extern short gsn_acid_spit;
extern short gsn_bear_call;
extern short gsn_trophy;
extern short gsn_tail;
extern short gsn_spellbane;
extern short gsn_request;
extern short gsn_endure;
extern short gsn_throw;
extern short gsn_nerve;
extern short gsn_poison_dust;
extern short gsn_blindness_dust;
extern short gsn_strangle;
extern short gsn_warcry;
extern short gsn_chimera_lion;
extern short gsn_chimera_goat;
extern short gsn_strange_form;
extern short gsn_enlist;
extern short gsn_shadowplane;
extern short gsn_blackjack_timer;
extern short gsn_strangle_timer;
extern short gsn_tame;
extern short gsn_track;
extern short gsn_find_water;
extern short gsn_shield_cleave;
extern short gsn_spellcraft;
extern short gsn_cloak_form;
extern short gsn_demand;
extern short gsn_shapeshift;
extern short gsn_regeneration;
extern short gsn_parrot;
extern short gsn_door_bash;

extern short gsn_headbutt;
extern short gsn_gore;
extern short gsn_scorch;
extern short gsn_shattered_bone;
extern short gsn_forget;
extern short gsn_earthbind;
extern short gsn_divine_touch;
extern short gsn_forage;
extern short gsn_assassinate;
extern short gsn_defend;
extern short gsn_intimidate;
extern short gsn_escape;
extern short gsn_moving_ambush;
extern short gsn_pugil;
extern short gsn_staff;

extern short gsn_turn_undead;
extern short gsn_spiritual_hammer;
extern short gsn_crimson_martyr;
extern short gsn_benevolence;
extern short gsn_intercept;
extern short gsn_champions_defense;
extern short gsn_indomitable_spirit;
extern short gsn_protection_heat_cold;

extern short gsn_rally;
extern short gsn_tactics;
extern short gsn_lash;
extern short gsn_prevent_healing;
extern short gsn_regeneration;
extern short gsn_quiet_movement;
extern short gsn_iron_resolve;

extern short gsn_atrophy;
extern short gsn_stoneskin;
extern short gsn_shield;
extern short gsn_starvation;
extern short gsn_spore;
extern short gsn_barbarian_strike;
extern short gsn_barbarian_bone_tear;
extern short gsn_dehydrated;			// For starve/thirst
extern short gsn_hold_person;			// Hold person -Detlef
extern short gsn_knife;				// Thief knife -Dioxide
extern short gsn_disperse;				// Disperse for Outlaw -Dioxide
extern short gsn_lightwalk;			// Quest lightwalk -Dioxide
extern short gsn_sanguine_ward;
extern short gsn_bind;
extern short gsn_deafen;				// Deafen for AP -Detlef
extern short gsn_divine_intervention;	// divine intervention -Dioxide
extern short gsn_creep;				// creep skill -Dioxide
extern short gsn_unholy_timer;
extern short gsn_parting_blow;
extern short gsn_mysterious_dream;
extern short gsn_pincer;
extern short gsn_devour;
extern short gsn_keen_vision;
extern short gsn_vampiric_touch;
extern short gsn_snare;
extern short gsn_snaretimer;
extern short gsn_soften;
extern short gsn_consecrate;
extern short gsn_blade_barrier;
extern short gsn_sunray;
extern short gsn_enlarge;
extern short gsn_mana_drain;
extern short gsn_worldbind;
extern short gsn_sceptre;
extern short gsn_sceptret;
extern short gsn_dark_blessing;
extern short gsn_web;
extern short gsn_volley;



extern short gsn_talismanic;
extern short gsn_hex;
extern short gsn_visceral;
extern short gsn_conflagration;
extern short gsn_gravity_well;
extern short gsn_ultradiffusion;
extern short gsn_molecular_disjunction;
extern short gsn_vacuum;
extern short gsn_cyclone_forming;
extern short gsn_cyclone;
extern short gsn_incandescense;
extern short gsn_diuretic;
extern short gsn_corona;
extern short gsn_earthquake;
extern short gsn_mana_conduit;
extern short gsn_synaptic_enhancement;
extern short gsn_synaptic_impairment;
extern short gsn_interference;
extern short gsn_elecshield;
extern short gsn_abite;
extern short gsn_lbite;
extern short gsn_hydroperception;
extern short gsn_watershield;
extern short gsn_flood;
extern short gsn_tidalwave;
extern short gsn_riptide;
extern short gsn_watermeld;
extern short gsn_sense_disturbance;
extern short gsn_travelease;
extern short gsn_stability;
extern short gsn_crush;
extern short gsn_earthshield;
extern short gsn_coldshield;
extern short gsn_heatshield;
extern short gsn_chill;
extern short gsn_reduce;
extern short gsn_airshield;
extern short gsn_coagulate;
extern short gsn_unholy_bond;
extern short gsn_drive;
extern short gsn_hypothermia;
extern short gsn_retreat;
extern short gsn_dash;
extern short gsn_imprisonvoice;
extern short gsn_howl;
extern short gsn_scramble_neurons;
extern short gsn_acid_vein;
extern short gsn_acid_stream;
extern short gsn_corrode_lock;
extern short gsn_attract;
extern short gsn_call_lightning;
extern short gsn_grounding;
extern short gsn_neutralize;
extern short gsn_caustic_vapor;
extern short gsn_noxious_fumes;
extern short gsn_drain;
extern short gsn_smokescreen;
extern short gsn_shroud_of_secrecy;
extern short gsn_smother;
extern short gsn_noxious_ward;
extern short gsn_blanket;
extern short gsn_accumulate_heat;
extern short gsn_earthsembrace;
extern short gsn_melt_rock;
extern short gsn_magma_tunnel;
extern short gsn_heat_earth;
extern short gsn_whiteout;
extern short gsn_unbreakable;
extern short gsn_freeze_door;
extern short gsn_frost_growth;
extern short gsn_bind_feet;
extern short gsn_glaciate;
extern short gsn_stalactites;
extern short gsn_ice_blast;
extern short gsn_icy_carapace;
extern short gsn_sheath_of_ice;
extern short gsn_ironskin;
extern short gsn_burden;
extern short gsn_cloak_of_mist;
extern short gsn_airy_water;
extern short gsn_cooling_mist;
extern short gsn_creeping_tomb;
extern short gsn_pass_without_trace;
extern short gsn_quicksand;
extern short gsn_quicksand_sinking;
extern short gsn_earthfade;
extern short gsn_rust;
extern short gsn_puddle;
extern short gsn_indom;
extern short gsn_traitors_luck;
extern short gsn_darksight;
extern short gsn_bloodlust;
extern short gsn_unholy_communion;
extern short gsn_lesser_demon;
extern short gsn_greater_demon;
extern short gsn_gamygyn_soul;
extern short gsn_orobas_soul;
extern short gsn_dark_familiar;
extern short gsn_word_of_command;
extern short gsn_mark_of_wrath;
extern short gsn_baals_mastery;
extern short gsn_living_blade;
extern short gsn_leech;
extern short gsn_plasma_arc;
extern short gsn_sphere_of_plasma;
extern short gsn_essence_of_plasma;
extern short gsn_plasma_cube;
extern short gsn_plasma_thread;
extern short gsn_fashion_crystal;
extern short gsn_farsee;
extern short gsn_blinding_orb;
extern short gsn_rotating_ward;
extern short gsn_plant;
extern short gsn_gag;
extern short gsn_drag;
extern short gsn_palm;
extern short gsn_trail;
extern short gsn_tripwire;
extern short gsn_sign;
extern short gsn_slash;
extern short gsn_stash;
extern short gsn_avoid;
extern short gsn_disguise;
extern short gsn_shadow_cloak;
extern short gsn_stealth;
extern short gsn_counterfeit;
extern short gsn_strip;
extern short gsn_silent_movement;
extern short gsn_infidels_weight;
extern short gsn_burning_vision;
extern short gsn_divine_ward;
extern short gsn_scribe;
extern short gsn_healing_sleep;
extern short gsn_deny_magic;
extern short gsn_nullify;
extern short gsn_bane;
extern short gsn_gking;
extern short gsn_lesser_golem;
extern short gsn_greater_golem;
extern short gsn_aggressor;
extern short gsn_subdue;
extern short gsn_drowning;
extern short gsn_hunters_awareness;
extern short gsn_hunters_stealth;
extern short gsn_hunters_net;
extern short gsn_severed;


extern short gsn_group_retreat;
extern short gsn_arms_of_light;
extern short gsn_arms_of_wrath;
extern short gsn_arms_of_purity;
extern short gsn_arms_of_judgement;
extern short gsn_holy_shroud;
extern short gsn_seraphic_mantle;
extern short gsn_shield_of_faith;
extern short gsn_rites_of_preparation;
extern short gsn_channeling;
extern short gsn_empathy;
extern short gsn_altruism;
extern short gsn_tower_of_fortitude;
extern short gsn_shadowsense;
extern short gsn_fervor;
extern short gsn_piety;
extern short gsn_spiritual_healing;
extern short gsn_retribution;
extern short gsn_shroud_of_light;
extern short gsn_infidels_fate;
extern short gsn_mana_sickness;
extern short gsn_mana_infusion;
extern short gsn_tangleroot;


//
// Gladiator
//

extern short gsn_fend;
extern short gsn_entrap;
extern short gsn_gouge;
extern short gsn_hobble;
extern short gsn_crippling_blow;
extern short gsn_bleed;

//
// Duelist
//

extern short gsn_maneuvering;
extern short gsn_catch;
extern short gsn_posture;
extern short gsn_unbalance;
extern short gsn_sidestep;
extern short gsn_concuss;
extern short gsn_disrupt_formation;

//
// Barbarian
//

extern short gsn_brutality;
extern short gsn_shatter;
extern short gsn_uppercut;
extern short gsn_impale;
extern short gsn_overhead;
extern short gsn_batter;


//
// Dragoon
//

extern short gsn_exchange;
extern short gsn_charge;
extern short gsn_deflect;
extern short gsn_ease;
extern short gsn_brace;
extern short gsn_shieldbash;


//
// Skirmisher
//

extern short gsn_evasion;
extern short gsn_hurl;
extern short gsn_dart;
extern short gsn_cracked_sternum;
extern short gsn_bloody_eyes;
extern short gsn_broken_arm;
extern short gsn_entwine;
extern short gsn_pull;
extern short gsn_fifth_attack;
extern short gsn_whirlwind;

//
// Tactician
//

extern short gsn_leadership;
extern short gsn_assess;
extern short gsn_exploit;
extern short gsn_outflank;
extern short gsn_terrain_mastery;
extern short gsn_analyze;

//
// Style skills to keep track of percents
//

extern short gsn_duelist_skill;
extern short gsn_gladiator_skill;
extern short gsn_barbarian_skill;
extern short gsn_dragoon_skill;
extern short gsn_skirmisher_skill;
extern short gsn_tactician_skill;


extern short gsn_rprog_elven_down;


extern MOB_INDEX_DATA * mob_index_hash[MAX_KEY_HASH];
extern OBJ_INDEX_DATA * obj_index_hash[MAX_KEY_HASH];
extern ROOM_INDEX_DATA *room_index_hash[MAX_KEY_HASH];
extern AREA_DATA *area_last;
extern char *top_string;
extern char str_empty[1];
extern short count_data[30000];
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
extern int top_vnum_room;
extern int top_vnum_mob;
extern int top_vnum_obj;
extern int mobile_count;
extern int newmobs;
extern int newobjs;
extern bool fBootDb;
extern FILE * fpArea;
extern char strArea[MAX_INPUT_LENGTH];
extern int num_pfiles;

extern int nAllocString; 
extern int sAllocString;
extern int nAllocPerm;
extern int sAllocPerm;

extern short cabal_members[MAX_CABAL];
extern short cabal_max[MAX_CABAL];


// TODO: UNKNOWN FUNCTION
extern int _filbuf (FILE *);


// macro for flag swapping
#define GET_UNSET(flag1,flag2)		(~(flag1)&((flag1)|(flag2)))

// Magic number for memory allocation
#define MAGIC_NUM					52571214

//#define social_debug				0

//
// LOCAL FUNCTIONS
//

char *munch (char *str);
void update_db_gold(void);
void load_area (FILE * fp);
void sort_areas (void);
void new_load_area (FILE * fp);				/* OLC */
void assign_area_vnum (int vnum);			/* OLC */
/*
 * Adds a reset to a room.  OLC
 * Similar to add_reset in olc.c
 */
void new_reset(ROOM_INDEX_DATA *pR, RESET_DATA *pReset);
void load_resets (FILE * fp);
/*
 * Snarf a shop section.
 */
void load_shops(FILE *fp);
void load_cabal_items (void);
void fix_exits (void);
void find_adjacents (void);
void area_update(void);
void reset_room (ROOM_INDEX_DATA *pRoom);	/* OLC */
void reset_area (AREA_DATA * pArea);		/* OLC */
CHAR_DATA *create_mobile (MOB_INDEX_DATA *pMobIndex);
void clone_mobile (CHAR_DATA *parent, CHAR_DATA *clone);
OBJ_DATA *create_object (OBJ_INDEX_DATA *pObjIndex, int level);
void clone_object (OBJ_DATA *parent, OBJ_DATA *clone);
void clear_char(CHAR_DATA *ch);
char *get_extra_descr (const char *name, EXTRA_DESCR_DATA *ed);
MOB_INDEX_DATA *get_mob_index (int vnum);
OBJ_INDEX_DATA *get_obj_index(int vnum);
ROOM_INDEX_DATA *get_room_index (int vnum);
char fread_letter (FILE *fp);
int fread_number (FILE *fp);
long fread_flag (FILE *fp);
void fread_flag_new (long vector[], FILE *fp);
long vector_convert (char letter);
long flag_convert (char letter);
void fread_vector(long vector[], FILE *fp);
char *fread_string (FILE *fp);
char *fread_string_eol(FILE *fp);
void fread_to_eol (FILE *fp);
char *fread_word (FILE *fp);
/*
 * Allocate some ordinary memory,
 *   with the expectation of freeing it someday.
 */
void *iamlame(int sMem);
/*
 * Free some memory.
 * Recycle it back onto the free list for blocks of that size.
 */
void soami(void *pMem, int sMem);
/*
 * Allocate some permanent memory.
 * Permanent memory is never freed,
 *   pointers into it may be copied safely.
 */
void *crappyold(int sMem);
/*
 * Duplicate a string into dynamic memory.
 * Fread_strings are read-only and shared.
 */
char *iamnotereet(const char *str);
void do_areas(CHAR_DATA *ch, char *argument);
void do_memory(CHAR_DATA *ch, char *argument);
void do_dump(CHAR_DATA *ch, char *argument);
int number_fuzzy (int number);
int number_range (int from, int to);
int number_percent (void);
int number_door (void);
int number_bits (int width);
void init_mm (void);
long number_mm (void);
int dice (int number, int size);
int interpolate(int level, int value_00, int value_32);
void smash_tilde (char *str);
bool str_cmp (const char *astr, const char *bstr);
bool str_prefix (const char *astr, const char *bstr);
bool str_infix (const char *astr, const char *bstr);
bool str_suffix (const char *astr, const char *bstr);
char *capitalize (const char *str);
void append_file (CHAR_DATA *ch, char *file, char *str);
void tail_chain(void);
void do_force_reset(CHAR_DATA *ch, char *argument);
/*
void do_alist(CHAR_DATA *ch,char *argument);
*/
void do_llimit(CHAR_DATA *ch, char *argument);
void load_rooms (FILE *fp);
void load_newresets (FILE * fp);
void clean_notes (void);
void load_race_info (void);
char intflag_to_char(int i);
char *bitvector_to_flag (long bitvect[]);

#endif /* DB_H */
