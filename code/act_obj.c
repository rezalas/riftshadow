/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                      *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                      *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                      *
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
#include "merc.h"
#include "magic.h"
#include "tables.h"
#include "spec.h"

/* command procedures needed */
DECLARE_DO_FUN(do_split		);
DECLARE_DO_FUN(do_yell		);
DECLARE_DO_FUN(do_say		);
DECLARE_DO_FUN(do_wake		);
DECLARE_DO_FUN(do_tell		);

/*
 * Local functions.
 */
#define CD CHAR_DATA
#define OD OBJ_DATA
#define CABAL_ITEMS_FILE	"citems.txt"
bool	remove_obj	args( (CHAR_DATA *ch, int iWear, bool fReplace ) );
void	wear_obj	args( (CHAR_DATA *ch, OBJ_DATA *obj, bool fReplace ) );
CD *	find_keeper	args( (CHAR_DATA *ch ) );
int	get_cost	args( (CHAR_DATA *keeper, OBJ_DATA *obj, bool fBuy ) );
void 	obj_to_keeper	args( (OBJ_DATA *obj, CHAR_DATA *ch ) );
OD *	get_obj_keeper	args( (CHAR_DATA *ch,CHAR_DATA *keeper,char *argument));
bool is_owner args((CHAR_DATA *ch,OBJ_DATA *corpse));
bool	check_arms	args( (CHAR_DATA *ch,OBJ_DATA *obj));

/* A pile of obj progs for use in legion.are, and the drannor areas.
Remove these if you don't want to use those areas
*/
void obj_give_hydra_head args((CHAR_DATA *ch,CHAR_DATA *lady,OBJ_DATA *obj));
void wear_obj_zhentil_robe args((CHAR_DATA *ch,OBJ_DATA *obj));
void wear_obj_fallen_wings args((CHAR_DATA *ch,OBJ_DATA *obj));
void remove_obj_fallen_wings args((CHAR_DATA *ch,OBJ_DATA *obj));
void wear_obj_symbol_magic args((CHAR_DATA *ch,OBJ_DATA *obj));
void remove_obj_symbol_magic args((CHAR_DATA *ch,OBJ_DATA *obj));
void recite_libram_conjuration args((CHAR_DATA *ch,CHAR_DATA *victim,OBJ_DATA *scroll,OBJ_DATA *obj,bool fRead));
void brandish_sceptre_dead args((CHAR_DATA *ch,OBJ_DATA *obj));
void wear_obj_misty_cloak args((CHAR_DATA *ch,OBJ_DATA *obj));
void remove_obj_misty_cloak args((CHAR_DATA *ch,OBJ_DATA *obj));
void save_cabal_items args( ( void ) );

#undef OD
#undef	CD

#define MINOTAUR_ONLY 		I

/* RT part of the corpse looting code */

bool check_arms(CHAR_DATA *ch, OBJ_DATA *obj) {
	char buf[MSL];
	OBJ_AFFECT_DATA *paf = NULL;

	if(is_affected_obj(obj, gsn_arms_of_light)
		|| is_affected_obj(obj, gsn_arms_of_wrath)
                || is_affected_obj(obj, gsn_arms_of_purity)
                || is_affected_obj(obj, gsn_arms_of_judgement)) {

                for(paf = obj->affected; paf != NULL; paf=paf->next) {
                        if((paf->type == gsn_arms_of_light
                                || paf->type == gsn_arms_of_wrath
                                || paf->type == gsn_arms_of_purity
                                || paf->type == gsn_arms_of_judgement) && paf->owner != ch) {
					act("$p shocks you, falling from your numb hands.",
						ch, obj, 0, TO_CHAR);
					act("$n drops $p.", ch, obj, 0, TO_ROOM);
					return TRUE;
			}
		}
	}

	return FALSE;
}


bool can_loot(CHAR_DATA *ch, OBJ_DATA *obj)
{
    CHAR_DATA *owner, *wch;
    char buf[MAX_STRING_LENGTH];

    if (obj->item_type == ITEM_CORPSE_PC && (!IS_NPC(ch) || IS_AFFECTED(ch,AFF_CHARM)))
    {
	sprintf(buf,"%s looting %s.",IS_NPC(ch) ?
((ch->master == NULL) ? "Unknown mob" : ch->master->name) : ch->name,
obj->short_descr);
	log_string(buf);
	ch->pause = 5;
    }
    if (obj->item_type != ITEM_CORPSE_PC)
	return TRUE;

    if(IS_NPC(ch) && IS_AFFECTED(ch,AFF_CHARM))
	return FALSE;

    if(IS_NPC(ch))
	return FALSE;

    if (!obj->owner || obj->owner == NULL)
	return TRUE;

    if(!str_cmp(ch->true_name,obj->owner))
    	return TRUE;

    if(!IS_NPC(ch) && ch->pcdata->newbie==TRUE)
	{
		send_to_char("You can't loot other players as a newbie.\n\r",ch);
		return FALSE;
	}
    return TRUE;

    owner = NULL;
    for ( wch = char_list; wch != NULL ; wch = wch->next )
        if (!IS_NPC(wch) && obj->owner && !str_cmp(wch->true_name,obj->owner))
            owner = wch;

    if (owner == NULL || IS_NPC(owner))
	return TRUE;

    if (!str_cmp(ch->true_name,owner->true_name))
	return TRUE;

    if (is_same_group(ch,owner))
	return TRUE;

    return FALSE;
}


void get_obj( CHAR_DATA *ch, OBJ_DATA *obj, OBJ_DATA *container, bool pcheck )
{
    /* variables for AUTOSPLIT */
    CHAR_DATA *gch;
    int members, pchance;
    char buffer[100];

	if ( is_affected_obj( obj, gsn_stash))
		affect_strip_obj(obj, gsn_stash);
	
	if ( !CAN_WEAR(obj, ITEM_TAKE) )
		return send_to_char( "You can't take that.\n\r", ch );

	if (obj->item_type == ITEM_CORPSE_PC && !IS_IMMORTAL(ch))
		return send_to_char( "You can't take that.\n\r", ch );

    if ( ch->carry_number + get_obj_number( obj ) > can_carry_n( ch ) )
		return act( "$p: you can't carry that many items.", ch, obj, 0, TO_CHAR );

    if ((!obj->in_obj || obj->in_obj->carried_by != ch)
    &&  (get_carry_weight(ch) + get_obj_weight(obj) > can_carry_w(ch)))
		return act( "$p: you can't carry that much weight.", ch, obj, 0, TO_CHAR );

    if (!can_loot(ch,obj))
		return act("Corpse looting is not permitted for you.",ch,NULL,NULL,TO_CHAR );

    if (obj->in_room != NULL)
    {
		for (gch = obj->in_room->people; gch != NULL; gch = gch->next_in_room)
	    	if (gch->on == obj)
				return act("$N appears to be using $p.", ch,obj,gch,TO_CHAR);
    }
		
    if(check_arms(ch, obj))
	return;

    if ( container != NULL )
    {
    	if (container->pIndexData->vnum == OBJ_VNUM_PIT
			&&  !CAN_WEAR(container, ITEM_TAKE))
	    	obj->timer = 0;	
	
		if ((pchance = get_skill(ch,gsn_drag)) != 0 && IS_AFFECTED(ch, AFF_SNEAK) && pcheck == TRUE)
		{
			act("You attempt to silently get $p from $P.", ch, obj, container, TO_CHAR);
			if(number_percent() < pchance)
			{
				act("IMM: $n palmed $p from $P.", ch, obj, container, TO_IMMINROOM );
				check_improve(ch, gsn_palm, TRUE, 1);
			}
			else
			{
				act( "$n gets $p from $P.", ch, obj, container, TO_ROOM );
				check_improve(ch, gsn_palm, FALSE, 1);
			}
		}
		else
		{
			act( "You get $p from $P.", ch, obj, container, TO_CHAR );
			act( "$n gets $p from $P.", ch, obj, container, TO_ROOM );
		}
	
		obj_from_obj( obj );
    }
    else
    {
		if ((pchance = get_skill(ch,gsn_palm)) != 0 && IS_AFFECTED(ch, AFF_SNEAK) && pcheck == TRUE)
		{
			act("You attempt to silently get $p.", ch, obj, NULL, TO_CHAR);
			if(number_percent() < pchance)
			{
				act("IMM: $n palmed $p.", ch, obj, NULL, TO_IMMINROOM );
				check_improve(ch, gsn_palm, TRUE, 1);
			}
			else
			{
				act( "$n gets $p.", ch, obj, NULL, TO_ROOM );
				check_improve(ch, gsn_palm, FALSE, 1);
			}
		}
		else
		{
			act( "You get $p.", ch, obj, container, TO_CHAR );
			act( "$n gets $p.", ch, obj, container, TO_ROOM );
		}
		obj_from_room( obj );
	}

    if ( obj->item_type == ITEM_MONEY)
    {
		ch->gold += obj->value[1];
        if (IS_SET(ch->act,PLR_AUTOSPLIT))
        { /* AUTOSPLIT code */
    	  members = 0;
    	  for (gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    	  {
            if (!IS_AFFECTED(gch,AFF_CHARM) && is_same_group( gch, ch ) )
              members++;
    	  }

	  	if ( members > 1 && (obj->value[0] > 1 || obj->value[1]))
	  	{
	    	sprintf(buffer,"%d",obj->value[1]);
	    	do_split(ch,buffer);	
	  	}
	}

	extract_obj( obj );
	}
    else
    {
		obj_to_char( obj, ch );
		if (IS_SET(obj->progtypes,IPROG_GET))
			(obj->pIndexData->iprogs->get_prog) (obj,ch);
    		if(TRAPS_IEVENT(obj,TRAP_ILOOT))
			CALL_IEVENT(obj,TRAP_ILOOT,ch,NULL,obj);
    }

    return;
}



void do_get( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj=NULL;
    OBJ_DATA *obj_next;
    OBJ_DATA *container;
    bool found;
    bool isPcCorpse;

    found = FALSE;
    isPcCorpse = FALSE;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (!str_cmp(arg2,"from"))
		argument = one_argument(argument,arg2);

    /* Get type. */
    if ( arg1[0] == '\0' )
		return send_to_char( "Get what?\n\r", ch );

	if (!IS_NPC(ch))
		if (ch->pcdata->death_status == HAS_DIED)
			return send_to_char("You are a hovering spirit, you can't pick up anything!\n\r",ch);

	if(IS_SHIFTED(ch))
		return send_to_char("You don't have hands to pick anything up.\n\r",ch);

    if ( arg2[0] == '\0' ) {
		if ( str_cmp( arg1, "all" ) && str_prefix( "all.", arg1 ) ) {
	    	/* 'get obj' */
	    	obj = get_obj_list( ch, arg1, ch->in_room->contents );
	    	if ( obj == NULL || (IS_SET(obj->extra_flags, ITEM_NOSHOW) && !IS_IMMORTAL(ch)))
				return act( "You see no $T here.", ch, NULL, arg1, TO_CHAR );

			get_obj( ch, obj, NULL, TRUE );

    		if (cant_carry(ch,obj)) {
				act("$n is burnt by $p and drops it.",ch,obj,0,TO_ROOM);
				act("You are burnt by $p and drop it.",ch,obj,0,TO_CHAR);
				obj_from_char(obj);
				obj_to_room(obj,ch->in_room);
    		}

		} else {
	    	/* 'get all' or 'get all.obj' */
	    	found = FALSE;
	    	for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next ) {
				obj_next = obj->next_content;

				if(IS_SET(obj->extra_flags, ITEM_NOSHOW) && !IS_IMMORTAL(ch))
					continue;

				if ( ( arg1[3] == '\0' || is_name( &arg1[4], obj->name ) )
					&&   can_see_obj( ch, obj ) ) {
		    		found = TRUE;
		    		get_obj( ch, obj, NULL, FALSE );

    				if (cant_carry(ch,obj)) {
						act("$n is burnt by $p and drops it.",ch,obj,0,TO_ROOM);
						act("You are burnt by $p and drop it.",ch,obj,0,TO_CHAR);
						obj_from_char(obj);
						obj_to_room(obj,ch->in_room);
    				}	

				}
	    	}

	    	if ( !found ) {
				if ( arg1[3] == '\0' )
		    		send_to_char( "I see nothing here.\n\r", ch );
				else
		    		act( "I see no $T here.", ch, NULL, &arg1[4], TO_CHAR );
	    	}
		}
    } else {
		/* 'get ... container' */
		if ( !str_cmp( arg2, "all" ) || !str_prefix( "all.", arg2 ) )
	    	return send_to_char( "You can't do that.\n\r", ch );

		if ( ( container = get_obj_here( ch, arg2 ) ) == NULL )
	    	return act( "I see no $T here.", ch, NULL, arg2, TO_CHAR );

		switch ( container->item_type ) {
			default:
	    		return send_to_char( "That's not a container.\n\r", ch );

			case ITEM_CONTAINER:
			case ITEM_CORPSE_NPC:
	    		break;

			case ITEM_CORPSE_PC:
	   	 	{
				isPcCorpse = TRUE;

				if (!can_loot(ch,container))
		    		return send_to_char( "You can't do that.\n\r", ch );
	    	}
		}

		if ( IS_SET_OLD(container->value[1], CONT_CLOSED) )
   	 		return act( "The $d is closed.", ch, NULL, container->name, TO_CHAR );

		if (is_affected_obj(container, gsn_ice_blast))
			return act("$p is frozen shut!",ch,container,0,TO_CHAR);
	
		if ( str_cmp( arg1, "all" ) && str_prefix( "all.", arg1 ) ) {
   		 	/* 'get obj container' */
			if(isPcCorpse && !IS_IMMORTAL(ch) && !str_infix(".",arg1) && !is_owner(ch,container))
				return send_to_char( "What exactly is it you want to get?\n\r", ch );

			obj = get_obj_list( ch, arg1, container->contains );

	    	if ( obj == NULL )
				return act( "I see nothing like that in the $T.", ch, NULL, arg2, TO_CHAR );

	        if (IS_SET(container->progtypes,IPROG_LOOT))
				if (((container->pIndexData->iprogs->loot_prog)
					(container,obj,ch)) == FALSE)
					return;

		if(TRAPS_IEVENT(container,TRAP_ILOOT))
			if(CALL_IEVENT(container,TRAP_ILOOT,ch,container,obj))
				return;

		if (isPcCorpse && is_affected_obj(container,gsn_noxious_ward)
			&& !is_owner(ch,container))
				damage_new(ch,ch,dice(ch->level,2),gsn_noxious_ward,DAM_OTHER,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"the noxious ward*");

		get_obj( ch, obj, container, TRUE );

   		if (cant_carry(ch,obj)) {
			act("$n is burnt by $p and drops it.",ch,obj,0,TO_ROOM);
			act("You are burnt by $p and drop it.",ch,obj,0,TO_CHAR);
			obj_from_char(obj);
			obj_to_room(obj,ch->in_room);
		}

		} else {
		    /* 'get all container' or 'get all.obj container' */
			if (isPcCorpse && !IS_IMMORTAL(ch) && !is_owner(ch,container))
				return send_to_char( "Why not try one object at a time?\n\r", ch );

			found = FALSE;
		    for ( obj = container->contains; obj != NULL; obj = obj_next )
		    {
				obj_next = obj->next_content;
				if ( ( arg1[3] == '\0' || is_name( &arg1[4], obj->name ) )
					&&   can_see_obj( ch, obj ) )
				{
		    			found = TRUE;

					if (IS_SET(container->progtypes,IPROG_LOOT))
						if (((container->pIndexData->iprogs->loot_prog)
							(container,obj,ch)) == FALSE)
							return;
					if(TRAPS_IEVENT(container,TRAP_ILOOT))
						if(CALL_IEVENT(container,TRAP_ILOOT,ch,container,obj))
							return;

			    		get_obj( ch, obj, container, FALSE );

					if (cant_carry(ch,obj))
					{
						act("$n is burnt by $p and drops it.",ch,obj,0,TO_ROOM);
						act("You are burnt by $p and drop it.",ch,obj,0,TO_CHAR);
						obj_from_char(obj);
						obj_to_room(obj,ch->in_room);
    					}
				}
		    }

		    if ( !found ) {
				if ( arg1[3] == '\0' )
			    		act( "I see nothing in the $T.",ch, NULL, arg2, TO_CHAR );
				else
		    			act( "I see nothing like that in the $T.",ch, NULL, arg2, TO_CHAR );
		    }
		}
	}

    return;
}



