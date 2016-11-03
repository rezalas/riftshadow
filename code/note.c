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
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <mysql.h>
#include "merc.h"
#include "recycle.h"
#include "tables.h"

/* globals from db.c for load_notes */
extern  int     _filbuf         args( (FILE *) );
extern FILE *                  fpArea;
extern char                    strArea[MAX_INPUT_LENGTH];

void parse_note(CHAR_DATA *ch, char *argument, int type);
bool hide_note(CHAR_DATA *ch, MYSQL_ROW row);

int count_spool(CHAR_DATA *ch, int type)
{
    int count = 0;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[MSL];
    sprintf(query,"SELECT * FROM notes WHERE type=%d",type);
    res		= one_query_res(query);
    while((row=mysql_fetch_row(res)))
    {
	if(!hide_note(ch,row))
		count++;
    }
    mysql_free_result(res);
    return count;
}

void do_unread(CHAR_DATA *ch, char *arg)
{
    char buf[MAX_STRING_LENGTH];
    int count;
    bool found = FALSE;

    if (IS_NPC(ch))
		return;
    if ((count = count_spool(ch,NOTE_NOTE)) > 0)
    {
	found = TRUE;
	sprintf(buf,"You have %d new note%s waiting.\n\r",
	    count, count > 1 ? "s" : "");
	send_to_char(buf,ch);
    }
    if ((count = count_spool(ch,NOTE_IDEA)) > 0)
    {
	found = TRUE;
	sprintf(buf,"You have %d unread idea%s to peruse.\n\r",
	    count, count > 1 ? "s" : "");
	send_to_char(buf,ch);
    }
    if (!found)
	send_to_char("You have no unread notes.\n\r",ch);
}

void do_note(CHAR_DATA *ch,char *argument)
{
    parse_note(ch,argument,NOTE_NOTE);
}

void do_news(CHAR_DATA *ch,char *argument)
{
    parse_note(ch,argument,NOTE_NEWS);
}

void do_changes(CHAR_DATA *ch,char *argument)
{
    parse_note(ch,argument,NOTE_CHANGES);
}

void append_note(NOTE_DATA *pnote)
{
    char query[MSL];
	char *escape;

	escape = (char *) escape_string(pnote->text);
    sprintf(query,"INSERT INTO notes VALUES(%d,\"%s\",'%s',\"%s\",\"%s\",\"%s\",%ld)",
	pnote->type, pnote->sender, pnote->date, pnote->to_list, pnote->subject, escape, pnote->date_stamp);
    one_query(query);
}

bool is_note_to( CHAR_DATA *ch, char *sender, char *to_list )
{
    if ( !str_cmp( ch->true_name, sender))
	return TRUE;

    if ( is_name( "all", to_list ))
	return TRUE;

    if ( IS_IMMORTAL(ch) && is_name( "immortal", to_list ) )
	return TRUE;

    if ( IS_HEROIMM(ch) && is_name( "heroimm", to_list ) )
	return TRUE;

    if ( IS_SET(ch->act, PLR_CODER) && is_name( "coder", to_list ) )
	return TRUE;

    if ( !IS_NPC(ch) && is_name(ch->Class()->name, to_list) )
		return TRUE;
	
    if (ch->cabal && is_name(cabal_table[ch->cabal].name,to_list))
	return TRUE;

    if (is_name(ch->true_name, to_list))
	return TRUE;

    if (is_number(to_list) && ch->level >= atoi(to_list))
	return TRUE;
    return FALSE;
}



void note_attach( CHAR_DATA *ch, int type )
{
    NOTE_DATA *pnote;

    if ( ch->pnote != NULL )
	return;

    pnote = new_note();

    pnote->next		= NULL;
    pnote->sender	= (IS_NPC(ch)) ? palloc_string(ch->short_descr) : palloc_string(ch->true_name);
    pnote->date		= palloc_string( "" );
    pnote->to_list	= palloc_string( "" );
    pnote->subject	= palloc_string( "" );
    pnote->text		= palloc_string( "" );
    pnote->type		= type;
    ch->pnote		= pnote;
    return;
}

