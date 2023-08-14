#ifndef TCP_UTILS_H
#define TCP_UTILS_H

#include "platform_specific.h"

#if WINDOWS_PLATFORM
#include <winsock2.h>
#else
// TODO: test on linux.
#include <netinet/in.h>
#endif

const void* get_in_addr(struct sockaddr* sock_addr);

#endif // TCP_UTILS_H
