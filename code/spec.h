//DO NOT TOUCH!!!!
#ifndef SPECSLOADED
#include "utility.h"			// is_npc, used by the mob fire helpers

//
// Object special programs.
//
// One slot per event the engine can fire at an object. A program fills only
// the slots it implements and leaves the rest null, so the set of events a
// program handles is the set of slots it fills. There is no separate event
// mask to keep in agreement with the handler bodies, and a handler that no
// event reaches, or an event advertised with nothing behind it, cannot be
// expressed.
//
// Each slot's parameter list is the payload the fire site passes. The fire
// helpers below are the only callers, so changing a payload changes both
// sides at once and the compiler checks every program against it.
//
struct ISpec
{
	// Name as it appears in area files. Loading and saving match on this.
	const char *name = nullptr;

	int (*on_wear)(CHAR_DATA *ch, OBJ_DATA *obj) = nullptr;
	int (*on_remove)(CHAR_DATA *ch, OBJ_DATA *obj) = nullptr;

	// container is null when the object is picked up off the floor.
	int (*on_get)(CHAR_DATA *ch, OBJ_DATA *container, OBJ_DATA *obj) = nullptr;
	int (*on_drop)(CHAR_DATA *ch, OBJ_DATA *obj) = nullptr;
	int (*on_sacrifice)(CHAR_DATA *ch, OBJ_DATA *obj) = nullptr;
	int (*on_give)(CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *obj) = nullptr;
	int (*on_greet)(CHAR_DATA *ch, OBJ_DATA *obj) = nullptr;
	int (*on_open)(CHAR_DATA *ch, OBJ_DATA *obj) = nullptr;

	int (*on_fight)(CHAR_DATA *ch, OBJ_DATA *obj) = nullptr;
	int (*on_death)(CHAR_DATA *victim, OBJ_DATA *obj) = nullptr;
	int (*on_speech)(CHAR_DATA *ch, OBJ_DATA *obj, char *argument) = nullptr;

	// Fires on the object itself, not on a character near it.
	int (*on_pulse)(OBJ_DATA *obj, bool isTick) = nullptr;

	int (*on_verb)(CHAR_DATA *ch, OBJ_DATA *obj, const char *verb, const char *argument) = nullptr;

	// dam, dt and dam_type are the running values for this blow. A program
	// may adjust them in place. Returning non-zero cancels the blow.
	int (*on_one_hit)(CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *wield, float &dam, int &dt, DamageType &dam_type) = nullptr;

	// vo is the command's resolved target and is still type erased. It keeps
	// whatever shape the skill table gave it.
	int (*on_do_fun)(CHAR_DATA *ch, OBJ_DATA *obj, int cmd, int sn, void *vo) = nullptr;
};

extern const ISpec ispec_table[];

//
// Fire helpers. Each one resolves the program attached to the object, returns
// 0 when there is no program or no handler for the event, and otherwise calls
// the handler. A return of 0 always means "nothing handled this".
//

/// Resolves the special program attached to an object, if any.
/// @param obj The object to resolve. May be null.
/// @return The attached program, or null when the object has none.
/// @note Replaces the owner->pIndexData->spec_prog dereference that TRAPS_IEVENT
///       and CALL_IEVENT each performed separately on every fire site.
inline const ISpec *obj_spec(const OBJ_DATA *obj)
{
	if (obj == nullptr || obj->pIndexData == nullptr)
		return nullptr;

	return obj->pIndexData->spec;
}

/// Fires the wear event for an object that has just been equipped.
/// @param obj The object being worn.
/// @param ch The character wearing it.
/// @return Non-zero if the program handled the event, 0 otherwise.
/// @note Replaces the TRAPS_IEVENT and CALL_IEVENT pair for TRAP_IWEAR. Bodies
///       written with the EVENT_IWEAR macro now fill ISpec::on_wear.
inline int spec_obj_wear(OBJ_DATA *obj, CHAR_DATA *ch)
{
	const ISpec *spec = obj_spec(obj);

	if (spec == nullptr || spec->on_wear == nullptr)
		return 0;

	return spec->on_wear(ch, obj);
}

