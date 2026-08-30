/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik Stærfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

/***************************************************************************
 *  ROM 2.4 is copyright 1993-1996 Russ Taylor                             *
 *  ROM has been brought to you by the ROM consortium                      *
 *      Russ Taylor (rtaylor@efn.org)                                      *
 *      Gabrielle Taylor (gtaylor@pacinfo.com)                             *
 *      Brian Moore (zump@rom.org)                                         *
 *  By using this code, you have agreed to follow the terms of the         *
 *  ROM license, in the file Tartarus/doc/rom.license                      *
 ***************************************************************************/

/***************************************************************************
 *       Tartarus code is copyright (C) 1997-1998 by Daniel Graham         *
 *  In using this code you agree to comply with the Tartarus license       *
 *       found in the file /Tartarus/doc/tartarus.doc                      *
 ***************************************************************************/

#ifdef macintosh
#include <types.h>
#include <time.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "merc.h"
#include "recycle.h"
#include "comm.h"
#include "db.h"
#include "entity/handles.h"
#include "handler.h"
#include "pstring.h"
#include "misc.h"
#include "utility.h"
#include "direction.h"


/* buffer sizes */
const int buf_size[MAX_BUF_LIST] = {16, 32, 64, 128, 256, 1024, 2048, 4096, 8192, 16384};


long last_pc_id;
long last_mob_id;

DESCRIPTOR_DATA *new_descriptor(void)
{
	// The parentheses matter: descriptor_data has no user-provided constructor,
	// so value-initialization zeroes every POD member before the implicit one
	// runs. That is what `*d = d_zero` used to do.
	//
	// The descriptor is not on descriptor_list yet. init_descriptor links it
	// once it has survived the ban checks, and the test fixtures never link
	// theirs.
	DESCRIPTOR_DATA *d = new DESCRIPTOR_DATA();

	d->self = descriptorHandles.Add(d);

	return d;
}

descriptor_data::~descriptor_data()
{
	free_pstring(host);

	if (outbuf)
		delete[] outbuf;

	// Expires every handle to this connection, which is what makes ch->desc and
	// another descriptor's snoop_by read as nothing from here on.
	descriptorHandles.Remove(self);
}

/// Destroys a descriptor that never made it onto descriptor_list. The list owns
/// the ones that did, and close_socket erasing the node is what destroys those.
/// @param d The descriptor to destroy.
void free_descriptor(DESCRIPTOR_DATA *d)
{
	// The slot map answers "is this a live connection this module handed out",
	// which is what the old `valid` bool was for and one thing more: a
	// descriptor that was never registered has a null handle, so a stack-built
	// one cannot be deleted by mistake.
	if (d == nullptr || Deref(d->self) != d)
		return;

	delete d;
}

/* Trophy list elements own their victname string (rule-of-5). */
trophy_data::trophy_data(const char *name)
	: victname(name ? palloc_string(name) : nullptr)
{
}

trophy_data::~trophy_data()
{
	free_pstring(victname);
}

trophy_data::trophy_data(const trophy_data &other)
	: victname(other.victname ? palloc_string(other.victname) : nullptr)
{
}

trophy_data &trophy_data::operator=(const trophy_data &other)
{
	if (this != &other)
	{
		char *new_victname = other.victname ? palloc_string(other.victname) : nullptr;

		free_pstring(victname);

		victname = new_victname;
	}

	return *this;
}

trophy_data::trophy_data(trophy_data &&other) noexcept
	: victname(other.victname)
{
	other.victname = nullptr;
}

trophy_data &trophy_data::operator=(trophy_data &&other) noexcept
{
	if (this != &other)
	{
		free_pstring(victname);

		victname = other.victname;
		other.victname = nullptr;
	}

	return *this;
}

/* mob speech memory management */

/* Speech/line list elements own their strings (freed on destruction; the
   parent std::list handles the unlinking the old free_speech/free_line did). */
line_data::~line_data()
{
	free_pstring(text);
}

speech_data::~speech_data()
{
	free_pstring(name);
}

