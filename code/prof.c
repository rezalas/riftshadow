/* Proficiencies.  -Cal */
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

//don't forget to increment max_prof in prof.h and clean make if your new proficiency goes over MAX_PROF
//or there are SIGSEGVs and axes to the face in your future
char * flag_name_lookup args((long bitv, const struct flag_type *flag_table));
CProficiencies prof_none; 
CProficiencies * char_data::Profs()
{ return pcdata ? &pcdata->profs : &prof_none; }

//TOFIX: THE CHECK TO PROF IMPROVEMENT % IS WAY OFF
#define MAX_PLEVEL 5
#define POINTS_PER_HOUR	5
const struct prof_level_type prof_level_table[MAX_PLEVEL] =
{
	{ "incapable",		-1 	},
	{ "proficient",		1	},
	{ "very proficient",	4	},
	{ "adept",		7	},
	{ "masterful",		10	},
};
/* note: profs can be referred to either by psn or by profname, use psn in the rare cases where it's checked very often
        otherwise use psn_none and refer by profname */

const struct proficiency_type prof_table [] =
{
	//ppsn, 		name,       		cost, 		mnlevel,requires, 	flags
	{ &psn_swimming, 	"swimming", 		18, 		1, 	NULL,		PFLAGS_BASIC 	},
	{ &psn_none, 		"butchery", 		18, 		1, 	NULL,		PFLAGS_BASIC 	},
	{ &psn_mountaineering,	"mountaineering", 	13,		1,	NULL,		PFLAGS_BASIC	},
	{ &psn_none,		"bandaging",		18,		1,	NULL,		PFLAGS_BASIC	},
	{ &psn_none,		"appraising", 		15,		1,	NULL,		PFLAGS_BASIC	},
	{ &psn_none,		"trap detecting", 	20,		1,	NULL,		PFLAGS_BASIC	},
	{ &psn_none,		"cooking",  		13,		1,	NULL,		PFLAGS_BASIC	},
	{ &psn_none,		"firestarting", 	15,		1,	NULL,		PFLAGS_BASIC	},
	{ &psn_none,		"wildlore", 		10,		1,	NULL,		0		},
	{ &psn_none,		"wilderness subsistence", 26, 		1,	NULL,		0		},
	{ &psn_none,		"ancient languages",	25,		30,	NULL,		0		},
	{ &psn_none,		"forgotten lore",	20,		30,	NULL,		0		},
	{ &psn_none,		"tracking",		30,		30,	NULL,		0		},
	{ &psn_none,		NULL,	     0,			0,	NULL,		0   		},
};

DECL_PROF_CMD(prof_butcher)
DECL_PROF_CMD(prof_bandage)
DECL_PROF_CMD(prof_appraise)
DECL_PROF_CMD(prof_cook)
DECL_PROF_CMD(prof_firestart)
DECL_PROF_CMD(prof_tracking)

/*
void prof_butcher (CHAR_DATA *ch, char *argument);
void prof_bandage (CHAR_DATA *ch, char *argument);
void prof_appraise (CHAR_DATA *ch, char *argument);
void prof_cook (CHAR_DATA *ch, char *argument);
void prof_firestart (CHAR_DATA *ch, char *argument);
void prof_tracking (CHAR_DATA *ch, char *argument);
*/
const struct prof_cmd_type prof_cmd_table [] = {
	{ "butcher",	prof_butcher,	"butchery"},
	{ "bandage",	prof_bandage,	"bandaging"},
	{ "appraise",	prof_appraise,	"appraise"},
	{ "cook",	prof_cook,	"cook"},
	{ "firestart",	prof_firestart, "firestarting"},
	{ "track",	prof_tracking,	"track"},
};

/* START_PROF_COMMANDS
 command name, function name, required prof
PROF_COMMAND(butcher, prof_butcher, butchery)
PROF_COMMAND(bandage, prof_bandage, bandaging)
PROF_COMMAND(appraise, prof_appraise, appraising)
PROF_COMMAND(cook, prof_cook, cooking)
PROF_COMMAND(firestart, prof_firestart, firestarting)
PROF_COMMAND(track, prof_tracking, tracking)
END_PROF_COMMANDS */

