#include "class.h"
#include <stdio.h> //temp
CClass *CClass::first = NULL;

CClass::CClass()
{
}

CClass::~CClass()
{
}

int CClass::GetIndex()
{
	return index;
}

CClass *CClass::GetClass(int nIndex)
{
	/* we subtract 1 because CLASS_ANTI is 1 to differentiate from CLASS_NONE,
	* but it's actually the 0th element in the array since there's no actual
	* CClass None */
	/* ignore that, no we don't */
	//return (CClass *)((char*)first + (nIndex) * sizeof(CClass));
	for(CClass *j = CClass::first; j; j = j->next)
		if(j->index == nIndex)
			return j;
	return CClass::first;
}

CClass * CClass::operator[](int nIndex)
{
	return GetClass(nIndex);
	//return (CClass *)((char*)first + (nIndex) * sizeof(CClass));
}

int CClass::Lookup (const char *name)
{
	int nclass;
	CClass *aclass;
	for (aclass = first; aclass; aclass = aclass->next)
		if (aclass->name.PrefixMatch(name))
			return aclass->index;

	return -1;
}

