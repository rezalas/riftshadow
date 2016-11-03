/* Home for everything Anti */
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

DECLARE_DO_FUN(do_taunt);
char *	format_obj_to_char	args(( OBJ_DATA *obj, CHAR_DATA *ch, bool fShort ));
void command_execute_delay	args(( CHAR_DATA *ch, char *command));
void furcas_vanish (CHAR_DATA *ch, CHAR_DATA *mob);
void insanity_two (CHAR_DATA *ch, int room);
void insanity_fight (CHAR_DATA *ch);
void mephisto_two (CHAR_DATA *ch, CHAR_DATA *victim, char *argument);

void check_bloodlust(CHAR_DATA *ch, CHAR_DATA *victim)
{
	AFFECT_DATA *af_old, af;
	int skill;
	int level = ch->level;
	
	if ((skill = get_skill(ch, gsn_bloodlust)) < number_percent()) {
		check_improve(ch,gsn_bloodlust,FALSE,1);
		return;
	}

	send_to_char("The sight of innocent blood fills your heart with unholy vigor!\n\r",ch);
	
	af_old = affect_find(ch->affected,gsn_bloodlust);
	
	if(af_old != NULL) {
		af_old->duration += UMIN(288,af_old->duration + 96);
	} else {
		init_affect(&af);
		af.where 		= 	TO_AFFECTS;
		af.aftype 		= 	AFT_SKILL;
		af.type 		= 	gsn_bloodlust;
		af.owner 		= 	ch;
		af.level 		= 	level;
		af.duration		=	96;
		af.modifier 	= 	level*2;
		af.location 	= 	APPLY_HIT;
		affect_to_char(ch,&af);

		af.location 	= 	APPLY_MANA;
		affect_to_char(ch,&af);
		
		af.modifier 	= 	level/4;
		af.location 	= 	APPLY_DAMROLL;
		affect_to_char(ch,&af);
	
		af.location 	= 	APPLY_HITROLL;
		affect_to_char(ch,&af);
	}

	check_improve(ch,gsn_bloodlust,TRUE,6);

	return;
}

void check_leech(CHAR_DATA *ch, CHAR_DATA *victim)
{
	int leeched, skill;
	
	if ((skill = get_skill(ch,gsn_leech)) < number_percent())
		return;
	
	if (IS_SET(victim->imm_flags,IMM_MAGIC))
		return;
	
	leeched = UMIN(victim->hit/100,number_range(8,12));
	
	if(victim->hit <= leeched)
		return;

	victim->hit -= leeched;
	ch->hit += leeched;

	check_improve(ch,gsn_leech,TRUE,6);
	
	return;
}

void spell_indomitability(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	
	act("You call upon the force of your will, using it to maintain your body's current state.",ch,0,0,TO_CHAR);
	act("$n looks more determined.",ch,0,0,TO_ROOM);

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = gsn_indom;
	af.owner = ch;
	af.level = level;
	af.duration = level/5;
	af.modifier = 0;
	af.location = 0;
	affect_to_char(ch,&af);

	return;
}

void do_taunt(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim;
	int chance;
	char arg[MAX_INPUT_LENGTH];
	argument = one_argument(argument,arg);

	if((victim=get_char_room(ch,arg)) == NULL)
		return send_to_char("They aren't here.\n\r",ch);
	
	if(ch == victim)
		return send_to_char("Funny.\n\r", ch);

	chance = get_skill(ch,skill_lookup("taunt")) - 20;	

	if(!IS_NPC(victim)
		&& IS_LAWFUL(victim)
		&& IS_SET(victim->in_room->room_flags,ROOM_LAW))
		chance -= 50;

	chance += 4*(get_curr_stat(ch,STAT_INT) - get_curr_stat(victim,STAT_INT));
	
	if(number_percent() < chance) {
		act("You shout and jeer at $N, angering $M until $E attacks!",ch,0,victim,TO_CHAR);
		act("$n shouts insults and curses at you and you attack $m in a fit of rage!",ch,0,victim,TO_VICT);
		act("$n shouts and curses at $N, angering $M until $E attacks!",ch,0,victim,TO_NOTVICT);
		do_murder(victim,ch->name);
		check_improve(ch,skill_lookup("taunt"),TRUE,1);
	} else {
		act("You shout and jeer at $N, but $E shrugs it off.",ch,0,victim,TO_CHAR);
		act("$n shouts insults and curses at you, but you calmly ignore $m.",ch,0,victim,TO_VICT);
		act("$n shouts and curses at $N, but $N ignores $m.",ch,0,victim,TO_NOTVICT);
		check_improve(ch,skill_lookup("taunt"),FALSE,1);
	}

	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	return;
}
	
void spell_wrack(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	
	if(is_affected(victim,skill_lookup("wrack")))
		return send_to_char("They are already wracked with anger.\n\r",ch);
	
	act("You infuse $N's body with your anger, causing $M to be vulnerable to attack!",ch,0,victim,TO_CHAR);
	act("You feel a sudden surge of anger, followed by wracking pain.",ch,0,victim,TO_VICT);
	act("$n infuses $s anger into $N, leaving $M vunlnerable.",ch,0,victim,TO_NOTVICT);
	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = sn;
	af.aftype = AFT_INVIS;
	af.duration = ch->level/15;
	af.level = level;
	af.location = APPLY_DAM_MOD;
	af.modifier = 20;
	af.owner = ch;
	affect_to_char(victim,&af);
	af.aftype = AFT_MALADY;
	af.modifier = -1;
	af.location = APPLY_STR;
	af.tick_fun = wrack_tick;
	affect_to_char(victim,&af);
	af.modifier = -1;
	af.location = APPLY_DEX;
	af.tick_fun = wrack_tick;
	affect_to_char(victim,&af);
	
	return;
}

void spell_radiance(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	
	act("You channel your magic towards augmenting your beauty.",ch,0,0,TO_CHAR);
	act("$n appears to glow momentarily.",ch,0,0,TO_ROOM);
	
	if(is_affected(ch,sn))
		return send_to_char("You are already augmenting your beauty.\n\r",ch);

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.owner = ch;
	af.level = level;
	af.location = 0;
	af.modifier = 3;
	af.type = sn;
	af.aftype = AFT_SPELL;
	af.duration = level/10;
	af.mod_name = MOD_APPEARANCE;
	affect_to_char(ch,&af);

	return;
}

void wrack_tick(CHAR_DATA *ch, AFFECT_DATA *af)
{
	if(number_percent() > (4 * get_curr_stat(ch,STAT_CON) - 5)) {
		send_to_char("Searing pain shoots through your limbs, sapping your strength!\n\r",ch);
		af->modifier-= 2;
		return;
	}
}

void spell_inspire_lust(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;	
	char buf[MSL];

	if(is_affected(victim,sn))
		return send_to_char("They are already filled with lust.",ch);
	
	if(!IS_NPC(victim)
		&& IS_LAWFUL(victim)
		&& IS_SET(victim->in_room->room_flags,ROOM_LAW))
		level-=15;
	
	if((victim!=ch) && (!saves_spell(level,victim,DAM_NEGATIVE))) {
		act("You call upon your unholy powers to taint $N's mind with greed!",ch,0,victim,TO_CHAR);
		act("A dark flash of greed fills your mind with thoughts of riches.",ch,0,victim,TO_VICT);
	
		init_affect(&af);
		af.where = TO_AFFECTS;
		af.aftype = AFT_SPELL;
		af.level = level;
		af.location = 0;
		af.modifier = 0;
		af.owner = ch;
		af.pulse_fun = lust_pulse;
		af.type = sn;
		af.duration = level/6;
		affect_to_char(victim,&af);
		return;
	} else if(victim!=ch) {
		act("You failed to fill $N's mind with greed.",ch,0,victim,TO_CHAR);
		if(!IS_NPC(victim)) {
			sprintf(buf,"Die, %s, you sorcerous dog!",ch->name);
			do_myell(victim,buf, ch);
		}
		multi_hit(victim,ch,TYPE_UNDEFINED);
		return;
	}

	if(victim==ch) {
		act("A dark flash of greed fills your mind with thoughts of riches.",ch,0,victim,TO_CHAR);
		init_affect(&af);
		af.where = TO_AFFECTS;
		af.aftype = AFT_SPELL;
		af.level = level;
		af.location = 0;
		af.modifier = 0;
		af.owner = ch;
		af.pulse_fun = lust_pulse;
		af.type = sn;
		af.duration = level/6;
		affect_to_char(victim,&af);
		return;
		}

	return;
}


void lust_pulse(CHAR_DATA *ch, AFFECT_DATA *af)
{
	OBJ_DATA *steal;
	CHAR_DATA *victim;
	char buf[MSL];
	int skill;

	if (ch->fighting || !IS_AWAKE(ch))
		return;
	
	skill = (int)(get_skill(ch,skill_lookup("steal")) * 0.8);
	
	if (skill == 0)
		skill = 50;
	
	for(victim = ch->in_room->people; victim; victim = victim->next_in_room) {
		if((victim == ch)
			|| (is_safe_new(victim,ch,FALSE))
			|| number_percent() < 95)
			continue;

		for(steal = victim->carrying; steal; steal=steal->next_content) {
			if(steal->wear_loc != WEAR_NONE
				|| !can_see_obj(ch,steal)
				|| number_percent() < 95)
				continue;
			
			act("Overcome by greed, you suddenly reach for $N's pack!",ch,0,victim,TO_CHAR);
			
			if(number_percent() > skill) {
				sprintf(buf,"%s tried to steal from me!",ch->name);
				do_myell(victim,buf, ch);
				if(IS_NPC(victim))
					multi_hit(victim,ch,TYPE_UNDEFINED);
			} else {
				obj_from_char(steal);
				obj_to_char(steal,ch);
				act("You carefully slip $p away from $N.",ch,steal,victim,TO_CHAR);
			}
			return;
		}
	}
	
	return;
}


void spell_dispaters(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim;
	OBJ_DATA *obj;
	char buf[MSL],arg[MAX_INPUT_LENGTH],buf2[MSL];
	int wear;

	target_name = one_argument(target_name,arg);

	if(((victim=get_char_world(ch,arg)) == NULL) || (!can_see(ch,victim)))
		return send_to_char("They aren't here.\n\r",ch);
	sprintf(buf,"Your lusty senses find %s at %s.\n\r",victim->name,get_room_name(victim->in_room));
	sprintf(buf2,"%s is wearing:\n\r",victim->name);
	send_to_char(buf,ch);
	send_to_char(buf2,ch);
	
	for(wear = 0; wear < MAX_WEAR; wear++) {
		if((obj = get_eq_char(victim,wear)) != NULL &&
		(can_see_obj(ch,obj))) {
			send_to_char(get_where_name(wear),ch);
			send_to_char(format_obj_to_char(obj,ch,TRUE),ch);
			send_to_char("\n\r",ch);
		}
	}
return;
}			

