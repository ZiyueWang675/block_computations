# Matrix multiplication using block computations


## Introduction
* We implemented block striped decomposition method using pthread, OpenMP and MPI.
* We also implemented Fox's and Cannon's algorithm for MPI.
* Test results are in the folder ``src/output``.
* Graphs of the results are in the folder ``graph``.


## Generate new n dimension matrix
* You can start generate new n by n matrices
    * Go to the ``src`` folder.
    * Run ``python3 matrix_generator.py -n <MATRIX DIMENSION>``
    * The new matrix is in the folder ``src/data``


## Run test
* In the folder ``src/parallel``, see what ``test.sh`` does
* Modify ``test.sh`` so that you can include new matrices as input
* Then run ``bash test.sh``
