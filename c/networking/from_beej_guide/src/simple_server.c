#include "platform_specific.h"

#if !WINDOWS_PLATFORM
#define _POSIX_C_SOURCE 200112L
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#include "error_handler.h"
#include "net_ids.h"
#include "tcp_utils.h"

#if WINDOWS_PLATFORM
#include <winsock2.h>
#include <ws2tcpip.h>

//#define close closesocket

//#pragma comment(lib, "ws2_32")
#else
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#endif

enum MAIN_RETURN_VALUES
{
    ALL_GOOD = 0,
    WSASTARTUP_FAILURE,
    GETADDRINFO_FAILURE,
    SETSOCKETOPT_FAILURE,
    BIND_FAILURE,
    LISTEN_FAILURE,
    SIGACTION_FAILURE,
    SEND_FAILURE
};

#define BACKLOG 10 // Max number of pending connections.

void sig_child_handler(int s)
{
    (void)s; // Unused variable warning cure.

    // Save the current errno value to restore it, because function waitpid()
    // might overwrite its value.
    const int errno_backup = errno;

    while (waitpid(-1, NULL, WNOHANG) > 0)
    {
    }

    errno = errno_backup;
}

int main(void)
{
    /*
     * Structure addrinfo is used to prepare socket address structures for
     * subsequent use. It is also used in host and service names lookups.
     */
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_UNSPEC; // Make the code IP-version agnostic.
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // Use my IP.

    // Use getaddrinfo function to fill an addrinfo linked list structure.
    // The first parameter is a host name or an ip address to connect to.
    // The second parameter is a port number or the name of a service (e.g.,
    // http, ftp, telnet etc.).
    // The third parameter is the hints, containing the relevant information.
    struct addrinfo* server_info;
    const int status = getaddrinfo(NULL, PORT, &hints, &server_info);

    if (status != 0)
    {
        fprintf(stderr, "Server: getaddrinfo error: %s\n",
            getaddrinfo_error_code_to_string(status));
        return GETADDRINFO_FAILURE;
    }

    // Loop and bind to the first possible.
    int socket_fd;
    struct addrinfo* current = server_info;
    for (; current; current = current->ai_next)
    {
        // Get a socket descriptor, referring to a created endpoint.
        socket_fd = socket(
            current->ai_family, current->ai_socktype, current->ai_protocol);

        if (socket_fd == -1)
        {
            report_error("Server: socket error");
            continue;
        }

        // Set socket options. SOL_SOCKET - options to be accessed to socket
        // level (not protocol level). SO_REUSEADDR - reuse of local addresses
        // is supported.
        const int yes = 1;
        if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes,
                sizeof(int))
            == -1)
        {
            report_error("Server: setsocketopt error");
            return SETSOCKETOPT_FAILURE;
        }

        // Bind to a socket: assigning the address (name) to the socket.
        if (bind(socket_fd, current->ai_addr, current->ai_addrlen) == -1)
        {
            close(socket_fd);
            report_error("Server: bind error");
            continue;
        }

        break;
    }

    freeaddrinfo(server_info);

    if (!current)
    {
        fprintf(stderr, "Server: failed to bind.\n");
        return BIND_FAILURE;
    }

    // Listen for connections on the socket. BACKLOG - maximum length of a
    // waiting queue of pending connections.
    if (listen(socket_fd, BACKLOG) == -1)
    {
        report_error("Server: listen error");
        return LISTEN_FAILURE;
    }

#if !WINDOWS_PLATFORM
    struct sigaction sig_action;
    sig_action.sa_handler = sig_child_handler; // Reap all dead processes.
    sigemptyset(&sig_action.sa_mask);
    sig_action.sa_flags = SA_RESTART;

    if (sigaction(SIGCHLD, &sig_action, NULL) == -1)
    {
        report_error("Server: sigaction error");
        return SIGACTION_FAILURE;
    }
#endif

    printf("Server: waiting for connections...\n");

    while (true)
    {
        struct sockaddr_storage their_addr;
        socklen_t addr_size = sizeof(their_addr);
        const int fd
            = accept(socket_fd, (struct sockaddr*)&their_addr, &addr_size);
        if (fd == -1)
        {
            report_error("Server: accept error");
            continue;
        }

        char s[INET6_ADDRSTRLEN];
        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr*)&their_addr), s, sizeof(s));

        printf("Server: got connection from %s\n", s);

        if (!fork()) // This is the child process.
        {
            close(socket_fd); // Child doesn't need the listener.

            if (send(fd, "Hello, World!", 13, 0) == -1)
            {
                report_error("Server: send error");
            }

            close(fd); // Parent doesn't need this.

            exit(0);
        }
    }

    return EXIT_SUCCESS;
}
