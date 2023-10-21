/* IMPROGS Expansion: AREA PROGS (Eladrian) */

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "aprog.h"
#include "weather_enums.h"
#include "comm.h"
#include "magic.h"
#include "interp.h"
#include "recycle.h"
#include "tables.h"
#include "dioextra.h"
#include "db.h"
#include "devextra.h"
#include "utility.h"
#include "fight.h"
#include "act_info.h"
#include "newmem.h"

const struct improg_type aprog_table[] = 
{
	{ "reset_prog", "reset_prog_cimsewer", (void*)reset_prog_cimsewer, "Sewers of Cimar"},
	{ "pulse_prog", "pulse_prog_ruins_shark", (void*)pulse_prog_ruins_shark, "Ruins of the Deep"},
	{ "sun_prog", "sun_prog_caelumaedan", (void*)sun_prog_caelumaedan, "None" },
	{ "sun_prog", "sun_prog_glasstower", (void*)sun_prog_glasstower, "None" },
	{ "sun_prog", "sun_prog_ilopheth", (void*)sun_prog_ilopheth, "Valley of Ilopheth"},
	{ "tick_prog", "tick_prog_academy_reset", (void *)tick_prog_academy_reset, "None" },
	{ "tick_prog", "tick_prog_ilopheth", (void*)tick_prog_ilopheth, "Valley of Ilopheth"},
	{ "myell_prog", "myell_prog_lawful_city", (void*)myell_prog_lawful_city, "Cimar"},
	{ nullptr, nullptr, nullptr, nullptr},
};

bool aprog_unset(AREA_DATA *area, const char *progtype, const char *name)
{
	int i;

	for (i = 0; aprog_table[i].name; i++)
	{
		if (!str_cmp(aprog_table[i].name, name))
			break;
	}

	if (!aprog_table[i].name)
		return false;

	if (!str_cmp(progtype, "pulse_prog"))
	{
		area->aprogs->pulse_prog = nullptr;
		free_pstring(area->aprogs->pulse_name);
		REMOVE_BIT(area->progtypes, APROG_PULSE);
	}
	else if (!str_cmp(progtype, "reset_prog"))
	{
		area->aprogs->reset_prog = nullptr;
		free_pstring(area->aprogs->reset_name);
		REMOVE_BIT(area->progtypes, APROG_RESET);
	}
	else if (!str_cmp(progtype, "sun_prog"))
	{
		area->aprogs->sun_prog = nullptr;
		free_pstring(area->aprogs->sun_name);
		REMOVE_BIT(area->progtypes, APROG_SUN);
	}
	else if (!str_cmp(progtype, "tick_prog"))
	{
		area->aprogs->tick_prog = nullptr;
		free_pstring(area->aprogs->tick_name);
		REMOVE_BIT(area->progtypes, APROG_TICK);
	}
	else if (!str_cmp(progtype, "aggress_prog"))
	{
		area->aprogs->aggress_prog = nullptr;
		free_pstring(area->aprogs->aggress_name);
		REMOVE_BIT(area->progtypes, APROG_AGGRESS);
	}
	else if (!str_cmp(progtype, "myell_prog"))
	{
		area->aprogs->myell_prog = nullptr;
		free_pstring(area->aprogs->myell_name);
		REMOVE_BIT(area->progtypes, APROG_MYELL);
	}

	return true;
}

