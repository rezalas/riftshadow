#ifndef ENTITY_CHAR_DATA_H
#define ENTITY_CHAR_DATA_H

#include <time.h>
#include <list>
#include <memory>

#include "fwd.h"
#include "limits.h"
#include "../enums.h"					// CLASS_NONE
#include "../characterClasses/class.h"	// CClass, used by the inline accessors
#include "mob_index_data.h"				// Class() falls back through pIndexData
#include "pc_data.h"					// Profs() reaches into pcdata
#include "gen_data.h"					// GEN_DATA held by unique_ptr in ch->gen_data
#include "mem_data.h"					// MEM_DATA held by value in ch->memory
#include "affect_data.h"				// AFFECT_DATA held by value in ch->affected
#include "../stdlibs/handle.h"			// self, and the handle-typed cross-references

//
// One character (PC or NPC).
//


extern CProficiencies prof_none; //empty proficiencies for jackasses who are going to ref ch->Profs() without checking is_npc

/// The global character list owns every character in the world. Declared here
/// rather than in db.h because a character caches its own node in it.
using CharacterList = std::list<std::unique_ptr<CHAR_DATA>>;

class	char_data
{
public:
	// Releases the carried objects and the affects, frees the owned strings and
	// expires every handle to this character. Runs when char_list erases the
	// node, which is how a real character dies, or when free_char destroys one
	// that was never linked.
	~char_data();

	// A handle naming this character, issued by new_char and retired when the
	// character is destroyed. It is how another entity refers to this one
	// without risking a pointer into a recycled slot. Null for a character that
	// never came from new_char.
	Handle<CHAR_DATA> self;
	// This character's node in the global character list, valid while it is on
	// it. Caching it is what makes extraction O(1) instead of a scan for the
	// predecessor, and erasing it is what destroys the character.
	CharacterList::iterator globalNode {};
	CHAR_DATA *next_in_room;
	// The follower graph. All non-owning, and any of the three can be
	// extracted independently of the others, so they are handles rather than
	// pointers. The sweeps in stop_follower/die_follower stay: they run mostly
	// for reasons nobody died of -- setting nofollow, switching who you
	// follow, a spell breaking a group -- so they are about who follows whom,
	// not about lifetime.
	//
	// Two deliberate rules live here. A follower whose leader is shed becomes
	// its OWN leader, so a handle to self is normal and must keep resolving.
	// And a follower affected by gsn_trail is exempt from the sweep on
	// purpose: the point of the skill is that the target cannot shake them.
	Handle<CHAR_DATA> master;
	Handle<CHAR_DATA> leader;
	// The character this one is in combat with. Non-owning, and the opponent
	// can be extracted independently, so it is a handle rather than a pointer.
	// stop_fighting clears it for both sides -- that is about the fight
	// ending, not about lifetime; almost every caller leaves both characters
	// alive.
	Handle<CHAR_DATA> fighting;
	// Who `reply` answers. Non-owning, and the other party can be extracted
	// independently, so it is a handle rather than a pointer. The clears
	// outside extract_char -- do_noreply, and cloaking with invis/incog -- are
	// a player closing the channel, not a lifetime event.
	Handle<CHAR_DATA> reply;
	// The charmed pet this character owns. Non-owning -- nuke_pets extracts it
	// rather than this field doing so -- and cleared by stop_follower when the
	// pet is dismissed, which leaves both alive.
	Handle<CHAR_DATA> pet;
	// The character this mob is hunting. Non-owning, and the quarry can be
	// extracted independently, so it is a handle rather than a pointer. The
	// clears outside extract_char are a mob giving up the hunt -- losing the
	// trail, a peace command -- which is about intent, not lifetime.
	Handle<CHAR_DATA> last_fought;
	int tracktimer;
	time_t last_fight_time;
	// The player this one last fought, for the lost-link wiznet and do_stat.
	// This used to be a `char *` aliasing the opponent's own true_name buffer
	// and compared by pointer identity, which meant it went stale whenever that
	// buffer was released -- on death, but also on a rename, which no scrub
	// could catch because the buffer was no longer anyone's name. Naming the
	// character instead of their name allocation removes both problems.
	Handle<CHAR_DATA> last_fight_opponent;
	// The character this mob is stalking -- set by the face sucker's greet prog
	// and by the sorcerer's aerial anchor. Non-owning, and the quarry can be
	// extracted independently, so it is a handle rather than a pointer. Nothing
	// clears it anywhere, on death or otherwise, so before it was a handle a
	// dead quarry's stalker was inherited by whoever took its address next.
	Handle<CHAR_DATA> hunting;
	// The character this one is guarding, and the one it is analyzing. Both
	// non-owning, both able to outlive their target, so both are handles.
	// Nothing scrubs either on extract -- `defending` is cleared only when a
	// player quits or stops defending, `analyzePC` never -- so before these
	// were handles they simply dangled on any other death.
	Handle<CHAR_DATA> defending;
	std::list<MEM_DATA> memory;
	GAME_FUN *game_fun;
	MOB_INDEX_DATA *pIndexData;
	// The connection driving this character, if any -- null for a mob, and for
	// a player who has gone link-dead. Non-owning, and the socket can be closed
	// and its struct recycled while the body stays in the world, so it is a
	// handle rather than a pointer. The only clear on a lifetime path is
	// close_socket's, and it is guarded: when an immortal is switched at
	// shutdown, close_socket frees the immortal and the descriptor but never
	// touches the mob, which before this was a handle left the mob holding a
	// descriptor the next connection would be handed.
	Handle<DESCRIPTOR_DATA> desc;
	std::list<AFFECT_DATA> affected;
	NOTE_DATA *pnote;
	OBJ_DATA *carrying;
	// The furniture this character is sitting/resting/sleeping on. Non-owning,
	// and the object can be destroyed independently, so it is a handle rather
	// than a pointer. Cleared outright when the character or the object leaves
	// the room -- that is about position, not lifetime.
	Handle<OBJ_DATA> on;
	ROOM_INDEX_DATA *in_room;
	ROOM_INDEX_DATA *was_in_room;
	AREA_DATA *zone;
	std::unique_ptr<PC_DATA> pcdata;
	std::unique_ptr<GEN_DATA> gen_data;
	char *name;
	char *true_name;
	long id;
	short version;
	char *short_descr;
	char *long_descr;
	char *description;
	char *prompt;
	char *prefix;
	short group;
	short cabal;
	Sex sex;

