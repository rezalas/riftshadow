#ifndef ENUMS_H
#define ENUMS_H


// Scoped constant enums, extracted verbatim from merc.h
//
// Dependency-free by construction: every enumerator is a literal, so this
// header includes nothing and compiles in isolation. Kept in the original
// merc.h source order (which is already roughly domain-clustered) rather 
// than reordered. The values are a wire format for player and area files:
// rename freely, never renumber.


// Mob-program trigger bit indices into mob_index_data::progtypes. One of four
// prog-trigger families (mprog/iprog/rprog/aprog), each on its own entity's
// progtypes field. Wire format - rename freely, never renumber.
enum MProgTrigger : int
{
	MPROG_BRIBE					= 0,
	MPROG_ENTRY					= 1,
	MPROG_GREET					= 2,
	MPROG_GIVE					= 3,
	MPROG_FIGHT					= 4,
	MPROG_DEATH					= 5,
	MPROG_PULSE					= 6,
	MPROG_SPEECH				= 7,
	MPROG_ATTACK				= 8,
	MPROG_MOVE					= 9,
	MPROG_BEAT					= 10,
	MPROG_AGGRESS				= 11,
};

// Item-program trigger bit indices into obj_index_data::progtypes.
// Wire format - rename freely, never renumber. Gap at 16.
enum IProgTrigger : int
{
	IPROG_WEAR					= 0,
	IPROG_REMOVE				= 1,
	IPROG_DROP					= 2,
	IPROG_SAC					= 3,
	IPROG_GIVE					= 4,
	IPROG_GREET					= 5,
	IPROG_FIGHT					= 6,
	IPROG_DEATH					= 7,
	IPROG_SPEECH				= 8,
	IPROG_ENTRY					= 9,
	IPROG_GET					= 10,
	IPROG_PULSE					= 11,
	IPROG_INVOKE				= 12,
	IPROG_VERB					= 13,
	IPROG_LOOT					= 14,
	IPROG_OPEN					= 15,
	IPROG_LOOK					= 17,
	IPROG_HIT					= 18,
};

// Room-program trigger bit indices into room_index_data::progtypes.
enum RProgTrigger : int
{
	RPROG_PULSE					= 0,
	RPROG_ENTRY					= 1,
	RPROG_MOVE					= 2,
	RPROG_DROP					= 3,
	RPROG_SPEECH				= 4,
	RPROG_OPEN					= 5,
};

// Area-program trigger bit indices into area_data::progtypes.
enum AProgTrigger : int
{
	APROG_PULSE					= 0,
	APROG_RESET					= 1,
	APROG_SUN					= 2,
	APROG_TICK					= 3,
	APROG_AGGRESS				= 4,
	APROG_MYELL					= 5,
};

// Yes/no prompt reply ordinal. Wire format - never renumber.
enum ReplyType : int
{
	REPLY_YES					= 1,
	REPLY_NO					= 2,
	REPLY_NEITHER				= 3,
};

// Descriptor connection state, an ordinal in descriptor_data::connected;
// switched on in nanny(). NOTE: CON_DIE_BOUND (defined far above, near the level
// constants) is NOT part of this family. It is a constitution-stat threshold
// that shares the CON_ prefix by accident, and keeps its #define.
// Wire format - never renumber.
enum ConnectionState : int
{
	CON_PLAYING					= 0,
	CON_GET_NAME				= 1,
	CON_GET_OLD_PASSWORD		= 2,
	CON_CONFIRM_NEW_NAME		= 3,
	CON_GET_NEW_PASSWORD		= 4,
	CON_CONFIRM_NEW_PASSWORD	= 5,
	CON_GET_NEW_RACE			= 6,
	CON_GET_NEW_SEX				= 7,
	CON_GET_NEW_CLASS			= 8,
	CON_ALLOCATE_STATS			= 9,
	CON_GET_ALIGNMENT			= 10,
	CON_GET_ETHOS				= 11,
	CON_DEFAULT_CHOICE			= 12,
	CON_GEN_GROUPS				= 13,
	CON_NEW_CHAR				= 14,
	CON_READ_IMOTD				= 15,
	CON_READ_MOTD				= 16,
	CON_BREAK_CONNECT			= 17,
	CON_GET_CABAL				= 18,
	CON_GET_SPEC_ONE			= 19,
	CON_GET_SPEC_TWO			= 20,
	CON_GET_THERMAL				= 21,
	CON_GET_MATERIAL			= 22,
	CON_GET_DYNAMIC				= 23,
	CON_CHOOSE_ELE				= 24,
	CON_LEGIT_NAME				= 25,
	CON_GET_BEAUTY				= 26,
	CON_CHOOSE_WEAPON			= 27,
};

// act() message target ordinal. A separate family from the TO_* "where" values
// below (which discriminate affect_data::bitvector); values 0-5 collide.
enum ActTarget : int
{
	TO_ROOM						= 0,
	TO_NOTVICT					= 1,
	TO_VICT						= 2,
	TO_CHAR						= 3,
	TO_ALL						= 4,
	TO_IMMINROOM				= 5,
	TO_GROUP					= 6,
	TO_NOTGROUP					= 7,
	TO_AREA						= 8,
};

// Door-bar restriction criterion. Three separate BAR_ families follow, colliding
// in value; each is its own enum. Wire format - never renumber.
enum BarCriterion : int
{
	BAR_CLASS					= 0,
	BAR_CABAL					= 1,
	BAR_SIZE					= 2,
	BAR_TATTOO					= 3,
	BAR_LEVEL					= 4,
};

// Bar comparison operator.
enum BarComparison : int
{
	BAR_EQUAL_TO				= 0,
	BAR_LESS_THAN				= 1,
	BAR_GREATER_THAN			= 2,
};

// Bar rejection-message style.
enum BarMessage : int
{
	BAR_SAY						= 0,
	BAR_ECHO					= 1,
	BAR_EMOTE					= 2,
};

// Primary-stat index into char_data::perm_stat[] / mod_stat[]. Wire format -
// rename freely, never renumber.
enum StatType : int
{
	STAT_STR					= 0,
	STAT_INT					= 1,
	STAT_WIS					= 2,
	STAT_DEX					= 3,
	STAT_CON					= 4,
};

// Affect-type ordinal, in affect_data::aftype. Wire format - never renumber.
enum AffectType : int
{
	AFT_SPELL					= 0,
	AFT_SKILL					= 1,
	AFT_POWER					= 2,
	AFT_MALADY					= 3,
	AFT_COMMUNE					= 4,
	AFT_INVIS					= 5,
	AFT_RUNE					= 6,
	AFT_TIMER					= 7,
};

// Spellcasting-class ordinal (how a class channels magic). Separate family from
// the character-class CLASS_NONE.. list and from CLASS_OPEN/CLOSED.
enum SpellClass : int
{
	CLASS_NEITHER				= 0,
	CLASS_CASTER				= 1,
	CLASS_COMMUNER				= 2,
};

// Whether a class is open for selection. A two-value ordinal, separate family.
enum ClassAvailability : int
{
	CLASS_OPEN					= 1,
	CLASS_CLOSED				= 0,
};

// Magic command-type ordinal. Wire format - never renumber.
enum MagicCommand : int
{
	CMD_NONE					= 0,
	CMD_SPELL					= 1,
	CMD_COMMUNE					= 2,
	CMD_POWER					= 3,
	CMD_BOTH					= 4,
	CMD_RUNE					= 5,
};

