#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "merc.h"
#include "necro.h"
#include "recycle.h"
#include "tables.h"
#include "lookup.h"
#include "magic.h"
#include "interp.h"

void spell_dark_vessel(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	OBJ_DATA *urn, *corpse;

	if(find_urn(ch))
		return send_to_char("You already have a dark vessel in your possession.\n\r",ch);
	
	corpse	= get_obj_here(ch,target_name);
	
	if(!corpse || !str_cmp(target_name,""))
		return send_to_char("You don't see that here.\n\r",ch);
	
	if ((corpse->item_type != ITEM_CORPSE_PC &&
		corpse->item_type != ITEM_CORPSE_NPC) || IS_SET(corpse->wear_flags,CORPSE_NO_ANIMATE))
		return send_to_char("That wouldn't be a suitable dark vessel.\n\r",ch);
	
	if(corpse->level<(ch->level-9))
		return send_to_char("That corpse isn't powerful enough to contain your dark energies.\n\r",ch);
	
	urn     = create_object(get_obj_index(OBJ_VNUM_URN),0);

	if(!urn)
		return;

	urn->level    = ch->level;
	urn->value[4] = 0;
	urn->value[3] = corpse->level;
	
	obj_to_char(urn,ch);
	SET_BIT(corpse->wear_flags,ITEM_NO_SAC);
	REMOVE_BIT(corpse->wear_flags,ITEM_TAKE);
	WAIT_STATE(ch, PULSE_VIOLENCE*2);
	
	act("$n plunges a hand into $p's chest cavity, and removes the heart.",ch,corpse,0,TO_ROOM);
	RS.Queue.AddToQueue(2, 5, act, "$n holds the heart aloft as it visibly begins to harden.", ch, 0, 0, TO_ROOM);
	send_to_char("You remove the corpse's heart and squeeze the blood out of it, hardening it into a solid object.\n\r",ch);
	
	RS.Queue.AddToQueue(4, 2, make_urn, ch, corpse);
}

void make_urn(CHAR_DATA *ch, OBJ_DATA *corpse)
{
	char buf[MSL];
	act("When this morbid sculpting is complete, $n is holding a solid receptacle.",ch,0,0,TO_ROOM);
	act("You craft a morbid urn from the coagulated blood of $p!",ch,corpse,0,TO_CHAR);
	if(corpse->item_type == ITEM_CORPSE_NPC)
		extract_obj(corpse);
	else if(corpse->item_type == ITEM_CORPSE_PC && corpse->owner)
	{
		free_pstring(corpse->description);
		sprintf(buf,"The defiled corpse of %s is here, its heart missing.",corpse->owner);
		corpse->description = palloc_string(buf);
		SET_BIT(corpse->wear_flags, CORPSE_NO_ANIMATE);
	}
}

OBJ_DATA *find_urn(CHAR_DATA *ch)
{
	OBJ_DATA *urn;
	for(urn = ch->carrying; urn!=NULL; urn = urn->next_content)
		if(urn->item_type == ITEM_URN)
			return urn;
	return NULL;
}

bool drain_urn(CHAR_DATA *ch, int charges)
{
	OBJ_DATA *urn = find_urn(ch);
	char buf[300];

	if(!urn) {
		send_to_char("You do not have an urn from which to drain blood!\n\r",ch);
		return FALSE;
	}

	if(urn->value[4]<charges) {
		send_to_char("Your urn does not contain enough blood to do that.\n\r",ch);
		return FALSE;
	}
	
	urn->value[4] -= charges;

	sprintf(buf,"You harness %s ounce%s of blood to strengthen your dark magic.\n\r",int_to_string(charges), (charges == 1) ? "" : "s");
	send_to_char(buf,ch);
	return TRUE;
}

void power_urn(CHAR_DATA *ch, int charges)
{
	OBJ_DATA *urn = find_urn(ch);
	char buf[300];

	if(!urn)
		return send_to_char("The blood dissipates due to your lack of a vessel.\n\r",ch);

	if(urn->value[4] + charges > urn->value[3])
		return send_to_char("The blood overflows your sated vessel, wasting the new strength.\n\r",ch);

	urn->value[4]+= charges;
	
	sprintf(buf,"You drain a small amount of blood into %s!\n\r",urn->short_descr);
	send_to_char(buf,ch);
}

void spell_siphon(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int dam, blood;
	dam	= dice(level,5);

	if(saves_spell(level,victim,DAM_NEGATIVE))
		dam/=2;
	
	if(victim == ch && !IS_IMMORTAL(ch))
		return send_to_char("You can't siphon yourself!\n\r",ch);
	
	act("$n siphons the blood from $N's body in a sudden crimson burst!",ch,0,victim,TO_NOTVICT);
	act("$n siphons the blood right out of your body in a crimson burst!",ch,0,victim,TO_VICT);
	act("You siphon the blood out of $N's body in a crimson burst!",ch,0,victim,TO_CHAR);
	
	damage_new(ch,victim,dam,TYPE_UNDEFINED,DAM_NEGATIVE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"siphoning");

	if (IS_SET(victim->imm_flags,IMM_NEGATIVE)
		|| IS_SET(victim->imm_flags,IMM_MAGIC))
		return;

	blood	= URANGE(1,ch->level/15,4);
	
	if(!IS_NPC(victim))
		power_urn(ch,blood);
	else if (victim->level > ch->level + 8 || (ch->level > 49 && victim->level > ch->level + 3))
		if(number_percent() > 40)
			power_urn(ch,blood/2);
}

