#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <algorithm>
#include "mspec.h"
#include "handler.h"
#include "spec.h"
#include "tables.h"
#include "mprog.h"
#include "act_comm.h"
#include "act_move.h"
#include "fight.h"
#include "newmem.h"
#include "comm.h"
#include "interp.h"
#include "db.h"
#include "devextra.h"
#include "utility.h"
#include "room.h"
#include "./include/spdlog/fmt/bundled/format.h"

BEGIN_MSPECS
	DEF_SPEC(mspec_academy_smith, TRAP_MGREET | TRAP_MGIVE | TRAP_MSPEECH) /* smith quest */
	DEF_SPEC(mspec_academy_greeters, TRAP_MGREET | TRAP_MSPEECH)
	DEF_SPEC(mspec_horde_tanner, TRAP_MGREET | TRAP_MGIVE)
	DEF_SPEC(mspec_scared_soldier, TRAP_MGREET)
	DEF_SPEC(mspec_academy_pet, TRAP_MSPEECH | TRAP_MPULSE)
	DEF_SPEC(mspec_minotaur_zombie, TRAP_MONEHIT)
	DEF_SPEC(mspec_fallendesert_spirits, TRAP_MDEATH)
END_SPECS

BEGIN_MEVENT_TYPES
	MEVENT("beat", TRAP_MBEAT)
	MEVENT("fight", TRAP_MFIGHT)
	MEVENT("give", TRAP_MGIVE)
	MEVENT("misc", TRAP_MMISC)
	MEVENT("move", TRAP_MMOVE)
	MEVENT("death", TRAP_MDEATH)
	MEVENT("pulse", TRAP_MPULSE)
	MEVENT("speech", TRAP_MSPEECH)
	MEVENT("entry", TRAP_MENTRY)
	MEVENT("attack", TRAP_MATTACK)
	MEVENT("aggress", TRAP_MAGGRESS)
	MEVENT("greet", TRAP_MGREET)
	MEVENT("onehit", TRAP_MONEHIT)
END_EVENT_TYPES

// Fallen Desert Progs
BEGIN_SPEC(mspec_fallendesert_spirits)
	EVENT_MDEATH
		EXIT_DATA *pexit = ch->in_room->exit[Directions::North];

		REMOVE_BIT(pexit->exit_info, EX_LOCKED);
		REMOVE_BIT(pexit->exit_info, EX_CLOSED);

		act("With a last wail of anguish, the spirits explode causing a shadow to engulf the cave....", mob, 0, 0, TO_ROOM);
		act("As the bright light recedes a pinpoint of light coming through a hole to the north.", mob, 0, 0, TO_ROOM);
	END_EVENT
END_SPEC

BEGIN_SPEC(mspec_minotaur_zombie)
	EVENT_MONEHIT
		CHAR_DATA *mob = victim;

		if (number_percent() > 25)
		{
			if (get_skill(ch, gsn_enhanced_damage) < 2)
				return 0;

			if (wield)
			{
				act("Your $p falls on empty air as $N rematerializes behind you and strikes!", ch, wield, mob, TO_CHAR);
				act("$n's $p falls on empty air as $N rematerializes behind $m and strikes!", ch, wield, mob, TO_ROOM);
			}
			else
			{
				act("Your strike falls on empty air as $N rematerializes behind you and strikes!", ch, wield, mob, TO_CHAR);
				act("$n's strike falls on empty air as $N rematerializes behind $m and strikes!", ch, wield, mob, TO_ROOM);
			}

			one_hit_new(mob, ch, TYPE_UNDEFINED, HIT_SPECIALS, HIT_UNBLOCKABLE, HIT_NOADD, 80, "flank attack");
			return 1;
		}
	END_EVENT
END_SPEC

BEGIN_SPEC(mspec_academy_greeters)
	EVENT_MGREET
	if (ch->in_room->vnum == 24537)
	{
		if (ch->level < 4)
			mprog_tell(1,
					"Greetings, recruit.  Westward is the Outlying Wilds, for more experienced adventurers than "\
					"yourself.  Eastward, you may find an appropriate challenge in the Overrun Area.", mob, ch);
		else
			mprog_tell(1, "Greetings, recruit.  To the west is the Outlying Wilds, which should be a good challenge for you.", mob, ch);
	}
	END_EVENT
