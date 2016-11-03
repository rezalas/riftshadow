/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Thanks to abaddon for proof-reading our comm.c and pointing out bugs.  *
 *  Any remaining bugs are, of course, our work, not his.  :)              *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

/***************************************************************************
*	ROM 2.4 is copyright 1993-1996 Russ Taylor			   *
*	ROM has been brought to you by the ROM consortium		   *
*	    Russ Taylor (rtaylor@pacinfo.com)				   *
*	    Gabrielle Taylor (gtaylor@pacinfo.com)			   *
*	    Brian Moore (rom@rom.efn.org)				   *
*	By using this code, you have agreed to follow the terms of the	   *
*	ROM license, in the file Tartarus/doc/rom.license                  *
***************************************************************************/

/***************************************************************************
*       Tartarus code is copyright (C) 1997-1998 by Daniel Graham          *
*	In using this code you agree to comply with the Tartarus license   *
*       found in the file /Tartarus/doc/tartarus.doc                       *
***************************************************************************/

/*
 * This file contains all of the OS-dependent stuff:
 *   startup, signals, BSD sockets for tcp/ip, i/o, timing.
 *
 * The data flow for input is:
 *    Game_loop ---> Read_from_descriptor ---> Read
 *    Game_loop ---> Read_from_buffer
 *
 * The data flow for output is:
 *    Game_loop ---> Process_Output ---> Write_to_descriptor -> Write
 *
 * The OS-dependent functions are Read_from_descriptor and Write_to_descriptor.
 * -- Furey  26 Jan 1993
 */

#include <sys/types.h>
#include <sys/time.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <crypt.h>

#include "merc.h"
#include "recycle.h"
#include "tables.h"
#include "olc.h"
#include "interp.h"

/* command procedures needed */
DECLARE_DO_FUN(easy_induct	);
void announce_login args( ( CHAR_DATA *ch));
void announce_logout args( ( CHAR_DATA *ch));

/*
 * Malloc debugging stuff.
 */

#if defined(MALLOC_DEBUG)
extern	int	malloc_debug	args( ( int  ) );
extern	int	malloc_verify	args( ( void ) );
#endif

bool bDebug = FALSE;

/*
 * Signal handling.
 *   I dance around it.
 */


/*
 * Socket and TCP/IP stuff.
 */
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "telnet.h"
const	char	echo_off_str	[] = { IAC, WILL, TELOPT_ECHO, '\0' };
const	char	echo_on_str	[] = { IAC, WONT, TELOPT_ECHO, '\0' };
const	char 	go_ahead_str	[] = { IAC, GA, '\0' };

/*
 * OS-dependent declarations.
 */

#if	defined(linux)
/*
    Linux shouldn't need these. If you have a problem compiling, try
    uncommenting accept and bind.
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
*/

int	close		args( ( int fd ) );
//int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
//int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int     gofday    args( ( struct timeval *tp, struct timezone *tzp ) );
//int	listen		args( ( int s, int backlog ) );
//int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
int	socket		args( ( int domain, int type, int protocol ) );
//int	write		args( ( int fd, char *buf, int nbyte ) );
#endif

/*
 * Global variables.
 */
DESCRIPTOR_DATA *   descriptor_list;	/* All open descriptors		*/
DESCRIPTOR_DATA *   d_next;		/* Next descriptor in loop	*/
FILE *		    fpReserve;		/* Reserved file handle		*/
bool		    god;		/* All new chars are gods!	*/
bool		    merc_down;		/* Shutdown			*/
bool			rebooting = FALSE;
int				reboot_num = -1;
bool		    wizlock = FALSE;	/* Game is wizlocked		*/
bool		    newlock = FALSE;	/* Game is newlocked		*/
bool            MOBtrigger = TRUE;  /* act() switch                 */
char		    str_boot_time[MAX_INPUT_LENGTH];
time_t		    current_time;	/* time of this pulse */	



/*
 * OS-dependent local functions.
 */

void	game_loop_unix		args( ( int control ) );
int	init_socket		args( ( int port ) );
void	init_descriptor		args( ( int control ) );
bool	read_from_descriptor	args( ( DESCRIPTOR_DATA *d ) );
bool	write_to_descriptor	args( ( int desc, char *txt, int length ) );


/*
 * Other local functions (OS-independent).
 */
bool	check_parse_name	args( ( char *name ) );
bool	check_reconnect		args( ( DESCRIPTOR_DATA *d, char *name,
				    bool fConn ) );
bool	check_playing		args( ( DESCRIPTOR_DATA *d, char *name ) );
int	main			args( ( int argc, char **argv ) );
void	nanny			args( ( DESCRIPTOR_DATA *d, char *argument ) );
bool	process_output		args( ( DESCRIPTOR_DATA *d, bool fPrompt ) );
void	read_from_buffer	args( ( DESCRIPTOR_DATA *d ) );
void	stop_idling		args( ( CHAR_DATA *ch ) );
void    bust_a_prompt           args( ( CHAR_DATA *ch ) );
bool 	output_buffer		args( ( DESCRIPTOR_DATA *d ) );
int     get_allocate_points	args( ( CHAR_DATA *ch ) );
void    show_allocate		args( ( CHAR_DATA *ch , int finish) );
void	process_text		args( ( CHAR_DATA *ch, char *text ) );
#define CHAR_WRAP		85

int mPort;

int main( int argc, char **argv )
{
    struct timeval now_time;
	char buf[MSL];
	int port=0;
    int control;

	/*
     * Memory debugging if needed.
     */
#if defined(MALLOC_DEBUG)
    malloc_debug( 2 );
#endif

    /*
     * Init time.
     */
    gettimeofday( &now_time, NULL );
    current_time 	= (time_t) now_time.tv_sec;
    strcpy( str_boot_time, ctime( &current_time ) );

    /*
     * Reserve one channel for our use.
     */
    if ( ( fpReserve = fopen( NULL_FILE, "r" ) ) == NULL )
    {
	perror( NULL_FILE );
	exit( 0 );
    }

    /*
     * Get the port number.
     */
    port = 9998;
    if ( argc > 1 )
    {
	if ( !is_number( argv[1] ) )
	{
	    fprintf( stderr, "Usage: %s [port #]\n", argv[0] );
	    exit( 0 );
	}
	else if ( (port = atoi(argv[1])) == 666)
	{
	    bDebug = TRUE;
	    port = 9998;
	}
	else if ( port <= 1024 )
	{
	    fprintf( stderr, "Port number must be above 1024.\n" );
	    exit( 0 );
	}
    }
    mPort = port;
    /*
     * Run the game.
     */
    control = init_socket( port );
    //boot_db( );
	RS.Bootup();
	sprintf(buf,"Riftshadow booted, binding on port %d.", port );
	log_string(buf);

	game_loop_unix( control );
    close (control);

    /*
     * That's all, folks.
     */
    log_string( "Normal termination of game." );
    exit( 0 );
    return 0;
}



int init_socket( int port )
{
    static struct sockaddr_in sa_zero;
    struct sockaddr_in sa;
    int x = 1;
    int fd;
    int done = 0;

BIND_AGAIN:

	if ( ( fd = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 )
    {
	perror( "Init_socket: socket" );
	exit( 0 );
    }

    if ( setsockopt( fd, SOL_SOCKET, SO_REUSEADDR,
    (char *) &x, sizeof(x) ) < 0 )
    {
	perror( "Init_socket: SO_REUSEADDR" );
	close(fd);
	exit( 0 );
    }
/*
#if defined(SO_DONTLINGER)
    {
	struct	linger	ld;

	ld.l_onoff  = 1;
	ld.l_linger = 1000;

	if ( setsockopt( fd, SOL_SOCKET, SO_DONTLINGER,
	(char *) &ld, sizeof(ld) ) < 0 )
	{
	    perror( "Init_socket: SO_DONTLINGER" );
	    close(fd);
	    exit( SAFE_EXIT );
	}
    }
#endif
*/
    sa		    = sa_zero;
    sa.sin_family   = AF_INET;
    sa.sin_port	    = htons( port );

    if ( bind( fd, (struct sockaddr *) &sa, sizeof(sa) ) < 0 )
    {
		perror("Init socket: bind" );
		if(--done < 0)
		{
			fprintf(stderr,"Unable to allocate port.  Exiting.\n\r");
			close(fd);
			exit(0);
		}
		else
		{
			sleep(10);
			goto BIND_AGAIN;
		}

    }


    if ( listen( fd, 3 ) < 0 )
    {
	perror("Init socket: listen");
	close(fd);
	exit(0);
    }

    return fd;
}

void game_loop_unix( int control )
{
    static struct timeval null_time;
    struct timeval last_time;

    signal( SIGPIPE, SIG_IGN );
    gettimeofday( &last_time, NULL );
    current_time = (time_t) last_time.tv_sec;

    /* Main loop */
    while ( !merc_down )
    {
	fd_set in_set;
	fd_set out_set;
	fd_set exc_set;
	DESCRIPTOR_DATA *d;
	int maxdesc;

#if defined(MALLOC_DEBUG)
	if ( malloc_verify( ) != 1 )
	    abort( );
#endif

	/*
	 * Poll all active descriptors.
	 */
	FD_ZERO( &in_set  );
	FD_ZERO( &out_set );
	FD_ZERO( &exc_set );
	FD_SET( control, &in_set );
	maxdesc	= control;
	for ( d = descriptor_list; d; d = d->next )
	{
	    maxdesc = UMAX( maxdesc, d->descriptor );
	    FD_SET( d->descriptor, &in_set  );
	    FD_SET( d->descriptor, &out_set );
	    FD_SET( d->descriptor, &exc_set );
	}

	if ( select( maxdesc+1, &in_set, &out_set, &exc_set, &null_time ) < 0 )
	{
	    perror( "Game_loop: select: poll" );
	    exit( 0 );
	}

	/*
	 * New connection?
	 */
	if ( FD_ISSET( control, &in_set ) )
	    init_descriptor( control );

	/*
	 * Kick out the freaky folks.
	 */
	for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next = d->next;
	    if ( FD_ISSET( d->descriptor, &exc_set ) )
	    {
		FD_CLR( d->descriptor, &in_set  );
		FD_CLR( d->descriptor, &out_set );
		if ( d->character && d->character->level > 1)
		    save_char_obj( d->character );
		d->outtop	= 0;
		close_socket( d );
	    }
	}

	/*
	 * Process input.
	 */
	for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next	= d->next;
	    d->fcommand	= FALSE;

	    if ( FD_ISSET( d->descriptor, &in_set ) )
	    {
		if ( d->character != NULL )
		    d->character->timer = 0;
		if ( !read_from_descriptor( d ) )
		{
		    FD_CLR( d->descriptor, &out_set );
		    if ( d->character != NULL && d->character->level > 1)
			save_char_obj( d->character );
		    d->outtop	= 0;
		    close_socket( d );
		    continue;
		}
	    }

	    if ( d->character != NULL && d->character->wait > 0 )
			--d->character->wait;

		if ( d->character != NULL && d->character->wait <= 0 && d->character->pcdata->pending)
		{
			int i=0;
			interpret(d->character, d->character->pcdata->queue[0]);

			if(!d->character) //Cal: Silly Morglum.  But what if we just interped a "quit"?
				continue; //then d->character is null and we just crashed. :(

			for(i=1;i<d->character->pcdata->write_next;i++) {
				strcpy(d->character->pcdata->queue[(i-1)],
					d->character->pcdata->queue[i]);
			}

			d->character->pcdata->write_next--;
			
			for(i=d->character->pcdata->write_next;i<MAX_QUEUE;i++) {
				d->character->pcdata->queue[i][0] = '\0';
			}
			
			if(d->character->pcdata->write_next == 0)
				d->character->pcdata->pending = FALSE;

			continue;
		/*
			d->character->pcdata->queue[d->character->pcdata->read_next][0] = '\0';
			d->character->pcdata->read_next++;
			
			if (d->character->pcdata->read_next > 19)
				d->character->pcdata->read_next = 0;
			if (d->character->pcdata->queue[d->character->pcdata->read_next][0] == '\0') 
				d->character->pcdata->pending = FALSE;
		*/	
		}
		
	    read_from_buffer( d );
	    if ( d->incomm[0] != '\0' )
	    {
		d->fcommand	= TRUE;
		stop_idling( d->character );


		/* OLC */
		if ( d->showstr_point )
			show_string( d, d->incomm );
		else if ( d->pString )
            string_add( d->character, d->incomm );
		else if ( d->connected == CON_PLAYING && d->character->pcdata && d->character->pcdata->entering_text )
            process_text(d->character, d->incomm);
		else
			switch( d->connected )
			{
				case CON_PLAYING:
					if ( !run_olc_editor( d ))
						substitute_alias( d, d->incomm );
				break;
				default:
					nanny( d, d->incomm );
				break;
			}
		
/*		if (d->showstr_point)
		    show_string(d,d->incomm);
		else if ( d->connected == CON_PLAYING && d->character->pcdata && d->character->pcdata->entering_text )
		    process_text(d->character, d->incomm);
		else if ( d->connected == CON_PLAYING )
		    substitute_alias( d, d->incomm );
		else
		    nanny( d, d->incomm );
*/
		d->incomm[0]	= '\0';
	    }
	}



	/*
	 * Autonomous game motion.
	 */
	update_handler( );
	

	/*
	 * Output.
	 */
	for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next = d->next;

	    if ( ( d->fcommand || d->outtop > 0 )
	    &&   FD_ISSET(d->descriptor, &out_set) )
	    {
		if ( !process_output( d, TRUE ) )
		{
		    if ( d->character != NULL && d->character->level > 1)
			save_char_obj( d->character );
		    d->outtop	= 0;
		    close_socket( d );
		}
	    }
	}



	/*
	 * Synchronize to a clock.
	 * Sleep( last_time + 1/PULSE_PER_SECOND - now ).
	 * Careful here of signed versus unsigned arithmetic.
	 */
	{
	    struct timeval now_time;
	    long secDelta;
	    long usecDelta;

	    gettimeofday( &now_time, NULL );
	    usecDelta	= ((int) last_time.tv_usec) - ((int) now_time.tv_usec)
			+ 1000000 / PULSE_PER_SECOND;
	    secDelta	= ((int) last_time.tv_sec ) - ((int) now_time.tv_sec );
	    while ( usecDelta < 0 )
	    {
		usecDelta += 1000000;
		secDelta  -= 1;
	    }

	    while ( usecDelta >= 1000000 )
	    {
		usecDelta -= 1000000;
		secDelta  += 1;
	    }

	    if ( secDelta > 0 || ( secDelta == 0 && usecDelta > 0 ) )
	    {
		struct timeval stall_time;

		stall_time.tv_usec = usecDelta;
		stall_time.tv_sec  = secDelta;
		if ( select( 0, NULL, NULL, NULL, &stall_time ) < 0 )
		{
		    perror( "Game_loop: select: stall" );
		    exit( 1 );
		}
	    }
	}

	gettimeofday( &last_time, NULL );
	current_time = (time_t) last_time.tv_sec;
    }

    return;
}

