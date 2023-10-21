/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik Stærfeldt, Tom Madsen, and Katja Nyboe.   *
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
 *  ROM 2.4 is copyright 1993-1996 Russ Taylor                             *
 *  ROM has been brought to you by the ROM consortium                      *
 *      Russ Taylor (rtaylor@efn.org)                                      *
 *      Gabrielle Taylor (gtaylor@pacinfo.com)                             *
 *      Brian Moore (zump@rom.org)                                         *
 *  By using this code, you have agreed to follow the terms of the         *
 *  ROM license, in the file Tartarus/doc/rom.license                      *
 ***************************************************************************/

/***************************************************************************
 *       Tartarus code is copyright (C) 1997-1998 by Daniel Graham         *
 *  In using this code you agree to comply with the Tartarus license       *
 *       found in the file /Tartarus/doc/tartarus.doc                      *
 ***************************************************************************/

#ifdef macintosh
#include <types.h>
#include <time.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "recycle.h"
#include "comm.h"
#include "db.h"
#include "handler.h"
#include "newmem.h"
#include "misc.h"
#include "utility.h"
#include "room.h"


/* buffer sizes */
const int buf_size[MAX_BUF_LIST] = {16, 32, 64, 128, 256, 1024, 2048, 4096, 8192, 16384};

DESCRIPTOR_DATA *descriptor_free;
GEN_DATA *gen_data_free;
EXTRA_DESCR_DATA *extra_descr_free;
OBJ_APPLY_DATA *apply_free;
AFFECT_DATA *affect_free;
ROOM_AFFECT_DATA *raffect_free;
OBJ_AFFECT_DATA *oaffect_free;
AREA_AFFECT_DATA *aaffect_free;
RUNE_DATA *rune_free;
QUEUE_DATA *queue_free;
OBJ_DATA *obj_free;
CHAR_DATA *char_free;
PC_DATA *pcdata_free;
OLD_CHAR *oldtype_free;
MEM_DATA *mem_data_free;
BUFFER *buf_free;

long last_pc_id;
long last_mob_id;

/* stuff for recycling descriptors */
DESCRIPTOR_DATA *new_descriptor(void)
{
	static DESCRIPTOR_DATA d_zero;
	DESCRIPTOR_DATA *d;

	if (descriptor_free == nullptr)
		d = new DESCRIPTOR_DATA;
	else
	{
		d = descriptor_free;
		descriptor_free = descriptor_free->next;
	}

	*d = d_zero;

	d->valid = true;
	return d;
}

void free_descriptor(DESCRIPTOR_DATA *d)
{
	if (!(d != nullptr && d->valid))
		return;

	free_pstring(d->host);

	if (d->outbuf)
		delete[] d->outbuf;

	d->valid = false;
	d->next = descriptor_free;
	descriptor_free = d;
}

/* stuff for recycling gen_data */
GEN_DATA *new_gen_data(void)
{
	static GEN_DATA gen_zero;
	GEN_DATA *gen;

	if (gen_data_free == nullptr)
	{
		gen = new GEN_DATA;
	}
	else
	{
		gen = gen_data_free;
		gen_data_free = gen_data_free->next;
	}

	*gen = gen_zero;

	gen->valid = true;
	return gen;
}

void free_gen_data(GEN_DATA *gen)
{
	if (!(gen != nullptr && gen->valid))
		return;

	gen->valid = false;
	gen->next = gen_data_free;
	gen_data_free = gen;
}

/* Stuff for recycling trophy lists */
TROPHY_DATA *new_trophy_data(char *victname)
{
	TROPHY_DATA *trophy = new TROPHY_DATA;

	trophy->victname = palloc_string(victname);
	trophy->next = nullptr;

	return trophy;
}

void free_trophy(TROPHY_DATA *trophy)
{
	if (!trophy)
	{
		bug("ERROR: Null trophy freed!", 0);
		return;
	}

	if (trophy->next == nullptr)
	{
		delete trophy;
	}
	else
	{
		free_trophy(trophy->next);
		delete trophy;
	}
}

/* mob speech memory management */

SPEECH_DATA *new_speech_data(void)
{
	SPEECH_DATA *speech = new SPEECH_DATA;

	speech->next = nullptr;
	speech->prev = nullptr;
	speech->mob = nullptr;
	speech->name = nullptr;
	speech->first_line = nullptr;
	speech->current_line = nullptr;

	return speech;
}