// affect_data::where - the discriminator that decides which family the sibling
// bitvector holds and which field it applies to (handler.c dispatches on it).
enum AffectWhere : int
{
	TO_AFFECTS					= 0,
	TO_OBJECT					= 1,
	TO_IMMUNE					= 2,
	TO_RESIST					= 3,
	TO_VULN						= 4,
	TO_WEAPON					= 5,
};

// What a rune is applied to - an ordinal. Separate family from RUNE_TRIGGER_*
// and the RUNE_* masks below (all three collide in value).
enum RuneTarget : int
{
	RUNE_TO_WEAPON				= 0,
	RUNE_TO_ARMOR				= 1,
	RUNE_TO_PORTAL				= 2,
	RUNE_TO_ROOM				= 3,
};

// When a rune fires - an ordinal.
enum RuneTrigger : int
{
	RUNE_TRIGGER_ENTRY			= 0,
	RUNE_TRIGGER_EXIT			= 1,
};

// room_affect_data::where - discriminator for a room affect's bitvector.
enum RoomAffectWhere : int
{
	TO_ROOM_AFFECTS				= 0,
	TO_ROOM_CONST				= 1,
	TO_ROOM_FLAGS				= 2,
};

// obj_affect_data::where - discriminator for an object affect's bitvector.
enum ObjAffectWhere : int
{
	TO_OBJ_AFFECTS				= 0,
	TO_OBJ_APPLY				= 1,
};

// Room-affect apply locations. A separate family from the main APPLY_* below:
// values 0-4 collide with APPLY_NONE..APPLY_WIS, so it must be its own enum.
enum ApplyRoomLocation : int
{
	APPLY_ROOM_NONE				= 0,
	APPLY_ROOM_HEAL				= 1,
	APPLY_ROOM_MANA				= 2,
	APPLY_ROOM_SECT				= 3,
	APPLY_ROOM_NOPE				= 4,
};

// Object apply locations (the obj->value[] slots). Another separate family -
// values 0-6 collide with both APPLY_ROOM_* and the main APPLY_*. Note
// APPLY_OBJ_PROPERTIES (100) below belongs with this family by name but sits
// far outside the range; kept where the source had it.
enum ApplyObjLocation : int
{
	APPLY_OBJ_NONE				= 0,
	APPLY_OBJ_V0				= 1,
	APPLY_OBJ_V1				= 2,
	APPLY_OBJ_V2				= 3,
	APPLY_OBJ_V3				= 4,
	APPLY_OBJ_V4				= 5,
	APPLY_OBJ_WEIGHT			= 6,
};

// Cabal identity ordinal, in char_data::cabal. NOTE: CABAL_LEADER (defined among
// the MAX_ constants below) is NOT part of this family - it is an induct rank
// compared against pcdata->induct, and keeps its #define. Wire format - never
// renumber. Gap at 7.
enum Cabal : int
{
	CABAL_NONE					= 0,
	CABAL_GUILD					= 1,
	CABAL_SCION					= 2,
	CABAL_HORDE					= 3,
	CABAL_BOUNTY				= 4,
	CABAL_THEATRE				= 5,
	CABAL_PHALANX				= 6,
	CABAL_NEGATIVE				= 8,
};

// Bit indices into char_data::act, mob_index_data::act and race_data::act.
// NOTE: char_data::act carries PlrFlag as well - the two families are disjoint
// interpretations of one word, chosen by is_npc(). See flags.c's "act" vs "plr".
// The values are a wire format (player files store them as letters keyed to the
// index; area files resolve names through act_flags[]), so they may be renamed
// but never renumbered. Gaps at 12, 13 and 23 are retired bits - do not reuse
// without checking existing player and area data.
enum ActFlag : int
{
	ACT_IS_NPC					= 0,	// Auto set for mobs
	ACT_SENTINEL				= 1,	// Stays in one room
	ACT_SCAVENGER				= 2,	// Picks up objects
	ACT_WARD_MOB				= 3,	// Ward mobs
	ACT_WANDER					= 4,	// wanders
	ACT_AGGRESSIVE				= 5,	// Attacks PC's
	ACT_STAY_AREA				= 6,	// Won't leave area
	ACT_WIMPY					= 7,
	ACT_PET						= 8,	// Auto set for pets
	ACT_TRAIN					= 9,	// Can train PC's
	ACT_PRACTICE				= 10,	// Can practice PC's
	ACT_SMARTTRACK				= 11,	// Will use pathfinding
	ACT_UNDEAD					= 14,
	ACT_INNER_GUARDIAN			= 15,	// yay.
	ACT_CLERIC					= 16,
	ACT_MAGE					= 17,
	ACT_INTELLIGENT 			= 18,
	ACT_FAST_TRACK				= 19,
	ACT_NOALIGN					= 20,
	ACT_NOPURGE					= 21,
	ACT_OUTDOORS				= 22,
	ACT_INDOORS					= 24,
	ACT_GUILDGUARD				= 25,
	ACT_IS_HEALER				= 26,
	ACT_GAIN					= 27,
	ACT_UPDATE_ALWAYS			= 28,
	ACT_DETECT_SPECIAL			= 29,
	ACT_BANKER					= 30,
	ACT_NOCTURNAL				= 31,
	ACT_DIURNAL					= 32,
	ACT_FASTWANDER				= 33,
	ACT_LAW						= 34,
};

// Damage classes - an ordinal passed around as an int and switched on. Wire
// format - rename freely, never renumber.
enum DamageType : int
{
	DAM_NONE					= 0,
	DAM_BASH					= 1,
	DAM_PIERCE					= 2,
	DAM_SLASH					= 3,
	DAM_FIRE					= 4,
	DAM_COLD					= 5,
	DAM_LIGHTNING				= 6,
	DAM_ACID					= 7,
	DAM_POISON					= 8,
	DAM_NEGATIVE				= 9,
	DAM_HOLY					= 10,
	DAM_ENERGY					= 11,
	DAM_MENTAL					= 12,
	DAM_DISEASE					= 13,
	DAM_DROWNING				= 14,
	DAM_LIGHT					= 15,
	DAM_OTHER					= 16,
	DAM_CHARM					= 17,
	DAM_SOUND					= 18,
	DAM_TRUESTRIKE				= 19,
};

// Offensive/assist bit indices into char_data::off_flags and
// mob_index_data::off_flags. One contiguous 0-26 numbering carrying five name
// prefixes (OFF_, ASSIST_, NO_, STATIC_, SPAM_) - they are a single family that
// was never given a single prefix, not distinct families. Kept as one enum with
// the original names. Wire format - rename freely, never renumber.
enum OffFlag : int
{
	OFF_AREA_ATTACK				= 0,
	OFF_BACKSTAB				= 1,
	OFF_BASH					= 2,
	OFF_BERSERK					= 3,
	OFF_DISARM					= 4,
	OFF_DODGE					= 5,
	OFF_FADE					= 6,	// UNUSED
	OFF_FAST					= 7,
	OFF_KICK					= 8,
	OFF_KICK_DIRT				= 9,
	OFF_PARRY					= 10,
	OFF_RESCUE					= 11,
	OFF_TAIL					= 12,
	OFF_TRIP					= 13,
	OFF_CRUSH					= 14,
	ASSIST_ALL					= 15,
	ASSIST_ALIGN				= 16,
	ASSIST_RACE					= 17,
	ASSIST_PLAYERS				= 18,
	ASSIST_GUARD				= 19,
	ASSIST_VNUM					= 20,
	NO_TRACK					= 21,
	STATIC_TRACKING				= 22,
	SPAM_MURDER					= 23,
	OFF_INTIMIDATED				= 24,
	OFF_UNDEAD_DRAIN			= 25,	// True undead drain, very powerful
	ASSIST_GROUP				= 26,
};

