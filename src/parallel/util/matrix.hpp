/**
 * Matrix Multiply API
 *
 **/

#ifndef _MATRIX_H

#define _MATRIX_H

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <assert.h>

/**
 *  A matrix_t stores a matrix with all its elements
 *  in a single one-dimensional vector in memory, in
 *  column major order, that is, with all the elements
 *  in a column lined up next to each other in order.
 *
 *  "colstride" is the distance in memory from the
 *  beginning of one column to the beginning of the next.
 *  Ordinarily this is equal to the number of rows;
 *  colstride will be used when we start breaking matrices
 *  up into blocks.  The BLAS library calls this "lda".
 *  
 *  The macro "element(X,i,j)" expands to a pointer to
 *  the element X(i,j) in storage.
 *
 *  Rows and columns of a matrix_t are indexed starting at 0.
 *  A matrix_t doesn't have to be square.
 */
typedef struct {
  int rows;          // number of rows
  int cols;          // number of columns
  int colstride;     // distance between column starts
  double *values;    // elements in column major order
} matrix_t;

#define element(X,i,j) (X)->values[(i) + (j)*(X)->colstride]

matrix_t * make_matrix(int rows, int cols);
void free_matrix(matrix_t *m);
void print_matrix(matrix_t *m);
double elapsed_seconds();
int serial_matrix_multiply(matrix_t *A, matrix_t *B, matrix_t *C);

#endif // _MATRIX_H
