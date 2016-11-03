/* Eladrian's Kickin' Quest Tracking System -- ph34r */

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "magic.h"
#include "recycle.h"
#include "db.h"
#include "lookup.h"
#include "tables.h"
#include "interp.h"
#include "spec.h"

void talismanic_reward(CHAR_DATA *ch);
void mprog_say args((int inc, char *speech, CHAR_DATA *mob, CHAR_DATA *ch));
void mprog_emote(int inc, char *arg, CHAR_DATA *mob, CHAR_DATA *ch);
int mprog_drop(int inc, char *arg, OBJ_DATA *obj, CHAR_DATA *mob, CHAR_DATA *ch);
int mprog_give(int inc, char *arg, OBJ_DATA *obj, CHAR_DATA *mob, CHAR_DATA *ch);

const struct quest_type quest_table[MAX_QUESTS] =
{
	{ 0, "default",	"Sample quest (not used)",
		1,	20,
		{ 	"Stage one",
			"Stage two",
			"Stage three",
			"Stage four",
			"Stage five",
			"Stage six",
			"Stage seven",
			"Stage eight",
			"Stage nine"
		}
	},

	{ 1, "Cimar Doll", "Find the Cimarrite girl's lost doll.",	
		1,	51,
		{	"Agreed to help find the doll.",
			"Refused to help find the doll.",
			"Lied about having found the doll.",
			"Quest completed.",
		}
	},

	{ 2, "Talismanic Aura", "Seek out the hidden magic. (Sorcerers only)",
		35, 51,
		{	"Told of magics by guildmaster, told to seek out hermit.",
			"Refused to help the hermit.",
			"Agreed to help the hermit (gather reagents).",
			"Found reagent for hermit, now must retrieve stolen page.",
			"Attacked the hermit.",
			"Quest completed.",
		}
	},
	
	{ 3, "Drow Scribe", "Bring the drow scribe fruit.",
		25, 30,
		{	"Offered job by drow scribe.",
			"Accepted the job offer.",
			"Declined the job offer.",
			"Gave the Scribe fruit.",
			"Accepted second offer.",
			"Declined second offer.",
		}
	},

	{ 4, "Mud School", "Mud School.",
		1, 10,
		{
			"Heard first greeting.",
			"Heard guildmaster speech.",
			"Got paid 1000 gold.",
		}
	},

	{ 5, "Starving Pete", "Take the beef balls to pete!",
		1, 51,
		{
			"Pete sang for them.",
			"Opened bag.",
			"Gave Pete meat -- Quest Completed.",
		}
	},
	{ 6, "Academy Weaponsmith", "Get components for a better weapon.",
		1, 10,
		{
			"Weaponsmith greeted them.",
			"Weaponsmith sent them on quest for components.",
			"Weaponsmith is waiting for a weapon.",
			"All 4 components retrieved, quest completed.",
		}
	},
	{ 7, "Tarnished Ring", "Bring the tarnished ring to the scared soldier.",
		1, 10,
		{
			"Heard the introduction speech.",
		}
	}
};

void do_clearquests(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim;
	int i;
	
	if((victim = get_char_world(ch,argument)) == NULL)
		return send_to_char("They aren't here.\n\r",ch);
	if(get_trust(victim) > get_trust(ch))
		return send_to_char("You can't do that.\n\r",ch);
	for(i=0;i<MAX_QUESTS;i++) {
		if(victim->pcdata->quests[i]) {
			victim->pcdata->quests[i] = 0;
			continue;
		}
	}
return;
}


int agrees(CHAR_DATA *ch, char *speech)
{
	if(!str_prefix("yes",speech) || !str_prefix("yeah",speech) || !str_prefix("okay",speech) || !str_prefix("aye",speech)) {
		return REPLY_YES;
	} else if(!str_prefix("no",speech) || !str_prefix("nah", speech)) {
		return REPLY_NO;
	} else {
		return REPLY_NEITHER;
	}
return 0;
}

bool can_do_quest(CHAR_DATA *ch, int quest)
{
	if(get_trust(ch) > 55)
		return TRUE;

	if (((ch->level < quest_table[quest].minlevel) || (ch->level > quest_table[quest].maxlevel)))
		return FALSE;	
		
	if(IS_NPC(ch))
		return FALSE;

	return TRUE;
}
	
#define STAGE(ch)		ch->pcdata->quests[nQuestIndex]
#define SET_STAGE(ch, i) ch->pcdata->quests[nQuestIndex] = i
void store_quest_val(CHAR_DATA *ch, char *valname, sh_int value)
{
	AFFECT_DATA af;
	init_affect(&af);
	af.type		= gsn_timer;
	af.aftype	= AFT_INVIS;
	af.duration = -1;
	af.level 	= value;
	af.name		= palloc_string(valname);
	SET_BIT(af.bitvector, AFF_PERMANENT);
	affect_to_char(ch, &af);
}

void setbit_quest_val(CHAR_DATA *ch, char *valname, long value)
{
	AFFECT_DATA *paf;
	for(paf = ch->affected; paf; paf = paf->next)
		if(paf->type == gsn_timer && paf->name && !str_cmp(paf->name, valname))
		{
			SET_BIT_OLD(paf->level, value);
			break;
		}
	if(!paf)
		store_quest_val(ch, valname, (long)pow(2, value));
}

int get_quest_val(CHAR_DATA *ch, char *valname)
{
	AFFECT_DATA *paf;
	for(paf = ch->affected; paf; paf = paf->next)
		if(paf->type == gsn_timer && paf->name && !str_cmp(paf->name, valname))
			return paf->level;
	return -1;
}

void delete_quest_val(CHAR_DATA *ch, char *valname)
{
	AFFECT_DATA *paf, *paf_next = NULL;
	for(paf = ch->affected; paf; paf = paf_next)
	{
		paf_next = paf->next;
		if(paf->type == gsn_timer && paf->name && !str_cmp(paf->name, valname))
			affect_remove(ch, paf);
	}			
	return;
}
/* QUEST #1 -- Return the doll (2298) to the red-eyed girl (2481) in Cimar */

