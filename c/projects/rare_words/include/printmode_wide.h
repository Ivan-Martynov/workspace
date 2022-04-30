#ifndef PRINT_MODE_WIDE_INCLUDED
#define PRINT_MODE_WIDE_INCLUDED

enum color_code
{
    BLACK_C,
    RED_C,
    GREEN_C,
    YELLOW_C,
    BLUE_C,
    MAGENTA_C,
    CYAN_C,
    WHITE_C,
};

/*
* Change text foreground
*/
void set_printmode_fg(const enum color_code foreground_color);
void set_printmode_fgh(const enum color_code foreground_color);

/*
* Change text background
*/
void set_printmode_bg(const enum color_code background_color);
void set_printmode_bgh(const enum color_code background_color);

void set_printmode_fg_bg(enum color_code foreground_color,
                         enum color_code background_color);
void set_printmode_fgh_bgh(enum color_code foreground_color,
                         enum color_code background_color);

/*
 * Reset to defaults
 */
void reset_printmode(void);

#endif // PRINT_MODE_WIDE_INCLUDED
