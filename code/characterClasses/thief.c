#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "thief.h"
#include "../comm.h"
#include "../devextra.h"
#include "../skills.h"
#include "../newmem.h"
#include "../update.h"
#include "../act_info.h"
#include "../act_wiz.h"
#include "../misc.h"
#include "../interp.h"
#include "../tables.h"
#include "../recycle.h"
#include "../magic.h"
#include "../db.h"
#include "../lookup.h"
#include "../act_move.h"
#include "../act_comm.h"
#include "../dioextra.h"
#include "../fight.h"
#include "../db.h"
#include "../const.h"
#include "../material.h"
#include "../utility.h"
#include "../include/spdlog/fmt/bundled/format.h"

void do_backstab(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	OBJ_DATA *obj;
	int dam;
	char buf[MAX_STRING_LENGTH];

	one_argument(argument, arg);

	if (get_skill(ch, gsn_backstab) < 2)
	{
		send_to_char("You don't know how to backstab.\n\r", ch);
		return;
	}

	if (arg[0] == '\0')
	{
		send_to_char("Backstab whom?\n\r", ch);
		return;
	}

	victim = get_char_room(ch, arg);

	if (victim == nullptr)
	{
		send_to_char("They aren't here.\n\r", ch);
		return;
	}

	if (ch->fighting != nullptr || victim->fighting)
	{
		send_to_char("You're facing the wrong end.\n\r", ch);
		return;
	}

	if (victim == ch)
	{
		send_to_char("How can you sneak up on yourself?\n\r", ch);
		return;
	}

	obj = get_eq_char(ch, WEAR_WIELD);

	if (!obj)
	{
		send_to_char("You must be wielding a weapon to backstab.\n\r", ch);
		return;
	}

	if (obj->value[0] != WEAPON_DAGGER)
	{
		obj = get_eq_char(ch, WEAR_DUAL_WIELD);

		if (obj == nullptr || obj->value[0] != WEAPON_DAGGER)
		{
			send_to_char("You must be wielding a dagger to backstab.\n\r", ch);
			return;
		}
	}

	if (is_safe(ch, victim))
		return;

	if (check_sidestep(ch, victim, gsn_backstab, 95))
		return;

	if (victim->hit < victim->max_hit * 8.5 / 10)
	{
		act("$N is hurt and suspicious ... you can't sneak up.", ch, nullptr, victim, TO_CHAR);
		return;
	}

	WAIT_STATE(ch, skill_table[gsn_backstab].beats);

	if (number_percent() < get_skill(ch, gsn_backstab))
	{
		check_improve(ch, gsn_backstab, true, 1);

		if (!is_npc(ch) && !is_npc(victim) && victim->fighting == nullptr)
		{
			switch (number_range(0, 1))
			{
				case 0:
					sprintf(buf, "Die, %s you backstabbing fool!", pers(ch, victim));
					break;
				case 1:
					sprintf(buf, "Help! I'm being backstabbed by %s!", pers(ch, victim));
					break;
			}

			do_myell(victim, buf, ch);
		}

		one_hit_new(ch, victim, gsn_backstab, HIT_NOSPECIALS, HIT_UNBLOCKABLE, HIT_NOADD, URANGE(100, (ch->level - 9) * 10, 375), nullptr);
	}
	else
	{
		check_improve(ch, gsn_backstab, false, 1);

		if (!is_npc(ch) && !is_npc(victim) && victim->fighting == nullptr)
		{
			switch (number_range(0, 1))
			{
				case 0:
					sprintf(buf, "Die, %s you backstabbing fool!", pers(ch, victim));
					break;
				case 1:
					sprintf(buf, "Help! I'm being backstabbed by %s!", pers(ch, victim));
					break;
			}

			do_myell(victim, buf, ch);
		}

		damage(ch, victim, 0, gsn_backstab, DAM_NONE, true);
	}

	if (ch->level < skill_table[gsn_dual_backstab].skill_level[ch->Class()->GetIndex()])
		return;

	if (number_range(0, 3) == 0)
	{
		if (get_skill(ch, gsn_dual_backstab) != 100)
			check_improve(ch, gsn_dual_backstab, false, 1);

		return;
	}

	if (number_percent() > (get_skill(ch, gsn_dual_backstab) + ch->level - victim->level))
	{
		check_improve(ch, gsn_dual_backstab, false, 1);
		dam = 0;
	}
	else
	{
		dam = dice(obj->value[1], obj->value[2]) * get_skill(ch, gsn_dual_backstab) / 100;

		if (ch->level < 35)
			dam *= number_range(2, 4);
		else if (ch->level < 40)
			dam *= number_range(2, 5);
		else if (ch->level < 45)
			dam *= number_range(3, 5);
		else if (ch->level < 50)
			dam *= number_range(4, 6);
		else
			dam *= number_range(4, 7);

		dam += (ch->damroll * get_skill(ch, gsn_dual_backstab) / 100);
	}

	damage_old(ch, victim, dam, gsn_dual_backstab, DAM_NONE, true);
}

void do_circle_stab(CHAR_DATA *ch, char *argument)
{
	bool attempt_dual;
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	CHAR_DATA *v_check;
	CHAR_DATA *v_next;
	OBJ_DATA *obj;
	int chance;
	int dam;

	attempt_dual = false;
	one_argument(argument, arg);

	chance = get_skill(ch, gsn_circle);

	if (chance <= 1)
	{
		send_to_char("Circling? What's that?\n\r", ch);
		return;
	}

	if (arg[0] == '\0')
	{
		victim = ch->fighting;

		if (victim == nullptr)
		{
			send_to_char("But you aren't fighting anyone.\n\r", ch);
			return;
		}
	}
	else if ((victim = get_char_room(ch, arg)) == nullptr)
	{
		send_to_char("They aren't here.\n\r", ch);
		return;
	}

	if (ch->fighting == nullptr)
	{
		send_to_char("You can't circle someone like that.\n\r", ch);
		return;
	}

	for (v_check = ch->in_room->people; v_check != nullptr; v_check = v_next)
	{
		v_next = v_check->next_in_room;

		if (v_check->fighting == ch)
		{
			send_to_char("Not while you're defending yourself!\n\r", ch);
			return;
		}
	}

	if (victim == ch)
	{
		send_to_char("huh?\n\r", ch);
		return;
	}

	obj = get_eq_char(ch, WEAR_WIELD);

	if (obj == nullptr || obj->value[0] != WEAPON_DAGGER)
	{
		attempt_dual = true;
		obj = get_eq_char(ch, WEAR_DUAL_WIELD);
	}

	if (obj == nullptr)
	{
		send_to_char("You must wield a dagger to circle stab.\n\r", ch);
		return;
	}

	if (obj->value[0] != 2)
	{
		send_to_char("You must wield a dagger to circle stab.\n\r", ch);
		return;
	}

	chance += (ch->level - victim->level);
	chance = URANGE(5, chance, 95);

	WAIT_STATE(ch, skill_table[gsn_circle].beats);

	if (number_percent() < chance)
	{
		act("You circle around $N to land a critical strike.", ch, nullptr, victim, TO_CHAR);
		act("$n circles around you to land a critical strike.", ch, nullptr, victim, TO_VICT);
		act("$n circles $N to land a critical strike.", ch, nullptr, victim, TO_NOTVICT);

		check_improve(ch, gsn_circle, true, 1);

		dam = dice(obj->value[1], obj->value[2]);
		dam += 40;

		if (ch->level <= 15)
			dam *= 1;
		else if (ch->level <= 20)
			dam *= 3 / 2;
		else if (ch->level < 25)
			dam *= 2;
		else if (ch->level < 30)
			dam *= 7 / 3;
		else if (ch->level < 40)
			dam *= 5 / 2;
		else if (ch->level <= 49)
			dam *= 7 / 2;
		else if (ch->level <= 55)
			dam *= 10 / 3;
		else
			dam *= 10 / 3;

		damage_old(ch, victim, dam, gsn_circle, attack_table[obj->value[3]].damage, true);
	}
	else
	{
		check_improve(ch, gsn_circle, false, 1);
		damage_old(ch, victim, 0, gsn_circle, DAM_NONE, true);
	}
}

