#include "sighandler.h"

#include <errno.h>
#include <string.h>

#define SH_PAIR(X, D) [X] = {.name = #X, .desc = "" D ""}

const sh_pair sh_pairs[] = {
    // Execution errors.
    SH_PAIR(SIGFPE, "erroneous arithmetic operation"),
    SH_PAIR(SIGILL, "invalid instruction"),
    SH_PAIR(SIGSEGV, "invalid access to storage"),
#ifdef SIGBUS
    SH_PAIR(SIGBUS, "bad address"),
#endif
    // Job control.
    SH_PAIR(SIGABRT, "abnormal termination"),
    SH_PAIR(SIGINT, "interactive attention signal"),
    SH_PAIR(SIGTERM, "termination request"),
#ifdef SIGKILL
    SH_PAIR(SIGKILL, "kill signal"),
#endif
#ifdef SIGQUIT
    SH_PAIR(SIGQUIT, "keyboard quit"),
#endif
#ifdef SIGSTOP
    SH_PAIR(SIGSTOP, "stop process"),
#endif
#ifdef SIGCONT
    SH_PAIR(SIGCONT, "continue if stopped"),
#endif
#ifdef SIGINFO
    SH_PAIR(SIGINFO, "status information request"),
#endif
};

const size_t sh_known = sizeof(sh_pairs) / sizeof(sh_pairs[0]);

#if ATOMIC_LONG_LOCK_FREE > 1
_Atomic(unsigned long) sh_counts[sizeof(sh_pairs) / sizeof(sh_pairs[0])];
#define SH_COU " (%lu times),"
#else
#define SH_COU "%0.0lu,"
#endif

void sh_count(int);
unsigned long sh_counted(int);

#define SH_HEAD "\r%s:%zu: "
#define SH_DOC "\t%s,\t%s"

void sh_print(FILE* stream, int sig, const char* filename, size_t line,
    const char* str)
{
    const char* doc = ((sig < (int)sh_known) && sh_pairs[sig].name)
                          ? sh_pairs[sig].desc
                          : "unknown signal number";

    if (errno)
    {
        const char* err = strerror(errno);
        errno = 0;

        if (sig == 0)
        {
            fprintf(stream, SH_HEAD "\t%s:\t%s\n", filename, line, str, err);
        }
        else if ((sig < (int)sh_known) && sh_pairs[sig].name)
        {
            fprintf(stream, SH_HEAD "%s" SH_COU SH_DOC ":\t%s\n", filename,
                line, sh_pairs[sig].name, sh_counted(sig), doc, str, err);
        }
        else
        {
            fprintf(stream, SH_HEAD "#%d" SH_COU SH_DOC ":\t%s\n", filename,
                line, sig, sh_counted(sig), doc, str, err);
        }
    }
    else
    {
        if (sig == 0)
        {
            fprintf(stream, SH_HEAD "\t%s\n", filename, line, str);
        }
        else if ((sig < (int)sh_known) && sh_pairs[sig].name)
        {
            fprintf(stream, SH_HEAD "%s" SH_COU SH_DOC ":\t\n", filename, line,
                sh_pairs[sig].name, sh_counted(sig), doc, str);
        }
        else
        {
            fprintf(stream, SH_HEAD "#%d" SH_COU SH_DOC ":\t\n", filename, line,
                sig, sh_counted(sig), doc, str);
        }
    }
}

sh_handler* sh_enable(int sig, sh_handler* handler_ptr)
{
    sh_handler* result = signal(sig, handler_ptr);

    if (result == SIG_ERR)
    {
        SH_PRINT(stderr, sig, "failed");
    }
    else if (result == SIG_IGN)
    {
        SH_PRINT(stderr, sig, "previously ignored.");
    }
    else if (result && (result != SIG_DFL))
    {
        SH_PRINT(stderr, sig, "previously set otherwise.");
    }
    else
    {
        SH_PRINT(stderr, sig, "ok");
    }

    return result;
}
