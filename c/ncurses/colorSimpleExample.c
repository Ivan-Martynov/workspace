#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *str);

int main(int argc, char *argv[])
{

	/* initialize ncurses */
	initscr();

	if (has_colors() == FALSE) {
		endwin();
		printf("Your terminal does not support color...\n");
		exit(1);
	}

	start_color(); /* start colour functionality */
	init_pair(1, COLOR_RED, COLOR_BLACK);

	attron(A_BOLD | COLOR_PAIR(1));
	print_in_middle(stdscr, LINES / 2, 0, 0, "Viola !!! In colour...");
	attroff(A_BOLD | COLOR_PAIR(1));

	getch();

	endwin();

	return 0;
}

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *str)
{
	int length, x, y;
	float temp;

	if (win == NULL)
		win = stdscr;
	getyx(win, y, x);
	if (startx != 0)
		x = startx;
	if (starty != 0)
		y = starty;
	if (width == 0)
		width = 80;

	length = strlen(str);
	temp = (width - length) / 2;
	x = startx + (int)temp;
	mvwprintw(win, y, x, "%s", str);
	refresh();
}