void do_blackjack(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	AFFECT_DATA af;
	CHAR_DATA *victim;
	int chance, size;
	char buf[MAX_STRING_LENGTH];

	if (is_npc(ch))
		return;

	one_argument(argument, arg);

	chance = get_skill(ch, gsn_blackjack);

	if (chance == 0 || (ch->level < skill_table[gsn_blackjack].skill_level[ch->Class()->GetIndex()]))
	{
		send_to_char("You don't know how to blackjack.\n\r", ch);
		return;
	}

	if (arg[0] == '\0')
	{
		send_to_char("Blackjack who?\n\r", ch);
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
		send_to_char("Amusing, but not possible.\n\r", ch);
		return;
	}

	if (is_safe(ch, victim))
		return;

	if (check_sidestep(ch, victim, gsn_blackjack, 95))
		return;

	if (check_blade_barrier(ch, victim))
		return;

	update_pc_last_fight(ch, victim);

	size = victim->size - ch->size;

	if (size >= 2)
	{
		send_to_char("That person is too large to blackjack.\n\r", ch);
		return;
	}

	if (victim->position == POS_SLEEPING)
	{
		send_to_char("But that person is already asleep!\n\r", ch);
		return;
	}

	if (is_affected(victim, gsn_blackjack_timer) || is_affected(victim, gsn_blackjack))
	{
		send_to_char("They are guarding their head too well right now.\n\r", ch);
		return;
	}

	chance *= 75;
	chance /= 90;
	chance += (ch->level - victim->level);

	if (size > 0)
		chance -= 15 * size;

	if (is_affected_by(victim, AFF_HASTE))
		chance += 10;

	if (get_eq_char(victim, WEAR_HEAD) != nullptr)
		chance -= 10;

	if (is_affected(victim, gsn_cloak_form))
		chance += 10;

	if (victim->position == POS_FIGHTING)
		chance -= 30;

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.level = ch->level;
	af.owner = ch;
	af.location = 0;
	af.type = gsn_blackjack;
	af.modifier = 0;
	af.aftype = AFT_SKILL;
	af.mod_name = MOD_CONC;

	if (ch->level == MAX_LEVEL)
		chance = 100;

	if (number_percent() < chance)
	{
		act("You whack $N over the head with a heavy looking sack. Ouch.", ch, 0, victim, TO_CHAR);
		act("$n whacks $N over the head with a heavy looking sack. Ouch.", ch, 0, victim, TO_NOTVICT);
		send_to_char("You feel a sudden pain erupt through the back of your skull.\n\r", victim);

		af.duration = 2;

		SET_BIT(af.bitvector, AFF_SLEEP);

		affect_to_char(victim, &af);

		af.type = gsn_blackjack;
		af.aftype = AFT_INVIS;
		af.duration = 4;

		zero_vector(af.bitvector);

		affect_to_char(victim, &af);

		stop_fighting(victim, true);
		victim->position = POS_SLEEPING;

		check_improve(ch, gsn_blackjack, true, 1);

		WAIT_STATE(ch, 2 * PULSE_VIOLENCE);
		return;
	}
	else
	{
		act("You attempt to blackjack $N but fail.", ch, 0, victim, TO_CHAR);
		act("$n attempts to blackjack $N but misses.", ch, 0, victim, TO_NOTVICT);
		act("$n hits you over the head with a heavy sack.", ch, 0, victim, TO_VICT);

		af.duration = 2;
		affect_to_char(victim, &af);

		check_improve(ch, gsn_blackjack, false, 1);
		damage_old(ch, victim, 2 + 2 * number_range(2, 5), gsn_blackjack, DAM_BASH, true);

		WAIT_STATE(ch, 2 * PULSE_VIOLENCE);

		if (!is_npc(victim))
		{
			sprintf(buf, "Help! %s tried to blackjack me!", pers(ch, victim));
			do_myell(victim, buf, ch);
			multi_hit(ch, victim, TYPE_UNDEFINED);
		}
	}
}

void do_ghetto_bind(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim;
	int chance;
	AFFECT_DATA af;
	char arg[MAX_INPUT_LENGTH];

	one_argument(argument, arg);

	if (is_npc(ch))
		return;

	chance = get_skill(ch, gsn_bind);

	if (chance == 0)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	if (arg[0] == '\0')
	{
		send_to_char("Attempt to bind who?\n\r", ch);
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
		send_to_char("You can't do that.\n\r", ch);
		return;
	}

	if (victim->position != POS_SLEEPING)
	{
		send_to_char("They must be sleeping for you to bind them.\n\r", ch);
		return;
	}

	if (is_affected(victim, gsn_bind))
	{
		send_to_char("They are already bound.\n\r", ch);
		return;
	}

	if (is_affected(victim, AFF_HASTE))
		chance -= 30;

	if (is_affected(ch, AFF_HASTE))
		chance += 30;

	if (get_curr_stat(victim, STAT_DEX) > get_curr_stat(ch, STAT_DEX))
		chance -= 20;

	if (number_percent() < chance)
	{
		af.aftype = AFT_SKILL;

		init_affect(&af);
		af.where = TO_AFFECTS;
		af.type = gsn_bind;
		af.level = ch->level;
		af.duration = -1;

		SET_BIT(af.bitvector, AFF_BLIND);
		af.modifier = -5;
		af.location = APPLY_DEX;
		affect_to_char(victim, &af);

		act("$n puts a sack over $N's head and ties a rope around $S legs tightly.", ch, 0, victim, TO_NOTVICT);
		act("You feel someone putting something over your head and legs.", ch, 0, victim, TO_VICT);
		act("You put a sack over $N's head and tie a rope around $S legs tightly.", ch, 0, victim, TO_CHAR);

		WAIT_STATE(ch, PULSE_VIOLENCE);
		check_improve(ch, gsn_bind, true, 1);
	}
	else
	{
		init_affect(&af);
		af.where = TO_AFFECTS;
		af.aftype = AFT_INVIS;
		af.type = gsn_bind;
		af.level = ch->level;
		af.duration = 3;
		af.modifier = 0;
		af.location = APPLY_NONE;
		affect_to_char(victim, &af);

		act("$n tries to put a sack over $N's head but it rips.", ch, 0, victim, TO_NOTVICT);
		send_to_char("You feel someone trying to put something over your head and legs.\n\r", victim);

		act("You try to put a sack over $N's head but it rips.", ch, 0, victim, TO_CHAR);

		WAIT_STATE(ch, PULSE_VIOLENCE * 3);
		check_improve(ch, gsn_bind, false, 3);
	}
}

void do_ghetto_unbind(CHAR_DATA *ch, char *argument)
{
	if (!is_affected(ch, gsn_bind))
	{
		send_to_char("You are not bound.\n\r", ch);
		return;
	}

	if (number_percent() > 30)
	{
		act("You break free of the bindings on your head and legs.", ch, 0, 0, TO_CHAR);
		act("$n breaks free of the bindings on $s head and legs.", ch, 0, 0, TO_ROOM);

		affect_strip(ch, gsn_bind);
		WAIT_STATE(ch, PULSE_VIOLENCE);
	}
	else
	{
		act("You fail to break free of the bindings on your head and legs.", ch, 0, 0, TO_CHAR);
		act("$n fails to break free of the bindings on $s head and legs.", ch, 0, 0, TO_ROOM);

		WAIT_STATE(ch, PULSE_VIOLENCE);
	}
}

void do_plant(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];
	char arg1[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	OBJ_DATA *obj;
	int percent;
	int chance;

	if (get_skill(ch, gsn_plant) == 0 || ch->level < skill_table[gsn_plant].skill_level[ch->Class()->GetIndex()])
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);

	if (arg1[0] == '\0' || arg2[0] == '\0')
	{
		send_to_char("Plant what on whom?\n\r", ch);
		return;
	}

	victim = get_char_room(ch, arg2);

	if (victim == nullptr)
	{
		send_to_char("They aren't here.\n\r", ch);
		return;
	}

	if (victim == ch)
	{
		send_to_char("That's pointless.\n\r", ch);
		return;
	}

	if (is_safe(ch, victim))
		return;

	if (victim->fighting != nullptr)
	{
		send_to_char("Can't plant while that person is fighting.\n\r", ch);
		return;
	}

	if (is_cabal_guard(victim))
	{
		send_to_char("You can't do that.\n\r", ch);
		return;
	}

	check_keen_vision(ch, victim);

	WAIT_STATE(ch, skill_table[gsn_plant].beats);
	chance = get_skill(ch, gsn_plant);

	percent = number_percent();
	percent += 25;
	percent += ((victim->level - ch->level) * 2);

	if (is_awake(victim))
	{
		percent -= get_curr_stat(ch, STAT_DEX);
		percent += get_curr_stat(victim, STAT_INT) / 2;
		percent += get_curr_stat(victim, STAT_DEX) / 3;
	}

	if (is_npc(victim) && victim->pIndexData->pShop)
		chance = 0;

	if (!is_awake(victim))
		chance += 100;

	obj = get_obj_carry(ch, arg1, ch);

	if (obj == nullptr)
	{
		send_to_char("You can't find it.\n\r", ch);
		return;
	}

	if (percent > chance)
	{
		send_to_char("Oops.\n\r", ch);
		affect_strip(ch, gsn_sneak);

		REMOVE_BIT(ch->affected_by, AFF_SNEAK);

		check_improve(ch, gsn_plant, false, 2);

		act("$n tried to slip $p into your inventory.\n\r", ch, obj, victim, TO_VICT);
		act("$n tried to slip $p into $N's inventory.\n\r", ch, obj, victim, TO_NOTVICT);

		if (!is_npc(ch))
		{
			if (is_npc(victim) && is_awake(victim))
			{
				multi_hit(victim, ch, TYPE_UNDEFINED);
			}
			else
			{
				sprintf(buf, "$N tried to plant %s on %s.", obj->name, victim->name);
				wiznet(buf, ch, nullptr, WIZ_FLAGS, 0, 0);
			}
		}

		return;
	}

	if (!can_drop_obj(ch, obj) || IS_SET(obj->extra_flags, ITEM_INVENTORY))
	{
		send_to_char("You can't let go of it.\n\r", ch);
		return;
	}

	if (victim->carry_number + get_obj_number(obj) > can_carry_n(victim))
	{
		send_to_char("They have their hands full.\n\r", ch);
		return;
	}

	if (victim->carry_weight + get_obj_weight(obj) > can_carry_w(victim))
	{
		send_to_char("They can't carry that much weight.\n\r", ch);
		return;
	}

	obj_from_char(obj);
	obj_to_char(obj, victim);
	check_improve(ch, gsn_plant, true, 2);

	act("Without $N noticing, you slip $p into $S inventory!", ch, obj, victim, TO_CHAR);
}