void spell_hex(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA hex;
	int roll = 0, severity = 0;
	int drain = 0;
	float drain_factor = 0;
	float chance = 0;

	if(is_affected(victim,sn))
		return send_to_char("They are already hexed.\n\r",ch);
	
	act("$n grins maniacally and spits out an archaic word, unleashing a hex upon $N!",ch,0,victim,TO_NOTVICT);
	act("$n grins maniacally and spits out an archaic word, unleashing a hex upon you!",ch,0,victim,TO_VICT);
	act("You grin maniacally and spit out an archaic word, unleashing a hex upon $N!",ch,0,victim,TO_CHAR);

	chance = get_skill(ch, sn);
	roll = number_percent();
	
	if(saves_spell(level+2,victim,DAM_NEGATIVE) && ((chance * 0.80) < roll))
	{
		act("$n staggers for a moment, but resists the hex.",victim,0,0,TO_ROOM);
		act("You stagger as the power of the hex strikes you, but manage to resist it.",victim,0,0,TO_CHAR);
		return;
	}
	
	init_affect(&hex);
	hex.where  	= TO_AFFECTS;
	hex.aftype 	= AFT_MALADY;
	hex.type   	= sn;
	hex.level	= level;
	hex.owner	= ch;
	hex.location	= APPLY_SAVING_SPELL;
	hex.modifier	= 30;
	hex.duration	= level/6;
	SET_BIT(hex.bitvector, AFF_CURSE);
	severity++;
	
	if(number_percent()>80)
	{
		SET_BIT(hex.bitvector, AFF_BLIND);
		severity++;
		hex.mod_name = MOD_VISION;
	}
	
	affect_to_char(victim, &hex);

	hex.mod_name = MOD_NONE;
	
	if(number_percent() >= 50 && !IS_AFFECTED(victim,AFF_PLAGUE)) {
		//plague
   		hex.location  = APPLY_STR;
    		hex.modifier  = -(ch->level / 7);
		SET_BIT(hex.bitvector, AFF_PLAGUE);
		hex.tick_fun  = plague_tick;
		affect_to_char(victim, &hex);
		severity++;
	}
	
	if(number_percent() >= 75 && !IS_AFFECTED(victim,AFF_POISON)) {
		//poison
		hex.location  = APPLY_STR;
		hex.modifier  = -(ch->level / 6);
		SET_BIT(hex.bitvector, AFF_POISON);
		hex.tick_fun = poison_tick;
		affect_to_char(victim, &hex);
		severity++;
	}
	
	if(severity>1 && severity<4) {
		act("$n sags as the hex strikes him powerfully!",victim,0,0,TO_ROOM);
		act("You slump as the hex strikes you powerfully.",victim,0,0,TO_CHAR);
	}
	
	if(severity==4) {
		act("$n screams in utter agony as the full power of the hex strangles $s soul!",victim,0,0,TO_ROOM);
		act("You scream in utter agony as the full power of the hex strangles your soul!",victim,0,0,TO_CHAR);
	}
	
	act("You feel drained from the power of your hex.",ch,0,0,TO_CHAR);
	drain_factor = 1 - (0.97 - (0.035 * severity));
	drain = (int) (drain_factor * ch->hit);
	ch->hit -= drain;
}

void spell_animate_dead( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *search;
    OBJ_DATA *corpse;
    char *obj_name;
    int chance, control = 0;
	
    if (level == 72)	/* necromancy scroll */
		
	target_name = "corpse";
	
    if (is_affected(ch,sn) && level < 70)
		return send_to_char("You have not yet regained your powers over the dead.\n\r",ch);
	
    for (search = char_list; search != NULL; search = search->next) {
		if (IS_NPC(search) && search->master == ch &&
			(search->pIndexData->vnum == MOB_VNUM_ZOMBIE ||
			(search->pIndexData->vnum >= 2940 &&
			 search->pIndexData->vnum <= 2947)))
			control++;
	}
	
    if(((control>=3 && ch->level<41) || (control>=4 && ch->level<52))
		&& !IS_IMMORTAL(ch))
 	   	return send_to_char("You already control as many undead as you can.\n\r",ch);
	
   	if (target_name[0] == '\0')
		return send_to_char("Animate which corpse?\n\r",ch);

    obj_name = palloc_string(target_name);
    corpse = get_obj_here(ch,obj_name);

	if (corpse == NULL || (corpse->item_type != ITEM_CORPSE_NPC &&
		corpse->item_type != ITEM_CORPSE_PC) || !corpse->ohp)
		return send_to_char("You can't animate that.\n\r",ch);

    if (corpse->item_type != ITEM_CORPSE_NPC && corpse->contains!=NULL)
		return send_to_char("It's too laden for you to gut.\n\r",ch);
	
	if (IS_SET(corpse->extra_flags,CORPSE_NO_ANIMATE))
		return send_to_char("That corpse can not sustain further life beyond the grave.\n\r",ch);
	
    chance = get_skill(ch,sn);
    chance-= (corpse->level - ch->level)*3;
    chance = UMIN(chance,80);
	corpse->value[4] = chance;
	
    SET_BIT(corpse->wear_flags, ITEM_NO_SAC);
    WAIT_STATE(ch, PULSE_VIOLENCE*3);

	ch->disrupted = FALSE;
	
    	act("You kneel before $p, chanting softly in an arcane language.",ch,corpse,0,TO_CHAR);
	act("$n kneels before $p, chanting softly in an unintelligible language.",ch,corpse,0,TO_ROOM);
	
	RS.Queue.AddToQueue(2, 2, animate_two, ch, corpse);
}

void animate_two(CHAR_DATA *ch, OBJ_DATA *corpse)
{
	if (ch->disrupted)	return;

	if (!corpse->in_room || corpse->in_room != ch->in_room) {
		ch->disrupted = TRUE;
		send_to_char("You don't see the corpse anymore.\n\r",ch);
		return;
	}

	if (ch->fighting) {
		act("The interruption disrupts your concentration, and $p crumbles to dust.",ch,corpse,NULL,TO_CHAR);
		act("$n is unable to continue the animation process, and $p crumbles to dust.",ch,corpse,NULL,TO_ROOM);
		extract_obj(corpse);
		ch->disrupted = TRUE;
		return;
	}

	act("You carve elaborate runes onto the torso of $p.",ch,corpse,NULL,TO_CHAR);
	act("$n carves elaborate runes onto the torso of $p.",ch,corpse,NULL,TO_ROOM);
	RS.Queue.AddToQueue(2, 2, animate_three, ch, corpse);
}

void animate_three(CHAR_DATA *ch, OBJ_DATA *corpse)
{
	if (ch->disrupted)	return;

	if (!corpse->in_room || corpse->in_room != ch->in_room) {
		ch->disrupted = TRUE;
		send_to_char("You don't see the corpse anymore.\n\r",ch);
		return;
	}

	if (ch->fighting) {
		act("The interruption disrupts your concentration, and $p crumbles to dust.",ch,corpse,NULL,TO_CHAR);
		act("$n is unable to continue the animation process, and $p crumbles to dust.",ch,corpse,NULL,TO_ROOM);
		extract_obj(corpse);
		ch->disrupted = TRUE;
		return;
	}

	act("The flesh of the corpse begins to decay rapidly, its skin left bloated and sagging.",ch,NULL,NULL,TO_ALL);
	RS.Queue.AddToQueue(2, 2, animate_four, ch, corpse);
}

