import sys
import urllib
import os
import datetime

output = sys.argv[1] + "\n"
for i in sys.argv[2:]:
    output += i + " "


pastebin_vars = {'api_dev_key': '1da7dc400eecfc58326146f50184741d', 'api_option': 'paste', 'api_paste_code': output}
response = urllib.urlopen('http://pastebin.com/api/api_post.php', urllib.urlencode(pastebin_vars))
url = response.read()

f = open("./link/linkToPython.txt", 'a')
f.write(str("Your note: " + url))
f.close()