void greet_prog_cimar_dollgirl(CHAR_DATA *mob, CHAR_DATA *ch)
{
	char buf[MSL];

	if (IS_NPC(ch))
		return;

	if (!can_see(mob,ch))
		return;

	if (!can_do_quest(ch,DOLL_QUEST))
		return;
	
	switch (ch->pcdata->quests[DOLL_QUEST]) {
		case(0):
			act("$n approaches $N and asks $M something.",mob,0,ch,TO_NOTVICT);
			sprintf(buf,"A red eyed girl asks '%sPlease... will you help me find my doll?%s'\n\r",get_char_color(ch,"speech"),END_COLOR(ch));
			send_to_char(buf,ch);
			break;
		case(1):
			act("$n approaches $N and asks $M something.",mob,0,ch,TO_NOTVICT);
			sprintf(buf,"A red-eyed girl asks '%sD-did you find my d-doll?%s'\n\r",get_char_color(ch,"speech"),END_COLOR(ch));
			send_to_char(buf,ch);
			break;
		case(2):
		case(3):
			act("$n catches sight of you and her sobs intensify.",mob,0,ch,TO_VICT);
			act("$n catches sight of $N and her sobs intensify.",mob,0,ch,TO_NOTVICT);
			break;
		case(4):
			act("$n catches sight of you and gives you a smile and a wink.",mob,0,ch,TO_VICT);
			break;
	}

	return;
}

void speech_prog_cimar_dollgirl(CHAR_DATA *mob, CHAR_DATA *ch, char *speech)
{
	char buf[MSL];
	
	if (IS_NPC(ch))
		return;

	if (!can_see(mob,ch))
		return;

	if (!can_do_quest(ch,DOLL_QUEST))
		return;
	
	switch (ch->pcdata->quests[DOLL_QUEST]) {
		case(0):
			if (!str_prefix("yes",speech)) {
				do_emote(mob,"'s eyes light up and gleam with hope.");
				act("$n approaches $N and murmurs something to $M.",mob,0,ch,TO_NOTVICT);
				sprintf(buf,"A red-eyed girl says '%sI lost it somewhere on Wormwood Avenue... I saw a rat, and it tried to bite me, so I ran away... and I think I dropped my doll somewhere b-by the sewer g-grate....%s'\n\r",get_char_color(ch,"speech"),END_COLOR(ch));
				send_to_char(buf,ch);
				do_emote(mob,"begins to bawl once more.");
				ch->pcdata->quests[DOLL_QUEST] = 1;
				break;
			}
			if (!str_prefix("no",speech)) {
				act("A brief look of shock crosses $n's face and she turns away, sobbing.",mob,0,0,TO_ROOM);
				ch->pcdata->quests[DOLL_QUEST] = 2;
				ch->pcdata->reputation -= 10;
				break;
			}
			break;
		case(1):
			if (!str_prefix("yes",speech)) {
				if (is_carrying(ch,2298)) {
					do_emote(mob,"squeaks with delight and extends one wavering hand.");
					do_say(mob,"Please?");
				} else {
					act("$n stops crying suddenly and studies your face for a moment.",mob,0,ch,TO_VICT);
					act("$n stops crying suddenly and studies $N's face for a moment.",mob,0,ch,TO_NOTVICT);
					do_say(mob,"Why... would you l-lie to me?  W-why?");
					act("$n turns away from you and wails pitifully.",mob,0,ch,TO_VICT);
					act("$n turns away from $N and wails pitifully.",mob,0,ch,TO_NOTVICT);
					ch->pcdata->quests[DOLL_QUEST] = 3;
					ch->pcdata->reputation -= 15;
				}
				break;
			}
			if (!str_prefix("no",speech)) {
				do_emote(mob,"sniffs and continues crying loudly, wailing into a corner.");
				break;
			}
	}

	return;
}

void give_prog_cimar_dollgirl(CHAR_DATA *mob, CHAR_DATA *ch, OBJ_DATA *obj)
{
	if (obj->pIndexData->vnum != 2298)
		return;

	if (ch->pcdata->quests[DOLL_QUEST] != 1)
		return;

	if (!can_do_quest(ch,DOLL_QUEST))
		return;

	act("$N snatches the doll from your hands and examines it intently.",ch,0,mob,TO_CHAR);
	act("$N snatches the doll from $n's hands and examines it intently.",ch,0,mob,TO_ROOM);
	act("$N lets out a squeal of gleeful laughter and skips away, dangling the doll loosely by its leg.", ch, 0, mob, TO_ALL);

	extract_char(mob,TRUE);

	ch->pcdata->quests[DOLL_QUEST] = 4;
	ch->pcdata->reputation += 15;

	send_to_char("You receive experience for your deeds!\n\r",ch);

	gain_exp(ch,2000);

	return;
}

/* QUEST #2 -- Talismanic aura quest for sorcerers, at level 35+ */

void greet_prog_cimar_sorcgm(CHAR_DATA *mob, CHAR_DATA *ch)
{
	char buf[MSL];

	if (IS_NPC(ch))
		return;

	if (!can_see(mob,ch))
		return;

	if (str_cmp(ch->Class()->name,"sorcerer"))
		return;

	if (!can_do_quest(ch,TALISMANIC_QUEST))
		return;

	if (ch->pcdata->quests[TALISMANIC_QUEST] == 0) {
		sprintf(buf,"$n whispers '%sAh, %s.  I have been noting your progress with some interest.  I believe you are now ready to grasp a powerful new form of magic.  I have sworn an oath not to divulge its secrets, but I can tell you this: seek out a hermit in a valley north and to the west of this place.  He is in hiding, however, for there are many who would seek to learn his secrets.  Pelamon is his name.  Speak it that he may know you are a friend.  Do not shame me by proving unworthy in his eyes.%s'", get_char_color(ch,"red"), ch->name, END_COLOR(ch));
		act(buf,mob,0,ch,TO_VICT);
		ch->pcdata->quests[TALISMANIC_QUEST] =1;
	} else if (ch->pcdata->quests[TALISMANIC_QUEST] == 1 ) {
		sprintf(buf,"$n whispers '%sRemember.  A hermit north and to the west.  His name is Pelamon.%s'",get_char_color(ch,"red"),END_COLOR(ch));
		act(buf,mob,0,ch,TO_VICT);
	}

	return;
}

