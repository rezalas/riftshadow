//Dev's awesome databased helpfiles. Yay.
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mysql.h>
#include <ctype.h>
#include "merc.h"
#include "db.h"
#include "lookup.h"
#include "tables.h"
#include "interp.h"
#include "help.h"

int can_see_help(CHAR_DATA *ch, MYSQL_ROW row, bool fOnlyResult)
{
	int sn = skill_lookup(row[2]);

	if(!str_cmp(row[2],"olc"))
	{
		if(ch->pcdata->security > 0)
			return TRUE;
		else
			return FALSE;
	}
	
	if(atoi(row[3]) > ch->level && ch->level > 0)
		return FALSE;

	if(atoi(row[3]) == 51 && (!str_cmp(row[2],"heroimm") && !IS_HEROIMM(ch)))
		return FALSE;

	if(sn > 1 && get_skill(ch,sn) < 1)
	{
		send_to_char("Viewing that helpfile requires a skill you do not have.\n\r",ch);
		return FALSE;
	}
	return TRUE;
}
void show_helpfile(CHAR_DATA *ch, MYSQL_ROW row)
{
	char buf[MSL];
      	sprintf(buf, "%s\n\r%s\n\r",
                        row[1], row[4]);
      	send_to_char(buf,ch);
}

void modhelp_end_fun(CHAR_DATA *ch, char *argument)
{
	MYSQL *conn;
	char query[MSL*3], results[MSL];
	conn = open_conn();
        sprintf(query,"UPDATE helpfiles SET helpdata=\"%s\" WHERE id=%d", argument, ch->pcdata->helpid);
        mysql_query(conn, query);
        sprintf(results,"%i helpfiles updated.\n\r", (int)mysql_affected_rows(conn));
	send_to_char(results, ch);
	do_disc(conn);
}
void do_modhelp(CHAR_DATA *ch, char *argument)
{
	MYSQL *conn;
	MYSQL_RES *res_set;
	MYSQL_ROW row;
	char query[MSL*2];
	int id;
	if(!str_cmp(argument,"") || !is_number(argument))
		return send_to_char("Syntax: modhelp <id>\n\r",ch);
	conn = open_conn();
        if(!conn)
                return send_to_char("Error opening help database.\n\r",ch);
	id = atoi(argument);
	sprintf(query, "SELECT * from helpfiles where id=%d", id);
	mysql_query(conn,query);
        res_set = mysql_store_result (conn);
        if (res_set == NULL)
                        send_to_char("Error accessing that id number.\n\r",ch);
	else
	{
			row = mysql_fetch_row (res_set);
			ch->pcdata->helpid	 = id;
			ch->pcdata->entered_text = palloc_string(row[4]);
			enter_text(ch, modhelp_end_fun);
			mysql_free_result(res_set);
	}
        do_disc(conn);

}