IPROG_DATA *new_iprog(void)
{
	// Value-initialized: this type is two parallel halves, eighteen program
	// pointers and eighteen names, and naming the members here only ever
	// covered the names. The pointers were left indeterminate for the caller
	// to zero afterwards.
	return new IPROG_DATA();
}

RACE_DATA *new_race_data(void)
{
	RACE_DATA *race_specs = new RACE_DATA;

	race_specs->next = nullptr;
	race_specs->first = nullptr;
	race_specs->name = nullptr;
	race_specs->number = -1;

	zero_vector(race_specs->act);
	zero_vector(race_specs->aff);
	zero_vector(race_specs->off);
	zero_vector(race_specs->imm);
	zero_vector(race_specs->res);
	zero_vector(race_specs->vuln);
	zero_vector(race_specs->form);
	zero_vector(race_specs->parts);

	return race_specs;
}

void free_race_data(RACE_DATA *race_specs)
{
	if (race_specs->next == nullptr)
	{
		delete race_specs;
	}
	else
	{
		free_race_data(race_specs->next);
		delete race_specs;
	}
}

/* stuff for recycling extended descs -- UGLY */

extra_descr_data::~extra_descr_data()
{
	free_pstring(keyword);
	free_pstring(description);
}

extra_descr_data::extra_descr_data(const extra_descr_data &other)
	: keyword(other.keyword ? palloc_string(other.keyword) : nullptr)
	, description(other.description ? palloc_string(other.description) : nullptr)
{
}

extra_descr_data &extra_descr_data::operator=(const extra_descr_data &other)
{
	if (this != &other)
	{
		char *new_keyword = other.keyword ? palloc_string(other.keyword) : nullptr;
		char *new_description = other.description ? palloc_string(other.description) : nullptr;

		free_pstring(keyword);
		free_pstring(description);

		keyword = new_keyword;
		description = new_description;
	}

	return *this;
}

extra_descr_data::extra_descr_data(extra_descr_data &&other) noexcept
	: keyword(other.keyword)
	, description(other.description)
{
	other.keyword = nullptr;
	other.description = nullptr;
}

extra_descr_data &extra_descr_data::operator=(extra_descr_data &&other) noexcept
{
	if (this != &other)
	{
		free_pstring(keyword);
		free_pstring(description);

		keyword = other.keyword;
		description = other.description;
		other.keyword = nullptr;
		other.description = nullptr;
	}

	return *this;
}

/* An affect owns its `name` string; the rest of its fields are trivially
 * copied. `owner` is a non-owning back-reference copied as-is. */
static void copy_affect_payload(AFFECT_DATA &dst, const AFFECT_DATA &src)
{
	dst.owner = src.owner;
	dst.where = src.where;
	dst.type = src.type;
	dst.level = src.level;
	dst.duration = src.duration;
	dst.location = src.location;
	dst.modifier = src.modifier;
	dst.mod_name = src.mod_name;
	copy_vector(dst.bitvector, src.bitvector);
	dst.aftype = src.aftype;
	dst.tick_fun = src.tick_fun;
	dst.pulse_fun = src.pulse_fun;
	dst.end_fun = src.end_fun;
	dst.init_duration = src.init_duration;
	dst.beat_fun = src.beat_fun;
}

affect_data::~affect_data()
{
	free_pstring(name);
}

affect_data::affect_data(const affect_data &other)
{
	copy_affect_payload(*this, other);
	name = other.name ? palloc_string(other.name) : nullptr;
}

affect_data &affect_data::operator=(const affect_data &other)
{
	if (this != &other)
	{
		char *new_name = other.name ? palloc_string(other.name) : nullptr;

		free_pstring(name);
		name = new_name;

		copy_affect_payload(*this, other);
	}

	return *this;
}

affect_data::affect_data(affect_data &&other) noexcept
{
	copy_affect_payload(*this, other);
	name = other.name;
	other.name = nullptr;
}

affect_data &affect_data::operator=(affect_data &&other) noexcept
{
	if (this != &other)
	{
		free_pstring(name);

		copy_affect_payload(*this, other);
		name = other.name;
		other.name = nullptr;
	}

	return *this;
}