struct proficiency_msg
{
	char *learning_msgs[5];
};

sh_int psn_none;
sh_int psn_swimming;
sh_int psn_mountaineering;

//last entry in each one should be null
const struct proficiency_msg prof_msg_table [] =
{
/* swimming */ 	{ { "$N describes basic swimming strokes.", "$N shares a few anecdotes about Shalaran rivers and oceans.", 
		"$N makes you practice breathing in the most efficient manner.", NULL } },
/* butchery */	{ { "$N grumbles and drags a dead calf out of the back room.", 
		"Taking time to point out the 'good parts', $N hacks up the corpse.",
		"By the time $E's done, chunks of fresh, edible meat litter the floor around the corpse.", NULL } },
/* mountain */  { { "$N begins to speak about the common mistakes inexperienced travelers make when traversing mountains.",  
		"$N describes how to avoid treacherous spots that could tire legs and make travel more difficult.", NULL } } ,
/* bandaging*/	{ { "$N takes out a roll of bandages and a tourniquet, explaining where to apply pressure.",
		"$N describes how to treat the various types of bleeding you might encounter.",
		"$N wraps a few practice bandages as you watch.", NULL } } ,
/* appraising */{ { "$N pulls out a gem and holds it up to the light, pointing out tiny flaws and irregularities.",
		"$N shows you basic methods of estimating value based on an items condition and rarity.",
		"$N describes how to best convince a shopkeeper that their price is too high based on your knowledge.", NULL } },
/* trap detect*/{ { "$N describes the most common types of traps you will encounter in your travels.",
		"$N shows you how to recognize trap triggers before you stumble into them.",
		"$N makes you practice rolling and ducking to avoid traps after you've triggered them.", NULL } },
/* cooking */	{ { "$N mutters and tells you to watch, tossing some raw meat into a pan and frying it.",
		"$N makes you practice cooking common foods, requiring you to prove basic competence before moving on.",
		"$N nods slightly as you finish cooking a chicken.", NULL } },
/* firest */	{ { "$N shows you how to gather kindling and small twigs.",
		"$N demonstrates the best way to arrange the wood for the fire.",
		"$N makes you rub sticks together until you can reliably produce a spark.", NULL } },
/* wildlore*/	{ { "Blah", NULL } },
/* w. subsist*/ { { "Blah2", NULL } },
/* ancient lan*/{ { "$N hobbles slowly over to the bookshelf, returning with numerous old, dusty books.", 
		"$N drops the books on a table with a thud, motioning for you to open the one before you.",
		"$N goes on to explain the intricacies of certain languages, demonstrating pronunciation, reading, and writing.", 
		NULL } },
/* forg lore */ { { "$N takes out some ancient artifacts, showing you common identifiers to recognize.",
		"$N hands you a musty old tome, suggesting you memorize the contents for future reference.",
		"You spend a few hours studying the index.", NULL } },
/* tracking */  { { "$N beckons you over to a nearby tree.", 
		"As $E points out small details you would not have noticed on your own, you see something has passed by recently.", 
		"$E tells you of other signs to look for, and you feel more adept at tracking.", NULL } },
};

