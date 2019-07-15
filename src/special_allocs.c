#include "special_allocs.h"


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
