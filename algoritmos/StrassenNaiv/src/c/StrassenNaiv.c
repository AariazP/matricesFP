#include <stdio.h>
#include <stdlib.h>

void add(int **A, int **B, int **C, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] + B[i][j];
}

void subtract(int **A, int **B, int **C, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] - B[i][j];
}

void strassen_naive(int **A, int **B, int **C, int n) {
    if (n == 1) {
        C[0][0] = A[0][0] * B[0][0];
        return;
    }

    int mid = n / 2;

    // Allocate memory for submatrices
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
    int **M1 = (int **)malloc(mid * sizeof(int *));
    int **M2 = (int **)malloc(mid * sizeof(int *));
    int **M3 = (int **)malloc(mid * sizeof(int *));
    int **M4 = (int **)malloc(mid * sizeof(int *));
    int **M5 = (int **)malloc(mid * sizeof(int *));
    int **M6 = (int **)malloc(mid * sizeof(int *));
    int **M7 = (int **)malloc(mid * sizeof(int *));
    int **temp1 = (int **)malloc(mid * sizeof(int *));
    int **temp2 = (int **)malloc(mid * sizeof(int *));

    for (int i = 0; i < mid; i++) {
        A11[i] = (int *)malloc(mid * sizeof(int));
        A12[i] = (int *)malloc(mid * sizeof(int));
        A21[i] = (int *)malloc(mid * sizeof(int));
        A22[i] = (int *)malloc(mid * sizeof(int));
        B11[i] = (int *)malloc(mid * sizeof(int));
        B12[i] = (int *)malloc(mid * sizeof(int));
        B21[i] = (int *)malloc(mid * sizeof(int));
        B22[i] = (int *)malloc(mid * sizeof(int));
        C11[i] = (int *)malloc(mid * sizeof(int));
        C12[i] = (int *)malloc(mid * sizeof(int));
        C21[i] = (int *)malloc(mid * sizeof(int));
        C22[i] = (int *)malloc(mid * sizeof(int));
        M1[i] = (int *)malloc(mid * sizeof(int));
        M2[i] = (int *)malloc(mid * sizeof(int));
        M3[i] = (int *)malloc(mid * sizeof(int));
        M4[i] = (int *)malloc(mid * sizeof(int));
        M5[i] = (int *)malloc(mid * sizeof(int));
        M6[i] = (int *)malloc(mid * sizeof(int));
        M7[i] = (int *)malloc(mid * sizeof(int));
        temp1[i] = (int *)malloc(mid * sizeof(int));
        temp2[i] = (int *)malloc(mid * sizeof(int));
    }

    // Divide matrices into submatrices
    for (int i = 0; i < mid; i++) {
        for (int j = 0; j < mid; j++) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + mid];
            A21[i][j] = A[i + mid][j];
            A22[i][j] = A[i + mid][j + mid];
            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + mid];
            B21[i][j] = B[i + mid][j];
            B22[i][j] = B[i + mid][j + mid];
        }
    }

    // Calculate the 7 Strassen products (M1 - M7)
    add(A11, A22, temp1, mid);
    add(B11, B22, temp2, mid);
    strassen_naive(temp1, temp2, M1, mid); // M1 = (A11 + A22) * (B11 + B22)

    add(A21, A22, temp1, mid);
    strassen_naive(temp1, B11, M2, mid); // M2 = (A21 + A22) * B11

    subtract(B12, B22, temp2, mid);
    strassen_naive(A11, temp2, M3, mid); // M3 = A11 * (B12 - B22)

    subtract(B21, B11, temp2, mid);
    strassen_naive(A22, temp2, M4, mid); // M4 = A22 * (B21 - B11)

    add(A11, A12, temp1, mid);
    strassen_naive(temp1, B22, M5, mid); // M5 = (A11 + A12) * B22

    subtract(A21, A11, temp1, mid);
    add(B11, B12, temp2, mid);
    strassen_naive(temp1, temp2, M6, mid); // M6 = (A21 - A11) * (B11 + B12)

    subtract(A12, A22, temp1, mid);
    add(B21, B22, temp2, mid);
    strassen_naive(temp1, temp2, M7, mid); // M7 = (A12 - A22) * (B21 + B22)

    // Calculate C11, C12, C21, and C22
    add(M1, M4, temp1, mid);
    subtract(temp1, M5, temp2, mid);
    add(temp2, M7, C11, mid); // C11 = M1 + M4 - M5 + M7

    add(M3, M5, C12, mid); // C12 = M3 + M5

    add(M2, M4, C21, mid); // C21 = M2 + M4

    add(M1, M3, temp1, mid);
    subtract(temp1, M2, temp2, mid);
    add(temp2, M6, C22, mid); // C22 = M1 + M3 - M2 + M6

    // Combine results into C
    for (int i = 0; i < mid; i++) {
        for (int j = 0; j < mid; j++) {
            C[i][j] = C11[i][j];
            C[i][j + mid] = C12[i][j];
            C[i + mid][j] = C21[i][j];
            C[i + mid][j + mid] = C22[i][j];
        }
    }

    // Free allocated memory
    for (int i = 0; i < mid; i++) {
        free(A11[i]); free(A12[i]); free(A21[i]); free(A22[i]);
        free(B11[i]); free(B12[i]); free(B21[i]); free(B22[i]);
        free(C11[i]); free(C12[i]); free(C21[i]); free(C22[i]);
        free(M1[i]); free(M2[i]); free(M3[i]); free(M4[i]);
        free(M5[i]); free(M6[i]); free(M7[i]);
        free(temp1[i]); free(temp2[i]);
    }

    free(A11); free(A12); free(A21); free(A22);
    free(B11); free(B12); free(B21); free(B22);
    free(C11); free(C12); free(C21); free(C22);
    free(M1); free(M2); free(M3); free(M4);
    free(M5); free(M6); free(M7);
    free(temp1); free(temp2);
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
            A[i][j] = rand() % 1000;
            B[i][j] = rand() % 1000;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", B[i][j]);
        }
        printf("\n");
    }

    strassen_naive(A, B, C, n);

    // Imprime el resultado
    printf("Result matrix C:\n");
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
