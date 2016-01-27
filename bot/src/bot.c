/*
 *   Author           : 93aef0ce4dd141ece6f5
 *   Title            : bot.c
 *   Description      : file includes main and other error
 *                        checking functions, fatal kills 
 *                        program on error, non_fatal does not
 *
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

/* 
 * setopt() to parse
 * command line options
 */
#include <unistd.h>

/*
 * compiling for WIN32 machines
 * include the folllowing
 */
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
/*
 * GetLastError() for
 * error checking
 */
#include <windows.h>

/*
 * compiling for linux machines
 * include the folllowing
 */
#elif defined(__linux__)

/*
 * errno value required
 * for error checking
 */
#include <errno.h>

#endif

#include "bot.h"

void non_fatal (char *str) {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)

    int err = 0;
    if ((err = (int)GetLastError())) {
        fprintf (stderr, "[!] %s error: %d\n", str, err);
    }
    
#elif defined(__linux__)

    fprintf (stderr, "[!] %s error: %s\n", str, strerror (errno));

#endif
}

void fatal (char *str) {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)

    int err = 0;
    if ((err = (int)GetLastError())) {
        fprintf (stderr, "[!] %s error: %d\n", str, err);
    }

#elif defined(__linux__)

    fprintf (stderr, "[!] %s error: %s\n", str, strerror (errno));

#endif
    exit (EXIT_FAILURE);
}

int main (void) {
    SOCKET s;

    pAccount account = new_account();
    if (account == NULL) {
        fatal ("New account");
    }

    s = server_connect (SERVER, PORT);

    setup_irc (s, account);

    str_to_lower (account->u_name);
    str_to_lower (account->n_name);
    
    start_recv (s, account);
    
    free (account);

    return EXIT_SUCCESS;
}