void init_descriptor( int control )
{
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *dnew;
    struct sockaddr_in sock;
    struct hostent *from;
    int desc;
    int size;

    size = sizeof(sock);
    getsockname( control, (struct sockaddr *) &sock, (socklen_t *) &size );
    if ( ( desc = accept( control, (struct sockaddr *) &sock, (socklen_t *) &size) ) < 0 )
    {
	perror( "New_descriptor: accept" );
	return;
    }

#if !defined(FNDELAY)
#define FNDELAY O_NDELAY
#endif

    if ( fcntl( desc, F_SETFL, FNDELAY ) == -1 )
    {
	perror( "New_descriptor: fcntl: FNDELAY" );
	return;
    }

    /*
     * Cons a new descriptor.
     */
    dnew = new_descriptor();

    dnew->descriptor	= desc;
    dnew->connected	= CON_GET_NAME;
    dnew->showstr_head	= NULL;
    dnew->showstr_point = NULL;
    dnew->outsize	= 2000;
    dnew->pEdit		= NULL;			/* OLC */
    dnew->pString	= NULL;			/* OLC */
    dnew->editor	= 0;			/* OLC */
    dnew->outbuf	= new char[dnew->outsize];
	dnew->outtop	= 0;

    size = sizeof(sock);
    if ( getpeername( desc, (struct sockaddr *) &sock, (socklen_t *) &size ) < 0 )
    {
	perror( "New_descriptor: getpeername" );
	dnew->host = palloc_string( "(unknown)" );
    }
    else
    {
	/*
	 * Would be nice to use inet_ntoa here but it takes a struct arg,
	 * which ain't very compatible between gcc and system libraries.
	 */
	int addr;

	addr = ntohl( sock.sin_addr.s_addr );
	sprintf( buf, "%d.%d.%d.%d",
	    ( addr >> 24 ) & 0xFF, ( addr >> 16 ) & 0xFF,
	    ( addr >>  8 ) & 0xFF, ( addr       ) & 0xFF
	    );
	sprintf( log_buf, "Sock.sinaddr:  %s", buf );
	log_string( log_buf );
	if(strstr(buf,"204.82.56."))
	{
		log_string("DNS lookup refused. Forbidding access.");
		write_to_descriptor( desc, "Your site has been banned from this mud due to DNS problems.\n\r", 0 );
		close( desc );
		free_descriptor(dnew);
		return;
	}

	if((from = gethostbyaddr( (char *) &sock.sin_addr, sizeof(sock.sin_addr), AF_INET )) != NULL)
	{
		dnew->host = palloc_string( from->h_name );
		dnew->type = 0;
	}
	else
	{
		dnew->host = palloc_string( buf );
		dnew->type = 1;
	}
	}
	
    /*
     * Swiftest: I added the following to ban sites.  I don't
     * endorse banning of sites, but Copper has few descriptors now
     * and some people from certain sites keep abusing access by
     * using automated 'autodialers' and leaving connections hanging.
     *
     * Furey: added suffix check by request of Nickel of HiddenWorlds.
     */
    if ( check_ban(dnew->host,NBAN_ALL,dnew->type))
    {
	write_to_descriptor( desc,
	    "Your site has been banned from this mud.\n\r", 0 );
	close( desc );
	free_descriptor(dnew);
	return;
    }
    /*
     * Init descriptor data.
     */
    dnew->next			= descriptor_list;
    descriptor_list		= dnew;

    /*
     * Send the greeting.
     */
    {
	extern char * help_greeting;
	if ( help_greeting[0] == '.' )
	    write_to_buffer( dnew, help_greeting+1, 0 );
	else
	    write_to_buffer( dnew, help_greeting  , 0 );
    }

    return;
}

void close_socket( DESCRIPTOR_DATA *dclose )
{
    CHAR_DATA *ch;
    char buf[MAX_STRING_LENGTH];

    if ( dclose->outtop > 0 )
	process_output( dclose, FALSE );

    if ( dclose->snoop_by != NULL )
    {
	write_to_buffer( dclose->snoop_by,
	    "Your victim has left the game.\n\r", 0 );
    }

    {
	DESCRIPTOR_DATA *d;

	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( d->snoop_by == dclose )
		d->snoop_by = NULL;
	}
    }

    if ( ( ch = dclose->character ) != NULL )
    {
	sprintf( log_buf, "Closing link to %s.", ch->name );
	log_string( log_buf );
	/* cut down on wiznet spam when rebooting */
	if ( dclose->connected == CON_PLAYING && !merc_down)
	{
	    int ftime = current_time - ch->last_fight_time;
	    if(ch->invis_level < 51)
	    	act( "$n has lost $s link.", ch, NULL, NULL, TO_ROOM );
	    sprintf(buf,"$N has lost $S link (Last fought %s %d %s ago).",
				ch->last_fight_name != NULL ? ch->last_fight_name : "nobody",
				ch->last_fight_time ? ftime > 600 ? 
				(int)(ftime/60) : 
				ftime : -1,
				ftime > 600 ? "minutes" : "seconds"
				);

	    wiznet(buf,ch,NULL,WIZ_LINKS,0,get_trust(ch));
	    ch->desc = NULL;
	}
	else
	{
	    free_char(dclose->original ? dclose->original :
		dclose->character );
	}
    }

    if ( d_next == dclose )
	d_next = d_next->next;

    if ( dclose == descriptor_list )
    {
	descriptor_list = descriptor_list->next;
    }
    else
    {
	DESCRIPTOR_DATA *d;

	for ( d = descriptor_list; d && d->next != dclose; d = d->next )
	    ;
	if ( d != NULL )
	    d->next = dclose->next;
	else
	    bug( "Close_socket: dclose not found.", 0 );
    }

    close( dclose->descriptor );
    free_descriptor(dclose);
    return;
}



bool read_from_descriptor( DESCRIPTOR_DATA *d )
{
	unsigned int iStart;

    /* Hold horses if pending command already. */
    if ( d->incomm[0] != '\0' )
	return TRUE;

    /* Check for overflow. */
    iStart = strlen(d->inbuf);
    if ( iStart >= sizeof(d->inbuf) - 10 )
    {
		sprintf( log_buf, "%s input overflow!", d->host );
		log_string( log_buf );
		write_to_descriptor( d->descriptor,"\n\r*** PUT A LID ON IT!!! ***\n\r", 0 );
		return FALSE;
    }

    for ( ; ; )
    {
		int nRead;

		nRead = read( d->descriptor, d->inbuf + iStart,
	    sizeof(d->inbuf) - 10 - iStart );
		if ( nRead > 0 )
		{
	    	iStart += nRead;
	    	if ( d->inbuf[iStart-1] == '\n' || d->inbuf[iStart-1] == '\r' )
				break;
		}
		else if ( nRead == 0 )
		{
	    	log_string( "EOF encountered on read." );
	    	return FALSE;
		}
        else if ( errno == EAGAIN )
	    	break;
		else
		{
	    	perror( "Read_from_descriptor" );
	    	return FALSE;
		}
    }

    d->inbuf[iStart] = '\0';
    return TRUE;
}



/*
 * Transfer one line from input buffer to input line.
 */
void read_from_buffer( DESCRIPTOR_DATA *d )
{
    int i, j, k;

    /*
     * Hold horses if pending command already.
     */
    if ( d->incomm[0] != '\0' )
	return;

    /*
     * Look for at least one new line.
     */
    for ( i = 0; d->inbuf[i] != '\n' && d->inbuf[i] != '\r'; i++ )
    {
	if ( d->inbuf[i] == '\0' )
	    return;
    }

    /*
     * Canonical input processing.
     */
    for ( i = 0, k = 0; d->inbuf[i] != '\n' && d->inbuf[i] != '\r'; i++ )
    {
	if ( k >= MAX_INPUT_LENGTH - 2 )
	{
	    write_to_descriptor( d->descriptor, "Line too long.\n\r", 0 );

	    /* skip the rest of the line */
	    for ( ; d->inbuf[i] != '\0'; i++ )
	    {
		if ( d->inbuf[i] == '\n' || d->inbuf[i] == '\r' )
		    break;
	    }
	    d->inbuf[i]   = '\n';
	    d->inbuf[i+1] = '\0';
	    break;
	}

	if ( d->inbuf[i] == '\b' && k > 0 )
	    --k;
	else if ( isascii(d->inbuf[i]) && isprint(d->inbuf[i]) )
	    d->incomm[k++] = d->inbuf[i];
    }

    /*
     * Finish off the line.
     */
    if ( k == 0 )
	d->incomm[k++] = ' ';
    d->incomm[k] = '\0';

    /*
     * Do '!' substitution.
     */
    if ( d->incomm[0] == '!' )
		strcpy( d->incomm, d->inlast );
    else
		strcpy( d->inlast, d->incomm );

    /*
     * Shift the input buffer.
     */
    while ( d->inbuf[i] == '\n' || d->inbuf[i] == '\r' )
	i++;
    for ( j = 0; ( d->inbuf[j] = d->inbuf[i+j] ) != '\0'; j++ )
	;
    return;
}


const char * get_battle_condition(CHAR_DATA *victim, int percent)
{
	AFFECT_DATA *b_af = NULL;

	if(is_affected(victim, gsn_bluff))
	{
		AFFECT_DATA *b_af = NULL;

		for(b_af = victim->affected; b_af != NULL; b_af = b_af->next)
		{
			if (b_af->type == gsn_bluff)
				break;
		}
		percent *= (b_af->modifier * 3);
	}

	
	if (percent >= 100)
		return "is in perfect condition.";
	else if (percent >= 90)
		return "has a few scratches.";
	else if (percent >= 75)
		return "has some small wounds.";
	else if (percent >= 50)
		return "is covered in bleeding wounds.";
	else if (percent >= 30)
		return "is gushing blood.";
	else if (percent >= 15)
		return "is writhing in pain.";
	else if (percent >= 0)
		return "is convulsing on the ground.";
	else
		return "is nearly dead.";
}


/*
 * Low level output function.
 */
bool process_output( DESCRIPTOR_DATA *d, bool fPrompt )
{
    extern bool merc_down;

    /*
     * Bust a prompt.
     */
    if (!merc_down && d->showstr_point)
        write_to_buffer(d,"[Hit Return to continue]\n\r",0);
	else if (d->pString != NULL)
		write_to_buffer(d,"EDIT> ",0);
	else if (!merc_down && fPrompt && d->connected == CON_PLAYING)
    {
   	CHAR_DATA *ch;
	CHAR_DATA *victim;
	
	ch = d->character;
	
	/* battle prompt */
	if ((victim = ch->fighting) != NULL)
	{
		int percent;
        char wound[100];
	    char buf[MAX_STRING_LENGTH];

        if (victim->max_hit > 0)
        	percent = victim->hit * 100 / victim->max_hit;
        else
        	percent = -1;


	    sprintf(wound,"%s", get_battle_condition(victim, percent));

	    sprintf(buf,"%s %s \n\r",
	            (can_see(ch,victim)) ? (IS_NPC(victim) ? victim->short_descr : PERS(victim,ch)) : "Someone",wound);
	    buf[0] = UPPER(buf[0]);
        write_to_buffer( d, buf, 0);

		if(IS_IMMORTAL(ch))
		{		
			sprintf(buf,"(Waiting: %d)\n\r", (ch->wait / PULSE_VIOLENCE));
			write_to_buffer( d, buf, 0);
		}
	}

	ch = d->original ? d->original : d->character;
	if (!IS_SET(ch->comm, COMM_COMPACT) )
	    write_to_buffer( d, "\n\r", 2 );
	if (!IS_NPC(ch) && ch->pcdata->entering_text)
	    write_to_buffer( d, ": ", 2 );
        else if ( IS_SET(ch->comm, COMM_PROMPT))
            bust_a_prompt( d->character );

	if (IS_SET(ch->comm,COMM_TELNET_GA))
	    write_to_buffer(d,go_ahead_str,0);
    }

    /*
     * Short-circuit if nothing to write.
     */
    if ( d->outtop == 0 )
	return TRUE;

    /*
     * Snoop-o-rama.
     */
    if ( d->snoop_by != NULL )
    {
	if (d->character != NULL)
	    write_to_buffer( d->snoop_by, d->character->name,0);
	write_to_buffer( d->snoop_by, "> ", 2 );
	write_to_buffer( d->snoop_by, d->outbuf, d->outtop );
    }

    /*
     * OS-dependent output.
     *
     * now done at output_buffer( ) to deal with color codes.
     * - Wreck
     */
/*    return output_buffer( d );*/
    if ( !write_to_descriptor( d->descriptor, d->outbuf, d->outtop ) )
    {
	d->outtop = 0;
	return FALSE;
    }
    else
    {
	d->outtop = 0;
	return TRUE;
    }
}

/*
   Player customizable prompts. -Dev
 */
