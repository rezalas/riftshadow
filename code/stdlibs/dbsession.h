#ifndef DBSESSION_H
#define DBSESSION_H

#include <mysql.h>

// Since `my_bool` was removed from the MySQL C API in 8.0,
// MariaDB Connector/C still provides it, we need to add this shim.
#if !defined(MARIADB_BASE_VERSION) && !defined(MARIADB_VERSION_ID) && \
	defined(MYSQL_VERSION_ID) && MYSQL_VERSION_ID >= 80000
typedef bool my_bool;
#endif

#include <string>
#include <vector>
#include <deque>
#include <memory>
#include <functional>
#include "../dbconnection.h"

/*
 * Core database classes that wrap a single persistent MySQL connection and supports prepared statements.
 * These classes are the foundation of the repository-pattern ORM, and are used by the repositories to 
 * access the database.
 *
 * Layers:
 *   IDbSession / IStatement - interfaces used by the different classes. Main use is for database mocking in unit tests.
 *   DbSession               - owns one MYSQL* connection; hands out prepared Statements.
 *   Statement               - a prepared MYSQL_STMT with fluent param binding and typed reads.
 *   Row                     - one fetched result row, exposed as typed column accessors.
 */

// TODO: Add transaction support. Currently all tables are MyISAM, so transactions are not supported. 
// When we migrate to InnoDB, we will add transaction support to the DbSession class.
 
// TODO: Transition to a more modern C++ MySQL library. Currently we are using the C API, which is not ideal. 
 

/// A single fetched result row. Values are materialised as strings (the server
/// converts on fetch) and exposed through typed accessors so mappers stay readable.
class Row
{
	friend class Statement;
public:
	Row() = default;

	/// Builds a row directly from column values, marking every column non-null.
	/// Primarily for tests that hand-craft canned result sets.
	/// @param cells: The column values, in positional order.
	explicit Row(std::vector<std::string> cells)
		: values(std::move(cells)), nulls(values.size(), 0) {}

	/// Builds a row from column values plus an explicit per-column null mask.
	/// @param cells: The column values, in positional order.
	/// @param nullMask: One flag per column; non-zero marks that column SQL NULL.
	Row(std::vector<std::string> cells, std::vector<char> nullMask)
		: values(std::move(cells)), nulls(std::move(nullMask)) {}

	int Columns() const;
	bool IsNull(int col) const;
	const std::string &	Str(int col) const;
	int I32(int col) const;
	long long I64(int col) const;
	float Flt(int col) const;

private:
	std::vector<std::string>	values;
	std::vector<char>			nulls;	// char, not vector<bool>, for a real element ref
};

/// Interface specification used for our concrete and mock prepared statement classes.
class IStatement
{
public:
	virtual ~IStatement() = default;

	/// Checks if the statement prepared successfully.
	/// @returns true if the statement is safe to use; otherwise false.
	virtual bool			IsValid() const = 0;

	/// Binds a SQL NULL at the next positional (?) parameter.
	/// @returns *this, for fluent chaining.
	virtual IStatement &	Bind(std::nullptr_t) = 0;

	/// Binds an integer at the next positional (?) parameter.
	/// @param value: The value to bind.
	/// @returns *this, for fluent chaining.
	virtual IStatement &	Bind(int value) = 0;

	/// Binds a long at the next positional (?) parameter.
	/// @param value: The value to bind.
	/// @returns *this, for fluent chaining.
	virtual IStatement &	Bind(long value) = 0;

	/// Binds a 64-bit integer at the next positional (?) parameter.
	/// @param value: The value to bind.
	/// @returns *this, for fluent chaining.
	virtual IStatement &	Bind(long long value) = 0;

	/// Binds a double at the next positional (?) parameter.
	/// @param value: The value to bind.
	/// @returns *this, for fluent chaining.
	virtual IStatement &	Bind(double value) = 0;

	/// Binds a C string at the next positional (?) parameter (a null pointer binds "").
	/// @param value: The value to bind.
	/// @returns *this, for fluent chaining.
	virtual IStatement &	Bind(const char *value) = 0;

