#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "util/parser.hpp"
#include "mpi.h"
#include "util/matrix.hpp"

using namespace std;

matrix_t *matrixC;
double *local_A, * local_B, *local_C, *tempA, *tempB;
int dimension;
int block, block_dim;
int processors, sqrt_processors, my_rank;
int row, col;

int getPosition(
        int row              /* in  */,
        int col              /* in  */);

void Get_args(
        char*   argv[]       /* in  */,
        int*    dimension    /* out */);

void fox();

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
	fox();

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
		printf("Time cost for mpi program with fox algorithm is %.2f\n", end-start);
        //print_matrix(matrixC);
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

void readMatrix()
{
    if (my_rank == 0)
	{
        int index;
        int row_start, row_end, col_start, col_end;
        matrix_t *matrixA, *matrixB;
        matrixA = make_matrix(dimension, dimension);
	    matrixB = make_matrix(dimension, dimension);

		string dim = to_string(dimension);
        string filename = "matrix"+ dim + "x"+ dim + ".txt";
        load_matrix(filename, dimension,  matrixA, matrixB);
		
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
		MPI_Status status;
		MPI_Recv(local_A, block_dim, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
		MPI_Recv(local_B, block_dim, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD, &status);
	}
    
}

void fox()
{
    MPI_Status status;
	int local_col = row;
	int pos_start, pos_end;
	for (int stage = 0; stage < sqrt_processors; stage++)
	{
		if (col == local_col)
		{
			pos_start = getPosition(row, 0);
			pos_end =   getPosition(row, sqrt_processors - 1);
			for (int i = pos_start; i <= pos_end; i++)
			{
				if (i == my_rank)
					continue;
				MPI_Send(local_A, block_dim, MPI_DOUBLE, i, 1, MPI_COMM_WORLD);
			}
			memcpy(tempA, local_A, block_dim * sizeof(double));
		}
		else
		{
			MPI_Recv(tempA, block_dim, MPI_DOUBLE, getPosition(row, local_col), 1, MPI_COMM_WORLD, &status);
		}
		local_col = (local_col + 1) % sqrt_processors;

		for (int i = 0; i < block; i++)
		{
			for (int j = 0; j < block; j++)
			{
				for (int k = 0; k < block; k++)
					local_C[i * block + j] += tempA[i * block + k] * local_B[k * block + j];
			}
		}
		MPI_Sendrecv(local_B, block_dim, MPI_DOUBLE, getPosition(row - 1, col), 2, tempB, block_dim, MPI_DOUBLE, getPosition(row + 1, col), 2, MPI_COMM_WORLD, &status);
		memcpy(local_B, tempB, block_dim * sizeof(double));
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
