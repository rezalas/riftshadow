 /**************************************************************************
 *  File: olc_save.c                   *
 *         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.              *
 *         *
 *  This code was freely distributed with the The Isles 1.1 source code,   *
 *  and has been used here for OLC - OLC would not be what it is without   *
 *  all the previous coders who released their source code.                *
 *         *
 ***************************************************************************/
/* OLC_SAVE.C
 * This takes care of saving all the .are information.
 * Notes:
 * -If a good syntax checker is used for setting vnum ranges of areas
 *  then it would become possible to just cycle through vnums instead
 *  of using the iHash stuff and checking that the room or reset or
 *  mob etc is part of that area.
 */

#if defined(macintosh)
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
#include "olc.h"
#include "tables.h"
#include "lookup.h"
#include "update.h"
#include "interp.h"
#include "spec.h"

char * flag_name_lookup args((long bitv, const struct flag_type *flag_table));
char * bitvector_to_flag args((long bitvect[]));
char * munch(char *str);
char * display_name_lookup args((long bitv, const struct display_type *flag_table));
char * restrict_name_lookup args((long bitv));
/*
 *  Verbose writes reset data in plain english into the comments
 *  section of the resets.  It makes areas considerably larger but
 *  may aid in debugging.
 */

/* #define VERBOSE */

/*****************************************************************************
 Name:		fix_string
 Purpose:	Returns a string without \r and ~.
 ****************************************************************************/
char *fix_string( const char *str )
{
    static char strfix[MAX_STRING_LENGTH];
    int i;
    int o;

    if ( str == NULL )
        return '\0';

    for ( o = i = 0; str[i+o] != '\0'; i++ )
    {
        if (str[i+o] == '\r' || str[i+o] == '~')
            o++;
        strfix[i] = str[i+o];
    }
    strfix[i] = '\0';
    return strfix;
}


/*
 * Name:	clean_olc_aflags
 * Purpose:	Removes the olc flags before saving the flags to the file.
 * Called by: do_asave(olc_save.c)
 */
void clean_olc_aflags(AREA_DATA *pArea)
{
	REMOVE_BIT( pArea->area_flags, AREA_CHANGED );
	REMOVE_BIT( pArea->area_flags, AREA_LOADING );
	REMOVE_BIT( pArea->area_flags, AREA_ADDED);
}


/*****************************************************************************
 Name:		save_area_list
 Purpose:	Saves the listing of files to be loaded at startup.
 Called by:	do_asave(olc_save.c).
 ****************************************************************************/
void save_area_list()
{
    FILE *fp;
    AREA_DATA *pArea;

    if ( ( fp = fopen( "area.lst", "w" ) ) == NULL )
    {
		bug( "Save_area_list: fopen", 0 );
		perror( "area.lst" );
    }
    else
    {
		for( pArea = area_first; pArea; pArea = pArea->next )
		{
	    	fprintf( fp, "%s\n", pArea->file_name );
		}

		fprintf( fp, "social.are\n");
		fprintf( fp, "$\n" );
		fclose( fp );
    }

    return;
}


/*
 * ROM OLC
 * Used in save_mobile and save_object below.  Writes
 * flags on the form fread_flag reads.
 *
 * buf[] must hold at least 32+1 characters.
 *
 * -- Hugin
 */
char *fwrite_flag( long flags, char buf[] )
{
    char offset;
    char *cp;

    buf[0] = '\0';

    if ( flags == 0 )
    {
	strcpy( buf, "0" );
	return buf;
    }

    /* 32 -- number of bits in a long */

    for ( offset = 0, cp = buf; offset < 32; offset++ )
	if ( flags & ( (long)1 << offset ) )
	{
	    if ( offset <= 'Z' - 'A' )
		*(cp++) = 'A' + offset;
	    else
		*(cp++) = 'a' + offset - ( 'Z' - 'A' + 1 );
	}

    *cp = '\0';

    return buf;
}


/*****************************************************************************
 Name:		save_mobile
 Purpose:	Save one mobile to file, new format -- Hugin
 Called by:	save_mobiles (below).
 ****************************************************************************/
