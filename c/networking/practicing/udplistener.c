/*
** listener.c -- a datagram sockets "server" demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MAXBUFLEN 100

int main(void)
{
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr;
	int sockfd, numbytes;
	char buf[MAXBUFLEN];
	char hostbuf[NI_MAXHOST];
	char portbuf[NI_MAXSERV];
	socklen_t addr_len;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family   = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags    = AI_PASSIVE;
	
	if (getaddrinfo(NULL, "6543", &hints, &servinfo)) {
		perror("getaddrinfo()");
		return 1;
	}
	
	for (p = servinfo; p != NULL; p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
		{
			perror("listener : socket");
			continue;
		}

		// ai_addr is struct sockaddr
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
		{
			close(sockfd);
			perror("listener : bind");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "listener: failed to bind socket\n");
		return 2;
	}

	freeaddrinfo(servinfo);
	
	printf("listener : waiting to recvfrom()... \n");

	addr_len = sizeof(their_addr);
	if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1, 0, (struct sockaddr *)&their_addr, &addr_len)) == -1)
	{
		perror("listener : recvfrom()");
		exit(1);
	}

	memset(&hostbuf, 0, NI_MAXHOST);
	memset(&portbuf, 0, NI_MAXSERV);

	// get client IP address and port
	if (getnameinfo((struct sockaddr *)&their_addr, sizeof(their_addr), hostbuf, sizeof(hostbuf), portbuf, sizeof(portbuf), 0))
		perror("getnameinfo()");
	else
		printf("listener : got packet from %s and %d bytes long:\n", hostbuf, numbytes);
	buf[numbytes] = '\0';

	close(sockfd);
	return 0;
}