void do_put( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *container;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
	int pchance;
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (!str_cmp(arg2,"in") || !str_cmp(arg2,"on"))
		argument = one_argument(argument,arg2);

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
		return send_to_char( "Put what in what?\n\r", ch );

    if ( !str_cmp( arg2, "all" ) || !str_prefix( "all.", arg2 ) )
		return send_to_char( "You can't do that.\n\r", ch );

    if ( ( container = get_obj_here( ch, arg2 ) ) == NULL )
		return act( "I see no $T here.", ch, NULL, arg2, TO_CHAR );

    if ( container->item_type != ITEM_CONTAINER )
		return send_to_char( "That's not a container.\n\r", ch );

	if (is_affected_obj(container, gsn_ice_blast))
		return act("$p is frozen shut!",ch,container,0,TO_CHAR);
	
    if ( IS_SET_OLD(container->value[1], CONT_CLOSED) )
		return act( "The $d is closed.", ch, NULL, container->name, TO_CHAR );

    if ( str_cmp( arg1, "all" ) && str_prefix( "all.", arg1 ) )
	{
	
		/* 'put obj container' */
	if ( ( obj = get_obj_carry( ch, arg1, ch ) ) == NULL )
    	return send_to_char( "You do not have that item.\n\r", ch );

	if ( obj == container )
	   	return send_to_char( "You can't fold it into itself.\n\r", ch );

	if ( !can_drop_obj( ch, obj ) )
	    return send_to_char( "You can't let go of it.\n\r", ch );

    if (isCabalItem(obj))
	    return send_to_char( "That item is too powerful to be contained.\n\r", ch );

	if (obj->pIndexData->limtotal > 0 && container->pIndexData->vnum != 31 )
	    return send_to_char( "That item is too powerful to be contained.\n\r", ch );

	if (IS_OBJ_STAT(obj, ITEM_NO_STASH))
		return send_to_char("You can't put that inside a container!\n\r",ch);

    if (WEIGHT_MULT(obj) != 100  && container->pIndexData->vnum != 31)
		return send_to_char("You have a feeling that would be a bad idea.\n\r",ch);

	if ((get_contents_number(container)) >= container->value[0] ||
		(get_contents_weight(container) + get_true_weight(obj) > container->value[3]))
	    return send_to_char( "It won't fit.\n\r", ch );
	
	if (container->pIndexData->vnum == OBJ_VNUM_PIT
		&&  !CAN_WEAR(container,ITEM_TAKE)) {
	        obj->timer = number_range(100,200);
	}

	obj_from_char( obj );
	obj_to_obj( obj, container );

	if ((pchance = get_skill(ch,gsn_palm)) != 0 && IS_AFFECTED(ch, AFF_SNEAK)) {
		if(number_percent() < pchance) {
			if(IS_SET_OLD(container->value[1],CONT_PUT_ON)) {
				act("You attempt to silently put $p on $P.", ch, obj, container, TO_CHAR);
				act("IMM: $n palmed $p on $P", ch, obj, container, TO_IMMINROOM );
			} else {
				act("You attempt to silently put $p in $P.", ch, obj, container, TO_CHAR);
				act("IMM: $n palmed $p in $P", ch, obj, container, TO_IMMINROOM );
			}
			check_improve(ch, gsn_palm, TRUE, 1);
		} else {
			if(IS_SET_OLD(container->value[1],CONT_PUT_ON))
			{
				act("You attempt to silently put $p on $P.", ch, obj, container, TO_CHAR);
				act("$n puts $p on $P.",ch,obj,container, TO_ROOM);
			}
			else
			{
				act("You attempt to silently put $p in $P.", ch, obj, container, TO_CHAR);
				act("$n puts $p in $P.",ch,obj,container, TO_ROOM);
			}
			check_improve(ch, gsn_palm, FALSE, 1);
		}		
	}
	else
	{
		if (IS_SET_OLD(container->value[1],CONT_PUT_ON))
		{
	    	act("$n puts $p on $P.",ch,obj,container, TO_ROOM);
	    	act("You put $p on $P.",ch,obj,container, TO_CHAR);
		}
		else
		{
	    	act( "$n puts $p in $P.", ch, obj, container, TO_ROOM );
	    	act( "You put $p in $P.", ch, obj, container, TO_CHAR );
		}
	}
	}
    else
    {
	/* 'put all container' or 'put all.obj container' */
	for ( obj = ch->carrying; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;

	    if ( ( arg1[3] == '\0' || is_name( &arg1[4], obj->name ) )
	    &&   can_see_obj( ch, obj )
	    &&   WEIGHT_MULT(obj) == 100
	    &&   obj->wear_loc == WEAR_NONE
	    &&   obj != container
	    &&   obj->pIndexData->limtotal <= 0
            && (get_contents_number(container)) < container->value[0]
            &&  (get_contents_weight(container) + get_true_weight(obj) <= container->value[3])
	    &&   can_drop_obj( ch, obj )
    && (!isCabalItem(obj)))
	    {

	    	if (container->pIndexData->vnum == OBJ_VNUM_PIT
	    	&&  !CAN_WEAR(obj, ITEM_TAKE) )
	    	{
	    	    	obj->timer = number_range(100,200);
		}
		obj_from_char( obj );
		obj_to_obj( obj, container );

		if (IS_SET_OLD(container->value[1],CONT_PUT_ON))
        	{
            	act("$n puts $p on $P.",ch,obj,container, TO_ROOM);
            	act("You put $p on $P.",ch,obj,container, TO_CHAR);
        	}
			else
			{
		    	act( "$n puts $p in $P.", ch, obj, container, TO_ROOM );
		    	act( "You put $p in $P.", ch, obj, container, TO_CHAR );
			}
	}
	}
    }

    return;
}



void do_drop( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    bool found;
	int pchance;

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Drop what?\n\r", ch );
	return;
    }

    if ( is_number( arg ) )
    {
	/* 'drop NNNN coins' */
	int amount, gold = 0;

	amount   = atoi(arg);
	argument = one_argument( argument, arg );
	if ( amount <= 0
	|| (str_cmp( arg, "gold"  )))
	{
	    send_to_char( "Sorry, you can't do that.\n\r", ch );
	    return;
	}

	    if (ch->gold < amount)
	    {
		send_to_char("You don't have that much gold.\n\r",ch);
		return;
	    }

	    ch->gold -= amount;
  	    gold = amount;

	for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;

	    switch ( obj->pIndexData->vnum )
	    {

	    case OBJ_VNUM_GOLD_ONE:
		gold += 1;
		extract_obj( obj );
		break;

	    case OBJ_VNUM_GOLD_SOME:
		gold += obj->value[1];
		extract_obj( obj );
		break;

	    case OBJ_VNUM_COINS:
		gold += obj->value[1];
		extract_obj(obj);
		break;
	    }
	}

	obj_to_room( create_money( gold ), ch->in_room );
	if ((pchance = get_skill(ch,gsn_palm)) != 0 && IS_AFFECTED(ch, AFF_SNEAK))
	{
		act("You attempt to silently drop some gold.", ch, NULL, NULL, TO_CHAR);
		if(number_percent() < pchance)
		{
			act("IMM: $n dropped some gold. (palm)", ch, obj, NULL, TO_IMMINROOM );
			check_improve(ch, gsn_palm, TRUE, 1);
		}
		else
		{
			act( "$n drops some gold.", ch, NULL, NULL, TO_ROOM );
			check_improve(ch, gsn_palm, FALSE, 1);
		}
	}
	else
	{
		act( "$n drops some gold.", ch, NULL, NULL, TO_ROOM );
		send_to_char( "OK.\n\r", ch );
		return;
    }
	}
    if ( str_cmp( arg, "all" ) && str_prefix( "all.", arg ) )
    {
	/* 'drop obj' */
	if ( ( obj = get_obj_carry( ch, arg, ch ) ) == NULL )
	{
	    send_to_char( "You do not have that item.\n\r", ch );
	    return;
	}

	if ( !can_drop_obj( ch, obj ) )
	{
	    send_to_char( "You can't let go of it.\n\r", ch );
	    return;
	}
	obj_from_char( obj );
	obj_to_room( obj, ch->in_room );
	
	if ((pchance = get_skill(ch,gsn_palm)) != 0 && IS_AFFECTED(ch, AFF_SNEAK))
	{
		act("You attempt to silently drop $p.", ch, obj, NULL, TO_CHAR);
		if(number_percent() < pchance)
		{
			act("IMM: $n palmed $p.", ch, obj, NULL, TO_IMMINROOM );
			check_improve(ch, gsn_palm, TRUE, 1);
		}
		else
		{
			act( "$n drops $p.", ch, obj, NULL, TO_ROOM );
			check_improve(ch, gsn_palm, FALSE, 1);
		}
	} else {
		act( "$n drops $p.", ch, obj, NULL, TO_ROOM );
		act( "You drop $p.", ch, obj, NULL, TO_CHAR );
	}
	
	if (IS_SET(obj->progtypes,IPROG_DROP))
		(obj->pIndexData->iprogs->drop_prog) (obj,ch);

	if (IS_SET(ch->in_room->progtypes, RPROG_DROP))
		(ch->in_room->rprogs->drop_prog) (ch->in_room, ch, obj);

	if (IS_OBJ_STAT(obj,ITEM_MELT_DROP)) {
	    act("$p dissolves into smoke.",ch,obj,NULL,TO_ROOM);
	    act("$p dissolves into smoke.",ch,obj,NULL,TO_CHAR);
	    extract_obj(obj);
	}
    }
    else
    {
	/* 'drop all' or 'drop all.obj' */
	found = FALSE;
	if(str_prefix( "all.", arg ))
		wiznet("$N just dropped all $S stuff!",ch,NULL,0,0,0);
	for ( obj = ch->carrying; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;

	    if ( ( arg[3] == '\0' || is_name( &arg[4], obj->name ) )
	    &&   can_see_obj( ch, obj )
	    &&   obj->wear_loc == WEAR_NONE
	    &&   can_drop_obj( ch, obj ) )
	    {
		found = TRUE;
		obj_from_char( obj );
		obj_to_room( obj, ch->in_room );

		act( "$n drops $p.", ch, obj, NULL, TO_ROOM );
		act( "You drop $p.", ch, obj, NULL, TO_CHAR );

		if (IS_SET(obj->progtypes,IPROG_DROP))
			(obj->pIndexData->iprogs->drop_prog) (obj,ch);

		if (IS_SET(ch->in_room->progtypes, RPROG_DROP))
			(ch->in_room->rprogs->drop_prog) (ch->in_room, ch, obj);

		if(TRAPS_IEVENT(obj,TRAP_IDROP))
			CALL_IEVENT(obj,TRAP_IDROP,ch,obj);
		
		if (IS_OBJ_STAT(obj,ITEM_MELT_DROP)) {
			act("$p dissolves into smoke.",ch,obj,NULL,TO_ROOM);
			act("$p dissolves into smoke.",ch,obj,NULL,TO_CHAR);
			extract_obj(obj);
		}
	    }
	}

	if ( !found )
	{
	    if ( arg[3] == '\0' )
		act( "You are not carrying anything.",
		    ch, NULL, arg, TO_CHAR );
	    else
		act( "You are not carrying any $T.",
		    ch, NULL, &arg[4], TO_CHAR );
	}
    }

    return;
}



void do_give( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA  *obj;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Give what to whom?\n\r", ch );
	return;
    }
    if ( is_number( arg1 ) )
    {
	/* 'give NNNN coins victim' */
	int amount;

	amount   = atoi(arg1);
	if ( amount <= 0
	|| ( str_cmp( arg2, "coins" ) && str_cmp( arg2, "coin" ) &&
	     str_cmp( arg2, "gold"  )) )
	{
	    send_to_char( "Sorry, you can't do that.\n\r", ch );
	    return;
	}


	argument = one_argument( argument, arg2 );
	if ( arg2[0] == '\0' )
	{
	    send_to_char( "Give what to whom?\n\r", ch );
	    return;
	}

	if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
	{
	    send_to_char( "They aren't here.\n\r", ch );
	    return;
	}
	if(IS_SHIFTED(victim))
		return send_to_char("Why don't you just put it down and back away?\n\r",ch);
	if (ch->gold < amount)
	{
	    send_to_char( "You haven't got that much.\n\r", ch );
	    return;
	}

	    ch->gold		-= amount;
	    victim->gold	+= amount;

	sprintf(buf,"$n gives you %d gold.",amount);
	act( buf, ch, NULL, victim, TO_VICT    );
	act( "$n gives $N some coins.",  ch, NULL, victim, TO_NOTVICT );
	sprintf(buf,"You give $N %d gold.",amount);
	act( buf, ch, NULL, victim, TO_CHAR    );

        if (IS_SET(victim->progtypes,MPROG_BRIBE))
          (victim->pIndexData->mprogs->bribe_prog) (victim,ch,amount);

	if(amount>999 && ch->cabal==CABAL_BOUNTY)
	{
		sprintf(buf,"%s gives %d gold to %s.",ch->name,amount,victim->true_name);
		bounty_log(buf);
	}	
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg1, ch ) ) == NULL )
    {
	send_to_char( "You do not have that item.\n\r", ch );
	return;
    }

    if ( obj->wear_loc != WEAR_NONE )
    {
	send_to_char( "You must remove it first.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ((IS_NPC(victim) && victim->pIndexData->pShop != NULL) && !IS_SET(victim->progtypes, MPROG_GIVE) )
	return (void)act("$E wouldn't want that.",ch,0,victim,TO_CHAR);

    if ( !can_drop_obj( ch, obj ) )
    {
	send_to_char( "You can't let go of it.\n\r", ch );
	return;
    }

	if (IS_AFFECTED(obj,AFF_OBJ_BURNING)) {
		send_to_char("You really think they would want that?\n\r",ch);
		return;
	}
	
    if ( victim->carry_number + get_obj_number( obj ) > can_carry_n( victim ) )
    {
		act( "$N has $S hands full.", ch, NULL, victim, TO_CHAR );
		act( "$n tries to hand you $p, but your hands are full.", ch, obj, victim, TO_VICT);
		act( "$n tries to hand $N $p, but $S hands are full.", ch, obj, victim, TO_NOTVICT);
		return;
    }

    if (get_carry_weight(victim) + get_obj_weight(obj) > can_carry_w( victim ) )
    {
		act( "$N can't carry that much weight.", ch, NULL, victim, TO_CHAR );
		act( "$n tries to hand you something, but you can not carry that much weight.", ch, NULL, victim, TO_VICT);
		act( "$n tries to hand $N $p but $E is already carrying to much weight.", ch, obj, victim, TO_NOTVICT);
		return;
    }

    if ( !can_see_obj( victim, obj ) )
    {
		act( "$N can't see it.", ch, NULL, victim, TO_CHAR );
		act( "$n waves $p in front of you.", ch, obj, victim, TO_VICT);
		act( "$n waves $p in front of $N.", ch, obj, victim, TO_NOTVICT);
		return;
    }

    if (IS_SET(obj->progtypes,IPROG_GIVE))
    {
        if((obj->pIndexData->iprogs->give_prog) (obj,ch,victim) == TRUE);
		return;
    }
    obj_from_char( obj );
    obj_to_char( obj, victim );
    act( "$n gives $p to $N.", ch, obj, victim, TO_NOTVICT );
    act( "$n gives you $p.",   ch, obj, victim, TO_VICT    );
    act( "You give $p to $N.", ch, obj, victim, TO_CHAR    );

    if (IS_SET(victim->progtypes,MPROG_GIVE))
      	(victim->pIndexData->mprogs->give_prog) (victim,ch,obj);
	if(TRAPS_MEVENT(victim,TRAP_MGIVE))
		CALL_MEVENT(victim,TRAP_MGIVE,ch,victim,obj);
    if(TRAPS_IEVENT(obj,TRAP_IGIVE))
      	CALL_IEVENT(obj,TRAP_IGIVE,ch,victim,obj);

    return;
}

void do_skin( CHAR_DATA *ch, char *argument)
{
    OBJ_DATA *find_corpse;
    OBJ_DATA *corpse;
    OBJ_DATA *sack;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    int number, count;
    AFFECT_DATA af;
    char *name;
    char *last_name;
    char arg1[MAX_STRING_LENGTH];
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];

    corpse = NULL;
    if ( (get_skill(ch,gsn_skin) == 0)
	|| (ch->level < skill_table[gsn_skin].skill_level[ch->Class()->GetIndex()]) )
    {
	send_to_char("But you don't know how to skin a corpse.\n\r",ch);
	return;
    }
    if (is_affected(ch,gsn_skin))
    {
	send_to_char("You do not feel up to fashioning a new skin yet.\n\r",ch);
	return;
    }
    if (ch->mana < 25)
    {
	send_to_char("You don't have the mana.\n\r",ch);
	return;
    }

    one_argument(argument,arg1);
    number = number_argument(argument,arg1);
    count = 0;
    for (find_corpse = ch->carrying; find_corpse != NULL; find_corpse = find_corpse->next_content)
    {
	if (find_corpse->wear_loc == WEAR_NONE && (can_see_obj(ch,find_corpse) )
    	    && ( arg1[0] == '\0' || is_name(arg1,find_corpse->name)) )
       {
	if (++count == number )
	corpse = find_corpse;
       }
    }

    if (corpse == NULL)
    {
	send_to_char("You don't have that.\n\r",ch);
	return;
    }
    if ((corpse->item_type != ITEM_CORPSE_NPC) && (corpse->item_type != ITEM_CORPSE_PC))
    {
 	send_to_char("You can only skin a corpse you are carrying.\n\r",ch);
	return;
    }

   if (number_percent() > ch->pcdata->learned[gsn_skin])
   {
	act("You fail and destroy $p.",ch,corpse,NULL,TO_CHAR);
	act("$n tries to skin a corpse but fails and destroys it.",ch,NULL,NULL,TO_ROOM);
	extract_obj(corpse);
	ch->mana -= 25;
	return;
    }

    act("$n brutally skins $p and fashions $mself a bloody sack.",ch,corpse,NULL,TO_ROOM);
    act("You skin $p and make a new rangers sack!",ch,corpse,NULL,TO_CHAR);

    name = corpse->short_descr;
    for (obj = corpse->contains; obj != NULL; obj = obj_next)
    {
	obj_next = obj->next_content;
	obj_from_obj(obj);
	obj_to_room(obj,ch->in_room);
    }

    sack = create_object(get_obj_index(OBJ_VNUM_SACK),1);
    sack->level = corpse->level;

    last_name = name;
    last_name = one_argument(last_name,name);
    last_name = one_argument(last_name,name);
    last_name = one_argument(last_name,name);
    name = last_name;
    sprintf(buf1, "a sack made from the skin of %s",name);
    sprintf(buf2, "a bloody sack fashioned from the torn skin of %s is here.",name);
    free_pstring(sack->short_descr);
    free_pstring(sack->description);
    sack->short_descr = palloc_string(buf1);
    sack->description = palloc_string(buf2);

    obj_to_char(sack,ch);
    extract_obj(corpse);
    ch->mana -= 25;

    init_affect(&af);
    af.where = TO_AFFECTS;
af.aftype = AFT_SKILL;
    af.type = gsn_skin;
    af.modifier = 0;
    af.location = 0;
    af.duration = 24;
    af.level = ch->level;
    affect_to_char(ch,&af);

    return;
}

void do_butcher( CHAR_DATA *ch, char *argument)
{
    OBJ_DATA *find_corpse;
    OBJ_DATA *corpse;
    OBJ_DATA *steak;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    int number, count;
    char *name;
    char *last_name;
    char arg1[MAX_STRING_LENGTH];
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];

    corpse = NULL;
    if ( (get_skill(ch,gsn_butcher) == 0)
	|| (ch->level < skill_table[gsn_butcher].skill_level[ch->Class()->GetIndex()]) )
    {
	send_to_char("But you don't know how to butcher.\n\r",ch);
	return;
    }

    one_argument(argument,arg1);
    number = number_argument(argument,arg1);
    count = 0;
    for (find_corpse = ch->in_room->contents;  find_corpse != NULL; find_corpse = find_corpse->next_content)
    {
	if ((can_see_obj(ch,find_corpse) )
    	    && ( arg1[0] == '\0' || is_name(arg1,find_corpse->name)) )
       {
	if (++count == number )
	corpse = find_corpse;
       }
    }

    if (corpse == NULL)
    {
	send_to_char("You don't have that.\n\r",ch);
	return;
    }
    if ((corpse->item_type != ITEM_CORPSE_NPC) && (corpse->item_type != ITEM_CORPSE_PC))
    {
 	send_to_char("You can only butcher a corpse.\n\r",ch);
	return;
    }

   if (number_percent() > ch->pcdata->learned[gsn_butcher])
   {
	act("You fail and destroy $p.",ch,corpse,NULL,TO_CHAR);
	act("$n tries to butcher a corpse but fails and destroys it.",ch,NULL,NULL,TO_ROOM);
	extract_obj(corpse);
	return;
    }

    act("$n butchers $p.",ch,corpse,0,TO_ROOM);
    act("You butcher $p.",ch,corpse,0,TO_CHAR);

    name = corpse->short_descr;
    for (obj = corpse->contains; obj != NULL; obj = obj_next)
    {
	obj_next = obj->next_content;
	obj_from_obj(obj);
	obj_to_room(obj,ch->in_room);
    }

    steak = create_object(get_obj_index(OBJ_VNUM_STEAK),1);
    steak->level = corpse->level;

    last_name = name;
    last_name = one_argument(last_name,name);
    last_name = one_argument(last_name,name);
    last_name = one_argument(last_name,name);
    name = last_name;
    sprintf(buf1, "a steak of %s",name);
    sprintf(buf2, "a steak of %s is here.",name);
    free_pstring(steak->short_descr);
    free_pstring(steak->description);
    steak->short_descr = palloc_string(buf1);
    steak->description = palloc_string(buf2);

    obj_to_room(steak,ch->in_room);
    extract_obj(corpse);
    return;
}