void animate_four(CHAR_DATA *ch, OBJ_DATA *corpse)
{
    CHAR_DATA *zombie;
    char buf1[MAX_STRING_LENGTH], buf2[MAX_STRING_LENGTH];
    char *name, *last_name;
    AFFECT_DATA af;

	if (ch->disrupted)	return;
	
    if (!corpse->in_room || corpse->in_room != ch->in_room)
		return send_to_char("You don't see the corpse anymore.\n\r",ch);
	
    if (number_percent() > corpse->value[4]) {
		act("The magics fail to take hold, leaving the corpse a worthless pile of entrails.",ch,0,0,TO_CHAR);
		act("$n's efforts to animate $p result only in a disgusting pile of entrails.",ch,corpse,NULL,TO_ROOM);
		extract_obj(corpse);
		return;
    }

    init_affect(&af);
    af.where = TO_AFFECTS;
    af.aftype = AFT_TIMER;
    af.type = skill_lookup("animate dead");
    af.level = ch->level;
    af.duration = 24;
    af.modifier = 0;
    af.location = 0;
    affect_to_char(ch,&af);

    act("$p convulses slightly and rises to its feet to serve $n!",ch,corpse,0,TO_ROOM);
    act("$p convulses slightly and rises to its feet to serve you!",ch,corpse,0,TO_CHAR);
    ch->wait = 0;

    zombie = create_mobile(get_mob_index(MOB_VNUM_ZOMBIE));
    char_to_room(zombie,ch->in_room);
    zombie->level = corpse->level;
    zombie->max_hit = (short)(corpse->ohp * .55);
    zombie->size = corpse->value[2];	
    if(corpse->item_type == ITEM_CORPSE_PC)
		zombie->max_hit = corpse->ohp * 6;
    if(corpse->item_type == ITEM_CORPSE_PC && corpse->level > 25)
	soul_add(ch,1);
    else if (corpse->item_type == ITEM_CORPSE_PC && corpse->level <= 25)
	act("The essence of $p was too weak for your studies.",ch,corpse,0,TO_CHAR);
    else
	act("The raising was successful, but $p had no soul to steal.",ch,corpse,0,TO_CHAR);

   	zombie->hit = zombie->max_hit;
   	zombie->damage[DICE_NUMBER] = (corpse->level - ((int)(corpse->level / 2.5)));
   	zombie->damage[DICE_TYPE]   = 2;
    	zombie->damroll	= corpse->level - URANGE(-50,(short)(2.5 * (corpse->level - ch->level)),50);
    	name      = corpse->short_descr;
    	last_name = one_argument(corpse->short_descr, name);
        last_name = one_argument(last_name,name);
        last_name = one_argument(last_name,name);
    	name = last_name;
	
	float dmod = corpse->level * (corpse->item_type == ITEM_CORPSE_PC ? -.55 : -.3);
	zombie->defense_mod = (sh_int)dmod;
   	sprintf( buf1, "the zombie of %s", name);
   	sprintf( buf2, "The %s%s zombie of %s lurches here, its flesh hanging loosely.\n\r", 
		zombie->size == 0 ? "tiny, " : zombie->size == 1 ? "small, " : zombie->size == 2 ? "" : 
		zombie->size == 3 ? "large, " : zombie->size == 4 ? "huge, " : zombie->size == 5 ? "giant, " : "immense, ",
		zombie->level < 11 ? "pathetic" : zombie->level < 21 ? "weak" : zombie->level < 31 ? "average" :
		zombie->level < 41 ? "strong" : zombie->level < 52 ? "powerful" : "utterly deadly",
		name);

   	free_pstring(zombie->short_descr);
    free_pstring(zombie->long_descr);
		
    zombie->short_descr = palloc_string(buf1);
    zombie->long_descr = palloc_string(buf2);
	
    sprintf( buf1, "%s", name);
    add_follower(zombie,ch);
    zombie->leader = ch;
    SET_BIT(zombie->affected_by,AFF_CHARM);
	
	extract_obj(corpse);
	
    check_bond(ch,zombie);
}

void spell_vampiric_touch(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;
	
    if (victim == ch)
        return;

    dam = dice(level,4) + level/5;
	
    if (saves_spell(level,victim,DAM_NEGATIVE))
		dam/=2;

	damage(ch,victim,dam,gsn_vampiric_touch,DAM_NEGATIVE,TRUE);

	if (IS_SET(victim->imm_flags,IMM_NEGATIVE))
		dam = 0;
	
    ch->hit+=dam;
}

void spell_black_circle(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *pet;
	AFFECT_DATA af;
	bool found = FALSE;
	if(is_affected(ch,sn))
		return send_to_char("You cannot summon your minions again yet.\n\r",ch);
	
	act("$n draws a black circle on the ground and falls into deep concentration.",ch,0,0,TO_ROOM);
	act("You draw a black circle on the ground and fall into deep concentration.",ch,0,0,TO_CHAR);
	
	for(pet = char_list; pet!= NULL; pet=pet->next)
		if(IS_NPC(pet) && IS_AFFECTED(pet,AFF_CHARM) && pet->master &&
			pet->master == ch) {
			stop_fighting(pet,TRUE);
			act("$n disappears suddenly.",pet,0,0,TO_ROOM);
			char_from_room(pet);
			char_to_room(pet, ch->in_room);
			act("$N arrives suddenly, kneeling before you outside the circle.",ch,NULL,pet,TO_CHAR);
			act("$N arrives suddenly, kneeling before $n outside the circle.",ch,0,pet,TO_ROOM);
			found = TRUE;
		}
	
	if(!found)	
		return send_to_char("Your summonings went unanswered.\n\r",ch);

	init_affect(&af);
	af.where	= TO_AFFECTS;
	af.aftype	= AFT_TIMER;
	af.type		= sn;
	af.modifier	= 0;
	af.location	= 0;
	af.duration	= 60;
	af.owner	= NULL;
	af.level	= ch->level;
	af.end_fun	= NULL;
	new_affect_to_char(ch,&af);
}

void spell_visceral(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	OBJ_DATA *corpse, *ccorpses[5];
	int corpses = 0, i;

	if(is_affected(ch,sn))
		return send_to_char("You cannot partake again in the visceral divination yet.\n\r",ch);
	
	for(corpse = object_list; corpse != NULL && corpses < 3; corpse = corpse->next)
	{
		if(corpse->in_room && corpse->in_room == ch->in_room && corpse->level>=ch->level &&
		  (corpse->pIndexData->item_type == ITEM_CORPSE_PC || corpse->pIndexData->item_type == ITEM_CORPSE_NPC))
			ccorpses[corpses++] = corpse;
	}
	
	if(corpses<3)
		return send_to_char("There are not enough corpses of sufficient power available here.\n\r",ch);
	
	if(!drain_urn(ch,20))
		return;

	for(i=0;i<3;i++)
		extract_obj(ccorpses[i]);

	ch->disrupted = FALSE;
	
	act("$n slices open three corpses, spreading their entrails upon the ground.",ch,0,0,TO_ROOM);
	act("You slice open three corpses, spreading their entrails upon the ground.",ch,0,0,TO_CHAR);
	RS.Queue.AddToQueue(6, 1, visceral_two, ch);
}

