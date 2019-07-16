#ifndef POSTGRES_H
#define POSTGRES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>

int get_rows(PGresult *res, char *date);
int get_columns(PGresult *res, char *date);
char ***get_data(char *date);

#endif