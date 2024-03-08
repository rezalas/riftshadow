/***************************************************************************
 *  File: olc_act.c                                                        *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 *                                                                         *
 *  This code was freely distributed with the The Isles 1.1 source code,   *
 *  and has been used here for OLC - OLC would not be what it is without   *
 *  all the previous coders who released their source code.                *
 *                                                                         *
 ***************************************************************************/

#ifdef macintosh
#include <types.h>
#else
#include <sys/types.h>
#endif

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <algorithm>
#include "olc_act.h"
#include "handler.h"
#include "olc.h"
#include "tables.h"
#include "recycle.h"
#include "lookup.h"
#include "spec.h"
#include "misc.h"
#include "act_comm.h"
#include "act_info.h"
#include "act_move.h"
#include "act_wiz.h"
#include "characterClasses/warrior.h"
#include "comm.h"
#include "db.h"
#include "bit.h"
#include "string.h"
#include "aprog.h"
#include "iprog.h"
#include "mprog.h"
#include "rprog.h"
#include "chardef.h"
#include "const.h"
#include "material.h"
#include "iprog.h"
#include "mprog.h"
#include "rprog.h"
#include "aprog.h"
#include "help.h"
#include "mem.h"
#include "room.h"
#include "interp.h"
#include "newmem.h"
#include "magic.h"
#include "./include/spdlog/fmt/bundled/format.h"
#include "./include/spdlog/fmt/bundled/printf.h"


/*
 * This table contains help commands and a brief description of each.
 * ------------------------------------------------------------------
 */
const struct olc_help_type help_table[] =
{
	{"area", area_flags, "Area attributes."},
	{"room", room_flags, "Room attributes."},
	{"sector", sector_flags, "Sector types, terrain."},
	{"exit", exit_flags, "Exit types."},
	{"type", type_flags, "Types of objects."},
	{"extra", extra_flags, "Object attributes."},
	{"wear", wear_flags, "Where to wear object."},
	{"sex", sex_flags, "Sexes."},
	{"act", act_flags, "Mobile attributes."},
	{"affect", affect_flags, "Mobile affects."},
	{"wear-loc", wear_loc_flags, "Where mobile wears object."},
	{"spells", skill_table, "Names of current spells."},
	{"weapon", weapon_flags, "Type of weapon."},
	{"container", container_flags, "Container status."},

	/* ROM specific bits: */
	{"armor", ac_type, "Ac for different attacks."},
	{"apply", apply_flags, "Apply flags"},
	{"form", form_flags, "Mobile body form."},
	{"part", part_flags, "Mobile body parts."},
	{"imm", imm_flags, "Mobile immunity."},
	{"res", res_flags, "Mobile resistance."},
	{"vuln", vuln_flags, "Mobile vulnerability."},
	{"off", off_flags, "Mobile offensive behaviour."},
	{"size", size_flags, "Mobile size."},
	{"position", position_flags, "Mobile positions."},
	{"material", material_type, "Material mob/obj is made from."},
	{"wclass", weapon_class, "Weapon class."},
	{"wtype", weapon_type2, "Special weapon type."},
	{"portal", portal_flags, "Portal types."},
	{"furniture", furniture_flags, "Furniture types."},
	{"traps", trap_table, "Trap types."},
	{nullptr, nullptr, nullptr}
};

const struct wear_type wear_table[] =
{
	{WEAR_NONE, ITEM_TAKE},
	{WEAR_LIGHT, ITEM_LIGHT},
	{WEAR_FINGER_L, ITEM_WEAR_FINGER},
	{WEAR_FINGER_R, ITEM_WEAR_FINGER},
	{WEAR_NECK_1, ITEM_WEAR_NECK},
	{WEAR_NECK_2, ITEM_WEAR_NECK},
	{WEAR_BODY, ITEM_WEAR_BODY},
	{WEAR_HEAD, ITEM_WEAR_HEAD},
	{WEAR_LEGS, ITEM_WEAR_LEGS},
	{WEAR_FEET, ITEM_WEAR_FEET},
	{WEAR_HANDS, ITEM_WEAR_HANDS},
	{WEAR_ARMS, ITEM_WEAR_ARMS},
	{WEAR_SHIELD, ITEM_WEAR_SHIELD},
	{WEAR_ABOUT, ITEM_WEAR_ABOUT},
	{WEAR_WAIST, ITEM_WEAR_WAIST},
	{WEAR_WRIST_L, ITEM_WEAR_WRIST},
	{WEAR_WRIST_R, ITEM_WEAR_WRIST},
	{WEAR_WIELD, ITEM_WEAR_WIELD},
	{WEAR_HOLD, ITEM_WEAR_HOLD},
	{NO_FLAG, NO_FLAG}
};

bool show_version(CHAR_DATA *ch, char *argument)
{
	send_to_char(SHOW_VERSION, ch);
	send_to_char("\n\r", ch);
	send_to_char(SHOW_AUTHOR, ch);
	send_to_char("\n\r", ch);
	send_to_char(SHOW_DATE, ch);
	send_to_char("\n\r", ch);
	send_to_char(SHOW_CREDITS, ch);
	send_to_char("\n\r", ch);

	return false;
}

void show_flag_cmds(CHAR_DATA *ch, const struct flag_type *flag_table)
{
	char buf[MAX_STRING_LENGTH];
	char buf1[MAX_STRING_LENGTH];
	int flag;
	int col;

	buf1[0] = '\0';
	col = 0;

	for (flag = 0; flag_table[flag].name != nullptr; flag++)
	{
		if (flag_table[flag].settable)
		{
			sprintf(buf, "%-19.18s", flag_table[flag].name);
			strcat(buf1, buf);

			if (++col % 4 == 0)
				strcat(buf1, "\n\r");
		}
	}

	if (col % 4 != 0)
		strcat(buf1, "\n\r");

	send_to_char(buf1, ch);
}

void show_skill_cmds(CHAR_DATA *ch, int tar)
{
	char buf[MAX_STRING_LENGTH];
	char buf1[MAX_STRING_LENGTH * 2];
	int sn;
	int col;

	buf1[0] = '\0';
	col = 0;

	for (sn = 0; sn < MAX_SKILL; sn++)
	{
		if (!skill_table[sn].name)
			break;

		if (!str_cmp(skill_table[sn].name, "reserved") || skill_table[sn].spell_fun == spell_null)
			continue;

		if (tar == -1 || skill_table[sn].target == tar)
		{
			sprintf(buf, "%-19.18s", skill_table[sn].name);
			strcat(buf1, buf);

			if (++col % 4 == 0)
				strcat(buf1, "\n\r");
		}
	}

	if (col % 4 != 0)
		strcat(buf1, "\n\r");

	send_to_char(buf1, ch);
}

/*
void show_spec_cmds( CHAR_DATA *ch )
{
	char buf  [ MAX_STRING_LENGTH ];
	char buf1 [ MAX_STRING_LENGTH ];
	int  spec;
	int  col;

	buf1[0] = '\0';
	col = 0;
	send_to_char( "Precede special functions with 'spec_'\n\r\n\r", ch );
	for (spec = 0; spec_table[spec].function != nullptr; spec++)
	{
	sprintf( buf, "%-19.18s", &spec_table[spec].name[5] );
	strcat( buf1, buf );
	if ( ++col % 4 == 0 )
		strcat( buf1, "\n\r" );
	}

	if ( col % 4 != 0 )
	strcat( buf1, "\n\r" );

	send_to_char( buf1, ch );
	return;
}
*/

bool show_help(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];
	char arg[MAX_INPUT_LENGTH];
	char spell[MAX_INPUT_LENGTH];
	int cnt;

	argument = one_argument(argument, arg);
	one_argument(argument, spell);

	/*
	 * Display syntax.
	 */
	if (arg[0] == '\0')
	{
		send_to_char("Syntax:  ? [command]\n\r\n\r", ch);
		send_to_char("[command]  [description]\n\r", ch);

		for (cnt = 0; help_table[cnt].command != nullptr; cnt++)
		{
			sprintf(buf, "%-10.10s -%s\n\r", capitalize(help_table[cnt].command), help_table[cnt].desc);
			send_to_char(buf, ch);
		}

		return false;
	}

	/*
	 * Find the command, show changeable data.
	 * ---------------------------------------
	 */
	for (cnt = 0; help_table[cnt].command != nullptr; cnt++)
	{
		if (arg[0] == help_table[cnt].command[0] && !str_prefix(arg, help_table[cnt].command))
		{
			if (help_table[cnt].structure == skill_table)
			{

				if (spell[0] == '\0')
				{
					send_to_char("Syntax:  ? spells [ignore/attack/defend/self/object/all]\n\r", ch);
					return false;
				}

				if (!str_prefix(spell, "all"))
					show_skill_cmds(ch, -1);
				else if (!str_prefix(spell, "ignore"))
					show_skill_cmds(ch, TAR_IGNORE);
				else if (!str_prefix(spell, "attack"))
					show_skill_cmds(ch, TAR_CHAR_OFFENSIVE);
				else if (!str_prefix(spell, "defend"))
					show_skill_cmds(ch, TAR_CHAR_DEFENSIVE);
				else if (!str_prefix(spell, "self"))
					show_skill_cmds(ch, TAR_CHAR_SELF);
				else if (!str_prefix(spell, "object"))
					show_skill_cmds(ch, TAR_OBJ_INV);
				else
					send_to_char("Syntax:  ? spell [ignore/attack/defend/self/object/all]\n\r", ch);

				return false;
			}
			else
			{
				show_flag_cmds(ch, (flag_type *)help_table[cnt].structure);
				return false;
			}
		}
	}

	show_help(ch, "");
	return false;
}

bool redit_rlist(CHAR_DATA *ch, char *argument)
{
	ROOM_INDEX_DATA *pRoomIndex;
	AREA_DATA *pArea;
	char buf[MAX_STRING_LENGTH];
	BUFFER *buf1;
	char arg[MAX_INPUT_LENGTH];
	bool found;
	int vnum;
	int col = 0;

	one_argument(argument, arg);

	pArea = ch->in_room->area;
	buf1 = new_buf();
	/*    buf1[0] = '\0'; */
	found = false;

	for (vnum = pArea->min_vnum; vnum <= pArea->max_vnum; vnum++)
	{
		pRoomIndex = get_room_index(vnum);
		if (pRoomIndex)
		{
			found = true;

			sprintf(buf, "[%5d] %-17.16s", vnum, capitalize(pRoomIndex->name));
			add_buf(buf1, buf);

			if (++col % 3 == 0)
				add_buf(buf1, "\n\r");
		}
	}

	if (!found)
	{
		send_to_char("Room(s) not found in this area.\n\r", ch);
		return false;
	}

	if (col % 3 != 0)
		add_buf(buf1, "\n\r");

	page_to_char(buf_string(buf1), ch);
	free_buf(buf1);
	return false;
}

bool redit_mview(CHAR_DATA *ch, char *argument)
{
	MOB_INDEX_DATA *pMob;
	AREA_DATA *pArea;
	char buf[MSL], arg[MSL];
	int vnum;

	one_argument(argument, arg);

	if (arg[0] == '\0' || (vnum = atoi(arg)) == -1)
	{
		send_to_char("Syntax:  mview <vnum>\n\r", ch);
		return false;
	}

	pArea = ch->in_room->area;

	if (vnum < pArea->min_vnum || vnum > pArea->max_vnum)
	{
		send_to_char("You can only view mobiles within your area range.\n\r", ch);
		return false;
	}

	pMob = get_mob_index(vnum);
	if (pMob == nullptr)
		return false;

	send_to_char("Viewing mobile:\n\r", ch);

	sprintf(buf, "Vnum:     [%d]\n\r", vnum);
	send_to_char(buf, ch);

	sprintf(buf, "Name:     [%s]\n\r", pMob->player_name);
	send_to_char(buf, ch);

	sprintf(buf, "Short:    [%s]\n\r", pMob->short_descr);
	send_to_char(buf, ch);

	sprintf(buf, "Long:     [%s]\n\r", pMob->long_descr);
	send_to_char(buf, ch);

	sprintf(buf, "Desc:\n\r%s\n\r", pMob->description);
	send_to_char(buf, ch);

	sprintf(buf, "Level:    [%d]\n\r", pMob->level);
	send_to_char(buf, ch);
	return false;
}

bool redit_mlist(CHAR_DATA *ch, char *argument)
{
	MOB_INDEX_DATA *pMobIndex;
	AREA_DATA *pArea;
	char buf[MAX_STRING_LENGTH];
	BUFFER *buf1;
	char arg[MAX_INPUT_LENGTH];
	bool fAll, found;
	int vnum;
	int col = 0;

	one_argument(argument, arg);

	if (arg[0] == '\0')
	{
		send_to_char("Syntax:  mlist <all/name>\n\r", ch);
		return false;
	}

	buf1 = new_buf();
	pArea = ch->in_room->area;
	/*    buf1[0] = '\0'; */
	fAll = !str_cmp(arg, "all");
	found = false;

	for (vnum = pArea->min_vnum; vnum <= pArea->max_vnum; vnum++)
	{
		pMobIndex = get_mob_index(vnum);

		if (pMobIndex != nullptr)
		{
			if (fAll || is_name(arg, pMobIndex->player_name))
			{
				found = true;

				sprintf(buf, "[%5d] %-17.16s", pMobIndex->vnum, capitalize(pMobIndex->short_descr));
				add_buf(buf1, buf);

				if (++col % 3 == 0)
					add_buf(buf1, "\n\r");
			}
		}
	}

	if (!found)
	{
		send_to_char("Mobile(s) not found in this area.\n\r", ch);
		return false;
	}

	if (col % 3 != 0)
		add_buf(buf1, "\n\r");

	page_to_char(buf_string(buf1), ch);
	free_buf(buf1);
	return false;
}

bool redit_olist(CHAR_DATA *ch, char *argument)
{
	OBJ_INDEX_DATA *pObjIndex;
	AREA_DATA *pArea;
	char buf[MAX_STRING_LENGTH];
	BUFFER *buf1;
	char arg[MAX_INPUT_LENGTH];
	bool fAll, found;
	int vnum;
	int col = 0;

	one_argument(argument, arg);

	if (arg[0] == '\0')
	{
		send_to_char("Syntax:  olist <all/name/item_type>\n\r", ch);
		return false;
	}

	pArea = ch->in_room->area;
	buf1 = new_buf();
	/*    buf1[0] = '\0'; */
	fAll = !str_cmp(arg, "all");
	found = false;

	for (vnum = pArea->min_vnum; vnum <= pArea->max_vnum; vnum++)
	{
		pObjIndex = get_obj_index(vnum);

		if (pObjIndex)
		{
			if (fAll || is_name(arg, pObjIndex->name) || flag_value(type_flags, arg) == pObjIndex->item_type)
			{
				found = true;

				sprintf(buf, "[%5d] %-17.16s", pObjIndex->vnum, capitalize(pObjIndex->short_descr));
				add_buf(buf1, buf);

				if (++col % 3 == 0)
					add_buf(buf1, "\n\r");
			}
		}
	}

	if (!found)
	{
		send_to_char("Object(s) not found in this area.\n\r", ch);
		return false;
	}

	if (col % 3 != 0)
		add_buf(buf1, "\n\r");

	page_to_char(buf_string(buf1), ch);
	free_buf(buf1);
	return false;
}

bool redit_mshow(CHAR_DATA *ch, char *argument)
{
	MOB_INDEX_DATA *pMob;
	int value;

	if (argument[0] == '\0')
	{
		send_to_char("Syntax:  mshow <vnum>\n\r", ch);
		return false;
	}

	if (!is_number(argument))
	{
		send_to_char("REdit: vnum has to be a number.\n\r", ch);
		return false;
	}

	if (is_number(argument))
	{
		value = atoi(argument);
		pMob = get_mob_index(value);
		if (!(pMob))
		{
			send_to_char("REdit:  That mobile does not exist.\n\r", ch);
			return false;
		}

		ch->desc->pEdit = (void *)pMob;
	}

	medit_show(ch, argument);
	ch->desc->pEdit = (void *)ch->in_room;
	return false;
}

bool redit_oshow(CHAR_DATA *ch, char *argument)
{
	OBJ_INDEX_DATA *pObj;
	int value;

	if (argument[0] == '\0')
	{
		send_to_char("Syntax:  oshow <vnum>\n\r", ch);
		return false;
	}

	if (!is_number(argument))
	{
		send_to_char("REdit: vnum has to be a number.\n\r", ch);
		return false;
	}

	if (is_number(argument))
	{
		value = atoi(argument);
		pObj = get_obj_index(value);
		if (!(pObj))
		{
			send_to_char("REdit:  That object does not exist.\n\r", ch);
			return false;
		}

		ch->desc->pEdit = (void *)pObj;
	}

	oedit_show(ch, argument);
	ch->desc->pEdit = (void *)ch->in_room;
	return false;
}

bool check_range(int lower, int upper)
{
	AREA_DATA *pArea;
	int cnt = 0;

	for (pArea = area_first; pArea; pArea = pArea->next)
	{
		/*
		 * lower < area < upper
		 */
		if ((lower <= pArea->min_vnum && pArea->min_vnum <= upper)
			|| (lower <= pArea->max_vnum && pArea->max_vnum <= upper))
		{
			++cnt;
		}

		if (cnt > 1)
			return false;
	}

	return true;
}

AREA_DATA *get_vnum_area(int vnum)
{
	AREA_DATA *pArea;

	for (pArea = area_first; pArea; pArea = pArea->next)
	{
		if (vnum >= pArea->min_vnum && vnum <= pArea->max_vnum)
			return pArea;
	}

	return 0;
}

bool medit_vnum(CHAR_DATA *ch, char *argument)
{
	MOB_INDEX_DATA *pMob;
	int vnum;

	EDIT_MOB(ch, pMob);

	return false;

	if (!*argument || !is_number(argument))
	{
		send_to_char("Syntax: vnum <vnum>\n\r", ch);
		return false;
	}

	vnum = atoi(argument);

	if (vnum >= ch->in_room->area->min_vnum && vnum <= ch->in_room->area->max_vnum)
	{
		pMob->vnum = vnum;
		send_to_char("Vnum set.\n\r", ch);
		return true;
	}
	else
	{
		send_to_char("That vnum is not within the area range.\n\r", ch);
		return false;
	}

	return false;
}

bool medit_group(CHAR_DATA *ch, char *argument)
{
	MOB_INDEX_DATA *pMob;

	EDIT_MOB(ch, pMob);

	if (!*argument || !is_number(argument))
	{
		send_to_char("Syntax:  group <xxyy>  - First vnum in your area\n\r", ch);
		return false;
	}

	pMob->group = atoi(argument);
	send_to_char("Group set.\n\r", ch);
	return true;
}

bool medit_speech(CHAR_DATA *ch, char *argument)
{
	MOB_INDEX_DATA *pMobIndex;
	SPEECH_DATA *speech, *sptr;
	char buf[MSL], cmd[MSL], name[MSL], arg3[MSL], arg4[MSL], arg5[MSL];
	LINE_DATA *lptr, *new_line;
	int type;

	EDIT_MOB(ch, pMobIndex);

	if (!*argument)
	{
		send_to_char("The following speech commands are available:\n\r", ch);
		send_to_char("   speech list\n\r", ch);
		send_to_char("   speech add <speech_name>\n\r", ch);
		send_to_char("   speech del <speech_name>\n\r", ch);
		send_to_char("   speech <speech_name> line add <#delay> <type> <text>\n\r", ch);
		send_to_char("   speech <speech_name> line list\n\r", ch);
		send_to_char("   speech <speech_name> line <#> text <text>\n\r", ch);
		send_to_char("   speech <speech_name> line <#> delay <#>\n\r", ch);
		send_to_char("   speech <speech_name> line <#> type <type>\n\r", ch);
		send_to_char("   speech <speech_name> line <#> delete\n\r", ch);
		/* list commands */
	}
	else
	{
		argument = one_argument(argument, cmd);
		argument = one_argument(argument, name);
		argument = one_argument(argument, arg3);

		speech = find_speech(pMobIndex, cmd);

		if (speech != nullptr)
		{
			sort_speech(speech);

			if (!str_prefix(name, "line"))
			{
				if (!str_prefix(arg3, "list"))
				{
					if (!speech->first_line)
					{
						send_to_char("This speech has no lines.\n\r", ch);
						return false;
					}

					sprintf(buf, "%s Lines:\n\r", speech->name);
					send_to_char(buf, ch);

					for (lptr = speech->first_line; lptr; lptr = lptr->next)
					{
						sprintf(buf, "Line #%d: %d %s %s\n\r",
							lptr->number,
							lptr->delay,
							flag_name_lookup(lptr->type, speech_table),
							lptr->text);
						send_to_char(buf, ch);
					}
				}
				else if (!str_prefix(arg3, "add"))
				{
					argument = one_argument(argument, arg4);
					argument = one_argument(argument, arg5);
					new_line = new_line_data();

					new_line->delay = atoi(arg4);
					type = flag_lookup(arg5, speech_table);

					if (type == NO_FLAG)
					{
						send_to_char("Not a valid type.\n\r", ch);
						return false;
					}

					new_line->type = type;
					new_line->text = palloc_string(argument);

					for (lptr = speech->first_line; lptr->next != nullptr; lptr = lptr->next)
					{
						lptr->next = new_line;
					}

					send_to_char("Added.\n\r", ch);
					return true;
				}
				else
				{
					argument = one_argument(argument, arg4);

					for (lptr = speech->first_line; lptr; lptr = lptr->next)
					{
						if (lptr->number == atoi(arg3))
							break;
					}

					if (!lptr)
					{
						send_to_char("Not a valid line number.\n\r", ch);
						return false;
					}
					else
					{
						if (!str_cmp(arg4, "delete"))
						{
							delete lptr;
							send_to_char("Deleted.\n\r", ch);
							return true;
						}
						else if (!str_prefix(arg4, "delay"))
						{
							lptr->delay = atoi(argument);
							send_to_char("Delay set.\n\r", ch);
							return true;
						}
						else if (!str_prefix(arg4, "number"))
						{
							lptr->number = atoi(argument);
							send_to_char("Number set.\n\r", ch);
							return true;
						}
						else if (!str_prefix(arg4, "type"))
						{
							type = flag_lookup(argument, speech_table);

							if (type == NO_FLAG)
							{
								send_to_char("Not a valid type.\n\r", ch);
								return false;
							}

							lptr->type = type;
							send_to_char("Type set.\n\r", ch);
							return true;
						}
						else if (!str_prefix(arg4, "text"))
						{
							free_pstring(lptr->text);
							lptr->text = palloc_string(argument);

							send_to_char("Text set.\n\r", ch);
							return true;
						}
						else
						{
							send_to_char("Not a valid entry.\n\r", ch);
							return false;
						}
					}
				}
			}
		}
		else if (!str_prefix(cmd, "list"))
		{
			if (!pMobIndex->speech)
			{
				send_to_char("Mobile has no speeches defined.\n\r", ch);
				return false;
			}

			send_to_char("Mobile has the following speeches defined:\n\r", ch);

			for (sptr = pMobIndex->speech; sptr; sptr = sptr->next)
			{
				sprintf(buf, "%s\n\r", sptr->name);
				send_to_char(buf, ch);
			}
		}
		else if (!str_prefix(cmd, "add"))
		{
			if (!*name)
			{
				send_to_char("You must specify a name for the new speech.\n\r", ch);
				return false;
			}

			speech = new_speech_data();
			speech->name = palloc_string(name);

			if (!pMobIndex->speech)
			{
				pMobIndex->speech = speech;
			}
			else
			{
				for (sptr = pMobIndex->speech; sptr->next; sptr = sptr->next);
				sptr->next = speech;
				speech->prev = sptr;
			}

			send_to_char("Speech added.\n\r", ch);
			return true;
		}
		else if (!str_prefix(cmd, "del"))
		{
			if (!*name)
			{
				send_to_char("Delete which speech?\n\r", ch);
				return false;
			}

			speech = find_speech(pMobIndex, name);

			if (!speech)
			{
				send_to_char("No speech by that name found.\n\r", ch);
				return false;
			}

			free_speech(speech);
			send_to_char("Speech deleted.\n\r", ch);
			return true;
		}
		else if (!str_prefix(cmd, "edit"))
		{
			/* blah */
		}
		else
		{
			send_to_char("Invalid speech command.\n\r", ch);
			return false;
		}

		if (speech)
			sort_speech(speech);
	}

	return true;
}

