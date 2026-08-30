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

		stackcopy->index = static_cast<CharClass>(i++);
		stackcopy->name = row.name;
		stackcopy->who_name = row.who_name;
		stackcopy->attr_prime = row.attr_prime;
		stackcopy->align = row.align;
		stackcopy->weapon = row.weapon;
		stackcopy->gainconst = row.gainconst;
		stackcopy->base_group = row.base_group;
		stackcopy->default_group = row.default_group;
		stackcopy->ctype = row.ctype;
		stackcopy->status = row.status;
		stackcopy->next = nullptr;
		lastp = stackcopy;
	}
}

CClass::~CClass()
{
}

CharClass CClass::GetIndex()
{
	return index;
}

CClass *CClass::GetClass(CharClass nIndex)
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

CClass * CClass::operator[](CharClass nIndex)
{
	return GetClass(nIndex);
	//return (CClass *)((char*)first + (nIndex) * sizeof(CClass));
}

/// Case-insensitive match over the shorter of the two strings: a query shorter
/// than the class name matches as a prefix, and one that is longer still matches
/// once the class name runs out. An empty query therefore matches the first
/// class. Kept exactly as it behaved when name was a refcounted string type.
static bool class_name_matches(const std::string &className, const char *query)
{
	const char *mystr = className.c_str();

	for (; *query && *mystr; query++, mystr++)
	{
		if (UPPER(*query) != UPPER(*mystr))
			return false;
	}

	return true;
}

std::optional<CharClass> CClass::Lookup (const char *name)
{
	CClass *aclass;
	for (aclass = first; aclass; aclass = aclass->next)
		if (class_name_matches(aclass->name, name))
			return aclass->index;

	return {};
}

