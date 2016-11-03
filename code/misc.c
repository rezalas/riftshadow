/* blah */
#include <sys/types.h>
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
#include <stdarg.h>

extern	sh_int	chessboard[8][8];
extern	CHAR_DATA *chess_white;
extern 	CHAR_DATA *chess_black;

void print_chessboard(CHAR_DATA *ch);
void reset_chessboard ( void );
void move_piece (CHAR_DATA *ch, int col_from, int row_from, int col_to, int row_to);
char *piece_name (int piece);
char num_to_letter (int coord);


char *act_msg(const char *point, CHAR_DATA *ch)
{
	static char buf[MSL];
	int i;

	for (i = 0; i < MSL; i++)
		buf[i] = '\0';

	for (i = 0; *point && *point != '\0'; i++)
	{
		if (*point == '$')
		{
			point++;
			if (*point == 'n')
			{
				strcat(buf,ch->name);
				i += (strlen(ch->name) - 1);
			}
			else if(*point == 'm')
			{
				if(ch->sex == SEX_MALE)
				{
					strcat(buf,"him");
					i += 2;
				}
				else if(ch->sex == SEX_FEMALE)
				{
					strcat(buf,"her");
					i += 2;
				}                                                                                     
				else                                                                                  
				{                                                                                     
					strcat(buf,"it");                                                                 
					i += 1;                                                                           
				}                                                                                     
			}                                                                                         
			else if(*point == 'e')                                                                    
			{                                                                                         
				if(ch->sex == SEX_MALE)                                                               
				{                                                                                     
					strcat(buf,"he");                                                                 
					i += 1;                                                                           
				}                                                                                     
				else if(ch->sex == SEX_FEMALE)                                                        
				{                                                                                     
					strcat(buf,"she");                                                                
					i += 2;                                                                           
				}         
				else
				{
					strcat(buf,"it");
					i += 1;
				}
			}                                                                                         
			else if(*point == 's')                                                                    
			{                                                                                         
				if(ch->sex == SEX_MALE)                                                               
				{                                                                                     
					strcat(buf,"his");                                                                
					i += 2;                                                                           
				}                                                                                     
				else if(ch->sex == SEX_FEMALE)                                                        
				{                                                                                     
					strcat(buf,"her");                                                                
					i += 2;                                                                           
				}                                                                                     
				else                                                                                  
				{                                                                                     
					strcat(buf,"it");                                                                 
					i += 1;                                                                           
				}                                                                                     
			}                                                                                         
			else                                                                                      
			{                                                                                         
				point--;                                                                              
				buf[i] = *point;                                                                      
			}                                                                                         
		}                                                                                             
		else                                                                                          
		{                                                                                             
			buf[i] = *point;                                                                          
		}                                                                                             
		point++;                                                                                      
	}          

	return buf;
}

void do_rngtest(CHAR_DATA *ch, char *argument)
{
	long total = 0;
	int count;
	int rolls, val, maxes = 0;
	char buf[MSL];

	for(count = 0; count < 100000; count++)
		update_db_gold();
	return;

	rolls = number_range(5000,20000);

	for (count = 0; count < rolls; count++) 
	{
		val = number_range(0,4);
		val += 1;
		total += val;
		if(val == 5)
			maxes++;
	}

//	sprintf(buf,"number_percent -- Total: %ld | Number of rolls: %d | Average: %f | Deviation: %f\n\r", total, count, (float)total/(float)count, ((float)total/(float)count) - (float)50);
	sprintf(buf,"number_range -- Total %ld | Number of rolls: %d | Average %f | Deviation %f | Occurances of Max %d (id20)\n\r",total,count,(float)total/(float)count,((float)total/(float)count) - (float)2.5,maxes);
	send_to_char(buf,ch);

	return;
}

int next_sline(SPEECH_DATA *speech)
{
	LINE_DATA *lptr;
	int max=0;
	
	for(lptr=speech->first_line; lptr; lptr = lptr->next)
		if(lptr->number > max)
			max = lptr->number;

	return max + 1;
}

