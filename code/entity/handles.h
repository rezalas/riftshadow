#ifndef ENTITY_HANDLES_H
#define ENTITY_HANDLES_H

#include "fwd.h"
#include "../stdlibs/handle.h"

//
// The two entity slot maps, and the shorthand for reading through a handle.
//
// These sit beside char_list and object_list -- the same entities, indexed a
// second way. Registration happens in new_char/new_obj and retirement in
// free_char/free_obj, so an entity is registered for exactly the span during
// which it is not on a free list. Nothing else should Add or Remove.
//
// An entity that was never handed out by new_char/new_obj -- one on the stack
// in a test, say -- is simply not registered, and its `self` handle stays
// null. Deref of a null handle is null, so unregistered entities are safe by
// default rather than dangerous by default.
//
// TWO QUESTIONS, AND THEY ARE NOT THE SAME ONE
//
// Every Deref raises two separate questions, and answering one is routinely
// mistaken for answering both.
//
//   1. Is the entity there at all? Deref returns null for a handle that was
//      never set or whose entity is gone, so the result has to be tested.
//   2. Is it still there by the time the pointer is used? A raw pointer is
//      only good until something can free the entity.
//
// Most of the defects found in this code were the first question, not the
// second: a resolve whose result went straight into a dereference, in a loop
// where a neighbouring test looked like it was doing the checking. A condition
// on something else is not a null check. `d->connected == CON_PLAYING` does not
// establish that `Deref(d->character)` resolved, because a connection can be
// playing and its character already freed.
//
// CACHING A Deref RESULT
//
// Reading the same handle several times in a row looks redundant and is a
// standing temptation to hoist into one local, but it is often load-bearing.
// Two families of call can invalidate an entity partway through the block that
// is reading it:
//
//   * Combat and movement. damage_new, multi_hit, one_hit, track_attack and
//     anything that moves a character can end a fight or kill outright.
//   * ANY OUTPUT. send_to_char, act and every page dump reach write_to_buffer,
//     and a buffer that passes its ceiling closes the socket. That destroys the
//     descriptor and frees the character on the end of it. This is the one that
//     gets missed, because writing to somebody does not read like a call that
//     can destroy them.
//
// A cached pointer survives both. The handle does not, which is the entire
// point of it being a handle.
//
// The rule, and it is worth stating because the two cases look identical:
//
//   * Read once into a local ONLY when nothing between the read and its last
//     use can invalidate the entity. Pure field reads and arithmetic qualify.
//     Display code does NOT, which is the trap: the display path is made of
//     output calls, so it is the least safe place to hoist rather than the
//     most. An earlier version of this note listed display code as safe and it
//     was wrong.
//   * Otherwise re-read through the handle at each use, and test the result.
//     A Deref is an array index and an integer compare. A stale pointer is a
//     use-after-free, and an untested one is a null dereference.
//
// A re-read that is never checked is half a guard. Several sites here resolved
// again for exactly the right reason, with a comment saying why, and then used
// the result without asking whether it came back empty.
//
// WHETHER A CALLEE TOLERATES NULL IS ITS OWN PROPERTY
//
// From the call site these are indistinguishable, so it is worth knowing which
// is which before leaning on one. send_to_char re-resolves the descriptor and
// tests it, so telling a character who is gone something is a no-op. can_see
// answers false for an absent character. Neither of those is a general rule,
// and most functions here dereference what they are handed.
//
// Sites that follow the second rule are commented where they sit. Do not
// "simplify" them without checking what the calls in between can do.
//

extern SlotMap<CHAR_DATA> charHandles;
extern SlotMap<OBJ_DATA> objectHandles;
extern SlotMap<DESCRIPTOR_DATA> descriptorHandles;

/// The character this handle names, or null if it has been extracted.
inline CHAR_DATA *Deref(Handle<CHAR_DATA> handle)
{
	return charHandles.Deref(handle);
}

/// The object this handle names, or null if it has been extracted.
inline OBJ_DATA *Deref(Handle<OBJ_DATA> handle)
{
	return objectHandles.Deref(handle);
}

/// The connection this handle names, or null if the socket has been closed.
inline DESCRIPTOR_DATA *Deref(Handle<DESCRIPTOR_DATA> handle)
{
	return descriptorHandles.Deref(handle);
}

#endif /* ENTITY_HANDLES_H */
