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
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "merc.h"
#include "tables.h"

int flag_lookup (const char *name, const struct flag_type *flag_table)
{
    int flag;

    for (flag = 0; flag_table[flag].name != NULL; flag++)
    {
	if (LOWER(name[0]) == LOWER(flag_table[flag].name[0])
	&&  !str_prefix(name,flag_table[flag].name))
	    return flag_table[flag].bit;
    }
    return NO_FLAG;
}

int climate_lookup (const char *name)
{
	int climate;

	for (climate = 0; climate_table[climate].name != NULL; climate++)
	{
		if (LOWER(name[0]) == LOWER(climate_table[climate].name[0])
			&& !str_prefix(name,climate_table[climate].name))
		{
			return climate_table[climate].number;
		}
	}

	return 0;
}

int restrict_lookup (const char *name)
{
    int flag;

    for (flag = 0; restrict_table[flag].name != NULL; flag++)
    {
        if (LOWER(name[0]) == LOWER(restrict_table[flag].name[0])
        &&  !str_prefix(name,restrict_table[flag].name))
            return restrict_table[flag].bit;
    }
    return NO_FLAG;
}

RACE_DATA *race_data_lookup (const int race)
{
	RACE_DATA *temp;
	bool found = FALSE;

	temp = race_list;

	while (temp) {
		if (race == temp->number) {
			found = TRUE;
			break;
		}
		temp = temp->next;
	}

	if (!found)
		bug("Race lookup failed!!",0);

	if (!temp || !found)
		temp = race_list;

	return(temp);
}

int display_lookup (const char *name, const struct display_type *flag_table)
{
    int flag;

    for (flag = 0; flag_table[flag].name != NULL; flag++)
    {
        if (LOWER(name[0]) == LOWER(flag_table[flag].name[0])
        &&  !str_prefix(name, flag_table[flag].name))
            return flag_table[flag].bit;
    }
    return 0;
}

char * get_demon_names(CHAR_DATA *ch)
{
	int favor, i;
	char buf[MSL], buf2[MSL], buf3[MSL];
	bool dFound = FALSE;

	sprintf(buf, "Lesser: ");
	buf2[0] = '\0';
	for(i=0; i < MAX_LESSER; i++)
		if((favor = ch->pcdata->lesserdata[i]) != FAVOR_NONE)
		{
			sprintf(buf3,"%s (%s)  ", capitalize(demon_table[i].name), 
					favor >= FAVOR_GRANTED ? "Granted" : favor == FAVOR_FAILED ? "Failed" : "In Progress");
			strcat(buf2,buf3);
		}
	
	if(buf2[0] == '\0')
		sprintf(buf2, "None\t");
	
	strcat(buf, buf2);
	strcat(buf, "\tGreater: ");
	buf2[0] = '\0';
	for(i = MAX_LESSER; i < MAX_GREATER + MAX_LESSER; i++)
		if((favor = ch->pcdata->lesserdata[i]) != FAVOR_NONE)
		{
			sprintf(buf3,"%s (%s)  ", capitalize(demon_table[i].name),
					favor >= FAVOR_GRANTED ? "Granted" : favor == FAVOR_FAILED ? "Failed" : "In Progress");
			strcat(buf2,buf3);
		}

	if(buf2[0] == '\0')
		sprintf(buf2, "None");

	strcat(buf, buf2);
	return talloc_string(buf);
}

char * display_name_lookup(long bitv, const struct display_type *flag_table)
{
        int flag;
        for(flag = 0; flag_table[flag].name != NULL; flag++)
                if(flag_table[flag].bit == bitv)
                        return (flag_table[flag].name);
        return NULL;
}
char * restrict_name_lookup(long bitv)
{
        int flag;
        for(flag = 0; restrict_table[flag].name != NULL; flag++)
			if(restrict_table[flag].bit == bitv)
				return (restrict_table[flag].name);
        return NULL;
}
char * tribe_name_lookup(long bitv)
{
	int flag;
	for(flag = 0; tribe_table[flag].name; flag++)
		if(tribe_table[flag].tribe_num == bitv)
			return (tribe_table[flag].name);
	return NULL;
}
int ele_name_lookup(const char *name)
{
	int i;
	for (i = 0; i < MAX_ELE; i++)
		if (LOWER(name[0]) == LOWER(sphere_table[i].name[0])
			&& !str_prefix(name, sphere_table[i].name))
			return (sphere_table[i].element);

	return -1;
}

char * wealth_lookup(int number)
{
	int i;
	for (i = 0; i < 6; i++)
		if (wealth_table[i].bit == number)
			return (wealth_table[i].name);

	return "none";
}

