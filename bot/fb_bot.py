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

b = bot(sys.argv[0], sys.argv[1])
b.listen()

