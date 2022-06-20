#ifndef VOTE_H
#define VOTE_H

#include "merc.h"

typedef struct ballot_data BALLOT_DATA;
typedef struct vote_data VOTE_DATA;
typedef struct vote_sort VOTE_SORT;

struct ballot_data
{
	BALLOT_DATA *next;			// next ballot.
	char *name;					// name of ballot
	char *top_votes_name[15];
	short top_votes[15];
	VOTE_DATA *first_vote;		// first vote. yay.
};

struct vote_data
{
	VOTE_DATA *next;
	char *voter;	// name of person placing vote
	char *vote_for;	// who they voted for
	char *time;		// time they voted
	char *ip;		// ip they voted from
};

struct vote_sort
{
	char who[MAX_STRING_LENGTH]; // person voted for
	int votes;					 // how many votes
};


extern BALLOT_DATA *ballot_first;
extern VOTE_DATA  *vote_first;


void sortvotes (CHAR_DATA *ch, BALLOT_DATA *ballot);
bool sort_votes (char *hold[], int *holdi, int cabal);
void do_listvotes (CHAR_DATA *ch, char *argument);
void do_vote (CHAR_DATA *ch, char *argument);
void load_votes (void);
void save_votes (void);


#endif /* VOTE_H */
