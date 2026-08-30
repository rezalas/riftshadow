#ifndef MACROS_H
#define MACROS_H

//
// The bit-flag macros.
//
// Extracted from merc.h so consumers — notably the tests/*_tests.c files 
// can pull them in without dragging in all of merc.h. Pure token macros with
// no dependencies; nothing to include.
//

// Array flags -- operate on a long[MAX_BITVECTOR]; the bit is split into a word
// index and an offset. `act`, `affected_by`, `bitvector` and friends.
#define IS_SET(flag, bit)			((flag)[(bit) / 32] &   (1L << ((bit) % 32)))
#define SET_BIT(var, bit)			((var)[(bit) / 32]  |=  (1L << ((bit) % 32)))
#define REMOVE_BIT(var, bit)		((var)[(bit) / 32]  &= ~(1L << ((bit) % 32)))

// Scalar flags -- operate on a single integer field, overwhelmingly obj->value[N]
// (the furniture flags in act_info.c among them). The bit is an absolute shift,
// with no word split. The _OLD suffix names the representation, NOT deprecation:
// these three have 134 live call sites (140 counting TOGGLE_BIT_OLD below).
#define IS_SET_OLD(flag, bit) 		((flag) &   (1L << (bit)))
#define SET_BIT_OLD(var, bit) 		((var)  |=  (1L << (bit)))
#define REMOVE_BIT_OLD(var,bit) 	((var)  &= ~(1L << (bit)))
#define TOGGLE_BIT(var, bit)		(IS_SET(var,bit) ? REMOVE_BIT(var,bit) : SET_BIT(var,bit))
#define TOGGLE_BIT_OLD(var, bit)	(IS_SET_OLD(var,bit) ? REMOVE_BIT_OLD(var,bit) : SET_BIT_OLD(var,bit))

#endif /* MACROS_H */