void do_ungag(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *vch;
	vch = get_char_room(ch, argument);

	if (vch == nullptr)
		vch = ch;

	if (!is_npc(vch) && vch != ch)
	{
		send_to_char("You cannot undo their gag.\n\r", ch);
		return;
	}

	if (!is_affected(vch, gsn_gag))
	{
		send_to_char("There is no gag to remove.\n\r", ch);
		return;
	}

	if (check_bind(vch, "arms") != nullptr)
	{
		act("$T arms are bound too tightly for you to reach $T gag!\n\r", ch, 0, vch == ch ? "your" : "their", TO_CHAR);
		return;
	}

	if (number_percent() > 30)
	{
		act("You tear away the gag from $T mouth.", ch, 0, vch == ch ? "your" : "their", TO_CHAR);

		if (vch == ch)
			act("$n tears away the gag from $s mouth.", ch, 0, 0, TO_ROOM);
		else
			act("$n tears away the gag from $N's mouth.", ch, 0, vch, TO_ROOM);

		affect_strip(ch, gsn_gag);
		WAIT_STATE(ch, PULSE_VIOLENCE);
	}
	else
	{
		act("The gag is tied too tightly and you fail to remove it.", ch, 0, 0, TO_CHAR);

		if (vch == ch)
			act("$n fails to remove $s gag.", ch, 0, 0, TO_ROOM);
		else
			act("$n fails to remove $N's gag.", ch, 0, vch, TO_ROOM);

		WAIT_STATE(ch, PULSE_VIOLENCE);
	}
}

