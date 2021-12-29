#include <ncurses.h>
#include <string.h>

int main()
{
	char msg[] = "Simple text message";
	int row, col;

	// initialize ncurses
	initscr();
	getmaxyx(stdscr, row, col); // get the number of rows and columns
	mvprintw(row / 2, (col - strlen(msg)) / 2, "%s", msg);
	mvprintw(row - 2, 0, "This screen has %d rows and %d columns\n", row, col);

	printw("Resize your window and run again\n");
	
	refresh();
	getch();
	endwin();

	return 0;
}