void visceral_two(CHAR_DATA *ch)
{
	if(ch->fighting) {
		send_to_char("The profane ritual disrupted, the magicks dissipate harmlessly.\n\r",ch);
		ch->disrupted = TRUE;
		return;
	}

	act("Upon this gore, $n pours a generous amount of blood, saturating it.",ch,0,0,TO_ROOM);
	act("Upon this gore, you pour a generous amount of blood, saturating it.",ch,0,0,TO_CHAR);
	RS.Queue.AddToQueue(6, 1, visceral_three, ch);
}

void visceral_three(CHAR_DATA *ch)
{
	if (ch->disrupted)	return;

	if (ch->fighting) {
		send_to_char("The dark powers are furious at the disruption of the unholy orgy!\n\r",ch);
		act("A swirling black cloud coalesces above and lashes out at $n!",ch,NULL,NULL,TO_ROOM);
		damage_new(ch,ch,dice(18,18),gsn_unholy_bond,DAM_NEGATIVE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"a nether bolt*");
		ch->disrupted = TRUE;
		return;
	}
	
	act("$n and $s minions blissfully writhe in the carnage, praising the Dark Gods.",ch,0,0,TO_ROOM);
	act("You blissfully writhe in the carnage, praising the Dark Gods.",ch,0,0,TO_CHAR);
	RS.Queue.AddToQueue(6, 1, visceral_four, ch);
}

void visceral_four(CHAR_DATA *ch)
{
	CHAR_DATA *mob;
	AFFECT_DATA af;

	if (ch->disrupted)	return;

	if (ch->fighting) {
		send_to_char ("The unseen dark powers howl in fury at the disruption!\n\r",ch);
		send_to_char ("Invisible claws rend at your flesh as they seek to sate their bloodthirst!\n\r",ch);
		act("Swirling shadows surround $n, tearing at his flesh as $e shrieks in agony!",ch,NULL,NULL,TO_ROOM);
		damage_new(ch,ch,dice(30,40),gsn_unholy_bond,DAM_TRUESTRIKE,TRUE,HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "the unholy assault*");
		return;
	}
	
	act("Emerging from this loathsome orgy, $n and $s creatures surge with new strength.",ch,0,0,TO_ROOM);
	act("Emerging from this loathsome orgy, you feel new strength surge through you.",ch,0,0,TO_CHAR);

	init_affect(&af);
	af.where	= TO_AFFECTS;
	af.type		= skill_lookup("visceral divination");
	af.aftype	= AFT_SPELL;
	af.level	= ch->level;
	af.duration	= 60;
	af.owner	= ch;
	af.end_fun	= NULL;
	af.location	= APPLY_HIT;
	af.modifier	= ch->level * 8;
	new_affect_to_char(ch,&af);

	af.location	= APPLY_NONE;
	af.modifier	= 0;
	new_affect_to_char(ch,&af);

	for(mob=ch->in_room->people;mob!=NULL;mob=mob->next_in_room)
		if(IS_NPC(mob) && IS_AFFECTED(mob,AFF_CHARM) && mob->master &&
			mob->master==ch) {
			af.location	= APPLY_DAMROLL;
			af.modifier	= (ch->level / 2) - 5;
			affect_to_char(mob, &af);
			act("$n's eyes glow with unholy fervor.",mob,0,0,TO_ROOM);
		}
}

void spell_ritual_soul(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *search, *victim = (CHAR_DATA *) vo;
	
	for (search = char_list; search != NULL; search = search->next) {
		if (IS_NPC(search) && search->master == ch &&
			search->pIndexData->vnum > 2939 && search->pIndexData->vnum<2945)
			return send_to_char("You cannot control another servitor of that type.\n\r",ch);
	}

	if(!IS_NPC(victim) || victim->pIndexData->vnum!=MOB_VNUM_ZOMBIE || victim->master!=ch)
		return send_to_char("You must cast the ritual upon a zombie you control.\n\r",ch);
	
	if(!drain_urn(ch,10))
		return;
		
	ch->disrupted = FALSE;
	
	act("$n begins to spread out some infernal relics and charms.",ch,0,0,TO_ROOM);
	act("You begin to spread out some infernal relics and charms.",ch,0,0,TO_CHAR);
	RS.Queue.AddToQueue(3, 2, ritual_two, ch, victim);
}

void ritual_two(CHAR_DATA *ch, CHAR_DATA *victim)
{
	if (ch->disrupted)	return;
	
    if (ch->fighting) {
		send_to_char("The unholy ritual disrupted, the harnessed energy dissipates harmlessly.\n\r",ch);
		ch->disrupted = TRUE;
		return;
	}

	act("$n pours some blood onto the floor from $s urn, and chants quietly.",ch,0,0,TO_ROOM);
	act("You spill out blood from your urn, chanting softly in an arcane tongue.",ch,0,0,TO_CHAR);
	RS.Queue.AddToQueue(3, 2, ritual_three, ch, victim);	
}

void ritual_three(CHAR_DATA *ch, CHAR_DATA *victim)
{
	if (ch->disrupted)	return;

    if (ch->fighting) {
		send_to_char("The unholy ritual disrupted, the harnessed energy dissipates harmlessly.\n\r",ch);
		ch->disrupted = TRUE;
		return;
	}
	
	act("$n calls upon the dark gods, and forfeits part of $s spirit in unholy sacrifice!",ch,0,victim,TO_ROOM);
	act("You forfeit part of your spirit in an unholy sacrifice before the Dark Gods!",ch,0,0,TO_CHAR);
	ch->mana = (short)(ch->mana * .8);
	ch->move = (short)(ch->move * .5);
	RS.Queue.AddToQueue(3, 2, ritual_four, ch, victim);
}

void ritual_four(CHAR_DATA* ch, CHAR_DATA *victim)
{

	CHAR_DATA *mob;
	AFFECT_DATA af;
	
	if (ch->disrupted)	return;
	
	if (ch->fighting) {
		send_to_char("The Dark Gods are angered at the disruption, and unleash their fury upon you!\n\r",ch);
		act("A swirling black cloud coalesces above and lashes out at $n!",ch,NULL,NULL,TO_ROOM);
		damage_new(ch,ch,dice(18,18),gsn_unholy_bond,DAM_NEGATIVE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"a nether bolt*");
		return;
	}
	
	act("$n points at $N and the rotting body crumbles to ashes!",ch,0,victim,TO_ROOM);
	act("You point at $N and the rotting body crumbles to ashes!",ch,0,victim,TO_CHAR);
	mob = create_mobile(get_mob_index(number_range(2940,2942)));
	char_to_room(mob,ch->in_room);
	mob->hit	= victim->hit;
	mob->max_hit	= victim->max_hit;
	mob->level	= victim->level;
        if(is_affected(victim,gsn_unholy_bond)) {
	init_affect(&af);
        af.where = TO_AFFECTS;
        af.type = gsn_unholy_bond;
        af.aftype = AFT_SKILL;
        af.duration = -1;
	affect_to_char(mob,&af);
	}
        extract_char(victim,TRUE);
        act("From the ashes rises $n!", mob, 0, ch, TO_ALL);
	add_follower(mob,ch);
        mob->leader = ch;
        SET_BIT(mob->affected_by, AFF_CHARM);
	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = gsn_unholy_bond;
	af.aftype = AFT_SKILL;
	af.duration = -1;

}

