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

void spell_healing_sleep( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if(is_affected(victim, gsn_healing_sleep))
        return send_to_char("They are already in a deep healing sleep!\n\r", ch);

    if(!trusts(ch,victim))
        return send_to_char("They must trust you before you put them into a healing sleep.\n\r", ch);

    if(ch != victim)
    {
        act("You put $N into a deep healing sleep.", ch, 0, victim, TO_CHAR);
        act("You fall into a deep healing sleep.", ch, 0, victim, TO_VICT);
        act("$N falls into a deep healing sleep.", ch, 0, victim, TO_NOTVICT);
    }
    else
    {
        act("You put yourself into a deep healing sleep.", ch, 0, 0, TO_CHAR);
        act("$n falls into a deep healing sleep.", ch, 0, 0, TO_ROOM);
    }

    victim->position = POS_SLEEPING;
    init_affect(&af);
    af.where = TO_AFFECTS;
    af.aftype = AFT_COMMUNE;
    af.type = sn;
    af.level = level;
    af.duration = 4;
    af.owner = ch;
    SET_BIT(af.bitvector, AFF_SLEEP);
    af.end_fun = healing_sleep_end;
    af.mod_name = MOD_REGEN;
    new_affect_to_char( victim, &af );

    return;
}

void healing_sleep_end(CHAR_DATA *ch, AFFECT_DATA *af)
{
    AFFECT_DATA *laf;

    if(IS_AWAKE(ch))
        return;

    for (laf = ch->affected; laf != NULL; laf = laf->next )
        if (laf->type > 5 && skill_table[laf->type].dispel & CAN_CLEANSE)
            affect_strip(ch, laf->type);

    ch->position = POS_STANDING;
    ch->hit = ch->max_hit;
    ch->mana = ch->max_mana;
    ch->move = ch->max_move;
    act("You wake up feeling completely healed and cured of all ailments.", ch, 0, 0, TO_CHAR);
    act("$n wakes and looks to be completely healed.", ch, 0, 0, TO_ROOM);
    return;
}
