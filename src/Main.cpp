
#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include "MPIImpl.hpp"
#include "Attribute.hpp"
#include "Cell.hpp"
#include "CellularSpace.hpp"
#include "CellularSpaceRectangular.hpp"
#include "Exponencial.hpp"
#include "Model.hpp"
#include "ModelRectangular.hpp"
#include "Defines.hpp"
#include "DefinesRectangular.hpp"
using namespace std;

int main(int argc, char *argv[]){
	int comm_rank;
	int comm_size;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);

	CellularSpace<double> cs1 = CellularSpace<double>(DIMX, DIMY);

	if(comm_rank == 0)
		cout << cs1.height << "\t" << cs1.width << endl;
	if(comm_rank == 0)
		cout << "\t(Model)m1.execute()" << endl;

	Model<Exponencial<double> > m1 =
		Model<Exponencial<double> >(Exponencial<double>(Cell<double>(19, 3, Attribute<double>(99, 2.2)), 0.1), 10.0, 0.2);

	m1.execute<double>(MPI_COMM_WORLD, cs1);

	// CellularSpaceRectangular<double> cs2 = CellularSpaceRectangular<double>(PROC_DIMX_REC, PROC_DIMY_REC);
	//
	// if(comm_rank == 0)
	// 	cout << cs2.height << "\t" << cs2.width << endl;
	// if(comm_rank == 0)
	// 	cout << "\t(ModelRectangular)m2.execute()" << endl;
	//
	// ModelRectangular<Exponencial<double> > m2 =
	// 	ModelRectangular<Exponencial<double> >(Exponencial<double>(Cell<double>(18, 19, Attribute<double>(99, 2.2)), 0.1), 10.0, 0.2);
	//
	// m2.execute<double>(MPI_COMM_WORLD, cs2);

	MPI_Finalize();

	return 0;
}

// Make a buffer with our comm_rank in it
// vector<int> buffer(1);
// buffer[0] = comm_rank;
//
// if (comm_rank == 0){
// 	for (int i = 1; i < comm_size; i++){
// 		Receive<int>(buffer, buffer.size(), i, FROM_MASTER);
// 	}
// }
// else{
// 	Send<int>(buffer, buffer.size(), MASTER, FROM_MASTER);
//}