void bust_a_prompt( CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH], buf3[MSL];
    /*const*/ char *str, *orig;
    const char *i = NULL;
    char *point;
    char doors[MAX_INPUT_LENGTH];
    EXIT_DATA *pexit;
    int number_people;
    bool found;
    const char *dir_name[] = {"N","E","S","W","U","D"};
    int door;
    DESCRIPTOR_DATA *d;
    point = buf;
	
    if(IS_NPC(ch) || !str_cmp(ch->prompt,"")
		|| !str_cmp(ch->prompt,"<%hhp %mm %vmv> ")
		|| !str_cmp(ch->prompt,"<%hhp %mm %vmv>")) {
    	sprintf( buf, "%s<%dhp %dm %dmv>%s ", get_char_color(ch,"prompt"),
    		ch->hit,ch->mana,ch->move,END_COLOR(ch));
    	send_to_char(buf,ch);
    	return;
    }

   if (IS_SET(ch->comm,COMM_AFK))
	   send_to_char("<AFK> ",ch);

   sprintf(buf2,"%s%s%s",get_char_color(ch,"prompt"),ch->prompt,END_COLOR(ch));
   strcpy(&buf3[0], &buf2[0]);
   orig = &buf3[0];
   str = orig;

   while( *str != '\0' )
   {
      if( *str != '%' )
      {
         *point++ = *str++;
         continue;
      }
      ++str;
      switch( *str )
      {
         default :
            i = " "; break;
		 case 'e':
	    	found = FALSE;
	    	doors[0] = '\0';
	    	for (door = 0; door < 6; door++)
	    	{
				if ((pexit = ch->in_room->exit[door]) != NULL
				&&  pexit ->u1.to_room != NULL
				&&  can_see_room(ch,pexit->u1.to_room)
				&&  !IS_SET(pexit->exit_info,EX_NONOBVIOUS)
				&&  (!IS_SET(pexit->exit_info,EX_CLOSED) || IS_IMMORTAL(ch)))
				{
		    		found = TRUE;
		    		strcat(doors,dir_name[door]);
				}
	    	}
	
			if (!found || (IS_AFFECTED(ch,AFF_BLIND) && !IS_IMMORTAL(ch)))
	 		strcat(buf,"none");
	    	sprintf(buf2,"%s",doors);
	    	i = buf2; break;
 	 	 case 'c' :
	    	sprintf(buf2,"%s","\n\r");
	    	i = buf2; break;
         case 'h' :
            if ( !IS_SET( ch->comm,COMM_ANSI ) )
              sprintf( buf2, "%d", ch->hit);
            else
              sprintf( buf2, "%d", ch->hit);
              i = buf2; break;
         case 'H' :
            sprintf( buf2, "%d", ch->max_hit );
            i = buf2; break;
         case 'm' :
            sprintf( buf2, "%d", ch->mana );
         	i = buf2; break;
         case 'M' :
            sprintf( buf2, "%d", ch->max_mana );
            i = buf2; break;
		 case 'L' :
		 	if(ch->wait > 1)
				sprintf(buf2,"+");
			else
				sprintf(buf2,"-");
			i= buf2; break;
		 case 'v':
            sprintf( buf2, "%d", ch->move );
         	i = buf2; break;
         case 'V' :
            sprintf( buf2, "%d", ch->max_move );
            i = buf2; break;
		 case '1' :
			if (ch->max_hit == 0)
				sprintf(buf2, "0%%");
			else
				sprintf( buf2, "%d%s", (100 * ch->hit)/ch->max_hit, "%");
			i = buf2; break;
	 	 case '2' :
			if (ch->max_mana == 0)
				sprintf(buf2, "0%%");
			else
				sprintf(buf2, "%d%s",(100 * ch->mana)/ch->max_mana, "%");
			i = buf2; break;
		 case '3' :
			if (ch->max_move == 0)
				sprintf(buf2, "0%%");
			else
				sprintf(buf2, "%d%s",(100 * ch->move)/ch->max_move, "%");
			i = buf2; break;
         case 'x' :
            sprintf( buf2, "%d", ch->exp );
            i = buf2; break;
	 	 case 'X' :
	    	sprintf(buf2, "%d", IS_NPC(ch) ? 0 :
            ch->level * exp_per_level(ch) - ch->exp);
	    	i = buf2; break;
         case 'g' :
            sprintf( buf2, "%ld", ch->gold);
            i = buf2; break;
         case 'a' :
               sprintf( buf2, "%s", IS_GOOD(ch) ? "good" : IS_EVIL(ch) ?
                "evil" : "neutral" );
            i = buf2; break;
         case 'r' :
            if( ch->in_room != NULL )
               sprintf( buf2, "%s",
			((!IS_NPC(ch) && IS_SET(ch->act,PLR_HOLYLIGHT)) ||
		 	(!IS_AFFECTED(ch,AFF_BLIND) && !room_is_dark( ch->in_room )))
			? get_room_name(ch->in_room) : "darkness");
            else
               sprintf( buf2, " " );
            i = buf2; break;
         case 'R' :
            if( IS_IMMORTAL( ch ) && ch->in_room != NULL )
               sprintf( buf2, "%d", ch->in_room->vnum );
            else
               sprintf( buf2, " " );
            i = buf2; break;
         case 'z' :
            if( IS_IMMORTAL( ch ) && ch->in_room != NULL )
               sprintf( buf2, "%s", ch->in_room->area->name );
            else
               sprintf( buf2, " " );
            i = buf2; break;
	 	 case 'p' :
	    	if(IS_IMMORTAL(ch) && ch->in_room != NULL)
	      	{
				number_people = 0;
    			for ( d = descriptor_list; d != NULL; d = d->next )
        			if ( d->connected == CON_PLAYING
        			&&   d->character->in_room != NULL
        			&&   d->character->in_room->area == ch->in_room->area
        			&&   !IS_IMMORTAL(d->character))
						number_people++;
	        	sprintf(buf2,"%i",number_people);
             }
	    	 else
				sprintf(buf2," ");
	    	 i = buf2; break;
         case 'P' :
            if(IS_IMMORTAL(ch) && ch->in_room != NULL)
              {
                number_people = 0;
                for ( d = descriptor_list; d != NULL; d = d->next )
                        if ( d->connected == CON_PLAYING
                        &&   d->character->in_room != NULL
                        &&   d->character->in_room->area == ch->in_room->area
                        &&   can_see(ch,d->character))
                                number_people++;
                sprintf(buf2,"%i",number_people);
              }
            else
                sprintf(buf2," ");
            i = buf2; break;
	 	 case 'C' :
            if(IS_IMMORTAL(ch))
              {
                number_people = 0;
                for ( d = descriptor_list; d != NULL; d = d->next )
                        if ( d->connected == CON_PLAYING && can_see(ch,d->character))
                                number_people++;
                sprintf(buf2,"%i",number_people);
              }
            else
                sprintf(buf2," ");
            i = buf2; break;
         case '%' :
            sprintf( buf2, "%%" );
            i = buf2; break;
         case 'o' :
			if(is_editing(ch))
			{
            	sprintf( buf2, " (%s - %s)", olc_ed_name(ch), olc_ed_vnum(ch) );
				i = buf2;
			}
			break;
	case 't' :
		sprintf(buf2,"%d%s %s",
			(time_info.hour % 12 == 0) ? 12 : time_info.hour % 12,
		       	(time_info.half) ? ":30" : "", 
			(time_info.hour >= 12) ? "pm" : "am");
		i = buf2;
		break;
	case 'q' :
		sprintf(buf2,"%d", ch->pcdata->write_next);
		i = buf2;
		break;
	  	}
      ++str;
      while( (*point = *i) != '\0' )
         ++point, ++i;
   }
   write_to_buffer( ch->desc, buf, point - buf );
//   free_pstring(orig);

   return;
}

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

bool output_buffer( DESCRIPTOR_DATA *d )
{
    char	buf[MAX_STRING_LENGTH];
    char	buf2[128];
    const char 	*str;
    char 	*i;
    char 	*point;
    bool	flash=FALSE, o_flash,
    		bold=FALSE, o_bold;
    bool	act=FALSE, ok=TRUE, color_code=FALSE;
    int		color=7, o_color;

    /* discard NULL descriptor */
    if ( d==NULL )
    	return FALSE;
    	
    bzero( buf, MAX_STRING_LENGTH );
    point=buf;
    str=d->outbuf;
    o_color=color;
    o_bold=bold;
    o_flash=flash;
    	
    while ( *str != '\0' && (str-d->outbuf)<d->outtop )
    {
    	if ( (int)(point-buf)>=MAX_STRING_LENGTH-32 )
    	{
    	    /* buffer is full, so send it through the socket */
    	    *point++='\0';
    	    if ( !(ok=write_to_descriptor( d->descriptor,
    	    				   buf,
    	    				   strlen( buf ) )) )
    	        break;
    	    bzero( buf, MAX_STRING_LENGTH );
    	    point=buf;
    	}

    	if ( *str != '{' )
    	{
    	    color_code=FALSE;
    	    *point++ = *str++;
    	    continue;
    	}

    	if ( !color_code && *(str+1)!='<' )
    	{
    	    o_color=color;
    	    o_bold=bold;
    	    o_flash=flash;
    	}
    	color_code=TRUE;
    	
    	act=FALSE;
    	str++;
    	switch ( *str )
    	{
    	    default:    sprintf( buf2, "{%c", *str ); 		       break;
    	    case 'x': 	sprintf( buf2, "{" );		     	       break;
    	    case '-': 	sprintf( buf2, "~" );		     	       break;
    	    case '<': 	color=o_color; bold=o_bold; flash=o_flash;
    	    						     act=TRUE; break;
    	    case '0':	color=0;	 		     act=TRUE; break;
    	    case '1':	color=1; 			     act=TRUE; break;
    	    case '2':	color=2; 			     act=TRUE; break;
    	    case '3':	color=3;	 		     act=TRUE; break;
    	    case '4':	color=4;		 	     act=TRUE; break;
    	    case '5':	color=5; 			     act=TRUE; break;
    	    case '6':	color=6;		 	     act=TRUE; break;
    	    case '7':	color=7;			     act=TRUE; break;
    	    case 'B':	bold=TRUE;   			     act=TRUE; break;
    	    case 'b':	bold=FALSE;  			     act=TRUE; break;
    	    case 'F':	flash=TRUE; 		 	     act=TRUE; break;
    	    case 'f':	flash=FALSE; 			     act=TRUE; break;
    	    case 'n':	if ( d->character && IS_ANSI( d->character ) )
    	    		    sprintf( buf2, "%s", ANSI_NORMAL );
    	    		else
    	    		    buf2[0]='\0';
    	    		bold=FALSE; color=7; flash=FALSE;	break;
    	}
	if ( act )
	{
	    if ( d->character && IS_ANSI( d->character ) )
 	    {
	        sprintf( buf2, "%s", color_value_string( color, bold, flash ) );
    	        color_code=TRUE;
	    }
	    else
	        buf2[0]='\0';
        }

        i=buf2;
        str++;
        while ( ( *point = *i ) != '\0' )
            ++point, ++i;
    }

    *point++='\0';
    ok=ok && (write_to_descriptor( d->descriptor, buf, strlen( buf ) ));
    d->outtop=0;

    return ok;
}


/*
 * Append onto an output buffer.
 */
void write_to_buffer( DESCRIPTOR_DATA *d, const char *txt, int length )
{
    /*
     * Find length in case caller didn't.
     */
    if ( length <= 0 )
		length = strlen(txt);

    /*
     * Initial \n\r if needed.
     */
    if ( d->outtop == 0 && !d->fcommand ) {
		d->outbuf[0]	= '\n';
		d->outbuf[1]	= '\r';
		d->outtop	= 2;
    }

    /*
     * Expand the buffer as needed.
     */
    while ( d->outtop + length >= d->outsize ) {
		char *outbuf;

		if (d->outsize >= 32000) {
		    bug("Buffer overflow. Closing.\n\r",0);
		    d->outtop = 0;
		    close_socket(d);
		    return;
 		}
		d->outsize 	*= 2;
		outbuf       = new char[d->outsize];
		strncpy( outbuf, d->outbuf, d->outtop );
		delete[] d->outbuf;
		d->outbuf   = outbuf;
    }

    /*
     * Copy.
     */
    strcpy( d->outbuf + d->outtop, txt );
    d->outtop += length;
    return;
}



/*
 * Lowest level output function.
 * Write a block of text to the file descriptor.
 * If this gives errors on very long blocks (like 'ofind all'),
 *   try lowering the max block size.
 */
bool write_to_descriptor( int desc, char *txt, int length )
{
    int iStart;
    int nWrite;
    int nBlock;

    if ( length <= 0 )
	length = strlen(txt);

    for ( iStart = 0; iStart < length; iStart += nWrite )
    {
	nBlock = UMIN( length - iStart, 4096 );
	if ( ( nWrite = write( desc, txt + iStart, nBlock ) ) < 0 )
	    { perror( "Write_to_descriptor" ); return FALSE; }
    }

    return TRUE;
}



/*
 * Deal with sockets that haven't logged in yet.
 */
