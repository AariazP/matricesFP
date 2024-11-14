#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Función para verificar asignación de memoria
void* checkAlloc(void* ptr) {
    if (ptr == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

// Función mejorada para crear matrices
long long** createMatrix(int rows, int cols) {
    if (rows <= 0 || cols <= 0) {
        fprintf(stderr, "Error: Dimensiones de matriz inválidas\n");
        exit(EXIT_FAILURE);
    }

    long long **matrix = (long long **)checkAlloc(malloc(rows * sizeof(long long *)));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (long long *)checkAlloc(calloc(cols, sizeof(long long)));
    }
    return matrix;
}

void winograd_scaled(long long **A, long long **B, long long **C, int m, int n, int p) {
    if (!A || !B || !C || m <= 0 || n <= 0 || p <= 0) {
        fprintf(stderr, "Error: Parámetros inválidos en winograd_scaled\n");
        return;
    }

    // Escalado de filas y columnas
    long long *row_scale = (long long *)checkAlloc(calloc(m, sizeof(long long)));
    long long *col_scale = (long long *)checkAlloc(calloc(p, sizeof(long long)));

    for (int i = 0; i < m; i++) {
        row_scale[i] = 1;
        for (int j = 0; j < n; j++) {
            if (llabs(A[i][j]) > row_scale[i]) {
                row_scale[i] = llabs(A[i][j]);
            }
        }
    }

    for (int j = 0; j < p; j++) {
        col_scale[j] = 1;
        for (int i = 0; i < n; i++) {
            if (llabs(B[i][j]) > col_scale[j]) {
                col_scale[j] = llabs(B[i][j]);
            }
        }
    }

    // Calcular factores de fila
    long long *row_factor = (long long *)checkAlloc(calloc(m, sizeof(long long)));
    for (int i = 0; i < m; i++) {
        if (row_scale[i] != 0) {
            for (int j = 0; j < n / 2; j++) {
                long long a1 = A[i][2 * j] / row_scale[i];
                long long a2 = A[i][2 * j + 1] / row_scale[i];
                row_factor[i] += a1 * a2;
            }
        }
    }

    // Calcular factores de columna
    long long *col_factor = (long long *)checkAlloc(calloc(p, sizeof(long long)));
    for (int j = 0; j < p; j++) {
        if (col_scale[j] != 0) {
            for (int i = 0; i < n / 2; i++) {
                long long b1 = B[2 * i][j] / col_scale[j];
                long long b2 = B[2 * i + 1][j] / col_scale[j];
                col_factor[j] += b1 * b2;
            }
        }
    }

    // Multiplicación principal
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            if (row_scale[i] == 0 || col_scale[j] == 0) {
                C[i][j] = 0;
                continue;
            }

            long long sum = -row_factor[i] - col_factor[j];
            
            for (int k = 0; k < n / 2; k++) {
                long long a1 = A[i][2 * k] / row_scale[i];
                long long a2 = A[i][2 * k + 1] / row_scale[i];
                long long b1 = B[2 * k][j] / col_scale[j];
                long long b2 = B[2 * k + 1][j] / col_scale[j];
                
                sum += (a1 + b2) * (a2 + b1);
            }

            // Caso impar
            if (n % 2 == 1) {
                sum += (A[i][n - 1] * B[n - 1][j]) / (row_scale[i] * col_scale[j]);
            }

            // Reescalamiento final
            C[i][j] = sum * row_scale[i] * col_scale[j];
        }
    }

    free(row_scale);
    free(col_scale);
    free(row_factor);
    free(col_factor);
}

void freeMatrix(long long **matrix, int rows) {
    if (!matrix) return;
    
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void fillMatrix(long long **matrix, int rows, int cols, int startVal) {
    if (!matrix) return;
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = startVal + i + j;
        }
    }
}

void printMatrix(long long **matrix, int rows, int cols) {
    if (!matrix) {
        printf("Matriz nula\n");
        return;
    }
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%lld\t", matrix[i][j]);
        }
        printf("\n");
    }
}

void testWinograd() {
    int rowA = 4, colA = 4, colB = 4;

    long long **A = createMatrix(rowA, colA);
    long long **B = createMatrix(colA, colB);
    long long **result = createMatrix(rowA, colB);

    fillMatrix(A, rowA, colA, 1);
    fillMatrix(B, colA, colB, 2);

    printf("Matriz A:\n");
    printMatrix(A, rowA, colA);
    printf("\nMatriz B:\n");
    printMatrix(B, colA, colB);

    winograd_scaled(A, B, result, rowA, colA, colB);

    printf("\nResultado (AxB):\n");
    printMatrix(result, rowA, colB);

    freeMatrix(A, rowA);
    freeMatrix(B, colA);
    freeMatrix(result, rowA);
}

int main() {
    testWinograd();
    return 0;
}
