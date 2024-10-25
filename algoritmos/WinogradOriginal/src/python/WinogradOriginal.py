import os
import time

def multiply_matrices_winograd(a, b):
    row_a = len(a)
    col_a = len(a[0])
    col_b = len(b[0])

    row_factor = [0] * row_a
    col_factor = [0] * col_b

    for i in range(row_a):
        for j in range(col_a // 2):
            row_factor[i] += a[i][2 * j] * a[i][2 * j + 1]

    for i in range(col_b):
        for j in range(len(b) // 2):
            col_factor[i] += b[2 * j][i] * b[2 * j + 1][i]

    result = [[-row_factor[i] - col_factor[j] for j in range(col_b)] for i in range(row_a)]

    for i in range(row_a):
        for j in range(col_b):
            for k in range(col_a // 2):
                result[i][j] += (a[i][2 * k] + b[2 * k + 1][j]) * (a[i][2 * k + 1] + b[2 * k][j])

    if col_a % 2 == 1:
        for i in range(row_a):
            for j in range(col_b):
                result[i][j] += a[i][col_a - 1] * b[col_a - 1][j]

    return result


def read_matrices(filename):
    matrices = []
    with open(filename, 'r') as file:
        matrix = []
        for line in file:
            if line.strip():
                row = list(map(int, line.split()))
                matrix.append(row)
            else:
                if matrix:
                    matrices.append(matrix)
                    matrix = []
        if matrix:
            matrices.append(matrix)
    return matrices


def write_matrix(filename, matrix):
    with open(filename, 'w') as file:
        for row in matrix:
            file.write(' '.join(map(str, row)) + '\n')


def write_execution_time(filename, elapsed_time):
    with open(filename, 'w') as file:
        file.write(f'Tiempo de ejecución: {elapsed_time:.6f} segundos\n')


def create_directories_recursively(path):
    os.makedirs(path, exist_ok=True)


def main(matrices_file):
    matrices = read_matrices(matrices_file)

    if len(matrices) < 2:
        raise ValueError("Error: Se requieren al menos dos matrices para multiplicar")

    a = matrices[0]
    b = matrices[1]

    if len(a[0]) != len(b):
        raise ValueError("Error: Las matrices no se pueden multiplicar")

    start_time = time.time()
    result = multiply_matrices_winograd(a, b)
    elapsed_time = time.time() - start_time

    # Crear directorios para guardar resultados
    tiempo_file = "../../../../Resultados/WinogradOriginal/python/tiempo/tiempo_WinogradOriginal_{}x{}.txt".format(len(a), len(b[0]))
    resultado_file = "../../../../Resultados/WinogradOriginal/python/resultadomultiplicacion/resultado_WinogradOriginal_{}x{}.txt".format(len(a), len(b[0]))

    create_directories_recursively("../../../../Resultados/WinogradOriginal/python/tiempo")
    create_directories_recursively("../../../../Resultados/WinogradOriginal/python/resultadomultiplicacion")

    write_execution_time(tiempo_file, elapsed_time)
    write_matrix(resultado_file, result)


if __name__ == "__main__":
    import sys

    if len(sys.argv) < 2:
        print(f"Uso: {sys.argv[0]} <archivo_matrices>")
        sys.exit(1)

    matrices_file = sys.argv[1]
    try:
        main(matrices_file)
    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)

