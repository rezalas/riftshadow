#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "merc.h"
#include "magic.h"
#include "recycle.h"
#include "db.h"
#include "lookup.h"
#include "tables.h"

void extract_rune args((RUNE_DATA *rune));
RUNE_DATA *find_rune args((void *vo, int target_type, int trigger_type, RUNE_DATA *rune_prev));
void draw_rune(void *vo, void *vo2);  //attempt to add a rune that's just been initiated
void apply_rune(RUNE_DATA *rune); //make new copy of rune and apply it to target

bool activate_stasis_wall(void *vo,void *vo2,void *vo3,void *vo4);	//list your rune trigs here for good reference
bool trigger_stasis_wall(void *vo,void *vo2,void *vo3,void *vo4);

void spell_stasis_wall( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
	int dir = 0;
	EXIT_DATA *pexit = NULL;
	RUNE_DATA *rune;
	if(target != RUNE_DOOR) //it's been casted, so we don't get the dir handed straight to us (this is pre TAR_DIR)
	{
		if((dir=direction_lookup(target_name))==-1)
			return send_to_char("That's not a valid direction.\n\r",ch);
	}
	else
		dir = (int)*(int*)vo;  //just copy this brutal little casting for converting a void pointer to an integer
	pexit = ch->in_room->exit[dir];
	if (!pexit || !IS_SET(pexit->exit_info, EX_ISDOOR))
		return send_to_char("There must be a door in that direction to place the rune on.\n\r",ch);

	rune = (RUNE_DATA *)talloc_struct(sizeof(rune)); //allocate temporary rune, it'll be discarded after apply_rune anyway
	if(target != RUNE_DOOR)  //if it's not RUNE_DOOR then it's been casted, not runed, so it's immediate
	{
		rune->level 		= level;
		rune->placed_on 	= (EXIT_DATA *) pexit;
		rune->target_type 	= RUNE_TO_PORTAL;	//grep for this too
		rune->owner		= ch;
		rune->trigger_type	= RUNE_TRIGGER_EXIT;	//grep for rune trigger types if you need to
		rune->type		= sn;
		rune->duration		= level / 10;
		rune->end_fun		= NULL;
		rune->function		= trigger_stasis_wall;  //this is what's called when the rune is triggered
		apply_rune(rune);				//this will create a new copy of the talloced rune
		act("$n gestures and an immovable barrier snaps into existence to the $t!",
			ch,direction_table[dir].name,0,TO_ROOM);
		act("You gesture and a stasis wall forms to the $t!",ch,direction_table[dir].name,0,TO_CHAR);
		return;
	}
	rune->level		= level;
	rune->placed_on		= (ROOM_INDEX_DATA *) ch->in_room;
	rune->target_type	= RUNE_TO_ROOM;
	rune->owner		= ch;
	rune->trigger_type 	= RUNE_TRIGGER_ENTRY;
	rune->type		= sn;
	rune->duration		= level / 10;
	rune->extra		= dir;
	rune->end_fun		= NULL;
	rune->drawn_in		= ch->in_room->vnum;
	rune->function		= activate_stasis_wall;
	RS.Queue.AddToQueue(9, 1, draw_rune,rune);	//this checks lose_conc and stuff before finalizing the rune,  
						//usually make sure the lag on the rune
						//is at least as long as the lag on the draw_rune queue
}


bool trigger_stasis_wall(void *vo,void *vo2,void *vo3,void *vo4)
{
	RUNE_DATA *rune = (RUNE_DATA *) vo;
	CHAR_DATA *victim = (CHAR_DATA *) vo2, *ch = rune->owner;
	int dir = (int)*(int*)vo3;
	if(!rune->owner)
		return FALSE;
	if(is_safe_new(ch,victim,FALSE))
		return FALSE;
	act("You try to move to the $t but a stasis wall flickers and holds you back!",victim,direction_table[dir].name,0,TO_CHAR);
	act("$n tries to move to the $t but is halted by a stasis wall!",victim,direction_table[dir].name,0,TO_ROOM);
        return TRUE;
}

