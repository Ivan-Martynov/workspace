#include <stdio.h>
#include <setjmp.h>

static const char* descent(const char* act,
    unsigned dp[restrict static 1], // Bad.
    size_t len, char buffer[static len], jmp_buf jmp_target)
{
    if (dp[0] + 3 > sizeof(head))
    {
        longjmp(jmp_target, too_deep);
    }

    ++dp[0];

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
            if (dp[0] != 1)
            {
                longjmp(jmp_target, plus_L);
            }
            else
            {
                goto ASCENT_LABEL;
            }
        }
    }

    fputs(&head[sizeof(head) - (dp[0] + 2)], stdout); // Header.

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
                if (dp[0] == 1)
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
    --dp[0];
    return act;
}

int main(void)
{
    return 0;
}