bool hide_note (CHAR_DATA *ch, MYSQL_ROW row)
{
    time_t last_read;

    if (IS_NPC(ch))
		return TRUE;

    switch (atoi(row[0]))
    {
	default:
	    return TRUE;
	case NOTE_NOTE:
	    last_read = ch->pcdata->last_note;
	    break;
	case NOTE_IDEA:
	    last_read = ch->pcdata->last_idea;
	    break;
	case NOTE_PENALTY:
	    last_read = ch->pcdata->last_penalty;
	    break;
	case NOTE_NEWS:
	    last_read = ch->pcdata->last_news;
	    break;
	case NOTE_CHANGES:
	    last_read = ch->pcdata->last_changes;
	    break;
    }

    if (atol(row[6]) <= last_read)
	return TRUE;

    if (!str_cmp(ch->name,row[1]))
	return TRUE;

    if (!is_note_to(ch,row[1],row[3]))
	return TRUE;

    return FALSE;
}

void update_read(CHAR_DATA *ch, long stamp, int type)
{

    if (IS_NPC(ch))
		return;
    switch (type)
    {
        default:
            return;
        case NOTE_NOTE:
	    ch->pcdata->last_note = UMAX(ch->pcdata->last_note,stamp);
            break;
        case NOTE_IDEA:
	    ch->pcdata->last_idea = UMAX(ch->pcdata->last_idea,stamp);
            break;
        case NOTE_PENALTY:
	    ch->pcdata->last_penalty = UMAX(ch->pcdata->last_penalty,stamp);
            break;
        case NOTE_NEWS:
	    ch->pcdata->last_news = UMAX(ch->pcdata->last_news,stamp);
            break;
        case NOTE_CHANGES:
	    ch->pcdata->last_changes = UMAX(ch->pcdata->last_changes,stamp);
            break;
    }
}
void add_prof_affect(CHAR_DATA *ch, char *name, int duration, bool fInvis = TRUE);
bool is_affected_prof(CHAR_DATA *ch, char *prof);

