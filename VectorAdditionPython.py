import numpy as np
from numba import cuda

# CUDA kernel to add two vectors
@cuda.jit
def add_vectors_kernel(A, B, C, N):
    idx = cuda.threadIdx.x + cuda.blockIdx.x * cuda.blockDim.x
    if idx < N:
        C[idx] = A[idx] + B[idx]

# Input: Number of elements in vectors
N = int(input("Enter number of elements: "))

# Input: Elements of vector A
print("Enter elements of vector A:")
A_host = np.array([int(input()) for _ in range(N)], dtype=np.int32)

# Input: Elements of vector B
print("Enter elements of vector B:")
B_host = np.array([int(input()) for _ in range(N)], dtype=np.int32)

# Prepare output vector C on host
C_host = np.zeros(N, dtype=np.int32)

# Allocate memory on device
A_device = cuda.to_device(A_host)
B_device = cuda.to_device(B_host)
C_device = cuda.device_array(N, dtype=np.int32)

# Define number of threads and blocks
threads_per_block = 256
blocks_per_grid = (N + threads_per_block - 1) // threads_per_block

# Launch the CUDA kernel
add_vectors_kernel[blocks_per_grid, threads_per_block](A_device, B_device, C_device, N)

# Copy result back from device to host
C_device.copy_to_host(C_host)

# Output: Resultant vector C
print("Resultant vector C:")
print(C_host)
