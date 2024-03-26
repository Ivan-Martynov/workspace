#ifndef _H_SIGHANDLER_H_
#define _H_SIGHANDLER_H_

#include <stdio.h>
#include <signal.h>

#ifndef __STDC_NO_ATOMIC__
#include <stdatomic.h>
#endif

// A pair of strings to hold signal information.
typedef struct sh_pair sh_pair;
struct sh_pair
{
    const char* name;
    const char* desc;
};

extern const sh_pair sh_pairs[];

extern const size_t sh_known;

#if ATOMIC_LONG_LOCK_FREE > 1
// Keep track of the number of calls into a signal handler for each possible
// signal. Don't use this array directly.
extern _Atomic(unsigned long) sh_counts[];

inline void sh_count(int sig)
{
    if (sig < (int)sh_known)
    {
        ++sh_counts[sig];
    }
}

inline unsigned long sh_counted(int sig)
{
    return (sig < (int)sh_known) ? sh_counts[sig] : 0;
}

#else
inline void sh_count(int sig)
{
}

inline unsigned long sh_counted(int sig)
{
    return 0;
}
#endif

// Prototype for signal handlers.
typedef void sh_handler(int);

void sh_print(FILE* stream, int sig, const char* filename, size_t line,
    const char* str);

#define SH_PRINT(S, SIG, STR) sh_print(S, SIG, __FILE__, __LINE__, STR)

sh_handler* sh_enable(int sig, sh_handler* handler_ptr);

#endif // _H_SIGHANDLER_H_
