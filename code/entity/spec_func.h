#ifndef ENTITY_SPEC_FUNC_H
#define ENTITY_SPEC_FUNC_H

//
// Special-program dispatch slot, embedded by value in the index prototypes.
//

typedef int SPECFUN (long event_vector, ...);

struct spec_func
{
	SPECFUN *func;
	long trapvector;
};

typedef struct spec_func SPEC_FUNC;

#endif /* ENTITY_SPEC_FUNC_H */
