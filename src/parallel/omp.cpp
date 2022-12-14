#include "util/matrix.hpp"
#include "util/parser.hpp"
#include <math.h>
#include <omp.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
using namespace std;

int dimension;
int thread_count;
matrix_t *matrix_A;
matrix_t *matrix_B;
matrix_t *matrix_C;

void Usage(char prog_name[]);

void readMatrix(int dimension /* in  */, matrix_t *matrix_A /* out */,
                matrix_t *matrix_B /* out */);

void Get_args(char *argv[] /* in  */, int *dimension /* out */,
              int *thread_count /* out */);

int main(int argc, char *argv[]) {
  if (argc != 3)
    Usage(argv[0]);
  Get_args(argv, &dimension, &thread_count);
  matrix_A = make_matrix(dimension, dimension);
  matrix_B = make_matrix(dimension, dimension);
  matrix_C = make_matrix(dimension, dimension);
  readMatrix(dimension, matrix_A, matrix_B);

  // for timer
  struct timeval t1, t2;

  gettimeofday(&t1, NULL);

#pragma omp parallel num_threads(thread_count)
  {
    int my_rank = omp_get_thread_num();

    int local_stripes = 1;
    int row_number = 0;
    local_stripes =
        (my_rank != thread_count - 1)
            ? (dimension / thread_count)
            : (dimension - (dimension / thread_count) * (thread_count - 1));

    // Threads start to work!

    for (int j = 0; j < dimension; j++) {
      for (int k = 0; k < dimension; k++) {
        for (int i = 0; i < local_stripes; i++) {
          row_number = (dimension / thread_count) * my_rank + i;
          element(matrix_C, row_number, j) +=
              element(matrix_A, row_number, k) * element(matrix_B, k, j);
        }
      }
    }
  }
  gettimeofday(&t2, NULL);
  double time_spend =
      (t2.tv_sec - t1.tv_sec) + (double)(t2.tv_usec - t1.tv_usec) / 1000000.0;
  printf("Time cost is %.5f for %i threads and %i dimension\n", time_spend,
         thread_count, dimension);

  return 0;
}

void Usage(char prog_name[]) {
  fprintf(stderr, "usage: %s ", prog_name);
  fprintf(stderr, "<matrix_dimension> <thread_count>\n");
  exit(0);
}

void readMatrix(int dimension, matrix_t *matrix_A, matrix_t *matrix_B) {

  string dim = to_string(dimension);
  string filename = "matrix" + dim + "x" + dim + ".txt";
  load_matrix(filename, dimension, matrix_A, matrix_B);
}

void Get_args(char *argv[], int *dimension, int *thread_count) {
  *dimension = strtol(argv[1], NULL, 10);
  *thread_count = strtol(argv[2], NULL, 10);
}
