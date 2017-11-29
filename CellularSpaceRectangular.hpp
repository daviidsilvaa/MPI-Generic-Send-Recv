
#ifndef CELLULARSPACE_HPP
#define CELLULARSPACE_HPP

#include "mpi.h"
#include "Cell.hpp"
#include "Defines.hpp"
#include "DefinesRectangular.hpp"
#include <iostream>
using namespace std;

template<class T>
class CellularSpace{
public:
    int x_init;
    int y_init;
    int width;
    int height;
    Cell<T> memoria[PROC_DIMX_REC*PROC_DIMY_REC];

    CellularSpace(){ }

    CellularSpace(const int &height, const int &width){
        this->x_init = 0;
        this->y_init = 0;
        this->width = width;
        this->height = height;
    }

    CellularSpace(const int &x_init, const int &y_init, const int &height, const int &width){
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

// void SetCell(Cell<T> cell, const int &i, const int &j){
//     this->memoria[i*this->height + j] = cell;
// }
//
// Cell<T> GetCell(const int &i, const int &j){
// 	return memoria[i*this->height + j];
// }

// static CellularSpace* create(const int &width, const int &height, const MPI_Comm &mpi_comm){
//     CellularSpace<T> *cs = new CellularSpace<T>;
//     cs->width = width;
//     cs->height = height;
//     cs->memoria = new Cell<T>[width*height];
//     cs->mpi_comm = mpi_comm;
//
//     return cs;
// }
// static CellularSpace* create(const int &width, const int &height){
//     CellularSpace<T> *cs = new CellularSpace<T>;
//     cs->width = width;
//     cs->height = height;
//     cs->memoria = new Cell<T>[height];
//
//     return cs;
// }

// void Scatter(){
//     MPI_Status mpi_status;
//     int comm_size, comm_rank;
//
//     MPI_Comm_size(this->mpi_comm, &comm_size);
//     if((this->width*this->height)%comm_size != 0){
//         throw std::runtime_error("CellularSpace::Scatter failed");
//     }
//     MPI_Comm_rank(this->mpi_comm, &comm_rank);
//
//     int rows, offset;
//     if(comm_rank == 0){
//         std::vector<T> vec(this->width*this->height);
//         for(int i = 0; i < this->width*this->height; i++)
//             vec[i] = this->memoria[i].attribute.value;
//
//         rows = (this->width*this->height)/comm_size;
//         offset = 0;
//
//         for(int dest = 1; dest <= comm_size-1; dest++){
//             MPI_Send(&offset, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
//             MPI_Send(&rows, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
//             // MPI_Send(rows, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
//             Send<T>(vec, rows, dest, 0);
//
//             offset = offset + rows;
//         }
//     }
//     if(comm_rank != 0){
//         int src = 0;
//
//         MPI_Recv(&offset, 1, MPI_INT, src, 0, MPI_COMM_WORLD, &mpi_status);
//         MPI_Recv(&rows, 1, MPI_INT, src, 0, MPI_COMM_WORLD, &mpi_status);
//
//         std::vector<T> vec(rows);
//         CellularSpace<T> *cs_reduced = CellularSpace<T>::create(offset, rows);
//
//         MPI_Recv(&rows, 1, MPI_INT, src, 0, MPI_COMM_WORLD, &mpi_status);
//         // Receive(&vec, rows, src, 0);
//     }
//
// }
//
// ~CellularSpace(){
//     delete memoria;
// }

// template<class T>
// MPI_Datatype Make_MPI_CellularSpace(){
//     MPI_Datatype MPI_Attribute = Make_MPI_Atribute<T>();
//     MPI_Datatype MPI_Cell = Make_MPI_Cell(&MPI_Attribute);
//
//     const int nitens = 4;
//
// }
//
// template<class T>
// MPI_Datatype Make_MPI_Atribute(){
//     MPI_Datatype MPI_Attribute;
//
//     const int nitens = 2;
//     MPI_Datatype blocktypes[2] = {MPI_INT, ConvertType(getAbstractionDataType<T>())};
//     int blocklengths[2] = {1, 1};
//     MPI_Aint offsets[2] = {offsetof(Attribute<T>, key), offsetof(Attribute<T>, value)};
//
//     MPI_Type_create_struct(nitens, blocklengths, offsets, blocktypes, &MPI_Attribute);
//     MPI_Type_commit(&MPI_Attribute);
//
//     return MPI_Attribute;
// }