void do_gag(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim = nullptr;
	AFFECT_DATA af, *paf;
	char arg[MSL];
	int chance;

	one_argument(argument, arg);

	chance = get_skill(ch, gsn_gag);

	if (chance == 0 || (ch->level < skill_table[gsn_gag].skill_level[ch->Class()->GetIndex()]))
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	if (arg[0] == '\0')
	{
		send_to_char("Gag who?\n\r", ch);
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
		send_to_char("Amusing, but not possible.\n\r", ch);
		return;
	}

	if (is_safe(ch, victim))
		return;

	if (is_awake(victim))
	{
		send_to_char("You can only gag people who are unconscious.\n\r", ch);
		return;
	}

	if (is_affected(victim, gsn_gag))
	{
		paf = affect_find(ch->affected, gsn_gag);

		if (paf && paf->aftype == AFT_SKILL)
		{
			send_to_char("They are already gagged!\n\r", ch);
			return;
		}
		else
		{
			send_to_char("You can not gag them again so soon.\n\r", ch);
			return;
		}
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = gsn_gag;
	af.level = ch->level;
	af.mod_name = MOD_SPEECH;

	if (number_percent() < chance)
	{
		act("You firmly place a gag around $N's mouth.", ch, nullptr, victim, TO_CHAR);
		act("You feel someone place a gag around your mouth.", ch, nullptr, victim, TO_VICT);
		act("$n places a gag around $N's mouth.", ch, nullptr, victim, TO_NOTVICT);

		af.aftype = AFT_SKILL;
		af.duration = -1;

		check_improve(ch, gsn_gag, true, 2);
		affect_to_char(victim, &af);
	}
	else
	{
		act("You try to place a gag around $N's mouth, but it breaks.", ch, nullptr, victim, TO_CHAR);
		act("You feel someone try to place a gag around your mouth, but the tightness goes away.", ch, nullptr, victim, TO_VICT);
		act("$n tries to place a gag around $N's mouth, but it breaks.", ch, nullptr, victim, TO_NOTVICT);
		check_improve(ch, gsn_gag, true, 2);
	}

	WAIT_STATE(ch, PULSE_VIOLENCE * 2);
}

bool check_drag(CHAR_DATA *ch, ROOM_INDEX_DATA *to_room, int where)
{
	return true;
}

void do_drag(CHAR_DATA *ch, char *argument)
{
	int where, skill;
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	CHAR_DATA *victim = nullptr;
	char *direction;
	char store[MSL], target[MSL], dir[MSL];

	/* Must have the skill to use it */
	skill = get_skill(ch, gsn_drag);

	if (skill == 0)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	/* break up the argument into two variables */
	if (!*argument)
	{
		send_to_char("Who are you trying to drag where?\n\r", ch);
		return;
	}

	argument = one_argument(argument, target);
	argument = one_argument(argument, dir);

	if (!*target)
	{
		send_to_char("Who are you trying to drag?\n\r", ch);
		return;
	}

	if (!*dir)
	{
		send_to_char("Where are you trying to drag them?\n\r", ch);
		return;
	}

	/* Set victim */
	victim = get_char_room(ch, target);

	if (victim == nullptr)
	{
		send_to_char("They aren't here.\n\r", ch);
		return;
	}

	/* Check for pk range */
	if (is_safe_new(ch, victim, true))
		return;

	if (is_awake(victim))
	{
		send_to_char("You can only drag unconscious people around.\n\r", ch);
		return;
	}

	if (is_npc(victim) && IS_SET(victim->act, ACT_SENTINEL))
	{
		act("You can't seem to move $N from $S position!", ch, 0, victim, TO_CHAR);
		return;
	}

	if (victim->size > (ch->size + 1))
	{
		/* Can't push yourself */
		if (victim == ch)
		{
			send_to_char("You can't drag yourself.\n\r", ch);
			return;
		}
	}

	if (ch->fighting)
	{
		send_to_char("You can't drag anyone while you are fighting!\n\r", ch);
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
		send_to_char("You cannot drag anyone that direction.\n\r", ch);
		return;
	}

	if (is_npc(victim) && victim->pIndexData->barred_entry)
	{
		send_to_char("They are too firmly in place to drag around.\n\r", ch);
		return;
	}

	if (check_barred(ch, pexit->u1.to_room))
		return;

	if (check_barred(ch, pexit->u1.to_room))
	{
		send_to_char("You were unable to drag them away.\n\r", ch);
		return;
	}

	if (!is_npc(ch) && !is_npc(victim) && (ch->fighting == nullptr || victim->fighting == nullptr))
	{
		sprintf(store, "Help! %s is dragging me around!", pers(ch, victim));
		do_myell(victim, store, ch);
	}

	skill -= (victim->size - ch->size) * 15;

	if (number_percent() < skill)
	{
		act("You grapple $N, and try to drag $M to the $t!", ch, dir, victim, TO_CHAR);
		act("$n grapples $N, and tries to drag $M to the $t!", ch, dir, victim, TO_NOTVICT);

		to_room = pexit->u1.to_room;

		move_char(ch, where, true, false);

		char_from_room(victim);
		char_to_room(victim, to_room);

		do_look(victim, "auto");

		if (victim->in_room != ch->in_room)
			return;

		act("$n drags $N in from the $t!", ch, dir, victim, TO_NOTVICT);
		check_improve(ch, gsn_drag, true, 1);
	}
	else
	{
		act("You grapple $N, and try to drag $M but $E doesn't budge!", ch, 0, victim, TO_CHAR);
		act("$n grapples $N, and tries to drag $M away but $N doesn't budge!", ch, 0, victim, TO_NOTVICT);
		check_improve(ch, gsn_drag, false, 1);
	}

	WAIT_STATE(ch, PULSE_VIOLENCE * 2);
}

void do_tripwire(CHAR_DATA *ch, char *argument)
{
	ROOM_INDEX_DATA *to_room;
	AFFECT_DATA af;
	ROOM_AFFECT_DATA raf;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_opp;
	CHAR_DATA *victim;
	char *direction;
	int skill, door = *((int *)argument);

	skill = get_skill(ch, gsn_tripwire);

	if (skill == 0)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	if (ch->in_room->sector_type != SECT_CITY && ch->in_room->sector_type != SECT_INSIDE && !is_immortal(ch))
	{
		send_to_char("You can only lay tripwires in cities.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_tripwire))
	{
		send_to_char("You can not setup another tripwire so soon.\n\r", ch);
		return;
	}

	pexit = ch->in_room->exit[door];
	to_room = pexit->u1.to_room;

	if (is_affected_room(ch->in_room, gsn_tripwire) && is_affected_room(to_room, gsn_tripwire))
	{
		send_to_char("A wire is already in place that way.\n\r", ch);
		return;
	}

	if (number_percent() < skill)
	{
		direction = flag_name_lookup(door, direction_table);

		act("You lay a concealed tripwire across the exit $Tward and draw it taut.", ch, 0, direction, TO_CHAR);

		for (victim = ch->in_room->people; victim != nullptr; victim = victim->next_in_room)
		{
			if (can_see(victim, ch))
				act("$n lays a concealed tripwire across the exit $tward and draws it taut.", ch, direction, victim, TO_VICT);
		}

		init_affect_room(&raf);
		raf.where = TO_ROOM_AFFECTS;
		raf.type = gsn_tripwire;
		raf.aftype = AFT_SKILL;
		raf.level = ch->level;
		raf.duration = 5;
		raf.owner = ch;
		raf.modifier = door;
		new_affect_to_room(ch->in_room, &raf);

		if (to_room != nullptr
			&& (pexit_opp = to_room->exit[reverse_d(door)]) != nullptr
			&& pexit_opp->u1.to_room == ch->in_room)
		{
			raf.modifier = reverse_d(door);
			new_affect_to_room(to_room, &raf);
		}

		init_affect(&af);
		af.where = TO_AFFECTS;
		af.type = gsn_tripwire;
		af.level = ch->level;
		af.aftype = AFT_SKILL;
		af.duration = ch->level / 2;
		affect_to_char(ch, &af);

		check_improve(ch, gsn_tripwire, true, 1);
	}
	else
	{
		check_improve(ch, gsn_tripwire, false, 1);
	}
}

void do_sign(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim;
	char buf[MAX_STRING_LENGTH];
	char fail[MAX_STRING_LENGTH];

	if (get_skill(ch, gsn_sign) == 0)
	{
		send_to_char("You don't know how to sign.\n\r", ch);
		return;
	}

	if (argument[0] == '\0')
	{
		send_to_char("Sign what?\n\r", ch);
		return;
	}

	check_ooc(ch, argument, "SIGN");

	sprintf(buf, "You sign '%s$T%s'", get_char_color(ch, "cyan"), END_COLOR(ch));

	argument[700] = '\0';

	act(buf, ch, nullptr, argument, TO_CHAR);

	switch (number_range(0, 6))
	{
		case 0:
			sprintf(fail, "$n rubs the bridge of $s nose.");
			break;
		case 1:
			sprintf(fail, "$n taps $s forehead several times.");
			break;
		case 2:
			sprintf(fail, "$n points at $s eyes and blinks.");
			break;
		case 3:
			sprintf(fail, "$n quickly twists $s right ear.");
			break;
		case 4:
			sprintf(fail, "$n taps the back of $s neck.");
			break;
		case 5:
			sprintf(fail, "$n quickly twists $s left ear.");
			break;
		case 6:
			sprintf(fail, "$n rapidly taps $s chin.");
			break;
	}

	for (victim = ch->in_room->people; victim != nullptr; victim = victim->next_in_room)
	{
		if (is_awake(victim))
		{
			{
				if (number_percent() < get_skill(ch, gsn_sign)
					&& number_percent() < get_skill(victim, gsn_sign))
				{
					sprintf(buf, "$n signs '%s$t%s'", get_char_color(victim, "speech"), END_COLOR(victim));
					act(buf, ch, argument, victim, TO_VICT);
				}
				else
				{
					act(fail, ch, argument, victim, TO_VICT);
				}
			}
		}
	}

	check_improve(ch, gsn_sign, true, 1);
	WAIT_STATE(ch, skill_table[gsn_sign].beats);
}

void do_slash(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];
	char arg1[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	OBJ_DATA *obj, *iobj, *iobj_next;
	int percent;
	int chance;

	if (get_skill(ch, gsn_slash) == 0)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);

	if (arg1[0] == '\0' || arg2[0] == '\0')
	{
		send_to_char("Slash what from whom?\n\r", ch);
		return;
	}

	victim = get_char_room(ch, arg2);

	if (victim == nullptr)
	{
		send_to_char("They aren't here.\n\r", ch);
		return;
	}

	if (victim == ch)
	{
		send_to_char("Why would you want to do that?\n\r", ch);
		return;
	}

	if (is_safe(ch, victim))
		return;

	if (victim->fighting != nullptr)
	{
		send_to_char("You can't do that while they are fighting.\n\r", ch);
		return;
	}

	if (!is_wielded(ch, WEAPON_SWORD, WIELD_PRIMARY)
		&& !is_wielded(ch, WEAPON_SWORD, WIELD_TWO)
		&& !is_wielded(ch, WEAPON_DAGGER, WIELD_PRIMARY)
		&& !is_wielded(ch, WEAPON_DAGGER, WIELD_TWO))
	{
		send_to_char("You must be using a dagger or sword in either hand to slash!\n\r", ch);
		return;
	}

	/* before item check to prevent spamming */
	WAIT_STATE(ch, skill_table[gsn_slash].beats);

	obj = get_obj_carry(victim, arg1, ch);

	if (obj == nullptr)
	{
		obj = get_obj_wear(victim, arg1);

		if (obj == nullptr)
		{
			send_to_char("They are not carrying or wearing that.\n\r", ch);
			return;
		}
	}

	if (obj->item_type != ITEM_CONTAINER)
	{
		send_to_char("That's not a container.\n\r", ch);
		return;
	}

	if (material_table[obj->pIndexData->material_index].mat_hardness > 2)
	{
		send_to_char("You can't slash that.\n\r", ch);
		return;
	}

	chance = (int)((float)get_skill(ch, gsn_slash) * 0.98);

	percent = number_percent();

	if (is_awake(victim))
	{
		percent -= get_curr_stat(ch, STAT_DEX) * 2;
		percent += get_curr_stat(victim, STAT_DEX) * 2;
	}
	else
	{
		chance -= 75;
	}

	if (percent > chance)
	{
		send_to_char("Oops.\n\r", ch);
		affect_strip(ch, gsn_sneak);

		REMOVE_BIT(ch->affected_by, AFF_SNEAK);

		check_improve(ch, gsn_slash, false, 2);

		act("$n tried to slash your belongings.\n\r", ch, nullptr, victim, TO_VICT);
		act("$n tried to slash $N's belongings.\n\r", ch, nullptr, victim, TO_NOTVICT);

		thief_yell(ch, victim);
		return;
	}

	if (obj->item_type != ITEM_CONTAINER)
	{
		send_to_char("That's not a container.\n\r", ch);
		return;
	}

	if (material_table[obj->pIndexData->material_index].mat_hardness > 2)
	{
		send_to_char("You can't slash that.\n\r", ch);
		return;
	}

	act("You sneak close to $N and slash $p, spilling its contents!", ch, obj, victim, TO_CHAR);
	act("$n sneaks close to you and slashes $p, spilling its contents!", ch, obj, victim, TO_VICT);
	act("$n sneaks close to $N and slashes $p, spilling its contents!", ch, obj, victim, TO_NOTVICT);

	check_improve(ch, gsn_slash, true, 1);

	for (iobj = obj->contains; iobj != nullptr; iobj = iobj_next)
	{
		iobj_next = iobj->next_content;
		obj_from_char(iobj);
		obj_to_room(iobj, ch->in_room);
	}

	extract_obj(obj);

	/*
	if (is_awake(victim) && !is_npc(victim))
	{
		sprintf(buf,"Help! %s just slashed my sack!", pers(ch,victim));
		do_myell(victim, buf,ch);
		multi_hit(victim, ch, TYPE_UNDEFINED);
	}
	*/
}

void do_stash(CHAR_DATA *ch, char *argument)
{
	char arg[MSL];
	OBJ_DATA *obj;
	OBJ_AFFECT_DATA oaf;
	int skill;

	skill = get_skill(ch, gsn_stash);

	if (skill == 0)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	if (ch->in_room->sector_type != SECT_CITY && ch->in_room->sector_type != SECT_INSIDE && !is_immortal(ch))
	{
		send_to_char("You can only stash things in cities.\n\r", ch);
		return;
	}

	argument = one_argument(argument, arg);

	obj = get_obj_carry(ch, arg, ch);

	if (obj == nullptr)
	{
		send_to_char("You can't find it.\n\r", ch);
		return;
	}

	act("You attempt to stash $p away.", ch, obj, 0, TO_CHAR);

	if (number_percent() < skill)
	{
		init_affect_obj(&oaf);
		oaf.where = TO_OBJ_AFFECTS;
		oaf.type = gsn_stash;
		oaf.aftype = AFT_SKILL;
		oaf.level = ch->level;
		oaf.duration = -1;
		oaf.owner = ch;
		affect_to_obj(obj, &oaf);

		check_improve(ch, gsn_stash, true, 1);
	}
	else
	{
		check_improve(ch, gsn_stash, false, 1);
	}

	obj_from_char(obj);
	obj_to_room(obj, ch->in_room);
}

void do_disguise(CHAR_DATA *ch, char *argument)
{
	OBJ_DATA *corpse;
	OBJ_AFFECT_DATA oaf;
	AFFECT_DATA af;
	MOB_INDEX_DATA *mob = nullptr;
	char arg[MSL];
	int skill;

	skill = get_skill(ch, gsn_disguise);

	if (skill == 0)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	argument = one_argument(argument, arg);

	corpse = get_obj_here(ch, arg);

	if (!corpse || !str_cmp(arg, ""))
	{
		send_to_char("You don't see that here.\n\r", ch);
		return;
	}

	if (corpse->item_type == ITEM_CORPSE_PC)
	{
		send_to_char("You'd hardly be able to blend in looking like that.\n\r", ch);
		return;
	}

	if (corpse->item_type != ITEM_CORPSE_NPC)
	{
		send_to_char("That's not a corpse!\n\r", ch);
		return;
	}

	mob = get_mob_index(corpse->mob_vnum);

	if (ch->race != mob->race)
	{
		send_to_char("You have to at least look similar to the deceased!\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_disguise))
	{
		send_to_char("You are already disguised!\n\r", ch);
		return;
	}

	if (is_affected_obj(corpse, gsn_disguise))
	{
		send_to_char("That corpse has already been robbed of its garb.\n\r", ch);
		return;
	}

	act("You strip $p of its garb and try to disguise yourself.", ch, corpse, 0, TO_CHAR);
	act("$n strips $p of its garb and tries to disguise $mself.", ch, corpse, 0, TO_ROOM);

	init_affect_obj(&oaf);
	oaf.where = TO_OBJ_AFFECTS;
	oaf.type = gsn_disguise;
	oaf.aftype = AFT_SKILL;
	oaf.level = ch->level;
	oaf.duration = -1;
	oaf.owner = ch;
	affect_to_obj(corpse, &oaf);

	if (number_percent() < skill)
	{
		ch->pcdata->old = new_oldchar();
		ch->pcdata->old->name = palloc_string(ch->true_name);
		ch->pcdata->old->short_descr = palloc_string(ch->short_descr);
		ch->pcdata->old->long_descr = palloc_string(ch->long_descr);
		ch->pcdata->old->description = palloc_string(ch->description);

		free_pstring(ch->name);
		ch->name = palloc_string(mob->short_descr);

		free_pstring(ch->short_descr);
		ch->short_descr = palloc_string("disguised");

		free_pstring(ch->long_descr);
		ch->long_descr = palloc_string(mob->long_descr);

		free_pstring(ch->description);
		ch->description = palloc_string(mob->description);

		init_affect(&af);
		af.where = TO_AFFECTS;
		af.type = gsn_disguise;
		af.owner = ch;
		af.level = ch->level;
		af.aftype = AFT_INVIS;
		af.duration = ch->level;
		af.modifier = corpse->mob_vnum;
		af.end_fun = disguise_end;
		af.pulse_fun = disguise_pulse;
		af.mod_name = MOD_APPEARANCE;
		affect_to_char(ch, &af);

		check_improve(ch, gsn_disguise, true, 1);
	}
	else
	{
		check_improve(ch, gsn_disguise, false, 1);
	}

	WAIT_STATE(ch, PULSE_VIOLENCE * 2);
}

void disguise_end(CHAR_DATA *ch, AFFECT_DATA *af)
{
	disguise_remove(ch);
}

void disguise_pulse(CHAR_DATA *ch, AFFECT_DATA *af)
{
	if (ch->fighting && number_percent() < 25)
	{
		act("Your shoddy disguise comes apart, the tatters falling away and revealing you!", ch, 0, 0, TO_CHAR);
		act("$n's garb falls to tatters around $m...  revealing $t beneath the disguise!", ch, ch->pcdata->old->name, 0, TO_ROOM);
		affect_strip(ch, gsn_disguise);
		return;
	}

	if (af->modifier < ch->in_room->area->min_vnum || af->modifier > ch->in_room->area->max_vnum)
	{
		act("Your disguise ceases to be effective as you leave the vicinity.", ch, 0, 0, TO_CHAR);
		affect_strip(ch, gsn_disguise);
	}
}

void disguise_remove(CHAR_DATA *ch)
{
	free_pstring(ch->name);
	ch->name = palloc_string(ch->pcdata->old->name);

	free_pstring(ch->short_descr);
	ch->short_descr = palloc_string(ch->pcdata->old->short_descr);

	free_pstring(ch->long_descr);
	ch->long_descr = palloc_string(ch->pcdata->old->long_descr);

	free_pstring(ch->description);
	ch->description = palloc_string(ch->pcdata->old->description);

	free_oldchar(ch->pcdata->old);
	ch->pcdata->old = nullptr;
}

void do_undisguise(CHAR_DATA *ch, char *argument)
{
	if (!is_affected(ch, gsn_disguise) || !ch->pcdata->old)
	{
		if (get_skill(ch, gsn_disguise) < 1)
		{
			send_to_char("Huh?\n\r", ch);
			return;
		}
		else
		{
			send_to_char("You are not disguised!\n\r", ch);
			return;
		}
	}

	act("$n suddenly slips out of $s garb... revealing $t beneath the disguise!", ch, ch->pcdata->old->name, 0, TO_ROOM);
	act("You deftly slip out of your disguise and cast it aside.", ch, 0, 0, TO_CHAR);

	affect_strip(ch, gsn_disguise);

	WAIT_STATE(ch, PULSE_VIOLENCE * 2);
}

void do_search(CHAR_DATA *ch, char *argument)
{
	OBJ_DATA *obj = nullptr;
	OBJ_AFFECT_DATA *oaf;
	int chance;

	act("You intently scrutinize your surroundings...", ch, 0, 0, TO_CHAR);
	act("$n intently scrutinizes $s surroundings...", ch, 0, 0, TO_ROOM);

	chance = number_percent();

	for (obj = ch->in_room->contents; obj != nullptr; obj = obj->next_content)
	{
		if (is_affected_obj(obj, gsn_stash))
		{
			for (oaf = obj->affected; oaf != nullptr; oaf = oaf->next)
			{
				if (oaf->type == gsn_stash)
					break;

				chance += (ch->level - oaf->level);

				if (ch->Class()->GetIndex() == CLASS_THIEF)
					chance += (ch->level / 2);

				if (oaf->owner != ch && (number_percent() < chance))
				{
					affect_strip_obj(obj, gsn_stash);
					act("You stumble across $p which seemed to be hidden from your eye.", ch, obj, 0, TO_CHAR);
					act("$n stumbles across and reveals $p.", ch, obj, 0, TO_ROOM);
				}
			}
		}
	}

	WAIT_STATE(ch, PULSE_VIOLENCE * 2);
}

void do_counterfeit(CHAR_DATA *ch, char *argument)
{
	OBJ_DATA *orig, *copy;
	char arg1[MSL], arg2[MSL], hold[MSL];
	int skill;
	OBJ_AFFECT_DATA oaf;

	skill = get_skill(ch, gsn_counterfeit);

	if (skill == 0)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);

	if (arg1[0] == '\0' || arg2[0] == '\0')
	{
		send_to_char("Syntax: counterfeit <original item> <base item for forgery>\n\r", ch);
		return;
	}

	orig = get_obj_carry(ch, arg1, ch);

	if (orig == nullptr)
	{
		send_to_char("You can't find the original item.\n\r", ch);
		return;
	}

	copy = get_obj_carry(ch, arg2, ch);

	if (copy == nullptr)
	{
		send_to_char("You can't find the copy item.\n\r", ch);
		return;
	}

	if (orig->item_type != copy->item_type)
	{
		send_to_char("Even with your skill, you can't make those two look alike.\n\r", ch);
		return;
	}

	if (str_cmp(orig->material, copy->material))
	{
		send_to_char("Those two items are not even the same material.\n\r", ch);
		return;
	}

	if (number_percent() < skill)
	{
		act("You successfully counterfeit $p.", ch, orig, 0, TO_CHAR);

		free_pstring(copy->short_descr);
		copy->short_descr = palloc_string(orig->short_descr);

		free_pstring(copy->name);
		copy->name = palloc_string(orig->name);

		free_pstring(copy->description);
		copy->description = palloc_string(orig->description);

		check_improve(ch, gsn_counterfeit, true, 1);
	}
	else
	{
		act("You botch the counterfeit and ruin $p!", ch, copy, 0, TO_CHAR);

		free_pstring(copy->short_descr);
		sprintf(hold, "a shoddy imitation of %s", orig->short_descr);
		copy->short_descr = palloc_string(hold);

		free_pstring(copy->name);
		copy->name = palloc_string(orig->name);

		free_pstring(copy->description);
		copy->description = palloc_string(orig->description);

		check_improve(ch, gsn_counterfeit, false, 1);
	}

	init_affect_obj(&oaf);
	oaf.where = TO_OBJ_AFFECTS;
	oaf.type = gsn_counterfeit;
	oaf.aftype = AFT_SKILL;
	oaf.level = ch->level;
	oaf.duration = ch->level;
	oaf.owner = ch;
	oaf.end_fun = counterfeit_end;
	affect_to_obj(copy, &oaf);

	WAIT_STATE(ch, PULSE_VIOLENCE * 2);
}

void counterfeit_end(OBJ_DATA *obj, OBJ_AFFECT_DATA *af)
{

	if (obj->carried_by)
	{
		act("You suddenly realize that $p is counterfeit, and really $T!",
			obj->carried_by,
			obj,
			obj->pIndexData->short_descr,
			TO_CHAR);
	}

	if (obj->in_room && obj->in_room->people)
	{
		act("You suddenly realize that $p is counterfeit, and really $T!",
			obj->carried_by,
			obj,
			obj->pIndexData->short_descr,
			TO_ALL);
	}

	free_pstring(obj->short_descr);
	obj->short_descr = palloc_string(obj->pIndexData->short_descr);

	free_pstring(obj->name);
	obj->name = palloc_string(obj->pIndexData->name);

	free_pstring(obj->description);
	obj->description = palloc_string(obj->pIndexData->description);
}

void do_shadow_cloak(CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;
	int skill, cost;

	skill = get_skill(ch, gsn_shadow_cloak);

	if (skill == 0)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_shadow_cloak))
	{
		send_to_char("You are already cloaked!\n\r", ch);
		return;
	}

	cost = skill_table[gsn_shadow_cloak].min_mana;

	if ((ch->mana - cost) < 0)
	{
		send_to_char("You do not have the mana.\n\r", ch);
		return;
	}

	if (number_percent() < skill)
	{
		act("You sharpen your mind and attempt to shield yourself from scrying magic.", ch, 0, 0, TO_CHAR);

		init_affect(&af);
		af.where = TO_AFFECTS;
		af.type = gsn_shadow_cloak;
		af.level = ch->level;
		af.duration = ch->level / 2;
		af.aftype = AFT_SKILL;
		affect_to_char(ch, &af);
		check_improve(ch, gsn_shadow_cloak, true, 1);
	}
	else
	{
		act("You sharpen your mind and attempt to shield yourself from scrying magic, but you fail.", ch, 0, 0, TO_CHAR);
		check_improve(ch, gsn_shadow_cloak, false, 1);
	}

	WAIT_STATE(ch, PULSE_VIOLENCE * 2);
}

bool check_stealth(CHAR_DATA *ch, CHAR_DATA *mob)
{
	int skill;

	if (!is_affected_by(ch, AFF_SNEAK))
		return false;

	if (mob->last_fought == ch)
		return false;

	skill = get_skill(ch, gsn_stealth);

	if (skill == 0)
		return false;

	if (IS_SET(mob->act, ACT_DETECT_SPECIAL))
		return false;

	skill += (get_curr_stat(ch, STAT_DEX) - 20) * 3;

	if (number_percent() < skill)
	{
		check_improve(ch, gsn_stealth, true, 5);
		return true;
	}
	else
	{
		act("You stumble slightly and $N notices you!", ch, 0, mob, TO_CHAR);
		check_improve(ch, gsn_stealth, false, 3);
		return false;
	}
}

void do_strip(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim;
	OBJ_DATA *obj;
	char arg1[MSL], arg2[MSL];
	int chance;

	if (argument[0] == '\0')
	{
		send_to_char("Strip what from whom?\n\r", ch);
		return;
	}

	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);

	chance = get_skill(ch, gsn_strip);

	if (chance == 0)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	if (arg2[0] == '\0')
	{
		send_to_char("Strip whom?\n\r", ch);
		return;
	}

	victim = get_char_room(ch, arg2);

	if (victim == nullptr)
	{
		send_to_char("They aren't here.\n\r", ch);
		return;
	}

	if (victim == ch)
	{
		send_to_char("That would be silly.\n\r", ch);
		return;
	}

	if (arg1[0] == '\0')
	{
		send_to_char("Strip what?\n\r", ch);
		return;
	}

	for (obj = victim->carrying; obj; obj = obj->next_content)
	{
		if (obj->wear_loc != WEAR_NONE && is_name(arg1, obj->name))
			break;
	}

	if (!obj)
	{
		act("$N is not wearing $t.", ch, arg1, victim, TO_CHAR);
		return;
	}

	if (obj->wear_loc == WEAR_BODY
		|| obj->wear_loc == WEAR_ARMS
		|| obj->wear_loc == WEAR_LEGS
		|| obj->wear_loc == WEAR_WAIST
		|| obj->wear_loc == WEAR_ABOUT
		|| obj->wear_loc == WEAR_BRAND)
	{
		act("You couldn't possibly strip that!", ch, obj, victim, TO_CHAR);
		return;
	}

	chance /= 2;
	chance += 6 * (get_curr_stat(ch, STAT_DEX) - 20);

	if (is_npc(victim))
		chance -= 5 * (victim->level - ch->level);
	else
		chance -= 2 * (victim->level - ch->level);

	if (can_see(victim, ch))
		chance -= 25;
	else
		chance += 5;

	if (!is_affected_by(ch, AFF_SNEAK))
		chance -= 10;

	if (!is_awake(victim))
		chance += 25;
	else
		chance -= 2 * (get_curr_stat(victim, STAT_DEX) - 15);

	if (is_npc(victim) && victim->pIndexData->pShop)
		chance = 0;

	if ((is_awake(victim)
			&& (obj->wear_loc == WEAR_FINGER_L || obj->wear_loc == WEAR_FINGER_R))
		|| (!is_awake(victim)))
	{
		if (number_percent() < (chance - 30))
		{
			unequip_char(victim, obj, true);

			obj_from_char(obj);
			obj_to_char(obj, ch);

			act("You silently strip $p from $N.", ch, obj, victim, TO_CHAR);

			check_improve(ch, gsn_strip, true, 1);

			if (number_range(1, 5) > 3)
			{
				act("You botch your strip, still managing to get $p, but $N wakes up!", ch, obj, victim, TO_CHAR);
				act("$n botches $s strip, still managing to get $p from you!", ch, obj, victim, TO_VICT);
				thief_yell(ch, victim);
			}
		}
		else
		{
			act("Oops.", ch, 0, victim, TO_CHAR);
			thief_yell(ch, victim);
			check_improve(ch, gsn_strip, false, 1);
		}
	}
	else
	{
		act("Oops.", ch, 0, victim, TO_CHAR);
		thief_yell(ch, victim);
		check_improve(ch, gsn_strip, false, 1);
	}

	WAIT_STATE(ch, PULSE_VIOLENCE * 2);
}

