/* IMPROGS Expansion: ROOM PROGS (Eladrian) */

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "magic.h"
#include "interp.h"
#include "recycle.h"
#include "tables.h"
#include "room.h"
#include "rift.h"

DECLARE_RPROG_FUN_PULSE( pulse_prog_cimar_sewergrate );
DECLARE_RPROG_FUN_SPEECH( speech_prog_ilopheth_shack );
DECLARE_RPROG_FUN_PULSE(pulse_prog_cim_conv);
DECLARE_RPROG_FUN_ENTRY(entry_prog_ilopheth_flute);
DECLARE_RPROG_FUN_ENTRY(entry_prog_sidhe_ankle);
DECLARE_RPROG_FUN_OPEN(open_prog_mudschool_key);
DECLARE_RPROG_FUN_MOVE(move_prog_mudschool_key);
DECLARE_RPROG_FUN_MOVE(move_prog_door_close);
DECLARE_RPROG_FUN_PULSE(pulse_prog_mudschool_snake);
DECLARE_RPROG_FUN_PULSE(pulse_prog_elven_star);
DECLARE_RPROG_FUN_OPEN(open_prog_bust_room);
DECLARE_RPROG_FUN_OPEN(open_prog_nodoor);
DECLARE_RPROG_FUN_MOVE(move_prog_stone_roll);
DECLARE_RPROG_FUN_MOVE(move_prog_horde_shrine);
DECLARE_RPROG_FUN_ENTRY(entry_prog_iseldheim_lift);
DECLARE_RPROG_FUN_DROP(drop_prog_elven_star);
DECLARE_RPROG_FUN_SPEECH(speech_prog_elven_down);

const	struct	improg_type	rprog_table[] =
{
	{	"pulse_prog",	"pulse_prog_cimar_sewergrate",		(void*)pulse_prog_cimar_sewergrate,	"None"},
	{	"speech_prog",	"speech_prog_ilopheth_shack",		(void*)speech_prog_ilopheth_shack,		"None"},
	{	"speech_prog",	"speech_prog_elven_down",		(void*)speech_prog_elven_down,		"None"},
	{	"pulse_prog",	"pulse_prog_cim_conv",			(void*)pulse_prog_cim_conv,		"Cimar"},
	{	"pulse_prog",	"pulse_prog_elven_star",		(void*)pulse_prog_elven_star,		"None"},
	{	"entry_prog",	"entry_prog_ilopheth_flute",		(void*)entry_prog_ilopheth_flute,		"Ilopheth" },
	{	"entry_prog",	"entry_prog_sidhe_ankle",		(void*)entry_prog_sidhe_ankle,		"Sidhe Village" },
	{	"open_prog",	"open_prog_mudschool_key",		(void*)open_prog_mudschool_key,		"None" },
	{	"move_prog",	"move_prog_mudschool_key",		(void*)move_prog_mudschool_key,		"None" },
	{	"move_prog",	"move_prog_door_close",			(void*)move_prog_door_close,			"None" },
	{	"move_prog",	"move_prog_horde_shrine",		(void*)move_prog_horde_shrine,			"None" },
	{	"move_prog",	"move_prog_stone_roll",			(void*)move_prog_stone_roll,			"None" },
	{	"pulse_prog",	"pulse_prog_mudschool_snake",		(void*)pulse_prog_mudschool_snake,		"None" },
	{	"open_prog",	"open_prog_bust_room",			(void*)open_prog_bust_room,			"None" },
	{	"open_prog",	"open_prog_nodoor",			(void*)open_prog_nodoor,			"None" },
	{	"entry_prog",	"entry_prog_iseldheim_lift",		(void*)entry_prog_iseldheim_lift,		"None" },
	{	"drop_prog",	"drop_prog_elven_star",			(void*)drop_prog_elven_star,			"None" },
	{	NULL,			NULL,								NULL					, NULL},
};

