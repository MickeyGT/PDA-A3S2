#include<mpi.h>
#include<iostream>
using namespace std;

int v[100] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};

int rc, processID, commSize;
int sum = 0,temp;
MPI_Status status;

int main(int argc, char** argv)
{
	rc = MPI_Init(&argc, &argv);
	if (rc != MPI_SUCCESS) 
	{
		cout << "Error starting MPI program. Terminating.\n";
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	MPI_Comm_size(MPI_COMM_WORLD, &commSize);
	MPI_Comm_rank(MPI_COMM_WORLD, &processID);

	if (processID != 0)
	{
		for (int i = processID - 1; i < 14; i += 3)
		{
			sum += v[i];
		}
		MPI_Send(&sum, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
	}
	else
	{
		for (int i = 1; i <= 3; i++)
		{
			MPI_Recv(&temp, 1, MPI_INT, i, 1, MPI_COMM_WORLD, &status);
			sum += temp;
		}
		cout << "Suma este " << sum << "\n";
	}

	MPI_Finalize();
	return 0;
}
