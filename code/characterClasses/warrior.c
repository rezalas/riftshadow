/****************************************************************************/
/* Welcome to Warrior land.                                                 */
/****************************************************************************/
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <iterator>
#include "warrior.h"
#include "sorcerer.h"
#include "../act_comm.h"
#include "../comm.h"
#include "../devextra.h"
#include "../update.h"
#include "../act_move.h"
#include "../handler.h"
#include "../fight.h"
#include "../magic.h"
#include "../recycle.h"
#include "../db.h"
#include "../lookup.h"
#include "../tables.h"
#include "../skills.h"
#include "../newmem.h"
#include "../interp.h"
#include "../act_info.h"
#include "../act_obj.h"
#include "../chardef.h"
#include "../const.h"
#include "../material.h"
#include "../utility.h"

int style_lookup(char *argument)
{
	for (int i = 1; i < MAX_STYLE; i++)
	{
		if (!str_prefix(argument, style_table[i].name))
			return i;
	}

	return 0;
}

int style_count(long bits[])
{
	int count = 0;

	for (int i = 1; i < MAX_STYLE; i++)
	{
		if (IS_SET(bits, style_table[i].bit))
			count++;
	}

	return count;
}

void do_specialize(CHAR_DATA *ch, char *argument)
{
	int i, style = 0;
	char buf[MSL];

	if (is_npc(ch))
		return;

	if (ch->Class()->GetIndex() != CLASS_WARRIOR)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	if (!str_cmp(argument, ""))
	{
		send_to_char("You can choose from the following specializations:\n\r", ch);

		for (i = 1; i < MAX_STYLE; i++)
		{
			if (!IS_SET(ch->pcdata->styles, style_table[i].bit))
			{
				send_to_char(style_table[i].name, ch);
				send_to_char("\n\r", ch);
			}
		}

		return;
	}

	if (!ch->pcdata->special)
	{
		send_to_char("You have no available specializations.\n\r", ch);
		return;
	}

	style = style_lookup(argument);

	if (!style)
	{
		send_to_char("That's not a valid style to specialize in.\n\r", ch);
		return;
	}

	SET_BIT(ch->pcdata->styles, style_table[style].bit);
	ch->pcdata->special--;

	sprintf(buf, "You are now specialized in the %s fighting style.\n\r", style_table[style].name);
	send_to_char(buf, ch);

	group_add(ch, style_table[style].name, false);

	ch->pcdata->learned[skill_lookup(style_table[style].name)] = 2;

	if (ch->pcdata->style == 0)
		ch->pcdata->style = style;
}

void do_style(CHAR_DATA *ch, char *argument)
{
	int style = 0, skill, i;

	if (is_npc(ch) && !is_switched(ch))
		return;

	if (!str_cmp(argument, ""))
	{
		send_to_char("You know the following fighting styles:\n\r", ch);

		for (i = 1; i < MAX_STYLE; i++)
		{
			if (IS_SET(ch->pcdata->styles, style_table[i].bit))
			{
				send_to_char(style_table[i].name, ch);
				send_to_char("\n\r", ch);
			}
		}

		if (ch->pcdata->style)
			act("You are fighting in the style of the $t.", ch, style_table[ch->pcdata->style].name, 0, TO_CHAR);

		return;
	}

	style = style_lookup(argument);

	if (ch->pcdata->learned[skill_lookup(style_table[style].name)] < 2)
		ch->pcdata->learned[skill_lookup(style_table[style].name)] = 2;

	if (!style || !IS_SET(ch->pcdata->styles, style_table[style].bit))
	{
		send_to_char("You aren't familiar with any such style.\n\r", ch);
		return;
	}

	if (ch->fighting)
	{
		skill = get_skill(ch, gsn_integrate);

		if (skill < 2)
		{
			send_to_char("You don't know how to switch styles in combat.\n\r", ch);
			return;
		}

		if (number_percent() > skill)
		{
			send_to_char("You try to shift fighting styles, but blunder it.\n\r", ch);
			WAIT_STATE(ch, PULSE_VIOLENCE);
			check_improve(ch, gsn_integrate, false, 2);
			return;
		}

		check_improve(ch, gsn_integrate, true, 2);
	}

	if (style == ch->pcdata->style)
		act("You are already fighting in the $t style.", ch, style_table[style].name, 0, TO_CHAR);
	else
		act("You fluidly shift your fighting methods into the $t style.", ch, style_table[style].name, 0, TO_CHAR);

	WAIT_STATE(ch, PULSE_VIOLENCE);
	ch->pcdata->style = style;
}

void do_entrap(CHAR_DATA *ch, char *argument)
{
	float skill;
	CHAR_DATA *victim;
	OBJ_DATA *wield, *dual, *weapon;
	char buf[MAX_STRING_LENGTH];

	skill = get_skill(ch, gsn_entrap);

	if (skill == 0)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	victim = ch->fighting;

	if (victim == nullptr)
	{
		send_to_char("You must be in combat to entrap a foe's weapon!\n\r", ch);
		return;
	}

	if (is_wielded(ch, WEAPON_POLEARM, WIELD_PRIMARY)
		|| is_wielded(ch, WEAPON_SPEAR, WIELD_PRIMARY)
		|| is_wielded(ch, WEAPON_STAFF, WIELD_PRIMARY))
	{
		weapon = get_eq_char(victim, WEAR_WIELD);

		if (weapon != nullptr)
		{
			switch (weapon->value[0])
			{
				case WEAPON_SWORD:
					skill *= .8;
					break;
				case WEAPON_AXE:
					skill *= .8;
					break;
				case WEAPON_MACE:
					skill *= .8;
					break;
				case WEAPON_FLAIL:
					skill *= .7;
					break;
				case WEAPON_POLEARM:
					skill *= .6;
					break;
				case WEAPON_STAFF:
					skill *= .6;
					break;
				case WEAPON_SPEAR:
					skill *= .6;
					break;
				case WEAPON_WHIP:
					skill *= .5;
					break;
				case WEAPON_DAGGER:
					send_to_char("You cannot entrap a dagger.\n\r", ch);
					return;
				default:
					break;
			}

			wield = get_eq_char(ch, WEAR_WIELD);

			if (is_obj_stat(weapon, ITEM_NOREMOVE)
				|| is_obj_stat(weapon, ITEM_NODISARM)
				|| is_obj_stat(weapon, ITEM_NODROP))
			{
				sprintf(buf, "You entrap $N's %s beneath the haft of your %s but it won't budge!",
					weapon_name_lookup(weapon->value[0]),
					weapon_name_lookup(wield->value[0]));
				act(buf, ch, 0, victim, TO_CHAR);

				sprintf(buf, "$n entraps your %s beneath the haft of $s %s but it won't budge!",
					weapon_name_lookup(weapon->value[0]),
					weapon_name_lookup(wield->value[0]));
				act(buf, ch, 0, victim, TO_VICT);

				sprintf(buf, "$n entraps $N's %s beneath the haft of $s %s but it won't budge!",
					weapon_name_lookup(weapon->value[0]),
					weapon_name_lookup(wield->value[0]));

				act(buf, ch, 0, victim, TO_NOTVICT);
				WAIT_STATE(ch, PULSE_VIOLENCE * 2);
				return;
			}
			else if (number_percent() < skill)
			{
				sprintf(buf, "You entrap $N's %s beneath the haft of your %s and knock it to the ground!",
					weapon_name_lookup(weapon->value[0]),
					weapon_name_lookup(wield->value[0]));
				act(buf, ch, 0, victim, TO_CHAR);

				sprintf(buf, "$n entraps your %s beneath the haft of $s %s and knocks it to the ground!",
					weapon_name_lookup(weapon->value[0]),
					weapon_name_lookup(wield->value[0]));
				act(buf, ch, 0, victim, TO_VICT);

				sprintf(buf, "$n entraps $N's %s beneath the haft of $s %s and knocks it to the ground!",
					weapon_name_lookup(weapon->value[0]),
					weapon_name_lookup(wield->value[0]));
				act(buf, ch, 0, victim, TO_NOTVICT);
			}
			else
			{
				act("You fail to entrap $N's weapon.", ch, 0, victim, TO_CHAR);
				act("$n attempts to entrap your weapon with $s $t, but fails.", ch, weapon_name_lookup(wield->value[0]), victim, TO_VICT);
				act("$n attempts to entrap $N's weapon with $s $t, but fails.", ch, weapon_name_lookup(wield->value[0]), victim, TO_NOTVICT);
				WAIT_STATE(ch, PULSE_VIOLENCE * 2);
				return;
			}
		}
		else
		{
			send_to_char("Your foe is not wielding a weapon to entrap!\n\r", ch);
			return;
		}
	}
	else if (get_eq_char(ch, WEAR_WIELD) && get_eq_char(ch, WEAR_DUAL_WIELD))
	{
		weapon = get_eq_char(victim, WEAR_WIELD);

		if (weapon != nullptr)
		{
			switch (weapon->value[0])
			{
				case WEAPON_SWORD:
					skill *= .7;
					break;
				case WEAPON_AXE:
					skill *= .7;
					break;
				case WEAPON_MACE:
					skill *= .7;
					break;
				case WEAPON_FLAIL:
					skill *= .55;
					break;
				case WEAPON_POLEARM:
					skill *= .8;
					break;
				case WEAPON_STAFF:
					skill *= .8;
					break;
				case WEAPON_SPEAR:
					skill *= .8;
					break;
				case WEAPON_WHIP:
					skill *= .4;
					break;
				case WEAPON_DAGGER:
					send_to_char("You cannot entrap a dagger.\n\r", ch);
					return;
				default:
					break;
			}

			wield = get_eq_char(ch, WEAR_WIELD);

			if (is_obj_stat(weapon, ITEM_NOREMOVE)
				|| is_obj_stat(weapon, ITEM_FIXED)
				|| is_obj_stat(weapon, ITEM_NODISARM)
				|| is_affected(victim, gsn_unbreakable))
			{
				act("You catch $N's $t between your own weapons but it won't budge!", ch, weapon_name_lookup(weapon->value[0]), victim, TO_CHAR);
				act("$n catches your $t between $s two weapons but it won't budge!", ch, weapon_name_lookup(weapon->value[0]), victim, TO_VICT);
				act("$n catches $N's $t between $s two weapons but it won't budge!", ch, weapon_name_lookup(weapon->value[0]), victim, TO_NOTVICT);
				WAIT_STATE(ch, PULSE_VIOLENCE * 2);
				return;
			}
			else if (number_percent() < skill)
			{
				act("You catch $N's $t between your own weapons and with a rapid twist, jerk it free!", ch, weapon_name_lookup(weapon->value[0]), victim, TO_CHAR);
				act("$n catches your $t between $s two weapons, and with a sudden twist, sends it flying!", ch, weapon_name_lookup(weapon->value[0]), victim, TO_VICT);
				act("$n catches $N's $t between $s two weapons, and with a sudden twist, sends it flying!", ch, weapon_name_lookup(weapon->value[0]), victim, TO_NOTVICT);
			}
			else
			{
				act("You fail to entrap $N's weapon.", ch, 0, victim, TO_CHAR);
				act("$n attempts to entrap your weapon with $s two weapons, but fails.", ch, 0, victim, TO_VICT);
				act("$n attempts to entrap $N's weapon with $s two weapons, but fails.", ch, 0, victim, TO_NOTVICT);
				WAIT_STATE(ch, PULSE_VIOLENCE * 2);
				return;
			}
		}
		else
		{
			send_to_char("Your foe is not wielding a weapon to entrap!\n\r", ch);
			return;
		}
	}
	else
	{
		send_to_char("You cannot execute that move with your current armament.\n\r", ch);
		return;
	}

	obj_from_char(weapon);

	if (is_obj_stat(weapon, ITEM_NODROP) || is_obj_stat(weapon, ITEM_INVENTORY))
	{
		obj_to_char(weapon, victim);
	}
	else
	{
		obj_to_room(weapon, victim->in_room);

		if (is_npc(victim) && victim->wait == 0 && can_see_obj(victim, weapon))
			get_obj(victim, weapon, nullptr, true);
	}

	dual = get_eq_char(victim, WEAR_DUAL_WIELD);

	if (dual != nullptr)
	{
		unequip_char(victim, dual, false);
		act("You quickly swap $p into your primary hand.", victim, dual, 0, TO_CHAR);
		act("$n quickly swaps $p into $s primary hand.", victim, dual, 0, TO_ROOM);
		equip_char(victim, dual, WEAR_WIELD, false);
	}

	WAIT_STATE(ch, PULSE_VIOLENCE * 2);
}

void do_hobble(CHAR_DATA *ch, char *argument)
{
	int skill;
	CHAR_DATA *victim;
	OBJ_DATA *weapon;
	bool special = false;
	AFFECT_DATA af, af2;

	skill = get_skill(ch, gsn_hobble);

	if (skill == 0)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	victim = ch->fighting;

	if (victim == nullptr)
	{
		send_to_char("You must be in combat to do that!\n\r", ch);
		return;
	}

	if (victim->legs <= 0)
	{
		send_to_char("You cannot strike what does not exist.\n\r", ch);
		return;
	}
	else if (victim->legs <= 1)
	{
		send_to_char("They are already hobbled.\n\r", ch);
		return;
	}

	if (victim->size < ch->size - 1)
	{
		send_to_char("You cannot aim at such a small target.\n\r", ch);
		return;
	}

	if (number_percent() < .5 * skill)
		special = true;

	weapon = get_eq_char(ch, WEAR_WIELD);

	if (weapon != nullptr)
	{
		switch (weapon->value[0])
		{
			case WEAPON_MACE:
				act("You lower your mace near the ground and sweep at $N's legs!", ch, 0, victim, TO_CHAR);
				act("$n lowers $s mace near the ground and sweeps at your legs!", ch, 0, victim, TO_VICT);
				act("$n lowers $s mace near the ground and sweeps at $N's legs!", ch, 0, victim, TO_NOTVICT);
				break;
			case WEAPON_AXE:
				act("Pivoting suddenly, you bring your axe down in an arc towards $N's feet!", ch, 0, victim, TO_CHAR);
				act("Pivoting suddenly, $n brings $s axe down in an arc towards your feet!", ch, 0, victim, TO_VICT);
				act("Pivoting suddenly, $n brings $s axe down in an arc towards $N's feet!", ch, 0, victim, TO_NOTVICT);
				break;
			case WEAPON_SWORD:
				act("You drop your blade under $N's guard and strike at $S legs!", ch, 0, victim, TO_CHAR);
				act("$n drops $s blade under your guard and strikes at your legs!", ch, 0, victim, TO_VICT);
				act("$n drops $s blade under $N's guard and strikes at $S legs!", ch, 0, victim, TO_NOTVICT);
				break;
			case WEAPON_FLAIL:
				act("Lashing out with your flail, you aim a sweeping blow at $N's legs!", ch, 0, victim, TO_CHAR);
				act("Lashing out with $s flail, $n aims a sweeping blow at your legs!", ch, 0, victim, TO_VICT);
				act("Lashing out with $s flail, $n aims a sweeping blow at $N's legs!", ch, 0, victim, TO_NOTVICT);
				break;
			case WEAPON_STAFF:
				act("Twirling your staff rapidly, you unleash a snapping strike at $N's feet!", ch, 0, victim, TO_CHAR);
				act("Twirling $s staff rapidly, $n unleashes a snapping strike at your feet!", ch, 0, victim, TO_VICT);
				act("Twirling $s staff rapidly, $n unleashes a snapping strike at $N's feet!", ch, 0, victim, TO_NOTVICT);
				break;
			case WEAPON_SPEAR:
				act("Stabbing suddenly, you aim a precise strike at $N's calf!", ch, 0, victim, TO_CHAR);
				act("Stabbing suddenly, $n aims a precise strike at your calf!", ch, 0, victim, TO_VICT);
				act("Stabbing suddenly, $n aims a precise strike at $N's calf!", ch, 0, victim, TO_NOTVICT);
				break;
			case WEAPON_DAGGER:
				act("You circle around $N and make a sweeping slash at $S calves!", ch, 0, victim, TO_CHAR);
				act("$n circles around you and makes a sweeping slash at your calves!", ch, 0, victim, TO_VICT);
				act("$n circles around $N and makes a sweeping slash at $S calves!", ch, 0, victim, TO_NOTVICT);
				break;
			default:
				send_to_char("You cannot properly strike your opponent's legs with that.\n\r", ch);
				return;
		}
	}
	else
	{
		send_to_char("You cannot execute that move barehanded!\n\r", ch);
		return;
	}

	if (number_percent() < .75 * skill)
	{
		if (special)
		{
			init_affect(&af);
			af.where = TO_AFFECTS;
			af.duration = ch->level / 8;
			af.type = gsn_hobble;
			af.level = ch->level;
			af.aftype = AFT_MALADY;

			switch (weapon->value[0])
			{
				case WEAPON_MACE:
					act("You hear the chilling sound of snapping bone as you land a solid blow.", ch, 0, victim, TO_CHAR);
					act("You hear the chilling sound of snapping bone as $n lands a solid blow.", ch, 0, victim, TO_ROOM);

					af.location = APPLY_DEX;
					af.modifier = -(ch->level / 6);
					af.aftype = AFT_MALADY;
					affect_join(victim, &af);

					victim->move = std::max(victim->move / 2, 0);
					break;
				case WEAPON_AXE:
					act("$N's leg buckles beneath $m as your slash tears deep into muscle!", ch, 0, victim, TO_CHAR);
					act("Your leg buckles beneath you as $n's slash tears deep into muscle.", ch, 0, victim, TO_VICT);
					act("$N's leg buckles beneath $m as $n's slash tears deep into muscle.", ch, 0, victim, TO_NOTVICT);

					af.location = APPLY_DEX;
					af.modifier = -(ch->level / 8);
					af.aftype = AFT_MALADY;
					affect_join(victim, &af);

					init_affect(&af2);
					af2.where = TO_AFFECTS;
					af2.duration = ch->level / 15;
					af2.type = gsn_bleeding;
					af2.level = ch->level;
					af2.location = APPLY_NONE;
					af2.modifier = 0;
					af2.aftype = AFT_INVIS;
					af2.tick_fun = bleeding_tick;
					af2.end_fun = nullptr;
					af2.owner = ch;
					new_affect_to_char(victim, &af2);
					break;
				case WEAPON_SWORD:
					act("$N's leg buckles beneath $m as your slash tears deep into muscle!", ch, 0, victim, TO_CHAR);
					act("Your leg buckles beneath you as $n's slash tears deep into muscle.", ch, 0, victim, TO_VICT);
					act("$N's leg buckles beneath $m as $n's slash tears deep into muscle.", ch, 0, victim, TO_NOTVICT);

					af.location = APPLY_DEX;
					af.modifier = -(ch->level / 8);
					af.aftype = AFT_MALADY;
					affect_join(victim, &af);

					init_affect(&af2);
					af2.where = TO_AFFECTS;
					af2.duration = ch->level / 15;
					af2.type = gsn_bleeding;
					af2.level = ch->level;
					af2.location = APPLY_NONE;
					af2.modifier = 0;
					af2.aftype = AFT_INVIS;
					af2.tick_fun = bleeding_tick;
					af2.end_fun = nullptr;
					af2.owner = ch;
					new_affect_to_char(victim, &af2);
					break;
				case WEAPON_FLAIL:
					act("You hear the chilling sound of snapping bone as you land a solid blow.", ch, 0, victim, TO_CHAR);
					act("You hear the chilling sound of snapping bone as $n lands a solid blow.", ch, 0, victim, TO_ROOM);

					af.location = APPLY_DEX;
					af.modifier = -(ch->level / 6);
					af.aftype = AFT_MALADY;
					affect_join(victim, &af);

					victim->move = std::max(victim->move / 2, 0);
					break;
				case WEAPON_STAFF:
					act("You hear the chilling sound of snapping bone as you land a solid blow.", ch, 0, victim, TO_CHAR);
					act("You hear the chilling sound of snapping bone as $n lands a solid blow.", ch, 0, victim, TO_ROOM);

					af.location = APPLY_DEX;
					af.modifier = -(ch->level / 6);
					af.aftype = AFT_MALADY;
					affect_join(victim, &af);

					victim->move = std::max(victim->move / 2, 0);
					break;
				case WEAPON_SPEAR:
					act("$N falters, clutching $S leg in agony as your spear pierces $M clean through!", ch, 0, victim, TO_CHAR);
					act("You falter, clutching your leg in agony as $n's spear pierces you clean through!", ch, 0, victim, TO_VICT);
					act("$N falters, clutching $S leg in agony as $n's spear pierces $M clean through!", ch, 0, victim, TO_NOTVICT);

					af.location = APPLY_DEX;
					af.modifier = -(ch->level / 7);
					af.aftype = AFT_MALADY;
					affect_join(victim, &af);

					victim->move = std::max(victim->move - ch->level / 2, 0);
					LAG_CHAR(victim, PULSE_VIOLENCE * 2);
					break;
				case WEAPON_DAGGER:
					act("$N collapses to the ground in agony as you sever a tendon!", ch, 0, victim, TO_CHAR);
					act("You collapse to the ground in agony as $n severs a tendon!", ch, 0, victim, TO_VICT);
					act("$N collapses to the ground in agony as $n severs a tendon!", ch, 0, victim, TO_NOTVICT);

					af.location = APPLY_DEX;
					af.modifier = -(ch->level / 5);
					af.aftype = AFT_MALADY;
					affect_join(victim, &af);

					LAG_CHAR(victim, PULSE_VIOLENCE * 2);
					break;
			}

			init_affect(&af);
			af.where = TO_AFFECTS;
			af.duration = ch->level / 8;
			af.type = gsn_hobble;
			af.level = ch->level;
			af.location = APPLY_LEGS;
			af.modifier = -1;
			af.aftype = AFT_INVIS;
			affect_to_char(victim, &af);

			one_hit_new(ch, victim, gsn_hobble, HIT_SPECIALS, HIT_UNBLOCKABLE, HIT_NOADD, 120, nullptr);
		}
		else
		{
			one_hit_new(ch, victim, gsn_hobble, HIT_SPECIALS, HIT_UNBLOCKABLE, HIT_NOADD, 80, nullptr);

			if (!is_affected(victim, gsn_hobble))
			{
				init_affect(&af);
				af.where = TO_AFFECTS;
				af.duration = ch->level / 8;
				af.type = gsn_hobble;
				af.level = ch->level;
				af.location = APPLY_DEX;

				switch (weapon->value[0])
				{
					case WEAPON_MACE:
						af.modifier = -(ch->level / 10);
						break;
					case WEAPON_AXE:
						af.modifier = -(ch->level / 12);
						break;
					case WEAPON_SWORD:
						af.modifier = -(ch->level / 12);
						break;
					case WEAPON_FLAIL:
						af.modifier = -(ch->level / 10);
						break;
					case WEAPON_STAFF:
						af.modifier = -(ch->level / 10);
						break;
					case WEAPON_SPEAR:
						af.modifier = -(ch->level / 12);
						break;
					case WEAPON_DAGGER:
						af.modifier = -(ch->level / 8);
						break;
				}

				af.aftype = AFT_MALADY;
				affect_to_char(victim, &af);
			}
		}

		WAIT_STATE(ch, PULSE_VIOLENCE * 2);
		check_improve(ch, gsn_hobble, true, 1);
	}
	else
	{
		damage(ch, victim, 0, gsn_hobble, DAM_NONE, true);
		WAIT_STATE(ch, PULSE_VIOLENCE * 2);
		check_improve(ch, gsn_hobble, false, 1);
	}
}

