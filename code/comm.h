#ifndef COMM_H
#define COMM_H

#include <errno.h>
#include <time.h>
#include <signal.h>
#include <fcntl.h>

#ifdef _WIN32
	#include <windows.h>
	#include <wincrypt.h>
	#include <ws2tcpip.h>
	#include <winsock.h>
	#include <winsock2.h>
#else
	#include <crypt.h>
	#include <netdb.h>
	#include <netinet/in.h>
	#include <sys/socket.h>
#endif

#include <string>
#include <variant>

#include "entity/fwd.h"
#include "entity/limits.h"
#include "entity/descriptor_data.h"	// DescriptorList: descriptor_list owns its connections
#include "entity/list_cursor.h"		// walking descriptor_list while connections close
#include "telnet.h"

#define CHAR_WRAP			85

#ifndef FNDELAY
	#define FNDELAY			O_NDELAY
#endif

#ifdef MALLOC_DEBUG
	extern int malloc_debug (int);
	extern int malloc_verify (void);
#endif

/*
 * OS-dependent declarations.
 */

#ifdef _WIN32
	int accept (int s, struct sockaddr *addr, int *addrlen);
	int bind (int s, struct sockaddr *name, int namelen);

	/*
	* If we were in linux, this would be declared for us, 
	* but sadly we're not. This is a stopgap for signal
	* processing until a better option is created.  
	*/

	#ifndef SIGFPE
		#define SIGFPE			8
	#endif

	#ifndef SIGPIPE
		#define SIGPIPE			13
	#endif

	#ifndef F_SETFL
		#define F_SETFL			4
	#endif

	#ifndef O_NDELAY
		#define O_NDELAY		04000
	#endif
#elif __linux__
	int	socket (int domain, int type, int protocol);
#endif

// Owns every open connection. A descriptor is put here by init_descriptor once
// it has survived the ban checks, and destroyed by close_socket erasing its
// node. ch->desc and d->snoop_by name a connection without owning it.
extern DescriptorList descriptor_list;
extern bool merc_down;
extern bool rebooting;
extern int reboot_num;
extern bool wizlock;
extern bool newlock;
extern bool MOBtrigger;
extern char str_boot_time[MAX_INPUT_LENGTH];
extern time_t current_time;
extern int mPort;
extern bool bDebug;

// Telnet commands

const char echo_off_str[] = { static_cast<char>(IAC), static_cast<char>(WILL), static_cast<char>(TELOPT_ECHO), '\0' };
const char echo_on_str[] = { static_cast<char>(IAC), static_cast<char>(WONT), static_cast<char>(TELOPT_ECHO), '\0' };
const char go_ahead_str[] = { static_cast<char>(IAC), static_cast<char>(GA), '\0' };

//
// TODO: built-in functions. need to check windows equivalent
//
//#include <unistd.h>
int close (int fd);
// unknown
int gofday (struct timeval *tp, struct timezone *tzp);
//#include <sys/select.h>
int select (int width, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);

//
// LOCAL FUNCTIONS
//

int init_socket  (int port);
void init_descriptor (int control);
void close_socket (DESCRIPTOR_DATA *dclose);
bool read_from_descriptor (DESCRIPTOR_DATA *d);
/*
 * Transfer one line from input buffer to input line.
 */
void read_from_buffer (DESCRIPTOR_DATA *d);
const char *get_battle_condition (CHAR_DATA *victim, int percent);
/*
 * Low level output function.
 */
bool process_output (DESCRIPTOR_DATA *d, bool fPrompt);
/*
   Player customizable prompts. -Dev
 */
void bust_a_prompt (CHAR_DATA *ch);
/*
 * output_buffer( descriptor )
 * this function sends output down a socket. Color codes are stripped off
 * is the player is not using color, or converted to ANSI color sequences
 * to provide colored output.
 * When using ANSI, the buffer can become a lot larger due to the (sometimes)
 * lengthy ANSI sequences, thus potentially overflowing the buffer. Therefor
 * *new* buffer is send in chunks.
 * The 'bzero's may seem unnecessary, but i didn't want to take risks.
 *
 * - Wreck
 */
bool output_buffer (DESCRIPTOR_DATA *d);
/*
 * Append onto an output buffer.
 */
void write_to_buffer (DESCRIPTOR_DATA *d, const char *txt, int length);
/*
 * Lowest level output function.
 * Write a block of text to the file descriptor.
 * If this gives errors on very long blocks (like 'ofind all'),
 *   try lowering the max block size.
 */
