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

#define MAX_NEST	100

#ifndef KEY
#define KEY( literal, field, value )                \
                if ( !str_cmp( word, literal ) )    \
                {                                   \
                    field  = value;                 \
                    fMatch = true;                  \
                    break;                          \
                }
#endif

/* provided to free strings */
#ifndef KEYS
#define KEYS( literal, field, value )               \
                if ( !str_cmp( word, literal ) )    \
                {                                   \
                    free_pstring(field);            \
                    field  = value;                 \
                    fMatch = true;                  \
                    break;                          \
                }
#endif

#ifndef KEYV
#define KEYV( literal, field )                      \
                if ( !str_cmp( word, literal ) )    \
                {                                   \
                    fread_flag_new(field, fp);      \
                    fMatch = true;                  \
                    break;                          \
                }
#endif

extern int _filbuf (FILE *);
extern int mPort;
extern	OBJ_DATA	*obj_free;

int rename (const char *oldfname, const char *newfname);
bool check_parse_name (char *name);
char *print_flags (long flag[]);


/*
 * Local functions.
 */
void fwrite_char (CHAR_DATA *ch,  FILE *fp);
void fwrite_obj	(CHAR_DATA *ch,  OBJ_DATA  *obj, FILE *fp, int iNest);
void fwrite_pet	(CHAR_DATA *pet, FILE *fp);
void fwrite_charmie (CHAR_DATA *ch, FILE *fp);
void fread_char (CHAR_DATA *ch,  FILE *fp);
void fread_pet (CHAR_DATA *ch,  FILE *fp);
void fread_obj (CHAR_DATA *ch,  FILE *fp);
void fread_charmie (CHAR_DATA *ch, FILE *fp);


int isAftSpell (int aftype);
/*
 * Save a character and inventory.
 * Would be cool to save NPC's too for quest purposes,
 *   some of the infrastructure is provided.
 */
void save_char_obj (CHAR_DATA *ch);
/*
 * Write the char.
 */
void fwrite_charmie (CHAR_DATA *ch, FILE *fp);
void fread_charmie (CHAR_DATA *ch, FILE *fp);
void fwrite_char (CHAR_DATA *ch, FILE *fp);
/* write a pet */
void fwrite_pet ( CHAR_DATA *pet, FILE *fp);
/*
 * Write an object and its contents.
 */
void fwrite_obj (CHAR_DATA *ch, OBJ_DATA *obj, FILE *fp, int iNest);
/*
 * Load a char and inventory into a new ch structure.
 */
bool load_char_obj (DESCRIPTOR_DATA *d, char *name );
/*
 * Read in a char.
 */
void fread_char (CHAR_DATA *ch, FILE *fp );
/* load a pet from the forgotten reaches */
void fread_pet (CHAR_DATA *ch, FILE *fp );
void fread_obj (CHAR_DATA *ch, FILE *fp );

#endif /* SAVE_H */
