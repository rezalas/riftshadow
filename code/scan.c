#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

void show_char_to_char_0 args((CHAR_DATA *victim, CHAR_DATA *ch));
void scan_list           args((ROOM_INDEX_DATA *scan_room, CHAR_DATA *ch,
                               sh_int depth, sh_int door));
void do_scan(CHAR_DATA *ch, char *argument)
{
   extern char *const dir_name[];
   char arg1[MAX_INPUT_LENGTH], buf[MAX_INPUT_LENGTH];
   ROOM_INDEX_DATA *scan_room;
   OBJ_AFFECT_DATA *af;
   OBJ_DATA *obj;
   EXIT_DATA *pExit;
   sh_int door, depth, i;

   argument = one_argument(argument, arg1);

        if (!str_cmp(arg1, "n") || !str_cmp(arg1, "north")) door = 0;
   else if (!str_cmp(arg1, "e") || !str_cmp(arg1, "east"))  door = 1;
   else if (!str_cmp(arg1, "s") || !str_cmp(arg1, "south")) door = 2;
   else if (!str_cmp(arg1, "w") || !str_cmp(arg1, "west"))  door = 3;
   else if (!str_cmp(arg1, "u") || !str_cmp(arg1, "up" ))   door = 4;
   else if (!str_cmp(arg1, "d") || !str_cmp(arg1, "down"))  door = 5;
   else if (!str_cmp(arg1, "a") || !str_cmp(arg1, "all"))   door = -1;
   else { send_to_char("Which way do you want to scan?\n\r", ch); return; }

	if (IS_AFFECTED(ch,AFF_BLIND) || (get_bv_stage(ch) > 0))
		return send_to_char("You can't see anything, you're blind!\n\r",ch);

   if(door==-1)
   {
	/* "scan all", aka CheapHackLand */
	do_scan(ch,"north");
	do_scan(ch,"east");
	do_scan(ch,"south");
	do_scan(ch,"west");
	do_scan(ch,"up");
	do_scan(ch,"down");
	return;
   }

   if(is_affected_area(ch->in_room->area, gsn_cyclone))
        return send_to_char("You can't see anything but swirling winds and debris!\n\r",ch);

   act("You scan $T.", ch, NULL, dir_name[door], TO_CHAR);
   act("$n scans $T.", ch, NULL, dir_name[door], TO_ROOM);

	if(is_affected_room(ch->in_room, gsn_smokescreen))
		return send_to_char("Smoke fills the room and limits your vision!\n\r", ch);

   sprintf(buf,"All you can see %s is a towering wall of fire and smoke!\n\r",dir_name[door]);

   scan_room = ch->in_room;

   depth = 3;

   if (sun == SUN_LIGHT)	
	   depth++;
   if (sun == SUN_DARK)		
	   depth--;

   if (ch->in_room->area->sky == SKY_DRIZZLE ||
	   ch->in_room->area->sky == SKY_FLURRY ||
	   ch->in_room->area->sky == SKY_HAIL) {
	   depth--;
   }

   if (ch->in_room->area->sky == SKY_DOWNPOUR ||
	   ch->in_room->area->sky == SKY_TSTORM ||
	   ch->in_room->area->sky == SKY_BLIZZARD) {
	   depth -= 2;
   }

   if (ch->in_room->area->wind > 2) 	
	   depth--;

   if (!IS_OUTSIDE(ch))	
	   depth = 3;

   if ((obj = get_eq_char(ch,WEAR_HOLD)) != NULL)
	   if (is_affected (ch,gsn_farsee)
		   && obj->pIndexData->vnum == OBJ_VNUM_CRYSTAL) {
		   af = affect_find_obj(obj->affected,gsn_fashion_crystal);
		   if (af) {
			   act("Your vision sharpens as you peer through your multifaceted crystal.",ch,0,0,TO_CHAR);
			   depth += af->modifier/100;
		   }
	   }

   if (depth > 7)
	   depth = 7;

   if (depth <= 0) {
	   send_to_char("Weather conditions hamper your visibilty too much to see that far.\n\r",ch);
	   return;
   }

   for (i = 1; i <= depth; i++) {
	   if ((pExit = scan_room->exit[door]) != NULL) {
		   if (!IS_SET(pExit->exit_info, EX_CLOSED) || IS_SET(pExit->exit_info,EX_TRANSLUCENT)) {
			   scan_room = pExit->u1.to_room;
			   if (scan_room == NULL)
				   return;
			   if(is_affected_room(scan_room, gsn_conflagration))
				   return send_to_char(buf, ch);
			   if(is_affected_room(scan_room, gsn_smokescreen))
				   return send_to_char("A wall of smoke blocks your vision!\n\r", ch);
			   if(IS_SET(pExit->exit_info,EX_NONOBVIOUS))
				return;
			   scan_list(pExit->u1.to_room, ch, i, door);
			   send_to_char("\n\r", ch);
		   }
	   }
   }

   return;
}

void scan_list(ROOM_INDEX_DATA *scan_room, CHAR_DATA *ch, sh_int depth,
               sh_int door)
{
   CHAR_DATA *rch;
   extern char *const dir_name[];
   char *const distance[7]= {	" **** 1 %s **** ", " **** 2 %s **** ", " **** 3 %s **** ",
				" **** 4 %s **** ", " **** 5 %s **** ", " **** 6 %s **** ", " **** 7 %s **** "};
//   extern char *const distance[];
   char buf[MAX_INPUT_LENGTH], buf2[MAX_INPUT_LENGTH];

   buf[0] = '\0';

   if (scan_room == NULL) return;

   sprintf(buf2, distance[(depth-1)], dir_name[door]);

   strcat(buf, buf2);

   send_to_char(buf, ch);
   send_to_char("\n\r", ch);

   for (rch=scan_room->people; rch != NULL; rch=rch->next_in_room) {
	   if (rch == ch) continue;
	   if (!IS_NPC(rch) && rch->invis_level > get_trust(ch)) continue;
	   if (can_see(ch, rch)) show_char_to_char_0(rch, ch);
   }
   return;
}