void save_mobile( FILE *fp, MOB_INDEX_DATA *pMobIndex )
{
	long i;
	int styles;
	char buf[MSL], buf2[MSL];
	long dummy[MAX_BITVECTOR], dummy2[MAX_BITVECTOR];
	
fprintf(fp,"#%d\n", pMobIndex->vnum);
fprintf(fp,"%s~\n", pMobIndex->player_name);
fprintf(fp,"%s~\n%s~\n", pMobIndex->short_descr,
munch(pMobIndex->long_descr));
fprintf(fp,"%s~\n", munch(pMobIndex->description));
fprintf(fp,"%d %d %d %s~\n", pMobIndex->alignment, pMobIndex->group,
pMobIndex->xp_mod, race_table[pMobIndex->race].name);
fprintf(fp,"%d %dd%d+%d %dd%d+%d %d %d %s\n", pMobIndex->level,
    pMobIndex->hit[DICE_NUMBER], pMobIndex->hit[DICE_TYPE],
    pMobIndex->hit[DICE_BONUS], pMobIndex->damage[DICE_NUMBER],
    pMobIndex->damage[DICE_TYPE], pMobIndex->damage[DICE_BONUS],
    pMobIndex->hitroll, (int)pMobIndex->dam_mod,
    attack_table[pMobIndex->dam_type].name);
    fprintf(fp,"%d %d %d %d\n", pMobIndex->ac[AC_PIERCE], pMobIndex->ac[AC_BASH],
    pMobIndex->ac[AC_SLASH], pMobIndex->ac[AC_EXOTIC]);
    fprintf(fp,"%s %s ", position_table[UMAX(pMobIndex->start_pos,0)].name,
    sex_table[UMAX(pMobIndex->sex,1)].name);
	fprintf(fp,"%s\n", wealth_lookup(pMobIndex->wealth));
	
    copy_vector(dummy, pMobIndex->form);
	BITWISE_XAND(dummy, race_data_lookup(pMobIndex->race)->form);
	copy_vector(dummy2, pMobIndex->parts);
	BITWISE_XAND(dummy2, race_data_lookup(pMobIndex->race)->parts);
	
	fprintf(fp,"%s %s %s\n", bitvector_to_flag(dummy), bitvector_to_flag(dummy2),
    size_table[UMAX(pMobIndex->size,0)].name);
	
	for(i = 0; i < (32 * MAX_BITVECTOR); i++)
		if(IS_SET(pMobIndex->act, i) && !IS_SET(race_data_lookup(pMobIndex->race)->act, i))
			fprintf(fp,"ACT %s\n",(upstring(flag_name_lookup(i, act_flags))));
	for(i = 0; i < (32 * MAX_BITVECTOR); i++)
		if(IS_SET(pMobIndex->off_flags,i) && !IS_SET(race_data_lookup(pMobIndex->race)->off,i))
			fprintf(fp,"OFF %s\n",(upstring(flag_name_lookup(i, off_flags))));
	for(i = 0; i < (32 * MAX_BITVECTOR); i++)
		if(IS_SET(pMobIndex->affected_by, i) && !IS_SET(race_data_lookup(pMobIndex->race)->aff,i))
			fprintf(fp,"AFF %s\n",(upstring(flag_name_lookup(i, affect_flags))));
	for(i = 0; i < (32 * MAX_BITVECTOR); i++)
		if(IS_SET(pMobIndex->imm_flags, i) && !IS_SET(race_data_lookup(pMobIndex->race)->imm,i))
			fprintf(fp,"IMM %s\n",(upstring(flag_name_lookup(i, imm_flags))));
	for(i = 0; i < (32 * MAX_BITVECTOR); i++)
		if(IS_SET(pMobIndex->res_flags, i) && !IS_SET(race_data_lookup(pMobIndex->race)->res,i))
			fprintf(fp,"RES %s\n",(upstring(flag_name_lookup(i, imm_flags))));
	for(i = 0; i < (32 * MAX_BITVECTOR); i++)
		if(IS_SET(pMobIndex->vuln_flags, i) && !IS_SET(race_data_lookup(pMobIndex->race)->vuln,i))
			fprintf(fp,"VUL %s\n",(upstring(flag_name_lookup(i, imm_flags))));
	if (pMobIndex->Class()->GetIndex() != CLASS_NONE) {
		fprintf(fp,"CLASS %s ", (RSTR)pMobIndex->Class()->name);
		switch(pMobIndex->Class()->GetIndex()) {
			case(CLASS_WARRIOR):
				styles = 0;
				for(i=1; i<MAX_STYLE; i++)
					if(IS_SET(pMobIndex->styles, style_table[i].bit)) {
						styles++;
						fprintf(fp, "%s ",style_table[i].name);
					}
				for(;styles < 2; styles++) {
					fprintf(fp, "none ");
				}
				break;
			case(CLASS_SORCERER):
				fprintf(fp,"%s %s",sphere_table[pMobIndex->ele_major].name,
					sphere_table[pMobIndex->ele_para].name);
				break;
		}
		fprintf(fp,"\n");
	}
	if(pMobIndex->restrict_low != -1 && pMobIndex->restrict_high != 65535)
		fprintf(fp,"LIMIT %d %d\n",pMobIndex->restrict_low,pMobIndex->restrict_high);
	if(pMobIndex->attack_yell)
		fprintf(fp,"YELL %s~\n", pMobIndex->attack_yell);
	if(pMobIndex->pShop)
		fprintf(fp,"SHOPKEEPER OPEN %d CLOSE %d EXIT %s\n", pMobIndex->pShop->open_hour, pMobIndex->pShop->close_hour,
				direction_table[pMobIndex->pShop->direction].name);
	if(pMobIndex->cabal)
		fprintf(fp,"CABAL %s\n", cabal_table[pMobIndex->cabal].name);
	if (pMobIndex->notes)
		fprintf(fp,"NOTES %s~\n", pMobIndex->notes);
	if (pMobIndex->barred_entry) {
		switch(pMobIndex->barred_entry->comparison) {
			case(BAR_EQUAL_TO):
				sprintf(buf,"EQUALTO");
				break;
			case(BAR_LESS_THAN):
				sprintf(buf,"LESSTHAN");
				break;
			case(BAR_GREATER_THAN):
				sprintf(buf,"GREATERTHAN");
				break;
		}
		switch(pMobIndex->barred_entry->msg_type) {
			case(BAR_SAY):
				sprintf(buf2,"SAY");
				break;
			case(BAR_EMOTE):
				sprintf(buf2,"EMOTE");
				break;
			case(BAR_ECHO):
				sprintf(buf2,"ECHO");
				break;
		}
		fprintf(fp,"B %s %s %d %d %s %s~\n%s%s",
			flag_name_lookup(pMobIndex->barred_entry->type, criterion_flags),
			buf,
			pMobIndex->barred_entry->value,
			pMobIndex->barred_entry->vnum,
			buf2,
			pMobIndex->barred_entry->message,
			(pMobIndex->barred_entry->message_two
			&& pMobIndex->barred_entry->msg_type == BAR_ECHO) ? 
				pMobIndex->barred_entry->message_two : "",
			(pMobIndex->barred_entry->message_two
			&& pMobIndex->barred_entry->msg_type == BAR_ECHO) ?
			 	"~\n" : "");
	}
			
	for (i = 0; i < MAX_MOB_AFFECT; i++)
		if (pMobIndex->affect_sn[i] > -1) 
			fprintf(fp,"A '%s' %s\n",skill_table[pMobIndex->affect_sn[i]].name,
				flag_name_lookup(pMobIndex->affect_bit[i],affect_flags));
	for (i = 0; i < MAX_MOB_CAST; i++)
		if (pMobIndex->cast_spell[i])
			fprintf(fp,"C '%s'\n", pMobIndex->cast_spell[i]);
	for (i = 0; i < MAX_PROFS_TAUGHT_BY_MOB; i++)
		if (pMobIndex->profs_taught[i] > -1)
			fprintf(fp, "TEACHES %s~\n", prof_table[pMobIndex->profs_taught[i]].name);
	if (pMobIndex->speech) {
		SPEECH_DATA *sptr;
		LINE_DATA *lptr;
		
		for (sptr = pMobIndex->speech; sptr; sptr = sptr->next) {
			fprintf(fp,"SPEECH %s\n", sptr->name);
			for (lptr = sptr->first_line; lptr; lptr = lptr->next)
				fprintf(fp,"LINE %d %s %s~\n",
					lptr->delay,
					flag_name_lookup(lptr->type,speech_table),
					lptr->text);
			fprintf(fp,"END\n");
		}
	}

	return;
}


