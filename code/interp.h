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

/* this is a listing of all the commands and command related data */

/* for command types */
#define ML 	MAX_LEVEL	/* implementor */
#define L1	MAX_LEVEL - 1  	/* creator */
#define L2	MAX_LEVEL - 2	/* supreme being */
#define L3	MAX_LEVEL - 3	/* deity */
#define L4 	MAX_LEVEL - 4	/* god */
#define L5	MAX_LEVEL - 5	/* immortal */
#define L6	MAX_LEVEL - 6	/* demigod */
#define L7	MAX_LEVEL - 7	/* angel */
#define L8	MAX_LEVEL - 8	/* avatar */
#define IM	LEVEL_IMMORTAL 	/* avatar */
#define HE	LEVEL_HERO	/* hero */

#define COM_INGORE	1


/*
 * Structure for a command in the command lookup table.
 */
struct	cmd_type
{
    char * const	name;
    DO_FUN *		do_fun;
    sh_int		position;
    sh_int		level;
    sh_int		log;
    sh_int              show;
    sh_int		hide;
    char * const	skill_name;
};

/* the command table itself */
extern	const	struct	cmd_type	cmd_table	[];

/*
 * Command functions.
 * Defined in act_*.c (mostly).
 */
DECLARE_DO_FUN( do_acute_vision	);
DECLARE_DO_FUN( do_ambush	);
DECLARE_DO_FUN(	do_addapply	);
DECLARE_DO_FUN(	do_advance	);
DECLARE_DO_FUN( do_affects	);
DECLARE_DO_FUN( do_alia		);
DECLARE_DO_FUN( do_alias	);
DECLARE_DO_FUN(	do_unban	);
DECLARE_DO_FUN( do_answer	);
DECLARE_DO_FUN(	do_areas	);
DECLARE_DO_FUN(	do_at		);
DECLARE_DO_FUN(	do_auction	);
DECLARE_DO_FUN( do_autoabort	);
DECLARE_DO_FUN( do_autoassist	);
DECLARE_DO_FUN( do_autoexit	);
DECLARE_DO_FUN( do_autogold	);
DECLARE_DO_FUN( do_autolist	);
DECLARE_DO_FUN( do_autoloot	);
DECLARE_DO_FUN( do_autosac	);
DECLARE_DO_FUN( do_autosplit	);
DECLARE_DO_FUN(	do_backstab	);
DECLARE_DO_FUN(	do_bamfin	);
DECLARE_DO_FUN(	do_bamfout	);
DECLARE_DO_FUN(	do_ban		);
DECLARE_DO_FUN( do_bash		);
DECLARE_DO_FUN( do_berserk	);
DECLARE_DO_FUN(	do_brandish	);
DECLARE_DO_FUN(	do_brands	);
DECLARE_DO_FUN( do_brief	);
DECLARE_DO_FUN(	do_bug		);
DECLARE_DO_FUN(	do_buy		);
DECLARE_DO_FUN( do_camouflage	);
DECLARE_DO_FUN( do_camp		);
DECLARE_DO_FUN(	do_cast		);
DECLARE_DO_FUN(	do_commune	);
DECLARE_DO_FUN(	do_call		);
DECLARE_DO_FUN( do_changes	);
DECLARE_DO_FUN( do_channels	);
DECLARE_DO_FUN( do_chess	);
DECLARE_DO_FUN( do_circle_stab	);
DECLARE_DO_FUN( do_clone	);
DECLARE_DO_FUN(	do_close	);
DECLARE_DO_FUN( do_color        );
DECLARE_DO_FUN(	do_commands	);
DECLARE_DO_FUN( do_combine	);
DECLARE_DO_FUN( do_compact	);
DECLARE_DO_FUN(	do_compare	);
DECLARE_DO_FUN(	do_consider	);
DECLARE_DO_FUN( do_count	);
DECLARE_DO_FUN(	do_credits	);
DECLARE_DO_FUN(	do_crumble	);
DECLARE_DO_FUN( do_deaf		);
DECLARE_DO_FUN( do_deathstyle   );
DECLARE_DO_FUN( do_delet	);
DECLARE_DO_FUN( do_delete	);
DECLARE_DO_FUN(	do_deny		);
DECLARE_DO_FUN(	do_description	);
DECLARE_DO_FUN( do_detect_hidden);
DECLARE_DO_FUN( do_diku		);
DECLARE_DO_FUN( do_dirt		);
DECLARE_DO_FUN(	do_disarm	);
DECLARE_DO_FUN( do_disengage);
DECLARE_DO_FUN(	do_disconnect	);
DECLARE_DO_FUN(	do_down		);
DECLARE_DO_FUN(	do_drink	);
DECLARE_DO_FUN(	do_drop		);
DECLARE_DO_FUN( do_dump		);
DECLARE_DO_FUN(	do_east		);
DECLARE_DO_FUN(	do_eat		);
DECLARE_DO_FUN(	do_echo		);
DECLARE_DO_FUN(	do_immecho	);
DECLARE_DO_FUN(	do_emote	);
DECLARE_DO_FUN( do_empower	);
DECLARE_DO_FUN( do_envenom	);
DECLARE_DO_FUN(	do_equipment	);
DECLARE_DO_FUN(	do_examine	);
DECLARE_DO_FUN(	do_exits	);
DECLARE_DO_FUN( do_extitle      );
DECLARE_DO_FUN(	do_fill		);
DECLARE_DO_FUN( do_flag		);
DECLARE_DO_FUN(	do_flee		);
DECLARE_DO_FUN( do_flip		);
DECLARE_DO_FUN(	do_follow	);
DECLARE_DO_FUN(	do_force	);
DECLARE_DO_FUN(	do_freeze	);
DECLARE_DO_FUN( do_gain		);
DECLARE_DO_FUN(	do_get		);
DECLARE_DO_FUN(	do_ghost		);
DECLARE_DO_FUN(	do_give		);
DECLARE_DO_FUN( do_glance       );
DECLARE_DO_FUN(	do_goto		);
DECLARE_DO_FUN(	do_group	);
DECLARE_DO_FUN( do_groups	);
DECLARE_DO_FUN(	do_gtell	);
DECLARE_DO_FUN( do_heal		);
DECLARE_DO_FUN(	do_help		);
DECLARE_DO_FUN(	do_hit		);
DECLARE_DO_FUN(	do_hide		);
DECLARE_DO_FUN(	do_holylight	);
DECLARE_DO_FUN(	do_idea		);
DECLARE_DO_FUN(	do_immtalk	);
DECLARE_DO_FUN( do_builder);
DECLARE_DO_FUN( do_incognito	);
DECLARE_DO_FUN( do_induct       );
DECLARE_DO_FUN( do_leader       );
DECLARE_DO_FUN( do_cb    );
DECLARE_DO_FUN( do_imotd	);
DECLARE_DO_FUN(	do_inventory	);
DECLARE_DO_FUN(	do_invis	);
DECLARE_DO_FUN(   do_invoke     );
DECLARE_DO_FUN(	do_kick		);
DECLARE_DO_FUN(	do_kill		);
DECLARE_DO_FUN( do_laying_hands	);
DECLARE_DO_FUN(	do_list		);
DECLARE_DO_FUN( do_load		);
DECLARE_DO_FUN(	do_lock		);
DECLARE_DO_FUN(	do_log		);
DECLARE_DO_FUN(	do_look		);
DECLARE_DO_FUN( do_lore         ); /* Lore by Detlef */
DECLARE_DO_FUN(	do_memory	);
DECLARE_DO_FUN(	do_mfind	);
DECLARE_DO_FUN(	do_mload	);
DECLARE_DO_FUN( do_mob          );
DECLARE_DO_FUN( do_mpstat       );
DECLARE_DO_FUN( do_mpdump       );
DECLARE_DO_FUN(	do_mset		);
DECLARE_DO_FUN(	do_mstat	);
DECLARE_DO_FUN(	do_mwhere	);
DECLARE_DO_FUN( do_moron	);
DECLARE_DO_FUN( do_motd		);
DECLARE_DO_FUN( do_multicheck   );
DECLARE_DO_FUN(	do_murde	);
DECLARE_DO_FUN(	do_murder	);
DECLARE_DO_FUN( do_newlock	);
DECLARE_DO_FUN( do_news		);
DECLARE_DO_FUN( do_nochannels	);
DECLARE_DO_FUN(	do_noemote	);
DECLARE_DO_FUN( do_nofollow	);
DECLARE_DO_FUN( do_noloot	);
DECLARE_DO_FUN(	do_north	);
DECLARE_DO_FUN(	do_noshout	);
DECLARE_DO_FUN( do_nosummon	);
DECLARE_DO_FUN(	do_note		);
DECLARE_DO_FUN(	do_notell	);
DECLARE_DO_FUN(	do_ofind	);
DECLARE_DO_FUN(	do_oload	);
DECLARE_DO_FUN(	do_open		);
DECLARE_DO_FUN(	do_order	);
DECLARE_DO_FUN(	do_oset		);
DECLARE_DO_FUN(	do_ostat	);
DECLARE_DO_FUN( do_outfit	);
DECLARE_DO_FUN( do_owhere	);
DECLARE_DO_FUN(	do_pardon	);
DECLARE_DO_FUN(	do_password	);
DECLARE_DO_FUN(	do_peace	);
DECLARE_DO_FUN( do_pecho	);
DECLARE_DO_FUN(	do_pick		);
DECLARE_DO_FUN( do_play		);
DECLARE_DO_FUN( do_pmote	);
DECLARE_DO_FUN( do_pour		);
DECLARE_DO_FUN(	do_practice	);
DECLARE_DO_FUN( do_praclist	);
DECLARE_DO_FUN( do_pray         );
DECLARE_DO_FUN( do_prefi	);
DECLARE_DO_FUN( do_prefix	);
DECLARE_DO_FUN( do_prompt	);
DECLARE_DO_FUN( do_protect	);
DECLARE_DO_FUN(	do_purge	);
DECLARE_DO_FUN(	do_put		);
DECLARE_DO_FUN(	do_quaff	);
DECLARE_DO_FUN( do_question	);
DECLARE_DO_FUN( do_quest	);
DECLARE_DO_FUN(	do_qui		);
DECLARE_DO_FUN( do_quiet	);
DECLARE_DO_FUN(	do_quit		);
DECLARE_DO_FUN( do_quote	);
DECLARE_DO_FUN( do_read		);
DECLARE_DO_FUN(	do_reboo	);
DECLARE_DO_FUN(	do_reboot	);
DECLARE_DO_FUN(	do_recall	);
DECLARE_DO_FUN(	do_recho	);
DECLARE_DO_FUN(	do_recite	);
DECLARE_DO_FUN(	do_remove	);
DECLARE_DO_FUN(	do_rent		);
DECLARE_DO_FUN( do_replay	);
DECLARE_DO_FUN(	do_reply	);
DECLARE_DO_FUN(	do_report	);
DECLARE_DO_FUN(	do_rescue	);
DECLARE_DO_FUN(	do_rest		);
DECLARE_DO_FUN(	do_restore	);
DECLARE_DO_FUN(	do_return	);
DECLARE_DO_FUN( do_rngtest	);
DECLARE_DO_FUN( do_roll		);
DECLARE_DO_FUN(	do_rset		);
DECLARE_DO_FUN(	do_rstat	);
DECLARE_DO_FUN( do_rules	);
DECLARE_DO_FUN(	do_sacrifice	);
DECLARE_DO_FUN(	do_save		);
DECLARE_DO_FUN(	do_say		);
DECLARE_DO_FUN( do_scan 	);
DECLARE_DO_FUN(	do_score	);
DECLARE_DO_FUN( do_scroll	);
DECLARE_DO_FUN(	do_sell		);
DECLARE_DO_FUN( do_set		);
DECLARE_DO_FUN( do_show		);
DECLARE_DO_FUN( do_sing		);
DECLARE_DO_FUN( do_sit		);
DECLARE_DO_FUN( do_skills	);
DECLARE_DO_FUN( do_skin		);
DECLARE_DO_FUN( do_skullcrush	);
DECLARE_DO_FUN(	do_sla		);
DECLARE_DO_FUN(	do_slay		);
DECLARE_DO_FUN(	do_sleep	);
DECLARE_DO_FUN(	do_slookup	);
DECLARE_DO_FUN( do_retribution  );
DECLARE_DO_FUN( do_smite        );
DECLARE_DO_FUN( do_smote	);
DECLARE_DO_FUN(	do_sneak	);
DECLARE_DO_FUN(	do_snoop	);
DECLARE_DO_FUN( do_socials	);
DECLARE_DO_FUN(	do_south	);
DECLARE_DO_FUN( do_sockets	);
DECLARE_DO_FUN( do_spells	);
DECLARE_DO_FUN(	do_split	);
DECLARE_DO_FUN(	do_sset		);
DECLARE_DO_FUN(	do_stand	);
DECLARE_DO_FUN( do_stat		);
DECLARE_DO_FUN(	do_steal	);
DECLARE_DO_FUN( do_story	);
DECLARE_DO_FUN( do_string	);
DECLARE_DO_FUN(	do_strike_of_virtue);
DECLARE_DO_FUN( do_surrender    );
DECLARE_DO_FUN(	do_switch	);
DECLARE_DO_FUN(	do_tell		);
DECLARE_DO_FUN(	do_time		);
DECLARE_DO_FUN(	do_title	);
DECLARE_DO_FUN(	do_train	);
DECLARE_DO_FUN(	do_transfer	);
DECLARE_DO_FUN( do_trip		);
DECLARE_DO_FUN( do_trunksmash	);
DECLARE_DO_FUN(	do_trust	);
DECLARE_DO_FUN( do_tsunami	);
DECLARE_DO_FUN(	do_typo		);
DECLARE_DO_FUN( do_unalias	);
DECLARE_DO_FUN(	do_unbrands	);
DECLARE_DO_FUN(	do_unlock	);
DECLARE_DO_FUN( do_unread	);
DECLARE_DO_FUN(	do_up		);
DECLARE_DO_FUN(	do_value	);
DECLARE_DO_FUN(	do_visible	);
DECLARE_DO_FUN( do_violate	);
DECLARE_DO_FUN( do_vnum		);
DECLARE_DO_FUN(	do_wake		);
DECLARE_DO_FUN(	do_wear		);
DECLARE_DO_FUN(	do_weather	);
DECLARE_DO_FUN(	do_west		);
DECLARE_DO_FUN(	do_where	);
DECLARE_DO_FUN(	do_whisper	); /* whisper -- dioxide */
DECLARE_DO_FUN(	do_who		);
DECLARE_DO_FUN( do_whois	);
DECLARE_DO_FUN(	do_wimpy	);
DECLARE_DO_FUN(	do_wizhelp	);
DECLARE_DO_FUN(	do_wizlock	);
DECLARE_DO_FUN( do_wizlist	);
DECLARE_DO_FUN( do_wiznet	);
DECLARE_DO_FUN( do_worth	);
DECLARE_DO_FUN(	do_yell		);
DECLARE_DO_FUN(	do_zap		);
DECLARE_DO_FUN( do_zecho	);
DECLARE_DO_FUN( do_olc        );
DECLARE_DO_FUN( do_asave  );
DECLARE_DO_FUN( do_alist  );
DECLARE_DO_FUN( do_resets );
DECLARE_DO_FUN( do_redit  );
DECLARE_DO_FUN( do_aedit  );
DECLARE_DO_FUN( do_medit  );
DECLARE_DO_FUN( do_oedit  );
DECLARE_DO_FUN( do_revert	);
DECLARE_DO_FUN( do_barkskin	);
DECLARE_DO_FUN( do_shadowgate	);
DECLARE_DO_FUN( do_eye_of_the_predator);
DECLARE_DO_FUN( do_animal_call);
DECLARE_DO_FUN( do_lunge	);
DECLARE_DO_FUN( do_blackjack	);
DECLARE_DO_FUN( do_force_reset	);
DECLARE_DO_FUN( do_request	);
DECLARE_DO_FUN( do_bandage	);
DECLARE_DO_FUN( do_herb		);
DECLARE_DO_FUN( do_cleave	);
DECLARE_DO_FUN( do_aura_of_sustenance);

