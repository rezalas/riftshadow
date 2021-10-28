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
extern sh_int sun;
extern sh_int moon_berus;
extern sh_int berus_pos;
extern sh_int moon_calabren;
extern sh_int calabren_pos;
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
//extern sh_int gsn_battlecry;
//extern sh_int gsn_empower;
//extern sh_int gsn_power_word_fear;
//extern sh_int gsn_steel_nerves;
//extern sh_int gsn_bandage;
//extern sh_int gsn_flurry;				// Quest flurry -Dioxide
//extern sh_int gsn_drum;				// Quest drum -Dioxide
//extern sh_int gsn_executioner;
//extern sh_int gsn_phat_blunt;


extern sh_int gsn_timer;
extern sh_int gsn_repose;
extern sh_int gsn_medicine;
extern sh_int gsn_somatic_casting;
extern sh_int gsn_frigidaura;
extern sh_int gsn_stasis_wall;
extern sh_int gsn_integrate;
extern sh_int gsn_sensevibrations;
extern sh_int gsn_diamondskin;
extern sh_int gsn_hardenfist;
extern sh_int gsn_jackal;
extern sh_int gsn_mangled;
extern sh_int gsn_rage;
extern sh_int gsn_horde_communion;
extern sh_int gsn_bleeding;
extern sh_int gsn_calm;
extern sh_int gsn_agitate;
extern sh_int gsn_immolate;
extern sh_int gsn_enervate;
extern sh_int gsn_corona;
extern sh_int gsn_infravision;
extern sh_int gsn_faerie_fire;
extern sh_int gsn_faerie_fog;
extern sh_int gsn_backstab;
extern sh_int gsn_detect_hidden;
extern sh_int gsn_detect_movement;
extern sh_int gsn_bluff;
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

extern sh_int gsn_divine_frenzy;
extern sh_int gsn_awe;
extern sh_int gsn_strike_of_virtue;

extern sh_int gsn_decrepify;
extern sh_int gsn_displacement;
extern sh_int gsn_false_motives;

extern sh_int gsn_blindness;
extern sh_int gsn_chill_touch;
extern sh_int gsn_burning_hands;
extern sh_int gsn_charm_person;
extern sh_int gsn_curse;
extern sh_int gsn_invis;
extern sh_int gsn_mass_invis;
extern sh_int gsn_poison;
extern sh_int gsn_plague;
extern sh_int gsn_sleep;
extern sh_int gsn_sanctuary;
extern sh_int gsn_fly;

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
extern sh_int gsn_circle;
extern sh_int gsn_dual_backstab;
extern sh_int gsn_skin;
extern sh_int gsn_camp;
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

extern sh_int gsn_turn_undead;
extern sh_int gsn_spiritual_hammer;
extern sh_int gsn_crimson_martyr;
extern sh_int gsn_benevolence;
extern sh_int gsn_intercept;
extern sh_int gsn_champions_defense;
extern sh_int gsn_indomitable_spirit;
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
extern sh_int gsn_knife;				// Thief knife -Dioxide
extern sh_int gsn_disperse;				// Disperse for Outlaw -Dioxide
extern sh_int gsn_lightwalk;			// Quest lightwalk -Dioxide
extern sh_int gsn_sanguine_ward;
extern sh_int gsn_bind;
extern sh_int gsn_deafen;				// Deafen for AP -Detlef
extern sh_int gsn_divine_intervention;	// divine intervention -Dioxide
extern sh_int gsn_creep;				// creep skill -Dioxide
extern sh_int gsn_unholy_timer;
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


extern sh_int gsn_rprog_elven_down;


extern MOB_INDEX_DATA * mob_index_hash[MAX_KEY_HASH];
extern OBJ_INDEX_DATA * obj_index_hash[MAX_KEY_HASH];
extern ROOM_INDEX_DATA *room_index_hash[MAX_KEY_HASH];
extern AREA_DATA *area_last;
extern char *top_string;
extern char str_empty[1];
extern sh_int count_data[30000];
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

extern sh_int cabal_members[MAX_CABAL];
extern sh_int cabal_max[MAX_CABAL];


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
/*
 * Reports a bug.
 */
void bug(const char *str, int param);
void log_string (const char *str);
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
