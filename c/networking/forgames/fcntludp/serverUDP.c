#include "udputils.h"

int main_loop(int sockfd)
{
	int i;
	struct sockaddr_storage client_addr;
	socklen_t addrlen = sizeof(client_addr);
	char txtbuf[MSGSIZE];

	while (1) {
		memset(txtbuf, '\0', MSGSIZE);
		if ((i = recvfrom(sockfd, txtbuf, sizeof(txtbuf) - 1, 0, (struct sockaddr*)&client_addr, &addrlen)) < 0) {
            perror("recvfrom()");
            return -1;
        }
		txtbuf[i] = '\0';
		printf("Server: Got message \"%s\" %d bytes long\n", txtbuf, i);

		memset(&txtbuf, '\0', MSGSIZE);
        //strcat(strcpy(txtbuf, "Same to you (get it back): "), recvbuffer);
        strcpy(txtbuf, "Same to you (get it back): ");
        if ((i = sendto(sockfd, txtbuf, strlen(txtbuf), 0, (struct sockaddr*)&client_addr, addrlen)) < 0)
			perror("sendto()");
        printf("Server: sent data length = %d\n", i);
	}

	return 0;
}

int main(int argc, char *argv[]) {

	/* check the arguments */
	if (argc != 1)
	{
		fprintf(stderr, "serverUDP takes no parameters, just run ./serverUDP \n");
		exit(0);
	}
	
	// does nothing, unless in Windows
	init_sockets();

	int sockfd = init_socket_udp();

	main_loop(sockfd);

	close_socket(sockfd); /* REMEMBER ME! */   

	// does nothing, unless in Windows
	shutdown_sockets();

	return 0;
}

