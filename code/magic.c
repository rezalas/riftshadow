#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <algorithm>
#include "magic.h"
#include "characterClasses/necro.h"
#include "recycle.h"
#include "tables.h"
#include "act_obj.h"
#include "act_ente.h"
#include "act_comm.h"
#include "characterClasses/warrior.h"
#include "dioextra.h"
#include "characterClasses/sorcerer.h"
#include "act_move.h"
#include "fight.h"
#include "act_info.h"
#include "newmem.h"
#include "handler.h"
#include "effects.h"
#include "comm.h"
#include "interp.h"
#include "skill.h"
#include "devextra.h"
#include "db.h"
#include "chardef.h"
#include "const.h"
#include "material.h"
#include "utility.h"
#include "skills.h"
#include "lookup.h"
#include "./include/fmt/format.h"

char *target_name;

///
/// Returns the number for a skill, searched by name.
/// @param name: The name of the skill.
/// @return The number associated with the skill, or -1 if not found, null, or empty.
///
int skill_lookup(const char *name)
{
	if(name == nullptr)
		return -1;

	int sn;

	for (sn = 0; sn < MAX_SKILL; sn++)
	{
		if (skill_table[sn].name == nullptr)
			break;

		if (LOWER(name[0]) == LOWER(skill_table[sn].name[0]) && !str_prefix(name, skill_table[sn].name))
			return sn;
	}

	return -1;
}

int find_spell(CHAR_DATA *ch, const char *name)
{
	/* finds a spell the character can cast if possible */
	int sn, found = -1;

	if (is_npc(ch))
		return skill_lookup(name);

	for (sn = 0; sn < MAX_SKILL; sn++)
	{
		if (skill_table[sn].name == nullptr)
			break;

		if (LOWER(name[0]) == LOWER(skill_table[sn].name[0]) && !str_prefix(name, skill_table[sn].name))
		{
			if (found == -1)
				found = sn;

			if (ch->level >= skill_table[sn].skill_level[ch->Class()->GetIndex()] && ch->pcdata->learned[sn] > 0)
				return sn;
		}
	}

	return found;
}

/*
 * Utter mystical words for an sn.
 */
void say_spell(CHAR_DATA *ch, int sn)
{
	char buf[MAX_STRING_LENGTH];
	CHAR_DATA *rch;
	char *pName;
	int iSyl;
	int length;

	struct syl_type
	{
		char *old;
		char *newword;
	};

	static const struct syl_type syl_table[] = {
		{" ", " "},
		{"ar", "abra"},
		{"au", "kada"},
		{"bless", "fido"},
		{"blind", "nose"},
		{"bur", "mosa"},
		{"cu", "judi"},
		{"de", "oculo"},
		{"en", "unso"},
		{"light", "dies"},
		{"lo", "hi"},
		{"mor", "zak"},
		{"move", "sido"},
		{"ness", "lacri"},
		{"ning", "illa"},
		{"per", "duda"},
		{"ra", "gru"},
		{"fresh", "ima"},
		{"re", "candus"},
		{"son", "sabru"},
		{"tect", "infra"},
		{"tri", "cula"},
		{"ven", "nofo"},
		{"a", "a"},
		{"b", "b"},
		{"c", "q"},
		{"d", "e"},
		{"e", "z"},
		{"f", "y"},
		{"g", "o"},
		{"h", "p"},
		{"i", "u"},
		{"j", "y"},
		{"k", "t"},
		{"l", "r"},
		{"m", "w"},
		{"n", "i"},
		{"o", "a"},
		{"p", "s"},
		{"q", "d"},
		{"r", "f"},
		{"s", "g"},
		{"t", "h"},
		{"u", "j"},
		{"v", "z"},
		{"w", "x"},
		{"x", "n"},
		{"y", "l"},
		{"z", "k"},
		{"", ""}};

	if ((sn == skill_lookup("power word kill"))
		|| (sn == skill_lookup("power word stun"))
		|| (sn == skill_lookup("word of command")))
	{
		return;
	}

	buf[0] = '\0';

	for (pName = skill_table[sn].name; *pName != '\0'; pName += length)
	{
		for (iSyl = 0; (length = strlen(syl_table[iSyl].old)) != 0; iSyl++)
		{
			if (!str_prefix(syl_table[iSyl].old, pName))
			{
				strcat(buf, syl_table[iSyl].newword);
				break;
			}
		}

		if (length == 0)
			length = 1;
	}

	auto buffer2 = fmt::format("$n utters the words, '{}'.", buf);
	auto buffer = fmt::format("$n utters the words, '{}'.", skill_table[sn].name);

	for (rch = ch->in_room->people; rch; rch = rch->next_in_room)
	{
		if (rch != ch)
		{
			if (!is_npc(rch)
				&& is_awake(rch)
				&& rch->Profs()->HasProf("ancient languages")
				&& number_percent() > (rch->Profs()->GetProf("ancient languages") + 2) * 10)
			{
				send_to_char("Your knowledge of ancient languages allows you to translate the incantation.\n\r", rch);
				act(buffer.c_str(), ch, nullptr, rch, TO_VICT);
			}
			else if (!is_npc(rch) && ch->Class()->GetIndex() == rch->Class()->GetIndex())
			{
				act(buffer.c_str(), ch, nullptr, rch, TO_VICT);
			}
			else
			{
				act(buffer2.c_str(), ch, nullptr, rch, TO_VICT);
			}
		}
	}
}

/*
 * Compute a saving throw.
 * Negative apply's make saving throw better.
 */
bool saves_spell(int level, CHAR_DATA *victim, int dam_type)
{
	int roll;
	float save;
	AFFECT_DATA *af;
	//    char buf[MSL];
	if (victim->fighting && get_trust(victim->fighting) == MAX_LEVEL)
		return false;

	if (victim->saving_throw < 0)
		save = 40 + (victim->level - level) * 4 + (sqrt(abs(victim->saving_throw))) * 4;
	else
		save = 45 + (victim->level - level) * 4 - (sqrt(victim->saving_throw)) * 4;

	if (is_npc(victim))
		save += victim->level / 3; /* simulate npc saving throw */

	switch (check_immune(victim, dam_type))
	{
		case IS_IMMUNE:
			return true;
		case IS_RESISTANT:
			if (!is_npc(victim) && !str_cmp(pc_race_table[victim->race].name, "imp"))
				save *= 1.1;
			else
				save *= 1.3;

			break;
		case IS_VULNERABLE:
			save *= .7;
			break;
	}

	if (is_affected(victim, skill_lookup("strength of faith")) && (number_percent() < 50 || dam_type == DAM_HOLY))
		return true;

	/*
	if (!is_npc(victim) && victim->Class()->fMana)
		save = 9 * save / 10;
	*/

	if (is_affected(victim, gsn_traitors_luck))
	{
		af = affect_find(victim->affected, gsn_traitors_luck);
		if (victim->fighting && (victim->fighting == af->owner))
			save -= 50;
	}

	if (is_affected_room(victim->in_room, gsn_infidels_fate) && is_good(victim))
		save -= 20;

	save = URANGE(5, save, 95);
	roll = number_percent();

	/*
	if(victim->fighting && !is_npc(victim->fighting) && !is_npc(victim))
	{
		sprintf(buf,"Saves_spell: Caster is probably %s, victim is %s, dt is %d.  Victim level %d, spell level %d (%d - %.2f%%mod)."\
			" Victim has %dsvs (%.2f%%mod).  Chance of saving is %d, roll is %d: %s",
			victim->fighting->name,
			victim->true_name,
			dam_type,
			victim->level,
			level,
			victim->level - level,
			(float)((victim->level - level) * 4),
			victim->saving_throw,
			(float)sqrt(abs(victim->saving_throw)) * 4,
			save,
			roll,
			roll < save ? "SAVED" : "FAILED");
		wiznet(buf,0,nullptr,WIZ_DEBUG,0,0);
	}
	else if(!is_npc(victim))
	{
		sprintf(buf,"Saves_spell: victim is %s.  Victim level %d, spell level %d (%d - %.2f%%mod).",
			victim->name,
			victim->level,
			level,
			victim->level - level,
			(float)((victim->level - level) * 4));
		wiznet(buf,0,nullptr,WIZ_DEBUG,0,0);
	}
	*/

	return (roll < (int)save);
}

/* RT save for dispels */

bool saves_dispel(int dis_level, int spell_level, int duration)
{
	int save;

	save = 50 + (spell_level - dis_level) * 5;

	if (duration == -1)
		return true;

	save = URANGE(5, save, 98);
	return number_percent() < save;
}

bool check_dispel(int dis_level, CHAR_DATA *victim, int sn)
{
	AFFECT_DATA *af;

	if (is_affected(victim, gsn_indom))
	{
		send_to_char("Your body's state is maintained by the force of your will.\n\r", victim);
		return false;
	}

	if (is_affected(victim, sn))
	{
		for (af = victim->affected; af != nullptr; af = af->next)
		{
			if (af->type == sn)
			{
				if (!saves_dispel(dis_level, af->level, af->duration) && !IS_SET(af->bitvector, AFF_PERMANENT))
				{
					affect_strip(victim, sn);

					if (str_cmp(skill_table[sn].msg_off, ""))
					{
						send_to_char(skill_table[sn].msg_off, victim);
						send_to_char("\n\r", victim);
					}

					if (str_cmp(skill_table[sn].room_msg_off, ""))
						act(skill_table[sn].room_msg_off, victim, 0, 0, TO_ROOM);

					return true;
				}
			}
		}
	}

	return false;
}

/* co-routine for dispel magic and cancellation */

bool check_dispel_cancellation(int dis_level, CHAR_DATA *victim, CHAR_DATA *ch, int sn, bool commune, bool cancel)
{
	AFFECT_DATA *af;
	char buf[MAX_STRING_LENGTH];

	if (is_affected(victim, gsn_indom))
	{
		send_to_char("Your body's state is maintained by the force of your will.\n\r", victim);
		return false;
	}

	if (is_affected(victim, sn))
	{
		for (af = victim->affected; af != nullptr; af = af->next)
		{
			if (af->type == sn)
			{
				if (af->aftype == AFT_SPELL)
				{
					if (!saves_dispel(dis_level, af->level, af->duration) && !IS_SET(af->bitvector, AFF_PERMANENT))
					{
						sprintf(buf, "The magic of your %s spell unravels.\n\r", skill_table[sn].name);
						affect_strip(victim, sn);
						send_to_char(buf, victim);

						if (ch != victim)
						{
							sprintf(buf, "The magic of $N's %s spell unravels.", skill_table[sn].name);
							act(buf, ch, 0, victim, TO_CHAR);
						}

						if (sn == gsn_diamondskin)
						{
							act("Unspeakable agony wracks your entire frame as your very skin shatters!", victim, 0, 0, TO_CHAR);
							act("$n howls in agony as the diamond shell that was once $s skin shatters!", victim, 0, 0, TO_ROOM);
							damage_new(victim, victim, victim->max_hit / 2, gsn_diamondskin, DAM_TRUESTRIKE, true, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, nullptr);
						}

						return true;
					}
					else
					{
						sprintf(buf, "The magic of your %s spell is unaffected.\n\r", skill_table[sn].name);
						send_to_char(buf, victim);

						if (ch != victim)
						{
							sprintf(buf, "The magic of $N's %s spell is unaffected.", skill_table[sn].name);
							act(buf, ch, 0, victim, TO_CHAR);
						}
					}
				}
				else if (af->aftype == AFT_COMMUNE && commune && cancel)
				{
					if (!saves_dispel(dis_level, af->level, af->duration) && !IS_SET(af->bitvector, AFF_PERMANENT))
					{
						sprintf(buf, "The power of your %s supplication unravels.\n\r", skill_table[sn].name);
						affect_strip(victim, sn);
						send_to_char(buf, victim);

						if (ch != victim)
						{
							sprintf(buf, "The power of $N's %s supplication unravels.", skill_table[sn].name);
							act(buf, ch, 0, victim, TO_CHAR);
						}

						return true;
					}
					else
					{
						sprintf(buf, "The power of your %s supplication is unaffected.\n\r", skill_table[sn].name);
						send_to_char(buf, victim);

						if (ch != victim)
						{
							sprintf(buf, "The power of $N's %s supplication is unaffected.", skill_table[sn].name);
							act(buf, ch, 0, victim, TO_CHAR);
						}
					}
				}
			}
		}
	}

	return false;
}

/* for finding mana costs -- temporary version */
int mana_cost(CHAR_DATA *ch, int min_mana, int level)
{
	if (ch->level + 2 == level)
		return 1000;

	return std::max(min_mana, (100 / (2 + ch->level - level)));
}

