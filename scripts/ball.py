import random

ball = {1 : "It is certain\n", 2 : "It is decidedly so\n", 3 : "Without a doubt\n",
        4 : "Yes, definitely\n", 5 : "You may rely on it\n",
        6 : "As I see it, yes\n", 7 : "Most likely\n", 8 : "Outlook good\n",
        9 : "Yes\n", 10 : "Signs point to yes\n", 11 : "Reply hazy, try again\n",
        12 : "Ask again later\n", 13 : "Better not tell you now\n", 
        14 : "Cannot predict now\n", 15 : "Concentrate and ask again\n",
        16 : "Don't count on it\n", 17 : "My reply is now\n",
        18 : "My sources say no\n" , 19 : "Outlook not so good\n",
        20 : "Very doubtful\n"}

chance = random.randint(1, 20)

f = open("./link/linkToPython.txt", 'a')
f.write(ball[chance])
f.close()
