//DO NOT TOUCH!!!!
#ifndef SPECSLOADED
#include <stdarg.h>
extern va_list arglist;
typedef int SFUN (long event_vector, ...);

#define BEGIN_MSPECS			const struct spec_type mspec_table[] = {
#define END_SPECS			{ nullptr, nullptr, 0 }, };

#define DEF_SPEC(fname, events)		{ #fname, fname, events },
#define BEGIN_SPEC(sname)		int sname (long event_vector, ...) {
#define END_SPEC			return 0; }
#define DECLARE_SPEC(sname)		int sname (long event_vector, ...)
#define EVENT_TRAP(event)		if(event_vector == event) {
#define END_EVENT			} }
#define TRAPS_MEVENT(owner, event)	IS_NPC(owner) ? owner->pIndexData->spec_prog.trapvector & event : false
#define CALL_MEVENT(owner, event, ...)	(*owner->pIndexData->spec_prog.func) (event, __VA_ARGS__)

struct spec_type
{
	const char *spec_name;
	SFUN *spec_func;
	long spec_events;
};

#define BEGIN_MEVENT_TYPES		const struct flag_type mevent_table[] = {
#define MEVENT(name, bit)		{ name, bit, true },
#define END_EVENT_TYPES			{ nullptr, 0, false } };

#define TRAP_MBEAT			1
#define TRAP_MFIGHT			2
#define TRAP_MUNUSED		4
#define TRAP_MMISC			8
#define TRAP_MMOVE			16
#define TRAP_MDEATH			32
#define TRAP_MPULSE			64
#define TRAP_MSPEECH		128
#define TRAP_MENTRY			256
#define TRAP_MATTACK		512
#define TRAP_MAGGRESS		1024
#define TRAP_MGIVE			2048
#define TRAP_MGREET			4096
#define TRAP_MONEHIT		8192

#define START_TRAP			va_start(arglist, event_vector)
#define END_TRAP			va_end(arglist)

#define GET_CH(x)           CHAR_DATA *x = (CHAR_DATA *) va_arg(arglist, void *)
#define GET_OBJ(x)          OBJ_DATA *x = (OBJ_DATA *)va_arg(arglist, void *)
#define GET_STRING(x)		char *x = (char *)va_arg(arglist, char *)
#define EVENT_TRAP_CH(event)		EVENT_TRAP(event) START_TRAP; { \
				 CHAR_DATA *ch = (CHAR_DATA *)va_arg(arglist, void *); \
				 END_TRAP;

#define EVENT_TRAP_ONE_HIT(event) EVENT_TRAP(event) START_TRAP; { \
				 CHAR_DATA *ch = (CHAR_DATA *)va_arg(arglist, void *); \
				 CHAR_DATA *victim = (CHAR_DATA *)va_arg(arglist, void *); \
				 OBJ_DATA *wield = (OBJ_DATA *)va_arg(arglist, void *); \
				 float *damb = (float *)va_arg(arglist, float *), &dam = *damb; \
				 int *dtb = (int *)va_arg(arglist, int *), &dt = *dtb; \
				 int *dam_typeb = (int *)va_arg(arglist, int *), &dam_type = *dam_typeb; \
				 END_TRAP;

#define EVENT_TRAP_CH_MOB(event)   EVENT_TRAP(event) START_TRAP; { GET_CH(ch); GET_CH(mob); END_TRAP;
#define EVENT_TRAP_CH_MOB_OBJ(event) EVENT_TRAP(event) START_TRAP; { GET_CH(ch); GET_CH(mob); GET_OBJ(obj); END_TRAP;
#define EVENT_MBEAT			EVENT_TRAP_CH(TRAP_MBEAT)
#define EVENT_MFIGHT		EVENT_TRAP_CH_MOB(TRAP_MFIGHT)
#define EVENT_MGIVE			EVENT_TRAP_CH_MOB_OBJ(TRAP_MGIVE)
#define EVENT_MSPEECH		EVENT_TRAP(TRAP_MSPEECH) START_TRAP; { \
								GET_CH(ch); GET_CH(mob); GET_STRING(argument); END_TRAP;
#define EVENT_MGREET		EVENT_TRAP_CH_MOB(TRAP_MGREET)
#define EVENT_MMOVE			EVENT_TRAP(TRAP_MMOVE) START_TRAP; { \
								GET_CH(ch); GET_CH(mob); int door = (int)va_arg(arglist, int); END_TRAP;
#define EVENT_MPULSE		EVENT_TRAP_CH(TRAP_MPULSE)
#define EVENT_MDEATH		EVENT_TRAP_CH_MOB(TRAP_MDEATH)
#define EVENT_MONEHIT		EVENT_TRAP_ONE_HIT(TRAP_MONEHIT)
extern const struct spec_type mspec_table[];
extern const struct flag_type mevent_table[];

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
	int (*on_speech)(CHAR_DATA *ch, OBJ_DATA *obj, const char *argument) = nullptr;

	// Fires on the object itself, not on a character near it.
	int (*on_pulse)(OBJ_DATA *obj, bool isTick) = nullptr;

	int (*on_verb)(CHAR_DATA *ch, OBJ_DATA *obj, const char *verb, const char *argument) = nullptr;

	// dam, dt and dam_type are the running values for this blow. A program
	// may adjust them in place. Returning non-zero cancels the blow.
	int (*on_one_hit)(CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *wield, float &dam, int &dt, int &dam_type) = nullptr;

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
/// @param argument What was said.
/// @return Non-zero if the program handled the event, 0 otherwise.
/// @note Replaces the TRAPS_IEVENT and CALL_IEVENT pair for TRAP_ISPEECH. Bodies
///       written with the EVENT_ISPEECH macro now fill ISpec::on_speech.
inline int spec_obj_speech(OBJ_DATA *obj, CHAR_DATA *ch, const char *argument)
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
inline int spec_obj_one_hit(OBJ_DATA *wield, CHAR_DATA *ch, CHAR_DATA *victim, float &dam, int &dt, int &dam_type)
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

#define SPECSLOADED
#endif
