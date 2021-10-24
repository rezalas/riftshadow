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

#ifndef TABLES_H
#define TABLES_H

#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include "merc.h"
#include "material.h"
#include "weather_enums.h"

struct flag_type
{
	char *name;
	long bit;
	bool settable;
};

struct display_type
{
	char *name;
	long bit;
	char *display;
};

struct mod_name_type
{
	char *name;
	long type;
};

struct cabal_type
{
	char *name;
	char *who_name;
	char *extitle;
	char *long_name;
	bool independent; /* true for loners */
	sh_int item_vnum;
	sh_int max_members;
	sh_int start_level;
};

struct cabal_list
{
	int cabal;
	char *skill;
	int level;
	bool specific;
};

struct position_type
{
	char *name;
	char *short_name;
};

struct sex_type
{
	char *name;
};

struct size_type
{
	char *name;
};

struct apply_type
{
	char *refname;
	int  location;
};

struct bounty_type
{
	char *rank_name;
	char *rank_extitle;
	int credits_required;
};

struct cabal_message
{
	char *login;
	char *logout;
	char *entrygreeting;
};

struct hometown_type
{
	char *name;
	int recall;
	int pit;
	int align;
	int ethos;
	int restricted;
};

struct color_type
{
	char *color_name;
	char *color_ascii;
};

struct color_event_type
{
	char *event_name;
	char *default_color;
	int min_level;
};

struct sect_type
{
	char *name;
	int value;
	int move_cost;
	int wait;
};

struct quest_type
{
	sh_int number;
	char *name;
	char *description;
	sh_int minlevel;
	sh_int maxlevel;
	char *stages[9];
};

struct tribe_type
{
	char *name;
	char *long_name;
	int tribe_num;
};

struct restrict_type
{
	char *name;
	long bit;
	int type;
	char *value;
};

struct modifier_type
{
	int value;
	float modifier;
};

struct style_type
{
	long bit;
	char *name;
};

struct style_list
{
	char *name;
	sh_int percent;
};

struct para_sphere
{
	char *first;
	char *second;
	char *para;
};

struct ele_sphere
{
	sh_int element;
	sh_int type;
	char *name;
};

struct climate_type
{
	char *name;
	sh_int number;
	sh_int skyfreqs[NUM_SEASONS][WeatherCondition::MaxWeatherCondition];
	sh_int tempfreqs[NUM_SEASONS][Temperature::MaxTemperature];
};

struct demon_type
{
	char *name;
	sh_int number;
	sh_int type;
};

struct beauty_type
{
	char *male;
	char *female;
};

struct order_list
{
	char * command;
};

struct prof_cmd_type
{
	char *name;
	DO_FUN *cmd;
	char *requires;
};

extern const struct flag_type aftype_table[];
extern const struct display_type apply_locations[];
extern const struct mod_name_type mod_names[];
extern const struct cabal_type cabal_table[MAX_CABAL];
extern const struct cabal_message cabal_messages[MAX_CABAL];
extern const struct position_type position_table[];
extern const struct sex_type sex_table[];
extern const struct size_type size_table[];
extern const struct flag_type act_flags[];
extern const struct flag_type plr_flags[];
extern const struct flag_type affect_flags[];
extern const struct flag_type off_flags[];
extern const struct flag_type imm_flags[];
extern const struct flag_type extra_flags[];
extern const struct flag_type wear_locations[];
extern const struct flag_type wear_flags[];
extern const struct flag_type form_flags[];
extern const struct flag_type part_flags[];
extern const struct flag_type comm_flags[];
extern const struct flag_type criterion_flags[];
extern const struct flag_type trap_table[];
extern const struct flag_type speech_table[];
extern const struct bounty_type bounty_table[MAX_BOUNTY];
extern const struct hometown_type hometown_table[MAX_HOMETOWN];
extern const struct color_type color_table[MAX_COLORS];
extern const struct color_event_type color_event[MAX_EVENTS];
extern const struct sect_type sect_table[];
extern const struct flag_type room_flags[];
extern const struct flag_type direction_table[];
extern const struct flag_type exit_flags[];
extern const struct flag_type area_type_table[];
extern const struct flag_type sky_table[WeatherCondition::MaxWeatherCondition];
extern const struct flag_type temp_table[Temperature::MaxTemperature];
extern const struct flag_type wind_table[Windspeed::MaxWindspeed];
extern const struct restrict_type restrict_table[];
extern const struct tribe_type tribe_table[];
extern const char* day_name[];
extern const char* month_name[];
extern const char* season_name[];
extern const struct para_sphere para_list[];
extern const struct ele_sphere sphere_table[];
extern const struct style_type style_table[];
extern const struct style_list style_percent[];
extern const std::vector<cabal_list> cabal_skills;
extern const struct flag_type wealth_table[];
extern const struct demon_type demon_table[];
extern const struct climate_type climate_table[Climate::MaxClimate];
extern const struct beauty_type beauty_table[];
extern const struct order_list order_table[];
extern const struct flag_type area_flags[];
extern const struct flag_type sex_flags[];
extern const struct flag_type door_resets[];
extern const struct flag_type sector_flags[];
extern const struct flag_type type_flags[];
extern const struct flag_type apply_flags[];
extern const struct flag_type wear_loc_strings[];
extern const struct flag_type wear_loc_flags[];
extern const struct flag_type weapon_flags[];
extern const struct flag_type container_flags[];
extern const struct flag_type ac_type[];
extern const struct flag_type size_flags[];
extern const struct flag_type weapon_class[];
extern const struct flag_type weapon_type2[];
extern const struct flag_type res_flags[];
extern const struct flag_type vuln_flags[];
extern const struct flag_type material_type[];
extern const struct flag_type position_flags[];
extern const struct flag_type portal_flags[];
extern const struct flag_type furniture_flags[];
extern const struct display_type altdesc_condtable[];
//extern const struct proficiency_type prof_table[];

#endif /* TABLES_H */