void sort_speech(SPEECH_DATA *speech)
{
	LINE_DATA *lptr, *hold;
	bool sorted=FALSE,first=FALSE;

	while (!sorted)
	{
		sorted = TRUE;

		for(lptr = speech->first_line; lptr; lptr = lptr->next)
		{
			first = FALSE;

			if (!lptr->next)
				break;

			if (lptr == speech->first_line)
				first = TRUE;

			if (lptr->number > lptr->next->number)
			{
				sorted = FALSE;
				if (lptr->next->next)
					lptr->next->next->prev = lptr;
				lptr->next->prev = lptr->prev;
				if (!first)                                                                                       
					lptr->prev->next = lptr->next;                                                              
				lptr->prev = lptr->next;                                                                        
				hold = lptr->next->next;                                                                  
				lptr->next->next = lptr;                                                                        
				lptr->next = hold;                                                                        
				if (first)                                                                                        
					speech->first_line = lptr->prev; 
			}                                                                                                     
		}                                                                                                         
	}                                                                                                             
	return;
}

void BITWISE_OR(long bit1[], const long bit2[])
{
	int i;

	for (i = 0; i < MAX_BITVECTOR; i++)
		bit1[i] |= bit2[i];

	return;
}

void BITWISE_AND(long bit1[], const long bit2[])
{
	int i;
	for (i = 0; i < MAX_BITVECTOR; i++)
		bit1[i] &= bit2[i];

	return;
}

void BITWISE_XAND(long bit1[], const long bit2[])
{
	int i;
	for (i = 0; i < MAX_BITVECTOR; i++)
		bit1[i] &= ~bit2[i];

	return;
}

void BITWISE_XOR(long bit1[], const long bit2[])
{
	int i;

	for (i = 0; i < MAX_BITVECTOR; i++)
		bit1[i] ^= bit2[i];

	return;
}

void zero_vector(long vector[])
{
	int i;

	for (i = 0; i < MAX_BITVECTOR; i++)
		vector[i] = 0;

	return;
}

bool IS_ZERO_VECTOR(long vector[])
{
	int i;

	for (i = 0; i < MAX_BITVECTOR; i++)
		if (vector[i] != 0)
			return FALSE;

	return TRUE;
}

bool vector_equal(long vector1[], long vector2[])
{
	int i;

	for (i = 0; i < MAX_BITVECTOR; i++)
		if (vector1[i] != vector2[i])
			return FALSE;

	return TRUE;
}

void copy_vector(long copy[], const long original[])
{
	int i;
	
	for (i = 0; i < MAX_BITVECTOR; i++)
		copy[i] = original[i];

	return;
}

void fprint_vector(FILE *fp, char *string, long vector[], bool eol)
{
	char buf[MSL], buf2[MSL];
	int i;

	sprintf(buf,string);
	for (i = 0; i < MAX_BITVECTOR; i++) {
		sprintf(buf2," %ld",vector[i]);
		strcat(buf,buf2);
	}
	if (eol)
		strcat(buf,"\n");

	fprintf(fp, buf);

	return;
}

char *int_to_cap_string(int number)
{
	switch (number) {
		case (0):
			return("ZERO");
		case (1):
			return("ONE");
		case (2):
			return("TWO");
		case (3):
			return("THREE");
		case (4):
			return("FOUR");
		case (5):
			return("FIVE");
		case (6):
			return("SIX");
		case (7):
			return("SEVEN");
		case (8):
			return("EIGHT");
		case (9):
			return("NINE");
		case (10):
			return("TEN");
		case (11):
			return("ELEVEN");
		case (12):
			return("TWELVE");
		case (13):
			return("THIRTEEN");
		case (14):
			return("FOURTEEN");
		case (15):
			return("FIFTEEN");
		case (16):
			return("SIXTEEN");
		case (17):
			return("SEVENTEEN");
		case (18):
			return("EIGHTEEN");
		case (19):
			return("NINETEEN");
		case (20):
			return("TWENTY");
		case (30):
			return("THIRTY");
		case (40):
			return("FORTY");
		case (50):
			return("FIFTY");
		case (60):
			return("SIXTY");
		case (70):
			return("SEVENTY");
		case (80):
			return("EIGHT");
		case (90):
			return("NINETY");
		default:
			return("MANY");
	}
}


