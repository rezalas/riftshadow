#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "merc.h"
#include "interp.h"
#include "tables.h"
#include "recycle.h"
#include "magic.h"
#include "db.h"
#include "lookup.h"
#include "comm.h"
#include "skills.h"
#include "handler.h"
#include "chardef.h"

bool check_silent_movement(CHAR_DATA *ch, ROOM_INDEX_DATA *room)
{
	bool elf=false,drow=false;

	if(!str_cmp(race_table[ch->race].name,"elf"))
		elf=true;
	else if(!str_cmp(race_table[ch->race].name,"drow"))
		drow=true;
	else
		return false;

	if(room->sector_type == SECT_UNDERWATER
	   || room->sector_type == SECT_AIR
	   || room->sector_type == SECT_WATER)
		return false;

	if(elf && !(room->sector_type == SECT_INSIDE)
		   && !(room->sector_type == SECT_CITY)
		   && !(room->sector_type == SECT_CAVE))
		return true;
	else if(drow && ((room->sector_type == SECT_INSIDE)
				 || (room->sector_type == SECT_CITY)
				 || (room->sector_type == SECT_CAVE)))
		return true;
	else
		return false;
}

void do_silent_movement(CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;
	int skill;

	if((skill = get_skill(ch, gsn_silent_movement)) == 0
		|| ((str_cmp(race_table[ch->race].name,"elf"))
		&& (str_cmp(race_table[ch->race].name,"drow"))))
		return send_to_char("Huh?\n\r", ch);

	send_to_char( "You attempt to move silently.\n\r", ch );

	if (is_affected(ch,gsn_silent_movement))                                                                      
	    return;  

	if(!check_silent_movement(ch, ch->in_room))                                                                   
		return;

	if ( number_percent( ) < skill)
	{
		check_improve(ch,gsn_silent_movement,true,3);
		init_affect(&af);
		af.where     = TO_AFFECTS;
		af.aftype	 = AFT_INVIS;
		af.type      = gsn_silent_movement;
		af.level     = ch->level;
		af.duration  = ch->level;
		affect_to_char( ch, &af );
	}
	else
		check_improve(ch,gsn_silent_movement,false,3);
						
	return;
}