void thief_yell(CHAR_DATA *ch, CHAR_DATA *victim)
{
	char buf[MSL];

	switch (number_range(0, 3))
	{
		case 0:
		case 1:
			sprintf(buf, "%s is a lousy thief!", pers(ch, victim));
			break;
		case 2:
			sprintf(buf, "%s tried to rob me!", pers(ch, victim));
			break;
		case 3:
			sprintf(buf, "Keep your hands out of there, %s!", pers(ch, victim));
			break;
	}

	if (!is_awake(victim))
	{
		victim->position = POS_STANDING;
		update_pos(victim);
	}

	if (!is_npc(victim))
		do_myell(victim, buf, ch);

	one_hit(victim, ch, TYPE_UNDEFINED);
}

AFFECT_DATA *check_bind(CHAR_DATA *ch, char *type)
{
	AFFECT_DATA *af;

	for (af = ch->affected; af != nullptr; af = af->next)
	{
		if (af->type == gsn_bind && !str_infix(type, af->name))
			return af;
	}

	return nullptr;
}

void do_bind(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim;
	char arg1[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	char buf[MAX_INPUT_LENGTH];
	AFFECT_DATA af;
	int skill = 0;

	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);

	skill = get_skill(ch, gsn_bind);

	if (skill == 0)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	if (arg1[0] == '\0')
	{
		send_to_char("Syntax: bind <target> <head/arms/legs>\n\r", ch);
		return;
	}

	if (arg2[0] == '\0')
	{
		send_to_char("Syntax: bind <target> <head/arms/legs>\n\r", ch);
		return;
	}

	if (!str_prefix(arg2, "head"))
	{
		sprintf(arg2, "head");
	}
	else if (!str_prefix(arg2, "arms"))
	{
		if (skill < 85)
		{
			send_to_char("You aren't skilled enough to do that yet.\n\r", ch);
			return;
		}

		sprintf(arg2, "arms");
	}
	else if (!str_prefix(arg2, "legs"))
	{
		if (skill < 90)
		{
			send_to_char("You aren't skilled enough to do that yet.\n\r", ch);
			return;
		}

		sprintf(arg2, "legs");
	}
	else
	{
		send_to_char("Syntax: bind <target> <head/arms/legs>\n\r", ch);
		return;
	}

	victim = get_char_room(ch, arg1);

	if (victim == nullptr)
	{
		send_to_char("They aren't here.\n\r", ch);
		return;
	}

	if (victim == ch)
	{
		send_to_char("You can't do that.\n\r", ch);
		return;
	}

	if (is_npc(victim) && !IS_SET(victim->form, FORM_BIPED))
	{
		send_to_char("You can't do that to them.\n\r", ch);
		return;
	}

	if (victim->position != POS_SLEEPING)
	{
		send_to_char("They must be sleeping for you to bind them.\n\r", ch);
		return;
	}

	if (is_safe(ch, victim))
		return;

	if (!str_cmp(arg2, "head"))
	{
		if (check_bind(victim, "head") != nullptr)
		{
			send_to_char("Their head is already bound.\n\r", ch);
			return;
		}
	}

	if (!str_cmp(arg2, "arms"))
	{
		if (victim->arms > 0)
		{
			if (check_bind(victim, "arms") != nullptr)
			{
				send_to_char("Their arms are already bound.\n\r", ch);
				return;
			}
		}
		else
		{
			send_to_char("They have no arms to bind!\n\r", ch);
			return;
		}
	}

	if (!str_cmp(arg2, "legs"))
	{
		if (victim->legs > 0)
		{
			if (check_bind(victim, "legs") != nullptr)
			{
				send_to_char("Their legs are already bound.\n\r", ch);
				return;
			}
		}
		else
		{
			send_to_char("They have no legs to bind!.\n\r", ch);
			return;
		}
	}

	if (number_percent() < skill)
	{
		char buf[50];

		init_affect(&af);
		af.aftype = AFT_SKILL;
		af.where = TO_AFFECTS;
		af.type = gsn_bind;
		af.level = ch->level;

		auto buffer = fmt::format("bind {}", arg2);

		af.name = palloc_string(buffer.c_str()); // TODO: not sure what palloc_string does
		af.duration = -1;

		if (!str_cmp(arg2, "head"))
		{
			SET_BIT(af.bitvector, AFF_BLIND);

			af.location = APPLY_HITROLL;
			af.modifier = -(ch->level / 5);

			act("You lift up $N's head and slide a sack over it.", ch, 0, victim, TO_CHAR);
			act_new("You feel someone putting something over your head.", ch, 0, victim, TO_VICT, POS_SLEEPING);
			act("$n lifts up $N's head and slides a sack over it.", ch, 0, victim, TO_NOTVICT);
		}
		else if (!str_cmp(arg2, "arms"))
		{
			af.location = APPLY_STR;
			af.modifier = -(ch->level / 8);

			act("Pulling $N's arms together, you tie $S wrists with a rope.", ch, 0, victim, TO_CHAR);
			act_new("You feel something painfully constrict your arms.", ch, 0, victim, TO_VICT, POS_SLEEPING);
			act("Pulling $N's arms together, $n ties $S wrists with a rope.", ch, 0, victim, TO_NOTVICT);
		}
		else if (!str_cmp(arg2, "legs"))
		{
			af.location = APPLY_DEX;
			af.modifier = -(ch->level / 5);

			act("You tie $N's legs together tightly with a rope.", ch, 0, victim, TO_CHAR);
			act_new("You feel something being tied around your legs.", ch, 0, victim, TO_VICT, POS_SLEEPING);
			act("$n ties $N's legs together tightly with a rope.", ch, 0, victim, TO_NOTVICT);
		}

		affect_to_char(victim, &af);
		WAIT_STATE(ch, PULSE_VIOLENCE * 2);
		check_improve(ch, gsn_bind, true, 1);
	}
	else
	{
		if (!str_cmp(arg2, "head"))
		{
			act("You attempt to put a sack over $N's head, but it rips.", ch, 0, victim, TO_CHAR);
			act("$n attempts to put a sack over $N's head, but it rips.", ch, 0, victim, TO_NOTVICT);
		}
		else if (!str_cmp(arg2, "arms"))
		{
			act("You attempt to tie $N's wrists together, but the rope tears.", ch, 0, victim, TO_CHAR);
			act("$n attempts to tie $N's wrists together, but the rope tears.", ch, 0, victim, TO_NOTVICT);
		}
		else if (!str_cmp(arg2, "legs"))
		{
			act("You attempt to tie $N's legs together, but the rope tears.", ch, 0, victim, TO_CHAR);
			act("$n attempts to tie $N's legs together, but the rope tears.", ch, 0, victim, TO_NOTVICT);
		}

		WAIT_STATE(ch, PULSE_VIOLENCE * 2);
		check_improve(ch, gsn_bind, false, 2);
	}

	if (!is_affected_by(victim, AFF_SLEEP))
	{
		if (!str_cmp(arg2, "head"))
			sprintf(buf, "Help! %s is putting a sack over my head!", pers(ch, victim));

		if (!str_cmp(arg2, "arms"))
			sprintf(buf, "Help! %s is tying my arms behind my back!", pers(ch, victim));

		if (!str_cmp(arg2, "legs"))
			sprintf(buf, "Help! %s is tying up my legs!", pers(ch, victim));

		if (!is_npc(victim))
			do_myell(victim, buf, ch);

		do_stand(victim, "");
		multi_hit(victim, ch, TYPE_UNDEFINED);
	}
}