char *int_to_string(int number)
{
	switch (number) {
		case (0):
			return("zero");
		case (1):
			return("one");
		case (2):
			return("two");
		case (3):
			return("three");
		case (4):
			return("four");
		case (5):
			return("five");
		case (6):
			return("six");
		case (7):
			return("seven");
		case (8):
			return("eight");
		case (9):
			return("nine");
		case (10):
			return("ten");
		case (11):
			return("eleven");
		case (12):
			return("twelve");
		case (13):
			return("thirteen");
		case (14):
			return("fourteen");
		case (15):
			return("fifteen");
		case (16):
			return("sixteen");
		case (17):
			return("seventeen");
		case (18):
			return("eighteen");
		case (19):
			return("nineteen");
		case (20):
			return("twenty");
		case (30):
			return("thirty");
		case (40):
			return("forty");
		case (50):
			return("fifty");
		case (60):
			return("sixty");
		case (70):
			return("seventy");
		case (80):
			return("eighty");
		case (90):
			return("ninety");
		default:
			return("many");
	}
}

void spell_summon_nephilim(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *nephilim;
	int vnum = 2948;
	
	act("$n calls upon the forces of Hell for a servitor!",ch,0,0,TO_ALL);
	act("You call upon the forces of Hell for a servitor.\n\rA gigantic Nephilim appears at your bidding.",ch,0,0,TO_CHAR);
	nephilim = create_mobile(get_mob_index(vnum));
	nephilim->level = level +3;
	nephilim->max_hit = ch->max_hit + dice(level,20);
	nephilim->hit = nephilim->max_hit;
	char_to_room(nephilim,ch->in_room);
	add_follower(nephilim,ch);
	nephilim->leader = ch;
	SET_BIT(nephilim->affected_by,AFF_CHARM);
	return;
}

void do_damdice(CHAR_DATA *ch, char *argument)
{
	int damdice;
	int x;
	int y;
	char mid[MSL];
	char buf[MSL];
	if(!str_cmp(argument,""))
		return send_to_char("Syntax: damdice <dice number> <dice type>\n\r",ch);
	argument = one_argument(argument,mid);
	x = atoi(mid);
	y = atoi(argument);
	damdice = (x + x*y)/2;
	sprintf(buf,"%d",damdice);
	act(buf,ch,0,0,TO_CHAR);
	return;

}

void do_devilfavor(CHAR_DATA *ch, char *argument)
{
	char devil[MAX_INPUT_LENGTH],arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	int i, dev=-1, totalfavor = 0;
	
	if(argument[0] == '\0')
		return send_to_char("Syntax: favor <character> <devil name>\n\r",ch);

	argument = one_argument(argument,arg);
	if((victim=get_char_world(ch,arg)) == NULL)
		return send_to_char("They aren't here.\n\r",ch);
	argument = one_argument(argument,devil);
	if(devil == NULL)
		return send_to_char("Which devil?\n\r",ch);

	if(!str_cmp(devil,"moloch"))
		dev = DEVIL_MOLOCH;
	if(!str_cmp(devil,"asmodeus"))
		dev = DEVIL_ASMODEUS;
	if(!str_cmp(devil,"baal"))
		dev = DEVIL_BAAL;
	if(!str_cmp(devil,"dispater"))
		dev = DEVIL_DISPATER;
	if(!str_cmp(devil,"mephistopheles"))
		dev = DEVIL_MEPHISTO;

	if(dev < 0)
		return send_to_char("That's not a valid devil.\n\r",ch);

	for (i = 0; i < MAX_DEVIL; i++)
		totalfavor += victim->pcdata->devildata[i];

	if (victim->pcdata->devildata[dev] != totalfavor)
		return send_to_char("They already have favor from another devil.\n\r",ch);

	if (victim->pcdata->devildata[dev] == 5)
		return send_to_char("They already have all five levels of favor!\n\r",ch);
	
	victim->pcdata->devildata[dev]++;

	act("You grant $N another level of unholy favor.",ch,0,victim,TO_CHAR);
	act("You feel a piece of your soul torn away as new power surges through your veins!",ch,0,victim,TO_VICT);

	return;
}

