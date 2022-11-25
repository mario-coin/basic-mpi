#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv) {
	// MPI
	int rank, size;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int result;
	if (rank == 0) {
		result = 1;

		printf("Process %d => sending %d\n", rank, result);

		MPI_Send(&result, 1, MPI_INT, rank+1, 10, MPI_COMM_WORLD);
		MPI_Send(&result, 1, MPI_INT, rank+2, 10, MPI_COMM_WORLD);
	}
	else {
		MPI_Status status;
		int value1, value2;

		MPI_Recv(&value1, 1, MPI_INT, rank-1, 10, MPI_COMM_WORLD, &status);
		
		if (rank == 1) {
			result = value1;

			printf("Process %d => receiving number %d, and sending %d\n", rank, value1, result);
		}
		else if (rank > 1) {
			MPI_Recv(&value2, 1, MPI_INT, rank-2, 10, MPI_COMM_WORLD, &status);
			result = value1 + value2;

			printf("Process %d => receiving numbers %d and %d, and sending %d\n", rank, value1, value2, result);
		}

		if (rank < size-1) {
			MPI_Send(&result, 1, MPI_INT, rank+1, 10, MPI_COMM_WORLD);
		}
		if (rank < size-2) {
			MPI_Send(&result, 1, MPI_INT, rank+2, 10, MPI_COMM_WORLD);
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);

	int *recvbuffer = (int *)malloc(size*sizeof(int));
	MPI_Gather(&result, 1, MPI_INT, recvbuffer, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		printf("\nProcess %d => vector received. Printing\n", rank);
		for (int i = 0; i < size; i++) {
			printf("%d ", recvbuffer[i]);
		}
	}

	// Closing
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
    return 0;
}