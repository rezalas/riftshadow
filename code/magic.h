/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,	   *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *									   *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael	   *
 *  Chastain, Michael Quan, and Mitchell Tse.				   *
 *									   *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc	   *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.						   *
 *									   *
 *  Much time and thought has gone into this software and you are	   *
 *  benefitting.  We hope that you share your changes too.  What goes	   *
 *  around, comes around.						   *
 ***************************************************************************/
 
/***************************************************************************
*	ROM 2.4 is copyright 1993-1996 Russ Taylor			   *
*	ROM has been brought to you by the ROM consortium		   *
*	    Russ Taylor (rtaylor@efn.org)				   *
*	    Gabrielle Taylor						   *
*	    Brian Moore (zump@rom.org)					   *
*	By using this code, you have agreed to follow the terms of the	   *
*	ROM license, in the file Tartarus/doc/rom.license                  *
***************************************************************************/

/***************************************************************************
*       Tartarus code is copyright (C) 1997-1998 by Daniel Graham          *
*	In using this code you agree to comply with the Tartarus license   *
*       found in the file /Tartarus/doc/tartarus.doc                       *
***************************************************************************/

/*
 * Spell functions.
 * Defined in magic.c.
 */
DECLARE_SPELL_FUN(	spell_null		);
DECLARE_SPELL_FUN(	spell_acid_blast	);
DECLARE_SPELL_FUN(	spell_armor		);
DECLARE_SPELL_FUN(	spell_bless		);
DECLARE_SPELL_FUN(	spell_blindness		);
DECLARE_SPELL_FUN(	spell_burning_hands	);
DECLARE_SPELL_FUN(	spell_call_lightning	);
DECLARE_SPELL_FUN(      spell_calm		);
DECLARE_SPELL_FUN(      spell_cancellation	);
DECLARE_SPELL_FUN(	spell_cause_critical	);
DECLARE_SPELL_FUN(	spell_cause_light	);
DECLARE_SPELL_FUN(	spell_cause_serious	);
DECLARE_SPELL_FUN(	spell_change_sex	);
DECLARE_SPELL_FUN(      spell_chain_lightning   );
DECLARE_SPELL_FUN(	spell_charm_person	);
DECLARE_SPELL_FUN(	spell_chill		);
DECLARE_SPELL_FUN(	spell_chill_touch	);
DECLARE_SPELL_FUN(	spell_color_spray	);
DECLARE_SPELL_FUN(	spell_continual_light	);
DECLARE_SPELL_FUN(	spell_control_weather	);
DECLARE_SPELL_FUN(	spell_create_food	);
DECLARE_SPELL_FUN(	spell_create_rose	);
DECLARE_SPELL_FUN(	spell_create_spring	);
DECLARE_SPELL_FUN(	spell_create_water	);
DECLARE_SPELL_FUN(	spell_cure_blindness	);
DECLARE_SPELL_FUN(	spell_cure_critical	);
DECLARE_SPELL_FUN(      spell_cure_disease	);
DECLARE_SPELL_FUN(	spell_cure_light	);
DECLARE_SPELL_FUN(	spell_cure_poison	);
DECLARE_SPELL_FUN(	spell_cure_serious	);
DECLARE_SPELL_FUN(	spell_curse		);
DECLARE_SPELL_FUN(      spell_demonfire		);
DECLARE_SPELL_FUN(	spell_detect_evil	);
DECLARE_SPELL_FUN(	spell_detect_good	);
DECLARE_SPELL_FUN(	spell_detect_hidden	);
DECLARE_SPELL_FUN(	spell_detect_invis	);
DECLARE_SPELL_FUN(	spell_detect_magic	);
DECLARE_SPELL_FUN(	spell_detect_poison	);
DECLARE_SPELL_FUN(	spell_dispel_evil	);
DECLARE_SPELL_FUN(      spell_dispel_good       );
DECLARE_SPELL_FUN(	spell_dispel_magic	);
DECLARE_SPELL_FUN(	spell_earthquake	);
DECLARE_SPELL_FUN(	spell_energy_drain	);
DECLARE_SPELL_FUN(	spell_faerie_fire	);
DECLARE_SPELL_FUN(	spell_faerie_fog	);
DECLARE_SPELL_FUN(	spell_fireball		);
DECLARE_SPELL_FUN(	spell_fireproof		);
DECLARE_SPELL_FUN(	spell_flamestrike	);
DECLARE_SPELL_FUN(	spell_floating_disc	);
DECLARE_SPELL_FUN(	spell_fly		);
DECLARE_SPELL_FUN(      spell_frenzy		);
DECLARE_SPELL_FUN(	spell_gate		);
DECLARE_SPELL_FUN(	spell_giant_strength	);
DECLARE_SPELL_FUN(	spell_harm		);
DECLARE_SPELL_FUN(      spell_haste		);
DECLARE_SPELL_FUN(	spell_heal		);
DECLARE_SPELL_FUN(	spell_heat_metal	);
DECLARE_SPELL_FUN(      spell_holy_word		);
DECLARE_SPELL_FUN(	spell_identify		);
DECLARE_SPELL_FUN(	spell_infravision	);
DECLARE_SPELL_FUN(	spell_invis		);
DECLARE_SPELL_FUN(	spell_know_alignment	);
DECLARE_SPELL_FUN(	spell_lightning_bolt	);
DECLARE_SPELL_FUN(	spell_locate_object	);
DECLARE_SPELL_FUN(	spell_magic_missile	);
DECLARE_SPELL_FUN(      spell_mass_healing	);
DECLARE_SPELL_FUN(	spell_mass_invis	);
DECLARE_SPELL_FUN(	spell_nexus		);
DECLARE_SPELL_FUN(	spell_pass_door		);
DECLARE_SPELL_FUN(      spell_plague		);
DECLARE_SPELL_FUN(	spell_poison		);
DECLARE_SPELL_FUN(	spell_portal		);
DECLARE_SPELL_FUN(	spell_protection	);
DECLARE_SPELL_FUN(	spell_ray_of_truth	);
DECLARE_SPELL_FUN(	spell_recharge		);
DECLARE_SPELL_FUN(	spell_refresh		);
DECLARE_SPELL_FUN(	spell_remove_curse	);
DECLARE_SPELL_FUN(	spell_sanctuary		);
DECLARE_SPELL_FUN(	spell_shocking_grasp	);
DECLARE_SPELL_FUN(	spell_shield		);
DECLARE_SPELL_FUN(	spell_sleep		);
DECLARE_SPELL_FUN(	spell_slow		);
DECLARE_SPELL_FUN(	spell_stone_skin	);
DECLARE_SPELL_FUN(	spell_summon		);
DECLARE_SPELL_FUN(	spell_teleport		);
DECLARE_SPELL_FUN(	spell_ventriloquate	);
DECLARE_SPELL_FUN(	spell_weaken		);
DECLARE_SPELL_FUN(	spell_word_of_recall	);
DECLARE_SPELL_FUN(	spell_acid_breath	);
DECLARE_SPELL_FUN(	spell_fire_breath	);
DECLARE_SPELL_FUN(	spell_frost_breath	);
DECLARE_SPELL_FUN(	spell_gas_breath	);
DECLARE_SPELL_FUN(	spell_nether_breath	);
DECLARE_SPELL_FUN(	spell_lightning_breath	);
DECLARE_SPELL_FUN(	spell_general_purpose	);

