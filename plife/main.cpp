
#pragma region all the setup-y stuff

#include <windows.h>
#include <stdio.h>
#include "mysql.h" 

#include <string>

#pragma comment( lib, "libmysql.lib" ) 
#pragma endregion
#pragma region CODING WITH MYSQL


MYSQL mysql;    
                
MYSQL * conn ;  

char *host="localhost";
char *user="root";
char *password="1234";
char *database="mql5";

using namespace std;


char *read() {
	
  mysql_init( &mysql ) ;
  conn = mysql_real_connect(  &mysql,
                              host,// synonymous with 127.0.0.1
                              user,     // connect as user="root".  Uh, bad security here..
                              password,         // my root password is blank.  REALLY bad security :)
                              database,    // connect to the 'mysql' _database_ within MySQL itself.
                                          // if you create another database, then you can
                                          // specify you'd like to connect to that one here.

                              0,          // port.  Mine is on 3306, but you can leave it as 0
                                          // and it seems to work automatically.

                              0,          // unix_socket (not in use)

                              0 ) ;       // client flag:  usually 0, unless you want special features (see docs page)
						#pragma region running an actual query

  char * query = "select data from packet" ;
  int queryState;

  queryState = mysql_query( conn, query ) ;

  MYSQL_RES * resultset ;
  MYSQL_ROW row;  // MYSQL_ROW is #defined as (char **)
 
  resultset = mysql_store_result( conn );
  
  int numFields = mysql_num_fields( resultset ) ;
  
  
  while( row = mysql_fetch_row( resultset ) )
  {
  
    for ( int i = 0; i < numFields ; i++ )
    {
     // printf( "%25.25s", row[ i ] ) ;
		return row[ i ];
		
    }

  }
  
  #pragma endregion
  mysql_free_result( resultset );
  mysql_close( conn ) ;
  
	 
	
  return 0;
}

int insert(const char *sql) {

	  mysql_init( &mysql ) ;
  conn = mysql_real_connect(  &mysql,
                              host,// synonymous with 127.0.0.1
                              user,     // connect as user="root".  Uh, bad security here..
                              password,         // my root password is blank.  REALLY bad security :)
                              database,    // connect to the 'mysql' _database_ within MySQL itself.
                                          // if you create another database, then you can
                                          // specify you'd like to connect to that one here.

                              0,          // port.  Mine is on 3306, but you can leave it as 0
                                          // and it seems to work automatically.

                              0,          // unix_socket (not in use)

                              0 ) ;       // client flag:  usually 0, unless you want special features (see docs page)
  


  #pragma region running an actual query

  const char * query = sql;
  int queryState;

  queryState = mysql_query( conn, query ) ;

  MYSQL_RES * resultset ;
  MYSQL_ROW row;  // MYSQL_ROW is #defined as (char **)
 
  resultset = mysql_store_result( conn );
 
  #pragma endregion
  mysql_free_result( resultset );
  mysql_close( conn ) ; 
  return 0;
}

static wchar_t* charToWChar(const char* text)
{
    size_t size = strlen(text) + 1;
    wchar_t* wa = new wchar_t[size];
    mbstowcs(wa,text,size);
    return wa;
}

extern "C" __declspec(dllexport) wchar_t* __stdcall pMysql( wchar_t *process, wchar_t *sql)
{
	mysql_close( conn );
	wstring ws(process);
	string str(ws.begin(), ws.end());
	if(str=="read") {
	
		return  charToWChar(read());
		mysql_close( conn );
	}
	if(str=="insert") {
	
		wstring wss(sql);
		string _sql(wss.begin(), wss.end());
		const char *cstr =_sql.c_str();
		insert(cstr);
		mysql_close( conn );
	}

}


#pragma endregion // CODING WITH MYSQL