void do_crippling_blow(CHAR_DATA *ch, char *argument)
{
	int skill;
	CHAR_DATA *victim;
	OBJ_DATA *weapon;
	bool special = false;
	AFFECT_DATA af, af2;

	skill = get_skill(ch, gsn_crippling_blow);

	if (skill == 0)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	victim = ch->fighting;

	if (victim == nullptr)
	{
		send_to_char("You must be in combat to do that!\n\r", ch);
		return;
	}

	if (victim->arms <= 0)
	{
		send_to_char("Your foe has no arms for to you attempt to cripple.\n\r", ch);
		return;
	}
	else if (victim->arms <= 1)
	{
		send_to_char("They are already crippled.\n\r", ch);
		return;
	}
	else if (is_affected(victim, gsn_crippling_blow) && is_affected(victim, gsn_bleeding))
	{
		send_to_char("They are already crippled.\n\r", ch);
		return;
	}

	if (victim->size > ch->size + 1)
	{
		send_to_char("You cannot aim at such a large target.\n\r", ch);
		return;
	}

	if (number_percent() < .5 * skill)
		special = true;

	weapon = get_eq_char(ch, WEAR_WIELD);

	if (weapon != nullptr)
	{
		switch (weapon->value[0])
		{
			case WEAPON_MACE:
				act("You take a measured swing at $N's exposed offhand arm with your mace!", ch, 0, victim, TO_CHAR);
				act("$n takes a measured swing at your exposed offhand arm with $s mace!", ch, 0, victim, TO_VICT);
				act("$n takes a measured swing at $N's exposed offhand arm with $s mace!", ch, 0, victim, TO_NOTVICT);
				break;
			case WEAPON_AXE:
				act("Cleaving through $N's defenses, you strike at $N's vulnerable arm!", ch, 0, victim, TO_CHAR);
				act("Cleaving through your defenses, $n strikes at your vulnerable arm!", ch, 0, victim, TO_VICT);
				act("Cleaving through $N's defenses, $n strikes at $N's vulnerable arm!", ch, 0, victim, TO_NOTVICT);
				break;
			case WEAPON_SWORD:
				act("With a compact horizontal slice, you strike at $N's exposed arm!", ch, 0, victim, TO_CHAR);
				act("With a compact horizontal slice, $n strikes at your exposed arm!", ch, 0, victim, TO_VICT);
				act("With a compact horizontal slice, $n strikes at $N's exposed arm!", ch, 0, victim, TO_NOTVICT);
				break;
			case WEAPON_FLAIL:
				act("With a sidearm sweep, you bring the head of your flail about to strike $N's arm!", ch, 0, victim, TO_CHAR);
				act("With a sidearm sweep, $n brings the head of $s flail about to strike your arm!", ch, 0, victim, TO_VICT);
				act("With a sidearm sweep, $n brings the head of $s flail about to strike $N's arm!", ch, 0, victim, TO_NOTVICT);
				break;
			case WEAPON_STAFF:
				act("Using your right hand as a fulcrum, you swat suddenly at $N's exposed arm!", ch, 0, victim, TO_CHAR);
				act("Using $s right hand as a fulcrum, $n swats suddenly at your exposed arm!", ch, 0, victim, TO_VICT);
				act("Using $s right hand as a fulcrum, $n swats suddenly at $N's exposed arm!", ch, 0, victim, TO_NOTVICT);
				break;
			default:
				send_to_char("You cannot perform a crippling blow with that.\n\r", ch);
				return;
		}
	}
	else
	{
		send_to_char("You cannot execute that move barehanded!\n\r", ch);
		return;
	}

	if (number_percent() < .75 * skill)
	{
		if (special)
		{
			init_affect(&af);
			af.where = TO_AFFECTS;
			af.duration = ch->level / 8;
			af.type = gsn_crippling_blow;
			af.level = ch->level;
			af.aftype = AFT_MALADY;

			switch (weapon->value[0])
			{
				case WEAPON_MACE:
					act("You hear the sharp crack of splintering bone as $N's arm dangles loosely at $S side!", ch, 0, victim, TO_CHAR);
					act("You hear the sharp crack of splintering bone as your arm dangles loosely at your side!", ch, 0, victim, TO_VICT);
					act("You hear the sharp crack of splintering bone as $N's arm dangles loosely at $S side!", ch, 0, victim, TO_NOTVICT);

					af.location = APPLY_STR;
					af.modifier = -(ch->level / 7);
					af.aftype = AFT_MALADY;
					affect_join(victim, &af);

					init_affect(&af);
					af.where = TO_AFFECTS;
					af.duration = ch->level / 8;
					af.type = gsn_crippling_blow;
					af.level = ch->level;
					af.location = APPLY_ARMS;
					af.modifier = -1;
					af.aftype = AFT_INVIS;
					affect_to_char(victim, &af);
					break;
				case WEAPON_AXE:
					act("Your axe rends through flesh and muscle, biting deep into $N's arm!", ch, 0, victim, TO_CHAR);
					act("$n's axe rends through flesh and muscle, biting deep into your arm!", ch, 0, victim, TO_VICT);
					act("$n's axe rends through flesh and muscle, biting deep into $N's arm!", ch, 0, victim, TO_NOTVICT);

					af.location = APPLY_DEX;
					af.modifier = -(ch->level / 8);
					af.aftype = AFT_MALADY;
					affect_join(victim, &af);

					init_affect(&af2);
					af2.where = TO_AFFECTS;
					af2.duration = ch->level / 15;
					af2.type = gsn_bleeding;
					af2.level = ch->level;
					af2.location = APPLY_NONE;
					af2.modifier = 0;
					af2.aftype = AFT_INVIS;
					af2.tick_fun = bleeding_tick;
					af2.end_fun = nullptr;
					af2.owner = ch;
					new_affect_to_char(victim, &af2);
					break;
				case WEAPON_SWORD:
					act("Your blade sinks deep into $N's flesh as blood runs down $S forearm!", ch, 0, victim, TO_CHAR);
					act("$n's blade sinks deep into your flesh as blood runs down your forearm!", ch, 0, victim, TO_VICT);
					act("$n's blade sinks deep into $N's flesh as blood runs down $S forearm!", ch, 0, victim, TO_NOTVICT);

					af.location = APPLY_DEX;
					af.modifier = -(ch->level / 8);
					af.aftype = AFT_MALADY;
					affect_join(victim, &af);

					init_affect(&af2);
					af2.where = TO_AFFECTS;
					af2.duration = ch->level / 10;
					af2.type = gsn_bleeding;
					af2.level = ch->level;
					af2.location = APPLY_NONE;
					af2.modifier = 0;
					af2.aftype = AFT_INVIS;
					af2.tick_fun = bleeding_tick;
					af2.end_fun = nullptr;
					af2.owner = ch;
					new_affect_to_char(victim, &af2);
					break;
				case WEAPON_FLAIL:
					act("You hear the sharp crack of splintering bone as $N's arm dangles loosely at $S side!", ch, 0, victim, TO_CHAR);
					act("You hear the sharp crack of splintering bone as your arm dangles loosely at your side!", ch, 0, victim, TO_VICT);
					act("You hear the sharp crack of splintering bone as $N's arm dangles loosely at $S side!", ch, 0, victim, TO_NOTVICT);

					af.location = APPLY_STR;
					af.modifier = -(ch->level / 7);
					af.aftype = AFT_MALADY;
					affect_join(victim, &af);

					init_affect(&af);
					af.where = TO_AFFECTS;
					af.duration = ch->level / 8;
					af.type = gsn_crippling_blow;
					af.level = ch->level;
					af.location = APPLY_ARMS;
					af.modifier = -1;
					af.aftype = AFT_INVIS;
					affect_to_char(victim, &af);
					break;
				case WEAPON_STAFF:
					act("You hear the sharp crack of splintering bone as $N's arm dangles loosely at $S side!", ch, 0, victim, TO_CHAR);
					act("You hear the sharp crack of splintering bone as your arm dangles loosely at your side!", ch, 0, victim, TO_VICT);
					act("You hear the sharp crack of splintering bone as $N's arm dangles loosely at $S side!", ch, 0, victim, TO_NOTVICT);

					af.location = APPLY_STR;
					af.modifier = -(ch->level / 7);
					af.aftype = AFT_MALADY;
					affect_join(victim, &af);

					init_affect(&af);
					af.where = TO_AFFECTS;
					af.duration = ch->level / 8;
					af.type = gsn_crippling_blow;
					af.level = ch->level;
					af.location = APPLY_ARMS;
					af.modifier = -1;
					af.aftype = AFT_INVIS;
					affect_to_char(victim, &af);
					break;
			}

			one_hit_new(ch, victim, gsn_crippling_blow, HIT_SPECIALS, HIT_UNBLOCKABLE, HIT_NOADD, 120, nullptr);
		}
		else
		{
			one_hit_new(ch, victim, gsn_crippling_blow, HIT_SPECIALS, HIT_UNBLOCKABLE, HIT_NOADD, 80, nullptr);

			if (!is_affected(victim, gsn_crippling_blow))
			{
				init_affect(&af);
				af.where = TO_AFFECTS;
				af.duration = ch->level / 8;
				af.type = gsn_crippling_blow;
				af.level = ch->level;
				af.location = APPLY_STR;

				switch (weapon->value[0])
				{
					case WEAPON_MACE:
						af.modifier = -(ch->level / 7);
						break;
					case WEAPON_AXE:
						af.modifier = -(ch->level / 5);
						break;
					case WEAPON_SWORD:
						af.modifier = -(ch->level / 6);
						break;
					case WEAPON_FLAIL:
						af.modifier = -(ch->level / 7);
						break;
					case WEAPON_STAFF:
						af.modifier = -(ch->level / 7);
						break;
				}

				af.aftype = AFT_MALADY;
				affect_to_char(victim, &af);
			}
		}

		check_improve(ch, gsn_crippling_blow, true, 1);
	}
	else
	{
		damage(ch, victim, 0, gsn_crippling_blow, DAM_NONE, true);
		check_improve(ch, gsn_crippling_blow, false, 1);
	}

	WAIT_STATE(ch, PULSE_VIOLENCE * 2);
}

void do_gouge(CHAR_DATA *ch, char *argument)
{
	char hit1[MSL], hit2[MSL], hit3[MSL], miss1[MSL], miss2[MSL], miss3[MSL];
	int dammod, affect;
	float skill;
	CHAR_DATA *victim;
	OBJ_DATA *weapon;
	AFFECT_DATA af;

	skill = get_skill(ch, gsn_gouge);

	if (skill == 0)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	victim = ch->fighting;

	if (victim == nullptr)
	{
		send_to_char("You must be in combat to do that!\n\r", ch);
		return;
	}

	if (victim->size > ch->size + 1)
	{
		send_to_char("You cannot aim at such a large target.\n\r", ch);
		return;
	}

	weapon = get_eq_char(ch, WEAR_WIELD);

	if (weapon != nullptr)
	{
		switch (weapon->value[0])
		{
			case WEAPON_WHIP:
				sprintf(hit1, "You lunge forward and strike $N squarely in the eyes with the handle of your whip!");
				sprintf(hit2, "$n lunges forward and strikes you squarely in the eyes with the handle of $s whip!");
				sprintf(hit3, "$n lunges forward and strikes $N squarely in the eyes with the handle of $s whip!");
				sprintf(miss1, "You attempt to gouge $N's eyes with the handle of your whip, but miss.");
				sprintf(miss2, "$n attempts to gouge your eyes with the handle of $s whip, but misses.");
				sprintf(miss3, "$n attempts to gouge $N's eyes with the handle of $s whip, but misses.");

				skill *= .8;
				dammod = 25;
				affect = 2;
				break;
			case WEAPON_DAGGER:
				sprintf(hit1, "You strike $N right between the eyes with the handle of your dagger!");
				sprintf(hit2, "$n strikes you right between the eyes with the handle of $s dagger!");
				sprintf(hit3, "$n strikes $N right between the eyes with the handle of $s dagger!");
				sprintf(miss1, "Reversing your dagger to strike $N's eyes, you narrowly miss your mark.");
				sprintf(miss2, "Reversing $s dagger to strike your eyes, $n narrowly misses $s mark.");
				sprintf(miss3, "Reversing $s dagger to strike $N's eyes, $n narrowly misses $s mark.");

				skill *= .8;
				dammod = 25;
				affect = 2;
				break;
			case WEAPON_MACE:
				sprintf(hit1, "Raising your mace overhead, you bring the haft down on $N's face!");
				sprintf(hit2, "Raising $s mace overhead, $n brings the haft down on your face!");
				sprintf(hit3, "Raising $s mace overhead, $n brings the haft down on $N's face!");
				sprintf(miss1, "Swinging awkwardly, you fail to strike $N's eyes with the haft of your mace.");
				sprintf(miss2, "Swinging awkwardly, $n fails to strike your eyes with the haft of $s mace.");
				sprintf(miss3, "Swinging awkwardly, $n fails to strike $N's eyes with the haft of $s mace.");

				skill *= .6;
				dammod = 75;
				affect = 1;
				break;
			case WEAPON_SWORD:
				sprintf(hit1, "You strike $N in the face with the hilt of your blade, rapidly swelling $S eyes shut.");
				sprintf(hit2, "$n strikes you in the face with the hilt of $s blade, rapidly swelling your eyes shut.");
				sprintf(hit3, "$n strikes $N in the face with the hilt of $s blade, rapidly swelling $S eyes shut.");
				sprintf(miss1, "$N leans backward, evading your attempt to strike $S eyes.");
				sprintf(miss2, "You lean backward, evading $n's attempt to strike your eyes.");
				sprintf(miss3, "$N leans backward, evading $n's attempt to strike $S eyes.");

				skill *= .6;
				dammod = 75;
				affect = 1;
				break;
			case WEAPON_STAFF:
				sprintf(hit1, "Twirling your staff perpendicular to the ground, you strike $N squarely in the eyes!");
				sprintf(hit2, "Twirling $s staff perpendicular to the ground, $n strikes you squarely in the eyes!");
				sprintf(hit3, "Twirling $s staff perpendicular to the ground, $n strikes $N squarely in the eyes!");
				sprintf(miss1, "You spin your staff impressively, but fail to connect with $N's face.");
				sprintf(miss2, "$n spins $s staff impressively, but fails to connect with your face.");
				sprintf(miss3, "$n spins $s staff impressively, but fails to connect with $N's face.");

				skill *= .66;
				dammod = 60;
				affect = 1;
				break;
			case WEAPON_SPEAR:
				sprintf(hit1, "You jab $N in the eyes with the butt of your spear, blinding $M!");
				sprintf(hit2, "$n jabs you in the eyes with the butt of $s spear, blinding you!");
				sprintf(hit3, "$n jabs $N in the eyes with the butt of $s spear, blinding $M!");
				sprintf(miss1, "You jab at $N's eyes with the butt of your spear, but miss.");
				sprintf(miss2, "$n jabs at your eyes with the butt of $s spear, but misses.");
				sprintf(miss3, "$n jabs at $N's eyes with the butt of $s spear, but misses.");

				skill *= .66;
				dammod = 60;
				affect = 1;
				break;
			default:
				send_to_char("You cannot perform this attack with your weapon!\n\r", ch);
				return;
		}
	}
	else
	{
		sprintf(hit1, "You land a solid blow to $N's face, digging your thumb into $S eye!");
		sprintf(hit2, "$n lands a solid blow to your face, digging $s thumb into your eye!");
		sprintf(hit3, "$n lands a solid blow to $N's face, digging $s thumb into $S eye!");
		sprintf(miss1, "You swing wildly at $N's face, but miss.");
		sprintf(miss2, "$n swings wildly at your face, but misses.");
		sprintf(miss3, "$n swings wildly at $N's face, but misses.");

		skill *= .8;
		dammod = 100;
		affect = 2;
	}

	if (number_percent() < skill)
	{

		act(hit1, ch, 0, victim, TO_CHAR);
		act(hit2, ch, 0, victim, TO_VICT);
		act(hit3, ch, 0, victim, TO_NOTVICT);

		if (!is_affected_by(victim, AFF_BLIND))
		{
			init_affect(&af);
			af.where = TO_AFFECTS;
			af.duration = affect;

			SET_BIT(af.bitvector, AFF_BLIND);

			af.type = gsn_gouge;
			af.level = ch->level;
			af.location = APPLY_NONE;
			af.modifier = 0;
			af.aftype = AFT_MALADY;
			af.mod_name = MOD_VISION;
			affect_to_char(victim, &af);
		}

		one_hit_new(ch, victim, gsn_gouge, HIT_SPECIALS, HIT_UNBLOCKABLE, HIT_NOADD, dammod, "gouge");
		check_improve(ch, gsn_gouge, true, 1);
	}
	else
	{
		act(miss1, ch, 0, victim, TO_CHAR);
		act(miss2, ch, 0, victim, TO_VICT);
		act(miss3, ch, 0, victim, TO_NOTVICT);

		damage(ch, victim, 0, gsn_gouge, DAM_NONE, true);
		check_improve(ch, gsn_gouge, false, 1);
	}

	WAIT_STATE(ch, PULSE_VIOLENCE * 2);
}

