#ifndef SPECIAL_ALLOCS_H
#define SPECIAL_ALLOCS_H

#include <stdlib.h>

char ***alloc_data(int dim1, int dim2);
void free_data(char ***data, int dim1, int dim2);

#endif