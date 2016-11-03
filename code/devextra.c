/****************************************************************************/
/* Dev's happy place.							    */
/****************************************************************************/
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mysql.h>
#include "merc.h"
#include "magic.h"
#include "recycle.h"
#include "db.h"
#include "lookup.h"
#include "tables.h"
#include "time.h"
#include "interp.h"
#include "help.h"

#define LOGIN_LOG_FILE       "logins.txt"
#define BOUNTY_LOG_FILE	     "bounties.txt"
#define TEMP_GREP_RESULTS_TWO "../temp/tempgrepresults2.tmp"
#define HISTORY_FILE	     "history.txt"
char * flags_to_string args ((CHAR_DATA *ch, const struct flag_type *showflags, int flagsperline));
void bounty_log args ((char *string));
void bounty_credit args((CHAR_DATA *ch, int credit));
void sortvotes(CHAR_DATA *ch, BALLOT_DATA *ballot);
void clean_mud args((void));
int display_lookup args((const char *name, const struct display_type *flag_table));
int flag_lookup args((const char *name, const struct flag_type *flag_table));

bool IS_IMP(CHAR_DATA *ch)
{
	if(get_trust(ch)==MAX_LEVEL)
		return TRUE;
	return FALSE;
}
float calculate_inflation()
{
	float temp;
	temp = (float)(1 + 3* ((float)player_gold / (float)total_gold));
	return temp;
}
char * escape_string(char *string)
{
	char txt[MSL];
	mysql_escape_string(txt, string, strlen(string));
	return talloc_string(txt);
}
void do_pswitch(CHAR_DATA *ch, char *argument)
{
	DESCRIPTOR_DATA *d;
	char name[MAX_STRING_LENGTH];
	char buf[MAX_STRING_LENGTH];
	CHAR_DATA *victim;

	argument = one_argument(argument,name);
	if(get_trust(ch) < MAX_LEVEL)
		return send_to_char("Huh?\n\r",ch);

	if (name[0] == '\0' || argument[0] == '\0')
		return send_to_char("Syntax: pswitch <char> to load a player and switch into them\n\rpswitch end to return",ch);

	name[0] = UPPER(name[0]);
	return;
	if (get_char_world(ch,name) != NULL)
		return send_to_char("That character is already online!\n\r",ch);

	d = new_descriptor();

	if (!load_char_obj(d,name))
		return send_to_char("No such character exists.\n\r",ch);

	sprintf(buf,"cp %s%s%s %spload.txt",PLAYER_DIR,name,".plr",PLAYER_DIR);
	system(buf);

	d->character->desc = NULL;
	d->character->next = char_list;
	char_list = d->character;
	d->outsize = 2000;
	d->outbuf = new char [d->outsize];
	d->connected = CON_PLAYING;
	reset_char(d->character);
	victim = d->character;
	d->character->pcdata->host = palloc_string("PLOAD");	
}
void do_gold(CHAR_DATA *ch, char *argument)
{
	char buf[MSL];
	float gc;
	if(is_number(argument) && get_trust(ch) == MAX_LEVEL)
	{
		gold_constant = atoi(argument);
		total_gold = num_pfiles * gold_constant;
		act("Global gold constant rate is now set to $t.",ch,argument,0,TO_CHAR);
		return;
	}
	act("Riftshadow Gold System",ch,0,0,TO_CHAR);
	act("----------------------",ch,0,0,TO_CHAR);
	sprintf(buf,"Total gold in world  : %ld (%d players)\n\r", total_gold, num_pfiles);
	send_to_char(buf,ch);
	sprintf(buf,"Gold held by players : %ld (%ldgp mobile held)\n\r", player_gold, total_gold-player_gold);
	send_to_char(buf,ch);
	if(total_gold > 0)
		gc = (float)(1 + 3 * ((float)player_gold / (float)total_gold))*(float)100;
	else
		gc = 0;
	sprintf(buf,"Global inflation rate: %.1f%% (gold constant %ld)\n\r", gc, gold_constant);
	send_to_char(buf,ch);
}

void do_clean(CHAR_DATA *ch, char *argument)
{
	send_to_char("Cleaning logins...\n\r",ch);
	send_to_char("Cleaning notes...\n\r",ch);
	send_to_char("Cleaning offerings...\n\r",ch);
	send_to_char("Cleaning pklogs...\n\r",ch);
	send_to_char("Autodeleting players...\n\r",ch);
	send_to_char("Comparing actual records in players vs pfiles..\n\r", ch);
	clean_mud();
	send_to_char("Done.\n",ch);
}
void clean_mud()
{
	char buf[MSL], tbuf[MSL];
	int cres=0;
	CRow row;
	CSQLInterface nSQL;
	DESCRIPTOR_DATA *d;

	nSQL.StartSQLServer();
	//5184000 = one month
	sprintf(buf,"DELETE FROM logins WHERE ctime + 5184000 < %ld", current_time);
	one_query(buf);
	sprintf(buf,"DELETE FROM notes WHERE timestamp + 2592000 < %ld",current_time);
	one_query(buf);
	sprintf(buf,"DELETE FROM offerings WHERE time + 2592000 < %ld",current_time);
	one_query(buf);
	sprintf(buf,"DELETE FROM pklogs WHERE ctime + 5184000 < %ld",current_time);
	one_query(buf);

	//autodelete...
	cres = RS.SQL.Select("name FROM players WHERE lastlogin + 2592000 < %ld",current_time);
	if(cres)
	{
		d = new_descriptor();

		while(!RS.SQL.End())
		{
			row = RS.SQL.GetRow();
			strcpy(&tbuf[0],row[0]);
			if(!load_char_obj(d,tbuf))
			{
				free_char(d->character);
				continue;
			}
	        	d->character->desc = NULL;
			if(d->character->level>=30)
				perm_death_log(d->character, 4);
			delete_char(d->character->true_name, TRUE);
		}
	
		free_descriptor(d);
	}
	//compare db to pfiles
	cres = RS.SQL.Select("name FROM players");
	if(cres)
	{
		d = new_descriptor();
	
		while(!RS.SQL.End())
		{
			row = RS.SQL.GetRow();
			strcpy(&tbuf[0],row[0]);
			if(!load_char_obj(d, tbuf))
			{
				nSQL.Delete("players WHERE name = '%s'", row[0]);
				log_string("Deleting player...");
			}
			free_char(d->character);
		}
		free_descriptor(d);
	}
}


void do_listoffer(CHAR_DATA *ch, char *argument)
{
	MYSQL_RES *res_set;
	MYSQL_ROW row;
	char query[MSL], buf[MSL], result[200], arg1[MSL], arg2[MSL];
	bool autol = FALSE, found = FALSE;
	int status, i = 1, argnum;
	long ttime;
	if(!str_cmp(argument,"auto"))
		autol = TRUE;
	sprintf(query,"SELECT * FROM offerings WHERE deity = \"%s\" %s ORDER BY time ASC",
		ch->true_name, autol ? "AND status = 0" : "");
	res_set = one_query_res(query);
	if(autol && !mysql_num_rows(res_set))
		return mysql_free_result(res_set);
	else if(autol)
	{
		mysql_free_result(res_set);
		return send_to_char("You have unviewed offerings in your shrine.\n\r",ch);
	}
	if(strstr(argument,"accept") || strstr(argument,"reject"))
	{
		argument = one_argument(argument,arg1);
		argument = one_argument(argument,arg2);
		argnum = atoi(arg2);
		while((row = mysql_fetch_row(res_set)))
		{
			if(i++ < argnum || argnum < 1)
				continue;
			if(!str_cmp(arg1,"reject"))
				status = 1;
			else
				status = 2;
			sprintf(query,"UPDATE offerings SET status=%i WHERE time=%s AND player='%s'",
				status, row[5], row[3]);
			one_query(query);
			sprintf(buf,"%s's offering has been %sed.\n\r", row[3], arg1);
			send_to_char(buf,ch);
			found = TRUE;
			break;
		}
		if(!found)
			send_to_char("That offering wasn't found.\n\r",ch);
		return mysql_free_result(res_set);
	}
	if(is_number(argument))
	{
		argnum = atoi(argument);
		while((row = mysql_fetch_row(res_set)))
		{
			if(i++ < argnum || argnum < 1)
				continue;
			status = atoi(row[4]);
			ttime = atol(row[5]);
			strftime(result, 200, "%a %b %d, %l:%M%P", localtime(&ttime));
			sprintf(buf,"Offering %d:\n\rFrom: %s\n\rTime: %s\n\rStatus: %s\n\rOffering is %s (Vnum %s)\n\r",
				argnum, row[3], result, status == 0 ? "New" : status == 1 ? "Rejected" : status == 2
				? "Approved" : "", row[2], row[1]);
			send_to_char(buf,ch);
			found = TRUE;
			break;
		}
		mysql_free_result(res_set);
		if(!found)
			send_to_char("You have no offerings by that number.\n\r",ch);
		return;
	}
	send_to_char("Offering listing:\n\r",ch);
	while((row = mysql_fetch_row(res_set)))
	{
		status = atoi(row[4]);
		sprintf(buf,"[%3i%c]  %s offered %s\n\r", i++, status == 0 ? 'N' : status == 1 ? 'R' : status == 2 ? 'A' : ' ',
		row[3], row[2]);
		send_to_char(buf,ch);
		found = TRUE;
	}
	if(!found)
		send_to_char("There are currently no offerings to you.\n\r",ch);
	send_to_char("Use listoffer <number> to view an offering, and listoffer accept/reject <number> to accept it or not.\n\r",ch);
	mysql_free_result(res_set);
}

void do_offer(CHAR_DATA *ch, char *argument)
{
	MYSQL_ROW row;
	OBJ_DATA *obj, *altar;
	char query[MSL];
	char *escape;
	int status;
	//status: 0 unread 1 rejected 2 accepted
        for(altar = object_list; altar; altar = altar->next)
                if(altar->in_room && altar->item_type == ITEM_ALTAR && altar->in_room == ch->in_room)
                        break;
	if(!altar || !altar->in_room->owner || !str_cmp(altar->in_room->owner,""))
		return send_to_char("There's no altar here.\n\r",ch);
	if(!str_cmp(argument,""))
	{
		sprintf(query, "SELECT * FROM offerings WHERE deity = \"%s\" AND player = \"%s\" ORDER BY time DESC LIMIT 1",
			altar->in_room->owner, ch->true_name);
		row = one_query_row(query);
		if(!row)
			return send_to_char("You find no signs of any offerings you may have made.\n\r",ch);
		status = atoi(row[4]);
		if(status == 2)
			return send_to_char("Your offering seems to have been accepted.\n\r",ch);
		if(status == 1)
			return send_to_char("The other offerings that were here have been accepted, but yours has been left untouched.\n\r",ch);
		if(status == 0)
			return send_to_char("The pile of offerings doesn't appear to have been disturbed since your last visit.\n\r",ch);
	}
        if (!(obj = get_obj_carry(ch, argument, ch)))
		return send_to_char("You don't have that item.\n\r", ch);
        if (!can_drop_obj(ch, obj))
		return send_to_char("You can't let go of it.\n\r", ch);
	escape = (char *)escape_string(obj->short_descr);
	sprintf(query, "INSERT INTO offerings VALUES(\"%s\", %d, \"%s\", '%s', 0, %ld, NULL)",
		altar->in_room->owner, obj->pIndexData->vnum, escape, ch->true_name, current_time);
	one_query(query);
	act("You place $p on $P, offering it to your deity.",ch,obj,altar,TO_CHAR);
	act("$n places $s offering of $p on $P.",ch,obj,altar,TO_ROOM);
	extract_obj(obj);
}

