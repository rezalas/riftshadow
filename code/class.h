#include "rift.h"
/*
#define CLASS_NONE          0
#define CLASS_WARRIOR       1
#define CLASS_THIEF         2
#define CLASS_ZEALOT        3
#define CLASS_PALADIN       4
#define CLASS_ANTI_PALADIN  5
#define CLASS_RANGER        6
#define CLASS_ASSASSIN      7  
#define CLASS_SHAPESHIFTER  8
#define CLASS_HEALER        9  
#define CLASS_NECROMANCER   10  
#define CLASS_SORCERER      11
*/
class CClass //no redundancies here.
{
friend class CMud;
public:
		CClass();
		~CClass();
 
		CClass *		next;
		static CClass * first;
		static CClass * GetClass(int nIndex);			//get class ptr based on index, i.e. you pass CLASS_PALADIN
		int				GetIndex();	//get index of class, i.e. it returns CLASS_PALADIN
		CClass *		operator[](int nIndex);			//same as Getclass
		static int 		Lookup (const char *name);		//lookup "paladin" and return CLASS_PALADIN

		RString			name;
		RString			who_name;			//3 letter name that shows on who

		static void		LoadClassTable(const char *query);
		
		int				attr_prime;				//prime attrib
		int				align;
		int				weapon;					//weapon you start with
		int				gainconst;
		
		int				ctype;					//neither, caster, communer
		int				status;					//open for players to choose?

		RString			base_group;
		RString			default_group;
/*		enum
		{
			CLASS_NEITHER = 0,
			CLASS_CASTER,
			CLASS_COMMUNER
		};

		enum
		{
			ALIGN_ANY = 0,
			ALIGN_G,
			ALIGN_N,
			ALIGN_E,
			ALIGN_GN,
			ALIGN_GE,
			ALIGN_NE
		};
*/		
		int				index;
};