void do_consume(CHAR_DATA *ch, char *argument)
{
	OBJ_DATA *corpse;
	char arg[MAX_INPUT_LENGTH];
	int skill=get_skill(ch,skill_lookup("consume"));

	argument = one_argument(argument,arg);
	
	if(arg[0] == '\0')
		return send_to_char("Consume what?\n\r",ch);
	if(skill <= 1)
		return send_to_char("The thought of such a loathesome act repels you.\n\r",ch);

	if((corpse = get_obj_here(ch,arg)) == NULL)
		return send_to_char("You do not see that here.\n\r",ch);

	if(corpse->item_type != ITEM_CORPSE_PC && corpse->item_type != ITEM_CORPSE_NPC)
		return send_to_char("That is not a suitable corpse.\n\r",ch);
	
	if(corpse->item_type == ITEM_CORPSE_PC && corpse->contains != NULL)
		return send_to_char("It's too heavily laden for you to eat.\n\r",ch);

	if(corpse->item_type == ITEM_CORPSE_NPC && (ch->level - corpse->level > 9))
		return send_to_char("That corpse is not powerful enough to feast on.\n\r",ch);

	if(number_percent() > skill) {
		act("You tear at $p with your teeth, but fail to heal yourself from it.",ch,corpse,0,TO_CHAR);
		act("$n tears at $p with $s teeth and comes up looking angry.",ch,corpse,0,TO_ROOM);
		extract_obj(corpse);
	} else {
		act("You tear at $p with your teeth and heal yourself with its blood.",ch,corpse,0,TO_CHAR);
		act("$n tears at $p with $s teeth and suddenly looks better.",ch,corpse,0,TO_ROOM);
		ch->hit += dice(corpse->level,2);
		extract_obj(corpse);
	}

	return;
}

void spell_baals_mastery(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	int weapon;

	if(is_affected(ch,gsn_baals_mastery))
		return send_to_char("Your Lord will only grant you mastery of one type of weapon!\n\r",ch);
	
	if (!str_cmp(target_name,"axe"))
		weapon = WEAPON_AXE;
	else if (!str_cmp(target_name,"mace"))
		weapon = WEAPON_MACE;
	else if (!str_cmp(target_name,"sword"))
		weapon = WEAPON_SWORD;
	else if (!str_cmp(target_name,"spear"))
		weapon = WEAPON_SPEAR;
	else if (!str_cmp(target_name,"flail"))
		weapon = WEAPON_FLAIL;
	else {
		send_to_char("That is not a valid weapon type.\n\r",ch);
		send_to_char("Syntax: c 'baals mastery' <axe/flail/mace/spear/sword>\n\r",ch);
		return;
	}

	act("In a terrifying flash of dark insight, your Lord grants you mastery of $ts.",ch,target_name,0,TO_CHAR);

	init_affect(&af);
	af.where		=	TO_AFFECTS;
	af.aftype		=	AFT_INVIS;
	af.type			=	gsn_baals_mastery;
	af.owner		=	ch;
	af.level		=	MAX_LEVEL;
	af.location		=	0;
	af.modifier		=	weapon;
	af.duration		=	-1;
	SET_BIT(af.bitvector, AFF_PERMANENT);
	af.end_fun		=	NULL;
	af.tick_fun		=	NULL;
	affect_to_char(ch,&af);

	return;
}

void check_baals_mastery(CHAR_DATA *ch, CHAR_DATA *victim)
{
	AFFECT_DATA *af, af2;
	OBJ_DATA *weapon;
	int skill = get_skill(ch,gsn_baals_mastery);

	if (skill == 0)
		return;
	
	if (!is_affected(ch,gsn_baals_mastery))
		return;
	
	if (victim != ch->fighting)
		return;

	if (!(af = affect_find(ch->affected,gsn_baals_mastery)))
		return;

	if (!(weapon = get_eq_char(ch,WEAR_WIELD)))
		return;

	if (weapon->value[0] != af->modifier)
		return;
	
	if (skill / 10 <= number_percent())
		return;
	
	switch (af->modifier) {
		case(WEAPON_AXE):
			act("With a rush of unholy might, you hack brutally with your axe at $N's flesh!",ch,0,victim,TO_CHAR);
			act("A wicked grin flashes across $n's face as $s axe rends your flesh!",ch,0,victim,TO_VICT);
			act("A wicked grin flashes across $n's face as $s axe rends $N's flesh!",ch,0,victim,TO_NOTVICT);
			one_hit_new(ch,victim,TYPE_UNDEFINED,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,150,NULL);
			break;
		case(WEAPON_SWORD):
			act("With a surge of unholy vigor, your drive your blade deep into $N!",ch,0,victim,TO_CHAR);
			act("A wicked grin flashes across $n's face as $e plunges $s sword into you!",ch,0,victim,TO_VICT);
			act("A wicked grin flashes across $n's face as $e plunges $s sword into $N!",ch,0,victim,TO_NOTVICT);
			one_hit_new(ch,victim,TYPE_UNDEFINED,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,150,NULL);
			break;
		case(WEAPON_MACE):
			act("With a surge of unholy might, you bring your mace crashing down upon $N!",ch,0,victim,TO_CHAR);
			act("A wicked grin flashes across $n's face as $e brings $s mace down upon you!",ch,0,victim,TO_VICT);
			act("A wicked grin flashes across $n's face as $e brings $s mace down upon $N!",ch,0,victim,TO_NOTVICT);
			one_hit_new(ch,victim,TYPE_UNDEFINED,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,150,NULL);
			break;
		case(WEAPON_SPEAR):
			act("With a surge of unholy might, you thrust your spear through $N's guard!",ch,0,victim,TO_CHAR);
			act("A wicked grin flashes across $n's face as $e drives $s spear into you!",ch,0,victim,TO_VICT);
			act("A wicked grin flashes across $n's face as $e drives $s spear into $N!",ch,0,victim,TO_NOTVICT);
			one_hit_new(ch,victim,TYPE_UNDEFINED,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,200,NULL);
			if (number_percent() < 40 && !is_affected(victim,gsn_bleeding)) {
				act("Your dark heart exults as blood fountains forth from the wound!",ch,0,victim,TO_CHAR);
				act("$n's eyes flicker darkly as blood fountains forth from the wound!",ch,0,victim,TO_ROOM);
				init_affect(&af2);
				af2.where		=	TO_AFFECTS;
				af2.aftype		=	AFT_MALADY;
				af2.type		=	gsn_bleeding;
				af2.owner		=	ch;
				af2.level		=	ch->level;
				af2.location	=	APPLY_STR;
				af2.modifier	=	-4;
				af2.duration	=	6;
				af2.end_fun		=	0;
				af2.tick_fun	=	bleeding_tick;
				affect_to_char(ch,&af2);
			}
			break;
		case(WEAPON_FLAIL):
			act("With a surge of unholy might, you bring your flail crashing down upon $N!",ch,0,victim,TO_CHAR);
			act("A wicked grin flashes across $n's face as $e brings $s flail down upon you!",ch,0,victim,TO_VICT);
			act("A wicked grin flashes across $n's face as $e brings $s flail down upon $N!",ch,0,victim,TO_NOTVICT);
			one_hit_new(ch,victim,TYPE_UNDEFINED,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,175,NULL);
			break;
		default:
			return;
	}

	check_improve(ch,gsn_baals_mastery,TRUE,5);

	return;
}

void spell_word_of_command(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	char arg1[MSL], arg2[MSL], buf[MSL];
	AFFECT_DATA af;
	CHAR_DATA *victim;

	target_name = one_argument(target_name,arg1);
	target_name = one_argument(target_name,arg2);

	if (arg1[0] == '\0' || arg2[0] == '\0' || target_name[0] == '\0') {
		ch->wait = 0;
		ch->mana += 150;
		send_to_char("Syntax: cast 'word of command' <target> <command phrase> <action>\n\r",ch);
		return;
	}

	if ((victim = get_char_room(ch,arg1)) == NULL) {
		ch->wait = 0;
		ch->mana += 150;
		send_to_char("They aren't here.\n\r", ch);
		return;
	}

	if (ch == victim)
		return send_to_char("As persuasive as you are, you're unable to corrupt your own mind.\n\r",ch);
	
	if (IS_IMMORTAL(victim) && !IS_IMMORTAL(ch))
		return send_to_char("Don't even think about it.\n\r",ch);
	
	if (is_safe_new(ch,victim,TRUE))
		return;

	if (IS_NPC(victim))
		return send_to_char("You cannot manipulate such a being's mind.\n\r",ch);

	if (is_affected(ch,gsn_word_of_command))
		return send_to_char("They already have a word of command implanted in their mind.\n\r",ch);
	
	if (IS_IMMORTAL(victim) && get_trust(ch) < get_trust(victim))
		return send_to_char("Don't do that.  No, really.  Don't.\n\r",ch);
	
	if (!str_prefix(target_name,"delete") || !str_prefix(target_name,"quit")
		|| !str_prefix(target_name,"pass") || !str_prefix(target_name,"pray")) {
		send_to_char("What the hell do you think you're doing?\n\r",ch);
		act( "A bolt from the heavens smites you!", ch, 0, 0, TO_CHAR);
		act( "A bolt from the heavens smites $n!", ch, 0, 0, TO_ROOM);
		ch->hit /= 2;
		return;
	}

	if (saves_spell(level + 20 - get_curr_stat(victim,STAT_INT),victim,DAM_CHARM)) {
		act("$N's mind resists your word of command.",ch,0,victim,TO_CHAR);
		act("You fend off a dark mental assault by $n!",ch,0,victim,TO_VICT);
		sprintf(buf,"Die, %s you sorcerous dog!",ch->name);
		do_myell(victim,buf, ch);
		multi_hit(victim,ch,TYPE_UNDEFINED);
		return;
	}

	act("You successfully implant a word of command in $N's malleable mind.",ch,0,victim,TO_CHAR);

	arg2[strlen(arg2)] = '\0';
	
	victim->pcdata->command[0] = palloc_string(arg2);
	victim->pcdata->command[1] = palloc_string(target_name);

	init_affect(&af);
	af.where		=	TO_AFFECTS;
	af.aftype		=	AFT_INVIS;
	af.type			=	gsn_word_of_command;
	af.owner		=	ch;
	af.level		=	level;
	af.location		=	0;
	af.duration		=	-1;
	af.modifier		=	0;
	af.tick_fun		=	0;
	af.end_fun		=	0;
	affect_to_char(victim,&af);
	
	return;
}