/*****************************************************************************
 Name:		save_mobiles
 Purpose:	Save #MOBILES secion of an area file.
 Called by:	save_area(olc_save.c).
 Notes:         Changed for ROM OLC.
 ****************************************************************************/
void save_mobiles( FILE *fp, AREA_DATA *pArea )
{
    int i;
    MOB_INDEX_DATA *pMob;

    fprintf( fp, "#MOBS\n" );

    for( i = pArea->min_vnum; i <= pArea->max_vnum; i++ )
    {
	if ( (pMob = get_mob_index( i )) )
	    save_mobile( fp, pMob );
    }

    fprintf( fp, "#0\n\n\n\n" );
    return;
}





/*****************************************************************************
 Name:		save_object
 Purpose:	Save one object to file.
                new ROM format saving -- Hugin
 Called by:	save_objects (below).
 ****************************************************************************/
void save_object( FILE *fp, OBJ_INDEX_DATA *pObjIndex )
{
	long i;
	long dummy[MAX_BITVECTOR];
	OBJ_APPLY_DATA *app;
	AFFECT_DATA *paf;
	EXTRA_DESCR_DATA *ed;

	zero_vector(dummy);

	fprintf(fp,"#%d\n", pObjIndex->vnum);
	fprintf(fp,"%s~\n", pObjIndex->name);
        fprintf(fp,"%s~\n", pObjIndex->short_descr);
        fprintf(fp,"%s~\n",munch(pObjIndex->description));
        fprintf(fp,"%s\n", item_name(pObjIndex->item_type));
        fprintf(fp,"%s~\n", pObjIndex->material);

        switch(pObjIndex->item_type)
        {
            case ITEM_ARMOR:
                fprintf(fp,"%d %d %d %d %d\n",
                    pObjIndex->value[0], pObjIndex->value[1],
                    pObjIndex->value[2], pObjIndex->value[3],
                    pObjIndex->value[4]);
                break;
            case ITEM_WEAPON:
				dummy[0] = pObjIndex->value[4];
                fprintf(fp,"%s %d %d %s %s\n",
                    weapon_name_lookup(pObjIndex->value[0]),
                    pObjIndex->value[1], pObjIndex->value[2],
                    attack_table[UMAX(0,pObjIndex->value[3])].name,
                    bitvector_to_flag(dummy));
                break;
            case ITEM_DRINK_CON:
            case ITEM_FOUNTAIN:
                fprintf(fp,"%d %d '%s' %d %d\n", pObjIndex->value[0],
                    pObjIndex->value[1],
                    liq_table[UMAX(pObjIndex->value[2],0)].liq_name,
                    pObjIndex->value[3], pObjIndex->value[4]);
                break;
            case ITEM_WAND:
            case ITEM_STAFF:
                fprintf(fp,"%d %d %d '%s' %d\n", pObjIndex->value[0],
                    pObjIndex->value[1], pObjIndex->value[2],
                    skill_table[UMAX(pObjIndex->value[3],0)].name,
                    pObjIndex->value[4]);
                break;
            case ITEM_POTION:
            case ITEM_SCROLL:
            case ITEM_PILL:
                fprintf(fp, "%d '%s' '%s' '%s' '%s'\n", pObjIndex->value[0],
                    skill_table[UMAX(pObjIndex->value[1],0)].name,
                    skill_table[UMAX(pObjIndex->value[2],0)].name,
                    skill_table[UMAX(pObjIndex->value[3],0)].name,
                    skill_table[UMAX(pObjIndex->value[4],0)].name);
                break;
            case ITEM_CONTAINER:
				dummy[0] = pObjIndex->value[1];
                fprintf(fp,"%d %s %d %d %d\n",
                    pObjIndex->value[0], bitvector_to_flag(dummy),
                    pObjIndex->value[2], pObjIndex->value[3],
                    pObjIndex->value[4]);
                break;
            case ITEM_FURNITURE:
				dummy[0] = pObjIndex->value[2];
                fprintf(fp,"%d %d %s %d %d\n",
                    pObjIndex->value[0], pObjIndex->value[1],
                    bitvector_to_flag(dummy),
                    pObjIndex->value[3], pObjIndex->value[4]);
                break;
            default:
                fprintf(fp,"%d %d %d %d %d\n",
                    pObjIndex->value[0], pObjIndex->value[1],
                    pObjIndex->value[2], pObjIndex->value[3],
                    pObjIndex->value[4]);
                break;
        }

        fprintf(fp,"%d %d %d P\n",
            pObjIndex->level, pObjIndex->weight, pObjIndex->cost);

	for(i = 0; i < (32 * MAX_BITVECTOR); i++) {
		if(IS_SET(pObjIndex->wear_flags, i)) {
			fprintf(fp,"WEAR %s\n",(upstring(flag_name_lookup(i, wear_flags))));
		}
	}

	for(i = 0; i < (32 * MAX_BITVECTOR); i++) {
		if(IS_SET(pObjIndex->restrict_flags, i)) {
			fprintf(fp,"RESTRICT %s\n",(upstring(restrict_name_lookup(i))));
		}
	}
	
	for(i = 0; i < (32 * MAX_BITVECTOR); i++) {
		if(IS_SET(pObjIndex->extra_flags, i)) {
			fprintf(fp,"ITEM %s\n",(upstring(flag_name_lookup(i, extra_flags))));
		}
	}

        for(app = pObjIndex->apply; app; app = app->next)
            fprintf(fp,"APPLY %s %d\n",
                (upstring(display_name_lookup(app->location,apply_locations))),
                app->modifier);

        fprintf(fp,"LIMIT %d\n",pObjIndex->limtotal);

		if(pObjIndex->wear_echo[0] && pObjIndex->wear_echo[1])
            fprintf(fp,"MSG WEAR %s~\n%s~\n",pObjIndex->wear_echo[0],pObjIndex->wear_echo[1]);

        if(pObjIndex->remove_echo[0] && pObjIndex->remove_echo[1])
            fprintf(fp,"MSG REMOVE %s~\n%s~\n", pObjIndex->remove_echo[0],
                pObjIndex->remove_echo[1]);

        if (pObjIndex->verb)
            fprintf(fp,"VERB %s\n", pObjIndex->verb);

	if (pObjIndex->cabal)
	    	fprintf(fp,"CABAL %s\n", upstring(cabal_table[pObjIndex->cabal].name));

        if(pObjIndex->start_timer>-1)
            fprintf(fp,"TIMER %d\n",pObjIndex->start_timer);

	for(i = 0; i < (32 * MAX_BITVECTOR); i++) {
		if(IS_SET(pObjIndex->imm_flags,i)) {
			fprintf(fp,"FLAG IMM %s\n",(upstring(flag_name_lookup(i, imm_flags))));
		}
	}

	for(i = 0; i < (32 * MAX_BITVECTOR); i++) {
		if(IS_SET(pObjIndex->res_flags,i)) {
			fprintf(fp,"FLAG RES %s\n",(upstring(flag_name_lookup(i, imm_flags))));
		}
	}

	for(i = 0; i < (32 * MAX_BITVECTOR); i++) {
		if(IS_SET(pObjIndex->vuln_flags,i)) {
			fprintf(fp,"FLAG VUL %s\n",(upstring(flag_name_lookup(i, imm_flags))));
		}
	}

        for (paf = pObjIndex->charaffs; paf; paf = paf->next)
            fprintf(fp,"FLAG AFF '%s' %s %s\n",
				(skill_table[paf->type].name),
                (affect_bit_name(paf->bitvector)),
                (paf->aftype == AFT_SPELL) ? "SHOW" : "NOSHOW");

		if (pObjIndex->wear_loc_name)
			fprintf(fp,"NAMEOFLOC %s~\n", pObjIndex->wear_loc_name);
        if (pObjIndex->notes)
            fprintf(fp,"NOTES %s~\n", pObjIndex->notes);

        for(ed = pObjIndex->extra_descr; ed; ed = ed->next)
            fprintf(fp,"E %s~\n%s~\n", ed->keyword,
                munch(ed->description));

		fprintf(fp,"\n");
		return;
}