char * flag_name_lookup(long bitv, const struct flag_type *flag_table)
{
	int flag;
	for(flag = 0; flag_table[flag].name != NULL; flag++)
		if(flag_table[flag].bit == bitv)
			return talloc_string(flag_table[flag].name);
	return NULL;
}
int flag_index_ilookup(int i, const struct flag_type *flag_table)
{
    int flag;
    for (flag = 0; flag_table[flag].name != NULL; flag++)
	    if(flag_table[flag].bit == i)
            	return flag;
    return -1;
}
int flag_index_lookup(const char *name, const struct flag_type *flag_table)
{
    int flag;

    for (flag = 0; flag_table[flag].name != NULL; flag++)
    {
        if (LOWER(name[0]) == LOWER(flag_table[flag].name[0]) &&
				!str_prefix(name,flag_table[flag].name))
            return flag;
    }
    return -1;
}
int material_lookup(const char *name)
{
    int flag;

    for (flag = 0; material_table[flag].mat_name != NULL; flag++)
    {
        if (LOWER(name[0]) == LOWER(material_table[flag].mat_name[0])
        &&  !str_prefix(name,material_table[flag].mat_name))
            return flag;
    }
    return -1;
}
int sect_lookup(const char *name)
{
    int flag;

    for (flag = 0; sect_table[flag].name != NULL; flag++)
    {
        if (LOWER(name[0]) == LOWER(sect_table[flag].name[0])
        &&  !str_prefix(name,sect_table[flag].name))
            return flag;
    }
    return 0;
}

int sect_numlookup(int number)
{
    int flag;
	for(flag = 0; sect_table[flag].name!=NULL; flag++)
		if(sect_table[flag].value == number)
			return flag;
    return 0;
}
int cabal_lookup(const char *name)
{
    int cabal;

    for (cabal = 0; cabal < MAX_CABAL; cabal++)
    {
        if (LOWER(name[0]) == LOWER(cabal_table[cabal].name[0])
        &&  !str_prefix(name,cabal_table[cabal].name))
            return cabal;
    }

    return 0;
}

int position_lookup (const char *name)
{
   int pos;

   for (pos = 0; position_table[pos].name != NULL; pos++)
   {
	if (LOWER(name[0]) == LOWER(position_table[pos].name[0])
	&&  !str_prefix(name,position_table[pos].name))
	    return pos;
   }

   return -1;
}

int sex_lookup (const char *name)
{
   int sex;

   for (sex = 0; sex_table[sex].name != NULL; sex++)
   {
	if (LOWER(name[0]) == LOWER(sex_table[sex].name[0])
	&&  !str_prefix(name,sex_table[sex].name))
	    return sex;
   }

   return -1;
}

int size_lookup (const char *name)
{
   int size;

   for ( size = 0; size_table[size].name != NULL; size++)
   {
        if (LOWER(name[0]) == LOWER(size_table[size].name[0])
        &&  !str_prefix( name,size_table[size].name))
            return size;
   }

   return -1;
}

int hometown_lookup(const char *name)
{
    int hometown;

    for (hometown = 0; hometown < MAX_HOMETOWN; hometown++)
    {
        if (LOWER(name[0]) == LOWER(hometown_table[hometown].name[0])
        &&  !str_prefix(name,hometown_table[hometown].name))
            return hometown;
    }

    return 0;
}

/* Function to generate the number for the direction by passing the word, GABE */
int direction_lookup(char *dir)
{
 	int num;

	if (!str_cmp(dir, "n") || !str_cmp(dir, "north"))
		num=0;
	else if (!str_cmp(dir, "e") || !str_cmp(dir, "east"))
		num=1;
	else if (!str_cmp(dir, "s") || !str_cmp(dir, "south"))
		num=2;
	else if (!str_cmp(dir, "w") || !str_cmp(dir, "west"))
		num=3;
	else if (!str_cmp(dir, "u") || !str_cmp(dir, "up" ))
		num=4;
	else if (!str_cmp(dir, "d") || !str_cmp(dir, "down"))
		num=5;
	else
		num=-1;

 return num;
}

/* chop the string into two variables */
void half_chop(const char *string, char *arg1, char *arg2)
{
  while(isspace(*string))
    string++;

  while(*string && !isspace(*string))
    *arg1++ = *string++;
  *arg1 = '\0';

  while(isspace(*string))
    string++;

  strcpy(arg2, string);
}

char * chaldir(int dir)
{
	if(dir == DIR_NORTH)
		return "the north";
	else if(dir == DIR_SOUTH)
		return "the south";
	else if(dir == DIR_WEST)
		return "the west";
	else if(dir == DIR_EAST)
		return "the east";
	else if(dir == DIR_UP)
		return "above";
	else if(dir == DIR_DOWN)
		return "down";
	else
		return "null";
}
