// Calenduil was heeere! - note to self, possible memleak under returning without va_end
#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "entity/handles.h"
#include "spec.h"
#include "tables.h"
#include "interp.h"
#include "comm.h"
#include "act_comm.h"
#include "db.h"
#include "handler.h"
#include "devextra.h"

/// Blinds the wielder's opponent on a small chance each combat round.
/// @param ch The character wielding the flail.
/// @return Always 0, so combat continues normally.
/// @note Body taken from the EVENT_IFIGHT block of ispec_torture_flail.
static int torture_flail_fight(CHAR_DATA *ch, OBJ_DATA *)
{
	if (number_percent() <= 91 || !Deref(ch->fighting))
		return 0;

	AFFECT_DATA af;
	init_affect(&af);

	act("A scream escapes your flail as a shadow tears at $N's eyes!", ch, 0, Deref(ch->fighting), TO_CHAR);
	act("A scream escapes $n's flail as a shadow tears at $N's eyes!", ch, 0, Deref(ch->fighting), TO_NOTVICT);
	act("A scream escapes $n's flail as a shadow tears at your eyes!", ch, 0, Deref(ch->fighting), TO_VICT);
	act("$n appears to be blinded.", Deref(ch->fighting), 0, 0, TO_ROOM);

	af.type = gsn_blindness;
	af.aftype = AFT_MALADY;
	af.level = ch->level;
	af.duration = 2;

	SET_BIT(af.bitvector, AFF_BLIND);
	affect_to_char(Deref(ch->fighting), &af);

	return 0;
}

/// Complains at the character wearing the object.
/// @param ch The character wearing it.
/// @return Always 0.
/// @note Body taken from the EVENT_IWEAR block of ispec_g_money.
static int g_money_wear(CHAR_DATA *ch, OBJ_DATA *)
{
	act("J00 gonna wear me!? J00 GONNA WEAR ME?!?!?", ch, 0, 0, TO_CHAR);
	return 0;
}

/// Complains at the character removing the object.
/// @param ch The character removing it.
/// @param obj The object being removed.
/// @return Always 0.
/// @note Body taken from the EVENT_IREMOVE block of ispec_g_money.
static int g_money_remove(CHAR_DATA *ch, OBJ_DATA *obj)
{
	act("You feel vary vary stoopid as you remove $p.", ch, obj, 0, TO_CHAR);
	do_say(ch, "Duuuuuuuuh.");

	return 0;
}

/// Echoes a sadistic message on a small chance when the whip lands a blow.
/// @param ch The attacker wielding the whip.
/// @return Always 0, so the blow lands normally.
/// @note Body taken from the EVENT_IONEHIT block of ispec_qwhip. The damage,
///       damage type and damage class parameters are unused here. The commented
///       out lines that once adjusted them are kept for reference.
static int qwhip_one_hit(CHAR_DATA *ch, CHAR_DATA *, OBJ_DATA *, float &, int &, int &)
{
	if (number_percent() > 96 && Deref(ch->fighting))
	{
		act("Sadistic urges compel you to lash out viciously at $N!", ch, 0, Deref(ch->fighting), TO_CHAR);
		//dam = dam * 1.3;
		//dt = 42;
		//ch->hit = std::min(ch->max_hit, ch->hit + ((int)dam / 4));
	}

	return 0;
}

/// Staggers the wielder's opponent on a small chance each combat round.
/// @param ch The character wielding the whip.
/// @return Always 0, so combat continues normally.
/// @note Body taken from the EVENT_IFIGHT block of ispec_qwhip.
static int qwhip_fight(CHAR_DATA *ch, OBJ_DATA *)
{
	if (number_percent() <= 97 || !IS_SET(Deref(ch->fighting)->parts, PART_LEGS))
		return 0;

	CHAR_DATA *victim = Deref(ch->fighting);

	act("$n wraps $s whip around $N's legs, sending $M staggering!", ch, 0, victim, TO_NOTVICT);
	act("You wrap your whip around $N's legs, sending $M staggering.", ch, 0, victim, TO_CHAR);
	act("$n wraps $s whip around your legs, sending you staggering!", ch, 0, victim, TO_VICT);

	WAIT_STATE(victim, PULSE_VIOLENCE + 2);

	return 0;
}

//////////////////////////////////////////////////////////////////////
//////// ALL SPEC CODE IS ABOVE HERE - BELOW IS MAINTENANCE //////////

const ISpec ispec_table[] =
{
	// Traps both events but does nothing with either. Kept so that objects
	// already carrying this prog still load.
	{
		.name = "ispec_toy"
	},
	{
		.name = "ispec_g_money",
		.on_wear = g_money_wear,
		.on_remove = g_money_remove
	},
	{
		.name = "ispec_qwhip",
		.on_fight = qwhip_fight,
		.on_one_hit = qwhip_one_hit
	},
	{
		.name = "ispec_torture_flail",
		.on_fight = torture_flail_fight
	},
	{ }
};

/// Sends the names of the events a program handles to a character.
/// @param spec The program to describe. May be null, in which case nothing is sent.
/// @param to The character to send the list to.
/// @note The names match the strings the IEVENT entries of the old ievent_table
///       used, so builder facing output is unchanged.
void ispec_event_names(const ISpec *spec, CHAR_DATA *to)
{
	if (spec == nullptr)
		return;

	if (spec->on_wear) send_to_char("wear ", to);
	if (spec->on_remove) send_to_char("remove ", to);
	if (spec->on_get) send_to_char("get/loot ", to);
	if (spec->on_drop) send_to_char("drop ", to);
	if (spec->on_sacrifice) send_to_char("sac ", to);
	if (spec->on_give) send_to_char("give ", to);
	if (spec->on_greet) send_to_char("greet ", to);
	if (spec->on_open) send_to_char("open ", to);
	if (spec->on_fight) send_to_char("fight ", to);
	if (spec->on_death) send_to_char("death ", to);
	if (spec->on_speech) send_to_char("speech ", to);
	if (spec->on_pulse) send_to_char("pulse ", to);
	if (spec->on_verb) send_to_char("verb ", to);
	if (spec->on_one_hit) send_to_char("one_hit ", to);
	if (spec->on_do_fun) send_to_char("do_fun ", to);
}
