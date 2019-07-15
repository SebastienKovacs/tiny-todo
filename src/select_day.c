#include "select_day.h"


static struct tm get_date()
{
	time_t t = time(NULL);
	return *localtime(&t);
}


char *date_as_str()
{
	struct tm tm = get_date();
	char *date = malloc (15 * sizeof(char));

	sprintf(date, "_%d_%d_%d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

	return date;
}