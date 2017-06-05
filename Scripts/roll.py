import random
import sys

int = random.randint(1,sys.argv[2])

f = open("./../link/linktopyhton.txt", 'a')
f.write("roll " + int)
f.close()

