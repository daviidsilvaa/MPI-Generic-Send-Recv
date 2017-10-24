
#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include "MPIImpl.hpp"
#include "Attribute.hpp"
#include "Cell.hpp"
#include "CellularSpace.hpp"
#include "Flow.hpp"
#include "Defines.hpp"
using namespace std;

template<>
double Flow<double>::execute(){
	return 0.1;
}

int main(int argc, char **argv){
	int comm_rank;
	int comm_size;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);

	// Make a buffer with our comm_rank in it
	std::vector<int> buffer(1);
	buffer[0] = comm_rank;

	CellularSpace<double> cs = CellularSpace<double>(DIMX, DIMY);
	cs.Scatter(MPI_COMM_WORLD);

	if (comm_rank == 0){
		for (int i = 1; i < comm_size; i++){
			Receive<int>(buffer, buffer.size(), i, FROM_MASTER);
		}
	}
	else{
		Send<int>(buffer, buffer.size(), MASTER, FROM_MASTER);
	}

	MPI_Finalize();

	return 0;
}
