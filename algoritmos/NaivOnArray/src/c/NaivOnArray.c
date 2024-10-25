#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>

void multiplyMatrices(long long **a, long long **b, long long **result, int rowA, int colA, int colB) {
    for (int i = 0; i < rowA; i++) {
        for (int j = 0; j < colB; j++) {
            result[i][j] = 0; // Inicializa el resultado
            for (int k = 0; k < colA; k++) {
                result[i][j] += a[i][k] * b[k][j];
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

    while (!feof(file)) {
        long long **matrix = NULL;
        int r = 0, c = 0;
        char buffer[4096]; // Tamaño aumentado del buffer

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
                    matrix[r] = malloc(sizeof(long long) * 128); // Ajuste de estimación de columnas
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

void writeMatrix(const char *filename, long long **matrix, int rows, int cols) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("No se puede crear el archivo");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fprintf(file, "%lld ", matrix[i][j]); // Escribe cada elemento
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

    // Elimina el último componente de la ruta y trata de crear los directorios
    for (char *p = temp + 1; *p; p++) {
        if (*p == '/') {
            *p = '\0'; // Termina la cadena aquí para crear el directorio
            if (mkdir(temp, 0777) == -1 && errno != EEXIST) {
                perror("Error creando directorio");
                exit(EXIT_FAILURE);
            }
            *p = '/'; // Restaura el carácter de separación
        }
    }

    // Finalmente, crea el último directorio en la ruta
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

    long long ***matrices; // Cambia el tipo a un puntero a puntero a puntero
    int *rows, *cols, matrixCount;

    readMatrices(matricesFile, &matrices, &rows, &cols, &matrixCount);

    // Asumiendo que tienes exactamente 2 matrices para multiplicar
    if (matrixCount < 2) {
        fprintf(stderr, "Error: Se requieren al menos dos matrices para multiplicar\n");
        return EXIT_FAILURE;
    }

    // Imprimir el número de filas y columnas de las matrices que se van a multiplicar
    printf("Matriz A: %d filas, %d columnas\n", rows[0], cols[0]);
    printf("Matriz B: %d filas, %d columnas\n", rows[1], cols[1]);

    // Verifica si las matrices pueden ser multiplicadas
    if (cols[0] != rows[1]) {
        fprintf(stderr, "Error: Las matrices no se pueden multiplicar\n");
        return EXIT_FAILURE;
    }

    long long **result = malloc(rows[0] * sizeof(long long *));
    for (int i = 0; i < rows[0]; i++) {
        result[i] = calloc(cols[1], sizeof(long long)); // Inicializa el resultado a 0
    }

    clock_t start = clock();
    multiplyMatrices(matrices[0], matrices[1], result, rows[0], cols[0], cols[1]);
    clock_t end = clock();

    // Crea las rutas y nombres de archivos semánticos
    char tiempoFile[100], resultadoFile[100];
    sprintf(tiempoFile, "../../../../Resultados/NaivOnArray/tiempo/tiempo_NaivOnArray_%dx%d.txt", rows[0], cols[1]);
    sprintf(resultadoFile, "../../../../Resultados/NaivOnArray/resultadomultiplicacion/resultado_NaivOnArray_%dx%d.txt", rows[0], cols[1]);

    // Crea los directorios necesarios
    createDirectoriesRecursively("../../../../Resultados/NaivOnArray/tiempo");
    createDirectoriesRecursively("../../../../Resultados/NaivOnArray/resultadomultiplicacion");

    // Calcula y guarda el tiempo de ejecución
    double elapsedTime = (double)(end - start) / CLOCKS_PER_SEC;
    writeExecutionTime(tiempoFile, elapsedTime);

    // Escribe el resultado en el archivo
    writeMatrix(resultadoFile, result, rows[0], cols[1]);

    // Liberar memoria
    for (int i = 0; i < matrixCount; i++) {
        for (int j = 0; j < rows[i]; j++) free(matrices[i][j]);
        free(matrices[i]);
    }
    for (int i = 0; i < rows[0]; i++) free(result[i]);
    free(rows);
    free(cols);
    free(result);
    free(matrices); // Liberar memoria para matrices

    return EXIT_SUCCESS;
}

