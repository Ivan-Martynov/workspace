#ifndef PLATFORM_SPECIFIC_H
#define PLATFORM_SPECIFIC_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)\
    || defined(__CYGWIN__)
#define WINDOWS_PLATFORM 1
#else
#define WINDOWS_PLATFORM 0
#endif

#endif // PLATFORM_SPECIFIC_H