/// Fires the remove event for an object that has just been unequipped.
/// @param obj The object being removed.
/// @param ch The character removing it.
/// @return Non-zero if the program handled the event, 0 otherwise.
/// @note Replaces the TRAPS_IEVENT and CALL_IEVENT pair for TRAP_IREMOVE. Bodies
///       written with the EVENT_IREMOVE macro now fill ISpec::on_remove.
inline int spec_obj_remove(OBJ_DATA *obj, CHAR_DATA *ch)
{
	const ISpec *spec = obj_spec(obj);

	if (spec == nullptr || spec->on_remove == nullptr)
		return 0;

	return spec->on_remove(ch, obj);
}

/// Fires the get event when an object is picked up or looted.
/// @param owner The object whose program runs. This is the container when looting.
/// @param ch The character taking the object.
/// @param container The container being looted, or null when taking from the floor.
/// @param obj The object being taken.
/// @return Non-zero if the program handled the event, in which case the take is
///         cancelled. 0 otherwise.
/// @note Replaces the TRAPS_IEVENT and CALL_IEVENT pair for TRAP_ILOOT. Bodies
///       written with the EVENT_ILOOT or EVENT_IGET macros now fill ISpec::on_get.
///       Those two macros named the same bit and neither could compile, because
///       both expanded to EVENT_TRAP(event) against an identifier that no spec
///       function declares.
inline int spec_obj_get(OBJ_DATA *owner, CHAR_DATA *ch, OBJ_DATA *container, OBJ_DATA *obj)
{
	const ISpec *spec = obj_spec(owner);

	if (spec == nullptr || spec->on_get == nullptr)
		return 0;

	return spec->on_get(ch, container, obj);
}

/// Fires the drop event for an object leaving a character's inventory.
/// @param obj The object being dropped.
/// @param ch The character dropping it.
/// @return Non-zero if the program handled the event, 0 otherwise.
/// @note Replaces the TRAPS_IEVENT and CALL_IEVENT pair for TRAP_IDROP. Bodies
///       written with the EVENT_IDROP macro now fill ISpec::on_drop.
inline int spec_obj_drop(OBJ_DATA *obj, CHAR_DATA *ch)
{
	const ISpec *spec = obj_spec(obj);

	if (spec == nullptr || spec->on_drop == nullptr)
		return 0;

	return spec->on_drop(ch, obj);
}

/// Fires the sacrifice event for an object being given to the gods.
/// @param obj The object being sacrificed.
/// @param ch The character sacrificing it.
/// @return Non-zero if the program handled the event, in which case the sacrifice
///         is cancelled. 0 otherwise.
/// @note Replaces the TRAPS_IEVENT and CALL_IEVENT pair for TRAP_ISAC. Bodies
///       written with the EVENT_ISAC macro now fill ISpec::on_sacrifice.
inline int spec_obj_sacrifice(OBJ_DATA *obj, CHAR_DATA *ch)
{
	const ISpec *spec = obj_spec(obj);

	if (spec == nullptr || spec->on_sacrifice == nullptr)
		return 0;

	return spec->on_sacrifice(ch, obj);
}

/// Fires the give event for an object handed from one character to another.
/// @param obj The object being handed over.
/// @param ch The character giving it.
/// @param victim The character receiving it.
/// @return Non-zero if the program handled the event, 0 otherwise.
/// @note Replaces the TRAPS_IEVENT and CALL_IEVENT pair for TRAP_IGIVE. Bodies
///       written with the EVENT_IGIVE macro now fill ISpec::on_give.
inline int spec_obj_give(OBJ_DATA *obj, CHAR_DATA *ch, CHAR_DATA *victim)
{
	const ISpec *spec = obj_spec(obj);

	if (spec == nullptr || spec->on_give == nullptr)
		return 0;

	return spec->on_give(ch, victim, obj);
}

