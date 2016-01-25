#ifndef BOT_H
#define BOT_H

/*
 * max buffer size to send
 * or receive 
 */
#define MAX_MSG_SIZE 4096

/*
 * default size for malloc
 */
#define DEFAULT_MALLOC_SIZE 2048

/*
 * maximum irc name size
 */
#define MAX_NAME 31

#define zeroMem(x) memset((x), 0, sizeof (x));

/*
 * macro for easier sleep
 * times sleep (seconds)
 */
#define sleep(x) Sleep ((x)*1000);

/*
 * struct containing IRC
 * details of bot
 */
typedef struct _account {
	char u_name[MAX_NAME];		// username
	char n_name[MAX_NAME];		// nickname
	char *pword;			// username's password
	char *channel;			// channel/privmsg
} Account, *pAccount;

/*
 * struct to store information
 * from received messages 
 * from IRC
 */
typedef struct _message {
	char n_name[MAX_NAME];		// nickname
	char *contact;			// channel/privmsg
	int contactSize;
	char *command;			// command
	int commandSize;
	char *param;			// command [paramter]
	int paramSize;
	char *msg;			// entire message
	int msgSize;
} Message, *pMessage;

/*
 * definitions for
 * error checking
 */
enum {
	WINDOWS,
	POSIX
};

// bot.c
void nonFatal (char *, int);
void fatal (char *, int);

#endif
