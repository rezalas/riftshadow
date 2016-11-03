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

#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <crypt.h>
#include "merc.h"
//#include "math.h"
#include "magic.h"
#include "recycle.h"
#include "tables.h"
#include "lookup.h"
/* command procedures needed */
DECLARE_DO_FUN(	do_exits	);
DECLARE_DO_FUN( do_look		);
DECLARE_DO_FUN( do_help		);
DECLARE_DO_FUN( do_affects	);
DECLARE_DO_FUN( do_play		);
DECLARE_DO_FUN( do_tell		);
DECLARE_DO_FUN( descr_end_fun	);
DECLARE_DO_FUN( do_weather	);
#define DEBUG_LOG_FILE	"debug.txt"
#define GOD_LOG_FILE	"glog.txt"

char *	const	where_name	[] =
{
    "<used as light>     ",
    "<worn on finger>    ",
    "<worn on finger>    ",
    "<worn around neck>  ",
    "<worn around neck>  ",
    "<worn on torso>     ",
    "<worn on head>      ",
    "<worn on legs>      ",
    "<worn on feet>      ",
    "<worn on hands>     ",
    "<worn on arms>      ",
    "<worn as shield>    ",
    "<worn on body>      ",
    "<worn on waist>     ",
    "<worn around wrist> ",
    "<worn around wrist> ",
    "<wielded>           ",
    "<held>              ",
    "<dual wielded>      ",
	"<marked>            ",
    "<strapped on>       ",
    "<cosmetic gear>     ",
};

/* for do_count */
int max_on = 0;


/*
 * Local functions.
 */
char *	format_obj_to_char	args( ( OBJ_DATA *obj, CHAR_DATA *ch,
				    bool fShort ) );
void	show_list_to_char	args( ( OBJ_DATA *list, CHAR_DATA *ch,
				    bool fShort, bool fShowNothing ) );
void	show_char_to_char_0	args( ( CHAR_DATA *victim, CHAR_DATA *ch ) );
void	show_char_to_char_1	args( ( CHAR_DATA *victim, CHAR_DATA *ch ) );
void	show_char_to_char_2	args( ( CHAR_DATA *victim, CHAR_DATA *ch ) );
void	show_char_to_char_3	args( ( CHAR_DATA *victim, CHAR_DATA *ch ) );
void	show_char_to_char	args( ( CHAR_DATA *list, CHAR_DATA *ch ) );
bool	check_blind		args( ( CHAR_DATA *ch ) );

bool	can_shapeshift		args( ( CHAR_DATA *ch, int form_num) );
void	shapeshift_revert	args( ( CHAR_DATA *ch) );
bool    isCabalItem		args( ( OBJ_DATA *obj) );
bool    isNewbie		args( ( CHAR_DATA *ch) );
void print_chessboard	args( (CHAR_DATA *ch ) );
char * flag_name_lookup args((long bitv, const struct flag_type *flag_table));

char *format_obj_to_char( OBJ_DATA *obj, CHAR_DATA *ch, bool fShort )
{
    static char buf[MAX_STRING_LENGTH];
	OBJ_AFFECT_DATA *oaf;

    buf[0] = '\0';

    if ((fShort && (obj->short_descr == NULL || obj->short_descr[0] == '\0'))
    ||  (obj->description == NULL || obj->description[0] == '\0'))
		return buf;

	if ( IS_AFFECTED(obj,AFF_OBJ_BURNING) )   strcat(buf,"(Burning) ");
    if ( IS_OBJ_STAT(obj, ITEM_INVIS)     )   strcat( buf, "(Invis) "     );
    if ( IS_AFFECTED(ch, AFF_DETECT_EVIL)
         && IS_OBJ_STAT(obj, ITEM_EVIL)   )   strcat( buf, "(Red Aura) "  );
    if (IS_AFFECTED(ch, AFF_DETECT_GOOD)
    &&  IS_OBJ_STAT(obj,ITEM_BLESS))	      strcat( buf,"(Blue Aura) "	);
    if ( IS_AFFECTED(ch, AFF_DETECT_MAGIC)
         && IS_OBJ_STAT(obj, ITEM_MAGIC)  )   strcat( buf, "(Magical) "   );
    if ( IS_OBJ_STAT(obj, ITEM_GLOW)      )   strcat( buf, "(Glowing) "   );
	if ( IS_OBJ_STAT(obj, ITEM_DARK)	  )	  strcat( buf, "(Dark) "	  );
    if ( IS_OBJ_STAT(obj, ITEM_HUM)       )   strcat( buf, "(Humming) "   );
	if ( is_affected_obj(obj, gsn_stash) && IS_IMMORTAL(ch))
	{
		for (oaf = obj->affected; oaf != NULL; oaf = oaf->next)
			if(oaf->type == gsn_stash)
				break;
		sprintf( buf, "(Stashed by %s) ", oaf->owner->name);
	}

	if (IS_OBJ_STAT(obj,ITEM_NOSHOW) && IS_IMMORTAL(ch))
		strcat(buf,"(Nonobvious) ");
	if ( fShort )
    {
	if ( obj->short_descr != NULL )
	    strcat( buf, obj->short_descr );
    }
    else
    {
	if ( obj->description != NULL)
	    strcat( buf, obj->description );
    }

    return buf;
}



/*
 * Show a list to a character.
 * Can coalesce duplicated items.
 */
void show_list_to_char( OBJ_DATA *list, CHAR_DATA *ch, bool fShort, bool fShowNothing )
{
    char buf[MAX_STRING_LENGTH];
    BUFFER *output;
    char **prgpstrShow;
    int *prgnShow;
    char *pstrShow;
    OBJ_DATA *obj;
    int nShow;
    int iShow;
    int count;
    int line;
    bool fCombine;

    if ( ch->desc == NULL )
	return;

    /*
     * Alloc space for output lines.
     */
    line = ch->lines;
    count = 0;
    for ( obj = list; obj != NULL; obj = obj->next_content )
	count++;
    if(!count)
	return;
    ch->lines = 0;
    output = new_buf();
    prgpstrShow	= (char **)talloc_struct((count * sizeof(char *)));
    prgnShow    = (int *)talloc_struct((count * sizeof(int)));
    nShow	= 0;

    /*
     * Format the list of objects.
     */
    for ( obj = list; obj != NULL; obj = obj->next_content )
    {
		if(IS_SET(obj->extra_flags, ITEM_NOSHOW) && !IS_IMMORTAL(ch))
			continue;
		
		if ( obj->wear_loc == WEAR_NONE && can_see_obj( ch, obj ))
		{
	    	pstrShow = format_obj_to_char( obj, ch, fShort );
			fCombine = FALSE;

	    	if ( IS_NPC(ch) || IS_SET(ch->comm, COMM_COMBINE) )
	    	{
				/*
		 		 * Look for duplicates, case sensitive.
		 		 * Matches tend to be near end so run loop backwords.
		 		 */
				for ( iShow = nShow - 1; iShow >= 0; iShow-- )
				{
		    		if ( !strcmp( prgpstrShow[iShow], pstrShow ) )
		    		{
						prgnShow[iShow]++;
						fCombine = TRUE;
						break;
		    		}
				}
	    	}

	    	/*
	     	 * Couldn't combine, or didn't want to.
	     	 */
	    	if ( !fCombine )
	    	{
				prgpstrShow [nShow] = talloc_string( pstrShow );
				prgnShow    [nShow] = 1;
				nShow++;
	    	}
		}
    }

    /*
     * Output the formatted list.
     */
    for ( iShow = 0; iShow < nShow; iShow++ )
    {
		if (prgpstrShow[iShow][0] == '\0')
		    	continue;

		if ( IS_NPC(ch) || IS_SET(ch->comm, COMM_COMBINE) )
		{
	    	if ( prgnShow[iShow] != 1 )
	    	{
				sprintf( buf, "(%2d) ", prgnShow[iShow] );
				add_buf(output,buf);
	    	}
	    	else
	    	{
				add_buf(output,"     ");
	    	}
		}
		add_buf(output,prgpstrShow[iShow]);
		add_buf(output,"\n\r");
    }

    if ( fShowNothing && nShow == 0 )
    {
		if ( IS_NPC(ch) || IS_SET(ch->comm, COMM_COMBINE) )
	    	send_to_char( "     ", ch );
		send_to_char( "Nothing.\n\r", ch );
    }
    page_to_char(buf_string(output),ch);

    /*
     * Clean up.
     */
    free_buf(output);
    ch->lines = line;
    return;
}



void show_char_to_char_0( CHAR_DATA *victim, CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH],message[MAX_STRING_LENGTH];
    int sn_faerie_fog;
	int spellaffs = 0;
    AFFECT_DATA *af;
    OBJ_DATA *obj;
    buf[0] = '\0';

    obj = get_eq_char(victim,WEAR_HEAD);

    sn_faerie_fog = skill_lookup("faerie fog");
    if ( !IS_NPC(victim))
	{
	if ( IS_SET(victim->comm,COMM_AFK     )   ) strcat( buf, "[AFK] "        );
	if (victim->ghost)
		strcat(buf,"(Ghost) ");
	}
    if ( IS_AFFECTED(victim, AFF_INVISIBLE) ) strcat( buf, "(Invis) "      );
    if ( victim->invis_level >= LEVEL_HERO    ) strcat( buf, "(Wizi) "	     );
    if ( IS_AFFECTED(victim, AFF_HIDE)        ) strcat( buf, "(Hide) "       );
    if ( IS_AFFECTED(victim, AFF_CAMOUFLAGE)  ) strcat( buf, "(Camouflage) " );
    if ( IS_AFFECTED(victim, AFF_CHARM) && (!IS_NPC(victim) || victim->pIndexData->vnum != 80)) strcat( buf, "(Charmed) "    );
    if ( IS_AFFECTED(victim, AFF_PASS_DOOR)   ) strcat( buf, "(Translucent) ");
    if ( is_affected(victim, gsn_faerie_fire) ) strcat( buf, "(Pink Aura) "  );
	if(is_affected(victim,gsn_mana_sickness)) strcat(buf, "(Glowing) " );
    if ( is_affected(victim, gsn_incandescense)) strcat( buf, "(Glowing) "    );
   if (is_affected(victim,sn_faerie_fog) )	strcat( buf, "(Purple Aura) "	);
    if ( IS_EVIL(victim)
    &&   IS_AFFECTED(ch, AFF_DETECT_EVIL)     ) strcat( buf, "(Red Aura) "   );
    if ( IS_GOOD(victim)
    &&   IS_AFFECTED(ch, AFF_DETECT_GOOD)     ) strcat( buf, "(Golden Aura) ");

	if ( IS_AFFECTED(ch, AFF_DETECT_MAGIC)) {
		for (af = victim->affected; af; af = af->next)
			if (af->aftype == AFT_SPELL)
				spellaffs++;
		if (spellaffs >= 20) {
			strcat(buf,"(Blinding Aura) ");
		} else if (spellaffs >= 15) {
			strcat(buf,"(Blazing Aura) ");
		} else if (spellaffs >= 10) {
			strcat(buf,"(Luminous Aura) ");
		} else if (spellaffs >= 6) {
			strcat(buf,"(Bright Aura) ");
		} else if (spellaffs >= 3) {
			strcat(buf,"(Flickering Aura) ");
		} else if (spellaffs >= 1) {
			strcat(buf,"(Dim Aura) ");
		}
	}

	if (IS_AFFECTED(victim, AFF_NOSHOW))
	{
		if(IS_SET(victim->act, ACT_NOCTURNAL))
			strcat( buf, "(Nocturnal) ");
		else
			strcat( buf, "(Diurnal) ");
	}

    if ( IS_AFFECTED(victim, AFF_SANCTUARY))
	{
		if(IS_EVIL(victim))
			strcat( buf, "(Dark Aura) " );
		else
			strcat( buf, "(White Aura) ");
	}
	if (is_affected(victim,gsn_cloak_of_mist)) strcat(buf,"(Cloaked in Mist) ");
    if (is_affected(victim,gsn_sanguine_ward))  strcat( buf, "(Crimson Mist) " );
    if (is_affected(victim,gsn_snare))  strcat( buf, "(Ensnared) " );
    if (is_affected(victim,gsn_blade_barrier))  strcat( buf, "(Blade Barrier) ");
    if (is_affected(victim,gsn_hold_person))  strcat( buf, "(Rigid) ");
	if (is_affected(victim,gsn_creeping_tomb)
		&& ((af = affect_find(victim->affected,gsn_creeping_tomb)) != NULL)
		&& af->duration <= 2)					strcat (buf, "(Entombed) ");
	if (!IS_NPC(victim) && victim->pcdata->energy_state == -5)	strcat(buf, "(Frozen) ");
	if (is_affected(victim,gsn_earthfade))		strcat(buf,"(Earthfaded) ");
	if (is_affected(victim,gsn_ultradiffusion)) strcat(buf,"(Ultradiffuse) ");
	if (is_affected(victim,gsn_watermeld)) strcat(buf,"(Watermeld) ");
	if (is_affected(victim,gsn_sphere_of_plasma)) strcat(buf,"(Plasma Sphere) ");
	if (is_affected(victim,gsn_deny_magic)) strcat(buf,"(Blue Aura) ");
	
    if((IS_NPC(victim) || IS_SHIFTED(victim) || 
	   (str_cmp(victim->long_descr,"") && victim->long_descr[0]!='\0')) && 
		!is_affected(ch,gsn_plasma_arc))
    {
	strcat(buf,victim->long_descr);
    	send_to_char(buf,ch);
    	return;
    }
    strcat(buf,get_descr_form(victim,ch,FALSE));
    if ( !IS_NPC(victim) && !IS_SET(ch->comm, COMM_BRIEF))
	strcat( buf, victim->pcdata->title );

    switch ( victim->position )
    {
    case POS_DEAD:     strcat( buf, " is DEAD!!" );              break;
    case POS_MORTAL:   strcat( buf, " is mortally wounded." );   break;
    case POS_INCAP:    strcat( buf, " is incapacitated." );      break;
    case POS_STUNNED:  strcat( buf, " is lying here stunned." ); break;
    case POS_SLEEPING:
	if (victim->on != NULL)
	{
	    if (IS_SET_OLD(victim->on->value[2], SLEEP_AT))
  	    {
		sprintf(message," is here, sleeping at %s.",
		    victim->on->short_descr);
		strcat(buf,message);
	    }
	    else if (IS_SET_OLD(victim->on->value[2], SLEEP_ON))
	    {
		sprintf(message," is here, sleeping on %s.",
		    victim->on->short_descr);
		strcat(buf,message);
	    }
	    else
	    {
		sprintf(message, " is here, sleeping in %s.",
		    victim->on->short_descr);
		strcat(buf,message);
	    }
	}
	else
	    strcat(buf," is sleeping here.");
	break;
    case POS_RESTING:
        if (victim->on != NULL)
	{
            if (IS_SET_OLD(victim->on->value[2],REST_AT))
            {
                sprintf(message," is here, resting at %s.",
                    victim->on->short_descr);
                strcat(buf,message);
            }
            else if (IS_SET_OLD(victim->on->value[2],REST_ON))
            {
                sprintf(message," is here, resting on %s.",
                    victim->on->short_descr);
                strcat(buf,message);
            }
	    else if (IS_SET_OLD(victim->on->value[2],LOUNGE_ON))
	    {
		sprintf(message," is here, lounging on %s.",
		    victim->on->short_descr);
                strcat(buf,message);
	    }
            else
            {
                sprintf(message, " is here, resting in %s.",
                    victim->on->short_descr);
                strcat(buf,message);
            }
	}
        else
	    strcat( buf, " is resting here." );
	break;
    case POS_SITTING:
        if (victim->on != NULL)
        {
            if (IS_SET_OLD(victim->on->value[2],SIT_AT))
            {
                sprintf(message," is here, sitting at %s.",
                    victim->on->short_descr);
                strcat(buf,message);
            }
            else if (IS_SET_OLD(victim->on->value[2],SIT_ON))
            {
                sprintf(message," is here, sitting on %s.",
                    victim->on->short_descr);
                strcat(buf,message);
            }
            else
            {
                sprintf(message, " is here, sitting in %s.",
                    victim->on->short_descr);
                strcat(buf,message);
            }
        }
        else
	    strcat(buf, " is sitting here.");
	break;
    case POS_STANDING:
	if (victim->on != NULL)
	{
	    if (IS_SET_OLD(victim->on->value[2], STAND_AT))
	    {
		sprintf(message," is here, standing at %s.",
		    victim->on->short_descr);
		strcat(buf,message);
	    }
	    else if (IS_SET_OLD(victim->on->value[2],STAND_ON))
	    {
		sprintf(message," is here, standing on %s.",
		   victim->on->short_descr);
		strcat(buf,message);
	    }
	    else
	    {
		sprintf(message," is here, standing in %s.",
		    victim->on->short_descr);
		strcat(buf,message);
	    }
	}
	else
	    strcat( buf, " is here." );
	break;
    case POS_FIGHTING:
	strcat( buf, " is here, fighting " );
	if ( victim->fighting == NULL )
	    strcat( buf, "thin air??" );
	else if ( victim->fighting == ch )
	    strcat( buf, "YOU!" );
	else if ( victim->in_room == victim->fighting->in_room )
	{
	    strcat( buf, PERS( victim->fighting, ch ) );
	    strcat( buf, "." );
	}
	else
	    strcat( buf, "someone who left??" );
	break;
    }

    strcat( buf, "\n\r" );
    buf[0] = UPPER(buf[0]);
    send_to_char( buf, ch );
    return;
}



