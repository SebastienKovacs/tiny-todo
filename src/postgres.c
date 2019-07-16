#include "postgres.h"
#include "select_day.h"
#include "special_allocs.h"


/**Closes connection or frees res or both. This function can be called
*  with one or both of the arguments being null. I wrote it this way so 
*  I can call it even when I don't have both conn and res in scope.
*/
static void stop_connection_db(PGconn *conn, PGresult *res)
{
	if (conn){
		PQfinish(conn);
	}
	if (res){
		PQclear(res);
	}
}


/**Connects to a database. The connection string conninfo will later
*  be stored in a config file and be accessed with libconfig.
*/
static PGconn *connect_to_db()
{
	const char *conninfo = "dbname=tiny-todo";
	PGconn *conn = PQconnectdb(conninfo);

	if (PQstatus(conn) == CONNECTION_OK){
		return conn;
	}

	printf("Connection to database failed: %s", PQerrorMessage(conn));
	stop_connection_db(conn, NULL);
	return NULL;
}


/**Just takes the desired date as a string wich you can get from
*  date_as_str() in select_day.c and uses it to make an psql query.
*/
static char *make_sql_query(char *date)
{
	char *query = malloc (50 * sizeof(char));
	sprintf(query, "SELECT task, done FROM %s", date);

	return query;
}


/**This function is very usefull for the times you want to do memory
*  allocation for a pointer to fit a part or the whole database in it.
*  The function returns the number of rows in a table in a db.
*  I use it in the for loops in special_allocs.c 
*/
int get_rows(PGresult *res, char *date){
	int rows = 0;

	if (res == NULL){
		PGconn *conn= connect_to_db();
		char *query = make_sql_query(date);
		res = PQexec(conn, query);

		rows = PQntuples(res);
		stop_connection_db(conn, res);
		return rows;
	}

	rows = PQntuples(res);
	stop_connection_db(NULL, res);
	return rows; 
}


/**Same as above but with columns instead of rows.
*/
int get_columns(PGresult *res, char *date){
	int columns = 0;

	if (res == NULL){
		PGconn *conn =  connect_to_db();
		char *query = make_sql_query(date);
		res = PQexec(conn, query);

		columns = PQnfields(res);
		stop_connection_db(conn, res);

		return columns;
	}

	columns = PQnfields(res);
	stop_connection_db(NULL, res);

	return columns; 
}


/**Creates a table in given db, used when there is no table for a 
*  given date.
*/
static void create_table(PGconn *conn, char *date)
{
	const char *ctStr1 = "CREATE TABLE ";
	const char *ctStr2 = "(task text NOT NULL, done int NOT NULL)";
	char query[80];
	sprintf(query, "%s%s%s", ctStr1, date, ctStr2);
	printf("create table\n");

	PQexec(conn, query);
}


/**The big function of this file. It takes the whole table out of the db
*  and puts it in this triple char pointer, aka a string table and return
*  it.
*/
char ***get_data(char *date)
{
	PGconn *conn = connect_to_db();
	char *query = make_sql_query(date);
	PGresult *res = PQexec(conn, query);
	
	if (PQresultStatus(res) != PGRES_COMMAND_OK){
		printf("%d\n", PQresultStatus(res));
		create_table(conn, date);
		res = PQexec(conn, query);
	}

	int rows = PQntuples(res);
	int columns = PQnfields(res);
	char ***data = alloc_data(rows, columns); 

	for (int i = 0; i < rows ; ++i){
		for (int j = 0; j < columns; ++j){
			strcpy(data[i][j], PQgetvalue(res, i, j));
		}
	}

	stop_connection_db(conn, res);

	return data;
}