#ifndef PLATFORM_SPECIFIC_H
#define PLATFORM_SPECIFIC_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)                    \
    || defined(__CYGWIN__)
#define WINDOWS_PLATFORM 1

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#else
#define WINDOWS_PLATFORM 0
#endif

#endif // PLATFORM_SPECIFIC_H