void spell_ritual_flesh(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *search, *victim = (CHAR_DATA *) vo;
	
	for (search = char_list; search != NULL; search = search->next)
		if (IS_NPC(search) && search->master == ch &&
			search->pIndexData->vnum > 2944 && search->pIndexData->vnum<2948)
			return send_to_char("You cannot control another servitor of that type.\n\r",ch);
	

	if(!IS_NPC(victim) || victim->pIndexData->vnum!=MOB_VNUM_ZOMBIE
		|| victim->master!=ch)
		return send_to_char("You must cast the ritual upon a zombie you control.\n\r",ch);

	if(!drain_urn(ch,10))
		return;

	ch->disrupted = FALSE;
	
	RS.Queue.AddToQueue(3, 2, flesh_two, ch, victim);
	act("You prepare to make an unholy sacrifice to the Dark Gods!",ch,0,0,TO_CHAR);

}

void flesh_two(CHAR_DATA *ch, CHAR_DATA *victim)
{
	if (ch->fighting) {
		send_to_char("The unholy ritual disrupted, the harnessed energy dissipates harmlessly.\n\r",ch);
		ch->disrupted = TRUE;
		return;
	}

	act("$n pours blood onto the floor, chanting softly in an undecipherable tongue.",ch,0,0,TO_ROOM);
	act("You pour blood onto the floor, chanting softly in an arcane tongue.",ch,0,0,TO_CHAR);
	RS.Queue.AddToQueue(3, 2, flesh_three, ch, victim);
}

void flesh_three(CHAR_DATA *ch, CHAR_DATA *victim)
{
	if (ch->disrupted)	return;
	
	if (ch->fighting) {
		send_to_char("The unholy ritual disrupted, the harnessed energy dissipates harmlessly.\n\r",ch);
		ch->disrupted = TRUE;
	}
	
	act("$n calls upon the Dark Gods and forfeits part of $s vitality in sacrifice!",ch,0,0,TO_ROOM);
	act("You call upon the Dark Gods and offer part of your vitality as sacrifice!",ch,0,0,TO_CHAR);
	ch->hit = (short)(ch->hit * 0.8);
	RS.Queue.AddToQueue(3, 2, flesh_four, ch, victim);
}

void flesh_four(CHAR_DATA *ch, CHAR_DATA *victim)
{
	CHAR_DATA *mob;
	AFFECT_DATA af;
	OBJ_DATA *obj;
	int mob_vnum = number_range(2945,2947);
	
	if (ch->disrupted)	return;
	
	if (ch->fighting) {
		send_to_char("The Dark Gods are angered at the disruption, and unleash their fury upon you!\n\r",ch);
		act("A swirling black cloud coalesces above and lashes out at $n!",ch,NULL,NULL,TO_ROOM);
		damage_new(ch,ch,dice(18,18),gsn_unholy_bond,DAM_NEGATIVE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"a nether bolt*");
		return;
	}

	mob = create_mobile(get_mob_index(mob_vnum));
	char_to_room(mob,ch->in_room);
	mob->max_hit    = victim->max_hit;
	mob->hit        = mob->max_hit;
	mob->level      = victim->level;
	mob->damage[DICE_NUMBER]	= victim->level + 10;
	mob->damage[DICE_TYPE]	= 2;
	mob->damroll = victim->level - URANGE(-50,(short)(2.5 * (victim->level - ch->level)),50);

	act("$n points at $N and its rotting body crumbles into grey ashes.",ch,0,victim,TO_ROOM);
	act("You point at $N and its rotting body crumbles into ashes.",ch,0,victim,TO_CHAR);
	
	if(is_affected(victim,gsn_unholy_bond)) {
        init_affect(&af);
        af.where = TO_AFFECTS;
        af.type = gsn_unholy_bond;
        af.aftype = AFT_SKILL;
        af.duration = -1;
        affect_to_char(mob,&af);
	}
	
	extract_char(victim,TRUE);
  	act("Suddenly, the pile of ashes is scattered as $N arises to kneel before you!",ch,0,mob,TO_CHAR);
	act("Suddenly, the pile of ashes is scattered as $N arises to kneel before $n!",ch,0,mob,TO_ROOM);
  	add_follower(mob,ch);
   	mob->leader = ch;
   	SET_BIT(mob->affected_by, AFF_CHARM);
		
	if (mob_vnum == 2947) {
		obj = create_object(get_obj_index(number_range(2932,2935)),0);
		obj_to_char(obj,mob);
		equip_char(mob,obj,WEAR_WIELD,FALSE);
	}
}

void spell_decrepify(int sn,int level,CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	int mv;

	mv = (short)(ch->level * 3.5);

	if(saves_spell(ch->level,victim,DAM_MENTAL))
		mv /= 2;

	act("$N suddenly looks very worn out.",0,0,victim,TO_NOTVICT);
	act("You suddenly feel sluggish.",ch,0,victim,TO_VICT);
	act("$N suddenly looks very worn out.",ch,0,victim,TO_CHAR);

	victim->move -= mv;

	if(is_affected(ch,sn))
		return;

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_MALADY;
	af.type = sn;
	af.location = 0;
	af.modifier = 0;
	af.duration = ch->level/5;
	af.level = ch->level;
	af.mod_name = MOD_MOVEMENT;
	affect_to_char(victim,&af);

	return;
}
		
