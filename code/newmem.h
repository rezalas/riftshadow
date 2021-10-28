#ifndef NEWMEM_H
#define NEWMEM_H

#include "merc.h"


//
// LOCAL FUNCTIONS
//

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
