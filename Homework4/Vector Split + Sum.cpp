#include<mpi.h>
#include<iostream>
using namespace std;

int rc, processID, commSize;
int sum = 0, temp;
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

	if (processID == 0)
	{
		int v[100] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 },toSend[100],totalSum=0;
		int length = 14;
		for (int i = 1; i < commSize; i++)
		{
			int sendSize = 0;
			for (int j = i; j <= 13; j += commSize)
			{
				toSend[sendSize] = v[j];
				sendSize++;
			}
			MPI_Send(&sendSize, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
			MPI_Send(&toSend, sendSize, MPI_INT, i, 1, MPI_COMM_WORLD);
		}
		for (int j = 0; j <= 13; j += commSize)
		{
			cout << "I'm process " << processID << " and I'm adding " << v[j] << '\n';
			totalSum += v[j];
		}
		for (int i = 1; i < commSize; i++)
		{
			MPI_Recv(&temp, 1, MPI_INT, i, 1, MPI_COMM_WORLD, &status);
			totalSum += temp;
		}
		cout << "Suma este " << totalSum << "\n";
	}
	else
	{
		int sz, vec[100],sum=0;
		MPI_Recv(&sz, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&vec, sz, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		for (int i = 0; i < sz; i ++)
		{
			cout << "I'm process " << processID << " and I'm adding " << vec[i] << '\n';
			sum += vec[i];
		}
		MPI_Send(&sum, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
	}

	MPI_Finalize();
	return 0;
}