void do_sitetrack(CHAR_DATA *ch, char *argument)
{
	MYSQL *conn, *conn2;
	MYSQL_RES *res_set, *res2;
	MYSQL_ROW row, row2;
	BUFFER *buffer;
	char buf[MSL], arg1[MSL], arg2[MSL], query[MSL];
	char *escape;
	int id, results = 0;
	if(IS_NPC(ch))
		return;
	conn	= open_conn();
	if(!strcmp(argument,""))
	{
		buffer	= new_buf();
		conn2	= open_conn();
		sprintf(query,"SELECT site_id, denials, site_name from sitetracker");
		mysql_query(conn, query);
		res_set = mysql_store_result(conn);
		add_buf(buffer,"ID #     Site                       Denials    Comments\n\r");
		while((row = mysql_fetch_row (res_set)) != NULL)
		{
			id = atoi(row[0]);
			sprintf(query,"SELECT site_id FROM sitecomments WHERE site_id = %d", id);
			mysql_query(conn2, query);
			res2 = mysql_store_result(conn2);
			sprintf(buf,"%4d     %-26s %-10s %ld\n\r", id, row[2], row[1], (long) mysql_affected_rows(conn2));
			mysql_free_result(res2);
			add_buf(buffer, buf);
		}
		add_buf(buffer,"Use sitetrack <id> to view more details.\n\r");
		add_buf(buffer,"Use sitetrack comment <id> to enter a comment on the given site.\n\r");
		add_buf(buffer,"Use sitetrack add <site IP, i.e. aol.com>.  Use numeric IP ONLY if it's unresolvable.\n\r");
		if(get_trust(ch)>=58)
			add_buf(buffer,"Use sitetrack delcomment <comment id> to delete a comment, and sitetrack delsite <site id> to delete a site.\n\r");
		page_to_char(buf_string(buffer),ch);
		mysql_free_result(res_set);
		do_disc(conn);
		do_disc(conn2);
		free_buf(buffer);
		return;
	}
	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);
	if(!str_cmp(arg1,"add"))
	{
		if(!strcmp(arg2,""))
			return send_to_char("You must specify the site you wish to add.\n\r",ch);
		escape = (char *)escape_string(arg2);
		sprintf(query, "INSERT INTO sitetracker VALUES(NULL, '%s',0)", escape);
		one_query(query);
		sprintf(buf,"A new site (%s) was added to the IP listings.  You should add a comment now explaining why it was added.\n\r",
		arg2);
		do_disc(conn);
		send_to_char(buf, ch);
		return;
	}
	if(!str_cmp(arg1,"comment"))
	{
		if(!strcmp(arg2,"") || !is_number(arg2))
			return;
		ch->pcdata->entered_text = palloc_string("");
		ch->pcdata->helpid	 = atoi(arg2);
		enter_text(ch, comment_end_fun);
		do_disc(conn);
		return;
	}
	if(!str_cmp(arg1,"delcomment") && get_trust(ch)>=58)
	{
		sprintf(query,"DELETE FROM sitecomments WHERE comment_id=%s",arg2);
		one_query(query);
		do_disc(conn);
		return send_to_char("Ok.\n\r",ch);
	}
	if(!str_cmp(arg1,"delsite") && get_trust(ch)>=58)
	{
		sprintf(query,"DELETE FROM sitetracker WHERE site_id=%s",arg2);
		one_query(query);
		do_disc(conn);
		return send_to_char("Ok.\n\r",ch);
	}
	if(is_number(arg1))
	{
		buffer = new_buf();
		sprintf(query,"SELECT * from sitetracker where site_id=%s",arg1);
		mysql_query(conn,query);
		res_set = mysql_store_result(conn);
		if(!res_set || mysql_affected_rows(conn)<1)
		{
			send_to_char("Invalid ID number.\n\r",ch);
			return do_disc(conn);
		}
		row	= mysql_fetch_row(res_set);
		sprintf(buf,"Viewing %s (id %s).\n\r",row[1],row[0]);
		send_to_char(buf,ch);
		sprintf(buf,"%s players from this site have been denied.",row[2]);
		send_to_char("Comments:\n\r",ch);
		conn2	= open_conn();
		sprintf(query,"SELECT * from sitecomments where site_id=%s",arg1);
		mysql_query(conn2,query);
		res2	= mysql_store_result(conn2);
		buf[0]	= '\r';
		while((row2=mysql_fetch_row(res2)))
		{
			sprintf(buf,"Added by %s on %s (CID #%s):\n\r%s",row2[2],row2[3],row2[1],row2[4]);
			add_buf(buffer,buf);
		}
		if(buf[0]=='\r')
			add_buf(buffer,"(No comments)\n\r");
		if(get_trust(ch)>=56)
		{
			add_buf(buffer,"Players from site:\n\r");
			mysql_free_result(res2);
			sprintf(query,"SELECT DISTINCT name FROM logins WHERE site RLIKE '%s'",row[1]);
			mysql_query(conn2,query);
			res2 = mysql_store_result(conn2);
			buf[0]='\0';
			while((row2=mysql_fetch_row(res2)))
			{
				sprintf(query,"%-12s",row2[0]);
				strcat(buf,query);
				if(++results % 5 == 0)
					strcat(buf,"\n\r");
				else
					strcat(buf," ");
			}
			if(results && results % 5)
				strcat(buf,"\n\r");
			if(!results)
				sprintf(buf,"(No players)\n\r");
			add_buf(buffer,buf);
		}
		page_to_char(buf_string(buffer),ch);
		mysql_free_result(res_set);
		mysql_free_result(res2);
		do_disc(conn);
		do_disc(conn2);
		free_buf(buffer);
	}
}

void comment_end_fun(CHAR_DATA *ch, char *argument)
{
	char query[MSL];
	char *escape;

	if(strstr(argument,"\""))
		return send_to_char("Error:  Your comment contained a double-quotation mark.  You must use single quotes to prevent errors.\n\r",ch);

	escape = (char *)escape_string(argument);
	sprintf(query, "INSERT INTO sitecomments VALUES(%d, NULL, \"%s\", \"%s\", \"%s\")",
			ch->pcdata->helpid, ch->true_name, log_time(), escape);
	one_query(query);
	sprintf(query,"Your comment was added.  Use sitetrack %d to view your comments.\n\r", ch->pcdata->helpid);
	send_to_char(query,ch);
	ch->pcdata->entered_text[0] = '\0';
}

void show_database_info(CHAR_DATA *ch, char *argument)
{
	MYSQL *conn, *conn2;
	MYSQL_RES *res_set, *res2;
	MYSQL_ROW row, row2;
	char query[MSL], buf[MSL], buf2[MSL];
	float lpercent;

	return;

	sprintf(query,"SELECT * from players where name='%s'", argument);
	conn = open_conn();
	mysql_query(conn,query);
        res_set = mysql_store_result(conn);
        if(!mysql_affected_rows(conn))
	{
		mysql_free_result(res_set);
		do_disc(conn);
		return send_to_char("  No database entry for player found.\n\r",ch);
	}
	row = mysql_fetch_row (res_set);
	conn2 = open_conn();
	if((atoi(row[11]) + atoi(row[12])) > 0) //caballed
	{
		lpercent = atoi(row[12]) / (atoi(row[11]) + atoi(row[12])) * 100;
		sprintf(buf,"  Power: %0.2f%% (%s/%d)",lpercent,row[12],atoi(row[12])+atoi(row[11]));
		send_to_char(buf,ch);
		sprintf(query,"SELECT avg(c_logins) / (avg(c_logins) + avg(noc_logins))*100 from players where c_logins>0");
		mysql_query(conn2,query);
		res2 = mysql_store_result(conn2);
		row2 = mysql_fetch_row (res2);
		sprintf(buf2," and cabal=%s",row[5]);
		strcat(query,buf2);
		row2[0][6]='\0';
		sprintf(buf," [Game-wide: %4s%%, ",row2[0]);
		send_to_char(buf,ch);
                mysql_free_result(res2);
		mysql_query(conn2,query);
		res2 = mysql_store_result(conn2);
		row2 = mysql_fetch_row (res2);
		row2[0][6]='\0';
		sprintf(buf,"Cabal-wide: %s%%]\n\r",row2[0]);
		send_to_char(buf,ch);
		mysql_free_result(res2);
	}
	mysql_free_result(res_set);
	do_disc(conn2);
	do_disc(conn);
}

void do_demo(CHAR_DATA *ch, char *name)
{
	FILE *fp;
	char tempbuf[MSL], buf[MSL];
	int i = 0, r = 0, j;
	OBJ_INDEX_DATA *pIndex;
	for(j = 0; j < 32000; j++)
		if((pIndex = get_obj_index(j)) && pIndex->limtotal > 0)
		{
			i += pIndex->limtotal - pIndex->limcount;
			r += pIndex->limcount;
		}
	sprintf(buf,"There are currently %i limited items in the game (%i on players).\n\r",i,r);
	send_to_char(buf,ch);
	return;
	
	if(!(fp=fopen("demo.txt","r")))
		return send_to_char("Error opening demographics.\n\r",ch);
	buf[0] = '\0';
        while(fgets(tempbuf,300,fp))
		send_to_char(tempbuf,ch);
	fclose(fp);
}

void delete_char(char *name, bool save_pfile)
{
	char buf1[MSL], buf2[MSL], query[MSL];
	name	= capitalize(name);
	int cres=0;
	//whack their pfile.. or maybe just move it to dead_char
	if(save_pfile)
		sprintf(buf2,"mv %s%s.plr %sdead_char/%s.plr",PLAYER_DIR,name,PLAYER_DIR,name);
	else
		sprintf(buf2,"rm %s%s.plr",PLAYER_DIR,name);
	system(buf2);
	cres = RS.SQL.Delete("players WHERE name='%s'",name);
	one_query(query);
	sprintf(query,"DELETE FROM pklogs WHERE killer='%s'",name);
	one_query(query);
	sprintf(query,"DELETE FROM theft WHERE ch='%s'",name);
	one_query(query);
	sprintf(query,"DELETE FROM offerings WHERE player='%s'",name);
	one_query(query);
	sprintf(query,"DELETE FROM logins WHERE name='%s'",name);
	one_query(query);
	sprintf(query,"DELETE FROM inductions WHERE ch='%s'",name);
	one_query(query);
}
MYSQL *open_fconn(void)
{
    return do_conn("localhost","rift","rift123","riftforum",0,NULL,0);
}

MYSQL_ROW one_query_row(char *query)
{
	MYSQL_RES *res_set;
	MYSQL_ROW row;

	res_set = one_query_res(query);
	row	= mysql_fetch_row(res_set);
	mysql_free_result(res_set);
	return row;
}

MYSQL_ROW one_fquery_row(char *query)
{
    MYSQL *conn;
    MYSQL_RES *res_set;
    MYSQL_ROW row;
				
    conn = open_fconn();
    mysql_query(conn,query);
    res_set = mysql_store_result(conn);
    row = mysql_fetch_row(res_set);
    mysql_free_result(res_set);
    do_disc(conn);
    return row;
}
MYSQL_RES* one_query_res(char *query)
{
	MYSQL *conn;
	MYSQL_RES *res_set;
        conn = open_conn();
        mysql_query(conn,query);
	res_set	= mysql_store_result(conn);
        do_disc(conn);
	return res_set;
}
int one_query_count(char *query)
{
        MYSQL *conn;
        MYSQL_RES *res_set;
	int res;
        conn = open_conn();
        mysql_query(conn,query);
        res_set = mysql_store_result(conn);
	res = mysql_affected_rows(conn);
	mysql_free_result(res_set);
        do_disc(conn);
	return res;
}
void one_query(char *query)
{
	MYSQL *conn;
	conn = open_conn();
	mysql_query(conn,query);
	do_disc(conn);
}
void enter_text(CHAR_DATA *ch, DO_FUN *end_fun)
{
	if(IS_NPC(ch))
		return;
	ch->pcdata->entering_text = TRUE;
	ch->pcdata->end_fun = end_fun;
	send_to_char("Entering text editing mode.\n\r",ch);
	send_to_char("Type 'finish' to quit and save changes, 'exit' to quit without saving changes, 'backline' to delete the last\n\r", ch);
	send_to_char("line entered, 'show' to show existing lines, and anything else to add it to the existing text.\n\r",ch);
	if(ch->pcdata->entered_text!=NULL && ch->pcdata->entered_text[0]!='\0')
	{
		send_to_char("Current text:\n\r",ch);
		send_to_char(ch->pcdata->entered_text, ch);
		return;
	}
}

char *log_time(void)
{
	char result[200];
	strftime(result, 200, "%m/%d/%Y %l:%M%P", localtime(&current_time));
	return talloc_string(result);
}

MYSQL *open_conn(void)
{
	return do_conn(MYSQL_HOST,"rift","rift123","rift", 0, NULL, 0);
}
void one_fquery(char *query)
{
	MYSQL *conn;
	conn = open_fconn();
	mysql_query(conn,query);
	do_disc(conn);
}

