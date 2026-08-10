#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <algorithm>
#include "merc.h"
#include "entity/handles.h"
#include "mspec.h"
#include "handler.h"
#include "spec.h"
#include "tables.h"
#include "mprog.h"
#include "act_comm.h"
#include "act_move.h"
#include "fight.h"
#include "pstring.h"
#include "comm.h"
#include "interp.h"
#include "db.h"
#include "devextra.h"
#include "utility.h"
#include "direction.h"
#include "./include/spdlog/fmt/bundled/format.h"

// The program table lives at the bottom of this file, after the handlers.

// Fallen Desert Progs

/// Opens the cave to the north when the spirits are killed.
/// @param ch The killer.
/// @param mob The spirits that died.
/// @return Always 0, so the spirits die normally.
/// @note Body taken from the EVENT_MDEATH block of mspec_fallendesert_spirits.
static int fallendesert_spirits_death(CHAR_DATA *ch, CHAR_DATA *mob)
{
		EXIT_DATA *pexit = ch->in_room->exit[Directions::North];

		REMOVE_BIT(pexit->exit_info, EX_LOCKED);
		REMOVE_BIT(pexit->exit_info, EX_CLOSED);

		act("With a last wail of anguish, the spirits explode causing a shadow to engulf the cave....", mob, 0, 0, TO_ROOM);
		act("As the bright light recedes a pinpoint of light coming through a hole to the north.", mob, 0, 0, TO_ROOM);

		return 0;
}

/// Rematerializes behind an unskilled attacker and counterattacks.
/// @param ch The attacker.
/// @param mob The zombie taking the blow.
/// @param wield The attacker's weapon. May be null.
/// @return 1 when the counterattack lands, which cancels the attacker's blow.
///         0 otherwise.
/// @note Body taken from the EVENT_MONEHIT block of mspec_minotaur_zombie. The
///       macro bound the mob under the name victim and the body aliased it to
///       mob, so the alias is gone.
static int minotaur_zombie_one_hit(CHAR_DATA *ch, CHAR_DATA *mob, OBJ_DATA *wield, float &, int &, int &)
{
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

		return 0;
}

/// Points new recruits at an area suited to their level.
/// @param ch The character who entered.
/// @param mob The greeter.
/// @return Always 0.
/// @note Body taken from the EVENT_MGREET block of mspec_academy_greeters.
static int academy_greeters_greet(CHAR_DATA *ch, CHAR_DATA *mob)
{
	if (ch->in_room->vnum == 24537)
	{
		if (ch->level < 4)
			mprog_tell(1,
					"Greetings, recruit.  Westward is the Outlying Wilds, for more experienced adventurers than "\
					"yourself.  Eastward, you may find an appropriate challenge in the Overrun Area.", mob, ch);
		else
			mprog_tell(1, "Greetings, recruit.  To the west is the Outlying Wilds, which should be a good challenge for you.", mob, ch);
	}

	return 0;
}

/// Butchers a handed-over corpse into slices of raw meat.
/// @param ch The character handing over the corpse.
/// @param mob The tanner.
/// @param obj The object handed over.
/// @return Always 0.
/// @note Body taken from the EVENT_MGIVE block of mspec_horde_tanner.
static int horde_tanner_give(CHAR_DATA *ch, CHAR_DATA *mob, OBJ_DATA *obj)
{
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

			RS.Queue.AddToQueue(5, "sname", "act_queue", act_queue, "$N hands $n a stack of sliced raw meat.", ch, nullptr, mob, TO_ROOM);
			RS.Queue.AddToQueue(5, "sname", "act_queue", act_queue, "$N hands you a stack of sliced raw meat.", ch, nullptr, mob, TO_CHAR);

			extract_obj(obj);
		}

		return 0;
}

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

	mob->leader = ch->self;
	ch->pet = mob->self;

	RS.Logger.Info("entered create_academy_pet queue");
	RS.Queue.AddToQueue(3, "create_academy_pet", "do_say_queue", do_say_queue, mob,
						"I can aid you in finding food, water, a boat, and a place to practice.  If you need to find "\
						"somewhere to fight for learning, I can help with that, as well as a few other things.");

	RS.Queue.AddToQueue(5, "create_academy_pet", "do_say_queue", do_say_queue, mob, "To ask for my aid, direct your question to me.");

	auto buffer = fmt::format("{}, I need to find food.", mob->short_descr);
	RS.Queue.AddToQueue(8, "create_academy_pet", "do_say_queue", do_say_queue, ch, buffer);
}

