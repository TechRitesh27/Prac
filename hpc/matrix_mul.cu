// File: matrix_mul.cu

#include <stdio.h>
#include <cuda.h>

// Kernel function
__global__ void matrixMul(int *A, int *B, int *C, int N) {

    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < N && col < N) {
        int sum = 0;

        for (int k = 0; k < N; k++) {
            sum += A[row * N + k] * B[k * N + col];
        }

        C[row * N + col] = sum;
    }
}

int main() {
    int N;

    printf("Enter size of square matrix (N x N): ");
    scanf("%d", &N);

    int size = N * N * sizeof(int);

    // Host memory
    int *h_A = (int*)malloc(size);
    int *h_B = (int*)malloc(size);
    int *h_C = (int*)malloc(size);

    // Input matrices
    printf("Enter elements of Matrix A:\n");
    for(int i = 0; i < N*N; i++)
        scanf("%d", &h_A[i]);

    printf("Enter elements of Matrix B:\n");
    for(int i = 0; i < N*N; i++)
        scanf("%d", &h_B[i]);

    // Device memory
    int *d_A, *d_B, *d_C;
    cudaMalloc((void**)&d_A, size);
    cudaMalloc((void**)&d_B, size);
    cudaMalloc((void**)&d_C, size);

    // Copy to GPU
    cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);

    // Kernel launch config (2D)
    dim3 threads(16, 16);
    dim3 blocks((N + 15)/16, (N + 15)/16);

    matrixMul<<<blocks, threads>>>(d_A, d_B, d_C, N);

    // Copy result back
    cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);

    // Output result
    printf("\nResult Matrix:\n");
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            printf("%d ", h_C[i * N + j]);
        }
        printf("\n");
    }

    // Free memory
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);
    free(h_A);
    free(h_B);
    free(h_C);

    return 0;
}


/*

nvcc vector_add.cu -o vector_add
./vector_add

nvcc matrix_mul.cu -o matrix_mul
./matrix_mul

Vector Addition

Enter size of vectors: 4
Enter elements of vector A:
1 2 3 4
Enter elements of vector B:
5 6 7 8

Result Vector:
6 8 10 12

Matrix Multiplication

Enter size of square matrix (N x N): 2
Enter elements of Matrix A:
1 2
3 4
Enter elements of Matrix B:
5 6
7 8

Result Matrix:
19 22
43 50
*/