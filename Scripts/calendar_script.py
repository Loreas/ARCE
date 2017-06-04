import sys
name = str(sys.argv[0])
date= sys.argv[1]
time = sys.argv[2]
subject = sys.argv[3]

with open(name, 'a') as file: 
    file.write(str(date)+ ' | ' + str(time) + ' | ' + str(subject))