/// Fires the greet event when a character enters the object's room.
/// @param obj The object doing the greeting.
/// @param ch The character who entered.
/// @return Non-zero if the program handled the event, 0 otherwise.
/// @note Replaces the TRAPS_IEVENT and CALL_IEVENT pair for TRAP_IGREET. Bodies
///       written with the EVENT_IGREET macro now fill ISpec::on_greet.
inline int spec_obj_greet(OBJ_DATA *obj, CHAR_DATA *ch)
{
	const ISpec *spec = obj_spec(obj);

	if (spec == nullptr || spec->on_greet == nullptr)
		return 0;

	return spec->on_greet(ch, obj);
}

/// Fires the open event for a container being opened.
/// @param obj The container being opened.
/// @param ch The character opening it.
/// @return Non-zero if the program handled the event, in which case the open is
///         cancelled. 0 otherwise.
/// @note Replaces the TRAPS_IEVENT and CALL_IEVENT pair for TRAP_IOPEN. Bodies
///       written with the EVENT_IOPEN macro now fill ISpec::on_open.
inline int spec_obj_open(OBJ_DATA *obj, CHAR_DATA *ch)
{
	const ISpec *spec = obj_spec(obj);

	if (spec == nullptr || spec->on_open == nullptr)
		return 0;

	return spec->on_open(ch, obj);
}

/// Fires the fight event once per combat round for a carried object.
/// @param obj The object carried by the combatant.
/// @param ch The character fighting.
/// @return Non-zero if the program handled the event, 0 otherwise.
/// @note Replaces the TRAPS_IEVENT and CALL_IEVENT pair for TRAP_IFIGHT. Bodies
///       written with the EVENT_IFIGHT macro now fill ISpec::on_fight.
inline int spec_obj_fight(OBJ_DATA *obj, CHAR_DATA *ch)
{
	const ISpec *spec = obj_spec(obj);

	if (spec == nullptr || spec->on_fight == nullptr)
		return 0;

	return spec->on_fight(ch, obj);
}

/// Fires the death event for an object carried by a character who has died.
/// @param obj The carried object.
/// @param victim The character who died.
/// @return Non-zero if the program handled the event, in which case the rest of
///         the death routine is skipped. 0 otherwise.
/// @note Replaces the TRAPS_IEVENT and CALL_IEVENT pair for TRAP_IDEATH. Bodies
///       written with the EVENT_IDEATH macro now fill ISpec::on_death.
inline int spec_obj_death(OBJ_DATA *obj, CHAR_DATA *victim)
{
	const ISpec *spec = obj_spec(obj);

	if (spec == nullptr || spec->on_death == nullptr)
		return 0;

	return spec->on_death(victim, obj);
}

/// Fires the speech event when a character speaks near the object.
/// @param obj The object that overheard the speech.
/// @param ch The character speaking.
/// @param argument What was said. Mutable because handlers tokenize it with
///        one_argument, which takes a mutable pointer.
/// @return Non-zero if the program handled the event, 0 otherwise.
/// @note Replaces the TRAPS_IEVENT and CALL_IEVENT pair for TRAP_ISPEECH. Bodies
///       written with the EVENT_ISPEECH macro now fill ISpec::on_speech.
inline int spec_obj_speech(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	const ISpec *spec = obj_spec(obj);

	if (spec == nullptr || spec->on_speech == nullptr)
		return 0;

	return spec->on_speech(ch, obj, argument);
}

/// Fires the pulse event on the object's own update.
/// @param obj The object being updated.
/// @param isTick True on a full tick, false on an ordinary pulse.
/// @return Non-zero if the program handled the event, 0 otherwise.
/// @note Replaces the TRAPS_IEVENT and CALL_IEVENT pair for TRAP_IPULSE. Bodies
///       written with the EVENT_IPULSE macro now fill ISpec::on_pulse. That macro
///       bound the first argument as CHAR_DATA * while the fire site has always
///       passed the object, so the typed slot corrects the payload as well.
inline int spec_obj_pulse(OBJ_DATA *obj, bool isTick)
{
	const ISpec *spec = obj_spec(obj);

	if (spec == nullptr || spec->on_pulse == nullptr)
		return 0;

	return spec->on_pulse(obj, isTick);
}

