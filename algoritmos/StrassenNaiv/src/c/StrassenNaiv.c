#include <stdio.h>
#include <stdlib.h>

void strassen_naive(int **A, int **B, int **C, int n) {
    if (n == 1) {
        C[0][0] = A[0][0] * B[0][0];
    } else {
        int mid = n / 2;

        int **A11 = (int **)malloc(mid * sizeof(int *));
        int **A12 = (int **)malloc(mid * sizeof(int *));
        int **A21 = (int **)malloc(mid * sizeof(int *));
        int **A22 = (int **)malloc(mid * sizeof(int *));

        int **B11 = (int **)malloc(mid * sizeof(int *));
        int **B12 = (int **)malloc(mid * sizeof(int *));
        int **B21 = (int **)malloc(mid * sizeof(int *));
        int **B22 = (int **)malloc(mid * sizeof(int *));

        int **C11 = (int **)malloc(mid * sizeof(int *));
        int **C12 = (int **)malloc(mid * sizeof(int *));
        int **C21 = (int **)malloc(mid * sizeof(int *));
        int **C22 = (int **)malloc(mid * sizeof(int *));

        // Inicializa los punteros a las submatrices
        for (int i = 0; i < mid; i++) {
            A11[i] = A[i];
            A12[i] = A[i] + mid;
            A21[i] = A[i + mid];
            A22[i] = A[i + mid] + mid;

            B11[i] = B[i];
            B12[i] = B[i] + mid;
            B21[i] = B[i + mid];
            B22[i] = B[i + mid] + mid;

            C11[i] = C[i];
            C12[i] = C[i] + mid;
            C21[i] = C[i + mid];
            C22[i] = C[i + mid] + mid;
        }

        // Llamada recursiva a Strassen
        strassen_naive(A11, B11, C11, mid);
        strassen_naive(A11, B12, C12, mid);
        strassen_naive(A12, B21, C21, mid);
        strassen_naive(A12, B22, C22, mid);
        strassen_naive(A21, B11, C11, mid);
        strassen_naive(A21, B12, C12, mid);
        strassen_naive(A22, B21, C21, mid);
        strassen_naive(A22, B22, C22, mid);

        // Combina los resultados de las submatrices
        for (int i = 0; i < mid; i++) {
            for (int j = 0; j < mid; j++) {
                C[i][j] += C11[i][j];
                C[i][j + mid] += C12[i][j];
                C[i + mid][j] += C21[i][j];
                C[i + mid][j + mid] += C22[i][j];
            }
        }

        // Libera la memoria
        for (int i = 0; i < mid; i++) {
            free(A11[i]);
            free(A12[i]);
            free(A21[i]);
            free(A22[i]);
            free(B11[i]);
            free(B12[i]);
            free(B21[i]);
            free(B22[i]);
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
        free(C11);
        free(C12);
        free(C21);
        free(C22);
    }
}

int main() {
    int n = 4;
    int **A = (int **)malloc(n * sizeof(int *));
    int **B = (int **)malloc(n * sizeof(int *));
    int **C = (int **)malloc(n * sizeof(int *));

    // Inicializa las matrices A y B
    for (int i = 0; i < n; i++) {
        A[i] = (int *)malloc(n * sizeof(int));
        B[i] = (int *)malloc(n * sizeof(int));
        C[i] = (int *)malloc(n * sizeof(int));

        for (int j = 0; j < n; j++) {
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
        }
    }

    strassen_naive(A, B, C, n);

    // Imprime el resultado
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", C[i][j]);
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