void plug_graveyard(CHAR_DATA *ch, int type)
{
    int minlevel=30,day, tid, i,max;
	char buf[MSL], buf2[MSL], name[MSL], message[MSL], message_date[MSL],stid[MSL];
	char align[MSL], ethos[MSL],type_death[MSL],message_death[MSL];
	char *suf;
	char ntime[MSL],year[MSL], month[MSL], dom[MSL], time[MSL],cur_date[MSL],unique[MSL];
	MYSQL_ROW qrow;


	return;

	if(ch->level < minlevel)
		return;
	
	day = time_info.day + 1;
	if ( day > 4 && day <  20 )
		suf = "th";
	else if ( day % 10 ==  1 )
		suf = "st";
	else if ( day % 10 ==  2 )
		suf = "nd";
	else if ( day % 10 ==  3 )
		suf = "rd";
	else
		suf = "th";
	sprintf( message_date,
			"On the Day %s, %d%s of the Month %s, in the Year %d,",
			day_name[day % 6],day, suf,month_name[time_info.month],time_info.year);

	if(type == 1)
	{
		sprintf(type_death,"DEL");
		sprintf(message_death,"took %s own life.", (ch->sex == 2) ? "her" : "his");
	}
	else if(type == 2)
	{
		sprintf(type_death,"CON");
		sprintf(message_death,"perished in battle.");
	}
	else if(type == 3)
	{
		sprintf(type_death,"AGE");
		sprintf(message_death,"perished of old age.");
	}
	else
	{
		sprintf(type_death,"AUTO");
		sprintf(message_death,"perished in %s sleep.", (ch->sex == 2) ? "her" : "his");
	}

	strftime(month, 200, "%m", localtime(&current_time));
	strftime(dom, 200, "%d", localtime(&current_time));
	strftime(time, 200, "%H:%M:%S", localtime(&current_time));
	strftime(ntime, 200, "%H%M%S", localtime(&current_time));
	strftime(year, 200, "%Y", localtime(&current_time));
	
	qrow = one_fquery_row("select max(zthreadid) from gabe20010201051916");

	if(qrow[0] != NULL)
	{
		sprintf(stid,"%15.0f", (float)(atoi(qrow[0]) + 1));	
   		for(i=0; stid[i]!='\0'; i++)
			if(stid[i]==' ')
				stid[i]='0';
	}
	else
	{
		sprintf(stid,"000000000000000");
	}
	
	sprintf(cur_date,"%s/%s/%s %s", month, dom,year, time);
	sprintf(unique,"%s%s%s%s",year, month, dom, ntime);
	sprintf(name,"(%s) %s%s", type_death, ch->true_name, ch->pcdata->title);
	if(ch->pcdata->extitle != NULL)
		sprintf(name,"%s%s", name, ch->pcdata->extitle);

	/* Message */
	if(ch->alignment == 1000)
		sprintf(align,"Good");
	else if(ch->alignment == 0)
		sprintf(align,"Neutral");
	else
		sprintf(align,"Evil");
	if(ch->pcdata->ethos == 1000)
		sprintf(ethos,"Lawful");
	else if(ch->pcdata->ethos == 0)
		sprintf(ethos,"Neutral");
	else
		sprintf(ethos,"Chaotic");
	sprintf(message,"%s %s %s\n\r%s%s",
			message_date,
			ch->true_name,
			message_death,
			ch->true_name,
			ch->pcdata->title);
	if(ch->pcdata->extitle != NULL)
		sprintf(message,"%s%s", message, ch->pcdata->extitle);

	sprintf(message,"%s\rLevel: %d\rAge: %d %s years old, born in the Caelumaedani year %d.\rHours: %d"\
					"\rRace: %s\rSex: %s\rClass: %s\r",
			message,
			ch->level,
			get_age(ch),
			pc_race_table[ch->race].race_time,
			ch->pcdata->birth_date,
			get_hours(ch),
			race_table[ch->race].name,
			sex_table[ch->sex].name,
			(RSTR)ch->Class()->name);

	if(ch->Class()->GetIndex() == CLASS_WARRIOR)
	{
		sprintf(message,"%sStyle Specializations: ", message);
		for(i=1; i<MAX_STYLE; i++)
		{
			if(IS_SET(ch->pcdata->styles, style_table[i].bit))
			{
				sprintf(message,"%s%s ", message, style_table[i].name);
			}
		}
		sprintf(message,"%s\r", message);
	}
	else if(ch->Class()->GetIndex() == CLASS_SORCERER)
	{
		 sprintf(message, "%s%s's major elemental focus was %s and %s para-elemental focus was %s.\r",
				message,
				ch->true_name,
				sphere_table[ch->pcdata->ele_major].name,
				(ch->sex == 2) ? "her" : "his",
				sphere_table[ch->pcdata->ele_para].name);
	}

	sprintf(message,"%sAlignment: %s\rEthos: %s", message, align, ethos);

	if(ch->cabal)
	{
		sprintf(message,"%s\rCabal: %s, %s",message,cabal_table[ch->cabal].who_name, cabal_table[ch->cabal].long_name);
		if(ch->cabal == CABAL_HORDE && ch->pcdata->tribe)
			sprintf(message,"%s, of the %s Tribe", message, palloc_string(capitalize(tribe_table[ch->pcdata->tribe].name)));
	}

	if((ch->pcdata->frags[PK_KILLS] + ch->pcdata->fragged) > 0)
		sprintf(message,"%s\rPK Ratio: %.2f%%",
			message,(ch->pcdata->frags[PK_KILLS]*100 / (ch->pcdata->frags[PK_KILLS] + ch->pcdata->fragged)));
	else
		sprintf(message,"%s\rPK Ratio: 0%%", message);

	sprintf(buf2,"insert into graveyard(Pname, Pfrags, Pfgood, Pfneutral, Pfevil, Pfdeaths, Pmdeaths, Phours) "\
				 "values('%s',%.3f, %.3f, %.3f, %.3f, %.3f, %d, %d)",
				 ch->true_name,
				 ch->pcdata->frags[PK_KILLS],
				 ch->pcdata->frags[PK_GOOD],
				 ch->pcdata->frags[PK_NEUTRAL],
				 ch->pcdata->frags[PK_EVIL],
				 ch->pcdata->fragged,
				 ch->pcdata->killed[MOB_KILLED],
				 get_hours(ch));
	one_query(buf2);
	
	sprintf(buf,"insert into gabe20010201051916(zposter,zposter_email,zsubject,zmessage,zdatetime,zaddress,zunique,"\
				"zthreadid,zdelete,zmod) values('Death_Wizard','immortals@riftshadow.com','%s','%s',"\
				"'%s','localhost',%s,'%s',0,'Death_Wizard')",
				name,message,cur_date, unique, stid);
	one_fquery(buf);
}

void do_pktrack(CHAR_DATA *ch, char *argument)
{
	MYSQL *conn;
        MYSQL_ROW       row;
        MYSQL_RES       *res_set;
	BUFFER *buffer;
	char arg1[MSL], query[MSL], qpart[MSL], buf[MSL];
	int i = 0;
	bool found = FALSE;
	if(!str_cmp(argument,""))
		return send_to_char("Syntax: pktrack <wins/losses/all/date/location> <value>\n\r",ch);
	buffer = new_buf();
	argument = one_argument(argument, arg1);
	if(!str_cmp(arg1,"wins"))
		sprintf(qpart,"killer RLIKE '%s'",argument);
        else if(!str_cmp(arg1,"losses"))
                sprintf(qpart,"victim RLIKE '%s'",argument);
        else if(!str_cmp(arg1,"all"))
                sprintf(qpart,"killer RLIKE '%s' OR victim RLIKE '%s'",argument,argument);
        else if(!str_cmp(arg1,"date"))
                sprintf(qpart,"date RLIKE '%s'",argument);
        else if(!str_cmp(arg1,"location"))
                sprintf(qpart,"room RLIKE '%s'",argument);
	else return send_to_char("Invalid option.\n\r",ch);
	sprintf(query, "SELECT * FROM pklogs WHERE %s", qpart);
	conn = open_conn();
	mysql_query(conn,query);
        res_set = mysql_store_result (conn);
        if (res_set == NULL && mysql_field_count(conn)>0)
                        send_to_char("Error accessing results.\n\r",ch);
        else if (res_set)
        {
                while((row = mysql_fetch_row (res_set)) != NULL)
                {
			sprintf(buf, "%3d) %s(%s) killed %s(%s) at %s on %s\n\r", 
					++i, row[0], cabal_table[atoi(row[1])].name, row[2], cabal_table[atoi(row[3])].name, row[5], row[4]);
			add_buf(buffer, buf);
			found = TRUE;
                }
		if(!found)
			return send_to_char("No matching results found.\n\r",ch);
                mysql_free_result (res_set);
		page_to_char(buf_string(buffer),ch);
		free_buf(buffer);
        }
	do_disc(conn);
}
bool trusts(CHAR_DATA *ch, CHAR_DATA *victim)
{
	if(IS_NPC(ch))
		return FALSE;
	if(IS_NPC(victim)) {
		if (IS_AFFECTED(victim,AFF_CHARM)
			&& victim->master == ch)
			return TRUE;
		else
			return FALSE;
	}
	if(ch == victim)
		return TRUE;
	if(IS_SET(victim->pcdata->trust,TRUST_CABAL) && is_same_cabal(ch,victim))
		return TRUE;
	if(IS_SET(victim->pcdata->trust,TRUST_GROUP) && is_same_group(ch,victim))
		return TRUE;
	if(victim->pcdata->trusting == ch)
		return TRUE;
	return FALSE;
}
void sortvotes(CHAR_DATA *ch, BALLOT_DATA *ballot)
{
/*	char *top_name[200], *tempname, buf[MSL];
	sh_int top_value[200], temptop, top = 1, i = 0, x = 0;
	int mod_votes=0,fempty=0;
	VOTE_SORT svote[200];
	VOTE_DATA *vote;

        for(vote=ballot->first_vote; vote!=NULL; vote=vote->next)
        {
	 fempty=0;
	 if(svote[0].votes<1)
	 {
	  sprintf(svote[0].who,"%s",vote->vote_for);
	  svote[0].votes = 1;
	 }
         else
         {
          for(x=0; x<200; x++)
          {
	   if(!fempty)
	   {
            if(strcmp(svote[x].who, vote->vote_for)==0)
            {
             svote[x].votes++;
            }
	    else
	    {
             sprintf(svote[x].who,"%s",vote->vote_for);
             svote[x].votes = 1;
	     fempty=1;
	    }
           }
	  }
         }
	}	

        for(x=0; x<200; x++)
        {
	 if(svote[x].votes)
	 {
	  sprintf(buf,"%s has %d votes.\n\r",svote[x].who,svote[x].votes);
	  send_to_char(buf, ch);
	 }
        }*/
}

bool sort_votes(char *hold[], int *holdi, int cabal)
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char buf[MSL];
	int j,m,i;
	bool added=FALSE;
	char *temp;
	int tempi;


	sprintf(buf,"SELECT vote_for FROM votes WHERE cabal=%d", cabal);                                          
	res = one_query_res(buf);                                                                                 
	i = mysql_num_rows(res);                                                                                  
	if(i > 0)                                                                                                 
	{
		while((row = mysql_fetch_row(res)) != NULL)                                                           
		{
			added=FALSE;                                                                                      
			for(j = 0; j <= 200; j++)                                                                         
			{                                                                                                 
				if(hold[j] == NULL)                                                                           
					break;                                                                                    
                                                                     
				if(!str_cmp(hold[j], row[0]))                                                                 
				{                                                                                             
					added=TRUE;                                                                               
					holdi[j]++;                                                                               
					break;                                                                                    
				}                                                                                             
			}                                                                                                 

			if(!added)                                                                                        
			{                                                                                                 
				hold[j] = row[0];                                                                             
				holdi[j] = 1;                                                                                  
			}                                                                                                 
		}                                                                                                     

		for(j = 0; j <= 200; j++)
		{                                                                                                     
			for(m = 0; m <= 200 - j; m++)                                                                     
			{                                                                                                 
				if(!holdi[m] || hold[m]==NULL)                                                                
					continue;                                                                                 
                                                                                               
				if(holdi[m]<holdi[m+1])                                                                       
				{                                                                                             
					tempi=holdi[m];                                                                           
					holdi[m]=holdi[m+1];                                                                      
					holdi[m+1]=tempi;                                                                         
					temp = hold[m];                                                                           
					hold[m]=hold[m+1];                                                                        
					hold[m+1]=temp;                                                                           
				}                                                                                             
			}                                                                                                 
		}                                                                                                     
	}
	else
		return FALSE;

	return TRUE;
	mysql_free_result(res);
}

