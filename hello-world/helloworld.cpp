#include <mpi.h>
#include <stdio.h>
#define SEND_TAG 0
#define RECEIVE_TAG 1

void master();
void worker();

int rank;
int size;

int main(int argc, char** argv) {
	// init MPI lib
	MPI_Init(&argc, &argv);
	// get number of process ("size" variable)
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	// get rank of this running process, know who I am
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0) {
		master();
	}
	else {
		worker();
	}
	// exist this MPI process
	MPI_Finalize();
	return 0;
	
} 

void master() {
	printf("hello from master\n");
}

void worker() {
	printf("hello from worker number %d\n", rank);
}
