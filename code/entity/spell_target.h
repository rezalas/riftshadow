#ifndef ENTITY_SPELL_TARGET_H
#define ENTITY_SPELL_TARGET_H

#include <variant>

#include "fwd.h"

//
// What a spell or an elemental effect is aimed at.
//
// This replaces a pair of parameters that had to agree without anything
// checking that they did: a void pointer holding the payload, and a separate
// int holding a tag that said how to cast it. The tag was written from four
// unrelated enum families whose values overlap, so a caller could label a
// character as an object and no diagnostic would fire.
//
// Holding the payload and its discriminant in one value removes that class of
// mistake. The alternative is chosen by which constructor ran, so a caller
// cannot label a payload wrongly, and an accessor cannot reinterpret one type
// as another. A mismatched read logs and yields null rather than returning a
// pointer to the wrong kind of thing.
//
class SpellTarget
{
public:
	// No target. This is what a spell cast at nothing in particular carries.
	SpellTarget() = default;

	// Implicit by design. The payload determines the alternative, so callers
	// pass the entity they already have and the discriminant follows from it.
	SpellTarget(CHAR_DATA *ch);
	SpellTarget(OBJ_DATA *obj);
	SpellTarget(ROOM_INDEX_DATA *room);

	// Named, because a direction is an int and an implicit conversion from int
	// would let any stray number become a target.
	static SpellTarget Direction(int dir);

	bool IsNone() const;
	bool IsChar() const;
	bool IsObj() const;
	bool IsRoom() const;
	bool IsDir() const;

	// Each returns the payload only if it is of that kind. On a mismatch the
	// accessor logs the request and returns null, or -1 for a direction.
	CHAR_DATA *AsChar() const;
	OBJ_DATA *AsObj() const;
	ROOM_INDEX_DATA *AsRoom() const;
	int AsDir() const;

	// Name of the alternative currently held, for logging and diagnostics.
	const char *KindName() const;

	//
	// There is deliberately no conversion to a pointer. Reading the payload
	// goes through the accessors above, so a body cannot reach the wrong kind
	// of thing by writing a cast. Adding a conversion here would restore the
	// erasure this type exists to remove.
	//

private:
	// Wrapped so that a direction is a distinct alternative from any future
	// integer payload rather than being matched by index.
	struct DirectionValue
	{
		int value;
	};

	std::variant<std::monostate, CHAR_DATA *, OBJ_DATA *, ROOM_INDEX_DATA *, DirectionValue> payload;
};

#endif /* ENTITY_SPELL_TARGET_H */
