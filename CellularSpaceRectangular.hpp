
#ifndef CELLULARSPACERECTANGULAR_HPP
#define CELLULARSPACERECTANGULAR_HPP

#include "mpi.h"
#include "Cell.hpp"
#include "DefinesRectangular.hpp"
#include <iostream>
using namespace std;

template<class T>
class CellularSpaceRectangular{
public:
    int x_init;
    int y_init;
    int width;
    int height;
    Cell<T> memoria[PROC_DIMX_REC*PROC_DIMY_REC];

    CellularSpaceRectangular(){ }

    CellularSpaceRectangular(const int &height, const int &width){
        this->x_init = 0;
        this->y_init = 0;
        this->width = width;
        this->height = height;
    }

    CellularSpaceRectangular(const int &x_init, const int &y_init, const int &height, const int &width){
        this->x_init = x_init;
        this->y_init = y_init;
        this->width = width;
        this->height = height;
    }

    void Scatter(const MPI_Comm &mpi_comm){
        int comm_size, comm_rank, comm_workers, count, offset;
        MPI_Comm_size(mpi_comm, &comm_size);
        MPI_Comm_rank(mpi_comm, &comm_rank);
        MPI_Status mpi_status;
        comm_workers = comm_size - 1;

        if(comm_rank == 0){
            char word_cellular_space[23];
            count = (this->height*this->width)/comm_workers;
            offset = 0;
            int x_init_s, y_init_s, height_s, width_s;

            for(int dest = 1; dest <= comm_size-1; dest++){
                x_init_s = offset/this->width;
                y_init_s = this->y_init;
                height_s = this->height/comm_workers;
                width_s = this->width;
                offset = offset + count;
                MPI_Send(&x_init_s, 1, MPI_INT, dest, FROM_MASTER, MPI_COMM_WORLD);
                MPI_Send(&y_init_s, 1, MPI_INT, dest, FROM_MASTER, MPI_COMM_WORLD);
                MPI_Send(&height_s, 1, MPI_INT, dest, FROM_MASTER, MPI_COMM_WORLD);
                MPI_Send(&width_s, 1, MPI_INT, dest, FROM_MASTER, MPI_COMM_WORLD);
            }
        }
        if(comm_rank != 0){
            char word_cellular_space[23];
            int x_init_s, y_init_s, height_s, width_s;
            MPI_Recv(&x_init_s, 1, MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &mpi_status);
            MPI_Recv(&y_init_s, 1, MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &mpi_status);
            MPI_Recv(&height_s, 1, MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &mpi_status);
            MPI_Recv(&width_s, 1, MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &mpi_status);

            CellularSpace<T> cs = CellularSpace<T>(x_init_s, y_init_s, height_s, width_s);
            MPI_Aint aint;
            int address = MPI_Address(&cs, &aint);
            // cout << comm_rank << "\t" << aint << endl;
            for(int i = 0; i < (cs.height * cs.width); i++){
                cs.memoria[i] = Cell<T>((i + cs.x_init*cs.width), (i%(cs.height * cs.width)),
                    Attribute<T>((i + cs.x_init*cs.width), 0));
                cs.memoria[i] = cs.memoria[i].SetNeighbor();
            }
        }
    }
};

#endif
