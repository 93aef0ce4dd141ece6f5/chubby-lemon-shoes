/*
 * Title: dos.c
 * Description: denial of service functions
 *              go here.
 *
 *
 *
 *
 *
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if defined(WIN32)

#include <winsock2.h>
#include <windows.h>

#elif defined(__linux__)

#include <sys/socket.h>
// need other headers?

#endif

#include "bot.h"

/*
 * do we want to thread or 
 * fork the flood routines
 * in linux?
 */

void udp_flood (SOCKET s) {
    // todo
}
 
void syn_flood (SOCKET s) {
    // todo
}
