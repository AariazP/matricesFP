# Algoritmo extraido de https://www.youtube.com/watch?v=3luSXwpJ3IQ por Liz A. Noguera

import numpy as np
import os
import time

# Función para dividir una matriz en submatrices
def split(matrix):
    row, col = matrix.shape
    row2, col2 = row // 2, col // 2
    return matrix[:row2, :col2], matrix[:row2, col2:], matrix[row2:, :col2], matrix[row2:, col2:]

# Función de multiplicación de matrices usando el algoritmo de Strassen
def strassen(x, y):
    if len(x) == 1:
        return x * y
    # Dividir las matrices en submatrices
    a, b, c, d = split(x)
    e, f, g, h = split(y)

    # Calcular los productos intermedios usando llamadas recursivas
    p1 = strassen(a, f - h)
    p2 = strassen(a + b, h)
    p3 = strassen(c + d, e)
    p4 = strassen(d, g - e)
    p5 = strassen(a + d, e + h)
    p6 = strassen(b - d, g + h)
    p7 = strassen(a - c, e + f)

    # Calcular las submatrices del resultado final
    c11 = p5 + p4 - p2 + p6
    c12 = p1 + p2
    c21 = p3 + p4
    c22 = p1 + p5 - p3 - p7

    # Combinar las submatrices en una matriz completa
    c = np.vstack((np.hstack((c11, c12)), np.hstack((c21, c22))))
    return c

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
    result = strassen(a, b)
    elapsed_time = time.time() - start_time

    # Crear directorios para guardar resultados
    tiempo_file = "../../../../Resultados/StrassenNaiv/python/tiempo/tiempo_StrassenNaiv_{}x{}.txt".format(len(a), len(b[0]))
    resultado_file = "../../../../Resultados/StrassenNaiv/python/resultadomultiplicacion/resultado_StrassenNaiv_{}x{}.txt".format(len(a), len(b[0]))

    create_directories_recursively("../../../../Resultados/StrassenNaiv/python/tiempo")
    create_directories_recursively("../../../../Resultados/StrassenNaiv/python/resultadomultiplicacion")

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