void do_chess(CHAR_DATA *ch, char *argument)
{
	char arg1[MSL], arg2[MSL], arg3[MSL], arg4[MSL], buf[MSL];
	int col_from, row_from, col_to, row_to;
	OBJ_DATA *board;
	CHAR_DATA *victim;
	bool found = FALSE;

	argument = one_argument(argument,arg1);
	argument = one_argument(argument,arg2);
	argument = one_argument(argument,arg3);
	argument = one_argument(argument,arg4);
	arg4[2] = '\0';

	if (ch->fighting) {
		send_to_char("You really have more important things to worry about right now.\n\r",ch);
		return;
	}

	if (arg1[0] == '\0') {
		send_to_char("Syntax:\n\r",ch);
		send_to_char("   chess reset\n\r",ch);
		send_to_char("   chess challenge <player>\n\r",ch);
		send_to_char("   chess move <coords-from> <coords-to>\n\r",ch);
		send_to_char("   chess place <color> <piece-type> <coords>\n\r",ch);
		send_to_char("   chess display\n\r",ch);
		send_to_char("\n\rRead HELP CHESS for more details.\n\r",ch);
		return;
	}

	for (board = ch->in_room->contents; board; board = board->next_content) {
		if (board->pIndexData->vnum == OBJ_VNUM_CHESSBOARD) {
			found = TRUE;
			break;
		}
	}

	if (!found)
		return send_to_char("There's no chessboard here!\n\r",ch);
	
	if (!str_cmp(arg1,"reset")) {
		send_to_char("You sweep the pieces off the board and restore them to their starting positions.\n\r",ch);
		act("$n clears the pieces off the board and restores them to their original positions.",ch,0,0,TO_ROOM);
		reset_chessboard();
		return;
	}

	if (!str_cmp(arg1,"challenge")) {
		if ((victim = get_char_room(ch,arg2)) == NULL)
			return send_to_char("Challenge whom?\n\r",ch);
		act("You challenge $N to a game of chess!",ch,0,victim,TO_CHAR);
		act("$n challenges you to a game of chess!",ch,0,victim,TO_VICT);
		act("$n challenges $N to a game of chess!",ch,0,victim,TO_NOTVICT);
		return;
	}

	if (!str_cmp(arg1,"display")) {
		print_chessboard(ch);
		return;
	}

	if (!str_cmp(arg1,"move")
		&& isalpha(arg2[0])
		&& isdigit(arg2[1])
		&& isalpha(arg3[0])
		&& isdigit(arg3[1])) {
		switch (arg2[0]) {
			case ('A'):
			case ('a'):
				col_from = 0;
				break;
			case ('B'):
			case ('b'):
				col_from = 1;
				break;
			case ('C'):
			case ('c'):
				col_from = 2;
				break;
			case ('D'):
			case ('d'):
				col_from = 3;
				break;
			case ('E'):
			case ('e'):
				col_from = 4;
				break;
			case ('F'):
			case ('f'):
				col_from = 5;
				break;
			case ('G'):
			case ('g'):
				col_from = 6;
				break;
			case ('H'):
			case ('h'):
				col_from = 7;
				break;
			default:
				col_from = -1;
				break;
		}

		switch (arg2[1]) {
			case ('1'):
				row_from = 0;
				break;
			case ('2'):
				row_from = 1;
				break;
			case ('3'):
				row_from = 2;
				break;
			case ('4'):
				row_from = 3;
				break;
			case ('5'):
				row_from = 4;
				break;
			case ('6'):
				row_from = 5;
				break;
			case ('7'):
				row_from = 6;
				break;
			case ('8'):
				row_from = 7;
				break;
			default:
				row_from = -1;
				break;
		}

		switch (arg3[0]) {
			case ('A'):
			case ('a'):
				col_to = 0;
				break;
			case ('B'):
			case ('b'):
				col_to = 1;
				break;
			case ('C'):
			case ('c'):
				col_to = 2;
				break;
			case ('D'):
			case ('d'):
				col_to = 3;
				break;
			case ('E'):
			case ('e'):
				col_to = 4;
				break;
			case ('F'):
			case ('f'):
				col_to = 5;
				break;
			case ('G'):
			case ('g'):
				col_to = 6;
				break;
			case ('H'):
			case ('h'):
				col_to = 7;
				break;
			default:
				col_to = -1;
				break;
		}

		switch (arg3[1]) {
			case ('1'):
				row_to = 0;
				break;
			case ('2'):
				row_to = 1;
				break;
			case ('3'):
				row_to = 2;
				break;
			case ('4'):
				row_to = 3;
				break;
			case ('5'):
				row_to = 4;
				break;
			case ('6'):
				row_to = 5;
				break;
			case ('7'):
				row_to = 6;
				break;
			case ('8'):
				row_to = 7;
				break;
			default:
				row_to = -1;
				break;
		}

		move_piece(ch,col_from,row_from,col_to,row_to);
		return;
	}

	if (!str_cmp(arg1,"place")) {
		int color;
		int piece;
		
		if (!str_cmp(arg2,"white"))
			color = WHITE_PIECE;
		else if (!str_cmp(arg2,"black"))
			color = BLACK_PIECE;
		else
			color = -1;

		if (!str_cmp(arg3,"pawn"))
			piece = WHITE_PAWN;
		else if (!str_cmp(arg3,"knight"))
			piece = WHITE_KNIGHT;
		else if (!str_cmp(arg3,"bishop"))
			piece = WHITE_BISHOP;
		else if (!str_cmp(arg3,"rook"))
			piece = WHITE_ROOK;
		else if (!str_cmp(arg3,"queen"))
			piece = WHITE_QUEEN;
		else if (!str_cmp(arg3,"king"))
			piece = WHITE_KING;
		else
			piece = -1;

		if (piece != -1 && color == BLACK_PIECE)
			piece += 6;

		switch (arg4[0]) {
			case ('A'):
			case ('a'):
				col_to = 0;
				break;
			case ('B'):
			case ('b'):
				col_to = 1;
				break;
			case ('C'):
			case ('c'):
				col_to = 2;
				break;
			case ('D'):
			case ('d'):
				col_to = 3;
				break;
			case ('E'):
			case ('e'):
				col_to = 4;
				break;
			case ('F'):
			case ('f'):
				col_to = 5;
				break;
			case ('G'):
			case ('g'):
				col_to = 6;
				break;
			case ('H'):
			case ('h'):
				col_to = 7;
				break;
			default:
				col_to = -1;
				break;
		}

		switch (arg4[1]) {
			case ('1'):
				row_to = 0;
				break;
			case ('2'):
				row_to = 1;
				break;
			case ('3'):
				row_to = 2;
				break;
			case ('4'):
				row_to = 3;
				break;
			case ('5'):
				row_to = 4;
				break;
			case ('6'):
				row_to = 5;
				break;
			case ('7'):
				row_to = 6;
				break;
			case ('8'):
				row_to = 7;
				break;
			default:
				row_to = -1;
				break;
		}

		if (piece != -1 && color != -1) {
			if (chessboard[col_to][row_to] != PIECE_NONE)
				return send_to_char("There's already a piece there!\n\r",ch);

			sprintf(buf,"You place a %s %s on the board at %s.",arg2,arg3,arg4);
			act(buf,ch,0,0,TO_CHAR);
			sprintf(buf,"$n places a %s %s on the board at %s.",arg2,arg3,arg4);
			act(buf,ch,0,0,TO_ROOM);
			chessboard[col_to][row_to] = piece;
			return;
		}
	}

	send_to_char("Syntax:\n\r",ch);
	send_to_char("   chess reset\n\r",ch);
	send_to_char("   chess challenge <player>\n\r",ch);
	send_to_char("   chess move <coords-from> <coords-to>\n\r",ch);
	send_to_char("   chess place <color> <piece-type> <coords>\n\r",ch);
	send_to_char("   chess display\n\r",ch);
	send_to_char("\n\rRead HELP CHESS for more details.\n\r",ch);

	return;
}

