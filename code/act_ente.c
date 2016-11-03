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
*	    Russ Taylor (rtaylor@efn.org)				   *
*	    Gabrielle Taylor						   *
*	    Brian Moore (zump@rom.org)					   *
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

/* command procedures needed */
DECLARE_DO_FUN(do_look		);
DECLARE_DO_FUN(do_stand		);
DECLARE_SPELL_FUN(spell_teleport);

/* random room generation procedure */
ROOM_INDEX_DATA  *get_random_room(CHAR_DATA *ch)
{
    ROOM_INDEX_DATA *room;

    for ( ; ; )
    {
        room = get_room_index( number_range( 0, 65535 ) );
        if ( room != NULL )
        if ( can_see_room(ch,room)
	&&   !room_is_private(room)
   	&&   !IS_SET(room->room_flags, ROOM_PRIVATE)
  	&&   !IS_SET(room->room_flags, ROOM_SOLITARY)
	&&   !IS_SET(room->room_flags, ROOM_SAFE)
	&&	 !IS_SET(room->room_flags, ROOM_HEROES_ONLY)
	&&	 !IS_SET(room->room_flags, ROOM_GODS_ONLY)
	&&   !IS_SET(room->room_flags, ROOM_NO_GATE)) {
			if (!IS_NPC(ch)) {
				break;
			} else {
				if (!(IS_SET(ch->act,ACT_AGGRESSIVE) &&
					!IS_SET(room->room_flags,ROOM_LAW)))
	        	    break;
			}
		}
	}

    return room;
}

