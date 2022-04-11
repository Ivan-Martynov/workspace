#include <stdio.h>

#include "printmode.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || \
    defined(__NT__) || defined(__CYGWIN__)
/*
 * Change text foreground
 */
void set_printmode_fg(const enum color_code foreground_color)
{
    printf("\e[3%dm", foreground_color);
}

/*
 * Change text background
 */
void set_printmode_bg(const enum color_code background_color)
{
    printf("\e[4%dm", background_color);
}

void set_printmode_fg_bg(
    const enum color_code foreground_color,
    const enum color_code background_color)
{
    set_printmode_fg(foreground_color);
    set_printmode_bg(background_color);
}

void reset_printmode(void)
{
    printf("\e[0m");
}
#else
/*
 * Change text foreground
 */
void set_printmode_fg(const enum color_code foreground_color)
{
    printf("\e[3%dm", foreground_color);
}

/*
 * Change text background
 */
void set_printmode_bg(const enum color_code background_color)
{
    printf("\e[4%dm", background_color);
}

void set_printmode_fg_bg(
    const enum color_code foreground_color,
    const enum color_code background_color)
{
    set_printmode_fg(foreground_color);
    set_printmode_bg(background_color);
}

void reset_printmode(void)
{
    printf("\e[0m");
}

#endif