void nanny( DESCRIPTOR_DATA *d, char *argument )
{
    DESCRIPTOR_DATA *d_old, *d_next;
    char buf[MAX_STRING_LENGTH], word[200], tword[200], cword[200];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *ch;
    OBJ_DATA *fobj; /* For pfile limit bug */
    OBJ_DATA *fobj_next;
    char *pwdnew;
    char *p;
    int iClass, race, i, type, sn, modif, modamt;
    bool fOld;
    int dh=0, col;
    FILE *deadchar;
    char buffile[MAX_STRING_LENGTH];
    int hometown;
	int ele_num = -1, gn = -1;
	int cres=0;

    while ( isspace(*argument) )
		argument++;

    ch = d->character;

    switch ( d->connected )
    {

    default:
		bug( "Nanny: bad d->connected %d.", d->connected );
		close_socket( d );
		return;

    case CON_GET_NAME:
	if ( argument[0] == '\0' )
	{
	    close_socket( d );
	    return;
	}
	
	argument    = talloc_string(lowstring(argument));
	argument[0] = UPPER(argument[0]);

	if ( !check_parse_name( argument ) )
	{
	    write_to_buffer( d, "Illegal name, try another.\n\rName: ", 0 );
	    return;
	}
	
	sprintf(buffile,"%sdead_char/%s.plr",PLAYER_DIR,argument);

	if((deadchar=fopen(buffile,"r"))!=NULL)
	{
		//Ack. This name is in use in the dead_char directory. Spank them.
		write_to_buffer(d,"You are denied access.\n\r",0);
		close_socket(d);
		return;
	}

	fOld = load_char_obj( d, argument );
	ch   = d->character;

	if (IS_SET(ch->act, PLR_DENY))
	{
	    sprintf( log_buf, "Denying access to %s@%s.", argument, d->host );
	    log_string( log_buf );
	    write_to_buffer( d, "You are denied access.\n\r", 0 );
	    close_socket( d );
	    return;
	}

	if (check_ban(d->host,NBAN_ALL,d->type) && !IS_SET(ch->act,PLR_PERMIT))
	{
	    write_to_buffer(d,"Your site has been banned from this mud.\n\r",0);
	    close_socket(d);
	    return;
	}

	if ( check_reconnect( d, argument, FALSE ) )
	{
	    fOld = TRUE;
	}
	else
	{

	    if ( wizlock && (!IS_IMMORTAL(ch) && !(IS_HEROIMM(ch))) )
	    {
		write_to_buffer( d, "The game is currently wizlocked.\n\r", 0);
		close_socket( d );
		return;
	    }
	}

	if ( fOld )
	{
	    /* Old player */
	    write_to_buffer( d, "Pass: ", 0 );
	    write_to_buffer( d, echo_off_str, 0 );
	    d->connected = CON_GET_OLD_PASSWORD;
	    return;
	}
	else
	{
	    /* New player */
 	    if (newlock)
	    {
                write_to_buffer( d, "The game is newlocked.\n\r", 0 );
                close_socket( d );
                return;
            }

	    if (check_ban(d->host,NBAN_NEWBIE,d->type))
	    {
		write_to_buffer(d,"New players are not allowed from your site.\n\r",0);
		close_socket(d);
		return;
	    }
		sprintf( buf, "Did I get that right, %s (Y/N)? ", argument );
        	write_to_buffer( d, buf, 0 );
		d->connected = CON_CONFIRM_NEW_NAME;
	    return;
	}
	break;

	case CON_LEGIT_NAME:
		switch (*argument) {
			case('Y'):
			case('y'):
				break;
			case('N'):
			case('n'):
				write_to_buffer( d, "Thank you.  Please select a more suitable name: ", 0 );
				free_char( d->character );
				d->character = NULL;
				d->connected = CON_GET_NAME;
				return;
			default:
				write_to_buffer(d,"Please enter yes or no.\n\r", 0);
				return;
		}
		sprintf( buf, "Give me a password for %s: %s", ch->name, echo_off_str );
		write_to_buffer( d, buf, 0 );
		d->connected = CON_GET_NEW_PASSWORD;
	break;

	case CON_GET_OLD_PASSWORD:
	write_to_buffer( d, "\n\r", 2 );

	if ( strcmp( crypt( argument, ch->pcdata->pwd ), ch->pcdata->pwd ))
	{
	    write_to_buffer( d, "Wrong password.\n\r", 0 );
/*
 * This  should hopefully handle the pfile obj->limcount bug with bad pwds
 */
	for (fobj = ch->carrying; fobj != NULL; fobj = fobj_next)
	{
	fobj_next = fobj->next_content;
	fobj->pIndexData->limcount++; /* Increment count due to drop from */
				      /* bad password bug */	
	}
	
	    close_socket( d );
	    return;
	}

	write_to_buffer( d, echo_on_str, 0 );

	if (check_playing(d,(ch->true_name ? ch->true_name :ch->name)))
	    return;

	if ( check_reconnect( d, ch->name, TRUE ) )
	    return;

	free_pstring(ch->pcdata->logon_time);
	ch->pcdata->logon_time = palloc_string(ctime( &current_time ));
	ch->pcdata->logon_time[strlen(ch->pcdata->logon_time)-1] = '\0';

	sprintf( log_buf, "%s@%s has connected. [%d (%d) obj] %s",
	ch->true_name,
	d->host,
        count_carried(ch,FALSE),
        count_carried(ch,TRUE),
	auto_check_multi(d,d->host) ? " (MULTI)" : "");

	log_string( log_buf );
	login_log(ch,1);

	wiznet(log_buf,NULL,NULL,WIZ_SITES,0,get_trust(ch));

	if ( IS_IMMORTAL(ch) ) {
	    do_help( ch, "imotd" );
	    d->connected = CON_READ_IMOTD;
 	} else {
	    do_help( ch, "motd" );
		write_to_buffer(d,"[Hit Return to continue]\n\r",0);
	    d->connected = CON_READ_MOTD;
	}
	break;

/* RT code for breaking link */

    case CON_BREAK_CONNECT:
	switch( *argument )
	{
	case 'y' : case 'Y':
            for ( d_old = descriptor_list; d_old != NULL; d_old = d_next )
	    {
		d_next = d_old->next;
		if (d_old == d || d_old->character == NULL)
		    continue;

		if (str_cmp((ch->true_name ? ch->true_name :
			ch->name),d_old->original ?
		    d_old->original->name : (d_old->character->true_name
			? d_old->character->true_name :d_old->character->name)))
		    continue;

		close_socket(d_old);
	    }
	    if (check_reconnect(d,ch->name,TRUE))
	    	return;
	    write_to_buffer(d,"Reconnect attempt failed.\n\rName: ",0);
            if ( d->character != NULL )
            {
                free_char( d->character );
                d->character = NULL;
            }
	    d->connected = CON_GET_NAME;
	    break;

	case 'n' : case 'N':
	    write_to_buffer(d,"Name: ",0);
            if ( d->character != NULL )
            {
                free_char( d->character );
                d->character = NULL;
            }
	    d->connected = CON_GET_NAME;
	    break;

	default:
	    write_to_buffer(d,"Please type Y or N? ",0);
	    break;
	}
	break;

    case CON_CONFIRM_NEW_NAME:
	switch ( *argument )
	{
	case 'y': case 'Y':
	    sprintf(buf, "Only original and genre-appropriate names are allowed on Riftshadow.\n\r");
	    write_to_buffer( d, buf, 0 );
		sprintf( buf, "   1) Names may not be based on dictionary words (English or otherwise.)\n\r");
		write_to_buffer( d, buf, 0 );
		sprintf( buf, "      (e.g. 'Wintermoon', 'Darkblade', and the like are not acceptable.)\n\r");
		write_to_buffer( d, buf, 0 );
		sprintf( buf, "   2) Names must be original to the best of your knowledge.\n\r");
		write_to_buffer( d, buf, 0 );
		sprintf( buf, "      (i.e. not 'Aragorn' or 'Hercules' or 'Roosevelt' or... ) \n\r");
		write_to_buffer( d, buf, 0 );
		sprintf( buf, "   3) Names must be suited to a serious medieval fantasy setting.\n\r");
		write_to_buffer( d, buf, 0 );
		sprintf( buf, "      ('Bobby' and 'Marilou' are wonderful names, but not appropriate here.)\n\r\n\r");
		write_to_buffer( d, buf, 0 );
		sprintf( buf, "Names that fail to meet these basic standards will result in character denial.\n\rYou have been warned.  In light of the above, is your name suitable? ");
		write_to_buffer( d, buf, 0 );
	    d->connected = CON_LEGIT_NAME;
	    break;

	case 'n': case 'N':
	    write_to_buffer( d, "Ok, what IS it, then? ", 0 );
	    free_char( d->character );
	    d->character = NULL;
	    d->connected = CON_GET_NAME;
	    break;

	default:
	    write_to_buffer( d, "Please type Yes or No? ", 0 );
	    break;
	}
	break;

    case CON_GET_NEW_PASSWORD:
	write_to_buffer( d, "\n\r", 2 );

	if ( strlen(argument) < 5 )
	{
	    write_to_buffer( d,
		"Password must be at least five characters long.\n\rPassword: ",
		0 );
	    return;
	}

	pwdnew = crypt( argument, ch->name );
	for ( p = pwdnew; *p != '\0'; p++ )
	{
	    if ( *p == '~' )
	    {
		write_to_buffer( d,
		    "New password not acceptable, try again.\n\rPassword: ",
		    0 );
		return;
	    }
	}
	
	free_pstring( ch->pcdata->pwd );
	ch->pcdata->pwd	= palloc_string( pwdnew );
	write_to_buffer( d, "Please retype password: ", 0 );
	d->connected = CON_CONFIRM_NEW_PASSWORD;
	break;

    case CON_CONFIRM_NEW_PASSWORD:
	write_to_buffer( d, "\n\r", 2 );

	if ( strcmp( crypt( argument, ch->pcdata->pwd ), ch->pcdata->pwd ) )
	{
	    write_to_buffer( d, "Passwords don't match.\n\rRetype password: ",
		0 );
	    d->connected = CON_GET_NEW_PASSWORD;
	    return;
	}

	write_to_buffer( d, echo_on_str, 0 );
        write_to_buffer( d, "\n\rChoose your race:\n\r",0);
        col = 0;
        for ( race = 1; race < MAX_PC_RACE; race++ )
        {
		if (race == race_lookup("lich") || race == race_lookup("idiot")) {
			continue;
		}
                ( (pc_race_table[race].xpadd == 0) ?
        sprintf(buf,"%-15s ( no extra xp)",pc_race_table[race].name) :
        sprintf(buf,"%-15s (%3d extra xp)",pc_race_table[race].name,
        pc_race_table[race].xpadd));
                write_to_buffer(d, buf, 0);
        if (++col == 2)
                {
                write_to_buffer(d, "\n\r",0);
                col = 0;
                }
        else
                write_to_buffer(d, "   ",0);
        }

        write_to_buffer(d,"\n\r",0);
	race = race_lookup(argument);

	write_to_buffer(d,"What is your race (type 'help' for more information)? ",0);
	d->connected = CON_GET_NEW_RACE;
	break;

    case CON_GET_NEW_RACE:
	one_argument(argument,arg);

	if (!str_cmp(arg,"help"))
	{
	    argument = one_argument(argument,arg);
	    if (argument[0] == '\0')
			do_help(ch,"races");
	    else
			do_help(ch,argument);
            write_to_buffer(d,"\n\rWhat is your race (type 'help' for more information)?",0);
	    break;
  	}

	race = race_lookup(argument);

	if (race < 1 || !race_table[race].pc_race || race == race_lookup("lich") || race == race_lookup("idiot"))
	{
	    write_to_buffer(d,"\n\rThat is not a valid race. Choose one of the following:\n\r",0);
        col = 0;
        for ( race = 1; race < MAX_PC_RACE; race++ )
        {
		if (race == race_lookup("lich") || race == race_lookup("idiot")) {
			continue;
		}
                ( (pc_race_table[race].xpadd == 0) ?
        sprintf(buf,"%-15s ( no extra xp)",pc_race_table[race].name) :
        sprintf(buf,"%-15s (%3d extra xp)",pc_race_table[race].name,
        pc_race_table[race].xpadd));
                write_to_buffer(d, buf, 0);
        if (++col == 2)
                {
                write_to_buffer(d, "\n\r",0);
                col = 0;
                }
        else
                write_to_buffer(d, "   ",0);
        }
            write_to_buffer(d,
                "\n\rWhat is your race? (type 'help' for more information)",0);
	    break;
	}
        ch->race = race;

	/* initialize stats */

	ch->race=race;
	copy_vector(ch->affected_by, race_data_lookup(race)->aff);
	copy_vector(ch->imm_flags, race_data_lookup(race)->imm);
	copy_vector(ch->res_flags, race_data_lookup(race)->res);
	copy_vector(ch->vuln_flags, race_data_lookup(race)->vuln);
	copy_vector(ch->form, race_data_lookup(race)->form);
	copy_vector(ch->parts, race_data_lookup(race)->parts);

	/* add skills */
	for (i = 0; i < 5; i++)
	{
	    if (pc_race_table[race].skills[i] == NULL)
	 	break;
	    sn = skill_lookup(pc_race_table[race].skills[i]);
	    ch->pcdata->learned[sn] = 100;
	}

	if (!str_cmp(pc_race_table[race].name,"minotaur"))
		gn_add(ch,group_lookup("minotaur"));
	
	if (!str_cmp(pc_race_table[race].name,"elf"))
		gn_add(ch,group_lookup("elf"));

	if (!str_cmp(pc_race_table[race].name,"drow"))
		gn_add(ch,group_lookup("drow"));

	ch->size = pc_race_table[race].size;
	write_to_buffer(d,"Please choose the adjective that best describes your character's appearance.\n\r"\
					  "Note that there is no gameplay advantage or disadvantage based on this choice.\n\r\n\r"\
					  "1)  Ugly\n\r"\
					  "2)  Unattractive\n\r"\
					  "3)  Ordinary\n\r"\
					  "4)  Pleasant-looking\n\r"\
					  "5)  Handsome\n\r"\
					  "6)  Striking\n\r"\
					  "----------------\n\r"\
					  "Choose: ", 0);
	d->connected = CON_GET_BEAUTY;
	break;

	case CON_GET_BEAUTY:
		if(!str_cmp(argument,"1")
		   || !str_cmp(argument,"2")
		   || !str_cmp(argument,"3")
		   || !str_cmp(argument,"4")
		   || !str_cmp(argument,"5")
		   || !str_cmp(argument,"6"))
		{
			ch->pcdata->beauty = atoi(argument) + 2;
			write_to_buffer( d, "\n\rWhat is your sex (M/F)? ", 0 );
			d->connected = CON_GET_NEW_SEX;
		}
		else
		{
			write_to_buffer( d, "That is not a valid option.\n\rChoose: ", 0);
			return;
		}
	break;


    case CON_GET_NEW_SEX:
	switch ( argument[0] )
	{
	case 'm': case 'M': ch->sex = SEX_MALE;
			    ch->pcdata->true_sex = SEX_MALE;
			    break;
	case 'f': case 'F': ch->sex = SEX_FEMALE;
			    ch->pcdata->true_sex = SEX_FEMALE;
			    break;
	default:
	    write_to_buffer( d, "That's not a sex.\n\rWhat IS your sex? ", 0 );
	    return;
	}

        write_to_buffer( d, "\n\rThe following classes are available:\n\r",0);

        col = 0;
        for (int classind = 1; classind < MAX_CLASS; classind++ )
        {
		CClass *tClass = CClass::GetClass(classind);
		if (CClass::GetClass(classind)->status == CLASS_CLOSED)
			continue;
                sprintf(buf,"%-15s ( no extra xp)",(RSTR)tClass->name);
                write_to_buffer(d, buf, 0);
        	if (++col == 2)
                {
                write_to_buffer(d, "\n\r",0);
                col = 0;
                }
        else
                write_to_buffer(d, "   ",0);
        }
        write_to_buffer(d,"\n\r\n\r",0);

        strcpy( buf, "Your race may be one of these classes:\n\r" );
	for ( iClass = 1; iClass < MAX_CLASS; iClass++ )
	{
	    if (pc_race_table[ch->race].classes[iClass] == 1 && CClass::GetClass(iClass)->status == CLASS_OPEN)
            {
                strcat( buf, (RSTR)CClass::GetClass(iClass)->name );
                strcat( buf, " " );
            }
	}
	strcat( buf, "\n\r" );
	write_to_buffer( d, buf, 0 );
	write_to_buffer( d, "\n\r",0);
	write_to_buffer(d,"Choose your class (type 'help' for more information): ",0);
	d->connected = CON_GET_NEW_CLASS;
	break;

    case CON_GET_NEW_CLASS:
	one_argument(argument,arg);
	if (!strcmp(arg,"help"))
	{
	    argument = one_argument(argument,arg);
	    if (argument[0] == '\0')
		do_help(ch,"class");
	    else
		do_help(ch,argument);
            write_to_buffer(d,
		"\n\rChoose your class (type 'help' for more information):",0);
	    break;
  	}
	iClass = CClass::Lookup(argument);
	if ( iClass == -1 || CClass::GetClass(iClass)->status == CLASS_CLOSED)
	{
	    write_to_buffer( d,
		"That's not a class.\n\rChoose your class (type 'help' for more information): ", 0 );
	    return;
    }

	if (pc_race_table[ch->race].classes[iClass] != 1)
    {
    	strcpy( buf, "Your race may only be one of these classes:\n\r" );
		for ( iClass = 1; iClass < MAX_CLASS; iClass++ )
		{
	    	if (pc_race_table[ch->race].classes[iClass] == 1 && CClass::GetClass(iClass)->status == CLASS_OPEN)
            	{
                	strcat( buf, (RSTR)CClass::GetClass(iClass)->name );
                	strcat( buf, " " );
            	}
		}
		strcat( buf, "\n\r" );
		write_to_buffer( d, buf, 0 );
        write_to_buffer(d,"What is your class? (type 'help' for more information): ",0);
	    return;
	}

   	ch->SetClass(iClass);
	sprintf( log_buf, "%s@%s new player.%s", ch->name, d->host,
	auto_check_multi(d,d->host) ? " (MULTI-CHAR?)" : "" );
	log_string(log_buf);
	login_log(ch,0);
	wiznet("Newbie alert!  $N sighted.",ch,NULL,WIZ_NEWBIE,0,0);
   	wiznet(log_buf,NULL,NULL,WIZ_SITES,0,get_trust(ch));
	
	if (ch->Class()->GetIndex() == CLASS_SORCERER) {
		ch->pcdata->ele_temp[1] = -1;
		ch->pcdata->ele_temp[2] = -1;
		ch->pcdata->ele_temp[3] = -1;
		write_to_buffer(d,"\n\rChoose your Thermal focus [Heat/Cold] (type 'help' for more information):",0);
		d->connected = CON_GET_THERMAL;
	} else if (ch->Class()->GetIndex() == CLASS_WARRIOR) {
		write_to_buffer(d,"\n\rChoose your starting weapon (this does not affect you in the long run):\n\r[ ",0);
		for(i=0; weapon_table[i].name; i++)
		{
			if(weapon_table[i].type == WEAPON_HAND)
				continue;
			write_to_buffer(d,weapon_table[i].name,0);
			write_to_buffer(d," ",0);
		}
		write_to_buffer(d,"] ",0);
		d->connected = CON_CHOOSE_WEAPON;
		ch->pcdata->start_weapon = -1;
		return;
	} else {
		ch->perm_stat[STAT_STR] = 12;
   		ch->perm_stat[STAT_INT] = 12;
   		ch->perm_stat[STAT_WIS] = 12;
   		ch->perm_stat[STAT_DEX] = 12;
   		ch->perm_stat[STAT_CON] = 12;
		ch->train = get_allocate_points(ch);
		show_allocate(ch,0);
   		d->connected = CON_ALLOCATE_STATS;
	}
	return;
	
	case CON_GET_THERMAL:
		one_argument(argument,arg);
		if (!strcmp(arg,"help")) {
			argument = one_argument(argument,arg);
			if (argument[0] == '\0' )
				do_help(ch,"261");
			else
				do_help(ch,argument);
			write_to_buffer(d,"\n\rChoose your Thermal focus [Heat/Cold] (type 'help' for more information):",0);
			break;
		}

		ele_num = ele_name_lookup(arg);
		
		if (ele_num != ELE_HEAT && ele_num != ELE_COLD) {	
			write_to_buffer(d,"That is not a valid element.\n\r"\
							  "Choose your Thermal focus [Heat/Cold] (type 'help' for more information):", 0);
			return;
		}

		ch->pcdata->ele_temp[1] = ele_num;
		
		sprintf(buf,"You have chosen a Thermal focus of %s.\n\r",sphere_table[ele_num].name);
		write_to_buffer(d,buf,0);
		sprintf(buf,"\n\rChoose your Material focus [Air/Earth] (type 'help' for more information):");
		write_to_buffer(d,buf,0);
		d->connected = CON_GET_MATERIAL;
		return;
	
	case CON_GET_MATERIAL:
		one_argument(argument,arg);
		if (!strcmp(arg,"help")) {
			argument = one_argument(argument,arg);
			if (argument[0] == '\0' )
				do_help(ch,"261");
			else
				do_help(ch,argument);
			write_to_buffer(d,"\n\rChoose your Material focus [Air/Earth] (type 'help' for more information):",0);
			break;
		}

		ele_num = ele_name_lookup(arg);
		
		if (ele_num != ELE_AIR && ele_num != ELE_EARTH) {
			write_to_buffer(d,"That is not a valid element.\n\r"\
					"Choose your Material focus [Air/Earth] (type 'help' for more information):",0);
			return;
		}

		ch->pcdata->ele_temp[2] = ele_num;
		
		sprintf(buf,"You have chosen a Material focus of %s.\n\r",sphere_table[ele_num].name);
		write_to_buffer(d,buf,0);
		sprintf(buf,"\n\rChoose your Dynamic focus [Water/Electricity] (type 'help' for more information:");
		write_to_buffer(d,buf,0);
		d->connected = CON_GET_DYNAMIC;
		return;
	

	case CON_GET_DYNAMIC:
		one_argument(argument,arg);
		if (!strcmp(arg,"help")) {
			argument = one_argument(argument,arg);
			if (argument[0] == '\0' )
				do_help(ch,"261");
			else
				do_help(ch,argument);
			write_to_buffer(d,"\n\rChoose your Dynamic focus [Water/Electricity] (type 'help' for more information):",0);
			break;
		}

		ele_num = ele_name_lookup(arg);
		
		if (ele_num != ELE_WATER && ele_num != ELE_ELECTRICITY) {
			write_to_buffer(d,"That is not a valid element.\n\r"\
					"Choose your Dynamic focus [Water/Electricity] (type 'help' for more information):",0);
			return;
		}

		ch->pcdata->ele_temp[3] = ele_num;
		
		sprintf(buf,"You have chosen a Dynamic focus of %s.\n\r",sphere_table[ele_num].name);
		write_to_buffer(d,buf,0);
			
		sprintf(buf,"\n\rYou have chosen to study the elements of %s, %s, and %s.\n\r"\
				"Please select which of the three you wish to make your Major focus.\n\r"\
				"The remaining two will combine to form your Para-Elemental Minor focus:",
				sphere_table[ch->pcdata->ele_temp[1]].name,
				sphere_table[ch->pcdata->ele_temp[2]].name,
				sphere_table[ch->pcdata->ele_temp[3]].name);
		write_to_buffer(d,buf,0);
		d->connected = CON_CHOOSE_ELE;
		return;

	case CON_CHOOSE_ELE:
		one_argument(argument,arg);

		if (!strcmp(arg,"help")) {
			argument = one_argument(argument,arg);
			if (argument[0] == '\0' )
				do_help(ch,"elements");
			else
				do_help(ch,argument);
			write_to_buffer(d,"\n\rChoose your elemental foci (type 'help' for more information):",0);
			break;
		}

		ele_num = ele_name_lookup(arg);
		
		if (!(ele_num == ch->pcdata->ele_temp[1]
			|| ele_num == ch->pcdata->ele_temp[2]
			|| ele_num == ch->pcdata->ele_temp[3])) {
			sprintf(buf,"That is not a valid element.\n\r"\
					"You have chosen to study the elements of %s, %s, and %s.\n\r"\
					"Please select which of the three you wish to make your Major focus.\n\r"\
					"The remaining two will combine to form your Para-Elemental Minor focus:",
					sphere_table[ch->pcdata->ele_temp[1]].name,
					sphere_table[ch->pcdata->ele_temp[2]].name,
					sphere_table[ch->pcdata->ele_temp[3]].name);
			write_to_buffer(d,buf,0);
			return;
		}

		if (ele_num == ch->pcdata->ele_temp[1]) {
			ch->pcdata->ele_major = ch->pcdata->ele_temp[1];
			ch->pcdata->ele_para = para_compute(ch->pcdata->ele_temp[2],
				ch->pcdata->ele_temp[3]);
		} else if (ele_num == ch->pcdata->ele_temp[2]) {
			ch->pcdata->ele_major = ch->pcdata->ele_temp[2];
			ch->pcdata->ele_para = para_compute(ch->pcdata->ele_temp[1],
				ch->pcdata->ele_temp[3]);
		} else if (ele_num == ch->pcdata->ele_temp[3]) {
			ch->pcdata->ele_major = ch->pcdata->ele_temp[3];
			ch->pcdata->ele_para = para_compute(ch->pcdata->ele_temp[1],
				ch->pcdata->ele_temp[2]);
		}

		gn = group_lookup(sphere_table[ch->pcdata->ele_major].name);
		gn_add(ch,gn);

		gn = group_lookup(sphere_table[ch->pcdata->ele_para].name);
		gn_add(ch,gn);
		
		sprintf(buf,"Your Major focus is %s.\n\r",sphere_table[ch->pcdata->ele_major].name);
		write_to_buffer(d,buf,0);
		sprintf(buf,"Your Para-Elemental focus is %s.\n\r",sphere_table[ch->pcdata->ele_para].name);
		write_to_buffer(d,buf,0);
		
		ch->perm_stat[STAT_STR] = 12;
   		ch->perm_stat[STAT_INT] = 12;
   		ch->perm_stat[STAT_WIS] = 12;
   		ch->perm_stat[STAT_DEX] = 12;
   		ch->perm_stat[STAT_CON] = 12;
		ch->train = get_allocate_points(ch);
		show_allocate(ch,0);
		d->connected = CON_ALLOCATE_STATS;
		return;
    case CON_CHOOSE_WEAPON:
		if((i=weapon_lookup(argument)) < 0 || weapon_table[i].type == WEAPON_HAND)
		{
			write_to_buffer(d,"\n\rInvalid weapon choice.  Please select from the following:\n\r[ ",0);
                	for(i=0; weapon_table[i].name; i++)
                	{
                	        write_to_buffer(d,weapon_table[i].name,0);
        	                write_to_buffer(d," ",0);   
	                }
			write_to_buffer(d,"] ",0);
			return;
		}
		sprintf(buf,"You have chosen to begin using a %s.\n\r",weapon_table[i].name);
		ch->pcdata->start_weapon = i;
		write_to_buffer(d, buf, 0);
                ch->perm_stat[STAT_STR] = 12;
                ch->perm_stat[STAT_INT] = 12;  
                ch->perm_stat[STAT_WIS] = 12;
                ch->perm_stat[STAT_DEX] = 12;
                ch->perm_stat[STAT_CON] = 12;
		ch->train = get_allocate_points(ch);
                show_allocate(ch,0);
                d->connected = CON_ALLOCATE_STATS;
	
		return;
    case CON_ALLOCATE_STATS:
		
		argument = one_argument(argument,word);
		if(word[0]=='\0') {
			write_to_buffer(d,"> ", 0);
			break;
		}
		
		if(str_prefix(word,"FINISH"))
		{
			modif = -1;
			strcpy(tword,word);
			argument = one_argument(argument,cword);		
			if(!str_cmp(tword,"STR"))
				modif = STAT_STR;
			if(!str_cmp(tword,"INT"))
				modif = STAT_INT;
			if(!str_cmp(tword,"WIS"))
				modif = STAT_WIS;
			if(!str_cmp(tword,"DEX"))
				modif = STAT_DEX;
			if(!str_cmp(tword,"CON"))
				modif = STAT_CON;
			if(modif<0)
			{
				write_to_buffer(d,"That's not a valid choice.\n\r> ",0);
				break;
			}
			modamt = 1;
			if(cword[0]!='\0')
				modamt = atoi(cword);
			if(modamt>ch->train)
			{
				write_to_buffer(d,"You don't have that many points left.\n\r> ",0);
				break;
			}
			if((ch->perm_stat[modif]+modamt)<12)
			{
				write_to_buffer(d,"Statistics cannot go below 12.\n\r> ",0);
				break;
			}
			if((ch->perm_stat[modif]+modamt)>pc_race_table[ch->race].max_stats[modif] &&
				(strcmp(pc_race_table[ch->race].name,"human") || modif != ch->Class()->attr_prime || 
					ch->perm_stat[modif]+modamt > pc_race_table[ch->race].max_stats[modif]+3))
			{
				write_to_buffer(d,"That statistic is already at its racial maximum.\n\r> ",0);
				break;
			}
			ch->train -= modamt;
			ch->perm_stat[modif] += modamt;
			show_allocate(ch,1);
			break;
		}

		if(!str_prefix(word,"FINISH")) {
			ch->mod_stat[STAT_STR]=0;
			ch->mod_stat[STAT_INT]=0;
			ch->mod_stat[STAT_WIS]=0;
			ch->mod_stat[STAT_DEX]=0;
			ch->mod_stat[STAT_CON]=0;

			if (pc_race_table[ch->race].align == ALIGN_ANY && ch->Class()->align == ALIGN_ANY) {
				write_to_buffer( d, "\n\r", 2 );
				write_to_buffer( d, "You may be good, neutral, or evil.\n\r",0);
				write_to_buffer( d, "Which alignment (G/N/E)? ",0);
			} else if ((pc_race_table[ch->race].align == ALIGN_GN
	&& ch->Class()->align == ALIGN_GN)
        || (pc_race_table[ch->race].align == ALIGN_ANY
	&& ch->Class()->align == ALIGN_GN)
        || (pc_race_table[ch->race].align == ALIGN_GN
	&& ch->Class()->align == ALIGN_ANY))
        {
            write_to_buffer( d, "\n\r", 2 );
            write_to_buffer( d, "You may be good or neutral.\n\r",0);
            write_to_buffer( d, "Which alignment (G/N)? ",0);
        }
        else if ((pc_race_table[ch->race].align == ALIGN_GE
        && ch->Class()->align == ALIGN_GE)
        || (pc_race_table[ch->race].align == ALIGN_ANY
        && ch->Class()->align == ALIGN_GE)
        || (pc_race_table[ch->race].align == ALIGN_GE
        && ch->Class()->align == ALIGN_ANY))
        {
            write_to_buffer( d, "\n\r", 2 );
            write_to_buffer( d, "You may be good or evil.\n\r",0);
            write_to_buffer( d, "Which alignment (G/E)? ",0);
        }
        else if ((pc_race_table[ch->race].align == ALIGN_NE
	&& ch->Class()->align == ALIGN_NE)
        || (pc_race_table[ch->race].align == ALIGN_ANY
	&& ch->Class()->align == ALIGN_NE)
        || (pc_race_table[ch->race].align == ALIGN_NE
	&& ch->Class()->align == ALIGN_ANY))
        {
            write_to_buffer( d, "\n\r", 2 );
            write_to_buffer( d, "You may be neutral or evil.\n\r",0);
            write_to_buffer( d, "Which alignment (N/E)? ",0);
        }
        else
        {
            write_to_buffer( d, "Your alignment is determined by your race.\n\r[Hit Return to Continue]", 0);
        }
        d->connected = CON_GET_ALIGNMENT;
        break;
	}
	write_to_buffer(d,"That's not a valid choice.\n\r",0);
	show_allocate(ch,0);
	write_to_buffer(d,"> ",0);
	break;

	case CON_GET_ALIGNMENT:
        if (pc_race_table[ch->race].align == ALIGN_ANY
	&& ch->Class()->align == ALIGN_ANY)
        {
	switch( argument[0])
	{
            case 'g' : case 'G' : ch->alignment = 1000;  break;
	    case 'n' : case 'N' : ch->alignment = 0;	break;
            case 'e' : case 'E' : ch->alignment = -1000; break;
	    default:
		write_to_buffer(d,"That's not a valid alignment.\n\r",0);
		write_to_buffer(d,"Which alignment (G/N/E)? ",0);
		return;
	}
        }
        else if ((pc_race_table[ch->race].align == ALIGN_GN
	&& ch->Class()->align == ALIGN_GN)
        || (pc_race_table[ch->race].align == ALIGN_ANY
	&& ch->Class()->align == ALIGN_GN)
        || (pc_race_table[ch->race].align == ALIGN_GN
	&& ch->Class()->align == ALIGN_ANY))
        {
	switch( argument[0])
	{
            case 'g' : case 'G' : ch->alignment = 1000;  break;
	    case 'n' : case 'N' : ch->alignment = 0;	break;
	    default:
		write_to_buffer(d,"That's not a valid alignment.\n\r",0);
                write_to_buffer(d,"Which alignment (G/N)? ",0);
		return;
	}
        }
        else if ((pc_race_table[ch->race].align == ALIGN_GE
        && ch->Class()->align == ALIGN_GE)
        || (pc_race_table[ch->race].align == ALIGN_ANY
        && ch->Class()->align == ALIGN_GE)
        || (pc_race_table[ch->race].align == ALIGN_GE
        && ch->Class()->align == ALIGN_ANY))
        {
        switch( argument[0])
        {
            case 'g' : case 'G' : ch->alignment = 1000;  break;
            case 'e' : case 'E' : ch->alignment = -1000;    break;
            default:
                write_to_buffer(d,"That's not a valid alignment.\n\r",0);
                write_to_buffer(d,"Which alignment (G/E)? ",0);
                return;
        }
        }
        else if ((pc_race_table[ch->race].align == ALIGN_NE
	&& ch->Class()->align == ALIGN_NE)
        || (pc_race_table[ch->race].align == ALIGN_ANY
	&& ch->Class()->align == ALIGN_NE)
        || (pc_race_table[ch->race].align == ALIGN_NE
	&& ch->Class()->align == ALIGN_ANY))
        {
	switch( argument[0])
	{
	    case 'n' : case 'N' : ch->alignment = 0;	break;
            case 'e' : case 'E' : ch->alignment = -1000; break;
	    default:
		write_to_buffer(d,"That's not a valid alignment.\n\r",0);
                write_to_buffer(d,"Which alignment (N/E)? ",0);
		return;
	}
        }
        else if ((pc_race_table[ch->race].align == ALIGN_G
	|| ch->Class()->align == ALIGN_G) ||
	(pc_race_table[ch->race].align == ALIGN_GN
	&& ch->Class()->align == ALIGN_GE))
        {
	write_to_buffer( d, "\n\rYou must be of good alignment.\n\r",0);
                ch->alignment = 1000;
        }
        else if (pc_race_table[ch->race].align == ALIGN_N
	|| ch->Class()->align == ALIGN_N
        || (pc_race_table[ch->race].align == ALIGN_GN
	&& ch->Class()->align == ALIGN_NE)
        || (pc_race_table[ch->race].align == ALIGN_NE
	&& ch->Class()->align == ALIGN_GN))
        {
	write_to_buffer( d, "\n\rYou must be of neutral alignment.\n\r",0);
                ch->alignment = 0;
        }
        else if ((pc_race_table[ch->race].align == ALIGN_E
	|| ch->Class()->align == ALIGN_E) ||
        (pc_race_table[ch->race].align == ALIGN_NE
        && ch->Class()->align == ALIGN_GE))
        {
	write_to_buffer( d, "\n\rYou must be of evil alignment.\n\r",0);
                ch->alignment = -1000;
        }
	if (ch->Class()->GetIndex() != CLASS_PALADIN)
	{
	write_to_buffer( d, "\n\r", 2 );
        write_to_buffer( d, "You may be lawful, neutral, or chaotic.\n\r",0);
        write_to_buffer( d, "Which ethos (L/N/C)? ",0);
	}
	else
	{
	write_to_buffer(d,"\n\r",0);
        write_to_buffer(d,"[Hit Return to Continue]",0);
	}
        d->connected = CON_GET_ETHOS;
	break;

	case CON_GET_ETHOS:
	if (ch->Class()->GetIndex() == CLASS_PALADIN)
	{
		write_to_buffer( d, "\n\rYou must be lawful.\n\r",0);
		ch->pcdata->ethos = 1000;
	}
	else
	{
	switch( argument[0])
	{
            case 'l' : case 'L' : ch->pcdata->ethos = 1000;  break;
            case 'n' : case 'N' : ch->pcdata->ethos = 0;    break;
            case 'c' : case 'C' : ch->pcdata->ethos = -1000; break;
	    default:
                write_to_buffer(d,"That's not a valid ethos.\n\r",0);
                write_to_buffer(d,"Which ethos (L/N/C)? ",0);
		return;
	}
	}
	write_to_buffer(d,"\n\r",0);
    group_add(ch,"class basics",FALSE);
    ch->pcdata->learned[gsn_recall] = 100;
    for (type = 0; weapon_table[type].name != NULL; type++)
    {
	if ((ch->Class()->GetIndex() == CLASS_WARRIOR && type == ch->pcdata->start_weapon) ||
    		(ch->Class()->GetIndex() != CLASS_WARRIOR && ch->Class()->weapon == weapon_table[type].vnum))
        		ch->pcdata->learned[*weapon_table[type].gsn] = 40;
    }
    SET_BIT(ch->comm,COMM_SHOW_AFFECTS);
	do_help(ch,"hometown");	
	sprintf(buf,"Your hometown can be: ");
	for (hometown = 0;hometown < MAX_HOMETOWN;hometown++)
	{
			if (can_live_in(ch,hometown))
			{
				strcat(buf,hometown_table[hometown].name);
				strcat(buf,", ");
			}
	}
	chop(buf); chop(buf);
	strcat(buf,".\n\rNew players should choose Cimar.\n\rChoose your hometown? ");
	write_to_buffer(d,buf,0);

	d->connected = CON_DEFAULT_CHOICE;
	break;

	case CON_DEFAULT_CHOICE:

	if (hometown_lookup(argument) && (can_live_in(ch,hometown_lookup(argument))))
	{
		ch->hometown = hometown_lookup(argument);
	}
	else
	{
		sprintf(buf,"Invalid selection. Your hometown can be: ");
		for (hometown = 0;hometown < MAX_HOMETOWN;hometown++)
		{
				if (can_live_in(ch,hometown))
				{
					strcat(buf,hometown_table[hometown].name);
					strcat(buf,", ");
				}
		}
		chop(buf); chop(buf);
		strcat(buf,".\n\rChoose your hometown? ");
		write_to_buffer(d,buf,0);
		return;
	}

    write_to_buffer(d,"\n\r",2);
	d->connected = CON_NEW_CHAR;
    case CON_NEW_CHAR:
	//motd
	do_help(ch,"motd");
	write_to_buffer(d,"[Hit Return to continue]\n\r",0);
	d->connected = CON_READ_MOTD;
	break;

    case CON_GEN_GROUPS:
	send_to_char("\n\r",ch);
        do_help(ch,"menu choice");
        break;

    case CON_READ_IMOTD:
	write_to_buffer(d,"\n\r",2);
	//motd
        do_help( ch, "motd" );
		write_to_buffer(d,"[Hit Return to continue]\n\r",0);
        d->connected = CON_READ_MOTD;
	break;

    case CON_READ_MOTD:
        if ( ch->pcdata == NULL || ch->pcdata->pwd[0] == '\0')
        {
            write_to_buffer( d, "Warning! Null password!\n\r",0 );
            write_to_buffer( d, "Please report old password with bug.\n\r",0);
            write_to_buffer( d,
                "Type 'password null <new password>' to fix.\n\r",0);
        }

	write_to_buffer( d, "\n\rWelcome to Riftshadow! Remember to wipe your feet!\n\r\n\r", 0 );
	ch->next	= char_list;
	char_list	= ch;
	d->connected	= CON_PLAYING;
	reset_char(ch);

	free_pstring(ch->pcdata->logon_time);
	ch->pcdata->logon_time = palloc_string(ctime( &current_time ));
	ch->pcdata->logon_time[strlen(ch->pcdata->logon_time)-1] = '\0';

	if ( ch->level == 0 )
	{
		/* Let's set up the basic auto-actions players usually set on logging in */
		SET_BIT(ch->act,PLR_AUTOASSIST);
		SET_BIT(ch->act,PLR_AUTOEXIT);
		SET_BIT(ch->act,PLR_AUTOGOLD);
		SET_BIT(ch->act,PLR_COLOR);
		SET_BIT(ch->act,PLR_AUTOABORT);
		SET_BIT(ch->comm,COMM_ANSI);
		send_to_char("Autoassist, autoexit, autoabort and autogold have been set.\n\r",ch);
		send_to_char("Type 'auto' to see a list of all auto-actions that may be set.\n\r",ch);

        ch->level           = START_LEVEL;
        ch->exp             = (ch->level-1) * exp_per_level(ch);
        ch->max_hit         = (ch->Class()->gainconst + 2) * 8; 
        ch->hit             = ch->max_hit;
        ch->pcdata->perm_hit= ch->max_hit;
        ch->max_mana        = UMAX(ch->perm_stat[STAT_INT]-10,11) * 15;
        ch->pcdata->perm_mana=ch->max_mana;
        ch->max_move        = (sh_int)((ch->perm_stat[STAT_DEX] + ch->perm_stat[STAT_CON]) * 7.5);
        ch->mana            = ch->max_mana;
        ch->move            = ch->max_move;
		if(ch->perm_stat[STAT_WIS] / 3 < 5)
			ch->practice = 5;
	 	else 
			ch->practice 		= ch->perm_stat[STAT_WIS] / 3;
		ch->true_name = palloc_string(ch->name);
		ch->pcdata->special	= 0;
		ch->ghost			= 0;
		ch->pcdata->shifted = -1;
	    ch->pcdata->energy_state= 0;
		ch->pcdata->condition[COND_THIRST] = 1;
		ch->pcdata->condition[COND_HUNGER] = 1;
	    ch->pcdata->roll_time	= current_time;
	    ch->pcdata->birth_date	= time_info.year;
	    ch->pcdata->death_time	= 400 + pc_race_table[ch->race].death_modifier * 25 + number_range(-35, 50);
	    ch->pcdata->style		= 0;
		ch->position = POS_STANDING;
		zero_vector(ch->pcdata->styles);
		zero_vector(ch->pcdata->ele_sphere);
		ch->pcdata->tribe		= 0;
		cres = RS.SQL.Insert("players VALUES('%s',%ld,%d,%d,%d,%d,%d,%d,%d,%d,0,0,0,0,0,0)",
				ch->true_name, ch->logon, ch->level,
				ch->Class()->GetIndex(), ch->race, ch->cabal, ch->sex, (int)(ch->played + current_time - ch->logon) / 3600,
				ch->alignment, ch->pcdata->ethos);
	    sprintf( buf, "the %s",
		title_table [ch->Class()->GetIndex()] [ch->level]
		[ch->sex == SEX_FEMALE ? 1 : 0] );
	    set_title( ch, buf );
	    save_char_obj(ch);
	    char_to_room( ch, get_room_index(24525));
	    send_to_char("\n\r",ch);
	    do_help(ch,"newbie");
	    send_to_char("\n\r",ch);
	    do_outfit(ch,"");
       	    for (i=0; i < MAX_STORED_ITEMS; i++)
	          ch->pcdata->deposited_items[i] = 0;
	    if(ch->Class()->GetIndex() == CLASS_ANTI_PALADIN)
		ch->pcdata->learned[skill_lookup("steal")] = -2;
		if(!str_cmp(race_table[ch->race].name,"minotaur"))
		{
			ch->pcdata->learned[skill_lookup("gore")] = 70;
			ch->pcdata->learned[skill_lookup("headbutt")] = 70;
		}
		else if(!str_cmp(race_table[ch->race].name,"elf") || !str_cmp(race_table[ch->race].name,"drow"))
			ch->pcdata->learned[skill_lookup("silent movement")] = 70;
		ch->gold = 500;
		RS.Queue.AddToQueue(3, 1, create_academy_pet, ch);
		//academy pet here, on queue
	}
	else if ( ch->in_room != NULL )
	{
	    char_to_room( ch, ch->in_room );
	}
	else if ( IS_IMMORTAL(ch) )
	{
	    char_to_room( ch, get_room_index( ROOM_VNUM_CHAT ) );
	}
	else
	{
	    char_to_room( ch, get_room_index( 24525 ) );
	}

	ch->pcdata->host = palloc_string(ch->desc->host);

	if(!IS_IMMORTAL(ch))
	act( "$n awakens into the world of Shalar.", ch, NULL, NULL, TO_ROOM );

	group_add(ch,"class basics",FALSE);

    race = ch->race;
	
	ch->pcdata->condition[COND_DEHYDRATED] = dh;

	/* Destroy all cabal items...make sure players don't relog with items, can
	   occur if mud crashes, etc. Don't want cabal items being brought in while
	   another one exists.
	*/
	{
	OBJ_DATA *obj;
	OBJ_DATA *obj_next;
	for (obj = object_list; obj != NULL; obj = obj_next)
	{
	obj_next = obj->next;
	if (obj->carried_by == ch)
		{
		if (isCabalItem(obj))
			{
			extract_obj(obj);
			}
		else if (obj->pIndexData->limtotal != 0 && ch->level < 10)
			extract_obj(obj);
		}
	}
	}
	do_look( ch, "auto" );
	ch->pcdata->old_room=0;
	sprintf(buf,"$N (%s) has left real life behind.",(ch->cabal>0) ? capitalize(cabal_table[ch->cabal].name) : "Uncaballed");
	wiznet(buf,ch,NULL,WIZ_LOGINS,0,get_trust(ch));
	if(IS_IMMORTAL(ch))
	{
		do_unread(ch,"");
		do_listoffer(ch,"auto");
		if(IS_SET(ch->act, PLR_CODER))
                	do_buglist(ch,"auto");
		break;
	}
	sprintf(buf,"players SET logins=logins+1, lastlogin=%d", (int)ch->logon);
/*	if(ch->cabal>0)
	{
		if(cabal_down_new(ch,ch->cabal,FALSE))
			strcat(buf,", no_clogins=no_clogins+1");
		else
			strcat(buf,", c_logins=c_logins+1");
	} */
	sprintf(buffile,"%s WHERE name = '%s'",buf,ch->true_name);
	cres = RS.SQL.Update(buffile);

	if (ch->pet != NULL)
	{
	    char_to_room(ch->pet,ch->in_room);
	    act("$n awakens into the world of Shalar.",ch->pet,NULL,NULL,TO_ROOM);
	}

        if (ch->cabal != 0)
		announce_login(ch);
	//grep for LOWBIE LEVEL
	//note: the advance code will be in update, per tick

	if(ch->Class()->GetIndex() == CLASS_ZEALOT ||
			ch->Class()->GetIndex() == CLASS_HEALER ||
			ch->Class()->GetIndex() == CLASS_PALADIN) {
		SET_BIT(ch->act, PLR_EMPOWERED);
	}
	
	do_unread(ch,"");
	break;
    }

    return;
}



