#ifndef SAVE_H
#define SAVE_H

#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "recycle.h"
#include "lookup.h"
#include "tables.h"
#include "act_comm.h"
#include "act_wiz.h"
#include "skills.h"
#include "db.h"
#include "devextra.h"
#include "magic.h"
#include "dioextra.h"
#include "comm.h"

#define MAX_NEST	100

extern int _filbuf (FILE *);
extern int mPort;
extern	OBJ_DATA *obj_free;


//
// LOCAL FUNCTIONS
//

char *print_flags (long flag[]);
int isAftSpell (int aftype);
/*
 * Save a character and inventory.
 * Would be cool to save NPC's too for quest purposes,
 *   some of the infrastructure is provided.
 */
void save_char_obj (CHAR_DATA *ch);
void fwrite_charmie (CHAR_DATA *ch, FILE *fp);
void fread_charmie (CHAR_DATA *ch, FILE *fp);
void fwrite_char (CHAR_DATA *ch,  FILE *fp);
void fwrite_pet	(CHAR_DATA *pet, FILE *fp);
void fwrite_obj	(CHAR_DATA *ch,  OBJ_DATA  *obj, FILE *fp, int iNest);
/*
 * Load a char and inventory into a new ch structure.
 */
bool load_char_obj (DESCRIPTOR_DATA *d, char *name );
void fread_char (CHAR_DATA *ch,  FILE *fp);
void fread_pet (CHAR_DATA *ch,  FILE *fp);
void fread_obj (CHAR_DATA *ch,  FILE *fp);

#endif /* SAVE_H */
