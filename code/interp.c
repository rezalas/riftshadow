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

#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "interp.h"
#include "ban.h"
#include "handler.h"
#include "magic.h"
#include "tables.h"
#include "spec.h"
#include "act_comm.h"
#include "act_ente.h"
#include "act_move.h"
#include "act_obj.h"
#include "devextra.h"
#include "vote.h"
#include "skills.h"
#include "comm.h"
#include "act_wiz.h"
#include "fight.h"
#include "db.h"
#include "db2.h"
#include "note.h"
#include "olc.h"
#include "olc_act.h"
#include "olc_save.h"
#include "magic.h"
#include "heal.h"
#include "tattoo.h"
#include "quest.h"
#include "flags.h"
#include "scan.h"
#include "act_info.h"
#include "act_wiz.h"
#include "iprog.h"
#include "const.h"
#include "utility.h"
#include "ap.h"
#include "paladin.h"
#include "cabal.h"
#include "necro.h"
#include "help.h"
#include "warrior.h"
#include "misc.h"
#include "dioextra.h"
#include "chrono.h"
#include "thief.h"
#include "update.h"
#include "newmem.h"
#include "lookup.h"
#include "./include/fmt/format.h"

#define COM_INGORE		1

/*
 * Command logging types.
 */
#define LOG_NORMAL		0
#define LOG_ALWAYS		1
#define LOG_NEVER		2

#define LAST_COMMAND 	"lastcommand.txt"

bool command_result= false;
char *command_line;

/*
 * Log-all switch.
 */
bool fLogAll= false;

/*
 * Command table.
 */