/* New spells...these, as well as a lot of new skills, will need
 the area file 'skills.are' which contains default object and mobile
 templates for those skills/spells that require them.
 ....Ceran
*/
DECLARE_SPELL_FUN(	spell_grounding			);
DECLARE_SPELL_FUN(	spell_transfer_object	);
DECLARE_SPELL_FUN(	spell_divine_touch		);
DECLARE_SPELL_FUN(	spell_cremate			);
DECLARE_SPELL_FUN(	spell_animate_dead		);
DECLARE_SPELL_FUN(	spell_earthbind			);
DECLARE_SPELL_FUN(	spell_tsunami			);
DECLARE_SPELL_FUN(	spell_familiar			);
DECLARE_SPELL_FUN(	spell_forget			);
DECLARE_SPELL_FUN(	spell_atrophy			);
DECLARE_SPELL_FUN(	spell_fire_and_ice		);
DECLARE_SPELL_FUN(	spell_utter_heal		);
DECLARE_SPELL_FUN(	spell_lifeline			);
DECLARE_SPELL_FUN(	spell_lightshield		);
DECLARE_SPELL_FUN(	spell_shadowself		);
DECLARE_SPELL_FUN(	spell_wrath				);
DECLARE_SPELL_FUN(	spell_dark_wrath		);
DECLARE_SPELL_FUN(	spell_true_sight		);
DECLARE_SPELL_FUN(	spell_guard_call		);
DECLARE_SPELL_FUN(	spell_heavenly_sceptre_fire	);
DECLARE_SPELL_FUN(	spell_heavenly_sceptre_frenzy	);
DECLARE_SPELL_FUN(	spell_call_slayer		);
DECLARE_SPELL_FUN(	spell_lesser_golem		);
DECLARE_SPELL_FUN(	spell_greater_golem		);
DECLARE_SPELL_FUN(	spell_lifebane			);
DECLARE_SPELL_FUN(	spell_absorb			);
DECLARE_SPELL_FUN(	spell_acid_spit			);
DECLARE_SPELL_FUN(	spell_unholy_fire		);
DECLARE_SPELL_FUN(	spell_deathspell		);
DECLARE_SPELL_FUN(	spell_power_word_kill	);
DECLARE_SPELL_FUN(	spell_power_word_stun	);
DECLARE_SPELL_FUN(	spell_trace				);
DECLARE_SPELL_FUN(	spell_flame_scorch		);
DECLARE_SPELL_FUN(	spell_awe			);
DECLARE_SPELL_FUN(	spell_divine_frenzy		);
DECLARE_SPELL_FUN(	spell_soulbind			);
DECLARE_SPELL_FUN(	spell_talk_to_dead		);
DECLARE_SPELL_FUN(	spell_globe_of_darkness	);
DECLARE_SPELL_FUN(	spell_consecrate		);
DECLARE_SPELL_FUN(	spell_protective_shield	);
DECLARE_SPELL_FUN(	spell_drain				);
DECLARE_SPELL_FUN(	spell_web				);
DECLARE_SPELL_FUN(	spell_restoration		);
DECLARE_SPELL_FUN(	spell_holy_fire			);
DECLARE_SPELL_FUN(	spell_blade_barrier		);
DECLARE_SPELL_FUN(	spell_old_blade_barrier );
DECLARE_SPELL_FUN(	spell_fortitude			);
DECLARE_SPELL_FUN(	spell_prevent_healing	);
DECLARE_SPELL_FUN(	spell_spiritblade		);
DECLARE_SPELL_FUN(	spell_iceball			);
DECLARE_SPELL_FUN(	spell_hand_of_vengeance	);
DECLARE_SPELL_FUN(	spell_regenerate		);
DECLARE_SPELL_FUN(	spell_decoy				);
DECLARE_SPELL_FUN(	spell_preserve			);
DECLARE_SPELL_FUN(	spell_regeneration		);
DECLARE_SPELL_FUN(	spell_imbue_regeneration);
DECLARE_SPELL_FUN(	spell_decay_corpse		);
DECLARE_SPELL_FUN(	spell_dark_shield		);
DECLARE_SPELL_FUN(	spell_power_word_fear	);
DECLARE_SPELL_FUN(	spell_life_transfer		);
DECLARE_SPELL_FUN(	spell_turn_undead		);
DECLARE_SPELL_FUN(  spell_hold_person 		); /* Hold person -Detlef */
DECLARE_SPELL_FUN(  spell_slave_of_purgatory); /* Spell for Ancient Only -Detlef */
DECLARE_SPELL_FUN(  spell_mental_knife  	); /* Spell for Kyr's tattoo -Dio */
DECLARE_SPELL_FUN(	spell_scourge			); /* Scourge for Arcanas -Dev */
DECLARE_SPELL_FUN(	spell_sanguine_ward		);
DECLARE_SPELL_FUN(	spell_unholy_bless		);
DECLARE_SPELL_FUN(	spell_crushing_hand		);
DECLARE_SPELL_FUN(  spell_confuse  	    	); /* Confuse for Outlaw -Detlef */
DECLARE_SPELL_FUN(  spell_deafen        	); /* Deafen for AP -Detlef */
DECLARE_SPELL_FUN(  spell_scrying  			);
DECLARE_SPELL_FUN(  spell_nova  			);
DECLARE_SPELL_FUN(  spell_randomizer    	);
DECLARE_SPELL_FUN(  spell_rejuvenate 		); /* Cleric Rejuvenate -Dioxide */
DECLARE_SPELL_FUN(	spell_vampiric_touch	);
DECLARE_SPELL_FUN(	spell_dark_shade		); /* for ancient --dioxide */
DECLARE_SPELL_FUN(	spell_soften			);
DECLARE_SPELL_FUN(	spell_lightwalk			);

