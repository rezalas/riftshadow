#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "../merc.h"
#include "../entity/handles.h"
#include "paladin.h"
#include "../interp.h"
#include "../tables.h"
#include "../recycle.h"
#include "../magic.h"
#include "../db.h"
#include "../lookup.h"
#include "warrior.h"
#include "../fight.h"
#include "../skills.h"
#include "../const.h"
#include "../misc.h"
#include "../comm.h"
#include "../devextra.h"
#include "../utility.h"
#include "../act_comm.h"
#include "../act_info.h"
#include "../pstring.h"

void spell_rites_of_preparation(int sn, int /* level */, CHAR_DATA *ch, SpellTarget /* vo */, CastMode /* mode */)
{
	CHAR_DATA *to;
	AFFECT_DATA af;
	bool hasgroup= false;

	if (is_affected(ch, sn))
	{
		send_to_char("You cannot focus on preparing your party for battle again so soon.\n\r", ch);
		return;
	}

	for (to = ch->in_room->people; to != nullptr; to = to->next_in_room)
	{
		if (is_same_group(to, ch) && to != ch)
			hasgroup = true;
		else
			continue;

		if (is_same_group(to, ch) && !is_npc(to) && !is_affected(to, sn))
		{
			to->pcdata->condition[COND_HUNGER] = 0;
			to->pcdata->condition[COND_THIRST] = 0;

			init_affect(&af);
			af.where = TO_AFFECTS;
			af.type = sn;
			af.modifier = (ch->level / 7);
			af.location = APPLY_HITROLL;
			af.duration = 24;
			af.level = ch->level;
			affect_to_char(to, &af);

			af.type = sn;
			af.modifier = (ch->level / 7);
			af.location = APPLY_DAMROLL;
			af.duration = 24;
			af.level = ch->level;
			affect_to_char(to, &af);

			af.type = sn;
			af.modifier = -(ch->level / 8);
			af.location = APPLY_SAVES;
			af.duration = 24;
			af.aftype = AFT_COMMUNE;
			af.level = ch->level;
			affect_to_char(to, &af);
		}
	}

	if (hasgroup == false)
	{
		send_to_char("You need a party to perform these rites.\n\r", ch);
		return;
	}
	else
	{
		act("You recite some prayers to those in your party, bolstering their confidence and preparing them for battle.", ch, nullptr, nullptr, TO_CHAR);
		act("$n recites some prayers in a loud and confident voice, preparing you for righteous battle.", ch, nullptr, nullptr, TO_GROUP);

		init_affect(&af);
		af.where = TO_AFFECTS;
		af.type = sn;
		af.aftype = AFT_TIMER;
		af.level = ch->level;
		af.location = APPLY_NONE;
		af.modifier = 0;
		af.duration = 48;
		affect_to_char(ch, &af);
	}
}

void spell_spiritual_hammer(int sn, int level, CHAR_DATA *ch, SpellTarget vo, [[maybe_unused]] CastMode mode)
{
	CHAR_DATA *victim = vo.AsChar();
	float dam = dice(level, 4);

	if (victim == ch)
	{
		send_to_char("You can't do that.\n\r", ch);
		return;
	}

	if (is_evil(victim))
	{
		act("A brilliant hammer descends from the heavens, striking you with great force!", ch, nullptr, victim, TO_VICT);
		act("A brilliant hammer descends from the heavens, striking $N with great force!", ch, nullptr, victim, TO_NOTVICT);
		dam *= 1.5;
	}
	else if (is_good(victim))
	{
		act("A flickering hammer fades into existence above you and hesitantly descends.", ch, nullptr, victim, TO_VICT);
		act("A flickering hammer fades into existence above $N and slowly descends upon $M.", ch, nullptr, victim,
			TO_NOTVICT);
		dam *= .3;
	}
	else
	{
		act("A great hammer fades into existence above you and strikes down!", ch, nullptr, victim, TO_VICT);
		act("A great hammer fades into existence above $N and strikes down!", ch, nullptr, victim, TO_NOTVICT);
	}

	if (saves_spell(level, victim, DAM_HOLY))
		dam /= 2;

	damage_new(ch, victim, (int)dam, sn, DAM_HOLY, true, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "spiritual hammer");
}

