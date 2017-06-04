
import fbchat
import sys
import datetime
import os.path
from subprocess import call

class bot(fbchat.Client):
    def __init__(self, email, password, debug=True, info_log=True, user_agent=None, message_done=False, logging = False):
        fbchat.Client.__init__(self, email, password, debug, info_log, user_agent)
        self.logging = logging
        self.message_done=message_done

    def startlogging(self, author_name, message):
        call(["mkdir", "log"])
        c = datetime
        new = open("log/" + str(c.date.today()), "a")
        timestamp = datetime.datetime.now().time().strftime("%H:%M")
        new.write(str(timestamp+ " - " + author_name+ ": " + message))

    def log(self,arg,  message):

        if(arg == "start"):
            self.logging = True
        elif(arg == 'stop'):
            self.logging = False
        else:
            if(os.path.isfile("log/"+arg+".txt")):
                self.sendLocalImage(sys.argv[3],None, False,"log/"+arg+".txt")
            else:
                self.send(sys.argv[3], "logfile doesn't exist",False)





    def on_message(self, mid, author_id, author_name, message, metadata):
        self.markAsDelivered(author_id, mid)
        self.markAsRead(author_id)

        print(message)
        f = open("./link/link.txt", 'a')
        if message[0] == '!':
        	f.write(message[1:])
        f.close()

        if message == "!exit":
            self.stop_listening()

        if(self.logging):
            self.startlogging(author_name, message)



        # Checkfile for updates

        with open("./link/linkToPython.txt") as f:
            content = f.readline()
        for i in content:
            if(i[:3] == "log"):
                arg= i[3:]
                self.log(arg, message)
            elif(i[:7] == "adduser"):
                arg = i[8:]
                id = self.getUsers(arg)[0]
                self.add_users_to_chat(sys.argv[3], id)

            elif(i[:10] == "removeuser"):
                arg = i[10:]
                id = self.getUsers(arg)
                self.remove_user_from_chat(id)

            else:
                self.send(sys.argv[3], i, False)


        #clear file
        f = open("./link/linkToPython.txt", 'w')
        f.write("")
        f.close()







username = sys.argv[1]
password = sys.argv[2]
groupID = sys.argv[3]

# "python3 bot/fb_bot.py c588808@mvrht.net ARCE123 1230204977079375"

b = bot(username, password, False)

b.listen()