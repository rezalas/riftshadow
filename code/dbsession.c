#include "mud.h"
#include "stdlibs/dbsession.h"
#include <cstring>
#include <cstdlib>

//	-------------------------------------------------------------------------
//	Row
//	-------------------------------------------------------------------------

/// Retrieves the number of columns in the row.
/// @returns The number of columns in the row.
int Row::Columns() const
{
	return (int)values.size();
}

/// Checks if a specific column value is SQL NULL.
/// @note If the column index is out of range, returns true (SQL NULL).
/// @param col: Zero-based column index.
/// @returns true if the column is SQL NULL; false otherwise.
bool Row::IsNull(int col) const
{
	if (col < 0 || col >= (int)nulls.size())
		return true;
	return nulls[col] != 0;
}

/// Retrieves a specific column value from the row as a string.
/// @note If the column is SQL NULL or out of range, returns an empty string.
/// @param col: Zero-based column index.
/// @returns The column value as a string.
const std::string &Row::Str(int col) const
{
	static const std::string empty;
	if (col < 0 || col >= (int)values.size())
		return empty;
	return values[col];
}

/// Retrieves a specific column value from the row as a 32-bit integer.
/// @note If the column is SQL NULL or out of range, returns 0.
/// @param col: Zero-based column index.
/// @returns The column value parsed as a 32-bit integer.
int Row::I32(int col) const
{
	const std::string &s = Str(col);
	return s.empty() ? 0 : atoi(s.c_str());
}

/// Retrieves a specific column value from the row as a 64-bit integer.
/// @note If the column is SQL NULL or out of range, returns 0.
/// @param col: Zero-based column index.
/// @returns The column value parsed as a 64-bit int.
long long Row::I64(int col) const
{
	const std::string &s = Str(col);
	return s.empty() ? 0 : atoll(s.c_str());
}

/// Retrieves a specific column value from the row as a float.
/// @note If the column is SQL NULL or out of range, returns 0.0f.
/// @param col: Zero-based column index.
/// @returns The column value parsed as a float.
float Row::Flt(int col) const
{
	const std::string &s = Str(col);
	return s.empty() ? 0.0f : (float)atof(s.c_str());
}

//	-------------------------------------------------------------------------
//	Statement
//	-------------------------------------------------------------------------

/// Constructs a new statement.
/// @param conn A handle to the MySQL connection.
/// @param query A string containing the SQL query.
Statement::Statement(MYSQL *conn, const std::string &query)
	: connection(conn), sql(query)
{
	if (!conn)
		return;

	stmt = mysql_stmt_init(conn);
	if (!stmt)
	{
		RS.Logger.Error("DbSession: mysql_stmt_init failed for query: {}", query);
		return;
	}

	if (mysql_stmt_prepare(stmt, query.c_str(), query.length()))
	{
		RS.Logger.Error("DbSession: prepare failed: {} on query {}", mysql_stmt_error(stmt), query);
		mysql_stmt_close(stmt);
		stmt = nullptr;
	}
}

/// Destroys the statement if it is open.
Statement::~Statement()
{
	if (stmt)
		mysql_stmt_close(stmt);
}

/// Constructs a new statement by moving from another statement.
/// @param other The statement to move from.
Statement::Statement(Statement &&other) noexcept
	: connection(other.connection), stmt(other.stmt), sql(std::move(other.sql)),
	  params(std::move(other.params)), intStore(std::move(other.intStore)),
	  dblStore(std::move(other.dblStore)), strStore(std::move(other.strStore)),
	  lenStore(std::move(other.lenStore)), nullStore(std::move(other.nullStore))
{
	other.connection = nullptr;
	other.stmt = nullptr;
}

/// @brief Assigns a statement to this one by moving from another statement.
/// @param other The statement to move from.
/// @return A reference to this statement.
Statement &Statement::operator=(Statement &&other) noexcept
{
	if (this != &other)
	{
		if (stmt)
			mysql_stmt_close(stmt);

		connection = other.connection;
		stmt = other.stmt;
		sql = std::move(other.sql);
		params = std::move(other.params);
		intStore = std::move(other.intStore);
		dblStore = std::move(other.dblStore);
		strStore = std::move(other.strStore);
		lenStore = std::move(other.lenStore);
		nullStore = std::move(other.nullStore);

		other.connection = nullptr;
		other.stmt = nullptr;
	}
	return *this;
}