void show_char_to_char_1( CHAR_DATA *victim, CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH], buf2[MSL], buf3[MSL];
    OBJ_DATA *obj, *belt;
	TROPHY_DATA *placeholder;
	AFFECT_DATA *paf;
    int iWear;
    int percent, i;
	int counter;
    bool found;

    if ( victim->description[0] != '\0' )
		send_to_char( victim->description, ch );
    else
		act( "You see nothing special about $M.", ch, NULL, victim, TO_CHAR );

    if ( victim->max_hit > 0 )
		percent = ( 100 * victim->hit ) / victim->max_hit;
    else
		percent = -1;
	
    strcpy( buf, get_descr_form(victim,ch,FALSE) );

	if (!IS_NPC(victim)) {
		sprintf(buf2, ", a%s %s %s %s%s,",
			(victim->pcdata->beauty == 1 ||
			 victim->pcdata->beauty == 3 ||
			 victim->pcdata->beauty == 4) ? "n" : "",
			(victim->sex == 2) ? beauty_table[victim->pcdata->beauty].female : beauty_table[victim->pcdata->beauty].male,
			sex_table[victim->sex].name,
			pc_race_table[victim->race].name,
			(!str_cmp(pc_race_table[victim->race].name,"celestial")
			 	|| !str_cmp(pc_race_table[victim->race].name,"planar")
				|| !str_cmp(pc_race_table[victim->race].name,"abyss")) ?
			" titan" : "");

		strcat(buf, buf2);
	}

    sprintf(buf2," %s\n", get_battle_condition(victim, percent));
    strcat(buf, buf2);
    buf[0] = UPPER(buf[0]);
    send_to_char( buf, ch );

	if(IS_AFFECTED(victim,AFF_FLYING)) {
		sprintf(buf,"%s$E is hovering in mid-air!%s",get_char_color(ch,"white"),END_COLOR(ch));
		act(buf,ch,0,victim,TO_CHAR);
	}
    if(is_affected(victim,gsn_corona)) {
		sprintf(buf,"%s$E is surrounded by a faint corona of flickering flames!%s",get_char_color(ch,"lightred"),END_COLOR(ch));
		act(buf,ch,0,victim,TO_CHAR);
	}
	if(is_affected(victim,gsn_frigidaura)) {
		sprintf(buf,"%s$E is surrounded by a swirling aura of crackling frost!%s",get_char_color(ch,"lightblue"),END_COLOR(ch));
		act(buf,ch,0,victim,TO_CHAR);
	}
	if(is_affected(victim,gsn_rotating_ward)) {
		counter = 0;
		for (paf = victim->affected; paf; paf = paf->next)
			if (paf->type == gsn_rotating_ward)
				counter++;
		sprintf(buf,"%s%s rotating around $S body!%s",
			get_char_color(ch,"white"),
			(counter == 1) ? "A multifaceted crystal is" :
			"Multifaceted crystals are",
			END_COLOR(ch));
		act(buf,ch,0,victim,TO_CHAR);
	}
	

    if(!is_affected(victim, gsn_cloak_form)||IS_IMMORTAL(ch)) {
    found = FALSE;
    for ( iWear = 0; iWear < MAX_WEAR; iWear++ )
    {
		if(iWear == WEAR_WIELD) //cosmetics come right before "wield" on display
		{
			OBJ_DATA *tObj;
			for(tObj = victim->carrying; tObj; tObj = tObj->next_content)
			if(tObj->wear_loc == WEAR_COSMETIC)
			{
				OBJ_DATA *worn_over = NULL;
				for(worn_over = victim->carrying; worn_over; worn_over = worn_over->next_content)
					if(worn_over != tObj && IS_SET(tObj->extra_flags, ITEM_UNDER_CLOTHES) && is_worn(worn_over))
						if(worn_over->wear_flags[0] + pow(2, ITEM_WEAR_COSMETIC) == tObj->wear_flags[0])
							break;
				if(worn_over)
					continue;
				if(!found)
					act( "\n\r$N is using:", ch, NULL, victim, TO_CHAR );
				sprintf(buf2,"%s>",tObj->wear_loc_name ? tObj->wear_loc_name : "bug: unset wear_loc on cosmetic");
				sprintf(buf, "<worn %-14s",buf2);
				send_to_char(buf,ch);
				if(can_see_obj(ch,tObj))
					send_to_char( format_obj_to_char(tObj, ch, TRUE ), ch );
				else
					send_to_char("something.",ch);
				send_to_char("\n\r",ch);
				found = TRUE;
			}
		}
	if ( iWear != WEAR_COSMETIC && ( obj = get_eq_char( victim, iWear ) ) != NULL
	&&   can_see_obj( ch, obj ) )
	{
	    if ( !found )
	    {
		send_to_char( "\n\r", ch );
		act( "$N is using:", ch, NULL, victim, TO_CHAR );
		found = TRUE;
	    }
		send_to_char( where_name[iWear], ch );
	    send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
	    send_to_char( "\n\r", ch );
	}
    }

	if (!IS_NPC(victim)
	&& victim->cabal == CABAL_HORDE
	&& (belt = get_eq_char(victim,WEAR_WAIST)) != NULL
	&& belt->pIndexData->vnum == OBJ_VNUM_TROPHY_BELT
	&& victim->pcdata->trophy && belt->value[4] >= 1)
	{
		placeholder = victim->pcdata->trophy;
		act("\n\r$p catches your eye, meriting closer examination.",ch,belt,0,TO_CHAR);
		send_to_char(belt->extra_descr->description,ch);
		send_to_char("\n\rAttached to the belt are:\n\r", ch);
		for (i=0;i<MSL;i++)
		{
			buf2[i] = '\0';
			buf3[i] = '\0';
		}
		
		for (counter = 1; counter <= belt->value[4];counter++)
		{
			sprintf(buf2,"%s%s%c scalp of %s",
					counter > 1 ? ", " : "", 
					counter % 4 == 0 ? "\n\r" : "", 
					counter > 1 ? 'a' : 'A',
					victim->pcdata->trophy->victname);
			strcat(buf3,buf2);
			
			for (i = 0;i < MSL;i++)
				buf2[i] = '\0';

			if (counter >= belt->value[4])
				break;
	
			if (!victim->pcdata->trophy->next) 
				break;

			victim->pcdata->trophy = victim->pcdata->trophy->next;
		}
		strcat(buf3,".\n\r");
		send_to_char(buf3,ch);
		buf3[0] = '\0';
		victim->pcdata->trophy = placeholder;
    }

	if ( victim != ch
    &&   !IS_NPC(ch)
    && (IS_IMMORTAL(ch) || !IS_IMMORTAL(victim))
    &&   number_percent( ) < get_skill(ch,gsn_peek))
    {
		send_to_char( "\n\rYou peek at the inventory:\n\r", ch );
		check_improve(ch,gsn_peek,TRUE,4);
		show_list_to_char( victim->carrying, ch, TRUE, TRUE );
		if (!IS_NPC(victim)) {
			act("\n\rYou check for gold coins on $N's person:",ch,0,victim,TO_CHAR);
			if (victim->gold <= 0) {
				act("$N does not seem to be carrying any gold.",ch,0,victim,TO_CHAR);
			} else if (victim->gold < 10) {
				act("Scant few gold pieces jingle softly in $N's purse.",ch,0,victim,TO_CHAR);
			} else if (victim->gold < 100) {
				act("$N's coinpurse appears fairly empty.",ch,0,victim,TO_CHAR);
			} else if (victim->gold < 1000) {
				act("$N's coinpurse has a healthy bulge to it.",ch,0,victim,TO_CHAR);
			} else if (victim->gold < 10000) {
				act("$N's coinpurse hangs heavily at $S waist, laden with gold.",ch,0,victim,TO_CHAR);
			} else if (victim->gold < 100000) {
				act("$N's coinpurse bulges at the seams, overflowing with gold!",ch,0,victim,TO_CHAR);
			} else {
				act("Not only does $N have the largest coinpurse you've ever laid eyes upon, but it can scarcely contain the veritable motherlode $E is toting around.  Thieves the world over are having wet dreams about $N at this very moment.",ch,0,victim,TO_CHAR);
			}
    	}
	}
    }
    return;
}


void show_char_to_char_2( CHAR_DATA *victim, CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH], buf2[MSL];
    OBJ_DATA *obj;
    int iWear;
    int percent;
    bool found;

    if ( victim->max_hit > 0 )
	percent = ( 100 * victim->hit ) / victim->max_hit;
    else
	percent = -1;

    strcpy( buf, get_descr_form(victim,ch,FALSE) );

	if (!IS_NPC(victim)) {
		sprintf(buf2, ", a%s %s %s %s,",
			(victim->pcdata->beauty == 1 ||
			 victim->pcdata->beauty == 3 ||
			 victim->pcdata->beauty == 4) ? "n" : "",
			(victim->sex == 2) ? beauty_table[victim->pcdata->beauty].female : beauty_table[victim->pcdata->beauty].male,
			sex_table[victim->sex].name,
			pc_race_table[victim->race].name);

		strcat(buf, buf2);
	}


	sprintf(buf,"%s %s", victim->name, get_battle_condition(victim, percent));
	
    buf[0] = UPPER(buf[0]);
    send_to_char( buf, ch );

    found = FALSE;
    for ( iWear = 0; iWear < MAX_WEAR; iWear++ )
    {
	if ( ( obj = get_eq_char( victim, iWear ) ) != NULL
	&&   can_see_obj( ch, obj ) )
	{
	    if ( !found )
	    {
		send_to_char( "\n\r", ch );
		act( "$N is using:", ch, NULL, victim, TO_CHAR );
		found = TRUE;
	    }
	    send_to_char( where_name[iWear], ch );
	    send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
	    send_to_char( "\n\r", ch );
	}
    }

    if ( victim != ch
    &&   !IS_NPC(ch)
    && (IS_IMMORTAL(ch) || !IS_IMMORTAL(victim))
    &&   number_percent( ) < get_skill(ch,gsn_peek))
    {
	send_to_char( "\n\rYou peek at the inventory:\n\r", ch );
	check_improve(ch,gsn_peek,TRUE,4);
	show_list_to_char( victim->carrying, ch, TRUE, TRUE );
    }

    return;
}

void show_char_to_char_3( CHAR_DATA *victim, CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH], buf2[MSL];
    int percent;

    if ( can_see( victim, ch ) )
    {
	if (ch == victim)
	    act( "$n looks closely at $mself.",ch,NULL,NULL,TO_ROOM);
	else
	{
	    act( "$n looks closely at you.", ch, NULL, victim, TO_VICT    );
	    act( "$n looks closely at $N.",  ch, NULL, victim, TO_NOTVICT );
	}
    }

    if ( victim->description[0] != '\0' )
    {
	send_to_char( victim->description, ch );
    }
    else
    {
	act( "You see nothing special about $M.", ch, NULL, victim, TO_CHAR );
    }

    if ( victim->max_hit > 0 )
	percent = ( 100 * victim->hit ) / victim->max_hit;
    else
	percent = -1;

    strcpy( buf, get_descr_form(victim,ch,FALSE));

	if (!IS_NPC(victim)) {	
		sprintf(buf2, ", a%s %s %s %s,",
			(victim->pcdata->beauty == 1 ||
			 victim->pcdata->beauty == 3 ||
			 victim->pcdata->beauty == 4) ? "n" : "",
			(victim->sex == 2) ? beauty_table[victim->pcdata->beauty].female : beauty_table[victim->pcdata->beauty].male,
			sex_table[victim->sex].name,
			pc_race_table[victim->race].name);

		strcat(buf, buf2);
	}


	sprintf(buf,"%s %s", victim->name, get_battle_condition(victim, percent));
	
    buf[0] = UPPER(buf[0]);
    send_to_char( buf, ch );

    return;
}


void show_char_to_char( CHAR_DATA *list, CHAR_DATA *ch )
{
    CHAR_DATA *rch;
    bool cant_see = FALSE;

    for ( rch = list; rch != NULL; rch = rch->next_in_room )
    {
	if ( rch == ch )
	    continue;

	if ( get_trust(ch) < rch->invis_level)
	    continue;

	if (IS_NPC(rch) && IS_SET(rch->act, ACT_WARD_MOB))
		continue;
	else if ( (cant_see))
		continue;
	else if ( can_see( ch, rch ) )
	{
	    show_char_to_char_0( rch, ch );
	}
    }

    return;
}



bool check_blind( CHAR_DATA *ch )
{
	AREA_AFFECT_DATA *paf;

    if (!IS_NPC(ch) && IS_SET(ch->act,PLR_HOLYLIGHT))
	return TRUE;

    if ( IS_AFFECTED(ch, AFF_BLIND) )
    {
	send_to_char( "You can't see a thing!\n\r", ch );
	return FALSE;
    }

	if ( is_affected_area(ch->in_room->area, gsn_whiteout) &&
		IS_OUTSIDE(ch)	) {
		for (paf = ch->in_room->area->affected;paf;paf = paf->next) {
			if (paf->type == gsn_whiteout)	break;
		}
		if (paf->owner != ch) {
			send_to_char( "You can't see a thing through the snow!\n\r",ch);
			return FALSE;
		}
	}

    return TRUE;
}

/* changes your scroll */
void do_scroll(CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    char buf[100];
    int lines;

    one_argument(argument,arg);

    if (arg[0] == '\0')
    {
	if (ch->lines == 0)
	    send_to_char("You do not page long messages.\n\r",ch);
	else
	{
	    sprintf(buf,"You currently display %d lines per page.\n\r",
		    ch->lines + 2);
	    send_to_char(buf,ch);
	}
	return;
    }

    if (!is_number(arg))
    {
	send_to_char("You must provide a number.\n\r",ch);
	return;
    }

    lines = atoi(arg);

    if (lines == 0)
    {
        send_to_char("Paging disabled.\n\r",ch);
        ch->lines = 0;
        return;
    }

    if (lines < 10 || lines > 100)
    {
	send_to_char("You must provide a reasonable number.\n\r",ch);
	return;
    }

    sprintf(buf,"Scroll set to %d lines.\n\r",lines);
    send_to_char(buf,ch);
    ch->lines = lines - 2;
}

/* RT does socials */
void do_socials(CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    int iSocial;
    int col;

    col = 0;
    for (iSocial = 0; social_table[iSocial].name[0] != '\0'; iSocial++)
    {
	sprintf(buf,"%-12s",social_table[iSocial].name);
	send_to_char(buf,ch);
	if (++col % 2 == 0) {
	    send_to_char("\n\r",ch);
	    return;
	}
    }

    if ( col % 6 != 0)
	send_to_char("\n\r",ch);
    return;
}



/* RT Commands to replace news, motd, imotd, etc from ROM */

void do_motd(CHAR_DATA *ch, char *argument)
{
    do_help(ch,"motd");
}

void do_imotd(CHAR_DATA *ch, char *argument)
{
    do_help(ch,"imotd");
}

void do_rules(CHAR_DATA *ch, char *argument)
{
    do_help(ch,"rules");
}

void do_story(CHAR_DATA *ch, char *argument)
{
    do_help(ch,"story");
}

void do_wizlist(CHAR_DATA *ch, char *argument)
{
    do_help(ch,"wizlist");
	return;
}


/* RT this following section holds all the auto commands from ROM, as well as
   replacements for config */

void do_autolist(CHAR_DATA *ch, char *argument)
{
    /* lists most player flags */
    if (IS_NPC(ch))
      return;

    send_to_char("   action     status\n\r",ch);
    send_to_char("---------------------\n\r",ch);

    send_to_char("autoabort      ",ch);
    if (IS_SET(ch->act,PLR_AUTOABORT))
	send_to_char("ON\n\r",ch);
    else
	send_to_char("OFF\n\r",ch);

    send_to_char("autoassist     ",ch);
    if (IS_SET(ch->act,PLR_AUTOASSIST))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch);

    send_to_char("autoexit       ",ch);
    if (IS_SET(ch->act,PLR_AUTOEXIT))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch);

    send_to_char("autogold       ",ch);
    if (IS_SET(ch->act,PLR_AUTOGOLD))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch);

    send_to_char("autoloot       ",ch);
    if (IS_SET(ch->act,PLR_AUTOLOOT))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch);

    send_to_char("autosac        ",ch);
    if (IS_SET(ch->act,PLR_AUTOSAC))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch);

    send_to_char("autosplit      ",ch);
    if (IS_SET(ch->act,PLR_AUTOSPLIT))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch);

    send_to_char("compact mode   ",ch);
    if (IS_SET(ch->comm,COMM_COMPACT))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch);

    send_to_char("prompt         ",ch);
    if (IS_SET(ch->comm,COMM_PROMPT))
	send_to_char("ON\n\r",ch);
    else
	send_to_char("OFF\n\r",ch);

    send_to_char("color          ",ch);
    if (IS_SET(ch->comm,COMM_ANSI))
	send_to_char("ON\n\r",ch);
    else
	send_to_char("OFF\n\r",ch);

    send_to_char("combine items  ",ch);
    if (IS_SET(ch->comm,COMM_COMBINE))
	send_to_char("ON\n\r",ch);
    else
	send_to_char("OFF\n\r",ch);

    if (IS_SET(ch->act,PLR_NOSUMMON))
	send_to_char("You cannot be summoned by anyone out of PK.\n\r",ch);
    else
	send_to_char("You can be summoned by anyone.\n\r",ch);

    if (IS_SET(ch->act,PLR_NOFOLLOW))
	send_to_char("You do not welcome followers.\n\r",ch);
    else
	send_to_char("You accept followers.\n\r",ch);

}

void do_color(CHAR_DATA *ch, char *argument)
{
    int i, eventnum;
    char buf[MAX_STRING_LENGTH];
    char arg1[MAX_STRING_LENGTH], arg2[MAX_STRING_LENGTH];
    char *color;
    if (IS_NPC(ch))
	return;
    if(!str_prefix(argument,"list") && str_cmp(argument,""))
    {
	send_to_char("The following colors are available:\n\r",ch);
    	for(i=0;i<MAX_COLORS;i++)
	{
		sprintf(buf,"%i) %s%s%s\n\r",i+1,color_table[i].color_ascii,color_table[i].color_name,END_COLOR(ch));
		send_to_char(buf,ch);
	}
	return;
    }
    if(!str_prefix(argument, "toggle") && str_cmp(argument,"")) {
    if(IS_SET(ch->comm,COMM_ANSI))
    {
	send_to_char("Color disabled.\n\r",ch);
        REMOVE_BIT(ch->comm,COMM_ANSI);
    }
    else
    {
	send_to_char("Color enabled.\n\r",ch);
        SET_BIT(ch->comm,COMM_ANSI);
    }
    return;
    }
    if(!str_cmp(argument,""))
    {
    send_to_char("Your color scheme is:\n\r",ch);
    for(i=0;i<MAX_EVENTS;i++)
    {
	if(get_trust(ch)<color_event[i].min_level)
		continue;
	sprintf(buf,"%i) %-20s (currently %s%s%s)\n\r",i+1,capitalize(color_event[i].event_name),
	get_char_color(ch,ch->pcdata->color_scheme[i]), ch->pcdata->color_scheme[i],END_COLOR(ch));
	send_to_char(buf,ch);
    }
    sprintf(buf,"Your color is %s.  Use color TOGGLE to switch color on/off.\n\r",IS_SET(ch->comm,COMM_ANSI) ? "ON" : "OFF");
    send_to_char(buf,ch);
    send_to_char("Use color <event> <color> to change the color of a given event.\n\r",ch);
    send_to_char("Use color list to see a list of available colors.\n\r",ch);
    return;
    }

    argument = one_argument(argument,arg1);
    argument = one_argument(argument,arg2);

    if(arg1[0]=='\0' || arg2[0]=='\0')
		return send_to_char("Invalid choices.\n\r",ch);
	
    if((eventnum = get_event_number(arg1))<0)
		return send_to_char("Invalid event.\n\r",ch);
	
    if(!(color=get_color_name(arg2)))
		return send_to_char("Invalid color selection.\n\r",ch);
	
    ch->pcdata->color_scheme[eventnum] = get_color_name(arg2);
    sprintf(buf,"%s has been color remapped to %s.\n\r",capitalize(color_event[eventnum].event_name),color);
    send_to_char(buf,ch);
}

