
#include <mpi.h>
#include <stdio.h>
#include <unistd.h>
#include "MPIImpl.hpp"
#include "Attribute.hpp"

#define MASTER 0
#define FROM_MASTER 0
#define FROM_WORKER 1

int main(int argc, char **argv){
	int rank;
	int numtasks;
	int rc;
	char hostname[256];

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	gethostname(hostname, 255);

	// Make a buffer with our rank in it
	std::vector<int> buffer(1);
	buffer[0] = rank;

	std::vector<Attribute<char>> att(1);
	MPI_Datatype MPI_ATTRIBUTE = Make_MPI_Atribute<char>();
	Attribute<char> attrib = Attribute<char>(0, '#');
	att[0] = attrib;

	if (rank == 0){


		printf("Hello world!  I am process number: %02d on host %s\n",
			buffer[0], hostname);

		for (int i = 1; i < numtasks; i++){
			Receive<int>(buffer, i, FROM_MASTER);
			// Send<Attribute<char>>(att, i, FROM_MASTER);
			printf("\tProc %02d reports: %02d\n",
				i, buffer[0]);
		}
	}
	else{
		Send<int>(buffer, MASTER, FROM_MASTER);
		// Receive<Attribute<char>>(att, MASTER, FROM_MASTER);
		printf("Hello world!  I am process number: %02d on host %s\n",
			buffer[0], hostname);
	}

	MPI_Finalize();

	return 0;
}
