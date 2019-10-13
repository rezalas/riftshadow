#ifndef MEM_H
#define MEM_H

#ifdef macintosh
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

/*
 * Globals
 */
extern int top_reset;
extern int top_area;
extern int top_exit;
extern int top_ed;
extern int top_room;
extern EXTRA_DESCR_DATA	* extra_descr_free;
extern HELP_DATA * help_last;
extern AFFECT_DATA *affect_free;

void free_extra_descr (EXTRA_DESCR_DATA *pExtra);
void free_affect (AFFECT_DATA *af);
RESET_DATA *new_reset_data (void);
void free_reset_data (RESET_DATA *pReset);
AREA_DATA *new_area (void);
void free_area (AREA_DATA *pArea);
EXIT_DATA *new_exit (void);
void free_exit (EXIT_DATA *pExit);
/*
EXTRA_DESCR_DATA *new_extra_descr (void);
void free_extra_descr (EXTRA_DESCR_DATA *pExtra);
*/
ROOM_INDEX_DATA *new_room_index (void);
/*
AFFECT_DATA *new_affect (void);
void free_affect (AFFECT_DATA* pAf);
*/
SHOP_DATA *new_shop (void);
void free_shop (SHOP_DATA *pShop);
OBJ_INDEX_DATA *new_obj_index (void);
void free_obj_index (OBJ_INDEX_DATA *pObj);
MOB_INDEX_DATA *new_mob_index (void);
void free_mob_index (MOB_INDEX_DATA *pMob);

#endif /* MEM_H */