void do_bleed(CHAR_DATA *ch, char *argument)
{
	char hit1[MSL], hit2[MSL], hit3[MSL], miss1[MSL], miss2[MSL], miss3[MSL];
	int skill, dammod, affect;
	CHAR_DATA *victim;
	OBJ_DATA *weapon;
	AFFECT_DATA af;

	skill = get_skill(ch, gsn_bleed);

	if (skill == 0)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	victim = ch->fighting;

	if (victim == nullptr)
	{
		send_to_char("You must be in combat to do that!\n\r", ch);
		return;
	}

	weapon = get_eq_char(ch, WEAR_WIELD);

	if (weapon != nullptr)
	{
		switch (weapon->value[0])
		{
			case WEAPON_DAGGER:
				sprintf(hit1, "Blood fountains from $N's artery as you make a jagged cut with your dagger!");
				sprintf(hit2, "Blood fountains from your artery as $n makes a jagged cut with $s dagger!");
				sprintf(hit3, "Blood fountains from $N's artery as $n makes a jagged cut with $s dagger!");
				sprintf(miss1, "You lunge forward, attempting to strike $N's artery, but are repulsed.");
				sprintf(miss2, "$n lunges forward, attempting to strike your artery, but is repulsed.");
				sprintf(miss3, "$n lunges forward, attempting to strike $N's artery, but is repulsed.");

				dammod = 125;
				affect = ch->level / 3;
				break;
			case WEAPON_SWORD:
				sprintf(hit1, "Rivulets of blood run to the ground as your blade finds $N's artery!");
				sprintf(hit2, "Rivulets of blood run to the ground as $n's blade finds your artery!");
				sprintf(hit3, "Rivulets of blood run to the ground as $n's blade finds $N's artery!");
				sprintf(miss1, "You slash at $N's exposed artery but fail to connect.");
				sprintf(miss2, "$n slashes at your exposed artery but fails to connect.");
				sprintf(miss3, "$n slashes at $N's exposed artery but fails to connect.");

				dammod = 100;
				affect = ch->level / 4;
				break;
			case WEAPON_AXE:
				sprintf(hit1, "You slightly twist your axe as it sinks into $N's flesh, and a torrent of blood gushes forth!");
				sprintf(hit2, "$n slightly twists $s axe as it sinks into your flesh, and a torrent of blood gushes forth!");
				sprintf(hit3, "$n slightly twists $s axe as it sinks into $N's flesh, and a torrent of blood gushes forth!");
				sprintf(miss1, "You swing your axe in a wide arc but fail to connect with $N.");
				sprintf(miss2, "$n swings $s axe in a wide arc but fails to connect with you.");
				sprintf(miss3, "$n swings $s axe in a wide arc but fails to connect with $N.");

				dammod = 125;
				affect = ch->level / 4;
				break;
			case WEAPON_SPEAR:
				sprintf(hit1, "You drive your spear deep into $N's flesh, blood spilling forth as you withdraw it!");
				sprintf(hit2, "$n drives $s spear deep into your flesh, blood spilling forth as $e withdraws it!");
				sprintf(hit3, "$n drives $s spear deep into $N's flesh, blood spilling forth as $e withdraws it!");
				sprintf(miss1, "You lunge powerfully forward with your spear, but fail to connect.");
				sprintf(miss2, "$n lunges powerfully forward towards you with $s spear, but fails to connect.");
				sprintf(miss3, "$n lunges powerfully forward towards $N with $s spear, but fails to connect.");

				dammod = 200;
				affect = ch->level / 7;
				break;
			default:
				send_to_char("You cannot perform this attack with your weapon!\n\r", ch);
				return;
		}
	}
	else
	{
		send_to_char("You cannot execute that move barehanded!\n\r", ch);
		return;
	}

	if (number_percent() < .65 * skill)
	{

		act(hit1, ch, 0, victim, TO_CHAR);
		act(hit2, ch, 0, victim, TO_VICT);
		act(hit3, ch, 0, victim, TO_NOTVICT);

		if (!is_affected(victim, gsn_bleeding))
		{
			init_affect(&af);
			af.where = TO_AFFECTS;
			af.duration = affect;
			af.type = gsn_bleeding;
			af.level = ch->level;
			af.location = APPLY_NONE;
			af.modifier = 0;
			af.aftype = AFT_MALADY;
			af.end_fun = nullptr;
			af.tick_fun = bleeding_tick;
			af.owner = ch;
			new_affect_to_char(victim, &af);
		}

		one_hit_new(ch, victim, gsn_bleed, HIT_SPECIALS, HIT_UNBLOCKABLE, HIT_NOADD, dammod, nullptr);
		check_improve(ch, gsn_bleed, true, 1);
	}
	else
	{
		act(miss1, ch, 0, victim, TO_CHAR);
		act(miss2, ch, 0, victim, TO_VICT);
		act(miss3, ch, 0, victim, TO_NOTVICT);

		damage(ch, victim, 0, gsn_bleed, DAM_NONE, true);
		check_improve(ch, gsn_bleed, false, 1);
	}

	WAIT_STATE(ch, PULSE_VIOLENCE * 2);
}

int check_posture(CHAR_DATA *ch)
{
	AFFECT_DATA *paf;
	int modifier = POSTURE_NONE;

	for (paf = ch->affected; paf != nullptr; paf = paf->next)
	{
		if (paf->type == gsn_posture)
		{
			modifier = paf->modifier;
			break;
		}
	}

	return modifier;
}