void command_execute (CHAR_DATA *ch)
{
	char buf[MSL];
	
	if (!is_affected(ch,gsn_word_of_command))
		return;

	if (ch->position < POS_FIGHTING)
		do_wake(ch, "");

	if (ch->position < POS_FIGHTING)
		return;

	affect_strip(ch,gsn_word_of_command);
	
	sprintf(buf,"An irresistible urge forces you to '%s'.\n\r",ch->pcdata->command[1]);
	send_to_char(buf,ch);
	
	RS.Queue.AddToQueue(1, 2, command_execute_delay, ch, ch->pcdata->command[1]);
	
	WAIT_STATE(ch,PULSE_PER_SECOND + 1);
	
	free_pstring(ch->pcdata->command[0]);

	return;
}

void command_execute_delay(CHAR_DATA *ch, char* command)
{
	interpret(ch,command);
	
	free_pstring(ch->pcdata->command[1]);
	
	return;
}

void spell_mark_of_wrath(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	if (IS_NPC(victim))
		return send_to_char("That foe is not deserving of your Lord's mark.\n\r",ch);

	if (victim == ch)
		return send_to_char("Even great self-loathing has its limits.\n\r",ch);
	
	if (is_affected(ch,gsn_mark_of_wrath))
		return send_to_char("You may only mark one foe at a time.\n\r",ch);

	act("You intone unholy words and a flaming brand appears on $N's forehead!",ch,0,victim,TO_CHAR);
	act("A wave of rage clouds your thoughts as $n chants in an unintelligble tongue.",ch,0,victim,TO_VICT);
	act("A flaming brand appears on $N's forehead, radiating malevolence.",ch,0,victim,TO_NOTVICT);

	init_affect(&af);
	af.where		=	TO_AFFECTS;
	af.aftype		=	AFT_INVIS;
	af.type			=	gsn_mark_of_wrath;
	af.owner		=	victim;
	af.level		=	level;
	if (IS_GOOD(victim))

		af.duration =	(int)(level * 0.8);
	else
		af.duration	=	(int)(level * 0.4);
	af.location		=	0;
	af.modifier		=	0;
	af.tick_fun		=	0;
	af.end_fun		=	0;
	new_affect_to_char(ch,&af);

	return;
}

void spell_living_blade(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	OBJ_DATA *weapon = (OBJ_DATA *) vo;
	OBJ_AFFECT_DATA oaf;

	if (weapon->item_type != ITEM_WEAPON)
		return send_to_char("That is not a weapon!\n\r",ch);

	act("You call upon the power of Baal to imbue $p with a dark vitality.",ch,weapon,0,TO_CHAR);
	act("$p writhes briefly in $n's grasp.",ch,weapon,0,TO_ROOM);

	init_affect_obj(&oaf);
	oaf.where		=	TO_OBJ_APPLY;
	oaf.type		=	gsn_living_blade;
	oaf.aftype		=	AFT_SPELL;
	oaf.owner		=	ch;
	oaf.level		=	level;
	oaf.duration	=	level;
	oaf.location	=	APPLY_DAMROLL;
	oaf.modifier	=	0;
	oaf.end_fun		=	living_blade_end;
	oaf.tick_fun	=	0;
	oaf.pulse_fun	=	living_blade_pulse;
	affect_to_obj(weapon,&oaf);
	oaf.location	=	APPLY_HITROLL;
	affect_to_obj(weapon,&oaf);

	return;
}

/* Note: this is fugly. */

void living_blade_pulse(OBJ_DATA *obj, OBJ_AFFECT_DATA *af)
{
	char buf[MSL];
	CHAR_DATA *ch;
	OBJ_AFFECT_DATA *af2;

	if ((ch = obj->carried_by) == NULL)
		return;

	if (ch != af->owner) {
		if (number_percent() < 7 && af->location == APPLY_DAMROLL)
			switch (number_range(1,3)) {
				case(1):
					act("$p hisses at you: 'Insignificant whelp, you are unworthy of my power!'",ch,obj,0,TO_CHAR);
					break;
				case(2):
					act("$p writhes in your grasp as you hear a faint cry of outrage.",ch,obj,0,TO_CHAR);
					break;
				case(3):
					act("$p whispers menacingly: 'My master will devour your soul, blasphemer.'",ch,obj,0,TO_CHAR);
					break;
			}
		return;
	}

	if (!ch->fighting && af->modifier == 0) {
		if (number_percent() < 4 && af->location == APPLY_DAMROLL) {
			switch (number_range(1,4)) {
				case(1):
					sprintf(buf,"%sBlood... give me blood, %s, lest I feast upon thine....%s\n\r",get_char_color(ch,"red"),ch->name,END_COLOR(ch));
					break;
				case(2):
					sprintf(buf,"%sBaal will be most displeased to learn of your timidity....%s\n\r",get_char_color(ch,"red"),END_COLOR(ch));
					break;
				case(3):
					 sprintf(buf,"%sI was meant to be wielded by hands more powerful than thine, mortal.%s\n\r",get_char_color(ch,"red"),END_COLOR(ch));
					 break;
				case(4):
					 sprintf(buf,"%sFool!  Wield me in battle and I will make thee as a god!%s\n\r",get_char_color(ch,"red"),END_COLOR(ch));
					 break;
			}
			act("$p twists in your grasp, hissing caustically:",ch,obj,0,TO_CHAR);
			send_to_char(buf,ch);
		}
	}

	if (!ch->fighting && number_percent() <= 33) {
		af2 = new_affect_obj();
		af2->where		=	af->where;
		af2->aftype		=	af->aftype;
		af2->type		=	af->type;
		af2->owner		=	af->owner;
		af2->level		=	af->level;
		af2->location	=	af->location;
		af2->modifier	=	af->modifier;
		af2->duration	=	af->duration;
		af2->end_fun	=	af->end_fun;
		af2->pulse_fun	=	af->pulse_fun;
		affect_remove_obj(obj,af,FALSE);
		af2->modifier--;
		af2->modifier = UMAX(0,af2->modifier);
		affect_to_obj(obj,af2);
		return;
	} else if (!ch->fighting)
		return;

	if (!(obj->wear_loc == WEAR_WIELD || obj->wear_loc == WEAR_DUAL_WIELD))
		return;

	if (IS_NPC(ch->fighting) && number_percent() > 5)
		return;

	af2 = new_affect_obj();
	af2->where		=	af->where;
	af2->aftype		=	af->aftype;
	af2->type		=	af->type;
	af2->owner		=	af->owner;
	af2->level		=	af->level;
	af2->location	=	af->location;
	af2->modifier	=	af->modifier;
	af2->duration	=	af->duration;
	af2->end_fun	=	af->end_fun;
	af2->pulse_fun	=	af->pulse_fun;
	affect_remove_obj(obj,af,FALSE);
	
	if (IS_NPC(ch->fighting) && af2->modifier <= 15) {
		af2->modifier++;
		af2->modifier = UMIN(af2->modifier, 15);
	} else if (IS_NPC(ch->fighting)) {
		af2->modifier -= 2;
	} else {
		af2->modifier++;
		af2->modifier = UMIN(af2->modifier,50);
	}

	affect_to_obj(obj,af2);
	
	if (number_percent() < 10 && af2->location == APPLY_DAMROLL) {
		switch (number_range(1,3)) {
			case(1):
				act("A hollow laughter reaches your ears as $p pulses in your grasp.",ch,obj,0,TO_CHAR);
				break;
			case(2):
				act("$p writhes as you faintly hear sounds not unlike cries of carnal ecstasy.",ch,obj,0,TO_CHAR);
				break;
			case(3):
				act("$p cries with glee: 'Yes!  Yes!  Tear their heart out!'",ch,obj,0,TO_CHAR);
				break;
		}
	}
	
	return;
}

void living_blade_end(OBJ_DATA *obj, OBJ_AFFECT_DATA *af)
{
	if (af->location == APPLY_HITROLL)
		return;

	if (!obj->carried_by)
		return;

	act("$p ceases its slight writhing and seems less animated.",
		obj->carried_by,obj,0,TO_CHAR);
	
	return;
}

void traitor_pulse(CHAR_DATA *ch, AFFECT_DATA *af)
{
	if (!af->owner) {
		affect_remove(ch,af);
		return;
	}

	if (!is_same_group(ch,af->owner) && af->duration == -1) {
		af->duration = 10;
		return;
	}

	return;
}

void spell_dark_familiar(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *fam, *check;
	AFFECT_DATA af;
	char buf[MSL];
	int devil = -1;
	int vnum;
	int i;
	float hp_mod = 1;
	bool found = FALSE;

	for(check = char_list; check != NULL; check=check->next) {
		if(IS_NPC(check) && check->leader == ch) {
			found = TRUE;
			break;
		}
	}
	if (is_affected(ch,gsn_dark_familiar) && found)
		return send_to_char("You cannot call another familiar so soon.\n\r",ch);

	for (i = 0; i < MAX_DEVIL; i++)
		if (ch->pcdata->devildata[i] > 0) {
			devil = i;
			break;
		}

	if (devil == -1)
		return send_to_char("You have not been granted the right to call upon an infernal familiar.\n\r",ch);

	switch (devil) {
		case (DEVIL_ASMODEUS):
			if (ch->pcdata->devildata[DEVIL_ASMODEUS] == 1) {
				vnum = MOB_VNUM_HAMATULA_FAM;
				hp_mod = 1.5;
			} else {
				vnum = MOB_VNUM_PITFIEND_FAM;
				hp_mod = 2.5;
			}
			break;
		case (DEVIL_MOLOCH):
			vnum = MOB_VNUM_CORNOGUN_FAM;
			hp_mod = 0.75;
			break;
		case (DEVIL_BAAL):
			vnum = MOB_VNUM_MINOTAUR_FAM;
			hp_mod = 2;
			break;
		case (DEVIL_DISPATER):
			vnum = MOB_VNUM_IMP_FAM;
			hp_mod = 0.667;
			break;
		case (DEVIL_MEPHISTO):
			vnum = MOB_VNUM_GEULGON_FAM;
			hp_mod = 1;
			break;
		default:
			bug ( "dark_familiar: invalid devil", 0);
			return;
	}

	fam = create_mobile(get_mob_index(vnum));

	fam->level 					=	ch->level;
	fam->max_hit				=	(short)(ch->max_hit * hp_mod);
	fam->hit					=	fam->max_hit;
	fam->damage[DICE_TYPE]		=	3;
	fam->damage[DICE_NUMBER]	=	ch->level;
	fam->damage[DICE_BONUS]		=	ch->level / 3;
	if(ch->cabal)
		fam->cabal = ch->cabal;
	add_follower(fam,ch);
	fam->leader = ch;
	SET_BIT(fam->affected_by, AFF_CHARM);
	char_to_room(fam,ch->in_room);

	sprintf(buf,"%sA jagged incarnadine rift opens in mid-air, and $N emerges.%s", get_char_color(ch,"red"), END_COLOR(ch));
	act(buf, ch, 0, fam, TO_ALL);

	init_affect(&af);
	af.where		=	TO_AFFECTS;
	af.type			=	gsn_dark_familiar;
	af.aftype		=	AFT_TIMER;
	af.level		=	ch->level;
	af.owner		=	ch;
	af.duration		=	64;
	affect_to_char(ch,&af);
	
	return;
}

	/* The spell used to begin the demon-summoning rituals for AP favors. */