void do_autoabort(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;

    if (IS_SET(ch->act,PLR_AUTOABORT))
    {
      send_to_char("Autoabort removed.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_AUTOABORT);
    }
    else
    {
      send_to_char("You will now automatically exit edit mode when attacked.\n\r",ch);
      SET_BIT(ch->act,PLR_AUTOABORT);
    }
}

void do_autoassist(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;

    if (IS_SET(ch->act,PLR_AUTOASSIST))
    {
      send_to_char("Autoassist removed.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_AUTOASSIST);
    }
    else
    {
      send_to_char("You will now assist when needed.\n\r",ch);
      SET_BIT(ch->act,PLR_AUTOASSIST);
    }
}

void do_autoexit(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;

    if (IS_SET(ch->act,PLR_AUTOEXIT))
    {
      send_to_char("Exits will no longer be displayed.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_AUTOEXIT);
    }
    else
    {
      send_to_char("Exits will now be displayed.\n\r",ch);
      SET_BIT(ch->act,PLR_AUTOEXIT);
    }
}

void do_autogold(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;

    if (IS_SET(ch->act,PLR_AUTOGOLD))
    {
      send_to_char("Autogold removed.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_AUTOGOLD);
    }
    else
    {
      send_to_char("Automatic gold looting set.\n\r",ch);
      SET_BIT(ch->act,PLR_AUTOGOLD);
    }
}

void do_autoloot(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;

    if (IS_SET(ch->act,PLR_AUTOLOOT))
    {
      send_to_char("Autolooting removed.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_AUTOLOOT);
    }
    else
    {
      send_to_char("Automatic corpse looting set.\n\r",ch);
      SET_BIT(ch->act,PLR_AUTOLOOT);
    }
}

void do_autosac(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;

    if (IS_SET(ch->act,PLR_AUTOSAC))
    {
      send_to_char("Autosacrificing removed.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_AUTOSAC);
    }
    else
    {
      send_to_char("Automatic corpse sacrificing set.\n\r",ch);
      SET_BIT(ch->act,PLR_AUTOSAC);
    }
}

void do_autosplit(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;

    if (IS_SET(ch->act,PLR_AUTOSPLIT))
    {
      send_to_char("Autosplitting removed.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_AUTOSPLIT);
    }
    else
    {
      send_to_char("Automatic gold splitting set.\n\r",ch);
      SET_BIT(ch->act,PLR_AUTOSPLIT);
    }
}

void do_brief(CHAR_DATA *ch, char *argument)
{
    if (IS_SET(ch->comm,COMM_BRIEF))
    {
      send_to_char("Full descriptions activated.\n\r",ch);
      REMOVE_BIT(ch->comm,COMM_BRIEF);
    }
    else
    {
      send_to_char("Short descriptions activated.\n\r",ch);
      SET_BIT(ch->comm,COMM_BRIEF);
    }
}

void do_compact(CHAR_DATA *ch, char *argument)
{
    if (IS_SET(ch->comm,COMM_COMPACT))
    {
      send_to_char("Compact mode removed.\n\r",ch);
      REMOVE_BIT(ch->comm,COMM_COMPACT);
    }
    else
    {
      send_to_char("Compact mode set.\n\r",ch);
      SET_BIT(ch->comm,COMM_COMPACT);
    }
}

void do_show(CHAR_DATA *ch, char *argument)
{
    if (IS_SET(ch->comm,COMM_SHOW_AFFECTS))
    {
      send_to_char("Affects will no longer be shown in score.\n\r",ch);
      REMOVE_BIT(ch->comm,COMM_SHOW_AFFECTS);
    }
    else
    {
      send_to_char("Affects will now be shown in score.\n\r",ch);
      SET_BIT(ch->comm,COMM_SHOW_AFFECTS);
    }
}

void do_prompt(CHAR_DATA *ch, char *argument)
{
   char buf[MAX_STRING_LENGTH];

   if(!str_cmp(argument,""))
   {
	send_to_char("Syntax: prompt toggle\n\r",ch);
	send_to_char("Syntax: prompt default\n\r",ch);
	send_to_char("Syntax: prompt (string)\n\r\n\r",ch);
	send_to_char("The following key phrases may be used in the (string) and will be translated:\n\r",ch);
	send_to_char("%h: Current HP      %H: Maximum HP\n\r",ch);
	send_to_char("%m: Current Mana    %M: Maximum Mana\n\r",ch);
	send_to_char("%v: Current Moves   %V: Maximum Moves\n\r",ch);
	send_to_char("%1: % of Max HP     %2: % of Max Mana\n\r",ch);
	send_to_char("%3: % of Max MV\n\r",ch);
	send_to_char("%x: Total XP        %X: XP TNL\n\r",ch);
	send_to_char("%g: Total Gold      %r: Room Name\n\r",ch);
	send_to_char("%e: Room Exits      %c: Newline (for long prompts)\n\r",ch);
	send_to_char("%L: Lag Status (+ is lagged, and - is not)\n\r", ch);
	if(IS_IMMORTAL(ch))
	send_to_char("%z: Area Name       %R: Room Vnum\n\r",ch);
	if(IS_IMMORTAL(ch))
	send_to_char("%p: Mortals in Area %P: All PCs in area\n\r",ch);
	if(IS_IMMORTAL(ch))
	send_to_char("%C: Player Count\n\r",ch);
	send_to_char("Example: prompt <%hhp %mm %vmv> will set your prompt to <10hp 100m 100mv>.\n\r",ch);
	send_to_char("Prompt toggle will toggle whether you see prompts or not.\n\r",ch);
	send_to_char("Prompt default will set your prompt to the default <hp mana moves> format.\n\r",ch);
	return;
   }
   if(!str_prefix(argument,"toggle")) {
	if (IS_SET(ch->comm,COMM_PROMPT))
   	{
      	    send_to_char("Prompts disabled.\n\r",ch);
      	    REMOVE_BIT(ch->comm,COMM_PROMPT);
    	}
    	else
    	{
      	    send_to_char("Prompts enabled.\n\r",ch);
      	    SET_BIT(ch->comm,COMM_PROMPT);
    	}
       return;
   }
   if( !strcmp(argument, "default" ) )
      strcpy( buf, "<%hhp %mm %vmv> ");
   else
   {
      if ( strlen(argument) > 75 )
         argument[75] = '\0';
      strcpy( buf, argument );
      smash_tilde( buf );
      if (str_suffix("%c",buf))
	strcat(buf," ");
	
   }

   free_pstring( ch->prompt );
   ch->prompt = palloc_string( buf );
   sprintf(buf,"Your prompt is now set to %s.\n\r",ch->prompt );
   send_to_char(buf,ch);
   return;
}

void do_combine(CHAR_DATA *ch, char *argument)
{
    if (IS_SET(ch->comm,COMM_COMBINE))
    {
      send_to_char("Long inventory selected.\n\r",ch);
      REMOVE_BIT(ch->comm,COMM_COMBINE);
    }
    else
    {
      send_to_char("Combined inventory selected.\n\r",ch);
      SET_BIT(ch->comm,COMM_COMBINE);
    }
}

void do_nofollow(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;

    if (IS_AFFECTED(ch,AFF_CHARM))
	{
	 send_to_char("Now why would you want to leave your master?\n\r",ch);
	 return;
	}

    if (IS_SET(ch->act,PLR_NOFOLLOW))
    {
      send_to_char("You now accept followers.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_NOFOLLOW);
    }
    else
    {
      send_to_char("You no longer accept followers.\n\r",ch);
      SET_BIT(ch->act,PLR_NOFOLLOW);
      die_follower( ch );
    }	
}

void do_nosummon(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
    {
      if (IS_SET(ch->imm_flags,IMM_SUMMON))
      {
	send_to_char("You may now be summoned by anyone.\n\r",ch);
	REMOVE_BIT(ch->imm_flags,IMM_SUMMON);
      }
      else
      {
	send_to_char("You may no longer be summoned.\n\r",ch);
	SET_BIT(ch->imm_flags,IMM_SUMMON);
      }
    }
    else
    {
      if (IS_SET(ch->act,PLR_NOSUMMON))
      {
        send_to_char("You may now be summoned by anyone.\n\r",ch);
        REMOVE_BIT(ch->act,PLR_NOSUMMON);
      }
      else
      {
        send_to_char("You may only be summoned by those in PK.\n\r",ch);
        SET_BIT(ch->act,PLR_NOSUMMON);
      }
    }
}

void do_glance( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;

    if ( ch->desc == NULL )
	return;

    if ( argument[0] == '\0' )
    {
	send_to_char( "Glance at whom?\n\r", ch );
	return;
    }

    if ( ch->position < POS_SLEEPING )
    {
	send_to_char( "You can't see anything but stars!\n\r", ch );
	return;
    }

    if ( ch->position == POS_SLEEPING )
    {
	send_to_char( "You can't see anything, you're sleeping!\n\r", ch );
	return;
    }

    if ( !check_blind( ch ) )
	return;

    if ( (victim = get_char_room(ch, argument)) == NULL )
    {
	send_to_char( "That person isn't here.\n\r", ch );
	return;
    }
    if (is_affected(victim,gsn_cloak_form))
    {
        send_to_char("The figure is buried deep within a dark colored cloak.\n\r",ch);
        return;
    }

    show_char_to_char_2(victim,ch);
    return;
}

void do_examine( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    if ( ch->desc == NULL )
	return;
    one_argument(argument,arg);

    if ( arg[0] == '\0' )
    {
	send_to_char( "Examine what?\n\r", ch );
	return;
    }

    if ( ch->position < POS_SLEEPING )
    {
	send_to_char( "You can't see anything but stars!\n\r", ch );
	return;
    }

    if ( ch->position == POS_SLEEPING )
    {
	send_to_char( "You can't see anything, you're sleeping!\n\r", ch );
	return;
    }

    if ( !check_blind( ch ) )
	return;

    if ( (obj = get_obj_here(ch,arg)) != NULL)
    {
	switch(obj->item_type)
   	{
	case (ITEM_DRINK_CON):
	case (ITEM_CONTAINER):
	case (ITEM_CORPSE_NPC):
	case (ITEM_CORPSE_PC):
		sprintf(buf,"in %s",arg);
		break;
	default:
		sprintf(buf,"%s",arg);
		break;
	}
	do_look(ch,buf);
    }
    else
	do_look(ch,arg);
    return;
}

static char * const moon_look[MAX_MOON] =
{
	"new",
	"crescent waxing",
	"half waxing",
	"gibbous waxing",
	"full",
	"gibbous waning",
	"half waning",
	"crescent waning"
};

bool   show_altdesc(ROOM_INDEX_DATA *room)
{
	if(!room->alt_description)
		return FALSE;
	if(room->alt_description_cond == AD_COND_NIGHT && sun == SUN_DARK)
		return TRUE;
	return FALSE;
}
char * get_room_description(ROOM_INDEX_DATA *room)
{
	if(!room->alt_description)
		return room->description;
	if(show_altdesc(room))
		return room->alt_description;
	return room->description;
}
char * get_room_name(ROOM_INDEX_DATA *room)
{
	if(!room->alt_description)
		return room->name;
	if(show_altdesc(room))
		return room->alt_name;
	return room->name;	
}

void do_look( CHAR_DATA *ch, char *argument )
{
    char buf  [MAX_STRING_LENGTH];
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    EXIT_DATA *pexit;
    CHAR_DATA *victim;
    OBJ_DATA *obj, *crystal;
	ROOM_INDEX_DATA *in_room;
    char *pdesc, *direction;
    int door;
    int number,count, i;
	bool found = FALSE;
	AFFECT_DATA *af;
    ROOM_AFFECT_DATA *raf;

    if ( ch->desc == NULL )
		return;

    if ( ch->position < POS_SLEEPING )
		return send_to_char( "You can't see anything but stars!\n\r", ch );

    if ( ch->position == POS_SLEEPING )
		return send_to_char( "You can't see anything, you're sleeping!\n\r", ch );

    if ( !check_blind( ch ) )
		return;

    if ( !IS_NPC(ch)
    	&&   !IS_SET(ch->act, PLR_HOLYLIGHT)
    	&&   room_is_dark( ch->in_room )
    	&&   !IS_AFFECTED(ch, AFF_DARK_VISION) )
	{
		send_to_char( "It is pitch black ... \n\r", ch );
		show_char_to_char( ch->in_room->people, ch );
		return;
    }

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    number = number_argument(arg1,arg3);
    count = 0;

    if ( arg1[0] == '\0' || !str_cmp( arg1, "auto" ) )
    {
	/* 'look' or 'look auto' */

	if(get_bv_stage(ch) < 2)
	{
		sprintf(buf, get_room_name(ch->in_room));
		buf[0] = UPPER(buf[0]);
		send_to_char(buf, ch );

		if ((IS_IMMORTAL(ch) && (IS_NPC(ch) || IS_SET(ch->act,PLR_HOLYLIGHT))) || 
			(!IS_NPC(ch) && IS_HEROIMM(ch) && strstr(ch->in_room->area->builders, ch->name)))
		{
	    		sprintf(buf," [Room %d]",ch->in_room->vnum);
	    		send_to_char(buf,ch);
			if(show_altdesc(ch->in_room))
				send_to_char(" [Alt Desc]", ch);
		}
		if(IS_IMMORTAL(ch))
		{
			for(victim = ch->in_room->people; victim; victim = victim->next_in_room)
				if(IS_NPC(victim) && IS_SET(victim->act, ACT_WARD_MOB))
				{
					send_to_char(" [Ward Mob]",ch);
					break;
				}
		}
		
		send_to_char( "\n\r", ch );
	}

	if ( (arg1[0] == '\0'
	|| ( !IS_NPC(ch) && !IS_SET(ch->comm, COMM_BRIEF) )) && !is_affected(ch,gsn_plasma_arc) && (get_bv_stage(ch) < 2))
	{
	    send_to_char( "  ",ch);
	    send_to_char(get_room_description(ch->in_room), ch );
	}

    if ( (IS_NPC(ch) || (!IS_NPC(ch) && IS_SET(ch->act, PLR_AUTOEXIT))) && (get_bv_stage(ch) < 1))
	{
	    send_to_char("\n\r",ch);
            do_exits( ch, "auto" );
	}	

	if(is_affected_room(ch->in_room, gsn_conflagration) && ch->in_room->sector_type == SECT_CONFLAGRATION)
	{
		sprintf(buf,"%sA wave of heat washes over you as a raging inferno blazes throughout the area!%s\n\r",
			get_char_color(ch,"lightred"), END_COLOR(ch));
		send_to_char(buf,ch);
	}

	if(is_affected_room(ch->in_room, gsn_essence_of_plasma)) 
	{
		sprintf(buf,"%sAn orb of pure plasma whizzes erratically through the area!%s\n\r",
			get_char_color(ch,"lightmagenta"), END_COLOR(ch));
		send_to_char(buf,ch);
	}

	if(is_affected_room(ch->in_room, gsn_glaciate) && ch->in_room->sector_type == SECT_ICE) 
	{
		sprintf(buf,"%sThe water here has congealed into a thick glacial icefloe!%s\n\r",
			get_char_color(ch,"white"), END_COLOR(ch));
		send_to_char(buf,ch);
	}
	
	if(is_affected_room(ch->in_room, gsn_caustic_vapor)) 
	{
		sprintf(buf,"%sA thick cloud of noxious vapors hangs over the area.%s\n\r",
			get_char_color(ch,"lightgreen"), END_COLOR(ch));
		send_to_char(buf,ch);
	}

	if(is_affected_room(ch->in_room, gsn_blanket)) 
	{
		sprintf(buf,"%sA layer of glimmering snow blankets the area.%s\n\r",
				get_char_color(ch,"white"), END_COLOR(ch));
		send_to_char(buf,ch);
	};
	
	if(is_affected_room(ch->in_room, gsn_flood))
	{
		sprintf(buf,"%sThe area is entirely submerged beneath a massive flood!%s\n\r",
			get_char_color(ch,"cyan"), END_COLOR(ch));
		send_to_char(buf,ch);
	}
	
	if(is_affected_room(ch->in_room, gsn_riptide))
	{
		for (raf = ch->in_room->affected; raf != NULL; raf = raf->next)
		{
			if (raf->type == gsn_riptide && raf->owner == ch && raf->location == APPLY_ROOM_NONE && raf->modifier == 1)
			{
			sprintf(buf,"%sThe calm surface of the water belies the deadly riptide churning beneath!%s\n\r",
				get_char_color(ch,"cyan"), END_COLOR(ch));
			send_to_char(buf,ch);
			break;
			} else if (raf->type == gsn_riptide && raf->owner == ch && raf->location == APPLY_ROOM_NONE && raf->modifier == 2)
			{
			sprintf(buf,"%sThe waters swirl menacingly, ready to receive the riptide's prey.%s\n\r",
				get_char_color(ch,"cyan"), END_COLOR(ch));
			send_to_char(buf,ch);
			break;
			}
		}
	}

	if (is_affected(ch,gsn_mark_of_wrath)) 
	{
		af = affect_find(ch->affected,gsn_mark_of_wrath);
		for (i=0;i<MAX_TRACKS;i++) {
			if (!ch->in_room->tracks[i])
				break;
			if (ch->in_room->tracks[i]->prey != af->owner)
				continue;
			direction = (char*)flag_name_lookup(ch->in_room->tracks[i]->direction,direction_table);
			sprintf(buf,"%sThrough the veil of your rage, you sense %s's tracks leading %s.%s\n\r",
				get_char_color(ch,"lightred"),
				af->owner->name,
				direction,
				END_COLOR(ch));
			send_to_char(buf,ch);
		}
	}
			
	
	if (ch->in_room->sector_type == SECT_SNOW) {
		for (i=0;i<=2;i++) 
		{
			if (!ch->in_room->tracks[i])	
				continue;
			if (!ch->in_room->tracks[i]->prey)
				continue;
			if (ch->in_room->tracks[i]->flying)
				continue;
			direction = (char *)flag_name_lookup(ch->in_room->tracks[i]->direction,direction_table);
			sprintf(buf,"%sYou see %s footprints leading %s through the snow.%s\n\r",
				get_char_color(ch,"white"),
				(ch->in_room->tracks[i]->prey->race == 4) ? "elven" :
					pc_race_table[ch->in_room->tracks[i]->prey->race].race_time,
				direction,
				END_COLOR(ch));
			send_to_char(buf,ch);
		}
	}
			
			
	show_list_to_char( ch->in_room->contents, ch, FALSE, FALSE );
	show_char_to_char( ch->in_room->people,   ch );
	return;
    }

	pdesc = get_extra_descr(arg3,ch->in_room->extra_descr);                                                       
    if (pdesc != NULL)                                                                                            
		if (++count == number)                                                                                    
			return send_to_char(pdesc,ch);
	
    if ( !str_cmp( arg1, "i" ) || !str_cmp(arg1, "in")  || !str_cmp(arg1,"on"))
    {
		/* 'look in' */
		if ( arg2[0] == '\0' )
			return send_to_char( "Look in what?\n\r", ch );

		if (is_affected(ch,gsn_plasma_arc))
			return send_to_char("You can't seem to see that!\n\r",ch);

		if ( ( obj = get_obj_here( ch, arg2 ) ) == NULL )
			return send_to_char( "You do not see that here.\n\r", ch );

		switch ( obj->item_type )
		{
			default:
	    		send_to_char( "That is not a container.\n\r", ch );
	    	break;

			case ITEM_DRINK_CON:
	    		if ( obj->value[1] <= 0 )
	    		{
					send_to_char( "It is empty.\n\r", ch );
					break;
	    		}

	    		sprintf( buf, "It's %sfilled with a %s liquid.\n\r",
					obj->value[1] <     obj->value[0] / 4
		    		? "less than half-" :
					obj->value[1] < 3 * obj->value[0] / 4
		    		? "about half-"     : "more than half-",
					liq_table[obj->value[2]].liq_color);
	    		send_to_char( buf, ch );
	    	break;

			case ITEM_CONTAINER:
			case ITEM_CORPSE_NPC:
			case ITEM_CORPSE_PC:
	    		if ( IS_SET_OLD(obj->value[1], CONT_CLOSED) )
	    		{
					send_to_char( "It is closed.\n\r", ch );
					break;
	    		}

				if (is_affected_obj(obj, gsn_ice_blast))
				{
					act("$p is frozen shut!",ch,obj,0,TO_CHAR);
					break;
				}
		
				act( "$p holds:", ch, obj, NULL, TO_CHAR );
				if(obj->contains)
					show_list_to_char( obj->contains, ch, TRUE, TRUE );
				else
					send_to_char("Nothing.\n\r",ch);
	    	break;
		}
		return;
    }
	
	
	if (!str_cmp(arg1,"berus") || !str_cmp(arg1,"calabren")) 
	{
		char buf2[MSL];
		bool isCalabren = !str_cmp(arg1, "calabren");
		if (!IS_OUTSIDE(ch))
			return send_to_char("You cannot see the sky, much less the moons!\n\r", ch);
		
		if (sun == SUN_LIGHT)
			return send_to_char("You cannot make out the moons during the day.\n\r", ch);
		
		if (ch->in_room->area->sky == SKY_OVERCAST ||
			ch->in_room->area->sky >= SKY_DOWNPOUR)
			return send_to_char("You cannot make out the moons through the cloud cover.\n\r", ch);
		sprintf(buf2,"The %s moon of %s orbits above you in the night sky.\n\r",
				moon_look[isCalabren ? moon_calabren : moon_berus], isCalabren ? "Calabren" : "Berus"); 
		send_to_char(buf2,ch);
		return;
	}
	
    if ( ( victim = get_char_room( ch, arg1 ) ) != NULL )
	{
		if (is_affected(ch,gsn_plasma_arc))
			return send_to_char("You can't seem to see that!\n\r",ch);
		if(IS_IMMORTAL(victim))
			act("$n looks at you.",ch,0,victim,TO_VICT);
		show_char_to_char_1( victim, ch );
		return;
    }

    for ( obj = ch->in_room->contents; obj != NULL; obj = obj->next_content )
	{
		if ( can_see_obj( ch, obj ) )
		{
			pdesc = get_extra_descr( arg3, obj->extra_descr );
			if ( pdesc != NULL )
			{
		    	if (++count == number)
				{
					found = TRUE;	
		    		send_to_char( pdesc, ch );
					break;
				} 
				else 
				{
					continue;
				}
			}

		    pdesc = get_extra_descr( arg3, obj->pIndexData->extra_descr );
		    if ( pdesc != NULL )
			{
	    		if (++count == number)
				{
					found = TRUE;
					send_to_char( pdesc, ch);
					break;
				} 
				else 
				{
					continue;
				}
			}

			if ( is_name( arg3, obj->name ) )
				if (++count == number)
				{
					found = TRUE;
					send_to_char( obj->description, ch );
				    send_to_char("\n\r",ch);
					break;
				}
		}
    }

    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
	{
		if ( can_see_obj( ch, obj ) ) 
		{  /* player can see object */
	    	pdesc = get_extra_descr( arg3, obj->extra_descr );
	    	if ( pdesc != NULL )
			{
		    	if (++count == number)
				{
					found = TRUE;
		    		send_to_char( pdesc, ch );
					break;
				} 
				else 
				{
					continue;
				}
			}
			
			pdesc = get_extra_descr( arg3, obj->pIndexData->extra_descr );
			if ( pdesc != NULL )
			{
				if (++count == number)
				{
					found = TRUE;
					send_to_char(pdesc,ch);
					break;
				} 
				else 
				{
					continue;
				}
			}
			
		    if ( is_name( arg3, obj->name ) )
	    		if (++count == number)
				{
					found = TRUE;
					send_to_char (obj->description, ch);
					send_to_char ("\n\r",ch);
					break;
				}
		}	
	}

	if (obj && (obj->item_type == ITEM_URN))
	{
		sprintf(buf,"Peering into the urn, you estimate that it currently holds %s ounce%s of blood.\n\r",
			int_to_string(obj->value[4]), (obj->value[4] == 1) ? "" : "s");
		send_to_char(buf,ch);
	}

	if (obj && (obj->item_type == ITEM_SCROLL)
		&& (obj->pIndexData->vnum == OBJ_VNUM_SCRIBE))
	{
		if(ch->cabal == CABAL_SCION)
		{
			sprintf(buf,"Deciphering the symbols, you realize this scroll contains the spell %s.\n\r",
				skill_table[obj->value[1]].name);
			send_to_char(buf, ch);
		}
	}

	if (found)
		return;

	if (obj && (obj->pIndexData->vnum == OBJ_VNUM_CHESSBOARD))
		print_chessboard(ch);
	
	if (obj && (obj->item_type == ITEM_SCROLL)
		&& (obj->pIndexData->vnum == OBJ_VNUM_SCRIBE))
	{
		if(ch->cabal == CABAL_SCION)
		{
			sprintf(buf,"Deciphering the symbols, you realize this scroll contains the spell %s.\n\r",
				skill_table[obj->value[1]].name);
			send_to_char(buf, ch);
		}
	}
	
	if (obj && obj->item_type == ITEM_URN)
	{
		sprintf(buf,"Peering into the urn, you estimate that it currently holds %s ounce%s of blood.\n\r",
			int_to_string(obj->value[4]), (obj->value[4] == 1) ? "" : "s");
		send_to_char(buf,ch);
	}

	if (found)
		return;

    if (count > 0 && count != number)
	{
    	if (count == 1)
    	    sprintf(buf,"You only see one %s here.\n\r",arg3);
    	else
    	    sprintf(buf,"You only see %d of those here.\n\r",count);
    	
    	send_to_char(buf,ch);
    	return;
    }

	if ( !str_cmp( arg1, "n" ) || !str_cmp( arg1, "north" ) )
		door = 0;
	else if ( !str_cmp( arg1, "e" ) || !str_cmp( arg1, "east"  ) ) 
		door = 1;
	else if ( !str_cmp( arg1, "s" ) || !str_cmp( arg1, "south" ) ) 
		door = 2;
	else if ( !str_cmp( arg1, "w" ) || !str_cmp( arg1, "west"  ) ) 
		door = 3;
	else if ( !str_cmp( arg1, "u" ) || !str_cmp( arg1, "up"    ) ) 
		door = 4;
	else if ( !str_cmp( arg1, "d" ) || !str_cmp( arg1, "down"  ) ) 
		door = 5;
	else
		return send_to_char( "You do not see that here.\n\r", ch );

    /* 'look direction' */

	/* if the current room is outside, up should show weather.. */

	pexit = ch->in_room->exit[door];

	if (door == 4 && IS_OUTSIDE(ch) && pexit == NULL)
		return do_weather(ch, "");
	
	if ( pexit == NULL )
		return send_to_char( "Nothing special there.\n\r", ch );

    
    if (pexit->u1.to_room && is_affected_room(pexit->u1.to_room,gsn_conflagration))
		return send_to_char("Billowing smoke and towering flames obscure your ability"\
							" to see anything in that direction.\n\r",ch);

	if (is_affected_room(pexit->u1.to_room, gsn_smokescreen) || is_affected_room(ch->in_room, gsn_smokescreen))
		return send_to_char("Smoke disrupts your vision and prevents you from looking around there.\n\r", ch);

    if ( pexit->description != NULL && pexit->description[0] != '\0' )
		send_to_char( pexit->description, ch );
    else
		send_to_char( "Nothing special there.\n\r", ch );

	if ( pexit && !IS_SET(pexit->exit_info, EX_CLOSED)
		&& is_affected(ch,gsn_farsee)
		&& ((crystal = get_eq_char(ch,WEAR_HOLD)) != NULL)
		&& pexit->u1.to_room
		&& crystal->pIndexData->vnum == OBJ_VNUM_CRYSTAL) {
		send_to_char("Focusing through your crystal, you extend your sights....\n\r",ch);
		in_room = ch->in_room;
		char_from_room(ch);
		char_to_room(ch,pexit->u1.to_room);
		do_look(ch,"auto");
		char_from_room(ch);
		char_to_room(ch,in_room);
	}
	
    if ( pexit->keyword    != NULL
    &&   pexit->keyword[0] != '\0'
    &&   pexit->keyword[0] != ' ' )
    {
	if ( IS_SET(pexit->exit_info, EX_CLOSED) && !IS_SET(pexit->exit_info,EX_NONOBVIOUS))
	    act( "The $T is closed.", ch, NULL, pexit->keyword, TO_CHAR );
	else if ( IS_SET(pexit->exit_info, EX_ISDOOR) && !IS_SET(pexit->exit_info,EX_NONOBVIOUS))
	    act( "The $T is open.",   ch, NULL, pexit->keyword, TO_CHAR );
    }

    return;
}

/* RT added back for the hell of it */
void do_read (CHAR_DATA *ch, char *argument )
{
    do_look(ch,argument);
}


/*
 * Thanks to Zrin for auto-exit part.
 */
void do_exits( CHAR_DATA *ch, char *argument )
{
    extern char * const dir_name[];
    char buf[MAX_STRING_LENGTH];
    EXIT_DATA *pexit;
    bool found;
    bool fAuto;
    int door;

    fAuto  = !str_cmp( argument, "auto" );

    if ( !check_blind( ch ) )
	return;

	if(is_affected_room(ch->in_room, gsn_smokescreen)) {
		sprintf(buf,"%sDense smoke prevents any vision beyond this room!%s\n\r",
			get_char_color(ch,"darkgrey"), END_COLOR(ch));
		send_to_char(buf,ch);
	}

    if (fAuto)
	sprintf(buf,"[Exits:");
    else if (IS_IMMORTAL(ch))
	sprintf(buf,"Obvious exits from room %d:\n\r",ch->in_room->vnum);
    else
	sprintf(buf,"Obvious exits:\n\r");

    found = FALSE;
    for ( door = 0; door <= 5; door++ )
    {
	if ( ( pexit = ch->in_room->exit[door] ) != NULL
	&&   pexit->u1.to_room != NULL
	&&   can_see_room(ch,pexit->u1.to_room)
	&&   (!IS_SET(pexit->exit_info, EX_NONOBVIOUS) || IS_IMMORTAL(ch))
	&&   (!is_affected_room(ch->in_room,gsn_smokescreen)))
	{
	    found = TRUE;
	    if ( fAuto )
	    {
		strcat( buf, " " );
		if (IS_SET(pexit->exit_info,EX_CLOSED) || IS_SET(pexit->exit_info,EX_NONOBVIOUS))
			strcat(buf, "(");
		strcat( buf, dir_name[door] );
		if (IS_SET(pexit->exit_info,EX_CLOSED) || IS_SET(pexit->exit_info,EX_NONOBVIOUS))
			strcat(buf, ")");
	    }
	    else
	    {
		sprintf( buf + strlen(buf), "%-5s - %s",
		    capitalize( dir_name[door] ),
		    room_is_dark( pexit->u1.to_room )
			?  "Too dark to tell"
			: get_room_name(pexit->u1.to_room)
		    );
		if (IS_IMMORTAL(ch))
		    sprintf(buf + strlen(buf),
			" %s%s(room %d)\n\r",IS_SET(pexit->exit_info,EX_CLOSED) ? "(CLOSED) " : "",
					     IS_SET(pexit->exit_info,EX_NONOBVIOUS) ? "(NONOBVIOUS) "
					     : "", pexit->u1.to_room->vnum);
		else
		    sprintf(buf + strlen(buf), "\n\r");
	    }
	}
    }

    if ( !found )
	strcat( buf, fAuto ? " none" : "None.\n\r" );

    if ( fAuto )
	strcat( buf, "]\n\r" );

    send_to_char( buf, ch );
    return;
}

void do_worth( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    if (IS_NPC(ch))
    {
	sprintf(buf,"You have %ld gold.\n\r",
	    ch->gold);
	send_to_char(buf,ch);
	return;
    }

    sprintf(buf,
    "You have %ld gold and %d experience (%d exp to level).\n\r",
	ch->gold, ch->exp,
        ch->level * exp_per_level(ch) - ch->exp);

    send_to_char(buf,ch);

    return;
}


void do_score( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char *state;
    int i;

    if ( IS_NPC(ch) )
		return send_to_char( "Nope, not for NPC's. Try stat <mob>.\n\r", ch );

    sprintf( buf,
	"You are %s%s, level %d (%d hours).\n\r",
	ch->true_name,
	ch->pcdata->title,
	ch->level,
	get_hours(ch));
    send_to_char( buf, ch );
    sprintf( buf,
        "You are %d %s years old, born in the Caelumaedani year %d.\n\r",
        get_age(ch), pc_race_table[ch->race].race_time, ch->pcdata->birth_date);
    send_to_char( buf, ch );
    float tmp = (float) get_hours(ch) / (float) ch->pcdata->death_time * 100.001;
	i = (int)tmp;
    if(i<15)
        state = "You feel energetic and ready to take on the world!";
    else if(i<40)
        state = "You feel vigorous and hale.";
    else if(i<65)
        state = "Your arduous life is beginning to take its toll upon your body.";
    else if(i<90)
        state = "You have lived a full life, but your body grows weary of the strain.";
    else
        state = "Your bones ache from a lifetime's toil, and you feel the end approaching.";
    sprintf(buf,"%s\n\r",state);
    send_to_char(buf,ch);
    sprintf(buf, "Race: %s  Sex: %s  Class: %s\n\r",
        race_table[ch->race].name,
		sex_table[ch->sex].name,
        IS_NPC(ch) ? "mobile" : ch->Class()->name);
    send_to_char(buf,ch);

    if ( get_trust( ch ) != ch->level )
    {
	sprintf( buf, "You are trusted at level %d.\n\r",
	    get_trust( ch ) );
	send_to_char( buf, ch );
    }

    sprintf( buf,
	"You have %d/%d hit, %d/%d mana, %d/%d movement.\n\r",
	ch->hit,  ch->max_hit,
	ch->mana, ch->max_mana,
	ch->move, ch->max_move);
    send_to_char( buf, ch );

	if ((ch->Class()->GetIndex() == CLASS_WARRIOR) || ch->pcdata->special > 0) {
    sprintf( buf,
	"You have %d practices and %d training sessions and %d specializations.\n\r",
	ch->practice, ch->train,ch->pcdata->special);
    send_to_char( buf, ch );

	} else {
    sprintf( buf,
	"You have %d practices and %d training sessions.\n\r",
	ch->practice, ch->train);
    send_to_char( buf, ch );
	}
    sprintf( buf, "You have %d proficiency points.\n\r", ch->Profs()->GetPoints());
    send_to_char(buf, ch);

    sprintf( buf,
	"You are carrying %d/%d items with weight %ld/%d pounds.\n\r",
	ch->carry_number, can_carry_n(ch),
	get_carry_weight(ch), can_carry_w(ch) );
    send_to_char( buf, ch );

	sprintf( buf,
	"Str: %d(%d)  Int: %d(%d)  Wis: %d(%d)  Dex: %d(%d)  Con: %d(%d)\n\r",
	ch->perm_stat[STAT_STR],
	get_curr_stat(ch,STAT_STR),
	ch->perm_stat[STAT_INT],
	get_curr_stat(ch,STAT_INT),
	ch->perm_stat[STAT_WIS],
	get_curr_stat(ch,STAT_WIS),
	ch->perm_stat[STAT_DEX],
	get_curr_stat(ch,STAT_DEX),
	ch->perm_stat[STAT_CON],
	get_curr_stat(ch,STAT_CON) );
    send_to_char( buf, ch );

	sprintf( buf,
	"You have scored %d exp, and have %ld gold.\n\r",
	ch->exp,  ch->gold);
    send_to_char( buf, ch );

    /* RT shows exp to level */
    if (!IS_NPC(ch) && ch->level < LEVEL_HERO)
    {
      sprintf (buf,
	"You need %d exp to level.\n\r",
        (ch->level * exp_per_level(ch) - ch->exp));
      send_to_char( buf, ch );
     }

	sprintf( buf, "Wimpy set to %d hit points.  Hometown is %s.\n\r",
		ch->wimpy,hometown_table[ch->hometown].name);
    send_to_char( buf, ch );

    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK]   > 10 )
	send_to_char( "You are drunk.\n\r",   ch );
    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_THIRST] > 35 )
	send_to_char( "You are thirsty.\n\r", ch );
    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_HUNGER] > 35 )
	send_to_char( "You are hungry.\n\r",  ch );

    switch ( ch->position )
    {
    case POS_DEAD:
	send_to_char( "You are DEAD!!\n\r",		ch );
	break;
    case POS_MORTAL:
	send_to_char( "You are mortally wounded.\n\r",	ch );
	break;
    case POS_INCAP:
	send_to_char( "You are incapacitated.\n\r",	ch );
	break;
    case POS_STUNNED:
	send_to_char( "You are stunned.\n\r",		ch );
	break;
    case POS_SLEEPING:
	send_to_char( "You are sleeping.\n\r",		ch );
	break;
    case POS_RESTING:
	send_to_char( "You are resting.\n\r",		ch );
	break;
    case POS_SITTING:
	send_to_char( "You are sitting.\n\r",		ch );
	break;
    case POS_STANDING:
	send_to_char( "You are standing.\n\r",		ch );
	break;
    case POS_FIGHTING:
	send_to_char( "You are fighting.\n\r",		ch );
	break;
    }


    /* print AC values -- print negative AC as 0 */
    if (ch->level >= 30)
    {	
	sprintf( buf,"Armor: pierce: %d  bash: %d  slash: %d  magic: %d\n\r",
		 UMAX(0,GET_AC(ch,AC_PIERCE)),
		 UMAX(0,GET_AC(ch,AC_BASH)),
		 UMAX(0,GET_AC(ch,AC_SLASH)),
		 UMAX(0,GET_AC(ch,AC_EXOTIC)));
    	send_to_char(buf,ch);
    }

    for (i = 0; i < 4; i++)
    {
	char * temp;

	switch(i)
	{
	    case(AC_PIERCE):	temp = "piercing";	break;
	    case(AC_BASH):	temp = "bashing";	break;
	    case(AC_SLASH):	temp = "slashing";	break;
	    case(AC_EXOTIC):	temp = "magic";		break;
	    default:		temp = "error";		break;
	}
	
	send_to_char("You are ", ch);

	if (GET_AC(ch,i) <= 20)
	    sprintf(buf,"defenseless against %s.\n\r",temp);
	else if (GET_AC(ch,i) <= 40)
	    sprintf(buf,"barely protected from %s.\n\r",temp);
	else if (GET_AC(ch,i) <= 60)
	    sprintf(buf,"slightly armored against %s.\n\r",temp);
	else if (GET_AC(ch,i) <= 80)
	    sprintf(buf,"somewhat armored against %s.\n\r",temp);
	else if (GET_AC(ch,i) <= 100)
	    sprintf(buf,"armored against %s.\n\r",temp);
	else if (GET_AC(ch,i) <= 120)
	    sprintf(buf,"well-armored against %s.\n\r",temp);
	else if (GET_AC(ch,i) <= 140)
	    sprintf(buf,"very well-armored against %s.\n\r",temp);
	else if (GET_AC(ch,i) <= 160)
	    sprintf(buf,"heavily armored against %s.\n\r",temp);
	else if (GET_AC(ch,i) <= 180)
		sprintf(buf,"superbly armored against %s.\n\r",temp);
	else if (GET_AC(ch,i) <= 200)
		sprintf(buf,"impenetrably armored against %s.\n\r",temp);
	else if (GET_AC(ch,i) <= 220)
	    sprintf(buf,"nigh-invulnerable to %s.\n\r",temp);
    else
        sprintf(buf,"divinely armored against %s.\n\r",temp);

	send_to_char(buf,ch);
    }


    /* RT wizinvis and holy light */
    if ( IS_IMMORTAL(ch))
    {
      send_to_char("Holy Light: ",ch);
      if (IS_SET(ch->act,PLR_HOLYLIGHT))
        send_to_char("on",ch);
      else
        send_to_char("off",ch);

      if (ch->invis_level)
      {
        sprintf( buf, "  Invisible: level %d",ch->invis_level);
        send_to_char(buf,ch);
      }

      if (ch->incog_level)
      {
	sprintf(buf,"  Incognito: level %d",ch->incog_level);
	send_to_char(buf,ch);
      }
      send_to_char("\n\r",ch);
    }

    if ( ch->level >= 15 )
    {
	sprintf( buf, "Hitroll: %d  Damroll: %d.\n\r",
	    GET_HITROLL(ch), GET_DAMROLL(ch) );
	send_to_char( buf, ch );
    }
    send_to_char( "You are ", ch );
         if ( ch->alignment == 1000 ) send_to_char( "good,", ch );
    else if ( ch->alignment == 0 ) send_to_char( "neutral,", ch );
    else                             send_to_char( "evil,", ch );
    send_to_char( " and have a ", ch );
         if ( ch->pcdata->ethos == 1000 ) send_to_char( "lawful ethos.\n\r", ch );
    else if ( ch->pcdata->ethos == 0 ) send_to_char( "neutral ethos.\n\r", ch );
    else                               send_to_char( "chaotic ethos.\n\r", ch );

	if(ch->pcdata->trusting)
	{
		sprintf(buf,"You are trusting %s.\n\r", ch->pcdata->trusting->name);
		send_to_char(buf,ch);
	}
    if (ch->Class()->GetIndex() == CLASS_SORCERER) {
	sprintf(buf, "Your major elemental focus is %s and your para-elemental focus is %s.\n\r",sphere_table[ch->pcdata->ele_major].name,sphere_table[ch->pcdata->ele_para].name);
	send_to_char(buf,ch);
    }
    if (ch->Class()->GetIndex() == CLASS_ANTI_PALADIN)
	for(i=0; demon_table[i].name; i++)
	{
		if(demon_table[i].type == LESSER_DEMON && ch->pcdata->lesserdata[i] >= FAVOR_GRANTED)
			act("You have been granted the favor of the lesser demon $t.",
				ch,capitalize(demon_table[i].name),0,TO_CHAR);
		if(demon_table[i].type == GREATER_DEMON && i>=MAX_LESSER && 
		   ch->pcdata->greaterdata[i-MAX_LESSER] >= FAVOR_GRANTED)
			act("You have been granted the favor of the greater demon $t.",
				ch,capitalize(demon_table[i].name),0,TO_CHAR);
	}

    for(i=1; i<MAX_STYLE; i++)
          if(IS_SET(ch->pcdata->styles, style_table[i].bit))
          {
		sprintf(buf,"You are %s in the fighting style of the %s.\n\r",
		ch->pcdata->style && ch->pcdata->style == i ? "currently" : "versed", style_table[i].name);
		send_to_char(buf,ch);
          }
    if (IS_SET(ch->comm,COMM_SHOW_AFFECTS))
	do_affects(ch,"");
}

void do_affects(CHAR_DATA *ch, char *argument )
{
    AFFECT_DATA *paf, *paf_last = NULL;
    char buf[MAX_STRING_LENGTH], buf2[MSL], buf3[MSL];
	int showdur;
	int i;

    if ( ch->affected != NULL && (ch->affected->aftype!=AFT_INVIS || ch->affected->next!=NULL))
    {
	send_to_char( "You are affected by:\n\r", ch );
	for ( paf = ch->affected; paf != NULL; paf = paf->next )
	{

	    if(paf->aftype==AFT_INVIS)
		continue;

		for (i=0;i<MSL;i++) {
			buf[i] = '\0';
			buf2[i]= '\0';
		}
	    if (paf_last != NULL && (paf->type == paf_last->type && ((paf->name == NULL && paf_last->name == NULL) || !str_cmp(paf->name,paf_last->name))))
		{
		if (ch->level >= 20)
		    sprintf( buf, "                          ");
		else
		    continue;
	    } else {
		if (paf->aftype == AFT_SKILL)
	    	sprintf( buf, "Skill  : %-17s", paf->name ? paf->name : skill_table[paf->type].name );
		else if (paf->aftype == AFT_POWER)
	    	sprintf( buf, "Power  : %-17s", paf->name ? paf->name : skill_table[paf->type].name );
		else if (paf->aftype == AFT_MALADY)
	    	sprintf( buf, "Malady : %-17s", paf->name ? paf->name : skill_table[paf->type].name );
		else if (paf->aftype == AFT_COMMUNE)
	    	sprintf( buf, "Commune: %-17s", paf->name ? paf->name : skill_table[paf->type].name );
		else if (paf->aftype == AFT_RUNE)
			sprintf( buf, "Rune   : %-17s", paf->name ? paf->name : skill_table[paf->type].name );
		else if (paf->aftype == AFT_TIMER)
			sprintf( buf, "Timer  : %-17s", paf->name ? paf->name : skill_table[paf->type].name );
		else if (paf->aftype != AFT_INVIS)
			sprintf( buf, "Spell  : %-17s", paf->name ? paf->name : skill_table[paf->type].name );
		}

	    	send_to_char( buf, ch );

	    if ( ch->level >= 20 )
	    {
			showdur = paf->duration+1;
			sprintf( buf3, "%d", paf->modifier);

		sprintf( buf,
		    "| modifies %s%s%s ",
		(paf->mod_name > -1) ? mod_names[paf->mod_name].name : affect_loc_name(paf->location),
		(paf->mod_name > -1) ? "" : " by ",
		(paf->mod_name > -1) ? "" : buf3);
		if(paf->aftype!=AFT_INVIS)
			send_to_char( buf, ch );
		if ( paf->duration == -1 )
		    sprintf( buf, "permanently" );
		else {
			sprintf( buf2, " %d", (showdur/2));
		    sprintf( buf, "for%s%s%s hour%s.",
				(showdur == 1) ? "" : buf2,
				(showdur % 2 == 0) ? "" : (showdur == 1) ? "" :" and",
				(showdur % 2 == 0) ? "" : " a half",
				(showdur == 1 || showdur == 2) ? "" : "s");
		}
		if(paf->aftype!=AFT_INVIS)
			send_to_char( buf, ch );
	    }
	    if(paf->aftype!=AFT_INVIS);
	    	send_to_char( "\n\r", ch );
	    paf_last = paf;
	}
    }
    else
	send_to_char("You are not affected by anything.\n\r",ch);

    return;
}



void do_time( CHAR_DATA *ch, char *argument )
{
    extern char str_boot_time[];
    char buf[MAX_STRING_LENGTH];
    char *suf;
    int day;
    char *time;

    day     = time_info.day + 1;

         if ( day > 4 && day <  20 ) suf = "th";
    else if ( day % 10 ==  1       ) suf = "st";
    else if ( day % 10 ==  2       ) suf = "nd";
    else if ( day % 10 ==  3       ) suf = "rd";
    else                             suf = "th";

    sprintf( buf,
	"It is %s%d o'clock %s, Day %s, %d%s of the Month %s.\n\rIt is the Season %s in the Year %d.\n\r",
	(time_info.half) ? "half past " : "",
	(time_info.hour % 12 == 0) ? 12 : time_info.hour %12,
	time_info.hour >= 12 ? "pm" : "am",
	day_name[day % 6],
	day, suf,
	month_name[time_info.month],
	season_name[time_info.season],
	time_info.year);
    send_to_char(buf,ch);

	time = talloc_string(str_boot_time);
	chomp(time);
    sprintf(buf,"Riftshadow started up at %s.\n\r",time);
	send_to_char(buf,ch);

	time = talloc_string(ctime(&current_time));
	chomp(time);

	sprintf(buf,"The system time is %s.\n\r",time);
    send_to_char( buf, ch );

    return;
}



    static char * const sky_look[MAX_SKY] =
    {
	"The sky is cloudless",
	"The sky is partly cloudy",
	"The sky is overcast",
	"A light drizzle falls from the sky",
	"A drenching rain pours from the sky",
	"The sky is lit by flashes of lightning",
	"A light flurry of snowflakes falls from the sky",
	"A ferocious blizzard blots out the sky",
	"Pebble-sized hailstones fall from the sky"
    };

	static char * const temp_look[MAX_TEMP] =
	{
	"hot",
	"warm",
	"cool",
	"cold"
	};
	
	static char * const wind_look[MAX_WIND] =
	{
	"is perfectly still.",
	"wafts on a gentle breeze.",
	"swirls briskly.",
	"whips across the area with gale force!"
	};

void do_weather( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH], buf2[MSL];

    if ( !IS_OUTSIDE(ch) ) {
		sprintf(buf,"You can't see the sky from here, but the air around you is %s.\n\r", temp_look[ch->in_room->area->temp]);
		send_to_char(buf,ch);
		return;
    }

    sprintf( buf, "%s and the %s air %s\n\r",
		sky_look[ch->in_room->area->sky],
		temp_look[ch->in_room->area->temp],
		wind_look[ch->in_room->area->wind]);
	send_to_char(buf,ch);

	if (sun != SUN_LIGHT) {
		if (ch->in_room->area->sky == SKY_OVERCAST ||
			ch->in_room->area->sky >= SKY_DOWNPOUR) {
			send_to_char("You cannot make out the twin moons through the thick cloud cover.\n\r",ch);
			return;
		}
	
		sprintf( buf2, "Berus is %s and Calabren is %s.\n\r",
			moon_look[moon_berus],
			moon_look[moon_calabren]
			);
	    send_to_char( buf2, ch );
	}

    return;
}

void do_oldhelp( CHAR_DATA *ch, char *argument )
{
    HELP_DATA *pHelp;
    BUFFER *output;
    bool found = FALSE;
    char argall[MAX_INPUT_LENGTH],argone[MAX_INPUT_LENGTH];
    int level;

    output = new_buf();

    if ( argument[0] == '\0' )
	argument = "summary";

    /* this parts handles help a b so that it returns help 'a b' */
    argall[0] = '\0';
    while (argument[0] != '\0' )
    {
	argument = one_argument(argument,argone);
	if (argall[0] != '\0')
	    strcat(argall," ");
	strcat(argall,argone);
    }

    for ( pHelp = help_first; pHelp != NULL; pHelp = pHelp->next )
    {
    	level = (pHelp->level < 0) ? -1 * pHelp->level - 1 : pHelp->level;

	if (level > get_trust( ch ) )
	{
	if (pHelp->level < 61)
		continue;
    	}

	if ( is_name( argall, pHelp->keyword ) )
	{
	    /* add seperator if found */
	    if (found)
		add_buf(output,
    "\n\r============================================================\n\r\n\r");
	    if ( pHelp->level >= 0 && str_cmp( argall, "imotd" ) )
	    {
		add_buf(output,pHelp->keyword);
		add_buf(output,"\n\r");
	    }

	    /*
	     * Strip leading '.' to allow initial blanks.
	     */
	    if ( pHelp->text[0] == '.' )
		add_buf(output,pHelp->text+1);
	    else
		add_buf(output,pHelp->text);
	    found = TRUE;
	    /* small hack :) */
	    if (ch->desc != NULL && ch->desc->connected != CON_PLAYING
	    &&  		    ch->desc->connected != CON_GEN_GROUPS)
		break;
	}
    }

    if (!found)
    	send_to_char( "No help on that word.\n\r", ch );
    else
	page_to_char(buf_string(output),ch);
    free_buf(output);
}


/* whois command */
void do_whois (CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    BUFFER *output;
    char buf[MAX_STRING_LENGTH];
    char rbuf[MAX_STRING_LENGTH];
    char disp[MSL];
    DESCRIPTOR_DATA *d;
    bool found = FALSE;

    one_argument(argument,arg);

    if (arg[0] == '\0')
        return send_to_char("Whois who?\n\r",ch);

    output = new_buf();

    for (d = descriptor_list; d != NULL; d = d->next) {
		CHAR_DATA *wch;
		char const *class_name;
		char const *imm_lvl;

 		if (d->connected != CON_PLAYING || !can_see(ch,d->character))
		    continue;
	
		wch = ( d->original != NULL ) ? d->original : d->character;

 		if (!can_see(ch,wch))
	    	continue;

		if (is_affected(d->character, gsn_disguise))
			continue;

		if (str_cmp(wch->name, wch->true_name) ? IS_IMMORTAL(ch) ?
			!str_prefix(arg,wch->true_name) : 0 :
			!str_prefix(arg,wch->name)) {
	   		found = TRUE;
	
	    /* work out the printing */
	    class_name = wch->Class()->who_name;
	    switch(wch->level) {
			case MAX_LEVEL - 0 : class_name = "IMP"; 	break;
			case MAX_LEVEL - 1 : class_name = "CRE";	break;
			case MAX_LEVEL - 2 : class_name = "SUP";	break;
			case MAX_LEVEL - 3 : class_name = "DEI";	break;
			case MAX_LEVEL - 4 : class_name = "GOD";	break;
			case MAX_LEVEL - 5 : class_name = "IMM";	break;
			case MAX_LEVEL - 6 : class_name = "DEM";	break;
			case MAX_LEVEL - 7 : class_name = "ANG";	break;
			case MAX_LEVEL - 8 : class_name = "AVA";	break;
	    }

		switch(wch->level) {
			default			   : imm_lvl = "";			break;
			case MAX_LEVEL - 0 : imm_lvl = "Creator";	break;
			case MAX_LEVEL - 1 : imm_lvl = " Elder ";	break;
			case MAX_LEVEL - 2 : imm_lvl = " Elder ";	break;
			case MAX_LEVEL - 3 : imm_lvl = "Greater";	break;
			case MAX_LEVEL - 4 : imm_lvl = "Greater";	break;
			case MAX_LEVEL - 5 : imm_lvl = "Greater";	break;
			case MAX_LEVEL - 6 : imm_lvl = " Minor ";	break;
			case MAX_LEVEL - 7 : imm_lvl = " Minor ";	break;
			case MAX_LEVEL - 8 : imm_lvl = "Avatar ";	break;
	    }
			
		
	    /* a little formatting */
		if (IS_IMMORTAL(ch) || (IS_HEROIMM(wch) && wch->level<52)) {
			if((get_trust(ch) >= 52))
				strcpy(rbuf,"  ");
			else
				strcpy(rbuf,"");

			sprintf(buf, "[%2d %-5s %s%s] %s%s%s%s%s%s%s%s%s%s%s%s\n\r",
                wch->level,
				pc_race_table[wch->race].who_name_five,
                class_name,
				(get_trust(ch) >= 52) && wch->pcdata->history_buffer != NULL ?
					" *" : rbuf,
             	can_pk(ch, wch) ? "(PK) " : "",
	     		wch->incog_level >= LEVEL_HERO ? "(Incog) ": "",
 	     		wch->invis_level >= LEVEL_HERO ? "(Wizi) " : "",
             	cabal_table[wch->cabal].who_name,
				(get_trust(ch)>=57) && IS_SET(wch->act, PLR_MORON) ?
					"(MORON) " : "",
	     		(IS_SET(wch->comm, COMM_AFK) && (ch->level > 51)) ? "[AFK] " : "",
	     		IS_SET(wch->act,PLR_CRIMINAL) ? "(WANTED) " : "",
             	IS_SET(wch->act,PLR_THIEF) ? "(THIEF) " : "",
				isNewbie(wch) ? "(NEWBIE) " : "",
				wch->true_name,
 				IS_NPC(wch) ? "" : wch->pcdata->title,
				IS_NPC(wch) ? "" :
				(wch->pcdata->extitle) ? wch->pcdata->extitle : "");
	    		add_buf(output,buf);
		} else if (get_trust(wch)>=52 && !IS_IMMORTAL(ch)) {
			if (wch->pcdata->shifted > -1)
				continue;
                if(IS_IMMORTAL(wch)) {
                                sprintf(disp,"*   %s  *", imm_lvl);
                        } else {
                                sprintf(disp,"[%2d %-5s %s]",
                                                wch->level,
                                                pc_race_table[wch->race].who_name_five,
                                                class_name);
                        }

			sprintf(buf, "%s %s%s%s%s\n\r",
				disp,
				(wch->cabal==ch->cabal) ? cabal_table[wch->cabal].who_name :"",	
				wch->name,
				IS_NPC(wch) ? "" : wch->pcdata->title,
				IS_NPC(wch) ? "" :
				(wch->pcdata->extitle) ? wch->pcdata->extitle : "");
			add_buf(output,buf);
		} else {
			if(wch->pcdata->shifted>-1)
				continue;
			if(IS_IMMORTAL(wch)) {
				sprintf(disp,"*   %s  *", imm_lvl);
			} else {
				sprintf(disp,"[%2d %-5s %s]",
						wch->level,
						pc_race_table[wch->race].who_name_five,
						class_name);
			}
			sprintf(buf, "%s %s%s%s%s%s%s%s%s%s%s%s%s\n\r",
					disp,
					can_pk(ch, wch) ? "(PK) " : "",
					wch->incog_level >= LEVEL_HERO ? "(Incog) " : "",
					wch->invis_level >= LEVEL_HERO ? "(Wizi) " : "",
					(wch->cabal == ch->cabal || IS_IMMORTAL(wch)) ?
					cabal_table[wch->cabal].who_name : "", "",
					IS_SET(wch->comm, COMM_AFK) ? "[AFK] " : "",
					IS_SET(wch->act,PLR_CRIMINAL) ? "(WANTED) " : "",
					IS_SET(wch->act,PLR_THIEF) ? "(THIEF) " : "",
					isNewbie(wch) ? "(NEWBIE) " : "",
					wch->name,
					IS_NPC(wch) ? "" : wch->pcdata->title,
					IS_NPC(wch) ? "" :
						(wch->pcdata->extitle) ? wch->pcdata->extitle : "");
				add_buf(output,buf);
			}
		}
	}

	if (!found)
		return send_to_char("No one of that name is playing.\n\r",ch);

    page_to_char(buf_string(output),ch);
    free_buf(output);
}


/*
 * New 'who' command originally by Alander of Rivers of Mud.
 */
void do_who( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char rbuf[MAX_STRING_LENGTH];
    char disp[MSL];
    BUFFER *output;
    DESCRIPTOR_DATA *d;
    int iClass;
    int iRace;
    int iCabal;
    int iLevelLower;
    int iLevelUpper;
    int nNumber;
    int nMatch;
    int anc=0, tempnum;
    int fAlign = 0, fEthos = 0;
    bool rgfClass[MAX_CLASS];
    bool rgfRace[MAX_PC_RACE];
    bool rgfCabal[MAX_CABAL];
    bool fCriminal = FALSE;
    bool fClassRestrict = FALSE;
    bool fCabalRestrict = FALSE;
    bool fCabal = FALSE;
    bool fRaceRestrict = FALSE;
    bool fImmortalOnly = FALSE;
    bool fPkOnly = FALSE;
    bool fBuilder = FALSE;
    bool fNewbie = FALSE;
    bool fNoDesc = FALSE;
    bool fRole = FALSE;
	/*
     * Set default arguments.
     */
    iLevelLower    = 0;
    iLevelUpper    = MAX_LEVEL;
    for ( iClass = 0; iClass < MAX_CLASS; iClass++ )
        rgfClass[iClass] = FALSE;
    for ( iRace = 0; iRace < MAX_PC_RACE; iRace++ )
        rgfRace[iRace] = FALSE;
    for (iCabal = 0; iCabal < MAX_CABAL; iCabal++)
        rgfCabal[iCabal] = FALSE;

    /*
     * Parse arguments.
     */
    nNumber = 0;
    for ( ;; )
    {
        char arg[MAX_STRING_LENGTH], arg2[500];

        argument = one_argument( argument, arg );
        if ( arg[0] == '\0' )
            break;

        if ( is_number( arg ) && IS_IMMORTAL(ch)) {
			if((tempnum=atoi(arg))<1 || tempnum>MAX_LEVEL)
				return send_to_char("Invalid who level range.\n\r",ch);
			iLevelLower = tempnum;
			one_argument(argument,arg2);
			if(arg2[0]!='\0' && is_number(arg2)) {
				tempnum = atoi(arg2);
				if(tempnum<=iLevelLower || tempnum>MAX_LEVEL)
					return send_to_char("Invalid who level range.\n\r",ch);
				iLevelUpper = tempnum;
			}
        } else {
            /*
             * Look for classes to turn on.
             */
            if (!str_prefix(arg,"immortal"))
                fImmortalOnly = TRUE;
            else if (!str_prefix(arg,"pk"))
                fPkOnly = TRUE;
	    	else if (!str_prefix(arg,"wanted") || !str_prefix(arg,"criminal"))
				fCriminal = TRUE;
			else if (!str_prefix(arg,"builder") && IS_IMMORTAL(ch))
				fBuilder = TRUE;
			else if (!str_prefix(arg,"newbie"))
				fNewbie = TRUE;
		else if (!str_prefix(arg,"good") && IS_IMMORTAL(ch))
			fAlign = 3;
		else if (!str_prefix(arg,"neutral") && IS_IMMORTAL(ch))
			fAlign = 2;
		else if (!str_prefix(arg,"evil") && IS_IMMORTAL(ch))
			fAlign = 1;
		else if (!str_prefix(arg,"lawful") && IS_IMMORTAL(ch))
			fEthos = 3;
		else if (!str_prefix(arg,"eneutral") && IS_IMMORTAL(ch))
			fEthos = 2;
		else if (!str_prefix(arg,"chaotic") && IS_IMMORTAL(ch))
			fEthos = 1;
		else if (!str_prefix(arg,"nodesc") && IS_IMMORTAL(ch))
			fNoDesc = TRUE;
		else if (!str_prefix(arg,"role") && IS_IMMORTAL(ch))
			fRole = TRUE;
            else {
                iClass = CClass::Lookup(arg);
                if (iClass == -1) {
                    iRace = race_lookup(arg);
                    if (iRace == 0 || iRace > MAX_PC_RACE) {
                        iCabal = cabal_lookup(arg);
                        if (iCabal) {
                            if (iCabal == ch->cabal || IS_IMMORTAL(ch) ) {
                                fCabalRestrict = TRUE;
                                rgfCabal[iCabal] = TRUE;
                            } else {
                                send_to_char(
                                "You're not a member of that cabal.\n\r",
                                   ch);
                                return;
                            }
                        } else {
                            send_to_char(
                            "That's not a valid race or cabal.\n\r",
                               ch);
                            return;
                        }
					} else {
                        fRaceRestrict = TRUE;
                        rgfRace[iRace] = TRUE;
                    }
                } else {
		
					if (IS_IMMORTAL(ch)) {
						fClassRestrict = TRUE;
						rgfClass[iClass] = TRUE;
					} else {
                            send_to_char(
                            "That's not a valid race or cabal.\n\r",
                               ch);
                            return;
					}
                }
            }
        }
    }

    /*
     * Now show matching chars.
     */
    nMatch = 0;
    buf[0] = '\0';
    output = new_buf();
    for ( d = descriptor_list; d != NULL; d = d->next )
    {
        CHAR_DATA *wch;
        char const *class_name;
		char const *imm_lvl;
		bool fOChar = FALSE;




		fOChar = FALSE;
        /*
         * Check for match against restrictions.
         * Don't use trust as that exposes trusted mortals.
         */
        if ( d->connected != CON_PLAYING || !can_see( ch, d->character ) )
            continue;

        	wch   = ( d->original != NULL ) ? d->original : d->character;

		if (!can_see(ch,wch))
			continue;

		if (wch->pcdata->old && !IS_IMMORTAL(ch))
			fOChar = TRUE;

		if(!IS_IMMORTAL(ch) && is_affected(wch,gsn_cloak_form) && ch!=wch
   	        	&& ch->pcdata->induct == 5)
		{
		    if(can_pk(ch,wch))
			anc=1;
		    continue;
		}

		if ( wch->level < iLevelLower
        		||   wch->level > iLevelUpper
        		|| ( fImmortalOnly  && (wch->level < LEVEL_IMMORTAL && !(IS_HEROIMM(wch)) ))
        		|| ( fPkOnly && !can_pk(ch, wch) )
        		|| ( fClassRestrict && !rgfClass[wch->Class()->GetIndex()] )
        		|| ( fRaceRestrict && !rgfRace[wch->race])
			|| ( fCriminal && !IS_SET(wch->act,PLR_CRIMINAL))
			|| ( fBuilder && !IS_SET(wch->comm,COMM_BUILDER))
			|| ( fNewbie && wch->level > 10)
        		|| ( fCabal && !is_cabal(wch) && !IS_IMMORTAL(ch))
        		|| ( fCabalRestrict && !rgfCabal[wch->cabal])
			|| (fAlign == 3 && !IS_GOOD(wch))
			|| (fAlign == 2 && !IS_NEUTRAL(wch))
			|| (fAlign == 1 && !IS_EVIL(wch))
			|| (fEthos == 3 && !IS_LAWFUL(wch))
			|| (fEthos == 2 && !IS_ENEUTRAL(wch))
			|| (fEthos == 1 && !IS_CHAOTIC(wch))
			|| (fRole && !wch->pcdata->role)
			|| (fNoDesc && wch->description != NULL && strlen(wch->description) > 144)
			|| fOChar)
            		continue;

        nMatch++;

	/*
	 * If the character is disguised, do not show him on who, except to immortals
	 */
	if (is_affected(wch, gsn_disguise) && !IS_IMMORTAL(ch))
		continue;

    /*
     * Figure out what to print for class.
	 */
		class_name = wch->Class()->who_name;
		switch ( wch->level ) {
			default: 								break;
            case MAX_LEVEL - 0 : class_name = "IMP";     break;
            case MAX_LEVEL - 1 : class_name = "CRE";     break;
            case MAX_LEVEL - 2 : class_name = "SUP";     break;
            case MAX_LEVEL - 3 : class_name = "DEI";     break;
            case MAX_LEVEL - 4 : class_name = "GOD";     break;
            case MAX_LEVEL - 5 : class_name = "IMM";     break;
            case MAX_LEVEL - 6 : class_name = "DEM";     break;
            case MAX_LEVEL - 7 : class_name = "ANG";     break;
            case MAX_LEVEL - 8 : class_name = "AVA";     break;
		}

		switch(wch->level) {
			default			   : imm_lvl = "";			break;
			case MAX_LEVEL - 0 : imm_lvl = "Creator";	break;
			case MAX_LEVEL - 1 : imm_lvl = " Elder ";	break;
			case MAX_LEVEL - 2 : imm_lvl = " Elder ";	break;
			case MAX_LEVEL - 3 : imm_lvl = "Greater";	break;
			case MAX_LEVEL - 4 : imm_lvl = "Greater";	break;
			case MAX_LEVEL - 5 : imm_lvl = "Greater";	break;
			case MAX_LEVEL - 6 : imm_lvl = " Minor ";	break;
			case MAX_LEVEL - 7 : imm_lvl = " Minor ";	break;
			case MAX_LEVEL - 8 : imm_lvl = "Avatar ";	break;
	    }
	/*
	 * Format it up.
	 */
	if(IS_IMMORTAL(ch) || (wch->level < 52 && IS_HEROIMM(wch)))
	{
		if((get_trust(ch) >= 52))
			strcpy(rbuf,"  ");
		else
			strcpy(rbuf,"");

		sprintf( buf, "[%2d %-5s %s%s] %s%s%s%s%s%s%s%s%s%s%s%s%s\n\r",
    				wch->level,
	   				pc_race_table[wch->race].who_name_five,
	    			class_name,
					(get_trust(ch) >= 52) && wch->pcdata->history_buffer != NULL ? " *" : rbuf,
            		can_pk(ch, wch) ? "(PK) " : "",
	    			wch->incog_level >= LEVEL_HERO ? "(Incog) " : "",
	    			wch->invis_level >= LEVEL_HERO ? "(Wizi) " : "",
            		cabal_table[wch->cabal].who_name,
					(get_trust(ch)>=57) && IS_SET(wch->act, PLR_MORON) ? "(MORON) " : "",
	    			(IS_SET(wch->comm, COMM_AFK) && (ch->level > 51)) ? "[AFK] " : "",
            		IS_SET(wch->act, PLR_CRIMINAL) ? "(WANTED) " : "",
            		IS_SET(wch->act, PLR_THIEF)  ? "(THIEF) "  : "",
					isNewbie(wch) ? "(NEWBIE) " : "",
					is_affected(wch, gsn_disguise) ? "(Disguised) " : "",
	    			wch->true_name,
	    			IS_NPC(wch) ? "" : wch->pcdata->title ,
					IS_NPC(wch) ? "" :
					(wch->pcdata->extitle) ? wch->pcdata->extitle : "");
		add_buf(output,buf);
	} 
	else if (get_trust(wch) >= 52 && !IS_IMMORTAL(ch))
	{
		if (wch->pcdata->shifted > -1)
			continue;
                if(IS_IMMORTAL(wch)) {
                                sprintf(disp,"*   %s  *", imm_lvl);
                        } else {
                                sprintf(disp,"[%2d %-5s %s]",
                                                wch->level,
                                                pc_race_table[wch->race].who_name_five,
                                                class_name);
                        }

		sprintf(buf, "%s %s%s%s%s\n\r",
				disp,	
				(wch->cabal==ch->cabal) ? cabal_table[wch->cabal].who_name :"",	
				wch->true_name,
				IS_NPC(wch) ? "" : wch->pcdata->title,
				IS_NPC(wch) ? "" :
				(wch->pcdata->extitle) ? wch->pcdata->extitle : "");
		add_buf(output,buf);
 	} 
	else 
	{
		if(wch->pcdata->shifted>-1)
			continue;
		if(IS_IMMORTAL(wch)) {
                                sprintf(disp,"*   %s  *", imm_lvl);
                        } else {
                                sprintf(disp,"[%2d %-5s %s]",
                                                wch->level,
                                                pc_race_table[wch->race].who_name_five,
                                                class_name);
                        }

		sprintf( buf, "%s %s%s%s%s%s%s%s%s%s%s%s\n\r",
				disp,
            		can_pk(ch, wch) ? "(PK) " : "",
	    			wch->incog_level >= LEVEL_HERO ? "(Incog) " : "",
	    			wch->invis_level >= LEVEL_HERO ? "(Wizi) " : "",
            		(wch->cabal == ch->cabal || IS_IMMORTAL(wch)) ?
            		cabal_table[wch->cabal].who_name : "",
	    			IS_SET(wch->comm, COMM_AFK) ? "[AFK] " : "",
            		IS_SET(wch->act, PLR_CRIMINAL) ? "(WANTED) " : "",
            		IS_SET(wch->act, PLR_THIEF)  ? "(THIEF) "  : "",
					isNewbie(wch) ? "(NEWBIE) " : "",
					wch->true_name,
	    			IS_NPC(wch) ? "" : wch->pcdata->title,
					IS_NPC(wch) ? "" :
					(wch->pcdata->extitle) ? wch->pcdata->extitle : "" );
		add_buf(output,buf);
    }
    }

    sprintf( buf2, "\n\rPlayers found: %d\n\r", nMatch );
    add_buf(output,buf2);
    page_to_char( buf_string(output), ch );
    free_buf(output);
    return;
}

void do_count ( CHAR_DATA *ch, char *argument )
{
    int count;
    DESCRIPTOR_DATA *d;
    int not_seen;
    char buf[MAX_STRING_LENGTH];

    not_seen = 0;
    count = 0;

    for ( d = descriptor_list; d != NULL; d = d->next )
        if ( d->connected == CON_PLAYING)
	{
	 if ( can_see( ch, d->character ) && !IS_SWITCHED(d->character))
	    count++;
	 else
	    not_seen++;
    	}

    if ( (count + not_seen) >= count_data[0])
	count_data[0] = (count + not_seen);

        sprintf(buf,"There are %d characters on that you can see.\n\r",
	    count);
    send_to_char(buf,ch);

    return;

}

void do_inventory( CHAR_DATA *ch, char *argument )
{
    send_to_char( "You are carrying:\n\r", ch );
    show_list_to_char( ch->carrying, ch, TRUE, TRUE );
    return;
}



void do_equipment( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    int iWear;
    bool found;

    send_to_char( "You are using:\n\r", ch );
    found = FALSE;
    for ( iWear = 0; iWear < MAX_WEAR; iWear++ )
    {
		if(iWear == WEAR_WIELD) //cosmetics come right before "wield" on display
		{
			OBJ_DATA *tObj;
			for(tObj = ch->carrying; tObj; tObj = tObj->next_content)
			if(tObj->wear_loc == WEAR_COSMETIC)
			{
				char buf[MSL], buf2[MSL];
				sprintf(buf2,"%s>",tObj->wear_loc_name ? tObj->wear_loc_name : "bug: unset wear_loc on cosmetic");
				sprintf(buf, "<worn %-14s",buf2);
				send_to_char(buf,ch);
				if(can_see_obj(ch,tObj))
					send_to_char( format_obj_to_char(tObj, ch, TRUE ), ch );
				else
					send_to_char("something.",ch);
				send_to_char("\n\r",ch);
				found = TRUE;
			}
		}
		if ( iWear == WEAR_COSMETIC || ( obj = get_eq_char( ch, iWear ) ) == NULL)
	    	continue;
	
		send_to_char( where_name[iWear], ch );
	
		if ( can_see_obj( ch, obj ) )
		    send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
		else
		    send_to_char( "something.", ch );
		send_to_char("\n\r",ch);
		found = TRUE;
    }
    if ( !found )
		send_to_char( "Nothing.\n\r", ch );

    return;
}



void do_compare( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj1;
    OBJ_DATA *obj2;
    int value1;
    int value2;
    char *msg;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' )
		return send_to_char( "Compare what to what?\n\r", ch );

    if ( ( obj1 = get_obj_carry( ch, arg1, ch ) ) == NULL )
		return send_to_char( "You do not have that item.\n\r", ch );

    if (arg2[0] == '\0') {
		for (obj2 = ch->carrying; obj2 != NULL; obj2 = obj2->next_content) {
			if (obj2->wear_loc != WEAR_NONE
	   	 		&&  can_see_obj(ch,obj2)
	   	 		&&  obj1->item_type == obj2->item_type
	   			&&  (obj1->wear_flags[0] == obj2->wear_flags[0])
				&& IS_SET(obj1->wear_flags, ITEM_TAKE)
				&& IS_SET(obj2->wear_flags,ITEM_TAKE))
				break;
		}

		if (obj2 == NULL)
		    return send_to_char("You aren't wearing anything comparable.\n\r",ch);
	}

    else if ( (obj2 = get_obj_carry(ch,arg2,ch) ) == NULL )
		return send_to_char("You do not have that item.\n\r",ch);

    msg		= NULL;
    value1	= 0;
    value2	= 0;

    if ( obj1 == obj2 )
		msg = "You compare $p to itself.  It looks about the same.";
    else if ( obj1->item_type != obj2->item_type )
		msg = "You can't compare $p and $P.";
	else
		switch ( obj1->item_type ) {
			default:
		    	msg = "You can't compare $p and $P.";
				break;
			case ITEM_ARMOR:
			    value1 = obj1->value[0] + obj1->value[1] + obj1->value[2];
			    value2 = obj2->value[0] + obj2->value[1] + obj2->value[2];
			    break;	
			case ITEM_WEAPON:
				value1 = (1 + obj1->value[2]) * obj1->value[1];
				value2 = (1 + obj2->value[2]) * obj2->value[1];
			    break;
		}

    if ( msg == NULL ) {
		if ( value1 == value2 ) 	 msg = "$p and $P look about the same.";
		else if ( value1  > value2 ) msg = "$p looks better than $P.";
		else                         msg = "$p looks worse than $P.";
    }

    act( msg, ch, obj1, obj2, TO_CHAR );
    return;
}





void do_where( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    DESCRIPTOR_DATA *d;
    bool found;

    one_argument( argument, arg );

	if(ch == NULL)
		return;

    if (IS_AFFECTED(ch,AFF_BLIND) || (get_bv_stage(ch) > -1))
		return send_to_char("You can't see anything!\n\r",ch);

	if (is_affected_area(ch->in_room->area, gsn_cyclone))
		return send_to_char("The swirling winds and debris prevent you from seeing anything!\n\r",ch);

	if (is_affected_room(ch->in_room, gsn_smokescreen))
		return send_to_char("Smoke fills the room and prevents you from looking around!\n\r", ch);
	
    if ( arg[0] == '\0' )
    {
	send_to_char( "Players near you:\n\r", ch );
	found = FALSE;
	for ( d = descriptor_list; d; d = d->next )
	{
	    if ( d->connected == CON_PLAYING
	    && ( victim = d->character ) != NULL
	    &&   !IS_NPC(victim)
	    &&   victim->in_room != NULL
	    &&   !IS_SET(victim->in_room->room_flags,ROOM_NOWHERE)
	    &&   (!IS_SET(victim->in_room->room_flags,ROOM_IMP_ONLY) || ch->level==60)
 	    &&   (is_room_owner(ch,victim->in_room)
	    ||    !room_is_private(victim->in_room))
	    &&   (victim->in_room->area == ch->in_room->area
		||   (is_adjacent_area(victim->in_room->area,ch->in_room->area)
		&&   (IS_IMMORTAL(ch) || ch->pcdata->greaterdata[GREATER_GERYON] == GERYON_FINGER)) )
	    &&   can_see( ch, victim )
		&&	 !is_affected_room(victim->in_room, gsn_smokescreen)
		&& 	!is_affected(victim, gsn_disguise))
	    {
		found = TRUE;

		sprintf( buf, "%s%-28s %s%s%s\n\r",
		    can_pk(ch,victim) ? "(PK) " : "     ",
			(!IS_NPC(ch) && ch == victim && is_affected(ch, gsn_shroud_of_secrecy)) ? ch->true_name : PERS(victim,ch),
			IS_IMMORTAL(ch) ? victim->in_room->area->name : "",
			IS_IMMORTAL(ch) ? ": " : "",get_room_name(victim->in_room) );
		send_to_char( buf, ch );
	    }
	}

	if ( !found )
	    send_to_char( "None.\n\r", ch );
    }
    else if (!str_prefix(arg,"pk"))
    {
	found = FALSE;
	for ( d = descriptor_list; d; d = d->next )
	{

	    if ( d->connected == CON_PLAYING
	    && ( victim = d->character ) != NULL
	    &&   !IS_NPC(victim)
	    &&   victim->in_room != NULL
	    &&   !IS_SET(victim->in_room->room_flags,ROOM_NOWHERE)
	    &&   (!IS_SET(victim->in_room->room_flags,ROOM_IMP_ONLY) || ch->level==60)
 	    &&   (is_room_owner(ch,victim->in_room)
	    ||    !room_is_private(victim->in_room))
	    &&   (victim->in_room->area == ch->in_room->area
		||   (is_adjacent_area(victim->in_room->area,ch->in_room->area)
		&&   IS_IMMORTAL(ch)) )
	    &&   can_pk(ch,victim)
	    &&   can_see( ch, victim )
		&&   !is_affected_room(victim->in_room, gsn_smokescreen))
	    {
		found = TRUE;

		sprintf( buf, "%s%-28s %s%s%s\n\r",
		    can_pk(ch,victim) ? "(PK) " : "     ",
			(!IS_NPC(ch) && ch == victim && is_affected(ch, gsn_shroud_of_secrecy)) ? ch->true_name : PERS(victim,ch),
			IS_IMMORTAL(ch) ? victim->in_room->area->name : "",
			IS_IMMORTAL(ch) ? ": " : "",get_room_name(victim->in_room));
		send_to_char( buf, ch );
	    }
	}
	if ( !found )
	    act( "You didn't find anyone in PK.", ch, NULL, arg, TO_CHAR );
    }
    else
    {
	found = FALSE;
	for ( victim = char_list; victim != NULL; victim = victim->next )
	{
	    if ( victim->in_room != NULL
	    &&   (victim->in_room->area == ch->in_room->area
		||   (is_adjacent_area(victim->in_room->area,ch->in_room->area)
		&&   IS_IMMORTAL(ch)) )
	    &&   !IS_AFFECTED(victim, AFF_SNEAK)
	    &&   (!IS_SET(victim->in_room->room_flags,ROOM_IMP_ONLY) || ch->level==60)
	    &&   can_see( ch, victim )
	    &&   is_name( arg, victim->name )
		&&   !is_affected_room(victim->in_room, gsn_smokescreen))
	    {
		found = TRUE;
		sprintf( buf, "%s%-28s %s%s%s\n\r",
		    can_pk(ch,victim) ? "(PK) " : "     ",
			PERS(victim,ch),IS_IMMORTAL(ch) ? victim->in_room->area->name : "",
			IS_IMMORTAL(ch) ? ": " : "",get_room_name(victim->in_room));
		send_to_char( buf, ch );
		break;
	    }
	}
	if ( !found )
	    act( "You didn't find any $T.", ch, NULL, arg, TO_CHAR );
    }

    return;
}




void do_consider( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int diff;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
	return send_to_char( "Consider killing whom?\n\r", ch );

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
	return send_to_char( "They're not here.\n\r", ch );

    diff = victim->level - ch->level;

	if ( diff <= -10 )
		act("Your sneeze would kill $N.",ch,0,victim,TO_CHAR);
    else if ( diff <=  -5 )
		act("$N wouldn't last long against you.",ch,0,victim,TO_CHAR);
    else if ( diff <=  -2 )
		act("$N looks like an easy kill.",ch,0,victim,TO_CHAR);
    else if ( diff <=   1 )
		act("$N is about a perfect match for you!",ch,0,victim,TO_CHAR);
    else if ( diff <=   4 )
		act("$N looks a little tough.",ch,0,victim,TO_CHAR);
    else if ( diff <=   9 )
		act("You wouldn't last too long against $N by yourself.",ch,0,victim,TO_CHAR);
    else
		act("You must have a fascination with death.",ch,0,victim,TO_CHAR);
    
    diff = victim->size - ch->size;
	if(diff > 1)
		act("$N easily towers over you.",ch,0,victim,TO_CHAR);
	else if(diff == 1)
		act("$N is considerably larger than you.",ch,0,victim,TO_CHAR);
	else if (!diff)
		act("$N is about the same size as you.",ch,0,victim,TO_CHAR);
	else if (diff == -1)
		act("$N is considerably smaller than you.",ch,0,victim,TO_CHAR);
	else if (diff < -1)
		act("You tower over $N.  Be careful not to step on $M.",ch,0,victim,TO_CHAR);

    if(victim->alignment >= 1)
	act("$N seems relatively benign.",ch,0,victim,TO_CHAR);
    return;
}



void set_title( CHAR_DATA *ch, char *title )
{
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) )
		return bug( "Set_title: NPC.", 0 );

    if ( title[0] != '.' && title[0] != ',' && title[0] != '!' && title[0] != '?' )
    {
		buf[0] = ' ';
		strcpy( buf+1, title );
    }
    else
    {
		strcpy( buf, title );
    }

    free_pstring( ch->pcdata->title );
    ch->pcdata->title = palloc_string( buf );
    return;
}



