/* display.c -- simple display routines
 * 03/09/99
 ****************************************
 * copyright 1999 Marcus Berglund
 */

#include <stdio.h>
#include <stdlib.h>
#include "m-irc.h"

/* Global functions */
void dispError(char *data) {
	fprintf(stderr, "Error: %s", data);
}

void disp(struct ircCmd *ircCmd) {
	switch(ircCmd->type) {
		case PING:
			printf("ping %s\n", ircCmd->data);
			break;
		case CTCP:
			printf("ctcp(%s)<%s> %s\n", ircCmd->target, ircCmd->nick, 
					ircCmd->data);
			break;
		case MSG:
			printf("(%s)<%s> %s\n", ircCmd->target, ircCmd->nick, ircCmd->data);
			break;
		case CHANNEL:
			printf("#%s<%s> %s\n", ircCmd->target, ircCmd->nick, ircCmd->data);
			break;
		case JOIN:
			printf("#%s join(%s)\n", ircCmd->target, ircCmd->nick);
			break;
		case NOTICE:
			printf("notice(%s)<%s> %s\n", ircCmd->target, ircCmd->nick, 
					ircCmd->data);
			break;
		case PART:
			printf("#%s join(%s)\n", ircCmd->target, ircCmd->nick);
			break;
		case NICK:
			printf("nick(%s -> %s)\n", ircCmd->nick, ircCmd->target);
			break;
		case QUIT:
			printf("#%s quit(%s)\n", ircCmd->target, ircCmd->nick);
			break;
		default:
			printf("Oops!");
			break;
	}
	return;
}

void dispText(char *data) {
	printf("Text: %s\n", data);
	return;
}
