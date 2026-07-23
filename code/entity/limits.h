#ifndef ENTITY_LIMITS_H
#define ENTITY_LIMITS_H

//
// Array bounds baked into the entity struct layouts, moved out of merc.h so
// the entity headers can stand alone.  Changing any of these changes entity
// memory layout (and breaks player-file compatibility where arrays are
// written raw) -- treat them as part of the entity definitions themselves.
//

#define MAX_INPUT_LENGTH			4608
#define MAX_BITVECTOR				2
#define MAX_STATS					5

#define MAX_SKILL					800
#define MAX_SOCIALS					256
#define MAX_IN_GROUP				354
#define MAX_CLASS					12
#define MAX_CABAL					7
#define MAX_GROUP					50
#define MAX_ADJACENT				25
#define MAX_ALIAS					10
#define MAX_QUESTS					8
#define MAX_QUEUE					20
#define MAX_EVENTS					13

#define MAX_MOB_AFFECT				10
#define MAX_MOB_CAST				10
#define MAX_PROFS_TAUGHT_BY_MOB		4

// AP demonic favors
#define MAX_DEVIL					5
#define MAX_LESSER					5
#define MAX_GREATER					5

#endif /* ENTITY_LIMITS_H */
