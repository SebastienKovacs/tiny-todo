#include "postgres.h"
#include "select_day.h"
#include "special_allocs.h"


void stop_connection_db(PGconn *conn, PGresult *res)
{
	if (conn){
		PQfinish(conn);
	}
	if (res){
		PQclear(res);
	}
}


PGconn *connect_to_db()
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


int get_rows(PGresult *res, char *date){
	int rows = 0;

	if (res == NULL){
		PGconn *conn= connect_to_db();
		char *query = make_sql_query(date);
		res = PQexec(conn, query);

		rows = PQnfields(res);
		stop_connection_db(conn, res);
		return rows;
	}

	rows = PQnfields(res);
	stop_connection_db(NULL, res);

	return rows; 
}


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


char *make_sql_query(char *date)
{
	char *query = malloc (50 * sizeof(char));
	sprintf(query, "SELECT task, done FROM %s", date);

	return query;
}


char ***get_data(char *date)
{
	PGconn *conn = connect_to_db();
	char *query = make_sql_query(date);
	PGresult *res = PQexec(conn, query);

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