
import fbchat
import sys
import datetime
import os
from subprocess import call
import urllib
import logging
import requests

log = logging.getLogger("client")
log.setLevel(logging.DEBUG)

class bot(fbchat.Client):
    def __init__(self, email, password, debug=True, info_log=True, user_agent=None, message_done=False, logging = False):
        fbchat.Client.__init__(self, email, password, debug, info_log, user_agent)
        self.logging = logging
        self.message_done=message_done
        self.authorname = "Empty"
        self.message = "Empty"
        self.groupID = sys.argv[3]

    def check_file(self, path):
        # check for udpdate in the file
        while(os.stat(path).st_size == 0):
            continue

        f = open(path)
        for line in f:
            content = line.split(' ', 1)
            if (content[0] == "log"):
                arg = content[1][:-1]
                self.log(arg, self.message, self.authorname)
                print("Log", arg)
            elif (content[0] == "adduser"):
                arg = content[1][:-1]
                userId = self.getUsers(arg)[0]
                self.add_users_to_chat(self.groupID, userId)
                print("Adding user", arg)
            elif (content[0] == "removeuser"):
                arg = content[1][:-1]
                userId = self.getUsers(arg)[0]
                self.remove_user_from_chat(self.groupID, userId)
                print("Removing user", arg)
            else:
                self.send(self.groupID, line, False)
        f.close()

        # clear file
        f = open("./link/linkToPython.txt", 'w')
        f.write("")
        f.close()


    def startlogging(self, author_name, message):
        call(["mkdir", "log"])
        c = datetime
        new = open("./log/" + str(c.date.today()) +".txt", "a")
        timestamp = datetime.datetime.now().time().strftime("%H:%M")
        new.write(str("\nLog on " + std(c.date.today())))
        new.write(str("\n" + timestamp + " - " + author_name + ": " + message))

    def log(self, arg,  message, author_name):

        logPath = "./log/" + arg + ".txt";

        if(str(arg) == "start"):
            self.logging = True
            self.startlogging(author_name, message)
            self.send(groupID, "Alright, logging all messages.", False)
        elif(str(arg) == 'stop'):
            self.logging = False
            self.send(groupID, "No longer keeping a log.", False)
        elif(str(arg) == "list"):
            loglist = os.listdir("./log")
            out = ""
            for s in loglist:
                out += s[:-4] + "\n"
            self.send(self.groupID, out, False)
        else:
            if(os.path.isfile(logPath)):
                #self.sendLocalImage(self.groupID, None, False, logPath)
                logf = open(logPath, 'r')
                content = logf.read()
                pastebin_vars = {'api_dev_key':'1da7dc400eecfc58326146f50184741d','api_option':'paste','api_paste_code': content}
                response = urllib.urlopen('http://pastebin.com/api/api_post.php', urllib.urlencode(pastebin_vars))
                url = response.read()
                self.send(self.groupID, str(url), False)
            else:
                self.send(self.groupID, ("Logfile '" + logPath + "' doesn't exist."), False)



    def on_message(self, mid, author_id, author_name, message, metadata):
        self.markAsDelivered(author_id, mid)
        self.markAsRead(author_id)
        author_name = self.getUserInfo(author_id)['name']
        self.authorname = author_name
        self.message = message

        if message == "!exit":
            self.stop_listening()
            f = open("./link/link.txt", 'a')
            f.write(message[1:])
            f.close()
            self.send(sys.argv[3], "System shutting down, goodbye!", False)
            return

        print(">", author_name, ":", message)
        if message[0] == '!':
            f = open("./link/link.txt", 'a')
            f.write(message[1:])
            f.write("\n")
            f.close()
            self.check_file("./link/linkToPython.txt")

        if(self.logging):
            self.startlogging(author_name, message)


    def do_one_listen(self, markAlive=True):
        """Does one cycle of the listening loop.
        This method is only useful if you want to control fbchat from an
        external event loop."""

        try:
            if markAlive: self.ping(self.sticky)
            try:
                content = self._pullMessage(self.sticky, self.pool)
                if content: self._parseMessage(content)
            except requests.exceptions.RequestException as e:
                pass
        except KeyboardInterrupt:
            self.listening = False
        except requests.exceptions.Timeout:
            pass


    def listen(self, markAlive=True):
        self.start_listening()

        log.info("Listening...")
        # Listen loop & checking for commands in file
        while self.listening:
            self.do_one_listen(markAlive)

        self.stop_listening()



username = sys.argv[1]
password = sys.argv[2]
groupID = sys.argv[3]

# "python3 fb_bot.py c588808@mvrht.net ARCE123 1230204977079375"

b = bot(username, password, False)
b.send(groupID,"ARCE-bot at your service!",False)
b.listen()