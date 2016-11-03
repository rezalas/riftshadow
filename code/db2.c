/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

/***************************************************************************
*	ROM 2.4 is copyright 1993-1996 Russ Taylor			   *
*	ROM has been brought to you by the ROM consortium		   *
*	    Russ Taylor (rtaylor@pacinfo.com)				   *
*	    Gabrielle Taylor (gtaylor@pacinfo.com)			   *
*	    Brian Moore (rom@rom.efn.org)				   *
*	By using this code, you have agreed to follow the terms of the	   *
*	ROM license, in the file Tartarus/doc/rom.license                  *
***************************************************************************/

/***************************************************************************
*       Tartarus code is copyright (C) 1997-1998 by Daniel Graham          *
*	In using this code you agree to comply with the Tartarus license   *
*       found in the file /Tartarus/doc/tartarus.doc                       *
***************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>
#include "merc.h"
#include "db.h"
#include "lookup.h"
#include "tables.h"
#include "recycle.h"
#include "spec.h"

/* values for db2.c */
struct		social_type	social_table		[MAX_SOCIALS];
extern int flag_lookup args((const char *name, const struct flag_type *flag_table));
extern int flag_index_lookup args((const char *name, const struct flag_type *flag_table));
int display_lookup args((const char *name, const struct display_type *flag_table));
int		social_count;
void		bugout args((char *reason));
extern		AREA_DATA * area_last;
extern		MOB_INDEX_DATA *mindex_list;
extern		OBJ_INDEX_DATA *oIndex_list;
extern		SHOP_DATA *             shop_first;
extern		SHOP_DATA *             shop_last;
/* snarf a socials file */
void load_socials( FILE *fp)
{
    for ( ; ; )
    {
    	struct social_type social;
    	char *temp;
        /* clear social */
	social.char_no_arg = NULL;
	social.others_no_arg = NULL;
	social.char_found = NULL;
	social.others_found = NULL;
	social.vict_found = NULL;
	social.char_not_found = NULL;
	social.char_auto = NULL;
	social.others_auto = NULL;

    	temp = fread_word(fp);
    	if (!strcmp(temp,"#0"))
	{
	    return;  /* done */
	}
#ifdef social_debug
	else
	    fprintf(stderr,"%s\n\r",temp);
#endif
    	strcpy(social.name,temp);
    	fread_to_eol(fp);


	temp = fread_string_eol(fp);
	if (!strcmp(temp,"$"))
	     social.char_no_arg = NULL;
	else if (!strcmp(temp,"#"))
	{
	     social_table[social_count] = social;
	     social_count++;
	     continue;
	}
        else
	    social.char_no_arg = temp;

#ifdef social_debug
	fprintf(stderr,"%s\n\r",temp);
#endif


        temp = fread_string_eol(fp);
        if (!strcmp(temp,"$"))
             social.others_no_arg = NULL;
        else if (!strcmp(temp,"#"))
        {
	     social_table[social_count] = social;
             social_count++;
             continue;
        }
        else
		social.others_no_arg = temp;

#ifdef social_debug
	        fprintf(stderr,"%s\n\r",temp);
#endif
		

	
        temp = fread_string_eol(fp);
        if (!strcmp(temp,"$"))
             social.char_found = NULL;
        else if (!strcmp(temp,"#"))
        {
	     social_table[social_count] = social;
             social_count++;
             continue;
        }
       	else
	    social.char_found = temp;

#ifdef social_debug
	        fprintf(stderr,"%s\n\r",temp);
#endif
		
	
        temp = fread_string_eol(fp);
        if (!strcmp(temp,"$"))
             social.others_found = NULL;
        else if (!strcmp(temp,"#"))
        {
	     social_table[social_count] = social;
             social_count++;
             continue;
        }
        else
	    social.others_found = temp;


#ifdef social_debug
	        fprintf(stderr,"%s\n\r",temp);
#endif
		
	
        temp = fread_string_eol(fp);
        if (!strcmp(temp,"$"))
             social.vict_found = NULL;
        else if (!strcmp(temp,"#"))
        {
	     social_table[social_count] = social;
             social_count++;
             continue;
        }
        else
	    social.vict_found = temp;


#ifdef social_debug
	        fprintf(stderr,"%s\n\r",temp);
#endif
		
	
        temp = fread_string_eol(fp);
        if (!strcmp(temp,"$"))
             social.char_not_found = NULL;
        else if (!strcmp(temp,"#"))
        {
	     social_table[social_count] = social;
             social_count++;
             continue;
        }
        else
	    social.char_not_found = temp;

#ifdef social_debug
	        fprintf(stderr,"%s\n\r",temp);
#endif

	
        temp = fread_string_eol(fp);
        if (!strcmp(temp,"$"))
             social.char_auto = NULL;
        else if (!strcmp(temp,"#"))
        {
	     social_table[social_count] = social;
             social_count++;
             continue;
        }
        else
	    social.char_auto = temp;

        temp = fread_string_eol(fp);
        if (!strcmp(temp,"$"))
             social.others_auto = NULL;
        else if (!strcmp(temp,"#"))
        {
             social_table[social_count] = social;
             social_count++;
             continue;
        }
        else
	    social.others_auto = temp;
	
	social_table[social_count] = social;
    	social_count++;
   }
   return;
}

