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

#if defined(macintosh)
#include <types.h>
#include <time.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "merc.h"
#include "recycle.h"

/* stuff for recyling notes */
NOTE_DATA *note_free;

NOTE_DATA *new_note()
{
    NOTE_DATA *note;

    if (note_free == NULL)
	note  = new NOTE_DATA;
    else
    {
	note = note_free;
	note_free = note_free->next;
    }
    VALIDATE(note);
    return note;
}

void free_note(NOTE_DATA *note)
{
    if (!IS_VALID(note))
	return;

    free_pstring( note->text    );
    free_pstring( note->subject );
    free_pstring( note->to_list );
    free_pstring( note->date    );
    free_pstring( note->sender  );
    INVALIDATE(note);

    note->next = note_free;
    note_free   = note;
}


/* stuff for recycling ban structures */
BAN_DATA *ban_free;

BAN_DATA *new_ban(void)
{
    static BAN_DATA ban_zero;
    BAN_DATA *ban;

    if (ban_free == NULL)
	ban  = new BAN_DATA;
    else
    {
	ban = ban_free;
	ban_free = ban_free->next;
    }

    *ban = ban_zero;
    VALIDATE(ban);
    ban->name = &str_empty[0];
    return ban;
}

void free_ban(BAN_DATA *ban)
{
    if (!IS_VALID(ban))
	return;

    free_pstring(ban->name);
    INVALIDATE(ban);

    ban->next = ban_free;
    ban_free = ban;
}

/* stuff for recycling descriptors */
DESCRIPTOR_DATA *descriptor_free;

DESCRIPTOR_DATA *new_descriptor(void)
{
    static DESCRIPTOR_DATA d_zero;
    DESCRIPTOR_DATA *d;

    if (descriptor_free == NULL)
	d  = new DESCRIPTOR_DATA;
    else
    {
	d = descriptor_free;
	descriptor_free = descriptor_free->next;
    }
	
    *d = d_zero;
    VALIDATE(d);
    return d;
}

void free_descriptor(DESCRIPTOR_DATA *d)
{
    if (!IS_VALID(d))
	return;

    free_pstring( d->host );
    if(d->outbuf)
    	delete[] d->outbuf;
    INVALIDATE(d);
    d->next = descriptor_free;
    descriptor_free = d;
}

/* stuff for recycling gen_data */
GEN_DATA *gen_data_free;

GEN_DATA *new_gen_data(void)
{
    static GEN_DATA gen_zero;
    GEN_DATA *gen;

    if (gen_data_free == NULL)
	gen  = new GEN_DATA;
    else
    {
	gen = gen_data_free;
	gen_data_free = gen_data_free->next;
    }
    *gen = gen_zero;
    VALIDATE(gen);
    return gen;
}

void free_gen_data(GEN_DATA *gen)
{
    if (!IS_VALID(gen))
	return;

    INVALIDATE(gen);

    gen->next = gen_data_free;
    gen_data_free = gen;
}

/* Stuff for recycling trophy lists */

TROPHY_DATA *new_trophy_data(char *victname)
{
	TROPHY_DATA* trophy;

	trophy  = new TROPHY_DATA;

	if (trophy == NULL) {
		return (NULL);
	}

	trophy->victname = palloc_string(victname);
	trophy->next = NULL;

	return trophy;
}

void free_trophy(TROPHY_DATA *trophy)
{
	if(!trophy)
		return (void)bug("ERROR: Null trophy freed!",0);
	if(trophy->next == NULL) {
		delete trophy;
		return;
	} else {
		free_trophy(trophy->next);
		delete trophy;

		return;
	}
}

/* mob speech memory management */

SPEECH_DATA *new_speech_data(void)
{
	SPEECH_DATA *speech;

	speech  = new SPEECH_DATA;

	speech->next = NULL;
	speech->prev = NULL;
	speech->mob = NULL;
	speech->name = NULL;
	speech->first_line = NULL;
	speech->current_line = NULL;

	return speech;
}

