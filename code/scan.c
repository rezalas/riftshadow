#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "scan.h"
#include "handler.h"
#include "act_info.h"
#include "characterClasses/zealot.h"
#include "comm.h"
#include "interp.h"
#include "db.h"
#include "utility.h"
#include "weather_enums.h"
#include "act_move.h"

void do_scan(CHAR_DATA *ch, char *argument)
{
	char arg1[MAX_INPUT_LENGTH], buf[MAX_INPUT_LENGTH];
	ROOM_INDEX_DATA *scan_room;
	OBJ_AFFECT_DATA *af;
	OBJ_DATA *obj;
	EXIT_DATA *pExit;
	short door, depth, i;

	argument = one_argument(argument, arg1);

	if (!str_cmp(arg1, "n") || !str_cmp(arg1, "north"))
	{
		door = 0;
	}
	else if (!str_cmp(arg1, "e") || !str_cmp(arg1, "east"))
	{
		door = 1;
	}
	else if (!str_cmp(arg1, "s") || !str_cmp(arg1, "south"))
	{
		door = 2;
	}
	else if (!str_cmp(arg1, "w") || !str_cmp(arg1, "west"))
	{
		door = 3;
	}
	else if (!str_cmp(arg1, "u") || !str_cmp(arg1, "up"))
	{
		door = 4;
	}
	else if (!str_cmp(arg1, "d") || !str_cmp(arg1, "down"))
	{
		door = 5;
	}
	else if (!str_cmp(arg1, "a") || !str_cmp(arg1, "all"))
	{
		door = -1;
	}
	else
	{
		send_to_char("Which way do you want to scan?\n\r", ch);
		return;
	}

	if (is_affected_by(ch, AFF_BLIND) || (get_bv_stage(ch) > 0))
	{
		send_to_char("You can't see anything, you're blind!\n\r", ch);
		return;
	}

	if (door == -1)
	{
		/* "scan all", aka CheapHackLand */
		do_scan(ch, "north");
		do_scan(ch, "east");
		do_scan(ch, "south");
		do_scan(ch, "west");
		do_scan(ch, "up");
		do_scan(ch, "down");
		return;
	}

	if (is_affected_area(ch->in_room->area, gsn_cyclone))
	{
		send_to_char("You can't see anything but swirling winds and debris!\n\r", ch);
		return;
	}

	act("You scan $T.", ch, nullptr, dir_name[door], TO_CHAR);
	act("$n scans $T.", ch, nullptr, dir_name[door], TO_ROOM);

	if (is_affected_room(ch->in_room, gsn_smokescreen))
	{
		send_to_char("Smoke fills the room and limits your vision!\n\r", ch);
		return;
	}

	sprintf(buf, "All you can see %s is a towering wall of fire and smoke!\n\r", dir_name[door]);

	scan_room = ch->in_room;

	depth = 3;

	if (sun == SolarPosition::Daylight)
		depth++;

	if (sun == SolarPosition::Dark)
		depth--;

	if (ch->in_room->area->sky == WeatherCondition::Drizzle
		|| ch->in_room->area->sky == WeatherCondition::SnowFlurry
		|| ch->in_room->area->sky == WeatherCondition::Hail)
	{
		depth--;
	}

	if (ch->in_room->area->sky == WeatherCondition::Downpour
		|| ch->in_room->area->sky == WeatherCondition::ThunderStorm
		|| ch->in_room->area->sky == WeatherCondition::Blizzard)
	{
		depth -= 2;
	}

	if (ch->in_room->area->wind > 2)
		depth--;

	if (!is_outside(ch))
		depth = 3;

	if ((obj = get_eq_char(ch, WEAR_HOLD)) != nullptr)
	{
		if (is_affected(ch, gsn_farsee) && obj->pIndexData->vnum == OBJ_VNUM_CRYSTAL)
		{
			af = affect_find_obj(obj->affected, gsn_fashion_crystal);

			if (af)
			{
				act("Your vision sharpens as you peer through your multifaceted crystal.", ch, 0, 0, TO_CHAR);
				depth += af->modifier / 100;
			}
		}
	}

	if (depth > 7)
		depth = 7;

	if (depth <= 0)
	{
		send_to_char("Weather conditions hamper your visibilty too much to see that far.\n\r", ch);
		return;
	}

	for (i = 1; i <= depth; i++)
	{
		if ((pExit = scan_room->exit[door]) != nullptr)
		{
			if (!IS_SET(pExit->exit_info, EX_CLOSED) || IS_SET(pExit->exit_info, EX_TRANSLUCENT))
			{
				scan_room = pExit->u1.to_room;

				if (scan_room == nullptr)
					return;

				if (is_affected_room(scan_room, gsn_conflagration))
				{
					send_to_char(buf, ch);
					return;
				}

				if (is_affected_room(scan_room, gsn_smokescreen))
				{
					send_to_char("A wall of smoke blocks your vision!\n\r", ch);
					return;
				}

				if (IS_SET(pExit->exit_info, EX_NONOBVIOUS))
					return;

				scan_list(pExit->u1.to_room, ch, i, door);
				send_to_char("\n\r", ch);
			}
		}
	}
}

void scan_list(ROOM_INDEX_DATA *scan_room, CHAR_DATA *ch, short depth, short door)
{
	CHAR_DATA *rch;
	char *const distance[7] =
	{
		" **** 1 %s **** ",
		" **** 2 %s **** ",
		" **** 3 %s **** ",
		" **** 4 %s **** ",
		" **** 5 %s **** ",
		" **** 6 %s **** ",
		" **** 7 %s **** "
	};

	char buf[MAX_INPUT_LENGTH], buf2[MAX_INPUT_LENGTH];

	buf[0] = '\0';

	if (scan_room == nullptr)
		return;

	sprintf(buf2, distance[(depth - 1)], dir_name[door]);
	strcat(buf, buf2);

	send_to_char(buf, ch);
	send_to_char("\n\r", ch);

	for (rch = scan_room->people; rch != nullptr; rch = rch->next_in_room)
	{
		if (rch == ch)
			continue;

		if (!is_npc(rch) && rch->invis_level > get_trust(ch))
			continue;

		if (can_see(ch, rch))
			show_char_to_char_0(rch, ch);
	}
}
