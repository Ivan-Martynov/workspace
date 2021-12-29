#include <ncurses.h>

int main(int argc, char *argv[])
{
	/* initialize ncurses */
	initscr();
	start_color(); /* colours functionality */

	init_pair(1, COLOR_CYAN, COLOR_BLACK);
	printw("A big string which I did not care about to type till the very end of it...");

	/* first two parameters specify the position at which to start
	 * third parameter - number of character to update (-1 == till EOL)
	 * Fourth parameter is attribute
	 * Fifth attribute - colour index
	 * Sixth is always NULL
	 */
	mvchgat(0, 0, -1, A_BLINK, 1, NULL);

	refresh();
	getch();
	endwin();

	return 0;
}
