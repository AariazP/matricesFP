import numpy as np
import time
import os

def multiply_matrices_unrolled(a, b):
    rowA, colA = a.shape
    colB = b.shape[1]
    result = np.zeros((rowA, colB), dtype=np.int64)
    
    for i in range(rowA):
        for j in range(colB):
            sum_ = 0
            k = 0
            while k < colA - 1:
                sum_ += a[i, k] * b[k, j] + a[i, k + 1] * b[k + 1, j]
                k += 2
            if k < colA:  # Si colA es impar, multiplica el último elemento
                sum_ += a[i, k] * b[k, j]
            result[i, j] = sum_
    return result

def read_matrices(filename):
    matrices = []
    with open(filename, 'r') as file:
        matrix = []
        for line in file:
            if line.strip() == "":
                if matrix:
                    matrices.append(np.array(matrix, dtype=np.int64))
                    matrix = []
            else:
                matrix.append([int(num) for num in line.split()])
        if matrix:  # Añadir la última matriz si el archivo no termina en una línea vacía
            matrices.append(np.array(matrix, dtype=np.int64))
    return matrices

def write_matrix(filename, matrix):
    os.makedirs(os.path.dirname(filename), exist_ok=True)
    np.savetxt(filename, matrix, fmt='%d')

def write_execution_time(filename, time_elapsed):
    os.makedirs(os.path.dirname(filename), exist_ok=True)
    with open(filename, 'w') as file:
        file.write(f"Tiempo de ejecución: {time_elapsed:.6f} segundos\n")

def main(matrices_file):
    matrices = read_matrices(matrices_file)

    if len(matrices) < 2:
        print("Error: Se requieren al menos dos matrices para multiplicar")
        return

    # Verifica si las matrices pueden ser multiplicadas
    if matrices[0].shape[1] != matrices[1].shape[0]:
        print("Error: Las matrices no se pueden multiplicar")
        return

    # Multiplica matrices y calcula el tiempo de ejecución
    start_time = time.time()
    result = multiply_matrices_unrolled(matrices[0], matrices[1])
    end_time = time.time()
    elapsed_time = end_time - start_time

    # Crea los nombres de archivos
    tiempo_file = f"../../../../Resultados/NaivLoopUnrollingTwo/python/tiempo/tiempo_NaivLoopUnrollingTwo_{matrices[0].shape[0]}x{matrices[1].shape[1]}.txt"
    resultado_file = f"../../../../Resultados/NaivLoopUnrollingTwo/python/resultadomultiplicacion/resultado_NaivLoopUnrollingTwo_{matrices[0].shape[0]}x{matrices[1].shape[1]}.txt"

    # Guarda el resultado y el tiempo
    write_execution_time(tiempo_file, elapsed_time)
    write_matrix(resultado_file, result)

if __name__ == "__main__":
    import sys
    if len(sys.argv) < 2:
        print("Uso: python script.py <archivo_matrices>")
    else:
        main(sys.argv[1])

