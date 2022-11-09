#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>

#if WINDOWS_PLATFORM
    #include <winsock2.h>
    #include <ws2tcpip.h>
    //#pragma comment(lib, "ws2_32")
#else
    #include <string.h>
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <netdb.h>
    #include <arpa/inet.h>
    #include <netinet/in.h>
#endif

#include "platform_specific.h"
#include "error_handler.h"

enum MAIN_RETURN_VALUES
{
    ALL_GOOD,
    WSASTARTUP_FAILURE,
    GETADDRINFO_FAILURE
};

void show_help(void)
{
    printf("Program displays list of ip addresses for a given node (ip address "
           "or web address).\nIf it is run without arguments, then it uses the "
           "localhost. Otherwise, it takes the first argument and tries to "
           "find its ip address(es).\n");
}

int main(int argc, char* argv[])
{
#if WINDOWS_PLATFORM // Windows specific.
    //WSADATA wsaData;
    //if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0)
    //{
    //    fprintf(stderr, "WSAStartup failed.\n");
    //    return WSASTARTUP_FAILURE;
    //}
#endif

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
    int status = getaddrinfo(node, NULL, &hints, &results);

    if (status != 0)
    {
        fprintf(stderr, "getaddrinfo error: %s\n",
            getaddrinfo_error_code_to_string(status));
        //fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        return GETADDRINFO_FAILURE;
    }

    printf("Ip address for %s:\n", node);

    for (struct addrinfo* current = results; current != NULL;
         current = current->ai_next)
    {
        char* ip_version;
        void* address;
        if (current->ai_family == AF_INET)
        {
            struct sockaddr_in* ip_v4 = (struct sockaddr_in*)current->ai_addr;
            address = &(ip_v4->sin_addr);
            ip_version = "IPv4";
        }
        else
        {
            struct sockaddr_in6* ip_v6 = (struct sockaddr_in6*)current->ai_addr;
            address = &(ip_v6->sin6_addr);
            ip_version = "IPv6";
        }

        char ip_string[INET6_ADDRSTRLEN];
        inet_ntop(current->ai_family, address, ip_string, sizeof(ip_string));

        printf("%s: %s\n", ip_version, ip_string);
    }

    freeaddrinfo(results);

#if WINDOWS_PLATFORM // Windows specific.
    //WSACleanup();
#endif

    return EXIT_SUCCESS;
}