bool activate_stasis_wall(void *vo,void *vo2,void *vo3,void *vo4)
{
	RUNE_DATA *rune = (RUNE_DATA *) vo, new_rune;
	CHAR_DATA *victim = (CHAR_DATA *) vo2, *ch = rune->owner;
	int dir = reverse_d((int)*(int*)vo3);
	EXIT_DATA *pexit = ch->in_room->exit[dir];
	if(!rune->owner || rune->owner == victim || dir != rune->extra || !pexit)
		return FALSE;
	if(is_safe_new(ch,victim,FALSE))
		return FALSE;
	act("As you pass through the $t, a stasis wall snaps into existence behind you!",victim,pexit->keyword,0,TO_CHAR);
	act("As $n passes through the $t, a stasis wall snaps into existence behind $m!",victim,pexit->keyword,0,TO_ROOM);
        new_rune.level              = rune->level;
        new_rune.placed_on          = (EXIT_DATA *) pexit;
        new_rune.target_type        = RUNE_TO_PORTAL;
        new_rune.owner              = rune->owner;
        new_rune.trigger_type       = RUNE_TRIGGER_EXIT;
        new_rune.type               = rune->type;
        new_rune.duration           = rune->level / 10;
        new_rune.end_fun            = NULL;
        new_rune.function           = trigger_stasis_wall;
	apply_rune(&new_rune);
	return FALSE;
}

void draw_rune(void *vo, void *vo2)
{
        RUNE_DATA *rune = (RUNE_DATA *) vo;
        CHAR_DATA *ch = rune->owner;
        if(ch->in_room->vnum != rune->drawn_in)
        {
                send_to_char("A backlash of energy whips through you as your uncompleted rune overloads!\n\r",ch);
                damage_new(ch,ch,dice(rune->level,4),TYPE_UNDEFINED,DAM_ENERGY,TRUE,HIT_UNBLOCKABLE,0,1,"mana surge");
                return;
        }
        if(number_percent()>get_skill(ch,rune->type))
        {
                act("The rune flares brightly before vanishing!",ch,0,0,TO_ROOM);
                send_to_char("The improperly scribed rune flares brightly before vanishing!\n\r",ch);
                return;
        }
        act("The rune flares $t!",ch,skill_table[rune->type].msg_off,0,TO_ALL);
        apply_rune(rune);
}