#define IF_PROF_IS(x) if(!str_cmp(profname, #x))
float CProficiencies::ProfEffect(char *profname, float nArg)
{
	int pindex = ProfIndexLookup(profname);
	/* swimming */
	IF_PROF_IS(appraising)
	{
		CheckImprove(profname, 50);
		return 100 - ((profs[pindex] * 2) + 10); // 12% off at 1, 30% at 10
	}
	IF_PROF_IS(trap detecting)
	{
		//returned value needs to be >= than quality of the trap to detect its presence
		//if you have a skill of 1 and a trap of 5, you need a rnd >=4 (40% chance)
		//if you have a skill of 1 and a trap of 9, you need a rnd =8 (10% chance)
		//if you have a skill of 1 and a trap of 1, you need a rnd >=0 (80% chance)
		//that's for detection in adjacent room, to avoid the trap as it fires the chance is detect / 2
		return number_range(-2, 8) + profs[pindex];
	}
	IF_PROF_IS(firestarting)
	{
		return (nArg + 25) / 25; // multiplied by gain to get hp gain
	}
	return 0;
}
void add_prof_affect(CHAR_DATA *ch, char *name, int duration, bool fInvis = TRUE)
{
        AFFECT_DATA af;
        init_affect(&af); 
	af.type = gsn_timer; af.name = palloc_string(name); af.duration = duration; af.aftype = fInvis ? AFT_INVIS : AFT_SKILL;
        affect_to_char(ch, &af);
}
bool is_affected_prof(CHAR_DATA *ch, char *prof)
{
	AFFECT_DATA *paf;
	for(paf = ch->affected; paf; paf = paf->next)
		if(paf->type == gsn_timer && paf->name && !str_cmp(paf->name, prof))
			return TRUE;
	return FALSE;
}

#define OBJ_VNUM_MEAT_CHUNKS 75
#define OBJ_VNUM_CAMPFIRE 76

void prof_tracking(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim;
	int i, sect = ch->in_room->sector_type;
	char buf[MSL], *direction = NULL;
	
	if(is_affected_prof(ch, "tracking"))
		return send_to_char("You cannot attempt to track them again yet.\n\r",ch);
	if(!(victim = get_char_world(ch, argument)) || victim == ch || IS_NPC(victim))
		return send_to_char("Track who?\n\r",ch);
	if(!IS_GROUND(ch->in_room) || sect == SECT_CITY || sect == SECT_INSIDE || sect == SECT_BURNING || sect == SECT_ROAD)
		return send_to_char("Even if they had been here, there would be no suitable tracks left for you to follow.\n\r",ch);
	act("$n begins to poke and prod at the ground, clearly searching for something.",ch, 0, 0, TO_ROOM);
	sprintf(buf,"You were unable to find any sign of $N here.");
	for (i=0;i<MAX_TRACKS;i++)
	{
        	if (!ch->in_room->tracks[i])
			break;
		if (ch->in_room->tracks[i]->prey != victim || ch->in_room->tracks[i]->flying || ch->in_room->tracks[i]->sneaking)
			continue;
		TIME_INFO_DATA &ttime = ch->in_room->tracks[i]->time;
		if (abs(ttime.day - time_info.day) > 1 || (ttime.month != time_info.month && ttime.day != 30))
			continue;
		if ((ttime.day == time_info.day && abs(time_info.hour - ttime.hour) > 3) ||
		    (ttime.day == time_info.day - 1 && ttime.hour >= 20 && time_info.hour <= 3))
		{
			sprintf(buf,"You can tell $N has been here, but the tracks have deteriorated too much to tell which way $E left.");
			break;
		}
		int diruse = (number_percent() > (35 - (ch->Profs()->GetProf("tracking")) * 2)) ? 
			ch->in_room->tracks[i]->direction : number_range(0, MAX_DIR - 1);
		direction = (char*)flag_name_lookup(diruse,direction_table);
		sprintf(buf,"From the pattern of tracks here, you suspect $N left $t.");
	}
	act(buf,ch,direction,victim,TO_CHAR);
	add_prof_affect(ch, "tracking", (UMAX(0, 5 - ch->Profs()->GetProf("tracking"))));
	ch->Profs()->CheckImprove("tracking", 150);
}

