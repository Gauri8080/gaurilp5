HPC_4 : Matrix Mulitiplication using CUDA
%%writefile HPC-4.cu
#include <stdio.h>
#include <stdlib.h>
__global__
void add(int* A, int* B, int* C, int size) {
int tid = blockIdx.x * blockDim.x + threadIdx.x;
if (tid < size) {
C[tid] = A[tid] + B[tid];
}
}
__global__
void multiply(int* A, int* B, int* C, int size) {
int row = blockIdx.y * blockDim.y + threadIdx.y;
int col = blockIdx.x * blockDim.x + threadIdx.x;
if (row < size && col < size) {
int sum = 0;
for (int i = 0; i < size; i++) {
sum += A[row * size + i] * B[i * size + col];
}
C[row * size + col] = sum;
}
}
void initializeVector(int* vector, int size) {
for (int i = 0; i < size; i++) {
vector[i] = rand() % 10;
}
}
void initializeMatrix(int* matrix, int size) {
for (int i = 0; i < size * size; i++) {
matrix[i] = rand() % 10;
}
}
void printVector(int* vector, int size) {
for (int i = 0; i < size; i++) {
printf("%d ", vector[i]);
}
printf("\n");
}
void printMatrix(int* matrix, int size) {
for (int row = 0; row < size; row++) {
for (int col = 0; col < size; col++) {
printf("%d ", matrix[row * size + col]);
}
printf("\n");
}
printf("\n");
}
int main() {
int N = 4;
int* A, * B, * C;
int vectorSize = N;
size_t vectorBytes = vectorSize * sizeof(int);
A = (int*)malloc(vectorBytes);
B = (int*)malloc(vectorBytes);
C = (int*)malloc(vectorBytes);
initializeVector(A, vectorSize);
initializeVector(B, vectorSize);
printf("Vector A: ");
printVector(A, N);
printf("Vector B: ");
printVector(B, N);
int* X, * Y, * Z;
cudaMalloc(&X, vectorBytes);
cudaMalloc(&Y, vectorBytes);
cudaMalloc(&Z vectorBytes);
3/17/25, 1:05 PM HPC_4.ipynb - Colab

https://colab.research.google.com/drive/1J-qroaCQCqB2Tu1BHqOBaWlgxuOuhL0p#scrollTo=WWO7f0pvSvQ7&printMode=true 1/3

cudaMalloc(&Z, vectorBytes);
cudaMemcpy(X, A, vectorBytes, cudaMemcpyHostToDevice);
cudaMemcpy(Y, B, vectorBytes, cudaMemcpyHostToDevice);
int threadsPerBlockVec = 256;
int blocksPerGridVec = (N + threadsPerBlockVec - 1) / threadsPerBlockVec;
add<<<blocksPerGridVec, threadsPerBlockVec>>>(X, Y, Z, N);
// Check for errors after the kernel call
cudaError_t error = cudaGetLastError();
if (error != cudaSuccess) {
printf("CUDA error in addition kernel: %s\n", cudaGetErrorString(error));
}
cudaDeviceSynchronize(); // Ensure the kernel has finished executing
cudaMemcpy(C, Z, vectorBytes, cudaMemcpyDeviceToHost);
printf("Addition: ");
printVector(C, N);
free(A);
free(B);
free(C);
cudaFree(X);
cudaFree(Y);
cudaFree(Z);
// Matrix multiplication
int* D, * E, * F;
int matrixSize = N;
size_t matrixBytes = matrixSize * matrixSize * sizeof(int);
D = (int*)malloc(matrixBytes);
E = (int*)malloc(matrixBytes);
F = (int*)malloc(matrixBytes);
initializeMatrix(D, matrixSize);
initializeMatrix(E, matrixSize);
printf("\nMatrix D: \n");
printMatrix(D, matrixSize);
printf("Matrix E: \n");
printMatrix(E, matrixSize);
int* M, * NMat, * O;
cudaMalloc(&M, matrixBytes);
cudaMalloc(&NMat, matrixBytes);
cudaMalloc(&O, matrixBytes);
cudaMemcpy(M, D, matrixBytes, cudaMemcpyHostToDevice);
cudaMemcpy(NMat, E, matrixBytes, cudaMemcpyHostToDevice);
// Use 16x16 thread blocks for better performance
int threadsPerBlockMat = 16;
dim3 threadsMat(threadsPerBlockMat, threadsPerBlockMat);
dim3 blocksMat((matrixSize + threadsPerBlockMat - 1) / threadsPerBlockMat, (matrixSize + threadsPerBlockMat - 1) / threadsPerBlockMat
multiply<<<blocksMat, threadsMat>>>(M, NMat, O, matrixSize);
// Check for errors after the kernel call
error = cudaGetLastError();
if (error != cudaSuccess) {
printf("CUDA error in multiplication kernel: %s\n", cudaGetErrorString(error));
}
cudaDeviceSynchronize(); // Ensure the kernel has finished executing
cudaMemcpy(F, O, matrixBytes, cudaMemcpyDeviceToHost);
printf("Multiplication: \n");
printMatrix(F, matrixSize);
free(D);
free(E);
free(F);
cudaFree(M);
cudaFree(NMat);
cudaFree(O);
return 0;
3/17/25, 1:05 PM HPC_4.ipynb - Colab

https://colab.research.google.com/drive/1J-qroaCQCqB2Tu1BHqOBaWlgxuOuhL0p#scrollTo=WWO7f0pvSvQ7&printMode=true 2/3

}
Writing HPC-4.cu
# Make sure to remove the CUDA symlink if needed
!rm -rf /usr/local/cuda
!ln -s /usr/local/cuda-12.5 /usr/local/cuda # Replace with your CUDA version if necessary
# Compile the CUDA code with the correct architecture for Tesla P4 (sm_61)
!nvcc -arch=sm_75 HPC-4.cu -o HPC-4

import os
print(os.geteuid() == 0)
True
!find / -name "nvcc" 2>/dev/null
/usr/local/cuda-12.5/bin/nvcc
!./HPC-4
Vector A: 3 6 7 5
Vector B: 3 5 6 2
Addition: 6 11 13 7
Matrix D:
9 1 2 7
0 9 3 6
0 6 2 6
1 8 7 9
Matrix E:
2 0 2 3
7 5 9 2
2 8 9 7
3 6 1 2
Multiplication:
50 63 52 57
87 105 114 51
64 82 78 38
99 150 146 86

!nvcc --version
nvcc: NVIDIA (R) Cuda compiler driver
Copyright (c) 2005-2024 NVIDIA Corporation
Built on Thu_Jun__6_02:18:23_PDT_2024
Cuda compilation tools, release 12.5, V12.5.82
Build cuda_12.5.r12.5/compiler.34385749_0
!nvidia-smi --query-gpu=compute_cap --format=csv,noheader