DECLARE_SPELL_FUN(	spell_cure_deafness		);
DECLARE_SPELL_FUN(	spell_awaken			);
DECLARE_SPELL_FUN(	spell_resist_heat		);
DECLARE_SPELL_FUN(	spell_resist_cold		);
DECLARE_SPELL_FUN(	spell_resist_lightning	);
DECLARE_SPELL_FUN(	spell_resist_mental		);
DECLARE_SPELL_FUN(	spell_resist_acid		);
DECLARE_SPELL_FUN(	spell_resist_negative	);
DECLARE_SPELL_FUN(	spell_group_teleport	);
DECLARE_SPELL_FUN(	spell_remove_paralysis	);
DECLARE_SPELL_FUN(	spell_cleanse			);
DECLARE_SPELL_FUN(	spell_sunray			);
DECLARE_SPELL_FUN(	spell_enlarge			);
DECLARE_SPELL_FUN(	spell_fatigue			);
DECLARE_SPELL_FUN(	spell_strength			);
DECLARE_SPELL_FUN(	spell_remove_taint		);
DECLARE_SPELL_FUN(	spell_worldbind			);
DECLARE_SPELL_FUN(	spell_informant			);
DECLARE_SPELL_FUN(	spell_hunters_strength	);
DECLARE_SPELL_FUN(	spell_hire_mercenary	);
DECLARE_SPELL_FUN(	spell_hunters_vision	);
DECLARE_SPELL_FUN(	spell_infernal_aid		);
DECLARE_SPELL_FUN(  spell_hunters_awareness	);
DECLARE_SPELL_FUN(	spell_talismanic_aura	);