/* RT Enter portals */
void do_enter( CHAR_DATA *ch, char *argument)
{
    ROOM_INDEX_DATA *location, *to_room;
    if ( ch->fighting != NULL )
	return;
    /* nifty portal stuff */
    if (argument[0] != '\0')
    {
        ROOM_INDEX_DATA *old_room;
	OBJ_DATA *portal, *obj;
	CHAR_DATA *fch, *fch_next;

        old_room = ch->in_room;

	portal = get_obj_list( ch, argument,  ch->in_room->contents );
	
	if (portal == NULL)
	{
	    send_to_char("You don't see that here.\n\r",ch);
	    return;
	}

	if (portal->item_type != ITEM_PORTAL
        ||  (IS_SET_OLD(portal->value[1],EX_CLOSED) && !IS_TRUSTED(ch,ANGEL)))
	{
	    send_to_char("You can't seem to find a way in.\n\r",ch);
	    return;
	}

	if (!IS_TRUSTED(ch,ANGEL) && !IS_SET_OLD(portal->value[2],GATE_NOCURSE)
	&&  (IS_AFFECTED(ch,AFF_CURSE)
	||   IS_SET(old_room->room_flags,ROOM_NO_RECALL)))
	{
	    send_to_char("Something prevents you from leaving...\n\r",ch);
	    return;
	}

	if (IS_SET_OLD(portal->value[2],GATE_RANDOM) || portal->value[3] == -1 ||
		(IS_SET_OLD(portal->value[2],GATE_BUGGY) && number_percent() < 5))
	{
		send_to_char("You enter the portal and find yourself elsewhere.\n\r",ch);
		act("$n steps into $p.",ch,portal,0,TO_ROOM);	
		spell_teleport(skill_lookup("teleport"),ch->level,ch,ch,TAR_CHAR_SELF);
		return;
	}
        else
            location = get_room_index(portal->value[3]);

	if (location == NULL
	||  location == old_room
	||  !can_see_room(ch,location)
	||  (room_is_private(location) && !IS_TRUSTED(ch,IMPLEMENTOR)))
	{
	   act("$p doesn't seem to go anywhere.",ch,portal,NULL,TO_CHAR);
	   return;
	}

        if (IS_NPC(ch) && IS_SET(ch->act,ACT_AGGRESSIVE)
        &&  IS_SET(location->room_flags,ROOM_LAW))
        {
            send_to_char("Something prevents you from leaving...\n\r",ch);
            return;
        }

	act("$n steps into $p.",ch,portal,NULL,TO_ROOM);
	
	if (IS_SET_OLD(portal->value[2],GATE_NORMAL_EXIT))
	    act("The world bends around you as you enter $p.",ch,portal,NULL,TO_CHAR);
	else
	    act("You walk through $p and find yourself somewhere else...",
	        ch,portal,NULL,TO_CHAR);

	char_from_room(ch);
	char_to_room(ch, location);

	if (IS_SET_OLD(portal->value[2],GATE_GOWITH)) /* take the gate along */
	{
	    obj_from_room(portal);
	    obj_to_room(portal,location);
	}

	if (IS_SET_OLD(portal->value[2],GATE_NORMAL_EXIT))
	    act("$n has arrived.",ch,portal,NULL,TO_ROOM);
	else
	    act("$n has arrived through $p.",ch,portal,NULL,TO_ROOM);

	do_look(ch,"auto");

	/* charges */
	if (portal->value[0] > 0)
	{
	    portal->value[0]--;
	}

	/* protect against circular follows */
	if (old_room == location)
	    return;
	to_room = location;

        for (fch = to_room->people; fch != NULL; fch = fch_next)
        {
                fch_next = fch->next_in_room;
        /* greet trigger for items carried by people in room */
	        for (obj = fch->carrying; obj != NULL; obj = obj->next_content)
                	if (IS_SET(obj->progtypes,IPROG_GREET))
                        	(obj->pIndexData->iprogs->greet_prog) (obj,ch);
      /* Greet trigger for mobs  */
      		if (IS_NPC(fch) && IS_SET(fch->progtypes,MPROG_GREET))
        		(fch->pIndexData->mprogs->greet_prog) (fch,ch);
        }

        for (obj = ch->carrying; obj != NULL; obj = obj->next_content)
                if (IS_SET(obj->progtypes,IPROG_ENTRY))
                        (obj->pIndexData->iprogs->entry_prog) (obj);

        if (IS_SET(to_room->progtypes, RPROG_ENTRY))
                (to_room->rprogs->entry_prog) (to_room, ch);

    	for ( fch = old_room->people; fch != NULL; fch = fch_next )
    	{
            fch_next = fch->next_in_room;

            if (portal == NULL || portal->value[0] == 0)
	    /* no following through dead portals */
                continue;

            if ( fch->master == ch && IS_AFFECTED(fch,AFF_CHARM)
            &&   fch->position < POS_STANDING)
            	do_stand(fch,"");

            if ( fch->master == ch && fch->position == POS_STANDING)
            {

                if (IS_SET(ch->in_room->room_flags,ROOM_LAW)
                &&  (IS_NPC(fch) && IS_SET(fch->act,ACT_AGGRESSIVE)))
                {
                    act("You can't bring $N into the city.",
                    	ch,NULL,fch,TO_CHAR);
                    act("You aren't allowed in the city.",
                    	fch,NULL,NULL,TO_CHAR);
                    continue;
            	}

            	act( "You follow $N.", fch, NULL, ch, TO_CHAR );
		do_enter(fch,argument);
            }
    	}

 	if (portal != NULL && portal->value[0] == 0)
	{
	    act("$p shimmers and rapidly closes behind you.",ch,portal,NULL,TO_CHAR);
	    if (ch->in_room == old_room)
		act("$p flares brilliantly and fades away.",ch,portal,NULL,TO_ROOM);
	    else if (old_room->people != NULL)
	    {
		act("$p flares brilliantly and fades away.",
		    old_room->people,portal,NULL,TO_CHAR);
		act("$p flares brilliantly and fades away.",
		    old_room->people,portal,NULL,TO_ROOM);
	    }
	    extract_obj(portal);
	}
	return;
    }

    send_to_char("Nope, can't do it.\n\r",ch);
    return;
}
