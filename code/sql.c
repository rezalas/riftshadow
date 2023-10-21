#include "mud.h"
#include <stdlib.h>
#include "mysql.h"
#include <string>

CSQLInterface::CSQLInterface(void)
{
	connstate = STATE_INVALID;
	result_set = nullptr;
	Settings = Config(CONFIG_FILE);
}

CSQLInterface::~CSQLInterface(void)
{
	if(!SQLValid())
		return;
	
	FreeResults();
	mysql_close(connection);
}

inline bool CSQLInterface::SQLValid(void)
{
	return connstate > STATE_INVALID;
}

bool CSQLInterface::StartSQLServer(const char* host, const char* db, const char* user, const char* pass)
{
	connection = mysql_init(nullptr);

	if(!mysql_real_connect(connection, host, user, pass, db, 0, nullptr, 0))
	{
		RS.Bug("Unable to connect to mysql database: %s\r\n", mysql_error(connection));
		return false;
	}
	
	connstate = STATE_VALID;
	return true;
}

void CSQLInterface::FreeResults(void)
{
	if(!result_set)
		return;
	
	mysql_free_result(result_set);
	result_set = nullptr;
}

int CSQLInterface::IQuery(const char *query)
{
	if(result_set)
		FreeResults();
	
	if(mysql_query(connection, query))
	{
		RS.Bug("MYSQL IQuery Error: %s on query %s", mysql_error(connection), query);
		return 0;
	}

	row.maxcol = 0;
	row.maxrow = 0;
	row.rowpos = 0;
	
	if((result_set = mysql_store_result(connection)))
	{ row.maxrow = mysql_num_rows(result_set); return row.maxrow; }
	else if(mysql_field_count(connection) == 0)
		return mysql_affected_rows(connection);
	else
		RS.Bug("MYSQL IQuery Store Error: %s on query %s", mysql_error(connection), query);

	return 0;
}

int CSQLInterface::Select(const char *pquery, ...)
{
	TString query, tquery;
	
	MUNCH_VARARG(pquery, query)
	
	tquery = "SELECT ";
	tquery += (const char *)query;
	return IQuery(tquery);
}

int CSQLInterface::Update(const char *pquery, ...)
{
	TString query, tquery;

	MUNCH_VARARG(pquery, query)

	tquery = "UPDATE ";
	tquery += (const char *)query;
	return IQuery(tquery);
}

int CSQLInterface::Insert(const char *pquery, ...)
{
	TString query, tquery;
	
	MUNCH_VARARG(pquery, query)

	tquery = "INSERT INTO ";
	tquery += (const char *)query;
	return IQuery(tquery);
}

int CSQLInterface::Delete(const char *pquery, ...)
{
	TString query, tquery;

	MUNCH_VARARG(pquery, query)

	tquery = "DELETE FROM ";
	tquery += (const char *)query;
	return IQuery(tquery);
}

bool CSQLInterface::End(void)
{
	if(row.rowpos >= row.maxrow)
		return true;
	return false;
}

CRow &CSQLInterface::GetRow(void)
{
	if(!result_set)
		throw("MYSQL Error: GetRow on empty result set.");
	
	row.row = mysql_fetch_row(result_set);

	if(!row.maxcol)
		row.maxcol = mysql_num_fields(result_set);
	
	row.rowpos++;
	
	return row;
}

char * CRow::operator[](int rowindex)
{
	//if(rowindex >= maxcol)
	//	throw("MYSQL Error: Reading beyond max column in row[].");

	return this->row[rowindex];
}

char * CRow::GetVal(int rowindex)
{
	return (*this)[rowindex];
}

int CRow::RowToNumber(int rowindex)
{
	return atoi(this->row[rowindex]);
}

float CRow::RowToFloat(int rowindex)
{
	return atof(this->row[rowindex]);
}
