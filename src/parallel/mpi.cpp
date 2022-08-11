#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <math.h>
#include "util/parser.hpp"
#include "util/matrix.hpp"
using namespace std;

int dimension;
matrix_t* matrix_A;
matrix_t* matrix_B;

void Usage(char prog_name[]);

void readMatrix(
      int dimension      /* in  */,
      matrix_t* matrix_A /* out */,
      matrix_t* matrix_B /* out */);

void Get_args(
      char*    argv[]      /* in  */,
      int*     dimension   /* out */);

int main(int argc, char* argv[])
{
    if (argc != 2) Usage(argv[0]); 
    Get_args(argv, &dimension);
    matrix_A = make_matrix(dimension,dimension);
    matrix_B = make_matrix(dimension,dimension);
    readMatrix(dimension, matrix_A, matrix_B);
}

void Usage(char prog_name[])
{
   fprintf(stderr, "usage: %s ", prog_name); 
   fprintf(stderr, "<matrix_dimension>\n");
   exit(0);
}

void readMatrix(int dimension, matrix_t* matrix_A, matrix_t* matrix_B)
{
    
    string dim = to_string(dimension);
    string filename = "matrix"+ dim + "x"+ dim + ".txt";
    printf("%s\n", filename.c_str());
    load_matrix(filename, dimension,  matrix_A, matrix_B);
}

void Get_args(char* argv[], int* dimension)
{
    *dimension = strtol(argv[1], NULL, 10);
}