void free_speech(SPEECH_DATA *speech)
{
	free_pstring(speech->name);
	if (speech->next) {
		if (speech->prev) {
			speech->prev->next = speech->next;
			speech->next->prev = speech->prev;
		} else {
			speech->mob->speech = speech->next;
		}
	}
	delete speech;
	return;
}

void free_line(LINE_DATA *line)
{
	LINE_DATA *lptr;
	
	free_pstring(line->text);
	if (line->next) {
		if (line->prev) {
			line->prev->next = line->next;
			line->next->prev = line->prev;
		} else {
			line->speech->first_line = line->next;
		}
	}

	for (lptr = line; lptr; lptr = lptr->next)
		lptr->number--;

	delete line;
	return;
}

LINE_DATA *new_line_data(void)
{
	LINE_DATA *line;

	line = new LINE_DATA;

	line->speech = NULL;
	line->next = NULL;
	line->prev = NULL;
	line->number = -1;
	line->delay = -1;
	line->type = -1;
	line->text = NULL;

	return line;
}

IPROG_DATA *new_iprog(void)
{
	IPROG_DATA *ipr = new IPROG_DATA;
	ipr->wear_name = NULL;
	ipr->remove_name = NULL;
	ipr->get_name = NULL;
	ipr->loot_name = NULL;
	ipr->drop_name = NULL;
	ipr->sac_name = NULL;
	ipr->sac_name = NULL;
	ipr->entry_name = NULL;
	ipr->give_name = NULL;
	ipr->greet_name = NULL;
	ipr->fight_name = NULL;
	ipr->death_name = NULL;
	ipr->speech_name = NULL;
	ipr->pulse_name = NULL;
	ipr->invoke_name = NULL;
	ipr->open_name = NULL;
	ipr->look_name = NULL;
	ipr->verb_name = NULL;
	ipr->hit_name = NULL;
	return ipr;
}

RACE_DATA *new_race_data(void)
{
	RACE_DATA *race_specs;

	race_specs = new RACE_DATA;

	if (race_specs == NULL) {
		return (NULL);
	}

	race_specs->next = NULL;
	race_specs->first = NULL;
	race_specs->name = NULL;
	race_specs->number = -1;
	zero_vector(race_specs->act);
	zero_vector(race_specs->aff);
	zero_vector(race_specs->off);
	zero_vector(race_specs->imm);
	zero_vector(race_specs->res);
	zero_vector(race_specs->vuln);
	zero_vector(race_specs->form);
	zero_vector(race_specs->parts);

	return(race_specs);
}

void free_race_data(RACE_DATA *race_specs)
{
	if (race_specs->next == NULL) {
		delete race_specs;
		return;
	} else {
		free_race_data(race_specs->next);
		delete race_specs;

		return;
	}
}
	

/* Stuff for recycling track data */

TRACK_DATA *new_track_data(void)
{
	TRACK_DATA * tracks;

	tracks = new TRACK_DATA;

	if (tracks == NULL) {
		return (NULL);
	}

	tracks->prey = NULL;
	tracks->time = time_info;
	tracks->direction = -1;

	return tracks;
}

void free_track(TRACK_DATA *tracks)
{
	delete tracks;
	return;
}

PATHFIND_DATA *new_path_data(void)
{
	PATHFIND_DATA *path;
	int i;

	path = new PATHFIND_DATA;

	if (path == NULL) {
		return (NULL);
	}

	path->room 		=	NULL;
	path->evaluated	=	FALSE;
	path->dir_from	=	-1;
	path->steps		=	-1;
	path->prev		=	NULL;
	for (i = 0; i < 6; i++)
		path->dir_to[i] = NULL;

	return (path);
}

