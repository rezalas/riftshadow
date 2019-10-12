#ifndef MISC_H
#define MISC_H

#include <sys/types.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "merc.h"
#include "magic.h"
#include "recycle.h"
#include "db.h"
#include "lookup.h"
#include "tables.h"

extern sh_int chessboard[8][8];
extern CHAR_DATA *chess_white;
extern CHAR_DATA *chess_black;

char *act_msg (const char *point, CHAR_DATA *ch);
void do_rngtest (CHAR_DATA *ch, char *argument);
int next_sline (SPEECH_DATA *speech);
void sort_speech (SPEECH_DATA *speech);
void BITWISE_OR (long bit1[], const long bit2[]);
void BITWISE_AND (long bit1[], const long bit2[]);
void BITWISE_XAND (long bit1[], const long bit2[]);
void BITWISE_XOR (long bit1[], const long bit2[]);
void zero_vector (long vector[]);
bool IS_ZERO_VECTOR (long vector[]);
bool vector_equal (long vector1[], long vector2[]);
void copy_vector (long copy[], const long original[]);
void fprint_vector (FILE *fp, char *string, long vector[], bool eol);
char *int_to_cap_string (int number);
char *int_to_string (int number);
void spell_summon_nephilim (int sn, int level, CHAR_DATA *ch, void *vo, int target);
void do_damdice (CHAR_DATA *ch, char *argument);
void do_devilfavor (CHAR_DATA *ch, char *argument);
void do_chess (CHAR_DATA *ch, char *argument);
void print_chessboard (CHAR_DATA *ch);
void reset_chessboard (void);
void move_piece (CHAR_DATA *ch, int col_from, int row_from, int col_to, int row_to);
char *piece_name (int piece);
char num_to_letter (int coord);
void do_diku (CHAR_DATA *ch, char *argument);
void do_antiidle (CHAR_DATA *ch, char *arg);
void idle_pulse (CHAR_DATA *ch, AFFECT_DATA *af);

#endif /* MISC_H */