void do_posture(CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;
	int type;

	if (argument[0] == '\0')
	{
		send_to_char("Syntax: posture neutral\n\r        posture offense\n\r        posture defense\n\r", ch);

		if (check_posture(ch) == POSTURE_OFFENSE)
			send_to_char("You are currently in an offensive stance.\n\r", ch);
		else if (check_posture(ch) == POSTURE_DEFENSE)
			send_to_char("You are currently in a defensive stance.\n\r", ch);
		else
			send_to_char("You are not currently focused on an offensive or defensive posture.\n\r", ch);

		return;
	}

	WAIT_STATE(ch, PULSE_VIOLENCE);

	if (!str_prefix(argument, "neutral"))
	{
		if (is_affected(ch, gsn_posture))
		{
			send_to_char("You no longer focus on offense or defense.\n\r", ch);
			affect_strip(ch, gsn_posture);
		}
		else
		{
			send_to_char("You are not currently focused on an offensive or defensive posture.\n\r", ch);
		}

		return;
	}
	else if (!str_prefix(argument, "offense"))
	{
		if (is_affected(ch, gsn_posture))
		{
			if (check_posture(ch) == POSTURE_OFFENSE)
			{
				send_to_char("You are already focused in your posture.\n\r", ch);
				return;
			}
			else
			{
				affect_strip(ch, gsn_posture);
			}
		}

		send_to_char("You now focus your posture on offense.\n\r", ch);
		type = POSTURE_OFFENSE;
	}
	else if (!str_prefix(argument, "defense"))
	{
		if (is_affected(ch, gsn_posture))
		{
			if (check_posture(ch) == POSTURE_DEFENSE)
			{
				send_to_char("You are already focused in your posture.\n\r", ch);
				return;
			}
			else
			{
				affect_strip(ch, gsn_posture);
			}
		}

		send_to_char("You now focus your posture on defense.\n\r", ch);
		type = POSTURE_DEFENSE;
	}
	else
	{
		send_to_char("Syntax: posture neutral\n\r        posture offense\n\r        posture defense\n\r", ch);
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_INVIS;
	af.type = gsn_posture;
	af.level = ch->level;
	af.duration = -1;
	af.modifier = type;
	af.location = APPLY_NONE;
	SET_BIT(af.bitvector, AFF_PERMANENT);
	af.pulse_fun = posture_pulse;
	affect_to_char(ch, &af);
}

void posture_pulse(CHAR_DATA *ch, AFFECT_DATA *af)
{
	if (style_table[ch->pcdata->style].bit != STYLE_DUELIST)
		affect_remove(ch, af);
}

void do_unbalance(CHAR_DATA *ch, char *argument)
{
	int skill;
	CHAR_DATA *victim;
	AFFECT_DATA af;

	skill = get_skill(ch, gsn_unbalance);
	if (skill == 0)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	victim = ch->fighting;

	if (victim == nullptr)
	{
		send_to_char("You must be in combat to do that!\n\r", ch);
		return;
	}

	if (is_affected(victim, gsn_unbalance))
	{
		act("$N is already unbalanced.", ch, 0, victim, TO_CHAR);
		return;
	}

	if (number_percent() < .9 * skill)
	{
		act("You lunge at $N and suddenly withdraw, throwing $M off balance!", ch, 0, victim, TO_CHAR);
		act("$n lunges at you and suddenly withdraws, throwing you off balance!", ch, 0, victim, TO_VICT);
		act("$n lunges at $N and suddenly withdraws, throwing $M off balance!", ch, 0, victim, TO_NOTVICT);

		if (victim->balance >= 40)
		{
			act("You stumble awkwardly and topple to the ground!", victim, 0, 0, TO_CHAR);
			act("$n stumbles awkwardly and topples to the ground!", victim, 0, 0, TO_ROOM);
			LAG_CHAR(victim, PULSE_VIOLENCE * 2);
		}
		else if (victim->balance >= 30)
		{
			act("You wobble unsteadily, struggling to stay on your feet.", victim, 0, 0, TO_CHAR);
			act("$n wobbles unsteadily, struggling to stay on $s feet.", victim, 0, 0, TO_ROOM);
			LAG_CHAR(victim, PULSE_VIOLENCE);
		}

		victim->balance += 10;
		check_improve(ch, gsn_unbalance, true, 1);

		init_affect(&af);
		af.where = TO_AFFECTS;
		af.aftype = AFT_INVIS;
		af.type = gsn_unbalance;
		af.level = ch->level;
		af.duration = -1;
		af.modifier = 1;
		af.location = APPLY_NONE;
		affect_to_char(victim, &af);
	}
	else
	{
		act("You lunge at $N and suddenly withdraw, but $E doesn't fall for your feint.", ch, 0, victim, TO_CHAR);
		act("$n lunges at you and suddenly withdraws, but you don't fall for $s feint.", ch, 0, victim, TO_VICT);
		act("$n lunges at $N and suddenly withdraws, but $E doesn't fall for $s feint.", ch, 0, victim, TO_NOTVICT);
		check_improve(ch, gsn_unbalance, false, 1);
	}

	WAIT_STATE(ch, PULSE_VIOLENCE * 2);
}

void do_uppercut(CHAR_DATA *ch, char *argument)
{
	int success = 0, skill, special = 100, duration = ch->level;
	CHAR_DATA *victim = ch->fighting;
	AFFECT_DATA af;
	OBJ_DATA *weapon;
	char ch1[MSL], chspecial[MSL], vict1[MSL], victspecial[MSL], nv1[MSL], nvspecial[MSL];
	skill = get_skill(ch, skill_lookup("uppercut"));

	if (victim == nullptr)
	{
		send_to_char("You must be in combat do to that!\n\r", ch);
		return;
	}

	weapon = get_eq_char(ch, WEAR_WIELD);

	if (weapon && (weapon->weight >= 15))
	{
		switch (weapon->value[0])
		{
			case WEAPON_MACE:
				sprintf(ch1, "You bring your mace sweeping up from beneath $N's guard!");
				sprintf(chspecial, "Bone and cartilage shatter as your mace catches $N solidly beneath the jaw!");
				sprintf(vict1, "$n brings $s mace sweeping up beneath your guard!");
				sprintf(victspecial, "Bone and cartilage shatter as $n's mace catches you solidly beneath the jaw!");
				sprintf(nv1, "$n brings $s mace sweeping up beneath $N's guard!");
				sprintf(nvspecial, "Bone and cartilage shatter as $n's mace catches $N solidly beneath the jaw!");

				special = 110;
				duration = ch->level / 25;
				break;
			case WEAPON_FLAIL:
				sprintf(ch1, "You bring your flail sweeping up from beneath $N's guard!");
				sprintf(chspecial, "Bone and cartilage shatter as your flail catches $N solidly beneath the jaw!");
				sprintf(vict1, "$n brings $s flail sweeping up beneath your guard!");
				sprintf(victspecial, "Bone and cartilage shatter as $n's flail catches you solidly beneath the jaw!");
				sprintf(nv1, "$n brings $s flail sweeping up beneath $N's guard!");
				sprintf(nvspecial, "Bone and cartilage shatter as $n catches $N solidly beneath the jaw!");

				special = 110;
				duration = ch->level / 25;
				break;
			case WEAPON_STAFF:
				sprintf(ch1, "Twirling your staff, you bring it sweeping up beneath $N's guard!");
				sprintf(chspecial, "You catch $N squarely beneath the jaw, crushing bone and snapping $s head back!");
				sprintf(vict1, "$n brings $s staff up beneath your guard!");
				sprintf(victspecial, "You feel bone crack as your head is snapped back by $n's blow!");
				sprintf(nv1, "$n twirls $s staff, bringing it up beneath $N's guard!");
				sprintf(nvspecial, "Bone cracks as $N's head is snapped back by $n's blow!");

				special = 110;
				duration = ch->level / 25;
				break;
			case WEAPON_AXE:
				sprintf(ch1, "Your massive axe sings through the air, arcing beneath $N's guard!");
				sprintf(chspecial, "Your axe impacts with $N's jaw, met with the crunch of bone and a thick spray of blood!");
				sprintf(vict1, "$n's massive axe sings through the air, arcing beneath your guard!");
				sprintf(victspecial, "$n's axe solidly strikes your jaw, causing bone to crack and blood to spray!");
				sprintf(nv1, "$n's massive axe sings through the air, arcing beneath $N's guard!");
				sprintf(nvspecial, "$n's axe solidly strikes $N, causing bone to crack and blood to spray!");

				special = 150;
				duration = ch->level / 40;
				break;
			default:
				send_to_char("You cannot perform this attack with that weapon.\n\r", ch);
				return;
				break;
		}
	}
	else
	{
		if (weapon == nullptr)
		{
			send_to_char("You can't do that with your hands!\n\r", ch);
			return;
		}

		if (weapon->weight < 15)
		{
			send_to_char("Your weapon isn't heavy enough to perform an uppercut.\n\r", ch);
			return;
		}

		return;
	}

	if (number_percent() < .8 * skill)
	{
		success = 1;
		act(ch1, ch, 0, victim, TO_CHAR);
		act(vict1, ch, 0, victim, TO_VICT);
		act(nv1, ch, 0, victim, TO_NOTVICT);

		if (success == 1
			&& number_percent() < (.4 * skill * (1 + weapon->weight / 200) * (1 + (get_curr_stat(ch, STAT_STR) - 20) / 10)))
		{
			act(chspecial, ch, 0, victim, TO_CHAR);
			act(victspecial, ch, 0, victim, TO_VICT);
			act(nvspecial, ch, 0, victim, TO_NOTVICT);

			if (!is_affected(victim, gsn_uppercut))
			{
				init_affect(&af);
				af.where = TO_AFFECTS;
				af.level = ch->level;
				af.duration = duration;
				af.owner = ch;
				af.type = gsn_uppercut;
				af.aftype = AFT_MALADY;
				af.mod_name = MOD_SPEECH;
				affect_to_char(victim, &af);
			}
		}

		one_hit_new(ch, victim, TYPE_UNDEFINED, HIT_SPECIALS, HIT_UNBLOCKABLE, HIT_NOADD, special, "uppercut");
		check_improve(ch, gsn_uppercut, true, 1);
	}
	else
	{
		act(ch1, ch, 0, victim, TO_CHAR);
		act(vict1, ch, 0, victim, TO_VICT);
		act(nv1, ch, 0, victim, TO_NOTVICT);

		damage(ch, victim, 0, gsn_uppercut, DAM_NONE, true);
		check_improve(ch, gsn_uppercut, false, 1);

		WAIT_STATE(ch, PULSE_VIOLENCE * 3);
		return;
	}

	WAIT_STATE(ch, PULSE_VIOLENCE * 2);
}

int skirmisher_max_weapweight(CHAR_DATA *ch)
{
	if (is_npc(ch))
		return 15;

	if (get_curr_stat(ch, STAT_STR) > 23)
		return 14;

	if (get_curr_stat(ch, STAT_STR) > 21)
		return 12;

	if (get_curr_stat(ch, STAT_STR) > 20)
		return 11;

	return 10;
}

void do_dart(CHAR_DATA *ch, char *argument)
{
	OBJ_DATA *weapon;
	AFFECT_DATA af;
	CHAR_DATA *victim = ch->fighting;
	char cha[MSL], vict[MSL], nv[MSL];
	int skill = get_skill(ch, gsn_dart);

	if (victim == nullptr)
	{
		send_to_char("You must be fighting to perform this attack!\n\r", ch);
		return;
	}

	weapon = get_eq_char(ch, WEAR_WIELD);

	if (weapon == nullptr)
	{
		send_to_char("You must be wielding a weapon to perform this attack!\n\r", ch);
		return;
	}

	if (weapon->weight > skirmisher_max_weapweight(ch))
	{
		send_to_char("That weapon is too heavy to dart with.\n\r", ch);
		return;
	}

	init_affect(&af);

	switch (weapon->value[0])
	{
		case WEAPON_DAGGER:
			sprintf(cha, "You slash your dagger across $N's brow and blood runs into $S eyes!");
			sprintf(vict, "$n slashes $s dagger across your brow, causing blood to run into your eyes!");
			sprintf(nv, "$n slashes $s dagger across $N's brow and blood runs into $S eyes!");

			af.type = gsn_bloody_eyes;
			af.aftype = AFT_MALADY;
			af.mod_name = MOD_VISION;
			SET_BIT(af.bitvector, AFF_BLIND);
			af.location = APPLY_HITROLL;
			af.modifier = -ch->level / 12;
			af.duration = 1;
			break;
		case WEAPON_SWORD:
			sprintf(cha, "You plunge your sword between $N's ribs and blood spills forth!");
			sprintf(vict, "$n plunges $s sword between your ribs and blood spills forth!");
			sprintf(nv, "$n plunges $s sword between $N's ribs and blood spills forth!");

			af.type = gsn_bleeding;
			af.aftype = AFT_MALADY;
			af.duration = ch->level / 12;
			af.tick_fun = bleeding_tick;
			break;
		case WEAPON_MACE:
			sprintf(cha, "Twirling your mace adeptly, you deal a sharp blow to $N's forearm!");
			sprintf(vict, "Twirling $s mace adeptly, $n deals a sharp blow to your forearm!");
			sprintf(nv, "Twirling $s mace adeptly, $n deals a sharp blow to $N's forearm!");

			af.type = gsn_broken_arm;
			af.location = APPLY_STR;
			af.aftype = AFT_MALADY;
			af.modifier = -ch->level / 10;
			af.duration = ch->level / 10;
			break;
		case WEAPON_AXE:
			sprintf(cha, "You hear bone cracking as you drive your axe into $N's sternum!");
			sprintf(vict, "You hear bone cracking as $n drives $s axe into your sternum!");
			sprintf(nv, "You hear bone cracking as $n drives $s axe into $N's sternum!");

			af.type = gsn_cracked_sternum;
			af.aftype = AFT_MALADY;
			af.location = APPLY_DEX;
			af.modifier = -ch->level / 12;
			af.duration = ch->level / 10;
			break;
		default:
			send_to_char("You cannot execute a darting strike with that weapon.\n\r", ch);
			return;
	}

	if (number_percent() < (get_curr_stat(ch, STAT_DEX) + skill) * .80)
	{
		act("Moving nimbly, you dart in under $N's guard and strike!", ch, 0, victim, TO_CHAR);
		act("$n nimbly darts under your guard and strikes!", ch, 0, victim, TO_VICT);
		act(cha, ch, 0, victim, TO_CHAR);
		act(vict, ch, 0, victim, TO_VICT);
		act(nv, ch, 0, victim, TO_NOTVICT);

		af.where = TO_AFFECTS;
		af.level = ch->level;
		af.owner = ch;
		af.end_fun = nullptr;

		if (!is_affected(victim, af.type))
			new_affect_to_char(victim, &af);

		one_hit_new(ch, victim, TYPE_UNDEFINED, HIT_SPECIALS, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "darting strike");
		check_improve(ch, gsn_dart, true, 1);
	}
	else
	{
		act("Moving nimbly, you dart in under $N's guard and strike!", ch, 0, victim, TO_CHAR);
		act("$n nimbly darts under your guard and strikes!", ch, 0, victim, TO_VICT);
		damage(ch, victim, 0, gsn_dart, DAM_NONE, true);
		check_improve(ch, gsn_dart, false, 1);
	}

	WAIT_STATE(ch, PULSE_VIOLENCE * 2);
}

void do_impale(CHAR_DATA *ch, char *argument)
{
	OBJ_DATA *weapon;
	AFFECT_DATA af, iaf;
	CHAR_DATA *victim;
	char cha[MSL], vict[MSL], nv[MSL], break1[MSL], break2[MSL], buf[MSL];
	int skill = get_skill(ch, skill_lookup("impale")), chance;

	if (skill == 0)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	victim = ch->fighting;

	if (victim == nullptr)
	{
		if (argument[0] == '\0')
		{
			send_to_char("Impale who?\n\r", ch);
			return;
		}

		victim = get_char_room(ch, argument);

		if (victim == nullptr)
		{
			send_to_char("They aren't here.\n\r", ch);
			return;
		}
	}

	if (is_safe(ch, victim))
		return;

	weapon = get_eq_char(ch, WEAR_WIELD);

	if (!weapon)
	{
		send_to_char("Impale them on what?  Your fingernails?\n\r", ch);
		return;
	}

	if (weapon->weight < 15)
	{
		send_to_char("Your weapon is too light for you to execute that maneuver.\n\r", ch);
		return;
	}

	switch (weapon->value[0])
	{
		case WEAPON_SPEAR:
			sprintf(cha, "With a single powerful motion, you plunge your spear into $N's chest!");
			sprintf(vict, "$n drives towards you, plunging $s spear into your chest!");
			sprintf(nv, "$n drives towards $N, plunging $s spear into $N's chest!");
			sprintf(break1, "$N shrieks in agony as the spear snaps off in the gaping wound!");
			sprintf(break2, "You shriek in agony as the spear snaps off in your gaping wound!");
			break;
		case WEAPON_POLEARM:
			sprintf(cha, "With a single powerful motion, you plunge your polearm into $N's chest!");
			sprintf(vict, "$n drives towards you, plunging $s polearm into your chest!");
			sprintf(nv, "$n drives towards $N, plunging $s polearm into $N's chest!");
			sprintf(break1, "$N shrieks in agony as the polearm snaps off in the gaping wound!");
			sprintf(break2, "You shriek in agony as the polearm snaps off in your gaping wound!");
			break;
		default:
			return send_to_char("You can't impale someone with that!\n\r", ch);
			break;
	}

	if (!is_npc(victim) && !is_npc(ch) && (!ch->fighting || !victim->fighting))
	{
		sprintf(buf, "Help! %s is impaling me!", pers(ch, victim));
		do_myell(victim, buf, ch);
	}

	if (victim->position <= POS_RESTING)
		chance = skill;
	else
		chance = skill / 2;

	if (number_percent() < chance)
	{
		if (!is_affected(victim, gsn_impale) &&
			number_percent() < 20 - (material_table[weapon->pIndexData->material_index].mat_hardness * 5))
		{
			act(cha, ch, 0, victim, TO_CHAR);
			act(vict, ch, 0, victim, TO_VICT);
			act(nv, ch, 0, victim, TO_NOTVICT);
			act(break1, ch, 0, victim, TO_NOTVICT);
			act(break1, ch, 0, victim, TO_CHAR);
			act(break2, ch, 0, victim, TO_VICT);
			one_hit_new(ch, victim, TYPE_UNDEFINED, HIT_NOSPECIALS, HIT_UNBLOCKABLE, HIT_NOADD, 300, "impalement");

			if (!is_affected(victim, gsn_bleeding))
			{
				init_affect(&af);
				af.where = TO_AFFECTS;
				af.type = gsn_bleeding;
				af.aftype = AFT_MALADY;
				af.level = ch->level;
				af.duration = ch->level / 9;
				af.location = APPLY_STR;
				af.modifier = -ch->level / 8;
				af.owner = ch;
				new_affect_to_char(victim, &af);

				af.location = APPLY_DEX;
				af.tick_fun = bleeding_tick;
				new_affect_to_char(victim, &af);
			}

			init_affect(&iaf);
			iaf.where = TO_AFFECTS;
			iaf.type = gsn_impale;
			iaf.aftype = AFT_INVIS;
			iaf.duration = -1;
			iaf.level = ch->level;
			iaf.owner = ch;
			new_affect_to_char(victim, &iaf);

			extract_obj(weapon);
		}
		else
		{
			act(cha, ch, 0, victim, TO_CHAR);
			act(vict, ch, 0, victim, TO_VICT);
			act(nv, ch, 0, victim, TO_NOTVICT);
			one_hit_new(ch, victim, TYPE_UNDEFINED, HIT_NOSPECIALS, HIT_UNBLOCKABLE, HIT_NOADD, 200, "impalement");

			if (!is_affected(victim, gsn_bleeding))
			{
				init_affect(&af);
				af.where = TO_AFFECTS;
				af.type = gsn_bleeding;
				af.aftype = AFT_MALADY;
				af.level = ch->level;
				af.duration = ch->level / 8;
				af.location = APPLY_STR;
				af.modifier = -ch->level / 8;
				af.owner = ch;
				new_affect_to_char(victim, &af);

				af.location = APPLY_DEX;
				af.tick_fun = bleeding_tick;
				new_affect_to_char(victim, &af);
			}
		}

		check_improve(ch, gsn_impale, true, 1);
	}
	else
	{
		act("You make a ferocious lunge at $N with your weapon, but miss!", ch, 0, victim, TO_CHAR);
		act("$n makes a ferocious lunge at you with $s weapon, but misses!", ch, 0, victim, TO_VICT);
		act("$n makes a ferocious lunge at $N with $s weapon, but misses!", ch, 0, victim, TO_NOTVICT);

		damage_new(ch, victim, 0, TYPE_UNDEFINED, DAM_NONE, true, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "impalement");
		check_improve(ch, gsn_impale, false, 1);
	}

	WAIT_STATE(ch, PULSE_VIOLENCE * 2);
}

void do_hurl(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim;
	int skill, direction;
	char *dirname;
	EXIT_DATA *pexit = nullptr;
	ROOM_INDEX_DATA *to_room = nullptr;
	ROOM_INDEX_DATA *victroom = nullptr;
	char arg1[MSL], arg2[MSL], buf[MSL];
	OBJ_DATA *weapon, *dual;
	AFFECT_DATA af;

	skill = get_skill(ch, gsn_hurl);
	if (skill == 0)
		return send_to_char("Huh?\n\r", ch);

	weapon = get_eq_char(ch, WEAR_WIELD);

	if (weapon == nullptr)
	{
		send_to_char("You must be primary wielding the weapon you wish to hurl.\n\r", ch);
		return;
	}

	if (weapon->weight > skirmisher_max_weapweight(ch))
	{
		act("That $t is too heavy to be hurled properly.", ch, weapon_name_lookup(weapon->value[0]), 0, TO_CHAR);
		return;
	}

	if (is_obj_stat(weapon, ITEM_NOREMOVE) || is_obj_stat(weapon, ITEM_NODROP) || is_obj_stat(weapon, ITEM_INVENTORY))
	{
		send_to_char("You can't hurl that.\n\r", ch);
		return;
	}

	if (argument[0] == '\0')
	{
		send_to_char("Hurl your weapon at who?\n\r", ch);
		return;
	}

	if (is_wielded(ch, WEAPON_SWORD, WIELD_PRIMARY)
		|| is_wielded(ch, WEAPON_DAGGER, WIELD_PRIMARY)
		|| is_wielded(ch, WEAPON_AXE, WIELD_PRIMARY)
		|| is_wielded(ch, WEAPON_MACE, WIELD_PRIMARY))
	{
		half_chop(argument, arg1, arg2);
	}
	else
	{
		send_to_char("You can't hurl that.\n\r", ch);
		return;
	}

	direction = direction_lookup(arg2);

	if (direction == -1)
	{
		victim = get_char_room(ch, arg1);

		if (victim == nullptr)
		{
			send_to_char("They aren't here.\n\r", ch);
			return;
		}
	}
	else
	{
		pexit = ch->in_room->exit[direction];

		if (pexit == nullptr)
		{
			dirname = flag_name_lookup(direction, direction_table);
			act("You can't hurl your weapon $t.", ch, dirname, 0, TO_CHAR);
			return;
		}

		if (IS_SET(pexit->exit_info, EX_CLOSED))
		{
			dirname = flag_name_lookup(direction, direction_table);
			act("You can't hurl your weapon $t.", ch, dirname, 0, TO_CHAR);
			return;
		}

		to_room = pexit->u1.to_room;

		victim = get_char_from_room(ch, to_room, arg1);

		if (victim == nullptr)
		{
			dirname = flag_name_lookup(direction, direction_table);
			act("You can't see them $t.", ch, dirname, 0, TO_CHAR);
			return;
		}
	}

	victroom = victim->in_room;

	if (is_safe(ch, victim))
		return;

	if (is_affected(victim, gsn_hurl))
	{
		act("$N has taken cover and you can't get a clear shot at $M.", ch, 0, victim, TO_CHAR);
		return;
	}

	if (to_room)
	{
		dirname = flag_name_lookup(direction, direction_table);

		sprintf(buf, "Sighting $N in the distance %sward, you hurl your %s at $M!", dirname, weapon_name_lookup(weapon->value[0]));
		act(buf, ch, 0, victim, TO_CHAR);

		sprintf(buf, "$n suddenly hurls $s %s %sward!", weapon_name_lookup(weapon->value[0]), dirname);
		act(buf, ch, 0, victim, TO_ROOM);

		dirname = flag_name_lookup(reverse_d(direction), direction_table);

		sprintf(buf, "A %s comes hurtling in from the %s, flipping end over end!", weapon_name_lookup(weapon->value[0]), dirname);
		act(buf, victim, 0, 0, TO_ALL);

		if (!is_npc(victim) && victim->fighting == nullptr)
		{
			sprintf(buf, "Help! %s is hurling a weapon at me!", pers(ch, victim));
			do_myell(victim, buf, ch);
		}

		if (number_percent() < (85 - 7 * (26 - get_curr_stat(ch, STAT_DEX))) * skill)
		{
			one_hit_new(ch, victim, gsn_hurl, HIT_NOSPECIALS, HIT_UNBLOCKABLE, HIT_NOADD, 100 * (int)(0.67 + (float)ch->level / 30), nullptr);

			if (is_npc(victim))
			{
				init_affect(&af);
				af.where = TO_AFFECTS;
				af.type = gsn_hurl;
				af.aftype = AFT_INVIS;
				af.level = ch->level;
				af.duration = 2;
				af.location = APPLY_NONE;
				af.modifier = 0;
				af.owner = ch;
				affect_to_char(victim, &af);
			}
		}
		else
		{
			damage_new(ch, victim, 0, gsn_hurl, DAM_NONE, true, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, nullptr);
		}
	}
	else
	{
		act("With a flick of your wrist, you hurl your $t at $N!", ch, weapon_name_lookup(weapon->value[0]), victim, TO_CHAR);
		act("With a flick of $s wrist, $n hurls $S $t at you!", ch, weapon_name_lookup(weapon->value[0]), victim, TO_VICT);
		act("With a flick of $s wrist, $n hurls $S $t at $N!", ch, weapon_name_lookup(weapon->value[0]), victim, TO_NOTVICT);

		if (!is_npc(victim) && victim->fighting == nullptr)
		{
			sprintf(buf, "Help! %s is hurling a weapon at me!", pers(ch, victim));
			do_myell(victim, buf, ch);
		}

		if (number_percent() < (100 - 7 * (26 - get_curr_stat(ch, STAT_DEX))) * skill)
		{
			one_hit_new(ch, victim, gsn_hurl, HIT_NOSPECIALS, HIT_UNBLOCKABLE, HIT_NOADD, 100 * (1 + ch->level / 20), nullptr);

			if (is_npc(victim))
			{
				init_affect(&af);
				af.where = TO_AFFECTS;
				af.type = gsn_hurl;
				af.aftype = AFT_INVIS;
				af.level = ch->level;
				af.duration = 2;
				af.location = APPLY_NONE;
				af.modifier = 0;
				af.owner = ch;
				affect_to_char(victim, &af);
			}
		}
		else
		{
			damage_new(ch, victim, 0, gsn_hurl, DAM_NONE, true, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, nullptr);
		}
	}

	obj_from_char(weapon);
	obj_to_room(weapon, victroom);

	dual = get_eq_char(ch, WEAR_DUAL_WIELD);
	if (dual != nullptr)
	{
		unequip_char(ch, dual, false);
		act("You quickly swap $p into your primary hand.", ch, dual, 0, TO_CHAR);
		act("$n quickly swaps $p into $s primary hand.", ch, dual, 0, TO_ROOM);
		equip_char(ch, dual, WEAR_WIELD, false);
	}

	WAIT_STATE(ch, PULSE_VIOLENCE * 2);
}

void do_overhead(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
	int skill;
	float special;
	int dammod = 100;
	CHAR_DATA *victim;
	OBJ_DATA *weapon;
	AFFECT_DATA af;

	one_argument(argument, arg);

	skill = get_skill(ch, gsn_overhead);

	if (skill == 0)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	if (argument[0] == '\0')
	{
		victim = ch->fighting;
		if (victim == nullptr)
		{
			send_to_char("Overhead who?\n\r", ch);
			return;
		}
	}
	else
	{
		victim = get_char_room(ch, argument);

		if (victim == nullptr)
		{
			send_to_char("They aren't here.\n\r", ch);
			return;
		}
	}

	if (victim == ch)
	{
		send_to_char("You aren't too bright, are you?\n\r", ch);
		return;
	}

	weapon = get_eq_char(ch, WEAR_WIELD);

	if (!weapon)
	{
		send_to_char("You need to be wielding a weapon for that maneuver!\n\r", ch);
		return;
	}
	
	if (weapon->weight < 15)
	{
		send_to_char("Your weapon is too light for you to execute that maneuver.\n\r", ch);
		return;
	}

	if (is_safe(ch, victim))
		return;

	if (victim->size > ch->size + 1)
	{
		send_to_char("They are too large for you to properly perform an overhead strike.\n\r", ch);
		return;
	}

	if (ch->fighting == nullptr && is_affected(victim, gsn_overhead))
	{
		send_to_char("They are guarding their head too well right now.\n\r", ch);
		return;
	}

	switch (weapon->value[0])
	{
		case WEAPON_MACE:
			act("You swing your mace overhead and bring it down toward $N with terrible force!", ch, 0, victim, TO_CHAR);
			act("$n swings $s mace overhead and brings it down toward you with terrible force!", ch, 0, victim, TO_VICT);
			act("$n swings $s mace overhead and brings it down toward $N with terrible force!", ch, 0, victim, TO_NOTVICT);
			break;
		case WEAPON_STAFF:
			act("You let loose a vicious longarm strike, your staff arcing toward $N's head!", ch, 0, victim, TO_CHAR);
			act("$n lets loose a vicious longarm strike, $s staff arcing toward your head!", ch, 0, victim, TO_VICT);
			act("$n lets loose a vicious longarm strike, $s staff arcing towards $N's head!", ch, 0, victim, TO_NOTVICT);
			break;
		case WEAPON_FLAIL:
			act("The head of your flail whistles through the air, arcing towards $N's head!", ch, 0, victim, TO_CHAR);
			act("The head of $n's flail whistles through the air, arcing towards your head!", ch, 0, victim, TO_VICT);
			act("The head of $n's flail whistles through the air, arcing towards $N's head!", ch, 0, victim, TO_NOTVICT);
			break;
		case WEAPON_AXE:
			act("With a surge of brute force, you bring your axe arcing down towards $N's head!", ch, 0, victim, TO_CHAR);
			act("With a surge of brute force, $n brings $s axe arcing down towards your head!", ch, 0, victim, TO_VICT);
			act("With a surge of brute force, $n brings $s axe arcing down towards $N's head!", ch, 0, victim, TO_NOTVICT);
			break;
		case WEAPON_SWORD:
			act("You raise your sword and attempt a vicious overhead slice at $N!", ch, 0, victim, TO_CHAR);
			act("$n raises $s sword and attempts a vicious overhead slice at you!", ch, 0, victim, TO_VICT);
			act("$n raises $s sword and attempts a vicious overhead slice at $N!", ch, 0, victim, TO_NOTVICT);
			break;
		default:
			send_to_char("You cannot perform an overhead strike with that weapon.\n\r", ch);
			return;
	}

	special = (float)(skill * (1.00f + (ch->size - victim->size) / 4.00f) * (1.00f + weapon->weight / 200.00f));

	if (number_percent() < .7 * skill)
	{
		if (ch->fighting == nullptr)
		{
			if (number_percent() < .3 * special)
			{
				init_affect(&af);
				af.where = TO_AFFECTS;
				af.duration = 2;

				SET_BIT(af.bitvector, AFF_SLEEP);

				af.type = gsn_overhead;
				af.level = ch->level;
				af.aftype = AFT_MALADY;
				af.location = 0;
				af.modifier = 0;
				af.mod_name = MOD_CONC;

				switch (weapon->value[0])
				{
					case WEAPON_MACE:
						act("$N collapses in a heap as your mace comes down solidly on top of $S head!", ch, 0, victim, TO_CHAR);
						act("The world goes black as $n's mace crashes into your head.", ch, 0, victim, TO_VICT);
						act("$N collapses in a heap as the mace comes down solidly on top of $S head!", ch, 0, victim, TO_NOTVICT);

						affect_to_char(victim, &af);
						stop_fighting(victim, true);

						victim->position = POS_SLEEPING;

						check_improve(ch, gsn_overhead, true, 1);
						WAIT_STATE(ch, 2 * PULSE_VIOLENCE);
						return;
					case WEAPON_STAFF:
						act("$N collapses in a heap as your staff comes down solidly on top of $S head!", ch, 0, victim, TO_CHAR);
						act("The world goes black as $n's staff crashes into your head.", ch, 0, victim, TO_VICT);
						act("$N collapses in a heap as the staff comes down solidly on top of $S head!", ch, 0, victim, TO_NOTVICT);

						affect_to_char(victim, &af);
						stop_fighting(victim, true);

						victim->position = POS_SLEEPING;

						check_improve(ch, gsn_overhead, true, 1);
						WAIT_STATE(ch, 2 * PULSE_VIOLENCE);
						return;
					case WEAPON_FLAIL:
						act("$N collapses in a heap as your flail comes down solidly on top of $S head!", ch, 0, victim, TO_CHAR);
						act("The world goes black as $n's flail crashes into your head.", ch, 0, victim, TO_VICT);
						act("$N collapses in a heap as the flail comes down solidly on top of $S head!", ch, 0, victim, TO_NOTVICT);

						affect_to_char(victim, &af);
						stop_fighting(victim, true);

						victim->position = POS_SLEEPING;

						check_improve(ch, gsn_overhead, true, 1);
						WAIT_STATE(ch, 2 * PULSE_VIOLENCE);
						return;
				}
			}
		}

		if (number_percent() < .5 * special)
		{
			init_affect(&af);
			af.where = TO_AFFECTS;
			af.type = gsn_bleeding;
			af.location = APPLY_NONE;
			af.modifier = 0;
			af.aftype = AFT_INVIS;
			af.tick_fun = bleeding_tick;
			af.end_fun = nullptr;
			af.owner = ch;
			af.level = ch->level;

			switch (weapon->value[0])
			{
				case WEAPON_MACE:
					act("$N staggers and looks stunned as your mace crashes into $S head!", ch, 0, victim, TO_CHAR);
					act("You stagger in a daze as $n's mace crashes into your head!", ch, 0, victim, TO_VICT);
					act("$N staggers and looks stunned as the mace crashes into $S head!", ch, 0, victim, TO_NOTVICT);

					LAG_CHAR(victim, 2 * PULSE_VIOLENCE);
					dammod = 150;
					break;
				case WEAPON_STAFF:
					act("$N staggers and looks stunned as your staff crashes into $S head!", ch, 0, victim, TO_CHAR);
					act("You stagger in a daze as $n's staff crashes into your head!", ch, 0, victim, TO_VICT);
					act("$N staggers and looks stunned as the staff crashes into $S head!", ch, 0, victim, TO_NOTVICT);

					LAG_CHAR(victim, 2 * PULSE_VIOLENCE);
					dammod = 150;
					break;
				case WEAPON_FLAIL:
					act("$N staggers and looks stunned as your flail crashes into $S head!", ch, 0, victim, TO_CHAR);
					act("You stagger in a daze as $n's flail crashes into your head!", ch, 0, victim, TO_VICT);
					act("$N staggers and looks stunned as the flail crashes into $S head!", ch, 0, victim, TO_NOTVICT);

					LAG_CHAR(victim, 2 * PULSE_VIOLENCE);
					dammod = 150;
					break;
				case WEAPON_AXE:
					act("$N is enshrouded in a fine mist of blood as your mighty blow strikes true!", ch, 0, victim, TO_CHAR);
					act("You are enshrouded in a fine mist of blood as $n's axe strikes true!", ch, 0, victim, TO_VICT);
					act("$N is enshrouded in a fine mist of blood as the mighty blow strikes true!", ch, 0, victim, TO_NOTVICT);

					af.duration = ch->level / 8;

					/* fixed by cal 05/01/02 */
					if (!is_affected(victim, gsn_bleeding))
						new_affect_to_char(victim, &af);

					break;
				case WEAPON_SWORD:
					act("Your massive blade cleaves deeply into $N's flesh, striking bone!", ch, 0, victim, TO_CHAR);
					act("$n's massive blade cleaves deeply into your flesh, striking bone!", ch, 0, victim, TO_VICT);
					act("The massive blade cleaves deeply into $N's flesh, striking bone!", ch, 0, victim, TO_NOTVICT);

					dammod = 200;
					af.duration = ch->level / 12;

					if (!is_affected(victim, gsn_bleeding))
						new_affect_to_char(victim, &af);

					break;
			}
		}

		if (!is_npc(victim) && (ch->fighting == nullptr || victim->fighting == nullptr))
		{
			if (!can_see(victim, ch))
			{
				do_myell(victim, "Help! Someone is trying to crush my skull!", ch);
			}
			else
			{
				sprintf(buf, "Help! %s is trying to crush my skull!", pers(ch, victim));
				do_myell(victim, buf, ch);
			}
		}

		one_hit_new(ch, victim, gsn_overhead, HIT_SPECIALS, HIT_UNBLOCKABLE, HIT_NOADD, dammod, nullptr);
		check_improve(ch, gsn_overhead, true, 1);
	}
	else
	{
		damage(ch, victim, 0, gsn_overhead, DAM_NONE, true);
		check_improve(ch, gsn_overhead, false, 1);
	}

	WAIT_STATE(ch, PULSE_VIOLENCE * 2);
}

void do_extract(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *owner;
	int dam;
	AFFECT_DATA *af;

	if (!is_affected(ch, gsn_impale))
	{
		send_to_char("You have no shaft in you to extract!\n\r", ch);
		return;
	}

	dam = (int)((float)ch->max_hit * .2);

	send_to_char("You rip the broken shaft out of your wound and blood gushes forth!\n\r", ch);
	act("$n rips the broken shaft out of his wound!", ch, 0, 0, TO_ROOM);

	owner = ch;
	af = affect_find(ch->affected, gsn_impale);

	if (af && af->owner)
		owner = af->owner;

	damage_new(owner, ch, dam, gsn_impale, DAM_OTHER, true, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "the extraction*");
	affect_strip(ch, gsn_impale);
}

void do_exchange(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim;
	int AC;
	float chance = get_skill(ch, gsn_exchange);

	if ((chance == 0) || (ch->level < skill_table[gsn_exchange].skill_level[ch->Class()->GetIndex()] && !is_npc(ch)))
	{
		send_to_char("You don't know how to do that.\n\r", ch);
		return;
	}

	if (!ch->fighting)
	{
		send_to_char("You must be fighting to exchange blows!\n\r", ch);
		return;
	}

	if (is_wielded(ch, WEAPON_SPEAR, WIELD_PRIMARY)
		|| is_wielded(ch, WEAPON_STAFF, WIELD_PRIMARY)
		|| is_wielded(ch, WEAPON_POLEARM, WIELD_PRIMARY))
	{
		send_to_char("Your weapon is ineffective in close quarters.", ch);
		return;
	}

	victim = ch->fighting;

	if (victim == nullptr)
	{
		send_to_char("Who are you trying to exchange blows with?\n\r", ch);
		return;
	}

	if (victim == ch)
	{
		send_to_char("You really don't want to hit yourself.\n\r", ch);
		return;
	}

	chance = URANGE(30, chance * .9, 90);
	AC = average_ac(ch) / 5;

	if (AC > 50)
		AC = 50;

	AC = 100 - AC;

	act("Ignoring $N's onslaught, you close the ground between you and strike!", ch, 0, victim, TO_CHAR);
	act("Ignoring your onslaught, $n closes the ground between you and strikes!", ch, 0, victim, TO_VICT);
	act("Ignoring $N's onslaught, $n closes the ground between them and strikes!", ch, 0, victim, TO_NOTVICT);

	one_hit_new(victim, ch, TYPE_UNDEFINED, HIT_NOSPECIALS, HIT_UNBLOCKABLE, HIT_NOADD, AC, nullptr);

	if (number_percent() < chance)
	{
		one_hit_new(ch, victim, TYPE_UNDEFINED, HIT_NOSPECIALS, HIT_UNBLOCKABLE, HIT_NOADD, 200, nullptr);
		check_improve(ch, gsn_exchange, true, 1);
	}
	else
	{
		one_hit_new(ch, victim, TYPE_UNDEFINED, HIT_NOSPECIALS, HIT_UNBLOCKABLE, HIT_NOADD, 0, nullptr);
		check_improve(ch, gsn_exchange, false, 1);
	}

	WAIT_STATE(ch, 2 * PULSE_VIOLENCE);
}

/* Returns total weight of PC's armor for use with dragoon skills. */

int armor_weight(CHAR_DATA *ch)
{
	int i, total_weight = 0;
	OBJ_DATA *eq;

	if (is_npc(ch))
		return 0;

	for (i = 1; i < 16; i++)
	{
		eq = get_eq_char(ch, i);

		if (eq != nullptr)
			total_weight += eq->weight;
	}

	return total_weight;
}

void do_charge(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	char buf[MSL];
	int skill, lag, dam, avoid;

	one_argument(argument, arg);

	skill = get_skill(ch, gsn_charge);
	if (skill == 0)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	if (ch->fighting)
	{
		send_to_char("You can't charge while in combat.\n\r", ch);
		return;
	}

	if (arg[0] == '\0')
	{
		send_to_char("Charge into whom?\n\r", ch);
		return;
	}

	victim = get_char_room(ch, arg);

	if (victim == nullptr)
	{
		send_to_char("They aren't here.\n\r", ch);
		return;
	}

	if (victim == ch)
	{
		send_to_char("Clever.\n\r", ch);
		return;
	}

	if (is_safe(ch, victim))
		return;

	if (ch->size < victim->size - 1)
	{
		send_to_char("They are too massive to charge.\n\r", ch);
		return;
	}

	if (check_sidestep(ch, victim, gsn_charge, 95))
		return;

	avoid = get_skill(victim, gsn_avoid);

	if (avoid > 0)
	{
		avoid /= 2;
		avoid += 5 * (get_curr_stat(ch, STAT_DEX) - 20);
	}

	if (number_percent() < avoid)
	{
		act("You deftly avoid $N's charge and $E topples to the ground.", victim, 0, ch, TO_CHAR);
		act("$n deftly avoids $N's charge and $E topples to the ground.", victim, 0, ch, TO_NOTVICT);
		act("Your charge is deftly avoided by $n, and you go crashing to the ground.", victim, 0, ch, TO_VICT);
		WAIT_STATE(ch, 3 * PULSE_VIOLENCE);
		return;
	}

	if (!is_npc(victim))
	{
		if (!can_see(victim, ch))
		{
			do_myell(victim, "Help! Someone is charging into me!", ch);
		}
		else
		{
			sprintf(buf, "Help! %s is charging into me!", pers(ch, victim));
			do_myell(victim, buf, ch);
		}
	}

	if (number_percent() < 0.7 * skill)
	{
		act("You lower your shoulder and barrel into $N, sending $M flying!", ch, 0, victim, TO_CHAR);
		act("$n lowers $s shoulder and barrels into you, sending you flying!", ch, 0, victim, TO_VICT);
		act("$n lowers $s shoulder and barrels into $N, sending $M flying!", ch, 0, victim, TO_NOTVICT);

		lag = 1 + (armor_weight(ch) / 150);
		dam = 10 + dice(armor_weight(ch) / 15, (int)(0.4 * (float)ch->level));

		WAIT_STATE(ch, PULSE_VIOLENCE * 2);
		LAG_CHAR(victim, PULSE_VIOLENCE * lag);

		damage(ch, victim, number_range(dam - 1, dam + 1), gsn_charge, DAM_BASH, true);
		check_improve(ch, gsn_charge, true, 2);
	}
	else
	{
		act("You charge toward $N but miss, crashing awkwardly to the ground!", ch, 0, victim, TO_CHAR);
		act("$n charges toward you but misses, crashing awkwardly to the ground!", ch, 0, victim, TO_VICT);
		act("$n charges toward $N but misses, crashing awkwardly to the ground!", ch, 0, victim, TO_NOTVICT);

		damage(ch, victim, 0, gsn_charge, DAM_NONE, true);
		WAIT_STATE(ch, PULSE_VIOLENCE * 3);
		check_improve(ch, gsn_charge, false, 2);
	}
}

bool check_ease(CHAR_DATA *ch)
{
	int skill;

	if (!is_npc(ch) && !style_check(gsn_ease, ch->pcdata->style))
		return false;

	skill = get_skill(ch, gsn_ease);

	if (skill == 0)
		return false;

	if (number_percent() < skill)
	{
		check_improve(ch, gsn_ease, true, 4);
		return true;
	}
	else
	{
		return false;
	}
}

void do_shieldbash(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim;
	int skill, chance, weight;
	int dam;
	int lag = 1;
	OBJ_DATA *shield;

	skill = get_skill(ch, gsn_shieldbash);

	if (skill == 0)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	victim = ch->fighting;

	if (victim == nullptr)
	{
		send_to_char("You must be in combat to do that!\n\r", ch);
		return;
	}

	shield = get_eq_char(ch, WEAR_SHIELD);

	if (shield == nullptr)
	{
		send_to_char("You can't perform a shield bash without a shield!\n\r", ch);
		return;
	}

	if (ch->size < victim->size)
	{
		send_to_char("They are too large to shield bash.\n\r", ch);
		return;
	}

	if (ch->size > victim->size + 1)
	{
		send_to_char("They are too small to properly aim a shield bash at.\n\r", ch);
		return;
	}

	weight = armor_weight(ch);

	chance = (int)(0.7 * (float)skill);
	chance += 5 * (ch->size - victim->size);
	chance -= get_curr_stat(victim, STAT_DEX);

	if (is_npc(victim))
		chance += (ch->level - victim->level) * 2;

	if (number_percent() < chance)
	{
		act("You smash your shield into $N's chest, bearing down upon it with your weight!", ch, 0, victim, TO_CHAR);
		act("$n smashes $s shield into your chest, bearing down upon you with $s weight!", ch, 0, victim, TO_VICT);
		act("$n smashes $s shield into $N's chest, bearing down upon it with $s weight!", ch, 0, victim, TO_NOTVICT);

		dam = (int)((float)dice(ch->level / 4, 4) + ((float)weight / (float)20));

		if (pow(2, ch->size - victim->size) * weight > 250)
			lag = 2;

		WAIT_STATE(ch, PULSE_VIOLENCE * 2);
		LAG_CHAR(victim, PULSE_VIOLENCE * lag);

		damage(ch, victim, dam, gsn_shieldbash, DAM_BASH, true);
		check_improve(ch, gsn_shieldbash, true, 1);
	}
	else
	{
		act("You thrust your shield towards $N, but $E avoids it.", ch, 0, victim, TO_CHAR);
		act("$n thrusts $s shield towards you, but you avoid it.", ch, 0, victim, TO_VICT);
		act("$n thrusts $s shield towards $N, but $E avoids it.", ch, 0, victim, TO_NOTVICT);

		WAIT_STATE(ch, PULSE_VIOLENCE * 2);

		damage(ch, victim, 0, gsn_shieldbash, DAM_BASH, true);
		check_improve(ch, gsn_shieldbash, false, 1);
	}
}

void do_brace(CHAR_DATA *ch, char *arg)
{
	int skill, ac;
	float bracemod;
	float *braceptr = nullptr;
	CHAR_DATA *victim;

	skill = get_skill(ch, gsn_brace);

	if (skill == 0)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	victim = ch->fighting;

	if (victim == nullptr)
	{
		send_to_char("Brace yourself against what blows?\n\r", ch);
		return;
	}

	if (number_percent() > .95 * skill)
	{
		act("You fail to position yourself properly to ward off incoming blows.", ch, 0, victim, TO_CHAR);
		act("$n crouches awkwardly for a moment, and then straightens $mself out.", ch, 0, victim, TO_VICT);
		act("$n crouches awkwardly for a moment, and then straightens $mself out.", ch, 0, victim, TO_NOTVICT);
		WAIT_STATE(ch, PULSE_VIOLENCE * 2);
		check_improve(ch, gsn_brace, false, 3);
		return;
	}
	else
	{
		act("You brace yourself against incoming blows.", ch, 0, victim, TO_CHAR);
		act("$n crouches slightly, steeling $mself against incoming blows.", ch, 0, victim, TO_VICT);
		act("$n crouches slightly, steeling $mself against incoming blows.", ch, 0, victim, TO_NOTVICT);
		WAIT_STATE(ch, PULSE_VIOLENCE * 2);
		ac = average_ac(ch);

		if (ac < 0)
			ac = 0;

		bracemod = (float)1 - (float)ac / (float)400;

		if (bracemod < 0.25)
			bracemod = 0.25;
		if (bracemod > 1)
			bracemod = 1;

		ch->dam_mod *= bracemod;

		braceptr = (float *)talloc_struct(sizeof(float));
		*braceptr = bracemod;

		RS.Queue.AddToNewQueue(8, brace_helper_undo, ch, braceptr);
		RS.Queue.AddToNewQueue(8, act_queue, "You are no longer bracing yourself against incoming blows.", ch, nullptr, nullptr, TO_CHAR);
		check_improve(ch, gsn_brace, true, 3);
	}
}

void brace_helper_undo(CHAR_DATA *ch, float *braceptr)
{
	ch->dam_mod /= *braceptr;
}

void do_shatter(CHAR_DATA *ch, char *argument)
{
	float skill;
	float chance = 100;
	CHAR_DATA *victim = ch->fighting;
	OBJ_DATA *weapon, *wield, *handone, *handtwo, *secondary;
	char chsuc[MSL], chfail[MSL], victsuc[MSL], victfail[MSL], nvictsuc[MSL], nvictfail[MSL];
	bool isprimary = false;
	bool isoffhand = false;

	skill = get_skill(ch, gsn_shatter);

	if (skill == 0)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	if (victim == nullptr)
	{
		send_to_char("You aren't fighting anyone.\n\r", ch);
		return;
	}

	if (argument[0] == '\0')
	{
		send_to_char("Syntax: shatter <primary/offhand>\n\r", ch);
		return;
	}

	if (!str_prefix(argument, "primary"))
	{
		isprimary = true;
	}
	else if (!str_prefix(argument, "offhand"))
	{
		isoffhand = true;
	}
	else
	{
		send_to_char("Syntax: shatter <primary/offhand>\n\r", ch);
		return;
	}

	weapon = get_eq_char(ch, WEAR_WIELD);

	if (weapon == nullptr)
	{
		send_to_char("You can't do that with your bare hands!\n\r", ch);
		return;
	}

	if (weapon->value[0] == WEAPON_DAGGER || weapon->value[0] == WEAPON_WHIP || weapon->value[0] == WEAPON_EXOTIC)
	{
		send_to_char("You can't shatter anything with that weapon!\n\r", ch);
		return;
	}

	wield = get_eq_char(victim, WEAR_WIELD);

	if ((isprimary && wield == nullptr)
		|| (isoffhand && (get_eq_char(victim, WEAR_SHIELD) == nullptr) && (get_eq_char(victim, WEAR_DUAL_WIELD) == nullptr))
		|| is_affected_by(ch, AFF_BLIND))
	{
		send_to_char("There's nothing there to shatter!\n\r", ch);
		return;
	}

	if (weapon->weight < 15)
	{
		send_to_char("Your weapon isn't heavy enough to perform that move.\n\r", ch);
		return;
	}

	wield = get_eq_char(victim, WEAR_SHIELD);

	if (isoffhand && (wield != nullptr))
	{
		if (material_table[weapon->pIndexData->material_index].mat_hardness >=
			material_table[wield->pIndexData->material_index].mat_hardness)
		{
			sprintf(chfail, "You land a powerful blow on the surface of $N's shield, but it holds firm.");
			sprintf(victfail, "$n lands a powerful blow on the surface of your shield, but it holds firm.");
			sprintf(nvictfail, "$n lands a powerful blow on the surface of $N's shield, but it holds firm.");

			switch (weapon->value[0])
			{
				case WEAPON_SWORD:
					sprintf(chsuc, "$N's shield shatters as your sword smashes against it!");
					sprintf(victsuc, "Your shield shatters as $n's sword smashes against it!");
					sprintf(nvictsuc, "$N's shield shatters as $n's sword smashes against it!");

					chance *= .7;
					break;
				case WEAPON_MACE:
					sprintf(chsuc, "$N's shield shatters as your mace smashes against it!");
					sprintf(victsuc, "Your shield shatters as $n's mace smashes against it!");
					sprintf(nvictsuc, "$N's shield shatters as $n's mace smashes against it!");

					chance *= .8;
					break;
				case WEAPON_AXE:
					sprintf(chsuc, "$N's shield shatters as your axe smashes against it!");
					sprintf(victsuc, "Your shield shatters as $n's axe smashes against it!");
					sprintf(nvictsuc, "$N's shield shatters as $n's axe smashes against it!");

					chance *= .9;
					break;
				case WEAPON_SPEAR:
					sprintf(chsuc, "$N's shield shatters as your spear smashes against it!");
					sprintf(victsuc, "Your shield shatters as $n's spear smashes against it!");
					sprintf(nvictsuc, "$N's shield shatters as $n's spear smashes against it!");

					chance *= .85;
					break;
				case WEAPON_STAFF:
					sprintf(chsuc, "$N's shield shatters as your staff smashes against it!");
					sprintf(victsuc, "Your shield shatters as $n's staff smashes against it!");
					sprintf(nvictsuc, "$N's shield shatters as $n's staff smashes against it!");

					chance *= .65;
					break;
				case WEAPON_FLAIL:
					sprintf(chsuc, "$N's shield shatters as your flail smashes against it!");
					sprintf(victsuc, "Your shield shatters as $n's flail smashes against it!");
					sprintf(nvictsuc, "$N's shield shatters as $n's flail smashes against it!");

					chance *= .7;
					break;
				case WEAPON_POLEARM:
					sprintf(chsuc, "$N's shield shatters as your polearm smashes against it!");
					sprintf(victsuc, "Your shield shatters as $n's polearm smashes against it!");
					sprintf(nvictsuc, "$N's shield shatters as $n's polearm smashes against it!");

					chance *= .7;
					break;
			}
		}
		else
		{
			sprintf(chfail, "You land a powerful blow on the surface of $N's shield, but it holds firm.");
			sprintf(victfail, "$n lands a powerful blow on the surface of your shield, but it holds firm.");
			sprintf(nvictfail, "$n lands a powerful blow on the surface of $N's shield, but it holds firm.");
			chance = 0;
		}
	}
	else
	{
		handone = get_eq_char(victim, WEAR_WIELD);
		handtwo = get_eq_char(victim, WEAR_DUAL_WIELD);

		if ((isoffhand && (handtwo != nullptr)) || (isprimary && (handone != nullptr)))
		{
			if (isoffhand)
			{
				wield = get_eq_char(victim, WEAR_DUAL_WIELD);
			}
			else
			{
				wield = get_eq_char(victim, WEAR_WIELD);
			}

			switch (wield->value[0])
			{
				case WEAPON_SWORD:
					break;
				case WEAPON_MACE:
					break;
				case WEAPON_AXE:
					break;
				case WEAPON_SPEAR:
					break;
				case WEAPON_STAFF:
					break;
				case WEAPON_FLAIL:
					break;
				case WEAPON_POLEARM:
					break;
				default:
					return send_to_char("You can't shatter that.\n\r", ch);
					break;
			}

			/*
			if (material_table[weapon->pIndexData->material_index].mat_hardness >=
				material_table[wield->pIndexData->material_index].mat_hardness)
			{
			*/
			switch (weapon->value[0])
			{
				case WEAPON_SWORD:
					switch (wield->value[0])
					{
						case WEAPON_SWORD:
							chance *= .70;
							break;
						case WEAPON_MACE:
							chance *= .65;
							break;
						case WEAPON_AXE:
							chance *= .65;
							break;
						case WEAPON_SPEAR:
							chance *= .70;
							break;
						case WEAPON_STAFF:
							chance *= .75;
							break;
						case WEAPON_FLAIL:
							chance *= .60;
							break;
						case WEAPON_POLEARM:
							chance *= .60;
							break;
						default:
							chance = 0;
							break;
					}

					break;
				case WEAPON_MACE:
					switch (wield->value[0])
					{
						case WEAPON_SWORD:
							chance *= .75;
							break;
						case WEAPON_MACE:
							chance *= .70;
							break;
						case WEAPON_AXE:
							chance *= .70;
							break;
						case WEAPON_SPEAR:
							chance *= .45;
							break;
						case WEAPON_STAFF:
							chance *= .45;
							break;
						case WEAPON_FLAIL:
							chance *= .55;
							break;
						case WEAPON_POLEARM:
							chance *= .45;
							break;
						default:
							chance = 0;
							break;
					}

					break;
				case WEAPON_AXE:
					switch (wield->value[0])
					{
						case WEAPON_SWORD:
							chance *= .85;
							break;
						case WEAPON_MACE:
							chance *= .70;
							break;
						case WEAPON_AXE:
							chance *= .80;
							break;
						case WEAPON_SPEAR:
							chance *= .80;
							break;
						case WEAPON_STAFF:
							chance *= .80;
							break;
						case WEAPON_FLAIL:
							chance *= .75;
							break;
						case WEAPON_POLEARM:
							chance *= .75;
							break;
						default:
							chance = 0;
							break;
					}

					break;
				case WEAPON_SPEAR:
					switch (wield->value[0])
					{
						case WEAPON_SWORD:
							chance *= 0;
							break;
						case WEAPON_MACE:
							chance *= 0;
							break;
						case WEAPON_AXE:
							chance *= 0;
							break;
						case WEAPON_SPEAR:
							chance *= .50;
							break;
						case WEAPON_STAFF:
							chance *= .40;
							break;
						case WEAPON_FLAIL:
							chance *= .25;
							break;
						case WEAPON_POLEARM:
							chance *= .40;
							break;
						default:
							chance = 0;
							break;
					}

					break;
				case WEAPON_STAFF:
					switch (weapon->value[0])
					{
						case WEAPON_SWORD:
							chance *= .50;
							break;
						case WEAPON_MACE:
							chance *= .30;
							break;
						case WEAPON_AXE:
							chance *= .40;
							break;
						case WEAPON_SPEAR:
							chance *= .30;
							break;
						case WEAPON_STAFF:
							chance *= .30;
							break;
						case WEAPON_FLAIL:
							chance *= .45;
							break;
						case WEAPON_POLEARM:
							chance *= .30;
							break;
						default:
							chance = 0;
							break;
					}

					break;
				case WEAPON_FLAIL:
					switch (weapon->value[0])
					{
						case WEAPON_SWORD:
							chance *= .65;
							break;
						case WEAPON_MACE:
							chance *= .60;
							break;
						case WEAPON_AXE:
							chance *= .55;
							break;
						case WEAPON_SPEAR:
							chance *= .50;
							break;
						case WEAPON_STAFF:
							chance *= .40;
							break;
						case WEAPON_FLAIL:
							chance *= .60;
							break;
						case WEAPON_POLEARM:
							chance *= .45;
							break;
						default:
							chance = 0;
							break;
					}

					break;
				case WEAPON_POLEARM:
					switch (weapon->value[0])
					{
						case WEAPON_SWORD:
							chance *= 0;
							break;
						case WEAPON_MACE:
							chance *= 0;
							break;
						case WEAPON_AXE:
							chance *= 0;
							break;
						case WEAPON_SPEAR:
							chance *= .60;
							break;
						case WEAPON_STAFF:
							chance *= .70;
							break;
						case WEAPON_FLAIL:
							chance *= .25;
							break;
						case WEAPON_POLEARM:
							chance *= .55;
							break;
						default:
							chance = 0;
							break;
					}

					break;
			}
			/*
			}
			else
			{
				chance = 0;
			}
			*/

			sprintf(chfail, "You strike mightily at $N's $t, but it holds firm.");
			sprintf(victfail, "$n strikes mightily at your $t, but it holds firm.");
			sprintf(nvictfail, "$n strikes mightily at $N's $t, but it holds firm.");
			sprintf(chsuc, "Driving in mightily, you shatter $N's $t with a single powerful blow!");
			sprintf(victsuc, "Driving in mightily, $n shatters your $t with a single powerful blow!");
			sprintf(nvictsuc, "Driving in mightily, $n shatters $N's $t with a single powerful blow!");
		}
	}

	if (is_obj_stat(wield, ITEM_NODISARM) || is_obj_stat(wield, ITEM_FIXED))
		chance = 0;

	chance = skill * chance * .01;
	chance *= (130 - 20 * material_table[wield->pIndexData->material_index].mat_hardness);
	chance *= .01;

	if (is_immortal(ch))
	{
		int successChance = (int)skill;
		int rawChance = (int)chance;
		act("Chance of success is $i%, raw chance is $I%.", ch, &successChance, &rawChance, TO_CHAR);
	}

	if (number_percent() > chance)
	{
		act(chfail, ch, weapon_name_lookup(wield->value[0]), victim, TO_CHAR);
		act(victfail, ch, weapon_name_lookup(wield->value[0]), victim, TO_VICT);
		act(nvictfail, ch, weapon_name_lookup(wield->value[0]), victim, TO_NOTVICT);
		check_improve(ch, gsn_shatter, false, 1);

		WAIT_STATE(ch, 2 * PULSE_VIOLENCE);
		return;
	}

	act(chsuc, ch, weapon_name_lookup(wield->value[0]), victim, TO_CHAR);
	act(victsuc, ch, weapon_name_lookup(wield->value[0]), victim, TO_VICT);
	act(nvictsuc, ch, weapon_name_lookup(wield->value[0]), victim, TO_NOTVICT);

	extract_obj(wield);
	secondary = get_eq_char(victim, WEAR_DUAL_WIELD);

	if (secondary != nullptr)
	{
		unequip_char(victim, secondary, false);
		act("You quickly swap $p into your primary hand.", victim, secondary, 0, TO_CHAR);
		act("$n quickly swaps $p into $s primary hand.", victim, secondary, 0, TO_ROOM);
		equip_char(victim, secondary, WEAR_WIELD, false);
	}

	WAIT_STATE(ch, 2 * PULSE_VIOLENCE);
	check_improve(ch, gsn_shatter, true, 1);
	return;
}

void do_whirlwind(CHAR_DATA *ch, char *argument)
{
	char buf[MSL];
	int skill, chance = 95, chdex;
	CHAR_DATA *vch, *vch_next, *victim;
	OBJ_DATA *wield, *dualwield;

	skill = get_skill(ch, gsn_whirlwind);
	if (skill == 0)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	if (argument[0] == '\0')
	{
		victim = ch->fighting;
		if (victim == nullptr)
		{
			send_to_char("Who are you trying to whirlwind?\n\r", ch);
			return;
		}
	}
	else
	{
		victim = get_char_room(ch, argument);
		if (victim == nullptr)
		{
			send_to_char("They aren't here.\n\r", ch);
			return;
		}
	}

	wield = get_eq_char(ch, WEAR_WIELD);
	if (wield == nullptr)
	{
		send_to_char("You cannot do that barehanded.\n\r", ch);
		return;
	}

	dualwield = get_eq_char(ch, WEAR_DUAL_WIELD);
	if (dualwield == nullptr)
	{
		send_to_char("You must be wielding two weapons to perform that maneuver.\n\r", ch);
		return;
	}

	if ((wield->value[0] == WEAPON_WHIP) || (dualwield->value[0] == WEAPON_WHIP))
	{
		send_to_char("You cannot use a whip to do that!\n\r", ch);
		return;
	}

	if (wield->weight > skirmisher_max_weapweight(ch) || dualwield->weight > skirmisher_max_weapweight(ch))
	{
		send_to_char("You can only execute a whirlwind while wielding very light weapons.\n\r", ch);
		return;
	}

	if (!is_npc(victim) && ((ch->fighting == nullptr) || (victim->fighting == nullptr)))
	{
		sprintf(buf, "Help! %s is whirling into me!", pers(ch, victim));
		do_myell(victim, buf, ch);
	}

	act("You leap at $N, spinning and striking rapidly in all directions!", ch, 0, victim, TO_CHAR);
	act("$n leaps at you, spinning and striking rapidly in all directions!", ch, 0, victim, TO_VICT);
	act("$n leaps at $N, spinning and striking rapidly in all directions!", ch, 0, victim, TO_NOTVICT);

	chdex = get_curr_stat(ch, STAT_DEX);
	chance -= 5 * (25 - chdex);
	skill *= chance;

	WAIT_STATE(ch, PULSE_VIOLENCE * 2);

	for (vch = ch->in_room->people; vch != nullptr; vch = vch_next)
	{
		vch_next = vch->next_in_room;
		if (number_percent() < skill)
		{
			if ((vch != ch && is_same_group(vch, victim) && ((can_see(ch, vch) || vch->fighting != nullptr))))
			{
				one_hit_new(ch, vch, TYPE_UNDEFINED, HIT_NOSPECIALS, HIT_UNBLOCKABLE, HIT_NOADD, 100, nullptr);
			}
			else
			{
				if (is_npc(victim) && is_npc(vch))
				{
					if (victim->pIndexData->vnum == vch->pIndexData->vnum)
					{
						one_hit_new(ch, vch, TYPE_UNDEFINED, HIT_NOSPECIALS, HIT_UNBLOCKABLE, HIT_NOADD, 100, nullptr);
					}
				}
			}
		}
		else
		{
			if ((vch != ch && is_same_group(vch, victim) && ((can_see(ch, vch) || vch->fighting != nullptr))))
			{
				one_hit_new(ch, vch, TYPE_UNDEFINED, HIT_NOSPECIALS, HIT_UNBLOCKABLE, HIT_NOADD, 100, nullptr);
			}
			else if (is_npc(victim) && is_npc(vch))
			{
				if ((victim->pIndexData->vnum == vch->pIndexData->vnum))
				{
					one_hit_new(ch, vch, TYPE_UNDEFINED, HIT_NOSPECIALS, HIT_UNBLOCKABLE, HIT_NOADD, 0, nullptr);
				}
			}
		}
	}
}

void check_batter(CHAR_DATA *ch)
{
	OBJ_DATA *wield, *dual;
	int skill;

	skill = get_skill(ch, gsn_batter);
	if (skill == 0)
		return;

	if (number_percent() > skill)
		return;

	wield = get_eq_char(ch, WEAR_WIELD);
	if (wield == nullptr)
		return;

	if (wield->weight < 15)
		return;

	dual = get_eq_char(ch, WEAR_DUAL_WIELD);

	if (dual != nullptr && dual->weight >= 15)
		ch->batter += 5;
	else
		ch->batter += 3;

	check_improve(ch, gsn_batter, true, 5);
}

bool check_entwine(CHAR_DATA *ch, int type)
{
	for (AFFECT_DATA *af = ch->affected; af != nullptr; af = af->next)
	{
		if (type == 0 && af->type == gsn_entwine && af->modifier == 0)
			return true;
		else if (type == 1 && af->type == gsn_entwine && af->modifier == 1)
			return true;
		else if (type == 2 && af->type == gsn_entwine && af->location == APPLY_DEX)
			return true;
	}

	return false;
}

void do_entwine(CHAR_DATA *ch, char *argument)
{
	int skill;
	CHAR_DATA *victim;
	char arg1[MSL], buf[MSL];
	bool arm = false, leg = false;
	AFFECT_DATA af;

	skill = get_skill(ch, gsn_entwine);
	if (skill == 0)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	if (check_entwine(ch, 0))
	{
		send_to_char("Your whip is already entwined.\n\r", ch);
		return;
	}

	argument = one_argument(argument, arg1);

	victim = ch->fighting;
	if (victim == nullptr)
	{
		if (arg1[0] == '\0')
		{
			send_to_char("Who are you trying to entwine?\n\r", ch);
			return;
		}
		else
		{
			victim = get_char_room(ch, arg1);
			if (victim == nullptr)
			{
				send_to_char("They aren't here.\n\r", ch);
				return;
			}
		}

		if (!str_cmp(argument, "arm"))
		{
			arm = true;
		}
		else if (!str_cmp(argument, "leg"))
		{
			leg = true;
		}
		else
		{
			send_to_char("Syntax: entwine <char> <arm/leg>\n\r", ch);
			return;
		}

		skill *= 60 + 2 * (get_curr_stat(ch, STAT_DEX) - get_curr_stat(victim, STAT_DEX));
		skill /= 100;
	}
	else
	{
		if (!str_cmp(arg1, "arm"))
		{
			arm = true;
		}
		else if (!str_cmp(arg1, "leg"))
		{
			leg = true;
		}
		else
		{
			send_to_char("Syntax: entwine <arm/leg>\n\r", ch);
			return;
		}

		skill *= 40 + 2 * (get_curr_stat(ch, STAT_DEX) - get_curr_stat(victim, STAT_DEX));
		skill /= 100;
	}

	if (victim == ch)
	{
		send_to_char("You cannot entwine yourself.\n\r", ch);
		return;
	}

	if (is_safe_new(ch, victim, true))
		return;

	if (!is_wielded(ch, WEAPON_WHIP, WIELD_PRIMARY))
	{
		send_to_char("You cannot entwine with that.\n\r", ch);
		return;
	}

	if (check_entwine(victim, 1) && arm)
	{
		send_to_char("Their arm is already entwined.\n\r", ch);
		return;
	}
	else if (check_entwine(victim, 2) && leg)
	{
		send_to_char("Their leg is already entwined.\n\r", ch);
		return;
	}

	if (arm && victim->arms < 1)
	{
		send_to_char("They have no arms to entwine.\n\r", ch);
		return;
	}

	if (leg && victim->legs < 1)
	{
		send_to_char("They have no legs to entwine.\n\r", ch);
		return;
	}

	if (!is_npc(victim) && (ch->fighting == nullptr || victim->fighting == nullptr))
	{
		sprintf(buf, "Help! %s is entwining me!", pers(ch, victim));
		do_myell(victim, buf, ch);
	}

	WAIT_STATE(ch, PULSE_VIOLENCE * 2);

	if (number_percent() < skill)
	{
		init_affect(&af);
		af.where = TO_AFFECTS;
		af.duration = -1;
		af.type = gsn_entwine;
		af.level = ch->level;

		if (arm)
		{
			af.location = APPLY_NONE;
			af.modifier = 1;
		}
		else if (leg)
		{
			af.location = APPLY_DEX;
			af.modifier = -(ch->level / 15);
		}

		af.aftype = AFT_SKILL;
		af.owner = ch;
		affect_to_char(victim, &af);

		init_affect(&af);
		af.where = TO_AFFECTS;
		af.duration = -1;
		af.type = gsn_entwine;
		af.level = ch->level;
		af.location = APPLY_NONE;
		af.modifier = 0;
		af.aftype = AFT_INVIS;
		af.owner = victim;
		affect_to_char(ch, &af);

		act("You crack your whip, deftly coiling it about $N's $t and pulling it taut!", ch, arm ? "arm" : leg ? "leg" : "", victim, TO_CHAR);
		act("$n cracks $s whip, deftly coiling it about your $t and pulling it taut!", ch, arm ? "arm" : leg ? "leg" : "", victim, TO_VICT);
		act("$n cracks $s whip, deftly coiling it about $N's $t and pulling it taut!", ch, arm ? "arm" : leg ? "leg" : "", victim, TO_NOTVICT);
		check_improve(ch, gsn_entwine, true, 1);
	}
	else
	{
		act("Your whip makes a loud *crack* as it snaps, barely missing $N's $t!", ch, arm ? "arm" : leg ? "leg" : "", victim, TO_CHAR);
		act("$n's whip makes a loud *crack* as it snaps, barely missing your $t!", ch, arm ? "arm" : leg ? "leg" : "", victim, TO_VICT);
		act("$n's whip makes a loud *crack* as it snaps, barely missing $N's $t!", ch, arm ? "arm" : leg ? "leg" : "", victim, TO_NOTVICT);
		check_improve(ch, gsn_entwine, false, 1);
	}

	if (!ch->fighting && !victim->fighting)
	{
		set_fighting(ch, victim);
		multi_hit(victim, ch, TYPE_UNDEFINED);
	}
}

void do_uncoil(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *guy;
	AFFECT_DATA *af, *af2 = nullptr;
	int type;
	OBJ_DATA *weapon, *dual;

	if (check_entwine(ch, 1))
	{
		type = 1;
	}
	else if (check_entwine(ch, 2))
	{
		type = 2;
	}
	else if (check_entwine(ch, 0))
	{
		type = 0;
	}
	else
	{
		send_to_char("You aren't entwined!\n\r", ch);
		return;
	}

	for (af = ch->affected; af != nullptr; af = af->next)
	{
		if (type == 0 && af->type == gsn_entwine && af->modifier == 0)
			break;
		else if (type == 1 && af->type == gsn_entwine && af->modifier == 1)
			break;
		else if (type == 2 && af->type == gsn_entwine && af->location == APPLY_DEX)
			break;
	}

	guy = af->owner;

	if (guy != nullptr)
	{
		for (af2 = guy->affected; af2 != nullptr; af2 = af2->next)
		{
			if (af2->type == gsn_entwine && af2->owner == ch)
				break;
		}
	}

	if (number_percent() < ((get_curr_stat(ch, STAT_STR) - 13) * 8) || type == 0)
	{
		if (type == 0)
		{
			if (str_cmp(argument, "automagic"))
			{
				act("With a sudden jerk of your wrist, you uncoil your whip, freeing $N's $t!", ch, af2->modifier == 1 ? "arm" : af2->location == APPLY_DEX ? "leg" : "", guy, TO_CHAR);
				act("With a sudden jerk of $n's wrist, $e uncoils $s whip, freeing your $t!", ch, af2->modifier == 1 ? "arm" : af2->location == APPLY_DEX ? "leg" : "", guy, TO_VICT);
				act("With a sudden jerk of $n's wrist, $e uncoils $s whip, freeing $N's $t!", ch, af2->modifier == 1 ? "arm" : af2->location == APPLY_DEX ? "leg" : "", guy, TO_NOTVICT);
			}

			affect_remove(ch, af);

			if (guy != nullptr)
				affect_remove(guy, af2);
		}
		else if (type == 1 || type == 2)
		{
			act("With a surge of force, you tear your $t away from the coils of $N's whip!", ch, type == 1 ? "arm" : type == 2 ? "leg" : "", guy, TO_CHAR);
			act("With a surge of force, $n tears $s $t away from the coils of your whip!", ch, type == 1 ? "arm" : type == 2 ? "leg" : "", guy, TO_VICT);
			act("With a surge of force, $n tears $s $t away from the coils of $N's whip!", ch, type == 1 ? "arm" : type == 2 ? "leg" : "", guy, TO_NOTVICT);
			affect_remove(ch, af);

			if (guy != nullptr)
				affect_remove(guy, af2);

			weapon = get_eq_char(guy, WEAR_WIELD);

			if (weapon != nullptr
				&& number_percent() > 70
				&& !is_obj_stat(weapon, ITEM_NODISARM)
				&& !is_obj_stat(weapon, ITEM_FIXED))
			{
				act("$p snaps in half as you tear it away from $N's grasp!", ch, weapon, guy, TO_CHAR);
				act("$p snaps in half as $n tears it away from your grasp!", ch, weapon, guy, TO_VICT);
				act("$p snaps in half as $n tears it away from $N's grasp!", ch, weapon, guy, TO_NOTVICT);

				obj_from_char(weapon);
				extract_obj(weapon);

				dual = get_eq_char(guy, WEAR_DUAL_WIELD);

				if (dual != nullptr)
				{
					unequip_char(guy, dual, false);
					act("You quickly swap $p into your primary hand.", guy, dual, 0, TO_CHAR);
					act("$n quickly swaps $p into $s primary hand.", guy, dual, 0, TO_ROOM);
					equip_char(guy, dual, WEAR_WIELD, false);
				}
			}
		}
	}
	else
	{
		act("You struggle to free yourself from $N's grasp, but the whip holds firm!", ch, 0, guy, TO_CHAR);
		act("$n struggles to free $mself from your grasp, but the whip holds firm!", ch, 0, guy, TO_VICT);
		act("$n struggles to free $mself from $N's grasp, but the whip holds firm!", ch, 0, guy, TO_NOTVICT);
	}

	WAIT_STATE(ch, PULSE_VIOLENCE);
}

void do_pull(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *guy;
	AFFECT_DATA *af, *af2, taf;
	int dir, skill;
	char *direction;
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;

	skill = get_skill(ch, gsn_entwine);

	if (skill == 0)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	if (check_entwine(ch, 0))
	{
		for (af = ch->affected; af != nullptr; af = af->next)
		{
			if (af->type == gsn_entwine && af->modifier == 0)
				break;
		}

		guy = af->owner;

		for (af2 = guy->affected; af2 != nullptr; af2 = af2->next)
		{
			if (af2->type == gsn_entwine && af2->owner == ch)
				break;
		}

		WAIT_STATE(ch, PULSE_VIOLENCE * 2);

		if (af2->location == APPLY_DEX)
		{
			if (number_percent() < (skill * 0.8))
			{
				act("You draw your whip taut, pulling $N's legs out from under $M!", ch, 0, guy, TO_CHAR);
				act("$n draws $s whip taut, pulling your legs out from under you!", ch, 0, guy, TO_VICT);
				act("$n draws $s whip taut, pulling $N's legs out from under $M!", ch, 0, guy, TO_NOTVICT);
				WAIT_STATE(guy, PULSE_VIOLENCE * 2);
				check_improve(ch, gsn_pull, true, 1);
			}
			else
			{
				act("$N staggers slightly as you jerk your whip back, but remains standing.", ch, 0, guy, TO_CHAR);
				act("You stagger slightly as $n jerks $s whip back, but remain standing.", ch, 0, guy, TO_VICT);
				act("$N staggers slightly as $n jerks $s whip back, but remains standing.", ch, 0, guy, TO_NOTVICT);
				check_improve(ch, gsn_pull, false, 1);
			}
		}
		else if (af2->modifier == 1)
		{
			if (argument[0] == '\0')
			{
				send_to_char("Pull them in which direction?\n\r", ch);
				return;
			}

			dir = direction_lookup(argument);
			if (dir == -1)
			{
				send_to_char("That is not a direction!\n\r", ch);
				return;
			}

			pexit = ch->in_room->exit[dir];
			if (pexit == nullptr)
			{
				send_to_char("You cannot pull them in that direction.\n\r", ch);
				return;
			}

			if (number_percent() < (skill * 0.9) && !(is_npc(guy) && IS_SET(guy->act, ACT_SENTINEL)))
			{
				direction = flag_name_lookup(dir, direction_table);
				act("Suddenly pulling your whip taut, you drag $N $tward!", ch, direction, guy, TO_CHAR);
				act("Suddenly pulling $s whip taut, $n drags you $tward!", ch, direction, guy, TO_VICT);
				act("Suddenly pulling $s whip taut, $n drags $N $tward!", ch, direction, guy, TO_NOTVICT);

				init_affect(&taf);
				taf.type = gsn_pull;
				affect_to_char(ch, &taf);
				affect_to_char(guy, &taf);

				to_room = pexit->u1.to_room;

				stop_fighting(ch, true);
				move_char(ch, dir, true, false);
				stop_fighting(guy, true);
				move_char(guy, dir, true, false);

				direction = flag_name_lookup(reverse_d(dir), direction_table);
				act("$n enters from $t, pulling along a tightly-entwined $N with him!", ch, direction, guy, TO_NOTVICT);

				if (guy->in_room == ch->in_room && !ch->fighting && !guy->fighting)
				{
					set_fighting(ch, guy);
					multi_hit(guy, ch, TYPE_UNDEFINED);
				}

				affect_strip(ch, gsn_pull);
				affect_strip(guy, gsn_pull);
				check_improve(ch, gsn_pull, true, 1);
			}
			else
			{
				direction = flag_name_lookup(dir, direction_table);
				act("You attempt to drag $N $tward, but $E holds $S ground.", ch, direction, guy, TO_CHAR);
				act("$n attempts to drag you $tward, but you hold your ground.", ch, direction, guy, TO_VICT);
				act("$n attempts to drag $N $tward, but $E holds $S ground.", ch, direction, guy, TO_NOTVICT);
				check_improve(ch, gsn_pull, false, 1);
			}
		}
	}
	else
	{
		send_to_char("Your whip is not entwined!\n\r", ch);
	}
}

char *get_assess_line(long where[], int percent)
{
	char buf[MSL], last[MSL], *beep;
	int i;

	buf[0] = '\0';
	last[0] = '\0';

	for (i = 0; imm_flags[i].name != nullptr; i++)
	{
		if (IS_SET(where, imm_flags[i].bit) && number_percent() < percent)
		{
			if (str_cmp(last, ""))
			{
				strcat(buf, last);
				strcat(buf, ", ");
			}

			sprintf(last, "%s", imm_flags[i].name);
		}
	}

	if (str_cmp(buf, "") && str_cmp(last, ""))
	{
		strcat(buf, "and ");
		strcat(buf, last);
	}
	else if (!str_cmp(buf, "") && str_cmp(last, ""))
	{
		strcat(buf, last);
	}
	else
	{
		strcat(buf, "nothing");
	}

	beep = palloc_string(buf);
	return beep;
}

void do_assess(CHAR_DATA *ch, char *argument)
{
	int skill, found = 0;
	float dammod;
	CHAR_DATA *victim = nullptr;

	skill = get_skill(ch, gsn_assess);
	if (skill == 0)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	victim = ch->fighting;
	if (argument[0] == '\0' && !victim)
	{
		send_to_char("Assess who?\n\r", ch);
		return;
	}

	if (!victim && (victim = get_char_room(ch, argument)) == nullptr)
	{
		send_to_char("They aren't here.\n\r", ch);
		return;
	}

	act("You examine $N closely, using your keen eye to assess $S weaknesses.", ch, 0, victim, TO_CHAR);
	act("$n examines you closely, using $S keen eye to assess your weaknesses.", ch, 0, victim, TO_VICT);
	act("$n examines $N closely, sizing $M up.", ch, 0, victim, TO_NOTVICT);

	if (skill >= 60)
	{
		act("$N appears to be resistant to $t.", ch, (char *)get_assess_line(victim->res_flags, skill), victim, TO_CHAR);
		found++;
	}

	if (skill >= 80)
	{
		act("$N appears to be immune to $t.", ch, (char *)get_assess_line(victim->imm_flags, skill), victim, TO_CHAR);
		found++;
	}

	if (skill >= 90)
	{
		act("$N appears to be vulnerable to $t.", ch, (char *)get_assess_line(victim->vuln_flags, skill), victim, TO_CHAR);
		found++;
	}

	if (skill >= 100)
	{
		dammod = victim->dam_mod;

		if (is_affected(victim, gsn_sanctuary))
			dammod *= .5;

		if (is_affected(victim, gsn_sanguine_ward))
			dammod *= .8;

		if (is_affected(victim, gsn_visceral))
			dammod *= .87;

		if (is_affected(victim, gsn_stoneskin))
			dammod *= .8;

		if (is_affected(victim, gsn_soften))
			dammod *= 1.3;

		if (victim->dam_mod < 20)
			act("$N is virtually impervious to harm!", ch, 0, victim, TO_CHAR);
		else if (victim->dam_mod <= 40)
			act("$N has extensive protection from all sorts of harm.", ch, 0, victim, TO_CHAR);
		else if (victim->dam_mod <= 60)
			act("$N is well protected from all sorts of harm.", ch, 0, victim, TO_CHAR);
		else if (victim->dam_mod <= 80)
			act("$N is somewhat protected from all sorts of harm.", ch, 0, victim, TO_CHAR);
		else if (victim->dam_mod <= 99)
			act("$N's susceptibility to damage is slightly reduced.", ch, 0, victim, TO_CHAR);
		else if (victim->dam_mod <= 100)
			act("$N does not appear to have any special protections.", ch, 0, victim, TO_CHAR);
		else
			act("$N is highly susceptible to all damage.", ch, 0, victim, TO_CHAR);

		found++;
	}

	if (found == 0)
		send_to_char("Your assessment reveals nothing interesting.\n\r", ch);

	check_improve(ch, gsn_assess, true, 1);

	WAIT_STATE(ch, PULSE_VIOLENCE * 2);
}

bool check_exploit_armor_break(CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *exposed)
{
	if (is_obj_stat(exposed, ITEM_FIXED) || is_obj_stat(exposed, ITEM_BURN_PROOF))
		return false;

	int matindex = exposed->pIndexData->material_index;
	float chance = get_skill(ch, gsn_exploit), avgac = 0;
	OBJ_DATA *chwield = get_eq_char(ch, WEAR_WIELD);

	if (exposed->item_type != ITEM_ARMOR)
	{
		act("$p is not being used as armor.", ch, exposed, victim, TO_CHAR);
		return false;
	}

	for (int i = 0; i < MAX_AC; i++)
	{
		avgac += exposed->value[i];
	}

	avgac /= MAX_AC;
	chance += (material_table[chwield->pIndexData->material_index].mat_hardness - material_table[matindex].mat_hardness) * 8;
	chance -= avgac * 7;
	chance *= .35;

	if (number_percent() < (int)chance) // destroy
	{
		act("You land a calculated blow upon $p, damaging it beyond repair!", ch, exposed, 0, TO_CHAR);
		act("$n lands a calculated blow upon $N's $p, damaging it beyond repair!", ch, exposed, victim, TO_NOTVICT);
		act("$n lands a calculated blow upon your $p, damaging it beyond repair!", ch, exposed, victim, TO_VICT);

		extract_obj(exposed);
		return true;
	}

	if (is_immortal(ch))
	{
		int breakChance = (int)chance;
		int dentChance = (int)(chance * 1.5);
		act("Chance of destruction is $i%, chance of denting is $I%.", ch, &breakChance, &dentChance, TO_CHAR);
	}

	chance *= 1.5;

	if (number_percent() < (int)chance) // damage
	{
		act("You land a calculated blow upon $p, weakening it significantly.", ch, exposed, 0, TO_CHAR);
		act("$n lands a calculated blow upon $N's $p, weakening it.", ch, exposed, victim, TO_NOTVICT);
		act("$n lands a calculated blow upon your $p, weakening it.", ch, exposed, victim, TO_VICT);

		for (int i = 0; i < MAX_AC; i++)
		{
			exposed->value[i] = std::max(0, exposed->value[i] - number_range(1, 2));
		}

		return true;
	}

	act("Your blow fails to do any lasting damage to $p.", ch, exposed, 0, TO_CHAR);
	return false;
}

void do_exploit(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim;
	OBJ_DATA *exposed, *wield, *weapon;
	char chsuc[MSL], victsuc[MSL], nvictsuc[MSL];
	char chspe[MSL], victspe[MSL], nvictspe[MSL];
	char chfail[MSL], anfail[MSL];
	bool arm = false, leg = false, torso = false, head = false, anatomy = false, broken = false;
	char arg[MAX_INPUT_LENGTH];
	int dam = 120;
	float skill = 0;
	weapon = get_eq_char(ch, WEAR_WIELD);

	one_argument(argument, arg);

	skill = get_skill(ch, gsn_exploit);
	if (skill <= 1)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	victim = ch->fighting;
	if (victim == nullptr)
	{
		send_to_char("You aren't fighting anyone.\n\r", ch);
		return;
	}

	if (get_eq_char(ch, WEAR_WIELD) == nullptr)
	{
		send_to_char("You need some sort of weapon to exploit your foe's weaknesses.\n\r", ch);
		return;
	}

	if (!str_cmp(arg, "arm"))
	{
		arm = true;
	}
	else if (!str_cmp(arg, "leg"))
	{
		leg = true;
	}
	else if (!str_cmp(arg, "torso"))
	{
		torso = true;
	}
	else if (!str_cmp(arg, "head"))
	{
		head = true;
	}
	else
	{
		send_to_char("Syntax: exploit <arm/leg/torso/head>\n\r", ch);
		return;
	}

	wield = get_eq_char(victim, WEAR_WIELD);
	skill *= .9;

	if (head && !str_cmp(race_table[victim->race].name, "minotaur"))
	{
		send_to_char("A minotaur's thick skull and horns prevent any sort of meaningful strike.\n\r", ch);
		WAIT_STATE(ch, PULSE_VIOLENCE);
		return;
	}

	if (!str_cmp(race_table[victim->race].name, "human")
		|| !str_cmp(race_table[victim->race].name, "dwarf")
		|| !str_cmp(race_table[victim->race].name, "elf")
		|| !str_cmp(race_table[victim->race].name, "drow")
		|| !str_cmp(race_table[victim->race].name, "planar")
		|| !str_cmp(race_table[victim->race].name, "imp")
		|| !str_cmp(race_table[victim->race].name, "halfling")
		|| !str_cmp(race_table[victim->race].name, "abyss")
		|| !str_cmp(race_table[victim->race].name, "duergar")
		|| !str_cmp(race_table[victim->race].name, "celestial")
		|| !str_cmp(race_table[victim->race].name, "sidhe")
		|| !str_cmp(race_table[victim->race].name, "minotaur"))
	{
		if (arm)
		{
			exposed = get_eq_char(victim, WEAR_ARMS);
			if (exposed != nullptr)
			{
				arm = false;
				broken = check_exploit_armor_break(ch, victim, exposed);
			}
			else
			{
				sprintf(chsuc, "$N's unprotected arm goes limp for a moment as your $t strikes it!");
				sprintf(victsuc, "Your unprotected arm goes limp for a moment as $n's $t strikes it!");
				sprintf(nvictsuc, "$N's unprotected arm goes limp for a moment as $n's $t strikes it!");
				sprintf(chspe, "$N's $t falls to the ground!");
				sprintf(victspe, "Your $t falls to the ground!");
				sprintf(nvictspe, "$N's $t falls to the ground!");
				dam = 150;
			}
		}
		else if (leg)
		{
			exposed = get_eq_char(victim, WEAR_LEGS);
			if ((exposed != nullptr))
			{
				leg = false;
				broken = check_exploit_armor_break(ch, victim, exposed);
			}
			else
			{
				sprintf(chsuc, "$N staggers as your $t strikes $S exposed leg!");
				sprintf(victsuc, "You stagger as $n's $t strikes your exposed leg!");
				sprintf(nvictsuc, "$N staggers as $n's $t strikes $S exposed leg!");
				dam = 150;
			}
		}
		else if (torso)
		{
			exposed = get_eq_char(victim, WEAR_BODY);
			if (exposed != nullptr)
			{
				torso = false;
				broken = check_exploit_armor_break(ch, victim, exposed);
				dam = 150;
			}
			else
			{
				sprintf(chsuc, "You savage $N's unprotected torso, causing grievous injury!");
				sprintf(victsuc, "$n savages your unprotected torso, causing grievous injury!");
				sprintf(nvictsuc, "$n savages $N's unprotected torso, causing grievous injury!");
				dam = 300;
			}
		}
		else if (head)
		{
			exposed = get_eq_char(victim, WEAR_HEAD);
			if (exposed != nullptr)
			{
				head = false;
				broken = check_exploit_armor_break(ch, victim, exposed);
			}
			else
			{
				sprintf(chsuc, "$N staggers dazedly as your $t strikes $S exposed head!");
				sprintf(victsuc, "You stagger dazedly as $n's $t strikes your exposed head!");
				sprintf(nvictsuc, "$N staggers dazedly as $n's $t strikes $S exposed head!");
				dam = 200;
			}
		}

		sprintf(chfail, "You are unable to find a weakness in $N's armor.");
		anatomy = true;
	}
	else
	{
		sprintf(anfail, "You lack a sufficient understanding of $N's anatomy.");
	}

	if (number_percent() <= skill && anatomy && (arm || leg || torso || head || broken))
	{
		if (!broken)
		{
			act(chsuc, ch, weapon_name_lookup(weapon->value[0]), victim, TO_CHAR);
			act(victsuc, ch, weapon_name_lookup(weapon->value[0]), victim, TO_VICT);
			act(nvictsuc, ch, weapon_name_lookup(weapon->value[0]), victim, TO_NOTVICT);
		}

		if (arm)
		{
			if (wield != nullptr && (!is_obj_stat(wield, ITEM_NODISARM) && !is_obj_stat(wield, ITEM_NOREMOVE)))
			{
				act(chspe, ch, weapon_name_lookup(wield->value[0]), victim, TO_CHAR);
				act(victspe, ch, weapon_name_lookup(wield->value[0]), victim, TO_VICT);
				act(nvictspe, ch, weapon_name_lookup(wield->value[0]), victim, TO_NOTVICT);

				obj_from_char(wield);

				if (is_obj_stat(wield, ITEM_NODROP) || is_obj_stat(wield, ITEM_INVENTORY))
				{
					obj_to_char(wield, victim);
				}
				else
				{
					obj_to_room(wield, victim->in_room);

					if (is_npc(victim) && victim->wait == 0 && can_see_obj(victim, wield))
						get_obj(victim, wield, nullptr, true);
				}
			}
		}
		else if (leg)
		{
			LAG_CHAR(victim, PULSE_VIOLENCE);
			victim->position = POS_RESTING;
		}
		else if (head)
		{
			LAG_CHAR(victim, PULSE_VIOLENCE * 2);
		}

		one_hit_new(ch, victim, TYPE_UNDEFINED, HIT_NOSPECIALS, HIT_UNBLOCKABLE, HIT_NOADD, dam, nullptr);
		check_improve(ch, gsn_exploit, true, 1);
	}
	else if (!anatomy)
	{
		act(anfail, ch, 0, victim, TO_CHAR);
		check_improve(ch, gsn_exploit, false, 1);
	}
	else
	{
		act(chfail, ch, 0, victim, TO_CHAR);
		check_improve(ch, gsn_exploit, false, 1);
	}

	WAIT_STATE(ch, PULSE_VIOLENCE * 2);
}

void do_offhand(CHAR_DATA *ch, char *arg)
{
	int chance;
	OBJ_DATA *wield, *offhand;
	bool found;
	CHAR_DATA *victim = ch->fighting;

	chance = get_skill(ch, skill_lookup("offhand disarm"));

	if (victim == nullptr)
	{
		send_to_char("You aren't fighting anyone.\n\r", ch);
		return;
	}

	wield = get_eq_char(ch, WEAR_WIELD);
	if (wield == nullptr)
	{
		send_to_char("You must be wielding a weapon to offhand disarm your opponent.\n\r", ch);
		return;
	}

	/* Offhand disarm can offhand: a dual wielded weapon, a shield, a held item, or a light */
	found = false;

	offhand = get_eq_char(victim, WEAR_DUAL_WIELD);
	if (offhand != nullptr)
	{
		found = true;
	}
	else if ((offhand = get_eq_char(victim, WEAR_SHIELD)) != nullptr)
	{
		found = true;
	}
	else if ((offhand = get_eq_char(victim, WEAR_HOLD)) != nullptr)
	{
		found = true;
	}
	else
	{
		offhand = get_eq_char(victim, WEAR_LIGHT);
		found = true;
	}

	if (offhand && !can_see_obj(ch, offhand))
		found = false;

	if (!found)
	{
		send_to_char("Your opponent is not wielding anything in their off hand.\n\r", ch);
		return;
	}

	chance -= 40;

	if (is_affected_by(ch, AFF_HASTE))
		chance += 10;

	if (is_affected_by(victim, AFF_HASTE))
		chance -= 15;

	if (number_percent() < chance)
	{
		if (is_obj_stat(offhand, ITEM_NOREMOVE) || is_obj_stat(offhand, ITEM_NODISARM))
		{
			act("You try to offhand disarm $N, but $p won't budge!", ch, offhand, victim, TO_CHAR);
			act("$n tries to offhand disarm $N, but $p won't budge!", ch, offhand, victim, TO_NOTVICT);
			act("$n tries to offhand disarm you, but $p won't budge!", ch, offhand, victim, TO_VICT);
			return;
		}

		act("You knock $N's $p to the ground!", ch, offhand, victim, TO_CHAR);
		act("$n knocks $N's $p to the ground!", ch, offhand, victim, TO_NOTVICT);
		act("$n knocks your $p to the ground!", ch, offhand, victim, TO_VICT);
		check_improve(ch, skill_lookup("offhand disarm"), true, 1);
		obj_from_char(offhand);

		if (is_obj_stat(offhand, ITEM_NODROP))
		{
			obj_to_char(offhand, victim);
		}
		else
		{
			if (check_catch(ch, victim, offhand))
				obj_to_char(offhand, ch);
			else
				obj_to_room(offhand, victim->in_room);
		}

		if (is_npc(victim) && !is_affected_by(victim, AFF_BLIND))
			interpret(victim, "get 1.");

		command_result = true;
		WAIT_STATE(ch, PULSE_VIOLENCE * 2);
		return;
	}

	act("You try to knock $N's $p to the ground, but fail.", ch, offhand, victim, TO_CHAR);
	act("$n tries to knock $N's $p to the ground, but fails.", ch, offhand, victim, TO_NOTVICT);
	act("$n tries to knock your $p to the ground, but fails.", ch, offhand, victim, TO_VICT);
	check_improve(ch, skill_lookup("offhand disarm"), false, 1);
	command_result = true;
	WAIT_STATE(ch, PULSE_VIOLENCE * 2);
}

void do_drive(CHAR_DATA *ch, char *argument)
{
	int where, skill;
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	CHAR_DATA *victim = nullptr;
	char *direction;
	char store[MSL], target[MSL], dir[MSL];

	/* Must have the skill to use it */
	skill = get_skill(ch, gsn_drive);
	if (skill == 0)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	/* break up the argument into two variables */
	if (!*argument)
	{
		send_to_char("Drive who where?\n\r", ch);
		return;
	}

	/* Check to see if they are fighting */
	if (ch->fighting)
	{
		where = direction_lookup(argument);
		if (where == -1)
		{
			half_chop(argument, target, dir);

			if (!*target)
			{
				send_to_char("Drive who?\n\r", ch);
				return;
			}

			if (!*dir)
			{
				send_to_char("Drive where?\n\r", ch);
				return;
			}

			victim = get_char_room(ch, target);
			if (victim == nullptr)
			{
				send_to_char("They aren't here.\n\r", ch);
				return;
			}
		}
		else
		{
			sprintf(dir, "%s", argument);
			victim = ch->fighting;
		}
	}
	else
	{
		skill = (skill - (skill / 3));
		half_chop(argument, target, dir);

		if (!*target)
		{
			send_to_char("Drive who?\n\r", ch);
			return;
		}

		if (!*dir)
		{
			send_to_char("Drive where?\n\r", ch);
			return;
		}

		/* Set victim */
		victim = get_char_room(ch, target);
		if (victim == nullptr)
		{
			send_to_char("They aren't here.\n\r", ch);
			return;
		}
	}

	/* Check for pk range */
	if (is_safe_new(ch, victim, true))
		return;

	if (check_sidestep(ch, victim, gsn_drive, 95))
		return;

	/* Can't push yourself */
	if (victim == ch)
	{
		send_to_char("You can't push yourself around.\n\r", ch);
		return;
	}

	/* Check to see if the direction verb is valid */
	where = direction_lookup(dir);
	if (where == -1)
	{
		send_to_char("That is not a direction!\n\r", ch);
		return;
	}

	/* Get the default word for the direction */
	direction = flag_name_lookup(where, direction_table);
	sprintf(dir, "%s", direction);

	/* Make sure the exit is valid */
	pexit = ch->in_room->exit[where];
	if (pexit == nullptr)
	{
		send_to_char("You cannot drive in that direction.\n\r", ch);
		return;
	}

	if (!is_npc(ch) && !is_npc(victim) && (ch->fighting == nullptr || victim->fighting == nullptr))
	{
		sprintf(store, "Help! %s is pushing me around!", pers(ch, victim));
		do_myell(victim, store, ch);
	}

	if (is_npc(victim)
		&& pexit->u1.to_room
		&& victim->pIndexData->restrict_low != -1
		&& victim->pIndexData->restrict_high != 65535)
	{
		if (pexit->u1.to_room->vnum < victim->pIndexData->restrict_low ||
			pexit->u1.to_room->vnum > victim->pIndexData->restrict_high)
			return act("$N will not budge that direction!", ch, 0, victim, TO_CHAR);
	}

	if (is_npc(victim) && IS_SET(victim->act, ACT_SENTINEL))
		skill = 0;

	if (number_percent() < skill)
	{
		act("You surge at $N, pressing $M towards the $t!", ch, dir, victim, TO_CHAR);
		act("$n surges at you, pressing you towards the $t!", ch, dir, victim, TO_VICT);
		act("$n surges at $N, pressing $M towards the $t!", ch, dir, victim, TO_NOTVICT);

		/* Set the direction room */
		to_room = pexit->u1.to_room;

		/* stop and move driver */
		stop_fighting(ch, true);
		move_char(ch, where, true, false);

		/* stop and move drivee */
		stop_fighting(victim, true);
		move_char(victim, where, true, false);
		check_improve(ch, gsn_drive, true, 1);

		if (victim->in_room != ch->in_room)
			return;

		act("$n drives $N in from the $t!", ch, dir, victim, TO_NOTVICT);
	}
	else
	{
		act("You surge at $N, but $E doesn't budge!", ch, 0, victim, TO_CHAR);
		act("$n surges at you, but you don't budge!", ch, 0, victim, TO_VICT);
		act("$n surges at $N, but $E doesn't budge!", ch, 0, victim, TO_NOTVICT);
		check_improve(ch, gsn_drive, false, 1);
	}

	if (victim->in_room == ch->in_room && !ch->fighting && !victim->fighting)
		set_fighting(ch, victim);

	one_hit(victim, ch, TYPE_HIT);
	WAIT_STATE(ch, PULSE_VIOLENCE * 2);
}

void do_dash(CHAR_DATA *ch, char *argument)
{
	int where, skill;
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	char command[MSL], dir[MSL], word[MSL];
	char *direction;

	/* Must have the skill to use it */
	skill = get_skill(ch, gsn_dash);
	if (skill == 0)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	/* break up the argument into two variables */
	if (!*argument)
	{
		send_to_char("Dash where and do what?\n\r", ch);
		return;
	}

	/* Check to see if they are fighting */
	if (ch->fighting)
	{
		send_to_char("You can't dash while you are fighting!\n\r", ch);
		return;
	}

	half_chop(argument, dir, command);

	if (!*command)
	{
		send_to_char("You must specify an action when you dash.\n\r", ch);
		return;
	}

	if (!*dir)
	{
		send_to_char("Dash which direction?\n\r", ch);
		return;
	}

	one_argument(command, word);

	if (!str_prefix(word, "dash")
		|| !str_prefix(word, "east")
		|| !str_prefix(word, "north")
		|| !str_prefix(word, "west")
		|| !str_prefix(word, "south")
		|| !str_prefix(word, "up")
		|| !str_prefix(word, "down"))
	{
		send_to_char("You can't do that.\n\r", ch);
		return;
	}

	/* Check to see if the direction verb is valid */
	where = direction_lookup(dir);
	if (where == -1)
	{
		send_to_char("That is not a direction!\n\r", ch);
		return;
	}

	/* Get the default word for the direction */
	direction = flag_name_lookup(reverse_d(where), direction_table);
	sprintf(dir, "%s", direction);

	/* Make sure the exit is valid */
	pexit = ch->in_room->exit[where];
	if (pexit == nullptr)
	{
		send_to_char("You cannot dash in that direction.\n\r", ch);
		return;
	}

	/* Set the direction room */
	to_room = pexit->u1.to_room;

	if (number_percent() < skill)
	{
		/* stop and move driver */
		move_char(ch, where, true, false);

		if (ch->in_room != to_room)
			return;

		ch->wait = 0;

		act("$n dashes in from the $t!", ch, dir, 0, TO_ROOM);
		interpret(ch, command);
		WAIT_STATE(ch, PULSE_VIOLENCE);
		check_improve(ch, gsn_dash, true, 5);
	}
	else
	{
		send_to_char("You fail to prepare for your dash properly, and manage only to stumble in!\n\r", ch);

		/* stop and move driver */
		move_char(ch, where, true, false);
		act("$n stumbles in from the $t!", ch, dir, 0, TO_ROOM);
		WAIT_STATE(ch, PULSE_VIOLENCE * 2);
		check_improve(ch, gsn_dash, false, 5);
	}
}

void do_concuss(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH], af_name[MSL];
	AFFECT_DATA af;
	CHAR_DATA *victim;
	int chance, size;
	bool concussed = false;
	OBJ_DATA *weapon;
	char weapon_name[MSL];

	one_argument(argument, arg);

	chance = get_skill(ch, gsn_concuss);
	if (chance == 0 || (ch->level < skill_table[gsn_concuss].skill_level[ch->Class()->GetIndex()] && !is_npc(ch)))
	{
		send_to_char("You don't know how to do that.\n\r", ch);
		return;
	}

	if (!ch->fighting)
	{
		send_to_char("You must in combat to do that!\n\r", ch);
		return;
	}

	victim = ch->fighting;

	if (is_wielded(ch, WEAPON_AXE, WIELD_ONE)
		|| is_wielded(ch, WEAPON_WHIP, WIELD_ONE)
		 ||is_wielded(ch, WEAPON_POLEARM, WIELD_ONE))
	{
		send_to_char("You cannot use such a weapon to concuss anyone.\n\r", ch);
		return;
	}

	if (victim == ch)
	{
		send_to_char("You really don't want to hit yourself.\n\r", ch);
		return;
	}

	if (is_affected(victim, gsn_concuss))
		concussed = true;

	if (is_safe(ch, victim))
		return;

	size = victim->size - ch->size;

	if (size > 2)
	{
		send_to_char("That person is too large for you to hit their head.\n\r", ch);
		return;
	}

	chance = (int)((float)chance * .75);
	chance += (ch->level - victim->level);

	if (size > 0)
		chance -= 20 * size;

	if (is_affected_by(victim, AFF_HASTE))
		chance -= 20;

	if (is_affected_by(ch, AFF_HASTE))
		chance += 10;

	if (get_eq_char(victim, WEAR_HEAD) != nullptr)
		chance -= 20;

	sprintf(af_name, "concussion");

	init_affect(&af);
	af.name = palloc_string(af_name);
	af.where = TO_AFFECTS;
	af.level = ch->level;
	af.location = 0;
	af.aftype = AFT_MALADY;
	af.type = gsn_concuss;
	af.modifier = 0;
	af.mod_name = MOD_CONC;

	if (!is_npc(ch) && get_trust(ch) == MAX_LEVEL)
		chance = 101;

	if (is_wielded(ch, WEAPON_HAND, WIELD_ONE))
	{
		sprintf(weapon_name, "fist");
	}
	else
	{
		weapon = get_eq_char(ch, WEAR_WIELD);
		sprintf(weapon_name, "%s", weapon_name_lookup(weapon->value[0]));
	}

	if (number_percent() < chance)
	{
		act("$N staggers and clutches his head as your $t crashes into his temple!", ch, weapon_name, victim, TO_CHAR);
		act("$n slams $s $t into $N's temple!", ch, weapon_name, victim, TO_NOTVICT);
		act("Throbbing pain sears your mind as $n's $t crashes into your temple!", ch, weapon_name, victim, TO_VICT);

		af.duration = 2;

		if (!concussed)
			affect_to_char(victim, &af);

		check_improve(ch, gsn_concuss, true, 1);
		one_hit_new(ch, victim, TYPE_UNDEFINED, HIT_NOSPECIALS, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "concussion");
	}
	else
	{
		act("You fail to connect with $N's temple.", ch, 0, victim, TO_CHAR);
		act("$n fails to connect with $N's temple.", ch, 0, victim, TO_NOTVICT);
		act("$n fails to hit you in the temple.", ch, 0, victim, TO_VICT);
		damage(ch, victim, 0, gsn_concuss, DAM_NONE, true);
		check_improve(ch, gsn_concuss, false, 1);
	}

	WAIT_STATE(ch, 2 * PULSE_VIOLENCE);
}

