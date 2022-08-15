#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <sys/time.h>
#include "util/parser.hpp"
#include "mpi.h"
#include "util/matrix.hpp"

using namespace std;

matrix_t *matrixC;
double *local_A, * local_B, *local_C, *tempA, *tempB;
int dimension;
struct timeval t1, t2;
int block, block_dim;
int processors, sqrt_processors, my_rank;
int row, col;

void Get_args(
        char*   argv[]       /* in  */,
        int*    dimension    /* out */);

void copy(
        double*   x       /* out */,
        double*   y       /* in  */,
		int       size    /* in  */);

void cannon();

void readMatrix();

void Usage(char prog_name[]);

#define getPosition(row,col) (col + sqrt_processors) % sqrt_processors + ((row + sqrt_processors) % sqrt_processors) * sqrt_processors

int main(int argc, char* argv[])
{
    if (argc != 2) Usage(argv[0]); 
    Get_args(argv, &dimension);
    MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &processors);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Barrier(MPI_COMM_WORLD);
    float start = MPI_Wtime();

	sqrt_processors = sqrt(processors);

    if (dimension % sqrt_processors != 0)
    {
        if(my_rank == 0)
            printf("The dimension must be divisible by the square root of processors number!\n");

        MPI_Finalize();
		return 0;
    }

	if (sqrt_processors * sqrt_processors != processors)
	{
		if (my_rank == 0)
			printf("Number of processors must be a perfect square!\n");

		MPI_Finalize();
		return 0;
	}

	block = dimension / sqrt_processors;
	block_dim = block * block;
	row = my_rank / sqrt_processors;
	col = my_rank % sqrt_processors;
    matrixC = make_matrix(dimension, dimension);
    local_A = (double*) malloc(block_dim * sizeof(double));
    local_B = (double*) malloc(block_dim * sizeof(double));
    local_C = (double*) malloc(block_dim * sizeof(double));
    tempA= (double*) malloc(block_dim * sizeof(double));
	tempB = (double*) malloc(block_dim * sizeof(double));

	for(int i = 0; i<block_dim; i++)
    {
        local_C[i] = 0;
    }

	readMatrix();

    //The only difference between Fox Algorithm
	cannon();

	if (my_rank == 0)
	{
		MPI_Status status;
	    int row_start, row_end, col_start, col_end;
	    for (int i = 0; i < block; i++)
		    for (int j = 0; j < block; j++)
			    element(matrixC,i,j) = local_C[i * block + j];
	    for (int i = 1; i < processors; i++)
	    {
		    MPI_Recv(local_C, block_dim, MPI_DOUBLE, i, 1, MPI_COMM_WORLD, &status);
		    row_start = (i / sqrt_processors) * block;
		    row_end = row_start + block;
		    col_start = (i % sqrt_processors) * block;
		    col_end = col_start + block;
		    for (int j = row_start; j < row_end; j++)
			    for (int k = col_start; k < col_end; k++)
				    element(matrixC,j,k) = local_C[(j - row_start) * block + k - col_start];
	    }
		float end = MPI_Wtime();
        float read_time = (t2.tv_sec - t1.tv_sec)+(double)(t1.tv_usec - t2.tv_usec)/1000000.0;
		printf("Time cost is %.5f for %i processors and %i dimension\n", end-start-read_time, processors, dimension);
	}
	else
	{
		MPI_Send(local_C, block_dim, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
	}

    free_matrix(matrixC);
    free(local_A);
    free(local_B);
    free(local_C);
    free(tempA);
    free(tempB);

    MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	return 0;
}

void copy(double* x, double* y, int size)
{
	for(int i = 0; i< size; i++)
	{
		x[i] = y[i];
	}
}

void readMatrix()
{
    MPI_Status status;
    if (my_rank == 0)
	{
        int index;
        int row_start, row_end, col_start, col_end;
        matrix_t *matrixA, *matrixB;
        matrixA = make_matrix(dimension, dimension);
	    matrixB = make_matrix(dimension, dimension);

		gettimeofday(&t1, NULL);
		string dim = to_string(dimension);
        string filename = "matrix"+ dim + "x"+ dim + ".txt";
        load_matrix(filename, dimension,  matrixA, matrixB);
		gettimeofday(&t2, NULL);
		
	    for (int i = 0; i < processors; i++)
	    {
		    row_start = (i / sqrt_processors) * block;
		    row_end = row_start + block;
		    col_start = (i % sqrt_processors) * block;
		    col_end = col_start + block;

            if(i == 0)
            {
		        for (int j = row_start; j < row_end; j++)
		        {
			        for (int k = col_start; k < col_end; k++)
			        {
                        index = (j - row_start) * block + k - col_start;
                        local_A[index] = element(matrixA, j, k);
                        local_B[index] = element(matrixB, j, k);
			        }
		        }
            }
		    else
            {
                for (int j = row_start; j < row_end; j++)
		        {
			        for (int k = col_start; k < col_end; k++)
			        {
                        index = (j - row_start) * block + k - col_start;
                        tempA[index] = element(matrixA, j, k);
				        tempB[index] = element(matrixB, j, k);
			        }
		        }
			    MPI_Send(tempA, block_dim, MPI_DOUBLE, i, 1, MPI_COMM_WORLD);
			    MPI_Send(tempB, block_dim, MPI_DOUBLE, i, 2, MPI_COMM_WORLD);
		    }
	    }
        free_matrix(matrixA);
        free_matrix(matrixB);
	}
	else
	{
		MPI_Recv(local_A, block_dim, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
		MPI_Recv(local_B, block_dim, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD, &status);
	}
}

void cannon()
{
    MPI_Status status;

    //initial send
    MPI_Sendrecv(local_A, block_dim, MPI_DOUBLE, getPosition(row, col - row), 1, tempA, block_dim, MPI_DOUBLE, getPosition(row, col + row), 1, MPI_COMM_WORLD, &status);
	MPI_Sendrecv(local_B, block_dim, MPI_DOUBLE, getPosition(row - col, col), 2, tempB, block_dim, MPI_DOUBLE, getPosition(row + col, col), 2, MPI_COMM_WORLD, &status);
	copy(local_A, tempA, block_dim);
	copy(local_B, tempB, block_dim);

	for (int stage = 0; stage < sqrt_processors; stage++)
	{
		for (int i = 0; i < block; i++)
		{
			for (int j = 0; j < block; j++)
			{
				for (int k = 0; k < block; k++)
					local_C[i * block + j] += local_A[i * block + k] * local_B[k * block + j];
			}
		}
		MPI_Sendrecv(local_A, block_dim, MPI_DOUBLE, getPosition(row, col - 1), 1, tempA, block_dim, MPI_DOUBLE, getPosition(row, col + 1), 1, MPI_COMM_WORLD, &status);
		MPI_Sendrecv(local_B, block_dim, MPI_DOUBLE, getPosition(row - 1, col), 2, tempB, block_dim, MPI_DOUBLE, getPosition(row + 1, col), 2, MPI_COMM_WORLD, &status);
		copy(local_A, tempA, block_dim);
		copy(local_B, tempB, block_dim);
	}
}

void Usage(char prog_name[])
{
   fprintf(stderr, "usage: %s ", prog_name); 
   fprintf(stderr, "<matrix_dimension>\n");
   exit(0);
}

void Get_args(char* argv[], int* dimension)
{
    *dimension = strtol(argv[1], NULL, 10);
}