void apet_force(CHAR_DATA *ch, const char *cmd, int delay)
{
	char *tcmd;

	auto buffer = fmt::format("You feel the irresistible urge to '{}}'.\n\r", cmd);

	RS.Queue.AddToQueue(delay, "apet_force", "interpret_queue", interpret_queue, ch, std::string(cmd));
	RS.Queue.AddToQueue(delay, "apet_force", "send_to_char_queue", send_to_char_queue, buffer, ch);

	ch->wait = std::min((int)ch->wait, 20);
}

void apet_at_room(CHAR_DATA *ch, int vnum)
{
	CHAR_DATA *player = Deref(ch->leader);

	player->master = nullptr;
	ch->master = ch->leader;

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

	sprintf(cname, "%s", player->Class()->name.c_str());
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

	// Read after the move rather than before it: walk_to_room relocates this
	// mob and can fire progs on the way.
	CHAR_DATA *player = Deref(ch->leader);

	if (ch->in_room == oldroom || ch->in_room != player->in_room)
	{
		if (ch->in_room != player->in_room)
		{
			char_from_room(ch);
			char_to_room(ch, player->in_room);
		}

		do_say(ch, "I can't seem to lead us over there from here.  Try heading back to another part of town?");

		player->master = ch->self;
		ch->master = ch->leader;		// handle to handle; no lookup needed
		return;
	}

	WAIT_STATE(player, PULSE_VIOLENCE);

	RS.Queue.AddToQueue(2, "apet_walk_to_room", "apet_walk_to_room", apet_walk_to_room, ch, vnum);
}

