/***************************************************************************
 *  File: mem.c                                                            *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 *                                                                         *
 *  This code was freely distributed with the The Isles 1.1 source code,   *
 *  and has been used here for OLC - OLC would not be what it is without   *
 *  all the previous coders who released their source code.                *
 *                                                                         *
 ***************************************************************************/



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

/*
 * Globals
 */
extern          int                     top_reset;
extern          int                     top_area;
extern          int                     top_exit;
extern          int                     top_ed;
extern          int                     top_room;

AREA_DATA		*	area_free;
extern EXTRA_DESCR_DATA	*	extra_descr_free;
EXIT_DATA		*	exit_free;
ROOM_INDEX_DATA		*	room_index_free;
OBJ_INDEX_DATA		*	obj_index_free;
SHOP_DATA		*	shop_free;
MOB_INDEX_DATA		*	mob_index_free;
RESET_DATA		*	reset_free;
HELP_DATA		*	help_free;

extern HELP_DATA		*	help_last;

void free_extra_descr( EXTRA_DESCR_DATA *pExtra );
void free_affect( AFFECT_DATA *af );

RESET_DATA *new_reset_data( void )
{
    RESET_DATA *pReset;

    if ( !reset_free )
    {
        pReset          =   new RESET_DATA;
		CLEAR_MEM(pReset, sizeof(RESET_DATA));
        top_reset++;
    }
    else
    {
        pReset          =   reset_free;
        reset_free      =   reset_free->next;
    }

	pReset->next        =   NULL;
	pReset->command     =   'X';
    pReset->arg1        =   0;
    pReset->arg2        =   0;
    pReset->arg3        =   0;

    return pReset;
}



void free_reset_data( RESET_DATA *pReset )
{
    pReset->next            = reset_free;
    reset_free              = pReset;
    return;
}



AREA_DATA *new_area( void )
{
    AREA_DATA *pArea;
    char buf[MAX_INPUT_LENGTH];

    if ( !area_free )
    {
		pArea = new AREA_DATA;
		CLEAR_MEM(pArea, sizeof(AREA_DATA));
        top_area++;
    }
    else
    {
        pArea       =   area_free;
        area_free   =   area_free->next;
    }

    pArea->next             =   NULL;
    pArea->name             =   palloc_string( "New area" );
    SET_BIT(pArea->area_flags, AREA_ADDED);
    pArea->security         =   1;
    pArea->builders         =   palloc_string( "None" );
	pArea->credits			=	palloc_string( "None" );
	pArea->min_vnum            =   0;
    pArea->max_vnum            =   0;
    pArea->age              =   0;
    pArea->nplayer          =   0;
    pArea->empty            =   TRUE;              /* ROM patch */
    sprintf( buf, "area%d.are", pArea->vnum );
    pArea->file_name        =   palloc_string( buf );
    pArea->vnum             =   top_area-1;
	pArea->area_type		=	ARE_UNOPENED;
	pArea->climate			=	0;
	pArea->low_range		=	0;
	pArea->high_range		=	0;
	zero_vector(pArea->progtypes);
	return pArea;
}



void free_area( AREA_DATA *pArea )
{
    free_pstring( pArea->name );
    free_pstring( pArea->file_name );
    free_pstring( pArea->builders );

    pArea->next         =   area_free->next;
    area_free           =   pArea;
    return;
}



EXIT_DATA *new_exit( void )
{
    EXIT_DATA *pExit;

    if ( !exit_free )
    {
		pExit = new EXIT_DATA;
		CLEAR_MEM(pExit, sizeof(EXIT_DATA));
        top_exit++;
    }
    else
    {
        pExit           =   exit_free;
        exit_free       =   exit_free->next;
    }

    pExit->u1.to_room   =   NULL;                  /* ROM OLC */
    pExit->next         =   NULL;
/*  pExit->vnum         =   0;                        ROM OLC */
    zero_vector(pExit->exit_info);
    pExit->key          =   0;
    pExit->keyword      =  	NULL;
    pExit->description  =   NULL;

    return pExit;
}



