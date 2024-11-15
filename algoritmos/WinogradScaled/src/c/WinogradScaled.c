#include <stdio.h>
#include <stdlib.h>
#include <math.h>

long long** winograd_scaled(long long **A, long long **B, long long m, long long n, long long p) {
    long long **C = calloc(m, sizeof(long long*));
    for (long long i = 0; i < m; i++) {
        C[i] = calloc(p, sizeof(long long));
    }

    long long *row_scale = calloc(m, sizeof(long long));
    long long *col_scale = calloc(p, sizeof(long long));

    // Calculate row and column scale factors
    for (long long i = 0; i < m; i++) {
        for (long long j = 0; j < n; j++) {
            if (abs(A[i][j]) > row_scale[i]) {
                row_scale[i] = abs(A[i][j]);
            }
        }
    }
    for (long long j = 0; j < p; j++) {
        for (long long i = 0; i < n; i++) {
            if (abs(B[i][j]) > col_scale[j]) {
                col_scale[j] = abs(B[i][j]);
            }
        }
    }

    long long *row_factor = calloc(m, sizeof(long long));
    long long *col_factor = calloc(p, sizeof(long long));

    // Calculate row and column factors
    for (long long i = 0; i < m; i++) {
        for (long long j = 0; j < n / 2; j++) {
            row_factor[i] += (A[i][2 * j] / row_scale[i]) * (A[i][2 * j + 1] / row_scale[i]);
        }
    }
    for (long long j = 0; j < p; j++) {
        for (long long i = 0; i < n / 2; i++) {
            col_factor[j] += (B[2 * i][j] / col_scale[j]) * (B[2 * i + 1][j] / col_scale[j]);
        }
    }

    // Compute the result matrix C
    for (long long i = 0; i < m; i++) {
        for (long long j = 0; j < p; j++) {
            C[i][j] = -row_factor[i] - col_factor[j];
            for (long long k = 0; k < n / 2; k++) {
                C[i][j] += ((A[i][2 * k] / row_scale[i] + B[2 * k + 1][j] / col_scale[j]) *
                            (A[i][2 * k + 1] / row_scale[i] + B[2 * k][j] / col_scale[j]));
            }
            if (n % 2 == 1) {
                C[i][j] += (A[i][n - 1] / row_scale[i]) * (B[n - 1][j] / col_scale[j]);
            }
            C[i][j] *= row_scale[i] * col_scale[j];
            C[i][j] = (long long)C[i][j];
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
    long long m = 3, n = 3, p = 3;

    // Crear matrices A y B para la prueba
    long long** A = (long long**)malloc(m * sizeof(long long*));
    for (long long i = 0; i < m; i++) {
        A[i] = (long long*)malloc(n * sizeof(long long));
    }

    long long** B = (long long**)malloc(n * sizeof(long long*));
    for (long long i = 0; i < n; i++) {
        B[i] = (long long*)malloc(p * sizeof(long long));
    }

    // Llenar las matrices A y B con valores de ejemplo
    A[0][0] = 1; A[0][1] = 2; A[0][2] = 3;
    A[1][0] = 4; A[1][1] = 5; A[1][2] = 6;
    A[2][0] = 7; A[2][1] = 8; A[2][2] = 9;

    B[0][0] = 9; B[0][1] = 8; B[0][2] = 7;
    B[1][0] = 6; B[1][1] = 5; B[1][2] = 4;
    B[2][0] = 3; B[2][1] = 2; B[2][2] = 1;

    // Llamar a la función winograd_scaled
    long long** C = winograd_scaled(A, B, m, n, p);

    // Imprimir la matriz resultante C
    printf("Resultado de la multiplicación de matrices Winograd escalada:\n");
    for (long long i = 0; i < m; i++) {
        for (long long j = 0; j < p; j++) {
            printf("%lld ", C[i][j]);
        }
        printf("\n");
    }

    // Liberar memoria de las matrices A, B y C
    for (long long i = 0; i < m; i++) {
        free(A[i]);
    }
    for (long long i = 0; i < n; i++) {
        free(B[i]);
    }
    free(A);
    free(B);

    // Liberar memoria de la matriz C después de que haya sido utilizada
    for (long long i = 0; i < m; i++) {
        free(C[i]);
    }
    free(C);
}

int main() {
    // Ejecutar la prueba
    test_winograd_scaled();
    return 0;
}
