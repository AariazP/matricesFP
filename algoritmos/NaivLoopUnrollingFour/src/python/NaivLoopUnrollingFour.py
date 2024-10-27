import os
import time
import errno
import numpy as np

def multiply_matrices_naiv_loop_unrolling_four(a, b):
    rowA, colA = a.shape
    colB = b.shape[1]
    result = np.zeros((rowA, colB), dtype=np.int64)
    
    for i in range(rowA):
        for j in range(colB):
            for k in range(0, colA, 4):
                result[i, j] += a[i, k] * b[k, j]
                if k + 1 < colA:
                    result[i, j] += a[i, k + 1] * b[k + 1, j]
                if k + 2 < colA:
                    result[i, j] += a[i, k + 2] * b[k + 2, j]
                if k + 3 < colA:
                    result[i, j] += a[i, k + 3] * b[k + 3, j]
    return result

def read_matrices(filename):
    matrices = []
    with open(filename, 'r') as file:
        matrix = []
        for line in file:
            if line.strip() == '':
                if matrix:
                    matrices.append(np.array(matrix, dtype=np.int64))
                    matrix = []
            else:
                matrix.append(list(map(int, line.strip().split())))
        if matrix:
            matrices.append(np.array(matrix, dtype=np.int64))
    return matrices

def write_matrix(filename, matrix):
    with open(filename, 'w') as file:
        for row in matrix:
            file.write(" ".join(map(str, row)) + "\n")

def write_execution_time(filename, elapsed_time):
    with open(filename, 'w') as file:
        file.write(f"Tiempo de ejecución: {elapsed_time:.6f} segundos\n")

def create_directories_recursively(path):
    try:
        os.makedirs(path, exist_ok=True)
    except OSError as e:
        if e.errno != errno.EEXIST:
            raise

def main(filename):
    matrices = read_matrices(filename)

    if len(matrices) < 2:
        raise ValueError("Error: Se requieren al menos dos matrices para multiplicar")

    a, b = matrices[0], matrices[1]
    
    if a.shape[1] != b.shape[0]:
        raise ValueError("Error: Las matrices no se pueden multiplicar")

    start_time = time.time()
    result = multiply_matrices_naiv_loop_unrolling_four(a, b)
    elapsed_time = time.time() - start_time

    # Rutas para los archivos de tiempo y resultados
    tiempo_dir = "../../../../Resultados/NaivLoopUnrollingFour/python/tiempo"
    resultado_dir = "../../../../Resultados/NaivLoopUnrollingFour/python/resultadomultiplicacion"
    tiempo_filename = f"{tiempo_dir}/tiempo_NaivLoopUnrollingFour_{a.shape[0]}x{b.shape[1]}.txt"
    resultado_filename = f"{resultado_dir}/resultado_NaivLoopUnrollingFour_{a.shape[0]}x{b.shape[1]}.txt"

    create_directories_recursively(tiempo_dir)
    create_directories_recursively(resultado_dir)

    write_execution_time(tiempo_filename, elapsed_time)
    write_matrix(resultado_filename, result)

if __name__ == "__main__":
    import sys
    if len(sys.argv) < 2:
        print("Uso: python script.py <archivo_matrices>")
        sys.exit(1)

    matrices_file = sys.argv[1]
    main(matrices_file)

