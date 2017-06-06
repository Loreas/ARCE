import sys
import urllib
import os
import datetime
fro

output = sys.argv[1] + "\n"
for i in sys.argv[2:]:
    output += i + " "


pastebin_vars = {'api_dev_key': '57fe1369d02477a235057557cbeabaa1', 'api_option': 'paste', 'api_paste_code': output}
response = urllib.urlopen('http://pastebin.com/api/api_post.php', urllib.urlencode(pastebin_vars))
url = response.read()
