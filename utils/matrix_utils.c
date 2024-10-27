#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int read_matrices_from_file(const char *filename, int64_t ***matrix1, int64_t ***matrix2, int *n) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    *n = 0;
    int64_t value;
    char line[1024];

    // Read the first line to determine matrix size
    if (fgets(line, sizeof(line), file) != NULL) {
        char *ptr = line;
        while (sscanf(ptr, "%ld", &value) == 1) {  // Read as long long
            (*n)++;

            // Move the pointer forward
            ptr += 6;  // Move ptr to the end of the number
            while (*ptr == ' ') {  // Skip any spaces
                ptr++;
            }

        }
    } else {
        fprintf(stderr, "Error reading the first line of the matrix.\n");
        fclose(file);
        return -1;
    }

    // Allocate memory for matrices
    *matrix1 = (int64_t **)malloc(*n * sizeof(int64_t *));
    *matrix2 = (int64_t **)malloc(*n * sizeof(int64_t *));
    for (int i = 0; i < *n; i++) {
        (*matrix1)[i] = (int64_t *)malloc(*n * sizeof(int64_t));
        (*matrix2)[i] = (int64_t *)malloc(*n * sizeof(int64_t));
    }

    // Reset file pointer and read the first matrix
    rewind(file);
    for (int row = 0; row < *n; row++) {
        for (int col = 0; col < *n; col++) {
            if (fscanf(file, "%ld", &(*matrix1)[row][col]) != 1) {  // Read as long long
                perror("Error reading the first matrix");
                fclose(file);
                return -1;
            }
        }
    }

    // Read a blank line (ignore any extra whitespace)
    while (fgets(line, sizeof(line), file) != NULL) {
        if (strcmp(line, "\n") == 0) {
            break;
        }
    }

    // Read the second matrix
    for (int row = 0; row < *n; row++) {
        for (int col = 0; col < *n; col++) {
            if (fscanf(file, "%ld", &(*matrix2)[row][col]) != 1) {  // Read as long long
                perror("Error reading the second matrix");
                fclose(file);
                return -1;
            }
        }
    }

    fclose(file);
    return 0;
}


