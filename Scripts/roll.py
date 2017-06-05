import random
import sys

result = random.randint(1, int(sys.argv[1]))

f = open("./link/linkToPython.txt", 'a')
f.write("You rolled " + str(result) + "!\n")
f.close()

