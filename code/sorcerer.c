
/****************************************************************************/
/* Welcome to Sorcerer land.						    					*/
/****************************************************************************/
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "merc.h"
#include "magic.h"
#include "recycle.h"
#include "db.h"
#include "lookup.h"
#include "interp.h"
#include "tables.h"

int flag_lookup args((const char *name, const struct flag_type *flag_table));
void concave_shell_move args((CHAR_DATA *ch, int *dirptr, ROOM_INDEX_DATA *oldroom));
char * flag_name_lookup args((long bitv, const struct flag_type *flag_table));
int damage_queue args((CHAR_DATA *ch, CHAR_DATA *victim, int dam, int damtype, bool blockable, int add, int mult, char *dnoun));
int para_compute(int ele1, int ele2)
{
	switch (ele1) {
		case(ELE_HEAT):
			switch (ele2) {
				case(ELE_AIR):			return ELE_SMOKE;
				case(ELE_EARTH):		return ELE_MAGMA;
				case(ELE_WATER):		return ELE_ACID;
				case(ELE_ELECTRICITY):	return ELE_PLASMA;
			}
			return -1;
		case(ELE_COLD):
			switch (ele2) {
				case(ELE_AIR):			return ELE_BLIZZARD;
				case(ELE_EARTH):		return ELE_FROST;
				case(ELE_WATER):		return ELE_ICE;
				case(ELE_ELECTRICITY):	return ELE_CRYSTAL;
			}
			return -1;
		case(ELE_AIR):
			switch (ele2) {	
				case(ELE_HEAT):			return ELE_SMOKE;
				case(ELE_COLD):			return ELE_BLIZZARD;
				case(ELE_WATER):		return ELE_MIST;
				case(ELE_ELECTRICITY):	return ELE_LIGHTNING;
			}
			return -1;
		case(ELE_EARTH):
			switch (ele2) {	
				case(ELE_HEAT):			return ELE_MAGMA;
				case(ELE_COLD):			return ELE_FROST;
				case(ELE_WATER):		return ELE_OOZE;
				case(ELE_ELECTRICITY):	return ELE_METAL;
			}
			return -1;
		case(ELE_WATER):
			switch (ele2) {
				case(ELE_HEAT):			return ELE_ACID;
				case(ELE_COLD):			return ELE_ICE;
				case(ELE_AIR):			return ELE_MIST;
				case(ELE_EARTH):		return ELE_OOZE;
			}
			return -1;
		case(ELE_ELECTRICITY):
			switch (ele2) {
				case(ELE_HEAT):			return ELE_PLASMA;
				case(ELE_COLD):			return ELE_CRYSTAL;
				case(ELE_AIR):			return ELE_LIGHTNING;
				case(ELE_EARTH):		return ELE_METAL;
			}
			return -1;
		default:
			return -1;
	}
	return -1;
}

void spell_scorch(int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int dam;
	dam = dice(20 + level,3);
	if(saves_spell(level,victim,DAM_FIRE))
		dam /= 2;
	
	act("Waves of heat ripple through the air as $n's flesh crackles.",victim,0,ch,TO_ROOM);
	act("Waves of heat ripple through the air towards you.\n\rYou feel your skin crackling.",ch,0,victim,TO_VICT);
	damage_new(ch,victim,dam,sn,DAM_FIRE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);
}

void spell_gravity_well(int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	OBJ_DATA *well = get_obj_list(ch, target_name, ch->in_room->contents), *gwell;
	AFFECT_DATA af;
	ROOM_AFFECT_DATA raf;
	AREA_AFFECT_DATA aaf;
	
	if(!well || !str_cmp(target_name,""))
		return send_to_char("You don't see that object here to condense.\n\r",ch);
	if(!IS_SET(well->wear_flags,ITEM_TAKE))
		return send_to_char("You can't get a grasp on that.\n\r",ch);
	
	if(is_affected(ch,sn))
		return send_to_char("You can't create a new gravity well yet.\n\r",ch);
	
	if(well->weight<10)
		return send_to_char("That object isn't massive enough to support a gravity well.\n\r",ch);
	
    if (well->item_type == ITEM_CORPSE_PC && well->contains!=NULL)
		return send_to_char("You cannot focus on the corpse while it is laden.\n\r",ch);

	if (is_affected_area(ch->in_room->area, gsn_gravity_well))
		return send_to_char("Gravitational disturbances in the area prevent the creation of your well.\n\r",ch);

	act("$p folds in upon itself, warping the space around it as it shrinks until no more than the tiniest speck remains.",
	ch, well, 0, TO_ALL);
	gwell = create_object(get_obj_index(2950), 0);
	gwell->weight	= well->weight;
	obj_to_room(gwell, ch->in_room);
	extract_obj(well);

	init_affect_room(&raf);
	raf.where       = TO_ROOM_AFFECTS;
	raf.type        = sn;
	raf.aftype      = AFT_SPELL;
	raf.level       = level;
	raf.duration    = level/6;
	raf.location    = 0;
	raf.modifier	= 0;
	raf.owner       = ch;
	raf.tick_fun	= NULL;
	raf.end_fun	= gravity_well_explode;
	new_affect_to_room(ch->in_room,&raf);
	
	init_affect(&af);
	af.where	= TO_AFFECTS;
	af.type		= sn;
	af.aftype	= AFT_TIMER;
	af.level	= level;
	af.duration	= 72;
	af.location	= 0;
	af.modifier	= 0;
	affect_to_char(ch, &af);

	init_affect_area(&aaf);
	aaf.where	=	TO_AREA_AFFECTS;
	aaf.type	=	gsn_gravity_well;
	aaf.aftype	=	AFT_SPELL;
	aaf.level	=	level;
	aaf.duration=	-1;
	aaf.location=	0;
	aaf.modifier=	0;
	aaf.owner	=	ch;
	aaf.tick_fun=	NULL;
	aaf.end_fun	=	NULL;
	affect_to_area(ch->in_room->area,&aaf);
	
}

void gravity_well_explode(ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *af)
{
	CHAR_DATA *victim, *v_next, *ch = af->owner;
	OBJ_DATA *well;
	int dam;
	bool wasinroom = TRUE;

	affect_strip_area (room->area, gsn_gravity_well);
	
	for(well = object_list; well!=NULL; well = well->next)
		if(well->item_type == ITEM_GRAVITYWELL && well->in_room && well->in_room == room)
		break;
	if(!well)
		return;

	send_to_char("You lose control of your gravity well and it ruptures violently!\n\r",ch);

	if(well->in_room->people)
		act("The gravity well flares brightly and explodes, sending waves of force rippling outward!",
			well->in_room->people, 0, 0, TO_ALL);

	extract_obj(well);
	affect_strip_room(room, gsn_gravity_well);
	for(victim = room->people; victim!=NULL; victim = v_next)
	{
		v_next = victim->next_in_room;
		if(!IS_NPC(victim) && is_safe_new(ch,victim,FALSE))
			continue;
		dam = dice(20,20);
		if (ch == victim)	dam /= 2;
		
		if (!ch->in_room) {	
			char_to_room(ch,room);
			wasinroom = FALSE;
		}
		
		damage_new(ch,victim,dam,af->type,DAM_ENERGY,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"the violent explosion*");
		if (!wasinroom) char_from_room(ch);
	}
}

void spell_cyclone(int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	AREA_AFFECT_DATA aaf;
	AFFECT_DATA af;
	
	if(ch->in_room->sector_type == SECT_UNDERWATER ||
		ch->in_room->sector_type == SECT_INSIDE	   ||
		ch->in_room->sector_type == SECT_CITY 	   ||
		IS_SET(ch->in_room->room_flags,ROOM_LAW))
			return send_to_char("You can't create a cyclone here.\n\r",ch);
	
	if(is_affected(ch,gsn_cyclone))
		return send_to_char("You cannot create another cyclone yet.\n\r",ch);
	
	if (is_affected_area(ch->in_room->area,gsn_cyclone) ||
		is_affected_area(ch->in_room->area,gsn_cyclone_forming) ||
		is_affected_area(ch->in_room->area,gsn_whiteout))
		return send_to_char("This area is already affected by a weather disturbance.\n\r",ch);

	init_affect_area(&aaf);
	aaf.where       = TO_AREA_AFFECTS;
	aaf.type        = gsn_cyclone_forming;
	aaf.aftype      = AFT_SPELL;
	aaf.level       = level;
	aaf.duration    = 6;
	aaf.location    = 0;
	aaf.modifier    = 0;
	aaf.owner       = ch;
	aaf.tick_fun    = cyclone_begin_tick;
	aaf.end_fun     = cyclone_begin;
	affect_to_area(ch->in_room->area, &aaf);
	
	init_affect(&af);
	af.where        = TO_AFFECTS;
	af.type         = gsn_cyclone;
	af.aftype       = AFT_TIMER;
	af.level        = level;
	af.duration     = 95;
	af.location     = 0;
	af.modifier     = 0;
	affect_to_char(ch, &af);
	
	send_to_char("As you feed energy into the air overhead, powerful, swirling currents begin to develop.\n\r",ch);
	act("$n concentrates intently and the surrounding winds seem to pick up slightly.",ch,0,0,TO_ROOM);
}

void cyclone_begin(AREA_DATA *area, AREA_AFFECT_DATA *af)
{
	AREA_AFFECT_DATA aaf;
	
	if (is_affected_area(area,gsn_cyclone) ||
		is_affected_area(area,gsn_whiteout))	
		return;

	init_affect_area(&aaf);
   	zone_echo(area, "{cThe sky turns dusty grey and winds swirl around you as an ominous black funnel cloud forms!{x");
   	aaf.where       = TO_AREA_AFFECTS;
   	aaf.type        = gsn_cyclone;
   	aaf.aftype      = AFT_SPELL;
   	aaf.level       = af->level;
   	aaf.duration    = 18;
   	aaf.location    = 0;
   	aaf.modifier    = 0;
   	aaf.owner       = af->owner;
   	aaf.tick_fun    = NULL;
   	aaf.end_fun     = cyclone_end_fun;
	affect_to_area(area,&aaf);
}

void cyclone_begin_tick(AREA_DATA *area, AREA_AFFECT_DATA *af)
{
	zone_echo(area, "{cThe surrounding winds seem to pick up noticably as the horizon darkens.{x");
}

void cyclone_end_fun(AREA_DATA *area, AREA_AFFECT_DATA *af)
{
	zone_echo(area, "{cThe cyclone gradually dissipates as the skies lighten and debris settles.{x");
}

void spell_chill(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
 	int dam;
	AFFECT_DATA af;
	act("You draw the heat out of $N's flesh, chilling $M.",ch,0,victim,TO_CHAR);
	dam = dice(5,4) + level;
	if(!saves_spell(level,victim,DAM_COLD)) {
		init_affect(&af);
		af.where = TO_AFFECTS;
		af.aftype = AFT_MALADY;
		af.type = sn;
		af.location = APPLY_STR;
		af.owner = ch;
		af.duration = 10;
		af.level = level;
		af.modifier = -2;
		if (is_affected(victim,gsn_chill)) {
			affect_join(victim,&af);
		} else {
			affect_to_char(victim,&af);
		}
		act("$n's flesh turns purple as he shivers!",victim,0,ch,TO_ROOM);
		act("You begin shivering violently.",0,0,victim,TO_VICT);
		damage_new(ch,victim,dam,TYPE_UNDEFINED,DAM_COLD,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"chill");
	} else {
	dam /= 2;
	damage_new(ch,victim,dam,TYPE_UNDEFINED,DAM_COLD,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"chill");
	}
	return;
}

void spell_chillmetal(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *victim=NULL;
    OBJ_DATA *obj=NULL;
    OBJ_AFFECT_DATA oaf;

    if (target == TARGET_CHAR)
    {
        victim = (CHAR_DATA *) vo;

        if ((obj = get_eq_char(victim, WEAR_WIELD)) == NULL)
        {
            send_to_char("They are not wielding a weapon.\n\r", ch);
            return;
        }
    }
    else {
        obj = (OBJ_DATA *) vo;
    }
    if (obj->pIndexData->material_index < 0 || material_table[obj->pIndexData->material_index].mat_conductivity <= 0)
    {
        act("You can only draw sufficient heat out of metal weapons.",ch,obj,NULL,TO_CHAR);
        return;
    }

    if (target == TARGET_CHAR) {
	if (ch == victim)
            act("You draw the heat out of your $p.",ch,obj,victim,TO_CHAR);
	else
            act("You draw the heat out of $N's $p.",ch,obj,victim,TO_CHAR);
        act("Your $p turns icy in your hands!",ch,obj,victim,TO_VICT);
        act("$N's $p suddenly takes on dull blue hue.",ch,obj,victim,TO_ROOM);
    }
    else {
        act("You draw the heat out of $p, leaving it icy to the touch.",ch,obj,NULL,TO_CHAR);
    }

    init_affect_obj(&oaf);
    oaf.where        = TO_OBJ_AFFECTS;
    oaf.type         = sn;
    oaf.aftype       = get_spell_aftype(ch);
    oaf.level        = level;
    oaf.duration     = level;
    oaf.location     = APPLY_OBJ_V3;
    oaf.modifier     = 30 - obj->value[3];
    affect_to_obj(obj,&oaf);

    if (target == TARGET_CHAR) {
        if(!trusts(ch,victim))
  	    multi_hit(victim,ch,TYPE_HIT);
    }
    return;
}

void spell_conflagration(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	ROOM_AFFECT_DATA raf;
	int i;

	if (is_affected(ch,sn))
		return send_to_char("You are not yet ready to channel such large amounts of heat energy again.\n\r",ch);

	if (is_affected_room(ch->in_room,sn))
		return send_to_char("The room is already in flames.\n\r",ch);

 	if ((ch->in_room->sector_type) && ((ch->in_room->sector_type == SECT_WATER)
		|| (ch->in_room->sector_type == SECT_AIR)
		|| (ch->in_room->sector_type == SECT_DESERT)
		|| (ch->in_room->sector_type == SECT_INSIDE)
		|| (ch->in_room->sector_type == SECT_CITY)))
	{
		send_to_char("This room is not flammable.\n\r",ch);
		return;
	}
	for(i=0; i<=6; i++)
		if(ch->in_room->exit[i])
			break;
	if(i==6)
		return send_to_char("The fire needs room to spread.\n\r",ch);
	init_affect_room(&raf);
	raf.where	= TO_ROOM_AFFECTS;
	raf.type	= sn;
	raf.aftype	= get_spell_aftype(ch);
	raf.level	= level;
	raf.duration	= 20;
	raf.location	= APPLY_ROOM_SECT;
	raf.modifier	= SECT_CONFLAGRATION - ch->in_room->sector_type;
	raf.owner	= ch;
	raf.end_fun	= conflag_burnout;
	raf.pulse_fun = conflagration_pulse;
	raf.tick_fun	= NULL;
	new_affect_to_room(ch->in_room,&raf);

	init_affect(&af);
	af.where	= TO_AFFECTS;
	af.type		= sn;
	af.aftype	= AFT_TIMER;
	af.level	= level;
	af.duration	= 144;
	af.location	= 0;
	af.modifier	= 0;
	affect_to_char(ch,&af);

	act("The room erupts in a fiery explosion and is engulfed in searing flames!",ch,0,0,TO_ALL);
	zone_echo(ch->in_room->area,"The crackling roar of fire reaches your ears.\n\r");
	return;
}

bool conflagrate_room(ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *oldaf)
{
	CHAR_DATA *rch;
	ROOM_AFFECT_DATA raf;

	int i;
	if (is_affected_room(room,gsn_conflagration))
		return FALSE;
        for(i=0; i<=6; i++)
                if(room->exit[i])
                        break;
        if(i==6)
		return FALSE;
 	if (((room->sector_type) && (room->sector_type == SECT_WATER))
		|| (room->sector_type == SECT_AIR)
		|| (room->sector_type == SECT_DESERT)
		|| (room->sector_type == SECT_INSIDE)
		|| (room->sector_type == SECT_CONFLAGRATION)
		|| (room->sector_type == SECT_CITY))
	{
		return FALSE;
	}

	init_affect_room(&raf);
	raf.where	= TO_ROOM_AFFECTS;
	raf.type	= oldaf->type;
	raf.aftype	= oldaf->aftype;
	raf.level	= oldaf->level;
	raf.duration	= 20;
	raf.location	= APPLY_ROOM_SECT;
	raf.modifier	= SECT_CONFLAGRATION - room->sector_type;
	raf.owner	= oldaf->owner;
	raf.end_fun	= oldaf->end_fun;
	raf.pulse_fun	= conflagration_pulse;
	raf.tick_fun	= NULL;
	new_affect_to_room(room,&raf);

	for (rch = room->people; rch != NULL; rch = rch->next_in_room )
		act("The room erupts in a fiery explosion and is engulfed in searing flames!",rch,NULL,NULL,TO_CHAR);

	zone_echo(room->area,"The crackling roar of fire reaches your ears.\n\r");

	return TRUE;
}

void conflagration_pulse(ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *af)
{
	CHAR_DATA *vch,*vch_next;
	ROOM_INDEX_DATA *room_next;
	int dam, exit;
	
	if (room->sector_type != SECT_CONFLAGRATION)
		return;

	if (number_range(1,3) != 1)
		return;

	if (room->people) {
		for (vch = room->people; vch; vch = vch_next) {
			vch_next = vch->next_in_room;

			dam = dice(vch->level/2,3) + 5;

			if (IS_NPC(vch) && (vch->max_hit) > 0
				&& ((vch->hit*100)/vch->max_hit < 50)) {
				dam -= (int)((float)dam * 0.05) * (50 - (vch->hit*100)/vch->max_hit);
			}

			if (is_affected(vch,gsn_heatshield))
				dam = (int)((float)dam * .1);

			if (dam <= 1)
				continue;

			damage_new(af->owner,vch,dam,TYPE_UNDEFINED,DAM_FIRE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"the raging wildfire*");
		}
	}

	while(!room->exit[(exit = number_range(0,5))]);
	if (room->exit[exit] && (room_next = room->exit[exit]->u1.to_room)
		&& room_next->area == room->area && number_percent () <= 8)
		conflagrate_room(room_next,af);

	return;
}

void conflag_burnout(ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *af)
{
	ROOM_AFFECT_DATA raf;
	CHAR_DATA *rch;

	init_affect_room(&raf);
	raf.where	= TO_ROOM_AFFECTS;
	raf.type	= skill_lookup("reserved");
	raf.aftype	= af->aftype;
	raf.level	= af->level;
	raf.duration	= 144;
	raf.location	= APPLY_ROOM_SECT;
	raf.modifier	= SECT_DESERT - (room->sector_type - af->modifier);
	raf.owner	= af->owner;
	raf.end_fun	= NULL;
	raf.tick_fun	= NULL;
	new_affect_to_room(room,&raf);

	for (rch = room->people; rch != NULL; rch = rch->next_in_room )
		act("The raging flames die out as smoldering smoke lingers about scorched land.",rch,NULL,NULL,TO_CHAR);

	return;
}

void spell_ultradiffusion(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;

	if (!is_affected(ch,sn))
	{
		init_affect(&af);
		af.where	= TO_AFFECTS;
		af.type		= sn;
		af.aftype	= get_spell_aftype(ch);
		af.level	= level;
		af.duration	= 26;
		af.location	= 0;
		af.modifier	= 0;
		SET_BIT(af.bitvector, AFF_PASS_DOOR);
		af.owner	= NULL;
		af.end_fun	= ultradiffusion_end;
		af.tick_fun	= ultradiffusion_tick;
		af.mod_name = MOD_PHASE;
		act("The molecules of your body diffuse as your body becomes invisible to the naked eye.",ch,NULL,NULL,TO_CHAR);
		act("The molecules of $n's body diffuse as $e vanishes from sight.",ch,NULL,NULL,TO_ROOM);
        new_affect_to_char(ch,&af);
		return;
	}
	else
	{
		send_to_char("Your molecules are already diffused from your body.\n\r",ch);
		return;
	}

	return;
}

void ultradiffusion_tick(CHAR_DATA *ch, AFFECT_DATA *af)
{
	switch(af->duration) {
		case(20):
		case(18):
		case(16):
			send_to_char("You note the odd sensation of a few of your component particles drifting away.\n\r",ch);
			break;
		case(14):
		case(12):
		case(10):
			send_to_char("You are starting to lose control of your molecular cohesion.\n\r",ch);
			break;
		case(8):
		case(6):
		case(4):
			send_to_char("You sense that you are coming dangerously close to losing cohesion entirely.\n\r",ch);
			break;
		case(3):
		case(2):
		case(1):
			send_to_char("Your consciousness begins to fade as you lose cohesion.\n\rIt may already be too late.\n\r",ch);
			break;
		default:
			break;
	}

	return;
}

void un_ultradiffusion(CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA *paf;

	if (!is_affected(ch,gsn_ultradiffusion))
		return;

	for (paf = ch->affected; paf != NULL; paf = paf->next)
	{
		if (paf->type == gsn_ultradiffusion)
			break;
	}

	if (paf->duration == 0)
		return send_to_char("Utter horror sweeps over you as you realize that you cannot reassemble your original form!\n\r",ch);

	affect_remove(ch,paf);
	return;
}
void ultradiffusion_end(CHAR_DATA *ch, AFFECT_DATA *af)
{
	AFFECT_DATA paf, *rpaf;
	int modifier = 0;

	if (af->duration == -1) {
		return;
	} else if (af->duration == 0) {
		act("As your molecular cohesion entirely evaporates, your consciousness drifts away into nothingness.",ch,NULL,NULL,TO_CHAR);
		act("$n's molecular cohesion entirely evaporates!",ch,NULL,NULL,TO_ROOM);
	} else {
		act("You attempt to recollect the molecules of your body as you become visible to the naked eye.",ch,NULL,NULL,TO_CHAR);
		act("$n attempts to recollect the molecules of $s body as $e becomes visible.",ch,NULL,NULL,TO_ROOM);
	}

	for(rpaf = ch->affected; rpaf; rpaf = rpaf->next)
		if(rpaf->type == gsn_ultradiffusion && rpaf->end_fun)
			rpaf->end_fun = NULL;

	switch (af->duration) {
		case (20):
		case (19):
			damage_new(ch,ch,20,gsn_ultradiffusion,DAM_TRUESTRIKE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);
			break;
		case (18):
		case (17):
			damage_new(ch,ch,30,gsn_ultradiffusion,DAM_TRUESTRIKE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);
			break;
		case (16):
		case (15):
			damage_new(ch,ch,50,gsn_ultradiffusion,DAM_TRUESTRIKE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);
			break;
		case (14):
		case (13):
			damage_new(ch,ch,100,gsn_ultradiffusion,DAM_TRUESTRIKE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);
			break;
		case (12):
		case (11):
			damage_new(ch,ch,200,gsn_ultradiffusion,DAM_TRUESTRIKE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);
			break;
		case (10):
		case (9):
			damage_new(ch,ch,300,gsn_ultradiffusion,DAM_TRUESTRIKE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);
			break;
		case (8):
		case (7):
			damage_new(ch,ch,400,gsn_ultradiffusion,DAM_TRUESTRIKE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);
			break;
		case (6):
		case (5):
			damage_new(ch,ch,600,gsn_ultradiffusion,DAM_TRUESTRIKE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);
			modifier = 1;
			break;
		case (4):
		case (3):
			damage_new(ch,ch,800,gsn_ultradiffusion,DAM_TRUESTRIKE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);
			modifier = 2;
			break;
		case (2):
		case (1):
			damage_new(ch,ch,1000,gsn_ultradiffusion,DAM_TRUESTRIKE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);
			modifier = 3;
			break;
		case (0):
			af->duration = -1;
			raw_kill(ch,ch); break;
		default:
			break;
	}

	if (modifier != 0)
	{
		if (af && af->duration <= 3)
		{
			init_affect(&paf);
			paf.where	= TO_AFFECTS;
			paf.type	= gsn_molecular_disjunction;
			paf.aftype	= AFT_MALADY;
			paf.level	= ch->level;
			paf.duration	= -1;
			paf.location	= APPLY_INT;
			paf.modifier	= modifier * number_range(-6,-5);
			affect_to_char(ch,&paf);
		}
		if (af && af->duration <= 2)
		{
			paf.location	= APPLY_DEX;
			paf.modifier	= modifier * number_range(-6,-5);
			affect_to_char(ch,&paf);
		}
		if (af && af->duration <= 1)
		{
			paf.location	= APPLY_STR;
			paf.modifier	= modifier * number_range(-6,-5);
			affect_to_char(ch,&paf);
		}
	}


	WAIT_STATE(ch,3*PULSE_VIOLENCE);
	return;
}


void spell_heat_metal(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	OBJ_DATA *obj, *obj2;

	if ((obj = get_eq_char(victim, WEAR_WIELD)) == NULL)
	{
		send_to_char("They are not wielding a weapon.\n\r", ch);
		return;
	}

	if (obj->pIndexData->material_index < 0 || material_table[obj->pIndexData->material_index].mat_conductivity <= 0)
	{
		act("$p cannot be heated.", ch, obj, victim, TO_CHAR);
		return;
	}

	if (number_range(0,10) > material_table[obj->pIndexData->material_index].mat_conductivity)
	{
		act("You fail to sufficiently heat $N's $p.", ch, obj, victim, TO_CHAR);
		act("$p grows slightly warm in your hand.", ch, obj, victim, TO_VICT);
		act("$N's $p glows a dull red.", ch, obj, victim, TO_NOTVICT);
		return;
	}

	if (IS_OBJ_STAT(obj,ITEM_NODISARM))
	{
		act("You rapidly raise the temperature of $N's weapon, but $E adjusts $S grip.", ch, obj, victim, TO_CHAR);
		act("Your weapon suddenly becomes burning to the touch, but you quickly find a comfortable grip.", ch, obj, victim, TO_VICT);
		act("$N's weapon glows a dull red, but $E adjusts $S grip.", ch, obj, victim, TO_NOTVICT);
		damage_new(ch,victim,dice(4,4),gsn_burning_hands,DAM_FIRE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);
		set_fighting(ch,victim);
		return;
	}

	if (IS_OBJ_STAT(obj,ITEM_NOREMOVE))
	{
		act("You rapidly raise the temperature of $N's weapon, but it clings to $S hand, burning him!", ch, obj, victim, TO_CHAR);
		act("Your weapon suddenly becomes burning to the touch, but you cannot let go of it!", ch, obj, victim, TO_VICT);
		act("$N's weapon glows a dull red, but it clings to $S hand, burning him!", ch, obj, victim, TO_NOTVICT);
		damage_new(ch,victim,dice(level,5),gsn_burning_hands,DAM_FIRE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);
		set_fighting(ch,victim);
		return;
	}

	act("You rapidly raise the temperature of $N's weapon and $E drops it, pulling back $S hand in pain!", ch, obj, victim, TO_CHAR);
	act("Your weapon suddenly becomes burning to the touch, and you instinctively drop it!", ch, obj, victim, TO_VICT);
	act("$N's weapon glows a dull red and $E drops it, pulling back $S hand in pain!", ch, obj, victim, TO_NOTVICT);

	damage_new(ch,victim,dice(4,4),gsn_burning_hands,DAM_FIRE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);
	set_fighting(ch,victim);

	obj_from_char(obj);

	if (IS_OBJ_STAT(obj,ITEM_NODROP) || IS_OBJ_STAT(obj,ITEM_INVENTORY))
	{
		obj_to_char(obj, victim);
	}
	else
	{
		obj_to_room(obj, victim->in_room);
		if (IS_NPC(victim) && victim->wait == 0 && can_see_obj(victim,obj))
			get_obj(victim,obj,NULL, TRUE);
 	}
	reslot_weapon(victim);

	if ((obj2 = get_eq_char(victim,WEAR_DUAL_WIELD)) != NULL)
	{
		unequip_char(victim,obj2,FALSE);
		act("You hastily swap $p into your primary hand.",ch,obj2,0,TO_CHAR);
		act("$n hastily swaps $p into $s primary hand.",ch,obj2,0,TO_ROOM);
		equip_char(victim,obj2,WEAR_WIELD,FALSE);
	}

	return;
}

void spell_knock(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	int door;
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_opp;
	CHAR_DATA *fch;
	char *direction;

	if (!str_cmp(target_name,"")) {
		return send_to_char("Which door?\n\r",ch);
	}

	if ((door = find_door(ch,target_name)) >= 0) {
		pexit = ch->in_room->exit[door];
		if(!pexit->u1.to_room)
			return send_to_char("There is no door there.\n\r",ch);

		if (!IS_SET(pexit->exit_info,EX_CLOSED))
			return send_to_char("That door is not closed.\n\r",ch);
		if (!IS_SET(pexit->exit_info,EX_LOCKED))
			return send_to_char("That door is not locked.\n\r",ch);
		if (IS_SET(pexit->exit_info,EX_NOPASS)) {
			direction = (char*)flag_name_lookup(door,direction_table);
			act("You rapidly heat the $T door, causing it to expand within its confines.",ch,0,direction,TO_CHAR);
			act("The $T door buckles slightly, but remains intact.",ch,0,direction,TO_ALL);
			return;
		}

		direction = (char*)flag_name_lookup(door,direction_table);
		act("You rapidly heat the $T door, causing it to expand within its confines.",ch,0,direction,TO_CHAR);
		act("The $T door suddenly buckles on its hinges and bursts open!",ch,0,direction,TO_ALL);

		REMOVE_BIT(pexit->exit_info,EX_LOCKED);
		REMOVE_BIT(pexit->exit_info,EX_CLOSED);
		REMOVE_BIT(pexit->exit_info, EX_ISDOOR);

		if ((to_room = pexit->u1.to_room) != NULL
			&& (pexit_opp = to_room->exit[reverse_d(door)]) != NULL
			&& pexit_opp->u1.to_room == ch->in_room) {
			REMOVE_BIT(pexit_opp->exit_info,EX_LOCKED);
			REMOVE_BIT(pexit_opp->exit_info,EX_CLOSED);
			REMOVE_BIT(pexit_opp->exit_info, EX_ISDOOR);
		}

		direction = (char*)flag_name_lookup(reverse_d(door),direction_table);
		for (fch = to_room->people; fch != NULL; fch = fch->next_in_room) {
			act("The $T door suddenly buckles on its hinges and bursts open!",fch,0,direction,TO_ALL);
			break;
		}
	}
	send_to_char("There is no door there.\n\r",ch);
	return;
}


