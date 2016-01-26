/*
 * Title: bot.c
 * Description: file includes main and other error
 *              checking functions, fatal kills 
 *              program on error, non_fatal does not
 *
 *
 *
 *
 *
 *
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
#if defined(WIN32)
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
    #if defined(WIN32)
    int err = 0;
    if ((err = (int)GetLastError())) {
        fprintf (stderr, "[!] %s error: %d\n", str, err);
    }
    #elif defined(__linux__)
    fprintf (stderr, "[!] %s error: %s\n", str, strerror (errno));
    #endif
}

void fatal (char *str) {
    #if defined(WIN32)
    int err = 0;
    if ((err = (int)GetLastError())) {
        fprintf (stderr, "[!] %s error: %d\n", str, err);
    }
    #elif defined(__linux__)
    fprintf (stderr, "[!] %s error: %s\n", str, strerror (errno));
    #endif
    exit (EXIT_FAILURE);
}

int main (int argc, char *argv[]) {
    SOCKET s;

    pAccount account = new_account();
    if (account == NULL) {
        fatal ("New account");
    }

    s = irc_connect (SERVER, PORT);

    setup_irc (s, account);
    
    start_recv (s, account);
    
    free (account);

    return EXIT_SUCCESS;
}
