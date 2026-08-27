#ifndef TESTS_WORLD_FIXTURE_H
#define TESTS_WORLD_FIXTURE_H

#include <vector>

#include "../code/merc.h"
#include "../code/handler.h"
#include "../code/comm.h"
#include "../code/db.h"
#include "../code/recycle.h"
#include "../code/pstring.h"
#include "../code/characterClasses/class.h"
#include "../code/entity/handles.h"
#include "../code/entity/char_data.h"
#include "../code/entity/obj_data.h"
#include "../code/entity/obj_index_data.h"
#include "../code/entity/room_index_data.h"
#include "../code/entity/area_data.h"
#include "../code/entity/mob_index_data.h"
#include "../code/entity/descriptor_data.h"
#include "../code/prof.h"
#include "../code/mud.h"

//
// A loaded world, small enough to build in a test.
//
// The pieces were already here as file private helpers in two test files, and
// neither knew about the other. One had the room, the handle registration and
// the queue, the other had a descriptor with an output buffer, and it lives in
// a file the build excludes. What no test could do was combine them, which is
// what act() and everything that ends in act() needs: two characters in one
// room, a descriptor whose buffer can be read back, and a class table.
//
// Header only, the way the older helper file is, because tests/CMakeLists.txt
// builds one executable per test source, so an inline definition here is one
// definition per binary.
//

/// A room, its characters, its objects and their connections, torn down with
/// the fixture. Everything it hands back is registered the way the game
/// registers it, so handle-typed references resolve and Deref does not read as
/// "nothing".
///
/// Declare one per SCENARIO, by value. The destructor frees what it created, in
/// the order the ownership rules need: characters first, since a character
/// destroys what it is carrying, then whatever objects are left, then the
/// connections and the rooms.
class TestWorld
{
public:
	TestWorld()
	{
		BuildClassTable();
	}

	~TestWorld()
	{
		// A character is destroyed through free_char so the linked and unlinked
		// cases route the same way they do in the game. Its handle is what says
		// whether it is still alive: extract_char during the test destroys it
		// already, and a destroyed entity retires its handle, so Deref stops
		// resolving back to the pointer we recorded.
		for (CHAR_DATA *ch : characters)
		{
			if (Deref(ch->self) == ch)
				free_char(ch);
		}

		// Same liveness question for objects. A character destroys what it
		// carries, so by this point some of these are already gone.
		for (OBJ_DATA *obj : objects)
		{
			if (Deref(obj->self) == obj)
				free_obj(obj);
		}

		for (DESCRIPTOR_DATA *d : descriptors)
		{
			if (Deref(d->self) == d)
				free_descriptor(d);
		}

		// The prototypes outlive what was stamped from them, the way the index
		// tables do in the game, so they come down after.
		for (MOB_INDEX_DATA *proto : mobPrototypes)
			delete proto;

		for (OBJ_INDEX_DATA *proto : prototypes)
		{
			free_pstring(proto->name);
			free_pstring(proto->short_descr);
			free_pstring(proto->description);
			delete proto;
		}

		// room_index_data has no destructor, so its owned string is this
		// fixture's to release.
		for (ROOM_INDEX_DATA *room : rooms)
		{
			free_pstring(room->name);
			delete room->area;
			delete room;
		}

		for (CClass *cclass : classes)
			delete cclass;

		CClass::first = savedClassList;
	}

	TestWorld(const TestWorld &) = delete;
	TestWorld &operator=(const TestWorld &) = delete;

	/// Builds a lit room in an area of its own. Light matters because the room
	/// is what can_see consults, and a dark room makes act() output depend on
	/// equipment nobody wrote into the test.
	ROOM_INDEX_DATA *CreateRoom(const char *name = "The Testing Room")
	{
		auto room = new room_index_data();
		room->name = palloc_string(name);
		room->light = 3;
		room->sector_type = SECT_INSIDE;
		room->area = new area_data();
		room->area->empty = false;

		rooms.push_back(room);

		return room;
	}

