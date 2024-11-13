#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void multiplyMatricesWinogradScaled(long long **a, long long **b, long long **result, int rowA, int colA, int colB) {
    // Inicializar factores de filas y columnas
    long long *rowFactor = (long long *)calloc(rowA, sizeof(long long));
    long long *colFactor = (long long *)calloc(colB, sizeof(long long));

    // Calcular factores de fila
    for (int i = 0; i < rowA; i++) {
        for (int j = 0; j < colA / 2; j++) {
            rowFactor[i] += a[i][2 * j] * a[i][2 * j + 1];
        }
    }

    // Calcular factores de columna
    for (int i = 0; i < colB; i++) {
        for (int j = 0; j < rowA / 2; j++) {
            colFactor[i] += b[2 * j][i] * b[2 * j + 1][i];
        }
    }

    // Calcular la matriz de resultado usando los factores
    for (int i = 0; i < rowA; i++) {
        for (int j = 0; j < colB; j++) {
            result[i][j] = -rowFactor[i] - colFactor[j];
            for (int k = 0; k < colA / 2; k++) {
                result[i][j] += (a[i][2 * k] + b[2 * k + 1][j]) *
                                (a[i][2 * k + 1] + b[2 * k][j]);
            }
        }
    }

    // Ajuste para el caso en que colA es impar
    if (colA % 2 == 1) {
        for (int i = 0; i < rowA; i++) {
            for (int j = 0; j < colB; j++) {
                result[i][j] += a[i][colA - 1] * b[colA - 1][j];
            }
        }
    }

    // Liberar memoria
    free(rowFactor);
    free(colFactor);
}

// Función para inicializar matrices con valores de ejemplo
long long** createMatrix(int rows, int cols) {
    long long **matrix = (long long **)malloc(rows * sizeof(long long *));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (long long *)malloc(cols * sizeof(long long));
    }
    return matrix;
}

// Función para liberar la memoria de una matriz
void freeMatrix(long long **matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// Función para llenar una matriz con valores de ejemplo
void fillMatrix(long long **matrix, int rows, int cols, int startVal) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = startVal + i + j;  // Llena con valores de ejemplo
        }
    }
}

// Función para imprimir una matriz
void printMatrix(long long **matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%lld ", matrix[i][j]);
        }
        printf("\n");
    }
}

// Función de prueba del algoritmo Winograd
void testWinograd() {
    int rowA = 4, colA = 4, colB = 4; // Dimensiones de ejemplo

    // Crear e inicializar matrices A y B
    long long **A = createMatrix(rowA, colA);
    long long **B = createMatrix(colA, colB);
    fillMatrix(A, rowA, colA, 1);  // Llena A con valores de ejemplo
    fillMatrix(B, colA, colB, 2);  // Llena B con valores de ejemplo

    // Crear la matriz de resultado
    long long **result = createMatrix(rowA, colB);

    // Llamar a la función Winograd
    multiplyMatricesWinogradScaled(A, B, result, rowA, colA, colB);

    // Imprimir matrices y resultado
    printf("Matriz A:\n");
    printMatrix(A, rowA, colA);
    printf("\nMatriz B:\n");
    printMatrix(B, colA, colB);
    printf("\nResultado (AxB):\n");
    printMatrix(result, rowA, colB);

    // Liberar memoria
    freeMatrix(A, rowA);
    freeMatrix(B, colA);
    freeMatrix(result, rowA);
}

int main() {
    testWinograd();
    return 0;
}