bool medit_prog(CHAR_DATA *ch, char *argument)
{
	int count = 0, col = 0;
	char buf[MSL], add[MSL], prog[MSL], prog_type[MSL];
	std::string buffer;
	MOB_INDEX_DATA *pMobIndex;
	bool found = false;

	EDIT_MOB(ch, pMobIndex);

	if (!*argument)
	{
		send_to_char("The following mob progs are available:\n\r", ch);

		for (count = 0; mprog_table[count].type != nullptr; count++)
		{
			if (ch->pcdata->security < 10)
			{
				if (!str_cmp(ch->in_room->area->name, mprog_table[count].owner))
				{
					sprintf(buf, "%s ", mprog_table[count].name);
					send_to_char(buf, ch);
					col++;
				}
			}
			else
			{
				sprintf(buf, "%s ", mprog_table[count].name);
				send_to_char(buf, ch);
				col++;
			}

			if (col % 4 == 0 && col > 0)
				send_to_char("\n\r", ch);
		}
		if (col % 4 != 0)
			send_to_char("\n\r", ch);
	}
	else
	{
		argument = one_argument(argument, add);
		argument = one_argument(argument, prog);

		if (!*prog)
		{
			send_to_char("Syntax: prog add <prog_name>\n\r" \
						 "        prog del <prog_name>\n\r", ch);
			return false;
		}

		for (count = 0; mprog_table[count].type != nullptr; count++)
		{
			if (!str_cmp(mprog_table[count].name, prog))
			{
				sprintf(prog_type, "%s", mprog_table[count].type);
				found = true;
			}
		}

		if (!found)
		{
			buffer = fmt::format("{} is not a valid program to choose from.\n\r", prog); //TODO: change the rest of the sprintf calls to format
			send_to_char(buffer.c_str(), ch);
			return false;
		}
		else if (!str_prefix(add, "add"))
		{
			if (pMobIndex->mprogs == nullptr)
			{
				pMobIndex->mprogs = new MPROG_DATA;
				CLEAR_MEM(pMobIndex->mprogs, sizeof(MPROG_DATA))
			}

			mprog_set(pMobIndex, prog_type, prog);

			buffer = fmt::format("The '{}' field for this mobile as been set to the '{}' program.\n\r", prog_type, prog);
			send_to_char(buffer.c_str(), ch);
		}
		else if (!str_prefix(add, "del"))
		{
			if (pMobIndex->mprogs && mprog_unset(pMobIndex, prog_type, prog))
			{
				buffer = fmt::format("The '{}' field for this mobile has been cleared.\n\r", prog_type);
				send_to_char(buffer.c_str(), ch);
			}
			else
			{
				buffer = fmt::format("The '{}' field is not set.\n\r", prog_type);
				send_to_char(buffer.c_str(), ch);
				return false;
			}
		}
	}

	return true;
}

bool oedit_prog(CHAR_DATA *ch, char *argument)
{
	int count = 0, col = 0;
	char buf[MSL], add[MSL], prog[MSL], prog_type[MSL];
	std::string buffer;
	OBJ_INDEX_DATA *pObjIndex;
	bool found = false;

	EDIT_OBJ(ch, pObjIndex);

	if (!*argument)
	{
		send_to_char("The following object programs are available:\n\r", ch);

		for (count = 0; iprog_table[count].type != nullptr; count++)
		{
			if (ch->pcdata->security < 10)
			{
				if (!str_cmp(ch->in_room->area->name, iprog_table[count].owner))
				{
					sprintf(buf, "%s ", iprog_table[count].name);
					send_to_char(buf, ch);
					col++;
				}
			}
			else
			{
				sprintf(buf, "%s ", iprog_table[count].name);
				send_to_char(buf, ch);
				col++;
			}

			if (col % 4 == 0 && col > 0)
				send_to_char("\n\r", ch);
		}
		if (col % 4 != 0)
			send_to_char("\n\r", ch);
	}
	else
	{
		argument = one_argument(argument, add);
		argument = one_argument(argument, prog);

		if (!*prog)
		{
			send_to_char("Syntax: prog add <prog_name>\n\r"
						 "        prog del <prog_name>\n\r",
						 ch);
			return false;
		}

		if (pObjIndex->iprogs == nullptr)
		{
			pObjIndex->iprogs = new IPROG_DATA;
			CLEAR_MEM(pObjIndex->iprogs, sizeof(IPROG_DATA))
		}

		for (count = 0; iprog_table[count].type != nullptr; count++)
		{
			if (!str_cmp(iprog_table[count].name, prog))
			{
				sprintf(prog_type, "%s", iprog_table[count].type);
				found = true;
			}
		}

		if (!found)
		{
			buffer = fmt::format("{} is not a valid program to choose from.\n\r", prog); //TODO: change the rest of the sprintf calls to format
			send_to_char(buffer.c_str(), ch);
			return false;
		}
		else if (!str_prefix(add, "add"))
		{
			iprog_set(pObjIndex, prog_type, prog);
			buffer = fmt::format("The '{}' field for this object as been set to the '{}' program.\n\r", prog_type, prog); 
			send_to_char(buffer.c_str(), ch);
		}
		else if (!str_prefix(add, "del"))
		{
			if (iprog_unset(pObjIndex, prog_type, prog))
			{
				buffer = fmt::format("The '{}' field for this mobile has been cleared.\n\r", prog_type);
				send_to_char(buffer.c_str(), ch);
			}
			else
			{
				buffer = fmt::format("The '{}' field is not set.\n\r", prog_type);
				send_to_char(buffer.c_str(), ch);
				return false;
			}
		}
	}

	return true;
}

bool oedit_spec(CHAR_DATA *ch, char *argument)
{
	int count = 0;
	char buf[MSL], add[MSL], prog[MSL];
	std::string buffer;
	OBJ_INDEX_DATA *pObjIndex;

	EDIT_OBJ(ch, pObjIndex);

	argument = one_argument(argument, add);
	argument = one_argument(argument, prog);

	if (!*add || !*prog)
	{
		send_to_char("Syntax: spec add <prog_name>\n\r"\
					 "        spec del <prog_name>\n\r",
					 ch);
		return false;
	}

	for (count = 0; ispec_table[count].spec_name; count++)
	{
		if (!str_cmp(ispec_table[count].spec_name, prog))
			break;
	}

	if (!ispec_table[count].spec_name)
	{
		buffer = fmt::format("{} is not a valid program to choose from.\n\r", prog); //TODO: change the rest of the sprintf calls to format
		send_to_char(buffer.c_str(), ch);
		return false;
	}
	else if (!str_prefix(add, "add"))
	{
		pObjIndex->spec_prog.func = ispec_table[count].spec_func;
		pObjIndex->spec_prog.trapvector = ispec_table[count].spec_events;
		buffer = fmt::format("Object spec prog has been set to '{}'.\n\r", prog);
		send_to_char(buffer.c_str(), ch);
	}
	else if (!str_prefix(add, "del"))
	{
		if (pObjIndex->spec_prog.func)
		{
			pObjIndex->spec_prog.func = nullptr;
			pObjIndex->spec_prog.trapvector = 0;
			sprintf(buf, "The item spec for this item has been cleared.\n\r");
			send_to_char(buf, ch);
		}
		else
		{
			send_to_char("That is not a valid item spec.\n\r", ch);
			return false;
		}
	}

	return true;
}

bool redit_trap(CHAR_DATA *ch, char *argument)
{
	ROOM_INDEX_DATA *pRoom;
	TRAP_DATA *trap;
	char arg1[MSL];
	int num;

	EDIT_ROOM(ch, pRoom);

	argument = one_argument(argument, arg1);

	if (pRoom->trap && arg1[0] != '\0')
	{
		if (!str_cmp(arg1, "type"))
		{
			num = flag_lookup(argument, trap_table);

			if (num == NO_FLAG)
			{
				send_to_char("That is not a valid trap.\n\r", ch);
				return false;
			}

			pRoom->trap->type = num;
			send_to_char("Trap type updated.\n\r", ch);
		}
		else if (!str_cmp(arg1, "quality"))
		{
			num = atoi(argument);

			if (num > 0 && num < 11)
			{
				pRoom->trap->quality = num;
				send_to_char("Trap quality updated.\n\r", ch);
				return true;
			}
			else
			{
				send_to_char("Quality must be a number between 1 and 10.\n\r", ch);
				return false;
			}
		}
		else if (!str_cmp(arg1, "complexity"))
		{
			num = atoi(argument);

			if (num > 0 || num < 101)
			{
				pRoom->trap->complexity = num;
				send_to_char("Trap complexity updated.\n\r", ch);
				return true;
			}
			else
			{
				send_to_char("Complexity must be a number between 1 and 100.\n\r", ch);
				return false;
			}
		}
		else if (!str_cmp(arg1, "timer"))
		{
			num = atoi(argument);

			if (num >= 0 && num < 16)
			{
				pRoom->trap->timer = num;
				send_to_char("Trap timer updated.\n\r", ch);
				return true;
			}
			else
			{
				send_to_char("Timer must be a number between 0 (immediate) and 15 seconds.\n\r", ch);
				return false;
			}
		}
		else if (!str_cmp(arg1, "trigger"))
		{
			pRoom->trap->trig_echo = palloc_string(argument);
			send_to_char("Trigger echo updated.\n\r", ch);
			return true;
		}
		else if (!str_cmp(arg1, "execute"))
		{
			pRoom->trap->exec_echo = palloc_string(argument);
			send_to_char("Execute echo updated.\n\r", ch);
			return true;
		}
		else if (!str_cmp(arg1, "delete"))
		{
			free_trap(pRoom->trap);
			pRoom->trap = nullptr;
			send_to_char("Trap deleted.\n\r", ch);
			return true;
		}
		else
		{
			redit_trap(ch, "");
			return false;
		}
	}
	else if (!pRoom->trap && !str_cmp(arg1, "new"))
	{
		trap = new_trap();
		trap->room = pRoom;
		trap->type = TRAP_NONE;
		trap->quality = 0;
		trap->complexity = 0;
		trap->timer = 0;
		trap->trig_echo = nullptr;
		trap->exec_echo = nullptr;
		trap->armed = false;
		pRoom->trap = trap;
		send_to_char("Trap created.\n\r", ch);
		return true;
	}
	else
	{
		if (pRoom->trap)
		{
			send_to_char("Syntax:  trap type <type>\n\r"\
						 "              quality <#1-10>\n\r"\
						 "              complexity <#1-10>\n\r"\
						 "              timer <#1-10>\n\r"\
						 "              trigger <mesg>\n\r"\
						 "              execute <mesg>\n\r"\
						 "              delete\n\r", ch);
		}
		else
			send_to_char("Syntax:  trap new\n\r", ch);

		return false;
	}

	return false;
}

bool redit_prog(CHAR_DATA *ch, char *argument)
{
	int count = 0, col = 0;
	char buf[MSL], add[MSL], prog[MSL], prog_type[MSL];
	std::string buffer;
	ROOM_INDEX_DATA *pRoomIndex;
	bool found = false;

	EDIT_ROOM(ch, pRoomIndex);

	if (!*argument)
	{
		send_to_char("The following room programs are available:\n\r", ch);

		for (count = 0; rprog_table[count].type != nullptr; count++)
		{
			if (ch->pcdata->security < 10)
			{
				if (!str_cmp(ch->in_room->area->name, rprog_table[count].owner))
				{
					sprintf(buf, "%s ", rprog_table[count].name);
					send_to_char(buf, ch);
					col++;
				}
			}
			else
			{
				sprintf(buf, "%s ", rprog_table[count].name);
				send_to_char(buf, ch);
				col++;
			}

			if (col % 4 == 0 && col > 0)
				send_to_char("\n\r", ch);
		}

		if (col % 4 != 0)
			send_to_char("\n\r", ch);
	}
	else
	{
		argument = one_argument(argument, add);
		argument = one_argument(argument, prog);

		if (!*prog)
		{
			send_to_char("Syntax: prog add <prog_name>\n\r"\
						 "        prog del <prog_name>\n\r", ch);
			return false;
		}

		if (pRoomIndex->rprogs == nullptr)
		{
			pRoomIndex->rprogs = new RPROG_DATA;
			CLEAR_MEM(pRoomIndex->rprogs, sizeof(RPROG_DATA))
		}

		for (count = 0; rprog_table[count].type != nullptr; count++)
		{
			if (!str_cmp(rprog_table[count].name, prog))
			{
				sprintf(prog_type, "%s", rprog_table[count].type);
				found = true;
			}
		}

		if (!found)
		{
			buffer = fmt::format("{} is not a valid program to choose from.\n\r", prog); //TODO: change the rest of the sprintf calls to format
			send_to_char(buffer.c_str(), ch);
			return false;
		}
		else if (!str_prefix(add, "add"))
		{
			rprog_set(pRoomIndex, prog_type, prog);
			buffer = fmt::format("The '{}' field for this room as been set to the '{}' program.\n\r", prog_type, prog);
			send_to_char(buffer.c_str(), ch);
		}
		else if (!str_prefix(add, "del"))
		{
			if (rprog_unset(pRoomIndex, prog_type, prog))
			{
				buffer = fmt::format("The '{}' field for this room has been cleared.\n\r", prog_type);
				send_to_char(buffer.c_str(), ch);
			}
			else
			{
				buffer = fmt::format("The '{}' field is not set.\n\r", prog_type);
				send_to_char(buffer.c_str(), ch);
				return false;
			}
		}
	}

	return true;
}

bool redit_altdesc(CHAR_DATA *ch, char *argument)
{
	ROOM_INDEX_DATA *pRoom;
	EDIT_ROOM(ch, pRoom);
	char arg1[MSL];

	if (!*argument)
	{
		send_to_char("Syntax: altdesc add <type>    - adds alternate description shown during <type>\n\r", ch);
		send_to_char("        altdesc title <title> - changes alternate room title to <title>\n\r", ch);
		send_to_char("        altdesc edit          - edits existing altdesc\n\r", ch);
		send_to_char("        altdesc delete        - deletes altdesc\n\r", ch);
		send_to_char("        altdesc types          - shows valid values for when altdesc is shown\n\r", ch);
		return false;
	}

	argument = one_argument(argument, arg1);

	if (!str_cmp(arg1, "delete") && pRoom->alt_description)
	{
		free_pstring(pRoom->alt_description);
		free_pstring(pRoom->alt_name);
		pRoom->alt_description_cond = 0;
		pRoom->alt_description = nullptr;

		send_to_char("Alternate description deleted.\n\r", ch);
		return true;
	}

	if (!str_cmp(arg1, "types"))
	{
		char buf[MSL];
		send_to_char("Alternate description types:\n\r", ch);

		for (int i = 0; altdesc_condtable[i].name != nullptr; i++)
		{
			sprintf(buf, "%s - %s.\n\r", altdesc_condtable[i].name, capitalize(altdesc_condtable[i].display));
			send_to_char(buf, ch);
		}

		return false;
	}

	if (!str_cmp(arg1, "add"))
	{
		if (!*argument)
		{
			send_to_char("You must specify a valid type of alternate description to add.\n\r", ch);
			return false;
		}

		if (pRoom->alt_description)
		{
			send_to_char("You must first delete the alternate description before adding a new one.\n\r", ch);
			return false;
		}

		int i;

		for (i = 0; altdesc_condtable[i].name != nullptr; i++)
		{
			if (!str_cmp(altdesc_condtable[i].name, argument))
				break;
		}

		if (!altdesc_condtable[i].name)
		{
			send_to_char("Invalid alternate description type.\n\r", ch);
			return false;
		}

		pRoom->alt_description = palloc_string("Default alternate description.");
		pRoom->alt_name = palloc_string(pRoom->name);
		pRoom->alt_description_cond = i;

		send_to_char("Alternate description added.\n\r", ch);
		return true;
	}

	if (!str_cmp(arg1, "edit"))
	{
		if (!pRoom->alt_description)
		{
			send_to_char("You must first create an alternate description with altdesc add before changing the description.\n\r", ch);
			return false;
		}

		string_append(ch, &pRoom->alt_description);
		return true;
	}

	if (!str_cmp(arg1, "title"))
	{
		if (!pRoom->alt_description)
		{
			send_to_char("You must create an alternate description with altdesc add before changing the title.\n\r", ch);
			return false;
		}

		if (!*argument)
		{
			send_to_char("You must specify a new title to set.\n\r", ch);
			return false;
		}

		free_pstring(pRoom->alt_name);
		pRoom->alt_name = palloc_string(argument);

		send_to_char("Alternate room title changed.\n\r", ch);
		return true;
	}

	redit_altdesc(ch, "");
	return false;
}

bool aedit_type(CHAR_DATA *ch, char *argument)
{
	AREA_DATA *pArea;
	int type = 0;

	EDIT_AREA(ch, pArea);

	if (!*argument)
	{
		send_to_char("Syntax:  type <type>\n\r", ch);
		return false;
	}

	type = flag_lookup(argument, area_type_table);
	if (type == NO_FLAG)
	{
		send_to_char("Not a valid type.\n\r", ch);
		return false;
	}

	pArea->area_type = type;
	send_to_char("Type set.\n\r", ch);

	return true;
}

bool aedit_prog(CHAR_DATA *ch, char *argument)
{
	int count = 0, col = 0;
	char buf[MSL], add[MSL], prog[MSL], prog_type[MSL];
	std::string buffer;
	AREA_DATA *pArea;
	bool found = false;

	EDIT_AREA(ch, pArea);

	if (!*argument)
	{
		send_to_char("The following area programs are available:\n\r", ch);

		for (count = 0; aprog_table[count].type != nullptr; count++)
		{
			if (ch->pcdata->security < 10)
			{
				if (!str_cmp(ch->in_room->area->name, aprog_table[count].owner))
				{
					sprintf(buf, "%s ", aprog_table[count].name);
					send_to_char(buf, ch);
					col++;
				}
			}
			else
			{
				sprintf(buf, "%s ", aprog_table[count].name);
				send_to_char(buf, ch);
				col++;
			}

			if (col % 4 == 0 && col > 0)
				send_to_char("\n\r", ch);
		}
		if (col % 4 != 0)
			send_to_char("\n\r", ch);
	}
	else
	{
		argument = one_argument(argument, add);
		argument = one_argument(argument, prog);

		if (!*prog)
		{
			send_to_char("Syntax: prog add <prog_name>\n\r"\
						 "        prog del <prog_name>\n\r", ch);
			return false;
		}

		if (pArea->aprogs == nullptr)
		{
			pArea->aprogs = new APROG_DATA;
			CLEAR_MEM(pArea->aprogs, sizeof(APROG_DATA))
		}

		for (count = 0; aprog_table[count].type != nullptr; count++)
		{
			if (!str_cmp(aprog_table[count].name, prog))
			{
				sprintf(prog_type, "%s", aprog_table[count].type);
				found = true;
			}
		}

		if (!found)
		{
			buffer = fmt::format("{} is not a valid program to choose from.\n\r", prog); //TODO: change the rest of the sprintf calls to format
			send_to_char(buffer.c_str(), ch);
			return false;
		}
		else if (!str_prefix(add, "add"))
		{
			aprog_set(pArea, prog_type, prog);
			buffer = fmt::format("The '{}' field for this area as been set to the '{}' program.\n\r", prog_type, prog);
			send_to_char(buffer.c_str(), ch);
		}
		else if (!str_prefix(add, "del"))
		{
			if (aprog_unset(pArea, prog_type, prog))
			{
				buffer = fmt::format("The '{}' field for this area has been cleared.\n\r", prog_type);
				send_to_char(buffer.c_str(), ch);
			}
			else
			{
				buffer = fmt::format("The '{}' field is not set.\n\r", prog_type);
				send_to_char(buffer.c_str(), ch);
				return false;
			}
		}
	}

	return true;
}

/*
 *  * Area Editor Functions.
 *
 *   */

bool aedit_security(CHAR_DATA *ch, char *argument)
{
	AREA_DATA *pArea;
	char sec[MAX_STRING_LENGTH];
	char buf[MAX_STRING_LENGTH];
	int value;

	EDIT_AREA(ch, pArea);

	one_argument(argument, sec);

	if (!is_number(sec) || sec[0] == '\0')
	{
		send_to_char("Syntax:  security [#xlevel]\n\r", ch);
		return false;
	}

	value = atoi(sec);

	if (value > ch->pcdata->security || value < 0)
	{
		if (ch->pcdata->security != 0)
		{
			sprintf(buf, "Security is 0-%d.\n\r", ch->pcdata->security);
			send_to_char(buf, ch);
		}
		else
		{
			send_to_char("Security is 0 only.\n\r", ch);
		}

		return false;
	}

	pArea->security = value;
	send_to_char("Security set.\n\r", ch);
	return true;
}

bool aedit_show(CHAR_DATA *ch, char *argument)
{
	AREA_DATA *pArea;
	char buf[MAX_STRING_LENGTH];

	EDIT_AREA(ch, pArea);

	sprintf(buf, "Name:     [%5d] %s\n\r", pArea->vnum, pArea->name);
	send_to_char(buf, ch);

	sprintf(buf, "File:     %s\n\r", pArea->file_name);
	send_to_char(buf, ch);

	sprintf(buf, "Vnums:    [%d-%d]\n\r", pArea->min_vnum, pArea->max_vnum);
	send_to_char(buf, ch);

	sprintf(buf, "Levels:   [%2d - %2d]\n\r", pArea->low_range, pArea->high_range);
	send_to_char(buf, ch);

	sprintf(buf, "Age:      [%d]\n\r", pArea->age);
	// send_to_char( buf, ch );
	sprintf(buf, "Type:     [%s]\n\r", area_type_table[pArea->area_type].name);
	send_to_char(buf, ch);

	sprintf(buf, "Players:  [%d]\n\r", pArea->nplayer);
	send_to_char(buf, ch);

	sprintf(buf, "Builders: [%s]\n\r", pArea->builders);
	send_to_char(buf, ch);

	sprintf(buf, "Credits:  [%s]\n\r", pArea->credits);
	send_to_char(buf, ch);

	sprintf(buf, "Security: [%d]\n\r", pArea->security);
	send_to_char(buf, ch);

	sprintf(buf, "Climate:  [%s]\n\r", climate_table[pArea->climate].name);
	send_to_char(buf, ch);

	sprintf(buf, "Flags:    [%s]\n\r", flag_string(area_flags, pArea->area_flags));
	send_to_char(buf, ch);

	if (!IS_ZERO_VECTOR(pArea->progtypes))
	{
		send_to_char("Area Programs:\n\r", ch);

		if (IS_SET(pArea->progtypes, APROG_PULSE))
		{
			sprintf(buf, " [pulse_prog] %s\n\r", pArea->aprogs->pulse_name);
			send_to_char(buf, ch);
		}

		if (IS_SET(pArea->progtypes, APROG_RESET))
		{
			sprintf(buf, " [reset_prog] %s\n\r", pArea->aprogs->reset_name);
			send_to_char(buf, ch);
		}

		if (IS_SET(pArea->progtypes, APROG_SUN))
		{
			sprintf(buf, " [sun_prog] %s\n\r", pArea->aprogs->sun_name);
			send_to_char(buf, ch);
		}

		if (IS_SET(pArea->progtypes, APROG_TICK))
		{
			sprintf(buf, " [tick_prog] %s\n\r", pArea->aprogs->tick_name);
			send_to_char(buf, ch);
		}

		if (IS_SET(pArea->progtypes, APROG_AGGRESS))
		{
			sprintf(buf, " [aggress_prog] %s\n\r", pArea->aprogs->aggress_name);
			send_to_char(buf, ch);
		}
		if (IS_SET(pArea->progtypes, APROG_MYELL))
		{
			sprintf(buf, " [myell_prog] %s\n\r", pArea->aprogs->myell_name);
			send_to_char(buf, ch);
		}
	}

	return false;
}