void free_speech(SPEECH_DATA *speech)
{
	free_pstring(speech->name);

	if (speech->next)
	{
		if (speech->prev)
		{
			speech->prev->next = speech->next;
			speech->next->prev = speech->prev;
		}
		else
		{
			speech->mob->speech = speech->next;
		}
	}

	delete speech;
}

void free_line(LINE_DATA *line)
{
	LINE_DATA *lptr;

	free_pstring(line->text);

	if (line->next)
	{
		if (line->prev)
		{
			line->prev->next = line->next;
			line->next->prev = line->prev;
		}
		else
		{
			line->speech->first_line = line->next;
		}
	}

	for (lptr = line; lptr; lptr = lptr->next)
	{
		lptr->number--;
	}

	delete line;
}

LINE_DATA *new_line_data(void)
{
	LINE_DATA *line = new LINE_DATA;

	line->speech = nullptr;
	line->next = nullptr;
	line->prev = nullptr;
	line->number = -1;
	line->delay = -1;
	line->type = -1;
	line->text = nullptr;

	return line;
}

IPROG_DATA *new_iprog(void)
{
	IPROG_DATA *ipr = new IPROG_DATA;

	ipr->wear_name = nullptr;
	ipr->remove_name = nullptr;
	ipr->get_name = nullptr;
	ipr->loot_name = nullptr;
	ipr->drop_name = nullptr;
	ipr->sac_name = nullptr;
	ipr->sac_name = nullptr;
	ipr->entry_name = nullptr;
	ipr->give_name = nullptr;
	ipr->greet_name = nullptr;
	ipr->fight_name = nullptr;
	ipr->death_name = nullptr;
	ipr->speech_name = nullptr;
	ipr->pulse_name = nullptr;
	ipr->invoke_name = nullptr;
	ipr->open_name = nullptr;
	ipr->look_name = nullptr;
	ipr->verb_name = nullptr;
	ipr->hit_name = nullptr;

	return ipr;
}

RACE_DATA *new_race_data(void)
{
	RACE_DATA *race_specs = new RACE_DATA;

	race_specs->next = nullptr;
	race_specs->first = nullptr;
	race_specs->name = nullptr;
	race_specs->number = -1;

	zero_vector(race_specs->act);
	zero_vector(race_specs->aff);
	zero_vector(race_specs->off);
	zero_vector(race_specs->imm);
	zero_vector(race_specs->res);
	zero_vector(race_specs->vuln);
	zero_vector(race_specs->form);
	zero_vector(race_specs->parts);

	return race_specs;
}

void free_race_data(RACE_DATA *race_specs)
{
	if (race_specs->next == nullptr)
	{
		delete race_specs;
	}
	else
	{
		free_race_data(race_specs->next);
		delete race_specs;
	}
}

/* Stuff for recycling track data */

TRACK_DATA *new_track_data(void)
{
	TRACK_DATA *tracks = new TRACK_DATA;

	tracks->prey = nullptr;
	tracks->time = time_info;
	tracks->direction = -1;

	return tracks;
}

void free_track(TRACK_DATA *tracks)
{
	delete tracks;
}

PATHFIND_DATA *new_path_data(void)
{
	PATHFIND_DATA *path = new PATHFIND_DATA;

	path->room = nullptr;
	path->evaluated= false;
	path->dir_from = -1;
	path->steps = -1;
	path->prev = nullptr;

	for (int i = 0; i < 6; i++)
	{
		path->dir_to[i] = nullptr;
	}

	return path;
}

void free_path(PATHFIND_DATA *path)
{
	if (!path->dir_to[Directions::North]
		&& !path->dir_to[Directions::East]
		&& !path->dir_to[Directions::South]
		&& !path->dir_to[Directions::West]
		&& !path->dir_to[Directions::Up]
		&& !path->dir_to[Directions::Down])
	{
		delete path;
	}
	else
	{
		for (int i = 0; i < 6; i++)
		{
			if (path->dir_to[i])
				free_path(path->dir_to[i]);
		}

		delete path;
	}
}

/* stuff for recycling extended descs -- UGLY */

EXTRA_DESCR_DATA *new_extra_descr(void)
{
	EXTRA_DESCR_DATA *ed;

	if (extra_descr_free == nullptr)
	{
		ed = new EXTRA_DESCR_DATA;
	}
	else
	{
		ed = extra_descr_free;
		extra_descr_free = extra_descr_free->next;
	}

	ed->keyword = &str_empty[0];
	ed->description = &str_empty[0];

	ed->valid = true;
	return ed;
}

