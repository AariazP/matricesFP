#include <stdio.h>
#include <stdlib.h>

void strassen_winograd(long long **A, long long **B, long long **C, int n) {
    if (n <= 2) {
        // Base case: 2x2 or 1x1 matrix
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                C[i][j] = 0;
                for (int k = 0; k < n; k++) {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }
        return;
    }

    int mid = n / 2;

    // Crear submatrices
    long long **A11 = (long long **)malloc(mid * sizeof(long long *));
    long long **A12 = (long long **)malloc(mid * sizeof(long long *));
    long long **A21 = (long long **)malloc(mid * sizeof(long long *));
    long long **A22 = (long long **)malloc(mid * sizeof(long long *));
    
    long long **B11 = (long long **)malloc(mid * sizeof(long long *));
    long long **B12 = (long long **)malloc(mid * sizeof(long long *));
    long long **B21 = (long long **)malloc(mid * sizeof(long long *));
    long long **B22 = (long long **)malloc(mid * sizeof(long long *));

    long long **M1 = (long long **)malloc(mid * sizeof(long long *));
    long long **M2 = (long long **)malloc(mid * sizeof(long long *));
    long long **M3 = (long long **)malloc(mid * sizeof(long long *));
    long long **M4 = (long long **)malloc(mid * sizeof(long long *));
    long long **M5 = (long long **)malloc(mid * sizeof(long long *));
    long long **M6 = (long long **)malloc(mid * sizeof(long long *));
    long long **M7 = (long long **)malloc(mid * sizeof(long long *));

    long long **C11 = (long long **)malloc(mid * sizeof(long long *));
    long long **C12 = (long long **)malloc(mid * sizeof(long long *));
    long long **C21 = (long long **)malloc(mid * sizeof(long long *));
    long long **C22 = (long long **)malloc(mid * sizeof(long long *));

    // Inicializa los punteros a las submatrices
    for (int i = 0; i < mid; i++) {
        A11[i] = (long long *)malloc(mid * sizeof(long long));
        A12[i] = (long long *)malloc(mid * sizeof(long long));
        A21[i] = (long long *)malloc(mid * sizeof(long long));
        A22[i] = (long long *)malloc(mid * sizeof(long long));

        B11[i] = (long long *)malloc(mid * sizeof(long long));
        B12[i] = (long long *)malloc(mid * sizeof(long long));
        B21[i] = (long long *)malloc(mid * sizeof(long long));
        B22[i] = (long long *)malloc(mid * sizeof(long long));

        M1[i] = (long long *)malloc(mid * sizeof(long long));
        M2[i] = (long long *)malloc(mid * sizeof(long long));
        M3[i] = (long long *)malloc(mid * sizeof(long long));
        M4[i] = (long long *)malloc(mid * sizeof(long long));
        M5[i] = (long long *)malloc(mid * sizeof(long long));
        M6[i] = (long long *)malloc(mid * sizeof(long long));
        M7[i] = (long long *)malloc(mid * sizeof(long long));

        C11[i] = (long long *)malloc(mid * sizeof(long long));
        C12[i] = (long long *)malloc(mid * sizeof(long long));
        C21[i] = (long long *)malloc(mid * sizeof(long long));
        C22[i] = (long long *)malloc(mid * sizeof(long long));
    }

    // Calcular las 7 matrices intermedias
    strassen_winograd(A11, B11, M1, mid);
    strassen_winograd(A21, B11, M2, mid);
    strassen_winograd(A11, B12, M3, mid);
    strassen_winograd(A22, B21, M4, mid);
    strassen_winograd(A11, B22, M5, mid);
    strassen_winograd(A21, B12, M6, mid);
    strassen_winograd(A12, B21, M7, mid);

    // Calcular la matriz C a partir de las matrices intermedias
    for (int i = 0; i < mid; i++) {
        for (int j = 0; j < mid; j++) {
            C11[i][j] = M1[i][j] + M4[i][j] - M5[i][j] + M7[i][j];
            C12[i][j] = M3[i][j] + M5[i][j];
            C21[i][j] = M2[i][j] + M4[i][j];
            C22[i][j] = M1[i][j] - M2[i][j] + M3[i][j] + M6[i][j];
        }
    }

    for (int i = 0; i < mid; i++) {
        for (int j = 0; j < mid; j++) {
            C[i][j] = C11[i][j];
            C[i][j + mid] = C12[i][j];
            C[i + mid][j] = C21[i][j];
            C[i + mid][j + mid] = C22[i][j];
        }
    }

    // Fusiona las submatrices en C
    for (int i = 0; i < mid; i++) {
        for (int j = 0; j < mid; j++) {
            C[i][j] = C11[i][j];
            C[i][j + mid] = C12[i][j];
            C[i + mid][j] = C21[i][j];
            C[i + mid][j + mid] = C22[i][j];
        }
    }

    for (int i = 0; i < mid; i++) {
        free(A11[i]);
        free(A12[i]);
        free(A21[i]);
        free(A22[i]);
        free(B11[i]);
        free(B12[i]);
        free(B21[i]);
        free(B22[i]);
        free(M1[i]);
        free(M2[i]);
        free(M3[i]);
        free(M4[i]);
        free(M5[i]);
        free(M6[i]);
        free(M7[i]);
        free(C11[i]);
        free(C12[i]);
        free(C21[i]);
        free(C22[i]);
    }
    free(A11);
    free(A12);
    free(A21);
    free(A22);
    free(B11);
    free(B12);
    free(B21);
    free(B22);
    free(M1);
    free(M2);
    free(M3);
    free(M4);
    free(M5);
    free(M6);
    free(M7);
    free(C11);
    free(C12);
    free(C21);
    free(C22);
}

void multiplicacion_estandar(long long **A, long long **B, long long **C, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = 0;
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    int n = 4;
    long long **A = (long long **)malloc(n * sizeof(long long *));
    long long **B = (long long **)malloc(n * sizeof(long long *));
    long long **C = (long long **)malloc(n * sizeof(long long *));

    // Inicializa las matrices A y B
    for (int i = 0; i < n; i++) {
        A[i] = (long long *)malloc(n * sizeof(long long));
        B[i] = (long long *)malloc(n * sizeof(long long));
        C[i] = (long long *)malloc(n * sizeof(long long));

        for (int j = 0; j < n; j++) {
            A[i][j] = rand() % 1000;  // Genera números aleatorios más grandes
            B[i][j] = rand() % 1000;  // Genera números aleatorios más grandes
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%lld ", A[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%lld ", B[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    strassen_winograd(A, B, C, n);

    printf("Resultado de la multiplicación Strassen:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%lld ", C[i][j]);
        }
        printf("\n");
    }

    multiplicacion_estandar(A, B, C, n);

    printf("Resultado de la multiplicación estandar:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%lld ", C[i][j]);
        }
        printf("\n");
    }

    // Libera la memoria
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