void do_title( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg1[MAX_INPUT_LENGTH];
    char *pArg;
    char cEnd;
	char buf[MSL];
    FILE *fp;
    char *strtime;

    if ( IS_NPC(ch) || ch->level < 52 )
	return;

    /*
     * Can't use one_argument here because it smashes case.
     * So we just steal all its code.  Bleagh.
     */
    pArg = arg1;
    while ( isspace(*argument) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
	*pArg++ = *argument++;
    }
    *pArg = '\0';

    if ( arg1[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "Change who's title to what?\n\r", ch );
	return;
    }

    if ( (victim = get_char_world(ch, arg1)) == NULL)
    {
    	send_to_char( "Player isn't on.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Trying to change a mob's title?\n\r", ch );
	return;
    }

    if ( ch != victim && get_trust(victim) >= get_trust(ch) )
    {
  	send_to_char( "You can't do that to that person.\n\r", ch );
	return;
    }

    if ( strlen(argument) > 45 )
	argument[45] = '\0';

    	strtime                    = ctime( &current_time );
    	strtime[strlen(strtime)-1] = '\0';	
   if ( ( fp = fopen( GOD_LOG_FILE, "a" ) ) != NULL )
	{
	fprintf(fp,"%s: TITLE- %s is titling %s.\n",strtime,ch->name,argument);
    fclose(fp);
	}
	sprintf(buf,"AUTO: Titled to (%s) by %s.\n\r",argument, ch->true_name);
	add_history(NULL,victim,buf);
    smash_tilde( argument );
    set_title( victim, argument );
    send_to_char( "Ok.\n\r", ch );
}

