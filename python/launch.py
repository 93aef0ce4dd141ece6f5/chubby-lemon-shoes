#!/usr/bin/env python
import os
while True:
	try:
		os.system("nmap -iR 100 -sU -p 53 -n -T5 -oG - --open | grep /open/ | ./oDNSresolver_scanner.py")
	except KeyboardInterrupt:
		break