/// Keeps the pet with its owner and offers occasional advice.
/// @param mob The pet being updated.
/// @return Always 0.
/// @note Body taken from the EVENT_MPULSE block of mspec_academy_pet. That macro
///       bound the pet under the name ch, while the speech and death blocks of
///       the same program used ch for the other character and mob for the pet.
///       The body is renamed so that mob means the pet in every handler here.
static int academy_pet_pulse(CHAR_DATA *mob)
{
		CHAR_DATA *player = Deref(mob->leader);

		if (player == nullptr)
		{
			extract_char(mob, true);
			return 0;
		}

		if (player->level > 22 && !is_immortal(player))
		{
			do_say(mob, "You are strong enough to stand on your own now.  Perhaps we shall meet again.");
			act("$n fades into the shadows.", mob, 0, 0, TO_ROOM);

			player->pet = nullptr;
			mob->leader = nullptr;
			mob->master = nullptr;

			extract_char(mob, true);
			return 0;
		}

		if (mob->in_room != player->in_room)
		{
			if (Deref(mob->fighting))
			{
				act("$n seems to fade into the shadows.", mob, 0, 0, TO_ROOM);
				stop_fighting(mob, true);
			}

			char_from_room(mob);
			char_to_room(mob, player->in_room);

			act("$n emerges from the shadows behind you.", mob, 0, player, TO_VICT);
			return 0;
		}

		if (mob->hit < mob->max_hit / 3)
			stop_fighting(mob, true);

		if (mob->in_room->vnum == 24527 && number_percent() > 90)
		{
			do_say(mob, "Things in Shalar can often be deceptive at first glance.  Perhaps the bust in this room is worth a second look.");
			return 0;
		}

		if (number_percent() > 96 && !Deref(mob->fighting) && !IS_SET(mob->comm, COMM_NOGOSSIP))
		{
			char *msg;

			switch (number_range(0, 4))
			{
				case 0:
					msg = "I know an inexpensive place to buy food, if you need some.";
					break;
				case 1:
					if (get_skill(player, gsn_enhanced_damage) < 1)
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

			if (player->level < 15 && number_percent() > 50)
				msg = "When you are ready to leave the Academy, walk out or recall and you will be trained to the twentieth level of your guild.";

			do_say(mob, msg);
			return 0;
		}

		return 0;
}

/// Answers the owner's questions and leads them to places in Cimar.
/// @param ch The character speaking.
/// @param mob The pet.
/// @param argument What was said. Tokenized in place with one_argument.
/// @return Always 0.
/// @note Body taken from the EVENT_MSPEECH block of mspec_academy_pet.
static int academy_pet_speech(CHAR_DATA *ch, CHAR_DATA *mob, char *argument)
{
		if (ch != Deref(mob->leader))
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

			ch->master = mob->self;
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

			ch->master = mob->self;
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

			ch->master = mob->self;
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
				RS.Queue.AddToQueue(2, "sname", "do_say_queue", do_say_queue, mob,
									"A good place to fight for us might be the Dying Forest.  I understand the trolls there "\
									"are good opponents.");
				RS.Queue.AddToQueue(4, "sname", "do_say_queue", do_say_queue, mob,
									"You can reach it by going out the eastern gate of Cimar along the Cimarrite Causeway, "\
									"then south through the Stunted Forest to the Dying Forest.");
				RS.Queue.AddToQueue(6, "sname", "do_say_queue", do_say_queue, mob,
									"On the other hand, I've heard rumors of a crypt accessible through the Cimar Library "\
									"north of the water fountain, where you can fight the undead.  I'm not sure how you get "\
									"in, but I bet you could find it if you looked there.");
			}

			if (ch->alignment == -1000)
			{
				RS.Queue.AddToQueue(2, "sname", "do_say_queue", do_say_queue, mob, "We can slay the fool dryads in the Emerald Forest!");
				RS.Queue.AddToQueue(4, "sname", "do_say_queue", do_say_queue, mob,
									"To reach it, first you should go to the major city of Melcene, out the eastern gate of "\
									"Cimar along the Cimarrite Causeway, along the Great Shalaran Road.");
				RS.Queue.AddToQueue(6, "sname", "do_say_queue", do_say_queue, mob,
									"Then, when you're before the Gates of Melcene, head north all the way, east as far as you "\
									"can, north, and east all the way into the village square of the Emerald Forest.");
			}

			if (get_skill(ch, gsn_enhanced_damage) < 1)
			{
				RS.Queue.AddToQueue(7, "sname", "do_say_queue", do_say_queue, mob,
									"Although... I've heard of caves around Iseldheim.  Caves where there are zombies nearly "\
									"impervious to physical attacks: they can rematerialize at the touch of metal.. but if you "\
									"can think of a way around that - magic, maybe - you'd be good to go.");
				RS.Queue.AddToQueue(8, "sname", "do_say_queue", do_say_queue, mob,
									"To get there, first head to the great Northern City of Iseldheim.  Leave Cimar by the Northern Gate, then "\
									"head all north and east through the village along the North Cimar Road.");
				RS.Queue.AddToQueue(9, "sname", "do_say_queue", do_say_queue, mob,
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

			ch->master = mob->self;
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

		return 0;
}

/// Keeps the pet alive when it is killed, so long as it still has an owner.
/// @param mob The pet.
/// @return 1, which stops the death routine so the pet survives.
/// @note Body taken from the EVENT_MDEATH block of mspec_academy_pet. This
///       handler is reachable because the program now advertises the events it
///       fills. Under the mask it was never reached, since the mask omitted
///       TRAP_MDEATH while the body implemented it.
static int academy_pet_death(CHAR_DATA *, CHAR_DATA *mob)
{
		mob->hit = mob->max_hit / 3;

		stop_fighting(mob, true);

		if (!Deref(mob->leader))
			extract_char(mob, true);

		return 1;
}

//////////////////////////////////////////////////////////////////////
//////// ALL SPEC CODE IS ABOVE HERE - BELOW IS MAINTENANCE //////////

const MSpec mspec_table[] =
{
	{
		.name = "mspec_academy_smith",		/* smith quest */
		.on_greet = academy_smith_greet,
		.on_give = academy_smith_give,
		.on_speech = academy_smith_speech
	},
	{
		.name = "mspec_academy_greeters",
		.on_greet = academy_greeters_greet
	},
	{
		.name = "mspec_horde_tanner",
		.on_give = horde_tanner_give
	},
	{
		.name = "mspec_scared_soldier",
		.on_greet = scared_soldier_greet
	},
	{
		.name = "mspec_academy_pet",
		.on_speech = academy_pet_speech,
		.on_pulse = academy_pet_pulse,
		.on_death = academy_pet_death
	},
	{
		.name = "mspec_minotaur_zombie",
		.on_one_hit = minotaur_zombie_one_hit
	},
	{
		.name = "mspec_fallendesert_spirits",
		.on_death = fallendesert_spirits_death
	},
	{ }
};

/// Sends the names of the events a program handles to a character.
/// @param spec The program to describe. May be null, in which case nothing is sent.
/// @param to The character to send the list to.
/// @note The names match the strings the MEVENT entries of the old mevent_table
///       used, so builder facing output is unchanged for the events that a
///       program can actually handle.
void mspec_event_names(const MSpec *spec, CHAR_DATA *to)
{
	if (spec == nullptr)
		return;

	if (spec->on_greet) send_to_char("greet ", to);
	if (spec->on_give) send_to_char("give ", to);
	if (spec->on_speech) send_to_char("speech ", to);
	if (spec->on_pulse) send_to_char("pulse ", to);
	if (spec->on_death) send_to_char("death ", to);
	if (spec->on_one_hit) send_to_char("onehit ", to);
	if (spec->on_move) send_to_char("move ", to);
}