void speech_prog_ilopheth_shack(ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *speech)
{
	CHAR_DATA *rch, *mob;
	bool alone = TRUE;
	char buf[MSL];
	ROOM_INDEX_DATA *shack;

	mob = get_char_world(ch, "pelamon");

	shack = get_room_index(9148);
	
	if (IS_NPC(ch))
		return;

	if (!mob)
		return;

	if (ch->in_room != room)
		return;

	if (str_cmp(ch->Class()->name,"sorcerer"))
		return;

	if (!can_do_quest(ch,TALISMANIC_QUEST))
		return;
	
	if (!strstr(speech,"Pelamon") && !strstr(speech,"pelamon"))
		return;

	for (rch = room->people; rch; rch = rch->next_in_room) {
		if (!IS_NPC(rch) && (rch != ch))
			alone = FALSE;
	}

	switch(ch->pcdata->quests[TALISMANIC_QUEST]) {
		case(0):
			return;
		case(1):
			if (!alone) {
				sprintf(buf,"%s Who're those people with you?  If you want any dealings with me, get rid of them.",ch->name);
				do_tell(mob,buf);
			} else {
				char_from_room(mob);
				char_to_room(mob, room);
				act("A previously-unnoticed door on the shack to the west creaks loudly open.",ch,0,0,TO_ALL);
				act("A hermit clad in tattered robes grabs $n by the hand and drags $m in!",ch,0,0,TO_ROOM);
				act("A hermit clad in tattered robes grabs you by the hand and drags you in!",ch,0,0,TO_CHAR);
				char_from_room(mob);
				char_from_room(ch);
				char_to_room(mob,shack);
				char_to_room(ch,shack);
				do_look(ch,"auto");
				do_say(mob,"So he told you my name, did he?  You're here to learn, eh?  They always are.  Always coming, wanting what's mine.  Wanting, taking, with their grubby minds and dull hands, yes.");
				RS.Queue.AddToQueue(5, 2, do_say, mob, (char*)"Well you can't have it!");
				RS.Queue.AddToQueue(8, 2, do_emote,mob, (char*)"pauses for a moment and strokes his long,  ragged beard.");
				RS.Queue.AddToQueue(10, 2, do_say,mob,(char*)"Unless....  Yes,  yes... you can help me!  Ah,  my turn to grub!");
				RS.Queue.AddToQueue(13, 2, do_say,mob,(char*)"Can't go out there,  or they'd tear me to pieces,  yes!  Crack my skull open and snatch my brains and all their knowledge!  But I need something....");
				RS.Queue.AddToQueue(15, 2, do_say,mob, (char*)"Moss!  Not just any moss.  Special moss.  White moss.  Glows,  it does.");
				RS.Queue.AddToQueue(17, 2, do_say,mob,(char*)"So,  you'll get it for me,  yes?");
				RS.Queue.AddToQueue(18, 2, do_emote, mob, (char*)"peers at you expectantly.");
			}
			break;
		case(2):
			return;
		case(3):
			if (!alone) {
				sprintf(buf,"%s Who're those people with you?  If you want any dealings with me, get rid of them.",ch->name);
				do_tell(mob,buf);
			} else {
				if (!is_carrying(ch,230)) {
					sprintf(buf,"%s Hmm... moss?  You don't have my moss!  Don't come back until you do, grubber!",ch->name);
					do_tell(mob,buf);
				} else {
					char_from_room(mob);
					char_to_room(mob, room);
					act("A previously-unnoticed door on the shack to the west creaks loudly open.",ch,0,0,TO_ALL);
					act("A hermit clad in tattered robes grabs $n by the hand and drags $m in!",ch,0,0,TO_ROOM);
					act("A hermit clad in tattered robes grabs you by the hand and drags you in!",ch,0,0,TO_CHAR);
					char_from_room(mob);
					char_from_room(ch);
					char_to_room(mob,shack);
					char_to_room(ch,shack);
					do_look(ch,"auto");
					do_say(mob,"Well... let's see it!");
					do_emote(mob,"peers at you expectantly.");
				}
			}
			break;
		case(4):
			if (!alone) {
				sprintf(buf,"%s Who're those people with you?  If you want any dealings with me, get rid of them.",ch->name);
				do_tell(mob,buf);
			} else {
				if (!is_carrying(ch,1709)) {
					sprintf(buf,"%s I can't do a thing for you unless you find that blasted page!  And if you find it, you best come back here right away before some grubber gets it, yes!",ch->name);
					do_tell(mob,buf);
				} else {
					char_from_room(mob);
					char_to_room(mob, room);
					act("A previously-unnoticed door on the shack to the west creaks loudly open.",ch,0,0,TO_ALL);
					act("A hermit clad in tattered robes grabs $n by the hand and drags $m in!",ch,0,0,TO_ROOM);
					act("A hermit clad in tattered robes grabs you by the hand and drags you in!",ch,0,0,TO_CHAR);
					char_from_room(mob);
					char_from_room(ch);
					char_to_room(mob,shack);
					char_to_room(ch,shack);
					do_look(ch,"auto");
					do_say(mob,"You found it!  In the hands of some grubber no doubt!  Let me see!");
					do_emote(mob,"peers at you eagerly, a gleam in his eyes.");
				}
			}
			break;
		case(5):
			act("A bolt of lightning streaks down from the clouds above!",ch,0,0,TO_ALL);
			do_myell(ch,"Argh!  I've been struck by lightning!", NULL);
			damage_new(mob,ch,dice(ch->level,8),gsn_call_lightning,DAM_LIGHTNING,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"The lightning strike*");
			sprintf(buf,"%s Get the hell away from here, and don't you ever come back, you dirty grubber!",ch->name);
			do_tell(mob,buf);
			break;
		case(6):
			sprintf(buf,"%s Nope, you're not getting another thing out of me, grubber!",ch->name);
			do_tell(mob,buf);
			break;
		default:
			return;
	}

	return;
}

