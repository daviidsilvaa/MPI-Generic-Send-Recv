
#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include "MPIImpl.hpp"
#include "Attribute.hpp"
#include "Cell.hpp"
#include "CellularSpace.hpp"
#include "Exponencial.hpp"
#include "Model.hpp"
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

	// cs.Scatter(MPI_COMM_WORLD);

	CellularSpace<double> cs = CellularSpace<double>(DIMX, DIMY);
	Model<Exponencial<double>> m1 =
		Model<Exponencial<double>>(Exponencial<double>(Cell<double>(19, 3, Attribute<double>(99, 2.2)), 0.1), 10.0, 0.2);

	m1.execute<double>(MPI_COMM_WORLD, cs);

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
