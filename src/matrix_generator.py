import random
import argparse


def matrix_generator(matrices, in_script):
    index = ['A', 'B']
    if in_script:
        for i in range(len(matrices)):
            filename = str(matrices[i]) + 'x' + str(matrices[i]) + '.txt'
            with open('../data/matrix'+filename, 'w') as f:
                for counter in range(2):
                    f.write("matrix " + index[counter] + ":"+"\n")
                    for j in range(int(matrices[i])):
                        for k in range(int(matrices[i])):
                            f.write(str(random.randint(0, 1000))+" ")
                        f.write("\n")
                    f.write("\n")
            f.close()
        return

    for i in range(len(matrices)):
        filename = str(matrices[i]) + 'x' + str(matrices[i]) + '.txt'
        with open('data/matrix'+filename, 'w') as f:
            for counter in range(2):
                f.write("matrix " + index[counter] + ":"+"\n")
                for j in range(int(matrices[i])):
                    for k in range(int(matrices[i])):
                        f.write(str(random.randint(0, 1000))+" ")
                    f.write("\n")
                f.write("\n")
        f.close()


if __name__ == "__main__":
    matrices = [4, 16, 64, 256, 1024, 2048]
    parser = argparse.ArgumentParser()
    parser.add_argument('-n', '--newDimension', default=None, type=int,
                        help='Add two new matrices with dimension defined by yourself', required=False)
    parser.add_argument('--in_script', action='store_true', default=False,
                        help='Indicate that the program is running in script in the directory ./parallel')
    args = parser.parse_args()
    if args.newDimension == None:
        matrix_generator(matrices, args.in_script)
    else:
        matrix_generator([args.newDimension], args.in_script)
