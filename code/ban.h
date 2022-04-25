#ifndef BAN_H
#define BAN_H

#include "merc.h"
#include "newmem.h"

//
// Site ban structure.
//

#define BAN_SUFFIX					A
#define BAN_PREFIX					B
#define BAN_NEWBIES					C
#define BAN_ALL						D
#define BAN_PERMIT					E
#define BAN_PERMANENT				F

//
// New ban defines
//

#define NBAN_ALL					0
#define NBAN_NEWBIE					1

#define NBAN_IP						1
#define NBAN_HOST					0

typedef struct ban_data BAN_DATA;
struct ban_data
{
	BAN_DATA *next;
	bool valid;
	long ban_flags[MAX_BITVECTOR];
	sh_int level;
	char *name;
};

//
// LOCAL FUNCTIONS
//

bool check_ban (char *usite, int type, int host);
void do_ban (CHAR_DATA *ch, char *argument);
void do_unban (CHAR_DATA *ch, char *argument);

#endif /* BAN_H */