DECLARE_DO_FUN( do_astrip	);	/* Imm command */
DECLARE_DO_FUN( do_butcher	);
DECLARE_DO_FUN( do_balance	);
DECLARE_DO_FUN( do_deposit	);
DECLARE_DO_FUN( do_withdraw	);
DECLARE_DO_FUN( do_limcounter	);
DECLARE_DO_FUN( do_vanish	);
DECLARE_DO_FUN( do_alist	);
DECLARE_DO_FUN( do_release);
DECLARE_DO_FUN( do_classes);
DECLARE_DO_FUN( do_bear_call);
DECLARE_DO_FUN( do_tail);
DECLARE_DO_FUN( do_rinfo);
DECLARE_DO_FUN( do_nerve);
DECLARE_DO_FUN( do_endure);
DECLARE_DO_FUN( do_throw);
DECLARE_DO_FUN(do_warcry);
DECLARE_DO_FUN(do_poison_dust);
DECLARE_DO_FUN(do_blindness_dust);
DECLARE_DO_FUN(do_strangle);
DECLARE_DO_FUN(do_ram);
DECLARE_DO_FUN(do_enlist);
DECLARE_DO_FUN(do_lagout);
DECLARE_DO_FUN(do_llimit);
DECLARE_DO_FUN( do_global);
DECLARE_DO_FUN( do_tame);
DECLARE_DO_FUN( do_find_water);
DECLARE_DO_FUN( do_track);
DECLARE_DO_FUN( do_shield_cleave);
DECLARE_DO_FUN(	do_group_retreat);
DECLARE_DO_FUN( do_valiant_charge);
DECLARE_DO_FUN( do_cloak);
DECLARE_DO_FUN( do_demand);
DECLARE_DO_FUN( do_shapeshift);
DECLARE_DO_FUN( do_door_bash);
DECLARE_DO_FUN( do_hometown);
DECLARE_DO_FUN(do_forage);
DECLARE_DO_FUN(do_assassinate);
DECLARE_DO_FUN(do_defend);
DECLARE_DO_FUN(do_intimidate);
DECLARE_DO_FUN(do_lash);
DECLARE_DO_FUN(do_pugil);
DECLARE_DO_FUN(do_protection_heat_cold);
DECLARE_DO_FUN(do_rally);
DECLARE_DO_FUN(do_notransfer);
DECLARE_DO_FUN(do_iron_resolve);
DECLARE_DO_FUN(do_quiet_movement);
DECLARE_DO_FUN(do_evaluation);
DECLARE_DO_FUN(do_deathmessage);
DECLARE_DO_FUN(do_max_limits);
DECLARE_DO_FUN(do_renam); /* Safety check on do_rename */
DECLARE_DO_FUN(do_rename);
DECLARE_DO_FUN(do_embalm);
DECLARE_DO_FUN(do_records);
DECLARE_DO_FUN(do_ratings);
DECLARE_DO_FUN(do_knife);
DECLARE_DO_FUN(do_disperse);
DECLARE_DO_FUN(do_impale);
DECLARE_DO_FUN(do_bind);
DECLARE_DO_FUN(do_unbind);
DECLARE_DO_FUN(do_xlook);
DECLARE_DO_FUN(do_affrem);
DECLARE_DO_FUN(do_divine_intervention);
DECLARE_DO_FUN(do_creep);
DECLARE_DO_FUN(do_ccb);
DECLARE_DO_FUN(do_gouge);
DECLARE_DO_FUN(do_stab);
DECLARE_DO_FUN(do_raffects);
DECLARE_DO_FUN(do_powers);
DECLARE_DO_FUN(do_supps);
DECLARE_DO_FUN(do_devour);
DECLARE_DO_FUN(do_strip);
DECLARE_DO_FUN(do_choke);
DECLARE_DO_FUN(do_heroimm);
DECLARE_DO_FUN(do_pay);
DECLARE_DO_FUN(do_vstat);
DECLARE_DO_FUN(do_lag);
DECLARE_DO_FUN(do_snare);
DECLARE_DO_FUN(do_bearcharge);
DECLARE_DO_FUN(do_newbie);
DECLARE_DO_FUN(do_charge);
DECLARE_DO_FUN(do_history);
DECLARE_DO_FUN(do_legsweep);
DECLARE_DO_FUN(do_weaponbreaker);
DECLARE_DO_FUN(do_cranial);
DECLARE_DO_FUN(do_entwine);
DECLARE_DO_FUN(do_whirl);
DECLARE_DO_FUN(do_finger);
DECLARE_DO_FUN(do_assess);
DECLARE_DO_FUN(do_offhand);
DECLARE_DO_FUN(do_ltrack);
DECLARE_DO_FUN(do_afk);
DECLARE_DO_FUN(do_affto);
DECLARE_DO_FUN(do_rchanges);
DECLARE_DO_FUN(do_pktrack);
DECLARE_DO_FUN(do_ctrack);
DECLARE_DO_FUN(do_bounty);
DECLARE_DO_FUN(do_topbounties);
DECLARE_DO_FUN(do_enter);
DECLARE_DO_FUN(do_revert);
DECLARE_DO_FUN(do_forms);
DECLARE_DO_FUN(do_pload);
DECLARE_DO_FUN(do_pswitch);
DECLARE_DO_FUN(do_vote);
DECLARE_DO_FUN(do_damage);
DECLARE_DO_FUN(do_listvotes);
DECLARE_DO_FUN(do_trustcabal);
DECLARE_DO_FUN(do_trustgroup);
DECLARE_DO_FUN(do_trustchar);
DECLARE_DO_FUN(do_addhelp);
DECLARE_DO_FUN(do_listhelp);
DECLARE_DO_FUN(do_blast);
DECLARE_DO_FUN(do_forcetick);
DECLARE_DO_FUN(do_role);
DECLARE_DO_FUN(do_delhelp);
DECLARE_DO_FUN(do_oldhelp);
DECLARE_DO_FUN(do_modhelp);
DECLARE_DO_FUN(do_demo);
DECLARE_DO_FUN(do_sitetrack);
DECLARE_DO_FUN(do_offer);
DECLARE_DO_FUN(do_listoffer);
DECLARE_DO_FUN(do_storytell);
DECLARE_DO_FUN(do_rastrip);
DECLARE_DO_FUN(do_aastrip);
DECLARE_DO_FUN(do_oastrip);
DECLARE_DO_FUN(do_specialize);
DECLARE_DO_FUN(do_style);
DECLARE_DO_FUN(do_proficiencies);
DECLARE_DO_FUN(do_entrap);
DECLARE_DO_FUN(do_drive);
DECLARE_DO_FUN(do_dash);
DECLARE_DO_FUN(do_rune);
DECLARE_DO_FUN(do_howl);
DECLARE_DO_FUN(do_damdice);
DECLARE_DO_FUN(do_hobble);
DECLARE_DO_FUN(do_crippling_blow);
DECLARE_DO_FUN(do_clean);
DECLARE_DO_FUN(do_gold);
DECLARE_DO_FUN(do_bleed);
DECLARE_DO_FUN(do_posture);
DECLARE_DO_FUN(do_unbalance);
DECLARE_DO_FUN(do_drain);
DECLARE_DO_FUN(do_concuss);
DECLARE_DO_FUN(do_retreat);
DECLARE_DO_FUN(do_uppercut);
DECLARE_DO_FUN(do_overhead);
DECLARE_DO_FUN(do_disrupt_formation);
DECLARE_DO_FUN(do_dart);
DECLARE_DO_FUN(do_hurl);
DECLARE_DO_FUN(do_extract);
DECLARE_DO_FUN(do_exchange);
DECLARE_DO_FUN(do_shieldbash);
DECLARE_DO_FUN(do_brace);
DECLARE_DO_FUN(do_shatter);
DECLARE_DO_FUN(do_whirlwind);
DECLARE_DO_FUN(do_leadership);
DECLARE_DO_FUN(do_uncoil);
DECLARE_DO_FUN(do_pull);
DECLARE_DO_FUN(do_exploit);
DECLARE_DO_FUN(do_outflank);
DECLARE_DO_FUN(do_taunt);
DECLARE_DO_FUN(do_devilfavor);
DECLARE_DO_FUN(do_clearfavors);
DECLARE_DO_FUN(do_consume);
DECLARE_DO_FUN(do_darksight);
DECLARE_DO_FUN(do_gore);
DECLARE_DO_FUN(do_headbutt);
DECLARE_DO_FUN(do_gore);
DECLARE_DO_FUN(do_plant);
DECLARE_DO_FUN(do_gag);
DECLARE_DO_FUN(do_ungag);
DECLARE_DO_FUN(do_drag);
DECLARE_DO_FUN(do_sign);
DECLARE_DO_FUN(do_slash);
DECLARE_DO_FUN(do_breath_mephisto);
DECLARE_DO_FUN(do_stash);
DECLARE_DO_FUN(do_disguise);
DECLARE_DO_FUN(do_undisguise);
DECLARE_DO_FUN(do_shadow_cloak);
DECLARE_DO_FUN(do_search);
DECLARE_DO_FUN(do_counterfeit);
DECLARE_DO_FUN(do_silent_movement);
DECLARE_DO_FUN(do_touch);
DECLARE_DO_FUN(do_gsnlist);
DECLARE_DO_FUN(do_clearquests);
DECLARE_DO_FUN(do_ccl);
DECLARE_DO_FUN(do_antiidle);
DECLARE_DO_FUN(do_exile);
DECLARE_DO_FUN(do_noreply);
DECLARE_DO_FUN(do_noskills);
DECLARE_DO_FUN(do_memtest);
DECLARE_DO_FUN(do_givexp);
DECLARE_DO_FUN(do_tripwire);
DECLARE_DO_FUN(do_buglist);
DECLARE_DO_FUN(do_constdump);
DECLARE_DO_FUN(do_turn_undead);
DECLARE_DO_FUN(do_setpath);
DECLARE_DO_FUN(do_racedump);
DECLARE_DO_FUN(do_interpdump);
DECLARE_DO_FUN(do_bluff);
DECLARE_DO_FUN(do_false_motives);
DECLARE_DO_FUN(do_phalanx);
DECLARE_DO_FUN(do_createcosmetic);
DECLARE_DO_FUN(do_forcewear);
DECLARE_DO_FUN(do_forceremove);

DECLARE_DO_FUN(descr_end_fun);
DECLARE_DO_FUN(addhelp_end_fun);
DECLARE_DO_FUN(modhelp_end_fun);
DECLARE_DO_FUN(comment_end_fun);
DECLARE_DO_FUN(redit_end_fun);
DECLARE_DO_FUN(buglist_end_fun);