DECLARE_SPELL_FUN(	spell_waterbreath		);
DECLARE_SPELL_FUN(	spell_dark_vessel		);
DECLARE_SPELL_FUN(	spell_siphon			);
DECLARE_SPELL_FUN(	spell_hex				);
DECLARE_SPELL_FUN(	spell_black_circle		);
DECLARE_SPELL_FUN(	spell_visceral			);
DECLARE_SPELL_FUN(	spell_arms_of_light		);
DECLARE_SPELL_FUN(	spell_arms_of_wrath		);
DECLARE_SPELL_FUN(	spell_arms_of_purity		);
DECLARE_SPELL_FUN(	spell_arms_of_judgement		);
DECLARE_SPELL_FUN(	spell_shield_of_faith		);
DECLARE_SPELL_FUN(	spell_holy_shroud		);
DECLARE_SPELL_FUN(	spell_seraphic_mantle		);
DECLARE_SPELL_FUN(	spell_voice_of_damnation	);
DECLARE_SPELL_FUN(	spell_blinding_orb		);
DECLARE_SPELL_FUN(	spell_scorch			);
DECLARE_SPELL_FUN(	spell_ritual_soul		);
DECLARE_SPELL_FUN(	spell_ritual_flesh		);
DECLARE_SPELL_FUN(	spell_summon_nephilim	);
DECLARE_SPELL_FUN(	spell_conflagration		);
DECLARE_SPELL_FUN(	spell_gravity_well		);
DECLARE_SPELL_FUN(	spell_ultradiffusion	);
DECLARE_SPELL_FUN(	spell_cyclone			);
DECLARE_SPELL_FUN(	spell_knock				);
DECLARE_SPELL_FUN(	spell_vacuum			);
DECLARE_SPELL_FUN(	spell_incandescense		);
DECLARE_SPELL_FUN(	spell_diuretic			);
DECLARE_SPELL_FUN(	spell_corona			);
DECLARE_SPELL_FUN(	spell_immolate			);
DECLARE_SPELL_FUN(	spell_scathing			);
DECLARE_SPELL_FUN(	spell_agitate			);
DECLARE_SPELL_FUN(	spell_enervate			);
DECLARE_SPELL_FUN(	spell_electrocute		);
DECLARE_SPELL_FUN(	spell_induce_pain		);
DECLARE_SPELL_FUN(	spell_disrupt_vision	);
DECLARE_SPELL_FUN(	spell_mana_conduit		);
DECLARE_SPELL_FUN(	spell_synaptic_enhancement	);
DECLARE_SPELL_FUN(	spell_synaptic_impairment	);
DECLARE_SPELL_FUN(	spell_elecshield		);
DECLARE_SPELL_FUN(	spell_mana_leech		);
DECLARE_SPELL_FUN(	spell_interference		);
DECLARE_SPELL_FUN(	spell_rage				);
DECLARE_SPELL_FUN(	spell_horde_communion	);
DECLARE_SPELL_FUN(	spell_hydroperception	);
DECLARE_SPELL_FUN(	spell_dehydrate			);
DECLARE_SPELL_FUN(	spell_drown				);
DECLARE_SPELL_FUN(	spell_hydration			);
DECLARE_SPELL_FUN(	spell_decrepify			);
DECLARE_SPELL_FUN(	spell_epic				);
DECLARE_SPELL_FUN(	spell_corrupt_flesh		);
DECLARE_SPELL_FUN(	spell_corpse_trap		);
DECLARE_SPELL_FUN(	spell_regeneration		);
DECLARE_SPELL_FUN(	spell_watershield		);
DECLARE_SPELL_FUN(	spell_flood				);
DECLARE_SPELL_FUN(	spell_tidalwave			);
DECLARE_SPELL_FUN(	spell_riptide			);
DECLARE_SPELL_FUN(	spell_watermeld			);
DECLARE_SPELL_FUN(	spell_sense_disturbance	);
DECLARE_SPELL_FUN(	spell_travelease		);
DECLARE_SPELL_FUN(	spell_diffusion			);
DECLARE_SPELL_FUN(	spell_disruption		);
DECLARE_SPELL_FUN(	spell_hardenfist		);
DECLARE_SPELL_FUN(	spell_stability			);
DECLARE_SPELL_FUN(	spell_crush				);
DECLARE_SPELL_FUN(	spell_sensevibrations	);
DECLARE_SPELL_FUN(	spell_diamondskin		);
DECLARE_SPELL_FUN(	spell_overbear			);
DECLARE_SPELL_FUN(	spell_earthshield		);
DECLARE_SPELL_FUN(	spell_coldshield		);
DECLARE_SPELL_FUN(	spell_heatshield		);
DECLARE_SPELL_FUN(	spell_reduce			);
DECLARE_SPELL_FUN(	spell_anchor			);
DECLARE_SPELL_FUN(	spell_aerial_transferrence	);
DECLARE_SPELL_FUN(	spell_airshield			);
DECLARE_SPELL_FUN(	spell_coagulate			);
DECLARE_SPELL_FUN(	spell_frigidaura		);
DECLARE_SPELL_FUN(	spell_chillmetal		);
DECLARE_SPELL_FUN(	spell_freezemetal		);
DECLARE_SPELL_FUN(	spell_hypothermia		);
DECLARE_SPELL_FUN(	spell_frostbite			);
DECLARE_SPELL_FUN( 	spell_imprisonvoice		);
DECLARE_SPELL_FUN(	spell_stasis_wall		);
DECLARE_SPELL_FUN(	spell_scramble_neurons	);
DECLARE_SPELL_FUN(	spell_acid_stream		);
DECLARE_SPELL_FUN(	spell_acid_vein			);
DECLARE_SPELL_FUN(  spell_corrode_lock		);
DECLARE_SPELL_FUN(	spell_attract			);
DECLARE_SPELL_FUN(	spell_grounding			);
DECLARE_SPELL_FUN(	spell_thunderclap		);
DECLARE_SPELL_FUN(	spell_neutralize		);
DECLARE_SPELL_FUN(	spell_caustic_vapor		);
DECLARE_SPELL_FUN(	spell_smokescreen		);
DECLARE_SPELL_FUN(	spell_smother			);
DECLARE_SPELL_FUN(	spell_putrid_air		);
DECLARE_SPELL_FUN(	spell_asphyxiate		);
DECLARE_SPELL_FUN(	spell_shroud_of_secrecy	);
DECLARE_SPELL_FUN(	spell_noxious_ward		);
DECLARE_SPELL_FUN(	spell_molten_stones		);
DECLARE_SPELL_FUN(	spell_heat_earth		);
DECLARE_SPELL_FUN(	spell_blanket			);
DECLARE_SPELL_FUN(	spell_boreal_wind		);
DECLARE_SPELL_FUN(	spell_concave_shell		);
DECLARE_SPELL_FUN(	spell_frost_glaze		);
DECLARE_SPELL_FUN(	spell_unbreakable		);
DECLARE_SPELL_FUN(	spell_earthsembrace		);
DECLARE_SPELL_FUN(	spell_accumulate_heat	);
DECLARE_SPELL_FUN(	spell_frigid_breeze		);
DECLARE_SPELL_FUN(	spell_whiteout			);
DECLARE_SPELL_FUN(	spell_pure_air			);
DECLARE_SPELL_FUN(	spell_icelance			);
DECLARE_SPELL_FUN(	spell_freeze_door		);
DECLARE_SPELL_FUN(	spell_frost_growth		);
DECLARE_SPELL_FUN(	spell_bind_feet			);
DECLARE_SPELL_FUN(	spell_glaciate			);
DECLARE_SPELL_FUN(	spell_hailstorm			);
DECLARE_SPELL_FUN(	spell_stalactites		);
DECLARE_SPELL_FUN( 	spell_ice_blast			);
DECLARE_SPELL_FUN(	spell_icy_carapace		);
DECLARE_SPELL_FUN(	spell_sheath_of_ice		);
DECLARE_SPELL_FUN(	spell_ironskin			);
DECLARE_SPELL_FUN(	spell_burden			);
DECLARE_SPELL_FUN(	spell_metal_shards		);
DECLARE_SPELL_FUN(	spell_fortify_weapon	);
DECLARE_SPELL_FUN(	spell_fortify_armor		);
DECLARE_SPELL_FUN(	spell_alter_metal		);
DECLARE_SPELL_FUN(	spell_cloak_of_mist		);
DECLARE_SPELL_FUN(	spell_vigorize			);
DECLARE_SPELL_FUN(	spell_creeping_tomb		);
DECLARE_SPELL_FUN(	spell_pass_without_trace);
DECLARE_SPELL_FUN(	spell_quicksand			);
DECLARE_SPELL_FUN(	spell_earthfade			);
DECLARE_SPELL_FUN(	spell_sap_endurance		);
DECLARE_SPELL_FUN(	spell_emulsify			);
DECLARE_SPELL_FUN(	spell_rust				);
DECLARE_SPELL_FUN(	spell_airy_water		);
DECLARE_SPELL_FUN(	spell_cooling_mist		);
DECLARE_SPELL_FUN(	spell_prismatic_spray	);
DECLARE_SPELL_FUN(	spell_unholy_communion	);
DECLARE_SPELL_FUN(	spell_indomitability	);
DECLARE_SPELL_FUN(	spell_mark_of_wrath		);
DECLARE_SPELL_FUN(	spell_living_blade		);
DECLARE_SPELL_FUN(	spell_wrack				);
DECLARE_SPELL_FUN(	spell_radiance			);
DECLARE_SPELL_FUN(	spell_inspire_lust		);
DECLARE_SPELL_FUN(	spell_dispaters			);
DECLARE_SPELL_FUN(	spell_baals_mastery		);
DECLARE_SPELL_FUN(	spell_dark_familiar		);
DECLARE_SPELL_FUN(	spell_word_of_command	);
DECLARE_SPELL_FUN(	spell_plasma_arc		);
DECLARE_SPELL_FUN(	spell_sphere_of_plasma	);
DECLARE_SPELL_FUN(	spell_plasma_thread		);
DECLARE_SPELL_FUN(	spell_plasma_bolt		);
DECLARE_SPELL_FUN(	spell_plasma_cube		);
DECLARE_SPELL_FUN(	spell_essence_of_plasma	);
DECLARE_SPELL_FUN(	spell_fashion_crystal	);
DECLARE_SPELL_FUN(	spell_farsee			);
DECLARE_SPELL_FUN(	spell_detonation		);
DECLARE_SPELL_FUN(	spell_rotating_ward		);
DECLARE_SPELL_FUN(	spell_mana_beam			);
DECLARE_SPELL_FUN(	spell_fortify_crystal	);
DECLARE_SPELL_FUN(	spell_melt_rock			);
DECLARE_SPELL_FUN( 	spell_magma_tunnel		);
DECLARE_SPELL_FUN(	spell_insanity			);
DECLARE_SPELL_FUN(	spell_infidels_weight	);
DECLARE_SPELL_FUN(	spell_burning_vision	);
DECLARE_SPELL_FUN(	spell_divine_malison	);
DECLARE_SPELL_FUN(	spell_mana_transfer		);
DECLARE_SPELL_FUN(	spell_scribe			);
DECLARE_SPELL_FUN(	spell_dark_insight);
DECLARE_SPELL_FUN(	spell_healing_sleep		);
DECLARE_SPELL_FUN(	spell_deny_magic		);
DECLARE_SPELL_FUN(	spell_bane				);
DECLARE_SPELL_FUN(  spell_medicine			);
DECLARE_SPELL_FUN(	spell_repose			);
DECLARE_SPELL_FUN(	spell_colorful			);
DECLARE_SPELL_FUN(	spell_rites_of_preparation);
DECLARE_SPELL_FUN(	spell_spiritual_hammer);
DECLARE_SPELL_FUN(	spell_empathy			);
DECLARE_SPELL_FUN(	spell_altruism			);
DECLARE_SPELL_FUN(	spell_tower_of_fortitude);
DECLARE_SPELL_FUN(	spell_indomitable_spirit);
DECLARE_SPELL_FUN(	spell_fervor);
DECLARE_SPELL_FUN(	spell_piety);
DECLARE_SPELL_FUN(	spell_spiritual_healing);
DECLARE_SPELL_FUN(	spell_crimson_martyr);
DECLARE_SPELL_FUN(	spell_retribution);
DECLARE_SPELL_FUN(  spell_shroud_of_light);
DECLARE_SPELL_FUN(	spell_safehaven);
DECLARE_SPELL_FUN(	spell_mana_infusion);
DECLARE_SPELL_FUN(	spell_tangleroot);
/*-----------------AFF_FUNS-----------------*/

			/* CHAR BEAT FUNS */
