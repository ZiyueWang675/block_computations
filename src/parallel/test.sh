#!/bin/bash
declare -a arr=(4 16 32 64 128 256 512 1024 2048)

for i in "${arr[@]}"
do 
    python3 ../matrix_generator.py -n $i --in_script 
done

make

echo "Serial Program is running"
echo "Serial Program: " > ../output/serial.txt
for i in "${arr[@]}"
do 
    ./serial $i
done >> ../output/serial.txt
echo "Serial Program is finished"

echo "Pthread Program is running"
echo "Pthread Program: " > ../output/pthread.txt
for i in "${arr[@]}"
do 
    for j in {2..20}
    do
        ./pthread $i $j
    done
done >> ../output/pthread.txt
echo "Pthread Program is finished"

echo "OpenMP Program is running"
echo "OpenMP Program: " > ../output/omp.txt
for i in "${arr[@]}"
do 
    for j in {2..20}
    do
        ./omp $i $j
    done
done >> ../output/omp.txt
echo "OpenMP Program is finished"

echo "MPI Program is running"
echo "MPI Program: " > ../output/mpi.txt
for i in "${arr[@]}"
do 
    for j in 2 4 8
    do
        mpiexec -n $j ./mpi $i
    done
done >> ../output/mpi.txt
echo "MPI Program is finished"

echo "MPI Fox Program is running"
echo "MPI Fox Program: " > ../output/mpi_fox.txt
for i in "${arr[@]}"
do 
    mpiexec -n 4 ./mpi_fox $i
done >> ../output/mpi_fox.txt
echo "MPI Fox Program is finished"

echo "MPI Cannon Program is running"
echo "MPI Cannon Program: " >  ../output/mpi_cannon.txt
for i in "${arr[@]}"
do 
    mpiexec -n 4 ./mpi_fox $i
done >> ../output/mpi_cannon.txt
echo "MPI Cannon Program is finished"

make clean
