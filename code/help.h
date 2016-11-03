MYSQL * do_conn (char *host_name, char *user_name, char *password, char *db_name,
			unsigned int port_num, char *socket_name, unsigned int flags);
void do_disc (MYSQL *conn);
void print_error (MYSQL *conn, char *message);
void process_query (MYSQL *con, char *query);