DECLARE_AFF_FUN (	ispirit_beat			);

			/* CHAR TICK FUNS */
DECLARE_AFF_FUN (	retribution_tick		);
DECLARE_AFF_FUN (	infusion_tick);
DECLARE_AFF_FUN	(	plague_tick				);
DECLARE_AFF_FUN	(	poison_tick				);
DECLARE_AFF_FUN	(	attract_tick			);
DECLARE_AFF_FUN	(	bleeding_tick			);
DECLARE_AFF_FUN	(	agitate_tick			);
DECLARE_AFF_FUN	(	creeping_tomb_tick		);
DECLARE_AFF_FUN (	wrack_tick				);
DECLARE_AFF_FUN	(	earthfade_tick			);
DECLARE_AFF_FUN	( 	ultradiffusion_tick		);
DECLARE_AFF_FUN (	rotating_tick			);
DECLARE_AFF_FUN	(	lesser_demon_tick		);
DECLARE_AFF_FUN	(	greater_demon_tick		);
DECLARE_AFF_FUN (	gamygyn_tick			);
DECLARE_AFF_FUN	(	orobas_tick				);
DECLARE_AFF_FUN (	burning_vision_tick		);
DECLARE_AFF_FUN (	communion_tick			);
DECLARE_AFF_FUN (	drowning_tick			);

			/* CHAR PULSE FUNS */
