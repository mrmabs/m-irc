/* inet.c
 * 03/09/99
 ****************************************
 * copyright 1999 Marcus Berglund
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "m-irc.h"

/* Global Variable */
int					sock;
FILE				*sockFD;
struct sockaddr_in	sockin;

/* Internal functions */
/* Split this one up more? : find host -> init socket */
int init(char *host, int port) {
	struct hostent		*hp;

	// Hostname, ot IP?
	sockin.sin_addr.s_addr = inet_addr(host);
	if(sockin.sin_addr.s_addr == -1) {		// signed & unsigned ???
		// if Hostname, get IP for name
		hp = gethostbyname(host);
		if(hp == NULL) {
			dispError("Can't gethostbyname()\n");
			return(ERR_GHBN);
		}
		memcpy(&sockin.sin_addr.s_addr, hp->h_addr, hp->h_length);
	}

	// set up 'sockin'
	sockin.sin_family = AF_INET;
	sockin.sin_port = htons(port);
	
	// create a Socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0) {
		dispError("socket() failed\n");
		return(ERR_SOCK);
	}
	return(0);
}

int conn() {
	if (connect(sock, (struct sockaddr *) & sockin, sizeof(sockin)) == -1) {
		/* did we connect ?*/
		dispError("connect() Error\n");
		return(ERR_CONN);
	}

	if(fcntl(sock, F_SETFL, O_NONBLOCK) < 0) {
		dispError("couldn't set socket non-blocking\n");
		return(ERR_SOCK);
	}

	sockFD = fdopen(sock, "a+");
	return(0);
}

/* Global functions */
int netSend(FILE *socketFD, char *data) {

//	int			length, wlength;
	
//	memcpy(sockFD, socketFD, sizeof(sockFD));

//	length = strlen((const char *)data);
//	wlength = fwrite(data, 1, length, sockFD);
//	printf("%d %d\n", wlength, length);

	if (fwrite(data, strlen((const char *)data), 1, sockFD) != 1) {
		dispError("Error 'write()'ing to FD\n");
		return(ERR_SOCK);
	}
	return(0);
}

int netRecv(FILE *socketFD, char *data) {
	//char		serverData[1024] = "";

	// Have we got an eof situation?
	if (feof(sockFD) != 0) {
		dispError("EOF Caught!\n");
		return(ERR_SOCK);
	}

	// no, so lets read up too 1024 bytes from the input buffer
	if (fgets(data, 1024, sockFD) == NULL)
		return(0);      // no data

	return(0);
}

int netOpen(char *host, int port, FILE *socketFD) {
	if(init(host, port) != 0) {
		dispError("Error in inet init\n");
		return(-1);
	}

	if(conn() != 0) {
		dispError("Error in inet connect\n");
		return(-1);
	}

//	memcpy(socketFD, sockFD, sizeof(socketFD));
//	socketFD = sockFD;
//	printf("%d %d\n", *sockFD, *socketFD);

	return(0);
}

int netClose(FILE *socketFD) {
	fclose(sockFD);
	close(sock);
	return(0);
}