void build_fire(CHAR_DATA *ch, int dur)
{
	OBJ_DATA *fire;
	char buf[MSL];
	if(number_percent() > 60 + ch->Profs()->GetProf("firestarting") * 3)
	{
		act("$n's fire dies out despite $s best attempts to kindle it.", ch, 0, 0, TO_ROOM);
		act("Your fire dies out despite your best attempts to kindle it.", ch, 0, 0, TO_CHAR);
		ch->Profs()->CheckImprove("firestarting", 300);
		return;
	}
	sprintf(buf,"%sThe spark is caught by some dried brush, and it quickly erupts into a blazing campfire!%s", get_char_color(ch, "lightred"), END_COLOR(ch));
	act(buf, ch, 0, 0, TO_ALL);
	fire = create_object(get_obj_index(OBJ_VNUM_CAMPFIRE), ch->level);
        fire->value[0] = ch->Profs()->GetProf("firestarting") + 1;
	fire->timer = dur;
	obj_to_room(fire, ch->in_room);
	add_prof_affect(ch, "firestarting", dur);
	ch->Profs()->CheckImprove("firestarting", 250);
}
void prof_firestart(CHAR_DATA *ch, char *argument)
{
	int dur, sect = ch->in_room->sector_type;
	OBJ_DATA *fire;
	if(is_affected_prof(ch, "firestarting"))
		return send_to_char("You cannot start another fire yet.\n\r",ch);
	if(sect == SECT_WATER || sect == SECT_UNDERWATER || sect == SECT_VERTICAL || sect == SECT_AIR || sect == SECT_CITY ||
		sect == SECT_INSIDE)
		return send_to_char("You can't build a fire here.\n\r",ch);
	for(fire = ch->in_room->contents; fire; fire = fire->next_content)
		if(fire->item_type == ITEM_CAMPFIRE)
			return send_to_char("There is already a fire burning here.\n\r",ch);
	if(ch->move < ch->level)
		return send_to_char("You are too tired to gather wood.\n\r",ch);
	dur = 12;
	act("You begin to build a campfire, gathering sticks and twigs from your surroundings.", ch, 0, 0, TO_CHAR);
	act("$n begins to build a campfire, gathering sticks and twigs from $s surroundings.", ch, 0, 0, TO_ROOM);
	ch->move -= ch->level;
	RS.Queue.AddToQueue(1, 2, send_to_char, "You rub two sticks together, trying to produce a flame.\n\r", ch);
	RS.Queue.AddToQueue(1, 5, act, "$n rubs two sticks together, trying to produce a flame.", ch, 0, 0, TO_ROOM);
	RS.Queue.AddToQueue(3, 2, send_to_char, "The sticks begin to smoke, and soon you produce a spark.\n\r", ch);
	RS.Queue.AddToQueue(3, 5, act, "The sticks begin to smoke, and soon $n produces a spark.", ch, 0, 0, TO_ROOM);
	RS.Queue.AddToQueue(5, 2, build_fire, ch, dur);
	WAIT_STATE(ch, PULSE_VIOLENCE * 2);
}
void prof_cook(CHAR_DATA *ch, char *argument)
{
	OBJ_DATA *cooked;
	if(argument[0] == '\0' || !(cooked = get_obj_carry(ch, argument, ch)))
		return send_to_char("Syntax: cook <food>",ch);
	
}
void prof_appraise(CHAR_DATA *ch, char *argument)
{
	OBJ_DATA *obj;
	if(argument[0] == '\0' || !(obj = get_obj_carry(ch, argument, ch)))
		return send_to_char("You aren't carrying that.\n\r",ch);
	float mcost = (10 - ch->Profs()->GetProf("appraising")) * number_percent() / 1200, tcost;
	if(number_percent() > 50)
		tcost = obj->cost + (mcost * obj->cost);
	else
		tcost = UMAX(0, obj->cost - (mcost * obj->cost));
	char buf[MSL];
	sprintf(buf,"You estimate the value of $p to be approximately %d gold.",(int)tcost);
	act(buf, ch, obj, 0, TO_CHAR);
	ch->Profs()->CheckImprove("appraising", 80);
	WAIT_STATE(ch, PULSE_VIOLENCE);
}
void prof_butcher(CHAR_DATA *ch, char *argument)
{
	OBJ_DATA *obj, *meat;
	if(argument[0] == '\0' || !(obj = get_obj_here(ch, argument)))
		return send_to_char("You don't see that here.\n\r",ch);
	if(obj->item_type != ITEM_CORPSE_NPC)
		return send_to_char("You can't butcher that.\n\r",ch);
	if(obj->contains)
		return send_to_char("That corpse is too full to be gutted.\n\r",ch);
	act("$n butchers $p, carefully gutting it, extracting the edible portions and slicing them into rough chunks.",
		ch, obj, 0, TO_ROOM);
	act("You butcher $p, carefully gutting it, extracting the edible portions and slicing them into rough chunks.",
		ch, obj, 0, TO_CHAR);
	extract_obj(obj);
	int nummeat = UMAX(1, (ch->Profs()->GetProf("butchery") + 2) / 3);
	act("After the butchering process, you are left with $i pieces of meat.", ch, (void *)nummeat, 0, TO_CHAR);
	for(int i = 0; i < nummeat; i++)
	{
		meat = create_object(get_obj_index(OBJ_VNUM_MEAT_CHUNKS), ch->level);
		obj_to_char(meat, ch);
		meat->value[0] += ch->Profs()->GetProf("butchery");
		meat->value[1] += ch->Profs()->GetProf("butchery");
		meat->timer = 12 + ch->Profs()->GetProf("butchery") * 2;
	}
	ch->Profs()->CheckImprove("butchery", 100);
	WAIT_STATE(ch, PULSE_VIOLENCE);
	return;
}
void prof_bandage(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim = ch;
	AFFECT_DATA af, *paf, *baf;
	init_affect(&af);
	if(argument[0] != '\0')
		victim = get_char_room(ch, argument);
	if(!victim)
		return send_to_char("They aren't here.\n\r",ch);
	if(!(baf = affect_find(victim->affected, gsn_bleeding)))
		return send_to_char("You can only bandage wounds that are bleeding.\n\r",ch);
	if(is_affected_prof(ch, "bandage"))
		return send_to_char("That wound has been bandaged too recently.\n\r",ch);
	//if(baf->level)
	//	baf->level /= 2;
	add_prof_affect(ch, "bandage", 2, FALSE);
	if(victim == ch)
	{
		send_to_char("You bandage your wounds, staunching the worst of the bleeding.\n\r",ch);
		act("$n bandages $s wounds, staunching the worst of $s bleeding.",ch,0,0,TO_ROOM);
	}
	else
	{
		act("You bandage $N's wounds, staunching the worst of $S bleeding.",ch,0,victim,TO_CHAR);
		act("$n bandages your wounds, staunching the worst of the bleeding.",ch,0,victim,TO_VICT);
		act("$n bandages $N's wounds, staunching the worst of $S bleeding.",ch,0,victim,TO_NOTVICT);
	}
	float hadd = (UMAX(ch->Profs()->GetProf("bandaging") * .4, 1) * victim->level) + victim->hit;
	victim->hit = UMIN(victim->max_hit, (int)hadd);
	ch->Profs()->CheckImprove("bandaging", 1000);
	WAIT_STATE(ch, PULSE_VIOLENCE);
}

