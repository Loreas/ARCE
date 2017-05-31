import fbchat
import sys

class bot(fbchat.Client):
    def __init__(self,email, password,debug=True, user_agent=False,message_done=False):
        fbchat.Client.__init__(self,email, password,debug,user_agent)
        self.message_done=message_done

    def on_message(self, mid, author_id, author_name, message, metadata):
        self.markAsDelivered(author_id, mid)
        self.markAsRead(author_id)

        f = open("../link/link.txt", 'w')
        f.write(message)
        f.close()
        """Checkfile for updates."""
        with open("../../link/linkToPython.txt") as f:
            content = f.readline()
        for i in content:
            self.send(sys.argv[2], i, False)



print(sys.argv[1])
b = bot(sys.argv[1], sys.argv[2])
b.listen()