void free_exit( EXIT_DATA *pExit )
{
    free_pstring( pExit->keyword );
    free_pstring( pExit->description );

    pExit->next         =   exit_free;
    exit_free           =   pExit;
    return;
}


/*
EXTRA_DESCR_DATA *new_extra_descr( void )
{
    EXTRA_DESCR_DATA *pExtra;

    if ( !extra_descr_free )
    {
        pExtra              =   calloc_struct( sizeof(*pExtra) );
        top_ed++;
    }
    else
    {
        pExtra              =   extra_descr_free;
        extra_descr_free    =   extra_descr_free->next;
    }

    pExtra->keyword         =   NULL;
    pExtra->description     =   NULL;
    pExtra->next            =   NULL;

    return pExtra;
} */


/*
void free_extra_descr( EXTRA_DESCR_DATA *pExtra )
{
    free_pstring( pExtra->keyword );
    free_pstring( pExtra->description );

    pExtra->next        =   extra_descr_free;
    extra_descr_free    =   pExtra;
    return;
}
*/


ROOM_INDEX_DATA *new_room_index( void )
{
    ROOM_INDEX_DATA *pRoom;
    int door,i;

    if ( !room_index_free )
    {
		pRoom = new ROOM_INDEX_DATA;
		CLEAR_MEM(pRoom, sizeof(ROOM_INDEX_DATA));
        top_room++;
    }
    else
    {
        pRoom           =   room_index_free;
        room_index_free =   room_index_free->next;
    }

    pRoom->next             =   NULL;
	pRoom->next_room		=	NULL;
	pRoom->aff_next			=	NULL;
	pRoom->people           =   NULL;
    pRoom->contents         =   NULL;
    pRoom->extra_descr      =   NULL;
    pRoom->area             =   NULL;
    pRoom->alt_description  =	NULL;
    pRoom->alt_name	    =	NULL;
    pRoom->alt_description_cond = 0;
    for ( door=0; door < MAX_DIR; door++ )
        pRoom->exit[door]   =   NULL;
	
	for ( i = 0; i < MAX_TRACKS; i++)
		pRoom->tracks[i] = new_track_data();
	
	pRoom->path				=	NULL;
    pRoom->name             =   palloc_string("Dummy Room");
    pRoom->description      =   NULL;
	pRoom->owner			=	palloc_string("none");
	pRoom->vnum             =   0;
    zero_vector(pRoom->room_flags);
    pRoom->sector_type      =   0;
    pRoom->heal_rate	    =   100;
    pRoom->mana_rate	    =   100;
	pRoom->cabal			=	0;
	pRoom->guild			=	0;
	pRoom->affected			=	NULL;
	zero_vector(pRoom->affected_by);
	pRoom->move_progs		=	FALSE;
	pRoom->rune				=	NULL;
	pRoom->has_rune			=	FALSE;
	pRoom->light			=	0;
	pRoom->reset_first		=	NULL;
	pRoom->reset_last		=	NULL;
	zero_vector(pRoom->progtypes);
	pRoom->trap				=	NULL;
	return pRoom;
}



extern AFFECT_DATA *affect_free;
/*
AFFECT_DATA *new_affect( void )
{
    AFFECT_DATA *pAf;

    if ( !affect_free )
    {
        pAf             =   calloc_struct( sizeof(*pAf) );
        top_affect++;
    }
    else
    {
        pAf             =   affect_free;
        affect_free     =   affect_free->next;
    }

    pAf->next       =   NULL;
    pAf->location   =   0;
    pAf->modifier   =   0;
    pAf->type       =   0;
    pAf->duration   =   0;
    pAf->bitvector  =   0;

    return pAf;
}
*/

/*
void free_affect( AFFECT_DATA* pAf )
{
    pAf->next           = affect_free;
    affect_free         = pAf;
    return;
}
*/


