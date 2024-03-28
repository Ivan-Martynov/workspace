#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <setjmp.h>
#include "sighandler.h"

#define LEFT '{'
#define RIGHT '}'

static const char head[] = ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>| ";

static const char* skipspace(const char* s)
{
    while (s && isspace((const unsigned char)*s))
    {
        ++s;
    }

    return s;
}

enum state
{
    execution = 0,
    plus_L,
    plus_R,
    too_deep,
    eof_out,
    interrupted
};

typedef enum state state;

static const char* end_line(const char* s, jmp_buf jump_target)
{
    if (putchar('\n') == EOF)
    {
        longjmp(jump_target, eof_out);
    }

    return skipspace(s);
}

static volatile sig_atomic_t interrupt = 0;
static volatile unsigned deepest = 0;

static void signal_handler(int sig)
{
    sh_count(sig);
    switch (sig)
    {
        case SIGTERM:
            quick_exit(EXIT_FAILURE);

        case SIGABRT:
            _Exit(EXIT_FAILURE);

#ifdef SIGCONT
        case SIGCONT:
            return;
#endif

        default: // reset the handling to its default.
            signal(sig, SIG_DFL);
            interrupt = sig;
            return;
    }
}

static const char* descend1(const char act[static 1], unsigned dp,
    size_t len, char buffer[static len], jmp_buf jmp_target)
{
    if (dp + 2 > sizeof(head))
    {
        longjmp(jmp_target, too_deep);
    }

NEW_LINE_LABEL:     // Loops on output.
    while (!act[0]) // Loops for input.
    {
        if (interrupt)
        {
            longjmp(jmp_target, interrupted);
        }

        act = fgets(buffer, len, stdin);

        if (!act)
        {
            deepest = dp;
            longjmp(jmp_target, plus_L);
        }

        act = skipspace(act);
    }

    fputs(&head[sizeof(head) - (dp + 3)], stdout); // Header.

    for (; act && act[0]; ++act) // Remainder of the line.
    {
        switch (act[0])
        {
            case LEFT: // Descend on the left brace.
                act = end_line(act + 1, jmp_target);
                act = descend1(act, dp + 1, len, buffer, jmp_target);
                act = end_line(act + 1, jmp_target);
                goto NEW_LINE_LABEL;

            case RIGHT:
                return act;

            default:
                putchar(act[0]);
        }
    }

    goto NEW_LINE_LABEL;
}

static void descend0(size_t len, char buffer[static len], jmp_buf jump_target)
{
    const char* act = NULL;
NEW_LINE_LABEL:
    do
    {
        if (interrupt)
        {
            longjmp(jump_target, interrupted);
        }

        act = fgets(buffer, len, stdin);

        if (!act)
        {
            return;
        }

        act = skipspace(act);
    } while (!act[0]);

    // act is never NULL from here on.
    fputs(&head[sizeof(head) - 3], stdout);
    for (; act[0]; ++act)
    {
        switch (act[0])
        {
            case LEFT:
                act = end_line(act + 1, jump_target);
                act = descend1(act, 1, len, buffer, jump_target);
                act = end_line(act + 1, jump_target);
                goto NEW_LINE_LABEL;

            case RIGHT:
                longjmp(jump_target, plus_R);

            default:
                if (putchar(act[0]) == EOF)
                {
                    longjmp(jump_target, eof_out);
                }
        }
    }

    goto NEW_LINE_LABEL;
}