void set_extitle(CHAR_DATA *ch, char *title) {
    char buf[MAX_STRING_LENGTH];

    if(IS_NPC(ch)) {
        bug("Set_extitle: NPC.", 0);
        return;
    }

    if(title[0]!='.'&&title[0]!=','&&title[0]!='!'&&title[0]!='?') {
        buf[0] = ' ';
        strcpy(buf+1, title);
    } else {
        strcpy(buf, title);
    }

    free_pstring(ch->pcdata->extitle);
    ch->pcdata->extitle = palloc_string(buf);
    return;
}

void do_extitle( CHAR_DATA *ch, char *argument ) {
    CHAR_DATA *victim;
    char arg1[MAX_INPUT_LENGTH];
    char *pArg;
    char cEnd;
	char buf[MSL];
    FILE *fp;
    char *strtime;

    if (IS_NPC(ch))
        return;

    /*
     * Can't use one_argument here because it smashes case.
     * So we just steal all its code.  Bleagh.
     */
    pArg = arg1;
    while ( isspace(*argument) )
        argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
        cEnd = *argument++;

    while ( *argument != '\0' )
    {
        if ( *argument == cEnd )
        {
            argument++;
            break;
        }
        *pArg++ = *argument++;
    }
    *pArg = '\0';

    if ( arg1[0] == '\0')
    {
        send_to_char( "Change who's extra title to what?\n\r", ch );
        return;
    }

    if ( (victim = get_char_world(ch, arg1)) == NULL)
    {
        send_to_char( "That player isn't on.\n\r", ch );
        return;
    }

    if ( IS_NPC(victim) )
    {
        send_to_char( "Trying to change a mob's extra title?\n\r", ch );
        return;
    }

    if ( ch != victim && get_trust(victim) >= get_trust(ch) )
    {
        send_to_char( "You can't do that to that person.\n\r", ch );
        return;
    }

    if(argument[0]=='\0') {
	free_pstring(ch->pcdata->extitle);
	ch->pcdata->extitle = NULL;
	send_to_char("Extitle cleared.\n\r",ch);
	return;
    }	

    if ( strlen(argument) > 45 )
        argument[45] = '\0';
    	strtime                    = ctime( &current_time );
    	strtime[strlen(strtime)-1] = '\0';	
   if ( ( fp = fopen( GOD_LOG_FILE, "a" ) ) != NULL )
   {
	fprintf(fp,"%s: TITLE- %s is titling %s.\n",strtime,ch->name,argument);
    fclose(fp);
   }
	sprintf(buf,"AUTO: Extitled to (%s) by %s.\n\r", argument, ch->true_name);
        add_history(NULL,victim,buf);
    smash_tilde( argument );
    set_extitle( victim, argument );
    send_to_char( "Ok.\n\r", ch );
}

