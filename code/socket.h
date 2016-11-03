#include "rift.h"
#include <arpa/inet.h>
#define WIZ_LINKS 1

#define PLAYER_OUTPUT_SIZE	8192	//flah.
#define MAX_IBUF			4096
#define MAX_QUEUED_ARGLEN	2048
#define MAX_INLINE_LEN		256
#define MAX_CMD_LEN			20
#define MAX_COMMANDS		10
#define PRESS_RETURN_MSG	"[Press Return to Continue]"
#define UNKNOWN_CMD_MSG		"Huh?\n\r"

#define C_PLAYING			0
#define C_GREETSCREEN		1
#define C_PASSWORD			2
#define C_CONFIRM_NEW		3
#define C_MOTD				4
#define C_START_CHARGEN		5

typedef void SOCKHANDLER (CSocket *sock, const char *cmd, const char *args);

SOCKHANDLER std_input_handler;
SOCKHANDLER login_handler;
SOCKHANDLER new_char_handler;

struct CCmd
{
	char state;
	CCommand *cmdptr;
	char cmdargs[MAX_INLINE_LEN];
};

class CSocket
{
	friend class	CInterpreter;
public:
	CSocket();
	~CSocket();
	RString &		GetHost();
	
	CCharacter *	GetCharacter();
	int				GetState();
	void			SetState(int newstate, const char *msg = NULL);

	static CSocket *first;
	CSocket * 		next;
	
	bool			IsSnooped();
	bool			IsSwitched();

	void			Snoop(CSocket *snooper);
	void			EndSnoop(CSocket *snooper);
	void			EndAllSnoops();

	void			Switch(CCharacter *newchar);
	void			EndSwitch();
	
	int				ScrollLines();

	static void		InitializeSockets();

	static void		CheckSocketActivity(fd_set *in_set, fd_set *out_set);
	static void		HandleNewConnections(fd_set *in_set);
	static void		FlushOutputToPlayers();
	static void		ReadInputFromSockets();
	static void		CleanSockets();
	
	void			AddToBuffer(const char *txt, int slen = 0);
	void			AddOutputQueue(const char *txt, int slen = 0);
	void			FlushOutput();
	void			ReadFromSocket();
	void			WriteToSocket(const char *txt, int slen = 0);	//low level IMMEDIATE WRITE, unnecessary 99% of the time
	void			SetInputHandler(SOCKHANDLER newhandler, const char *msg = NULL);
	
	void			SocketInterpret(const char *cmd, const char *args);

	void			Disconnect(void);
	void			DestroySocket(void);	
	static void		InitSocket(int fd, sockaddr_in &sock);

	CCmd *			cmdbuf[MAX_COMMANDS];
	int				cmdqueuepos;
	int				cmdqueuecount;
	bool			entered_command;

	RString			char_name;  //temp placeholder
private:
	SOCKHANDLER *   handler_function;
	/* input */
	char			inlinebuf[MAX_INLINE_LEN];
	
	/* output */
	char *			outbuf;
	char *			outbufpos;
	char *			qbuf;		//buffered stuff we page to character one [Press return] at a time
	char *			qbufpos;
	char *			qbufprintpos;
	int				qbufsize; //alloc size

	bool			waiting_on_return;		//press return to continue
	CCharacter *	character;
	
	int				descriptor;
	int				state;

	RString			host;

	static CSocket	control;					//main socket for handling new people and such
};
