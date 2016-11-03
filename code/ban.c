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
#include "recycle.h"
#include "time.h"

bool check_ban(char *usite,int type,int host)
{
    BAN_DATA *pban;
    char buf[MSL],site[MSL];
	int res=0;
	CRow row;

	strcpy(site,capitalize(usite));
    site[0] = LOWER(site[0]);


	
	res = RS.SQL.Select("site,duration FROM bans WHERE ban_type=%d AND host_type=%d", type,host);
	if(res)
	{
		while(!RS.SQL.End())
		{
			row = RS.SQL.GetRow();
			if(strstr(site,row[0]) != NULL)
			{
				sprintf(buf,"BANNED - %s just tried to connect.", site);
				wiznet(buf,NULL,NULL,WIZ_LOGINS,0,0);
				return TRUE;
			}
		}
	}

	return FALSE;
}


void do_ban(CHAR_DATA *ch, char *argument)
{
    char buf[MSL];
	char arg1[MSL], arg2[MSL], arg3[MSL], arg4[MSL], date[MSL];
    BUFFER *buffer;
    int ban_type=0,host_type=0,res,duration=0;
	CRow row;
	
	argument = one_argument(argument,arg1);
    argument = one_argument(argument,arg2);
	argument = one_argument(argument,arg3);
	argument = one_argument(argument,arg4);
	
	if (arg1[0] != '\0' && !str_prefix(arg1,"show"))
    {
		res = RS.SQL.Select("bans.*,DATE_FORMAT(date,\'%%m/%%d/%%Y\') FROM bans ORDER BY duration DESC");
		if(res)
		{
			buffer = new_buf();
			
			sprintf(buf,"%-25s\t%-10s\t%-10s\t%-10s\t%-10s\t%-10s\n\r", "Site", "By", "Type", "Date", "Duration", "Reason");
			add_buf(buffer,buf);
			while(!RS.SQL.End())
			{
				row = RS.SQL.GetRow();
			
				sprintf(buf,"%-25s\t%-10s\t%-10s\t%-10s\t%-10s\t%-10s\n\r",row[0],row[1],(atoi(row[5]) > 0) ? "Newbie" : "All",row[7],row[4],row[2]);
				
				add_buf(buffer,buf);
			}
			
			page_to_char( buf_string(buffer), ch );
			free_buf(buffer);
		}
		else
			send_to_char("There are no current bans placed.\n\r", ch);
        return;
    }

    if (arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' || arg4[0] == '\0')
	{
		do_ban(ch,"show");
		send_to_char("Syntax:    ban <site> <ip/host> <newbie/all> <duration> <reason>    (-1 duration is permanent)\n\r"\
							"           unban <site>\n\r"\
							"           ban show\n\r",ch);
		return;
	}

	if (!str_prefix(arg2,"ip"))
		host_type = NBAN_IP;
	else if (!str_prefix(arg2,"host"))
		host_type = NBAN_HOST;
	else
		return do_ban(ch,"");

    if (!str_prefix(arg3,"all"))
		ban_type = NBAN_ALL;
    else if (!str_prefix(arg3,"newbie"))
		ban_type = NBAN_NEWBIE;
    else
		return do_ban(ch,"");

	duration = atoi(arg4);

	strftime(date, 200, "%Y-%m-%d", localtime(&current_time));

	res = RS.SQL.Insert("bans VALUES(\'%s\',\'%s\',\'%s\',\'%s\',%d,%d,%d)",
			arg1,ch->true_name,argument,date,duration,ban_type,host_type);
	if(res)
		return send_to_char("Ban added.\n\r", ch);
	else
		return send_to_char("Ban failed, error..yell at Morglum.\n\r", ch);
}

void do_unban( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
	int res=0;

    if ( argument[0] == '\0' )
		return send_to_char( "Remove which site from the ban list?\n\r", ch );


	res = RS.SQL.Delete("bans WHERE site LIKE \'%%%s%%\'", argument);

	if(res)
		send_to_char("Site unbanned.\n\r", ch);
	else
		send_to_char( "Site is not banned.\n\r", ch );
    
	return;
}
