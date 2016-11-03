/*
 * Replacement for const.c skill/spell table.
 * As well as anything with a gsn.
 */
#ifndef BEENHERE
//#include "stdlibs/funcptr.h"
#include "rift.h"
#endif
class CSkill
{
public:
	CSkill			*next;

	static CSkill *	GetSkill(int nGSN);			//we don't go through the linked list for this.  my pointer arithmetic makes you cry.
	RString			name;
	
	int 			gsn;						//auto-generated gsn #define
												//gsn corresponds to position in LL: gsn #2 is firstskill->next
	//int				skill_level[MAX_CLASS];
	int				target_type;
	int				min_pos;
	int				min_mana;
	int				min_lag;
	int				spell_type;					//spell, prayer, skill or power?
	RString 		damage_noun;
	RString			remove_msg;
	RString 		room_remove_msg;
	
	CBitvector		flags;
	
	CFuncPtr		skill_fun;
};
