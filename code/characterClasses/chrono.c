#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <algorithm>
#include "../merc.h"
#include "chrono.h"
#include "../magic.h"
#include "../comm.h"
#include "../recycle.h"
#include "../db.h"
#include "../lookup.h"
#include "../tables.h"
#include "../const.h"
#include "../utility.h"
#include "../skills.h"
#include "../devextra.h"
#include "../fight.h"
#include "../interp.h"
#include "../update.h"

void spell_stasis_wall(int sn, int level, CHAR_DATA *ch, SpellTarget vo, CastMode mode)
{
	// Both ways in resolve the direction before dispatching, so it arrives in
	// the target: do_cast for a TAR_DIR cast, cast_rune for a drawn rune.  The
	// cast path used to re-parse the global holding the caster's argument
	// instead, which meant any producer aiming this spell at something other
	// than a direction quietly used whatever the last command had left there.
	// Asking the target reports a mismatch and gives a direction of -1.
	int dir = vo.AsDir();
	EXIT_DATA *pexit = nullptr;

	if (dir < 0 || dir >= MAX_DIR)
	{
		send_to_char("That's not a valid direction.\n\r", ch);
		return;
	}

	pexit = ch->in_room->exit[dir];

	if (!pexit || !IS_SET(pexit->exit_info, EX_ISDOOR))
	{
		send_to_char("There must be a door in that direction to place the rune on.\n\r", ch);		
		return;
	}

	// a cast wall goes up now, a drawn one when the rune is finished
	if (mode != CastMode::Rune)
	{
		// apply_rune copies this into a rune of its own, so the template only
		// has to outlive the call.
		RUNE_DATA rune = {};

		rune.level = level;
		rune.placed_on = pexit;
		rune.target_type = RUNE_TO_PORTAL; // grep for this too
		rune.owner = ch->self;
		rune.trigger_type = RUNE_TRIGGER_EXIT; // grep for rune trigger types if you need to
		rune.type = sn;
		rune.duration = level / 10;
		rune.end_fun = nullptr;
		rune.function = trigger_stasis_wall; // this is what's called when the rune is triggered
		apply_rune(&rune);

		act("$n gestures and an immovable barrier snaps into existence to the $t!", ch, direction_table[dir].name, nullptr, TO_ROOM);
		act("You gesture and a stasis wall forms to the $t!", ch, direction_table[dir].name, nullptr, TO_CHAR);
		return;
	}

	// The drawn rune has to survive nine ticks on the queue before draw_rune
	// finalises it, so it needs a lifetime of its own rather than any kind of
	// scratch storage the next caller can reuse.
	//
	// The queue captures its arguments by value into a std::function, so it
	// cannot hold a move-only type: ownership is released into it here and
	// adopted back by draw_rune, whose signature takes the unique_ptr so that
	// every path out of it frees by scope.
	//
	// The one hole left is a queue entry cancelled rather than run
	// (DeleteQueuedEventsInvolving, which extract_char calls for NPCs): that
	// leaks the struct, because the queue cancels with a tombstone and has no
	// hook to run on the way. A leaked rune is the better failure and it is
	// now a leak LeakSanitizer can see rather than an orphan on a free list.
	auto rune = std::make_unique<RUNE_DATA>();

	rune->level = level;
	rune->placed_on = (ROOM_INDEX_DATA *)ch->in_room;
	rune->target_type = RUNE_TO_ROOM;
	rune->owner = ch->self;
	rune->trigger_type = RUNE_TRIGGER_ENTRY;
	rune->type = sn;
	rune->duration = level / 10;
	rune->extra = dir;
	rune->end_fun = nullptr;
	rune->drawn_in = ch->in_room->vnum;
	rune->function = activate_stasis_wall;

	// this checks lose_conc and stuff before finalizing the rune,
	// usually make sure the lag on the rune is at least as long 
	// as the lag on the draw_rune queue
	RS.Queue.AddToQueue(9, "spell_stasis_wall", "draw_rune_queue", draw_rune_queue, rune.release(), ch);
}

bool trigger_stasis_wall(void *vo, void *vo2, void *vo3, [[maybe_unused]] void *vo4)
{
	RUNE_DATA *rune = (RUNE_DATA *)vo;
	CHAR_DATA *victim = (CHAR_DATA *)vo2, *ch = Deref(rune->owner);
	int dir = (int)*(int *)vo3;

	if (ch == nullptr)
		return false;

	if (is_safe_new(ch, victim, false))
		return false;

	act("You try to move to the $t but a stasis wall flickers and holds you back!", victim, direction_table[dir].name, nullptr, TO_CHAR);
	act("$n tries to move to the $t but is halted by a stasis wall!", victim, direction_table[dir].name, nullptr, TO_ROOM);

	return true;
}

