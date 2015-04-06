#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

#define SEND_TAG 0
#define RECEIVE_TAG 1

#define MASTER 0

typedef struct {
        int start;
        int end;
} DataRange;

void master(int n);
void slave();

int procRank;
int numOfProc;

int n;

int main(int argc, char** argv) {
        // init MPI
        MPI_Init(&argc, &argv);
        // get number of process
        MPI_Comm_size(MPI_COMM_WORLD, &numOfProc);
        // get rank of this current process
        MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
        // get input
        n = 5000;
        if (procRank == MASTER) {
                master(n);
        }
        else {
                slave();
        }
        // exist this MPI process
        MPI_Finalize();
        return 0;
}

bool isEven(int x) {
        if (x % 2 == 0) {
                return true;
        }
        return false;
}

void master(int n) {
        // split data for processes
        DataRange* range = new DataRange[numOfProc];
        // total range from 1 -> n, 
        // miss number '0' (0 is even), count '0' later
        for (int i = 0; i < numOfProc; i++) {
                range[i].start = i * n / numOfProc + 1;
                range[i].end = (i+1) * n / numOfProc;
        }
        printf("range for master = %d -> %d", range[0].start, range[0].end);
        // send range data to processes
        // start from range[1]
        // range[0] is process by master
        for (int i = 1; i < numOfProc; i++) {
                // MPI_Send(void* buffer, int countBuffer, MPI_Datatype, , int destination, int tag, MPI_Comm comm)
                printf("Master send data to slave number %d\n", i);
                printf("range for slave %d = %d->%d\n", i, range[i].start, range[i].end);
                MPI_Send(&range[i], sizeof(DataRange), MPI_BYTE, i, SEND_TAG, MPI_COMM_WORLD);
        }
        // master process work
        int count = 0;
        for (int i = range[0].start; i <= range[0].end; i++) {
                if (isEven(i)) {
                        count++;
                }
        }
        printf("master count = %d\n", count);
        // receive data from slave processes
        MPI_Status status;
        for (int i = 1; i < numOfProc; i++) {
                printf("Master receive data from slave number %d\n", i);
                int temp;
                MPI_Recv(&temp, 1, MPI_INT, i, RECEIVE_TAG, MPI_COMM_WORLD, &status);
                printf("master recieve count from slave %d = %d\n", i, temp);
                count += temp;
        }
        // count number '0'
        count += 1;
        // print result
        printf("Number of even number in range from 0 -> %d is %d\n", n, count);
        // clean pointer
        delete[] range;
}

void slave() {
        // create buffer for receiving data from master
        DataRange buffer;
        printf("Slave number %d start processing\n", procRank);
        // Receive data from master
        MPI_Status status;
        // int MPI_Recv(void* buffer, int count, MPI_Datatype type, int src, int tag, MPI_Comm, MPI_Status* status)
        MPI_Recv(&buffer, sizeof(DataRange), MPI_BYTE, 0, SEND_TAG, MPI_COMM_WORLD, &status);
        
        // slave process work
        int count = 0;
        for (int i = buffer.start; i <= buffer.end; i++) {
                if (isEven(i)) {
                        count++;
                }
        }
        // send result back to master process
        printf("slave %d - count = %d\n", procRank, count); 
        MPI_Send(&count, 1, MPI_INT, 0, SEND_TAG, MPI_COMM_WORLD);
}
