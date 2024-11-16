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

    print(row_scale)
    print(col_scale)

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
            
            # Usar un umbral para evitar redondeo prematuro a cero
            if abs(C[i][j]) < 1e-6:  # Umbral pequeño (ajústalo según tu caso)
                C[i][j] = 0
            else:
                C[i][j] = round(C[i][j])  # Redondear si el valor es significativo

    return C

def main():
    # Matriz A (4x4)
    A = [
        [1, 2, 3, 4],
        [5, 6, 7, 8],
        [9, 10, 11, 12],
        [13, 14, 15, 16]
    ]

    # Matriz B (4x4)
    B = [
        [17, 18, 19, 20],
        [21, 22, 23, 24],
        [25, 26, 27, 28],
        [29, 30, 31, 32]
    ]

    # Calcular el producto escalado usando Winograd
    C = winograd_scaled(A, B)

    # Mostrar las matrices y el resultado
    print("Matriz A:")
    for row in A:
        print(row)
    print("\nMatriz B:")
    for row in B:
        print(row)
    print("\nResultado (AxB):")
    for row in C:
        print(row)

if __name__ == "__main__":
    main()