void do_help(CHAR_DATA *ch, char *argument)
{
	int numresults=0;
	MYSQL *conn;
	MYSQL_RES *res_set = NULL, *res_set2 = NULL;
	MYSQL_ROW row;
	char query[MSL*2], buf[MSL];

	if(!str_cmp(argument,""))
		return do_help(ch, "topics");
	else
	{
		if((!is_alphanum(argument) && !have_space(argument)) || have_schar(argument))
			return do_help(ch, "topics");
	}

	conn = open_conn();
	if(!conn)
		return send_to_char("Error opening help database.\n\r",ch);

	if(is_number(argument))
		sprintf(query,"select * from helpfiles where id=%d", atoi(argument));
	else if(!is_number(argument))
		sprintf(query,"select * from helpfiles where title RLIKE '%s' ORDER BY id ASC", argument);

	mysql_query(conn,query);
	res_set = mysql_store_result(conn);
	numresults = mysql_affected_rows(conn);
	
	if(!numresults || (res_set == NULL && mysql_field_count(conn)>0))
		send_to_char("No matching helpfiles found.\n\r",ch);
	else if(numresults == 1)
	{
		row = mysql_fetch_row (res_set);
		if(!can_see_help(ch, row, TRUE))
			send_to_char("No matching helpfiles found.\n\r",ch);
		else
			show_helpfile(ch, row);
	}
	else
	{
		sprintf(query,"select * from helpfiles where title RLIKE '\\'%s\\'' OR title = '%s'", argument, argument);
		mysql_query(conn,query);
		res_set2 = mysql_store_result(conn);
		numresults = mysql_affected_rows(conn);
		if(numresults > 0)
		{
			row = mysql_fetch_row(res_set2);
			if(!can_see_help(ch, row, FALSE))
				send_to_char("No matching helpfiles found.\n\r", ch);
			else
			{
				show_helpfile(ch, row);
				mysql_free_result(res_set2);
			}
		}
		else
		{
			send_to_char("Multiple helpfiles matched your request:\n\r",ch);
			while((row = mysql_fetch_row (res_set)) != NULL)
        	{
	   			if(!can_see_help(ch,row, FALSE))
					continue;
	   			sprintf(buf,"%-5s %s\n\r", row[0], row[1]);
	   			send_to_char(buf,ch);
        	}
		}
	}
	mysql_free_result (res_set);
	do_disc(conn);
}
void do_delhelp(CHAR_DATA *ch, char *argument)
{
	MYSQL *conn;
	char buf[MSL];
	int id;
	if(!str_cmp(argument,""))
	{
		send_to_char("Syntax: delhelp <id #>\n\r",ch);
		send_to_char("        Deletes the helpfile with the given ID number.\n\r",ch);
		return;
	}
	conn = open_conn();
        if(!conn)
                return send_to_char("Error opening help database.\n\r",ch);
	if(!is_number(argument))
		return send_to_char("Argument must be a number.\n\r",ch);
	id = atoi(argument);
	sprintf(buf,"DELETE FROM helpfiles WHERE id=%d", id);
	mysql_query(conn, buf);
	sprintf(buf,"%i helpfiles deleted.\n\r", (int)mysql_affected_rows(conn));
	send_to_char(buf,ch);
	do_disc(conn);
}

void addhelp_end_fun(CHAR_DATA *ch, char *argument)
{
	send_to_char("Help text modified.\n\r",ch);
	send_to_char("Use addhelp to specify the rest of your helpfiles settings and add it.\n\r",ch);
	return;
}
void do_addhelp(CHAR_DATA *ch, char *argument)
{
	MYSQL *conn;
	char buf[MSL*3], title[MSL], skill[MSL], arg[MSL], *ttitle;
	char *escape, *escape2;
	int minlevel;
	if(!str_cmp(argument,""))
	{
		send_to_char("Syntax: addhelp edit\n\r",ch);
		send_to_char("Syntax: addhelp 'help title' 'skill required' minimumlevel\n\r",ch);
		send_to_char("Example: addhelp 'HELP NEW FORMAT' 'none' 0\n\r",ch);
		send_to_char("Any words in the title will be keywords, case insensitive, that the help command searches for.\n\r",ch);
		send_to_char("The skill required is the name of a skill you must have at 1% or greater to view the help.\n\r",ch);
		send_to_char("It should only be used for cabal power helpfiles and other semi-secret helpfiles and should be 'none' otherwise.\n\r",ch);
		send_to_char("Minimum level is the minimum level required to view this helpfile, and should have a 0 if it's not\n\r",ch);
		send_to_char("an immortal or level dependant helpfile.\n\r",ch);
		send_to_char("Addhelp edit will allow you to change and add the actual text of your helpfile, and must be done first.\n\r",ch);
		return;
	}
	if(!str_cmp(argument, "edit"))
		return enter_text(ch, addhelp_end_fun);
	if(!ch->pcdata->entered_text || ch->pcdata->entered_text[0] == '\0' || !str_cmp(ch->pcdata->entered_text, ""))
		return send_to_char("You haven't specified the contents of your helpfile.\n\r",ch);
	conn = open_conn();
        if(!conn)
                return send_to_char("Error opening help database.\n\r",ch);
	argument = one_argument(argument,title);
	argument = one_argument(argument,skill);
	argument = one_argument(argument,arg);
	ttitle = palloc_string(upstring(title));
	minlevel = atoi(arg);
	escape = (char *)escape_string(ttitle);
	escape2 = (char *)escape_string(ch->pcdata->entered_text);
	sprintf(buf,"INSERT INTO helpfiles VALUES(NULL, \"%s\", \"%s\", %d, \"%s\")",
		escape, skill, minlevel, escape2);
	mysql_query(conn, buf);
	sprintf(buf,"Help file added:\n\rTitle: %s\n\rSkill Required: %s\n\rMinimum Level: %d\n\rHelp Text: %s\n\r",
			ttitle, skill, minlevel, ch->pcdata->entered_text);
	send_to_char(buf,ch);
	do_disc(conn);
}

