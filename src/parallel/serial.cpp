#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <sys/time.h>
#include "util/parser.hpp"
#include "util/matrix.hpp"

void readMatrix(
      int dimension      /* in  */,
      matrix_t* matrix_A /* out */,
      matrix_t* matrix_B /* out */);

void Get_args(
        char*   argv[]       /* in  */,
        int*    dimension    /* out */);

void Usage(char prog_name[]);

int main(int argc, char* argv[])
{
    int dimension;
    matrix_t* matrix_A;
    matrix_t* matrix_B;
    matrix_t* matrix_C;
    struct timeval t1, t2;
    if (argc != 2) Usage(argv[0]); 
    Get_args(argv, &dimension);
    matrix_A = make_matrix(dimension,dimension);
    matrix_B = make_matrix(dimension,dimension);
    matrix_C = make_matrix(dimension,dimension);
    readMatrix(dimension, matrix_A, matrix_B);
    gettimeofday(&t1, NULL);
    serial_matrix_multiply(matrix_A, matrix_B, matrix_C);
    gettimeofday(&t2, NULL);
    double time_spend = (t2.tv_sec - t1.tv_sec)+(double)(t2.tv_usec - t1.tv_usec)/1000000.0;
    printf("Time cost is %.5f for dimension %i\n", time_spend, dimension);
}

void readMatrix(int dimension, matrix_t* matrix_A, matrix_t* matrix_B)
{
    string dim = to_string(dimension);
    string filename = "matrix"+ dim + "x"+ dim + ".txt";
    load_matrix(filename, dimension,  matrix_A, matrix_B);
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