void do_listvotes(CHAR_DATA *ch, char *argument)
{
	char buf[MSL],arg1[MSL],time[MSL],arg2[MSL];
	MYSQL_RES *res;
	MYSQL_ROW row;
	char *hold[200];
	int holdi[200];
	int count=0,cabal,i,j;
	time_t test;

	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);

	if(str_cmp(arg2,"sort"))
	{
		if(arg1[0] == '\0' || (cabal=cabal_lookup(arg1)) == CABAL_NONE)
			return send_to_char("Syntax:  listvotes <cabal> [<sort>]\n\r", ch);

		sprintf(buf,"SELECT * FROM votes WHERE cabal=%d", cabal);
		res = one_query_res(buf);
		i = mysql_num_rows(res);
		if(i > 0)
		{
			count=0;
			while((row = mysql_fetch_row(res)) != NULL)
			{
				count++;
				test = (time_t) atol(row[3]);
				strftime(time, 200, "%m-%d-%Y %H:%M:%S", localtime(&test));
				sprintf(buf,
					"\t%d: %s votes for %s (at %s from %s)\n\r",
					count,row[0],row[1],time,row[4]);
				send_to_char(buf,ch);
			}
		}
		else
		{
			send_to_char("No valid votes to list.\n\r", ch);
			mysql_free_result(res);
		}
	}
	else
	{
		for(j=0; j<=200; j++)
		{
			hold[j] = NULL;
			holdi[j] = 0;
		}
		
		if(arg1[0] == '\0' || (cabal=cabal_lookup(arg1)) == CABAL_NONE)
			return send_to_char("Syntax:  listvotes <cabal> [<sort>]\n\r", ch);
		
		if(!sort_votes(hold,holdi,cabal))
		{
			return send_to_char("No valid votes to list.\n\r", ch);
		}
			
		if(cabal == CABAL_GUILD)
			send_to_char("Common Guild voting polls:\n\r", ch);
			
		for(j = 0; j <= 200; j++)
		{
			if(hold[j] == NULL)
				continue;
			count++;
			sprintf(buf,"\t%d: %s has %d votes.\n\r", count, hold[j], holdi[j]);
			send_to_char(buf, ch);
		}
	}

	return;
}

#if defined(KEY)
#undef KEY
#endif

#define KEY( literal, field, value )                                    \
                                if ( !str_cmp( word, literal ) )        \
                                {                                       \
                                    field  = value;                     \
                                    fMatch = TRUE;                      \
                                    break;                              \
                                }

/* provided to free strings */
#if defined(KEYS)
#undef KEYS
#endif

void do_vote(CHAR_DATA *ch, char *argument)
{
	char arg1[MSL], dir[MSL], buf[MSL], *word;
	char *escape;
	int cabal=0,cvote=0;
	FILE *fp;
	CHAR_DATA *victim;
	MYSQL_RES *res;
	MYSQL_ROW row;
	bool end = FALSE, fMatch = FALSE;

	if(IS_NPC(ch))
		return;
	
	if(!str_cmp(argument,""))
		return send_to_char("vote <person>:  Place your vote for the selected player, who does not have to be playing.\n\r",ch);
	
	argument = one_argument(argument,arg1);
	
	arg1[0] = UPPER(arg1[0]);

	sprintf(dir,"%s%s%s",PLAYER_DIR,arg1,".plr");

	if ( ( victim = get_char_world( ch, arg1 ) ) != NULL )
	{
		cabal = victim->cabal;
	}
	else
	{
	if ( ((fp = fopen(dir, "r" )) == NULL))
		return send_to_char("There is no such person.\n\r",ch);
	else
	{
		for ( ; ; )
		{
			word   = feof( fp ) ? (char*)"End" : fread_word( fp );
			fMatch = FALSE;

			switch ( UPPER(word[0]) )
			{
				case '*':
					fMatch = TRUE;
					fread_to_eol( fp );
				break;
				case 'C':
					KEY( "Cabal",           cabal,          cabal_lookup(fread_string(fp)));
				break;
				case 'E':
					if ( !str_cmp( word, "End" ) )
						end = TRUE;
				break;
			}

			if ( !fMatch )
				fread_to_eol( fp );

			if (end)
				break;
		}
	}
	fclose (fp);
	}

	if (!ch->cabal || ch->cabal != CABAL_GUILD)
	 return send_to_char("Huh?\n\r",ch);
	else if (cabal != ch->cabal)
	 return send_to_char("You can only vote for members of The Common Guild.\n\r", ch);

	sprintf(buf,"SELECT COUNT(voter) FROM votes WHERE voter='%s'", ch->true_name);
	res = one_query_res(buf);
	row = mysql_fetch_row(res);
	cvote = atoi(row[0]);
	escape = (char *) ((ch->pcdata->host) ? escape_string(ch->pcdata->host) : escape_string(ch->desc->host));
	if(cvote > 0)
	{
		mysql_free_result(res);
		sprintf(buf,"SELECT vote_for FROM votes WHERE voter='%s'", ch->true_name);
		res = one_query_res(buf);
		if(res)
		{
			row = mysql_fetch_row(res);
			sprintf(buf,"You have already voted for %s during this election.\n\r", row[0]);
			send_to_char(buf , ch);
		}
		else
			return mysql_free_result(res);
	}
	else
	{
		sprintf(buf,
				"INSERT INTO votes VALUES('%s','%s', %d, %ld, '%s')", 
				ch->true_name, arg1, ch->cabal, current_time, escape);	
		one_query(buf);
		sprintf(buf,"You have placed your vote for %s.\n\r", (!str_cmp(arg1,ch->true_name)) ? "yourself" : arg1);
		send_to_char(buf, ch);
	}
	mysql_free_result(res);
}

void mob_recho( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    if(argument[0]=='\0')
    {
        return;
    }

    for ( d = descriptor_list; d; d = d->next )
    {
        if ( d->connected == CON_PLAYING
        &&   d->character->in_room == ch->in_room
	&&   IS_AWAKE(d->character)
	&&   d->character != ch)
        {
            if (get_trust(d->character) >= 55)
                send_to_char( "local mob> ",d->character);
            send_to_char( argument, d->character );
            send_to_char( "\n\r",   d->character );
        }
    }
    return;
}

void area_echo(CHAR_DATA *ch, char *echo)
{
	DESCRIPTOR_DATA *d;
	char buffer[MAX_STRING_LENGTH*2];
    	for (d = descriptor_list; d; d = d->next)
    	{
        if (d->connected == CON_PLAYING
        &&  d->character->in_room != NULL && ch->in_room != NULL
        &&  d->character->in_room->area == ch->in_room->area)
        {
            colorconv(buffer, echo, d->character);
            send_to_char(buffer,d->character);
            send_to_char("\n\r",d->character);
        }
    }
}

void rarea_echo(ROOM_INDEX_DATA *room, char *echo)
{
        DESCRIPTOR_DATA *d;
        char buffer[MAX_STRING_LENGTH*2];
        for (d = descriptor_list; d; d = d->next)
        {
        if (d->connected == CON_PLAYING
        &&  d->character->in_room != NULL && room != NULL
        &&  d->character->in_room->area == room->area)
        {
            colorconv(buffer, echo, d->character);
            send_to_char(buffer,d->character);
            send_to_char("\n\r",d->character);
        }
    }
}

void outdoors_echo(AREA_DATA *area, char *echo)
{
	DESCRIPTOR_DATA *d;
	char buffer[MSL*2];
	for (d = descriptor_list; d; d = d->next) {
		if (d->connected == CON_PLAYING &&
			d->character->in_room != NULL && area != NULL &&
			d->character->in_room->area == area &&
			d->character->in_room->sector_type != SECT_INSIDE &&
			d->character->in_room->sector_type != SECT_UNDERWATER &&
			IS_AWAKE(d->character) &&
			!IS_SET(d->character->in_room->room_flags,ROOM_INDOORS)) {
			colorconv(buffer,echo,d->character);
			send_to_char(buffer,d->character);
			send_to_char("\n\r",d->character);
		}
	}
}

bool check_volley(CHAR_DATA *ch, CHAR_DATA *victim)
{
	int skill, chance;
	//CH is caster of offensive spell, victim is victim
	return FALSE;
	if(!ch || !victim)
		return FALSE;
	if(!(skill=get_skill(victim,gsn_volley)) || victim->fighting || !IS_AWAKE(victim) || victim==ch)
		return FALSE;
	if(get_trust(victim) == MAX_LEVEL)
		return TRUE;
	chance = (int)((float)skill * .6);
	chance -= get_curr_stat(ch,STAT_INT);
	chance += get_curr_stat(victim,STAT_INT);
	if(number_percent()<chance)
		return TRUE;
	return FALSE;
}

char * get_char_color(CHAR_DATA *ch, char *event)
{
	int i = get_event_number(event);

	if (IS_SET(ch->comm, COMM_LOTS_O_COLOR))
		return color_table[number_range(0,MAX_COLORS-1)].color_ascii;

	if ( ch->desc != NULL && ch->desc->original != NULL )
		ch = ch->desc->original;

	if (IS_NPC(ch))
		return "";

    if(!IS_SET(ch->comm,COMM_ANSI))
		return "";

	if(i>=0)
		return color_name_to_ascii(ch->pcdata->color_scheme[i]);

	for(i=0; i < MAX_COLORS; i++)
		if(!str_cmp(event,color_table[i].color_name))
			return color_table[i].color_ascii;

	return "";
}
int get_event_number(char *name)
{
	int i;
	for(i=0; i<MAX_EVENTS; i++)
		if(!str_cmp(name,color_event[i].event_name))
			return i;
	return -1;
}
char * get_color_name(char *name)
{
	int i;

	for(i=0; i<MAX_COLORS; i++)
		if(!str_prefix(name,color_table[i].color_name)) {
			return color_table[i].color_name;
		}

	return NULL;	
}

char * color_name_to_ascii(char *name)
{
	int i;
	if(!strcmp(name,""))
		return NULL;
	for(i=0; i<MAX_COLORS; i++)
		if(!str_cmp(name,color_table[i].color_name)) {
			return color_table[i].color_ascii;
		}

	return NULL;
}

char * END_COLOR(CHAR_DATA *ch)
{
	if (IS_SET(ch->comm, COMM_LOTS_O_COLOR))
		return color_table[number_range(0,MAX_COLORS-1)].color_ascii;

	if (ch->desc != NULL && ch->desc->original != NULL)
	{
		if (IS_SET(ch->desc->original->comm,COMM_ANSI))
			return "\x01B[0m";
		else
			return "";
	}
	else
	{
		if (IS_SET(ch->comm,COMM_ANSI))
			return "\x01B[0m";
		else
			return "";
	}
	return "";
}
void ADD_WAIT_STATE(CHAR_DATA *ch, int npulse)
{
	if (!ch)
		return;

	WAIT_STATE(ch,ch->wait + npulse);
	return;
}

void WAIT_STATE(CHAR_DATA *ch, int npulse)
{
	ROOM_AFFECT_DATA *raf;
	int wait = npulse;

	if (!ch)
		return;
	if (!IS_NPC(ch) && ch->pcdata->energy_state == -4) {
		wait *= 2;
	}

	if(ch->in_room && is_affected_room(ch->in_room, gsn_gravity_well))
	{
		for(raf = ch->in_room->affected; raf!=NULL; raf=raf->next)
			if(raf->type == gsn_gravity_well)
				break;
		if(ch == raf->owner)
			wait += PULSE_VIOLENCE;
	}
	if(get_trust(ch)==MAX_LEVEL)
		wait = 0;
	ch->wait = UMAX(ch->wait,wait);
}

void LAG_CHAR(CHAR_DATA *ch, int npulse)
{
	int wait = npulse;
	
	if (!ch)
		return;

	if (get_trust(ch) == MAX_LEVEL)
		wait = 0;

	if(is_affected(ch, gsn_tower_of_fortitude))
		wait -= PULSE_VIOLENCE;

	if(wait < 0)
		wait = 0;
	
	ch->wait = UMAX(ch->wait,wait);

	return;
}

