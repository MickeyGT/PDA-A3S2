#include<mpi.h>
#include<iostream>
#include<time.h>
#include<Windows.h>
#define N 5
using namespace std;

int graf[N][N] =
{
	0,3,9,8,3,
	5,0,1,4,2,
	6,6,0,4,5,
	2,9,2,0,7,
	7,9,3,2,0
};

__global__ void compareAndReplace(int graf[N][N])
{
	int i = threadIdx.x;
	int k = threadIdx.y;
	for (int j = 0; j < N; j++)
	{
		if (graf[i][k] + graf[k][j] < graf[i][j])
			graf[i][j] = graf[i][k] + graf[k][j];
	}
}

int main(int argc, char** argv)
{
	int* gr;
	int sz = N * N * sizeof(int);
	cudaMalloc(&gr, sz);
	cudaMemcpy(gr, graf, size, cudaMemcpyHostToDevice);
	dim3 threadsPerBlock(N, N);
	compareAndReplace <<<numBlocks, threadsPerBlock >>>(A, B, C);
	cudaMemcpy(graf, gr, size, cudaMemcpyDeviceToHost);
	cudaFree(gr);
	for (int i = 0; k < N; k++)
	{
		for (int j = 0; j < N; j++)
		{
			cout << graf[i][j] << " ";
		}
		cout << '\n';
	}
	return 0;
}