/// Fires the verb event when a character types the object's custom verb.
/// @param obj The object that owns the verb.
/// @param ch The character who typed it.
/// @param verb The verb as typed.
/// @param argument The remainder of the command line.
/// @return Non-zero if the program handled the event, 0 otherwise.
/// @note Replaces the TRAPS_IEVENT and CALL_IEVENT pair for TRAP_IVERB. Bodies
///       written with the EVENT_IVERB macro now fill ISpec::on_verb.
inline int spec_obj_verb(OBJ_DATA *obj, CHAR_DATA *ch, const char *verb, const char *argument)
{
	const ISpec *spec = obj_spec(obj);

	if (spec == nullptr || spec->on_verb == nullptr)
		return 0;

	return spec->on_verb(ch, obj, verb, argument);
}

/// Fires the one hit event for the weapon delivering a blow.
/// @param wield The weapon being swung. May be null for an unarmed blow.
/// @param ch The attacker.
/// @param victim The defender.
/// @param dam The running damage for this blow. A program may adjust it in place.
/// @param dt The running damage type index. A program may adjust it in place.
/// @param dam_type The running damage class. A program may adjust it in place.
/// @return Non-zero if the program handled the event, in which case the blow is
///         cancelled. 0 otherwise.
/// @note Replaces the TRAPS_IEVENT and CALL_IEVENT pair for TRAP_IONEHIT. Bodies
///       written with the EVENT_IONEHIT macro now fill ISpec::on_one_hit. The
///       macro passed dam, dt and dam_type as pointers and rebound them as
///       references, which the slot now does directly.
inline int spec_obj_one_hit(OBJ_DATA *wield, CHAR_DATA *ch, CHAR_DATA *victim, float &dam, int &dt, DamageType &dam_type)
{
	const ISpec *spec = obj_spec(wield);

	if (spec == nullptr || spec->on_one_hit == nullptr)
		return 0;

	return spec->on_one_hit(ch, victim, wield, dam, dt, dam_type);
}

/// Fires the command event for each object a character carries, before the
///        command itself runs.
/// @param obj The carried object.
/// @param ch The character issuing the command.
/// @param cmd The index of the command in cmd_table.
/// @param sn The skill number when the command is a skill, otherwise unset.
/// @param vo The command's resolved target. Still type erased.
/// @return Non-zero if the program handled the event, in which case the command
///         does not run. 0 otherwise.
/// @note Replaces the TRAPS_IEVENT and CALL_IEVENT pair for TRAP_IDOFUN. Bodies
///       written with the EVENT_IDOFUN macro now fill ISpec::on_do_fun.
inline int spec_obj_do_fun(OBJ_DATA *obj, CHAR_DATA *ch, int cmd, int sn, void *vo)
{
	const ISpec *spec = obj_spec(obj);

	if (spec == nullptr || spec->on_do_fun == nullptr)
		return 0;

	return spec->on_do_fun(ch, obj, cmd, sn, vo);
}

/// Sends the names of the events a program handles to a character.
/// @param spec The program to describe. May be null, in which case nothing is sent.
/// @param to The character to send the list to.
/// @note Replaces the loop in the stat commands that walked every bit of
///       spec_events and mapped each one to a name through flag_name_lookup and
///       ievent_table. Both that table and its IEVENT macro are gone. The table
///       carried no null terminator, so a bit with no entry read past its end.
void ispec_event_names(const ISpec *spec, CHAR_DATA *to);

