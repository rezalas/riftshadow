//DO NOT TOUCH!!!!
#ifndef SPECSLOADED
#include <stdarg.h>
extern va_list arglist;
typedef int SFUN (long event_vector, ...);

#define BEGIN_MSPECS			const struct spec_type mspec_table[] = {
#define BEGIN_ISPECS    		const struct spec_type ispec_table[] = {
#define BEGIN_RSPECS    		const struct spec_type rspec_table[] = {
#define BEGIN_ASPECS    		const struct spec_type aspec_table[] = {
#define END_SPECS			{ NULL, NULL, 0 }, };

#define DEF_SPEC(fname, events)		{ #fname, fname, events },
#define BEGIN_SPEC(sname)		int sname (long event_vector, ...) {
#define END_SPEC			return 0; }
#define DECLARE_SPEC(sname)		int sname (long event_vector, ...)
#define EVENT_TRAP(event)		if(event_vector == event) {
#define END_EVENT			} }
#define BV(x)				pow(2,x)
#define TRAPS_IEVENT(owner, event)	owner->pIndexData->spec_prog.trapvector & event
#define TRAPS_MEVENT(owner, event)	IS_NPC(owner) ? owner->pIndexData->spec_prog.trapvector & event : FALSE
#define CALL_IEVENT(owner, event, ...)	(*owner->pIndexData->spec_prog.func) (event, __VA_ARGS__)
#define CALL_MEVENT(owner, event, ...)	(*owner->pIndexData->spec_prog.func) (event, __VA_ARGS__)

struct spec_type
{
const char *spec_name;
SFUN *spec_func;
long spec_events;
};

#define BEGIN_IEVENT_TYPES		const struct flag_type ievent_table[] = {
#define BEGIN_MEVENT_TYPES		const struct flag_type mevent_table[] = {
#define IEVENT(name, bit)		{ name, bit, TRUE },
#define MEVENT(name, bit)		{ name, bit, TRUE },
#define END_EVENT_TYPES			};
#define TRAP_IWEAR			1
#define TRAP_IREMOVE		2
#define TRAP_IGET			4
#define TRAP_ILOOT			4 			/*get == loot with null container*/
#define TRAP_IDROP			8
#define TRAP_ISAC			16
#define TRAP_IGIVE			32
#define TRAP_IGREET			64
#define TRAP_IFIGHT			128
#define TRAP_IDEATH			256
#define TRAP_ISPEECH		512
#define TRAP_IPULSE			1024
#define TRAP_IVERB			2048
#define TRAP_IOPEN			4096
#define TRAP_IONEHIT		8192
#define TRAP_IDOFUN			16384
#define TRAP_IMISC			32768			/*used for low frequency events checked individually*/

#define TRAP_MBEAT			1
#define TRAP_MFIGHT			2
#define TRAP_MUNUSED			4
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
#define TRAP_MONEHIT			8192

#define START_TRAP			va_start(arglist, event_vector)
#define END_TRAP			va_end(arglist)

#define GET_CH(x)           CHAR_DATA *x = (CHAR_DATA *) va_arg(arglist, void *)
#define GET_OBJ(x)          OBJ_DATA *x = (OBJ_DATA *)va_arg(arglist, void *)
#define GET_STRING(x)		char *x = (char *)va_arg(arglist, char *)
#define EVENT_TRAP_CH(event)		EVENT_TRAP(event) START_TRAP; { \
				 CHAR_DATA *ch = (CHAR_DATA *)va_arg(arglist, void *); \
				 END_TRAP;
				 
#define EVENT_TRAP_CH_OBJ(event)	EVENT_TRAP(event) START_TRAP; { \
				 CHAR_DATA *ch = (CHAR_DATA *)va_arg(arglist, void *); \
				 OBJ_DATA  *obj= (OBJ_DATA *)va_arg(arglist, void *); \
				 END_TRAP;

#define EVENT_TRAP_CH_VICTIM(event)	EVENT_TRAP(event) START_TRAP; { \
				 CHAR_DATA *ch = (CHAR_DATA *)va_arg(arglist, void *); \
				 CHAR_DATA *victim = (CHAR_DATA *)va_arg(arglist, void *); \
				 END_TRAP;
				 
#define EVENT_TRAP_CH_VICTIM_OBJ(event) EVENT_TRAP(event) START_TRAP; { \
				 CHAR_DATA *ch = (CHAR_DATA *)va_arg(arglist, void *); \
				 CHAR_DATA *victim = (CHAR_DATA *)va_arg(arglist, void *); \
				 OBJ_DATA *obj = (OBJ_DATA *)va_arg(arglist, void *); \
				 END_TRAP;
				 
