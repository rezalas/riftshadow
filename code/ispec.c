// Calenduil was heeere! - note to self, possible memleak under returning without va_end
#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "spec.h"
#include "tables.h"
#include "interp.h"
va_list arglist;

DECLARE_SPEC(ispec_g_money);
DECLARE_SPEC(ispec_qwhip);
DECLARE_SPEC(ispec_torture_flail);
DECLARE_SPEC(ispec_toy);
	
BEGIN_ISPECS
	DEF_SPEC(ispec_toy, TRAP_IDOFUN | TRAP_IVERB)
	DEF_SPEC(ispec_g_money, TRAP_IWEAR | TRAP_IREMOVE)
	DEF_SPEC(ispec_qwhip, TRAP_IFIGHT | TRAP_IONEHIT)
	DEF_SPEC(ispec_torture_flail, TRAP_IFIGHT)
END_SPECS

BEGIN_SPEC(ispec_toy)
	EVENT_IDOFUN
	END_EVENT
	EVENT_IVERB
	END_EVENT
END_SPEC
	
BEGIN_SPEC(ispec_torture_flail)
	EVENT_IFIGHT
		if(number_percent()>91 && ch->fighting)
		{
			AFFECT_DATA af;
			init_affect(&af);
			act("A scream escapes your flail as a shadow tears at $N's eyes!",ch,0,ch->fighting,TO_CHAR);
			act("A scream escapes $n's flail as a shadow tears at $N's eyes!",ch,0,ch->fighting,TO_NOTVICT);
			act("A scream escapes $n's flail as a shadow tears at your eyes!",ch,0,ch->fighting,TO_VICT);
			act("$n appears to be blinded.",ch->fighting,0,0,TO_ROOM);
			af.type = gsn_blindness;
			af.aftype = AFT_MALADY;
			af.level = ch->level;
			af.duration = 2;
			SET_BIT(af.bitvector, AFF_BLIND);
			affect_to_char(ch->fighting, &af);
		}
	END_EVENT
END_SPEC

BEGIN_SPEC(ispec_g_money)
	EVENT_IWEAR
		act("J00 gonna wear me!? J00 GONNA WEAR ME?!?!?",ch,0,0,TO_CHAR);
		return 0;
	END_EVENT
	EVENT_IREMOVE
		act("You feel vary vary stoopid as you remove $p.",ch,obj,0,TO_CHAR);
		do_say(ch,"Duuuuuuuuh.");
	END_EVENT
END_SPEC

BEGIN_SPEC(ispec_qwhip)
	EVENT_IONEHIT
		if(number_percent()>96 && ch->fighting)
		{
			act("Sadistic urges compel you to lash out viciously at $N!",ch,0,ch->fighting,TO_CHAR);
			//(*dam) = (int)((float)*dam * 1.3);
			//(*dt)   = 42;
			//ch->hit = UMIN(ch->max_hit, ch->hit + ((*dam) / 4));
		}
	END_EVENT
	EVENT_IFIGHT
		if(number_percent()>97 && IS_SET(ch->fighting->parts,PART_LEGS))
		{
			CHAR_DATA *victim = ch->fighting;
			act("$n wraps $s whip around $N's legs, sending $M staggering!",ch,0,victim,TO_NOTVICT);
			act("You wrap your whip around $N's legs, sending $M staggering.",ch,0,victim,TO_CHAR);
			act("$n wraps $s whip around your legs, sending you staggering!",ch,0,victim,TO_VICT);
			WAIT_STATE(victim,PULSE_VIOLENCE+2);
		}
	END_EVENT
END_SPEC



//////////////////////////////////////////////////////////////////////
//////// ALL SPEC CODE IS ABOVE HERE - BELOW IS MAINTENANCE //////////
////////                      DO NOT TOUCH                  //////////
BEGIN_IEVENT_TYPES
	IEVENT("wear", 		TRAP_IWEAR)
	IEVENT("remove",	TRAP_IREMOVE)
	IEVENT("drop", 		TRAP_IDROP)
	IEVENT("sac",		TRAP_ISAC)
	IEVENT("give",		TRAP_IGIVE)
	IEVENT("greet",		TRAP_IGREET)
	IEVENT("fight",		TRAP_IFIGHT)
	IEVENT("death",		TRAP_IDEATH)
	IEVENT("speech",	TRAP_ISPEECH)
	IEVENT("pulse",		TRAP_IPULSE)
	IEVENT("verb",		TRAP_IVERB)
	IEVENT("one_hit",	TRAP_IONEHIT)
	IEVENT("get/loot",	TRAP_ILOOT)
	IEVENT("do_fun",	TRAP_IDOFUN)
END_EVENT_TYPES