void free_extra_descr(EXTRA_DESCR_DATA *ed)
{
	if (!(ed != nullptr && ed->valid))
		return;

	free_pstring(ed->keyword);
	free_pstring(ed->description);

	ed->valid = false;
	ed->next = extra_descr_free;
	extra_descr_free = ed;
}

OBJ_APPLY_DATA *new_apply_data(void)
{
	static OBJ_APPLY_DATA app_zero;
	OBJ_APPLY_DATA *app;

	if (apply_free == nullptr)
	{
		app = new OBJ_APPLY_DATA;
	}
	else
	{
		app = apply_free;
		apply_free = apply_free->next;
	}

	*app = app_zero;
	app->type = 0;

	app->valid = true;
	return app;
}

void free_apply(OBJ_APPLY_DATA *app)
{
	if (!(app != nullptr && app->valid))
		return;

	app->valid = false;
	app->next = apply_free;
	apply_free = app;
}

/* stuff for recycling affects */

AFFECT_DATA *new_affect(void)
{
	static AFFECT_DATA af_zero;
	AFFECT_DATA *af;

	if (affect_free == nullptr)
	{
		af = new AFFECT_DATA;
	}
	else
	{
		af = affect_free;
		affect_free = affect_free->next;
	}

	*af = af_zero;

	af->valid = true;
	return af;
}

TRAP_DATA *new_trap(void)
{
	TRAP_DATA *trap = new TRAP_DATA;

	return trap;
}

void free_trap(TRAP_DATA *trap)
{
	free_pstring(trap->exec_echo);
	free_pstring(trap->trig_echo);

	delete trap;
}

RUNE_DATA *new_rune(void)
{
	static RUNE_DATA rune_zero;
	RUNE_DATA *rune;

	if (rune_free == nullptr)
	{
		rune = new RUNE_DATA;
	}
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
	rune_free = rune;
}

QUEUE_DATA *new_queue(void)
{
	static QUEUE_DATA queue_zero;
	QUEUE_DATA *queue;

	if (queue_free == nullptr)
	{
		queue = new QUEUE_DATA;
	}
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
	queue_free = queue;
}

void free_affect(AFFECT_DATA *af)
{
	if (!(af != nullptr && af->valid))
		return;

	free_pstring(af->name);

	af->valid = false;
	af->next = affect_free;
	affect_free = af;
}

ROOM_AFFECT_DATA *new_affect_room(void)
{
	static ROOM_AFFECT_DATA af_zero;
	ROOM_AFFECT_DATA *af;

	if (raffect_free == nullptr)
	{
		af = new ROOM_AFFECT_DATA;
	}
	else
	{
		af = raffect_free;
		raffect_free = raffect_free->next;
	}

	*af = af_zero;

	af->valid = true;
	return af;
}

void free_affect_room(ROOM_AFFECT_DATA *af)
{
	if (!(af != nullptr && af->valid))
		return;

	af->valid = false;
	af->next = raffect_free;
	raffect_free = af;
}

OBJ_AFFECT_DATA *new_affect_obj(void)
{
	static OBJ_AFFECT_DATA af_zero;
	OBJ_AFFECT_DATA *af;

	if (oaffect_free == nullptr)
	{
		af = new OBJ_AFFECT_DATA;
	}
	else
	{
		af = oaffect_free;
		oaffect_free = oaffect_free->next;
	}

	*af = af_zero;

	af->valid = true;
	return af;
}

void free_affect_obj(OBJ_AFFECT_DATA *af)
{
	if (!(af != nullptr && af->valid))
		return;

	af->valid = false;
	af->next = oaffect_free;
	oaffect_free = af;
}

AREA_AFFECT_DATA *new_affect_area(void)
{
	static AREA_AFFECT_DATA af_zero;
	AREA_AFFECT_DATA *af;

	if (aaffect_free == nullptr)
	{
		af = new AREA_AFFECT_DATA;
	}
	else
	{
		af = aaffect_free;
		aaffect_free = aaffect_free->next;
	}

	*af = af_zero;

	af->valid = true;
	return af;
}

void free_affect_area(AREA_AFFECT_DATA *af)
{
	if (!(af != nullptr && af->valid))
		return;

	af->valid = false;
	af->next = aaffect_free;
	aaffect_free = af;
}

/* stuff for recycling objects */

