
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

    // missing implement
    double execute(){
        for(double t = 0; t < this->time; t = t + this->time_step){
            this->flow.execute();
        }
    }

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
            char word_cs_send[23];

            // para cada maquina { crie uma regiao (i.e.linhas) do espaço celular }
            for(int dest = 1; dest <= comm_size-1; dest++){
                sprintf(word_cs_send, "%d|%d:%d|%d",
                    offset/cellular_space.width, cellular_space.y_init, cellular_space.height/comm_workers, cellular_space.width);
                MPI_Send(word_cs_send, 23, MPI_CHAR, dest, FROM_MASTER, MPI_COMM_WORLD);
                offset = offset + count;
                index[dest]= offset;
            }

            // Request Flow::execute() from process
            char word_execute_send[23];
            int dest_ = (this->flow.source.x/(cellular_space.height/comm_workers)) + 1;
            sprintf(word_execute_send, "%d|%d:%d|%lf", dest_, this->flow.source.x, this->flow.source.y, this->flow.flow_rate);
            cout << word_execute_send << endl;

            // cout << word_execute_send << endl;

            for(int dest = 1; dest <= comm_size-1; dest++){
                MPI_Send(word_execute_send, 23, MPI_CHAR, dest, 999, mpi_comm);
            }

        }
        if(comm_rank != 0){
            char word_cs_recv[23], word_execute_recv[23];

            MPI_Recv(word_cs_recv, 23, MPI_CHAR, MASTER, FROM_MASTER, MPI_COMM_WORLD, &mpi_status);
            char *x_init_c = strtok(word_cs_recv, "|");
            char *y_init_c = strtok(NULL, ":");
            char *height_c = strtok(NULL, "|");
            char *width_c = strtok(NULL, ":");
            int x_init_s = atoi(x_init_c);
            int y_init_s = atoi(y_init_c);
            int height_s = atoi(height_c);
            int width_s = atoi(width_c);


            CellularSpace<R> cs = CellularSpace<R>(x_init_s, y_init_s, height_s, width_s);
            MPI_Aint aint;
            int address = MPI_Address(&cs, &aint);

            // Initializing the neighbors of the cellular space created in each process
            for(int i = 0; i < (cs.height * cs.width); i++){
                cs.memoria[i] = Cell<R>((cs.x_init + (i/cs.width)), (i%cs.width), Attribute<R>(i, 1));
                cs.memoria[i] = cs.memoria[i].SetNeighbor();
            }

            MPI_Recv(word_execute_recv, 23, MPI_CHAR, MASTER, 999, mpi_comm, &mpi_status);
            char *rank_c = strtok(word_execute_recv, "|:");
            char *x_c = strtok(NULL, ":");
            char *y_c = strtok(NULL, "|");
            char *flow_rate_c = strtok(NULL, "|:");
            int rank_ = atoi(rank_c);
            int x_ = atoi(x_c);
            int y_ = atoi(y_c);
            int flow_rate_ = atoi(flow_rate_c);

            if(comm_rank == rank_){
                cout << cs.memoria[(x_%cs.width)*cs.height + y_].x << " " << cs.memoria[(x_%cs.width)*cs.height + y_].y
                    << " " << cs.memoria[(x_%cs.width)*cs.height + y_].count_neighbors << endl;
            }

            for(double t = 0; t < this->time; t = t + this->time_step){
                // cout << comm_rank << ": " << this->flow.execute() << endl;
                this->flow.last_execute = this->flow.execute();
            }
        }
    }
};

#endif