void do_retreat(CHAR_DATA *ch, char *arg)
{

	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	int skill;
	int direction;
	int *dirptr = nullptr;

	skill = get_skill(ch, gsn_retreat);
	if (skill == 0)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	if (!ch->fighting)
	{
		send_to_char("You must be fighting to retreat!\n\r", ch);
		return;
	}

	direction = direction_lookup(arg);
	if (direction < 0 || !ch->in_room->exit[direction])
	{
		send_to_char("That's not a valid direction.\n\r", ch);
		return;
	}

	pexit = ch->in_room->exit[direction];

	to_room = pexit->u1.to_room;

	if (!to_room)
	{
		send_to_char("You can't move in that direction.\n\r", ch);
		return;
	}

	act("You begin plotting a retreat.", ch, 0, 0, TO_CHAR);

	dirptr = (int *)talloc_struct(sizeof(int));
	*dirptr = direction;

	RS.Queue.AddToNewQueue(3, execute_retreat, ch, dirptr);

	WAIT_STATE(ch, PULSE_VIOLENCE);
}

void execute_retreat(CHAR_DATA *ch, int *direction)
{
	char *dirname;
	int dir = *direction;
	int skill = get_skill(ch, gsn_retreat);
	ROOM_INDEX_DATA *start = ch->in_room;

	if (number_percent() > skill)
	{
		send_to_char("You fail to find an opening to make your retreat.\n\r", ch);
		check_improve(ch, gsn_retreat, false, 1);
		return;
	}

	dirname = flag_name_lookup(dir, direction_table);
	act("You make a hasty retreat $t.", ch, dirname, 0, TO_CHAR);
	act("$n makes a hasty retreat $t.", ch, dirname, 0, TO_ROOM);

	move_char(ch, dir, true, false);

	if (ch->in_room != start)
		stop_fighting(ch, true);
}