void free_path (PATHFIND_DATA *path)
{
	int i;
	
	if (!path->dir_to[DIR_NORTH]
		&& !path->dir_to[DIR_EAST]
		&& !path->dir_to[DIR_SOUTH]
		&& !path->dir_to[DIR_WEST]
		&& !path->dir_to[DIR_UP]
		&& !path->dir_to[DIR_DOWN]) {
		delete path;
	} else {
		for (i = 0; i < 6; i++)
			if (path->dir_to[i])
				free_path(path->dir_to[i]);
		delete path;
	}

	return;
}

/* stuff for recycling extended descs -- UGLY */
EXTRA_DESCR_DATA *extra_descr_free;

EXTRA_DESCR_DATA *new_extra_descr(void)
{
    EXTRA_DESCR_DATA *ed;

    if (extra_descr_free == NULL)
	ed  = new EXTRA_DESCR_DATA;
    else
    {
	ed = extra_descr_free;
	extra_descr_free = extra_descr_free->next;
    }

    ed->keyword = &str_empty[0];
    ed->description = &str_empty[0];
    VALIDATE(ed);
    return ed;
}

void free_extra_descr(EXTRA_DESCR_DATA *ed)
{
    if (!IS_VALID(ed))
	return;

    free_pstring(ed->keyword);
    free_pstring(ed->description);
    INVALIDATE(ed);

    ed->next = extra_descr_free;
    extra_descr_free = ed;
}

OBJ_APPLY_DATA *apply_free;

OBJ_APPLY_DATA *new_apply_data(void)
{
	static OBJ_APPLY_DATA app_zero;
	OBJ_APPLY_DATA *app;

	if (apply_free == NULL)
		app  = new OBJ_APPLY_DATA;
	else {
		app = apply_free;
		apply_free = apply_free->next;
	}

	*app = app_zero;
	app->type = 0;

	VALIDATE(app);
	return app;
}

void free_apply (OBJ_APPLY_DATA *app)
{
	if (!IS_VALID(app))
		return;

	INVALIDATE(app);
	app->next = apply_free;
	apply_free = app;
}

/* stuff for recycling affects */
AFFECT_DATA *affect_free;
ROOM_AFFECT_DATA *raffect_free;
OBJ_AFFECT_DATA *oaffect_free;
AREA_AFFECT_DATA *aaffect_free;

AFFECT_DATA *new_affect(void)
{
    static AFFECT_DATA af_zero;
    AFFECT_DATA *af;

    if (affect_free == NULL)
	af  = new AFFECT_DATA;
    else
    {
	af = affect_free;
	affect_free = affect_free->next;
    }

    *af = af_zero;

    VALIDATE(af);
    return af;
}

TRAP_DATA *new_trap(void)
{
	TRAP_DATA *trap;

	trap  = new TRAP_DATA;

	return trap;
}

void free_trap(TRAP_DATA *trap)
{
	free_pstring(trap->exec_echo);
	free_pstring(trap->trig_echo);
	delete trap;
	
	return;
}

RUNE_DATA *rune_free;
RUNE_DATA *new_rune(void)
{
    static RUNE_DATA rune_zero;
    RUNE_DATA *rune;

    if(rune_free == NULL)
	rune  = new RUNE_DATA;
    else
    {
	rune = rune_free;
	rune_free = rune->next;
    }
    *rune = rune_zero;
    return rune;
}
void free_rune(RUNE_DATA *rune)
{
    rune->next = rune_free;
    rune_free  = rune;
}

QUEUE_DATA *queue_free;

QUEUE_DATA *new_queue(void)
{
    static QUEUE_DATA queue_zero;
    QUEUE_DATA *queue;

    if (queue_free == NULL)
        queue  = new QUEUE_DATA;
    else
    {
        queue = queue_free;
        queue_free = queue->next;
    }

    *queue = queue_zero;

    return queue;

}
void free_queue(QUEUE_DATA *queue)
{
    queue->next = queue_free;
    queue_free  = queue;
}

void free_affect(AFFECT_DATA *af)
{
    if (!IS_VALID(af))
	return;

    free_pstring(af->name);
    INVALIDATE(af);
    af->next = affect_free;
    affect_free = af;
}

