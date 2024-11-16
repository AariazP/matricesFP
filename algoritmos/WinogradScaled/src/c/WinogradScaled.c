#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <math.h>

// algoritmo Winograd Scaled para multiplicacion de matrices
long double** multiplyMatricesWinogradScaled(long double **A, long double **B, long long n) {

    long double **C = calloc(n, sizeof(long double*));
    for (long long i = 0; i < n; i++) {
        C[i] = calloc(n, sizeof(long double));
    }

    long double *row_scale = calloc(n, sizeof(long double));
    long double *col_scale = calloc(n, sizeof(long double));

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
            C[i][j] = (long double)C[i][j];
        }
    }

    free(row_scale);
    free(col_scale);
    free(row_factor);
    free(col_factor);

    return C;
}

// Función para leer matrices desde un archivo
void readMatrices(const char *filename, long double ****matrices, int **rows, int **cols, int *matrixCount) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("No se puede abrir el archivo");
        exit(EXIT_FAILURE);
    }

    long double ***tempMatrices = malloc(10 * sizeof(long long **));
    *rows = malloc(10 * sizeof(int));
    *cols = malloc(10 * sizeof(int));
    int count = 0;

    while (!feof(file)) {
        long double **matrix = NULL;
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
void writeMatrix(const char *filename, long double **matrix, int rows, int cols) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("No se puede crear el archivo");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fprintf(file, "%Lf ", matrix[i][j]);
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

    long double ***matrices;
    int *rows, *cols, matrixCount;
    long double **result;

    readMatrices(matricesFile, &matrices, &rows, &cols, &matrixCount);

    if (matrixCount < 2) {
        fprintf(stderr, "Error: Se requieren al menos dos matrices para multiplicar\n");
        return EXIT_FAILURE;
    }

    if (cols[0] != rows[1]) {
        fprintf(stderr, "Error: Las matrices no se pueden multiplicar\n");
        return EXIT_FAILURE;
    }

    result = malloc(rows[0] * sizeof(long long *));
    for (int i = 0; i < rows[0]; i++) {
        result[i] = calloc(cols[1], sizeof(long long));
    }

    clock_t start = clock();
    result = multiplyMatricesWinogradScaled(matrices[0], matrices[1], rows[0]);
    clock_t end = clock();

    char tiempoFile[150], resultadoFile[150];
    snprintf(tiempoFile, sizeof(tiempoFile), "../../../../Resultados/WinogradScaled/c/tiempo/tiempo_WinogradScaled_%dx%d.txt", rows[0], cols[1]);
    snprintf(resultadoFile, sizeof(resultadoFile), "../../../../Resultados/WinogradScaled/c/resultadomultiplicacion/resultado_WinogradScaled_%dx%d.txt", rows[0], cols[1]);

    createDirectoriesRecursively("../../../../Resultados/WinogradScaled/c/tiempo");
    createDirectoriesRecursively("../../../../Resultados/WinogradScaled/c/resultadomultiplicacion");

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