#include "special_allocs.h"


/**Basically allocs a triple char pointer according the the dimensions 
*  specified in the parameters. Used to alloc the char ***data which
*  holds a complete table from the database.
*/
char ***alloc_data(int dim1, int dim2)
{
	char ***data = malloc (dim1 * sizeof(char**));

	for (int i = 0; i < dim1; ++i){
		data[i] = malloc (dim2 * sizeof(char*));
		for (int j = 0; j < dim2; ++j){
			data[i][j] = malloc (50 * sizeof(char));
		}
	}

	return data;
}


/**Frees said memeory allocated above.
*/
void free_data(char ***data, int dim1, int dim2)
{
	for (int i = 0; i < dim1; ++i){
		for (int j = 0; j < dim2; ++j){
			free (data[i][j]);
		}
		free (data[i]);
	}
	free (data);
}
