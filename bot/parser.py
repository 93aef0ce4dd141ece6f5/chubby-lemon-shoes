#!/usr/bin/env python

#Imports:
import argparse
from random import SystemRandom
from string import ascii_uppercase, digits

#Adding commmand line options:
defName = ''.join(SystemRandom().choice(ascii_uppercase+digits) for _ in range(6)) #Random uppercase 6 character name.

parser = argparse.ArgumentParser(description="Script to compile bot code.")

parser.add_argument("-s","--server",help="IRC server to connect to.", default="irc.freenode.net")
parser.add_argument("-r","--port",help="IRC server port.", default=6667)
parser.add_argument("-c","--channel",help="Channel to join after connecting.", default="#nullbyte")
parser.add_argument("-u","--username",help="Bot username.", default=defName)
parser.add_argument("-n","--nickname",help="Bot nickname.", default=defName)
parser.add_argument("-p","--password",help="Bot password.", default=None)
parser.add_argument("-a","--admins",help="Comma separated list of admins.", default="dontrustme")
parser.add_argument("-v","--verbosity",help="Increase verbosity.", default=False, action='store_true')

args = parser.parse_args()

#Print parsed oprions
if args.verbosity:
	for arg in vars(args):
		print(arg+" = "+str(getattr(args, arg)))
