import random
import sys

result1 = random.randint(1, int(sys.argv[1]))
result2 = random.randint(1, int(sys.argv[1]))

f = open("./link/linkToPython.txt", 'a')
f.write("You rolled " + str(result1) + " and " +str(result2) + "!\n")
f.close()