void parse_note( CHAR_DATA *ch, char *argument, int type )
{
    BUFFER *buffer;
    char buf[MAX_STRING_LENGTH], query[MSL];
    char arg[MAX_INPUT_LENGTH];
    MYSQL_RES *res;
    MYSQL_ROW row;
    char *list_name;
    int vnum;
    int anum;

    switch(type)
    {
	default:
	    return;
        case NOTE_NOTE:
	    list_name = "notes";
            break;
        case NOTE_IDEA:
	    list_name = "ideas";
            break;
        case NOTE_PENALTY:
	    list_name = "penalties";
            break;
        case NOTE_NEWS:
	    list_name = "news";
            break;
        case NOTE_CHANGES:
	    list_name = "changes";
            break;
    }

    argument = one_argument( argument, arg );
    smash_tilde( argument );

    if ( arg[0] == '\0' || !str_prefix( arg, "read" ) )
    {
        bool fAll;

        if ( !str_cmp( argument, "all" ) )
        {
            fAll = TRUE;
            anum = 0;
        }

        else if ( argument[0] == '\0' || !str_prefix(argument, "next"))
        /* read next unread note */
        {
            vnum = 0;
	    sprintf(query,"SELECT * FROM notes WHERE type=%d ORDER BY timestamp ASC",type);
	    res	= one_query_res(query);
            while((row=mysql_fetch_row(res)))
            {
                if (!hide_note(ch,row))
                {
                    sprintf( buf, "[%3d] %s: %s\n\r%s\n\rTo: %s\n\r",
                        vnum, row[1], row[4], row[2], row[3]);
                    send_to_char( buf, ch );
                    page_to_char( row[5], ch );
                    update_read(ch,atol(row[6]),atoi(row[0]));
		    mysql_free_result(res);
                    return;
                }
		else if(is_note_to(ch,row[1],row[3]))
		    vnum++;
            }
	    sprintf(buf,"You have no unread %s.\n\r",list_name);
	    send_to_char(buf,ch);
	    mysql_free_result(res);
            return;
        }

        else if ( is_number( argument ) )
        {
            fAll = FALSE;
            anum = atoi( argument );
        }
        else
        {
            send_to_char( "Read which number?\n\r", ch );
            return;
        }

        vnum = 0;
	sprintf(query,"SELECT * FROM notes WHERE type=%d ORDER BY timestamp ASC",type);
	res	= one_query_res(query);
        while((row=mysql_fetch_row(res)))
        {
            if (is_note_to(ch,row[1],row[3]) && (vnum++ == anum))
            {
                sprintf( buf, "[%3d] %s: %s\n\r%s\n\rTo: %s\n\r",
                        anum, row[1], row[4], row[2], row[3]);
                send_to_char( buf, ch );
                page_to_char( row[5], ch );
		update_read(ch,atol(row[6]),atoi(row[0]));
		mysql_free_result(res);
                return;
            }
        }

	sprintf(buf,"There aren't that many %s.\n\r",list_name);
	send_to_char(buf,ch);
	mysql_free_result(res);
        return;
    }

    if ( !str_prefix( arg, "list" ) )
    {
	vnum = 0;
	sprintf(query,"SELECT * FROM notes WHERE type=%d ORDER BY timestamp ASC",type);
	res	= one_query_res(query);

        while((row=mysql_fetch_row(res)))
	{
	    if (is_note_to(ch,row[1],row[3]))
	    {
		sprintf( buf, "[%3d%s] %s: %s\n\r",
		    vnum, hide_note(ch,row) ? " " : "N",
		    row[1], row[4]);
		send_to_char( buf, ch );
		vnum++;
	    }
	}
	if (!vnum)
	{
	    switch(type)
	    {
		case NOTE_NOTE:	
		    send_to_char("There are no notes for you.\n\r",ch);
		    break;
		case NOTE_IDEA:
		    send_to_char("There are no ideas for you.\n\r",ch);
		    break;
		case NOTE_PENALTY:
		    send_to_char("There are no penalties for you.\n\r",ch);
		    break;
		case NOTE_NEWS:
		    send_to_char("There is no news for you.\n\r",ch);
		    break;
		case NOTE_CHANGES:
		    send_to_char("There are no changes for you.\n\r",ch);
		    break;
	    }
	}
	mysql_free_result(res);
	return;
    }

    if ( !str_prefix( arg, "remove" ) )
    {
        if ( !is_number( argument ) )
            return send_to_char( "Note remove which number?\n\r", ch );


        anum = atoi( argument );
        vnum = 0;
	sprintf(query,"SELECT * FROM notes WHERE type=%d ORDER BY timestamp ASC",type);
	res	= one_query_res(query);
        while((row=mysql_fetch_row(res)))
	{
            if (!str_cmp(ch->true_name, row[1]) && vnum++ == anum )
            {
                sprintf(query,"DELETE FROM notes WHERE timestamp=%s AND sender=\"%s\"", row[6], row[1]);
		one_query(query);
                send_to_char( "Ok.\n\r", ch );
		mysql_free_result(res);
                return;
            }
        }

	send_to_char("You must provide the number of a note you have written to remove.\n\r",ch);
	mysql_free_result(res);
        return;
    }

    if ( !str_prefix( arg, "delete" ) && get_trust(ch) >= MAX_LEVEL - 2)
    {
        if ( !is_number( argument ) )
            return send_to_char( "Note delete which number?\n\r", ch );

        anum = atoi( argument );
        vnum = 0;
        sprintf(query,"SELECT * FROM notes WHERE type=%d ORDER BY timestamp ASC",type);
	res	= one_query_res(query);
        while((row=mysql_fetch_row(res)))
	{
            if ( is_note_to( ch,row[1],row[3] ) && vnum++ == anum )
            {
                sprintf(query,"DELETE FROM notes WHERE timestamp=%s AND sender=\"%s\"", row[6], row[1]);
		one_query(query);
		send_to_char("Ok.\n\r",ch);
		mysql_free_result(res);
		return;
            }
        }

 	sprintf(buf,"There aren't that many %s.\n\r",list_name);
	send_to_char(buf,ch);
	mysql_free_result(res);
        return;
    }

    /* below this point only certain people can edit notes */
    if ((type == NOTE_NEWS && !IS_TRUSTED(ch,ANGEL))
    ||  (type == NOTE_CHANGES && !IS_TRUSTED(ch,CREATOR)))
    {
	sprintf(buf,"You aren't high enough level to write %s.",list_name);
	send_to_char(buf,ch);
	return;
    }

    if ( !str_cmp( arg, "+" ) )
    {
	note_attach( ch,type );
	if (ch->pnote->type != type)
	    return send_to_char("You already have a different note in progress.\n\r",ch);

	if (strlen(ch->pnote->text)+strlen(argument) >= 4096)
	    return send_to_char( "Note too long.\n\r", ch );

 	buffer = new_buf();

	add_buf(buffer,ch->pnote->text);
	add_buf(buffer,argument);
	add_buf(buffer,"\n\r");
	free_pstring( ch->pnote->text );
	ch->pnote->text = palloc_string( buf_string(buffer) );
	free_buf(buffer);
	send_to_char( "Ok.\n\r", ch );
	return;
    }

    if (!str_cmp(arg,"-"))
    {
 	int len;
	bool found = FALSE;

	note_attach(ch,type);
        if (ch->pnote->type != type)
            return send_to_char("You already have a different note in progress.\n\r",ch);

	if (ch->pnote->text == NULL || ch->pnote->text[0] == '\0')
	    return send_to_char("No lines left to remove.\n\r",ch);

	strcpy(buf,ch->pnote->text);

	for (len = strlen(buf); len > 0; len--)
 	{
	    if (buf[len] == '\r')
	    {
		if (!found)  /* back it up */
		{
		    if (len > 0)
			len--;
		    found = TRUE;
		}
		else /* found the second one */
		{
		    buf[len + 1] = '\0';
		    free_pstring(ch->pnote->text);
		    ch->pnote->text = palloc_string(buf);
		    return;
		}
	    }
	}
	buf[0] = '\0';
	free_pstring(ch->pnote->text);
	ch->pnote->text = palloc_string(buf);
	return;
    }

    if ( !str_prefix( arg, "subject" ) )
    {
	note_attach( ch,type );
        if (ch->pnote->type != type)
            return send_to_char("You already have a different note in progress.\n\r",ch);

	free_pstring( ch->pnote->subject );
	ch->pnote->subject = palloc_string( argument );
	send_to_char( "Ok.\n\r", ch );
	return;
    }

    if ( !str_prefix( arg, "to" ) )
    {
	note_attach( ch,type );
        if (ch->pnote->type != type)
            return send_to_char("You already have a different note in progress.\n\r",ch);

	if (is_name("all", argument)
	&& !IS_IMMORTAL(ch)
	&& !IS_HEROIMM(ch)
	&& !(ch->pcdata->induct == CABAL_LEADER))
		return	send_to_char("Sorry, you can't do that!\n\r",ch);
	if (is_number(argument) && !IS_IMMORTAL(ch))
		return send_to_char("You can't do that.\n\r",ch);	
	free_pstring( ch->pnote->to_list );
	ch->pnote->to_list = palloc_string( argument );
	send_to_char( "Ok.\n\r", ch );
	return;
	}

    if ( !str_prefix( arg, "clear" ) )
    {
	if ( ch->pnote != NULL )
	{
	    free_note(ch->pnote);
	    ch->pnote = NULL;
	}

	send_to_char( "Ok.\n\r", ch );
	return;
    }

    if ( !str_prefix( arg, "show" ) )
    {
	if ( ch->pnote == NULL )
	    return send_to_char( "You have no note in progress.\n\r", ch );

	if (ch->pnote->type != type)
	    return send_to_char("You aren't working on that kind of note.\n\r",ch);

	sprintf( buf, "%s: %s\n\rTo: %s\n\r",
	    ch->pnote->sender,
	    ch->pnote->subject,
	    ch->pnote->to_list
	    );
	send_to_char( buf, ch );
	send_to_char( ch->pnote->text, ch );
	return;
    }

    if ( !str_prefix( arg, "post" ) || !str_prefix(arg, "send"))
    {
	char *strtime;

	if ( ch->pnote == NULL )
	    return send_to_char( "You have no note in progress.\n\r", ch );

        if (ch->pnote->type != type)
            return send_to_char("You aren't working on that kind of note.\n\r",ch);

	if (!str_cmp(ch->pnote->to_list,""))
	    return send_to_char("You need to provide a recipient (name, all, or immortal).\n\r", ch);

	if (!str_cmp(ch->pnote->subject,""))
	    return send_to_char("You need to provide a subject.\n\r",ch);
	if (is_affected_prof(ch, "note_written") && !IS_IMMORTAL(ch))
	    return send_to_char("You have written a note too recently.\n\r",ch);
	ch->pnote->next			= NULL;
	strtime				= ctime( &current_time );
	strtime[strlen(strtime)-1]	= '\0';
	ch->pnote->date			= palloc_string( strtime );
	ch->pnote->date_stamp		= current_time;

	append_note(ch->pnote);
	ch->pnote = NULL;
	send_to_char("Note sent.\n\r",ch);
	add_prof_affect(ch, "note_written", 4, TRUE);
	return;
    }

    send_to_char( "You can't do that.\n\r", ch );
    return;
}