OBJ_DATA *new_obj(void)
{
	static OBJ_DATA obj_zero;
	OBJ_DATA *obj;

	if (obj_free == nullptr)
	{
		obj = new OBJ_DATA;
	}
	else
	{
		obj = obj_free;
		obj_free = obj_free->next;
	}

	*obj = obj_zero;

	obj->valid = true;
	return obj;
}

void free_obj(OBJ_DATA *obj)
{
	OBJ_AFFECT_DATA *paf, *paf_next;
	EXTRA_DESCR_DATA *ed, *ed_next;

	if (!(obj != nullptr && obj->valid))
		return;

	for (paf = obj->affected; paf != nullptr; paf = paf_next)
	{
		paf_next = paf->next;
		free_affect_obj(paf);
	}

	obj->affected = nullptr;

	for (ed = obj->extra_descr; ed != nullptr; ed = ed_next)
	{
		ed_next = ed->next;
		free_extra_descr(ed);
	}

	obj->extra_descr = nullptr;

	free_pstring(obj->name);
	free_pstring(obj->description);
	free_pstring(obj->short_descr);

	// free_pstring( obj->owner     );
	obj->valid = false;
	obj->next = obj_free;
	obj_free = obj;
}

/* stuff for recyling characters */

CHAR_DATA *new_char(void)
{
	static CHAR_DATA ch_zero;
	CHAR_DATA *ch;
	int i;

	if (char_free == nullptr)
	{
		ch = new CHAR_DATA;

		if (bDebug)
			bug("Char free is null.  . . . . !", 0);
	}
	else
	{
		ch = char_free;
		char_free = char_free->next;
	}

	*ch = ch_zero;

	ch->valid = true;

	ch->name = &str_empty[0];
	ch->short_descr = &str_empty[0];
	ch->long_descr = &str_empty[0];
	ch->description = &str_empty[0];
	ch->prompt = &str_empty[0];
	ch->prefix = &str_empty[0];
	ch->logon = current_time;
	ch->played = 0;
	ch->lines = PAGELEN;

	for (i = 0; i < 4; i++)
	{
		ch->armor[i] = 0;
	}

	ch->position = POS_STANDING;
	ch->hit = 20;
	ch->max_hit = 20;
	ch->mana = 100;
	ch->max_mana = 100;
	ch->move = 100;
	ch->max_move = 100;
	ch->arms = 2;
	ch->legs = 2;
	ch->regen_rate = 0;
	ch->balance = 0;
	ch->batter = 0;
	ch->analyze = 0;
	ch->talismanic = 0;
	ch->law_pass= false;

	zero_vector(ch->imm_flags);

	for (i = 0; i < MAX_STATS; i++)
	{
		ch->perm_stat[i] = 13;
		ch->mod_stat[i] = 0;
	}

	return ch;
}

void free_char(CHAR_DATA *ch)
{
	OBJ_DATA *obj;
	OBJ_DATA *obj_next;
	AFFECT_DATA *paf;
	AFFECT_DATA *paf_next;

	if (!(ch != nullptr && ch->valid) || !ch)
		return;

	if (is_npc(ch))
		mobile_count--;

	for (obj = ch->carrying; obj != nullptr; obj = obj_next)
	{
		obj_next = obj->next_content;
		extract_obj(obj);
	}

	for (paf = ch->affected; paf != nullptr; paf = paf_next)
	{
		paf_next = paf->next;
		paf->pulse_fun = nullptr;
		paf->tick_fun = nullptr;
		paf->end_fun = nullptr;
		affect_remove(ch, paf);
	}

	free_pstring(ch->name);
	free_pstring(ch->short_descr);
	free_pstring(ch->long_descr);
	free_pstring(ch->true_name);
	free_pstring(ch->description);
	free_pstring(ch->prompt);
	free_pstring(ch->prefix);

	if (ch->pcdata != nullptr)
		free_pcdata(ch->pcdata);

	ch->next = char_free;
	char_free = ch;

	ch->valid = false;
}

PC_DATA *new_pcdata(void)
{
	int alias;

	static PC_DATA pcdata_zero;
	PC_DATA *pcdata;

	if (pcdata_free == nullptr)
	{
		pcdata = new PC_DATA;
	}
	else
	{
		pcdata = pcdata_free;
		pcdata_free = pcdata_free->next;
	}

	*pcdata = pcdata_zero;

	for (alias = 0; alias < MAX_ALIAS; alias++)
	{
		pcdata->alias[alias] = nullptr;
		pcdata->alias_sub[alias] = nullptr;
	}

	pcdata->buffer = new_buf();

	pcdata->valid = true;

	pcdata->trusting = nullptr;
	pcdata->death_status = 0;
	return pcdata;
}