void do_unbind(CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA *af = check_bind(ch, "arms");

	if (af != nullptr)
	{
		if (number_percent() < get_curr_stat(ch, STAT_STR) * 6)
		{
			act("Exerting all of your strength, you break the bindings on your arms!", ch, 0, 0, TO_CHAR);
			act("Exerting all of $s strength, $n breaks the bindings on $s arms!", ch, 0, 0, TO_ROOM);
			affect_remove(ch, af);
		}
		else
		{
			act("You struggle with the bindings on your arms, but fail to break them.", ch, 0, 0, TO_CHAR);
			act("$n struggles with the bindings on $s arms, but fails to break them.", ch, 0, 0, TO_ROOM);
		}

		WAIT_STATE(ch, PULSE_VIOLENCE);
	}
	else if ((af = check_bind(ch, "head")) != nullptr)
	{
		if (number_percent() < get_curr_stat(ch, STAT_DEX) * 6)
		{
			act("Staggering blindly, you manage to rip the sack off your head!", ch, 0, 0, TO_CHAR);
			act("Staggering blindly, $n manages to rip the sack off $s head!", ch, 0, 0, TO_ROOM);
			affect_remove(ch, af);
		}
		else
		{
			act("You struggle with the sack on your head, but fail to remove it.", ch, 0, 0, TO_CHAR);
			act("$n struggles with the sack on $s head, but fails to remove it.", ch, 0, 0, TO_ROOM);
		}

		WAIT_STATE(ch, PULSE_VIOLENCE);
	}
	else if ((af = check_bind(ch, "legs")) != nullptr)
	{
		if (number_percent() < (get_curr_stat(ch, STAT_DEX) * 3 + get_curr_stat(ch, STAT_STR) * 3))
		{
			act("You pull the bindings on your legs, ripping them apart!", ch, 0, 0, TO_CHAR);
			act("$n pulls the bindings on $s legs, ripping them apart!", ch, 0, 0, TO_ROOM);
			affect_remove(ch, af);
		}
		else
		{
			act("You struggle with the bindings on your legs, but fail to break them.", ch, 0, 0, TO_CHAR);
			act("$n struggles with the bindings on $s legs, but fails to break them.", ch, 0, 0, TO_ROOM);
		}

		WAIT_STATE(ch, PULSE_VIOLENCE);
	}
	else
	{
		send_to_char("You are not bound.\n\r", ch);
	}
}