	/// Binds a string at the next positional (?) parameter.
	/// @param value: The value to bind.
	/// @returns *this, for fluent chaining.
	virtual IStatement &	Bind(const std::string &value) = 0;

	/// Executes a write (INSERT/UPDATE/DELETE).
	/// @returns The number of affected rows, or -1 on error.
	virtual long long		Execute() = 0;

	/// Executes a read and maps each result row to T.
	/// @param map: Converts one Row into a T.
	/// @returns One T per result row, or an empty vector on error.
	template <typename T>
	std::vector<T> Query(const std::function<T(const Row &)> &map)
	{
		std::vector<T> results;
		if (!FetchInto([&](const Row &row) { results.push_back(map(row)); }))
			results.clear();
		return results;
	}

protected:
	/// Executes the statement and feeds each result row to the sink. The primitive
	/// Query<T>() is built on; overridden by concrete/fake statements.
	/// @param sink: Invoked once per fetched row.
	/// @returns true on success (including an empty/no result set); false on error.
	virtual bool FetchInto(const std::function<void(const Row &)> &sink) = 0;
};

/// A prepared statement over a live MYSQL_STMT. Non-copyable; move-only (it owns the
/// underlying statement handle).
class Statement : public IStatement
{
	friend class DbSession;

public:
	~Statement() override;
	Statement(Statement &&other) noexcept;
	Statement &operator=(Statement &&other) noexcept;
	Statement(const Statement &) = delete;
	Statement &operator=(const Statement &) = delete;

	/// Checks if the statement is valid.
	/// @return true if the statement is valid; otherwise false.
	bool IsValid() const override { return stmt != nullptr; }

	IStatement &	Bind(std::nullptr_t) override;
	IStatement &	Bind(int value) override;
	IStatement &	Bind(long value) override;
	IStatement &	Bind(long long value) override;
	IStatement &	Bind(double value) override;
	IStatement &	Bind(const char *value) override;
	IStatement &	Bind(const std::string &value) override;

	long long		Execute() override;

protected:
	bool			FetchInto(const std::function<void(const Row &)> &sink) override;

private:
	/// Prepares a statement against the connection. Constructed only by DbSession.
	/// @param conn: The owning connection handle.
	/// @param query: The SQL text, with positional (?) parameters.
	Statement(MYSQL *conn, const std::string &query);

	bool	BindParams();
	void	LogError(const char *what) const;

	MYSQL *			connection = nullptr;
	MYSQL_STMT *	stmt = nullptr;
	std::string		sql;

	// Bound-parameter descriptors plus stable backing storage. std::deque keeps
	// element addresses stable across push_back, so the MYSQL_BIND pointers below
	// stay valid as more params are bound.
	std::vector<MYSQL_BIND>			params;
	std::deque<long long>			intStore;
	std::deque<double>				dblStore;
	std::deque<std::string>			strStore;
	std::deque<unsigned long>		lenStore;
	std::deque<my_bool>				nullStore;
};

/// Interface specification used for our concrete and mock database session classes.
class IDbSession
{
public:
	virtual ~IDbSession() = default;

	/// Prepares a statement. Check the result's IsValid() before use.
	/// @param sql: The SQL text, with positional (?) parameters.
	/// @returns An owning handle to the prepared statement.
	virtual std::unique_ptr<IStatement>	Prepare(const std::string &sql) = 0;
};

/// Owns one persistent MySQL connection. Connect once at boot, then Prepare()
/// statements against it for the lifetime of the process.
class DbSession : public IDbSession
{
public:
	DbSession() = default;
	~DbSession() override;
	DbSession(const DbSession &) = delete;
	DbSession &operator=(const DbSession &) = delete;

	bool Connect(const DbConnection &conn);

	/// @returns true if the connection is open.
	bool IsValid() const { return connection != nullptr; }

	std::unique_ptr<IStatement>	Prepare(const std::string &sql) override;

	/// Escape hatch for the legacy shims during migration only. Do not use in
	/// new repository code.
	/// @returns The raw underlying connection handle.
	MYSQL * Handle() const	{ return connection; }

private:
	MYSQL *	connection = nullptr;
};

#endif /* DBSESSION_H */
