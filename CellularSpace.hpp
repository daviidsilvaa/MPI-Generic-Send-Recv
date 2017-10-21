
#ifndef CELLULARSPACE_HPP
#define CELLULARSPACE_HPP

#include "mpi.h"
#include "Cell.hpp"

template<class T>
class CellularSpace{
public:
    int width;
    int height;
    Cell<T> *memoria;

    static CellularSpace* create(const int &width, const int &height){
        CellularSpace<T> *cs = new CellularSpace<T>;
        cs->width = width;
        cs->height = height;
        cs->memoria = new Cell<T>[width*height];

        return cs;
    }

    void Scatter(const int &num_workers, const int &rank_id, MPI_Status &mpi_status){
        if((this->width*this->height)%num_workers != 0){
            throw std::runtime_error("CellularSpace::Scatter failed");
            return 0;
        }

        int rows, offset;

        if(rank_id == 0){
            std::vector<T> vec(this->width*this->height);
            for(int i = 0; i < this->width*this->height; i++)
                vec[i] = this->memoria[i].value;

            rows = (this->width*this->height)/num_workers;
            offset = 0;

            for(int dest = 1; dest <= num_workers; dest++){
                MPI_Send(&offset, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
                MPI_Send(&rows, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
                Send(&vec[offset], rows, ConvertType(getAbstractionDataType<T>()), dest, 0);
                offset = offset + rows;
            }
        }
        if(rank_id != 0){
            int src = 0;

            MPI_Recv(&offset, 1, MPI_INT, src, 0, MPI_COMM_WORLD, &mpi_status);
            MPI_Recv(&rows, 1, MPI_INT, src, 0, MPI_COMM_WORLD, &mpi_status);

            std::vector<T> vec(rows);

            Receive(&vec, rows, ConvertType(getAbstractionDataType<T>()), src, 0);
        }

    }

    ~CellularSpace(){
        delete memoria;
    }

    void SetCell(Cell<T> cell, const int &i, const int &j){
        this->memoria[i*this->width + j] = cell;
    }

    Cell<T> GetCell(const int &i, const int &j){
		return memoria[i*this->width + j];
	}
};

#endif
