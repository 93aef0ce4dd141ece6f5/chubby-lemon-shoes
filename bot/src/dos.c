/*
 * Author           : 93aef0ce4dd141ece6f5
 * Title            : dos.c
 * Description      : denial of service functions
 *                    go here.
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
// need other headers?

#endif

#include "bot.h"

/*
 * udp flood
 * max packet data
 * and send out as many
 * packets as possible
 */
void udp_flood (SOCKET s) {
    // todo
}
 
/*
 * syn flood
 * randomise source ip
 * randomise dest port?
 */
void syn_flood (SOCKET s) {
    // todo
}

void dns_amp (SOCKET s) {
	// todo
}