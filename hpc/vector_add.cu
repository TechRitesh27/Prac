// File: vector_add.cu

#include <stdio.h>
#include <cuda.h>

// Kernel function (runs on GPU)
__global__ void vectorAdd(int *A, int *B, int *C, int n)
{
    /*
    blockIdx.x → block number
    blockDim.x → threads per block
    threadIdx.x → thread ID inside block
    */
    int i = blockIdx.x * blockDim.x + threadIdx.x; // This line gives each thread a unique index i

    if (i < n)
    {
        C[i] = A[i] + B[i];
    }
}

int main()
{
    int n;

    printf("Enter size of vectors: ");
    scanf("%d", &n);

    int size = n * sizeof(int);

    // Host memory
    int *h_A = (int *)malloc(size);
    int *h_B = (int *)malloc(size);
    int *h_C = (int *)malloc(size);

    // Input
    printf("Enter elements of vector A:\n");
    for (int i = 0; i < n; i++)
        scanf("%d", &h_A[i]);

    printf("Enter elements of vector B:\n");
    for (int i = 0; i < n; i++)
        scanf("%d", &h_B[i]);

    // Device memory
    int *d_A, *d_B, *d_C;
    cudaMalloc((void **)&d_A, size);
    cudaMalloc((void **)&d_B, size);
    cudaMalloc((void **)&d_C, size);

    // Copy data to GPU
    cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);

    // Launch kernel
    int threads = 256;
    int blocks = (n + threads - 1) / threads;

    vectorAdd<<<blocks, threads>>>(d_A, d_B, d_C, n);

    // Copy result back
    cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);
    // cudaMemcpy(destination, source, size, direction)

    // Output
    printf("\nResult Vector:\n");
    for (int i = 0; i < n; i++)
        printf("%d ", h_C[i]);

    printf("\n");

    // Free memory
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);
    free(h_A);
    free(h_B);
    free(h_C);

    return 0;
}