bool activate_stasis_wall(void *vo, void *vo2, void *vo3, [[maybe_unused]] void *vo4)
{
	// Zero-initialized, like the sibling template in spell_stasis_wall: this sets
	// nine of the twelve fields and apply_rune copies the whole struct, so
	// leaving it default-initialized handed `extra`, `drawn_in` and
	// `next_content` to the copy as indeterminate values.
	RUNE_DATA *rune = (RUNE_DATA *)vo, new_rune = {};
	CHAR_DATA *victim = (CHAR_DATA *)vo2, *ch = Deref(rune->owner);
	int dir = reverse_d((int)*(int *)vo3);

	// The owner check has to come first: it used to sit below a line that
	// already read ch->in_room, so a rune outliving its caster dereferenced
	// null before ever reaching the guard written for exactly that case.
	if (ch == nullptr || ch == victim || dir != rune->extra)
		return false;

	EXIT_DATA *pexit = ch->in_room->exit[dir];

	if (pexit == nullptr)
		return false;

	if (is_safe_new(ch, victim, false))
		return false;

	act("As you pass through the $t, a stasis wall snaps into existence behind you!", victim, pexit->keyword, nullptr, TO_CHAR);
	act("As $n passes through the $t, a stasis wall snaps into existence behind $m!", victim, pexit->keyword, nullptr, TO_ROOM);

	new_rune.level = rune->level;
	new_rune.placed_on = pexit;
	new_rune.target_type = RUNE_TO_PORTAL;
	new_rune.owner = rune->owner;
	new_rune.trigger_type = RUNE_TRIGGER_EXIT;
	new_rune.type = rune->type;
	new_rune.duration = rune->level / 10;
	new_rune.end_fun = nullptr;
	new_rune.function = trigger_stasis_wall;
	apply_rune(&new_rune);

	return false;
}

// Consumes the rune spell_stasis_wall queued. Taking the unique_ptr by value is
// the point: the queue held the only reference, so every path out of here has to
// end the rune's life, and now the ones that decide the draw failed do it by
// returning. apply_rune takes a copy rather than the struct itself.
void draw_rune(std::unique_ptr<RUNE_DATA> rune)
{
	CHAR_DATA *ch = Deref(rune->owner);

	// Nine ticks pass between the queue entry being made and this running, and
	// extract_char only cancels pending events for NPCs -- so the caster may
	// simply be gone by now.
	if (ch == nullptr)
		return;

	if (ch->in_room->vnum != rune->drawn_in)
	{
		send_to_char("A backlash of energy whips through you as your uncompleted rune overloads!\n\r", ch);
		damage_new(ch, ch, dice(rune->level, 4), TYPE_UNDEFINED, DAM_ENERGY, true, HIT_UNBLOCKABLE, 0, 1, "mana surge");
		return;
	}

	if (number_percent() > get_skill(ch, rune->type))
	{
		act("The rune flares brightly before vanishing!", ch, nullptr, nullptr, TO_ROOM);
		send_to_char("The improperly scribed rune flares brightly before vanishing!\n\r", ch);
		return;
	}

	act("The rune flares $t!", ch, skill_table[rune->type].msg_off, nullptr, TO_ALL);
	apply_rune(rune.get());
}

// The queue's entry point. `ch` is unused here but has to stay in the signature:
// CQueue::GetCharacterData scrapes the argument list for CHAR_DATA pointers to
// build the cancellation index, so dropping it would make this entry
// uncancellable by DeleteQueuedEventsInvolving.
void draw_rune_queue(RUNE_DATA *rune, [[maybe_unused]] CHAR_DATA *ch)
{
	draw_rune(std::unique_ptr<RUNE_DATA>(rune));
}