void descr_end_fun(CHAR_DATA *ch, char *argument)
{
	free_pstring(ch->description);
	ch->description = palloc_string(argument);
	send_to_char("Description modified.\n\r",ch);
	ch->pcdata->entered_text[0] = '\0';
}

void do_description( CHAR_DATA *ch, char *argument )
{
	char arg1[MSL],buf[MSL];
	BUFFER *buffer;
	
    	if (IS_NPC(ch))
		return;

	argument = one_argument(argument, arg1);
	
	if(!str_cmp(arg1,"clear"))
	{
		if(ch->description)
			free_pstring( ch->description );
		ch->description = NULL;
		send_to_char("Description cleared.\n\r", ch);
	}
	else if(!str_cmp(arg1,"+"))
	{
		smash_tilde(argument);
		buffer = new_buf();
		if(ch->description)
			add_buf(buffer, ch->description);
		add_buf(buffer, argument);
		add_buf(buffer, "\n\r");
		if(ch->description)
			free_pstring( ch->description );
		ch->description = palloc_string( buf_string(buffer) );
		free_buf(buffer);
		send_to_char("Line added.\n\r", ch);
		return;
	}
	else if(!str_cmp(arg1,"-"))
	{
		int len;
		bool found = FALSE;

		if(ch->description == NULL || ch->description[0] == '\0')
			return send_to_char("No lines left to remove.\n\r", ch);

		strcpy(buf, ch->description);

		for (len = strlen(buf); len > 0; len--)                                                                       
		{                                                                                                             
			if (buf[len] == '\r')                                                                                     
			{                  
				if (!found)  /* back it up */                                                                             			    {          
					if (len > 0)
						len--;                                                                                           			    found = TRUE;
				}                                
				else /* found the second one */
				{                       
					buf[len + 1] = '\0';
					free_pstring(ch->description);   
					ch->description = palloc_string(buf);     
					return;
				}                                                                             
			}                                                                                                         
		}                                                                                                             
		buf[0] = '\0';                                                                                                
		free_pstring(ch->description);                                                                                 
		ch->description = palloc_string(buf);                                                                               
		send_to_char("Ok\n\r", ch);
		return;                            
	}
	else if(!str_cmp(arg1,"format"))
	{
		if(!ch->description)
			return send_to_char("You have no description to format!\n\r",ch);

		ch->description = format_string(ch->description);
		return send_to_char("Description formatted.\n\r",ch);
	}
        send_to_char( "Your description is:\n\r", ch );
        send_to_char( ch->description ? ch->description : "(None)\n\r", ch );
        return;
}



