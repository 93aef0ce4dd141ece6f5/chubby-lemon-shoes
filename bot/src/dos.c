/*
 *   Author           : 93aef0ce4dd141ece6f5
 *   Title            : dos.c
 *   Description      : denial of service functions
 *                      go here.
 *  
 *   Copyright (C) 2016  93aef0ce4dd141ece6f5
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)

#include <winsock2.h>
#include <windows.h>

#elif defined(__linux__)

#include <sys/socket.h>
#include <pthread.h>
// need other headers?

#endif

#include "bot.h"

/*
 * udp flood
 * max packet data
 * and send out as many
 * packets as possible
 */
// usage: $udp -v [IPv4] -p [PORT] -h [THREADS] -t [TIME (s)]
void *udp_flood (void *args) {
    // todo
    char output[MAX_MSG_SIZE];
    SOCKET s = ((thd_args *)args)->s;
    
    snprintf (output, sizeof (output), "PRIVMSG %s :UDP Flooding %s:%s with %d threads for %ds.\r\n", 
                ((thd_args *)args)->contact, ((thd_args *)args)->addr, ((thd_args *)args)->port, 
                ((thd_args *)args)->threads, ((thd_args *)args)->time);
    send (s, output, strlen (output), 0);

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)

    ExitThread (0);

#elif defined(__linux__)

    pthread_exit (0);

#endif
    return NULL;
}
 
/*
 * syn flood
 * randomise source ip
 * randomise dest port?
 */
void *syn_flood (void *args) {
    // todo

    return NULL;
}

/*
 * dns aplification
 * would be best if we
 * had an open dns resolver
 * to test the functionality
 * and to confirm if it works
 * dns header packets located
 * in the header file
 */
void *dns_amp (void *args) {
    // todo

    return NULL;
}