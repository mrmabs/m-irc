/* inet.c
 * 03/09/99
 ****************************************
 * copyright 1999 Marcus Berglund
 */

/* Inet functions */
int netSend(FILE *socketFD, char *data);
int netRecv(FILE *socketFD, char *data);
int netOpen(char *host, int port, FILE *socketFD);
int netClose(FILE *socketFD);

/* inet defines */
// Inet Errors
#define ERR_GHBN    -1  // Socket Error
#define ERR_CONN    -2  // Connection Error
#define ERR_BUFOVR  -3  // GetHostByName Error
#define ERR_SOCK    -4  // Possible Attempt at Buffer Overflow
#define ERR_HCF     -5  // HCF stands for "Halt Catch Fire" :O)
						//  or die real quick

/* ircCmd stuff & struct */
typedef enum {
	CTCP,
	PING,
	MSG,
	CHANNEL,
	JOIN,
	NOTICE,
	PART,
	NICK,
	USER,
	RAW,
	PONG,
	QUIT,
	NUL,
	NA 			// NA = not aplicable, eg. ircInit
} ircType;

typedef struct ircCmd {
	int		port;
	ircType	type;
	char    nick[32];		// nick also = chan
	char    username[32];
	char    host[128];
	char    server[64];
	char    pass[32];
	char    realname[128];
	char    target[64];
	char	data[500];
	char	password[64];
} ircCmd;

/* irc functions */
//int ircSend(struct ircCmd *ircCmd);
//int ircRecv(struct ircCmd *ircCmd);
int ircStart(struct ircCmd *ircInit);
int ircMain();

/* Display functions */
void dispError(char *data);
void disp(struct ircCmd *ircCmd);
void dispText(char *data);