void spell_unholy_communion(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;

	if (str_cmp(ch->Class()->name,"anti-paladin"))
		return send_to_char("Only anti-paladins may call upon the demonic powers in this manner.\n\r",ch);

	if (is_affected(ch,gsn_unholy_communion))
		return send_to_char("It would be most unwise to disturb the unholy powers again so soon.\n\r",ch);

	if (ch->ghost > 0)
		return send_to_char("Dabbling with such powers while a ghost could easily result in your destruction.\n\r",ch);
	
	if (!IS_SET(ch->in_room->room_flags,ROOM_DARK) && sun != SUN_SET && sun != SUN_DARK)
		return send_to_char("The unholy powers prefer to conduct their affairs under cover of darkness.\n\r",ch);
	
	act("A chill breeze sweeps through the area as the shadows flicker malevolently.",ch,0,0,TO_ROOM);
	act("A sudden chill runs down your spine as you call upon the powers of darkness.",ch,0,0,TO_CHAR);

	init_affect(&af);
	af.where		=	TO_AFFECTS;
	af.type			=	gsn_unholy_communion;
	af.aftype		=	AFT_SPELL;
	af.level		=	level;
	af.owner		=	ch;
	af.duration		=	12;
	af.end_fun		=	communion_end;
	affect_to_char(ch,&af);	

	return;
}

void communion_end (CHAR_DATA *ch, AFFECT_DATA *af)
{
	AFFECT_DATA timer;

	if (af->duration == 0)
		send_to_char("The sense of palpable darkness dissipates as the period of communion ends.\n\r",ch);
	
	init_affect(&timer);
	timer.where		=	TO_AFFECTS;
	timer.type		=	gsn_unholy_communion;
	timer.aftype	=	AFT_TIMER;
	timer.level		=	ch->level;
	timer.owner		=	ch;
	timer.duration	=	96;
	affect_to_char(ch,&timer);

	return;
}

void check_unholy_communion (CHAR_DATA *ch, char *argument)
{
	DESCRIPTOR_DATA *d;
	AFFECT_DATA *af;
	int demon = -1, type = -1;
	int *typeptr = NULL, *demonptr = NULL;
	int i;
	int failed = 0;
	char *said;

	if(IS_NPC(ch))
		return;

	af = affect_find(ch->affected,gsn_unholy_communion);
	if (af->aftype == AFT_TIMER)
		return;
	
	if (str_cmp(ch->Class()->name,"anti-paladin"))
		return;

	said = palloc_string(lowstring(argument));
	
	for (i = 0; i < MAX_GREATER + MAX_LESSER; i++) {
		if (strstr(said,demon_table[i].name)) {
			if (demon_table[i].type == LESSER_DEMON && ch->level >= 25) {
				demon = demon_table[i].number;
				type = LESSER_DEMON;
				break;
			} else if (demon_table[i].type == GREATER_DEMON && ch->level > 34) {
				demon = demon_table[i].number;
				type = GREATER_DEMON;
				break;
			} else {
				continue;
			}
		}
	}

	free_pstring(said);

	if (type == -1)
		return;
	
	if ((!IS_OUTSIDE(ch) && !IS_SET(ch->in_room->room_flags, ROOM_DARK)) || (IS_OUTSIDE(ch) &&
		sun != SUN_DARK))
		return send_to_char("It occurs to you that a well-lit place is perhaps not ideal for this.\n\r",ch);

	for (d = descriptor_list; d; d = d->next)
		if (d->connected == CON_PLAYING
			&& !IS_IMMORTAL(d->character)
			&& !IS_NPC(d->character)
			&& d->character->in_room->area == ch->in_room->area
			&& d->character != ch)
			return send_to_char("A voice hisses in your mind: 'Ssssolitude, mortal.  There are othersss near...'\n\r",ch);
	
	if (type == LESSER_DEMON) {
		for (i = 0; i < MAX_LESSER; i++) {
			if (ch->pcdata->lesserdata[i] == FAVOR_FAILED)
				failed++;
	/*
			if (ch->pcdata->lesserdata[i] == FAVOR_IN_PROGRESS)
				return send_to_char("You have unresolved dealings with a lesser demon already!\n\r",ch);
	*/	
			if (ch->pcdata->lesserdata[i] >= FAVOR_GRANTED)
				return send_to_char("You have already received the favor of a lesser demon.\n\rFurther summons will not be answered.\n\r",ch);
		}
		if (failed >= 3 || ch->pcdata->lesserdata[demon] == FAVOR_FAILED)
			return send_to_char("A voice whispers in your mind: 'You are not worth our time, mortal....'\n\r",ch);
		ch->pcdata->lesserdata[demon] = FAVOR_IN_PROGRESS;
	}
	failed = 0;
	if (type == GREATER_DEMON) {
		for (i = 0; i < MAX_GREATER; i++) {
			if (ch->pcdata->greaterdata[i] == FAVOR_FAILED)
				failed++;
			if(failed >= 3 || ch->pcdata->greaterdata[demon] == FAVOR_FAILED)
				return send_to_char("A voice whispers in your mind: 'You are not worth our time, mortal....'\n\r",ch);
	/*
			if (ch->pcdata->greaterdata[i] == FAVOR_IN_PROGRESS)
				return send_to_char("You have unresolved dealings with a greater demon already!\n\r",ch);
	*/
			if (ch->pcdata->greaterdata[i] >= FAVOR_GRANTED)
				return send_to_char("You have already received the favor of a greater demon.\n\rFurther summons will not be answered.\n\r",ch);
		}
		ch->pcdata->greaterdata[demon] = FAVOR_IN_PROGRESS;
	}

	WAIT_STATE(ch,5*PULSE_VIOLENCE);

	act("As the name of the $t demon escapes your lips, the shadows writhe violently.",ch,(type == LESSER_DEMON) ? "lesser" : "greater", 0, TO_CHAR);

	RS.Queue.AddToQueue(2, 5, act, "A sonorous throbbing fills your surroundings, and then all is deathly silent.", ch, NULL, NULL, TO_ALL);

	affect_remove (ch, af);

	typeptr = (int *)talloc_struct(sizeof(int));
	demonptr = (int *)talloc_struct(sizeof(int));
	
	*typeptr = type;
	*demonptr = demon;
	
	RS.Queue.AddToQueue(3, 3, demon_appear, ch, demonptr, typeptr);
	
	return;
}

