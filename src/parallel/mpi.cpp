#include "util/matrix.hpp"
#include "util/parser.hpp"
#include <math.h>
#include <mpi.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
using namespace std;

int dimension;
matrix_t *matrixC;
struct timeval t1, t2;
double *local_A, *local_B, *local_C, *tempA, *tempB;
float start;
int my_rank, processors;
int local_stripes;
int stripe_number;
int original_distribution;

void Usage(char prog_name[]);

void copy(double *x, double *y, int size);

void readMatrix();

void block_striped();

void Get_args(char *argv[] /* in  */, int *dimension /* out */);

int main(int argc, char *argv[]) {
  if (argc != 2)
    Usage(argv[0]);
  Get_args(argv, &dimension);

  gettimeofday(&t1, NULL);

  /* Start up MPI */
  MPI_Init(NULL, NULL);
  /* Get the number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &processors);
  /* Get my rank among all the processes */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  if (processors > dimension) {
    if (my_rank == 0)
      printf("Processor number should be either smaller than or equal to "
             "dimension!\n");

    MPI_Finalize();
    return 0;
  }

  if (dimension % processors != 0) {
    if (my_rank == 0)
      printf("Dimension should be divisible by processor number!\n");

    MPI_Finalize();
    return 0;
  }

  MPI_Barrier(MPI_COMM_WORLD);
  start = MPI_Wtime();
  local_stripes =
      (my_rank != processors - 1)
          ? (dimension / processors)
          : (dimension - (dimension / processors) * (processors - 1));

  matrixC = make_matrix(dimension, dimension);
  stripe_number =
      (dimension % processors == 0)
          ? local_stripes * dimension
          : (dimension - (dimension / processors) * (processors - 1)) *
                dimension;
  local_A = (double *)malloc(stripe_number * sizeof(double));
  local_B = (double *)malloc(stripe_number * sizeof(double));
  local_C = (double *)malloc(stripe_number * sizeof(double));
  tempA = (double *)malloc(stripe_number * sizeof(double));
  tempB = (double *)malloc(stripe_number * sizeof(double));
  for (int i = 0; i < stripe_number; i++) {
    local_C[i] = 0;
  }
  original_distribution = my_rank;

  readMatrix();

  block_striped();

  if (my_rank == 0) {
    MPI_Status status;
    int processor_stripes;
    for (int i = 0; i < local_stripes; i++)
      for (int j = 0; j < dimension; j++) {
        element(matrixC, i, j) = local_C[i * dimension + j];
      }
    for (int i = 1; i < processors; i++) {
      processor_stripes =
          (i != processors - 1)
              ? (dimension / processors)
              : (dimension - (dimension / processors) * (processors - 1));
      MPI_Recv(local_C, dimension * processor_stripes, MPI_DOUBLE, i, 1,
               MPI_COMM_WORLD, &status);
      for (int j = 0; j < processor_stripes; j++)
        for (int k = 0; k < dimension; k++)
          element(matrixC, (dimension / processors) * i + j, k) =
              local_C[j * dimension + k];
    }
    float end = MPI_Wtime();
    float read_time =
        (t2.tv_sec - t1.tv_sec) + (double)(t2.tv_usec - t1.tv_usec) / 1000000.0;
    printf("Time cost is %.5f for %i processors and %i dimension\n",
           end - start - read_time, processors, dimension);
  } else {

    MPI_Send(local_C, dimension * local_stripes, MPI_DOUBLE, 0, 1,
             MPI_COMM_WORLD);
  }

  free(local_A);
  free(local_B);
  free(local_C);
  free(tempA);
  free(tempB);
  free_matrix(matrixC);
  MPI_Barrier(MPI_COMM_WORLD);
  /* Shut down MPI */
  MPI_Finalize();
  return 0;
}

void readMatrix() {
  if (my_rank == 0) {
    int index;
    int processor_stripes;
    matrix_t *matrixA, *matrixB;

    matrixA = make_matrix(dimension, dimension);
    matrixB = make_matrix(dimension, dimension);
    gettimeofday(&t1, NULL);
    string dim = to_string(dimension);
    string filename = "matrix" + dim + "x" + dim + ".txt";
    load_matrix(filename, dimension, matrixA, matrixB);
    gettimeofday(&t2, NULL);

    // serial_matrix_multiply(matrixA, matrixB, matrixD);
    for (int i = 0; i < processors; i++) {

      processor_stripes =
          (i != processors - 1)
              ? (dimension / processors)
              : (dimension - (dimension / processors) * (processors - 1));
      if (i == 0) {
        for (int j = 0; j < processor_stripes; j++) {
          for (int k = 0; k < dimension; k++) {
            index = k + j * dimension;
            local_A[index] =
                element(matrixA, (dimension / processors) * my_rank + j, k);
            local_B[index] =
                element(matrixB, k, (dimension / processors) * my_rank + j);
          }
        }
      } else {

        for (int j = 0; j < processor_stripes; j++) {
          for (int k = 0; k < dimension; k++) {
            index = k + j * dimension;
            tempA[index] =
                element(matrixA, (dimension / processors) * i + j, k);
            tempB[index] =
                element(matrixB, k, (dimension / processors) * i + j);
          }
        }
        MPI_Send(tempA, stripe_number, MPI_DOUBLE, i, 1, MPI_COMM_WORLD);
        MPI_Send(tempB, stripe_number, MPI_DOUBLE, i, 2, MPI_COMM_WORLD);
      }
    }
    free_matrix(matrixA);
    free_matrix(matrixB);

  } else {
    MPI_Status status;
    MPI_Recv(local_A, stripe_number, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(local_B, stripe_number, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD, &status);
  }
}

void block_striped() {
  MPI_Status status;
  int processor_stripes;
  int col_index;
  int src, dest;
  int local_c_stripe_pos = original_distribution;
  for (int stage = 0; stage < processors; stage++) {
    src = (my_rank + processors * (stage + 1) - stage - 1) % processors;
    dest = (my_rank + stage + 1) % processors;
    processor_stripes =
        (local_c_stripe_pos != processors - 1)
            ? (dimension / processors)
            : (dimension - (dimension / processors) * (processors - 1));
    for (int i = 0; i < processor_stripes; i++) {
      for (int j = 0; j < processor_stripes; j++) {
        col_index =
            dimension * i + (dimension / processors) * local_c_stripe_pos + j;

        for (int k = 0; k < dimension; k++) {
          local_C[col_index] +=
              local_A[dimension * i + k] * local_B[dimension * j + k];
        }
      }
    }

    // src processor number from the end of last stage
    MPI_Sendrecv(local_B, stripe_number, MPI_DOUBLE, dest, 2, tempB,
                 stripe_number, MPI_DOUBLE, src, 2, MPI_COMM_WORLD, &status);
    MPI_Sendrecv(&local_c_stripe_pos, 1, MPI_INT, dest, 2,
                 &original_distribution, 1, MPI_INT, src, 2, MPI_COMM_WORLD,
                 &status);
    local_c_stripe_pos = original_distribution;
    copy(local_B, tempB, stripe_number);
  }
}

void copy(double *x, double *y, int size) {
  for (int i = 0; i < size; i++) {
    x[i] = y[i];
  }
}

void Usage(char prog_name[]) {
  fprintf(stderr, "usage: %s ", prog_name);
  fprintf(stderr, "<matrix_dimension>\n");
  exit(0);
}

void Get_args(char *argv[], int *dimension) {
  *dimension = strtol(argv[1], NULL, 10);
}