void speech_prog_ilopheth_hermit(CHAR_DATA *mob, CHAR_DATA *ch, char *speech)
{
	ROOM_INDEX_DATA *outside;
	bool found = FALSE;
	
	if (IS_NPC(ch))
		return;

	if (ch->pcdata->quests[TALISMANIC_QUEST] != 1)
		return;

	outside = get_room_index(9147);

	if (!str_prefix("yes", speech)) {
		do_say(mob,"Good!  Good!  Moss!  This special moss... it grows only in very cold places, and at high altitude.  There is a mountain to the south where it used to grow... mayhaps it still does!  Mayhaps, yes?  Now, go!");
		do_emote(mob,"opens the door and hustles you outside.");
		ch->pcdata->quests[TALISMANIC_QUEST] = 3;
		found = TRUE;
	} else if (!str_prefix("no", speech)) {
		do_emote(mob,"'s eyes flash suddenly with anger, and he straightens to his full height, towering over you.");
		do_say(mob,"Ungrateful grubber!  Abusing my hospitality!  I bet you're one of them, aren't you?  Aren't you?!");
		do_say(mob,"OUT!!");
		do_emote(mob,"waves a hand and the door behind you opens.  He casts you out violently!");
		ch->pcdata->quests[TALISMANIC_QUEST] = 2;
		found = TRUE;
	}

	if (found) {
		char_from_room(ch);
		char_to_room(ch,outside);
		do_look(ch,"auto");
	}

	return;
}

void give_prog_ilopheth_hermit(CHAR_DATA *mob, CHAR_DATA *ch, OBJ_DATA *obj)
{
	bool found = FALSE;
	ROOM_INDEX_DATA *outside = get_room_index(9147);

	if (IS_NPC(ch))
		return;
	
	if (obj->pIndexData->vnum != 230 && obj->pIndexData->vnum != 1709)
		return;

	if (obj->pIndexData->vnum == 230 &&
		ch->pcdata->quests[TALISMANIC_QUEST] == 3) {
		obj_from_char(obj);
		extract_obj(obj);
		/* moss */
		do_emote(mob,"'s eyes beam as he gingerly handles the white moss in his hands.");
		RS.Queue.AddToQueue(3, 2, do_say,mob,(char*)"This is it!  You've found it!  Oh yes, yes!  A fine specimen indeed!  Not bad for some grubber, no,  no,  not bad at all!");
		RS.Queue.AddToQueue(7, 2, do_emote, mob, (char*)"pauses in his examination of the moss and peers at you.");
		RS.Queue.AddToQueue(10, 2, do_say,mob,(char*)"Oh yes!  You want me to help you.  With magic, yes?  Knowledge...  powerful magic,  perhaps you can handle it.  Perhaps... perhaps not!  Perhaps your head will crack like a delicate little egg,  hee hee!");
		RS.Queue.AddToQueue(15, 2, do_emote,mob, (char*)"goes over to a towering stack of books and scraps in a far corner of the shack,  and begins to rummage.");
		RS.Queue.AddToQueue(18, 2, do_say,mob,(char*)"Mysteries... no... not this one.  Arcane... no.  Curses of Ba... hrm, hee hee,  no not that one either.  Ah,  here we go!  Secrets of the Talismanic Powers.");
		RS.Queue.AddToQueue(23, 2, do_emote,mob, (char*)"slowly straightens and totters toward you,  cradling the tome in his hands.");
		RS.Queue.AddToQueue(27, 2, do_say, mob, (char*)"Here it is... right on page... what the... grubbers!  The page.. it's stolen!  Gone!  Vanished into thin air!  You stole it!  I'll kill you!");
		RS.Queue.AddToQueue(31, 2, do_say,mob,(char*)"Oh, wait.  No, you didn't.  Yes,  yes,  I know.  Those grubbers must've got it!");
		RS.Queue.AddToQueue(34, 2, do_emote, mob, (char*)"waves his arms around wildly.  VERY wildly.  He must be seriously perturbed.");
		RS.Queue.AddToQueue(37, 2, do_say,mob,(char*)"Grubbers!  Well,  a secret I'll tell you.  Yes,  a secret!  I didn't always used to live here!  No no!");
		RS.Queue.AddToQueue(42, 2, do_say,mob, (char*)"I lived deep in a forest bit east of that big city.  Silmur or something.  That city.  Anyway... nice trees.  Very nice.  Very quiet,  too.");
		RS.Queue.AddToQueue(48, 2, do_say,mob,(char*)"Well, except for the bandits.  Bastardly grubbers, I tell you.  Woke up one morning, and they were in my shack, goin' right through my things!  Trying to get my secrets!  Oh yes,  the precious secrets,  hee hee!");
		RS.Queue.AddToQueue(53, 2, do_say,mob,(char*)"So I moved.  Here.  To this wretched place.  But my secrets are safe here!  Oh yes, yes,  hee hee!  Safe until you came in here,  you grubber!  And stole 'em!");
		RS.Queue.AddToQueue(58, 2, do_say,mob,(char*)"Oh, wait.  No, you didn't.  I said I'd give it to you.  But yes yes, you need to find that page!  Bring it back here,  and I'll give you what you want,  you grubber!");
		RS.Queue.AddToQueue(70, 2, do_say,mob,(char*)"Are you still here?  Go on,  and find that page!  Now out you go,  hee hee!");
		RS.Queue.AddToQueue(71, 2, do_look, ch, (char*)"auto");
		RS.Queue.AddToQueue(71, 2, char_to_room, ch, outside);
		RS.Queue.AddToQueue(71, 1, char_from_room, ch);
		RS.Queue.AddToQueue(71, 2, do_emote, mob, (char*)"hustles you rapidly out of his shack.");
		ch->pcdata->quests[TALISMANIC_QUEST] = 4;
		return;

	}

	if (obj->pIndexData->vnum == 1709 &&
		ch->pcdata->quests[TALISMANIC_QUEST] == 4) {
		/* page */
		do_emote(mob,"scrunches his haggard face up at he peers intently at the page.");
		RS.Queue.AddToQueue(3, 2, do_say,mob, (char*)"This is the missing page!  So you're the one who stole it!  You dirty grubber,  you'll pay for this!");
		RS.Queue.AddToQueue(8, 2, do_say,mob,(char*)"Oh, wait.  No you didn't.  Now I can show you what I've been meaning to all this time,  hee hee.  I think you'll like it,  oh yes you will.");
		RS.Queue.AddToQueue(13, 2, do_emote,mob, (char*)"inserts the page into the book,  confirming the perfect fit with a glance.");
		RS.Queue.AddToQueue(16, 2, do_emote, mob, (char*)"totters over to your side and begins to murmur softly.");
		RS.Queue.AddToQueue(20, 2, do_emote,mob, (char*)"speaks with remarkable lucidity for one so seemingly deranged,  guiding you through an explanation of the arcane tome.");
		RS.Queue.AddToQueue(25, 2, send_to_char,(char*)"You suddenly have a flash of insight,  and understand precisely what Pelamon is attempting to teach.  Amazing... a protective spell of remarkable power!\n\r", ch);
		RS.Queue.AddToQueue(36, 1, talismanic_reward, ch);
		RS.Queue.AddToQueue(30, 2, do_say,mob,(char*)"Ah, I see you've got it,  yes yes!  And no cracks in your head,  hee hee!  Maybe you're not such a grubber after all.");
		RS.Queue.AddToQueue(34, 2, do_say,mob,(char*)"Well,  you've taken enough of my time.  You'll have to be on your way now... I'm expecting company,  hee hee!");
		RS.Queue.AddToQueue(37, 2, do_emote, mob, (char*)"winks mischievously.");
		RS.Queue.AddToQueue(40, 2, do_look, ch, (char*)"auto");
		RS.Queue.AddToQueue(40, 2, char_to_room, ch, outside);
		RS.Queue.AddToQueue(40, 1, char_from_room, ch);
		RS.Queue.AddToQueue(40, 2, do_emote, mob, (char*)"hustles you rapidly out of his shack.");
		RS.Queue.AddToQueue(41, 1, talismanic_reward, ch);
	}

	if (found) {
		char_from_room(ch);
		char_to_room(ch,outside);
		do_look(ch,"auto");
	}

	return;
}

