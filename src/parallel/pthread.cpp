#include "util/matrix.hpp"
#include "util/parser.hpp"
#include <math.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
using namespace std;

int dimension;
int thread_count;
pthread_t *threads;
matrix_t *matrix_A;
matrix_t *matrix_B;
matrix_t *matrix_C;

void Usage(char prog_name[]);

void readMatrix(int dimension /* in  */, matrix_t *matrix_A /* out */,
                matrix_t *matrix_B /* out */);

void Get_args(char *argv[] /* in  */, int *dimension /* out */,
              int *thread_count /* out */);

void *thread_work(void *);

int main(int argc, char *argv[]) {
  struct timeval start, end;
  double time;
  int i;

  if (argc != 3)
    Usage(argv[0]);
  Get_args(argv, &dimension, &thread_count);

  if (thread_count > dimension)
    thread_count = dimension;

  threads = (pthread_t *)malloc(thread_count * sizeof(pthread_t));

  matrix_A = make_matrix(dimension, dimension);
  matrix_B = make_matrix(dimension, dimension);
  matrix_C = make_matrix(dimension, dimension);
  readMatrix(dimension, matrix_A, matrix_B);
  // gettimeofday(&end, NULL);
  // time = (end.tv_sec - start.tv_sec)+(double)(end.tv_usec -
  // start.tv_usec)/1000000.0; printf("Time cost for reading matrices is
  // %.2f\n", time);

  // gettimeofday(&start, NULL);

  // This section is for comparison purpose, you can ignore it if you want
  // matrix_t* matrix_D;
  // matrix_D = make_matrix(dimension,dimension);
  // gettimeofday(&start, NULL);
  // serial_matrix_multiply(matrix_A, matrix_B, matrix_D);
  // gettimeofday(&end, NULL);
  // time = (end.tv_sec - start.tv_sec)+(double)(end.tv_usec -
  // start.tv_usec)/1000000.0; printf("Time cost for serial program is %.2f\n",
  // time); free_matrix(matrix_D);

  gettimeofday(&start, NULL);

  for (i = 0; i < thread_count; i++) {
    pthread_create(&threads[i], NULL, thread_work, (void *)(intptr_t)i);
  }

  for (i = 0; i < thread_count; i++) {
    pthread_join(threads[i], NULL);
  }

  gettimeofday(&end, NULL);

  time = (end.tv_sec - start.tv_sec) +
         (double)(end.tv_usec - start.tv_usec) / 1000000.0;
  printf("Time cost is %.5f for %i threads and %i dimension\n", time,
         thread_count, dimension);
  free(threads);
  free_matrix(matrix_A);
  free_matrix(matrix_B);
  free_matrix(matrix_C);

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

void *thread_work(void *rank) {
  int my_rank = (int)(intptr_t)rank;
  int local_stripes = 1;
  int row_number = 0;
  // int tasks_per_thread = 1;

  local_stripes =
      (my_rank != thread_count - 1)
          ? (dimension / thread_count)
          : (dimension - (dimension / thread_count) * (thread_count - 1));
  // tasks_per_thread = (my_rank!=thread_count-1) ?
  // (dimension*dimension/thread_count) :
  // (dimension*dimension-(dimension*dimension/thread_count)*(thread_count -
  // 1));

  // Threads start to work!
  for (int i = 0; i < local_stripes; i++) {
    row_number = (dimension / thread_count) * my_rank + i;
    for (int j = 0; j < dimension; j++) {
      for (int k = 0; k < dimension; k++) {
        element(matrix_C, row_number, j) +=
            element(matrix_A, row_number, k) * element(matrix_B, k, j);
      }
    }
  }

  return 0;
}