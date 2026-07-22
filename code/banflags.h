#ifndef BANFLAGS_H
#define BANFLAGS_H

//
// Ban classification enums, split out of ban.h (phase-02 3.3) so they can be
// included without dragging in merc.h. Dependency-free by construction: every
// enumerator is a literal, this header includes nothing, and it compiles in
// isolation. The values are persisted in the `bans` table (ban_type/host_type
// columns) -- rename freely, never renumber.
//

// bans.ban_type -- what the ban applies to.
enum BanType : int
{
	NBAN_ALL					= 0,
	NBAN_NEWBIE					= 1,
};

// bans.host_type -- how the site string is matched.
enum HostType : int
{
	NBAN_HOST					= 0,
	NBAN_IP						= 1,
};

#endif /* BANFLAGS_H */