void do_listhelp(CHAR_DATA *ch, char *argument)
{
	MYSQL		*conn;
	MYSQL_FIELD	*field;
	MYSQL_ROW	row;
	MYSQL_RES 	*res_set;
	char buf[MSL], query[MSL], arg1[MSL];
	if(!str_cmp(argument,""))
	{
		send_to_char("Syntax: listhelp all\n\r",ch);
		send_to_char("        listhelp <field> <value>\n\r",ch);
		send_to_char("        Valid fields are: id, title, skill, minlevel, helpdata\n\r",ch);
		send_to_char("        You can search for substrings by entering them normally.\n\r",ch);
		return;
	}
	argument = one_argument(argument,arg1);
	if(!str_cmp(arg1,"ALL"))
		sprintf(query, "select * from helpfiles");
	else
		sprintf(query, "select * from helpfiles where %s RLIKE '%s'", arg1, argument);
	conn = open_conn();
	if(!conn)
		return send_to_char("Error opening help database.\n\r",ch);
	mysql_query (conn, query);
	res_set = mysql_store_result (conn);
	if (res_set == NULL || mysql_field_count(conn)<1)
			send_to_char("Error accessing results.\n\r",ch);
	else
	{
		while((row = mysql_fetch_row (res_set)) != NULL)
		{
			mysql_field_seek (res_set, 0);
			field	= mysql_fetch_field (res_set);
                	sprintf(buf, "Help ID: %s\n\rHelp Title: %s\n\rRequired Skill: %s\n\rMinimum Level: %s\n\r",
                        	row[0], row[1], row[2], row[3]);
                	send_to_char(buf,ch);
        	        sprintf(buf,"Help Text:\n\r%s\n\r", row[4]);
	                send_to_char(buf,ch);
		}
		mysql_free_result(res_set);
	}
	do_disc (conn);
}

MYSQL * do_conn (char *host_name, char *user_name, char *password, char *db_name,
			unsigned int port_num, char *socket_name, unsigned int flags)
{
MYSQL	*conn;	/* pointer to connection handler */

	conn = mysql_init (NULL);	/* allocate, initialize connection handler */
	if (conn == NULL)
	{
		print_error (NULL, "mysql_init() failed (probably out of memory)");
		return (NULL);
	}
	if (mysql_real_connect (conn, host_name, user_name, password,
					db_name, port_num, socket_name, flags) == NULL)
	{
		print_error (conn, "mysql_real_connect() failed");
		return (NULL);
	}
	return (conn);			/* connection is established */
}

void do_disc (MYSQL *conn)
{
	mysql_close (conn);
}

void print_error (MYSQL *conn, char *message)
{
	fprintf (stderr, "%s\n", message);
	if (conn != NULL)
	{
		fprintf (stderr, "Error %u (%s)\n",
				mysql_errno (conn), mysql_error (conn));
	}
}