bool rprog_unset(ROOM_INDEX_DATA *room, const char *progtype, const char *name)
{
	int i;
	for (i = 0; rprog_table[i].name; i++)
		if (!str_cmp(rprog_table[i].name, name))
			break;

	if (!rprog_table[i].name)
		return FALSE;

	//IF YOU ADD A NEW TYPE ALSO PUT IT IN DB.C DO_ADUMP
	if (!str_cmp(progtype, "pulse_prog"))
	{
		room->rprogs->pulse_prog = NULL;
		free_pstring(room->rprogs->pulse_name);
		REMOVE_BIT(room->progtypes, RPROG_PULSE);
	}
	else if (!str_cmp(progtype, "entry_prog"))
	{
		room->rprogs->entry_prog = NULL;
		free_pstring(room->rprogs->entry_name);
		REMOVE_BIT(room->progtypes, RPROG_ENTRY);
	}
	else if (!str_cmp(progtype, "move_prog"))
	{
		room->rprogs->move_prog = NULL;
		free_pstring(room->rprogs->move_name);
		REMOVE_BIT(room->progtypes, RPROG_MOVE);
	}
	else if (!str_cmp(progtype, "drop_prog"))
	{
		room->rprogs->drop_prog = NULL;
		free_pstring(room->rprogs->drop_name);
		REMOVE_BIT(room->progtypes, RPROG_DROP);
	}
	else if (!str_cmp(progtype, "speech_prog"))
	{
		room->rprogs->speech_prog = NULL;
		free_pstring(room->rprogs->speech_name);
		REMOVE_BIT(room->progtypes, RPROG_SPEECH);
	}
	else if (!str_cmp(progtype, "open_prog"))
	{
		room->rprogs->open_prog = NULL;
		free_pstring(room->rprogs->open_name);
		REMOVE_BIT(room->progtypes, RPROG_OPEN);
	}
	return TRUE;
}

void rprog_set(ROOM_INDEX_DATA *room, const char *progtype, const char *name)
{
	int i;
	for (i = 0; rprog_table[i].name; i++)
		if (!str_cmp(rprog_table[i].name, name))
			break;
	if (!rprog_table[i].name)
		return bug("Lood_improgs: 'R': Function not found for vnum %d",room->vnum);

	//IF YOU ADD A NEW TYPE ALSO PUT IT IN DB.C DO_ADUMP
	if (!str_cmp(progtype, "pulse_prog"))
		{
			room->rprogs->pulse_prog = (RPROG_FUN_PULSE *)rprog_table[i].function;
				free_pstring(room->rprogs->pulse_name);
			room->rprogs->pulse_name = palloc_string(name);
			SET_BIT(room->progtypes, RPROG_PULSE);
			return;
		}
	if (!str_cmp(progtype, "entry_prog"))
		{
			room->rprogs->entry_prog = (RPROG_FUN_ENTRY *)rprog_table[i].function;
				free_pstring(room->rprogs->entry_name);
			room->rprogs->entry_name = palloc_string(name);
			SET_BIT(room->progtypes, RPROG_ENTRY);
			return;
		}
	if (!str_cmp(progtype, "move_prog"))
		{
			room->rprogs->move_prog = (RPROG_FUN_MOVE *)rprog_table[i].function;
			if(room->progtypes)
			free_pstring(room->rprogs->move_name);
			room->rprogs->move_name = palloc_string(name);
			SET_BIT(room->progtypes, RPROG_MOVE);
			return;
		}
	if (!str_cmp(progtype, "drop_prog"))
		{
			room->rprogs->drop_prog = (RPROG_FUN_DROP *)rprog_table[i].function;
				free_pstring(room->rprogs->drop_name);
			room->rprogs->drop_name = palloc_string(name);
			SET_BIT(room->progtypes, RPROG_DROP);
			return;
		}
	if (!str_cmp(progtype, "speech_prog"))
	{
		room->rprogs->speech_prog = (RPROG_FUN_SPEECH *)rprog_table[i].function;
			free_pstring(room->rprogs->speech_name);
		room->rprogs->speech_name = palloc_string(name);
		SET_BIT(room->progtypes, RPROG_SPEECH);
		return;
	}
	if(!str_cmp(progtype, "open_prog"))
	{
		room->rprogs->open_prog = (RPROG_FUN_OPEN *)rprog_table[i].function;
			free_pstring(room->rprogs->open_name);
		room->rprogs->open_name = palloc_string(name);
		SET_BIT(room->progtypes, RPROG_OPEN);
		return;
	}
	bug ("Load_improgs: 'R': invalid program type for vnum %d", room->vnum);
	exit(1);
}

