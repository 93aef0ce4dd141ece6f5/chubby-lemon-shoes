#ifndef BOT_H
#define BOT_H

#define MAX_MSG_SIZE 4096
#define DEFAULT_MALLOC_SIZE 2048
#define MAX_NAME 31

#define zeroMem(x) memset((x), 0, sizeof (x));
#define sleep(x) Sleep ((x)*1000);

typedef struct _account {
	char u_name[MAX_NAME];		// username
	char n_name[MAX_NAME];		// nickname
	char *pword;				// username's password
	char *channel;				// channel/privmsg
} Account, *pAccount;

typedef struct _message {
	char n_name[MAX_NAME];		// nickname
	char *contact;				// channel/privmsg
	int contactSize;
	char *command;				// $ command
	int commandSize;
	char *param;				// $ command [paramter]
	int paramSize;
	char *msg;					// entire message
	int msgSize;
} Message, *pMessage;

enum {
	WINDOWS,
	POSIX
};

//main
void nonFatal (char *, int);
void fatal (char *, int);
void error (char *);

#endif
