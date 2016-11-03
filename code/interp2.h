#include "rift.h"

#define MAX_QUEUED_COMMANDS		10
#define MAX_QUEUED_DATA			4000

class CCommand
{
public:
	static CCommand *			first;
	CCommand *					next;

	int							index;

	static CCommand *			GetCommand(int nIndex);
	static CCommand *			GetCommand(const char *cmd);
	bool						CanBeDoneBy(CCharacter *ch);

	static void					LoadCmdTable(const char *query);
	
	RString						command_name;
	bool						immediate_eval;
	short int					minimum_level;
	short int					log_level;
	CFuncPtr					do_fun;
	CBitvector					visibility;
};

class CInterpreter
{
public:
	/*
	 * Note: we're a friend class of CSocket and refer to it interchangably
	 */
	CInterpreter();
	~CInterpreter();
	void						Interpret(CCharacter *ch, const char *str); 				//interpret a specific command _after_
	void						Interpret(CCharacter *ch, CCommand *cmd, const char *args); //they're allowed to do it
	void						ProcessInput();				//manage all input queues
private:
	void						ProcessInputQueue(CSocket *d);	//after all else is said and done, we're willing to consider popping
																//the top of the stack and doing a command.  If you're polite.
};