void bounty_log(char *string)
{
        FILE *fp;
        char *strtime;

        strtime = ctime(&current_time);
        strtime[strlen(strtime)-1] = '\0';


        if ((fp = fopen( BOUNTY_LOG_FILE, "a" )) != NULL)
        {
                fprintf( fp, "%s :: %s\n", strtime, string);
                  fclose(fp);
        }
        return;
}
void do_credits(CHAR_DATA *ch, char *argument)
{
	char buf[500], buf2[500], buf3[500];
	CHAR_DATA *victim;
	int i=0;
	if(IS_NPC(ch))
		return;
	if(!ch->pcdata->bounty_credits && ch->cabal!=CABAL_BOUNTY && !IS_IMMORTAL(ch))
		return send_to_char("You are not in that cabal.\n\r",ch);
	if(IS_IMMORTAL(ch) && str_cmp(argument,""))
	{
                argument=one_argument(argument,buf);
                argument=one_argument(argument,buf2);
                victim=get_char_world(ch,buf);
                if(!victim)
                   return send_to_char("They aren't here.\n\r",ch);
                if(IS_NPC(victim))
                   return send_to_char("Mobs have no bounty credits.\n\r",ch);
		if(buf && buf2[0]=='\0')
		{
			sprintf(buf3,"%s has %i bounty credits.\n\r",victim->name,victim->pcdata->bounty_credits);
			send_to_char(buf3,ch);
			return;	
		}
		if(is_number(buf2))
		{
			victim->pcdata->bounty_credits=atoi(buf2);
			sprintf(buf3,"%s now has %i bounty credits.\n\r",victim->name,victim->pcdata->bounty_credits);
			send_to_char(buf3,ch);
			return;
		}
	}
	for(i=0;i<MAX_BOUNTY;i++)
	{
		sprintf(buf,"%-15s - %i\n\r",capitalize(bounty_table[i].rank_name),bounty_table[i].credits_required);
		send_to_char(buf,ch);
	}
	sprintf(buf2,"You have %i bounty credits.\n\r",ch->pcdata->bounty_credits);
	send_to_char(buf2,ch);
}
void bounty_credit(CHAR_DATA *ch, int credit)
{
	char buf[300], buf2[300], blog[500];
	int i, old_credit=ch->pcdata->bounty_credits;

	if(IS_NPC(ch))
		return;

	sprintf(buf,"You receive %i bounty credits for your kill!\n\r",credit);
	send_to_char(buf,ch);
	ch->pcdata->bounty_credits+=credit;

	for(i=1;i<MAX_BOUNTY;i++)
	{
		if(ch->pcdata->bounty_credits>=bounty_table[i].credits_required 
		&& old_credit<bounty_table[i].credits_required)
		{
			sprintf(buf,"You gain a title within the Bounty Guild, advancing to %s!\n\r",bounty_table[i].rank_extitle);
			send_to_char(buf,ch);
			sprintf(buf2,", %s",bounty_table[i].rank_extitle);
			set_extitle(ch,buf2);
			sprintf(buf2,"%s has advanced to the position of %s.",ch->name,bounty_table[i].rank_extitle);
			bounty_cb(buf2);
			sprintf(blog,"%s advances to %s.",ch->name,bounty_table[i].rank_extitle);
			bounty_log(blog);
			ch->pcdata->cabal_level++;
			update_cskills(ch);
		}
	}
}

void bounty_cb(char *string)
{
CHAR_DATA *guardian;
guardian=get_cabal_guardian(CABAL_BOUNTY);
if(guardian)
	do_cb(guardian,string);
}
void do_topbounties(CHAR_DATA *ch, char *argument)
{
	int i, pnum = 0, plus = 0;
	char buf[MAX_STRING_LENGTH];
	return;	
	send_to_char("      The Top Ten Most Wanted:\n\r",ch);
	for(i=0;i<=MAX_TOP_BOUNTY_SHOWN;i++)
	{
		if(top_bounty_value[i])
		{
			if (i > 0 && top_bounty_value[i] == top_bounty_value[i-1]) {
				pnum--;
				plus++;
			} else {
				pnum += plus;
				plus = 0;
			}
			sprintf(buf,"%2i) %-15s %11d gold\n\r",pnum+1,top_bounty_name[i],(int)top_bounty_value[i]);
			send_to_char(buf,ch);
		}
		pnum++;
	}
}
void do_bounty(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim;
	char arg1[MAX_STRING_LENGTH], arg2[MAX_STRING_LENGTH], tempstr[50], buf[200], buf2[200];
	int amount;
   	float famount;
	argument = one_argument(argument,arg1);
	argument = one_argument(argument,arg2);

	if(check_horde(ch))
		return;
	
	if(!arg1 || !arg2 || (!is_number(arg2) && !IS_IMMORTAL(ch)))
	{
		send_to_char("Syntax:   bounty <character> <amount>\n\r",ch);
		send_to_char("Places a bounty of the specified amount on the life of the specified character.\n\r",ch);
		if(IS_IMMORTAL(ch))
		{
			send_to_char("          bounty <character> <clear>\n\r",ch);
			send_to_char("Clears the bounty on the specified character.\n\r",ch);
		}
		return;
	}

	if(!(victim=get_char_world(ch,arg1)))
		return send_to_char("They aren't here.\n\r",ch);

	if(IS_NPC(victim) || ch->cabal==CABAL_BOUNTY || ((IS_IMMORTAL(victim) || IS_HEROIMM(victim)) && !IS_IMMORTAL(ch)))
		return send_to_char("You can't do that.\n\r",ch);

	if(!is_number(arg2) && IS_IMMORTAL(ch) && !str_cmp(arg2,"clear"))
    	{
		sprintf(tempstr,"%ld",victim->pcdata->bounty);
		act("The bounty on $N's head has been cleared. (formerly $t gold)",ch,tempstr,victim,TO_CHAR);
		victim->pcdata->bounty=0;
		return;
	}
	
	if(victim->bounty_timer > 0)
		return send_to_char("Your bounty offer has been rejected by Dargor.\n\r", ch);

	if(!is_number(arg2))
		return send_to_char("Invalid amount.\n\r",ch);;
    amount = atoi(arg2);

	if(amount<1000)
		return send_to_char("You must place a bounty of at least 1000 gold coins.\n\r",ch);

	if(amount > ch->gold)
		return send_to_char("You don't have enough money.\n\r",ch);

	if(victim == ch && !IS_IMMORTAL(ch))
		return send_to_char("Desperate for attention?\n\r",ch);

	famount = amount / (UMAX(calculate_inflation() - 1,1));
	victim->pcdata->bounty += (int)famount;
	ch->gold -= amount;
	act("You give the Guild a bounty of $t gold to place on $N's head.",ch,arg2,victim,TO_CHAR);
	sprintf(buf,"Someone has placed a bounty of %.0f gold on the life of %s.",famount,victim->name);
	bounty_cb(buf);
    sprintf(buf2,"%s places a bounty of %.0f gold on the life of %s.",ch->true_name,famount,victim->name);	
	bounty_log(buf2);
	wiznet(buf2,NULL,NULL,WIZ_LOG,0,0);
}
void pay_bounty(CHAR_DATA *ch, CHAR_DATA *victim)
{
	/* CH is killer, VICTIM is deadguy */
	char sbounty[200];
	char buf[200];
	int credit, bonus;
	float mod, total_mod;


	if(!victim->pcdata->bounty || ch->cabal!=CABAL_BOUNTY)
		return;

	sprintf(sbounty,"You claim the bounty on %s's life, receiving %ld gold!\n\r",victim->name,victim->pcdata->bounty);
	send_to_char(sbounty,ch);
	ch->gold+=victim->pcdata->bounty;

	credit= (int)((float)victim->pcdata->bounty/(float)1000);

	mod = 2.0 * (float)((float)victim->level / (float)100);
	if(!mod)
		bug("Error: Mod is 0.",0);
        if(victim->cabal>0)
                credit=(int)((float)credit * 1.3);
        
        if(victim->pcdata->induct)
                credit=(int)((float)credit * 1.6);

	bonus = (int)pow(2, victim->pcdata->bounty_killed);

	victim->pcdata->bounty_killed = 0;

	total_mod = (float)credit * (float)mod + bonus;

	credit = (int)total_mod;

	if(credit > 1000)
		credit = 1000;

	if(credit < 0)
		credit = 0;

	sprintf(buf,"Base - %.2f, Mod - %.2f, Bonus - %.2f", (float)credit, (float)mod, (float)bonus);
	wiznet(buf, NULL, NULL, WIZ_LOG, 0, 0);
	sprintf(sbounty,"%s collects the %ld gold bounty on %s.",ch->name,victim->pcdata->bounty,victim->name);
	wiznet(sbounty,NULL,NULL,WIZ_CABAL,0,0);
	sprintf(buf,"%s has collected the %ld gold bounty on the life of %s.",ch->name,victim->pcdata->bounty,victim->true_name);
	bounty_cb(buf);
    	bounty_credit(ch,credit);
	bounty_log(sbounty);
    	victim->pcdata->bounty=0;

	return;
}
void do_rchanges(CHAR_DATA *ch, char *argument)
{
	/*
	char arg1[MAX_STRING_LENGTH], buf[MAX_STRING_LENGTH], list[2000][110], nlist[110];
	char writestring[MAX_STRING_LENGTH];
	FILE *fpChar;
    	long size = 0;
    	BUFFER *output;
    	int numMatches=0, numEntries = 25;


    	output = new_buf();

	one_argument(argument,arg1);
	if(is_number(arg1))
		numEntries=atoi(arg1);

	if(!str_cmp(arg1,"options"))
	{
	send_to_char("Use rchanges <number> to see the most recent <number> of entries.\n\r",ch);
	send_to_char("To add an entry, use rchanges add <information>. It will appear like:\n\r",ch);
	send_to_char("    rchanges add added the new command rchange.\n\r",ch);
	send_to_char("[Sun Aug 27 4:02pm] Dev added the new command rchange.\n\r",ch);
	return;
	}
	if(!str_cmp(arg1,"add"))
	{
	//add a new entry, first getting rid of the word 'add'
		argument=one_argument(argument,arg1);
		fpChar=fopen(HISTORY_FILE,"a");
		if(!fpChar)
			return;
		fprintf(fp,"[%s] %s %s",ttime,ch->name,argument);
		fclose(fpChar);
	}
	//load recent history and show it
	send_to_char("See rchanges options for options.\n\r");
        fpChar=fopen(HISTORY_FILE,"r");
        if(!fpChar)
                return;
        results[0]='\0';
        while(fgets(results,250,fpChar))
        {
		buf[0]='\0';
		temp[0]='\0';
		numMatches++;
		sprintf(list[numMatches],"%s",results);
                if(numMatches>1999)
                        break;
		//If there's over 2000 entries, ick.
	}	
	for(loop=numMatches;loop>=1;loop--)
	{
		sprintf(nlist,"%3d) %s\r",numMatches+1-loop,list[loop]);
                size+=sizeof(nlist);
                if(size>=MAX_BUF || (numMatches-loop)==numEntries)
                        break;
		add_buf(output,nlist);
	}
	page_to_char(buf_string(output),ch);
	free_buf(output);
	fclose(fpChar);
*/		
}
void do_affto(CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;
	CHAR_DATA *victim;
	char cname[MAX_INPUT_LENGTH], skill_name[MAX_INPUT_LENGTH], location[MAX_INPUT_LENGTH], targ[MAX_INPUT_LENGTH], saftype[MAX_INPUT_LENGTH], sbitvector[MAX_INPUT_LENGTH];
	int duration = -2, sn=2, nlocation=-2, modifier=0, aftype=-2;
	long bitvector = 0;
	char returnstr[MAX_INPUT_LENGTH];
	sprintf(returnstr,"Syntax:  affto <character> <skill name> <location> <modifier> <duration> <aftype> <bitvector>\n\rLocations:\n\r");
	strcat(returnstr,"\n\rAffect types:\n\r");
	strcat(returnstr,flags_to_string(ch, aftype_table, 15));
	strcat(returnstr,"\n\rBitvectors:\n\r");
	strcat(returnstr,flags_to_string(ch, affect_flags, 9));
	strcat(returnstr,"\n\r");
	if(!str_cmp(argument,""))
	{
		send_to_char(returnstr,ch);
		return;
	}
	argument=one_argument(argument,cname);
	argument=one_argument(argument,skill_name);
	argument=one_argument(argument,location);
	argument=one_argument(argument,targ);
	if(targ[0]!='\0')
		modifier=atoi(targ);
	targ[0]='\0';
	argument=one_argument(argument,targ);
	if(targ[0]!='\0')
		duration=atoi(targ);
	targ[0]='\0';
	argument=one_argument(argument,saftype);
	argument=one_argument(argument,sbitvector);
	if(targ[0]!='\0')
		bitvector=atoi(targ);
	if(cname[0]=='\0' || skill_name[0]=='\0' || saftype[0]=='\0' || sbitvector[0]=='\0' || duration<-1)
	{
		send_to_char(returnstr,ch);
		return;
	}
	if(!(victim=get_char_world(ch,cname)))
	{
		send_to_char("They aren't here.\n\r",ch);
	}
	if((sn=skill_lookup(skill_name))==-1)
	{
		send_to_char("There is no skill by that name.\n\r",ch);
		return;
	}
	if((nlocation=display_lookup(location,apply_locations))==0 && str_cmp(location,"none"))
	{
		send_to_char("There is no affect location by that name.\n\r",ch);
		return;
	}
	if((aftype=flag_lookup(saftype,aftype_table))==NO_FLAG && str_cmp(saftype,"AFT_SPELL"))
	{
		send_to_char("There is no aftype by that name.\n\r",ch);
		return;
	}
	bitvector=flag_lookup(sbitvector,affect_flags);
	if (bitvector == NO_FLAG)
		return send_to_char("Invalid bitvector.\n\r",ch);
	init_affect(&af);
	af.where	= TO_AFFECTS;
	af.type		= sn;
	af.aftype	= aftype;
	af.level	= ch->level;
	af.location	= nlocation;
	af.modifier	= modifier;
	af.duration	= duration;
	SET_BIT(af.bitvector, bitvector);
	affect_to_char(victim,&af);	
	sprintf(returnstr,"%s has been given the %s affect.\n\r",IS_NPC(victim) ? victim->short_descr : victim->name,skill_table[sn].name);
	send_to_char(returnstr,ch);
}
char * flags_to_string(CHAR_DATA *ch, const struct flag_type *showflags, int flagsperline)
{
	int flag, col=0;
	char temp_value[MAX_INPUT_LENGTH];
	temp_value[0]='\0';
	for(flag=0; showflags[flag].name!=NULL; flag=flag++)
	{
		strcat(temp_value,showflags[flag].name);
		if(++col % flagsperline==0)
			strcat(temp_value,"\n\r");
		else
			strcat(temp_value," ");
	}
	if(temp_value[0]=='\0')
		sprintf(temp_value,"Error looking up flag names.\n\r");
	return palloc_string(temp_value);
}
void do_ltrack(CHAR_DATA *ch, char *argument)
{
	MYSQL *conn;
	MYSQL_ROW       row;
	MYSQL_RES       *res_set;
	BUFFER *buffer;
	char arg1[MSL], arg2[MSL], query[MSL], buf[MSL];
	int type = -1, show = -1, i = 0;

	argument = one_argument(argument,arg1);
	argument = one_argument(argument,arg2);

	if (arg1[0] == '\0')
		return send_to_char("Syntax: ltrack <name, date, site> <all/new/in/out?> <#display?>\n\r",ch);

	if (arg2[0] != '\0')
	{
		if (!str_cmp(arg2,"new"))
		{
			type	= 0;
		}
		else if (!str_cmp(arg2,"in"))
		{
			type	= 1;
		}
		else if (!str_cmp(arg2,"out"))
		{
			type	= 2;
		}

		if (argument[0] != '\0' && is_number(argument))
		{
			show = atoi(argument);
		}
	}

	buffer = new_buf();
	sprintf(buf,"%d",type);
	sprintf(query, "SELECT * FROM logins WHERE (name RLIKE '%s' OR site RLIKE '%s' OR time RLIKE '%s') %s%s ORDER BY ctime DESC",
		arg1, arg1, arg1, type > -1 ? "AND type=" : "", type > -1 ? buf : "");

	conn = open_conn();
	mysql_query(conn,query);
	res_set = mysql_store_result (conn);
	if (res_set == NULL && mysql_field_count(conn)>0)
	{
		send_to_char("Error accessing results.\n\r",ch);
	}
	else if (res_set)
	{
		while((row = mysql_fetch_row (res_set)) != NULL)
		{
			i++;
			if ((show != -1 && i > show) || i>300)
				break;
			sprintf(buf,"%s",row[7]);
			type = atoi(buf);
			sprintf(buf, "%s: %s@%s logged %s. [%s%s%s (%s) obj]\n\r",
				row[2],
				row[0],
				row[1],
				type == 0 ? "new" : type == 1 ? "in" : type == 2 ? "out" : "?",
				type == 2 ? row[4] : "",
				type == 2 ? " played, " : "",
				row[5],
				row[6]);
                        add_buf(buffer, buf);
		}
		mysql_free_result (res_set);
		page_to_char(buf_string(buffer),ch);
		free_buf(buffer);
		return do_disc(conn);
	}
	send_to_char("No matching results were found.\n\r",ch);
	return do_disc(conn);
}

