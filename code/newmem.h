#ifndef NEWMEM_H
#define NEWMEM_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <ctype.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>
#include "merc.h"
#include "db.h"
#include "recycle.h"
#include "lookup.h"
#include "tables.h"
#include "update.h"
#include "interp.h"

//#define free public_fREe
//tweakable parameters
#define TEMP_STRING_MEM_SIZE		320000
#define TEMP_STRUCT_MEM_SIZE		100000
#define PERM_STRING_LIST_CHUNK		30000
#define PERM_STRING_CHUNK_SIZE		131072
#define PERM_STRING_MAX_SIZE		3500000
#define EMERGENCY_DISABLE_TALLOC	FALSE

typedef struct string_entry STRING_ENTRY;
typedef struct mallinfo MALLOC_INFO;

extern int nAllocString; 
extern int sAllocString;
extern int nAllocPerm;
extern int sAllocPerm;
extern bool bDebug;

char *talloc_string (const char *str);
void *talloc_struct (long nStructSize);
char *palloc_string (const char *str);
/*
void *palloc_struct (long nStructSize);
*/
void free_pstring (char *pstr);
/*
void free_pstruct (void *pstruct);
void *calloc_struct (long nStructSize);
*/

//Internal functions.  DO NOT CALL ANYWHERE ELSE [or die!].
void *init_temp_struct_mem (void);
void *init_temp_string_mem (void);
void do_memtest (CHAR_DATA *ch, char *argument);

#endif /* NEWMEM_H */
