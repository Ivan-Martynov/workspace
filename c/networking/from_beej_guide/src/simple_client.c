#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "platform_specific.h"
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

#define MAXDATASIZE 100 // Max number of bytes to send at once.

enum MAIN_RETURN_VALUES
{
    ALL_GOOD = 0,
    WSASTARTUP_FAILURE,
    GETADDRINFO_FAILURE,
    SETSOCKETOPT_FAILURE,
    BIND_FAILURE,
    LISTEN_FAILURE,
    SIGACTION_FAILURE,
    SEND_FAILURE,
    CONNECT_FAILURE,
    RECV_FAILURE
};

int main(int argc, char* argv[])
{
    char* node;
    if (argc < 2)
    {
        printf("Using local host.\nProgram can be run with a parameter: %s "
               "\"ip/web address\"\n",
            argv[0]);
        node = "::1";
    }
    else
    {
        node = argv[1];
    }

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    struct addrinfo* results;
    int status = getaddrinfo(node, PORT, &hints, &results);

    if (status != 0)
    {
        fprintf(stderr, "Client: getaddrinfo error: %s\n",
            getaddrinfo_error_code_to_string(status));
        return GETADDRINFO_FAILURE;
    }

    struct addrinfo* current = results;
    int socket_fd;
    for (; current != NULL; current = current->ai_next)
    {
        socket_fd = socket(
            current->ai_family, current->ai_socktype, current->ai_protocol);

        if (socket_fd == -1)
        {
            report_error("Client: socket error");
            continue;
        }

        if (connect(socket_fd, current->ai_addr, current->ai_addrlen) == -1)
        {
            close(socket_fd);
            report_error("Client: connect error");
            continue;
        }

        break;
    }

    if (current == NULL)
    {
        fprintf(stderr, "Client: failed to connect.\n");
        return CONNECT_FAILURE;
    }

    char s[INET6_ADDRSTRLEN];
    inet_ntop(current->ai_family,
        get_in_addr((struct sockaddr*)current->ai_addr), s, sizeof(s));

    printf("Client: connecting to %s\n", s);

    freeaddrinfo(results);

    char buffer[MAXDATASIZE];
    const int num_bytes = recv(socket_fd, buffer, MAXDATASIZE - 1, 0);

    if (num_bytes == -1)
    {
        report_error("Client: recv error");
        return RECV_FAILURE;
    }

    buffer[num_bytes] = '\0';

    printf("Client: received '%s'\n", buffer);

    close(socket_fd);

    return EXIT_SUCCESS;
}
