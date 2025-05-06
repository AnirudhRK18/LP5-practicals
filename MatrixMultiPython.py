import numpy as np
from numba import cuda

# CUDA kernel for matrix multiplication
@cuda.jit
def matmul_kernel(A, B, C, N):
    row = cuda.threadIdx.y + cuda.blockIdx.y * cuda.blockDim.y
    col = cuda.threadIdx.x + cuda.blockIdx.x * cuda.blockDim.x

    if row < N and col < N:
        temp = 0
        for k in range(N):
            temp += A[row, k] * B[k, col]
        C[row, col] = temp

# Input: Size of square matrix (N x N)
N = int(input("Enter size N for NxN matrices: "))

# Input matrices from user
print("Enter elements of matrix A row-wise:")
A_host = np.array([int(input()) for _ in range(N * N)], dtype=np.int32).reshape(N, N)

print("Enter elements of matrix B row-wise:")
B_host = np.array([int(input()) for _ in range(N * N)], dtype=np.int32).reshape(N, N)

# Prepare output matrix
C_host = np.zeros((N, N), dtype=np.int32)

# Transfer to device
A_device = cuda.to_device(A_host)
B_device = cuda.to_device(B_host)
C_device = cuda.device_array((N, N), dtype=np.int32)

# Kernel launch config
threads_per_block = (16, 16)
blocks_per_grid_x = (N + threads_per_block[0] - 1) // threads_per_block[0]
blocks_per_grid_y = (N + threads_per_block[1] - 1) // threads_per_block[1]

# Launch the kernel
matmul_kernel[(blocks_per_grid_x, blocks_per_grid_y), threads_per_block](A_device, B_device, C_device, N)

# Copy result back
C_device.copy_to_host(C_host)

# Print result
print("Resultant Matrix C = A * B:")
print(C_host)