void pulse_prog_cimar_sewergrate(ROOM_INDEX_DATA *room)
{
	if (number_percent() > 7)
		return;

	if (!room->people)
		return;

	act("The sewer grate emits a soft burbling sound, and a stench fills the air.",room->people,0,0,TO_ALL);

	return;
}

void pulse_prog_cim_conv(ROOM_INDEX_DATA *room)
{
	if(number_percent() != 1)
		return;
	
	if(!room->people)
		return;
	
	act("Conversations buffet you, and your eyes are drawn to a sign on the wall.",room->people,0,0,TO_ALL);

	return;
}

void entry_prog_ilopheth_flute(ROOM_INDEX_DATA *room, CHAR_DATA *ch)
{
	ROOM_INDEX_DATA *cliff, *adj;
	CHAR_DATA *man = NULL, *vch;
	char buf[MSL];

	cliff = get_room_index(9121);
	adj = get_room_index(9120);

	for (vch = cliff->people; vch; vch = vch->next_in_room) {
		if (!IS_NPC(vch))
			continue;
		if (vch->pIndexData->vnum == 9010) {
			man = vch;
			break;
		}
	}

	if (!man)
		return;

	if (room == cliff) {
		sprintf(buf,"%sThe weathered old man ceases his flute-playing and regards you intently.%s",get_char_color(ch,"magenta"),END_COLOR(ch));
		act(buf,man,0,ch,TO_VICT);
	} else if (room == adj) {
		sprintf(buf,"%sA haunting melody echoes from the north, its rich tones exuding melancholy and grief.%s",get_char_color(ch,"magenta"),END_COLOR(ch));
		act(buf,ch,0,0,TO_CHAR);
	}

	return;
}

void entry_prog_sidhe_ankle(ROOM_INDEX_DATA *room, CHAR_DATA *ch)
{
	AFFECT_DATA af;
	
	if (IS_AFFECTED(ch, AFF_FLYING))
		return;

	if (IS_NPC(ch) || IS_IMMORTAL(ch))
		return;

	if (number_percent() < 60)
		return;

	send_to_char("You feel a sharp pain in your ankle as your foot slips on a loose rock.\n\r",ch);
	act("$n winces in pain as $e twists his ankle on a loose rock.",ch,0,0,TO_ROOM);

	damage_new(ch,ch,dice(ch->level,2),TYPE_UNDEFINED,DAM_BASH,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"twisted ankle");

	if (number_percent() < 20 && ch->legs > 0)
	{
		send_to_char("You feel a brutal *crack* as your weight comes down on your twisted ankle.\n\r",ch);
		
		init_affect(&af);
		af.where = TO_AFFECTS;
		af.duration = 12;
		af.type = gsn_hobble;
		af.level = ch->level;
		af.aftype   = AFT_MALADY;
		af.location = APPLY_DEX;
		af.modifier = -4;
		affect_to_char(ch,&af);
		
		af.location = APPLY_LEGS;
		af.modifier = -1;
		af.aftype = AFT_INVIS;
		affect_to_char(ch,&af);
	}

	return;
}

bool open_prog_mudschool_key(ROOM_INDEX_DATA *room, CHAR_DATA *ch, EXIT_DATA *exit)
{
	OBJ_DATA *obj,*obj2;
	bool found = FALSE;


	for(obj2 = ch->carrying; obj2 != NULL; obj2 = obj2->next_content) {
		if(obj2->pIndexData->vnum == 24596) {
			found = TRUE;
			break;
		}
	}
	
	if(found)
		return TRUE;

	if(!IS_SET(exit->exit_info,EX_LOCKED))
		return TRUE;

	obj = create_object(get_obj_index(24596),1);
	send_to_char("It's locked...\n\r",ch);
	obj_to_char(obj,ch);
	act("With a sudden flash of lavender light, a pudgy hand pops into existence and gives you a key!",ch,0,0,TO_CHAR);
	act("With a sudden flash of lavender light, a pudgy hand pops into existence gives $n a key!",ch,0,0,TO_ROOM);

	return FALSE;
}