END_SPEC

BEGIN_SPEC(mspec_horde_tanner)
	EVENT_MGIVE
		if (obj->item_type == ITEM_CORPSE_NPC)
		{
			OBJ_DATA *venison = nullptr;
			int i, num;
			char give[MSL], buf2[MSL];

			num = dice(3, 4);
			sprintf(give, "venison %s", ch->true_name);

			for (i = 0; i < num; i++)
			{
				venison = create_object(get_obj_index(OBJ_VNUM_STEAK), 0);

				free_pstring(venison->name);
				free_pstring(venison->short_descr);
				free_pstring(venison->description);
				venison->name = palloc_string("slice meat raw");
				venison->short_descr = palloc_string("a slice of raw meat");
				venison->description = palloc_string("A slice of raw meat is here.");

				obj_to_char(venison, ch);
			}

			mprog_say(1, "Ya jus give me a sec and I'll hack this up for ya.", mob, ch);
			mprog_emote(3, "slams the corpse on his work table and begins cutting it up.", mob, ch);

			RS.Queue.AddToQueue(5, act_queue, "$N hands $n a stack of sliced raw meat.", ch, nullptr, mob, TO_ROOM);
			RS.Queue.AddToQueue(5, act_queue, "$N hands you a stack of sliced raw meat.", ch, nullptr, mob, TO_CHAR);

			extract_obj(obj);
		}
	END_EVENT
END_SPEC

void create_academy_pet(CHAR_DATA *ch)
{
	CHAR_DATA *mob = create_mobile(get_mob_index(ACADEMY_PET));
	char *name;

	if (!mob)
		return;

	free_pstring(mob->name);
	free_pstring(mob->short_descr);
	free_pstring(mob->long_descr);
	free_pstring(mob->description);

	switch (number_range(0, 16))
	{
		case 0:
			name = "Aeveni";
			break;
		case 1:
			name = "Onomath";
			break;
		case 2:
			name = "Bharan";
			break;
		case 3:
			name = "Kasiq";
			break;
		case 4:
			name = "Cienarian";
			break;
		case 5:
			name = "Dennahd";
			break;
		case 6:
			name = "Fierjin";
			break;
		case 7:
			name = "Enilqic";
			break;
		case 8:
			name = "Serifek";
			break;
		case 9:
			name = "Jinarre";
			break;
		case 10:
			name = "Ilikaan";
			break;
		case 11:
			name = "Quilik";
			break;
		case 12:
			name = "Xivar";
			break;
		case 13:
			name = "Ixipin";
			break;
		case 14:
			name = "Rishak";
			break;
		case 15:
			name = "Thritak";
			break;
		default:
			name = "Kaveh";
			break;
	}

	char namebuf[MSL], ldesc[MSL];

	switch (ch->alignment)
	{
		case 0:
			mob->alignment = 0;

			sprintf(namebuf, "academy pet efreet %s", name);
			sprintf(ldesc, "A mysterious efreet hovers near %s.\n\r", ch->name);

			mob->description = palloc_string("Obscured within a cloud of smoke that never seems to dissipate, this efreet effortlessly levitates near its master.  ");
			break;
		case 1000:
			mob->alignment = 1000;

			sprintf(namebuf, "academy pet pixie %s", name);
			sprintf(ldesc, "A winged pixie flutters nervously around %s.\n\r", ch->name);

			mob->description = palloc_string("The wings of this tiny pixie are nearly translucent, yet surprisingly "\
											"sturdy.  It hovers nervously around its master, flitting through the air.  ");
			break;
		default:
			mob->alignment = -1000;

			sprintf(namebuf, "academy pet imp %s", name);
			sprintf(ldesc, "A mischievous imp lurks in the shadows near %s.\n\r", ch->name);

			mob->description = palloc_string("Sliding through the shadows and out on thick, leathery wings, your eye seems "\
											"to slide past this imp unless you concentrate closely.  ");
			break;
	}

	mob->name = palloc_string(namebuf);
	mob->short_descr = palloc_string(name);
	mob->long_descr = palloc_string(ldesc);
	mob->description[strlen(mob->description) - 2] = '\n';
	mob->description[strlen(mob->description) - 1] = '\r';

	char_to_room(mob, ch->in_room);

	if (ch->alignment == 1000)
	{
		do_say(mob, "Greetings, friend.  I have been sent to help you become acclimated to the perils of these lands, "\
					"for the good-hearted must always be wary.");
	}
	else if (ch->alignment == -1000)
	{
		do_say(mob, "You there.  The lower powers have seen your weakness, and sent me to guide you.  I will assist "\
					"you until you are strong enough to be of more use to us.");
	}
	else
	{
		do_say(mob, "Shalar welcomes you.  I have been sent to ensure you have the chance to make your mark upon these "\
					"lands.  I will be guiding you for now.");
	}

	add_follower(mob, ch);

	mob->leader = ch;
	ch->pet = mob;

	RS.Queue.AddToQueue(3, do_say_queue, mob,
						"I can aid you in finding food, water, a boat, and a place to practice.  If you need to find "\
						"somewhere to fight for learning, I can help with that, as well as a few other things.");

	RS.Queue.AddToQueue(5, do_say_queue, mob, "To ask for my aid, direct your question to me.");

	auto buffer = fmt::format("{}, I need to find food.", mob->short_descr);
	RS.Queue.AddToQueue(8, do_say_queue, ch, buffer);
}

