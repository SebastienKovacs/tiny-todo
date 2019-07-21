#include "select_day.h"


/**Just returns the current date in form of 
*/
static struct tm get_date()
{
	time_t t = time(NULL);
	return *localtime(&t);
}


/**Makes a string out of the current_date struct. This is needed because 
*  this string is actually the name of the psql table corresponding to the
*  said date.
*/
char *date_as_str()
{
	struct tm current_time = get_date();
	char *date = malloc (15 * sizeof(char));

	sprintf(date, "_%d_%d_%d", current_time.tm_year + 1900,
				   current_time.tm_mon + 1,
				   current_time.tm_mday);

	return date;
}


char *pretty_date_as_str(char *date)
{
	char *tmp;
	char *prettyDate = malloc (strlen(date));
	strcpy(prettyDate, date);

	while ((tmp = strchr(prettyDate, 95)) != NULL){
		*tmp = 32;
	}

	return prettyDate;
}
