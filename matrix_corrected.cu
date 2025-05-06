#include <bits/stdc++.h>

using namespace std;
using namespace std::chrono;

__global__ void multiply(int* A, int* B, int* C, int M, int N, int K) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < M && col < K) {
        int sum = 0;
        for (int i = 0; i < N; i++) {
            sum += A[row * N + i] * B[i * K + col];
        }
        C[row * K + col] = sum;
    }
}

void initialize(int* matrix, int rows, int cols) {
    for (int i = 0; i < rows * cols; i++) {
        cout << "Enter element " << i + 1 << ": ";
        cin >> matrix[i];
    }
}

void print(int* matrix, int rows, int cols) {
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            cout << matrix[row * cols + col] << " ";
        }
        cout << '\n';
    }
    cout << '\n';
}

void sequentialMultiply(int* A, int* B, int* C, int M, int N, int K) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < K; j++) {
            int sum = 0;
            for (int k = 0; k < N; k++) {
                sum += A[i * N + k] * B[k * K + j];
            }
            C[i * K + j] = sum;
        }
    }
}

int main() {
    int M, N, K;
    cout << "Enter the number of rows and columns of the first matrix: ";
    cin >> M >> N;
    cout << "Enter the number of columns of the second matrix: ";
    cin >> K;

    int* A, * B, * C;

    int matrixSize = M * K;
    size_t matrixBytes = matrixSize * sizeof(int);

    A = new int[M * N];
    B = new int[N * K];
    C = new int[M * K];

    initialize(A, M, N);
    initialize(B, N, K);

    cout << "Matrix A: \n";
    print(A, M, N);

    cout << "Matrix B: \n";
    print(B, N, K);

    int* X, * Y, * Z;
    cudaMalloc(&X, M * N * sizeof(int));
    cudaMalloc(&Y, N * K * sizeof(int));
    cudaMalloc(&Z, M * K * sizeof(int));

    cudaMemcpy(X, A, M * N * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(Y, B, N * K * sizeof(int), cudaMemcpyHostToDevice);

    int THREADS = 16;
    int BLOCKS = (M + THREADS - 1) / THREADS;

    dim3 threads(THREADS, THREADS);
    dim3 blocks(BLOCKS, BLOCKS);

    // Sequential multiplication
    auto start = high_resolution_clock::now();
    sequentialMultiply(A, B, C, M, N, K);
    auto stop = high_resolution_clock::now();
    auto seq_duration = duration_cast<microseconds>(stop - start);

    cout << "Sequential Multiplication of matrix A and B: \n";
    print(C, M, K);

    // Parallel multiplication
    start = high_resolution_clock::now();
    multiply<<<blocks, threads>>>(X, Y, Z, M, N, K);
    cudaMemcpy(C, Z, M * K * sizeof(int), cudaMemcpyDeviceToHost);
    stop = high_resolution_clock::now();
    auto par_duration = duration_cast<microseconds>(stop - start);

    cout << "Parallel Multiplication of matrix A and B: \n";
    print(C, M, K);

    cout << "Sequential Multiplication Time: " << seq_duration.count() << " microseconds" << endl;
    cout << "Parallel Multiplication Time: " << par_duration.count() << " microseconds" << endl;

    delete[] A;
    delete[] B;
    delete[] C;

    cudaFree(X);
    cudaFree(Y);
    cudaFree(Z);

    return 0;
}
