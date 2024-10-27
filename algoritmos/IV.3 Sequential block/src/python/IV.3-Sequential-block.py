import numpy as np
import sys

def read_matrices_from_file(filename):
    with open(filename, 'r') as f:
        lines = [line.strip() for line in f if line.strip()]
        
        matrix_size = lines.index('') if '' in lines else len(lines) // 2
        n = matrix_size

        A = np.array(
            [[int(num) for num in line.split()] for line in lines[:n]], dtype=np.int64
        )
        
        B = np.array(
            [[int(num) for num in line.split()] for line in lines[n:]], dtype=np.int64
        )
    
    return A, B, n

def sequential_block_multiply(A, B, n, block_size):
    C = np.zeros((n, n), dtype=np.int64)

    for i in range(0, n, block_size):
        for j in range(0, n, block_size):
            for k in range(0, n, block_size):
                for ii in range(i, min(i + block_size, n)):
                    for jj in range(j, min(j + block_size, n)):
                        sum = 0
                        for kk in range(k, min(k + block_size, n)):
                            sum += A[ii, kk] * B[kk, jj]
                        C[ii, jj] += sum
    return C

def print_matrix(M):
    for row in M:
        print(" ".join(map(str, row)))

def main():
    if len(sys.argv) < 2:
        print("Usage: python Sequential-Block.py <matrix_file>")
        sys.exit(1)
    
    filename = sys.argv[1]
    A, B, n = read_matrices_from_file(filename)
    block_size = 2  # You can adjust the block size if needed

    C = sequential_block_multiply(A, B, n, block_size)
    
    print("Result matrix C:")
    print_matrix(C)

if __name__ == "__main__":
    main()

