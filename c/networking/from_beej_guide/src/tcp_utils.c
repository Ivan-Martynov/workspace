#include "tcp_utils.h"

#if WINDOWS_PLATFORM
#include <winsock2.h>
#include <ws2tcpip.h>

#else
// TODO: test on linux.
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#endif

// Get sockaddr, IPv4 or IPv6.
void* get_in_addr(struct sockaddr* sock_addr)
{
    if (sock_addr->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in*)sock_addr)->sin_addr);
    }
    else
    {
        return &(((struct sockaddr_in6*)sock_addr)->sin6_addr);
    }
}