/*
 * Parse a name for acceptability.
 */
bool check_parse_name( char *name )
{
    /*
     * Reserved words.
     */
    if ( is_name( name,
	"all auto immortal self zzz someone something the you scion theatre horde bounty common anti-paladin sorcerer warrior thief paladin zealot healer necromancer chronomancer") )
		return FALSE;
	
    /*
     * Length restrictions.
     */

    if ( strlen(name) <  2 )
		return FALSE;

    if ( strlen(name) > 12 )
		return FALSE;

    /*
     * Alphanumerics only.
     * Lock out IllIll twits.
     */
    {
	char *pc;
	bool fIll,adjcaps = FALSE,cleancaps = FALSE;
 	unsigned int total_caps = 0;

	fIll = TRUE;
	for ( pc = name; *pc != '\0'; pc++ )
	{
	    if ( !isalpha(*pc) )
		return FALSE;

	    if ( isupper(*pc)) /* ugly anti-caps hack */
	    {
		if (adjcaps)
		    cleancaps = TRUE;
		total_caps++;
		adjcaps = TRUE;
	    }
	    else
		adjcaps = FALSE;

	    if ( LOWER(*pc) != 'i' && LOWER(*pc) != 'l' )
		fIll = FALSE;
	}

	if ( fIll )
	    return FALSE;

	if (cleancaps || (total_caps > (strlen(name)) / 2 && strlen(name) < 3))
	    return FALSE;
    }

    /*
     * Prevent players from naming themselves after mobs.
     */
    {
	extern MOB_INDEX_DATA *mob_index_hash[MAX_KEY_HASH];
	MOB_INDEX_DATA *pMobIndex;
	int iHash;

	for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
	{
	    for ( pMobIndex  = mob_index_hash[iHash];
		  pMobIndex != NULL;
		  pMobIndex  = pMobIndex->next )
	    {
		if ( is_name( name, pMobIndex->player_name ) )
		    return FALSE;
	    }
	}
    }

    return TRUE;
}



