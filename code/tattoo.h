#ifndef TATTOO_H
#define TATTOO_H

#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "merc.h"
#include "recycle.h"
#include "tables.h"
#include "lookup.h"
#include "comm.h"
#include "interp.h"
#include "db.h"
#include "utility.h"

//
// TODO: UNKNOWN FUNCTIONS
//

// void do_invoke_dioxide (CHAR_DATA *ch);
// void do_invoke_seracuz (CHAR_DATA *ch);
// void do_invoke_butter (CHAR_DATA *ch);
// void do_invoke_jhyrbian (CHAR_DATA *ch);
// void do_invoke_dev (CHAR_DATA *ch, char *argument);
// void do_invoke_daedelex (CHAR_DATA *ch);
// void do_invoke_xanthrailles (CHAR_DATA *ch);
// void do_invoke_iomahk (CHAR_DATA *ch);
// void do_invoke_lestregus (CHAR_DATA *ch);
// void do_invoke_kyrissaean (CHAR_DATA *ch);
// void do_invoke_lichen (CHAR_DATA *ch);

//
// LOCAL FUNCTIONS
//

void do_brands (CHAR_DATA *ch, char *argument);
void do_unbrands (CHAR_DATA *ch, char *argument);
void do_invoke (CHAR_DATA *ch, char *argument);
void do_invoke_jackass (CHAR_DATA *ch );
/*
void do_invoke_detlef (CHAR_DATA *ch);
*/
void do_invoke_gabe (CHAR_DATA *ch);

#endif /* TATTOO_H */