void do_knife(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	char buf[MAX_STRING_LENGTH];
	AFFECT_DATA af;

	one_argument(argument, arg);

	if ((get_skill(ch, gsn_knife) == 0) || ch->level < skill_table[gsn_knife].skill_level[ch->Class()->GetIndex()])
	{
		if (!is_npc(ch))
		{
			send_to_char("You don't know how to knife.\n\r", ch);
			return;
		}
	}

	if (arg[0] == '\0')
	{
		send_to_char("Knife whom?\n\r", ch);
		return;
	}

	if (ch->fighting != nullptr)
	{
		send_to_char("No way! You're still fighting!\n\r", ch);
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
		send_to_char("Bah, you can't knife yourself.\n\r", ch);
		return;
	}

	if (!is_wielded(ch, WEAPON_DAGGER, WIELD_ONE))
	{
		send_to_char("You need to be wielding a dagger to knife.\n\r", ch);
		return;
	}

	if (is_safe(ch, victim))
		return;

	WAIT_STATE(ch, PULSE_VIOLENCE);

	if (!is_npc(ch) && !is_npc(victim))
	{
		switch (number_range(0, 1))
		{
			case 0:
				sprintf(buf, "Die, %s you knifing fool!", pers(ch, victim));
				break;
			case 1:
				sprintf(buf, "Help! %s knifed me!", pers(ch, victim));
				break;
		}

		do_myell(victim, buf, ch);
	}

	if (number_percent() < get_skill(ch, gsn_knife))
	{
		act("You slam your dagger into $N's side!", ch, 0, victim, TO_CHAR);
		act("$n slams $s dagger into your side!", ch, 0, victim, TO_VICT);
		act("$n slams $s dagger into $N's side!", ch, 0, victim, TO_NOTVICT);

		bool killed = one_hit_new(ch, victim, gsn_knife, HIT_SPECIALS, HIT_UNBLOCKABLE, HIT_NOADD, 125, nullptr);

		check_improve(ch, gsn_knife, true, 1);

		if (killed)
			return;

		if (number_range(0, 5) == 3)
		{
			act("Bright red blood spurts out of $S wound!\n\r", ch, 0, victim, TO_CHAR);

			init_affect(&af);
			af.where = TO_AFFECTS;
			af.duration = ch->level / 8;
			af.type = gsn_bleeding;
			af.level = ch->level;
			af.modifier = 0;
			af.aftype = AFT_INVIS;
			af.tick_fun = bleeding_tick;
			af.owner = ch;
			new_affect_to_char(victim, &af);
		}
	}
	else
	{
		damage(ch, victim, 0, gsn_knife, DAM_NONE, true);
		check_improve(ch, gsn_knife, false, 2);
	}
}