void print_chessboard(CHAR_DATA *ch)
{
	extern const   struct  piece_type      piece_table[2][MAX_PIECE];
	char buf[MSL];
	int i, j;
	int square;
	int piece;

	send_to_char("\n\r",ch);
	send_to_char("       A        B        C        D        E        F        G        H\n\r",ch);
	send_to_char("   +--------+--------+--------+--------+--------+--------+--------+--------+\n\r",ch);

	for(i = 0; i < 8; i++) {
		send_to_char("   |",ch);
		for (j = 0; j < 8; j++) {
			if ((i + j) % 2 == 0)
				square = WHITE_SQUARE;
			else
				square = BLACK_SQUARE;
			piece = chessboard[j][7-i];
			if (piece == PIECE_NONE) {
				if (square == WHITE_SQUARE)
					send_to_char("::::::::|",ch);
				else
					send_to_char("        |",ch);
			} else {
				send_to_char(piece_table[square][piece].row1,ch);
				send_to_char("|",ch);
			}
		}
		send_to_char("\n\r",ch);

		sprintf(buf," %d |",8-i);
		send_to_char(buf,ch);

		for (j = 0; j < 8; j++) {
			if ((i + j) % 2 == 0)
				square = WHITE_SQUARE;
			else
				square = BLACK_SQUARE;
			piece = chessboard[j][7-i];
			if (piece == PIECE_NONE) {
				if (square == WHITE_SQUARE)
					send_to_char("::::::::|",ch);
				else
					send_to_char("        |",ch);
			} else {
				send_to_char(piece_table[square][piece].row2,ch);
				send_to_char("|",ch);
			}
		}
		sprintf(buf," %d\n\r",8-i);
		send_to_char(buf,ch);

		send_to_char("   |",ch);
		for (j = 0; j < 8; j++) {
			if ((i + j) % 2 == 0)
				square = WHITE_SQUARE;
			else
				square = BLACK_SQUARE;
			piece = chessboard[j][7-i];
			if (piece == PIECE_NONE) {
				if (square == WHITE_SQUARE)
					send_to_char("::::::::|",ch);
				else
					send_to_char("        |",ch);
			} else {
				send_to_char(piece_table[square][piece].row3,ch);
				send_to_char("|",ch);
			}
		}
		send_to_char("\n\r",ch);

		send_to_char("   +--------+--------+--------+--------+--------+--------+--------+--------+\n\r",ch);
	}

	send_to_char("       A        B        C        D        E        F        G        H\n\r",ch);

	return;
}