SHOP_DATA *new_shop( void )
{
    SHOP_DATA *pShop;

	pShop = new SHOP_DATA;
	CLEAR_MEM(pShop, sizeof(SHOP_DATA));
	pShop->open_hour    =   0;
    pShop->close_hour   =   23;
	pShop->direction	=	0;
    return pShop;
}



void free_shop( SHOP_DATA *pShop )
{
    pShop->next = shop_free;
    shop_free   = pShop;
    return;
}

OBJ_INDEX_DATA *new_obj_index( void )
{
    OBJ_INDEX_DATA *pObj;
    int value;

    if ( !obj_index_free )
    {
		pObj = new OBJ_INDEX_DATA;
		CLEAR_MEM(pObj, sizeof(OBJ_INDEX_DATA));
        top_obj_index++;
    }
    else
    {
        pObj            =   obj_index_free;
        obj_index_free  =   obj_index_free->next;
    }

    pObj->next          =   NULL;
    pObj->extra_descr   =   NULL;
    pObj->affected      =   NULL;
    pObj->area          =   NULL;
    pObj->name          =   palloc_string( "no name" );
    pObj->level		=   1;
    pObj->short_descr   =   palloc_string( "dummy object" );
    pObj->description   =   palloc_string( "dummy desc" );
    pObj->vnum          =   0;
    pObj->item_type     =   ITEM_TRASH;
    zero_vector(pObj->extra_flags);
    zero_vector(pObj->wear_flags);
    zero_vector(pObj->restrict_flags);
    pObj->start_timer	=   0;
    pObj->count         =   0;
    pObj->weight        =   0;
    pObj->cost          =   0;
    pObj->limtotal	=   0;
    pObj->material      =   palloc_string( "unknown" );      /* ROM */
    pObj->condition     =   100;                        /* ROM */
    pObj->verb			=   NULL;
	pObj->wear_echo[0]	=	NULL;
	pObj->wear_echo[1]	=	NULL;
	pObj->wear_echo[2]	=	NULL;
	pObj->remove_echo[0]=	NULL;
	pObj->remove_echo[1]=	NULL;
	pObj->remove_echo[2]=	NULL;
	pObj->cabal			=	0;
	pObj->notes			=	NULL;
	pObj->wear_loc_name =	NULL;
	pObj->charaffs		=	NULL;
	pObj->extra_descr	=	NULL;
	pObj->apply			=	NULL;
	
	for ( value = 0; value < 5; value++ )               /* 5 - ROM */
        pObj->value[value]  =   0;
    pObj->spec_prog.trapvector = 0;
    pObj->new_format    = TRUE; /* ROM */
	zero_vector(pObj->progtypes);
	zero_vector(pObj->imm_flags);
	zero_vector(pObj->res_flags);
	zero_vector(pObj->vuln_flags);
    pObj->limcount		= 0;
    return pObj;
}



void free_obj_index( OBJ_INDEX_DATA *pObj )
{
    EXTRA_DESCR_DATA *pExtra;
    AFFECT_DATA *pAf;

    free_pstring( pObj->name );
    free_pstring( pObj->short_descr );
    free_pstring( pObj->description );

    for ( pAf = pObj->affected; pAf; pAf = pAf->next )
    {
        free_affect( pAf );
    }

    for ( pExtra = pObj->extra_descr; pExtra; pExtra = pExtra->next )
    {
        free_extra_descr( pExtra );
    }

    pObj->next              = obj_index_free;
    obj_index_free          = pObj;
    return;
}