/*
 * Look for link-dead player to reconnect.
 */
bool check_reconnect( DESCRIPTOR_DATA *d, char *name, bool fConn )
{
    CHAR_DATA *ch, *fch, *fch_next;
    OBJ_DATA *obj;

    for ( ch = char_list; ch != NULL; ch = ch->next )
    {
	if ( !IS_NPC(ch)
	&&   (!fConn || ch->desc == NULL)
	&&   !str_cmp( (d->character->true_name ?
		d->character->true_name: d->character->name),
		 (ch->true_name?
		ch->true_name : ch->name ) ))
	{
	    if ( fConn == FALSE )
	    {
		free_pstring( d->character->pcdata->pwd );
		d->character->pcdata->pwd = palloc_string( ch->pcdata->pwd );
	    }
	    else
	    {
		for(fch = char_list; fch; fch = fch_next)
		{
			fch_next = fch->next;
			if(IS_NPC(fch) && (is_affected(fch,gsn_animate_dead) || IS_AFFECTED(fch,AFF_CHARM))
			    && fch->master == d->character)
				 extract_char(fch,TRUE);
		}
		free_char( d->character );
		d->character = ch;
		ch->desc	 = d;
		ch->timer	 = 0;
		send_to_char(
		    "Reconnecting. Type replay to see missed tells.\n\r", ch );
		if(ch->invis_level < 51)
			act( "$n has reconnected.", ch, NULL, NULL, TO_ROOM );
		ch->pcdata->host = palloc_string(ch->desc->host);
/* Limit crap to balance reconnect objects from extracted link object */
	for (obj = ch->carrying; obj != NULL; obj = obj->next_content)
	{
	obj->pIndexData->limcount++;
	}

		sprintf( log_buf, "%s@%s reconnected.", ch->name, d->host );
		log_string( log_buf );
		wiznet("$N recovers from link death.",
		    ch,NULL,WIZ_LINKS,0,get_trust(ch));
		d->connected = CON_PLAYING;
	    }
	    return TRUE;
	}
    }

    return FALSE;
}



