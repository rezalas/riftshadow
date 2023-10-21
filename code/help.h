#ifndef HELP_H
#define HELP_H

#include <mysql.h>
#include "merc.h"

//
// TODO: UNKNOWN FUNCTIONS
//
//void process_query (MYSQL *con, char *query);

//
// LOCAL FUNCTIONS
//

int can_see_help (CHAR_DATA *ch, MYSQL_ROW row, bool fOnlyResult);
void show_helpfile (CHAR_DATA *ch, MYSQL_ROW row);
void modhelp_end_fun (CHAR_DATA *ch, char *argument);
void do_modhelp (CHAR_DATA *ch, char *argument);
void do_help (CHAR_DATA *ch, char *argument);
void do_delhelp (CHAR_DATA *ch, char *argument);
void addhelp_end_fun (CHAR_DATA *ch, char *argument);
void do_addhelp (CHAR_DATA *ch, char *argument);
void do_listhelp (CHAR_DATA *ch, char *argument);
MYSQL *do_conn (const char *host_name, const char *user_name, const char *password, const char *db_name, unsigned int port_num, const char *socket_name, unsigned int flags);
void do_disc (MYSQL *conn);
void print_error (MYSQL *conn, char *message);

#endif /* HELP_H */
