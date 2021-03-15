#ifndef OLC_SAVE_H
#define OLC_SAVE_H

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
#include "handler.h"
#include "olc.h"
#include "tables.h"
#include "lookup.h"
#include "update.h"
#include "interp.h"
#include "spec.h"
#include "db.h"
#include "act_comm.h"
#include "chardef.h"
#include "const.h"
#include "material.h"

/*
 *  Verbose writes reset data in plain english into the comments
 *  section of the resets.  It makes areas considerably larger but
 *  may aid in debugging.
 */

/* #define VERBOSE */

//
// LOCAL FUNCTIONS
//

/*****************************************************************************
 Name:		fix_string
 Purpose:	Returns a string without \r and ~.
 ****************************************************************************/
char *fix_string (const char *str);
/*
 * Name:	clean_olc_aflags
 * Purpose:	Removes the olc flags before saving the flags to the file.
 * Called by: do_asave(olc_save.c)
 */
void clean_olc_aflags (AREA_DATA *pArea);
/*****************************************************************************
 Name:		save_area_list
 Purpose:	Saves the listing of files to be loaded at startup.
 Called by:	do_asave(olc_save.c).
 ****************************************************************************/
void save_area_list (void);
/*
 * ROM OLC
 * Used in save_mobile and save_object below.  Writes
 * flags on the form fread_flag reads.
 *
 * buf[] must hold at least 32+1 characters.
 *
 * -- Hugin
 */
char *fwrite_flag (long flags, char buf[]);
/*****************************************************************************
 Name:		save_mobile
 Purpose:	Save one mobile to file, new format -- Hugin
 Called by:	save_mobiles (below).
 ****************************************************************************/
void save_mobile (FILE *fp, MOB_INDEX_DATA *pMobIndex);
/*****************************************************************************
 Name:		save_mobiles
 Purpose:	Save #MOBILES secion of an area file.
 Called by:	save_area(olc_save.c).
 Notes:         Changed for ROM OLC.
 ****************************************************************************/
void save_mobiles (FILE *fp, AREA_DATA *pArea);
/*****************************************************************************
 Name:		save_object
 Purpose:	Save one object to file.
                new ROM format saving -- Hugin
 Called by:	save_objects (below).
 ****************************************************************************/
void save_object (FILE *fp, OBJ_INDEX_DATA *pObjIndex);
/*****************************************************************************
 Name:		save_objects
 Purpose:	Save #OBJECTS section of an area file.
 Called by:	save_area(olc_save.c).
 Notes:         Changed for ROM OLC.
 ****************************************************************************/
void save_objects (FILE *fp, AREA_DATA *pArea);
/*****************************************************************************
 Name:		save_rooms
 Purpose:	Save #ROOMS section of an area file.
 Called by:	save_area(olc_save.c).
 ****************************************************************************/
void save_rooms (FILE *fp, AREA_DATA *pArea);
/*****************************************************************************
 Name:		save_progs
 Purpose:	Save #IPROGS section of area file.
 Called by:	save_area(olc_save.c).
 ****************************************************************************/
void save_progs (FILE *fp, AREA_DATA *pArea);
void save_specs (FILE *fp, AREA_DATA *pArea);
/*****************************************************************************
 Name:		save_resets
 Purpose:	Saves the #RESETS section of an area file.
 Called by:	save_area(olc_save.c)
 ****************************************************************************/
void save_resets (FILE *fp, AREA_DATA *pArea);
/*****************************************************************************
 Name:		save_shops
 Purpose:	Saves the #SHOPS section of an area file.
 Called by:	save_area(olc_save.c)
 ****************************************************************************/
void save_shops (FILE *fp, AREA_DATA *pArea);
/*****************************************************************************
 Name:		save_area
 Purpose:	Save an area, note that this format is new.
 Called by:	do_asave(olc_save.c).
 ****************************************************************************/
void save_area (AREA_DATA *pArea);
/*****************************************************************************
 Name:		do_asave
 Purpose:	Entry point for saving area data.
 Called by:	interpreter(interp.c)
 ****************************************************************************/
void do_asave (CHAR_DATA *ch, char *argument);

#endif /* OLC_SAVE_H */