bool move_prog_mudschool_key(ROOM_INDEX_DATA *room, CHAR_DATA *ch, int dir)
{
	EXIT_DATA *exit = room->exit[dir];
	OBJ_DATA *obj;
	bool found = FALSE;

	for(obj = ch->carrying; obj != NULL; obj = obj->next) {
		if(obj->pIndexData->vnum == 24596) {
			found = TRUE;
			break;
		}
	}

	if(IS_SET(exit->exit_info,EX_CLOSED))
		return FALSE;

    SET_BIT(exit->exit_info,EX_CLOSED);
    SET_BIT(exit->exit_info,EX_LOCKED);
	
	if(found) {
                act("As you pass through the door, $p crumbles to dust.",ch,obj,0,TO_CHAR);
		obj_from_char(obj);
		extract_obj(obj);
	}
	
	if(room->people)
		act("The eastern door slams shut.",room->people,0,0,TO_ROOM);
	

	return TRUE;
}

bool move_prog_door_close(ROOM_INDEX_DATA *room, CHAR_DATA *ch, int dir)
{
	EXIT_DATA *exit = room->exit[dir];

	if(IS_SET(exit->exit_info,EX_CLOSED))
		return TRUE;

	act("The door to the north closes up behind you, locking with a tiny click.",ch,0,0,TO_CHAR);
	act("The door to the south closes up behind $n, locking with a tiny click.",ch,0,0,TO_ROOM);    
	SET_BIT(exit->exit_info,EX_CLOSED);
	SET_BIT(exit->exit_info,EX_LOCKED);
	SET_BIT(exit->exit_info,EX_NONOBVIOUS);

	return TRUE;
}

void pulse_prog_mudschool_snake(ROOM_INDEX_DATA *room)
{
	CHAR_DATA *ch, *snake;
	ROOM_AFFECT_DATA af;
	AFFECT_DATA paf;
	
	if(!room->people)
		return;

	if(is_affected_room(room,gsn_bash))
		return;

	if(number_percent() > 90)
		return;
	
	for(ch = room->people; ch != NULL; ch = ch->next_in_room)
	{
		if(is_affected(ch,gsn_trip))
			continue;
	}

	if(ch == NULL)
		return;

	snake = create_mobile(get_mob_index(24546));
	char_to_room(snake, room);
	act("Sensing warmth and moisture, a rootsnake bursts from a nearby root and attacks you!",ch,0,0,TO_CHAR);
	act("Sensing warmth and moisture, a rootsnake bursts from a nearby root and attacks $n!",ch,0,0,TO_ROOM);

	do_murder(snake,ch->name);

	init_affect_room(&af);
	af.where = TO_ROOM_AFFECTS;
	af.type = gsn_bash;
	af.aftype = AFT_SKILL;
	af.duration = 3;
	af.modifier = 0;
	af.location = 0;
	new_affect_to_room(room,&af);

	init_affect(&paf);
	paf.where = TO_AFFECTS;
	paf.type = gsn_trip;
	paf.aftype = AFT_INVIS;
	paf.duration = 4;
	affect_to_char(ch,&paf);
	return;
}


bool open_prog_bust_room(ROOM_INDEX_DATA *room, CHAR_DATA *ch, EXIT_DATA *exit)
{
	send_to_char("You see no door south here.\n\r",ch);
	
	return FALSE;
}

bool open_prog_nodoor(ROOM_INDEX_DATA *room, CHAR_DATA *ch, EXIT_DATA *exit)
{
	int dir;
	char buf[MSL];

	for(dir = 0; dir < MAX_EXIT; dir++) {
	 	 if(room->exit[dir] == exit)
	 	 	break;
	}

	sprintf(buf,"You see no door %s here.\n\r",dir_name[dir]);
	send_to_char(buf,ch);
	return FALSE;
}
	
bool move_prog_stone_roll(ROOM_INDEX_DATA *room, CHAR_DATA *ch, int dir)
{
	ROOM_INDEX_DATA *room2 = get_room_index(24559);
	EXIT_DATA *exit = room2->exit[DIR_EAST];
	OBJ_DATA *obj, *corpse;
	bool found = FALSE;

	if(IS_SET(exit->exit_info,EX_CLOSED))
		return TRUE;

	SET_BIT(exit->exit_info,EX_CLOSED);
	SET_BIT(exit->exit_info,EX_LOCKED);
	SET_BIT(exit->exit_info,EX_NONOBVIOUS);

	act("As you pass through, the stone rolls violently back into place!",ch,0,0,TO_CHAR);

	for(corpse = room->contents; corpse != NULL; corpse = corpse->next) {
		if(corpse->pIndexData->vnum == 24558) {
			found = TRUE;
			break;
			}
	}

	if(!found) {
		bug("Corpse in 24560 missing.  Odd.",0);
		return TRUE;
	}

	if(corpse->contains)
		return TRUE;

	obj = create_object(get_obj_index(24560),10);
	obj_to_obj(obj,corpse);
	return TRUE; 
}