void do_disrupt_formation(CHAR_DATA *ch, char *arg)
{
	int skill, percent;
	CHAR_DATA *vch, *vch_next, *gch;
	bool grouped = false;
	CHAR_DATA *victim = nullptr;

	if (arg[0] == '\0')
	{
		victim = ch->fighting;
		if (victim == nullptr)
		{
			send_to_char("But you aren't fighting anyone!\n\r", ch);
			return;
		}
	}
	else
	{
		victim = get_char_room(ch, arg);
		if (victim == nullptr)
		{
			send_to_char("They aren't here.\n\r", ch);
			return;
		}
	}

	skill = get_skill(ch, gsn_disrupt_formation);
	if (skill == 0)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	if (ch->fighting && !is_npc(ch))
	{
		send_to_char("You have other things to worry about at the moment.\n\r", ch);
		return;
	}

	if (ch->leader)
		grouped = true;

	if (is_npc(victim))
	{
		act("$N does not have a group to disrupt!", ch, 0, victim, TO_CHAR);
		return;
	}

	for (gch = char_list; gch != nullptr; gch = gch->next)
	{
		if (is_same_group(gch, ch) && gch != ch)
		{
			grouped = true;
		}
	}

	if (grouped)
	{
		send_to_char("You cannot disrupt a formation while part of a group.\n\r", ch);
		return;
	}

	if (number_percent() < skill)
	{
		act("You dart in to strike $N, weaving chaotically around $M!", ch, 0, victim, TO_CHAR);
		act("$n darts in to strike you, weaving around chaotically!", ch, 0, victim, TO_VICT);
		act("$n darts in to strike $N, weaving around chaotically!", ch, 0, victim, TO_NOTVICT);

		for (vch = ch->in_room->people; vch != nullptr; vch = vch_next)
		{
			vch_next = vch->next_in_room;

			if (vch == ch || vch == victim || !is_same_group(vch, victim) || is_npc(vch))
				continue;

			percent = 30;

			if (vch->Class()->GetIndex() == CLASS_WARRIOR)
			{
				percent = 50;

				if (!is_npc(vch) && vch->pcdata->style == style_lookup("duelist"))
					percent = 100;
			}
			else if (vch->Class()->ctype == CLASS_CASTER || vch->Class()->ctype == CLASS_COMMUNER)
			{
				percent = 10;
			}

			if (check_leadership_save(vch, gsn_disrupt_formation))
				percent = 100;

			if (number_percent() > percent)
			{
				act("$n's assault drives you out of formation and separates you from your group!", ch, 0, vch, TO_VICT);
				act("Your assault drives $N out of formation and separates $M from the group.", ch, 0, vch, TO_CHAR);
				act("$n's assault drives $N out of formation and separates $M from the group!", ch, 0, vch, TO_NOTVICT);
				vch->leader = nullptr;
			}
			else
			{
				act("$N retains $S composure amidst $n's onslaught, remaining with $S group.", ch, 0, vch, TO_NOTVICT);
				act("$N retains $S composure amidst your onslaught, remaining with $S group.", ch, 0, vch, TO_CHAR);
				act("You retain your composure amidst $n's onslaught, remaining with your group.", ch, 0, vch, TO_VICT);
			}
		}
	}
	else
	{
		act("Darting in to strike $N, you stumble into the midst of $S group.", ch, 0, victim, TO_CHAR);
		act("Darting in to strike $N, $n stumbles and fails his ploy.", ch, 0, victim, TO_NOTVICT);
		act("Darting in to strike you, $n stumbles into the midst of your group.", ch, 0, victim, TO_VICT);
	}

	one_hit(victim, ch, TYPE_UNDEFINED);
}

