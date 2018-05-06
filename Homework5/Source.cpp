#include<mpi.h>
#include<iostream>
#include<time.h>
#include<Windows.h>
using namespace std;

int main(int argc, char** argv)
{
	int size, processId, rc, toSend[2], toReceive[2], leaderId;
	MPI_Status status;
	MPI_Request request;

	rc = MPI_Init(&argc, &argv);
	if (rc != MPI_SUCCESS)
	{
		cout << "Error starting MPI program. Terminating.\n";
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &processId); 

	leaderId = processId;

	srand(processId*time(NULL));

	int biggestPriority = rand() % 100;
	cout << "I am process " << processId << " and I have priority " << biggestPriority << "\n";
	if (processId == 0)
	{
		toSend[0] = leaderId;
		toSend[1] = biggestPriority;
		MPI_Isend(&toSend, 2, MPI_INT, (processId + 1) % size, 0, MPI_COMM_WORLD, &request);
		MPI_Recv(&toReceive, 2, MPI_INT, (size + processId - 1) % size, 0, MPI_COMM_WORLD, &status);
		leaderId = toReceive[0];
		biggestPriority = toReceive[1];
		toSend[0] = leaderId;
		toSend[1] = biggestPriority;
		MPI_Isend(&toSend, 2, MPI_INT, (processId + 1) % size, 0, MPI_COMM_WORLD, &request);
	}
	else
	{
		MPI_Recv(&toReceive, 2, MPI_INT, (size + processId - 1) % size, 0, MPI_COMM_WORLD, &status);
		if (toReceive[1] > biggestPriority)
		{
			leaderId = toReceive[0];
			biggestPriority = toReceive[1];
		}
		else
		{
			if (toReceive[1] == biggestPriority)
			{
				if (toReceive[0] > processId)
				{
					leaderId = toReceive[0];
					biggestPriority = toReceive[1];
				}
			}
		}
		toSend[0] = leaderId;
		toSend[1] = biggestPriority;
		MPI_Isend(&toSend, 2, MPI_INT, (processId + 1) % size, 0, MPI_COMM_WORLD, &request);
		MPI_Recv(&toReceive, 2, MPI_INT, (size + processId - 1) % size, 0, MPI_COMM_WORLD, &status);
		leaderId = toReceive[0];
		biggestPriority = toReceive[1];
		toSend[0] = leaderId;
		toSend[1] = biggestPriority;
		MPI_Isend(&toSend, 2, MPI_INT, (processId + 1) % size, 0, MPI_COMM_WORLD, &request);
	}
	Sleep(1000);
	cout << "I'm process " << processId << " and my leader is " << leaderId << " with priority " << biggestPriority << "\n";
	MPI_Finalize();
	return 0;
}


I am process 3 and I have priority 80
I'm process 3 and my leader is 3 with priority 80
I am process 0 and I have priority 38
I'm process 0 and my leader is 3 with priority 80
I am process 1 and I have priority 75
I'm process 1 and my leader is 3 with priority 80
I am process 2 and I have priority 44
I'm process 2 and my leader is 3 with priority 80
I am process 4 and I have priority 49
I'm process 4 and my leader is 3 with priority 80
