#ifndef VOTE_H
#define VOTE_H

#include "merc.h"

bool sort_votes (char *hold[], int *holdi, int cabal);
void do_listvotes (CHAR_DATA *ch, char *argument);
void do_vote (CHAR_DATA *ch, char *argument);


#endif /* VOTE_H */
