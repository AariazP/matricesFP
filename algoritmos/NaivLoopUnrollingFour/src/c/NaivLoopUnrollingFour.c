#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>

#define BUFFER_SIZE 4096 // Tamaño de buffer aumentado

void multiplyMatricesNaivLoopUnrollingFour(long long **a, long long **b, long long **result, int rowA, int colA, int colB) {
    for (int i = 0; i < rowA; i++) {
        for (int j = 0; j < colB; j++) {
            result[i][j] = 0;
            for (int k = 0; k < colA; k += 4) {
                result[i][j] += a[i][k] * b[k][j];
                if (k + 1 < colA) result[i][j] += a[i][k + 1] * b[k + 1][j];
                if (k + 2 < colA) result[i][j] += a[i][k + 2] * b[k + 2][j];
                if (k + 3 < colA) result[i][j] += a[i][k + 3] * b[k + 3][j];
            }
        }
    }
}

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

    char buffer[BUFFER_SIZE];
    while (!feof(file)) {
        long long **matrix = NULL;
        int r = 0, c = -1;  // Establece c en -1 para verificar si cambia

        while (fgets(buffer, sizeof(buffer), file)) {
            if (buffer[0] == '\n') {
                if (r > 0) break;
                continue;
            }

            int colCount = 0;
            char *token = strtok(buffer, " ");
            matrix = realloc(matrix, sizeof(long long *) * (r + 1));
            matrix[r] = malloc(sizeof(long long) * BUFFER_SIZE);

            while (token != NULL) {
                matrix[r][colCount++] = atoll(token);
                token = strtok(NULL, " ");
            }

            if (c == -1) {
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

void writeExecutionTime(const char *filename, double time) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("No se puede crear el archivo de tiempo");
        exit(EXIT_FAILURE);
    }
    fprintf(file, "Tiempo de ejecución: %f segundos\n", time);
    fclose(file);
}

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

    printf("Matriz A: %d filas, %d columnas\n", rows[0], cols[0]);
    printf("Matriz B: %d filas, %d columnas\n", rows[1], cols[1]);

    if (cols[0] != rows[1]) {
        fprintf(stderr, "Error: Las matrices no se pueden multiplicar\n");
        return EXIT_FAILURE;
    }

    long long **result = malloc(rows[0] * sizeof(long long *));
    for (int i = 0; i < rows[0]; i++) {
        result[i] = calloc(cols[1], sizeof(long long));
    }

    clock_t start = clock();
    multiplyMatricesNaivLoopUnrollingFour(matrices[0], matrices[1], result, rows[0], cols[0], cols[1]);
    clock_t end = clock();

    char tiempoFile[200], resultadoFile[200];
    sprintf(tiempoFile, "../../../../Resultados/NaivLoopUnrollingFour/c/tiempo/tiempo_NaivLoopUnrollingFour_%dx%d.txt", rows[0], cols[1]);
    sprintf(resultadoFile, "../../../../Resultados/NaivLoopUnrollingFour/c/resultadomultiplicacion/resultado_NaivLoopUnrollingFour_%dx%d.txt", rows[0], cols[1]);

    createDirectoriesRecursively("../../../../Resultados/NaivLoopUnrollingFour/c/tiempo");
    createDirectoriesRecursively("../../../../Resultados/NaivLoopUnrollingFour/c/resultadomultiplicacion");

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