bool aedit_reset(CHAR_DATA *ch, char *argument)
{
	AREA_DATA *pArea;

	EDIT_AREA(ch, pArea);

	reset_area(pArea);
	send_to_char("Area reset.\n\r", ch);

	return false;
}

bool aedit_create(CHAR_DATA *ch, char *argument)
{
	AREA_DATA *pArea;

	if (get_security(ch) < 9)
	{
		send_to_char("You do not have the security to do that.\n\r", ch);
		return false;
	}

	pArea = new_area();
	area_last->next = pArea;
	area_last = pArea; /* Thanks, Walker. */
	ch->desc->pEdit = (void *)pArea;

	SET_BIT(pArea->area_flags, AREA_ADDED);
	send_to_char("Area Created.\n\r", ch);
	return false;
}

bool aedit_name(CHAR_DATA *ch, char *argument)
{
	AREA_DATA *pArea;

	EDIT_AREA(ch, pArea);

	if (argument[0] == '\0')
	{
		send_to_char("Syntax:   name [$name]\n\r", ch);
		return false;
	}

	free_pstring(pArea->name);
	pArea->name = palloc_string(argument);

	send_to_char("Name set.\n\r", ch);
	return true;
}

bool aedit_credits(CHAR_DATA *ch, char *argument)
{
	AREA_DATA *pArea;

	EDIT_AREA(ch, pArea);

	if (argument[0] == '\0')
	{
		send_to_char("Syntax:  credits [string]\n\r", ch);
		return false;
	}

	free_pstring(pArea->credits);
	pArea->credits = palloc_string(argument);

	send_to_char("Credits set.\n\r", ch);
	return true;
}

bool aedit_file(CHAR_DATA *ch, char *argument)
{
	AREA_DATA *pArea;
	char file[MAX_STRING_LENGTH];
	int i, length;

	EDIT_AREA(ch, pArea);

	one_argument(argument, file); /* Forces Lowercase */

	if (argument[0] == '\0')
	{
		send_to_char("Syntax:  filename [$file]\n\r", ch);
		return false;
	}

	/*
	 * Simple Syntax Check.
	 */
	length = strlen(argument);
	if (length > 16)
	{
		send_to_char("No more than eight characters allowed.\n\r", ch);
		return false;
	}

	/*
	 * Allow only letters and numbers.
	 */
	for (i = 0; i < length; i++)
	{
		if (!isalnum(file[i]))
		{
			send_to_char("Only letters and numbers are valid.\n\r", ch);
			return false;
		}
	}

	free_pstring(pArea->file_name);
	strcat(file, ".are");
	pArea->file_name = palloc_string(file);

	send_to_char("Filename set.\n\r", ch);
	return true;
}

bool aedit_level(CHAR_DATA *ch, char *argument)
{
	AREA_DATA *pArea;
	char lowc[MSL], highc[MSL];
	int low, high;

	EDIT_AREA(ch, pArea);

	argument = one_argument(argument, lowc);
	argument = one_argument(argument, highc);

	if (lowc[0] == '\0' || highc[0] == '\0' || !is_number(lowc) || !is_number(highc))
	{
		send_to_char("Syntax:  level [low] [high]\n\r", ch);
		return false;
	}

	low = atoi(lowc);
	high = atoi(highc);

	if (low < 1 || high > 51)
	{
		send_to_char("Level range is from 1 to 51.\n\r", ch);
		return false;
	}

	pArea->low_range = low;
	pArea->high_range = high;
	send_to_char("Level range set.\n\r", ch);
	return true;
}

bool aedit_age(CHAR_DATA *ch, char *argument)
{
	AREA_DATA *pArea;
	char age[MAX_STRING_LENGTH];

	EDIT_AREA(ch, pArea);

	one_argument(argument, age);

	if (!is_number(age) || age[0] == '\0')
	{
		send_to_char("Syntax:  age [#xage]\n\r", ch);
		return false;
	}

	pArea->age = atoi(age);

	send_to_char("Age set.\n\r", ch);
	return true;
}

bool aedit_builder(CHAR_DATA *ch, char *argument)
{
	AREA_DATA *pArea;
	char buf[MAX_STRING_LENGTH];

	EDIT_AREA(ch, pArea);

	if (argument[0] == '\0')
	{
		send_to_char("Syntax:  builder [$name]  -toggles builder\n\r", ch);
		send_to_char("Syntax:  builder All      -allows everyone\n\r", ch);
		return false;
	}

	argument[0] = UPPER(argument[0]);

	if (strstr(pArea->builders, argument) != (char *)'\0')
	{
		pArea->builders = string_replace(pArea->builders, argument, "\0");
		pArea->builders = string_unpad(pArea->builders);

		if (pArea->builders[0] == '\0')
		{
			free_pstring(pArea->builders);
			pArea->builders = palloc_string("None");
		}

		send_to_char("Builder removed.\n\r", ch);
		return true;
	}
	else
	{
		buf[0] = '\0';

		if (strstr(pArea->builders, "None") != (char *)'\0')
		{
			pArea->builders = string_replace(pArea->builders, "None", "\0");
			pArea->builders = string_unpad(pArea->builders);
		}

		if (pArea->builders[0] != '\0')
		{
			strcat(buf, pArea->builders);
			strcat(buf, " ");
		}

		strcat(buf, argument);
		free_pstring(pArea->builders);
		pArea->builders = string_proper(palloc_string(buf));

		send_to_char("Builder added.\n\r", ch);
		return true;
	}

	return false;
}

bool aedit_vnum(CHAR_DATA *ch, char *argument)
{
	AREA_DATA *pArea;
	char lower[MAX_STRING_LENGTH];
	char upper[MAX_STRING_LENGTH];
	int ilower;
	int iupper;

	EDIT_AREA(ch, pArea);

	argument = one_argument(argument, lower);
	one_argument(argument, upper);

	if (!is_number(lower) || lower[0] == '\0' || !is_number(upper) || upper[0] == '\0')
	{
		send_to_char("Syntax:  vnum [#xlower] [#xupper]\n\r", ch);
		return false;
	}

	if ((ilower = atoi(lower)) > (iupper = atoi(upper)))
	{
		send_to_char("AEdit:  Upper must be larger then lower.\n\r", ch);
		return false;
	}

	if (!check_range(atoi(lower), atoi(upper)))
	{
		send_to_char("AEdit:  Range must include only this area.\n\r", ch);
		return false;
	}

	if (get_vnum_area(ilower) && get_vnum_area(ilower) != pArea)
	{
		send_to_char("AEdit:  Lower vnum already assigned.\n\r", ch);
		return false;
	}

	pArea->min_vnum = ilower;
	send_to_char("Lower vnum set.\n\r", ch);

	if (get_vnum_area(iupper) && get_vnum_area(iupper) != pArea)
	{
		send_to_char("AEdit:  Upper vnum already assigned.\n\r", ch);
		return true; /* The lower value has been set. */
	}

	pArea->max_vnum = iupper;
	send_to_char("Upper vnum set.\n\r", ch);

	return true;
}

bool aedit_lvnum(CHAR_DATA *ch, char *argument)
{
	AREA_DATA *pArea;
	char lower[MAX_STRING_LENGTH];
	int ilower;
	int iupper;

	EDIT_AREA(ch, pArea);

	one_argument(argument, lower);

	if (!is_number(lower) || lower[0] == '\0')
	{
		send_to_char("Syntax:  min_vnum [#xlower]\n\r", ch);
		return false;
	}

	if ((ilower = atoi(lower)) > (iupper = pArea->max_vnum))
	{
		send_to_char("AEdit:  Value must be less than the max_vnum.\n\r", ch);
		return false;
	}

	if (!check_range(ilower, iupper))
	{
		send_to_char("AEdit:  Range must include only this area.\n\r", ch);
		return false;
	}

	if (get_vnum_area(ilower) && get_vnum_area(ilower) != pArea)
	{
		send_to_char("AEdit:  Lower vnum already assigned.\n\r", ch);
		return false;
	}

	pArea->min_vnum = ilower;
	send_to_char("Lower vnum set.\n\r", ch);
	return true;
}

bool aedit_uvnum(CHAR_DATA *ch, char *argument)
{
	AREA_DATA *pArea;
	char upper[MAX_STRING_LENGTH];
	int ilower;
	int iupper;

	EDIT_AREA(ch, pArea);

	one_argument(argument, upper);

	if (!is_number(upper) || upper[0] == '\0')
	{
		send_to_char("Syntax:  max_vnum [#xupper]\n\r", ch);
		return false;
	}

	if ((ilower = pArea->min_vnum) > (iupper = atoi(upper)))
	{
		send_to_char("AEdit:  Upper must be larger then lower.\n\r", ch);
		return false;
	}

	if (!check_range(ilower, iupper))
	{
		send_to_char("AEdit:  Range must include only this area.\n\r", ch);
		return false;
	}

	if (get_vnum_area(iupper) && get_vnum_area(iupper) != pArea)
	{
		send_to_char("AEdit:  Upper vnum already assigned.\n\r", ch);
		return false;
	}

	pArea->max_vnum = iupper;
	send_to_char("Upper vnum set.\n\r", ch);

	return true;
}

int aclimate_lookup(const char *name)
{
	int sn;

	for (sn = 0; sn < Climate::MaxClimate; sn++)
	{
		if (climate_table[sn].name == nullptr)
			break;

		if (LOWER(name[0]) == LOWER(climate_table[sn].name[0]) && !str_prefix(name, climate_table[sn].name))
			return sn;
	}

	return -1;
}

bool aedit_climate(CHAR_DATA *ch, char *argument)
{
	AREA_DATA *pArea;
	std::string buffer;
	char wcli[MSL];
	char hold[MSL];
	int icli = 0, climate;

	EDIT_AREA(ch, pArea);

	one_argument(argument, wcli);

	icli = aclimate_lookup(wcli);

	if (icli < 0)
	{
		buffer = fmt::format("{} is not a valid climate.\n\r", wcli);
		send_to_char(buffer.c_str(), ch);
		send_to_char("The following climates are available:\n\r", ch);

		for (climate = 0; climate < Climate::MaxClimate; climate++)
		{
			if (climate_table[climate].name == nullptr)
				break;

			sprintf(hold, "%s ", climate_table[climate].name);

			if (climate % 3 == 0)
				buffer = fmt::format("{}\n\r", hold);

			send_to_char(buffer.c_str(), ch);
		}

		if (climate % 3 != 0)
			send_to_char("\n\r", ch);

		return false;
	}
	else
	{
		pArea->climate = icli;
		send_to_char("Climate set.\n\r", ch);
		return true;
	}
}

/*
 * Room Editor Functions.
 */

bool redit_cabal(CHAR_DATA *ch, char *argument)
{
	ROOM_INDEX_DATA *pRoom;
	int cabal;

	EDIT_ROOM(ch, pRoom);

	if (argument[0] == '\0')
	{
		send_to_char("Syntax:  cabal (cabal name)\n\r", ch);
		return false;
	}

	cabal = cabal_lookup(argument);
	if (cabal == 0)
	{
		send_to_char("That is not a valid cabal.\n\r", ch);
		return false;
	}

	pRoom->cabal = cabal;
	send_to_char("Cabal set.\n\r", ch);
	return true;
}

bool redit_owner(CHAR_DATA *ch, char *argument)
{
	ROOM_INDEX_DATA *pRoom;

	EDIT_ROOM(ch, pRoom);

	if (argument[0] == '\0')
	{
		send_to_char("Syntax:  owner (name)\n\r", ch);
		return false;
	}

	if (get_security(ch) < 5)
	{
		send_to_char("You are not high enough level for that.\n\r", ch);
		return false;
	}

	pRoom->owner = palloc_string(argument);
	send_to_char("Owner set.\n\r", ch);
	return true;
}

bool redit_show(CHAR_DATA *ch, char *argument)
{
	ROOM_INDEX_DATA *pRoom;
	char buf[MAX_STRING_LENGTH];
	char buf1[2 * MAX_STRING_LENGTH];
	OBJ_DATA *obj;
	CHAR_DATA *rch;
	int door;
	bool fcnt;

	EDIT_ROOM(ch, pRoom);

	buf1[0] = '\0';

	sprintf(buf, "Description:\n\r%s", pRoom->description);
	strcat(buf1, buf);

	if (pRoom->alt_description)
	{
		sprintf(buf, "Alt Desc:\n\r%sAlternate Room Title is %s.\n\rDescription is %s.\n\r",
			pRoom->alt_description,
			pRoom->alt_name,
			altdesc_condtable[pRoom->alt_description_cond].display);
		strcat(buf1, buf);
	}
	else
		strcat(buf1, "Alt Desc:   [None]\n\r");

	sprintf(buf, "Name:       [%s]\n\rArea:       [%5d] %s\n\r",
		pRoom->name,
		pRoom->area->vnum,
		pRoom->area->name);
	strcat(buf1, buf);

	sprintf(buf, "Vnum:       [%5d]\n\rSector:     [%s]\n\r",
		pRoom->vnum,
		flag_string_old(sector_flags, pRoom->sector_type));
	strcat(buf1, buf);

	sprintf(buf, "Cabal:      [%s]\n\r", cabal_table[pRoom->cabal].name);
	strcat(buf1, buf);

	sprintf(buf, "Owner:      [%s]\n\r", (pRoom->owner) ? pRoom->owner : "None");
	strcat(buf1, buf);

	sprintf(buf, "Room flags: [%s]\n\r", flag_string(room_flags, pRoom->room_flags));
	strcat(buf1, buf);

	sprintf(buf, "Health recovery:[%d]\n\rMana recovery  :[%d]\n\r", pRoom->heal_rate, pRoom->mana_rate);
	strcat(buf1, buf);

	strcat(buf1, "Trap:");

	if (pRoom->trap)
	{
		auto buffer = 
			fmt::sprintf("\n\r Type:          [%s]\n\r Quality:       [%2d]\n\r Complexity:    [%3d]\n\r Timer:         [%d seconds]\n\r Trigger Echo:  [%s]\n\r Execute Echo:  [%s]\n\r",
				flag_name_lookup(pRoom->trap->type, trap_table),
				pRoom->trap->quality,
				pRoom->trap->complexity,
				pRoom->trap->timer,
				(pRoom->trap->trig_echo != nullptr) ? pRoom->trap->trig_echo : "None",
				(pRoom->trap->exec_echo != nullptr) ? pRoom->trap->exec_echo : "None"); //TODO: change the rest of the sprintf calls to format

		strcat(buf1, buffer.data());
	}
	else
	{
		strcat(buf1, "       [None]\n\r");
	}

	send_to_char(buf1, ch);
	buf1[0] = '\0';

	if (!IS_ZERO_VECTOR(pRoom->progtypes))
	{
		send_to_char("Room Programs:\n\r", ch);
		if (IS_SET(pRoom->progtypes, RPROG_PULSE))
		{
			sprintf(buf, " [pulse_prog] %s\n\r", pRoom->rprogs->pulse_name);
			send_to_char(buf, ch);
		}

		if (IS_SET(pRoom->progtypes, RPROG_ENTRY))
		{
			sprintf(buf, " [entry_prog] %s\n\r", pRoom->rprogs->entry_name);
			send_to_char(buf, ch);
		}

		if (IS_SET(pRoom->progtypes, RPROG_MOVE))
		{
			sprintf(buf, " [move_prog] %s\n\r", pRoom->rprogs->move_name);
			send_to_char(buf, ch);
		}

		if (IS_SET(pRoom->progtypes, RPROG_DROP))
		{
			sprintf(buf, " [drop_prog] %s\n\r", pRoom->rprogs->drop_name);
			send_to_char(buf, ch);
		}

		if (IS_SET(pRoom->progtypes, RPROG_SPEECH))
		{
			sprintf(buf, " [speech_prog] %s\n\r", pRoom->rprogs->speech_name);
			send_to_char(buf, ch);
		}

		if (IS_SET(pRoom->progtypes, RPROG_OPEN))
		{
			sprintf(buf, " [open_prog] %s\n\r", pRoom->rprogs->open_name);
			send_to_char(buf, ch);
		}
	}

	strcat(buf1, "Characters: [");
	fcnt = false;

	for (rch = pRoom->people; rch; rch = rch->next_in_room)
	{
		one_argument(rch->name, buf);
		strcat(buf1, buf);
		strcat(buf1, " ");
		fcnt = true;
	}

	if (fcnt)
	{
		int end = strlen(buf1) - 1;
		buf1[end] = ']';
		strcat(buf1, "\n\r");
	}
	else
	{
		strcat(buf1, "none]\n\r");
	}

	strcat(buf1, "Objects:    [");
	fcnt = false;

	for (obj = pRoom->contents; obj; obj = obj->next_content)
	{
		one_argument(obj->name, buf);
		strcat(buf1, buf);
		strcat(buf1, " ");
		fcnt = true;
	}

	if (fcnt)
	{
		int end = strlen(buf1) - 1;
		buf1[end] = ']';
		strcat(buf1, "\n\r");
	}
	else
	{
		strcat(buf1, "none]\n\r");
	}

	if (pRoom->extra_descr)
	{
		for (EXTRA_DESCR_DATA *ed = pRoom->extra_descr; ed; ed = ed->next)
		{
			strcat(buf1, "Extra Desc Keyword: ");
			strcat(buf1, ed->keyword);
			strcat(buf1, "\n\r");
		}
	}

	for (door = 0; door < MAX_DIR; door++)
	{
		EXIT_DATA *pexit = pRoom->exit[door];

		if (pexit != nullptr)
		{
			char word[MAX_INPUT_LENGTH];
			char reset_state[MAX_STRING_LENGTH];
			char *state;
			int i, length;

			sprintf(buf, "-%-5s to [%5d] Key: [%5d] ",
				capitalize(dir_name[door]),
				pexit->u1.to_room ? pexit->u1.to_room->vnum : 0, /* ROM OLC */
				pexit->key);
			strcat(buf1, buf);

			/*
			 * Format up the exit info.
			 * Capitalize all flags that are not part of the reset info.
			 */
			state = flag_string(exit_flags, pexit->exit_info);
			strcat(buf1, " Exit flags: [");

			for (;;)
			{
				state = one_argument(state, word);

				if (word[0] == '\0')
				{
					int end = strlen(buf1) - 1;
					buf1[end] = ']';
					strcat(buf1, "\n\r");
					break;
				}

				if (str_infix(word, reset_state))
				{
					length = strlen(word);

					for (i = 0; i < length; i++)
					{
						word[i] = UPPER(word[i]);
					}
				}

				strcat(buf1, word);
				strcat(buf1, " ");
			}

			if (pexit->keyword && pexit->keyword[0] != '\0')
			{
				sprintf(buf, "Kwds: [%s]\n\r", pexit->keyword);
				strcat(buf1, buf);
			}

			if (pexit->description && pexit->description[0] != '\0')
			{
				sprintf(buf, "%s", pexit->description);
				strcat(buf1, buf);
			}
		}
	}

	send_to_char(buf1, ch);
	send_to_char("Syntax:  resets\n\r---- RESETS ----\n\r", ch);
	do_resets(ch, "");
	return false;
}

/* Local function. */
bool change_exit(CHAR_DATA *ch, char *argument, int door)
{
	ROOM_INDEX_DATA *pRoom, *move_to;
	char command[MAX_INPUT_LENGTH];
	char arg[MAX_INPUT_LENGTH];
	int value, count = 0;
	bool dFound = false, dMove = false;

	EDIT_ROOM(ch, pRoom);

	argument = one_argument(argument, command);
	argument = one_argument(argument, arg);

	/*
	 * Set the exit flags, needs full argument.
	 * ----------------------------------------
	 */
	value = flag_value(exit_flags, command);

	if (value != NO_FLAG)
	{
		ROOM_INDEX_DATA *pToRoom;
		short rev; /* ROM OLC */

		if (!pRoom->exit[door])
		{
			send_to_char("Exit flag doesn't exist.\n\r", ch);
			return false;
		}
		/*   pRoom->exit[door] = new_exit(); */

		/*
		 * This room.
		 */
		TOGGLE_BIT(pRoom->exit[door]->exit_info, value);

		send_to_char("Current room exit flag toggled.\n\r", ch);

		/*
		 * Connected room.
		 */
		if (str_prefix(arg, "single"))
		{
			pToRoom = pRoom->exit[door]->u1.to_room; /* ROM OLC */
			rev = rev_dir[door];

			if (pToRoom->exit[rev] != nullptr)
				TOGGLE_BIT(pToRoom->exit[rev]->exit_info, value);

			send_to_char("Linking room exit flag toggled.\n\r", ch);
		}

		return true;
	}

	if (command[0] == '\0' && argument[0] == '\0') /* Move command. */
	{
		move_char(ch, door, true, true); /* ROM OLC */
		return false;
	}

	if (command[0] == '?')
	{
		do_help(ch, "EXIT");
		return false;
	}

	if (!str_cmp(command, "delete"))
	{
		ROOM_INDEX_DATA *pToRoom;
		short rev; /* ROM OLC */

		if (!pRoom->exit[door])
		{
			send_to_char("REdit:  Cannot delete a null exit.\n\r", ch);
			return false;
		}

		/*
		 * Remove ToRoom Exit.
		 */
		rev = rev_dir[door];
		pToRoom = pRoom->exit[door]->u1.to_room; /* ROM OLC */

		if (pToRoom->exit[rev])
		{
			free_exit(pToRoom->exit[rev]);
			pToRoom->exit[rev] = nullptr;
		}

		/*
		 * Remove this exit.
		 */
		free_exit(pRoom->exit[door]);
		pRoom->exit[door] = nullptr;

		send_to_char("Exit unlinked.\n\r", ch);
		return true;
	}

	if (!str_cmp(command, "link"))
	{
		EXIT_DATA *pExit;

		if (arg[0] == '\0' || !is_number(arg))
		{
			send_to_char("Syntax:  [direction] link [vnum]\n\r", ch);
			return false;
		}

		value = atoi(arg);

		if (!get_room_index(value))
		{
			send_to_char("REdit:  Cannot link to non-existant room.\n\r", ch);
			return false;
		}

		if (!IS_BUILDER(ch, get_room_index(value)->area))
		{
			send_to_char("REdit:  Cannot link to that area.\n\r", ch);
			return false;
		}

		if (get_room_index(value)->exit[rev_dir[door]])
		{
			send_to_char("REdit:  Remote side's exit already exists.\n\r", ch);
			return false;
		}

		if (!pRoom->exit[door])
		{
			pRoom->exit[door] = new_exit();
		}

		pRoom->exit[door]->u1.to_room = get_room_index(value); /* ROM OLC */
		pRoom->exit[door]->orig_door = door;

		/*	pRoom->exit[door]->vnum = value;                Can't set vnum in ROM */

		pRoom = get_room_index(value);
		door = rev_dir[door];
		pExit = new_exit();
		pExit->u1.to_room = ch->in_room;
		/*	pExit->vnum             = ch->in_room->vnum;    Can't set vnum in ROM */
		pExit->orig_door = door;
		pRoom->exit[door] = pExit;

		send_to_char("Two-way link established.\n\r", ch);
		return true;
	}

	if (!str_cmp(command, "dig"))
	{
		if (arg[0] == '\0' || !is_number(arg))
		{
			for (count = ch->in_room->area->min_vnum; count <= ch->in_room->area->max_vnum; count++)
			{
				if ((get_room_index(count)) == nullptr)
				{
					dFound = true;
					dMove = true;
					break;
				}
			}

			if (!dFound)
			{
				send_to_char("Syntax: [direction] dig        -- Assigns next available vnum\n\r"\
							 "        [direction] dig <vnum> -- Assigns that specific vnum\n\r", ch);
				return false;
			}
			else
			{
				sprintf(arg, "%d", count);
			}
		}

		redit_create(ch, arg);
		auto buffer = fmt::format("link {}", arg);
		change_exit(ch, buffer.data(), door);

		move_to = get_room_index(atoi(arg));

		if (move_to != nullptr && dMove)
		{
			char_from_room(ch);
			char_to_room(ch, move_to);
		}

		return true;
	}

	if (!str_cmp(command, "room"))
	{
		if (arg[0] == '\0' || !is_number(arg))
		{
			send_to_char("Syntax:  [direction] room [vnum]\n\r", ch);
			return false;
		}

		if (!pRoom->exit[door])
			pRoom->exit[door] = new_exit();

		value = atoi(arg);

		if (!get_room_index(value))
		{
			send_to_char("REdit:  Cannot link to non-existant room.\n\r", ch);
			return false;
		}

		pRoom->exit[door]->u1.to_room = get_room_index(value); /* ROM OLC */
		pRoom->exit[door]->orig_door = door;
		/*	pRoom->exit[door]->vnum = value;                 Can't set vnum in ROM */

		send_to_char("One-way link established.\n\r", ch);
		return true;
	}

	if (!str_cmp(command, "key"))
	{
		if (arg[0] == '\0' || !is_number(arg))
		{
			send_to_char("Syntax:  [direction] key [vnum]\n\r", ch);
			return false;
		}

		if (!pRoom->exit[door])
			pRoom->exit[door] = new_exit();

		value = atoi(arg);

		if (!get_obj_index(value))
		{
			send_to_char("REdit:  Key doesn't exist.\n\r", ch);
			return false;
		}

		if (get_obj_index(value)->item_type != ITEM_KEY)
		{
			send_to_char("REdit:  Key doesn't exist.\n\r", ch);
			return false;
		}

		pRoom->exit[door]->key = value;

		send_to_char("Exit key set.\n\r", ch);
		return true;
	}

	if (!str_cmp(command, "copydesc"))
	{

		if (!pRoom->exit[door])
		{
			send_to_char("REdit:  Exit doesn't exist.\n\r", ch);
			return false;
		}

		if (!pRoom->exit[door]->u1.to_room)
		{
			send_to_char("REdit:  Room doesn't exist.\n\r", ch);
			return false;
		}

		free_pstring(pRoom->exit[door]->u1.to_room->description);
		pRoom->exit[door]->u1.to_room->description = palloc_string(pRoom->description);

		send_to_char("Room description copied.\n\r", ch);
		return true;
	}

	if (!str_cmp(command, "name"))
	{
		if (arg[0] == '\0')
		{
			send_to_char("Syntax:  [direction] name [string]\n\r", ch);
			return false;
		}

		if (!pRoom->exit[door])
		{
			pRoom->exit[door] = new_exit();
		}

		free_pstring(pRoom->exit[door]->keyword);
		pRoom->exit[door]->keyword = palloc_string(arg);

		send_to_char("Exit name set.\n\r", ch);
		return true;
	}

	if (!str_prefix(command, "description"))
	{
		if (arg[0] == '\0')
		{
			if (!pRoom->exit[door])
				pRoom->exit[door] = new_exit();

			string_append(ch, &pRoom->exit[door]->description);
			return true;
		}

		send_to_char("Syntax:  [direction] desc\n\r", ch);
		return false;
	}

	return false;
}