void do_assess_old(CHAR_DATA *ch, char *argument)
{
	int skill, fuzzy, showdur;
        char arg[MAX_INPUT_LENGTH];
        CHAR_DATA *victim;
        char buf[MAX_STRING_LENGTH];
	AFFECT_DATA *paf;

        one_argument(argument,arg);

	if(argument[0]=='\0')
	{
		send_to_char("Syntax: assess <target>\n\r",ch);
		return;
	}
	if((skill=get_skill(ch,skill_lookup("assess")))<5)
	{
		send_to_char("You don't know how to assess people's conditions.\n\r",ch);
		return;
	}
        if ((victim = get_char_room(ch,arg)) == NULL)
        {
                send_to_char("They aren't here.\n\r",ch);
                return;
        }
	if(victim==ch)
	{
		send_to_char("Why not just use affects?\n\r",ch);
		return;
	}
	act("You study $N intently, searching for signs of affliction.",ch,0,victim,TO_CHAR);
	if(number_percent()>skill)
	{
		send_to_char("You lose your concentration before finding any telltale signs of affliction.\n\r",ch);
	        check_improve(ch,skill_lookup("assess"),FALSE,1);
		WAIT_STATE(ch,PULSE_VIOLENCE);
		ch->mana-=20;
		return;
	}
	if(victim->affected==NULL)
	{
		send_to_char("You are unable to find any signs of affliction.\n\r",ch);
	        check_improve(ch,skill_lookup("assess"),TRUE,1);
		WAIT_STATE(ch,PULSE_VIOLENCE);
		ch->mana-=40;
		return;
	}
	for ( paf = victim->affected; paf != NULL; paf = paf->next )
	{
		buf[0]='\0';
		if(skill<91);
		sprintf(buf,"%s seems to be affected by %s.\n\r",IS_NPC(victim) ?
victim->short_descr : victim->name,skill_table[paf->type].name);
		if(skill>=91);
		{
		fuzzy=number_range(0,2);
		//Let's fuz up the duration a bit if it's not permanent.
		if(paf->duration>-1)
		{
		if(number_range(0,1)==0)
			showdur=paf->duration+fuzzy;
		else
			showdur=paf->duration-fuzzy;
		sprintf(buf,"%s seems to be affected by %s for about %d hours.\n\r",IS_NPC(victim) ? victim->short_descr :
victim->name,skill_table[paf->type].name,showdur);
		}
		if(paf->duration==-1)
			sprintf(buf,"%s seems to be affected by %s permanently.\n\r",IS_NPC(victim) ? victim->short_descr :
victim->name,skill_table[paf->type].name);
		}
	send_to_char(buf,ch);
	}
	check_improve(ch,skill_lookup("assess"),TRUE,1);
        WAIT_STATE(ch,PULSE_VIOLENCE);
        ch->mana-=40;

}
void do_supps( CHAR_DATA *ch, char *argument )
{
    BUFFER *buffer;
    char arg[MAX_INPUT_LENGTH];
    char spell_list[LEVEL_HERO + 1][MAX_STRING_LENGTH];
    char spell_columns[LEVEL_HERO + 1];
    int sn, level, min_lev = 1, max_lev = LEVEL_HERO, mana;
    bool fAll = FALSE, found = FALSE;
    char buf[MAX_STRING_LENGTH];

    if (IS_NPC(ch))
      return;
    if(ch->Class()->ctype!=CLASS_COMMUNER && !IS_IMMORTAL(ch))
    {
		return send_to_char("Your class knows no prayers.\n\r",ch);
    }
    if (argument[0] != '\0')
    {
	fAll = TRUE;

	if (str_prefix(argument,"all"))
	{
	    argument = one_argument(argument,arg);
	    if (!is_number(arg))
	    {
		send_to_char("Arguments must be numerical or all.\n\r",ch);
		return;
	    }
	    max_lev = atoi(arg);

	    if (max_lev < 1 || max_lev > LEVEL_HERO)
	    {
		sprintf(buf,"Levels must be between 1 and %d.\n\r",LEVEL_HERO);
		send_to_char(buf,ch);
		return;
	    }

	    if (argument[0] != '\0')
	    {
		argument = one_argument(argument,arg);
		if (!is_number(arg))
		{
		    send_to_char("Arguments must be numerical or all.\n\r",ch);
		    return;
		}
		min_lev = max_lev;
		max_lev = atoi(arg);

		if (max_lev < 1 || max_lev > LEVEL_HERO)
		{
		    sprintf(buf,
			"Levels must be between 1 and %d.\n\r",LEVEL_HERO);
		    send_to_char(buf,ch);
		    return;
		}

		if (min_lev > max_lev)
		{
		    send_to_char("That would be silly.\n\r",ch);
		    return;
		}
	    }
	}
    }


    /* initialize data */
    for (level = 0; level < LEVEL_HERO + 1; level++)
    {
        spell_columns[level] = 0;
        spell_list[level][0] = '\0';
    }

    for (sn = 0; sn < MAX_SKILL; sn++)
    {
        if (skill_table[sn].name == NULL )
	    break;

	if ((level = skill_table[sn].skill_level[ch->Class()->GetIndex()]) < LEVEL_HERO + 1
	&&  level >= min_lev && level <= max_lev
	&&  skill_table[sn].spell_fun != spell_null
	&&  ch->pcdata->learned[sn] > 0
	&&  (skill_table[sn].ctype==CMD_COMMUNE || skill_table[sn].ctype==CMD_BOTH))
        {
	    found = TRUE;
	    level = skill_table[sn].skill_level[ch->Class()->GetIndex()];
	    if (ch->level < level)
	    	sprintf(buf,"%-18s n/a      ", skill_table[sn].name);
	    else
	    {
        mana = UMAX(skill_table[sn].min_mana,
		    100/(2 + ch->level - level));
            sprintf(buf,"%-18s  %3d mana  ",skill_table[sn].name,mana);
	    }

	    if (spell_list[level][0] == '\0')
          	sprintf(spell_list[level],"\n\rLevel %2d: %s",level,buf);
	    else /* append */
	    {
          	if ( ++spell_columns[level] % 2 == 0)
		    strcat(spell_list[level],"\n\r          ");
          	strcat(spell_list[level],buf);
	    }
	}
    }

    /* return results */

    if (!found)
    {
      	send_to_char("No spells found.\n\r",ch);
      	return;
    }

    buffer = new_buf();
    for (level = 0; level < LEVEL_HERO + 1; level++)
      	if (spell_list[level][0] != '\0')
	    add_buf(buffer,spell_list[level]);
    add_buf(buffer,"\n\r");
    page_to_char(buf_string(buffer),ch);
    free_buf(buffer);
}
void do_commune( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    EXIT_DATA *pexit;
    void *vo;
    int mana;
    int sn, where;
    AFFECT_DATA af,*paf;
    int target;
    if ( IS_NPC(ch) && ch->desc == NULL)
	return;
    target_name = one_argument( argument, arg1 );
    one_argument( target_name, arg2 );

    if(ch->Class()->ctype!=CLASS_COMMUNER && !IS_IMMORTAL(ch))
	return send_to_char("You aren't in touch well enough with the gods to commune prayers.\n\r",ch);

    if (IS_SET(ch->act,PLR_BETRAYER))
	return send_to_char("Your god has forsaken you!\n\r",ch);

    if(!IS_NPC(ch) && ch->pcdata->energy_state>3)
	return send_to_char("You cannot collect your thoughts enough to contact your deity.\n\r",ch);

    if(!IS_NPC(ch) && ch->pcdata->oalign==3 && ch->alignment==-1000)
	return send_to_char("Your god has forsaken you!\n\r",ch);

    if(is_affected(ch,gsn_worldbind))
	return send_to_char("Your prayers go unheeded.\n\r",ch);

    if ( arg1[0] == '\0' )
	return send_to_char( "Commune which what where?\n\r", ch );
 
    if ((sn = find_spell(ch,arg1)) < 1
    ||  skill_table[sn].spell_fun == spell_null
    || (!IS_NPC(ch) && get_skill(ch,sn) < 5)
    || (!IS_NPC(ch) && ch->pcdata->learned[sn] == 0))
	return send_to_char( "You don't know any prayers of that name.\n\r", ch );


	if(is_affected(ch, gsn_severed))
	{
		for(paf = ch->affected; paf != NULL; paf = paf->next)
			if(paf->type == gsn_severed)
				break;
		
		if(paf)
		{
			if(skill_table[sn].skill_level[ch->Class()->GetIndex()] > paf->modifier)
				return send_to_char("The ties to your god are severed and you can not commune that prayer.\n\r", ch);

		}
	}

    if(skill_table[sn].skill_level[ch->Class()->GetIndex()]>=25 && !IS_SET(ch->act,PLR_EMPOWERED) && !IS_IMMORTAL(ch))
	return send_to_char("The gods do not find you worthy of a prayer of that magnitude.\n\r",ch);

	if (is_affected(ch,gsn_scramble_neurons))
		sn = scramble_sn(ch,sn);

    if ( ch->position < skill_table[sn].minimum_position )
	return send_to_char( "You can't concentrate enough.\n\r", ch );

    if((skill_table[sn].ctype==CMD_SPELL || skill_table[sn].ctype==CMD_POWER || skill_table[sn].ctype == CMD_RUNE)
		&& !IS_IMMORTAL(ch))
	return send_to_char("You can't commune that.\n\r",ch);

    if (ch->level + 2 == skill_table[sn].skill_level[ch->Class()->GetIndex()])
	mana = 50;
    else
    {
	if(is_affected(ch,gsn_concuss))
	{
    		mana = UMAX(
	    		(skill_table[sn].min_mana*4),
	    		100 / ( 2 + ch->level - skill_table[sn].skill_level[ch->Class()->GetIndex()] ) );
	}
	else
	{
            mana = UMAX(
                 skill_table[sn].min_mana,
                 100 / ( 2 + ch->level - skill_table[sn].skill_level[ch->Class()->GetIndex()] ) );
	}	
    }
    /*
     * Locate targets.
     */
    victim	= NULL;
    obj		= NULL;
    vo		= NULL;
    target	= TARGET_NONE;
	af.aftype = AFT_COMMUNE;
    switch ( skill_table[sn].target )
    {
    default:
	bug( "Do_cast: bad target for sn %d.", sn );
	return;
    case TAR_IGNORE:
	break;
    case TAR_CHAR_OFFENSIVE:
	if ( arg2[0] == '\0' )
	{
	    if ( ( victim = ch->fighting ) == NULL )
	    {
		send_to_char( "Commune the prayer on whom?\n\r", ch );
		return;
	    }
	}
	else
	{
	    if ( ( victim = get_char_room( ch, target_name ) ) == NULL )
	    {
		send_to_char( "They aren't here.\n\r", ch );
		return;
	    }
	}
        if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
	{
	    send_to_char( "You can't do that on your own master.\n\r",
		ch );
	    return;
	}
	vo = (void *) victim;
	target = TARGET_CHAR;
	break;
    case TAR_CHAR_DEFENSIVE:
	if ( arg2[0] == '\0' )
	{
	    victim = ch;
	}
	else
	{
	    if ( ( victim = get_char_room( ch, target_name ) ) == NULL )
	    {
		send_to_char( "They aren't here.\n\r", ch );
		return;
	    }
              if(ch->Class()->GetIndex() == CLASS_ZEALOT && (skill_table[sn].target == TAR_CHAR_DEFENSIVE) && (victim != ch))
 	             return send_to_char("You cannot commune that prayer on another.\n\r",ch);
	}

	vo = (void *) victim;
	target = TARGET_CHAR;
	break;
    case TAR_CHAR_SELF:
	if ( arg2[0] != '\0' && !is_name( target_name, ch->name ) )
	{
	    send_to_char( "You cannot commune this prayer on another.\n\r", ch );
	    return;
	}

	vo = (void *) ch;
	target = TARGET_CHAR;
	break;
    case TAR_OBJ_INV:
	if ( arg2[0] == '\0' )
	{
	    send_to_char( "What should the prayer be communed upon?\n\r", ch );
	    return;
	}

	if ( ( obj = get_obj_carry( ch, target_name, ch ) ) == NULL )
	{
	    send_to_char( "You are not carrying that.\n\r", ch );
	    return;
	}

	vo = (void *) obj;
	target = TARGET_OBJ;
	break;
    case TAR_OBJ_CHAR_OFF:
	if (arg2[0] == '\0')
	{
	    if ((victim = ch->fighting) == NULL)
	    {
		send_to_char("Commune the prayer on whom or what?\n\r",ch);
		return;
	    }
	    target = TARGET_CHAR;
	}
	else if ((victim = get_char_room(ch,target_name)) != NULL)
	{
	    target = TARGET_CHAR;
	}
	if (target == TARGET_CHAR) /* check the sanity of the attack */
	{
            if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
			return send_to_char( "You can't do that on your own follower.\n\r", ch );

	    vo = (void *) victim;
 	}
	else if ((obj = get_obj_here(ch,target_name)) != NULL)
	{
	    vo = (void *) obj;
	    target = TARGET_OBJ;
	}
	else
		return send_to_char("You don't see that here.\n\r",ch);

	break;
    case TAR_OBJ_CHAR_DEF:
        if (arg2[0] == '\0')
        {
            vo = (void *) ch;
            target = TARGET_CHAR;
        }
        else if ((victim = get_char_room(ch,target_name)) != NULL)
        {
            vo = (void *) victim;
            target = TARGET_CHAR;
	}
	else if ((obj = get_obj_carry(ch,target_name,ch)) != NULL)
	{
	    vo = (void *) obj;
	    target = TARGET_OBJ;
	}
	else
		return send_to_char("You don't see that here.\n\r",ch);

	break;
    case TAR_DIR:
        if((where = direction_lookup(target_name))<0 || !(pexit=ch->in_room->exit[where]))
                return send_to_char("That's not a valid direction.\n\r",ch);
        vo = &where;
        target = TARGET_RUNE;
        break;
    }
    if ( !IS_NPC(ch) && ch->mana < mana )
	return send_to_char( "You don't have enough mana.\n\r", ch );

    WAIT_STATE( ch, skill_table[sn].beats );
    if ( !IS_NPC(ch) && (number_percent( ) > get_skill(ch,sn)))
    {
	send_to_char( "You failed to commune the power of your deity.\n\r", ch );
	check_improve(ch,sn,FALSE,1);
	ch->mana -= mana / 2;
    }
    else
    {
        ch->mana -= mana;
/*	if (IS_SET(ch->in_room->room_flags,ROOM_NO_MAGIC) && !(ch->level > LEVEL_HERO))
	{
	act("$n's spell fizzles.",ch,0,0,TO_ROOM);
	send_to_char("Your prayer fizzles and dies.\n\r",ch);
	return;
	}
*/	if (skill_table[sn].target == TAR_CHAR_OFFENSIVE
	&& is_safe(ch,victim))
		return;
	
	if (skill_table[sn].target == TAR_CHAR_OFFENSIVE)
	{
	if (!IS_NPC(ch) && !IS_NPC(victim)
	&& (ch->fighting == NULL || victim->fighting == NULL))
	{
		switch(number_range(0,2))
		{
		case (0):
		case (1):
		sprintf(buf,"Die, %s you filthy dog!",PERS(ch,victim));
		break;
		case (2):
		sprintf(buf,"Help! %s is communing a supplication on me!",PERS(ch,victim));
		break;
		}
	if (victim != ch && !IS_NPC(ch))
		do_myell(victim,buf, ch);
	}
	}
	if(skill_table[sn].target==TAR_CHAR_OFFENSIVE && victim!=ch)
	{
		act("You narrow your eyes and glare in $N's direction.",ch,0,victim,TO_CHAR);
		act("$n narrows $s eyes and glares in $N's direction.",ch,0,victim,TO_NOTVICT);
		act("$n narrows $s eyes and glares in your direction.",ch,0,victim,TO_VICT);
                if(check_volley(ch,victim))
                {
                        act("$N reflects your spell right back at you!",ch,0,victim,TO_CHAR);
                        act("You reflect $n's spell right back at $m!",ch,0,victim,TO_VICT);
                        act("$N reflects $n's spell right back at $m!",ch,0,victim,TO_NOTVICT);
                        (*skill_table[sn].spell_fun) ( sn, ch->level*2, victim, ch, target);
                        return;
                }
	}
	if(skill_table[sn].target==TAR_CHAR_SELF || skill_table[sn].target==TAR_IGNORE || skill_table[sn].target==TAR_OBJ_CHAR_DEF || skill_table[sn].target==TAR_OBJ_CHAR_OFF || (skill_table[sn].target==TAR_CHAR_DEFENSIVE && victim==ch))
	{
		act("You close your eyes and concentrate for a moment.",ch,0,0,TO_CHAR);
		act("$n closes $s eyes with a look of concentration for a moment.",ch,0,0,TO_ROOM);
	}
	if(skill_table[sn].target==TAR_CHAR_DEFENSIVE && victim!=ch)
	{
		act("You close your eyes for a moment and nod at $N.",ch,0,victim,TO_CHAR);
		act("$n closes $s eyes for a moment and nods at $N.",ch,0,victim,TO_NOTVICT);
		act("$n closes $s eyes for a moment and nods at you.",ch,0,victim,TO_VICT);
	}
	if(skill_table[sn].target==TAR_OBJ_INV)
	{
		act("You furrow your brow as you look through your possessions.",ch,0,0,TO_CHAR);
		act("$n furrows $s brow as $e looks through $s possessions.",ch,0,0,TO_ROOM);
	}
            (*skill_table[sn].spell_fun) ( sn, ch->level, ch, vo,target);
        if (sn == gsn_rage) {
			check_improve(ch,sn,TRUE,2);
		} else {
			check_improve(ch,sn,TRUE,1);
		}
    }
    if ((skill_table[sn].target == TAR_CHAR_OFFENSIVE
    ||   (skill_table[sn].target == TAR_OBJ_CHAR_OFF && target == TARGET_CHAR))
    &&   victim != ch
    &&   victim->master != ch)
    {
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;
	un_blade_barrier(ch,NULL);
	for ( vch = ch->in_room->people; vch; vch = vch_next )
	{
	    vch_next = vch->next_in_room;
	    if ( victim == vch && victim->fighting == NULL ) 
		{
			multi_hit( victim, ch, TYPE_UNDEFINED );
			break;
	    }
	}
    }
    return;
}