//
// Mob special programs.
//
// The same slot rule as ISpec above. The naming rule is worth stating because
// the macros this replaces did not follow one: mob is always the character
// that owns the program, and ch is always the other party. Under the old
// macros ch meant the owning mob in a pulse event and the speaker in a speech
// event, so one name meant two things in adjacent blocks of one program.
//
struct MSpec
{
	// Name as it appears in area files. Loading and saving match on this.
	const char *name = nullptr;

	int (*on_greet)(CHAR_DATA *ch, CHAR_DATA *mob) = nullptr;
	int (*on_give)(CHAR_DATA *ch, CHAR_DATA *mob, OBJ_DATA *obj) = nullptr;
	int (*on_speech)(CHAR_DATA *ch, CHAR_DATA *mob, char *argument) = nullptr;

	// The mob's own update. There is no other party.
	int (*on_pulse)(CHAR_DATA *mob) = nullptr;

	int (*on_death)(CHAR_DATA *ch, CHAR_DATA *mob) = nullptr;

	// dam, dt and dam_type are the running values for the blow landing on mob.
	// A program may adjust them in place. Returning non-zero cancels the blow.
	int (*on_one_hit)(CHAR_DATA *ch, CHAR_DATA *mob, OBJ_DATA *wield, float &dam, int &dt, DamageType &dam_type) = nullptr;

	int (*on_move)(CHAR_DATA *ch, CHAR_DATA *mob, int door) = nullptr;
};

extern const MSpec mspec_table[];

/// Resolves the special program attached to a mob, if any.
/// @param mob The character to resolve. May be null or a player.
/// @return The attached program, or null when there is none.
/// @note Replaces the owner->pIndexData->spec_prog dereference in TRAPS_MEVENT
///       and CALL_MEVENT. The is_npc test that guarded TRAPS_MEVENT lives here
///       now, so callers no longer repeat it.
inline const MSpec *mob_spec(CHAR_DATA *mob)
{
	if (mob == nullptr || !is_npc(mob) || mob->pIndexData == nullptr)
		return nullptr;

	return mob->pIndexData->spec;
}

/// Fires the greet event when a character enters the mob's room.
/// @param mob The mob doing the greeting.
/// @param ch The character who entered.
/// @return Non-zero if the program handled the event, 0 otherwise.
/// @note Replaces the TRAPS_MEVENT and CALL_MEVENT pair for TRAP_MGREET. Bodies
///       written with the EVENT_MGREET macro now fill MSpec::on_greet.
inline int spec_mob_greet(CHAR_DATA *mob, CHAR_DATA *ch)
{
	const MSpec *spec = mob_spec(mob);

	if (spec == nullptr || spec->on_greet == nullptr)
		return 0;

	return spec->on_greet(ch, mob);
}

/// Fires the give event when a character hands the mob an object.
/// @param mob The mob receiving the object.
/// @param ch The character giving it.
/// @param obj The object handed over.
/// @return Non-zero if the program handled the event, 0 otherwise.
/// @note Replaces the TRAPS_MEVENT and CALL_MEVENT pair for TRAP_MGIVE. Bodies
///       written with the EVENT_MGIVE macro now fill MSpec::on_give.
inline int spec_mob_give(CHAR_DATA *mob, CHAR_DATA *ch, OBJ_DATA *obj)
{
	const MSpec *spec = mob_spec(mob);

	if (spec == nullptr || spec->on_give == nullptr)
		return 0;

	return spec->on_give(ch, mob, obj);
}

/// Fires the speech event when a character speaks near the mob.
/// @param mob The mob that overheard the speech.
/// @param ch The character speaking.
/// @param argument What was said. Mutable because handlers tokenize it with
///        one_argument, which takes a mutable pointer.
/// @return Non-zero if the program handled the event, 0 otherwise.
/// @note Replaces the TRAPS_MEVENT and CALL_MEVENT pair for TRAP_MSPEECH. Bodies
///       written with the EVENT_MSPEECH macro now fill MSpec::on_speech.
inline int spec_mob_speech(CHAR_DATA *mob, CHAR_DATA *ch, char *argument)
{
	const MSpec *spec = mob_spec(mob);

	if (spec == nullptr || spec->on_speech == nullptr)
		return 0;

	return spec->on_speech(ch, mob, argument);
}