ROOM_AFFECT_DATA *new_affect_room(void)
{
    static ROOM_AFFECT_DATA af_zero;
    ROOM_AFFECT_DATA *af;

    if (raffect_free == NULL)
        af  = new ROOM_AFFECT_DATA;
    else
    {
        af = raffect_free;
        raffect_free = raffect_free->next;
    }

    *af = af_zero;

    VALIDATE(af);
    return af;
}

void free_affect_room(ROOM_AFFECT_DATA *af)
{
    if (!IS_VALID(af))
	return;

    INVALIDATE(af);
    af->next = raffect_free;
    raffect_free = af;
}

OBJ_AFFECT_DATA *new_affect_obj (void)
{
	static OBJ_AFFECT_DATA af_zero;
	OBJ_AFFECT_DATA *af;

	if (oaffect_free == NULL) {
		af  = new OBJ_AFFECT_DATA;
	} else {
		af = oaffect_free;
		oaffect_free = oaffect_free->next;
	}

	*af = af_zero;

	VALIDATE(af);
	return af;
}

void free_affect_obj(OBJ_AFFECT_DATA *af)
{
	if(!IS_VALID(af))	return;

	INVALIDATE(af);
	af->next = oaffect_free;
	oaffect_free = af;
}
	
AREA_AFFECT_DATA *new_affect_area(void)
{
	static AREA_AFFECT_DATA af_zero;
	AREA_AFFECT_DATA *af;

	if (aaffect_free == NULL) {
		af  = new AREA_AFFECT_DATA;
	} else {
		af = aaffect_free;
		aaffect_free = aaffect_free->next;
	}

	*af = af_zero;

	VALIDATE(af);
	return af;
}

void free_affect_area(AREA_AFFECT_DATA *af)
{
	if (!IS_VALID(af))
		return;

	INVALIDATE(af);
	af->next = aaffect_free;
	aaffect_free = af;
}

/* stuff for recycling objects */
OBJ_DATA *obj_free;

OBJ_DATA *new_obj(void)
{
    static OBJ_DATA obj_zero;
    OBJ_DATA *obj;

    if (obj_free == NULL)
	obj  = new OBJ_DATA;
    else
    {
	obj = obj_free;
	obj_free = obj_free->next;
    }
    *obj = obj_zero;
    VALIDATE(obj);

    return obj;
}

void free_obj(OBJ_DATA *obj)
{
    OBJ_AFFECT_DATA *paf, *paf_next;
    EXTRA_DESCR_DATA *ed, *ed_next;

    if (!IS_VALID(obj))
	return;

    for (paf = obj->affected; paf != NULL; paf = paf_next)
    {
	paf_next = paf->next;
	free_affect_obj(paf);
    }
    obj->affected = NULL;

    for (ed = obj->extra_descr; ed != NULL; ed = ed_next )
    {
	ed_next = ed->next;
	free_extra_descr(ed);
     }
     obj->extra_descr = NULL;

    free_pstring( obj->name        );
    free_pstring( obj->description );
    free_pstring( obj->short_descr );
   // free_pstring( obj->owner     );
    INVALIDATE(obj);

    obj->next   = obj_free;
    obj_free    = obj;
}


