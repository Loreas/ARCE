import sys

print("Template command script.\nArguments: ")
for arg in sys.argv:
	if arg != sys.argv[0]:
		print(arg)