/*****************************************************************************
 Name:		save_objects
 Purpose:	Save #OBJECTS section of an area file.
 Called by:	save_area(olc_save.c).
 Notes:         Changed for ROM OLC.
 ****************************************************************************/
void save_objects( FILE *fp, AREA_DATA *pArea )
{
    int i;
    OBJ_INDEX_DATA *pObj;

    fprintf( fp, "#OBJS\n" );

    for( i = pArea->min_vnum; i <= pArea->max_vnum; i++ )
    {
		if ( (pObj = get_obj_index( i )) ) {
			save_object( fp, pObj );
		}
	}

    fprintf( fp, "#0\n\n\n\n" );
    return;
}





/*****************************************************************************
 Name:		save_rooms
 Purpose:	Save #ROOMS section of an area file.
 Called by:	save_area(olc_save.c).
 ****************************************************************************/
void save_rooms( FILE *fp, AREA_DATA *pArea )
{
    ROOM_INDEX_DATA *pRoom;
    EXTRA_DESCR_DATA *ed;
    EXIT_DATA *pexit;
	char buf1[MSL], buf2[MSL];
    int iHash;
	long i;
	long long rs_temp;
    fprintf( fp, "#ROOMS\n" );

	for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
	{
		for ( pRoom = room_index_hash[iHash]; pRoom; pRoom = pRoom->next)
		{
			if(pRoom->area != pArea)
				continue;

			sprintf(buf1,munch(pRoom->name));
			sprintf(buf2,munch(pRoom->description));

			fprintf(fp,"#%d\n",pRoom->vnum);
			fprintf(fp,"%s~\n%s~\n", buf1, buf2);
			fprintf(fp,"%s\n",sect_table[UMAX(0,pRoom->sector_type)].name);

			rs_temp = 0;	
			for(i = 0; i < (32 * MAX_BITVECTOR); i++)	
				if(IS_SET(pRoom->room_flags,i))
				{
					rs_temp += (long long int)pow(2, i);
					fprintf(fp,"ROOM %s\n",
						(upstring(flag_name_lookup(i, room_flags))));
				}

			for(i = 0; i < 6; i++)
				if((pexit = pRoom->exit[i]))
				{
					sprintf(buf1,pexit->keyword ? munch(pexit->keyword) : "door");
					sprintf(buf2,munch(pexit->description));
					fprintf(fp,"D %s %d %s %d\n%s~\n%s~\n",
						upstring(direction_table[UMAX(0,i)].name),
						pexit->u1.to_room ? pexit->u1.to_room->vnum : 0,
						bitvector_to_flag(pexit->exit_info), pexit->key,
						buf1,
						buf2);
				}

			if (pRoom->trap)
			{
				fprintf(fp,"TRAP '%s' %d %d %d\n%s~\n%s~\n",
					flag_name_lookup(pRoom->trap->type,trap_table),
					pRoom->trap->quality,
					pRoom->trap->complexity,
					pRoom->trap->timer,
					pRoom->trap->trig_echo,
					pRoom->trap->exec_echo);
			}
			if(pRoom->alt_description)
				fprintf(fp,"ALTDESC %d %s~\n %s~\n", pRoom->alt_description_cond, pRoom->alt_name, pRoom->alt_description);
			for(ed = pRoom->extra_descr; ed; ed = ed->next)
				fprintf(fp,"E %s~\n%s~\n",ed->keyword, munch(ed->description));
			if(pRoom->cabal)
				fprintf(fp,"CABAL %s\n", cabal_table[pRoom->cabal].name);

			if(pRoom->owner && pRoom->owner[0] != '\0')
				fprintf(fp,"OWNER %s~\n", pRoom->owner);
		}
	}

	fprintf( fp, "#0\n\n\n\n" );
    return;
}



