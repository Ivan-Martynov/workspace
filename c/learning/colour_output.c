#include <stdio.h>

/*
* Testing color output
*/
void display_color_test() {
    // \e or \033 (oct) or \x1b (hex) is the escape character
    printf("\e[31mRed text on default background\n\e[0m");
    printf("\e[32mGreen text on default background\n\e[0m");
    printf("\e[33mYellow text on default background\n\e[0m");
    printf("\e[34mBlue text on default background\n\e[0m");
    printf("\e[35mPurple text on default background\n\e[0m");
    printf("\e[36mCyan text on default background\n\e[0m");
    printf("\e[37;44mWhite text on blue background\n\e[0m");
    printf("\e[0;30mBlack text on default background\n\e[0m");

    printf("\e[0mback to defaults\n");
}

int main(int argc, const char* argv[static argc]) {
    display_color_test();

    return 0;
}