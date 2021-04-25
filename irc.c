/* irc.c
 * 03/09/99
 ****************************************
 * copyright 1999 Marcus Berglund
 */

#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include "m-irc.h"

/* Variables */
FILE		*socketFD = 3;
char		sendData[1024];

/* Local functions */
char *send(struct ircCmd *ircCmd) {
//	char        sendData[1024];

	switch(ircCmd->type) {		// Generate the Command
		case CTCP:      		// hope this is right,
								// will have to check to make sure...
			sprintf(sendData, "PRIVMSG %s :\1 %s\n", ircCmd->nick,
					ircCmd->data);
			break;
		case PING:
			sprintf(sendData, "PING %s\n", ircCmd->data);
			break;
		case MSG:
		case CHANNEL:
			sprintf(sendData, "PRIVMSG %s :%s\n", ircCmd->nick,
					ircCmd->data);
			break;
		case JOIN:
			sprintf(sendData, "JOIN %s\n", ircCmd->nick);
			break;
		case NOTICE:
			sprintf(sendData, "NOTICE %s :%s\n", ircCmd->nick,
					ircCmd->data);
			break;
		case PART:
			sprintf(sendData, "PART %s :%s\n", ircCmd->nick,
					ircCmd->data);
			break;
		case NICK:
			sprintf(sendData, "NICK %s\n", ircCmd->nick);
			break;
		case USER:
			sprintf(sendData, "USER %s %s %s :%s\n",
					ircCmd->username, ircCmd->server,
					ircCmd->host, ircCmd->realname);
			break;
		case RAW:
			strcpy(sendData, ircCmd->data);
			break;
		case PONG:
			sprintf(sendData, "PONG %s\n", ircCmd->data);
			break;
		default:
			strcpy(sendData, "");
			dispError("Attempt to send Invalid Command, Fix Me!\n");
			return("");
			break;
	}
	printf(sendData);

	// Send the data
	return(sendData);
}

struct ircCmd recv(const char *data) {
	struct ircCmd	ircCmd;
	char			command[16];
	char			userhost[128];
	char			errorText[50];
	char			temp[500];
//	char			*inputText[1024];

	// Get some data - return if no data
//	if (netRecv(socketFD, *inputText) != 0) {
//		ircCmd->type = NUL;
//		return(ircCmd);
//	}

	ircCmd.data[0] = '\0';
	printf("DEBUG_1\n");
	printf("%s\n", data);

	/* I don't know where, but somewhere along the way sscanf has
	 * changed %s now does what the man page says -no white space-
	 *
	 * Input looks like: [:<server | user@host>] <COMMAND> <params>
	 * here we try to filter a <COMMAND> <params>, if not...
	 */
	strncpy(ircCmd.data, strsep(*data, ":"), 500);
	if (sscanf(data, "%[^: ] %*s\n", command) <= 0) {
		// ...lets try [:<server | user@host>] <COMMAND> <params>
		printf("DEBUG_1_5\n");
		if (sscanf(data, ":%[^: ] %[^: ] %*s\n", userhost, command) <= 0) {
			sprintf(errorText, "Parse Error for command %s\n",
					data);
			dispError(errorText);
			// need to put in drop on error here
		}
		/* Get out param data after the ':'
		 */
		strncpy(temp, strsep(*ircCmd.data, ":"), 500);
		strncpy(ircCmd.data, temp, 500);

		// user@host, or server msg?
		if (sscanf(userhost, "%s!%s@%s", ircCmd.nick, ircCmd.username,
					ircCmd.host) <= 0) {
			strcpy(ircCmd.server, userhost);
			strcpy(ircCmd.username, userhost);
		}
	}

	printf("%s\n", ircCmd.data);

	// What command are we getting?
	// might have to go through and get rid of the leading ':' on the Data
	// if the sscanf's fail, we don't have to stress, All, will be ok
	strcpy(ircCmd.target, "");
	if (strncasecmp(command, "PRIV", 4) == 0) {
//		sscanf(ircCmd.data, "%[^: ] %*s", ircCmd.target);
//		strncpy(temp, strsep(*ircCmd.data, ":"), 500);
		if (ircCmd.nick[1] == '#')
			ircCmd.type = CHANNEL;
		else if (ircCmd.data[1] == 0x01)
			ircCmd.type = CTCP;
		else
			ircCmd.type = MSG;
	} else if (strncasecmp(command, "PING", 4) == 0)
		ircCmd.type = PING;
	else if (strncasecmp(command, "NICK", 4) == 0) {
		strcpy(ircCmd.target, ircCmd.data);
		ircCmd.type = NICK;
	} else if (strncasecmp(command, "JOIN", 4) == 0) {
		strcpy(ircCmd.target, ircCmd.data);
		ircCmd.type = JOIN;
	} else if (strncasecmp(command, "PART", 4) == 0) {
		sscanf(ircCmd.data, "%s %s", ircCmd.target, ircCmd.data);
		ircCmd.type = PART;
	} else if (strncasecmp(command, "QUIT", 4) == 0) {
		sscanf(ircCmd.data, "%s %s", ircCmd.target, ircCmd.data);
		ircCmd.type = QUIT;
	} else if (strncasecmp(command, "NOTI", 4) == 0) {
		sscanf(ircCmd.data, "%s %s", ircCmd.target, ircCmd.data);
		ircCmd.type = NOTICE;
	} else {
		sprintf(errorText, "Unknown Type: \"%s\"\n", data);
		dispError(errorText);
	}

	return(ircCmd);
}

/* Simple ping-pong handler */
void pingPong(struct ircCmd *ircCmd) {
	ircCmd->type = PONG;
	netSend(socketFD, send(ircCmd));
}

/* Global functions */
int ircStart(struct ircCmd *ircInit) {
	if(netOpen(ircInit->server, ircInit->port, socketFD) != 0)
		return(-1);
	
	// send nick command
	ircInit->type = NICK;
	if(netSend(socketFD, send(ircInit)) != 0)
		return(-1);

	// send user command
	ircInit->type = USER;
	if(netSend(socketFD, send(ircInit)) != 0)
		return(-1);

	return(0);
}

int ircMain() {
	/* Main Loop */
	while(1) {
		struct ircCmd 	ircCmd;
		char			cmdData[1024] = "";

		sleep(1);

		if(netRecv(socketFD, cmdData) != 0)
			return(-1);

		if(strlen(cmdData) == 0)
			continue;

		printf("%d\n", strlen(cmdData));

		ircCmd = recv(cmdData);

		switch(ircCmd.type) {
			case NUL:
				break;
			case PING:
				pingPong(&ircCmd);
				// break;
			case CTCP:
			case MSG:
			case CHANNEL:
			case JOIN:
			case NOTICE:
			case PART:
			case NICK:
			case QUIT:
				disp(&ircCmd);
				break;
			default:
				dispError("Unknown Command\n");
				break;
		}

		/* check for user input should be made here */

		/* sleep here */
	}

	netClose(socketFD);

	return(0);
}
