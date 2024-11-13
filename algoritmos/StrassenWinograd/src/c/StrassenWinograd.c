#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Función auxiliar para crear una matriz dinámica de tamaño n x n
long long** crear_matriz(int n) {
    long long** matriz = (long long**)malloc(n * sizeof(long long*));
    if (matriz == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria\n");
        exit(1);
    }
    for (int i = 0; i < n; i++) {
        matriz[i] = (long long*)calloc(n, sizeof(long long));
        if (matriz[i] == NULL) {
            fprintf(stderr, "Error: No se pudo asignar memoria\n");
            exit(1);
        }
    }
    return matriz;
}

// Función para liberar la memoria de una matriz
void liberar_matriz(long long** matriz, int n) {
    if (matriz == NULL) return;
    for (int i = 0; i < n; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

// Función para sumar dos matrices
void sumar_matrices(long long** A, long long** B, long long** C, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
}

// Función para restar dos matrices
void restar_matrices(long long** A, long long** B, long long** C, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
}

// Función para multiplicar matrices usando el algoritmo de Strassen-Winograd
void strassen_winograd(long long** A, long long** B, long long** C, int n) {
    // Caso base: matrices pequeñas
    if (n <= 64) { // Umbral ajustable según rendimiento
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
    long long** A11 = crear_matriz(mid);
    long long** A12 = crear_matriz(mid);
    long long** A21 = crear_matriz(mid);
    long long** A22 = crear_matriz(mid);
    long long** B11 = crear_matriz(mid);
    long long** B12 = crear_matriz(mid);
    long long** B21 = crear_matriz(mid);
    long long** B22 = crear_matriz(mid);
    long long** C11 = crear_matriz(mid);
    long long** C12 = crear_matriz(mid);
    long long** C21 = crear_matriz(mid);
    long long** C22 = crear_matriz(mid);
    
    // Matrices temporales para Winograd
    long long** S1 = crear_matriz(mid);
    long long** S2 = crear_matriz(mid);
    long long** S3 = crear_matriz(mid);
    long long** S4 = crear_matriz(mid);
    long long** T1 = crear_matriz(mid);
    long long** T2 = crear_matriz(mid);
    long long** T3 = crear_matriz(mid);
    long long** T4 = crear_matriz(mid);
    long long** P1 = crear_matriz(mid);
    long long** P2 = crear_matriz(mid);
    long long** P3 = crear_matriz(mid);
    long long** P4 = crear_matriz(mid);
    long long** P5 = crear_matriz(mid);
    long long** U1 = crear_matriz(mid);
    long long** U2 = crear_matriz(mid);
    long long** U3 = crear_matriz(mid);
    long long** U4 = crear_matriz(mid);
    long long** U5 = crear_matriz(mid);

    // Dividir las matrices en submatrices
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

    // Cálculos de Winograd
    sumar_matrices(A21, A22, S1, mid);   // S1 = A21 + A22
    restar_matrices(S1, A11, S2, mid);   // S2 = S1 - A11
    restar_matrices(A11, A21, S3, mid);  // S3 = A11 - A21
    restar_matrices(A12, S2, S4, mid);   // S4 = A12 - S2
    
    restar_matrices(B12, B11, T1, mid);  // T1 = B12 - B11
    restar_matrices(B22, T1, T2, mid);   // T2 = B22 - T1
    restar_matrices(B22, B12, T3, mid);  // T3 = B22 - B12
    restar_matrices(T2, B21, T4, mid);   // T4 = T2 - B21

    // Productos recursivos
    strassen_winograd(A11, B11, P1, mid);      // P1 = A11 * B11
    strassen_winograd(A12, B21, P2, mid);      // P2 = A12 * B21
    strassen_winograd(S4, B22, P3, mid);       // P3 = S4 * B22
    strassen_winograd(A22, T4, P4, mid);       // P4 = A22 * T4
    strassen_winograd(S1, T1, P5, mid);        // P5 = S1 * T1

    // Cálculo de las submatrices de C usando Winograd
    sumar_matrices(P1, P2, C11, mid);          // C11 = P1 + P2
    sumar_matrices(P1, P3, C12, mid);          // C12 = P1 + P3
    sumar_matrices(P1, P4, C21, mid);          // C21 = P1 + P4
    sumar_matrices(C11, P5, C22, mid);         // C22 = C11 + P5

    // Combinar las submatrices en la matriz resultado
    for (int i = 0; i < mid; i++) {
        for (int j = 0; j < mid; j++) {
            C[i][j] = C11[i][j];
            C[i][j + mid] = C12[i][j];
            C[i + mid][j] = C21[i][j];
            C[i + mid][j + mid] = C22[i][j];
        }
    }

    // Liberar memoria
    liberar_matriz(A11, mid); liberar_matriz(A12, mid);
    liberar_matriz(A21, mid); liberar_matriz(A22, mid);
    liberar_matriz(B11, mid); liberar_matriz(B12, mid);
    liberar_matriz(B21, mid); liberar_matriz(B22, mid);
    liberar_matriz(C11, mid); liberar_matriz(C12, mid);
    liberar_matriz(C21, mid); liberar_matriz(C22, mid);
    liberar_matriz(S1, mid); liberar_matriz(S2, mid);
    liberar_matriz(S3, mid); liberar_matriz(S4, mid);
    liberar_matriz(T1, mid); liberar_matriz(T2, mid);
    liberar_matriz(T3, mid); liberar_matriz(T4, mid);
    liberar_matriz(P1, mid); liberar_matriz(P2, mid);
    liberar_matriz(P3, mid); liberar_matriz(P4, mid);
    liberar_matriz(P5, mid);
    liberar_matriz(U1, mid); liberar_matriz(U2, mid);
    liberar_matriz(U3, mid); liberar_matriz(U4, mid);
    liberar_matriz(U5, mid);
}

// Función de multiplicación estándar para comparación
void multiplicacion_estandar(long long** A, long long** B, long long** C, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = 0;
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Función para imprimir una matriz
void imprimir_matriz(long long** matriz, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%lld ", matriz[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    srand(time(NULL));  // Inicializar semilla para números aleatorios
    
    // Asegurarse que n sea potencia de 2
    int n = 4;
    
    // Crear y asignar memoria para las matrices
    long long** A = crear_matriz(n);
    long long** B = crear_matriz(n);
    long long** C = crear_matriz(n);
    long long** C_std = crear_matriz(n);

    // Inicializar matrices con valores aleatorios
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            A[i][j] = rand() % 10;  // Números más pequeños para facilitar verificación
            B[i][j] = rand() % 10;
        }
    }

    printf("Matriz A:\n");
    imprimir_matriz(A, n);
    
    printf("Matriz B:\n");
    imprimir_matriz(B, n);
    
    // Realizar multiplicación usando Strassen-Winograd
    strassen_winograd(A, B, C, n);
    printf("Resultado usando Strassen-Winograd:\n");
    imprimir_matriz(C, n);
    
    // Realizar multiplicación estándar para comparación
    multiplicacion_estandar(A, B, C_std, n);
    printf("Resultado usando multiplicación estándar:\n");
    imprimir_matriz(C_std, n);

    // Liberar memoria
    liberar_matriz(A, n);
    liberar_matriz(B, n);
    liberar_matriz(C, n);
    liberar_matriz(C_std, n);

    return 0;
}