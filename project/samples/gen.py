import random
import sys

if len(sys.argv) == 5:
    f = open(sys.argv[4], 'w')
    f.write(sys.argv[1] + '\n')
    for i in range(int(sys.argv[1])):
        f.write(str(random.randint(int(sys.argv[2]), int(sys.argv[3]))) + '\n')
    print("Successfully written {} random values from {} to {} into {}".format(sys.argv[1], sys.argv[2], sys.argv[3],
                                                                               sys.argv[4]))
else:
    print("example: python3 gen.py 1000 10 100 sample1.txt")