void reset_chessboard ( void )
{
	int i, j;

	chessboard[0][0] = WHITE_ROOK;
	chessboard[1][0] = WHITE_KNIGHT;
	chessboard[2][0] = WHITE_BISHOP;
	chessboard[3][0] = WHITE_QUEEN;
	chessboard[4][0] = WHITE_KING;
	chessboard[5][0] = WHITE_BISHOP;
	chessboard[6][0] = WHITE_KNIGHT;
	chessboard[7][0] = WHITE_ROOK;
	
	for (i=0;i < 8; i++)
		chessboard[i][1] = WHITE_PAWN;

	for (j=2;j < 6; j++)
		for (i=0;i < 8; i++)
			chessboard[i][j] = PIECE_NONE;

	for (i=0;i < 8; i++)
		chessboard[i][6] = BLACK_PAWN;

	chessboard[0][7] = BLACK_ROOK;
	chessboard[1][7] = BLACK_KNIGHT;
	chessboard[2][7] = BLACK_BISHOP;
	chessboard[3][7] = BLACK_QUEEN;
	chessboard[4][7] = BLACK_KING;
	chessboard[5][7] = BLACK_BISHOP;
	chessboard[6][7] = BLACK_KNIGHT;
	chessboard[7][7] = BLACK_ROOK;

	return;
}

void move_piece (CHAR_DATA *ch, int col_from, int row_from, int col_to, int row_to)
{
	char buf[MSL];
	int piece_from, piece_to;
		
	if (col_from < 0 || col_from > 7
		|| col_to < 0 || col_to > 7
		|| row_from < 0 || row_from > 7
		|| row_to < 0 || row_to > 7)
		return send_to_char("Illegal move.\n\r",ch);
	
	piece_from = chessboard[col_from][row_from];
	piece_to = chessboard[col_to][row_to];

	if (piece_from == PIECE_NONE)
		return send_to_char("Move what...?\n\r",ch);

	if ((IS_WHITE(piece_to) && IS_WHITE(piece_from))
		|| (IS_BLACK(piece_to) && IS_BLACK(piece_from)))
		return send_to_char("You already have a piece there!\n\r",ch);

	chessboard[col_to][row_to] = piece_from;
	chessboard[col_from][row_from] = PIECE_NONE;
	
	sprintf(buf,"You move the %s %s from %c%d to %c%d.\n\r",
		(IS_WHITE(piece_from)) ? "white" : "black",
		piece_name(piece_from),
		num_to_letter(col_from),
		row_from + 1,
		num_to_letter(col_to),
		row_to + 1);

	send_to_char(buf,ch);
	
	sprintf(buf,"$n moves the %s %s from %c%d to %c%d.",
		(IS_WHITE(piece_from)) ? "white" : "black",
		piece_name(piece_from),
		num_to_letter(col_from),
		row_from + 1,
		num_to_letter(col_to),
		row_to + 1);

	act(buf,ch,0,0,TO_ROOM);

	if (piece_to == PIECE_NONE)
		return;

	sprintf(buf,"You capture the %s %s.\n\r",
		(IS_WHITE(piece_to)) ? "white" : "black",
		piece_name(piece_to));

	send_to_char(buf,ch);

	sprintf(buf,"$n captures the %s %s.",
		(IS_WHITE(piece_to)) ? "white" : "black",
		piece_name(piece_to));

	act(buf,ch,0,0,TO_ROOM);
				
	return;
}

