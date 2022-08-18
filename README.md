# Matrix multiplication using block computations


## Introduction
* We implemented block striped decomposition method using Pthreads, OpenMP and MPI.
* We also implemented Fox's algorithm and Cannon's algorithm using MPI.
* Test results are in the folder ``src/output``.
* Graphs of the test results are in the folder ``graph``.
* Note that our program only support tests with square matrices

## Generate new n dimension matrix
* You can start generate new n by n matrices
    * Go to the ``src`` folder.
    * Run ``python3 matrix_generator.py -n <MATRIX DIMENSION>``
    * The new matrix is in the folder ``src/data``

## Run test
* In the folder ``src/parallel``, see what ``test.sh`` does
* Modify ``test.sh`` so that you can include new matrices as input
* Then run ``bash test.sh`` and the results should be generated in the folder ``src/output``