OLD_CHAR *new_oldchar(void)
{
	static OLD_CHAR oldtype_zero;
	OLD_CHAR *oldtype;

	if (oldtype_free == nullptr)
	{
		oldtype = new OLD_CHAR;
	}
	else
	{
		oldtype = oldtype_free;
		oldtype_free = oldtype_free->next;
	}

	*oldtype = oldtype_zero;
	return oldtype;
}

void free_oldchar(OLD_CHAR *old)
{
	free_pstring(old->name);
	free_pstring(old->short_descr);
	free_pstring(old->long_descr);
	free_pstring(old->description);

	old->next = oldtype_free;
	oldtype_free = old;
}

void free_pcdata(PC_DATA *pcdata)
{
	if (!(pcdata != nullptr && pcdata->valid))
		return;

	free_pstring(pcdata->pwd);
	free_pstring(pcdata->bamfin);
	free_pstring(pcdata->bamfout);
	free_pstring(pcdata->title);
	free_buf(pcdata->buffer);

	for (int i = 0; i < 100; i++)
	{
		if (pcdata->recentkills[i] != nullptr)
			free_pstring(pcdata->recentkills[i]);
	}

	/*
	for (alias = 0; alias < MAX_ALIAS; alias++)
	{
		if(pcdata->alias[alias] != nullptr)
		{
			free_pstring(pcdata->alias[alias]);
			free_pstring(pcdata->alias_sub[alias]);
		}
	}
	*/

	pcdata->valid = false;

	pcdata->next = pcdata_free;
	pcdata_free = pcdata;
}

/* stuff for setting ids */

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

/* procedures and constants needed for buffering */

MEM_DATA *new_mem_data(void)
{
	MEM_DATA *memory;

	if (mem_data_free == nullptr)
		memory = new MEM_DATA;
	else
	{
		memory = mem_data_free;
		mem_data_free = mem_data_free->next;
	}

	memory->next = nullptr;
	memory->id = 0;
	memory->reaction = 0;
	memory->when = 0;
	memory->valid = true;

	return memory;
}

void free_pstruct_data(MEM_DATA *memory)
{
	if (!(memory != nullptr && memory->valid))
		return;

	memory->next = mem_data_free;
	mem_data_free = memory;
	memory->valid = false;
}

/* local procedure for finding the next acceptable size */
/* -1 indicates out-of-boundary error */
int get_size(int val)
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

	if (buf_free == nullptr)
	{
		buffer = new BUFFER;
	}
	else
	{
		buffer = buf_free;
		buf_free = buf_free->next;
	}

	buffer->next = nullptr;
	buffer->state = BUFFER_SAFE;

	/*
	buffer->size = get_size(BASE_BUF);

	buffer->string = palloc_struct(buffer->size);
	buffer->string[0] = '\0';						//JUST SAY NO TO DIKU.  --D
	*/

	buffer->size = 0;
	buffer->string = nullptr;

	buffer->valid = true;
	return buffer;
}

void free_buf(BUFFER *buffer)
{
	if (!(buffer != nullptr && buffer->valid))
		return;

	if (buffer->string)
		free_pstring(buffer->string);

	buffer->string = nullptr;
	buffer->size = 0;
	buffer->state = BUFFER_FREED;

	buffer->valid = false;
	buffer->next = buf_free;
	buf_free = buffer;
}

bool add_buf(BUFFER *buffer, char *string)
{
	int len;
	char *tptr;

	if (string[0] == '\0' || string == nullptr)
		return false;

	if (!buffer->string || !strlen(buffer->string)) // like a virgin.. touched for the very first tiiiiime
	{
		buffer->string = palloc_string(string);
		buffer->size = strlen(string) + 1;
		return true;
	}

	len = strlen(buffer->string) + strlen(string) + 1;

	if (len > 32766)
		return true;

	tptr = buffer->string;
	buffer->string = new char[len];

	if (!buffer->string)
		return false;

	buffer->size = len;

	strcpy(buffer->string, tptr);
	strcat(buffer->string, string);

	delete[] tptr;
	return true;
}

void clear_buf(BUFFER *buffer)
{
	free_pstring(buffer->string);
	buffer->string = nullptr;
	buffer->state = BUFFER_SAFE;
}

char *buf_string(BUFFER *buffer)
{
	return buffer->string;
}
