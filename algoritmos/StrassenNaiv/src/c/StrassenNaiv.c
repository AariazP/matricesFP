#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

// Operaciones básicas para matrices
void add(long long **A, long long **B, long long **C, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] + B[i][j];
}

void subtract(long long **A, long long **B, long long **C, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] - B[i][j];
}

// Algoritmo de Strassen
void multiplyMatricesStrassenNaiv(long long **A, long long **B, long long **C, int n) {
    if (n == 1) {
        C[0][0] = A[0][0] * B[0][0];
        return;
    }

    int mid = n / 2;

    // Reservar memoria para las submatrices
    long long **A11 = malloc(mid * sizeof(long long *));
    long long **A12 = malloc(mid * sizeof(long long *));
    long long **A21 = malloc(mid * sizeof(long long *));
    long long **A22 = malloc(mid * sizeof(long long *));
    long long **B11 = malloc(mid * sizeof(long long *));
    long long **B12 = malloc(mid * sizeof(long long *));
    long long **B21 = malloc(mid * sizeof(long long *));
    long long **B22 = malloc(mid * sizeof(long long *));
    long long **C11 = malloc(mid * sizeof(long long *));
    long long **C12 = malloc(mid * sizeof(long long *));
    long long **C21 = malloc(mid * sizeof(long long *));
    long long **C22 = malloc(mid * sizeof(long long *));
    long long **M1 = malloc(mid * sizeof(long long *));
    long long **M2 = malloc(mid * sizeof(long long *));
    long long **M3 = malloc(mid * sizeof(long long *));
    long long **M4 = malloc(mid * sizeof(long long *));
    long long **M5 = malloc(mid * sizeof(long long *));
    long long **M6 = malloc(mid * sizeof(long long *));
    long long **M7 = malloc(mid * sizeof(long long *));
    long long **temp1 = malloc(mid * sizeof(long long *));
    long long **temp2 = malloc(mid * sizeof(long long *));

    for (int i = 0; i < mid; i++) {
        A11[i] = malloc(mid * sizeof(long long));
        A12[i] = malloc(mid * sizeof(long long));
        A21[i] = malloc(mid * sizeof(long long));
        A22[i] = malloc(mid * sizeof(long long));
        B11[i] = malloc(mid * sizeof(long long));
        B12[i] = malloc(mid * sizeof(long long));
        B21[i] = malloc(mid * sizeof(long long));
        B22[i] = malloc(mid * sizeof(long long));
        C11[i] = malloc(mid * sizeof(long long));
        C12[i] = malloc(mid * sizeof(long long));
        C21[i] = malloc(mid * sizeof(long long));
        C22[i] = malloc(mid * sizeof(long long));
        M1[i] = malloc(mid * sizeof(long long));
        M2[i] = malloc(mid * sizeof(long long));
        M3[i] = malloc(mid * sizeof(long long));
        M4[i] = malloc(mid * sizeof(long long));
        M5[i] = malloc(mid * sizeof(long long));
        M6[i] = malloc(mid * sizeof(long long));
        M7[i] = malloc(mid * sizeof(long long));
        temp1[i] = malloc(mid * sizeof(long long));
        temp2[i] = malloc(mid * sizeof(long long));
    }

    // Dividir en submatrices
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

    // Calcular los productos de Strassen
    add(A11, A22, temp1, mid);
    add(B11, B22, temp2, mid);
    multiplyMatricesStrassenNaiv(temp1, temp2, M1, mid); // M1 = (A11 + A22) * (B11 + B22)

    add(A21, A22, temp1, mid);
    multiplyMatricesStrassenNaiv(temp1, B11, M2, mid); // M2 = (A21 + A22) * B11

    subtract(B12, B22, temp2, mid);
    multiplyMatricesStrassenNaiv(A11, temp2, M3, mid); // M3 = A11 * (B12 - B22)

    subtract(B21, B11, temp2, mid);
    multiplyMatricesStrassenNaiv(A22, temp2, M4, mid); // M4 = A22 * (B21 - B11)

    add(A11, A12, temp1, mid);
    multiplyMatricesStrassenNaiv(temp1, B22, M5, mid); // M5 = (A11 + A12) * B22

    subtract(A21, A11, temp1, mid);
    add(B11, B12, temp2, mid);
    multiplyMatricesStrassenNaiv(temp1, temp2, M6, mid); // M6 = (A21 - A11) * (B11 + B12)

    subtract(A12, A22, temp1, mid);
    add(B21, B22, temp2, mid);
    multiplyMatricesStrassenNaiv(temp1, temp2, M7, mid); // M7 = (A12 - A22) * (B21 + B22)

    // Calcular C11, C12, C21 y C22
    add(M1, M4, temp1, mid);
    subtract(temp1, M5, temp2, mid);
    add(temp2, M7, C11, mid); // C11 = M1 + M4 - M5 + M7

    add(M3, M5, C12, mid); // C12 = M3 + M5

    add(M2, M4, C21, mid); // C21 = M2 + M4

    add(M1, M3, temp1, mid);
    subtract(temp1, M2, temp2, mid);
    add(temp2, M6, C22, mid); // C22 = M1 + M3 - M2 + M6

    // Combinar submatrices en C
    for (int i = 0; i < mid; i++) {
        for (int j = 0; j < mid; j++) {
            C[i][j] = C11[i][j];
            C[i][j + mid] = C12[i][j];
            C[i + mid][j] = C21[i][j];
            C[i + mid][j + mid] = C22[i][j];
        }
    }

    // Liberar memoria
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
    multiplyMatricesStrassenNaiv(matrices[0], matrices[1], result, rows[0]);
    clock_t end = clock();

    char tiempoFile[150], resultadoFile[150];
    snprintf(tiempoFile, sizeof(tiempoFile), "../../../../Resultados/StrassenNaiv/c/tiempo/tiempo_StrassenNaiv_%dx%d.txt", rows[0], cols[1]);
    snprintf(resultadoFile, sizeof(resultadoFile), "../../../../Resultados/StrassenNaiv/c/resultadomultiplicacion/resultado_StrassenNaiv_%dx%d.txt", rows[0], cols[1]);

    createDirectoriesRecursively("../../../../Resultados/StrassenNaiv/c/tiempo");
    createDirectoriesRecursively("../../../../Resultados/StrassenNaiv/c/resultadomultiplicacion");

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