/* for poisoning weapons and food/drink */
void do_envenom(CHAR_DATA *ch, char *argument)
{
    OBJ_DATA *obj;
    OBJ_AFFECT_DATA oaf;
    int percent,skill;

    /* find out what */
    if (argument == '\0')
    {
	send_to_char("Envenom what item?\n\r",ch);
	return;
    }

    obj =  get_obj_list(ch,argument,ch->carrying);

    if (obj== NULL)
    {
	send_to_char("You don't have that item.\n\r",ch);
	return;
    }

    if ((skill = get_skill(ch,gsn_envenom)) < 1)
    {
	send_to_char("Are you crazy? You'd poison yourself!\n\r",ch);
	return;
    }

    if (obj->item_type == ITEM_FOOD || obj->item_type == ITEM_DRINK_CON)
    {
	if (IS_OBJ_STAT(obj,ITEM_BLESS) || IS_OBJ_STAT(obj,ITEM_BURN_PROOF))
	{
	    act("You fail to poison $p.",ch,obj,NULL,TO_CHAR);
	    return;
	}

	if (number_percent() < skill)  /* success! */
	{
	    act("$n treats $p with deadly poison.",ch,obj,NULL,TO_ROOM);
	    act("You treat $p with deadly poison.",ch,obj,NULL,TO_CHAR);
	    if (!obj->value[3])
	    {
		obj->value[3] = 1;
		check_improve(ch,gsn_envenom,TRUE,4);
	    }
	    WAIT_STATE(ch,skill_table[gsn_envenom].beats);
	    return;
	}

	act("You fail to poison $p.",ch,obj,NULL,TO_CHAR);
	if (!obj->value[3])
	    check_improve(ch,gsn_envenom,FALSE,4);
	WAIT_STATE(ch,skill_table[gsn_envenom].beats);
	return;
     }

    if (obj->item_type == ITEM_WEAPON)
    {
        if (IS_WEAPON_STAT(obj,WEAPON_FLAMING)
        ||  IS_WEAPON_STAT(obj,WEAPON_FROST)
        ||  IS_WEAPON_STAT(obj,WEAPON_VAMPIRIC)
        ||  IS_WEAPON_STAT(obj,WEAPON_SHARP)
        ||  IS_WEAPON_STAT(obj,WEAPON_VORPAL)
        ||  IS_WEAPON_STAT(obj,WEAPON_SHOCKING)
        ||  IS_OBJ_STAT(obj,ITEM_BLESS) || IS_OBJ_STAT(obj,ITEM_BURN_PROOF))
        {
            act("You can't seem to envenom $p.",ch,obj,NULL,TO_CHAR);
            return;
        }

	if (obj->value[3] < 0
	||  attack_table[obj->value[3]].damage == DAM_BASH)
	{
	    send_to_char("You can only envenom edged weapons.\n\r",ch);
	    return;
	}

        if (IS_WEAPON_STAT(obj,WEAPON_POISON))
        {
            act("$p is already envenomed.",ch,obj,NULL,TO_CHAR);
            return;
        }

	percent = number_percent();
	if (percent < skill)
	{

            init_affect_obj(&oaf);
            oaf.where     = TO_WEAPON;
			oaf.aftype = AFT_SKILL;
            oaf.type      = gsn_poison;
            oaf.level     = ch->level * percent / 100;
            oaf.duration  = ch->level/2 * percent / 100;
            oaf.location  = 0;
            oaf.modifier  = 0;
            SET_BIT(oaf.bitvector, WEAPON_POISON);
            affect_to_obj(obj,&oaf);

            act("$n coats $p with deadly venom.",ch,obj,NULL,TO_ROOM);
	    act("You coat $p with venom.",ch,obj,NULL,TO_CHAR);
	    check_improve(ch,gsn_envenom,TRUE,3);
	    WAIT_STATE(ch,skill_table[gsn_envenom].beats);
            return;
        }
	else
	{
	    act("You fail to envenom $p.",ch,obj,NULL,TO_CHAR);
	    check_improve(ch,gsn_envenom,FALSE,3);
	    WAIT_STATE(ch,skill_table[gsn_envenom].beats);
	    return;
	}
    }

    act("You can't poison $p.",ch,obj,NULL,TO_CHAR);
    return;
}

void do_fill( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *fountain;
    bool found;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Fill what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg, ch ) ) == NULL )
    {
	send_to_char( "You do not have that item.\n\r", ch );
	return;
    }

    found = FALSE;
    for ( fountain = ch->in_room->contents; fountain != NULL;
	fountain = fountain->next_content )
    {
	if ( fountain->item_type == ITEM_FOUNTAIN )
	{
	    found = TRUE;
	    break;
	}
    }

    if ( !found )
    {
	send_to_char( "There is no fountain here!\n\r", ch );
	return;
    }

    if ( obj->item_type != ITEM_DRINK_CON )
    {
	send_to_char( "You can't fill that.\n\r", ch );
	return;
    }

    if ( obj->value[1] != 0 && obj->value[2] != fountain->value[2] )
    {
	send_to_char( "There is already another liquid in it.\n\r", ch );
	return;
    }

    if ( obj->value[1] >= obj->value[0] )
    {
	send_to_char( "Your container is full.\n\r", ch );
	return;
    }

    sprintf(buf,"You fill $p with %s from $P.",
	liq_table[fountain->value[2]].liq_name);
    act( buf, ch, obj,fountain, TO_CHAR );
    sprintf(buf,"$n fills $p with %s from $P.",
	liq_table[fountain->value[2]].liq_name);
    act(buf,ch,obj,fountain,TO_ROOM);
    obj->value[2] = fountain->value[2];
    obj->value[1] = obj->value[0];
    return;
}

/* new pour - dio */
void do_pour(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_STRING_LENGTH],buf[MAX_STRING_LENGTH];
	OBJ_DATA *out, *on, *puddle;
	OBJ_AFFECT_DATA oaf;
	int amount;

	argument = one_argument(argument,arg);

	if (arg[0] == '\0')
	{
		send_to_char("Pour what on what?\n\r",ch);
		return;
	}


	if ((out = get_obj_carry(ch,arg, ch)) == NULL)
	{
		send_to_char("You don't have that item.\n\r",ch);
		return;
	}

	if (out->item_type != ITEM_DRINK_CON)
	{
		send_to_char("That's not a drink container.\n\r",ch);
		return;
	}

	if (out->value[1] == 0)
	{
		send_to_char("It's already empty.\n\r",ch);
		return;
	}

	if (argument[0] == '\0')
	{
		out->value[1] = 0;
		out->value[3] = 0;
		sprintf(buf,"You invert $p, spilling %s all over the ground.", liq_table[out->value[2]].liq_name);
		act(buf,ch,out,NULL,TO_CHAR);
	
		sprintf(buf,"$n inverts $p, spilling %s all over the ground.", liq_table[out->value[2]].liq_name);
		act(buf,ch,out,NULL,TO_ROOM);
		if (out->value[2] != LIQ_WATER) {
			sprintf(buf, "%s evaporates rapidly and is gone.",liq_table[out->value[2]].liq_name);
			act(buf,ch,0,0,TO_ALL);
		} else if (IS_GROUND(ch->in_room)) {
			puddle = create_object(get_obj_index(OBJ_VNUM_PUDDLE),ch->level);
			obj_to_room(puddle,ch->in_room);
			
			init_affect_obj(&oaf);
			oaf.where		=	TO_OBJ_AFFECTS;
			oaf.aftype		=	AFT_SPELL;
			oaf.type		=	gsn_puddle;
			oaf.location	=	0;
			oaf.modifier	=	0;
			oaf.duration	=	4;
			oaf.owner		=	ch;
			oaf.end_fun		=	puddle_evaporate;
			oaf.tick_fun	=	0;
			affect_to_obj(puddle,&oaf);
			
			sprintf(buf, "The water forms a puddle on the ground.");
			act(buf,ch,0,0,TO_ALL);
		}

		return;
	}

	if ((on = get_obj_here(ch,argument)) == NULL)
	{
		send_to_char("You don't see that here.\n\r",ch);
 		return;
	}

	if (out == on)
	{
		send_to_char("You cannot pour that on itself.\n\r",ch);
 		return;
	}

	amount = liq_table[out->value[2]].liq_affect[4] * 3;
	amount = UMIN(amount, out->value[1]);
	out->value[1] -= amount;

    	sprintf(buf,"You pour %s from $p onto $P.",liq_table[out->value[2]].liq_name);
    	act(buf,ch,out,on,TO_CHAR);
    	sprintf(buf,"$n pours %s from $p onto $P.",liq_table[out->value[2]].liq_name);
    	act(buf,ch,out,on,TO_ROOM);

	if (IS_AFFECTED(on,AFF_OBJ_BURNING)) {
		act("$p fizzles and is extinguished.",ch,on,0,TO_ALL);
		if (is_affected_obj(on,gsn_immolate))
			affect_strip_obj(on,gsn_immolate);
	}
	return;
}

void do_drink( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    int amount;
    int liquid;

    one_argument( argument, arg );

    if(is_affected(ch,gsn_uppercut))
	return send_to_char("Your jaw hurts too much to move.\n\r",ch);

    if ( arg[0] == '\0' )
    {
	for ( obj = ch->in_room->contents; obj; obj = obj->next_content )
	{
	    if ( obj->item_type == ITEM_FOUNTAIN )
		break;
	}

	if ( obj == NULL )
	{
	    send_to_char( "Drink what?\n\r", ch );
	    return;
	}
    }
    else
    {
	if ( ( obj = get_obj_here( ch, arg ) ) == NULL )
	{
	    send_to_char( "You can't find it.\n\r", ch );
	    return;
	}
    }

    if (ch->fighting != NULL)
    {
	send_to_char("You're too busy fighting to drink anything.\n\r", ch);
	return;
    }

    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK] > 20 )
    {
	send_to_char( "You fail to reach your mouth.  *Hic*.\n\r", ch );
	return;
    }

    switch ( obj->item_type )
    {
    default:
	send_to_char( "You can't drink from that.\n\r", ch );
	return;

    case ITEM_FOUNTAIN:
        if ( ( liquid = obj->value[2] )  < 0 )
        {
	    sprintf(buf,"Do_drink: bad liquid number, %d, on %s.",
	liquid, obj->short_descr);
		bug(buf,0);
/*
            bug( "Do_drink: bad liquid number %d.", liquid );
*/
            liquid = obj->value[2] = 0;
        }
	amount = liq_table[liquid].liq_affect[4] * 3;
	break;

    case ITEM_DRINK_CON:
	if ( obj->value[1] <= 0 )
	{
	    send_to_char( "It is already empty.\n\r", ch );
	    return;
	}

	if ( ( liquid = obj->value[2] )  < 0 )
	{
	    sprintf(buf,"Do_drink: bad liquid number, %d, on %s.",
	liquid, obj->short_descr);
		bug(buf,0);
/*
	    bug( "Do_drink: bad liquid number %d.", liquid );
*/
	    liquid = obj->value[2] = 0;
	}

        amount = liq_table[liquid].liq_affect[4];
        amount = UMIN(amount, obj->value[1]);
	break;
     }
    act( "$n drinks $T from $p.",
	ch, obj, liq_table[liquid].liq_name, TO_ROOM );
    act( "You drink $T from $p.",
	ch, obj, liq_table[liquid].liq_name, TO_CHAR );

    gain_condition( ch, COND_DRUNK,
	amount * liq_table[liquid].liq_affect[COND_DRUNK] / 36 );
    gain_condition( ch, COND_THIRST,
	-(amount * liq_table[liquid].liq_affect[COND_THIRST] / 3) );
    gain_condition(ch, COND_HUNGER,
	-(amount * liq_table[liquid].liq_affect[COND_HUNGER] / 3) );

    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK]  > 10 )
	send_to_char( "You feel drunk.\n\r", ch );
    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_THIRST] < 5 )
	send_to_char( "Your thirst is quenched.\n\r", ch );
	
    if ( obj->value[3] != 0 )
    {
	/* The drink was poisoned ! */
	AFFECT_DATA af;

	act( "$n chokes and gags.", ch, NULL, NULL, TO_ROOM );
	send_to_char( "You choke and gag.\n\r", ch );

	if (!is_affected(ch,gsn_poison)) {
		init_affect(&af);
		af.where     = TO_AFFECTS;
		af.aftype    = AFT_MALADY;
		af.type      = gsn_poison;
		af.level	 = number_fuzzy(amount);
		af.duration  = amount;
		af.location  = APPLY_STR;
		af.modifier  = -5;
		SET_BIT(af.bitvector, AFF_POISON);
		af.owner	 = ch;
		af.tick_fun	 = poison_tick;
		affect_join( ch, &af );
	}
    }
	
    if (obj->value[0] > 0)
        obj->value[1] -= amount;

    return;
}

void do_eat( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Eat what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg, ch ) ) == NULL )
    {
	send_to_char( "You do not have that item.\n\r", ch );
	return;
    }

    if (ch->fighting != NULL)
    {
	send_to_char( "You're too busy fighting to worry about food.\n\r", ch);
	return;
    }

    if ( !IS_IMMORTAL(ch) )
    {
	if ( obj->item_type != ITEM_FOOD && obj->item_type != ITEM_PILL )
	{
	    send_to_char( "That's not edible.\n\r", ch );
	    return;
	}
	    	
	if ( !IS_NPC(ch) && ch->pcdata->condition[COND_HUNGER] < 5 )
	{
	    send_to_char( "You are too full to eat more.\n\r", ch );
	    return;
	}

    }

	act( "$n eats $p.",  ch, obj, NULL, TO_ROOM );
        act( "You eat $p.", ch, obj, NULL, TO_CHAR );

    switch ( obj->item_type )
    {

    case ITEM_FOOD:
	if ( !IS_NPC(ch) )
	{
	    int condition;

	    condition = ch->pcdata->condition[COND_HUNGER];
	    gain_condition( ch, COND_HUNGER, -(obj->value[1]*3));
	    if (ch->pcdata->condition[COND_HUNGER] < COND_HUNGRY && condition > COND_HUNGRY)
		send_to_char( "You are no longer hungry.\n\r", ch );
	    else if ( ch->pcdata->condition[COND_HUNGER] < 5 )
		send_to_char( "You are full.\n\r", ch );
	}

	if ( obj->value[3] != 0 )
	{
	    /* The food was poisoned! */
	    AFFECT_DATA af;

	    act( "$n chokes and gags.", ch, 0, 0, TO_ROOM );
	    send_to_char( "You choke and gag.\n\r", ch );

		if (!is_affected(ch,gsn_poison)) {
		    init_affect(&af);
		    af.where	 = TO_AFFECTS;
			af.aftype	 = AFT_MALADY;
	 	   	af.type      = gsn_poison;
	   	 	af.level 	 = number_fuzzy(obj->value[0]);
	   	 	af.duration  = 2 * obj->value[0];
	   	 	af.location  = APPLY_STR;
	   	 	af.modifier  = -5;
	   	 	SET_BIT(af.bitvector, AFF_POISON);
			af.owner	 = ch;
			af.tick_fun	 = poison_tick;
	   	 	affect_join( ch, &af );
		}
	}
	break;

    case ITEM_PILL:
	obj_cast_spell( obj->value[1], obj->value[0], ch, ch, NULL );
	obj_cast_spell( obj->value[2], obj->value[0], ch, ch, NULL );
	obj_cast_spell( obj->value[3], obj->value[0], ch, ch, NULL );
	break;
    }

    extract_obj( obj );
    return;
}



/*
 * Remove an object.
 */
bool remove_obj( CHAR_DATA *ch, int iWear, bool fReplace )
{
    OBJ_DATA *obj, *secondary, *revealed;

	if (iWear == WEAR_COSMETIC || ( obj = get_eq_char( ch, iWear ) ) == NULL )
	return TRUE;

    if ( !fReplace )
	return FALSE;

    if ( IS_SET(obj->extra_flags, ITEM_NOREMOVE) ) {
		act( "You can't remove $p.", ch, obj, NULL, TO_CHAR );
		if (!IS_IMMORTAL(ch))
			return FALSE;
    }

	if (is_affected_obj(obj,gsn_rust)) {
		act ("$p is covered with rust and can't be pried off!",ch,obj,0,TO_CHAR);
		return FALSE;
	}
	
	if ( iWear == WEAR_WIELD && is_affected(ch,gsn_unbreakable)) {
		act( "$p is frozen to your hand!",ch,obj,NULL,TO_CHAR);
		if (!IS_IMMORTAL(ch))
			return FALSE;
	}
	
	if (check_entwine(ch,1) && (iWear == WEAR_DUAL_WIELD || iWear == WEAR_SHIELD || iWear == WEAR_HOLD))
	{
		act( "You can't remove $p.", ch, obj, NULL, TO_CHAR );
		return FALSE;
	}

	if (check_entwine(ch,0) && (iWear == WEAR_WIELD))
	{
		act( "You can't remove $p.", ch, obj, NULL, TO_CHAR );
		return FALSE;
	}

	secondary = get_eq_char(ch,WEAR_DUAL_WIELD);

	unequip_char( ch, obj, TRUE);
	
    if (iWear == WEAR_DUAL_WIELD) {
		act("$n stops dual wielding $p.",ch,obj,0,TO_ROOM);
		act("You stop dual wielding $p.",ch,obj,0,TO_CHAR);
		return TRUE;
	}
	for(revealed = ch->carrying; revealed; revealed = revealed->next_content)
		if(is_worn(revealed) && CAN_WEAR(revealed, ITEM_WEAR_COSMETIC))
			if(IS_SET(revealed->extra_flags, ITEM_UNDER_CLOTHES))
				if(revealed->wear_flags[0] - pow(2, ITEM_WEAR_COSMETIC) == obj->wear_flags[0])
					break;
	if(revealed)
	{
		act("$n removes $p, revealing $P underneath.",ch,obj,revealed,TO_ROOM);
		act("You remove $p, revealing $P underneath.",ch,obj,revealed,TO_CHAR);
	}
	else
	{
 	   	act( "$n removes $p.", ch, obj, NULL, TO_ROOM );
    	act( "You remove $p.", ch, obj, NULL, TO_CHAR );
	}
    if (iWear == WEAR_WIELD && secondary != NULL) {
		unequip_char(ch,secondary,FALSE);
		act("You swap $p into your primary hand.",ch,secondary,NULL,TO_CHAR);
		act("$n swaps $p into $s primary hand.",ch,secondary,NULL,TO_ROOM);
		equip_char(ch,secondary,WEAR_WIELD,FALSE);
	}

    return TRUE;
}