void spell_vacuum(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	ROOM_AFFECT_DATA raf;
	CHAR_DATA *vch, *vch_next;
	EXIT_DATA *pexit;
	ROOM_INDEX_DATA *to_room;
	int i;
	char *direction;
	char buf[MAX_STRING_LENGTH];

	if (is_affected_room(ch->in_room,sn))
		return send_to_char("The room is already a vacuum.\n\r",ch);

	for (i=0; i<=6; i++)
		if(ch->in_room->exit[i])
			break;
	if (i==6)
		return send_to_char("There is nowhere for the air to go!\n\r",ch);


	act("Concentrating intently, you force all the air out of the area!",ch,0,0,TO_CHAR);
	act("$n raises $s arms, and in a violent rush, all the air is torn from the surrounding area!",ch,0,0,TO_ROOM);

	init_affect_room(&raf);
	raf.where		= TO_ROOM_AFFECTS;
	raf.type		= sn;
	raf.aftype		= get_spell_aftype(ch);
	raf.level		= level;
	raf.duration	= -1;
	raf.location	= 0;
	raf.modifier	= 0;
	raf.owner		= ch;
	raf.end_fun		= vacuum_end_fun;
	raf.tick_fun	= NULL;
	new_affect_to_room(ch->in_room,&raf);

	for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
	{
		vch_next = vch->next_in_room;
		if (!IS_NPC(vch) && is_safe_new(ch,vch,FALSE))
			continue;

		if (!IS_NPC(vch) && !is_same_group(vch,ch))
		{
			sprintf(buf,"Die, %s you sorcerous dog!",PERS(ch,vch));
			do_myell(vch,buf,ch);
		}

		WAIT_STATE(vch,PULSE_VIOLENCE);
		damage_new(ch,vch,dice(level,6),TYPE_HIT + attack_lookup("asphyxiation"),DAM_ENERGY,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"the vacuum*");
	}

	for (i=0; i<=5; i++)
		if((pexit = ch->in_room->exit[i]))
			if ((to_room = pexit->u1.to_room))
				if (to_room->people) {
					direction = (char*)flag_name_lookup(reverse_d(i),direction_table);
					act("A sudden blast of air rushes in from the $T!",to_room->people,0,direction,TO_ALL);
				}

	return;
}

void vacuum_end_fun(ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *af)
{
	CHAR_DATA *vch, *vch_next, *smacked;
	EXIT_DATA *pexit;
	ROOM_INDEX_DATA *to_room;
	int i, j, roomcount = 0, pplcount = 0, randperson = 0, objcount = 0;
	char *direction;
	OBJ_DATA *obj, *obj_next;

	if (room->people)
		act("Air rushes back into the area in a fierce torrent!",room->people,0,0,TO_ALL);

	for (i=0; i<=5; i++)
	{
		if((pexit = room->exit[i]))
		{
			if (!IS_SET(pexit->exit_info,EX_CLOSED))
				roomcount++;
			if ((to_room = pexit->u1.to_room))
				if (to_room->people) {
					direction = (char*)flag_name_lookup(reverse_d(i),direction_table);
					act("Air rushes back $Tward!",to_room->people,0,direction,TO_ALL);
				}
		}
	}

	for (vch = room->people; vch != NULL; vch = vch_next)
	{
		vch_next = vch->next_in_room;
		if (is_safe(af->owner,vch)) continue;
		WAIT_STATE(vch,PULSE_VIOLENCE);
		damage_new(af->owner,vch,(3*af->level)/roomcount,TYPE_HIT + attack_lookup("asphyxiation"),DAM_ENERGY,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"the vacuum*");
		if (vch) pplcount++;
	}

	for (i=0; i<=5; i++)
	{
		if((pexit = room->exit[i]))
		{
			if (IS_SET(pexit->exit_info,EX_CLOSED))
				continue;
			if ((to_room = pexit->u1.to_room))
			{
				for (obj = to_room->contents; obj != NULL; obj = obj_next )
				{
					if (objcount > 8)
						break;
					obj_next = obj->next_content;
					if (get_true_weight(obj) < 5 && IS_SET(obj->wear_flags, ITEM_TAKE))
					{
						if (to_room->people)
							act("$p is sucked out of the room!",to_room->people,obj,0,TO_ALL);
						obj_from_room(obj);
						obj_to_room(obj,room);
						smacked = NULL;
						while (!smacked && room->people)
						{
						randperson = number_range(0,pplcount);
						j = 0;
						for (vch = room->people; vch != NULL; vch = vch_next)
						{
						vch_next = vch->next_in_room;
						if (j == randperson && !is_safe_new(af->owner,vch,FALSE))
							smacked = vch;
						j++;
						}
						}
						if (!smacked)
							break;
						act("$p is sucked into the room, striking $n!",smacked,obj,0,TO_ROOM);
						act("$p is sucked into the room, striking you!",smacked,obj,0,TO_CHAR);
						damage_new(af->owner,smacked,dice(get_true_weight(obj)+1,9),TYPE_UNDEFINED,DAM_BASH,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"the flying debris*");
						objcount++;
					}
				}
			}
		}
	}

	if (af->modifier > 3)
	{
	for (i=0; i<=5; i++)
	{
		if((pexit = room->exit[i]))
		{
			if (IS_SET(pexit->exit_info,EX_CLOSED))
				continue;
			if ((to_room = pexit->u1.to_room))
			{
				for (vch = to_room->people; vch != NULL; vch = vch_next )
				{
					vch_next = vch->next_in_room;
					if(IS_NPC(vch) && IS_SET(vch->act,ACT_SENTINEL))
						continue;
					if(!IS_NPC(vch) && is_safe_new(af->owner,vch,FALSE))
						continue;
					direction = (char*)flag_name_lookup(reverse_d(i),direction_table);
					act("A torrent of air pulls $n $T!",vch,0,direction,TO_ROOM);
					char_from_room(vch);
					char_to_room(vch, room);
					act("A torrent of air pulls you unexpectedly $T!",vch,0,direction,TO_CHAR);
					do_look(vch,"auto");
					direction = (char*)flag_name_lookup(i,direction_table);
					act("The torrent of air pulls $n into the room from the $T!",vch,0,direction,TO_ROOM);
				}
			}
		}
	}
	}
	return;
}

void spell_incandescense( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if(is_affected(victim,sn))
        return;

    init_affect(&af);
    af.where     = TO_AFFECTS;
    af.aftype    = get_spell_aftype(ch);
    af.type      = sn;
    af.level     = level;
    af.duration  = level/3;
    af.location  = APPLY_AC;
    af.modifier  = (int)(-1 * (average_ac(victim) * (float)((float) level/ (float) 400)));
	af.mod_name = MOD_VISIBILITY;
	affect_to_char( victim, &af );
    send_to_char( "You begin to glow with a soft white light.\n\r", victim );
    act( "$n begins to glow with a soft white light.", victim, NULL, NULL, TO_ROOM );
}
void spell_infravision( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    AFFECT_DATA af;

    if (is_affected(ch, sn))
        return send_to_char("Your eyes are already accomodated to detecting heat nearby.\n\r",ch);

    init_affect(&af);
    af.where     = TO_AFFECTS;
    af.aftype    = get_spell_aftype(ch);
    af.type      = sn;
    af.level     = level;
    af.duration  = level/3;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
	af.mod_name = MOD_VISION;
	affect_to_char(ch, &af );
    send_to_char( "Your sensitivity to heat sources increases.\n\r", ch);
    return;
}

void spell_diuretic( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
   	AFFECT_DATA *af;
	CHAR_DATA *victim = get_char_room(ch, target_name);
	int heal;
	if(!strcmp(target_name,""))
		victim = ch;
	if(!victim)
		return send_to_char("They aren't here.\n\r",ch);
	if(!trusts(ch,victim) && is_safe(ch,victim))
		return;
	af = affect_find(victim->affected, gsn_bleeding);
	if(!trusts(ch,victim) && saves_spell(level,victim,DAM_OTHER))
	{
			act("$N resists the effects.",ch,0,victim,TO_CHAR);
			act_new("You feel a rush of warmth, but it fades.",ch,0,victim,TO_VICT,POS_SLEEPING);
			act("$N looks flushed for a moment, but it passes.",ch,0,victim,TO_NOTVICT);
			if(!trusts(ch,victim))
				multi_hit(victim,ch,TYPE_UNDEFINED);
			return;
	}
	if(ch!=victim) {
	act("You infuse $N with a rush of healing warmth.",ch,0,victim,TO_CHAR);
	act_new("A feeling of warmth washes over you.",victim,NULL,NULL,TO_CHAR,POS_SLEEPING);
	act("$n looks flushed for a moment.",ch,0,victim,TO_NOTVICT);
	}
	if(ch==victim) {
		send_to_char("A feeling of warmth washes over you.\n\r",ch);
	}

	if(af)
	{
		for(af = victim->affected; af!=NULL; af = af->next)
			if(af->type == gsn_bleeding && af->level<150)
				af->level = (int)((float)af->level * 1.33);
		send_to_char("The flow of blood from your wounds intensifies.\n\r",victim);
		act("The blood seems to gush from $n's wounds more rapidly.",victim,0,0,TO_ROOM);
	}
        heal = dice(2, 8) + level/2;
        victim->hit = UMIN( victim->hit + heal, victim->max_hit );
        update_pos(victim);
	if(!trusts(ch,victim))
		multi_hit(victim,ch,TYPE_HIT);
}

void spell_corona( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
	AFFECT_DATA af;
	if(is_affected(ch,sn))
		return send_to_char("You are already surrounded by a corona of flames.\n\r",ch);
	act("$n is surrounded by a corona of flames that rapidly fades away.",ch,0,0,TO_ROOM);
	send_to_char("You form heat energy into a protective barrier surrounding you.\n\r",ch);
	init_affect(&af);
	af.where	= TO_RESIST;
	af.type		= sn;
	af.aftype	= AFT_SPELL;
	af.location	= 0;
	af.modifier	= 0;
	af.level	= level;
	af.duration	= level / 3;
	af.mod_name = MOD_RESISTANCE;
	SET_BIT(af.bitvector, RES_FIRE);
	affect_to_char(ch,&af);
	af.where	= TO_VULN;
	zero_vector(af.bitvector);
	SET_BIT(af.bitvector, VULN_COLD);
	affect_to_char(ch,&af);
}

void spell_heatshield(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
        AFFECT_DATA af;

        if (is_affected(ch,gsn_heatshield))
        	return send_to_char("You are already surrounded by a shield of heat.\n\r",ch);
        init_affect(&af);
        af.where = TO_AFFECTS;
		af.aftype = AFT_SPELL;
        af.type = gsn_heatshield;
        af.modifier = 0;
        af.level = level;
        af.duration = level / 8;
        af.location = 0;
		af.mod_name = MOD_PROTECTION;
		affect_to_char(ch,&af);
	send_to_char("The air around you ripples with heat.\n\r",ch);
	act("The air around $n ripples with heat.",ch,0,0,TO_ROOM);
}

void spell_immolate(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	OBJ_AFFECT_DATA oaf;
	OBJ_DATA *obj;
	int dam = 0;
	
	act("You concentrate intently, unleashing a burst of heat upon $N's armor.",ch,0,victim,TO_CHAR);
	act("$n extends $s hand towards you, releasing an oppressive wave of heat that washes over you.",ch,0,victim,TO_VICT);
	act("$n extends $s hand towards $N, releasing a burst of heat that washes over $M!",ch,0,victim,TO_NOTVICT);

	for(obj=victim->carrying; obj!=NULL; obj=obj->next_content)
	{
		if (!is_worn(obj) || obj->wear_loc == WEAR_BRAND)
			continue;
		if(obj->pIndexData->material_index>0 &&
			number_percent() < 30 * material_table[obj->pIndexData->material_index].mat_flammability &&
			!(is_affected_obj(obj,gsn_immolate)
			|| IS_AFFECTED(obj,AFF_OBJ_BURNING))
			&& !IS_OBJ_STAT(obj, ITEM_BURN_PROOF))
		{
			init_affect_obj(&oaf);
			oaf.where		=	TO_OBJ_AFFECTS;
			oaf.aftype		=	AFT_SPELL;
			oaf.type		=	gsn_immolate;
			oaf.owner		=	ch;
			oaf.level		=	level;
			oaf.location	=	0;
			oaf.modifier	=	0;
			oaf.duration	=	(short)(2 * pow(material_table[obj->pIndexData->material_index].mat_flammability,2));
			SET_BIT(oaf.bitvector, AFF_OBJ_BURNING);
			oaf.end_fun		=	immolate_end;
			oaf.tick_fun	=	NULL;
			affect_to_obj(obj,&oaf);
			
			act("$p bursts into flames!",victim,obj,0,TO_ALL);
			dam += dice(6,6);
		}
	}

	if(dam)
		damage_new(ch,victim,dam,TYPE_UNDEFINED,DAM_FIRE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"immolation");
	else
		act("You failed to ignite any of $N's armor.",ch,0,victim,TO_CHAR);
}

void immolate_end(OBJ_DATA *obj, OBJ_AFFECT_DATA *af)
{
    if (obj->carried_by)
		act("$p stops burning.",obj->carried_by,obj,0,TO_CHAR);

	if (obj->in_room && obj->in_room->people)
		act("$p stops burning.",obj->in_room->people,obj,0,TO_ALL);
		
	return;
}

void spell_scathing(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
	CHAR_DATA *vch, *vch_next;
	AFFECT_DATA af;
	int dam;
	char buf[MSL];	

	send_to_char("You create violent, scorching gusts of wind to sweep across the area.\n\r",ch);
	act("Heat radiates from $n's fingertips as a scathing wind sweeps across the area.",ch,0,0,TO_ROOM);
	
	init_affect(&af);
	af.where	= TO_AFFECTS;
	af.level	= level;
	af.type		= sn;
	af.aftype	= AFT_SPELL;
	af.location	= APPLY_HITROLL;
	af.modifier	= -4;
	af.duration	= 0;
	SET_BIT(af.bitvector, AFF_BLIND);
	af.mod_name = MOD_VISION;
	for(vch = ch->in_room->people; vch!=NULL; vch=vch_next)
	{
		vch_next = vch->next_in_room;
	        if (is_same_group(vch,ch) || is_safe(ch,vch) || is_same_cabal(ch,vch))
        	    continue;
        	if (!IS_NPC(ch) && !IS_NPC(vch) && (ch->fighting == NULL || vch->fighting == NULL))
	        {
        	        sprintf(buf,"Die, %s you sorcerous dog!",PERS(ch,vch));
                	do_myell(vch,buf,ch);
	        }
		dam = dice((int)(.66 * (float)level), 9);
   		if (saves_spell(level, vch, DAM_FIRE) )
        		dam /= 2;
		if(number_percent()<=20 && !is_affected(vch,sn) &&
			!(IS_SET(vch->act,ACT_UNDEAD)
			|| IS_SET(vch->form,FORM_UNDEAD)))
		{
			act("The heated air sears $n's eyes, blinding $m!",vch,0,0,TO_ROOM);
			send_to_char("The heated air sears your eyes, blinding you!\n\r",vch);
			affect_to_char(vch, &af);
		}
		damage_new(ch,vch,dam,TYPE_UNDEFINED,DAM_FIRE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"scathing wind");
	}
}
void spell_earthquake(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	ROOM_AFFECT_DATA raf;
	CHAR_DATA *vch, *vch_next;
	char buf[MAX_STRING_LENGTH];

	if (is_affected_room(ch->in_room,sn))
		return send_to_char("Are you mad? You'd bring the entire ceiling down!\n\r",ch);

 	if (((ch->in_room->sector_type) && (ch->in_room->sector_type == SECT_WATER)) || (ch->in_room->sector_type == SECT_AIR))
		return;

	act("The earth beneath you rumbles violently!",ch,0,0,TO_ALL);

	for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
	{
		vch_next = vch->next_in_room;

		if ((IS_IMMORTAL(vch) && !can_see(ch,vch)) || is_same_group(vch,ch) || is_safe(ch,vch) || is_same_cabal(ch,vch) || IS_AFFECTED(vch,AFF_FLYING))
			continue;

		if (!IS_NPC(vch))
		{
			sprintf(buf,"Die, %s you sorcerous dog!",PERS(ch,vch));
			do_myell(vch,buf,ch);
		}

		act("You lose your footing amidst the shaking and fall to the ground!",vch,0,0,TO_CHAR);

		WAIT_STATE(vch,PULSE_VIOLENCE);
		damage_new(ch,vch,dice(10,3),sn,DAM_BASH,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);
	}

	switch (ch->in_room->sector_type) {
		case (SECT_FIELD):
			return;
		case (SECT_DESERT):
			return;
		case (SECT_ROAD):
			return;
		case (SECT_HILLS):
			act("Loose rocks and earth tumbles down from the hills around you!",ch,0,0,TO_ALL);
		case (SECT_CAVE):
			if(ch->in_room->sector_type != SECT_HILLS)
				act("Loose rocks and earth tumbles down from the cave around you!",ch,0,0,TO_ALL);
			for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
			{
				vch_next = vch->next_in_room;

				if (is_same_group(vch,ch) || is_safe(ch,vch) || is_same_cabal(ch,vch))
					continue;

				damage_new(ch,vch,dice(4,10),sn,DAM_BASH,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"the scattered debris*");
			}
			return;
		case (SECT_CITY):
			for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
			{
				vch_next = vch->next_in_room;

				if (is_same_group(vch,ch) || is_safe(ch,vch) || is_same_cabal(ch,vch))
					continue;

				act("Loose bricks and mortar tumble down from the buildings around you!",vch,0,0,TO_CHAR);
				damage_new(ch,vch,dice(10,10),sn,DAM_BASH,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"the falling debris*");
			}
			return;
		case (SECT_FOREST):
			for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
			{
				vch_next = vch->next_in_room;

				if (is_same_group(vch,ch) || is_safe(ch,vch) || is_same_cabal(ch,vch))
					continue;

				ADD_WAIT_STATE(vch,PULSE_VIOLENCE);
				act("Nearby trees topple, crushing you beneath their weight!",vch,0,0,TO_CHAR);
				damage_new(ch,vch,dice(10,15),sn,DAM_BASH,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"the falling trees*$");
			}
			return;
		case (SECT_INSIDE):
			init_affect_room(&raf);
			raf.where		= TO_ROOM_AFFECTS;
			raf.type		= sn;
			raf.aftype		= get_spell_aftype(ch);
			raf.level		= level;
			raf.duration	= 1;
			raf.location	= 0;
			raf.modifier	= 0;
			raf.owner		= ch;
			raf.end_fun		= NULL;
			raf.tick_fun	= NULL;
			new_affect_to_room(ch->in_room,&raf);

			for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
			{
				vch_next = vch->next_in_room;

				if (is_safe(ch,vch))
					continue;

				act("A torrential downpour of stone and debris pours down from the ceiling above!",vch,0,0,TO_CHAR);

				if (is_same_group(vch,ch) || is_same_cabal(ch,vch))
					damage_new(ch,vch,dice(10,15),sn,DAM_BASH,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"the falling debris*");
				else
					damage_new(ch,vch,dice(20,15),sn,DAM_BASH,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"the falling debris*");

				ADD_WAIT_STATE(vch,PULSE_VIOLENCE);
			}
		default:
			return;
	}
	return;
}

void spell_electrocute(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	OBJ_DATA *obj;
	int tconduct = 0;

	for (obj = victim->carrying; obj != NULL; obj = obj->next_content)
	{
		if (obj->pIndexData->material_index < 0)
			continue;

		if(!is_worn(obj))
			continue;

		tconduct += material_table[obj->pIndexData->material_index].mat_conductivity;
	}
	act("You attempt to send an electric current though $N's armor.", ch, 0, victim, TO_CHAR);
	act("$n attempts to send an electric current though your armor.", ch, 0, victim, TO_VICT);
	act("$n attempts to send an electric current though $N's armor.", ch, 0, victim, TO_NOTVICT);

	if (tconduct <= 0)
	{
	act("Your armor harmlessly absorbs the electric charge.", victim, 0, 0, TO_CHAR);
	act("$n's armor harmlessly absorbs the electric charge.", victim, 0, 0, TO_ROOM);
	return;
	}
	else if (tconduct < 15)
	{
	act("You experience a brief jolt of pain as electricity hits your armor.", victim, 0, 0, TO_CHAR);
	act("$n jerks slightly as $s armor crackles with electricity.", victim, 0, 0, TO_ROOM);
	}
	else if (tconduct < 30)
	{
	act("Your body is wracked with pain as electricity surges through you!", victim, 0, 0, TO_CHAR);
	act("$n convulses as bursts of electricity explode through $s armor!", victim, 0, 0, TO_ROOM);
	}
	else if (tconduct < 45)
	{
	act("You open your mouth in a silent scream as raw electricity explodes through your joints!", victim, 0, 0, TO_CHAR);
	act("$n's face contorts in agony as $e convulses, briefly paralyzed!", victim, 0, 0, TO_ROOM);
	}
	else
	{
	act("Every inch of your body explodes with pain, your flesh melting as electricity courses into your body!", victim, 0, 0, TO_CHAR);
	act("$n lets out a horrible shriek as the electric current roasts $m alive within $s armor!", victim, 0, 0, TO_ROOM);
	}

	damage_new(ch,victim,dice((level/15)*tconduct,4),sn,DAM_LIGHTNING,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);
	LAG_CHAR(victim,(tconduct/15)*PULSE_VIOLENCE);
	return;
}

void spell_induce_pain(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int dam = dice(level,7) + 10;


	if (saves_spell(level,victim,DAM_ENERGY))
		dam /= 2;

	if (get_curr_stat(victim,STAT_INT) > 10 && number_range(0,200) < (pow(get_curr_stat(victim,STAT_INT)-10,2)) )
	{
		act("Pain courses through your body, but you faintly realize that it is an illusion.",victim,0,0,TO_CHAR);
		dam /= 2;
	}
	else
	{
		act("Your vision clouds as searing pain courses through your body!",victim,0,0,TO_CHAR);
	}

	damage_new(ch,victim,dam,sn,DAM_OTHER,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);

	return;
}

void spell_disrupt_vision( int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	if (IS_AFFECTED(victim, AFF_BLIND) || saves_spell(level,victim,DAM_ENERGY)
		|| IS_SET(victim->act,ACT_UNDEAD) || IS_SET(victim->form,FORM_UNDEAD))
	{
		send_to_char("Spell failed.\n\r",ch);
		send_to_char("Slight flickering briefly obscures your vision but then subsides.\n\r",victim);
		return;
	}

	init_affect(&af);
	af.where	= TO_AFFECTS;
	af.aftype	= get_spell_aftype(ch);
	af.type		= sn;
	af.level	= level;
	af.location	= 0;
	af.modifier	= 0;
	af.duration	= level/12;
	SET_BIT(af.bitvector, AFF_BLIND);
	af.mod_name = MOD_VISION;
	affect_to_char(victim, &af);
	send_to_char("A white flash blankets your field of vision, and you cannot see anything!\n\r",victim);
	act("$n appears to be blinded.",victim,NULL,NULL,TO_ROOM);
	return;
}

void spell_mana_conduit(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
        CHAR_DATA *victim = (CHAR_DATA *) vo;
        AFFECT_DATA af;

        if (is_affected(victim,sn))
        {
                send_to_char("Their ability to channel mana is already energized.\n\r",ch);
                return;
        }

        init_affect(&af);
        af.where        = TO_AFFECTS;
        af.aftype       = get_spell_aftype(ch);
        af.type         = sn;
        af.level        = level;
        af.location     = 0;
        af.modifier     = 0;
        af.duration     = level/3;
		af.mod_name = MOD_REGEN;
		affect_to_char(victim, &af);
	
		if (ch != victim)
	        act("You enhance $N's ability to channel mana.",ch,0,victim,TO_CHAR);
        act("You feel energized, mana flowing more easily through your body.",victim,0,0,TO_CHAR);
        return;
}

void spell_synaptic_enhancement(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
        CHAR_DATA *victim = (CHAR_DATA *) vo;
        AFFECT_DATA af;

        if (is_affected(victim,sn))
                return send_to_char("They are already affected by synaptic enhancement.\n\r",ch);

	if (is_affected(victim,gsn_synaptic_impairment))
	{
		send_to_char("The haze lifts from your mind, and you can think clearly once again.\n\r",victim);
		act("$n appears to be thinking normally again.",victim,NULL,NULL,TO_ROOM);
		affect_strip(victim,gsn_synaptic_impairment);
		return;
	}

        init_affect(&af);
        af.where        = TO_AFFECTS;
        af.aftype       = get_spell_aftype(ch);
        af.type         = sn;
        af.level        = level;
        af.location     = 0;
        af.modifier     = 0;
        af.duration     = level/4;
		af.mod_name = MOD_CONC;
		affect_to_char(victim, &af);
        act("$n twitches as $e suddenly looks more aware.",victim,0,0,TO_ROOM);
        act("Your mind suddenly feels much clearer and your reflexes sharpen.",victim,0,0,TO_CHAR);
        return;
}

void spell_synaptic_impairment(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
        CHAR_DATA *victim = (CHAR_DATA *) vo;
        AFFECT_DATA af;

        if (is_affected(victim,sn))
                return send_to_char("They are already affected by synaptic impairment.\n\r",ch);

	if (saves_spell(level,victim,DAM_MENTAL))
		return send_to_char("You failed.\n\r",ch);

	if (is_affected(victim,gsn_synaptic_enhancement))
	{
		send_to_char("Your mind clouds slightly as your synaptic enhancement wears off.\n\r",victim);
		act("$n appears to be thinking normally again.",victim,NULL,NULL,TO_ROOM);
		affect_strip(victim,gsn_synaptic_enhancement);
		return;
	}

        init_affect(&af);
        af.where        = TO_AFFECTS;
        af.aftype       = get_spell_aftype(ch);
        af.type         = sn;
        af.level        = level;
        af.location     = 0;
        af.modifier     = 0;
        af.duration     = level/5;
	af.mod_name	= MOD_CONC;
        affect_to_char(victim, &af);
	act("You disrupt $N's ability to focus with a controlled electrical burst!",ch,0,victim,TO_CHAR);
	act("$n disrupts $N's ability to focus with a controlled electrical burst!",ch,0,victim,TO_NOTVICT);
        act("Your mind clouds and you find concentration somewhat more difficult.",ch,0,victim,TO_VICT);
        return;
}

void spell_elecshield(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
        AFFECT_DATA af;

        if (is_affected(ch,sn))
        	return send_to_char("You are already surrounded by a shield of electricity.\n\r",ch);

        init_affect(&af);
        af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
        af.type = sn;
        af.modifier = 0;
        af.level = level;
        af.duration = level / 4;
        af.location = 0;
	af.mod_name = MOD_PROTECTION;
        affect_to_char(ch,&af);

	send_to_char("You create an electrically-charged sphere around you.\n\r",ch);
	act("A crackling sphere of electricity briefly surrounds $n.",ch,0,0,TO_ROOM);
	return;
}

void spell_scramble_neurons( int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	if (is_affected(victim,sn))
		return send_to_char("Their neurons are already scrambled.\n\r",ch);
	if (saves_spell(level,victim,DAM_ENERGY))
		return send_to_char("Spell failed.\n\r",ch);

	init_affect(&af);
	af.where	= TO_AFFECTS;
	af.aftype	= get_spell_aftype(ch);
	af.type		= sn;
	af.level	= level;
	af.location	= 0;
	af.modifier	= 0;
	af.duration	= dice(4,2) + level/25;
	af.mod_name 	= MOD_CONC;
	affect_to_char(victim, &af);
	act("You disrupt $N's neurons!",ch,NULL,victim,TO_CHAR);
	act("You reel in confusion as an odd sensation muddies your thoughts.",ch,NULL,victim,TO_VICT);
	act("$n disrupts $N's neurons!",ch,NULL,victim,TO_NOTVICT);
	return;
}

void spell_mana_leech(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int drain;

	act("You extend tendrils of electricity towards $N.", ch, 0, victim, TO_CHAR);
	act("$n extends tendrils of electricity towards you.", ch, 0, victim, TO_VICT);
	act("$n extends tendrils of electricity towards $N.", ch, 0, victim, TO_NOTVICT);

	damage_new(ch,victim,dice(5,2),sn,DAM_ENERGY,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);

	if (saves_spell(level,victim,DAM_ENERGY))
	{
		act("You fail to siphon away any of $N's mana.", ch, 0, victim, TO_CHAR);
		act("You feel a brief tingling sensation, but it quickly dissipates.", ch, 0, victim, TO_VICT);
		return;
	}
	else
	{
		if (victim->mana <= 1)
			return;
		act("You siphon away $N's mana energy!", ch, 0, victim, TO_CHAR);
		act("You feel weak as mana energy is ripped from your body!", ch, 0, victim, TO_VICT);
		act("$N looks drained as the tendrils lash across his body!", ch, 0, victim, TO_NOTVICT);
		drain = dice(20,15);
		if ((victim->mana/2) < drain)
			drain = victim->mana/2;
		victim->mana -= drain;
		ch->mana += drain/2;
	}

	return;
}


void spell_interference(int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	AREA_AFFECT_DATA aaf;
	AREA_DATA *area;
	AFFECT_DATA af;
	char buf[MAX_STRING_LENGTH];

	area = ch->in_room->area;
	
	if(is_affected(ch,sn))
		return send_to_char("You cannot conjure more electrical interference yet.\n\r",ch);

	if(is_affected_area(area,gsn_interference))
		return send_to_char("This area is already blanketed by a veil of electrical interference.\n\r",ch);
	
	init_affect_area(&aaf);
	aaf.where       = TO_ROOM_AFFECTS;
	aaf.type        = gsn_interference;
	aaf.aftype      = AFT_SPELL;
	aaf.level       = level;
	aaf.duration    = dice(2,3)+5;
	aaf.location    = 0;
	aaf.modifier    = 0;
	aaf.owner       = ch;
	aaf.tick_fun    = NULL;
	aaf.end_fun     = interference_end;
	affect_to_area(area, &aaf);

	init_affect(&af);
	af.where		= TO_AFFECTS;
	af.type			= gsn_interference;
	af.aftype		= AFT_TIMER;
	af.duration		= 48;
	af.location		= 0;
	af.modifier 	= 0;
	af.owner		= ch;
	affect_to_char(ch,&af);
	
	send_to_char("You blanket the area with a veil of electrical interference.\n\r",ch);
	sprintf(buf,"{yA smell of ozone fills the %s as electricity crackles around you.{x",
		ch->in_room->sector_type == SECT_UNDERWATER ? "water" : "air");
	rarea_echo(ch->in_room,buf);
	return;
}