/*****************************************************************************
 Name:		save_progs
 Purpose:	Save #IPROGS section of area file.
 Called by:	save_area(olc_save.c).
 ****************************************************************************/
void save_progs( FILE *fp, AREA_DATA *pArea )
{
	long iHash;
	MOB_INDEX_DATA *mIndex = NULL;
	OBJ_INDEX_DATA *pObjIndex = NULL;
	ROOM_INDEX_DATA *pRoomIndex = NULL;
	
	fprintf(fp,"#IMPROGS\n");

	if(IS_SET(pArea->progtypes, APROG_PULSE))
		fprintf(fp,"A pulse_prog %s\n",pArea->aprogs->pulse_name);
	if(IS_SET(pArea->progtypes, APROG_RESET))
		fprintf(fp,"A reset_prog %s\n",pArea->aprogs->reset_name);
	if(IS_SET(pArea->progtypes, APROG_SUN))
		fprintf(fp,"A sun_prog %s\n",pArea->aprogs->sun_name);
	if (IS_SET(pArea->progtypes, APROG_TICK))
		fprintf(fp,"A tick_prog %s\n",pArea->aprogs->tick_name);
	if (IS_SET(pArea->progtypes, APROG_AGGRESS))
		fprintf(fp,"A aggress_prog %s\n",pArea->aprogs->aggress_name);
	if (IS_SET(pArea->progtypes, APROG_MYELL))
		fprintf(fp,"A myell_prog %s\n", pArea->aprogs->myell_name);

	for (iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
    {
	  for ( pRoomIndex = room_index_hash[iHash]; pRoomIndex; pRoomIndex = pRoomIndex->next)
	  {
		if(pRoomIndex->area != pArea)
			continue;

		if(pRoomIndex->progtypes)
		{
			if(IS_SET(pRoomIndex->progtypes, RPROG_PULSE))
				fprintf(fp,"R %d pulse_prog %s\n", pRoomIndex->vnum, pRoomIndex->rprogs->pulse_name);
			if(IS_SET(pRoomIndex->progtypes, RPROG_ENTRY))
				fprintf(fp,"R %d entry_prog %s\n", pRoomIndex->vnum, pRoomIndex->rprogs->entry_name);
			if(IS_SET(pRoomIndex->progtypes, RPROG_MOVE))
				fprintf(fp,"R %d move_prog %s\n", pRoomIndex->vnum, pRoomIndex->rprogs->move_name);
			if(IS_SET(pRoomIndex->progtypes, RPROG_DROP))
				fprintf(fp,"R %d drop_prog %s\n", pRoomIndex->vnum, pRoomIndex->rprogs->drop_name);
			if(IS_SET(pRoomIndex->progtypes, RPROG_SPEECH))
				fprintf(fp,"R %d speech_prog %s\n", pRoomIndex->vnum, pRoomIndex->rprogs->speech_name);
			if(IS_SET(pRoomIndex->progtypes, RPROG_OPEN))
                fprintf(fp,"R %d open_prog %s\n", pRoomIndex->vnum, pRoomIndex->rprogs->open_name);
		}
	  }
	
	  for(mIndex = mob_index_hash[iHash]; mIndex; mIndex = mIndex->next)
      {
        if(mIndex->area != pArea)
            continue;
        if(!mIndex->progtypes)
            continue;
        if(mIndex->progtypes)
        {
            if(IS_SET(mIndex->progtypes, MPROG_BRIBE))
                fprintf(fp,"M %d bribe_prog %s\n", mIndex->vnum,
                    mIndex->mprogs->bribe_name);
            if(IS_SET(mIndex->progtypes, MPROG_ENTRY))
                fprintf(fp,"M %d entry_prog %s\n", mIndex->vnum,
                    mIndex->mprogs->entry_name);
            if(IS_SET(mIndex->progtypes, MPROG_GREET))
                fprintf(fp,"M %d greet_prog %s\n", mIndex->vnum,
                    mIndex->mprogs->greet_name);
            if(IS_SET(mIndex->progtypes, MPROG_GIVE))
                fprintf(fp,"M %d give_prog %s\n", mIndex->vnum,
                    mIndex->mprogs->give_name);
            if(IS_SET(mIndex->progtypes, MPROG_FIGHT))
                fprintf(fp,"M %d fight_prog %s\n", mIndex->vnum,
                    mIndex->mprogs->fight_name);
            if(IS_SET(mIndex->progtypes, MPROG_DEATH))
                fprintf(fp,"M %d death_prog %s\n", mIndex->vnum,
                    mIndex->mprogs->death_name);
            if(IS_SET(mIndex->progtypes, MPROG_BEAT))
                fprintf(fp,"M %d beat_prog %s\n", mIndex->vnum,
                    mIndex->mprogs->beat_name);
            if(IS_SET(mIndex->progtypes, MPROG_PULSE))
                fprintf(fp,"M %d pulse_prog %s\n", mIndex->vnum,
                    mIndex->mprogs->pulse_name);
            if(IS_SET(mIndex->progtypes, MPROG_SPEECH))
                fprintf(fp,"M %d speech_prog %s\n", mIndex->vnum,
                    mIndex->mprogs->speech_name);
            if(IS_SET(mIndex->progtypes, MPROG_ATTACK))
                fprintf(fp,"M %d attack_prog %s\n", mIndex->vnum,
                    mIndex->mprogs->attack_name);
            if(IS_SET(mIndex->progtypes, MPROG_MOVE))
                fprintf(fp,"M %d move_prog %s\n", mIndex->vnum,
                    mIndex->mprogs->move_name);
			if(IS_SET(mIndex->progtypes, MPROG_AGGRESS))
				fprintf(fp,"M %d aggress_prog %s\n", mIndex->vnum,
					mIndex->mprogs->aggress_name);
		}
    }
    }

    for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
    {
    for(pObjIndex=obj_index_hash[iHash];pObjIndex; pObjIndex = pObjIndex->next)
    {
        if(pObjIndex->area != pArea || !pObjIndex->progtypes)
            continue;
        if(IS_SET(pObjIndex->progtypes, IPROG_WEAR))
            fprintf(fp,"I %d wear_prog %s\n", pObjIndex->vnum,
                pObjIndex->iprogs->wear_name);
        if(IS_SET(pObjIndex->progtypes, IPROG_REMOVE))
            fprintf(fp,"I %d remove_prog %s\n", pObjIndex->vnum,
                pObjIndex->iprogs->remove_name);
        if(IS_SET(pObjIndex->progtypes, IPROG_DROP))
            fprintf(fp,"I %d drop_prog %s\n", pObjIndex->vnum,
                pObjIndex->iprogs->drop_name);
        if(IS_SET(pObjIndex->progtypes, IPROG_SAC))
            fprintf(fp,"I %d sac_prog %s\n", pObjIndex->vnum,
                pObjIndex->iprogs->sac_name);
        if(IS_SET(pObjIndex->progtypes, IPROG_GIVE))
            fprintf(fp,"I %d give_prog %s\n", pObjIndex->vnum,
                pObjIndex->iprogs->give_name);
        if(IS_SET(pObjIndex->progtypes, IPROG_GREET))
            fprintf(fp,"I %d greet_prog %s\n", pObjIndex->vnum,
                pObjIndex->iprogs->greet_name);
        if(IS_SET(pObjIndex->progtypes, IPROG_FIGHT))
            fprintf(fp,"I %d fight_prog %s\n", pObjIndex->vnum,
                pObjIndex->iprogs->fight_name);
        if(IS_SET(pObjIndex->progtypes, IPROG_DEATH))
            fprintf(fp,"I %d death_prog %s\n", pObjIndex->vnum,
                pObjIndex->iprogs->death_name);
        if(IS_SET(pObjIndex->progtypes, IPROG_SPEECH))
            fprintf(fp,"I %d speech_prog %s\n", pObjIndex->vnum,
                pObjIndex->iprogs->speech_name);
        if(IS_SET(pObjIndex->progtypes, IPROG_ENTRY))
            fprintf(fp,"I %d entry_prog %s\n", pObjIndex->vnum,
                pObjIndex->iprogs->entry_name);
        if(IS_SET(pObjIndex->progtypes, IPROG_GET))
            fprintf(fp,"I %d get_prog %s\n", pObjIndex->vnum,
                pObjIndex->iprogs->get_name);
        if(IS_SET(pObjIndex->progtypes, IPROG_PULSE))
            fprintf(fp,"I %d pulse_prog %s\n", pObjIndex->vnum,
                pObjIndex->iprogs->pulse_name);
        if(IS_SET(pObjIndex->progtypes, IPROG_INVOKE))
            fprintf(fp,"I %d invoke_prog %s\n", pObjIndex->vnum,
                pObjIndex->iprogs->invoke_name);
        if (IS_SET(pObjIndex->progtypes, IPROG_VERB))
            fprintf(fp,"I %d verb_prog %s\n", pObjIndex->vnum,
                pObjIndex->iprogs->verb_name);
		if (IS_SET(pObjIndex->progtypes, IPROG_SAC))
			fprintf(fp,"I %d sac_prog %s\n", pObjIndex->vnum,
				pObjIndex->iprogs->sac_name);
		if (IS_SET(pObjIndex->progtypes, IPROG_LOOK))
			fprintf(fp,"I %d look_prog %s\n", pObjIndex->vnum,
				pObjIndex->iprogs->look_name);
		if (IS_SET(pObjIndex->progtypes, IPROG_OPEN))
			fprintf(fp,"I %d open_prog %s\n", pObjIndex->vnum,
				pObjIndex->iprogs->open_name);
		if (IS_SET(pObjIndex->progtypes, IPROG_LOOT))
			fprintf(fp,"I %d loot_prog %s\n", pObjIndex->vnum,
				pObjIndex->iprogs->loot_name);
		if (IS_SET(pObjIndex->progtypes, IPROG_HIT))
			fprintf(fp,"I %d hit_prog %s\n", pObjIndex->vnum,
				pObjIndex->iprogs->hit_name);
	}
    }
    fprintf(fp,"END\n");
    return;
}

void save_specs(FILE *fp, AREA_DATA *pArea)
{
	int iHash, i;
	OBJ_INDEX_DATA *pObjIndex;
	MOB_INDEX_DATA *pMob;
	fprintf(fp,"#SPECS\n");
	for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
    {
    	for( pObjIndex = obj_index_hash[iHash]; pObjIndex; pObjIndex = pObjIndex->next)
		{
			if(pObjIndex->area != pArea || !pObjIndex->spec_prog.func)
            	continue;
			for(i = 0; ispec_table[i].spec_name; i++)
				if(ispec_table[i].spec_func == pObjIndex->spec_prog.func)
					fprintf(fp,"I %d %s\n", pObjIndex->vnum, ispec_table[i].spec_name);
				/*else
					bug("Problem saving ispec on obj %d.", pObjIndex->vnum);*/
		}
	}
	for (iHash = 0; iHash < MAX_KEY_HASH; iHash++)
	{
		for(pMob = mob_index_hash[iHash]; pMob; pMob = pMob->next)
		{
			if(pMob->area != pArea || !pMob->spec_prog.func)
				continue;
			for(i = 0; mspec_table[i].spec_name; i++)
				if(mspec_table[i].spec_func == pMob->spec_prog.func)
					fprintf(fp,"M %d %s\n", pMob->vnum, mspec_table[i].spec_name);
				/*else if(mspec_table[i].spec_func != pMob->spec_prog.func)
					bug("Problem saving mspec on mob %d.", pMob->vnum);*/
		}
	}
	fprintf(fp,"END\n");
}
/*****************************************************************************
 Name:		save_resets
 Purpose:	Saves the #RESETS section of an area file.
 Called by:	save_area(olc_save.c)
 ****************************************************************************/
void save_resets( FILE *fp, AREA_DATA *pArea )
{
	RESET_DATA *pReset;
	ROOM_INDEX_DATA *pRoomIndex;
	int i;
	
    fprintf(fp,"#RESETS\n");
	for(i = pArea->min_vnum; i < (pArea->max_vnum + 1); i++)
	{
		if((pRoomIndex = get_room_index(i)) == NULL)
			continue;

		for (pReset = pRoomIndex->reset_first; pReset != NULL; pReset = pReset->next )
        	switch(pReset->command)
        	{
            	case 'M':
                fprintf(fp,"MOB %d TO %d GLIMIT %d LLIMIT %d\n", pReset->arg1, pReset->arg3, pReset->arg2, pReset->arg4);
                break;
            	case 'O':
                fprintf(fp,"OBJECT %d TO %d\n", pReset->arg1, pReset->arg3);
                break;
            	case 'P':
                fprintf(fp,"PUT %d IN %d COUNT %d\n", pReset->arg1,pReset->arg3, pReset->arg4);
				break;
            	case 'G':
                fprintf(fp,"GIVE %d\n", pReset->arg1);
                break;
            	case 'F':
                fprintf(fp,"FOLLOW %d %d\n", pReset->arg1, pReset->arg2);
                break;
            	case 'E':
                fprintf(fp,"EQUIP %d %s\n", pReset->arg1,(upstring(flag_name_lookup(pReset->arg3, wear_locations))));
				break;
            	case 'D':
                fprintf(fp,"DOOR %d %s %s\n", pReset->arg1,
                    (direction_table[pReset->arg2].name),
                    pReset->arg3 == 0 ? "OPEN" : pReset->arg3 == 1 ? "CLOSED"
                    : "LOCKED");
                break;
            	case 'R':
                fprintf(fp,"RANDOMIZE %d %d\n", pReset->arg1, pReset->arg2);
                break;
            	default:
                break;
        	}
	}
	fprintf(fp,"S\n\n");
	return;
}



/*****************************************************************************
 Name:		save_shops
 Purpose:	Saves the #SHOPS section of an area file.
 Called by:	save_area(olc_save.c)
 ****************************************************************************/
void save_shops( FILE *fp, AREA_DATA *pArea )
{
    return;
}



/*****************************************************************************
 Name:		save_area
 Purpose:	Save an area, note that this format is new.
 Called by:	do_asave(olc_save.c).
 ****************************************************************************/
void save_area( AREA_DATA *pArea )
{
	long long temp_bit;
    char buf[MSL];
	FILE *fp=NULL;
    fclose( fpReserve );


	sprintf(buf,"mv -f %s ../area/backup/%s.bak",pArea->file_name,pArea->file_name);
	system(buf);

	system("touch /home/rift/code/area-dump.txt");
	
	if ( !( fp = fopen( pArea->file_name, "w" ) ) )
	{
		bug( "Open_area: fopen", 0 );
		perror( pArea->file_name );
	}

	fprintf(fp,"#AREA\n");
	fprintf(fp,"%s~\n%s~\n%s~\n", pArea->file_name, pArea->name, pArea->credits);
	fprintf(fp,"%d %d\n", pArea->low_range, pArea->high_range);
	fprintf(fp,"%d %d\n", pArea->min_vnum, pArea->max_vnum);
	fprintf(fp,"%s\n", area_type_table[pArea->area_type].name);
	fprintf(fp,"%s\n", climate_table[pArea->climate].name);
	fprintf(fp,"%s\n", bitvector_to_flag(pArea->area_flags));
	fprintf(fp,"%d\n", pArea->security);
	fprintf(fp,"%s~\n\n", pArea->builders);
	temp_bit = pArea->area_flags[0] + pArea->area_flags[1];

	save_mobiles( fp, pArea );
	save_objects( fp, pArea );
	save_rooms( fp, pArea );
	save_resets( fp, pArea );
	save_progs( fp, pArea );
	save_specs( fp, pArea );
	
	fprintf( fp, "#$\n" );

	fclose( fp );
	fpReserve = fopen( NULL_FILE, "r" );
	return;
}


/*****************************************************************************
 Name:		do_asave
 Purpose:	Entry point for saving area data.
 Called by:	interpreter(interp.c)
 ****************************************************************************/
void do_asave( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH], buf[MSL];
	AREA_DATA *pArea;
    FILE *fp;
    int value;
	bool found=FALSE;

	fp = NULL;

	if(!check_security(ch))
		return;
	
	if ( !ch )/* Do an autosave */
	{
		save_area_list();
		for( pArea = area_first; pArea; pArea = pArea->next )
		{
			clean_olc_aflags( pArea );
	    	save_area( pArea );
		}
		return;
	}

	if(get_security(ch) < 10)
	{
		argument = one_argument(argument, arg1);
		if( arg1[0] == '\0' )
		{
			if(IS_BUILDER(ch, ch->in_room->area))
			{
				clean_olc_aflags( ch->in_room->area);
				save_area( ch->in_room->area);
				send_to_char("Saved.\n\r", ch);
				return;
			}
			else
			{
				send_to_char("You are not in your area.\n\r", ch);
				return;
			}
		}
		else if( !str_cmp( arg1, "changed" ))
		{
			for( pArea = area_first; pArea; pArea = pArea->next )                                                     
			{                                                                                                         
				if ( !IS_BUILDER( ch, pArea ) )                                                                       
					continue;                                                                                         
                                                                                         
				if ( IS_SET(pArea->area_flags, AREA_CHANGED) || IS_SET(pArea->area_flags, AREA_ADDED))                
				{                                                                                                     
					found=TRUE;
					clean_olc_aflags( pArea );                                                                        
					save_area( pArea );                                                                               
					sprintf( buf, "%24s - '%s'\n\r", pArea->name, pArea->file_name );                                 
					send_to_char( buf, ch );                                                                          
				}                                                                                                     
			}

			if(!found)
			{
				send_to_char("No areas were saved.\n\r", ch);
			}
		}
		else
		{
			send_to_char( "Syntax:\n\r", ch);
			send_to_char( "  asave         - saves area you are currently in.\n\r", ch);
			send_to_char( "  asave changed - saves all areas in your name that have been changed.\n\r", ch);
			return;
		}
	}
	else
	{
    	smash_tilde( argument );
    	strcpy( arg1, argument );
    	if ( arg1[0] == '\0' )
    	{
    		send_to_char( "Syntax:\n\r", ch );
    		send_to_char( "  asave <vnum>   - saves a particular area\n\r",	ch );
			send_to_char( "  asave list     - saves the area.lst file\n\r",	ch );
    		send_to_char( "  asave area     - saves the area being edited\n\r",	ch );
			send_to_char( "  asave changed  - saves all changed zones\n\r",	ch );
			send_to_char( "  asave world    - saves the world! (db dump)\n\r",	ch );
    		send_to_char( "\n\r", ch );
        	return;
    	}

    	/* Snarf the value (which need not be numeric). */
    	value = atoi( arg1 );
    	if ( !( pArea = get_area_data( value ) ) && is_number( arg1 ) )
    	{
			send_to_char( "That area does not exist.\n\r", ch );
			return;
    	}

		/* Save area of given vnum. */
    	/* ------------------------ */
    	if ( is_number( arg1 ) )
    	{
			if ( !IS_BUILDER( ch, pArea ) )
	    		return send_to_char( "You are not a builder for this area.\n\r", ch );

			save_area_list();
			clean_olc_aflags( pArea );
			save_area( pArea );
			return;
    	}

	/* Save the world, only authorized areas. */
    /* -------------------------------------- */
    if ( !str_cmp( "world", arg1 ))
    {
		save_area_list();
		for( pArea = area_first; pArea != NULL; pArea = pArea->next )
		{
			char buf[MSL];
			if ( !IS_BUILDER( ch, pArea ) )
				continue;	

			if ( !IS_SET(pArea->area_flags, AREA_CHANGED) && !IS_SET(pArea->area_flags, AREA_ADDED))
				continue;
			
			clean_olc_aflags( pArea );
			log_string(pArea->name);

			sprintf(buf,"Saving..%s\n", pArea->name);
			wiznet(buf,0,NULL,WIZ_DEBUG,0,0);
			
	    		save_area( pArea );
		}
		send_to_char( "You saved the world.\n\r", ch );
		return;
    }

    /* Save changed areas, only authorized areas. */
    /* ------------------------------------------ */
    if ( !str_cmp( "changed", arg1 ))
    {
		char buf[MAX_INPUT_LENGTH];

		save_area_list();

		send_to_char( "Saved zones:\n\r", ch );
		sprintf( buf, "None.\n\r" );

		for( pArea = area_first; pArea; pArea = pArea->next )
		{
	    	/* Builder must be assigned this area. */
	    	if ( !IS_BUILDER( ch, pArea ) )
				continue;

	    	/* Save changed areas. */
	    	if ( IS_SET(pArea->area_flags, AREA_CHANGED) || IS_SET(pArea->area_flags, AREA_ADDED))
	    	{
				clean_olc_aflags( pArea );
				save_area( pArea );
				sprintf( buf, "%24s - '%s'\n\r", pArea->name, pArea->file_name );
				send_to_char( buf, ch );
	    	}
        }

		if ( !str_cmp( buf, "None.\n\r" ) )
	    	send_to_char( buf, ch );
        return;
    }

    /* Save the area.lst file. */
    /* ----------------------- */
    if ( !str_cmp( arg1, "list" ))
		return save_area_list();

    /* Save area being edited, if authorized. */
    /* -------------------------------------- */
    if ( !str_cmp( arg1, "area" ) )
    {
		/* Is character currently editing. */
		if ( ch->desc->editor == 0 )
	   		return send_to_char( "You are not editing an area therefore an area vnum is required.\n\r", ch );
	
		/* Find the area to save. */
		switch (ch->desc->editor)
		{
	    	case ED_AREA:
				pArea = (AREA_DATA *)ch->desc->pEdit;
			break;
	    	case ED_ROOM:
				pArea = ch->in_room->area;
			break;
	    	case ED_OBJECT:
				pArea = ( (OBJ_INDEX_DATA *)ch->desc->pEdit )->area;
			break;
	    	case ED_MOBILE:
				pArea = ( (MOB_INDEX_DATA *)ch->desc->pEdit )->area;
			break;
	    	default:
				pArea = ch->in_room->area;
			break;
		}

		if ( !IS_BUILDER( ch, pArea ) )
			return send_to_char( "You are not a builder for this area.\n\r", ch );

		save_area_list();
		clean_olc_aflags( pArea );
		save_area( pArea );
		send_to_char( "Area saved.\n\r", ch );
		return;
    }

    /* Show correct syntax. */
    /* -------------------- */
	do_asave( ch, "" );
    return;
	}
}

