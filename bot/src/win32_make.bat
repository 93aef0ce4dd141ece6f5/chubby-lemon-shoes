@echo off
gcc -Wall -Werror -O -o bot bot.c init.c recv.c exec.c dos.c -lws2_32
echo Done.
pause >NUL