void do_report( CHAR_DATA *ch, char *argument )
{
    char buf[MSL], buf2[MSL], buf3[MSL], buf4[MSL];
    int percenta, percentb, percentc;

    if (ch->max_hit == 0)
	percenta = 0;
    else
	percenta = (ch->hit * 100) / ch->max_hit;
    if (ch->max_mana == 0)
	percentb = 0;
    else
        percentb = (ch->mana * 100) / ch->max_mana;
    if (ch->max_move == 0)
 	percentc = 0;
    else
        percentc = (ch->move * 100) / ch->max_move;

	if (percenta >= 100)
		sprintf(buf,"in perfect health");
	else if (percenta < 100 && percenta >= 80)
		sprintf(buf,"slightly injured");
	else if (percenta < 80 && percenta >= 60)
		sprintf(buf,"somewhat wounded");
	else if (percenta < 60 && percenta >= 40)
		sprintf(buf,"seriously hurt");
	else if (percenta < 40 && percenta >= 20)
		sprintf(buf,"grievously wounded");
	else if (percenta < 20 && percenta > 0)
		sprintf(buf,"hanging on by a thread");
	else
		sprintf(buf,"dead on my feet");

	if (percentb >= 100)
		sprintf(buf2,"full");
	else if (percentb < 100 && percentb >= 80)
		sprintf(buf2,"strong");
	else if (percentb < 80 && percentb >= 60)
		sprintf(buf2,"steady");
	else if (percentb < 60 && percentb >= 40)
		sprintf(buf2,"wavering");
	else if (percentb < 40 && percentb >= 20)
		sprintf(buf2,"weak");
	else if (percentb < 20 && percentb > 0)
		sprintf(buf2,"barely any");
	else
		sprintf(buf2,"no remaining");

	if (percentc >= 100)
		sprintf(buf3,"maximum");
	else if (percentc < 100 && percentc >= 80)
		sprintf(buf3,"great");
	else if (percentc < 80 && percentc >= 60)
		sprintf(buf3,"strong");
	else if (percentc < 60 && percentc >= 40)
		sprintf(buf3,"moderate");
	else if (percentc < 40 && percentc >= 20)
		sprintf(buf3,"fading");
	else if (percentc < 20 && percentc > 0)
		sprintf(buf3,"crumbling");
	else
		sprintf(buf3,"no remaining");
		
    sprintf( buf4, "I am %s, with %s mental focus, and %s endurance.",
		buf,buf2,buf3);
	do_say(ch,buf4);

    return;
}



void do_practice( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int sn,p,gn,style_skill;
    bool hide_skill;
    int mcap=0;
	int base;

    if ( IS_NPC(ch) )
	return;

    if ( argument[0] == '\0')
    {
		/* Hi, you may have seen me in such films as Cheap Hack 1 and Cheap Hack
		* 2: Electric boogaloo -cal */
	int col;
        if(ch->Class()->GetIndex() == CLASS_NECROMANCER) {
        if(ch->pcdata->learned[skill_lookup("ritual of flesh")] > 0 && ch->pcdata->souls < 30)
            ch->pcdata->learned[skill_lookup("ritual of flesh")] = -2;  
        if(ch->pcdata->learned[skill_lookup("ritual of soul")] > 0 && ch->pcdata->souls < 55)
            ch->pcdata->learned[skill_lookup("ritual of soul")] = -2;   
        if(ch->pcdata->learned[skill_lookup("visceral divination")] && ch->pcdata->souls < 70)
            ch->pcdata->learned[skill_lookup("visceral divination")] = -2;
        }

	col    = 0;
	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
		hide_skill=FALSE;
		if ( skill_table[sn].name == NULL )
		break;

		for(p=0; p<MAX_STYLE_SKILL; p++)
		{
			if(!str_cmp(style_percent[p].name, skill_table[sn].name))
			{
				gn = gn_skill_lookup(sn);
				if(gn > 1)
				{
					style_skill = skill_lookup( group_table[gn].name );
					if(ch->pcdata->learned[style_skill] < style_percent[p].percent)
					{
						hide_skill=TRUE;
					}
					if (hide_skill) break;
				}
			}
		}


		if ( sn == gsn_gladiator_skill || sn == gsn_duelist_skill ||
			 sn == gsn_barbarian_skill || sn == gsn_dragoon_skill ||
			 sn == gsn_skirmisher_skill || sn == gsn_tactician_skill)
			hide_skill=TRUE;

		if (skill_table[sn].skill_level[ch->Class()->GetIndex()] > 52)
		continue;
	    if ( ch->level < skill_table[sn].skill_level[ch->Class()->GetIndex()]
	      || ch->pcdata->learned[sn] < 1)
		continue;
	
        if (hide_skill==FALSE)
		{
			sprintf( buf, "%-18s %3d%%  ",
			skill_table[sn].name, ch->pcdata->learned[sn] );
	    	send_to_char( buf, ch );
	    	if ( ++col % 3 == 0 )
			send_to_char( "\n\r", ch );
		}
	}

	if ( col % 3 != 0 )
	    send_to_char( "\n\r", ch );

	sprintf( buf, "You have %d practice sessions left.\n\r",
	    ch->practice );
	send_to_char( buf, ch );
	if (IS_IMMORTAL(ch) && get_trust(ch)>58)
	send_to_char("Use 'set' to change class and see a prac list for other classes.\n\r",ch);
    }
    else
    {
	CHAR_DATA *mob;
	int adept;

	if ( !IS_AWAKE(ch) )
	    return send_to_char( "In your dreams, or what?\n\r", ch );
	if (IS_NPC(ch))
	    return;
	for ( mob = ch->in_room->people; mob != NULL; mob = mob->next_in_room )
	    if ( IS_NPC(mob) && IS_SET(mob->act, ACT_PRACTICE) )
			break;

	if ( mob == NULL)
	    return send_to_char( "You can't do that here.\n\r", ch );

	if ( ch->practice <= 0 )
		return send_to_char( "You have no practice sessions left.\n\r", ch );
	
	if ( ( sn = find_spell( ch,argument ) ) < 0 || ( !IS_NPC(ch)
	&&   (ch->level < skill_table[sn].skill_level[ch->Class()->GetIndex()]
 	||    ch->pcdata->learned[sn] < 1))) /* skill is not known */
	    return send_to_char( "You can't practice that.\n\r", ch );

	if (!str_prefix(argument,"skirmisher")
		|| !str_prefix(argument,"duelist")
		|| !str_prefix(argument,"tactician")
		|| !str_prefix(argument,"barbarian")
		|| (!str_prefix(argument,"dragoon") && str_prefix(argument,"drag"))
		|| !str_prefix(argument,"gladiator"))
		return send_to_char("You can't practice that.\n\r", ch);

        for(p=0; p<MAX_STYLE_SKILL; p++)
               if(!str_cmp(style_percent[p].name, skill_table[sn].name) && (gn=gn_skill_lookup(sn)) > 1)
                    if(ch->pcdata->learned[skill_lookup(group_table[gn].name)] < style_percent[p].percent)
                           return send_to_char("You can't practice that.\n\r",ch);
            
	adept = IS_NPC(ch) ? 100 : 75;

	if ( ch->pcdata->learned[sn] >= adept )
	{
	    sprintf( buf, "You are already learned at %s.\n\r",
		skill_table[sn].name );
	    send_to_char( buf, ch );
	}
	else
	{
	    ch->practice--;
		mcap = 80;

		base = int_app[get_curr_stat(ch,STAT_INT)].learn;
		base *= mcap - ch->pcdata->learned[sn];
		base /= mcap;

		if (base < 1)
			base = 1;

		ch->pcdata->learned[sn] += base;

		if(skill_table[sn].ctype == CMD_SPELL || (skill_table[sn].ctype == CMD_BOTH && 
			ch->Class()->ctype==CLASS_CASTER))
		{
			say_spell(mob,sn);
			act("As $n demonstrates the casting of $t, you gain new insight into the spell.",mob, 
				skill_table[sn].name, ch,TO_VICT);
			act("$N watches studiously as $n demonstrates the $t spell.",mob,skill_table[sn].name,ch,TO_NOTVICT);
		}
		else if(skill_table[sn].ctype == CMD_COMMUNE || (skill_table[sn].ctype == CMD_BOTH && 
			ch->Class()->ctype==CLASS_COMMUNER))
		{
			act("$n recites the prayer of $t.",mob,skill_table[sn].name,0,TO_ROOM);
			send_to_char("You try to memorize the fundamentals of the holy recitation.\n\r",ch);
			act("$N watches studiously as $n demonstrates the $t prayer.",mob,skill_table[sn].name,ch,TO_NOTVICT);
		}
		else// if(skill_table[sn].ctype == CMD_NONE)
		{
			act("$n demonstrates the usage of the $t skill.",mob,skill_table[sn].name,0,TO_ROOM);
			act("You study the fundamental aspects of $t.",ch,skill_table[sn].name,0,TO_CHAR);
			act("$N studiously watches $n's demonstration of the $t skill.",mob,skill_table[sn].name,ch,TO_NOTVICT);
		}
		if(ch->pcdata->learned[sn] >= adept)
		{
		   ch->pcdata->learned[sn] = adept;
		   act( "You have mastered the fundamental aspects of $T.", ch, NULL, skill_table[sn].name, TO_CHAR );
		}
	    
	}
    }
    return;
}



/*
 * 'Wimpy' originally by Dionysos.
 */
void do_wimpy( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    int wimpy;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
        {
	wimpy = ch->max_hit / 5;
        }
    else
	wimpy = atoi( arg );

    if ( wimpy < 0 )
    {
	send_to_char( "Your courage exceeds your wisdom.\n\r", ch );
	return;
    }
    if ( wimpy > ch->max_hit/2 )
    {
	send_to_char( "Such cowardice ill becomes you.\n\r", ch );
	return;
    }
    ch->wimpy	= wimpy;
    sprintf( buf, "Wimpy set to %d hit points.\n\r", wimpy );
    send_to_char(buf,ch);
    return;
}



void do_password( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char *pArg;
    char *pwdnew;
    char *p;
    char cEnd;

    if ( IS_NPC(ch) )
	return;

    /*
     * Can't use one_argument here because it smashes case.
     * So we just steal all its code.  Bleagh.
     */
    pArg = arg1;
    while ( isspace(*argument) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
	*pArg++ = *argument++;
    }
    *pArg = '\0';

    pArg = arg2;
    while ( isspace(*argument) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
	*pArg++ = *argument++;
    }
    *pArg = '\0';

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Syntax: password <old> <new>.\n\r", ch );
	return;
    }

    if ( strcmp(crypt(arg1, ch->pcdata->pwd ), ch->pcdata->pwd ) )
    {
	WAIT_STATE( ch, 40 );
	send_to_char( "Wrong password.  Wait 10 seconds.\n\r", ch );
	return;
    }

    if ( strlen(arg2) < 5 )
    {
	send_to_char(
	    "New password must be at least five characters long.\n\r", ch );
	return;
    }

    /*
     * No tilde allowed because of player file format.
     */
    pwdnew = crypt( arg2, ch->name );
    for ( p = pwdnew; *p != '\0'; p++ )
    {
	if ( *p == '~' )
	{
	    send_to_char(
		"New password not acceptable, try again.\n\r", ch );
	    return;
	}
    }

    free_pstring( ch->pcdata->pwd );
    ch->pcdata->pwd = palloc_string( pwdnew );
    save_char_obj( ch );
    send_to_char( "Ok.\n\r", ch );
    return;
}



char * get_descr_form(CHAR_DATA *ch,CHAR_DATA *looker, bool get_long)
{
    char buf[MAX_STRING_LENGTH];
    char *buf2;

    buf[0] = '\0';
    buf2 = "";
    if(get_long && !is_affected(ch,gsn_cloak_form))
            return ch->long_descr;

    return (char *)PERS(ch, looker);
}

/* Last returns are redundancy ones in case of some failure in checks */

/* Replacement for the handler.c get_char_room to handle morph dragon */
CHAR_DATA * get_char_room( CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *rch;
    int number, count;

    number = number_argument(argument, arg);
	count = 0;
    if (!str_cmp(arg,"self") )
        return ch;

    for (rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room)
    {
	if(argument[0] == '\0')
		return NULL;
        if (!can_see(ch,rch) )
            continue;
        if ( (arg[0] != '\0') && !is_name(arg,rch->name) && (!IS_IMMORTAL(ch) || !rch->true_name || !is_name(arg,rch->true_name)))
            continue;
        if ( ++count == number )
            return rch;
    }
    return NULL;
}

CHAR_DATA * get_char_from_room( CHAR_DATA *ch, ROOM_INDEX_DATA *room, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *rch;
    int number, count;

    number = number_argument(argument, arg);
    count = 0;
    if (!str_cmp(arg,"self") )
        return ch;

    for (rch = room->people; rch != NULL; rch = rch->next_in_room)
    {
        if (!can_see(ch,rch) )
            continue;
        if ( (arg[0] != '\0') && !is_name(arg,rch->name) && (!IS_IMMORTAL(ch) || !rch->true_name || !is_name(arg,rch->true_name)))
            continue;
        if ( ++count == number )
            return rch;
    }
    return NULL;
}

void do_balance(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *banker;
    char buf[100];

    for (banker = ch->in_room->people; banker != NULL; banker = banker->next_in_room)
        if (IS_NPC(banker) && IS_SET(banker->act,ACT_BANKER))
		break;
    if (banker == NULL)
        return send_to_char("You can't do that here.\n\r",ch);

    sprintf(buf,"You have %ld gold coins in your account.\n\r",ch->gold_bank);
    if (ch->gold_bank == 0)
        sprintf(buf,"You have no account here!\n\r");

    send_to_char(buf, ch);
    return;
}

void do_withdraw(CHAR_DATA *ch,char *argument)
{
    CHAR_DATA *banker;
    char arg[MAX_STRING_LENGTH];
    int amount;
    char buf[MAX_STRING_LENGTH];
    int charges, i;
    bool bFound = FALSE;
    OBJ_INDEX_DATA *pIndex;
    OBJ_DATA *pObj;
 
    for (banker = ch->in_room->people; banker != NULL; banker = banker->next_in_room)
        if (IS_SET(banker->act,ACT_BANKER))
            break;
    if (banker == NULL || IS_NPC(ch))
        return send_to_char("You can't do that here.\n\r",ch);

    argument = one_argument(argument,arg);
    if (arg[0] == '\0')
    {
	for(i=0; i<MAX_STORED_ITEMS; i++)
		if(ch->pcdata->deposited_items[i] > 0)
		{
			if(!bFound)
				act("$n hands you a slip of paper with a listing of your assets.",banker,0,ch, TO_VICT);
			pIndex = get_obj_index(ch->pcdata->deposited_items[i]);
			if(!pIndex)
			{
				ch->pcdata->deposited_items[i] = 0;
				continue;
			}
			bFound = TRUE;				
			sprintf(buf,"%d) %s - estimated at %d gold\n\r",i+1,pIndex->short_descr,
				UMAX(1,pIndex->cost+number_range((int)(-.1 * pIndex->cost), (int)(.1 * pIndex->cost))));
			send_to_char(buf,ch);
		}
	if(!bFound)
		mob_tell(banker,ch,"Sorry, you do not seem to have any items stored here.");
	return;
    }
    amount = atoi(arg);
    if (amount <= 0) 
	return mob_tell(banker,ch,"Sorry, withdraw how much?");

    if(amount > 0 && amount <= MAX_STORED_ITEMS)
    {
	if((i = ch->pcdata->deposited_items[amount-1]) == 0 || (pIndex = get_obj_index(i)) == NULL)
		return mob_tell(banker,ch,"I'm sorry, but I can't seem to find any record of that.");
	act("$n nods briefly before scurrying away.",banker,0,ch,TO_VICT);
	act("$n soon returns, followed by two guards hauling a heavy chest.",banker,0,ch,TO_VICT);
	act("$n gets out a large set of keys and unlocks the chest.",banker,0,ch,TO_VICT);
	pObj = create_object(pIndex, 0);
	if(!pObj)
		return;
	if(!can_see_obj(ch, pObj))
	{
		act("You are unable to find what you're looking for in the chest.",ch,0,0,TO_CHAR);
		extract_obj(pObj);
		return;
	}
	act("You retrieve $p from the storage chest.",ch,pObj,0,TO_CHAR);
	obj_to_char(pObj,ch);
	ch->pcdata->deposited_items[amount-1] = 0;
	for(i=1; i<MAX_STORED_ITEMS; i++)		//MUNCH.
		if(ch->pcdata->deposited_items[i-1] == 0 && ch->pcdata->deposited_items[i] > 0)
		{
			ch->pcdata->deposited_items[i-1] = ch->pcdata->deposited_items[i];
			ch->pcdata->deposited_items[i] = 0;
		}
	return;
    }
    charges = 0;

    if (ch->gold_bank < amount)
	return mob_tell(banker,ch,"Sorry, we don't give loans.");	

    ch->gold_bank -= amount;
    ch->gold += amount;
    ch->gold -= charges;
    sprintf(buf,"You withdraw %d gold.\n\r",amount);
    send_to_char(buf,ch);
    if (charges > 0)
    {
	sprintf(buf,"You are charged a fee of %d gold.\n\r",charges);
    	send_to_char(buf,ch);
    }
    return;
}

