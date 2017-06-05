import random
import sys

int = random.randint(1,sys.argv[2])

f = open("./../link/linkToPyhton.txt", 'a')
f.write("rolled " + int)
f.close()

