#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <math.h>

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
void multiplyMatricesStrassenWinograd(long long** A, long long** B, long long** C, int n) {
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
    multiplyMatricesStrassenWinograd(A11, B11, P1, mid);      // P1 = A11 * B11
    multiplyMatricesStrassenWinograd(A12, B21, P2, mid);      // P2 = A12 * B21
    multiplyMatricesStrassenWinograd(S4, B22, P3, mid);       // P3 = S4 * B22
    multiplyMatricesStrassenWinograd(A22, T4, P4, mid);       // P4 = A22 * T4
    multiplyMatricesStrassenWinograd(S1, T1, P5, mid);        // P5 = S1 * T1

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

// Función para leer matrices desde un archivo
void readMatrices(const char *filename, long long ****matrices, int **rows, int **cols, int *matrixCount) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("No se puede abrir el archivo");
        exit(EXIT_FAILURE);
    }

    long long ***tempMatrices = malloc(10 * sizeof(long long **));
    *rows = malloc(10 * sizeof(int));
    *cols = malloc(10 * sizeof(int));
    int count = 0;

    while (!feof(file)) {
        long long **matrix = NULL;
        int r = 0, c = 0;
        char buffer[8192]; // Tamaño aumentado para grandes matrices

        while (fgets(buffer, sizeof(buffer), file)) {
            if (buffer[0] == '\n') {
                if (r > 0) break;
                continue;
            }

            int colCount = 0;
            char *token = strtok(buffer, " ");
            while (token != NULL) {
                if (colCount == 0) {
                    matrix = realloc(matrix, sizeof(long long *) * (r + 1));
                    if (!matrix) {
                        perror("Error al reasignar memoria");
                        exit(EXIT_FAILURE);
                    }
                    matrix[r] = malloc(sizeof(long long) * (strlen(buffer) / 2 + 1));
                }
                matrix[r][colCount++] = atoll(token);
                token = strtok(NULL, " ");
            }

            if (r == 0) {
                c = colCount;
            } else if (colCount != c) {
                fprintf(stderr, "Error: todas las filas deben tener el mismo número de columnas\n");
                exit(EXIT_FAILURE);
            }

            r++;
        }

        if (r > 0) {
            tempMatrices[count] = matrix;
            (*rows)[count] = r;
            (*cols)[count] = c;
            count++;
        }
    }

    fclose(file);
    *matrices = realloc(tempMatrices, sizeof(long long **) * count);
    *matrixCount = count;
}

// Función para escribir la matriz de resultados en un archivo
void writeMatrix(const char *filename, long long **matrix, int rows, int cols) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("No se puede crear el archivo");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fprintf(file, "%lld ", matrix[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

// Función para registrar el tiempo de ejecución
void writeExecutionTime(const char *filename, double time) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("No se puede crear el archivo de tiempo");
        exit(EXIT_FAILURE);
    }
    fprintf(file, "Tiempo de ejecución: %f segundos\n", time);
    fclose(file);
}

// Función para crear directorios de forma recursiva
void createDirectoriesRecursively(const char *path) {
    char temp[256];
    snprintf(temp, sizeof(temp), "%s", path);

    for (char *p = temp + 1; *p; p++) {
        if (*p == '/') {
            *p = '\0';
            if (mkdir(temp, 0777) == -1 && errno != EEXIST) {
                perror("Error creando directorio");
                exit(EXIT_FAILURE);
            }
            *p = '/';
        }
    }

    if (mkdir(temp, 0777) == -1 && errno != EEXIST) {
        perror("Error creando directorio");
        exit(EXIT_FAILURE);
    }
}

// Función principal
int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <archivo_matrices>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *matricesFile = argv[1];

    long long ***matrices;
    int *rows, *cols, matrixCount;

    readMatrices(matricesFile, &matrices, &rows, &cols, &matrixCount);

    if (matrixCount < 2) {
        fprintf(stderr, "Error: Se requieren al menos dos matrices para multiplicar\n");
        return EXIT_FAILURE;
    }

    if (cols[0] != rows[1]) {
        fprintf(stderr, "Error: Las matrices no se pueden multiplicar\n");
        return EXIT_FAILURE;
    }

    long long **result = malloc(rows[0] * sizeof(long long *));
    for (int i = 0; i < rows[0]; i++) {
        result[i] = calloc(cols[1], sizeof(long long));
    }

    clock_t start = clock();
    multiplyMatricesStrassenWinograd(matrices[0], matrices[1], result, rows[0]);
    clock_t end = clock();

    char tiempoFile[150], resultadoFile[150];
    snprintf(tiempoFile, sizeof(tiempoFile), "../../../../Resultados/StrassenWinograd/c/tiempo/tiempo_StrassenWinograd_%dx%d.txt", rows[0], cols[1]);
    snprintf(resultadoFile, sizeof(resultadoFile), "../../../../Resultados/StrassenWinograd/c/resultadomultiplicacion/resultado_StrassenWinograd_%dx%d.txt", rows[0], cols[1]);

    createDirectoriesRecursively("../../../../Resultados/StrassenWinograd/c/tiempo");
    createDirectoriesRecursively("../../../../Resultados/StrassenWinograd/c/resultadomultiplicacion");

    double elapsedTime = (double)(end - start) / CLOCKS_PER_SEC;
    writeExecutionTime(tiempoFile, elapsedTime);
    writeMatrix(resultadoFile, result, rows[0], cols[1]);

    for (int i = 0; i < matrixCount; i++) {
        for (int j = 0; j < rows[i]; j++) free(matrices[i][j]);
        free(matrices[i]);
    }
    for (int i = 0; i < rows[0]; i++) free(result[i]);
    free(rows);
    free(cols);
    free(result);
    free(matrices);

    return EXIT_SUCCESS;
}