bool redit_north(CHAR_DATA *ch, char *argument)
{
	if (change_exit(ch, argument, Directions::North))
		return true;

	return false;
}

bool redit_south(CHAR_DATA *ch, char *argument)
{
	if (change_exit(ch, argument, Directions::South))
		return true;

	return false;
}

bool redit_east(CHAR_DATA *ch, char *argument)
{
	if (change_exit(ch, argument, Directions::East))
		return true;

	return false;
}

bool redit_west(CHAR_DATA *ch, char *argument)
{
	if (change_exit(ch, argument, Directions::West))
		return true;

	return false;
}

bool redit_up(CHAR_DATA *ch, char *argument)
{
	if (change_exit(ch, argument, Directions::Up))
		return true;

	return false;
}

bool redit_down(CHAR_DATA *ch, char *argument)
{
	if (change_exit(ch, argument, Directions::Down))
		return true;

	return false;
}

bool redit_ed(CHAR_DATA *ch, char *argument)
{
	ROOM_INDEX_DATA *pRoom;
	EXTRA_DESCR_DATA *ed;
	char command[MAX_INPUT_LENGTH];

	EDIT_ROOM(ch, pRoom);

	argument = one_argument(argument, command);

	if (command[0] == '\0' || argument[0] == '\0')
	{
		send_to_char("Syntax:  ed add [keywords]\n\r", ch);
		send_to_char("         ed edit [one of keywords]\n\r", ch);
		send_to_char("         ed delete [one of keywords]\n\r", ch);
		send_to_char("         ed format [one of keywords]\n\r", ch);
		return false;
	}

	if (!str_cmp(command, "add"))
	{
		if (argument[0] == '\0')
		{
			send_to_char("Syntax:  ed add [keyword]\n\r", ch);
			return false;
		}

		ed = new_extra_descr();
		ed->keyword = palloc_string(argument);
		ed->description = palloc_string("");
		ed->next = pRoom->extra_descr;
		pRoom->extra_descr = ed;

		string_append(ch, &ed->description);

		return true;
	}

	if (!str_cmp(command, "edit"))
	{
		if (argument[0] == '\0')
		{
			send_to_char("Syntax:  ed edit [keyword]\n\r", ch);
			return false;
		}

		for (ed = pRoom->extra_descr; ed; ed = ed->next)
		{
			if (is_name(argument, ed->keyword))
				break;
		}

		if (!ed)
		{
			send_to_char("REdit:  Extra description keyword not found.\n\r", ch);
			return false;
		}

		string_append(ch, &ed->description);

		return true;
	}

	if (!str_cmp(command, "delete"))
	{
		EXTRA_DESCR_DATA *ped = nullptr;

		if (argument[0] == '\0')
		{
			send_to_char("Syntax:  ed delete [keyword]\n\r", ch);
			return false;
		}

		for (ed = pRoom->extra_descr; ed; ed = ed->next)
		{
			if (is_name(argument, ed->keyword))
				break;

			ped = ed;
		}

		if (!ed)
		{
			send_to_char("REdit:  Extra description keyword not found.\n\r", ch);
			return false;
		}

		if (!ped)
			pRoom->extra_descr = ed->next;
		else
			ped->next = ed->next;

		free_extra_descr(ed);

		send_to_char("Extra description deleted.\n\r", ch);
		return true;
	}

	if (!str_cmp(command, "format"))
	{
		if (argument[0] == '\0')
		{
			send_to_char("Syntax:  ed format [keyword]\n\r", ch);
			return false;
		}

		for (ed = pRoom->extra_descr; ed; ed = ed->next)
		{
			if (is_name(argument, ed->keyword))
				break;
		}

		if (!ed)
		{
			send_to_char("REdit:  Extra description keyword not found.\n\r", ch);
			return false;
		}

		ed->description = format_string(ed->description);

		send_to_char("Extra description formatted.\n\r", ch);
		return true;
	}

	redit_ed(ch, "");
	return false;
}

bool redit_create(CHAR_DATA *ch, char *argument)
{
	AREA_DATA *pArea;
	ROOM_INDEX_DATA *pRoom;
	int value;
	int iHash;
	char buf[MSL];

	EDIT_ROOM(ch, pRoom);

	value = atoi(argument);

	if (argument[0] == '\0' || value <= 0)
	{
		send_to_char("Syntax:  create [vnum > 0]\n\r", ch);
		return false;
	}

	pArea = get_vnum_area(value);

	if (!pArea)
	{
		send_to_char("REdit:  That vnum is not assigned an area.\n\r", ch);
		return false;
	}

	if (!IS_BUILDER(ch, pArea))
	{
		send_to_char("REdit:  Vnum in an area you cannot build in.\n\r", ch);
		return false;
	}

	if (get_room_index(value))
	{
		send_to_char("REdit:  Room vnum already exists.\n\r", ch);
		return false;
	}

	pRoom = new_room_index();
	pRoom->area = pArea;
	pRoom->vnum = value;

	if (value > top_vnum_room)
		top_vnum_room = value;

	iHash = value % MAX_KEY_HASH;
	pRoom->next = room_index_hash[iHash];
	room_index_hash[iHash] = pRoom;
	ch->desc->pEdit = (void *)pRoom;

	sprintf(buf, "Room #%d created.\n\r", value);
	send_to_char(buf, ch);
	return true;
}

bool redit_name(CHAR_DATA *ch, char *argument)
{
	ROOM_INDEX_DATA *pRoom;

	EDIT_ROOM(ch, pRoom);

	if (argument[0] == '\0')
	{
		send_to_char("Syntax:  name [name]\n\r", ch);
		return false;
	}

	free_pstring(pRoom->name);
	pRoom->name = palloc_string(argument);

	send_to_char("Name set.\n\r", ch);
	return true;
}

void redit_end_fun(CHAR_DATA *ch, char *argument)
{
	free_pstring(ch->in_room->description);
	ch->in_room->description = palloc_string(ch->pcdata->entered_text);
	ch->pcdata->entered_text[0] = '\0';
}

bool redit_desc(CHAR_DATA *ch, char *argument)
{
	ROOM_INDEX_DATA *pRoom;

	EDIT_ROOM(ch, pRoom);

	if (argument[0] == '\0')
	{
		string_append(ch, &pRoom->description);
		return true;
	}

	send_to_char("Syntax:  desc\n\r", ch);
	return false;
}

bool redit_heal(CHAR_DATA *ch, char *argument)
{
	ROOM_INDEX_DATA *pRoom;

	EDIT_ROOM(ch, pRoom);

	if (is_number(argument))
	{
		pRoom->heal_rate = atoi(argument);
		send_to_char("Heal rate set.\n\r", ch);
		return true;
	}

	send_to_char("Syntax : heal <#xnumber>\n\r", ch);
	return false;
}

bool redit_mana(CHAR_DATA *ch, char *argument)
{
	ROOM_INDEX_DATA *pRoom;

	EDIT_ROOM(ch, pRoom);

	if (is_number(argument))
	{
		pRoom->mana_rate = atoi(argument);
		send_to_char("Mana rate set.\n\r", ch);
		return true;
	}

	send_to_char("Syntax : mana <#xnumber>\n\r", ch);
	return false;
}

bool redit_clan(CHAR_DATA *ch, char *argument)
{
	ROOM_INDEX_DATA *pRoom;

	EDIT_ROOM(ch, pRoom);

	send_to_char("Clan set.\n\r", ch);
	return true;
}

bool redit_format(CHAR_DATA *ch, char *argument)
{
	ROOM_INDEX_DATA *pRoom;

	EDIT_ROOM(ch, pRoom);

	pRoom->description = format_string(pRoom->description);

	send_to_char("String formatted.\n\r", ch);
	return true;
}

bool redit_mreset(CHAR_DATA *ch, char *argument)
{
	ROOM_INDEX_DATA *pRoom;
	MOB_INDEX_DATA *pMobIndex;
	CHAR_DATA *newmob;
	char arg[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];

	RESET_DATA *pReset;
	char output[MAX_STRING_LENGTH];

	EDIT_ROOM(ch, pRoom);

	argument = one_argument(argument, arg);
	argument = one_argument(argument, arg2);

	if (arg[0] == '\0' || !is_number(arg))
	{
		send_to_char("Syntax:  mreset <vnum> <max #x> <mix #x>\n\r", ch);
		return false;
	}

	pMobIndex = get_mob_index(atoi(arg));

	if (!pMobIndex)
	{
		send_to_char("REdit: No mobile has that vnum.\n\r", ch);
		return false;
	}

	if (pMobIndex->area != pRoom->area)
	{
		send_to_char("REdit: No such mobile in this area.\n\r", ch);
		return false;
	}

	/*
	 * Create the mobile reset.
	 */
	pReset = new_reset_data();
	pReset->command = 'M';
	pReset->arg1 = pMobIndex->vnum;
	pReset->arg2 = is_number(arg2) ? atoi(arg2) : MAX_MOB;
	pReset->arg3 = pRoom->vnum;
	pReset->arg4 = is_number(argument) ? atoi(argument) : 1;
	add_reset(pRoom, pReset, 0 /* Last slot*/);

	/*
	 * Create the mobile.
	 */
	newmob = create_mobile(pMobIndex);
	char_to_room(newmob, pRoom);

	sprintf(output,
			"%s (%d) has been loaded and added to resets.\n\r"\
			"There will be a maximum of %d loaded to this room.\n\r",
			capitalize(pMobIndex->short_descr), pMobIndex->vnum, pReset->arg2);
	send_to_char(output, ch);

	act("$n has created $N!", ch, nullptr, newmob, TO_ROOM);
	return true;
}

int wear_loc(int bits, int count)
{
	for (int flag = 0; wear_table[flag].wear_bit != NO_FLAG; flag++)
	{
		if (IS_SET_OLD(bits, wear_table[flag].wear_bit) && --count < 1)
			return wear_table[flag].wear_loc;
	}

	return NO_FLAG;
}

int wear_bit(int loc)
{
	for (int flag = 0; wear_table[flag].wear_loc != NO_FLAG; flag++)
	{
		if (loc == wear_table[flag].wear_loc)
			return wear_table[flag].wear_bit;
	}

	return 0;
}

bool redit_oreset(CHAR_DATA *ch, char *argument)
{
	ROOM_INDEX_DATA *pRoom;
	OBJ_INDEX_DATA *pObjIndex;
	OBJ_DATA *newobj;
	OBJ_DATA *to_obj;
	CHAR_DATA *to_mob;
	char arg1[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	int olevel = 0;

	RESET_DATA *pReset;
	RESET_DATA *pReset1, *pReset2;
	char output[MAX_STRING_LENGTH];

	EDIT_ROOM(ch, pRoom);

	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);

	if (arg1[0] == '\0' || !is_number(arg1))
	{
		send_to_char("Syntax:  oreset <vnum> <args>\n\r", ch);
		send_to_char("        -no_args               = into room\n\r", ch);
		send_to_char("        -<obj_name>            = into obj\n\r", ch);
		send_to_char("        -<mob_name> <wear_loc> = into mob\n\r", ch);
		return false;
	}

	pObjIndex = get_obj_index(atoi(arg1));

	if (!(pObjIndex))
	{
		send_to_char("REdit: No object has that vnum.\n\r", ch);
		return false;
	}

	if (pObjIndex->area != pRoom->area)
	{
		send_to_char("REdit: No such object in this area.\n\r", ch);
		return false;
	}

	/*
	 * Load into room.
	 */
	if (arg2[0] == '\0')
	{
		pReset = new_reset_data();
		pReset->command = 'O';
		pReset->arg1 = pObjIndex->vnum;
		pReset->arg2 = 0;
		pReset->arg3 = pRoom->vnum;
		pReset->arg4 = 0;
		add_reset(pRoom, pReset, 0 /* Last slot*/);

		newobj = create_object(pObjIndex, number_fuzzy(olevel));
		obj_to_room(newobj, pRoom);

		sprintf(output, "%s (%d) has been loaded and added to resets.\n\r",
			capitalize(pObjIndex->short_descr),
			pObjIndex->vnum);
		send_to_char(output, ch);
	}
	/*
	 * Load into object's inventory.
	 */
	else if (argument[0] == '\0' && ((to_obj = get_obj_list(ch, arg2, pRoom->contents)) != nullptr))
	{
		pReset = new_reset_data();
		pReset->command = 'P';
		pReset->arg1 = pObjIndex->vnum;
		pReset->arg2 = 0;
		pReset->arg3 = to_obj->pIndexData->vnum;
		pReset->arg4 = 1;
		//		add_reset( pRoom, pReset, 0/* Last slot*/ );

		for (pReset1 = pRoom->reset_first; pReset1 != nullptr; pReset1 = pReset1->next)
		{
			pReset2 = pReset1;

			if (pReset1->command == 'O')
			{
				if (pReset1->arg1 == pReset->arg3)
				{
					pReset->next = pReset1;
					pReset2->next = pReset;
					break;
				}
			}
		}

		newobj = create_object(pObjIndex, number_fuzzy(olevel));
		newobj->cost = 0;
		newobj->owner = palloc_string("none");
		obj_to_obj(newobj, to_obj);

		sprintf(output,
				"%s (%d) has been loaded into "\
				"%s (%d) and added to resets.\n\r",
				capitalize(newobj->short_descr), newobj->pIndexData->vnum, to_obj->short_descr,
				to_obj->pIndexData->vnum);
		send_to_char(output, ch);
	}
	/*
	 * Load into mobile's inventory.
	 */
	else
	{
		to_mob = get_char_room(ch, arg2);
	}

	if (to_mob != nullptr)
	{
		int wear_loc;

		/*
		 * Make sure the location on mobile is valid.
		 */
		wear_loc = flag_value(wear_loc_flags, argument);

		if (wear_loc == NO_FLAG)
		{
			send_to_char("REdit: Invalid wear_loc.  '? wear-loc'\n\r", ch);
			return false;
		}

		/*
		 * Disallow loading a sword(WEAR_WIELD) into WEAR_HEAD.
		 */
		if (!IS_SET(pObjIndex->wear_flags, wear_bit(wear_loc)))
		{
			sprintf(output, "%s (%d) has wear flags: [%s]\n\r",
				capitalize(pObjIndex->short_descr),
				pObjIndex->vnum,
				flag_string(wear_flags, pObjIndex->wear_flags));
			send_to_char(output, ch);
			return false;
		}

		/*
		 * Can't load into same position.
		 */
		if (get_eq_char(to_mob, wear_loc))
		{
			send_to_char("REdit:  Object already equipped.\n\r", ch);
			return false;
		}

		pReset = new_reset_data();
		pReset->arg1 = pObjIndex->vnum;
		pReset->arg2 = wear_loc;

		if (pReset->arg2 == WEAR_NONE)
			pReset->command = 'G';
		else
			pReset->command = 'E';

		pReset->arg3 = wear_loc;

		add_reset(pRoom, pReset, 0 /* Last slot*/);

		olevel = URANGE(0, to_mob->level - 2, LEVEL_HERO);
		newobj = create_object(pObjIndex, number_fuzzy(olevel));

		if (to_mob->pIndexData->pShop) /* Shop-keeper? */
		{
			switch (pObjIndex->item_type)
			{
				case ITEM_PILL:
					olevel = number_range(0, 10);
					break;
				case ITEM_POTION:
					olevel = number_range(0, 10);
					break;
				case ITEM_SCROLL:
					olevel = number_range(5, 15);
					break;
				case ITEM_WAND:
					olevel = number_range(10, 20);
					break;
				case ITEM_STAFF:
					olevel = number_range(15, 25);
					break;
				case ITEM_ARMOR:
					olevel = number_range(5, 15);
					break;
				case ITEM_WEAPON:
					if (pReset->command == 'G')
						olevel = number_range(5, 15);
					else
						olevel = number_fuzzy(olevel);

					break;
				default:
					olevel = 0;
					break;
			}

			newobj = create_object(pObjIndex, olevel);

			if (pReset->arg2 == WEAR_NONE)
				SET_BIT(newobj->extra_flags, ITEM_INVENTORY);
		}
		else
		{
			newobj = create_object(pObjIndex, number_fuzzy(olevel));
		}

		obj_to_char(newobj, to_mob);

		if (pReset->command == 'E')
			equip_char(to_mob, newobj, pReset->arg3, false);

		sprintf(output,
				"%s (%d) has been loaded "\
				"%s of %s (%d) and added to resets.\n\r",
				capitalize(pObjIndex->short_descr),
				pObjIndex->vnum,
				flag_string_old(wear_loc_strings, pReset->arg3),
				to_mob->short_descr,
				to_mob->pIndexData->vnum);
		send_to_char(output, ch);
	}
	else /* Display Syntax */
	{
		send_to_char("REdit:  That mobile isn't here.\n\r", ch);
		return false;
	}

	act("$n has created $p!", ch, newobj, nullptr, TO_ROOM);
	return true;
}

/*
 * Object Editor Functions.
 */
void show_obj_values(CHAR_DATA *ch, OBJ_INDEX_DATA *obj)
{
	char buf[MAX_STRING_LENGTH];

	switch (obj->item_type)
	{
		case ITEM_LIGHT:
			if (obj->value[2] == -1 || obj->value[2] == 999) /* ROM OLC */
				sprintf(buf, "[v2] Light:  Infinite[-1]\n\r");
			else
				sprintf(buf, "[v2] Light:  [%d]\n\r", obj->value[2]);
			send_to_char(buf, ch);

			break;
		case ITEM_WAND:
		case ITEM_STAFF:
			sprintf(buf,
					"[v0] Level:          [%d]\n\r"\
					"[v1] Charges Total:  [%d]\n\r"\
					"[v2] Charges Left:   [%d]\n\r"\
					"[v3] Spell:          %s\n\r",
					obj->value[0], obj->value[1], obj->value[2],
					obj->value[3] != -1 ? skill_table[obj->value[3]].name : "none");
			send_to_char(buf, ch);

			break;
		case ITEM_PORTAL:
			sprintf(buf,
					"[v0] Charges:        [%d]\n\r"\
					"[v1] Exit Flags:     %s\n\r"\
					"[v2] Portal Flags:   %s\n\r"\
					"[v3] Goes to (vnum): [%d]\n\r",
					obj->value[0], flag_string_old(exit_flags, obj->value[1]), flag_string_old(portal_flags, obj->value[2]),
					obj->value[3]);
			send_to_char(buf, ch);

			break;
		case ITEM_FURNITURE:
			sprintf(buf,
					"[v0] Max people:      [%d]\n\r"\
					"[v1] Max weight:      [%d]\n\r"\
					"[v2] Furniture Flags: %s\n\r"\
					"[v3] Heal bonus:      [%d]\n\r"\
					"[v4] Mana bonus:      [%d]\n\r",
					obj->value[0], obj->value[1], flag_string_old(furniture_flags, obj->value[2]), obj->value[3],
					obj->value[4]);
			send_to_char(buf, ch);

			break;
		case ITEM_SCROLL:
		case ITEM_POTION:
		case ITEM_PILL:
			sprintf(buf,
					"[v0] Level:  [%d]\n\r"\
					"[v1] Spell:  %s\n\r"\
					"[v2] Spell:  %s\n\r"\
					"[v3] Spell:  %s\n\r"\
					"[v4] Spell:  %s\n\r",
					obj->value[0], obj->value[1] != -1 ? skill_table[obj->value[1]].name : "none",
					obj->value[2] != -1 ? skill_table[obj->value[2]].name : "none",
					obj->value[3] != -1 ? skill_table[obj->value[3]].name : "none",
					obj->value[4] != -1 ? skill_table[obj->value[4]].name : "none");
			send_to_char(buf, ch);

			break;
		case ITEM_ARMOR:
			/* ARMOR for ROM */

			sprintf(buf,
					"[v0] Ac pierce       [%d]\n\r"\
					"[v1] Ac bash         [%d]\n\r"\
					"[v2] Ac slash        [%d]\n\r"\
					"[v3] Ac exotic       [%d]\n\r",
					obj->value[0], obj->value[1], obj->value[2], obj->value[3]);
			send_to_char(buf, ch);

			break;
		case ITEM_WEAPON:
			/* WEAPON changed in ROM: */
			/* I had to split the output here, I have no idea why, but it helped -- Hugin */
			/* It somehow fixed a bug in showing scroll/pill/potions too ?! */

			sprintf(buf, "[v0] Weapon class:   %s\n\r", weapon_name_lookup(obj->value[0]));
			send_to_char(buf, ch);

			sprintf(buf, "[v1] Number of dice: [%d]\n\r", obj->value[1]);
			send_to_char(buf, ch);

			sprintf(buf, "[v2] Type of dice:   [%d]\n\r", obj->value[2]);
			send_to_char(buf, ch);

			sprintf(buf, "[v3] Type:           %s\n\r", attack_table[std::max(0, obj->value[3])].name);
			send_to_char(buf, ch);

			sprintf(buf, "[v4] Special type:   %s\n\r", flag_string_old(weapon_type2, obj->value[4]));
			send_to_char(buf, ch);

			break;
		case ITEM_CONTAINER:
			sprintf(buf,
					"[v0] Weight:     [%d kg]\n\r"\
					"[v1] Flags:      [%s]\n\r"\
					"[v2] Key:     %s [%d]\n\r"\
					"[v3] Capacity    [%d]\n\r"\
					"[v4] Weight Mult [%d]\n\r",
					obj->value[0], cont_bit_name(obj->value[1]),
					get_obj_index(obj->value[2]) ? get_obj_index(obj->value[2])->short_descr : "none", obj->value[2],
					obj->value[3], obj->value[4]);
			send_to_char(buf, ch);

			break;
		case ITEM_DRINK_CON:
			sprintf(buf,
					"[v0] Liquid Total: [%d]\n\r"\
					"[v1] Liquid Left:  [%d]\n\r"\
					"[v2] Liquid:       %s\n\r"\
					"[v3] Poisoned:     %s\n\r",
					obj->value[0], obj->value[1], liq_table[obj->value[2]].liq_name, obj->value[3] != 0 ? "Yes" : "No");
			send_to_char(buf, ch);

			break;
		case ITEM_FOUNTAIN:
			sprintf(buf,
					"[v0] Liquid Total: [%d]\n\r"\
					"[v1] Liquid Left:  [%d]\n\r"\
					"[v2] Liquid:	    %s\n\r",
					obj->value[0], obj->value[1], liq_table[obj->value[2]].liq_name);
			send_to_char(buf, ch);

			break;
		case ITEM_FOOD:
			sprintf(buf,
					"[v0] Food hours: [%d]\n\r"\
					"[v1] Full hours: [%d]\n\r"\
					"[v3] Poisoned:   %s\n\r",
					obj->value[0], obj->value[1], obj->value[3] != 0 ? "Yes" : "No");
			send_to_char(buf, ch);

			break;
		case ITEM_MONEY:
			sprintf(buf, "[v0] Gold:   [%d]\n\r", obj->value[0]);
			send_to_char(buf, ch);

			break;
		default: /* No values. */
			break;
	}
}