// Damage-susceptibility result ordinal (check_immune etc.). Wire format -
// never renumber.
enum Susceptibility : int
{
	IS_NORMAL					= 0,
	IS_IMMUNE					= 1,
	IS_RESISTANT				= 2,
	IS_VULNERABLE				= 3,
};

// Immunity bit indices, into char_data::imm_flags (and affect_data::bitvector
// where where == TO_IMMUNE). IMM_/RES_/VULN_ are three value-identical parallel
// families - same damage domain, three storages. The flag tables and the area
// file format route all three through the imm_flags table (flags.c, db2.c), so
// they are effectively one namespace on disk; they are kept as three enums here
// only because their C names differ (IMM_SUMMON vs RES_SUMMON) and renaming call
// sites is out of scope. Wire format - never renumber.
enum ImmuneFlag : int
{
	IMM_SUMMON					= 0,
	IMM_CHARM					= 1,
	IMM_MAGIC					= 2,
	IMM_WEAPON					= 3,
	IMM_BASH					= 4,
	IMM_PIERCE					= 5,
	IMM_SLASH					= 6,
	IMM_FIRE					= 7,
	IMM_COLD					= 8,
	IMM_LIGHTNING				= 9,
	IMM_ACID					= 10,
	IMM_POISON					= 11,
	IMM_NEGATIVE				= 12,
	IMM_HOLY					= 13,
	IMM_ENERGY					= 14,
	IMM_MENTAL					= 15,
	IMM_DISEASE					= 16,
	IMM_DROWNING				= 17,
	IMM_LIGHT					= 18,
	IMM_SOUND					= 19,
	IMM_INTERNAL				= 20,
	IMM_MITHRIL					= 23,
	IMM_SILVER					= 24,
	IMM_IRON					= 25,
	IMM_SLEEP					= 26,	// no RES_/VULN_ counterpart
};

// Resistance bit indices, into char_data::res_flags. Parallel to ImmuneFlag -
// see the note there. Stops at 25; no RES_SLEEP.
enum ResistFlag : int
{
	RES_SUMMON					= 0,
	RES_CHARM					= 1,
	RES_MAGIC					= 2,
	RES_WEAPON					= 3,
	RES_BASH					= 4,
	RES_PIERCE					= 5,
	RES_SLASH					= 6,
	RES_FIRE					= 7,
	RES_COLD					= 8,
	RES_LIGHTNING				= 9,
	RES_ACID					= 10,
	RES_POISON					= 11,
	RES_NEGATIVE				= 12,
	RES_HOLY					= 13,
	RES_ENERGY					= 14,
	RES_MENTAL					= 15,
	RES_DISEASE					= 16,
	RES_DROWNING				= 17,
	RES_LIGHT					= 18,
	RES_SOUND					= 19,
	RES_INTERNAL				= 20,
	RES_MITHRIL					= 23,
	RES_SILVER					= 24,
	RES_IRON					= 25,
};

// Vulnerability bit indices, into char_data::vuln_flags. Parallel to ImmuneFlag
// - see the note there. Stops at 25; no VULN_SLEEP.
enum VulnFlag : int
{
	VULN_SUMMON					= 0,
	VULN_CHARM					= 1,
	VULN_MAGIC					= 2,
	VULN_WEAPON					= 3,
	VULN_BASH					= 4,
	VULN_PIERCE					= 5,
	VULN_SLASH					= 6,
	VULN_FIRE					= 7,
	VULN_COLD					= 8,
	VULN_LIGHTNING				= 9,
	VULN_ACID					= 10,
	VULN_POISON					= 11,
	VULN_NEGATIVE				= 12,
	VULN_HOLY					= 13,
	VULN_ENERGY					= 14,
	VULN_MENTAL					= 15,
	VULN_DISEASE				= 16,
	VULN_DROWNING				= 17,
	VULN_LIGHT					= 18,
	VULN_SOUND					= 19,
	VULN_INTERNAL				= 20,
	VULN_MITHRIL				= 23,
	VULN_SILVER					= 24,
	VULN_IRON					= 25,
};

// Body-form bit indices into char_data::form (and race_type::form). Note this
// is a DIFFERENT field from char_data::act despite magic.c:1408 testing
// ACT_UNDEAD against it. Wire format - never renumber.
enum FormFlag : int
{
	// material form
	FORM_EDIBLE					= 0,
	FORM_POISON					= 1,
	FORM_MAGICAL				= 2,
	FORM_INSTANT_DECAY			= 3,
	FORM_OTHER					= 4,	// defined by material bit

	// actual form
	FORM_ANIMAL					= 6,
	FORM_SENTIENT				= 7,
	FORM_UNDEAD					= 8,
	FORM_CONSTRUCT				= 9,
	FORM_MIST					= 10,
	FORM_INTANGIBLE				= 11,

	FORM_BIPED					= 12,
	FORM_AQUATIC				= 13,
	FORM_INSECT					= 14,
	FORM_SPIDER					= 15,
	FORM_CRUSTACEAN				= 16,
	FORM_WORM					= 17,
	FORM_BLOB					= 18,

	FORM_MAMMAL					= 21,
	FORM_BIRD					= 22,
	FORM_REPTILE				= 23,
	FORM_SNAKE					= 24,
	FORM_DRAGON					= 25,
	FORM_AMPHIBIAN				= 26,
	FORM_FISH					= 27,
	FORM_COLD_BLOOD				= 28,
	FORM_NOSPEECH				= 29,
};

// Body-part bit indices into char_data::parts (and race_type::parts). Wire
// format - rename freely, never renumber. Gaps at 17-19.
enum PartFlag : int
{
	PART_HEAD					= 0,
	PART_ARMS					= 1,
	PART_LEGS					= 2,
	PART_HEART					= 3,
	PART_BRAINS					= 4,
	PART_GUTS					= 5,
	PART_HANDS					= 6,
	PART_FEET					= 7,
	PART_FINGERS				= 8,
	PART_EAR					= 9,
	PART_EYE					= 10,
	PART_LONG_TONGUE			= 11,
	PART_EYESTALKS				= 12,
	PART_TENTACLES				= 13,
	PART_FINS					= 14,
	PART_WINGS					= 15,
	PART_TAIL					= 16,

	// for combat
	PART_CLAWS					= 20,
	PART_FANGS					= 21,
	PART_HORNS					= 22,
	PART_SCALES					= 23,
	PART_TUSKS					= 24,
};

// Bit indices for character affects. Reached through several storages:
// char_data::affected_by, affect_data::bitvector (when where == TO_AFFECTS),
// and race_type::aff[] - which holds bit INDICES in a plain array, not a bit
// vector. act_move.c:3149 confuses the two.
// Wire format - rename freely, never renumber.
enum AffectFlag : int
{
	AFF_BLIND					= 0,
	AFF_INVISIBLE				= 1,
	AFF_DETECT_EVIL				= 2,
	AFF_DETECT_INVIS			= 3,
	AFF_DETECT_MAGIC			= 4,
	AFF_DETECT_HIDDEN			= 5,
	AFF_DETECT_GOOD				= 6,
	AFF_SANCTUARY				= 7,
	AFF_DETECT_CAMO				= 8,
	AFF_INFRARED				= 9,	// unused!
	AFF_CURSE					= 10,
	AFF_CAMOUFLAGE				= 11,
	AFF_POISON					= 12,
	AFF_PROTECTION				= 13,
	AFF_RAGE					= 14,
	AFF_SNEAK					= 15,
	AFF_HIDE					= 16,
	AFF_SLEEP					= 17,
	AFF_CHARM					= 18,
	AFF_FLYING					= 19,
	AFF_PASS_DOOR				= 20,
	AFF_HASTE					= 21,
	AFF_CALM					= 22,
	AFF_PLAGUE					= 23,
	AFF_PERMANENT				= 24,
	AFF_DARK_VISION				= 25,
	AFF_BERSERK					= 26,
	AFF_WATERBREATH				= 27,
	AFF_REGENERATION			= 28,
	AFF_SLOW					= 29,
	AFF_NOSHOW					= 30,
};