void interference_end(AREA_DATA *area, AREA_AFFECT_DATA *af)
{
	zone_echo(area,"{yThe electrical interference in the area subsides.{x");
	return;
}

void spell_hydroperception( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    AFFECT_DATA af;

    if (is_affected(ch, sn))
        return send_to_char("Your eyes are already accomodated to seeing in water.\n\r",ch);

    init_affect(&af);
    af.where     = TO_AFFECTS;
    af.aftype    = get_spell_aftype(ch);
    af.type      = sn;
    af.level     = level;
    af.duration  = level/2;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.mod_name	 = MOD_PERCEPTION;
	affect_to_char(ch, &af);
    send_to_char( "You focus your senses on changes in the water around you.\n\r", ch);
    return;
}


void spell_dehydrate(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;

	if (!IS_NPC(victim) && victim->pcdata->condition[COND_THIRST] > COND_HUNGRY)
	{
		act("$N is already too dehydrated to be affected.", ch, 0, victim, TO_CHAR);
		return;
	}

	act("You attempt to draw fluids from $N's body.", ch, 0, victim, TO_CHAR);
	act("A wave of nausea overcomes you as your gut clenches and sweat gushes from your pores.\n\rYou feel desperately thirsty.", ch, 0, victim, TO_VICT);
	act("$N doubles over in agony as rolls of sweat drip from $S body.", ch, 0, victim, TO_NOTVICT);

	if (IS_NPC(victim))
	{
	damage_new(ch,victim,dice(level,3),sn,DAM_INTERNAL,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);
	} else {
	damage_new(ch,victim,dice(COND_HUNGRY - victim->pcdata->condition[COND_THIRST],level / 5),sn,DAM_INTERNAL,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);
		if (victim->pcdata->condition[COND_THIRST] >= 0) {
			victim->pcdata->condition[COND_THIRST] = COND_HUNGRY;
		}
	}

	return;
}

void spell_drown(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int dam = dice(level + 10,4);

	act("You choke and gasp for air as your lungs fill with water!", victim, 0, 0, TO_CHAR);
	act("$n sputters and clutches $s chest as $s lungs fill with water!", victim, 0, 0, TO_ROOM);

	if (ch->in_room->sector_type == SECT_WATER || ch->in_room->sector_type == SECT_UNDERWATER)
		dam *=2;

	if (IS_AFFECTED(victim,AFF_WATERBREATH))
		dam /=2;

	damage_new(ch,victim,dam,sn,DAM_DROWNING,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);


	return;
}

void spell_hydration(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int heal = 0;
	OBJ_DATA *obj;
	bool spring = FALSE;

	for (obj = ch->in_room->contents; obj; obj = obj->next_content)
	{
		if (obj->item_type == ITEM_FOUNTAIN)
			spring = TRUE;
	}

	switch (ch->in_room->sector_type) {
		case (SECT_CITY):
			heal = dice(4,10); break;
		case (SECT_DESERT):
			heal = dice(4,10); break;
		case (SECT_ROAD):
			heal = dice(4,10); break;
		case (SECT_AIR):
			heal = dice(4,10); break;
		case (SECT_INSIDE):
			heal = dice(4,10); break;
		case (SECT_HILLS):
			heal = dice(6,10); if (spring) heal = (int)((float)heal * 1.5); break;
		case (SECT_MOUNTAIN):
			heal = dice(6,10); if (spring) heal = (int)((float)heal * 1.5); break;
		case (SECT_FIELD):
			heal = dice(8,10); if (spring) heal = (int)((float)heal * 1.5); break;
		case (SECT_FOREST):
			heal = dice(8,10); if (spring) heal = (int)((float)heal * 1.5); break;
		case (SECT_SWAMP):
			heal = dice(12,10);
		case (SECT_WATER):
			heal = dice(16,10); break;
		case (SECT_UNDERWATER):
			heal = dice(16,10); break;
		case (SECT_CONFLAGRATION):
			heal = 0; break;
		case (SECT_BURNING):
			heal = 0; break;
		default:
			heal = dice(4,10); break;
	}

	if (ch->in_room->area->sky == SKY_DRIZZLE && ch->in_room->sector_type != SECT_WATER && ch->in_room->sector_type != SECT_INSIDE && ch->in_room->sector_type != SECT_UNDERWATER)
		heal += dice(4,10);

	if (victim == ch)
		act("You draw upon the water in your surroundings to revitalize yourself.",ch,0,victim, TO_CHAR);
	else
		act("You draw upon the water in your surroundings to revitalize $N.",ch,0,victim,TO_CHAR);

	if (heal == 0) {
		act("The flames burn up all moisture around you!",victim,0,0,TO_CHAR);
	} else if (heal <= 30) {
		act("You feel slightly more vigorous as moisture bathes your skin.",victim,0,0,TO_CHAR);
	} else if (heal <= 80) {
		act("A renewed energy surges through your limbs as the healing waters wash over you.",victim,0,0,TO_CHAR);
	} else {
		act("You feel your wounds mending rapidly as the life-giving water fills you with vigor!",victim,0,0,TO_CHAR);
	}

	victim->hit = UMIN(victim->hit+heal,victim->max_hit);
	return;
}

void spell_regeneration(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	if(is_affected(victim, sn))
	{
		if(ch == victim)
			send_to_char("You can not aid your regeneration any further.\n\r", ch);
		else
			act("You can not aid $N's regeneration any further.", ch, 0, victim, TO_VICT);
		return;
	}
	
	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = sn;
	af.location = APPLY_NONE;
	af.modifier = 0;
	af.level = level;
	af.duration = level/3;
	af.mod_name = MOD_REGEN;
	affect_to_char(victim,&af);

	if (victim != ch)
		act("You harness the restorative powers of water to aid $N's regeneration.",ch,0,victim,TO_CHAR);
	else
		act("You harness the restorative powers of water to aid your regeneration.",ch,0,victim,TO_CHAR);

	send_to_char("A soothing coolness washes over you as you feel a surge of vitality.\n\r",victim);

	return;
}

void spell_watershield(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
	AFFECT_DATA af;

	if (is_affected(ch,gsn_watershield))
		return send_to_char("You are already surrounded by a shield of water.\n\r",ch);

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = gsn_watershield;
	af.modifier = 0;
	af.level = level;
	af.duration = level / 4;
	af.location = 0;
	af.mod_name = MOD_PROTECTION;
	affect_to_char(ch,&af);

	send_to_char("You form a shield of swirling water around you.\n\r",ch);
	act("A magical sphere of swirling water briefly surrounds $n.",ch,0,0,TO_ROOM);

	return;
}

void spell_flood(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	ROOM_INDEX_DATA *to_room;
	int door, duration;
	ROOM_AFFECT_DATA raf;
	ROOM_AFFECT_DATA *oaf;
	EXIT_DATA *pexit;
	char *direction;

	if (ch->in_room->sector_type != SECT_WATER && ch->in_room->sector_type != SECT_UNDERWATER) {
		send_to_char("There is no water here.\n\r",ch);
		return;
	}

	if (!str_cmp(target_name, "n") || !str_cmp(target_name, "north")) door = 0;
	else if (!str_cmp(target_name, "e") || !str_cmp(target_name, "east"))  door = 1;
	else if (!str_cmp(target_name, "s") || !str_cmp(target_name, "south")) door = 2;
	else if (!str_cmp(target_name, "w") || !str_cmp(target_name, "west"))  door = 3;
	else if (!str_cmp(target_name, "u") || !str_cmp(target_name, "up" ))   door = 4;
	else if (!str_cmp(target_name, "d") || !str_cmp(target_name, "down"))  door = 5;
	else return send_to_char("Flood the area in what direction?\n\r",ch);

	pexit = ch->in_room->exit[door];

	if (pexit != NULL && ((to_room = pexit->u1.to_room) != NULL)) {

		if (to_room->sector_type == SECT_CITY) {
			send_to_char("A magical barrier prevents you from driving the water in that direction.\n\r",ch);
			return;
		} else if (to_room->sector_type == SECT_WATER) {
			send_to_char("You don't need magic to flood that room.\n\r",ch);
			return;
		} else if (to_room->sector_type == SECT_UNDERWATER) {
			send_to_char("You don't need magic to flood that room.\n\r",ch);
			return;
		} else if (to_room->sector_type == SECT_AIR) {
			send_to_char("You struggle in vain to saturate the air with the surrounding water.\n\r",ch);
			return;
		}

		direction = (char*)flag_name_lookup(door,direction_table);
		act("You gather the water around you and direct it to overflow the bank to the $T!",ch,0,direction,TO_CHAR);

		if (to_room->people) {
			direction = (char*)flag_name_lookup(reverse_d(door),direction_table);
			act("A torrent of water rushes in from the $T, flooding the room!",to_room->people,0,direction,TO_ALL);
		}

		duration = 24;

		if (is_affected_room(ch->in_room,sn)) {
			for (oaf = ch->in_room->affected; oaf != NULL; oaf = oaf->next )
			{
				if (oaf->type == sn)
					break;
			}
			duration = UMAX(oaf->duration - 1,0);
		}

		init_affect_room(&raf);
		raf.where	= TO_ROOM_AFFECTS;
		raf.type	= sn;
		raf.aftype	= AFT_SPELL;
		raf.level	= level;
		raf.duration	= duration;
		raf.location	= APPLY_ROOM_SECT;
		raf.modifier	= SECT_WATER - to_room->sector_type;
		raf.owner	= ch;
		raf.end_fun	= flood_recede;
		raf.tick_fun	= NULL;
		new_affect_to_room(to_room,&raf);
	} else {
		send_to_char("There is nowhere for anything to go in that direction!\n\r",ch);
		return;
	}

	return;
}

void flood_recede(ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *af)
{
	if (room->people)
		act("The flood begins to recede, and before long no trace of the water remains.",room->people,NULL,NULL,TO_ALL);

	if (is_affected_room(room,gsn_riptide))
		affect_strip_room(room,gsn_riptide);

	return;
}


void spell_tidalwave(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	int door, i;
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	ROOM_AFFECT_DATA raf;
	AFFECT_DATA af;
	CHAR_DATA *vch, *v_next;
	char *direction;
	char buf[MAX_STRING_LENGTH];

	if (is_affected(ch,sn))
		return send_to_char("You are not yet ready to create another tidal wave.\n\r",ch);

	if (ch->in_room->sector_type != SECT_WATER) {
		send_to_char("You cannot create a tidal wave here.\n\r",ch);
		return;
	}

        if((door = direction_lookup(target_name)) == -1)
	 return send_to_char("Create a tidal wave in what direction?\n\r",ch);

	to_room = ch->in_room;

	pexit = ch->in_room->exit[door];

	if (pexit == NULL)
		return send_to_char("You cannot force any water in that direction.\n\r",ch);

	to_room = pexit->u1.to_room;

	for (i = 1; i < 5; i++)
	{
		if (to_room != NULL) {
			if (to_room->sector_type == SECT_WATER) {
				init_affect_room(&raf);
				raf.where	= TO_ROOM_AFFECTS;
				raf.type	= sn;
				raf.aftype	= AFT_SPELL;
				raf.level	= level;
				raf.duration	= -1;
				raf.location	= APPLY_ROOM_NONE;
				raf.modifier	= UMAX((i * 2) - 1,2);
				raf.owner	= ch;
				raf.end_fun	= NULL;
				raf.tick_fun	= NULL;
				new_affect_to_room(to_room,&raf);
				raf.location	= APPLY_ROOM_NOPE;
					if (i == 1)	raf.modifier	= 25;
				else if (i == 2)	raf.modifier	= 50;
				else if (i == 3)	raf.modifier	= 50;
				else if (i == 4)	raf.modifier	= 25;
				new_affect_to_room(to_room,&raf);
			} else if (to_room->sector_type == SECT_FIELD
					|| to_room->sector_type == SECT_FOREST
					|| to_room->sector_type == SECT_HILLS
					|| to_room->sector_type == SECT_MOUNTAIN
					|| to_room->sector_type == SECT_INSIDE
					|| to_room->sector_type == SECT_DESERT
					|| to_room->sector_type == SECT_ROAD) {
				init_affect_room(&raf);
				raf.where	= TO_ROOM_AFFECTS;
				raf.type	= sn;
				raf.aftype	= AFT_SPELL;
				raf.level	= level;
				raf.duration	= -1;
				raf.location	= APPLY_ROOM_NONE;
				raf.modifier	= UMAX((i * 2) - 1,2);
				raf.owner	= ch;
				raf.end_fun	= NULL;
				raf.tick_fun	= NULL;
				new_affect_to_room(to_room,&raf);
				raf.location	= APPLY_ROOM_NOPE;
					if (i == 1)	raf.modifier	= 10;
				else if (i == 2)	raf.modifier	= 20;
				else if (i == 3)	raf.modifier	= 40;
				else if (i == 4)	raf.modifier	= 40;
				new_affect_to_room(to_room,&raf);
				break;
			}
		}

		if (!to_room)
			break;

		pexit = to_room->exit[door];

		if (pexit == NULL)
			break;

		to_room = pexit->u1.to_room;
	}

	act("You gather the waters around you into a massive tidal wave!",ch,0,0,TO_CHAR);
	act("As $n gestures, a massive wave rises from the waters before him.",ch,0,0,TO_ROOM);
	direction = (char*)flag_name_lookup(door,direction_table);
	act("The tidal wave surges $T, leaving havoc in its wake!",ch,0,direction,TO_ALL);

	for (vch = ch->in_room->people; vch != NULL; vch = v_next)
	{
		v_next = vch->next_in_room;
		if (vch == ch) continue;
		if (!IS_NPC(vch) && is_safe_new(ch,vch,FALSE))
			continue;
		if (!IS_NPC(vch)) {
		sprintf(buf,"Help! I'm being drowned by %s's tidal wave!",PERS(ch,vch));
		do_myell(vch,buf,ch);
		}
		damage_new(ch,vch,dice(10,10),gsn_tidalwave,DAM_DROWNING,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"the tidal wave*");
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_TIMER;
	af.type = sn;
	af.modifier = 0;
	af.level = level;
	af.duration = 4;
	af.location = APPLY_NONE;
	affect_to_char(ch,&af);

	return;
}

void spell_riptide(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	ROOM_INDEX_DATA *room, *first_room = NULL, *second_room = NULL;
	AFFECT_DATA af;
	ROOM_AFFECT_DATA *raf, *fraf=NULL, nraf;

	if (is_affected(ch,sn))
	{
		send_to_char("You are not yet ready to create another riptide.\n\r",ch);
		return;
	}

	if (is_affected_room(ch->in_room,sn))
	{
		send_to_char("You cannot create a riptide here.\n\r",ch);
		return;
	}

	for (room = top_affected_room; room ; room = room->aff_next )
	{
		if (is_affected_room(room,sn))
		{
			for (raf = room->affected; raf != NULL; raf = raf->next)
			{
				if (raf->type == sn && raf->owner == ch && raf->location == APPLY_ROOM_NONE && raf->modifier == 1)
				{
					first_room = room;
					fraf = raf;
				}
				else if (raf->type == sn && raf->owner == ch && raf->location == APPLY_ROOM_NONE && raf->modifier == 2)
				{
					second_room = room;
				}
			}
		}
	}

	if (first_room)
	{
		if (ch->in_room->sector_type != SECT_WATER && ch->in_room->sector_type != SECT_UNDERWATER) {
			send_to_char("You cannot create a riptide here.\n\r",ch);
			return;
		}

		if (first_room->area != ch->in_room->area)
		{
			send_to_char("You cannot create this riptide far from the original.\n\r",ch);
			return;
		}

		if (first_room == ch->in_room)
		{
			send_to_char("You cannot create a riptide here.\n\r",ch);
			return;
		}

		act("You shape the water currents around you to flow into this area.",ch,0,0,TO_CHAR);

		init_affect(&af);
		af.where	= TO_AFFECTS;
		af.type		= sn;
		af.aftype	= AFT_TIMER;
		af.level	= level;
		af.duration	= 36;
		af.location	= 0;
		af.modifier	= 0;
		affect_to_char(ch,&af);

		init_affect_room(&nraf);
		nraf.where	= TO_ROOM_AFFECTS;
		nraf.type	= sn;
		nraf.aftype	= AFT_SPELL;
		nraf.level	= level;
		nraf.duration	= 24;
		nraf.location	= APPLY_ROOM_NONE;
		nraf.modifier	= 2;
		nraf.owner	= ch;
		nraf.end_fun	= riptide_two_end;
		nraf.tick_fun	= NULL;
		new_affect_to_room(ch->in_room,&nraf);

		fraf->duration = 24;

	}
	else if (second_room)
	{
		send_to_char("You cannot create another riptide!\n\r",ch);
	}
	else
	{
		if (ch->in_room->sector_type != SECT_WATER) {
			send_to_char("You cannot create a riptide here.\n\r",ch);
			return;
		}

		act("Faint eddies disturb the surface of the water as you create a dangerous riptide.",ch,0,0,TO_CHAR);
		act("The water near $n ripples slightly.",ch,0,0,TO_ROOM);
		init_affect_room(&nraf);
		nraf.where	= TO_ROOM_AFFECTS;
		nraf.type	= sn;
		nraf.aftype	= AFT_SPELL;
		nraf.level	= level;
		nraf.duration	= 4;
		nraf.location	= APPLY_ROOM_NONE;
		nraf.modifier	= 1;
		nraf.owner	= ch;
		nraf.end_fun	= riptide_one_end;
		nraf.tick_fun	= NULL;
		new_affect_to_room(ch->in_room,&nraf);
	}

	return;
}

void riptide_one_end(ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *af)
{

	act("Your control over your riptide's water currents wanes.",af->owner,NULL,NULL,TO_CHAR);

	if (room->people)
		act("The ripples in the water cease.",room->people,NULL,NULL,TO_ALL);

	return;
}

void riptide_two_end(ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *af)
{
	if (room->people)
		act("The ripples in the water cease.",room->people,NULL,NULL,TO_ALL);

	return;
}

int average_ac(CHAR_DATA *ch)
{
    int i, avg = 0;
    for(i=0; i<3; i++)
	avg+=ch->armor[i];
    return avg / 3;
}

void spell_watermeld(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;

	if (!is_affected(ch,sn))
	{
		if (ch->in_room->sector_type != SECT_WATER && ch->in_room->sector_type != SECT_UNDERWATER)
			return send_to_char("You cannot watermeld here.\n\r",ch);

		init_affect(&af);
		af.where	= TO_AFFECTS;
		af.type	= sn;
		af.aftype	= AFT_SPELL;
		af.level	= level;
		af.duration	= level/5;
		af.location	= 0;
		af.modifier	= 0;
		af.owner	= NULL;
		af.end_fun	= NULL;
		af.mod_name = MOD_VISIBILITY;
		new_affect_to_char(ch,&af);

		act("You blend into the water around you and fade from sight.",ch,NULL,NULL,TO_CHAR);
		act("$n blends into the water around $m until $e can no longer be seen.",ch,NULL,NULL,TO_ROOM);
	}
	else
		send_to_char("You are already melded into the water.\n\r",ch);

	return;
}

void spell_sense_disturbance( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
	AFFECT_DATA af;

	if (is_affected(ch, sn))
		return send_to_char("You already sense disturbances in the air.\n\r",ch);

	init_affect(&af);
	af.where     = TO_AFFECTS;
	af.aftype    = get_spell_aftype(ch);
	af.type      = sn;
	af.level     = level;
	af.duration  = level/2;
	af.location  = APPLY_NONE;
	af.modifier  = 0;
	af.mod_name = MOD_PERCEPTION;
	affect_to_char(ch, &af);
	send_to_char("You attune yourself to notice disturbances in the air around you.\n\r", ch);
	return;
}

void spell_travelease( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
	AFFECT_DATA af;
 	CHAR_DATA *gch;

	for (gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room)
	{
		if (!is_same_group(gch, ch))
			continue;

		if (is_affected(gch, sn))
			continue;

		init_affect(&af);
		af.where     = TO_AFFECTS;
		af.aftype    = AFT_SPELL;
		af.type      = sn;
		af.level     = level;
		af.duration  = level/2;
		af.location  = APPLY_NONE;
		af.modifier  = 0;
		af.mod_name = MOD_MOVEMENT;
		affect_to_char(gch, &af);
		send_to_char("A gentle tailwind touches your back and movement becomes less of a chore.\n\r", gch);
	}
	send_to_char("Ok.\n\r",ch);

	return;
}

void spell_diffusion(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	if (!IS_AFFECTED(victim,AFF_PASS_DOOR))
	{
		init_affect(&af);
		af.where	= TO_AFFECTS;
		af.type		= sn;
		af.aftype	= AFT_SPELL;
		af.level	= level;
		af.duration	= level/3;
		af.location	= 0;
		af.modifier	= 0;
		SET_BIT(af.bitvector, AFF_PASS_DOOR);
		af.owner	= NULL;
		af.end_fun	= NULL;
		act("An odd tingling washes over you as your density is decreased to the point of translucence.",victim,NULL,NULL,TO_CHAR);
		act("$n's body seems to stretch slightly as $e becomes translucent!",victim,NULL,NULL,TO_ROOM);
		af.mod_name = MOD_PHASE;
		new_affect_to_char(victim,&af);
		if (ch != victim)
			send_to_char("Ok.\n\r",ch);

		return;
	}
	else
	{
		if (victim == ch)
			send_to_char("You are already translucent.\n\r",ch);
		else
			send_to_char("They are already translucent.\n\r",ch);

		return;
	}

	return;
}


void spell_disruption(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int dam = dice(level,5) + 20;


	if (saves_spell(level,victim,DAM_INTERNAL))
		dam /= 2;

	act("You concentrate and briefly disrupt the molecular structure of $N's organs!",ch,0,victim,TO_CHAR);
	act("$n extends a hand towards you and you feel a horrible rending pain deep within you!",ch,0,victim,TO_VICT);
	act("$n extends a hand towards $N, who lurches in agony despite no visible injury!",ch,0,victim,TO_NOTVICT);

	damage_new(ch,victim,dam,sn,DAM_INTERNAL,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);

	return;
}

void spell_anchor(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *anchor, *wch, *wch_next, *oldanchor = NULL;

	if (ch->in_room->sector_type == SECT_INSIDE
		|| ch->in_room->sector_type == SECT_UNDERWATER
		|| IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL))
	{
		send_to_char("You cannot create an anchor here.\n\r",ch);
		return;
	}

	for (wch = char_list; wch != NULL; wch = wch_next)
	{
		wch_next = wch->next;
		if (IS_NPC(wch) && wch->pIndexData->vnum == MOB_VNUM_ANCHOR && wch->hunting == ch)
		{
			oldanchor = wch;
			break;
		}
	}

	if (oldanchor)
	{
		act("$n dissipates harmlessly.",oldanchor,0,0,TO_ROOM);
		extract_char(oldanchor,TRUE);
	}

	anchor = create_mobile(get_mob_index(MOB_VNUM_ANCHOR));
	char_to_room(anchor,ch->in_room);
	anchor->level = level;
	anchor->hunting = ch;

	act("You harness the energy in the surrounding air to anchor your essence to this spot.",ch,0,0,TO_CHAR);
	act("$n concentrates intently, and a small funnel cloud begins to spin in place beside $m.",ch,0,0,TO_ROOM);

	return;
}

void spell_aerial_transferrence(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *anchor = NULL, *wch, *wch_next;
	ROOM_INDEX_DATA *pRoomIndex;

	if (IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL) ||
		ch->in_room->sector_type == SECT_INSIDE ||
		ch->in_room->sector_type == SECT_UNDERWATER)
	{
		send_to_char("You cannot cast that here.\n\r",ch);
		return;
	}

	if (is_affected(ch,gsn_bind_feet))
		return send_to_char("Your feet are rooted to the ground!\n\r",ch);
	
	for (wch = char_list; wch != NULL; wch = wch_next)
	{
		wch_next = wch->next;
		if (IS_NPC(wch) && wch->pIndexData->vnum == MOB_VNUM_ANCHOR && wch->hunting == ch)
		{
			anchor = wch;
			break;
		}
	}

	if (anchor)
	{
		act("As you call upon the power of the winds, a large funnel cloud materializes to carry you off into the air!",ch,0,0,TO_CHAR);
		act("A large funnel cloud materializes out of nowhere and carries $n soaring into the sky!",ch,0,0,TO_ROOM);
		char_from_room(ch);
		char_to_room(ch, anchor->in_room);
		act("You soar through the air and are soon set down beside your anchor.",ch,0,0,TO_CHAR);
		act("$n suddenly drops gently to the ground, beside the small funnel cloud.",ch,0,0,TO_ROOM);
		do_look(ch, "auto");
		check_plasma_thread(ch,-1);
	}
	else
	{
		for ( ; ; )
		{
			pRoomIndex = get_random_room(ch);
			if (pRoomIndex->sector_type == SECT_CITY || pRoomIndex->sector_type == SECT_INSIDE || pRoomIndex->sector_type == SECT_WATER || pRoomIndex->sector_type == SECT_UNDERWATER)
				continue;
			if (!IS_SET(pRoomIndex->room_flags, ROOM_NO_RECALL) && !IS_EXPLORE(pRoomIndex) && !pRoomIndex->cabal)
				break;
		}
		act("As you call upon the power of the winds, a large funnel cloud materializes to carry you off into the air!",ch,0,0,TO_CHAR);
		act("A large funnel cloud materializes out of nowhere and carries $n soaring into the sky!",ch,0,0,TO_ROOM);
		char_from_room(ch);
		char_to_room(ch, pRoomIndex);
		act("Without an anchor to secure you, the winds fling you haphazardly through the sky! ",ch,0,0,TO_CHAR);
		act("$n plummets suddenly from the sky, hitting the ground hard!",ch,0,0,TO_ROOM);
		do_look(ch, "auto");
		damage_new(ch,ch,dice(10,10),sn,DAM_BASH,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);
		check_plasma_thread(ch,-1);
	}

	return;
}

void spell_airshield(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
	AFFECT_DATA af;

	if (is_affected(ch,gsn_airshield))
		return send_to_char("You are already surrounded by a shield of air.\n\r",ch);

	if (ch->in_room->sector_type == SECT_UNDERWATER)
		return send_to_char("You can't cast that here.\n\r",ch);

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = gsn_airshield;
	af.modifier = 0;
	af.level = level;
	af.duration = level / 4;
	af.location = 0;
	af.mod_name = MOD_PROTECTION;
	affect_to_char(ch,&af);

	send_to_char("The air begins to swirl rapidly around you.\n\r",ch);
	act("$n raises $s arms, and swirling winds surround $m.",ch,0,0,TO_ROOM);

	return;
}

/* Earth spells */

void spell_hardenfist(int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	AFFECT_DATA af;
	if (!is_affected(ch,sn))
	{
		init_affect(&af);
		af.where	= TO_AFFECTS;
		af.type	= sn;
		af.aftype	= AFT_SPELL;
		af.level	= level;
		af.duration	= level/3;
		af.location	= 0;
		af.modifier	= 0;
		af.owner	= NULL;
		af.end_fun	= NULL;
		new_affect_to_char(ch,&af);

		act("$n clenches $s fists tightly as the flesh seems to take on a stony pallor.",ch,0,0,TO_ROOM);
		act("As you clench your fist, it becomes as hard as granite.",ch,0,0,TO_CHAR);
	}
	else {
		send_to_char("Your fists are already as hard as granite.\n\r",ch);
		return;
	}
}

void spell_stability(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
        AFFECT_DATA af;

        if (is_affected(ch,sn))
        {
         send_to_char("You are already manipulating your own mass.\n\r",ch);
         return;
        }

        init_affect(&af);
        af.where = TO_AFFECTS;
        af.aftype = AFT_SPELL;
        af.location = 0;
        af.modifier = 0;
        af.duration = level/4;
        af.type = sn;
        af.level = level;
		af.mod_name = MOD_LEVITATION;
		affect_to_char(ch,&af);
        act("$n suddenly seems much more sure of $s balance.",ch,0,0,TO_ROOM);
        send_to_char("You focus on manipulating your own mass, steadying your balance.\n\r",ch);
        return;
}

void spell_crush(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice(level,3) + 20;

    if (saves_spell(level,victim,DAM_BASH))
       dam /= 2;

    act("You concentrate and briefly disrupt the molecular structure of $N's organs!",ch,0,victim,TO_CHAR);
    act("$n extends a hand towards you and you feel a horrible rending pain deep within you!",ch,0,victim,TO_VICT);
    act("$n extends a hand towards $N, who lurches in agony despite no visible injury!",ch,0,victim,TO_NOTVICT);

    damage_new(ch,victim,dam,sn,DAM_INTERNAL,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"crushing force");

    return;
}

void spell_sensevibrations(int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	AFFECT_DATA af;
	if (!is_affected(ch,sn))
	{
		init_affect(&af);
		af.where	= TO_AFFECTS;
		af.type	= sn;
		af.aftype	= AFT_SPELL;
		af.level	= level;
		af.duration	= level/2;
		af.location	= 0;
		af.modifier	= 0;
		af.owner	= NULL;
		af.end_fun	= NULL;
		af.mod_name = MOD_PERCEPTION;
		new_affect_to_char(ch,&af);

		act("You attune your senses to the vibrations of the ground beneath you.",ch,0,0,TO_CHAR);
	}
	else {
		send_to_char("You are already attuned to the vibrations of the ground.\n\r",ch);
		return;
	}
}

void spell_diamondskin(int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	AFFECT_DATA af;
  if(!(ch->hit < ch->max_hit*.75)) {
	if (!is_affected(ch,sn))
	{
		init_affect(&af);
		af.where	= TO_AFFECTS;
		af.type	= sn;
		af.aftype	= AFT_SPELL;
		af.level	= level+8;
		af.duration	= 18 - level/5;
		af.location	= 0;
		af.owner	= NULL;
		af.end_fun	= NULL;
		af.modifier = (dice(5, 20) + 100) * 20;
		af.mod_name = MOD_PROTECTION;
		new_affect_to_char(ch,&af);

		act("$n concentrates, and $s skin hardens into a diamond-like carapace.", ch, 0, 0, TO_ROOM);
		act("The outer layers of your skin become almost impossibly hard.",ch,0,0,TO_CHAR);
		if (is_affected(ch, gsn_stoneskin))
		    affect_strip(ch, gsn_stoneskin);
		if (is_affected(ch, gsn_ironskin))
			affect_strip(ch,gsn_ironskin);
	}
	else {
		send_to_char("Your skin is already as hard as it can become.\n\r",ch);
		return;
	}
  } else {
	return send_to_char("You are bleeding too much to harden your skin.\n\r",ch);
  }
}

