#ifndef CONST_H
#define CONST_H

#include <vector>
#include "merc.h"

struct item_type
{
	int type;
	char *name;
};

struct weapon_type
{
	char *name;
	short vnum;
	short type;
	short *gsn;
};

struct wiznet_type
{
	char *name;
	int flag;
	int level;
};

struct attack_type
{
	char *name;		// name
	char *noun;		// message
	int damage;		// damage class
	int modifier;	// difficulty to parry, - for harder + for easier
};

//
// Skills include spells as a particular case.
//

struct skill_type
{
	char *name;							// Name of skill
	short skill_level[MAX_CLASS];		// Level needed by class
	SPELL_FUN *spell_fun;				// Spell pointer (for spells)
	short target;						// Legal targets
	short minimum_position;			// Position for caster / user
	short *pgsn;						// Pointer to associated gsn
	short dispel;						// affect bitvector for dispel
	short min_mana;					// Minimum mana used
	short beats;						// Waiting time after use
	char *noun_damage;					// Damage message
	char *msg_off;						// Wear off message
	char *room_msg_off;					// Wear off message to room
	short ctype;						// Is this a spell or prayer or power?
};

struct  group_type
{
	char *name;
	char *spells[MAX_IN_GROUP];
};

extern const std::vector<item_type> item_table;
extern const std::vector<weapon_type> weapon_table;
extern const std::vector<wiznet_type> wiznet_table;
extern const std::vector<attack_type> attack_table;
extern const struct skill_type skill_table[MAX_SKILL];
extern const struct group_type group_table[MAX_GROUP];

/*
 * The skill and spell table.
 * Slot numbers must never be changed as they appear in #OBJECTS sections.
 */
#define SLOT(n)	n


#endif /* CONST_H */
