
#include <mpi.h>
#include "MPIImpl.hpp"
#include "Attribute.hpp"
#include <stdexcept>

void Make(MPI_Datatype *AttributeMPI){
	const int nitens = 2;
	MPI_Datatype blocktypes[2] = {MPI_INT, ConvertType(getAbstractionDataType<T>())};
	int blocklengths[2] = {1, 1};
	MPI_Aint offsets[2] = {offsetof(Attribute<T>, key), offsetof(Attribute<T>, value)};

	MPI_Type_create_struct(nitens, blocklengths, offsets, blocktypes, AttributeMPI);
	MPI_Type_commit(AttributeMPI);
}

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

void SendImpl(void *data, int count, Abstraction::DataType type, int dest, int tag){
	if (MPI_Send(data, count, ConvertType(type), dest, tag, MPI_COMM_WORLD) != MPI_SUCCESS)
		throw std::runtime_error("MPI_Send failed");
}

void ReceiveImpl(void *data, int count, Abstraction::DataType type, int src, int tag){
	MPI_Status status;
	if (MPI_Recv(data, count, ConvertType(type), src, tag, MPI_COMM_WORLD, &status) != MPI_SUCCESS)
		throw std::runtime_error("MPI_Recv failed");
}
