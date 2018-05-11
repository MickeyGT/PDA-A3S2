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

int main(int argc, char** argv)
{
	int size, rank, rc;

	rc = MPI_Init(&argc, &argv);
	if (rc != MPI_SUCCESS)
	{
		cout << "Error starting MPI program. Terminating.\n";
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	for (int k = 0; k < N; k++)
	{
		for (int j = 0; j < N; j++)
		{
			if (graf[rank][k] + graf[k][j] < graf[rank][j])
				graf[rank][j] = graf[rank][k] + graf[k][j];
		}
		MPI_Allgather(graf[rank], N, MPI_INT, graf, N, MPI_INT, MPI_COMM_WORLD);		
	}
	MPI_Barrier(MPI_COMM_WORLD);
	if (rank == 0)
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
				cout << graf[i][j] << " ";
			cout << '\n';
		}

	MPI_Finalize();
	return 0;
}