	/// Builds a player: a character with pcdata, a class, a connection whose
	/// output can be read back, and a place to stand. The pcdata is not
	/// optional. can_see reaches pcdata->death_status through pers() for the $n
	/// substitution without checking it, so a player built without one crashes
	/// the first act() that mentions anybody.
	CHAR_DATA *CreatePlayer(const char *name, ROOM_INDEX_DATA *room, int cclass = CLASS_WARRIOR)
	{
		CHAR_DATA *ch = CreateCharacter(name, room);

		ch->pcdata = std::make_unique<pc_data>();
		// The proficiency block holds a back pointer to its character, and the
		// only place production sets it is the load path. Anything that ticks a
		// player reaches it, so a fixture player without it crashes the first
		// char_update.
		ch->Profs()->SetChar(ch);
		ch->SetClass(cclass);
		ch->desc = CreateConnection(ch)->self;
		ch->level = 50;
		ch->trust = 0;

		return ch;
	}

	/// Builds a mob. It gets the prototype a mob is expected to have and the
	/// ACT_IS_NPC flag, which is what is_npc reads. A mob needs no connection:
	/// act() skips the descriptor check for NPCs.
	CHAR_DATA *CreateMob(const char *name, ROOM_INDEX_DATA *room)
	{
		CHAR_DATA *mob = CreateCharacter(name, room);

		mob->pIndexData = CreateMobPrototype();
		SET_BIT(mob->act, ACT_IS_NPC);

		return mob;
	}

	/// Builds an object, registered the way new_obj registers one. Without the
	/// handle its own self reference reads as nothing and every assertion about
	/// where it ended up passes vacuously.
	OBJ_DATA *CreateItem(const char *name = "trinket", const char *shortDescr = "a test trinket")
	{
		auto obj = new obj_data();
		obj->self = objectHandles.Add(obj);
		// The prototype is not decoration. extract_obj reaches
		// obj->pIndexData->limcount with no null check, so an object built
		// without one segfaults the moment anything destroys it, including the
		// character that is carrying it.
		obj->pIndexData = CreatePrototype(name, shortDescr);
		obj->name = palloc_string(name);
		obj->short_descr = palloc_string(shortDescr);
		obj->description = palloc_string("A test trinket lies here.");
		obj->item_type = ITEM_TRASH;
		obj->wear_loc = WEAR_NONE;
		obj->level = 1;
		obj->condition = 100;

		objects.push_back(obj);

		return obj;
	}

	/// Everything act() and send_to_char have written to this character since
	/// the last Clear. Empty rather than null when nothing was written, so a
	/// test can compare without a null check first.
	static const char *Output(CHAR_DATA *ch)
	{
		DESCRIPTOR_DATA *d = Deref(ch->desc);

		if (d == nullptr || d->outbuf == nullptr || d->outtop <= 0)
			return "";

		d->outbuf[d->outtop] = '\0';

		return d->outbuf;
	}

	/// Drops what has been written so far. Use it between the setup and the act
	/// so the assertion reads only the output under test.
	static void ClearOutput(CHAR_DATA *ch)
	{
		DESCRIPTOR_DATA *d = Deref(ch->desc);

		if (d != nullptr)
			d->outtop = 0;
	}

	/// Whether the text appears anywhere in what this character was told.
	static bool Heard(CHAR_DATA *ch, const char *text)
	{
		return strstr(Output(ch), text) != nullptr;
	}

	/// Resolves the gsn_* skill numbers from skill_table, which is the static
	/// compiled-in table rather than anything the database loads. Boot does this
	/// once, so a test that reaches a skill number has to do it too, or every
	/// gsn_* reads as zero and indexes the reserved row.
	static void WireSkillNumbers()
	{
		RS.LoadGsn();
	}

