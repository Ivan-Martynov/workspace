#include <unistd.h>
#include <stdio.h>
#include <string.h>
/* defines various data types; accept() */
#include <sys/types.h>
/* defines socket prototypes, macros, variable and structs sockaddr, msghrd, linger; accept(), socket(), bind(), etc. */
#include <sys/socket.h>
/* prototypes, macros, variables, structs sockaddr_in (_in6) */
#include <netinet/in.h>
/* prototypes for network library routines to convert Internet address and dotted-decimal notation, e.g. inet_makeaddr() */
#include <arpa/inet.h>
/* network library routines, struct hostent, netent; getaddrinfo et al. */
#include <netdb.h>

#define MSGSIZE 1024

// check if input is a number
int isNumeric(const char *s)
{
    if (s == NULL || *s == '\0' || isspace(*s))
		return 0;
    char *p;
    strtod(s, &p);
    return *p == '\0';
}

int main()
{
	char txtbuf[MSGSIZE];
	int n, i;
	memset(txtbuf, '\0', MSGSIZE);
	fgets(txtbuf, MSGSIZE, stdin);
	txtbuf[strlen(txtbuf) - 1] = '\0';
	
	if (isNumeric(txtbuf) > 0) {
		n = atoi(txtbuf);
		printf("%s and %lu\n", txtbuf, strlen(txtbuf));
		printf("String %s and its conversion %d\n", txtbuf, n);
		printf("Number squared %d\n", n * n);
	} else printf("Input should be numeric\n");

	return 0;
}
