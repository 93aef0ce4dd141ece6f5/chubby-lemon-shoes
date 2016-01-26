#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(_WIN32) || defined(__WIN32__)
#define _WIN32_WINNT 0x501

#include <ws2tcpip.h>
#include <windows.h>

#elif defined(__linux__)

#include <sys/socket.h>
// getaddrinfo
#include <netdb.h>
#endif

#include "bot.h"

char *default_admins[] = {"dontrustme"};

/*
 * function to initlialise the
 * account struct
 */
pAccount new_account (void) {
	pAccount a = malloc (sizeof (*a));
	if (a == NULL) {
		return a;
	}

	memset (a->u_name, 0, MAX_NAME);
	memset (a->n_name, 0, MAX_NAME);

	strncpy (a->u_name, U_NAME, MAX_NAME);
	strncpy (a->n_name, N_NAME, MAX_NAME);

	a->pword = IRC_PWORD;
	a->channel = CHANNEL;
	a->admin_size = sizeof (default_admins)/sizeof (*default_admins);
	a->num_admins = a->admin_size;
	a->admins = malloc (sizeof (char *)*a->num_admins);
	if (a->admins == NULL) {
		return a;
	}

	int i;
	for (i = 0; i < a->admin_size; i++) {
		a->admins[i] = default_admins[i];
	}

	return a;
}

/*
 * function to initialise the
 * message struct
 * default sizes are
 * DEFAULT_MALLOC_SIZE (2048)
 */
pMessage new_message (void) {
	pMessage m = malloc (sizeof (*m));
	if (m == NULL) {
		return m;
	}
	
	memset (m->n_name, 0, MAX_NAME);

	m->contact = malloc (DEFAULT_MALLOC_SIZE);
	if (m->contact == NULL) {
		return NULL;
	}
	m->contactSize = DEFAULT_MALLOC_SIZE;

	m->command = malloc (DEFAULT_MALLOC_SIZE);
	if (m->command == NULL) {
		return NULL;
	}
	m->commandSize = DEFAULT_MALLOC_SIZE;

	m->param = malloc (DEFAULT_MALLOC_SIZE);
	if (m->param == NULL) {
		return NULL;
	}
	m->paramSize = DEFAULT_MALLOC_SIZE;

	m->msg = malloc (DEFAULT_MALLOC_SIZE);
	if (m->msg == NULL) {
		return NULL;
	}
	m->msgSize = DEFAULT_MALLOC_SIZE;

	return m;
}

/*
 * function to create a 
 * connection to the IRC
 * server, returns socket
 */
SOCKET irc_connect (char *addr, char *port) {
    SOCKET s;
    int gai;
    struct addrinfo hints, *res;
    memset (&hints, 0, sizeof (hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)

    WSADATA wsa;
    
    if (WSAStartup (MAKEWORD (2, 2), &wsa)) {
        fatal ("WSAStartup");
    }

#endif
    
    gai = getaddrinfo (addr, port, &hints, &res);
    if (gai) {
        fprintf(stderr, "[!] Get info error: %s\n", gai_strerror (gai));
        exit (EXIT_FAILURE);
    }
    
    s = socket (res->ai_family, res->ai_socktype, res->ai_protocol);
    if (s == -1) {
        fatal ("Socket");
    }
    
    puts ("[*] Connecting...");
    if (connect (s, res->ai_addr, res->ai_addrlen) == -1) {
        fatal ("Connect");
    }
    
    return s;
}

/*
 * set up bot's nickname and 
 * username with the IRC server
 */
int setup_irc (SOCKET s, pAccount account) {
    // need to remove magic numbers
    char set_nick[strlen (account->n_name) + 9];
    char set_user[strlen (account->u_name) + 21];
    char join_chan[strlen (account->channel) + 8];

    snprintf (set_nick, sizeof (set_nick), "NICK %s\r\n", account->n_name);
    snprintf (set_user, sizeof (set_user), "USER %s 0 * :~bot~\r\n", account->u_name);
    snprintf (join_chan, sizeof (join_chan), "JOIN %s\r\n", account->channel);

    if (send (s, set_nick, strlen (set_nick), 0) == -1) {
        fatal ("Send nickname");
    }

    if (send (s, set_user, strlen (set_user), 0) == -1) {
        fatal ("Send username");
    }

    if (send (s, join_chan, strlen (join_chan), 0) == -1) {
        fatal ("Join chan");
    }

    return EXIT_SUCCESS;
}

/*
 * convert all characters to 
 * lowercase for easier
 * string comparisons
 */
void str_to_lower (char *s) {
	if (s != NULL) {
		int i;
		
		for (i = 0; i < strlen (s); i++) {
			if (isalpha (s[i]) && isupper (s[i])) {
				s[i] = tolower (s[i]);
			}
		}
	}
}

/*
 * null terminates the
 * trailing \r\n
 */
static void trim_string (char *s) {
	if (s != NULL) {
		int i;

		for (i = strlen (s)-10; i < strlen (s); i++) {
			if (s[i] == '\r' || s[i] == '\n') {
				s[i] = '\0';
				break;
			}
		}
	}
}

/*
 * modify the strings of
 * the received messages
 * for easier string
 * comparisons
 */
void format_message (pMessage m) {
	str_to_lower (m->n_name);
	str_to_lower (m->contact);
	str_to_lower (m->command);
	str_to_lower (m->param);
	str_to_lower (m->msg);

	trim_string (m->n_name);
	trim_string (m->contact);
	trim_string (m->command);
	trim_string (m->param);
	trim_string (m->msg);
}
