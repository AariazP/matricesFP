#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "../../../../utils/matrix_utils.c"  // Include your matrix utility functions

#define BLOCK_SIZE 2  // Block size

// Function to perform sequential block matrix multiplication
void sequential_block_multiply(int64_t **A, int64_t **B, int64_t **C, int n, int block_size) {
    for (int i = 0; i < n; i += block_size) {
        for (int j = 0; j < n; j += block_size) {
            for (int k = 0; k < n; k += block_size) {
                for (int ii = i; ii < i + block_size && ii < n; ii++) {
                    for (int jj = j; jj < j + block_size && jj < n; jj++) {
                        int64_t sum = 0; // Local sum for this block
                        for (int kk = k; kk < k + block_size && kk < n; kk++) {
                            sum += A[ii][kk] * B[kk][jj];
                        }
                        C[ii][jj] += sum; // Accumulate result in C
                    }
                }
            }
        }
    }
}

// Function to print the matrix
void print_matrix(int64_t **M, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%" PRId64 " ", M[i][j]);
        }
        printf("\n");
    }
}

// Main function
int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <matrix_file>\n", argv[0]);
        return 1;
    }

    int64_t **A, **B, **C;
    int n;
    const char *filename = argv[1];  // Path to the matrix file

    if (read_matrices_from_file(filename, &A, &B, &n) != 0) {
        fprintf(stderr, "Failed to read matrices from file.\n");
        return -1;
    }

    // Allocate memory for the result matrix C and initialize to zero
    C = (int64_t **)malloc(n * sizeof(int64_t *));
    for (int i = 0; i < n; i++) {
        C[i] = (int64_t *)malloc(n * sizeof(int64_t));
        memset(C[i], 0, n * sizeof(int64_t)); // Initialize to zero
    }

    // Perform sequential block matrix multiplication
    sequential_block_multiply(A, B, C, n, BLOCK_SIZE);

    // Print the result matrix
    printf("Result matrix C:\n");
    print_matrix(C, n);

    // Free allocated memory
    for (int i = 0; i < n; i++) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);

    return 0;
}

