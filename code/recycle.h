/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,	   *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *									   *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael	   *
 *  Chastain, Michael Quan, and Mitchell Tse.				   *
 *									   *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc	   *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.						   *
 *									   *
 *  Much time and thought has gone into this software and you are	   *
 *  benefitting.  We hope that you share your changes too.  What goes	   *
 *  around, comes around.						   *
 ***************************************************************************/
 
/***************************************************************************
*	ROM 2.4 is copyright 1993-1996 Russ Taylor			   *
*	ROM has been brought to you by the ROM consortium		   *
*	    Russ Taylor (rtaylor@efn.org)				   *
*	    Gabrielle Taylor						   *
*	    Brian Moore (zump@rom.org)					   *
*	By using this code, you have agreed to follow the terms of the	   *
*	ROM license, in the file Tartarus/doc/rom.license                  *
***************************************************************************/

/***************************************************************************
*       Tartarus code is copyright (C) 1997-1998 by Daniel Graham          *
*	In using this code you agree to comply with the Tartarus license   *
*       found in the file /Tartarus/doc/tartarus.doc                       *
***************************************************************************/

/* externs */
extern char str_empty[1];
extern int mobile_count;

/* stuff for providing a crash-proof buffer */

#define MAX_BUF		16384
#define MAX_BUF_LIST 	10
#define BASE_BUF 	1024

/* valid states */
#define BUFFER_SAFE	0
#define BUFFER_OVERFLOW	1
#define BUFFER_FREED 	2

RUNE_DATA *new_rune args ( ( void ) );
void	  free_rune args( ( RUNE_DATA *rune) );
/* note recycling */
#define ND NOTE_DATA
ND	*new_note args( (void) );
void	free_note args( (NOTE_DATA *note) );
#undef ND

/* ban data recycling */
#define BD BAN_DATA
BD	*new_ban args( (void) );
void	free_ban args( (BAN_DATA *ban) );
#undef BD

/* descriptor recycling */
#define DD DESCRIPTOR_DATA
DD	*new_descriptor args( (void) );
void	free_descriptor args( (DESCRIPTOR_DATA *d) );
#undef DD

/* char gen data recycling */
#define GD GEN_DATA
GD 	*new_gen_data args( (void) );
void	free_gen_data args( (GEN_DATA * gen) );
#undef GD

/* extra descr recycling */
#define ED EXTRA_DESCR_DATA
ED	*new_extra_descr args( (void) );
void	free_extra_descr args( (EXTRA_DESCR_DATA *ed) );
#undef ED

/* trophy recycling */
#define TD TROPHY_DATA
TD *new_trophy_data args( (char *victname) ) ;
void	free_trophy args( ( TROPHY_DATA *trophy ) );
#undef TD

/* race_data recycling (never gonna happen!) */
#define RD RACE_DATA
RD *new_race_data args( (void) );
void	free_race_data args( ( RACE_DATA *race_specs ) );
#undef RD

/* tracks recycling */
#define TD TRACK_DATA
TD *new_track_data args( (void) ) ;
void free_track args( ( TRACK_DATA *tracks ) );
#undef TD

/* pathfind recycling */
#define PD PATHFIND_DATA
PD *new_path_data args( (void) );
void free_path args( ( PD *path) );
#undef PD

/* trap recycling */
#define TD TRAP_DATA
TD *new_trap args( (void) );
void free_trap args( ( TD*trap) );
#undef TD

/* apply recycling */
#define OAD OBJ_APPLY_DATA
OAD *new_apply_data args( (void) );
void free_apply args( ( OAD *apply) );
#undef OAD 

/* affect recycling */
#define AD AFFECT_DATA
AD	*new_affect args( (void) );
void	free_affect args( (AD *af) );
#undef AD

#define OAD OBJ_AFFECT_DATA
OAD *new_affect_obj args ((void));
void free_affect_obj args ((OAD *af));
#undef OAD

#define AAD AREA_AFFECT_DATA
AAD *new_affect_area args ((void));
void free_affect_area args ((AAD *af));
#undef AAD

/* object recycling */
#define OD OBJ_DATA
OD	*new_obj args( (void) );
void	free_obj args( (OBJ_DATA *obj) );
#undef OD

/* character recyling */
#define CD CHAR_DATA
#define PD PC_DATA
#define OC OLD_CHAR
CD	*new_char args( (void) );
void	free_char args( (CHAR_DATA *ch) );
PD	*new_pcdata args( (void) );
void	free_pcdata args( (PC_DATA *pcdata) );
OC	*new_oldchar args((void));
void	free_oldchar args( (OLD_CHAR *old) );

#undef PD
#undef CD
#undef OC

/* speech recycling */
#define SD SPEECH_DATA
#define LD LINE_DATA

SD *new_speech_data args( (void) );
void free_speech args( (SPEECH_DATA *speech) );
void free_line args( (LINE_DATA *line) );
LD *new_line_data args( (void) );

#undef LD
#undef SD

/* mob id and memory procedures */
#define MD MEM_DATA
long 	get_pc_id args( (void) );
long	get_mob_id args( (void) );
MD	*new_mem_data args( (void) );
void	free_mem_data args( ( MEM_DATA *memory) );
MD	*find_memory args( (MEM_DATA *memory, long id) );
#undef MD

/* buffer procedures */

BUFFER	*new_buf args( (void) );
void	free_buf args( (BUFFER *buffer) );
bool	add_buf args( (BUFFER *buffer, char *string) );
void	clear_buf args( (BUFFER *buffer) );
char	*buf_string args( (BUFFER *buffer) );