/*
 * Wear one object.
 * Optional replacement of existing objects.
 * Big repetitive code, ick.
 */
void wear_obj( CHAR_DATA *ch, OBJ_DATA *obj, bool fReplace )
{
	char buf[MSL];
    OBJ_DATA *secondary;
    bool wield_primary;
    OBJ_DATA *primary;
    OBJ_DATA *weapon;
    OBJ_DATA *oldobj;
    int sn, skill;
	OBJ_AFFECT_DATA *paf;

	
    if(CAN_WEAR(obj, ITEM_WEAR_COSMETIC))
    {
	OBJ_DATA *pObj;
        int i = 0;
        for(pObj = ch->carrying; pObj; pObj = pObj->next_content)
                if(pObj->wear_loc == WEAR_COSMETIC)
                    i++;
        if(i>=5 && !IS_IMMORTAL(ch))
            return send_to_char("You are already wearing as many ornamental items as you can.\n\r",ch);
        act("$n wears $p.",ch,obj,NULL,TO_ROOM);
        act("You wear $p.",ch,obj,NULL,TO_CHAR);
        equip_char(ch,obj,WEAR_COSMETIC,FALSE);
        return;
    }	
    if ( CAN_WEAR( obj, ITEM_WEAR_FINGER ) )
    {
	if ( get_eq_char( ch, WEAR_FINGER_L ) != NULL
	&&   get_eq_char( ch, WEAR_FINGER_R ) != NULL
	&&   !remove_obj( ch, WEAR_FINGER_L, fReplace )
	&&   !remove_obj( ch, WEAR_FINGER_R, fReplace ) )
	    return;

	if ( get_eq_char( ch, WEAR_FINGER_L ) == NULL )
	{
	    act( "$n wears $p on $s left finger.",    ch, obj, NULL, TO_ROOM );
	    act( "You wear $p on your left finger.",  ch, obj, NULL, TO_CHAR );
	    equip_char( ch, obj, WEAR_FINGER_L, TRUE);
	    return;
	}

	if ( get_eq_char( ch, WEAR_FINGER_R ) == NULL )
	{
	    act( "$n wears $p on $s right finger.",   ch, obj, NULL, TO_ROOM );
	    act( "You wear $p on your right finger.", ch, obj, NULL, TO_CHAR );
	    equip_char( ch, obj, WEAR_FINGER_R, TRUE);
	    return;
	}

	return send_to_char( "You are already wearing two rings.\n\r", ch );
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_NECK ) )
    {
	if ( get_eq_char( ch, WEAR_NECK_1 ) != NULL
	&&   get_eq_char( ch, WEAR_NECK_2 ) != NULL
	&&   !remove_obj( ch, WEAR_NECK_1, fReplace )
	&&   !remove_obj( ch, WEAR_NECK_2, fReplace ) )
	    return;

	if ( get_eq_char( ch, WEAR_NECK_1 ) == NULL )
	{
	    act( "$n ducks $s head and slips $p around $s neck.",   ch, obj, NULL, TO_ROOM );
	    act( "You duck your head and slip $p around your neck.", ch, obj, NULL, TO_CHAR );
	    equip_char( ch, obj, WEAR_NECK_1, TRUE);
	    return;
	}

	if ( get_eq_char( ch, WEAR_NECK_2 ) == NULL )
	{
	    act( "$n ducks $s head and slips $p around $s neck.",   ch, obj, NULL, TO_ROOM );
	    act( "You duck your head and slip $p around your neck.", ch, obj, NULL, TO_CHAR );
	    equip_char( ch, obj, WEAR_NECK_2, TRUE );
	    return;
	}

	bug( "Wear_obj: no free neck.", 0 );
	send_to_char( "You already wear two neck items.\n\r", ch );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_BODY ) )
    {
	if ( !remove_obj( ch, WEAR_BODY, fReplace ) )
	    return;
	act( "$n shrugs into $p.",   ch, obj, NULL, TO_ROOM );
	act( "You shrug into $p.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_BODY, TRUE );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_HEAD ) )
    {

	if (!str_cmp(race_table[ch->race].name,"minotaur") && !IS_SET(obj->pIndexData->restrict_flags, MINOTAUR_ONLY)) {
		sprintf(buf,"Your horns prevent you from donning %s.\n\r",obj->short_descr);
		return send_to_char(buf,ch);
	}
		
	oldobj =(get_eq_char(ch, WEAR_HEAD));
	if ( !remove_obj( ch, WEAR_HEAD, fReplace ) )
	    return;
	 	
	act( "$n dons $p.",   ch, obj, NULL, TO_ROOM );
	act( "You don $p.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_HEAD, TRUE );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_LEGS ) )
    {
	if ( !remove_obj( ch, WEAR_LEGS, fReplace ) )
	    return;
	act( "$n slips $s legs into $p.",   ch, obj, NULL, TO_ROOM );
	act( "You slip your legs into $p.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_LEGS, TRUE );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_FEET ) )
    {

		if (!str_cmp(race_table[ch->race].name,"minotaur") && !IS_SET(obj->pIndexData->restrict_flags, MINOTAUR_ONLY)) {
			sprintf(buf,"Your hooves do not allow you to wear %s.\n\r",obj->short_descr);
			return send_to_char(buf,ch);
		}
		
		
	if ( !remove_obj( ch, WEAR_FEET, fReplace ) )
	    return;
	act( "$n places $p on $s feet.",   ch, obj, NULL, TO_ROOM );
	act( "You place $p on your feet.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_FEET, TRUE );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_HANDS ) )
    {
	if ( !remove_obj( ch, WEAR_HANDS, fReplace ) )
	    return;
	act( "$n slips $p onto $s hands.",   ch, obj, NULL, TO_ROOM );
	act( "You slip $p onto your hands.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_HANDS, TRUE );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_ARMS ) )
    {
	if ( !remove_obj( ch, WEAR_ARMS, fReplace ) )
	    return;
	act( "$n slides $p onto $s arms.",   ch, obj, NULL, TO_ROOM );
	act( "You slide $p onto your arms.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_ARMS, TRUE );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_ABOUT ) )
    {
	if ( !remove_obj( ch, WEAR_ABOUT, fReplace ) )
	    return;
	act( "$n wraps $p about $s body.",   ch, obj, NULL, TO_ROOM );
	act( "You wrap $p about your body.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_ABOUT, TRUE );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_WAIST ) )
    {
	if ( !remove_obj( ch, WEAR_WAIST, fReplace ) )
	    return;
	act( "$n clasps $p about $s waist.",   ch, obj, NULL, TO_ROOM );
	act( "You clasp $p about your waist.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_WAIST, TRUE );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_WRIST ) )
    {
	if ( get_eq_char( ch, WEAR_WRIST_L ) != NULL
	&&   get_eq_char( ch, WEAR_WRIST_R ) != NULL
	&&   !remove_obj( ch, WEAR_WRIST_L, fReplace )
	&&   !remove_obj( ch, WEAR_WRIST_R, fReplace ) )
	    return;

	if ( get_eq_char( ch, WEAR_WRIST_L ) == NULL )
	{
	    act( "$n slips $p onto $s left wrist.",
		ch, obj, NULL, TO_ROOM );
	    act( "You slip $p onto your left wrist.",
		ch, obj, NULL, TO_CHAR );
	    equip_char( ch, obj, WEAR_WRIST_L, TRUE );
	    return;
	}

	if ( get_eq_char( ch, WEAR_WRIST_R ) == NULL )
	{
	    act( "$n slips $p onto $s right wrist.",
		ch, obj, NULL, TO_ROOM );
	    act( "You slip $p onto your right wrist.",
		ch, obj, NULL, TO_CHAR );
	    equip_char( ch, obj, WEAR_WRIST_R, TRUE);
	    return;
	}

	bug( "Wear_obj: no free wrist.", 0 );
	send_to_char( "You already wear two wrist items.\n\r", ch );
	return;
    }

/* ====== DUAL WIELD STUFF STARTS HERE ======= */
/* Dual wielding and limiting to 2 hands for objects */


    if (CAN_WEAR(obj,ITEM_WEAR_HOLD))
    {
	if ((get_eq_char(ch,WEAR_HOLD) == NULL) && (hands_full(ch)) )
	{
		if (!fReplace)
			return;
		else
		{
		 send_to_char("Your hands are full.\n\r",ch);
		 return;
 		}
	}
    if (!remove_obj(ch,WEAR_HOLD,fReplace))
	return;
    weapon = get_eq_char(ch,WEAR_WIELD);
    if (weapon != NULL && (ch->size < SIZE_LARGE
	&& (IS_WEAPON_STAT(weapon,WEAPON_TWO_HANDS)
	|| weapon->value[0]==WEAPON_STAFF
	|| weapon->value[0]==WEAPON_POLEARM
	|| weapon->value[0]==WEAPON_SPEAR)))
    {
	send_to_char("Your hands are tied up with your weapon.\n\r",ch);
	return;
    }

	if (obj->item_type == ITEM_POTION && ch->fighting != NULL) {
		send_to_char("You can't handle a potion in the heat of combat!\n\r",ch);
		return;
	}
	
    act("$n holds $p in $s hands.",ch,obj,0,TO_ROOM);
    act("You hold $p in your hands.",ch,obj,0,TO_CHAR);
    equip_char(ch,obj,WEAR_HOLD, TRUE);
    return;
    }

    if (CAN_WEAR(obj,ITEM_WEAR_SHIELD))
    {
	if ((get_eq_char(ch,WEAR_SHIELD) == NULL) && (hands_full(ch)) )
	{
		if (!fReplace)
			return;
		else
		{
		 send_to_char("Your hands are full.\n\r",ch);
		 return;
 		}
	}
    if (!remove_obj(ch,WEAR_SHIELD,fReplace))
	return;
    weapon = get_eq_char(ch,WEAR_WIELD);
    if (weapon != NULL && ((ch->size < SIZE_LARGE
	&& IS_WEAPON_STAT(weapon,WEAPON_TWO_HANDS))
	|| weapon->value[0]==WEAPON_STAFF
	|| weapon->value[0]==WEAPON_POLEARM
	|| weapon->value[0]==WEAPON_SPEAR))
    {
	send_to_char("Your hands are tied up with your weapon.\n\r",ch);
	return;
    }
    act("$n wears $p as a shield.",ch,obj,0,TO_ROOM);
    act("You wear $p as a shield.",ch,obj,0,TO_CHAR);
    equip_char(ch,obj,WEAR_SHIELD, TRUE);
    return;
    }

/* Lots of dual wield stuff now */
    if (obj->item_type == ITEM_LIGHT)
    {
	if ((get_eq_char(ch,WEAR_LIGHT) == NULL) && (hands_full(ch)) )
	{
		if (!fReplace)
			return;
		else
		{
		 send_to_char("Your hands are full.\n\r",ch);
		 return;
 		}
	}
    if (!remove_obj(ch,WEAR_LIGHT,fReplace))
	return;
    weapon = get_eq_char(ch,WEAR_WIELD);
    if (weapon != NULL && ch->size < SIZE_LARGE
	&& (IS_WEAPON_STAT(weapon,WEAPON_TWO_HANDS)
	|| weapon->value[0]==WEAPON_STAFF
	|| weapon->value[0]==WEAPON_POLEARM
	|| weapon->value[0]==WEAPON_SPEAR))
    {
	send_to_char("Your hands are tied up with your weapon.\n\r",ch);
	return;
    }
    act("$n lights $p and holds it.",ch,obj,0,TO_ROOM);
    act("You light $p and hold it.",ch,obj,0,TO_CHAR);
    equip_char(ch,obj,WEAR_LIGHT, TRUE);
    return;
    }


    if (CAN_WEAR(obj,ITEM_WEAR_WIELD))
    {
	wield_primary = TRUE;
	primary = get_eq_char(ch,WEAR_WIELD);
	secondary = get_eq_char(ch,WEAR_DUAL_WIELD);

	if (primary == NULL && (hands_full(ch)) )
	{
		if (!fReplace)
			return;
		else
		{
		 send_to_char("Your hands are full.\n\r",ch);
		 return;
		}

	}	
	if (get_eq_char(ch, WEAR_DUAL_WIELD) != NULL)
		wield_primary = FALSE;

	if (primary != NULL && (!hands_full(ch)) )
		wield_primary = FALSE;
	if (get_skill(ch,gsn_dual_wield) < 3 && (!IS_NPC(ch)))
		wield_primary = TRUE;
	if (ch->level < skill_table[gsn_dual_wield].skill_level[ch->Class()->GetIndex()]
	&& !IS_NPC(ch))
		wield_primary = TRUE;
	if (ch->size < SIZE_LARGE && IS_WEAPON_STAT(obj,WEAPON_TWO_HANDS))
		wield_primary = TRUE;
	if (primary != NULL && ch->size < SIZE_LARGE
	&& IS_WEAPON_STAT(obj,WEAPON_TWO_HANDS))
		wield_primary = TRUE;
	if (primary != NULL && ch->size < SIZE_LARGE
	&& IS_WEAPON_STAT(primary,WEAPON_TWO_HANDS))
		wield_primary = TRUE;
	if (obj != NULL && (obj->value[0]==WEAPON_SPEAR ||
		obj->value[0]==WEAPON_STAFF ||
		obj->value[0]==WEAPON_POLEARM))
		wield_primary = TRUE;
	if (primary != NULL && (primary->value[0]==WEAPON_SPEAR ||
		primary->value[0]==WEAPON_STAFF ||
		primary->value[0]==WEAPON_POLEARM))
		wield_primary = TRUE;
/* First attempt to dual wield if !primary. */

    if (!wield_primary)
    {
	if (secondary == NULL)
	 {
	   if (obj->weight <= primary->weight
	    || obj->weight < 5)
	   {
		act("$n wields $p in $s offhand.",ch,obj,0,TO_ROOM);
		act("You wield $p in your offhand.",ch,obj,0,TO_CHAR);
		equip_char(ch,obj,WEAR_DUAL_WIELD, TRUE);

	switch(obj->value[0])
	{
        default :               sn = -1;                break;
        case(WEAPON_SWORD):     sn = gsn_sword;         break;
        case(WEAPON_DAGGER):    sn = gsn_dagger;        break;
        case(WEAPON_SPEAR):     sn = gsn_spear;         break;
        case(WEAPON_MACE):      sn = gsn_mace;          break;
        case(WEAPON_AXE):       sn = gsn_axe;           break;
        case(WEAPON_FLAIL):     sn = gsn_flail;         break;
        case(WEAPON_WHIP):      sn = gsn_whip;          break;
        case(WEAPON_POLEARM):   sn = gsn_polearm;       break;
	case(WEAPON_STAFF):	sn = gsn_staff;	break;
	}
	skill = get_weapon_skill(ch,sn);
	if (skill >= 100)
		act("$p feels like a part of you!",ch,obj,0,TO_CHAR);
	else if (skill > 85)
		act("You feel quite confident with $p.",ch,obj,0,TO_CHAR);
	else if (skill > 70)
		act("You are skilled with $p.",ch,obj,0,TO_CHAR);
	else if (skill > 50)
		act("Your skill with $p is adequate.",ch,obj,0,TO_CHAR);
	else if (skill > 25)
		act("$p feels a little clumsy in your hands.",ch,obj,0,TO_CHAR);
	else if (skill > 1)
		act("You fumble and almost drop $p.",ch,obj,0,TO_CHAR);
	else
		act("You don't even know which end is up on $p.",ch,obj,0,TO_CHAR);

		return;
	   }
	  }
	else if (obj->weight < secondary->weight)
	{
	   if (!remove_obj(ch,WEAR_DUAL_WIELD,fReplace))
		return;
	    act("$n wields $p in $s offhand.",ch,obj,0,TO_ROOM);
	    act("You wield $p in your offhand.",ch,obj,0,TO_CHAR);
	    equip_char(ch,obj,WEAR_DUAL_WIELD, TRUE);

	switch(obj->value[0])
	{
        default :               sn = -1;                break;
        case(WEAPON_SWORD):     sn = gsn_sword;         break;
        case(WEAPON_DAGGER):    sn = gsn_dagger;        break;
        case(WEAPON_SPEAR):     sn = gsn_spear;         break;
        case(WEAPON_MACE):      sn = gsn_mace;          break;
        case(WEAPON_AXE):       sn = gsn_axe;           break;
        case(WEAPON_FLAIL):     sn = gsn_flail;         break;
        case(WEAPON_WHIP):      sn = gsn_whip;          break;
        case(WEAPON_POLEARM):   sn = gsn_polearm;       break;
	case (WEAPON_STAFF):	sn = gsn_staff;	break;
	}
	
	skill = get_weapon_skill(ch,sn);
	if (skill >= 100)
		act("$p feels like a part of you!",ch,obj,0,TO_CHAR);
	else if (skill > 85)
		act("You feel quite confident with $p.",ch,obj,0,TO_CHAR);
	else if (skill > 70)
		act("You are skilled with $p.",ch,obj,0,TO_CHAR);
	else if (skill > 50)
		act("Your skill with $p is adequate.",ch,obj,0,TO_CHAR);
	else if (skill > 25)
		act("$p feels a little clumsy in your hands.",ch,obj,0,TO_CHAR);
	else if (skill > 1)
		act("You fumble and almost drop $p.",ch,obj,0,TO_CHAR);
	else
		act("You don't even know which end is up on $p.",ch,obj,0,TO_CHAR);

	    return;
	}
	else
	{
	  if (!remove_obj(ch,WEAR_WIELD,fReplace))
		return;

	    if (ch->size < SIZE_LARGE
		&& (IS_WEAPON_STAT(obj,WEAPON_TWO_HANDS)
		|| obj->value[0]==WEAPON_STAFF
		|| obj->value[0]==WEAPON_POLEARM
		|| obj->value[0]==WEAPON_SPEAR)
		&& ch->arms < 2)
	    {
		send_to_char("Your hands are full.\n\r",ch);
		return;
	    }

	  if (secondary->weight < 0.75 * obj->weight
	    || secondary->weight < 5)
	  {
  	    if (get_obj_weight(obj) > (str_app[get_curr_stat(ch,STAT_STR)].wield)
				&& !IS_IMMORTAL(ch))
		 {
		    send_to_char("It is too heavy for you to wield.\n\r",ch);
		    return;
		 }
		 act("$n wields $p.",ch,obj,0,TO_ROOM);
		 act("You wield $p.",ch,obj,0,TO_CHAR);
		 unequip_char(ch,secondary,TRUE);
		 if(secondary==NULL)
			return;
		 equip_char(ch,obj,WEAR_WIELD, TRUE);
		 equip_char(ch,secondary,WEAR_DUAL_WIELD,TRUE);

	sn = get_weapon_sn(ch);
	if (sn == gsn_hand_to_hand)
		return;
	skill = get_weapon_skill(ch,sn);
	if (skill >= 100)
		act("$p feels like a part of you!",ch,obj,0,TO_CHAR);
	else if (skill > 85)
		act("You feel quite confident with $p.",ch,obj,0,TO_CHAR);
	else if (skill > 70)
		act("You are skilled with $p.",ch,obj,0,TO_CHAR);
	else if (skill > 50)
		act("Your skill with $p is adequate.",ch,obj,0,TO_CHAR);
	else if (skill > 25)
		act("$p feels a little clumsy in your hands.",ch,obj,0,TO_CHAR);
	else if (skill > 1)
		act("You fumble and almost drop $p.",ch,obj,0,TO_CHAR);
	else
		act("You don't even know which end is up on $p.",ch,obj,0,TO_CHAR);

	 	 return;
	  }
		act("$p is too heavy to dual wield,",ch,obj,0,TO_CHAR);
		return;
       }
     }
		
/* If you can't wield the dual weapon then see if you can replace primary */
	 if (!remove_obj(ch,WEAR_WIELD,fReplace))
		return;
	 if (get_obj_weight(obj) > (str_app[get_curr_stat(ch,STAT_STR)].wield)
				 && !IS_IMMORTAL(ch))
	 {
	    send_to_char("It is too heavy for you to wield.\n\r",ch);
	    return;
	 }

	if ((ch->size < SIZE_LARGE && IS_WEAPON_STAT(obj,WEAPON_TWO_HANDS))
	    || obj->value[0]==WEAPON_SPEAR
	    || obj->value[0]==WEAPON_STAFF
	    || obj->value[0]==WEAPON_POLEARM)
	{	
	   if (get_eq_char(ch,WEAR_SHIELD) != NULL
	   || get_eq_char(ch,WEAR_HOLD) != NULL
	   || get_eq_char(ch,WEAR_LIGHT) != NULL
	   || get_eq_char(ch,WEAR_WIELD) != NULL)
	   {
	    send_to_char("You need both hands free for that weapon.\n\r",ch);
	    return;
	   }
	}
	 act("$n wields $p.",ch,obj,0,TO_ROOM);
	 act("You wield $p.",ch,obj,0,TO_CHAR);
	 equip_char(ch,obj,WEAR_WIELD, TRUE);

	sn = get_weapon_sn(ch);
	if (sn == gsn_hand_to_hand)
		return;
	skill = get_weapon_skill(ch,sn);
	if (skill >= 100)
		act("$p feels like a part of you!",ch,obj,0,TO_CHAR);
	else if (skill > 85)
		act("You feel quite confident with $p.",ch,obj,0,TO_CHAR);
	else if (skill > 70)
		act("You are skilled with $p.",ch,obj,0,TO_CHAR);
	else if (skill > 50)
		act("Your skill with $p is adequate.",ch,obj,0,TO_CHAR);
	else if (skill > 25)
		act("$p feels a little clumsy in your hands.",ch,obj,0,TO_CHAR);
	else if (skill > 1)
		act("You fumble and almost drop $p.",ch,obj,0,TO_CHAR);
	else
		act("You don't even know which end is up on $p.",ch,obj,0,TO_CHAR);

 	 return;
}

/* ==== END DUAL_WIELD CODE ======= */
    if ( CAN_WEAR(obj,ITEM_WEAR_BRAND) )
    {
	if (!remove_obj(ch,WEAR_BRAND, fReplace) )
	    return;
        act("$n wears $p.",ch,obj,NULL,TO_ROOM);
	act("You wear $p.",ch,obj,NULL,TO_CHAR);
	equip_char(ch,obj,WEAR_BRAND, TRUE);
	return;
    }
    if ( CAN_WEAR(obj,ITEM_WEAR_STRAPPED) )
    {
        if (!remove_obj(ch,WEAR_STRAPPED, fReplace) )
            return;
        act( "$n wears $p.",   ch, obj, NULL, TO_ROOM );
        act( "You wear $p.", ch, obj, NULL, TO_CHAR );
	equip_char(ch,obj,WEAR_STRAPPED, TRUE);
        return;
    }
	if(CAN_WEAR(obj, ITEM_WEAR_COSMETIC))
	{
		OBJ_DATA *pObj;
		int i = 0;
		for(pObj = ch->carrying; pObj; pObj = pObj->next_content)
				if(pObj->wear_loc == WEAR_COSMETIC)
					i++;
		if(i>=6)
		{
			send_to_char("You are already wearing as many ornamental items as you can.\n\r",ch);
			return;
		}
		act("$n wears $p.",ch,obj,NULL,TO_ROOM);
		act("You wear $p.",ch,obj,NULL,TO_CHAR);
		equip_char(ch,obj,WEAR_COSMETIC,FALSE);
		return;
	}
    if ( fReplace )
	send_to_char( "You can't wear, wield, or hold that.\n\r", ch );

    return;
}



void do_wear( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
		return send_to_char( "Wear, wield, or hold what?\n\r", ch );

    if ( IS_NPC(ch) && IS_AFFECTED(ch,AFF_CHARM))
    {
	send_to_char( "A mob wearing eq? Ha!\n\r", ch );
	return;
    }
    if ( !str_cmp( arg, "all" ) && is_affected(ch, gsn_rage))
	return send_to_char("You're too filled with rage to care about clothing!\n\r",ch);

    if ( !str_cmp( arg, "all" ) )
    {
	OBJ_DATA *obj_next;

	for ( obj = ch->carrying; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;
	    if (obj->wear_loc == WEAR_NONE && can_see_obj( ch, obj ) )
			wear_obj( ch, obj, FALSE );
	}
	return;
    }
    else
    {
	if ( ( obj = get_obj_carry( ch, arg, ch ) ) == NULL )
	{
	    send_to_char( "You do not have that item.\n\r", ch );
	    return;
	}
	if( is_affected(ch, gsn_rage) &&
		!IS_SET(obj->pIndexData->wear_flags, ITEM_WEAR_WIELD) &&
		!IS_SET(obj->pIndexData->wear_flags, ITEM_WEAR_SHIELD) &&
		!IS_SET(obj->pIndexData->wear_flags, ITEM_WEAR_HOLD))
		return send_to_char("You are too filled with rage to bother with equipment!\n\r",ch);
	wear_obj( ch, obj, TRUE );
    }
    return;
}



void do_remove( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
		return send_to_char( "Remove what?\n\r", ch );

    if ( !str_cmp( arg, "all" ) ) {
		OBJ_DATA *obj_next;

		for ( obj = ch->carrying; obj != NULL; obj = obj_next ) {
		    obj_next = obj->next_content;
				if ( obj->wear_loc != WEAR_NONE
       		  		&& can_see_obj( ch, obj )
       	   		    && obj->wear_loc != WEAR_BRAND )
					remove_obj( ch, obj->wear_loc, TRUE );
		}
		return;
    }
    else if ( ( obj = get_obj_wear( ch, arg ) ) == NULL )
    {
	send_to_char( "You do not have that item.\n\r", ch );
	return;
    }

	if (obj->wear_loc == WEAR_COSMETIC)
	{
		act( "$n stops using $p.", ch, obj, NULL, TO_ROOM );
		act( "You stop using $p.", ch, obj, NULL, TO_CHAR );
		unequip_char( ch, obj, TRUE);
		return;
	}
	
    if (obj->wear_loc == WEAR_BRAND && !IS_IMMORTAL(ch))
    {
	send_to_char( "You can't remove a tattoo!\n\r", ch );
	return;
    }

		
    remove_obj( ch, obj->wear_loc, TRUE );
    return;
}



void do_sacrifice( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *ibj;
    OBJ_DATA *ibj_next;
    CHAR_DATA *gch;
    one_argument( argument, arg );

	if (!IS_NPC(ch) && ch->pcdata->death_status == HAS_DIED)
		return send_to_char("You are a hovering spirit, you can't sacrifice anything!\n\r",ch);

    if ( arg[0] == '\0' || !str_cmp( arg, ch->name ) )
		return send_to_char("The gods appreciate your offer and may accept it later.\n\r", ch );

    obj = get_obj_list( ch, arg, ch->in_room->contents );

    if ( obj == NULL )
		return send_to_char( "You can't find it.\n\r", ch );

    if ( obj->item_type == ITEM_CORPSE_PC )
	   return send_to_char("The gods wouldn't like that.\n\r",ch);
	
    if ( !CAN_WEAR(obj, ITEM_TAKE) || IS_OBJ_STAT(obj, ITEM_NO_SAC))
		return act( "$p is not an acceptable sacrifice.", ch, obj, 0, TO_CHAR );

    if (obj->in_room != NULL)
		for (gch = obj->in_room->people; gch != NULL; gch = gch->next_in_room)
			if (gch->on == obj)
				return act("$N appears to be using $p.", ch,obj,gch,TO_CHAR);
		
    if (isCabalItem(obj)==TRUE)
		return act("You can't do that!",ch,obj,0,TO_CHAR);


    act("You sacrifice $p to the gods.",ch,obj,NULL,TO_CHAR);
    act( "$n sacrifices $p to the gods.", ch, obj, NULL, TO_ROOM );
    if (IS_SET(obj->progtypes,IPROG_SAC))
  	if ( (obj->pIndexData->iprogs->sac_prog) (obj,ch) )
   		return;

    if(TRAPS_IEVENT(obj,TRAP_ISAC))
	if( CALL_IEVENT(obj,TRAP_ISAC,ch,obj) )
		return;
    if ((obj->item_type == ITEM_CORPSE_NPC || obj->item_type == ITEM_CONTAINER) && obj->contains)
    {
            for (ibj = obj->contains ; ibj != NULL ; ibj = ibj_next)
            {
                ibj_next = ibj->next_content;
                obj_from_obj(ibj);
                obj_to_room(ibj, obj->in_room);
            }
    }
    wiznet("$N sends up $p as a burnt offering.",
	   ch,obj,WIZ_SACCING,0,0);
    extract_obj( obj );
    return;
}


void quaff_potion(void *arg1, void *arg2, void *arg3, void *arg4, void *arg5)
{
	CHAR_DATA *ch = (CHAR_DATA *) arg1;
	int sLevel = (int)arg2, sn1 = (int)arg3, sn2 = (int)arg4, sn3 = (int)arg5;
	send_to_char("You feel the unmistakable tingle of magic as the liquid flows through your body.\n\r",ch);
	obj_cast_spell(sn1, sLevel, ch, ch, NULL );
	obj_cast_spell(sn2, sLevel, ch, ch, NULL );
	obj_cast_spell(sn3, sLevel, ch, ch, NULL );
}
void do_quaff( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int nDelay = 0;
    one_argument( argument, arg );
    if(!IS_NPC(ch) && (ch->Class()->name == "necromancer" ||
		ch->Class()->name == "sorcerer" ||
		ch->Class()->name == "anti-paladin"))
	nDelay = 1;
    else
	nDelay = 2;

	/* Can quaff held potions, even in combat! -- Eladrian */
	
    if ( arg[0] == '\0' )
	{
		if ((obj = get_eq_char(ch,WEAR_HOLD)) != NULL)
		{
			if (obj->item_type == ITEM_POTION)
			{
				act("$n quaffs $p in one large gulp.",ch,obj,NULL,TO_ROOM);
				act("You quaff the contents of $p in one large gulp.",ch,obj,NULL,TO_CHAR);
				if(!check_deny_magic(ch))
				{
					RS.Queue.AddToQueue(nDelay, 5, (void *)quaff_potion,ch,(void*)obj->value[0],
						(void*)obj->value[1],(void*)obj->value[2],(void*)obj->value[3]);
				}
				extract_obj(obj);
				return;
			}
		}
		send_to_char( "Quaff what?\n\r", ch );
		return;
    }

    if ( ( obj = get_obj_carry( ch, arg, ch ) ) == NULL )
    {
	send_to_char( "You do not have that potion.\n\r", ch );
	return;
    }

    if ( obj->item_type != ITEM_POTION )
    {
	send_to_char( "You can quaff only potions.\n\r", ch );
	return;
    }

    if ( ch->fighting != NULL )
    {
	send_to_char( "You're too busy fighting to quaff.\n\r", ch );
	return;
    }
 
    act("$n quaffs $p in one large gulp.",ch,obj,NULL,TO_ROOM);
    act("You quaff the contents of $p in one large gulp.",ch,obj,NULL,TO_CHAR);
    if(!check_deny_magic(ch))
  	  RS.Queue.AddToQueue(nDelay, 5, (void *)quaff_potion,ch,(void*)obj->value[0],
                 (void*)obj->value[1],(void*)obj->value[2],(void*)obj->value[3]);
    extract_obj(obj);
    return;
}


void do_recite( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *scroll;
    OBJ_DATA *obj;
    int alangbonus = 0;
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( ( scroll = get_obj_carry( ch, arg1, ch ) ) == NULL )
    {
	send_to_char( "You do not have that scroll.\n\r", ch );
	return;
    }

    if ( scroll->item_type != ITEM_SCROLL )
    {
	send_to_char( "You can recite only scrolls.\n\r", ch );
	return;
    }

	if (scroll->pIndexData->vnum == OBJ_VNUM_SCRIBE && ch->cabal != CABAL_SCION && !IS_IMMORTAL(ch))
		return send_to_char("You fail to understand the arcane symbols.\n\r", ch);

    if ( ch->level < scroll->level)
    {
	send_to_char("This scroll is too complex for you to comprehend.\n\r",ch);
	return;
    }

    if (ch->fighting != NULL)
    {
	send_to_char( "You're too busy fighting to recite.\n\r", ch);
	return;
    }

    obj = NULL;
    if ( arg2[0] == '\0' )
		victim = ch;
    else
    {
	if ( ( victim = get_char_room ( ch, arg2 ) ) == NULL
	&&   ( obj    = get_obj_here  ( ch, arg2 ) ) == NULL )
		return send_to_char( "You can't find it.\n\r", ch );
    }

    if(!IS_NPC(ch) && (alangbonus = ch->Profs()->GetProf("ancient languages") > -1))
    {
	send_to_char("With your knowledge of ancient languages, you better understand the invocation of the scroll.\n\r",ch);
	ch->Profs()->CheckImprove("ancient languages", 100);
    }

    act( "$n recites $p.", ch, scroll, NULL, TO_ROOM );
    act( "You recite $p.", ch, scroll, NULL, TO_CHAR );

    if (number_percent() >= 15 + get_skill(ch,gsn_scrolls) * 4/5 + UMAX(0, alangbonus * 3))
    {
		send_to_char("You mispronounce a syllable.\n\r",ch);
		check_improve(ch,gsn_scrolls,FALSE,2);
    }
    else
    {
		if(!check_deny_magic(ch))
		{
			obj_cast_spell( scroll->value[1], scroll->value[0] + URANGE(0, alangbonus, 3), ch, victim, obj );
    			obj_cast_spell( scroll->value[2], scroll->value[0] + URANGE(0, alangbonus, 2), ch, victim, obj );
    			obj_cast_spell( scroll->value[3], scroll->value[0] + URANGE(0, alangbonus, 1), ch, victim, obj );
		}
		check_improve(ch,gsn_scrolls,TRUE,2);
    }

    extract_obj( scroll );
    return;
}



void do_brandish( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    OBJ_DATA *staff;
    int sn;


    if ( ( staff = get_eq_char( ch, WEAR_HOLD ) ) == NULL )
    {
	send_to_char( "You hold nothing in your hand.\n\r", ch );
	return;
    }

    if ( staff->item_type != ITEM_STAFF )
    {
	send_to_char( "You can brandish only with a staff.\n\r", ch );
	return;
    }

    if ( ( sn = staff->value[3] ) < 0
    ||   sn >= MAX_SKILL
    ||   skill_table[sn].spell_fun == 0 )
    {
	bug( "Do_brandish: bad sn %d.", sn );
	return;
    }

    WAIT_STATE( ch, 2 * PULSE_VIOLENCE );

    if ( staff->value[2] > 0 )
    {
	act( "$n brandishes $p.", ch, staff, NULL, TO_ROOM );
	act( "You brandish $p.",  ch, staff, NULL, TO_CHAR );
	if ( ch->level < staff->level
	||   number_percent() >= 20 + get_skill(ch,gsn_staves) * 4/5)
 	{
	    act ("You fail to invoke $p.",ch,staff,NULL,TO_CHAR);
	    act ("...and nothing happens.",ch,NULL,NULL,TO_ROOM);
	    check_improve(ch,gsn_staves,FALSE,2);
	}
	
	else if(!check_deny_magic(ch))
	{
		for ( vch = ch->in_room->people; vch; vch = vch_next )
		{
	    vch_next	= vch->next_in_room;

		if (vch->invis_level >= LEVEL_HERO)
			continue;

	    switch ( skill_table[sn].target )
	    {
	    	default:
				bug( "Do_brandish: bad target for sn %d.", sn );
				return;

	    	case TAR_IGNORE:
				if ( vch != ch )
		    		continue;
			break;

	    	case TAR_CHAR_OFFENSIVE:
		case TAR_OBJ_CHAR_OFF:
				if ( IS_NPC(ch) ? IS_NPC(vch) : !IS_NPC(vch) )
		    		continue;
			break;
		
	    	case TAR_CHAR_DEFENSIVE:
		case TAR_OBJ_CHAR_DEF:
		case TAR_CHAR_GENERAL:
			break;
		
		case TAR_CHAR_AMBIGUOUS:
				if (ch != vch && !trusts(ch,vch) && is_safe(ch,vch))
				continue;
			break;

	    	case TAR_CHAR_SELF:
				if ( vch != ch )
		    		continue;
			break;
	
	    }
	    obj_cast_spell( staff->value[3], staff->value[0], ch, vch, NULL );
	    check_improve(ch,gsn_staves,TRUE,2);
		}
	}
    }

    if ( --staff->value[2] <= 0 )
    {
	act( "$n's $p blazes bright and is gone.", ch, staff, NULL, TO_ROOM );
	act( "Your $p blazes bright and is gone.", ch, staff, NULL, TO_CHAR );
	extract_obj( staff );
    }

    return;
}



void do_zap( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *wand;
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' && ch->fighting == NULL )
    {
	send_to_char( "Zap whom or what?\n\r", ch );
	return;
    }

    if ( ( wand = get_eq_char( ch, WEAR_HOLD ) ) == NULL )
    {
	send_to_char( "You hold nothing in your hand.\n\r", ch );
	return;
    }

    if ( wand->item_type != ITEM_WAND )
    {
	send_to_char( "You can zap only with a wand.\n\r", ch );
	return;
    }

    obj = NULL;
    if ( arg[0] == '\0' )
    {
	if ( ch->fighting != NULL )
	{
	    victim = ch->fighting;
	}
	else
	{
	    send_to_char( "Zap whom or what?\n\r", ch );
	    return;
	}
    }
    else
    {
	if ( ( victim = get_char_room ( ch, arg ) ) == NULL
	&&   ( obj    = get_obj_here  ( ch, arg ) ) == NULL )
	{
	    send_to_char( "You can't find it.\n\r", ch );
	    return;
	}
    }
	if (check_entwine(ch,1))
		return send_to_char("You are too entwined to do that.\n\r",ch);


    WAIT_STATE( ch, 2 * PULSE_VIOLENCE );

    if ( wand->value[2] > 0 )
    {
	if ( victim != NULL )
	{
	    act( "$n zaps $N with $p.", ch, wand, victim, TO_NOTVICT );
	    act( "You zap $N with $p.", ch, wand, victim, TO_CHAR );
	    act( "$n zaps you with $p.",ch, wand, victim, TO_VICT );
	}
	else
	{
	    act( "$n zaps $P with $p.", ch, wand, obj, TO_ROOM );
	    act( "You zap $P with $p.", ch, wand, obj, TO_CHAR );
	}

 	if (ch->level < wand->level
	||  number_percent() >= 20 + get_skill(ch,gsn_wands) * 4/5)
	{
	    act( "Your efforts with $p produce only smoke and sparks.",
		 ch,wand,NULL,TO_CHAR);
	    act( "$n's efforts with $p produce only smoke and sparks.",
		 ch,wand,NULL,TO_ROOM);
	    check_improve(ch,gsn_wands,FALSE,2);
	}
	else
	{
		if(!check_deny_magic(ch))
			obj_cast_spell( wand->value[3], wand->value[0], ch, victim, obj );
	    check_improve(ch,gsn_wands,TRUE,2);
	}
    }

    if ( --wand->value[2] <= 0 )
    {
	act( "$n's $p explodes into fragments.", ch, wand, NULL, TO_ROOM );
	act( "Your $p explodes into fragments.", ch, wand, NULL, TO_CHAR );
	extract_obj( wand );
    }

    return;
}

void do_steal( CHAR_DATA *ch, char *argument )
{
    char buf  [MAX_STRING_LENGTH];
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    int chance;
	char query[MSL];

    if(get_skill(ch,gsn_steal)==0
		|| ch->level<skill_table[gsn_steal].skill_level[ch->Class()->GetIndex()])
		return send_to_char("Huh?\n\r",ch);

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
		return send_to_char( "Steal what from whom?\n\r", ch );

    if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
		return send_to_char( "They aren't here.\n\r", ch );

    if ( victim == ch )
		return send_to_char( "That's pointless.\n\r", ch );

    if (is_safe(ch,victim))
		return;

    if (victim->fighting != NULL)
		return send_to_char("You can't get close enough in the fray!\n\r",ch);

	if (IS_CABAL_GUARD(victim))
		return send_to_char("You can't do that.\n\r",ch);

	check_keen_vision(ch,victim);

    WAIT_STATE( ch, skill_table[gsn_steal].beats );

    chance = get_skill(ch,gsn_steal);
	
	chance /= 2;

	chance += 5 * (get_curr_stat(ch,STAT_DEX) - 20);

	if (IS_NPC(victim))
		chance -= 5 * (victim->level - ch->level);
	else
		chance -= 2 * (victim->level - ch->level);

	if (can_see(victim,ch))
		chance -= 25;
	else
		chance += 5;

	if (!IS_AFFECTED(ch,AFF_SNEAK))
		chance -= 10;

	if (!IS_AWAKE(victim))
		chance += 25;
	else
		chance -= 2 * (get_curr_stat(victim,STAT_DEX) - 15);

	if (ch->Class()->name == "anti-paladin")
		chance = UMAX(40,chance);
	
    if(IS_NPC(victim) && victim->pIndexData->pShop)
		chance = 0;
    if(get_trust(ch) >= 59)
	chance = 100;
    if ( number_percent() > chance)
    {
	/*
	 * Failure.
	 */
	send_to_char( "Oops.\n\r", ch );
	affect_strip(ch,gsn_sneak);
	REMOVE_BIT(ch->affected_by,AFF_SNEAK);

	check_improve(ch,gsn_steal,FALSE,2);
	act( "$n tried to steal from you.\n\r", ch, NULL, victim, TO_VICT    );
	act( "$n tried to steal from $N.\n\r",  ch, NULL, victim, TO_NOTVICT );

	if(!IS_NPC(ch) && !IS_NPC(victim) && !IS_IMMORTAL(ch) && !IS_IMMORTAL(victim))
		RS.SQL.Insert("theft(ch, victim, obj, success) values('%s','%s','none',0)", ch->true_name, victim->true_name);

	switch(number_range(0,3)) {
		case 0 :
			sprintf( buf, "%s is a lousy thief!", PERS(ch,victim) );
			break;
        case 1 :
			sprintf( buf, "%s couldn't rob %s way out of a paper bag!",
				PERS(ch,victim),(ch->sex == 2) ? "her" : "his");
			break;
		case 2 :
			sprintf( buf,"%s tried to rob me!",PERS(ch,victim) );
			break;
		case 3 :
			sprintf(buf,"Keep your hands out of there, %s!",PERS(ch,victim));
			break;
	}
	
	if (IS_AWAKE(victim) && !IS_NPC(victim))
	    do_yell( victim, buf );

	if ( !IS_NPC(ch)) {
	    if ( IS_NPC(victim) && IS_AWAKE(victim))
			multi_hit( victim, ch, TYPE_UNDEFINED );
	    else {
			sprintf(buf,"$N tried to steal from %s.",victim->name);
			wiznet(buf,ch,NULL,WIZ_FLAGS,0,0);
	    }
	}

	return;
    }

    if ( !str_cmp( arg1, "coin"  ) ||   !str_cmp( arg1, "coins") ||   !str_cmp( arg1, "gold"  ))
    {
	int gold;


		if(victim->gold <= 100)
			gold = victim->gold;
		else
			gold = number_range((int)(victim->gold / 8), victim->gold);

		if ( gold <= 0)
	    		return send_to_char( "You couldn't get any coins.\n\r", ch );

		ch->gold += gold;
		victim->gold -= gold;
	    	sprintf( buf, "Bingo!  You got %d gold coins.\n\r", gold );

		if(!IS_NPC(ch) && !IS_NPC(victim) && !IS_IMMORTAL(ch) && !IS_IMMORTAL(victim))
			RS.SQL.Insert("theft(ch, victim, obj, success) values('%s','%s','%d',1)", ch->true_name, victim->true_name, gold);

		send_to_char( buf, ch );
		check_improve(ch,gsn_steal,TRUE,2);

		if(IS_NPC(victim) && !victim->stolen_from)
			victim->stolen_from = TRUE;
		
		return;
    }

    if ( ( obj = get_obj_carry( victim, arg1, ch ) ) == NULL )
		return send_to_char( "You can't find it.\n\r", ch );
	
    if ( !can_drop_obj( ch, obj )
		||   IS_SET(obj->extra_flags, ITEM_INVENTORY))
		return send_to_char( "You can't pry it away.\n\r", ch );

    if ( ch->carry_number + get_obj_number( obj ) > can_carry_n( ch ) )
		return send_to_char( "You have your hands full.\n\r", ch );

    if ( ch->carry_weight + get_obj_weight( obj ) > can_carry_w( ch ) )
		return send_to_char( "You can't carry that much weight.\n\r", ch );

    obj_from_char( obj );
    obj_to_char( obj, ch );
    check_improve(ch,gsn_steal,TRUE,2);

	if(is_affected(victim, gsn_accumulate_heat)) {
		act("As you grab for $p you are burned!",ch,obj,victim, TO_CHAR);
		act("As $n grabs for $p, $e is burned!",ch,obj,victim, TO_VICT);
		damage_new(victim,ch,dice(4,6),gsn_burning_hands,DAM_FIRE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);
	}

	act( "You steal $p from $N.", ch, obj, victim, TO_CHAR);

    if(!IS_NPC(ch) && !IS_NPC(victim) && !IS_IMMORTAL(ch) && !IS_IMMORTAL(victim))
		RS.SQL.Insert("theft(ch, victim, obj, success) values('%s','%s','%s',1)",
				ch->true_name,
				victim->true_name,
				obj->short_descr);
		
	return;
}



/*
 * Shopping commands.
 */
CHAR_DATA *find_keeper( CHAR_DATA *ch )
{
    /*char buf[MAX_STRING_LENGTH];*/
    CHAR_DATA *keeper;
    SHOP_DATA *pShop;

    pShop = NULL;
    for ( keeper = ch->in_room->people; keeper; keeper = keeper->next_in_room )
    {
	if ( IS_NPC(keeper) && (pShop = keeper->pIndexData->pShop) != NULL )
	    break;
    }

    if ( pShop == NULL )
    {
	send_to_char( "You can't do that here.\n\r", ch );
	return NULL;
    }

    /*
     * Undesirables.
     *
    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_KILLER) )
    {
	do_say( keeper, "Killers are not welcome!" );
	sprintf( buf, "%s the KILLER is over here!\n\r", ch->name );
	do_yell( keeper, buf );
	return NULL;
    }

    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_THIEF) )
    {
	do_say( keeper, "Thieves are not welcome!" );
	sprintf( buf, "%s the THIEF is over here!\n\r", ch->name );
	do_yell( keeper, buf );
	return NULL;
    }
	*/
    /*
     * Shop hours.
     */
    if ( time_info.hour < pShop->open_hour && pShop->open_hour != pShop->close_hour)
    {
	do_say( keeper, "Sorry, I am closed. Come back later." );
	return NULL;
    }

    if ( time_info.hour > pShop->close_hour && pShop->open_hour != pShop->close_hour)
    {
	do_say( keeper, "Sorry, I am closed. Come back tomorrow." );
	return NULL;
    }

    /*
     * Invisible or hidden people.
     */
    if ( !can_see( keeper, ch ) )
    {
	do_say( keeper, "I don't trade with folks I can't see." );
	return NULL;
    }

    return keeper;
}

/* insert an object at the right spot for the keeper */
void obj_to_keeper( OBJ_DATA *obj, CHAR_DATA *ch )
{
    OBJ_DATA *t_obj, *t_obj_next;

    /* see if any duplicates are found */
    for (t_obj = ch->carrying; t_obj != NULL; t_obj = t_obj_next)
    {
	t_obj_next = t_obj->next_content;

	if (obj->pIndexData == t_obj->pIndexData
	&&  !str_cmp(obj->short_descr,t_obj->short_descr))
	{
	    /* if this is an unlimited item, destroy the new one */
	    if (IS_OBJ_STAT(t_obj,ITEM_INVENTORY))
	    {
		extract_obj(obj);
		return;
	    }
	    obj->cost = t_obj->cost; /* keep it standard */
	    break;
	}
    }

    if (t_obj == NULL)
    {
	obj->next_content = ch->carrying;
	ch->carrying = obj;
    }
    else
    {
	obj->next_content = t_obj->next_content;
	t_obj->next_content = obj;
    }

    obj->carried_by      = ch;
    obj->in_room         = NULL;
    obj->in_obj          = NULL;
    ch->carry_number    += get_obj_number( obj );
    ch->carry_weight    += get_obj_weight( obj );
}

/* get an object from a shopkeeper's list */
OBJ_DATA *get_obj_keeper( CHAR_DATA *ch, CHAR_DATA *keeper, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int number;
    int count;

    number = number_argument( argument, arg );
    count  = 0;
    for ( obj = keeper->carrying; obj != NULL; obj = obj->next_content )
    {
        if (obj->wear_loc == WEAR_NONE
        &&  can_see_obj( keeper, obj )
	&&  can_see_obj(ch,obj)
        &&  is_name( arg, obj->name ) )
        {
            if ( ++count == number )
                return obj;
	
	    /* skip other objects of the same name */
	    while (obj->next_content != NULL
	    && obj->pIndexData == obj->next_content->pIndexData
	    && !str_cmp(obj->short_descr,obj->next_content->short_descr))
		obj = obj->next_content;
        }
    }

    return NULL;
}

int get_cost( CHAR_DATA *keeper, OBJ_DATA *obj, bool fBuy )
{
    SHOP_DATA *pShop;
    float cost;

    if ( obj == NULL || ( pShop = keeper->pIndexData->pShop ) == NULL )
	return 0;

    cost = obj->cost * calculate_inflation();

/*    else
    {
	OBJ_DATA *obj2;
	int itype;

	cost = 0;
	for ( itype = 0; itype < MAX_TRADE; itype++ )
	{
	    if ( obj->item_type == pShop->buy_type[itype] )
	    {
		cost = (obj->cost * pShop->profit_sell / 100) * calculate_inflation();
		break;
	    }
	}

	if (!IS_OBJ_STAT(obj,ITEM_SELL_EXTRACT))
	    for ( obj2 = keeper->carrying; obj2; obj2 = obj2->next_content )
	    {
	    	if ( obj->pIndexData == obj2->pIndexData
		&&   !str_cmp(obj->short_descr,obj2->short_descr) )
	 	{
		    if (IS_OBJ_STAT(obj2,ITEM_INVENTORY))
			cost /= 2;
		    else
                    	cost = cost * 3 / 4;
		}
	    }
    }

    if ( obj->item_type == ITEM_STAFF || obj->item_type == ITEM_WAND )
    {
	if (obj->value[1] == 0)
	    cost /= 4;
	else
	    cost = cost * obj->value[2] / obj->value[1];
    }*/

    return (int)cost;
}



void do_buy( CHAR_DATA *ch, char *argument )
{
    int cost,roll;
	int i;

	if(check_horde(ch))
		return send_to_char("You would never buy what can be be taken!\n\r",ch);

	if ( argument[0] == '\0' )
		return send_to_char( "Buy what?\n\r", ch );

    if ( IS_SET(ch->in_room->room_flags, ROOM_PET_SHOP) )
    {
	char arg[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
	CHAR_DATA *pet;
	ROOM_INDEX_DATA *pRoomIndexNext;
	ROOM_INDEX_DATA *in_room;

	if ( IS_NPC(ch) )
	    return;

	argument = one_argument(argument,arg);

	/* hack to make new thalos pets work */
	    pRoomIndexNext = get_room_index( ch->in_room->vnum + 1 );
	if ( pRoomIndexNext == NULL )
	{
	    bug( "Do_buy: bad pet shop at vnum %d.", ch->in_room->vnum );
	    send_to_char( "Sorry, you can't buy that here.\n\r", ch );
	    return;
	}

	in_room     = ch->in_room;
	ch->in_room = pRoomIndexNext;
	pet         = get_char_room( ch, arg );
	ch->in_room = in_room;

	if ( pet == NULL || !IS_SET(pet->act, ACT_PET) )
	   return send_to_char( "Sorry, you can't buy that here.\n\r", ch );

	if ( ch->pet != NULL )
	    return send_to_char("You already own a pet.\n\r",ch);

 	cost = 10 * pet->level * pet->level;

	if ( (ch->gold) < cost )
	    return send_to_char( "You can't afford it.\n\r", ch );

	if ( ch->level < pet->level )
	    return send_to_char("You're not powerful enough to master this pet.\n\r", ch );

	/* haggle */
	roll = number_percent();
	bool appraised = FALSE;
	if (roll < get_skill(ch,gsn_haggle) || (appraised = ch->Profs()->HasProf("appraising")))
	{
	    if(get_skill(ch, gsn_haggle) > 60)
   		cost -= cost / 3 * roll / 100;
	    sprintf(buf,"You haggle the price down to %d coins.\n\r",cost);
	    if(appraised && cost)
	    {
		float mcost = cost * ch->Profs()->ProfEffect("appraising") / 100;
                cost = (int) mcost;
		sprintf(buf,"Knowing it is inflated, you argue the price down to %d coins.\n\r",cost);
	    }
	    send_to_char(buf,ch);
	    check_improve(ch,gsn_haggle,TRUE,4);
	
	}

	deduct_cost(ch,cost);
	pet			= create_mobile( pet->pIndexData );
	SET_BIT(pet->act, ACT_PET);
	SET_BIT(pet->affected_by, AFF_CHARM);
	SET_BIT(pet->comm, COMM_NOTELL);
	SET_BIT(pet->comm, COMM_NOSHOUT);
	SET_BIT(pet->comm, COMM_NOCHANNELS);

	argument = one_argument( argument, arg );
	if ( arg[0] != '\0' )
	{
	    sprintf( buf, "%s %s", pet->name, arg );
	    free_pstring( pet->name );
	    pet->name = palloc_string( buf );
	}
	sprintf( buf, "%sA neck tag says 'I belong to %s'.\n\r",
	    pet->description, ch->name );
	free_pstring( pet->description );
	pet->description = palloc_string( buf );

	char_to_room (pet, ch->in_room );
	add_follower( pet, ch );
	pet->leader = ch;
	ch->pet = pet;
	send_to_char( "Enjoy your pet.\n\r", ch );
	act( "$n bought $N as a pet.", ch, NULL, pet, TO_ROOM );
	return;
    }
    else
    {
	CHAR_DATA *keeper;
	OBJ_DATA *obj=NULL,*t_obj=NULL,*d_obj=NULL,*c_obj=NULL;
	char arg[MAX_INPUT_LENGTH], buf[MSL];
	int number = 1, count = 1;

	if ( ( keeper = find_keeper( ch ) ) == NULL )
	    return;

	if (keeper == ch)
		return;

	argument = one_argument(argument,arg);

	if (argument[0] != '\0')
		number = atoi(argument);
	obj  = get_obj_keeper( ch,keeper, arg );
	cost = get_cost( keeper, obj, TRUE );

	if (number < 1)
	    return interpret(keeper,"snort");

	if (number > 9) {
		do_say(keeper,"I'm not selling you that many at once.");
		return;
	}

	if ( cost <= 0 || !can_see_obj( ch, obj ) )
	{
	    sprintf(buf,"%s I don't sell that, try 'list'ing to see what I do sell.",ch->name);
	    do_tell(keeper,buf);
	    return;
	}

	if (!IS_OBJ_STAT(obj,ITEM_INVENTORY))
	{
	    for (t_obj = obj->next_content;
	     	 count < number && t_obj != NULL;
	     	 t_obj = t_obj->next_content)
	    {
	    	if (t_obj->pIndexData == obj->pIndexData
	    	&&  !str_cmp(t_obj->short_descr,obj->short_descr))
		    count++;
	    	else
		    break;
	    }

	    if (count < number)
	    {
	    	act("$n tells you 'I don't have that many in stock.",keeper,NULL,ch,TO_VICT);
	    	ch->reply = keeper;
	    	return;
	    }
	}

	if ( ch->gold < cost * number )
	{
	    if (number > 1)
		act("$n tells you 'You can't afford to buy that many.",
		    keeper,obj,ch,TO_VICT);
	    else
	    	act( "$n tells you 'You can't afford to buy $p'.",
		    keeper, obj, ch, TO_VICT );
	    ch->reply = keeper;
	    return;
	}
	
	if ( obj->level > ch->level )
	{
	    act( "$n tells you 'You can't use $p yet'.",
		keeper, obj, ch, TO_VICT );
	    ch->reply = keeper;
	    return;
	}

	if (ch->carry_number +  number * get_obj_number(obj) > can_carry_n(ch))
	{
	    send_to_char( "You can't carry that many items.\n\r", ch );
	    return;
	}

	if ( ch->carry_weight + number * get_obj_weight(obj) > can_carry_w(ch))
	{
	    send_to_char( "You can't carry that much weight.\n\r", ch );
	    return;
	}

	if (check_benevolence(ch,keeper))
		cost /= 2;

	/* haggle */
	roll = number_percent();
	bool appraised = FALSE;
	if ((!IS_OBJ_STAT(obj,ITEM_SELL_EXTRACT)
	&& roll < get_skill(ch,gsn_haggle)) || (appraised = ch->Profs()->HasProf("appraising")))
	{
	    if(get_skill(ch, gsn_haggle) > 60)
	    	cost -= obj->cost / 2 * roll / 100;
            if(appraised && cost)
            {
		float mcost = cost * ch->Profs()->ProfEffect("appraising") / 100;
		cost = (int) mcost;
		sprintf(buf,"Knowing it is inflated, you argue the price down to %d coins.\n\r",cost);
		send_to_char(buf,ch);
            }
	    else
	    	act("You haggle with $N.",ch,NULL,keeper,TO_CHAR);
	    check_improve(ch,gsn_haggle,TRUE,4);
	}

	act("$n hands some gold to $N.",ch,0,keeper,TO_ROOM);
	sprintf(buf,"You hand $N %d gold.",cost * number);
	act(buf,ch,0,keeper,TO_CHAR);
	
	for (i=0;i < number;i++) {
		act("$N hands you $p.",ch,obj,keeper,TO_CHAR);
		act("$N hands $n $p.",ch,obj,keeper,TO_ROOM);
	}
	
	deduct_cost(ch,cost * number);

	for (count = 0; count < number; count++)
	{
	    if ( IS_SET( obj->extra_flags, ITEM_INVENTORY ) ) {
	    	t_obj = create_object( obj->pIndexData, obj->level );

			if(obj->item_type == ITEM_CONTAINER && obj->contains) {
				for(c_obj = obj->contains; c_obj != NULL; c_obj = c_obj->next_content) {
					d_obj = create_object(get_obj_index(c_obj->pIndexData->vnum),c_obj->level);
					obj_to_obj(d_obj,t_obj);
					}
				}		
		}
	    else
	    {
		t_obj = obj;
		obj = obj->next_content;
	    	obj_from_char( t_obj );
	    }

	    if (t_obj->timer > 0)
	    	t_obj->timer = 0;
	    obj_to_char( t_obj, ch );
	    if (cost < t_obj->cost)
	    	t_obj->cost = cost;
	}
    }
}



void do_list( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

	if(check_horde(ch))
		return send_to_char("You would never buy what can be taken!\n\r",ch);


	
    if ( IS_SET(ch->in_room->room_flags, ROOM_PET_SHOP) )
    {
	ROOM_INDEX_DATA *pRoomIndexNext;
	CHAR_DATA *pet;
	bool found;

        /* hack to make new thalos pets work */
        if (ch->in_room->vnum == 9621)
            pRoomIndexNext = get_room_index(9706);
        else
            pRoomIndexNext = get_room_index( ch->in_room->vnum + 1 );

	if ( pRoomIndexNext == NULL )
	{
	    bug( "Do_list: bad pet shop at vnum %d.", ch->in_room->vnum );
	    send_to_char( "You can't do that here.\n\r", ch );
	    return;
	}

	found = FALSE;
	for ( pet = pRoomIndexNext->people; pet; pet = pet->next_in_room )
	{
	    if ( IS_SET(pet->act, ACT_PET) )
	    {
		if ( !found )
		{
		    found = TRUE;
		    send_to_char( "Pets for sale:\n\r", ch );
		}
		sprintf( buf, "[%2d] %8d - %s\n\r",
		    pet->level,
		    10 * pet->level * pet->level,
		    pet->short_descr );
		send_to_char( buf, ch );
	    }
	}
	if ( !found )
	    send_to_char( "Sorry, we're out of pets right now.\n\r", ch );
	return;
    }
    else
    {
	CHAR_DATA *keeper;
	OBJ_DATA *obj;
	int cost,count;
	bool found;
	char arg[MAX_INPUT_LENGTH];

	if ( ( keeper = find_keeper( ch ) ) == NULL )
	    return;
        one_argument(argument,arg);

	found = FALSE;
	for ( obj = keeper->carrying; obj; obj = obj->next_content )
	{
	    if ( obj->wear_loc == WEAR_NONE
	    &&   can_see_obj( ch, obj )
	    &&   ( cost = get_cost( keeper, obj, TRUE ) ) > 0
	    &&   ( arg[0] == '\0'
 	       ||  is_name(arg,obj->name) ))
	    {
		if ( !found )
		{
		    found = TRUE;
		    send_to_char( "[Lv Price Qty] Item\n\r", ch );
		}

		if (IS_OBJ_STAT(obj,ITEM_INVENTORY))
		    sprintf(buf,"[%2d %5d -- ] %s\n\r",
			obj->level,cost,obj->short_descr);
		else
		{
		    count = 1;

		    while (obj->next_content != NULL
		    && obj->pIndexData == obj->next_content->pIndexData
		    && !str_cmp(obj->short_descr,
			        obj->next_content->short_descr))
		    {
			obj = obj->next_content;
			count++;
		    }
		    sprintf(buf,"[%2d %5d %2d ] %s\n\r",
			obj->level,cost,count,obj->short_descr);
		}
		send_to_char( buf, ch );
	    }
	}

	if ( !found )
	    send_to_char( "You can't buy anything here.\n\r", ch );
	return;
    }
}



void do_sell( CHAR_DATA *ch, char *argument )
{
//    char buf[MAX_STRING_LENGTH];
//    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *keeper;
//    OBJ_DATA *obj;
//    int cost,roll;

    if ( argument[0] == '\0' )
    {
	send_to_char( "Sell what?\n\r", ch );
	return;
    }

    if ( ( keeper = find_keeper( ch ) ) == NULL )
	return;

    do_say(keeper,"I don't want to buy anything from the likes of you.");
    return;
    /*
    act( "$n sells $p.", ch, obj, NULL, TO_ROOM );
    roll = number_percent();
    if (!IS_OBJ_STAT(obj,ITEM_SELL_EXTRACT) && roll < get_skill(ch,gsn_haggle))
    {
        send_to_char("You haggle with the shopkeeper.\n\r",ch);
        cost += obj->cost / 2 * roll / 100;
        cost = UMIN(cost,95 * get_cost(keeper,obj,TRUE) / 100);
	cost = UMIN(cost,(keeper->gold));
        check_improve(ch,gsn_haggle,TRUE,4);
    }
    sprintf( buf, "You sell $p for %d gold piece%s.", cost/100, cost == 1 ? "" : "s" );
    act( buf, ch, obj, NULL, TO_CHAR );
    ch->gold     += cost/100;
    deduct_cost(keeper,cost);
    if ( keeper->gold < 0 )
	keeper->gold = 0;

    if ( obj->item_type == ITEM_TRASH || IS_OBJ_STAT(obj,ITEM_SELL_EXTRACT))
    {
	extract_obj( obj );
    }
    else
    {
	obj_from_char( obj );
	obj->timer = number_range(50,100);
	obj_to_keeper( obj, keeper );
    }
    */
    return;
}



void do_value( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *keeper;
    OBJ_DATA *obj;
    int cost;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Value what?\n\r", ch );
	return;
    }

    if ( ( keeper = find_keeper( ch ) ) == NULL )
	return;

    if ( ( obj = get_obj_carry( ch, arg, ch ) ) == NULL )
    {
	act( "$n tells you 'You don't have that item'.",
	    keeper, NULL, ch, TO_VICT );
	ch->reply = keeper;
	return;
    }

    if (!can_see_obj(keeper,obj))
    {
        act("$n doesn't see what you are offering.",keeper,NULL,ch,TO_VICT);
        return;
    }

    if ( !can_drop_obj( ch, obj ) )
    {
	send_to_char( "You can't let go of it.\n\r", ch );
	return;
    }

    if ( ( cost = get_cost( keeper, obj, FALSE ) ) <= 0 )
    {
	act( "$n looks uninterested in $p.", keeper, obj, ch, TO_VICT );
	return;
    }

    sprintf( buf,
	"$n tells you 'I'll give you %d gold coins for $p.'", cost/100 );
    act( buf, keeper, obj, ch, TO_VICT );
    ch->reply = keeper;

    return;
}


void do_request(CHAR_DATA *ch,char *argument)
{
    AFFECT_DATA af;
    CHAR_DATA  *victim;
    OBJ_DATA *obj;
    char i_name[MAX_INPUT_LENGTH];
    char m_name[MAX_INPUT_LENGTH];
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
	char *tname;
	
    tname = one_argument(argument,i_name);  /* syntax: request <obj>
<mob> */
    one_argument(tname,m_name);

    if (IS_NPC(ch))
        return;     /* NPCs can't request */
    if (i_name[0] == '\0' || m_name[0] == '\0')
    {
	send_to_char("Request what of whom?\n\r",ch);
	return;
    }
    if ((victim = get_char_room(ch,m_name)) == NULL )
    {
        send_to_char("They aren't here.\n\r",ch);
        return;
    }

    if (is_affected(ch,gsn_request))
	{
	send_to_char("You are still in contemplation of the last act of generosity.\n\r",ch);
	return;
	}

    if (!IS_NPC(victim))
    {
        send_to_char("Why not ask for it yourself?\n\r",ch);
        return;
    }

    if (IS_CABAL_GUARD(victim))
    {
	sprintf(buf1,"Do not dare to request my belongings!");
	sprintf(buf2,"Help! I'm being attacked by %s!",victim->short_descr);
	do_say(victim,buf1);
	return;
    }

    if (!IS_GOOD(victim))
    {
	sprintf(buf1, "You dare to ask for my belongings!");
	sprintf(buf2, "Help! I'm being attacked by %s!",victim->short_descr);
	do_say(victim,buf1);
	return;
    }

    if (!IS_GOOD(ch))
    {
        sprintf(buf1,"You are unworthy to ask for anything!");
        sprintf(buf2,"Help! I'm being attacked by %s!", victim->short_descr);
        do_say(victim,buf1);
        return;
    }

    if ( (obj = get_obj_list(victim, i_name, victim->carrying)) == NULL)
        return send_to_char("They don't have that object.\n\r",ch);

    if (!can_see_obj(ch,obj) )
        return send_to_char("They don't have that object.\n\r",ch);

    if (!can_see(victim,ch))
	return mob_tell(victim, ch, "I can't give you it if I can't see you.");

    if (!can_see_obj(victim,obj))
	return mob_tell(victim, ch, "I can't see that.");

    if ((victim->level >= (ch->level + 8) && victim->level < 58) ||
    	(victim->level >= 59 && ch->level < 51))
		return mob_tell(victim, ch, "All in good time, my child.");
    if (ch->move < obj->level || ch->hit < (ch->max_hit*3/7))
        return mob_tell(victim, ch, "You look tired, perhaps you should rest first.");
    
    if ((is_worn(obj) && IS_SET(obj->extra_flags, ITEM_NOREMOVE)) || IS_SET(obj->extra_flags, ITEM_NODROP))
        return mob_tell(victim, ch, "I'm sorry, friend, but I cannot let go of it.");

    if ( ch->carry_weight + get_obj_weight(obj)  > can_carry_w(ch) )
    {
        act("$N tells you, 'You can't carry the weight $n.'",ch,0,victim,TO_CHAR);
        return;
    }
    if ( ch->carry_number + 1 > can_carry_n(ch) )
    {
        act("$N tells you, 'You can't carry that many items $n.'",ch,0,victim,TO_CHAR);
        return;
    }
    if (isCabalItem(obj))
	return send_to_char("You can't do that.\n\r",ch);

    act("$n requests off $N.",ch,0,victim,TO_ROOM);
    if (obj->wear_loc != WEAR_NONE)
    {
        act("$n stops using $p.",victim,obj,NULL,TO_ROOM);
        act("You stop using $p.",victim,obj,NULL,TO_CHAR);
    }
    act("$N gives $p to $n.",ch,obj,victim,TO_NOTVICT);
    act("$N gives you $p.",ch,obj,victim,TO_CHAR);
    obj_from_char(obj);
    obj_to_char(obj,ch);

	if (IS_SET(obj->progtypes,IPROG_GIVE))
		(obj->pIndexData->iprogs->give_prog) (obj,ch,victim);

  if (IS_SET(victim->progtypes,MPROG_GIVE))
    (victim->pIndexData->mprogs->give_prog) (victim,ch,obj);


    send_to_char("You stop and contemplate the goodness you have seen in the world.\n\r",ch);


init_affect(&af);
af.where = TO_AFFECTS;
af.aftype = AFT_SKILL;
af.type = gsn_request;
af.modifier = 0;
af.duration = 5;
af.location = 0;
af.level = ch->level;
affect_to_char(ch,&af);

    ch->move -= obj->level;
    ch->hit -= obj->level;
    return;
}




bool hands_full(CHAR_DATA *ch)
{
    OBJ_DATA *weapon;
    int count;

    count = 0;

   if (get_eq_char(ch,WEAR_LIGHT) != NULL)
	count++;
   if (get_eq_char(ch,WEAR_WIELD) != NULL)
	count++;
   if (get_eq_char(ch,WEAR_HOLD) != NULL)
	count++;
   if (get_eq_char(ch,WEAR_SHIELD) != NULL)
	count++;
   if (get_eq_char(ch,WEAR_DUAL_WIELD) != NULL)
	count++;
   if ((weapon = get_eq_char(ch,WEAR_WIELD) )
	&& (weapon->value[0] == WEAPON_STAFF
	|| weapon->value[0] == WEAPON_POLEARM
	|| weapon->value[0] == WEAPON_SPEAR))
	count ++;

	count -= ch->arms - 2;

   if (count < 2)
	return FALSE;
   if (count > 2)
   	bug("Hands full: Character holding %d items.",count);

    return TRUE;

}

void do_embalm(CHAR_DATA *ch,char *argument)
{
    OBJ_DATA *corpse;
    char arg[100];

    if ( (get_skill(ch,gsn_embalm) == 0)
    || ch->level < skill_table[gsn_embalm].skill_level[ch->Class()->GetIndex()])
    {
        send_to_char("You don't know how to embalm corpses.\n\r",ch);
        return;
    }
    one_argument(argument,arg);
    if (arg[0] == '\0')
    {
        send_to_char("Embalm what?\n\r",ch);
        return;
    }

    if ( (corpse = get_obj_here(ch,arg)) == NULL)
    {
        send_to_char("You don't have that item to embalm.\n\r",ch);
        return;
    }
    if (corpse->item_type != ITEM_CORPSE_NPC)
    {
        act("$p isn't a corpse you can embalm.\n\r",ch,corpse,0,TO_CHAR);
        return;
    }
    if (corpse->value[4] == 1)
    {
        send_to_char("That corpse has already been embalmed.\n\r",ch);
        return;
    }

    if (number_percent() > ch->pcdata->learned[gsn_embalm])
    {
        act("You spill your embalming fluids all over $p, ruining it.",ch,corpse,0,TO_CHAR);
        act("$n spills embalming fluids over $p and ruins it.",ch,corpse,0,TO_ROOM);
        check_improve(ch,gsn_embalm,FALSE,4);
	extract_obj(corpse);
        return;
    }
    act("$n embalms $p!",ch,corpse,0,TO_ROOM);
    act("You succeed in embalming $p.",ch,corpse,0,TO_CHAR);
    corpse->timer += ch->level/2;
    corpse->value[4] = 1;
    check_improve(ch,gsn_embalm,TRUE,4);
    return;
}

void cabal_shudder(int cabal, bool itemloss)
{
	CHAR_DATA *ch;

	for (ch = char_list; ch != NULL; ch = ch->next)
	{
        if (ch->cabal == cabal)
		{
			if (IS_SET(ch->comm,COMM_ANSI) && itemloss)
				send_to_char("\x01B[1;31mYou feel a shudder in your Cabal power!\x01B[0m\n\r",ch);
			else if (IS_SET(ch->comm,COMM_ANSI) && !itemloss) 
                send_to_char("\x01B[1;32mYou feel a shudder in your Cabal power!\x01B[0m\n\r",ch);
			else 
				send_to_char("You feel a shudder in your Cabal power!\n\r",ch);
		}
	}
	return;
}

bool cabal_down(CHAR_DATA *ch, int cabal)
{
	return cabal_down_new(ch,cabal,TRUE);
}

bool cabal_down_new(CHAR_DATA *ch, int cabal, bool show)
{
    OBJ_DATA *obj;
    bool is_down = FALSE;
    int objvnum = cabal_table[ch->cabal].item_vnum;

	
	for (obj = object_list; obj != NULL; obj = obj->next)
        if (obj->pIndexData->vnum == objvnum)
			break;

    if(!obj || !obj->carried_by || !IS_NPC(obj->carried_by))
    	return FALSE;

    if(obj->carried_by->cabal && obj->carried_by->cabal != cabal)
		is_down = TRUE;

	if (is_down)
    {
		if (IS_IMMORTAL(ch) || IS_HEROIMM(ch))
		{
			if(show)
				send_to_char("Your cabal has no power but you can do that anyway.\n\r",ch);
			return FALSE;
        }
	  
		if(show)
        	send_to_char("You cannot find the power within you.\n\r",ch);
    }

    return is_down;
}

bool cant_carry(CHAR_DATA *ch, OBJ_DATA *obj)
{
    bool status;
    status = FALSE;
    return FALSE;
}

bool is_restricted(CHAR_DATA *ch,OBJ_DATA *obj)
{
    int i;
	long restrict[MAX_BITVECTOR];
    bool status;
    char *race;

    if (IS_NPC(ch) && !IS_AFFECTED(ch,AFF_CHARM))
	return FALSE;

    if (str_cmp(obj->owner,"none") && !is_owner(ch,obj))
	return TRUE;

    race = race_table[ch->race].name;
	copy_vector(restrict, obj->pIndexData->restrict_flags);

    status = FALSE;

    if (!restrict)
	return FALSE;
    for(i = 0; restrict_table[i].name != NULL; i++)
    {
	if(!IS_SET(restrict, restrict_table[i].bit))
		continue;
	if(restrict_table[i].type == RESTRICT_RACE)
		if(!strstr(race_table[ch->race].name, restrict_table[i].value) && (str_cmp(restrict_table[i].value,"titan") 
		    ||  (str_cmp(race_table[ch->race].name,"abyss") && str_cmp(race_table[ch->race].name,"celestial") && 
		        str_cmp(race_table[ch->race].name,"planar"))))
			return TRUE;
	if(restrict_table[i].type == RESTRICT_CLASS)
		if(str_cmp(ch->Class()->name, restrict_table[i].value))
			return TRUE;
	if(restrict_table[i].type == RESTRICT_CABAL)
		if(str_cmp(cabal_table[ch->cabal].name, restrict_table[i].value))
			return TRUE;
	if(restrict_table[i].type == RESTRICT_OTHER)
	{
		if(IS_NPC(ch))
			return FALSE;
		if(!str_cmp(restrict_table[i].name,"mage_only") && ch->Class()->GetIndex() != CLASS_NECROMANCER &&
				ch->Class()->GetIndex() != CLASS_SHAPESHIFTER && ch->Class()->GetIndex() != CLASS_SORCERER)
			return TRUE;
		if(!str_cmp(restrict_table[i].name,"male_only") && ch->pcdata->true_sex != SEX_MALE)
			return TRUE;
		if(!str_cmp(restrict_table[i].name,"female_only") && ch->pcdata->true_sex != SEX_FEMALE)
			return TRUE;
	}
    }
    return FALSE;
}

void do_demand(CHAR_DATA *ch,char *argument)
{
    CHAR_DATA  *victim;
    OBJ_DATA *obj;
    char i_name[MAX_INPUT_LENGTH];
    char m_name[MAX_INPUT_LENGTH];
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char *tname;
    int chance, vlevel;

    tname = one_argument(argument,i_name);
    one_argument(tname,m_name);

    chance = ch->pcdata->learned[gsn_demand];
    if (chance == 0
    || ch->level < skill_table[gsn_demand].skill_level[ch->Class()->GetIndex()])
    {
        send_to_char("You are hardly intimidating enough to demand off others.\n\r",ch);
        return;
    }

    if (IS_NPC(ch))
        return;
    if ((victim = get_char_room(ch,m_name)) == NULL )
    {
        send_to_char("They aren't here.\n\r",ch);
        return;
    }
    if (!IS_NPC(victim))
    {
        send_to_char("Why not just threaten them in person?\n\r",ch);
        return;
    }
    vlevel = victim->level;
    chance /= 2;
    chance += (3*ch->level);
    chance -= 2*vlevel;

    if (IS_GOOD(victim))
        chance -= 4*vlevel;
    else if (IS_EVIL(victim))
        chance -= 2*vlevel;
    else
        chance -= 3*vlevel;

    vlevel += 8;

    if ((obj = get_obj_list(victim, i_name, victim->carrying)) == NULL)
    {
        send_to_char("They do not have that object.\n\r",ch);
        return;
    }
    if (!can_see_obj(ch,obj) )
    {
        send_to_char("They do not have that object.\n\r",ch);
        return;
    }

    if (vlevel > ch->level
    || number_percent() > chance)
    {
        check_improve(ch,gsn_demand,FALSE,2);
        sprintf(buf1,"I don't think i'd give my belongings to one as weak as you!");
        sprintf(buf2,"Help! I'm being attacked by %s!",victim->short_descr);
        do_say(victim,buf1);
        do_yell(ch,buf2);
        multi_hit(victim,ch,TYPE_UNDEFINED);
        return;
    }

    if (!can_see(victim,ch))
    {
        act("$N tells you, 'I can't give to those i can't see.'",ch,0,victim,TO_CHAR);
        return;
    }

    if (!can_see_obj(victim,obj))
    {
        act("$N tells you, 'I can't see such an object.'",ch,0,victim,TO_CHAR);
        return;
    }

    if (obj->level > ch->level + 8)
    {
        do_say(victim, "That item is far to precious to hand over to scum like you!");
        sprintf(buf1,"Help! I'm being attacked by %s!",victim->short_descr);
        do_yell(ch,buf1);
        multi_hit(victim,ch,TYPE_UNDEFINED);
        return;
    }

    if (ch->move < obj->level)
    {
        act("$N tells you, 'Hah! You couldn't even get away if i chased you!'.",ch,0,victim,TO_CHAR);
        sprintf(buf1,"Help! I'm being attacked by %s!",victim->short_descr);
        do_yell(ch,buf1);
        multi_hit(victim,ch,TYPE_UNDEFINED);
        return;
    }

    if (ch->hit < (ch->max_hit*3/7))
    {
        do_say(victim,"Hah! You look weak enough that even I could kill you!");
        sprintf(buf1,"Help! I'm being attacked by %s!",victim->short_descr);
        do_yell(ch,buf1);
        multi_hit(victim,ch,TYPE_UNDEFINED);
        return;
    }

    if (ch->mana < 30)
    {
        send_to_char("You don't have the mana.\n\r",ch);
        return;
    }

    if ((obj->wear_loc != WEAR_NONE) && IS_SET(obj->extra_flags, ITEM_NOREMOVE) )
    {
        act("$N tells you, 'I'm unable to release $p'.",ch,obj,victim,TO_CHAR);
        act("$N cowers back from you in fright.",ch,0,victim,TO_CHAR);
        act("$N cowers back from $n in fright.",ch,0,victim,TO_NOTVICT);
        return;
    }
    if (IS_SET(obj->extra_flags,ITEM_NODROP) )
    {
        act("$N tells you, 'I'm unable to release $p'.",ch,obj,victim,TO_CHAR);
        act("$N cowers back from you in fright.",ch,0,victim,TO_CHAR);
        act("$N cowers back from $n in fright.",ch,0,victim,TO_NOTVICT);
        return;
    }

    if ( ch->carry_weight + get_obj_weight(obj)  > can_carry_w(ch) )
    {
        act("$N tells you, 'You can't carry the weight $n.'",ch,0,victim,TO_CHAR);
        return;
    }
    if ( ch->carry_number + 1 > can_carry_n(ch) )
    {
        act("$N tells you, 'You can't carry that many items $n.'",ch,0,victim,TO_CHAR);
        return;
    }

    act("$N caves in to the bully tactics of $n.",ch,0,victim,TO_NOTVICT);
    act("$N shivers in fright and caves in to your bully tactics.",ch,0,victim,TO_CHAR);

    if (obj->wear_loc != WEAR_NONE)
    {
        act("$n stops using $p.",victim,obj,NULL,TO_ROOM);
        act("You stop using $p.",victim,obj,NULL,TO_CHAR);
    }
    act("$N gives $p to $n.",ch,obj,victim,TO_NOTVICT);
    act("$N gives you $p.",ch,obj,victim,TO_CHAR);
    check_improve(ch,gsn_demand,TRUE,2);
    WAIT_STATE(ch,24);
    obj_from_char(obj);
    obj_to_char(obj,ch);
    ch->mana -= 30;
    ch->hit -= 30;
    return;
}


void reslot_weapon(CHAR_DATA *ch)
{
    OBJ_DATA *primary;
    OBJ_DATA *secondary;
    primary = get_eq_char(ch,WEAR_WIELD);
    secondary = get_eq_char(ch,WEAR_DUAL_WIELD);

    if (primary == NULL && secondary != NULL)
    {
        unequip_char(ch,secondary,FALSE);
	  if(secondary==NULL)
		return;
        equip_char(ch,secondary,WEAR_WIELD,FALSE);
        primary = secondary;
        secondary = NULL;
	}

    return;
}

void report_weapon_skill(CHAR_DATA *ch,OBJ_DATA *obj)
{
    int skill, sn;
    char buf[MAX_STRING_LENGTH];

    if (IS_NPC(ch))
        return;
    if (obj->item_type != ITEM_WEAPON)
        {
        sprintf(buf,"report_weapon_skill: Bad obj->type, %d, vnum %d, carried by %s.",
        obj->item_type, obj->pIndexData->vnum,ch->name);
        bug(buf,0);
        return;
        }

    if (obj->value[0] == WEAPON_DAGGER)
        sn = gsn_dagger;
    else if (obj->value[0] == WEAPON_SWORD)
        sn = gsn_sword;
    else if (obj->value[0] == WEAPON_MACE)
        sn = gsn_mace;
    else if (obj->value[0] == WEAPON_SPEAR)
        sn = gsn_spear;
    else if (obj->value[0] == WEAPON_AXE)
        sn = gsn_axe;
    else if (obj->value[0] == WEAPON_FLAIL)
        sn = gsn_flail;
    else if (obj->value[0] == WEAPON_STAFF)
	sn = gsn_staff;
	else
	sn = -1;

    if (sn == -1)
        skill = ch->level * 3;
    else
        skill = ch->pcdata->learned[sn];
    if (skill >= 100)
        act("$p feels like a part of you!",ch,obj,NULL,TO_CHAR);
    else if (skill > 85)
        act("You feel quite confident with $p.",ch,obj,0,TO_CHAR);
    else if (skill > 70)
        act("You are skilled with $p.",ch,obj,0,TO_CHAR);
    else if (skill > 50)
        act("Your skill with $p is adequate.",ch,obj,0,TO_CHAR);
    else if (skill > 25)
        act("$p feels a little clumsy in your hands.",ch,obj,0,TO_CHAR);
    else if (skill > 1)
        act("You fumble and almost drop $p.",ch,obj,0,TO_CHAR);
    else
        act("You don't even know which end is up on $p.",ch,obj,0,TO_CHAR);

return;
}

void wear_obj_fallen_wings(CHAR_DATA *ch,OBJ_DATA *obj)
{
AFFECT_DATA af;

act("$n's shredded wings slowly beat and $s feet rise of the ground.",ch,0,0,TO_ROOM);
send_to_char("Your shredded angel wings slowly beat and your feet rise off the ground.\n\r",ch);
affect_strip(ch,skill_lookup("fly"));

	init_affect(&af);
	af.where = TO_AFFECTS;
af.aftype = AFT_SKILL;
af.type = skill_lookup("fly");
af.modifier = 0;
af.location = 0;
SET_BIT(af.bitvector, AFF_FLYING);
af.level = 51;
af.duration = -1;
affect_to_char(ch,&af);

return;
}

void remove_obj_fallen_wings(CHAR_DATA *ch,OBJ_DATA *obj)
{
	int sn_fly;
	sn_fly = skill_lookup("fly");
if (!is_affected(ch,sn_fly))
	return;
act("$n slowly floats back to the ground.",ch,0,0,TO_ROOM);
send_to_char("Your feet slowly float back to the ground.\n\r",ch);
affect_strip(ch,sn_fly);
return;
}

void save_cabal_items(void)
{
	OBJ_DATA *obj;
        FILE *fp;
	int i, vnum;
    	if ( ( fp = fopen( CABAL_ITEMS_FILE, "w" ) ) == NULL )
        	return perror( CABAL_ITEMS_FILE );
	for(i = 1; i<MAX_CABAL; i++)
	{
		vnum = cabal_table[i].item_vnum;
		if (vnum == 0) continue;
		for (obj = object_list; obj != NULL; obj = obj->next)
			if(obj->pIndexData->vnum == vnum)
				fprintf(fp,"%i %i\n", i,
				(obj->carried_by && IS_NPC(obj->carried_by) && IS_CABAL_GUARD(obj->carried_by)) ?
				obj->carried_by->pIndexData->vnum : 0);
	}
	fprintf(fp,"-1");
	fclose(fp);
}

bool is_owner(CHAR_DATA *ch,OBJ_DATA *corpse) {
	if (!str_cmp(ch->true_name,corpse->owner)) {
		return TRUE;
	} else {
		return FALSE;
	}
}

void do_roll(CHAR_DATA *ch, char *argument)
{
	char buf[MSL], buf2[MSL];
	OBJ_DATA *obj;
	int i, number, sides;

	if (argument[0] == '\0') {
		act("You roll your eyes in disgust.",ch,0,0,TO_CHAR);
		act("$n rolls $s eyes, disgusted.",ch,0,0,TO_ROOM);
		return;
	}

	if ((obj = get_obj_carry(ch,argument,ch)) == NULL)
		return send_to_char("Roll what?\n\r",ch);

	if (obj->item_type != ITEM_DICE)
		return send_to_char("Those aren't dice!\n\r",ch);

	if (ch->fighting)
		return send_to_char("You really have more important things to worry about right now!\n\r",ch);
	
	if (!IS_GROUND(ch->in_room))
		return send_to_char("You need a hard surface beneath you on which to roll.\n\r",ch);
	
	number = obj->value[0];
	sides = obj->value[1];

	if (number <= 0 || sides <= 1)
		return send_to_char("Something's wrong with these dice, and they can't be rolled.\n\r",ch);
	
	if (number > 1)
		sprintf(buf,"them");
	else
		sprintf(buf,"it");
	
	act("You cup your hands, rattle $p around, and roll $T.",ch,obj,buf,TO_CHAR);
	act("$n cups $s hands, rattles $p around, and rolls $T.",ch,obj,buf,TO_ROOM);

	act(".\n\r.\n\r.",ch,0,0,TO_ALL);
	
	if (number == 1) {
		sprintf(buf,"It rolls a brief distance before coming to a stop, showing a %d.",number_range(1,sides));
	} else {
		sprintf(buf,"They clatter on the ground, coming to a stop showing: ");
		for (i = 0; i < number; i++) {
			sprintf(buf2," %d",number_range(1,sides));
			strcat(buf,buf2);
		}
		strcat(buf,".");
	}
	
	act(buf,ch,0,0,TO_ALL);

	obj_from_char(obj);
	obj_to_room(obj,ch->in_room);
	
	return;
}

void do_flip (CHAR_DATA *ch, char *argument)
{
	char buf[MSL];
	OBJ_DATA *coin;
	
	if (ch->fighting)
		return send_to_char("You really have more important things to worry about right now!\n\r",ch);

	if (argument[0] == '\0') {
		if (!IS_GROUND(ch->in_room))
			return send_to_char("Do a flip without any ground under you?  Poor idea.\n\r",ch);
		if (number_percent() < ((get_curr_stat(ch,STAT_DEX) * 10) - 150)) {
			act("You perform an agile backflip, landing solidly on your feet!",ch,0,0,TO_CHAR);
			act("$n suddenly performs an agile backflip, landling solidly on $s feet!",ch,0,0,TO_ROOM);
			WAIT_STATE(ch,PULSE_VIOLENCE);
			return;
		} else {
			act("You attempt an ill-advised backflip, and crash awkwardly to the ground!",ch,0,0,TO_CHAR);
			act("$n looks foolish as $e attempts a backflip and crashes awkwardly to the ground!",ch,0,0,TO_ROOM);
			WAIT_STATE(ch,3*PULSE_VIOLENCE);
			damage_new(ch,ch,dice(4,4),gsn_bash,DAM_BASH,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"fall");
			ch->position = POS_RESTING;
			return;
		}
	}
	
	if (str_cmp(argument,"coin"))
		return send_to_char("Sorry, you can only flip coins.\n\r",ch);
	
	if (ch->gold <= 0)
		return send_to_char("You haven't even a single coin on your person at the moment!\n\r",ch);

	act("You flip a single gold coin high in the air, and it tumbles rapidly.",ch,0,0,TO_CHAR);
	act("$n flips a single gold coin high in the air, and it tumbles rapidly.",ch,0,0,TO_ROOM);

	act(".\n\r.\n\r.",ch,0,0,TO_ALL);

	if (number_percent() > 50)
		sprintf(buf,"Heads");
	else
		sprintf(buf,"Tails");

	if (number_range(1,25) <= get_curr_stat(ch,STAT_DEX)) {
		act("You catch the coin in your palm and turn it over... $T!",ch,0,buf,TO_CHAR);
		act("$n catches the coin in $s palm and turns it over... $T!",ch,0,buf,TO_ROOM);
	} else {
		if (number_range(1,1000) == 1) {
			act("You fumble trying to catch the coin and it hits the ground. . . \n\r" \
				"Oh my, it landed on its EDGE!  What are the odds??",ch,0,0,TO_CHAR);
			act("$n fumbles trying to catch the coin and it hits the ground. . .\n\r" \
				"Oh my, it landed on its EDGE!  What are the odds??",ch,0,0,TO_ROOM);
		} else {
			act("You fumble trying to catch the coin and it hits the ground, showing... $T!",ch,0,buf,TO_CHAR);
			act("$n fumbles trying to catch the coin and it hits the ground, showing... $T!",ch,0,buf,TO_ROOM);
		}

		coin = create_object(get_obj_index(2),0);
		obj_to_room(coin,ch->in_room);
		ch->gold--;
	}

	return;
}

bool is_carrying(CHAR_DATA *ch, int vnum)
{
	OBJ_DATA *obj, *cont;

	for (obj = ch->carrying; obj; obj = obj->next_content) {
		if (obj->pIndexData->vnum == vnum)
			return TRUE;
					
		for (cont = obj->contains; cont; cont = cont->next_content)
			if (cont->pIndexData->vnum == vnum)
				return TRUE;
	}
						
	return FALSE;
}

bool is_carrying_type(CHAR_DATA *ch, int type)
{
	OBJ_DATA *obj;

	for(obj = ch->carrying; obj; obj = obj->next_content)
		if(obj->item_type == ITEM_BOAT)
			return TRUE;

	return FALSE;
}
