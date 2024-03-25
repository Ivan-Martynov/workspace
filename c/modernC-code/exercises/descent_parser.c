#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <setjmp.h>
#include <signal.h>

#define LEFT '{'
#define RIGHT '}'

static const char head[] = ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>| ";

static const char* skipspace(const char* s)
{
    while (s && isspace(*s))
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

static volatile sig_atomic_t interrupt = 0;

static const char* descent(const char* act, volatile unsigned dp, size_t len,
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
                act = descent(act, dp, len, buffer, jmp_target);
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
    volatile unsigned depth = 0;
    const char* format = "All %0.0d%c %c blocks have been closed correctly\n";

    jmp_buf jump_target;
    switch (setjmp(jump_target))
    {
        case 0:
            descent(0, depth, maxline, buffer, jump_target);
            break;

        case plus_L:
            format = "Warning: %d %c %c blocks have not been closed properly\n";
            break;

        case plus_R:
            format = "Error: closing too many (%d) %c %c blocks\n";
            break;

        case too_deep:
            format = "Error: nesting (%d) of %c %c blocks is too deep\n";
            break;

        case eof_out:
            format = "Error: EOF for stdout at nesting (%d) of %c %c blocks\n";
            break;

        case interrupted:
            format = "Interrupted at level %d of %c %c block nesting\n";
            break;

        default:
            format = "Error: unknown error within (%d) %c %c blocks\n";
            break;
    }

    fflush(stdout);
    fprintf(stderr, format, depth, LEFT, RIGHT);

#if 0
    if (interrupt)
    {
        SH_PRINT(stderr, interrupt, "Is somebody interrupting us?");
        raise(interrupt);
    }
#endif
}

int main(void)
{
    return 0;
}