/* stuff for recyling characters */
CHAR_DATA *char_free;
extern bool bDebug;
CHAR_DATA *new_char (void)
{
    static CHAR_DATA ch_zero;
    CHAR_DATA *ch;
    int i;

    if (char_free == NULL)
    {
		ch  = new CHAR_DATA;
		if(bDebug)
			bug("Char free is null.  . . . . !",0);
    }
    else
    {
	ch = char_free;
	char_free = char_free->next;
    }

    *ch				= ch_zero;
    VALIDATE(ch);
    ch->name                    = &str_empty[0];
    ch->short_descr             = &str_empty[0];
    ch->long_descr              = &str_empty[0];
    ch->description             = &str_empty[0];
    ch->prompt                  = &str_empty[0];
    ch->prefix			= &str_empty[0];
    ch->logon                   = current_time;
    ch->played			= 0;
    ch->lines                   = PAGELEN;
    for (i = 0; i < 4; i++)
        ch->armor[i]            = 0;
    ch->position                = POS_STANDING;
    ch->hit                     = 20;
    ch->max_hit                 = 20;
    ch->mana                    = 100;
    ch->max_mana                = 100;
    ch->move                    = 100;
    ch->max_move                = 100;
    ch->arms					= 2;
    ch->legs					= 2;
	ch->regen_rate				= 0;
    ch->balance					= 0;
	ch->batter					= 0;
	ch->analyze					= 0;
	ch->talismanic				= 0;
    ch->law_pass				= FALSE;
	zero_vector(ch->imm_flags);
    for (i = 0; i < MAX_STATS; i ++)
    {
        ch->perm_stat[i] = 13;
        ch->mod_stat[i] = 0;
    }

    return ch;
}


void free_char (CHAR_DATA *ch)
{
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    AFFECT_DATA *paf;
    AFFECT_DATA *paf_next;
    if (!IS_VALID(ch) || !ch)
	return;

    if (IS_NPC(ch))
	mobile_count--;

    for (obj = ch->carrying; obj != NULL; obj = obj_next)
    {
		obj_next = obj->next_content;
		extract_obj(obj);
    }

    for (paf = ch->affected; paf != NULL; paf = paf_next)
    {
		paf_next = paf->next;
		paf->pulse_fun	= NULL;
		paf->tick_fun	= NULL;
		paf->end_fun	= NULL;
		affect_remove(ch,paf);
    }
    free_pstring(ch->name);
    free_pstring(ch->short_descr);
    free_pstring(ch->long_descr);
    free_pstring(ch->true_name);
    free_pstring(ch->description);
    free_pstring(ch->prompt);
    free_pstring(ch->prefix);

    if (ch->pcdata != NULL)
    	free_pcdata(ch->pcdata);

    ch->next = char_free;
    char_free  = ch;
    INVALIDATE(ch);
    return;
}

PC_DATA *pcdata_free;

PC_DATA *new_pcdata(void)
{
    int alias;

    static PC_DATA pcdata_zero;
    PC_DATA *pcdata;

    if (pcdata_free == NULL)
	pcdata  = new PC_DATA;
    else
    {
	pcdata = pcdata_free;
	pcdata_free = pcdata_free->next;
    }

    *pcdata = pcdata_zero;

    for (alias = 0; alias < MAX_ALIAS; alias++)
    {
	pcdata->alias[alias] = NULL;
	pcdata->alias_sub[alias] = NULL;
    }

    pcdata->buffer = new_buf();

    VALIDATE(pcdata);
	pcdata->trusting = NULL;
	pcdata->death_status = 0;
    return pcdata;
}

OLD_CHAR *oldtype_free;	

OLD_CHAR *new_oldchar(void)
{
    static OLD_CHAR oldtype_zero;
    OLD_CHAR *oldtype;

    if (oldtype_free == NULL)
        oldtype  = new OLD_CHAR;
    else
    {
        oldtype = oldtype_free;
        oldtype_free = oldtype_free->next;
    }

    *oldtype = oldtype_zero;

    return oldtype;
}

void free_oldchar( OLD_CHAR *old )
{
	free_pstring(old->name);
	free_pstring(old->short_descr);
	free_pstring(old->long_descr);
	free_pstring(old->description);

	old->next = oldtype_free;
	oldtype_free = old;
	return;
}


void free_pcdata(PC_DATA *pcdata)
{
    if (!IS_VALID(pcdata))
	return;

    free_pstring(pcdata->pwd);
    free_pstring(pcdata->bamfin);
    free_pstring(pcdata->bamfout);
    free_pstring(pcdata->title);
    free_buf(pcdata->buffer);
	for(int i = 0; i < 100; i++)
		if(pcdata->recentkills[i] != NULL)
			free_pstring(pcdata->recentkills[i]);

/*    for (alias = 0; alias < MAX_ALIAS; alias++)
    {
		if(pcdata->alias[alias] != NULL)
		{	
			free_pstring(pcdata->alias[alias]);
			free_pstring(pcdata->alias_sub[alias]);
		}
    } */
    INVALIDATE(pcdata);
    pcdata->next = pcdata_free;
    pcdata_free = pcdata;

    return;
}

	


