
#ifndef MPI_REPORT_HPP
#define MPI_REPORT_HPP

class MPI_Report{
public:
	int comm_size;
	int rank_id;

	MPI_Report(){ }
	MPI_Report(const MPI_Report &mpi_report){
		this->comm_size = mpi_report.comm_size;
		this->rank_id = mpi_report.rank_id;
	}
	MPI_Report(const int &comm_size, const int &rank_id){
		this->comm_size = comm_size;
		this->rank_id = rank_id;
	}
	~MPI_Report(){ }
};

#endif