void talismanic_reward(CHAR_DATA *ch)
{
	if (ch->in_room->vnum == 9148) {
		ch->pcdata->quests[TALISMANIC_QUEST] = 6;
		ch->pcdata->learned[skill_lookup("talismanic aura")] = 70;
	} else {
		send_to_char("You pause briefly to reflect on all you have seen and learned in your dealings with the hermit.\n\r",ch);
		WAIT_STATE(ch,PULSE_VIOLENCE * 3);
		send_to_char("You feel more experienced!\n\r",ch);
		gain_exp(ch,4500);
	}

	return;
}

void pulse_prog_talismanic_page(OBJ_DATA *obj, bool isTick)
{
	OBJ_DATA *scrap;
	CHAR_DATA *ch;
	
	if (!obj->carried_by)
		return;

	ch = obj->carried_by;

	if (IS_NPC(ch))
		return;

	if (ch->pcdata->quests[TALISMANIC_QUEST] == 4)
		return;

	obj_from_char(obj);
	extract_obj(obj);

	scrap = create_object(get_obj_index(1708),0);

	obj_to_char(scrap, ch);

	return;
}

void pulse_prog_talismanic_scrap(OBJ_DATA *obj, bool isTick)
{
	OBJ_DATA *page;
	CHAR_DATA *ch;
	
	if (!obj->carried_by)
		return;

	ch = obj->carried_by;

	if (IS_NPC(ch))
		return;

	if (ch->pcdata->quests[TALISMANIC_QUEST] != 4)
		return;

	if (obj->pIndexData->vnum != 1708)
		return;

	obj_from_char(obj);
	extract_obj(obj);

	page = create_object(get_obj_index(1709),0);

	obj_to_char(page, ch);

	send_to_char("It suddenly occurs to you that the scrap of parchment in your possession is the missing page the hermit is looking for!\n\r",ch);

	return;
}

bool aggress_prog_ilopheth_hermit(CHAR_DATA *mob, CHAR_DATA *attacker)
{
	ROOM_INDEX_DATA *outside = get_room_index(9147);

	do_emote(mob,"'s suddenly twists with rage, and he stands to a towering height.");
	do_say(mob,"You... you lying sneaky little grubber!  I kill you!  Out!  Out!  OUT!!");

	do_emote(mob,"suddenly unleashes a blast of raw energy, sending you flying!");
	damage_new(mob,attacker,dice(20,10),gsn_bash,DAM_ENERGY,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"energy wave");

	attacker->pcdata->quests[TALISMANIC_QUEST] = 5;

	char_from_room(attacker);
	char_to_room(attacker,outside);
	do_look(attacker,"auto");

	return TRUE;
}

void pulse_prog_ilopheth_hermit(CHAR_DATA *mob)
{
	DESCRIPTOR_DATA *d;
	char buf[MSL];

	if (mob->in_room->area->nplayer == 0)
		return;

	for (d = descriptor_list; d; d = d->next) {
		if (d->connected == CON_PLAYING 
			&& !IS_NPC(d->character)
			&& d->character->in_room != NULL 
			&& d->character->in_room->area != NULL
			&& d->character->in_room->area == mob->in_room->area
			&& d->character->pcdata->quests[TALISMANIC_QUEST] == 5
			&& number_percent() < 5) {
			sprintf(buf,"%s You!  Coming again to grub, eh?  You'll pay, oh yes yes, you will!",d->character->name);
			do_tell(mob,buf);
			act("A bolt of lightning streaks down from the clouds above!",d->character,0,0,TO_ALL);
			do_myell(d->character,"Argh!  I've been struck by lightning!",NULL);
			damage_new(mob,d->character,dice(d->character->level,8),gsn_call_lightning,DAM_LIGHTNING,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"The lightning strike*");
		}
	}

	return;
}

/* QUEST 3 - Bring the drow scribe (23811) fruit from island in the Crimson Sea */

