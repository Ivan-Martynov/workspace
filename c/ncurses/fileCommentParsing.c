#include <ncurses.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int ch, prev, row, col, y, x;
	FILE *fp;
	prev = EOF;

	if (argc != 2) {
		printf("Usage: %s <a .c file name>\n", argv[0]);
		exit(1);
	}

	fp = fopen(argv[1], "r");
	if (fp == NULL) {
		perror("Cannot open input file");
		exit(1);
	}

	/* initialize ncurses */
	initscr();
	getmaxyx(stdscr, row, col); /* get the number of rows and columns */

	/* read the file till the end */
	while ((ch = fgetc(fp)) != EOF) {
		getyx(stdscr, y, x); /* get the current cursor position */
		if (y == (row - 1)) { /* if we are at the end of the screen */
			printw("<-Press any key->");
			getch();
			clear(); /* clear the screen */
			move(0, 0); /* start at the beginning of the screen */
		}

		if (prev == '/' && ch == '*') {
			attron(A_UNDERLINE | A_BOLD | A_BLINK); /* turn bold on */
			getyx(stdscr, y, x) ;
			move(y, x - 1); /* back up one space */
			printw("%c%c", '/', ch);
		} else {
			printw("%c", ch);
		}
		refresh();
		if (prev == '*' && ch == '/') {
			attroff(A_UNDERLINE | A_BOLD | A_BLINK); /* turn bold on */
		}
		prev = ch;
	}
	printw("<-Press any key->");
	getch();
	endwin();
	fclose(fp);

	return 0;
}
