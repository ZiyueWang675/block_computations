# Matrix multiplication using block computations

## Introduction

- We implemented block striped decomposition method using Pthreads, OpenMP and MPI.
- We also implemented Fox's algorithm and Cannon's algorithm using MPI.
- Test results are in the folder `src/output`.
- Graphs of the test results are in the folder `graph`.
- Note that our program only support tests with square matrices

## Generate new n dimension matrix

- You can start generate new n by n matrices
  - Go to the `src` folder.
  - Run `python3 matrix_generator.py -n <MATRIX DIMENSION>`
  - The new matrix is in the folder `src/data`

## Run test

- In the folder `src/parallel`, see what `test.sh` does
- Modify `test.sh` so that you can include new matrices as input
- Then run `bash test.sh` and the results should be generated in the folder `src/output`

## Project Tree

```
block_computations
├─ README.md
├─ cpu_info.txt
├─ graph
│  ├─ MPI.png
│  ├─ MPI_9.png
│  ├─ memory
│  │  ├─ cannon.png
│  │  ├─ fox.png
│  │  └─ mpi.png
│  ├─ speedup.png
│  ├─ threads.png
│  ├─ total.png
│  └─ total_log.png
└─ src
   ├─ data
   │  ├─ matrix1024x1024.txt
   │  ├─ matrix128x128.txt
   │  ├─ matrix16x16.txt
   │  ├─ matrix1800x1800.txt
   │  ├─ matrix2048x2048.txt
   │  ├─ matrix256x256.txt
   │  ├─ matrix2700x2700.txt
   │  ├─ matrix32x32.txt
   │  ├─ matrix4x4.txt
   │  ├─ matrix512x512.txt
   │  ├─ matrix64x64.txt
   │  └─ matrix900x900.txt
   ├─ matrix_generator.py
   ├─ output
   │  ├─ mpi.txt
   │  ├─ mpi_9_processors.txt
   │  ├─ mpi_cannon.txt
   │  ├─ mpi_cannon_9_processors.txt
   │  ├─ mpi_fox.txt
   │  ├─ mpi_fox_9_processors.txt
   │  ├─ omp.txt
   │  ├─ pthread.txt
   │  └─ serial.txt
   └─ parallel
      ├─ makefile
      ├─ mpi.cpp
      ├─ mpi_cannon.cpp
      ├─ mpi_fox.cpp
      ├─ omp.cpp
      ├─ pthread.cpp
      ├─ serial.cpp
      ├─ test.sh
      └─ util
         ├─ matrix.cpp
         ├─ matrix.hpp
         ├─ parser.cpp
         └─ parser.hpp

```