void spell_overbear (int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int str = 0;
	int chance = 0;

	if (victim->position <= POS_RESTING)
		return send_to_char("They are not on their feet to begin with.",ch);
	
	str = get_curr_stat(ch,STAT_STR);
	
	switch (str) {
		case(14):	chance 	=	95;	break;
		case(15):	chance	=	85; break;
		case(16):	chance	=	75; break;
		case(17):	chance	=	65; break;
		case(18):	chance	=	50;	break;
		case(19):	chance	=	40; break;
		case(20):	chance	=	35;	break;
		case(21):	chance	=	25;	break;
		case(22):	chance	=	20;	break;
		case(23):	chance	=	15; break;
		case(24):	chance 	=	 5; break;
	}	

	if (str >= 25)
		chance = 0;
	if (str <= 13)
		chance = 100;

	if (number_percent() >= chance) {
		act("An invisible weight presses down upon you, but you shrug it off.",victim,0,0,TO_CHAR);
		act("$n staggers slightly for a moment, but maintains $s balance.",victim,0,0,TO_ROOM);
		return;
	} else {
		act("An invisible weight bears down upon you, forcing you to your knees!",victim,0,0,TO_CHAR);
		act("$n loses his footing, staggering under an unseen burden!",victim,0,0,TO_ROOM);
		damage_new(ch,victim,dice(2,6),sn,DAM_BASH,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"overbearing force");
		victim->position = POS_RESTING;
		LAG_CHAR(victim,2 * PULSE_VIOLENCE);
	}
	
	return;
}

void spell_reduce(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{

	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	char buf[MSL];
		
	if(is_affected(victim,gsn_reduce) || victim->size<=SIZE_TINY) {
		if(victim != ch)
			return send_to_char("They are already reduced.\n\r",ch);
		else
			return send_to_char("You are already reduced.\n\r",ch);
	}

	if(!trusts(ch,victim) && is_safe(ch,victim))
		return;
	
	if (is_affected(victim,gsn_enlarge)) {
		send_to_char("You shrink down to your normal size.\n\r",victim);
		act("$n shrinks down to $s normal size.",victim,0,0,TO_ROOM);
		affect_strip(victim,gsn_enlarge);
		return;
	}
        init_affect(&af);
        af.where        = TO_AFFECTS;
        af.aftype       = AFT_SPELL;
        af.type         = gsn_reduce;
        af.level        = level;
        af.duration     = level/4;
        af.location     = APPLY_SIZE;
        af.modifier     = -1;
        affect_to_char(victim,&af);
	af.aftype	= AFT_INVIS;
        af.location     = APPLY_DAM_MOD;
        af.modifier     = -20;
        affect_to_char(victim,&af);

        send_to_char("Your entire body suddenly contracts, leaving you significantly smaller but somehow sturdier.\n\r",victim);
        act("$n rapidly shrinks to two-thirds of $s original size!",victim,0,0,TO_ROOM);
        if(!trusts(ch,victim))
        {
                sprintf(buf,"Die, %s, you sorcerous dog!",PERS(ch,victim));
                do_myell(victim,buf,ch);
                multi_hit(victim,ch,TYPE_UNDEFINED);
        }
        return;
}

void spell_earthshield(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
        AFFECT_DATA af;

        if (is_affected(ch,gsn_earthshield))
                return send_to_char("You are already surrounded by a shield of earth.\n\r",ch);
        init_affect(&af);
        af.where = TO_AFFECTS;
        af.aftype = AFT_SPELL;
        af.type = gsn_earthshield;
        af.modifier = 0;
        af.level = level;
        af.duration = level / 4;
        af.location = 0;
		af.mod_name = MOD_PROTECTION;
		affect_to_char(ch,&af);
        send_to_char("You ready a shield of solid matter to deflect the bodies of your foes.\n\r",ch);
        act("$n's form is suddenly masked by an opaque gray shield that vanishes as quickly as it appeared.",ch,0,0,TO_ROOM);
}

void spell_coldshield(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
        AFFECT_DATA af;

        if (is_affected(ch,sn))
                return send_to_char("You are already surrounded by a shield of cooled air.\n\r",ch);
        init_affect(&af);
        af.where = TO_AFFECTS;
        af.aftype = AFT_SPELL;
        af.type = sn;
        af.modifier = 0;
        af.level = level;
        af.duration = level / 4;
        af.location = 0;
		af.mod_name = MOD_PROTECTION;
        affect_to_char(ch,&af);
        send_to_char("The air around you rapidly chills.\n\r",ch);
        act("The air around $n suddenly turns frigid.",ch,0,0,TO_ROOM);

	return;
}

void spell_coagulate( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
	CHAR_DATA *victim = get_char_room(ch, target_name);
	int dam;
	if(!str_cmp(target_name,"")) {
		if (ch->fighting != NULL) {
			victim = ch->fighting;
		} else {
			send_to_char("On whom?\n\r",ch);
			return;
		}
	}
	if(!victim)
		return send_to_char("They aren't here.\n\r",ch);
	if(!trusts(ch,victim) && is_safe(ch,victim))
		return;

	dam = dice(5,4) + level;

	act("You draw heat out of $N's bloodstream.",ch,0,victim,TO_CHAR);
	act("Your blood chills as $n extends a hand towards you.",ch,0,victim,TO_VICT);
	if(
	damage_new(ch,victim,dam,TYPE_UNDEFINED,DAM_COLD,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"coagulation") == -1)
		return;
	
	if (is_affected(victim, gsn_bleeding)) {
		act("$N's blood congeals, and $S bleeding stops.",ch,0,victim,TO_CHAR);
		act("You shiver slightly from the cold, but your bleeding stops.",ch,0,victim,TO_VICT);
		affect_strip(victim,gsn_bleeding);
	}

	return;
}

void spell_hypothermia( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_SLEEP)
    ||   (IS_NPC(victim) && IS_SET(victim->act,ACT_UNDEAD))
    ||   saves_spell(level-1, victim,DAM_COLD)
    ||   IS_SET(victim->imm_flags,IMM_SLEEP))
        {
        act("$N resisted your hypothermia spell.",ch,0,victim,TO_CHAR);
        return;
        }

    init_affect(&af);
    af.where     = TO_AFFECTS;
    af.aftype    = AFT_SPELL;
    af.type      = sn;
    af.level     = level;
    af.duration  = level/6;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
	SET_BIT(af.bitvector, AFF_SLEEP);
	af.mod_name = MOD_CONC;
	affect_join( victim, &af );

    if ( IS_AWAKE(victim) )
    {
		if (is_affected(victim,gsn_indom)) {
			act("$N's body's state is maintained by the force of $S will.",ch,0,victim,TO_CHAR);
			return;
		}
        send_to_char( "A sudden wave of cold shocks you, and then your consciousness slips away.\n\r", victim );
        act( "$n is overcome with a sudden fit of shivering and slumps to the ground.", victim, NULL, NULL, TO_ROOM );
        victim->position = POS_SLEEPING;
    }
    return;
}

void spell_imprisonvoice( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
        CHAR_DATA *victim = (CHAR_DATA *) vo;
        AFFECT_DATA af;

        if (ch == victim)
        {
        send_to_char("Imprison whose voice?\n\r",ch);
        return;
        }

        if (is_affected(victim,sn))
        {
        act("$N's voice is already imprisoned.",ch,NULL,victim,TO_CHAR);
        return;
        }

        if (saves_spell(level-1,victim, DAM_COLD))
        return send_to_char("They resisted your spell.\n\r",ch);;

        init_affect(&af);
        af.where     = TO_AFFECTS;
	af.aftype     = AFT_SPELL;
        af.type      = sn;
        af.level     = level;
        af.duration  = level/10;
        af.modifier  = 0;
        af.location  = 0;
		af.mod_name = MOD_SPEECH;
        affect_to_char(victim,&af);

        act("You freeze $N's vocal cords, rendering speech impossible.",ch,NULL,victim,TO_CHAR);
        act_new("Your throat constricts painfully as your vocal cords are frozen solid.",ch,NULL,victim,TO_VICT,POS_SLEEPING);

    return;
}

void spell_frigidaura(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
        AFFECT_DATA af;

        if (is_affected(ch,gsn_frigidaura))
        	return send_to_char("You are already surrounded by an aura of cold.\n\r",ch);
	init_affect(&af);
	af.where	= TO_RESIST;
	af.type		= sn;
	af.aftype	= AFT_SPELL;
	af.location	= 0;
	af.modifier	= 0;
	af.level	= level;
	af.duration	= level / 3;
	af.mod_name = MOD_RESISTANCE;
	SET_BIT(af.bitvector, RES_COLD);
	affect_to_char(ch,&af);
	af.where	= TO_VULN;
	zero_vector(af.bitvector);
	SET_BIT(af.bitvector, VULN_FIRE);
	affect_to_char(ch,&af);
	act("You chill the air around you, prepared to lash out at any who come near.",ch,0,0,TO_CHAR);

	act("$n is briefly surrounded by an aura of ice, which rapidly fades away.",ch,0,0,TO_ROOM);

	return;
}

void spell_enervate_agitate_helper(int sn, int level, CHAR_DATA *ch, void *vo, int target, int iDir)
{
	CHAR_DATA *victim = get_char_room(ch,target_name);
	AFFECT_DATA af;
	int dam, dmod, es;
	
	if(!victim || !strcmp(target_name,""))
		return send_to_char("You don't see them here.\n\r",ch);
	
	if(IS_NPC(victim))
		return act("You lack the necessary understanding of $N's structure to accomplish that.",ch,0,victim,TO_CHAR);
	
	if(is_safe(ch,victim))
		return;

	es = victim->pcdata->energy_state + iDir;

	if (!trusts(ch,victim) || abs(es) > 1 || !((victim->pcdata->energy_state < -1 && iDir > 0) || (victim->pcdata->energy_state > 1 && iDir < 0)))
	{
		if(saves_spell(level,victim,(sn == gsn_agitate) ? DAM_FIRE: DAM_COLD))
		{
			if (iDir > 0)
			    act("You failed to accelerate the particles of $N.",ch,0,victim,TO_CHAR);
			else
			    act("You failed to decelerate the particles of $N.",ch,0,victim,TO_CHAR);
			multi_hit(victim,ch,TYPE_UNDEFINED);
			return;
		}
	}

	if ((iDir > 0) && (victim->pcdata->energy_state>4))
		return send_to_char("They're already as good as dead.\n\r",ch);
	
	else if ((iDir < 0) && (victim->pcdata->energy_state<-4))
		return send_to_char("They're already frozen solid.\n\r",ch);
	
	if (iDir > 0)
	{
	    act("You infuse the particles of $N's body with kinetic energy.",ch,0,victim,TO_CHAR);
	    act("$n focuses intense energy upon $N's body!",ch,0,victim,TO_NOTVICT);
	}
	else
	{
	    act("Drawing heat from $N's body in large quantity, you slow down $s particles.",ch,0,victim,TO_CHAR);
	    act("$n draws energy from $N's body!",ch,0,victim,TO_NOTVICT);
	}
	
	if (iDir > 0) {
	    switch(es) {
			case 1:
				send_to_char("Burning pain wracks your body, but when it subsides you feel infused with energy!\n\r",victim);
				break;
			case 2:
				send_to_char("Your body is again wracked with searing pain, but this time it fails to subside!\n\r",victim);
				break;
			case 3:
				send_to_char("You stagger as the heat intensifies, your every motion increasing the agony that consumes you!\n\r",victim);
				break;
			case 4:
				send_to_char("You stumble, dazed, as your body feels on the verge of being torn apart!\n\r",victim);
				break;
			case 5:
				send_to_char("A moment of detached calm washes over you. You can no longer feel anything, but you dimly realize you are about to die.\n\r",victim);
				break;
			default:
				act("$n infuses your body with powerful heat energy!",ch,0,victim,TO_VICT);
				break;
	    }
	} else {
	    switch(es) {
			case -1:
				send_to_char("You shiver as you suddenly feel cold all over, but when it passes, your reflexes feel sharper!\n\r",victim);
				break;
			case -2:
				send_to_char("The painful shock of extreme cold returns, this time leaving your body feeling numb and unwieldy.\n\r",victim);
				break;
			case -3:
				send_to_char("The cold returns, somehow less painful than last time, but every movement is now a chore.\n\r",victim);
				break;
			case -4:
				send_to_char("You are distantly aware that you are still in danger, but the numbing cold saps your resistance.\n\r",victim);
				break;
			case -5:
				send_to_char("Your body grinds to a halt as you lose control of the last of your mobility.\n\r",victim);
				break;
			default:
				act("$n infuses your body with frigid cold energy!",ch,0,victim,TO_VICT);
			break;
	    }
	}
	
	if (is_affected(victim,gsn_enervate))
	    affect_strip(victim, gsn_enervate);

	if (is_affected(victim,gsn_agitate))
	    affect_strip(victim, gsn_agitate);

	victim->pcdata->energy_state = 0;
	
	if (es != 0)  {
	    init_affect(&af);
	    af.where	= TO_AFFECTS;
	    af.aftype	= AFT_SPELL;
	    if (es > 0)
	        af.type	= gsn_agitate;
	    else
			af.type	= gsn_enervate;
	    af.level	= level;
	    af.duration	= 5;
	    af.location	= APPLY_ENERGYSTATE;
	    af.modifier	= es;
		af.mod_name = MOD_ENERGY_STATE;
		af.end_fun	= NULL;
		if (es > 0)
			af.tick_fun = agitate_tick;
		else
			af.tick_fun = NULL;
	    af.owner	= ch;
	    if(victim->pcdata->energy_state==1)
			SET_BIT(af.bitvector, AFF_HASTE);
	    new_affect_to_char(victim, &af);
	}

	if (es < -1) {
	    init_affect(&af);
	    af.where	= TO_AFFECTS;
	    af.aftype	= AFT_SPELL;
	    af.type		= gsn_enervate;
	    af.level	= level;
	    af.duration	= 5;
	    af.location	= APPLY_DEX;
	    af.modifier	= -4 + (es+2)*2;
	    af.end_fun	= NULL;
	    af.owner	= ch;
	    new_affect_to_char(victim, &af);
	}

	dmod = abs(es) == 1 ? 5 : abs(es) == 2 ? 10 : abs(es) == 3 ? 20 : abs(es) == 4 ? 40 : abs(es) == 5 ? 40 : 0;
	dam = dice(dmod, 10);
	if (dmod) {
	    if ((es > 0) && (sn == gsn_agitate))
		damage_new(ch,victim,dam,TYPE_UNDEFINED,DAM_FIRE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"agitation");
	    else if ((es < 0) && (sn == gsn_enervate))
		damage_new(ch,victim,dam,TYPE_UNDEFINED,DAM_COLD,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"enervation");
	}

	if(trusts(ch,victim) && abs(es) < 2)
	{
        if (ch->fighting == victim)
                stop_fighting(ch,FALSE);
        if (victim->fighting == ch)
                stop_fighting(victim,FALSE);
	}
}

void spell_enervate(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    spell_enervate_agitate_helper(sn, level, ch, vo, target, -1);
}

void spell_agitate(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    spell_enervate_agitate_helper(sn, level, ch, vo, target, 1);
}

void agitate_tick (CHAR_DATA *ch, AFFECT_DATA *af)
{
	int dam;
	
	if(IS_NPC(ch))
		return;

	if (ch->pcdata->energy_state > 4) {
		act("$n collapses in a heap, wisps of smoke rising from $s charred corpse.",ch,0,0,TO_ROOM);
		send_to_char("You crumple to the ground, your consciousness fading away, as your body collapses under the heat.\n\r",ch);
		if (af->owner)
			raw_kill(af->owner,ch);
		else
			raw_kill(ch,ch);
		
		return;
	}

	if (ch->pcdata->energy_state > 1) {
		send_to_char("Your body is wracked by searing pains!\n\r",ch);
		if (!af->owner)
			af->owner = ch;
		dam = ch->pcdata->energy_state;
		dam = dice(dam == 2 ? 5 : dam == 3 ? 10 : dam == 4 ? 20 : 5, 10);
		damage_new(af->owner,ch,dam,gsn_agitate,DAM_FIRE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"agitation");
	}
	
	return;
}

void spell_freezemetal(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    OBJ_DATA *obj;
    int iWear, highchance, crushdam, piercedam;

    act("The air around you suddenly turns ice cold.",ch,0,victim,TO_VICT);
    act("You attempt to siphon heat away from $N's armor.",ch,0,victim,TO_CHAR);
    act("$n extends a hand toward $N, as a chill fills the air.",ch,0,victim,TO_NOTVICT);

    crushdam = 0;
    piercedam = 0;

    for ( iWear = 0; iWear < MAX_WEAR; iWear++ )
    {
        if (( ( obj = get_eq_char( victim, iWear ) ) == NULL ) ||
	    (iWear == WEAR_WIELD) ||
	    (iWear == WEAR_DUAL_WIELD))
            continue;
	if(IS_OBJ_STAT(obj, ITEM_BURN_PROOF))
	    continue;
	if (obj->pIndexData->material_index > 0)
	    highchance = material_table[obj->pIndexData->material_index].mat_conductivity * 20;
	else
	    highchance = 0;

	if (number_range(0, 100) >= highchance)
	    continue;

	act_new("Your $p contracts due to the cold, crushing you painfully!",ch,obj,victim,TO_VICT,POS_DEAD);
	act("$N's $p contracts, crushing him painfully!",ch,obj,victim,TO_CHAR);
	act("$N's $p contracts, crushing him painfully!",ch,obj,victim,TO_NOTVICT);
	crushdam += dice(level/10,7);

	if (obj->pIndexData->material_index > 0)
	    highchance = material_table[obj->pIndexData->material_index].mat_conductivity * 4;
	else
	    highchance = 0;

	if (number_range(0, 100) >= highchance)
	    continue;

	act_new("Your $p shatters violently, piercing you with countless shards!",ch,obj,victim,TO_VICT,POS_DEAD);
	act("$N's $p shatters violently due to the cold!",ch,obj,victim,TO_CHAR);
	act("$N's $p shatters violently due to the cold!",ch,obj,victim,TO_NOTVICT);
	piercedam += dice(level/5,9);

	extract_obj(obj);
    }
    if (crushdam > 0)
	damage_new(ch,victim,crushdam,sn,DAM_BASH,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"the contracting armor*");
    if (piercedam > 0)
	damage_new(ch,victim,piercedam,sn,DAM_PIERCE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"the metal shards*$");

    return;
}


void spell_frostbite(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af, *paf;

	if(saves_spell(level,victim,DAM_COLD))
		return send_to_char("You failed.\n\r",ch);

	if (number_range(0,1) == 0)
	{
		if (is_affected(victim,sn) && victim->arms < 2)
			return send_to_char("There is no warmth remaining in their arm to be siphoned away.\n\r",ch);

		act("$N's arm goes limp as you draw heat from it.",ch,0,victim,TO_CHAR);
		act_new("Your arm goes limp as $n draws heat from it.",ch,0,victim,TO_VICT,POS_SLEEPING);
		act("$N's arm goes limp as $n draws heat from it.",ch,0,victim,TO_NOTVICT);

		for (paf = victim->affected; paf != NULL; paf = paf->next)
		{
			if (paf->type == sn && paf->location == APPLY_STR)
				affect_remove(victim,paf);			
		}

		init_affect(&af);
		af.where = TO_AFFECTS;
		af.aftype = AFT_MALADY;
		af.type = sn;
		af.location = APPLY_STR;
		af.duration = level/6;
		af.level = level;
		af.modifier = -(level/5);
		affect_to_char(victim,&af);

		if(!saves_spell(level-2,victim,DAM_COLD))
		{
			act("$N's screams in pain as $S arm, now a pale bluish-white color, dangles uselessly at $S side!",ch,0,victim,TO_CHAR);
			act_new("Shooting pains run through your arm as $n chills it, and then suddenly, you have no feeling in it at all.",ch,0,victim,TO_VICT,POS_SLEEPING);
			act("$N's screams in pain as $S arm, now a pale bluish-white color, dangles uselessly at $S side!",ch,0,victim,TO_NOTVICT);
			init_affect(&af);
			af.where = TO_AFFECTS;
			af.aftype = AFT_INVIS;
			af.type = sn;
			af.location = APPLY_ARMS;
			af.duration = level/6;
			af.level = level;
			af.modifier = -1;
			affect_to_char(victim,&af);
		}
	}
	else
	{
		if (is_affected(victim,sn) && victim->legs < 2)
			return send_to_char("There is no warmth remaining in their leg to be siphoned away.\n\r",ch);

		act("$N's leg goes limp as you draw heat from it.",ch,0,victim,TO_CHAR);
		act_new("Your leg goes limp as $n draws heat from it.",ch,0,victim,TO_VICT,POS_SLEEPING);
		act("$N's leg goes limp as $n draws heat from it.",ch,0,victim,TO_NOTVICT);

		for (paf = victim->affected; paf != NULL; paf = paf->next)
		{
			if (paf->type == sn && paf->location == APPLY_DEX)
				affect_remove(victim,paf);			
		}

		init_affect(&af);
		af.where = TO_AFFECTS;
		af.aftype = AFT_MALADY;
		af.type = sn;
		af.location = APPLY_DEX;
		af.duration = level/6;
		af.level = level;
		af.modifier = -(level/5);
		affect_to_char(victim,&af);

		if(!saves_spell(level-2,victim,DAM_COLD))
		{
			act("$N nearly collapses to the ground as $S leg is frozen solid!",ch,0,victim,TO_CHAR);
			act_new("Shooting pains run through your leg as $n chills it, and then suddenly, you have no feeling in it at all.",ch,0,victim,TO_VICT,POS_SLEEPING);
			act("$N nearly collapses to the ground as $S leg is frozen solid!",ch,0,victim,TO_NOTVICT);
			init_affect(&af);
			af.where = TO_AFFECTS;
			af.aftype = AFT_INVIS;
			af.type = sn;
			af.location = APPLY_LEGS;
			af.duration = level/6;
			af.level = level;
			af.modifier = -1;
			affect_to_char(victim,&af);
		}
	}

	return;
}

int scramble_sn( CHAR_DATA *ch, int sn )
{
	int gsn, found = 0, snarray[100];

	for (gsn = 0; gsn < MAX_SKILL; gsn++ )
	{
		if (skill_table[gsn].name == NULL || skill_table[gsn].skill_level[ch->Class()->GetIndex()] > 52
		|| ch->level < skill_table[gsn].skill_level[ch->Class()->GetIndex()] || ch->pcdata->learned[gsn] < 1
		|| skill_table[gsn].spell_fun == spell_null)
			continue;

		if (skill_table[gsn].target == skill_table[sn].target
		&& skill_table[gsn].ctype == skill_table[gsn].ctype)
		{
			snarray[found] = gsn;
			found++;
		}
	}

	return snarray[number_range(0,found-1)];
}