MOB_INDEX_DATA *new_mob_index( void )
{
    MOB_INDEX_DATA *pMob;
	int i;

    if ( !mob_index_free )
    {
		pMob = new MOB_INDEX_DATA;
		CLEAR_MEM(pMob, sizeof(MOB_INDEX_DATA));
        top_mob_index++;
    }
    else
    {
        pMob            =   mob_index_free;
        mob_index_free  =   mob_index_free->next;
    }

    pMob->next          =   NULL;
    pMob->pShop         =   NULL;
    pMob->area          =   NULL;
    pMob->player_name   =   palloc_string( "no name" );
    pMob->short_descr   =   palloc_string( "dummy mob" );
    pMob->long_descr    =   palloc_string( "A dummy mob is standing here.\n\r" );
    pMob->description   =   NULL;
    pMob->vnum          =   0;
    pMob->count         =   0;
    pMob->killed        =   0;
    pMob->sex           =   0;
    pMob->level         =   0;
	pMob->dam_type		=	0;
	zero_vector(pMob->act);
	SET_BIT(pMob->act, ACT_IS_NPC);
    zero_vector(pMob->affected_by);
    pMob->alignment     =   0;
    pMob->hitroll	=   0;
	pMob->dam_mod		=	100;
    pMob->race          =   race_lookup( "human" ); /* - Hugin */
    zero_vector(pMob->form);           /* ROM patch -- Hugin */
    zero_vector(pMob->parts);          /* ROM patch -- Hugin */
    zero_vector(pMob->imm_flags);           /* ROM patch -- Hugin */
    zero_vector(pMob->res_flags);           /* ROM patch -- Hugin */
	zero_vector(pMob->vuln_flags);           /* ROM patch -- Hugin */
    zero_vector(pMob->off_flags);           /* ROM patch -- Hugin */
    pMob->size          =   SIZE_MEDIUM; /* ROM patch -- Hugin */
    pMob->ac[AC_PIERCE]	=   0;           /* ROM patch -- Hugin */
    pMob->ac[AC_BASH]	=   0;           /* ROM patch -- Hugin */
    pMob->ac[AC_SLASH]	=   0;           /* ROM patch -- Hugin */
    pMob->ac[AC_EXOTIC]	=   0;           /* ROM patch -- Hugin */
    pMob->hit[DICE_NUMBER]	=   0;   /* ROM patch -- Hugin */
    pMob->hit[DICE_TYPE]	=   0;   /* ROM patch -- Hugin */
    pMob->hit[DICE_BONUS]	=   0;   /* ROM patch -- Hugin */
    pMob->mana[DICE_NUMBER]	=   0;   /* ROM patch -- Hugin */
    pMob->mana[DICE_TYPE]	=   0;   /* ROM patch -- Hugin */
    pMob->mana[DICE_BONUS]	=   0;   /* ROM patch -- Hugin */
    pMob->damage[DICE_NUMBER]	=   0;   /* ROM patch -- Hugin */
    pMob->damage[DICE_TYPE]	=   0;   /* ROM patch -- Hugin */
    pMob->damage[DICE_NUMBER]	=   0;   /* ROM patch -- Hugin */
    pMob->start_pos             =   POS_STANDING; /*  -- Hugin */
    pMob->wealth                =   0;
	pMob->restrict_low			=	-1;
	pMob->restrict_high			=	65535;
	pMob->SetClass(CLASS_NONE);
	pMob->new_format            = TRUE;  /* ROM */
	pMob->barred_entry			= NULL;
	pMob->mprogs				= NULL;
    pMob->game_fun		= NULL;
	zero_vector(pMob->progtypes);
	for(i=0; i<MAX_MOB_AFFECT; i++)
		pMob->affect_sn[i]=-1;
    for(i=0; i<MAX_MOB_CAST; i++)
		pMob->cast_spell[i]=NULL;
	pMob->speech		= NULL; //DIE MORGLUM DIE
    pMob->cabal			= 0;
    pMob->attack_yell	= NULL;
	pMob->notes			= NULL;
	zero_vector(pMob->styles);
	pMob->ele_major		= 0;
	pMob->ele_para		= 0;
	for(int j = 0; j < MAX_PROFS_TAUGHT_BY_MOB; j++)
		pMob->profs_taught[j] = -1;

	return pMob;
}



void free_mob_index( MOB_INDEX_DATA *pMob )
{
    free_pstring( pMob->player_name );
    free_pstring( pMob->short_descr );
    free_pstring( pMob->long_descr );
    free_pstring( pMob->description );

    free_shop( pMob->pShop );

    pMob->next              = mob_index_free;
    mob_index_free          = pMob;
    return;
}