/// @brief Logs an error message related to the statement.
/// @param what The operation that failed.
void Statement::LogError(const char *what) const
{
	RS.Logger.Error("DbSession: {} failed: {} on query {}",
		what, stmt ? mysql_stmt_error(stmt) : "no statement", sql);
}

/// @brief Binds a SQL NULL at the next positional (?) parameter.
/// @return A pointer back to the statement for chaining.
IStatement &Statement::Bind(std::nullptr_t)
{
	MYSQL_BIND b;
	memset(&b, 0, sizeof(b));
	nullStore.push_back(1);
	b.buffer_type = MYSQL_TYPE_NULL;
	b.is_null = &nullStore.back();
	params.push_back(b);
	return *this;
}

/// Binds an integer value at the next positional (?) parameter.
/// @param value The integer value to bind.
/// @return A pointer back to the statement for chaining.
IStatement &Statement::Bind(int value)
{
	return Bind((long long)value);
}

/// Binds a long value at the next positional (?) parameter.
/// @param value The long value to bind.
/// @return A pointer back to the statement for chaining.
IStatement &Statement::Bind(long value)
{
	return Bind((long long)value);
}

/// Binds a 64-bit integer value at the next positional (?) parameter.
/// @param value The 64-bit integer value to bind.
/// @return A pointer back to the statement for chaining.
IStatement &Statement::Bind(long long value)
{
	MYSQL_BIND b;
	memset(&b, 0, sizeof(b));
	intStore.push_back(value);
	b.buffer_type = MYSQL_TYPE_LONGLONG;
	b.buffer = &intStore.back();
	params.push_back(b);
	return *this;
}

/// Binds a double value at the next positional (?) parameter.
/// @param value The double value to bind.
/// @return A pointer back to the statement for chaining.
IStatement &Statement::Bind(double value)
{
	MYSQL_BIND b;
	memset(&b, 0, sizeof(b));
	dblStore.push_back(value);
	b.buffer_type = MYSQL_TYPE_DOUBLE;
	b.buffer = &dblStore.back();
	params.push_back(b);
	return *this;
}

/// Binds a C string at the next positional (?) parameter.
/// @param value The C string to bind.
/// @return A pointer back to the statement for chaining.
IStatement &Statement::Bind(const char *value)
{
	return Bind(std::string(value ? value : ""));
}

/// Binds a string at the next positional (?) parameter.
/// @param value The string to bind.
/// @return A pointer back to the statement for chaining.
IStatement &Statement::Bind(const std::string &value)
{
	MYSQL_BIND b;
	memset(&b, 0, sizeof(b));
	strStore.push_back(value);
	lenStore.push_back((unsigned long)strStore.back().size());
	b.buffer_type = MYSQL_TYPE_STRING;
	b.buffer = (void *)strStore.back().data();
	b.buffer_length = lenStore.back();
	b.length = &lenStore.back();
	params.push_back(b);
	return *this;
}

/// Gathers all bound parameters and adds them to the statement for execution.
/// @return true if the parameters were successfully bound; otherwise false.
bool Statement::BindParams()
{
	if (params.empty())
		return true;

	if (mysql_stmt_bind_param(stmt, params.data()))
	{
		LogError("bind_param");
		return false;
	}
	return true;
}

/// Executes the prepared statement.
/// @return The result of the execution. Returns -1 on error.
long long Statement::Execute()
{
	if (!stmt)
		return -1;

	if (!BindParams())
		return -1;

	if (mysql_stmt_execute(stmt))
	{
		LogError("execute");
		return -1;
	}

	return (long long)mysql_stmt_affected_rows(stmt);
}

