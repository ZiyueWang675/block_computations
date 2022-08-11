#include <iostream>
#include <fstream>
#include "parser.hpp"
#include "matrix.hpp"
using namespace std;


int main(){
    matrix_t *my_matrix_A;
    matrix_t *my_matrix_B;
    
    

    string matrix_name = "matrix256x256.txt";

    //get size of matrix
    string num; 
    for (int i = 6; i < matrix_name.length(); i++){
        if(isdigit(matrix_name[i])){
            num = num + matrix_name[i];
        }else{
            break;
        }
    }
    int size = stoi(num);

    my_matrix_A = make_matrix(size,size);
    my_matrix_B = make_matrix(size,size);

    load_matrix(matrix_name, my_matrix_A,my_matrix_B);
    //matrix_t *my_matrix_A;
    // my_matrix_A = make_matrix(2, 2);
    // element(my_matrix_A, 0, 0) = 0;
    // element(my_matrix_A, 0, 1) = 1;
    // element(my_matrix_A, 1, 0) = 2;
    // element(my_matrix_A, 1, 1) = 3;

    print_matrix(my_matrix_A);
    print_matrix(my_matrix_B);

    free_matrix(my_matrix_A);
    free_matrix(my_matrix_B);
    return 0;
}