/*
 * Check if already playing.
 */
bool check_playing( DESCRIPTOR_DATA *d, char *name )
{
    DESCRIPTOR_DATA *dold;

    for ( dold = descriptor_list; dold; dold = dold->next )
    {
	if ( dold != d
	&&   dold->character != NULL
	&&   dold->connected != CON_GET_NAME
	&&   dold->connected != CON_GET_OLD_PASSWORD
	&&   !str_cmp( name, dold->original
	         ? dold->original->true_name : dold->character->true_name) )
	{
	    write_to_buffer( d, "That character is already playing.\n\r",0);
	    write_to_buffer( d, "Do you wish to connect anyway (Y/N)?",0);
	    d->connected = CON_BREAK_CONNECT;
	    return TRUE;
	}
    }

    return FALSE;
}



void stop_idling( CHAR_DATA *ch )
{
    if ( ch == NULL
    ||   ch->desc == NULL
    ||   ch->desc->connected != CON_PLAYING
    ||   ch->was_in_room == NULL
    ||   ch->in_room != get_room_index(ROOM_VNUM_LIMBO))
	return;

    ch->timer = 0;
    char_from_room( ch );
    char_to_room( ch, ch->was_in_room );
    ch->was_in_room	= NULL;
    act( "$n has returned from the void.", ch, NULL, NULL, TO_ROOM );
    return;
}



/*
 * Write to one char.
 */
void send_to_char( const char *txt, CHAR_DATA *ch )
{
    if ( txt != NULL && ch->desc != NULL )
        write_to_buffer( ch->desc, txt, strlen(txt) );
    return;
}

void send_to_chars( const char *txt, CHAR_DATA *ch, int min, ... )
{
    if ( txt != NULL && ch->desc != NULL )
        write_to_buffer( ch->desc, txt, strlen(txt) );
    return;
}

/*
 * Send a page to one char.
 */
void page_to_char( const char *txt, CHAR_DATA *ch )
{
    if ( txt == NULL || ch->desc == NULL)
        return;

    if (ch->lines == 0 )
    {
	send_to_char(txt,ch);
	return;
    }
	
    ch->desc->showstr_head  = new char[strlen(txt) + 1];
    strcpy(ch->desc->showstr_head,txt);
    ch->desc->showstr_point = ch->desc->showstr_head;
    show_string(ch->desc,"");
}


/* string pager */
void show_string(struct descriptor_data *d, char *input)
{
    char buffer[4*MAX_STRING_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    register char *scan, *chk;
    int lines = 0, toggle = 1;
    int show_lines;

    one_argument(input,buf);
    if (buf[0] != '\0')
    {
	if (d->showstr_head)
	{
	    delete[] d->showstr_head;
	    d->showstr_head = 0;
	}
    	d->showstr_point  = 0;
	return;
    }

    if (d->character)
	show_lines = d->character->lines;
    else
	show_lines = 0;

    for (scan = buffer; ; scan++, d->showstr_point++)
    {
	if (((*scan = *d->showstr_point) == '\n' || *scan == '\r')
	    && (toggle = -toggle) < 0)
	    lines++;

	else if (!*scan || (show_lines > 0 && lines >= show_lines))
	{
	    *scan = '\0';
	    write_to_buffer(d,buffer,strlen(buffer));
	    for (chk = d->showstr_point; isspace(*chk); chk++);
	    {
		if (!*chk)
		{
		    if (d->showstr_head)
        	    {
            		delete[] d->showstr_head;
            		d->showstr_head = 0;
        	    }
        	    d->showstr_point  = 0;
    		}
	    }
	    return;
	}
    }
    return;
}
	

/* quick sex fixer */
void fix_sex(CHAR_DATA *ch)
{
    if (ch->sex < 0 || ch->sex > 2)
    	ch->sex = IS_NPC(ch) ? 0 : ch->pcdata->true_sex;
}

void act (const char *format, CHAR_DATA *ch, const void *arg1, const void *arg2,
	  int type)
{
    /* to be compatible with older code */
    act_new(format,ch,arg1,arg2,type,POS_RESTING);
}

void act_area( const char *format, CHAR_DATA *ch, CHAR_DATA *victim)
{
    static char * const he_she  [] = { "it",  "he",  "she" };
    static char * const him_her [] = { "it",  "him", "her" };
    static char * const his_her [] = { "its", "his", "her" };
    char buf[MAX_STRING_LENGTH], buf2[MSL];
    CHAR_DATA *to;
    const char *str;
    const char *i;
    char *point;
	DESCRIPTOR_DATA *d;

    /*
     * Discard null and zero-length messages.
     */
    if ( format == NULL || format[0] == '\0' )
        return;

	/*colorconv(format, format, ch);*/
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
		if ( d->connected == CON_PLAYING
			 && d->character
			 && d->character->in_room != NULL
			 && d->character->in_room->area == ch->in_room->area
			 && !IS_SET(d->character->comm,COMM_QUIET))
		{
			if (IS_SET(d->character->in_room->room_flags, ROOM_SILENCE))
				continue;

			if(!IS_AWAKE(d->character))
				continue;

			to = d->character;

			if ( (!IS_NPC(to) && to->desc == NULL )) 
        		continue;

			point = buf;
        	str = format;
        	while ( *str != '\0' )
        	{
            	if ( *str != '$' )
            	{
                	*point++ = *str++;
                	continue;
            	}
            	++str;

            	if ( *str >= 'A' && *str <= 'Z' )
            	{
                	bug( "Act: missing arg2 for code %d.", *str );
                	i = " <@@@> ";
            	}
            	else
            	{
                	switch ( *str )
                	{
                		default:  
							bug( "Act: bad code %d.", *str );
                        	i = " <@@@> ";
							break;
                		/* Thx alex for 't' idea */
                		case 'n': i = get_descr_form(victim,ch,FALSE); break;
                		case 'e': i = he_she  [URANGE(0, victim->sex, 2)];        break;
                		case 'm': i = him_her [URANGE(0, victim->sex, 2)];        break;
                		case 's': i = his_her [URANGE(0, victim->sex, 2)];        break;
                	}
            	}

            	++str;
            	while ( ( *point = *i ) != '\0' )
                	++point, ++i;
        	}

        	*point   = '\0';

			if ( to->desc != NULL )
			{
				sprintf(buf2,"%s yells '%s", ch->short_descr, get_char_color(to, "yells"));
				buf2[0] = UPPER(buf2[0]); 
				send_to_char(buf2, to);
            	write_to_buffer( to->desc, buf, point - buf );
				sprintf(buf2,"%s'\n\r", END_COLOR(to));
				send_to_char(buf2, to);
			}
		}
    }

    return;
}

void act_new( const char *format, CHAR_DATA *ch, const void *arg1,
	      const void *arg2, int type, int min_pos)
{
    static char * const he_she  [] = { "it",  "he",  "she" };
    static char * const him_her [] = { "it",  "him", "her" };
    static char * const his_her [] = { "its", "his", "her" };
	
    char buf[MAX_STRING_LENGTH], buf2[100];
    char fname[MAX_INPUT_LENGTH];
    CHAR_DATA *to;
    CHAR_DATA *vch = (CHAR_DATA *) arg2;
    OBJ_DATA *obj1 = (OBJ_DATA  *) arg1;
    OBJ_DATA *obj2 = (OBJ_DATA  *) arg2;
    const char *str;
    const char *i;
    char *point;

    /*
     * Discard null and zero-length messages.
     */
    if ( format == NULL || format[0] == '\0' )
        return;

    /* discard null rooms and chars */
    if (ch == NULL || ch->in_room == NULL)
		return;
    /*colorconv(format, format, ch);*/
    to = ch->in_room->people;
    if ( type == TO_VICT )
    {
        if ( vch == NULL )
        {
			sprintf(buf,"Act: null vch with TO_VICT. -- %s", format);
			bug(buf,0);
            return;
        }

		if (vch->in_room == NULL)
	    	return;

        to = vch->in_room->people;
    }

    for ( ; to != NULL; to = to->next_in_room )
    {
        if ( (!IS_NPC(to) && to->desc == NULL )
        ||    to->position < min_pos )
            continue;

        if ( (type == TO_CHAR) && to != ch )
            continue;
        if ( type == TO_VICT && ( to != vch || to == ch ) )
            continue;
        if ( type == TO_ROOM && to == ch )
            continue;
        if ( type == TO_NOTVICT && (to == ch || to == vch) )
            continue;
        if ( type == TO_IMMINROOM && (!(IS_IMMORTAL(to)) || (to == ch) || !(can_see(to,ch))))
            continue;
		if ( type == TO_GROUP && !is_same_group(to,ch))
			continue;
		if ( type == TO_NOTGROUP && is_same_group(to,ch))
			continue;
		if ( (type == TO_GROUP || type == TO_NOTGROUP) && to==ch)
			continue;
		point   = buf;
        str     = format;
        while ( *str != '\0' )
        {
            if ( *str != '$' )
            {
                *point++ = *str++;
                continue;
            }
            ++str;

            if ( arg2 == NULL && *str >= 'A' && *str <= 'Z' && *str != 'I')
            {
                bug( "Act: missing arg2 for code %d.", *str );
                i = " <@@@> ";
            }
            else
            {
                switch ( *str )
                {
                default:  bug( "Act: bad code %d.", *str );
                          i = " <@@@> ";                                break;
                /* Thx alex for 't' idea */
                case 't': i = (char *) arg1;                            break;
                case 'T': i = (char *) arg2;                            break;
                case 'n':
			i = get_descr_form(ch,to,FALSE);
			break;
                case 'N':
			i = get_descr_form(vch,to,FALSE);
			break;
		case 'f' :
			i = (ch->true_name ? ch->true_name:ch->name);
			break;
		case 'F' :
			i = (vch->true_name ? vch->true_name:vch->name);
			break;

		case 'i':
			sprintf(buf2,"%d",(int)arg1);
			i = (const char *)&buf2;
			break;

		case 'I':
			sprintf(buf2,"%d",(int)arg2);
			i = (const char*)&buf2;
			break;

            case 'e': i = he_she  [URANGE(0, ch  ->sex, 2)];        break;
            case 'E': i = he_she  [URANGE(0, vch ->sex, 2)];        break;
            case 'm': i = him_her [URANGE(0, ch  ->sex, 2)];        break;
            case 'M': i = him_her [URANGE(0, vch ->sex, 2)];        break;
            case 's': i = his_her [URANGE(0, ch  ->sex, 2)];        break;
            case 'S': i = his_her [URANGE(0, vch ->sex, 2)];        break;

                case 'p':
		    if(!obj1)
		    {
			i="something";
			break;
		    }
                    i = can_see_obj( to, obj1 )
                            ? obj1->short_descr
                            : "something";
		    //obj article truncation: back up 2-don't truncate the/a from obj short descr unless it's preceded by Your or 's
		    if(((str - format) >= 4 && *(str - 4) == '\'' && *(str - 3) == 's')
			|| ((str - format) >= 6 && *(str - 5) == 'o' && *(str - 4) == 'u' && *(str - 3) == 'r'))
		    {
			if(!str_prefix("the ", i))
				i += 4;
			if(!str_prefix("a ", i))
				i += 2;
		    }
                    break;

                case 'P':
                    i = can_see_obj( to, obj2 )
                            ? obj2->short_descr
                            : "something";
                    break;

                case 'd':
                    if ( arg2 == NULL || ((char *) arg2)[0] == '\0' )
                    {
                        i = "door";
                    }
                    else
                    {
                        one_argument( (char *) arg2, fname );
                        i = fname;
                    }
                    break;
                }
            }

            ++str;
            while ( ( *point = *i ) != '\0' )
                ++point, ++i;
        }

        *point++ = '\n';
        *point++ = '\r';
        *point   = '\0';

		if (!str_prefix("\x01B[0m",buf))
			buf[4] = UPPER(buf[4]);
		else if (!str_prefix("\x01B",buf))
			buf[7] = UPPER(buf[7]);
		else
			buf[0]   = UPPER(buf[0]);
        if ( to->desc != NULL )
            write_to_buffer( to->desc, buf, point - buf );
    }

    return;
}