DECLARE_AFF_FUN (	quicksand_pulse_sink	);
DECLARE_AFF_FUN (	lust_pulse				);
DECLARE_AFF_FUN (	traitor_pulse			);
DECLARE_AFF_FUN (	sphere_of_plasma_pulse	);
DECLARE_AFF_FUN (	insanity_pulse			);
DECLARE_AFF_FUN (	disguise_pulse			);
DECLARE_AFF_FUN (	burning_pulse			);
DECLARE_AFF_FUN (	posture_pulse			);
DECLARE_AFF_FUN	(	mana_drain_pulse		);
DECLARE_AFF_FUN (	idle_pulse			);

			/* CHAR END FUNS */
DECLARE_AFF_FUN	(	ultradiffusion_end		);
DECLARE_AFF_FUN	(	talismanic_end			);
DECLARE_AFF_FUN	(	shroud_end				);
DECLARE_AFF_FUN	(	grounding_end			);
DECLARE_AFF_FUN	(	earthfade_end			);
DECLARE_AFF_FUN	(	sphere_of_plasma_end	);
DECLARE_AFF_FUN	(	plasma_thread_end		);
DECLARE_AFF_FUN	(	communion_end			);
DECLARE_AFF_FUN (	darksight_end			);
DECLARE_AFF_FUN (	insanity_end			);
DECLARE_AFF_FUN (	disguise_end			);
DECLARE_AFF_FUN (	healing_sleep_end		);
DECLARE_AFF_FUN (	waterbreath_end			);
DECLARE_AFF_FUN (	colorful_end			);
DECLARE_AFF_FUN (	ispirit_end			);
DECLARE_AFF_FUN (	empathy_end			);
DECLARE_AFF_FUN (	false_motives_end		);
DECLARE_AFF_FUN (	rprog_elven_down_end		);
/*------------------------------------------*/

			/* ROOM TICK FUNS */

			/* ROOM PULSE FUNS */