void greet_prog_drow_scribe(CHAR_DATA *mob, CHAR_DATA *ch)
{
	char buf[MSL];

	if(IS_NPC(ch))
		return;

	if (!can_do_quest(ch,SCRIBE_QUEST))
		return;
	
	if(ch->pcdata->quests[SCRIBE_QUEST] == 0) {
		act("The drow scribe looks up at you with a look of contemplation on his face.",ch,0,0,TO_CHAR);
		act("The drow scribe looks up at $n with a look of contemplation on his face.",ch,0,0,TO_ROOM);
		do_say(mob,"You there... You look like you could use a bit of extra gold.  Am I right?");
		ch->pcdata->quests[SCRIBE_QUEST] = 1;
	} else if(ch->pcdata->quests[SCRIBE_QUEST] == 1) {
		act("The drow scribe snorts at you as you enter.",ch,0,0,TO_CHAR);
		act("The drow scribe snorts at $n as $e enters.",ch,0,0,TO_ROOM);
		sprintf(buf,"Well, %s?  Do you want the job or not?",ch->name);
		do_say(mob,buf);		
	} else if(ch->pcdata->quests[SCRIBE_QUEST] == 2) {
		act("The drow scribe looks up from his work quickly.",ch,0,0,TO_ROOM);
		sprintf(buf,"Hand me the fruit, %s.",ch->name);
		do_say(mob,buf);
	}

	return;
}

void speech_prog_drow_scribe(CHAR_DATA *mob, CHAR_DATA *ch, char *speech)
{
	if (IS_NPC(ch))
		return;
	
	if (!can_do_quest(ch,SCRIBE_QUEST))
		return;
	
	if(ch->pcdata->quests[SCRIBE_QUEST] == 1 && agrees(ch,speech) == REPLY_YES) {
		do_say(mob,"Excellent. I am in need of a certain fruit to mix into my ink to add to its potency. The fruit is about the size of a human's fist with white skin and a purple pulp.  It can be found on the island in the midst of the Crimson Sea. Bring it back to me. Go now.");
		ch->pcdata->quests[SCRIBE_QUEST] = 2;
	} else if(ch->pcdata->quests[SCRIBE_QUEST] == 1 && agrees(ch,speech) == REPLY_NO) {
		do_emote(mob,"shrugs and returns to his work.");
		ch->pcdata->quests[SCRIBE_QUEST] = 3;
	}

	return;
}	

void give_prog_drow_scribe(CHAR_DATA *mob, CHAR_DATA *ch, OBJ_DATA *obj)
{
	int gold = number_range(7,10);
	char buf[MSL];

	if(!can_do_quest(ch,SCRIBE_QUEST))
		return;
	
	if(ch->pcdata->quests[SCRIBE_QUEST] != 2)
		return;

	if(obj->pIndexData->vnum != 14222) {
		do_say(mob,"Bah!  This is not what I want!");
		act("$n roughly throws $p to the ground.",mob,obj,0,TO_ROOM);
		obj_from_char(obj);
		obj_to_room(obj,mob->in_room);
		return;
		}

	if(obj->pIndexData->vnum == 14222 && ch->pcdata->quests[SCRIBE_QUEST] == 2) {
		do_say(mob,"Ah, yes.  Here you go.");
		ch->gold += gold;
		sprintf(buf,"The drow scribe gives you %d gold.\n\r",gold);
		send_to_char(buf,ch);		
		do_emote(mob,"returns to his work.");
		RS.Queue.AddToQueue(2, 2, do_say,mob, (char*)"Well,  while you're here...");
		RS.Queue.AddToQueue(4, 2, do_say,mob, (char*)"I have a scroll made up for a master mage on the fourth floor... Deliver it to him,  and he'll surely tip you.");
		RS.Queue.AddToQueue(6, 2, do_say, mob, (char*)"I offer this to you only because you didn't seem very pleased with the tip that I gave you.");
		RS.Queue.AddToQueue(8, 2, do_say, mob, (char*)"Do you accept?");
		ch->pcdata->quests[SCRIBE_QUEST] = 4;
		return;
	}
return;
}

void greet_prog_opening_greet(CHAR_DATA *mob, CHAR_DATA *ch)
{
	if(!can_do_quest(ch,MUD_SCHOOL))
		return;

	if(ch->pcdata->quests[MUD_SCHOOL])
		return;

	mprog_say(1,"Hello there!  Nothing like a rough landing to brighten your day.",mob,ch);
	do_emote(mob,"winks.");
	ch->pcdata->quests[MUD_SCHOOL] = 1;
}

void greet_prog_headmaster(CHAR_DATA *mob, CHAR_DATA *ch)
{
	if(!can_do_quest(ch,MUD_SCHOOL))
		return;

	if(ch->pcdata->quests[MUD_SCHOOL] > 1)
		return;

	act("$N nods solemnly to you.",ch,0,mob,TO_CHAR);
	mprog_tell(1,"Welcome to the guildhall of the Shalaran Academy.  Here, my guildmasters and I will train you in the fine arts of combat.  Type 'practice' and 'train' here to prepare yourself before moving on.",mob,ch);
	ch->pcdata->quests[MUD_SCHOOL] = 2;
	return;
}

void greet_prog_pay_char(CHAR_DATA *mob, CHAR_DATA *ch)
{
	if(!can_do_quest(ch,MUD_SCHOOL))
		return;

	if(ch->pcdata->quests[MUD_SCHOOL] > 2)
		return;

	mprog_tell(0,"I highly recommend you visit Cimar's shops and stock up on items you might need.  The kiosk and the general store are very important.",mob,ch);
	act("$N gives you 500 gold.",ch,0,mob,TO_CHAR);
	ch->gold += 500;
	mprog_tell(3,"I won't expect change... Cimar's prices have been rising lately.  But don't expect any more handouts.  The city is out the exit at the south end of the Academy.",mob,ch);
	ch->pcdata->quests[MUD_SCHOOL] = 3;
	return;
}