void do_rune(CHAR_DATA *ch, char *argument)
{
	char arg1[MAX_INPUT_LENGTH], arg2[MAX_INPUT_LENGTH], ttype[MSL];
	OBJ_DATA *obj;
	EXIT_DATA *pexit = nullptr;
	SpellTarget vo;
	int mana, where, sn, target = 0;

	if (is_npc(ch) && Deref(ch->desc) == nullptr)
		return;

	if (ch->Class()->ctype != CLASS_CASTER && !is_immortal(ch))
		send_to_char("You do not know how to create runes.\n\r", ch);

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

	if (!is_npc(ch) && ch->pcdata->energy_state > 2)
	{
		send_to_char("You cannot concentrate enough to complete the incantation.\n\r", ch);		
		return;
	}

	argument = one_argument(argument, ttype);
	target_name = one_argument(argument, arg1);

	one_argument(target_name, arg2);

	if (arg1[0] == '\0')
	{
		send_to_char("Syntax:\n\rrune   <type> <name> <target>\n\r", ch);
		return;
	}

	if ((sn = find_spell(ch, arg1)) < 1
		|| skill_table[sn].spell_fun == spell_null
		|| (!is_npc(ch) && get_skill(ch, sn) < 5)
		|| (!is_npc(ch) && ch->pcdata->learned[sn] == 0))
	{
		send_to_char("You don't know any runes of that name.\n\r", ch);
		return;
	}

	if (ch->position < skill_table[sn].minimum_position)
	{
		send_to_char("You can't concentrate enough.\n\r", ch);
		return;
	}

	if ((skill_table[sn].ctype == CMD_COMMUNE || skill_table[sn].ctype == CMD_POWER ||
		 skill_table[sn].ctype == CMD_SPELL) &&
		!(skill_table[sn].target & RUNE_CAST) && !is_immortal(ch))
	{
		send_to_char("You can't draw that rune.\n\r", ch);
		return;
	}

	if (ch->level + 2 == skill_table[sn].skill_level[ch->Class()->GetIndex()])
		mana = 50;
	else
		mana = std::max((int)skill_table[sn].min_mana, 100 / (2 + ch->level - skill_table[sn].skill_level[ch->Class()->GetIndex()]));

	obj = nullptr;
	vo = SpellTarget();

	if (!str_prefix(ttype, "armor"))
	{
		if (!(skill_table[sn].dispel & RUNE_ARMOR))
		{
			send_to_char("You can't place that type of rune on that.\n\r", ch);
			return;
		}

		target = RUNE_ARMOR;
	}
	else if (!str_prefix(ttype, "weapon"))
	{
		if (!(skill_table[sn].dispel & RUNE_WEAPON))
		{
			send_to_char("You can't place that type of rune on that.\n\r", ch);
			return;
		}

		target = RUNE_WEAPON;
	}
	else if (!str_prefix(ttype, "portal"))
	{
		if (!(skill_table[sn].dispel & RUNE_DOOR))
		{
			send_to_char("You can't place that type of rune on that.\n\r", ch);
			return;
		}

		target = RUNE_DOOR;
	}
	else
	{
		if (!(skill_table[sn].dispel & RUNE_ROOM))
		{
			send_to_char("You can't place that type of rune in here.\n\r", ch);
			return;
		}

		target = RUNE_ROOM;
	}

	if (target == RUNE_WEAPON || target == RUNE_ARMOR)
	{
		if (arg2[0] == '\0')
		{
			send_to_char("What should the rune be placed upon?\n\r", ch);
			return;
		}

		if ((obj = get_obj_carry(ch, target_name, ch)) == nullptr)
		{
			send_to_char("You are not carrying that.\n\r", ch);
			return;
		}

		vo = obj;

		act("$n carefully begins to scribe an intricate rune on $p.", ch, obj, nullptr, TO_ROOM);
		act("You carefully begin to scribe an intricate rune on $p.", ch, obj, nullptr, TO_CHAR);
	}
	else if (target == RUNE_DOOR)
	{
		if ((where = direction_lookup(target_name)) < 0 || !(pexit = ch->in_room->exit[where]))
		{
			send_to_char("That's not a valid direction.\n\r", ch);
			return;
		}

		vo = SpellTarget::Direction(where);

		act("$n carefully begins to scribe an intricate rune on the $t.", ch, strcmp(pexit->keyword, "") ? pexit->keyword : "door", nullptr, TO_ROOM);
		act("You carefully begin to scribe an intricate rune on the $t.", ch, strcmp(pexit->keyword, "") ? pexit->keyword : "door", nullptr, TO_CHAR);
	}
	else if (target == RUNE_ROOM)
	{
		vo = (ROOM_INDEX_DATA *)ch->in_room;
		act("$n moves $s hands slowly, tracing a glowing rune in midair.", ch, nullptr, nullptr, TO_ROOM);
		// A room rune has no door, so pexit is still null on this path. The
		// message carries no $t either, so the argument was only ever read and
		// discarded, which is why the crash was in the read rather than the act.
		act("Slowly tracing a glowing pattern in front of you, you create a rune in midair.", ch, nullptr, nullptr, TO_CHAR);
	}
	else
	{
		return;
	}

	if (!is_npc(ch) && ch->mana < mana)
	{
		send_to_char("You don't have enough mana.\n\r", ch);
		return;
	}

	WAIT_STATE(ch, skill_table[sn].beats);

	ch->mana -= mana;
	if (IS_SET(ch->in_room->room_flags, ROOM_NO_MAGIC) && !(ch->level > LEVEL_HERO))
	{
		act("$n's spell fizzles.", ch, nullptr, nullptr, TO_ROOM);
		send_to_char("Your spell fizzles and dies.\n\r", ch);
		return;
	}

	(*skill_table[sn].spell_fun)(sn, ch->level * 2, ch, vo, CastMode::Rune);
	check_improve(ch, sn, true, 1);
}

