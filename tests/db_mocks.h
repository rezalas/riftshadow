#ifndef DB_MOCKS_H
#define DB_MOCKS_H

#include <string>
#include <vector>
#include <memory>
#include "../code/stdlibs/dbsession.h"

/*
 * Group of classes that mock a database session and prepared statement for unit-testing repositories.
 *
 * A FakeDbSession records the SQL that was prepared and the parameters
 * that were bound, and hands back caller-supplied canned result rows. 
 * With this we can assert three things per repository method:
 *   1. the exact SQL text (columns, WHERE, ORDER BY),
 *   2. the bound parameters (proving injection-safety - wildcards live in the
 *      *value*, not the SQL), and
 *   3. that result rows map onto the entity correctly.
 * Thus we can verify the repository's SQL and mapping logic is correct without needing a real database.
 */

class FakeDbSession;

/// A prepared statement that records what it is asked to do and replays canned
/// rows, writing all observations back into its owning FakeDbSession.
class FakeStatement : public IStatement
{
public:
	explicit FakeStatement(FakeDbSession &owner) : owner(owner) {}

	bool			IsValid() const override	{ return true; }

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
	FakeDbSession &	owner;
};

/// A fake DbSession used to mock a real database session.
/// @note The Rows, ExecuteResult, and FetchSucceeds members must be set before calling the repository method under test.
///	@note The LastSql, Binds, PrepareCount, ExecuteCount, and FetchCount members are set by the repository method under test.
class FakeDbSession : public IDbSession
{
public:
	// members that need configuring before calling the repository method under test
	std::vector<Row>	Rows;					// rows FetchInto will replay
	long long			ExecuteResult = 1;		// what Execute() returns
	bool				FetchSucceeds = true;	// false simulates a fetch error

	// members that store the results after calling the repository method under test
	std::string					LastSql;		// SQL of the most recent Prepare
	std::vector<std::string>	Binds;			// bound params of the most recent statement
	int							PrepareCount = 0;
	int							ExecuteCount = 0;
	int							FetchCount = 0;

	/// Creates a mock prepared statement that records what it is asked to do and replays canned rows.
	/// @param sql The sql statement that would execute if it were real.
	/// @return The mock statement is ready to execute.
	std::unique_ptr<IStatement> Prepare(const std::string &sql) override
	{
		PrepareCount++;
		LastSql = sql;
		Binds.clear();
		return std::unique_ptr<IStatement>(new FakeStatement(*this));
	}
};

//	-------------------------------------------------------------
//	FakeStatement Implementation
//	-------------------------------------------------------------


/// Adds a SQL NULL parameter to the mocked prepared statement.
/// @return A pointer back to the statement for chaining.
inline IStatement &FakeStatement::Bind(std::nullptr_t)
{
	owner.Binds.push_back("NULL");
	return *this;
}

/// Adds an integer parameter to the mocked prepared statement.
/// @param value The integer value to bind.
/// @return A pointer back to the statement for chaining.
inline IStatement &FakeStatement::Bind(int value)
{
	owner.Binds.push_back(std::to_string(value));
	return *this;
}

/// Adds a long parameter to the mocked prepared statement.
/// @param value The long value to bind.
/// @return A pointer back to the statement for chaining.
inline IStatement &FakeStatement::Bind(long value)
{
	owner.Binds.push_back(std::to_string(value));
	return *this;
}

/// Adds a long long parameter to the mocked prepared statement.
/// @param value The long long value to bind.
/// @return A pointer back to the statement for chaining.
inline IStatement &FakeStatement::Bind(long long value)
{
	owner.Binds.push_back(std::to_string(value));
	return *this;
}

/// Adds a double parameter to the mocked prepared statement.
/// @param value The double value to bind.
/// @return A pointer back to the statement for chaining.
inline IStatement &FakeStatement::Bind(double value)
{
	owner.Binds.push_back(std::to_string(value));
	return *this;
}

/// Adds a string parameter to the mocked prepared statement.
/// @param value The string value to bind.
/// @return A pointer back to the statement for chaining.
inline IStatement &FakeStatement::Bind(const char *value)
{
	owner.Binds.push_back(value ? value : "");
	return *this;
}

/// Adds a string parameter to the mocked prepared statement.
/// @param value The string value to bind.
/// @return A pointer back to the statement for chaining.
inline IStatement &FakeStatement::Bind(const std::string &value)
{
	owner.Binds.push_back(value);
	return *this;
}

/// Executes the mocked prepared statement.
/// @return The result of the execution.
inline long long FakeStatement::Execute()
{
	owner.ExecuteCount++;
	return owner.ExecuteResult;
}

/// Fetches rows from the mocked prepared statement.
/// @param sink The function to call for each fetched row.
/// @return True if the fetch was successful, false otherwise.
inline bool FakeStatement::FetchInto(const std::function<void(const Row &)> &sink)
{
	owner.FetchCount++;

	if (!owner.FetchSucceeds)
		return false;

	for (const Row &row : owner.Rows)
	{
		sink(row);
	}

	return true;
}

#endif /* DB_MOCKS_H */