void apet_force(CHAR_DATA *ch, const char *cmd, int delay)
{
	char buf[MSL], *tal, *tcmd;

	sprintf(buf, "You feel the irresistible urge to '%s'.\n\r", cmd);

	tal = talloc_string(buf);

	RS.Queue.AddToQueue(delay, interpret_queue, ch, std::string(cmd));
	RS.Queue.AddToQueue(delay, send_to_char, tal, ch);

	ch->wait = std::min((int)ch->wait, 20);
}

void apet_at_room(CHAR_DATA *ch, int vnum)
{
	ch->leader->master = nullptr;
	ch->master = ch->leader;

	CHAR_DATA *player = ch->leader;

	char buf[MSL], cname[50];

	if (vnum == CIM_FOOD)
	{
		do_say(ch, "Here's the Small Kiosk.  You can see what's offered by looking at the list, and then buy some.");
		do_say(ch, "You can also buy water skins here.");

		apet_force(player, "list", 2);
		return;
	}

	if (vnum == CIM_WATER)
	{
		do_say(ch, "This fountain's free for all the citizens.  Fill your water container up while you're here.");
		apet_force(player, "fill skin", 2);
		return;
	}

	if (vnum == CIM_BOAT)
	{
		do_say(ch, "This General Store also sells some other useful things besides the raft, such as lanterns.");
		apet_force(player, "list", 2);
		return;
	}

	sprintf(cname, "%s", (RSTR)player->Class()->name);
	cname[0] = UPPER(cname[0]);

	sprintf(buf, "This is the %s's Guild.  You can practice skills and train physically here.", cname);
	do_say(ch, buf);

	apet_force(player, "practice", 2);
	apet_force(player, "train", 5);
}

void apet_walk_to_room(CHAR_DATA *ch, int vnum)
{
	ROOM_INDEX_DATA *room = get_room_index(vnum), *oldroom = ch->in_room;

	if (ch->in_room == room)
	{
		apet_at_room(ch, vnum);
		return;
	}

	walk_to_room(ch, room);

	if (ch->in_room == oldroom || ch->in_room != ch->leader->in_room)
	{
		if (ch->in_room != ch->leader->in_room)
		{
			char_from_room(ch);
			char_to_room(ch, ch->leader->in_room);
		}

		do_say(ch, "I can't seem to lead us over there from here.  Try heading back to another part of town?");

		ch->leader->master = ch;
		ch->master = ch->leader;
		return;
	}

	WAIT_STATE(ch->leader, PULSE_VIOLENCE);

	RS.Queue.AddToQueue(2, apet_walk_to_room, ch, vnum);
}