void do_rune( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH], arg2[MAX_INPUT_LENGTH], ttype[MSL];
    OBJ_DATA *obj;
    EXIT_DATA *pexit = NULL;
    void *vo;
    int mana, where, sn, target=0;

    if ( IS_NPC(ch) && ch->desc == NULL)
		return;
    if(ch->Class()->ctype!=CLASS_CASTER && !IS_IMMORTAL(ch))
		send_to_char("You do not know how to create runes.\n\r",ch);
    if (IS_SET(ch->act,PLR_BETRAYER))
		return send_to_char("You feel cut off from your source of power.\n\r",ch);
    if(!IS_NPC(ch) && ch->pcdata->oalign==3 && ch->alignment==-1000)
		return send_to_char("You feel cut off from your source of power.\n\r",ch);
    if (!IS_NPC(ch) && ch->pcdata->energy_state>2)
		return send_to_char("You cannot concentrate enough to complete the incantation.\n\r",ch);
    argument = one_argument(argument, ttype);
    target_name = one_argument( argument, arg1 );
    one_argument( target_name, arg2 );

    if ( arg1[0] == '\0' )
		return send_to_char( "Syntax:\n\rrune   <type> <name> <target>\n\r", ch );

    if ((sn = find_spell(ch,arg1)) < 1
    ||  skill_table[sn].spell_fun == spell_null
    || (!IS_NPC(ch) && get_skill(ch,sn) < 5)
    || (!IS_NPC(ch) && ch->pcdata->learned[sn] == 0))
		return send_to_char( "You don't know any runes of that name.\n\r", ch );
    if ( ch->position < skill_table[sn].minimum_position )
		return send_to_char( "You can't concentrate enough.\n\r", ch );
    if((skill_table[sn].ctype==CMD_COMMUNE || skill_table[sn].ctype==CMD_POWER || skill_table[sn].ctype == CMD_SPELL) && 
			!(skill_table[sn].target & RUNE_CAST) && !IS_IMMORTAL(ch))
		return send_to_char("You can't draw that rune.\n\r",ch);
    if (ch->level + 2 == skill_table[sn].skill_level[ch->Class()->GetIndex()])
	mana = 50;
    else
    	mana = UMAX( skill_table[sn].min_mana, 100 / ( 2 + ch->level - skill_table[sn].skill_level[ch->Class()->GetIndex()] ) );
    obj		= NULL;
    vo		= NULL;
    if(!str_prefix(ttype,"armor"))
    {
        if(!(skill_table[sn].dispel & RUNE_ARMOR))
		return send_to_char("You can't place that type of rune on that.\n\r",ch);
	target = RUNE_ARMOR;
    }
    else if(!str_prefix(ttype,"weapon"))
    {
        if(!(skill_table[sn].dispel & RUNE_WEAPON))
                return send_to_char("You can't place that type of rune on that.\n\r",ch);
        target = RUNE_WEAPON;
    }
    else if(!str_prefix(ttype,"portal"))
    {
        if(!(skill_table[sn].dispel & RUNE_DOOR))
                return send_to_char("You can't place that type of rune on that.\n\r",ch);
        target = RUNE_DOOR;
    }
    else
    {
        if(!(skill_table[sn].dispel & RUNE_ROOM))
                return send_to_char("You can't place that type of rune in here.\n\r",ch);
        target = RUNE_ROOM;
    }
    if(target == RUNE_WEAPON || target == RUNE_ARMOR)
    {
	if ( arg2[0] == '\0' )
		return send_to_char( "What should the rune be placed upon?\n\r", ch );
	if ( ( obj = get_obj_carry( ch, target_name, ch ) ) == NULL )
		return send_to_char( "You are not carrying that.\n\r", ch );
	vo = (void *) obj;
        act("$n carefully begins to scribe an intricate rune on $p.",ch,obj,0,TO_ROOM);
        act("You carefully begin to scribe an intricate rune on $p.",ch,obj,0,TO_CHAR);
    }
    else if(target == RUNE_DOOR)
    {
	if((where = direction_lookup(target_name))<0 || !(pexit=ch->in_room->exit[where]))
		return send_to_char("That's not a valid direction.\n\r",ch);
	vo = &where;
        act("$n carefully begins to scribe an intricate rune on the $t.",ch,strcmp(pexit->keyword,"") ? pexit->keyword : "door",0,TO_ROOM);
        act("You carefully begin to scribe an intricate rune on the $t.",ch,strcmp(pexit->keyword,"") ? pexit->keyword : "door",0,TO_CHAR);
    }
    else if(target == RUNE_ROOM)
    {
	vo = (ROOM_INDEX_DATA *) ch->in_room;
        act("$n moves $s hands slowly, tracing a glowing rune in midair.",ch,0,0,TO_ROOM);
        act("Slowly tracing a glowing pattern in front of you, you create a rune in midair.",ch,pexit->keyword,0,TO_CHAR);
    }
    else
	return;	
    if ( !IS_NPC(ch) && ch->mana < mana )
	return send_to_char( "You don't have enough mana.\n\r", ch );

    WAIT_STATE( ch, skill_table[sn].beats );

        ch->mana -= mana;
	if (IS_SET(ch->in_room->room_flags,ROOM_NO_MAGIC) && !(ch->level > LEVEL_HERO))
	{
		act("$n's spell fizzles.",ch,0,0,TO_ROOM);
		send_to_char("Your spell fizzles and dies.\n\r",ch);
		return;
	}
        (*skill_table[sn].spell_fun) ( sn, ch->level*2, ch, vo, target);
        check_improve(ch,sn,TRUE,1);

    return;
}