void do_call( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    EXIT_DATA *pexit;
    void *vo;
    int mana, where;
    int sn;

    int target;
    if ( IS_NPC(ch) && ch->desc == NULL)
	return;
    target_name = one_argument( argument, arg1 );
    one_argument( target_name, arg2 );
    if ( arg1[0] == '\0' )
	return send_to_char( "Call which what where?\n\r", ch );

    if ((sn = find_spell(ch,arg1)) < 1
    ||  skill_table[sn].spell_fun == spell_null
    || (!IS_NPC(ch) && get_skill(ch,sn) < 5)
    || (!IS_NPC(ch) && ch->pcdata->learned[sn] == 0))
	return send_to_char( "You don't know any powers of that name.\n\r", ch );

    if ( ch->position < skill_table[sn].minimum_position && get_trust(ch)<MAX_LEVEL-1)
	return send_to_char( "You can't concentrate enough.\n\r", ch );

    if(skill_table[sn].ctype!=CMD_POWER && !IS_IMMORTAL(ch))
	return send_to_char("You can't call that.\n\r",ch);

    if(cabal_down(ch,ch->cabal))
	return;

    if (ch->level + 2 == skill_table[sn].skill_level[ch->Class()->GetIndex()])
		mana = 50;
    else
    	mana = UMAX(
	    skill_table[sn].min_mana,
	    100 / ( 2 + ch->level - skill_table[sn].skill_level[ch->Class()->GetIndex()] ) );
    {

    }
    /*
     * Locate targets.
     */
    victim	= NULL;
    obj		= NULL;
    vo		= NULL;
    target	= TARGET_NONE;
    switch ( skill_table[sn].target )
    {
    default:
	bug( "Do_cast: bad target for sn %d.", sn );
	return;
    case TAR_IGNORE:
	break;
	case TAR_CHAR_GENERAL:
	case TAR_CHAR_OFFENSIVE:
	if ( arg2[0] == '\0' )
	{
	    if ( ( victim = ch->fighting ) == NULL )
		return send_to_char( "Call the power on whom?\n\r", ch );
	}
	else
	{
	    if ( ( victim = get_char_room( ch, target_name ) ) == NULL )
		return send_to_char( "They aren't here.\n\r", ch );
	}
	
	if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
		return send_to_char( "You can't do that on your own master.\n\r", ch );

	vo = (void *) victim;
	target = TARGET_CHAR;
	break;
    case TAR_CHAR_DEFENSIVE:
	if ( arg2[0] == '\0' )
	    victim = ch;
	else
	{
	    if ( ( victim = get_char_room( ch, target_name ) ) == NULL )
		return send_to_char( "They aren't here.\n\r", ch );
	}
	vo = (void *) victim;
	target = TARGET_CHAR;
	break;
    case TAR_CHAR_SELF:
	if ( arg2[0] != '\0' && !is_name( target_name, ch->name ) )
		return send_to_char( "You cannot call this power on another.\n\r", ch );

	vo = (void *) ch;
	target = TARGET_CHAR;
	break;
    case TAR_OBJ_INV:
	if ( arg2[0] == '\0' )
		return send_to_char( "What should the power be called upon?\n\r", ch );

	if ( ( obj = get_obj_carry( ch, target_name, ch ) ) == NULL )
		return send_to_char( "You are not carrying that.\n\r", ch );

	vo = (void *) obj;
	target = TARGET_OBJ;
	break;
    case TAR_OBJ_CHAR_OFF:
	if (arg2[0] == '\0')
	{
	    if ((victim = ch->fighting) == NULL)
		return send_to_char("Call the power on who or what?\n\r",ch);
	    target = TARGET_CHAR;
	}
	else if ((victim = get_char_room(ch,target_name)) != NULL)
	{
	    target = TARGET_CHAR;
	}
	if (target == TARGET_CHAR) /* check the sanity of the attack */
	{
            if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
                return send_to_char( "You can't do that on your own follower.\n\r", ch );

	    vo = (void *) victim;
 	}
	else if ((obj = get_obj_here(ch,target_name)) != NULL)
	{
	    vo = (void *) obj;
	    target = TARGET_OBJ;
	}
	else
		return send_to_char("You don't see that here.\n\r",ch);
	break;
    case TAR_OBJ_CHAR_DEF:
        if (arg2[0] == '\0')
        {
            vo = (void *) ch;
            target = TARGET_CHAR;
        }
        else if ((victim = get_char_room(ch,target_name)) != NULL)
        {
            vo = (void *) victim;
            target = TARGET_CHAR;
	}
	else if ((obj = get_obj_carry(ch,target_name,ch)) != NULL)
	{
	    vo = (void *) obj;
	    target = TARGET_OBJ;
	}
	else
		return send_to_char("You don't see that here.\n\r",ch);
	break;
    case TAR_DIR:
        if((where = direction_lookup(target_name))<0 || !(pexit=ch->in_room->exit[where]))
                return send_to_char("That's not a valid direction.\n\r",ch);
        vo = &where;
        target = TARGET_DIR;
        break;
    }
    if ( !IS_NPC(ch) && ch->mana < mana )
	return send_to_char( "You don't have enough mana.\n\r", ch );

    WAIT_STATE( ch, skill_table[sn].beats );
    if ( !IS_NPC(ch) && (number_percent( ) > get_skill(ch,sn)))
    {
	send_to_char( "You lost your concentration.\n\r", ch );
		if (sn != gsn_rage) check_improve(ch,sn,FALSE,1);
	ch->mana -= mana / 2;
    }
    else
    {
        ch->mana -= mana;
	if (skill_table[sn].target == TAR_CHAR_OFFENSIVE
	&& is_safe(ch,victim))
		return;
	
	if (skill_table[sn].target == TAR_CHAR_OFFENSIVE)
	{
		if (!IS_NPC(ch) && !IS_NPC(victim)
			&& (ch->fighting == NULL || victim->fighting == NULL))
		{
			switch(number_range(0,2))
			{
				case (0):
				case (1):
					sprintf(buf,"Die, %s you sorcerous dog!",PERS(ch,victim));
				break;
				case (2):
					sprintf(buf,"Help! %s is casting a spell on me!",PERS(ch,victim));
			}
			if (victim != ch && !IS_NPC(ch))
				do_myell(victim,buf, ch);
		}
                if(check_volley(ch,victim))
                {
                        act("$N reflects your spell right back at you!",ch,0,victim,TO_CHAR);
                        act("You reflect $n's spell right back at $m!",ch,0,victim,TO_VICT);
                        act("$N reflects $n's spell right back at $m!",ch,0,victim,TO_NOTVICT);
                        (*skill_table[sn].spell_fun) ( sn, ch->level*2, victim, ch, target);
                        return;
                }
	}
            (*skill_table[sn].spell_fun) ( sn, ch->level, ch, vo,target);
        check_improve(ch,sn,TRUE,1);
    }
    if ((skill_table[sn].target == TAR_CHAR_OFFENSIVE
    ||   (skill_table[sn].target == TAR_OBJ_CHAR_OFF && target == TARGET_CHAR))
    &&   victim != ch
    &&   victim->master != ch)
    {
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;
	un_blade_barrier(ch,NULL);
	for ( vch = ch->in_room->people; vch; vch = vch_next )
	{
	    vch_next = vch->next_in_room;
		if ( victim == vch && victim->fighting == NULL ) {
		multi_hit( victim, ch, TYPE_UNDEFINED );
		break;
	    }
	}
    }
    return;
}
void do_snare( CHAR_DATA *ch, char *argument )
{
	ROOM_AFFECT_DATA af;
	AFFECT_DATA snaretimer;

	if(get_skill(ch,gsn_snare)<5)
	{
		send_to_char("You lay some sticks down in a threatening snare to anyone under an inch tall.\n\r",ch);
		return;
	}
	if(is_affected(ch,gsn_snaretimer))
	{
		send_to_char("You do not feel ready to lay a new snare yet.\n\r",ch);
		return;
	}
	if(is_affected_room(ch->in_room,gsn_snare))
	{
		send_to_char("There is already a snare here.\n\r",ch);
		return;
	}

	if((ch->in_room->sector_type!=SECT_FOREST &&
	   ch->in_room->sector_type!=SECT_MOUNTAIN &&
	   ch->in_room->sector_type!=SECT_DESERT &&
	   ch->in_room->sector_type!=SECT_FIELD &&
	   ch->in_room->sector_type!=SECT_HILLS &&
	   !IS_IMMORTAL(ch)) ||
	   (ch->in_room->vnum==5700 || ch->in_room->cabal))
	{
		send_to_char("You cannot create a snare in this environment.\n\r",ch);
		return;
	}

   	init_affect_room(&af);
   	af.where     = TO_ROOM_AFFECTS;
	af.aftype    = AFT_SKILL;
	af.type      = gsn_snare;
   	af.level     = ch->level;
	af.duration  = 24;
	af.location  = APPLY_NONE;
	af.modifier  = 0;
	af.owner     = ch;
	af.end_fun   = NULL;
	af.tick_fun  = NULL;
	new_affect_to_room(ch->in_room, &af);
   	init_affect(&snaretimer);
   	snaretimer.where     = TO_AFFECTS;
	snaretimer.type      = gsn_snaretimer;
  	snaretimer.level     = ch->level;
	snaretimer.duration  = 24;
	snaretimer.location  = APPLY_NONE;
	snaretimer.modifier  = 0;
	snaretimer.aftype    = AFT_INVIS;
	affect_to_char(ch,&snaretimer);
	send_to_char("You lay down vines and plants in a cunningly concealed snare to trap the unwary.\n\r",ch);
	act("$n lays down vines and plants in a cunningly concealed snare to trap the unwary.\n\r",ch,0,0,TO_ROOM);
	WAIT_STATE(ch,PULSE_VIOLENCE*4);
}

