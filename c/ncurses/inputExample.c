#include <ncurses.h>
#include <string.h>

int main()
{
	char msg[] = "Enter a string: ";
	char str[80];
	int row, col;

	// initialize ncurses
	initscr();
	getmaxyx(stdscr, row, col); // get the number of rows and columns
	mvprintw(row / 2, (col - strlen(msg)) / 2, "%s", msg);
	getstr(str);
	mvprintw(LINES - 2, 0, "You've entered: %s", str);
//	refresh();
	getch();
	endwin();

	return 0;
}
