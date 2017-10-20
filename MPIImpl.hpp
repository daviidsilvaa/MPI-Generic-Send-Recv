
#include "Abstraction.hpp"
#include "mpi.h"
#include <vector>

void Make(MPI_Datatype *AttributeMPI);

void SendImpl(void *data, int count, Abstraction::DataType type, int dest, int tag);
void ReceiveImpl(void *data, int count, Abstraction::DataType type, int src, int tag);

template<class T>
void Send(std::vector<T> &vec, int dest, int tag = 0){
	SendImpl(vec.data(), vec.size(), getAbstractionDataType<T>(), dest, tag);
}

template<class T>
void Receive(std::vector<T> &vec, int src, int tag = 0){
	ReceiveImpl(vec.data(), vec.size(), getAbstractionDataType<T>(), src, tag);
};
