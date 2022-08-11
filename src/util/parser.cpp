#include "matrix.hpp"
#include <iostream>
#include <fstream>

using namespace std;

void load_matrix(string name_of_file, matrix_t* matrix_A, matrix_t* matrix_B){
    
    string raw = "../data/";
    raw = raw + name_of_file;
    ifstream input_file (raw);

    //error
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '"
             << name_of_file << "'" << endl;
        exit(1);
    }
    
    //get size of matrix
    string num; 
    for (int i = 6; i < name_of_file.length(); i++){
        if(isdigit(name_of_file[i])){
            num = num + name_of_file[i];
        }else{
            break;
        }
    }
    int size = stoi(num);
    

    //first we get the label of matrix A
    string line;
    getline(input_file, line);

    //get number for matrix A
    int counter = 0;
    int number;
    int row = 0;
    int col = 0;
    while(input_file >> number){
        element(matrix_A, row, col) = number;
        col += 1;
        if(col == size){
            col = 0;
            row += 1;
        }
        
        counter += 1;
        if(counter == pow(size,2)){
            break;
        }
    }
    //skip current line
    getline(input_file, line);
    //skip empty line
    getline(input_file, line);
    //Then we get the label of matrix B
    getline(input_file, line);

    row = 0;
    col = 0;
    while(input_file >> number){
        element(matrix_B, row, col) = number;
        col += 1;
        if(col == size){
            col = 0;
            row += 1;
        }
    }

}


int main(){
    matrix_t *my_matrix_A;
    matrix_t *my_matrix_B;
    
    

    string matrix_name = "matrix64x64.txt";

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