void do_leadership(CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;
	CHAR_DATA *to;
	bool leader = false;
	int chance;

	chance = get_skill(ch, gsn_leadership);
	if (chance == 0 || ch->level < skill_table[gsn_leadership].skill_level[ch->Class()->GetIndex()])
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	for (to = ch->in_room->people; to != nullptr; to = to->next_in_room)
	{
		if (is_same_group(to, ch) && to != ch)
			leader = true;
	}

	if (is_affected(ch, gsn_leadership))
	{
		send_to_char("You recently organized your party and do not feel up to it yet.\n\r", ch);
		return;
	}

	if (!leader)
	{
		send_to_char("You are not leading a group!\n\r", ch);
		return;
	}

	if (ch->mana < 50)
	{
		send_to_char("You don't have the energy to organize your group.\n\r", ch);
		return;
	}

	if (number_percent() > chance)
	{
		act("$n tries to explain combat tactics to you, but he just becomes frustrated at himself.", ch, 0, 0, TO_GROUP);
		act("You try to explain combat tactics to your group, but they just do not understand.", ch, 0, 0, TO_CHAR);
		check_improve(ch, gsn_leadership, false, 2);
		ch->mana -= 25;
		WAIT_STATE(ch, PULSE_VIOLENCE * 2);
	}
	else
	{
		act("You organize your group into an efficient combat formation.", ch, 0, 0, TO_CHAR);
		act("$n organizes the group into an efficient combat formation.", ch, 0, 0, TO_GROUP);
		act("$n gives some instructions to $s group as they listen intently.", ch, 0, 0, TO_NOTGROUP);
		check_improve(ch, gsn_leadership, true, 2);

		init_affect(&af);
		af.where = TO_AFFECTS;
		af.type = gsn_leadership;
		af.modifier = 0;
		af.location = 0;
		af.duration = ch->level;
		af.duration = ch->level / 3;
		af.level = ch->level;
		af.aftype = AFT_INVIS;
		affect_to_char(ch, &af);

		to = ch->in_room->people;

		for (; to != nullptr; to = to->next_in_room)
		{
			if (to != ch && is_same_group(to, ch))
			{
				if (!is_affected(to, gsn_leadership))
				{
					// Add affect to the group.
					init_affect(&af);
					af.where = TO_AFFECTS;
					af.type = gsn_leadership;
					af.modifier = ch->level / 5;
					af.location = APPLY_HITROLL;
					af.duration = ch->level;
					af.duration = ch->level / 3;
					af.level = ch->level;
					af.aftype = AFT_SKILL;
					affect_to_char(to, &af);
					af.location = APPLY_SAVES;
					af.modifier = -(ch->level / 5);
					affect_to_char(to, &af);
				}
			}
		}

		ch->mana -= 50;
		WAIT_STATE(ch, PULSE_VIOLENCE * 2);
	}
}