void spell_corrupt_flesh(int sn, int level,CHAR_DATA *ch, void *vo, int target) {
	char arg1[MSL], arg2[MSL];
	OBJ_DATA *obj;
	OBJ_DATA *obj2;
	CHAR_DATA *victim, *mob;
	char buf[MSL];
	int chance;
	
	chance = get_skill(ch,sn);
	
	if(!str_cmp(target_name,"")) {
		ch->wait = 0;
		return send_to_char("Syntax: cast 'corrupt flesh' <body part> <target>\n\r",ch);
	}
		
	target_name = one_argument(target_name,arg1);
	target_name = one_argument(target_name,arg2);

	victim = ch;
	if(arg1[0]=='\0' || (obj=get_obj_carry(ch,arg1,ch)) == NULL)
		return send_to_char("You aren't carrying that.\n\r",ch);
	
	if(str_cmp(arg1,"head") && (arg2[0]=='\0' || (victim=get_char_room(ch,arg2)) == NULL)) {
		ch->wait = 0;
		return send_to_char("They aren't here.\n\r",ch);
	}
	
	if((obj->pIndexData->vnum != OBJ_VNUM_SEVERED_HEAD)
		&& (obj->pIndexData->vnum != OBJ_VNUM_SLICED_ARM)
		&& (obj->pIndexData->vnum != OBJ_VNUM_SLICED_LEG))
		return send_to_char("You must use a severed body part.\n\r",ch);

	if(is_safe(ch,victim))
		return;
	
	if(number_percent() > chance)
	{
		send_to_char("Your dark magic fails to take hold in the flesh!\n\r",ch);
		send_to_char("The rotting flesh disintegrates!\n\r",ch);
		extract_obj(obj);
		return;
	}

	if(str_cmp(arg1,"head"))
	{
		act("You give ephemeral life to $p and send it to attack $N!",ch,obj,victim,TO_CHAR);
		act("$p rises into the air and attacks $N!",ch,obj,victim,TO_NOTVICT);
		act("$p rises into the air and attacks you!",ch,obj,victim,TO_VICT);
		sprintf(buf,"Help!  I am being attacked by %s!",obj->short_descr);
		do_myell(victim,buf,ch);
		obj_from_char(obj);
		if(ch->fighting == NULL)
			one_hit(victim,ch,TYPE_HIT);
	}
	
	if(!str_cmp(arg1,"arm"))
	{
		if(!(obj2 = get_eq_char(victim,WEAR_WIELD)))
		{
			act("$N is not wielding a weapon.",ch,0,victim,TO_CHAR);
			return;
		}
		
		if ((IS_SET(obj2->extra_flags,ITEM_NOREMOVE)) ||
			(IS_SET(obj2->extra_flags,ITEM_NODISARM)))
		{
			act("$p catches hold of $N's weapon, but it won't budge!",0,obj,victim,TO_NOTVICT);
			act("$p catches hold of $N's weapon, but it won't budge!",ch,obj,victim,TO_CHAR);
			act("$p catches hold of your weapon, but it won't budge!",ch,obj,victim,TO_VICT);
			extract_obj(obj);
			return;
		}
		
		if((number_range(0,1) * 100) < number_percent())
		{
			act("$p catches hold of $N's weapon, but fails to wrench it from $M.",0,obj,victim,TO_NOTVICT);
			act("$p catches hold of $N's weapon, but fails to wrench it from $M.",ch,obj,victim,TO_CHAR);
			act("$p catches hold of your weapon, but fails to wrench it from you.",0,obj,victim,TO_VICT);
		}
		else
		{
			act("$p catches hold of $N's weapon and wrenches it from $S grasp!",0,obj,victim,TO_NOTVICT);
			act("$p catches hold of $N's weapon and wrenches it from $S grasp!",ch,obj,victim,TO_CHAR);
			act("$p catches hold of your weapon and wrenches it from your grasp!",ch,obj,victim,TO_VICT);
			obj_from_char(obj2);
			if(IS_SET(obj2->extra_flags,ITEM_NODROP))
			{
				extract_obj(obj);
				obj_to_char(obj2,victim);
				return;
			}
			else if(number_percent() > 50)
			{
				act("$p gives $N's weapon to $n before the black magic consumes it.",ch,obj,victim,TO_NOTVICT);
				act("$p gives $N's weapon to you before the black magic consumes it.",ch,obj,victim,TO_CHAR);
				act("$p gives your weapon to $n before the black magic consumes it.",ch,obj,victim,TO_VICT);
				extract_obj(obj);
				obj_to_char(obj2,ch);
				return;
			}
			else
			{
				act("The black magic animating it consumes $p.",ch,obj,victim,TO_NOTVICT);
				act("$N's weapon falls to the ground amongst a pile of ashes.",ch,obj,victim,TO_NOTVICT);
				act("The black magic animating it consumes $p.",ch,obj,victim,TO_CHAR);
				act("$N's weapon falls to the ground amongst a pile of ashes.",ch,obj,victim,TO_CHAR);
				act("The black magic animating it consumes $p.",ch,obj,victim,TO_VICT);
				act("Your weapon falls to the ground amongst a pile of ashes.",ch,obj,victim,TO_VICT);
				extract_obj(obj);
				obj_to_room(obj2,victim->in_room);
				return;
			}
		}
	}
	else if(!str_cmp(arg1,"leg"))
	{
		if(IS_AFFECTED(victim,AFF_FLYING))
		{
			act("$p flies at $N's legs, but $E flies over it.",ch,obj,victim,TO_NOTVICT);
			act("$p flies at $N's legs, but $E flies over it.",ch,obj,victim,TO_CHAR);
			act("$p flies at your legs, but you fly over it.",ch,obj,victim,TO_VICT);
			extract_obj(obj);
			return;
		}
		
		if(number_percent() > 90)
		{
			act("$p flies at $N's legs, but fails to trip $M.",ch,obj,victim,TO_NOTVICT);
			act("$p flies at $N's legs, but fails to trip $M.",ch,obj,victim,TO_CHAR);
			act("$p flies at your legs, but fails to trip you.",ch,obj,victim,TO_VICT);
			extract_obj(obj);
			return;
		}
		else
		{
			act("$p flies into $N's legs, bringing $M to the ground.",ch,obj,victim,TO_NOTVICT);
			act("$p flies into $N's legs, bringing $M to the ground.",ch,obj,victim,TO_CHAR);
			act("$p flies into your legs, bringing you to the ground.",ch,obj,victim,TO_VICT);
			extract_obj(obj);
			LAG_CHAR(victim,PULSE_VIOLENCE*2);
			return;
		}
	}
	else if(!str_cmp(arg1,"head"))
	{
		mob = create_mobile(get_mob_index(2962));
		char_to_room(mob,ch->in_room);
		act("The flesh of $p withers away rapidly as you sap its remaining life force.",ch,obj,0,TO_CHAR);
		act("The flesh of $p withers away rapidly, leaving $n holding a putrid skull.",ch,obj,0,TO_ROOM);
		obj_from_char(obj);
		extract_obj(obj);
		mob->level = 1;
		mob->leader = ch;
	}
}		