void demon_appear (CHAR_DATA *ch, int *demonptr, int *typeptr)
{
	char buf[MSL];
	AFFECT_DATA af;
	int vnum = -1;
	int demon = *demonptr, type = *typeptr;
	CHAR_DATA *mob;
	
	if (type == LESSER_DEMON)
		switch (demon) {
			case(LESSER_BARBAS):
				vnum = MOB_VNUM_BARBAS;
				act("Frothing at the mouth, a livid lesser demon materializes before you!",ch,0,0,TO_ALL);
				break;
			case(LESSER_AAMON):
				vnum = MOB_VNUM_AAMON;
				act("With a peal of laughter like the sound of a dying goat, a lesser demon appears!",ch,0,0,TO_ALL);
				break;
			case(LESSER_MALAPHAR):
				vnum = MOB_VNUM_MALAPHAR;
				act("A well-dressed man with a wicked twinkle in his eye steps from the shadows!",ch,0,0,TO_ALL);
				break;
			case(LESSER_FURCAS):
				vnum = MOB_VNUM_FURCAS;
				act("A shivering lesser demon materializes before you, keeping to the shadows.",ch,0,0,TO_ALL);
				break;
			case(LESSER_IPOS):
				act("With a soft 'pop' and a flash of subdued reddish light, a horned demon appears!",ch,0,0,TO_ALL);
				vnum = MOB_VNUM_IPOS;
				break;
			default:
				bug( "demon_appear: invalid lesser", 0);
				return;
		}

	if (type == GREATER_DEMON)
		switch (demon) {
			case(GREATER_OZE):
				act("With a great sucking sound and a fine mist of blood, a fearsome demon appears!",ch,0,0,TO_ALL);
				vnum = MOB_VNUM_OZE;
				break;
			case(GREATER_GAMYGYN):
				act("A deceptively mellifluous chorus heralds the arrival of a massive darkened form!",ch,0,0,TO_ALL);
				vnum = MOB_VNUM_GAMYGYN;
				break;
			case(GREATER_OROBAS):
				act("In a flurry of blurred hands, feet, heads and hair, a gruesome demon appears!",ch,0,0,TO_ALL);
				vnum = MOB_VNUM_OROBAS;
				break;
			case(GREATER_GERYON):
				act("A swirl of mist rises and falls, revealing a terrible demon in its place!",ch,0,0,TO_ALL);
				vnum = MOB_VNUM_GERYON;
				break;
			case(GREATER_CIMERIES):
				act("A huge demon materializes in mid-air and crashes unceremoniously to the ground.",ch,0,0,TO_ALL);
				vnum = MOB_VNUM_CIMERIES;
				break;
			default:
				bug( "demon_appear: invalid greater", 0);
				return;
		}

	if (vnum == -1)
		return;
	
	mob = create_mobile(get_mob_index(vnum));
	char_to_room(mob,ch->in_room);

	if (type == LESSER_DEMON) {
		init_affect(&af);
		af.where		=	TO_AFFECTS;
		af.aftype		=	AFT_INVIS;
		af.type			=	gsn_lesser_demon;
		af.owner		=	ch;
		af.level		=	60;
		af.tick_fun		=	lesser_demon_tick;

		switch (demon) {
			case(LESSER_BARBAS):
				RS.Queue.AddToQueue(2, 2, do_emote, mob, (char*)"roars with anger and beats his meaty fists against his chest!");
				RS.Queue.AddToQueue(4, 5, act, "Still growling, $N peers toward you with a sneering rictus of despite.", ch, 0, mob, TO_CHAR);
				RS.Queue.AddToQueue(6, 2, do_say,mob, (char*)"Summon me,  you fool?  Summon ME!?");
				RS.Queue.AddToQueue(8, 2, do_say, mob, (char*)"I'll bet me reputation an' a slice o' me power that I'm about ta beat you senseless!");
				RS.Queue.AddToQueue(10, 2, do_murder, mob, ch->name);
				RS.Queue.AddToQueue(10, 5, act, "With a feral leap and a scream, $N is suddenly upon you!", ch, 0, mob, TO_CHAR);
				af.duration		=	-1;
				affect_to_char(mob,&af);
				break;
			case(LESSER_AAMON):
				WAIT_STATE(ch,5*PULSE_VIOLENCE);
				RS.Queue.AddToQueue(2, 2, do_emote,mob, (char*)"twitches violently as he stares around,  bewildered.");
				RS.Queue.AddToQueue(4, 2, do_whisper,mob, (char*)"And where am I now,  precisely?");
				RS.Queue.AddToQueue(6, 5, act, "Jerking suddenly as if to watch in all directions at once, the demon spins.", mob, 0, 0, TO_ROOM);
				RS.Queue.AddToQueue(8, 2, do_say,mob, (char*)"You've summoned me!  And now you will answer my riddle,  yes!");
				RS.Queue.AddToQueue(10, 2, do_emote, mob, (char*)"clears his throat and glances around pensively.");
				RS.Queue.AddToQueue(12, 2, do_say,mob, (char*)"Most often by hoes and by gardeners I'm chased, ");
				RS.Queue.AddToQueue(13, 2, do_say, mob, (char*)"They cut off my head and then smash it to paste;");
				RS.Queue.AddToQueue(14, 2, do_say,mob,(char*)"Against them, however,  my body is braced, ");
				RS.Queue.AddToQueue(15, 2, do_say, mob, (char*)"For always I'm growing due south of the waist.");
				RS.Queue.AddToQueue(16, 2, do_say, mob, (char*)"Now then... what am I?");
				af.duration	 	=	8;
				af.modifier		=	0;
				affect_to_char(mob,&af);
				break;
			case(LESSER_MALAPHAR):
				RS.Queue.AddToQueue(2, 2, do_emote, mob, (char*)"'s eyes light up as he realizes where he is.");
				RS.Queue.AddToQueue(4, 2, do_say,mob,(char*)"My friend!  My friend,  my patron,  my emptor!");
				RS.Queue.AddToQueue(6, 5, act, "Malaphar sidles up to you, and with a conspiratorial wink, waves his hand.", ch, 0, 0, TO_CHAR);
				RS.Queue.AddToQueue(8, 2, do_whisper, mob, (char*)"I have something... something in which you would most undoubtedly be interested.");
				RS.Queue.AddToQueue(10, 2, do_say,mob,(char*)"Sale or trade only,  I'm afraid,  no credit.  And don't waste my time offering me paltry trifles.");
				af.duration 	=	7;
				affect_to_char(mob,&af);
				break;
			case(LESSER_FURCAS):
				RS.Queue.AddToQueue(2, 2, do_emote,mob, (char*)"appears shocked,  and he winces away from you automatically.");
				RS.Queue.AddToQueue(4, 5, act, "Pawing his way around you, viewing you from all angles, Furcas blinks.", ch, 0, 0, TO_CHAR);
				RS.Queue.AddToQueue(6, 2, do_whisper, mob, (char*)"...can it find us?  We think it cannot.  We hope it tries its hardest.");
				RS.Queue.AddToQueue(8, 5, act, "Suddenly, you blink and Furcas has vanished, leaving a cold sniff in the air!", mob, 0, 0, TO_ROOM);
				RS.Queue.AddToQueue(9, 2, furcas_vanish, ch, mob);

				af.duration		=	24;
				affect_to_char(mob,&af);
				break;
			case(LESSER_IPOS):
				RS.Queue.AddToQueue(2, 5, act, "$n, still steaming, looks around him with a small smile on his face.", mob, 0, 0, TO_ROOM);
				RS.Queue.AddToQueue(4, 5, act, "His gaze settles upon you, and his smile broadens to a genuine grin.", ch, 0, 0, TO_CHAR);
				RS.Queue.AddToQueue(6, 2, do_say,mob, (char*)"Greetings my rageful,  irascible friend!  The time for your fury has come to an end.");
				RS.Queue.AddToQueue(7, 2, do_say,mob,(char*)"For a moment at least,  for you see,  I am weary of warriors bleak and dark paladins dreary.");
				RS.Queue.AddToQueue(8, 2, do_say,mob,(char*)"Instead,  it's my wish as I put pen to paper,  that you'll indulge me with a frolicsome caper.");
				RS.Queue.AddToQueue(9, 2, do_say,mob, (char*)"Be mindful,  my ears are accustomed to rhymes; I hope you are willing to speak me four lines....");
				RS.Queue.AddToQueue(10, 2, do_emote,mob, (char*)"thrusts forward,  anticipating your poem with an evil gleam in his eye.");
				af.modifier 	=	0;
				af.duration		=	8;
				affect_to_char(mob,&af);
				break;
			default:
				bug( "demon_appear: invalid lesser", 0);
				return;
		}
	}
			
	if (type == GREATER_DEMON) {
		init_affect(&af);
		af.where		=	TO_AFFECTS;
		af.aftype		=	AFT_INVIS;
		af.type			=	gsn_greater_demon;
		af.owner		=	ch;
		af.level		=	60;
		af.tick_fun		=	greater_demon_tick;
		switch (demon) {
			case(GREATER_OZE):
				RS.Queue.AddToQueue(2, 2, do_emote,mob, (char*)"nearly collapses as he doubles over in sudden agony,  howling pitifully.");
				RS.Queue.AddToQueue(3, 2, do_say, mob, (char*)"What have you done...?  What have you DONE?");
				RS.Queue.AddToQueue(4, 5, act, "Blubbering like a child, $n slowly composes $mself and rises to $s \"feet.\"", mob, 0, 0, TO_ROOM);
				RS.Queue.AddToQueue(6, 5, act, "$e points a hideously deformed arm toward you, dripping blood and bits of sinew.", mob, 0, 0, TO_ROOM);
				RS.Queue.AddToQueue(8, 2, do_say,mob,(char*)"I charge you,  mortal,  with returning me to the abyss.  I have been plucked from a place... a place from whence I should not have been plucked.");
				RS.Queue.AddToQueue(10, 2, do_emote, mob, (char*)"shudders slightly before continuing.");
				RS.Queue.AddToQueue(11, 2, do_say,mob,(char*)"I require your blood, meddler,  in order that I might survive long enough to return....  Speak the words,  whelp.  Yes or no?");
				af.duration		=	6;
				affect_to_char(mob,&af);
				break;
			case(GREATER_GAMYGYN):
				RS.Queue.AddToQueue(2, 5, act, "The undulating, shimmering form before you rings with unearthly sound.", mob, 0, 0, TO_ROOM);
				RS.Queue.AddToQueue(4, 5, act, "Swooping toward your face, $n seems to peer directly into your eyes.", mob, 0, 0, TO_ROOM);
				RS.Queue.AddToQueue(6, 2, do_tell, mob, buf);
				RS.Queue.AddToQueue(6, 3, sprintf,buf, (char*)"%s I see you,  mortal.", ch->name);
				RS.Queue.AddToQueue(6, 5, act, "A sudden voice echoes unsettlingly in your mind:", mob, 0, ch, TO_VICT);
				RS.Queue.AddToQueue(7, 5, act, "You wince involuntarily, pictures of transactions gone wrong in your mind's eye.", mob, 0, ch, TO_VICT);
				RS.Queue.AddToQueue(9, 2, do_tell, mob, buf);
				RS.Queue.AddToQueue(9, 3, sprintf, buf, (char*)"%s I will not devour you; you may yet entertain me.  Rewards shall be yours if you provide me with the dead flesh of one of your aberrant lightwalking kin.  Do you submit yourself to my desire?", ch->name);
				af.duration		=	5;
				affect_to_char(mob,&af);
				break;
			case(GREATER_OROBAS):
				RS.Queue.AddToQueue(2, 5, act, "Orobas' many faces stare around the area, nonplussed by your summoning.", mob, 0, ch, TO_VICT);
				RS.Queue.AddToQueue(4, 2, do_say, mob, (char*)"I see.");
				RS.Queue.AddToQueue(6, 5, act, "A great howling sound boils into existence as Orobas' limbs flail wildly!", mob, 0, 0, TO_ROOM);
				RS.Queue.AddToQueue(8, 2, do_say, mob, (char*)"I am displeased to be here.");
				RS.Queue.AddToQueue(9, 2, do_say,mob,(char*)"You will do my bidding,  mortal,  or I shall inform the abyss of your grievous error at once.");
				RS.Queue.AddToQueue(10, 5, act, "The choir of blurred and screaming heads reaches a new pitch of fervor.", mob, 0, 0, TO_ALL);
				RS.Queue.AddToQueue(12, 2, do_whisper,mob, (char*)"Speak now,  or forever mourn your decision.  Do you submit?");
				af.duration		=	5;
				affect_to_char(mob,&af);
				break;
			case(GREATER_GERYON):
				RS.Queue.AddToQueue(2, 2, do_emote,mob,(char*)"smiles pleasantly,  extending a cool,  dry hand to shake your own.");
				RS.Queue.AddToQueue(4, 2, do_say,mob,(char*)"It's wonderful to be back here,  I must say.  It's been far too long.  Who was the last fellow?  Ah,  yes....");
				RS.Queue.AddToQueue(5, 2, do_emote,mob, (char*)"seems to finger something in his pocket,  his eyes alight with pleasure.");
				RS.Queue.AddToQueue(6, 5, act, "An uncomfortable sensation crawls over you as you eye this well-dressed demon.", mob, 0, ch, TO_VICT);
				RS.Queue.AddToQueue(8, 2, do_say,mob,(char*)"But enough of my reminiscing!  I imagine you have something you wanted to discuss,  yes?  Power,  no doubt!");
				RS.Queue.AddToQueue(10, 2, do_emote,mob, (char*)"'s smile expands to a wider grin,  and his eyes flicker red for a moment.");
				RS.Queue.AddToQueue(11, 5, act, "...though it may have been a trick of the light.", mob, 0, 0, TO_ROOM);
				RS.Queue.AddToQueue(12, 2, do_say,mob,(char*)"So,  to business.  What shall it be,  then?  An eye or a finger?");
				af.duration 	=	5;
				affect_to_char(mob,&af);
				break;
			case(GREATER_CIMERIES):
				WAIT_STATE(ch, 5*PULSE_VIOLENCE);
				RS.Queue.AddToQueue(2, 2, do_emote,mob, (char*)"rises to his feet,  and in a sudden panic attempts to flee by air.");
				RS.Queue.AddToQueue(3, 5, act, "The greater demon's pathetic wings, completely impotent, flap helplessly.", mob, 0, 0, TO_ROOM);
				RS.Queue.AddToQueue(4, 5, act, "You stave off a pang of guilt at having summoned this massive beast to flounder.", mob, 0, ch, TO_VICT);
				RS.Queue.AddToQueue(6, 5, act, "Your misplaced guilt quickly turns to fear as Cimeries rounds on you savagely!", mob, 0, ch, TO_VICT);
				RS.Queue.AddToQueue(7, 5, act, "Before you can react, Cimeries has clamped your head between two massive claws!", mob, 0, ch, TO_VICT);
				RS.Queue.AddToQueue(9, 5, act, "You gasp in pain, blood running down your face, as his talons pierce your nose and ear!", mob, 0, ch, TO_VICT);
				RS.Queue.AddToQueue(10, 5, act, "$n speaks in a low grunting language, and you cannot understand a word.", mob, 0, 0, TO_ROOM);
				RS.Queue.AddToQueue(11, 5, act, "$n tugs harshly on your ear and nose in turn, eliciting another gasp of pain.", mob, 0, ch, TO_VICT);
				RS.Queue.AddToQueue(13, 5, act, "Suddenly, his cruel choice for you becomes clear: ear or nose?", mob, 0, ch, TO_VICT);
				RS.Queue.AddToQueue(15, 5, act, "A shiver runs down your spine while you contemplate your fate.", mob, 0, ch, TO_VICT);
				af.duration = 4;
				affect_to_char(mob,&af);
				break;
			default:
				bug( "demon_appear: invalid greater", 0);
				return;
		}
	}
	
	return;
}