/*
 * Snarf an IMprog section
 * format: M <mob vnum> <program type> <function name>
 */

void load_improgs( FILE *fp )
{
	ROOM_INDEX_DATA *room;
	char progtype[MAX_INPUT_LENGTH];
	char progname[MAX_INPUT_LENGTH];

    for ( ; ; )
    {
		MOB_INDEX_DATA *pMobIndex;
		OBJ_INDEX_DATA *pObjIndex;
		char letter;
		
		
		switch ( letter = fread_letter( fp ) )
		{
			default:
	    		bug( "Load_improgs: letter '%c' not *IMS.", letter );
	    		exit( 1 );

			case 'E':
				fread_word(fp);
				return;
			
			case '*':
			break;
			
			case 'I':
				pObjIndex = get_obj_index ( fread_number ( fp ) );

				if (pObjIndex->iprogs == NULL)
				{
					pObjIndex->iprogs  = new_iprog();
					CLEAR_MEM(pObjIndex->iprogs, sizeof(IPROG_DATA));
				}
	    		strcpy(progtype, fread_word(fp));
	    		strcpy(progname, fread_word(fp));
	    		iprog_set( pObjIndex, progtype, progname);
			break;

		case 'R':
			room = get_room_index (fread_number (fp) );
			if (!room->rprogs)
			{
				room->rprogs = new RPROG_DATA;
				CLEAR_MEM(room->rprogs, sizeof(RPROG_DATA));
			}
			strcpy(progtype, fread_word(fp));
			strcpy(progname, fread_word(fp));
			rprog_set(room, progtype, progname);
		break;

		case 'A':
			if (!area_last->aprogs)
			{
				area_last->aprogs = new APROG_DATA;
				CLEAR_MEM(area_last->aprogs, sizeof(APROG_DATA));
			}
			strcpy(progtype, fread_word(fp));
			strcpy(progname, fread_word(fp));
			aprog_set(area_last, progtype, progname);
		break;
	
		case 'M':
	    	pMobIndex = get_mob_index( fread_number ( fp ) );
	    	if (pMobIndex->mprogs == NULL)
			{
	      		pMobIndex->mprogs = new MPROG_DATA;
		  		CLEAR_MEM(pMobIndex->mprogs, sizeof(MPROG_DATA));
			}
			strcpy(progtype,fread_word(fp));
			strcpy(progname,fread_word(fp));
			mprog_set( pMobIndex,progtype,progname);
	    break;

		case 'S':
		break;
	}

	fread_to_eol( fp );
    }
}