/// Fetches rows from the prepared statement.
/// @param sink The function to call for each fetched row.
/// @return True if the fetch was successful, false otherwise.
bool Statement::FetchInto(const std::function<void(const Row &)> &sink)
{
	if (!stmt)
		return false;

	if (!BindParams())
		return false;

	if (mysql_stmt_execute(stmt))
	{
		LogError("execute");
		return false;
	}

	MYSQL_RES *meta = mysql_stmt_result_metadata(stmt);
	if (!meta)		// statement produced no result set
		return true;

	unsigned int ncol = mysql_num_fields(meta);
	MYSQL_FIELD *fields = mysql_fetch_fields(meta);

	// Bind every column as a string buffer sized to the column's declared width.
	// The server converts numeric/date columns to text on fetch, which keeps the
	// core type-agnostic; the Row accessors convert back on read.
	std::vector<std::vector<char>>	buffers(ncol);
	std::vector<unsigned long>		lengths(ncol, 0);
	std::vector<my_bool>			nulls(ncol, 0);
	std::vector<my_bool>			errors(ncol, 0);
	std::vector<MYSQL_BIND>			rbind(ncol);
	memset(rbind.data(), 0, sizeof(MYSQL_BIND) * ncol);

	for (unsigned int i = 0; i < ncol; i++)
	{
		unsigned long cap = fields[i].length + 1;
		if (cap < 32)
			cap = 32;
		if (cap > 65536)		// clamp huge TEXT columns; truncation handled below
			cap = 65536;

		buffers[i].resize(cap);
		rbind[i].buffer_type = MYSQL_TYPE_STRING;
		rbind[i].buffer = buffers[i].data();
		rbind[i].buffer_length = cap;
		rbind[i].length = &lengths[i];
		rbind[i].is_null = &nulls[i];
		rbind[i].error = &errors[i];
	}

	if (mysql_stmt_bind_result(stmt, rbind.data()))
	{
		LogError("bind_result");
		mysql_free_result(meta);
		return false;
	}

	// store the full result set client-side for stable iteration
	mysql_stmt_store_result(stmt);

	int rc;
	bool ok = true;
	while ((rc = mysql_stmt_fetch(stmt)) == 0 || rc == MYSQL_DATA_TRUNCATED)
	{
		Row row;
		row.values.resize(ncol);
		row.nulls.resize(ncol);

		for (unsigned int i = 0; i < ncol; i++)
		{
			if (nulls[i])
			{
				row.nulls[i] = 1;
				continue;
			}

			row.nulls[i] = 0;
			unsigned long n = lengths[i];
			if (n >= buffers[i].size())		// truncated against clamp above
				n = buffers[i].size() - 1;
			row.values[i].assign(buffers[i].data(), n);
		}

		sink(row);
	}

	if (rc != MYSQL_NO_DATA)
	{
		LogError("fetch");
		ok = false;
	}

	mysql_free_result(meta);
	mysql_stmt_free_result(stmt);
	return ok;
}

//	-------------------------------------------------------------------------
//	DbSession
//	-------------------------------------------------------------------------

/// Destroys the database session and closes the connection if it is open.
DbSession::~DbSession()
{
	if (connection)
		mysql_close(connection);
}

/// Opens a connection to the database.
/// @note This method is only called once during the lifetime of the application.
/// @param conn The parameters used to connect.
/// @return true if the connection was successful; otherwise false.
bool DbSession::Connect(const DbConnection &conn)
{
	connection = mysql_init(nullptr);
	if (!connection)
	{
		RS.Logger.Error("DbSession: mysql_init failed.");
		return false;
	}

	if (!mysql_real_connect(connection, conn.Host.c_str(), conn.User.c_str(),
		conn.Pwd.c_str(), conn.Db.c_str(), conn.Port, nullptr, 0))
	{
		RS.Logger.Error("DbSession: unable to connect to database {}: {}",
			conn.Db, mysql_error(connection));
		mysql_close(connection);
		connection = nullptr;
		return false;
	}

	return true;
}

/// @brief Prepares a statement against the connection.
/// @param sql The SQL text, with positional (?) parameters.
/// @return An owning handle to the prepared statement.
std::unique_ptr<IStatement> DbSession::Prepare(const std::string &sql)
{
	return std::unique_ptr<IStatement>(new Statement(connection, sql));
}