/// Fires the pulse event on the mob's own update.
/// @param mob The mob being updated.
/// @return Non-zero if the program handled the event, 0 otherwise.
/// @note Replaces the TRAPS_MEVENT and CALL_MEVENT pair for TRAP_MPULSE. Bodies
///       written with the EVENT_MPULSE macro now fill MSpec::on_pulse. That
///       macro bound the mob to the name ch, unlike every other mob event.
inline int spec_mob_pulse(CHAR_DATA *mob)
{
	const MSpec *spec = mob_spec(mob);

	if (spec == nullptr || spec->on_pulse == nullptr)
		return 0;

	return spec->on_pulse(mob);
}

/// Fires the death event for a mob that has just been killed.
/// @param mob The mob that died.
/// @param ch The killer.
/// @return Non-zero if the program handled the event, in which case the rest of
///         the death routine is skipped and the mob does not actually die.
///         0 otherwise.
/// @note Replaces the TRAPS_MEVENT and CALL_MEVENT pair for TRAP_MDEATH. Bodies
///       written with the EVENT_MDEATH macro now fill MSpec::on_death.
inline int spec_mob_death(CHAR_DATA *mob, CHAR_DATA *ch)
{
	const MSpec *spec = mob_spec(mob);

	if (spec == nullptr || spec->on_death == nullptr)
		return 0;

	return spec->on_death(ch, mob);
}

/// Fires the one hit event for a mob taking a blow.
/// @param mob The mob being hit.
/// @param ch The attacker.
/// @param wield The attacker's weapon. May be null for an unarmed blow.
/// @param dam The running damage for this blow. A program may adjust it in place.
/// @param dt The running damage type index. A program may adjust it in place.
/// @param dam_type The running damage class. A program may adjust it in place.
/// @return Non-zero if the program handled the event, in which case the blow is
///         cancelled. 0 otherwise.
/// @note Replaces the TRAPS_MEVENT guard and the CALL_IEVENT dispatch for
///       TRAP_MONEHIT. The fire site used the object macro to dispatch a mob
///       event, which went unnoticed because the two macros were identical.
///       Bodies written with the EVENT_MONEHIT macro now fill MSpec::on_one_hit.
inline int spec_mob_one_hit(CHAR_DATA *mob, CHAR_DATA *ch, OBJ_DATA *wield, float &dam, int &dt, DamageType &dam_type)
{
	const MSpec *spec = mob_spec(mob);

	if (spec == nullptr || spec->on_one_hit == nullptr)
		return 0;

	return spec->on_one_hit(ch, mob, wield, dam, dt, dam_type);
}

/// Fires the move event when a character leaves the mob's room.
/// @param mob The mob watching the exit.
/// @param ch The character moving.
/// @param door The direction taken.
/// @return Non-zero if the program handled the event, in which case the move is
///         cancelled. 0 otherwise.
/// @note Replaces the TRAPS_MEVENT and CALL_MEVENT pair for TRAP_MMOVE. Bodies
///       written with the EVENT_MMOVE macro now fill MSpec::on_move. No program
///       fills this slot, so the fire site is reachable but idle.
inline int spec_mob_move(CHAR_DATA *mob, CHAR_DATA *ch, int door)
{
	const MSpec *spec = mob_spec(mob);

	if (spec == nullptr || spec->on_move == nullptr)
		return 0;

	return spec->on_move(ch, mob, door);
}

/// Sends the names of the events a program handles to a character.
/// @param spec The program to describe. May be null, in which case nothing is sent.
/// @param to The character to send the list to.
/// @note Replaces the loop in the stat commands that walked every bit of
///       spec_events and mapped each one to a name through flag_name_lookup and
///       mevent_table. Both that table and its MEVENT macro are gone.
void mspec_event_names(const MSpec *spec, CHAR_DATA *to);

#define SPECSLOADED
#endif