RUNE_DATA *find_rune(void *vo, int target_type, int trigger_type, RUNE_DATA *rune_prev)
{
	RUNE_DATA *rune = nullptr, *rune_loop;

	switch (target_type)
	{
		case RUNE_TO_WEAPON:
		case RUNE_TO_ARMOR:
			rune = ((OBJ_DATA *)vo)->rune;
			break;
		case RUNE_TO_PORTAL:
			rune = ((EXIT_DATA *)vo)->rune;
			break;
		case RUNE_TO_ROOM:
			rune = ((ROOM_INDEX_DATA *)vo)->rune;
			break;
	}

	if (rune_prev && !rune_prev->next_content)
		return nullptr;

	if (rune_prev)
		rune = rune_prev->next_content;

	for (rune_loop = rune; rune_loop; rune_loop = rune_loop->next_content)
	{
		if (rune_loop->trigger_type == trigger_type)
			return rune_loop;
	}

	return nullptr;
}

/*
 * The head of the per-container rune chain, as a pointer to the field itself so
 * a caller can rewrite it. `placed_on` is a void * discriminated by
 * target_type, and this is the only place that has to know that.
 */
static RUNE_DATA **rune_container_head(RUNE_DATA *rune)
{
	switch (rune->target_type)
	{
		case RUNE_TO_WEAPON:
		case RUNE_TO_ARMOR:
			return &((OBJ_DATA *)rune->placed_on)->rune;
		case RUNE_TO_PORTAL:
			return &((EXIT_DATA *)rune->placed_on)->rune;
		case RUNE_TO_ROOM:
			return &((ROOM_INDEX_DATA *)rune->placed_on)->rune;
	}

	return nullptr;
}

void extract_rune(RUNE_DATA *rune)
{
	RUNE_DATA *rune_prev;

	// Unlink from the container's chain first. Every rune is on two lists, and
	// erasing it from rune_list below destroys it. This has to happen while
	// the node is still readable, or the chain find_rune walks is left pointing
	// at freed memory. The ordering was load-bearing by accident before
	// rune_list owned its runes; it is load-bearing on purpose now.
	RUNE_DATA **head = rune_container_head(rune);

	if (head != nullptr)
	{
		if (*head == rune)
		{
			*head = rune->next_content;
		}
		else
		{
			for (rune_prev = *head; rune_prev != nullptr; rune_prev = rune_prev->next_content)
			{
				if (rune_prev->next_content == rune)
				{
					rune_prev->next_content = rune->next_content;
					break;
				}
			}
		}
	}

	// And this is the destruction point.
	auto owned = std::find_if(rune_list.begin(), rune_list.end(),
		[rune](const std::unique_ptr<RUNE_DATA> &candidate) { return candidate.get() == rune; });

	if (owned != rune_list.end())
		rune_list.erase(owned);
}

void apply_rune(RUNE_DATA *rune)
{
	OBJ_DATA *obj;
	EXIT_DATA *pexit;
	ROOM_INDEX_DATA *room;

	// The caller's rune is a template it owns itself, often a stack struct,
	// so the copy is what goes on the lists. push_front keeps the order the
	// hand-rolled link had.
	rune_list.push_front(std::make_unique<RUNE_DATA>(*rune));

	RUNE_DATA *rune_new = rune_list.front().get();
	rune_new->next_content = nullptr;

	switch (rune_new->target_type)
	{
		case RUNE_TO_WEAPON:
		case RUNE_TO_ARMOR:
			obj = (OBJ_DATA *)rune_new->placed_on;
			rune_new->next_content = obj->rune;
			obj->rune = rune_new;
			break;
		case RUNE_TO_PORTAL:
			pexit = (EXIT_DATA *)rune_new->placed_on;
			rune_new->next_content = pexit->rune;
			pexit->rune = rune_new;
			break;
		case RUNE_TO_ROOM:
			room = (ROOM_INDEX_DATA *)rune_new->placed_on;
			rune_new->next_content = room->rune;
			room->rune = rune_new;
			break;
	}
}
