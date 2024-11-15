import os
import time

def winograd_scaled(A, B):
    # Dimensiones de las matrices
    m, n = len(A), len(A[0])  # A es de tamaño m x n
    p = len(B[0])  # B es de tamaño n x p

    # Inicialización de la matriz resultado C (m x p)
    C = [[0] * p for _ in range(m)]

    # Factores de escala
    row_scale = [max(abs(A[i][j]) for j in range(n)) for i in range(m)]
    col_scale = [max(abs(B[i][j]) for i in range(n)) for j in range(p)]

    # Factores de fila y columna
    row_factor = [0] * m
    col_factor = [0] * p

    # Calcular factores de fila escalados
    for i in range(m):
        for j in range(n // 2):
            row_factor[i] += (A[i][2 * j] / row_scale[i]) * (A[i][2 * j + 1] / row_scale[i])

    # Calcular factores de columna escalados
    for j in range(p):
        for i in range(n // 2):
            col_factor[j] += (B[2 * i][j] / col_scale[j]) * (B[2 * i + 1][j] / col_scale[j])

    # Calcular la matriz resultado C usando los factores escalados
    for i in range(m):
        for j in range(p):
            # Incluir los factores de fila y columna
            C[i][j] = -row_factor[i] - col_factor[j]
            for k in range(n // 2):
                # Multiplicación escalada de los elementos
                C[i][j] += ((A[i][2 * k] / row_scale[i] + B[2 * k + 1][j] / col_scale[j]) *
                            (A[i][2 * k + 1] / row_scale[i] + B[2 * k][j] / col_scale[j]))
            # Ajuste para matrices con dimensiones impares
            if n % 2 == 1:
                C[i][j] += (A[i][n - 1] / row_scale[i]) * (B[n - 1][j] / col_scale[j])
            # Normalizar el resultado al aplicar los factores de escala
            C[i][j] *= row_scale[i] * col_scale[j]
            
            C[i][j] = int(C[i][j])

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
    result = winograd_scaled(a, b)
    elapsed_time = time.time() - start_time

    # Crear directorios para guardar resultados
    tiempo_file = "../../../../Resultados/WinogradScaled/python/tiempo/tiempo_WinogradScaled_{}x{}.txt".format(len(a), len(b[0]))
    resultado_file = "../../../../Resultados/WinogradScaled/python/resultadomultiplicacion/resultado_WinogradScaled_{}x{}.txt".format(len(a), len(b[0]))

    create_directories_recursively("../../../../Resultados/WinogradScaled/python/tiempo")
    create_directories_recursively("../../../../Resultados/WinogradScaled/python/resultadomultiplicacion")

    write_execution_time(tiempo_file, elapsed_time)
    write_matrix(resultado_file, result)

