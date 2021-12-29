/* selectserver.c -- a cheezy multiperson chat server */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT "9034"

// get sockaddr (IPv4 or 6)
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void sigchld_handler(int s)
{
    /** waitpid() might overwrite errno => we save and restore it */
    int saved_errno = errno;

    while (waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

int main(void)
{
	// master and temp file descriptor lists for select()
	fd_set master, read_fds;

	int fdmax; // maximum file descriptor number
	int listener; // listening socket descriptor
	int newfd; // newly accept()ed socket descr
	struct sockaddr_storage remoteaddr; // client addr
	socklen_t addrlen;

	struct sigaction sa;

	char buf[256]; // buffer for client data
	int nbytes;

	char remoteIP[INET6_ADDRSTRLEN];

	int yes = 1; // for getsockopt() SO_REUSEADDR
	int i, j, rv;

	struct addrinfo hints, *ai, *p;

	FD_ZERO(&master); // clear sets
	FD_ZERO(&read_fds);

	// get a socket and bind it
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0) {
		fprintf(stderr, "selectserver : %s\n", gai_strerror(rv));
		exit(1);
	}

	for (p = ai; p != NULL; p = p->ai_next) {
		listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (listener < 0) {
			continue;
		}

		// lose pesky "address already in use" error message
		setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

		if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
			close(listener);
			continue;
		}

		break;
	}
	
	// if p is NULL => did not bind
	if (p == NULL) {
		fprintf(stderr, "selectserver : failed to bind\n");
		exit(2);
	}

	freeaddrinfo(ai);

	// listen
	if (listen(listener, 10) == -1) {
		perror("listen");
		exit(3);
	}

	/** reap all dead processes */
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }

	printf("server: waiting for connections...\n");

	// add the listener to the master set
	FD_SET(listener, &master);

	// keep track of the biggest file descriptor
	fdmax = listener;

	while (1) {
		read_fds = master; // copy
		if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1) {
			perror("select");
			exit(4);
		}

		// go through existing connections looking for data to read
		for (i = 0; i <= fdmax; i++)
			if (FD_ISSET(i, &read_fds)) { // we got one!
				if (i == listener) {
					// handle new connections
					addrlen = sizeof(remoteaddr);
					newfd = accept(listener, (struct sockaddr *)&remoteaddr, &addrlen);

					if (newfd == -1)
						perror("accept");
					else {
						FD_SET(newfd, &master); // add to master set
						if (newfd > fdmax) // keep track of the max
							fdmax = newfd;
						printf("selectserver : new connection from %s on socket %d\n", inet_ntop(remoteaddr.ss_family, get_in_addr((struct sockaddr*)&remoteaddr), remoteIP, INET6_ADDRSTRLEN), newfd);
					}
				} else {
					// handle data from a client
					if ((nbytes = recv(i, buf, sizeof(buf), 0)) <= 0) {
						// got error or connection close by the client
						if (nbytes == 0)
							// connection closed
							printf("selectserver : socket %d hung up\n", i);
						else
							perror("recv");
						close(i);
						FD_CLR(i, &master); // remove from master set
					} else
						// got data from a client
						for (j = 0; j <= fdmax; j++)
							// send to everyone
							if (FD_ISSET(j, &master))
								// except the listener and ourselves
								if (j != listener && j != i)
									if (send(j, buf, nbytes, 0) == -1)
										perror("send");
				}
			}
	}
	return 0;
}