// Bit indices into room_index_data::affected_by, and into
// room_affect_data::bitvector when where == TO_ROOM_AFFECTS.
//
// These reuse AffectFlag's numbering space but are NOT aliases of it: this is a
// separate family on separate storage. Five of the six happen to share a name
// with their AffectFlag counterpart (CURSE, POISON, SLEEP, PLAGUE, SLOW) and
// the same bit, but AFF_ROOM_RANDOMIZER == 0 collides with AFF_BLIND == 0 while
// meaning something completely unrelated. Kept as its own enum for that reason.
enum AffectRoomFlag : int
{
	AFF_ROOM_RANDOMIZER			= 0,
	AFF_ROOM_CURSE				= 10,
	AFF_ROOM_POISON				= 12,
	AFF_ROOM_SLEEP				= 17,
	AFF_ROOM_PLAGUE				= 23,
	AFF_ROOM_SLOW				= 29,
};

// Bit indices into obj_data::affected_by, and into obj_affect_data::bitvector
// when where == TO_OBJ_AFFECTS. A third family in AffectFlag's numbering space,
// on a third storage. One member so far.
enum AffectObjFlag : int
{
	AFF_OBJ_BURNING				= 0,
};

// Sex ordinal, in char_data::sex. Wire format - never renumber.
enum Sex : int
{
	SEX_NEUTRAL					= 0,
	SEX_MALE					= 1,
	SEX_FEMALE					= 2,
};

// Armor-class index into char_data::armor[]. Wire format - never renumber.
enum ArmorClass : int
{
	AC_PIERCE					= 0,
	AC_BASH						= 1,
	AC_SLASH					= 2,
	AC_EXOTIC					= 3,
};

// Index into a dice spec's value triple (number/type/bonus). Wire format -
// never renumber.
enum DiceField : int
{
	DICE_NUMBER					= 0,
	DICE_TYPE					= 1,
	DICE_BONUS					= 2,
};

// Creature size, an ordinal in char_data::size; compared relationally. Wire
// format - never renumber. Kept ascending so size comparisons hold.
enum Size : int
{
	SIZE_TINY					= 0,
	SIZE_SMALL					= 1,
	SIZE_MEDIUM					= 2,
	SIZE_LARGE					= 3,
	SIZE_HUGE					= 4,
	SIZE_GIANT					= 5,
	SIZE_IMMENSE				= 6,
};

// Item TYPES - an ordinal scalar stored in obj_data::item_type (a short), not
// a bit field. Compared with == and switched on; never passed to IS_SET.
//
// The ITEM_ prefix covers THREE unrelated families (types here, extra flags and
// wear flags below) whose values collide: ITEM_LIGHT == 1 is a type,
// ITEM_HUM == 1 is an extra-flag bit, ITEM_WEAR_FINGER == 1 is a wear bit.
// They are separate enums for that reason. Names are unchanged, so no call site
// moves; the split is documentation the compiler happens to hold.
// Wire format (area files) - rename freely, never renumber.
enum ItemType : int
{
	ITEM_LIGHT					= 1,
	ITEM_SCROLL					= 2,
	ITEM_WAND					= 3,
	ITEM_STAFF					= 4,
	ITEM_WEAPON					= 5,
	ITEM_NULL6					= 6,
	ITEM_DICE					= 7,
	ITEM_TREASURE				= 8,
	ITEM_ARMOR					= 9,
	ITEM_POTION					= 10,
	ITEM_CLOTHING				= 11,
	ITEM_FURNITURE				= 12,
	ITEM_TRASH					= 13,
	ITEM_CONTAINER				= 15,
	ITEM_DRINK_CON				= 17,
	ITEM_KEY					= 18,
	ITEM_FOOD					= 19,
	ITEM_MONEY					= 20,
	ITEM_BOAT					= 22,
	ITEM_CORPSE_NPC				= 23,
	ITEM_CORPSE_PC				= 24,
	ITEM_FOUNTAIN				= 25,
	ITEM_PILL					= 26,
	ITEM_PROTECT				= 27,
	ITEM_MAP					= 28,
	ITEM_PORTAL					= 29,
	ITEM_WARP_STONE				= 30,
	ITEM_ROOM_KEY				= 31,
	ITEM_GEM					= 32,
	ITEM_JEWELRY				= 33,
	ITEM_CAMPFIRE				= 34,
	ITEM_CABAL_ITEM				= 35,
	ITEM_SKELETON				= 36,
	ITEM_URN					= 37,
	ITEM_GRAVITYWELL			= 38,
	ITEM_BOOK					= 39,
	ITEM_PEN					= 40,
	ITEM_ALTAR					= 41,
	// 42 was ITEM_DONATION_PIT - retired. Note ITEM_DONATION_PIT now exists as
	// an *extra flag* with value 33, which is a different family entirely.
	ITEM_STONE					= 43,
};

// Extra flags - bit indices into obj_data::extra_flags, and into
// obj_index_data's affect bitvector where ITEM_EVIL / ITEM_BURN_PROOF are set
// (magic.c). Distinct from ItemType above despite the shared prefix.
// Wire format - rename freely, never renumber. Gap at 23.
//
// CORPSE_NO_ANIMATE is a member of THIS family despite its different prefix:
// value 27 sits between ITEM_BRAND (26) and ITEM_ANTI_LAWFUL (28), and it is
// set on extra_flags at fight.c:3025. Note characterClasses/necro.c sets and
// tests it on *wear_flags* instead, where bit 27 is outside the wear range
// (0-18).
enum ItemExtraFlag : int
{
	ITEM_GLOW					= 0,
	ITEM_HUM					= 1,
	ITEM_DARK					= 2,
	ITEM_NOSHOW					= 3,
	ITEM_EVIL					= 4,
	ITEM_INVIS					= 5,
	ITEM_MAGIC					= 6,
	ITEM_NODROP					= 7,
	ITEM_BLESS					= 8,
	ITEM_ANTI_GOOD				= 9,
	ITEM_ANTI_EVIL				= 10,
	ITEM_ANTI_NEUTRAL			= 11,
	ITEM_NOREMOVE				= 12,
	ITEM_INVENTORY				= 13,
	ITEM_NOPURGE				= 14,
	ITEM_ROT_DEATH				= 15,
	ITEM_VIS_DEATH				= 16,
	ITEM_FIXED					= 17,
	ITEM_NODISARM				= 18,
	ITEM_NOLOCATE				= 19,
	ITEM_MELT_DROP				= 20,
	ITEM_UNDER_CLOTHES			= 21,
	ITEM_SELL_EXTRACT			= 22,
	ITEM_BURN_PROOF				= 24,
	ITEM_NOUNCURSE				= 25,
	ITEM_BRAND					= 26,
	CORPSE_NO_ANIMATE			= 27,
	ITEM_ANTI_LAWFUL			= 28,
	ITEM_ANTI_NEUT				= 29,
	ITEM_ANTI_CHAOTIC			= 30,
	ITEM_NO_STASH				= 31,
	ITEM_NO_SAC					= 32,
	ITEM_DONATION_PIT			= 33,
};