	/// Puts a character on char_list so get_char_world and friends can find it.
	/// Ownership moves to the list, which is what happens at the real link
	/// sites, and the teardown routes through free_char either way.
	void LinkToCharList(CHAR_DATA *ch)
	{
		char_list.push_front(std::unique_ptr<CHAR_DATA>(ch));
		ch->globalNode = char_list.begin();
	}

private:
	/// The half of a character that a player and a mob share.
	CHAR_DATA *CreateCharacter(const char *name, ROOM_INDEX_DATA *room)
	{
		auto ch = new char_data();
		ch->self = charHandles.Add(ch);	// as new_char would
		ch->name = palloc_string(name);
		ch->short_descr = palloc_string(name);
		ch->long_descr = palloc_string(name);
		ch->true_name = palloc_string(name);
		// act() skips anybody below its minimum position, which is POS_RESTING
		// for act() itself. A character left at the zero value is dead and
		// hears nothing.
		ch->position = POS_STANDING;
		ch->level = 1;
		ch->hit = ch->max_hit = 100;
		ch->sex = SEX_MALE;

		characters.push_back(ch);

		if (room != nullptr)
			char_to_room(ch, room);

		return ch;
	}

	/// The prototype behind a mob. free_char does not touch it, because in the
	/// game the index tables own every prototype and outlive the mobs stamped
	/// from them, so the fixture owns this one.
	MOB_INDEX_DATA *CreateMobPrototype()
	{
		auto proto = new mob_index_data();

		mobPrototypes.push_back(proto);

		return proto;
	}

	/// The prototype every object in the world is stamped from. The fixture
	/// gives each object its own rather than sharing one, so a test that
	/// destroys an object cannot disturb another test's counts.
	OBJ_INDEX_DATA *CreatePrototype(const char *name, const char *shortDescr)
	{
		auto proto = new obj_index_data();
		proto->name = palloc_string(name);
		proto->short_descr = palloc_string(shortDescr);
		proto->description = palloc_string("A test trinket lies here.");
		proto->vnum = 1;
		proto->item_type = ITEM_TRASH;
		proto->limtotal = -1;
		proto->condition = 100;

		prototypes.push_back(proto);

		return proto;
	}

	/// A connection with a growable output buffer and no socket behind it.
	/// write_to_buffer is pure buffer manipulation, so output capture needs
	/// nothing else: the only path that touches the socket is the overflow
	/// branch, which a test does not reach.
	DESCRIPTOR_DATA *CreateConnection(CHAR_DATA *ch)
	{
		auto d = new descriptor_data();
		d->self = descriptorHandles.Add(d);	// as new_descriptor would
		d->character = ch->self;
		d->connected = CON_PLAYING;
		// Not a real socket, and saying so keeps the periodic autosave off it:
		// char_update saves every connection whose file descriptor matches the
		// save counter, and a test has no business writing player files. Real
		// descriptors are never negative.
		d->descriptor = -1;
		d->fcommand = false;
		d->outsize = 2000;
		d->outbuf = new char[d->outsize];
		d->outtop = 0;

		descriptors.push_back(d);

		return d;
	}

	/// The one table the game loads from the database that a test in this
	/// fixture actually needs. CClass::LoadClassTable reads RS.Db, so in a test
	/// CClass::first stays null, GetClass returns null and char_data::Class()
	/// hands that null straight back to its caller. Every member is public, so
	/// the list is built by hand instead. The names are the ones the class
	/// ordinals mean. Nothing here asserts on them.
	void BuildClassTable()
	{
		static const struct
		{
			int index;
			const char *name;
			const char *who_name;
		} table[] = {
			{CLASS_NONE, "none", "   "},
			{CLASS_WARRIOR, "warrior", "War"},
			{CLASS_THIEF, "thief", "Thf"},
			{CLASS_ZEALOT, "zealot", "Zea"},
			{CLASS_PALADIN, "paladin", "Pal"},
			{CLASS_ANTI_PALADIN, "anti-paladin", "Ant"},
			{CLASS_RANGER, "ranger", "Ran"},
			{CLASS_ASSASSIN, "assassin", "Asn"},
			{CLASS_SHAPESHIFTER, "shapeshifter", "Shp"},
			{CLASS_HEALER, "healer", "Hea"},
			{CLASS_NECROMANCER, "necromancer", "Nec"},
			{CLASS_SORCERER, "sorcerer", "Sor"},
		};

		// Whatever the binary had is put back by the destructor. A test file
		// with its own hand-built class list keeps working, and this fixture
		// does not have to trust the shape of it.
		savedClassList = CClass::first;
		CClass::first = nullptr;

		CClass *last = nullptr;

		for (const auto &row : table)
		{
			auto cclass = new CClass();
			cclass->index = row.index;
			cclass->name = row.name;
			cclass->who_name = row.who_name;
			cclass->next = nullptr;

			if (last == nullptr)
				CClass::first = cclass;
			else
				last->next = cclass;

			last = cclass;
			classes.push_back(cclass);
		}
	}