void spell_acid_stream (int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	OBJ_DATA *armor;
	AFFECT_DATA af;
	char tochar[MSL], tovict[MSL], toroom[MSL], tochar2[MSL], tovict2[MSL], toroom2[MSL], tochar3[MSL], tovict3[MSL], toroom3[MSL], bodypart[MSL];
	int dam = dice(level,3);
	int diceroll, location, hardness = 0;

	if (((diceroll = number_percent()) <= 20) && IS_SET(victim->parts,PART_ARMS))
	{
		location = WEAR_ARMS;
		strcpy(bodypart, "arm");
	}
	else if ((diceroll <= 40 && IS_SET(victim->parts,PART_LEGS)))
	{
		location = WEAR_LEGS;
		strcpy(bodypart, "leg");
	}
	else if ((diceroll <= 60 && IS_SET(victim->parts,PART_HEAD)))
	{
		location = WEAR_HEAD;
		strcpy(bodypart, "head");
	}
	else
	{
		location = WEAR_BODY;
		strcpy(bodypart, "chest");
	}

	if ((armor = get_eq_char(victim, location)) != NULL)
	{
		hardness = material_table[armor->pIndexData->material_index].mat_hardness;

		if (hardness <= 3)
		{		
			sprintf(tovict2,"The sizzling acid seeps through your %s armor!",material_table[armor->pIndexData->material_index].mat_name);
			sprintf(tochar2,"The sizzling acid seeps through $N's %s armor!",material_table[armor->pIndexData->material_index].mat_name);
			sprintf(toroom2,"The sizzling acid seeps through $N's %s armor!",material_table[armor->pIndexData->material_index].mat_name);
		}
		else
		{
			sprintf(tovict2,"The sizzling acid drips off your %s armor!",material_table[armor->pIndexData->material_index].mat_name);
			sprintf(tochar2,"The sizzling acid drips off $N's %s armor!",material_table[armor->pIndexData->material_index].mat_name);
			sprintf(toroom2,"The sizzling acid drips off $N's %s armor!",material_table[armor->pIndexData->material_index].mat_name);
			dam /= 2;
		}
		
	}
	else
	{
		sprintf(tovict2,"You shriek in agony as the acid burns your exposed flesh!");
		sprintf(tochar2,"$N shrieks in agony as the acid burns $S exposed flesh!");
		sprintf(toroom2,"$N shrieks in agony as the acid burns $S exposed flesh!");
		dam *= 2;
	}

	sprintf(tochar,"You spray a stream of hissing acid at $N, striking $S %s!",bodypart);
    sprintf(tovict,"$n sprays a stream of hissing acid at you, striking your %s!",bodypart);
    sprintf(toroom,"$n sprays a stream of hissing acid at $N, striking $S %s!",bodypart);

	if(ch != victim)
	{
		act(tochar,ch,0,victim,TO_CHAR);
		act(tovict,ch,0,victim,TO_VICT);
 		act(toroom,ch,0,victim,TO_NOTVICT);
	}
	else
	{
		sprintf(tovict,"A stream of hissing acid hits you, striking your %s!",bodypart);
		sprintf(toroom,"A stream of hissing acid hits $n, striking $s %s!",bodypart);
		act(tovict,ch,0,0,TO_CHAR);
		act(toroom,ch,0,0,TO_ROOM);
	}
	
	if(ch != victim)
	{
		act(tochar2,ch,0,victim,TO_CHAR);
		act(tovict2,ch,0,victim,TO_VICT);
	}
	else
		act(tovict2,ch,0,0,TO_CHAR);

	act(toroom2,ch,0,victim,TO_NOTVICT);

	if (hardness > 3 || (armor != NULL && number_percent() > 50 ))
	{
		if(ch != victim)
			damage_new(ch,victim,dam,gsn_acid_stream,DAM_ACID,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"acid stream");
		else
			damage_new(victim,victim,dam,gsn_acid_stream,DAM_ACID,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"The stream of acid*");
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = gsn_acid_stream;
	af.level = level;
	af.owner = ch;
	
	switch (location) {
		case (WEAR_ARMS):
			sprintf(tochar3,"$N's arm goes limp as the acid sears $S flesh!");
			sprintf(tovict3,"Your muscles clench as the acid sears your flesh!");
			sprintf(toroom3,"$N's arm goes limp as the acid sears $S flesh!");
			af.duration = level/10;
			af.location = APPLY_STR;
			af.modifier = -level/10;
			break;
		case (WEAR_LEGS):
			sprintf(tochar3,"$N's legs buckle as the acid sears $S flesh!");
			sprintf(tovict3,"Your legs buckle beneath you as the acid sears your flesh!");
			sprintf(toroom3,"$N's legs buckle as the acid sears $S flesh!");
			af.duration = level/10;
			af.location = APPLY_DEX;
			af.modifier = -level/10;
			LAG_CHAR(victim,PULSE_VIOLENCE);
			break;
		case (WEAR_HEAD):
			sprintf(tochar3,"$N shrieks in agony as the acid burns $S face!");
			sprintf(tovict3,"The world dissolves into a haze of pain as acid sears your face!");
			sprintf(toroom3,"$N shrieks in agony as the acid burns $S face!");
			af.duration = 1;
			af.location = APPLY_HITROLL;
			af.modifier = -4;
			SET_BIT(af.bitvector, AFF_BLIND);
			break;
		case (WEAR_BODY):
			dam = (int)((float)dam * 1.5);
			break;
	}
	
		if (!is_affected(victim,gsn_acid_stream) && location != WEAR_BODY) {
		affect_to_char(victim,&af);
	}
	if (ch == victim)
	{
		af.duration = (int)((float)af.duration * 1.5);
		af.modifier = (int)((float)af.modifier * 1.5);
	}
	if (location != WEAR_BODY) {
		if(ch != victim)
		{
			act(tochar3,ch,0,victim,TO_CHAR);
			act(tovict3,ch,0,victim,TO_VICT);
		}
		else
			act(tovict3,ch,0,0,TO_CHAR);
		act(toroom3,ch,0,victim,TO_NOTVICT);
	}
	if(ch != victim)
		damage_new(ch,victim,dam,gsn_acid_stream,DAM_ACID,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"acid stream");
	else
		damage_new(victim,victim,dam,gsn_acid_stream,DAM_ACID,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"The stream of acid*");
}


void spell_acid_vein(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	char arg[MSL];
	OBJ_DATA *weapon;
	OBJ_AFFECT_DATA oaf;

	target_name = one_argument(target_name,arg);

	if((weapon=get_obj_carry(ch,arg,ch))==NULL)
		return send_to_char("You aren't carrying that.\n\r",ch);

	if(weapon->item_type != ITEM_WEAPON)
		return send_to_char("You can only cast this on weapons.\n\r",ch);

	if (is_affected_obj(weapon,gsn_acid_vein))
		return send_to_char("This weapon is already coated with acid!\n\r",ch);

	init_affect_obj(&oaf);
	oaf.where = TO_OBJ_AFFECTS;
	oaf.type = sn;
	oaf.aftype = AFT_SPELL;
	oaf.location = APPLY_OBJ_V3;
	oaf.modifier = 31 - weapon->value[3];
	oaf.duration = level*3;
	oaf.end_fun = acid_end;
	oaf.level = level;
	oaf.owner = ch;
	affect_to_obj(weapon,&oaf);
	act("You coat $p with acid.",ch,weapon,0,TO_CHAR);
	WAIT_STATE(ch,PULSE_VIOLENCE*2);

	return;
}

void acid_end(OBJ_DATA *obj, OBJ_AFFECT_DATA *af)
{
	if (obj->carried_by)
		act("$p is dissolved by the acid coating it.",obj->carried_by,obj,0,TO_CHAR);
	
	if (obj->in_room && obj->in_room->people)
		act("$p is dissolved by the acid coating it.",obj->in_room->people,obj,0,TO_ALL);

	if (obj->in_obj && obj->in_obj->carried_by)
		act("You hear a faint hissing sound coming from $p.",obj->in_obj->carried_by,obj->in_obj,0,TO_CHAR);

	extract_obj(obj);
	return;
}

extern char *  const   dir_name[];
extern const   sh_int  rev_dir[];

void spell_corrode_lock(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    char arg[MAX_INPUT_LENGTH];
	OBJ_DATA *obj;
	int door;

	target_name = one_argument( target_name, arg );

    if ( arg[0] == '\0' )
    {
    	return send_to_char( "What are you trying to corrode?\n\r", ch );
    }

	if (number_percent( ) > get_skill(ch,gsn_corrode_lock))
	{
	    send_to_char( "Your acid touch has no effect on the lock.\n\r", ch);
	    check_improve(ch,gsn_pick_lock,FALSE,2);
	    return;
	}
	else
	{
    	if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    	{
	    	if (obj->item_type == ITEM_PORTAL)
	    	{
				if (!IS_SET_OLD(obj->value[1],EX_ISDOOR))
		    		return send_to_char("You can't do that.\n\r",ch);
		    	if (!IS_SET_OLD(obj->value[1],EX_CLOSED))
		    		return send_to_char("It's not closed.\n\r",ch);
		    	if (obj->value[4] < 0)
		    		return send_to_char("It can not be melted.\n\r",ch);
		    	if (IS_SET_OLD(obj->value[1],EX_PICKPROOF))
		    		return send_to_char("You failed.\n\r",ch);
		    	REMOVE_BIT_OLD(obj->value[1],EX_LOCKED);
	    	}
			else
			{	
    			if ( obj->item_type != ITEM_CONTAINER )
        			return send_to_char( "That's not a container.\n\r", ch );
    			if ( !IS_SET_OLD(obj->value[1], CONT_CLOSED) )
        			return send_to_char( "It's not closed.\n\r",        ch );
    			if ( obj->value[2] < 0 )
        			return send_to_char( "It can not be melted.\n\r",   ch );
    			if ( !IS_SET_OLD(obj->value[1], CONT_LOCKED) )
        			return send_to_char( "It's already unlocked.\n\r",  ch );
    			if ( IS_SET_OLD(obj->value[1], CONT_PICKPROOF) )
        			return send_to_char( "You failed.\n\r",             ch );
    			REMOVE_BIT_OLD(obj->value[1], CONT_LOCKED);
			}
			act("You touch $p with an acid coated hand and corrode the lock.",ch,obj,NULL,TO_CHAR);
     		act("$n corrodes the lock on $p with his acid touch.",ch,obj,NULL,TO_ROOM);
    		check_improve(ch,gsn_corrode_lock,TRUE,2);
			return;
		}
		else
    		if ( ( door = find_door( ch, arg ) ) >= 0 )
    		{
    			ROOM_INDEX_DATA *to_room;
    			EXIT_DATA *pexit;
    			EXIT_DATA *pexit_rev;

    			pexit = ch->in_room->exit[door];
    			if ( !IS_SET(pexit->exit_info, EX_CLOSED) && !IS_IMMORTAL(ch))
					return send_to_char( "It's not closed.\n\r",        ch );
    			if ( pexit->key < 0 && !IS_IMMORTAL(ch))
					return send_to_char( "It can't be corroded.\n\r",     ch );
    			if ( !IS_SET(pexit->exit_info, EX_LOCKED) )
					return send_to_char( "It's already unlocked.\n\r",  ch );
    			if ( IS_SET(pexit->exit_info, EX_PICKPROOF) && !IS_IMMORTAL(ch))
					return send_to_char( "You failed.\n\r",             ch );

    			REMOVE_BIT(pexit->exit_info, EX_LOCKED);
    			send_to_char( "You corrode the lock on the door.\n\r", ch );
    			act( "$n corrodes the lock on $T.", ch, NULL, pexit->keyword, TO_ROOM );
    			check_improve(ch,gsn_corrode_lock,TRUE,2);
    			if ( ( to_room   = pexit->u1.to_room            ) != NULL
    			&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
    			&&   pexit_rev->u1.to_room == ch->in_room )
    			{
        			REMOVE_BIT( pexit_rev->exit_info, EX_LOCKED );
    			}
    		}
	}
    return;
}

void spell_attract (int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	if (is_affected(victim,sn)) {
		return send_to_char("They are already attracting lightning.\n\r",ch);
	}

	if (saves_spell(level,victim,DAM_LIGHTNING)) {
		send_to_char("You failed.\n\r",ch);
		act_new("You feel a slight tingling sensation, but it disspates.",ch,NULL,victim,TO_VICT,POS_SLEEPING);
		return;
	}

	init_affect(&af);
	af.where	= TO_AFFECTS;
	af.aftype	= get_spell_aftype(ch);
	af.type		= sn;
	af.level	= level;
	af.location	= 0;
	af.owner	= ch;
	af.modifier = 0;
	af.duration = dice(4,2) + level/5;
	af.tick_fun	= attract_tick;
	af.mod_name = MOD_RESISTANCE;
	new_affect_to_char(victim, &af);
	act("You enchant $N to attract raw lightning energy.",ch,NULL,victim,TO_CHAR);
	act_new("An intense tingling sensation overcomes you, but you note no immediate effects.",ch,NULL,victim,TO_VICT,POS_SLEEPING);
	act("$N crackles with electricity for a moment as $n extends a hand toward $M.",ch,NULL,victim,TO_NOTVICT);
	return;
}

void attract_tick (CHAR_DATA *ch, AFFECT_DATA *af)
{
    if (ch->in_room->area->sky < SKY_OVERCAST
		|| IS_SET(ch->in_room->room_flags,ROOM_INDOORS))
		return;
		
	act("Your skin tingles briefly, and a bolt arcs down from the dark skies above!",ch,NULL,ch,TO_CHAR);
	act("The skies light up as lightning arcs like quicksilver towards $N!",ch,NULL,ch,TO_ROOM);
	
	if (!af->owner)
		af->owner = ch;
	
	damage_new(af->owner,ch,dice(af->level,4),gsn_attract,DAM_LIGHTNING,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"the lightning strike*");
	if(!IS_AWAKE(ch))
		ch->position = POS_STANDING;
	return;
}

void spell_absorb (int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	if (is_affected(victim,gsn_absorb))
		return send_to_char("You are already absorbing electricity.\n\r",ch);

	init_affect(&af);
	af.where	= TO_AFFECTS;
	af.aftype	= AFT_SPELL;
	af.type		= gsn_absorb;
	af.level	= level;
	af.duration = dice(4,2) + level/4;
	af.modifier = 0;
	af.location = 0;
	affect_to_char(victim,&af);
	send_to_char("Warmth fills your body as you attune yourself to channel raw lightning.\n\r",victim);
	act("Flashes of light illuminate $N's eyes for a moment as $E completes the spell.",ch,NULL,victim,TO_ROOM);
	return;
}

void spell_call_lightning (int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim;
	AFFECT_DATA af;
	int dam;
	
	if (is_affected(ch,gsn_call_lightning))
		return send_to_char("You have not yet recovered from your prior casting.\n\r",ch);

	victim = get_char_world(ch, target_name);

	if (victim == NULL
		|| (ch->in_room->area != victim->in_room->area
		|| IS_NPC(victim)))
		return send_to_char("Call lightning on whom?\n\r",ch);

	if (victim == ch)
		return send_to_char("That's really not a good idea.\n\r",ch);

	if (ch->in_room->area->sky < SKY_DRIZZLE)
		return send_to_char("The weather is not suitable for calling lightning.\n\r",ch);
	
	if (IS_SET(victim->in_room->room_flags,ROOM_INDOORS) ||
		victim->in_room->sector_type == SECT_INSIDE)
		return send_to_char("Lightning cannot strike indoors.\n\r",ch);

	if (is_safe(ch,victim)) 	
		return;
	
	act("You call forth a bolt of lightning upon $N's location!",ch,NULL,victim,TO_CHAR);
	act("A bolt of lightning streaks down from the clouds above!",ch,NULL,victim,TO_VICT);

	init_affect(&af);
	af.where	= TO_AFFECTS;
	af.aftype   = AFT_TIMER;
	af.type		= sn;
	af.level	= level;
	af.duration	= 36;
	af.location	= 0;
	af.modifier = 0;
	affect_to_char(ch,&af);

	do_myell(victim,"Argh, I've been struck by lightning!", NULL);
	
	dam = dice(level,8);
	
	if(saves_spell(level,victim,DAM_LIGHTNING))
		dam /= 2;
	
	damage_new(ch,victim,dice(level,8),sn,DAM_LIGHTNING,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"The lightning strike*");

	return;
}

void spell_grounding (int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = get_char_room(ch,target_name);
	AFFECT_DATA af;
	char buf[MSL];

	if(!victim || !str_cmp(target_name,""))
		victim = ch;
	
	if (is_affected(victim,sn))
		return send_to_char("They are already grounded.\n\r",ch);

	if (ch->in_room->sector_type == SECT_AIR ||
		ch->in_room->sector_type == SECT_WATER ||
		ch->in_room->sector_type == SECT_UNDERWATER ||
		ch->in_room->sector_type == SECT_VERTICAL)
		return send_to_char("There is nothing with which to ground them here.\n\r",ch);
	
	if (!trusts(ch,victim) && is_safe(ch,victim))
		return send_to_char("They don't allow you to do that to them.\n\r",ch);

	if (!trusts(ch,victim) && saves_spell(level,victim,DAM_OTHER)) {
		send_to_char("They resisted your spell.\n\r",ch);
		sprintf(buf,"Die, %s, you sorcerous dog!",PERS(ch,victim));
		do_myell(victim,buf, ch);
		multi_hit(victim,ch,TYPE_UNDEFINED);
		return;
	}
	
	init_affect(&af);
	af.where	= TO_RESIST;
	af.aftype	= AFT_SPELL;
	af.type		= sn;
	af.location	= 0;
	af.modifier	= 0;
	af.duration	= level/5;
	af.level	= level;
	SET_BIT(af.bitvector, RES_LIGHTNING);
	af.mod_name = MOD_RESISTANCE;
	new_affect_to_char(victim,&af);

	af.mod_name = MOD_LEVITATION;
	zero_vector(af.bitvector);
	af.end_fun	= grounding_end;
	new_affect_to_char(victim,&af);

	act("You bind $N to the earth, grounding $M against lightning.",ch,NULL,victim,TO_CHAR);
	
	if (is_affected(victim, gsn_fly))
	{
		affect_strip(victim,gsn_fly);
		act("As $n gestures, you drop heavily to the ground!",ch,NULL,victim,TO_VICT);
		act("As $n gestures, $N drops heavily to the ground!",ch,NULL,victim,TO_NOTVICT);
	}
	else
	{
		act("As $n gestures, you feel more firmly connected to the earth beneath you.",ch,NULL,victim,TO_VICT);
		act("As $n gestures, $N seems more firmly connected to the earth beneath $M.",ch,NULL,victim,TO_NOTVICT);
	}
	
	if (IS_AFFECTED(victim,AFF_FLYING))
	{
		REMOVE_BIT (victim->affected_by, AFF_FLYING);
	}
	
	if (!trusts(ch,victim))
	{
		sprintf(buf,"Die, %s, you sorcerous dog!",PERS(ch,victim));
		do_myell(victim,buf,ch);
		multi_hit(victim,ch,TYPE_UNDEFINED);
	}
	
	return;
}

void grounding_end(CHAR_DATA *ch, AFFECT_DATA *af)
{
	if ((!str_cmp(race_table[ch->race].name,"imp")) ||
		(!str_cmp(race_table[ch->race].name,"sidhe"))) {
		SET_BIT (ch->affected_by, AFF_FLYING);
		send_to_char("As the magical grounding expires, you rise once more on your natural wings.\n\r",ch);
	}

	if (is_affected(ch,gsn_fly)) {
		SET_BIT (ch->affected_by, AFF_FLYING);
		send_to_char("As the grounding expires, your fly spell regains its effectiveness.\n\r",ch);
	}

	return;
}

void spell_thunderclap(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
	CHAR_DATA *vch, *vch_next;
	AFFECT_DATA af;
	char buf[MSL];	
	send_to_char("As you gesture, a booming thunderclap rips through the air!\n\r",ch);
	act("As $n gestures, a booming thunderclap rips through the air!",ch,0,0,TO_ROOM);

  	init_affect(&af);
  	af.where     = TO_AFFECTS;
	af.aftype    = AFT_SPELL;
   	af.type      = gsn_deafen;
  	af.level     = level;
   	af.duration  = 3;
   	af.modifier  = 0;
   	af.location  = 0;
	af.mod_name = MOD_HEARING;

	for(vch = ch->in_room->people; vch!=NULL; vch=vch_next)
	{
		vch_next = vch->next_in_room;
	        if (is_same_group(vch,ch) || is_safe(ch,vch) || is_same_cabal(ch,vch))
        	    continue;
        	if (!IS_NPC(ch) && !IS_NPC(vch) && (ch->fighting == NULL || vch->fighting == NULL)) {
        	 	sprintf(buf,"Die, %s you sorcerous dog!",PERS(ch,vch));
              	do_myell(vch,buf,ch);
	        }
		if(!saves_spell(level,vch,DAM_OTHER) &&
		   !saves_spell(level,vch,DAM_OTHER) && !is_affected(vch,sn)) {
			act("$n staggers, dazed by the force of the deafening thunderclap!",vch,0,0,TO_ROOM);
			send_to_char("The thunderclap resonates in your ears, deafening you!\n\r",vch);
			affect_to_char(vch, &af);
		}
		multi_hit(vch,ch,TYPE_UNDEFINED);
	}
}

void spell_neutralize(int sn, int level,CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	
	if(is_affected(victim,gsn_neutralize))
		return act("$N is already neutralized.",ch,0,victim,TO_CHAR);

	send_to_char("Your body becomes tempered against acids.\n\r",victim);
	act("$n's skin flushes slightly green for a moment, then returns to normal.",victim,0,0,TO_ROOM);
	init_affect(&af);
	af.where	=	TO_IMMUNE;
	af.aftype	=	AFT_SPELL;
	af.type		=	gsn_neutralize;
	af.level	=	level;
	af.duration	=	level / 3;
	af.modifier	=	0;
	af.location	=	0;

	if(victim==ch)
	{
		SET_BIT(af.bitvector, IMM_ACID);
    	affect_to_char(victim, &af);
		zero_vector(af.bitvector);
		SET_BIT(af.bitvector, RES_POISON);
		af.where	= 	TO_RESIST;
		af.aftype	=	AFT_INVIS;
	}
	else
	{
		af.where	=	TO_RESIST;
		SET_BIT(af.bitvector, RES_ACID);
	}

	af.mod_name = MOD_RESISTANCE;
	affect_to_char(victim, &af);
}

void spell_caustic_vapor( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    AFFECT_DATA af;
    ROOM_AFFECT_DATA raf;

    if (is_affected(ch,sn))
        return send_to_char("You are not yet ready to channel such large amounts of acid energy again.\n\r",ch);

    if (is_affected_room(ch->in_room,sn))
        return send_to_char("The room is already steaming with acid.\n\r",ch);

    if (((ch->in_room->sector_type) && (ch->in_room->sector_type == SECT_WATER))
        || (ch->in_room->sector_type == SECT_AIR)
		|| (ch->in_room->sector_type == SECT_UNDERWATER)
		|| (ch->in_room->sector_type == SECT_VERTICAL))
    {
        send_to_char("You can not cast caustic vapor in this room.\n\r",ch);
        return;
    }

	init_affect_room(&raf);
    raf.where  		= TO_ROOM_AFFECTS;
    raf.type   		= sn;
    raf.aftype 		= get_spell_aftype(ch);
    raf.level  		= level;
    raf.duration    = 20;
    raf.location    = 0;
    raf.modifier    = 0;
    raf.owner 	 	= ch;
    raf.end_fun 	= caustic_vapor_burnout;
    raf.tick_fun    = NULL;
    new_affect_to_room(ch->in_room,&raf);
	
    init_affect(&af);
    af.where    	= TO_AFFECTS;
    af.type    		= sn;
    af.aftype  		= AFT_TIMER;
    af.level   		= level;
    af.duration		= 47;
    af.location		= 0;
    af.modifier		= 0;
    affect_to_char(ch,&af);
    act("Vapors of acidic mist rise from the ground and begin to engulf the room.",ch,0,0,TO_ALL);
    zone_echo(ch->in_room->area,"You hear a massive hissing sound.");
    return;

}

void caustic_vapor_burnout(ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *af)
{
    CHAR_DATA *rch;

    for (rch = room->people; rch != NULL; rch = rch->next_in_room )
        act("The caustic vapors dissipate, leaving nary a trace behind.",rch,NULL,NULL,TO_CHAR);

    return;
}

void spell_smokescreen(int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
	ROOM_AFFECT_DATA raf;
	AFFECT_DATA af;

	if (is_affected(ch, gsn_smokescreen))
		return send_to_char("You can't produce another smokescreen so soon.\n\r", ch);

	if (is_affected_room(ch->in_room,sn))
		return send_to_char("The room is already filled with smoke.\n\r",ch);

	init_affect_room(&raf);
	raf.where  		= TO_ROOM_AFFECTS;
	raf.type   		= sn;
	raf.aftype 		= get_spell_aftype(ch);
	raf.level  		= level;
	raf.duration	= level / 5;
	raf.location	= 0;
	raf.modifier	= 0;
	raf.owner		= ch;
	raf.end_fun 	= smokescreen_end;
	raf.tick_fun		= NULL;
	new_affect_to_room(ch->in_room,&raf);

    init_affect(&af);
    af.where        = TO_AFFECTS;
    af.type         = sn;
    af.aftype       = AFT_SKILL;
    af.level        = level;
    af.duration     = 20;
    affect_to_char(ch,&af);
	
	act("Dense smoke fills the room, concealing any way out!",ch,0,0,TO_ALL);
	return;
}

void smokescreen_end(ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *af)
{
	if (room->people)
		act("The thick smoke in the room clears.",room->people,NULL,NULL,TO_ALL);

	return;
}

void spell_smother(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	act("$N chokes and gasps for air as you smother $M with thick smoke!",ch,0,victim,TO_CHAR);
	act("You choke and gasp for air as $n smothers you with thick smoke!",ch,0,victim,TO_VICT);
	act("$N chokes and gasps for air as $n smothers $M with thick smoke!",ch,0,victim,TO_NOTVICT);

	if (number_percent() > 6 * (get_curr_stat(victim,STAT_CON) - 12))
	{
		init_affect(&af);
		af.where = TO_AFFECTS;
		af.aftype = AFT_MALADY;
		af.type = sn;
		af.location = 0;
		af.duration = 1;
		af.level = level;
		af.modifier = 0;
		SET_BIT(af.bitvector, AFF_BLIND);
		af.mod_name = MOD_VISION;
		af.owner = ch;
		affect_to_char(victim,&af);
	}

	LAG_CHAR(victim,PULSE_VIOLENCE);
	return;
}

void spell_putrid_air(int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
	CHAR_DATA *vch, *vch_next;

	if (ch->in_room->sector_type && ch->in_room->sector_type == SECT_UNDERWATER)
	{
		send_to_char("You cannot cast this here.\n\r",ch);
		return;
	}

	act("A wave of putrid air sweeps through the area!",ch,0,0,TO_ALL);

	for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
	{
		vch_next = vch->next_in_room;

		if (is_affected(vch,gsn_ultradiffusion))
			continue;

		if (IS_NPC(vch) && !IS_SET(vch->form,FORM_BIPED))
			continue;

		if (vch == ch)
			continue;

		if (number_percent() > 10 * (get_curr_stat(vch,STAT_CON) - 15))
		{
			do_visible(vch, "");
			act("You burst out in a fit of coughing.",vch,NULL,NULL,TO_CHAR);
			act("$n bursts out in a fit of coughing.",vch,NULL,NULL,TO_ROOM);
		}
	}
	return;
}

void spell_asphyxiate(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int dam = dice(level,5);

	if (ch->in_room->sector_type && ch->in_room->sector_type == SECT_UNDERWATER)
	{
		send_to_char("You cannot cast this here.\n\r",ch);
		return;
	}

	act("$N gasps as you corrupt the air $E is breathing!",ch,0,victim,TO_CHAR);
	act("You gasp as $n corrupts the air you are breathing!",ch,0,victim,TO_VICT);
	act("$N gasps as $n corrupts the air $E is breathing!",ch,0,victim,TO_NOTVICT);

	if (saves_spell(level,victim,DAM_INTERNAL))
		dam /= 2;

	damage_new(ch,victim,dam,sn,DAM_INTERNAL,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);

	return;
}

void spell_shroud_of_secrecy(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;

	if(IS_NPC(ch))
		return send_to_char("I don't think so.\n\r", ch);

	if (ch->in_room->sector_type && ch->in_room->sector_type == SECT_UNDERWATER)
		return send_to_char("You cannot cast this here.\n\r",ch);

	act("A shroud of smoke bursts out around you, concealing your presence.",ch,0,0,TO_CHAR);
	act("A shroud of smoke bursts out around $n, concealing $s presence.",ch,0,0,TO_ROOM);

	free_pstring(ch->name);
	ch->name = palloc_string("Someone");

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = sn;
	af.location = 0;
	af.duration = level/5;
	af.level = level;
	af.modifier = 0;
	af.end_fun = shroud_end;
	af.mod_name = MOD_VISIBILITY;
	affect_to_char(ch,&af);

	return;
}

void shroud_end(CHAR_DATA *ch, AFFECT_DATA *af)
{
	free_pstring(ch->name);
	ch->name = palloc_string(ch->true_name);
	act("The shroud of smoke concealing you dissipates.",ch,0,0,TO_CHAR);
	act("The shroud of smoke concealing $n dissipates.",ch,0,0,TO_ROOM);
	return;
}

void spell_noxious_ward(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;

	if (is_affected(ch,sn))
		return send_to_char("You are already protected by a noxious ward.\n\r",ch);

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = sn;
	af.location = 0;
	af.duration = 12;
	af.level = level;
	af.modifier = 0;
	af.mod_name = MOD_PROTECTION;
	affect_to_char(ch,&af);

	act("You feel protected by a noxious ward.",ch,0,0,TO_CHAR);

	return;
}

void spell_molten_stones(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int blunt=0, fire=0;

	if((ch->in_room->sector_type == SECT_WATER) || (ch->in_room->sector_type == SECT_UNDERWATER) || (ch->in_room->sector_type == SECT_AIR))
		return send_to_char("You can't find any stone here.\n\r",ch);

	blunt = dice(level,3);
	fire = dice(level,2);
	if(saves_spell(level,victim,DAM_BASH))
		blunt /= 2;
	if(saves_spell(level,victim,DAM_FIRE))
		fire /= 2;

	act("$n grabs a large stone, liquifies it in a flash and sends it flying at $N!",ch,0,victim,TO_NOTVICT);
	act("You grab a large stone, liquify it with a burst of heat, and send it flying at $N!",ch,0,victim,TO_CHAR);
	act("$n grabs a large stone, liquifies it in a flash and sends it flying at you!",ch,0,victim,TO_VICT);
	
	damage_new(ch,victim,blunt,sn,DAM_BASH,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"molten stone");

	send_to_char("Your flesh sizzles as the hurled magma strikes you!\n\r",victim);
	
	damage_new(ch,victim,fire,sn,DAM_FIRE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"the searing heat*");
	
	return;
}

void spell_heat_earth(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	char buf[MSL];
	CHAR_DATA *vch, *vch_next;
	int dam;

	if (!IS_GROUND(ch->in_room))
		return send_to_char("You must be on solid ground to cast this spell.\n\r",ch);
	
	dam = dice (level,7);

	act("You superheat the earth in the area, searing your foes!",ch,0,0,TO_CHAR);
	act("$n waves $s arm at the ground and waves of heat radiate upward from beneath you!",ch,0,0,TO_ROOM);

	for(vch = ch->in_room->people; vch; vch = vch_next) {
		vch_next = vch->next_in_room;
		if (is_same_group(vch,ch) || is_safe(ch,vch) || is_same_cabal(ch,vch))
			continue;
		if (!IS_NPC(ch) && !IS_NPC(vch) && (!ch->fighting || !vch->fighting)) {
			sprintf(buf,"Die, %s you sorcerous dog!",PERS(ch,vch));
			do_myell(vch,buf,ch);
		}
		if (saves_spell(level,vch,DAM_FIRE))
			dam /= 2;
		if (IS_AFFECTED(vch,AFF_FLYING))
			dam /= 2;
		damage_new(ch,vch,dam,TYPE_UNDEFINED,DAM_FIRE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"scorched earth");
	}

	return;
}

void spell_blanket(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	ROOM_AFFECT_DATA raf;

	if (is_affected(ch,sn))
		return send_to_char("You have not yet fully recovered from your last casting.\n\r",ch);

	if ((ch->in_room->sector_type) && ((ch->in_room->sector_type == SECT_AIR)
		|| (ch->in_room->sector_type == SECT_WATER)
		|| (ch->in_room->sector_type == SECT_UNDERWATER)
		|| (ch->in_room->sector_type == SECT_VERTICAL)))
		return send_to_char("Blanket what??\n\r",ch);

	if ((ch->in_room->sector_type)
		&& ((ch->in_room->sector_type == SECT_BURNING)
		|| (ch->in_room->sector_type == SECT_DESERT)))
		return send_to_char("Your magic is not powerful enough to create snow here.\n\r",ch);

	if (!IS_OUTSIDE(ch))
		return send_to_char("No amount of concentration will make it snow indoors.\n\r",ch);
	
	if (ch->in_room->area->temp == TEMP_HOT)
		return send_to_char("It is too hot to create even magical snow right now.\n\r", ch);
	
	init_affect_room(&raf);
	raf.where 	= 	TO_ROOM_AFFECTS;
	raf.type	=	gsn_blanket;
	raf.aftype	=	AFT_SPELL;
	raf.level	=	level;
	raf.duration=	24;
	raf.location=	APPLY_ROOM_SECT;
	raf.modifier=	SECT_SNOW - ch->in_room->sector_type;
	raf.owner	=	ch;
	raf.end_fun	=	blanket_melt;
	raf.tick_fun=	NULL;
	new_affect_to_room(ch->in_room,&raf);

	init_affect(&af);
	af.where	=	TO_AFFECTS;
	af.type		=	gsn_blanket;
	af.aftype	=	AFT_TIMER;
	af.level	=	level;
	af.duration	=	24;
	af.location	=	0;
	af.modifier	=	0;
	affect_to_char(ch,&af);

	clear_tracks(ch->in_room);
	
	act("A sudden gust sweeps through, bearing heavy snowflakes that rapidly blanket the ground.",ch,0,0,TO_ALL);

	return;
}

void blanket_melt (ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *af)
{
	CHAR_DATA *rch;

	for (rch = room->people; rch != NULL; rch = rch->next_in_room)
		act("The layer of snow blanketing the earth fades away as the magic dissipates.",rch,0,0,TO_CHAR);

	clear_tracks(room);
	
	return;
}

void spell_boreal_wind (int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int dam;

	if (ch->in_room->sector_type == SECT_UNDERWATER)
		return send_to_char("You cannot cast this spell underwater.\n\r",ch);
	
	dam = dice(level,5);

	switch (ch->in_room->area->temp) {
		case (TEMP_HOT):
			dam = (int)((float)dam * 0.25);
			break;
		case (TEMP_WARM):
			dam = (int)((float)dam * 0.75);
			break;
		case (TEMP_COOL):
			dam = (int)((float)dam * 1.25);
			break;
		case (TEMP_COLD):
			dam = (int)((float)dam * 1.75);
			break;
		default:
			break;
	}

	act("You gesture at $N, sending a bitter blast of frigid air towards $M!",ch,NULL,victim,TO_CHAR);
	act("$n gestures at you, sending a bitter blast of frigid air towards you!",ch,NULL,victim,TO_VICT);
	act("$n gestures at $N, sending a bitter blast of frigid air towards $M!",ch,NULL,victim,TO_NOTVICT);

	if (saves_spell(level,victim,DAM_COLD))		dam /= 2;

	damage_new(ch,victim,dam,sn,DAM_COLD,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);

	return;
}

void spell_concave_shell(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	EXIT_DATA *pexit;
	int dir;
	int *dirptr = NULL;

	if (!str_cmp(target_name, "n") || !str_cmp(target_name, "north"))
		dir = DIR_NORTH;
	else if (!str_cmp(target_name, "e") || !str_cmp(target_name, "east"))
		dir = DIR_EAST;
	else if (!str_cmp(target_name, "s") || !str_cmp(target_name, "south"))
		dir = DIR_SOUTH;
	else if (!str_cmp(target_name, "w") || !str_cmp(target_name, "west"))
		dir = DIR_WEST;
	else if (!str_cmp(target_name, "u") || !str_cmp(target_name, "up" ))
		dir = DIR_UP;
	else if (!str_cmp(target_name, "d") || !str_cmp(target_name, "down"))
		dir = DIR_DOWN;
	else return send_to_char("In which direction?\n\r",ch);

	pexit = ch->in_room->exit[dir];

	if (pexit == NULL || pexit->u1.to_room == NULL
	   || (IS_SET(pexit->exit_info,EX_CLOSED) && !IS_AFFECTED(ch,AFF_PASS_DOOR))
	   || (IS_SET(pexit->exit_info,EX_CLOSED)
	   && IS_SET(pexit->exit_info,EX_NOPASS))) {
		send_to_char("That direction is obstructed!\n\r",ch);
		return;
	}
	
	ch->disrupted = FALSE;

	dirptr = (int *)talloc_struct(sizeof(int));	
	*dirptr = dir;
	
	RS.Queue.AddToQueue(6, 3, concave_shell_move, ch, dirptr, ch->in_room);

	act("Air begins to swirl rapidly around you.",ch,0,0,TO_CHAR);
	act("Swirling winds begin to mass near $n.",ch,0,0,TO_ROOM);
	
	return;
}

void concave_shell_move (CHAR_DATA *ch, int *dirptr,ROOM_INDEX_DATA *oldroom)
{
	char buf[MSL];
	int i, dir, range;
	bool verbose = FALSE, stopped = FALSE;
	EXIT_DATA *pexit;
	char *direction;
	CHAR_DATA *wch, *wch_next;

	dir = *dirptr;

	if (ch->in_room->vnum != oldroom->vnum)
		return;
	
	if (ch->disrupted) {
		act("The shell of air dissipates with no effect.",ch,0,0,TO_ALL);
		return;
	}

	if (is_affected(ch,gsn_bind_feet))
		return send_to_char("Your feet are too tightly bound to the earth for the shell to move you.\n\r",ch);
	
	range = ch->level / 3;

	direction = (char *)flag_name_lookup(dir,direction_table);
	act("The shell of air finishes forming and rushes $tward, taking you with it!",ch,direction,0,TO_CHAR);
	act("The shell of air finishes forming and rushes $tward, taking $n with it!",ch,direction,0,TO_ROOM);

	if(!IS_SET(ch->comm,COMM_BRIEF)) {
		SET_BIT(ch->comm,COMM_BRIEF);
		verbose = TRUE;
	}
	
	pexit = ch->in_room->exit[dir];

	
	if (pexit == NULL || pexit->u1.to_room == NULL ||
		(IS_SET(pexit->exit_info,EX_CLOSED) &&
		!IS_AFFECTED(ch,AFF_PASS_DOOR)) ||
		(IS_SET(pexit->exit_info,EX_CLOSED) &&
		IS_SET(pexit->exit_info,EX_NOPASS))) {
		act("Since the exit $t has become obstructed, the swirling winds dissipate.",ch,direction,0,TO_ALL);
		return;
	}
		
	for (i = 1; i <= range; i++) {
		move_char(ch,dir,TRUE,TRUE);
		
		pexit = ch->in_room->exit[dir];	

		if (pexit == NULL || pexit->u1.to_room == NULL ||
			(IS_SET(pexit->exit_info,EX_CLOSED) &&
			!IS_AFFECTED(ch,AFF_PASS_DOOR)) ||
			(IS_SET(pexit->exit_info,EX_CLOSED) &&
			IS_SET(pexit->exit_info,EX_NOPASS))) {
			send_to_char("Your rapid motion comes to a crashing halt as you strike an obstacle!\n\r",ch);
			act("$n flies into the room and crashes into the obstruction $tward!",ch,direction,0,TO_ROOM);
			damage_new(ch,ch,dice(10,10),gsn_bash,DAM_BASH,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"bone-jarring collision");
			WAIT_STATE(ch,PULSE_VIOLENCE*2);
			ch->position = POS_RESTING;
			break;
		}
		
		for (wch = ch->in_room->people; wch != NULL; wch = wch_next) {
			wch_next = wch->next_in_room;

			if (IS_NPC(wch)
			&& ((IS_SET(wch->act, ACT_AGGRESSIVE) && wch->level >= ch->level+5)
			|| IS_SET(wch->off_flags,SPAM_MURDER))
			&& IS_AWAKE(wch)
			&& wch->fighting != NULL
			&& can_see(wch,ch)
			&& !IS_AFFECTED(wch,AFF_CALM)
			&& !IS_SET(ch->in_room->room_flags,ROOM_SAFE)) {
				sprintf(buf,"As you hurtle along, %s suddenly obstructs your path!\n\r",PERS(wch,ch));
				send_to_char(buf,ch);
				WAIT_STATE(ch,PULSE_VIOLENCE);
				ch->position = POS_RESTING;
				stopped = TRUE;
				break;
			}
		}
		if (stopped)	break;
	}

	if (verbose)
		REMOVE_BIT(ch->comm,COMM_BRIEF);

	return;
	
}

void spell_frost_glaze(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	int acmod;

	if(is_affected(ch,sn))
		return send_to_char("Your armor is already covered in frost.\n\r",ch);
	
	act("Your armor becomes covered in frost.",ch,0,0,TO_CHAR);
	act("$n's armor becomes covered in frost.",ch,0,0,TO_ROOM);
	
	acmod = level/2;
	switch (ch->in_room->area->temp) {
		case(TEMP_HOT):
			acmod /= (int)2;
			break;
		case(TEMP_WARM):
			acmod = (int)((float)acmod * 0.8);
			break;
		case(TEMP_COOL):
			acmod = (int)((float)acmod * 1.25);
			break;
		case(TEMP_COLD):
			acmod = (int)((float)acmod * 1.5);
			break;
	}
	
	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = sn;
	af.aftype = AFT_SPELL;
	af.location = APPLY_AC;
	af.duration = 1 + level/4;
	af.modifier = acmod;
	af.level = level;
	affect_to_char(ch,&af);
	af.where = TO_RESIST;
	af.location = 0;
	af.modifier = 0;
	SET_BIT(af.bitvector, RES_COLD);
	af.mod_name = MOD_RESISTANCE;
	affect_to_char(ch,&af);
	return;
}

void spell_unbreakable(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	OBJ_DATA *obj;
	AFFECT_DATA af;

	if(victim == NULL)
		victim = ch;
	obj = get_eq_char(victim,WEAR_WIELD);

	if (obj) {
		if(victim == ch) {
			act("You painfully freeze your weapon to your hand!",ch,0,victim,TO_CHAR);
			act("$n's fist turns a rigid white as $e grips $s weapon tightly.",ch,0,0,TO_ROOM);
		} else {
			act("You freeze $N's weapon to $S hand.",ch,0,victim,TO_CHAR);
			act("$n painfully freezes your weapon to your hand!",ch,0,victim,TO_VICT);
			act("$N's fist turns a rigid white as $E grips $S weapon tightly.",ch,0,victim,TO_NOTVICT);
		}
	} else {
		if(victim == ch) {
			act("You are not wielding a weapon!",ch,0,0,TO_CHAR);
		} else {
			act("$N is not wielding a weapon!",ch,0,victim,TO_CHAR);
		}
		return;
	}
	
	damage_new(ch,victim,dice(6,9),TYPE_UNDEFINED,DAM_COLD,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"the intense cold*");

	if(is_affected(victim,gsn_unbreakable)) {
		if(ch == victim) {
			return send_to_char("Your weapon is already frozen to your hand!\n\r",ch);
		} else {
			act("$N's weapon is already frozen to $S hand!",ch,0,victim,TO_CHAR);
			send_to_char("Your weapon is already frozen to your hand!\n\r",victim);
			return;
		}
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = gsn_unbreakable;
	af.aftype = AFT_SPELL;
	af.owner = ch;
	af.modifier = 0;
	af.duration = level/4;
	af.location = 0;
	af.level = level;
	affect_to_char(victim,&af);

	return;
}

void spell_earthsembrace(int sn,int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	if (!IS_GROUND(ch->in_room)) {
		send_to_char("You must be in contact with the ground to cast this spell.\n\r",ch);
		return;
	}
	
	if ( is_affected( ch, sn ) || is_affected(ch, gsn_diamondskin) || is_affected(ch, gsn_stoneskin))
	{
		if (victim == ch)
			send_to_char("Your skin is already as hard as a rock.\n\r",ch);
		else
			act("$N is already as hard as can be.",ch,NULL,victim,TO_CHAR);
	}
	else
	{
		init_affect(&af);
		af.where	 = TO_AFFECTS;
		af.aftype	 = get_spell_aftype(ch);
		af.type      = sn;
		af.level     = level;
		af.duration  = level/2;
		af.location  = APPLY_AC;
		af.modifier  = level / 2;
		affect_to_char( victim, &af );
		act( "Stone and soil flow like liquid from the ground below, encasing $n.", victim, NULL, NULL, TO_ROOM );
		send_to_char( "Stone and soil flow like liquid from the ground, encasing you in armor.\n\r", victim );
	}
	return;
}

void spell_whiteout(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AREA_DATA *area;
	AREA_AFFECT_DATA aaf;
	AFFECT_DATA af;

	area = ch->in_room->area;

	if (is_affected(ch,gsn_whiteout))
		return send_to_char("You have not yet fully recovered from the last time you cast whiteout.\n\r",ch);
	
	if (!IS_OUTSIDE(ch))
		return send_to_char("You must be able to see the sky to create a whiteout.\n\r",ch);
	
	if (is_affected_area(area,gsn_whiteout) ||
		is_affected_area(area,gsn_cyclone) ||
		is_affected_area(area,gsn_cyclone_forming)) {
		send_to_char("There is already a weather disturbance in this area.\n\r",ch);
		return;
	}

	if (area->temp == TEMP_HOT || area->sky == SKY_CLEAR)
		return send_to_char("It is beyond your magic to change the weather so drastically.\n\r",ch);
	
	outdoors_echo(area,"{WAn unnaturally-cold wind whips up, and in an instant, torrents of snow\n\rreduce visibility to absolutely nil!{x");
	
	init_affect_area(&aaf);
	aaf.where 	=	TO_AREA_AFFECTS;
	aaf.type	=	gsn_whiteout;
	aaf.aftype	=	AFT_SPELL;
	aaf.level	=	level;
	aaf.duration=	2;
	aaf.location=	APPLY_AREA_TEMP;
	aaf.modifier=	TEMP_COLD - area->temp;
	aaf.owner	=	ch;
	aaf.end_fun	=	whiteout_end;
	aaf.tick_fun=	NULL;
	affect_to_area(area,&aaf);
	aaf.end_fun	=	NULL;
	aaf.location=	APPLY_AREA_WIND;
	aaf.modifier=	WIND_GALE - area->wind;
	affect_to_area(area,&aaf);
	aaf.location=	APPLY_AREA_SKY;
	aaf.modifier=	SKY_BLIZZARD - area->sky;
	affect_to_area(area,&aaf);

	init_affect(&af);
	af.where	=	TO_AFFECTS;
	af.type		=	gsn_whiteout;
	af.aftype	=	AFT_TIMER;
	af.level	=	level;
	af.duration	=	36;
	af.location	=	0;
	af.modifier	=	0;
	af.owner	=	ch;
	affect_to_char(ch,&af);

	return;
}

void whiteout_end (AREA_DATA *area, AREA_AFFECT_DATA *af)
{
	outdoors_echo(area,"{WThe whipping winds and thick snow suddenly subside as visibility returns.{x");
	return;
}


void spell_frigid_breeze(int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
	CHAR_DATA *vch, *vch_next;
	int chance;

	if (ch->in_room->sector_type && ch->in_room->sector_type == SECT_UNDERWATER)
	{
		send_to_char("You cannot cast this here.\n\r",ch);
		return;
	}

	act("A wave of chilled air sweeps through the area!",ch,0,0,TO_ALL);

	for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
	{
		vch_next = vch->next_in_room;

		if (is_affected(vch,gsn_ultradiffusion))
			continue;

		if (IS_NPC(vch) && !IS_SET(vch->form,FORM_BIPED))
			continue;

		if (vch == ch)
			continue;

		chance = number_percent();
		
		switch (ch->in_room->area->temp) {
			case (TEMP_HOT):
				if (chance <= 80)	continue;
				break;
			case (TEMP_WARM):
				if (chance <= 60)	continue;
				break;
			case (TEMP_COOL):
				if (chance <= 40)	continue;
				break;
			case (TEMP_COLD):
				if (chance <= 20)	continue;
				break;
		}	
		
		act("Your breath mists in the cold air, revealing your presence!",vch,NULL,NULL,TO_CHAR);
		act("A plume of misted breath betrays $n's presence!",vch,NULL,NULL,TO_ROOM);
		do_visible(vch, "");
	}
	return;
}

void spell_pure_air (int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *vch;
	bool cleansed;
	AFFECT_DATA *laf;
	ROOM_AFFECT_DATA *raf;

	if (ch->in_room->sector_type && ch->in_room->sector_type == SECT_UNDERWATER)
		return send_to_char ("You cannot cast this here.\n\r",ch);

	if (ch->in_room->area->temp <= TEMP_WARM)
		return send_to_char ("The air here is too warm for a purifying breeze.\n\r",ch);

	act("A gentle breeze of icy air sweeps through the area.",ch,0,0,TO_ALL);

	for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room) {
		cleansed = FALSE;

		for(laf = ch->affected; laf != NULL; laf = laf->next )
			if (skill_table[laf->type].dispel & CAN_PURIFY)
			{
				affect_strip(vch,laf->type);
				cleansed = TRUE;
			}

		for(raf = ch->in_room->affected; raf != NULL; raf = raf->next)
			if (skill_table[raf->type].dispel & CAN_PURIFY)
				affect_strip_room(ch->in_room, raf->type);

		if (cleansed)
			act("The air around $n is purified.",vch,0,0,TO_ROOM);
	}

	return;
}

void spell_icelance(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	int dam = 2 * dice(level, 3);

	if (!IS_GROUND(ch->in_room))
		return send_to_char("You must have ground beneath you to cast this spell.\n\r",ch);
	
	if(saves_spell(level,victim,DAM_COLD))
		dam /= 2;

	act("You gesture at the ground and a jagged lance of ice erupts, impaling $N!",ch,0,victim,TO_CHAR);
	act("$n gestures at the ground and a jagged lance of ice erupts, impaling you!",ch,0,victim,TO_VICT);
	act("$n gestures at the ground and a jagged lance of ice erupts, impaling $N!",ch,0,victim,TO_NOTVICT);

	damage_new(ch,victim,dam,sn,DAM_PIERCE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);

	if (number_percent() <= 15) {
		init_affect(&af);
		af.where = TO_AFFECTS;
		af.duration = 8;
		af.type = gsn_bleeding;
		af.level = level;
		af.location = 0;
		af.modifier = 0;
		af.aftype = AFT_MALADY;
		af.tick_fun = bleeding_tick;
		af.end_fun = NULL;
		af.owner = ch;
		new_affect_to_char(victim,&af);

		act("Blood spills forth from your gaping wound!",victim,0,0,TO_CHAR);
		act("Blood begins to pour from $n's gaping wound!",victim,0,0,TO_ROOM);
	}

	return;
}

void spell_freeze_door(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	int door;
	ROOM_INDEX_DATA *to_room;
	ROOM_AFFECT_DATA raf;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_opp;
	char *direction;

	if (!str_cmp(target_name,""))
		return send_to_char("What door?\n\r",ch);

	if ((door = find_door(ch,target_name)) == -1)
		return;
	
	pexit = ch->in_room->exit[door];
	
	if (!IS_SET(pexit->exit_info,EX_CLOSED))
		return send_to_char("That door is not closed.\n\r",ch);
	
	direction = (char *)flag_name_lookup(door,direction_table);
	act("You cover the $T door with a thick frost, freezing it shut.",ch,0,direction,TO_CHAR);
	act("$n covers the $T door with a thick frost, freezing it shut.",ch,0,direction,TO_ROOM);

	SET_BIT(pexit->exit_info,EX_JAMMED);

	init_affect_room(&raf);
	raf.where	=	TO_ROOM_AFFECTS;
	raf.type	=	gsn_freeze_door;
	raf.aftype	=	AFT_SPELL;
	raf.level	=	level;
	raf.duration=	8;
	raf.owner	=	ch;
	raf.modifier=	door;
	raf.end_fun	=	door_unfreeze;
	new_affect_to_room(ch->in_room,&raf);
	
	if ((to_room = pexit->u1.to_room) != NULL
		&& (pexit_opp = to_room->exit[reverse_d(door)]) != NULL
		&& pexit_opp->u1.to_room == ch->in_room) {
		SET_BIT(pexit_opp->exit_info,EX_JAMMED);
		raf.modifier=	reverse_d(door);
		new_affect_to_room(to_room,&raf);
	}

	return;
}

void door_unfreeze (ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *af)
{
	EXIT_DATA *pexit;
	char *direction;

	pexit = room->exit[af->modifier];

	if(room->people) {
		direction = (char*)flag_name_lookup(af->modifier,direction_table);
		act("The frost covering the door $T thaws.",room->people,0,direction,TO_ALL);
	}
	
	REMOVE_BIT(pexit->exit_info,EX_JAMMED);
	
	return;
}

void spell_frost_growth (int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	ROOM_AFFECT_DATA raf;

	if (!IS_GROUND(ch->in_room))
		return send_to_char("You must be on solid ground to cast this spell.\n\r", ch);

	act("As you gesture, a barely-visible slick frost covers the ground beneath you.",ch,0,0,TO_CHAR);
	act("As $n gestures, a barely-visible slick frost covers the ground beneath you.",ch,0,0,TO_ROOM);

	init_affect_room(&raf);
	raf.where	=	TO_ROOM_AFFECTS;
	raf.type	=	gsn_frost_growth;
	raf.aftype	=	AFT_SPELL;
	raf.level	=	level;
	raf.duration=	level/3;
	raf.owner	=	ch;
	raf.end_fun	=	ground_thaw;
	new_affect_to_room(ch->in_room,&raf);

	return;
}

void ground_thaw (ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *af)
{
	if (room->people)
		act("The fine layer of frost coating the ground melts away.",room->people,0,0,TO_ALL);

	return;
}

void spell_bind_feet (int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	if (!IS_GROUND(ch->in_room))
		return send_to_char("There is no ground here to entrap their feet.\n\r",ch);

	if (IS_AFFECTED(victim,AFF_FLYING))
		return send_to_char("Their feet are not in contact with the ground.\n\r",ch);

	if (saves_spell(level, victim, DAM_COLD)) {
		act("A chill runs through your feet.",victim,0,0,TO_CHAR);
		act("$n's legs stiffen briefly before resuming their motion.",victim,0,0,TO_ROOM);
		return;
	}

	act("You gesture at $N's legs and a thick frost binds $N's feet to the ground!",ch,0,victim,TO_CHAR);
	act("$n gestures at your legs and a thick frost binds your feet to the ground!",ch,0,victim,TO_VICT);
	act("$n gestures at $N's legs and a thick frost binds $S feet to the ground!",ch,0,victim,TO_NOTVICT);
	
	init_affect(&af);
	af.where	=	TO_AFFECTS;
	af.type		=	gsn_bind_feet;
	af.aftype	=	AFT_SPELL;
	af.level	=	level;
	af.duration	=	level/10;
	af.owner	=	ch;
	affect_to_char(victim,&af);

	return;
}

void spell_glaciate (int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	ROOM_INDEX_DATA *room = ch->in_room;
	ROOM_AFFECT_DATA raf;

	if (room->sector_type != SECT_WATER)
		return send_to_char("You must be on the surface of water to cast this spell.\n\r",ch);

	if (room->area->temp == TEMP_HOT)
		return send_to_char ("The ambient temperature is too high to freeze the water.\n\r",ch);

	init_affect_room(&raf);
	raf.where	=	TO_ROOM_AFFECTS;
	raf.type	=	gsn_glaciate;
	raf.aftype	=	AFT_SPELL;
	raf.level	=	level;
	raf.duration=	3 + level/7;
	raf.location=	APPLY_ROOM_SECT;
	raf.modifier=	SECT_ICE - ch->in_room->sector_type;
	raf.owner	=	ch;
	raf.end_fun	=	glaciate_melt;
	raf.tick_fun=	NULL;
	new_affect_to_room(room, &raf);

	act("The water beneath you suddenly congeals into glacial ice.",ch,0,0,TO_CHAR);

	return;
	
}

void glaciate_melt (ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *raf)
{
	if (room->people)
		act("The ice beneath you melts into water as the magic dissipates.",room->people,0,0, TO_ALL);

	return;
}

void spell_hailstorm (int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	char buf[MSL];
	CHAR_DATA *vch, *vch_next;
	int dam;

	dam = dice(level, 4);

	if (IS_OUTSIDE(ch))
		dam = (int)((float)dam * 1.75);

	act("You conjure a rain of deadly hailstones to pelt the area!",ch,0,0,TO_CHAR);
	act("$n throws $s arms in the air, calling a rain of deadly hailstones upon the area!",ch,0,0,TO_ROOM);
	
	for(vch = ch->in_room->people; vch; vch = vch_next) {
		vch_next = vch->next_in_room;
		if (is_same_group(vch,ch) || is_safe(ch,vch) || is_same_cabal(ch,vch))
			continue;
		if (!IS_NPC(ch) && !IS_NPC(vch) && (!ch->fighting || !vch->fighting)) {
			sprintf(buf,"Die, %s you sorcerous dog!",PERS(ch,vch));
			do_myell(vch,buf,ch);
		}
		if (saves_spell(level,vch,DAM_COLD))
			dam /= 2;
		damage_new(ch,vch,dam,TYPE_UNDEFINED,DAM_COLD,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"hailstorm");
	}
	
	return;
}

void spell_stalactites (int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	ROOM_AFFECT_DATA raf;

	if (IS_OUTSIDE(ch))
		return send_to_char("You must be indoors to cast this spell.\n\r",ch);

	if (is_affected_room(ch->in_room, gsn_stalactites))
		return send_to_char("There are already stalactites hanging here.\n\r",ch);
	
	act("You cause razor-sharp stalactites to sprout from the ceiling above.",ch,0,0,TO_CHAR);
	act("$n raises one arm and icy stalactites form on the ceiling above.",ch,0,0,TO_ROOM);

	init_affect_room(&raf);
	raf.where		=	TO_ROOM_AFFECTS;
	raf.aftype		=	AFT_SPELL;
	raf.type		=	gsn_stalactites;
	raf.level		=	level;
	raf.location	=	0;
	raf.modifier	=	dice(1,6);
	raf.duration	=	level/5;
	raf.owner		=	ch;
	raf.end_fun		=	NULL;
	raf.tick_fun	=	NULL;
	new_affect_to_room(ch->in_room,&raf);

	return;
}

void spell_ice_blast (int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	OBJ_DATA *obj, *obj_next;
	OBJ_AFFECT_DATA oaf;

	act("You extend an arm toward $N and send forth a frigid blast!",ch,0,victim,TO_CHAR);
	act("$n extends an arm toward you and sends forth a frigid blast!",ch,0,victim,TO_VICT);
	act("$n extends an arm toward $N and sends forth a frigid blast!",ch,0,victim,TO_NOTVICT);

	damage_new(ch, victim, dice(level, 3), gsn_ice_blast, DAM_COLD, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, NULL);

	for (obj = victim->carrying; obj; obj = obj_next) {
		obj_next = obj->next_content;

		switch (obj->item_type) {
			case (ITEM_DRINK_CON):
				if (number_percent() <= 75) {
					act("$p freezes and shatters!",victim,obj,0,TO_ALL);
					extract_obj(obj);
				}
				break;
			case (ITEM_POTION):
				if (number_percent() <= 45) {
					act("$p freezes and shatters!",victim,obj,0,TO_ALL);
					extract_obj(obj);
				}
				break;
			case (ITEM_CONTAINER):
				if (number_percent() <= 15) {
					act("$p is frozen shut by a thick layer of ice!",victim,obj,0,TO_ALL);
					init_affect_obj(&oaf);
					oaf.where		=	TO_OBJ_AFFECTS;
					oaf.aftype		=	AFT_SPELL;
					oaf.type		=	gsn_ice_blast;
					oaf.level		=	level;
					oaf.location	=	0;
					oaf.modifier	=	0;
					oaf.duration	=	level/6;
					oaf.owner		=	ch;
					oaf.end_fun		=	container_defrost;
					oaf.tick_fun	=	NULL;
					affect_to_obj(obj,&oaf);
				}
				break;
			default:
				continue;
		}
	}

	return;
}

void container_defrost (OBJ_DATA *obj, OBJ_AFFECT_DATA *af)
{
	if (obj->carried_by)
		return act("The ice sealing $p melts.",obj->carried_by,obj,0,TO_CHAR);
}

void spell_icy_carapace (int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	OBJ_DATA *obj = NULL;
	AFFECT_DATA af;
	bool puddle = FALSE, fountain = FALSE;

	if (is_affected(ch,gsn_icy_carapace))
		return send_to_char("You are already protected by an icy carapace.\n\r",ch);

	if (ch->in_room->sector_type != SECT_WATER && ch->in_room->sector_type != SECT_UNDERWATER)
	{
		for (obj = ch->in_room->contents; obj; obj = obj->next_content)
		{
			if (obj->item_type == ITEM_FOUNTAIN)
				fountain = TRUE;
			if (obj->pIndexData->vnum == OBJ_VNUM_PUDDLE)
				puddle = TRUE;
		}
		if (!puddle && !fountain)
			return send_to_char("There is not enough water here to form an icy carapace.\n\r",ch);
	}

	if (fountain)
	{
		act("Water flows up from $p, forming an icy carapace around you.",ch,obj,0,TO_CHAR);
		act("Water flows up from $p, solidifying as ice around $n.",ch,obj,0,TO_ROOM);
	}
	else if (puddle)
	{
		act("Water flows up from the puddle on the ground, forming an icy carapace around you.",ch,0,0,TO_CHAR);
		act("Water flows up from the puddle on the ground, solidifying as ice around $n.",ch,0,0,TO_ROOM);
		if (obj)
			extract_obj(obj);
	}
	else
	{
		act("A shell of water flows up around you and solidifies into an icy carapace.",ch,0,0,TO_CHAR);
		act("A shell of water flows up around $n and solidifies into an icy carapace.",ch,0,0,TO_ROOM);
	}

	init_affect(&af);
	af.where	=	TO_AFFECTS;
	af.aftype	=	AFT_SPELL;
	af.type		=	gsn_icy_carapace;
	af.location	=	APPLY_AC;
	af.level	=	level;
	af.duration	=	(int)((float)level * 0.7);
	af.modifier	=	(int)((float)level * 0.82);
	af.owner	=	ch;
	affect_to_char(ch,&af);
	af.location	=	APPLY_DEX;
	af.modifier	=	-3;
	affect_to_char(ch,&af);
	af.aftype	=	AFT_INVIS;
	af.location	=	0;
	af.where	=	TO_RESIST;
	af.mod_name = MOD_RESISTANCE;
	SET_BIT(af.bitvector,RES_COLD);
	affect_to_char(ch,&af);
	
	return;
}

void spell_sheath_of_ice(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	OBJ_AFFECT_DATA oaf;
	OBJ_DATA *wield;

	if ((wield = get_eq_char(victim, WEAR_WIELD)) == NULL)
		return send_to_char("They are not wielding a weapon!\n\r",ch);

	if (is_affected_obj(wield,gsn_sheath_of_ice))
		return send_to_char("Their weapon is already sheathed in ice!\n\r",ch);
	
	if (ch->in_room->area->temp == TEMP_HOT)
		return send_to_char("It is too hot to create ice in such quantity.\n\r",ch);

	act("You coat $N's $p with a thick layer of ice.",ch,wield,victim,TO_CHAR);
	act("$p feels heavier in your hand as a thick layer of ice covers it.",ch,wield,victim,TO_VICT);
	act("$n coats $N's $p with a thick layer of ice.",ch,wield,victim,TO_NOTVICT);
	
	init_affect_obj(&oaf);
	oaf.where	=	TO_OBJ_AFFECTS;
	oaf.aftype	=	AFT_SPELL;
	oaf.type	=	gsn_sheath_of_ice;
	oaf.location	=	APPLY_OBJ_WEIGHT;
	oaf.modifier	=	UMAX(5,wield->weight * (level / 50));
	oaf.duration	=	level/5;
	oaf.level	=	level;
	oaf.owner	=	ch;
	oaf.end_fun	=	ice_sheath_melt;
	oaf.tick_fun=	NULL;
	oaf.pulse_fun=	NULL;
	affect_to_obj(wield,&oaf);

	/* Cheaphack to make it check for strength disarm... this is about as
	 * cheap as they come. :P
	 */
	init_affect(&af);
	af.type		=	gsn_bash;
	affect_to_char(ch,&af);

	affect_strip(ch,gsn_bash);
	
	return;
}

void ice_sheath_melt (OBJ_DATA *obj, OBJ_AFFECT_DATA *af)
{
	if (obj->carried_by)
		act("The ice surrounding $p melts.",obj->carried_by,obj,0,TO_CHAR);

	return;
}

void spell_ironskin (int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	
	if (is_affected(ch,gsn_ironskin)
		|| is_affected(ch,gsn_diamondskin)
		|| is_affected(ch,gsn_stoneskin))
		return send_to_char("Your skin is already magically hardenened.\n\r",ch);

	act("Your skin hardens and takes on a metallic tone.",ch,0,0,TO_CHAR);
	act("$n's skin hardens and takes on a metallic tone.",ch,0,0,TO_ROOM);

	init_affect(&af);
	af.where		=	TO_AFFECTS;
	af.aftype		=	AFT_SPELL;
	af.type			=	gsn_ironskin;
	af.owner		=	ch;
	af.level		=	level;
	af.location		=	APPLY_AC;
	af.modifier		=	level/3;
	af.duration		=	level/4;
	af.end_fun		=	NULL;
	new_affect_to_char(ch,&af);
	af.where		=	TO_VULN;
	af.location		=	0;
	af.modifier		=	0;
	af.mod_name = MOD_RESISTANCE;
	SET_BIT(af.bitvector, VULN_LIGHTNING);
	new_affect_to_char(ch,&af);
	
	return;
}

void spell_metal_shards (int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	OBJ_DATA *weapon;
	int number, dam, i;

	if((weapon = get_eq_char(ch,WEAR_WIELD)) == NULL)
		return send_to_char("You must be wielding a weapon to cast this spell.\n\r",ch);

	if (!IS_METAL(weapon))	
		return send_to_char("You are not wielding a metallic weapon!\n\r",ch);

	number = number_range(level/10 - 1, level/10 + 1);
	
	if (number <= 0)
		number = 1;

	act("You wave your $p in a broad arc, sending a hail of razor-sharp metal at $N!",ch,weapon,victim,TO_CHAR);
	act("$n waves $p in a broad arc, sending a hail of razor-sharp metal at you!",ch,weapon,victim,TO_VICT);
	act("$n waves $p in a broad arc, sending a hail of razor-sharp metal at $N!",ch,weapon,victim,TO_NOTVICT);

	for (i=0;i < number; i++) {
		dam = dice(1 + level/10,9);
		if ( (!str_cmp(weapon->material,"iron")
			&& !str_cmp(race_table[victim->race].name,"elf"))
			|| (!str_cmp(weapon->material,"mithril")
			&& !str_cmp(race_table[victim->race].name,"drow")))
			dam = (int)((float)dam * 1.33);
		damage_new(ch,victim,dam,sn,DAM_PIERCE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"metal shard");

		if (is_safe_new(ch,victim,FALSE))
			return;
		if (ch->in_room != victim->in_room)
			return;
	}	
	
	return;
}

void spell_burden (int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	if (is_affected(victim,gsn_burden))
		return send_to_char("They are already staggering under an additional burden.",ch);

	if (saves_spell(level,victim,DAM_OTHER)) {
		act("$N resisted your spell.",ch,0,victim,TO_CHAR);
		act("You feel a weight bearing down upon you briefly, but it goes away.",ch,0,victim,TO_VICT);
		act("$N seems slightly weighed down for a moment, but it passes.",ch,0,victim,TO_NOTVICT);
		return;
	} else {
		act("$n staggers beneath an unseen burden, struggling to remain standing.",victim,0,0,TO_ROOM);
		act("You stagger beneath an unseen burden, struggling to remain standing.",victim,0,0,TO_CHAR);
	}

	init_affect(&af);
	af.where	=	TO_AFFECTS;
	af.aftype	=	AFT_SPELL;
	af.type		=	gsn_burden;
	af.owner	=	ch;
	af.level	=	level;
	af.location	=	APPLY_CARRY_WEIGHT;
	af.modifier	=	(level * victim->carry_weight) / 50;
	af.duration	=	level/4;
	af.end_fun	=	NULL;
	new_affect_to_char(victim,&af);

	return;
}

void spell_fortify_weapon(int sn,int level, CHAR_DATA *ch, void *vo, int target)
{
	OBJ_DATA *weapon = (OBJ_DATA *) vo;
	OBJ_APPLY_DATA *hitapp = NULL, *damapp = NULL;
	bool damfound = FALSE, hitfound = FALSE;
	bool dambonus = FALSE, hitbonus = FALSE;
	int chance = 50, diff, oldhit = 0, olddam = 0;

	if (weapon->item_type != ITEM_WEAPON)
		return send_to_char("You can only cast this on weapons.\n\r",ch);

	if (!IS_METAL(weapon))
		return send_to_char("You can only fortify metallic weapons.\n\r",ch);

	if (is_worn(weapon))
		return send_to_char("You must remove it to enchant it.\n\r",ch);

	chance += (4 * (level - ((1 + weapon->value[2]) * weapon->value[1])));

	if (weapon->pIndexData->limtotal == 0)
		chance += 20;
	if (weapon->pIndexData->limtotal == 1 || weapon->pIndexData->limtotal == 2)
		chance -= 50;
	if (weapon->pIndexData->limtotal >= 3 && weapon->pIndexData->limtotal <= 20)
		chance -= (int)(50 - (2.5 * (float)weapon->pIndexData->limtotal));
	
	diff = chance - number_percent();

	if (diff > 50) {
		hitbonus = TRUE;
		dambonus = TRUE;
	} else if (diff > 0) {
		if (number_percent() > 50)
			dambonus = TRUE;
		else
			hitbonus = TRUE;
	} else if (diff > -25) {
		act("Your magic fails to take hold on $p.",ch,weapon,0,TO_CHAR);
		act("$p hums very briefly in $n's hands.",ch,weapon,0,TO_ROOM);
		return;
	} else {
		act("$p shudders and explodes in your hands!",ch,weapon,0,TO_CHAR);
		act("$p shudders and explodes in $n's hands!",ch,weapon,0,TO_ROOM);
		damage_new(ch,ch,dice(weapon->level,5),TYPE_UNDEFINED,DAM_ENERGY,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"the explosion*");
		extract_obj(weapon);
		return;
	}
	
	if (hitbonus) {
		for (hitapp = weapon->apply; hitapp; hitapp = hitapp->next)
		if (hitapp->location == APPLY_HITROLL && hitapp->type == sn) {
				hitfound 		= 	TRUE;
				oldhit			= 	hitapp->modifier;
				break;
			}
		if (!hitfound) {
			hitapp				=	new_apply_data();
			hitapp->type		=	sn;
			hitapp->location 	= 	APPLY_HITROLL;
			hitapp->modifier	=	0;
			hitapp->next 		= 	weapon->apply;
			weapon->apply 		= 	hitapp;
		}
	}

	if (dambonus) {
		for (damapp = weapon->apply; damapp; damapp = damapp->next)
			if (damapp->location == APPLY_DAMROLL && damapp->type == sn) {
				damfound 		= 	TRUE;
				olddam 			= 	damapp->modifier;
				break;
			}
		if (!damfound) {
			damapp				=	new_apply_data();
			damapp->type		=	sn;
			damapp->location 	= 	APPLY_DAMROLL;
			damapp->modifier	=	0;
			damapp->next 		= 	weapon->apply;
			weapon->apply 		= 	damapp;
		}
	}
		
	chance = 50;
	chance -= (int) (7.5 * (olddam + oldhit));
	chance += 4 * (level - ((weapon->value[2]) * weapon->value[1]));

	if (hitbonus && dambonus)
		chance += 30;

	if (olddam > 10 || oldhit > 10)
		chance = 2;

	if (hitbonus) {
		if (number_percent() < chance) {
			hitapp->modifier += 1;
			act("You feel $p's balance improve in your hands as it is infused with magic.",ch,weapon,0,TO_CHAR);
			act("$p rings with a light piercing tone as $n's magic works upon it.",ch,weapon,0,TO_ROOM);
		} else {
			if (number_percent() > chance) {
				act("$p shudders and explodes in your hands!",ch,weapon,0,TO_CHAR);
				act("$p shudders and explodes in $n's hands!",ch,weapon,0,TO_ROOM);
				damage_new(ch,ch,dice(weapon->level,5),TYPE_UNDEFINED,DAM_ENERGY,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"the explosion*");
				extract_obj(weapon);
				return;
			} else {
				hitapp->modifier -= 1;
				act("You feel $p twist in your hands as the magic goes awry.",ch,weapon,0,TO_CHAR);
				act("$p emits a discordant tone as $n's magic works upon it.",ch,weapon,0,TO_ROOM);
			}
		}
	}

	if (dambonus) {
		if (number_percent() < chance) {
			damapp->modifier += 1;
			act("You feel $p grow sturdier in your hands as it is infused with magic.",ch,weapon,0,TO_CHAR);
			act("$p flickers with a cerulean glow as $n's magic works upon it.",ch,weapon,0,TO_ROOM);
		} else {
			if (number_percent() > chance) {
				act("$p shudders and explodes in your hands!",ch,weapon,0,TO_CHAR);
				act("$p shudders and explodes in $n's hands!",ch,weapon,0,TO_ROOM);
				damage_new(ch,ch,dice(weapon->level,5),TYPE_UNDEFINED,DAM_ENERGY,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"the explosion*");
				extract_obj(weapon);
				return;
			} else {
				damapp->modifier -= 1;
				act("You feel $p grow frail in your hands as the magic goes awry.",ch,weapon,0,TO_CHAR);
				act("$p flickers with a crimson glow as $n's magic works upon it.",ch,weapon,0,TO_ROOM);
			}
		}
	}
	
	return;
}

void spell_fortify_armor(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	OBJ_DATA *armor = (OBJ_DATA *) vo;
	int chance = 50, i, avg_ac = 0;

	if (armor->item_type != ITEM_ARMOR)
		return send_to_char("You can only cast this on armor.\n\r",ch);

	if (!IS_METAL(armor))
		return send_to_char("You can only fortify metallic armor.\n\r",ch);

	if (is_worn(armor))
		return send_to_char("You must remove it to enchant it.\n\r",ch);

	for (i = 0; i < 4;i++)
		avg_ac += armor->value[i];

	avg_ac /= 4;

	chance -= 10 * (avg_ac - 5);
	chance += level;
	
	if (avg_ac == 9)
		chance -= 20;
	if (avg_ac == 10)
		chance -= 50;

	if (armor->pIndexData->limtotal > 0)
		chance -= 20;
	if (armor->pIndexData->limtotal == 1 || armor->pIndexData->limtotal == 2)
		chance -= 30;

	if (avg_ac > 10)
		chance = 2;

	if (number_percent() < chance) {
		if (number_percent() < chance) {
			for (i = 0; i < 4; i++)
				armor->value[i]++;
			if (armor->weight > 5) {
				armor->weight = (int)((float)armor->weight * 1.1);
			} else {
				armor->weight++;
			}
			act("$p feels sturdier in your hands, fortified by your magic.",ch,armor,0,TO_CHAR);
			act("$n focuses intently and $p glows briefly with an inner light.",ch,armor,0,TO_ROOM);
		} else {
			for (i = 0; i < 4; i++)
				armor->value[i]--;
			act("The magic goes awry, leaving $p feeling brittle in your hands.",ch,armor,0,TO_CHAR);
			act("$n focuses intently and a shadow briefly flickers across $p.",ch,armor,0,TO_ROOM);
		}
	} else {
		act("$p shudders and explodes in your hands!",ch,armor,0,TO_CHAR);
		act("$p shudders and explodes in $n's hands!",ch,armor,0,TO_ROOM);
		damage_new(ch,ch,dice(armor->level,5),TYPE_UNDEFINED,DAM_ENERGY,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"the explosion*");
		extract_obj(armor);
	}

	return;
}

void spell_alter_metal(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	char arg1[MSL], arg2[MSL];
	OBJ_DATA *obj;
	int chance = 0;

	target_name = one_argument(target_name,arg1);
	target_name = one_argument(target_name,arg2);

	if(arg1[0] == '\0')
		return send_to_char("Syntax: c 'alter metal' <object> <copper/silver/iron/steel/mithril>\n\r",ch);
	
	if((obj=get_obj_carry(ch,arg1,ch))==NULL)
		return send_to_char("You aren't carrying that.\n\r",ch);
	
	if (!IS_METAL(obj))
		return send_to_char("You can only cast this spell on metal objects.\n\r",ch);

	if (str_cmp(arg2,"iron") && str_cmp(arg2,"steel") && str_cmp(arg2,"copper")
		&& str_cmp(arg2,"mithril") && str_cmp(arg2,"silver"))
		return send_to_char("Syntax: c 'alter metal' <object> <copper/silver/iron/steel/mithril>\n\r",ch);

	if (!str_cmp(obj->material,arg2))
		return send_to_char("It's already made of that!\n\r",ch);
	
	if (!str_cmp(arg2,"copper"))
		chance = 80;

	if (!str_cmp(arg2,"iron"))
		chance = 65;

	if (!str_cmp(arg2,"steel"))
		chance = 50;

	if (!str_cmp(arg2,"silver"))
		chance = 35;

	if (!str_cmp(arg2,"mithril"))
		chance = 20;

	chance += 5 * (ch->level - obj->level);
	
	if (obj->pIndexData->limtotal > 0 && obj->pIndexData->limtotal < 4)
		chance = 0;

	if (obj->pIndexData->limtotal >= 4)
		chance -= 20;

	if (number_percent() < chance) {
		free_pstring(obj->material);
		obj->material = palloc_string(arg2);
		act("$p writhes in your grasp as the magic courses through it.",ch,obj,0,TO_CHAR);
		act("$p is reconstituted into $T!",ch,obj,arg2,TO_CHAR);
		act("$p writhes in $n's grasp, altered somehow.",ch,obj,0,TO_ROOM);
	} else {
		act("$p shudders and explodes in your hands!",ch,obj,0,TO_CHAR);
		act("$p shudders and explodes in $n's hands!",ch,obj,0,TO_ROOM);
		damage_new(ch,ch,dice(obj->level,5),TYPE_UNDEFINED,DAM_ENERGY,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"the explosion*");
		extract_obj(obj);
	}
	
	return;
}

void spell_cloak_of_mist(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;

	if(is_affected(ch, gsn_cloak_of_mist))
		return send_to_char("You are already cloaked with mist!\n\r", ch);
	
	act("You gather a thick cloak of mist around yourself.",ch,0,0,TO_CHAR);
	act("A thick cloak of mist gathers around $n.",ch,0,0,TO_ROOM);

	init_affect(&af);
	af.where		=	TO_AFFECTS;
	af.type			=	gsn_cloak_of_mist;
	af.aftype		=	AFT_SPELL;
	af.level		=	level;
	af.owner		=	ch;
	af.location		=	0;
	af.modifier		=	0;
	af.duration		=	level/4;
	af.end_fun		=	NULL;
	af.mod_name = MOD_PROTECTION;
	affect_to_char(ch,&af);

	return;
}

void spell_vigorize (int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *vch;
	int refresh = 0;

	if (ch->in_room->sector_type == SECT_UNDERWATER)
		return send_to_char("You cannot cast this spell underwater.\n\r",ch);

	refresh = level;

	if (ch->in_room->area->temp == TEMP_HOT
		|| ch->in_room->area->temp == TEMP_COLD)
		refresh /= 2;
	
	act("You blanket the area with a soothing mist.",ch,0,0,TO_CHAR);
	act("$n blankets the area with a soothing mist.",ch,0,0,TO_ROOM);
	
	for (vch = ch->in_room->people; vch; vch = vch->next_in_room) {
		if (!(is_same_group(ch,vch) || is_same_cabal(ch,vch)))
			refresh /= 3;
		send_to_char("As the mist settles over you, you feel refreshed!\n\r",vch);
		vch->move = UMIN(vch->move + refresh, vch->max_move);
	}

	return;
}

void spell_creeping_tomb(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	if(!IS_GROUND(ch->in_room))
		return send_to_char("You must be on land to cast this spell.\n\r",ch);

	if(saves_spell(level - 5, victim, DAM_OTHER))
	{
		act("$N resisted your spell.",ch,0,victim,TO_CHAR);
		act("Damp earth cakes briefly on your legs but crumbles rapidly to dust.",ch,0,victim,TO_VICT);
		act("Damp earth cakes briefly on $N's legs but crumbles rapidly to dust.",ch,0,victim,TO_NOTVICT);
		return;
	}

	act("A mass of living ooze begins to slowly encircle $N's legs.",ch,0,victim,TO_CHAR);
	act("A mass of living ooze coalesces beneath you and begins to creep up your legs!",ch,0,victim,TO_VICT);
	act("A mass of living ooze coalesces beneath $N and begins to creep up $S legs!",ch,0,victim,TO_NOTVICT);

	init_affect(&af);
	af.where		=	TO_AFFECTS;
	af.owner		=	ch;
	af.aftype		=	AFT_SPELL;
	af.type			=	gsn_creeping_tomb;
	af.level		=	level + 15;
	af.location		=	APPLY_DEX;
	af.modifier		=	-level/11;
	af.duration		=	8;
	af.end_fun		=	NULL;
	af.tick_fun		=	creeping_tomb_tick;
	af.mod_name = MOD_MOVEMENT;
	affect_to_char(victim,&af);

	return;
}

void creeping_tomb_tick(CHAR_DATA *ch, AFFECT_DATA *af)
{
	switch(af->duration) {
		case(4):
			act("The living ooze ascends further, threatening to completely engulf you!",ch,0,0,TO_CHAR);
			act("$n squirms uncomfortably as the living ooze covering $m creeps upward.",ch,0,0,TO_ROOM);
			af->modifier--;
			return;
		case(3):
			act("The living ooze suddenly solidifies, entombing you from head to toe!",ch,0,0,TO_CHAR);
			act("The mass of living ooze surrounding $n suddenly solidifies, entombing $m!",ch,0,0,TO_ROOM);
			return;
		case(2):
		case(1):
			act("The ooze that has hardened around you begins to crumble.",ch,0,0,TO_CHAR);
			return;
		case(0):
		case(-1):
			return;
		default:
			act("The living ooze covering you continues to creep inexorably upward.",ch,0,0,TO_CHAR);
			act("$n squirms uncomfortably as the living ooze covering $m creeps upward.",ch,0,0,TO_ROOM);
			af->modifier--;
			return;
	}
	
	return;
}

void spell_pass_without_trace(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	
	if (is_affected(ch,gsn_pass_without_trace))
		return send_to_char("Your tracks on land are already concealed.\n\r",ch);

	send_to_char("You mold the earth beneath you to conceal your tracks.\n\r",ch);
	
	init_affect(&af);
	af.where		=	TO_AFFECTS;
	af.aftype		=	AFT_SPELL;
	af.type			=	gsn_pass_without_trace;
	af.owner		=	ch;
	af.level		=	level;
	af.duration		=	level/6;
	af.location		=	0;
	af.modifier		=	0;
	af.mod_name = MOD_MOVEMENT;
	affect_to_char(ch, &af);

	return;
}

void spell_quicksand (int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	ROOM_AFFECT_DATA raf;
	AFFECT_DATA af;

	if (!IS_GROUND(ch->in_room))
		return send_to_char("You must be on land to cast this spell.\n\r",ch);

	if (is_affected(ch,gsn_quicksand))
		return send_to_char("You have not yet recovered from your last casting of this spell.\n\r",ch);
	
	act("You gesture at the ground beneath you, liquifying it to trap the unsuspecting.",ch,0,0,TO_CHAR);
	act("$n gestures at the ground, which seems to ripple ominously for a moment.",ch,0,0,TO_ROOM);
	
	init_affect_room(&raf);
	raf.where		=	TO_ROOM_AFFECTS;
	raf.aftype		=	AFT_SPELL;
	raf.type		=	gsn_quicksand;
	raf.owner		=	ch;
	raf.level		=	level;
	raf.location	=	0;
	raf.duration	=	24;
	raf.modifier	=	0;
	raf.tick_fun	=	NULL;
	raf.end_fun		=	quicksand_end;
	new_affect_to_room(ch->in_room,&raf);

	init_affect(&af);
	af.where		=	TO_AFFECTS;
	af.aftype		=	AFT_TIMER;
	af.type			=	gsn_quicksand;
	af.owner		=	ch;
	af.level		=	level;
	af.duration		=	48;
	affect_to_char(ch,&af);
	
	return;
}

void quicksand_end (ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *af)
{
	if (room->people)
		act("The ground beneath you solidifies.",room->people,0,0,TO_ALL);
	
	return;
}

void quicksand_pulse_sink (CHAR_DATA *ch, AFFECT_DATA *af)
{
	if(!IS_GROUND(ch->in_room) || !is_affected_room(ch->in_room,gsn_quicksand)){
		affect_remove(ch,af);	
		return;
	}
	
	if (IS_AFFECTED(ch,AFF_FLYING)) {
		affect_remove(ch,af);
		send_to_char("You float into the air, escaping from the quicksand.\n\r",ch);
		return;
	}
	
	if (number_percent() > ch->carry_weight / 5)
		return;

	send_to_char("You sink deeper into the quicksand, encumbered by your belongings!\n\r",ch);

	af->modifier++;

	return;
}

void spell_sap_endurance(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	
	if(!IS_GROUND(ch->in_room))
		return send_to_char("You must be on solid ground to cast this spell.\n\r",ch);

	act("A tendril of ooze springs from the earth to sap $N's endurance!",ch,0,victim,TO_CHAR);
	act("A tendril of ooze springs from the earth to sap your endurance!",ch,0,victim,TO_VICT);
	act("A tendril of ooze springs from the earth to sap $N's endurance!",ch,0,victim,TO_NOTVICT);

	victim->move = UMAX(0,victim->move - number_range((int)((float)level*.9),(int)((float)level*1.1)));

	return;
}

void spell_emulsify (int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int dam;
	float ddice = 1.5 * level;
	dam = dice((int)ddice,3);

	if (saves_spell(level, victim, DAM_INTERNAL))
		dam /= 2;
	
	act("$n writhes in agony as $s midsection suddenly distends.",victim,0,0,TO_ROOM);
	act("Your insides twist with pain, as your organs begin to dissolve into viscous pools.",ch,0,victim,TO_VICT);

	damage_new(ch,victim,dam,sn,DAM_INTERNAL,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"emulsification");	
	
	return;
}

void spell_rust (int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	OBJ_DATA *eq;
	OBJ_AFFECT_DATA oaf;

	act("You conjure a cloud of mist in an attempt to rust $N's armor!",ch,0,victim,TO_CHAR);
	
	if (saves_spell(level, victim, DAM_OTHER)) {
		act("A cloud of mist envelops you, but dissipates rapidly.",victim,0,0,TO_CHAR);
		act("A misty fog envelops $n, but dissipates rapidly.",victim,0,0,TO_ROOM);
		return;
	}

	act("A thick cloud of mist envelops you, swirling around your garments.",ch,0,victim,TO_VICT);
	act("A thick cloud of mist envelops $N, swirling around $S garments.",ch,0,victim,TO_NOTVICT);
	
	init_affect_obj(&oaf);
	oaf.where	=	TO_OBJ_APPLY;
	oaf.type	=	gsn_rust;
	oaf.aftype	=	AFT_SPELL;
	oaf.owner	=	ch;
	oaf.level	= 	level;
	oaf.duration=	level/4;
	oaf.location=	APPLY_DEX;
	
	for (eq = victim->carrying; eq; eq = eq->next_content) {
		if (eq->wear_loc == WEAR_NONE
			|| eq->wear_loc == WEAR_WIELD
			|| eq->wear_loc == WEAR_DUAL_WIELD
			|| eq->wear_loc == WEAR_SHIELD
			|| eq->wear_loc == WEAR_LIGHT
			|| eq->wear_loc == WEAR_HOLD
			|| eq->wear_loc == WEAR_BRAND)
			continue;

		if (number_percent() > 50)
			continue;
		
		if (!CAN_RUST(eq))
			continue;

		switch (eq->wear_loc) {
			case (WEAR_FINGER_L):
			case (WEAR_FINGER_R):
				oaf.modifier = 0;
				break;
			case (WEAR_NECK_1):
			case (WEAR_NECK_2):
			case (WEAR_WRIST_L):
			case (WEAR_WRIST_R):
				oaf.modifier = -1;
				break;
			case (WEAR_WAIST):
			case (WEAR_HEAD):
				oaf.modifier = -2;
				break;
			case (WEAR_ARMS):
			case (WEAR_HANDS):
			case (WEAR_FEET):
			case (WEAR_LEGS):
			case (WEAR_ABOUT):
				oaf.modifier = -3;
			case (WEAR_BODY):
				oaf.modifier = -4;
			default:
				continue;
		}

		act("$p is covered with a thick patina of rust!",victim,eq,0,TO_CHAR);
		act("$p is covered with a thick patina of rust!",ch,eq,0,TO_CHAR);

		affect_to_obj(eq,&oaf);
	}

	return;
}

void spell_airy_water (int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	ROOM_INDEX_DATA *room = ch->in_room;
	ROOM_AFFECT_DATA raf;

	if (room->sector_type != SECT_UNDERWATER)
		return send_to_char("This spell may only be cast underwater.\n\r",ch);

	if (is_affected_room(room,gsn_airy_water))
		return send_to_char("The water here is already aerated.\n\r",ch);

	act("You infuse the water around you with air, making it breathable.",ch,0,0,TO_CHAR);
	act("Bubbles fill the water as $n infuses the area with breathable air.",ch,0,0,TO_ROOM);

	init_affect_room(&raf);
	raf.where		=	TO_ROOM_AFFECTS;
	raf.type		=	gsn_airy_water;
	raf.aftype		=	AFT_SPELL;
	raf.owner		=	ch;
	raf.level		=	level;
	raf.duration	=	12;
	raf.location	=	0;
	raf.modifier	=	0;
	raf.end_fun		=	0;
	raf.tick_fun	=	0;
	affect_to_room(room,&raf);

	return;
}

void spell_cooling_mist (int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	if (is_affected(victim,gsn_cooling_mist))
		return send_to_char("They are already surrounded by a cooling mist.\n\r",ch);
	
	if (ch != victim) {
		act("You conjure a soothing mist to refresh $N and shelter $M from heat.",ch,0,victim,TO_CHAR);
	}
	
	act("A soothing mist envelops you, sheltering you from heat.",victim,0,0,TO_CHAR);
	act("A light mist envelops $n as $e appears soothed.",victim,0,0,TO_ROOM);

	init_affect(&af);
	af.where		=	TO_RESIST;
	af.aftype		=	AFT_SPELL;
	af.type			=	gsn_cooling_mist;
	af.owner		=	ch;
	af.level		=	level;
	af.location		=	0;
	af.modifier		=	0;
	af.duration		=	level/3;
	SET_BIT(af.bitvector, RES_FIRE);
	af.end_fun		=	0;
	af.tick_fun		=	0;
	af.mod_name = MOD_RESISTANCE;
	affect_to_char(victim,&af);
	
	return;
}

void spell_prismatic_spray(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	int color = number_range(1,7), dam = 0;
	bool poisoned = FALSE, disint = FALSE, blinded = FALSE;

	act("You send a diffuse spray of varicolored light streaking towards $N!",ch,0,victim,TO_CHAR);
	act("$n sends a diffuse spray of varicolored light streaking towards you!",ch,0,victim,TO_VICT);
	act("$n sends a diffuse spray of varicolored light streaking towards $N!",ch,0,victim,TO_NOTVICT);

	switch(color) {
		case(1):
			act("A red beam of light strikes $n!",victim,0,0,TO_ROOM);
			act("A red beam of light strikes you!",victim,0,0,TO_CHAR);
			dam = dice(level,2);
			if (saves_spell(level,victim,DAM_LIGHT))
				dam /= 2;
			break;
		case(2):
			act("An orange beam of light strikes $n!",victim,0,0,TO_ROOM);
			act("An orange beam of light strikes you!",victim,0,0,TO_CHAR);
			dam = dice(level,4);
			if (saves_spell(level,victim,DAM_LIGHT))
				dam /= 2;
			break;
		case(3):
			act("A yellow beam of light strikes $n!",victim,0,0,TO_ROOM);
			act("A yellow beam of light strikes you!",victim,0,0,TO_CHAR);
			dam = dice(level,7);
			if (saves_spell(level,victim,DAM_LIGHT))
				dam /= 2;
			break;
		case(4):
			act("A green beam of light strikes $n!",victim,0,0,TO_ROOM);
			act("A green beam of light strikes you!",victim,0,0,TO_CHAR);
			dam = dice(level,3);
			if (saves_spell(level,victim,DAM_LIGHT))
				dam /= 2;
			if (!saves_spell(level,victim,DAM_POISON))
				poisoned = TRUE;
			break;
		case(5):
			act("A blue beam of light strikes $n!",victim,0,0,TO_ROOM);
			act("A blue beam of light strikes you!",victim,0,0,TO_CHAR);
			dam = dice(level,3);
			if (saves_spell(level,victim,DAM_LIGHT))
				dam /= 2;
			LAG_CHAR(victim,2*PULSE_VIOLENCE);
			break;
		case(6):
			act("A indigo beam of light strikes $n!",victim,0,0,TO_ROOM);
			act("A indigo beam of light strikes you!",victim,0,0,TO_CHAR);
			dam = dice(level,3);
			if (saves_spell(level,victim,DAM_LIGHT))
				dam /= 2;
			if (saves_spell(level,victim,DAM_CHARM))
				blinded = TRUE;
			break;
		case(7):
			act("A violet beam of light strikes $n!",victim,0,0,TO_ROOM);
			act("A violet beam of light strikes you!",victim,0,0,TO_CHAR);
			dam = dice(level,3);
			if (!saves_spell(level,victim,DAM_INTERNAL))
				disint = TRUE;
			break;
	}

	damage_new(ch,victim,dam,sn,DAM_LIGHT,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"prismatic spray");

	if (poisoned && !is_affected(victim,gsn_poison)) {
		act("Poison creeps into your veins as the sickly light washes over you.",victim,0,0,TO_CHAR);
		act("$n looks very ill.",victim,0,0,TO_ROOM);
		init_affect(&af);
		af.where		=	TO_AFFECTS;
		af.type			=	gsn_poison;
		af.aftype		=	AFT_MALADY;
		af.owner		=	ch;
		af.level		=	level;
		af.location		=	APPLY_STR;
		af.modifier		=	-5;
		af.duration		=	level/4;
		SET_BIT(af.bitvector, AFF_POISON);
		af.tick_fun		=	poison_tick;
		af.end_fun		=	NULL;
		affect_to_char(victim,&af);
	}

	if (blinded) {
		act("The intense indigo light sears your eyes, blinding you!",victim,0,0,TO_CHAR);
		act("$n appears to be blinded.",victim,0,0,TO_ROOM);
		init_affect(&af);
		af.where		=	TO_AFFECTS;
		af.type			=	gsn_blindness;
		af.aftype		=	AFT_SPELL;
		af.owner		=	ch;
		af.level		=	level;
		af.location		=	APPLY_HITROLL;
		af.modifier		=	-4;
		af.duration		=	level/6;
		SET_BIT(af.bitvector, AFF_BLIND);
		af.tick_fun		=	NULL;
		af.end_fun		=	NULL;
		affect_to_char(victim,&af);
	}
		
	if (disint) {
		act("You feel a piercing pain in your chest as the violet light washes over you!",victim,0,0,TO_CHAR);
		act("$n writhes in agony as the violet light passes through $m!",victim,0,0,TO_ROOM);
		if (victim->level < ch->level - 9) {
			act("You gasp in horror as your body disintegrates into nothingness.",victim,0,0,TO_CHAR);
			act("A final look of horror crosses $n's face before $e disintegrates into dust.",victim,0,0,TO_ROOM);
			raw_kill(ch,victim);
		} else {
			damage_new(ch,victim,dice(level,9),sn,DAM_INTERNAL,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"disintegration");
		}
			
	}
	
	return;
}

void spell_earthfade (int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;

	if (is_affected(ch,gsn_earthfade))
		return send_to_char("You are already melded with the ground.\n\r",ch);

	if (!IS_GROUND(ch->in_room))
		return send_to_char("Fade into what earth?\n\r",ch);

	if (is_affected(ch,gsn_faerie_fire)
		|| is_affected(ch,gsn_faerie_fog)
		|| is_affected(ch,gsn_incandescense)
		|| is_affected(ch,gsn_mana_sickness))
		return send_to_char("You cannot fade into the earth while you are glowing!\n\r",ch);
	
	if (ch->pause > 2)
		return send_to_char("You are breathing too heavily to conceal yourself properly.\n\r",ch);

	act("The ground beneath $n liquifies and $e vanishes below the surface!",ch,0,0,TO_ROOM);
	act("You liquify the ground beneath you and conceal yourself within.",ch,0,0,TO_CHAR);

	init_affect(&af);
	af.where		=	TO_AFFECTS;
	af.aftype		=	AFT_SPELL;
	af.type			=	gsn_earthfade;
	af.level		=	level;
	af.owner		=	ch;
	af.modifier		=	0;
	af.location		=	0;
	af.duration		=	level / 5;
	af.end_fun		=	earthfade_end;
	af.tick_fun		=	earthfade_tick;
	af.mod_name = MOD_VISIBILITY;
	affect_to_char(ch,&af);

	return;
}

void earthfade_end(CHAR_DATA *ch, AFFECT_DATA *af)
{

	return;
}

void earthfade_tick(CHAR_DATA *ch, AFFECT_DATA *af)
{

	return;
}

void spell_plasma_arc( int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	act("You conjure a super-bright arc of plasma at $N!",ch,NULL,victim,TO_CHAR);
	act("$n conjures a super-bright arc of plasma at you!",ch,NULL,victim,TO_VICT);
	act("$n conjures a super-bright arc of plasma at $N!",ch,NULL,victim,TO_NOTVICT);

	if (saves_spell(level,victim,DAM_ENERGY))
	{
		send_to_char("Spell failed.\n\r",ch);
		send_to_char("You are unaffected.\n\r",victim);
		return;
	}

	init_affect(&af);
	af.where	= TO_AFFECTS;
	af.aftype	= get_spell_aftype(ch);
	af.type	= sn;
	af.level	= level;
	af.location	= 0;
	af.modifier	= 0;
	af.duration	= level/12;
	af.mod_name = MOD_VISION;
	affect_to_char(victim, &af);
	send_to_char("Your vision is disrupted!\n\r",victim);
	act("$n's vision is disrupted!",victim,NULL,NULL,TO_ROOM);

	return;
}


void spell_plasma_bolt(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *vch, *vch_next;
	ROOM_INDEX_DATA *pRoomIndex = NULL;
	char buf[MSL];
	int i;
	
	for (i = 0;i < 50000; i++)
	{
		pRoomIndex = get_random_room(ch);
		if ((pRoomIndex->area == ch->in_room->area
			&& !IS_SET(pRoomIndex->room_flags, ROOM_NO_RECALL)
			&&  !pRoomIndex->cabal) || pRoomIndex == ch->in_room)
			break;
	}

	if (i >= 49999)
		return send_to_char("You failed.\n\r",ch);

	act("You briefly envelop yourself in pure plasma, streaking away!",ch,0,0,TO_CHAR);
	act("$n flares with energy and streaks out of the area!",ch,0,0,TO_ROOM);

	for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
	{
		vch_next = vch->next_in_room;

		if (is_same_group(vch,ch) || is_safe(ch,vch))
			continue;

		if (!IS_NPC(vch))
		{
			sprintf(buf,"Die, %s you sorcerous dog!",PERS(ch,vch));
			do_myell(vch,buf,ch);
		}

		damage_new(ch,vch,level/2,sn,DAM_ENERGY,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);
	}
	stop_fighting(ch,TRUE);

	char_from_room(ch);
	char_to_room(ch, pRoomIndex);
	act("$n appears in a flash of plasma!",ch,0,0,TO_ROOM);
	act("The plasma around you fades as quickly as it formed, and you are elsewhere.",ch,0,0,TO_CHAR);
	do_look(ch, "auto");
	check_plasma_thread(ch,-1);

	for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
	{
		vch_next = vch->next_in_room;

		if (is_same_group(vch,ch) || is_safe(ch,vch))
			continue;

		if (!IS_NPC(vch))
		{
			sprintf(buf,"Die, %s you sorcerous dog!",PERS(ch,vch));
			do_myell(vch,buf,ch);
		}

		damage_new(ch,vch,level/2,sn,DAM_ENERGY,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);
	}

	return;
}

void sphere_of_plasma_end(CHAR_DATA *ch, AFFECT_DATA *paf)
{
	AFFECT_DATA af;

	if (paf->aftype == AFT_TIMER)
		return send_to_char("You feel ready to create another sphere of plasma.\n\r",ch);

	act("The sphere of plasma encasing you dissipates.",ch,NULL,NULL,TO_CHAR);

	init_affect(&af);
	af.where	= TO_AFFECTS;
	af.aftype	= AFT_TIMER;
	af.type	= gsn_sphere_of_plasma;
	af.level	= ch->level;
	af.location	= 0;
	af.modifier	= 0;
	af.duration	= ch->level/5;
	affect_to_char(ch, &af);

	return;
}

void sphere_of_plasma_pulse(CHAR_DATA *ch, AFFECT_DATA *af)
{
	if (!ch->fighting)
		return;

	if (number_percent() > 60)
		return;
	
	act("A tendril of plasma from the sphere encircling you lashes out at $N!",ch,0,ch->fighting,TO_CHAR);
	act("A writhing, pulsating tendril of plasma lashes out from $n's body!",ch,0,0,TO_ROOM);
	
	damage_new(ch,ch->fighting,af->level,gsn_sphere_of_plasma,DAM_TRUESTRIKE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);

	if (--af->modifier <= 0)
		affect_remove(ch,af);

	return;
}

void spell_sphere_of_plasma( int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;

	if (is_affected(ch,gsn_sphere_of_plasma))
		return send_to_char("You cannot create another sphere of plasma yet.\n\r",ch);

	act("Rippling links of plasma join to form a massive sphere of plasma around you!",ch,NULL,NULL,TO_CHAR);
	act("Rippling links of plasma join to form a massive sphere of plasma around $n!",ch,NULL,NULL,TO_ROOM);

	init_affect(&af);
	af.where	= TO_AFFECTS;
	af.aftype	= AFT_SPELL;
	af.type		= sn;
	af.level	= level;
	af.owner	= ch;
	af.location	= 0;
	af.modifier	= dice(level/13,2);
	af.duration	= level/5;
	af.end_fun	= sphere_of_plasma_end;
	af.pulse_fun= sphere_of_plasma_pulse;
	af.mod_name = MOD_PROTECTION;
	affect_to_char(ch, &af);

	die_follower(ch);

	return;
}

void spell_plasma_cube(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	ROOM_AFFECT_DATA raf;

	if (is_affected_room(ch->in_room,sn))
		return send_to_char("This room already has a plasma cube.\n\r",ch);

	act("You conjure a massive cube of interlaced plasma in the room.",ch,0,0,TO_CHAR);
	act("$n conjures a massive cube of interlaced plasma in the room.",ch,0,0,TO_ROOM);

	init_affect_room(&raf);
	raf.where	=	TO_ROOM_AFFECTS;
	raf.type	=	sn;
	raf.aftype	=	AFT_SPELL;
	raf.owner	=	ch;
	raf.level	=	level;
	raf.duration	=	12;
	raf.location	=	0;
	raf.modifier	=	0;
	affect_to_room(ch->in_room,&raf);

	return;
}

void spell_essence_of_plasma(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	ROOM_INDEX_DATA *room;
	ROOM_AFFECT_DATA raf;
	AFFECT_DATA af;

	room = ch->in_room;

	if(is_affected(ch,gsn_essence_of_plasma))
		return send_to_char("You cannot yet call upon the essence of plasma again.",ch);

	if(is_affected_room(room,gsn_essence_of_plasma))
		return send_to_char("The essence of plasma is already manifested here!",ch);

	act("As you gesture, an intensely bright point of radiance appears before you.",ch,0,0,TO_CHAR);
	act("An intensely bright point of radiance appears before you.",ch,0,0,TO_ROOM);

	act("The brilliant singularity flares into a pulsating ball of pure plasma!",ch,0,0,TO_ROOM);

	init_affect_room(&raf);
	raf.where		=	TO_ROOM_AFFECTS;
	raf.aftype		=	AFT_SPELL;
	raf.type		=	gsn_essence_of_plasma;
	raf.owner		=	ch;
	raf.level		=	level;
	raf.location	=	0;
	raf.duration	=	12;
	raf.modifier	=	0;
	raf.end_fun		=	essence_of_plasma_end;
	raf.pulse_fun	=	essence_of_plasma_pulse;
	new_affect_to_room(room,&raf);

	init_affect(&af);
	af.where		=	TO_AFFECTS;
	af.aftype		=	AFT_TIMER;
	af.type			=	gsn_essence_of_plasma;
	af.owner		=	ch;
	af.level		=	level;
	af.location		=	0;
	af.duration		=	12;
	af.modifier		=	1;
	affect_to_char(ch,&af);	
	
	return;
}

void essence_of_plasma_pulse(ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *af)
{
	CHAR_DATA *victim;
	bool valid_target = FALSE, found = FALSE, fighting = FALSE;
	int dam;
	
	if (!room->people)
		return;

	for(victim = room->people; victim; victim = victim->next_in_room)
		if (!is_safe_new(af->owner,victim,FALSE))
			valid_target = TRUE;
		
	if (!valid_target)
		return;

	while (!found) {
		for (victim = room->people; victim; victim = victim->next_in_room) {
			if (is_safe_new(af->owner,victim,FALSE))
				continue;
			if (number_percent() < 80)
				continue;
			found = TRUE;
			break;
		}
	}
	
	if (victim->fighting)
		fighting = TRUE;
	
	if (af->modifier == 0)
		af->modifier = 1;
	
	dam = dice(af->level,10) / af->modifier;
	
	act("The ball of plasma hurtles about wildly, discharging pure energy into you!",victim,0,0,TO_CHAR);
	act("The ball of plasma hurtles about wildly, discharging pure energy into $n!",victim,0,0,TO_ROOM);

	damage_new(af->owner,victim,dam,gsn_essence_of_plasma,DAM_ENERGY,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"the essence of plasma*");

	if (!fighting && !IS_NPC(victim))
		stop_fighting(victim,TRUE);
	
	af->modifier++;

	if (af->modifier >= 25)
		affect_remove_room(room, af);
	
	return;
}

void essence_of_plasma_end(ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *af)
{
	if (!room->people)
		return;

	act("The writhing mass of plasma flickers one last time and then dissipates.",room->people,0,0,TO_ALL);
	
	return;
}

void plasma_thread_end(CHAR_DATA *ch, AFFECT_DATA *paf)
{
	if (paf->owner)
		act("The thread of plasma no longer connects you to $N.",ch,NULL,paf->owner,TO_CHAR);

	return;
}

void spell_plasma_thread(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	CHAR_DATA *victim;

	if (is_affected(ch,sn))
		return send_to_char("You are already bound in a thread of plasma.\n\r",ch);

	if (!strcmp(target_name,""))
		return send_to_char("You cannot cast this on yourself.\n\r",ch);

	if ((victim = get_char_room(ch, target_name)) == NULL)
		return send_to_char("They aren't here.\n\r",ch);

	if (!trusts(ch,victim) && is_safe(ch,victim))
		return;

	act("Concentrating intently, you conjure forth a plasma conduit at $N!",ch,0,victim,TO_CHAR);
	act("Reaching $s arms toward you, $n conjures forth a plasma conduit!",ch,0,victim,TO_VICT);
	act("Reaching $s arms toward $N, $n conjures forth a plasma conduit!",ch,0,victim,TO_NOTVICT);

	if (!trusts(ch,victim))
	{
		if (saves_spell(level,victim,DAM_ENERGY))
		{
			act("The thread of plasma dissipates before connecting you to $M.",ch,0,victim,TO_CHAR);
			act("The thread of plasma dissipates before connecting $m to you.",ch,0,victim,TO_VICT);
			act("The thread of plasma dissipates before connecting $n to $N.",ch,0,victim,TO_NOTVICT);
			multi_hit(victim,ch,TYPE_UNDEFINED);
			return;
		}
	}

	act("The rippling thread of plasma connects, attaching you to $N!",ch,0,victim,TO_CHAR);
	act("The rippling thread of plasma connects, attaching $n to you!",ch,0,victim,TO_VICT);
	act("The rippling thread of plasma connects, attaching $n to $N!",ch,0,victim,TO_NOTVICT);

	init_affect(&af);
	af.where		=	TO_AFFECTS;
	af.aftype		=	AFT_SPELL;
	af.type			=	sn;
	af.owner		=	ch;
	af.level		=	level;
	af.location		=	0;
	af.duration		=	2;
	af.modifier		=	0;
	af.end_fun		=	plasma_thread_end;
	affect_to_char(victim,&af);

	init_affect(&af);
	af.where		=	TO_AFFECTS;
	af.aftype		=	AFT_SPELL;
	af.type			=	sn;
	af.owner		=	victim;
	af.level		=	level;
	af.location		=	0;
	af.duration		=	2;
	af.modifier		=	0;
	af.end_fun		=	plasma_thread_end;
	affect_to_char(ch,&af);

	if (!trusts(ch,victim)) multi_hit(victim,ch,TYPE_UNDEFINED);

	return;
}

void check_plasma_thread(CHAR_DATA *ch, int direction)
{
	CHAR_DATA *victim;
	AFFECT_DATA *af;

	af = affect_find(ch->affected, gsn_plasma_thread);

	if (af != NULL)
	{
		victim = af->owner;
		if (victim == NULL)
		{
			affect_remove(ch,af);
			return;
		}
		if (!is_affected(victim,gsn_plasma_thread))
		{
			affect_remove(ch,af);
			return;
		}
	}
	else
	{
		return;
	}

	if (ch->in_room == victim->in_room)
		return;

	if (direction == -1)
	{
		act("The rippling thread of plasma pulls $N along with you.",ch,0,victim,TO_CHAR);
		act("A rippling thread of plasma pulls $N along with $n.",ch,0,victim,TO_ROOM);
		act("The rippling thread of plasma pulls you along with $N.",victim,0,ch,TO_CHAR);
		act("A rippling thread of plasma pulls $n along with $N.",victim,0,ch,TO_ROOM);
		char_from_room(victim);
		char_to_room(victim,ch->in_room);
		do_look(victim,"auto");
	}
	else
	{
		act("The thread of plasma pulls $N along with you.",ch,0,victim,TO_CHAR);
		act("A thread of plasma pulls $N along with $n.",ch,0,victim,TO_ROOM);
		act("The thread of plasma pulls you along with $N.",victim,0,ch,TO_CHAR);
		act("A thread of plasma pulls $n along with $N.",victim,0,ch,TO_ROOM);
		move_char(victim, direction, TRUE, TRUE);
	}

	if (ch->in_room != victim->in_room)
	{
		act("The writhing thread of plasma suddenly snaps you back to $N!",ch,0,victim,TO_CHAR);
		act("A writhing thread of plasma suddenly snaps $n back to $N!",ch,0,victim,TO_ROOM);
		act("The writhing thread of plasma suddenly snaps $N back to you!",victim,0,ch,TO_CHAR);
		act("The writhing thread of plasma suddenly snaps $N back to $n!",victim,0,ch,TO_ROOM);
		char_from_room(ch);
		char_to_room(ch,victim->in_room);
		do_look(ch,"auto");
	}
}

void spell_accumulate_heat(int sn,int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if (!IS_GROUND(ch->in_room)) {
        send_to_char("You must be in contact with the ground to cast this spell.\n\r",ch);
        return;
    }

    if ( is_affected( ch, sn ))
    {
            send_to_char("You are already drawing heat to you.\n\r",ch);
    }
    else
    {
		init_affect(&af);
        af.where     = TO_RESIST;
        af.aftype    = get_spell_aftype(ch);
		SET_BIT(af.bitvector, RES_FIRE);
		af.type      = sn;
        af.level     = level;
        af.duration  = level/2;
        af.modifier  = 0;
		af.mod_name = MOD_RESISTANCE;
		affect_to_char( victim, &af );
        act( "Waves of heat begin to radiate from $n.", victim, NULL, NULL, TO_ROOM );

        send_to_char( "Waves of heat raise from the ground and begin radiating from you.\n\r", victim );
    }
    return;
}

void spell_melt_rock(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    OBJ_DATA *obj;
    int iWear, dam;

    act("A wave of oppressive heat emanates from your fingertips as you gesture at $N.",ch,0,victim,TO_CHAR);
    act("As $n gestures, a wave of oppressive heat washes over you.",ch,0,victim,TO_VICT);
    act("$n gestures, sending a wave of oppressive heat washing over $N.",ch,0,victim,TO_NOTVICT);

    for ( iWear = 0; iWear < MAX_WEAR; iWear++ )
    {
        if (((obj = get_eq_char(victim, iWear)) == NULL) ||
        	(iWear == WEAR_WIELD) ||
        	(iWear == WEAR_DUAL_WIELD))
            continue;

		if(IS_OBJ_STAT(obj, ITEM_BURN_PROOF))
        	continue;

		if(!IS_STONE(obj))
			continue;

    	if (number_range(0, 100) > (40 + (ch->level / 2)))
        	continue;

		act("$p melts under the intense heat, scorching you!",ch,obj,victim,TO_VICT);
		act("$p melts under the intense heat, scorching $m!",ch,obj,victim,TO_CHAR);
		act("$p melts under the intense heat, scorching $m!",ch,obj,victim,TO_NOTVICT);
    	dam = dice(obj->level,2)/2;
		damage_new(ch,victim,dam,sn,DAM_BASH,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"The molten rock*");
		extract_obj(obj);
    }

	if(is_affected(victim, skill_lookup("stoneskin")))
	{
		if(check_dispel(ch->level+3,victim,skill_lookup("stoneskin")))
		{
			act("Your stoneskin melts under the intense heat, scorching you!",ch,obj,victim,TO_VICT);
			act("$N's stoneskin melts under the intense heat, scorching $m!",ch,obj,victim,TO_CHAR);
			act("$N's stoneskin melts under the intense heat, scorching $m!",ch,obj,victim,TO_NOTVICT);
			damage_new(ch,victim,dice(ch->level,10),sn,DAM_BASH,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"The molten rock*");
		}
	}
	return;
}

void spell_magma_tunnel(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    ROOM_INDEX_DATA *to_room = NULL, *old_room = NULL;
	EXIT_DATA *pexit;
	int where, count=0, mcount=(ch->level/15)+2;
	bool next=TRUE;

	old_room = ch->in_room;
	if((where = direction_lookup(target_name)) < 0)
		return send_to_char("Syntax: cast 'magma tunnel' <direction>\n\r",ch);
	if(old_room->sector_type == SECT_UNDERWATER || old_room->sector_type == SECT_WATER || old_room->sector_type == SECT_AIR ||
           old_room->sector_type == SECT_VERTICAL)
		return send_to_char("You can't create a magma tunnel here.\n\r",ch);
	while(next)
	{
		if(count >= mcount)
			break;
		if((pexit = old_room->exit[where]) == NULL || (to_room = pexit->u1.to_room) == NULL) //can't move any further
		{
			if(count > 0)
				break;
			else
				return send_to_char("You can not magma tunnel that direction.\n\r",ch);
		}
		count++;
    		if(to_room->sector_type == SECT_UNDERWATER ||
			to_room->sector_type == SECT_WATER ||
	   		to_room->sector_type == SECT_AIR ||
	   		to_room->sector_type == SECT_VERTICAL ||
			to_room == ch->in_room)
				break;
		old_room = to_room;
	}
	act("As you gesture, the ground around your feet turns to magma and you are engulfed.", ch, 0, 0, TO_CHAR);
	act("The ground around $n's feet turns into magma and $e is engulfed.",ch, 0, 0, TO_ROOM);
	char_from_room(ch);
	char_to_room(ch, old_room);
	do_look(ch,"auto");
	act("You emerge from the ground.", ch, 0, 0, TO_CHAR);
	act("$n emerges from the ground.", ch, 0, 0, TO_ROOM);
	return;
}

void spell_fashion_crystal(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{	
	OBJ_DATA *obj;
	OBJ_AFFECT_DATA oaf;
	int mana = 100;
	
	if (target_name[0] != '\0') {
		if (is_number(target_name))
			mana += atoi(target_name);
		else
			return send_to_char("Syntax: c 'fashion crystal' [amount of mana]\n\r",ch);
	}

	if (mana > ch->mana + 100)
		return send_to_char("You don't have that much mana to channel!\n\r",ch);
	else {
		ch->mana -= mana;
		ch->mana += 100;
	}

	act("You focus intently, channeling raw mana to form a pure energy crystal.",ch,0,0,TO_CHAR);
	act("$n focuses intently and a flash of luminescence flickers in $s hands.",ch,0,0,TO_ROOM);
	if (mana > 100)
		act("You channel additional mana into forming the crystal, infusing it with power.",ch,0,0,TO_CHAR);

	obj = create_object(get_obj_index(OBJ_VNUM_CRYSTAL),level);
	obj_to_char(obj,ch);

	init_affect_obj(&oaf);
	oaf.where		=	TO_OBJ_AFFECTS;
	oaf.type		=	gsn_fashion_crystal;
	oaf.aftype		=	AFT_SPELL;
	oaf.owner		=	ch;
	oaf.level		=	level;
	oaf.location	=	0;
	oaf.modifier	=	mana;
	oaf.duration	=	-1;
	oaf.tick_fun	=	crystal_tick;
	affect_to_obj(obj,&oaf);

	return;
}

void crystal_tick(OBJ_DATA *obj, OBJ_AFFECT_DATA *af)
{
	af->modifier = (int)((float)af->modifier * 0.96);

	if (af->modifier <= 0) {
		if (obj->carried_by)
			act("$p crumbles to dust as the energy contained within dissipates.",obj->carried_by,obj,0,TO_CHAR);
		extract_obj(obj);
	}

	return;
}

void spell_farsee (int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;

	if (is_affected(ch,gsn_farsee))
		return send_to_char("Your vision is already magically heightened.\n\r",ch);
	
	act("Your vision sharpens as you attune yourself to magical crystals.",ch,0,0,TO_CHAR);
	
	init_affect(&af);
	af.where		=	TO_AFFECTS;
	af.aftype		=	AFT_SPELL;
	af.type			=	gsn_farsee;
	af.level		=	level;
	af.owner		=	ch;
	af.location		=	0;
	af.modifier		=	0;
	af.duration		=	level/4;
	af.mod_name = MOD_VISION;
	affect_to_char(ch,&af);

	return;
}

void spell_mana_beam (int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	OBJ_AFFECT_DATA *af;
	OBJ_DATA *obj;
	int dam;

	if ((obj = get_eq_char(ch,WEAR_HOLD)) == NULL)
		return send_to_char("You must be holding a charged crystal to cast this spell.\n\r",ch);

	af = affect_find_obj(obj->affected,gsn_fashion_crystal);

	if (!af)
		return send_to_char("You must be holding a *charged* crystal to cast this spell.\n\r",ch);

	act("You thrust forth your pulsing crystal, unleashing a beam of mana at $N!",ch,0,victim,TO_CHAR);
	act("$n thrusts forth a pulsing crystal, unleashing a beam of mana to sear you!",ch,0,victim,TO_VICT);
	act("$n thrusts forth a pulsing crystal, unleashing a beam of mana at $N!",ch,0,victim,TO_NOTVICT);

	dam = number_range(35,45) * af->modifier;
	dam /= 100;

	af->modifier = (int)((float)af->modifier * 0.9);

	damage_new(ch,victim,dam,sn,DAM_ENERGY,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"mana beam");

	if (af->modifier == 0) {
		act("$p crumbles to dust in your hands.",ch,obj,0,TO_CHAR);
		extract_obj(obj);
	}
	
	return;
}

void spell_detonation (int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *vch, *vch_next;
	OBJ_AFFECT_DATA *af;
	OBJ_DATA *obj;
	int dam;

	if ((obj = get_eq_char(ch,WEAR_HOLD)) == NULL)
		return send_to_char("You must be holding a charged crystal to cast this spell.\n\r",ch);

	af = affect_find_obj(obj->affected,gsn_fashion_crystal);

	if (!af)
		return send_to_char("You must be holding a *charged* crystal to cast this spell.\n\r",ch);

	act("You hurl $p, sending it arcing through the air!",ch,obj,0,TO_CHAR);
	act("$n hurls $p, sending it arcing through the air!",ch,obj,0,TO_ROOM);
	act("You focus on the energy contained within $p and unleash a massive explosion!",ch,obj,0,TO_CHAR);
	act("$p suddenly explodes in mid-air, unleashing a torrent of raw energy!",ch,obj,0,TO_ROOM);

	for (vch = ch->in_room->people; vch; vch = vch_next) {
		vch_next = vch->next_in_room;
		if (vch == ch)
			continue;
		if (!IS_NPC(vch) && is_safe_new(ch,vch,TRUE))
			continue;
		if (is_same_group(ch,vch) || is_same_cabal(ch,vch))
			continue;
		
		dam = number_range(45,60) * af->modifier;
		dam /= 100;
		
		damage_new(ch,vch,dam,sn,DAM_ENERGY,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"detonation");
	}

	extract_obj(obj);

	return;
}

void spell_rotating_ward(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	OBJ_AFFECT_DATA *oaf;
	AFFECT_DATA *paf, af;
	OBJ_DATA *obj;
	int charges;
	bool affected = FALSE;
	int count = 0;

	if ((obj = get_eq_char(ch,WEAR_HOLD)) == NULL)
		return send_to_char("You must be holding a charged crystal to cast this spell.\n\r",ch);

	oaf = affect_find_obj(obj->affected,gsn_fashion_crystal);

	if (!oaf)
		return send_to_char("You must be holding a *charged* crystal to cast this spell.\n\r",ch);

	charges = UMAX(1,oaf->modifier / 100);

	if (is_affected(ch,gsn_rotating_ward)) {
		for (paf = ch->affected; paf; paf = paf->next)
			if (paf->type == gsn_rotating_ward)
				count++;
		affected = TRUE;
	}

	if (count >= (ch->level / 25))
		return send_to_char("You cannot control so many orbiting crystals at the same time.\n\r",ch);

	if (affected)
		act("You raise your hand and release another charged crystal to circle your body.",ch,0,0,TO_CHAR);
	else
		act("You raise your hand, release your crystal, and it begins orbiting your body.",ch,0,0,TO_CHAR);
		
	act("$n holds out a multifaceted crystal and it begins orbiting $s body.",ch,0,0,TO_ROOM);

	init_affect(&af);
	af.where		=	TO_AFFECTS;
	af.type			=	gsn_rotating_ward;
	af.aftype		=	AFT_SPELL;
	af.owner		=	ch;
	af.level		=	level;
	af.modifier		=	charges;
	af.duration		=	level/2;
	af.location		=	0;
	af.tick_fun		=	rotating_tick;
	af.mod_name = MOD_PROTECTION;
	affect_to_char(ch,&af);

	extract_obj(obj);
	
	return;
}

void rotating_tick (CHAR_DATA *ch, AFFECT_DATA *af)
{
	if (number_percent() < 15)
		af->modifier--;

	if (af->modifier <= 0) {
		send_to_char("A crystal orbiting you disintegrates.\n\r",ch);
		affect_remove(ch,af);
	}

	return;
}

void spell_fortify_crystal (int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	OBJ_DATA *crystal = (OBJ_DATA *) vo;
	OBJ_AFFECT_DATA *af;

	af = affect_find_obj(crystal->affected, gsn_fashion_crystal);

	if (!af)
		return send_to_char("That crystal is devoid of any charge!\n\r",ch);

	act("$p hums briefly as you stabilize the energies within it.",ch,crystal,0,TO_CHAR);

	af->tick_fun	= 	NULL;
	af->modifier /= 2;

	return;
}
void mana_infusion_helper(CHAR_DATA *ch, CHAR_DATA *victim)
{
	AFFECT_DATA af;
        act("Mana continues to flow out of $N, leaving $M glowing brightly.",ch,0,victim,TO_CHAR);
        act("Mana continues to flow out of $N, leaving $M glowing brightly.",ch,0,victim,TO_NOTVICT);
	act("Mana continues to flow out of you, leaving you glowing brightly.",ch,0,victim,TO_VICT);
	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = gsn_mana_sickness;
	af.aftype = AFT_MALADY;
	af.location = 0;
	af.duration = number_range(1,3);
	af.modifier = 0;
	af.owner = ch;
	af.tick_fun = infusion_tick;
	affect_to_char(victim,&af);
}
void spell_mana_infusion(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int dammod;
	AFFECT_DATA af1,af;

	if(is_affected(victim, gsn_mana_infusion))
		return send_to_char("They are already infused with mana!\n\r", ch);

	
	if(ch->level < 10)
		dammod = 300;
	else if(10 > ch->level < 15)
		dammod = 200;
	else
		dammod = 120;

	act("You infuse $N's body with pure mana, overwhelming it!",ch,0,victim,TO_CHAR);
	act("$n gestures at you and your body suddenly feels full!",ch,0,victim,TO_VICT);
	act("$n gestures at $N, who begins to glow.",ch,0,victim,TO_NOTVICT);

	init_affect(&af1);
	af1.where = TO_AFFECTS;
	af1.type = gsn_mana_infusion;
	af1.aftype = AFT_INVIS;
	af1.duration = 2;
	af1.owner = ch;
	new_affect_to_char(victim, &af1);

	RS.Queue.AddToQueue(2, 8, damage_queue, ch, victim, level, DAM_OTHER, HIT_UNBLOCKABLE, HIT_NOADD, dammod,(char *)"the escaping mana*");
	RS.Queue.AddToQueue(2, 2, affect_strip, victim, gsn_mana_infusion);
	RS.Queue.AddToQueue(2, 5, act,"You scream in pain as the mana rushes from your body!",victim,0,0,TO_CHAR);
	RS.Queue.AddToQueue(2, 5, act,"As the mana rushes out of $n's body, $e cries out in pain!",victim,0,0,TO_ROOM);

	if(ch->level > 40 && number_percent() > 50 && !is_affected(victim,gsn_mana_sickness) && victim->hit > level + 30)
		RS.Queue.AddToQueue(3,2,mana_infusion_helper,ch,victim);
	return;
}

void infusion_tick(CHAR_DATA *ch, AFFECT_DATA *af)
{
	act("You writhe in pain as the mana continues to flow out of you.",ch,0,0,TO_CHAR);
	act("$n writhes in pain as the mana continues to flow out of $m.",ch,0,0,TO_ROOM);
	
	damage_new(af->owner ? af->owner : ch,ch,af->owner ? af->owner->level/2 : ch->level/2,gsn_mana_sickness,TRUE,DAM_OTHER,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"the escaping mana*");
	return;
}
