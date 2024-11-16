import numpy as np
import os
import time

def strassen_winograd(A, B):
    n = A.shape[0]

    if n <= 2:  # Caso base: multiplicación directa
        return np.dot(A, B)

    # Divide las matrices en submatrices
    midpoint = n // 2
    A11, A12, A21, A22 = A[:midpoint, :midpoint], A[:midpoint, midpoint:], A[midpoint:, :midpoint], A[midpoint:, midpoint:]
    B11, B12, B21, B22 = B[:midpoint, :midpoint], B[:midpoint, midpoint:], B[midpoint:, :midpoint], B[midpoint:, midpoint:]

    # Calcular las 7 matrices intermedias
    M1 = strassen_winograd(A11 + A22, B11 + B22)
    M2 = strassen_winograd(A21 + A22, B11)
    M3 = strassen_winograd(A11, B12 - B22)
    M4 = strassen_winograd(A22, B21 - B11)
    M5 = strassen_winograd(A11 + A12, B22)
    M6 = strassen_winograd(A21 - A11, B11 + B12)
    M7 = strassen_winograd(A12 - A22, B21 + B22)

    # Calcular la matriz C a partir de las matrices intermedias
    C11 = M1 + M4 - M5 + M7
    C12 = M3 + M5
    C21 = M2 + M4
    C22 = M1 - M2 + M3 + M6

    # Combinar las submatrices en C
    C = np.zeros((n, n))
    C[:midpoint, :midpoint] = C11
    C[:midpoint, midpoint:] = C12
    C[midpoint:, :midpoint] = C21
    C[midpoint:, midpoint:] = C22

    return C

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
                    matrices.append(np.array(matrix))  # Convertir a np.array
                    matrix = []
        if matrix:
            matrices.append(np.array(matrix))  # Agregar la última matriz
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

    A, B = matrices[:2]

    if A.shape[1] != B.shape[0]:
        raise ValueError("Error: Las matrices no se pueden multiplicar")

    start_time = time.time()
    result = strassen_winograd(A, B)
    elapsed_time = time.time() - start_time

    # Crear directorios para guardar resultados
    base_path = "../../../../Resultados/StrassenWinograd/python"
    tiempo_dir = os.path.join(base_path, "tiempo")
    resultado_dir = os.path.join(base_path, "resultadomultiplicacion")

    create_directories_recursively(tiempo_dir)
    create_directories_recursively(resultado_dir)

    # Crear nombres de archivo dinámicos
    tiempo_file = os.path.join(tiempo_dir, f"tiempo_StrassenWinograd_{A.shape[0]}x{B.shape[1]}.txt")
    resultado_file = os.path.join(resultado_dir, f"resultado_StrassenWinograd_{A.shape[0]}x{B.shape[1]}.txt")

    # Escribir resultados
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