#define EVENT_TRAP_DO_FUN		EVENT_TRAP(TRAP_IDOFUN) START_TRAP; { \
				 CHAR_DATA *ch = (CHAR_DATA *)va_arg(arglist, void *); \
				 char *argument = va_arg(arglist, void *);\
				 END_TRAP;
				 
#define EVENT_TRAP_ONE_HIT(event) EVENT_TRAP(event) START_TRAP; { \
				 CHAR_DATA *ch = (CHAR_DATA *)va_arg(arglist, void *); \
				 CHAR_DATA *victim = (CHAR_DATA *)va_arg(arglist, void *); \
				 OBJ_DATA *wield = (OBJ_DATA *)va_arg(arglist, void *); \
				 float *damb = (float *)va_arg(arglist, float *), &dam = *damb; \
				 int *dtb = (int *)va_arg(arglist, int *), &dt = *dtb; \
				 int *dam_typeb = (int *)va_arg(arglist, int *), &dam_type = *dam_typeb; \
				 END_TRAP;
#define EVENT_TRAP_CH_OBJ_ARG(event)  EVENT_TRAP(event) START_TRAP; { \
				 GET_CH(ch); \
				 GET_OBJ(obj); \
				 GET_STRING(argument); END_TRAP;
#define EVENT_TRAP_IVERB(event) EVENT_TRAP(event) START_TRAP; { \
				 GET_CH(ch); \
				 GET_OBJ(obj); \
				 GET_STRING(verb); \
				 GET_STRING(argument); END_TRAP;
#define EVENT_TRAP_IDOFUN(event)	EVENT_TRAP(event) START_TRAP; { \
					 GET_CH(ch); \
					 GET_OBJ(obj); \
					 int cmd = va_arg(arglist, int); \
					 int sn = va_arg(arglist, int); \
					 void *vo = va_arg(arglist, void *); \
					 END_TRAP;

#define EVENT_TRAP_CH_MOB(event)   EVENT_TRAP(event) START_TRAP; { GET_CH(ch); GET_CH(mob); END_TRAP;
#define EVENT_TRAP_CH_MOB_OBJ(event) EVENT_TRAP(event) START_TRAP; { GET_CH(ch); GET_CH(mob); GET_OBJ(obj); END_TRAP;
#define EVENT_IWEAR			EVENT_TRAP_CH_OBJ(TRAP_IWEAR)
#define EVENT_IREMOVE		EVENT_TRAP_CH_OBJ(TRAP_IREMOVE)
#define EVENT_IFIGHT		EVENT_TRAP_CH_OBJ(TRAP_IFIGHT)
#define EVENT_IONEHIT		EVENT_TRAP_ONE_HIT(TRAP_IONEHIT)
#define EVENT_ILOOT			EVENT_TRAP(event) START_TRAP; { \
								GET_CH(ch); \
								GET_OBJ(container); \
								GET_OBJ(obj);
								
#define EVENT_IGET			EVENT_ILOOT
#define EVENT_IDROP			EVENT_TRAP_CH_OBJ(TRAP_IDROP)
#define EVENT_ISAC			EVENT_TRAP_CH_OBJ(TRAP_ISAC)
#define EVENT_IGIVE			EVENT_TRAP_CH_VICTIM_OBJ(TRAP_IGIVE)
#define EVENT_IGREET		EVENT_TRAP_CH_OBJ(TRAP_IGREET)
#define EVENT_IDEATH		EVENT_TRAP_CH_OBJ(TRAP_IDEATH)
#define EVENT_ISPEECH		EVENT_TRAP_CH_OBJ_ARG(TRAP_ISPEECH)
#define EVENT_IPULSE		EVENT_TRAP(TRAP_IPULSE) START_TRAP; { \
								GET_CH(ch); bool isTick = (bool)va_arg(arglist, bool); END_TRAP;
#define EVENT_IVERB			EVENT_TRAP_IVERB(TRAP_IVERB)
#define EVENT_IOPEN			EVENT_TRAP_CH_OBJ(TRAP_IOPEN)
#define EVENT_IDOFUN		EVENT_TRAP_IDOFUN(TRAP_IDOFUN)
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
extern const struct spec_type aspec_table[];
extern const struct spec_type rspec_table[];
extern const struct spec_type ispec_table[];
extern const struct spec_type mspec_table[];
extern const struct flag_type ievent_table[];
extern const struct flag_type mevent_table[];
#define SPECSLOADED
#endif