bool write_to_descriptor (int desc, char *txt, int length);
/*
 * Deal with sockets that haven't logged in yet.
 */
void nanny (DESCRIPTOR_DATA *d, char *argument);
/*
 * Parse a name for acceptability.
 */
bool check_parse_name (char *name);
/*
 * Look for link-dead player to reconnect.
 */
bool check_reconnect (DESCRIPTOR_DATA *d, char *name, bool fConn);
/*
 * Check if already playing.
 */
bool check_playing (DESCRIPTOR_DATA *d, char *name);
void stop_idling (CHAR_DATA *ch);
/*
 * Write to one char.
 */
void send_to_char (const char *txt, CHAR_DATA *ch);
void send_to_char_queue (std::string txt, CHAR_DATA *ch);
void send_to_chars (const char *txt, CHAR_DATA *ch, int min, ...);
/*
 * Send a page to one char.
 */
void page_to_char (const char *txt, CHAR_DATA *ch);
/* string pager */
void show_string (struct descriptor_data *d, char *input);
/* quick sex fixer */
void fix_sex (CHAR_DATA *ch);
///
/// One of act()'s two substitution arguments.
///
/// These were const void * and were reinterpreted by format code at runtime, so
/// arg1 was read as three unrelated types and arg2 as four, with the choice made
/// by a character in the format string and checked by nothing. Carrying the type
/// with the value lets the format switch ask what it was handed. A mismatch is
/// then a logged, attributable message instead of a wild read.
///
/// The constructors are implicit so call sites pass what they always passed. The
/// types that are absent are absent on purpose. A std::string has no conversion to
/// any alternative, so passing one is a compile error rather than the address of a
/// string object arriving where a char pointer was expected. A bare void * and a
/// literal 0 are both rejected as ambiguous for the same reason.
///
struct ActArg
{
	std::variant<std::monostate, const char *, const int *, OBJ_DATA *, CHAR_DATA *> value;

	ActArg() : value(std::monostate()) {}
	ActArg(std::nullptr_t) : value(std::monostate()) {}
	ActArg(const char *text) : value(text) {}
	ActArg(char *text) : value(static_cast<const char *>(text)) {}
	ActArg(const int *number) : value(number) {}
	ActArg(int *number) : value(static_cast<const int *>(number)) {}
	ActArg(OBJ_DATA *obj) : value(obj) {}
	ActArg(CHAR_DATA *ch) : value(ch) {}

	/// True when the caller passed no argument at all.
	bool IsEmpty() const { return std::holds_alternative<std::monostate>(value); }

	/// Each of these returns nullptr when the argument holds something else, which
	/// is what turns a type confusion into a message the caller can be found by.
	const char *AsText() const
	{
		const char *const *held = std::get_if<const char *>(&value);
		return held != nullptr ? *held : nullptr;
	}

	const int *AsNumber() const
	{
		const int *const *held = std::get_if<const int *>(&value);
		return held != nullptr ? *held : nullptr;
	}

	OBJ_DATA *AsObject() const
	{
		OBJ_DATA *const *held = std::get_if<OBJ_DATA *>(&value);
		return held != nullptr ? *held : nullptr;
	}

	CHAR_DATA *AsCharacter() const
	{
		CHAR_DATA *const *held = std::get_if<CHAR_DATA *>(&value);
		return held != nullptr ? *held : nullptr;
	}
};

void act (const char *format, CHAR_DATA *ch, ActArg arg1, ActArg arg2, int type);
void act_queue (std::string format, CHAR_DATA *ch, OBJ_DATA *arg1, CHAR_DATA *arg2, int type);
void act_area (const char *format, CHAR_DATA *ch, CHAR_DATA *victim);
void act_new (const char *format, CHAR_DATA *ch, ActArg arg1, ActArg arg2, int type, int min_pos);
void announce_login (CHAR_DATA *ch);
void announce_logout (CHAR_DATA *ch);
void do_rename (CHAR_DATA* ch, char* argument);
void do_renam (CHAR_DATA *ch,char *argument);
int color (char type, CHAR_DATA *ch, char *string );
void colorconv (char *buffer, const char *txt, CHAR_DATA *ch );
int get_allocate_points (CHAR_DATA *ch);
void show_allocate (CHAR_DATA *ch, int finish);
void process_text (CHAR_DATA *ch, char *text);

#endif /* COMM_H */