void greet_prog_starvin_pete(CHAR_DATA *mob, CHAR_DATA *ch)
{
	if(!can_do_quest(ch,PETE_QUEST))
		return;

	switch(ch->pcdata->quests[PETE_QUEST]) {
		case(0):
			do_sing(mob,"Boiled meat, bitter meat/Garbage, trash, or litter meat/Sour, burnt, or livin' meat/Don't care what you're givin' Pete;/Pete'll eat it -- Pete likes meat./What you got for Starvin' Pete?");
			ch->pcdata->quests[PETE_QUEST] = 1;
			break;

		case(1):
			mprog_say(0,"You got meat for Pete?",mob,ch);
			break;

		case(2):
			mprog_say(0,"Bag!  Must have bag!",mob,ch);
			break;		
		case(3):
			act("$N winks at you.",ch,0,mob,TO_CHAR);
			act("$N winks at $n.",ch,0,mob,TO_ROOM);
			break;
		}
return;
}

void give_prog_starvin_pete(CHAR_DATA *mob, CHAR_DATA *ch, OBJ_DATA *obj)
{
	char buf[MSL];
	AFFECT_DATA af, *paf;

	if(!can_do_quest(ch,PETE_QUEST))
		return;
	
	switch(ch->pcdata->quests[PETE_QUEST]) {
		case(1):
			if(!str_cmp(material_table[obj->pIndexData->material_index].mat_name,"meat")) {
				if(is_affected(mob,gsn_bash) && 
				((paf = affect_find(mob->affected,gsn_bash))!= NULL && 
				(ch == paf->owner))) {
					act("$n gobbles down $p noisily.",mob,obj,ch,TO_ALL);
					do_say(mob,"Pete got no more money for you.");
					do_emote(mob,"shrugs.");
					obj_from_char(obj);
					extract_obj(obj);
					}
				act("$n sings joyfully at the sight of meat!",mob,0,ch,TO_ALL);
				act("$N gratefully hands you 10 gold coins.",ch,0,mob,TO_CHAR);
				act("$N hands $n some coins.",ch,0,mob,TO_ROOM);
				ch->gold += 10;
				gain_exp(ch, 50);
				act("$N gobbles down $p noisily.",ch,obj,mob,TO_ALL);
				obj_from_char(obj);
				extract_obj(obj);
				init_affect(&af);
				af.where = TO_AFFECTS;
				af.aftype = AFT_INVIS;
				af.type = gsn_bash;
				af.duration = 2;
				af.owner = ch;
				
			} else {
				act("$N gobbles down $p noisily.",ch,obj,mob,TO_ALL);
				obj_from_char(obj);
				extract_obj(obj);
			}

			break;
		case(2):
			if(obj->pIndexData->vnum == 2416) {
				act("$N dances wildly before you!",ch,0,mob,TO_ALL);
				sprintf(buf,"$N whispers '%sVery near here... plate in the ground, treasure under the plate... Pete's strange friend tell Pete... but dangerous down there, bring safety!  Oh... also, need rope.%s'",get_char_color(ch,"red"),END_COLOR(ch));
				act(buf,ch,0,mob,TO_CHAR);
				ch->pcdata->quests[PETE_QUEST] = 3;
			} else {
				act("$N drops $p.",ch,obj,mob,TO_ALL);
				obj_from_char(obj);
				obj_to_room(obj,ch->in_room);
				mprog_say(1,"Bag!  Must have bag!",mob,ch);
				}
			break;
		case(3):
                        if(!str_cmp(material_table[obj->pIndexData->material_index].mat_name,"meat")) {
                                act("$n sings joyfully at the sight of meat!",mob,0,ch,TO_ALL);
                                act("$N gratefully hands you 10 gold coins.",ch,0,mob,TO_CHAR);
                                act("$N hands $n some coins.",ch,0,mob,TO_ROOM);
                                ch->gold += 10;
                                act("$N gobbles down $p noisily.",ch,obj,mob,TO_ALL);
                                obj_from_char(obj);
                                extract_obj(obj);
                        } else {
                                act("$N gobbles down $p noisily.",ch,obj,mob,TO_ALL);
                                obj_from_char(obj);
                                extract_obj(obj);
                        }
                
                        break;
	}
return;
}

