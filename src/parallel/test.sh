#!/bin/bash
declare -a arr=(4 16 32 64 128 256 512 1024 2048 4096)
declare -a arr_serial=(4 16 32 64 128 256 512 1024 2048 4096 900 1800 2700 3600 4500 6300)
declare -a arr9=(900 1800 2700 3600 4500 6300)

make


echo "Pthread Program is running"
echo "Pthread Program: " > ../output/pthread.txt
for i in "${arr[@]}"
do 
    for j in 2 4 8 16 20
    do
        ./pthread $i $j
    done
done >> ../output/pthread.txt
echo "Pthread Program is finished"

echo "OpenMP Program is running"
echo "OpenMP Program: " > ../output/omp.txt
for i in "${arr[@]}"
do 
    for j in 2 4 8 16 20
    do
        ./omp $i $j
    done
done >> ../output/omp.txt
echo "OpenMP Program is finished"



make clean
