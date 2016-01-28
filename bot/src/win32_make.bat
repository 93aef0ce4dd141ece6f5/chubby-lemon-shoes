@echo off
gcc -Wall -Werror -O -o ../win32exe/bot bot.c init.c recv.c exec.c dos.c install.c -lws2_32
echo Done.
pause >NUL
