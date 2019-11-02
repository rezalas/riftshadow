#ifndef SORCERER_H
#define SORCERER_H


/****************************************************************************
 *                        Welcome to Sorcerer land.                         *
 ****************************************************************************/
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "merc.h"
#include "magic.h"
#include "recycle.h"
#include "db.h"
#include "lookup.h"
#include "interp.h"
#include "tables.h"

extern int flag_lookup (const char *name, const struct flag_type *flag_table);
extern char * flag_name_lookup (long bitv, const struct flag_type *flag_table);
extern int damage_queue (CHAR_DATA *ch, CHAR_DATA *victim, int dam, int damtype, bool blockable, int add, int mult, char *dnoun);

extern char * const dir_name[];
extern const sh_int rev_dir[];

int para_compute (int ele1, int ele2);
void spell_scorch (int sn, int level, CHAR_DATA *ch, void *vo, int target );
void spell_gravity_well (int sn, int level, CHAR_DATA *ch, void *vo, int target );
void gravity_well_explode (ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *af);
void spell_cyclone (int sn, int level, CHAR_DATA *ch, void *vo, int target );
void cyclone_begin (AREA_DATA *area, AREA_AFFECT_DATA *af);
void cyclone_begin_tick (AREA_DATA *area, AREA_AFFECT_DATA *af);
void cyclone_end_fun (AREA_DATA *area, AREA_AFFECT_DATA *af);
void spell_chill (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_chillmetal (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_conflagration (int sn, int level, CHAR_DATA *ch, void *vo, int target);
bool conflagrate_room (ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *oldaf);
void conflagration_pulse (ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *af);
void conflag_burnout (ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *af);
void spell_ultradiffusion (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void ultradiffusion_tick (CHAR_DATA *ch, AFFECT_DATA *af);
void un_ultradiffusion (CHAR_DATA *ch, char *argument);
void ultradiffusion_end (CHAR_DATA *ch, AFFECT_DATA *af);
void spell_heat_metal (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_knock (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_vacuum (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void vacuum_end_fun (ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *af);
void spell_incandescense (int sn, int level, CHAR_DATA *ch, void *vo,int target );
void spell_infravision (int sn, int level, CHAR_DATA *ch, void *vo,int target );
void spell_diuretic (int sn, int level, CHAR_DATA *ch, void *vo,int target );
void spell_corona (int sn, int level, CHAR_DATA *ch, void *vo,int target );
void spell_heatshield (int sn,int level,CHAR_DATA *ch,void *vo,int target);
void spell_immolate (int sn,int level,CHAR_DATA *ch,void *vo,int target);
void immolate_end (OBJ_DATA *obj, OBJ_AFFECT_DATA *af);
void spell_scathing (int sn,int level,CHAR_DATA *ch,void *vo,int target);
void spell_earthquake (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_electrocute (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_induce_pain (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_disrupt_vision (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_mana_conduit (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_synaptic_enhancement (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_synaptic_impairment (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_elecshield (int sn,int level,CHAR_DATA *ch,void *vo,int target);
void spell_scramble_neurons (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_mana_leech (int sn,int level,CHAR_DATA *ch,void *vo,int target);
void spell_interference (int sn, int level, CHAR_DATA *ch, void *vo, int target );
void interference_end (AREA_DATA *area, AREA_AFFECT_DATA *af);
void spell_hydroperception (int sn, int level, CHAR_DATA *ch, void *vo,int target );
void spell_dehydrate (int sn,int level,CHAR_DATA *ch,void *vo,int target);
void spell_drown (int sn,int level,CHAR_DATA *ch,void *vo,int target);
void spell_hydration (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_regeneration (int sn,int level,CHAR_DATA *ch,void *vo,int target);
void spell_watershield (int sn,int level,CHAR_DATA *ch,void *vo,int target);
void spell_flood (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void flood_recede (ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *af);
void spell_tidalwave (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_riptide (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void riptide_one_end (ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *af);
void riptide_two_end (ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *af);
int average_ac (CHAR_DATA *ch);
void spell_watermeld (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_sense_disturbance (int sn, int level, CHAR_DATA *ch, void *vo,int target );
void spell_travelease (int sn, int level, CHAR_DATA *ch, void *vo,int target );
void spell_diffusion (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_disruption (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_anchor (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_aerial_transferrence (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_airshield (int sn,int level,CHAR_DATA *ch,void *vo,int target);
/* Earth spells */
void spell_hardenfist (int sn, int level, CHAR_DATA *ch, void *vo, int target );
void spell_stability (int sn,int level,CHAR_DATA *ch,void *vo,int target);
void spell_crush (int sn,int level,CHAR_DATA *ch,void *vo,int target);
void spell_sensevibrations (int sn, int level, CHAR_DATA *ch, void *vo, int target );
void spell_diamondskin (int sn, int level, CHAR_DATA *ch, void *vo, int target );
void spell_overbear (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_reduce (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_earthshield (int sn,int level,CHAR_DATA *ch,void *vo,int target);
void spell_coldshield (int sn,int level,CHAR_DATA *ch,void *vo,int target);
void spell_coagulate (int sn, int level, CHAR_DATA *ch, void *vo,int target );
void spell_hypothermia (int sn, int level, CHAR_DATA *ch, void *vo,int target );
void spell_imprisonvoice (int sn, int level, CHAR_DATA *ch, void *vo,int target );
void spell_frigidaura (int sn,int level,CHAR_DATA *ch,void *vo,int target);
void spell_enervate_agitate_helper (int sn, int level, CHAR_DATA *ch, void *vo, int target, int iDir);
void spell_enervate (int sn,int level,CHAR_DATA *ch,void *vo,int target);
void spell_agitate (int sn,int level,CHAR_DATA *ch,void *vo,int target);
void agitate_tick (CHAR_DATA *ch, AFFECT_DATA *af);
void spell_freezemetal (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_frostbite (int sn, int level, CHAR_DATA *ch, void *vo, int target);
int scramble_sn (CHAR_DATA *ch, int sn );
void spell_acid_stream  (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_acid_vein (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void acid_end (OBJ_DATA *obj, OBJ_AFFECT_DATA *af);
void spell_corrode_lock (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_attract  (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void attract_tick (CHAR_DATA *ch, AFFECT_DATA *af);
void spell_absorb  (int sn, int level, CHAR_DATA *ch, void *vo, int target );
void spell_call_lightning  (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_grounding  (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void grounding_end (CHAR_DATA *ch, AFFECT_DATA *af);
void spell_thunderclap (int sn,int level,CHAR_DATA *ch,void *vo,int target);
void spell_neutralize (int sn, int level,CHAR_DATA *ch, void *vo, int target);
void spell_caustic_vapor (int sn, int level, CHAR_DATA *ch, void *vo,int target );
void caustic_vapor_burnout (ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *af);
void spell_smokescreen (int sn, int level, CHAR_DATA *ch, void *vo,int target);
void smokescreen_end (ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *af);
void spell_smother (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_putrid_air (int sn, int level, CHAR_DATA *ch, void *vo,int target);
void spell_asphyxiate (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_shroud_of_secrecy (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void shroud_end (CHAR_DATA *ch, AFFECT_DATA *af);
void spell_noxious_ward (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_molten_stones (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_heat_earth (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_blanket (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void blanket_melt (ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *af);
void spell_boreal_wind (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_concave_shell (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void concave_shell_move (CHAR_DATA *ch, int *dirptr,ROOM_INDEX_DATA *oldroom);
void spell_frost_glaze (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_unbreakable (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_earthsembrace (int sn,int level, CHAR_DATA *ch, void *vo, int target);
void spell_whiteout (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void whiteout_end (AREA_DATA *area, AREA_AFFECT_DATA *af);
void spell_frigid_breeze (int sn, int level, CHAR_DATA *ch, void *vo,int target);
void spell_pure_air (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_icelance (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_freeze_door (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void door_unfreeze (ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *af);
void spell_frost_growth (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void ground_thaw (ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *af);
void spell_bind_feet (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_glaciate (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void glaciate_melt (ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *raf);
void spell_hailstorm (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_stalactites (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_ice_blast (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void container_defrost (OBJ_DATA *obj, OBJ_AFFECT_DATA *af);
void spell_icy_carapace (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_sheath_of_ice (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void ice_sheath_melt (OBJ_DATA *obj, OBJ_AFFECT_DATA *af);
void spell_ironskin (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_metal_shards (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_burden (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_fortify_weapon (int sn,int level, CHAR_DATA *ch, void *vo, int target);
void spell_fortify_armor (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_alter_metal (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_cloak_of_mist (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_vigorize  (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_creeping_tomb (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void creeping_tomb_tick (CHAR_DATA *ch, AFFECT_DATA *af);
void spell_pass_without_trace (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_quicksand (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void quicksand_end (ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *af);
void quicksand_pulse_sink (CHAR_DATA *ch, AFFECT_DATA *af);
void spell_sap_endurance (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_emulsify (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_rust (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_airy_water (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_cooling_mist (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_prismatic_spray (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_earthfade (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void earthfade_end(CHAR_DATA *ch, AFFECT_DATA *af);
void earthfade_tick(CHAR_DATA *ch, AFFECT_DATA *af);
void spell_plasma_arc (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_plasma_bolt (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void sphere_of_plasma_end (CHAR_DATA *ch, AFFECT_DATA *paf);
void sphere_of_plasma_pulse (CHAR_DATA *ch, AFFECT_DATA *af);
void spell_sphere_of_plasma (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_plasma_cube (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_essence_of_plasma (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void essence_of_plasma_pulse (ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *af);
void essence_of_plasma_end (ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *af);
void plasma_thread_end (CHAR_DATA *ch, AFFECT_DATA *paf);
void spell_plasma_thread (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void check_plasma_thread (CHAR_DATA *ch, int direction);
void spell_accumulate_heat (int sn,int level, CHAR_DATA *ch, void *vo, int target);
void spell_melt_rock (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_magma_tunnel (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_fashion_crystal (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void crystal_tick (OBJ_DATA *obj, OBJ_AFFECT_DATA *af);
void spell_farsee (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_mana_beam (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_detonation (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void spell_rotating_ward (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void rotating_tick (CHAR_DATA *ch, AFFECT_DATA *af);
void spell_fortify_crystal (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void mana_infusion_helper (CHAR_DATA *ch, CHAR_DATA *victim);
void spell_mana_infusion (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void infusion_tick (CHAR_DATA *ch, AFFECT_DATA *af);

#endif /* SORCERER_H */