bool CProficiencies::InterpCommand(char *command, char *argument)
{
	int i, pindex;

	// Quick Test - Morglum
	return FALSE;

	for(i = 0; prof_cmd_table[i].name != NULL; i++)
		if(!str_prefix(command, prof_cmd_table[i].name)
		&& 
			(!str_cmp(prof_cmd_table[i].requires,"none") ||
			(pindex = CProficiencies::ProfIndexLookup(prof_cmd_table[i].requires)) == -1 ||
			ch->Profs()->HasProf(pindex) == TRUE))
			{
				(prof_cmd_table[i].cmd) (ch, argument);
				return TRUE;
			}
			
	return FALSE;
}

void do_proficiencies(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *iterator, *trainer = NULL;
	char buf[MSL];
	bool bBreak = FALSE;

	if(IS_NPC(ch))
		return;
	for(iterator = ch->in_room->people; iterator && !bBreak; iterator = iterator->next_in_room)
		for(int i = 0; i < MAX_PROFS_TAUGHT_BY_MOB && !bBreak; i++)
			if(IS_NPC(iterator) && iterator->pIndexData->profs_taught[i] > -1)
			{
				bBreak = TRUE;
				trainer = iterator;
			}
	if(!trainer && str_cmp(argument, "basic"))
	{
		bool bFound = FALSE;
		for(int i = 0; prof_table[i].name != NULL; i++)
			if(ch->Profs()->GetProf(i) > -1)
			{
				if(!bFound)
					send_to_char("Your proficiencies are:\n\r",ch);
				bFound = TRUE;
				sprintf(buf,"You are %s at %s.\n\r", ch->Profs()->GetSkillLevelName(i), prof_table[i].name);
				send_to_char(buf,ch);
			}
		if(!bFound)
			send_to_char("You currently have no proficiencies.\n\r",ch);
		return;
	}
	else if (!str_cmp(argument, "basic"))
	{
		send_to_char("The basic proficiencies available to adventurers are:\n\r",ch);
		for(int i = 0; prof_table[i].name != NULL; i++)
                        if(prof_table[i].flags & PFLAGS_BASIC)
                        {
                                sprintf(buf,"%-20s  ", prof_table[i].name);
				if((i+1) % 3 == 0)
				{
					int slen = strlen(buf), j;
					for(j = 1; buf[slen - j] == ' '; j++)
						;
					buf[slen - j + 1] = '\0';
				}
                                send_to_char(buf,ch);
                                if((i+1) % 3 == 0)
                                        send_to_char("\n\r",ch);
                        }
		send_to_char(
		"\n\rMany other proficiencies are known to Shalaran adventurers, but you must first find a teacher.\n\r",ch);
		return;
	}
	if(!trainer)
		return;
	if(argument[0] != '\0')
	{
		char arg1[MSL];
		argument = one_argument(argument, arg1);
		if(str_cmp(arg1, "train"))
			return send_to_char("Syntax: proficiencies train <proficiency>\n\r",ch);
		int prof = CProficiencies::ProfIndexLookup(argument), i;
		for(i = 0; i < MAX_PROFS_TAUGHT_BY_MOB; i++)
			if(trainer->pIndexData->profs_taught[i] == prof)
				break;
		if(prof == -1 || i == MAX_PROFS_TAUGHT_BY_MOB)
			return send_to_char("You can't study that here.\n\r",ch);
		if(ch->Profs()->GetProf(prof) > 0)
			return send_to_char("You are already familiar with that proficiency.\n\r",ch);
		if(prof_table[prof].cost > ch->Profs()->GetPoints())
			return send_to_char("You don't have enough points to study that proficiency.\n\r",ch);
		if(prof_table[prof].requires && CProficiencies::ProfIndexLookup(prof_table[prof].requires) != -1 
			&& ch->Profs()->GetProf(CProficiencies::ProfIndexLookup(prof_table[prof].requires)) < 0)
			return send_to_char("That proficiency requires understanding of a proficiency you do not possess.\n\r",ch);
		if(prof_table[prof].minlevel > ch->level)
			return send_to_char("You are not advanced enough in your guild to learn that proficiency.\n\r",ch);
		act("You ask $N to teach you about $t.", ch, prof_table[prof].name, trainer, TO_CHAR);
		for(i = 0; prof_msg_table[prof].learning_msgs[i] != NULL && i < 5; i++)
		{
			if(i == 4 || !prof_msg_table[prof].learning_msgs[i+1])
			{
				sprintf(buf,"%sYou are now proficient at %s.%s\n\r",
					get_char_color(ch,"yellow"),prof_table[prof].name,END_COLOR(ch));
                		char *tptr = talloc_string(buf); 
                		RS.Queue.AddToQueue((i + 1) * 2, 2, send_to_char, tptr, ch);				
			}
			RS.Queue.AddToQueue((i + 1) * 2, 5, act, prof_msg_table[prof].learning_msgs[i], ch, 0, trainer, TO_CHAR);
		}
		ch->Profs()->DeductPoints(prof_table[prof].cost);
		ch->Profs()->SetProf(prof, 1);
		WAIT_STATE(ch, (i + 1) * 2);
		return;
	}
	act("You may learn the following proficiencies from $N:", ch, 0, trainer, TO_CHAR);
	for(int i = 0; prof_table[i].name != NULL; i++)
		for(int j = 0; j < MAX_PROFS_TAUGHT_BY_MOB; j++)
			if(trainer->pIndexData->profs_taught[j] == i)
			{
				sprintf(buf,"%-16s | %d points\n\r", prof_table[i].name, prof_table[i].cost);
				send_to_char(buf,ch);
			}
}
void CProficiencies::CheckImprove(int pindex, int chance) //chance is out of 10000
{
	if(profs[pindex] >= 10 || profs[pindex] < 1)
		return;
	if(number_percent() > 10)
		return;
	if(number_percent() > 10)
		return;
	if(profs[pindex] > 4)
		if(number_percent() > (6 * profs[pindex]))
			return;
	if(number_percent() > chance)
		return;
	char buf[MSL];
	profs[pindex]++;
	sprintf(buf,"%sYou have %s %s.%s\n\r", get_char_color(ch, "green"), profs[pindex] == 10 ? "mastered" : "improved at",
		prof_table[pindex].name, END_COLOR(ch));
	send_to_char(buf,ch);
	gain_exp(ch, 250);
}
inline float CProficiencies::ProfEffect(int psn) { return ProfEffect(prof_table[psn].name); }

