
#ifndef MODEL_HPP
#define MODEL_HPP

#include "CellularSpace.hpp"
#include "Flow.hpp"
#include <iostream>
using namespace std;

template<class T>
class Model{
public:
    T flow;
    double time;
    double time_step;

    Model(){ }

    Model(const T &flow, const double &time, const double &time_step){
        this->flow = flow;
        this->time = time;
        this->time_step = time_step;
    }

    Model(const Model<T> &model){
        this->flow = model.flow;
        this->time = model.time;
        this->time_step = model.time_step;
    }

    Model<T>& operator=(const Model<T> &model){
        if(this != &model){
            this->flow = model.flow;
            this->time = model.time;
            this->time_step = model.time_step;
        }
        return *this;
    }

    ~Model(){ }

    template<class R>
    void execute(const MPI_Comm &mpi_comm, const CellularSpace<R> &cellular_space){
        int comm_size, comm_rank, comm_workers, count, offset;
        MPI_Comm_size(mpi_comm, &comm_size);
        MPI_Comm_rank(mpi_comm, &comm_rank);
        MPI_Status mpi_status;
        comm_workers = comm_size - 1;

        if(comm_rank == 0){
            count = (cellular_space.height*cellular_space.width)/comm_workers;
            offset = 0;
            int x_init_s, y_init_s, height_s, width_s;
            int index[comm_size];

            for(int dest = 1; dest <= comm_size-1; dest++){
                x_init_s = offset/cellular_space.width;
                y_init_s = cellular_space.y_init;
                height_s = cellular_space.height/comm_workers;
                width_s = cellular_space.width;
                offset = offset + count;
                MPI_Send(&x_init_s, 1, MPI_INT, dest, FROM_MASTER, MPI_COMM_WORLD);
                MPI_Send(&y_init_s, 1, MPI_INT, dest, FROM_MASTER, MPI_COMM_WORLD);
                MPI_Send(&height_s, 1, MPI_INT, dest, FROM_MASTER, MPI_COMM_WORLD);
                MPI_Send(&width_s, 1, MPI_INT, dest, FROM_MASTER, MPI_COMM_WORLD);
                index[dest]= offset;
            }
        }
        if(comm_rank != 0){
            char word_cellular_space[23];
            int x_init_s, y_init_s, height_s, width_s;
            MPI_Recv(&x_init_s, 1, MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &mpi_status);
            MPI_Recv(&y_init_s, 1, MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &mpi_status);
            MPI_Recv(&height_s, 1, MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &mpi_status);
            MPI_Recv(&width_s, 1, MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &mpi_status);

            CellularSpace<R> cs = CellularSpace<R>(x_init_s, y_init_s, height_s, width_s);
            MPI_Aint aint;
            int address = MPI_Address(&cs, &aint);

            for(int i = 0; i < (cs.height * cs.width); i++){
                cs.memoria[i] = Cell<R>
                    ((i + cs.x_init*cs.width), (i%(cs.height * cs.width)), Attribute<R>((i + cs.x_init*cs.width), 10));
                cs.memoria[i] = cs.memoria[i].SetNeighbor();
            }

            R last_executes[cs.height * cs.width];
            int x_, y_;
            for(double t = 0; t < this->time; t = t + this->time_step){

                // Calcule flow and save they
                for(int i = 0; i < (cs.height * cs.width); i++){
                    this->flow.source = cs.memoria[i];
                    last_executes[i] = this->flow.execute();
                }

                for(int i = 0; i < (cs.height * cs.width); i++){
                    for(int j = 0; j < cs.memoria[i].count_neighbors; j++){

                    }
                }

            }
        }
    }

    // missing implement
    double execute(){
        for(double t = 0; t < this->time; t = t + this->time_step){
            this-> time = this-> time*1;
        }
    }
};

#endif