// Wear flags - bit indices into obj_data::wear_flags. The third and last
// family under the ITEM_ prefix. Range is 0-18, which is why necro.c's use of
// CORPSE_NO_ANIMATE (27) on this field cannot work (§0.8.1).
// Wire format - rename freely, never renumber.
enum ItemWearFlag : int
{
	ITEM_TAKE					= 0,
	ITEM_WEAR_FINGER			= 1,
	ITEM_WEAR_NECK				= 2,
	ITEM_WEAR_BODY				= 3,
	ITEM_WEAR_HEAD				= 4,
	ITEM_WEAR_LEGS				= 5,
	ITEM_WEAR_FEET				= 6,
	ITEM_WEAR_HANDS				= 7,
	ITEM_WEAR_ARMS				= 8,
	ITEM_WEAR_SHIELD			= 9,
	ITEM_WEAR_ABOUT				= 10,
	ITEM_WEAR_WAIST				= 11,
	ITEM_WEAR_WRIST				= 12,
	ITEM_WEAR_WIELD				= 13,
	ITEM_WEAR_HOLD				= 14,
	ITEM_WEAR_FLOAT				= 15,
	ITEM_WEAR_BRAND				= 16,
	ITEM_WEAR_STRAPPED			= 17,
	ITEM_WEAR_COSMETIC			= 18,	// cosmetic, misc, up to 5/person
};

// Object-restriction criterion ordinal. Wire format - never renumber.
enum RestrictType : int
{
	RESTRICT_OTHER				= 0,
	RESTRICT_CLASS				= 1,
	RESTRICT_RACE				= 2,
	RESTRICT_CABAL				= 3,
};

// Shapeshifter tribe ordinal. Wire format - never renumber.
enum Tribe : int
{
	TRIBE_NONE					= 0,
	TRIBE_BOAR					= 1,
	TRIBE_WOLF					= 2,
	TRIBE_BEAR					= 3,
	TRIBE_HAWK					= 4,
	TRIBE_LION					= 5,
	TRIBE_ELK					= 6,
	TRIBE_JACKAL				= 7,
	TRIBE_FOX					= 8,
	TRIBE_BULL					= 9,
	TRIBE_PANTHER				= 10,
};

// Paladin-order ordinal. Wire format - never renumber.
enum PaladinOrder : int
{
	PALADIN_NONE				= 0,
	PALADIN_PROTECTOR			= 1,
	PALADIN_CRUSADER			= 2,
};

// Combat-style ordinal. STYLE_NONE is the -1 sentinel (hence : int). Wire
// format - never renumber.
enum CombatStyle : int
{
	STYLE_NONE					= -1,
	STYLE_GLADIATOR				= 0,
	STYLE_BARBARIAN				= 1,
	STYLE_DUELIST				= 2,
	STYLE_SKIRMISHER			= 3,
	STYLE_DRAGOON				= 4,
	STYLE_TACTICIAN				= 5,
};

// Weapon CLASS - an ordinal scalar in obj->value[0], compared with ==. This is
// a separate family from the weapon-flag bits below, whose values 0-10 collide
// with these completely. (The source's own "weapon class" / "weapon types"
// comments have these two backwards; the names are kept, the labels corrected.)
enum WeaponClass : int
{
	WEAPON_EXOTIC				= 0,
	WEAPON_SWORD				= 1,
	WEAPON_DAGGER				= 2,
	WEAPON_SPEAR				= 3,
	WEAPON_MACE					= 4,
	WEAPON_AXE					= 5,
	WEAPON_FLAIL				= 6,
	WEAPON_WHIP					= 7,
	WEAPON_POLEARM				= 8,
	WEAPON_STAFF				= 9,
	WEAPON_HAND					= 10,
};

// Weapon FLAGS - bit indices tested with IS_SET_OLD on obj->value[4] (a scalar
// int field, hence the _OLD operators). Separate family from WeaponClass above.
enum WeaponFlag : int
{
	WEAPON_FLAMING				= 0,
	WEAPON_FROST				= 1,
	WEAPON_VAMPIRIC				= 2,
	WEAPON_SHARP				= 3,
	WEAPON_VORPAL				= 4,
	WEAPON_TWO_HANDS			= 5,
	WEAPON_SHOCKING				= 6,
	WEAPON_POISON				= 7,
	WEAPON_AVENGER				= 8,
	WEAPON_SHADOWBANE			= 9,
	WEAPON_LIGHTBRINGER			= 10,
};

// Portal gate flags - bit indices tested with IS_SET_OLD on a portal's
// obj->value[] scalar. Wire format - rename freely, never renumber.
enum GateFlag : int
{
	GATE_NORMAL_EXIT			= 0,
	GATE_NOCURSE				= 1,
	GATE_GOWITH					= 2,
	GATE_BUGGY					= 3,
	GATE_RANDOM					= 4,
};

// Furniture position flags - bit indices tested with IS_SET_OLD on a piece of
// furniture's obj->value[2] (act_info.c). One family across six prefixes
// (STAND_/SIT_/REST_/SLEEP_/PUT_/LOUNGE_), a single 0-16 numbering. Kept as one
// enum with the original names. Wire format - rename freely, never renumber.
enum FurniturePosition : int
{
	STAND_AT					= 0,
	STAND_ON					= 1,
	STAND_IN					= 2,
	SIT_AT						= 3,
	SIT_ON						= 4,
	SIT_IN						= 5,
	REST_AT						= 6,
	REST_ON						= 7,
	REST_IN						= 8,
	SLEEP_AT					= 9,
	SLEEP_ON					= 10,
	SLEEP_IN					= 11,
	PUT_AT						= 12,
	PUT_ON						= 13,
	PUT_IN						= 14,
	PUT_INSIDE					= 15,
	LOUNGE_ON					= 16,
};

// The main apply-location family: affect_data::location, an ordinal scalar
// (short), not a bit field. Compared with == and switched on. APPLY_ROOM_* and
// APPLY_OBJ_* (above) are separate families on separate location fields.
// Wire format - rename freely, never renumber.
enum ApplyLocation : int
{
	APPLY_NONE					= 0,
	APPLY_STR					= 1,
	APPLY_DEX					= 2,
	APPLY_INT					= 3,
	APPLY_WIS					= 4,
	APPLY_CON					= 5,
	APPLY_SEX					= 6,
	APPLY_CLASS					= 7,
	APPLY_LUCK					= 8,	// UNUSED!!
	APPLY_AGE					= 9,
	APPLY_HEIGHT				= 10,
	APPLY_WEIGHT				= 11,
	APPLY_MANA					= 12,
	APPLY_HIT					= 13,
	APPLY_MOVE					= 14,
	APPLY_GOLD					= 15,
	APPLY_EXP					= 16,
	APPLY_AC					= 17,
	APPLY_HITROLL				= 18,
	APPLY_DAMROLL				= 19,
	APPLY_SAVES					= 20,
	APPLY_SAVING_PARA			= 21,
	APPLY_SAVING_ROD			= 22,
	APPLY_SAVING_PETRI			= 23,
	APPLY_SAVING_BREATH			= 24,
	APPLY_SAVING_SPELL			= 25,
	APPLY_SPELL_AFFECT			= 26,
	APPLY_CARRY_WEIGHT			= 27,
	APPLY_DEFENSE				= 28,
	APPLY_REGENERATION			= 29,
	APPLY_SIZE					= 30,
	APPLY_ENERGYSTATE			= 31,
	APPLY_DAM_MOD				= 32,
	APPLY_LEGS					= 33,
	APPLY_ARMS					= 34,
	APPLY_BEAUTY				= 35,
	APPLY_ALIGNMENT				= 36,
	APPLY_ETHOS					= 37,
	// APPLY_OBJ_PROPERTIES == 100 lived here originally, in the main family's
	// numbering despite its APPLY_OBJ_ name. Kept in this enum, at its value.
	APPLY_OBJ_PROPERTIES		= 100,
};

