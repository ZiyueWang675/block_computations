#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <assert.h>

#include "matrix.hpp"

/*
 * Allocates a rows-by-cols matrix and returns it
 */
matrix_t *make_matrix(int rows, int cols)
{
  matrix_t *new_matrix = (matrix_t *)malloc(sizeof(matrix_t));
  new_matrix->rows = rows;
  new_matrix->cols = cols;
  new_matrix->colstride = rows;
  new_matrix->values = (double *) malloc(sizeof(double) * rows * cols);
  return new_matrix; 
}

/*
 * Frees an allocated matrix (not a submatrix)
 */
void free_matrix(matrix_t *m)
{
  free(m->values);
  free(m);
}

/*
 * Print Matrix
 */
void print_matrix(matrix_t *m)
{
  int i, j;
  printf("------------\n");
  for (i = 0; i < m->rows; i++) {
    for (j = 0; j < m->cols; j++) {
      printf("  %g  ", element(m,i,j));
    }
    printf("\n");
  }
  printf("------------\n");
}

int serial_matrix_multiply(matrix_t *A, matrix_t *B, matrix_t *C)
{
  int i, j, k;
  for (i = 0; i < A->rows; i++) {
    for (k = 0; k < A->cols; k++) {
      for (j = 0; j < B->cols; j++) {
        element(C,i,j) += element(A,i,k) * element(B,k,j);
      }
    }
  }
  return 0;
}