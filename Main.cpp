
#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include "MPIImpl.hpp"
#include "Attribute.hpp"
#include "Cell.hpp"
#include "CellularSpace.hpp"
using namespace std;

#define MASTER 0
#define FROM_MASTER 0
#define FROM_WORKER 1
#define TAM 3

int main(int argc, char **argv){
	int rank_id;
	int comm_size;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank_id);

	// Make a buffer with our rank_id in it
	std::vector<int> buffer(1);
	buffer[0] = rank_id;

	std::vector<Attribute<char>> att(1);
	MPI_Datatype MPI_ATTRIBUTE = Make_MPI_Atribute<char>();
	Attribute<char> attrib = Attribute<char>(0, '#');
	att[0] = attrib;

	if (rank_id == 0){
		printf("Hello world!  I am process number: %02d on host\n",
			buffer[0]);

		for (int i = 1; i < comm_size; i++){
			Receive<int>(buffer, buffer.size(), i, FROM_MASTER);
			// Send<Attribute<char>>(att, i, FROM_MASTER);
			printf("\tProc %02d reports: %02d\n",
				i, buffer[0]);
		}
	}
	else{
		Send<int>(buffer, buffer.size(), MASTER, FROM_MASTER);
		// Receive<Attribute<char>>(att, MASTER, FROM_MASTER);
		printf("Hello world!  I am process number: %02d on host\n",
			buffer[0]);
	}
	CellularSpace<double> *cs = CellularSpace<double>::create(TAM, TAM, MPI_COMM_WORLD);
	cs->Scatter();
	
	MPI_Finalize();

	return 0;
}
