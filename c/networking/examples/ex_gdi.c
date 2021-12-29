/** exercising with getaddrinfo() function */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char *argv[]) {
	printf("Hello, World!\n");

	int status;
	struct addrinfo hints;
	struct addrinfo *servinfo; // will point to the results

	memset(&hints, 0, sizeof hints);  // make sure the struct is empty
	hints.ai_family   = AF_UNSPEC;    // don't care IPv4 or 6
	hints.ai_socktype = SOCK_STREAM;  // TCP stream sockets
	hints.ai_flags    = AI_PASSIVE;   // fill in my IP for me
	// printf("%d\n", hints.ai_flags);

	if ((status = getaddrinfo(NULL, "3490", &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
		exit(1);
	}
	// status = getaddrinfo("www.example.net", "3490", &hints, servinfo);

	// servinfo now points to a linked list of 1 or more struct "addrinfo"s

	// ... do everything until you don't need servinfo anymore ...
	freeaddrinfo(servinfo); // free the linked-list
}
