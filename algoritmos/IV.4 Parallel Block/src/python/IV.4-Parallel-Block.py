import numpy as np
import multiprocessing as mp

BLOCK_SIZE = 2  # Block size

def read_matrices_from_file(filename):
    # This function should read two matrices and their size from a file.
    # Replace this with actual file reading logic.
    # Example implementation with dummy matrices for testing:
    with open(filename, 'r') as f:
        # Read lines and strip any surrounding whitespace
        lines = [line.strip() for line in f.readlines() if line.strip()]

        # Determine the matrix size based on the number of rows up to the first blank line
        matrix_size = lines.index('') if '' in lines else len(lines) // 2
        n = matrix_size  # Assuming both matrices are square with the same size

        # Read the first matrix
        A = np.array( [[int(num) for num in line.split()] for line in lines[:n]], dtype=np.int64 )

        # Read the second matrix
        B = np.array( [[int(num) for num in line.split()] for line in lines[n:]], dtype=np.int64 )

    return A, B, n


def parallel_block_multiply_worker(args):
    """Performs block matrix multiplication for a specific block."""
    A, B, n, i, j, k, block_size = args
    C_local = np.zeros((block_size, block_size), dtype=np.int64)
    
    for ii in range(i, min(i + block_size, n)):
        for jj in range(j, min(j + block_size, n)):
            sum = 0
            for kk in range(k, min(k + block_size, n)):
                sum += A[ii, kk] * B[kk, jj]
            C_local[ii - i, jj - j] += sum
    return (i, j, C_local)

def parallel_block_multiply(A, B, n, block_size):
    C = np.zeros((n, n), dtype=np.int64)
    tasks = [
        (A, B, n, i, j, k, block_size)
        for i in range(0, n, block_size)
        for j in range(0, n, block_size)
        for k in range(0, n, block_size)
    ]

    with mp.Pool(mp.cpu_count()) as pool:
        results = pool.map(parallel_block_multiply_worker, tasks)
        for i, j, C_local in results:
            C[i:i + block_size, j:j + block_size] += C_local
    return C

def print_matrix(M):
    for row in M:
        print(" ".join(map(str, row)))

def main(filename):
    # Load matrices from file
    A, B, n = read_matrices_from_file(filename)

    # Perform parallel block matrix multiplication
    C = parallel_block_multiply(A, B, n, BLOCK_SIZE)

    # Print the result matrix
    print("Result matrix C:")
    print_matrix(C)

if __name__ == "__main__":
    import sys
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <input_file>")
        sys.exit(1)

    filename = sys.argv[1]
    main(filename)