void lesser_demon_tick (CHAR_DATA *mob, AFFECT_DATA *af)
{
	char buf[MSL];
	
	if (!IS_NPC(mob) || !af->owner)
		return;

	switch(mob->pIndexData->vnum) {
		case(MOB_VNUM_BARBAS):
			break;
		case(MOB_VNUM_FURCAS):
			if (af->owner && af->duration == 12) {
				sprintf (buf,"%s Does it want a hint?  Does it?  We might not be in the same area as we were before, we might not... but always somewhere near, yes....",af->owner->name);
				do_tell(mob,buf);
				break;
			}
			if (af->owner && af->duration == 1) {
				sprintf (buf,"%s We give up... it cannnot find us... a pity, yes.  Goodbye, it.",af->owner->name);
				do_tell(mob,buf);
				act("$n vanishes in a crimson flash!",mob,0,0,TO_ROOM);
				af->owner->pcdata->lesserdata[LESSER_FURCAS] = FAVOR_FAILED;
				extract_char(mob,TRUE);
			}
			break;
		case(MOB_VNUM_AAMON):
			if (af->duration == 4) {
				do_emote(mob,"taps his foot irritably.");
				break;
			}
			if (af->duration == 1) {
				do_emote(mob,"laughs giddily and whirls to leave.");
				RS.Queue.AddToQueue(1, 2, do_say,mob, (char*)"I'll be leaving now,  and leaving you to your thoughts.  You'll never know the answer to my riddle.");
				RS.Queue.AddToQueue(2, 5, act, "With a puff of hazy purple smoke and a sound like a cough, $n disappears.", mob, 0, 0, TO_ROOM);
				RS.Queue.AddToQueue(3, 1, delay_extract, mob);
				break;
			}
		case(MOB_VNUM_MALAPHAR):
			if (af->duration % 2 == 0) {
				do_emote(mob,"taps his foot impatiently.");
				break;
			}
			if (af->duration == 1) {
				do_say(mob,"My friend, I have better things to do with my time.  A pity.");
				act("$n vanishes in a crimson flash!",mob,0,0,TO_ROOM);
				af->owner->pcdata->lesserdata[LESSER_MALAPHAR] = FAVOR_FAILED;
				extract_char(mob,TRUE);
			}
			break;
		case(MOB_VNUM_IPOS):
			if (af->duration == 1) {
				do_say(mob,"I've better things to do with my time, than wait for you to mumble a rhyme!");
				act("$n vanishes in a crimson flash!",mob,0,0,TO_ROOM);
				af->owner->pcdata->lesserdata[LESSER_IPOS] = FAVOR_FAILED;
				extract_char(mob,TRUE);
			}
			break;
		default:
			return;
	}

	return;
}

void greater_demon_tick (CHAR_DATA *mob, AFFECT_DATA *af)
{
	char buf[MSL];

	if (!IS_NPC(mob) || !af->owner)
		return;

	switch(mob->pIndexData->vnum) {
		case(MOB_VNUM_OZE):
			if (af->duration == 5) {
				do_emote(mob,"gives a convulsive shiver and seems to slide toward the ground.");
				break;
			}
			if (af->duration == 3) {
				act("Crying out suddenly, Oze begins to liquefy, giving off a hideous odor.",mob,0,0,TO_ROOM);
				break;
			}
			if (af->duration == 1) {
				sprintf(buf,"The abyss will not forget this treachery, %s.",af->owner->name);
				do_whisper(mob,buf);
				act("The puddle of gore before you which was once a greater demon seeps downward.",mob,0,0,TO_ROOM);
				af->owner->pcdata->greaterdata[GREATER_OZE] = FAVOR_FAILED;
                extract_char(mob,TRUE);
				break;
			}
			break;
		case(MOB_VNUM_GAMYGYN):
			if (af->duration == 1) {
				sprintf(buf,"%s You fool.  You utter fool.  You know not what power you have squandered.",af->owner->name);
				do_tell(mob,buf);
				act("Flashing a great dark light, $n vanishes from sight.",mob,0,0,TO_ROOM);
				extract_char(mob,TRUE);
				break;
			}
			break;
		case(MOB_VNUM_OROBAS):
			if (af->duration == 1) {
				sprintf(buf,"%s, you fool.  You utter fool.  You know not what power you have squandered!",af->owner->name);
				do_say(mob,buf);
				act("With a fierce snarl, and many assorted whispers, Orobas streaks into the sky.",mob,0,0,TO_ROOM);
				extract_char(mob,TRUE);
				break;
			}
			break;
		case(MOB_VNUM_GERYON):
			if (af->duration == 4) {
				do_say(mob,"It must be one or the other, friend.  An eye, or a finger.");
				do_emote(mob,"smiles irritably.");
				break;
			}
			if (af->duration == 3) {
				act("Exploding suddenly, $n's face darkens and his eyes seethe an angry red!",mob,0,0,TO_ALL);
				do_yell(mob,"WHY, BY THE FIVE DEVILS, MUST I BE SADDLED WITH SUCH INCOMPETENCE?!");
				do_emote(mob,"draws his howling visage close to your own, a mask of impenetrable hate.");
				act("Very suddenly, the greater demon composes himself and smiles firmly once more.",mob,0,0,TO_ALL);
				break;
			}

			if (af->duration == 2) {
				do_emote(mob,"'s eyes flash with anger.");
				do_say(mob,"EYE or FINGER, mortal, it's really very simple!");
				break;
			}
			if (af->duration == 1) {
				do_say(mob,"Very well, fool.  The lords of the abyss will hear of this.");
				act("Turning coldly and melting into a cool mist, $n wafts away on a breeze.",mob,0,0,TO_ALL);
				af->owner->pcdata->greaterdata[GREATER_GERYON] = FAVOR_FAILED;
				extract_char(mob,TRUE);
				break;
			}
			break;
		case(MOB_VNUM_CIMERIES):
			if (af->duration == 3) {
				do_emote(mob,", growing impatient, plucks at your ear and nose once more.");
				break;
			}
			if (af->duration == 2) {
				act("As blood trickles down your neck, your mind races to decide:  ear or nose?",mob,0,af->owner,TO_VICT);
				break;
			}
			if (af->duration == 1) {
				act("In disgust, $n rips his claws free and moves his massive frame back.",mob,0,0,TO_ROOM);
				act("Slowly, the monstrous demon fades into shadows and dissipates.",mob,0,0,TO_ROOM);
				af->owner->pcdata->greaterdata[GREATER_CIMERIES] = FAVOR_FAILED;
				extract_char(mob,TRUE);
			}
			break;
		default:
			return;
	}
		
	return;
}

void furcas_vanish (CHAR_DATA *ch, CHAR_DATA *mob)
{
	char buf[MSL];
	ROOM_INDEX_DATA *pRoomIndex;
	long nocrash;
	
	for (nocrash = 1;nocrash < 1000000 ;nocrash++) {
		pRoomIndex = get_random_room(mob);

		if(nocrash > 100000)
			bug("Furcas bug!",0);
		
		if (!IS_EXPLORE(pRoomIndex)
			&& !pRoomIndex->cabal
			&& pRoomIndex->area->area_type != ARE_UNOPENED
			&& pRoomIndex->area->area_type != ARE_SHRINE
			&& !IS_SET(pRoomIndex->room_flags, ROOM_NO_SUMMON_FROM)
			&& !IS_SET(pRoomIndex->room_flags, ROOM_NO_SUMMON_TO)
			&& !IS_SET(pRoomIndex->room_flags, ROOM_NO_GATE)
			&& (is_adjacent_area(ch->in_room->area,
				pRoomIndex->area)
			|| (ch->in_room->area == pRoomIndex->area)))
			break;
	}

	char_from_room(mob);
	char_to_room(mob,pRoomIndex);
	
	return;
}


void spell_insanity(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;

	act("You feel a rush of dark power take over your body!",ch,0,0,TO_CHAR);
	
	init_affect(&af);
	af.where = TO_AFFECTS;
	af.level = level;
	af.type = sn;
	af.aftype = AFT_SPELL;
	af.duration = 3;
	af.modifier = level/2;
	af.location = APPLY_DAMROLL;
	af.pulse_fun = insanity_pulse;
	af.end_fun = insanity_end;
	af.owner = ch;
	affect_to_char(ch,&af);
	af.pulse_fun = NULL;
	af.end_fun = NULL;
	af.location = APPLY_HITROLL;
	affect_to_char(ch,&af);

	return;
}

void insanity_pulse(CHAR_DATA *ch, AFFECT_DATA *af)
{
	CHAR_DATA *victim;
	int room;
	room = number_range(0,5);
	
	if(number_percent() < 5)
		return;
	if(ch->fighting != NULL) {
		insanity_fight(ch);
		return;	
	}  else if(!IS_IMMORTAL(ch)) {
		WAIT_STATE(ch,4*PULSE_PER_SECOND);
	}
	

	for(victim = ch->in_room->people; victim != NULL; victim = victim->next_in_room) {
		if(ch->fighting != NULL)
			return;
		
		if((victim == ch) ||
		(is_safe_new(ch,victim,FALSE)) ||
		(number_percent() > 95)) {
			insanity_two(ch,room);
			break;
		}
		
		switch(number_range(1,5)) {
			
			case(1):
				act("With a wild look in $s eyes, $n charges into battle!",ch,0,0,TO_ROOM);
				act("A touch of froth escapes your lips as you charge into $N.",ch,0,victim,TO_CHAR);
				do_bash(ch,victim->name);
				break;
			case(2):
				act("With a wild look in $s eyes, $n charges into battle!",ch,0,0,TO_ROOM);
				act("A touch of froth escapes your lips as you charge into $N.",ch,0,victim,TO_CHAR);
				do_murder(ch,victim->name);
				break;
			case(3):
				act("$n flashes a quick grin, $s eyes gleaming with a dark madness.",ch,0,0,TO_ROOM);
				act("Coalesced energy erupts from $n's body, covering the area in flames.",ch,0,0,TO_ROOM);
				act("You utter arcane words as a ball of fire erupts from your body.",ch,0,0,TO_CHAR);
				spell_fireball(skill_lookup("fireball"),ch->level,ch,NULL,TAR_IGNORE);
				break;
			case(4):
				insanity_two(ch,room);
				break;
			case(5):
				act("With a wild look in $s eyes, $n charges into battle!",ch,0,0,TO_ROOM);
				act("A touch of froth escapes your lips as you charge into $N.",ch,0,victim,TO_CHAR);
				do_cleave(ch,victim->name);
				break;
			}
		break;
	}
}