void mob_tell(CHAR_DATA *mob, CHAR_DATA *ch, char *tell)
{
	char buf[MSL];
	sprintf(buf,"%s %s",ch->name,tell);
	do_tell(mob,buf);
}
int flag_value( const struct flag_type *flag_table, char *argument);
void show_flag_cmds( CHAR_DATA *ch, const struct flag_type *flag_table );

void do_forcewear(CHAR_DATA *ch, char *argument)
{
	char arg1[MSL];
	CHAR_DATA *victim;
	OBJ_DATA *obj;
	argument = one_argument(argument, arg1);
	if(!str_cmp(arg1,"") || !str_cmp(argument,"") || !(victim = get_char_room(ch, arg1)) || !(obj = get_obj_carry( ch, argument, ch )))
		return send_to_char("forcewear <character> <item in your inv>\n\r",ch);
	obj_from_char(obj);
	obj_to_char(obj,victim);
	act("You put $p on $N.",ch,obj,victim,TO_CHAR);
	act("$n puts $p on you.",ch,obj,victim,TO_VICT);
	act("$n puts $p on $N.",ch,obj,victim,TO_NOTVICT);
	do_wear(victim,obj->name);
}

void do_forceremove(CHAR_DATA *ch, char *argument)
{
    char arg1[MSL];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    argument = one_argument(argument, arg1);
    if(!str_cmp(arg1,"") || !str_cmp(argument,"") || !(victim = get_char_room(ch, arg1)) || !(obj = get_obj_wear(victim, argument)))
        return send_to_char("forceremove <character> <item they're wearing>\n\r",ch);
    act("You strip $N of $p.",ch,obj,victim,TO_CHAR);
    act("$n strips you of $p.",ch,obj,victim,TO_VICT);
    act("$n strips $N of $p.",ch,obj,victim,TO_NOTVICT);
	unequip_char(victim,obj,FALSE);
	obj_from_char(obj);
	obj_to_char(obj, ch);
}

void do_createcosmetic(CHAR_DATA *ch, char *argument)
{
	char arg1[MSL], arg2[MSL], wlname[MSL], *wearname;
	OBJ_DATA *obj;
	int value = 0;
	bool under = FALSE, wear = FALSE;
	if(!strcmp(argument,""))
	{
		send_to_char("createcosmetic 'wear location' <name>\n\r",ch);
		send_to_char("createcosmetic under <eq location> 'wear location' <name>\n\r",ch);
		send_to_char("createcosmetic eqlocations\n\r",ch);
		send_to_char("createcosmetic wear <eq location> <name>\n\r\n\r",ch);
		send_to_char("Example 1: createcosmetic 'on earlobe' a jewel-encrusted earring\n\rwould produce 'a jewel-encrusted earring' that's <worn on earlobe>\n\r",ch);
		send_to_char("Example 2: createcosmetic under arms 'as undershirt' a plain white t-shirt\n\rwould produce 'a plain white t-shirt' that's <worn as undershirt> and visible only when arms are not covered.\n\r",ch);
		send_to_char("Example 3: createcosmetic eqlocations will show all valid values for <eq location>\n\r",ch);
		return;
	}

	argument = one_argument(argument, arg1);
	if(!str_cmp(arg1,"eqlocations"))
	{
		show_flag_cmds(ch,wear_flags);
		return;
	}
	else if(!str_cmp(arg1,"under"))
	{
		argument = one_argument(argument, arg2);
		argument = one_argument(argument, wlname);
		if(!str_cmp(arg2,"") || !str_cmp(wlname,"") || !str_cmp(argument,"") ||
			(value = flag_value( wear_flags, arg2 )) == NO_FLAG)
			return send_to_char("Invalid arguments.\n\r",ch);
		under = TRUE;
	}
	else if(!str_cmp(arg1,"wear"))
	{
		argument = one_argument(argument, arg2);
		if(!str_cmp(arg2,"") || !str_cmp(argument,"") || (value = flag_value( wear_flags, arg2 )) == NO_FLAG)
			return send_to_char("Invalid arguments.\n\r",ch);
		wear = TRUE;
	}
	else if(!wear && !under)
	{
		strcpy(&wlname[0], &arg1[0]);
		if(!str_cmp(argument, ""))
			return;
	}
	else
		return;
	obj = create_object(get_obj_index(40),0);
	if(under)
		SET_BIT(obj->extra_flags, ITEM_UNDER_CLOTHES);
	if(under || wear)
		SET_BIT(obj->wear_flags, value);
	if(wear)
		REMOVE_BIT(obj->wear_flags, ITEM_WEAR_COSMETIC);
	free_pstring(obj->name);
	obj->name = palloc_string(argument);
	free_pstring(obj->short_descr);
	obj->short_descr = palloc_string(argument);
	obj_to_char(obj, ch);
	if(!wear)
	{
		if(obj->wear_loc_name)
			free_pstring(obj->wear_loc_name);
		obj->wear_loc_name = palloc_string(wlname);
	}
	act("You create $p.",ch,obj,0,TO_CHAR);
}
OBJ_DATA *make_cosmetic(char *name, char *wearloc, char *underloc, char *cosmeticloc)
{
	CHAR_DATA *ch;
	OBJ_DATA *obj;
	for(ch = char_list; ch->next; ch = ch->next)
		if(IS_NPC(ch))
			break;
	char buf[MSL];
	if(!ch)
		{ bug("Make_cosmetic bug: no charlist?!",0); return 0; }
	if(!str_cmp(wearloc,"cosmetic"))
	{
		if(!underloc)
			sprintf(buf,"'%s' %s", cosmeticloc, name);
		else
			sprintf(buf,"under %s '%s' %s", underloc, cosmeticloc, name);
	}
	else
		sprintf(buf,"wear %s %s", wearloc, name);
	do_createcosmetic(ch,buf);
	obj = ch->carrying;
	obj_from_char(obj);
	return obj;
}

#define COSM_UNDER(a,b,c,d) 	obj_to_obj(make_cosmetic(a,b,c,d),obj);
#define COSM_WEAR(a,b)			obj_to_obj(make_cosmetic(a,b,NULL,NULL),obj);
#define COSM_NORM(a,b,c)		obj_to_obj(make_cosmetic(a,b,NULL,c),obj);
void pulse_prog_repop_container(OBJ_DATA *obj, bool isTick)
{
	if(obj->contains)
		return;
}