/* academy blacksmith quest - bring him components for a better weapon -cal */
#define SMITH_QUEST_LASTCOMP "Smith Quest Last Component"
#define SMITH_QUEST_COMP_ACQUIRED "Smith Quest Components Used"
BEGIN_SPEC(mspec_academy_smith)
	const int nQuestIndex = SMITH_QUEST, MIN_COMPONENT = 24500, MAX_COMPONENT = 24503;
	int improved = 0;
	EVENT_MGREET
		if(!can_do_quest(ch, SMITH_QUEST) || STAGE(ch) != 0)
			return 0;
		act("$N looks up from his forge, sweat dripping down his brow.",ch,0,mob,TO_CHAR);
		if(IS_GOOD(ch))
			act("$N smiles at you broadly.",ch,0,mob,TO_CHAR);
		else
			act("$N regards you carefully, sizing you up.",ch,0,mob,TO_CHAR);
		mprog_say(2, "You there.  Yer look like yer in the market for something more powerful, eh?", mob, ch);
		SET_STAGE(ch, 1);
	END_EVENT
	EVENT_MSPEECH
		if(!can_do_quest(ch, SMITH_QUEST) || STAGE(ch) != 1)
			return 0;
		if(agrees(ch, argument) != REPLY_YES)
			return 0;
		mprog_say(2, "Good!  I'm in short bloody supply of materials to work with these days.", mob, ch);
		mprog_say(4, "If yer can find me some, I'll keep half for myself and use the other half to strengthen yer weapon.", mob, ch);;
		mprog_say(6, "I need four raw metals.  Iandia said there might be some secreted in the Outlying Wilds and thereabouts.", mob, ch);
		RS.Queue.AddToQueue(8, 5, act, "$N returns to his forge, turning his back to you.", ch, 0, mob, TO_CHAR);
		SET_STAGE(ch, 2);
	END_EVENT
	EVENT_MGIVE
		OBJ_AFFECT_DATA *paf;
		if(!can_do_quest(ch, SMITH_QUEST))
			return 0;
		if(STAGE(ch) == 2)
		{
			AFFECT_DATA af;
			int ival;
			WAIT_STATE(ch, PULSE_VIOLENCE * 2);
			act("$N examines $p closely, looking for flaws or weaknesses.", ch, obj, mob, TO_CHAR);
			if(obj->pIndexData->vnum < MIN_COMPONENT || obj->pIndexData->vnum > MAX_COMPONENT)
				return mprog_drop(1, "What am I supposed to do with this?!", obj, mob, ch);
			if((ival = get_quest_val(ch, SMITH_QUEST_COMP_ACQUIRED)) > -1 && IS_SET_OLD(ival, obj->pIndexData->vnum - MIN_COMPONENT))
				return mprog_drop(1,"I already used this on yer damn weapon, what more do you want?", obj, mob, ch);
			delete_quest_val(ch, SMITH_QUEST_LASTCOMP); //if they've already got one
			store_quest_val(ch, SMITH_QUEST_LASTCOMP, obj->pIndexData->vnum);
			mprog_say(5, "Yah, this'll do.", mob, ch);
			mprog_say(7, "Give me yer academy weapon and I'll see what I can do with it.", mob, ch);
			SET_STAGE(ch, 3);
			extract_obj(obj);
			return 0;
		}
		else if(STAGE(ch) == 3)
		{
			int comp_vnum, done_comps;
			if(obj->pIndexData->vnum < OBJ_VNUM_SCHOOL_FLAIL || obj->pIndexData->vnum > OBJ_VNUM_SCHOOL_SWORD)
				return mprog_drop(1, "I can't work with this slop!", obj, mob, ch);
			if((comp_vnum = get_quest_val(ch, SMITH_QUEST_LASTCOMP)) == -1)
				{ bug("Unable to find component vnum on player for Blacksmith quest.",0); return 0; }
			delete_quest_val(ch, SMITH_QUEST_LASTCOMP);
			WAIT_STATE(ch, PULSE_VIOLENCE * 2);
			if((done_comps = get_quest_val(ch, SMITH_QUEST_COMP_ACQUIRED)) == -1)
			{
				setbit_quest_val(ch, SMITH_QUEST_COMP_ACQUIRED, (comp_vnum - MIN_COMPONENT));
				//obj->value[1] == 3; obj->value[2] = 4;i
				obj->value[1] = 15;
				obj->value[2] = 1;
			}
			else
			{
/*				if(obj->value[1] == 3 && obj->value[2] == 5)
				{	obj->value[1] = 4; obj->value[2] = 4; improved = 3;}
				else if(obj->value[1] == 4 && obj->value[2] == 3)
				{	obj->value[1] = 3; obj->value[2] = 5; improved = 2;}
				else if(obj->value[1] == 3 && obj->value[2] == 4)
				{	obj->value[1] = 4; obj->value[2] = 3; improved = 1;}*/
				obj->value[1] = UMAX(obj->value[1] + 1, 18);
			}
			char buf[MSL], *tptr;
			sprintf(buf,"%s",obj->short_descr);
			tptr = talloc_string(buf);
			RS.Queue.AddToQueue(1, 5, act, "Accepting $t, $N turns to the forge, preparing his tools.",ch,tptr,mob,TO_CHAR);
			RS.Queue.AddToQueue(3, 5, act, "$N begins to reshape $t, the hammering ringing loud in your ears.",ch,tptr,mob,TO_CHAR);
			RS.Queue.AddToQueue(7, 5, act, "Hefting it with a flourish, $N turns to you.",ch,obj,mob,TO_CHAR);
			if (improved == 0)
				mprog_say(8, "Har!  This piece o' duergar dung might actually be worth using now.", mob, ch);
			else
				mprog_say(8, "Har!  Yer lucky to have the benefits of me skilled hands!", mob, ch);
			REMOVE_BIT(obj->extra_flags, ITEM_MELT_DROP);
			if(get_quest_val(ch, SMITH_QUEST_COMP_ACQUIRED) >= 15)
				sprintf(buf, "a fully strengthened Academy %s", weapon_name(obj->value[0]));
			else
				sprintf(buf, "a reinforced Academy %s", weapon_name(obj->value[0]));
			free_pstring(obj->short_descr);
			obj->short_descr = palloc_string(buf);
			setbit_quest_val(ch, SMITH_QUEST_COMP_ACQUIRED, (comp_vnum - MIN_COMPONENT));
			mprog_give(10, NULL, obj, mob, ch);
			SET_STAGE(ch, 2);
			if(get_quest_val(ch, SMITH_QUEST_COMP_ACQUIRED) >= 15) //all done
			{
				SET_STAGE(ch, 4);
				RS.Queue.AddToQueue(10, 2, gain_exp, ch, 3000);
				RS.Queue.AddToQueue(10, 2, send_to_char, "Handling your newly completed weapon, you feel ready to take on the world!\n\r",ch);
				mprog_say(10, "That's about all I can do with the thing.  Good luck, ye flamin' elfhugger.", mob, ch);
				delete_quest_val(ch, SMITH_QUEST_COMP_ACQUIRED);
			}
			else
				mprog_say(10, "I can probably work with it more if ye find me more decent metals.", mob, ch);
		}
	END_EVENT
END_SPEC

BEGIN_SPEC(mspec_scared_soldier)
	const int nQuestIndex = RING_QUEST;
	EVENT_MGREET
		if(!can_do_quest(ch,RING_QUEST))
			return 0;
		if(STAGE(ch) == 0) {
			mprog_emote(1, "looks up at you as you enter.",mob,ch);
			mprog_say(2,"Please... help me.",mob,ch);
			mprog_say(3,"My father's ring has been stolen by a mutant.",mob,ch);
			mprog_emote(4,"breaks down sobbing.",mob,ch);
			mprog_say(5,"He lives under the ground in the overrun area.  You need a rope to get into his hiding place.",mob,ch);
			mprog_emote(6,"curls up into a tighter ball.",mob,ch);
			SET_STAGE(ch,1);
			return 0;
		} else {
			mprog_emote(1,"whimpers softly.",mob,ch);
			return 0;					
		}
	END_EVENT
END_SPEC