RUNE_DATA *find_rune(void *vo, int target_type, int trigger_type, RUNE_DATA *rune_prev)
{
	RUNE_DATA *rune = NULL, *rune_loop;
	switch(target_type)
	{
		case RUNE_TO_WEAPON:
		case RUNE_TO_ARMOR:
			rune = ((OBJ_DATA *) vo)->rune;
		break;
		case RUNE_TO_PORTAL:
			rune = ((EXIT_DATA *) vo)->rune;
		break;
		case RUNE_TO_ROOM:
			rune = ((ROOM_INDEX_DATA *) vo)->rune;
		break;
	}
        if(rune_prev && !rune_prev->next_content)
		return NULL;
	if(rune_prev)
		rune = rune_prev->next_content;
	for(rune_loop = rune; rune_loop; rune_loop = rune_loop->next_content)
		if(rune_loop->trigger_type == trigger_type)
			return rune_loop;
	return NULL;
}

void extract_rune(RUNE_DATA *rune)
{
	OBJ_DATA *obj; EXIT_DATA *exit; ROOM_INDEX_DATA *room;
	RUNE_DATA *rune_prev;
	switch(rune->target_type)
	{
		case RUNE_TO_WEAPON:
		case RUNE_TO_ARMOR:
			obj = (OBJ_DATA *) rune->placed_on;
			if(obj->has_rune && rune->next_content)
			{       obj->rune = rune->next_content; break; }
			obj->has_rune = FALSE;
		break;
		case RUNE_TO_PORTAL:
			exit = (EXIT_DATA *) rune->placed_on;
			if(exit->has_rune && rune->next_content)
			{	exit->rune = rune->next_content; break; }
			exit->has_rune = FALSE;
		break;
		case RUNE_TO_ROOM:
			room = (ROOM_INDEX_DATA *) rune->placed_on;
			if(room->has_rune && rune->next_content)
			{       room->rune = rune->next_content; break; }
			room->has_rune = FALSE;
		break;
	}
	if(rune_list == rune)
		rune_list = rune_list->next;
	else
	{
		for(rune_prev = rune_list; rune_prev; rune_prev = rune_prev->next)
			if(rune_prev->next == rune)
			{
				rune_prev->next = rune->next;
				break;
			}
	}
	free_rune(rune);	
}
void apply_rune(RUNE_DATA *rune)
{
	RUNE_DATA *rune_new;
	OBJ_DATA *obj; EXIT_DATA *pexit; ROOM_INDEX_DATA *room;
	rune_new 	= new_rune();
	*rune_new 	= *rune;
	rune_new->next	= rune_list;
	rune_list	= rune_new;
	rune_new->next_content = NULL;

	switch(rune_new->target_type)
	{
		case RUNE_TO_WEAPON:
		case RUNE_TO_ARMOR:
			obj = (OBJ_DATA *) rune_new->placed_on;
			if(obj->has_rune)
				rune_new->next_content = obj->rune;
			obj->rune = rune_new;
			obj->has_rune = TRUE;
		break;
		case RUNE_TO_PORTAL:
			pexit = (EXIT_DATA *) rune_new->placed_on;
			if(pexit->has_rune)
				rune_new->next_content = pexit->rune;
			pexit->rune = rune_new;
			pexit->has_rune = TRUE;
		break;
		case RUNE_TO_ROOM:
			room = (ROOM_INDEX_DATA *) rune_new->placed_on;
			if(room->has_rune)
				rune_new->next_content = room->rune;
			room->rune = rune_new;
			room->has_rune = TRUE;
		break;
	}
}