void do_deposit(CHAR_DATA *ch,char *argument)
{
    CHAR_DATA *banker;
    char arg[MAX_STRING_LENGTH];
    int amount, charges, i;
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *deposited = NULL;
    bool bFull = TRUE;

    for (banker = ch->in_room->people; banker != NULL; banker = banker->next_in_room)
        if (IS_NPC(banker) && IS_SET(banker->act,ACT_BANKER))
			break;
	
    if (!banker || IS_NPC(ch))
        return send_to_char("You can't do that here.\n\r",ch);

    one_argument(argument,arg);
    if (arg[0] == '\0')
        return send_to_char("Syntax: deposit <number of coins>\n\r        deposit <item>\n\r",ch);
    if (!is_number(arg) && (deposited = get_obj_carry( ch, arg, ch )) == NULL)
	return send_to_char("You don't have that.\n\r",ch);

    if (is_number(arg))
    {
    	amount = atoi(arg);
    	if (amount <= 0)
		return mob_tell(banker,ch,"Hey genius, around here we call that a 'with-draw-al'.");

	if (ch->gold < amount)
		return mob_tell(banker,ch,"Sorry, we don't offer loans.");

	if (amount + ch->gold_bank > ch->level * 4000)
		return mob_tell(banker,ch,"Sorry, we can't secure a deposit of that size for you.");

	ch->gold_bank += (2 * amount) / 3;
	ch->gold -= amount;
	charges = amount / 3;

    	sprintf(buf,"You deposit %d gold, less a %d gold transaction fee.\n\r",amount, charges);
   	send_to_char(buf,ch);

    	sprintf(buf,"Your new balance is %ld gold.\n\r",ch->gold_bank);
    	send_to_char(buf,ch);
    	return;
    }
    if(deposited->pIndexData->limtotal > 0)
    {
	act("$n examines $p with an appraising eye.",banker,deposited,ch,TO_VICT);
	mob_tell(banker,ch,"Sorry, we don't insure items of this rarity.");
	return;
    }
    for(i=0; i<MAX_STORED_ITEMS; i++)
	if(ch->pcdata->deposited_items[i] == 0)
	{
		bFull = FALSE;
		break;
	}
    if(bFull)
	return do_say(banker,"I'm sorry, but we are unable to store any more of your belongings.");
    charges = UMAX(deposited->level * 15, deposited->cost / 8);
    if(ch->gold < charges)
	return do_say(banker,"You don't seem to have enough money to cover storage on this.");
    act("$n examines $p at length.",banker,deposited,ch,TO_VICT);
    interpret(banker,"nod");
    sprintf(buf,"%s A pleasure to do business with you.",ch->name);
    do_tell(banker,buf);
    sprintf(buf,"You deposit %s at a cost of %d gold.\n\r",deposited->short_descr,charges);
    ch->gold -= charges;
    send_to_char(buf,ch);
    ch->pcdata->deposited_items[i] = deposited->pIndexData->vnum;
    extract_obj(deposited);
    return;
}

/*
void do_trace(CHAR_DATA *ch,char *argument)
{
    CHAR_DATA *victim;


*/


/* Some shapeshifter code */
/* do a if (can_shapeshift(ch,form_num)) check.
 * If not then return and don't revert. Otherwise shapeshift_revert()
 * and then apply new morph num to the player in the do_morph_dragon fun.
 */

void do_records(CHAR_DATA *ch,char *argument)
{
	char buf[MAX_STRING_LENGTH];
	char buf1[MAX_STRING_LENGTH];
	CHAR_DATA *victim;
 	int count = 0;

    if (!IS_IMMORTAL(ch))
	{
	send_to_char("Huh?\n\r",ch);
	return;
	}

 	buf1[0] = '\0';
	sprintf(buf,"Listing of current active players :\n\r");

	for (victim = char_list; victim != NULL; victim = victim->next)
	{
	if (IS_NPC(victim))
		continue;
	if (victim->pcdata->wanteds > 0)
	{
	count++;
	sprintf(buf1,"%s has %d flags.\n\r",victim->name,victim->pcdata->wanteds);
	strcat(buf,buf1);
	}
	}
	if (count == 0)
	strcat(buf,"None in the realms right now have had a flag before.\n\r");

	send_to_char(buf,ch);
	send_to_char("\n\r",ch);
	return;
}

void do_ratings(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];
	CHAR_DATA *victim;

	if (IS_NPC(ch))
	return;

	if (!(ch->level > LEVEL_HERO)) {
		send_to_char("Huh?\n\r",ch);
		return;
	}

	argument = one_argument( argument, buf);

	if ( buf[0] == '\0' )
	{
		send_to_char("syntax: ratings <char>\n\r",ch);
		return;
	} else {
		victim = get_char_world(ch,buf);
		if (victim == NULL) {
			return;
		}
		if (IS_NPC(victim)) {
			return;
		}
		send_to_char("\n\r",ch);
		act( "$N's PK record:", ch, NULL, victim, TO_CHAR );
sprintf(buf,"Total PKills: %d\n\rGood Kills: %d\n\rNeutral Kills: %d\n\rEvil Kills: %d\n\r\n\rPKDeaths: %d\n\rMob Deaths: %d",
victim->pcdata->kills[PK_KILLS],
victim->pcdata->kills[PK_GOOD],
victim->pcdata->kills[PK_NEUTRAL],
victim->pcdata->kills[PK_EVIL],
victim->pcdata->killed[PK_KILLED],
victim->pcdata->killed[MOB_KILLED]
);
		send_to_char(buf,ch);
		send_to_char("\n\r",ch);
	}
	return;
}

void do_lore( CHAR_DATA *ch, char *argument )
{
  char object_name[MAX_INPUT_LENGTH + 100];
  int i;
  OBJ_DATA *obj;
  OBJ_APPLY_DATA *app;
  int lorebonus = 0;
  char buf[MAX_STRING_LENGTH];
  argument = one_argument(argument, object_name);

  if (get_skill(ch,gsn_lore) <= 1)
      return send_to_char( "You don't know how to do that.\n\r", ch );

  if (object_name[0] == '\0')
	  return send_to_char("Lore what?\n\r", ch);

  if ( ( obj = get_obj_carry( ch, object_name, ch ) ) == NULL )
      return send_to_char( "You are not carrying that.\n\r", ch );

  if(ch->mana < UMAX(obj->level-10, 15))
	return send_to_char("You're too tired to examine it right now.\n\r",ch);
  if(!IS_NPC(ch) && (lorebonus = ch->Profs()->GetProf("forgotten lore") > -1))
  {
        send_to_char("Your knowledge of forgotten lore grants you additional insight.\n\r",ch);
        ch->Profs()->CheckImprove("forgotten lore", 100);
  }
  lorebonus = UMAX(lorebonus, 0);

  act("You examine $p intently.",ch, obj, 0, TO_CHAR);
  if (number_percent() < get_skill(ch,gsn_lore) + lorebonus * 15)
  {
    	sprintf(buf, "Object '%s' is type %s.\n\r",
				obj->name,item_name(obj->item_type));
	sprintf(buf, "%sIt weighs %d pounds.\n\r", buf, obj->weight);
	send_to_char(buf, ch);
	sprintf(buf, "It is level %d.\n\r", obj->level);
	if(lorebonus)
		send_to_char(buf,ch);

    
	switch ( obj->item_type )
      	{
    		case ITEM_SCROLL:
    		case ITEM_POTION:
    		case ITEM_PILL:
		if(lorebonus)
		{
        		sprintf( buf, "It is enchanted with level %d spells of:", obj->value[0] );
        		send_to_char( buf, ch );

			for(int j = 1; j < 5; j++)
        		if(obj->value[j] >= 0 && obj->value[j] < MAX_SKILL && str_cmp(skill_table[obj->value[j]].name,"reserved"))
        		{
            			send_to_char( " '", ch );
            			send_to_char( skill_table[obj->value[j]].name, ch );
            			send_to_char( "'", ch );
        		}
        	send_to_char( ".\n\r", ch );
		}
        break;      
			case ITEM_DRINK_CON:
				sprintf(buf, "It holds %s-colored %s.\n\r", liq_table[obj->value[2]].liq_color, liq_table[obj->value[2]].liq_name);
				send_to_char(buf, ch);
			break;

			case ITEM_CONTAINER:
				sprintf(buf, "It can contain %d objects at a maximum of %d pounds.\n\r",
						obj->value[0], obj->value[3]);
				send_to_char(buf,ch);
			break;

			case ITEM_WEAPON:
				send_to_char("It is a",ch);
				switch (obj->value[0])
				{
					case(WEAPON_EXOTIC):
						send_to_char("n exotic.\n\r",ch);
						break;
					case(WEAPON_SWORD):
						send_to_char(" sword.\n\r",ch);
						break;
					case(WEAPON_DAGGER):
						send_to_char(" dagger.\n\r",ch);
						break;
					case(WEAPON_SPEAR): 
						send_to_char(" spear.\n\r",ch);
						break;
					case(WEAPON_MACE):
						send_to_char(" mace.\n\r",ch);
						break;
					case(WEAPON_AXE):
						send_to_char("n axe.\n\r",ch);
						break;
					case(WEAPON_FLAIL):
						send_to_char(" flail.\n\r",ch);
						break;
					case(WEAPON_WHIP):
						send_to_char(" whip.\n\r",ch);
						break;
					case(WEAPON_POLEARM):
						send_to_char(" polearm.\n\r",ch);
						break;
					case(WEAPON_STAFF):
						send_to_char(" staff.\n\r",ch);
						break;
					default:
						send_to_char("n unknown.\n\r",ch);
						break;
          		}
				
          		sprintf(buf,"Damage is %dd%d (average %d).\n\r", 
							obj->value[1],obj->value[2], (1 + obj->value[2]) * obj->value[1] / 2);
			send_to_char(buf, ch);
			if (obj->value[4] && lorebonus)  /* weapon flags */
			{
				sprintf(buf, "Weapon specials: %s\n\r",weapon_bit_name(obj->value[4]));
				send_to_char(buf,ch);
			}
			break;

			case ITEM_ARMOR:
				sprintf(buf, "Armor class is %d pierce, %d bash, %d slash, and %d vs. magic.\n\r",
						obj->value[0], obj->value[1], obj->value[2], obj->value[3] );
				send_to_char(buf,ch);
			break;
		}

		for (app = obj->apply; app; app = app->next)
		{
			sprintf( buf, "It affects your %s by %d.\n\r", affect_loc_name( app->location ), app->modifier );
			send_to_char(buf,ch);
		}
		check_improve(ch, gsn_lore, TRUE, 2);
	}
	else
	{
		act("You examine $p closely but fail to gain any insight into it.", ch,obj,0,TO_CHAR);
		check_improve(ch, gsn_lore, FALSE, 2);
	}
	ch->mana -= UMAX(obj->level-10 - (lorebonus * 3), 15);
	return;
}

bool isCabalItem( OBJ_DATA *obj )
{
	int i, ovnum = obj->pIndexData->vnum;
	for(i = 1; i<MAX_CABAL; i++)
		if(cabal_table[i].item_vnum == ovnum)
			return TRUE;
	return FALSE;
}

bool isNewbie( CHAR_DATA *ch )
{
/*	if (IS_NPC(ch))
		return FALSE;

	if (ch->pcdata->newbie==TRUE)
		return TRUE;
*/
	return FALSE;
}


void debug_string( const char *str )
{
	FILE *fp;
	
   if ( ( fp = fopen( DEBUG_LOG_FILE, "a" ) ) != NULL )
    {
    fprintf( fp, "DEBUG :: %s\n", str );
    fclose(fp);
    }
}

void do_xlook( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim;
	OBJ_DATA *obj;
	int iWear, foundIR;
	char buf[MAX_STRING_LENGTH];
      CHAR_DATA *vch;
      CHAR_DATA *vch_next;

	if (argument[0] == '\0') {
		send_to_char("Syntax: xlook <char>\n\r", ch );
		return;
	}

	if ( ( victim = get_char_world( ch, argument ) ) == NULL )
	{
		send_to_char( "No such player found.\n\r", ch );
		return;
	}

	if ( !can_see( ch, victim ) )
	{
		send_to_char( "No such player found.\n\r", ch );
		return;
	}

	sprintf(buf,"%s is at '%s' (%d) of area '%s'",
			IS_NPC(victim) ? victim->short_descr : victim->name,
			get_room_name(victim->in_room),
			victim->in_room->vnum,
			victim->in_room->area->name);
	
	send_to_char (buf, ch);
	send_to_char ("\n\r", ch);
	sprintf(buf, "%ihp/%ihp  %im/%im  %imv/%imv\n\r",ch->hit,ch->max_hit,ch->mana,ch->max_mana,ch->move,ch->max_move);
	foundIR = 0;
	send_to_char ("In room: ", ch);
	for (vch = victim->in_room->people; vch != NULL; vch = vch_next)
	{
		vch_next = vch->next_in_room;
		if (vch == victim) {
			continue;
		}
		if (IS_NPC(vch)) {
			continue;
		}
		if (!can_see(ch,vch))
			continue;
		if (is_same_group(vch,victim) )
		{
			send_to_char ("(GROUP)",ch);
		}
		send_to_char(vch->name,ch);
		send_to_char(" ",ch);
		foundIR++;
	}
	if (foundIR == 0) {
		send_to_char("none.",ch);
	}

	if (victim->fighting != NULL) {
		send_to_char ("\n\rFighting: ", ch);
		send_to_char ((IS_NPC(victim->fighting) ? victim->fighting->short_descr : victim->fighting->name), ch);
	} else {
		send_to_char ("\n\rFighting: none.", ch);
	}
	if(victim->description)
	{
		sprintf(buf,"\n\rDescription: %s",victim->description);
		send_to_char(buf,ch);
	}
	send_to_char ("\n\rEquipment:\n\r", ch);

	for ( iWear = 0; iWear < MAX_WEAR; iWear++ )
	{
		if ( ( obj = get_eq_char( victim, iWear ) ) != NULL
		&&   can_see_obj( ch, obj ) )
		{
			send_to_char(where_name[iWear], ch);
			send_to_char(format_obj_to_char( obj, ch, TRUE ), ch);
			send_to_char("\n\r", ch);
		}
	}

	send_to_char ("\n\r", ch);
	send_to_char ("Inventory:\n\r", ch);
	show_list_to_char( victim->carrying, ch, TRUE, FALSE );
	return;
}

char * get_where_name(int iWear)
{
	return where_name[iWear];
}

void do_trustgroup(CHAR_DATA *ch, char *argument)
{
	if(IS_NPC(ch))
		return;
	TOGGLE_BIT(ch->pcdata->trust, TRUST_GROUP);
	if(IS_SET(ch->pcdata->trust,TRUST_GROUP))
		send_to_char("You now trust your group with questionable actions.\n\r",ch);
        if(!IS_SET(ch->pcdata->trust,TRUST_GROUP))
                send_to_char("You no longer trust your group with questionable actions.\n\r",ch);
}
void do_trustcabal(CHAR_DATA *ch, char *argument)
{
        if(IS_NPC(ch))
                return;
        TOGGLE_BIT(ch->pcdata->trust, TRUST_CABAL);
        if(IS_SET(ch->pcdata->trust,TRUST_CABAL))
                send_to_char("You now trust your cabal with questionable actions.\n\r",ch);
        if(!IS_SET(ch->pcdata->trust,TRUST_CABAL))
                send_to_char("You no longer trust your cabal with questionable actions.\n\r",ch);
}

void do_trustchar(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim = get_char_room(ch, argument);
    if(IS_NPC(ch))
		return;
	if(!victim && str_cmp(argument, "self"))
		return send_to_char("Syntax: trust <character>\n\r        trust self",ch);
	if(IS_NPC(victim))
		return send_to_char("They are not interested in your trust.\n\r",ch);
	
	if(!str_cmp(argument,"self"))
	{
		if(!ch->pcdata->trusting)
			return send_to_char("You are not trusting anybody specific.\n\r",ch);
		act("You no longer trust $N with questionable actions.",ch,0,ch->pcdata->trusting,TO_CHAR);
		ch->pcdata->trusting = NULL;
		return;
	}
	ch->pcdata->trusting = victim;
	act("You now trust $N with questionable actions.",ch,0,victim,TO_CHAR);
	return;
}



void do_role(CHAR_DATA *ch, char *argument)
{
    char buf[MAX_BUF], arg1[MAX_BUF], obuf[MAX_BUF];
    CHAR_DATA *victim;
    bool found = FALSE;
    int len;
    BUFFER *output;

	if (IS_NPC(ch))
        return;

	if ( argument[0] != '\0' )
    {
        buf[0] = '\0';
        smash_tilde( argument );
        argument = one_argument(argument,arg1);
	
	if(!strcmp(arg1,"show") && IS_IMMORTAL(ch) && get_trust(ch)>52)
	{
		if(!(victim=get_char_world(ch,argument)))
			return send_to_char("They aren't here.\n\r",ch);
		
		if(IS_NPC(victim))
			return send_to_char("You can't role a mob.\n\r",ch);
        	
		output = new_buf();
		send_to_char(victim->true_name, ch);
        	send_to_char("'s role:\n\r",ch);
        	
		if (victim->pcdata->role == NULL || victim->pcdata->role[0] == '\0') 
		{
                	send_to_char("No role available.\n\r",ch);
        	} 
		else 
		{
                	add_buf(output,victim->pcdata->role);
                	page_to_char(buf_string(output),ch);
                	free_buf(output);
        	}
		
		return;
	}
        
	if (!strcmp(arg1,"-"))
        {
                if (ch->pcdata->temp_role == NULL || ch->pcdata->temp_role[0] == '\0')
                        return send_to_char("No lines left to remove.\n\r",ch);
                strcpy(obuf,ch->pcdata->temp_role);
                for (len = strlen(obuf); len > 0; len--)
                {
                        if (obuf[len] == '\r')
                        {
                                if (!found)
                                {
                                        if (len > 0)
                                                len--;
                                        found = TRUE;
                                }
                                else
                                {
                                        obuf[len + 1] = '\0';
                                        free_pstring(ch->pcdata->temp_role);
                                        ch->pcdata->temp_role = palloc_string(obuf);
                                        return send_to_char("Line deleted.\n\r",ch);
                                }
                        }
                }
                obuf[0] = '\0';
                free_pstring(ch->pcdata->temp_role);
                ch->pcdata->temp_role = palloc_string(obuf);
                return send_to_char("Line deleted.\n\r",ch);
        }
        
	if (!strcmp(arg1,"+"))
        {
                obuf[0] = '\0';
                        
		if (ch->pcdata->temp_role)
                       	strcat(obuf,ch->pcdata->temp_role);        
		strcat(obuf,argument[0] != '\0' ? argument : " ");
                strcat(obuf,"\n\r");
		if(strlen(obuf) + (ch->pcdata->role ? strlen(ch->pcdata->role) : 0) > MAX_BUF-1000)
			return send_to_char("Sorry, your role is too long.\n\r",ch);
                        
		free_pstring(ch->pcdata->temp_role);
                ch->pcdata->temp_role = palloc_string(obuf);
		return send_to_char("Ok.\n\r",ch);
        }
        
	if (!strcmp(arg1,"add")) 
	{
                if (ch->pcdata->temp_role == NULL || ch->pcdata->temp_role[0] == '\0')
                        return send_to_char("You have no role in progress to add.\n\r",ch);
                else 
		{
                        add_role(ch, ch->pcdata->temp_role);
                        free_pstring(ch->pcdata->temp_role);
                        ch->pcdata->temp_role = NULL;
                        show_role(ch);
                }
                return;
        }
	
	if(!str_cmp(arg1,"format"))
	{
		if(!ch->pcdata->temp_role)
			return send_to_char("You have no role in progress to format.\n\r",ch);
		ch->pcdata->temp_role = format_string(ch->pcdata->temp_role);
		send_to_char("Your role in progress has been formatted.\n\r",ch);
		return;
	}
     }
     show_role(ch);
    if(ch->pcdata->temp_role!=NULL)
	show_temp_role(ch);
}


void add_role(CHAR_DATA *ch, char *string)
{
        char buf[MAX_BUF], obuf[MAX_BUF], *strtime;
        strtime = ctime( &current_time );
        chomp(strtime);
        if (ch->pcdata->role)
                sprintf(buf,ch->pcdata->role);
        else
                strcpy(buf,"");
        sprintf(obuf,"Role entry added on %s at level %d (%d hours):\n\r",
                strtime, ch->level, (int)((ch->played + current_time - ch->logon) / 3600));
        strcat(buf,obuf);
        strcat(buf,string);
        strcat(buf,"\n\r");
        free_pstring(ch->pcdata->role);
        ch->pcdata->role = palloc_string(buf);
}

void show_temp_role(CHAR_DATA *ch)
{
        BUFFER *output;
        output = new_buf();
        send_to_char("Role entry in progress:\n\r",ch);
        if (ch->pcdata->temp_role == NULL || ch->pcdata->temp_role[0] == '\0')
                send_to_char("You have no role in progress.\n\r", ch );
        else {
                add_buf(output,ch->pcdata->temp_role);
                page_to_char(buf_string(output),ch);
                free_buf(output);
        }
}

void show_role(CHAR_DATA *ch)
{
        BUFFER *output;
        output = new_buf();
        send_to_char("Your role:\n\r",ch);
        if (ch->pcdata->role == NULL || ch->pcdata->role[0] == '\0') {
                send_to_char("(Empty)\n\r",ch);
        } else {
                add_buf(output,ch->pcdata->role);
                page_to_char(buf_string(output),ch);
                free_buf(output);
        }
        return;
}