void do_turn_undead(CHAR_DATA *ch, [[maybe_unused]] char *argument)
{
	int dam;
	int difference = 0;
	CHAR_DATA *victim, *v_next;
	AFFECT_DATA af;
	bool forceflee= false;

	if (get_skill(ch, gsn_turn_undead) == 0)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_turn_undead))
	{
		send_to_char("You have not yet regained enough power to turn undead again.\n\r", ch);
		return;
	}

	if (ch->mana < 75)
	{
		send_to_char("You don't have enough mana.\n\r", ch);
		return;
	}

	act("You raise your hands aloft and extend an aura of purity about you.", ch, nullptr, nullptr, TO_CHAR);
	act("$n raises $s hands aloft, and a faint aura billows out from $s form.", ch, nullptr, nullptr, TO_ROOM);

	for (victim = ch->in_room->people; victim != nullptr; victim = v_next)
	{
		v_next = victim->next_in_room;

		if (!is_npc(victim) || victim == ch || !IS_SET(victim->act, ACT_UNDEAD))
			continue;

		difference = ch->level - victim->level;
		if (IS_SET(victim->act, ACT_UNDEAD))
			set_fighting(ch, victim);
		if (difference >= 10 && is_npc(victim))
		{
			act("Horrible screams of undeath fill the air as $n is reduced to scattered ashes by brilliant white flames.", victim, nullptr, ch, TO_ROOM);
			raw_kill(ch, victim);
			forceflee= false;
		}
		else if (difference > 1)
		{
			dam = dice(ch->level, 20);
			act("$n is suddenly engulfed by holy fire!", victim, nullptr, ch, TO_ROOM);
			damage_new(ch, victim, dam, gsn_turn_undead, DAM_HOLY, true, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, nullptr);
			forceflee = true;
		}
		else if (difference > -5)
		{
			dam = dice(ch->level, 10);
			act("$n is burned by holy fire!", victim, nullptr, ch, TO_ROOM);

			if (number_percent() > 50)
				forceflee = true;

			damage_new(ch, victim, dam, gsn_turn_undead, DAM_HOLY, true, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, nullptr);
		}
		else
		{
			if (!is_affected_by(victim, AFF_CHARM))
			{
				act("$N shrieks in unholy fury and bounds toward you!", ch, nullptr, victim, TO_CHAR);
				act("$n shrieks in unholy fury and bounds toward $N!", victim, nullptr, ch, TO_ROOM);
				multi_hit(victim, ch, TYPE_UNDEFINED);
				forceflee= false;
			}
			else
			{
				forceflee= false;
			}
		}

		if (forceflee == true && !IS_SET(victim->act, ACT_SENTINEL))
		{
			act("$n shambles away in hopes of escaping $N's holy aura!", victim, nullptr, ch, TO_ROOM);
			act("$n shambles away from your holy presence!", ch, nullptr, victim, TO_CHAR);
			do_flee(victim, "");
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
	affect_to_char(ch, &af);
}

bool check_benevolence(CHAR_DATA *ch, CHAR_DATA *keeper)
{
	if (!is_good(keeper))
		return false;

	if (number_percent() > get_skill(ch, gsn_benevolence))
		return false;

	act("$N smiles warmly at you and bows slightly, offering you a large discount.", ch, nullptr, keeper, TO_CHAR);

	return true;
}

bool check_intercept(CHAR_DATA *ch, CHAR_DATA *victim, CHAR_DATA *paladin, int dt)
{
	int chance, skill, gsize = 0;
	CHAR_DATA *gch;
	char buf1[MSL], buf2[MSL], buf3[MSL];
	char *attack;

	if (!is_same_group(victim, paladin))
		return false;

	if (!is_awake(paladin))
		return false;

	if (Deref(paladin->fighting) != ch)
		return false;

	if (!is_npc(ch) && (!can_pk(ch, paladin) || !can_pk(paladin, ch)))
		return false;

	if ((skill = get_skill(paladin, gsn_intercept)) <= 1)
		return false;

	if (get_eq_char(paladin, WEAR_SHIELD) == nullptr)
		return false;

	if (paladin == victim)
		return false;

	for (gch = paladin->in_room->people; gch; gch = gch->next_in_room)
	{
		if (is_same_group(paladin, gch))
			gsize++;
	}

	switch (gsize)
	{
		case 2:
			chance = 40;
			break;
		case 3:
			chance = 20;
			break;
		default:
			return false;
	}

	skill = skill * (15 + get_skill(ch, gsn_shield_block) / 4) / 100;
	chance = skill * chance / 100;

	if (!is_npc(ch))
	{
		if (check_posture(ch) == POSTURE_DEFENSE && style_check(gsn_posture, ch->pcdata->style))
			chance += 8;
	}

	if (is_npc(victim) || is_npc(ch))
		chance += (victim->level - ch->level) / 2;

	attack = get_dam_message(ch, dt);

	sprintf(buf1, "You thrust your shield between %s's %s and $N.",
		is_npc(ch) ? ch->short_descr : ch->name,
		attack);
	act(buf1, paladin, nullptr, victim, TO_CHAR);

	sprintf(buf2, "%s thrusts $s shield in front of you, deflecting $N's %s.",
		is_npc(paladin) ? paladin->short_descr : paladin->name,
		attack);
	act(buf2, victim, nullptr, ch, TO_CHAR);

	sprintf(buf3, "$n thrusts $s shield in front of %s, deflecting your %s.",
		is_npc(victim) ? victim->short_descr : victim->name,
		attack);
	act(buf3, ch, nullptr, paladin, TO_CHAR);

	check_improve(paladin, gsn_intercept, true, 4);
	return true;
}

void spell_blinding_orb(int sn, int level, CHAR_DATA *ch, SpellTarget vo, [[maybe_unused]] CastMode mode)
{
	CHAR_DATA *victim = vo.AsChar(), *vch_next;
	AFFECT_DATA af;
	int dam, hitroll, duration, b_hitroll, b_duration;

	act("You bring forth an orb of blinding light to sear the eyes of your foes!", ch, nullptr, nullptr, TO_CHAR);
	act("$n calls forth a brilliant orb of radiant light!", ch, nullptr, nullptr, TO_ROOM);

	for (victim = ch->in_room->people; victim != nullptr; victim = vch_next)
	{
		vch_next = victim->next_in_room;

		if (is_same_group(ch, victim) || is_same_cabal(ch, victim) || is_safe(ch, victim))
			continue;

		dam = dice(level, 2);

		b_hitroll = -4;
		b_duration = 1;

		if (is_good(victim))
		{
			if (number_percent() > 25)
				continue;

			dam /= 3;
			hitroll = b_hitroll;
			duration = b_duration;
		}
		else if (is_neutral(victim))
		{
			if (number_percent() > 75)
				continue;

			dam /= 2;
			hitroll = b_hitroll * 2;
			duration = b_duration * 3;
		}
		else
		{
			dam = (int)(dam * 1.5);
			hitroll = b_hitroll * 4;
			duration = b_duration * 6;
		}

		char buf2[MSL];
		sprintf(buf2, "Die, %s, you sorcerous dog!", pers(ch, victim));

		if (!Deref(victim->fighting) && !is_npc(victim))
			do_myell(victim, buf2, ch);

		damage_new(ch, victim, dam, sn, DAM_LIGHT, true, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, nullptr);

		if (!saves_spell(ch->level, victim, DAM_LIGHT) && !is_affected_by(victim, AFF_BLIND))
		{
			act("$n falters as the light sears $s eyes.", victim, nullptr, ch, TO_ROOM);
			act("You are blinded by the bright light!", ch, nullptr, victim, TO_VICT);

			init_affect(&af);
			af.where = TO_AFFECTS;
			af.type = sn;
			af.aftype = AFT_COMMUNE;
			af.level = level;
			af.location = APPLY_HITROLL;
			af.modifier = hitroll;
			SET_BIT(af.bitvector, AFF_BLIND);
			af.duration = duration;
			af.mod_name = MOD_VISION;
			affect_to_char(victim, &af);
		}
	}
}

void spell_voice_of_damnation(int sn, int /* level */, CHAR_DATA *ch, SpellTarget vo, CastMode /* mode */)
{
	CHAR_DATA *victim = vo.AsChar(), *vch_next;

	act("Imbued with the strength of your god, you threaten those around you with eternal damnation!", ch, nullptr, victim, TO_CHAR);
	act("$N shouts threats of damnation in a strange and powerful voice!", victim, nullptr, ch, TO_ROOM);

	for (victim = ch->in_room->people; victim != nullptr; victim = vch_next)
	{
		vch_next = victim->next_in_room;

		if (is_same_cabal(ch, victim)
			|| is_same_group(ch, victim)
			|| is_affected(victim, gsn_deafen)
			|| is_safe(ch, victim))
		{
			continue;
		}

		if ((is_npc(victim) && !is_evil(victim))
			|| (!is_npc(victim) && (is_good(victim) || is_neutral(victim))
				&& (victim->pcdata->kills[PK_GOOD] <= victim->pcdata->kills[PK_EVIL])))
		{
			act("Knowing that you have not persecuted the righteous in the past, you are not concerned.", ch, nullptr, victim, TO_VICT);
			continue;
		}

		act("The terrifying voice resounds in your head, filling you with fear about your fate!", ch, nullptr, victim, TO_VICT);
		damage_new(ch, victim, dice(ch->level, 5), sn, DAM_MENTAL, true, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, nullptr);
		WAIT_STATE(victim, PULSE_VIOLENCE * 1);
	}
}

void spell_seraphic_mantle(int sn, int /* level */, CHAR_DATA *ch, SpellTarget vo, CastMode /* mode */)
{
	CHAR_DATA *victim = vo.AsChar();
	AFFECT_DATA af;

	if (victim != ch)
	{
		send_to_char("You can only commune this upon yourself.\n\r", ch);
		return;
	}

	if (is_affected(ch, sn))
	{
		send_to_char("You are already protected by a seraphic mantle.\n\r", ch);
		return;
	}

	act("A shining mantle of light descends upon your shoulders, protecting you from harm.", ch, nullptr, nullptr, TO_CHAR);
	act("A shining mantle of light descends upon $n, lending $m an angelic presence.", ch, nullptr, nullptr, TO_ROOM);

	init_affect(&af);
	af.aftype = AFT_COMMUNE;
	af.modifier = -40;
	af.location = APPLY_DAM_MOD;
	af.duration = ch->level / 5;
	af.level = ch->level;
	af.type = sn;
	af.mod_name = MOD_PROTECTION;
	affect_to_char(ch, &af);

	af.mod_name = MOD_NONE;
	af.modifier = -(ch->level / 2);
	af.location = APPLY_SAVES;
	af.level = ch->level;
	af.type = sn;
	affect_to_char(ch, &af);
}

void spell_arms_of_light(int sn, int level, CHAR_DATA *ch, SpellTarget /* vo */, CastMode /* mode */)
{
	char arg[MSL];
	OBJ_DATA *weapon;
	AFFECT_DATA af;
	OBJ_AFFECT_DATA oaf;

	target_name = one_argument(target_name, arg);

	if ((weapon = get_obj_carry(ch, arg, ch)) == nullptr)
	{
		send_to_char("You aren't carrying that.\n\r", ch);
		return;
	}

	if (weapon->item_type != ITEM_WEAPON)
	{
		send_to_char("You can only commune this upon weapons.\n\r", ch);
		return;
	}

	if (is_affected(ch, sn))
	{
		send_to_char("You may not commune this supplication again yet.\n\r", ch);
		return;
	}

	if (is_affected_obj(weapon, gsn_arms_of_light)
		|| is_affected_obj(weapon, gsn_arms_of_purity)
		|| is_affected_obj(weapon, gsn_arms_of_wrath)
		|| is_affected_obj(weapon, gsn_arms_of_judgement))
	{
		send_to_char("This weapon is already enchanted with holy power\n\r", ch);
		return;
	}

	init_affect_obj(&oaf);
	oaf.where = TO_OBJ_AFFECTS;
	oaf.type = sn;
	oaf.aftype = AFT_COMMUNE;
	oaf.location = obj_location(APPLY_NONE);
	oaf.modifier = 0;
	oaf.duration = level;
	oaf.level = ch->level;
	oaf.owner = ch->self;
	affect_to_obj(weapon, &oaf);

	act("A bright glow begins to emanate from $n's $p.", ch, weapon, nullptr, TO_ROOM);
	act("You enchant $p with holy light.", ch, weapon, nullptr, TO_CHAR);

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = sn;
	af.location = APPLY_NONE;
	af.modifier = 0;
	af.aftype = AFT_TIMER;
	af.duration = ch->level;
	af.level = ch->level;
	affect_to_char(ch, &af);

	WAIT_STATE(ch, PULSE_VIOLENCE * 2);
}

void spell_arms_of_purity(int sn, int level, CHAR_DATA *ch, SpellTarget /* vo */, CastMode /* mode */)
{
	char arg[MSL];
	OBJ_DATA *weapon;
	AFFECT_DATA af;
	OBJ_AFFECT_DATA oaf;
	target_name = one_argument(target_name, arg);

	if ((weapon = get_obj_carry(ch, arg, ch)) == nullptr)
	{
		send_to_char("You aren't carrying that.\n\r", ch);
		return;
	}

	if (weapon->item_type != ITEM_WEAPON)
	{
		send_to_char("You can only commune this upon weapons.\n\r", ch);
		return;
	}

	if (is_affected_obj(weapon, gsn_arms_of_light)
		|| is_affected_obj(weapon, gsn_arms_of_purity)
		|| is_affected_obj(weapon, gsn_arms_of_wrath)
		|| is_affected_obj(weapon, gsn_arms_of_judgement))
	{
		send_to_char("Your weapon is already enchanted with holy power.\n\r", ch);
		return;
	}

	if (is_affected(ch, sn))
	{
		send_to_char("You may not commune this supplication again yet.\n\r", ch);
		return;
	}

	init_affect_obj(&oaf);
	oaf.where = TO_OBJ_AFFECTS;
	oaf.level = ch->level;
	oaf.type = sn;
	oaf.duration = level;
	oaf.location = obj_location(APPLY_NONE);
	oaf.modifier = 0;
	oaf.owner = ch->self;
	affect_to_obj(weapon, &oaf);

	act("Rippling waves of warm energy play up and down $n's $p.", ch, weapon, nullptr, TO_ROOM);
	act("You enchant $p with righteous purity.", ch, weapon, nullptr, TO_CHAR);

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = sn;
	af.duration = level;
	af.modifier = 0;
	af.location = APPLY_NONE;
	af.level = ch->level;
	af.aftype = AFT_TIMER;
	affect_to_char(ch, &af);

	WAIT_STATE(ch, PULSE_VIOLENCE * 2);
}

void spell_arms_of_wrath(int sn, int level, CHAR_DATA *ch, SpellTarget /* vo */, CastMode /* mode */)
{
	char arg[MSL];
	OBJ_DATA *weapon;
	AFFECT_DATA af;
	OBJ_AFFECT_DATA oaf;

	target_name = one_argument(target_name, arg);

	if ((weapon = get_obj_carry(ch, arg, ch)) == nullptr)
	{
		send_to_char("You aren't carrying that.\n\r", ch);
		return;
	}

	if (weapon->item_type != ITEM_WEAPON)
	{
		send_to_char("You can only commune this upon weapons.\n\r", ch);
		return;
	}

	if (is_affected(ch, sn))
	{
		send_to_char("You may not commune this supplication again yet.\n\r", ch);
		return;
	}

	if (is_affected_obj(weapon, gsn_arms_of_light)
		|| is_affected_obj(weapon, gsn_arms_of_purity)
		|| is_affected_obj(weapon, gsn_arms_of_wrath)
		|| is_affected_obj(weapon, gsn_arms_of_judgement))
	{
		send_to_char("This weapon is already enchanted with holy power!\n\r", ch);
		return;
	}

	init_affect_obj(&oaf);
	oaf.where = TO_OBJ_AFFECTS;
	oaf.type = sn;
	oaf.aftype = AFT_COMMUNE;
	oaf.location = obj_location(APPLY_NONE);
	oaf.modifier = 0;
	oaf.duration = level;
	oaf.level = ch->level;
	oaf.owner = ch->self;
	affect_to_obj(weapon, &oaf);

	act("$n's $p is set ablaze with white flames!", ch, weapon, nullptr, TO_ROOM);
	act("You enchant $p with holy wrath.", ch, weapon, nullptr, TO_CHAR);

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = sn;
	af.location = APPLY_NONE;
	af.modifier = 0;
	af.aftype = AFT_TIMER;
	af.duration = ch->level;
	af.level = ch->level;
	affect_to_char(ch, &af);

	WAIT_STATE(ch, PULSE_VIOLENCE * 2);
}

void spell_arms_of_judgement(int sn, int level, CHAR_DATA *ch, SpellTarget /* vo */, CastMode /* mode */)
{
	char arg[MSL];
	OBJ_DATA *weapon;
	AFFECT_DATA af;
	OBJ_AFFECT_DATA oaf;

	target_name = one_argument(target_name, arg);

	if ((weapon = get_obj_carry(ch, arg, ch)) == nullptr)
	{
		send_to_char("You aren't carrying that.\n\r", ch);
		return;
	}

	if (weapon->item_type != ITEM_WEAPON)
	{
		send_to_char("You can only commune this upon weapons.\n\r", ch);
		return;
	}

	if (is_affected(ch, sn))
	{
		send_to_char("You may not commune this supplication again yet.\n\r", ch);
		return;
	}

	if (is_affected_obj(weapon, gsn_arms_of_light)
		|| is_affected_obj(weapon, gsn_arms_of_purity)
		|| is_affected_obj(weapon, gsn_arms_of_wrath)
		|| is_affected_obj(weapon, gsn_arms_of_judgement))
	{
		return send_to_char("This weapon is already enchanted with holy power!\n\r", ch);
	}

	init_affect_obj(&oaf);
	oaf.where = TO_OBJ_AFFECTS;
	oaf.type = sn;
	oaf.aftype = AFT_COMMUNE;
	oaf.location = obj_location(APPLY_NONE);
	oaf.modifier = 0;
	oaf.duration = level;
	oaf.level = ch->level;
	oaf.owner = ch->self;
	affect_to_obj(weapon, &oaf);

	act("Bright golden light radiates from $n's $p!", ch, weapon, nullptr, TO_ROOM);
	act("You enchant $p with the power of divine judgement.", ch, weapon, nullptr, TO_CHAR);

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = sn;
	af.location = APPLY_NONE;
	af.modifier = 0;
	af.aftype = AFT_TIMER;
	af.duration = ch->level;
	af.level = ch->level;
	affect_to_char(ch, &af);

	WAIT_STATE(ch, PULSE_VIOLENCE * 2);
}

void do_strike_of_virtue(CHAR_DATA *ch, [[maybe_unused]] char *argument)
{
	OBJ_DATA *weapon;
	CHAR_DATA *victim = Deref(ch->fighting);

	weapon = get_eq_char(ch, WEAR_WIELD);

	if (ch->position != POS_FIGHTING)
	{
		send_to_char("You must be fighting to perform this maneuver.\n\r", ch);
		return;
	}

	if (weapon == nullptr)
	{
		send_to_char("You must be wielding a weapon.\n\r", ch);
		return;
	}

	if (get_skill(ch, gsn_strike_of_virtue) * .85 > number_percent())
	{
		act("$n brings $s $p around for a particularly brutal blow!", ch, weapon, nullptr, TO_ROOM);
		act("You deliver a brutal blow to $N!", ch, nullptr, victim, TO_CHAR);

		one_hit_new(ch, victim, gsn_strike_of_virtue, HIT_SPECIALS, HIT_UNBLOCKABLE, HIT_NOADD, 125, nullptr);
		check_improve(ch, gsn_strike_of_virtue, true, 1);
	}
	else
	{
		act("You swing hard, but fail to connect with your opponent.", ch, nullptr, nullptr, TO_CHAR);
		damage(ch, victim, 0, gsn_strike_of_virtue, DAM_NONE, true);
		check_improve(ch, gsn_strike_of_virtue, false, 1);
	}

	WAIT_STATE(ch, PULSE_VIOLENCE * 2);
}

void spell_divine_frenzy(int sn, int /* level */, CHAR_DATA *ch, SpellTarget /* vo */, CastMode /* mode */)
{
	AFFECT_DATA af;

	if (is_affected(ch, gsn_divine_frenzy))
	{
		send_to_char("You are already in a divine frenzy!\n\r", ch);
		return;
	}

	act("You go into a vengeful rage against the wicked!", ch, nullptr, nullptr, TO_CHAR);
	act("$n begins to breathe heavily as a vengeful look spreads across $s face.", ch, nullptr, nullptr, TO_ROOM);

	init_affect(&af);
	af.type = sn;
	af.where = TO_AFFECTS;
	af.aftype = AFT_COMMUNE;
	af.location = APPLY_HITROLL;
	af.modifier = 15;
	af.duration = 8;
	af.level = ch->level;
	affect_to_char(ch, &af);

	af.location = APPLY_DAMROLL;
	af.modifier = 15;
	af.duration = 8;
	af.level = ch->level;
	affect_to_char(ch, &af);
}

void do_group_retreat(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *to = nullptr;
	CHAR_DATA *vch_next;
	CHAR_DATA *victim = Deref(ch->fighting);
	ROOM_INDEX_DATA *to_room = nullptr;
	EXIT_DATA *pexit;
	int dir;
	int skill;
	char *direction;

	one_argument(argument, arg);

	if (!Deref(ch->fighting))
	{
		send_to_char("You aren't fighting!\n\r", ch);
		return;
	}

	if (arg[0] == '\0')
	{
		send_to_char("Retreat in which direction?\n\r", ch);
		return;
	}

	if (!str_cmp(arg, "n") || !str_cmp(arg, "north"))
	{
		dir = 0;
	}
	else if (!str_cmp(arg, "e") || !str_cmp(arg, "east"))
	{
		dir = 1;
	}
	else if (!str_cmp(arg, "s") || !str_cmp(arg, "south"))
	{
		dir = 2;
	}
	else if (!str_cmp(arg, "w") || !str_cmp(arg, "west"))
	{
		dir = 3;
	}
	else if (!str_cmp(arg, "u") || !str_cmp(arg, "up"))
	{
		dir = 4;
	}
	else if (!str_cmp(arg, "d") || !str_cmp(arg, "down"))
	{
		dir = 5;
	}
	else
	{
		send_to_char("Retreat in which direction?\n\r", ch);
		return;
	}

	pexit = ch->in_room->exit[dir];

	if ((skill = get_skill(ch, gsn_group_retreat)) == 0)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	if (skill - 10 < number_percent())
	{
		WAIT_STATE(ch, PULSE_VIOLENCE);
		for (to = ch->in_room->people; to != nullptr; to = to->next_in_room)
		{
			if (is_same_group(victim, to))
				one_hit_new(to, ch, TYPE_UNDEFINED, HIT_SPECIALS, HIT_UNBLOCKABLE, HIT_NOADD, 125, nullptr);
		}

		send_to_char("You expose yourself to your foes, but fail to cover your group's retreat!\n\r", ch);
		return;
	}

	if (pexit != nullptr
		&& ((to_room = pexit->u1.to_room) != nullptr)
		&& (!IS_SET(pexit->exit_info, EX_CLOSED)
			|| ((IS_SET(pexit->exit_info, EX_CLOSED)
				&& is_affected_by(ch, AFF_PASS_DOOR))
				&& !IS_SET(pexit->exit_info, EX_NOPASS))))
	{
		direction = flag_name_lookup(dir, direction_table);

		act("You quickly position yourself to cover your group's retreat!", ch, nullptr, nullptr, TO_CHAR);
		act("$n positions $mself to cover $s group's retreat!", ch, nullptr, nullptr, TO_ROOM);

		for (to = ch->in_room->people; to != nullptr; to = vch_next)
		{
			vch_next = to->next_in_room;

			if (is_same_group(victim, to))
				one_hit_new(to, ch, TYPE_UNDEFINED, HIT_SPECIALS, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, nullptr);

			if (is_same_group(to, ch) && to != ch)
			{
				stop_fighting(to, true);
				act("$n retreats to the $t under $N's cover!", ch, direction, to, TO_NOTVICT);
				act("$N retreats under your cover!", ch, nullptr, to, TO_CHAR);
				act("You retreat under $n's cover.", ch, nullptr, to, TO_VICT);
				char_from_room(to);
				char_to_room(to, to_room);
			}
		}

		act("$s group having retreated, $n follows suit!", ch, nullptr, nullptr, TO_ROOM);
		stop_fighting(ch, true);

		char_from_room(ch);
		char_to_room(ch, to_room);

		send_to_char("You retreat from combat to join your groupmates!\n\r", ch);
		do_look(ch, "auto");
	}
	else
	{
		send_to_char("You can't retreat in that direction!\n\r", ch);
	}
}

void do_valiant_charge(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	CHAR_DATA *to;
	int count = 0;

	one_argument(argument, arg);

	if (arg[0] == '\0')
	{
		send_to_char("Lead a charge against whom?\n\r", ch);
		return;
	}
	else if ((victim = get_char_room(ch, arg)) == nullptr)
	{
		send_to_char("They aren't here.\n\r", ch);
		return;
	}

	if (ch == victim || is_same_group(ch, victim))
	{
		send_to_char("Lead your group into combat against themselves!?\n\r", ch);
		return;
	}

	if (victim->hit < victim->hit / 2)
	{
		send_to_char("They are too injured and wary to meet your charge.\n\r", ch);
		return;
	}

	for (to = ch->in_room->people; to != nullptr; to = to->next_in_room)
	{
		if (is_same_group(ch, to) && to != ch)
		{
			count++;
			break;
		}
	}

	if (count == 0)
	{
		send_to_char("It's hard to lead a group into combat if you don't have a group.\n\r", ch);
		return;
	}

	act("$n charges into combat with $N, bursting through $S defenses!", ch, nullptr, victim, TO_ROOM);
	act("$n drops his guard and charges directly into you!", ch, nullptr, victim, TO_VICT);
	act("You charge into combat, opening the way for others to strike!", ch, nullptr, victim, TO_CHAR);

	for (to = ch->in_room->people; to != nullptr; to = to->next_in_room)
	{
		if (is_same_group(victim, to))
			one_hit_new(to, ch, TYPE_UNDEFINED, HIT_SPECIALS, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, nullptr);

		if (is_same_group(ch, to) && to != ch && count < 5)
		{
			count++;
			one_hit_new(to, victim, TYPE_UNDEFINED, HIT_SPECIALS, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, nullptr);
		}
	}

	WAIT_STATE(ch, PULSE_VIOLENCE * 3);
}

void spell_awe(int /* level */, int /* sn */, CHAR_DATA *ch, SpellTarget /* vo */, CastMode /* mode */)
{
	AFFECT_DATA af;

	if (is_affected(ch, gsn_awe))
	{
		send_to_char("You are already radiating an awe-inspiring holy aura.\n\r", ch);
		return;
	}

	act("Holy energy crackles about your frame, forming a shimmering aura of light!", ch, nullptr, nullptr, TO_CHAR);
	act("Godly light suddenly explodes from within $n, finally settling into an incredible and almost intoxicating aura of power!", ch, nullptr, nullptr, TO_ROOM);

	init_affect(&af);
	af.level = ch->level;
	af.type = gsn_awe;
	af.where = TO_AFFECTS;
	af.aftype = AFT_COMMUNE;
	af.location = APPLY_NONE;
	af.modifier = 0;
	af.duration = 20;
	af.mod_name = MOD_APPEARANCE;
	affect_to_char(ch, &af);
}

void spell_shield_of_faith(int /* level */, int /* sn */, CHAR_DATA *ch, SpellTarget vo, CastMode /* mode */)
{
	AFFECT_DATA af;
	CHAR_DATA *to = vo.AsChar();

	if (to == nullptr)
		to = ch;

	if (is_affected(ch, gsn_shield_of_faith) && ch == to)
	{
		send_to_char("You are already surrounded by a shield of faith.\n\r", ch);
		return;
	}

	if (is_affected(to, gsn_shield_of_faith))
	{
		send_to_char("They are already surrounded by a shield of faith.\n\r", ch);
		return;
	}

	act("A pearly white barrier flickers into existence around you.", to, nullptr, nullptr, TO_CHAR);
	act("A pearly white barrier flickers into existence around $n.", to, nullptr, nullptr, TO_ROOM);

	init_affect(&af);
	af.level = ch->level;
	af.type = gsn_shield_of_faith;
	af.where = TO_AFFECTS;
	af.aftype = AFT_COMMUNE;
	af.location = APPLY_NONE;
	af.modifier = 0;
	af.duration = ch->level / 3;
	af.mod_name = MOD_PROTECTION;
	affect_to_char(to, &af);
}

void spell_holy_shroud(int /* level */, int /* sn */, CHAR_DATA *ch, SpellTarget vo, CastMode /* mode */)
{
	AFFECT_DATA af;
	CHAR_DATA *to = vo.AsChar();

	if (to == nullptr)
		to = ch;

	if (is_affected(ch, gsn_holy_shroud) && ch == to)
	{
		send_to_char("You are already protected by a holy shroud.\n\r", ch);
		return;
	}

	if (is_affected(to, gsn_holy_shroud))
	{
		send_to_char("They are already protected by a holy shroud.\n\r", ch);
		return;
	}

	act("A luminescent veil of white mist slowly drapes itself about your frame.", to, nullptr, nullptr, TO_CHAR);
	act("A luminescent veil of white mist slowly drapes itself about $n's frame.", to, nullptr, nullptr, TO_ROOM);

	init_affect(&af);
	af.level = ch->level;
	af.type = gsn_holy_shroud;
	af.where = TO_AFFECTS;
	af.aftype = AFT_COMMUNE;
	af.location = APPLY_NONE;
	af.modifier = 0;
	af.duration = ch->level / 5;
	af.mod_name = MOD_PROTECTION;
	affect_to_char(to, &af);
}

int check_arms(CHAR_DATA *ch, OBJ_DATA *wield, bool bOncePerRound)
{
	CHAR_DATA *victim = Deref(ch->fighting);
	AFFECT_DATA af;

	if (is_affected_obj(wield, gsn_arms_of_light))
	{
		if (bOncePerRound == true && number_percent() > 90)
		{
			act("$p suddenly flares brightly!", 0, wield, nullptr, TO_ROOM);

			if (!saves_spell(ch->level, victim, DAM_LIGHT))
			{
				act("$n appears to be blinded.", victim, nullptr, nullptr, TO_ROOM);
				act("You are blinded!", ch, nullptr, victim, TO_VICT);

				init_affect(&af);
				af.where = TO_AFFECTS;
				af.aftype = AFT_SKILL;
				af.type = gsn_blindness;
				af.duration = ch->level / 12;
				af.modifier = -4;
				af.location = APPLY_HITROLL;

				SET_BIT(af.bitvector, AFF_BLIND);

				affect_to_char(victim, &af);
			}
		}

		if (bOncePerRound == false && number_percent() < ch->level / 2)
			return 46;
	}

	if (is_affected_obj(wield, gsn_arms_of_wrath) && bOncePerRound == false)
	{
		if (number_percent() > 96 && victim)
		{
			act("$p explodes in holy flames!", ch, wield, nullptr, TO_ALL);
			damage_new(ch, victim, dice(ch->level, 2), 0, DAM_HOLY, true, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "holy wrath");

			if (number_percent() < ch->level / 2)
				return 18;
		}
	}

	if (is_affected_obj(wield, gsn_arms_of_purity))
	{
		if (bOncePerRound == true && number_percent() > 75)
		{
			act("$p glows softly and grows warm in your hands.", ch, wield, nullptr, TO_CHAR);
			act("$n's $p glows softly.", ch, wield, nullptr, TO_ROOM);
			obj_cast_spell(skill_lookup("cure critical"), ch->level, ch, ch, wield);
		}
	}

	if (is_affected_obj(wield, gsn_arms_of_judgement))
	{
		if (bOncePerRound == true
			&& number_percent() > 75
			&& !is_npc(victim)
			&& (is_evil(victim) || victim->pcdata->kills[PK_GOOD] < victim->pcdata->kills[PK_EVIL]))
		{
			if (!saves_spell(ch->level, victim, DAM_HOLY))
			{
				act("Brilliant light, followed by a loud booming noise emanates from $n's $p!", ch, wield, nullptr, TO_ROOM);
				act("$p shakes violently in your hands, exploding with holy vengeance!", ch, wield, nullptr, TO_CHAR);
				act("An invisible force bears down upon you.", ch, nullptr, victim, TO_VICT);
				act("$n staggers and reels, overcome by an invisible force!", victim, nullptr, nullptr, TO_ROOM);
				act("You succumb to the force, becoming temporarily paralyzed!", victim, nullptr, nullptr, TO_CHAR);
				WAIT_STATE(victim, PULSE_VIOLENCE * 2);
			}
		}

		if (bOncePerRound == false && ch->level / 2 > number_percent())
			return 20;
	}

	return 0;
}

void spell_empathy(int /* level */, int /* sn */, CHAR_DATA *ch, SpellTarget vo, CastMode /* mode */)
{
	AFFECT_DATA af;
	CHAR_DATA *vict = vo.AsChar();

	if (ch == vict)
	{
		send_to_char("You can not commune this on yourself.\n\r", ch);
		return;
	}

	if (is_affected(vict, gsn_empathy))
	{
		send_to_char("They are already being empathized with.\n\r", ch);
		return;
	}

	act("You touch your hand to $N's forehead and form a spiritual link.", ch, nullptr, vict, TO_CHAR);
	act("$n touches $s hand to your forehead and forms a spiritual link.", ch, nullptr, vict, TO_VICT);
	act("$n touches $N on $S forehead forming a spiritual link.", ch, nullptr, vict, TO_ROOM);

	init_affect(&af);
	af.owner = ch->self;
	af.level = ch->level;
	af.type = gsn_empathy;
	af.where = TO_AFFECTS;
	af.aftype = AFT_COMMUNE;
	af.location = APPLY_NONE;
	af.modifier = 0;
	af.duration = 12;
	af.end_fun = empathy_end;
	affect_to_char(vict, &af);
}

void empathy_end(CHAR_DATA *ch, AFFECT_DATA *af)
{
	// ch=char with empathy, af->owner = paladin
	if (Deref(af->owner))
		act("You feel pained as your spiritual link with $n is severed!", Deref(af->owner), nullptr, ch, TO_VICT);
}

void spell_tower_of_fortitude(int /* level */, int /* sn */, CHAR_DATA *ch, SpellTarget /* vo */, CastMode /* mode */)
{
	AFFECT_DATA af;

	if (is_affected(ch, gsn_tower_of_fortitude))
	{
		send_to_char("You are already affected by tower of fortitude.\n\r", ch);
		return;
	}

	act("You center your body and relax your muscles ready for incoming attacks.", ch, nullptr, nullptr, TO_CHAR);
	act("$n centers $mself and readies for combat.", ch, nullptr, nullptr, TO_ROOM);

	init_affect(&af);
	af.owner = ch->self;
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

void spell_indomitable_spirit(int /* level */, int /* sn */, CHAR_DATA *ch, SpellTarget /* vo */, CastMode /* mode */)
{
	AFFECT_DATA af;

	if (is_affected(ch, gsn_indomitable_spirit))
	{
		send_to_char("You are already affected by an indomitable spirit.\n\r", ch);
		return;
	}

	act("You call upon your deity to raise your spirit.", ch, nullptr, nullptr, TO_CHAR);
	act("$n calls up on $s deity to raise his spirit.", ch, nullptr, nullptr, TO_ROOM);

	init_affect(&af);
	af.owner = ch->self;
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

	if (ch->hit < 0)
	{
		diff = abs(ch->hit);

		if (diff > (ch->mana / 2))
		{
			af->end_fun = nullptr;
			affect_strip(ch, gsn_indomitable_spirit);

			act("Your body collapses under the strain of your spiritual exertion.", ch, nullptr, nullptr, TO_CHAR);
			act("$n's body collapses under the strain of $s spiritual exertion!", ch, nullptr, nullptr, TO_ROOM);
			act("$n is DEAD!!", ch, nullptr, nullptr, TO_ROOM);

			if (Deref(ch->fighting))
				raw_kill(Deref(ch->fighting), ch);
			else
				raw_kill(ch, ch);
		}
	}
}

void ispirit_end(CHAR_DATA *ch, [[maybe_unused]] AFFECT_DATA *af)
{
	if (ch->hit < 0)
	{
		act("Your body collapses under the strain of your spiritual exertion.", ch, nullptr, nullptr, TO_CHAR);
		act("$n's body collapses under the strain of $s spiritual exertion!", ch, nullptr, nullptr, TO_ROOM);
		act("$n is DEAD!!", ch, nullptr, nullptr, TO_ROOM);
		raw_kill(ch, ch);
	}
}

void spell_altruism(int /* level */, int /* sn */, CHAR_DATA *ch, SpellTarget vo, CastMode /* mode */)
{
	CHAR_DATA *vict = vo.AsChar();

	if (ch == vict)
	{
		send_to_char("You can only commune this on others.\n\r", ch);
		return;
	}

	for (auto it = vict->affected.begin(); it != vict->affected.end(); )
	{
		auto next = std::next(it);

		if (skill_table[it->type].dispel & CAN_CLEANSE)
		{
			AFFECT_DATA to_af;
			init_affect(&to_af);
			to_af.owner = it->owner;

			// palloc (not talloc): the affect owns this string and frees it
			// with free_pstring; a temp-pool string would be freed wrong.
			if (it->name != nullptr)
				to_af.name = palloc_string(it->name);

			to_af.where = it->where;
			to_af.type = it->type;
			to_af.level = it->level;
			to_af.duration = it->duration;
			to_af.location = it->location;
			to_af.modifier = it->modifier;

			copy_vector(to_af.bitvector, it->bitvector);

			to_af.aftype = it->aftype;
			to_af.tick_fun = it->tick_fun;
			to_af.pulse_fun = it->pulse_fun;
			to_af.end_fun = it->end_fun;
			to_af.init_duration = it->init_duration;
			affect_to_char(ch, &to_af);
			affect_remove(vict, &*it);
		}

		it = next;
	}

	act("You absorb all of $N's maledictions.", ch, nullptr, vict, TO_CHAR);
	act("You feel cleansed as $n absorbs your maledictions.", ch, nullptr, vict, TO_VICT);
	act("$n absorbs all of $N's maledictions.", ch, nullptr, vict, TO_ROOM);
}