// Spell/effect modifier type, an ordinal. MOD_NONE is the -1 sentinel, hence
// the required : int underlying type. Wire format - never renumber.
enum ModType : int
{
	MOD_NONE					= -1,
	MOD_VISION					= 0,
	MOD_MOVEMENT				= 1,
	MOD_TOUGHNESS				= 2,
	MOD_SPEED					= 3,
	MOD_LEVITATION				= 4,
	MOD_VISIBILITY				= 5,
	MOD_PHASE					= 6,
	MOD_CONC					= 7,
	MOD_PROTECTION				= 8,
	MOD_APPEARANCE				= 9,
	MOD_HEARING					= 10,
	MOD_PERCEPTION				= 11,
	MOD_RESISTANCE				= 12,
	MOD_ENERGY_STATE			= 13,
	MOD_SPEECH					= 14,
	MOD_REGEN					= 15,
	MOD_WARDROBE				= 16,
};

// Container flags - bit indices tested with IS_SET_OLD on a container's
// obj->value[1] scalar. Wire format - rename freely, never renumber.
enum ContainerFlag : int
{
	CONT_CLOSEABLE				= 0,
	CONT_PICKPROOF				= 1,
	CONT_CLOSED					= 2,
	CONT_LOCKED					= 3,
	CONT_PUT_ON					= 4,
};

// Wield-hand ordinal. Wire format - never renumber.
enum WieldType : int
{
	WIELD_ONE					= 1,
	WIELD_TWO					= 2,
	WIELD_PRIMARY				= 3,
};

// Bit indices into room_index_data::room_flags. Also reachable through
// room_affect_data::bitvector via TO_ROOM_FLAGS (handler.c).
// Wire format - rename freely, never renumber. Gaps at 1, 5-8, 24 and 26 are
// retired bits; check existing area data before reusing any of them.
//
// NOTE: the ROOM_VNUM_* constants above are NOT part of this family. They are
// room virtual numbers (values into the thousands), not bit indices, and they
// share the prefix only by accident. ROOM_VNUM_LIMBO == 2 collides with
// ROOM_NO_MOB == 2 harmlessly because the two are never used on the same thing.
enum RoomFlag : int
{
	ROOM_DARK					= 0,
	ROOM_NO_MOB					= 2,
	ROOM_INDOORS				= 3,
	ROOM_NO_CONSECRATE			= 4,
	ROOM_PRIVATE				= 9,
	ROOM_SAFE					= 10,
	ROOM_SOLITARY				= 11,
	ROOM_PET_SHOP				= 12,
	ROOM_NO_RECALL				= 13,
	ROOM_IMP_ONLY				= 14,
	ROOM_GODS_ONLY				= 15,
	ROOM_HEROES_ONLY			= 16,
	ROOM_NEWBIES_ONLY			= 17,
	ROOM_LAW					= 18,
	ROOM_NOWHERE				= 19,
	ROOM_NO_GATE				= 20,
	ROOM_SILENCE				= 21,
	ROOM_NO_SUMMON_TO			= 22,
	ROOM_NO_SUMMON_FROM			= 23,
	ROOM_NO_ALARM				= 25,
	ROOM_FORCE_DUEL				= 27,
	ROOM_NO_MAGIC				= 28,
	ROOM_AREA_EXPLORE			= 29,	// Don't use - Use area flags instead
	ROOM_NO_COMMUNE				= 30,
};

// Exit flags - bit indices tested with IS_SET_OLD on exit_data::exit_info (a
// scalar int). Wire format - rename freely, never renumber.
enum ExitFlag : int
{
	EX_ISDOOR					= 0,
	EX_CLOSED					= 1,
	EX_LOCKED					= 2,
	EX_PICKPROOF				= 3,
	EX_NOPASS					= 4,
	EX_NOCLOSE					= 5,
	EX_NOLOCK					= 6,
	EX_NOBASH					= 7,
	EX_NONOBVIOUS				= 8,
	EX_TRANSLUCENT				= 9,
	EX_JAMMED					= 10,
};

// Area sector/category ordinal. Wire format - never renumber.
enum AreaSector : int
{
	ARE_NORMAL					= 0,
	ARE_ROAD_RIVER				= 1,
	ARE_CABAL					= 2,
	ARE_QUEST					= 3,
	ARE_CITY					= 4,
	ARE_UNOPENED				= 5,
	ARE_SHRINE					= 6,
};

// Sector (terrain) types, an ordinal in room_index_data::sector_type. Note the
// deliberate collision: SECT_INSIDE and SECT_UNUSED are both 7. SECT_MAX (21) is
// a count/sentinel, not a terrain. Wire format - never renumber.
enum SectorType : int
{
	SECT_INSIDE					= 7,
	SECT_CITY					= 1,
	SECT_FIELD					= 2,
	SECT_FOREST					= 3,
	SECT_HILLS					= 4,
	SECT_MOUNTAIN				= 5,
	SECT_WATER					= 6,
	SECT_UNUSED					= 7,
	SECT_UNDERWATER				= 8,
	SECT_AIR					= 9,
	SECT_DESERT					= 10,
	SECT_ROAD					= 11,
	SECT_CONFLAGRATION			= 12,
	SECT_BURNING				= 13,
	SECT_TRAIL					= 14,
	SECT_SWAMP					= 15,
	SECT_PARK					= 16,
	SECT_VERTICAL				= 17,
	SECT_ICE					= 18,
	SECT_SNOW					= 19,
	SECT_CAVE					= 20,
	SECT_MAX					= 21,	// count/sentinel, not a terrain
};

// Trap effect ordinal. Wire format - never renumber.
enum TrapType : int
{
	TRAP_NONE					= 0,
	TRAP_DART					= 1,
	TRAP_PDART					= 2,
	TRAP_FIREBALL				= 3,
	TRAP_LIGHTNING				= 4,
	TRAP_SLEEPGAS				= 5,
	TRAP_POISONGAS				= 6,
	TRAP_ACID					= 7,
	TRAP_PIT					= 8,
	TRAP_BOULDER				= 9,
	TRAP_DRAIN					= 10,
};

// Alignment SELECTION values (creation/OLC choices), an ordinal. Distinct from
// the raw alignment scale ALIGN_NEUTRAL sits on above. ALIGN_NONE is the -1
// sentinel (hence : int). Wire format - never renumber.
enum AlignmentChoice : int
{
	ALIGN_NONE					= -1,
	ALIGN_ANY					= 0,
	ALIGN_GN					= 1,
	ALIGN_NE					= 2,
	ALIGN_GE					= 3,
	ALIGN_G						= 4,
	ALIGN_N						= 5,
	ALIGN_E						= 6,
};

// Ethos SELECTION values (creation/OLC choices), an ordinal. Distinct from the
// raw ethos scale ETHOS_NEUTRAL sits on. ETHOS_NONE is the -1 sentinel.
enum EthosChoice : int
{
	ETHOS_NONE					= -1,
	ETHOS_ANY					= 0,
	ETHOS_LN					= 1,
	ETHOS_NC					= 2,
	ETHOS_LC					= 3,
	ETHOS_L						= 4,
	ETHOS_N						= 5,
	ETHOS_C						= 6,
};

