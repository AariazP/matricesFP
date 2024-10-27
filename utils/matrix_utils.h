// matrix_utils.h
#ifndef MATRIX_UTILS_H
#define MATRIX_UTILS_H

#include <stdio.h>
#include <stdlib.h>

// Function to read two n x n matrices from a file
int read_matrices_from_file(const char *filename, int ***matrix1, int ***matrix2, int *n);

// Function to perform parallel block matrix multiplication
void parallel_block_multiply(int **A, int **B, int **C, int n, int block_size);

#endif // MATRIX_UTILS_H

