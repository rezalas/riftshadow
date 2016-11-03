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


/*
void spell_imbue_stone(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	char item[MSL];
	char type[MSL];
	OBJ_DATA *obj;
	AFFECT_DATA af;
	OBJ_AFFECT_DATA oaf;
	int charges = 0;
	obj = get_obj_char(ch, item);

	target_name = one_argument(target_name, item);
	target_name = one_argument(target_name, type);

	if(!str_cmp(item,""))
		return send_to_char("Imbue what?\n\r");
	
	if(!str_cmp(type,"") || (str_cmp(type," berus") && str_cmp(type, "calabren") && str_cmp(type, "sun")))
		return send_to_char("Do you intend on harnessing the energy of the sun, Berus or Calabren?\n\r",ch);
	
	if(is_affected(ch,gsn_imbue_stone))
		return send_to_char("You can't seem to focus your natural energies again just yet.\n\r",ch);

	if(obj->value[0] != 0)
		return send_to_char("This stone has already been attuned to a specific kind of energy.\n\r",ch);	
	
	if(obj->item_type != ITEM_STONE)
		return send_to_char("That's not a stone.\n\r",ch);
	
	if(!str_cmp(type, "berus"))  
		obj->value[0] = 1;
	
	if(!str_cmp(type, "calabren"))
   		obj->value[0] = 2;
	
	if(!str_cmp(type, "sun"))
		obj->value[0] = 3;

	charges = (ch->level * .8);		// number of uses stonesyield, stored in V1
	obj->value[1] = charges;
	
	init_affect_obj(&oaf);
	oaf.where = TO_OBJ_AFFECTS;
	oaf.type = sn;
	oaf.aftype = AFT_COMMUNE;
	oaf.modifier = APPLY_NONE;
	oaf.duration = (336 - ((ch->level -51) * 5));
	oaf.level = ch->level;
	affect_to_obj(obj,&oaf);
	
	init_aff(&af);
	af.where = TO_AFFECTS;
	af.type = sn;
	af.aftype = AFT_TIMER;
	af.modifier = APPLY_NONE;
	af.duration = 96; 
	af.level = ch->level;
	affect_to_char(ch,&af);
		
	return;
}

*/

void spell_tangleroot(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int dam = (int)((float)((float)ch->level * (float).6) + (dice(10, (int)(level/4))));

	act("The ground beneath your feet begins to rumble.",ch->in_room->people,NULL,NULL,TO_ALL);
	
	if(number_percent() > 10)
	{
		RS.Queue.AddToQueue(2, 2, LAG_CHAR,victim,(2*PULSE_VIOLENCE));
		RS.Queue.AddToQueue(2, 8, damage_queue, ch, victim, (int) dam, DAM_BASH, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT,(char *)"entangling roots*");
		RS.Queue.AddToQueue(2, 5, act, "A tangleroot bursts out of the ground and entangles $N!",ch,0,victim,TO_NOTVICT);
		RS.Queue.AddToQueue(2, 5, act, "A tangleroot bursts out of the ground and entangles you!",ch, 0, victim,TO_VICT);
		RS.Queue.AddToQueue(2, 5, act, "A tangleroot bursts out of the ground and entangles $N!",ch,0,victim,TO_CHAR);
		return;
	}
	
	RS.Queue.AddToQueue(2,5,act,"A tangleroot bursts out of the ground, but $N avoids it!",ch,0,victim,TO_NOTVICT);
	RS.Queue.AddToQueue(2,5,act,"A tangleroot bursts out of the ground, but you manage to avoid it!",ch,0,victim,TO_VICT);
	RS.Queue.AddToQueue(2,5,act,"A tangleroot bursts out of the ground, but $N avoids it!",ch,0,victim,TO_CHAR);
	return;
}
