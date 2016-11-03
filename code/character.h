#include "stdlibs/file.h"
#include "rift.h"

class CCharacter
{
friend class CPCData;
friend class CMobTemplate;
public:
	CCharacter();
	~CCharacter();

	static CCharacter *			first;
	CCharacter *				next;

	CMobTemplate *				mobdata;
	CPCData	*					pcdata;

	static void					LoadPlayer(CSocket *sock);
	void						LoadPFile(); //fills in ch with appropriate details from pfile
	void						LoadPlayerData(CFile &myFile);
	void						LoadPlayerObject(CFile &myFile);
	void						LoadPlayerCharmed(CFile &myFile);

	void						HandleInput();	//calls various crap in CInterpreter to manage player input

	/*
	 *	Member Variable Access
	 */	
	
	CRoom *						GetRoom();
	CArea *						GetArea();
	
	CSocket *					GetSocket();	
	CCharacter *				GetNextInRoom();
	CCharacter *				GetFighting();
	CCharacter *				GetReply();

	CGroup *					GetGroup();
	
	CAffect *					GetFirstAffect();
	CObject *					GetFirstCarried();
	
	RString &					GetName();			//returns short descr for mobs, name for pcs
	RString &					GetTrueName();  	//returns name for mobs, true name for pcs
	
	RString &					GetDescr();			//returns description
	RString &					GetPrompt();
	
	bool						IsNPC();
	
	int							GetLevel();
	int							GetTrust();
	int							GetLag();
	int							GetPosition();
	int							GetPractices();
	int							GetTrains();
	int							GetSavesSpell();
	int							GetAlignment();		//todo
	int							GetHitroll();
	int							GetDamroll();
	int							GetAC(int iIndex);
	int							GetWimpy();
	int							GetStat(int iStat);
	int							GetPermStat(int iStat);
	float						GetDamMod();
	int							GetDefenseMod();
	int							GetLegs();
	int							GetArms();
	int							GetBalance();
	int							GetSex();
	const char *				GetSexName(bool nPossessive = FALSE); //"he"
	
	CSize *						GetSize();	
	CClass *					GetClass();
	CRace *						GetRace();
	CCabal *					GetCabal();

	bool						HasPet();

	unsigned int				GetHP();
	unsigned int				GetMana();
	unsigned int				GetMovement();
	unsigned int				GetMaxHP();
	unsigned int				GetMaxMana();
	unsigned int				GetMaxMovement();
	unsigned int				GetPermHP();
	unsigned int				GetPermMana();
	unsigned int				GetPermMovement();

	int							GetCarriedWeight(int nMax = CURRENT_VAL);
	int							GetCarriedNumber(int nMax = CURRENT_VAL);
		
	void						SetHP(float nHP, int nReplace = VAL_REPLACE, int nType = CURRENT_VAL); 	//val_add adds
	void						SetMana(float nMana, int nReplace = VAL_REPLACE, int nType = CURRENT_VAL);	//to current
	void						SetMovement(float nMove, int nReplace = VAL_REPLACE, int nType = CURRENT_VAL);

	void						SetMaxHP(float nHP, int nReplace = VAL_REPLACE);	//just wraps onto SetHP
	void						SetMaxMana(float nMana, int nReplace = VAL_REPLACE);
	void						SetMaxMove(float nMove, int nReplace = VAL_REPLACE);

	void						SetSocket(CSocket *newSocket);
	/*
	 * 	Character Enums
	 */
	
	/*
	 * 	Utility Enums
	 */
	
	enum
	{
		VAL_REPLACE, 		VAL_ADD, 		VAL_SUBTRACT, 	VAL_MULTIPLY, 	VAL_DIVIDE
	};
	enum
	{
		CURRENT_VAL, 		MAXIMUM_VAL, 	PERM_VAL
	};
	enum
	{
		COMMAND_LAG, 		ENEMY_LAG
	};
	enum
	{
		PULSES, 			ROUNDS, 		SECONDS
	};

	/*
	 * 	Utility Functions:
	 */
	
	void						CharFromRoom();
	void						LagChar(int nLag, int nLagType = COMMAND_LAG, int nDurationType = ROUNDS);
	
	bool						IsCaballed();				
	void						StopFollowing();				//stop following leader
	void						KillFollowers();				//stop all followers, extract pets/mobs
	bool						IsSameGroup(CCharacter *bch);
	bool						CanCarry(CCharacter *ch, CObject *obj);
	bool						HandsFull(CCharacter *ch);

	void						ExtractChar();
	/*
	 * 	Bitvectors
	 */
	
	CBitvector					act_flags;
	CBitvector					comm_flags;
	CBitvector					imm_flags;
	CBitvector					res_flags;
	CBitvector					vuln_flags;
	CBitvector					aff_flags;
	CBitvector					parts;
	CBitvector					form;

private:
	RString						name;
	RString						true_name;
	int							level;

	int							alignment;

	CInterpreter *				interpreter;
	CSocket *					socket;
	CCharacter *				next_in_room;
	CCharacter *				leader;
	CCharacter *				fighting;
	CCharacter *				reply;

	CRoom *						in_room;
	CObject *					on;

	int							hp;
	int							max_hp;
};