bool set_obj_values(CHAR_DATA *ch, OBJ_INDEX_DATA *pObj, int value_num, char *argument)
{
	int value;
	switch (pObj->item_type)
	{
		case ITEM_LIGHT:
			switch (value_num)
			{
				case 2:
					send_to_char("HOURS OF LIGHT SET.\n\r\n\r", ch);
					pObj->value[2] = atoi(argument);
					break;
				default:
					do_help(ch, "ITEM_LIGHT");
					return false;
			}

			break;
		case ITEM_WAND:
		case ITEM_STAFF:
			switch (value_num)
			{
				case 0:
					send_to_char("SPELL LEVEL SET.\n\r\n\r", ch);
					pObj->value[0] = atoi(argument);
					break;
				case 1:
					send_to_char("TOTAL NUMBER OF CHARGES SET.\n\r\n\r", ch);
					pObj->value[1] = atoi(argument);
					break;
				case 2:
					send_to_char("CURRENT NUMBER OF CHARGES SET.\n\r\n\r", ch);
					pObj->value[2] = atoi(argument);
					break;
				case 3:
					send_to_char("SPELL TYPE SET.\n\r", ch);
					pObj->value[3] = skill_lookup(argument);
					break;
				default:
					do_help(ch, "ITEM_STAFF_WAND");
					return false;
			}

			break;
		case ITEM_SCROLL:
		case ITEM_POTION:
		case ITEM_PILL:
			switch (value_num)
			{
				case 0:
					send_to_char("SPELL LEVEL SET.\n\r\n\r", ch);
					pObj->value[0] = atoi(argument);
					break;
				case 1:
					send_to_char("SPELL TYPE 1 SET.\n\r\n\r", ch);
					pObj->value[1] = skill_lookup(argument);
					break;
				case 2:
					send_to_char("SPELL TYPE 2 SET.\n\r\n\r", ch);
					pObj->value[2] = skill_lookup(argument);
					break;
				case 3:
					send_to_char("SPELL TYPE 3 SET.\n\r\n\r", ch);
					pObj->value[3] = skill_lookup(argument);
					break;
				case 4:
					send_to_char("SPELL TYPE 4 SET.\n\r\n\r", ch);
					pObj->value[4] = skill_lookup(argument);
					break;
				default:
					do_help(ch, "ITEM_SCROLL_POTION_PILL");
					return false;
			}

			break;
		case ITEM_ARMOR:
			/* ARMOR for ROM: */

			switch (value_num)
			{
				case 0:
					send_to_char("AC PIERCE SET.\n\r\n\r", ch);
					pObj->value[0] = atoi(argument);
					break;
				case 1:
					send_to_char("AC BASH SET.\n\r\n\r", ch);
					pObj->value[1] = atoi(argument);
					break;
				case 2:
					send_to_char("AC SLASH SET.\n\r\n\r", ch);
					pObj->value[2] = atoi(argument);
					break;
				case 3:
					send_to_char("AC EXOTIC SET.\n\r\n\r", ch);
					pObj->value[3] = atoi(argument);
					break;
				default:
					do_help(ch, "ITEM_ARMOR");
					return false;
			}

			break;
		case ITEM_WEAPON:
			/* WEAPONS changed in ROM */

			switch (value_num)
			{
				case 0:
					send_to_char("WEAPON CLASS SET.\n\r\n\r", ch);
					if (!str_prefix(argument, "hand"))
						pObj->value[0] = weapon_type_lookup("weapon");
					else
						pObj->value[0] = weapon_type_lookup(argument);
					break;
				case 1:
					send_to_char("NUMBER OF DICE SET.\n\r\n\r", ch);
					pObj->value[1] = atoi(argument);
					break;
				case 2:
					send_to_char("TYPE OF DICE SET.\n\r\n\r", ch);
					pObj->value[2] = atoi(argument);
					break;
				case 3:
					send_to_char("WEAPON TYPE SET.\n\r\n\r", ch);
					pObj->value[3] = attack_lookup(argument);
					break;
				case 4:
					send_to_char("SPECIAL WEAPON TYPE SET.\n\r\n\r", ch);
					value = flag_value(weapon_type2, argument);
					if (value != NO_FLAG)
						TOGGLE_BIT_OLD(pObj->value[4], value);
					break;
				default:
					do_help(ch, "ITEM_WEAPON");
					return false;
			}

			break;
		case ITEM_PORTAL:
			switch (value_num)
			{
				case 0:
					send_to_char("CHARGES SET.\n\r\n\r", ch);
					pObj->value[0] = atoi(argument);
					break;
				case 1:
					send_to_char("EXIT FLAGS SET.\n\r\n\r", ch);
					value = flag_value(exit_flags, argument);
					if (value != NO_FLAG)
						TOGGLE_BIT_OLD(pObj->value[1], value);
					break;
				case 2:
					send_to_char("PORTAL FLAGS SET.\n\r\n\r", ch);
					value = flag_value(portal_flags, argument);
					if (value != NO_FLAG)
						TOGGLE_BIT_OLD(pObj->value[2], value);
					break;
				case 3:
					send_to_char("EXIT VNUM SET.\n\r\n\r", ch);
					pObj->value[3] = atoi(argument);
					break;
				default:
					do_help(ch, "ITEM_PORTAL");
					return false;
			}

			break;
		case ITEM_FURNITURE:
			switch (value_num)
			{
				case 0:
					send_to_char("NUMBER OF PEOPLE SET.\n\r\n\r", ch);
					pObj->value[0] = atoi(argument);
					break;
				case 1:
					send_to_char("MAX WEIGHT SET.\n\r\n\r", ch);
					pObj->value[1] = atoi(argument);
					break;
				case 2:
					send_to_char("FURNITURE FLAGS SET.\n\r\n\r", ch);
					value = flag_value(furniture_flags, argument);

					if ((value) != NO_FLAG)
						TOGGLE_BIT_OLD(pObj->value[2], value);

					break;
				case 3:
					send_to_char("HEAL BONUS SET.\n\r\n\r", ch);
					pObj->value[3] = atoi(argument);
					break;
				case 4:
					send_to_char("MANA BONUS SET.\n\r\n\r", ch);
					pObj->value[4] = atoi(argument);
					break;
				default:
					do_help(ch, "ITEM_FURNITURE");
					return false;
			}

			break;
		case ITEM_CONTAINER:
			switch (value_num)
			{
				case 0:
					send_to_char("WEIGHT CAPACITY SET.\n\r\n\r", ch);
					pObj->value[0] = atoi(argument);
					break;
				case 1:
					value = flag_value(container_flags, argument);

					if (value != NO_FLAG)
					{
						TOGGLE_BIT_OLD(pObj->value[1], value);
					}
					else
					{
						do_help(ch, "ITEM_CONTAINER");
						return false;
					}

					send_to_char("CONTAINER TYPE SET.\n\r\n\r", ch);
					break;
				case 2:
					if (atoi(argument) != 0)
					{
						if (!get_obj_index(atoi(argument)))
						{
							send_to_char("THERE IS NO SUCH ITEM.\n\r\n\r", ch);
							return false;
						}

						if (get_obj_index(atoi(argument))->item_type != ITEM_KEY)
						{
							send_to_char("THAT ITEM IS NOT A KEY.\n\r\n\r", ch);
							return false;
						}
					}

					send_to_char("CONTAINER KEY SET.\n\r\n\r", ch);
					pObj->value[2] = atoi(argument);
					break;
				case 3:
					send_to_char("CONTAINER MAX WEIGHT SET.\n\r", ch);
					pObj->value[3] = atoi(argument);
					break;
				case 4:
					send_to_char("WEIGHT MULTIPLIER SET.\n\r\n\r", ch);
					pObj->value[4] = atoi(argument);
					break;
				default:
					do_help(ch, "ITEM_CONTAINER");
					return false;
			}

			break;
		case ITEM_DRINK_CON:
			switch (value_num)
			{
				case 0:
					send_to_char("MAXIMUM AMOUT OF LIQUID HOURS SET.\n\r\n\r", ch);
					pObj->value[0] = atoi(argument);
					break;
				case 1:
					send_to_char("CURRENT AMOUNT OF LIQUID HOURS SET.\n\r\n\r", ch);
					pObj->value[1] = atoi(argument);
					break;
				case 2:
					send_to_char("LIQUID TYPE SET.\n\r\n\r", ch);
					pObj->value[2] = (liq_lookup(argument) != -1 ? liq_lookup(argument) : 0);
					break;
				case 3:
					send_to_char("POISON VALUE TOGGLED.\n\r\n\r", ch);
					pObj->value[3] = (pObj->value[3] == 0) ? 1 : 0;
					break;
				default:
					do_help(ch, "ITEM_DRINK");
					/* OLC		    do_help( ch, "liquids" );    */
					return false;
			}

			break;
		case ITEM_FOUNTAIN:
			switch (value_num)
			{
				case 0:
					send_to_char("MAXIMUM AMOUT OF LIQUID HOURS SET.\n\r\n\r", ch);
					pObj->value[0] = atoi(argument);
					break;
				case 1:
					send_to_char("CURRENT AMOUNT OF LIQUID HOURS SET.\n\r\n\r", ch);
					pObj->value[1] = atoi(argument);
					break;
				case 2:
					send_to_char("LIQUID TYPE SET.\n\r\n\r", ch);
					pObj->value[2] = (liq_lookup(argument) != -1 ? liq_lookup(argument) : 0);
					break;
				default:
					do_help(ch, "ITEM_FOUNTAIN");
					/* OLC		    do_help( ch, "liquids" );    */
					return false;
			}

			break;
		case ITEM_FOOD:
			switch (value_num)
			{
				case 0:
					send_to_char("HOURS OF FOOD SET.\n\r\n\r", ch);
					pObj->value[0] = atoi(argument);
					break;
				case 1:
					send_to_char("HOURS OF FULL SET.\n\r\n\r", ch);
					pObj->value[1] = atoi(argument);
					break;
				case 3:
					send_to_char("POISON VALUE TOGGLED.\n\r\n\r", ch);
					pObj->value[3] = (pObj->value[3] == 0) ? 1 : 0;
					break;
				default:
					do_help(ch, "ITEM_FOOD");
					return false;
			}

			break;
		case ITEM_MONEY:
			switch (value_num)
			{
				case 0:
					send_to_char("GOLD AMOUNT SET.\n\r\n\r", ch);
					pObj->value[0] = atoi(argument);
					break;
				case 1:
					send_to_char("SILVER AMOUNT SET.\n\r\n\r", ch);
					pObj->value[1] = atoi(argument);
					break;
				default:
					do_help(ch, "ITEM_MONEY");
					return false;
			}

			break;
		default:
			break;
	}

	show_obj_values(ch, pObj);

	return true;
}

bool oedit_show(CHAR_DATA *ch, char *argument)
{
	OBJ_INDEX_DATA *pObj;
	char buf[MAX_STRING_LENGTH];
	OBJ_APPLY_DATA *paf;
	int cnt, i;
	AFFECT_DATA *af;

	EDIT_OBJ(ch, pObj);

	sprintf(buf, "Name:             [%s]\n\r", pObj->name);
	send_to_char(buf, ch);

	sprintf(buf, "Short desc:       [%s]\n\r", pObj->short_descr);
	send_to_char(buf, ch);

	sprintf(buf, "Long desc:        [%s]\n\r", pObj->description);
	send_to_char(buf, ch);

	sprintf(buf, "Area:             [%5d] %s\n\r",
		!pObj->area ? -1 : pObj->area->vnum,
		!pObj->area ? "No Area" : pObj->area->name);
	send_to_char(buf, ch);

	sprintf(buf, "Vnum:             [%5d]\n\r", pObj->vnum);
	sprintf(buf, "Type:             [%s]\n\r", flag_string_old(type_flags, pObj->item_type));
	send_to_char(buf, ch);

	sprintf(buf, "Level:            [%5d]\n\r", pObj->level);
	send_to_char(buf, ch);

	sprintf(buf, "Wear flags:       [%s]\n\r", flag_string(wear_flags, pObj->wear_flags));
	send_to_char(buf, ch);

	sprintf(buf, "Extra flags:      [%s]\n\r", flag_string(extra_flags, pObj->extra_flags));
	send_to_char(buf, ch);

	sprintf(buf, "Restrict flags:   [%s]\n\r", restrict_string(restrict_table, pObj->restrict_flags));
	send_to_char(buf, ch);

	sprintf(buf, "Material:         [%s]\n\r", pObj->material);
	send_to_char(buf, ch);

	sprintf(buf, "Condition:        [%5d]\n\r", pObj->condition);
	send_to_char(buf, ch);

	sprintf(buf, "Weight:           [%5d]\n\r", pObj->weight);
	send_to_char(buf, ch);

	sprintf(buf, "Cost:             [%5d]\n\r", pObj->cost);
	send_to_char(buf, ch);

	sprintf(buf, "Object Limit:     [%d]\n\r", (pObj->limtotal > 0) ? pObj->limtotal : -1);
	send_to_char(buf, ch);

	sprintf(buf, "Timer:            [%d]\n\r", pObj->start_timer);
	send_to_char(buf, ch);

	sprintf(buf, "Cabal:            [%s]\n\r", cabal_table[pObj->cabal].name);
	send_to_char(buf, ch);

	sprintf(buf, "Verb:             [%s]\n\r", (pObj->verb) ? pObj->verb : "none");
	send_to_char(buf, ch);

	if (IS_SET(pObj->wear_flags, ITEM_WEAR_COSMETIC))
	{
		sprintf(buf, "Worn:             [\"%s\"]\n\r",
				pObj->wear_loc_name ? pObj->wear_loc_name : "Default - Needs Set");
		send_to_char(buf, ch);
	}

	if (pObj->spec_prog.func)
	{
		for (int i = 0; ispec_table[i].spec_name; i++)
		{
			if (ispec_table[i].spec_func == pObj->spec_prog.func)
				sprintf(buf, "ISpec:            [%s]\n\r", ispec_table[i].spec_name);
		}

		send_to_char(buf, ch);
	}

	if (!IS_ZERO_VECTOR(pObj->progtypes))
	{
		send_to_char("Objects Programs:\n\r", ch);

		if (IS_SET(pObj->progtypes, IPROG_WEAR))
		{
			sprintf(buf, " [wear_prog] %s\n\r", pObj->iprogs->wear_name);
			send_to_char(buf, ch);
		}

		if (IS_SET(pObj->progtypes, IPROG_REMOVE))
		{
			sprintf(buf, " [remove_prog] %s\n\r", pObj->iprogs->remove_name);
			send_to_char(buf, ch);
		}

		if (IS_SET(pObj->progtypes, IPROG_DROP))
		{
			sprintf(buf, " [drop_prog] %s\n\r", pObj->iprogs->drop_name);
			send_to_char(buf, ch);
		}

		if (IS_SET(pObj->progtypes, IPROG_SAC))
		{
			sprintf(buf, " [sac_prog] %s\n\r", pObj->iprogs->sac_name);
			send_to_char(buf, ch);
		}

		if (IS_SET(pObj->progtypes, IPROG_GIVE))
		{
			sprintf(buf, " [give_prog] %s\n\r", pObj->iprogs->give_name);
			send_to_char(buf, ch);
		}

		if (IS_SET(pObj->progtypes, IPROG_GREET))
		{
			sprintf(buf, " [greet_prog] %s\n\r", pObj->iprogs->greet_name);
			send_to_char(buf, ch);
		}

		if (IS_SET(pObj->progtypes, IPROG_FIGHT))
		{
			sprintf(buf, " [fight_prog] %s\n\r", pObj->iprogs->fight_name);
			send_to_char(buf, ch);
		}

		if (IS_SET(pObj->progtypes, IPROG_DEATH))
		{
			sprintf(buf, " [death_prog] %s\n\r", pObj->iprogs->death_name);
			send_to_char(buf, ch);
		}

		if (IS_SET(pObj->progtypes, IPROG_SPEECH))
		{
			sprintf(buf, " [speech_prog] %s\n\r", pObj->iprogs->speech_name);
			send_to_char(buf, ch);
		}

		if (IS_SET(pObj->progtypes, IPROG_ENTRY))
		{
			sprintf(buf, " [entry_prog] %s\n\r", pObj->iprogs->entry_name);
			send_to_char(buf, ch);
		}

		if (IS_SET(pObj->progtypes, IPROG_GET))
		{
			sprintf(buf, " [get_prog] %s\n\r", pObj->iprogs->get_name);
			send_to_char(buf, ch);
		}

		if (IS_SET(pObj->progtypes, IPROG_PULSE))
		{
			sprintf(buf, " [pulse_prog] %s\n\r", pObj->iprogs->pulse_name);
			send_to_char(buf, ch);
		}

		if (IS_SET(pObj->progtypes, IPROG_INVOKE))
		{
			sprintf(buf, " [invoke_prog] %s\n\r", pObj->iprogs->invoke_name);
			send_to_char(buf, ch);
		}

		if (IS_SET(pObj->progtypes, IPROG_VERB))
		{
			sprintf(buf, " [verb_prog] %s\n\r", pObj->iprogs->verb_name);
			send_to_char(buf, ch);
		}

		if (IS_SET(pObj->progtypes, IPROG_LOOT))
		{
			sprintf(buf, " [loot_prog] %s\n\r", pObj->iprogs->loot_name);
			send_to_char(buf, ch);
		}

		if (IS_SET(pObj->progtypes, IPROG_OPEN))
		{
			sprintf(buf, " [open_prog] %s\n\r", pObj->iprogs->open_name);
			send_to_char(buf, ch);
		}

		if (IS_SET(pObj->progtypes, IPROG_LOOK))
		{
			sprintf(buf, " [look_prog] %s\n\r", pObj->iprogs->look_name);
			send_to_char(buf, ch);
		}

		if (IS_SET(pObj->progtypes, IPROG_HIT))
		{
			sprintf(buf, " [hit_prog] %s\n\r", pObj->iprogs->hit_name);
			send_to_char(buf, ch);
		}
	}

	if (pObj->wear_echo[0] || pObj->wear_echo[1])
	{
		sprintf(buf, "Wear Echos:\n\r");
		send_to_char(buf, ch);

		if (pObj->wear_echo[0])
		{
			sprintf(buf, " Character [%s]\n\r", pObj->wear_echo[0]);
			send_to_char(buf, ch);
		}

		if (pObj->wear_echo[1])
		{
			sprintf(buf, "      Room [%s]\n\r", pObj->wear_echo[1]);
			send_to_char(buf, ch);
		}
	}

	if (pObj->remove_echo[0] || pObj->remove_echo[1])
	{
		sprintf(buf, "Remove Echos:\n\r");
		send_to_char(buf, ch);

		if (pObj->remove_echo[0])
		{
			sprintf(buf, " Character [%s]\n\r", pObj->remove_echo[0]);
			send_to_char(buf, ch);
		}

		if (pObj->remove_echo[1])
		{
			sprintf(buf, "      Room [%s]\n\r", pObj->remove_echo[1]);
			send_to_char(buf, ch);
		}
	}

	if (pObj->notes != nullptr)
	{
		sprintf(buf, "Notes: %s\n\r", pObj->notes);
		send_to_char(buf, ch);
	}

	if (!IS_ZERO_VECTOR(pObj->imm_flags)
		|| !IS_ZERO_VECTOR(pObj->res_flags)
		|| !IS_ZERO_VECTOR(pObj->vuln_flags)
		|| pObj->charaffs)
	{
		send_to_char("Flag data:\n\r", ch);

		if (!IS_ZERO_VECTOR(pObj->imm_flags))
		{
			sprintf(buf, " IMM [%s]\n\r", flag_string(imm_flags, pObj->imm_flags));
			send_to_char(buf, ch);
		}

		if (!IS_ZERO_VECTOR(pObj->res_flags))
		{
			sprintf(buf, " RES [%s]\n\r", flag_string(imm_flags, pObj->res_flags));
			send_to_char(buf, ch);
		}

		if (!IS_ZERO_VECTOR(pObj->vuln_flags))
		{
			sprintf(buf, " VUL [%s]\n\r", flag_string(imm_flags, pObj->vuln_flags));
			send_to_char(buf, ch);
		}

		if (pObj->charaffs)
		{
			i = 0;

			std::string buffer = std::string(" AFF ");
			for (af = pObj->charaffs; af; af = af->next)
			{
				if (i > 0)
					buffer += fmt::format("{}     ", buffer);

				buffer += fmt::format("[{}]\n\r", skill_table[af->type].name);
				i++;
			}

			send_to_char(buffer.c_str(), ch);
		}
	}

	if (pObj->extra_descr)
	{
		for (EXTRA_DESCR_DATA *ed = pObj->extra_descr; ed; ed = ed->next)
		{
			send_to_char("Extra Desc Keyword: ", ch);
			send_to_char(ed->keyword, ch);
			send_to_char("\n\r", ch);
		}
	}

	for (cnt = 0, paf = pObj->apply; paf; paf = paf->next)
	{
		if (cnt == 0)
		{
			send_to_char("Number Modifier Apply\n\r", ch);
			send_to_char("------ -------- -------\n\r", ch);
		}

		sprintf(buf, "[%4d] %-8d %s\n\r", cnt, paf->modifier, flag_string_old(apply_flags, paf->location));
		send_to_char(buf, ch);
		cnt++;
	}

	show_obj_values(ch, pObj);

	return false;
}