void insanity_two(CHAR_DATA *ch, int room)
{
	ROOM_INDEX_DATA *in_room = ch->in_room;

	if (!in_room)
		return;
	
	switch(room) {
		case(DIR_NORTH):
			if(in_room->exit[DIR_NORTH] == NULL) {
				room = number_range(0,5);
				insanity_two(ch,room);
				return;
			} else {
				do_north(ch,"");
				break;
			}
		
		case(DIR_EAST):
			if(in_room->exit[room] == NULL) {
				room = number_range(0,5);
				insanity_two(ch,room);
				return;
			} else {
				do_east(ch,"");
				break;
			}
		case(DIR_SOUTH):
			if(in_room->exit[room] == NULL) {
				room = number_range(0,5);
				insanity_two(ch,room);
				return;
			} else {
				do_south(ch,"");
				break;
			}
		case(DIR_WEST):
			if(in_room->exit[room] == NULL) {
				room = number_range(0,5);
				insanity_two(ch,room);
				return;
			} else {
				do_west(ch,"");
				break;
			}
		case(DIR_UP):
			if(in_room->exit[room] == NULL) {
				room = number_range(0,5);
				insanity_two(ch,room);
				return;
			} else {
				do_up(ch,"");
				break;
			}
		case(DIR_DOWN):
			if(in_room->exit[room] == NULL) {
				room = number_range(0,5);
				insanity_two(ch,room);
				return;
			} else {
				do_down(ch,"");
				break;
			}
	}
	return;
}

void insanity_end(CHAR_DATA *ch, AFFECT_DATA *af)
{
	return send_to_char("Your sanity returns as the dark magic relinquishes control over your body.\n\r",ch);
}

void insanity_fight(CHAR_DATA *ch)
{
	CHAR_DATA *victim = ch->fighting;

	if(victim == NULL)
		return;

	if(number_percent() < 50)
		return;	

	switch(number_range(1,5)) {
		case(1):
			if (get_eq_char(ch,WEAR_WIELD) == NULL)
				break;
			act("$n brutally drives $s weapon into your chest!",ch,0,victim,TO_VICT);
			act("You brutally drive your weapon into $N's chest!",ch,0,victim,TO_CHAR);
			act("$n brutally drives $s weapon into $N's chest!",ch,0,victim,TO_NOTVICT);
			one_hit_new(ch,victim,TYPE_UNDEFINED,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,125,"crushing assault");
			if(number_percent() > 25) {
				act("Your head hits the ground as $n sweeps your legs from under you.",ch,0,victim,TO_VICT);
				act("As $N recovers from your attack, you swiftly knock $S legs out from under $M.",ch,0,victim,TO_CHAR);			
				act("As $N sways under the force of $n's attack, $n knocks $S legs out from under $M.",ch,0,victim,TO_NOTVICT);
				LAG_CHAR(victim,(int)(1.5*PULSE_VIOLENCE));
			}
			WAIT_STATE(ch,PULSE_VIOLENCE);
			break;
		case(2):
			do_bash(ch,victim->name);
			break;
		case(3):
			act("$n grunts and growls unintelligible words.",ch,0,0,TO_ROOM);
			act("You grunt and growl unintelligible words.",ch,0,0,TO_CHAR);
			break;
		case(4):
			do_trip(ch,victim->name);
			break;
		case(5):
			act("$n screams arcane words of power in a fit of rage.",ch,0,0,TO_ROOM);
			act("As your insanity overtakes you, you scream arcane words in rage.",ch,0,0,TO_CHAR);
			switch(number_range(0,3)) {
				case(0):
					spell_curse(skill_lookup("curse"),ch->level,ch,victim,TAR_CHAR_OFFENSIVE);
					break;
				case(1):
					spell_blindness(skill_lookup("blindness"),ch->level,ch,victim,TAR_CHAR_OFFENSIVE);
					break;
				case(2):
					spell_energy_drain(skill_lookup("energy drain"),ch->level,ch,victim,TAR_CHAR_OFFENSIVE);
					break;
				case(3):
					spell_iceball(skill_lookup("iceball"),ch->level,ch,NULL,TAR_IGNORE);
					break;
			}
			act("As $n finishes $s incantation, $e charges you!",ch,0,victim,TO_VICT);
			act("As $n finishes $s incantation, $e charges $N!",ch,0,victim,TO_NOTVICT);
			act("Finishing your spell, you charge $N!",ch,0,victim,TO_CHAR);
			do_bash(ch,victim->name);
			break;
		}

	return;
}

void gamygyn_tick (CHAR_DATA *ch, AFFECT_DATA *af)
{
	char buf[MSL];
	ROOM_INDEX_DATA *room = ch->in_room;
	OBJ_DATA *corpse;
	
	if (af->duration % 24 == 0) {
		sprintf(buf,"%sA dark soul writhes within you, discomfited, and you remember your bargain.%s\n\r",get_char_color(ch,"blue"),END_COLOR(ch));
		send_to_char(buf,ch);
	}

	if (af->duration == 1) {
		WAIT_STATE(ch,PULSE_VIOLENCE * 2);
		act("You feel a sudden wrenching pain from within, and collapse in agony.",ch,0,0,TO_CHAR);
		act("$n's face twists in sudden agony as $e collapses to the ground, writhing.",ch,0,0,TO_ROOM);
		act("Pure horror sweeps over you as you recall your bargain with the fiend Gamygyn!",ch,0,0,TO_CHAR);
		act("A razor-clawed hand bursts from your belly as the world fades to black....",ch,0,0,TO_CHAR);
		act("Blood fountains from $n's body as a clawed hand emerges from $s belly!",ch,0,0,TO_ROOM);
		act("A dripping demon emerges from the mangled remains and vanishes into the earth.",ch,0,0,TO_ROOM);
		
		raw_kill(ch,ch);
		
		for (corpse = room->contents; corpse; corpse = corpse->next_content)
			if (corpse->item_type == ITEM_CORPSE_PC)
				break;
		if (corpse && corpse->item_type == ITEM_CORPSE_PC)
			extract_obj(corpse);

		sprintf(buf,"%sMortal....  I was made a promise of flesh.  I claim thine....%s\n\r",get_char_color(ch,"red"),END_COLOR(ch));
		send_to_char(buf,ch);
		ch->pcdata->greaterdata[GREATER_GAMYGYN] = FAVOR_FAILED;
	}
		
	return;
}

void orobas_tick(CHAR_DATA *ch, AFFECT_DATA *af)
{
	char buf[MSL];
	ROOM_INDEX_DATA *room = ch->in_room;
	OBJ_DATA *corpse;
	
	if (af->duration % 24 == 0) {
		sprintf(buf,"%sA dark soul writhes within you, discomfited, and you remember your bargain.%s\n\r",get_char_color(ch,"red"),END_COLOR(ch));
		send_to_char(buf,ch);
	}

	if (af->duration == 1) {
		act("You feel a sudden wrenching pain from within, and collapse in agony.",ch,0,0,TO_CHAR);
		act("$n's face twists in sudden agony as $e collapses to the ground, writhing.",ch,0,0,TO_ROOM);
		act("Pure horror sweeps over you as you recall your bargain with the fiend Orobas!",ch,0,0,TO_CHAR);
		act("A razor-clawed hand bursts from your belly as the world fades to black....",ch,0,0,TO_CHAR);
		act("Blood fountains from $n's body as a clawed hand emerges from $s belly!",ch,0,0,TO_ROOM);
		act("A dripping demon emerges from the mangled remains and vanishes into the earth.",ch,0,0,TO_ROOM);
		
		raw_kill(ch,ch);
		
		for (corpse = room->contents; corpse; corpse = corpse->next_content)
			if (corpse->item_type == ITEM_CORPSE_PC)
				break;
		if (corpse && corpse->item_type == ITEM_CORPSE_PC)
			extract_obj(corpse);

		sprintf(buf,"%sMortal....  I was made a promise of flesh.  I claim thine....%s\n\r",get_char_color(ch,"red"),END_COLOR(ch));
		send_to_char(buf,ch);
		ch->pcdata->greaterdata[GREATER_OROBAS] = FAVOR_FAILED;
	}

	return;
}


void do_breath_mephisto(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim;

	if (get_skill(ch,skill_lookup("breath of mephisto")) < 5)
		return send_to_char("Huh?\n\r",ch);

	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	
	if(is_affected(ch,skill_lookup("breath of mephisto")))
		return send_to_char("You cannot yet unleash another frigid blast.\n\r",ch);	

	if (argument[0] == '\0')  {
		if (ch->fighting)
			victim = ch->fighting;
		else
			return send_to_char("Breathe on whom?\n\r",ch);
	} else
		victim = get_char_room(ch,argument);
	
	if (!victim)
		return send_to_char("They aren't here.\n\r",ch);
	
	if(number_percent() > (get_skill(ch,skill_lookup("breath of mephisto")))) {
		send_to_char("You fail to build up the intense cold within you.\n\r",ch);
		check_improve(ch,skill_lookup("breath of mephisto"),FALSE,2);
		return;
	}
	
	act("You begin building up the intense cold within you.",ch,0,0,TO_CHAR);
	act("$n's skin shifts to a shade of blue.",ch,0,0,TO_ROOM);
	RS.Queue.AddToQueue(3, 3, mephisto_two, ch, victim, argument);

	return;
}

void mephisto_two(CHAR_DATA *ch, CHAR_DATA *victim, char *argument)
{
	AFFECT_DATA af;

	if(!(ch->fighting) && !(get_char_room(ch,argument))) {
		act("You let the intense cold dissipate, your target having escaped.",ch,0,0,TO_CHAR);
		return;
	}

	act("$n opens $s mouth and a plume of deathly cold issues forth!",ch,0,0,TO_ROOM);
	act("You open your mouth and unleash your icy fury!",ch,0,0,TO_CHAR);
	
	damage_new(ch,victim,ch->level*3,TYPE_UNDEFINED,TRUE,DAM_COLD,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"icy breath");
	
	act("$n freezes in $s tracks under the icy onslaught.",victim,0,ch,TO_ROOM);
	act("The intense cold shocks you into temporary immobility!",ch,0,victim,TO_VICT);

	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	LAG_CHAR(victim,PULSE_VIOLENCE*4);
	
	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_TIMER;
	af.type = skill_lookup("breath of mephisto");
	af.owner = ch;
	af.duration = 4;
	af.modifier = 0;
	af.location = 0;
	affect_to_char(ch,&af);
	return;
}