// Guild ordinal (room guild-guard associations). Wire format - never renumber.
enum Guild : int
{
	GUILD_WARRIOR				= 1,
	GUILD_THIEF					= 2,
	GUILD_CLERIC				= 3,
	GUILD_PALADIN				= 4,
	GUILD_ANTI_PALADIN			= 5,
	GUILD_RANGER				= 6,
	GUILD_MONK					= 7,
	GUILD_SHAPESHIFTER			= 8,
	GUILD_ASSASSIN				= 9,
	GUILD_NECROMANCER			= 10,
	GUILD_SORCERER				= 11,
};

// Character class, an ordinal in char_data::Class()/class. The main CLASS_
// family. Wire format - rename freely, never renumber.
enum CharClass : int
{
	CLASS_NONE					= 0,
	CLASS_WARRIOR				= 1,
	CLASS_THIEF					= 2,
	CLASS_ZEALOT				= 3,
	CLASS_PALADIN				= 4,
	CLASS_ANTI_PALADIN			= 5,
	CLASS_RANGER				= 6,
	CLASS_ASSASSIN				= 7,
	CLASS_SHAPESHIFTER			= 8,
	CLASS_HEALER				= 9,
	CLASS_NECROMANCER			= 10,
	CLASS_SORCERER				= 11,
};

// Worn-equipment slot index into char_data::equipment[]. WEAR_NONE is the -1
// sentinel (hence : int); WEAR_DUAL_WIELD and WEAR_FLOAT deliberately share slot
// 18. Wire format - rename freely, never renumber.
enum WearLocation : int
{
	WEAR_NONE					= -1,
	WEAR_LIGHT					= 0,
	WEAR_FINGER_L				= 1,
	WEAR_FINGER_R				= 2,
	WEAR_NECK_1					= 3,
	WEAR_NECK_2					= 4,
	WEAR_BODY					= 5,
	WEAR_HEAD					= 6,
	WEAR_LEGS					= 7,
	WEAR_FEET					= 8,
	WEAR_HANDS					= 9,
	WEAR_ARMS					= 10,
	WEAR_SHIELD					= 11,
	WEAR_ABOUT					= 12,
	WEAR_WAIST					= 13,
	WEAR_WRIST_L				= 14,
	WEAR_WRIST_R				= 15,
	WEAR_WIELD					= 16,
	WEAR_HOLD					= 17,
	WEAR_DUAL_WIELD				= 18,
	WEAR_FLOAT					= 18,
	WEAR_BRAND					= 19,
	WEAR_STRAPPED				= 20,
	WEAR_COSMETIC				= 21,
};

// damage_new() flag arguments. Three independent two-value families, passed
// positionally. Kept as three enums matching their argument slots.
enum HitBlockable : int
{
	HIT_UNBLOCKABLE				= 0,
	HIT_BLOCKABLE				= 1,
};

enum HitSpecials : int
{
	HIT_NOSPECIALS				= 0,
	HIT_SPECIALS				= 1,
};

enum HitModifier : int
{
	HIT_NOMULT					= 1,
	HIT_NOADD					= 0,
};

// Combat posture, an ordinal spanning -1..1. POSTURE_DEFENSE is -1 (hence : int).
enum Posture : int
{
	POSTURE_OFFENSE				= 1,
	POSTURE_NONE				= 0,
	POSTURE_DEFENSE				= -1,
};

// Condition index into pc_data::condition[]. COND_HUNGRY (50) is an outlier
// value, not adjacent to the rest. Wire format - never renumber.
enum ConditionType : int
{
	COND_DRUNK					= 0,
	COND_FULL					= 1,
	COND_THIRST					= 2,
	COND_HUNGER					= 3,
	COND_STARVING				= 4,
	COND_DEHYDRATED				= 5,
	COND_HUNGRY					= 50,
};

// Character position, an ordinal in char_data::position (a short). Ordering is
// load-bearing: ~96 relational comparisons (position >= POS_RESTING etc.) and it
// is stored in cmd_type::position for the command table. Wire format - never
// renumber. Kept ascending exactly as-is so all comparisons hold.
enum Position : int
{
	POS_DEAD					= 0,
	POS_MORTAL					= 1,
	POS_INCAP					= 2,
	POS_STUNNED					= 3,
	POS_SLEEPING				= 4,
	POS_RESTING					= 5,
	POS_SITTING					= 6,
	POS_FIGHTING				= 7,
	POS_STANDING				= 8,
};

// PK alignment-kill counters. Separate family from the killed[] index pair
// below. Wire format - never renumber.
enum PkKillType : int
{
	PK_KILLS					= 0,
	PK_GOOD						= 1,
	PK_NEUTRAL					= 2,
	PK_EVIL						= 3,
};

// Index into pcdata::killed[]: player kills vs mob kills. One family, two
// prefixes (PK_/MOB_).
enum KilledIndex : int
{
	PK_KILLED					= 0,
	MOB_KILLED					= 1,
};

// Bit indices into char_data::act - the SAME field ActFlag uses, disambiguated
// at runtime by is_npc(). Values collide with ActFlag deliberately and must not
// be reconciled: PLR_AUTOABORT and ACT_SENTINEL are both bit 1 with unrelated
// meanings. Wire format, as ActFlag - rename freely, never renumber.
// Gap at 18: see the "2 bits reserved" note, of which only 19 was ever taken.
enum PlrFlag : int
{
	PLR_IS_NPC					= 0,	// Don't EVER set.

	// RT auto flags
	PLR_AUTOABORT				= 1,
	PLR_AUTOASSIST				= 2,
	PLR_AUTOEXIT				= 3,
	PLR_AUTOLOOT				= 4,
	PLR_AUTOSAC					= 5,
	PLR_AUTOGOLD				= 6,
	PLR_AUTOSPLIT				= 7,
	PLR_COLOR					= 8,
	PLR_IGNORANT				= 9,
	PLR_BETRAYER				= 10,

	// RT personal flags
	PLR_CODER					= 11,
	PLR_HEROIMM					= 12,
	PLR_HOLYLIGHT				= 13,
	PLR_EMPOWERED				= 14,
	PLR_NOVOID					= 15,
	PLR_NOSUMMON				= 16,
	PLR_NOFOLLOW				= 17,

	// 2 bits reserved, S-T
	PLR_NO_TRANSFER				= 19,

	// Bad flags
	PLR_PERMIT					= 20,
	PLR_MORON					= 21,
	PLR_LOG						= 22,
	PLR_DENY					= 23,
	PLR_FREEZE					= 24,
	PLR_THIEF					= 25,
	PLR_KILLER					= 26,
	PLR_CRIMINAL				= 27,
};

// Bit indices into char_data::comm. The values are a wire format - they are
// written to player files as letters keyed to the index (save.c print_flags),
// so they may be renamed but never renumbered. Note the gap at 17.
enum CommFlag : int
{
	COMM_QUIET					= 0,
	COMM_DEAF					= 1,
	COMM_NOWIZ					= 2,
	COMM_NOAUCTION				= 3,
	COMM_NOGOSSIP				= 4,
	COMM_NOQUESTION				= 5,
	COMM_NONEWBIE				= 6,
	COMM_NOCABAL				= 7,
	COMM_NOQUOTE				= 8,
	COMM_SHOUTSOFF				= 9,
	COMM_ANSI					= 10,

	// display flags
	COMM_COMPACT				= 11,
	COMM_BRIEF					= 12,
	COMM_PROMPT					= 13,
	COMM_COMBINE				= 14,
	COMM_TELNET_GA				= 15,
	COMM_SHOW_AFFECTS			= 16,
	COMM_IMMORTAL				= 18,

