#define _POSIX_C_SOURCE 200112L
#define _GNU_SOURCE

#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "error_handler.h"
#include "platform_specific.h"

#if WINDOWS_PLATFORM
    #include <ws2tcpip.h>
#else
    #include <netdb.h>
#endif

char* getaddrinfo_error_code_to_string(const int error_code)
{
    char* result;
    switch (error_code)
    {
        case EAI_AGAIN:
            result = "Temporary failure in name resolution";
            break;

        case EAI_FAMILY:
            result = "Address family for hostname is not supported";
            break;

        case EAI_BADFLAGS:
            result = "Bad value for ai_flags";
            break;

        case EAI_FAIL:
            result = "Non-recoverable faliure in name resolution";
            break;

        case EAI_MEMORY:
            result = "Memory allocation failure";
            break;

        case EAI_NODATA:
            result = "No address associated with hostname";
            break;

        case EAI_NONAME:
            result = "Name or service not known";
            break;

        case EAI_SERVICE:
            result = "Servname not supported for ai_socktype";
            break;

        case EAI_SOCKTYPE:
            result = "ai_socktype not supported";
            break;

        default:
            result = "Uknown error";
            break;
    }

    return result;
}

int report_error(const char* const message)
{
    const int n = fprintf(stderr, message, ": %s\n", strerror(errno));

    // Reset the errno value.
    errno = 0;

    return n;
}
