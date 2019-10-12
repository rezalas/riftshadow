#ifndef FLAGS_H
#define FLAGS_H

#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "merc.h"
#include "tables.h"

int flag_lookup (const char *name, const struct flag_type *flag_table);
void do_flag (CHAR_DATA *ch, char *argument);

#endif /* FLAGS_H */
