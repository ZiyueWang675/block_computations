#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <math.h>
#include <mpi.h> 
#include <sys/time.h>
#include "util/parser.hpp"
#include "util/matrix.hpp"
using namespace std;

int dimension;
matrix_t* matrix_A;
matrix_t* matrix_B;
matrix_t* matrix_C;

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
    matrix_C = make_matrix(dimension,dimension);
    readMatrix(dimension, matrix_A, matrix_B); 
    
    //for timer
    struct timeval t1, t2;
    gettimeofday(&t1, NULL);

    /* Start up MPI */
    MPI_Init(NULL, NULL);
    /* Get the number of processes */
    int my_size;
    MPI_Comm_size(MPI_COMM_WORLD, &my_size); 
    /* Get my rank among all the processes */
    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

    int local_stripes = 1;
    int row_number = 0;
    local_stripes = (my_rank!=my_size-1) ? (dimension/my_size) : (dimension-(dimension/my_size)*(my_size - 1));

    //Threads start to work!
    for(int i = 0; i < local_stripes; i++){
        row_number = (dimension/my_size)*my_rank + i;
        for(int k = 0; k < dimension; k++){
            for(int j = 0; j < dimension; j++){
                element(matrix_C, row_number, j) += element(matrix_A, row_number, k) * element(matrix_B, k, j);
            }
        }
    }

    //make a local array for MPI send
    int size_tmp = 0;
    if(my_rank != my_size-1){
        size_tmp = (dimension / my_size) * dimension;
    }else{
        size_tmp = (dimension - (dimension / my_size) * (my_size - 1))* dimension;
    }
    int my_result[size_tmp];

    int col_num = 0;
    int temp = (dimension / my_size) * my_rank;
    for (int i = 0; i < size_tmp; i++){
        my_result[i] = element(matrix_C,temp,col_num);
        col_num += 1;
        if(col_num == dimension){
            temp += 1;
            col_num = 0;
        }
    }

        // for other processes, send their result of matrix C to process 0
    if (my_rank != 0){
        MPI_Send(my_result, size_tmp, MPI_INT, 0, 0, MPI_COMM_WORLD);    
 
    }

    //insert the value from other porcess to process 0
    if(my_rank == 0){
        int a = (dimension / my_size);
        for (int b = 1; b < my_size-1; b+=1){
            MPI_Recv(my_result, size_tmp, MPI_INT, b, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            for (int i = 0; i < size_tmp; i++){
                cout << my_result[i] << endl;
            }

            int col_temp = 0;
            for (int i = 0; i < size_tmp; i++){
                element(matrix_C,a,col_temp) = my_result[i];
                col_temp += 1;
                if(col_temp == dimension){
                    a += 1;
                    col_temp = 0;
                }
            }
        }

        size_tmp = (dimension - (dimension / my_size) * (my_size - 1))* dimension;
        MPI_Recv(my_result, size_tmp, MPI_INT, my_size-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        int col_temp = 0;
        for (int i = 0; i < size_tmp; i++){
            element(matrix_C,a,col_temp) = my_result[i];
            col_temp += 1;
            if(col_temp == dimension){
                a += 1;
                col_temp = 0;
            }
        }
    }




    gettimeofday(&t2, NULL);
    double time_spend = (t2.tv_sec - t1.tv_sec)+(double)(t2.tv_usec - t1.tv_usec)/1000000.0;
    if(my_rank==0){
        print_matrix(matrix_C);
        printf("Time needed is %f\n", time_spend);
    }
    

    /* Shut down MPI */
    MPI_Finalize();
    return 0;
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
