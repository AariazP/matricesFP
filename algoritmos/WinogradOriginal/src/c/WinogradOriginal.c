#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>

// Implementación de la multiplicación de matrices con el algoritmo Winograd
void multiplyMatricesWinograd(long long **a, long long **b, long long **result, int rowA, int colA, int colB) {
    long long *rowFactor = calloc(rowA, sizeof(long long));
    long long *colFactor = calloc(colB, sizeof(long long));

    for (int i = 0; i < rowA; i++)
        for (int j = 0; j < colA / 2; j++)
            rowFactor[i] += a[i][2 * j] * a[i][2 * j + 1];

    for (int i = 0; i < colB; i++)
        for (int j = 0; j < rowA / 2; j++)
            colFactor[i] += b[2 * j][i] * b[2 * j + 1][i];

    for (int i = 0; i < rowA; i++) {
        for (int j = 0; j < colB; j++) {
            result[i][j] = -rowFactor[i] - colFactor[j];
            for (int k = 0; k < colA / 2; k++) {
                result[i][j] += (a[i][2 * k] + b[2 * k + 1][j]) *
                                (a[i][2 * k + 1] + b[2 * k][j]);
            }
        }
    }

    if (colA % 2 == 1) {
        for (int i = 0; i < rowA; i++)
            for (int j = 0; j < colB; j++)
                result[i][j] += a[i][colA - 1] * b[colA - 1][j];
    }

    free(rowFactor);
    free(colFactor);
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
    multiplyMatricesWinograd(matrices[0], matrices[1], result, rows[0], cols[0], cols[1]);
    clock_t end = clock();

    char tiempoFile[150], resultadoFile[150];
    snprintf(tiempoFile, sizeof(tiempoFile), "../../../../Resultados/WinogradOriginal/c/tiempo/tiempo_WinogradOriginal_%dx%d.txt", rows[0], cols[1]);
    snprintf(resultadoFile, sizeof(resultadoFile), "../../../../Resultados/WinogradOriginal/c/resultadomultiplicacion/resultado_WinogradOriginal_%dx%d.txt", rows[0], cols[1]);

    createDirectoriesRecursively("../../../../Resultados/WinogradOriginal/c/tiempo");
    createDirectoriesRecursively("../../../../Resultados/WinogradOriginal/c/resultadomultiplicacion");

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