void announce_login(CHAR_DATA *ch)
{
        CHAR_DATA *guardian;
	  char *rstring, buf[MAX_STRING_LENGTH],  rbuf[MAX_STRING_LENGTH];
        if (ch->cabal == 0 || ch->cabal > MAX_CABAL || IS_IMMORTAL(ch))
        return;
	guardian = get_cabal_guardian(ch->cabal);
if(!guardian)
	return;
	rstring = talloc_string(cabal_messages[ch->cabal].login);
	sprintf(buf,rstring,ch->name);
	sprintf(rbuf,"\x01B[1;37m%s\x01B[0;37m",buf);
	do_cb(guardian,buf);
}


void announce_logout(CHAR_DATA *ch)
{
        CHAR_DATA *guardian;
          char *rstring, buf[MAX_STRING_LENGTH],  rbuf[MAX_STRING_LENGTH];
        if (ch->cabal == 0 || ch->cabal > MAX_CABAL || IS_IMMORTAL(ch))
        return;
        guardian = get_cabal_guardian(ch->cabal);
if(!guardian)
        return;
        rstring = talloc_string(cabal_messages[ch->cabal].logout);
        sprintf(buf,rstring,ch->name);
        sprintf(rbuf,"\x01B[1;37m%s\x01B[0;37m",buf);
        do_cb(guardian,buf);
}

void do_rename (CHAR_DATA* ch, char* argument)
{
	char old_name[MAX_INPUT_LENGTH], new_name[MAX_INPUT_LENGTH], strsave [MAX_INPUT_LENGTH], pbuf[MSL], *cname;
	CHAR_DATA *victim;
	int cres=0;
	argument = one_argument(argument, old_name);
	one_argument(argument, new_name);

	if (old_name[0] == '\0')
	{
		send_to_char ("Rename who?\n\r",ch);
		return;
	}

	if (new_name[0] == '\0')
	{
		send_to_char ("Rename to what new name?\n\r",ch);
		return;
	}

	victim = get_char_world (ch, old_name);

	if (!victim)
	{
		send_to_char ("There is no such a person online.\n\r",ch);
		return;
	}

	if (IS_NPC(victim))
	{
		send_to_char("Use string.\n\r",ch);
		return;
	}

	if ((victim != ch) && (get_trust (victim) >= get_trust(ch)))
	{
		send_to_char ("You failed.\n\r",ch);
		return;
	}

	if (!victim->desc || (victim->desc->connected != CON_PLAYING) )
	{
		send_to_char("They are link-dead.\n\r",ch);
		return;
	}

	if (!check_parse_name(new_name))
	{
		send_to_char("That new name is illegal.\n\r",ch);
		return;
	}

	if(is_affected(victim,gsn_cloak_form) || IS_SHIFTED(victim))
		return send_to_char("They must be in normal form.\n\r",ch);

	sprintf(strsave, "%s%s.plr", PLAYER_DIR, capitalize(new_name));

	if (fopen(strsave,"r") != NULL)
	{
		send_to_char ("A player with that name already exists.\n\r",ch);
		return;
	}

	if (get_char_world(ch,new_name))
	{
		send_to_char ("A player with the name you specified already exists.\n\r",ch);
		return;
	}
	cres = RS.SQL.Update("players SET name = '%s' WHERE name = '%s' LIMIT 1", capitalize(new_name), victim->true_name);
	cname	= palloc_string(victim->true_name);
	free_pstring(victim->name);
	free_pstring(victim->true_name);
	free_pstring(victim->backup_true_name);
	victim->name = palloc_string(capitalize(new_name));
	victim->true_name = palloc_string(capitalize(new_name));
	victim->backup_true_name = palloc_string(capitalize(new_name));
	save_char_obj(victim);
	delete_char(cname, FALSE);
//	unlink(strsave);
	send_to_char ("Character renamed.\n\r",ch);
	act("$n has renamed you to $N!",ch,NULL,victim,TO_VICT);

	return;
}

void do_renam(CHAR_DATA *ch,char *argument)
{
send_to_char("If you want to RENAME an existing player your must type rename in full.\n\r",ch);
send_to_char("rename <current name> <new name>\n\r",ch);

return;
}


int color( char type, CHAR_DATA *ch, char *string )
{
    char	code[ 20 ];
    char	*p = '\0';

    if( IS_NPC( ch ) && !IS_SET(ch->act,ACT_WARD_MOB))
	return( 0 );

    switch( type )
    {
	default:
	    sprintf( code, CLEAR );
	    break;
	case 'x':
	    sprintf( code, CLEAR );
	    break;
	case 'b':
	    sprintf( code, C_BLUE );
	    break;
	case 'c':
	    sprintf( code, C_CYAN );
	    break;
	case 'g':
	    sprintf( code, C_GREEN );
	    break;
	case 'm':
	    sprintf( code, C_MAGENTA );
	    break;
	case 'r':
	    sprintf( code, C_RED );
	    break;
	case 'w':
	    sprintf( code, C_WHITE );
	    break;
	case 'y':
	    sprintf( code, C_YELLOW );
	    break;
	case 'B':
	    sprintf( code, C_B_BLUE );
	    break;
	case 'C':
	    sprintf( code, C_B_CYAN );
	    break;
	case 'G':
	    sprintf( code, C_B_GREEN );
	    break;
	case 'M':
	    sprintf( code, C_B_MAGENTA );
	    break;
	case 'R':
	    sprintf( code, C_B_RED );
	    break;
	case 'W':
	    sprintf( code, C_B_WHITE );
	    break;
	case 'Y':
	    sprintf( code, C_B_YELLOW );
	    break;
	case 'D':
	    sprintf( code, C_D_GREY );
	    break;
	case '*':
	    sprintf( code, "%c", 007 );
	    break;
	case '/':
	    sprintf( code, "%c", 012 );
	    break;
	case '{':
	    sprintf( code, "%c", '{' );
	    break;
	case '|':
	    sprintf( code, "\n\r");
	    break;
    }

    p = code;
    while( *p != '\0' )
    {
	*string = *p++;
	*++string = '\0';
    }

    return( strlen( code ) );
}

void colorconv( char *buffer, const char *txt, CHAR_DATA *ch )
{
    const	char	*point;
		int	skip = 0;

    if( txt )
    {
	if( IS_SET( ch->act, PLR_COLOR ) || IS_NPC(ch))
	{
	    for( point = txt ; *point ; point++ )
	    {
		if( *point == '{' )
		{
		    point++;
		    skip = color( *point, ch, buffer );
		    while( skip-- > 0 )
			++buffer;
		    continue;
		}
		*buffer = *point;
		*++buffer = '\0';
	    }			
	    *buffer = '\0';
	}
	else
	{
	    for( point = txt ; *point ; point++ )
	    {
		if( *point == '{' )
		{
		    point++;
		    continue;
		}
		*buffer = *point;
		*++buffer = '\0';
	    }
	    *buffer = '\0';
	}
    }
    return;
}

int get_allocate_points (CHAR_DATA *ch)
{
	int allocate = 0, i;
	for(i=STAT_STR;i<MAX_STATS;i++)
		allocate+=(pc_race_table[ch->race].max_stats[i]-13);
	if(!strcmp(pc_race_table[ch->race].name,"human"))
		allocate+=2;
	return allocate;
}

void show_allocate (CHAR_DATA *ch, int finish)
{
	char buf[MSL];

        if(!finish) {
                write_to_buffer(ch->desc,"Type '<str/int/wis/dex/con> <amount>' to modify a particular stat.\n\r",0);
                write_to_buffer(ch->desc,"The default amount is 1, use negative numbers to deallocate points.\n\r",0);
                write_to_buffer(ch->desc,"Type 'finish' when you're done.\n\r",0);
		write_to_buffer(ch->desc,"\n\rTime to allocate points to your stats.\n\r",0);
	}
	sprintf(buf, "You have %d points to allocate.\n\r",ch->train);
	write_to_buffer(ch->desc, buf, 0);

sprintf(buf,"Your stats: Str: %d (Max %d)  Int: %d (Max %d)  Wis: %d (Max %d)  Dex: %d (Max %d)  Con: %d (Max %d)\n\r",
	ch->perm_stat[STAT_STR], pc_race_table[ch->race].max_stats[STAT_STR],
	ch->perm_stat[STAT_INT], pc_race_table[ch->race].max_stats[STAT_INT],
	ch->perm_stat[STAT_WIS], pc_race_table[ch->race].max_stats[STAT_WIS],
	ch->perm_stat[STAT_DEX], pc_race_table[ch->race].max_stats[STAT_DEX],
	ch->perm_stat[STAT_CON], pc_race_table[ch->race].max_stats[STAT_CON]);
	send_to_char(buf,ch);

	write_to_buffer(ch->desc,"> ", 0);
}

void process_text(CHAR_DATA *ch, char *text)
{
	char obuf[MSL*2], tbuf[MSL], rbuf[MSL];
	bool found = FALSE;
	unsigned int len, rcount, i, iloc;
	if(!str_cmp(text,""))
		return;
	if(!str_cmp(text,"exit"))
	{
		ch->pcdata->entering_text = FALSE;
		ch->pcdata->entered_text[0] = '\0';
		ch->pcdata->end_fun = NULL;
		send_to_char("Exiting without saving changes.\n\r",ch);
		return;
	}
	if(!str_cmp(text,"finish"))
	{
		send_to_char("Saving changes and exiting...\n\r",ch);
		if(ch->pcdata->end_fun)
			(*ch->pcdata->end_fun) (ch, ch->pcdata->entered_text);
		ch->pcdata->entering_text = FALSE;
		ch->pcdata->end_fun = NULL;
		return;
	}
	if(!str_cmp(text,"backline"))
	{
                if (ch->pcdata->entered_text == NULL || ch->pcdata->entered_text[0] == '\0')
                        return send_to_char("No lines left to remove.",ch);
                strcpy(obuf,ch->pcdata->entered_text);
                for (len = strlen(obuf); len > 0; len--)
                {
                        if (obuf[len] == '\r')
                        {
                                if (!found)
                                {
                                        if (len > 0)
                                                len--;
                                        found = TRUE;
                                }
                                else
                                {
                                        obuf[len + 1] = '\0';
                                        free_pstring(ch->pcdata->entered_text);
                                        ch->pcdata->entered_text = palloc_string(obuf);
                                        return send_to_char("Line deleted.",ch);
                                }
                        }
                }
                obuf[0] = '\0';
                free_pstring(ch->pcdata->entered_text);
                ch->pcdata->entered_text = palloc_string(obuf);
                send_to_char("Line deleted.",ch);
		return send_to_char(ch->pcdata->entered_text,ch);
	}
	if(!str_cmp(text,"show"))
		return send_to_char(ch->pcdata->entered_text,ch);

	if (text[0] != '\0')
    {
		obuf[0] = '\0';
		if(strlen(text)>80)
		{
			sprintf(tbuf, "%s", text);
			for(i = 1; i<=strlen(text) / 80; i++)
			{
				for(rcount = 80 * i; rcount > (80 * i) - 15; rcount--)
					if(tbuf[rcount] == ' ')
						break;
				if(rcount <= (80 * i) - 15)
					rcount = 80 * i;
				for(iloc = rcount; tbuf[iloc + 1]; iloc++)
					rbuf[iloc - rcount] = tbuf[iloc + 1];
				rbuf[iloc - rcount] = '\0';
				tbuf[rcount] 	= '\n';
				tbuf[rcount+1] 	= '\r';
				tbuf[rcount+2]	= '\0';
				strcat(tbuf,rbuf);
			}
			send_to_char("Over 80 characters, wrapping line.\n\r",ch);
		}

		if (ch->pcdata->entered_text)
	    	strcat(obuf,ch->pcdata->entered_text);

		if(strlen(text)<80)
		{
			for(i=0; i<=strlen(text); i++)
			{
				if(text[i]=='~')
					return send_to_char("Tilde found, line will not be added.\n\r", ch);
			}
	        strcat(obuf,text);
		}
		else
		{
			strcat(obuf,tbuf);
            for(i=0; i<=strlen(tbuf); i++)
		    {
		    	if(tbuf[i]=='~')
		    		return send_to_char("Tilde found, line will not be added.\n\r", ch);
		    }
		}
		strcat(obuf,"\n\r");
       	if(strlen(obuf) >= MSL*2)
       		return send_to_char("String exceeds buffer.\n\r",ch);
        free_pstring(ch->pcdata->entered_text);
        ch->pcdata->entered_text = palloc_string(obuf);
	}
	else if (text[0] == '\0')
    	return send_to_char("Add what?",ch);
    return send_to_char("Line added.",ch);
}
