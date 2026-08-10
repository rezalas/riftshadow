#include "merc.h"
#include "entity/spell_target.h"

//
// Accessors live out of line so that the mismatch reporting stays in one place.
// A mismatch means a caller asked for a kind of payload the target does not
// hold, which is a programming error rather than a game condition, so it is
// logged and answered with an empty value.
//


/// Reports an accessor being asked for a kind the target does not hold.
/// @param wanted Name of the kind the caller asked for.
/// @param target The target that was read.
static void report_mismatch(const char *wanted, const SpellTarget *target)
{
	RS.Logger.Warn("SpellTarget: asked for {} but the target holds {}.", wanted, target->KindName());
}

/// Builds a target aimed at a character.
/// @param ch The character being targeted.
SpellTarget::SpellTarget(CHAR_DATA *ch)
	: payload(ch)
{
}

/// Builds a target aimed at an object.
/// @param obj The object being targeted.
SpellTarget::SpellTarget(OBJ_DATA *obj)
	: payload(obj)
{
}

/// Builds a target aimed at a room.
/// @param room The room being targeted.
SpellTarget::SpellTarget(ROOM_INDEX_DATA *room)
	: payload(room)
{
}

/// Builds a target aimed in a direction.
/// @param dir The direction index.
/// @return A target holding that direction.
/// @note Named rather than a constructor because a direction is an int, and an
///       implicit conversion from int would let any stray number become a
///       target.
SpellTarget SpellTarget::Direction(int dir)
{
	SpellTarget target;
	target.payload = DirectionValue{dir};

	return target;
}

/// Reports whether the target holds nothing.
/// @return True when no payload was ever set.
bool SpellTarget::IsNone() const
{
	return std::holds_alternative<std::monostate>(payload);
}

/// Reports whether the target holds a character.
/// @return True when the payload is a character.
bool SpellTarget::IsChar() const
{
	return std::holds_alternative<CHAR_DATA *>(payload);
}

/// Reports whether the target holds an object.
/// @return True when the payload is an object.
bool SpellTarget::IsObj() const
{
	return std::holds_alternative<OBJ_DATA *>(payload);
}

/// Reports whether the target holds a room.
/// @return True when the payload is a room.
bool SpellTarget::IsRoom() const
{
	return std::holds_alternative<ROOM_INDEX_DATA *>(payload);
}

/// Reports whether the target holds a direction.
/// @return True when the payload is a direction.
bool SpellTarget::IsDir() const
{
	return std::holds_alternative<DirectionValue>(payload);
}

/// Reads the target as a character.
/// @return The character, or null if the target holds something else.
/// @note Replaces the unchecked (CHAR_DATA *)vo cast. That cast is what read an
///       object's memory as a character when a spell's object branch failed to
///       match, because the two enum families used for the tag overlap.
CHAR_DATA *SpellTarget::AsChar() const
{
	if (!IsChar())
	{
		report_mismatch("a character", this);
		return nullptr;
	}

	return std::get<CHAR_DATA *>(payload);
}

/// Reads the target as an object.
/// @return The object, or null if the target holds something else.
/// @note Replaces the unchecked (OBJ_DATA *)vo cast.
OBJ_DATA *SpellTarget::AsObj() const
{
	if (!IsObj())
	{
		report_mismatch("an object", this);
		return nullptr;
	}

	return std::get<OBJ_DATA *>(payload);
}

/// Reads the target as a room.
/// @return The room, or null if the target holds something else.
/// @note Replaces the unchecked (ROOM_INDEX_DATA *)vo cast.
ROOM_INDEX_DATA *SpellTarget::AsRoom() const
{
	if (!IsRoom())
	{
		report_mismatch("a room", this);
		return nullptr;
	}

	return std::get<ROOM_INDEX_DATA *>(payload);
}

/// Reads the target as a direction.
/// @return The direction index, or -1 if the target holds something else.
/// @note Replaces the unchecked (int *)vo cast, which pointed at a caller's
///       stack local. The direction is held by value here instead.
int SpellTarget::AsDir() const
{
	if (!IsDir())
	{
		report_mismatch("a direction", this);
		return -1;
	}

	return std::get<DirectionValue>(payload).value;
}

/// Names the kind of payload the target holds.
/// @return A short description, suitable for a log line.
const char *SpellTarget::KindName() const
{
	if (IsChar())
		return "a character";

	if (IsObj())
		return "an object";

	if (IsRoom())
		return "a room";

	if (IsDir())
		return "a direction";

	return "nothing";
}
