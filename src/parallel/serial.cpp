#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <math.h>
#include <assert.h>

#include "matrix.hpp"
#include "parser.hpp"

using namespace std;

int matrix_multiply(matrix_t *A, matrix_t *B, matrix_t *C)
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

int main()
{
    int Anr = 4;
    int Anc = 5;
    int Bnr = 3;
    int Bnc = 3;
    double time1, time2, elapsed, flops;
    matrix_t *A,*B,*C;
    A = make_matrix(Anr, Anc);
    B = make_matrix(Anc, Bnc);
    C = make_matrix(Anr, Bnc);
    time1 = elapsed_seconds(); 
    matrix_multiply(A, B, C);
    time2 = elapsed_seconds();
    elapsed = time2 - time1;
    printf("Serial: Time for multiplying %ix%i and %ix%i matrices is:    %.2f sec\n", Anc, Anr, Bnc, Bnr, elapsed);
}