DECLARE_RAFF_FUN(	essence_of_plasma_pulse	);
DECLARE_RAFF_FUN(	conflagration_pulse		);

			/* ROOM END FUNS */
DECLARE_RAFF_FUN(	caustic_vapor_burnout	);
DECLARE_RAFF_FUN(	conflag_burnout			);
DECLARE_RAFF_FUN(	gravity_well_explode	);
DECLARE_RAFF_FUN(	vacuum_end_fun			);
DECLARE_RAFF_FUN(	flood_recede			);
DECLARE_RAFF_FUN(	riptide_one_end			);
DECLARE_RAFF_FUN(	riptide_two_end			);
DECLARE_RAFF_FUN(	smokescreen_end			);
DECLARE_RAFF_FUN(	blanket_melt			);
DECLARE_RAFF_FUN(	door_unfreeze			);
DECLARE_RAFF_FUN(	ground_thaw				);
DECLARE_RAFF_FUN(	glaciate_melt			);
DECLARE_RAFF_FUN(	quicksand_end			);
DECLARE_RAFF_FUN(	essence_of_plasma_end	);
DECLARE_RAFF_FUN(	trapdoor_end		);

/*------------------------------------------*/

			/* OBJ TICK FUNS */
DECLARE_OAFF_FUN(	crystal_tick			);

			/* OBJ PULSE FUNS */
