#include "udputils.h"

int chat_loop(int sockfd, struct sockaddr *servaddr, socklen_t servaddrlen)
{
	int i;
	char txtbuf[MSGSIZE];
	while(true) {
		memset(txtbuf, '\0', MSGSIZE);
		fgets(txtbuf, MSGSIZE, stdin);
		txtbuf[strlen(txtbuf) - 1] = '\0';
		//if ((sendto(sockfd, txtbuf, strlen(txtbuf), 0, servaddr, servaddrlen)) < 0) {
		if (send_packet_to(sockfd, txtbuf, strlen(txtbuf), 0, servaddr, servaddrlen) < 0) {
			perror("sendto()");
			break;
		} else {
			printf("Client: Sent a message\n");
		}

		memset(txtbuf, '\0', MSGSIZE);
		if ((i = recvfrom(sockfd, txtbuf, sizeof(txtbuf) - 1, 0, servaddr, &servaddrlen)) == -1) {
			perror("recvfrom()");
			break;
		} else {
			txtbuf[i] = '\0';
			printf("Game message : %s\n", txtbuf);
		}
	}
	return 0;
}

int main(int argc, char *argv[])
{
	/* check the arguments */
	char servip[INET6_ADDRSTRLEN];
    if (argc != 2) {
        printf("Using localhost. You can use: %s ipaddress\n", argv[0]);
		strcpy(servip, "127.0.0.1");
    } else {
		strcpy(servip, argv[1]);
	}

	// does nothing, unless in Windows
	init_sockets();

	// get server info and udp socket
	int sockfd = -1;
	socklen_t servaddrlen = 0;
	struct sockaddr *servaddr = init_udpclient(servip, &sockfd, &servaddrlen);

	printf("| - - - - - C L I E N T - - - - - |\n");
	printf("%d\n", (unsigned char) (50 << 2));
	printf("%lu\n", sizeof((unsigned char) (50 << 2)));

	chat_loop(sockfd, servaddr, servaddrlen);

	close_socket(sockfd);

	// does nothing, unless in Windows
	shutdown_sockets();

	return 0;
}