void aprog_set(AREA_DATA *area, const char *progtype, const char *name)
{
	int i;
	char buf[MSL];

	for (i = 0; aprog_table[i].name; i++)
	{
		if (!str_cmp(aprog_table[i].name, name))
			break;
	}

	if (!aprog_table[i].name)
	{
		sprintf(buf, "Load_improgs: 'A': Function not found for area %s.", area->name);
		bug(buf, 0);
		return;
	}

	// IF YOU ADD A NEW TYPE ALSO PUT IT IN DB.C DO_ADUMP!

	if (!str_cmp(progtype, "pulse_prog"))
	{
		area->aprogs->pulse_prog = (APROG_FUN_PULSE *)aprog_table[i].function;
		free_pstring(area->aprogs->pulse_name);
		area->aprogs->pulse_name = palloc_string(name);
		SET_BIT(area->progtypes, APROG_PULSE);
		return;
	}

	if (!str_cmp(progtype, "reset_prog"))
	{
		area->aprogs->reset_prog = (APROG_FUN_RESET *)aprog_table[i].function;
		free_pstring(area->aprogs->reset_name);
		area->aprogs->reset_name = palloc_string(name);
		SET_BIT(area->progtypes, APROG_RESET);
		return;
	}

	if (!str_cmp(progtype, "sun_prog"))
	{
		area->aprogs->sun_prog = (APROG_FUN_SUN *)aprog_table[i].function;
		free_pstring(area->aprogs->sun_name);
		area->aprogs->sun_name = palloc_string(name);
		SET_BIT(area->progtypes, APROG_SUN);
		return;
	}

	if (!str_cmp(progtype, "tick_prog"))
	{
		area->aprogs->tick_prog = (APROG_FUN_TICK *)aprog_table[i].function;

		free_pstring(area->aprogs->tick_name);
		area->aprogs->tick_name = palloc_string(name);
		SET_BIT(area->progtypes, APROG_TICK);
		return;
	}

	if (!str_cmp(progtype, "aggress_prog"))
	{
		area->aprogs->aggress_prog = (APROG_FUN_AGGRESS *)aprog_table[i].function;
		free_pstring(area->aprogs->aggress_name);
		area->aprogs->aggress_name = palloc_string(name);
		SET_BIT(area->progtypes, APROG_AGGRESS);
		return;
	}

	if (!str_cmp(progtype, "myell_prog"))
	{
		area->aprogs->myell_prog = (APROG_FUN_MYELL *)aprog_table[i].function;
		free_pstring(area->aprogs->myell_name);
		area->aprogs->myell_name = palloc_string(name);
		SET_BIT(area->progtypes, APROG_MYELL);
		return;
	}
}

void tick_prog_academy_reset(AREA_DATA *area)
{
	CHAR_DATA *ch;

	for (ch = char_list; ch; ch = ch->next)
	{
		if (!is_npc(ch) && ch->in_room->area == area && ch->level > 10 && !ch->fighting && !is_immortal(ch))
		{
			char buf[MSL];

			send_to_char("A small contingent of guards rapidly marches up to you.\n\r", ch);
			act("A small contingent of guards rapidly marches up to $n, marching $m off.", ch, 0, 0, TO_ROOM);

			sprintf(buf, "A Royal Academy Guard says '%sBy the orders of the King, you have completed your training and must depart.%s'\n\r",
				get_char_color(ch, "speech"),
				END_COLOR(ch));
			send_to_char(buf, ch);
			send_to_char("The guards lead you to the exit of the Academy.\n\r", ch);

			do_look(ch, "auto");
			sprintf(buf, "A Royal Academy Guard says '%sThe King allows a small reward for the completion of your training here.%s'\n\r",
				get_char_color(ch, "speech"),
				END_COLOR(ch));
			send_to_char(buf, ch);
			send_to_char("A Royal Academy Guard gives you 25 gold.\n\r", ch);

			ch->gold += 25;

			char_from_room(ch);
			char_to_room(ch, get_room_index(ROOM_VNUM_TEMPLE));

			sprintf(buf, "A Royal Academy Guard says '%sA tip from me: if you're looking to learn further, I might try %s.%s'\n\r",
				get_char_color(ch, "speech"),
				is_good(ch)
					? "the parrots in the forest off of the road south of Cimar or the trolls in the forests off the Great Shalaran Road"
					: "the parrots in the forest off of the road south of Cimar or the pixies in the Emerald Forest off the Melcene Plains Road",
				END_COLOR(ch));
			send_to_char(buf, ch);
		}
	}

	if (area->age > 2 && area->age < 6)
	{
		if (number_percent() > 50)
		{
			reset_area(area);
			return;
		}
	}

	if (area->age >= 6)
		reset_area(area);

	/*
	for ( ch->level = ch->level ; ch->level < 25; ch->level++ )
	{
		advance_level( ch,false);
	}
	ch->exp = exp_per_level(ch) * (ch->level - 1);
	*/
}

