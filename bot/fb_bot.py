
import fbchat
import sys
import datetime
import os.path
from subprocess import call
import urllib


class bot(fbchat.Client):
    def __init__(self, email, password, debug=True, info_log=True, user_agent=None, message_done=False, logging = False):
        fbchat.Client.__init__(self, email, password, debug, info_log, user_agent)
        self.logging = logging
        self.message_done=message_done
        self.groupID = sys.argv[3]

    def startlogging(self, author_name, message):
        call(["mkdir", "log"])
        c = datetime
        new = open("log/" + str(c.date.today()), "a")
        timestamp = datetime.datetime.now().time().strftime("%H:%M")
        new.write(str("\n" + timestamp+ " - " + author_name + ": " + message))

    def log(self, arg,  message, author_name):

        print(arg)
        logPath = "./log/" + arg;

        if(arg == "start"):
            self.logging = True
            self.startlogging(author_name, message)
            self.send(groupID, "Alright, logging all messages.", False)
        elif(arg == 'stop'):
            self.logging = False
            self.send(groupID, "No longer keeping a log.", False)
        else:
            if(os.path.isfile(logPath)):
                #self.sendLocalImage(self.groupID, None, False, logPath)
                logf = open(logPath, 'r')
                content = logf.read()
                pastebin_vars = {'api_dev_key':'57fe1369d02477a235057557cbeabaa1','api_option':'paste','api_paste_code': content}
                response = urllib.urlopen('http://pastebin.com/api/api_post.php', urllib.urlencode(pastebin_vars))
                url = response.read()
                self.send(self.groupID, str(url), False)
            else:
                self.send(self.groupID, ("Logfile '" + logPath + "' doesn't exist."), False)





    def on_message(self, mid, author_id, author_name, message, metadata):
        self.markAsDelivered(author_id, mid)
        self.markAsRead(author_id)

        author_name = self.getUserInfo(author_id)['name']

        print(">", author_name, ":", message)
        if message[0] == '!':
            f = open("./link/link.txt", 'a')
            f.write(message[1:])
            f.write("\n")
            f.close()

        if(self.logging):
            self.startlogging(author_name, message)

        # Checkfile for updates
        f = open("./link/linkToPython.txt")
        for line in f:
            if(line[:3] == "log"):
                arg = line[4:-1]
                self.log(arg, message, author_name)
                print("Log", arg)
            elif(line[:7] == "adduser"):
                arg = line[8:-1]
                userId = self.getUsers(arg)[0]
                self.add_users_to_chat(self.groupID, userId)
                print("Adding user", arg)
            elif(line[:10] == "removeuser"):
                arg = line[11:-1]
                userId = self.getUsers(arg)[0]
                self.remove_user_from_chat(self.groupID, userId)
                print("Removing user", arg)
            else:
                self.send(sys.argv[3], line, False)
        f.close()

        #clear file
        f = open("./link/linkToPython.txt", 'w')
        f.write("")
        f.close()

        if message == "!exit":
            self.send(sys.argv[3], "System shutting down, goodbye!", False)
            self.stop_listening()





username = sys.argv[1]
password = sys.argv[2]
groupID = sys.argv[3]

# "python3 bot/fb_bot.py c588808@mvrht.net ARCE123 1230204977079375"

b = bot(username, password, False)
b.send(groupID,"ARCE-bot at your service!",False)
b.listen()