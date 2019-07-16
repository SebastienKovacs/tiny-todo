#ifndef POSTGRES_H
#define POSTGRES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>

void stop_connection_db(PGconn *conn, PGresult *res);
PGconn *connect_to_db();
int get_rows(PGresult *res, char *date);
int get_columns(PGresult *res, char *date);
char *make_sql_query(char *date);
char ***get_data(char *date);

#endif