void load_specs(FILE *fp)
{
	char progname[MAX_INPUT_LENGTH];
    for ( ; ; )
    {
	OBJ_INDEX_DATA *pObjIndex;
	MOB_INDEX_DATA *pMobIndex;
	char letter;
	int i;
	switch ( letter = fread_letter( fp ) )
	{
	default:
	    bug( "Load_specs: letter '%c' not *IMRA.", letter );
	    exit( 1 );

	case 'E':
	    fread_word(fp);
	    return;

	case '*':
	    break;
	
        case 'I':
	    pObjIndex = get_obj_index ( fread_number ( fp ) );
	    strcpy(progname, fread_word(fp));
	    for(i=0; ispec_table[i].spec_name; i++)
			if(!str_cmp(progname,ispec_table[i].spec_name))
			{
				pObjIndex->spec_prog.trapvector = ispec_table[i].spec_events;
				pObjIndex->spec_prog.func = ispec_table[i].spec_func;
				break;
			}
		if(!ispec_table[i].spec_name)
			bug("Error: Unable to load ispec for #%d.",pObjIndex->vnum);
	    break;
/*
	case 'R':
		room = get_room_index (fread_number (fp) );
		if (!room->rprogs)
			room->rprogs = palloc_struct(sizeof(RPROG_DATA));
		strcpy(progname, fread_word(fp));
		rprog_set(room, progtype, progname);
		break;

		case 'A':
		if (!area_last->aprogs)
			area_last->aprogs = palloc_struct(sizeof(APROG_DATA));
		strcpy(progtype, fread_word(fp));
		strcpy(progname, fread_word(fp));
		aprog_set(area_last, progtype, progname);
		break;
*/	
	case 'M':
	    pMobIndex		= get_mob_index	( fread_number ( fp ) );
		strcpy(progname, fread_word(fp));
		for(i=0; mspec_table[i].spec_name; i++)
		{
			if(!str_cmp(progname,mspec_table[i].spec_name))
			{
				pMobIndex->spec_prog.trapvector = mspec_table[i].spec_events;
				pMobIndex->spec_prog.func = mspec_table[i].spec_func;
				break;
			}
		}
		if(!mspec_table[i].spec_name)
			bug("Error: Unable to load mspec for #%d.",pMobIndex->vnum);
	    break;
	}
	fread_to_eol( fp );
    }	
}
void load_mobs( FILE *fp )
{
    MOB_INDEX_DATA *pMobIndex;
    int i = 0, pos = 0;
    char *aword, *bword, *temp_wealth;
	SPEECH_DATA *speech;
	LINE_DATA *line;
    char bugtext[250];
	sh_int wealth;
    for ( ; ; )
    {
        sh_int vnum;
        char letter, let[5];
        int iHash;

        letter                          = fread_letter( fp );
        if ( letter != '#' )
        {
            bug( "Load_new_mobiles: # not found.", 0 );
            exit( 1 );
        }

        vnum                            = fread_number( fp );
        if ( vnum == 0 )
            break;

        fBootDb = FALSE;
        if ( get_mob_index( vnum ) != NULL )
        {
            bug( "Load_new_mobiles: vnum %d duplicated.", vnum );
            exit( 1 );
        }
        fBootDb = TRUE;

        pMobIndex                        = new MOB_INDEX_DATA;
        pMobIndex->vnum                 = vnum;
		pMobIndex->new_format			= TRUE;
		pMobIndex->area					= area_last;
		newmobs++;
		pMobIndex->count				= 0;
        pMobIndex->player_name          = fread_string( fp );
        pMobIndex->short_descr          = fread_string( fp );
        pMobIndex->long_descr           = fread_string( fp );
        pMobIndex->description          = fread_string( fp );
		pMobIndex->barred_entry			= NULL;
        pMobIndex->long_descr[0]        = UPPER(pMobIndex->long_descr[0]);
        pMobIndex->description[0]       = UPPER(pMobIndex->description[0]);

        pMobIndex->pShop                = NULL;
		pMobIndex->notes				= NULL;
        pMobIndex->alignment            = fread_number( fp );
        pMobIndex->group                = fread_number( fp );
		pMobIndex->xp_mod				= fread_number( fp );
        pMobIndex->race                 = race_lookup(fread_string( fp ));
		

		
        pMobIndex->level                = fread_number( fp );
		pMobIndex->SetClass(CLASS_NONE);
		pMobIndex->cabal				= 0;
		pMobIndex->spec_prog.trapvector = 0;
		pMobIndex->spec_prog.func		= NULL;

        if(!mindex_list)
                mindex_list     = pMobIndex;
	/* read hit dice */
        pMobIndex->hit[DICE_NUMBER]     = fread_number( fp );
        /* 'd'          */                fread_letter( fp );
        pMobIndex->hit[DICE_TYPE]   	= fread_number( fp );
        /* '+'          */                fread_letter( fp );
        pMobIndex->hit[DICE_BONUS]      = fread_number( fp );

    /* read mana dice */
/*    pMobIndex->mana[DICE_NUMBER]    = fread_number( fp );
					  fread_letter( fp );
    pMobIndex->mana[DICE_TYPE]  = fread_number( fp );
					  fread_letter( fp );
    pMobIndex->mana[DICE_BONUS] = fread_number( fp ); */
	
	pMobIndex->mana[DICE_NUMBER]	=	1;
	pMobIndex->mana[DICE_TYPE]		=	1;
	pMobIndex->mana[DICE_BONUS]		=	99;

	/* read damage dice */
	pMobIndex->damage[DICE_NUMBER]	= fread_number( fp );
					  fread_letter( fp );
	pMobIndex->damage[DICE_TYPE]	= fread_number( fp );
					  fread_letter( fp );
	pMobIndex->damage[DICE_BONUS]	= fread_number( fp );
	pMobIndex->hitroll		= fread_number(fp);
	pMobIndex->dam_mod		= fread_number(fp);
	pMobIndex->dam_type		= attack_lookup(fread_word(fp));

	/* read armor class */
	pMobIndex->ac[AC_PIERCE]	= fread_number( fp );
	pMobIndex->ac[AC_BASH]		= fread_number( fp );
	pMobIndex->ac[AC_SLASH]		= fread_number( fp );
	pMobIndex->ac[AC_EXOTIC]	= fread_number( fp );

	/* read flags and add in data from the race table*/
	copy_vector(pMobIndex->act, race_data_lookup(pMobIndex->race)->act);
	SET_BIT(pMobIndex->act, ACT_IS_NPC);
	copy_vector(pMobIndex->affected_by, race_data_lookup(pMobIndex->race)->aff);
	copy_vector(pMobIndex->off_flags, race_data_lookup(pMobIndex->race)->off);
	copy_vector(pMobIndex->imm_flags, race_data_lookup(pMobIndex->race)->imm);
	copy_vector(pMobIndex->res_flags, race_data_lookup(pMobIndex->race)->res);
	copy_vector(pMobIndex->vuln_flags, race_data_lookup(pMobIndex->race)->vuln);

	/* vital statistics */
	pMobIndex->start_pos		= position_lookup(fread_word(fp));
	pMobIndex->sex			= sex_lookup(fread_word(fp));
	pMobIndex->sex			= UMAX(0, pMobIndex->sex);
	temp_wealth				= fread_word(fp);
	if(is_number(temp_wealth))
		wealth = atoi(temp_wealth);
	else
		wealth = flag_index_lookup(temp_wealth,wealth_table);

	if (wealth <= 0)
		pMobIndex->wealth = 0;
	else
		pMobIndex->wealth = (int)pow(10,wealth - 1);

	fread_flag_new(pMobIndex->form, fp);
	BITWISE_OR(pMobIndex->form, race_data_lookup(pMobIndex->race)->form);
	fread_flag_new(pMobIndex->parts, fp);
	BITWISE_OR(pMobIndex->parts,race_data_lookup(pMobIndex->race)->parts);
	/* size */
	pMobIndex->size			= size_lookup(fread_word(fp));
	pMobIndex->size			= UMAX(pMobIndex->size, 0);
 	pMobIndex->mprogs		= NULL;
	pMobIndex->restrict_low	 	= LOW_VNUM;
	pMobIndex->restrict_high	= HIGH_VNUM;
	for(int j = 0; j < MAX_PROFS_TAUGHT_BY_MOB; j++)
		pMobIndex->profs_taught[j] = -1;
	zero_vector(pMobIndex->styles);
	zero_vector(pMobIndex->progtypes);

	for ( ; ; )
	{
		let[0] = fread_letter(fp);
		let[1] = fread_letter(fp);
		let[2] = UPPER(let[0]);
		let[3] = UPPER(let[1]);
        ungetc(let[1],fp);
		ungetc(let[0],fp);

		if( (let[2]!='A' && let[2]!='O'
			 && let[2]!='I' && let[2]!='R'
			 && let[2]!='V' && let[2]!='C'
			 && let[2]!='S') ||
			(let[3]!='C' && let[3]!='F'
			 && let[3]!='M'
			 && let[3]!='E'
			 && let[3]!='U'
			 && let[3]!='P'
			 && let[3]!='L'))
			break;

		aword = talloc_string(fread_word(fp));
		bword = talloc_string(fread_word(fp));

		if(!str_cmp(aword,"ACT") 
			&& ((pos = flag_lookup(bword,act_flags)) != NO_FLAG) 
			&& pos != ACT_IS_NPC) {
			SET_BIT(pMobIndex->act, pos);
			BITWISE_OR(pMobIndex->act, race_data_lookup(pMobIndex->race)->act);
		}
		if(!str_cmp(aword,"AFF") 
			&& (pos = flag_lookup(bword,affect_flags)) != NO_FLAG) {
			SET_BIT(pMobIndex->affected_by, pos);
			BITWISE_OR(pMobIndex->affected_by, race_data_lookup(pMobIndex->race)->aff);
		}
		if(!str_cmp(aword,"OFF") 
			&& (pos = flag_lookup(bword,off_flags)) != NO_FLAG) {
			SET_BIT(pMobIndex->off_flags, pos);
			BITWISE_OR(pMobIndex->off_flags, race_data_lookup(pMobIndex->race)->off);
		}
		if(!str_cmp(aword,"IMM") 
			 && (pos = flag_lookup(bword,imm_flags)) != NO_FLAG) {
			SET_BIT(pMobIndex->imm_flags, pos);
			BITWISE_OR(pMobIndex->imm_flags, race_data_lookup(pMobIndex->race)->imm);
		}
		if(!str_cmp(aword,"RES") 
			 && (pos = flag_lookup(bword,imm_flags)) != NO_FLAG) {	
			SET_BIT(pMobIndex->res_flags, pos);
			BITWISE_OR(pMobIndex->res_flags, race_data_lookup(pMobIndex->race)->res);
		}
		if(!str_cmp(aword,"VUL") 
			&& (pos = flag_lookup(bword,imm_flags)) != NO_FLAG) {	
			SET_BIT(pMobIndex->vuln_flags, pos);
			BITWISE_OR(pMobIndex->vuln_flags, race_data_lookup(pMobIndex->race)->vuln);
		}
	
		if(!str_cmp(aword,"CLASS"))
		{
			pMobIndex->SetClass(CClass::Lookup(bword));
			if (pMobIndex->Class()->GetIndex() == CLASS_WARRIOR)
			{
				long 	bit;

				bit = style_table[style_lookup(fread_word(fp))].bit;
				SET_BIT(pMobIndex->styles, bit);
				bit = style_table[style_lookup(fread_word(fp))].bit;
				SET_BIT(pMobIndex->styles, bit);
			}
			else if (pMobIndex->Class()->GetIndex() == CLASS_SORCERER)
			{
				pMobIndex->ele_major = ele_name_lookup(fread_word(fp));
				pMobIndex->ele_para = ele_name_lookup(fread_word(fp));
			}
		}

		if(!str_cmp(aword,"SPEECH"))
		{
			char *word;
			
			speech = new_speech_data();
			speech->mob = pMobIndex;
			speech->name = palloc_string(bword);
			
			if (!pMobIndex->speech)
				pMobIndex->speech = speech;
			else 
			{
				SPEECH_DATA *sptr;

				for (sptr = pMobIndex->speech; sptr->next ; sptr = sptr->next);

				sptr->next = speech;
				speech->prev = sptr;
			}
				
			for ( ; ; )
			{
				word = fread_word(fp);
				if (!str_cmp(word, "LINE"))
				{
					line = new_line_data();
					line->speech = speech;
					if (!speech->first_line)
					{
						line->number = 0;
						speech->first_line = line;
						speech->current_line = line;
					} 
					else 
					{
						line->number = speech->current_line->number + 1;
						speech->current_line->next = line;
						line->prev = speech->current_line;
						speech->current_line = line;
					}
					line->delay = fread_number(fp);
					line->type = pos = flag_lookup(fread_word(fp),speech_table);
					if (pos == NO_FLAG)
						bugout("Improper type in speech line!");
					line->text = fread_string(fp);
					continue;
				}
				else if (!str_cmp(word, "END")) 
				{
					speech->current_line = speech->first_line;
					break;
				} 
				else 
				{
					bugout("Malformed line in speech!");
				}
			}
		}
		
		if(pos==-1)
		{
			sprintf(bugtext,"No flag in the %s table named %s was found (mob %s - %d).",
				aword,bword,pMobIndex->player_name,pMobIndex->vnum);
			bug(bugtext,0);
			exit(1);
		}
	}

	for(i=0;i<10;i++)
		pMobIndex->affect_sn[i]=-1;
	
	/* Morg - Valgrind fix */
	for(i=0;i<=9;i++)
		pMobIndex->cast_spell[i]=NULL;
	
	for ( ; ; )
	{

		letter = fread_letter( fp );

		if (letter == 'F')
		{
			char *word;
			long vector;

			word = fread_word(fp);
			vector = fread_flag(fp);

			if (!str_prefix(word,"act"))
		    	REMOVE_BIT(pMobIndex->act,vector);
			else if (!str_prefix(word,"aff"))
		    	REMOVE_BIT(pMobIndex->affected_by,vector);
			else if (!str_prefix(word,"off"))
		    	REMOVE_BIT(pMobIndex->off_flags,vector);
			else if (!str_prefix(word,"imm"))
		    	REMOVE_BIT(pMobIndex->imm_flags,vector);
			else if (!str_prefix(word,"res"))
		    	REMOVE_BIT(pMobIndex->res_flags,vector);
			else if (!str_prefix(word,"vul"))
		    	REMOVE_BIT(pMobIndex->vuln_flags,vector);
			else if (!str_prefix(word,"for"))
		    	REMOVE_BIT(pMobIndex->form,vector);
			else if (!str_prefix(word,"par"))
		    	REMOVE_BIT(pMobIndex->parts,vector);
			else
			{
		    	bug("Flag remove: flag not found.",0);
		    	exit(1);
			}
		}
	    else if ( letter == 'A' )
	    {
			char *word, *word2;
			int sn;
			long bit;
		
			/* dev - mob affects */
			word = fread_word(fp);
			sn = skill_lookup(word);
			word2 = fread_word(fp);
			bit = flag_lookup(word2,affect_flags);
			if(sn)
			{
				for(i=0;i<10;i++)
				{
					if(pMobIndex->affect_sn[i]<1)
					{
						pMobIndex->affect_sn[i]=sn;
						pMobIndex->affect_bit[i] = bit;
						break;
					}
				}
			}
	    }
	    else if ( letter == 'C' )
	    {
			char *word, letter, *discard;
			int i;
		
			/* Mob casts - dev */
			if((letter = fread_letter(fp)) == 'A')
			{
				discard = fread_word(fp);
				pMobIndex->cabal = cabal_lookup(fread_word(fp));
				continue;
			}
			ungetc(letter, fp);

			word = fread_word(fp);
			for(i=0;i<=9;i++)
			{
				if(pMobIndex->cast_spell[i]==NULL)
				{
					pMobIndex->cast_spell[i] = palloc_string(word);
					break;
				}
			}
	    }
	    else if ( letter == 'B' )
	    {
			BARRED_DATA *bar;
			char *word;
			
			if(pMobIndex->barred_entry)
				bugout("Mobile has multiple barred entries.");
			
			bar  = new BARRED_DATA;
			bar->type = flag_lookup(fread_word(fp),criterion_flags);
		
			if(bar->type == NO_FLAG)
				bugout("Invalid barred entry type.");
		
			word = fread_word(fp);
			bar->comparison	= -1;
		
			if(!str_cmp(word,"EQUALTO"))
				bar->comparison = BAR_EQUAL_TO;
			if(!str_cmp(word,"LESSTHAN"))
				bar->comparison = BAR_LESS_THAN;
			if(!str_cmp(word,"GREATERTHAN"))
				bar->comparison = BAR_GREATER_THAN;
			if(bar->comparison<0)
				bugout("Invalid comparison in barred entry.");
		
			bar->value = fread_number(fp);
			bar->vnum = fread_number(fp);
			bar->msg_type = -1;
			word = fread_word(fp);
			if(!str_cmp(word,"SAY"))
				bar->msg_type = BAR_SAY;
			if(!str_cmp(word,"EMOTE"))
				bar->msg_type = BAR_EMOTE;
			if(!str_cmp(word,"ECHO"))
			{
				bar->msg_type = BAR_ECHO;
				bar->message = fread_string(fp);
				bar->message_two = fread_string(fp);
			
				if(!str_cmp(bar->message_two,""))
					bar->message_two = NULL;
				pMobIndex->barred_entry = bar;
				continue;
			}
		
			if(bar->msg_type<0)
				bugout("Invalid message type in barred entry.");
		
			bar->message = fread_string(fp);
			pMobIndex->barred_entry = bar;
	    }
		else if(letter == 'N')
		{
			char *discard;

			discard = fread_word(fp);
			pMobIndex->notes = fread_string(fp);
		}
		else if(letter == 'L')
		{
			char *discard;
		
			discard = fread_word(fp);
			pMobIndex->restrict_low = fread_number(fp);
			pMobIndex->restrict_high = fread_number(fp);
		}
	    else if(letter == 'S')
	    {
			SHOP_DATA *pShop;
			char *discard;//, *temp;

			discard = fread_word(fp);
			pShop  = new SHOP_DATA;
			fread_word(fp); //open
			pShop->open_hour = fread_number(fp);
			fread_word(fp); //close
			pShop->close_hour = fread_number(fp);
			fread_word(fp); //exit
			pShop->direction = flag_lookup(fread_word( fp ),direction_table);
			/*
			temp = fread_word(fp);
			if(!strcmp(temp,"NORMAL_SHOP"))
			{
				pShop->fIsPetShop 	= FALSE;
				pShop->pet_cage_vnum 	= 0;
				fread_number(fp);
			}
			else if (!strcmp(temp,"PET_SHOP"))
			{
				pShop->fIsPetShop	= TRUE;
				pShop->pet_cage_vnum	= fread_number(fp);
			}
			*/
			if (pShop->direction == NO_FLAG)
				bugout("Improper direction in shopkeeper definition.");
        	if ( shop_first == NULL )
           		shop_first = pShop;
        	if ( shop_last  != NULL )
            	shop_last->next = pShop;
			pMobIndex->pShop	= pShop;
			pShop->pMobIndex	= pMobIndex;
	    }
	    else if(letter == 'T')
	    {
		char *discard = fread_word(fp);
		int opennum;
		for(opennum = 0; opennum < MAX_PROFS_TAUGHT_BY_MOB; opennum++)
			if(pMobIndex->profs_taught[opennum] == -1)
				break;
		if(opennum >= MAX_PROFS_TAUGHT_BY_MOB)
			bug("Error: Mob %d teaches too many profs.", pMobIndex->vnum);
		pMobIndex->profs_taught[opennum] = CProficiencies::ProfIndexLookup(fread_string(fp));
		if(pMobIndex->profs_taught[opennum] == -1)
			bug("Error: Mob %d trying to teach invalid proficiency.", pMobIndex->vnum);
            }
		else if(letter == 'Y')
		{
			char *discard;
			discard = fread_word(fp);
			pMobIndex->attack_yell = fread_string(fp);
		}
        else
        {
			ungetc(letter,fp);
			break;
        }
	}

        pMobIndex->arms = race_data_lookup(pMobIndex->race)->arms;
        pMobIndex->legs = race_data_lookup(pMobIndex->race)->legs;

        if (!IS_SET(pMobIndex->parts,PART_ARMS))
			pMobIndex->arms = 0;
        if (!IS_SET(pMobIndex->parts,PART_LEGS))
			pMobIndex->legs = 0;

        iHash                   = vnum % MAX_KEY_HASH;
        pMobIndex->next         = mob_index_hash[iHash];
        mob_index_hash[iHash]   = pMobIndex;
        top_mob_index++;
        top_vnum_mob = top_vnum_mob < vnum ? vnum : top_vnum_mob;  /* OLC */
        assign_area_vnum( vnum );                                  /* OLC */
        kill_table[URANGE(0, pMobIndex->level, MAX_LEVEL-1)].number++;
    }

    return;
}
void bugout(char *reason)
{
	FILE *fp;
	bug(reason,0);
	fp = fopen("/home/rift/area/bugout.txt","a");
	fprintf(fp,"%s\n",reason);
	fclose(fp);
	exit(3);
}

