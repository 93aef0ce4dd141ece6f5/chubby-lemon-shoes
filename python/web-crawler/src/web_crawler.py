###################################
#                                 #
#     Creepy Web Crawler          #
#                                 #
#     Author: ManWuzi             #
#                                 #
###################################


import urllib2, os
import urlparse, sys
from bs4 import BeautifulSoup
import socket, socks

def usingTor():
	socks.setdefaultproxy(socks.PROXY_TYPE_SOCKS5, "127.0.0.1", 9050)
	socket.socket = socks.socksocket
	print "Your tor IP is: " + str(urllib2.urlopen("http://icanhazip.com").read())

if len(sys.argv) < 2:
	print "Usage: %s <url>" %sys.argv[0]
	sys.exit()

tor = raw_input("Do you want to use tor[Y/n]: ")

if tor.lower() == "n":
	pass
elif tor.lower() == "y":
	usingTor()
elif tor == "":
	usingTor()
else:
	print "[*] Invalid option!!\n[*]Exiting..."
	sys.exit()	
	 
url = sys.argv[1]

if os.path.exists(os.path.join("crawled_sites", url[7:])) == False:
	f = open(os.path.join("crawled_sites", url[7:]), "w")
	f.close()

urls = []
visited = []

g = open(os.path.join("crawled_sites", url[7:]), "a")

urls.append(url)

user_agent = "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:40.0) Gecko/20100101 Firefox/40.1"

headers = {'User-Agent': user_agent}

while len(urls)>0:
	try:
		req = urllib2.Request(urls[0], None, headers)
		html = urllib2.urlopen(req)
	except urllib2.HTTPError, e:
		print urls[0] + "-->" + str(e.code)
		continue
		
	soup = BeautifulSoup(html.read())

	for tag in soup.findAll('a', href=True):
		tag['href'] = urlparse.urljoin(url, tag['href'])

		if url in tag['href'] and tag['href'] not in visited:
			g.write(tag['href']+"\n")
			urls.append(tag['href'])
			visited.append(tag['href'])

	urls.pop(0)

g.close()