TRAP_DATA *new_trap(void)
{
	TRAP_DATA *trap = new TRAP_DATA;

	return trap;
}

void free_trap(TRAP_DATA *trap)
{
	free_pstring(trap->exec_echo);
	free_pstring(trap->trig_echo);

	delete trap;
}

/* stuff for recycling objects */

OBJ_DATA *new_obj(void)
{
	// The parentheses matter: obj_data has no user-provided constructor, so
	// value-initialization zeroes every POD member before the implicit one runs.
	// That is what `*obj = obj_zero` used to do.
	//
	// The object is not on object_list yet. create_object and fread_obj link it
	// once they have filled it in, and the test fixtures never link theirs.
	OBJ_DATA *obj = new OBJ_DATA();

	obj->self = objectHandles.Add(obj);

	return obj;
}

obj_data::~obj_data()
{
	// affected/charaffs/extra_descr/apply are std::lists of value types and
	// destruct themselves; the obj owns its charaffs and apply copies, which were
	// once shared with the index.

	free_pstring(name);
	free_pstring(description);
	free_pstring(short_descr);

	// free_pstring( owner );

	// Expires every handle to this object.
	objectHandles.Remove(self);
}

/// Destroys an object that never made it onto object_list. The list owns the
/// ones that did, and extract_obj erasing the node is what destroys those.
/// @param obj The object to destroy.
void free_obj(OBJ_DATA *obj)
{
	if (obj == nullptr || Deref(obj->self) != obj)
		return;

	delete obj;
}

/* stuff for recycling characters */

CHAR_DATA *new_char(void)
{
	CHAR_DATA *ch;
	int i;

	// The parentheses matter. char_data has no user-provided constructor, so
	// value-initialization zeroes every POD member before the implicit one
	// runs. That is what `*ch = CHAR_DATA()` used to do to a recycled struct.
	//
	// The character is not on char_list yet. create_mobile links it, and the
	// login hands it over at CON_READ_MOTD once the player is really playing.
	// A character loaded for the login prompt is never linked at all.
	ch = new CHAR_DATA();

	ch->self = charHandles.Add(ch);

	ch->name = &str_empty[0];
	ch->short_descr = &str_empty[0];
	ch->long_descr = &str_empty[0];
	ch->description = &str_empty[0];
	ch->prompt = &str_empty[0];
	ch->prefix = &str_empty[0];
	ch->logon = current_time;
	ch->played = 0;
	ch->lines = PAGELEN;

	for (i = 0; i < 4; i++)
	{
		ch->armor[i] = 0;
	}

	ch->position = POS_STANDING;
	ch->hit = 20;
	ch->max_hit = 20;
	ch->mana = 100;
	ch->max_mana = 100;
	ch->move = 100;
	ch->max_move = 100;
	ch->arms = 2;
	ch->legs = 2;
	ch->regen_rate = 0;
	ch->balance = 0;
	ch->batter = 0;
	ch->analyze = 0;
	ch->talismanic = 0;
	ch->law_pass= false;

	zero_vector(ch->imm_flags);

	for (i = 0; i < MAX_STATS; i++)
	{
		ch->perm_stat[i] = 13;
		ch->mod_stat[i] = 0;
	}

	return ch;
}

char_data::~char_data()
{
	OBJ_DATA *obj;
	OBJ_DATA *obj_next;

	// The guard free_char carried, and it has to stay here rather than move to
	// the callers. It answers "is this a character this module handed out",
	// which is what decides whether the strings below are owned at all. The
	// test fixtures build characters directly and point `name` at string
	// literals, so running this body over one of those would free a literal.
	if (Deref(self) != this)
		return;

	if (is_npc(this))
		mobile_count--;

	for (obj = carrying; obj != nullptr; obj = obj_next)
	{
		obj_next = obj->next_content;
		extract_obj(obj);
	}

	for (auto it = affected.begin(); it != affected.end(); )
	{
		auto next = std::next(it);
		it->pulse_fun = nullptr;
		it->tick_fun = nullptr;
		it->end_fun = nullptr;
		affect_remove(this, &*it);
		it = next;
	}

	memory.clear();

	free_pstring(name);
	free_pstring(short_descr);
	free_pstring(long_descr);
	free_pstring(true_name);
	free_pstring(description);
	free_pstring(prompt);
	free_pstring(prefix);

	// pcdata and gen_data are unique_ptrs and destruct themselves.

	// Expires every handle to this character, which is what makes d->character,
	// ch->master and the rest read as nothing from here on.
	charHandles.Remove(self);
}