BEGIN_SPEC(mspec_academy_pet)
	EVENT_MPULSE
		if (!ch->leader)
		{
			extract_char(ch, true);
			return 0;
		}

		if (ch->leader->level > 22 && !is_immortal(ch->leader))
		{
			do_say(ch, "You are strong enough to stand on your own now.  Perhaps we shall meet again.");
			act("$n fades into the shadows.", ch, 0, 0, TO_ROOM);

			ch->leader->pet = nullptr;
			ch->leader = nullptr;
			ch->master = nullptr;

			extract_char(ch, true);
			return 0;
		}

		if (ch->in_room != ch->leader->in_room)
		{
			if (ch->fighting)
			{
				act("$n seems to fade into the shadows.", ch, 0, 0, TO_ROOM);
				stop_fighting(ch, true);
			}

			char_from_room(ch);
			char_to_room(ch, ch->leader->in_room);

			act("$n emerges from the shadows behind you.", ch, 0, ch->leader, TO_VICT);
			return 0;
		}

		if (ch->hit < ch->max_hit / 3)
			stop_fighting(ch, true);

		if (ch->in_room->vnum == 24527 && number_percent() > 90)
		{
			do_say(ch, "Things in Shalar can often be deceptive at first glance.  Perhaps the bust in this room is worth a second look.");
			return 0;
		}

		if (number_percent() > 96 && !ch->fighting && !IS_SET(ch->comm, COMM_NOGOSSIP))
		{
			char *msg;

			switch (number_range(0, 4))
			{
				case 0:
					msg = "I know an inexpensive place to buy food, if you need some.";
					break;
				case 1:
					if (get_skill(ch->leader, gsn_enhanced_damage) < 1)
						msg = "Looking for adventure? I know just the place!";
					else
						msg = "I've heard of a new place for you to learn that might be ideal for you.";

					break;
				case 2:
					msg = "If you don't want me to make suggestions on my own, tell me to be quiet.";
					break;
				default:
					msg = "I can show you where to find food, water, a boat, or a place to practice if you ask.  Remember to "\
						"address me by my name, though.";
					break;
			}

			if (ch->leader->level < 15 && number_percent() > 50)
				msg = "When you are ready to leave the Academy, walk out or recall and you will be trained to the twentieth level of your guild.";

			do_say(ch, msg);
			return 0;
		}
	END_EVENT

	EVENT_MSPEECH
		if (ch != mob->leader)
			return 0;

		char arg1[MSL];

		argument = one_argument(argument, arg1);

		if (argument[0] == '\0' || arg1[0] == '\0' || str_prefix(mob->short_descr, arg1))
			return 0;

		if (strstr(argument, "food") || strstr(argument, "Food"))
		{
			do_say(mob, "A good place to purchase food is at the Small Kiosk in Cimar.");

			if (str_cmp(mob->in_room->area->name, "Cimar"))
			{
				do_say(mob, "If you take us to Cimar I can lead you to the Kiosk.");
				return 0;
			}

			ch->master = mob;
			mob->master = nullptr;

			apet_walk_to_room(mob, CIM_FOOD);
			return 0;
		}

		if (strstr(argument, "water") || strstr(argument, "Water"))
		{
			do_say(mob, "I know a fountain where you can get some free water.");

			if (str_cmp(mob->in_room->area->name, "Cimar"))
			{
				do_say(mob, "If you head over to Cimar I'll show you the place.");
				return 0;
			}

			ch->master = mob;
			mob->master = nullptr;

			apet_walk_to_room(mob, CIM_WATER);
			return 0;
		}

		if (strstr(argument, "practice")
			|| strstr(argument, "Practice")
			|| strstr(argument, "guild")
			|| strstr(argument, "Guild"))
		{
			do_say(mob, "You can practice and train in your guild.");

			if (str_cmp(mob->in_room->area->name, "Cimar"))
			{
				do_say(mob, "If you walk back to Cimar I can show you one there.");
				return 0;
			}

			ch->master = mob;
			mob->master = nullptr;

			int vnum = 0, cclass = ch->Class()->GetIndex();

			apet_find_guild();

			if (vnum == 0)
			{
				do_say(mob, "Sorry, I don't know your guild for some reason.  Perhaps you should pray about it.");
				return 0;
			}

			apet_walk_to_room(mob, vnum);
			return 0;
		}

		if (strstr(argument, "learn")
			|| strstr(argument, "Learn")
			|| strstr(argument, "rank")
			|| strstr(argument, "level")
			|| strstr(argument, "adventure"))
		{
			do_say(mob, "The best way to advance within your guild is to fight.  Remember to 'consider' foes before picking a fight with someone.");

			if (ch->alignment == 0)
				do_say(mob, "Since you have no particular moral alignment, you will learn the same from fighting anything.");

			if (ch->alignment == 1000)
				do_say(mob, "As a friend of the light, you will learn best by killing those who walk the path of evil, "\
							"although you can also fight those of no particular moral alignment.");

			if (ch->alignment == -1000)
				do_say(mob, "Darkwalkers like us learn best by killing the weakling do-gooders, although we can kill anybody "\
							"we want and still gain more experience.");

			if (ch->alignment > -1)
			{
				RS.Queue.AddToQueue(2, do_say_queue, mob,
									"A good place to fight for us might be the Dying Forest.  I understand the trolls there "\
									"are good opponents.");
				RS.Queue.AddToQueue(4, do_say_queue, mob,
									"You can reach it by going out the eastern gate of Cimar along the Cimarrite Causeway, "\
									"then south through the Stunted Forest to the Dying Forest.");
				RS.Queue.AddToQueue(6, do_say_queue, mob,
									"On the other hand, I've heard rumors of a crypt accessible through the Cimar Library "\
									"north of the water fountain, where you can fight the undead.  I'm not sure how you get "\
									"in, but I bet you could find it if you looked there.");
			}

			if (ch->alignment == -1000)
			{
				RS.Queue.AddToQueue(2, do_say_queue, mob, "We can slay the fool dryads in the Emerald Forest!");
				RS.Queue.AddToQueue(4, do_say_queue, mob,
									"To reach it, first you should go to the major city of Melcene, out the eastern gate of "\
									"Cimar along the Cimarrite Causeway, along the Great Shalaran Road.");
				RS.Queue.AddToQueue(6, do_say_queue, mob,
									"Then, when you're before the Gates of Melcene, head north all the way, east as far as you "\
									"can, north, and east all the way into the village square of the Emerald Forest.");
			}

			if (get_skill(ch, gsn_enhanced_damage) < 1)
			{
				RS.Queue.AddToQueue(7, do_say_queue, mob,
									"Although... I've heard of caves around Iseldheim.  Caves where there are zombies nearly "\
									"impervious to physical attacks: they can rematerialize at the touch of metal.. but if you "\
									"can think of a way around that - magic, maybe - you'd be good to go.");
				RS.Queue.AddToQueue(8, do_say_queue, mob,
									"To get there, first head to the great Northern City of Iseldheim.  Leave Cimar by the Northern Gate, then "\
									"head all north and east through the village along the North Cimar Road.");
				RS.Queue.AddToQueue(9, do_say_queue, mob,
									"Once you're in Iseldheim, go up to the top level, out the eastern gate, and go northeast "\
									"till you reach a woodland trail.  Caves should be around there... but be careful.");
			}

			return 0;
		}

		if (strstr(argument, "boat") || strstr(argument, "boat"))
		{
			do_say(mob, "You need a boat to cross bodies of water that aren't bridged.  I'll show you a good place to get one.");

			if (str_cmp(mob->in_room->area->name, "Cimar"))
			{
				do_say(mob, "If you head over to Cimar I'll show you the way to the General Store.");
				return 0;
			}

			ch->master = mob;
			mob->master = nullptr;

			apet_walk_to_room(mob, CIM_BOAT);
			return 0;
		}
		if ((strstr(argument, "be quiet") || strstr(argument, "Be quiet")) && !IS_SET(mob->comm, COMM_NOGOSSIP))
		{
			interpret(mob, "snort");
			do_say(mob, "Fine, have it your way.  No more comments from me.  If you come to your senses, you can ask me to talk again.");

			SET_BIT(mob->comm, COMM_NOGOSSIP);
			return 0;
		}

		if ((strstr(argument, "talk") || strstr(argument, "talk")) && IS_SET(mob->comm, COMM_NOGOSSIP))
		{
			interpret(mob, "sage");
			do_say(mob, "Glad to see you came to your senses.  I'll give you my advice from now on, unless you tell me to be quiet again.");

			REMOVE_BIT(mob->comm, COMM_NOGOSSIP);
			return 0;
		}
	END_EVENT

	EVENT_MDEATH
		mob->hit = mob->max_hit / 3;

		stop_fighting(mob, true);

		if (!mob->leader)
			extract_char(mob, true);

		return 1;
	END_EVENT
END_SPEC
