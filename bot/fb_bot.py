import fbchat
import sys
import datetime
from subprocess import call

class bot(fbchat.Client):
    def __init__(self,email, password,debug=True, user_agent=False,message_done=False, logging = False):
        fbchat.Client.__init__(self,email, password,debug,user_agent)
        self.logging = logging
        self.message_done=message_done

    def logmessage(self, message):
        call(["mkdir","log"])
        c = datetime
        new = open(str(c.date.today()), "a")
        new.write(message)


    def on_message(self, mid, author_id, author_name, message, metadata):
        self.markAsDelivered(author_id, mid)
        self.markAsRead(author_id)

        print(message)
        f = open("./link/link.txt", 'a')
        f.write(message)
        f.close()

        if message == "!exit":
            self.stop_listening()


        # Checkfile for updates

        with open("./link/linkToPython.txt") as f:
            content = f.readline()
        for i in content:
            if(i == "start_logging"):
                self.logging = True
            elif(i == "stop_logging == false"):
                self.logging = False
            else:
                self.send(sys.argv[3], i, False)


        #clear file
        f = open("./link/linkToPython.txt", 'w')
        f.write("")
        f.close()


        if(self.logging == True):
            self.logmessage(message)






print(sys.argv[2])
b = bot(str(sys.argv[1]), str(sys.argv[2]), False)

b.listen()

