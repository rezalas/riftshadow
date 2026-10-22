#ifndef ENTITY_DESCRIPTOR_DATA_H
#define ENTITY_DESCRIPTOR_DATA_H

#include <list>
#include <memory>

#include "fwd.h"
#include "limits.h"
#include "handles.h"	// self, and the handle-typed cross-references

//
// One player connection.
//

/// The global descriptor list owns every open connection. Declared here rather
/// than in comm.h because a descriptor caches its own node in it.
using DescriptorList = std::list<std::unique_ptr<DESCRIPTOR_DATA>>;

struct descriptor_data
{
	// Frees the owned strings and the output buffer, and expires every handle to
	// this connection. Runs when descriptor_list erases the node (which is how a
	// real connection dies) or when free_descriptor deletes a descriptor that
	// was never linked.
	~descriptor_data();

	// This connection's node in the global descriptor list, valid while it is on
	// it. Caching it is what makes close_socket O(1) instead of a scan for the
	// predecessor, and erasing it is what closes the connection.
	DescriptorList::iterator globalNode {};
	// This connection's own handle, handed out so a character can name it
	// without holding a pointer into a recycled struct. Registered by
	// new_descriptor and retired when the descriptor is destroyed.
	Handle<DESCRIPTOR_DATA> self;
	// The connection watching this one, if any. Non-owning. It expires by
	// itself when that connection closes, which is what close_socket's
	// hand-written sweep over descriptor_list used to do.
	Handle<DESCRIPTOR_DATA> snoop_by;
	// The body this connection drives. While an immortal is switched into a
	// mob, `original` is the immortal's own body parked behind it. Non-owning
	// both ways: a character can be extracted while the connection lives on, so
	// these are handles rather than pointers. `character` used to be nulled by
	// hand on each of the five paths that free it. `original` was nulled on
	// none of them, and check_playing reads its `true_name` for every open
	// connection on every login attempt.
	Handle<CHAR_DATA> character;
	Handle<CHAR_DATA> original;
	char *host;
	short descriptor;
	short connected;
	bool fcommand;
	char inbuf[4 * MAX_INPUT_LENGTH];
	char incomm[MAX_INPUT_LENGTH];
	char inlast[MAX_INPUT_LENGTH];
	int repeat;
	char *outbuf;
	int outsize;
	int outtop;
	// Set when the output buffer overflows. The connection takes no more output
	// and runs no more input, and ClosePendingConnections closes it by the end
	// of the pulse. Closing it on the spot, inside write_to_buffer, made every
	// output call a call that could destroy the connection being written to.
	bool closePending = false;
	char *showstr_head;
	char *showstr_point;
	void *pEdit;	// OLC
	char **pString;	// OLC
	int editor;		// OLC
	short type;
};

#endif /* ENTITY_DESCRIPTOR_DATA_H */