DECLARE_OAFF_FUN(	living_blade_pulse		);
DECLARE_OAFF_FUN(	wine_pulse			);

			/* OBJ END FUNS */
DECLARE_OAFF_FUN(	immolate_end			);
DECLARE_OAFF_FUN(	acid_end				);
DECLARE_OAFF_FUN(	container_defrost		);
DECLARE_OAFF_FUN(	puddle_evaporate		);
DECLARE_OAFF_FUN(	ice_sheath_melt			);
DECLARE_OAFF_FUN(	living_blade_end		);
DECLARE_OAFF_FUN(	counterfeit_end			);
DECLARE_OAFF_FUN(	baby_end				);
DECLARE_OAFF_FUN(	baby_burp				);
DECLARE_OAFF_FUN(	rope_end				);
DECLARE_OAFF_FUN(	formian_egg_hatch		);
/*------------------------------------------*/

			/* AREA TICK FUNS */
DECLARE_AAFF_FUN(   cyclone_begin           );
DECLARE_AAFF_FUN(   cyclone_begin_tick      );

			/* AREA PULSE FUNS */

			/* AREA END FUNS */
DECLARE_AAFF_FUN(	whiteout_end			);
DECLARE_AAFF_FUN(	interference_end		);
DECLARE_AAFF_FUN(	cyclone_end_fun			);

/*------------------------------------------*/

DECLARE_RUNE_FUN(	trigger_stasis_wall		);

/* Queue function declarations */
void recall_execute args ( (CHAR_DATA *ch, ROOM_INDEX_DATA *location) );