void tick_prog_ilopheth(AREA_DATA *area)
{
	OBJ_DATA *pedestal = nullptr, *sphere = nullptr, *old_portal = nullptr, *new_portal = nullptr, *obj;
	ROOM_INDEX_DATA *pedroom = get_room_index(9096);
	ROOM_INDEX_DATA *portroom = get_room_index(9114);
	char buf[MSL];
	bool sph_found = false;

	if (time_info.day != 0 && time_info.day != 14)
		return;

	if ((time_info.hour != 15 && time_info.hour != 17) || time_info.half)
		return;

	if (area->sky >= WeatherCondition::Overcast)
		return;

	for (obj = pedroom->contents; obj; obj = obj->next_content)
	{
		if (obj->pIndexData->vnum == 9015)
		{
			pedestal = obj;
			break;
		}
	}

	if (!pedestal)
		return;

	for (obj = pedestal->contains; obj; obj = obj->next_content)
	{
		if (obj->pIndexData->vnum == 9016)
		{
			sphere = obj;
			sph_found = true;
			break;
		}
	}

	if (time_info.hour == 15)
	{
		if (!sph_found)
		{
			if (pedroom->people)
			{
				sprintf(buf, "%sAs the afternoon sun drops lower in the sky a single shaft of sunlight shines through a gap in the stones, coming to rest directly on the white marble pedestal.%s",
					get_char_color(pedroom->people, "yellow"),
					END_COLOR(pedroom->people));
				act(buf, pedroom->people, 0, 0, TO_ALL);
			}
		}
		else
		{
			if (pedroom->people)
			{
				sprintf(buf, "%sAs the afternoon sun drops lower in the sky a single shaft of sunlight shines through a gap in the stones, coming to rest directly on the white marble pedestal.%s",
					get_char_color(pedroom->people, "yellow"),
					END_COLOR(pedroom->people));
				act(buf, pedroom->people, 0, 0, TO_ALL);

				RS.Queue.AddToQueue(4, 5, act, "The opacity of the crystal sphere appears to fade, and it begins to glow a brilliant white!", pedroom->people, 0, 0, TO_ALL);
				RS.Queue.AddToQueue(7, 5, act, "To the northwest, an immense pillar of light ascends from the forest into the heavens!", pedroom->people, 0, 0, TO_ALL);
			}

			for (obj = portroom->contents; obj; obj = obj->next_content)
			{
				if (obj->pIndexData->vnum == 9009)
				{
					old_portal = obj;
					break;
				}
			}

			if (!old_portal)
			{
				return;
			}
			else
			{
				if (portroom->people)
				{
					sprintf(buf, "The mass of energy begins to glow a brilliant white, and releases an immense pillar of energy which streaks into the heavens.  The disturbance seems to stabilize into an arched, opalescent portal.");
					act(buf, portroom->people, 0, 0, TO_ALL);
				}

				obj_from_room(old_portal);
				extract_obj(old_portal);

				new_portal = create_object(get_obj_index(9008), 0);

				obj_to_room(new_portal, portroom);
			}
		}
	}

	if (time_info.hour == 17)
	{
		if (pedroom->people)
			act("As the sun descends toward the horizon, the shaft of light illuminating the pedestal fades to a spot, and disappears.", pedroom->people, 0, 0, TO_ALL);

		for (obj = portroom->contents; obj; obj = obj->next_content)
		{
			if (obj->pIndexData->vnum == 9008)
			{
				new_portal = obj;
				break;
			}
		}

		if (!new_portal)
		{
			return;
		}
		else
		{
			if (portroom->people)
				act("The opalescent gateway begins to break down, its edges flexing and twisting wildly.", portroom->people, 0, 0, TO_ALL);

			obj_from_room(new_portal);
			extract_obj(new_portal);

			old_portal = create_object(get_obj_index(9009), 0);

			obj_to_room(old_portal, portroom);
		}
	}
}