const struct cmd_type cmd_table[] = {
	/*
	 * Common movement commands.
	 */
	{"north", do_north, POS_STANDING, 0, LOG_NEVER, 0, 0, "none"},
	{"east", do_east, POS_STANDING, 0, LOG_NEVER, 0, 0, "none"},
	{"south", do_south, POS_STANDING, 0, LOG_NEVER, 0, 0, "none"},
	{"west", do_west, POS_STANDING, 0, LOG_NEVER, 0, 0, "none"},
	{"up", do_up, POS_STANDING, 0, LOG_NEVER, 0, 0, "none"},
	{"down", do_down, POS_STANDING, 0, LOG_NEVER, 0, 0, "none"},

	/*
	 * Common other commands.
	 * Placed here so one and two letter abbreviations work.
	 */
	{"affects", do_affects, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"at", do_at, POS_DEAD, L6, LOG_NORMAL, 1, 0, "none"},
	{"cast", do_cast, POS_FIGHTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"call", do_call, POS_FIGHTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"commune", do_commune, POS_FIGHTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"rest", do_rest, POS_SLEEPING, 0, LOG_NORMAL, 1, 0, "none"},
	{"rune", do_rune, POS_FIGHTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"raffects", do_raffects, POS_DEAD, L6, LOG_NORMAL, 1, 0, "none"},
	{"buy", do_buy, POS_RESTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"channels", do_channels, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"exits", do_exits, POS_RESTING, 0, LOG_NORMAL, 1, 0, "none"},
	{"get", do_get, POS_RESTING, 0, LOG_NORMAL, 1, 0, "none"},
	{"goto", do_goto, POS_DEAD, L8, LOG_NORMAL, 1, 0, "none"},
	{"group", do_group, POS_SLEEPING, 0, LOG_NORMAL, 1, 0, "none"},
	{"balance", do_balance, POS_RESTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"deposit", do_deposit, POS_RESTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"ratings", do_ratings, POS_DEAD, L5, LOG_NORMAL, 1, 0, "none"},
	{"hit", do_hit, POS_FIGHTING, 0, LOG_NORMAL, 0, 1, "none"},
	{"look", do_look, POS_RESTING, 0, LOG_NORMAL, 1, 0, "none"},
	{"lore", do_lore, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"cleader", do_leader, POS_DEAD, L2, LOG_ALWAYS, 1, 0, "none"},
	{"i", do_inventory, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"in", do_inventory, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"inventory", do_inventory, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"induct", do_induct, POS_DEAD, 0, LOG_ALWAYS, 1, 0, "none"},
	{"kill", do_kill, POS_FIGHTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"cb", do_cb, POS_SLEEPING, 0, LOG_NORMAL, 1, 0, "none"},
	{"/", do_recall, POS_STANDING, 0, LOG_NORMAL, 1, 1, "none"},
	{"order", do_order, POS_RESTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"practice", do_practice, POS_SLEEPING, 0, LOG_NORMAL, 1, 0, "none"},
	{"pray", do_pray, POS_DEAD, 0, LOG_ALWAYS, 1, 0, "none"},
	{"sit", do_sit, POS_SLEEPING, 0, LOG_NORMAL, 1, 0, "none"},
	{"sockets", do_sockets, POS_DEAD, L6, LOG_NORMAL, 1, 0, "none"},
	{"extitle", do_extitle, POS_DEAD, L5, LOG_NORMAL, 1, 0, "none"},
	{"multicheck", do_multicheck, POS_DEAD, L6, LOG_NORMAL, 1, 0, "none"},
	{"stand", do_stand, POS_SLEEPING, 0, LOG_NORMAL, 1, 0, "none"},
	{"tell", do_tell, POS_RESTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"unlock", do_unlock, POS_RESTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"wield", do_wear, POS_RESTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"withdraw", do_withdraw, POS_RESTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"wizhelp", do_wizhelp, POS_DEAD, IM, LOG_NORMAL, 1, 0, "none"},
	{"request", do_request, POS_STANDING, 0, LOG_NORMAL, 1, 1, "none"},
	{"demand", do_demand, POS_STANDING, 0, LOG_NORMAL, 1, 1, "none"},
	/*
	 * Informational commands.
	 */
	{"areas", do_areas, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"bug", do_bug, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"changes", do_changes, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"commands", do_commands, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"compare", do_compare, POS_RESTING, 0, LOG_NORMAL, 1, 0, "none"},
	{"consider", do_consider, POS_RESTING, 0, LOG_NORMAL, 1, 0, "none"},
	{"count", do_count, POS_SLEEPING, 0, LOG_NORMAL, 1, 0, "none"},
	{"creds", do_credits, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"credits", do_diku, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"equipment", do_equipment, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	//    { "examine",	do_examine,	POS_RESTING,	 0,  LOG_NORMAL, 1, 0, "none" },
	{"glance", do_glance, POS_RESTING, 0, LOG_NORMAL, 1, 0, "none"},
	/*  { "groups",		do_groups,	POS_SLEEPING,	 0,  LOG_NORMAL, 1, 0, "none" }, */
	{"oldhelp", do_oldhelp, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"help", do_help, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"idea", do_idea, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"info", do_groups, POS_SLEEPING, 0, LOG_NORMAL, 1, 0, "none"},
	{"motd", do_motd, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"news", do_news, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"powers", do_powers, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"supplications", do_supps, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"read", do_look, POS_RESTING, 0, LOG_NORMAL, 1, 0, "none"},
	{"report", do_report, POS_RESTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"rules", do_rules, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"score", do_score, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"skills", do_skills, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"socials", do_socials, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"show", do_show, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"spells", do_spells, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"story", do_story, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"time", do_time, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"typo", do_typo, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"vote", do_vote, POS_RESTING, 0, LOG_ALWAYS, 1, 0, "none"},
	{"weather", do_weather, POS_RESTING, 0, LOG_NORMAL, 1, 0, "none"},
	{"who", do_who, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"whois", do_whois, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"wizlist", do_wizlist, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"worth", do_worth, POS_SLEEPING, 0, LOG_NORMAL, 1, 0, "none"},

	/*
	 * Configuration commands.
	 */
	//{ "alia",		do_alia,	POS_DEAD,	 0,  LOG_NORMAL, 0, 0, "none" },
	//{ "alias",		do_alias,	POS_DEAD,	 0,  LOG_NORMAL, 1, 0, "none" },
	{"autolist", do_autolist, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"autoabort", do_autoabort, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"autoassist", do_autoassist, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"autoexit", do_autoexit, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"autogold", do_autogold, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"autoloot", do_autoloot, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"autosac", do_autosac, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"autosplit", do_autosplit, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"brief", do_brief, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	/*  { "channels",	do_channels,	POS_DEAD,	 0,  LOG_NORMAL, 1, 0, "none" }, */
	{"chess", do_chess, POS_RESTING, 0, LOG_NORMAL, 1, 0, "none"},
	{"color", do_color, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"combine", do_combine, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"compact", do_compact, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"description", do_description, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"delet", do_delet, POS_DEAD, 0, LOG_ALWAYS, 0, 0, "none"},
	{"delete", do_delete, POS_STANDING, 0, LOG_ALWAYS, 1, 0, "none"},
	{"nofollow", do_nofollow, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"nosummon", do_nosummon, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"outfit", do_outfit, POS_RESTING, 0, LOG_NORMAL, 1, 0, "none"},
	{"password", do_password, POS_DEAD, 0, LOG_NEVER, 1, 0, "none"},
	{"prompt", do_prompt, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"role", do_role, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"scroll", do_scroll, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"specialize", do_specialize, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"proficiencies", do_proficiencies, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"title", do_title, POS_DEAD, L5, LOG_NORMAL, 1, 0, "none"},
	//{ "unalias",	do_unalias,	POS_DEAD,	 0,  LOG_NORMAL, 1, 0, "none" },
	{"wimpy", do_wimpy, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},

	/*
	 * Communication commands.
	 */
	{"deaf", do_deaf, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"emote", do_emote, POS_RESTING, 0, LOG_NORMAL, 1, 0, "none"},
	{"pmote", do_pmote, POS_RESTING, 0, LOG_NORMAL, 1, 0, "none"},
	{",", do_emote, POS_RESTING, 0, LOG_NORMAL, 0, 0, "none"},
	{"gtell", do_gtell, POS_DEAD, 0, LOG_NORMAL, 1, 1, "none"},
	{";", do_gtell, POS_DEAD, 0, LOG_NORMAL, 0, 1, "none"},
	{"builder", do_builder, POS_DEAD, 0, LOG_NORMAL, 0, 1, "none"},
	{".", do_builder, POS_DEAD, 0, LOG_NORMAL, 0, 1, "none"},
	{"immtalk", do_immtalk, POS_DEAD, L8, LOG_NORMAL, 1, 0, "none"},
	{"note", do_note, POS_SLEEPING, 0, LOG_NORMAL, 1, 0, "none"},
	{"noreply", do_noreply, POS_SLEEPING, 0, LOG_NORMAL, 1, 0, "none"},
	{"newbie", do_newbie, POS_SLEEPING, 0, LOG_NORMAL, 1, 0, "none"},
	/*
		{ "quest",		do_quest,	POS_RESTING,	 0,  LOG_NORMAL, 1, 1, "none" },
	*/
	/* Newbie command by Dev */
	{"reply", do_reply, POS_RESTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"replay", do_replay, POS_SLEEPING, 0, LOG_NORMAL, 1, 0, "none"},
	{"say", do_say, POS_RESTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"'", do_say, POS_RESTING, 0, LOG_NORMAL, 0, 1, "none"},
	{"sing", do_sing, POS_RESTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"whisper", do_whisper, POS_RESTING, 0, LOG_NORMAL, 1, 0, "none"},
	{"[", do_whisper, POS_RESTING, 0, LOG_NORMAL, 1, 0, "none"},
	{"unread", do_unread, POS_SLEEPING, 0, LOG_NORMAL, 1, 0, "none"},
	{"yell", do_yell, POS_RESTING, 0, LOG_NORMAL, 1, 1, "none"},

	/*
	 * Object manipulation commands.
	 */
	{"brandish", do_brandish, POS_RESTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"close", do_close, POS_RESTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"drink", do_drink, POS_RESTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"drop", do_drop, POS_RESTING, 0, LOG_NORMAL, 1, 0, "none"},
	{"embalm", do_embalm, POS_STANDING, 0, LOG_NORMAL, 0, 1, "none"},
	{"eat", do_eat, POS_RESTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"envenom", do_envenom, POS_RESTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"fill", do_fill, POS_RESTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"give", do_give, POS_RESTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"heal", do_heal, POS_RESTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"hold", do_wear, POS_RESTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"list", do_list, POS_RESTING, 0, LOG_NORMAL, 1, 0, "none"},
	{"lock", do_lock, POS_RESTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"open", do_open, POS_RESTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"pick", do_pick, POS_RESTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"pour", do_pour, POS_STANDING, 0, LOG_NORMAL, 1, 1, "none"},
	{"put", do_put, POS_RESTING, 0, LOG_NORMAL, 1, 0, "none"},
	{"quaff", do_quaff, POS_RESTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"quiet", do_quiet, POS_SLEEPING, 0, LOG_NORMAL, 1, 0, "none"},
	{"recite", do_recite, POS_RESTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"remove", do_remove, POS_RESTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"roll", do_roll, POS_RESTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"doorbash", do_door_bash, POS_FIGHTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"headbutt", do_headbutt, POS_FIGHTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"silentmovement", do_silent_movement, POS_FIGHTING, 0, LOG_NORMAL, 1, 0, "none"},
	{"gore", do_gore, POS_STANDING, 0, LOG_NORMAL, 1, 1, "none"},
	{"sell", do_sell, POS_RESTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"take", do_get, POS_RESTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"sacrifice", do_sacrifice, POS_RESTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"junk", do_sacrifice, POS_RESTING, 0, LOG_NORMAL, 0, 1, "none"},
	{"skin", do_skin, POS_STANDING, 0, LOG_NORMAL, 0, 1, "none"},
	/*  { "unlock",		do_unlock,	POS_RESTING,	 0,  LOG_NORMAL, 1, 1, "none" }, */
	{"value", do_value, POS_RESTING, 0, LOG_NORMAL, 1, 0, "none"},
	{"wear", do_wear, POS_RESTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"listvotes", do_listvotes, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"zap", do_zap, POS_RESTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"vanish", do_vanish, POS_FIGHTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"aura", do_aura_of_sustenance, POS_RESTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"wake", do_wake, POS_SLEEPING, 0, LOG_NORMAL, 1, 0, "none"},

	/*
	 * Combat commands.
	 */
	{"style", do_style, POS_FIGHTING, 0, LOG_NORMAL, 0, 0, "none"},
	{"cleave", do_cleave, POS_STANDING, 0, LOG_NORMAL, 0, 0, "none"},
	{"herb", do_herb, POS_STANDING, 0, LOG_NORMAL, 0, 0, "none"},
	{"records", do_records, POS_SLEEPING, 0, LOG_NORMAL, 0, 0, "none"},
	{"ambush", do_ambush, POS_FIGHTING, 0, LOG_NORMAL, 0, 1, "none"},
	{"pugil", do_pugil, POS_FIGHTING, 0, LOG_NORMAL, 0, 1, "none"},
	{"lash", do_lash, POS_FIGHTING, 0, LOG_NORMAL, 0, 1, "none"},
	{"shieldcleave", do_shield_cleave, POS_FIGHTING, 0, LOG_NORMAL, 0, 1, "none"},
	{"backstab", do_backstab, POS_FIGHTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"knife", do_knife, POS_FIGHTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"bash", do_bash, POS_FIGHTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"throw", do_throw, POS_FIGHTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"strangle", do_strangle, POS_STANDING, 0, LOG_NORMAL, 1, 0, "none"},
	{"blindness", do_blindness_dust, POS_FIGHTING, 0, LOG_NORMAL, 0, 1, "none"},
	{"poison", do_poison_dust, POS_FIGHTING, 0, LOG_NORMAL, 0, 1, "none"},
	{"warcry", do_warcry, POS_FIGHTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"endure", do_endure, POS_STANDING, 0, LOG_NORMAL, 1, 1, "none"},
	{"nerve", do_nerve, POS_FIGHTING, 0, LOG_NORMAL, 1, 1, "none"},
	//    { "ram",		do_ram,		POS_FIGHTING,    0,  LOG_NORMAL, 0, 1, "none" 		},
	{"tail", do_tail, POS_FIGHTING, 0, LOG_NORMAL, 0, 1, "none"},
	{"blackjack", do_blackjack, POS_STANDING, 0, LOG_NORMAL, 1, 0, "none"},
	{"bj", do_blackjack, POS_STANDING, 0, LOG_NORMAL, 0, 0, "none"},
	{"bs", do_backstab, POS_FIGHTING, 0, LOG_NORMAL, 0, 0, "none"},
	{"berserk", do_berserk, POS_FIGHTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"circle", do_circle_stab, POS_FIGHTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"darksight", do_darksight, POS_STANDING, 0, LOG_NORMAL, 1, 1, "none"},
	{"detect", do_detect_hidden, POS_STANDING, 0, LOG_NORMAL, 1, 0, "none"},
	{"bluff", do_bluff, POS_STANDING, 0, LOG_NORMAL, 1, 1, "none"},
	{"falsemotives", do_false_motives, POS_STANDING, 0, LOG_NORMAL, 1, 1, "none"},
	{"dirt", do_dirt, POS_FIGHTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"disarm", do_disarm, POS_FIGHTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"disengage", do_disengage, POS_FIGHTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"flee", do_flee, POS_FIGHTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"kick", do_kick, POS_FIGHTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"murde", do_murde, POS_FIGHTING, 0, LOG_NORMAL, 0, 0, "none"},
	{"murder", do_murder, POS_FIGHTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"rescue", do_rescue, POS_FIGHTING, 0, LOG_NORMAL, 0, 1, "none"},
	{"trip", do_trip, POS_FIGHTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"defend", do_defend, POS_FIGHTING, 0, LOG_NORMAL, 1, 0, "none"},
	{"assassinate", do_assassinate, POS_STANDING, 0, LOG_NORMAL, 0, 1, "none"},
	{"forage", do_forage, POS_STANDING, 0, LOG_NORMAL, 1, 1, "none"},
	{"protection", do_protection_heat_cold, POS_STANDING, 0, LOG_NORMAL, 1, 1, "none"},
	{"intimidate", do_intimidate, POS_FIGHTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"bind", do_bind, POS_STANDING, 0, LOG_NORMAL, 0, 0, "none"},
	{"unbind", do_unbind, POS_FIGHTING, 0, LOG_NORMAL, 0, 1, "none"},
	{"charge", do_charge, POS_STANDING, 0, LOG_NORMAL, 0, 0, "charge"},
	{"offhand", do_offhand, POS_FIGHTING, 0, LOG_NORMAL, 0, 0, "none"},
	//    { "tsunami",	do_tsunami,	POS_FIGHTING,	0,	LOG_NEVER,	0, 0,	"none" },
	//    { "trunksmash",	do_trunksmash,	POS_FIGHTING,	0,	LOG_NEVER,	0, 0,	"none" },
	//    { "skullcrush",	do_skullcrush,	POS_FIGHTING,	0,	LOG_NEVER,	0, 0,	"none" },
	//    { "blast",		do_blast,	POS_FIGHTING,	0,	LOG_NEVER,	0, 0,	"none" },
	{"enter", do_enter, POS_STANDING, 0, LOG_NEVER, 0, 0, "none"},
	{"storytell", do_storytell, POS_RESTING, 0, LOG_NEVER, 0, 0, "none"},
	{"entrap", do_entrap, POS_FIGHTING, 0, LOG_NORMAL, 0, 0, "entrap"},
	{"drive", do_drive, POS_FIGHTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"dash", do_dash, POS_STANDING, 0, LOG_NORMAL, 1, 1, "none"},
	{"howl", do_howl, POS_FIGHTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"hobble", do_hobble, POS_FIGHTING, 0, LOG_NORMAL, 0, 0, "hobble"},
	{"cripplingblow", do_crippling_blow, POS_FIGHTING, 0, LOG_NORMAL, 0, 0, "crippling blow"},
	{"gouge", do_gouge, POS_FIGHTING, 0, LOG_NORMAL, 1, 1, "gouge"},
	{"bleed", do_bleed, POS_FIGHTING, 0, LOG_NORMAL, 1, 1, "bleed"},
	{"posture", do_posture, POS_FIGHTING, 0, LOG_NORMAL, 0, 0, "posture"},
	{"unbalance", do_unbalance, POS_FIGHTING, 0, LOG_NORMAL, 0, 0, "unbalance"},
	{"drain", do_drain, POS_STANDING, 0, LOG_NORMAL, 0, 0, "none"},
	{"concuss", do_concuss, POS_FIGHTING, 0, LOG_NORMAL, 0, 0, "concuss"},
	{"retreat", do_retreat, POS_FIGHTING, 0, LOG_NORMAL, 0, 0, "retreat"},
	{"overhead", do_overhead, POS_FIGHTING, 0, LOG_NORMAL, 0, 0, "overhead"},
	{"uppercut", do_uppercut, POS_FIGHTING, 0, LOG_NORMAL, 0, 0, "uppercut"},
	{"disrupt", do_disrupt_formation, POS_STANDING, 0, LOG_NORMAL, 0, 0, "disrupt formation"},
	{"dart", do_dart, POS_FIGHTING, 0, LOG_NORMAL, 0, 0, "dart"},
	{"hurl", do_hurl, POS_STANDING, 0, LOG_NORMAL, 0, 0, "hurl"},
	{"extract", do_extract, POS_STANDING, 0, LOG_NORMAL, 1, 1, "none"},
	{"exchange", do_exchange, POS_FIGHTING, 0, LOG_NORMAL, 0, 0, "exchange"},
	{"shieldbash", do_shieldbash, POS_FIGHTING, 0, LOG_NORMAL, 0, 1, "shieldbash"},
	{"brace", do_brace, POS_FIGHTING, 0, LOG_NORMAL, 0, 1, "brace"},
	{"shatter", do_shatter, POS_FIGHTING, 0, LOG_NORMAL, 0, 0, "shatter"},
	{"whirlwind", do_whirlwind, POS_FIGHTING, 0, LOG_NORMAL, 0, 0, "whirlwind"},
	{"leadership", do_leadership, POS_STANDING, 0, LOG_NORMAL, 0, 0, "leadership"},
	{"uncoil", do_uncoil, POS_FIGHTING, 0, LOG_NORMAL, 0, 1, "none"},
	{"entwine", do_entwine, POS_FIGHTING, 0, LOG_NORMAL, 0, 1, "entwine"},
	{"pull", do_pull, POS_FIGHTING, 0, LOG_NORMAL, 0, 1, "pull"},
	{"assess", do_assess, POS_FIGHTING, 0, LOG_NORMAL, 0, 1, "assess"},
	{"exploit", do_exploit, POS_FIGHTING, 0, LOG_NORMAL, 0, 1, "exploit"},
	{"outflank", do_outflank, POS_FIGHTING, 0, LOG_NORMAL, 0, 0, "outflank"},
	{"impale", do_impale, POS_FIGHTING, 0, LOG_NORMAL, 0, 1, "impale"},
	{"valiantcharge", do_valiant_charge, POS_STANDING, 0, LOG_NORMAL, 0, 1, "valiant charge"},
	{"turnundead", do_turn_undead, POS_FIGHTING, 0, LOG_NORMAL, 0, 0, "none"},
	{"strikeofvirtue", do_strike_of_virtue, POS_FIGHTING, 0, LOG_NORMAL, 0, 0, "strike of virtue"},
	{"groupretreat", do_group_retreat, POS_FIGHTING, 0, LOG_NORMAL, 0, 0, "group retreat"},
	/*
	 * Miscellaneous commands.
	 */
	{"flip", do_flip, POS_RESTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"follow", do_follow, POS_RESTING, 0, LOG_NORMAL, 1, 0, "none"},
	{"creep", do_creep, POS_STANDING, 0, LOG_NORMAL, 1, 0, "none"},
	{"scan", do_scan, POS_RESTING, 0, LOG_NORMAL, 1, 0, "none"},
	{"gain", do_gain, POS_STANDING, 0, LOG_NORMAL, 1, 0, "none"},
	{"groups", do_groups, POS_SLEEPING, 0, LOG_NORMAL, 1, 0, "none"},
	{"acute", do_acute_vision, POS_STANDING, 0, LOG_NORMAL, 1, 0, "none"},
	{"barkskin", do_barkskin, POS_STANDING, 0, LOG_NORMAL, 1, 0, "none"},
	{"animal", do_animal_call, POS_STANDING, 0, LOG_NORMAL, 1, 0, "none"},
	{"enlist", do_enlist, POS_STANDING, 0, LOG_NORMAL, 1, 0, "none"},
	{"camouflage", do_camouflage, POS_STANDING, 0, LOG_NORMAL, 1, 0, "none"},
	{"camp", do_camp, POS_RESTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"hide", do_hide, POS_RESTING, 0, LOG_NORMAL, 1, 0, "none"},
	{"laying", do_laying_hands, POS_STANDING, 0, LOG_NORMAL, 1, 0, "none"},
	{"qui", do_qui, POS_DEAD, 0, LOG_NORMAL, 0, 0, "none"},
	{"quit", do_quit, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"release", do_release, POS_FIGHTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"recall", do_recall, POS_FIGHTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"tame", do_tame, POS_FIGHTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"find water", do_find_water, POS_STANDING, 0, LOG_NORMAL, 1, 1, "none"},
	{"fw", do_find_water, POS_STANDING, 0, LOG_NORMAL, 0, 1, "none"},
	//    { "track", 		do_track, 	POS_RESTING, 	 0,  LOG_NORMAL, 1, 1, "none" },
	{"rent", do_rent, POS_DEAD, 0, LOG_NORMAL, 0, 0, "none"},
	{"save", do_save, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"sleep", do_sleep, POS_SLEEPING, 0, LOG_NORMAL, 1, 0, "none"},
	{"sneak", do_sneak, POS_STANDING, 0, LOG_NORMAL, 1, 0, "none"},
	{"split", do_split, POS_RESTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"steal", do_steal, POS_STANDING, 0, LOG_NEVER, 1, 0, "none"},
	{"plant", do_plant, POS_STANDING, 0, LOG_NEVER, 1, 0, "none"},
	{"gag", do_gag, POS_STANDING, 0, LOG_NEVER, 1, 0, "none"},
	{"ungag", do_ungag, POS_STANDING, 0, LOG_NEVER, 1, 0, "none"},
	{"drag", do_drag, POS_STANDING, 0, LOG_NEVER, 1, 0, "none"},
	{"sign", do_sign, POS_STANDING, 0, LOG_NEVER, 1, 0, "none"},
	{"slash", do_slash, POS_STANDING, 0, LOG_NEVER, 1, 0, "none"},
	{"tripwire", do_tripwire, POS_STANDING, 0, LOG_NEVER, 1, 0, "tripwire"},
	{"stash", do_stash, POS_STANDING, 0, LOG_NEVER, 1, 0, "none"},
	{"disguise", do_disguise, POS_STANDING, 0, LOG_NEVER, 1, 1, "none"},
	{"undisguise", do_undisguise, POS_STANDING, 0, LOG_NEVER, 1, 1, "none"},
	{"shadowcloak", do_shadow_cloak, POS_STANDING, 0, LOG_NEVER, 1, 1, "none"},
	{"search", do_search, POS_STANDING, 0, LOG_NEVER, 1, 0, "none"},
	{"counterfeit", do_counterfeit, POS_STANDING, 0, LOG_NEVER, 1, 0, "none"},
	{"strip", do_strip, POS_STANDING, 0, LOG_NEVER, 1, 0, "none"},
	{"train", do_train, POS_RESTING, 0, LOG_NORMAL, 1, 0, "none"},
	{"visible", do_visible, POS_SLEEPING, 0, LOG_NORMAL, 1, 0, "none"},
	{"where", do_where, POS_RESTING, 0, LOG_NORMAL, 1, 0, "none"},
	{"cloak", do_cloak, POS_STANDING, 0, LOG_NORMAL, 1, 0, "none"},
	{"hometown", do_hometown, POS_RESTING, 0, LOG_ALWAYS, 1, 1, "none"},
	{"invoke", do_invoke, POS_DEAD, 0, LOG_NORMAL, 1, 1, "none"},
	{"snare", do_snare, POS_STANDING, 0, LOG_NORMAL, 0, 0, "none"},
	{"bearcall", do_bear_call, POS_STANDING, 0, LOG_ALWAYS, 0, 0, "none"},
	{"bounty", do_bounty, POS_STANDING, 0, LOG_ALWAYS, 0, 0, "none"},
	{"topbounties", do_topbounties, POS_SLEEPING, L5, LOG_NORMAL, 0, 0, "none"},
	{"offer", do_offer, POS_STANDING, 0, LOG_ALWAYS, 0, 0, "none"},
	{"listoffer", do_listoffer, POS_STANDING, L7, LOG_NORMAL, 0, 0, "none"},
	{"taunt", do_taunt, POS_STANDING, 0, LOG_NORMAL, 1, 1, "none"},
	{"consume", do_consume, POS_STANDING, 0, LOG_NORMAL, 1, 1, "none"},
	{"breathofmephisto", do_breath_mephisto, POS_FIGHTING, 0, LOG_NORMAL, 1, 1, "none"},
	{"touch", do_touch, POS_FIGHTING, 0, LOG_NORMAL, 1, 1, "burning touch"},
	{"exile", do_exile, POS_DEAD, 0, LOG_ALWAYS, 1, 1, "none"},

	{"trust", do_trustchar, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"trustgroup", do_trustgroup, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"trustcabal", do_trustcabal, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},

	/*
	 * Immortal commands.
	 */
	{"advance", do_advance, POS_DEAD, ML, LOG_ALWAYS, 1, 0, "none"},
	{"alist", do_alist, POS_DEAD, IM, LOG_NORMAL, 1, 0, "none"},
	{"dump", do_dump, POS_DEAD, ML, LOG_ALWAYS, 0, 0, "none"},
	{"immtrust", do_trust, POS_DEAD, ML, LOG_ALWAYS, 1, 0, "none"},
	{"violate", do_violate, POS_DEAD, ML, LOG_ALWAYS, 1, 0, "none"},
	{"unban", do_unban, POS_DEAD, L2, LOG_ALWAYS, 1, 0, "none"},
	{"ban", do_ban, POS_DEAD, L2, LOG_ALWAYS, 1, 0, "none"},
	{"rinfo", do_rinfo, POS_DEAD, L6, LOG_NEVER, 1, 1, "none"},
	{"deny", do_deny, POS_DEAD, L2, LOG_ALWAYS, 1, 0, "none"},
	{"disconn", do_disconnect, POS_DEAD, L5, LOG_ALWAYS, 1, 0, "none"},
	{"flag", do_flag, POS_DEAD, L3, LOG_ALWAYS, 1, 0, "none"},
	{"freeze", do_freeze, POS_DEAD, L3, LOG_ALWAYS, 1, 0, "none"},
	{"lag", do_lag, POS_DEAD, ML, LOG_ALWAYS, 1, 0, "none"},
	{"global", do_global, POS_DEAD, L6, LOG_NORMAL, 0, 0, "none"},
	{"smite", do_smite, POS_DEAD, L5, LOG_ALWAYS, 1, 0, "none"},
	{"iprotect", do_protect, POS_DEAD, L1, LOG_ALWAYS, 1, 0, "none"},
	{"reboo", do_reboo, POS_DEAD, L5, LOG_NORMAL, 0, 0, "none"},
	{"reboot", do_reboot, POS_DEAD, L5, LOG_ALWAYS, 1, 0, "none"},
	{"set", do_set, POS_DEAD, L2, LOG_ALWAYS, 1, 0, "none"},
	{"wizlock", do_wizlock, POS_DEAD, L2, LOG_ALWAYS, 1, 0, "none"},
	{"classes", do_classes, POS_DEAD, IM, LOG_NORMAL, 1, 0, "none"},
	{"force", do_force, POS_DEAD, L7, LOG_ALWAYS, 1, 0, "none"},
	{"load", do_load, POS_DEAD, L5, LOG_ALWAYS, 1, 0, "none"},
	{"newlock", do_newlock, POS_DEAD, L2, LOG_ALWAYS, 1, 0, "none"},
	{"nochann", do_nochannels, POS_DEAD, L5, LOG_ALWAYS, 1, 0, "none"},
	{"noemote", do_noemote, POS_DEAD, L5, LOG_ALWAYS, 1, 0, "none"},
	{"noshout", do_noshout, POS_DEAD, L5, LOG_ALWAYS, 1, 0, "none"},
	{"notell", do_notell, POS_DEAD, L5, LOG_ALWAYS, 1, 0, "none"},
	{"pecho", do_pecho, POS_DEAD, L7, LOG_ALWAYS, 1, 0, "none"},
	{"pardon", do_pardon, POS_DEAD, L3, LOG_ALWAYS, 1, 0, "none"},
	{"purge", do_purge, POS_DEAD, L4, LOG_ALWAYS, 1, 0, "none"},
	{"restore", do_restore, POS_DEAD, L3, LOG_ALWAYS, 1, 0, "none"},
	{"sla", do_sla, POS_DEAD, L3, LOG_NORMAL, 0, 0, "none"},
	{"slay", do_slay, POS_DEAD, L3, LOG_ALWAYS, 1, 0, "none"},
	{"teleport", do_transfer, POS_DEAD, L5, LOG_ALWAYS, 1, 0, "none"},
	{"transfer", do_transfer, POS_DEAD, L5, LOG_ALWAYS, 1, 0, "none"},
	/*  { "at",		do_at,		POS_DEAD,	L6,  LOG_NORMAL, 1, 0, "none" }, */
	{"poofin", do_bamfin, POS_DEAD, L8, LOG_NORMAL, 1, 0, "none"},
	{"addapply", do_addapply, POS_DEAD, L2, LOG_NORMAL, 1, 0, "none"},
	{"maxlimits", do_max_limits, POS_DEAD, L3, LOG_ALWAYS, 1, 0, "none"},
	{"poofout", do_bamfout, POS_DEAD, L8, LOG_NORMAL, 1, 0, "none"},
	{"freset", do_force_reset, POS_DEAD, 51, LOG_ALWAYS, 1, 0, "none"},
	{"gecho", do_echo, POS_DEAD, L5, LOG_ALWAYS, 1, 0, "none"},
	{"astrip", do_astrip, POS_DEAD, L5, LOG_ALWAYS, 1, 0, "none"},
	{"tattoo", do_brands, POS_DEAD, L7, LOG_ALWAYS, 1, 0, "none"},
	{"untattoo", do_unbrands, POS_DEAD, L7, LOG_ALWAYS, 1, 0, "none"},
	/*  { "goto",		do_goto,	POS_DEAD,	L8,  LOG_NORMAL, 1, 0, "none" }, */
	{"holylight", do_holylight, POS_DEAD, IM, LOG_NORMAL, 1, 0, "none"},
	{"incognito", do_incognito, POS_DEAD, L6, LOG_NORMAL, 1, 0, "none"},
	{"log", do_log, POS_DEAD, L1, LOG_ALWAYS, 1, 0, "none"},
	{"memory", do_memory, POS_DEAD, L5, LOG_NORMAL, 1, 0, "none"},
	{"mwhere", do_mwhere, POS_DEAD, L5, LOG_NORMAL, 1, 0, "none"},
	{"owhere", do_owhere, POS_DEAD, L5, LOG_ALWAYS, 1, 0, "none"},
	{"peace", do_peace, POS_DEAD, L6, LOG_NORMAL, 1, 0, "none"},
	{"praclist", do_praclist, POS_DEAD, IM, LOG_NORMAL, 1, 0, "none"},
	{"ghost", do_ghost, POS_DEAD, L6, LOG_ALWAYS, 1, 0, "none"},
	{"echo", do_recho, POS_DEAD, L6, LOG_ALWAYS, 1, 0, "none"},
	{"return", do_return, POS_DEAD, L7, LOG_NORMAL, 1, 0, "none"},
	{"snoop", do_snoop, POS_DEAD, L5, LOG_ALWAYS, 1, 0, "none"},
	{"stat", do_stat, POS_DEAD, L7, LOG_NORMAL, 1, 0, "none"},
	{"string", do_string, POS_DEAD, L5, LOG_ALWAYS, 1, 0, "none"},
	{"switch", do_switch, POS_DEAD, L7, LOG_ALWAYS, 1, 0, "none"},
	{"wizinvis", do_invis, POS_DEAD, L6, LOG_NORMAL, 1, 0, "none"},
	{"vnum", do_vnum, POS_DEAD, L7, LOG_NORMAL, 1, 0, "none"},
	{"vstat", do_vstat, POS_DEAD, L7, LOG_NORMAL, 1, 0, "none"},
	{"zecho", do_zecho, POS_DEAD, L6, LOG_ALWAYS, 1, 0, "none"},
	{"clone", do_clone, POS_DEAD, L5, LOG_ALWAYS, 1, 0, "none"},
	{"wiznet", do_wiznet, POS_DEAD, IM, LOG_NORMAL, 1, 0, "none"},
	{"imotd", do_imotd, POS_DEAD, IM, LOG_NORMAL, 1, 0, "none"},
	{":", do_immtalk, POS_DEAD, 0, LOG_NORMAL, 0, 0, "none"},
	{"renam", do_renam, POS_DEAD, L3, LOG_ALWAYS, 0, 0, "none"},
	{"rename", do_rename, POS_DEAD, L3, LOG_ALWAYS, 0, 0, "none"},
	{"smote", do_smote, POS_DEAD, IM, LOG_NORMAL, 1, 0, "none"},
	{"prefi", do_prefi, POS_DEAD, IM, LOG_NORMAL, 0, 0, "none"},
	{"prefix", do_prefix, POS_DEAD, IM, LOG_NORMAL, 1, 0, "none"},
	{"xlook", do_xlook, POS_DEAD, L7, LOG_NORMAL, 1, 0, "none"},
	{"affrem", do_affrem, POS_DEAD, L2, LOG_ALWAYS, 1, 0, "none"},
	{"divine", do_divine_intervention, POS_DEAD, L2, LOG_ALWAYS, 1, 0, "none"},
	{"ccb", do_ccb, POS_DEAD, L7, LOG_ALWAYS, 1, 0, "none"},
	{"heroimm", do_heroimm, POS_DEAD, ML, LOG_ALWAYS, 1, 0, "none"},
	{"history", do_history, POS_DEAD, L7, LOG_ALWAYS, 1, 0, "none"},
	{"finger", do_finger, POS_DEAD, L3, LOG_ALWAYS, 1, 0, "none"},
	{"ltrack", do_ltrack, POS_DEAD, ML, LOG_ALWAYS, 1, 0, "none"},
	{"sitetrack", do_sitetrack, POS_DEAD, ML, LOG_ALWAYS, 1, 0, "none"},
	{"afk", do_afk, POS_DEAD, L7, LOG_NORMAL, 1, 0, "none"},
	{"affto", do_affto, POS_DEAD, L2, LOG_NORMAL, 1, 0, "none"},
	{"crumble", do_crumble, POS_DEAD, L7, LOG_NORMAL, 1, 0, "none"},
	{"empower", do_empower, POS_DEAD, L7, LOG_ALWAYS, 1, 0, "none"},
	{"pktrack", do_pktrack, POS_DEAD, L7, LOG_ALWAYS, 1, 0, "none"},
	{"ctrack", do_ctrack, POS_DEAD, L5, LOG_ALWAYS, 1, 0, "none"},
	{"pload", do_pload, POS_DEAD, ML, LOG_ALWAYS, 1, 0, "none"},
	{"pswitch", do_pswitch, POS_DEAD, 0, LOG_ALWAYS, 1, 0, "none"},
	{"listhelp", do_listhelp, POS_DEAD, IM, LOG_NORMAL, 1, 0, "none"},
	/*    { "addhelp",       	do_addhelp,    	POS_DEAD,       IM,  LOG_NORMAL, 1, 0, "none" },
		{ "delhelp",       	do_delhelp,    	POS_DEAD,       IM,  LOG_NORMAL, 1, 0, "none" },
		{ "modhelp",        do_modhelp,     POS_DEAD,       IM,  LOG_NORMAL, 1, 0, "none" },
	*/
	{"damage", do_damage, POS_DEAD, L3, LOG_ALWAYS, 1, 0, "none"},
	{"forcetick", do_forcetick, POS_DEAD, ML, LOG_ALWAYS, 1, 0, "none"},
	{"demograph", do_demo, POS_DEAD, L7, LOG_ALWAYS, 1, 0, "none"},
	{"deathmsg", do_deathmessage, POS_DEAD, IM, LOG_ALWAYS, 1, 0, "none"},
	{"rastrip", do_rastrip, POS_DEAD, L5, LOG_ALWAYS, 1, 0, "none"},
	{"oastrip", do_oastrip, POS_DEAD, L5, LOG_ALWAYS, 1, 0, "none"},
	{"aastrip", do_aastrip, POS_DEAD, L5, LOG_ALWAYS, 1, 0, "none"},
	{"damdice", do_damdice, POS_DEAD, IM, LOG_NEVER, 1, 0, "none"},
	{"rngtest", do_rngtest, POS_DEAD, IM, LOG_NEVER, 1, 0, "none"},
	{"clean", do_clean, POS_DEAD, ML, LOG_ALWAYS, 1, 0, "none"},
	{"gold", do_gold, POS_DEAD, L6, LOG_ALWAYS, 1, 0, "none"},
	{"favor", do_devilfavor, POS_DEAD, L4, LOG_ALWAYS, 0, 0, "none"},
	{"clearfavors", do_clearfavors, POS_DEAD, L2, LOG_ALWAYS, 0, 0, "none"},
	{"gsnlist", do_gsnlist, POS_DEAD, L2, LOG_ALWAYS, 0, 0, "none"},
	{"clearquests", do_clearquests, POS_DEAD, L2, LOG_ALWAYS, 0, 0, "none"},
	{"ccl", do_ccl, POS_DEAD, L2, LOG_NEVER, 0, 0, "none"},
	{"antiidle", do_antiidle, POS_DEAD, L8, LOG_NEVER, 0, 0, "none"},
	{"noskills", do_noskills, POS_DEAD, L7, LOG_NEVER, 0, 0, "none"},
	{"givexp", do_givexp, POS_DEAD, L5, LOG_ALWAYS, 0, 0, "none"},
	/*
	 * OLC
	 */
	{"edit", do_olc, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"asave", do_asave, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	//    { "alist",		do_alist,	POS_DEAD,    0,  LOG_NORMAL, 1 },
	{"resets", do_resets, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"redit", do_redit, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"medit", do_medit, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"aedit", do_aedit, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"oedit", do_oedit, POS_DEAD, 0, LOG_NORMAL, 1, 0, "none"},
	{"buglist", do_buglist, POS_DEAD, IM, LOG_NORMAL, 1, 0, "none"},
	{"questmod", do_memtest, POS_DEAD, ML, LOG_NEVER, 1, 0, "none"},
	{"constdump", do_constdump, POS_DEAD, L1, LOG_ALWAYS, 1, 0, "none"},
	{"setpath", do_setpath, POS_DEAD, L7, LOG_ALWAYS, 1, 0, "none"},
	{"racedump", do_racedump, POS_DEAD, L2, LOG_NORMAL, 1, 0, "none"},
	{"interpdump", do_interpdump, POS_DEAD, L2, LOG_NORMAL, 1, 0, "none"},
	{"phalanx", do_phalanx, POS_DEAD, 0, LOG_ALWAYS, 0, 0, "none"},
	{"createcosmetic", do_createcosmetic, POS_DEAD, L6, LOG_NORMAL, 1, 0, "none"},
	{"forcewear", do_forcewear, POS_DEAD, L7, LOG_NORMAL, 1, 0, "none"},
	{"forceremove", do_forceremove, POS_DEAD, L7, LOG_NORMAL, 1, 0, "none"},
	/*
	 * End of list.
	 */
	{"", 0, POS_DEAD, 0, LOG_NORMAL, 0, 0, "none"}};

void list_cmd_queue(CHAR_DATA *ch)
{
	// TODO
}

void clear_cmd_queue(CHAR_DATA *ch)
{
	int i = 0;
	ch->pcdata->read_next = 0;
	ch->pcdata->write_next = 0;
	ch->pcdata->pending= false;

	for (i = ch->pcdata->write_next; i < MAX_QUEUE; i++)
	{
		ch->pcdata->queue[i][0] = '\0';
	}

	send_to_char("Command queue cleared.\n\r", ch);
}

/*
 * The main entry point for executing commands.
 * Can be recursively called from 'at', 'order', 'force'.
 */
void interpret(CHAR_DATA *ch, char *argument)
{
	char command[MAX_INPUT_LENGTH], arg_dup[MSL], object[MSL];
	char logline[MAX_INPUT_LENGTH];
	char skill_name[MSL];
	char buf[MSL];
	int cmd, gn, skill_num, cmd2;
	int trust, sn = 0, where, mana = 0;
	bool found, is_social= false, vprog= false;
	void *vo = NULL;
	int i;
	AFFECT_DATA *paf;
	CHAR_DATA *victim = NULL;
	OBJ_DATA *obj, *vpobj = NULL;

	/*
	 * Strip leading spaces.
	 */
	while (isspace(*argument))
	{
		argument++;
	}

	if (argument[0] == '\0')
		return;

	/*
	 * Implement freeze command.
	 */
	if (!is_npc(ch) && IS_SET(ch->act, PLR_FREEZE) && !IS_IMP(ch))
	{
		send_to_char("You're totally frozen!\n\r", ch);
		return;
	}

	/*
	 * Grab the command word.
	 * Special parsing so ' can be a command,
	 *   also no spaces needed after punctuation.
	 */
	strcpy(logline, argument);

	if (!isalpha(argument[0]) && !isdigit(argument[0]))
	{
		command[0] = argument[0];
		command[1] = '\0';
		argument++;

		while (isspace(*argument))
		{
			argument++;
		}
	}
	else
	{
		argument = one_argument(argument, command);
	}

	/*
	 * Look for command in command table.
	 */

	if (!str_prefix("qc", command))
	{
		clear_cmd_queue(ch);
		return;
	}

	if (ch->wait > 0 && !is_npc(ch))
	{
		if (ch->pcdata->write_next > (MAX_QUEUE - 1))
		{
			send_to_char("Command queue full.\n\r", ch);
			return;
		}

		auto qcommand = fmt::format("{} {}", command, argument);
		strcpy(ch->pcdata->queue[ch->pcdata->write_next], qcommand.c_str());
		//		sprintf(buf,"Command '%s' queued.\n\r",qcommand.c_str());
		//		send_to_char(buf,ch);
		ch->pcdata->write_next++;
		ch->pcdata->pending = true;

		return;
	}

	found= false;
	trust = get_trust(ch);

	for (cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++)
	{
		if (command[0] == cmd_table[cmd].name[0]
			&& !str_prefix(command, cmd_table[cmd].name)
			&& cmd_table[cmd].level <= trust
			&& knows_command(ch, cmd))
		{
			found = true;
			break;
		}
	}

	/*
	 * Log and snoop.
	 */
	if (cmd_table[cmd].log == LOG_NEVER)
		strcpy(logline, "");

	if ((!is_npc(ch) && IS_SET(ch->act, PLR_LOG)) || fLogAll || cmd_table[cmd].log == LOG_ALWAYS)
	{
		sprintf(log_buf, "Log %s: %s", ch->desc->original ? ch->desc->original->true_name : ch->true_name, logline);
		wiznet(log_buf, ch, NULL, WIZ_SECURE, 0, get_trust(ch));
		log_string(log_buf);
	}

	if (ch->desc != NULL && ch->desc->snoop_by != NULL)
	{
		write_to_buffer(ch->desc->snoop_by, "% ", 2);
		write_to_buffer(ch->desc->snoop_by, logline, 0);
		write_to_buffer(ch->desc->snoop_by, "\n\r", 2);
	}

	/* Hold person */

	if (!is_npc(ch)
		&& (is_affected(ch, gsn_hold_person) || ch->pcdata->energy_state < -4)
		&& get_trust(ch) < MAX_LEVEL
		&& cmd_table[cmd].name != "immtalk"
		&& cmd_table[cmd].name != "astrip")
	{
		send_to_char("You are totally frozen!\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_creeping_tomb))
	{
		paf = affect_find(ch->affected, gsn_creeping_tomb);
		
		if (paf->duration <= 2 && (cmd_table[cmd].name != "astrip") && (cmd_table[cmd].name != "immtalk"))
		{
			send_to_char("You are entombed in ooze and cannot move.\n\r", ch);
			return;
		}
	}

	if (is_affected(ch, gsn_horde_communion))
	{
		paf = affect_find(ch->affected, gsn_horde_communion);

		if (paf->modifier > 0 && ch->in_room->vnum != 23633)
			return;
	}

	if (is_affected(ch, gsn_ultradiffusion)
		&& cmd_table[cmd].name != "visible"
		&& cmd_table[cmd].name != "score"
		&& cmd_table[cmd].name != "look"
		&& cmd_table[cmd].name != "who"
		&& cmd_table[cmd].name != "where"
		&& cmd_table[cmd].name != "affects"
		&& cmd_table[cmd].name != "group"
		&& cmd_table[cmd].name != "gt"
		&& cmd_table[cmd].name != "gtell"
		&& cmd_table[cmd].name != "tell"
		&& cmd_table[cmd].name != "reply"
		&& cmd_table[cmd].name != "north"
		&& cmd_table[cmd].name != "east"
		&& cmd_table[cmd].name != "south"
		&& cmd_table[cmd].name != "west"
		&& cmd_table[cmd].name != "up"
		&& cmd_table[cmd].name != "down"
		&& cmd_table[cmd].name != "equipment"
		&& cmd_table[cmd].name != "skills"
		&& cmd_table[cmd].name != "save"
		&& cmd_table[cmd].name != "skills"
		&& cmd_table[cmd].name != "spells"
		&& cmd_table[cmd].name != "pray"
		&& !(cmd_table[cmd].level > LEVEL_HERO))
	{
		send_to_char("You cannot do that while your molecules are diffused from your body.\n\r", ch);
		return;
	}

	for (cmd2 = 0; social_table[cmd2].name[0] != '\0'; cmd2++)
	{
		if (command[0] == social_table[cmd2].name[0] && !str_prefix(command, social_table[cmd2].name))
		{
			is_social = true;
			break;
		}
	}

	if (is_affected(ch, gsn_bind_feet)
		&& cmd_table[cmd].name != "score"
		&& cmd_table[cmd].name != "look"
		&& cmd_table[cmd].name != "glance"
		&& cmd_table[cmd].name != "examine"
		&& cmd_table[cmd].name != "get"
		&& cmd_table[cmd].name != "wear"
		&& cmd_table[cmd].name != "remove"
		&& cmd_table[cmd].name != "wield"
		&& cmd_table[cmd].name != "zap"
		&& cmd_table[cmd].name != "recite"
		&& cmd_table[cmd].name != "brandish"
		&& cmd_table[cmd].name != "invoke"
		&& cmd_table[cmd].name != "quaff"
		&& cmd_table[cmd].name != "eat"
		&& cmd_table[cmd].name != "drink"
		&& cmd_table[cmd].name != "say"
		&& cmd_table[cmd].name != "'"
		&& cmd_table[cmd].name != "tell"
		&& cmd_table[cmd].name != "whisper"
		&& cmd_table[cmd].name != "["
		&& cmd_table[cmd].name != ";"
		&& cmd_table[cmd].name != ","
		&& cmd_table[cmd].name != "yell"
		&& cmd_table[cmd].name != "who"
		&& cmd_table[cmd].name != "where"
		&& cmd_table[cmd].name != "chess"
		&& cmd_table[cmd].name != "affects"
		&& cmd_table[cmd].name != "group"
		&& cmd_table[cmd].name != "gt"
		&& cmd_table[cmd].name != "gtell"
		&& cmd_table[cmd].name != "reply"
		&& cmd_table[cmd].name != "immtalk"
		&& cmd_table[cmd].name != "equipment"
		&& cmd_table[cmd].name != "save"
		&& cmd_table[cmd].name != "trustgroup"
		&& cmd_table[cmd].name != "trustall"
		&& cmd_table[cmd].name != "trustcabal"
		&& cmd_table[cmd].name != "skills"
		&& cmd_table[cmd].name != "spells"
		&& cmd_table[cmd].name != "supplications"
		&& cmd_table[cmd].name != "powers"
		&& cmd_table[cmd].name != "pray"
		&& cmd_table[cmd].name != "cast"
		&& cmd_table[cmd].name != "commune"
		&& cmd_table[cmd].name != "cast"
		&& cmd_table[cmd].name != "astrip"
		&& cmd_table[cmd].name != "force"
		&& !is_social)
	{
		send_to_char("Your feet are rooted to the ground and you cannot move!\n\r", ch);
		return;
	}

	for (i = 0; verb_prog_table[i].type; i++)
	{
		if (!str_cmp(command, verb_prog_table[i].type))
		{
			vprog = true;
			break;
		}
	}

	strcpy(arg_dup, argument);

	strcpy(arg_dup, one_argument(arg_dup, object));

	if (vprog && ((vpobj = get_obj_here(ch, object)) != NULL) && (vpobj->pIndexData->verb) &&
		(!str_cmp(command, vpobj->pIndexData->verb) && (ch->position >= POS_RESTING)))
	{
		if (IS_SET(vpobj->progtypes, IPROG_VERB))
			(vpobj->pIndexData->iprogs->verb_prog)(vpobj, ch, arg_dup);

		if (TRAPS_IEVENT(vpobj, TRAP_IVERB))
			CALL_IEVENT(vpobj, TRAP_IVERB, ch, vpobj, command, arg_dup);

		return;
	}

	if (!found && !is_npc(ch) && ch->position >= POS_FIGHTING)
	{
		if (ch->Profs()->InterpCommand(command, argument))
			return;
	}

	if (!found && !check_social(ch, command, argument))
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	else if (!found)
	{
		return;
	}

	/*
	 * Character not in position for command?
	 */
	if (ch->position < cmd_table[cmd].position)
	{
		switch (ch->position)
		{
			case POS_DEAD:
				send_to_char("Lie still; you are DEAD.\n\r", ch);
				break;
			case POS_MORTAL:
			case POS_INCAP:
				send_to_char("You are hurt far too bad for that.\n\r", ch);
				break;
			case POS_STUNNED:
				send_to_char("You are too stunned to do that.\n\r", ch);
				break;
			case POS_SLEEPING:
				send_to_char("In your dreams, or what?\n\r", ch);
				break;
			case POS_RESTING:
				send_to_char("Nah... You feel too relaxed...\n\r", ch);
				break;
			case POS_SITTING:
				send_to_char("Better stand up first.\n\r", ch);
				break;
			case POS_FIGHTING:
				send_to_char("No way!  You are still fighting!\n\r", ch);
				break;
		}

		return;
	}

	/*
	 * Hide parsing.
	 */
	if (cmd_table[cmd].hide)
		un_hide(ch, NULL);

	/* Style checks */
	if (!is_npc(ch) && get_trust(ch) < 60 && ch->pcdata->style && str_cmp(cmd_table[cmd].skill_name, "none"))
	{
		if (!str_cmp(cmd_table[cmd].skill_name, "none"))
			sprintf(skill_name, "%s", cmd_table[cmd].name);
		else
			sprintf(skill_name, "%s", cmd_table[cmd].skill_name);

		if ((skill_num = skill_lookup(skill_name)) > 0)
		{
			if (!str_cmp(cmd_table[cmd].skill_name, skill_table[skill_num].name))
			{
				if ((gn = gn_skill_lookup(skill_num)) > 1)
				{
					if (str_cmp(group_table[gn].name, style_table[ch->pcdata->style].name))
					{
						send_to_char("You are not in the proper style.\n\r", ch);
						return;
					}
				}
			}
		}
	}

	/*
	 * Dispatch the command.
	 */
	if (str_cmp(cmd_table[cmd].skill_name, "none") && (sn = skill_lookup(cmd_table[cmd].skill_name)) > 1)
	{
		switch (skill_table[sn].target)
		{
			case TAR_CHAR_OFFENSIVE:
				if (argument[0] == '\0')
				{
					if ((victim = ch->fighting) == NULL)
					{
						send_to_char("Do that to who?\n\r", ch);
						return;
					}
				}
				else
				{
					if ((victim = get_char_room(ch, argument)) == NULL)
					{
						send_to_char("They aren't here.\n\r", ch);
						return;
					}
				}

				if (is_safe(ch, victim))
					return;

				vo = (void *)victim;
				break;
			case TAR_CHAR_DEFENSIVE:
				if (argument[0] == '\0')
				{
					victim = ch;
				}
				else
				{
					if ((victim = get_char_room(ch, argument)) == NULL)
					{
						send_to_char("They aren't here.\n\r", ch);
						return;
					}
				}

				vo = (void *)victim;
				break;
			case TAR_CHAR_SELF:
				if (argument[0] != '\0' && !is_name(argument, ch->name))
				{
					send_to_char("You can't do that to someone else.\n\r", ch);
					return;
				}

				vo = (void *)ch;
				break;
			case TAR_OBJ_INV:
				if (argument[0] == '\0')
				{
					send_to_char("What do you want to do that to?\n\r", ch);
					return;
				}

				if ((obj = get_obj_carry(ch, argument, ch)) == NULL)
				{
					send_to_char("You are not carrying that.\n\r", ch);
					return;
				}

				vo = (void *)obj;
				break;
			case TAR_DIR:
				if ((where = direction_lookup(argument)) < 0 || !ch->in_room->exit[where])
				{
					send_to_char("That's not a valid direction.\n\r", ch);
					return;
				}

				vo = (void *)(long)where;
				break;
			case TAR_IGNORE:
			default:
				vo = argument;
				break;
		}

		if ((mana = skill_table[sn].min_mana) > std::max((int)ch->mana, 0))
		{
			send_to_char("You don't have enough mana.\n\r", ch);
			return;
		}
	}
	else
	{
		vo = argument;
	}

	command_line = argument;

	OBJ_DATA *iObj;
	for (iObj = ch->carrying; iObj; iObj = iObj->next_content)
	{
		if (TRAPS_IEVENT(iObj, TRAP_IDOFUN))
		{
			if (CALL_IEVENT(iObj, TRAP_IDOFUN, ch, iObj, cmd, sn, vo))
				return;
		}
	}

	(*cmd_table[cmd].do_fun)(ch, (char *)vo);

	if (command_result && sn > 3)
	{
		WAIT_STATE(ch, skill_table[sn].beats);
		ch->mana -= mana;
		command_result= false;

		if (skill_table[sn].target == TAR_CHAR_OFFENSIVE
			&& victim
			&& victim != ch
			&& !victim->fighting
			&& victim->master != ch)
		{
			multi_hit(victim, ch, TYPE_UNDEFINED);
		}
	}

	tail_chain();
}

bool knows_command(CHAR_DATA *ch, int cmd)
{
	if (!cmd_table[cmd].skill_name || cmd_table[cmd].skill_name == "none")
		return true;

	if (get_skill(ch, skill_lookup(cmd_table[cmd].skill_name)) < 2)
		return false;

	return true;
}

bool check_social(CHAR_DATA *ch, char *command, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	int cmd;
	bool found;

	found= false;
	for (cmd = 0; social_table[cmd].name[0] != '\0'; cmd++)
	{
		if (command[0] == social_table[cmd].name[0] && !str_prefix(command, social_table[cmd].name))
		{
			found = true;
			break;
		}
	}

	if (!found)
		return false;

	if (!is_npc(ch) && IS_SET(ch->comm, COMM_NOEMOTE))
	{
		send_to_char("You are anti-social!\n\r", ch);
		return true;
	}

	switch (ch->position)
	{
	case POS_DEAD:
			send_to_char("Lie still; you are DEAD.\n\r", ch);
			return true;
		case POS_INCAP:
		case POS_MORTAL:
			send_to_char("You are hurt far too bad for that.\n\r", ch);
			return true;
		case POS_STUNNED:
			send_to_char("You are too stunned to do that.\n\r", ch);
			return true;
		case POS_SLEEPING:
			/*
			* I just know this is the path to a 12" 'if' statement.  :(
			* But two players asked for it already!  -- Furey
			*/
			if (!str_cmp(social_table[cmd].name, "snore"))
				break;

			send_to_char("In your dreams, or what?\n\r", ch);
			return true;
	}

	one_argument(argument, arg);

	victim = NULL;

	if (arg[0] == '\0')
	{
		act(social_table[cmd].others_no_arg, ch, NULL, victim, TO_ROOM);
		act(social_table[cmd].char_no_arg, ch, NULL, victim, TO_CHAR);
	}
	else if ((victim = get_char_room(ch, arg)) == NULL)
	{
		send_to_char("They aren't here.\n\r", ch);
	}
	else if (victim == ch)
	{
		act(social_table[cmd].others_auto, ch, NULL, victim, TO_ROOM);
		act(social_table[cmd].char_auto, ch, NULL, victim, TO_CHAR);
	}
	else
	{
		act(social_table[cmd].others_found, ch, NULL, victim, TO_NOTVICT);
		act(social_table[cmd].vict_found, ch, NULL, victim, TO_VICT);
		act(social_table[cmd].char_found, ch, NULL, victim, TO_CHAR);
	}

	return true;
}

/*
 * Return true if an argument is completely numeric.
 */
bool is_number(char *arg)
{
	if (*arg == '\0')
		return false;

	if (*arg == '+' || *arg == '-')
		arg++;

	for (; *arg != '\0'; arg++)
	{
		if (!isdigit(*arg))
			return false;
	}

	return true;
}

bool is_alphanum(char *arg)
{
	if (*arg == '\0')
		return false;

	for (; *arg != '\0'; arg++)
	{
		if (!isdigit(*arg) && !isalpha(*arg))
			return false;
	}

	return true;
}

int have_space(char *arg)
{
	int space = 0;

	if (*arg == '\0')
		return 0;

	for (; *arg != '\0'; arg++)
	{
		if (*arg == ' ')
			space++;
	}

	return space;
}

bool have_schar(char *arg)
{
	if (*arg == '\0')
		return false;

	for (; *arg != '\0'; arg++)
	{
		if (!isdigit(*arg) && !isalpha(*arg) && *arg != ' ')
			return true;
	}

	return false;
}

/*
 * Given a string like 14.foo, return 14 and 'foo'
 */
int number_argument(char *argument, char *arg)
{
	char *pdot;
	int number;

	for (pdot = argument; *pdot != '\0'; pdot++)
	{
		if (*pdot == '.')
		{
			*pdot = '\0';
			number = atoi(argument);
			*pdot = '.';

			strcpy(arg, pdot + 1);
			return number;
		}
	}

	strcpy(arg, argument);
	return 1;
}

/*
 * Given a string like 14*foo, return 14 and 'foo'
 */
int mult_argument(char *argument, char *arg)
{
	char *pdot;
	int number;

	for (pdot = argument; *pdot != '\0'; pdot++)
	{
		if (*pdot == '*')
		{
			*pdot = '\0';
			number = atoi(argument);
			*pdot = '*';

			strcpy(arg, pdot + 1);
			return number;
		}
	}

	strcpy(arg, argument);
	return 1;
}

/*
 * Pick off one argument from a string and return the rest.
 * Understands quotes.
 */
char *one_argument(char *argument, char *arg_first)
{
	char cEnd;

	while (isspace(*argument))
		argument++;

	cEnd = ' ';
	if (*argument == '\'' || *argument == '"')
		cEnd = *argument++;

	while (*argument != '\0')
	{
		if (*argument == cEnd)
		{
			argument++;
			break;
		}

		*arg_first = LOWER(*argument);
		arg_first++;
		argument++;
	}

	*arg_first = '\0';

	while (isspace(*argument))
	{
		argument++;
	}

	return argument;
}

/*
 * Contributed by Alander.
 */
void do_commands(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];
	int cmd;
	int col;

	col = 0;

	for (cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++)
	{
		if (cmd_table[cmd].level < LEVEL_HERO && cmd_table[cmd].level <= get_trust(ch) && cmd_table[cmd].show)
		{
			sprintf(buf, "%-12s", cmd_table[cmd].name);
			send_to_char(buf, ch);

			if (++col % 6 == 0)
				send_to_char("\n\r", ch);
		}
	}

	if (col % 6 != 0)
		send_to_char("\n\r", ch);
}

void do_wizhelp(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];
	int cmd;
	int col, showval = 0, arrangeListLooper = LEVEL_HERO;
	bool showlevel= false;
	col = 0;

	if (is_number(argument))
	{
		showlevel = true;
		showval = atoi(argument);
	}

	if (showlevel && showval > get_trust(ch))
		return;

	for (cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++)
	{
		if ((!showlevel && cmd_table[cmd].level >= LEVEL_HERO && cmd_table[cmd].level <= get_trust(ch))
			|| (showlevel && cmd_table[cmd].level == showval)
			&& cmd_table[cmd].level == arrangeListLooper)
		{
			if (!showlevel && argument[0] != '\0' && str_prefix(argument, cmd_table[cmd].name))
				continue;

			sprintf(buf, "(%i)%-18s  ", cmd_table[cmd].level, cmd_table[cmd].name);
			send_to_char(buf, ch);

			if (++col % 5 == 0)
				send_to_char("\n\r", ch);

			if (cmd_table[cmd + 1].name[0] == '\0' && arrangeListLooper < MAX_LEVEL)
			{
				arrangeListLooper++;
				cmd = 0;
			}
		}
	}

	if (col % 5 != 0)
		send_to_char("\n\r", ch);
}
