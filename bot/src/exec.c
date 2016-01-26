/*
 * Author           : 93aef0ce4dd141ece6f5
 * Title            : exec.c
 * Description      : functions which concern executing
 *                    or authentication go here
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

#include <winsock2.h>
#include <windows.h>

#elif defined(__linux__)

#include <sys/socket.h>
// need other headers?

#endif

#include "bot.h"

/*
 * function to check if user
 * has authenticated with correct
 * password
 * return TRUE if correct else
 * return FALSE
 */
int check_pass (char *pass) {
    if (strncmp (pass, BOT_PWORD, strlen (BOT_PWORD)) == 0) {
        return TRUE;
    }

    return FALSE;
}

/*
 * function to add nickname to 
 * admin list
 * requires a bit of error checking
 * to see if larger than max admin
 * size (65535) and if admin array needs to
 * be expanded
 */
int add_admin (pAccount a, pMessage m) {
    // check if admins are maxed out
    if (a->num_admins < sizeof (unsigned short)) {
        unsigned short prev_admin_size = a->admin_size;
        // check if admin array needs to be expanded
        if (a->num_admins >= a->admin_size) {
            // expand array with realloc
            a->admins = realloc (a->admins, sizeof (char *)*a->admin_size*2+1);
            if (a->admins == NULL) {
                return FALSE;
            }
            // update admin size
            a->admin_size = a->admin_size*2+1;

            // iterate through admins to NULL any unused array pointers
            int i;
            for (i = 0; i < prev_admin_size; i++);
            // initialise unused array pointers to NULL
            for (; i < a->num_admins; i++) {
                a->admins[i] = NULL;
            }
        }
        // add new admin to next available array element
        // increase number of admins by 1
        a->admins[a->num_admins++] = strdup (m->n_name);

    }

    return TRUE;
}

/*
 * function to check if user's
 * nickname is registered as an
 * admin
 * iterate through admin list with
 * strcmp and return non-zero if
 * is admin, else return 0
 */
int is_admin (pAccount a, pMessage m) {
    int i;
    for (i = 0; i < a->num_admins; i++) {
        if (strcmp (m->n_name, a->admins[i])) {
            return TRUE;
        }
    }

    return FALSE;
}