import sys

name = str(argv[0])
message = str(argv[1])

with open(name, "w") as file:
    file.write(message)