void reset_prog_cimsewer(AREA_DATA *area)
{
	zone_echo(area, "The stench of death fills the air.");
}

void pulse_prog_ruins_shark(AREA_DATA *area)
{
	DESCRIPTOR_DATA *d;
	CHAR_DATA *shark, *ch = nullptr;
	int count = 0;

	if (area->nplayer == 0)
		return;

	for (shark = char_list; shark; shark = shark->next)
	{
		if (!is_npc(shark))
			continue;

		if (shark->pIndexData->vnum == 20111)
		{
			if (!shark->fighting && !shark->last_fought && number_percent() > 50 && shark->in_room)
				extract_char(shark, true);
			else
				count++;
		}
	}

	if (count > 5)
		return;

	for (d = descriptor_list; d; d = d->next)
	{
		if (d->connected == CON_PLAYING
			&& d->character->in_room != nullptr
			&& d->character->in_room->area == area
			&& number_percent() > 90)
		{
			ch = d->character;

			if ((d->character->hit * 2) > d->character->max_hit)
				continue;

			if (d->character->in_room->vnum < 20100
				|| (d->character->in_room->vnum > 20150 && d->character->in_room->vnum < 20181)
				|| d->character->in_room->vnum > 20219)
			{
				continue;
			}

			shark = create_mobile(get_mob_index(20111));

			char_to_room(shark, ch->in_room);
			act("Drawn by the scent of blood in the water, a fierce shark swims in and attacks!", ch, 0, 0, TO_ALL);

			do_murder(shark, ch->name);
			return;
		}
	}
}

void sun_prog_ilopheth(AREA_DATA *area)
{
	switch (sun)
	{
		case SolarPosition::Sunrise:
			RS.Queue.AddToQueue(15, 2, (void *)zone_echo, area,	(char *)"With the beginning of the day sounds of life fill the valley, as the forest comes alive.");
			break;
		case SolarPosition::Sunset:
			RS.Queue.AddToQueue(15, 2, (void *)zone_echo, area, (char *)"The forest grows still, as the night sky settles over the valley.");
			break;
	}
}

void sun_prog_caelumaedan(AREA_DATA *area)
{
	zone_echo(area, "Carantoc 0wns j00.");
}

void sun_prog_glasstower(AREA_DATA *area)
{
	switch (sun)
	{
		case (SolarPosition::Sunrise):
			zone_echo(area, "{RThe magical luminescence of the Tower fades, replaced by the ruddy hue of sunrise.{x");
			break;
		case (SolarPosition::Daylight):
			zone_echo(area, "{YBrilliant rays of light are shattered by the complex structures of glass as the sun illuminates the Tower.{x");
			break;
		case (SolarPosition::Sunset):
			zone_echo(area, "{MAs the sun begins to set, the Tower takes on the color of the dusky sky.{x");
			break;
		case (SolarPosition::Dark):
			zone_echo(area, "{WThe natural light now being insufficient, the Tower itself begins to glow with magical energy.{x");
			break;
	}
}

void myell_prog_lawful_city(AREA_DATA *area, CHAR_DATA *ch, CHAR_DATA *victim)
{
	AFFECT_DATA af;

	if ((is_npc(victim) && !IS_SET(victim->act, ACT_LAW))
		|| is_npc(ch)
		|| is_affected(ch, gsn_aggressor))
	{
		return;
	}

	if (ch->law_pass)
	{
		ch->law_pass = false;
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = gsn_aggressor;
	af.aftype = AFT_INVIS;
	af.level = 60;
	af.duration = 12;
	affect_to_char(ch, &af);
}
