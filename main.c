/* main.c -- dun do much, but it's here
 * 03/09/99
 ****************************************
 * copyright 1999 Marcus Berglund
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "m-irc.h"

int main(int argc, char *argv[]) {
	struct ircCmd	ircInit;
	char			displayText[50];
	const char		username[] = "test";
	const char		realname[] = "m irc user";
	const char		password[] = "";
	const char		host[] = "mabs";

	// Enough command line args?
	if(argc != 4) {
		sprintf(displayText, "Useage:\nm-irc <nick> <host> <port>\n");
		dispText(displayText);
		//printf("Useage:\nm-irc <nick> <host> <port>\n");
		exit(0);
	}

	// We assume too much, very big smacks
	strncpy(ircInit.nick, argv[1], 32);
	strncpy(ircInit.server, argv[2], 64);
	strncpy(ircInit.host, host, 128);
	strncpy(ircInit.username, username, 32);
	strncpy(ircInit.realname, realname, 128);
	strncpy(ircInit.password, password, 32);
	ircInit.port = atoi(argv[3]);
	ircInit.type = NA;
	
	sprintf(displayText, "Command-line Args: %s %s %d\n", ircInit.nick,
			ircInit.server, ircInit.port);
	dispText(displayText);

	if(ircStart(&ircInit) != 0)
		return(-1);
	if(ircMain() != 0)
		return(-1);

	return(0);
}
