#include <mysql.h>
#include "../rift.h"
#include "../config.h"

class CSQLInterface;

class CRow
{
friend class CSQLInterface;
public:
	operator MYSQL_ROW () { return row; }
	char * 					operator[](int rowindex);
	char * 					GetVal(int rowindex);
	int						RowToNumber(int rowindex);
	float					RowToFloat(int rowindex);
//private:
	long 					maxcol;
	long 					maxrow;
	long 					rowpos;
	MYSQL_ROW 				row;
};

class CSQLInterface
{
public:
	CSQLInterface();
	~CSQLInterface();
	void 		StartSQLServer(const char* host, const char* db, const char* user, const char* pass);		//called at boot
	void 		FreeResults(void);			//self explanatory
	
	Config					Settings;
	int	 	 	Select(const char *query, ...);
	int 	 	Insert(const char *query, ...);
	int			Delete(const char *query, ...);
	int			Update(const char *query, ...);
	int			IQuery(const char *query);

	bool		End(void);				//end of results	
	CRow & 		GetRow(void);			//get first/next row

private:
	bool		SQLValid(void);		//am I ready to rock?
	
	RString		sResult;

	MYSQL *		connection;
	MYSQL *		connection2;
	MYSQL_RES * result_set;
	MYSQL_FIELD field;
	CRow		row;

	int			connstate;
	
};
