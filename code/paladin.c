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

char * flag_name_lookup args((long bitv, const struct flag_type *flag_table));

void spell_rites_of_preparation(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *to;
	AFFECT_DATA af;
	bool hasgroup = FALSE;

	if(is_affected(ch,sn))
		return send_to_char("You cannot focus on preparing your party for battle again so soon.\n\r",ch);

	for(to = ch->in_room->people; to!=NULL; to = to->next_in_room)
	{
		if(is_same_group(to,ch) && to != ch)
			hasgroup = TRUE;
		else
			continue;

		if(is_same_group(to,ch) && !IS_NPC(to) && !is_affected(to,sn))
		{
			to->pcdata->condition[COND_HUNGER] = 0;
			to->pcdata->condition[COND_THIRST] = 0;
			
			init_affect(&af);
			af.where = TO_AFFECTS;
			af.type = sn;
			af.modifier = (ch->level/7);
			af.location = APPLY_HITROLL;
			af.duration = 24;
			af.level = ch->level;
			affect_to_char(to,&af);
			af.type = sn;
			af.modifier = (ch->level/7);
			af.location = APPLY_DAMROLL;
			af.duration = 24;
			af.level = ch->level;
			affect_to_char(to,&af);
			af.type = sn;
			af.modifier = -(ch->level/8);
			af.location = APPLY_SAVES;
			af.duration = 24;
			af.aftype = AFT_COMMUNE;
			af.level = ch->level;
			affect_to_char(to,&af);
		}
	}
	
	if(hasgroup == FALSE)
		return send_to_char("You need a party to perform these rites.\n\r",ch);
	else
	{

		act("You recite some prayers to those in your party, bolstering their confidence and preparing them for battle.",
				ch,0,0,TO_CHAR);
		act("$n recites some prayers in a loud and confident voice, preparing you for righteous battle.",ch,0,0,TO_GROUP);

		init_affect(&af);	
		af.where = TO_AFFECTS;
		af.type = sn;
		af.aftype = AFT_TIMER;
		af.level = ch->level;
		af.location = APPLY_NONE;
		af.modifier = 0;
		af.duration = 48;
		affect_to_char(ch,&af);
	}

	return;
}

void spell_spiritual_hammer(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	float dam = dice(level, 4);

	if(victim == ch)
		return send_to_char("You can't do that.\n\r",ch);
	if(IS_EVIL(victim))
	{
		act("A brilliant hammer descends from the heavens, striking you with great force!",ch,0,victim,TO_VICT);
		act("A brilliant hammer descends from the heavens, striking $N with great force!",ch,0,victim,TO_NOTVICT);
		dam *= 1.5;
	}
	else if(IS_GOOD(victim))
	{
		act("A flickering hammer fades into existence above you and hesitantly descends.",ch,0,victim,TO_VICT);
		act("A flickering hammer fades into existence above $N and slowly descends upon $M.",ch,0,victim,TO_NOTVICT);
		dam *= .3;
	}
	else
	{
		act("A great hammer fades into existence above you and strikes down!",ch,0,victim,TO_VICT);
		act("A great hammer fades into existence above $N and strikes down!",ch,0,victim,TO_NOTVICT);
	}
	if(saves_spell(level,victim,DAM_HOLY))
		dam/=2;

	damage_new(ch,victim,(int)dam,sn,DAM_HOLY,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"spiritual hammer");

	return;
}
	

