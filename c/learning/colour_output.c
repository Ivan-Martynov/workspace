#include <stdio.h>

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
void set_printmode_fg(const enum color_code foreground_color) {
    printf("\e[3%dm", foreground_color);
}

/*
* Change text background
*/
void set_printmode_bg(const enum color_code background_color) {
    printf("\e[4%dm", background_color);
}

void set_printmode_fg_bg(
    const enum color_code foreground_color,
    const enum color_code background_color)
{
    set_printmode_fg(foreground_color);
    set_printmode_bg(background_color);
}

void reset_printmode(void) {
    printf("\e[0m");
}

/*
* Testing color output
*/
void display_color_test() {
    set_printmode_fg_bg(RED_C, BLACK_C);
    printf("Red text on black background");
    reset_printmode();
    printf("\n");

    set_printmode_fg_bg(WHITE_C, BLUE_C);
    printf("White text on blue background");
    reset_printmode();
    printf("\n");

    set_printmode_fg_bg(BLACK_C, GREEN_C);
    printf("Black text on green background");
    reset_printmode();
    printf("\n");

    set_printmode_fg_bg(MAGENTA_C, YELLOW_C);
    printf("Magenta text on yellow background");
    reset_printmode();
    printf("\n");

    set_printmode_fg(BLUE_C);
    printf("Blue text");
    reset_printmode();
    printf("\n");

    set_printmode_fg(CYAN_C);
    printf("Cyan text");
    reset_printmode();
    printf("\n");

    printf("Back to defaults\n");
}

int main(int argc, const char* argv[static argc]) {
    display_color_test();

    return 0;
}