/*
 * Need to issue warning if flag isn't valid. -- does so now -- Hugin.
 */
bool oedit_addapply(CHAR_DATA *ch, char *argument)
{
	int value;
	OBJ_INDEX_DATA *pObj;
	OBJ_APPLY_DATA *pAf;
	char loc[MAX_STRING_LENGTH];
	char mod[MAX_STRING_LENGTH];

	EDIT_OBJ(ch, pObj);

	argument = one_argument(argument, loc);
	one_argument(argument, mod);

	if (loc[0] == '\0' || mod[0] == '\0' || !is_number(mod))
	{
		send_to_char("Syntax:  addapply [location] [#mod]\n\r", ch);
		return false;
	}

	value = display_lookup(loc, apply_locations);

	if (value == -1)
	{
		send_to_char("Invalid apply.\n\r", ch);
		return false;
	}

	pAf = new_apply_data();
	pAf->location = value;
	pAf->modifier = atoi(mod);
	pAf->next = pObj->apply;
	pObj->apply = pAf;

	send_to_char("Affect added.\n\r", ch);
	return true;
}

/*
bool oedit_addaffect(CHAR_DATA *ch, char *argument)
{
	int value,value3;
	OBJ_INDEX_DATA *pObj;
	AFFECT_DATA *pAf;
	char loc[MAX_STRING_LENGTH];
	char mod[MAX_STRING_LENGTH];
	char value2[MAX_STRING_LENGTH];

	EDIT_OBJ(ch, pObj);

	argument = one_argument( argument, loc );
	argument = one_argument( argument, mod );
	argument = one_argument( argument, value2 );

	if ( loc[0] == '\0' || mod[0] == '\0' || !is_number( mod ) )
	{
	send_to_char( "Syntax:  addaffect [location] [#xmod]\n\r", ch );
	return false;
	}

	if ( ( value = flag_value( apply_flags, loc ) ) == NO_FLAG )
	{
		send_to_char( "Valid affects are:\n\r", ch );
	show_help( ch, "apply" );
	return false;
	}

	value3 = flag_value( affect_flags, value2 );

	pAf             =   new_affect();
	pAf->location   =   value;
	pAf->modifier   =   atoi( mod );
	pAf->type       =   (value3 == NO_FLAG) ? -1 : skill_lookup(value2);
	pAf->duration   =   -1;
	pAf->bitvector  =   (value3 == NO_FLAG) ? 0 : value3;
	pAf->where	    =	0;
	pAf->next       =   pObj->affected;
	pObj->affected  =   pAf;

	send_to_char( "Affect added.\n\r", ch);
	return true;
}
*/

/*
 * My thanks to Hans Hvidsten Birkeland and Noam Krendel(Walker)
 * for really teaching me how to manipulate pointers.
 */
bool oedit_delapply(CHAR_DATA *ch, char *argument)
{
	OBJ_INDEX_DATA *pObj;
	OBJ_APPLY_DATA *pAf;
	OBJ_APPLY_DATA *pAf_next;
	char apply[MAX_STRING_LENGTH];
	int value;
	int cnt = 0;

	EDIT_OBJ(ch, pObj);

	one_argument(argument, apply);

	if (!is_number(apply) || apply[0] == '\0')
	{
		send_to_char("Syntax:  delapply [#xapply]\n\r", ch);
		return false;
	}

	value = atoi(apply);

	if (value < 0)
	{
		send_to_char("Only non-negative apply-numbers allowed.\n\r", ch);
		return false;
	}

	if (!(pAf = pObj->apply))
	{
		send_to_char("OEdit:  Non-existant apply.\n\r", ch);
		return false;
	}

	if (value == 0) /* First case: Remove first affect */
	{
		pAf = pObj->apply;
		pObj->apply = pAf->next;
		free_apply(pAf);
	}
	else /* Affect to remove is not the first */
	{
		while ((pAf_next = pAf->next) && (++cnt < value))
		{
			pAf = pAf_next;
		}

		if (pAf_next) /* See if it's the next affect */
		{
			pAf->next = pAf_next->next;
			free_apply(pAf_next);
		}
		else /* Doesn't exist */
		{
			send_to_char("No such affect.\n\r", ch);
			return false;
		}
	}

	send_to_char("Affect removed.\n\r", ch);
	return true;
}

bool oedit_msg(CHAR_DATA *ch, char *argument)
{
	OBJ_INDEX_DATA *pObj;
	char type[MSL];
	char target[MSL];

	EDIT_OBJ(ch, pObj);

	argument = one_argument(argument, type);
	argument = one_argument(argument, target);

	if (*type == '\0')
	{
		send_to_char("Syntax:  msg wear ch (echo)\n\r"\
					 "         msg wear room (echo)\n\r"\
					 "         msg remove ch (echo)\n\r"\
					 "         msg remove room (echo)\n\r", ch);
		return false;
	}

	if (!str_prefix(type, "wear"))
	{
		if (!str_prefix(target, "ch"))
		{
			if (pObj->wear_echo[0])
				free_pstring(pObj->wear_echo[0]);

			if (argument != nullptr)
				pObj->wear_echo[0] = palloc_string(argument);
		}
		else if (!str_prefix(target, "room"))
		{
			if (pObj->wear_echo[1])
				free_pstring(pObj->wear_echo[1]);

			if (argument != nullptr)
				pObj->wear_echo[1] = palloc_string(argument);
		}
		else
		{
			send_to_char("That is not a valid target for the message echo.\n\r", ch);
			return false;
		}
	}
	else if (!str_prefix(type, "remove"))
	{
		if (!str_prefix(target, "ch"))
		{
			if (pObj->remove_echo[0])
				free_pstring(pObj->remove_echo[0]);

			if (argument != nullptr)
				pObj->remove_echo[0] = palloc_string(argument);
		}
		else if (!str_prefix(target, "room"))
		{
			if (pObj->remove_echo[1])
				free_pstring(pObj->remove_echo[1]);

			if (argument != nullptr)
				pObj->remove_echo[1] = palloc_string(argument);
		}
		else
		{
			send_to_char("That is not a valid target for the message echo.\n\r", ch);
			return false;
		}
	}
	else
	{
		send_to_char("Syntax:  msg wear ch (echo)\n\r"\
					 "         msg wear room (echo)\n\r"\
					 "         msg remove ch (echo)\n\r"\
					 "         msg remove room (echo)\n\r", ch);
		return false;
	}

	send_to_char("Echo updated.\n\r", ch);
	return true;
}

bool oedit_limit(CHAR_DATA *ch, char *argument)
{
	OBJ_INDEX_DATA *pObj;
	int num;

	EDIT_OBJ(ch, pObj);

	if (argument[0] == '\0')
	{
		send_to_char("Syntax:  limit (number)\n\r"\
					 "         Setting limit to 0 will make the object unlimited.\n\r",
					 ch);
		return false;
	}

	num = atoi(argument);

	if (num == -1)
	{
		send_to_char("Limit must be a valid number greater than or equal to 0.\n\r", ch);
		return false;
	}

	pObj->limtotal = num;
	send_to_char("Limit set.\n\r", ch);
	return true;
}

bool oedit_verb(CHAR_DATA *ch, char *argument)
{
	OBJ_INDEX_DATA *pObj;

	EDIT_OBJ(ch, pObj);

	if (argument[0] == '\0')
	{
		send_to_char("Syntax:  verb (verb name)\n\r", ch);
		return false;
	}

	pObj->verb = palloc_string(argument);
	send_to_char("Verb set.\n\r", ch);
	return true;
}

bool oedit_cabal(CHAR_DATA *ch, char *argument)
{
	OBJ_INDEX_DATA *pObj;
	int cabal;

	EDIT_OBJ(ch, pObj);

	if (argument[0] == '\0')
	{
		send_to_char("Syntax:  cabal (cabal name)\n\r", ch);
		return false;
	}

	cabal = cabal_lookup(argument);

	if (cabal == 0)
	{
		send_to_char("That is not a valid cabal.\n\r", ch);
		return false;
	}

	pObj->cabal = cabal;
	send_to_char("Cabal set.\n\r", ch);
	return true;
}

bool oedit_timer(CHAR_DATA *ch, char *argument)
{
	OBJ_INDEX_DATA *pObj;
	int num;

	EDIT_OBJ(ch, pObj);

	if (argument[0] == '\0')
	{
		send_to_char("Syntax:  timer (number)\n\r"\
					 "         -1 is no timer and 0 is one tick.\n\r", ch);
		return false;
	}

	num = atoi(argument);

	if (num < 0)
	{
		send_to_char("Timer must be a number greater than -1.\n\r", ch);
		return false;
	}

	pObj->start_timer = num;
	send_to_char("Timer set.\n\r", ch);
	return true;
}

bool oedit_restrict(CHAR_DATA *ch, char *argument)
{
	OBJ_INDEX_DATA *pObj;
	int bit, count;

	EDIT_OBJ(ch, pObj);

	if (argument[0] == '\0')
	{
		send_to_char("Syntax:  restrict (type)\n\r", ch);
		return false;
	}

	bit = restrict_lookup(argument);
	if (bit == NO_FLAG)
	{
		send_to_char("That was an invalid restriction type.  Choose from the following:\n\r", ch);

		std::string buffer = std::string("");
		for (count = 0; restrict_table[count].name != nullptr; count++)
		{
			buffer += fmt::format("{} ", restrict_table[count].name);

			if (count % 5 == 0 && count > 0)
				buffer += std::string("\n\r");
		}

		if (count % 5 != 0)
			buffer += std::string("\n\r");

		send_to_char(buffer.c_str(), ch);
		return false;
	}

	TOGGLE_BIT(pObj->restrict_flags, bit);
	send_to_char("Restrict flag toggled.\n\r", ch);
	return true;
}

bool oedit_notes(CHAR_DATA *ch, char *argument)
{
	OBJ_INDEX_DATA *pObj;

	EDIT_OBJ(ch, pObj);

	string_append(ch, &pObj->notes);
	return true;
}

bool oedit_wear_name(CHAR_DATA *ch, char *argument)
{
	OBJ_INDEX_DATA *pObj;
	EDIT_OBJ(ch, pObj);

	if (argument[0] == '\0')
	{
		send_to_char("Syntax:  worn (location) will show <worn (location)> on look\n\r", ch);
		return false;
	}

	pObj->wear_loc_name = palloc_string(argument);
	send_to_char("Wear location name set.\n\r", ch);
	return true;
}

bool oedit_flag(CHAR_DATA *ch, char *argument)
{
	OBJ_INDEX_DATA *pObj;
	char arg1[MSL], arg2[MSL], arg3[MSL], arg4[MSL];
	long bit;
	AFFECT_DATA pAf, *af;

	EDIT_OBJ(ch, pObj);

	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);
	argument = one_argument(argument, arg3);
	argument = one_argument(argument, arg4);

	if (!str_cmp(arg1, "imm"))
	{
		bit = flag_lookup(arg2, imm_flags);

		if (bit == NO_FLAG)
		{
			send_to_char("That is not a valid flag type for immunities.\n\r", ch);
			return false;
		}
		else
		{
			TOGGLE_BIT(pObj->imm_flags, bit);
		}
	}
	else if (!str_cmp(arg1, "res"))
	{
		bit = flag_lookup(arg2, imm_flags);
		if (bit == NO_FLAG)
		{
			send_to_char("That is not a valid flag type for resistances.\n\r", ch);
			return false;
		}
		else
		{
			TOGGLE_BIT(pObj->res_flags, bit);
		}
	}
	else if (!str_cmp(arg1, "vul"))
	{
		bit = flag_lookup(arg2, imm_flags);

		if (bit == NO_FLAG)
		{
			send_to_char("That is not a valid flag type for vulnerabilities.\n\r", ch);
			return false;
		}
		else
		{
			TOGGLE_BIT(pObj->vuln_flags, bit);
		}
	}
	else if (!str_cmp(arg1, "aff"))
	{
		if (skill_lookup(arg2) == -1)
		{
			send_to_char(arg2, ch);
			return false;
		}

		if (!str_cmp(arg3, "delete"))
		{
			for (af = pObj->charaffs; af != nullptr; af = af->next)
			{
				if (af->type == skill_lookup(arg2))
					break;
			}

			if (af == nullptr || skill_lookup(arg2) == -1)
			{
				send_to_char("That object doesn't have that flag affect.\n\r", ch);
				return false;
			}

			charaff_from_obj_index(pObj, af);
			send_to_char("Flag affect removed.\n\r", ch);
			return true;
		}

		init_affect(&pAf);
		pAf.where = TO_AFFECTS;
		pAf.aftype = AFT_INVIS;
		pAf.type = skill_lookup(arg2);
		bit = flag_lookup(arg3, affect_flags);

		if (bit != NO_FLAG)
			SET_BIT(pAf.bitvector, bit);

		if (!str_prefix(arg4, "show"))
			pAf.aftype = AFT_SPELL;

		pAf.level = pObj->level;
		charaff_to_obj_index(pObj, &pAf);
		top_affect++;
		return true;
	}
	else
	{
		send_to_char("Syntax:  flag imm <type>\n\r"\
					 "         flag res <type>\n\r"\
					 "         flag vul <type>\n\r"\
					 "         flag aff 'spell' <affect_bit> show|noshow\n\r"\
					 "         flag aff 'spell' delete\n\r", ch);
		return false;
	}

	send_to_char("Flag toggled.\n\r", ch);
	return true;
}

bool oedit_name(CHAR_DATA *ch, char *argument)
{
	OBJ_INDEX_DATA *pObj;

	EDIT_OBJ(ch, pObj);

	if (argument[0] == '\0')
	{
		send_to_char("Syntax:  name [string]\n\r", ch);
		return false;
	}

	free_pstring(pObj->name);
	pObj->name = palloc_string(argument);

	send_to_char("Name set.\n\r", ch);
	return true;
}

bool oedit_short(CHAR_DATA *ch, char *argument)
{
	OBJ_INDEX_DATA *pObj;

	EDIT_OBJ(ch, pObj);

	if (argument[0] == '\0')
	{
		send_to_char("Syntax:  short [string]\n\r", ch);
		return false;
	}

	free_pstring(pObj->short_descr);
	pObj->short_descr = palloc_string(argument);
	pObj->short_descr[0] = LOWER(pObj->short_descr[0]);

	send_to_char("Short description set.\n\r", ch);
	return true;
}

bool oedit_long(CHAR_DATA *ch, char *argument)
{
	OBJ_INDEX_DATA *pObj;

	EDIT_OBJ(ch, pObj);

	if (argument[0] == '\0')
	{
		send_to_char("Syntax:  long [string]\n\r", ch);
		return false;
	}

	free_pstring(pObj->description);
	pObj->description = palloc_string(argument);
	pObj->description[0] = UPPER(pObj->description[0]);

	send_to_char("Long description set.\n\r", ch);
	return true;
}

bool set_value(CHAR_DATA *ch, OBJ_INDEX_DATA *pObj, char *argument, int value)
{
	if (argument[0] == '\0')
	{
		set_obj_values(ch, pObj, -1, ""); /* '\0' changed to "" -- Hugin */
		return false;
	}

	if (set_obj_values(ch, pObj, value, argument))
		return true;

	return false;
}

bool oedit_values(CHAR_DATA *ch, char *argument, int value)
{
	OBJ_INDEX_DATA *pObj;

	EDIT_OBJ(ch, pObj);

	if (set_value(ch, pObj, argument, value))
		return true;

	return false;
}

bool oedit_value0(CHAR_DATA *ch, char *argument)
{
	if (oedit_values(ch, argument, 0))
		return true;

	return false;
}

bool oedit_value1(CHAR_DATA *ch, char *argument)
{
	if (oedit_values(ch, argument, 1))
		return true;

	return false;
}

bool oedit_value2(CHAR_DATA *ch, char *argument)
{
	if (oedit_values(ch, argument, 2))
		return true;

	return false;
}

bool oedit_value3(CHAR_DATA *ch, char *argument)
{
	if (oedit_values(ch, argument, 3))
		return true;

	return false;
}

bool oedit_value4(CHAR_DATA *ch, char *argument)
{
	if (oedit_values(ch, argument, 4))
		return true;

	return false;
}

bool oedit_weight(CHAR_DATA *ch, char *argument)
{
	OBJ_INDEX_DATA *pObj;

	EDIT_OBJ(ch, pObj);

	if (argument[0] == '\0' || !is_number(argument))
	{
		send_to_char("Syntax:  weight [number]\n\r", ch);
		return false;
	}

	pObj->weight = atoi(argument);

	send_to_char("Weight set.\n\r", ch);
	return true;
}

bool oedit_cost(CHAR_DATA *ch, char *argument)
{
	OBJ_INDEX_DATA *pObj;

	EDIT_OBJ(ch, pObj);

	if (argument[0] == '\0' || !is_number(argument))
	{
		send_to_char("Syntax:  cost [number]\n\r", ch);
		return false;
	}

	pObj->cost = atoi(argument);

	send_to_char("Cost set.\n\r", ch);
	return true;
}

bool oedit_create(CHAR_DATA *ch, char *argument)
{
	OBJ_INDEX_DATA *pObj;
	AREA_DATA *pArea;
	int value;
	int iHash;

	value = atoi(argument);

	if (argument[0] == '\0' || value == 0)
	{
		send_to_char("Syntax:  oedit create [vnum]\n\r", ch);
		return false;
	}

	pArea = get_vnum_area(value);

	if (!pArea)
	{
		send_to_char("OEdit:  That vnum is not assigned an area.\n\r", ch);
		return false;
	}

	if (!IS_BUILDER(ch, pArea))
	{
		send_to_char("OEdit:  Vnum in an area you cannot build in.\n\r", ch);
		return false;
	}

	if (get_obj_index(value))
	{
		send_to_char("OEdit:  Object vnum already exists.\n\r", ch);
		return false;
	}

	pObj = new_obj_index();
	pObj->vnum = value;
	pObj->area = pArea;

	if (value > top_vnum_obj)
		top_vnum_obj = value;

	iHash = value % MAX_KEY_HASH;
	pObj->next = obj_index_hash[iHash];
	obj_index_hash[iHash] = pObj;
	ch->desc->pEdit = (void *)pObj;

	send_to_char("Object Created.\n\r", ch);
	return true;
}

bool oedit_ed(CHAR_DATA *ch, char *argument)
{
	OBJ_INDEX_DATA *pObj;
	EXTRA_DESCR_DATA *ed;
	char command[MAX_INPUT_LENGTH];

	EDIT_OBJ(ch, pObj);

	argument = one_argument(argument, command);

	if (command[0] == '\0')
	{
		send_to_char("Syntax:  ed add [keywords]\n\r", ch);
		send_to_char("         ed delete [keyword]\n\r", ch);
		send_to_char("         ed edit [keyword]\n\r", ch);
		send_to_char("         ed format [keyword]\n\r", ch);
		return false;
	}

	if (!str_cmp(command, "add"))
	{
		if (argument[0] == '\0')
		{
			send_to_char("Syntax:  ed add [keywords]\n\r", ch);
			return false;
		}

		ed = new_extra_descr();
		ed->keyword = palloc_string(argument);
		ed->next = pObj->extra_descr;
		pObj->extra_descr = ed;

		string_append(ch, &ed->description);

		return true;
	}

	if (!str_cmp(command, "edit"))
	{
		if (argument[0] == '\0')
		{
			send_to_char("Syntax:  ed edit [keyword]\n\r", ch);
			return false;
		}

		for (ed = pObj->extra_descr; ed; ed = ed->next)
		{
			if (is_name(argument, ed->keyword))
				break;
		}

		if (!ed)
		{
			send_to_char("OEdit:  Extra description keyword not found.\n\r", ch);
			return false;
		}

		string_append(ch, &ed->description);

		return true;
	}

	if (!str_cmp(command, "delete"))
	{
		EXTRA_DESCR_DATA *ped = nullptr;

		if (argument[0] == '\0')
		{
			send_to_char("Syntax:  ed delete [keyword]\n\r", ch);
			return false;
		}

		for (ed = pObj->extra_descr; ed; ed = ed->next)
		{
			if (is_name(argument, ed->keyword))
				break;
			ped = ed;
		}

		if (!ed)
		{
			send_to_char("OEdit:  Extra description keyword not found.\n\r", ch);
			return false;
		}

		if (!ped)
			pObj->extra_descr = ed->next;
		else
			ped->next = ed->next;

		free_extra_descr(ed);

		send_to_char("Extra description deleted.\n\r", ch);
		return true;
	}

	if (!str_cmp(command, "format"))
	{
		EXTRA_DESCR_DATA *ped = nullptr;

		if (argument[0] == '\0')
		{
			send_to_char("Syntax:  ed format [keyword]\n\r", ch);
			return false;
		}

		for (ed = pObj->extra_descr; ed; ed = ed->next)
		{
			if (is_name(argument, ed->keyword))
				break;
			ped = ed;
		}

		if (!ed)
		{
			send_to_char("OEdit:  Extra description keyword not found.\n\r", ch);
			return false;
		}

		ed->description = format_string(ed->description);

		send_to_char("Extra description formatted.\n\r", ch);
		return true;
	}

	oedit_ed(ch, "");
	return false;
}

/* ROM object functions : */

bool oedit_extra(CHAR_DATA *ch, char *argument) /* Moved out of oedit() due to naming conflicts -- Hugin */
{
	OBJ_INDEX_DATA *pObj;
	int value;

	if (argument[0] != '\0')
	{
		EDIT_OBJ(ch, pObj);

		value = flag_value(extra_flags, argument);
		if ((value) != NO_FLAG)
		{
			TOGGLE_BIT(pObj->extra_flags, value);

			send_to_char("Extra flag toggled.\n\r", ch);
			return true;
		}
	}

	send_to_char("Syntax:  extra [flag]\n\r"
				 "Type '? extra' for a list of flags.\n\r",
				 ch);
	return false;
}

bool oedit_wear(CHAR_DATA *ch, char *argument) /* Moved out of oedit() due to naming conflicts -- Hugin */
{
	OBJ_INDEX_DATA *pObj;
	int value;

	if (argument[0] != '\0')
	{
		EDIT_OBJ(ch, pObj);

		value = flag_value(wear_flags, argument);
		if ((value) != NO_FLAG)
		{
			TOGGLE_BIT(pObj->wear_flags, value);

			send_to_char("Wear flag toggled.\n\r", ch);
			return true;
		}
	}

	send_to_char("Syntax:  wear [flag]\n\r"\
				 "Type '? wear' for a list of flags.\n\r", ch);
	return false;
}

bool oedit_type(CHAR_DATA *ch, char *argument) /* Moved out of oedit() due to naming conflicts -- Hugin */
{
	OBJ_INDEX_DATA *pObj;
	int value;

	if (argument[0] != '\0')
	{
		EDIT_OBJ(ch, pObj);

		value = flag_value(type_flags, argument);
		if ((value) != NO_FLAG)
		{
			pObj->item_type = value;

			send_to_char("Type set.\n\r", ch);

			/*
			 * Clear the values.
			 */
			pObj->value[0] = 0;
			pObj->value[1] = 0;
			pObj->value[2] = 0;
			pObj->value[3] = 0;
			pObj->value[4] = 0; /* ROM */

			return true;
		}
	}

	send_to_char("Syntax:  item [flag]\n\r"\
				 "Type '? item' for a list of flags.\n\r",
				 ch);
	return false;
}

bool oedit_material(CHAR_DATA *ch, char *argument)
{
	OBJ_INDEX_DATA *pObj;

	EDIT_OBJ(ch, pObj);

	if (argument[0] == '\0')
	{
		send_to_char("Syntax:  material [string]\n\r", ch);
		return false;
	}

	free_pstring(pObj->material);
	pObj->material = palloc_string(argument);

	send_to_char("Material set.\n\r", ch);
	return true;
}

