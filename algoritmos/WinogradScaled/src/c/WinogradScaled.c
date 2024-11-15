#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void imprimirMatriz(long double **C, long long tamanio) {
    for (int i = 0; i < tamanio; i++) {
        for (int j = 0; j < tamanio; j++) {
            printf("%Lf ", C[i][j]);  // %Lf para imprimir long double
        }
        printf("\n");
    }
}

void imprimirArray(long double *C, long long tamano) {
    for (int i = 0; i < tamano; i++) {
        printf("%Lf ", C[i]);  // %Lf para imprimir long double
    }
    printf("\n");
}

long double** winograd_scaled(long double **A, long double **B, long long n) {
    long double **C = calloc(n, sizeof(long double*));
    for (long long i = 0; i < n; i++) {
        C[i] = calloc(n, sizeof(long double));
    }

    long double *row_scale = calloc(n, sizeof(long double));
    long double *col_scale = calloc(n, sizeof(long double));

    // Calculate row and column scale factors
    for (long i = 0; i < n; i++) {
        for (long j = 0; j < n; j++) {
            if (fabs(A[i][j]) > row_scale[i]) {
                row_scale[i] = fabs(A[i][j]);
            }
        }
    }

    for (long j = 0; j < n; j++) {
        for (long i = 0; i < n; i++) {
            if (fabs(B[i][j]) > col_scale[j]) {
                col_scale[j] = fabs(B[i][j]);
            }
        }
    }

    long double *row_factor = calloc(n, sizeof(long double));
    long double *col_factor = calloc(n, sizeof(long double));

    // Calculate row and column factors
    for (long i = 0; i < n; i++) {
        for (long j = 0; j < n / 2; j++) {
            row_factor[i] += (A[i][2 * j] / row_scale[i]) * (A[i][2 * j + 1] / row_scale[i]);
        }
    }
    for (long j = 0; j < n; j++) {
        for (long i = 0; i < n / 2; i++) {
            col_factor[j] += (B[2 * i][j] / col_scale[j]) * (B[2 * i + 1][j] / col_scale[j]);
        }
    }

    // Compute the result matrix C
    for (long i = 0; i < n; i++) {
        for (long j = 0; j < n; j++) {
            C[i][j] = -row_factor[i] - col_factor[j];
            for (long k = 0; k < n / 2; k++) {
                C[i][j] += ((A[i][2 * k] / row_scale[i] + B[2 * k + 1][j] / col_scale[j]) *
                            (A[i][2 * k + 1] / row_scale[i] + B[2 * k][j] / col_scale[j]));
            }
            if (n % 2 == 1) {
                C[i][j] += (A[i][n - 1] / row_scale[i]) * (B[n - 1][j] / col_scale[j]);
            }
            C[i][j] *= row_scale[i] * col_scale[j];
            C[i][j] = (long double)C[i][j];  // Casting to long double
        }
    }

    // No liberar la memoria de C aquí, se hace fuera de la función
    free(row_scale);
    free(col_scale);
    free(row_factor);
    free(col_factor);

    return C;
}

void test_winograd_scaled() {
    // Definir las dimensiones de las matrices
    long long n = 4;

    // Crear matrices A y B para la prueba
    long double** A = (long double**)malloc(n * sizeof(long double*));
    for (long long i = 0; i < n; i++) {
        A[i] = (long double*)malloc(n * sizeof(long double));
    }

    long double** B = (long double**)malloc(n * sizeof(long double*));
    for (long long i = 0; i < n; i++) {
        B[i] = (long double*)malloc(n * sizeof(long double));
    }

    // Llenar las matrices A y B con valores de ejemplo
    A[0][0] = 1; A[0][1] = 2; A[0][2] = 3; A[0][3] = 4;
    A[1][0] = 5; A[1][1] = 6; A[1][2] = 7; A[1][3] = 8;
    A[2][0] = 9; A[2][1] = 10; A[2][2] = 11; A[2][3] = 12;
    A[3][0] = 13; A[3][1] = 14; A[3][2] = 15; A[3][3] = 16;

    // Asignación de valores a la matriz B
    B[0][0] = 17; B[0][1] = 18; B[0][2] = 19; B[0][3] = 20;
    B[1][0] = 21; B[1][1] = 22; B[1][2] = 23; B[1][3] = 24;
    B[2][0] = 25; B[2][1] = 26; B[2][2] = 27; B[2][3] = 28;
    B[3][0] = 29; B[3][1] = 30; B[3][2] = 31; B[3][3] = 32;

    // Llamar a la función winograd_scaled
    long double** C = winograd_scaled(A, B, n);

    // Imprimir la matriz resultante C
    printf("Resultado de la multiplicación de matrices Winograd escalada:\n");
    for (long long i = 0; i < n; i++) {
        for (long long j = 0; j < n; j++) {
            printf("%Lg ", C[i][j]);  // %Lf para imprimir long double
        }
        printf("\n");
    }

    // Liberar memoria de las matrices A, B y C
    for (long long i = 0; i < n; i++) {
        free(A[i]);
    }
    for (long long i = 0; i < n; i++) {
        free(B[i]);
    }
    free(A);
    free(B);

    // Liberar memoria de la matriz C después de que haya sido utilizada
    for (long long i = 0; i < n; i++) {
        free(C[i]);
    }
    free(C);
}

int main() {
    // Ejecutar la prueba
    test_winograd_scaled();
    return 0;
}