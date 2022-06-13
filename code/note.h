#ifndef NOTE_H
#define NOTE_H

#include <stdio.h>
#include "merc.h"
#include "prof.h"

//	== table layout ==
//	CREATE TABLE `notes` (
//		`type` tinyint(4) default NULL,
//		`sender` tinytext,
//		`date` tinytext,
//		`to_list` tinytext,
//		`subject` tinytext,
//		`message` text,
//		`timestamp` int(11) default NULL
//	) ENGINE=MyISAM DEFAULT CHARSET=latin1;

#define NOTE_FILE RIFT_AREA_DIR "/notes.not"

#define NOTE_NOTE		0
#define NOTE_IDEA		1
#define NOTE_PENALTY	2
#define NOTE_NEWS		3
#define NOTE_CHANGES	4

typedef struct note_data NOTE_DATA;
struct note_data
{
	NOTE_DATA *next;
	bool valid;
	sh_int type;
	char *sender;
	char *date;
	char *to_list;
	char *subject;
	char *text;
	time_t date_stamp;
};

extern NOTE_DATA *note_free;

//
// LOCAL FUNCTIONS
//

int count_spool (CHAR_DATA *ch, int type);
void do_unread (CHAR_DATA *ch, char *arg);
void do_note (CHAR_DATA *ch,char *argument);
void do_news (CHAR_DATA *ch,char *argument);
void do_changes (CHAR_DATA *ch,char *argument);
void append_note (NOTE_DATA *pnote);
bool is_note_to (CHAR_DATA *ch, char *sender, char *to_list);
void note_attach (CHAR_DATA *ch, int type);
bool hide_note (CHAR_DATA *ch, MYSQL_ROW row);
void update_read (CHAR_DATA *ch, long stamp, int type);
void parse_note (CHAR_DATA *ch, char *argument, int type);

/* note recycling */
NOTE_DATA *new_note(void);
void free_note(NOTE_DATA *note);

#endif /* NOTE_H */