static const char* descend(const char* act, volatile unsigned dp, size_t len,
    char buffer[static len], jmp_buf jmp_target)
{
    if (dp + 3 > sizeof(head))
    {
        longjmp(jmp_target, too_deep);
    }

    ++dp;

NEW_LINE_LABEL:             // Loops on output.
    while (!act || !act[0]) // Loops for input.
    {
        if (interrupt)
        {
            longjmp(jmp_target, interrupted);
        }

        act = skipspace(fgets(buffer, len, stdin));

        if (!act) // End of the stream.
        {
            if (dp != 1)
            {
                longjmp(jmp_target, plus_L);
            }
            else
            {
                goto ASCENT_LABEL;
            }
        }
    }

    fputs(&head[sizeof(head) - (dp + 2)], stdout); // Header.

    for (; act && act[0]; ++act) // Remainder of the line.
    {
        switch (act[0])
        {
            case LEFT: // Descend on the left brace.
                act = end_line(act + 1, jmp_target);
                act = descend(act, dp, len, buffer, jmp_target);
                act = end_line(act + 1, jmp_target);
                goto NEW_LINE_LABEL;

            case RIGHT:
                if (dp == 1)
                {
                    longjmp(jmp_target, plus_R);
                }
                else
                {
                    goto ASCENT_LABEL;
                }

            default:
                putchar(act[0]);
        }
    }

    goto NEW_LINE_LABEL;

ASCENT_LABEL:
    --dp;
    return act;
}

enum
{
    maxline = 256
};

void basic_blocks(void)
{
    char buffer[maxline];
    char format[256];
    strcpy(format, "All %0.0d%c %c blocks have been closed correctly\n");

    jmp_buf jump_target;
    switch (setjmp(jump_target))
    {
        case 0:
            deepest = 0;
            if (1)
            {
                descend0(maxline, buffer, jump_target);
            }
            else
            {
                descend(0, deepest, maxline, buffer, jump_target);
            }
            break;

        case plus_L:
            // format = "Warning: %d %c %c blocks have not been closed
            // properly\n";
            strcpy(format,
                "Warning: %d %c %c blocks have not been closed properly\n");
            break;

        case plus_R:
            // format = "Error: closing too many (%d) %c %c blocks\n";
            strcpy(format, "Error: closing too many (%d) %c %c blocks\n");
            break;

        case too_deep:
            // format = "Error: nesting (%d) of %c %c blocks is too deep\n";
            strcpy(format, "Error: nesting (%d) of %c %c blocks is too deep\n");
            break;

        case eof_out:
            // format = "Error: EOF for stdout at nesting (%d) of %c %c
            // blocks\n";
            strcpy(format,
                "Error: EOF for stdout at nesting (%d) of %c %c blocks\n");
            break;

        case interrupted:
            // format = "Interrupted at level %d of %c %c block nesting\n";
            strcpy(format, "Interrupted at level %d of %c %c block nesting\n");
            break;

        default:
            // format = "Error: unknown error within (%d) %c %c blocks\n";
            strcpy(format, "Error: unknown error within (%d) %c %c blocks\n");
            break;
    }

    fflush(stdout);
    fprintf(stderr, format, deepest, LEFT, RIGHT);

#if 1
    if (interrupt)
    {
        SH_PRINT(stderr, interrupt, "Is somebody interrupting us?");
        raise(interrupt);
    }
#endif
}

static char** last_open = NULL;

void do_at_exit(void)
{
    if (last_open && last_open[0])
    {
        fprintf(stderr, "\n*********\nAbnormal exit, last open file was %s\n",
            last_open[0]);
    }
}

int main(const int argc, char* argv[static argc + 1])
{
    if (setvbuf(stdout, 0, _IOLBF, maxline + sizeof(head) + 2))
    {
        fputs("We could not establish line buffering for stdout, terminating.",
            stderr);
        return EXIT_FAILURE;
    }

    atexit(do_at_exit);
    at_quick_exit(do_at_exit);

    for (unsigned int i = 1; i < sh_known; ++i)
    {
        sh_enable(i, signal_handler);
    }

    last_open = argv;

    if (argc < 2)
    {
        goto RUN_LABEL;
    }

    for (++last_open; last_open[0]; ++last_open)
    {
        if (!freopen(last_open[0], "r", stdin))
        {
            perror(last_open[0]);
            return EXIT_FAILURE;
        }
        printf("++++++++++++ %s ++++++++++++\n", last_open[0]);
    RUN_LABEL:
        basic_blocks();
    }

    return 0;
}