void bug_exit(char *file, int nLine)
{
	char buf[MSL];
	sprintf(buf,"Exiting MUD: called from %s:%d.",file,nLine);
	bugout(buf);
}

/*
 * Snarf an obj section. dev style
 */
void load_objs( FILE *fp )
{
    OBJ_INDEX_DATA *pObjIndex;
	bool goodflag = FALSE;

    if ( !area_last )   /* OLC */
    {
        bug( "Load_objects: no #AREA seen yet.", 0 );
        exit( 1 );
    }

    for ( ; ; )
    {
        sh_int vnum;
        char letter;
        int iHash;

        letter                          = fread_letter( fp );
        if ( letter != '#' )
        {
            bug( "Load_objects: # not found.", 0 );
            exit( 1 );
        }

        vnum                            = fread_number( fp );
        if ( vnum == 0 )
            break;

        fBootDb = FALSE;
        if ( get_obj_index( vnum ) != NULL )
        {
            bug( "Load_objects: vnum %d duplicated.", vnum );
            exit( 1 );
        }
        fBootDb = TRUE;

        pObjIndex                        = new OBJ_INDEX_DATA;
	if(!oIndex_list)
		oIndex_list = pObjIndex;
        pObjIndex->vnum                 = vnum;
	pObjIndex->area			= area_last;
        pObjIndex->new_format           = TRUE;
	pObjIndex->reset_num		= 0;
	newobjs++;
	pObjIndex->limcount		= 0;
	pObjIndex->limtotal		= 0;
	pObjIndex->extra_descr	= NULL;

	pObjIndex->name                 = fread_string( fp );
        pObjIndex->short_descr          = fread_string( fp );
        pObjIndex->description          = fread_string( fp );
        pObjIndex->item_type            = item_lookup(fread_word( fp ));
        pObjIndex->material		    = fread_string( fp );
	pObjIndex->material_index	    = material_lookup(pObjIndex->material);
        zero_vector(pObjIndex->extra_flags);
	zero_vector(pObjIndex->wear_flags);
  	zero_vector(pObjIndex->restrict_flags);
	zero_vector(pObjIndex->imm_flags);
	zero_vector(pObjIndex->res_flags);
	zero_vector(pObjIndex->vuln_flags);
        pObjIndex->start_timer		= -1;
		pObjIndex->notes			=	NULL;
		pObjIndex->charaffs				= NULL;
		pObjIndex->apply				= NULL;
        pObjIndex->wear_echo[0]         = NULL;
        pObjIndex->remove_echo[0]       = NULL;
        pObjIndex->wear_echo[1]         = NULL;
        pObjIndex->remove_echo[1]       = NULL;
		pObjIndex->wear_loc_name		= NULL;
		pObjIndex->affected				= NULL;
		pObjIndex->spec_prog.trapvector = 0;
		pObjIndex->cabal				= 0;
		pObjIndex->verb					= NULL;
	pObjIndex->spec_prog.func = NULL;
	switch(pObjIndex->item_type)
	{
	case ITEM_WEAPON:
	    pObjIndex->value[0]		= weapon_type(fread_word(fp));
	    pObjIndex->value[1]		= fread_number(fp);
	    pObjIndex->value[2]		= fread_number(fp);
	    pObjIndex->value[3]		= attack_lookup(fread_word(fp));
	    pObjIndex->value[4]		= fread_flag(fp);
	    break;
	case ITEM_CONTAINER:
	    pObjIndex->value[0]		= fread_number(fp);
	    pObjIndex->value[1]		= fread_flag(fp);
	    pObjIndex->value[2]		= fread_number(fp);
	    pObjIndex->value[3]		= fread_number(fp);
	    pObjIndex->value[4]		= fread_number(fp);
	    break;
        case ITEM_DRINK_CON:
	case ITEM_FOUNTAIN:
            pObjIndex->value[0]         = fread_number(fp);
            pObjIndex->value[1]         = fread_number(fp);
            pObjIndex->value[2]         = liq_lookup(fread_word(fp));
            pObjIndex->value[3]         = fread_number(fp);
            pObjIndex->value[4]         = fread_number(fp);
            break;
	case ITEM_WAND:
	case ITEM_STAFF:
	    pObjIndex->value[0]		= fread_number(fp);
	    pObjIndex->value[1]		= fread_number(fp);
	    pObjIndex->value[2]		= fread_number(fp);
	    pObjIndex->value[3]		= skill_lookup(fread_word(fp));
	    pObjIndex->value[4]		= fread_number(fp);
	    break;
	case ITEM_POTION:
	case ITEM_PILL:
	case ITEM_SCROLL:
 	    pObjIndex->value[0]		= fread_number(fp);
	    pObjIndex->value[1]		= skill_lookup(fread_word(fp));
	    pObjIndex->value[2]		= skill_lookup(fread_word(fp));
	    pObjIndex->value[3]		= skill_lookup(fread_word(fp));
	    pObjIndex->value[4]		= skill_lookup(fread_word(fp));
	    break;
	default:
        pObjIndex->value[0]             = fread_flag( fp );
        pObjIndex->value[1]             = fread_flag( fp );
        pObjIndex->value[2]             = fread_flag( fp );
        pObjIndex->value[3]             = fread_flag( fp );
	    pObjIndex->value[4]		    = fread_flag( fp );
	    break;
	}
	pObjIndex->level		= fread_number( fp );
      pObjIndex->weight               = fread_number( fp );
      pObjIndex->cost                 = fread_number( fp );
	pObjIndex->iprogs               = NULL;
		zero_vector(pObjIndex->progtypes);
        /* condition */
        letter 				= fread_letter( fp );
	switch (letter)
 	{
	    case ('P') :		pObjIndex->condition = 100; break;
	    case ('G') :		pObjIndex->condition =  90; break;
	    case ('A') :		pObjIndex->condition =  75; break;
	    case ('W') :		pObjIndex->condition =  50; break;
	    case ('D') :		pObjIndex->condition =  25; break;
	    case ('B') :		pObjIndex->condition =  10; break;
	    case ('R') :		pObjIndex->condition =   0; break;
	    default:			pObjIndex->condition = 100; break;
	}

        for ( ; ; )
        {
            char letter;
			char *discard;
            letter = fread_letter( fp );
			letter = UPPER(letter);
            if ( letter == 'A' )
            {
                OBJ_APPLY_DATA *apply;
		    //read in PPLY and discard it
 		    	discard			    = fread_word(fp);
                apply               = new_apply_data();
				apply->location		= display_lookup(fread_word(fp),apply_locations);
		    if(apply->location == -1)
				bugout("Invalid affect apply location.");
                apply->modifier           = fread_number( fp );
                apply->next               = pObjIndex->apply;
                pObjIndex->apply 	    = apply;
            }	
        else if ( letter == 'C') /* Cabal */
           {
                fread_word(fp);
                if (pObjIndex->cabal)
                {
                    bug("Load_rooms: duplicate cabal fields.",0);
                    exit(1);
                }
                pObjIndex->cabal = cabal_lookup(fread_word(fp));
            }
	    else if (letter == 'F')
            {
				char *discard, *word;
                discard                 = fread_word(fp);	
				word 					= fread_word(fp);
		
		if(!str_cmp(word,"AFF"))
		{
			AFFECT_DATA paf;

			long 	bit;
			
			init_affect(&paf);
            paf.where          	= TO_AFFECTS;
			paf.aftype			= AFT_INVIS;	
			paf.type			= skill_lookup(fread_word(fp));
			if (paf.type == -1)
				bugout ("ERROR: Failed FLAG AFF skill lookup.");

			bit = flag_lookup(fread_word(fp),affect_flags);
			if (bit != NO_FLAG)
				SET_BIT(paf.bitvector, bit);

		    if(!str_cmp(fread_word(fp),"SHOW"))
				paf.aftype = AFT_SPELL;

			paf.level		= pObjIndex->level;
			paf.duration	= -1;
			paf.location	= 0;
			paf.modifier	= 0;
			charaff_to_obj_index(pObjIndex, &paf);
			
			goodflag = TRUE;
			top_affect++;
		}
        if(!str_cmp(word,"IMM"))
		{
			long	bit;

			bit = flag_lookup(fread_word(fp),imm_flags);
			if (bit != NO_FLAG)
				SET_BIT(pObjIndex->imm_flags, bit);
			goodflag = TRUE;
		}
		if(!str_cmp(word,"RES"))
		{
			long	bit;

			bit = flag_lookup(fread_word(fp),imm_flags);
			if (bit != NO_FLAG)
				SET_BIT(pObjIndex->res_flags, bit);
			goodflag = TRUE;
		}
		if(!str_cmp(word,"VUL"))
		{
			long	bit;

			bit = flag_lookup(fread_word(fp),imm_flags);
			if (bit != NO_FLAG)
				SET_BIT(pObjIndex->vuln_flags,bit);
			goodflag = TRUE;
		}
		if(!goodflag)
			bugout("Invalid flag.");
			}

            else if ( letter == 'E' )
            {
                EXTRA_DESCR_DATA *ed;

                ed                      = new_extra_descr();
                ed->keyword             = fread_string( fp );
                ed->description         = fread_string( fp );
                ed->next                = pObjIndex->extra_descr;
                pObjIndex->extra_descr  = ed;
                top_ed++;
            }
	    else if (letter=='I')
	    {
		long	bit;
		discard			  = fread_word(fp);
		bit = flag_lookup(fread_word(fp),extra_flags);
		if (bit != NO_FLAG)
			SET_BIT(pObjIndex->extra_flags, bit);
	    }
	    else if (letter=='M')
	    {
		char *word;
		discard			  = fread_word(fp);
		word			  = fread_word(fp);
		if(!str_cmp(word,"WEAR"))
		{
			pObjIndex->wear_echo[0]	= fread_string(fp);
			pObjIndex->wear_echo[1] = fread_string(fp);
			continue;
		}
		if(!str_cmp(word,"REMOVE"))
		{
			pObjIndex->remove_echo[0] = fread_string(fp);
			pObjIndex->remove_echo[1] = fread_string(fp);
			continue;
		}
			bugout("Error, invalid MESSAGE word.");
	    }
	    else if ( letter == 'R' )
	    {
		long	bit;
		//discard the ESTRICT
		discard			  = fread_word(fp);
		bit = restrict_lookup(fread_word(fp));
		if (bit == NO_FLAG)
			bugout("Invalid restrict type.");
		SET_BIT(pObjIndex->restrict_flags, bit);
	    }
	    else if ( letter == 'L' )
	    {
		//discard the IMIT
		discard			  = fread_word(fp);
		pObjIndex->limtotal 	  = fread_number(fp);
	    }
		else if ( letter == 'S' )
		{
		//discard the PELL
		discard			= fread_word(fp);
		}	
	    else if (letter=='W')
	    {
		long	bit;
		
		discard			  = fread_word(fp);
		bit = flag_lookup(fread_word(fp),wear_flags);
		if (bit != NO_FLAG)
			SET_BIT(pObjIndex->wear_flags,bit);
	    }
		else if (letter=='V')
		{
		discard			  =	fread_word(fp);
		pObjIndex->verb	  = palloc_string(fread_word(fp));
		}
	    else if (letter=='T')
	    {
		//IMER
		discard			  = fread_word(fp);
		pObjIndex->start_timer	  = fread_number(fp);
	    }
        else if (letter=='N')
	    {
			discard           = fread_word(fp);
			if(!str_cmp(discard,"AMEOFLOC"))
			{
				pObjIndex->wear_loc_name = fread_string(fp);
			}
			else
			{
				pObjIndex->notes  = fread_string(fp);
			}
		}		
            else
            {
                ungetc(letter, fp );
                break;
            }
        }

        iHash                   = vnum % MAX_KEY_HASH;
        pObjIndex->next         = obj_index_hash[iHash];
        obj_index_hash[iHash]   = pObjIndex;
        top_obj_index++;
    }

    return;
}
