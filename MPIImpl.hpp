
#ifndef MPIIMPL_HPP
#define MPIIMPL_HPP

#include "mpi.h"
#include "Abstraction.hpp"
#include "Attribute.hpp"
#include "Cell.hpp"
#include <vector>

static MPI_Datatype ConvertType(Abstraction::DataType type){
	switch (type){
	case Abstraction::type_char: return MPI_CHAR;
	case Abstraction::type_unsigned_char: return MPI_UNSIGNED_CHAR;
	case Abstraction::type_short: return MPI_SHORT;
	case Abstraction::type_unsigned_short: return MPI_UNSIGNED_SHORT;
	case Abstraction::type_int: return MPI_INT;
	case Abstraction::type_unsigned_int: return MPI_UNSIGNED;
	case Abstraction::type_long: return MPI_LONG;
	case Abstraction::type_unsigned_long: return MPI_UNSIGNED_LONG;
	case Abstraction::type_float: return MPI_FLOAT;
	case Abstraction::type_double: return MPI_DOUBLE;
	};
	throw std::runtime_error("MPI_Datatype Convert(Abstraction::DataType) failed");
}

template<class T>
MPI_Datatype Make_MPI_Atribute(){
    MPI_Datatype MPI_Attribute;

	const int nitens = 2;
	MPI_Datatype blocktypes[2] = {MPI_INT, ConvertType(getAbstractionDataType<T>())};
	int blocklengths[2] = {1, 1};
	MPI_Aint offsets[2] = {offsetof(Attribute<T>, key), offsetof(Attribute<T>, value)};

	MPI_Type_create_struct(nitens, blocklengths, offsets, blocktypes, &MPI_Attribute);
	MPI_Type_commit(&MPI_Attribute);

	return MPI_Attribute;
}

template<typename T>
MPI_Datatype Make_MPI_Cell(MPI_Datatype *MPI_Attribute){
    Make_MPI_Atribute<T>(MPI_Attribute);

    MPI_Datatype MPI_Cell;

    const int nitens = 3;
    MPI_Datatype blocktypes[3] = {MPI_INT, MPI_INT, *MPI_Attribute};
    int blocklengths[3] = {1, 1, 1};
    MPI_Aint offsets[3] = {offsetof(Cell<T>, x), offsetof(Cell<T>, y), offsetof(Cell<T>, attribute)};

    MPI_Type_create_struct(nitens, blocklengths, offsets, blocktypes, &MPI_Cell);
    MPI_Type_commit(&MPI_Cell);

    return MPI_Cell;
}

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

#endif
