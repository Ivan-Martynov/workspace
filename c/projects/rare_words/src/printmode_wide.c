#include <wchar.h>

#include "printmode_wide.h"

/*
 * Change text foreground
 */
void set_printmode_fg(const enum color_code foreground_color)
{
    wprintf(L"\033[3%dm", foreground_color);
}

/*
 * Change text foreground, high intensity
 */
void set_printmode_fgh(const enum color_code foreground_color)
{
    wprintf(L"\033[9%dm", foreground_color);
}

/*
 * Change text background
 */
void set_printmode_bg(const enum color_code background_color)
{
    wprintf(L"\033[4%dm", background_color);
}

/*
 * Change text background, high intensity
 */
void set_printmode_bgh(const enum color_code background_color)
{
    wprintf(L"\033[10%dm", background_color);
}

void set_printmode_fg_bg(
    const enum color_code foreground_color,
    const enum color_code background_color)
{
    set_printmode_fg(foreground_color);
    set_printmode_bg(background_color);
}

void set_printmode_fgh_bgh(
    const enum color_code foreground_color,
    const enum color_code background_color)
{
    set_printmode_fgh(foreground_color);
    set_printmode_bgh(background_color);
}

void reset_printmode(void)
{
    //wprintf(L"\033[39;49m");
    wprintf(L"\033[0m");
}
