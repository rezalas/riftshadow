//#include "../rift.h" //includes bitvector.h
#include <math.h>
#include "stdlibs/gmacro.h"
#include "stdlibs/bitvector.h"
#include <stdlib.h>

#define ARR(x)		x < 32 ? 0 : 1

CBitvector::CBitvector()
{
	bitvector[0] = 0;
	bitvector[1] = 0;
}

CBitvector::~CBitvector()
{
}

inline bool CBitvector::IsSet(int nBit)
{
	return (bitvector[ARR(nBit)] & (long long)pow(2, nBit));
}

inline void CBitvector::SetBit(int nBit)
{
	return (void) (bitvector[ARR(nBit)] |= (long long)pow(2, nBit));
}

inline void CBitvector::RemoveBit(int nBit)
{
	return (void) (bitvector[ARR(nBit)] &= (long long)pow(2, nBit));
}

void CBitvector::ToggleBit(int nBit)
{
	return IsSet(nBit) ? RemoveBit(nBit) : SetBit(nBit);
}

void CBitvector::ClearBits(void)
{
	bitvector[0] = 0;
	bitvector[1] = 0;
}

void CBitvector::SetToDouble(const char *rowptr)
{
	SetToDouble(atoll(rowptr));
}

void CBitvector::SetToDouble(long long lval)
{
	bitvector[0] = (unsigned long)lval;
	bitvector[1] = (lval>>31);
}
