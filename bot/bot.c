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

void print_usage (char *prog) {
    fprintf (stderr, "Usage: %s -c [CHANNEL] -n [NICKNAME] -u [USERNAME] -p [PASSWORD]\n"
                "\t-u [USERNAME]\n"
                "\t-n [NICKNAME]\n"
                "\t-p (OPTIONAL) [PASSWORD] : Required to authenticate your nickname\n"
                "\t-c [CHANNEL] : Required to join channel\n", prog);
    exit (EXIT_FAILURE);
}

int main (int argc, char *argv[]) {
    if (argc <= 4) {
        printUsage (argv[0]);
    }

    int opt, uflag = 0, pflag = 0;
    SOCKET s;

    //pAccount account = newAccount (1);

    while ((opt = getopt (argc, argv, "c:n:p:u:")) != -1) {
        switch (opt) {
            case 'c':
                account->channel = optarg;
                break;
            case 'u':
                uflag = 1;
                strncpy (account->u_name, optarg, MAX_NAME-1);
                break;
            case 'n':
                strncpy (account->n_name, optarg, MAX_NAME-1);
                break;
            case 'p':
                pflag = 1;
                account->pword = optarg;
                break;
            default:
                printUsage (argv[0]);
        }
    }

    // start_connection
    // receive
    
    free (account);

    return EXIT_SUCCESS;
}
