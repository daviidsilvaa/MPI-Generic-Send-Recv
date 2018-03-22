
#ifndef MODELRECTANGULAR_HPP
#define MODELRECTANGULAR_HPP

#include "CellularSpaceRectangular.hpp"
#include "Flow.hpp"
#include "MPIImpl.hpp"
#include "assert.h"
#include <iostream>
#include <fstream>
using namespace std;

template<class T>
class ModelRectangular{
public:
    T flow;
    double time;
    double time_step;

    ModelRectangular(){ }

    ModelRectangular(const T &flow, const double &time, const double &time_step){
        this->flow = flow;
        this->time = time;
        this->time_step = time_step;
    }

    ModelRectangular(const ModelRectangular<T> &model){
        this->flow = model.flow;
        this->time = model.time;
        this->time_step = model.time_step;
    }

    ModelRectangular<T>& operator=(const ModelRectangular<T> &model){
        if(this != &model){
            this->flow = model.flow;
            this->time = model.time;
            this->time_step = model.time_step;
        }
        return *this;
    }

    ~ModelRectangular(){ }

    // missing implement
    double execute(){
        for(double t = 0; t < this->time; t = t + this->time_step){
            this->flow.execute();
        }
    }

    template<class R>
    void execute(const MPI_Comm &mpi_comm, const CellularSpaceRectangular<R> &cellular_space){
        int comm_size, comm_rank, comm_workers, offset_x, offset_y;
        MPI_Comm_size(mpi_comm, &comm_size);
        MPI_Comm_rank(mpi_comm, &comm_rank);
        MPI_Status mpi_status, mpi_status_consumer;
        MPI_Request mpi_consumer_request, mpi_recv_request;
        comm_workers = comm_size - 1;

        // maquina master
        if(comm_rank == 0){
            offset_x = offset_y = 0;
            int x_init_s, y_init_s, height_s, width_s;
            int index[comm_size];
            char word_cs_send[23];

            // para cada maquina { crie uma regiao (i.e. retangulos) do espaço celular }
            for(int dest = 1; dest <= comm_workers; dest++){
                sprintf(word_cs_send, "%d|%d:%d|%d",
                    offset_x, offset_y, PROC_DIMX_REC, PROC_DIMY_REC);
                MPI_Send(word_cs_send, 23, MPI_CHAR, dest, FROM_MASTER, MPI_COMM_WORLD);

                offset_y = offset_y + PROC_DIMY_REC;
                // verifica se a proxima linha de retangulos sera passada para sua maquina correspondente
                if(offset_y == DIMY_REC){
                    offset_x = offset_x + PROC_DIMX_REC;
                    offset_y = 0;
                }
            }

            // envia os dados necessarios para que Flow::execute() seja feito nos demais processos
            char word_execute_send[23];
            int dest_ = ((this->flow.source.x/LINES_REC)/(cellular_space.height/this->flow.source.y)) + 1;
            sprintf(word_execute_send, "%d|%d:%d|%lf", dest_, this->flow.source.x, this->flow.source.y, this->flow.flow_rate);
            cout << word_execute_send << " " << dest_ << endl;

            for(int dest = 1; dest <= comm_workers; dest++){
                MPI_Send(word_execute_send, 23, MPI_CHAR, dest, 999, mpi_comm);
            }
            //
            // // recebe a soma dos cell.attribute.value presente em cada maquina
            // R temp, acumulated_value_recv = 0;
            // for(int source = 1; source <= comm_workers; source++){
            //     MPI_Irecv(&temp, 1, ConvertType(getAbstractionDataType<R>()), source, source, MPI_COMM_WORLD, &mpi_recv_request);
            //     acumulated_value_recv += temp;
            // }
            //
            // // verifica se os valores simulados foram conservados
            // assert((acumulated_value_recv - 10000) < 0.001);
            //
            // // no final da execucao, a maquina master computa os dados gerados e unifica-o em uma mesma estrutura de dados(ie, aquivo .txt)
            // char file_output_name_recv[20];
            // string line_;
            // fstream file_output, file_output_recv;
            //
            // for(int source = 1; source <= comm_workers; source++){
            //     MPI_Irecv(file_output_name_recv, 20, MPI_CHAR, source, source, MPI_COMM_WORLD, &mpi_consumer_request);
            //     // MPI_Wait(&mpi_consumer_request, &mpi_status_consumer);
            //     file_output_recv.open(file_output_name_recv, fstream::in);
            //     file_output.open("OutputRectangular/output.txt", fstream::out | fstream::ate);
            //
            //     if(file_output_recv.is_open()){
            //         if(file_output.is_open()){
            //             while(getline(file_output_recv, line_)){
            //                 file_output << line_;
            //             }
            //         }else{
            //             cout << __FILE__ << ": " << __LINE__ << endl;
            //         }
            //     }else{
            //         cout << __FILE__ << ": " << __LINE__ << endl;
            //     }
            // }
            // // cout << comm_rank << ":\t" << __FILE__ << ": " << __LINE__ << endl;
            // file_output_recv.close();
            // file_output.close();

        }

        // executa a simulacao nas maquinas slayers
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


            // inicializando a vizinhanca do cellular space criado em cada maquina
            for(int i = 0; i < (cs.height * cs.width); i++){
                cs.memoria[i] = Cell<R>((cs.x_init + (i/cs.width)), (i%cs.width), Attribute<R>(i, 1));
                cs.memoria[i] = cs.memoria[i].SetNeighbor();
            }

            // maquina i recebe a ordem de execucao do fluxo
            MPI_Recv(word_execute_recv, 23, MPI_CHAR, MASTER, 999, mpi_comm, &mpi_status);
            char *rank_c = strtok(word_execute_recv, "|:");
            char *x_c = strtok(NULL, ":");
            char *y_c = strtok(NULL, "|");
            char *flow_rate_c = strtok(NULL, "|:");
            int rank_ = atoi(rank_c);
            int x_ = atoi(x_c);
            int y_ = atoi(y_c);
            int flow_rate_ = atoi(flow_rate_c);

            // corrigir endereçamento de cell
            // corrigir x_init
            // corrigir acesso a cell
            // subtrair flow
            // incrementar flow

            if(comm_rank == rank_){
                cout << cs.memoria[x_*cs.width + y_ - cs.x_init].x << " " << cs.memoria[x_*cs.width + y_ - cs.x_init].y
                    << " " << cs.memoria[x_*cs.width + y_].count_neighbors << endl;

                //for(double t = 0; t < this->time; t = t + this->time_step){
                    this->flow.last_execute = this->flow.execute();
                    cout << comm_rank << ": " << this->flow.execute() << endl;
                // }

                int count_neighbors_send, y_send;
                R last_execute_send;

                // atualizando o valor do atributo na maquina vizinha
                // se a atualização for em comm_rank + 1
                if(cs.memoria[x_*cs.width + y_ - cs.x_init].x - cs.x_init == PROC_DIMX_REC-1){
                    switch(cs.memoria[x_*cs.width + y_ - cs.x_init].count_neighbors){
                        case 3:
                            cout << __FILE__ << ": " << __LINE__ << endl;
                            break;
                        case 5:
                            if(cs.memoria[x_*cs.width + y_ - cs.x_init].y == 0){
                                count_neighbors_send = 2;
                                last_execute_send = this->flow.last_execute/cs.memoria[x_*cs.width + y_ - cs.x_init].count_neighbors;
                                y_send = cs.memoria[x_*cs.width + y_ - cs.x_init].y;

                                MPI_Send(&count_neighbors_send, 1, MPI_INT, rank_+1, rank_, MPI_COMM_WORLD);
                                MPI_Send(&last_execute_send, 1, ConvertType(getAbstractionDataType<R>()), rank_+1, rank_, MPI_COMM_WORLD);
                                MPI_Send(&y_send, 1, MPI_INT, rank_+1, rank_+10, MPI_COMM_WORLD);
                            }else{
                                count_neighbors_send = 2;
                                last_execute_send = this->flow.last_execute/cs.memoria[x_*cs.width + y_ - cs.x_init].count_neighbors;
                                y_send = cs.memoria[x_*cs.width + y_ - cs.x_init].y - 1;

                                MPI_Send(&count_neighbors_send, 1, MPI_INT, rank_+1, rank_, MPI_COMM_WORLD);
                                MPI_Send(&last_execute_send, 1, ConvertType(getAbstractionDataType<R>()), rank_+1, rank_, MPI_COMM_WORLD);
                                cout << __FILE__ << ": " << __LINE__ << endl;
                                MPI_Send(&y_send, 1, MPI_INT, rank_+1, rank_+10, MPI_COMM_WORLD);
                            }
                            break;
                        case 8:
                            count_neighbors_send = 3;
                            last_execute_send = this->flow.last_execute/cs.memoria[x_*cs.width + y_ - cs.x_init].count_neighbors;
                            y_send = cs.memoria[x_*cs.width + y_ - cs.x_init].y - 1;

                            MPI_Send(&count_neighbors_send, 1, MPI_INT, rank_+1, rank_, MPI_COMM_WORLD);
                            MPI_Send(&last_execute_send, 1, ConvertType(getAbstractionDataType<R>()), rank_+1, rank_, MPI_COMM_WORLD);
                            MPI_Send(&y_send, 1, MPI_INT, rank_+1, rank_+10, MPI_COMM_WORLD);

                            cs.memoria[x_*cs.width + y_-1 - cs.x_init].attribute.value += last_execute_send;
                            cs.memoria[x_*cs.width + y_+1 - cs.x_init].attribute.value += last_execute_send;
                            for(int i = 0; i < count_neighbors_send; i++)
                                cs.memoria[(x_-1)*cs.width + y_-1+i - cs.x_init].attribute.value += last_execute_send;

                            cs.memoria[x_*cs.width + y_ - cs.x_init].attribute.value -= this->flow.last_execute;

                            break;
                        default:
                            cout << __FILE__ << ": " << __LINE__ << endl;
                    }
                }
            }
        //
        //     if(comm_rank == rank_+1){
        //         int count_neighbors_recv, y_recv;
        //         R last_execute_recv;
        //
        //         MPI_Recv(&count_neighbors_recv, 1, MPI_INT, rank_, rank_, MPI_COMM_WORLD, &mpi_status);
        //         MPI_Recv(&last_execute_recv, 1, ConvertType(getAbstractionDataType<R>()), rank_, rank_, MPI_COMM_WORLD, &mpi_status);
        //         MPI_Recv(&y_recv, 1, MPI_INT, rank_, rank_+10, MPI_COMM_WORLD, &mpi_status);
        //
        //         // cout << rank_+1 << ": " << count_neighbors_recv << " " << last_execute_recv << " " << y_recv << endl;
        //         for(int i = 0; i < count_neighbors_recv; i++)
        //             cs.memoria[y_recv + i].attribute.value += last_execute_recv;
        //     }
        //
        //     // Calculando o somatorio dos valores atributos resultante na particao do espaco celular
        //     R acumulated_value_send = 0;
        //     for(int i = 0; i < cs.height*cs.width; i++)
        //         acumulated_value_send += cs.memoria[i].attribute.value;
        //
        //     // cout << "\t" << comm_rank << ": " << acumulated_value_send << endl;
        //     MPI_Send(&acumulated_value_send, 1, ConvertType(getAbstractionDataType<R>()), MASTER, comm_rank, MPI_COMM_WORLD);
        //
        //     // Arquivo que armazenará os dados de saida
        //     fstream file_output;
        //     char file_output_name[20];
        //
        //     sprintf(file_output_name, "OutputRectangular/comm_rank%d.txt", comm_rank);
        //     file_output.open(file_output_name, fstream::out | fstream::trunc);
        //
        //     for(int i = 0; i < PROC_DIMX*PROC_DIMY; i++){
        //         file_output << cs.memoria[i].x << "\t" << cs.memoria[i].y << "\t";
        //         file_output << cs.memoria[i].attribute.value << endl;
        //     }
        //     file_output.close();
        //
        //     MPI_Send(file_output_name, 20, MPI_CHAR, MASTER, comm_rank, MPI_COMM_WORLD);
        }
    }
};

#endif
