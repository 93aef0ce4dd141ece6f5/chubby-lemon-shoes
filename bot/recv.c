/*
 * Title: recv.c
 * Description: functions which concern receiving
 *              and received data go here.
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
 * networking functions go here
 */
#ifdef _WIN32

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#elif __linux__

#include <sys/socket.h>
//getaddrinfo
#include <netdb.h>

#endif

#include "bot.h"

/*
 * function to close sockets, free
 * heap allocs in account and message
 */
int cleanup (SOCKET s, pAccount a, pMessage m) {
    // free heap allocs
    if (p != NULL) {
        free (p);
    }

    if (m != NULL) {
        free (m->contact);
        free (m->command);
        free (m->param);
        free (m->msg);
        free (m);
    }
    
	int err = 0;
    // close socket
    err = shutdown (s, SD_BOTH);
    if (err == -1) {
    	return err;
    }
    
    #ifdef _WIN32
    err = closesocket (s);
    if (err == SOCKET_ERROR) {
    	return err;
    }
    err = WSACleanup();
    if (err == SOCKET_ERROR) {
    	return err;
    }
    
    #elif __linux__
    
    err = close (s);
    if (err == -1) {
    	return err;
    }
    #endif
}

/*
 * extract elements from received
 * messages from IRC
 */
static void extract (pMessage m, char *s) {
    char *token;
    const char delims[] = " !:";

	/*
	 * extract the user's message
	 */
    if ((token = strstr (s+1, ":")) != NULL) {
        if (strlen (token) > m->msgSize) {
            m->msg = realloc (m->msg, m->msgSize*2);
            m->msgSize *= 2;
        } else if ((strlen (token)*2) < m->msgSize && m->msgSize > DEFAULT_MALLOC_SIZE) {
            m->msg = realloc (m->msg, m->msgSize/2);
            m->msgSize /= 2;
        }
        strncpy (m->msg, token+1, m->msgSize-1);
    }

	/*
	 * extract nickname of message
	 */
    if ((token = strtok (s+1, delims)) != NULL) {
        strncpy (m->n_name, token, MAX_NAME-1);
    }
    
    /*
     * remove some unwanted elements
     */
    if ((token = strtok (NULL, delims)) != NULL) {
    }

    if ((token = strtok (NULL, delims)) != NULL) {
    }

	/* 
	 * extract the channel or user (from pm)
	 */
    if ((token = strtok (NULL, delims)) != NULL) {
        if (strlen (token) > m->contactSize) {
            m->contact = realloc (m->contact, m->contactSize*2);
            m->contactSize *= 2;
        } else if ((strlen (token)*2) < m->contactSize && m->contactSize > DEFAULT_MALLOC_SIZE) {
            m->contact = realloc (m->contact, m->contactSize/2);
            m->contactSize /= 2;
        }
        strncpy (m->contact, token, m->contactSize-1);
    }

	/*
	 * extract the first word of user's message
	 */
    if ((token = strtok (NULL, delims)) != NULL) {
        if (strlen (token) > m->commandSize) {
            m->command = realloc (m->command, m->commandSize*2);
            m->commandSize *= 2;
        } else if ((strlen (token)*2) < m->commandSize && m->commandSize > DEFAULT_MALLOC_SIZE) {
            m->command = realloc (m->command, m->commandSize/2);
            m->commandSize /= 2;
        }
        strncpy (m->command, token, m->commandSize-1);
    }

	/*
	 * extract the second word of user's message
	 */
    if ((token = strtok (NULL, delims)) != NULL) {
        if (strlen (token) > m->paramSize) {
            m->param = realloc (m->param, m->paramSize*2);
            m->paramSize *= 2;
        } else if ((strlen (token)*2) < m->paramSize && m->paramSize > DEFAULT_MALLOC_SIZE) {
            m->param = realloc (m->param, m->paramSize/2);
            m->paramSize /= 2;
        }
        strncpy (m->param, token, m->paramSize-1);
    } else {
        memset (m->param, 0, m->paramSize);
    }
}

/*
 * function to receive data from the chat
 */
void start_recv (SOCKET s, pAccount account) {
    int recv_status, err = 0;
    char output[MAX_MSG_SIZE];

    pMessage message = new_message();
    if (message == NULL) {
        fatal ("Message struct failed");
    }

    while (TRUE) {
        zeroMem (output);
        if ((recv_status = recv (s, output, sizeof (output), 0)) == SOCKET_ERROR) {
            non_fatal ("Receive");
            exit (EXIT_FAILURE);
        } else if (recv_status == 0) {
            non_fatal ("Receive");
        }

        if (!strncmp (output, "PING", 4)) {
            output[1] = 'O';
            send (s, output, strlen (output), 0);
        } else {
            printf ("%s", output);
            // extract elements from messages
            //extract (message, output);
            //format_message (message);

                
        }
    }
    
    err = cleanup (s, account, message);
    if (err == -1) {
    	non_fatal ("Cleanup");
    }
}
