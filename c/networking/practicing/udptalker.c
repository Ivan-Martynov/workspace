/*
 ** talker.c -- a datagram sockets "client" demo
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


int main(int argc, char *argv[])
{
	int    sockfd, numbytes;
	struct addrinfo hints, *servinfo, *p;



	if (argc != 3) {
		fprintf(stderr, "usage: talker hostname message\n");
		exit(1);
	}

	memset(&hints, 0, sizeof(hints));
	hints.ai_family   = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	if (getaddrinfo(argv[1], "6543", &hints, &servinfo)) {
		perror("talker : getaddrinfo");
		return 1;
	}

	for (p = servinfo; p != NULL; p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
		{
			perror("talker : socket");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "talker : failed to create a socket\n");
		return 2;
	}

	if ((numbytes = sendto(sockfd, argv[2], strlen(argv[2]), 0, p->ai_addr, p->ai_addrlen)) == -1)
	{
		perror("talker : sendto");
		exit(1);
	}
	
	freeaddrinfo(servinfo);

	printf("talker : sent %d bytes to %s\n", numbytes, argv[1]);

	close(sockfd);

	return 0;
}