void CProficiencies::UpdateProfPoints(void)
{
	char buf[MSL];
	int i = 0;
	if(ch->played + (current_time - ch->logon) < pawardedtime + 3600)
		return;
	sprintf(buf,"%sYou feel ready to study new proficiencies.%s\n\r",
		get_char_color(ch,"yellow"), END_COLOR(ch));
	if(pawardedtime == 0)
		send_to_char("See HELP PROFICIENCY for more information.\n\r",ch);
	points += POINTS_PER_HOUR;
	send_to_char(buf,ch);
	pawardedtime = ch->played + (current_time - ch->logon);
}
void CProficiencies::DisplayProfsForStat(CHAR_DATA *imm)
{
	char buf[MSL], buf2[MSL];
	int i;
	sprintf(buf, "Proficiencies (%d pts left): ", GetPoints());
	for(i = 0; prof_table[i].name != NULL; i++)
	{
		if(profs[i] == -1)
			continue;
		sprintf(buf2,"%s (%d) ", prof_table[i].name, profs[i]);
		strcat(buf, buf2);
	}
	char *fbuf = palloc_string(buf);
	fbuf = format_string(fbuf);
	send_to_char(fbuf, imm);
	free_pstring(fbuf);
}
void CProficiencies::ShowProfsToChar()
{
	char buf[MSL];
	for(int i = 0; prof_table[i].name != NULL; i++)
		if(profs[i] > -1)
		{
			sprintf(buf,"You are %s in %s.\n\r", GetSkillLevelName(i), prof_table[i].name);
			send_to_char(buf, ch);
		}
}
const char* CProficiencies::GetSkillLevelName(int ind)
{
	int slevel = profs[ind];
	for(int i = MAX_PLEVEL - 1; i >= 0; i--)
		if(prof_level_table[i].level <= slevel)
			return prof_level_table[i].name;
	return prof_level_table[0].name;
}
void CProficiencies::WriteProfs(void *vfp)
{
	FILE *fp = (FILE *)vfp;
	fprintf(fp,"ProfPoints %d %ld\n", points, pawardedtime);
	for(int i = 0; prof_table[i].name != NULL; i++)
		if(this->profs[i] > -1)
			fprintf(fp, "Prof %s~ %d\n", prof_table[i].name, profs[i]);
}
void CProficiencies::AssignPsns()
{
	for(int i = 0; prof_table[i].name != NULL; i++)
		*(prof_table[i].ppsn) = i;
}
sh_int CProficiencies::ProfIndexLookup(const char *profname)
{
	for(int i = 0; prof_table[i].name != NULL; i++)
		if(!str_cmp(profname, prof_table[i].name))
			return i;
	return -1;
}