/*
bool oedit_material(CHAR_DATA *ch, char *argument)
{
	OBJ_INDEX_DATA *pObj;
	int value;

	EDIT_OBJ(ch, pObj);

	if ( argument[0] != '\0' )
	{

	if ( ( value = flag_value( material_type, argument ) ) != NO_FLAG )
	{
		pObj->material = material_name(value);
		send_to_char( "Material type set.\n\r", ch);
		return true;
	}
	}

	send_to_char( "Syntax:  material [material-name]\n\r"
		  "Type '? material' for a list of materials.\n\r", ch );
	return false;
}
*/

bool oedit_level(CHAR_DATA *ch, char *argument)
{
	OBJ_INDEX_DATA *pObj;

	EDIT_OBJ(ch, pObj);

	if (argument[0] == '\0' || !is_number(argument))
	{
		send_to_char("Syntax:  level [number]\n\r", ch);
		return false;
	}

	pObj->level = atoi(argument);

	send_to_char("Level set.\n\r", ch);
	return true;
}

bool oedit_condition(CHAR_DATA *ch, char *argument)
{
	OBJ_INDEX_DATA *pObj;
	int value;

	if (argument[0] != '\0' && (value = atoi(argument)) >= 0 && (value <= 100))
	{
		EDIT_OBJ(ch, pObj);

		pObj->condition = value;
		send_to_char("Condition set.\n\r", ch);

		return true;
	}

	send_to_char("Syntax:  condition [number]\n\r"\
				 "Where number can range from 0 (ruined) to 100 (perfect).\n\r", ch);
	return false;
}

/*
 * Mobile Editor Functions.
 */
bool medit_limit(CHAR_DATA *ch, char *argument)
{
	MOB_INDEX_DATA *pMob;
	char arg1[MSL], arg2[MSL];
	int low, high;

	EDIT_MOB(ch, pMob);
	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);

	if (!*arg1 || !*arg2)
	{
		send_to_char("Syntax:  limit <low vnum> <high vnum>\n\r", ch);
		return false;
	}

	if ((low = atoi(arg1)) == -1 || (high = atoi(arg2)) == -1)
	{
		send_to_char("Syntax:  limit <low vnum> <high vnum>\n\r", ch);
		return false;
	}

	pMob->restrict_low = low;
	pMob->restrict_high = high;

	send_to_char("Limit set.\n\r", ch);
	return true;
}

bool medit_optional(CHAR_DATA *ch, char *argument)
{
	MOB_INDEX_DATA *pMob;
	char arg1[MSL], arg2[MSL], arg3[MSL], arg4[MSL];
	char arg5[MSL], arg6[MSL], arg7[MSL];
	int sn, bit, i;
	bool added = false;
	BARRED_DATA *bar = nullptr;

	EDIT_MOB(ch, pMob);

	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);
	argument = one_argument(argument, arg3);
	argument = one_argument(argument, arg4);
	argument = one_argument(argument, arg5);
	argument = one_argument(argument, arg6);

	if (arg1[0] == 'A' || arg1[0] == 'a')
	{
		if (!str_cmp(arg2, "delete"))
		{
			sn = atoi(arg3);

			if (sn >= 0 && sn < MAX_MOB_AFFECT)
			{
				pMob->affect_sn[sn] = -1;
				pMob->affect_bit[sn] = 0;
				send_to_char("Optional Mob Affect Removed.\n\r", ch);
				return true;
			}
			else
			{
				send_to_char("Not a valid id for mob affect.\n\r", ch);
				return false;
			}
		}

		sn = skill_lookup(arg2);

		if (sn == -1)
		{
			send_to_char("That is not a valid spell.\n\r", ch);
			return false;
		}

		bit = flag_lookup(arg3, affect_flags);

		if (sn)
		{
			for (i = 0; i < MAX_MOB_AFFECT; i++)
			{
				if (pMob->affect_sn[i] < 1)
				{
					pMob->affect_sn[i] = sn;
					pMob->affect_bit[i] = bit;
					added = true;
					break;
				}
			}
		}

		if (added)
		{
			send_to_char("Affected added.\n\r", ch);
			return true;
		}
	}
	else if (arg1[0] == 'B' || arg1[0] == 'b')
	{
		bar = new BARRED_DATA;
		CLEAR_MEM(bar, sizeof(BARRED_DATA))

		if (pMob->barred_entry)
		{
			if (!str_cmp(arg2, "delete"))
			{
				pMob->barred_entry = nullptr;
				delete pMob->barred_entry;
				send_to_char("Barred entry removed.\n\r", ch);
				return false;
			}
			else
			{
				send_to_char("This mob already has a barred exit.\n\r", ch);
				return false;
			}
		}

		bar->type = flag_lookup(arg2, criterion_flags);

		if (bar->type == NO_FLAG)
		{
			send_to_char("Invalid Bar Entry Type.\n\r", ch);
			return false;
		}

		bar->comparison = -1;

		if (!str_cmp(arg3, "EQUALTO") || !str_cmp(arg3, "equalto"))
		{
			bar->comparison = BAR_EQUAL_TO;
		}
		else if (!str_cmp(arg3, "LESSTHAN") || !str_cmp(arg3, "lessthan"))
		{
			bar->comparison = BAR_LESS_THAN;
		}
		else if (!str_cmp(arg3, "GREATERTHAN") || !str_cmp(arg3, "greaterthan"))
		{
			bar->comparison = BAR_GREATER_THAN;
		}
		else
		{
			send_to_char("Not a valid operator.\n\r", ch);
			return false;
		}

		if (bar->type == 1)
			bar->value = CClass::Lookup(arg4);
		else
			bar->value = atoi(arg4);

		if (bar->value == -1)
		{
			send_to_char("Not a valid value.\n\r", ch);
			return false;
		}

		bar->vnum = atoi(arg5);

		if (bar->vnum == -1)
		{
			send_to_char("Not a valid vnum.\n\r", ch);
			return false;
		}

		if (!str_cmp(arg6, "SAY") || !str_cmp(arg6, "say"))
		{
			bar->msg_type = BAR_SAY;
		}
		else if (!str_cmp(arg6, "EMOTE") || !str_cmp(arg6, "emote"))
		{
			bar->msg_type = BAR_EMOTE;
		}
		else if (!str_cmp(arg6, "ECHO") || !str_cmp(arg6, "echo"))
		{
			argument = one_argument(argument, arg7);

			bar->msg_type = BAR_ECHO;
			bar->message = palloc_string(arg7);
			bar->message_two = palloc_string(argument);

			if (!str_cmp(bar->message_two, ""))
				bar->message_two = nullptr;

			pMob->barred_entry = bar;
		}
		else
		{
			send_to_char("Not a valid message type.\n\r", ch);
			return false;
		}

		bar->message = palloc_string(argument);
		pMob->barred_entry = bar;
		send_to_char("Barred exit added.\n\r", ch);
		return true;
	}
	else if (arg1[0] == 'C' || arg1[0] == 'c')
	{
		if (!str_cmp(arg2, "delete"))
		{
			sn = atoi(arg3);

			if (sn >= 0 && sn < MAX_MOB_CAST)
			{
				free_pstring(pMob->cast_spell[sn]);
				free_pstring(pMob->cast_spell[sn] = nullptr);
				send_to_char("Optional: Castable Spell Reset.\n\r", ch);
				return true;
			}
			else
			{
				send_to_char("That is not a valid ID for a castable spell.\n\r", ch);
				return false;
			}
		}

		sn = skill_lookup(arg2);

		if (sn == -1)
		{
			send_to_char("That is not a valid spell.\n\r", ch);
			return false;
		}

		if (sn)
		{
			for (i = 0; i < MAX_MOB_CAST; i++)
			{
				if (pMob->cast_spell[i] == nullptr)
				{
					pMob->cast_spell[i] = palloc_string(arg2);
					added = true;
					break;
				}
			}
		}

		if (added)
		{
			send_to_char("Castable spell added.\n\r", ch);
			return true;
		}
	}
	else
	{
		send_to_char("Syntax:  optional a 'spell' <bit>\n\r"\
					 "         optional b <field> <operator> <value> <vnum> <response type> <message>\n\r"\
					 "         optional c 'spell'\n\r"\
					 "         optional <a/b/c> delete #\n\r", ch);
		return false;
	}

	return false;
}

bool medit_yell(CHAR_DATA *ch, char *argument)
{
	MOB_INDEX_DATA *pMob;

	EDIT_MOB(ch, pMob);

	pMob->attack_yell = palloc_string(argument);
	return true;
}

bool medit_notes(CHAR_DATA *ch, char *argument)
{
	MOB_INDEX_DATA *pMob;

	EDIT_MOB(ch, pMob);

	string_append(ch, &pMob->notes);
	return true;
}

void clean_mob_class(MOB_INDEX_DATA *pMob, int class_index)
{
	if (class_index == CLASS_WARRIOR || class_index == CLASS_NONE)
	{
		pMob->ele_major = 0;
		pMob->ele_para = 0;
	}

	if (class_index == CLASS_SORCERER || class_index == CLASS_NONE)
	{
		zero_vector(pMob->styles);
	}

	pMob->SetClass(class_index);
}

bool medit_class(CHAR_DATA *ch, char *argument)
{
	MOB_INDEX_DATA *pMob;
	int style;
	char arg1[MSL], arg2[MSL], arg3[MSL];

	EDIT_MOB(ch, pMob);

	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);
	argument = one_argument(argument, arg3);

	if (!str_cmp(arg1, "warrior"))
	{
		style = style_lookup(arg2);

		if (!style)
		{
			send_to_char("Not a valid style.\n\r", ch);
			return false;
		}

		if ((style_count(pMob->styles) >= 2) && !IS_SET(pMob->styles, style_table[style].bit))
		{
			send_to_char("Mobs may only have two styles.\n\r", ch);
			return false;
		}

		TOGGLE_BIT(pMob->styles, style_table[style].bit);

		if (IS_ZERO_VECTOR(pMob->styles))
			pMob->SetClass(CLASS_NONE);
		else
			clean_mob_class(pMob, CLASS_WARRIOR);

		send_to_char("Style toggled.\n\r", ch);
		return true;
	}
	else if (!str_cmp(arg1, "sorcerer"))
	{
		clean_mob_class(pMob, CLASS_SORCERER);

		if (!str_prefix(arg2, "major"))
		{
			style = ele_name_lookup(arg3);

			if (!style)
			{
				send_to_char("Not a valid element name.\n\r", ch);
				return false;
			}

			pMob->ele_major = style;
		}
		else
		{
			style = ele_name_lookup(arg3);

			if (!style)
			{
				send_to_char("Not a valid element name.\n\r", ch);
				return false;
			}

			pMob->ele_para = style;

		}
		if (pMob->ele_major == ELE_NONE && pMob->ele_para == ELE_NONE)
			pMob->SetClass(CLASS_NONE);
		else
			clean_mob_class(pMob, CLASS_SORCERER);

		send_to_char("Sorcerer focus set.\n\r", ch);
		return true;
	}
	else if (!str_cmp(arg1, "delete"))
	{
		clean_mob_class(pMob, CLASS_NONE);
		send_to_char("Class removed.\n\r", ch);
		return true;
	}
	else
	{
		send_to_char("Syntax:  class warrior <style>\n\r"\
					 "         class sorcerer <major/para> <element>\n\r"\
					 "         class delete\n\r", ch);
		return false;
	}
}

bool medit_show(CHAR_DATA *ch, char *argument)
{
	MOB_INDEX_DATA *pMob;
	char buf[MAX_STRING_LENGTH], msg_type[MSL], comparison[MSL];
	int i;

	EDIT_MOB(ch, pMob);

	sprintf(buf, "Name:        [%s]\n\rArea:        [%5d] %s\n\r",
		pMob->player_name,
		!pMob->area ? -1 : pMob->area->vnum,
		!pMob->area ? "No Area" : pMob->area->name);
	send_to_char(buf, ch);

	sprintf(buf, "Cabal:       [%s]\n\r", cabal_table[pMob->cabal].name);
	send_to_char(buf, ch);

	sprintf(buf, "Act:         [%s]\n\r", flag_string(act_flags, pMob->act));
	send_to_char(buf, ch);

	sprintf(buf, "Vnum:        [%5d]\n\rSex:         [%s]\n\r",
		pMob->vnum,
		pMob->sex == SEX_MALE ? "male" :
			pMob->sex == SEX_FEMALE ? "female" :
			pMob->sex == 3 ? "random" : "neutral"); /* ROM magic number */
	send_to_char(buf, ch);

	sprintf(buf, "Race:        [%s]\n\r", race_table[pMob->race].name); /* ROM OLC */
	send_to_char(buf, ch);

	if (pMob->Class()->GetIndex() != CLASS_NONE)
	{
		sprintf(buf, "Class:       [%s]\n\r", (RSTR)pMob->Class()->name);
		send_to_char(buf, ch);

		if (pMob->Class()->GetIndex() == CLASS_WARRIOR)
		{
			sprintf(buf, "-Styles:     [%s]\n\r", style_string(style_table, pMob->styles));
			send_to_char(buf, ch);
		}
		else if (pMob->Class()->GetIndex() == CLASS_SORCERER)
		{
			sprintf(buf,
					"-Major:      [%s]\n\r"\
					"-Para:       [%s]\n\r",
					sphere_table[pMob->ele_major].name, sphere_table[pMob->ele_para].name);
			send_to_char(buf, ch);
		}
	}

	sprintf(buf, "Level:       [%2d]\n\rAlign:       [%4d]\n\r", pMob->level, pMob->alignment);
	send_to_char(buf, ch);

	/* ROM values: */

	sprintf(buf, "Hitroll:     [%d]\n\r", pMob->hitroll);
	send_to_char(buf, ch);

	sprintf(buf, "Damtype:     [%s]\n\r", attack_table[pMob->dam_type].name);
	send_to_char(buf, ch);

	sprintf(buf, "Hitdice:     [%2dd%-3d+%4d]\n\r",
		pMob->hit[DICE_NUMBER],
		pMob->hit[DICE_TYPE],
		pMob->hit[DICE_BONUS]);
	send_to_char(buf, ch);

	sprintf(buf, "Damdice:     [%2dd%-3d+%4d]\n\r",
		pMob->damage[DICE_NUMBER],
		pMob->damage[DICE_TYPE],
		pMob->damage[DICE_BONUS]);
	send_to_char(buf, ch);

	/* ROM values end */

	sprintf(buf, "Affected by: [%s]\n\r", flag_string(affect_flags, pMob->affected_by));
	send_to_char(buf, ch);

	/* ROM values: */

	sprintf(buf, "Armor:       [pierce: %d  bash: %d  slash: %d  magic: %d]\n\r",
		pMob->ac[AC_PIERCE],
		pMob->ac[AC_BASH],
		pMob->ac[AC_SLASH],
		pMob->ac[AC_EXOTIC]);
	send_to_char(buf, ch);

	sprintf(buf, "Form:        [%s]\n\r", flag_string(form_flags, pMob->form));
	send_to_char(buf, ch);

	sprintf(buf, "Parts:       [%s]\n\r", flag_string(part_flags, pMob->parts));
	send_to_char(buf, ch);

	sprintf(buf, "Imm:         [%s]\n\r", flag_string(imm_flags, pMob->imm_flags));
	send_to_char(buf, ch);

	sprintf(buf, "Res:         [%s]\n\r", flag_string(res_flags, pMob->res_flags));
	send_to_char(buf, ch);

	sprintf(buf, "Vuln:        [%s]\n\r", flag_string(vuln_flags, pMob->vuln_flags));
	send_to_char(buf, ch);

	sprintf(buf, "Off:         [%s]\n\r", flag_string(off_flags, pMob->off_flags));
	send_to_char(buf, ch);

	sprintf(buf, "Size:        [%s]\n\r", flag_string_old(size_flags, pMob->size));
	send_to_char(buf, ch);

	sprintf(buf, "Start pos.   [%s]\n\r", flag_string_old(position_flags, pMob->start_pos));
	send_to_char(buf, ch);

	sprintf(buf, "Wealth:      [%s]\n\r", flag_name_lookup(pMob->wealth, wealth_table));
	send_to_char(buf, ch);

	/* ROM values end */

	sprintf(buf, "Short descr: %s\n\rLong descr:\n\r%s", pMob->short_descr, pMob->long_descr);
	send_to_char(buf, ch);

	sprintf(buf, "Description:\n\r%s", pMob->description);
	send_to_char(buf, ch);

	if (pMob->notes)
	{
		sprintf(buf, "Notes:\n\r%s\n\r", pMob->notes);
		send_to_char(buf, ch);
	}

	if (pMob->attack_yell)
	{
		sprintf(buf, "Attack yell: [%s]\n\r", pMob->attack_yell);
		send_to_char(buf, ch);
	}

	if (pMob->pShop)
	{
		SHOP_DATA *pShop = pMob->pShop;

		sprintf(buf, "  Hours: %d to %d.\n\r", pShop->open_hour, pShop->close_hour);
		send_to_char(buf, ch);
	}

	if (pMob->restrict_low != LOW_VNUM && pMob->restrict_high != HIGH_VNUM)
	{
		sprintf(buf, "Limit:       [%d-%d]\n\r", pMob->restrict_low, pMob->restrict_high);
		send_to_char(buf, ch);
	}

	sprintf(buf, "Optional: Spell Affects:\n\r");
	send_to_char(buf, ch);

	for (i = 0; i < MAX_MOB_AFFECT; i++)
	{
		if (pMob->affect_sn[i] > -1)
		{
			sprintf(buf, " [%2d] '%s' %s\n",
				i,
				skill_table[pMob->affect_sn[i]].name,
				flag_name_lookup(pMob->affect_bit[i], affect_flags));
			send_to_char(buf, ch);
		}
	}

	sprintf(buf, "Optional: Castable Spells:\n\r");
	send_to_char(buf, ch);

	for (i = 0; i < MAX_MOB_CAST; i++)
	{
		if (pMob->cast_spell[i])
		{
			sprintf(buf, " [%2d] '%s'\n\r", i, pMob->cast_spell[i]);
			send_to_char(buf, ch);
		}
	}

	if (pMob->barred_entry)
	{
		sprintf(buf, "Optional: Barred Exit:\n\r");
		send_to_char(buf, ch);

		switch (pMob->barred_entry->comparison)
		{
			case BAR_EQUAL_TO:
				sprintf(comparison, "EQUALTO");
				break;
			case BAR_LESS_THAN:
				sprintf(comparison, "LESSTHAN");
				break;
			case BAR_GREATER_THAN:
				sprintf(comparison, "GREATERTHAN");
				break;
		}

		switch (pMob->barred_entry->msg_type)
		{
			case BAR_SAY:
				sprintf(msg_type, "SAY");
				break;
			case BAR_EMOTE:
				sprintf(msg_type, "EMOTE");
				break;
			case BAR_ECHO:
				sprintf(msg_type, "ECHO");
				break;
		}

		sprintf(buf, " Check Type:   [%s]\n\r", flag_name_lookup(pMob->barred_entry->type, criterion_flags));
		send_to_char(buf, ch);

		auto buffer = fmt::format(" Comparison:   [{}]\n\r", comparison); //TODO: change the rest of the sprintf calls to format
		send_to_char(buffer.c_str(), ch);

		sprintf(buf, " Value:        [%d]\n\r", pMob->barred_entry->value);
		send_to_char(buf, ch);

		sprintf(buf, " Target Vnum:  [%d]\n\r", pMob->barred_entry->vnum);
		send_to_char(buf, ch);

		buffer = fmt::format(" Message Type: [{}]\n\r", msg_type);
		send_to_char(buffer.c_str(), ch);

		if (pMob->barred_entry->msg_type == BAR_ECHO)
		{
			sprintf(buf, " Message to:   [%s]\n\r", pMob->barred_entry->message);
			send_to_char(buf, ch);

			sprintf(buf, " Message room: [%s]\n\r", pMob->barred_entry->message_two);
			send_to_char(buf, ch);
		}
		else
		{
			sprintf(buf, " Message:      [%s]\n\r", pMob->barred_entry->message);
			send_to_char(buf, ch);
		}
	}

	if (pMob->pShop)
	{
		sprintf(buf,
				"Shop:\n\r"\
				" Open:      [%d]\n\r"\
				" Close:     [%d]\n\r"\
				" Direction: [%s]\n\r",
				pMob->pShop->open_hour, pMob->pShop->close_hour,
				flag_name_lookup(pMob->pShop->direction, direction_table));
		send_to_char(buf, ch);
	}

	if (!IS_ZERO_VECTOR(pMob->progtypes))
	{
		send_to_char("Mob Programs:\n\r", ch);

		if (IS_SET(pMob->progtypes, MPROG_BRIBE))
		{
			sprintf(buf, " [bribe_prog] %s\n\r", pMob->mprogs->bribe_name);
			send_to_char(buf, ch);
		}

		if (IS_SET(pMob->progtypes, MPROG_ENTRY))
		{
			sprintf(buf, " [entry_prog] %s\n\r", pMob->mprogs->entry_name);
			send_to_char(buf, ch);
		}

		if (IS_SET(pMob->progtypes, MPROG_GREET))
		{
			sprintf(buf, " [greet_prog] %s\n\r", pMob->mprogs->greet_name);
			send_to_char(buf, ch);
		}

		if (IS_SET(pMob->progtypes, MPROG_FIGHT))
		{
			sprintf(buf, " [fight_prog] %s\n\r", pMob->mprogs->fight_name);
			send_to_char(buf, ch);
		}

		if (IS_SET(pMob->progtypes, MPROG_DEATH))
		{
			sprintf(buf, " [death_prog] %s\n\r", pMob->mprogs->death_name);
			send_to_char(buf, ch);
		}

		if (IS_SET(pMob->progtypes, MPROG_BEAT))
		{
			sprintf(buf, " [beat_prog] %s\n\r", pMob->mprogs->beat_name);
			send_to_char(buf, ch);
		}

		if (IS_SET(pMob->progtypes, MPROG_PULSE))
		{
			sprintf(buf, " [pulse_prog] %s\n\r", pMob->mprogs->pulse_name);
			send_to_char(buf, ch);
		}

		if (IS_SET(pMob->progtypes, MPROG_SPEECH))
		{
			sprintf(buf, " [speech_prog] %s\n\r", pMob->mprogs->speech_name);
			send_to_char(buf, ch);
		}

		if (IS_SET(pMob->progtypes, MPROG_GIVE))
		{
			sprintf(buf, " [give_prog] %s\n\r", pMob->mprogs->give_name);
			send_to_char(buf, ch);
		}

		if (IS_SET(pMob->progtypes, MPROG_ATTACK))
		{
			sprintf(buf, " [attack_prog] %s\n\r", pMob->mprogs->attack_name);
			send_to_char(buf, ch);
		}

		if (IS_SET(pMob->progtypes, MPROG_AGGRESS))
		{
			sprintf(buf, " [aggress_prog] %s\n\r", pMob->mprogs->aggress_name);
			send_to_char(buf, ch);
		}

		if (IS_SET(pMob->progtypes, MPROG_MOVE))
		{
			sprintf(buf, " [move_prog] %s\n\r", pMob->mprogs->move_name);
			send_to_char(buf, ch);
		}
	}

	return false;
}

bool medit_create(CHAR_DATA *ch, char *argument)
{
	MOB_INDEX_DATA *pMob;
	AREA_DATA *pArea;
	int value;
	int iHash;

	value = atoi(argument);

	if (argument[0] == '\0' || value == 0)
	{
		send_to_char("Syntax:  medit create [vnum]\n\r", ch);
		return false;
	}

	pArea = get_vnum_area(value);

	if (!pArea)
	{
		send_to_char("MEdit:  That vnum is not assigned an area.\n\r", ch);
		return false;
	}

	if (!IS_BUILDER(ch, pArea))
	{
		send_to_char("MEdit:  Vnum in an area you cannot build in.\n\r", ch);
		return false;
	}

	if (get_mob_index(value))
	{
		send_to_char("MEdit:  Mobile vnum already exists.\n\r", ch);
		return false;
	}

	pMob = new_mob_index();
	pMob->vnum = value;
	pMob->area = pArea;

	if (value > top_vnum_mob)
		top_vnum_mob = value;

	SET_BIT(pMob->act, ACT_IS_NPC);

	iHash = value % MAX_KEY_HASH;

	pMob->next = mob_index_hash[iHash];
	mob_index_hash[iHash] = pMob;
	ch->desc->pEdit = (void *)pMob;

	send_to_char("Mobile Created.\n\r", ch);
	return true;
}