	// penalties
	COMM_NOEMOTE				= 19,
	COMM_NOSHOUT				= 20,
	COMM_NOTELL					= 21,
	COMM_NOCHANNELS				= 22,
	COMM_BUILDER				= 23,
	COMM_SNOOP_PROOF			= 24,
	COMM_AFK					= 25,
	COMM_ALL_CABALS				= 26,
	COMM_NOSOCKET				= 27,
	COMM_SWITCHSKILLS			= 28,
	COMM_NOBUILDER				= 29,
	COMM_LOTS_O_COLOR			= 30,
};

// Wiznet channel bit indices into char_data::wiznet, and passed as `long flag`
// to wiznet(). Wire format - rename freely, never renumber. Gap at 23.
enum WiznetFlag : int
{
	WIZ_ON						= 0,
	WIZ_TICKS					= 1,
	WIZ_LOGINS					= 2,
	WIZ_SITES					= 3,
	WIZ_LINKS					= 4,
	WIZ_DEATHS					= 5,
	WIZ_RESETS					= 6,
	WIZ_MOBDEATHS					= 7,
	WIZ_FLAGS					= 8,
	WIZ_PENALTIES					= 9,
	WIZ_SACCING					= 10,
	WIZ_LEVELS					= 11,
	WIZ_SECURE					= 12,
	WIZ_SWITCHES					= 13,
	WIZ_SNOOPS					= 14,
	WIZ_RESTORE					= 15,
	WIZ_LOAD					= 16,
	WIZ_NEWBIE					= 17,
	WIZ_PREFIX					= 18,
	WIZ_SPAM					= 19,
	WIZ_CABAL					= 20,
	WIZ_PERCENT					= 21,
	WIZ_LOG						= 22,
	WIZ_OOC						= 23,
	WIZ_DEBUG					= 24,
};

// Devil-patron ordinal. Wire format - never renumber.
enum Devil : int
{
	DEVIL_ASMODEUS				= 0,
	DEVIL_MOLOCH				= 1,
	DEVIL_BAAL					= 2,
	DEVIL_DISPATER				= 3,
	DEVIL_MEPHISTO				= 4,
};

// Lesser-demon name ordinal. MAX_LESSER stays a #define (constexpr commit).
enum LesserDemon : int
{
	LESSER_BARBAS				= 0,
	LESSER_AAMON				= 1,
	LESSER_MALAPHAR				= 2,
	LESSER_FURCAS				= 3,
	LESSER_IPOS					= 4,
};

// Greater-demon name ordinal. Separate family from GreaterDemon-tier below.
enum GreaterDemon : int
{
	GREATER_OZE					= 0,
	GREATER_GAMYGYN				= 1,
	GREATER_OROBAS				= 2,
	GREATER_GERYON				= 3,
	GREATER_CIMERIES			= 4,
};

// Demon tier discriminator (lesser vs greater). LESSER_DEMON/GREATER_DEMON share
// the LESSER_/GREATER_ prefixes with the name families above but are a distinct
// two-value family.
enum DemonTier : int
{
	LESSER_DEMON				= 0,
	GREATER_DEMON				= 1,
};

// Favor/quest-state ordinal. FAVOR_FAILED is the -1 sentinel (hence : int).
enum FavorState : int
{
	FAVOR_NONE					= 0,
	FAVOR_FAILED				= -1,
	FAVOR_IN_PROGRESS			= 1,
	FAVOR_GRANTED				= 2,
};

// Elemental type, an ordinal. Separate from ELE_TYPE_* below (which collide in
// value). Wire format - never renumber.
enum Element : int
{
	ELE_HEAT					= 0,
	ELE_COLD					= 1,
	ELE_AIR						= 2,
	ELE_EARTH					= 3,
	ELE_WATER					= 4,
	ELE_ELECTRICITY				= 5,
	ELE_SMOKE					= 6,
	ELE_MAGMA					= 7,
	ELE_PLASMA					= 8,
	ELE_ACID					= 9,
	ELE_BLIZZARD				= 10,
	ELE_FROST					= 11,
	ELE_CRYSTAL					= 12,
	ELE_ICE						= 13,
	ELE_LIGHTNING				= 14,
	ELE_MIST					= 15,
	ELE_METAL					= 16,
	ELE_OOZE					= 17,
	ELE_NONE					= 18,
};

// Elemental slot type, an ordinal. Separate family from Element above.
enum ElementSlot : int
{
	ELE_TYPE_NONE				= 0,
	ELE_TYPE_PRIMARY			= 1,
	ELE_TYPE_PARA				= 2,
};

// Material optical property. Separate family from MAT_SOLID.. below (collide).
enum MaterialOptical : int
{
	MAT_TRANSLUCENT				= 0,
	MAT_TRANSPARENT				= 1,
	MAT_EDIBLE					= 2,
};

// Material state of matter.
enum MaterialState : int
{
	MAT_SOLID					= 0,
	MAT_LIQUID					= 1,
	MAT_GAS						= 2,
	MAT_PLASMA					= 3,
};

// Speech-act ordinal. Wire format - never renumber.
enum SpeechType : int
{
	SPEECH_SAY					= 0,
	SPEECH_SAYTO				= 1,
	SPEECH_TELL					= 2,
	SPEECH_WHISPER				= 3,
	SPEECH_YELL					= 4,
	SPEECH_EMOTE				= 5,
	SPEECH_ECHO					= 6,
	SPEECH_SING					= 7,
};

// Skill/spell target-type ordinal, in skill_type::target. TAR_END (666) is a
// deliberate sentinel, not adjacent. Wire format - never renumber.
enum SkillTarget : int
{
	TAR_IGNORE					= 0,
	TAR_CHAR_OFFENSIVE			= 1,
	TAR_CHAR_DEFENSIVE			= 2,
	TAR_CHAR_SELF				= 3,
	TAR_OBJ_INV					= 4,
	TAR_OBJ_CHAR_DEF			= 5,
	TAR_OBJ_CHAR_OFF			= 6,
	TAR_DIR						= 7,
	TAR_CHAR_AMBIGUOUS			= 8,
	TAR_CHAR_GENERAL			= 9,
	TAR_END						= 666,	// don't touch
};

// Spell effect target-type ordinal. Separate family from SkillTarget above.
enum TargetType : int
{
	TARGET_CHAR					= 0,
	TARGET_OBJ					= 1,
	TARGET_ROOM					= 2,
	TARGET_NONE					= 3,
	TARGET_RUNE					= 4,
	TARGET_DIR					= 5,
};

// Rune target bitmasks - these are pre-shifted MASKS (1,2,4,8,16), not bit
// indices, tested with IS_SET_OLD directly and compared with <. A third RUNE_
// family, distinct from RuneTarget/RuneTrigger. Wire format - never renumber.
enum RuneMask : int
{
	RUNE_CAST					= 1,
	RUNE_ARMOR					= 2,
	RUNE_WEAPON					= 4,
	RUNE_DOOR					= 8,
	RUNE_ROOM					= 16,
};

// Area flags - bit indices into area_data::area_flags. AREA_NONE and
// AREA_EXPLORE are both 0 by design (alias). Wire format - never renumber.
enum AreaFlag : int
{
	AREA_NONE					= 0,
	AREA_EXPLORE				= 0,	// So far, only that gear returns to newbies
	AREA_NO_NEWBIES				= 1,	// Newbies can't go in
	AREA_UNGHOST				= 2,	// Walking in unghosts you
	AREA_CHANGED				= 3,	// Area has been modified.
	AREA_ADDED					= 4,	// Area has been added to.
	AREA_LOADING				= 5,	// Used for counting in db.c
};

#endif /* ENUMS_H */