void spell_corpse_trap(int sn, int level, CHAR_DATA *ch, void *vo, int target) {
	OBJ_DATA *corpse;
	OBJ_DATA *trap;
	AFFECT_DATA af;
	int chance;
	int vnum = 2926;
	
	if(is_affected(ch,sn))
		return send_to_char("You are not ready to make another trap yet.\n\r",ch);
	
	corpse = get_obj_here(ch,target_name);
	
	if(!corpse || !str_cmp(target_name,""))
		return send_to_char("You don't see that here.\n\r",ch);
	
	if(corpse->item_type != ITEM_CORPSE_NPC && corpse->item_type != ITEM_CORPSE_PC)
		return send_to_char("That's not a suitable corpse.\n\r",ch);
	
	if((corpse->level + 6) < ch->level)
		return send_to_char("That corpse is not powerful enough.\n\r",ch);
	
	if (corpse->item_type != ITEM_CORPSE_NPC && corpse->contains!=NULL)
        	return send_to_char("You can't trap a corpse with things in it!\n\r",ch);
	
	chance = get_skill(ch,sn)/2;
	chance += ch->level - corpse->level;
	chance += get_curr_stat(ch,STAT_INT);
	
	if(number_percent() > chance) {
		send_to_char("Your magic accelerates the decaying process too quickly, destroying the corpse.\n\r",ch);
		act("$p decays rapidly, leaving nothing but dust.",ch,corpse,0,TO_ROOM);
		extract_obj(corpse);
		return;
	}

	trap = create_object(get_obj_index(vnum),corpse->level);
	trap->level = ch->level;
	trap->timer = number_range(5,8);
	
	if(corpse->item_type == ITEM_CORPSE_NPC)
		trap->item_type = ITEM_CORPSE_NPC;
	
	SET_BIT(trap->extra_flags,CORPSE_NO_ANIMATE);
	act("You carefully accelerate the decay rate of $p, preparing it to explode.",ch,corpse,0,TO_CHAR);
	act("$p's skin begins to wrinkle and fold.",ch,corpse,0,TO_ROOM);
	extract_obj(corpse);
	obj_to_room(trap,ch->in_room);
	trap->owner = palloc_string(ch->true_name);

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_TIMER;
	af.type = sn;
	af.location = 0;
	af.modifier = 0;
	af.duration = 24;
	af.level = ch->level;
	affect_to_char(ch,&af);	
}

void spell_lesser_golem(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *mob;
	int vnum = 0;
	char type[MSL];
	int num;
	CHAR_DATA *check;
	AFFECT_DATA af;
	
	target_name = one_argument(target_name,type);
	num = atoi(target_name);
	
	if(!str_cmp(type,""))
		return send_to_char("Syntax:  cast 'lesser golem' <bone/blood/flesh> <ounces of blood>\n\r",ch);
	
	if(str_cmp(type,"blood") && str_cmp(type,"bone") && str_cmp(type,"flesh"))
		return send_to_char("That is not an acceptable golem type.\n\r"\
							"Syntax:  cast 'lesser golem' <bone/blood/flesh> <ounces of blood>\n\r",ch);
	
	if(num == 0)
		return send_to_char("You must use at least one ounce of blood.\n\r"\
							"Syntax:  cast 'lesser golem' <bone/blood/flesh> <ounces of blood>\n\r",ch);
	
	if(num > 20)
		return send_to_char("You cannot use more than twenty ounces of blood at once.\n\r",ch);

	if(is_affected(ch, gsn_lesser_golem))
        return send_to_char("You cannot creater another lesser golem so soon.\n\r", ch);
	
	for (check = char_list; check != NULL; check = check->next)
		if (IS_NPC(check) && check->master == ch &&
			(check->pIndexData->vnum == 2955 || check->pIndexData->vnum == 2956
			|| check->pIndexData->vnum == 2957))
			return send_to_char("You already have a golem under your command.\n\r",ch);
	
	if(!drain_urn(ch,num))
		return;
	
	if(!str_cmp(type,"blood"))
		vnum = 2957;
	
	if(!str_cmp(type,"bone"))
		vnum = 2956;
	
	if(!str_cmp(type,"flesh"))
		vnum = 2955;
	
	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_TIMER;
	af.location = 0;
	af.modifier = 0;
	af.duration = dice(5,8);
	af.type = sn;
	af.level = ch->level;
	affect_to_char(ch,&af);

	mob = create_mobile(get_mob_index(vnum));
	mob->max_hit = (short)(ch->max_hit*1.5);
	mob->hit = mob->max_hit;
	mob->damroll = 0;
	mob->level = ch->level - 10 + (num/2);
	mob->damage[DICE_TYPE] = 2;
	mob->damage[DICE_NUMBER] = mob->level;
	mob->damage[DICE_BONUS] = mob->level / 3;
	add_follower(mob,ch);
	mob->leader = ch;
	SET_BIT(mob->affected_by, AFF_CHARM);
	char_to_room(mob,ch->in_room);
	act("$n pours some blood on the floor.  It begins shifting and changing, finally forming $N!",ch,0,mob,TO_ROOM);
	act("You pour some blood on the floor.  It begins shifting and changing, finally forming $N!",ch,0,mob,TO_CHAR);
}