bool move_prog_horde_shrine(ROOM_INDEX_DATA *room, CHAR_DATA *ch, int dir)
{
	if(dir == DIR_SOUTH && !is_affected(ch,gsn_horde_communion)) {
		send_to_char("Alas, you cannot go that way.\n\r",ch);
		return FALSE;
	}

	if(dir == DIR_SOUTH && is_affected(ch,gsn_horde_communion)) {
		send_to_char("With a muted hush, the bushes part easily, allowing you passage to the south.\n\r",ch);
		return TRUE;
	}

return TRUE;
}

void entry_prog_iseldheim_lift(ROOM_INDEX_DATA *room, CHAR_DATA *ch)
{
	send_to_char("You step onto the swaying lift.\n\r",ch);
	act("$n steps onto the lift, causing it to sway momentarily.",ch,0,0,TO_ROOM);
}

void drop_prog_elven_star(ROOM_INDEX_DATA *room, CHAR_DATA *ch, OBJ_DATA *obj)
{
	if(obj->pIndexData->vnum != (4637 || 4638 || 4639 || 4640 || 4641))
		return;

	act("As $p falls to the ground, it is immediately drawn to it's place in the star of the Chilliad.",ch,obj,0,TO_ROOM);
	return;
}


void speech_prog_elven_down(ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *speech)
{
	AFFECT_DATA af;
	ROOM_AFFECT_DATA raf;
	CHAR_DATA *ich = NULL;

	if(IS_NPC(ch))
		return;
	
	if(!str_cmp(speech,"hakri gwenidill mafre finae")) {

		if (is_affected_room(ch->in_room,gsn_rprog_elven_down)) {
			return;
		}

		init_affect_room(&raf);
        	raf.where = TO_ROOM_AFFECTS;
        	raf.type = gsn_rprog_elven_down;
        	raf.aftype = AFT_TIMER;
        	raf.duration = 0;
        	affect_to_room(ch->in_room,&raf);

		act("A chill wind blows through the room, extinguishing the candle.",ch,0,0,TO_CHAR);
		act("A chill wind blows through the room, extinguishing the candle.",ch,0,0,TO_ROOM);

		for ( ich = ch->in_room->people; ich != NULL; ich = ich->next_in_room ) {
			// Small char affect that will restore the original light after a tick.
			init_affect(&af);
			af.where = TO_AFFECTS;
			af.aftype = AFT_INVIS;
			af.duration = 0;
			SET_BIT(af.bitvector, AFF_BLIND);
			af.end_fun = rprog_elven_down_end;
			new_affect_to_char(ch,&af);
		}
	}
	return;
}

void rprog_elven_down_end (CHAR_DATA *ch, AFFECT_DATA *af) {
	ROOM_INDEX_DATA *to_room = NULL;

	to_room = get_room_index(4704);

	char_from_room( ch );
	char_to_room( ch, to_room );

	act("You feel the floor drop beneath you, and you feel yourself fall and then impact as you hit the ground.",ch,0,0,TO_CHAR);
	act("$n appears out of thin air, crumpling in to the ground with a thud.",ch,0,0,TO_ROOM);
	return;
}


void pulse_prog_elven_star(ROOM_INDEX_DATA *room)
{
	/*
	OBJ_DATA *obj;
	CHAR_DATA *ch=room->people;
	int nMatch=0;

	for(obj=room->contents; obj != NULL; obj=obj->next_content) {
		if(obj->pIndexData->vnum == (4637 || 4638 || 4639 || 4640 || 4641)) {
			nMatch++;
			continue;
		}
        if(nMatch > 4) {
                if(ch != NULL) {
                        act("As the star is completed, you hear a noise nearby.",ch,0,0,TO_ROOM);
                        act("Their purpose fulfilled, the gems disintegrate.",ch,0,0,TO_ROOM);
                }
                
                extract_obj(obj);
                break;
        }
	}
	*/
return;
}
