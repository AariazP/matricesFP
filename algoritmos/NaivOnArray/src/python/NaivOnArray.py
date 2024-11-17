import numpy as np
import time
import os

def multiply_matrices(a, b):
    rows_a, cols_a = a.shape
    rows_b, cols_b = b.shape
    if cols_a != rows_b:
        raise ValueError("Error: Las matrices no se pueden multiplicar (dimensiones incompatibles).")
    
    # Crear matriz resultado con ceros
    result = np.zeros((rows_a, cols_b), dtype=int)

    # Calcular el producto de matrices explícitamente
    for i in range(rows_a):
        for j in range(cols_b):
            for k in range(cols_a):
                result[i, j] += a[i, k] * b[k, j]
    
    return result

def read_matrices(filename):
    matrices = []
    with open(filename, 'r') as file:
        matrix = []
        for line in file:
            if line.strip() == "":
                if matrix:
                    matrices.append(np.array(matrix))
                    matrix = []
            else:
                matrix.append([int(num) for num in line.split()])
        if matrix:  # Añadir la última matriz si el archivo no termina en una línea vacía
            matrices.append(np.array(matrix))
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
    result = multiply_matrices(matrices[0], matrices[1])
    end_time = time.time()
    elapsed_time = end_time - start_time

    # Crea los nombres de archivos
    tiempo_file = f"../../../../Resultados/NaivOnArray/python/tiempo/tiempo_NaivOnArray_{matrices[0].shape[0]}x{matrices[1].shape[1]}.txt"
    resultado_file = f"../../../../Resultados/NaivOnArray/python/resultadomultiplicacion/resultado_NaivOnArray_{matrices[0].shape[0]}x{matrices[1].shape[1]}.txt"

    # Guarda el resultado y el tiempo
    write_execution_time(tiempo_file, elapsed_time)
    write_matrix(resultado_file, result)

if __name__ == "__main__":
    import sys
    if len(sys.argv) < 2:
        print("Uso: python script.py <archivo_matrices>")
    else:
        main(sys.argv[1])