/*
bool medit_spec(CHAR_DATA *ch, char *argument)
{
	MOB_INDEX_DATA *pMob;

	EDIT_MOB(ch, pMob);

	if ( argument[0] == '\0' )
	{
		send_to_char( "Syntax:  spec [special function]\n\r", ch );
		return false;
	}

	if ( !str_cmp( argument, "none" ) )
	{
		pMob->spec_fun = nullptr;

		send_to_char( "Spec removed.\n\r", ch);
		return true;
	}

	if ( spec_lookup( argument ) )
	{
		pMob->spec_fun = spec_lookup( argument );
		send_to_char( "Spec set.\n\r", ch);
		return true;
	}

	send_to_char( "MEdit: No such special function.\n\r", ch );
	return false;
}
*/
bool medit_damtype(CHAR_DATA *ch, char *argument)
{
	MOB_INDEX_DATA *pMob;
	int value = NO_FLAG;

	EDIT_MOB(ch, pMob);

	if (argument[0] == '\0')
	{
		send_to_char("Syntax:  damtype [damage message]\n\r", ch);
		send_to_char("Para ver una lista de tipos de mensajes, pon '? weapon'.\n\r", ch);
		return false;
	}

	value = flag_value(weapon_flags, argument);

	if (value != NO_FLAG)
	{
		pMob->dam_type = value;
		send_to_char("Damage type set.\n\r", ch);
		return true;
	}

	send_to_char("MEdit: No such damage type.\n\r", ch);
	return false;
}

bool medit_align(CHAR_DATA *ch, char *argument)
{
	MOB_INDEX_DATA *pMob;

	EDIT_MOB(ch, pMob);

	if (argument[0] == '\0' || !is_number(argument))
	{
		send_to_char("Syntax:  alignment [number]\n\r", ch);
		return false;
	}

	pMob->alignment = atoi(argument);

	send_to_char("Alignment set.\n\r", ch);
	return true;
}

bool medit_level(CHAR_DATA *ch, char *argument)
{
	MOB_INDEX_DATA *pMob;

	EDIT_MOB(ch, pMob);

	if (argument[0] == '\0' || !is_number(argument))
	{
		send_to_char("Syntax:  level [number]\n\r", ch);
		return false;
	}

	pMob->level = atoi(argument);

	send_to_char("Level set.\n\r", ch);

	pMob->hit[DICE_NUMBER] = pMob->level;
	pMob->hit[DICE_TYPE] = 8;
	pMob->hit[DICE_BONUS] =
		(int)pow((double)((double)pMob->level / (double)5), (double)((double)pMob->level / (double)20)) +
		(int)(pow(pMob->level, 2) * 3 / 2);

	if (pMob->level > 51)
		pMob->hit[DICE_BONUS] += (int)pow(pMob->level - 46, 3) + pMob->level * 2;

	send_to_char("Auto-hitdice Set.\n\r", ch);
	return true;
}

bool medit_desc(CHAR_DATA *ch, char *argument)
{
	MOB_INDEX_DATA *pMob;

	EDIT_MOB(ch, pMob);

	if (argument[0] == '\0')
	{
		string_append(ch, &pMob->description);
		return true;
	}

	send_to_char("Syntax:  desc    - line edit\n\r", ch);
	return false;
}

bool medit_long(CHAR_DATA *ch, char *argument)
{
	MOB_INDEX_DATA *pMob;

	EDIT_MOB(ch, pMob);

	if (argument[0] == '\0')
	{
		send_to_char("Syntax:  long [string]\n\r", ch);
		return false;
	}

	free_pstring(pMob->long_descr);
	strcat(argument, "\n\r");
	pMob->long_descr = palloc_string(argument);
	pMob->long_descr[0] = UPPER(pMob->long_descr[0]);

	send_to_char("Long description set.\n\r", ch);
	return true;
}

bool medit_short(CHAR_DATA *ch, char *argument)
{
	MOB_INDEX_DATA *pMob;

	EDIT_MOB(ch, pMob);

	if (argument[0] == '\0')
	{
		send_to_char("Syntax:  short [string]\n\r", ch);
		return false;
	}

	free_pstring(pMob->short_descr);
	pMob->short_descr = palloc_string(argument);

	send_to_char("Short description set.\n\r", ch);
	return true;
}

bool medit_name(CHAR_DATA *ch, char *argument)
{
	MOB_INDEX_DATA *pMob;

	EDIT_MOB(ch, pMob);

	if (argument[0] == '\0')
	{
		send_to_char("Syntax:  name [string]\n\r", ch);
		return false;
	}

	free_pstring(pMob->player_name);
	pMob->player_name = palloc_string(argument);

	send_to_char("Name set.\n\r", ch);
	return true;
}

bool medit_cabal(CHAR_DATA *ch, char *argument)
{
	MOB_INDEX_DATA *pMob;
	int cabal;

	EDIT_MOB(ch, pMob);

	if (argument[0] == '\0')
	{
		send_to_char("Syntax:  cabal (cabal name)\n\r", ch);
		return false;
	}

	cabal = cabal_lookup(argument);

	if (cabal == 0)
	{
		send_to_char("That is not a valid cabal.\n\r", ch);
		return false;
	}

	pMob->cabal = cabal;
	send_to_char("Cabal set.\n\r", ch);
	return true;
}

bool medit_shop(CHAR_DATA *ch, char *argument)
{
	MOB_INDEX_DATA *pMob;
	char arg1[MAX_INPUT_LENGTH], arg2[MSL];
	int direction;
	SHOP_DATA *pShop = nullptr;

	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);

	EDIT_MOB(ch, pMob);

	if (arg1[0] == '\0')
	{
		send_to_char("Syntax:  shop [open_hour] [close_hour] [direction]\n\r", ch);
		send_to_char("         shop delete\n\r", ch);
		return false;
	}

	if (!str_cmp(arg1, "delete"))
	{
		free_shop(pMob->pShop);
		pMob->pShop = nullptr;
		send_to_char("Shop deleted.\n\r", ch);
		return false;
	}

	if (arg1[0] == '\0' || !is_number(arg1) || arg2[0] == '\0' || !is_number(arg2) || argument[0] == '\0')
	{
		medit_shop(ch, "");
		return false;
	}

	direction = flag_lookup(argument, direction_table);

	if (direction == NO_FLAG)
	{
		send_to_char("That was not a valid direction.\n\r", ch);
		return false;
	}

	if (!pMob->pShop)
	{
		pShop = new SHOP_DATA;
		CLEAR_MEM(pShop, sizeof(SHOP_DATA))
		pMob->pShop = pShop;
		pShop->pMobIndex = pMob;
	}

	pMob->pShop->open_hour = atoi(arg1);
	pMob->pShop->close_hour = atoi(arg2);
	pMob->pShop->direction = direction;

	send_to_char("Shop set.\n\r", ch);
	return true;
}

/* ROM medit functions: */

bool medit_sex(CHAR_DATA *ch, char *argument) /* Moved out of medit() due to naming conflicts -- Hugin */
{
	MOB_INDEX_DATA *pMob;
	int value;

	if (argument[0] != '\0')
	{
		EDIT_MOB(ch, pMob);

		value = flag_value(sex_flags, argument);

		if (value != NO_FLAG)
		{
			pMob->sex = value;

			send_to_char("Sex set.\n\r", ch);
			return true;
		}
	}

	send_to_char("Syntax: sex [sex]\n\r"\
				 "Type '? sex' for a list of flags.\n\r", ch);
	return false;
}

bool medit_act(CHAR_DATA *ch, char *argument) /* Moved out of medit() due to naming conflicts -- Hugin */
{
	MOB_INDEX_DATA *pMob;
	int value;

	if (argument[0] != '\0')
	{
		EDIT_MOB(ch, pMob);

		value = flag_value(act_flags, argument);

		if (value != NO_FLAG)
		{
			TOGGLE_BIT(pMob->act, value);
			SET_BIT(pMob->act, ACT_IS_NPC);

			send_to_char("Act flag toggled.\n\r", ch);
			return true;
		}
	}

	send_to_char("Syntax: act [flag]\n\r"\
				 "Type '? act' for a list of flags.\n\r", ch);
	return false;
}

bool medit_affect(CHAR_DATA *ch, char *argument) /* Moved out of medit() due to naming conflicts -- Hugin */
{
	MOB_INDEX_DATA *pMob;
	int value;

	if (argument[0] != '\0')
	{
		EDIT_MOB(ch, pMob);

		value = flag_value(affect_flags, argument);
		if ((value) != NO_FLAG)
		{
			TOGGLE_BIT(pMob->affected_by, value);

			send_to_char("Affect flag toggled.\n\r", ch);
			return true;
		}
	}

	send_to_char("Syntax: affect [flag]\n\r"\
				 "Type '? affect' for a list of flags.\n\r", ch);
	return false;
}

bool medit_ac(CHAR_DATA *ch, char *argument)
{
	MOB_INDEX_DATA *pMob;
	char arg[MAX_INPUT_LENGTH];
	int pierce, bash, slash, exotic;

	do /* So that I can use break and send the syntax in one place */
	{
		if (argument[0] == '\0')
			break;

		EDIT_MOB(ch, pMob);
		argument = one_argument(argument, arg);

		if (!is_number(arg))
			break;

		pierce = atoi(arg);
		argument = one_argument(argument, arg);

		if (arg[0] != '\0')
		{
			if (!is_number(arg))
				break;

			bash = atoi(arg);
			argument = one_argument(argument, arg);
		}
		else
		{
			bash = pMob->ac[AC_BASH];
		}

		if (arg[0] != '\0')
		{
			if (!is_number(arg))
				break;

			slash = atoi(arg);
			argument = one_argument(argument, arg);
		}
		else
		{
			slash = pMob->ac[AC_SLASH];
		}

		if (arg[0] != '\0')
		{
			if (!is_number(arg))
				break;

			exotic = atoi(arg);
		}
		else
		{
			exotic = pMob->ac[AC_EXOTIC];
		}

		pMob->ac[AC_PIERCE] = pierce;
		pMob->ac[AC_BASH] = bash;
		pMob->ac[AC_SLASH] = slash;
		pMob->ac[AC_EXOTIC] = exotic;

		send_to_char("Ac set.\n\r", ch);
		return true;
	} while (false); /* Just do it once.. */

	send_to_char("Syntax:  ac [ac-pierce [ac-bash [ac-slash [ac-exotic]]]]\n\r"\
				 "help MOB_AC  gives a list of reasonable ac-values.\n\r", ch);
	return false;
}

bool medit_form(CHAR_DATA *ch, char *argument)
{
	MOB_INDEX_DATA *pMob;
	int value;

	if (argument[0] != '\0')
	{
		EDIT_MOB(ch, pMob);

		value = flag_value(form_flags, argument);

		if (value != NO_FLAG)
		{
			TOGGLE_BIT(pMob->form, value);
			send_to_char("Form toggled.\n\r", ch);
			return true;
		}
	}

	send_to_char("Syntax: form [flags]\n\r"\
				 "Type '? form' for a list of flags.\n\r",
				 ch);
	return false;
}

bool medit_part(CHAR_DATA *ch, char *argument)
{
	MOB_INDEX_DATA *pMob;
	int value;

	if (argument[0] != '\0')
	{
		EDIT_MOB(ch, pMob);

		value = flag_value(part_flags, argument);

		if (value != NO_FLAG)
		{
			TOGGLE_BIT(pMob->parts, value);
			send_to_char("Parts toggled.\n\r", ch);
			return true;
		}
	}

	send_to_char("Syntax: part [flags]\n\r"\
				 "Type '? part' for a list of flags.\n\r",
				 ch);
	return false;
}

bool medit_imm(CHAR_DATA *ch, char *argument)
{
	MOB_INDEX_DATA *pMob;
	int value;

	if (argument[0] != '\0')
	{
		EDIT_MOB(ch, pMob);

		value = flag_value(imm_flags, argument);

		if (value != NO_FLAG)
		{
			TOGGLE_BIT(pMob->imm_flags, value);
			send_to_char("Immunity toggled.\n\r", ch);
			return true;
		}
	}

	send_to_char("Syntax: imm [flags]\n\r"\
				 "Type '? imm' for a list of flags.\n\r",
				 ch);
	return false;
}

bool medit_res(CHAR_DATA *ch, char *argument)
{
	MOB_INDEX_DATA *pMob;
	int value;

	if (argument[0] != '\0')
	{
		EDIT_MOB(ch, pMob);

		value = flag_value(res_flags, argument);

		if (value != NO_FLAG)
		{
			TOGGLE_BIT(pMob->res_flags, value);
			send_to_char("Resistance toggled.\n\r", ch);
			return true;
		}
	}

	send_to_char("Syntax: res [flags]\n\r"\
				 "Type '? res' for a list of flags.\n\r",
				 ch);
	return false;
}

bool medit_vuln(CHAR_DATA *ch, char *argument)
{
	MOB_INDEX_DATA *pMob;
	int value;

	if (argument[0] != '\0')
	{
		EDIT_MOB(ch, pMob);

		value = flag_value(vuln_flags, argument);

		if (value != NO_FLAG)
		{
			TOGGLE_BIT(pMob->vuln_flags, value);
			send_to_char("Vulnerability toggled.\n\r", ch);
			return true;
		}
	}

	send_to_char("Syntax: vuln [flags]\n\r"\
				 "Type '? vuln' for a list of flags.\n\r",
				 ch);
	return false;
}

bool medit_material(CHAR_DATA *ch, char *argument)
{
	return false;
}

/*
bool medit_material(CHAR_DATA *ch, char *argument)
{
	MOB_INDEX_DATA *pMob;
	int value;

	EDIT_MOB(ch, pMob);

	if ((value = flag_value( material_type, argument)) != NO_FLAG)
	{
		pMob->material = material_name(value);
		send_to_char( "Material type set.\n\r", ch);
		return true;
	}

	send_to_char("Unknown material type, '? material' for a list.\n\r", ch);
	return false;

}
*/

bool medit_off(CHAR_DATA *ch, char *argument)
{
	MOB_INDEX_DATA *pMob;
	int value;

	if (argument[0] != '\0')
	{
		EDIT_MOB(ch, pMob);

		value = flag_value(off_flags, argument);

		if (value != NO_FLAG)
		{
			TOGGLE_BIT(pMob->off_flags, value);
			send_to_char("Offensive behavior toggled.\n\r", ch);
			return true;
		}
	}

	send_to_char("Syntax: off [flags]\n\r"\
				 "Type '? off' for a list of flags.\n\r",
				 ch);
	return false;
}

bool medit_size(CHAR_DATA *ch, char *argument)
{
	MOB_INDEX_DATA *pMob;
	int value;

	if (argument[0] != '\0')
	{
		EDIT_MOB(ch, pMob);

		value = flag_value(size_flags, argument);

		if (value != NO_FLAG)
		{
			pMob->size = value;
			send_to_char("Size set.\n\r", ch);
			return true;
		}
	}

	send_to_char("Syntax: size [size]\n\r"\
				 "Type '? size' for a list of sizes.\n\r",
				 ch);
	return false;
}

bool medit_hitdice(CHAR_DATA *ch, char *argument)
{
	static char syntax[] = "Syntax:  hitdice <number> d <type> + <bonus>\n\r";
	char *num, *type, *bonus, *cp;
	MOB_INDEX_DATA *pMob;

	EDIT_MOB(ch, pMob);

	if (argument[0] == '\0')
	{
		send_to_char(syntax, ch);
		return false;
	}

	num = cp = argument;

	while (isdigit(*cp))
	{
		++cp;
	}

	while (*cp != '\0' && !isdigit(*cp))
	{
		*(cp++) = '\0';
	}

	type = cp;

	while (isdigit(*cp))
	{
		++cp;
	}

	while (*cp != '\0' && !isdigit(*cp))
	{
		*(cp++) = '\0';
	}

	bonus = cp;

	while (isdigit(*cp))
	{
		++cp;
	}

	if (*cp != '\0')
		*cp = '\0';

	if ((!is_number(num) || atoi(num) < 1)
		|| (!is_number(type) || atoi(type) < 1)
		|| (!is_number(bonus) || atoi(bonus) < 0))
	{
		send_to_char(syntax, ch);
		return false;
	}

	pMob->hit[DICE_NUMBER] = atoi(num);
	pMob->hit[DICE_TYPE] = atoi(type);
	pMob->hit[DICE_BONUS] = atoi(bonus);

	send_to_char("Hitdice set.\n\r", ch);
	return true;
}

bool medit_manadice(CHAR_DATA *ch, char *argument)
{
	static char syntax[] = "Syntax:  manadice <number> d <type> + <bonus>\n\r";
	char *num, *type, *bonus, *cp;
	MOB_INDEX_DATA *pMob;

	EDIT_MOB(ch, pMob);

	if (argument[0] == '\0')
	{
		send_to_char(syntax, ch);
		return false;
	}

	num = cp = argument;

	while (isdigit(*cp))
	{
		++cp;
	}

	while (*cp != '\0' && !isdigit(*cp))
	{
		*(cp++) = '\0';
	}

	type = cp;

	while (isdigit(*cp))
	{
		++cp;
	}

	while (*cp != '\0' && !isdigit(*cp))
	{
		*(cp++) = '\0';
	}

	bonus = cp;

	while (isdigit(*cp))
	{
		++cp;
	}

	if (*cp != '\0')
		*cp = '\0';

	if (!(is_number(num) && is_number(type) && is_number(bonus)))
	{
		send_to_char(syntax, ch);
		return false;
	}

	if ((!is_number(num) || atoi(num) < 1)
		|| (!is_number(type) || atoi(type) < 1)
		|| (!is_number(bonus) || atoi(bonus) < 0))
	{
		send_to_char(syntax, ch);
		return false;
	}

	pMob->mana[DICE_NUMBER] = atoi(num);
	pMob->mana[DICE_TYPE] = atoi(type);
	pMob->mana[DICE_BONUS] = atoi(bonus);

	send_to_char("Manadice set.\n\r", ch);
	return true;
}

bool medit_damdice(CHAR_DATA *ch, char *argument)
{
	static char syntax[] = "Syntax:  damdice <number> d <type> + <bonus>\n\r";
	char *num, *type, *bonus, *cp;
	MOB_INDEX_DATA *pMob;

	EDIT_MOB(ch, pMob);

	if (argument[0] == '\0')
	{
		send_to_char(syntax, ch);
		return false;
	}

	num = cp = argument;

	while (isdigit(*cp))
	{
		++cp;
	}

	while (*cp != '\0' && !isdigit(*cp))
	{
		*(cp++) = '\0';
	}

	type = cp;

	while (isdigit(*cp))
	{
		++cp;
	}

	while (*cp != '\0' && !isdigit(*cp))
	{
		*(cp++) = '\0';
	}

	bonus = cp;

	while (isdigit(*cp))
	{
		++cp;
	}

	if (*cp != '\0')
		*cp = '\0';

	if (!(is_number(num) && is_number(type) && is_number(bonus)))
	{
		send_to_char(syntax, ch);
		return false;
	}

	if ((!is_number(num) || atoi(num) < 1)
		|| (!is_number(type) || atoi(type) < 1)
		|| (!is_number(bonus) || atoi(bonus) < 0))
	{
		send_to_char(syntax, ch);
		return false;
	}

	pMob->damage[DICE_NUMBER] = atoi(num);
	pMob->damage[DICE_TYPE] = atoi(type);
	pMob->damage[DICE_BONUS] = atoi(bonus);

	send_to_char("Damdice set.\n\r", ch);
	return true;
}

bool medit_race(CHAR_DATA *ch, char *argument)
{
	MOB_INDEX_DATA *pMob;
	int race;

	if (argument[0] != '\0' && (race = race_lookup(argument)) != -1)
	{
		EDIT_MOB(ch, pMob);

		pMob->race = race;
		BITWISE_OR(pMob->off_flags, race_data_lookup(race)->off);
		BITWISE_OR(pMob->imm_flags, race_data_lookup(race)->imm);
		BITWISE_OR(pMob->res_flags, race_data_lookup(race)->res);
		BITWISE_OR(pMob->vuln_flags, race_data_lookup(race)->vuln);
		BITWISE_OR(pMob->form, race_data_lookup(race)->form);
		BITWISE_OR(pMob->parts, race_data_lookup(race)->parts);

		send_to_char("Race set.\n\r", ch);
		return true;
	}

	if (argument[0] == '?')
	{
		char buf[MAX_STRING_LENGTH];

		send_to_char("Available races are:", ch);

		for (race = 0; race_table[race].name != nullptr; race++)
		{
			if (race % 3 == 0)
				send_to_char("\n\r", ch);

			sprintf(buf, " %-15s", race_table[race].name);
			send_to_char(buf, ch);
		}

		send_to_char("\n\r", ch);
		return false;
	}

	send_to_char("Syntax:  race [race]\n\r"\
				 "Type 'race ?' for a list of races.\n\r", ch);
	return false;
}

bool medit_position(CHAR_DATA *ch, char *argument)
{
	MOB_INDEX_DATA *pMob;
	char arg[MAX_INPUT_LENGTH];
	int value;

	argument = one_argument(argument, arg);

	value = position_lookup(arg);
	if (value == -1)
		return false;

	EDIT_MOB(ch, pMob);

	pMob->start_pos = value;
	send_to_char("Start position set.\n\r", ch);
	return true;
}

bool medit_gold(CHAR_DATA *ch, char *argument)
{
	MOB_INDEX_DATA *pMob;
	int index;

	EDIT_MOB(ch, pMob);

	if (argument[0] == '\0')
	{
		send_to_char("Syntax:  wealth [none/poor/moderate/affluent/rich/peerless]\n\r", ch);
		return false;
	}

	index = flag_lookup(argument, wealth_table);

	if (index == NO_FLAG)
	{
		medit_gold(ch, "");
		return false;
	}

	pMob->wealth = index;
	send_to_char("Wealth set.\n\r", ch);
	return true;
}

bool medit_hitroll(CHAR_DATA *ch, char *argument)
{
	MOB_INDEX_DATA *pMob;

	EDIT_MOB(ch, pMob);

	if (argument[0] == '\0' || !is_number(argument))
	{
		send_to_char("Syntax:  hitroll [number]\n\r", ch);
		return false;
	}

	pMob->hitroll = atoi(argument);
	send_to_char("Hitroll set.\n\r", ch);
	return true;
}

bool oedit_liqlist(CHAR_DATA *ch, char *argument)
{
	BUFFER *buffer;
	char buf[MAX_STRING_LENGTH];

	buffer = new_buf();

	for (int liq = 0; liq_table[liq].liq_name != nullptr; liq++)
	{
		if (liq % 21 == 0)
			add_buf(buffer, "Name                 Color          Proof Full Thirst Food Ssize\n\r");

		sprintf(buf, "%-20s %-14s %5d %4d %6d %4d %5d\n\r",
			liq_table[liq].liq_name,
			liq_table[liq].liq_color,
			liq_table[liq].liq_affect[0],
			liq_table[liq].liq_affect[1],
			liq_table[liq].liq_affect[2],
			liq_table[liq].liq_affect[3],
			liq_table[liq].liq_affect[4]);
		add_buf(buffer, buf);
	}

	page_to_char(buf_string(buffer), ch);
	free_buf(buffer);

	return true;
}
