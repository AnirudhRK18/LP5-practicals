#include <iostream>
#include <cuda_runtime.h>

using namespace std;

// CUDA kernel to add two vectors
__global__ void addVectors(int* A, int* B, int* C, int N) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < N) {
        C[i] = A[i] + B[i];
    }
}

int main() {
    int N;
    cout << "Enter number of elements: ";
    cin >> N;

    int size = N * sizeof(int);

    // Host vectors
    int* A = new int[N];
    int* B = new int[N];
    int* C = new int[N];

    // Input vectors
    cout << "Enter elements of vector A:\n";
    for (int i = 0; i < N; i++) {
        cin >> A[i];
    }

    cout << "Enter elements of vector B:\n";
    for (int i = 0; i < N; i++) {
        cin >> B[i];
    }

    // Device vectors
    int *d_A, *d_B, *d_C;
    cudaMalloc(&d_A, size);
    cudaMalloc(&d_B, size);
    cudaMalloc(&d_C, size);

    // Copy data to device
    cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice);

    // Launch kernel
    int threadsPerBlock = 256;
    int blocks = (N + threadsPerBlock - 1) / threadsPerBlock;
    addVectors<<<blocks, threadsPerBlock>>>(d_A, d_B, d_C, N);

    // Copy result back to host
    cudaMemcpy(C, d_C, size, cudaMemcpyDeviceToHost);

    // Print result
    cout << "Result of A + B:\n";
    for (int i = 0; i < N; i++) {
        cout << C[i] << " ";
    }
    cout << endl;

    // Free memory
    delete[] A;
    delete[] B;
    delete[] C;
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    return 0;
}