char *piece_name (int piece)
{
	switch (piece){
		case(WHITE_PAWN):
		case(BLACK_PAWN):
			return ("pawn");
		case(WHITE_KNIGHT):
		case(BLACK_KNIGHT):
			return ("knight");
		case(WHITE_BISHOP):
		case(BLACK_BISHOP):
			return ("bishop");
		case(WHITE_ROOK):
		case(BLACK_ROOK):
			return ("rook");
		case(WHITE_QUEEN):
		case(BLACK_QUEEN):
			return ("queen");
		case(WHITE_KING):
		case(BLACK_KING):
			return ("king");
		default:
			return ("[ERROR]");
	}
}

char num_to_letter (int coord)
{
	switch (coord) {
		case(0):
			return 'A';
		case(1):
			return 'B';
		case(2):
			return 'C';
		case(3):
			return 'D';
		case(4):
			return 'E';
		case(5):
			return 'F';
		case(6):
			return 'G';
		case(7):
			return 'H';
		default:
			return '?';
	}
}

void do_diku (CHAR_DATA *ch, char *argument)
{
	send_to_char("                    Original game idea, concept, and design:\n\r\n\r",ch);
	send_to_char("          Katja Nyboe               [Superwoman] (katz@freja.diku.dk)\n\r",ch);
	send_to_char("          Tom Madsen              [Stormbringer] (noop@freja.diku.dk)\n\r",ch);
	send_to_char("          Hans Henrik Staerfeldt           [God] (bombman@freja.diku.dk)\n\r",ch);
	send_to_char("          Michael Seifert                 [Papi] (seifert@freja.diku.dk)\n\r",ch);
	send_to_char("          Sebastian Hammer               [Quinn] (quinn@freja.diku.dk)\n\r",ch);
	send_to_char("\n\r                     Additional contributions from:\n\r\n\r",ch);
	send_to_char("Michael Curran  - the player title collection and additional locations.\n\r",ch);
	send_to_char("Ragnar Loenn    - the bulletin board.\n\r",ch);
	send_to_char("Bill Wisner     - for being the first to successfully port the game,\n\r",ch);
	send_to_char("                  uncovering several old bugs, uh, inconsistencies,\n\r",ch);
	send_to_char("                  in the process.\n\r",ch);
	send_to_char("\n\rAnd: Mads Haar and Stephan Dahl for additional locations.\n\r\n\r",ch);
	send_to_char("Developed at: DIKU -- The Department of Computer Science\n\r",ch);
	send_to_char("                      at the University of Copenhagen.\n\r",ch);

	return;
}

void do_antiidle(CHAR_DATA *ch, char *arg)
{
	AFFECT_DATA af;
	
	if(is_affected(ch,gsn_trip)) {
		affect_strip(ch,gsn_trip);
		send_to_char("Anti-idle disabled.\n\r",ch);
		return;
	}
	
	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = gsn_trip;
	af.aftype = AFT_INVIS;
	af.duration = -1;
	SET_BIT(af.bitvector,AFF_PERMANENT);
	af.modifier = 0;
	af.pulse_fun = idle_pulse;
	affect_to_char(ch,&af);
	send_to_char("Anti-idle enabled.\n\r",ch);
	
	return;
}

void idle_pulse(CHAR_DATA *ch, AFFECT_DATA *af)
{
	if(number_percent() < 99)
		return;

	send_to_char("\n\r",ch);
	return;
}

