
#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include "MPIImpl.hpp"
#include "Attribute.hpp"
#include "Cell.hpp"
#include "CellularSpace.hpp"
#include "Defines.hpp"
using namespace std;

int main(int argc, char **argv){
	int comm_rank;
	int comm_size;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);

	// Make a buffer with our comm_rank in it
	std::vector<int> buffer(1);
	buffer[0] = comm_rank;

	CellularSpace<double> cs = CellularSpace<double>(0, 0, DIMX, DIMY);
	cs.Scatter(MPI_COMM_WORLD);

	if (comm_rank == 0){
		// printf("Hello world!  I am process number: %02d\n",
		// 	buffer[0]);

		for (int i = 1; i < comm_size; i++){
			Receive<int>(buffer, buffer.size(), i, FROM_MASTER);
			// Send<Attribute<char>>(att, i, FROM_MASTER);
			// printf("\tProc %02d reports: %02d\n",
			// 	i, buffer[0]);
		}
	}
	else{
		Send<int>(buffer, buffer.size(), MASTER, FROM_MASTER);
		// Receive<Attribute<char>>(att, MASTER, FROM_MASTER);
		// printf("Hello world!  I am process number: %02d\n",
		// 	buffer[0]);
	}

	MPI_Finalize();

	return 0;
}
