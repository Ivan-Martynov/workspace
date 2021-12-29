#include "udputils.h"

// initialize a upd socket for a server
int init_socket_udp()
{
	struct addrinfo hints, *result = NULL;

	memset(&hints, 0, sizeof(hints));
	hints.ai_flags    = AI_PASSIVE;
	hints.ai_family   = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	if (getaddrinfo(NULL, PORTNUM, &hints, &result)) {
		perror("cannot get addresses for server");
		return -1;
	}

	struct addrinfo *iter;
	int sockfd = -1, yes = 1;

	for (iter = result; iter != NULL; iter = iter->ai_next) {
		if ((sockfd = socket(iter->ai_family, iter->ai_socktype, iter->ai_protocol)) < 0) {
			perror("socket()");
			return -1;
		}

		setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
		if (bind(sockfd, iter->ai_addr, iter->ai_addrlen) < 0 ) {
			close_socket(sockfd);
			perror("bind()");
			return -1;
		}

		// using fcntl for non-blocking
#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
		if (fcntl(sockfd, F_SETFL, O_ASYNC) < 0) {
			close_socket(sockfd);
			perror("fcntl()");
			return -1;
		}
#elif PLATFORM == PLATFORM_WINDOWS
		DWORD nb = 1;
		if (ioctlsocket(sockfd, FIONBIO, &nb) != 0) {
			printf("Failed to set non-blocking socket\n");
			close_socket(sockfd);
			return -1;
		}
#endif

		break;
	}

	unsigned int v = 0;
	socklen_t vlen = sizeof(v);
	if (getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &v, &vlen) == -1)
		perror("Cannot get read buffer size getsockopt()");

	freeaddrinfo(result);

	return sockfd;
}

// initialize a udp socket for a client
int get_socket_udp(char *servaddrchar)
{
	struct addrinfo hints, *result = NULL, *res = NULL;

	memset(&hints, 0, sizeof(hints));
	hints.ai_flags    = AI_NUMERICHOST | AI_NUMERICSERV;
	hints.ai_family   = PF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	if (getaddrinfo(servaddrchar, PORTNUM, &hints, &result) < 0) {
		perror("Cannot resolve address");
		exit(1);
	}

	int sockfd;
	for (res = result; res != NULL; res = res->ai_next) {
		if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) {
			perror("socket()");
			continue;
		}
		break;
	}

#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
		if (fcntl(sockfd, F_SETFL, O_ASYNC) < 0) {
			close_socket(sockfd);
			perror("fcntl()");
			return -1;
		}
#elif PLATFORM == PLATFORM_WINDOWS
		DWORD nb = 1;
		if (ioctlsocket(sockfd, FIONBIO, &nb) != 0) {
			printf("Failed to set non-blocking socket\n");
			close_socket(sockfd);
			return -1;
		}
#endif

	freeaddrinfo(result);

	if (res == NULL) {
		fprintf(stderr, "client : failed to create socket\n");
		exit(1);
	}

	return sockfd;
}

int send_packet_to(int sockfd, const char data[], int size, int flags, const struct sockaddr *to, socklen_t len)
{
	int i;
	if ((i = sendto(sockfd, (const char*)data, size, flags, to, len)) < 0) {
		perror("send()");
		return -1;
	}
	return i;
}

// get udp socket file descriptor for a client
struct sockaddr *init_udpclient(char *servaddrchar, int *sockfd, socklen_t *servaddrlen)
{
	struct addrinfo hints, *result = NULL, *res = NULL;

	memset(&hints, 0, sizeof(hints));
	hints.ai_flags    = AI_NUMERICHOST | AI_NUMERICSERV;
	hints.ai_family   = PF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	if (getaddrinfo(servaddrchar, PORTNUM, &hints, &result) < 0) {
		perror("Cannot resolve address");
		exit(1);
	}

	for (res = result; res != NULL; res = res->ai_next) {
		if ((*sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) {
			perror("socket()");
			continue;
		}
		break;
	}

#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
		if (fcntl(*sockfd, F_SETFL, O_ASYNC) < 0) {
			close_socket(*sockfd);
			perror("fcntl()");
			return NULL;
		}
#elif PLATFORM == PLATFORM_WINDOWS
		DWORD nb = 1;
		if (ioctlsocket(*sockfd, FIONBIO, &nb) != 0) {
			printf("Failed to set non-blocking socket\n");
			close_socket(*sockfd);
			return NULL;
		}
#endif

	freeaddrinfo(result);

	if (res == NULL) {
		fprintf(stderr, "client : failed to create socket\n");
		exit(1);
	}

	*servaddrlen = res->ai_addrlen;
	return res->ai_addr;
}