/// Destroys a character. The list owns the ones that were linked, so those are
/// destroyed by erasing the node. The login builds characters that never make it
/// onto char_list, and those are deleted outright.
///
/// Unlike free_obj and free_descriptor, this handles both cases rather than
/// warning on the linked one. Eight of its nine call sites are login teardown
/// paths that free a character before CON_READ_MOTD ever links it, and the ninth
/// is extract_char. Keeping the linked case correct means a caller cannot arm a
/// double free by freeing a character that turned out to be in the world.
/// @param ch The character to destroy.
void free_char(CHAR_DATA *ch)
{
	// A character that was never registered has a null handle, so a stack-built
	// one cannot be destroyed by mistake.
	if (ch == nullptr || Deref(ch->self) != ch)
		return;

	if (ch->globalNode != CharacterList::iterator{})
	{
		// Advance the walks before the erase, while the node still links to its
		// successor. The erase is also the destruction point, so this is the
		// last moment `ch` is readable.
		OwningCursorRegistry<CHAR_DATA>::Advance(ch->globalNode);
		char_list.erase(ch->globalNode);
		return;
	}

	delete ch;
}

std::unique_ptr<PC_DATA> new_pcdata(void)
{
	auto pcdata = std::make_unique<PC_DATA>();	// value-init zeroes every POD field

	pcdata->buffer = new BUFFER;

	return pcdata;
}

old_char::~old_char()
{
	free_pstring(name);
	free_pstring(short_descr);
	free_pstring(long_descr);
	free_pstring(description);
}

pc_data::~pc_data()
{
	free_pstring(pwd);
	free_pstring(bamfin);
	free_pstring(bamfout);
	free_pstring(title);
	delete buffer;

	for (int i = 0; i < 100; i++)
	{
		if (recentkills[i] != nullptr)
			free_pstring(recentkills[i]);
	}

	// trophy (std::list) and profs (CProficiencies) auto-destruct.
	// alias[]/alias_sub[] are intentionally not freed, matching the prior
	// free_pcdata (a pre-existing leak preserved here, not introduced).
}

/* stuff for setting ids */

long get_pc_id(void)
{
	int val;

	val = (current_time <= last_pc_id) ? last_pc_id + 1 : current_time;
	last_pc_id = val;
	return val;
}

long get_mob_id(void)
{
	last_mob_id++;
	return last_mob_id;
}

/* local procedure for finding the next acceptable size */
/* -1 indicates out-of-boundary error */
int get_size(int val)
{
	int i;

	for (i = 0; i < MAX_BUF_LIST; i++)
		if (buf_size[i] >= val)
		{
			return buf_size[i];
		}

	return -1;
}

buf_type::~buf_type()
{
	if (string)
		free_pstring(string);
}

bool buf_type::add(const char *text)
{
	int len;
	char *tptr;

	if (text[0] == '\0' || text == nullptr)
		return false;

	if (!string || !strlen(string)) // like a virgin.. touched for the very first tiiiiime
	{
		string = palloc_string(text);
		size = strlen(text) + 1;
		return true;
	}

	len = strlen(string) + strlen(text) + 1;

	if (len > 32766)
		return true;

	tptr = string;
	string = new char[len];

	if (!string)
		return false;

	size = len;

	strcpy(string, tptr);
	strcat(string, text);

	delete[] tptr;
	return true;
}

void buf_type::clear()
{
	free_pstring(string);
	string = nullptr;
	state = BUFFER_SAFE;
}

const char *buf_type::str() const
{
	return string;
}