	CClass *Class()
	{
		return !my_class
			? (pIndexData && pIndexData->Class()
				? pIndexData->Class()
				:  CClass::GetClass(CLASS_NONE))
			: my_class;
	}

	void		SetClass(CharClass nClassIndex)
	{
		my_class = CClass::GetClass(nClassIndex);
	}

	short race;
	short level;
	short trust;
	int played;
	int lines;							// for the pager
	time_t logon;
	short timer;
	short wait;
	short regen_rate;					// For imbue regeneration spell
	int hit;
	short max_hit;
	short mana;
	short max_mana;
	short move;
	short max_move;
	long gold;
	long gold_bank;
	int exp;
	long act[MAX_BITVECTOR];
	long comm[MAX_BITVECTOR];			// RT added to pad the vector
	long wiznet[MAX_BITVECTOR];			// wiz stuff
	long imm_flags[MAX_BITVECTOR];
	long res_flags[MAX_BITVECTOR];
	long vuln_flags[MAX_BITVECTOR];
	short invis_level;
	short incog_level;
	long affected_by[MAX_BITVECTOR];
	Position position = POS_STANDING;
	short practice;
	short train;
	short carry_weight;
	short carry_number;
	short saving_throw;				//svs
	short alignment;
	short hitroll;
	short damroll;
	short armor[4];
	short wimpy;
	char *speechbuf[10];				// Buffer for various mob speech, progs, etc...

	//
	// stats
	//

	short perm_stat[MAX_STATS];
	short mod_stat[MAX_STATS];

	//
	// parts stuff
	//

	long form[MAX_BITVECTOR];
	long parts[MAX_BITVECTOR];
	Size size;
	char *material;

	//
	// mobile stuff
	//

	ROOM_INDEX_DATA *home_room;
	int mobstyle;
	long off_flags[MAX_BITVECTOR];
	short damage[3];
	short dam_type;
	Position start_pos = POS_STANDING;
	short pause;
	short ghost;
	int status;
	long progtypes[MAX_BITVECTOR];
	int hometown;
	float dam_mod;
	short defense_mod;
	bool moved;

	//
	// skill/spell-specific stuff
	//

	short arms;
	short legs;
	short balance;
	short batter;
	Handle<CHAR_DATA> analyzePC;
	int analyze;
	short pulseTimer;					// counter for racial combat pulse
	char *backup_true_name;				// Dev's SUPAR CLEVAR CORRUPTION FIX!!!
	float talismanic;
	bool disrupted;						// Has queue-using spell been disrupted?
	int bounty_timer;
	bool law_pass;
	bool stolen_from;
	CProficiencies *Profs() { return pcdata ? &pcdata->profs : &prof_none; }
private:
	CClass *	my_class;
};

#endif /* ENTITY_CHAR_DATA_H */