void do_turn_undead(CHAR_DATA *ch, char *argument)
{
	int dam;
	int difference = 0;
	CHAR_DATA *victim, *v_next;
	OBJ_DATA *corpse;
	AFFECT_DATA af;
	bool forceflee = FALSE;

	if(get_skill(ch,gsn_turn_undead) == 0)
		return send_to_char("Huh?\n\r",ch);
	
	if(is_affected(ch,gsn_turn_undead))
		return send_to_char("You have not yet regained enough power to turn undead again.\n\r",ch);	

	if(ch->mana < 75)
		return send_to_char("You don't have enough mana.\n\r",ch);
	
	act("You raise your hands aloft and extend an aura of purity about you.",ch,0,0,TO_CHAR);
	act("$n raises $s hands aloft, and a faint aura billows out from $s form.",ch,0,0,TO_ROOM);	

	for(victim = ch->in_room->people; victim != NULL; victim = v_next)
	{
		v_next = victim->next_in_room;
	
		if(!IS_NPC(victim) || victim == ch || !IS_SET(victim->act,ACT_UNDEAD))
			continue;

		difference = ch->level - victim->level;
		if(IS_SET(victim->act,ACT_UNDEAD))
			set_fighting(ch,victim);
		if(difference >= 10 && IS_NPC(victim))
		{
			act("Horrible screams of undeath fill the air as $n is reduced to scattered ashes by brilliant white flames.",
					victim,0,ch,TO_ROOM);
			raw_kill(ch,victim);
			forceflee = FALSE;
		}
		else if(difference > 1)
		{
			dam = dice(ch->level,20);
			act("$n is suddenly engulfed by holy fire!",victim,0,ch,TO_ROOM);
			damage_new(ch,victim,dam,gsn_turn_undead,DAM_HOLY,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);
			forceflee = TRUE;
		}
		else if(difference > -5)
		{
			dam = dice(ch->level,10);
			act("$n is burned by holy fire!",victim,0,ch,TO_ROOM);
			if(number_percent() > 50)
				forceflee = TRUE;
			damage_new(ch,victim,dam,gsn_turn_undead,DAM_HOLY,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);
		}
		else
		{
			if (!IS_AFFECTED(victim,AFF_CHARM)) {
				act("$N shrieks in unholy fury and bounds toward you!",ch,0,victim,TO_CHAR);
				act("$n shrieks in unholy fury and bounds toward $N!",victim,0,ch,TO_ROOM);
				multi_hit(victim,ch,TYPE_UNDEFINED);
				forceflee = FALSE;
			} else {
				forceflee = FALSE;
			}
		}
		if(forceflee == TRUE && !IS_SET(victim->act,ACT_SENTINEL))
		{
			act("$n shambles away in hopes of escaping $N's holy aura!",victim,0,ch,TO_ROOM);
			act("$n shambles away from your holy presence!",ch,0,victim,TO_CHAR);
			do_flee(victim,"");
		}
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = gsn_turn_undead;
	af.aftype = AFT_TIMER;
	af.level = ch->level;
	af.location = APPLY_NONE;
	af.modifier = 0;
	af.duration = 24;
	affect_to_char(ch,&af);

	return;
}

bool check_benevolence (CHAR_DATA *ch, CHAR_DATA *keeper)
{
	if (!IS_GOOD(keeper))
		return FALSE;

	if (number_percent() > get_skill(ch,gsn_benevolence))
		return FALSE;

	act("$N smiles warmly at you and bows slightly, offering you a large discount.",ch,0,keeper,TO_CHAR);

	return TRUE;
}

bool check_intercept(CHAR_DATA *ch, CHAR_DATA *victim, CHAR_DATA *paladin, int dt)
{
	int chance, skill, gsize = 0;
	CHAR_DATA *gch;
	char buf1[MSL], buf2[MSL], buf3[MSL];
	char *attack;

	if (!is_same_group(victim, paladin))
		return FALSE;

	if (!IS_AWAKE(paladin))
		return FALSE;

	if (paladin->fighting != ch)
		return FALSE;

	if (!IS_NPC(ch) && (!can_pk(ch,paladin) || !can_pk(paladin,ch)))
		return FALSE;

	if ((skill = get_skill(paladin,gsn_intercept)) <= 1) 
		return FALSE;

	if (get_eq_char(paladin,WEAR_SHIELD) == NULL)
		return FALSE;

	if (paladin == victim)
		return FALSE;
	
	for(gch = paladin->in_room->people; gch; gch = gch->next_in_room)
		if (is_same_group(paladin,gch))
			gsize++;
	
	switch (gsize)
	{
		case(2):
			chance = 40;
			break;
		case(3):
			chance = 20;
			break;
		default:
			return FALSE;
	}

	skill = skill * (15 + get_skill(ch, gsn_shield_block)/4) / 100;
	
	chance = skill * chance / 100;

	if (!IS_NPC(ch))
		if (check_posture(ch) == POSTURE_DEFENSE && style_check(gsn_posture, ch->pcdata->style))
			chance += 8;

	if (IS_NPC(victim) || IS_NPC(ch))
		chance += (victim->level - ch->level) / 2;

	attack = get_dam_message(ch,dt);

	sprintf(buf1,"You thrust your shield between %s's %s and $N.", 
			IS_NPC(ch) ? ch->short_descr : ch->name, attack);
	act(buf1, paladin, 0, victim, TO_CHAR);
	sprintf(buf2,"%s thrusts $s shield in front of you, deflecting $N's %s.", 
			IS_NPC(paladin) ? paladin->short_descr : paladin->name, attack);
	act(buf2, victim, 0, ch, TO_CHAR);
	sprintf(buf3,"$n thrusts $s shield in front of %s, deflecting your %s.", 
			IS_NPC(victim) ? victim->short_descr : victim->name, attack);
	act(buf3, ch, 0, paladin, TO_CHAR);
	check_improve(paladin, gsn_intercept,TRUE,4);

	return TRUE;
}

void spell_blinding_orb(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo, *vch_next;
	AFFECT_DATA af;
	int dam,hitroll,duration,b_hitroll,b_duration;
	act("You bring forth an orb of blinding light to sear the eyes of your foes!",ch,0,0,TO_CHAR);
	act("$n calls forth a brilliant orb of radiant light!",ch,0,0,TO_ROOM);
	for (victim=ch->in_room->people; victim != NULL; victim = vch_next)
	{
		vch_next = victim->next_in_room;
		if(is_same_group(ch,victim) || is_same_cabal(ch, victim) || is_safe(ch, victim))
			continue;

		dam = dice(level,2);

		b_hitroll = -4;
		b_duration = 1;

		if(IS_GOOD(victim))
		{
			if(number_percent() > 25)
				continue;
			dam /= 3;
			hitroll = b_hitroll;
			duration = b_duration;
		} else if(IS_NEUTRAL(victim)) {
			if(number_percent() > 75)
				continue;
			dam /= 2;
			hitroll = b_hitroll * 2;
			duration = b_duration * 3;
		} else {
			dam = (int)(dam * 1.5);
			hitroll = b_hitroll * 4;
			duration = b_duration * 6;
		}
		char buf2[MSL];
		sprintf(buf2,"Die, %s, you sorcerous dog!", PERS(ch, victim));
		if(!victim->fighting && !IS_NPC(victim))
			do_myell(victim,buf2,ch);
		damage_new(ch,victim,dam,sn,DAM_LIGHT,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);
		if(!saves_spell(ch->level,victim,DAM_LIGHT) && !IS_AFFECTED(victim,AFF_BLIND))
		{
			act("$n falters as the light sears $s eyes.",victim,0,ch,TO_ROOM);
			act("You are blinded by the bright light!",ch,0,victim,TO_VICT);
			init_affect(&af);
			af.where = TO_AFFECTS;
			af.type = sn;
			af.aftype = AFT_COMMUNE;
			af.level = level;
			af.location = APPLY_HITROLL;
			af.modifier = hitroll;
			SET_BIT(af.bitvector,AFF_BLIND);
			af.duration = duration;
			af.mod_name = MOD_VISION;
			affect_to_char(victim, &af);
		}
	}
return;
}

void spell_voice_of_damnation(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo, *vch_next;
	act("Imbued with the strength of your god, you threaten those around you with eternal damnation!",ch,0,victim,TO_CHAR);
	act("$N shouts threats of damnation in a strange and powerful voice!",victim,0,ch,TO_ROOM);
	for(victim = ch->in_room->people; victim !=NULL; victim = vch_next)
	{
		vch_next = victim->next_in_room;
		if(is_same_cabal(ch, victim) || is_same_group(ch,victim) || is_affected(victim,gsn_deafen) || is_safe(ch, victim))
			continue;
		if((IS_NPC(victim) && !IS_EVIL(victim))
		|| (!IS_NPC(victim) && (IS_GOOD(victim) || IS_NEUTRAL(victim)) && (victim->pcdata->kills[PK_GOOD] <= victim->pcdata->kills[PK_EVIL])))
		{
			act("Knowing that you have not persecuted the righteous in the past, you are not concerned.",ch,0,victim,TO_VICT);
			continue;
		}
		act("The terrifying voice resounds in your head, filling you with fear about your fate!",ch,0,victim,TO_VICT);
		damage_new(ch,victim,dice(ch->level,5),sn,DAM_MENTAL,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);
		WAIT_STATE(victim,PULSE_VIOLENCE * 1);
	}
return;
}

void spell_seraphic_mantle(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;
	if(victim != ch)
		return send_to_char("You can only commune this upon yourself.\n\r",ch);
	if(is_affected(ch,sn))
		return send_to_char("You are already protected by a seraphic mantle.\n\r",ch);
	act("A shining mantle of light descends upon your shoulders, protecting you from harm.",ch,0,0,TO_CHAR);
	act("A shining mantle of light descends upon $n, lending $m an angelic presence.",ch,0,0,TO_ROOM);
	init_affect(&af);
	af.aftype = AFT_COMMUNE;
	af.modifier = -40;
	af.location = APPLY_DAM_MOD;
	af.duration = ch->level/5;
	af.level = ch->level;
	af.type = sn;
	af.mod_name = MOD_PROTECTION;
	affect_to_char(ch,&af);
	af.mod_name = MOD_NONE;
	af.modifier = -(ch->level/2);
	af.location = APPLY_SAVES;
	af.level = ch->level;
	af.type = sn;
	affect_to_char(ch,&af);
return;
}

void spell_arms_of_light(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	char arg[MSL];
	OBJ_DATA *weapon;
	AFFECT_DATA af;
	OBJ_AFFECT_DATA oaf;
	target_name = one_argument(target_name,arg);
	if((weapon=get_obj_carry(ch,arg,ch))==NULL)
		return send_to_char("You aren't carrying that.\n\r",ch);
	if(weapon->item_type != ITEM_WEAPON)
		return send_to_char("You can only commune this upon weapons.\n\r",ch);
	if (is_affected(ch,sn))
		return send_to_char("You may not commune this suplication again yet.\n\r",ch);
	if (is_affected_obj(weapon,gsn_arms_of_light) || is_affected_obj(weapon,gsn_arms_of_purity) || is_affected_obj(weapon,gsn_arms_of_wrath) || is_affected_obj(weapon,gsn_arms_of_judgement))
		return send_to_char("This weapon is already enchanted with holy power\n\r",ch);
     	
	init_affect_obj(&oaf);
	oaf.where = TO_OBJ_AFFECTS;
	oaf.type = sn;
	oaf.aftype = AFT_COMMUNE;
	oaf.location = APPLY_NONE;
	oaf.modifier = 0;
	oaf.duration = level;
	oaf.level = ch->level;
	oaf.owner = ch;
	affect_to_obj(weapon,&oaf);
	act("A bright glow begins to emanate from $n's $p.",ch,weapon,0,TO_ROOM);
	act("You enchant $p with holy light.",ch,weapon,0,TO_CHAR);
	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = sn;
	af.location = APPLY_NONE;
	af.modifier = 0;
	af.aftype = AFT_TIMER;
	af.duration = ch->level;
	af.level = ch->level;
	affect_to_char(ch, &af);
	
	WAIT_STATE(ch,PULSE_VIOLENCE*2);

	return;
}

void spell_arms_of_purity(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	char arg[MSL];
	OBJ_DATA *weapon;
	AFFECT_DATA af;
	OBJ_AFFECT_DATA oaf;
        target_name = one_argument(target_name,arg);
	
	if((weapon=get_obj_carry(ch,arg,ch))==NULL)
		return send_to_char("You aren't carrying that.\n\r",ch);
	if(weapon->item_type != ITEM_WEAPON)
		return send_to_char("You can only commune this upon weapons.\n\r",ch);
	if(is_affected_obj(weapon,gsn_arms_of_light) || is_affected_obj(weapon,gsn_arms_of_purity) || is_affected_obj(weapon,gsn_arms_of_wrath) || is_affected_obj(weapon,gsn_arms_of_judgement))
		return send_to_char("Your weapon is already enchanted with holy power.\n\r",ch);
	if(is_affected(ch,sn))
		return send_to_char("You may not commune this supplication again yet.\n\r",ch);
	
	init_affect_obj(&oaf);
	oaf.where = TO_OBJ_AFFECTS;
	oaf.level = ch->level;
	oaf.type = sn;
	oaf.duration = level;
	oaf.location = APPLY_NONE;
	oaf.modifier = 0;
	oaf.owner = ch;
	affect_to_obj(weapon,&oaf);
	act("Rippling waves of warm energy play up and down $n's $p.",ch,weapon,0,TO_ROOM);
	act("You enchant $p with righteous purity.",ch,weapon,0,TO_CHAR);
	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = sn;
	af.duration = level;
	af.modifier = 0;
	af.location = APPLY_NONE;
	af.level = ch->level;
	af.aftype = AFT_TIMER;
	affect_to_char(ch,&af);
	WAIT_STATE(ch,PULSE_VIOLENCE * 2);
	
	return;
}
	
void spell_arms_of_wrath(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	char arg[MSL];
	OBJ_DATA *weapon;
	AFFECT_DATA af;
	OBJ_AFFECT_DATA oaf;
	target_name = one_argument(target_name,arg);
	if((weapon=get_obj_carry(ch,arg,ch))==NULL)
		return send_to_char("You aren't carrying that.\n\r",ch);
	if(weapon->item_type != ITEM_WEAPON)
		return send_to_char("You can only commune this upon weapons.\n\r",ch);
	if (is_affected(ch,sn))
		return send_to_char("You may not commune this supplication again yet.\n\r",ch);
	if (is_affected_obj(weapon,gsn_arms_of_light) || is_affected_obj(weapon,gsn_arms_of_purity) || is_affected_obj(weapon,gsn_arms_of_wrath) || is_affected_obj(weapon,gsn_arms_of_judgement))
		return send_to_char("This weapon is already enchanted with holy power!\n\r",ch);

	init_affect_obj(&oaf);
	oaf.where = TO_OBJ_AFFECTS;
	oaf.type = sn;
	oaf.aftype = AFT_COMMUNE;
	oaf.location = APPLY_NONE;
	oaf.modifier = 0;
	oaf.duration = level;
	oaf.level = ch->level;
	oaf.owner = ch;
	affect_to_obj(weapon,&oaf);
	act("$n's $p is set ablaze with white flames!",ch,weapon,0,TO_ROOM);
	act("You enchant $p with holy wrath.",ch,weapon,0,TO_CHAR);
	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = sn;
	af.location = APPLY_NONE;
	af.modifier = 0;
	af.aftype = AFT_TIMER;
	af.duration = ch->level;                                                                     
	af.level = ch->level;                                                                        
	affect_to_char(ch, &af);                                                                     

	WAIT_STATE(ch,PULSE_VIOLENCE*2);                                                             

	return;
}

void spell_arms_of_judgement(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	char arg[MSL];
	OBJ_DATA *weapon;
	AFFECT_DATA af;
	OBJ_AFFECT_DATA oaf;
	target_name = one_argument(target_name,arg);
	if((weapon=get_obj_carry(ch,arg,ch))==NULL)
		return send_to_char("You aren't carrying that.\n\r",ch);

	if(weapon->item_type != ITEM_WEAPON)
		return send_to_char("You can only commune this upon weapons.\n\r",ch);
	if (is_affected(ch,sn))
		return send_to_char("You may not commune this supplication again yet.\n\r",ch);
	if (is_affected_obj(weapon,gsn_arms_of_light) || is_affected_obj(weapon,gsn_arms_of_purity) || is_affected_obj(weapon,gsn_arms_of_wrath) || is_affected_obj(weapon,gsn_arms_of_judgement))
		return send_to_char("This weapon is already enchanted with holy power!\n\r",ch);

	init_affect_obj(&oaf);
	oaf.where = TO_OBJ_AFFECTS;
	oaf.type = sn;
	oaf.aftype = AFT_COMMUNE;
	oaf.location = APPLY_NONE;
	oaf.modifier = 0;
	oaf.duration = level;
	oaf.level = ch->level;
	oaf.owner = ch;
	affect_to_obj(weapon,&oaf);
	act("Bright golden light radiates from $n's $p!",ch,weapon,0,TO_ROOM);
	act("You enchant $p with the power of divine judgement.",ch,weapon,0,TO_CHAR);
	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = sn;
	af.location = APPLY_NONE;
	af.modifier = 0;
	af.aftype = AFT_TIMER;
	af.duration = ch->level;                                                                     
	af.level = ch->level;                                                                        
	affect_to_char(ch, &af);                                                                     

	WAIT_STATE(ch,PULSE_VIOLENCE*2);                                                             

	return;
}

void do_strike_of_virtue(CHAR_DATA *ch, char *argument)
{
	OBJ_DATA *weapon;
	CHAR_DATA *victim = ch->fighting;
	weapon = get_eq_char(ch,WEAR_WIELD);
	if(ch->position != POS_FIGHTING)
		return send_to_char("You must be fighting to perform this maneuver.\n\r",ch);
	if(weapon == NULL)
		return send_to_char("You must be wielding a weapon.\n\r",ch);

	if(get_skill(ch,gsn_strike_of_virtue)*.85>number_percent())
	{
		act("$n brings $s $p around for a particularly brutal blow!",ch,weapon,0,TO_ROOM);
		act("You deliver a brutal blow to $N!",ch,0,victim,TO_CHAR);
		one_hit_new(ch,victim,gsn_strike_of_virtue,HIT_SPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,125,NULL);
		check_improve(ch,gsn_strike_of_virtue,TRUE,1);
	}
	else
	{
		act("You swing hard, but fail to connect with your opponent.",ch,0,0,TO_CHAR);
		damage(ch,victim,0,gsn_strike_of_virtue,DAM_NONE,TRUE);
		check_improve(ch,gsn_strike_of_virtue,FALSE,1);
	}
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	return;
}

void spell_divine_frenzy(int sn,int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	
	if(is_affected(ch,gsn_divine_frenzy))
		return send_to_char("You are already in a divine frenzy!\n\r",ch);
	
	act("You go into a vengeful rage against the wicked!",ch,0,0,TO_CHAR);
	act("$n begins to breathe heavily as a vengeful look spreads across $s face.",ch,0,0,TO_ROOM);
	
	init_affect(&af);
	af.type = sn;
	af.where = TO_AFFECTS;
	af.aftype = AFT_COMMUNE;
	af.location = APPLY_HITROLL;
	af.modifier = 15;
	af.duration = 8;
	af.level = ch->level;
	affect_to_char(ch,&af);
	af.location = APPLY_DAMROLL;
	af.modifier = 15;
	af.duration = 8;
	af.level = ch->level;
	affect_to_char(ch,&af);
	
	return;
}

void do_group_retreat(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *to = NULL;
	CHAR_DATA *vch, *vch_next;
	CHAR_DATA *victim = ch->fighting;
	ROOM_INDEX_DATA *to_room = NULL;
	char buf[MAX_INPUT_LENGTH];
	EXIT_DATA *pexit;
	int dir;
	int skill;
	char *direction;

	one_argument(argument,arg);
	
	if(!ch->fighting)
		return send_to_char("You aren't fighting!\n\r",ch);	
	if(arg[0] == '\0')
		return send_to_char("Retreat in which direction?\n\r",ch);

	if(!str_cmp(arg, "n") || !str_cmp(arg, "north")) dir = 0;
	else if(!str_cmp(arg, "e") || !str_cmp(arg, "east"))  dir = 1;
	else if(!str_cmp(arg, "s") || !str_cmp(arg, "south")) dir = 2;
	else if(!str_cmp(arg, "w") || !str_cmp(arg, "west"))  dir = 3;
	else if(!str_cmp(arg, "u") || !str_cmp(arg, "up" ))   dir = 4;
	else if(!str_cmp(arg, "d") || !str_cmp(arg, "down"))  dir = 5;
	else return send_to_char("Retreat in which direction?\n\r",ch);
	
	pexit = ch->in_room->exit[dir];
	if((skill = get_skill(ch,gsn_group_retreat)) == 0)
		return send_to_char("Huh?\n\r",ch);
	if(skill-10 < number_percent())
	{
		WAIT_STATE(ch,PULSE_VIOLENCE);
		for(to = ch->in_room->people; to != NULL; to = to->next_in_room)
		{
			if(is_same_group(victim,to))
				one_hit_new(to,ch,TYPE_UNDEFINED,HIT_SPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,125,NULL);
		}
				
					
		return send_to_char("You expose yourself to your foes, but fail to cover your group's retreat!\n\r",ch);
	}	
	if(pexit != NULL 
	   && ((to_room = pexit->u1.to_room) != NULL)
	   && (!IS_SET(pexit->exit_info,EX_CLOSED)
	   || ((IS_SET(pexit->exit_info,EX_CLOSED) 
	   && IS_AFFECTED(ch,AFF_PASS_DOOR))
	   && !IS_SET(pexit->exit_info,EX_NOPASS))))
	{
		direction = (char*)flag_name_lookup(dir,direction_table);
		act("You quickly position yourself to cover your group's retreat!",ch,0,0,TO_CHAR);
		act("$n positions $mself to cover $s group's retreat!",ch,0,0,TO_ROOM);
		for(to = ch->in_room->people; to != NULL; to = vch_next)
		{
			vch_next = to->next_in_room;
			if(is_same_group(victim,to))
				one_hit_new(to,ch,TYPE_UNDEFINED,HIT_SPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);
			if(is_same_group(to,ch) && to != ch)
			{
				stop_fighting(to,TRUE);
				act("$n retreats to the $t under $N's cover!",ch,direction,to,TO_NOTVICT);
				act("$N retreats under your cover!",ch,0,to,TO_CHAR);
				act("You retreat under $n's cover.",ch,0,to,TO_VICT);
				char_from_room(to);
				char_to_room(to,to_room);
			}
		}
		act("$s group having retreated, $n follows suit!",ch,0,0,TO_ROOM);
		stop_fighting(ch,TRUE);
		char_from_room(ch);
		char_to_room(ch,to_room);
		send_to_char("You retreat from combat to join your groupmates!\n\r",ch);
		return do_look(ch, "auto");
	}
	else
		return send_to_char("You can't retreat in that direction!\n\r",ch);
	
	return;
}

void do_valiant_charge(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	CHAR_DATA *to;
	char buf[MAX_INPUT_LENGTH];
	int count = 0;

	one_argument(argument,arg);
	if(arg[0] == '\0')
		return send_to_char("Lead a charge against whom?\n\r",ch);
	else if((victim = get_char_room(ch,arg))==NULL)
		return send_to_char("They aren't here.\n\r",ch);
	if(ch == victim || is_same_group(ch,victim))
		return send_to_char("Lead your group into combat against themselves!?\n\r",ch);
	if(victim->hit < victim->hit/2)
		return send_to_char("They are too injured and wary to meet your charge.\n\r",ch);
	for(to = ch->in_room->people; to!=NULL; to = to->next_in_room)
	{
		if(is_same_group(ch,to) && to != ch)
		{
			count++;
			break;
		}
	}
	if(count == 0)
		return send_to_char("It's hard to lead a group into combat if you don't have a group.\n\r",ch);
	
	act("$n charges into combat with $N, bursting through $S defenses!",ch,0,victim,TO_ROOM);
	act("$n drops his guard and charges directly into you!",ch,0,victim,TO_VICT);
	act("You charge into combat, opening the way for others to strike!",ch,0,victim,TO_CHAR);
	for(to = ch->in_room->people; to!=NULL; to = to->next_in_room)
	{	
		if(is_same_group(victim,to))
			one_hit_new(to,ch,TYPE_UNDEFINED,HIT_SPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);		
		if(is_same_group(ch,to) && to != ch && count < 5)
		{
			count++;
			one_hit_new(to,victim,TYPE_UNDEFINED,HIT_SPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);
		}

	}
	WAIT_STATE(ch,PULSE_VIOLENCE * 3);
	return;
}


void spell_awe(int level, int sn, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	
	if(is_affected(ch, gsn_awe))
		return send_to_char("You are already radiating an awe-inspiring holy aura.\n\r",ch);

	act("Holy energy crackles about your frame, forming a shimmering aura of light!",ch,0,0,TO_CHAR);
	act("Godly light suddenly explodes from within $n, finally settling into an incredible and almost intoxicating aura of power!",
			ch,0,0,TO_ROOM);
	init_affect(&af);
	af.level = ch->level;
	af.type = gsn_awe;
	af.where = TO_AFFECTS;
	af.aftype = AFT_COMMUNE;
	af.location = APPLY_NONE;
	af.modifier = 0;
	af.duration = 20;
	af.mod_name = MOD_APPEARANCE;
	affect_to_char(ch,&af);
	return;
}

void spell_shield_of_faith(int level, int sn, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	CHAR_DATA *to = (CHAR_DATA *)vo;

	if(to == NULL)
		to = ch;
	
	if(is_affected(ch,gsn_shield_of_faith) && ch == to)
		return send_to_char("You are already surrounded by a shield of faith.\n\r",ch);
	if(is_affected(to,gsn_shield_of_faith))
		return send_to_char("They are already surrounded by a shield of faith.\n\r",ch);

	act("A pearly white barrier flickers into existence around you.",to,0,0,TO_CHAR);
	act("A pearly white barrier flickers into existence around $n.",to,0,0,TO_ROOM);
	init_affect(&af);
	af.level = ch->level;
	af.type = gsn_shield_of_faith;
	af.where = TO_AFFECTS;
	af.aftype = AFT_COMMUNE;
	af.location = APPLY_NONE;
	af.modifier = 0;
	af.duration = ch->level/3;
	af.mod_name = MOD_PROTECTION;
	affect_to_char(to,&af);
return;
}

void spell_holy_shroud(int level, int sn, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	CHAR_DATA *to = (CHAR_DATA *)vo;

	if(to == NULL)
		to = ch;

	if(is_affected(ch,gsn_holy_shroud) && ch == to)
		return send_to_char("You are already protected by a holy shroud.\n\r",ch);
	if(is_affected(to,gsn_holy_shroud))
		return send_to_char("They are already protected by a holy shroud.\n\r",ch);

	act("A luminescent veil of white mist slowly drapes itself about your frame.",to,0,0,TO_CHAR);
	act("A luminescent veil of white mist slowly drapes itself about $n's frame.",to,0,0,TO_ROOM);
	init_affect(&af);
	af.level = ch->level;
	af.type = gsn_holy_shroud;
	af.where = TO_AFFECTS;
	af.aftype = AFT_COMMUNE;
	af.location = APPLY_NONE;
	af.modifier = 0;
	af.duration = ch->level/5;
	af.mod_name = MOD_PROTECTION;
	affect_to_char(to,&af);
return;
}



int check_arms(CHAR_DATA *ch, OBJ_DATA *wield, bool bOncePerRound)
{
	CHAR_DATA *victim = ch->fighting;
	AFFECT_DATA af;

	if(is_affected_obj(wield,gsn_arms_of_light))
	{        
		if(bOncePerRound == TRUE && number_percent() > 90)
		{
			act("$p suddenly flares brightly!",0,wield,0,TO_ROOM);
			if(!saves_spell(ch->level,victim,DAM_LIGHT))
			{       
				act("$n appears to be blinded.",victim,0,0,TO_ROOM);
				act("You are blinded!",ch,0,victim,TO_VICT);
				init_affect(&af);
				af.where = TO_AFFECTS;
				af.aftype = AFT_SKILL;
				af.type = gsn_blindness;
				af.duration = ch->level/12;
				af.modifier = -4;
				af.location = APPLY_HITROLL;
				SET_BIT(af.bitvector,AFF_BLIND);
				affect_to_char(victim,&af);       
			}
		}

		if(bOncePerRound == FALSE && number_percent() < ch->level/2)
			return 46;
	}

	if(is_affected_obj(wield,gsn_arms_of_wrath) && bOncePerRound == FALSE)
	{
		if(number_percent() > 96 && victim)
		{
		act("$p explodes in holy flames!",ch,wield,0,TO_ALL);
		damage_new(ch,victim,dice(ch->level,2),0,DAM_HOLY,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"holy wrath");

		if(number_percent() < ch->level/2)
			return 18;
		}
	}

	if(is_affected_obj(wield,gsn_arms_of_purity))	
	{
		if(bOncePerRound == TRUE && number_percent() > 75)
		{
			act("$p glows softly and grows warm in your hands.",ch,wield,0,TO_CHAR);
			act("$n's $p glows softly.",ch,wield,0,TO_ROOM);
			obj_cast_spell(skill_lookup("cure critical"), ch->level,ch,ch,wield);
		}
	}

	if(is_affected_obj(wield,gsn_arms_of_judgement))
	{	       
		if(bOncePerRound == TRUE && number_percent() > 75 && !IS_NPC(victim) && (IS_EVIL(victim) || victim->pcdata->kills[PK_GOOD] < victim->pcdata->kills[PK_EVIL]))
		{
			if(!saves_spell(ch->level,victim,DAM_HOLY))
			{
				act("Brilliant light, followed by a loud booming noise emanates from $n's $p!",ch,wield,0,TO_ROOM);
				act("$p shakes violently in your hands, exploding with holy vengeance!",ch,wield,0,TO_CHAR);
				act("An invisible force bears down upon you.",ch,0,victim,TO_VICT);
				act("$n staggers and reels, overcome by an invisible force!",victim,0,0,TO_ROOM);
				act("You succumb to the force, becoming temporarily paralyzed!",victim,0,0,TO_CHAR);
				WAIT_STATE(victim,PULSE_VIOLENCE * 2);
			}
		}
		if(bOncePerRound == FALSE && ch->level/2 > number_percent())
			return 20;
	}
	
	return 0;
}

void spell_empathy(int level, int sn, CHAR_DATA *ch, void *vo, int target)
{
    AFFECT_DATA af;
    CHAR_DATA *vict = (CHAR_DATA *)vo;


	if(ch == vict)
		return send_to_char("You can not commune this on yourself.\n\r", ch);
	
	if(is_affected(vict,gsn_empathy))
		return send_to_char("They are already being empathized with.\n\r",ch);

	act("You touch your hand to $N's forehead and form a spiritual link.",ch,0,vict,TO_CHAR);
	act("$n touches $s hand to your forehead and forms a spiritual link.",ch,0,vict,TO_VICT);
	act("$n touches $N on $S forehead forming a spiritual link.", ch, 0, vict, TO_ROOM);

	init_affect(&af);
	af.owner = ch;
	af.level = ch->level;
	af.type = gsn_empathy;
	af.where = TO_AFFECTS;
	af.aftype = AFT_COMMUNE;
	af.location = APPLY_NONE;
	af.modifier = 0;
	af.duration = 12;
	af.end_fun = empathy_end;
	affect_to_char(vict,&af);

}

void empathy_end(CHAR_DATA *ch, AFFECT_DATA *af)
{
	//ch=char with empathy, af->owner = paladin
	if(af->owner)
		act("You feel pained as your spiritual link with $n is severed!", af->owner, 0, ch, TO_VICT);
}

void spell_tower_of_fortitude(int level, int sn, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	CHAR_DATA *vict = (CHAR_DATA *)vo;

	if(is_affected(ch, gsn_tower_of_fortitude))
		return send_to_char("You are already affected by tower of fortitude.\n\r", ch);

	act("You center your body and relax your muscles ready for incoming attacks.",ch, 0, 0, TO_CHAR);
	act("$n centers $mself and readies for combat.", ch, 0, 0, TO_ROOM);

	init_affect(&af);
	af.owner = ch;
	af.level = ch->level;
	af.type = gsn_tower_of_fortitude;
	af.where = TO_AFFECTS;
	af.aftype = AFT_COMMUNE;
	af.location = APPLY_NONE;
	af.modifier = 0;
	af.duration = ch->level / 6;
	af.mod_name = MOD_PROTECTION;
	affect_to_char(ch, &af);
}

void spell_indomitable_spirit(int level, int sn, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	CHAR_DATA *vict = (CHAR_DATA *)vo;

	if(is_affected(ch, gsn_indomitable_spirit))
		return send_to_char("You are already affected by an indomitable spirit.\n\r", ch);

	act("You call upon your deity to raise your spirit.", ch, 0, 0, TO_CHAR);
	act("$n calls up on $s deity to raise his spirit.", ch, 0, 0, TO_ROOM);

	init_affect(&af);
	af.owner = ch;
	af.level = ch->level;
	af.type = gsn_indomitable_spirit;
	af.where = TO_AFFECTS;
	af.aftype = AFT_COMMUNE;
	af.location = APPLY_NONE;
	af.modifier = 0;
	af.duration = ch->level / 5;
	af.beat_fun = ispirit_beat;
	af.end_fun = ispirit_end;
	af.mod_name = MOD_TOUGHNESS;
	affect_to_char(ch, &af);
}

void ispirit_beat(CHAR_DATA *ch, AFFECT_DATA *af)
{
	int diff = 0;

	if(ch->hit < 0)
	{
		diff = abs(ch->hit);
		if(diff > (ch->mana / 2))
		{
			af->end_fun 	= NULL;
			affect_strip(ch, gsn_indomitable_spirit);
			act("Your body collapses under the strain of your spiritual exertion.", ch, 0, 0, TO_CHAR);
			act("$n's body collapses under the strain of $s spiritual exertion!", ch, 0, 0, TO_ROOM);
			act( "$n is DEAD!!", ch, 0, 0, TO_ROOM );
			if(ch->fighting)
				raw_kill(ch->fighting, ch);
			else
				raw_kill(ch, ch);
		}
	}
}

void ispirit_end(CHAR_DATA *ch, AFFECT_DATA *af)
{
	if(ch->hit < 0)
	{
		act("Your body collapses under the strain of your spiritual exertion.", ch, 0, 0, TO_CHAR);
		act("$n's body collapses under the strain of $s spiritual exertion!", ch, 0, 0, TO_ROOM);
		act( "$n is DEAD!!", ch, 0, 0, TO_ROOM );
		raw_kill(ch, ch);
	}
}

void spell_altruism(int level, int sn, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA *from_af, *faf_next, to_af;
	CHAR_DATA *vict = (CHAR_DATA *)vo;

	if(ch == vict)
		return send_to_char("You can only commune this on others.\n\r", ch);
	
	for (from_af = vict->affected; from_af != NULL; from_af = faf_next )
	{
		faf_next = from_af->next;
		if (skill_table[from_af->type].dispel & CAN_CLEANSE)
		{
			init_affect(&to_af);
			to_af.owner = from_af->owner;
			if(from_af->name != NULL)
				to_af.name = talloc_string(from_af->name);
			to_af.valid = from_af->valid;
			to_af.where = from_af->where;
			to_af.type = from_af->type;
			to_af.level = from_af->level;
			to_af.duration = from_af->duration;
			to_af.location = from_af->location;
			to_af.modifier = from_af->modifier;
			copy_vector(to_af.bitvector, from_af->bitvector);
			to_af.aftype = from_af->aftype;
			to_af.tick_fun = from_af->tick_fun;
			to_af.pulse_fun = from_af->pulse_fun;
			to_af.end_fun = from_af->end_fun;
			to_af.init_duration = from_af->init_duration;
			affect_to_char(ch, &to_af);
			affect_remove(vict, from_af);
		}
	}
	act("You absorb all of $N's maledictions.", ch, 0, vict, TO_CHAR);
	act("You feel cleansed as $n absorbs your maledictions.", ch, 0, vict, TO_VICT);
	act("$n absorbs all of $N's maledictions.", ch, 0, vict, TO_ROOM);

}
