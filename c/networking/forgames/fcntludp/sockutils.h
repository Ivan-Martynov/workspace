#include "common.h"

#if PLATFORM == PLATFORM_WINDOWS
#include <winsock2.h>
typedef int socklen_t;
#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#endif

#define MSGSIZE 1024

// initialize socket layer, required on Windows
// On Unix and Mac does not do anything
bool init_sockets();

// shutdwon socket layer, required on Windows
void shutdown_sockets();

// close socket
void close_socket(int);
