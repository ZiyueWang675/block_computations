#include "parser.hpp"

using namespace std;

void load_matrix(string name_of_file, int size, matrix_t *matrix_A,
                 matrix_t *matrix_B) {

  string raw = "../data/";
  raw = raw + name_of_file;
  ifstream input_file(raw);

  // error
  if (!input_file.is_open()) {
    cerr << "Could not open the file - '" << name_of_file << "'" << endl;
    exit(1);
  }

  // first we get the label of matrix A
  string line;
  getline(input_file, line);

  // get number for matrix A
  int counter = 0;
  int number;
  int row = 0;
  int col = 0;
  while (input_file >> number) {
    element(matrix_A, row, col) = number;
    col += 1;
    if (col == size) {
      col = 0;
      row += 1;
    }

    counter += 1;
    if (counter == pow(size, 2)) {
      break;
    }
  }
  // skip current line
  getline(input_file, line);
  // skip empty line
  getline(input_file, line);
  // Then we get the label of matrix B
  getline(input_file, line);

  row = 0;
  col = 0;
  while (input_file >> number) {
    element(matrix_B, row, col) = number;
    col += 1;
    if (col == size) {
      col = 0;
      row += 1;
    }
  }
}