void spell_greater_golem(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *mob, *check;
	AFFECT_DATA af;
	char type[MSL];
	char arg[MSL];
	OBJ_DATA *obj;
	int vnum = 0;
	
	target_name = one_argument(target_name,type);
	target_name = one_argument(target_name,arg);
	
	if(!str_cmp(type,""))
		return send_to_char("Syntax:  cast 'greater golem' <glass / stone / shadow>\n\r",ch);

	if(is_affected(ch, gsn_greater_golem))
		return send_to_char("You cannot creater another greater golem so soon.\n\r", ch);
	
	for (check = char_list; check != NULL; check = check->next)
        if (IS_NPC(check) && check->master == ch &&
			(check->pIndexData->vnum == 2959 || check->pIndexData->vnum == 2960
			|| check->pIndexData->vnum == 2961))
			return send_to_char("You already have a golem under your command.\n\r",ch);
	
	if(str_cmp(type,"glass") && str_cmp(type,"stone") && str_cmp(type,"shadow"))
		return send_to_char("Syntax:  cast 'greater golem' <glass / stone / shadow>\n\r",ch);
	
	if(!str_cmp(type,"glass") && !str_cmp(arg,""))
		return send_to_char("You must specify an item to use to create your golem.\n\r",ch);
	
	if(!str_cmp(type,"glass")) {
		if((obj = get_obj_carry(ch,arg,ch)) == NULL)
			return send_to_char("You aren't carrying that.\n\r",ch);
		if(str_cmp(obj->pIndexData->material,"glass") || obj->weight < 20)
			if(!IS_IMMORTAL(ch))
				return send_to_char("To make a glass golem, you must use a large item made of glass.\n\r",ch);
		vnum = 2961;
		act("$n fashions a golem from a lifeless hunk of glass!",ch,0,0,TO_ROOM);
		act("You fashion a golem from $p.",ch,obj,0,TO_CHAR);
		extract_obj(obj);
	}
	
	if(!str_cmp(type,"stone")) {
		if(ch->in_room->sector_type != SECT_MOUNTAIN)
			return send_to_char("You must be in a mountainous region to make a stone golem.\n\r",ch);
		vnum = 2959;
		act("$n fashions a golem from a mass of stone!",ch,0,0,TO_ROOM);
		act("You fashion a golem from a mass of stone.",ch,0,0,TO_CHAR);
	}
	
	if(!str_cmp(type,"shadow")) {
		if(!IS_SET(ch->in_room->room_flags, ROOM_DARK))
			return send_to_char("This room contains too much light to create a shadow golem.\n\r",ch);
		vnum = 2960;
		act("$n gives life to the shadows around him!",ch,0,0,TO_ROOM);
		act("You give life to the shadows around you.",ch,0,0,TO_CHAR);
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_TIMER;
	af.location = 0;
	af.modifier = 0;
	af.duration = dice(5,8);
	af.type = sn;
	af.level = ch->level;
	affect_to_char(ch,&af);

	mob = create_mobile(get_mob_index(vnum));
	mob->max_hit = ch->max_hit*2;
	mob->hit = mob->max_hit;
	mob->damroll = 0;
	mob->level = ch->level;
	mob->damage[DICE_TYPE] = 4;
	mob->damage[DICE_NUMBER] = ch->level;
	mob->damage[DICE_BONUS] = ch->level / 2;
	add_follower(mob,ch);
	mob->leader = ch;
	SET_BIT(mob->affected_by, AFF_CHARM);
	char_to_room(mob,ch->in_room);	
}

void do_drain(CHAR_DATA *ch, char *argument)
{
	OBJ_DATA *corpse = get_obj_here(ch,argument);
	int chance = get_skill(ch,skill_lookup("drain")), blood;
	if(!str_cmp(argument,""))
		return send_to_char("You need to specify a corpse to drain.\n\r",ch);
	
	if(corpse == NULL)
		return send_to_char("You don't see that here.\n\r",ch);
	
	if((corpse->item_type != ITEM_CORPSE_NPC) && (corpse->item_type != ITEM_CORPSE_PC))
		return send_to_char("That's not a suitable corpse.\n\r",ch);
	
	if(corpse->item_type == ITEM_CORPSE_PC && ((corpse->level+ 6) < (ch->level)))
		return send_to_char("That corpse is not powerful enough.\n\r",ch);
	
	if(corpse->item_type == ITEM_CORPSE_NPC && ((corpse->level + 6) < ch->level))
		return send_to_char("That corpse is not powerful enough.\n\r",ch);
	
	if(IS_SET(corpse->extra_flags,CORPSE_NO_ANIMATE))
		return send_to_char("That's not a suitable corpse.\n\r",ch);

	if(chance > number_percent()) {
		act("You successfully drain $p of its blood!",ch,corpse,0,TO_CHAR);
		act("$n crouches over $p, chanting softly as $e desiccates it.",ch,corpse,0,TO_ROOM);
		blood   = URANGE(1,ch->level/15, corpse->item_type == ITEM_CORPSE_NPC ? 3 : 4);
		power_urn(ch,blood);
		SET_BIT(corpse->extra_flags,CORPSE_NO_ANIMATE);
		WAIT_STATE(ch,PULSE_VIOLENCE*2);
		check_improve(ch,skill_lookup("drain"),TRUE,1);
	} else {
		act("You fail to drain the blood of $p and render it useless!",ch,corpse,0,TO_CHAR);
		act("$n attempts to drain the blood from $p, but ruins it, blood spilling everywhere.",ch,corpse,0,TO_ROOM);
		SET_BIT(corpse->extra_flags,CORPSE_NO_ANIMATE);
		WAIT_STATE(ch,PULSE_VIOLENCE*2);
		check_improve(ch,skill_lookup("drain"),FALSE,1);
	}
	return;
}

bool check_bond(CHAR_DATA *ch, CHAR_DATA *mob)
{
	AFFECT_DATA af;
	int chance = get_skill(ch,skill_lookup("unholy bond"));
	if(number_percent() < chance - 20) {
		init_affect(&af);
		af.where = TO_AFFECTS;
		af.duration = -1;
		af.owner = ch;
		af.type = gsn_unholy_bond;
		af.aftype = AFT_SKILL;
		affect_to_char(mob,&af);
		act("The zombie's eyes suddenly flare a deep crimson, which rapidly fades away.",ch,0,mob,TO_ROOM);
		act("The zombie's eyes suddenly flare a deep crimson, which rapidly fades away.",ch,0,mob,TO_CHAR);
		act("You form a successful bond with $N!",ch,0,mob,TO_CHAR);
		check_improve(ch,gsn_unholy_bond,TRUE,1);
		return TRUE;
	}
check_improve(ch,gsn_unholy_bond,FALSE,1);
return FALSE;
}

bool check_zombie_summon(CHAR_DATA *ch)
{
	CHAR_DATA *mob;

	if(get_skill(ch,gsn_unholy_bond) < 1)
		return FALSE;	
	
	for(mob = char_list; mob!= NULL; mob=mob->next)
		if(IS_NPC(mob) && IS_AFFECTED(mob,AFF_CHARM) && mob->master &&
			mob->master == ch && is_affected(mob,gsn_unholy_bond)) {
	
		if (number_percent() > 75)
			return FALSE;

		stop_fighting(mob,TRUE);
		act("$n disappears suddenly.",mob,0,0,TO_ROOM);
		char_from_room(mob);
		char_to_room(mob, ch->in_room);
		act("$n arrives suddenly.",mob,0,0,TO_ROOM);
		return TRUE;
		}

	return FALSE;	
}

void soul_add(CHAR_DATA *ch, int souls)
{
	if(IS_NPC(ch))
		return;
	
	if(ch->pcdata->souls > 250)
		return;

	ch->pcdata->souls += souls;
	if(ch->Class()->GetIndex() != CLASS_NECROMANCER)
		return;
	
	send_to_char("A flood of ecstasy washes over you as you steal a portion of the corpse's soul!\n\r",ch);
	if(ch->pcdata->souls == 20)
		send_to_char("You feel the beginnings of a dark insight start to fill your mind.\n\r",ch);
	
	if(ch->pcdata->souls >= 30 && ch->pcdata->learned[skill_lookup("ritual of flesh")] < 1)
	{
		ch->pcdata->learned[skill_lookup("ritual of flesh")] = 1;
		return send_to_char("As you draw on the essence of the corpse, a dark insight fills your mind.\n\r",ch);
	}
	
	if(ch->pcdata->souls >= 55 && ch->pcdata->learned[skill_lookup("ritual of soul")] < 1)
	{	
		ch->pcdata->learned[skill_lookup("ritual of soul")] = 1;
        return send_to_char("As you draw on the essence of the corpse, a dark insight fills your mind.\n\r",ch);
    }
	
    if(ch->pcdata->souls >= 80 && ch->pcdata->learned[skill_lookup("visceral divination")] < 1)
	{
        ch->pcdata->learned[skill_lookup("visceral divination")] = 1;
        return send_to_char("As you draw on the essence of the corpse, the final insights of the Dark Gods fill your mind.\n\r",ch);
    }
return;
}