bool check_leadership_save(CHAR_DATA *ch, int skill)
{
	if (ch->leader == nullptr || ch->leader == ch)
		return false;

	if (skill == gsn_howl || skill == gsn_disrupt_formation)
	{
		return true;
	}

	return false;
}

void check_leadership_affect(CHAR_DATA *ch)
{
	AFFECT_DATA *af, *af_next;

	for (af = ch->affected; af != nullptr; af = af_next)
	{
		af_next = af->next;
		if (af->type == gsn_leadership)
			affect_remove(ch, af);
	}
}

int check_outflank(CHAR_DATA *ch)
{
	AFFECT_DATA *paf;
	int modifier = -1;

	for (paf = ch->affected; paf != nullptr; paf = paf->next)
	{
		if (paf->type == gsn_outflank)
		{
			modifier = paf->modifier;
			break;
		}
	}

	return modifier;
}

void do_outflank(CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;
	int type, dir;
	char buf[MSL];
	char *direction;

	if (argument[0] == '\0' && !is_npc(ch))
	{
		send_to_char("Syntax: outflank <direction>\n\r", ch);

		type = check_outflank(ch);

		if (type != -1)
		{
			direction = flag_name_lookup(type, direction_table);
			sprintf(buf, "You are currently blocking your foes from escaping %sward.\n\r", direction);
			send_to_char(buf, ch);
		}

		return;
	}

	type = direction_lookup(argument);
	if (type == -1 && !is_npc(ch))
	{
		send_to_char("That is not a direction!\n\r", ch);
		return;
	}

	if (is_npc(ch))
	{
		type = -2;

		for (auto exit : ch->in_room->exit)
		{
			if (exit != nullptr)
			{
				type = -1;
				break;
			}
		}

		if (type == -2)
			return;

		auto size = static_cast<int>(std::size(ch->in_room->exit)) - 1;
		while (type == -1)
		{
			dir = number_range(0, size);
			if (ch->in_room->exit[dir] != nullptr)
				type = dir;
		}
	}

	if (check_outflank(ch) == type)
	{
		send_to_char("You are already blocking that direction!\n\r", ch);
		return;
	}

	WAIT_STATE(ch, PULSE_VIOLENCE * 2);

	direction = flag_name_lookup(type, direction_table);
	act("You position yourself so as to obstruct the $t egress.", ch, direction, 0, TO_CHAR);
	act("$n positions $mself so as to obstruct the $t egress.", ch, direction, 0, TO_ROOM);

	affect_strip(ch, gsn_outflank);

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_INVIS;
	af.type = gsn_outflank;
	af.level = ch->level;
	af.duration = -1;
	af.modifier = type;
	af.location = APPLY_NONE;
	affect_to_char(ch, &af);
}

bool outflank_me(CHAR_DATA *ch, int dir)
{
	CHAR_DATA *victim;
	OBJ_DATA *weapon;
	float outflank;
	char *direction;

	for (victim = ch->in_room->people; victim != nullptr; victim = victim->next_in_room)
	{
		if (victim != ch && !is_same_group(ch, victim))
		{
			outflank = get_skill(victim, gsn_outflank);
			if (outflank != 0 && check_outflank(victim) == dir)
			{
				weapon = get_eq_char(victim, WEAR_WIELD);
				if (weapon != nullptr)
				{
					switch (weapon->value[0])
					{
						case WEAPON_SWORD:
							outflank *= .7;
							break;
						case WEAPON_AXE:
							outflank *= .65;
							break;
						case WEAPON_MACE:
							outflank *= .6;
							break;
						case WEAPON_FLAIL:
							outflank *= .6;
							break;
						case WEAPON_POLEARM:
							outflank *= .95;
							break;
						case WEAPON_STAFF:
							outflank *= .85;
							break;
						case WEAPON_SPEAR:
							outflank *= .9;
							break;
						case WEAPON_WHIP:
							outflank *= .75;
							break;
						case WEAPON_DAGGER:
							outflank *= .55;
							break;
					}
				}
				else
				{
					outflank *= .5;
				}

				direction = flag_name_lookup(dir, direction_table);
				if (number_percent() < outflank)
				{
					act("As $N attempts to flee, you bar $S exit $tward!", victim, direction, ch, TO_CHAR);
					act("As you attempt to flee, $n bars your exit $tward!", victim, direction, ch, TO_VICT);
					act("As $N attempts to flee, $n bars $s exit $tward!", victim, direction, ch, TO_NOTVICT);
					check_improve(victim, gsn_outflank, true, 1);
					WAIT_STATE(ch, PULSE_VIOLENCE);
					return true;
				}
				else
				{
					act("$N slips past you, escaping $t!", victim, direction, ch, TO_CHAR);
					act("You slip past $n as $e attempts to block your path!", victim, 0, ch, TO_VICT);
					act("$N slips past $n as $e attempts to block $S path!", victim, 0, ch, TO_NOTVICT);
					check_improve(victim, gsn_outflank, false, 2);
				}
			}
		}
	}

	return false;
}
