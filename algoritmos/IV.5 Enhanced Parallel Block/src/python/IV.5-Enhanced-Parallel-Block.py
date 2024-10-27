import numpy as np
from concurrent.futures import ThreadPoolExecutor
import sys

# Function to read matrices from file

def read_matrices_from_file(filename):
    with open(filename, 'r') as file:
        lines = file.readlines()

    # Determine the dimension n based on the input file
    n = len(lines) // 2 - 1  # Assuming two n x n matrices back-to-back in file

    # Initialize matrices A and B based on n
    A = np.zeros((n, n), dtype=np.int64)
    B = np.zeros((n, n), dtype=np.int64)

    # Fill matrix A
    for i in range(n):
        A[i] = np.array(list(map(int, lines[i].strip().split())))

    # Fill matrix B
    for i in range(n):
        B[i] = np.array(list(map(int, lines[i + n + 1 ].strip().split())))

    

    return A, B, n


# Function to perform block multiplication
def block_multiply(A, B, C, block_size, n, i, j, k):
    for ii in range(i, min(i + block_size, n)):
        for jj in range(j, min(j + block_size, n)):
            sum_val = C[ii, jj]
            for kk in range(k, min(k + block_size, n)):
                sum_val += A[ii, kk] * B[kk, jj]
            C[ii, jj] = sum_val

# Enhanced parallel block matrix multiplication
def enhanced_parallel_block_multiply(A, B, n, block_size=2):
    C = np.zeros((n, n), dtype=np.int64)
    with ThreadPoolExecutor() as executor:
        futures = [
            executor.submit(block_multiply, A, B, C, block_size, n, i, j, k)
            for i in range(0, n, block_size)
            for j in range(0, n, block_size)
            for k in range(0, n, block_size)
        ]
        for future in futures:
            future.result()  # Wait for all threads to complete
    return C

# Function to print the matrix
def print_matrix(M):
    for row in M:
        print(" ".join(map(str, row)))

# Main function
if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python enhanced_parallel_block.py <matrix_file>")
        sys.exit(1)

    filename = sys.argv[1]
    A, B, n = read_matrices_from_file(filename)

    # Perform enhanced parallel block matrix multiplication
    C = enhanced_parallel_block_multiply(A, B, n)

    # Print the result matrix
    print("Result matrix C:")
    print_matrix(C)
