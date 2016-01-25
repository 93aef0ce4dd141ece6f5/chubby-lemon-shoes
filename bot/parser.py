#!/usr/bin/env python

#Imports:
import argparse
import sys
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

#Print parsed options if verbosity is increased.
if args.verbosity:
	for arg in vars(args):
		print(arg+" = "+str(getattr(args, arg)))
		
#Open the file and read it.
modifiedfile = open("./newhead.h","w")
with open("./bot.h","rw") as headerfile:
	while True:
		line = headerfile.readline()
		if not line: break
		if "#define SERVER" in line:
			modifiedfile.write("#define SERVER\t"+'"'+args.server+'"\n')
		elif "#define PORT" in line:
			modifiedfile.write("#define PORT\t"+str(args.port)+"\n")
		elif "#define CHANNEL" in line:
			modifiedfile.write("#define CHANNEL\t"+'"'+args.channel+'"\n')
		elif "#define U_NAME" in line:
			modifiedfile.write("#define U_NAME\t"+'"'+args.username+'"\n')
		elif "#define N_NAME" in line:
			modifiedfile.write("#define N_NAME\t"+'"'+args.nickname+'"\n')
		elif "#define PWORD" in line and args.password:
			modifiedfile.write("#define PWORD\t"+'"'+args.password+'"\n')
		elif "#define PWORD" in line and not args.password:
			modifiedfile.write("#define PWORD\tNULL\n")
		#if "//extern" in line: #admins would go here
		#	modifiedfile.write("#define SERVER\t"+args.server)
		else:
			modifiedfile.write(line)
<<<<<<< HEAD
modifiedfile.close()
=======
>>>>>>> ed33df15b206b40eb31f7dd03e1aee2f20ffd1dc