void do_barkskin(CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;

	if ((get_skill(ch, gsn_barkskin) == 0) ||
		(ch->level < skill_table[gsn_barkskin].skill_level[ch->Class()->GetIndex()]))
	{
		send_to_char("You do not know how to turn your skin to bark.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_barkskin))
	{
		send_to_char("Your skin is already covered in bark.\n\r", ch);
		return;
	}

	if (ch->mana < 40)
	{
		send_to_char("You do not have the mana.\n\r", ch);
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SKILL;
	af.type = gsn_barkskin;
	af.level = ch->level;
	af.location = APPLY_AC;
	af.modifier = ch->level / 4;
	af.duration = ch->level;
	affect_to_char(ch, &af);

	ch->mana -= 40;
	act("$n's skin slowly becomes covered in bark.", ch, nullptr, nullptr, TO_ROOM);
	send_to_char("Your skin slowly becomes covered in hardened bark.\n\r", ch);
}

void do_laying_hands(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim; /* Well not really victim, but keeping it standard varibale anmes */
	AFFECT_DATA af;
	int heal;
	int chance;

	if ((get_skill(ch, gsn_laying_hands) == 0) ||
		(ch->level < skill_table[gsn_laying_hands].skill_level[ch->Class()->GetIndex()]))
	{
		send_to_char("You do not have the ability to divinely heal.\n\r", ch);
		return;
	}

	one_argument(argument, arg);

	if (arg[0] == '\0')
	{
		send_to_char("Give your divine touch to whom?\n\r", ch);
		return;
	}

	if ((victim = get_char_room(ch, arg)) == nullptr)
	{
		send_to_char("They aren't here.\n\r", ch);
		return;
	}

	if (ch == victim)
	{
		send_to_char("You can't lay hands on yourself.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_laying_hands))
	{
		send_to_char("You may only grant divine healing once per day.\n\r", ch);
		return;
	}

	if (ch->mana < 35)
	{
		send_to_char("You don't have the mana.\n\r", ch);
		return;
	}

	ch->mana -= skill_table[gsn_laying_hands].min_mana;

	WAIT_STATE(ch, skill_table[gsn_laying_hands].beats);

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_TIMER;
	af.type = gsn_laying_hands;
	af.level = ch->level;
	af.duration = 48;
	af.modifier = 0;
	af.location = 0;
	affect_to_char(ch, &af);

	heal = dice(ch->level, 12);

	if (ch == victim)
	{
		send_to_char("You cannot lay hands on yourself.\n\r", ch);
		return;
	}

	if (!is_evil(victim))
	{
		victim->hit = std::min(victim->hit + heal, (int)victim->max_hit);
		act("$n lays $s hands upon $N and $N's wounds seem to heal.", ch, nullptr, victim, TO_NOTVICT);
		act("You lay your hands upon $N and $S wounds seem to heal.", ch, nullptr, victim, TO_CHAR);
		act("$n lays $s hands upon you in blessed communication and your wounds heal.", ch, nullptr, victim, TO_VICT);

		if (check_dispel(ch->level - (dice(1, 10)), victim, gsn_poison))
		{
			send_to_char("A warm feeling runs through your body.\n\r", victim);
			act("$n looks much better.", victim, nullptr, nullptr, TO_ROOM);
		}

		if (check_dispel(ch->level - (dice(3, 4)), victim, gsn_plague))
		{
			send_to_char("Your plague sores vanish.\n\r", victim);
			act("$n looks relieved as $s plague sores vanish.", victim, nullptr, nullptr, TO_ROOM);
		}

		if (check_dispel(ch->level - (dice(3, 5)), victim, gsn_blindness))
		{
			send_to_char("You can see again.\n\r", victim);
		}
	}
	else
	{
		act("$n lays $s hands upon $N, and $n's flesh begins to crackle!", ch, nullptr, victim, TO_NOTVICT);
		act("You lay your hands upon $N... and your flesh begins to crackle!", ch, nullptr, victim, TO_CHAR);
		act("$n lays $s hands upon you... and $s flesh begins to crackle!", ch, nullptr, victim, TO_VICT);

		damage_new(ch, ch, heal, gsn_laying_hands, DAM_HOLY, true, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, nullptr);
	}
}

void cast_myell(CHAR_DATA *ch, CHAR_DATA *victim)
{
	char buf[MSL];

	if (victim == ch)
		return;

	if (!is_npc(ch) && !is_npc(victim) && (ch->fighting == nullptr || victim->fighting == nullptr))
	{
		if (can_see(ch, victim) && victim->position > POS_SLEEPING)
		{
			if (number_percent() > 50)
				sprintf(buf, "Die, %s you sorcerous dog!", pers(ch, victim));
			else
				sprintf(buf, "Help! %s is casting a spell on me!", pers(ch, victim));
		}
		else
		{
			sprintf(buf, "Help! Someone is attacking me!");
		}

		do_myell(victim, buf, ch);
	}
}

/*
 * The kludgy global is for spells who want more stuff from command line.
 */
void do_cast(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];
	char arg1[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	AFFECT_DATA *paf;
	OBJ_DATA *obj;
	EXIT_DATA *pexit;
	void *vo;
	bool somatic = false;
	int mana, where;
	int sn;
	int encumb;
	int targtype;
	int target;

	if (is_npc(ch) && is_affected_by(ch, AFF_CHARM))
		return;

	if (ch->Class()->ctype != CLASS_CASTER && !is_immortal(ch))
	{
		send_to_char("You must ask your deity for prayers instead of pulling it from the world.\n\r", ch);
		return;
	}

	if (is_shifted(ch) && !is_immortal(ch))
	{
		send_to_char("You cannot form the words necessary for your magic.\n\r", ch);
		return;
	}

	if (IS_SET(ch->act, PLR_BETRAYER))
	{
		send_to_char("You feel cut off from your source of power.\n\r", ch);
		return;
	}

	if (!is_npc(ch) && ch->pcdata->oalign == 3 && ch->alignment == -1000)
	{
		send_to_char("You feel cut off from your source of power.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_uppercut))
	{
		if (check_somatic(ch))
		{
			somatic = true;
		}
		else
		{
			send_to_char("Your jaw hurts too much to move.\n\r", ch);
			return;
		}
	}

	if (IS_SET(ch->in_room->room_flags, ROOM_SILENCE))
	{
		if (check_somatic(ch))
		{
			somatic = true;
		}
		else
		{
			send_to_char("You cannot say the words necessary for your magic.\n\r", ch);
			return;
		}
	}

	if (is_affected(ch, gsn_mangled))
	{
		if (check_somatic(ch))
		{
			somatic = true;
		}
		else
		{
			send_to_char("Your torn throat prevents you from getting the words out.\n\r", ch);
			return;
		}
	}

	if (is_affected(ch, gsn_imprisonvoice))
	{
		if (check_somatic(ch))
		{
			somatic = true;
		}
		else
		{
			send_to_char("Your vocal cords are frozen solid!\n\r", ch);
			return;
		}
	}

	if (is_affected(ch, gsn_gag))
	{
		if (check_somatic(ch))
		{
			somatic = true;
		}
		else
		{
			send_to_char("You are gagged and cannot speak!\n\r", ch);
			return;
		}
	}

	if (is_affected(ch, gsn_deafen))
	{
		if (check_somatic(ch))
		{
			somatic = true;
		}
		else
		{
			send_to_char("You can't get the right intonations.\n\r", ch);
			return;
		}
	}

	if (!is_npc(ch) && ch->pcdata->energy_state > 2)
	{
		send_to_char("You cannot concentrate enough to complete the incantation.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_molecular_disjunction) && number_percent() < 30)
	{
		send_to_char("You lose your focus on the spell.\n\r", ch);
		WAIT_STATE(ch, PULSE_VIOLENCE);
		return;
	}

	target_name = one_argument(argument, arg1);
	one_argument(target_name, arg2);

	if (arg1[0] == '\0')
{		return send_to_char("Cast which what where?\n\r", ch);}

	if ((sn = find_spell(ch, arg1)) < 1
		|| skill_table[sn].spell_fun == spell_null
		|| (!is_npc(ch) && get_skill(ch, sn) < 5)
		|| (!is_npc(ch) && ch->pcdata->learned[sn] == 0))
	{
		send_to_char("You don't know any spells of that name.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_scramble_neurons))
		sn = scramble_sn(ch, sn);

	if (ch->position < skill_table[sn].minimum_position)
	{
		send_to_char("You can't concentrate enough.\n\r", ch);
		return;
	}

	if ((skill_table[sn].ctype == CMD_COMMUNE || skill_table[sn].ctype == CMD_POWER) && !is_immortal(ch))
	{
		send_to_char("You can't cast that.\n\r", ch);
		return;
	}

	if (ch->level + 2 == skill_table[sn].skill_level[ch->Class()->GetIndex()])
	{
		mana = 50;
	}
	else
	{
		if (is_affected(ch, gsn_concuss))
		{
			mana = std::max((skill_table[sn].min_mana * 4), 100 / (2 + ch->level - skill_table[sn].skill_level[ch->Class()->GetIndex()]));
		}
		else
		{
			mana = std::max((int)skill_table[sn].min_mana, 100 / (2 + ch->level - skill_table[sn].skill_level[ch->Class()->GetIndex()]));
		}
	}

	if (somatic)
		mana *= 2;

	/*
	 * Locate targets.
	 */
	victim = nullptr;
	obj = nullptr;
	vo = nullptr;
	target = TARGET_NONE;
	targtype = skill_table[sn].target;
	/*
		if(targtype < RUNE_CAST) {
	*/
	/* For spells that are offensive if not trusting. */
	if (targtype == TAR_CHAR_AMBIGUOUS)
	{
		if ((victim = get_char_room(ch, target_name)) == nullptr)
		{
			send_to_char("They aren't here.\n\r", ch);
			return;
		}

		targtype = (trusts(ch, victim)) ? TAR_CHAR_DEFENSIVE : TAR_CHAR_OFFENSIVE;
	}

	switch (targtype)
	{
		case TAR_IGNORE:
			break;
		case TAR_CHAR_GENERAL:
		case TAR_CHAR_OFFENSIVE:
			if (arg2[0] == '\0')
			{
				if ((victim = ch->fighting) == nullptr)
				{
					send_to_char("Cast the spell on whom?\n\r", ch);
					return;
				}
			}
			else
			{
				if ((victim = get_char_room(ch, target_name)) == nullptr)
				{
					send_to_char("They aren't here.\n\r", ch);
					return;
				}
			}

			if (is_affected_by(ch, AFF_CHARM) && ch->master == victim)
			{
				send_to_char("You can't do that on your own master.\n\r", ch);
				return;
			}

			vo = (void *)victim;
			target = TARGET_CHAR;
			break;
		case TAR_CHAR_DEFENSIVE:
			if (arg2[0] == '\0')
			{
				victim = ch;
			}
			else
			{
				if ((victim = get_char_room(ch, target_name)) == nullptr)
				{
					send_to_char("They aren't here.\n\r", ch);
					return;
				}
			}

			if (!victim)
			{
				send_to_char("They aren't here.\n\r", ch);
				return;
			}

			vo = (void *)victim;
			target = TARGET_CHAR;
			break;
		case TAR_CHAR_SELF:
			if (arg2[0] != '\0' && !is_name(target_name, ch->name))
			{
				send_to_char("You cannot cast this spell on another.\n\r", ch);
				return;
			}

			vo = (void *)ch;
			target = TARGET_CHAR;
			break;
		case TAR_OBJ_INV:
			if (arg2[0] == '\0')
			{
				send_to_char("What should the spell be cast upon?\n\r", ch);
				return;
			}

			if ((obj = get_obj_carry(ch, target_name, ch)) == nullptr)
			{
				send_to_char("You are not carrying that.\n\r", ch);
				return;
			}

			vo = (void *)obj;
			target = TARGET_OBJ;
			break;
		case TAR_OBJ_CHAR_OFF:
			if (arg2[0] == '\0')
			{
				if ((victim = ch->fighting) == nullptr)
				{
					send_to_char("Cast the spell on whom or what?\n\r", ch);
					return;
				}

				target = TARGET_CHAR;
			}
			else if ((victim = get_char_room(ch, target_name)) != nullptr)
			{
				target = TARGET_CHAR;
			}

			if (target == TARGET_CHAR) /* check the sanity of the attack */
			{
				if (is_affected_by(ch, AFF_CHARM) && ch->master == victim)
				{
					send_to_char("You can't do that on your own follower.\n\r", ch);
					return;
				}

				vo = (void *)victim;
			}
			else if ((obj = get_obj_here(ch, target_name)) != nullptr)
			{
				vo = (void *)obj;
				target = TARGET_OBJ;
			}
			else
			{
				send_to_char("You don't see that here.\n\r", ch);
				return;
			}

			break;
		case TAR_OBJ_CHAR_DEF:
			if (arg2[0] == '\0')
			{
				vo = (void *)ch;
				target = TARGET_CHAR;
			}
			else if ((victim = get_char_room(ch, target_name)) != nullptr)
			{
				vo = (void *)victim;
				target = TARGET_CHAR;
			}
			else if ((obj = get_obj_carry(ch, target_name, ch)) != nullptr)
			{
				vo = (void *)obj;
				target = TARGET_OBJ;
			}
			else
			{
				send_to_char("You don't see that here.\n\r", ch);
				return;
			}

			break;
		case TAR_DIR:
			if ((where = direction_lookup(target_name)) < 0 || !(pexit = ch->in_room->exit[where]))
			{
				send_to_char("That's not a valid direction.\n\r", ch);
				return;
			}

			vo = &where;
			target = TARGET_DIR;
			break;
		default:
			bug("Do_cast: bad target for sn %d.", sn);
			return;
	}
	/*
	}
	else
	{
		if(!IS_SET_OLD(targtype, RUNE_CAST))
			return send_to_char("You can't cast that.\n\r",ch);
		target = RUNE_CAST;
	}
	*/

	if (!is_npc(ch) && ch->mana < mana)
	{
		send_to_char("You don't have enough mana.\n\r", ch);
		return;
	}

	if (str_cmp(skill_table[sn].name, "ventriloquate") && !somatic && (!is_npc(ch) || !IS_SET(ch->form, FORM_NOSPEECH)))
		say_spell(ch, sn);

	WAIT_STATE(ch, skill_table[sn].beats);

	if (!is_npc(ch) && (number_percent() > get_skill(ch, sn)))
	{
		if (targtype == TAR_CHAR_OFFENSIVE)
			cast_myell(ch, victim);

		send_to_char("You failed to complete your incantation.\n\r", ch);
		check_improve(ch, sn, false, 1);
		ch->mana -= mana / 2;
	}
	else
	{
		ch->mana -= mana;

		if (IS_SET(ch->in_room->room_flags, ROOM_NO_MAGIC) && !(ch->level > LEVEL_HERO))
		{
			act("$n's spell fizzles.", ch, 0, 0, TO_ROOM);
			send_to_char("Your spell fizzles and dies.\n\r", ch);
			return;
		}

		if (targtype == TAR_CHAR_OFFENSIVE && is_safe(ch, victim))
			return;

		if (targtype == TAR_CHAR_OFFENSIVE)
			cast_myell(ch, victim);

		/* Armor encumbrance for mages. */

		if (((ch->Class()->GetIndex() == CLASS_SORCERER)
			|| (ch->Class()->GetIndex() == CLASS_NECROMANCER)
			|| (ch->Class()->GetIndex() == CLASS_SHAPESHIFTER))
			&& (encumb = armor_weight(ch)) >= 90)
		{
			if (number_percent() < ((encumb - 90) * .2))
			{
				WAIT_STATE(ch, PULSE_VIOLENCE);
				send_to_char("Encumbered by your heavy armor, you fumble the incantation.\n\r", ch);
				return;
			}
		}

		if (targtype == TAR_CHAR_OFFENSIVE && is_affected(victim, gsn_cloak_of_mist) && number_percent() > 60)
		{
			act("Your spell dissipates in the mist swirling around $N.", ch, 0, victim, TO_CHAR);
			act("$n's spell dissipates in the mist swirling around you.", ch, 0, victim, TO_VICT);
			act("$n's spell dissipates in the mist swirling around $N.", ch, 0, victim, TO_NOTVICT);
			return;
		}

		if (targtype == TAR_CHAR_OFFENSIVE && is_affected(victim, gsn_rotating_ward))
		{
			paf = affect_find(victim->affected, gsn_rotating_ward);
			paf->modifier--;

			act("A crystal rotating around $N's body absorbs your spell!", ch, 0, victim, TO_CHAR);
			act("A crystal rotating around your body absorbs $n's spell!", ch, 0, victim, TO_VICT);
			act("$n's spell is absorbed by a crystal rotating around $N's body!", ch, 0, victim, TO_NOTVICT);

			if (paf->modifier <= 0)
			{
				affect_remove(victim, paf);
				act("The crystal crumbles under the stress!", ch, 0, 0, TO_ROOM);
			}

			return;
		}

		if (targtype == TAR_CHAR_OFFENSIVE
			&& get_skill(victim, gsn_nullify) > 1
			&& !(is_npc(victim) && victim->cabal != CABAL_SCION)
			&& (!is_immortal(ch) || !is_immortal(victim))
			&& (!victim->fighting || (saves_spell(ch->level, victim, DAM_OTHER) && number_percent() < 50)))
		{
			if (!cabal_down(victim, CABAL_SCION))
			{
				if (number_percent() < (get_skill(victim, gsn_nullify) - 10))
				{
					act("Your magic fizzles out before it reaches $N!", ch, 0, victim, TO_CHAR);
					act("$n's magic fizzles out before it reaches you!", ch, 0, victim, TO_VICT);
					act("$n's magic fizzles out before it reaches $N!", ch, 0, victim, TO_NOTVICT);
					check_improve(victim, gsn_nullify, true, 5);
					return;
				}
				else
				{
					check_improve(victim, gsn_nullify, false, 1);
				}
			}
		}

		if (!somatic)
			(*skill_table[sn].spell_fun)(sn, ch->level + check_spellcraft(ch, sn), ch, vo, target);
		else
			(*skill_table[sn].spell_fun)(sn, ch->level / 2, ch, vo, target);

		check_improve(ch, sn, true, 1);
	}

	if ((targtype == TAR_CHAR_OFFENSIVE || (targtype == TAR_OBJ_CHAR_OFF && target == TARGET_CHAR))
		&& victim != ch
		&& victim->master != ch
		&& !(is_affected(victim, gsn_bind_feet) && sn == gsn_bind_feet))
	{
		CHAR_DATA *vch, *vch_next;

		for (vch = ch->in_room->people; vch; vch = vch_next)
		{
			vch_next = vch->next_in_room;
			if (victim == vch && victim->fighting == nullptr)
			{
				multi_hit(victim, ch, TYPE_UNDEFINED);
				break;
			}
		}
	}
}

/*
 * Cast spells at targets using a magical object.
 */
void obj_cast_spell(int sn, int level, CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *obj)
{
	void *vo = nullptr;
	int target = TARGET_NONE;

	if (sn <= 0)
		return;

	if (sn >= MAX_SKILL || skill_table[sn].spell_fun == 0)
	{
		bug("Obj_cast_spell: bad sn %d.", sn);
		return;
	}

	if (IS_SET(ch->in_room->room_flags, ROOM_NO_MAGIC))
	{
		act("$n's spell fizzles.", ch, 0, 0, TO_ROOM);
		send_to_char("Your spell fizzles and dies.\n\r", ch);
		return;
	}

	switch (skill_table[sn].target)
	{
		case TAR_IGNORE:
			vo = nullptr;
			break;
		case TAR_CHAR_OFFENSIVE:
			if (victim == nullptr)
				victim = ch->fighting;

			if (victim == nullptr)
			{
				send_to_char("You can't do that.\n\r", ch);
				return;
			}

			if (is_safe(ch, victim) && ch != victim)
			{
				send_to_char("Something isn't right...\n\r", ch);
				return;
			}

			vo = (void *)victim;
			target = TARGET_CHAR;
			break;
		case TAR_CHAR_DEFENSIVE:
		case TAR_CHAR_SELF:
			if (victim == nullptr)
				victim = ch;

			vo = (void *)victim;
			target = TARGET_CHAR;
			break;
		case TAR_OBJ_INV:
			if (obj == nullptr)
			{
				send_to_char("You can't do that.\n\r", ch);
				return;
			}

			vo = (void *)obj;
			target = TARGET_OBJ;
			break;
		case TAR_OBJ_CHAR_OFF:
			if (victim == nullptr && obj == nullptr)
			{
				if (ch->fighting != nullptr)
				{
					victim = ch->fighting;
				}
				else
				{
					send_to_char("You can't do that.\n\r", ch);
					return;
				}
			}

			if (victim != nullptr)
			{
				if (is_safe_spell(ch, victim, false) && ch != victim)
				{
					send_to_char("Something isn't right...\n\r", ch);
					return;
				}

				vo = (void *)victim;
				target = TARGET_CHAR;
			}
			else
			{
				vo = (void *)obj;
				target = TARGET_OBJ;
			}

			break;
		case TAR_OBJ_CHAR_DEF:
			if (victim == nullptr && obj == nullptr)
			{
				vo = (void *)ch;
				target = TARGET_CHAR;
			}
			else if (victim != nullptr)
			{
				vo = (void *)victim;
				target = TARGET_CHAR;
			}
			else
			{
				vo = (void *)obj;
				target = TARGET_OBJ;
			}

			break;
		case TAR_CHAR_AMBIGUOUS:
			if (victim == ch && !ch->fighting)
			{
				vo = (void *)victim;
				target = TARGET_CHAR;
			}
			else if (victim == nullptr && !ch->fighting)
			{
				victim = ch;
				vo = (void *)victim;
				target = TARGET_CHAR;
			}
			else if (victim == nullptr && ch->fighting)
			{
				victim = ch->fighting;
				vo = (void *)victim;
				target = TARGET_CHAR;
			}
			else if (victim)
			{
				vo = (void *)victim;
				target = TARGET_CHAR;
			}

			break;
		default:
			bug("Obj_cast_spell: bad target for sn %d.", sn);
			return;
	}

	target_name = "";
	(*skill_table[sn].spell_fun)(sn, level, ch, vo, target);

	if ((skill_table[sn].target == TAR_CHAR_OFFENSIVE
			|| (skill_table[sn].target == TAR_OBJ_CHAR_OFF && target == TARGET_CHAR)
			|| (skill_table[sn].target == TAR_CHAR_AMBIGUOUS && (!trusts(ch, victim) && !is_safe(ch, victim))))
		&& victim != ch
		&& victim->master != ch)
	{
		CHAR_DATA *vch;
		CHAR_DATA *vch_next;

		for (vch = ch->in_room->people; vch; vch = vch_next)
		{
			vch_next = vch->next_in_room;
			if (victim == vch && victim->fighting == nullptr)
			{
				multi_hit(victim, ch, TYPE_UNDEFINED);
				break;
			}
		}
	}
}

/*
 * Spell functions.
 */

void spell_acid_blast(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	int dam;

	dam = dice(level, 11);

	if (saves_spell(level, victim, DAM_ACID))
		dam /= 2;

	damage_old(ch, victim, dam, sn, DAM_ACID, true);
}

void spell_armor(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected(victim, sn))
	{
		if (victim == ch)
			send_to_char("You are already armored.\n\r", ch);
		else
			act("$N is already armored.", ch, nullptr, victim, TO_CHAR);

		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = get_spell_aftype(ch);
	af.type = sn;
	af.level = level;
	af.duration = level;
	af.modifier = level / 4;
	af.location = APPLY_AC;
	affect_to_char(victim, &af);

	send_to_char("You feel someone protecting you.\n\r", victim);

	if (ch != victim)
		act("$N is protected by your magic.", ch, nullptr, victim, TO_CHAR);
}

void spell_bless(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim;
	AFFECT_DATA af;

	if (target == TARGET_OBJ)
	{
		send_to_char("Uh, what are you thinking?\n\r", ch);
		return;
	}

	/* character target */
	victim = (CHAR_DATA *)vo;

	if (victim->position == POS_FIGHTING || is_affected(victim, sn))
	{
		if (victim == ch)
			send_to_char("You are already blessed.\n\r", ch);
		else
			act("$N already has divine favor.", ch, nullptr, victim, TO_CHAR);

		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = sn;
	af.level = level;
	af.duration = 6 + level;
	af.location = APPLY_HITROLL;
	af.aftype = AFT_COMMUNE;

	if (level < 8)
		af.modifier = 1;
	else
		af.modifier = level / 8;

	affect_to_char(victim, &af);

	af.location = APPLY_SAVING_SPELL;

	if (level < 8)
		af.modifier = -1;
	else
		af.modifier = 0 - level / 8;

	affect_to_char(victim, &af);
	send_to_char("You feel righteous.\n\r", victim);

	if (ch != victim)
		act("You grant $N the favor of your god.", ch, nullptr, victim, TO_CHAR);
}

void spell_blindness(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;
	int dur;

	if (is_affected_by(victim, AFF_BLIND))
	{
		act("$N is already blind!", ch, 0, victim, TO_CHAR);
		return;
	}

	if (saves_spell(level - 3, victim, DAM_OTHER)
		|| IS_SET(victim->form, ACT_UNDEAD)
		|| IS_SET(victim->form, FORM_UNDEAD))
	{
		act("You failed to blind $N.", ch, 0, victim, TO_CHAR);
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_INVIS;
	af.type = sn;
	af.level = level;
	af.location = APPLY_HITROLL;
	af.modifier = -4;
	af.owner = ch;

	dur = level / 4;

	if (dur > 10)
		dur = 10;

	af.duration = dur;
	affect_to_char(victim, &af);

	af.aftype = get_spell_aftype(ch);
	af.location = 0;
	af.modifier = 0;
	af.mod_name = MOD_VISION;

	SET_BIT(af.bitvector, AFF_BLIND);

	affect_to_char(victim, &af);

	send_to_char("You are blinded!\n\r", victim);
	act("$n appears to be blinded.", victim, nullptr, nullptr, TO_ROOM);
}

void spell_burning_hands(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	static const short dam_each[] =
	{
		0,  0,  0,  0,  0,  14, 17, 20, 23, 26, 29, 29, 29, 30, 30, 31, 31,
		32, 32, 33, 33, 34, 34, 35, 35, 36, 36, 37, 37, 38, 38, 39, 39, 40,
		40, 41, 41, 42, 42, 43, 43, 44, 44, 45, 45, 46, 46, 47, 47, 48, 48
	};

	int dam;

	level = std::min(level, (int)sizeof(dam_each) / (int)sizeof(dam_each[0]) - 1);
	level = std::max(0, level);

	dam = number_range(dam_each[level] / 2, dam_each[level] * 2);

	if (saves_spell(level, victim, DAM_FIRE))
		dam /= 2;

	damage_old(ch, victim, dam, sn, DAM_FIRE, true);
}

/* RT calm spell stops all fighting in the room */

void spell_cancellation(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	char arg1[MSL], arg2[MSL];
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA *af;
	bool found = false;
	bool commune = false;
	int spell;

	target_name = one_argument(target_name, arg1);
	one_argument(target_name, arg2);

	if (arg1[0] == '\0')
		victim = ch;

	if (arg1[0] != '\0')
	{
		if ((victim = get_char_room(ch, arg1)) == nullptr)
		{
			ch->wait = 0;
			return send_to_char("They aren't here.\n\r", ch);
		}
	}

	if (!trusts(ch, victim))
	{
		send_to_char("They don't allow you to do that to them.\n\r", ch);
		return;
	}

	level += 2;

	if (ch->Class()->ctype == CLASS_COMMUNER)
		commune = true;

	if (arg2[0] == '\0')
	{
		if ((!is_npc(ch) && is_npc(victim) && !(is_affected_by(ch, AFF_CHARM) && ch->master == victim))
			|| (is_npc(ch) && !is_npc(victim)))
		{
			send_to_char("You failed.\n\r", ch);
		}

		for (af = victim->affected; af != nullptr; af = af->next)
		{
			if (skill_table[af->type].dispel & CAN_CANCEL)
			{
				check_dispel_cancellation(level, victim, ch, af->type, commune, true);
				found = true;
			}
		}
	}
	else
	{
		spell = find_spell(victim, arg2);

		if (spell < 1)
			return;

		for (af = victim->affected; af != nullptr; af = af->next)
		{
			if (af->type == spell)
				break;
		}

		if (!af || !af->type || !(skill_table[af->type].dispel & CAN_CANCEL))
			return;

		found = true;
		check_dispel_cancellation(level + 3, victim, ch, af->type, commune, true);
	}

	if (!found && victim == ch)
		send_to_char("You aren't affected by anything that can be cancelled.\n\r", ch);
	else if (!found && victim != ch)
		send_to_char("They aren't affected by anything that can be cancelled.\n\r", ch);
}

void spell_cause_light(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	damage_old(ch, (CHAR_DATA *)vo, dice(1, 8) + level / 3, sn, DAM_INTERNAL, true);
}

void spell_cause_critical(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	damage_old(ch, (CHAR_DATA *)vo, dice(3, 8) + level - 6, sn, DAM_INTERNAL, true);
}

void spell_cause_serious(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	damage_old(ch, (CHAR_DATA *)vo, dice(2, 8) + level / 2, sn, DAM_INTERNAL, true);
}

void spell_chain_lightning(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	CHAR_DATA *tmp_vict, *last_vict, *next_vict;
	bool found;
	int dam;

	/* first strike */
	if (is_affected(ch, gsn_grounding))
	{
		send_to_char("The electricity fizzles at your feet.\n\r", ch);
		act("A lightning bolt leaps from $n's hand but fizzles into the ground.", ch, 0, 0, TO_ROOM);
		return;
	}

	act("A lightning bolt leaps from $n's hand and arcs to $N.", ch, nullptr, victim, TO_NOTVICT);
	act("A lightning bolt leaps from your hand and arcs to $N.", ch, nullptr, victim, TO_CHAR);
	act("A lightning bolt leaps from $n's hand and hits you!", ch, nullptr, victim, TO_VICT);

	dam = dice(level + 2, 5);

	if (saves_spell(level, victim, DAM_LIGHTNING))
		dam /= 3;

	damage_old(ch, victim, dam, sn, DAM_LIGHTNING, true);

	last_vict = victim;
	level -= 4; /* decrement damage */

	/* new targets */
	while (level > 0)
	{
		found = false;

		for (tmp_vict = ch->in_room->people; tmp_vict != nullptr; tmp_vict = next_vict)
		{
			next_vict = tmp_vict->next_in_room;

			if (!is_safe_spell(ch, tmp_vict, true) && tmp_vict != last_vict)
			{
				found = true;
				last_vict = tmp_vict;
				if (last_vict == ch)
				{
					if (number_percent() >= ch->level)
					{
						act("The bolt arcs back to $n, and it stabilizes!", tmp_vict, nullptr, nullptr, TO_ROOM);
						act("The bolt arcs back to you and you stabilize its energy!", tmp_vict, nullptr, nullptr, TO_ROOM);
						level += 7;
					}
					else
					{
						act("The bolt arcs back to $n!", tmp_vict, nullptr, nullptr, TO_ROOM);
						act("The bolt arcs back to you!", tmp_vict, nullptr, nullptr, TO_ROOM);
					}
				}
				else
				{
					act("The bolt arcs to $n!", tmp_vict, nullptr, nullptr, TO_ROOM);
					act("The bolt hits you!", tmp_vict, nullptr, nullptr, TO_CHAR);
					dam = dice(level, 5);
					if (saves_spell(level, tmp_vict, DAM_LIGHTNING))
						dam /= 3;
					damage_old(ch, tmp_vict, dam, sn, DAM_LIGHTNING, true);
					level -= 4; /* decrement damage */
				}
			}
		} /* end target searching loop */

		if (!found) /* no target found, hit the caster */
		{
			if (ch == nullptr)
				return;

			if (last_vict == ch) /* no double hits */
			{
				act("The bolt seems to have fizzled out.", ch, nullptr, nullptr, TO_ROOM);
				act("The bolt grounds out harmlessly through your body.", ch, nullptr, nullptr, TO_CHAR);
				return;
			}

			last_vict = ch;

			act("The bolt arcs back to $n!", ch, nullptr, nullptr, TO_ROOM);
			send_to_char("The bolt arcs harmlessly back to you.\n\r", ch);

			level -= 4; /* decrement damage */

			if (ch == nullptr)
				return;
		}
		/* now go back and find more targets */
	}
}

void spell_change_sex(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected(victim, sn))
	{
		if (victim == ch)
			send_to_char("You've already been changed.\n\r", ch);
		else
			act("$N has already had $s(?) sex changed.", ch, nullptr, victim, TO_CHAR);
		return;
	}

	if (saves_spell(level, victim, DAM_OTHER))
		return;

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = sn;
	af.level = level;
	af.duration = 2 * level;
	af.location = APPLY_SEX;

	do
	{
		af.modifier = number_range(0, 2) - victim->sex;
	} while (af.modifier == 0);

	affect_to_char(victim, &af);
	send_to_char("You feel different.\n\r", victim);
	act("$n doesn't look like $mself anymore...", victim, nullptr, nullptr, TO_ROOM);
}

void spell_charm_person(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;
	CHAR_DATA *check;
	int count;

	count = 0;

	if (is_safe(ch, victim))
		return;

	if (victim == ch)
	{
		send_to_char("You like yourself even better!\n\r", ch);
		return;
	}

	if (is_affected_by(victim, AFF_CHARM)
		|| is_affected_by(ch, AFF_CHARM)
		|| level < victim->level
		|| IS_SET(victim->imm_flags, IMM_CHARM)
		|| (is_npc(victim) && IS_SET(victim->act, ACT_AGGRESSIVE))
		|| saves_spell(level - 3, victim, DAM_CHARM))
	{
		return;
	}

	for (check = char_list; check != nullptr; check = check->next)
	{
		if (check->leader == ch && is_affected_by(check, AFF_CHARM))
			count++;
	}

	if ((ch->level < 40 && count > 0) || (ch->level < 52 && count > 1) || count > 2)
	{
		send_to_char("You already control as many charmies as you can.\n\r", ch);
		return;
	}

	if (victim->master)
		stop_follower(victim);

	add_follower(victim, ch);

	victim->leader = ch;

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = sn;
	af.level = level;
	af.duration = number_fuzzy(level / 4);
	af.location = 0;
	af.modifier = 0;
	af.mod_name = MOD_CONC;

	SET_BIT(af.bitvector, AFF_CHARM);

	affect_to_char(victim, &af);

	act("Isn't $n just so nice?", ch, nullptr, victim, TO_VICT);

	if (ch != victim)
		act("$N looks at you with adoring eyes.", ch, nullptr, victim, TO_CHAR);
}

void spell_chill_touch(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	static const short dam_each[] =
	{
		0,  0,  0,  6,  7,  8,  9,  12, 13, 13, 13, 14, 14, 14, 15, 15, 15,
		16, 16, 16, 17, 17, 17, 18, 18, 18, 19, 19, 19, 20, 20, 20, 21, 21,
		21, 22, 22, 22, 23, 23, 23, 24, 24, 24, 25, 25, 25, 26, 26, 26, 27
	};

	AFFECT_DATA af;
	int dam;

	level = std::min(level, (int)sizeof(dam_each) / (int)sizeof(dam_each[0]) - 1);
	level = std::max(0, level);

	dam = number_range(dam_each[level] / 2, dam_each[level] * 2);
	dam *= 2;

	if (!saves_spell(level, victim, DAM_COLD))
	{
		act("$n turns blue and shivers.", victim, nullptr, nullptr, TO_ROOM);

		init_affect(&af);
		af.where = TO_AFFECTS;
		af.aftype = AFT_SPELL;
		af.type = sn;
		af.level = level;
		af.duration = 10;
		af.location = APPLY_STR;
		af.modifier = -1;
		affect_join(victim, &af);
	}
	else
	{
		dam /= 2;
	}

	damage_old(ch, victim, dam, sn, DAM_COLD, true);
}

void spell_color_spray(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	static const short dam_each[] =
	{
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  30, 35, 40, 45, 50, 55,
		55, 55, 56, 57, 58, 58, 59, 60, 61, 61, 62, 63, 64, 64, 65, 66, 67,
		67, 68, 69, 70, 70, 71, 72, 73, 73, 74, 75, 76, 76, 77, 78, 79, 79
	}
	;
	int dam;

	level = std::min(level, (int)sizeof(dam_each) / (int)sizeof(dam_each[0]) - 1);
	level = std::max(0, level);

	dam = number_range(dam_each[level] / 2, dam_each[level] * 2);

	if (saves_spell(level, victim, DAM_LIGHT))
		dam /= 2;
	else
		spell_blindness(skill_lookup("blindness"), level / 2, ch, (void *)victim, TARGET_CHAR);

	damage_old(ch, victim, dam, sn, DAM_LIGHT, true);
}

void spell_wrath(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	int dam = 0;

	if (!is_evil(victim))
	{
		act("$N is unaffected by $n's heavenly wrath.", ch, nullptr, victim, TO_NOTVICT);
		act("You are unaffected by $n's heavenly wrath.\n\r", ch, nullptr, victim, TO_VICT);
		send_to_char("The Gods do not enhance your wrath and frown on your actions\n\r", ch);
		return;
	}

	if (level <= 25)
		dam += dice(level - 5, 2);
	else if (level <= 30)
		dam += dice(level / 2, 2) + dice(level / 2, 3);
	else if (level < 35)
		dam += dice(level - 5, 3);
	else if (level < 40)
		dam += dice(level / 2, 3) + dice(level / 2, 4);
	else if (level < 46)
		dam += dice(level - 5, 4);
	else
		dam += dice(level - 5, 5);

	if (saves_spell(level, victim, DAM_HOLY) || saves_spell(level, victim, DAM_HOLY))
		dam /= 2;

	act("You call down the wrath of god upon $N.", ch, 0, victim, TO_CHAR);
	act("$n calls down the wrath of god upon $N.", ch, 0, victim, TO_NOTVICT);
	act("$n calls down the wrath of god upon you.", ch, 0, victim, TO_VICT);

	damage_old(ch, victim, dam, sn, DAM_HOLY, true);

	if (number_range(0, 3) != 0)
		return;

	spell_curse(gsn_curse, level, ch, (void *)victim, TARGET_CHAR);
}

void spell_continual_light(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	OBJ_DATA *light;

	if (target_name[0] != '\0') /* do a glow on some object */
	{
		light = get_obj_carry(ch, target_name, ch);

		if (light == nullptr)
		{
			send_to_char("You don't see that here.\n\r", ch);
			return;
		}

		if (is_obj_stat(light, ITEM_GLOW))
		{
			act("$p is already glowing.", ch, light, nullptr, TO_CHAR);
			return;
		}

		SET_BIT(light->extra_flags, ITEM_GLOW);
		act("$p glows with a white light.", ch, light, nullptr, TO_ALL);
		return;
	}

	light = create_object(get_obj_index(OBJ_VNUM_LIGHT_BALL), 0);

	obj_to_room(light, ch->in_room);

	act("$n twiddles $s thumbs and $p appears.", ch, light, nullptr, TO_ROOM);
	act("You twiddle your thumbs and $p appears.", ch, light, nullptr, TO_CHAR);
}

void spell_control_weather(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	return;
}

void spell_create_food(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	OBJ_DATA *mushroom;

	mushroom = create_object(get_obj_index(OBJ_VNUM_MUSHROOM), 0);
	mushroom->value[0] = level / 2;
	mushroom->value[1] = level;

	obj_to_room(mushroom, ch->in_room);

	act("$p suddenly appears.", ch, mushroom, nullptr, TO_ROOM);
	act("$p suddenly appears.", ch, mushroom, nullptr, TO_CHAR);
}

void spell_create_rose(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	OBJ_DATA *rose = create_object(get_obj_index(OBJ_VNUM_ROSE), 0);

	act("$n has created a beautiful red rose.", ch, rose, nullptr, TO_ROOM);
	send_to_char("You create a beautiful red rose.\n\r", ch);
	obj_to_char(rose, ch);
}

void spell_create_spring(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	OBJ_DATA *spring;

	if (ch->in_room->sector_type == SECT_WATER
		|| ch->in_room->sector_type == SECT_UNDERWATER
		|| ch->in_room->sector_type == SECT_AIR
		|| ch->in_room->sector_type == SECT_BURNING
		|| ch->in_room->sector_type == SECT_VERTICAL)
	{
		send_to_char("You cannot create a spring here.\n\r", ch);
		return;
	}

	spring = create_object(get_obj_index(OBJ_VNUM_SPRING), 0);

	spring->timer = level;

	obj_to_room(spring, ch->in_room);

	act("$p flows from the ground.", ch, spring, nullptr, TO_ROOM);
	act("$p flows from the ground.", ch, spring, nullptr, TO_CHAR);
}

void spell_create_water(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	OBJ_DATA *obj = (OBJ_DATA *)vo;
	int water;

	if (obj->item_type != ITEM_DRINK_CON)
	{
		send_to_char("It is unable to hold water.\n\r", ch);
		return;
	}

	if (obj->value[2] != LIQ_WATER && obj->value[1] != 0)
	{
		send_to_char("It contains some other liquid.\n\r", ch);
		return;
	}

	water = std::min(level * (ch->in_room->area->sky >= WeatherCondition::Drizzle ? 4 : 2), obj->value[0] - obj->value[1]);

	if (water > 0)
	{
		obj->value[2] = LIQ_WATER;
		obj->value[1] += water;

		if (!is_name("water", obj->name))
		{
			char buf[MAX_STRING_LENGTH];

			sprintf(buf, "%s water", obj->name);

			free_pstring(obj->name);
			obj->name = palloc_string(buf);
		}

		act("$p is filled.", ch, obj, nullptr, TO_CHAR);
	}
}

void spell_cure_blindness(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;

	if (!is_affected(victim, gsn_blindness)
		&& !is_affected(victim, gsn_blindness_dust)
		&& !is_affected(victim, gsn_blinding_orb))
	{
		if (victim == ch)
			send_to_char("You aren't blind.\n\r", ch);
		else
			act("$N doesn't appear to be blinded.", ch, nullptr, victim, TO_CHAR);

		return;
	}

	if (is_affected(victim, gsn_blindness_dust))
	{
		if (check_dispel(level, victim, gsn_blindness_dust))
		{
			act("The dust in $n's eyes fades away.", victim, 0, 0, TO_ROOM);

			if (!is_affected(victim, gsn_blindness))
				return;
		}
	}

	if (check_dispel(level, victim, gsn_blindness))
	{
		send_to_char("Your vision returns!\n\r", victim);
		act("$n is no longer blinded.", victim, nullptr, nullptr, TO_ROOM);
	}
	else if (is_affected(victim, gsn_blindness))
	{
		if (is_npc(ch) && IS_SET(ch->act, ACT_IS_HEALER))
		{
			affect_strip(victim, gsn_blindness);
			send_to_char("Your vision returns!\n\r", victim);
		}
		else
		{
			send_to_char("Spell failed.\n\r", ch);
		}
	}

	if (check_dispel(level, victim, gsn_blinding_orb))
	{
		act("$n is no longer blinded.", victim, nullptr, nullptr, TO_ROOM);
	}
	else
	{
		send_to_char("Spell failed.\n\r", ch);
	}
}

void spell_cure_critical(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	int heal;

	heal = dice(3, 8) + level - 6;
	victim->hit = std::min(victim->hit + heal, (int)victim->max_hit);

	update_pos(victim);

	send_to_char("You feel better!\n\r", victim);

	if (ch != victim)
		send_to_char("Ok.\n\r", ch);
}

/* RT added to cure plague */
void spell_cure_disease(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;

	if (ch->Class()->name == "assassin" && victim != ch)
	{
		send_to_char("You are only able to purge your own diseases.\n\r", ch);
		return;
	}

	if (!is_affected(victim, gsn_plague))
	{
		if (victim == ch)
			send_to_char("You aren't ill.\n\r", ch);
		else
			act("$N doesn't appear to be diseased.", ch, nullptr, victim, TO_CHAR);

		return;
	}

	if (check_dispel(level, victim, gsn_plague))
	{
		send_to_char("Your sores vanish.\n\r", victim);
		act("$n looks relieved as $s sores vanish.", victim, nullptr, nullptr, TO_ROOM);
	}
	else
	{
		send_to_char("Spell failed.\n\r", ch);
	}
}

void spell_cure_light(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	int heal;

	heal = dice(1, 8) + level / 3;
	victim->hit = std::min(victim->hit + heal, (int)victim->max_hit);

	update_pos(victim);
	send_to_char("You feel better!\n\r", victim);

	if (ch != victim)
		send_to_char("Ok.\n\r", ch);
}

void spell_cure_poison(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;

	if (ch->Class()->name == "assassin" && victim != ch)
	{
		send_to_char("You are only able to purge toxins from your own body.\n\r", ch);
		return;
	}

	if (!is_affected(victim, gsn_poison) && !is_affected(victim, gsn_poison_dust))
	{
		if (victim == ch)
			send_to_char("You aren't poisoned.\n\r", ch);
		else
			act("$N doesn't appear to be poisoned.", ch, nullptr, victim, TO_CHAR);

		return;
	}

	if (is_affected(victim, gsn_poison_dust))
	{
		if (check_dispel(level, victim, gsn_poison_dust))
			send_to_char("A warm feelings runs through you as the poison dust vanishes.\n\r", victim);

		act("$n looks much better.", victim, 0, 0, TO_ROOM);

		if (!is_affected(victim, gsn_poison))
			return;
	}

	if (check_dispel(level, victim, gsn_poison))
	{
		send_to_char("A warm feeling runs through your body.\n\r", victim);
		act("$n looks much better.", victim, nullptr, nullptr, TO_ROOM);
	}
	else
	{
		send_to_char("Spell failed.\n\r", ch);
	}
}

void spell_cure_serious(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	int heal;

	heal = dice(2, 8) + level / 2;
	victim->hit = std::min(victim->hit + heal, (int)victim->max_hit);

	update_pos(victim);
	send_to_char("You feel better!\n\r", victim);

	if (ch != victim)
		send_to_char("Ok.\n\r", ch);
}

void spell_curse(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim;
	OBJ_DATA *obj;
	AFFECT_DATA af;
	OBJ_AFFECT_DATA oaf;

	/* deal with the object case first
	if (target == TARGET_OBJ)
	{
		obj = (OBJ_DATA *) vo;
		if (is_obj_stat(obj,ITEM_EVIL))
		{
			act("$p is already filled with evil.",ch,obj,nullptr,TO_CHAR);
			return;
		}

		if (is_obj_stat(obj,ITEM_BLESS))
		{
			OBJ_AFFECT_DATA *paf;

			paf = affect_find_obj(obj->affected,skill_lookup("bless"));
			if (!saves_dispel(level,paf != nullptr ? paf->level : obj->level,0))
			{
				if (paf != nullptr)
					affect_remove_obj(obj,paf,true);
				act("$p glows with a red aura.",ch,obj,nullptr,TO_ALL);
				REMOVE_BIT(obj->extra_flags,ITEM_BLESS);
				return;
			}
			else
			{
				act("The holy aura of $p is too powerful for you to overcome.",
					ch,obj,nullptr,TO_CHAR);
				return;
			}
		}

		init_affect_obj(&oaf);
		oaf.where        = TO_OBJ_AFFECTS;
		oaf.type         = sn;
		oaf.aftype     = get_spell_aftype(ch);
		oaf.level        = level;
		oaf.duration     = 2 * level;
		oaf.location     = APPLY_SAVES;
		oaf.modifier     = +1;
		SET_BIT(oaf.bitvector, ITEM_EVIL);
		affect_to_obj(obj,&oaf);

		act("$p glows with a malevolent aura.",ch,obj,nullptr,TO_ALL);

		if (obj->wear_loc != WEAR_NONE)
			ch->saving_throw += 1;

		return;
	}
	*/

	/* character curses */
	victim = (CHAR_DATA *)vo;

	if (is_affected_by(victim, AFF_CURSE))
	{
		send_to_char("They are already cursed.\n\r", ch);
		return;
	}

	if (saves_spell(level, victim, DAM_NEGATIVE))
	{
		act("You failed to curse $N.", ch, 0, victim, TO_CHAR);
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = get_spell_aftype(ch);
	af.type = sn;
	af.level = level;
	af.duration = (4 + level / 3);
	af.location = APPLY_HITROLL;
	af.modifier = -1 * (level / 8);

	SET_BIT(af.bitvector, AFF_CURSE);

	affect_to_char(victim, &af);

	af.location = APPLY_SAVING_SPELL;
	af.modifier = level / 8;
	affect_to_char(victim, &af);

	send_to_char("You feel unclean.\n\r", victim);

	if (ch != victim)
		act("$N looks very uncomfortable.", ch, nullptr, victim, TO_CHAR);
}

void spell_dark_wrath(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	int dam;

	if (!is_good(victim))
	{
		act("$N is unaffected by $n's dark wrath.", ch, nullptr, victim, TO_NOTVICT);
		act("$N is unaffected by your dark wrath.", ch, nullptr, victim, TO_CHAR);
		act("You are unaffected by $n's dark wrath.", ch, nullptr, victim, TO_VICT);
		return;
	}

	dam = dice(level, 9);

	if (saves_spell(level, victim, DAM_NEGATIVE))
		dam /= 2;

	damage_old(ch, victim, dam, sn, DAM_NEGATIVE, true);

	if (number_range(0, 3) != 0)
		return;

	spell_curse(gsn_curse, level, ch, (void *)victim, TARGET_CHAR);
}

/* RT replacement demonfire spell */

void spell_demonfire(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	int dam, dam_mod, oldhp;

	oldhp = victim->hit;

	if (is_opp_align(ch, victim))
		dam_mod = (int)5.5;
	else
		dam_mod = 4;

	dam = dice(level, dam_mod);

	if (victim != ch)
	{
		act("$n calls forth the demons of Hell upon $N!", ch, nullptr, victim, TO_NOTVICT);
		act("$n assails you with the demons of Hell!", ch, nullptr, victim, TO_VICT);
		send_to_char("You conjure forth the demons of hell!\n\r", ch);
	}

	if (saves_spell(level, victim, DAM_NEGATIVE))
		dam /= 2;

	damage_old(ch, victim, dam, sn, DAM_NEGATIVE, true);

	if (victim->ghost)
		return;

	if (number_percent() > 75 && !is_affected_by(victim, AFF_CURSE))
		spell_curse(gsn_curse, 3 * level / 4, ch, (void *)victim, TARGET_CHAR);
}

void spell_detect_evil(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected_by(victim, AFF_DETECT_EVIL))
	{
		if (victim == ch)
			send_to_char("You can already sense evil.\n\r", ch);
		else
			act("$N can already detect evil.", ch, nullptr, victim, TO_CHAR);

		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_COMMUNE;
	af.type = sn;
	af.level = level;
	af.duration = level;
	af.modifier = 0;
	af.location = APPLY_NONE;

	SET_BIT(af.bitvector, AFF_DETECT_EVIL);

	af.mod_name = MOD_VISION;
	affect_to_char(victim, &af);

	send_to_char("Your eyes tingle.\n\r", victim);

	if (ch != victim)
		send_to_char("Ok.\n\r", ch);
}

void spell_detect_good(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected_by(victim, AFF_DETECT_GOOD))
	{
		if (victim == ch)
			send_to_char("You can already sense good.\n\r", ch);
		else
			act("$N can already detect good.", ch, nullptr, victim, TO_CHAR);

		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = get_spell_aftype(ch);
	af.type = sn;
	af.level = level;
	af.duration = level;
	af.modifier = 0;
	af.location = APPLY_NONE;

	SET_BIT(af.bitvector, AFF_DETECT_GOOD);
	af.mod_name = MOD_VISION;
	affect_to_char(victim, &af);

	send_to_char("Your eyes tingle.\n\r", victim);

	if (ch != victim)
		send_to_char("Ok.\n\r", ch);
}

void do_detect_hidden(CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;
	int number;

	if (is_affected_by(ch, AFF_DETECT_HIDDEN))
	{
		send_to_char("You are already as alert as you can be. \n\r", ch);
		return;
	}

	number = get_skill(ch, gsn_detect_hidden);

	if (number == 0
		|| ch->level < skill_table[gsn_detect_hidden].skill_level[ch->Class()->GetIndex()])
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	number = number_percent();

	if (number > get_skill(ch, gsn_detect_hidden))
	{
		send_to_char("You peer into the shadows but your vision stays the same.\n\r", ch);
		check_improve(ch, gsn_detect_hidden, false, 2);
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SKILL;
	af.type = gsn_detect_hidden;
	af.level = ch->level;
	af.duration = ch->level / 2;
	af.location = APPLY_NONE;
	af.modifier = 0;

	SET_BIT(af.bitvector, AFF_DETECT_HIDDEN);

	af.mod_name = MOD_VISION;
	affect_to_char(ch, &af);

	send_to_char("Your awareness improves.\n\r", ch);
	check_improve(ch, gsn_detect_hidden, true, 2);
}

void do_detect_movement(CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;
	int number;

	if (is_affected(ch, gsn_detect_movement))
	{
		send_to_char("You are already listening closely.\n\r", ch);
		return;
	}

	number = get_skill(ch, gsn_detect_movement);

	if (number == 0
		|| ch->level < skill_table[gsn_detect_movement].skill_level[ch->Class()->GetIndex()])
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	if (number_percent() > number)
	{
		check_improve(ch, gsn_detect_movement, false, 2);
	}
	else
	{
		init_affect(&af);
		af.where = TO_AFFECTS;
		af.aftype = AFT_INVIS;
		af.type = gsn_detect_movement;
		af.level = ch->level;
		af.duration = ch->level;
		af.location = ch->level / 2;
		af.location = APPLY_NONE;
		af.modifier = 0;
		af.mod_name = MOD_PERCEPTION;
		affect_to_char(ch, &af);

		check_improve(ch, gsn_detect_movement, true, 2);
	}

	send_to_char("You listen intently to your surroundings.\n\r", ch);
}

void spell_detect_invis(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected_by(victim, AFF_DETECT_INVIS))
	{
		if (victim == ch)
			send_to_char("You can already see invisible.\n\r", ch);
		else
			act("$N can already see invisible things.", ch, nullptr, victim, TO_CHAR);

		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = get_spell_aftype(ch);
	af.type = sn;
	af.level = level;
	af.duration = level;
	af.mod_name = MOD_VISION;
	af.modifier = 0;
	af.location = APPLY_NONE;

	SET_BIT(af.bitvector, AFF_DETECT_INVIS);

	affect_to_char(victim, &af);

	send_to_char("Your eyes tingle.\n\r", victim);

	if (ch != victim)
		send_to_char("Ok.\n\r", ch);
}

void spell_detect_magic(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected_by(victim, AFF_DETECT_MAGIC))
	{
		if (victim == ch)
			send_to_char("You can already sense magical auras.\n\r", ch);
		else
			act("$N can already detect magic.", ch, nullptr, victim, TO_CHAR);
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = get_spell_aftype(ch);
	af.type = sn;
	af.level = level;
	af.duration = level;
	af.modifier = 0;
	af.location = APPLY_NONE;

	SET_BIT(af.bitvector, AFF_DETECT_MAGIC);

	af.mod_name = MOD_VISION;
	affect_to_char(victim, &af);

	send_to_char("Your eyes tingle.\n\r", victim);

	if (ch != victim)
		send_to_char("Ok.\n\r", ch);
}

void spell_detect_poison(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	OBJ_DATA *obj = (OBJ_DATA *)vo;

	if (obj->item_type == ITEM_DRINK_CON || obj->item_type == ITEM_FOOD)
	{
		if (obj->value[3] != 0)
			send_to_char("You smell poisonous fumes.\n\r", ch);
		else
			send_to_char("It looks delicious.\n\r", ch);
	}
	else
	{
		send_to_char("It doesn't look poisoned.\n\r", ch);
	}
}

void spell_dispel_evil(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	int dam;

	if (!is_npc(ch) && is_evil(ch))
		victim = ch;

	if (is_good(victim))
	{
		act("$N is protected.", ch, nullptr, victim, TO_ROOM);
		return;
	}

	if (is_neutral(victim))
	{
		act("$N does not seem to be affected.", ch, nullptr, victim, TO_CHAR);
		return;
	}

	dam = dice(level, 4);

	if (saves_spell(level, victim, DAM_HOLY))
		dam /= 2;

	damage_old(ch, victim, dam, sn, DAM_HOLY, true);
}

void spell_dispel_good(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	int dam;

	if (!is_npc(ch) && is_good(ch))
		victim = ch;

	if (is_evil(victim))
	{
		act("$N is protected by $S evil.", ch, nullptr, victim, TO_ROOM);
		return;
	}

	if (is_neutral(victim))
	{
		act("$N does not seem to be affected.", ch, nullptr, victim, TO_CHAR);
		return;
	}

	dam = dice(level, 4);

	if (saves_spell(level, victim, DAM_NEGATIVE))
		dam /= 2;

	damage_old(ch, victim, dam, sn, DAM_NEGATIVE, true);
}

/* modified for enhanced use */

void spell_dispel_magic(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	char arg1[MSL], arg2[MSL];
	char buf[MSL];
	CHAR_DATA *victim = nullptr;
	AFFECT_DATA *af;
	bool found = false;
	bool commune = false;
	int spell;

	if (!ch->fighting && !str_cmp(target_name, ""))
	{
		ch->wait = 0;

		send_to_char("Syntax: cast 'dispel magic' <target> [spellname].\n\r", ch);
		return;
	}

	if (ch->fighting && !str_cmp(target_name, ""))
		victim = ch->fighting;

	target_name = one_argument(target_name, arg1);
	one_argument(target_name, arg2);

	if (!victim)
	{
		if ((victim = get_char_room(ch, arg1)) == nullptr)
		{
			ch->wait = 0;

			send_to_char("They aren't here.\n\r", ch);
			return;
		}
	}

	if (ch->Class()->ctype == CLASS_COMMUNER)
		commune = true;

	if (!is_npc(victim) && (ch != victim) && (!ch->fighting || !victim->fighting))
	{
		sprintf(buf, "Die, %s, you sorcerous dog!", pers(ch, victim));
		do_myell(victim, buf, ch);
	}

	if (!victim->fighting && (ch != victim))
		multi_hit(victim, ch, TYPE_UNDEFINED);

	if (arg2[0] == '\0')
	{
		for (af = victim->affected; af != nullptr; af = af->next)
		{
			if (skill_table[af->type].dispel & CAN_DISPEL)
			{
				check_dispel_cancellation(level, victim, ch, af->type, commune, false);
				found = true;
			}
		}
	}
	else
	{
		spell = find_spell(victim, arg2);

		if (spell < 1)
			return;

		for (af = victim->affected; af != nullptr; af = af->next)
		{
			if (af->type == spell)
				break;
		}

		if (!af || !af->type || !(skill_table[af->type].dispel & CAN_DISPEL))
			return;

		check_dispel_cancellation(level + 3, victim, ch, af->type, commune, false);
	}

	return;
}

void spell_fireball(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;
	char buf[MAX_STRING_LENGTH];
	static const short dam_each[] =
	{
		  0,   0,	0,	 0,	  0,   0,	0,	 0,	  0,   0,	0,	 0,	  0,
		  0,   0,	30,	 35,  40,  45,	50,	 55,  60,  65,	70,	 75,  80,
		  82,  84,	86,	 88,  90,  92,	94,	 96,  98,  100, 102, 104, 106,
		  108, 110, 112, 114, 116, 118, 120, 122, 124, 126, 128, 130
	};

	int dam;

	level = std::min(level, (int)sizeof(dam_each) / (int)sizeof(dam_each[0]) - 1);
	level = std::max(0, level);

	dam = number_range(dam_each[level] / 2, dam_each[level] * 2);
	dam += dice(level, 3);
	dam *= 8;
	dam /= 10;

	for (vch = ch->in_room->people; vch != nullptr; vch = vch_next)
	{
		vch_next = vch->next_in_room;

		if (is_same_group(vch, ch) || is_safe(ch, vch) || is_same_cabal(ch, vch))
			continue;

		if (!is_npc(ch) && !is_npc(vch) && (ch->fighting == nullptr || vch->fighting == nullptr))
		{
			switch (number_range(0, 2))
			{
				case 0:
				case 1:
					sprintf(buf, "Die, %s you sorcerous dog!", pers(ch, vch));
					break;
				case 2:
					sprintf(buf, "Help! %s is casting a spell on me!", pers(ch, vch));
			}

			if (vch != ch)
				do_myell(vch, buf, ch);
		}

		if (saves_spell(level, vch, DAM_FIRE))
			dam /= 2;

		damage_old(ch, vch, dam, sn, DAM_FIRE, true);
	}
}

void spell_fireproof(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	OBJ_DATA *obj = (OBJ_DATA *)vo;
	OBJ_AFFECT_DATA oaf;

	if (is_obj_stat(obj, ITEM_BURN_PROOF))
	{
		act("$p is already protected from burning.", ch, obj, nullptr, TO_CHAR);
		return;
	}

	init_affect_obj(&oaf);
	oaf.where = TO_OBJ_AFFECTS;
	oaf.type = sn;
	oaf.level = level;
	oaf.duration = number_fuzzy(level / 4);
	oaf.location = APPLY_NONE;
	oaf.modifier = 0;
	SET_BIT(oaf.bitvector, ITEM_BURN_PROOF);

	affect_to_obj(obj, &oaf);

	act("You protect $p from fire.", ch, obj, nullptr, TO_CHAR);
	act("$p is surrounded by a protective aura.", ch, obj, nullptr, TO_ROOM);
}

void spell_flamestrike(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	int dam;

	dam = dice(6 + level / 2, 8);

	if (saves_spell(level, victim, DAM_FIRE))
		dam /= 2;

	damage_old(ch, victim, dam, sn, DAM_FIRE, true);
}

void spell_faerie_fire(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected(victim, sn))
	{
		act("$N is already glowing with faerie fire.", ch, 0, victim, TO_CHAR);
		return;
	}

	if (saves_spell(level + 15, victim, DAM_OTHER))
	{
		act("$N resists your magical faerie fire.", ch, 0, victim, TO_CHAR);
		act("You resist $n's magical faerie fire.", ch, 0, victim, TO_VICT);
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = get_spell_aftype(ch);
	af.type = sn;
	af.level = level;
	af.duration = 6 + (level / 5);
	af.location = APPLY_AC;
	af.modifier = -1 * (short)(average_ac(victim) * (level / 400.00f));
	af.mod_name = MOD_VISIBILITY;
	affect_to_char(victim, &af);

	send_to_char("You are surrounded by a pink outline.\n\r", victim);
	act("$n is surrounded by a pink outline.", victim, nullptr, nullptr, TO_ROOM);
}

void spell_faerie_fog(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *ich;
	AFFECT_DATA af;

	act("$n conjures a cloud of purple smoke.", ch, nullptr, nullptr, TO_ROOM);
	send_to_char("You conjure a cloud of purple smoke.\n\r", ch);

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = get_spell_aftype(ch);
	af.type = sn;
	af.level = level;
	af.duration = 4 + (level / 5);
	af.modifier = 0;
	af.location = 0;
	af.mod_name = MOD_VISIBILITY;

	for (ich = ch->in_room->people; ich != nullptr; ich = ich->next_in_room)
	{
		if (ich->invis_level > 0)
			continue;

		if (ich->trust > 51)
			continue;

		if (ich == ch || saves_spell(level, ich, DAM_OTHER))
			continue;

		if (is_affected(ich, sn))
			continue;

		affect_to_char(ich, &af);

		affect_strip(ich, gsn_camouflage);
		affect_strip(ich, gsn_invis);
		affect_strip(ich, gsn_mass_invis);
		affect_strip(ich, gsn_sneak);

		REMOVE_BIT(ich->affected_by, AFF_HIDE);
		REMOVE_BIT(ich->affected_by, AFF_INVISIBLE);
		REMOVE_BIT(ich->affected_by, AFF_SNEAK);

		act("$n is revealed!", ich, nullptr, nullptr, TO_ROOM);
		send_to_char("You are revealed!\n\r", ich);
	}
}

void spell_fly(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected_by(victim, AFF_FLYING))
	{
		if (victim == ch)
			send_to_char("You are already airborne.\n\r", ch);
		else
			act("$N doesn't need your help to fly.", ch, nullptr, victim, TO_CHAR);

		return;
	}

	if (is_affected(victim, gsn_grounding))
	{
		send_to_char("Magical grounding interferes with your spell.\n\r", ch);
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = get_spell_aftype(ch);
	af.type = sn;
	af.level = level;
	af.duration = level / 4;
	af.location = 0;
	af.modifier = 0;

	SET_BIT(af.bitvector, AFF_FLYING);

	af.mod_name = MOD_LEVITATION;
	affect_to_char(victim, &af);

	send_to_char("Your feet rise off the ground.\n\r", victim);
	act("$n's feet rise off the ground.", victim, nullptr, nullptr, TO_ROOM);
}

/* RT clerical berserking spell */

void spell_frenzy(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected(victim, sn) || is_affected_by(victim, AFF_BERSERK))
	{
		if (victim == ch)
			send_to_char("You are already in a frenzy.\n\r", ch);
		else
			act("$N is already in a frenzy.", ch, nullptr, victim, TO_CHAR);

		return;
	}

	if (is_affected(victim, skill_lookup("calm")))
	{
		if (victim == ch)
			send_to_char("Why don't you just relax for a while?\n\r", ch);
		else
			act("$N doesn't look like $e wants to fight anymore.", ch, nullptr, victim, TO_CHAR);

		return;
	}

	if ((is_good(ch) && !is_good(victim))
		|| (is_neutral(ch) && !is_neutral(victim))
		|| (is_evil(ch) && !is_evil(victim)))
	{
		act("Your god doesn't seem to like $N", ch, nullptr, victim, TO_CHAR);
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_COMMUNE;
	af.type = sn;
	af.level = level;
	af.duration = level / 3;
	af.modifier = level / 6;

	af.location = APPLY_HITROLL;
	affect_to_char(victim, &af);

	af.location = APPLY_DAMROLL;
	affect_to_char(victim, &af);

	send_to_char("You are filled with holy wrath!\n\r", victim);
	act("$n gets a wild look in $s eyes!", victim, nullptr, nullptr, TO_ROOM);
}

/* RT ROM-style gate */

void spell_gate(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim;
	bool gate_pet;

	if (ch->fighting != nullptr)
	{
		send_to_char("You can't concentrate enough.\n\r", ch);
		return;
	}

	victim = get_char_world(ch, target_name);

	if (!(victim == nullptr))
	{
		if ((is_npc(victim)) && (IS_SET(victim->res_flags, RES_SUMMON)))
			level -= 5;

		if ((is_npc(victim)) && (IS_SET(victim->vuln_flags, VULN_SUMMON)))
			level += 5;
	}

	if (victim == nullptr
		|| victim == ch
		|| victim->in_room == nullptr
		|| !can_see_room(ch, victim->in_room)
		|| IS_SET(victim->in_room->room_flags, ROOM_NO_GATE)
		|| IS_SET(victim->in_room->room_flags, ROOM_SAFE)
		|| IS_SET(victim->in_room->room_flags, ROOM_PRIVATE)
		|| victim->in_room->area->area_type == ARE_UNOPENED
		|| ch->in_room->guild != 0
		|| victim->in_room->guild != 0
		|| (victim->in_room->cabal != 0 && victim->in_room->cabal != ch->cabal)
		|| IS_SET(victim->in_room->room_flags, ROOM_SOLITARY)
		|| IS_SET(victim->in_room->room_flags, ROOM_NO_RECALL)
		|| IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL)
		|| victim->level >= level + 4
		|| (!is_npc(victim) && !can_pk(ch, victim) && IS_SET(victim->act, PLR_NOSUMMON))
		|| (!is_npc(victim) && saves_spell(level, victim, DAM_OTHER))
		|| (!is_npc(victim) && victim->level > LEVEL_HERO) /* NOT trust */
		|| (is_npc(victim) && IS_SET(victim->imm_flags, IMM_SUMMON))
		|| (is_npc(victim) && saves_spell(level, victim, DAM_OTHER))
		|| (is_affected_room(ch->in_room, gsn_plasma_cube) && number_percent() < 50)
		|| (is_affected_room(victim->in_room, gsn_plasma_cube) && number_percent() < 50))
	{
		send_to_char("You failed.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_bind_feet))
	{
		send_to_char("Your feet are rooted to the ground!\n\r", ch);
		return;
	}

	if (victim->in_room->cabal != 0 && victim->in_room->cabal != victim->cabal)
	{
		send_to_char("You failed.\n\r", victim);
		return;
	}

	if (ch->pet != nullptr && ch->in_room == ch->pet->in_room)
		gate_pet = true;
	else
		gate_pet = false;

	un_blade_barrier(ch, nullptr);

	act("$n steps through a gate and vanishes.", ch, nullptr, nullptr, TO_ROOM);

	send_to_char("You step through a gate and vanish.\n\r", ch);
	char_from_room(ch);
	char_to_room(ch, victim->in_room);

	act("$n has arrived through a gate.", ch, nullptr, nullptr, TO_ROOM);
	do_look(ch, "auto");
	check_plasma_thread(ch, -1);

	if (gate_pet)
	{
		act("$n steps through a gate and vanishes.", ch->pet, nullptr, nullptr, TO_ROOM);
		send_to_char("You step through a gate and vanish.\n\r", ch->pet);

		char_from_room(ch->pet);
		char_to_room(ch->pet, victim->in_room);

		act("$n has arrived through a gate.", ch->pet, nullptr, nullptr, TO_ROOM);
		do_look(ch->pet, "auto");
	}
}

void spell_giant_strength(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected(victim, sn))
	{
		if (victim == ch)
			send_to_char("You are already as strong as you can get!\n\r", ch);
		else
			act("$N can't get any stronger.", ch, nullptr, victim, TO_CHAR);
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = get_spell_aftype(ch);
	af.type = sn;
	af.level = level;
	af.duration = level;
	af.location = APPLY_STR;
	af.modifier = 1 + (level >= 18) + (level >= 25) + (level >= 32);
	affect_to_char(victim, &af);

	send_to_char("Your muscles surge with heightened power!\n\r", victim);
	act("$n's muscles surge with heightened power.", victim, nullptr, nullptr, TO_ROOM);
}

void spell_harm(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	int dam;

	dam = dice(level, 5);
	if (saves_spell(level, victim, DAM_INTERNAL))
		dam /= 2;

	damage_old(ch, victim, dam, sn, DAM_INTERNAL, true);
}

/* RT haste spell */

void spell_haste(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (ch->Class()->name == "assassin" && victim != ch)
	{
		send_to_char("You are only able to enhance your own physical abilities.\n\r", ch);
		return;
	}

	if (is_affected(victim, sn) || is_affected_by(victim, AFF_HASTE) || IS_SET(victim->off_flags, OFF_FAST))
	{
		if (victim == ch)
			send_to_char("You can't move any faster!\n\r", ch);
		else
			act("$N is already moving as fast as $E can.", ch, nullptr, victim, TO_CHAR);

		return;
	}

	if (is_affected_by(victim, AFF_SLOW))
	{
		send_to_char("You start moving at normal speed again.\n\r", victim);
		act("$n starts moving at normal speed again.", victim, nullptr, nullptr, TO_ROOM);
		affect_strip(victim, skill_lookup("slow"));

		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = sn;
	af.level = level;
	af.duration = level / 2;
	af.location = APPLY_DEX;
	af.modifier = 1 + (level >= 18) + (level >= 25) + (level >= 32);

	SET_BIT(af.bitvector, AFF_HASTE);

	af.mod_name = MOD_SPEED;
	affect_to_char(victim, &af);

	send_to_char("You feel yourself moving more quickly.\n\r", victim);
	act("$n is moving more quickly.", victim, nullptr, nullptr, TO_ROOM);

	if (ch != victim)
		send_to_char("Ok.\n\r", ch);
}

void spell_heal(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;

	victim->hit = std::min(victim->hit + 100, (int)victim->max_hit);

	update_pos(victim);
	send_to_char("A warm feeling fills your body.\n\r", victim);

	if (ch != victim)
		send_to_char("Ok.\n\r", ch);
}

/* RT really nasty high-level attack spell */
void spell_holy_word(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;
	int dam;
	int bless_num, curse_num, frenzy_num, sanc_num;
	int wrath_num;

	if (is_good(ch))
		wrath_num = skill_lookup("wrath");
	else
		wrath_num = skill_lookup("dark wrath");

	sanc_num = skill_lookup("sanctuary");
	bless_num = skill_lookup("bless");
	curse_num = skill_lookup("curse");
	frenzy_num = skill_lookup("frenzy");

	act("$n utters a word of divine power!", ch, nullptr, nullptr, TO_ROOM);
	send_to_char("You utter a word of divine power.\n\r", ch);

	for (vch = ch->in_room->people; vch != nullptr; vch = vch_next)
	{
		vch_next = vch->next_in_room;

		if (vch->invis_level > LEVEL_HERO)
			continue;

		if ((is_good(ch) && is_good(vch))
			|| (is_evil(ch) && is_evil(vch))
			|| ((is_neutral(ch) && is_neutral(vch)) && ch == vch))
		{
			send_to_char("You feel more powerful.\n\r", vch);
			spell_frenzy(frenzy_num, level, ch, (void *)vch, TARGET_CHAR);
			spell_bless(bless_num, level, ch, (void *)vch, TARGET_CHAR);
		}
		else if ((is_good(ch) && is_evil(vch))
			|| (is_evil(ch) && is_good(vch)))
		{
			if (!is_safe_spell(ch, vch, true))
			{
				spell_curse(curse_num, level, ch, (void *)vch, TARGET_CHAR);
				send_to_char("You are struck down!\n\r", vch);

				if (is_good(ch))
					spell_wrath(wrath_num, level, ch, (void *)vch, TARGET_CHAR);
				else
					spell_dark_wrath(wrath_num, level, ch, (void *)vch, TARGET_CHAR);

				/*
				dam = dice(level,6);
				damage_old(ch,vch,dam,sn,DAM_ENERGY,true);
				*/
			}
		}
		else if (is_neutral(ch))
		{
			if (!is_safe_spell(ch, vch, true))
			{
				spell_curse(curse_num, level / 2, ch, (void *)vch, TARGET_CHAR);
				send_to_char("You are struck down!\n\r", vch);
				dam = dice(level, 4);
				damage_old(ch, vch, dam, sn, DAM_ENERGY, true);
			}
		}
	}

	send_to_char("You feel drained.\n\r", ch);
	ch->hit -= 100;
}

void spell_identify(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	OBJ_DATA *obj = (OBJ_DATA *)vo;
	char buf[MAX_STRING_LENGTH];
	OBJ_APPLY_DATA *app;
	int lorebonus = 0;

	if (!is_npc(ch) && (lorebonus = ch->Profs()->GetProf("forgotten lore") > -1))
	{
		send_to_char("Your knowledge of forgotten lore grants you additional insight.\n\r", ch);
		ch->Profs()->CheckImprove("forgotten lore", 100);
	}

	lorebonus = std::max(lorebonus, 0);

	sprintf(buf, "Object '%s' is type %s, extra flags %s.\n\r",
		obj->name,
		item_name_lookup(obj->item_type),
		extra_bit_name(obj->extra_flags));
	strcat(buf, "It appears to be made of ");
	strcat(buf, obj->material);
	strcat(buf, ".\n\r");
	send_to_char(buf, ch);

	switch (obj->item_type)
	{
		case ITEM_SCROLL:
		case ITEM_POTION:
		case ITEM_PILL:
			sprintf(buf, "It is enchanted with level %d spells of:", obj->value[0]);
			send_to_char(buf, ch);

			if (obj->value[1] >= 0 && obj->value[1] < MAX_SKILL && str_cmp(skill_table[obj->value[1]].name, "reserved"))
			{
				send_to_char(" '", ch);
				send_to_char(skill_table[obj->value[1]].name, ch);
				send_to_char("'", ch);
			}

			if (obj->value[2] >= 0 && obj->value[2] < MAX_SKILL && str_cmp(skill_table[obj->value[2]].name, "reserved"))
			{
				send_to_char(" '", ch);
				send_to_char(skill_table[obj->value[2]].name, ch);
				send_to_char("'", ch);
			}

			if (obj->value[3] >= 0 && obj->value[3] < MAX_SKILL && str_cmp(skill_table[obj->value[3]].name, "reserved"))
			{
				send_to_char(" '", ch);
				send_to_char(skill_table[obj->value[3]].name, ch);
				send_to_char("'", ch);
			}

			if (obj->value[4] >= 0 && obj->value[4] < MAX_SKILL && str_cmp(skill_table[obj->value[4]].name, "reserved"))
			{
				send_to_char(" '", ch);
				send_to_char(skill_table[obj->value[4]].name, ch);
				send_to_char("'", ch);
			}

			send_to_char(".\n\r", ch);
			break;
		case ITEM_WAND:
		case ITEM_STAFF:
			sprintf(buf, "It is enchanted with %d charges of level %d", obj->value[2], obj->value[0]);
			send_to_char(buf, ch);

			if (obj->value[3] >= 0 && obj->value[3] < MAX_SKILL && str_cmp(skill_table[obj->value[3]].name, "reserved"))
			{
				send_to_char(" '", ch);
				send_to_char(skill_table[obj->value[3]].name, ch);
				send_to_char("'", ch);
			}

			send_to_char(".\n\r", ch);
			break;
		case ITEM_CONTAINER:
			sprintf(buf, "It %s contain about %d objects.\n\r",
				lorebonus ? "can" : "might be able to",
				lorebonus ? obj->value[0] : obj->value[0] + number_range(obj->value[0] * -1 / 2, obj->value[0] * 1 / 2));
			send_to_char(buf, ch);

			break;
		case ITEM_WEAPON:
			send_to_char("It appears to be a", ch);

			switch (obj->value[0])
			{
				case WEAPON_EXOTIC:
					send_to_char("n exotic.\n\r", ch);
					break;
				case WEAPON_SWORD:
					send_to_char(" sword.\n\r", ch);
					break;
				case WEAPON_DAGGER:
					send_to_char(" dagger.\n\r", ch);
					break;
				case WEAPON_SPEAR:
					send_to_char(" spear/staff.\n\r", ch);
					break;
				case WEAPON_MACE:
					send_to_char(" mace/club.\n\r", ch);
					break;
				case WEAPON_AXE:
					send_to_char("n axe.\n\r", ch);
					break;
				case WEAPON_FLAIL:
					send_to_char(" flail.\n\r", ch);
					break;
				case WEAPON_WHIP:
					send_to_char(" whip.\n\r", ch);
					break;
				case WEAPON_POLEARM:
					send_to_char(" polearm.\n\r", ch);
					break;
				case WEAPON_STAFF:
					send_to_char(" staff.\n\r", ch);
					break;
				default:
					send_to_char(" weapon of unknown species.\n\r", ch);
					break;
			}

			if (lorebonus)
				sprintf(buf, "Damage averages %d.\n\r", (1 + obj->value[2]) * obj->value[1] / 2);
			else
				sprintf(buf, "The average damage is probably around %d.\n\r", ((1 + obj->value[2]) * obj->value[1] / 2) + number_range(-5, 3));

			send_to_char(buf, ch);
			break;
		case ITEM_ARMOR:
			sprintf(buf, "Armor class averages %d.\n\r", (obj->value[0] + obj->value[1] + obj->value[2] + obj->value[3]) / 4);

			if (lorebonus)
				send_to_char(buf, ch);

			break;
	}

	for (app = obj->apply; app; app = app->next)
	{
		sprintf(buf, "Affects %s by %d.\n\r", affect_loc_name(app->location), app->modifier);
		send_to_char(buf, ch);
	}
}

void spell_invis(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim;
	AFFECT_DATA af;
	int sn_fog, sn_fire;

	sn_fog = skill_lookup("faerie fog");
	sn_fire = skill_lookup("faerie fire");

	if (is_affected(ch, sn_fog) || is_affected(ch, sn_fire))
	{
		send_to_char("You can't turn invisible while glowing.\n\r", ch);
		return;
	}

	if (target == TARGET_OBJ)
	{
		send_to_char("You must be crazy.\n\r", ch);
		return;
	}
	/* character invisibility */
	victim = (CHAR_DATA *)vo;

	if (is_affected_by(victim, AFF_INVISIBLE))
		return;

	act("$n fades out of existence.", victim, nullptr, nullptr, TO_ROOM);

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = sn;
	af.level = level;
	af.duration = level + 12;
	af.location = APPLY_NONE;
	af.modifier = 0;

	SET_BIT(af.bitvector, AFF_INVISIBLE);

	af.mod_name = MOD_VISIBILITY;
	affect_to_char(victim, &af);

	send_to_char("You fade out of existence.\n\r", victim);
}

void spell_know_alignment(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	char *msg;
	int ap;

	ap = get_align(victim);

	if (ap > 0)
		msg = "$N has a pure and good aura.";
	else if (ap == 0)
		msg = "$N doesn't have a firm moral commitment.";
	else
		msg = "$N is the embodiment of pure evil!.";

	act(msg, ch, nullptr, victim, TO_CHAR);

	if (!is_npc(victim))
		ap = get_ethos(victim);

	if (ap > 0)
		msg = "$N abides by the law of the land.";
	else if (ap == 0)
		msg = "$N doesn't know what to think about the law";
	else
		msg = "$N has absolutely no cares for the law.";

	act(msg, ch, nullptr, victim, TO_CHAR);
	return;
}

void spell_lightning_bolt(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	static const short dam_each[] =
	{
		0,  0,  0,  0,  0,  0,  0,  0,  0,  25, 28, 31, 34, 37, 40, 40, 41,
		42, 42, 43, 44, 44, 45, 46, 46, 47, 48, 48, 49, 50, 50, 51, 52, 52,
		53, 54, 54, 55, 56, 56, 57, 58, 58, 59, 60, 60, 61, 62, 62, 63, 64
	}
	;
	float dam;

	level = std::min(level, (int)sizeof(dam_each) / (int)sizeof(dam_each[0]) - 1);
	level = std::max(0, level);
	dam = number_range(dam_each[level] / 2, dam_each[level] * 2);

	if (ch->Class()->name == "sorcerer")
	{
		act("You extend an arm towards $N and raw lightning leaps from your fingertips!", ch, nullptr, victim, TO_CHAR);
		act("$n extends an arm towards you and raw lightning leaps from $s fingertips!", ch, nullptr, victim, TO_VICT);
		act("$n extends an arm towards $N and raw lightning leaps from $s fingertips!", ch, nullptr, victim, TO_NOTVICT);
		dam *= 1.5;
	}

	if (saves_spell(level, victim, DAM_LIGHTNING))
		dam /= 2;

	damage_old(ch, victim, (int)dam, sn, DAM_LIGHTNING, true);
}

void spell_locate_object(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	char buf[MAX_INPUT_LENGTH];
	BUFFER *buffer;
	OBJ_DATA *obj;
	OBJ_DATA *in_obj;
	bool found;
	int number = 0, max_found;

	found = false;
	number = 0;
	max_found = is_immortal(ch) ? 200 : 2 * level;

	buffer = new_buf();

	for (obj = object_list; obj != nullptr; obj = obj->next)
	{
		if (!can_see_obj(ch, obj)
			|| !is_name(target_name, obj->name)
			|| is_obj_stat(obj, ITEM_NOLOCATE)
			|| number_percent() > level
			|| ch->level < obj->level
			|| (obj->carried_by && is_immortal(obj->carried_by)))
		{
			continue;
		}

		found = true;
		number++;

		for (in_obj = obj; in_obj->in_obj != nullptr; in_obj = in_obj->in_obj)
			;

		if (in_obj->carried_by != nullptr)
		{
			if (!is_affected(in_obj->carried_by, gsn_shadow_cloak))
			{
				sprintf(buf, "%s is carried by %s.\n\r", obj->short_descr, pers(in_obj->carried_by, ch));
			}
			else
			{
				found = false;
				number--;
				continue;
			}
		}
		else if (in_obj->in_room != nullptr)
		{
			if (is_affected(in_obj->carried_by, gsn_stash))
				sprintf(buf, "%s's location is blurry, you have a hard time trying to locate it.\n\r", obj->short_descr);
			else
				sprintf(buf, "%s is in %s.\n\r", obj->short_descr, get_room_name(in_obj->in_room));
		}
		else
		{
			found = false;
			number--;
			continue;
		}

		buf[0] = UPPER(buf[0]);

		add_buf(buffer, buf);

		if (number >= max_found)
			break;
	}

	if (!found)
		send_to_char("Nothing like that in heaven or earth.\n\r", ch);
	else
		page_to_char(buf_string(buffer), ch);

	free_buf(buffer);
}

void spell_magic_missile(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	int dam;

	dam = dice(level, 3);
	dam += level / 4;

	if (saves_spell(level, victim, DAM_ENERGY))
		dam /= 2;

	act("Missiles of pure mana streak from $n's hands to strike $N!", ch, 0, victim, TO_NOTVICT);
	act("Missiles of pure mana streak from $n's hands to strike you!", ch, 0, victim, TO_VICT);
	act("Missiles of pure mana streak from your hands to strike $N!", ch, 0, victim, TO_CHAR);

	damage_new(ch, victim, dam, sn, DAM_ENERGY, true, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "magic missiles$");
}

void spell_mass_healing(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *gch;
	int heal_num, refresh_num;

	heal_num = skill_lookup("heal");
	refresh_num = skill_lookup("refresh");

	for (gch = ch->in_room->people; gch != nullptr; gch = gch->next_in_room)
	{
		if (is_same_group(ch, gch))
		{
			spell_heal(heal_num, level, ch, (void *)gch, TARGET_CHAR);
			spell_refresh(refresh_num, level, ch, (void *)gch, TARGET_CHAR);
		}
	}
}

void spell_mass_invis(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	CHAR_DATA *gch;
	int sn_fog, sn_fire;

	sn_fog = skill_lookup("faerie fog");
	sn_fire = skill_lookup("faerie fire");

	for (gch = ch->in_room->people; gch != nullptr; gch = gch->next_in_room)
	{
		if (!is_same_group(gch, ch) || is_affected_by(gch, AFF_INVISIBLE))
			continue;

		if (is_affected(ch, sn_fog) || is_affected(ch, sn_fire))
		{
			send_to_char("You can't turn invisible while glowing.\n\r", gch);
			continue;
		}

		act("$n slowly fades out of existence.", gch, nullptr, nullptr, TO_ROOM);
		send_to_char("You slowly fade out of existence.\n\r", gch);

		init_affect(&af);
		af.where = TO_AFFECTS;
		af.aftype = get_spell_aftype(ch);
		af.type = sn;
		af.level = level / 2;
		af.duration = 24;
		af.location = APPLY_NONE;
		af.modifier = 0;

		SET_BIT(af.bitvector, AFF_INVISIBLE);

		af.mod_name = MOD_VISIBILITY;
		affect_to_char(gch, &af);
	}

	send_to_char("Ok.\n\r", ch);
}

void spell_null(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	send_to_char("That's not a spell!\n\r", ch);
}

void spell_pass_door(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected_by(victim, AFF_PASS_DOOR))
	{
		if (victim == ch)
			send_to_char("You are already out of phase.\n\r", ch);
		else
			act("$N is already shifted out of phase.", ch, nullptr, victim, TO_CHAR);

		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = get_spell_aftype(ch);
	af.type = sn;
	af.level = level;
	af.duration = number_fuzzy(level / 4);
	af.location = APPLY_NONE;
	af.modifier = 0;

	SET_BIT(af.bitvector, AFF_PASS_DOOR);

	af.mod_name = MOD_PHASE;
	affect_to_char(victim, &af);

	act("$n turns translucent.", victim, nullptr, nullptr, TO_ROOM);
	send_to_char("You turn translucent.\n\r", victim);
}

/* RT plague spell, very nasty */

void spell_plague(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (IS_SET(victim->imm_flags, IMM_DISEASE))
		return;

	if (saves_spell(level, victim, DAM_DISEASE)
		|| (is_npc(victim) && IS_SET(victim->act, ACT_UNDEAD))
		|| is_affected_by(victim, AFF_PLAGUE))
	{
		if (ch == victim)
		{
			act("A sudden fever washes over you, but it passes.", ch, 0, 0, TO_CHAR);
			act("$n looks flushed for a moment, but it passes.", ch, 0, 0, TO_ROOM);
		}
		else
		{
			act("$N resists the disease.", ch, nullptr, victim, TO_CHAR);
			act("A sudden fever washes over you, but it passes.", ch, 0, victim, TO_VICT);
			act("$N looks flushed for a moment, but it passes.", ch, 0, victim, TO_NOTVICT);
		}

		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_MALADY;
	af.type = gsn_plague;
	af.level = level * 3 / 4;
	af.duration = level / 3;
	af.location = APPLY_STR;
	af.modifier = -5;

	SET_BIT(af.bitvector, AFF_PLAGUE);

	af.owner = ch;
	af.tick_fun = plague_tick;
	af.end_fun = nullptr;
	new_affect_join(victim, &af);

	send_to_char("You scream in agony as plague sores erupt from your skin.\n\r", victim);
	act("$n screams in agony as plague sores erupt from $s skin.", victim, nullptr, nullptr, TO_ROOM);
}

void plague_tick(CHAR_DATA *ch, AFFECT_DATA *af)
{
	AFFECT_DATA plague;
	CHAR_DATA *vch;
	int dam;

	if (!ch->in_room)
		return;

	act("$n writhes in agony, consumed by a debilitating plague.", ch, 0, 0, TO_ROOM);
	act("You writhe in agony from the plague.", ch, 0, 0, TO_CHAR);

	if (af->level == 1)
		return;

	init_affect(&plague);
	plague.where = TO_AFFECTS;
	plague.aftype = AFT_MALADY;
	plague.type = gsn_plague;
	plague.owner = af->owner;
	plague.level = af->level - 1;
	plague.duration = af->level / 5;
	plague.location = APPLY_STR;
	plague.modifier = -5;

	SET_BIT(plague.bitvector, AFF_PLAGUE);

	plague.tick_fun = plague_tick;
	plague.end_fun = nullptr;

	for (vch = ch->in_room->people; vch; vch = vch->next_in_room)
	{
		if (!saves_spell(plague.level - 2, vch, DAM_DISEASE)
			&& !is_immortal(vch)
			&& (!is_npc(vch) || !IS_SET(vch->act, ACT_UNDEAD))
			&& !is_affected_by(vch, AFF_PLAGUE) && number_bits(4) == 0)
		{
			act("You feel hot and feverish.", vch, 0, 0, TO_CHAR);
			act("$n shivers and looks very ill.", vch, 0, 0, TO_ROOM);
			new_affect_join(vch, &plague);
		}
	}

	dam = std::min(ch->level, af->level);
	dam = std::max((int)(dam * .7), 10);

	damage_new(af->owner, ch, dam, gsn_plague, DAM_DISEASE, true, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "sickness");

	ch->mana -= number_range(af->level / 2, (int)(af->level * 1.5));
	ch->move -= number_range(af->level / 2, (int)(af->level * 1.5));
}

void spell_poison(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim;
	OBJ_DATA *obj;
	AFFECT_DATA af;

	if (target == TAR_OBJ_INV)
	{
		obj = (OBJ_DATA *)vo;

		if (obj->item_type == ITEM_FOOD || obj->item_type == ITEM_DRINK_CON)
		{
			if (is_obj_stat(obj, ITEM_BLESS) || is_obj_stat(obj, ITEM_BURN_PROOF))
			{
				act("Your spell fails to corrupt $p.", ch, obj, nullptr, TO_CHAR);
				return;
			}

			obj->value[3] = 1;

			act("$p is infused with poisonous vapors.", ch, obj, nullptr, TO_ALL);
			return;
		}

		act("You can't poison $p.", ch, obj, nullptr, TO_CHAR);
		return;
	}

	victim = (CHAR_DATA *)vo;

	if (is_affected(victim, gsn_poison))
	{
		send_to_char("They are already poisoned.\n\r", ch);
		return;
	}

	if (saves_spell(level, victim, DAM_POISON))
	{
		act("$n turns slightly green, but it passes.", victim, nullptr, nullptr, TO_ROOM);
		send_to_char("You feel momentarily ill, but it passes.\n\r", victim);
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = get_spell_aftype(ch);
	af.type = sn;
	af.level = level;
	af.duration = level / 8;
	af.location = APPLY_STR;
	af.modifier = -6;

	SET_BIT(af.bitvector, AFF_POISON);

	af.owner = ch;
	af.end_fun = nullptr;
	af.tick_fun = poison_tick;
	new_affect_to_char(victim, &af);

	send_to_char("You feel very sick.\n\r", victim);
	act("$n looks very ill.", victim, nullptr, nullptr, TO_ROOM);
}

void poison_tick(CHAR_DATA *ch, AFFECT_DATA *af)
{
	if (!is_affected_by(ch, AFF_POISON))
		return;

	act("$n shivers and suffers.", ch, nullptr, nullptr, TO_ROOM);
	send_to_char("You shiver and suffer.\n\r", ch);

	if (!af->owner)
		af->owner = ch;

	damage_new(af->owner, ch, af->level, gsn_poison, DAM_POISON, true, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "poison");
}

void spell_protection(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;

	if (is_affected(ch, skill_lookup("protection")))
	{
		send_to_char("You are already protected.\n\r", ch);
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = get_spell_aftype(ch);
	af.type = sn;
	af.level = level;
	af.duration = level / 2;
	af.modifier = 0;

	SET_BIT(af.bitvector, AFF_PROTECTION);

	af.mod_name = MOD_PROTECTION;
	affect_to_char(ch, &af);

	send_to_char("You feel protected.\n\r", ch);
}

void spell_ray_of_truth(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	int dam;

	if (is_evil(ch))
	{
		victim = ch;
		send_to_char("The energy explodes inside you!\n\r", ch);
	}

	if (victim != ch)
	{
		act("$n raises $s hand, and a blinding ray of light shoots forth!", ch, nullptr, nullptr, TO_ROOM);
		send_to_char("You raise your hand and a blinding ray of light shoots forth!\n\r", ch);
	}

	if (is_good(victim))
	{
		act("$n seems unharmed by the light.", victim, nullptr, victim, TO_ROOM);
		send_to_char("The light seems powerless to affect you.\n\r", victim);
		return;
	}

	dam = dice(level, 5);
	dam += 20;

	if (saves_spell(level, victim, DAM_HOLY))
		dam /= 2;

	if (!is_evil(victim))
		dam /= 2;

	damage_old(ch, victim, dam, sn, DAM_HOLY, true);
	spell_blindness(gsn_blindness, 3 * level / 4, ch, (void *)victim, TARGET_CHAR);
}

void spell_recharge(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	OBJ_DATA *obj = (OBJ_DATA *)vo;
	int chance, percent;

	if (obj->item_type != ITEM_WAND && obj->item_type != ITEM_STAFF)
	{
		send_to_char("That item does not carry charges.\n\r", ch);
		return;
	}

	if (obj->value[3] >= 3 * level / 2)
	{
		send_to_char("Your skills are not great enough for that.\n\r", ch);
		return;
	}

	if (obj->value[1] == 0)
	{
		send_to_char("That item has already been recharged once.\n\r", ch);
		return;
	}

	chance = 40 + 2 * level;
	chance -= obj->value[3]; /* harder to do high-level spells */
	chance -= (obj->value[1] - obj->value[2]) * (obj->value[1] - obj->value[2]);
	chance = std::max(level / 2, chance);

	percent = number_percent();

	if (percent < chance / 2)
	{
		act("$p glows softly.", ch, obj, nullptr, TO_CHAR);
		act("$p glows softly.", ch, obj, nullptr, TO_ROOM);
		obj->value[2] = std::max(obj->value[1], obj->value[2]);
		obj->value[1] = 0;
		return;
	}
	else if (percent <= chance)
	{
		int chargeback, chargemax;

		act("$p glows softly.", ch, obj, nullptr, TO_CHAR);
		act("$p glows softly.", ch, obj, nullptr, TO_CHAR);

		chargemax = obj->value[1] - obj->value[2];

		if (chargemax > 0)
			chargeback = std::max(1, chargemax * percent / 100);
		else
			chargeback = 0;

		obj->value[2] += chargeback;
		obj->value[1] = 0;
		return;
	}
	else if (percent <= std::min(95, (int)(3 * chance / 2)))
	{
		send_to_char("Nothing seems to happen.\n\r", ch);
		if (obj->value[1] > 1)
			obj->value[1]--;
		return;
	}
	else /* whoops! */
	{
		act("$p glows brightly and explodes!", ch, obj, nullptr, TO_CHAR);
		act("$p glows brightly and explodes!", ch, obj, nullptr, TO_ROOM);
		extract_obj(obj);
	}
}

void spell_refresh(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;

	victim->move = std::min(victim->move + level, (int)victim->max_move);

	if (victim->max_move == victim->move)
		send_to_char("You feel fully refreshed!\n\r", victim);
	else
		send_to_char("You feel less tired.\n\r", victim);

	if (ch != victim)
		send_to_char("Ok.\n\r", ch);
}

void spell_remove_curse(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim;
	OBJ_DATA *obj;
	bool found = false;

	/* do object cases first */
	if (target == TARGET_OBJ)
	{
		obj = (OBJ_DATA *)vo;
		if (is_obj_stat(obj, ITEM_NODROP) || is_obj_stat(obj, ITEM_NOREMOVE))
		{
			if (!is_obj_stat(obj, ITEM_NOUNCURSE) && !saves_dispel(level + 2, obj->level, 0))
			{
				REMOVE_BIT(obj->extra_flags, ITEM_NODROP);
				REMOVE_BIT(obj->extra_flags, ITEM_NOREMOVE);
				act("$p glows blue.", ch, obj, nullptr, TO_ALL);
				return;
			}

			act("The curse on $p held.", ch, obj, nullptr, TO_CHAR);
			return;
		}
		act("There doesn't seem to be a curse on $p.", ch, obj, nullptr, TO_CHAR);
		return;
	}

	/* characters */
	victim = (CHAR_DATA *)vo;

	if (is_affected(victim, gsn_curse) && number_percent() > level / 3)
	{
		send_to_char("You feel better.\n\r", victim);
		act("$n looks more relaxed.", victim, nullptr, nullptr, TO_ROOM);
		affect_strip(victim, gsn_curse);
		return;
	}

	if (!trusts(ch, victim) && !is_npc(ch))
	{
		send_to_char("They won't let you do that to them.\n\r", ch);
		return;
	}

	for (obj = victim->carrying; (obj != nullptr && !found); obj = obj->next_content)
	{
		if ((is_obj_stat(obj, ITEM_NODROP) || is_obj_stat(obj, ITEM_NOREMOVE))
			&& !is_obj_stat(obj, ITEM_NOUNCURSE))
		{ /* attempt to remove curse */
			if (!saves_dispel(level, obj->level, 0))
			{
				found = true;
				REMOVE_BIT(obj->extra_flags, ITEM_NODROP);
				REMOVE_BIT(obj->extra_flags, ITEM_NOREMOVE);
				act("Your $p glows blue.", victim, obj, nullptr, TO_CHAR);
				act("$n's $p glows blue.", victim, obj, nullptr, TO_ROOM);
			}
		}
	}
}

void spell_sanctuary(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (ch->Class()->GetIndex() == CLASS_ZEALOT && victim != ch)
	{
		send_to_char("You can't commune that prayer on others.\n\r", ch);
		return;
	}

	if (is_affected_by(ch, AFF_RAGE))
	{
		send_to_char("You cannot concentrate enough to maintain sanctuary while in a rage.\n\r", ch);
		return;
	}

	if (is_affected_by(victim, AFF_SANCTUARY))
	{
		if (victim == ch)
			send_to_char("You are already in sanctuary.\n\r", ch);
		else
			act("$N is already in sanctuary.", ch, nullptr, victim, TO_CHAR);

		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = get_spell_aftype(ch);
	af.type = sn;
	af.level = level;
	af.duration = level / 6;
	af.location = APPLY_NONE;
	af.modifier = 0;

	SET_BIT(af.bitvector, AFF_SANCTUARY);

	af.mod_name = MOD_PROTECTION;

	if (!is_evil(ch))
	{
		act("$n is surrounded by a white aura.", victim, nullptr, nullptr, TO_ROOM);
		send_to_char("You are surrounded by a white aura.\n\r", victim);
	}
	else
	{
		act("$n is surrounded by a dark aura.", victim, 0, 0, TO_ROOM);
		act("You are surrounded by a dark aura.", victim, 0, 0, TO_CHAR);
	}

	affect_to_char(victim, &af);
}

void spell_shield(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected(victim, sn))
	{
		if (victim == ch)
			send_to_char("You are already shielded from harm.\n\r", ch);
		else
			act("$N is already protected by a shield.", ch, nullptr, victim, TO_CHAR);

		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = get_spell_aftype(ch);
	af.type = sn;
	af.level = level;
	af.duration = 8 + level;
	af.location = APPLY_AC;
	af.modifier = level / 4;
	affect_to_char(victim, &af);

	act("$n is surrounded by a force shield.", victim, nullptr, nullptr, TO_ROOM);
	send_to_char("You are surrounded by a force shield.\n\r", victim);
}

void spell_shocking_grasp(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	static const int dam_each[] =
	{
		0,  0,  0,  0,  16, 18, 20, 20, 25, 29, 33, 36, 39, 39, 39, 40, 40,
		41, 41, 42, 42, 43, 43, 44, 44, 45, 45, 46, 46, 47, 47, 48, 48, 49,
		49, 50, 50, 51, 51, 52, 52, 53, 53, 54, 54, 55, 55, 56, 56, 57, 57
	};

	int dam;

	level = std::min(level, (int)sizeof(dam_each) / (int)sizeof(dam_each[0]) - 1);
	level = std::max(0, level);

	dam = number_range(dam_each[level] / 2, dam_each[level] * 2);

	if (saves_spell(level, victim, DAM_LIGHTNING))
		dam /= 2;

	damage_old(ch, victim, dam, sn, DAM_LIGHTNING, true);
}

void spell_sleep(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected_by(victim, AFF_SLEEP)
		|| (is_npc(victim) && IS_SET(victim->act, ACT_UNDEAD))
		|| (level + 2) < victim->level
		|| saves_spell(level - 1, victim, DAM_CHARM)
		|| IS_SET(victim->imm_flags, IMM_SLEEP))
	{
		act("$N resisted your sleep spell.", ch, 0, victim, TO_CHAR);
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = sn;
	af.level = level;
	af.duration = level / 4;
	af.location = APPLY_NONE;
	af.modifier = 0;

	SET_BIT(af.bitvector, AFF_SLEEP);

	af.mod_name = MOD_CONC;
	affect_join(victim, &af);

	if (is_awake(victim))
	{
		send_to_char("You feel very sleepy ..... zzzzzz.\n\r", victim);
		act("$n goes to sleep.", victim, nullptr, nullptr, TO_ROOM);
		victim->position = POS_SLEEPING;
	}
}

void spell_slow(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (ch->Class()->name == "assassin" && victim != ch)
	{
		send_to_char("You are only able to slow your own metabolism.\n\r", ch);
		return;
	}

	if (is_safe(ch, victim))
		return;

	if (is_affected(victim, sn) || is_affected_by(victim, AFF_SLOW))
	{
		if (victim == ch)
			send_to_char("You can't move any slower!\n\r", ch);
		else
			act("$N can't get any slower than that.", ch, nullptr, victim, TO_CHAR);

		return;
	}

	if (is_affected_by(victim, AFF_HASTE))
	{
		affect_strip(victim, skill_lookup("haste"));

		if (is_affected_by(victim, AFF_HASTE))
			REMOVE_BIT(victim->affected_by, AFF_HASTE);

		send_to_char("You start moving at normal speed again.\n\r", victim);
		act("$n starts moving at normal speed again.", victim, nullptr, nullptr, TO_ROOM);
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = sn;
	af.level = level;
	af.duration = level / 4;
	af.location = APPLY_DEX;
	af.modifier = -1 - (level >= 18) - (level >= 25) - (level >= 32);

	SET_BIT(af.bitvector, AFF_SLOW);

	af.mod_name = MOD_SPEED;
	affect_to_char(victim, &af);

	send_to_char("You feel yourself moving more slowly.\n\r", victim);
	act("$n is moving more slowly.", victim, nullptr, nullptr, TO_ROOM);

	if (!trusts(ch, victim))
		multi_hit(victim, ch, TYPE_UNDEFINED);
}

void spell_stone_skin(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected(victim, sn) || is_affected(victim, gsn_diamondskin) || is_affected(victim, gsn_ironskin))
	{
		if (victim == ch)
			send_to_char("Your skin is already magically hardened.\n\r", ch);
		else
			act("$N's skin is already magically hardened.", ch, nullptr, victim, TO_CHAR);

		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = get_spell_aftype(ch);
	af.type = sn;
	af.level = level;
	af.duration = level / 3;
	af.location = APPLY_AC;
	af.modifier = level / 4;
	affect_to_char(victim, &af);

	act("$n's skin turns to stone.", victim, nullptr, nullptr, TO_ROOM);
	send_to_char("Your skin turns to stone.\n\r", victim);
}

void spell_summon(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim;

	victim = get_char_world(ch, target_name);

	if (victim == nullptr
		|| victim == ch
		|| victim->in_room == nullptr
		|| (!is_npc(victim) && victim->in_room->area != ch->in_room->area)
		|| IS_SET(ch->in_room->room_flags, ROOM_SAFE)
		|| IS_SET(ch->in_room->room_flags, ROOM_NO_SUMMON_TO)
		|| ch->in_room->guild != 0
		|| victim->in_room->guild != 0
		|| (is_npc(victim) && is_affected_by(victim, AFF_CHARM) && victim->in_room->area != ch->in_room->area)
		|| IS_SET(victim->in_room->room_flags, ROOM_SAFE)
		|| IS_SET(victim->in_room->room_flags, ROOM_PRIVATE)
		|| IS_SET(victim->in_room->room_flags, ROOM_SOLITARY)
		|| IS_SET(victim->in_room->room_flags, ROOM_NO_SUMMON_FROM)
		|| (is_npc(victim) && IS_SET(victim->act, ACT_AGGRESSIVE))
		|| (is_npc(victim) && victim->level >= (level + 10))
		|| (!is_npc(victim) && victim->level >= LEVEL_IMMORTAL && !is_immortal(ch))
		|| victim->fighting != nullptr
		|| (is_npc(victim) && IS_SET(victim->imm_flags, IMM_SUMMON))
		|| (is_npc(victim) && victim->pIndexData->pShop != nullptr)
		|| (is_npc(victim) && IS_SET(victim->act, ACT_AGGRESSIVE))
		|| (!is_npc(victim) && !can_pk(ch, victim) && IS_SET(victim->act, PLR_NOSUMMON))
		|| !can_see(ch, victim)
		|| is_explore(ch->in_room)
		|| is_explore(victim->in_room)
		|| is_affected(victim, gsn_bind_feet))
	{
		send_to_char("You failed.\n\r", ch);
		return;
	}

	if (is_affected_by(ch, AFF_RAGE))
	{
		send_to_char("You lack the concentration to summon while in a rage.\n\r", ch);
		return;
	}

	if (saves_spell(level + 1, victim, DAM_OTHER))
	{
		act("$N resisted your summoning.", ch, 0, victim, TO_CHAR);

		if (saves_spell(level - 1, victim, DAM_OTHER))
			send_to_char("You feel a strange wrenching sensation, but it passes.\n\r", victim);

		return;
	}

	RS.Queue.AddToQueue(3, 2, summon_char, ch, victim);

	act("You begin the summoning of $N.", ch, 0, victim, TO_CHAR);

	if (saves_spell(level, victim, DAM_OTHER))
		send_to_char("You feel a strange wrenching sensation at the very core of your being.\n\r", victim);
}

void summon_char(CHAR_DATA *ch, CHAR_DATA *victim)
{
	// same area OR 36% chance in adjacent OR 6% chance if they totally got away
	if (is_npc(victim)
		|| ch->in_room->area == victim->in_room->area
		|| (is_adjacent_area(ch->in_room->area, victim->in_room->area) && number_percent() > 70) // 30% chance + 6%
		|| number_percent() < 7)
	{
		act("$n disappears suddenly.", victim, nullptr, nullptr, TO_ROOM);

		char_from_room(victim);
		char_to_room(victim, ch->in_room);

		check_zombie_summon(victim);

		act("$n arrives suddenly.", victim, nullptr, nullptr, TO_ROOM);
		act("$n has summoned you!", ch, nullptr, victim, TO_VICT);

		do_look(victim, "auto");

		check_plasma_thread(victim, -1);

		if (is_npc(victim))
			victim->law_pass = true; // summoned mobs give less xp

		return;
	}

	act("The summoning of $N fails as $E escapes your grasp.", ch, 0, victim, TO_CHAR);
}

/* Modified teleport -- only within radius of one area now. */

void spell_teleport(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	ROOM_INDEX_DATA *pRoomIndex;
	long nocrash; /* Infinite loops = bad mojo */

	if (victim->in_room == nullptr
		|| (!is_npc(ch) && IS_SET(victim->in_room->room_flags, ROOM_NO_RECALL))
		|| (victim != ch && IS_SET(victim->imm_flags, IMM_SUMMON))
		|| (victim != ch && !is_immortal(ch))
		|| (victim->in_room->area->area_type == ARE_UNOPENED)
		|| (is_affected_room(ch->in_room, gsn_plasma_cube) && number_percent() < 50))
	{
		send_to_char("You failed.\n\r", ch);
		return;
	}

	if (is_affected(victim, gsn_bind_feet))
	{
		send_to_char("Your feet are rooted to the ground!\n\r", victim);
		return;
	}

	for (nocrash = 1; nocrash < 10000; nocrash++)
	{
		pRoomIndex = get_random_room(victim);
		if(pRoomIndex == nullptr)
		{
			continue; // this prevents us from evaluating rooms that don't exist due to our
			// lovely method of getting a "random" room.
		}
		if (!IS_SET(pRoomIndex->room_flags, ROOM_NO_RECALL)
			&& !IS_SET(pRoomIndex->room_flags, ROOM_NO_GATE)
			&& !is_explore(pRoomIndex)
			&& !pRoomIndex->cabal
			&& pRoomIndex->area->area_type != ARE_UNOPENED
			&& pRoomIndex->area->area_type != ARE_SHRINE
			&& (is_adjacent_area(ch->in_room->area, pRoomIndex->area) || (ch->in_room->area == pRoomIndex->area)))
		{
			break;
		}
	}

	/*
	 * If there's no valid place to tele to, they stay put.  Better than
	 * an infinite loop -- El
	 */
	if (nocrash >= 1000000)
		pRoomIndex = victim->in_room;

	if (victim != ch)
		send_to_char("You have been teleported!\n\r", victim);

	un_watermeld(victim, nullptr);

	act("$n vanishes!", victim, nullptr, nullptr, TO_ROOM);

	char_from_room(victim);
	char_to_room(victim, pRoomIndex);

	act("$n slowly fades into existence.", victim, nullptr, nullptr, TO_ROOM);

	un_blade_barrier(ch, nullptr);
	un_watermeld(ch, nullptr);

	do_look(victim, "auto");
	check_plasma_thread(victim, -1);
}

/* Modified turn undead spell. Good and neutral clerics attempt to destroy
undead, with good aligns getting a bonus. Evil try to either subdue aggro
undead or control them, depending on the difference in their level.
*/
void spell_turn_undead(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim;
	CHAR_DATA *v_next;
	int dam = 0;
	int value, count, num;
	CHAR_DATA *follower;

	follower = nullptr; /* follower, count and num used for evil clerics */
	count = 0;
	num = 0;

	if (!is_evil(ch))
	{
		act("$n raises $s hands and calls upon the gods to destroy the unholy.", ch, nullptr, nullptr, TO_ROOM);
		act("You raise your hands and call upon the gods to destroy the unholy.", ch, nullptr, nullptr, TO_CHAR);

		if (!is_good(ch))
			level -= 3;
	}
	else
	{
		act("$n turns $s unholy influence upon the room.", ch, 0, 0, TO_ROOM);
		send_to_char("You turn your unholy influence upon those in the room.\n\r", ch);
	}

	/*
	if (!is_npc(ch) && ch->pcdata->special == SPEC_PALADIN_HUNTER)
		level += number_range(2,level/7);
	*/

	if (!is_evil(ch))
	{
		for (victim = ch->in_room->people; victim != nullptr; victim = v_next)
		{
			v_next = victim->next_in_room;

			if (is_same_group(victim, ch) || !IS_SET(victim->act, ACT_UNDEAD))
				continue;

			value = std::max(1, level - victim->level + 10);
			value = std::max(13, value - 4);

			if (is_good(ch))
				dam = dice(level, value);

			if (((ch->level > (victim->level + 15)) && is_npc(victim) && is_good(ch))
				|| ((ch->level > (victim->level + 20)) && is_npc(victim))
				|| (is_good(ch) && is_npc(victim) && number_percent() < 5))
			{
				act("$N crumbles to dust from the power of $n's turning.", ch, nullptr, victim, TO_ROOM);
				act("$N's body crumbles to dust from the power of your turning.", ch, nullptr, victim, TO_CHAR);
				raw_kill(ch, victim);
			}
			else
			{

				damage_old(ch, victim, saves_spell(level, victim, DAM_HOLY) ? dam / 2 : dam, sn, DAM_HOLY, true);
			}
		}

		return;
	}
	else
	{
		for (follower = char_list; follower != nullptr; follower = follower->next)
		{
			if (follower->master == ch && IS_SET(follower->act, ACT_UNDEAD) && follower != ch)
			{
				num++;
				count += follower->level;
			}
		}

		for (victim = ch->in_room->people; victim != nullptr; victim = v_next)
		{
			v_next = victim->next_in_room;

			if (is_same_group(victim, ch) || !IS_SET(victim->act, ACT_UNDEAD))
				continue;

			if (is_affected_by(victim, AFF_CHARM))
				continue;

			if ((ch->level * 3 < count * 2)
				|| (ch->level < 20 && num > 0)
				|| (ch->level < 40 && num > 1)
				|| (ch->level < 51 && num > 2)
				|| (ch->level < (victim->level + 10))
				|| saves_spell(ch->level, victim, DAM_OTHER)
				|| !is_npc(victim))
			{
				act("You attempt to control $N but do not have the influence.", ch, 0, victim, TO_CHAR);
				continue;
			}
			else
			{
				act("$n stares in hatred for a moment then suddenly becomes very subdued.", victim, 0, 0, TO_NOTVICT);
				stop_fighting(victim, true);

				victim->master = ch;
				victim->leader = ch;

				SET_BIT(victim->affected_by, AFF_CHARM);
				REMOVE_BIT(victim->act, ACT_AGGRESSIVE);

				act("$N now follows you.", ch, 0, victim, TO_CHAR);

				num++;
				count += victim->level;
			}
		}
	}
}

void spell_ventriloquate(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	char speaker[MAX_INPUT_LENGTH];
	CHAR_DATA *vch;

	target_name = one_argument(target_name, speaker);

	auto buffer1 = fmt::format("{} says '{}'.\n\r", speaker, target_name);
	auto buffer2 = fmt::format("Someone makes {} say '{}'.\n\r", speaker, target_name);
	buffer1[0] = toupper(buffer1[0]);

	for (vch = ch->in_room->people; vch != nullptr; vch = vch->next_in_room)
	{
		if (!is_exact_name(speaker, vch->name) && is_awake(vch))
			send_to_char(saves_spell(level, vch, DAM_OTHER) ? buffer2.c_str() : buffer1.c_str(), vch);
	}
}

void spell_weaken(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (victim == ch)
	{
		send_to_char("You plan on weakening yourself?\n\r", ch);
		return;
	}

	if (is_affected(victim, sn) || saves_spell(level, victim, DAM_OTHER))
	{
		act("$N's muscles wither slightly then regain their vigor.", ch, 0, victim, TO_CHAR);
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = get_spell_aftype(ch);
	af.type = sn;
	af.level = level;
	af.duration = level / 2;
	af.location = APPLY_STR;
	af.modifier = -1 * (level / 5);
	affect_to_char(victim, &af);

	send_to_char("You feel your strength slip away.\n\r", victim);
	act("$n looks tired and weak.", victim, nullptr, nullptr, TO_ROOM);
}

/* RT recall spell is back */

void spell_word_of_recall(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	ROOM_INDEX_DATA *location;

	if (!trusts(ch, victim))
	{
		send_to_char("They won't allow you to do that to them.\n\r", ch);
		return;
	}

	location = get_room_index(hometown_table[victim->hometown].recall);

	if (location == nullptr)
	{
		location = get_room_index(ROOM_VNUM_TEMPLE);

		if (location == nullptr)
		{
			send_to_char("You are completely lost.\n\r", victim);
			return;
		}
	}

	if ((IS_SET(victim->in_room->room_flags, ROOM_NO_RECALL) && !is_affected_room(ch->in_room, gsn_consecrate))
		|| (is_affected_room(ch->in_room, gsn_plasma_cube) && number_percent() < 50)
		|| is_npc(victim))
	{
		send_to_char("Spell failed.\n\r", victim);
		return;
	}

	if (is_affected(victim, gsn_bind_feet))
	{
		send_to_char("Your feet are rooted to the ground!\n\r", victim);
		return;
	}

	if (ch != victim)
		act("You prepare to send $N home.", ch, 0, victim, TO_CHAR);

	RS.Queue.AddToQueue(6, 2, recall_execute, victim, location);

	send_to_char("A tingling feeling runs down your spine for a moment.\n\r", victim);

	victim->disrupted = false;
}

void recall_execute(CHAR_DATA *ch, ROOM_INDEX_DATA *location)
{
	if (ch->fighting || ch->disrupted)
	{
		send_to_char("The magic of the recall spell dissipates as your focus is disrupted.\n\r", ch);
		return;
	}

	ch->move /= 2;

	if (number_percent() > 40 && is_affected_by(ch, AFF_CURSE))
	{
		send_to_char("The power of the curse laid upon you dissipates the magic of the spell.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_camouflage))
	{
		affect_strip(ch, gsn_camouflage);
		act("$n steps out from $s cover.", ch, 0, 0, TO_ROOM);
		send_to_char("You step out from your cover.\n\r", ch);
	}

	un_watermeld(ch, nullptr);

	act("As the magic takes effect, a black haze clouds your mind.\n\rWhen it passes, you find yourself elsewhere.", ch, 0, 0, TO_CHAR);
	act("$n suddenly disappears.", ch, nullptr, nullptr, TO_ROOM);

	char_from_room(ch);
	char_to_room(ch, location);

	act("$n suddenly appears in the room.", ch, nullptr, nullptr, TO_ROOM);

	do_look(ch, "auto");
	check_plasma_thread(ch, -1);
}

/*
 * NPC spells.
 */
void spell_acid_breath(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	int dam, hp_dam, dice_dam, hpch;

	act("$n spits acid at $N.", ch, nullptr, victim, TO_NOTVICT);
	act("$n spits a stream of corrosive acid at you.", ch, nullptr, victim, TO_VICT);
	act("You spit acid at $N.", ch, nullptr, victim, TO_CHAR);

	hpch = ch->hit / 9;
	dice_dam = dice(level, 4);
	dam = hpch + dice_dam;

	if (dam > ch->hit)
		dam = ch->hit;

	hp_dam = 0;

	/*
	hpch = std::max(12,ch->hit);
	hp_dam = number_range(hpch/11 + 1, hpch/6);
	dice_dam = dice(level,16);

	dam = std::max(hp_dam + dice_dam/10,dice_dam + hp_dam/10);
	*/

	if (saves_spell(level, victim, DAM_ACID))
	{
		acid_effect(victim, level / 2, dam / 4, TARGET_CHAR);
		damage_old(ch, victim, dam / 2, sn, DAM_ACID, true);
	}
	else
	{
		acid_effect(victim, level, dam, TARGET_CHAR);
		damage_old(ch, victim, dam, sn, DAM_ACID, true);
	}
}

void spell_fire_breath(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	CHAR_DATA *vch, *vch_next;
	int dam, hp_dam, dice_dam;
	int hpch;

	act("$n breathes forth a cone of fire.", ch, nullptr, victim, TO_NOTVICT);
	act("$n breathes a cone of hot fire over you!", ch, nullptr, victim, TO_VICT);
	act("You breath forth a cone of fire.", ch, nullptr, nullptr, TO_CHAR);

	hpch = ch->hit / 9;
	dice_dam = dice(level, 5);
	dam = hpch + dice_dam;

	if (dam > ch->hit)
		dam = ch->hit;

	hp_dam = number_range(hpch / 9 + 1, hpch / 5);

	/*
	dice_dam = dice(level,20);

	dam = std::max(hp_dam + dice_dam /10, dice_dam + hp_dam / 10);
	*/

	fire_effect(victim->in_room, level, dam / 2, TARGET_ROOM);

	for (vch = victim->in_room->people; vch != nullptr; vch = vch_next)
	{
		vch_next = vch->next_in_room;
		/*
		if (is_safe_spell(ch,vch,true)
			|| (is_npc(vch) && is_npc(ch) && (ch->fighting != vch || vch->fighting != ch)))
		{
			continue;
		}
		*/

		if (is_safe(ch, vch) && vch->fighting != nullptr && vch->fighting != ch)
			continue;

		if (is_same_group(vch, ch))
			continue;

		if (is_immortal(vch))
			continue;

		if (vch == victim) /* full damage */
		{
			if (saves_spell(level, vch, DAM_FIRE))
			{
				fire_effect(vch, level / 2, dam / 4, TARGET_CHAR);
				damage_old(ch, vch, dam / 2, sn, DAM_FIRE, true);
			}
			else
			{
				fire_effect(vch, level, dam, TARGET_CHAR);
				damage_old(ch, vch, dam, sn, DAM_FIRE, true);
			}
		}
		else /* partial damage */
		{
			if (saves_spell(level - 2, vch, DAM_FIRE))
			{
				fire_effect(vch, level / 4, dam / 8, TARGET_CHAR);
				damage_old(ch, vch, dam / 4, sn, DAM_FIRE, true);
			}
			else
			{
				fire_effect(vch, level / 2, dam / 4, TARGET_CHAR);
				damage_old(ch, vch, dam / 2, sn, DAM_FIRE, true);
			}
		}
	}
}

void spell_frost_breath(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	CHAR_DATA *vch, *vch_next;
	int dam, hp_dam, dice_dam, hpch;

	act("$n breathes out a freezing cone of frost!", ch, nullptr, victim, TO_NOTVICT);
	act("$n breathes a freezing cone of frost over you!", ch, nullptr, victim, TO_VICT);
	act("You breath out a cone of frost.", ch, nullptr, nullptr, TO_CHAR);

	hpch = ch->hit / 9;
	dice_dam = dice(level, 5);
	dam = hpch + dice_dam;

	if (dam > ch->hit)
		dam = ch->hit;

	hp_dam = 0;

	/*
	hpch = std::max(12,ch->hit);
	hp_dam = number_range(hpch/11 + 1, hpch/6);
	dice_dam = dice(level,16);

	dam = std::max(hp_dam + dice_dam/10,dice_dam + hp_dam/10);
	*/

	cold_effect(victim->in_room, level, dam / 2, TARGET_ROOM);

	for (vch = victim->in_room->people; vch != nullptr; vch = vch_next)
	{
		vch_next = vch->next_in_room;
		/*
		if (is_safe_spell(ch,vch,true)
			||  (is_npc(vch) && is_npc(ch) && (ch->fighting != vch || vch->fighting != ch)))
		{
			continue;
		}
		*/

		if ((is_safe(ch, vch) && vch->fighting != nullptr && vch->fighting != ch) || vch == ch)
			continue;

		if (vch == victim) /* full damage */
		{
			if (saves_spell(level, vch, DAM_COLD))
			{
				cold_effect(vch, level / 2, dam / 4, TARGET_CHAR);
				damage_old(ch, vch, dam / 2, sn, DAM_COLD, true);
			}
			else
			{
				cold_effect(vch, level, dam, TARGET_CHAR);
				damage_old(ch, vch, dam, sn, DAM_COLD, true);
			}
		}
		else
		{
			if (saves_spell(level - 2, vch, DAM_COLD))
			{
				cold_effect(vch, level / 4, dam / 8, TARGET_CHAR);
				damage_old(ch, vch, dam / 4, sn, DAM_COLD, true);
			}
			else
			{
				cold_effect(vch, level / 2, dam / 4, TARGET_CHAR);
				damage_old(ch, vch, dam / 2, sn, DAM_COLD, true);
			}
		}
	}
}

void spell_gas_breath(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;
	int dam;

	act("$n breathes out a cloud of poisonous gas!", ch, nullptr, nullptr, TO_ROOM);
	act("You breath out a cloud of poisonous gas.", ch, nullptr, nullptr, TO_CHAR);

	dam = dice(level, 5);

	poison_effect(ch->in_room, level, dam, TARGET_ROOM);

	for (vch = ch->in_room->people; vch != nullptr; vch = vch_next)
	{
		vch_next = vch->next_in_room;

		if (is_safe(ch, vch) && vch->fighting != nullptr && vch->fighting != ch)
			continue;

		if (is_same_group(ch, vch))
			continue;

		if (saves_spell(level, vch, DAM_POISON))
		{
			poison_effect(vch, level / 2, dam / 4, TARGET_CHAR);
			damage_old(ch, vch, dam / 2, sn, DAM_POISON, true);
		}
		else
		{
			poison_effect(vch, level, dam, TARGET_CHAR);
			damage_old(ch, vch, dam, sn, DAM_POISON, true);
		}
	}
}

void spell_nether_breath(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;
	int dam;
	bool saved = true;

	act("$n opens $s mouth and unleashes a blast of pure darkness at you!", ch, 0, victim, TO_VICT);
	act("$n opens $s mouth and unleashes a blast of pure darkness at $N!", ch, 0, victim, TO_NOTVICT);

	dam = std::max(100, ch->hit / 10);

	if (saves_spell(level, victim, DAM_NEGATIVE))
		dam /= 2;
	else
		saved = false;

	damage_old(ch, victim, dam, sn, DAM_NEGATIVE, true);

	if (!saved && !is_affected(victim, gsn_curse))
	{
		init_affect(&af);
		af.where = TO_AFFECTS;
		af.type = gsn_curse;
		af.aftype = AFT_SPELL;
		af.location = APPLY_SAVES;
		af.modifier = level / 3;
		af.duration = 12;
		af.level = level;
		af.owner = ch;

		SET_BIT(af.bitvector, AFF_CURSE);

		affect_to_char(victim, &af);

		send_to_char("You feel unclean.\n\r", victim);
		act("$n looks very uncomfortable.", victim, 0, 0, TO_ROOM);
	}
}

void spell_lightning_breath(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	int dam, hp_dam, dice_dam, hpch;

	act("$n breathes a bolt of lightning at $N.", ch, nullptr, victim, TO_NOTVICT);
	act("$n breathes a bolt of lightning at you!", ch, nullptr, victim, TO_VICT);
	act("You breathe a bolt of lightning at $N.", ch, nullptr, victim, TO_CHAR);

	hpch = ch->hit / 9;
	dice_dam = dice(level, 5);
	dam = hpch + dice_dam;

	if (dam > ch->hit)
		dam = ch->hit;

	hp_dam = 0;

	/*
		hpch = std::max(10,ch->hit);
		hp_dam = number_range(hpch/9+1,hpch/5);
		dice_dam = dice(level,20);

		dam = std::max(hp_dam + dice_dam/10,dice_dam + hp_dam/10);
	*/

	if (saves_spell(level, victim, DAM_LIGHTNING))
	{
		shock_effect(victim, level / 2, dam / 4, TARGET_CHAR);
		damage_old(ch, victim, dam / 2, sn, DAM_LIGHTNING, true);
	}
	else
	{
		shock_effect(victim, level, dam, TARGET_CHAR);
		damage_old(ch, victim, dam, sn, DAM_LIGHTNING, true);
	}
}

void spell_iceball(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;
	char buf[MAX_STRING_LENGTH];

	int dam = dice(level, 6);

	for (vch = ch->in_room->people; vch != nullptr; vch = vch_next)
	{
		vch_next = vch->next_in_room;

		if (is_same_group(vch, ch))
			continue;

		if (ch->cabal > 0 && ch->cabal == vch->cabal)
			continue;

		if (is_safe(ch, vch))
			continue;

		if (!is_npc(ch) && !is_npc(vch) && (ch->fighting == nullptr || vch->fighting == nullptr))
		{
			switch (number_range(0, 2))
			{
				case 0:
				case 1:
					sprintf(buf, "Die, %s you sorcerous dog!", pers(ch, vch));
					break;
				case 2:
					sprintf(buf, "Help! %s is casting a spell on me!", pers(ch, vch));
			}

			if (vch != ch)
				do_myell(vch, buf, ch);
		}

		damage_old(ch, vch, (saves_spell(level, vch, DAM_COLD) ? dam / 2 : dam), sn, DAM_COLD, true);
	}
}

void spell_protective_shield(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;

	if (is_affected(ch, sn))
	{
		send_to_char("You are already protected by a protective shield.\n\r", ch);
		return;
	}

	if (is_affected_by(ch, AFF_RAGE))
	{
		send_to_char("You cannot concentrate enough to maintain a protective shield while in a rage.\n\r", ch);
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = get_spell_aftype(ch);
	af.location = 0;
	af.modifier = 0;
	af.duration = 5 + level / 5;
	af.type = sn;
	af.level = level;
	af.mod_name = MOD_PROTECTION;
	affect_to_char(ch, &af);

	act("$n is surrounded by a protective shield.", ch, 0, 0, TO_ROOM);
	send_to_char("You are surrounded by a protective shield.\n\r", ch);
}

int check_sanguine(CHAR_DATA *ch)
{
	AFFECT_DATA *paf = affect_find(ch->affected, gsn_sanguine_ward);

	if (!paf)
		return 0;

	return paf->modifier;
}

void sanguine_blind(CHAR_DATA *ch, CHAR_DATA *victim)
{
	AFFECT_DATA af;
	init_affect(&af);

	if (is_affected(victim, gsn_blindness))
		return;

	act("You force your way through $n's undulating ward of blood to strike at $m.", ch, 0, victim, TO_VICT);
	act("$N struggles through $n's ward to strike at $m before stumbling away clawing at his eyes!", ch, 0, victim, TO_NOTVICT);
	act("$N struggles through your ward to strike at you before stumbling away clawing at his eyes!", ch, 0, victim, TO_CHAR);
	send_to_char("Your vision fades to blackness as the coagulated blood flows hungrily over your eyes!\n\r", victim);

	af.where = TO_AFFECTS;
	af.aftype = AFT_MALADY;
	af.type = gsn_blindness;
	af.name = palloc_string("infected eyes");
	af.level = ch->level;
	af.owner = ch;
	af.duration = number_percent() > 50 ? 1 : 0;
	af.location = 0;
	af.modifier = 0;
	af.mod_name = MOD_VISION;

	SET_BIT(af.bitvector, AFF_BLIND);

	affect_to_char(victim, &af);
}

void spell_sanguine_ward(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;

	int charges = 1;

	if (target_name[0] != '\0' && is_number(target_name))
		charges = atoi(target_name);

	charges = URANGE(1, charges, 5);

	if (!drain_urn(ch, charges))
		return;

	if (is_affected(ch, sn))
	{
		send_to_char("You are already warded.\n\r", ch);
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.location = 0;
	af.modifier = charges;
	af.duration = level / 5 + std::min(charges * 2, 7);
	af.type = sn;
	af.level = level;
	af.mod_name = MOD_PROTECTION;
	affect_to_char(ch, &af);

	send_to_char("Using the blood from your urn, you trace a symbol upon your forehead.\n\r", ch);
	send_to_char("A translucent red shield flickers into existence around you.\n\r", ch);

	act("$n dips a finger into $s urn and traces a symbol onto $s forehead.", ch, 0, 0, TO_ROOM);
	act("A translucent red shield flickers into existence around $n.", ch, 0, 0, TO_ROOM);

	if (charges > 2 && charges != 5)
	{
		send_to_char("The strong ward seems nearly solid for a moment before fading back to translucence.\n\r", ch);
		act("The ward around $n seems nearly solid for a moment before fading back to translucence.", ch, 0, 0, TO_ROOM);
	}

	if (charges == 5)
	{
		send_to_char("Your surroundings seem to blur momentarily as the powerful ward coalesces around you.\n\r", ch);
		act("$n seems to flicker out of existence momentarily as the powerful ward coalesces around $m.", ch, 0, 0, TO_ROOM);
	}

	send_to_char("\n\r", ch);
}

void spell_consecrate(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	ROOM_INDEX_DATA *in_room;
	ROOM_INDEX_DATA *room_check;
	int count, door;
	EXIT_DATA *pexit;
	ROOM_AFFECT_DATA raf;
	AFFECT_DATA af;
	int chance;

	if (is_affected(ch, sn))
	{
		send_to_char("You do not feel up to purging a room of evil yet.\n\r", ch);
		return;
	}

	if (ch->in_room->vnum == 1212
		|| IS_SET(ch->in_room->room_flags, ROOM_NO_CONSECRATE)
		|| IS_SET(ch->in_room->room_flags, ROOM_FORCE_DUEL))
	{
		send_to_char("Spell failed.\n\r", ch);
		return;
	}

	if (is_affected_room(ch->in_room, gsn_consecrate))
	{
		send_to_char("This room is already consecrated.\n\r", ch);
		return;
	}

	if (!IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL))
	{
		send_to_char("This room is not in need of holy blessing.\n\r", ch);
		return;
	}

	chance = ch->level + (ch->pcdata->learned[sn] / 2);

	in_room = ch->in_room;
	count = 0;

	for (door = 0; door < 6; door++)
	{
		if ((pexit = in_room->exit[door]) == nullptr || (room_check = pexit->u1.to_room) == nullptr)
			continue;

		if (room_check == in_room)
			continue;

		if (IS_SET(room_check->room_flags, ROOM_NO_RECALL))
			count++;
	}

	chance -= count * 10;

	act("$n gestures around the room and utters a blessing of holy rights.", ch, 0, 0, TO_ROOM);
	send_to_char("You chant a blessing and give the divine somatic motions of consecration.\n\r", ch);

	if (number_percent() > chance)
	{
		act("You feel the atmosphere lighten for a moment but it passes.", ch, 0, 0, TO_ROOM);
		send_to_char("Your holy rights are invoked but fail to affect the room's evil.\n\r", ch);
		return;
	}

	act("The atmosphere in the room lightens.", ch, 0, 0, TO_ROOM);
	send_to_char("You succeed in consecrating the ground for a brief period!\n\r", ch);

	init_affect_room(&raf);
	raf.where = TO_ROOM_AFFECTS;
	raf.aftype = AFT_COMMUNE;
	raf.type = sn;
	raf.level = level;
	raf.duration = number_range(4, 6);
	raf.location = APPLY_NONE;
	raf.modifier = 0;
	raf.owner = ch;
	raf.end_fun = nullptr;
	raf.tick_fun = nullptr;
	new_affect_to_room(ch->in_room, &raf);

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_COMMUNE;
	af.type = sn;
	af.location = 0;
	af.modifier = 0;
	af.level = level;
	af.duration = 15;
	affect_to_char(ch, &af);
}

void spell_safety(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *gch;
	CHAR_DATA *gch_next;
	ROOM_INDEX_DATA *pRoomIndex;

	pRoomIndex = get_room_index(hometown_table[ch->hometown].recall);

	if (pRoomIndex == nullptr)
		pRoomIndex = get_room_index(ROOM_VNUM_TEMPLE);

	for (gch = ch->in_room->people; gch != nullptr; gch = gch_next)
	{
		gch_next = gch->next_in_room;

		if (is_npc(gch))
			continue;

		if (!is_same_group(ch, gch))
			continue;

		if (IS_SET(gch->in_room->room_flags, ROOM_NO_RECALL)
			|| is_affected_by(gch, AFF_CURSE)
			|| (ch->in_room->cabal != 0 && ch->cabal != ch->in_room->cabal))
		{
			send_to_char("Spell failed.\n\r", gch);
		}
		else
		{
			act("$n suddenly vanishes!", gch, 0, 0, TO_ROOM);
		}
	}

	for (gch = ch->in_room->people; gch != nullptr; gch = gch_next)
	{
		gch_next = gch->next_in_room;

		if (!is_same_group(ch, gch))
			continue;

		if (IS_SET(gch->in_room->room_flags, ROOM_NO_RECALL) || is_affected_by(gch, AFF_CURSE))
			continue;

		if (gch->fighting != nullptr)
			stop_fighting(gch, true);

		char_from_room(gch);
		char_to_room(gch, pRoomIndex);
		act("$n suddenly appears in the room.", gch, 0, 0, TO_ROOM);
		do_look(gch, "auto");
	}

	ch->move /= 2;
}

void spell_blade_barrier(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;

	if (is_affected(ch, gsn_blade_barrier))
	{
		send_to_char("You are already in a blade barrier.\n\r", ch);
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = gsn_blade_barrier;
	af.duration = 12;
	af.location = APPLY_NONE;
	af.modifier = 0;
	af.aftype = AFT_COMMUNE;
	af.level = ch->level;
	af.mod_name = MOD_PROTECTION;
	affect_to_char(ch, &af);

	send_to_char("A barrier of deadly whirling blades comes into existence around you.\n\r", ch);
	act("A barrier of deadly whirling blades comes into existence around $n.", ch, 0, 0, TO_ROOM);

	WAIT_STATE(ch, PULSE_VIOLENCE * 2);
}

void spell_old_blade_barrier(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	int dam, spins;
	int chance, i;

	chance = 100;

	dam = dice(level, 3);
	spins = number_range(2, 3);
	dam *= 9;
	dam /= 10;

	if (number_percent() > chance || victim == ch)
	{
		act("$n creates a whirlwind of spinning blades which turn and strike $m down!", ch, 0, 0, TO_ROOM);
		act("Your blade barrier turns and strikes you down!", ch, 0, 0, TO_CHAR);

		dam /= 2;

		damage_old(ch, ch, dam, sn, DAM_SLASH, true);

		for (i = 0; i < spins; i++)
		{
			if (ch->ghost > 0)
				return;

			if (saves_spell(level, ch, DAM_SLASH))
				dam /= 2;

			dam *= 3;
			dam /= 2;

			act("The blades spin and slice away at $n.", ch, 0, 0, TO_ROOM);
			act("The blades spin and slice away at you.", ch, 0, 0, TO_CHAR);
			damage_old(ch, ch, dam, sn, DAM_SLASH, true);
		}
	}
	else
	{
		act("You create a whirlwind of spinning blades to strike down $N!", ch, 0, victim, TO_CHAR);
		act("$n creates a deadly blade barrier that tears into $N!", ch, 0, victim, TO_NOTVICT);
		act("$n creates a deadly blade barrier that tears into you!", ch, 0, victim, TO_VICT);
		damage_old(ch, victim, dam, sn, DAM_SLASH, true);

		for (i = 0; i < spins; i++)
		{
			if (victim->ghost > 0 || (victim->in_room != ch->in_room))
				return;

			if (saves_spell(level, victim, DAM_SLASH))
				dam /= 2;

			dam *= 3;
			dam /= 4;
			act("The blades spin and slice away at $n.", victim, 0, 0, TO_ROOM);
			act("The blades spin and slice away at you.", victim, 0, 0, TO_CHAR);

			if (victim->ghost > 0)
				return;

			damage_old(ch, victim, dam, sn, DAM_SLASH, true);
		}
	}

	act("The blade barrier fades away.", ch, 0, 0, TO_ROOM);
	act("Your blade barrier fades away.", ch, 0, 0, TO_CHAR);
}

void spell_holy_fire(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	int dam;
	int chance, dam_mod;

	chance = 100;
	dam_mod = 10;

	if (is_good(ch) && is_good(victim))
	{
		dam_mod = 5;
		chance = 0;
	}
	else if (is_evil(ch))
	{
		dam_mod = 7;
		chance = 0;
	}
	else if (is_good(ch) && is_neutral(victim))
	{
		dam_mod = 8;
	}
	else if (is_neutral(ch))
	{
		dam_mod = 6;
		chance = 70;
	}

	dam = dice(level, 7);

	if (number_percent() > chance)
	{
		act("$n's heavenly fire turns on $m!", ch, 0, 0, TO_ROOM);
		act("Your heavenly fire turns on you for your sins!", ch, 0, 0, TO_CHAR);

		dam *= dam_mod;
		dam /= 10;

		if (saves_spell(level, ch, DAM_HOLY))
			dam /= 2;

		damage_old(ch, ch, dam, sn, DAM_HOLY, true);
		return;
	}

	act("$n calls down fire from the heavens!", ch, 0, 0, TO_ROOM);
	act("You call down fire from the heavens!", ch, 0, 0, TO_CHAR);

	dam *= dam_mod;
	dam /= 10;

	damage_old(ch, victim, dam, sn, DAM_HOLY, true);
}

void spell_talk_to_dead(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	OBJ_DATA *corpse;
	char *obj_name;
	char buf[MAX_STRING_LENGTH];

	if (target_name[0] == '\0')
	{
		send_to_char("Attempt to commune with which corpse?\n\r", ch);
		return;
	}

	obj_name = palloc_string(target_name);
	corpse = get_obj_here(ch, obj_name);

	if (corpse == nullptr)
	{
		send_to_char("You can't find that here.\n\r", ch);
		return;
	}

	if ((corpse->item_type != ITEM_CORPSE_PC) && (corpse->item_type != ITEM_CORPSE_NPC))
	{
		send_to_char("That object is not a corpse you can commune with.\n\r", ch);
		return;
	}

	if (IS_SET(corpse->extra_flags, CORPSE_NO_ANIMATE))
	{
		act("$p shudders slightly and a soft moan reaches your ears before it goes silent.", ch, corpse, 0, TO_CHAR);
		return;
	}

	act("You draw back the spirit of the fallen and $p speaks briefly to you.", ch, corpse, 0, TO_CHAR);

	sprintf(buf, "The spirit tells you 'I was slain by %s, now allow me my peace.'\n\r", corpse->talked);
	send_to_char(buf, ch);

	act("A faint glow surrounds $p and then fades away.", ch, corpse, 0, TO_ROOM);
}

/* New energy drain spell due to anti-paladin's always complaining how
useless the old one was.
-Ceran
*/
void spell_energy_drain(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	int amount;

	if (victim == ch)
	{
		send_to_char("You can't drain your own life force.\n\r", ch);
		return;
	}

	if (saves_spell(level - 3, victim, DAM_NEGATIVE))
	{
		send_to_char("You feel a cold chill wash over you, but it passes.\n\r", victim);
		act("You were unable to drain energy from $N.", ch, 0, victim, TO_CHAR);
		return;
	}

	amount = dice(level / 3, 2);

	act("You drain $N's life away!", ch, 0, victim, TO_CHAR);
	act("$n drains $N's life away!", ch, 0, victim, TO_NOTVICT);
	act("You feel your life slipping away!", ch, 0, victim, TO_VICT);

	damage_old(ch, victim, amount, sn, DAM_NEGATIVE, true);

	ch->hit += std::min(amount, 20);

	victim->mana /= 2;
	victim->move /= 2;
}

void spell_dark_shield(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected(victim, sn))
	{
		if (victim == ch)
			send_to_char("You are already surrounded by a dark shield.\n\r", ch);
		else
			send_to_char("They are already surrounded by a dark shield.\n\r", ch);

		return;
	}

	if (is_good(victim))
	{
		act("$N is too pure for such a spell.", ch, 0, victim, TO_CHAR);
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = sn;
	af.level = level;
	af.duration = (is_evil(victim) ? level : level / 2);
	af.modifier = level / 4;
	af.location = APPLY_AC;
	affect_to_char(victim, &af);

	act("$n is surrounded by a darkened shield.", victim, 0, 0, TO_ROOM);
	send_to_char("You are surrounded by a darkened shield.\n\r", victim);
}

int check_spellcraft(CHAR_DATA *ch, int sn)
{
	float bonus;

	bonus = (get_skill(ch, gsn_spellcraft) - 50) / 5;
	bonus -= number_range(0, 5);

	if (bonus <= 0 || ch->level < skill_table[gsn_spellcraft].skill_level[ch->Class()->GetIndex()])
		return 0;

	/*
	 * Can make exemptions for specific spells, as in the nonsense example of
	 * bash-casting below.
	 */

	if (sn == gsn_bash)
		bonus = 0;
	else
		bonus /= 0.5 * (27 - get_curr_stat(ch, STAT_INT));

	check_improve(ch, gsn_spellcraft, true, 8);

	return (int)bonus;
}

void spell_deathspell(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;
	int klevel, dam, bonus;
	char buf[MAX_STRING_LENGTH];
	klevel = level - 7;

	act("$n utters a word of power and the negative energy explodes in the room!", ch, 0, 0, TO_ROOM);
	send_to_char("You utter a word of power and negative energy explodes into the room!\n\r", ch);

	bonus = 0;
	dam = dice(klevel, 4);

	for (vch = ch->in_room->people; vch != nullptr; vch = vch_next)
	{
		vch_next = vch->next_in_room;

		if (is_safe(ch, vch))
			continue;

		if (!is_npc(ch) && !is_npc(vch) && (ch->fighting == nullptr || vch->fighting == nullptr))
		{
			switch (number_range(0, 2))
			{
				case 0:
				case 1:
					sprintf(buf, "Die, %s you sorcerous dog!", pers(ch, vch));
					break;
				case 2:
					sprintf(buf, "Help! %s is casting a spell on me!", pers(ch, vch));
			}

			if (vch != ch)
				do_myell(vch, buf, ch);
		}

		if (vch->level > klevel || IS_SET(vch->act, ACT_UNDEAD) || IS_SET(vch->imm_flags, IMM_NEGATIVE))
		{
			act("$n is unaffected by the negative energy field.", vch, 0, 0, TO_ROOM);
			act("You are unaffected by the negative energy field.", vch, 0, 0, TO_CHAR);
		}
		else if (vch == ch)
		{
			bonus = 6;
		}
		else
		{
			bonus = 0;
		}

		if (saves_spell(klevel - bonus, vch, DAM_NEGATIVE) || saves_spell(klevel, vch, DAM_NEGATIVE))
		{
			if (saves_spell(level - bonus, vch, DAM_NEGATIVE))
				damage_old(ch, vch, dam / 2, sn, DAM_NEGATIVE, true);
			else
				damage_old(ch, vch, dam, sn, DAM_NEGATIVE, true);
		}
		else
		{
			act("$n gets a horrible look in $s eye's then keels over dead!", vch, 0, 0, TO_ROOM);
			send_to_char("You feel your an intense pain in your head as the energy ruptures your skull!\n\r", vch);
			raw_kill(ch, vch);
		}
	}
}

void spell_lifebane(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;
	char buf[MAX_STRING_LENGTH];

	int sn_poison, sn_curse, sn_weaken;
	int dam, tmp_dam;
	bool inside = false;

	sn_poison = skill_lookup("poison");
	sn_curse = skill_lookup("curse");
	sn_weaken = skill_lookup("weaken");

	if (IS_SET(ch->in_room->room_flags, ROOM_INDOORS) || ch->in_room->sector_type == SECT_INSIDE)
		inside = true;

	dam = dice(level, 4);

	for (vch = ch->in_room->people; vch != nullptr; vch = vch_next)
	{
		vch_next = vch->next_in_room;

		if (is_safe(ch, vch))
			continue;

		if (!(inside) && is_same_group(ch, vch))
			continue;

		tmp_dam = dam;

		if (vch == ch)
			tmp_dam /= 2;

		if (!is_npc(ch) && !is_npc(vch) && (ch->fighting == nullptr || vch->fighting == nullptr))
		{
			switch (number_range(0, 2))
			{
				case 0:
				case 1:
					sprintf(buf, "Die, %s you sorcerous dog!", pers(ch, vch));
					break;
				case 2:
					sprintf(buf, "Help! %s is casting a spell on me!", pers(ch, vch));
			}

			if (vch != ch)
				do_myell(vch, buf, ch);
		}

		if (saves_spell(level, vch, DAM_NEGATIVE))
			damage_old(ch, vch, dam / 2, sn, DAM_NEGATIVE, true);
		else
			damage_old(ch, vch, dam, sn, DAM_NEGATIVE, true);

		spell_poison(sn_poison, level - 5, ch, vch, target);
		spell_weaken(sn_weaken, level - 5, ch, vch, target);
		spell_curse(sn_curse, level - 8, ch, vch, target);
	}
}

/* Green tower obj progs */

/* sceptre of heavens...say 'i am the wrath of god' */
void spell_heavenly_sceptre_frenzy(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	if (is_affected(ch, sn) || (!is_good(ch) && !is_evil(ch)))
	{
		act("$n's sceptre hums softly but nothing seems to happen.", ch, 0, 0, TO_ROOM);
		act("Your sceptre hums softly but nothing seems to happen.", ch, 0, 0, TO_CHAR);
		return;
	}

	af.type = sn;

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.duration = 10;
	af.modifier = ch->level / 7;
	af.location = APPLY_DAMROLL;
	af.level = level;
	affect_to_char(ch, &af);

	af.location = APPLY_HITROLL;
	affect_to_char(ch, &af);

	if (ch->level > 40)
		af.location = APPLY_SAVING_SPELL;

	af.modifier = -ch->level / 10;

	if (is_good(ch))
	{
		act("$n's sceptre glows white and a look of righteous anger in $s eyes.", ch, 0, 0, TO_ROOM);
		send_to_char("Your sceptre glows white and you feel a surge of righteous anger!\n\r", ch);
	}
	else if (is_evil(ch))
	{
		act("$n's sceptre darkens and a look of righteous anger enters $s eyes.", ch, 0, 0, TO_ROOM);
		send_to_char("Your sceptre darkens and you feel a surge of righteous anger!\n\r", ch);
	}
}

/* sceptre of heavens, say 'Feel the force of god' */
void spell_heavenly_sceptre_fire(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	OBJ_DATA *sceptre = nullptr;
	int dam;
	CHAR_DATA *victim;

	victim = ch->fighting;

	if (is_affected(ch, sn) || victim == nullptr || ch->level < 30)
	{
		act("$n's sceptre hums softly but nothing seems to happen.", ch, 0, 0, TO_ROOM);
		send_to_char("Your sceptre hums softly but nothing seems to happen.\n\r", ch);
		WAIT_STATE(ch, 12);
		return;
	}

	dam = dice(15, 20);

	if (saves_spell(ch->level, victim, DAM_HOLY))
		dam /= 2;

	if (saves_spell(65, victim, DAM_OTHER))
		dam /= 2;

	damage_old(ch, victim, dam, sn, DAM_HOLY, true);

	if (is_npc(ch))
		return;

	if (number_percent() > ch->level * 2)
	{
		act("Your sceptre of heavenly orders crumbles to dust.", ch, 0, 0, TO_ROOM);
		send_to_char("Your sceptre of heavenly orders crumbles to dust.\n\r", ch);
		extract_obj(sceptre);
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = sn;
	af.modifier = 0;
	af.duration = 1;
	af.location = 0;
	af.level = ch->level;
	affect_to_char(ch, &af);
}

void spell_lightshield(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (!is_good(victim))
	{
		if (victim == ch)
			send_to_char("You are not pure enough to receive the gift of light.\n\r", ch);
		else
			act("$N is not pure enough to receive the gift of light.", ch, nullptr, victim, TO_CHAR);

		return;
	}

	if (is_affected(victim, sn))
	{
		if (victim == ch)
			send_to_char("You are already protected.\n\r", ch);
		else
			act("$N is already protected.", ch, nullptr, victim, TO_CHAR);

		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = sn;
	af.level = level;
	af.duration = level;
	af.modifier = level / 4;
	af.location = APPLY_AC;
	affect_to_char(victim, &af);

	af.location = APPLY_SAVING_SPELL;
	af.modifier = -level / 10;

	SET_BIT(af.bitvector, AFF_DARK_VISION);

	affect_to_char(victim, &af);

	send_to_char("You are surrounded by a glowing afflatus of purity.\n\r", victim);

	if (ch != victim)
		act("$N is surrounded by a glowing afflatus of purity.", ch, nullptr, victim, TO_CHAR);
}

void spell_forget(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	CHAR_DATA *victim = (CHAR_DATA *)vo;

	if (is_affected(victim, sn))
	{
		if (victim == ch)
			send_to_char("You are already as senile as you can get.\n\r", ch);
		else
			send_to_char("They are already affected by a forget spell.\n\r", ch);

		return;
	}

	if (saves_spell(level - 5, victim, DAM_OTHER))
	{
		send_to_char("Spell failed.\n\r", ch);
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = sn;
	af.modifier = 0;
	af.owner = ch;
	af.duration = level / 10;
	af.location = 0;
	af.level = level;
	af.mod_name = MOD_CONC;
	affect_to_char(victim, &af);

	send_to_char("You feel your memories slip away.\n\r", victim);
	act("$n suddenly looks disoriented.", victim, 0, 0, TO_ROOM);
}

void spell_earthbind(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	CHAR_DATA *victim = (CHAR_DATA *)vo;

	if (is_affected(victim, sn) || !is_affected_by(victim, AFF_FLYING))
	{
		send_to_char("Their feet are already on the ground.\n\r", ch);
		return;
	}

	if (saves_spell(level, victim, DAM_OTHER))
	{
		send_to_char("Spell failed.\n\r", ch);
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = gsn_earthbind;
	af.modifier = 0;
	af.level = level;
	af.duration = level / 5;
	af.location = 0;
	af.mod_name = MOD_LEVITATION;
	affect_to_char(victim, &af);

	send_to_char("Your feet suddenly become heavy and earthbound.\n\r", victim);
	act("$n suddenly drops to the ground.", victim, 0, 0, TO_ROOM);
}

void spell_cremate(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	int dam;

	dam = dice(level, 7);

	act("$n is engulfed in raging fire!", victim, 0, 0, TO_ROOM);
	send_to_char("You are engulfed in raging fire!\n\r", victim);

	if (saves_spell(level, victim, DAM_FIRE))
		dam /= 2;

	damage_old(ch, victim, dam, sn, DAM_FIRE, true);
}

void spell_divine_touch(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;

	if (is_affected(ch, sn))
	{
		send_to_char("You are already blessed with a divine touch.\n\r", ch);
		return;
	}

	af.type = sn;
	af.modifier = 0;
	af.location = 0;
	af.duration = level / 5;
	af.level = level;
	affect_to_char(ch, &af);

	send_to_char("Your hands are surrounded in holy energy.\n\r", ch);
	act("$n's hands seem to glow with an inner energy.", ch, 0, 0, TO_ROOM);
}

void spell_transfer_object(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim;
	OBJ_DATA *obj;
	char arg1[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];

	target_name = one_argument(target_name, arg1);
	one_argument(target_name, arg2);

	if (arg1[0] == '\0' || arg2[0] == '\0')
	{
		send_to_char("Send what to whom?\n\r", ch);
		return;
	}

	if ((obj = get_obj_carry(ch, arg1, ch)) == nullptr)
	{
		send_to_char("You are not carrying that.\n\r", ch);
		return;
	}

	victim = get_char_world(ch, arg2);

	if (victim == nullptr)
	{
		send_to_char("They aren't here.\n\r", ch);
		return;
	}

	if (is_npc(victim))
	{
		send_to_char("They wouldn't want that sent to them.\n\r", ch);
		return;
	}

	if (victim == ch)
	{
		send_to_char("Why not just hold it in your other hand?\n\r", ch);
		return;
	}

	if (is_immortal(victim) && !is_immortal(ch))
	{
		send_to_char("You can't send to an immortal.\n\r", ch);
		return;
	}

	if (IS_SET(obj->extra_flags, ITEM_NODROP))
	{
		send_to_char("You can't seem to remove it from yourself to prepare for transfer.\n\r", ch);
		return;
	}

	if (victim->carry_number >= can_carry_n(victim)
		|| victim->carry_weight >= can_carry_w(victim)
		|| isCabalItem(obj))
	{
		act("$p glows softly but nothing happens.", ch, obj, 0, TO_CHAR);
		act("$p glows softly but nothing happens.", ch, obj, 0, TO_ROOM);
		return;
	}

	if (IS_SET(victim->act, PLR_NO_TRANSFER))
	{
		send_to_char("They wouldn't want them sent to them.\n\r", ch);
		return;
	}

	act("$p glows softly and vanishes!", ch, obj, 0, TO_CHAR);
	act("$p glows softly and vanishes!", ch, obj, 0, TO_ROOM);

	obj_from_char(obj);
	obj_to_char(obj, victim);

	act("You suddenly feel heavier as $p pops into your inventory!", victim, obj, 0, TO_CHAR);
	act("$p suddenly appears from nowhere and pops into $n's possession!", victim, obj, 0, TO_ROOM);
}

/* Necros use this to keep body parts longer...for lesser golems */
void spell_preserve(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	OBJ_DATA *obj = (OBJ_DATA *)vo;
	int vnum, chance;

	vnum = obj->pIndexData->vnum;

	if (vnum != OBJ_VNUM_SEVERED_HEAD
		&& vnum != OBJ_VNUM_TORN_HEART
		&& vnum != OBJ_VNUM_SLICED_ARM
		&& vnum != OBJ_VNUM_SLICED_LEG
		&& vnum != OBJ_VNUM_GUTS
		&& vnum != OBJ_VNUM_BRAINS)
	{
		send_to_char("You can't preserve that.\n\r", ch);
		return;
	}

	if (obj->timer > 10)
	{
		send_to_char("It's already in very well preserved condition.\n\r", ch);
		return;
	}

	chance = get_skill(ch, sn);
	chance *= 90;
	chance /= 100;

	if (number_percent() > chance)
	{
		act("$n destroys $p.", ch, obj, nullptr, TO_ROOM);
		act("You fail and destroy $p.", ch, obj, nullptr, TO_CHAR);
		extract_obj(obj);
		return;
	}

	act("You surround $p with necromantic magic to slow its decay.", ch, obj, nullptr, TO_CHAR);
	obj->timer += number_range(level / 2, level);
}

/* Make a victim flee...if they really fail their saves big time it can
kill them outright..(Ceran)
*/
void spell_power_word_fear(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;
	bool bad_fail, utter_fail;
	int range;

	bad_fail = false;
	utter_fail = false;

	if (victim == ch)
	{
		send_to_char("That wouldn't work.\n\r", ch);
		return;
	}

	act("$n points at $N and utters the word 'Fear!'", ch, 0, victim, TO_NOTVICT);
	act("$n points at you and utters the word 'Fear!'", ch, 0, victim, TO_VICT);
	act("You point at $N and utter the word 'Fear!'", ch, 0, victim, TO_CHAR);

	if (!is_awake(victim))
	{
		act("$n shivers momentarily but it passes.", victim, 0, 0, TO_ROOM);
		send_to_char("You feel a brief terror, but it passes away in your dreams.\n\r", victim);
		return;
	}

	if (is_affected(victim, sn))
	{
		send_to_char("They are already affected by a word of power.\n\r", ch);
		send_to_char("You feel a shiver pass through you but it has no further affect.\n\r", victim);
		return;
	}

	if (saves_spell(level, victim, DAM_OTHER))
	{
		act("$n shivers momentarily but it passes.", victim, 0, 0, TO_ROOM);
		send_to_char("You feel a brief terror, but it passes.\n\r", victim);
		return;
	}

	if (!saves_spell(level - 2, victim, DAM_OTHER))
	{
		bad_fail = true;

		if (!saves_spell(level - 5, victim, DAM_OTHER))
			utter_fail = true;
	}

	if (utter_fail)
	{
		act("$n's eyes widen and $s heart ruptures from shock!", victim, 0, 0, TO_ROOM);
		send_to_char("You feel a terror so intense your heart stops dead!\n\r", victim);
		raw_kill(ch, victim);
		return;
	}

	act("$n's eyes widen in shock and $s entire body freezes in momentary terror.", victim, nullptr, nullptr, TO_ROOM);
	send_to_char("You feel an overwhelming terror and you shudder in momentary shock.\n\r", victim);

	range = level / 10;

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = sn;
	af.level = level;
	af.duration = (number_range(1, 5) + range);
	af.location = APPLY_CON;
	af.modifier = -number_range(2, range + 1);
	affect_to_char(victim, &af);

	af.location = APPLY_STR;
	af.modifier = -number_range(2, range + 1);
	affect_to_char(victim, &af);

	af.location = APPLY_DEX;
	af.modifier = -number_range(1, range);
	affect_to_char(victim, &af);

	af.location = APPLY_HIT;
	af.modifier = -number_range(2, range);
	affect_to_char(victim, &af);

	af.location = APPLY_DAMROLL;
	af.modifier = -number_range(2, range);
	affect_to_char(victim, &af);

	if (victim->position == POS_FIGHTING)
		do_flee(victim, "");

	if (victim->position == POS_FIGHTING)
		do_flee(victim, "");

	if (victim->position == POS_FIGHTING)
		do_flee(victim, "");

	if (bad_fail)
		LAG_CHAR(victim, 12);
}

/* Causes a target's hp to regenerate at a constant rate, plus removes the
   effects of a wither prevent healing spell */
void spell_imbue_regeneration(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected(victim, gsn_atrophy))
	{
		act("$n's body stops wasting away.", victim, 0, 0, TO_ROOM);
		send_to_char("Your body stops wasting away.\n\r", victim);
		affect_strip(victim, gsn_atrophy);
		return;
	}

	if (is_affected(victim, skill_lookup("prevent healing")))
	{
		act("$n's sickly looking complexion clears up.", victim, 0, 0, TO_ROOM);
		send_to_char("You no longer feel so sick and weary.\n\r", victim);
		affect_strip(victim, skill_lookup("prevent healing"));
		return;
	}

	if (is_affected(victim, skill_lookup("wither")))
	{
		if (victim != ch)
		{
			act("$N's emanciated body heals up with your touch.", ch, 0, victim, TO_CHAR);
			act("Your emanciated body heals up with $n's touch.", ch, 0, victim, TO_VICT);
			act("$N's emanciated body heals up with $n's touch.", ch, 0, victim, TO_NOTVICT);
		}
		else
		{
			send_to_char("Your emaciated body parts heal up.\n\r", ch);
			act("$n's emaciated body heals up.", ch, 0, 0, TO_ROOM);
		}

		affect_strip(victim, skill_lookup("wither"));
		return;
	}

	if (is_affected(victim, sn))
	{
		if (victim == ch)
			send_to_char("You are already healing at an improved rate.\n\r", ch);
		else
			send_to_char("They are already healing at an improved rate.\n\r", ch);

		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_COMMUNE;
	af.type = sn;
	af.location = APPLY_REGENERATION;
	af.modifier = level / 11;

	if (!is_npc(ch) && ch->Class()->GetIndex() == CLASS_HEALER)
		af.modifier += 2;

	af.level = level;
	af.duration = level / 5;
	af.mod_name = MOD_REGEN;
	affect_to_char(victim, &af);

	send_to_char("You feel your body warm with an inner health.\n\r", victim);

	if (victim != ch)
		send_to_char("You boost their recuperation ability.\n\r", ch);
}

/*
	Restores forget, wither, prevent healing, atrophy.
	Also restores lost levels due to energy drain by powerful undead
*/
void spell_restoration(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	int sn_forget, sn_wither, sn_drain, sn_prevent_healing;
	bool success = false;

	sn_prevent_healing = skill_lookup("prevent healing");
	sn_forget = skill_lookup("forget");
	sn_wither = skill_lookup("wither");
	sn_drain = skill_lookup("undead drain");

	if (is_affected(victim, sn_forget))
	{
		send_to_char("You feel your mind becoming clearer.\n\r", victim);
		act("$n looks less confused.", victim, 0, 0, TO_ROOM);
		affect_strip(victim, sn_forget);
		success = true;
	}

	if (is_affected(victim, sn_wither))
	{
		send_to_char("Your emaciated body is restored.\n\r", victim);
		act("$n's emaciated body looks healthier.", victim, 0, 0, TO_ROOM);
		affect_strip(victim, sn_wither);
		success = true;
	}

	if (is_affected(victim, sn_prevent_healing))
	{
		send_to_char("You no longer feel so sickly and weary.\n\r", victim);
		act("$n's sickly complexion clears up.", victim, nullptr, nullptr, TO_ROOM);
		affect_strip(victim, sn_prevent_healing);
		success = true;
	}

	if (is_affected(victim, gsn_atrophy))
	{
		act("$n's body stops wasting away.", victim, 0, 0, TO_ROOM);
		send_to_char("Your body stops wasting away.\n\r", victim);
		affect_strip(victim, gsn_atrophy);
	}

	if (is_affected(victim, sn_drain) && check_dispel(level + 15, victim, sn_drain))
	{
		act("$n looks much better.", victim, nullptr, nullptr, TO_ROOM);
		affect_strip(victim, sn_drain);
		success = true;
	}

	if (!success)
		send_to_char("Spell had no effect.\n\r", ch);
}

void spell_prevent_healing(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected(victim, sn))
	{
		send_to_char("Their body is already deficient in healing ability.\n\r", ch);
		return;
	}

	if (saves_spell(level, victim, DAM_OTHER) || !str_cmp(race_table[victim->race].name, "troll"))
	{
		send_to_char("Spell failed.\n\r", ch);
		return;
	}

	send_to_char("You feel your body losing it's ability to heal.\n\r", victim);
	act("$n looks very sick.", victim, 0, 0, TO_ROOM);

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = gsn_prevent_healing;
	af.level = level;
	af.location = 0;
	af.modifier = 0;
	af.duration = level / 6;
	af.mod_name = MOD_REGEN;
	affect_to_char(victim, &af);
}

void spell_atrophy(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected(victim, sn))
	{
		if (victim == ch)
			send_to_char("You are already wasting away.\n\r", ch);
		else
			send_to_char("They are already wasting away.\n\r", ch);

		return;
	}

	if (saves_spell(level, victim, DAM_OTHER) || !str_cmp(race_table[victim->race].name, "troll"))
	{
		send_to_char("Spell failed.\n\r", ch);
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = sn;
	af.duration = level / 7;
	af.location = APPLY_REGENERATION;
	af.modifier = number_range(-3, -level / 12);
	af.level = level;
	af.mod_name = MOD_REGEN;
	affect_to_char(victim, &af);

	send_to_char("You feel an intense pain as your body begins to waste away.\n\r", victim);
	act("$n looks very sick as $s body starts wasting away before your eyes!", victim, nullptr, nullptr, TO_ROOM);
}

/*
	Heal spell, but also for only slightly more cost can cure poison and disease.
	Can stop wasting, but no heal benefit is gained if used this way. Won't
	restore undead_drains or wither etc.
*/
void spell_utter_heal(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	int sn_atrophy, sn_poison;

	if (is_evil(ch))
	{
		send_to_char("The Gods of darkness do not grant such beneficial spells to their clerics.\n\r", ch);
		return;
	}

	sn_atrophy = skill_lookup("atrophy");
	sn_poison = skill_lookup("poison");

	if (is_affected(victim, sn_atrophy))
	{
		affect_strip(victim, sn_atrophy);
		send_to_char("Your body stops wasting away.\n\r", victim);
		act("$n's body stops wasting away.", victim, 0, 0, TO_ROOM);
		return;
	}

	if (is_affected(ch, sn_poison) || is_affected(ch, gsn_poison_dust))
	{
		act("$n looks better.", victim, 0, 0, TO_ROOM);
		send_to_char("You feel a warm sensation running through you.\n\r", victim);
		affect_strip(victim, sn_poison);
		affect_strip(victim, gsn_poison_dust);
	}

	if (is_affected(ch, gsn_plague))
	{
		act("The sores on $n's body disappear.", victim, 0, 0, TO_ROOM);
		send_to_char("The sores on your body disappear.\n\r", victim);
		affect_strip(victim, gsn_plague);
	}

	send_to_char("You feel better!\n\r", victim);

	victim->hit = std::min(victim->hit + 100, (int)victim->max_hit);

	if (victim != ch)
		send_to_char("Ok.\n\r", ch);
}

void spell_hold_person(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;
	float chance;

	if (is_affected(victim, sn))
	{
		send_to_char("They are are already as paralyzed as they can be.\n\r", ch);
		check_improve(ch, sn, false, 1);
		WAIT_STATE(ch, PULSE_VIOLENCE);
		return;
	}

	chance = 55 + (victim->saving_throw * .5);

	if (chance < 35)
		chance = 35;

	if (is_affected_by(victim, AFF_SLOW))
		chance *= 1.5;

	if (is_affected_by(victim, AFF_HASTE))
		chance *= .5;

	if (victim->position == POS_FIGHTING)
		chance -= 13;

	if (is_affected(victim, skill_lookup("resist paralysis")))
		chance -= 25;

	chance -= 10;
	chance = std::min(chance, (float)35);

	if (chance < number_percent())
	{
		send_to_char("You failed.\n\r", ch);
		send_to_char("You feel a slight tightening of your muscles, but it goes away.\n\r", victim);
		WAIT_STATE(ch, PULSE_VIOLENCE * 2);
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = gsn_hold_person;
	af.location = 0;
	af.modifier = 0;
	af.duration = 1;
	af.level = level;
	af.mod_name = MOD_MOVEMENT;
	affect_to_char(victim, &af);

	send_to_char("Your body stiffens up, you can't do anything!\n\r", victim);
	act("$n suddenly stiffens up, $s face taut with agony.", victim, nullptr, nullptr, TO_ROOM);
	check_improve(ch, sn, true, 1);

	WAIT_STATE(ch, PULSE_VIOLENCE * 2);
}

void spell_crushing_hand(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	int dam;
	dam = 160;

	dam += (number_percent()) / 2;

	if (saves_spell(level + 1, victim, DAM_OTHER))
		dam -= 60;

	act("$n creates a huge spectral hand to brutally pummel $N!", ch, 0, victim, TO_NOTVICT);
	act("$n creates a huge spectral hand to brutally pummel you!", ch, 0, victim, TO_VICT);
	act("You create a huge spectral hand to brutally pummel $N!", ch, 0, victim, TO_CHAR);

	if (number_percent() < 60)
	{
		act("$N appears to be stunned by the blow.", ch, 0, victim, TO_NOTVICT);
		act("You feel stunned by the brutal blow.", ch, 0, victim, TO_VICT);
		act("$N appears to be stunned by the blow.", ch, 0, victim, TO_CHAR);
		LAG_CHAR(victim, PULSE_VIOLENCE * 2);
	}

	damage_old(ch, victim, dam, sn, DAM_BASH, true);
}

void spell_deafen(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (ch == victim)
	{
		send_to_char("Deafen who?\n\r", ch);
		return;
	}

	if (is_affected(victim, sn))
	{
		act("$N is already deaf.", ch, nullptr, victim, TO_CHAR);
		return;
	}

	if (saves_spell(level - 2, victim, DAM_CHARM))
	{
		act("You fail to deafen $N.", ch, 0, victim, TO_CHAR);
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = sn;
	af.level = level;
	af.duration = level / 12;
	af.modifier = 0;
	af.location = 0;
	af.mod_name = MOD_HEARING;
	affect_to_char(victim, &af);

	act("You have deafened $N!", ch, nullptr, victim, TO_CHAR);
	send_to_char("The world around you fades to silence.\n\r", victim);
}

void spell_talismanic_aura(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af, *paf;
	float reduction;

	if (ch->talismanic > 2)
	{
		send_to_char("You cannot yet call upon the talismanic powers again.\n\r", ch);
		return;
	}

	for (paf = ch->affected; paf != nullptr; paf = paf->next)
	{
		if (paf->type == gsn_talismanic)
			break;
	}

	if (ch->Class()->name == "necromancer")
		reduction = 0.6;
	else if (ch->Class()->name == "sorcerer")
		reduction = 0.7;
	else
		reduction = 0.6667;

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_INVIS;
	af.type = gsn_talismanic;
	af.level = level;
	af.duration = 8;
	af.location = APPLY_DAM_MOD;
	af.modifier = -(short)(ch->level * reduction);
	af.owner = ch;
	af.end_fun = talismanic_end;
	af.mod_name = MOD_PROTECTION;

	if (is_affected(ch, gsn_talismanic))
	{
		send_to_char("You channel energy into fortifying your talismanic aura.\n\r", ch);
		act("The mystical aura around $n shimmers briefly and glows more intensely.", ch, nullptr, ch, TO_ROOM);

		af.duration = 8;
		affect_to_char(ch, &af);

		af.location = 0;
		af.aftype = AFT_SPELL;
		af.modifier = 0;
		affect_to_char(ch, &af);

		ch->talismanic++;
	}
	else
	{
		send_to_char("A multihued aura coalesces around you.\n\r", ch);
		act("A multihued aura coalsesces around $n.", ch, nullptr, ch, TO_ROOM);
		affect_to_char(ch, &af);

		af.location = 0;
		af.aftype = AFT_SPELL;
		af.modifier = 0;
		affect_to_char(ch, &af);

		ch->talismanic++;
	}
}

void talismanic_end(CHAR_DATA *ch, AFFECT_DATA *af)
{
	if (af->aftype == AFT_INVIS)
		send_to_char("The multihued aura flickers and fades.\n\r", ch);
}

void puddle_evaporate(OBJ_DATA *obj, OBJ_AFFECT_DATA *af)
{
	if (obj->in_room && obj->in_room->people)
		act("The last traces of the puddle fade, as it seeps into the ground and evaporates.", obj->in_room->people, 0, 0, TO_ALL);

	extract_obj(obj);
}

bool check_somatic(CHAR_DATA *ch)
{
	int skill = get_skill(ch, gsn_somatic_casting);

	if (skill < 5)
		return false;

	skill = (int)((float)skill * 0.75);

	WAIT_STATE(ch, PULSE_VIOLENCE);

	if (number_percent() > skill)
	{
		act("$n gestures and focuses intently for a moment, but nothing happens.", ch, 0, 0, TO_ROOM);
		send_to_char("You fail to complete the incantation by using somatic gestures.\n\r", ch);
		return false;
	}

	act("$n concentrates intently as $e gestures with $s hands.", ch, 0, 0, TO_ROOM);
	act("You focus intently, using somatic gestures to complete the incantation.", ch, 0, 0, TO_CHAR);

	check_improve(ch, gsn_somatic_casting, true, 6);

	return true;
}

void mana_drain_pulse(CHAR_DATA *ch, AFFECT_DATA *af)
{
	ch->mana = 0;
}

void colorful_end(CHAR_DATA *ch, AFFECT_DATA *af)
{
	REMOVE_BIT(ch->comm, COMM_LOTS_O_COLOR);
}

void spell_colorful(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	char *parg, arg1[500], arg[500];
	CHAR_DATA *victim;

	parg = one_argument(target_name, arg1);
	one_argument(parg, arg);

	if (arg1[0] == '\0'
		|| !(victim = get_char_room(ch, arg1))
		|| arg[0] == '\0'
		|| !is_number(arg)
		|| atoi(arg) < 0
		|| is_npc(victim))
	{
		send_to_char("Syntax: c colorful target duration\n\r", ch);
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_COMMUNE;
	af.type = sn;
	af.level = ch->level;
	af.location = APPLY_NONE;
	af.modifier = 0;
	af.duration = atoi(arg);
	af.end_fun = colorful_end;
	af.mod_name = MOD_VISION;
	affect_to_char(victim, &af);

	SET_BIT(victim->comm, COMM_LOTS_O_COLOR);
}
