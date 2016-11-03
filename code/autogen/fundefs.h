typedef void (*DOFUNTYPE) (CCharacter *ch, const char *args);
struct do_fun_type{ DOFUNTYPE name; };
void do_north (CCharacter *ch, const char *args);
void do_east (CCharacter *ch, const char *args);
void do_south (CCharacter *ch, const char *args);
void do_west (CCharacter *ch, const char *args);
void do_up (CCharacter *ch, const char *args);
void do_down (CCharacter *ch, const char *args);
void do_affects (CCharacter *ch, const char *args);
void do_at (CCharacter *ch, const char *args);
void do_cast (CCharacter *ch, const char *args);
void do_call (CCharacter *ch, const char *args);
void do_commune (CCharacter *ch, const char *args);
void do_rest (CCharacter *ch, const char *args);
void do_rune (CCharacter *ch, const char *args);
void do_raffects (CCharacter *ch, const char *args);
void do_buy (CCharacter *ch, const char *args);
void do_channels (CCharacter *ch, const char *args);
void do_exits (CCharacter *ch, const char *args);
void do_get (CCharacter *ch, const char *args);
void do_goto (CCharacter *ch, const char *args);
void do_group (CCharacter *ch, const char *args);
void do_balance (CCharacter *ch, const char *args);
void do_deposit (CCharacter *ch, const char *args);
void do_ratings (CCharacter *ch, const char *args);
void do_hit (CCharacter *ch, const char *args);
void do_look (CCharacter *ch, const char *args);
void do_lore (CCharacter *ch, const char *args);
void do_leader (CCharacter *ch, const char *args);
void do_inventory (CCharacter *ch, const char *args);
void do_induct (CCharacter *ch, const char *args);
void do_kill (CCharacter *ch, const char *args);
void do_cb (CCharacter *ch, const char *args);
void do_recall (CCharacter *ch, const char *args);
void do_order (CCharacter *ch, const char *args);
void do_practice (CCharacter *ch, const char *args);
void do_pray (CCharacter *ch, const char *args);
void do_sit (CCharacter *ch, const char *args);
void do_sockets (CCharacter *ch, const char *args);
void do_extitle (CCharacter *ch, const char *args);
void do_multicheck (CCharacter *ch, const char *args);
void do_stand (CCharacter *ch, const char *args);
void do_tell (CCharacter *ch, const char *args);
void do_unlock (CCharacter *ch, const char *args);
void do_wear (CCharacter *ch, const char *args);
void do_withdraw (CCharacter *ch, const char *args);
void do_wizhelp (CCharacter *ch, const char *args);
void do_request (CCharacter *ch, const char *args);
void do_demand (CCharacter *ch, const char *args);
void do_areas (CCharacter *ch, const char *args);
void do_bug (CCharacter *ch, const char *args);
void do_changes (CCharacter *ch, const char *args);
void do_commands (CCharacter *ch, const char *args);
void do_compare (CCharacter *ch, const char *args);
void do_consider (CCharacter *ch, const char *args);
void do_count (CCharacter *ch, const char *args);
void do_credits (CCharacter *ch, const char *args);
void do_diku (CCharacter *ch, const char *args);
void do_equipment (CCharacter *ch, const char *args);
void do_examine (CCharacter *ch, const char *args);
void do_glance (CCharacter *ch, const char *args);
void do_oldhelp (CCharacter *ch, const char *args);
void do_help (CCharacter *ch, const char *args);
void do_idea (CCharacter *ch, const char *args);
void do_groups (CCharacter *ch, const char *args);
void do_motd (CCharacter *ch, const char *args);
void do_news (CCharacter *ch, const char *args);
void do_powers (CCharacter *ch, const char *args);
void do_supps (CCharacter *ch, const char *args);
void do_report (CCharacter *ch, const char *args);
void do_rules (CCharacter *ch, const char *args);
void do_score (CCharacter *ch, const char *args);
void do_skills (CCharacter *ch, const char *args);
void do_socials (CCharacter *ch, const char *args);
void do_show (CCharacter *ch, const char *args);
void do_spells (CCharacter *ch, const char *args);
void do_story (CCharacter *ch, const char *args);
void do_time (CCharacter *ch, const char *args);
void do_typo (CCharacter *ch, const char *args);
void do_vote (CCharacter *ch, const char *args);
void do_weather (CCharacter *ch, const char *args);
void do_who (CCharacter *ch, const char *args);
void do_whois (CCharacter *ch, const char *args);
void do_wizlist (CCharacter *ch, const char *args);
void do_worth (CCharacter *ch, const char *args);
void do_alias (CCharacter *ch, const char *args);
void do_autolist (CCharacter *ch, const char *args);
void do_autoabort (CCharacter *ch, const char *args);
void do_autoassist (CCharacter *ch, const char *args);
void do_autoexit (CCharacter *ch, const char *args);
void do_autogold (CCharacter *ch, const char *args);
void do_autoloot (CCharacter *ch, const char *args);
void do_autosac (CCharacter *ch, const char *args);
void do_autosplit (CCharacter *ch, const char *args);
void do_brief (CCharacter *ch, const char *args);
void do_chess (CCharacter *ch, const char *args);
void do_color (CCharacter *ch, const char *args);
void do_combine (CCharacter *ch, const char *args);
void do_compact (CCharacter *ch, const char *args);
void do_description (CCharacter *ch, const char *args);
void do_delete (CCharacter *ch, const char *args);
void do_nofollow (CCharacter *ch, const char *args);
void do_nosummon (CCharacter *ch, const char *args);
void do_outfit (CCharacter *ch, const char *args);
void do_password (CCharacter *ch, const char *args);
void do_prompt (CCharacter *ch, const char *args);
void do_role (CCharacter *ch, const char *args);
void do_scroll (CCharacter *ch, const char *args);
void do_specialize (CCharacter *ch, const char *args);
void do_title (CCharacter *ch, const char *args);
void do_unalias (CCharacter *ch, const char *args);
void do_wimpy (CCharacter *ch, const char *args);
void do_deaf (CCharacter *ch, const char *args);
void do_emote (CCharacter *ch, const char *args);
void do_pmote (CCharacter *ch, const char *args);
void do_gtell (CCharacter *ch, const char *args);
void do_builder (CCharacter *ch, const char *args);
void do_immtalk (CCharacter *ch, const char *args);
void do_note (CCharacter *ch, const char *args);
void do_noreply (CCharacter *ch, const char *args);
void do_newbie (CCharacter *ch, const char *args);
void do_reply (CCharacter *ch, const char *args);
void do_replay (CCharacter *ch, const char *args);
void do_say (CCharacter *ch, const char *args);
void do_sing (CCharacter *ch, const char *args);
void do_whisper (CCharacter *ch, const char *args);
void do_unread (CCharacter *ch, const char *args);
void do_yell (CCharacter *ch, const char *args);
void do_brandish (CCharacter *ch, const char *args);
void do_close (CCharacter *ch, const char *args);
void do_drink (CCharacter *ch, const char *args);
void do_drop (CCharacter *ch, const char *args);
void do_embalm (CCharacter *ch, const char *args);
void do_eat (CCharacter *ch, const char *args);
void do_envenom (CCharacter *ch, const char *args);
void do_fill (CCharacter *ch, const char *args);
void do_give (CCharacter *ch, const char *args);
void do_heal (CCharacter *ch, const char *args);
void do_list (CCharacter *ch, const char *args);
void do_lock (CCharacter *ch, const char *args);
void do_open (CCharacter *ch, const char *args);
void do_pick (CCharacter *ch, const char *args);
void do_pour (CCharacter *ch, const char *args);
void do_put (CCharacter *ch, const char *args);
void do_quaff (CCharacter *ch, const char *args);
void do_quiet (CCharacter *ch, const char *args);
void do_recite (CCharacter *ch, const char *args);
void do_remove (CCharacter *ch, const char *args);
void do_roll (CCharacter *ch, const char *args);
void do_door_bash (CCharacter *ch, const char *args);
void do_headbutt (CCharacter *ch, const char *args);
void do_silent_movement (CCharacter *ch, const char *args);
void do_gore (CCharacter *ch, const char *args);
void do_sell (CCharacter *ch, const char *args);
void do_sacrifice (CCharacter *ch, const char *args);
void do_butcher (CCharacter *ch, const char *args);
void do_skin (CCharacter *ch, const char *args);
void do_value (CCharacter *ch, const char *args);
void do_listvotes (CCharacter *ch, const char *args);
void do_zap (CCharacter *ch, const char *args);
void do_vanish (CCharacter *ch, const char *args);
void do_aura_of_sustenance (CCharacter *ch, const char *args);
void do_wake (CCharacter *ch, const char *args);
void do_style (CCharacter *ch, const char *args);
void do_cleave (CCharacter *ch, const char *args);
void do_herb (CCharacter *ch, const char *args);
void do_records (CCharacter *ch, const char *args);
void do_ambush (CCharacter *ch, const char *args);
void do_pugil (CCharacter *ch, const char *args);
void do_lash (CCharacter *ch, const char *args);
void do_shield_cleave (CCharacter *ch, const char *args);
void do_backstab (CCharacter *ch, const char *args);
void do_knife (CCharacter *ch, const char *args);
void do_bash (CCharacter *ch, const char *args);
void do_throw (CCharacter *ch, const char *args);
void do_strangle (CCharacter *ch, const char *args);
void do_blindness_dust (CCharacter *ch, const char *args);
void do_poison_dust (CCharacter *ch, const char *args);
void do_warcry (CCharacter *ch, const char *args);
void do_endure (CCharacter *ch, const char *args);
void do_nerve (CCharacter *ch, const char *args);
void do_tail (CCharacter *ch, const char *args);
void do_blackjack (CCharacter *ch, const char *args);
void do_berserk (CCharacter *ch, const char *args);
void do_circle_stab (CCharacter *ch, const char *args);
void do_darksight (CCharacter *ch, const char *args);
void do_detect_hidden (CCharacter *ch, const char *args);
void do_dirt (CCharacter *ch, const char *args);
void do_disarm (CCharacter *ch, const char *args);
void do_disengage (CCharacter *ch, const char *args);
void do_flee (CCharacter *ch, const char *args);
void do_kick (CCharacter *ch, const char *args);
void do_murder (CCharacter *ch, const char *args);
void do_rescue (CCharacter *ch, const char *args);
void do_trip (CCharacter *ch, const char *args);
void do_defend (CCharacter *ch, const char *args);
void do_assassinate (CCharacter *ch, const char *args);
void do_forage (CCharacter *ch, const char *args);
void do_protection_heat_cold (CCharacter *ch, const char *args);
void do_intimidate (CCharacter *ch, const char *args);
void do_bind (CCharacter *ch, const char *args);
void do_unbind (CCharacter *ch, const char *args);
void do_charge (CCharacter *ch, const char *args);
void do_offhand (CCharacter *ch, const char *args);
void do_enter (CCharacter *ch, const char *args);
void do_storytell (CCharacter *ch, const char *args);
void do_entrap (CCharacter *ch, const char *args);
void do_drive (CCharacter *ch, const char *args);
void do_dash (CCharacter *ch, const char *args);
void do_howl (CCharacter *ch, const char *args);
void do_hobble (CCharacter *ch, const char *args);
void do_crippling_blow (CCharacter *ch, const char *args);
void do_gouge (CCharacter *ch, const char *args);
void do_bleed (CCharacter *ch, const char *args);
void do_posture (CCharacter *ch, const char *args);
void do_unbalance (CCharacter *ch, const char *args);
void do_drain (CCharacter *ch, const char *args);
void do_concuss (CCharacter *ch, const char *args);
void do_retreat (CCharacter *ch, const char *args);
void do_overhead (CCharacter *ch, const char *args);
void do_uppercut (CCharacter *ch, const char *args);
void do_disrupt (CCharacter *ch, const char *args);
void do_dart (CCharacter *ch, const char *args);
void do_hurl (CCharacter *ch, const char *args);
void do_extract (CCharacter *ch, const char *args);
void do_exchange (CCharacter *ch, const char *args);
void do_shieldbash (CCharacter *ch, const char *args);
void do_brace (CCharacter *ch, const char *args);
void do_shatter (CCharacter *ch, const char *args);
void do_whirlwind (CCharacter *ch, const char *args);
void do_leadership (CCharacter *ch, const char *args);
void do_uncoil (CCharacter *ch, const char *args);
void do_entwine (CCharacter *ch, const char *args);
void do_pull (CCharacter *ch, const char *args);
void do_assess (CCharacter *ch, const char *args);
void do_exploit (CCharacter *ch, const char *args);
void do_outflank (CCharacter *ch, const char *args);
void do_impale (CCharacter *ch, const char *args);
void do_valiant_charge (CCharacter *ch, const char *args);
void do_turn_undead (CCharacter *ch, const char *args);
void do_strike_of_virtue (CCharacter *ch, const char *args);
void do_group_retreat (CCharacter *ch, const char *args);
void do_flip (CCharacter *ch, const char *args);
void do_follow (CCharacter *ch, const char *args);
void do_creep (CCharacter *ch, const char *args);
void do_scan (CCharacter *ch, const char *args);
void do_gain (CCharacter *ch, const char *args);
void do_acute_vision (CCharacter *ch, const char *args);
void do_barkskin (CCharacter *ch, const char *args);
void do_animal_call (CCharacter *ch, const char *args);
void do_enlist (CCharacter *ch, const char *args);
void do_camouflage (CCharacter *ch, const char *args);
void do_camp (CCharacter *ch, const char *args);
void do_hide (CCharacter *ch, const char *args);
void do_laying_hands (CCharacter *ch, const char *args);
void do_quit (CCharacter *ch, const char *args);
void do_release (CCharacter *ch, const char *args);
void do_tame (CCharacter *ch, const char *args);
void do_find_water (CCharacter *ch, const char *args);
void do_fw (CCharacter *ch, const char *args);
void do_rent (CCharacter *ch, const char *args);
void do_save (CCharacter *ch, const char *args);
void do_sleep (CCharacter *ch, const char *args);
void do_sneak (CCharacter *ch, const char *args);
void do_split (CCharacter *ch, const char *args);
void do_steal (CCharacter *ch, const char *args);
void do_plant (CCharacter *ch, const char *args);
void do_gag (CCharacter *ch, const char *args);
void do_ungag (CCharacter *ch, const char *args);
void do_drag (CCharacter *ch, const char *args);
void do_sign (CCharacter *ch, const char *args);
void do_slash (CCharacter *ch, const char *args);
void do_tripwire (CCharacter *ch, const char *args);
void do_stash (CCharacter *ch, const char *args);
void do_disguise (CCharacter *ch, const char *args);
void do_undisguise (CCharacter *ch, const char *args);
void do_shadow_cloak (CCharacter *ch, const char *args);
void do_search (CCharacter *ch, const char *args);
void do_counterfeit (CCharacter *ch, const char *args);
void do_strip (CCharacter *ch, const char *args);
void do_train (CCharacter *ch, const char *args);
void do_visible (CCharacter *ch, const char *args);
void do_where (CCharacter *ch, const char *args);
void do_cloak (CCharacter *ch, const char *args);
void do_hometown (CCharacter *ch, const char *args);
void do_invoke (CCharacter *ch, const char *args);
void do_snare (CCharacter *ch, const char *args);
void do_bear_call (CCharacter *ch, const char *args);
void do_bounty (CCharacter *ch, const char *args);
void do_topbounties (CCharacter *ch, const char *args);
void do_offer (CCharacter *ch, const char *args);
void do_listoffer (CCharacter *ch, const char *args);
void do_taunt (CCharacter *ch, const char *args);
void do_consume (CCharacter *ch, const char *args);
void do_breath_mephisto (CCharacter *ch, const char *args);
void do_touch (CCharacter *ch, const char *args);
void do_exile (CCharacter *ch, const char *args);
void do_trustgroup (CCharacter *ch, const char *args);
void do_trustcabal (CCharacter *ch, const char *args);
void do_trustall (CCharacter *ch, const char *args);
void do_advance (CCharacter *ch, const char *args);
void do_alist (CCharacter *ch, const char *args);
void do_dump (CCharacter *ch, const char *args);
void do_immtrust (CCharacter *ch, const char *args);
void do_violate (CCharacter *ch, const char *args);
void do_allow (CCharacter *ch, const char *args);
void do_ban (CCharacter *ch, const char *args);
void do_rinfo (CCharacter *ch, const char *args);
void do_deny (CCharacter *ch, const char *args);
void do_disconnect (CCharacter *ch, const char *args);
void do_flag (CCharacter *ch, const char *args);
void do_freeze (CCharacter *ch, const char *args);
void do_lag (CCharacter *ch, const char *args);
void do_global (CCharacter *ch, const char *args);
void do_smite (CCharacter *ch, const char *args);
void do_permban (CCharacter *ch, const char *args);
void do_protect (CCharacter *ch, const char *args);
void do_reboot (CCharacter *ch, const char *args);
void do_set (CCharacter *ch, const char *args);
void do_wizlock (CCharacter *ch, const char *args);
void do_classes (CCharacter *ch, const char *args);
void do_force (CCharacter *ch, const char *args);
void do_load (CCharacter *ch, const char *args);
void do_newlock (CCharacter *ch, const char *args);
void do_nochannels (CCharacter *ch, const char *args);
void do_noemote (CCharacter *ch, const char *args);
void do_noshout (CCharacter *ch, const char *args);
void do_notell (CCharacter *ch, const char *args);
void do_pecho (CCharacter *ch, const char *args);
void do_pardon (CCharacter *ch, const char *args);
void do_purge (CCharacter *ch, const char *args);
void do_restore (CCharacter *ch, const char *args);
void do_slay (CCharacter *ch, const char *args);
void do_transfer (CCharacter *ch, const char *args);
void do_bamfin (CCharacter *ch, const char *args);
void do_addapply (CCharacter *ch, const char *args);
void do_max_limits (CCharacter *ch, const char *args);
void do_bamfout (CCharacter *ch, const char *args);
void do_force_reset (CCharacter *ch, const char *args);
void do_gecho (CCharacter *ch, const char *args);
void do_astrip (CCharacter *ch, const char *args);
void do_brands (CCharacter *ch, const char *args);
void do_unbrands (CCharacter *ch, const char *args);
void do_holylight (CCharacter *ch, const char *args);
void do_incognito (CCharacter *ch, const char *args);
void do_invis (CCharacter *ch, const char *args);
void do_log (CCharacter *ch, const char *args);
void do_memory (CCharacter *ch, const char *args);
void do_mwhere (CCharacter *ch, const char *args);
void do_owhere (CCharacter *ch, const char *args);
void do_peace (CCharacter *ch, const char *args);
void do_praclist (CCharacter *ch, const char *args);
void do_ghost (CCharacter *ch, const char *args);
void do_recho (CCharacter *ch, const char *args);
void do_return (CCharacter *ch, const char *args);
void do_snoop (CCharacter *ch, const char *args);
void do_stat (CCharacter *ch, const char *args);
void do_string (CCharacter *ch, const char *args);
void do_switch (CCharacter *ch, const char *args);
void do_vnum (CCharacter *ch, const char *args);
void do_vstat (CCharacter *ch, const char *args);
void do_zecho (CCharacter *ch, const char *args);
void do_clone (CCharacter *ch, const char *args);
void do_wiznet (CCharacter *ch, const char *args);
void do_imotd (CCharacter *ch, const char *args);
void do_rename (CCharacter *ch, const char *args);
void do_smote (CCharacter *ch, const char *args);
void do_prefix (CCharacter *ch, const char *args);
void do_bandage (CCharacter *ch, const char *args);
void do_xlook (CCharacter *ch, const char *args);
void do_affrem (CCharacter *ch, const char *args);
void do_divine_intervention (CCharacter *ch, const char *args);
void do_ccb (CCharacter *ch, const char *args);
void do_heroimm (CCharacter *ch, const char *args);
void do_history (CCharacter *ch, const char *args);
void do_finger (CCharacter *ch, const char *args);
void do_otrack (CCharacter *ch, const char *args);
void do_ltrack (CCharacter *ch, const char *args);
void do_sitetrack (CCharacter *ch, const char *args);
void do_afk (CCharacter *ch, const char *args);
void do_affto (CCharacter *ch, const char *args);
void do_crumble (CCharacter *ch, const char *args);
void do_empower (CCharacter *ch, const char *args);
void do_pktrack (CCharacter *ch, const char *args);
void do_ctrack (CCharacter *ch, const char *args);
void do_pload (CCharacter *ch, const char *args);
void do_listhelp (CCharacter *ch, const char *args);
void do_addhelp (CCharacter *ch, const char *args);
void do_delhelp (CCharacter *ch, const char *args);
void do_modhelp (CCharacter *ch, const char *args);
void do_damage (CCharacter *ch, const char *args);
void do_forcetick (CCharacter *ch, const char *args);
void do_demo (CCharacter *ch, const char *args);
void do_deathmessage (CCharacter *ch, const char *args);
void do_rastrip (CCharacter *ch, const char *args);
void do_oastrip (CCharacter *ch, const char *args);
void do_aastrip (CCharacter *ch, const char *args);
void do_damdice (CCharacter *ch, const char *args);
void do_rngtest (CCharacter *ch, const char *args);
void do_clean (CCharacter *ch, const char *args);
void do_gold (CCharacter *ch, const char *args);
void do_devilfavor (CCharacter *ch, const char *args);
void do_clearfavors (CCharacter *ch, const char *args);
void do_gsnlist (CCharacter *ch, const char *args);
void do_clearquests (CCharacter *ch, const char *args);
void do_ccl (CCharacter *ch, const char *args);
void do_antiidle (CCharacter *ch, const char *args);
void do_noskills (CCharacter *ch, const char *args);
void do_givexp (CCharacter *ch, const char *args);
void do_buglist (CCharacter *ch, const char *args);
void do_memtest (CCharacter *ch, const char *args);
void do_constdump (CCharacter *ch, const char *args);
void do_setpath (CCharacter *ch, const char *args);
void do_racedump (CCharacter *ch, const char *args);
void do_interpdump (CCharacter *ch, const char *args);
const struct do_fun_type do_fun_table[] =
{
	{ do_north },
	{ do_east },
	{ do_south },
	{ do_west },
	{ do_up },
	{ do_down },
	{ do_affects },
	{ do_at },
	{ do_cast },
	{ do_call },
	{ do_commune },
	{ do_rest },
	{ do_rune },
	{ do_raffects },
	{ do_buy },
	{ do_channels },
	{ do_exits },
	{ do_get },
	{ do_goto },
	{ do_group },
	{ do_balance },
	{ do_deposit },
	{ do_ratings },
	{ do_hit },
	{ do_look },
	{ do_lore },
	{ do_leader },
	{ do_inventory },
	{ do_induct },
	{ do_kill },
	{ do_cb },
	{ do_recall },
	{ do_order },
	{ do_practice },
	{ do_pray },
	{ do_sit },
	{ do_sockets },
	{ do_extitle },
	{ do_multicheck },
	{ do_stand },
	{ do_tell },
	{ do_unlock },
	{ do_wear },
	{ do_withdraw },
	{ do_wizhelp },
	{ do_request },
	{ do_demand },
	{ do_areas },
	{ do_bug },
	{ do_changes },
	{ do_commands },
	{ do_compare },
	{ do_consider },
	{ do_count },
	{ do_credits },
	{ do_diku },
	{ do_equipment },
	{ do_examine },
	{ do_glance },
	{ do_oldhelp },
	{ do_help },
	{ do_idea },
	{ do_groups },
	{ do_motd },
	{ do_news },
	{ do_powers },
	{ do_supps },
	{ do_report },
	{ do_rules },
	{ do_score },
	{ do_skills },
	{ do_socials },
	{ do_show },
	{ do_spells },
	{ do_story },
	{ do_time },
	{ do_typo },
	{ do_vote },
	{ do_weather },
	{ do_who },
	{ do_whois },
	{ do_wizlist },
	{ do_worth },
	{ do_alias },
	{ do_autolist },
	{ do_autoabort },
	{ do_autoassist },
	{ do_autoexit },
	{ do_autogold },
	{ do_autoloot },
	{ do_autosac },
	{ do_autosplit },
	{ do_brief },
	{ do_chess },
	{ do_color },
	{ do_combine },
	{ do_compact },
	{ do_description },
	{ do_delete },
	{ do_nofollow },
	{ do_nosummon },
	{ do_outfit },
	{ do_password },
	{ do_prompt },
	{ do_role },
	{ do_scroll },
	{ do_specialize },
	{ do_title },
	{ do_unalias },
	{ do_wimpy },
	{ do_deaf },
	{ do_emote },
	{ do_pmote },
	{ do_gtell },
	{ do_builder },
	{ do_immtalk },
	{ do_note },
	{ do_noreply },
	{ do_newbie },
	{ do_reply },
	{ do_replay },
	{ do_say },
	{ do_sing },
	{ do_whisper },
	{ do_unread },
	{ do_yell },
	{ do_brandish },
	{ do_close },
	{ do_drink },
	{ do_drop },
	{ do_embalm },
	{ do_eat },
	{ do_envenom },
	{ do_fill },
	{ do_give },
	{ do_heal },
	{ do_list },
	{ do_lock },
	{ do_open },
	{ do_pick },
	{ do_pour },
	{ do_put },
	{ do_quaff },
	{ do_quiet },
	{ do_recite },
	{ do_remove },
	{ do_roll },
	{ do_door_bash },
	{ do_headbutt },
	{ do_silent_movement },
	{ do_gore },
	{ do_sell },
	{ do_sacrifice },
	{ do_butcher },
	{ do_skin },
	{ do_value },
	{ do_listvotes },
	{ do_zap },
	{ do_vanish },
	{ do_aura_of_sustenance },
	{ do_wake },
	{ do_style },
	{ do_cleave },
	{ do_herb },
	{ do_records },
	{ do_ambush },
	{ do_pugil },
	{ do_lash },
	{ do_shield_cleave },
	{ do_backstab },
	{ do_knife },
	{ do_bash },
	{ do_throw },
	{ do_strangle },
	{ do_blindness_dust },
	{ do_poison_dust },
	{ do_warcry },
	{ do_endure },
	{ do_nerve },
	{ do_tail },
	{ do_blackjack },
	{ do_berserk },
	{ do_circle_stab },
	{ do_darksight },
	{ do_detect_hidden },
	{ do_dirt },
	{ do_disarm },
	{ do_disengage },
	{ do_flee },
	{ do_kick },
	{ do_murder },
	{ do_rescue },
	{ do_trip },
	{ do_defend },
	{ do_assassinate },
	{ do_forage },
	{ do_protection_heat_cold },
	{ do_intimidate },
	{ do_bind },
	{ do_unbind },
	{ do_charge },
	{ do_offhand },
	{ do_enter },
	{ do_storytell },
	{ do_entrap },
	{ do_drive },
	{ do_dash },
	{ do_howl },
	{ do_hobble },
	{ do_crippling_blow },
	{ do_gouge },
	{ do_bleed },
	{ do_posture },
	{ do_unbalance },
	{ do_drain },
	{ do_concuss },
	{ do_retreat },
	{ do_overhead },
	{ do_uppercut },
	{ do_disrupt },
	{ do_dart },
	{ do_hurl },
	{ do_extract },
	{ do_exchange },
	{ do_shieldbash },
	{ do_brace },
	{ do_shatter },
	{ do_whirlwind },
	{ do_leadership },
	{ do_uncoil },
	{ do_entwine },
	{ do_pull },
	{ do_assess },
	{ do_exploit },
	{ do_outflank },
	{ do_impale },
	{ do_valiant_charge },
	{ do_turn_undead },
	{ do_strike_of_virtue },
	{ do_group_retreat },
	{ do_flip },
	{ do_follow },
	{ do_creep },
	{ do_scan },
	{ do_gain },
	{ do_acute_vision },
	{ do_barkskin },
	{ do_animal_call },
	{ do_enlist },
	{ do_camouflage },
	{ do_camp },
	{ do_hide },
	{ do_laying_hands },
	{ do_quit },
	{ do_release },
	{ do_tame },
	{ do_find_water },
	{ do_fw },
	{ do_rent },
	{ do_save },
	{ do_sleep },
	{ do_sneak },
	{ do_split },
	{ do_steal },
	{ do_plant },
	{ do_gag },
	{ do_ungag },
	{ do_drag },
	{ do_sign },
	{ do_slash },
	{ do_tripwire },
	{ do_stash },
	{ do_disguise },
	{ do_undisguise },
	{ do_shadow_cloak },
	{ do_search },
	{ do_counterfeit },
	{ do_strip },
	{ do_train },
	{ do_visible },
	{ do_where },
	{ do_cloak },
	{ do_hometown },
	{ do_invoke },
	{ do_snare },
	{ do_bear_call },
	{ do_bounty },
	{ do_topbounties },
	{ do_offer },
	{ do_listoffer },
	{ do_taunt },
	{ do_consume },
	{ do_breath_mephisto },
	{ do_touch },
	{ do_exile },
	{ do_trustgroup },
	{ do_trustcabal },
	{ do_trustall },
	{ do_advance },
	{ do_alist },
	{ do_dump },
	{ do_immtrust },
	{ do_violate },
	{ do_allow },
	{ do_ban },
	{ do_rinfo },
	{ do_deny },
	{ do_disconnect },
	{ do_flag },
	{ do_freeze },
	{ do_lag },
	{ do_global },
	{ do_smite },
	{ do_permban },
	{ do_protect },
	{ do_reboot },
	{ do_set },
	{ do_wizlock },
	{ do_classes },
	{ do_force },
	{ do_load },
	{ do_newlock },
	{ do_nochannels },
	{ do_noemote },
	{ do_noshout },
	{ do_notell },
	{ do_pecho },
	{ do_pardon },
	{ do_purge },
	{ do_restore },
	{ do_slay },
	{ do_transfer },
	{ do_bamfin },
	{ do_addapply },
	{ do_max_limits },
	{ do_bamfout },
	{ do_force_reset },
	{ do_gecho },
	{ do_astrip },
	{ do_brands },
	{ do_unbrands },
	{ do_holylight },
	{ do_incognito },
	{ do_invis },
	{ do_log },
	{ do_memory },
	{ do_mwhere },
	{ do_owhere },
	{ do_peace },
	{ do_praclist },
	{ do_ghost },
	{ do_recho },
	{ do_return },
	{ do_snoop },
	{ do_stat },
	{ do_string },
	{ do_switch },
	{ do_vnum },
	{ do_vstat },
	{ do_zecho },
	{ do_clone },
	{ do_wiznet },
	{ do_imotd },
	{ do_rename },
	{ do_smote },
	{ do_prefix },
	{ do_bandage },
	{ do_xlook },
	{ do_affrem },
	{ do_divine_intervention },
	{ do_ccb },
	{ do_heroimm },
	{ do_history },
	{ do_finger },
	{ do_otrack },
	{ do_ltrack },
	{ do_sitetrack },
	{ do_afk },
	{ do_affto },
	{ do_crumble },
	{ do_empower },
	{ do_pktrack },
	{ do_ctrack },
	{ do_pload },
	{ do_listhelp },
	{ do_addhelp },
	{ do_delhelp },
	{ do_modhelp },
	{ do_damage },
	{ do_forcetick },
	{ do_demo },
	{ do_deathmessage },
	{ do_rastrip },
	{ do_oastrip },
	{ do_aastrip },
	{ do_damdice },
	{ do_rngtest },
	{ do_clean },
	{ do_gold },
	{ do_devilfavor },
	{ do_clearfavors },
	{ do_gsnlist },
	{ do_clearquests },
	{ do_ccl },
	{ do_antiidle },
	{ do_noskills },
	{ do_givexp },
	{ do_buglist },
	{ do_memtest },
	{ do_constdump },
	{ do_setpath },
	{ do_racedump },
	{ do_interpdump },
};
