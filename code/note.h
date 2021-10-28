#ifndef NOTE_H
#define NOTE_H


#include <stdio.h>
#include "merc.h"

//#include "prof.h"
extern void add_prof_affect(CHAR_DATA *ch, char *name, int duration, bool fInvis);
extern bool is_affected_prof(CHAR_DATA *ch, char *prof);


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

#endif /* NOTE_H */