void do_touch(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim;
	AFFECT_DATA af;
	int chance=75;

	if(argument[0]=='\0') {
		if(ch->fighting)
			victim=ch->fighting;
		else
			return send_to_char("Touch who?\n\r",ch);
	} else
		if ((victim=get_char_room(ch,argument)) == NULL)
			return send_to_char("They aren't here.\n\r",ch);

	if((victim == ch))
		return send_to_char("You'll go blind.\n\r",ch);

	if((get_eq_char(ch,WEAR_WIELD) != NULL) && (get_eq_char(ch,WEAR_DUAL_WIELD) != NULL && get_eq_char(ch,WEAR_SHIELD) != NULL))
		return send_to_char("You must have a free hand to touch your opponent.\n\r",ch);

	if(is_safe_new(ch,victim,TRUE))
		return;

	if(ch->fighting)
		chance-=20;
	if(victim->position == POS_SLEEPING)
		chance+=100;
	if(victim->position < POS_STANDING)
		chance+=10;
	
	if(number_percent() < chance) {
		WAIT_STATE(ch,PULSE_VIOLENCE*2);
		act("You reach out and lay your hand on $N as $E bursts into flames!",ch,0,victim,TO_CHAR);
		act("$n places his hand on you as you feel fire engulf your body!",ch,0,victim,TO_VICT);
		act("$n places his hand on $N as $N bursts into flames!",ch,0,victim,TO_NOTVICT);
		damage_new(ch,victim,ch->level*2,TYPE_UNDEFINED,DAM_FIRE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"burning touch");
		if(is_affected(victim,skill_lookup("burning touch")))
			return;
		init_affect(&af);
		af.where = TO_AFFECTS;
		af.location = 0;
		af.modifier = 0;
		af.owner = ch;
		af.type = skill_lookup("burning touch");
		af.aftype = AFT_MALADY;
		af.end_fun = NULL;
		af.pulse_fun = burning_pulse;
		af.duration = ch->level/12;
		affect_to_char(victim,&af);
		return;

	} else {
		
		act("You reach out for $N, but $E twists away from you.",ch,0,victim,TO_CHAR);
		act("$n reaches for you, but you twist away from $m.",ch,0,victim,TO_VICT);
		act("$n reaches for $N, but $E twists away from $m.",ch,0,victim,TO_NOTVICT);
		WAIT_STATE(ch,PULSE_VIOLENCE*2);
		return;
	}
	
	return;
}

void check_orobas_gamygyn(CHAR_DATA *ch,CHAR_DATA *victim)
{
	OBJ_DATA *corpse;
	bool found = FALSE;
	char buf[MSL];

	for (corpse = ch->in_room->contents; corpse; corpse = corpse->next_content)
		if (corpse->item_type == ITEM_CORPSE_PC &&
			!str_cmp(corpse->owner,victim->true_name)) {
			found = TRUE;
			break;
		}

	if (!found)
		return;
	
	if (is_affected(ch,gsn_orobas_soul)
		&& victim->Class()->name == "anti-paladin") {
		act("With a sound of rending flesh, something tears itself from your body!",ch,0,0,TO_CHAR);
		act("With a sound of rending flesh, something tears itself from $n's body!",ch,0,0,TO_ROOM);
		act("You fall to your knees, bellowing in horror and pain!",ch,0,0,TO_CHAR);
		act("$n falls to $s knees, bellowing in horror and pain!",ch,0,0,TO_ROOM);
		act("A flailing multi-armed demon rends hungrily at $N's corpse with its many arms!",ch,0,victim,TO_ALL);
		act("Orobas turns to face you for a moment, and you feel infused with power!",ch,0,0,TO_CHAR);
		act("The monstrosity streaks away in a blur, cradling the corpse in its hands.",ch,0,0,TO_ALL);
		ch->pcdata->greaterdata[GREATER_OROBAS] = FAVOR_GRANTED;
		ch->pcdata->learned[skill_lookup("traitors luck")] = 1;
		extract_obj(corpse);
		return;
	}

	if (is_affected(ch,gsn_gamygyn_soul) && IS_GOOD(victim)) {
		act("With a terrible wail, a figure composed of gloomy light springs from your torso!",ch,0,0,TO_CHAR);
		act("Suddenly, a figure composed of gloomy light springs from from $n's torso!",ch,0,0,TO_ROOM);
		act("Leaping upon the fresh corpse, the fiend howls with delight and devours it.",ch,0,0,TO_ALL);
		extract_obj(corpse);
		sprintf(buf,"Gamygyn tells you '%sYou have met your end of the bargain, mortal, and now I shall meet mine.%s'\n\r",
			get_char_color(ch,"tells"), END_COLOR(ch));
		send_to_char(buf,ch);
		send_to_char("You feel an influx of power at Gamygyn's words.\n\r",ch);
		act("Enveloped in a black glow, the demon vanishes from sight.",ch,0,0,TO_ALL);
		ch->pcdata->greaterdata[GREATER_GAMYGYN] = FAVOR_GRANTED;
		ch->pcdata->learned[skill_lookup("bloodlust")] = 1;
		return;
	}

	return;
}

void burning_pulse(CHAR_DATA *ch, AFFECT_DATA *af)
{
	CHAR_DATA *owner=af->owner;
	if(number_percent() > 50)
		return;
	
	act("You scream in pain as fire erupts from within your body!",ch,0,0,TO_CHAR);
	act("$n screams in pain as fire erupts from within $s body!",ch,0,0,TO_ROOM);
	damage_new(owner,ch,owner->level/2,skill_lookup("burning touch"),DAM_FIRE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"the consuming fire*");
	return;
}

void do_darksight(CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;
	int number;

	if ( (number = get_skill(ch,gsn_darksight)) == 0 ||
		ch->level < skill_table[gsn_darksight].skill_level[ch->Class()->GetIndex()] )
		return send_to_char("Huh?\n\r", ch);

	if (is_affected(ch,gsn_darksight))
		return send_to_char("You are already peering intently into the shadows.\n\r",ch);

	if (ch->pcdata->greaterdata[GREATER_GERYON] == GERYON_EYE)
		return send_to_char("You cannot do that with only one eye.\n\r",ch);

	if (ch->mana < 40)
		return send_to_char("You don't have enough mana.\n\r",ch);

	if ( (number = number_percent()) > get_skill(ch, gsn_darksight) ) {
		send_to_char("You peer into the shadows but your vision stays the same.\n\r",ch);
		check_improve(ch,gsn_darksight,FALSE, 2);
		ch->mana -= 20;
		return;
	}

	WAIT_STATE(ch,PULSE_VIOLENCE);
	act("Dark power surges through your eyes as you focus upon the shadows.",ch,0,0,TO_CHAR);
	ch->mana -= 40;

	init_affect(&af);
	af.where		=	TO_AFFECTS;
	af.type			=	gsn_darksight;
	af.owner		=	ch;
	af.aftype		=	AFT_SKILL;
	af.duration		=	12;
	af.location		=	0;
	af.modifier		=	0;
	SET_BIT(af.bitvector, AFF_DARK_VISION);
	af.end_fun		=	darksight_end;
	affect_to_char(ch,&af);
	check_improve(ch,gsn_darksight,TRUE,1);

	return;
}

void darksight_end (CHAR_DATA *ch, AFFECT_DATA *af)
{
	AFFECT_DATA af2;

	init_affect(&af2);
	af2.where		=	TO_AFFECTS;
	af2.type		=	gsn_darksight;
	af2.owner		=	ch;
	af2.aftype		=	AFT_TIMER;
	af2.duration	=	36;
	af2.modifier	=	0;
	af2.location	=	0;
	affect_to_char(ch,&af2);

	return;
}

/* Dark Insight -- Completely ripped from assess */

char *get_insight_line(long where[])
{
        char buf[MSL], last[MSL], *beep;
        int i;

        buf[0] = '\0';
        last[0] = '\0';

        for (i = 0; imm_flags[i].name != NULL; i++)
        {
                if (IS_SET(where,imm_flags[i].bit))
                {
                        if (str_cmp(last,""))
                        {
                                strcat(buf,last);
                                strcat(buf,", ");
                        }
                        sprintf(last,"%s",imm_flags[i].name);
                }
        }
        if (str_cmp(buf,"") && str_cmp(last,""))
        {
                strcat(buf,"and ");
                strcat(buf,last);
        }
        else if (!str_cmp(buf,"") && str_cmp(last,""))
        {
                strcat(buf,last);
        }
        else
        {
                strcat(buf,"nothing");
        }

        beep = palloc_string(buf);
        return beep;
}

void spell_dark_insight(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
        int found = 0;
        float dammod;
        CHAR_DATA *victim = (CHAR_DATA *) vo;

        act("You call upon the dark powers to reveal $N's weaknesses.",ch,0,victim,TO_CHAR);
        act("$n utters arcane words as $e looks you over.",ch,0,victim,TO_VICT);
        act("$n utters arcane words as $e looks $N over.",ch,0,victim,TO_NOTVICT);


                act("$N appears to be resistant to $t.",ch,(char*)get_insight_line(victim->res_flags),victim,TO_CHAR);
                found++;


                act("$N appears to be immune to $t.",ch,(char*)get_insight_line(victim->imm_flags),victim,TO_CHAR);
                found++;

                act("$N appears to be vulnerable to $t.",ch,(char*)get_insight_line(victim->vuln_flags),victim,TO_CHAR);
                found++;



                dammod = victim->dam_mod;
                if (is_affected(victim,gsn_sanctuary))          dammod *= .5;
                if (is_affected(victim,gsn_sanguine_ward))      dammod *= .8;
                if (is_affected(victim,gsn_visceral))           dammod *= .87;
                if (is_affected(victim,gsn_stoneskin))          dammod *= .8;
                if (is_affected(victim,gsn_soften))                     dammod *= 1.3;

                if (victim->dam_mod < 20)
                        act("$N is virtually impervious to harm!",ch,0,victim,TO_CHAR);
                else if (victim->dam_mod <= 40)
                        act("$N has extensive protection from all sorts of harm.",ch,0,victim,TO_CHAR);
                else if (victim->dam_mod <= 60)
                        act("$N is well protected from all sorts of harm.",ch,0,victim,TO_CHAR);
                else if (victim->dam_mod <= 80)
                        act("$N is somewhat protected from all sorts of harm.",ch,0,victim,TO_CHAR);
                else if (victim->dam_mod <= 99)
                        act("$N's susceptibility to damage is slightly reduced.",ch,0,victim,TO_CHAR);
                else if (victim->dam_mod <= 100)
                        act("$N does not appear to have any special protections.",ch,0,victim,TO_CHAR);
                else
                        act("$N is highly susceptible to all damage.",ch,0,victim,TO_CHAR);
                found++;


        if (found == 0)
                send_to_char("Your insight cannot find a flaw to exploit.\n\r",ch);

        return;
}
