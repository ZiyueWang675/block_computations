#include "matrix.hpp"
#include <iostream>
#include <fstream>

using namespace std;

matrix_t* load_matrix(string name_of_file){
    matrix_t *my_matrix_A;
    my_matrix_A = make_matrix(2, 2);
    string raw = "/../../matrixA/";
    ifstream input_file ("/../../matrixA/");
    string line;
    getline(input_file, line);
    return my_matrix_A;
}


int main(){
    matrix_t *my_matrix_A;
    my_matrix_A = make_matrix(2, 2);
    element(my_matrix_A, 0, 0) = 0;
    element(my_matrix_A, 0, 1) = 1;
    element(my_matrix_A, 1, 0) = 2;
    element(my_matrix_A, 1, 1) = 3;

    print_matrix(my_matrix_A);

    free_matrix(my_matrix_A);

    return 0;
}

