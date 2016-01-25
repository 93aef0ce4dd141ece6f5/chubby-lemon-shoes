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
#ifdef _WIN32
/*
 * GetLastError() for
 * error checking
 */
#include <windows.h>

/*
 * compiling for linux machines
 * include the folllowing
 */
#elif __linux__

/*
 * errno value required
 * for error checking
 */
#include <errno.h>

#endif

#include "bot.h"

void non_fatal (char *str) {
    #ifdef _WIN32
    int err = 0;
    if ((err = (int)GetLastError())) {
        fprintf (stderr, "[!] %s error: %d\n", str, err);
    }
    #elif __linux__
    fprintf (stderr, "[!] %s error: %s\n", str, strerror (errno));
    #endif
}

void fatal (char *str) {
    #ifdef _WIN32
    int err = 0;
    if ((err = (int)GetLastError())) {
        fprintf (stderr, "[!] %s error: %d\n", str, err);
    }
    #elif __linux__
    fprintf (stderr, "[!] %s error: %s\n", str, strerror (errno));
    #endif
    exit (EXIT_FAILURE);
}

int main (int argc, char *argv[]) {
    SOCKET s;

    // irc_connect
    // start_recv
    
    free (account);

    return EXIT_SUCCESS;
}
