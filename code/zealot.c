#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "merc.h"
#include "interp.h"
#include "tables.h"
#include "recycle.h"
#include "magic.h"
#include "db.h"
#include "lookup.h"

void spell_infidels_weight( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	int mod;

	if(is_affected(victim, gsn_infidels_weight))
		return send_to_char("They are already burdened by the weight of infidels!\n\r", ch);

	if( saves_spell( level, victim, DAM_OTHER))
		mod = level * 4;
	else
		mod = level * 8;

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_COMMUNE;
	af.type = sn;
	af.level = level;
	af.duration = level/4;
	af.location = APPLY_CARRY_WEIGHT;
	af.modifier = mod;
	af.owner = ch;
	new_affect_to_char( victim, &af );

	act("You burden $N with the weight of a thousand infidels!", ch, 0, victim, TO_CHAR);
	act("$n burdens you with the weight of a thousand infidels!", ch, 0, victim, TO_VICT);
	act("$n burdens $N with the weight of a thousand infidels!", ch, 0, victim, TO_NOTVICT);

	return;
}

int get_bv_stage( CHAR_DATA *ch )
{
	AFFECT_DATA *af;

	if(!is_affected(ch, gsn_burning_vision) /*|| IS_IMMORTAL(ch)*/)
		return -1;

	for (af = ch->affected; af != NULL; af = af->next)
		if(af->type == gsn_burning_vision)
			break;

	return ((20 - af->duration) / af->modifier);
}
	
void spell_burning_vision( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af, *maf;
	int mod;
	
	if(is_affected(victim, gsn_burning_vision))
	{
		for (maf = victim->affected; maf != NULL; maf = maf->next)
			if(maf->type == gsn_burning_vision)
				break;

		mod = maf->modifier;
		mod--;
		if(mod < 1)
			return send_to_char("You can not further the progression of the burning vision!\n\r", ch);
		else
		{
			maf->modifier = mod;
			act("$n further burns your vision!", ch, 0, victim, TO_VICT);
			act("You further burn $N's vision!", ch, 0, victim, TO_CHAR);
			act("$n further burns $N's vision!", ch, 0, victim, TO_NOTVICT);
			return;
		}
	}
	else
	{
		init_affect(&af);
		af.where = TO_AFFECTS;
		af.aftype = AFT_COMMUNE;
		af.type = sn;
		af.level = level;
		af.duration = 20;
		af.modifier = 4;
		af.owner = ch;
		af.tick_fun = burning_vision_tick;
		af.mod_name = MOD_VISION;
		new_affect_to_char( victim, &af );
		act("You sear $N's vision!", ch, 0, victim, TO_CHAR);
		act("$n sears your vision!", ch, 0, victim, TO_VICT);
		act("$n sears $N's vision!", ch, 0, victim, TO_NOTVICT);

	}
	return;
}

void burning_vision_tick(CHAR_DATA *ch, AFFECT_DATA *af)
{
	AFFECT_DATA *caf;

	if((get_bv_stage(ch) >= 4) && !IS_AFFECTED(ch, AFF_BLIND))
	{
		act("You are blinded!", ch, 0, 0, TO_CHAR);
		act("$n appears to be blinded.", ch, 0, 0, TO_ROOM);
		for (caf = ch->affected; caf != NULL; caf = caf->next)
	        if(caf->type == gsn_burning_vision)
				break;
		SET_BIT(caf->bitvector, AFF_BLIND);
		SET_BIT(ch->affected_by,AFF_BLIND);
	}
	return;
}

void spell_divine_malison(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af, *paf;
	int reduction;

	if (victim == ch)
		return send_to_char("Your deity would not approve.\n\r",ch);

	if (victim
		&& (paf = affect_find(victim->affected,gsn_divine_ward)) != NULL
		&& paf->owner == ch)
		return act("Your deity already protects you from $N!",ch,0,victim,TO_CHAR);

	if (ch->fighting && saves_spell(level + 9, victim, DAM_HOLY)) {
		act("A nimbus flickers briefly around $n, but dissipates.",victim,0,0,TO_ROOM);
		act("A haze surrounds you briefly, but dissipates.",victim,0,0,TO_CHAR);
		return;
	}

	if (saves_spell( level + 6, victim, DAM_HOLY)) {
		reduction = 1;
		act("A hazy barrier forms between yourself and $n.",ch,0,victim,TO_VICT);
		act("A hazy barrier forms between yourself and $N.",ch,0,victim,TO_CHAR);
		act("A hazy barrier forms between $n and $N.",ch,0,victim,TO_NOTVICT);
	} else {
		act("A luminous barrier forms between yourself and $n.",ch,0,victim,TO_VICT);
		act("A luminous barrier forms between yourself and $N.",ch,0,victim,TO_CHAR);
		act("A luminous barrier forms between $n and $N.",ch,0,victim,TO_NOTVICT);
		reduction = 2;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_COMMUNE;
	af.type = gsn_divine_ward;
	af.level = level;
	af.duration = level/4;
	af.modifier = reduction;
	af.owner = ch;
	affect_to_char( victim, &af );
	
	return;
}