void check_detect_movement(CHAR_DATA *ch, char *in_out)
{
	for (CHAR_DATA *vch = ch->in_room->people; vch; vch = vch->next_in_room)
	{
		if (is_affected(vch, gsn_detect_movement))
		{
			if (number_percent() < (get_skill(vch, gsn_detect_movement) * .98))
			{
				act("You hear someone moving around, quickly scrutinizing you notice $N sneaking $t.", vch, in_out, ch, TO_CHAR);
				check_improve(vch, gsn_detect_movement, true, 2);
			}
			else
			{
				check_improve(vch, gsn_detect_movement, false, 2);
			}
		}
	}
}

void do_bluff(CHAR_DATA *ch, char *argument)
{
	char arg[MSL];
	int skill = 0;
	AFFECT_DATA af;

	one_argument(argument, arg);

	skill = get_skill(ch, gsn_bluff);

	if (skill == 0)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	if (arg[0] == '\0')
	{
		send_to_char("Syntax:\tbluff <good/bad>\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_bluff))
	{
		send_to_char("You are already bluffing your wounds.\n\r", ch);
		return;
	}

	if (ch->hit == ch->max_hit)
	{
		send_to_char("How do you expect to hide or exploit your wounds when you're not even hurt?\n\r", ch);
		return;
	}

	if (number_percent() < ((int)((float)skill * .98)))
	{
		init_affect(&af);
		af.where = TO_AFFECTS;
		af.duration = ch->level / 8;
		af.type = gsn_bluff;
		af.level = ch->level;

		if (!str_prefix(arg, "good"))
		{
			send_to_char("You cover your wounds and straighten your posture, putting on a convincing show of being healthier.\n\r", ch);
			af.modifier = 1;
		}
		else if (!str_prefix(arg, "bad"))
		{
			send_to_char("Slouching over and limping, you greatly exaggerate the true severity of your wounds.\n\r", ch);
			af.modifier = -1;
		}
		else
		{
			send_to_char("Syntax:\tbluff <good/bad>\n\r", ch);
			return;
		}

		af.aftype = AFT_SKILL;
		af.owner = ch;
		af.mod_name = MOD_APPEARANCE;
		new_affect_to_char(ch, &af);

		check_improve(ch, gsn_bluff, true, 2);
	}
	else
	{
		check_improve(ch, gsn_bluff, false, 2);
	}

	WAIT_STATE(ch, skill_table[gsn_bluff].beats);
}

void do_false_motives(CHAR_DATA *ch, char *argument)
{
	char arg1[MSL], arg2[MSL];
	int skill = 0;
	AFFECT_DATA af;

	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);

	skill = get_skill(ch, gsn_false_motives);

	if (skill < 2 || is_npc(ch))
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	if (arg1[0] == '\0' || arg2[0] == '\0')
	{
		send_to_char("Syntax:\tfalsemotives <lawful/neutral/chaotic> <good/neutral/evil>\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_false_motives))
	{
		send_to_char("You should wait before trying to change your behavior again.\n\r", ch);
		return;
	}

	if (number_percent() < ((float)skill * .98))
	{
		int alignval;

		init_affect(&af);
		af.where = TO_AFFECTS;
		af.mod_name = MOD_APPEARANCE;
		af.location = APPLY_ETHOS;
		af.duration = ch->level / 5;
		af.type = gsn_false_motives;
		af.level = ch->level;
		af.aftype = AFT_SKILL;

		if (!str_prefix(arg1, "lawful"))
		{
			af.modifier = ch->pcdata->ethos == ETHOS_NEUTRAL ? 1000 : ch->pcdata->ethos < ETHOS_NEUTRAL ? 2000 : 0;
		}
		else if (!str_prefix(arg1, "neutral"))
		{
			af.modifier = ch->pcdata->ethos == ETHOS_NEUTRAL ? 0 : ch->pcdata->ethos < ETHOS_NEUTRAL ? 1000 : -1000;
		}
		else if (!str_prefix(arg1, "chaotic"))
		{
			af.modifier = ch->pcdata->ethos == ETHOS_NEUTRAL ? -1000 : ch->pcdata->ethos < ETHOS_NEUTRAL ? 0 : -2000;
		}
		else
		{
			send_to_char("Syntax:\tfalsemotives <lawful/neutral/chaotic> <good/neutral/evil>\n\r", ch);
			return;
		}

		if (!str_prefix(arg2, "good"))
		{
			alignval = ch->alignment == ALIGN_NEUTRAL ? 1000 : ch->alignment < ALIGN_NEUTRAL ? 2000 : 0;
		}
		else if (!str_prefix(arg2, "neutral"))
		{
			alignval = ch->alignment == ALIGN_NEUTRAL ? 0 : ch->alignment < ALIGN_NEUTRAL ? 1000 : -1000;
		}
		else if (!str_prefix(arg2, "evil"))
		{
			alignval = ch->alignment == ALIGN_NEUTRAL ? -1000 : ch->alignment < ALIGN_NEUTRAL ? 0 : -2000;
		}
		else
		{
			send_to_char("Syntax:\tfalsemotives <lawful/neutral/chaotic> <good/neutral/evil>\n\r", ch);
			return;
		}

		new_affect_to_char(ch, &af);

		af.modifier = alignval;
		af.location = APPLY_ALIGNMENT;
		new_affect_to_char(ch, &af);

		send_to_char("You alter your behavioral patterns in an attempt to convince onlookers.\n\r", ch);
		check_improve(ch, gsn_false_motives, true, 2);
	}
	else
	{
		send_to_char("Your attempt at altering your behavior fooled no one.\n\r", ch);
		check_improve(ch, gsn_false_motives, false, 2);
	}

	WAIT_STATE(ch, skill_table[gsn_false_motives].beats);
}