	std::vector<CHAR_DATA *> characters;
	std::vector<OBJ_DATA *> objects;
	std::vector<OBJ_INDEX_DATA *> prototypes;
	std::vector<MOB_INDEX_DATA *> mobPrototypes;
	std::vector<DESCRIPTOR_DATA *> descriptors;
	std::vector<ROOM_INDEX_DATA *> rooms;
	std::vector<CClass *> classes;
	CClass *savedClassList = nullptr;
};

//
// The older proficiency helpers, moved here when this file was written.
//
// They build a character the fixture above does not: unregistered, so its own
// handle reads as nothing and the destructor's guard skips the body, with its
// strings pointing at literals the destructor must therefore never free. That
// is why they cannot simply call into TestWorld, and why a test that needs
// act() wants the fixture instead. Kept because the proficiency tests use them
// at 136 sites and their shape is not what those tests are about.
//

inline void TestHelperSetupPlayerBuffer(CHAR_DATA *player, char *name = (char *)"player1", char *room_name = (char *)"room1")
{
	player->name = name;
	player->pcdata = std::make_unique<pc_data>();
	auto dnew = new descriptor_data();
	dnew->self = descriptorHandles.Add(dnew);	// as new_descriptor would
	player->desc = dnew->self;
	dnew->outbuf = new char[2];
	dnew->outtop = 0;
	dnew->fcommand = false;
	dnew->outsize = 2;
	player->in_room = new room_index_data();
	player->in_room->name = room_name;
}

inline void TestHelperSetupTrainer(CHAR_DATA *trainer, char *name = (char *)"trainer1")
{
	trainer->name = name;
	trainer->pIndexData = new mob_index_data();
	memset(trainer->pIndexData->profs_taught, -1, sizeof(short) * MAX_PROFS_TAUGHT_BY_MOB);
}

/// Puts a fixture character on char_list so the code under test can find it
/// through get_char_world and friends. Ownership moves to the list, exactly as
/// it does at the real link sites, so cleanup goes through the list rather than
/// deleting the character directly.
inline void TestHelperLinkToCharList(CHAR_DATA *ch)
{
	char_list.push_front(std::unique_ptr<CHAR_DATA>(ch));
	ch->globalNode = char_list.begin();
}

inline void TestHelperCleanupPlayerObject(CHAR_DATA *player)
{
	if (player == nullptr)
		return;

	// ~descriptor_data frees the output buffer and retires the handle, so this
	// is one delete and not the hand-rolled teardown it used to be.
	delete Deref(player->desc);

	if (player->pIndexData != nullptr)
	{
		delete player->pIndexData;
	}

	player->pcdata.reset();

	if (player->in_room != nullptr)
		delete player->in_room;

	// Same routing free_char uses. A character that was linked is owned by
	// char_list, so erasing the node is what destroys it. One that was never
	// linked is this helper's to delete.
	if (player->globalNode != CharacterList::iterator{})
		char_list.erase(player->globalNode);
	else
		delete player;
}

#endif /* TESTS_WORLD_FIXTURE_H */
