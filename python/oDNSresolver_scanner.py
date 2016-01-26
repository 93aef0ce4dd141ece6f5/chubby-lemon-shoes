#!/usr/bin/env python
#To use: nmap -iR 1000 -sU -p 53 -n -T5 -oG - --open | grep /open/ | ./oDNSresolver_scanner.py
#You can put that in a bash script or something.
import logging
logging.getLogger("scapy.runtime").setLevel(logging.ERROR)
from scapy.all import *

stdin = []
iplist = []
for line in sys.stdin:
	stdin.append(str(line))

for line in stdin:
	line = line.split(":")[1].split("(")[0].strip(" ")
	iplist.append(line)
	
for ip in iplist:
	
	answer = sr1(IP(dst=ip)/UDP(dport=53)/DNS(rd=1,qd=DNSQR(qname="mibbit.com", qtype="ALL")),verbose=0, timeout=1)#Mibbit gives good gains
	try:
		print("Sent packet of size: "+str(len(IP(dst=ip)/UDP(dport=53)/DNS(rd=1,qd=DNSQR(qname="mibbit.com", qtype="ALL")))))
		print(str(ip)+" returned reply of length: "+str(len(answer)))
		with open("./dnsips","a") as f:
			if len(answer) > 56:
				try:
					f.write(ip+" returned "+str(len(answer))+" bytes from 56 bytes.\n") #Hardcoded 56 bytes, I'm lazy
				except:
					print("erro writn")
	except:
		continue
if not iplist:
	print("No servers :(")
