#include "class.h"
#include "../mud.h"
#include "../repositories/classrepository.h"

CClass *CClass::first = nullptr;

CClass::CClass()
{
}

/// Loads class_table into the static CClass linked list.
void CClass::LoadClassTable()
{
	auto classes = ClassRepository(RS.Db).FindAllOrdered();

	CClass *lastp = nullptr;
	int i = 0;
	for (const auto &row : classes)
	{
		CClass *stackcopy = new CClass;
		if (!CClass::first)
			CClass::first = stackcopy;
		else
			lastp->next = stackcopy;

		stackcopy->index = i++;
		stackcopy->name.SetBuffer(row.name.c_str());
		stackcopy->who_name.SetBuffer(row.who_name.c_str());
		stackcopy->attr_prime = row.attr_prime;
		stackcopy->align = row.align;
		stackcopy->weapon = row.weapon;
		stackcopy->gainconst = row.gainconst;
		stackcopy->base_group.SetBuffer(row.base_group.c_str());
		stackcopy->default_group.SetBuffer(row.default_group.c_str());
		stackcopy->ctype = row.ctype;
		stackcopy->status = row.status;
		stackcopy->next = nullptr;
		lastp = stackcopy;
	}
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

