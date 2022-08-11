import random
import argparse
import numpy as np
import os

def matrix_generator(matrix_A, matrix_B):
    parent_dir = os.getcwd() + '/../data'
    for i in range(len(matrix_A)):
        directory = 'matrices'+ str(i)
        path = os.path.join(parent_dir, directory)
        os.mkdir(path)
        filename = 'matrixA_'+ str(matrix_A[i][0]) + 'x'+ str(matrix_A[i][1]) + '.txt'
        with open('../data/matrices'+ str(i) +'/'+filename, 'w') as f:
            f.write(str(matrix_A[i][0])+"x"+ str(matrix_A[i][1]) + " matrix:\n")
            for j in range(int(matrix_A[i][0])):
                for k in range(int(matrix_A[i][1])):
                    f.write(str(random.randint(0, 1000))+" ")
                f.write("\n")
    f.close()
    for i in range(len(matrix_B)):
        filename = 'matrixB_'+str(matrix_B[i][0]) + 'x'+ str(matrix_B[i][1]) + '.txt'
        with open('../data/matrices'+ str(i) +'/'+filename, 'w') as f:
            f.write(str(matrix_B[i][0])+"x"+ str(matrix_B[i][1]) + " matrix:\n")
            for j in range(int(matrix_B[i][0])):
                for k in range(int(matrix_B[i][1])):
                    f.write(str(random.randint(0, 1000))+" ")
                f.write("\n")
    f.close()


if __name__ == "__main__":
    matrix_A = [[4,4],[16,16],[64,64],[256,256],[1024,1024],[2048,2048]]
    matrix_B = [[4,4],[16,16],[64,64],[256,256],[1024,1024],[2048,2048]]
    parser = argparse.ArgumentParser()
    parser.add_argument('-a','--matrixA', default=None, nargs="+", help='Add a new radom 2d matrix A defined by entering the size (Column number first)', required=False)
    parser.add_argument('-b','--matrixB', default=None, nargs="+", help='Add a new radom 2d matrix B defined by entering the size (Column number first)', required=False)
    args = parser.parse_args()
    if args.matrixA and args.matrixB:
        if(len(args.matrixA) != 2):
            print("ERROR: Matrix A is not 2D matrix! Program exit.")
            exit()
        if(len(args.matrixB) != 2):
            print("ERROR: Matrix B is not 2D matrix! Program exit.")
            exit()
        if(int(args.matrixA[0]) != int(args.matrixA[1])):
            print("ERROR: Matrix A is not square matrix! Program exit.")
            exit()
        if(int(args.matrixB[0]) != int(args.matrixB[1])):
            print("ERROR: Matrix B is not square matrix! Program exit.")
            exit()
        if(int(args.matrixA[0]) != int(args.matrixB[1])):
            print("ERROR: Matrix A and B cannot be multiplied! Program exit.")
            exit()
        newMatrixA = np.asarray(args.matrixA)
        newMatrixB = np.asarray(args.matrixB)
        matrix_A.append(newMatrixA)
        matrix_B.append(newMatrixB)
    matrix_generator(matrix_A, matrix_B)