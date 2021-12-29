#include <ncurses.h>

int main()
{
	int ch;

	// initialize ncurses
	initscr();
	raw(); // line buffering disabled
	keypad(stdscr, TRUE); // get F1, F2 etc.
	noecho(); // don't echo() while getch()

	printw("Type and character to see it in bold\n");

	ch = getch();
	if (ch == KEY_F(1)) {
		printw("F1 key pressed\n");
	} else {
		printw("The pressed key is ");
		attron(A_BOLD);
		printw("%c\n", ch);
		attroff(A_BOLD);
	}
	
	refresh();
	getch();
	endwin();

	return 0;
}