/* stuff for setting ids */
long	last_pc_id;
long	last_mob_id;

long get_pc_id(void)
{
    int val;

    val = (current_time <= last_pc_id) ? last_pc_id + 1 : current_time;
    last_pc_id = val;
    return val;
}

long get_mob_id(void)
{
    last_mob_id++;
    return last_mob_id;
}

MEM_DATA *mem_data_free;

/* procedures and constants needed for buffering */

BUFFER *buf_free;

MEM_DATA *new_mem_data(void)
{
    MEM_DATA *memory;

    if (mem_data_free == NULL)
	memory  = new MEM_DATA;
    else
    {
	memory = mem_data_free;
	mem_data_free = mem_data_free->next;
    }

    memory->next = NULL;
    memory->id = 0;
    memory->reaction = 0;
    memory->when = 0;
    VALIDATE(memory);

    return memory;
}

void free_pstruct_data(MEM_DATA *memory)
{
    if (!IS_VALID(memory))
	return;

    memory->next = mem_data_free;
    mem_data_free = memory;
    INVALIDATE(memory);
}



/* buffer sizes */
const int buf_size[MAX_BUF_LIST] =
{
    16,32,64,128,256,1024,2048,4096,8192,16384
};

/* local procedure for finding the next acceptable size */
/* -1 indicates out-of-boundary error */
int get_size (int val)
{
    int i;

    for (i = 0; i < MAX_BUF_LIST; i++)
	if (buf_size[i] >= val)
	{
	    return buf_size[i];
	}

    return -1;
}

BUFFER *new_buf()
{
    BUFFER *buffer;

    if (buf_free == NULL)
		buffer  = new BUFFER;
    else {
		buffer = buf_free;
		buf_free = buf_free->next;
    }

    buffer->next	= NULL;
    buffer->state	= BUFFER_SAFE;
//    buffer->size	= get_size(BASE_BUF);

//    buffer->string	= palloc_struct(buffer->size);
//    buffer->string[0]	= '\0';					JUST SAY NO TO DIKU.  --D
    buffer->size	= 0;
    buffer->string	= NULL;
    VALIDATE(buffer);

    return buffer;
}

void free_buf(BUFFER *buffer)
{
    if (!IS_VALID(buffer))
	return;
    if(buffer->string)
        free_pstring(buffer->string);
    buffer->string = NULL;
    buffer->size   = 0;
    buffer->state  = BUFFER_FREED;
    INVALIDATE(buffer);

    buffer->next  = buf_free;
    buf_free      = buffer;
}


bool add_buf(BUFFER *buffer, char *string)
{
    int len;
	char *tptr;
    if(string[0] == '\0' || string == NULL)
	return FALSE;

    if (!buffer->string || !strlen(buffer->string)) //like a virgin.. touched for the very first tiiiiime
    {
	buffer->string = palloc_string(string);
	buffer->size = strlen(string) + 1;
	return TRUE;
    }
	
    len = strlen(buffer->string) + strlen(string) + 1;
	if(len > 32766)
		return TRUE;
	tptr = buffer->string;
    buffer->string = new char[len];
    if(!buffer->string)
		return FALSE;
	buffer->size = len;
	strcpy(buffer->string, tptr);
    strcat(buffer->string, string);
	delete[] tptr;
    return TRUE;
}


void clear_buf(BUFFER *buffer)
{
    free_pstring(buffer->string);
    buffer->string = NULL;
    buffer->state     = BUFFER_SAFE;
}


char *buf_string(BUFFER *buffer)
{
    return buffer->string;
}

