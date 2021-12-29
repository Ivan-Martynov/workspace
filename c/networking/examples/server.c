/** server.c - a stream socket server demo */

/** include necessary libraries */
#include <stdio.h> 	/** printf et al. */
#include <stdlib.h>	/** exit() */
#include <unistd.h>	/** getopt, read, etc. */
#include <errno.h>	/** errno declaration, error codes */
#include <string.h>	/** String manipulation and memory functions */
#include <sys/types.h>	 
#include <sys/socket.h>	/** Socket functions (socket(), bind(), etc.) */
#include <netinet/in.h>	/** sockaddr_in and sockaddr_in6 defs */
#include <netdb.h>	/** getaddrinfo et al */
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

/** define port users will be connecting to */
#define PORT "3490"
/** how many pending connections queue will hold */
#define BACKLOG 10

void sigchld_handler(int s)
{
    /** waitpid() might overwrite errno => we save and restore it */
    int saved_errno = errno;

    while (waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

/** get sockaddr, IPv4 or v6 */
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{
    /** listen on sock_fd, new connection on new_fd */
    int sockfd, new_fd;
    /** struct arrdinfo has the following structure
     	{
			int ai_flags;              // AI_PASSIVE, AI_CANONNAME, etc.
			int ai_family;             // AF_INET, AF_INET6, AF_UNSPEC
			int ai_socktype;           // SOCK_STREAM, SOCK_DGRAM
			int ai_protocol;           // IPPROTO_TCP, IPPROTOC_UDP, "0" for any
			size_t ai_addrlen;         // size of ai_addr in bytes
			struct sockaddr *ai_addr;  // struct sockaddr_in or _in6
			char *ai_canonname;        // full canonical hostname
			struct addrino *ai_next;   // linked list, next node
		}
    */
    struct addrinfo hints, *servinfo, *p;
    /** connector's address info */
	/** struct sockaddr_storage is used to suppy sufficient storage
		to hold the entire address, which can be IPv4 or v6 address
		structure
	 */
    struct sockaddr_storage their_addr;
	/** size of address, unsigned opaque integral type */
    socklen_t sin_size;
	/** sigaction examines and changes a signal action */
    struct sigaction sa;
    int yes = 1;
    char s[INET6_ADDRSTRLEN];
    int rv;
    /** a message to send and its length */
    char *msg = "Hello, world! Message has been sent :)\n";
    int len   = strlen(msg);
    /** make sure the struct is empty */
    memset(&hints, 0, sizeof hints);
    /** don't care IPv4 or IPv6 */
    hints.ai_family   = AF_UNSPEC;
    /** TCP stream sockets */
    hints.ai_socktype = SOCK_STREAM;
    /** fill in my IP for me */
    hints.ai_flags    = AI_PASSIVE;
    
    /**
    printf("hints.ai_family = %d\n", hints.ai_family);
    printf("hints.ai_socktype = %d\n", hints.ai_socktype);
    printf("hints.ai_flags = %d\n", hints.ai_flags);
    */

	/** getaddrinfo(const char *node, // e.g. "www.ex.com" or IP
	 				const char *service, // e.g. "ftp" or port
					const struct addrinfo *hints,
					struct addrinfo **res);
		node - host name or IP address to connect to
		service - port number (e.g. "80") or name of the service
				  (e.g., "http", "ftp", "telnet", "smtp" etc.)
		hints - pointer to struct addrinfo filled with necessary
				information
		res - pointer to a linked-list of results, the "output"
	*/
    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    /** loop through all the results and bind to the first we can */
    for (p = servinfo; p != NULL; p = p->ai_next) {
		/* socket return a socket descriptor to be used later ... */
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            perror("server: socket");
            continue;
        }
        
		/* lose "Address already in use" error message
		   sockfd - socket
		   SOL_SOCKET - level...
		   SO_REUSEADDR - Allows other sockets to bind() to this port, unless there is an active listening socket bound to the port already. This enables you to get around those "Address already in use" error messages when you try to restart your server after a crash
		   yes - optval, pointer to int indicating value in question
		*/
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        {
            perror("setsockopt");
            exit(1);
        }

		/* after getting the socket desciptor call bind()
		   associate the socket with a port on your local machine
		   sockfd - socket descriptor,
		   ai_addr - infro about your port and IP */
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sockfd);
            perror("server: bind");
            continue;
        }
        break;
    }
    
    /** all done with this structure */
    freeaddrinfo(servinfo);

    if (p == NULL)
    {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

	/* sockfd - socket descriptor,
	   BACKLOG - number of connections allowed on the incoming queue
	   Call bind() before listen() to make sure the server is running
	   on a specific port (tell your friends which port to connect to)
	   */
    if (listen(sockfd, BACKLOG) == -1)
    {
        perror("listen");
        exit(1);
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
    
    /** main accept() loop */
    while (1)
    {
        sin_size = sizeof their_addr;
		printf("size of their_addr = %d\n", sin_size);
		/* accept an incoming connection */
		/* accept returns a new socket file descriptor to use for this
		 * single connection
		 * sockfd - listen()ing socket descriptor
		 * their_addr - info about the incoming connection
		 */
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1)
        {
            perror("accept");
            continue;
        }

		/* convert IP address to human-readable from */
        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
        printf("server: got connection from %s\n", s);

        /** child process */
        if (!fork())
        {
            /** child doesn't need the listener */
            clock(sockfd);
            if (send(new_fd, msg, len, 0) == -1)
            {
                perror("send");
            }
            close(new_fd);
            exit(0);
        }
        
        /** parent doesn't need this */
        close(new_fd);
    }

    return 0;
}
