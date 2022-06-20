#ifndef BAN_H
#define BAN_H

#include "merc.h"
#include "newmem.h"

//	== table layout ==
//	CREATE TABLE `bans` (
//		`site` varchar(254) NOT NULL default '',
//		`by` varchar(50) NOT NULL default '',
//		`reason` varchar(100) NOT NULL default '',
//		`date` datetime default NULL,
//		`duration` bigint(20) NOT NULL default '0',
//		`ban_type` int(11) NOT NULL default '0',
//		`host_type` int(11) NOT NULL default '0'
//	) ENGINE=MyISAM DEFAULT CHARSET=latin1;



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
	short level;
	char *name;
};

//
// LOCAL FUNCTIONS
//

bool check_ban (char *usite, int type, int host);
void do_ban (CHAR_DATA *ch, char *argument);
void do_unban (CHAR_DATA *ch, char *argument);

#endif /* BAN_H */
